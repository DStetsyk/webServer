#include <iostream>
#include <conio.h>
#include <atomic>
#include <process.h>
#include <thread>

#include "connection.h"
#include "http\http_request.h"
#include "server.h"
#include "web_site.h"

int main( int argc, char** argv )
{
    if( argc != 2 )
    {
        std::cout << "Usage: " << argv[0] << " <path-to-config>" << std::endl;
        return 1;
    }

    std::string configFile = argv[1];

    Server server( configFile );

    bool terminate = false;
    if( !server.start() )
    {
        std::cout << "Cannot start server. Invalid config file." << std::endl;
        terminate = true;
    }

    while( !terminate )
    {
        char ch = _getch();
        switch( ch )
        {
           case 'q':
           case 'Q':
           {
               server.stop();
               terminate = true;
               break;
           }
           case 'r':
           case 'R':
           {
               if( server.reload( configFile ) )
               {
                   std::cout << "Server restarted succesfully." << std::endl;
               }
               else
               {
                   std::cout << "Restart failed. Terminating server" << std::endl;
                   terminate = true;
               }
               break;
           }
        }
    } 


    return 0;
    
}
