cmake_minimum_required(VERSION 3.0)
cmake_policy(SET CMP0048 NEW)

project(Sort_Visualizer VERSION 0.01)

set(CMAKE_PROJECT_VERSION 0.1a)
set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
IF(CMAKE_BUILD_TYPE MATCHES RELEASE)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
ENDIF(CMAKE_BUILD_TYPE MATCHES RELEASE)

set(LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib)
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(RES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/res)

set(SOURCES
  ${SRC_DIR}/main.cc
)


# adding executable, changing executable suffix for windows
set(${PROJECT_NAME} ${PROJECT_NAME}.cc)
add_executable(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE "${SRC_DIR}")


# SFML
set(SFML_DIR "${LIB_DIR}/SFML")
set(BUILD_SHARED_LIBS FALSE)
set(SFML_USE_STATIC_STD_LIBS TRUE)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
add_subdirectory(${SFML_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE "${SFML_DIR}/include")
target_link_libraries(${PROJECT_NAME} sfml-system sfml-graphics sfml-audio sfml-window)

set(CMAKE_EXECUTABLE_SUFFIX ".cc")