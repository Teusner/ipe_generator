# - Try to find Ipe
# Once done this will define
# 
#  IPE_FOUND - system has Ipe
#  IPE_INCLUDE_DIR - the Ipe include directory
#  IPE_LIBRARIES - Link these to use Ipe
# https://raw.githubusercontent.com/CGAL/cgal/master/Installation/cmake/modules/FindIPE.cmake

set(IPE_FOUND FLASE)

find_package(PkgConfig)
# Is it already configured?
message(STATUS "Try to find ipe")

find_library(IPE_LIBRARY NAMES ipe
            HINTS /usr/lib
            NO_DEFAULT_PATH
      )

message(STATUS "IPE_LIBRARIES = ${IPE_LIBRARY}")

if(IPE_LIBRARY)
 set(IPE_FOUND TRUE)
endif()

include(FindPackageHandleStandardArgs)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(ipe
 REQUIRED_VARS IPE_LIBRARY
 FOUND_VAR IPE_FOUND)

if(IPE_FOUND)
 message(STATUS "Found Ipe: ${IPE_LIBRARY}")
endif()
