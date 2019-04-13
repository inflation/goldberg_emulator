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

#ifndef LOCAL_STORAGE_INCLUDE
#define LOCAL_STORAGE_INCLUDE

#define SETTINGS_STORAGE_FOLDER "settings"
#define REMOTE_STORAGE_FOLDER "remote"
#define STATS_STORAGE_FOLDER "stats"
#define USER_DATA_FOLDER "userdata"

#define GAME_SETTINGS_FOLDER "steam_settings"

#include <string>

#define MAX_FILENAME_LENGTH 300

class Local_Storage {
    std::string save_directory;
    std::string appid;
public:
    static std::string get_program_path();
    static std::string get_game_settings_path();
    static std::string get_user_appdata_path();
    Local_Storage(std::string save_directory);
    static int get_file_data(std::string full_path, char *data, unsigned int max_length);
    void setAppId(uint32 appid);
    static int store_file_data(std::string folder, std::string file, char *data, unsigned int length);
    static std::vector<std::string> get_filenames_path(std::string path);

    int store_data(std::string folder, std::string file, char *data, unsigned int length);
    int store_data_settings(std::string file, char *data, unsigned int length);
    int get_data(std::string folder, std::string file, char *data, unsigned int max_length);
    int get_data_settings(std::string file, char *data, unsigned int max_length);
    int count_files(std::string folder);
    bool iterate_file(std::string folder, int index, char *output_filename, int32 *output_size);
    bool file_exists(std::string folder, std::string file);
    unsigned int file_size(std::string folder, std::string file);
    bool file_delete(std::string folder, std::string file);
    uint64_t file_timestamp(std::string folder, std::string file);
    std::string get_path(std::string folder);

    bool update_save_filenames(std::string folder);
};

#endif
