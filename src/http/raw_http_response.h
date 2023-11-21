#pragma once
#include <string>

namespace Http
{
    class RawHttpResponse
    {
    public:
        RawHttpResponse();
        RawHttpResponse( char* data, size_t dataSize );
        RawHttpResponse( const RawHttpResponse& obj );
        RawHttpResponse( RawHttpResponse&& rv );
        ~RawHttpResponse();
        const char* getData() const;
        size_t getSize() const;
    private:
        char* m_data;
        size_t m_dataSize;
    };
}
