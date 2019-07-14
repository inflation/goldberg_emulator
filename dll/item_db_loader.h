/* Copyright (C) 2019 Nemirtingas (Maxime P)
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
   License along with the Goldberg Emulator;*/
#ifndef __ITEM_DB_LOADER_INCLUDED__
#define __ITEM_DB_LOADER_INCLUDED__

#include "base.h" // For SteamItemDef_t

#include <atomic>
void read_items_db(std::string items_db, std::map<SteamItemDef_t, std::map<std::string, std::string>> *items, std::atomic_bool *is_loaded);

#endif//__ITEM_DB_LOADER_INCLUDED__