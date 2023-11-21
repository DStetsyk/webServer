#include "server.h"

Server::Server( const std::string& configFileName )
    : m_terminator( false )
    , m_port( "80" )
    , m_defaultRootDir( ".\\" )
    , m_listenSocket( SOCKET() )
{
    configure( configFileName );
}

bool Server::start()
{
    m_terminator = false;
    if( m_webSites.empty() )
    {
        return false;
    }

        auto servFunction = ( [ this ]() 
                              { 
                                  serverFunction( this );
                              } );

        m_serverThread = new std::thread( servFunction );
        return true;
    return false;
}

void Server::stop()
{
    m_terminator = true;
    
    for( auto& webSite : m_webSites )
    {
        webSite.second->shutdown();
    }

    m_listenSocket.closeSock();
}

bool Server::reload( const std::string& configFileName )
{
    stop();
    m_serverThread->join();
    for( auto& webSite : m_webSites )
    {
        delete webSite.second;
    }
    m_webSites.clear();
    configure( configFileName );
    return start();
}

void Server::configure( const std::string& configFileName )
{
    std::string defaultHostName;
    defaultHostName.resize( MAX_STRING_LENGTH, '\0' );
    std::string port;
    port.resize( MAX_STRING_LENGTH, '\0' );
    std::list<std::string> registeredSites;
    std::list<WebSite::Type> types;
    int readLength = 0;

    readLength = GetPrivateProfileStringA( "General", 
                                            "DefaultHostName", 
                                            "", 
                                            defaultHostName.data(), 
                                            defaultHostName.capacity(), 
                                            configFileName.c_str() );
    defaultHostName.resize( readLength );
    m_defaultHostName = defaultHostName;
    if( readLength == 0 )
    {
        if( GetLastError() == ERROR_FILE_NOT_FOUND )
        {
            std::cout << "Cannot open " << configFileName << std::endl;
            return;
        }
        std::cout << "Default host name is not defined." << std::endl;
    }
    else
    {
        std::cout << "Default host name is set to: " << defaultHostName << std::endl;
    }
    readLength = GetPrivateProfileStringA( "General", 
                                            "Port", 
                                            "", 
                                            port.data(), 
                                            port.capacity(), 
                                            configFileName.c_str() );
    bool isPortValid = true;
    for( int i = 0; i < readLength; ++i )
    {
        if( !isdigit( port[i] ) )
        {
            isPortValid = false;
        }
    }

    if( isPortValid )
    {
        m_port = port;
    }
    else
    {
        std::cout << "Port value is invalid." << std::endl;
        return;
    }

    if( readLength == 0 )
    {
        std::cout << "Port is not specified. Server can't be configured." << std::endl;
        return;
    }
    else
    {
        std::cout << "Port is set to: " << port << std::endl;
    }
    port.resize( readLength );
    std::string strRegisteredSites;
    strRegisteredSites.resize( MAX_STRING_LENGTH, '\0' );
    readLength = GetPrivateProfileStringA( "General",
                                            "Register",
                                            "",
                                            strRegisteredSites.data(),
                                            strRegisteredSites.capacity(),
                                            configFileName.c_str() );
    strRegisteredSites.resize( readLength );
    registeredSites = splitString( strRegisteredSites, "," );

    std::string strTypes;
    strTypes.resize( MAX_STRING_LENGTH, '\0' );
    readLength = GetPrivateProfileSectionA( "Types",
                                            strTypes.data(),
                                            strTypes.capacity(),
                                            configFileName.c_str() );
    strTypes.resize( readLength );

    std::list<std::string> strTypesList = splitString( strTypes.c_str(), strTypes.length(), "\0" );
    for( std::string& strType : strTypesList )
    {
        WebSite::Type t;
        std::list<std::string> splittedType = splitString( strType, "=" );
        if( !splittedType.empty() )
        {
            t.mime_notations = splittedType.front();
            t.fileExtentions = splitString( splittedType.back(), "," );
            types.push_back( t );
        }
    }


    for( std::string& hostName : registeredSites )
    {
        std::string rootDirectory;
        rootDirectory.resize( MAX_STRING_LENGTH, '\0' );
        std::string errorDirectory;
        errorDirectory.resize( MAX_STRING_LENGTH, '\0' );
        std::string defaultFile;
        defaultFile.resize( MAX_STRING_LENGTH, '\0' );
        std::list<WebSite::Type>supportedTypes;

        readLength = GetPrivateProfileStringA( hostName.c_str(),
                                                "RootDirectory",
                                                ".\\",
                                                rootDirectory.data(),
                                                rootDirectory.capacity(),
                                                configFileName.c_str() );
        if( readLength == 0 )
        {
            std::cout << "Set root directroy for: " << hostName << std::endl;
        }
        else if( rootDirectory.back() != '\\' )
        {
            rootDirectory += '\\';
        }
        rootDirectory.resize( readLength );
        readLength = GetPrivateProfileStringA( hostName.c_str(),
                                        "DefaultFile",
                                        "index.html",
                                        defaultFile.data(),
                                        defaultFile.capacity(),
                                        configFileName.c_str() );
        defaultFile.resize( readLength );
        int err = GetLastError();
        readLength = GetPrivateProfileStringA( hostName.c_str(),
                                                "ErrorDirectory",
                                                "",
                                                errorDirectory.data(),
                                                errorDirectory.capacity(),
                                                configFileName.c_str() );
        errorDirectory.resize( readLength );
        std::string strSupportedTypes;
        strSupportedTypes.resize( MAX_STRING_LENGTH, '\0' );
        readLength = GetPrivateProfileStringA( hostName.c_str(),
                                                "SupportedTypes", 
                                                "", 
                                                strSupportedTypes.data(), 
                                                strSupportedTypes.capacity(), 
                                                configFileName.c_str() );
        strSupportedTypes.resize( readLength );

        std::list<std::string> strSupportedTypesList = splitString( strSupportedTypes.c_str(), 
                                                                    strSupportedTypes.length(), 
                                                                    "," );

        for( std::string& strType : strSupportedTypesList )
        {
            WebSite::Type supportedType = {};
            for( const WebSite::Type& t : types )
            {
                if( t.mime_notations == strType )
                {
                    supportedType.mime_notations = t.mime_notations;
                    supportedType.fileExtentions = t.fileExtentions;
                    supportedTypes.push_back( supportedType );
                    break;
                }
            }
        }
        if( !hostName.empty() )
        {
            m_webSites.emplace( hostName, new WebSite( hostName, 
                                                    rootDirectory, 
                                                    defaultFile, 
                                                    supportedTypes, 
                                                    errorDirectory ) );
            std::cout << "Registered: " << hostName << std::endl;
        }
    }
}

