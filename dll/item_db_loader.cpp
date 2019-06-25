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
   License along with the Goldberg Emulator; if not, see
   <http://www.gnu.org/licenses/>.  */
#include "item_db_loader.h"

#include <fstream>
#include "json.hpp"

std::map<SteamItemDef_t, std::map<std::string, std::string>> read_items_db(std::string const& items_db)
{
    std::map<SteamItemDef_t, std::map<std::string, std::string>> items;
   
    std::ifstream items_file(items_db);
    items_file.seekg(0, std::ios::end);
    size_t size = items_file.tellg();
    std::string buffer(size, '\0');
    items_file.seekg(0);
    items_file.read(&buffer[0], size);
    items_file.close();

    try
    {
        std::map<SteamItemDef_t, std::map<std::string, std::string>> tmp;
        nlohmann::json json = nlohmann::json::parse(buffer);

        for (auto& i : json.items())
        {
            SteamItemDef_t key = std::stoi((*i).key());
            nlohmann::json& value = (*i).value();
            for( auto& j : value.items() )
            {
                tmp[key][(*j).key()] = (*j).value();
            }
        }

        items.swap(tmp);
    }
    catch (std::exception& e)
    {
        PRINT_DEBUG("Error while parsing json: %s", e.what());
    }

    return items;
}