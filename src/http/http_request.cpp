#include "http_request.h"

Http::EHttpVersion Http::HttpRequest::getVersion() const
{
    return m_version;
}

const std::string& Http::HttpRequest::getMethod() const
{
    return m_reqMethod;
}

const std::string& Http::HttpRequest::getHost() const
{
    return m_reqHost;
}

const std::string& Http::HttpRequest::getPath() const
{
    return m_reqPath;
}

const std::list<Http::HttpHeader>& Http::HttpRequest::getAdditionalHeaders() const
{
    return m_additionalHeaders;
}

Http::HttpRequest::HttpRequest( const char* request, size_t requestLen )
{
    std::list<std::string> lines = splitString( request, requestLen, "\r\n" );
    if( lines.empty() )
    {
        m_version = EHttpVersion::eHV_Http1_1;
        m_reqMethod = "";
        m_reqHost = "";
        m_reqPath = "";
    }
    else
    {

        std::list<std::string> splitFirstLine = splitString( lines.front(), " " );
        m_reqMethod = splitFirstLine.front();
        splitFirstLine.pop_front();
        m_reqPath = splitFirstLine.front();
        splitFirstLine.pop_front();
        httpVersionStringConverter( m_version, splitFirstLine.front() );

        lines.pop_front();

        for( std::string& line : lines )
        {
            if( line.empty() )
            {
                break;
            }
            HttpHeader header( line );

            if( header.getHeaderName() == "Host" )
            {
                std::string hostName = splitString( header.getHeaderValues().front(), ":" ).front();
                m_reqHost = hostName;
            }
            else
            {
                m_additionalHeaders.push_back( header );
            }
        }
    }
}


