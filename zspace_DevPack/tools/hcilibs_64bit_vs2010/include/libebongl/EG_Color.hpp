#ifndef EG_COLOR_HPP_INCLUDED
#define EG_COLOR_HPP_INCLUDED

#include "EG_Utils.hpp"

namespace EbonGL
{
    class MH_INTERFACE EG_Color
    {
        public:
            //constructors
            EG_Color(void);
            EG_Color(GLfloat red,
                     GLfloat green,
                     GLfloat blue,
                     GLfloat alpha);

            //copy constructor
		    EG_Color(const EG_Color &source);

            //destructor
            ~EG_Color(void);

		    //Overloaded = operator
            EG_Color& operator= (const EG_Color &source);

            //Returns a pointer to the first element of the color array.
            //The array has four elements for red, green, blue and alpha,
            GLfloat* toArray(void);

            //Mutator
            void setColor(GLfloat red,
                          GLfloat green,
                          GLfloat blue,
                          GLfloat alpha);
            void setColor(const EG_Color &newColor);

            //Accessors
            GLfloat red(void) const;
            GLfloat green(void) const;
            GLfloat blue(void) const;
            GLfloat alpha(void) const;

        private:
            //The color being stored
            std::vector<GLfloat> theColor;

    };//EG_Color
}//namespace EbonGL

#endif
