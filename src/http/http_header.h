#pragma once
#include <string>
#include <list>
#include <utils.h>

namespace Http
{
    class HttpHeader
    {
    public:
        HttpHeader( const std::string& line );
        HttpHeader( const std::string& name, const std::list<std::string>& values );
        const std::string& getHeaderName() const;
        const std::list<std::string>& getHeaderValues() const;
    private:
        std::string m_headerName;
        std::list<std::string> m_headerValues;
    };  
}
