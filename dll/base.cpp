/* Copyright (C) 2019 Mr Goldberg
   This file is part of the Goldberg Emulator

   The Goldberg Emulator is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 3 of the License, or (at your option) any later version.

   The Goldberg Emulator is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the Goldberg Emulator; if not, see
   <http://www.gnu.org/licenses/>.  */

#include "base.h"

#ifdef __WINDOWS__

static void
randombytes(char * const buf, const size_t size)
{
    while (!RtlGenRandom((PVOID) buf, (ULONG) size)) {
        PRINT_DEBUG("RtlGenRandom ERROR\n");
        Sleep(100);
    }

}

std::string get_env_variable(std::string name)
{
    wchar_t env_variable[1024];
    DWORD ret = GetEnvironmentVariableW(utf8_decode(name).c_str(), env_variable, _countof(env_variable));
    if (ret <= 0) {
        return std::string();
    }

    env_variable[ret] = 0;
    return utf8_encode(env_variable);
}

bool set_env_variable(std::string name, std::string value)
{
    return SetEnvironmentVariableW(utf8_decode(name).c_str(), utf8_decode(value).c_str());
}

#else

static int fd = -1;

static void randombytes(char *buf, size_t size)
{
  int i;

  if (fd == -1) {
    for (;;) {
      fd = open("/dev/urandom",O_RDONLY);
      if (fd != -1) break;
      sleep(1);
    }
  }

  while (size > 0) {
    if (size < 1048576) i = size; else i = 1048576;

    i = read(fd,buf,i);
    if (i < 1) {
      sleep(1);
      continue;
    }

    buf += i;
    size -= i;
  }
}

std::string get_env_variable(std::string name)
{
    char *env = getenv(name.c_str());
    if (!env) {
        return std::string();
    }

    return std::string(env);
}

bool set_env_variable(std::string name, std::string value)
{
    return setenv(name.c_str(), value.c_str(), 1) == 0;
}

#endif

std::recursive_mutex global_mutex;

SteamAPICall_t generate_steam_api_call_id() {
    static SteamAPICall_t a;
    randombytes((char *)&a, sizeof(a));
    ++a;
    if (a == 0) ++a;
    return a;
}

int generate_random_int() {
    int a;
    randombytes((char *)&a, sizeof(a));
    return a;
}

static uint32 generate_steam_ticket_id() {
    /* not random starts with 2? */
    static uint32 a = 1;
    ++a;
    if (a == 0) ++a;
    return a;
}

static unsigned generate_account_id()
{
    int a;
    randombytes((char *)&a, sizeof(a));
    a = abs(a);
    if (!a) ++a;
    return a;
}

CSteamID generate_steam_id_user()
{
    return CSteamID(generate_account_id(), k_unSteamUserDefaultInstance, k_EUniversePublic, k_EAccountTypeIndividual);
}

static CSteamID generate_steam_anon_user()
{
    return CSteamID(generate_account_id(), k_unSteamUserDefaultInstance, k_EUniversePublic, k_EAccountTypeAnonUser);
}

CSteamID generate_steam_id_server()
{
    return CSteamID(generate_account_id(), k_unSteamUserDefaultInstance, k_EUniversePublic, k_EAccountTypeGameServer);
}

CSteamID generate_steam_id_anonserver()
{
    return CSteamID(generate_account_id(), k_unSteamUserDefaultInstance, k_EUniversePublic, k_EAccountTypeAnonGameServer);
}

CSteamID generate_steam_id_lobby()
{
    return CSteamID(generate_account_id(), k_EChatInstanceFlagLobby | k_EChatInstanceFlagMMSLobby, k_EUniversePublic, k_EAccountTypeChat);
}

bool check_timedout(std::chrono::high_resolution_clock::time_point old, double timeout)
{
    if (timeout == 0.0) return true;

    std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();
    if (std::chrono::duration_cast<std::chrono::duration<double>>(now - old).count() > timeout) {
        return true;
    }

    return false;
}

