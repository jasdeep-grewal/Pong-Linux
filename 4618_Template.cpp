////////////////////////////////////////////////////////////////
// ELEX 4618 Template project for BCIT
// Created Oct 5, 2016 by Craig Hennessey
// Last updated March 23, 2017
////////////////////////////////////////////////////////////////
#include "stdafx.h"

#include <string>
#include <iostream>
#include <thread>

#include "Client.h"
#include "Server.h"
#include "Serial.h" // Must include Windows.h after Winsock2.h, so Serial must include after Client/Server
#include "CControl.h"
#include <iomanip>
#include <conio.h>
#include <cmath>
#include "CSketch.h"
#include "CPong.h"

// OpenCV Include
#include "opencv.hpp"


// OpenCV Library
#pragma comment(lib,".\\opencv\\lib\\opencv_world310d.lib")
using namespace std;

enum type { DIGITAL, ANALOG, SERVO };

CControl device;


void process_msg()
{
  MSG msg;
  while (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
  {
    ::TranslateMessage(&msg);
    ::DispatchMessage(&msg);
  }
}

////////////////////////////////////////////////////////////////
// Serial Communication
////////////////////////////////////////////////////////////////
void test_com()
{
  char command;
  
  do {
	  cout << "\n****************************************" << endl << endl;
	  cout << "ELEX4618 Lab 3 Menu" << endl << endl;
	  cout << "****************************************" << endl << endl;
	  cout << "(A) Test Analog" << endl << endl;
	  cout << "(B) Test Digital" << endl << endl;
	  cout << "(C) Test Button" << endl << endl;
	  cout << "(D) Test Servos" << endl << endl;
	  cout << "(Q) Quit" << endl << endl;

	  char check;
	  cout << "CMD> ";
	  cin >> check;

	  int data;
	  int buttonCount = 0;
	  int max = 180;
	  int min = 0;

	
	  do
	  {
		  switch (toupper(check))
		  {
		  case 'A':
			  device.get_data(ANALOG, 9, data);
			  cout << "ANALOG TEST: CH 9 = " << data << "(" << data * 100 / 1024 << "%)";

			  device.get_data(ANALOG, 15, data);
			  cout << "CH 15 = " << data << "(" << data * 100 / 1024 << "%)\n";
			  break;

		  case 'B':
			  device.get_data(DIGITAL, 32, data);
			  device.set_data(DIGITAL, 39, data);
			  cout << "DIGITAL TEST: CH" << "?" << " = " << data << "\n";
			  break;

		  case 'C':
			  device.get_data(DIGITAL, 33, data);

			  if (data == 0) {
				  Sleep(200);
				  if (data == 0)
				  {
					  buttonCount++;
					  cout << "BUTTON TEST: " << buttonCount << "\n";
				  }
			  }

			  break;

		  case 'D':
			  //cout << "Enter Angle : ";
			  //cin >> data;
			  //device.set_data(SERVO, 19, data);
			  
			  for (int angle = 1; angle < max; angle++) {
				  device.set_data(SERVO, 19, angle);
				  if (angle = 176) {
					  for (int angle2 = angle; angle2 > min; angle2--) {
						  device.set_data(SERVO, 19, angle2);
					  }
				  }
			  }
				
			  
			  break;
		  case 'Q':
			  return;
		  }

	  } while (!kbhit()) ;
  } while (1);
}

////////////////////////////////////////////////////////////////
// Display Image on screen
////////////////////////////////////////////////////////////////
void do_image()
{
	cv::Mat im;

	im = cv::imread("BCIT.jpg");

	srand(time(0));

	for (int i = 0; i < 500; i++)
	{
		float radius = 50 * rand() / RAND_MAX;
		cv::Point center = cv::Point(im.size().width*rand() / RAND_MAX, im.size().height*rand() / RAND_MAX);

		cv::circle(im, center, radius, cv::Scalar(200, 200, 200), 1, cv::LINE_AA);

		im.at<char>(i, i) = 255;

		cv::imshow("Image", im);
		cv::waitKey(1);
	}
}

////////////////////////////////////////////////////////////////
// Display Video on screen
////////////////////////////////////////////////////////////////
void do_video()
{
  cv::VideoCapture vid;

  vid.open(0);

  if (vid.isOpened() == TRUE)
  {
    do
    {
      cv::Mat frame, edges;
      vid >> frame;
      if (frame.empty() == false)
      {
        cv::cvtColor(frame, edges, cv::COLOR_BGR2GRAY);
        cv::GaussianBlur(edges, edges, cv::Size(7, 7), 1.5, 1.5);
        cv::Canny(edges, edges, 0, 30, 3);
        cv::imshow("edges", edges);
      }
    }
    while (cv::waitKey(10) != ' ');
  }      
}		
    

////////////////////////////////////////////////////////////////
// Demo client server communication
////////////////////////////////////////////////////////////////
Server serv;

// Start TCP server
void serverfunc()
{
  serv.start(4618);
}

// Send image to TCP server
void serverimagefunc()
{
  cv::VideoCapture vid;

  vid.open(0);

  if (vid.isOpened() == TRUE)
  {
    do
    {
      cv::Mat frame;
      vid >> frame;
      serv.set_txim(frame);
    }
    while (cv::waitKey(10) != ' ');
  }
}

void clientserver()
{
  std::string str;
  cv::Mat im;

  // Start server
  std::thread t1(&serverfunc);
  t1.detach();

  // Start image send to server
  std::thread t2(&serverimagefunc);
  t2.detach();

  // Wait until server starts up (webcam is slow)
  Sleep(2000);

  // connect
  Client client(4618, "127.0.0.1");
  //Client client(4618, "192.168.1.80");

  // Wait until server starts up (webcam is slow)
  Sleep(500);

  client.tx_str("cmd");
  do
  {
    client.rx_str(str);
    if (str.length() > 0)
    {
      std::cout << "\nClient Rx: " << str;
    }
  } while (str.length() == 0);

  int count = 0;
  while (1)
  {
    client.tx_str("im");

    if (client.rx_im(im) == TRUE)
    {
      if (im.empty() == false)
      {
        count++;
        std::cout << "\nImage received: " << count;
        cv::imshow("rx", im);
        cv::waitKey(100);
      }
    }
  }
}

int main(int argc, char* argv[])
{
	int x, y, comport;
	x = 1000;
	y = 800;
	comport = 3;
	/*cout << "Enter X size: ";
	cin >> x;
	cout << "\nEnter Y size: ";
	cin >> y;
	cout << "\nEnter comport number: ";
	cin >> comport;
	CSketch sketch(cv::Size(x, y), comport);
	
	sketch.run();
	*/
	CPong pong(cv::Size(x, y), comport);
	pong.run();

	//device.init_com(3);
	//test_com();
	//do_image();
	//do_video ();
  //clientserver();
}
