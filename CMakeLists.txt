cmake_minimum_required(VERSION 3.0)
cmake_policy(SET CMP0048 NEW)

project(ASIO VERSION 0.01)

set(CMAKE_PROJECT_VERSION 0.1a)
set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
IF(CMAKE_BUILD_TYPE MATCHES RELEASE)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
ENDIF(CMAKE_BUILD_TYPE MATCHES RELEASE)

set(LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib)
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/utils)
set(RES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/res)

set(COMMON_SOURCES
  ${SRC_DIR}/Message.cpp
  ${SRC_DIR}/utils/MessageLoader.cpp
)

set(CLIENT_SOURCES
  ${SRC_DIR}/ClientApp.cc
  ${SRC_DIR}/Client.cpp
)

set(SERVER_SOURCES
  ${SRC_DIR}/Server.cpp
  ${SRC_DIR}/ServerApp.cc
)

# adding executable, changing executable suffix for windows
set(CMAKE_EXECUTABLE_SUFFIX ".cc")
# set(${PROJECT_NAME} ${PROJECT_NAME}.cc)
add_executable(Server ${SERVER_SOURCES} ${COMMON_SOURCES})
add_executable(Client ${CLIENT_SOURCES} ${COMMON_SOURCES})
target_include_directories(Server PRIVATE "${SRC_DIR}" "${INCLUDE_DIR}")
target_include_directories(Client PRIVATE "${SRC_DIR}" "${INCLUDE_DIR}")

# ASIO
set(ASIO_DIR "${LIB_DIR}/asio/asio")
add_definitions(-DASIO_STANDALONE)
add_definitions(-DASIO_HAS_STD_ADDRESSOF)
add_definitions(-DASIO_HAS_STD_ARRAY)
add_definitions(-DASIO_HAS_CSTDINT)
add_definitions(-DASIO_HAS_STD_SHARED_PTR)
add_definitions(-DASIO_HAS_STD_TYPE_TRAITS)
add_definitions(-DASIO_HAS_VARIADIC_TEMPLATES)
add_definitions(-DASIO_HAS_STD_FUNCTION)
add_definitions(-DASIO_HAS_STD_CHRONO)
add_definitions(-DBOOST_ALL_NO_LIB)
if(WIN32)
add_definitions(-D_WIN32_WINNT=0x0501)
add_definitions(-D_WINSOCK_DEPRECATED_NO_WARNINGS)
endif(WIN32)

target_include_directories(Server PRIVATE "${ASIO_DIR}/include")
target_include_directories(Client PRIVATE "${ASIO_DIR}/include")
if(WIN32)
target_link_libraries(Server wsock32 ws2_32)
target_link_libraries(Client wsock32 ws2_32)
endif(WIN32)








