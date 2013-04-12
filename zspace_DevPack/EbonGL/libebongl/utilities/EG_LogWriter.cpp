#include "EG_LogWriter.hpp"
#include "EG_Mutex.hpp"

using namespace std;
using namespace EbonGL;

EG_LogWriter::EG_LogWriter(const string &newFileName)
{
	ofstream outfile;//writes to the log file

	logMutex = new EG_Mutex;
	logMutex->lock();

	fileName = newFileName;
	numEntries = 0;

	outfile.open(fileName.c_str());
	if (!outfile.is_open())
		EG_Utils::fatalError();
	else
	{
		outfile << "EbonGL Log File" << endl;
		outfile << endl;

		outfile.close();
	}//else

	logMutex->unlock();
}//constructor

EG_LogWriter::EG_LogWriter(const EG_LogWriter &source)
{
	logMutex->lock();
    copyFrom(source);
	logMutex->unlock();
}//copy constructor

EG_LogWriter::~EG_LogWriter(void)
{
	delete logMutex;
	logMutex = NULL;
}//destructor

EG_LogWriter& EG_LogWriter::operator= (const EG_LogWriter &source)
{
	logMutex->lock();

    if (this != &source)
        copyFrom(source);

	logMutex->unlock();

    return *this;
}//=

void EG_LogWriter::copyFrom(const EG_LogWriter &source)
{
	fileName = source.fileName;
	numEntries = source.numEntries;
	outputLine = source.outputLine;
}//copyFrom

EG_LogWriter& EG_LogWriter::operator << (const std::string &text)
{
	logMutex->lock();
    outputLine.append(text);
	logMutex->unlock();

    return *this;
}//<< string

EG_LogWriter& EG_LogWriter::operator << (const int &number)
{
    stringstream converter;//converts number to text
    string text;//string version of the number to append

	logMutex->lock();

    converter << number;
    converter >> text;
    if (converter.fail())
        outputLine.append("Internal error in EG_LogWriter::<<: unable to convert int to string");
    else
        outputLine.append(text);

	logMutex->unlock();

    return *this;
}//<< int

EG_LogWriter& EG_LogWriter::operator << (const unsigned int &number)
{
    stringstream converter;//converts number to text
    string text;//string version of the number to append

	logMutex->lock();

    converter << number;
    converter >> text;
    if (converter.fail())
        outputLine.append("Internal error in EG_LogWriter::<<: unable to convert unsigned int to string");
    else
        outputLine.append(text);

	logMutex->unlock();

    return *this;
}//<< unsigned int

EG_LogWriter& EG_LogWriter::operator << (const float &number)
{
    stringstream converter;//converts number to text
    string text;//string version of the number to append

	logMutex->lock();

    converter << number;
    converter >> text;
    if (converter.fail())
        outputLine.append("Internal error in EG_LogWriter::<<: unable to convert float to string");
    else
        outputLine.append(text);

	logMutex->unlock();

    return *this;
}//<< float

EG_LogWriter& EG_LogWriter::operator << (const double &number)
{
    stringstream converter;//converts number to text
    string text;//string version of the number to append

	logMutex->lock();

    converter << number;
    converter >> text;
    if (converter.fail())
        outputLine.append("Internal error in EG_LogWriter::<<: unable to convert double to string");
    else
        outputLine.append(text);

	logMutex->unlock();

    return *this;
}//<< double

EG_LogWriter& EG_LogWriter::operator << (ostream& ( *pf )(ostream&))
{
	std::ofstream outfile;//the log file to write to

	logMutex->lock();

	if (numEntries < MAX_ENTRIES)
	{
		//### Open the log file ###
		outfile.open(fileName.c_str(), std::ios_base::app);

		//### Output the message to log ###
		if (outfile.is_open())
		{
			outfile << outputLine;
			outfile << pf;
		}//if is_open()

		//### Output the message to stdout ###
		cout << outputLine;
		cout << pf;

		//### Show a message when we hit the max number of log entries ###
		numEntries++;
		if (numEntries >= MAX_ENTRIES)
		{
			outfile << "Max number of log entries reached, future entries will be ignored" << endl;
			cout << "Max number of log entries reached, future entries will be ignored" << endl;
		}//if numEntries

		//### Close the log file ###
		if (outfile.is_open())
			outfile.close();
	}//if numEntries

    errMsg = outputLine;
    outputLine.clear();

	logMutex->unlock();

    return *this;
}//<< ostream weirdness

string EG_LogWriter::getErrMsg(void)
{
    return errMsg;
}//getErrMsg
