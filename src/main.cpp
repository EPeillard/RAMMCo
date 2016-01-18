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
    string rep;
    
    Projector* proj;
    Camera* camera;
    Simulation* simu;
    Core* core;
    
    while(rep!="o"&&rep!="O"){
    
      int id = 0;
      
      cout << "Camera n° ?";
      cin >> id; 
      
      proj = new Projector();
      camera = new Camera(id);
      simu = new Simulation();
      core = new Core(camera, proj, simu);

      cout << "Calibration de la camera ..." << endl; 
      core->init();
      cout << "Fait" << endl; 

      cout << "Calibration du projecteur ..." << endl; 
      core->detection();
      cout << "Fait" << endl; 
      
      core->genConvMat();
      proj->setR2P(core->getR2PMat());
      
      //Check the calibration
      proj->checkCalib();
      cout<<"La calibration est-elle correcte ? [o/N]  " << endl; //TODO fix si non (plante)
      cin >> rep; 
    }
    
    cout << "Début de la simulation" << endl; 
    
    waitKey(100);
    
    core->initSimu();
    
    while(1)
    {
      core->loopSimu();
    }

    return(0);
}