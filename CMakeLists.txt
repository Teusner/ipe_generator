# ==================================================================
#  ipegenerator - cmake configuration file
# ==================================================================

cmake_minimum_required(VERSION 3.5.1)


# Settings
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O3")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
set(PACKAGE_NAME ipegenerator)
set(CMAKE_CXX_FLAGS " -fPIC ${CMAKE_CXX_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")


set(CMAKE_BUILD_TYPE "DEBUG")
set(PROJECT_VERSION 1.1.0)
set(Upstream_VERSION 1.1.0)
set(PROJECT_DESCRIPTION "Lib to generate ipe file using ibex-lib and codac")

project(${PACKAGE_NAME})



################################################################################
# Options for directories
################################################################################

# Install directories
message(STATUS "[INSTALL] project will be installed in ${CMAKE_INSTALL_PREFIX}/ipegenerator")


set(CMAKE_INSTALL_INCLUDEDIR "ipegenerator/include" CACHE PATH "C++ header files
(include)")
set(CMAKE_INSTALL_LIBDIR "ipegenerator/lib" CACHE PATH "object code libraries (lib)")
set(CMAKE_INSTALL_BINDIR "ipegenerator/bin" CACHE PATH "user executables (bin)")
set(CMAKE_INSTALL_STYLE_DIR "ipegenerator/style" CACHE PATH "sheet style directory (style)")
set(CMAKE_INSTALL_PKGCONFIG "ipegenerator/share/pkgconfig" CACHE PATH "pkg files (share/pkgconfig)")
set(CMAKE_INSTALL_CMAKE "ipegenerator/share/ipegenerator/cmake" CACHE PATH "cmake files (share/ipegenerator/cmake)")


################################################################################
# Compilation configuration
################################################################################

#  # Check that the compiler supports c++11
#  include(CheckCXXCompilerFlag)
#  check_cxx_compiler_flag("-std=c++17" COMPILER_SUPPORTS_CXX17)
#
#  if(COMPILER_SUPPORTS_CXX17)
#    add_compile_options(-std=c++17)
#  else()
#    message(FATAL_ERROR "Codac needs a compiler with C++17 support")
#  endif()

#  if(WIN32)
#    # We need this for strdup under Windows (see issue #287 of ibex-lib repo)
#    add_definitions(-U__STRICT_ANSI__)
#  endif()

if(MSVC)
    add_compile_options(/D _CRT_SECURE_NO_WARNINGS)
else()
    add_compile_options(-Wall)
    add_compile_options(-O3)
endif()


################################################################################
# Looking for IBEX
################################################################################

find_package(IBEX REQUIRED)
ibex_init_common() # IBEX should have installed this function
message(STATUS "Found IBEX version ${IBEX_VERSION}")

################################################################################
# Looking for CODAC
################################################################################

find_package(CODAC REQUIRED)
message(STATUS "Found Codac version ${CODAC_VERSION}")
include_directories(${CODAC_INCLUDE_DIRS})

################################################################################
# Looking for Eigen3
################################################################################

find_package(Eigen3 REQUIRED NO_MODULE)
message(STATUS "Found Eigen3 version ${EIGEN3_VERSION}")
add_definitions(${EIGEN3_DEFINITIONS})
include_directories(${EIGEN3_INCLUDE_DIRS})

### Find package ###
find_package(PkgConfig)


# find_package(Boost REQUIRED COMPONENTS system thread filesystem)
# if(Boost_FOUND)
#     include_directories(${Boost_INCLUDE_DIR})
#     message(STATUS "[BOOST] OK")
# else()
#     message(FATAL_ERROR "[BOOST] NO")
# endif()

# find_package(OpenMP)
# if(OPENMP_FOUND)
#     set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
#     set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
#     message(STATUS "[OPENMP] OK")
# else()
#     message(FATAL_ERROR "[OPENMP] NO")
# endif()





################################################################################
# Compile sources
################################################################################

add_subdirectory(src) # C++ sources
# add_executable(test test/main.cpp)
# target_link_libraries(test ipe ipegenerator -lz -lpng -lspiro -ljpeg)

################################################################################
# Copy style sheet
################################################################################
install(FILES style/ipegenerator_style.isy DESTINATION ${CMAKE_INSTALL_STYLE_DIR})

