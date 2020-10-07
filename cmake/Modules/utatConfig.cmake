INCLUDE(FindPkgConfig)
PKG_CHECK_MODULES(PC_UTAT utat)

FIND_PATH(
    UTAT_INCLUDE_DIRS
    NAMES utat/api.h
    HINTS $ENV{UTAT_DIR}/include
        ${PC_UTAT_INCLUDEDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/include
          /usr/local/include
          /usr/include
)

FIND_LIBRARY(
    UTAT_LIBRARIES
    NAMES gnuradio-utat
    HINTS $ENV{UTAT_DIR}/lib
        ${PC_UTAT_LIBDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/lib
          ${CMAKE_INSTALL_PREFIX}/lib64
          /usr/local/lib
          /usr/local/lib64
          /usr/lib
          /usr/lib64
          )

include("${CMAKE_CURRENT_LIST_DIR}/utatTarget.cmake")

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(UTAT DEFAULT_MSG UTAT_LIBRARIES UTAT_INCLUDE_DIRS)
MARK_AS_ADVANCED(UTAT_LIBRARIES UTAT_INCLUDE_DIRS)
