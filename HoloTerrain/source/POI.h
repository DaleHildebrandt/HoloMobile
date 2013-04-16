#include "Cube.h"

class POI : public Cube{

public:
	POI::POI(float size, std::string text);
	POI::~POI(void);
	void display();

private:
	std::string text;


}