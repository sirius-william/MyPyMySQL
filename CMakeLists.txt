cmake_minimum_required(VERSION 3.17)
project(myPyMySQL)
set(CMAKE_BUILD_TYPE Release)   # 设置编译版本，Release或Debug
# =============== 以下配置为必须指定的配置 ================
set(CMAKE_MAKE_PROGRAM "F:\\Program Files\\JetBrains\\CLion 2020.2.4\\bin\\cmake\\win\\bin\\cmake.exe") # 配置cmake地址
set(CMAKE_C_COMPILER "F:/Program Files/mingw-w64/x86_64-8.1.0-posix-seh-rt_v6-rev0/bin/gcc.exe")    # 配置gcc地址
set(CMAKE_CXX_COMPILER "F:/Program Files/mingw-w64/x86_64-8.1.0-posix-seh-rt_v6-rev0/bin/g++.exe")  # 配置g++地址
# Python
set(PYTHON_BASE_DIR F:\\python3.7 CACHE PATH "python base direction")   # Python安装目录

# MySQL
set(MYSQL_BASE_DIR F:\\mysql-8.0.22 CACHE PATH "mysql server base direction")   # MySQL server 安装目录
# boost
set(BOOST_INCLUDE F:\\boost_1_74_0 CACHE PATH "boost include direction")    # boost头文件目录
set(BOOST_LIBS F:\\boost CACHE PATH "boost libs direction") # boost动态链接库目录

option(BOOST_USED "if using boost.." ON)    # 是否使用boost提供的Python库开发方式来编写，on则采用boost库，off则采用纯Python.h编写

# =============== 尽量不要修改下面的配置 ==================
SET(CMAKE_C_COMPILER_WORKS 1)
SET(CMAKE_CXX_COMPILER_WORKS 1)
if(CMAKE_BUILD_TYPE MATCHES Release)

    set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin/release)
ELSE()
    set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin/debug)
ENDIF()
## ===c++编译器配置
set(CMAKE_CXX_STANDARD 20)  # 语言标准：c++20
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")
set(CMAKE_CXX_FLAGS "-fexec-charset=GBK")   # 字符集

if(BOOST_USED)
    message("Using Boost ----- True")
    find_path(CHECK_BOOST_INCLUDE python.hpp ${BOOST_INCLUDE}/boost/ DOC "checking boost include dir...")
    if(CHECK_BOOST_INCLUDE)
        message("checking BOOST dir successfully...")
        include_directories(${BOOST_INCLUDE})
        link_directories(${BOOST_LIBS})
    else()
        message(FATAL_ERROR "can't find BOOST python.hpp in include dir, please check out the input or set BOOST_USED OFF")
    endif()
endif()

find_path(PYTHON_CHECK python.exe ${PYTHON_BASE_DIR} DOC "checking Python Interpreter...")
if(PYTHON_CHECK)
    message("checking Python Interpreter successfully")
    find_path(PYTHON_H_CHECK Python.h ${PYTHON_BASE_DIR}/include DOC "checking")
    if(PYTHON_H_CHECK)
        message("checking Python.h successfully")
        include_directories(${PYTHON_BASE_DIR}/include)
    else()
        message(FATAL_ERROR "cannot find Python.h")
    endif()
    find_path(PYTHON_LIB_CHECK python3.lib ${PYTHON_BASE_DIR}/libs DOC "checking python3.lib")
    if(PYTHON_LIB_CHECK)
        message("checking python3.lib successfully.")
        link_directories(${PYTHON_BASE_DIR}/libs)
    else()
        message(FATAL_ERROR "can not find python3.lib, please check out the PYTHON_BASE_DIR or be sure Python3 had been installed.")
    endif()
else()
    message(FATAL_ERROR "please install Python3.")
endif()


include_directories(include)

find_path(MYSQL_CHECK mysql.h ${MYSQL_BASE_DIR}/include DOC "checking mysql.h")
find_path(MYSQL_LIB_CHECK libmysql.dll ${MYSQL_BASE_DIR}/lib DOC "checking libmysql.dll")
if(MYSQL_CHECK)
    message("checking mysql.h successfully")
    include_directories(${MYSQL_BASE_DIR}/include)
    if(MYSQL_LIB_CHECK)
        message("checking libmysql.dll successful")
        link_directories(${MYSQL_BASE_DIR}/lib)
        if(CMAKE_BUILD_TYPE MATCHES Release)
            message("copying libmysql.dll to output dir--Release")
            file(COPY ${MYSQL_BASE_DIR}/lib/libmysql.dll DESTINATION ${PROJECT_SOURCE_DIR}/bin/release)
        else()
            message("copying libmysql.dll to output dir--Debug")
            file(COPY ${MYSQL_BASE_DIR}/lib/libmysql.dll DESTINATION ${PROJECT_SOURCE_DIR}/bin/debug)
        endif()
    else()
        message(FATAL_ERROR "can not find libmysql.dll, please check MYSQL_BASE_DIR or be sure MySQL Server has been installed.")
    endif()
else()
    message(FATAL_ERROR "can not find mysql.h in MYSQL_BASE_DIR/include, please check MYSQL_BASE_DIR or be sure MySQL Server has been installed.")
endif()

include_directories(config)

configure_file(
        "${PROJECT_SOURCE_DIR}/config/config.hpp.in"
        "${PROJECT_SOURCE_DIR}/config/config.hpp"
)

aux_source_directory(src dir)

add_library(myPyMySQL SHARED ${dir})

target_link_libraries(myPyMySQL libmysql.lib python3.lib)
