cmake_minimum_required(VERSION 3.12)

add_library(HDV_SRC 
  particle.cpp
  HDVCmdParser.cpp
  HDVParticle.cpp
  HDVSimulation.cpp
  HDVApplication.cpp
)
target_link_libraries(HDV_SRC PUBLIC eigen)

