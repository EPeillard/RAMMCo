/** \file Core.cpp
  *
  * \brief Implementation of the functions of the class Core
  *
  **/

///To avoid running the function to get corner coordinate (camera coordinate) and just use values written in the code
//#define COMP_MOD_NO_INIT

///To avoid running the function to get corner coordinate (projector coordinate) and just use values written in the code
//#define COMP_MOD_NO_DETECT

///To display all the information
#define COMP_MOD_VERBOSE

#include "define.h"
#include "Core.h"

#ifdef COMP_MOD_VERBOSE
Mat verbose;
#endif // COMP_MOD_VERBOSE


using namespace rammco;

Core::Core(Camera* camera, Projector* proj, Simulation* simu)
{
    ///Setting the rammco objects
    this->camera = camera;
    this->proj = proj;
    this->simu=simu;
    
#ifdef COMP_MOD_NO_INIT
    ///If the init() method is disabled, 
    //TODO
#endif // COMP_MOD_NO_INIT

#ifdef COMP_MOD_NO_DETECT
    ///If the detection() method is disabled
    //TODO
#endif // COMP_MOD_NO_DETECT

    namedWindow(WINDOW_PROJECTOR, CV_WINDOW_FREERATIO );

#ifdef COMP_MOD_VERBOSE
    namedWindow(WINDOW_VERBOSE, CV_WINDOW_AUTOSIZE);
#endif // COMP_MOD_VERBOSE
}

Core::~Core()
{
}

void Core::genConvMat()
{

}

void Core::init()
{
#ifndef COMP_MOD_NO_INIT
  ///Create a white window in order to facilitate the calibration
  proj->draw(PROJ_MOD_NOTHING);
  src = camera->getFrame();

  int nbCornersDetected[NB_BOARD] = {0};
  
  MarkerDetector myDetector;
  vector <Marker> markers;
  vector<Point2f> cornersCamera;
  vector<Point2f> cornersBoard;

  while(cornersBoard.size()<4){
    
    ///Detecting the markers on the board
    myDetector.detect(src,markers);
    nbCornersDetected[0]=0;

    for (unsigned int i=0;i<markers.size();i++){
      if(markers[i].id==N_MARKER_B1_UR || 
	markers[i].id==N_MARKER_B1_UL || 
	markers[i].id==N_MARKER_B1_LR || 
	markers[i].id==N_MARKER_B1_LL)
	  nbCornersDetected[0]++;
    }
      
    ///If a board is detected, fill the vector with their corners position
    if(nbCornersDetected[0]==4)
    {
      for (unsigned int i=0;i<markers.size();i++){
	if(markers[i].id==N_MARKER_B1_UR)
	{
	  cornersCamera.push_back(markers[i].getCenter());
	  cornersBoard.push_back(MARKER_B1_UR);
	}      
	if(markers[i].id==N_MARKER_B1_UL)
	{
	  cornersCamera.push_back(markers[i].getCenter());
	  cornersBoard.push_back(MARKER_B1_UL);
	}
	if(markers[i].id==N_MARKER_B1_LL)
	{
	  cornersCamera.push_back(markers[i].getCenter());
	  cornersBoard.push_back(MARKER_B1_LL);
	}
	if(markers[i].id==N_MARKER_B1_LR)
	{
	  cornersCamera.push_back(markers[i].getCenter());
	  cornersBoard.push_back(MARKER_B1_LR);
	}
      }
    }
    
    ///If everything goes right, create the transformation matrix
    if(cornersBoard.size()==4)
    {
      findHomography(cornersBoard, cornersCamera).convertTo(R2C, CV_32F);
      findHomography(cornersCamera, cornersBoard).convertTo(C2R, CV_32F);
    }
    else {
      cornersBoard.clear();
      cornersCamera.clear();
    }
  }
#else
  //TODO
#endif
}

