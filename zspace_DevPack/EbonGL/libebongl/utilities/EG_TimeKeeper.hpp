/* DESCRIPTION
  A simple millisecond-accurate timer. Each Time object contains a stored time,
  which can be compared to the current time to determine the time elapsed.
*/

#ifndef EG_TIMEKEEPER_HPP_INCLUDED
#define EG_TIMEKEEPER_HPP_INCLUDED

#include <stdint.h>
#include "../EG_Settings.hpp"

namespace EbonGL
{
    class MH_INTERFACE EG_TimeKeeper
    {
        public:
            //constructor
            EG_TimeKeeper(void);

            //copy constructor
            EG_TimeKeeper(const EG_TimeKeeper &source);

            //destructor
            ~EG_TimeKeeper(void);

            //Overloaded = operator
            EG_TimeKeeper& operator= (const EG_TimeKeeper &source);

            //updates the stored time to the current time
            void updateTime(void);

            //returns the number of milliseconds since the last time updateTime() was called
            uint32_t timeElapsed(void);

            //increases the stored time by the specified number of milliseconds
            void addTime(uint32_t newTime);

            //Sets the stored time to the specified values
            void setTime(uint32_t newSeconds,
                         uint16_t newMilliseconds);

            //return the stored time
            uint32_t getSeconds(void);
            uint16_t getMilliseconds(void);

            //Suspends execution of the calling thread by the specified number of milliseconds
            static void sleep(long int milliseconds);

        private:
            #ifdef EBONGL_WINDOWS
                //current value of the high-res timer
                LARGE_INTEGER winTimerValue;

                //number of high-res timer updates/sec, divided by 1000. Divide
                //the current winTimerValue by this to get milliseconds
                uint64_t winCounterDiv;
            #endif

            //the time being stored
            uint32_t seconds;
            uint16_t milliseconds;

            //############### FUNCTIONS ###############

            //Creates the windows high-res timer
            void createWinTimer(void);

            //Obtains the current time, stores it in the provided variables
            void getCurrentTime(uint32_t &currentSeconds,
                                uint16_t &currentMilliseconds);

            //De-registers with the current engine, if any.
            void leaveEngine(void);

    };//class EG_TimeKeeper
}//namespace EbonGL
#endif
