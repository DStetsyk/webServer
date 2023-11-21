#pragma once
#include "http.h"
#include "http_header.h"
#include "raw_http_response.h"

namespace Http
{
    class HttpResponse
    {
    public:
        HttpResponse();
        HttpResponse( EHttpCode respCode, 
            const std::string contentType = "" );
        ~HttpResponse();
        void setRespCode( EHttpCode code );
        void setContentType( const std::string& contentType );
        void setData( char* data, size_t dataSize );
        bool setDataFromFile( const std::string& fileName,
            std::string charset = "" );
        EHttpCode getCode() const;
        const std::list<HttpHeader>* getAdditionalHeaders() const;
        RawHttpResponse getRawResponse() const;
    private:
        EHttpVersion m_version;
        EHttpCode m_respCode;
        std::string m_respContentType;
        std::list<HttpHeader>* m_additionalHeaders;
        std::string m_contentCharset;
        char* m_data;
        size_t m_dataSize;
    };
}
