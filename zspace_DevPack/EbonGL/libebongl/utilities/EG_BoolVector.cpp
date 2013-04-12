#include "EG_BoolVector.hpp"

using namespace std;
using namespace EbonGL;

EG_BoolVector::EG_BoolVector(void)
{
}//constructor

EG_BoolVector::~EG_BoolVector(void)
{
}//destructor

void EG_BoolVector::resize(unsigned int numElements,
                           bool initialValue)
{
    theValues.clear();
    if (numElements > 0)
    {
        if (initialValue)
            theValues.resize(numElements, 1);
        else
            theValues.resize(numElements, 0);
    }//if numElements
}//resize

bool EG_BoolVector::get(unsigned int index) const
{
    bool result = false;//true if the corresponding element in theValues is set

    if (theValues.at(index) == 1)
        result = true;

    return result;
}//get

void EG_BoolVector::set(unsigned int index,
                        bool newValue)
{
    if (newValue)
        theValues.at(index) = 1;
    else
        theValues.at(index) = 0;
}//set

unsigned int EG_BoolVector::size(void) const
{
    return static_cast<unsigned int>(theValues.size());
}//size