#ifdef __LINUX__
std::string get_lib_path() {
  std::string dir = "/proc/self/map_files";
  DIR *dp;
  int i = 0;
  struct dirent *ep;
  dp = opendir (dir.c_str());
  uintptr_t p = (uintptr_t)&get_lib_path;

  if (dp != NULL)
  {
    while ((ep = readdir (dp))) {
      if (memcmp(ep->d_name, ".", 2) != 0 && memcmp(ep->d_name, "..", 3) != 0) {
            char *upper = NULL;
            uintptr_t lower_bound = strtoull(ep->d_name, &upper, 16);
            if (lower_bound) {
                ++upper;
                uintptr_t upper_bound = strtoull(upper, &upper, 16);
                if (upper_bound && (lower_bound < p && p < upper_bound)) {
                    std::string path = dir + PATH_SEPARATOR + ep->d_name;
                    char link[PATH_MAX] = {};
                    if (readlink(path.c_str(), link, sizeof(link)) > 0) {
                        std::string lib_path = link;
                        (void) closedir (dp);
                        return link;
                    }
                }
            }

        i++;
      }
    }

    (void) closedir (dp);
  }

  return ".";
}
#elif __APPLE__
std::string get_lib_path() {
  std::string library_path = "./";
  Dl_info infos;
  dladdr((void *)&get_lib_path, &infos);
  library_path = infos.dli_fname;
  return library_path;
}
#endif

std::string get_full_lib_path()
{
    std::string program_path;
#if defined(__WINDOWS__)
    wchar_t   DllPath[2048] = {0};
    GetModuleFileNameW((HINSTANCE)&__ImageBase, DllPath, _countof(DllPath));
    program_path = utf8_encode(DllPath);
#else
    program_path = get_lib_path();
#endif
    return program_path;
}

std::string get_full_program_path()
{
    std::string env_program_path = get_env_variable("SteamAppPath");
    if (env_program_path.length()) {
        if (env_program_path.back() != PATH_SEPARATOR[0]) {
            env_program_path = env_program_path.append(PATH_SEPARATOR);
        }

        return env_program_path;
    }

    std::string program_path;
    program_path = get_full_lib_path();
    return program_path.substr(0, program_path.rfind(PATH_SEPARATOR)).append(PATH_SEPARATOR);
}

std::string get_current_path()
{
    std::string path;
#if defined(STEAM_WIN32)
    char *buffer = _getcwd( NULL, 0 );
#elif __LINUX__
    char *buffer = get_current_dir_name();
#else
    char *buffer = getcwd(NULL, 0);
#endif
    if (buffer) {
        path = buffer;
        path.append(PATH_SEPARATOR);
        free(buffer);
    }

    return path;
}

std::string canonical_path(std::string path)
{
    std::string output;
#if defined(STEAM_WIN32)
    wchar_t *buffer = _wfullpath(NULL, utf8_decode(path).c_str(), 0);
    if (buffer) {
        output = utf8_encode(buffer);
        free(buffer);
    }
#elif defined (__LINUX__)
    char *buffer = canonicalize_file_name(path.c_str());
    if (buffer) {
        output = buffer;
        free(buffer);
    }
#else
    char *resolved_name = (char *)malloc(1024);
    char *buffer = realpath(path.c_str(), resolved_name);
    if (buffer) {
      output = buffer;
      free(buffer);
    }
#endif

    return output;
}

bool file_exists_(std::string full_path)
{
#if defined(STEAM_WIN32)
    struct _stat buffer;
    if (_wstat(utf8_decode(full_path).c_str(), &buffer) != 0)
        return false;

    if ( buffer.st_mode & _S_IFDIR)
        return false;
#else
    struct stat buffer;
    if (stat(full_path.c_str(), &buffer) != 0)
        return false;

    if (S_ISDIR(buffer.st_mode))
        return false;
#endif

    return true;
}

unsigned int file_size_(std::string full_path)
{
#if defined(STEAM_WIN32)
    struct _stat buffer = {};
    if (_wstat(utf8_decode(full_path).c_str(), &buffer) != 0) return 0;
#else
    struct stat buffer = {};
    if (stat (full_path.c_str(), &buffer) != 0) return 0;
#endif
    return buffer.st_size;
}

