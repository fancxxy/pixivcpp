#pragma once

#include "CurlType.h"

#include <memory>
#include <functional>

class CurlRequest 
{ 
    public: 
        CurlRequest();
        CurlResponse CurlGet(const CurlUrl &url, const CurlHeader &header = {}, 
                const CurlParams &params = {}, const std::string &file = "");
        CurlResponse CurlPost(const CurlUrl &url, const CurlData &data, 
                const CurlHeader &header = {}, const CurlParams &params = {});

    private: 
        static CurlContainer* newContainer();
        static void deleteContainer(CurlContainer *container);

        void setHeader(const CurlHeader &header);
        void setData(const CurlData &data);
        void setGet();
        CurlResponse makeRequest(const CurlUrl &url, const CurlParams &params, const std::string &file);

        std::unique_ptr<CurlContainer, std::function<void(CurlContainer*)>> curl_;
}; 