bool Server::serverFunction( Server* pThis )
{
    addrinfo ai = {};
    ai.ai_family = AF_INET;
    ai.ai_socktype = SOCK_STREAM;
    ai.ai_protocol = IPPROTO_TCP;
    ai.ai_flags = AI_PASSIVE;

    int err;
    addrinfo* result;
    err = getaddrinfo( NULL, pThis->m_port.c_str(), &ai, &result );
    if( err != 0 )
    {
        return false;
    }
    pThis->m_listenSocket.getSocket() = socket( result->ai_family, result->ai_socktype, result->ai_protocol );
    if( pThis->m_listenSocket.getSocket() == INVALID_SOCKET )
    {
        return false;
    }

    err = bind( pThis->m_listenSocket.getSocket(), result->ai_addr, result->ai_addrlen );
    if( err == SOCKET_ERROR )
    {
        return false;
    }

    while( !pThis->m_terminator.load() )
    {
        Net::Connection* newConnection = new Net::Connection();

        if( ! newConnection->connect( pThis->m_port, 
                                        pThis->m_listenSocket.getSocket() ) )
        {
            delete newConnection;
            continue;
        }
        TaskData* acceptData = new TaskData;
        acceptData->pThis = pThis;
        acceptData->data = newConnection;
        auto acceptCon = []( PTP_CALLBACK_INSTANCE inst,
            PVOID context, 
            PTP_WORK work )
        {
            Server* pThis = ( ( TaskData* ) context )->pThis;
            Net::Connection* connection = ( Net::Connection* ) ( ( TaskData* ) context )->data;
            connection->readData( 1024 );
            Http::HttpRequest req( connection->getData(), 
                connection->getDataSize() );


            auto search = pThis->m_webSites.find( req.getHost() );
            if( search != pThis->m_webSites.end() )
            {
                pThis->m_webSites[req.getHost()]->setConnection( *connection );
            }
            else if( !pThis->m_defaultHostName.empty() )
            {
                pThis->m_webSites[pThis->m_defaultHostName]->setConnection( *connection );
            }
            delete connection;
            delete context;
        };
        pThis->m_processingThreads.addEvent( acceptCon, acceptData );
    }

}
