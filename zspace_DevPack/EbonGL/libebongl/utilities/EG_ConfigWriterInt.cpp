#include "EG_ConfigWriterInt.hpp"
#include "../EG_Settings.hpp"
#include "../EG_Utils.hpp"
#include "EG_String.hpp"
#include <math.h>

using namespace std;
using namespace EbonGL;

string EG_ConfigWriterInt::path;

EG_ConfigWriterInt::EG_ConfigWriterInt(void)
{
}//constructor

EG_ConfigWriterInt::~EG_ConfigWriterInt(void)
{
}//destructor

void EG_ConfigWriterInt::setPath(const std::string &newPath)
{
    path = newPath;
}//setPath

string EG_ConfigWriterInt::loadString(const string &fileName,
                                   const string &configID)

{
    ifstream infile;//obtains input from the file
    string result;//the loaded string to return
    string oneLine;//a single line of text from the file
    string pathAndName;//the path to the file, including the file name
    bool foundID = false;//true if we found the configID

    //### Open the file ###
    pathAndName = path;
    pathAndName.append(fileName);
    infile.open(pathAndName.c_str());
    if (!infile.good())
    {
        EG_Utils::addLog(EG_String("ConfigWriter couldn't open "));
        EG_Utils::addLog(EG_String(pathAndName.c_str()));
        EG_Utils::writeLog(EG_String(" for reading"));
        EG_Utils::fatalError();
    }//if !good()

    //### Find the configID ###
    while ((infile.good()) && (!foundID))
    {
        //read the configID line
        getline(infile, oneLine);
        oneLine = cleanLine(oneLine);

        if (infile.good())
        {
            if (oneLine.size() > 0)
            {
                if (oneLine.at(0) == '%')
                {
                    //remove the % symbol
                    oneLine.erase(0, 1);

                    if (oneLine.size() > 0)
                    {
                        //check for matches
                        if (oneLine == configID)
                            foundID = true;
                    }//if size()
                    else
                    {
                        EG_Utils::addLog(EG_String("File "));
                        EG_Utils::addLog(EG_String(pathAndName.c_str()));
                        EG_Utils::writeLog(EG_String(" contains a % with no identifier"));
                        EG_Utils::fatalError();
                    }//else size
                }//if oneLine
            }//if size()
        }//if good()
    }//while good() && !foundID

    //### See if the ID was found ###
    if (!foundID)
    {
        EG_Utils::addLog(EG_String("ConfigWriter couldn't find "));
        EG_Utils::addLog(EG_String(configID.c_str()));
        EG_Utils::addLog(EG_String(" in file "));
        EG_Utils::writeLog(EG_String(pathAndName.c_str()));
        EG_Utils::fatalError();
    }//if !foundID

    //### Read the value ###
    getline(infile, result);
    result = cleanLine(result);
    if (!infile.good())
    {
        if (!infile.eof())
        {
            EG_Utils::addLog(EG_String("ConfigWriter couldn't read file after "));
            EG_Utils::addLog(EG_String(configID.c_str()));
            EG_Utils::addLog(EG_String(" in file "));
            EG_Utils::writeLog(EG_String(pathAndName.c_str()));
            EG_Utils::fatalError();
        }//if !eof()
    }//if !good()

    //### Verify that we read something ###
    if (result.size() <= 1)
    {
        EG_Utils::addLog(EG_String("ConfigWriter found no value for "));
        EG_Utils::addLog(EG_String(configID.c_str()));
        EG_Utils::addLog(EG_String(" in file "));
        EG_Utils::writeLog(EG_String(pathAndName.c_str()));
        EG_Utils::fatalError();
    }//if size()

    //### Remove the > symbol ###
    if (result.at(0) == '>')
        result.erase(0, 1);
    else
    {
        EG_Utils::addLog(EG_String("ConfigWriter expected > to indicate the value for "));
        EG_Utils::addLog(EG_String(configID.c_str()));
        EG_Utils::addLog(EG_String(" in file "));
        EG_Utils::writeLog(EG_String(pathAndName.c_str()));
        EG_Utils::fatalError();
    }//if result

    //### Close the file ###
    if (infile.is_open())
        infile.close();

    return result;
}//loadString

