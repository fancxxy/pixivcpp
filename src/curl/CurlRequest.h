#pragma once

#include "CurlType.h"

#include <memory>
#include <functional>

class CurlRequest 
{ 
    public: 
        CurlRequest();
        CurlResponse CurlGet(CurlUrl url, CurlHeader header = {}, CurlParams params = {});
        CurlResponse CurlPost(CurlUrl url, CurlData data, CurlHeader header = {}, CurlParams params = {});

    private: 
        static CurlContainer* newContainer();
        static void deleteContainer(CurlContainer *container);

        void setHeader(const CurlHeader &header);
        void setData(const CurlData &data);
        void setGet();
        CurlResponse makeRequest(const CurlUrl &url, const CurlParams &params);

        std::unique_ptr<CurlContainer, std::function<void(CurlContainer*)>> curl_;
}; 
