#include "Linux_Detour.h"

#include <algorithm>
#include <list>

#include <cstdint>

#include <unistd.h>
#include <malloc.h>
#include <sys/mman.h>
#include <errno.h>

//------------------------------------------------------------------------------//
// Helper funcs
//------------------------------------------------------------------------------//
constexpr static auto relative_addr_jump_size = sizeof(int32_t);
constexpr static auto relative_jump_size = 1 + relative_addr_jump_size;
constexpr static auto absolute_jump_size = 6;

struct
{
    bool has_r_m;
    uint8_t base_size;
} s_opcodes[256] =
{
    {false, 0}, {false, 0}, {false, 0}, {false, 0}, {false, 0}, {false, 0}, {false, 0}, {false, 0}, // 0   - 7
    {false, 0}, {false, 0}, {false, 0}, {false, 0}, {false, 0}, {false, 0}, {false, 0}, {false, 0}, // 8   - 15
    {false, 0}, {false, 0}, {false, 0}, {false, 0}, {false, 0}, {false, 0}, {false, 0}, {false, 0}, // 16  - 23
    {false, 0}, {false, 0}, {false, 0}, {false, 0}, {false, 0}, {false, 0}, {false, 0}, {false, 0}, // 24  - 31
    {false, 0}, {false, 0}, {false, 0}, {false, 0}, {false, 0}, {false, 0}, {false, 0}, {false, 0}, // 32  - 39
    {false, 0}, {false, 0}, {false, 0}, {false, 0}, {false, 0}, {false, 0}, {false, 0}, {false, 0}, // 40  - 47
    {false, 0}, {false, 0}, {false, 0}, {false, 0}, {false, 0}, {false, 0}, {false, 0}, {false, 0}, // 48  - 55
    {false, 0}, {false, 0}, {false, 0}, {false, 0}, {false, 0}, {false, 0}, {false, 0}, {false, 0}, // 56  - 63
    {false, 0}, {false, 0}, {false, 0}, {false, 0}, {false, 0}, {false, 0}, {false, 0}, {false, 0}, // 64  - 71
    {false, 0}, {false, 0}, {false, 0}, {false, 0}, {false, 0}, {false, 0}, {false, 0}, {false, 0}, // 72  - 79
    // PUSH ...
    {false, 1}, {false, 1}, {false, 1}, {false, 1}, {false, 1}, {false, 1}, {false, 1}, {false, 1}, // 80  - 87
    // POP ...
    {false, 1}, {false, 1}, {false, 1}, {false, 1}, {false, 1}, {false, 1}, {false, 1}, {false, 1}, // 88  - 95
    {false, 0}, {false, 0}, {false, 0}, {false, 0}, {false, 0}, {false, 0}, {false, 0}, {false, 0}, // 96  - 103
    {false, 0}, {false, 0}, {false, 0}, {false, 0}, {false, 0}, {false, 0}, {false, 0}, {false, 0}, // 104 - 111
    {false, 0}, {false, 0}, {false, 0}, {false, 0}, {false, 0}, {false, 0}, {false, 0}, {false, 0}, // 112 - 129
    {false, 0}, {false, 0}, {false, 0}, {false, 0}, {false, 0}, {false, 0}, {false, 0}, {false, 0}, // 120 - 127
    // MOV, ADD, ... R8 <- IMM8                     TEST_8      TEST        XCHG_8      XCHG
    {true , 3}, {true , 6}, {true , 3}, {true , 3}, {true , 2}, {true , 2}, {true , 2}, {true , 2}, // 128 - 135
    // MOV_8    MOV         MOV_R8_B    MOV_R32_D   MOV_32_ES   LEA         MOV_ES_32   POP
    {true , 2}, {true , 2}, {true , 2}, {true , 2}, {true , 2}, {true , 2}, {true , 2}, {false, 2}, // 136 - 143
    // NOP
    {false, 1}, {false, 0}, {false, 0}, {false, 0}, {false, 0}, {false, 0}, {false, 0}, {false, 0}, // 144 - 151
    {false, 0}, {false, 0}, {false, 0}, {false, 0}, {false, 0}, {false, 0}, {false, 0}, {false, 0}, // 152 - 159
    {false, 0}, {false, 0}, {false, 0}, {false, 0}, {false, 0}, {false, 0}, {false, 0}, {false, 0}, // 160 - 167
    {false, 0}, {false, 0}, {false, 0}, {false, 0}, {false, 0}, {false, 0}, {false, 0}, {false, 0}, // 168 - 175
    // MOV_AL   MOV_CL      MOV_DL      MOV_BL      MOV_AH      MOV_CH      MOV_DH      MOV_BH
    {false, 2}, {false, 2}, {false, 2}, {false, 2}, {false, 2}, {false, 2}, {false, 2}, {false, 2}, // 176 - 183
    // MOV_EAX  MOV_ECX     MOV_EDX     MOV_EBX     MOV_ESP     MOV_EBP     MOV_ESI     MOV_EDI,
    {false, 5}, {false, 5}, {false, 5}, {false, 5}, {false, 5}, {false, 5}, {false, 5}, {false, 5}, // 184 - 191
    //                      RETN_IMM16  RETN
    {false, 0}, {false, 0}, {false, 3}, {false, 1}, {false, 0}, {false, 0}, {false, 0}, {false, 0}, // 192 - 199
    //          LEAVE       RETF_IMM16  RETF        INT         INT_IMM8    INTO
    {false, 0}, {false, 1}, {false, 3}, {false, 1}, {false, 1}, {false, 1}, {false, 1}, {false, 0}, // 200 - 207
    {false, 0}, {false, 0}, {false, 0}, {false, 0}, {false, 0}, {false, 0}, {false, 0}, {false, 0}, // 208 - 215
    {false, 0}, {false, 0}, {false, 0}, {false, 0}, {false, 0}, {false, 0}, {false, 0}, {false, 0}, // 216 - 223
    {false, 0}, {false, 0}, {false, 0}, {false, 0}, {false, 0}, {false, 0}, {false, 0}, {false, 0}, // 224 - 231
    // CALL     JMP         LJMP        SHORT_JMP
    {false, 0}, {false, 0}, {false, 0}, {false, 0}, {false, 0}, {false, 0}, {false, 0}, {false, 0}, // 232 - 239
    {false, 0}, {false, 0}, {false, 0}, {false, 0}, {false, 0}, {false, 0}, {false, 0}, {false, 0}, // 240 - 247
    //                                                                                  EXTENDED
    {false, 0}, {false, 0}, {false, 0}, {false, 0}, {false, 0}, {false, 0}, {false, 0}, {false, 6}, // 248 - 255
};

