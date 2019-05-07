/** CPong.h
*
* @brief A class which is used to implement Pong.
*
* This class inherits from the base class called CBase4618.h and
* overrides the functions.
*
* @author Jasdeep Grewal
*
* @version 1 -- 8 February 2019
*/#pragma once
#include "stdafx.h"
#include "CControl.h"
#include "CBase4618.h"

class CPong : public CBase4618
{
private:
	int _com;
	cv::Size _canvasSize;
public:
	/** Creates constructor for canvas
	*
	* @param Canvas size is initialized
	* @param Comport number is initialized
	*
	* @return Does not return anything
	*/
	CPong(cv::Size, int);
	~CPong();
	/** Updates Y coordinates
	*
	* @param Gets analog inputs
	* @param Updates joysyick position continuously to mimic an Etch-A-Sketch
	*
	* @return Does not return anything
	*/
	void update(double &xPosition, double &yPosition);
	/** Moves paddle based on joystick position
	*
	* @param Repeatedly gets the analog input from the joystick
	* @param Moves paddle using the analog input
	*
	* @return Does not return anything
	*/
	void draw(int x, int y);
	/** Button debounce
	*
	* @param Uses number of ticks to debounce pushbutton
	*
	* @return True if button was pressed
	*/
	bool btnPressed(int gpioPin);
};