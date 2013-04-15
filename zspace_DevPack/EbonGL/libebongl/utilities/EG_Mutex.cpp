#include "EG_Mutex.hpp"

using namespace std;
using namespace EbonGL;

#ifdef EBONGL_WINDOWS

EG_Mutex::EG_Mutex(void)
{
    theMutex = CreateMutex(NULL, FALSE, NULL);
    if (theMutex == NULL)
    {
        string errString = "EG_Mutex::Mutex() CreateMutex failed";

        errMsg.appendCString(errString.c_str());
        EG_Utils::fatalError();
    }//if theMutex
}//constructor

EG_Mutex::~EG_Mutex(void)
{
    if (!CloseHandle(theMutex))
    {
        string errString = "EG_Mutex::~Mutex() CloseHandle failed";

        errMsg.appendCString(errString.c_str());
        //errMsg.append(GetLastError());

        EG_Utils::fatalError();
    }//if !CloseHandle
}//desctructor

void EG_Mutex::lock(void)
{
    if (WaitForSingleObject(theMutex, INFINITE) != WAIT_OBJECT_0)
    {
        string errString = "EG_Mutex::lock() failed!";

        errMsg.appendCString(errString.c_str());

        EG_Utils::fatalError();
    }//if WaitForSingleObject()
}//lock

void EG_Mutex::unlock(void)
{
    if (!ReleaseMutex(theMutex))
    {
        string errString = "EG_Mutex::unlock() failed!";

        errMsg.appendCString(errString.c_str());
        EG_Utils::fatalError();
    }//if !ReleaseMutex()
}//unlock

#else

EG_Mutex::EG_Mutex(void)
{
    if (pthread_mutex_init(&theMutex, NULL) != 0)
    {
        string errString = "EG_Mutex::Mutex() pthread_mutex_init failed";

        errMsg.appendCString(errString.c_str());
        EG_Utils::fatalError();
    }//if pthread_mutex_init()
}//constructor

EG_Mutex::~EG_Mutex(void)
{
    if (pthread_mutex_destroy(&theMutex) != 0)
    {
        string errString = "EG_Mutex::~Mutex() pthread_mutex_destroy failed";

        errMsg.appendCString(errString.c_str());
        EG_Utils::fatalError();
    }//if pthread_mutex_destroy()
}//desctructor

void EG_Mutex::lock(void)
{
    if (pthread_mutex_lock(&theMutex) != 0)
    {
        string errString = "EG_Mutex::lock() failed!";

        errMsg.appendCString(errString.c_str());
        EG_Utils::fatalError();
    }//if pthread_mutex_lock()
}//enterLock

void EG_Mutex::unlock(void)
{
    if (pthread_mutex_unlock(&theMutex) != 0)
    {
        string errString = "EG_Mutex::unlock() failed!";
        errMsg.appendCString(errString.c_str());

        EG_Utils::fatalError();
    }//if pthread_mutex_unlock()
}//unlock

#endif

EG_String EG_Mutex::getErrMsg(void)
{
    return errMsg;
}//getErrMsg