static constexpr auto mod_mask = 0xC0;
static constexpr auto register_addressing_mode = 0xC0;
static constexpr auto four_bytes_signed_displacement = 0x80;
static constexpr auto one_byte_signed_displacement = 0x40;

static constexpr auto rm_mask = 0x05;
static constexpr auto displacement_only_addressing = 0x05;
static constexpr auto sib_with_no_displacement = 0x04;
static constexpr auto register_indirect_addressing_mode = 0x00;

enum opcodes_e // Commonly used opcode in the beginning of functions
{
    PUSH_EAX = 0x50, PUSH_ECX, PUSH_EDX, PUSH_EBX, PUSH_ESP, PUSH_EBP, PUSH_ESI, PUSH_EDI,
    POP_EAX        , POP_ECX , POP_EDX , POP_EBX , POP_ESP , POP_EBP , POP_ESI , POP_EDI ,

    R8_IMM8 = 0x80, R32_IMM32, R8_IMM8_2, R32_IMM8,
    TEST_8, TEST,
    XCHG_8, XCHG,
    MOV_8 , MOV,
    MOV_8_B, MOV_32_D, MOV_32_ES,
    LEA,
    MOV_ES_32, POP,
    NOP,
    MOV_AL = 0xB0, MOV_CL, MOV_DL, MOV_BL, MOV_AH, MOV_CH, MOV_DH, MOV_BH,
    MOV_EAX, MOV_ECX, MOV_EDX, MOV_EBX, MOV_ESP, MOV_EBP, MOV_ESI, MOV_EDI,
    RETN_IMM16 = 0xC2, RETN,
    LEAVE = 0xC9, RETF_IMM16, RETF, INT, INT_IMM8, INTO,
    CALL = 0xE8, // 5 bytes don't process
    JMP, // 5 bytes don't process
    LJMP, // 7 bytes, don't process
    SHORT_JMP, // 2 bytes don't process
    EXTENDED    = 0xFF,
};

