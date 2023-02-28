cmake_minimum_required(VERSION 3.5.0)

################################################################################
##     This file is part of RTMaps                                            ##
##     Copyright (c) Intempora S.A. All rights reserved.                      ##
################################################################################

# A more advanced CMakeLists.txt file is available in: <RTMaps Install Dir>/templates.u/CMakeLists.txt
set(RTMAPS_BOARD nvidia_jetson_l4t_ubuntu1804)
project("my_package.u")

# RTMAPS_SDKDIR must contain the path to the RTMaps installation directory
if (NOT DEFINED RTMAPS_SDKDIR)
    if (DEFINED ENV{RTMAPS_SDKDIR})
        set(RTMAPS_SDKDIR "$ENV{RTMAPS_SDKDIR}" CACHE PATH "")
    else()
        message(FATAL_ERROR "RTMAPS_SDKDIR not defined")
    endif()
endif()


# rtmaps.cmake performs some important initializations
include("${RTMAPS_SDKDIR}/templates.u/rtmaps.cmake")

# This allows us to declare a simple CMake target named "my_package".
# This target builds an RTMaps package using the "my_package.pckinfo"
# as well and the source files that are contained in the "src" folder
add_rtmaps_package(rtmaps-rpi-gpio PCKINFO "rtmaps-rpi-gpio.pckinfo"
    "src"
    SIGN_PCK TRUE
 )
