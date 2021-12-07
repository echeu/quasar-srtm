# LICENSE:
# Copyright (c) 2015, CERN
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
#
# 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
#
# 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
# THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS
# BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
# GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
# LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

# Author: Ben Farnham <ben.farnham@cern.ch>
project(LogIt CXX)
cmake_minimum_required(VERSION 2.8)

option(LOGIT_BACKEND_STDOUTLOG "The basic back-end: logs to stdout" ON )
option(LOGIT_BACKEND_BOOSTLOG "Rotating file logger back-end: fixed size on disk based on boost logging library" OFF )
option(LOGIT_BACKEND_UATRACE "UnifiedAutomation toolkit logger" OFF )
option(LOGIT_BACKEND_WINDOWS_DEBUGGER "Windows debugger logger" OFF )
# handle binary library build (builds with -DLOGIT_BUILD_SHARED_LIB=ON)
option(LOGIT_BUILD_SHARED_LIB "Build a LogIt shared library if ON (otherwise builds an object library)" OFF )

set(LOGIT_SOURCES
    src/LogIt.cpp
    src/LogLevels.cpp
    src/LogRecord.cpp
    src/LogSinks.cpp
    src/ComponentAttributes.cpp
    src/LogItInstance.cpp
)

if( LOGIT_BACKEND_STDOUTLOG )
    add_definitions(-DLOGIT_BACKEND_STDOUTLOG)
    set(LOGIT_SOURCES ${LOGIT_SOURCES} src/StdOutLog.cpp)
endif()

if( LOGIT_BACKEND_BOOSTLOG )
    add_definitions(-DLOGIT_BACKEND_BOOSTLOG)
    set(LOGIT_SOURCES ${LOGIT_SOURCES} src/BoostRotatingFileLog.cpp)
endif()

if( LOGIT_BACKEND_UATRACE )
    add_definitions(-DLOGIT_BACKEND_UATRACE)
    set(LOGIT_SOURCES ${LOGIT_SOURCES} src/UaTraceSink.cpp)
endif()

if( LOGIT_BACKEND_WINDOWS_DEBUGGER )
    add_definitions(-DLOGIT_BACKEND_WINDOWS_DEBUGGER)
    set(LOGIT_SOURCES ${LOGIT_SOURCES} src/WindowsDebuggerSink.cpp)
endif()

MESSAGE( STATUS "building LogIt from source files [${LOGIT_SOURCES}]" )
MESSAGE( STATUS "LogIt build options: LOGIT_BUILD_SHARED_LIB [${LOGIT_BUILD_SHARED_LIB}], backends: LOGIT_BACKEND_STDOUTLOG [${LOGIT_BACKEND_STDOUTLOG}] LOGIT_BACKEND_BOOSTLOG [${LOGIT_BACKEND_BOOSTLOG}] LOGIT_BACKEND_UATRACE [${LOGIT_BACKEND_UATRACE}] LOGIT_BACKEND_WINDOWS_DEBUGGER [${LOGIT_BACKEND_WINDOWS_DEBUGGER}]" )

if(NOT LOGIT_BUILD_STAND_ALONE)
  add_library (LogIt OBJECT ${LOGIT_SOURCES})
else()
  #
  # common settings for static/shared library compilation
  #
  set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)
  set (CMAKE_CXX_STANDARD 11)
  include_directories( include )

  #
  # resolve boost dependencies
  #
  if(DEFINED CMAKE_TOOLCHAIN_FILE)
    message("CMAKE_TOOLCHAIN_FILE is defined -- including [${CMAKE_TOOLCHAIN_FILE}]")
    include(${CMAKE_TOOLCHAIN_FILE})
  else()
    message("CMAKE_TOOLCHAIN_FILE not defined - using default [boost_standard_install.cmake]")
    include(boost_standard_install.cmake)
  endif()
  if( NOT DEFINED BOOST_LIBS )
    message(FATAL_ERROR "Required variable BOOST_LIBS not defined. boost headers/libs must be resolved for build environment")
  endif()
  message(STATUS "Using boost libraries: BOOST_LIBS [${BOOST_LIBS}]")

  #
  # create target library
  #
  if(LOGIT_BUILD_STATIC_LIB)
    message( STATUS "Building LogIt as a stand-alone static library" )
    add_library( LogIt STATIC ${LOGIT_SOURCES} )
  elseif(LOGIT_BUILD_SHARED_LIB)
    message( STATUS "Building LogIt as a stand-alone shared library" )
    add_library( LogIt SHARED ${LOGIT_SOURCES} )
  else()
    message( FATAL "LOGIT_BUILD_STAND_ALONE but neither [LOGIT_BUILD_STATIC_LIB] nor [LOGIT_BUILD_SHARED_LIB] defined. Please define STATIC/SHARED" )
  endif()
  target_link_libraries( LogIt ${BOOST_LIBS} )

  install( TARGETS LogIt DESTINATION lib )
  install( DIRECTORY include/ DESTINATION include )
endif()
