#include "thread_pool.h"

ThreadPool::ThreadPool( int maxThreads )
{
    m_pool = CreateThreadpool( NULL );
    InitializeThreadpoolEnvironment( &m_callbackEnv );
    m_cleanupGroup = CreateThreadpoolCleanupGroup();
    SetThreadpoolCallbackPool( &m_callbackEnv, m_pool );
    SetThreadpoolCallbackCleanupGroup( &m_callbackEnv, m_cleanupGroup, NULL );
    SetThreadpoolThreadMaximum( m_pool, maxThreads );
}

ThreadPool::~ThreadPool()
{
    CloseThreadpool( m_pool );
}

void ThreadPool::addEvent( PTP_WORK_CALLBACK function, void* data )
{
    TP_WORK* worker = CreateThreadpoolWork( function, data, &m_callbackEnv );
    SubmitThreadpoolWork( worker );
}
