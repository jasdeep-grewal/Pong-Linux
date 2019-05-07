#pragma once
#include "stdafx.h"
#include "CControl.h"
#include "CBase4618.h"

class CSketch : public CBase4618
{
private:
	// future data members
	int _com;
	cv::Size _canvasSize;
	int color ;

public:
	CSketch(cv::Size, int);
	~CSketch();
	void update(double &xPosition, double &yPosition);
	void draw(int x, int y);
	bool btnPressed(int gpioPin);
};