string EG_ConfigWriterInt::cleanLine(const string &oneLine)
{
    string result;//the cleaned line to return
    char currentChar = ' ';//the current character to examine

    for (unsigned int i = 0; i < oneLine.size(); i++)
    {
        currentChar = oneLine.at(i);

        if ((currentChar >= 32) && (currentChar <= 126))
            result.push_back(currentChar);
    }//for i

    return result;
}//cleanLine

void EG_ConfigWriterInt::writeString(const string &fileName,
                                  const string &configID,
                                  const string &newString)
{
    vector <EG_ConfigComments*> fileComments;//the comments for all entries
    vector <string> fileIDs;//every configID read from the file
    vector <string> fileValues;//every string value from the file
    EG_ConfigComments *commentGroup = NULL;//a group of comments for a single entry
    ifstream infile;//obtains input from the file
    ofstream outfile;//re-writes the file
    string pathAndName;////the path to the file, including the file name
    string oneLine;//a single line of text from the file
    bool foundValue = false;//true if configID was present in the file

    //### Read the entire file into memory ###
    pathAndName = path;
    pathAndName.append(fileName);
    infile.open(pathAndName.c_str());
    if (infile.good())
    {
        while (infile.good())
        {
            getline(infile, oneLine);
            oneLine = cleanLine(oneLine);

            if (infile.good())
            {
                if (oneLine.size() > 0)
                {
                    if (readEntry(oneLine, infile, configID, newString,
                        fileComments, fileIDs, fileValues, pathAndName))
                    foundValue = true;
                }//if size()
            }//if good()
        }//while good()
    }//if good()

    //### Close the input file ###
    if (infile.is_open())
        infile.close();

    //### Open the output file ###
    outfile.open(pathAndName.c_str());
    if (!outfile.good())
    {
        EG_Utils::addLog(EG_String("ConfigWriter couldn't open "));
        EG_Utils::addLog(EG_String(pathAndName.c_str()));
        EG_Utils::writeLog(EG_String(" for writing"));
        EG_Utils::fatalError();
    }//if !good()

    //### Write the new config file ###
    for (unsigned int i = 0; i < fileComments.size(); i++)
    {
        commentGroup = fileComments.at(i);
        for (unsigned int j = 0; j < commentGroup->size(); j++)
            outfile << commentGroup->get(j) << endl;

        outfile << fileIDs.at(i) << endl;
        outfile << fileValues.at(i) << endl;
        outfile << endl;

        if (!outfile.good())
        {
            EG_Utils::addLog(EG_String("ConfigWriter: error writing to file "));
            EG_Utils::writeLog(EG_String(pathAndName.c_str()));
            EG_Utils::fatalError();
        }//if !good()
    }//for i

    //### Create a new entry if no existing entry was found ###
    if (!foundValue)
    {
        outfile << "%" << configID << endl;
        outfile << ">" << newString << endl;
        outfile << endl;
        if (!outfile.good())
        {
            EG_Utils::addLog(EG_String("ConfigWriter: error writing to file "));
            EG_Utils::writeLog(EG_String(pathAndName.c_str()));
            EG_Utils::fatalError();
        }//if !good()
    }//if !foundValue

    //### Close the output file ###
    if (outfile.is_open())
        outfile.close();

    //### Free memory ###
    for (unsigned int i = 0; i < fileComments.size(); i++)
    {
        delete fileComments.at(i);
        fileComments.at(i) = NULL;
    }//for i
    fileComments.clear();
}//writeString

