#include "utils.h"

std::list<std::string> splitString( const std::string& string, const std::string spliter )
{

    return splitString( string.c_str(), string.length(), spliter );
}

std::list<std::string> splitString( const char* string, size_t strLen, const std::string spliter )
{
    std::list<std::string> result;
    std::string bufString;
    for( int i = 0; i < strLen; ++i )
    {
        if( string[i] == spliter[0] )
        {
            int savedValue = i;
            bool isFits = true;
            for( int j = 0; j < spliter.length() && isFits; ++j, ++i )
            {
                if( string[i] != spliter[j] )
                {
                    isFits = false;
                }
            }

            if( !isFits )
            {
                i = savedValue;
                bufString += string[i];
            }
            else if( !bufString.empty() )
            {
                result.push_back( bufString );
                bufString = "";
                --i;
            }
        }
        else
        {
            bufString += string[i];
        }
    }

    if( !bufString.empty() )
    {
        result.push_back( bufString );
    }

    return result;
}

std::string getFileExtention( const std::string& fileName )
{
    std::string bufString;
    for( int i = fileName.length() - 1; i >= 0; --i )
    {
        if( fileName[i] == '.' )
        {
            return bufString;
        }
        else
        {
            bufString.insert( 0, 1, fileName[i] );
        }
    }

    return std::string();
}

