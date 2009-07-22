
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

PROJECT(somadspio)

IF(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
   SET(CMAKE_BUILD_TYPE RelWithDebInfo)
ENDIF(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)


SET(LIBRARY_VERSION "1.0")
SET(LIBRARY_SOVERSION "0")


SET(Boost_ADDITIONAL_VERSIONS "1.38" "1.38.0" "1.39" "1.39.0")


# boost requirements
find_package(Boost 1.37.0 COMPONENTS unit_test_framework system REQUIRED)
find_package(PkgConfig)

pkg_check_modules(SIGC++ REQUIRED sigc++-2.0>=2.0)
pkg_check_modules(SOMADEPS REQUIRED 
  somanetwork-1.0>=1.0)


ENABLE_TESTING()

include_directories(.)
include_directories(ext-dspboard/src)
include_directories(logging-ext/boost)


link_directories(
  ${Boost_LIBRARY_DIRS}
  ${SOMADEPS_LIBRARY_DIRS}
)


add_subdirectory(somadspio)
add_subdirectory(tests)

set(PKG_FILE_NAME somadspio-1.0.pc)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/${PKG_FILE_NAME}.in 
  ${CMAKE_CURRENT_BINARY_DIR}/${PKG_FILE_NAME})

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${PKG_FILE_NAME} 
  DESTINATION lib/pkgconfig)

