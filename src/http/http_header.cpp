#include "http_header.h"

Http::HttpHeader::HttpHeader( const std::string& line )
{
    std::list<std::string> splittedLine = splitString( line, ": " );
    if( !splittedLine.empty() )
    {
        m_headerName = splittedLine.front();
        m_headerValues = splitString( splittedLine.back(), "," );
    }
}

Http::HttpHeader::HttpHeader( const std::string& name, const std::list<std::string>& values )
{
    m_headerName = name;
    m_headerValues = values;
}

const std::string& Http::HttpHeader::getHeaderName() const
{
    return m_headerName;
}

const std::list<std::string>& Http::HttpHeader::getHeaderValues() const
{
    return m_headerValues;
}