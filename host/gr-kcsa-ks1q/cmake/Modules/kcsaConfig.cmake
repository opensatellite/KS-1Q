INCLUDE(FindPkgConfig)
PKG_CHECK_MODULES(PC_KCSA kcsa)

FIND_PATH(
    KCSA_INCLUDE_DIRS
    NAMES kcsa/api.h
    HINTS $ENV{KCSA_DIR}/include
        ${PC_KCSA_INCLUDEDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/include
          /usr/local/include
          /usr/include
)

FIND_LIBRARY(
    KCSA_LIBRARIES
    NAMES gnuradio-kcsa
    HINTS $ENV{KCSA_DIR}/lib
        ${PC_KCSA_LIBDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/lib
          ${CMAKE_INSTALL_PREFIX}/lib64
          /usr/local/lib
          /usr/local/lib64
          /usr/lib
          /usr/lib64
)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(KCSA DEFAULT_MSG KCSA_LIBRARIES KCSA_INCLUDE_DIRS)
MARK_AS_ADVANCED(KCSA_LIBRARIES KCSA_INCLUDE_DIRS)

