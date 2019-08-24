#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <cstdint>
#include <fstream>
#include <iomanip>

#include <curl/curl.h>
#include <json/json.hpp>

class CurlGlobal
{
    bool _init;

    CurlGlobal() :_init(false) {}

    ~CurlGlobal() { cleanup(); }

public:
    static CurlGlobal& Inst()
    {
        static CurlGlobal _this;
        return _this;
    }

    CURLcode init(long flags = CURL_GLOBAL_DEFAULT) { return curl_global_init(flags); }
    void cleanup()
    {
        if (_init)
        {
            curl_global_cleanup();
            _init = false;
        }
    }
};

class CurlEasy
{
    CURL* _me;
    bool _init;
    std::string _buffer;

    static int writer(char* data, size_t size, size_t nmemb,
        CurlEasy *_this)
    {
        if (_this == nullptr)
            return 0;

        _this->_buffer.append(data, size * nmemb);

        return size * nmemb;
    }

public:
    CurlEasy() :_me(nullptr), _init(false) {}
    ~CurlEasy() { cleanup(); }

    bool init()
    {
        _init = (_me = curl_easy_init()) != nullptr;
        if (_init)
        {
            if (curl_easy_setopt(_me, CURLOPT_WRITEFUNCTION, writer) != CURLE_OK)
            {
                cleanup();
                return false;
            }

            if (curl_easy_setopt(_me, CURLOPT_WRITEDATA, this) != CURLE_OK)
            {
                cleanup();
                return false;
            }
        }
        return _init;
    }

    void cleanup()
    {
        if (_init)
        {
            curl_easy_cleanup(_me);
        }
    }

    CURLcode set_url(const std::string& url)
    {
        return curl_easy_setopt(_me, CURLOPT_URL, url.c_str());
    }

    CURLcode skip_verifypeer(bool skip = true)
    {
        return curl_easy_setopt(_me, CURLOPT_SSL_VERIFYPEER, skip ? 0L : 1L);
    }

    CURLcode skip_verifhost(bool skip = true)
    {
        return curl_easy_setopt(_me, CURLOPT_SSL_VERIFYHOST, skip ? 0L : 1L);
    }

    CURLcode connect_only(bool connect = true)
    {
        return curl_easy_setopt(_me, CURLOPT_CONNECT_ONLY, connect ? 1L : 0L);
    }

    CURLcode perform()
    {
        _buffer.clear();
        return curl_easy_perform(_me);
    }

    CURLcode recv(void *buffer, size_t buflen, size_t* read_len)
    {
        return curl_easy_recv(_me, buffer, buflen, read_len);
    }

    CURLcode get_html_code(long &code)
    {
        return curl_easy_getinfo(_me, CURLINFO_RESPONSE_CODE, &code);
    }

    std::string const& get_answer() const { return _buffer; }
};

// Get all steam appid with their name: http://api.steampowered.com/ISteamApps/GetAppList/v2/
// Steam storefront webapi: https://wiki.teamfortress.com/wiki/User:RJackson/StorefrontAPI
// http://api.steampowered.com/ISteamUserStats/GetSchemaForGame/v2/?key=<key>&appid=<appid>
/*
{
  "game" : {
    "gameName" : "<name>",
    "availableGameStats" : {
      "achievements" : {
        ("<id>" : {
          "name" : "achievement_name",
          "displayName" : "achievement name on screen",
          "hidden" : (0|1),
          ["description" : "<desc>",]
          "icon" : "<url to icon when achievement is earned>",
          "icongray" : "<url to icon when achievement is not earned>"
        },
        ...)
      }
    }
  }
}
*/
// Get appid infos: http://store.steampowered.com/api/appdetails/?appids=218620
/*
"appid" : {
  "success" : (true|false),
  (success == true "data" : {
    ...
    "name" : "<name>",
    "steam_appid" : <appid>,
    (OPT "dlc" : [<dlc id>, <dlc id>]),
    "header_image" : "<miniature url>" <-- Use this in the overlay ?
    (OPT "achievements" : {
      "total" : <num of achievements>
    }),
    "background" : "<background url>" <-- Use this as the overlay background ?
    (OPT "packages" : [<package id>, <package id>])
  })
}
*/

#ifdef max
#undef max
#endif

int main()
{
    CurlGlobal& cglobal = CurlGlobal::Inst();
    cglobal.init();

    CurlEasy easy;
    if (easy.init())
    {
        std::string url;
        std::string steam_apikey;
        std::string app_id;

        std::cout << "Enter the game appid: ";
        std::cin >> app_id; 
        std::cout << "Enter your webapi key: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin >> steam_apikey;

        url = "http://api.steampowered.com/ISteamUserStats/GetSchemaForGame/v2/?key=";
        url += steam_apikey;
        url += "&appid=";
        url += app_id;
        easy.set_url(url);
        easy.perform();
        try
        {
            std::ofstream ach_file("achievements.json", std::ios::trunc | std::ios::out);
            nlohmann::json json = nlohmann::json::parse(easy.get_answer());
            nlohmann::json output_json = nlohmann::json::array();

            bool first = true;
            int i = 0;
            for (auto& item : json["game"]["availableGameStats"]["achievements"].items())
            {
                output_json[i]["name"]        = item.value()["name"];
                output_json[i]["displayName"] = item.value()["displayName"];
                output_json[i]["hidden"]      = std::to_string(item.value()["hidden"].get<int>());
                try
                {
                    output_json[i]["description"] = item.value()["description"];
                }
                catch (...)
                {
                    output_json[i]["description"] = "";
                }
                output_json[i]["icon"]        = item.value()["icon"];
                output_json[i]["icongray"]    = item.value()["icongray"];
                ++i;
            }
            ach_file << std::setw(2) << output_json;
        }
        catch (std::exception& e)
        {
            std::cerr << "Failed to get infos: ";
            long code;
            if (easy.get_html_code(code) == CURLE_OK && code == 403)
            {
                std::cerr << "Error in webapi key";
            }
            else
            {
                std::cerr << "Error while parsing json. Try to go at " << url << " and see what you can do to build your achivements.json";
            }
            std::cerr << std::endl;
        }
    }
}
    