static void steam_auth_ticket_callback(void *object, Common_Message *msg)
{
    PRINT_DEBUG("steam_auth_ticket_callback\n");

    Auth_Ticket_Manager *auth_ticket_manager = (Auth_Ticket_Manager *)object;
    auth_ticket_manager->Callback(msg);
}

Auth_Ticket_Manager::Auth_Ticket_Manager(class Settings *settings, class Networking *network, class SteamCallBacks *callbacks) {
    this->network = network;
    this->settings = settings;
    this->callbacks = callbacks;

    this->network->setCallback(CALLBACK_ID_AUTH_TICKET, settings->get_local_steam_id(), &steam_auth_ticket_callback, this);
    this->network->setCallback(CALLBACK_ID_USER_STATUS, settings->get_local_steam_id(), &steam_auth_ticket_callback, this);
}

#define STEAM_TICKET_PROCESS_TIME 0.03

void Auth_Ticket_Manager::launch_callback(CSteamID id, EAuthSessionResponse resp, double delay)
{
    ValidateAuthTicketResponse_t data;
    data.m_SteamID = id;
    data.m_eAuthSessionResponse = resp;
    data.m_OwnerSteamID = id;
    callbacks->addCBResult(data.k_iCallback, &data, sizeof(data), delay);
}

void Auth_Ticket_Manager::launch_callback_gs(CSteamID id, bool approved)
{
    if (approved) {
        GSClientApprove_t data;
        data.m_SteamID = data.m_OwnerSteamID = id;
        callbacks->addCBResult(data.k_iCallback, &data, sizeof(data));
    } else {
        GSClientDeny_t data;
        data.m_SteamID = id;
        data.m_eDenyReason = k_EDenyNotLoggedOn; //TODO: other reasons?
        callbacks->addCBResult(data.k_iCallback, &data, sizeof(data));
    }
}

#define STEAM_ID_OFFSET_TICKET (4 + 8)
#define STEAM_TICKET_MIN_SIZE (4 + 8 + 8)
Auth_Ticket_Data Auth_Ticket_Manager::getTicketData( void *pTicket, int cbMaxTicket, uint32 *pcbTicket )
{
    uint64 steam_id = settings->get_local_steam_id().ConvertToUint64();
    memset(pTicket, 123, cbMaxTicket);
    ((char *)pTicket)[0] = 0x14;
    ((char *)pTicket)[1] = 0;
    ((char *)pTicket)[2] = 0;
    ((char *)pTicket)[3] = 0;
    memcpy((char *)pTicket + STEAM_ID_OFFSET_TICKET, &steam_id, sizeof(steam_id));
    *pcbTicket = cbMaxTicket;

    Auth_Ticket_Data ticket_data;
    ticket_data.id = settings->get_local_steam_id();
    ticket_data.number = generate_steam_ticket_id();
    uint32 ttt = ticket_data.number;

    memcpy(((char *)pTicket) + sizeof(uint64), &ttt, sizeof(ttt));
    return ticket_data;
}
//Conan Exiles doesn't work with 512 or 128, 256 seems to be the good size
//Steam returns 234
#define STEAM_AUTH_TICKET_SIZE 234

uint32 Auth_Ticket_Manager::getTicket( void *pTicket, int cbMaxTicket, uint32 *pcbTicket )
{
    if (cbMaxTicket < STEAM_TICKET_MIN_SIZE) return 0;
    if (cbMaxTicket > STEAM_AUTH_TICKET_SIZE) cbMaxTicket = STEAM_AUTH_TICKET_SIZE;

    Auth_Ticket_Data ticket_data = getTicketData(pTicket, cbMaxTicket, pcbTicket );
    uint32 ttt = ticket_data.number;
    GetAuthSessionTicketResponse_t data;
    data.m_hAuthTicket = ttt;
    data.m_eResult = k_EResultOK;
    callbacks->addCBResult(data.k_iCallback, &data, sizeof(data), STEAM_TICKET_PROCESS_TIME);

    outbound.push_back(ticket_data);

    return ttt;
}

CSteamID Auth_Ticket_Manager::fakeUser()
{
    Auth_Ticket_Data data = {};
    data.id = generate_steam_anon_user();
    inbound.push_back(data);
    return data.id;
}

