INCLUDE(FindPkgConfig)
PKG_CHECK_MODULES(PC_GLOBALTRONICS globaltronics)

FIND_PATH(
    GLOBALTRONICS_INCLUDE_DIRS
    NAMES globaltronics/api.h
    HINTS $ENV{GLOBALTRONICS_DIR}/include
        ${PC_GLOBALTRONICS_INCLUDEDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/include
          /usr/local/include
          /usr/include
)

FIND_LIBRARY(
    GLOBALTRONICS_LIBRARIES
    NAMES gnuradio-globaltronics
    HINTS $ENV{GLOBALTRONICS_DIR}/lib
        ${PC_GLOBALTRONICS_LIBDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/lib
          ${CMAKE_INSTALL_PREFIX}/lib64
          /usr/local/lib
          /usr/local/lib64
          /usr/lib
          /usr/lib64
          )

include("${CMAKE_CURRENT_LIST_DIR}/globaltronicsTarget.cmake")

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(GLOBALTRONICS DEFAULT_MSG GLOBALTRONICS_LIBRARIES GLOBALTRONICS_INCLUDE_DIRS)
MARK_AS_ADVANCED(GLOBALTRONICS_LIBRARIES GLOBALTRONICS_INCLUDE_DIRS)
