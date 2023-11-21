#pragma once
#include <string>
#include <list>
#include <unordered_map>
#include <iostream>

#include "connection.h"
#include "http/http.h"
#include "http/http_header.h"
#include "http/http_request.h"
#include "http/http_response.h"
#include "http/raw_http_response.h"
#include "utils.h"

const unsigned int MAX_STRING_LENGTH = 1024;

class WebSite
{
public:

    struct Type 
    {
        std::string mime_notations;
        std::list<std::string> fileExtentions;
    };

    WebSite();
    WebSite( const std::string& hostName, 
                const std::string& rootDirectory, 
                const std::string& defaultFile, 
                const std::list<Type>& supportedTypes,
                const std::string& errorDirectory );
    const std::string& getHostName();
    void setConnection( Net::Connection& connection );
    void shutdown();

private:
    enum class EBrackets
    {
        eB_Undefined,
        eB_Header,
        eB_Block,
        eB_Literal
    };

    std::string m_hostName;
    std::string m_rootDirectory;
    std::string m_defaultFile;
    std::string m_errorDirectory;
    std::list<Type> m_supportedExtentions;
    Net::Connection m_connection;

};
