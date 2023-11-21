#include "connection.h"

int Net::Connection::m_objectCounter = 0;
std::mutex Net::Connection::m_counterMutex; 

Net::Connection::Connection()
    : m_socket( INVALID_SOCKET )
    , m_bufferSize( 1024 )
    , m_messageLen( 0 )
{
    std::scoped_lock<std::mutex> lock( m_counterMutex );
    if( m_objectCounter == 0 )
    {
        WSADATA data;
        WSAStartup( MAKEWORD( 2, 2 ), &data );
    }
    m_buffer = new char[m_bufferSize];
    ++m_objectCounter;
}

Net::Connection::Connection( const SOCKET socket, size_t bufferSize )
    : m_socket( socket )
    , m_bufferSize( bufferSize )
    , m_messageLen( 0 )
{
    if( m_objectCounter == 0 )
    {
        WSADATA data;
        WSAStartup( MAKEWORD( 2, 2 ), &data );
    }
    m_buffer = new char[bufferSize];
}

Net::Connection::~Connection()
{
    if( m_socket != INVALID_SOCKET )
    {
        closesocket( m_socket );
    }
    delete[] m_buffer;
    std::scoped_lock<std::mutex> lock( m_counterMutex );
    if( --m_objectCounter == 0 )
    {
        WSACleanup();
    }
}

Net::Connection::Connection( Connection&& rv )
{
    if( this != &rv )
    {
        m_socket = rv.m_socket;
        m_bufferSize = rv.m_bufferSize;
        m_messageLen = rv.m_messageLen;
        m_buffer = rv.m_buffer;
        rv.m_buffer = nullptr;
        rv.m_socket = INVALID_SOCKET;
        rv.m_bufferSize = 0;
        rv.m_messageLen = 0;
    }
}

void Net::Connection::operator=( Connection&& rv )
{
    if( this != &rv )
    {    
        if( m_socket != INVALID_SOCKET )
        {
            closesocket( m_socket );
        }
        m_socket = rv.m_socket;
        m_bufferSize = rv.m_bufferSize;
        m_messageLen = rv.m_messageLen;
        m_buffer = rv.m_buffer;
        rv.m_buffer = nullptr;
        rv.m_socket = INVALID_SOCKET;
        rv.m_messageLen = 0;
        rv.m_bufferSize = 0;
    }
}

const char* Net::Connection::getData() const
{
    return m_buffer;
}

size_t Net::Connection::getDataSize() const
{
    return m_messageLen;
}

SOCKET& Net::Connection::getSocket()
{
    return m_socket;
}

size_t Net::Connection::readData( size_t size )
{
    size_t dataSize = 0;
    if( m_socket == INVALID_SOCKET )
    {
        return 0;
    }
    dataSize = recv( m_socket, m_buffer, m_bufferSize, 0 );
    if( dataSize > m_bufferSize )
    {
        m_messageLen = 0;
    }
    else
    {
        m_messageLen = dataSize;
    }
    
    return dataSize;
}

size_t Net::Connection::sendData( const char* buf, size_t size ) const
{
    if( m_socket == INVALID_SOCKET ||
        buf == nullptr ||
        size == 0 )
    {
        return 0;
    }
    int result = 0;
    size_t writtedData = send( m_socket, buf, size, 0 );
    return writtedData;
}

bool Net::Connection::connect( const std::string& port, const SOCKET& listenSocket )
{
    int err;
    err = listen( listenSocket, SOMAXCONN );
    if( err == SOCKET_ERROR )
    {
        return false;
    }

    m_socket = accept( listenSocket, NULL, NULL );
    if( m_socket == INVALID_SOCKET )
    {
        return false;
    }


    return true;
}

bool Net::Connection::isValid() const
{
    if( m_socket != INVALID_SOCKET )
    {
        return true;
    }
    return false;
}

void Net::Connection::closeSock()
{
    if( m_socket == INVALID_SOCKET )
    {
        return;
    }
    closesocket( m_socket );
    m_socket = INVALID_SOCKET;
}

bool Net::Connection::disconnect()
{
    int err = 0;
    if( m_socket == INVALID_SOCKET )
    {
        return false;
    }
    err = shutdown( m_socket, SD_SEND );
    closesocket( m_socket );
    m_socket = INVALID_SOCKET;
    if( err == SOCKET_ERROR )
    {
        return false;
    }

    return true;
}

