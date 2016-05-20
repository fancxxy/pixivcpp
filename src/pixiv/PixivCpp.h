#pragma once

#include "../curl/CurlRequest.h"

#include <string> 
#include <json/value.h> 

class PixivCpp 
{ 
    public: 
        PixivCpp(); 

        void DownloadWorks(const std::string &illust_id);

        void DownloadFavorite();
    private: 
        void PixivOauth(const std::string &username, const std::string &password);

        void PixivDownload(const Json::Value&);

        const std::string GetWorks(const std::string &illust_id);

        const std::string GetFavorite(const int page = 1, 
                const std::string per_page = "30", 
                const std::string publicity = "public");

        CurlRequest request_;
        std::string access_token_;
};  
