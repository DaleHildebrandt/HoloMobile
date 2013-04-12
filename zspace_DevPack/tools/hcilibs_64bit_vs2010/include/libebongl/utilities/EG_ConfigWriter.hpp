#ifndef EG_CONFIGWRITER_HPP_INCLUDED
#define EG_CONFIGWRITER_HPP_INCLUDED

#include "../EG_Settings.hpp"

namespace EbonGL
{
    class EG_String;

    class MH_INTERFACE EG_ConfigWriter
    {
        public:
            //constructor
            EG_ConfigWriter(void);

            //destructor
            ~EG_ConfigWriter(void);

            //Set the path to the directory containing the config file(s), newPath should end
            //with a /
            static void setPath(const EbonGL::EG_String &newPath);

            //loads a string from the file, from the entry with the specified configID
            static EbonGL::EG_String loadString(const EbonGL::EG_String &fileName,
                                                const EbonGL::EG_String &configID);

            //writes newString into the file
            static void writeString(const EbonGL::EG_String &fileName,
                                    const EbonGL::EG_String &configID,
                                    const EbonGL::EG_String &newString);

            //loads an integer from the file
            static int loadInt(const EbonGL::EG_String &fileName,
                               const EbonGL::EG_String &configID);

            //writes theInt to the file
            static void writeInt(const EbonGL::EG_String &fileName,
                                 const EbonGL::EG_String &configID,
                                 int newInt);

            //Loads a boolean from the file
            static bool loadBool(const EbonGL::EG_String &fileName,
                                 const EbonGL::EG_String &configID);

            //writes newBool to the file
            static void writeBool(const EbonGL::EG_String &fileName,
                                  const EbonGL::EG_String &configID,
                                  bool newBool);

            //Loads a double from the file
            static double loadDouble(const EbonGL::EG_String &fileName,
                                     const EbonGL::EG_String &configID);

            //Writes newDouble to the file
            static void writeDouble(const EbonGL::EG_String &fileName,
                                    const EbonGL::EG_String &configID,
                                    double newDouble);

            //returns the string equivalent of number
            static EbonGL::EG_String intToStr(int number);

            //returns false if text is not a valid integer
            static bool strToInt(const EbonGL::EG_String &text,
                                 int &theInt);

    };//EG_ConfigWriter
}//namespace EbonGL

#endif
