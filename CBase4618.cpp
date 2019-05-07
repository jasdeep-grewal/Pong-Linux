#include "stdafx.h"
#include "CBase4618.h"


CBase4618::CBase4618(CControl _cctrlz, cv::Mat _canvasz)
{
	_cctrl = _cctrlz;
	_canvas = _canvasz;

}
CBase4618::CBase4618() {}
CBase4618::~CBase4618()
{

}

void CBase4618::update(double &xPosition, double &yPosition)
{

}

void CBase4618::draw(int x, int y)
{

}

void CBase4618::run()
{
	double  x, y;

	do {
		update(x, y);

		x = x * _canvas.size().width / 100;
		y = y * _canvas.size().height / 100;

		draw(x, y);
	
	} while(cv::waitKey(1) != 'q');
}