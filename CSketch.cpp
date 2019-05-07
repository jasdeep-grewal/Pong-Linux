#include "stdafx.h"
#include "CSketch.h"
#include <iostream>
#include <cstdlib>
#include <cmath>

CSketch::CSketch(cv::Size _canvasSize2, int _comport)
{
	color = 1;
	_com = _comport;
	_canvasSize = _canvasSize2;
	
	_cctrl.init_com(_com);

	int x = (sqrt(2) / 2) * _canvasSize2.width;
	int y = (sqrt(2) / 2) * _canvasSize2.height;
	
	_canvas.zeros(cv::Size(x, y), CV_8UC3);

	cv::Mat img(cv::Size(x, y), CV_8UC3, cv::Scalar(0,0,0)); // got from https://www.geeksforgeeks.org/opencv-c-plus-plus-program-to-create-a-single-colored-blank-image/

	_canvas = img;

	//_canvas = cv::imread("BCIT.jpg");
//	_canvas = cv::Mat::zeros(_canvas.size(), _canvas.type());
	
}
CSketch::~CSketch()
{

}

void CSketch::update(double &xPosition, double &yPosition)
{
	xPosition = _cctrl.get_analog(15);
	yPosition = 100 - _cctrl.get_analog(9);

	int data;
	//int color = 1;
	_cctrl.get_data(0, 33, data);

	//if (data == 0) {
	if(btnPressed(0)){
		color++;
		if (color == 4)
			color = 1;
	}
	if (color == 1)
	{
		_cctrl.set_data(0, 39, 1);
		_cctrl.set_data(0, 38, 0);
		_cctrl.set_data(0, 37, 0);

	}
	else if (color == 2)
	{
		_cctrl.set_data(0, 39, 0);
		_cctrl.set_data(0, 38, 1);
		_cctrl.set_data(0, 37, 0);
	}
	else if (color == 3)
	{
		_cctrl.set_data(0, 39, 0);
		_cctrl.set_data(0, 38, 0);
		_cctrl.set_data(0, 37, 1);
	}
	int clear;
	_cctrl.get_data(0, 32, clear);
	if (clear == 0)
		_canvas = cv::Mat::zeros(_canvas.size(), _canvas.type()); // got from http://answers.opencv.org/question/88254/most-efficient-way-to-clear-an-image-with-c-interface/
}

void CSketch::draw(int x, int y)
{
	static int x2 = 0, y2 = 0;
	static bool firstTime = true;

	if (firstTime)
	{
		x2 = x;
		y2 = y;
		firstTime = false;
		return;
	}

	cv::Scalar R = cv::Scalar(0, 0, 255);
	cv::Scalar G = cv::Scalar(0, 255, 0);
	cv::Scalar B = cv::Scalar(255, 0, 0);

	if (btnPressed(0)) {
		color++;
		if (color == 4)
			color = 1;
	}

	if(color == 1)
		cv::line(_canvas, cv::Point(x2, y2), cv::Point(x, y), R, 2, 1, 0);
	else if(color == 2)
		cv::line(_canvas, cv::Point(x2, y2), cv::Point(x, y), G, 2, 1, 0);
	else if(color == 3)
		cv::line(_canvas, cv::Point(x2, y2), cv::Point(x, y), B, 2, 1, 0);
	
	//cv::circle(_canvas, cv::Point(200, 350), 1, R, 1, cv::LINE_AA);
	//cv::line(_canvas, cv::Point(x2, y2), cv::Point(x, y), R, 2, 1, 0);
	//cv::circle(_canvas, cv::Point(x, y) , 1, cv::Scalar(0, 255, 0), 1, cv::LINE_AA);
	cv::imshow("Image", _canvas);
	x2 = x;
	y2 = y;
}
bool CSketch::btnPressed(int gpioPin)
{
	int data;
	static int timerCount = 1;
	static bool statusChanged = false;

	_cctrl.get_data(0, 33, data);

	if (data == 0)
	{
		if ((timerCount == 0) && (!statusChanged))
		{
			statusChanged = true;
			return true;
		}
		else
		{
			timerCount--;
		}
	}
	else
	{
		timerCount = 1;
		statusChanged = false;
	}
	return false;
}