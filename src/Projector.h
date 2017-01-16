/** \file Projector.h
  *
  * \date 07/04/2015
  *
  * \author Etienne Peillard
  *
  * \version 0
  *
  * Declaration of the class Projector
  **/
#ifndef PROJECTOR_H
#define PROJECTOR_H

//OpenCV includes
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

//Aruco includes
#include <aruco/aruco.h>
#include <aruco/arucofidmarkers.h>

#include <iostream>
#include <vector>

//rammco includes
#include "define.h"

namespace rammco{

/** \class  rammco::Projector Projector.h Projector
  *
  * \brief Class containing the drawing functions of the project.
  *
  * This class give function to display throw the projector the informatuions on the goban
  **/
class Projector{

public:

    /** \fn Projector()
      * Main class constructor
      **/
    Projector();
    /** \fn ~Projector()
      * Main class destructor
      **/
    ~Projector();

    /**\fn void draw(int, int=0, int=0, int=0)
      * Depreciate function for drawing the goban informations
      * \arg code of the projection mod
      * \arg x coordinate
      * \arg y coordinate
      * \arg
      **/
    void draw(int, int=0, int=0, int=0);
    
    /**\fn void draw(int, vector<int> &)
      * Only used to draw an Arruco marker filled board
      * \arg code of the projection mod
      * \arg list of the marker centers
      * \arg list of the id
      **/
    void draw(int, std::vector<cv::Point> &, std::vector<int> &);

    /** \fn draw(Mat *mat)
      * Drawing function. It draw on the projector screen
      * \arg Mat image to draw
      **/
    void draw(cv::Mat *mat); //TODO why * ?
    
    void draw(std::string);

    /** \fn void seR2P(Mat*)
      * Setter for the R2P matrix
      * \arg R2P matrix
      **/
    void setR2P(cv::Mat*);
    
    void setI2P(cv::Mat*);

    cv::Mat matDraw; //TODO faire un getter
    
    /** \fn void checkCalib()
      * Diplay white circle on each corner
      **/
    void checkCalib();

private:

    cv::Mat* R2P;
    cv::Mat* I2P;

};

}

#endif // PROJECTOR_H
