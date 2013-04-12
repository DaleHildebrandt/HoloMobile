#include "EG_String.hpp"
#include "EG_StringInternal.hpp"
#include <sstream>

using namespace EbonGL;
using namespace std;

EG_String::EG_String(void)
{
    internalString = new EG_StringInternal;
}//constructor

EG_String::EG_String(const char *source)
{
    unsigned int readIndex = 0;//the index in source to read from

    internalString = new EG_StringInternal;

    while (source[readIndex] != NULL)
    {
        internalString->append(source[readIndex]);
        readIndex++;
    }//while source
}//constructor

EG_String::EG_String(const EG_String &source)
{
    internalString = new EG_StringInternal;
    internalString->copyFrom(source.internalString);
}//copy constructor

EG_String& EG_String::operator= (const EG_String &source)
{
    if (this != &source)
        internalString->copyFrom(source.internalString);

    return *this;
}//=

EG_String::~EG_String(void)
{
    delete internalString;
    internalString = NULL;
}//destructor

EG_String EG_String::fromInt(long int source)
{
    EG_String result;//the string to return
    string text;//string containing converted number
    stringstream converter;//does the conversion

    converter << source;
    converter >> text;

    result.appendCString(text.c_str());

    return result;
}//fromInt

EG_String EG_String::fromDouble(double source)
{
    EG_String result;//the string to return
    string text;//string containing converted number
    stringstream converter;//does the conversion

    converter << source;
    converter >> text;

    result.appendCString(text.c_str());

    return result;
}//fromDouble

unsigned int EG_String::size(void) const
{
    return internalString->size();
}//size

char EG_String::getChar(unsigned int index) const
{
    return internalString->getChar(index);
}//getChar

void EG_String::setChar(unsigned int index,
                        char newValue)
{
    internalString->setChar(index, newValue);
}//setChar

void EG_String::clear(void)
{
    internalString->clear();
}//clear

void EG_String::append(char newValue)
{
    internalString->append(newValue);
}//append

const char* EG_String::toCString(void) const
{
    return internalString->toCString();
}//toCString

void EG_String::appendCString(const char *source)
{
    internalString->appendCString(source);
}//appendCString

bool EG_String::toInt(int &theInt) const
{
    stringstream converter;//converts text to an int
    string text;//the text we're storing
    int tempValue = 0;//dont change the parameter if there were errors
    bool result = true;//false on conversion error

    text.append(internalString->toCString());
    converter << text;
    converter >> tempValue;

    if ((converter.fail()) || (converter.good()))
        result = false;
    else
        theInt = tempValue;

  return result;
}//toInt

bool EG_String::toInt(long int &theInt) const
{
    stringstream converter;//converts text to an int
    string text;//the text we're storing
    long int tempValue = 0;//dont change the parameter if there were errors
    bool result = true;//false on conversion error

    text.append(internalString->toCString());
    converter << text;
    converter >> tempValue;

    if ((converter.fail()) || (converter.good()))
        result = false;
    else
        theInt = tempValue;

  return result;
}//toInt
