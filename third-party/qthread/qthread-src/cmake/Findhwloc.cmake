find_path(hwloc_INCLUDE_DIR hwloc.h PATH_SUFFIXES include)
find_library(hwloc_LIBRARY
  NAMES hwloc libhwloc
  PATH_SUFFIXES lib lib64 lib32)
if("${hwloc_INCLUDE_DIR}" STREQUAL "hwloc_INCLUDE_DIR-NOTFOUND" OR "${hwloc_LIBRARY}" STREQUAL "hwloc_LIBRARY-NOTFOUND")
  set(hwloc_FOUND FALSE)
else()
  set(hwloc_FOUND TRUE)
endif()

if(CMAKE_VERSION VERSION_LESS "3.17.0")
  message("-- Found hwloc: ${hwloc_FOUND}")
else()
  message(STATUS "Found hwloc: ${hwloc_FOUND}")
endif()
