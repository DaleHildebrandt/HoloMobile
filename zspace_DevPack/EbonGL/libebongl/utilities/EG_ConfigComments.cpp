#include "EG_ConfigComments.hpp"

using namespace std;
using namespace EbonGL;

EG_ConfigComments::EG_ConfigComments(void)
{
}//constructor

EG_ConfigComments::~EG_ConfigComments(void)
{
}//destructor

void EG_ConfigComments::add(std::string newComment)
{
    theComments.push_back(newComment);
}//add

string EG_ConfigComments::get(int index)
{
    return theComments.at(index);
}//get

unsigned int EG_ConfigComments::size(void)
{
    return static_cast<unsigned int>(theComments.size());
}//size
