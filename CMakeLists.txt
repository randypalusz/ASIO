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
set(RES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/res)

set(SOURCES
  ${SRC_DIR}/EchoClient.cc
  ${SRC_DIR}/EchoServer.cc
)

# adding executable, changing executable suffix for windows
set(CMAKE_EXECUTABLE_SUFFIX ".cc")
# set(${PROJECT_NAME} ${PROJECT_NAME}.cc)
add_executable(EchoServer ${SRC_DIR}/EchoServer.cc)
add_executable(EchoClient ${SRC_DIR}/EchoClient.cc)
target_include_directories(EchoServer PRIVATE "${SRC_DIR}")
target_include_directories(EchoClient PRIVATE "${SRC_DIR}")

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
add_definitions(-D_WIN32_WINNT=0x0501)
add_definitions(-D_WINSOCK_DEPRECATED_NO_WARNINGS)

target_include_directories(EchoServer PRIVATE "${ASIO_DIR}/include")
target_include_directories(EchoClient PRIVATE "${ASIO_DIR}/include")
target_link_libraries(EchoServer wsock32 ws2_32)
target_link_libraries(EchoClient wsock32 ws2_32)