const char* opcode_name(uint8_t opcode)
{
#define NAME(opcode_name) case opcode_name: return #opcode_name
    switch( opcode )
    {
        NAME(PUSH_EAX); NAME(PUSH_ECX); NAME(PUSH_EDX); NAME(PUSH_EBX);
        NAME(PUSH_ESP); NAME(PUSH_EBP); NAME(PUSH_ESI); NAME(PUSH_EDI);
        NAME(POP_EAX); NAME(POP_ECX); NAME(POP_EDX); NAME(POP_EBX);
        NAME(POP_ESP); NAME(POP_EBP); NAME(POP_ESI); NAME(POP_EDI);
        NAME(R8_IMM8); NAME(R32_IMM32); NAME(R8_IMM8_2); NAME(R32_IMM8);
        NAME(TEST_8); NAME(TEST); NAME(XCHG_8); NAME(XCHG); NAME(MOV_8); NAME(MOV);
        NAME(MOV_8_B); NAME(MOV_32_D); NAME(MOV_32_ES);
        NAME(LEA);
        NAME(MOV_ES_32); NAME(POP);
        NAME(NOP);
        NAME(MOV_AL) ; NAME(MOV_CL) ; NAME(MOV_DL) ; NAME(MOV_BL) ; NAME(MOV_AH) ; NAME(MOV_CH) ; NAME(MOV_DH) ; NAME(MOV_BH) ;
        NAME(MOV_EAX); NAME(MOV_ECX); NAME(MOV_EDX); NAME(MOV_EBX); NAME(MOV_ESP); NAME(MOV_EBP); NAME(MOV_ESI); NAME(MOV_EDI);
        NAME(RETN_IMM16); NAME(RETN); NAME(LEAVE); NAME(RETF_IMM16); NAME(RETF);
        NAME(INT); NAME(INT_IMM8); NAME(INTO); NAME(CALL); NAME(JMP); NAME(LJMP); NAME(SHORT_JMP);
        NAME(EXTENDED);
    }
#undef NAME
    return "no registered";
}

#pragma pack(push, 1)

struct trampoline_x86_t
{
    uint8_t trampolineBytes[16+relative_jump_size]; // trampoline + original function opcodes
    uint8_t hookJump[relative_jump_size]; // jump to hook addr, needed because of relative jump overflow
    uint8_t nOriginalBytes;               // number of original function bytes bkp
};

typedef trampoline_x86_t trampoline_t;

struct trampoline_region_t
{
    uint8_t numTrampolines; // current trampolines allocated
    trampoline_t *trampolines_start; // start pointer of current region trampolines
    trampoline_t *next_free_trampoline; // next free trampoline in region
};

#pragma pack(pop)

struct transaction_t
{
    bool detach;
    void** ppOriginalFunc;
    trampoline_t *trampoline;
};

static std::list<trampoline_region_t> trampoline_regions;

static bool transaction_started = false;
static std::list<transaction_t> cur_transaction;

inline size_t page_size()
{
    static size_t _page_size = sysconf(_SC_PAGESIZE);
    return _page_size;
}

inline size_t region_size()
{
    return page_size();
}

static uint8_t max_trampolines_in_region = region_size() / sizeof(trampoline_t);

