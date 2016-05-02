#pragma once 

#include <json/value.h>
#include <json/reader.h> 
#include <json/writer.h> 
#include <string> 
#include <fstream>

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

template <typename K>
std::string getJsonValue(const Json::Value &root, const K &key) {
    return root[key].asString();
}

template <typename K, typename... KS>
std::string getJsonValue(const Json::Value &root, const K &key, const KS&... keys) {
    return getJsonValue(root[key], keys...);
}

template <typename K>
std::string parseJson(const std::string &json, const K &key) {
    Json::Value root = parseJson(json);
    return root[key].asString();
}

template <typename K, typename... KS>
std::string parseJson(const std::string &json, const K &key, const KS&... keys) {
    Json::Value root = parseJson(json);
    return getJsonValue(root[key], keys...);
}
