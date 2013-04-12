/* DESCRIPTION
  A cross-platform Mutex. It's implemented with Pthreads and Windows native threads.
  We throw errors and set errMsg rather than use DEngine::fatalError, as fatalError
  needs to lock and unlock EG_Mutexes, which may in itself be the source of the error.
*/

#ifndef EG_MUTEX_HPP_INCLUDED
#define EG_MUTEX_HPP_INCLUDED

#include "../EG_Utils.hpp"
#include "EG_String.hpp"

namespace EbonGL
{
    class MH_INTERFACE EG_Mutex
    {
        public:
            //constructor
            EG_Mutex(void);

            //destructor
            ~EG_Mutex(void);

            //restrict access to critical areas
            void lock(void);
            void unlock(void);

            //If one of the other member functions throws an exception, errMsg will be set
            //with a description of the problem
            EG_String getErrMsg(void);

        private:
            //the mutex
            #ifdef EBONGL_WINDOWS
                HANDLE theMutex;
            #else
                pthread_mutex_t theMutex;
            #endif

            //A description of the most recent error
            EG_String errMsg;

    };//EG_Mutex
}//namespace EbonGL

#endif
