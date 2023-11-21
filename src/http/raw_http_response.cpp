#include "raw_http_response.h"

Http::RawHttpResponse::RawHttpResponse()
    : m_data( nullptr )
    , m_dataSize( 0 )
{}

Http::RawHttpResponse::RawHttpResponse( char* data, size_t dataSize )
    : m_data( data )
    , m_dataSize( dataSize )
{}

Http::RawHttpResponse::RawHttpResponse( const RawHttpResponse& obj )
{
    if( obj.m_data == nullptr )
    {
        m_data = nullptr;
        m_dataSize = 0;
    }
    else
    {
        m_data = new char[obj.m_dataSize];
        m_dataSize = obj.m_dataSize;
        memcpy( m_data, obj.m_data, m_dataSize );
    }
}

Http::RawHttpResponse::RawHttpResponse( RawHttpResponse&& rv )
{
    if( this != &rv )
    {
        m_dataSize = rv.m_dataSize;
        m_data = rv.m_data;
        rv.m_data = nullptr;
        rv.m_dataSize = 0;
    }
}

Http::RawHttpResponse::~RawHttpResponse()
{
    if( m_data != nullptr )
    {
        delete[] m_data;
    }
    m_dataSize = 0;
}

const char* Http::RawHttpResponse::getData() const
{
    return m_data;
}

size_t Http::RawHttpResponse::getSize() const
{
    return m_dataSize;
}