void Auth_Ticket_Manager::cancelTicket(uint32 number)
{
    auto ticket = std::find_if(outbound.begin(), outbound.end(), [&number](Auth_Ticket_Data const& item) { return item.number == number; });
    if (outbound.end() == ticket)
        return;

    Auth_Ticket *auth_ticket = new Auth_Ticket();
    auth_ticket->set_number(number);
    auth_ticket->set_type(Auth_Ticket::CANCEL);
    Common_Message msg;
    msg.set_source_id(settings->get_local_steam_id().ConvertToUint64());
    msg.set_allocated_auth_ticket(auth_ticket);
    network->sendToAll(&msg, true);

    outbound.erase(ticket);
}

bool Auth_Ticket_Manager::SendUserConnectAndAuthenticate( uint32 unIPClient, const void *pvAuthBlob, uint32 cubAuthBlobSize, CSteamID *pSteamIDUser )
{
    if (cubAuthBlobSize < STEAM_TICKET_MIN_SIZE) return false;

    Auth_Ticket_Data data;
    uint64 id;
    memcpy(&id, (char *)pvAuthBlob + STEAM_ID_OFFSET_TICKET, sizeof(id));
    uint32 number;
    memcpy(&number, ((char *)pvAuthBlob) + sizeof(uint64), sizeof(number));
    data.id = CSteamID(id);
    data.number = number;
    if (pSteamIDUser) *pSteamIDUser = data.id;

    for (auto & t : inbound) {
        if (t.id == data.id) {
            //Should this return false?
            launch_callback_gs(id, true);
            return true;
        }
    }

    inbound.push_back(data);
    launch_callback_gs(id, true);
    return true;
}

EBeginAuthSessionResult Auth_Ticket_Manager::beginAuth(const void *pAuthTicket, int cbAuthTicket, CSteamID steamID )
{
    if (cbAuthTicket < STEAM_TICKET_MIN_SIZE) return k_EBeginAuthSessionResultInvalidTicket;

    Auth_Ticket_Data data;
    uint64 id;
    memcpy(&id, (char *)pAuthTicket + STEAM_ID_OFFSET_TICKET, sizeof(id));
    uint32 number;
    memcpy(&number, ((char *)pAuthTicket) + sizeof(uint64), sizeof(number));
    data.id = CSteamID(id);
    data.number = number;
    data.created = std::chrono::high_resolution_clock::now();

    for (auto & t : inbound) {
        if (t.id == data.id && !check_timedout(t.created, STEAM_TICKET_PROCESS_TIME)) {
            return k_EBeginAuthSessionResultDuplicateRequest;
        }
    }

    inbound.push_back(data);
    launch_callback(steamID, k_EAuthSessionResponseOK, STEAM_TICKET_PROCESS_TIME);
    return k_EBeginAuthSessionResultOK;
}

uint32 Auth_Ticket_Manager::countInboundAuth()
{
    return inbound.size();
}

bool Auth_Ticket_Manager::endAuth(CSteamID id)
{
    bool erased = false;
    auto t = std::begin(inbound);
    while (t != std::end(inbound)) {
        if (t->id == id) {
            erased = true;
            t = inbound.erase(t);
        } else {
            ++t;
        }
    }

    return erased;
}

void Auth_Ticket_Manager::Callback(Common_Message *msg)
{
    if (msg->has_low_level()) {
        if (msg->low_level().type() == Low_Level::CONNECT) {
            
        }

        if (msg->low_level().type() == Low_Level::DISCONNECT) {
            PRINT_DEBUG("TICKET DISCONNECT\n");
            auto t = std::begin(inbound);
            while (t != std::end(inbound)) {
                if (t->id.ConvertToUint64() == msg->source_id()) {
                    launch_callback(t->id, k_EAuthSessionResponseUserNotConnectedToSteam);
                    t = inbound.erase(t);
                } else {
                    ++t;
                }
            }
        }
    }

    if (msg->has_auth_ticket()) {
        if (msg->auth_ticket().type() == Auth_Ticket::CANCEL) {
            PRINT_DEBUG("TICKET CANCEL %llu\n", msg->source_id());
            uint32 number = msg->auth_ticket().number();
            auto t = std::begin(inbound);
            while (t != std::end(inbound)) {
                if (t->id.ConvertToUint64() == msg->source_id() && t->number == number) {
                    PRINT_DEBUG("TICKET CANCELED\n");
                    launch_callback(t->id, k_EAuthSessionResponseAuthTicketCanceled);
                    t = inbound.erase(t);
                } else {
                    ++t;
                }
            }
        }
    }
}

