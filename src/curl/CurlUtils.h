#pragma once 

#include <string>
#include <sstream>
#include <cctype>
#include <iomanip>
#include <map>
#include <vector>

const std::string urlEncode(const std::string &str) {
    std::ostringstream escaped;
    escaped.fill('0');
    escaped << std::hex;

    for (auto c : str) { 
        if (std::isalnum(c) || c == '-' || c == '.' || c == '_' || c == '~') {
            escaped << c;
        } else {
            escaped << std::uppercase;
            escaped << "%" << std::setw(2) << static_cast<int>(c);
            escaped << std::nouppercase;
        }
    } 
    return escaped.str();
}

const std::string mergeParams(const std::map<std::string, std::string> &params) {
    std::string content = "";
    for (auto& e : params) { 
        if (!content.empty()) {
            content += "&";
        }
        content += e.first + "=" + e.second;
    } 
    return content;
}

const std::vector<std::string> splitString(const std::string &s, char delim) {
    std::vector<std::string> elems;
    std::stringstream stream(s);
    std::string item;
    while (std::getline(stream, item, delim)) {
        elems.push_back(item);
    }

    return elems;
}

template <typename T>
std::size_t writeStream(void *ptr, std::size_t size, std::size_t nmemb, T *stream) {
    T *data = stream;
    size_t count = size * nmemb;
    data->write(static_cast<const char*>(ptr), static_cast<std::streamsize>(count));
    return count;
}
