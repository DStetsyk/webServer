#pragma once
#include <string>
#include <list>

std::list<std::string> splitString( const std::string& string, const std::string spliter );
std::list<std::string> splitString( const char* string, size_t strLen, const std::string spliter );
std::string getFileExtention( const std::string& fileName );