void Core::detection()
{
#ifndef COMP_MOD_NO_DETECT

  vector<Point2f> pointCamera;
  vector<Point2f> pointProjector;
  vector<Point2f> pointReal;

  while(pointCamera.size()<8)
  {
    pointCamera.clear();
    
    vector<int> markerIds;
    vector<Point> markerPoints;

    proj->draw(PROJ_MOD_BOARD, markerPoints, markerIds);

    waitKey(2000);
    src = camera->getFrame();
    
    
    MarkerDetector myDetector;
    vector <Marker> markers;

    myDetector.detect(src,markers);

    for (unsigned int i=0;i<markers.size();i++){
	for(unsigned int j=0;j<markerIds.size();j++)
	{
	    if(markers[i].id==markerIds[j])
	    {
		pointCamera.push_back(markers[i].getCenter());
		pointProjector.push_back(markerPoints[j]);
	    }
	}
    }
  }
  
  ///If everything goes right, create the transformation matrix
  perspectiveTransform(pointCamera,pointReal,C2R);
  findHomography(pointReal, pointProjector).convertTo(R2P, CV_32F);
  
#else
  //TODO
#endif
}

Mat* Core::getR2PMat()
{
  return &R2P;
}

void Core::initSimu()
{
    Point2f Pnull(-1,-1);
    for(int i=0;i<10;i++)
      ref[i] = Pnull;
	  
    src = camera->getFrame();
        
    MarkerDetector myDetector;
    vector <Marker> markers;
    
    vector<Point2f> realPoint;

    myDetector.detect(src,markers);

    for (unsigned int i=0;i<markers.size();i++){
	for(unsigned int j=0;j<10;j++){
	  if(markers[i].id == MOBILE_ARCUO[j]) {
	    vector<Point2f> point;
	    point.push_back(markers[i].getCenter());
	    perspectiveTransform(point,realPoint,C2R);
	    ref[j]=realPoint[0];
	    
	    cout << "M"<<j<<" detected"<<endl; 
	  }
	}
    }
}

void Core::loopSimu()
{
  //Detecte les déplacements des points trackés
  src = camera->getFrame();
      
  MarkerDetector myDetector;
  vector <Marker> markers;
  
  vector<Point2f> realPoint;
  Point2f depl[10];
  Point2f Pnull(-1,-1);
  for(int i=0;i<10;i++)
    depl[i] = Pnull;

  myDetector.detect(src,markers);
  for (unsigned int i=0;i<markers.size();i++){
    for(unsigned int j=0;j<10;j++){
      if(markers[i].id == MOBILE_ARCUO[j]){
	vector<Point2f> point;
	point.push_back(markers[i].getCenter());
	perspectiveTransform(point,realPoint,C2R);
	
	depl[j] = realPoint[0]-ref[j];
      }
    }
  }
  
  adjustDepl(depl);  
  
  system("castem15 Cast3M/demoD.dgibi");
  system("convert -density 200 Cast3M/demoD.ps -rotate 90 Cast3M/demoD.jpg"); //Imagemagick script
  
  proj->draw("Cast3M/demoD.jpg");

}

void Core::adjustDepl(Point2f depl[])
{
  ifstream fichier("demo.dgibi", ios::in);  // on ouvre le fichier en lecture
  ostringstream stream;
 
  if(!(fichier))
  {   
    cerr<<"Impossible d'ouvrir le fichier !" << endl;
    return ;
  }
  
  string line;
  
  while(getline(fichier, line)){
    for (int i=0;i<10;i++){
      std::stringstream sstm;
      sstm << "M" << i;
      
      string s1 = sstm.str()+"x";
      string s2 = to_string(depl[i].x);
      replace_word(line,s1,s2);
      
      s1 = sstm.str()+"y";
      s2 = to_string(depl[i].y);
      replace_word(line,s1,s2);
    }
    stream << line << "\n";
  }
  
  ofstream out("demoD.dgibi",ios::out);  
  if(!(out))
  {   
    cerr<<"Impossible d'ouvrir le fichier !" << endl;
    return ;
  }
  out << stream;
  
  out.close();
  fichier.close();
	  
}

unsigned Core::replace_word(std::string &original, const std::string &aTrouver, 
                                             const std::string &aRemplacer)
{
	unsigned n = original.find(aTrouver);

	if (n==std::string::npos)
	{
		return 0;
	}
	else
	{
		original.replace(n,aTrouver.size(),aRemplacer);
		return 1+replace_word(original, aTrouver, aRemplacer);
	}
}
