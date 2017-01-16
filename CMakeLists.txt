# Ajustez en fonction de votre version de CMake
cmake_minimum_required (VERSION 2.8)

# Nom du projet
project (RAMMCo)

set(useRaspicam FALSE)

SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} /usr/local/lib/cmake/)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package (OpenCV REQUIRED)
find_package (aruco REQUIRED)
find_package(Octave REQUIRED)
find_package(MPI REQUIRED)
find_package(Gnuplot REQUIRED)

if(useRaspicam)
  find_package (raspicam REQUIRED)
endif(useRaspicam)


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

include_directories(${OCTAVE_INCLUDE_DIR})
include_directories(${MPI_INCLUDE_PATH})

add_executable (RAMMCo ${source_files} ${header_files})

if(useRaspicam)
  target_compile_definitions(RAMMCo PRIVATE PICAMERA)
endif(useRaspicam)

target_link_libraries (RAMMCo ${OpenCV_LIBS} ${aruco_LIBS} ${OCTAVE_LIBRARIES} octave octinterp ${MPI_LIBRARIES} ${raspicam_CV_LIBS})
