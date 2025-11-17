cmake_minimum_required(VERSION 3.6)
set(NAME BBC)
project(${NAME})


option(WITH_GEANT4_UIVIS "Build project with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
    find_package(Geant4 REQUIRED ui_all vis_all)
else()
    find_package(Geant4 REQUIRED)
endif()

find_package(HepMC3 REQUIRED)

include(${Geant4_USE_FILE}) 
include_directories(${PROJECT_SOURCE_DIR}/include)

file(GLOB MACRO_FILES "macros/*.mac")
file(COPY ${MACRO_FILES} DESTINATION ${CMAKE_BINARY_DIR}/)

file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh) 
 
add_executable(${NAME} BBC.cc ${sources} ${headers})
target_link_libraries(${NAME} ${Geant4_LIBRARIES} "HepMC3::HepMC3")
