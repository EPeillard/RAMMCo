/** \file Core.h
  *
  * \date 07/04/2015
  *
  * \author Etienne Peillard
  *
  * \version 0
  *
  * Declaration of the class Core
  **/

#ifndef CORE_H
#define CORE_H

#include "opencv2/calib3d/calib3d.hpp"
#include <aruco/aruco.h>

#include "define.h"
#include "Camera.h"
#include "Projector.h"
#include "Simulation.h"

using namespace cv;
using namespace std;
using namespace aruco;
/** \namespace rammco
  *
  * namespace used to define all the functions used in the rammco project
  *
  *
  **/
namespace rammco{

class Simulation;

/** \class  rammco::Core Core.h Core
  *
  * \brief Class containing the core functions of the project.
  *
  * This class give all the functions that give back the coordinates of the goban (camera, projector, VirtualGoban, goban coordinates).
  * It also give the matrix permitting the convertion between two system of coordintes.
  *
  * Other functions are use to detect stones and hands on the goban.
  **/
class Core{

public:

///Constructors and destructors

    /** \fn Core::Core(Camera* camera, Projector* proj, Simulation* simu)
      *
      * \brief Main void constructor
      * Main constructor of the Core class. It initialize all the field for initialization and detection.
      *
      * \param camera Pointer to the Camera object used to get image from the device.
      * \param proj Pointer to the Projector object used to display images.
      * \param simu Pointer to the Simulation object containing the simlation datas.
      *
      **/
    Core(Camera*, Projector*, Simulation*);

    /** \fn ~Core()
      * \brief Main void destructor
      * Main destructor of the Core class
      **/
    ~Core();

///Matrix for conversion between two coordinate system

    /** \fn void genConvMat()
      * \brief Generation of the conversion matrix
      * Generation of all the conversion matrix to change the coordinate
    **/
    void genConvMat();

///Main Core functions for the calibration of the camera/projector/goban

    /** \fn void init()
      * \brief Initialization
      * Initialization before the detection.
      * Adjust camera detection
    **/
    void init();

    /** \fn void detection()
      * \brief Detection by displaying markers
      * A marker is displayed and then it's position in the Camera system is saved.
      * It generates a list of coordonated points which allow to find a tranformation matrix.
    **/
    void detection();
    
    /**	\fn Mat getR2PMat()
     * \brief Get the Real to Projector Matrix
     * */
    Mat* getR2PMat();

private:
    /**	\brief Source Mat for image processing
    **/
    Mat src;
    /**	\brief Source Mat converted in gray
    **/
    Mat src_gray;
    /**	\brief Mat used to display calibration to the user
    **/
    Mat display;

    /**	\brief Matrix for the conversion from Projector to Camera
    **/
    Mat P2C;
    /**	\brief Matrix for the conversion from Camera to Real
    **/
    Mat C2R;    
    /**	\brief Matrix for the conversion from Real to Camera
    **/
    Mat R2C;
    /**	\brief Matric for the conversion from Real to Projector
     * */
    Mat R2P;
    
    /**	\brief Camera object of the project
    **/
    Camera* camera;
    /**	\brief Projector object of the project
    **/
    Projector* proj;
    /**	\brief Simulation object of the project
    **/
    Simulation* simu;

};

}

#endif // CORE_H
