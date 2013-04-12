/* DESCRIPTION
    There doesn seem to be any way to export vector<bool> through a DLL interface.
    EG_BoolVector provides an exportable class for this purpose. It uses a vector
    of GLushort rather than bool to avoid DLL warnings.
*/

#ifndef EG_BOOLVECTOR_HPP_INCLUDED
#define EG_BOOLVECTOR_HPP_INCLUDED

#include "../EG_Settings.hpp"

class MH_INTERFACE EG_BoolVector
{
    public:
        //constructor, creates an empty vector
        EG_BoolVector(void);

        //destructor
        ~EG_BoolVector(void);

        //Resizes the vector to have the specified number of elements. Every element
        //in the vector will be set to initialValue
        void resize(unsigned int numElements,
                    bool initialValue);

        //Returns the value at the specified index
        bool get(unsigned int index) const;

        //Set the value at the specified index
        void set(unsigned int index,
                 bool newValue);

        //Returns the number of elements in the vector
        unsigned int size(void) const;

    private:
        //The values being stored
        std::vector<GLushort> theValues;

};//EG_BoolVector

#endif
