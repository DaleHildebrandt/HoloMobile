/* DESCRIPTION
    EG_StringInternal stores an STL string used by EG_String. We store the string here,
    rather than directly in EG_String, to avoid DLL interface warnings.
*/

#ifndef EG_STRINGINTERNAL_HPP_INCLUDED
#define EG_STRINGINTERNAL_HPP_INCLUDED

#include <string>

namespace EbonGL
{
    class EG_StringInternal
    {
        public:
            //constructor
            EG_StringInternal(void);

            //destructor
            ~EG_StringInternal(void);

            //returns the number of characters in the string
            unsigned int size(void);

            //Retrieves the character at the specified index
            char getChar(unsigned int index);

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
            const char* toCString(void);

            //Set this EG_StringInternal to a copy of source
            void copyFrom(EG_StringInternal *source);

            //Adds source to the end of this string (deep copy)
            void appendCString(const char *source);

        private:
            //The string we're hiding from the DLL interface
            std::string theString;

    };//EG_StringInternal
}//namespace

#endif
