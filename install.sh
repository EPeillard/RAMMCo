## Script de préparation de du soft

#Pré-requis
sudo apt-get install freeglut3 libcv-dev libopencv-dev cmake g++

#Installation d'Aruco
mkdir DL
cd DL
wget http://downloads.sourceforge.net/project/aruco/1.3.0/aruco-1.3.0.tgz
tar zxvf aruco-1.3.0.tgz
cd aruco-1.3.0
mkdir build
cd build
cmake ..
make
sudo make install
cd ../../..
sudo rm -R DL/
