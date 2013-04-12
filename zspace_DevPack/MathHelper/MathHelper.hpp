/*
Copyright (c) 2009-2012 David C. McCallum

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the
use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

    1. The origin of this software must not be misrepresented; you must not
    claim that you wrote the original software. If you use this software in a
    product, an acknowledgment in the product documentation would be
    appreciated but is not required.

    2. Altered source versions must be plainly marked as such, and must not be
    misrepresented as being the original software.

    3. This notice may not be removed or altered from any source distribution.
*/

/* DESCRIPTION
    MathHelper is a collection of classes for 3D calculations.
    Includes the Quaternion, Matrix3x3, and Point3D classes.

    All calculations use the MHTypes::MHFloat level of precision.
*/

#ifndef MH_MATHHELPER_INCLUDED
#define MH_MATHHELPER_INCLUDED

#include <vector>
#include <iostream>
#include <list>

#include "libmathhelp/MHSettings.hpp"
#include "libmathhelp/Point3D.hpp"
#include "libmathhelp/Matrix3x3.hpp"
#include "libmathhelp/Quaternion.hpp"
#include "libmathhelp/Point2D.hpp"
#include "libmathhelp/MHVector.hpp"
#include "libmathhelp/EulerAngle.hpp"
#include "libmathhelp/Plane.hpp"

class MH_INTERFACE MathHelper
{
    public:
        //constructor
        MathHelper(void);

        //destructor
        ~MathHelper(void);

        //Given a system where angles vary from 180 to -180, returns the smallest
        //distance between two angles. eg, if angle1 is 175, and angle2 is -170,
        //returns 15
        static MHTypes::MHFloat shortAngleDistance(MHTypes::MHFloat angle1,
                                                   MHTypes::MHFloat angle2);

        //Re-orders the elements in unordered based on key and returns the result.
        //Elements are ordered so that if there are 24 ways to order them, the keys
        //0-24 will each have a unique order. The ordering repeats for keys 25-48,
        //and so forth. unordered will be empty when the function finishes, and its
        //elements will be in the returned vector.
        template <class T>
		static std::vector<T> orderElements(std::list<T> &unordered,
                                            unsigned int key);

        //Returns the median of the provided items, which do not need to be sorted.
        template <class T>
        static T calcMedian(std::vector<T> theItems);

        //Subsequent calls to genRandom will produce a series of random numbers
        //based on this seed.
        static void setRandSeed(unsigned int seed);

        //Generates a random number between minimum and maximum inclusive
        //The random seed should be set first with a call to setRandSeed().
        //This function is not thread-safe
        static long unsigned int genRandom(long unsigned int minimum,
                                           long unsigned int maximum);

        //Scaling constants to convert from radians to degrees and vice versa
        static MHTypes::MHFloat radToDeg(void);
        static MHTypes::MHFloat degToRad(void);

    private:
        //Scaling constants to convert from radians to degrees and vice versa
        static const MHTypes::MHFloat RADIANS_TO_DEGREES;
        static const MHTypes::MHFloat DEGREES_TO_RADIANS;

};//MathHelper

//FIXME: unordered was originally a QList, and may not have ported
//over to an std::list properly.
template <class T>
std::vector<T> MathHelper::orderElements(std::list<T> &unordered,
                                         unsigned int key)
{
    std::vector<T> result;//the ordered list to return
	unsigned int initialSize = unordered.size();//the number of elements in unordered
    int itemIndex = 0;//index of the current task to copy
    int numCombos = 0;//the number of combinations for a list of this size
    int myCombo = 0;//the index of the combo for this participant
    int chunkSize = 0;

    //### Calculate the number of combinations ###
    numCombos = 1;
    for (unsigned int i = 1; i <= initialSize; i++)
        numCombos *= i;

    //### Find the combo for this key ###
    myCombo = key % numCombos;

    while (unordered.size() > 0)
    {
        //### Find the proper item ###
        chunkSize = numCombos / unordered.size();
        if (chunkSize != 0)
            itemIndex = myCombo / chunkSize;
        else
            itemIndex = 0;

        //### Copy the item, remove it from list ###
        result.push_back(unordered.at(itemIndex));
        unordered.removeAt(itemIndex);

        //### Update the number of combinations ###
        numCombos = 1;
        for (int i = 1; i <= unordered.size(); i++)
            numCombos *= i;

        //### Match my combo to the new number of combinations ###
        myCombo = myCombo % chunkSize;
    }//while size()

    return result;
}//orderElements

template <class T>
T MathHelper::calcMedian(std::vector<T> theItems)
{
    typedef typename std::list<T>::iterator MHListIter;

    std::list<T> remainingItems;//a copy of theItems, we'll remove elements one by one as we sort
    std::vector<T> sortedItems;//contains elemens from theItems in ascending order
    MHListIter remainingIter;//iterates over the elements in remainingItems
    MHListIter smallestPos;//the position of smallest in remainingItems
    T smallest = 0;//The smallest value in remainingItems
    unsigned int midSize = 0;//The number of elements in the first half of theItems

    //### Copy every element in theItems to remainingItems ###
    for (unsigned int i = 0; i < theItems.size(); i++)
        remainingItems.push_back(theItems.at(i));

    //### Find the center index in theItems ###
    if (theItems.size() > 0)
    {
        midSize = theItems.size() / 2;

        if (midSize % 2 != 0)
            midSize += 1;
    }//if size();

    //### Sort theItems in ascending order ###
    while (sortedItems.size() < midSize)
    {
        //Find the smallest remaining item
        remainingIter = remainingItems.begin();

        smallest = *remainingIter;
        smallestPos = remainingIter;

        while (remainingIter != remainingItems.end())
        {
            if (*remainingIter < smallest)
            {
                smallest = *remainingIter;
                smallestPos = remainingIter;
            }//if *remainingIter

            remainingIter++;
        }//while remainingIter

        sortedItems.push_back(smallest);
        remainingItems.erase(smallestPos);
    }//while size()

    return sortedItems.at(sortedItems.size() - 1);
}//calcMedian

#endif
