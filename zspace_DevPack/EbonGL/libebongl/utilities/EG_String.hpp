/* DESCRIPTION
    It doesn't seem possible to export an std::string through a DLL interface,
    however, there are times when we need to communicate with the DLL via strings.
    EG_String is a simple wrapper around std::string for this purpose.
*/

#ifndef EG_STRING_HPP_INCLUDED
#define EG_STRING_HPP_INCLUDED

#include "../EG_Settings.hpp"

namespace EbonGL
{
    class EG_StringInternal;

    class MH_INTERFACE EG_String
    {
        public:
            //constructor
            EG_String(void);
            EG_String(const char *source);

            //copy constructor
		    EG_String(const EG_String &source);

            //destructor
            ~EG_String(void);

            //Overloaded = operator
            EG_String& operator= (const EG_String &source);

            //returns the number of characters in the string
            unsigned int size(void) const;

            //Retrieves the character at the specified index
            char getChar(unsigned int index) const;

            //Sets the character at the specified index to newValue
            void setChar(unsigned int index,
                         char newValue);

            //clear the string
            void clear(void);

            //Add newValue to the end of the string
            void append(char newValue);

            //Returns this string as a NULL-terminated array of characters. The return value
            //points to the EG_String's internal storage, and should not be modified. The values
            //will change if the EG_String is later modified.
            const char* toCString(void) const;

            //Adds source to the end of this string (deep copy)
            void appendCString(const char *source);

            //returns false if text is not a valid integer
            bool toInt(int &theInt) const;
            bool toInt(long int &theInt) const;

            //These functions return an EG_String with a text representation of source
            static EG_String fromInt(long int source);
            static EG_String fromDouble(double source);

        private:
            //Contains an STL string that can't be exported through a DLL interface
            EG_StringInternal *internalString;

    };//EG_String
}//namespace EbonGL
#endif