bool EG_ConfigWriterInt::readEntry(string oneLine,
                                   ifstream &infile,
                                   const string &configID,
                                   const string &newString,
                                   vector <EG_ConfigComments*> &fileComments,
                                   vector <string> &fileIDs,
                                   vector <string> &fileValues,
                                   const string &pathAndName)
{
    EG_ConfigComments *commentGroup = NULL;//a group of comments for a single entry
    string currentID;//the current config ID read from file
    string newValue;//the new value to use, equal to > plus newString
    bool result = false;//true if we found configID and replaced it's value with newString
    bool readComments = true;//set to false when we've finished reading comments

    //### Verify that oneLine is a comment or ID ###
    if ((oneLine.at(0) != '#') && (oneLine.at(0) != '%'))
    {
        EG_Utils::addLog(EG_String("ConfigWriter error reading "));
        EG_Utils::addLog(EG_String(pathAndName.c_str()));
        EG_Utils::addLog(EG_String(": "));
        EG_Utils::addLog(EG_String(oneLine.c_str()));
        EG_Utils::writeLog(EG_String(" Expected a comment or ID"));
        EG_Utils::fatalError();
    }//if oneLine && oneLine

    //### Read comments ###
    commentGroup = new EG_ConfigComments;
    fileComments.push_back(commentGroup);
    if (oneLine.at(0) == '#')
    {
        commentGroup->add(oneLine);

        while (readComments)
        {
            getline(infile, oneLine);
            oneLine = cleanLine(oneLine);

            if (!infile.good())
            {
                EG_Utils::addLog(EG_String("ConfigWriter couldn't read from file "));
                EG_Utils::addLog(EG_String(pathAndName.c_str()));
                EG_Utils::writeLog(EG_String(" Expected config ID after comments"));
                EG_Utils::fatalError();
            }//if !good()

            if (oneLine.size() == 0)
            {
                EG_Utils::addLog(EG_String("ConfigWriter error reading from file "));
                EG_Utils::addLog(EG_String(pathAndName.c_str()));
                EG_Utils::writeLog(EG_String(" Got a blank line; expected comment or ID"));
                EG_Utils::fatalError();
            }//if size()

            if (oneLine.at(0) == '#')
                commentGroup->add(oneLine);
            else if (oneLine.at(0) == '%')
                readComments = false;
            else
            {
                EG_Utils::addLog(EG_String("ConfigWriter error reading from file "));
                EG_Utils::addLog(EG_String(pathAndName.c_str()));
                EG_Utils::writeLog(EG_String(" Invalid line after comments section"));
                EG_Utils::fatalError();
            }//else oneLine
        }//while readComments
    }//if oneLine

    //### Verify that there's a config ID ###
    if (oneLine.size() <= 1)
    {
        EG_Utils::addLog(EG_String("ConfigWriter: expected config ID after % symbol in file "));
        EG_Utils::writeLog(EG_String(pathAndName.c_str()));
        EG_Utils::fatalError();
    }//if size()

    //### See if the config ID matches the ID to change ###
    fileIDs.push_back(oneLine);
    currentID = oneLine;
    currentID.erase(0, 1);
    if (currentID == configID)
        result = true;

    //### Read the value ###
    getline(infile, oneLine);
    oneLine = cleanLine(oneLine);
    if (!infile.good())
    {
        if (!infile.eof())
        {
            string problemID = fileIDs.at(fileIDs.size() - 1);

            EG_Utils::addLog(EG_String("ConfigWriter: expected value after ID "));
            EG_Utils::addLog(EG_String(problemID.c_str()));
            EG_Utils::addLog(EG_String(" in file "));
            EG_Utils::writeLog(EG_String(pathAndName.c_str()));
            EG_Utils::fatalError();
        }//if !eof()
    }//if !good()

    //### Verify that there was a value present in the file ###
    if (oneLine.size() <= 1)
    {
        string problemID = fileIDs.at(fileIDs.size() - 1);

        EG_Utils::addLog(EG_String("ConfigWriter: expected value after ID "));
        EG_Utils::addLog(EG_String(problemID.c_str()));
        EG_Utils::addLog(EG_String(" in file "));
        EG_Utils::writeLog(EG_String(pathAndName.c_str()));
        EG_Utils::fatalError();
    }//if size()

    //### Verify that the value is preceeded by > ###
    if (oneLine.at(0) != '>')
    {
        string problemID = fileIDs.at(fileIDs.size() - 1);

        EG_Utils::addLog(EG_String("ConfigWriter expected > to preceed the value for "));
        EG_Utils::addLog(EG_String(problemID.c_str()));
        EG_Utils::addLog(EG_String(" in file "));
        EG_Utils::writeLog(EG_String(pathAndName.c_str()));
        EG_Utils::fatalError();
    }//if oneLine

    //### Use the new value, if the config ID matches ###
    if (result)
    {
        newValue = ">";
        newValue.append(newString);
        fileValues.push_back(newValue);
    }//if result
    else
        fileValues.push_back(oneLine);

    return result;
}//readEntry