#ifdef EMU_EXPERIMENTAL_BUILD
#ifdef __WINDOWS__

struct ips_test {
    uint32_t ip_from;
    uint32_t ip_to;
};

static std::vector<struct ips_test> whitelist_ips;

void set_whitelist_ips(uint32_t *from, uint32_t *to, unsigned num_ips)
{
    whitelist_ips.clear();
    for (unsigned i = 0; i < num_ips; ++i) {
        struct ips_test ip_a;
        PRINT_DEBUG("from: %hhu.%hhu.%hhu.%hhu\n", ((unsigned char *)&from[i])[0], ((unsigned char *)&from[i])[1], ((unsigned char *)&from[i])[2], ((unsigned char *)&from[i])[3]);
        PRINT_DEBUG("to: %hhu.%hhu.%hhu.%hhu\n", ((unsigned char *)&to[i])[0], ((unsigned char *)&to[i])[1], ((unsigned char *)&to[i])[2], ((unsigned char *)&to[i])[3]);
        ip_a.ip_from = ntohl(from[i]);
        ip_a.ip_to = ntohl(to[i]);
        if (ip_a.ip_to < ip_a.ip_from) continue;
        if ((ip_a.ip_to - ip_a.ip_from) > (1 << 25)) continue;
        PRINT_DEBUG("added\n");
        whitelist_ips.push_back(ip_a);
    }
}

static bool is_whitelist_ip(unsigned char *ip)
{
    uint32_t ip_temp = 0;
    memcpy(&ip_temp, ip, sizeof(ip_temp));
    ip_temp = ntohl(ip_temp);

    for (auto &i : whitelist_ips) {
        if (i.ip_from <= ip_temp && ip_temp <= i.ip_to) {
            PRINT_DEBUG("WHITELISTED IP %hhu.%hhu.%hhu.%hhu\n", ip[0], ip[1], ip[2], ip[3]);
            return true;
        }
    }

    return false;
}

static bool is_lan_ipv4(unsigned char *ip)
{
    PRINT_DEBUG("CHECK LAN IP %hhu.%hhu.%hhu.%hhu\n", ip[0], ip[1], ip[2], ip[3]);
    if (is_whitelist_ip(ip)) return true;
    if (ip[0] == 127) return true;
    if (ip[0] == 10) return true;
    if (ip[0] == 192 && ip[1] == 168) return true;
    if (ip[0] == 169 && ip[1] == 254 && ip[2] != 0) return true;
    if (ip[0] == 172 && ip[1] >= 16 && ip[1] <= 31) return true;
    if ((ip[0] == 100) && ((ip[1] & 0xC0) == 0x40)) return true;
    if (ip[0] == 239) return true; //multicast
    if (ip[0] == 0) return true; //Current network
    if (ip[0] == 192 && (ip[1] == 18 || ip[1] == 19)) return true; //Used for benchmark testing of inter-network communications between two separate subnets.
    if (ip[0] >= 224) return true; //ip multicast (224 - 239) future use (240.0.0.0 - 255.255.255.254) broadcast (255.255.255.255)
    return false;
}

