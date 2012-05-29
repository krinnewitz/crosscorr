cmake_minimum_required (VERSION 2.6)
project (CROSSCORR) 

FIND_PACKAGE( OpenCV REQUIRED )

add_executable (crosscorr Main.cpp) 

TARGET_LINK_LIBRARIES( crosscorr ${OpenCV_LIBS} )

