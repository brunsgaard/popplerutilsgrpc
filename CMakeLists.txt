cmake_minimum_required(VERSION 3.10)
find_package(PkgConfig)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


# Set the project name
project (hello_cmake)
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
# check results and add flag
if(COMPILER_SUPPORTS_CXX11)#
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)#
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
    message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++14 support. Please use a different C++ compiler.")
endif()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

find_package(Protobuf REQUIRED)
find_package(GRPC REQUIRED)

pkg_search_module(PROTOBUF REQUIRED QUIET protobuf)
pkg_search_module(GRPC REQUIRED QUIET grpc)
pkg_search_module(POPPLERCPP REQUIRED poppler-cpp)

set(PROTOS
    ${CMAKE_CURRENT_SOURCE_DIR}/protos/helloworld.proto
    ${CMAKE_CURRENT_SOURCE_DIR}/protos/popplerutilsgrpc.proto
)

set(PROTO_SRC_DIR ${CMAKE_CURRENT_BINARY_DIR}/proto-src)
file(MAKE_DIRECTORY ${PROTO_SRC_DIR})
include_directories(${PROTO_SRC_DIR})

protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${PROTO_SRC_DIR} ${PROTOS})
grpc_generate_cpp(GRPC_SRCS GRPC_HDRS ${PROTO_SRC_DIR} ${PROTOS})


add_executable(hello_cmake
    src/main.cpp
    ${PROTO_SRCS}
)

# Include the boost headers
target_include_directories(hello_cmake
    PRIVATE ${POPPLERCPP_INCLUDE_DIRS}
)

# link against the boost libraries
target_link_libraries(hello_cmake
    PRIVATE
    protobuf::libprotobuf
    ${POPPLERCPP_LIBRARIES}
)

# GRPC STUFF

# 
# # Building server
# add_executable(greeter_server
#     ${CMAKE_CURRENT_SOURCE_DIR}/src/greeter_server.cc
#     ${PROTO_SRCS}
#     ${GRPC_SRCS}
# )
# 
# # Include the boost headers
# target_include_directories(greeter_server
#     PRIVATE ${POPPLERCPP_INCLUDE_DIRS}
# )
# 
# target_link_libraries(greeter_server
#     gRPC::grpc++_reflection
#     protobuf::libprotobuf
#     ${POPPLERCPP_LIBRARIES}
# )
# 
# # Building client
# add_executable(greeter_client
#     ${CMAKE_CURRENT_SOURCE_DIR}/src/greeter_client.cc
#     ${PROTO_SRCS}
#     ${GRPC_SRCS}
# )
# 
# # Include the boost headers
# target_include_directories(greeter_client
#     PRIVATE ${POPPLERCPP_INCLUDE_DIRS}
# )
# 
# target_link_libraries(greeter_client
#     gRPC::grpc++_reflection
#     protobuf::libprotobuf
#     ${POPPLERCPP_LIBRARIES}
# )