static bool is_lan_ip(const sockaddr *addr, int namelen)
{
    if (!namelen) return false;

    if (addr->sa_family == AF_INET) {
        struct sockaddr_in *addr_in = (struct sockaddr_in *)addr;
        unsigned char ip[4];
        memcpy(ip, &addr_in->sin_addr, sizeof(ip));
        if (is_lan_ipv4(ip)) return true;
    } else if (addr->sa_family == AF_INET6) {
        struct sockaddr_in6 *addr_in6 = (struct sockaddr_in6 *)addr;
        unsigned char ip[16];
        unsigned char zeroes[16] = {};
        memcpy(ip, &addr_in6->sin6_addr, sizeof(ip));
        PRINT_DEBUG("CHECK LAN IP6 %hhu.%hhu.%hhu.%hhu.%hhu.%hhu.%hhu.%hhu.%hhu.%hhu.%hhu.%hhu.%hhu.%hhu.%hhu.%hhu\n", ip[0], ip[1], ip[2], ip[3], ip[4], ip[5], ip[6], ip[7], ip[8], ip[9], ip[10], ip[11], ip[12], ip[13], ip[14], ip[15]);
        if (((ip[0] == 0xFF) && (ip[1] < 3) && (ip[15] == 1)) ||
        ((ip[0] == 0xFE) && ((ip[1] & 0xC0) == 0x80))) return true;
        if (memcmp(zeroes, ip, sizeof(ip)) == 0) return true;
        if (memcmp(zeroes, ip, sizeof(ip) - 1) == 0 && ip[15] == 1) return true;
        if (ip[0] == 0xff) return true; //multicast
        if (ip[0] == 0xfc) return true; //unique local
        if (ip[0] == 0xfd) return true; //unique local

        unsigned char ipv4_mapped[12] = {};
        ipv4_mapped[10] = 0xFF;
        ipv4_mapped[11] = 0xFF;
        if (memcmp(ipv4_mapped, ip, sizeof(ipv4_mapped)) == 0) {
            if (is_lan_ipv4(ip + 12)) return true;
        }
    }

    PRINT_DEBUG("NOT LAN IP\n");
    return false;
}

int ( WINAPI *Real_SendTo )( SOCKET s, const char *buf, int len, int flags, const sockaddr *to, int tolen) = sendto;
int ( WINAPI *Real_Connect )( SOCKET s, const sockaddr *addr, int namelen ) = connect;
int ( WINAPI *Real_WSAConnect )( SOCKET s, const sockaddr *addr, int namelen, LPWSABUF lpCallerData, LPWSABUF lpCalleeData, LPQOS lpSQOS, LPQOS lpGQOS) = WSAConnect;

static int WINAPI Mine_SendTo( SOCKET s, const char *buf, int len, int flags, const sockaddr *to, int tolen) {
    PRINT_DEBUG("Mine_SendTo\n");
    if (is_lan_ip(to, tolen)) {
        return Real_SendTo( s, buf, len, flags, to, tolen );
    } else {
        return len;
    }
}

static int WINAPI Mine_Connect( SOCKET s, const sockaddr *addr, int namelen )
{
    PRINT_DEBUG("Mine_Connect\n");
    if (is_lan_ip(addr, namelen)) {
        return Real_Connect(s, addr, namelen);
    } else {
        WSASetLastError(WSAECONNREFUSED);
        return SOCKET_ERROR;
    }
}

static int WINAPI Mine_WSAConnect( SOCKET s, const sockaddr *addr, int namelen, LPWSABUF lpCallerData, LPWSABUF lpCalleeData, LPQOS lpSQOS, LPQOS lpGQOS)
{
    PRINT_DEBUG("Mine_WSAConnect\n");
    if (is_lan_ip(addr, namelen)) {
        return Real_WSAConnect(s, addr, namelen, lpCallerData, lpCalleeData, lpSQOS, lpGQOS);
    } else {
        WSASetLastError(WSAECONNREFUSED);
        return SOCKET_ERROR;
    }
}

inline bool file_exists (const std::string& name) {
  struct stat buffer;   
  return (stat (name.c_str(), &buffer) == 0); 
}

#ifdef DETOURS_64BIT
    #define DLL_NAME "steam_api64.dll"
#else
    #define DLL_NAME "steam_api.dll"
#endif

HMODULE (WINAPI *Real_GetModuleHandleA)(LPCSTR lpModuleName) = GetModuleHandleA;
HMODULE WINAPI Mine_GetModuleHandleA(LPCSTR lpModuleName)
{
    PRINT_DEBUG("Mine_GetModuleHandleA %s\n", lpModuleName);
    if (!lpModuleName) return Real_GetModuleHandleA(lpModuleName);
    std::string in(lpModuleName);
    if (in == std::string(DLL_NAME)) {
        in = std::string("crack") + in;
    }

    return Real_GetModuleHandleA(in.c_str());
}

