#include "EG_TimeKeeper.hpp"
#include "../EG_Settings.hpp"
#include "../EG_Utils.hpp"
#include "EG_String.hpp"

using namespace std;
using namespace EbonGL;

EG_TimeKeeper::EG_TimeKeeper(void)
{
    createWinTimer();

    seconds = 0;
    milliseconds = 0;
}//constructor

EG_TimeKeeper::EG_TimeKeeper(const EG_TimeKeeper &source)
{
    seconds = source.seconds;
    milliseconds = source.milliseconds;

    createWinTimer();
}//copy constructor

EG_TimeKeeper::~EG_TimeKeeper(void)
{
}//destructor

EG_TimeKeeper& EG_TimeKeeper::operator= (const EG_TimeKeeper &source)
{
    if (this != &source)
    {
        seconds = source.seconds;
        milliseconds = source.milliseconds;
    }//if this

    return *this;
}//=

void EG_TimeKeeper::createWinTimer(void)
{
    #ifdef EBONGL_WINDOWS
        LARGE_INTEGER performanceFreq;//extracts the frequency of the high res timer
        string errMsg;//a description of any timer errors

        performanceFreq.QuadPart = 0;
        winTimerValue.QuadPart = 0;
        winCounterDiv = 0;

        if (QueryPerformanceFrequency(&performanceFreq))
        {
            winCounterDiv = performanceFreq.QuadPart;
            if (winCounterDiv != 0)
            {
                winCounterDiv = winCounterDiv / 1000;
                if (winCounterDiv == 0)
                    errMsg = "EG_TimeKeeper::createWinTimer(): high performance counter is updating less than 1000 x per sec";
            }//if winCounterDiv
            else
                errMsg = "EG_TimeKeeper::createWinTimer(): high performance counter is updating zero times per second!";
        }//if QueryPerformanceFrequency()
        else
            errMsg = "EG_TimeKeeper::createWinTimer(): QueryPerformanceFrequency() failed!";

        if (errMsg.size() > 0)
        {
            EG_Utils::writeLog(EG_String(errMsg.c_str()));
            EG_Utils::fatalError();
        }//if size()
    #endif
}//createWinTimer

void EG_TimeKeeper::updateTime(void)
{
    getCurrentTime(seconds, milliseconds);
}//updateTime

uint32_t EG_TimeKeeper::timeElapsed(void)
{
    uint32_t result = 0;//the number of elapsed milliseconds to return
    uint32_t currentSeconds = 0;//current time, in seconds
    long int timeAmount = 0;//check for negative time elapsed values
    uint16_t currentMilliseconds = 0;//millisecond component of the current time

    //### Get number of elapsed seconds ###
    getCurrentTime(currentSeconds, currentMilliseconds);
    timeAmount = currentSeconds - seconds;

    //### Make sure the number of milliseconds will fit in a 32-bit signed variable ###
    if (timeAmount > 2147482)
        timeAmount = 2147482;

    //### Convert to milliseconds ###
    if (timeAmount >= 0)
    {
        timeAmount *= 1000;
        timeAmount += currentMilliseconds - milliseconds;

        if (timeAmount >= 0)
            result = timeAmount;
    }//if timeAmount

    return result;
}//timeElapsed

void EG_TimeKeeper::getCurrentTime(uint32_t &currentSeconds,
                                uint16_t &currentMilliseconds)
{
    #ifdef EBONGL_WINDOWS
        if (QueryPerformanceCounter(&winTimerValue))
        {
            if (winTimerValue.QuadPart != 0)
                winTimerValue.QuadPart = winTimerValue.QuadPart / winCounterDiv;

            if (winTimerValue.QuadPart != 0)
            {
                currentSeconds = static_cast<uint32_t>(winTimerValue.QuadPart / 1000);
                currentMilliseconds = winTimerValue.QuadPart % 1000;
            }//if QuadPart
        }//if QueryPerformanceCounter()
        else
        {
            EG_Utils::writeLog(EG_String("EG_TimeKeeper::updateTime(): Couldn't access the high res timer."));
            EG_Utils::fatalError();
        }//else QueryPerformanceCounter()
    #else
        timeval currentTime;//the current time, microsecond accuracy
        uint32_t microseconds = 0;//used to store the time in microseconds

        if (gettimeofday(&currentTime, NULL) == 0)
        {
            currentSeconds = currentTime.tv_sec;
            microseconds = currentTime.tv_usec;
            if (microseconds != 0)
                microseconds = microseconds / 1000;
            currentMilliseconds = microseconds;
        }//if gettimeofday()
        else
        {
            EG_Utils::writeLog(EG_String("EG_TimeKeeper::updateTime(): gettimeofday() failed"));
            EG_Utils::fatalError();
        }//else gettimeofday()
    #endif
}//getCurrentTime

uint32_t EG_TimeKeeper::getSeconds(void)
{
    return seconds;
}//getSeconds

uint16_t EG_TimeKeeper::getMilliseconds(void)
{
    return milliseconds;
}//getMilliseconds

void EG_TimeKeeper::addTime(uint32_t newTime)
{
    newTime += milliseconds;

    if (newTime != 0)
    {
        seconds += newTime / 1000;
        milliseconds = newTime % 1000;
    }//if newTime
}//addTime

void EG_TimeKeeper::setTime(uint32_t newSeconds,
                    uint16_t newMilliseconds)
{
    seconds = newSeconds;
    milliseconds = newMilliseconds;
}//setTime

void EG_TimeKeeper::sleep(long int milliseconds)
{
    #ifdef EBONGL_WINDOWS
        Sleep(milliseconds);
    #else
        timespec sleepTime;

        if (milliseconds >= 1000)
        {
            sleepTime.tv_sec = milliseconds / 1000;
            milliseconds = milliseconds % 1000;
        }//if milliseconds
        else
            sleepTime.tv_sec = 0;

        sleepTime.tv_nsec = milliseconds * 1000000;

        nanosleep(&sleepTime, NULL);
    #endif
}//sleep
