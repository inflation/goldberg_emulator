#include "steam_inventory.h"

std::once_flag   Steam_Inventory::items_loading;
std::atomic_bool Steam_Inventory::items_loaded(false);
std::map<SteamItemDef_t, std::map<std::string, std::string>> Steam_Inventory::items;