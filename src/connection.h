#pragma once

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <string>
#include <mutex>

namespace Net
{
    enum class EProtocol
    {
        eP_Http = 80,
        eP_Https = 443
    };

    class Connection
    {
    public:
        Connection();
        Connection( const SOCKET socket, size_t bufferSize = 1024 );
        ~Connection();

        Connection( const Connection& obj ) = delete;
        Connection& operator=( const Connection& obj ) = delete;
        Connection( Connection&& rv );
        void operator=( Connection&& rv );

        const char* getData() const ;
        size_t getDataSize() const;
        SOCKET& getSocket();
        size_t readData( size_t size );
        size_t sendData( const char* buf, size_t size ) const;
        bool connect( const std::string& port, const SOCKET& listenSocket );
        bool isValid() const;
        void closeSock();
        bool disconnect();

    private:
        SOCKET m_socket;
        size_t m_bufferSize;
        size_t m_messageLen;
        char* m_buffer;
        static std::mutex m_counterMutex;
        static int m_objectCounter;
    };

}
