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
set(QTHREADS_CONTEXT_SWAP_IMPL fastcontext CACHE STRING "Which context swap implementation to use. Valid values are \"system\" and \"fastcontext\".")
set(HWLOC_INSTALL_DIR /usr/local CACHE PATH "Install path for hwloc library")
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
set_target_properties(qthread PROPERTIES C_VISIBILITY_PRESET hidden)
target_link_libraries(qthread PUBLIC Threads::Threads)
if ("${QTHREADS_TOPOLOGY}" STREQUAL "hwloc" OR "${QTHREADS_TOPOLOGY}" STREQUAL "binders")
  find_path(HWLOC_INCLUDE_DIR NAMES hwloc.h HINTS ${HWLOC_INSTALL_DIR} PATH_SUFFIXES include)
  target_include_directories(qthread PRIVATE ${HWLOC_INCLUDE_DIR})
  find_library(HWLOC_LIBRARY
    NAMES hwloc libhwloc
    HINTS ${HWLOC_INSTALL_DIR}
    PATH_SUFFIXES lib lib64 lib32)
  target_link_libraries(qthread PUBLIC "${HWLOC_LIBRARY}")
  if ("${QTHREADS_TOPOLOGY}" STREQUAL "hwloc")
    target_compile_definitions(qthread PRIVATE USE_HWLOC_MEM_AFFINITY)
  endif()
endif()
target_include_directories(qthread
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/qthread>
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
install(
  DIRECTORY ${CMAKE_SOURCE_DIR}/include/qthread
  DESTINATION include
  PATTERN "top" EXCLUDE
)
install(
  FILES "${CMAKE_SOURCE_DIR}/include/qthread/top/qthread.h"
  DESTINATION include
)
install(EXPORT qthread DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/qthread")
