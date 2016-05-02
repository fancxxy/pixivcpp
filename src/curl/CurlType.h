#pragma once

#include <curl/curl.h>
#include <map>
#include <string>
#include <initializer_list>

using CurlHeader = std::map<std::string, std::string>;
using CurlParams = std::map<std::string, std::string>;
using CurlCookies = std::map<std::string, std::string>;
using CurlUrl = std::string;

struct CurlContainer 
{
    CURL *handle;
    struct curl_slist *chunk;
    char error[CURL_ERROR_SIZE];
};

struct CurlData
{
    CurlData(const std::initializer_list<std::pair<const std::string, std::string>> &p_data) : 
        data(p_data), content() {} 

    std::map<std::string, std::string> data;
    mutable std::string content;
};

struct CurlResponse
{
    CurlResponse(const long &p_status_code, 
                 const std::string &p_text, 
                 const CurlCookies &p_cookies = CurlCookies()) :
        status_code(p_status_code), text(p_text), cookies(p_cookies) { }

    CurlResponse(long &&p_status_code, 
                 std::string &&p_text,
                 CurlCookies &&p_cookies = CurlCookies()) noexcept :
        status_code(p_status_code), text(p_text), cookies(p_cookies) { }

    long status_code;
    std::string text;
    CurlCookies cookies;
    //CurlError error;
};
