
#include "stdafx.h"

#include <string>
#include <iostream>
#include <thread>
#include "CControl.h"
#include "Client.h"
#include "Server.h"
#include "Serial.h" // Must include Windows.h after Winsock2.h, so Serial must include after Client/Server
using namespace std;
// OpenCV Include
#include "opencv.hpp"

CControl::CControl()
{

}
CControl::~CControl()
{
	gpioTerminate();
}

void CControl::init_com(int comport) {
	if (gpioInitialise() < 0)
	{
		return;
	}

	gpioSetMode(PINOUT1, PI_OUTPUT); // Setup PINOUT1 as Output

	gpioSetMode(PININ1, PI_INPUT); // Setup PININ1 as Input

	gpioSetMode(PIN_SERVO1, PI_OUTPUT);
}

bool CControl::get_data(int type, int channel, int &result) {

	switch (type) 
	{
		case DIGITAL :  result = gpioRead(channel);
						break;

		case ANALOG :  	unsigned char inBuf[3];
						char cmd[] = { channel, 128, 0 }; // 0b1XXX0000 where XXX=channel 0

						int handle = spiOpen(0, 200000, 3); // Mode 0, 200kHz

						spiXfer(handle, cmd, (char*)inBuf, 3); // Transfer 3 bytes

						result = ((inBuf[1] & 3) << 8) | inBuf[2] ; // Format 10 bits

						spiClose(handle); // Close SPI system

						break;
	}
}

bool CControl::set_data(int type, int channel, int val) {

	switch (type)
	{
	case DIGITAL:  gpioWrite(channel, val);
		break;

	case SERVO:  gpioServo(channel, val);
		break;
	}

	return true;

}

double CControl::get_analog(int channel)
{
	int data;

	get_data(1, channel, data);

	return data * 100 / 1023;
}


