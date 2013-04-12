#include "EG_SceneElement.hpp"
#include "EG_Utils.hpp"
#include "utilities/EG_LogWriter.hpp"
#include "utilities/EG_String.hpp"

using namespace std;
using namespace EbonGL;

EG_SceneElement::EG_SceneElement(void)
{
	haveEngine = false;
	engineID = 0;
	isSolid = true;
}//constructor

EG_SceneElement::~EG_SceneElement(void)
{
}//destructor

void EG_SceneElement::setEngine(unsigned int newEngineID,
                                list<EG_Object*>::iterator newEngineIter,
							    vector<list<EG_Object*>::iterator> newSafeIters,
							    bool newIsSolid)
{
	if (haveEngine)
	{
		EG_Utils::writeLog(EG_String("EG_SceneElement::setEngine(): this object already belongs to an Engine"));
		EG_Utils::fatalError();
	}//if haveEngine

	engineID = newEngineID;
	engineIter = newEngineIter;
	for (unsigned int i = 0; i < newSafeIters.size(); i++)
		safeIters.push_back(newSafeIters.at(i));
	isSolid = newIsSolid;
	haveEngine = true;
}//setEngine

void EG_SceneElement::removeEngine(unsigned int removeID,
							       list<EG_Object*>::iterator &removeIter,
								   vector<list<EG_Object*>::iterator> &removeSafe)
{
	removeSafe.clear();

	if (!haveEngine)
	{
		EG_Utils::writeLog(EG_String("EG_SceneElement::removeEngine(): this object doesn't belong to any Engine!"));
		EG_Utils::fatalError();
	}//if !haveEngine

	if (removeID != engineID)
	{
		EG_Utils::writeLog(EG_String("Object::removeEngine(): request made by wrong Engine!?"));
		EG_Utils::fatalError();
	}//if removeID

	haveEngine = false;
	removeIter = engineIter;

	for (unsigned int i = 0; i < safeIters.size(); i++)
		removeSafe.push_back(safeIters.at(i));
	safeIters.clear();
}//removeEngine

bool EG_SceneElement::getIsSolid(void)
{
	return isSolid;
}//getIsSolid
