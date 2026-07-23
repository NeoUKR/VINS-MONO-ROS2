cmake_minimum_required(VERSION 3.16)
project(vins_dependency_check LANGUAGES C CXX)

find_package(OpenCV 4 REQUIRED)
find_package(Ceres REQUIRED)
find_package(Eigen3 REQUIRED)

message(STATUS "VINS dependency check:")
message(STATUS "  OpenCV: ${OpenCV_VERSION}")
message(STATUS "  Ceres:  ${Ceres_VERSION}")
message(STATUS "  Eigen3: ${Eigen3_VERSION}")
