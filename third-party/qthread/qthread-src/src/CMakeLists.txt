set(QTHREADS_SCHEDULER nemesis CACHE STRING "Which scheduler to use for qthreads. Valid options are nemesis, sherwood, and distrib.")
set(QTHREADS_TOPOLOGY no CACHE STRING "Which topology detection/management system to use for qthreads. Valid options are no, hwloc, and binders.")
set(QTHREADS_BARRIER feb CACHE STRING "Which barrier implementation to use for qthreads. Valid options are feb, sinc, array, and log.")
set(QTHREADS_SINC donecount CACHE STRING "Which sinc implementation to use for qthreads. Valid options are donecount, donecoutn_cas, snzi, and original.")
set(QTHREADS_ALLOC base CACHE STRING "Wich allocation implementation to use for qthreads. Valid options are base, and chapel.")
set(QTHREADS_CACHELINE_SIZE_ESTIMATE 64 CACHE STRING "Estimate of the cacheline size of the target machine (used for optimizing data structure layouts).")
set(QTHREADS_DEFAULT_STACK_SIZE 32768 CACHE STRING "Default qthread stack size.")
set(QTHREADS_HASHMAP hashmap CACHE STRING "Which hashmap implementation to use. Valid values are \"hashmap\" and \"lf_hashmap\".")
set(QTHREADS_DICT_TYPE shavit CACHE STRING "Which dictionary implementation to use. Valid values are \"shavit\", \"trie\", and \"simple\".")
set(QTHREADS_TIMER_TYPE gettimeofday CACHE STRING "Which timer implementation to use. Valid values are \"clock_gettime\", \"mach\", \"gettimeofday\", and \"gethrtime\".")
# Only default to the fastcontext implementation in cases where it's confirmed to work.
# Note: apparently 32-bit x86 may show up as i386, i486, i586, or i686.
# Little-endian powerpc variants are excluded here as they're known not to work
# due to an unresolved bug in our fastcontext code.
if ("${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "x86_64" OR
    "${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "amd64" OR
    "${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "aarch64" OR
    "${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "arm64" OR
    "${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "arm" OR
    "${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "riscv64" OR
    "${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "i386" OR
    "${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "i486" OR
    "${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "i586" OR
    "${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "i686" OR
    "${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "ppc64" OR
    "${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "ppc")
  set(QTHREADS_CONTEXT_SWAP_IMPL fastcontext CACHE STRING "Which context swap implementation to use. Valid values are \"system\" and \"fastcontext\".")
else()
  message(WARNING "No fast context swap available on this system, falling back to the system-provided one.")
  set(QTHREADS_CONTEXT_SWAP_IMPL system CACHE STRING "Which context swap implementation to use. Valid values are \"system\" and \"fastcontext\".")
endif()
set(QTHREADS_HWLOC_GET_TOPOLOGY_FUNCTION "" CACHE STRING "function to get hwloc topology (otherwise uses hwloc_topology_init and hwloc_topology_load)")
set(QTHREADS_GUARD_PAGES OFF CACHE BOOL "Whether or not to guard memory pages to help with debugging stack overflows. Default is OFF.")
set(QTHREADS_CONDWAIT_QUEUE OFF CACHE BOOL "Use a waiting queue based on pthread condition variables instead of a spin-based queue for inter-thread communication. Default is OFF.")

set(QTHREADS_SOURCES
  cacheline.c
  envariables.c
  feb.c
  hazardptrs.c
  io.c
  locks.c
  qalloc.c
  qloop.c
  queue.c
  barrier/${QTHREADS_BARRIER}.c
  qutil.c
  syncvar.c
  qthread.c
  mpool.c
  shepherds.c
  workers.c
  threadqueues/${QTHREADS_SCHEDULER}_threadqueues.c
  sincs/${QTHREADS_SINC}.c
  alloc/${QTHREADS_ALLOC}.c
  affinity/common.c
  affinity/${QTHREADS_TOPOLOGY}.c
  threadpool.c
  touch.c
  tls.c
  teams.c
  ${QTHREADS_HASHMAP}.c
  ds/qarray.c
  ds/qdqueue.c
  ds/qlfqueue.c
  ds/qpool.c
  ds/qswsrqueue.c
  ds/dictionary/hash.c
  ds/dictionary/dictionary_${QTHREADS_DICT_TYPE}.c
  qtimer/${QTHREADS_TIMER_TYPE}.c
  syscalls/accept.c
  syscalls/connect.c
  syscalls/poll.c
  syscalls/pread.c
  syscalls/pwrite.c
  syscalls/read.c
  syscalls/select.c
  syscalls/system.c
  syscalls/user_defined.c
  syscalls/wait4.c
  syscalls/write.c
  patterns/allpairs.c
  patterns/wavefront.c
)

add_library(qthread ${QTHREADS_SOURCES})

if ("${QTHREADS_CONTEXT_SWAP_IMPL}" STREQUAL "fastcontext")
  target_sources(qthread PRIVATE fastcontext/asm.S fastcontext/context.c)
elseif ("${QTHREADS_CONTEXT_SWAP_IMPL}" STREQUAL "system")
  target_compile_definitions(qthread PRIVATE USE_SYSTEM_SWAPCONTEXT)
else()
  message(FATAL_ERROR "The specified context swap implementation does not match any known implementations.")
endif()

target_include_directories(qthread
  PRIVATE "../include"
)
set_target_properties(
  qthread
  PROPERTIES C_VISIBILITY_PRESET hidden
             VERSION ${PROJECT_VERSION}
             SOVERSION ${PROJECT_VERSION_MAJOR})
# Link pthreads.
target_link_libraries(qthread PUBLIC Threads::Threads)

# The c11 threading library has to be linked
# via a separate library on BSD OSs.
if ("${CMAKE_SYSTEM_NAME}" STREQUAL "FreeBSD" OR
    "${CMAKE_SYSTEM_NAME}" STREQUAL "kFreeBSD" OR
    "${CMAKE_SYSTEM_NAME}" STREQUAL "OpenBSD" OR
    "${CMAKE_SYSTEM_NAME}" STREQUAL "NetBSD" OR
    "${CMAKE_SYSTEM_NAME}" STREQUAL "DragonFly")
  target_link_libraries(qthread PUBLIC "stdthreads")
endif()

if ("${QTHREADS_TOPOLOGY}" STREQUAL "hwloc" OR "${QTHREADS_TOPOLOGY}" STREQUAL "binders")
  find_package(hwloc REQUIRED)
  target_include_directories(qthread PRIVATE "${hwloc_INCLUDE_DIR}")
  target_link_libraries(qthread PUBLIC "${hwloc_LIBRARY}")
  if ("${QTHREADS_TOPOLOGY}" STREQUAL "hwloc")
    target_compile_definitions(qthread PRIVATE USE_HWLOC_MEM_AFFINITY)
  endif()
endif()
target_include_directories(qthread
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
# TODO: move these into a configure header instead of piping them through the flags.
target_compile_definitions(qthread
  PRIVATE CACHELINE_WIDTH=${QTHREADS_CACHELINE_SIZE_ESTIMATE}
  PRIVATE QTHREAD_DEFAULT_STACK_SIZE=${QTHREADS_DEFAULT_STACK_SIZE}
)

if(NOT "${QTHREADS_HWLOC_GET_TOPOLOGY_FUNCTION}" STREQUAL "")
  target_compile_definitions(qthread
    PRIVATE HWLOC_GET_TOPOLOGY_FUNCTION=${QTHREADS_HWLOC_GET_TOPOLOGY_FUNCTION}
  )
endif()

if(QTHREADS_GUARD_PAGES)
  target_compile_definitions(qthread
    PRIVATE QTHREADS_GUARD_PAGES=1
  )
endif()

if(QTHREADS_CONDWAIT_QUEUE)
  target_compile_definitions(qthread
    PRIVATE QTHREAD_CONDWAIT_BLOCKING_QUEUE=1
  )
endif()

# CMAKE_INSTALL_LIBDIR is less reliable with CMake 3.21 and earlier.
# In those case, it may be necessary for the end-user to specify it manually.
install(
  TARGETS qthread
  EXPORT qthread
  LIBRARY
    DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    COMPONENT shlib
)
configure_package_config_file("${CMAKE_CURRENT_SOURCE_DIR}/../qthread-config.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/qthread-config.cmake"
  INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/qthread")
write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/qthread-config-version.cmake"
  VERSION 1.22
  COMPATIBILITY SameMajorVersion)
install(
  FILES
    "${CMAKE_CURRENT_BINARY_DIR}/qthread-config.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/qthread-config-version.cmake"
  DESTINATION
    "${CMAKE_INSTALL_LIBDIR}/cmake/qthread")
install(
  DIRECTORY "${CMAKE_SOURCE_DIR}/include/qthread"
  DESTINATION include
  PATTERN "top" EXCLUDE
)
install(
  FILES "${CMAKE_SOURCE_DIR}/include/qthread/top/qthread.h"
  DESTINATION include
)
install(EXPORT qthread DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/qthread")
