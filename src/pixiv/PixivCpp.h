#pragma once

#include "../curl/CurlRequest.h"
#include <string> 

class PixivCpp 
{ 
    public: 
        PixivCpp(const std::string username, const std::string password); 

        const std::string works(const std::string &illust_id);
        void DownloadWorks(const std::string &illust_id);

    private: 
        void download_(const CurlUrl &url, const std::string &file_name);

        CurlRequest request_;
        std::string access_token_;
        std::string refresh_token_;
};  
