#pragma once
#include <windows.h>
#include <thread>

class ThreadPool
{
public:
    ThreadPool( int maxThreads = std::thread::hardware_concurrency() );
    ~ThreadPool();
    void addEvent( PTP_WORK_CALLBACK func, void* data );
private:
    PTP_POOL m_pool;
    TP_CALLBACK_ENVIRON m_callbackEnv;
    PTP_CLEANUP_GROUP m_cleanupGroup;
};

