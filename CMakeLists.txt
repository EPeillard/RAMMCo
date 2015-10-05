# Ajustez en fonction de votre version de CMake
cmake_minimum_required (VERSION 2.8)

# Nom du projet
project (RAMMCo)

SET(CMAKE_MODULE_PATH /usr/local/lib/cmake/)

find_package (OpenCV REQUIRED)
find_package (aruco REQUIRED)
find_package (raspicam REQUIRED)


set(EXECUTABLE_OUTPUT_PATH bin)
SET(CMAKE_CXX_FLAGS "-std=c++11")

file(
        GLOB_RECURSE
        source_files
        src/*.cpp
)

file(
        GLOB_RECURSE
        header_files
        src/*.h
)



add_executable (RAMMCo ${source_files} ${header_files})

target_link_libraries (RAMMCo ${OpenCV_LIBS} ${aruco_LIBS} ${raspicam_CV_LIBS})
