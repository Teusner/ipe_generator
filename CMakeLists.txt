# ==================================================================
#  invariant-lib - cmake configuration file
# ==================================================================

cmake_minimum_required(VERSION 3.5.1)


# Settings
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O3")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
set(PACKAGE_NAME ipegenerator)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")

set(CMAKE_BUILD_TYPE "DEBUG")
set(PROJECT_VERSION 1.0.0)
set(Upstream_VERSION 1.0.0)
set(PROJECT_DESCRIPTION "Lib to generate ipe file using ibex-lib")

project(${PACKAGE_NAME})

if(NOT IBEX_ROOT)
	set(IBEX_ROOT "$ENV{HOME}/ibex")
endif()

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

FIND_PACKAGE(Ipe REQUIRED)
if(IPE_FOUND)
 set(LIBS ${LIBS} ${IPE_LIBRARY})

 message(STATUS "[IPE] IPE_LIBRARIES = ${IPE_LIBRARY}")
else()
 message(FATAL_ERROR "Unable to find IPE-lib.")
endif()

include_directories(include)
include_directories(include/ipe)

############## Generate Library
add_library(ipegenerator SHARED
 src/figure.cpp
 )
set_target_properties(${PACKAGE_NAME} PROPERTIES VERSION 1.0)
set_target_properties(${PACKAGE_NAME} PROPERTIES PUBLIC_HEADER src/figure.h)
include(GNUInstallDirs)
install(TARGETS ${PACKAGE_NAME}
 EXPORT ipegeneratorTargets
 LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
 PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PACKAGE_NAME}
 )
INSTALL(
    DIRECTORY ${CMAKE_SOURCE_DIR}/include/ipe/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PACKAGE_NAME}
    FILES_MATCHING PATTERN "*.h*"
    )

install(FILES style/basic.ipe DESTINATION ${CMAKE_INSTALL_SYSCONFDIR}/${PACKAGE_NAME} COMPONENT config)

configure_file(install/${PACKAGE_NAME}.pc.in ${PACKAGE_NAME}.pc @ONLY)
install(FILES ${CMAKE_BINARY_DIR}/${PACKAGE_NAME}.pc DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)

### Generate .cmake
#set(INSTALL_LIB_DIR lib CACHE PATH "Installation directory for libraries")
#set(INSTALL_INCLUDE_DIR include CACHE PATH "Installation directory for header files")
set(DEF_INSTALL_CMAKE_DIR "lib/CMake/${PACKAGE_NAME}")
set(INSTALL_CMAKE_DIR ${DEF_INSTALL_CMAKE_DIR} CACHE PATH "Installation directory for CMake files")

foreach(p LIB BIN INCLUDE CMAKE)
  set(var INSTALL_${p}_DIR)
  if(NOT IS_ABSOLUTE "${${var}}")
    set(${var} "${CMAKE_INSTALL_PREFIX}/${${var}}")
  endif()
endforeach()

export(TARGETS ipegenerator FILE "${PROJECT_BINARY_DIR}/${PACKAGE_NAME}Targets.cmake")
export(PACKAGE ${PACKAGE_NAME})

file(RELATIVE_PATH REL_INCLUDE_DIR "${INSTALL_CMAKE_DIR}" "${INSTALL_INCLUDE_DIR}")

set(CONF_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}" "${PROJECT_BINARY_DIR}")
configure_file(cmake/ipegeneratorConfig.cmake.in "${PROJECT_BINARY_DIR}/${PACKAGE_NAME}Config.cmake" @ONLY)

set(CONF_INCLUDE_DIRS "${CMAKE_INSTALL_LIBDIR}")
configure_file(cmake/ipegeneratorConfig.cmake.in "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/${PACKAGE_NAME}Config.cmake" @ONLY)

# ... for both
configure_file(cmake/ipegeneratorConfigVersion.cmake.in "${PROJECT_BINARY_DIR}/${PACKAGE_NAME}ConfigVersion.cmake" @ONLY)

# Install the FooBarConfig.cmake and FooBarConfigVersion.cmake
install(FILES
  "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/${PACKAGE_NAME}Config.cmake"
  "${PROJECT_BINARY_DIR}/${PACKAGE_NAME}ConfigVersion.cmake"
  DESTINATION "${INSTALL_CMAKE_DIR}" COMPONENT dev)

## Install the export set for use with the install-tree
install(EXPORT ipegeneratorTargets DESTINATION
  "${INSTALL_CMAKE_DIR}" COMPONENT dev)

#######################


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
