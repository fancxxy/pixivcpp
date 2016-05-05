#include "../curl/CurlRequest.h"
#include "PixivCpp.h"
#include "ParseJson.h"

#include <iostream> 
#include <fstream> 
#include <vector>

PixivCpp::PixivCpp()
{
    std::string username, password;

    std::ifstream ifile(".pixiv", std::ios::in | std::ios::binary);
    if (ifile.good()) {
        ifile >> username >> password;
    } else {
        std::cout << "input username and password: ";
        std::cin >> username >> password;

        std::ofstream ofile(".pixiv", std::ios::out | std::ios::binary);
        ofile << username << " " << password;
    }

    PixivOauth(username, password);
}

void PixivCpp::PixivOauth(const std::string &username, const std::string &password)
{
    CurlUrl url = "https://oauth.secure.pixiv.net/auth/token";

    CurlData data = {
        {"username", username},
        {"password", password},
        {"grant_type", "password"},
        {"device_token", "912f2f55833618abc5de13b27e9e0e65"},
        {"client_id", "bYGKuGVw91e0NMfPGp44euvGt59s"},
        {"client_secret", "HP3RmkgAmEGro0gn1x9ioawQE8WMfvLXDz3ZqxpK"}
    };

    CurlHeader header = {
        {"Referer", "http://www.pixiv.net/"}
    };
    
    CurlResponse response = request_.CurlPost(url, data, header);

    access_token_ =  parseJson(response.text, "response", "access_token");
}

const std::string PixivCpp::works(const std::string &illust_id) {
    CurlUrl url = "https://public-api.secure.pixiv.net/v1/works/" + illust_id + ".json";

    CurlParams params = {
        {"include_sanity_level", "true"},
        {"profile_image_sizes", "px_170x170,px_50x50"},
        {"image_sizes", "px_128x128,px_480mw,large"},
        {"include_stats", "true"}
    };

    CurlHeader header = {
        {"Referer", "http://www.pixiv.net/"},
        {"Authorization", "Bearer " + access_token_}
    };

    CurlResponse response = request_.CurlGet(url, header, params);
    return response.text;
}

void PixivCpp::DownloadWorks(const std::string &illust_id) {
    CurlUrl original_url;
    std::string file_name;
    std::string response_json = works(illust_id);            

    //CurlUrl original_url = parseJson(response_json, "response", 0, "image_urls", "large");
    Json::Value result = parseJson(response_json);
    int page_count = std::stoi(result["response"][0]["page_count"].asString());

    std::cout << "page_count: " << page_count << std::endl;
    for (int i = 0; i < page_count; i++) {
        if (page_count == 1)
            original_url = result["response"][0]["image_urls"]["large"].asString();
        else
            original_url = result["response"][0]["metadata"]["pages"][i]["image_urls"]["large"].asString();

        std::cout << i + 1 << ": " << original_url << std::endl;

        file_name = original_url.substr(original_url.rfind("/")+1);
        PixivDownload(original_url, file_name);
    }
}

void PixivCpp::PixivDownload(const CurlUrl &url, const std::string &file_name) {
    CurlHeader header = {
        {"Referer", "http://www.pixiv.net/"},
    };

    CurlResponse response = request_.CurlGet(url, header, {}, file_name);
}
