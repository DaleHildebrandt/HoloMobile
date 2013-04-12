#ifndef MODEL_LOADER_HPP
#define MODEL_LOADER_HPP

#include <string>
#include "../EbonGL/EbonGL.hpp"

using namespace std;

class ModelLoader
{
public:
	ModelLoader(const string &newConfigFile);
	void init();
	void contextInit(unsigned int myContext);
	void postInit(EbonGL::EG_Engine *glEngine);
	~ModelLoader(void);

	static const unsigned int NUM_CONTEXTS = 4;

private:
	std::string configFile;
	EbonGL::EG_Model *theModel;

	double loadDouble(const std::string &valueLabel);
};

#endif