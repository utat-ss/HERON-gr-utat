find_package(PkgConfig)

PKG_CHECK_MODULES(PC_GR_UTAT_HERON gnuradio-UTAT_HERON)

FIND_PATH(
    GR_UTAT_HERON_INCLUDE_DIRS
    NAMES gnuradio/UTAT_HERON/api.h
    HINTS $ENV{UTAT_HERON_DIR}/include
        ${PC_UTAT_HERON_INCLUDEDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/include
          /usr/local/include
          /usr/include
)

FIND_LIBRARY(
    GR_UTAT_HERON_LIBRARIES
    NAMES gnuradio-UTAT_HERON
    HINTS $ENV{UTAT_HERON_DIR}/lib
        ${PC_UTAT_HERON_LIBDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/lib
          ${CMAKE_INSTALL_PREFIX}/lib64
          /usr/local/lib
          /usr/local/lib64
          /usr/lib
          /usr/lib64
          )

include("${CMAKE_CURRENT_LIST_DIR}/gnuradio-UTAT_HERONTarget.cmake")

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(GR_UTAT_HERON DEFAULT_MSG GR_UTAT_HERON_LIBRARIES GR_UTAT_HERON_INCLUDE_DIRS)
MARK_AS_ADVANCED(GR_UTAT_HERON_LIBRARIES GR_UTAT_HERON_INCLUDE_DIRS)