int EG_ConfigWriterInt::loadInt(const string &fileName,
                             const string &configID)
{
    string stringValue;//the string read from file
    string pathAndName;//the path to the file, including the file name
    int result = 0;//the integer to return

    pathAndName = path;
    pathAndName.append(fileName);

    stringValue = loadString(fileName, configID);
    if (!strToInt(stringValue, result))
    {
        writeFileAndLabel(pathAndName, configID);
        EG_Utils::writeLog(EG_String(", expected int"));
        EG_Utils::fatalError();
    }//if strToInt()

    return result;
}//loadInt

void EG_ConfigWriterInt::writeInt(const string &fileName,
                               const string &configID,
                               int newInt)
{
    writeString(fileName, configID, intToStr(newInt));
}//writeInt

bool EG_ConfigWriterInt::loadBool(const string &fileName,
                               const string &configID)
{
    string stringValue;//the string read from file
    string pathAndName;////the path to the file, including the file name
    bool result = true;//the boolean to return

    pathAndName = path;
    pathAndName.append(fileName);

    stringValue = loadString(fileName, configID);
    if (stringValue == "TRUE")
        result = true;
    else if (stringValue == "FALSE")
        result = false;
    else
    {
        writeFileAndLabel(pathAndName, configID);
        EG_Utils::writeLog(EG_String(", expected TRUE or FALSE"));
        EG_Utils::fatalError();
    }//if strToInt()

    return result;
}//loadBool

void EG_ConfigWriterInt::writeBool(const string &fileName,
                                const string &configID,
                                bool newBool)
{
    if (newBool)
        writeString(fileName, configID, "TRUE");
    else
        writeString(fileName, configID, "FALSE");
}//writeBool

double EG_ConfigWriterInt::loadDouble(const std::string &fileName,
                                   const std::string &configID)
{
    stringstream converter;//converts the string to a double
    string stringValue;//the string read from file
    string pathAndName;////the path to the file, including the file name
    double result = 0;//the value to return

    pathAndName = path;
    pathAndName.append(fileName);
    stringValue = loadString(fileName, configID);

    converter << stringValue;
    converter >> result;
    if ((converter.fail()) || (!converter.eof()))
    {
        if (stringValue == "nan")
            result = std::numeric_limits<double>::quiet_NaN();
        else if (stringValue == "inf")
            result = std::numeric_limits<double>::infinity();
        else
        {
            writeFileAndLabel(pathAndName, configID);
            EG_Utils::writeLog(EG_String(", expected a double"));
            EG_Utils::fatalError();
        }//else stringValue
    }//if fail() || !eof()

    return result;
}//loadDouble

void EG_ConfigWriterInt::writeDouble(const std::string &fileName,
                                  const std::string &configID,
                                  double newDouble)
{
    stringstream converter;//converts newDouble to a string
    string text;//text version of newDouble

    converter << newDouble;
    converter >> text;

    writeString(fileName, configID, text);
}//writeDouble

bool EG_ConfigWriterInt::strToInt(const string &text,
                               int &theInt)
{
    stringstream converter;//converts text to an int
    int tempValue = 0;//dont change the parameter if there were errors
    bool result = true;//false on conversion error

    //### Verify there's a token in text ###
    if (converter.rdstate() & stringstream::eofbit)
        result = false;

    //### Do the conversion ###
    if (result)
    {
        converter << text;
        converter >> tempValue;

        //couldn't convert to int
        if (converter.rdstate() & stringstream::failbit)
            result = false;

        //unknown error
        else if (converter.rdstate() & stringstream::badbit)
            result = false;

        //got an int, but there's more tokens in the text
        else if (!(converter.rdstate() & stringstream::eofbit))
            result = false;
    }//if result

    //### Use this value ###
    if (result)
        theInt = tempValue;

  return result;
}//strToInt

string EG_ConfigWriterInt::intToStr(int number)
{
    string result;//string containing converted number
    stringstream converter;//does the conversion

    converter << number;
    converter >> result;

    return result;
}//intToStr

void EG_ConfigWriterInt::writeFileAndLabel(const string &pathAndName,
                                        const string &configID)
{
    EG_Utils::addLog(EG_String("For file "));
    EG_Utils::addLog(EG_String(pathAndName.c_str()));
    EG_Utils::addLog(EG_String(", label "));
    EG_Utils::addLog(EG_String(configID.c_str()));
}//writeFileAndLabel
