#include "EG_StringInternal.hpp"

using namespace std;
using namespace EbonGL;

EG_StringInternal::EG_StringInternal(void)
{
}//constructor

EG_StringInternal::~EG_StringInternal(void)
{
}//destructor

unsigned int EG_StringInternal::size(void)
{
    return static_cast<unsigned int>(theString.size());
}//size

char EG_StringInternal::getChar(unsigned int index)
{
    return theString.at(index);
}//getChar

void EG_StringInternal::setChar(unsigned int index,
                                char newValue)
{
    theString.at(index) = newValue;
}//setChar

void EG_StringInternal::clear(void)
{
    theString.clear();
}//clear

void EG_StringInternal::append(char newValue)
{
    theString.push_back(newValue);
}//append

const char* EG_StringInternal::toCString(void)
{
    return theString.c_str();
}//toCString

void EG_StringInternal::copyFrom(EG_StringInternal *source)
{
    theString.clear();
    theString.append(source->theString);
}//copyFrom

void EG_StringInternal::appendCString(const char *source)
{
    theString.append(source);
}//appendCString