inline void* library_address_by_handle(void *library)
{
    return (library == nullptr ? nullptr : *reinterpret_cast<void**>(library));
}

inline size_t page_align(size_t size, size_t page_size)
{
    return (size+(page_size-1)) & (((size_t)-1)^(page_size-1));
}

inline void* page_addr(void* addr, size_t page_size)
{
    return reinterpret_cast<void*>(reinterpret_cast<size_t>(addr) & (((size_t)-1)^(page_size-1)));
}

////////////////////////////////////////////////////
/// Tiny disasm

bool is_opcode_terminating_function(uint8_t* pCode)
{
    switch( *pCode )
    {
        case LEAVE:
        case RETN: case RETN_IMM16:
        case RETF: case RETF_IMM16:
        case INT: case INT_IMM8: case INTO:
            return true;
    }
    return false;
}

int find_space_for_trampoline(uint8_t* func, int bytes_needed)
{
    if( func == nullptr )
        return -1;

    int code_len = 0;
    bool search = true;
    uint8_t *startCode = reinterpret_cast<uint8_t*>(func);
    uint8_t *pCode = startCode;
    while( search ) // Find opcodes size and try to find at least 5 bytes for our JMP
    {
        if( is_opcode_terminating_function(pCode) )
            break;

        if( s_opcodes[*pCode].has_r_m )
        { // MOD-REG-R/M Byte
          //  7 6    5 4 3    2 1 0 - bits
          //[ MOD ][  REG  ][  R/M  ]
            switch( pCode[1] & mod_mask ) // Check MOD to know how many bytes we have after this opcode
            {
                case register_addressing_mode      : pCode += s_opcodes[*pCode].base_size  ; break;// register addressing mode [opcode] [R/M] [XX]
                case four_bytes_signed_displacement: pCode += s_opcodes[*pCode].base_size+5; break;// address mode byte + 4 bytes displacement
                case one_byte_signed_displacement  : pCode += s_opcodes[*pCode].base_size+2; break;// address mode byte + 1 byte displacement
                case 0x00:
                    switch( pCode[1] & rm_mask )
                    {
                        case sib_with_no_displacement         : pCode += s_opcodes[*pCode].base_size+1; break;// SIB with no displacement
                        case displacement_only_addressing     : pCode += s_opcodes[*pCode].base_size+4; break;// 4 bytes Displacement only addressing mode
                        case register_indirect_addressing_mode: pCode += s_opcodes[*pCode].base_size;         // Register indirect addressing mode
                    }
            }
        }
        else if( s_opcodes[*pCode].base_size )
        {
            pCode += s_opcodes[*pCode].base_size;
        }
        else
        {
            switch( *pCode )
            {
                case CALL: case JMP: case LJMP: case SHORT_JMP:
                    //std::cerr << "CALL and JMP are not supported for trampolines." << std::endl;
                    search = false;
                    break;

                case EXTENDED:
                    //std::cerr << "IMPORT_JUMP is not handled" << std::endl;
                    if (pCode[1] == 0x25) // This is an imported function
                    { // Get the true function call
                        //pCode = (uint8_t*)*(pCode+2);
                        //startCode = pCode;
                        // For now disable this case
                        search = false;
                    }
                    else
                    {
                        search = false;
                    }
                    break;

                default:
                    //std::cerr << "opcode " << std::hex << (uint32_t)*pCode << " no registered" << std::endl;
                    search = false;
            }
        }

        if( (pCode - startCode) >= bytes_needed && search )
        {
            search = false;
            code_len = pCode-startCode;
        }
    }
    return code_len;
}

///////////////////////////////////////////
// Tiny asm

inline uint8_t* gen_immediate_addr(uint8_t* opcode_addr, uint8_t* dest)
{
    *reinterpret_cast<int32_t*>(opcode_addr) = (dest - (opcode_addr + relative_addr_jump_size));
    return opcode_addr + relative_addr_jump_size;
}

