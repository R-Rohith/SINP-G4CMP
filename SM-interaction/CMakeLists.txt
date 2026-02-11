cmake_minimum_required(VERSION 3.5)
project(SM-interaction)

#----------------------------------------------------------------------------
# Find G4CMP package -- NOTE: Assumes user has run g4cmp_env.*sh script!
#
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_PREFIX_PATH $ENV{G4CMPINSTALL}/../..)
find_package(G4CMP)


#----------------------------------------------------------------------------
#You need to tell CMake where to find the ROOT installation. This can be done in a number of ways:
#   - ROOT built with classic configure/make use the provided $ROOTSYS/etc/cmake/FindROOT.cmake
#   - ROOT built with CMake. Add in CMAKE_PREFIX_PATH the installation prefix for ROOT
list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})

#---Locate the ROOT package and defines a number of variables (e.g. ROOT_INCLUDE_DIRS)
find_package(ROOT REQUIRED COMPONENTS RIO)

#---Define useful ROOT functions and macros (e.g. ROOT_GENERATE_DICTIONARY)
include(${ROOT_USE_FILE})
include_directories(${ROOT_INCLUDE_DIRS})
#----------------------------------------------------------------------------
# Find Geant4 package
# NOTE: WITH_GEANT4_UIVIS and USE_GEANT4_STATIC_LIBS are defined here
#
include($ENV{G4CMPINSTALL}/FindGeant4.cmake)

#----------------------------------------------------------------------------
# Setup include directories and compile definitions
# NOTE: Need to include G4CMP directories before G4.
#
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${G4CMP_INCLUDE_DIRS})
include(${G4CMP_USE_FILE})
include(${Geant4_USE_FILE})

#----------------------------------------------------------------------------
# RPATH stuff
#
# Automatically add paths of linked libraries to RPATH *unless* they are in the 
# build directory
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
# Set the RPATH of the installed objects *unless* they are installing to a 
# system directory, in which case they shouldn't need an explicitly set RPATH.
list(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES 
     "${CMAKE_INSTALL_PREFIX}/lib" is_SYSTEM_DIR)
if("${is_SYSTEM_DIR}" STREQUAL "-1")
    set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
endif()

#----------------------------------------------------------------------------
# Build stuff
#
set(project_SOURCES 
	${CMAKE_CURRENT_SOURCE_DIR}/src/SMActionInitialization.cc 
	${CMAKE_CURRENT_SOURCE_DIR}/src/SMConfigManager.cc 
	${CMAKE_CURRENT_SOURCE_DIR}/src/SMConfigMessenger.cc 
	${CMAKE_CURRENT_SOURCE_DIR}/src/SMDetectorConstruction.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/SMPrimaryGeneratorAction.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/SMSensitivity.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/SMRunAction.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/SMPhysicsList.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/SMEventAction.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/SMSteppingAction.cc
    )
    
if(USE_GEANT4_STATIC_LIBS)
	add_library(SMLib STATIC ${project_SOURCES})
else()
	add_library(SMLib SHARED ${project_SOURCES})
endif()
set_target_properties(SMLib PROPERTIES OUTPUT_NAME SM-interaction)

target_link_libraries(SMLib ${G4CMP_LIBRARIES} ${Geant4_LIBRARIES})

add_executable(SM-interaction SM-interaction.cc)
target_link_libraries(SM-interaction SMLib ${ROOT_LIBRARIES})

install(TARGETS SMLib DESTINATION lib)
install(TARGETS SM-interaction DESTINATION bin)
