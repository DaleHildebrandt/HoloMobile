#pragma once
class Color3f
{
public:
	Color3f(float red, float green, float blue);
	~Color3f(void);

	float getRed(void);
	float getGreen(void);
	float getBlue(void);

	void setRed(float red);
	void setGreen(float green);
	void setBlue(float blue);
	void setRGB(float color);
	void setRGB(float red, float green, float blue);

private:
	float red;
	float green;
	float blue;
};

