#pragma once
#include <string>
#include <list>
#include <fstream>

#include "utils.h"

namespace Http
{

   enum class EHttpVersion
   {
       eHV_Undefined,
       eHV_Http1,
       eHV_Http1_1,
       eHV_Http2
   };

   enum class EHttpCode
   {
       eHC_Ok = 200,
       eHC_InvalidMethod = 400,
       eHC_InvalidResourceType = 403,
       eHC_NotFound = 404
   };

   void httpVersionStringConverter( EHttpVersion httpVersion, std::string& str );
}