static void redirect_crackdll()
{
    DetourTransactionBegin();
    DetourUpdateThread( GetCurrentThread() );
    DetourAttach( &(PVOID &)Real_GetModuleHandleA, Mine_GetModuleHandleA );
    DetourTransactionCommit();
}

static void unredirect_crackdll()
{
    DetourTransactionBegin();
    DetourUpdateThread( GetCurrentThread() );
    DetourDetach( &(PVOID &)Real_GetModuleHandleA, Mine_GetModuleHandleA );
    DetourTransactionCommit();
}

HMODULE crack_dll_handle;
static void load_dll()
{
    std::string path = get_full_program_path();
    path += "crack";
    //path += PATH_SEPARATOR;
    path += DLL_NAME;
    PRINT_DEBUG("Crack file %s\n", path.c_str());
    if (file_exists(path)) {
        redirect_crackdll();
        crack_dll_handle = LoadLibraryW(utf8_decode(path).c_str());
        unredirect_crackdll();
        PRINT_DEBUG("Loaded crack file\n");
    }
}

#include "local_storage.h"
static void load_dlls()
{
    std::string path = Local_Storage::get_game_settings_path();
    path += "load_dlls";
    path += PATH_SEPARATOR;

    std::vector<std::string> paths = Local_Storage::get_filenames_path(path);
    for (auto & p: paths) {
        std::string full_path = path + p;
        size_t length = full_path.length();
        if (length < 4) continue;
        if (std::toupper(full_path[length - 1]) != 'L') continue;
        if (std::toupper(full_path[length - 2]) != 'L') continue;
        if (std::toupper(full_path[length - 3]) != 'D') continue;
        if (full_path[length - 4] != '.') continue;

        PRINT_DEBUG("Trying to load %s\n", full_path.c_str());
        if (LoadLibraryW(utf8_decode(full_path).c_str())) {
            PRINT_DEBUG("LOADED %s\n", full_path.c_str());
        }
    }
}

//For some reason when this function is optimized it breaks the shogun 2 prophet (reloaded) crack.
#pragma optimize( "", off )
bool crack_SteamAPI_RestartAppIfNecessary(uint32 unOwnAppID)
{
    if (crack_dll_handle) {
        bool (__stdcall* restart_app)(uint32) = (bool (__stdcall *)(uint32))GetProcAddress(crack_dll_handle, "SteamAPI_RestartAppIfNecessary");
        if (restart_app) {
            PRINT_DEBUG("Call crack SteamAPI_RestartAppIfNecessary\n");
            redirect_crackdll();
            bool ret = restart_app(unOwnAppID);
            unredirect_crackdll();
            return ret;
        }
    }

    return false;
}
#pragma optimize( "", on )

bool crack_SteamAPI_Init()
{
    if (crack_dll_handle) {
        bool (__stdcall* init_app)() = (bool (__stdcall *)())GetProcAddress(crack_dll_handle, "SteamAPI_Init");
        if (init_app) {
            PRINT_DEBUG("Call crack SteamAPI_Init\n");
            redirect_crackdll();
            bool ret = init_app();
            unredirect_crackdll();
            return ret;
        }
    }

    return false;
}

HINTERNET (WINAPI *Real_WinHttpConnect)(
  IN HINTERNET     hSession,
  IN LPCWSTR       pswzServerName,
  IN INTERNET_PORT nServerPort,
  IN DWORD         dwReserved
);

HINTERNET WINAPI Mine_WinHttpConnect(
  IN HINTERNET     hSession,
  IN LPCWSTR       pswzServerName,
  IN INTERNET_PORT nServerPort,
  IN DWORD         dwReserved
) {
    PRINT_DEBUG("Mine_WinHttpConnect %ls %u\n", pswzServerName, nServerPort);
    struct sockaddr_in ip4;
    struct sockaddr_in6 ip6;
    ip4.sin_family = AF_INET;
    ip6.sin6_family = AF_INET6;

    if ((InetPtonW(AF_INET, pswzServerName, &(ip4.sin_addr)) && is_lan_ip((sockaddr *)&ip4, sizeof(ip4))) || (InetPtonW(AF_INET6, pswzServerName, &(ip6.sin6_addr)) && is_lan_ip((sockaddr *)&ip6, sizeof(ip6)))) {
        return Real_WinHttpConnect(hSession, pswzServerName, nServerPort, dwReserved);
    } else {
        return Real_WinHttpConnect(hSession, L"127.1.33.7", nServerPort, dwReserved);
    }
}

