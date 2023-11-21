#include "http_response.h"

Http::HttpResponse::HttpResponse()
    : m_version( EHttpVersion::eHV_Http1_1 )
    , m_respCode( EHttpCode::eHC_Ok )
    , m_respContentType( "" )
    , m_contentCharset( "" )
    , m_data( nullptr )
    , m_dataSize( 0 )
{
    m_additionalHeaders = new std::list<HttpHeader>;
}


Http::HttpResponse::HttpResponse( EHttpCode respCode, 
    std::string contentType ) 
    : m_version( Http::EHttpVersion::eHV_Http1_1 )
    , m_respCode( respCode )
    , m_respContentType( contentType )
    , m_data( nullptr )
    , m_dataSize( 0 )
{
    m_additionalHeaders = new std::list<HttpHeader>;
}

Http::HttpResponse::~HttpResponse()
{
    if( m_data != nullptr )
    {
        delete[] m_data;
        m_data = nullptr;
    }
    if( m_additionalHeaders != nullptr )
    {
        delete m_additionalHeaders;
    }
}

void Http::HttpResponse::setRespCode( EHttpCode code )
{
    m_respCode = code;
}

void Http::HttpResponse::setContentType( const std::string& contentType )
{
    m_respContentType = contentType;
}

const std::list<Http::HttpHeader>* Http::HttpResponse::getAdditionalHeaders() const
{
    return m_additionalHeaders;
}

void Http::HttpResponse::setData( char* data, size_t dataSize )
{
    delete[] m_data;
    m_data = data;
    m_dataSize = dataSize;
}

bool Http::HttpResponse::setDataFromFile( const std::string& fileName, std::string charset )
{
    if( !charset.empty() )
    {
        m_contentCharset = charset;
    }
    else if( m_respContentType.substr( 0, 4 ) == "text" )
    {
        m_contentCharset = "utf-8";
    }

    std::ifstream file( fileName, std::ios_base::binary );
    if( !file )
    {
        if( m_data != nullptr )
        {
            delete[] m_data;
        }
        m_data = nullptr;
        m_dataSize = 0;
        m_respCode = EHttpCode::eHC_NotFound;
        return false;
    }

    file.seekg( 0, file.end );
    int fileSize = file.tellg();
    file.seekg( 0, file.beg );

    if( m_data != nullptr )
    {
        delete[] m_data;
    }
    m_data = new char[fileSize];

    file.read( m_data, fileSize );
    m_dataSize = fileSize;
    file.close();
    return true;
}

Http::EHttpCode Http::HttpResponse::getCode() const
{
    return m_respCode;
}

Http::RawHttpResponse Http::HttpResponse::getRawResponse() const
{
    std::string str;
    httpVersionStringConverter( EHttpVersion::eHV_Http1_1, str );
    str += " " + std::to_string( ( int ) m_respCode );
    str += "\r\n";
    if( !m_respContentType.empty() )
    {
        std::string contentType = m_respContentType;
        if( !m_contentCharset.empty() )
        {
            contentType += "; charset=" + m_contentCharset;
        }
        m_additionalHeaders->push_front( { "Content-Type", 
            { contentType } } );
    }
    m_additionalHeaders->push_front( { "Content-Length", 
        { std::to_string( m_dataSize ) } } );

    for( HttpHeader& header : *m_additionalHeaders )
    {
        str += header.getHeaderName() + ": ";

        int counter = header.getHeaderValues().size();
        for( std::string value : header.getHeaderValues() )
        {
            str += value;
            --counter;
            if( counter )
            {
                str += ", ";
            }
        }
        str += "\r\n";
    }
    str += "\r\n";

    char* result = new char[ str.length() + m_dataSize ];

    memcpy( result, str.data(), str.length() );

    size_t writtenData = 0;
    char* writePointer = result + str.length();
    size_t readSize = 1024;
    while( writtenData != m_dataSize )
    {
        if( ( m_dataSize - writtenData ) < readSize )
        {
            readSize /= 2;
            continue;
        }

        memcpy( writePointer, ( char* ) m_data + writtenData, readSize );

        writtenData += readSize;
        writePointer += readSize;
    }

    return { result, writtenData + str.length() };
}