#include "http.h"


void Http::httpVersionStringConverter( EHttpVersion httpVersion, std::string& str )
{
    if( str.empty() && httpVersion != EHttpVersion::eHV_Undefined )
    {
        switch( httpVersion )
        {
           case EHttpVersion::eHV_Http1:
           {
               str = "HTTP1";
               break;
           }
           case EHttpVersion::eHV_Http1_1:
           {
               str = "HTTP/1.1";
               break;
           }
           case EHttpVersion::eHV_Http2:
           {
               str = "HTTP/2";
               break;
           }
           case EHttpVersion::eHV_Undefined:
           {
               str = "";
               break;
           }
        }
    }
    else if( httpVersion == EHttpVersion::eHV_Undefined && !str.empty() )
    {
        if( str == "HTTP1" )
        {
            httpVersion = EHttpVersion::eHV_Http1;
        }
        else if( str == "HTTP/1.1" )
        {
            httpVersion = EHttpVersion::eHV_Http1_1;
        }
        else if( str == "HTTP/2" )
        {
            httpVersion = EHttpVersion::eHV_Http2;
        }
        else
        {
            httpVersion = EHttpVersion::eHV_Undefined;
        }
    }
}

 