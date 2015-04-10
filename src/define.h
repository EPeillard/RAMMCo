/**
 * This file only contains define values
 *
 */


#ifndef DEFINE_H
#define DEFINE_H

#include <aruco/aruco.h>

#define KEY_ESCAPE		27

///Adjust brightness
#define GRAY_SCALE		0.7

///Projector mod drawing the detection point
#define PROJ_MOD_DETECTION	1
///Projector mod drawing a Aruco marker
#define PROJ_MOD_MARKER		2
///Projector mod drawing an empty frame
#define PROJ_MOD_NOTHING	3
///Projector mod drawing a board of Aruco markers
#define PROJ_MOD_BOARD		4

///Number of boards used
#define NB_BOARD		1

//TODO : use const declaration and dynamic solve of new boards

///Markers number of the first kind of board
#define N_MARKER_B1_UL		25
#define N_MARKER_B1_UR		26
#define N_MARKER_B1_LL		27
#define N_MARKER_B1_LR		28

///Marker position on the board (in cm)
const cv::Point2f MARKER_B1_UL(0,0);
const cv::Point2f MARKER_B1_UR(0,50);
const cv::Point2f MARKER_B1_LL(50,0);
const cv::Point2f MARKER_B1_LR(50,50);

///Ratio of the windows size for the marker size
#define RATIO_MARKER_SIZE	0.1

///Windows names
#define WINDOW_CAMERA 		"Vue caméra"
#define WINDOW_PROJECTOR 	"Affichage projecteur"
#define WINDOW_VERBOSE 		"Mode verbose"
#define WINDOW_VG 		"VirtualGoban"

#endif

