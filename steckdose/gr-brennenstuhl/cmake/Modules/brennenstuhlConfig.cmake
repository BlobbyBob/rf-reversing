INCLUDE(FindPkgConfig)
PKG_CHECK_MODULES(PC_BRENNENSTUHL brennenstuhl)

FIND_PATH(
    BRENNENSTUHL_INCLUDE_DIRS
    NAMES brennenstuhl/api.h
    HINTS $ENV{BRENNENSTUHL_DIR}/include
        ${PC_BRENNENSTUHL_INCLUDEDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/include
          /usr/local/include
          /usr/include
)

FIND_LIBRARY(
    BRENNENSTUHL_LIBRARIES
    NAMES gnuradio-brennenstuhl
    HINTS $ENV{BRENNENSTUHL_DIR}/lib
        ${PC_BRENNENSTUHL_LIBDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/lib
          ${CMAKE_INSTALL_PREFIX}/lib64
          /usr/local/lib
          /usr/local/lib64
          /usr/lib
          /usr/lib64
          )

include("${CMAKE_CURRENT_LIST_DIR}/brennenstuhlTarget.cmake")

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(BRENNENSTUHL DEFAULT_MSG BRENNENSTUHL_LIBRARIES BRENNENSTUHL_INCLUDE_DIRS)
MARK_AS_ADVANCED(BRENNENSTUHL_LIBRARIES BRENNENSTUHL_INCLUDE_DIRS)
