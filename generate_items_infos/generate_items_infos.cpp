#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <iomanip>
#include <thread>

#include <json/json.hpp>
#include <steam_gameserver.h>

struct ClientCBS
{
    STEAM_CALLBACK(ClientCBS, OnSteamInventoryDefinitionUpdate, SteamInventoryDefinitionUpdate_t);
    STEAM_CALLBACK(ClientCBS, OnSteamInventoryResultReady     , SteamInventoryResultReady_t);
};

bool definition_update = false;

std::vector<std::string> split(const std::string& s, char delimiter)
{
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter))
    {
        tokens.push_back(token);
    }
    return tokens;
}

void ClientCBS::OnSteamInventoryResultReady(SteamInventoryResultReady_t* param)
{
    switch (param->m_result)
    {
    case k_EResultOK: break;
    case k_EResultPending: break;
    }

    if (param->m_handle)
    {
        SteamInventory()->DestroyResult(param->m_handle);
    }
}

void ClientCBS::OnSteamInventoryDefinitionUpdate(SteamInventoryDefinitionUpdate_t* param)
{
    std::ofstream out("items.json", std::ios::out | std::ios::trunc);
    nlohmann::json json = nlohmann::json::object();
    SteamItemDef_t* items;
    uint32 size = 0;
    SteamInventory()->GetItemDefinitionIDs(nullptr, &size);
    items = new SteamItemDef_t[size];
    SteamInventory()->GetItemDefinitionIDs(items, &size);

    definition_update = true;

    std::cerr << "Creating json, please wait..." << std::endl;

    for (int i = 0; i < size; ++i)
    {
        uint32 len;
        len = 0;
        if (SteamInventory()->GetItemDefinitionProperty(items[i], nullptr, nullptr, &len))
        {
            std::string buffer(len, '\0');
            if (SteamInventory()->GetItemDefinitionProperty(items[i], nullptr, &buffer[0], &len))
            {
                buffer.pop_back();
                std::vector<std::string> strs(std::move(split(buffer, ',')));

                std::string key = std::to_string(items[i]);

                for (auto j = strs.begin(); j != strs.end(); ++j)
                {
                    len = 0;
                    if (SteamInventory()->GetItemDefinitionProperty(items[i], j->c_str(), nullptr, &len))
                    {
                        std::string buffer(len, '\0');
                        if (SteamInventory()->GetItemDefinitionProperty(items[i], j->c_str(), &buffer[0], &len))
                        {
                            buffer.pop_back();
                            if( *j == "quantity")
                                json[key][*j] = "0";
                            else
                                json[key][*j] = buffer;
                        }
                    }
                }
            }
        }
    }

    out << std::setw(2) << json;

    delete[]items;
}

int main(int argc, char *argv[])
{
    uint32_t appid;

    if (argc == 2)
    {
        appid = std::stoi(argv[1]);
    }
    else
    {
        std::cout << "Enter the game appid: ";
        std::cin >> appid;
    }

    std::ofstream steam_api("steam_appid.txt", std::ios::out | std::ios::trunc);
    steam_api << appid;
    steam_api.close();

    if (SteamAPI_RestartAppIfNecessary(0))
    {
        std::cerr << "This app needs restart" << std::endl;
        return EXIT_FAILURE;
    }

    if (!SteamAPI_Init())
    {
        std::cerr << "SteamAPI_Init() failed" << std::endl;
        return EXIT_FAILURE;
    }

    auto SUser = SteamUser();

    if (!SUser->BLoggedOn())
    {
        std::cerr << "Steam user is not logged in" << std::endl;
        return EXIT_FAILURE;
    }

    ClientCBS cbs;

    int max_retry = 10;
    while (!definition_update && max_retry-- > 0)
    {
        std::cerr << "Running LoadItemDefinitions" << std::endl;
        bool ret = SteamInventory()->LoadItemDefinitions();
        int retry = 0;
        while (retry++ <= 100 && !definition_update)
        {
            SteamAPI_RunCallbacks();
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }

    SteamAPI_Shutdown();

    return 0;
}