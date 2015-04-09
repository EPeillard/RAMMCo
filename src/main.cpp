/**
  * Function for the calibration between the goban, the camera and the projector
  *
  * @author Etienne Peillard
  *
  **/

#include "define.h"
#include "Core.h"
#include "Camera.h"
#include "Simulation.h"
#include "Projector.h"

#include <iostream>
#include <sstream>
#include <time.h>
#include <stdio.h>

#include <string.h>

#include <sys/time.h>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

using namespace cv;
using namespace std;
using namespace rammco;

/** @function main */
int main(int argc, char** argv)
{
    Projector* proj = new Projector();
    Camera* camera = new Camera();
    Simulation* simu = new Simulation();
    Core* core = new Core(camera, proj, simu);

    ///Calibration of the camera
    core->init();

    ///Calibration of the projector
    core->detection();
    
    proj->setR2P(core->getR2PMat());
    
    //TODO Check the calibration

    waitKey(1000);

    return(0);
}