# - Try to find Ipe
# Once done this will define
#
#  IPE_FOUND - system has Ipe
#  IPE_INCLUDE_DIR - the Ipe include directory
#  IPE_LIBRARIES - Link these to use Ipe
# https://raw.githubusercontent.com/CGAL/cgal/master/Installation/cmake/modules/FindIPE.cmake

set(IPE_FOUND FLASE)

# Is it already configured?
if (IPE_LIBRARIES)
  set(IPE_FOUND TRUE)
else()
  message(STATUS "Try to find ipe")

  find_library(IPE_LIBRARIES 
               NAMES ipe
               PATHS /usr/lib
               NO_DEFAULT_PATH
              )
  message(STATUS "IPE_LIBRARIES = ${IPE_LIBRARIES}")

  if(IPE_LIBRARIES)
    set(IPE_FOUND TRUE)
  endif()
endif()

include(FindPackageHandleStandardArgs)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(IPE
  REQUIRED_VARS IPE_LIBRARIES
  FOUND_VAR IPE_FOUND)

if(IPE_FOUND)
    message(STATUS "Found Ipe: ${IPE_LIBRARIES}")
endif()
