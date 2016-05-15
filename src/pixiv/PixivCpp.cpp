#include "../curl/CurlRequest.h"
#include "PixivCpp.h"

#include <iostream> 
#include <fstream> 
#include <json/reader.h> 
#include <json/writer.h> 

Json::Value parseJson(const std::string &json) {
    Json::Value root;
    Json::Reader reader;
    reader.parse(json, root);
    return root;
}

void styleJson(const std::string &file_name, const std::string &json) {
    Json::Value root = parseJson(json);
    Json::StyledWriter writer;
    std::fstream file(file_name, std::ios::out);
    file <<  writer.write(root);
}

PixivCpp::PixivCpp()
{
    std::string username, password;

    std::ifstream ifile(".pixiv", std::ios::in | std::ios::binary);
    if (ifile.good()) {
        ifile >> username >> password; } else {
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

    access_token_ =  parseJson(response.text)["response"]["access_token"].asString();
}

const std::string PixivCpp::GetWorks(const std::string &illust_id) {
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
    styleJson(illust_id, response.text);
    return response.text;
}

const std::string PixivCpp::GetFavorite(const std::string page, 
        const std::string per_page, 
        const std::string publicity, 
        const std::string image_sizes) {
    CurlUrl url = "https://public-api.secure.pixiv.net/v1/me/favorite_works.json";

    CurlParams params = {
        {"page", page},
        {"per_page", per_page},
        {"publicity", publicity},
        {"image_sizes", image_sizes}
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
    std::string response = GetWorks(illust_id);            

    Json::Value json = parseJson(response);
    if (json["status"] == "success") {
        Json::Value works = json["response"][0];
        PixivDownload(works);
    }
}

void PixivCpp::DownloadFavorite(const std::string page, 
        const std::string per_page, 
        const std::string publicity, 
        const std::string image_sizes) {
    std::string response = GetFavorite(page, per_page, publicity, image_sizes);

    Json::Value json = parseJson(response);
    if (json["status"] == "success") {
        for (int count = 0; count < json["count"].asInt(); count++) {
            Json::Value favorite = json["response"][count]["work"];
            PixivDownload(favorite);
        }
    }
}

void PixivCpp::PixivDownload(const Json::Value &root) {
    CurlUrl url;
    CurlHeader header = {
        {"Referer", "http://www.pixiv.net/"},
    };


    std::string file_name;
    int page_count = root["page_count"].asInt(); 
    
    std::cout << "id: " << root["id"].asString() << std::endl;
    std::cout << "title: " << root["title"].asString() << std::endl;
    std::cout << "caption: " << root["caption"].asString() << std::endl;
    std::cout << "page_count: " << page_count << std::endl;

    for (int i = 0; i < page_count; i++) {
        if (page_count == 1)
            url = root["image_urls"]["large"].asString();
        else
            url = root["metadata"]["pages"][i]["image_urls"]["large"].asString();

        std::cout << i + 1 << ": " << url << std::endl;

        file_name = url.substr(url.rfind("/")+1);
        request_.CurlGet(url, header, {}, file_name);
    }
}
