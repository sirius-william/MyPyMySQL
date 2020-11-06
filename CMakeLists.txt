cmake_minimum_required(VERSION 3.19)

set(CMAKE_BUILD_TYPE Release)   # 设置编译版本，Release或Debug

SET(CMAKE_C_COMPILER_WORKS 1)
SET(CMAKE_CXX_COMPILER_WORKS 1)
# =============== 以下配置为必须指定的配置 ================

set(CMAKE_MAKE_PROGRAM "D:\\Program Files\\CMake\\bin\\cmake.exe") # 配置cmake地址
set(CMAKE_C_COMPILER "F:/Program Files/mingw-w64/x86_64-8.1.0-posix-seh-rt_v6-rev0/bin/gcc.exe")    # 配置gcc地址
set(CMAKE_CXX_COMPILER "F:/Program Files/mingw-w64/x86_64-8.1.0-posix-seh-rt_v6-rev0/bin/g++.exe")  # 配置g++地址

project(MyPyMySQL)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -fexec-charset=GBK -O3 -pthread")
# MySQL
set(MYSQL_BASE_DIR F:\\mysql-8.0.22 CACHE PATH "mysql server base direction")   # MySQL server 安装目录
# boost
set(Boost_ROOT F:\\boost_1_74_0)    # boost目录
set(BOOST_LIBS F:/boost_1_74_0/stage/lib) # boost动态链接库目录


if(CMAKE_BUILD_TYPE MATCHES Release)
    set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin/release)
ELSE()
    set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin/debug)
ENDIF()
## ===c++编译器配置
set(CMAKE_CXX_STANDARD 20)  # 语言标准：c++11

set(Boost_PYTHON37_LIBRARY_RELEASE F:/boost_1_74_0/stage/lib/libboost_python37-vc142-mt-sgd-x64-1_74.lib)
find_package(Python3)
find_package(PythonLibs)
#include(${BOOST_LIBS}/cmake/BoostDetectToolset-1.74.0.cmake)

find_package(Boost COMPONENTS python37 REQUIRED)


find_path(MYSQL_CHECK mysql.h ${MYSQL_BASE_DIR}/include DOC "checking mysql.h")

find_path(MYSQL_LIB_CHECK libmysql.dll ${MYSQL_BASE_DIR}/lib DOC "checking libmysql.dll")

if(CMAKE_BUILD_TYPE MATCHES Release)
    set(COPY_TARGET_DIR ${PROJECT_SOURCE_DIR}/bin/release)
else()
    set(COPY_TARGET_DIR ${PROJECT_SOURCE_DIR}/bin/debug)
endif()
file(COPY ${MYSQL_BASE_DIR}/lib/libmysql.dll DESTINATION ${COPY_TARGET_DIR})

find_path(CHECK_BOOST_INCLUDE python.hpp ${BOOST_ROOT}/boost/ DOC "checking boost include dir...")

include_directories(${MYSQL_BASE_DIR}/include)
include_directories(${PYTHON_INCLUDE_DIRS})
include_directories(${Boost_INCLUDE_DIR})

include_directories(config)
include_directories(include)

link_directories(${MYSQL_BASE_DIR}/lib)
link_directories(${BOOST_LIBS})

configure_file(
        "${PROJECT_SOURCE_DIR}/config/config.hpp.in"
        "${PROJECT_SOURCE_DIR}/config/config.hpp"
)

message(STATUS "PYTHON_LIBRARY: ${PYTHON_LIBRARY}")
message(STATUS "Boost_LIBRARIES: ${Boost_LIBRARIES}")

add_library(MyPyMySQL SHARED src/MyPyMySQL.cpp)
target_link_libraries(MyPyMySQL libmysql.lib ${PYTHON_LIBRARIES} ${Boost_LIBRARIES})
#get_cmake_property(_variableNames VARIABLES)
#foreach (_variableName ${_variableNames})
#    message(STATUS "${_variableName}=${${_variableName}}")
#endforeach()