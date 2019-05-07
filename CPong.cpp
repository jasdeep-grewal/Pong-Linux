#include "stdafx.h"
#include <iostream>
#include <cstdlib>
#include "CPong.h"
#include <string>
using namespace std;


CPong::CPong(cv::Size _canvasSize2, int _comport)
{
	_com = _comport;
	_canvasSize = _canvasSize2;
	_cctrl.init_com(_com);

	_canvas.zeros(_canvasSize, CV_8UC3);

	cv::Mat img(_canvasSize, CV_8UC3, cv::Scalar(0, 0, 0)); // got from https://www.geeksforgeeks.org/opencv-c-plus-plus-program-to-create-a-single-colored-blank-image/
	_canvas = img;
}

CPong::~CPong()
{

}

void CPong::update(double &xPosition, double &yPosition)
{
	yPosition = 100 - _cctrl.get_analog(9);
}

void CPong::draw(int x, int y)
{
	cv::line(_canvas, cv::Size(500, 0), cv::Size(500, 800), cv::Scalar(255, 255, 255), 1, 1, 0);
	double elapsed_time;
	static double freq = cv::getTickFrequency(); // Get tick frequency
	static double start_tic = cv::getTickCount(); // Get number of ticks since event (such as computer on)
									// Code under test
	elapsed_time = (cv::getTickCount() - start_tic) / freq;
	//cout << elapsed_time << endl;
	static int currentTime;
	currentTime = elapsed_time;
	static int previousTime = 0;
	static int frames = 1;
	static int delay = 17;
	static string fps;
	static int speed = 30;
	
	//cout << "Current time: " << currentTime << "          Previous time: " << previousTime << endl;
	if (currentTime > previousTime)
	{
		previousTime = currentTime;
		cout << "FPS is: " << frames << endl;
		fps = to_string(frames);
		speed = frames;
		double temp = frames / 30.0;
		temp = 1.0 / temp;
		temp = 1.0 - temp;
		delay += (temp / 30.0) * 1000;
		if (delay < 0)
			delay = 14;
		frames = 1;
	}
	else
	{
		frames++;
	}

	string str = "FPS: " + fps;
	static int P1score = 0;
	string Player1 = "Player 1: " + to_string(P1score);
	static int P2score = 0;
	string Player2 = "Player 2: " + to_string(P2score);

	cv::putText(_canvas, str, cv::Size(850, 30), CV_FONT_HERSHEY_PLAIN, 2, cv::Scalar(255, 255, 255), 2, cv::LINE_AA);
	cv::putText(_canvas, Player1, cv::Size(10, 30), CV_FONT_HERSHEY_PLAIN, 2, cv::Scalar(255, 255, 255), 2, cv::LINE_AA);
	cv::putText(_canvas, Player2, cv::Size(510, 30), CV_FONT_HERSHEY_PLAIN, 2, cv::Scalar(255, 255, 255), 2, cv::LINE_AA);

	static int x2 = 0, y2 = 0;
	int x3 = 10;
	int y3 = 50;
	static bool firstTime = true;
	static int step = 5;
	static int rct = 0;

	/*if (rct == 0)
		step = 5;
	if (rct == 700)
		step = -5;
	rct += step;
	*/
	

	if (firstTime)
	{
		x2 = x;
		y2 = y;
		firstTime = false;
		return;
	}

	static float xVel = 200;
	static float yVel = -250;
	static float xPos = 500;
	static float yPos = 400;
	xPos = xPos + (xVel/speed);
	yPos = yPos + (yVel/speed);

	string p2win = "Player 2 won!";

	if (yPos < 30)
		yVel = -yVel;
	if (xPos > 960)
		xVel = -xVel;
	if (yPos > 770)
		yVel = -yVel;
	if (xPos < 0)
	{
		xPos = 500;
		yPos = 400;
		P2score++;
		if (P2score == 5)
		{
			cv::putText(_canvas, p2win, cv::Size(350, 400), CV_FONT_HERSHEY_PLAIN, 3, cv::Scalar(255, 255, 255), 2, cv::LINE_AA);
			xPos = 500;
			yPos = 400;
			P2score = 0;
			P1score = 0;
			xVel = 200;
			yVel = -250;
		}
	}
	if (((xPos < 45) && (xPos > 38)) && ((yPos < (y + 85)) && (yPos > (y - 85))))
		xVel = -xVel;
		
	if (btnPressed(0))
	{
		P1score = 0;
		P2score = 0;
		xPos = 500;
		yPos = 400;
	}

	cv::rectangle(_canvas, cv::Point(980, 50 + yPos), cv::Point(990, yPos - 50), cv::Scalar(255, 255, 255), CV_FILLED, 0);

	/*static int xPos = 500, yPos = 400;
	int xshift = 6, yshift = 10;
	xPos += xshift;
	yPos += -yshift;
	if (yPos < 0 || xPos > 728)
		yPos += 2 * yshift;
	if (yPos > 400 || xPos > 970)
	{
		yPos += yshift;
		xPos -= 2*xshift+2;
	}
	else if (yPos > 760 && xPos < 728)
	{
		yPos += 4 * yshift;
		xPos += 2 * xshift;
	}*/
	cv::circle(_canvas, cv::Point(xPos, yPos), 20, cv::Scalar(255, 255, 255), CV_FILLED, 1, 0);

	if ((y + 50) > 800)
		y = 750;
	else if ((y - 50) < 0)
		y = 50;

	cv::rectangle(_canvas, cv::Point(10, y-50), cv::Point(20, 50+y), cv::Scalar(255, 255, 255), CV_FILLED, y);
	cv::imshow("Image", _canvas);
	x2 = x;
	y2 = y;
	_canvas = cv::Mat::zeros(_canvas.size(), _canvas.type());
	Sleep(delay);
}

bool CPong::btnPressed(int gpioPin)
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