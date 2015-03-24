#=============================================================================
# CMake configuration file for GPAPU_BENCHMARK
#=============================================================================

cmake_minimum_required(VERSION 2.8)
project(gpapu_benchmark)

# Location of additional CMAKE modules
SET(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

# ------------------------------------------------------------------------------
# OpenCL
# ------------------------------------------------------------------------------
find_package(OpenCL REQUIRED)
include_directories( ${OPENCL_INCLUDE_DIRS} )
set(BACKEND_LIBS ${OPENCL_LIBRARIES})
add_definitions(-DVEXCL_BACKEND_OPENCL)

# ------------------------------------------------------------------------------
# OpenMP
# ------------------------------------------------------------------------------
find_package("OpenMP")

# Add the OpenMP-specific compiler and linker flags
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")

# ------------------------------------------------------------------------------
# BLAZE -- required for some parallel solvers
# ------------------------------------------------------------------------------
IF(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
  SET(BLAZE_DIR ""  CACHE PATH   "Where is Blaze located?")
ELSEIF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  FIND_PATH(BLAZE_DIR NAMES blaze/Blaze.h PATHS "/usr/include" "/usr/local/include")
ELSEIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  FIND_PATH(BLAZE_DIR NAMES blaze/Blaze.h PATHS "/usr/include" "/usr/local/include")
ENDIF()

# ------------------------------------------------------------------------------
# Eigen -- required for some parallel solvers
# ------------------------------------------------------------------------------

FIND_PACKAGE(Eigen3)
INCLUDE_DIRECTORIES( ${EIGEN3_INCLUDE_DIR} )

# ------------------------------------------------------------------------------
# BOOST -- required for BLAZE
# ------------------------------------------------------------------------------
IF(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
   SET(BOOST_DIR ""  CACHE PATH   "Where is Boost located?")
ENDIF()

#SET(Boost_DEBUG  ON)
SET(BOOST_LIBRARYDIR    /usr/local/lib/)

FIND_PACKAGE(Boost 1.53.0 REQUIRED)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR} )
LINK_DIRECTORIES(${Boost_LIBRARY_DIR})

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

ADD_EXECUTABLE(benchmark  main.cpp blaze.cpp boost.cpp vexcl.cpp viennacl.cpp eigen.cpp)
TARGET_LINK_LIBRARIES(benchmark ${OPENCL_LIBRARIES} boost_system)
