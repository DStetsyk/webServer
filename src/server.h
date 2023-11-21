#pragma once
#include <atomic>
#include <string>
#include <thread>
#include <unordered_map>
#include <deque>

#include "web_site.h"
#include "thread_pool.h"

const int ACCEPTING_THREADS = 3;

class Server
{
public:
    Server( const std::string& configFileName );
    bool start();
    void stop();
    bool reload( const std::string& configFileName );
private:

    struct TaskData
    {
        Server* pThis;
        void* data;
    };

    std::unordered_map<std::string, WebSite*> m_webSites;
    std::thread* m_serverThread;
    ThreadPool m_processingThreads;
    std::atomic<bool> m_terminator;

    std::string m_port;
    std::string m_defaultHostName;
    std::string m_defaultRootDir;

    Net::Connection m_listenSocket;

    void configure( const std::string& configFileName );
    static bool serverFunction( Server* pThis );

};