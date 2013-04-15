#include "EG_ConfigWriter.hpp"
#include "EG_ConfigWriterInt.hpp"
#include "EG_String.hpp"

using namespace std;
using namespace EbonGL;

string egToStr(const EbonGL::EG_String &egString)
{
    string result;//the stl::string to return

    result.append(egString.toCString());

    return result;
}//egToStr

EG_ConfigWriter::EG_ConfigWriter(void)
{
}//constructor

EG_ConfigWriter::~EG_ConfigWriter(void)
{
}//destructor

void EG_ConfigWriter::setPath(const EG_String &newPath)
{
    EG_ConfigWriterInt::setPath(egToStr(newPath));
}//setPath

EG_String EG_ConfigWriter::loadString(const EG_String &fileName,
                                      const EG_String &configID)
{
    EG_String result;//The string to return
    string text;//stl version of result

    text = EG_ConfigWriterInt::loadString(egToStr(fileName), egToStr(configID));
    result.appendCString(text.c_str());

    return result;
}//loadString

void EG_ConfigWriter::writeString(const EG_String &fileName,
                                  const EG_String &configID,
                                  const EG_String &newString)
{
    EG_ConfigWriterInt::writeString(egToStr(fileName), egToStr(configID), egToStr(newString));
}//writeString

int EG_ConfigWriter::loadInt(const EG_String &fileName,
                             const EG_String &configID)
{
    return EG_ConfigWriterInt::loadInt(egToStr(fileName), egToStr(configID));
}//loadInt

void EG_ConfigWriter::writeInt(const EG_String &fileName,
                               const EG_String &configID,
                               int newInt)
{
    EG_ConfigWriterInt::writeInt(egToStr(fileName), egToStr(configID), newInt);
}//writeInt

bool EG_ConfigWriter::loadBool(const EG_String &fileName,
                               const EG_String &configID)
{
    return EG_ConfigWriterInt::loadBool(egToStr(fileName), egToStr(configID));
}//loadBool

void EG_ConfigWriter::writeBool(const EG_String &fileName,
                                const EG_String &configID,
                                bool newBool)
{
    EG_ConfigWriterInt::writeBool(egToStr(fileName), egToStr(configID), newBool);
}//writeBool

double EG_ConfigWriter::loadDouble(const EG_String &fileName,
                                   const EG_String &configID)
{
    return EG_ConfigWriterInt::loadDouble(egToStr(fileName), egToStr(configID));
}//loadDouble

void EG_ConfigWriter::writeDouble(const EG_String &fileName,
                                  const EG_String &configID,
                                  double newDouble)
{
    EG_ConfigWriterInt::writeDouble(egToStr(fileName), egToStr(configID), newDouble);
}//writeDouble

EG_String EG_ConfigWriter::intToStr(int number)
{
    EG_String result;//the text to return
    string text;//text equivalent of number

    text = EG_ConfigWriterInt::intToStr(number);
    result.appendCString(text.c_str());

    return result;
}//intToStr

bool EG_ConfigWriter::strToInt(const EG_String &text,
                               int &theInt)
{
    return EG_ConfigWriterInt::strToInt(text.toCString(), theInt);
}//strToInt