inline uint8_t* gen_immediate_jump(uint8_t* opcode_addr, uint8_t* dest)
{
    *opcode_addr++ = JMP;
    return gen_immediate_addr(opcode_addr, dest);
}

inline uint8_t* gen_immediate_call(uint8_t* opcode_addr, uint8_t* dest)
{
    *opcode_addr++ = CALL;
    return gen_immediate_addr(opcode_addr, dest);
}

uint8_t* relative_addr_to_absolute(int32_t rel_addr, uint8_t *code_addr)
{
    return code_addr + rel_addr + relative_jump_size;
}

void alloc_new_trampoline_region()
{
    trampoline_region_t region;

    region.numTrampolines = 0;
    // allocate new trampoline right in the middle of memory so relative jump can access any function
    region.trampolines_start = reinterpret_cast<trampoline_t*>(mmap((void*)std::numeric_limits<int32_t>::max(), // allocate the page near the half of memory addressing
                                                                    region_size(), // size
                                                                    PROT_EXEC|PROT_WRITE|PROT_READ, // protection
                                                                    MAP_PRIVATE|MAP_32BIT|MAP_ANONYMOUS, // don't map a file but memory
                                                                    -1, // fd = -1
                                                                    0) // offset
                                                               );
    // Fill the region with 0
    std::fill(reinterpret_cast<uint8_t*>(region.trampolines_start), reinterpret_cast<uint8_t*>(region.trampolines_start)+region_size(), 0);
    region.next_free_trampoline = region.trampolines_start;
    // Protect trampoline region memory
    mprotect((void*)region.trampolines_start, region_size(), PROT_READ|PROT_EXEC);

    trampoline_regions.push_back(region);
}

trampoline_t* get_free_trampoline()
{
    if (!transaction_started)
        return nullptr;

    trampoline_t *res = nullptr;
    auto it = std::find_if(trampoline_regions.begin(), trampoline_regions.end(), [&res](trampoline_region_t &region){
        if( region.numTrampolines == max_trampolines_in_region )
            return false;
        return true;
    });

    if( it == trampoline_regions.end() )
    {
        alloc_new_trampoline_region();
        it = --trampoline_regions.end();
    }
    res = it->next_free_trampoline;

    trampoline_t *next_new_trampoline = res+1;
    if( it->numTrampolines != max_trampolines_in_region )
    {
        while( next_new_trampoline->nOriginalBytes != 0 )
        {
            ++next_new_trampoline;
        }
    }
    else
    {
        next_new_trampoline = nullptr;
    }
    it->next_free_trampoline = next_new_trampoline;

    ++it->numTrampolines;

    return res;
}

void clear_trampoline(trampoline_region_t& region, trampoline_t *trampoline)
{
    --region.numTrampolines;

    std::fill(reinterpret_cast<uint8_t*>(trampoline), reinterpret_cast<uint8_t*>(trampoline+1), 0);
    if( region.next_free_trampoline == nullptr || region.next_free_trampoline > trampoline )
        region.next_free_trampoline = trampoline;
}

inline bool is_page_inside_region(void *page, trampoline_region_t& region)
{
    if( page >= region.trampolines_start && page <= (region.trampolines_start+region_size()) )
        return true;
    return false;
}

//------------------------------------------------------------------------------//

/*
#include <signal.h>
#include <dirent.h>

static pid_t tid = 0;

struct dism_pthread_t
{
    uint32_t d0;
    uint32_t d4;
    uint32_t d8;
    uint32_t dc;
    uint32_t d10;
    uint32_t d14;
    uint32_t d18;
    uint32_t d1c;
    uint32_t d20;
    uint32_t d24;
    uint32_t d28;
    uint32_t d2c;
    uint32_t d30;
    uint32_t d34;
    uint32_t d38;
    uint32_t d3c;
    uint32_t d40;
    uint32_t d44;
    uint32_t d48;
    uint32_t d4c;
    uint32_t d50;
    uint32_t d54;
    uint32_t d58;
    uint32_t d5c;
    uint32_t d60;
    uint32_t d64;
    pid_t task_id;
};
*/

