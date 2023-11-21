#include "web_site.h"

WebSite::WebSite()
    : m_hostName( "" )
    , m_defaultFile( "index.html" )
{}

 WebSite::WebSite( const std::string& hostName, 
                    const std::string& rootDirectory, 
                    const std::string& defaultFile, 
                    const std::list<Type>& supportedTypes,
                    const std::string& errorDirectory )
    : m_hostName( hostName )
    , m_rootDirectory( rootDirectory )
    , m_defaultFile( defaultFile )
    , m_supportedExtentions( supportedTypes )
    , m_errorDirectory( errorDirectory )
{}

const std::string& WebSite::getHostName()
{
    return m_hostName;
}

void WebSite::setConnection( Net::Connection& newConnection )
{


    if ( !newConnection.isValid() || newConnection.getDataSize() == 0 )
    {
        return;
    }
    Http::HttpRequest req( newConnection.getData(), newConnection.getDataSize() );
    if ( req.getMethod() != "GET" )
    {
        Http::HttpResponse resp;
        if ( !m_errorDirectory.empty() )
        {
            std::string errorFileName = m_errorDirectory;
            errorFileName += std::to_string((int)Http::EHttpCode::eHC_InvalidMethod) + ".html";
            resp.setDataFromFile( errorFileName, "utf-8" );
        }
        resp.setRespCode( Http::EHttpCode::eHC_InvalidMethod );
        Http::RawHttpResponse raw = resp.getRawResponse();
        newConnection.sendData( raw.getData(), raw.getSize() );
        newConnection.disconnect();
        return;
    }
    std::string fileName = m_rootDirectory + req.getPath();

    if (req.getPath() == "/")
    {
        fileName = m_rootDirectory + m_defaultFile;
    }

    std::string ext = getFileExtention( fileName );
    std::string contentType;

    for ( Type& tp : m_supportedExtentions )
    {
        if (!contentType.empty())
        {
            break;
        }

        for ( std::string& fileExt : tp.fileExtentions )
        {
            if (fileExt == ext)
            {
                contentType = tp.mime_notations;
                break;
            }
        }
    }

    if ( contentType.empty() )
    {
        Http::HttpResponse resp;
        if ( !m_errorDirectory.empty() )
        {
            std::string errorFileName = m_rootDirectory;
            errorFileName += m_errorDirectory;
            contentType = "text/html";
            errorFileName += std::to_string( (int)Http::EHttpCode::eHC_InvalidResourceType ) + ".html";
            resp.setDataFromFile(errorFileName, "utf-8");
        }
        resp.setContentType( contentType );
        resp.setRespCode( Http::EHttpCode::eHC_InvalidResourceType );
        Http::RawHttpResponse raw = resp.getRawResponse();
        newConnection.sendData( raw.getData(), raw.getSize() );
        newConnection.disconnect();
        return;
    }

    Http::HttpResponse resp;
    resp.setContentType( contentType );
    resp.setDataFromFile( fileName );

    if (resp.getCode() == Http::EHttpCode::eHC_NotFound)
    {
        if (!m_errorDirectory.empty())
        {
            contentType = "text/html";
            resp.setContentType( contentType );
            std::string errorFileName = m_rootDirectory;
            errorFileName += m_errorDirectory;
            errorFileName += std::to_string( ( int )Http::EHttpCode::eHC_NotFound ) + ".html";
            resp.setDataFromFile( errorFileName );
        }
    }

    Http::RawHttpResponse raw = resp.getRawResponse();
    newConnection.sendData( raw.getData(), raw.getSize() );

    newConnection.disconnect();
}

void WebSite::shutdown()
{
    m_connection.closeSock();
}
