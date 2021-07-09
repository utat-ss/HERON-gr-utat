if(NOT PKG_CONFIG_FOUND)
    INCLUDE(FindPkgConfig)
endif()
PKG_CHECK_MODULES(PC_UTAT3_9 utat3_9)

FIND_PATH(
    UTAT3_9_INCLUDE_DIRS
    NAMES utat3_9/api.h
    HINTS $ENV{UTAT3_9_DIR}/include
        ${PC_UTAT3_9_INCLUDEDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/include
          /usr/local/include
          /usr/include
)

FIND_LIBRARY(
    UTAT3_9_LIBRARIES
    NAMES gnuradio-utat3_9
    HINTS $ENV{UTAT3_9_DIR}/lib
        ${PC_UTAT3_9_LIBDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/lib
          ${CMAKE_INSTALL_PREFIX}/lib64
          /usr/local/lib
          /usr/local/lib64
          /usr/lib
          /usr/lib64
          )

include("${CMAKE_CURRENT_LIST_DIR}/utat3_9Target.cmake")

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(UTAT3_9 DEFAULT_MSG UTAT3_9_LIBRARIES UTAT3_9_INCLUDE_DIRS)
MARK_AS_ADVANCED(UTAT3_9_LIBRARIES UTAT3_9_INCLUDE_DIRS)
