# ==================================================================
#  invariant-lib - cmake configuration file
# ==================================================================

cmake_minimum_required(VERSION 3.2.2)

# Settings
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O1")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O1")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")
set(PACKAGE_NAME      ipe_test)
set(PACKAGE_VERSION "0.1")
set(CMAKE_BUILD_TYPE "DEBUG")
project(${PACKAGE_NAME})

set(IBEX_ROOT "$ENV{HOME}/ibex")

set(CMAKE_CXX_FLAGS " -fPIC ${CMAKE_CXX_FLAGS}")

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules/)

### Find package ###
FIND_PACKAGE(PkgConfig)

find_package(Boost REQUIRED COMPONENTS system thread filesystem)
if(Boost_FOUND)
	include_directories(${Boost_INCLUDE_DIR})
	message(STATUS "[BOOST] OK")
else()
	message(FATAL_ERROR "[BOOST] NO")
endif()

find_package(OpenMP)
if(OPENMP_FOUND)
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
	message(STATUS "[OPENMP] OK")
else()
	message(FATAL_ERROR "[OPENMP] NO")
endif()

set(IbexLib_USE_STATIC 1)
FIND_PACKAGE(IbexLib REQUIRED)
if(IBEX_FOUND)
 set(LIBS ${LIBS} ${IBEX_LIBRARIES})
 include_directories(${IBEX_INCLUDE_DIRS})

 message(STATUS "[IBEX] IBEX_LIBRARIES = ${IBEX_LIBRARIES}")
 message(STATUS "[IBEX] IBEX_INCLUDE_DIRS = ${IBEX_INCLUDE_DIRS}")
 message(STATUS "[IBEX] IBEX_DEFINITIONS = ${IBEX_DEFINITIONS}")
 message(STATUS "[IBEX] CMAKE_CXX_FLAGS = ${CMAKE_CXX_FLAGS}")
else()
 message(FATAL_ERROR "Unable to find IBEX-lib. You may need to set IBEX_ROOT ${IBEX_ROOT}")
endif()

FIND_PACKAGE(IPE REQUIRED)
if(IPE_FOUND)
 set(LIBS ${LIBS} ${IPE_LIBRARIES})

 message(STATUS "[IPE] IPE_LIBRARIES = ${IPE_LIBRARIES}")
else()
 message(FATAL_ERROR "Unable to find IPE-lib.")
endif()

include_directories(include)
include_directories(include/ipe)

set(SRC
  src/main.cpp
  src/figure.cpp
)

set(HEADERS
  src/figure.h
)

add_executable(ipe_test ${SRC})

target_link_libraries(ipe_test
 ${LIBS}
)