HINTERNET (WINAPI *Real_WinHttpOpenRequest)(
  IN HINTERNET hConnect,
  IN LPCWSTR   pwszVerb,
  IN LPCWSTR   pwszObjectName,
  IN LPCWSTR   pwszVersion,
  IN LPCWSTR   pwszReferrer,
  IN LPCWSTR   *ppwszAcceptTypes,
  IN DWORD     dwFlags
);

HINTERNET WINAPI Mine_WinHttpOpenRequest(
  IN HINTERNET hConnect,
  IN LPCWSTR   pwszVerb,
  IN LPCWSTR   pwszObjectName,
  IN LPCWSTR   pwszVersion,
  IN LPCWSTR   pwszReferrer,
  IN LPCWSTR   *ppwszAcceptTypes,
  IN DWORD     dwFlags
) {
    PRINT_DEBUG("Mine_WinHttpOpenRequest %ls %ls %ls %ls %i\n", pwszVerb, pwszObjectName, pwszVersion, pwszReferrer, dwFlags);
    if (dwFlags & WINHTTP_FLAG_SECURE) {
        dwFlags ^= WINHTTP_FLAG_SECURE;
    }

    return Real_WinHttpOpenRequest(hConnect, pwszVerb, pwszObjectName, pwszVersion, pwszReferrer, ppwszAcceptTypes, dwFlags);
}



static bool network_functions_attached = false;
BOOL WINAPI DllMain( HINSTANCE, DWORD dwReason, LPVOID ) {
    switch ( dwReason ) {
        case DLL_PROCESS_ATTACH:
            if (!file_exists(get_full_program_path() + "disable_lan_only.txt")) {
                PRINT_DEBUG("Hooking lan only functions\n");
                DetourTransactionBegin();
                DetourUpdateThread( GetCurrentThread() );
                DetourAttach( &(PVOID &)Real_SendTo, Mine_SendTo );
                DetourAttach( &(PVOID &)Real_Connect, Mine_Connect );
                DetourAttach( &(PVOID &)Real_WSAConnect, Mine_WSAConnect );

                HMODULE winhttp = GetModuleHandle("winhttp.dll");
                if (winhttp) {
                    Real_WinHttpConnect = (decltype(Real_WinHttpConnect))GetProcAddress(winhttp, "WinHttpConnect");
                    DetourAttach( &(PVOID &)Real_WinHttpConnect, Mine_WinHttpConnect );
                    // Real_WinHttpOpenRequest = (decltype(Real_WinHttpOpenRequest))GetProcAddress(winhttp, "WinHttpOpenRequest");
                    // DetourAttach( &(PVOID &)Real_WinHttpOpenRequest, Mine_WinHttpOpenRequest );
                }
    
                DetourTransactionCommit();
                network_functions_attached = true;
            }
            load_dll();
            load_dlls();
            break;

        case DLL_PROCESS_DETACH:
            if (network_functions_attached) {
                DetourTransactionBegin();
                DetourUpdateThread( GetCurrentThread() );
                DetourDetach( &(PVOID &)Real_SendTo, Mine_SendTo );
                DetourDetach( &(PVOID &)Real_Connect, Mine_Connect );
                DetourDetach( &(PVOID &)Real_WSAConnect, Mine_WSAConnect );
                if (Real_WinHttpConnect) {
                    DetourDetach( &(PVOID &)Real_WinHttpConnect, Mine_WinHttpConnect );
                    // DetourDetach( &(PVOID &)Real_WinHttpOpenRequest, Mine_WinHttpOpenRequest );
                }
                DetourTransactionCommit();
            }
        break;
    }

    return TRUE;
}
#else
void set_whitelist_ips(uint32_t *from, uint32_t *to, unsigned num_ips)
{

}
#endif
#else
void set_whitelist_ips(uint32_t *from, uint32_t *to, unsigned num_ips)
{

}
#endif
