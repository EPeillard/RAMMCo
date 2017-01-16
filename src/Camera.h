/** \file Camera.h
  *
  * \date 07/04/2015
  *
  * \author Etienne Peillard
  *
  * \version 0
  *
  * Declaration of the class Camera
  **/

//#define PICAMERA

#ifndef CAMERA_H
#define CAMERA_H

//OpenCV include
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>
#include <sstream>
#include <time.h>
#include <stdio.h>

#include <string.h>
#include <opencv2/calib3d/calib3d.hpp>

#include <stdexcept>
#include <thread>

#ifdef PICAMERA
#include <raspicam/raspicam_cv.h>
#endif

#include "define.h"

namespace rammco{

/** \class  rammco::Camera Camera.h Camera
  *
  * \brief Class representing the camera and all the functions associated
  *
  * This class contain all the Camera function from getting a frame to the calibration of the camera itself
  **/
class Camera{

public:

    /** \fn Camera()
      * Main constructor
    **/
    Camera(int id = 0);

    /** \fn Camera()
      * Main destructor
    **/
    ~Camera();

#ifndef PICAMERA
    /** \fn nextCam()
      * Use an other camera
    **/
    void nextCam();
#endif

    /** \fn IplImage* getFrame()
      * Give back a image just taken from the camera
      *
      **/
    cv::Mat getFrame();

#ifndef PICAMERA
    /** \fn close()
      * Close the steam to the camera
      **/
    void close();

    /** \fn void genYML(int, char**)
      * This function is used to calibrate the camera itself.
      * When you launch this function, you need to show a chessboard to the camera. It will taken image to calibrate itself.
      **/
    void genYML(int, char**);

    /** \fn Mat correction(IplImage)
      * Be careful, this function DOESN'T WORK, it normally corrects an image just taken.
    **/
    cv::Mat correction(IplImage);
#endif

private:
  
    /** Most recent frame
    **/
    cv::Mat frame;

#ifndef PICAMERA
    /** Camera stream for taking frames
    **/
    cv::VideoCapture capture;

    /** Camera id
    **/
    int id;

    /** Nb test until throwing exception for no cam found
    **/
    int nbTests;

    /** \fn void emptyBuffer();
      * This function empty the image buffer of the camera stream.
      **/
    void emptyBuffer();

    bool refreshing;
#else
    raspicam::RaspiCam_Cv capture;
#endif
};

}

#endif // CAMERA_H
