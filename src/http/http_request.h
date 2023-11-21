#pragma once
#include "http.h"
#include "http_header.h"

namespace Http
{
    class HttpRequest
    {
    public:
        HttpRequest( const char* request, size_t requestLen );
        EHttpVersion getVersion() const;
        const std::string& getMethod() const;
        const std::string& getHost() const;
        const std::string& getPath() const;
        const std::list<HttpHeader>& getAdditionalHeaders() const;
    private:
        EHttpVersion m_version;
        std::string m_reqMethod;
        std::string m_reqHost;
        std::string m_reqPath;
        std::list<HttpHeader> m_additionalHeaders;
    };
}
