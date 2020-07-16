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

#ifndef LOCAL_STORAGE_INCLUDE
#define LOCAL_STORAGE_INCLUDE

#include "base.h"

#define MAX_FILENAME_LENGTH 300

class Local_Storage {
public:
    static constexpr auto inventory_storage_folder = "inventory";
    static constexpr auto settings_storage_folder  = "settings";
    static constexpr auto remote_storage_folder    = "remote";
    static constexpr auto stats_storage_folder     = "stats";
    static constexpr auto user_data_storage        = "local";
    static constexpr auto game_settings_folder     = "steam_settings";

private:
    std::string save_directory;
    std::string appid;
public:
    static std::string get_program_path();
    static std::string get_game_settings_path();
    static std::string get_user_appdata_path();
    Local_Storage(std::string save_directory);
    static int get_file_data(std::string full_path, char *data, unsigned int max_length, unsigned int offset=0);
    void setAppId(uint32 appid);
    static int store_file_data(std::string folder, std::string file, char *data, unsigned int length);
    static std::vector<std::string> get_filenames_path(std::string path);

    int store_data(std::string folder, std::string file, char *data, unsigned int length);
    int store_data_settings(std::string file, char *data, unsigned int length);
    int get_data(std::string folder, std::string file, char *data, unsigned int max_length, unsigned int offset=0);
    int get_data_settings(std::string file, char *data, unsigned int max_length);
    int count_files(std::string folder);
    bool iterate_file(std::string folder, int index, char *output_filename, int32 *output_size);
    bool file_exists(std::string folder, std::string file);
    unsigned int file_size(std::string folder, std::string file);
    bool file_delete(std::string folder, std::string file);
    uint64_t file_timestamp(std::string folder, std::string file);
    std::string get_global_settings_path();
    std::string get_path(std::string folder);

    bool update_save_filenames(std::string folder);

    bool load_json(std::string full_path, nlohmann::json& json);
    bool load_json_file(std::string folder, std::string const& file, nlohmann::json& json);
    bool write_json_file(std::string folder, std::string const& file, nlohmann::json const& json);
};

#endif
