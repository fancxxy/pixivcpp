#pragma once

#include "../curl/CurlRequest.h"
#include <string> 

class PixivCpp 
{ 
    public: 
        PixivCpp(); 

        const std::string works(const std::string &illust_id);
        void DownloadWorks(const std::string &illust_id);


    private: 
        void PixivOauth(const std::string &username, const std::string &password);
        void PixivDownload(const CurlUrl &url, const std::string &file_name);

        CurlRequest request_;
        std::string access_token_;
};  
