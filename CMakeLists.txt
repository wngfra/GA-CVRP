cmake_minimum_required(VERSION 2.8)
project( GA-CVRP )

include_directories( include )
file(GLOB SOURCE "src/*.cc")
set( CMAKE_CXX_FLAGS  "-std=c++11 -O3 -fopenmp" )
add_executable( CVRP ${SOURCE} )
