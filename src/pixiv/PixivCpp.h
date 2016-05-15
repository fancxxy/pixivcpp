#pragma once

#include "../curl/CurlRequest.h"

#include <string> 
#include <json/value.h> 

class PixivCpp 
{ 
    public: 
        PixivCpp(); 

        void DownloadWorks(const std::string &illust_id);

        void DownloadFavorite(const std::string page = "1", 
                const std::string per_page = "50", 
                const std::string publicity = "public",
                const std::string image_sizes = "px_128x128,px_480mw,large");

    private: 
        void PixivOauth(const std::string &username, const std::string &password);

        void PixivDownload(const Json::Value&);

        const std::string GetWorks(const std::string &illust_id);

        const std::string GetFavorite(const std::string page = "1", 
                const std::string per_page = "50", 
                const std::string publicity = "public",
                const std::string image_sizes = "px_128x128,px_480mw,large");

        CurlRequest request_;
        std::string access_token_;
};  
