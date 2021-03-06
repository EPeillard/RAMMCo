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
cv::Mat verbose;
#endif // COMP_MOD_VERBOSE

using namespace cv;
using namespace std;
using namespace aruco;

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
 //TODO use?
}

void Core::init()
{
#ifndef COMP_MOD_NO_INIT
  ///Create a white window in order to facilitate the calibration
  proj->draw(PROJ_MOD_NOTHING);

  int nbCornersDetected[NB_BOARD] = {0};
  
  MarkerDetector myDetector;
  vector <Marker> markers;
  vector<Point2f> cornersCamera;
  vector<Point2f> cornersBoard;

  while(cornersBoard.size()<4){
    src = camera->getFrame();
    
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
    pointProjector.clear();
    
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
  
#ifdef OCTAVE
  vector<Point2f> pointImage;
  pointImage.push_back(Point2f(156,102));
  pointImage.push_back(Point2f(1086,102));
  pointImage.push_back(Point2f(156,766));
  pointImage.push_back(Point2f(1086,766));
  
  pointReal.clear();
  pointReal.push_back(MARKER_B1_UL);
  pointReal.push_back(MARKER_B1_UR);
  pointReal.push_back(MARKER_B1_LL);
  pointReal.push_back(MARKER_B1_LR);
  
  pointProjector.clear();
  perspectiveTransform(pointReal,pointProjector,R2P);
  findHomography(pointImage,pointProjector).convertTo(I2P,CV_32F);  
#endif
  
#else
  //TODO
#endif
}

Mat* Core::getR2PMat()
{
  return &R2P;
}

Mat* Core::getI2PMat()
{
  return &I2P;
}

void Core::initSimu()
{
    int toto=0;
    Point2f Pnull(-1,-1);
    for(int i=0;i<10;i++)
      ref[i] = Pnull;
        
    MarkerDetector myDetector;
    vector <Marker> markers;
    
    vector<Point2f> realPoint;

    while(toto<1){ //TODO something better
      toto=0;
      src = camera->getFrame();
      myDetector.detect(src,markers);
	
      for (unsigned int i=0;i<markers.size();i++){
	  for(unsigned int j=0;j<10;j++){
	    if(markers[i].id == MOBILE_ARCUO[j]) {
	      vector<Point2f> point;
	      point.push_back(markers[i].getCenter());
	      perspectiveTransform(point,realPoint,C2R);
	      ref[j]=realPoint[0];
	      
	      cout << "M"<<j<<" detected"<<endl; 
	      toto++;
	    }
	  }
      }
    }
#ifdef OCTAVE
  const char * argvv [] = {"" /* name of program, not relevant */, "--silent"};

  octave_main (2, (char **) argvv, true /* embedded */);

  octave_value_list functionArguments;

  const octave_value_list result = feval ("initiate", functionArguments);
#endif
}

void Core::loopSimu()
{
  //Detecte les déplacements des points trackés
      
  MarkerDetector myDetector;
  vector <Marker> markers;
  
  vector<Point2f> realPoint;
  Point2f depl[10];
  Point2f Pnull(-1,-1);
  for(int i=0;i<10;i++)
    depl[i] = Pnull;

  int toto=0;
  
  while(toto<1){
    toto=0;
    src = camera->getFrame();
    myDetector.detect(src,markers);
    for (unsigned int i=0;i<markers.size();i++){
      for(unsigned int j=0;j<10;j++){
	if(markers[i].id == MOBILE_ARCUO[j]){
	  vector<Point2f> point;
	  point.push_back(markers[i].getCenter());
	  perspectiveTransform(point,realPoint,C2R);
	  
	  depl[j] = realPoint[0]-ref[j];
	  
	  cout << "M"<<j<<" detected"<<endl; 
	  toto++;
	}
      }
    }
  }
  
#ifdef CASTEM
  adjustDepl(depl);  
  
  //system("./castem Cast3M/demoD.dgibi");
  system("castem15 Cast3M/demoD.dgibi | grep M0");
  system("convert -density 50 Cast3M/demoD.ps -rotate 90 Cast3M/demoD.jpg"); //Imagemagick script
  
  proj->draw("Cast3M/demoD.jpg");
#endif
#ifdef OCTAVE
  //Calculate U
  const char * argvv [] = {"" /* name of program, not relevant */, "--silent"};

  octave_main (2, (char **) argvv, true /* embedded */);

  octave_value_list functionArguments;
  
  functionArguments(0) = depl[0].y;

  const octave_value_list result = feval ("calculate", functionArguments, 1);
  
  //Create new display
  proj->draw("im.jpg");
  //proj->draw(result(0).img_value());
  
#endif
  

}

void Core::adjustDepl(Point2f depl[])
{
  ifstream fichier("Cast3M/demo.dgibi", ios::in);  // on ouvre le fichier en lecture
  if(!(fichier))
  {   
    cerr<<"Impossible d'ouvrir le fichier a lire !" << endl;
    return ;
  }
  
  ofstream out("Cast3M/demoD.dgibi",ios::out);  
  if(!(out))
  {   
    cerr<<"Impossible d'ouvrir le fichier a modifier !" << endl;
    return ;
  }
  
  string line;
  
  while(getline(fichier, line)){
    for (int i=0;i<10;i++){
      std::stringstream sstm;
      sstm << "M" << i;
      
      string s1 = sstm.str()+"x";
      string s2 = to_string(-depl[i].y);
      replace_word(line,s1,s2);
      
      s1 = sstm.str()+"y";
      s2 = to_string(-depl[i].x);
      replace_word(line,s1,s2);
  
      //cout << "Depl "<<i<<" x : "<<depl[i].x<<" ; y :"<<depl[i].y<<endl; 
    }
    out << line << "\n";
  }
    
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

vector<Point2f*> Core::reorderPoints(vector<Point2f*>& list_point)
{
    /// Creation of the absolute corner of the camera
    vector<Point2f*> list_corner_absolute_camera;
    Mat* mat = new Mat(camera->getFrame());
    list_corner_absolute_camera.push_back(new Point2f(0, 0));
    list_corner_absolute_camera.push_back(new Point2f(mat->cols, 0));
    list_corner_absolute_camera.push_back(new Point2f(mat->cols, mat->rows));
    list_corner_absolute_camera.push_back(new Point2f(0, mat->rows));

    vector<Point2f*> temp = list_point;
    vector<Point2f*> ret;

    /// While points still int temp (not ordered)
    while(temp.size()>0)
    {
        ///take each point of the camera absolute corners
        for(int j=0; j<list_corner_absolute_camera.size(); j++)
        {
            double d = 99999999999;
            int i, index=0;
            ///for each point in temp
            for(i=0; i<temp.size(); i++)
            {
                /// Caculate the distance between the point and a corner
                Point2f p(temp[i] - list_corner_absolute_camera[j]);
                double norme = sqrt(pow((temp[i]->x - list_corner_absolute_camera[j]->x), 2)+pow((temp[i]->y - list_corner_absolute_camera[j]->y), 2));
                /// If the distance is the minimum, the index of the point is saved
                if(d>norme){d=norme;index=i;}
            }
            /// The nearest point of the corner is added to the list
            ret.push_back(temp[index]);
            temp.erase(temp.begin()+index);
        }
    }
    ///Empty the list_point
    while(list_point.size()>0){list_point.pop_back();}
    return ret;
}
