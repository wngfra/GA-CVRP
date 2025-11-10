cmake_minimum_required(VERSION 2.8)
project( GA-CVRP )

include_directories( include )
file(GLOB SOURCE "src/*.cc")
set( CMAKE_CXX_FLAGS  "-std=c++17 -O3 -fopenmp" )

# Find Boost libraries for visualization
find_package(Boost REQUIRED COMPONENTS iostreams system)
include_directories(${Boost_INCLUDE_DIRS})

add_executable( CVRP ${SOURCE} )
target_link_libraries( CVRP ${Boost_LIBRARIES} )

# Note: Visualization requires gnuplot and Boost libraries to be installed
# Install with:
#   Linux: sudo apt-get install gnuplot libboost-iostreams-dev libboost-system-dev
#   macOS: brew install gnuplot boost
