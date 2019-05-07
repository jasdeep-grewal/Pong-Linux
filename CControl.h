#pragma once
/** CControl.cpp
 *
 * @brief A control class that gets or sets the data by opening up a comport.
 *
 * @author Jasdeep Grewal
 *
 * @version 1.0 -- 25 January 2019
 */
#include "stdafx.h"

#include <string>
#include <iostream>
#include <thread>

#include "Client.h"
#include "Server.h"
#include "Serial.h" // Must include Windows.h after Winsock2.h, so Serial must include after Client/Server

// OpenCV Include
#include "opencv.hpp"

// OpenCV Library

class CControl {
private:
	Serial _com;

public:
	CControl();
	~CControl();
	void init_com(int comport);
	/** Gets a comport.
	*
	* @param opens comport
	* 
	*
	* @return Returns nothing
	*/
	bool get_data(int type, int channel, int &result);
	/** Gets data.
	*
	* @param Gets values from type, channel, and result
	*
	*
	* @return Type, channel, and value if true
	*/
	bool set_data(int type, int channel, int val);
	/** Sets data.
	*
	* @param Sets value of type, channel, and value
	*
	*
	* @return Type, channel, and value if true
	*/
	double get_analog(int);
};