int Linux_Detour::update_thread(pthread_t thread_id)
{
    //dism_pthread_t *dt = (dism_pthread_t*)thread_id;

    // dt->task_id == syscall(SYS_gettid);


    return 0;
}

int Linux_Detour::transaction_begin()
{
    if( transaction_started )
        return -1;

    transaction_started = true;

    return 0;
}

int Linux_Detour::transaction_abort()
{
    if(!transaction_started)
        return -1;

    for( auto &i : cur_transaction )
    {
        trampoline_t *trampoline = i.trampoline;
        void *page_start = page_addr(reinterpret_cast<void*>(trampoline), page_size());
        auto it = std::find_if(trampoline_regions.begin(), trampoline_regions.end(), [page_start](trampoline_region_t &region){
            if( is_page_inside_region(page_start, region) )
                return true;
            return false;
        });
        if( it != trampoline_regions.end() )
        {
            clear_trampoline(*it, trampoline);
        }
    }
    cur_transaction.clear();
    transaction_started = false;

    return 0;
}

int Linux_Detour::transaction_commit()
{
    if (!transaction_started )
        return -1;

    for( auto &i : cur_transaction)
    {
        trampoline_t *trampoline = i.trampoline;
        void **ppOriginalFunc = i.ppOriginalFunc;

        int res;

        if( i.detach )
        {
            void* trampoline_page = page_addr(reinterpret_cast<void*>(trampoline), page_size());

            *ppOriginalFunc = (void*)(relative_addr_to_absolute(*reinterpret_cast<int32_t*>(trampoline->trampolineBytes+trampoline->nOriginalBytes+1),
                                                                trampoline->trampolineBytes));

            void* originalFunctionPage = page_addr(*ppOriginalFunc, page_size());

            // Allow write on the original func
            res = mprotect(originalFunctionPage, page_size()*2, PROT_READ|PROT_WRITE|PROT_EXEC);

            // Write the original opcodes
            std::copy(trampoline->trampolineBytes, trampoline->trampolineBytes+trampoline->nOriginalBytes,
                      reinterpret_cast<uint8_t*>(*ppOriginalFunc));

            // Remove write permission
            res = mprotect(originalFunctionPage, page_size()*2, PROT_READ|PROT_EXEC);

            // Allow write on trampoline page
            res = mprotect(trampoline_page, page_size()*2, PROT_READ|PROT_WRITE|PROT_EXEC);

            clear_trampoline(*reinterpret_cast<trampoline_region_t*>(trampoline_page), trampoline);

            // Remove write permission
            res = mprotect(trampoline_page, page_size()*2, PROT_READ|PROT_EXEC);
        }
        else
        {
            void* originalFunctionPage = page_addr(*ppOriginalFunc, page_size());

            // Allow write on the original func
            res = mprotect(originalFunctionPage, page_size()*2, PROT_READ|PROT_WRITE|PROT_EXEC);

            // Write the jump to trampoline
            gen_immediate_jump(reinterpret_cast<uint8_t*>(*ppOriginalFunc), trampoline->hookJump);

            // Remove write permission
            res = mprotect(originalFunctionPage, page_size()*2, PROT_READ|PROT_EXEC);

            *ppOriginalFunc = (void*)(trampoline->trampolineBytes);
        }
    }
    cur_transaction.clear();
    transaction_started = false;

    return 0;
}

int Linux_Detour::unhook_func(void** ppOriginalFunc, void* _hook)
{
    if( !transaction_started )
        return -EPERM;

        if( ppOriginalFunc == nullptr || _hook == nullptr || *ppOriginalFunc == nullptr )
        return -EINVAL;

    trampoline_t *trampoline = reinterpret_cast<trampoline_t*>(*ppOriginalFunc);
    void *page_start = page_addr(reinterpret_cast<void*>(trampoline), page_size());
    auto it = std::find_if(trampoline_regions.begin(), trampoline_regions.end(), [page_start](trampoline_region_t &region){
        if( is_page_inside_region(page_start, region) )
            return true;
        return false;
    });
    if( it != trampoline_regions.end() )
    {
        cur_transaction.push_back({true, ppOriginalFunc, trampoline});
    }

    return -EINVAL;
}

