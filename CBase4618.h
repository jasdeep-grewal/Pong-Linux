/** CBase4618.h
 *
 * @brief This class is a base class for future labs
 * which contains functions and members that can be
 * used by the derived class
 *
 * @author Jasdeep Grewal
 *
 * @version 1 -- 7 February 2019
 */
#pragma once
#include "stdafx.h"
#include "CControl.h"
#include "opencv.hpp"


class CBase4618
{
protected:
	CControl _cctrl;
	cv::Mat _canvas;

public:
	/** Creates contstructor for canvas and comport
	*
	* @param Canvas size is initialized
	* @param Comport number is initailized
	*
	* @return Does not return anything
	*/
	CBase4618(CControl _cctrl, cv::Mat _canvas);
	CBase4618();
	~CBase4618();
	/** Updates X and Y coordinates
	*
	* @param Gets analog inputs
	* @param Updates them continuously to mimic an Etch-A-Sketch
	*
	* @return Does not return anything
	*/
	virtual void update(double &, double &);
	/** Draws lines based on X and Y coordinates
	*
	* @param Repeatedly gets analog inputs and determines X and Y coordinates
	* @param Makes a line using the X and Y coordinates
	*
	* @return Does not return anything
	*/
	virtual void draw(int, int);
	/** Runs the draw and update function
	*
	* @param Runs functions until q is pressed to quit
	*
	* @return Does not return anything
	*/
	void run();
};