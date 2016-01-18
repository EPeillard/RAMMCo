## Script de préparation de du soft

mkdir log

#Pré-requis
echo "Installation des packages nécéssaires..."
sudo apt-get install freeglut3 libcv-dev libopencv-dev cmake g++ imagemagick >> ./log/log_packages

#Installation d'Aruco
echo "Installation d'Aruco..."
mkdir DL
cd DL
wget http://downloads.sourceforge.net/project/aruco/1.3.0/aruco-1.3.0.tgz >> ./../log/log_aruco
tar zxvf aruco-1.3.0.tgz >> ./../log/log_aruco 
cd aruco-1.3.0
mkdir build >> ./../../log/log_aruco
cd build
cmake .. >> ./../../../log/log_aruco
make >> ./../../../log/log_aruco
sudo make install >> ./../../../log/log_aruco
cd ../../..
sudo rm -R DL/

echo "Pour terminer l'instalation, merci d'installer castem en vous rendant sur le site web : www-cast3m.cea.fr"