int Linux_Detour::hook_func(void** ppOriginalFunc, void* _hook)
{
    if( !transaction_started )
        return -EPERM;

    if( ppOriginalFunc == nullptr || _hook == nullptr || *ppOriginalFunc == nullptr )
        return -EINVAL;

    uint8_t* hook = reinterpret_cast<uint8_t*>(_hook);
    uint8_t* pOriginalFunc = reinterpret_cast<uint8_t*>(*ppOriginalFunc);
    int code_len = find_space_for_trampoline(pOriginalFunc, relative_jump_size);

    if( code_len < relative_jump_size )
        return -ENOSPC;

    // Allocate the trampoline, try to put it right in the middle of the mem, so a relative jump can access any function in the app (+/-2GB)
    //
    // Our hook is a 5 bytes JMP (1 bytes for opcode, 4 for RELATIVE jump)
    // /!\ TODO: Add checks on JMP overflow

    trampoline_t *trampoline = get_free_trampoline();
    uint8_t *pTrampolineCode = trampoline->trampolineBytes;

    void* trampoline_page = page_addr(reinterpret_cast<void*>(trampoline), page_size());

    // Enable write to the trampoline region
    mprotect(trampoline_page, page_size()*2, PROT_READ|PROT_WRITE|PROT_EXEC);
    // Create relative jmp to hook
    gen_immediate_jump(trampoline->hookJump, hook);
    // Copy original opcodes
    trampoline->nOriginalBytes = code_len;
    std::copy((uint8_t*)pOriginalFunc, ((uint8_t*)pOriginalFunc)+code_len, pTrampolineCode);
    pTrampolineCode += code_len;
    // Create the relative jmp to original (function + backed up opcodes)
    pTrampolineCode = gen_immediate_jump(pTrampolineCode, pOriginalFunc+code_len);
    pTrampolineCode += relative_addr_jump_size;

    // Disable trampoline region write
    mprotect(trampoline_page, page_size()*2, PROT_READ|PROT_EXEC);

    cur_transaction.push_back({false, ppOriginalFunc, trampoline});

    return 0;
}

/* ------ DOCUMENTATION ------
http://www.c-jump.com/CIS77/CPU/x86/lecture.html                <- some help to understand [MOD][REG][R/M] (see paragraph #6)
http://shell-storm.org/online/Online-Assembler-and-Disassembler <- online assembler
http://ref.x86asm.net/coder32.html                              <- opcodes reference

X86

push ebx     : 0x53
sub  esp ??  : 0x83 0xEC 0x??
call ????????: 0xE8 0x?? 0x?? 0x?? 0x??


// relative jmp: ???????? = dst_addr - curr_addr - 5
jmp ???????? : 0xe9 0x?? 0x?? 0x?? 0x??
destination = 0x8dba8
jmp location: 0x91995 - opcodes: e9 0e c2 ff ff
0e c2 ff ff = 0x8dba8 - 0x91995 - 5

// short jmp: ?? = dst_addr - curr_addr - 2
jmp short ??: 0xeb 0x??
destination = 0x91964
jmp location: 0x9198f - opcodes: 0xeb 0xd3
d3 = 0x91964 - 0x9198f - 2

X64

TODO:
 Hint: make relative jump to near (+/-2Gb) code : 5 bytes
       load absolute addr into R11 and call a jmp on register R11 : 13 bytes
    Or
        Reuse x86 relative jmp method

Example:
mov r11, 0x0123456789abcdef -> 0x49 0xbb 0xef 0xcd 0xab 0x89 0x67 0x45 0x23 0x01
jmp r11 -> 0x41 0xff 0xe3


*/
