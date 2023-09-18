cmake_minimum_required(VERSION 3.15)
project(hwloc
    LANGUAGES C
    VERSION 2.9.0)

enable_testing()

option(HWLOC_ENABLE_TESTING "Enable testing" ON)
option(HWLOC_SKIP_LSTOPO "don't build/install lstopo")
option(HWLOC_SKIP_TOOLS "don't build/install other hwloc tools")
option(HWLOC_SKIP_INCLUDES "don't install headers")
option(HWLOC_WITH_LIBXML2 "use libxml2 instead of minimal XML")
option(HWLOC_WITH_OPENCL "enable OpenCL support")
if(CMAKE_VERSION VERSION_GREATER_EQUAL 3.17)
    option(HWLOC_WITH_CUDA "enable CUDA support")
endif()
option(HWLOC_BUILD_SHARED_LIBS "build shared libraries" ${BUILD_SHARED_LIBS})

set(TOPDIR ${PROJECT_SOURCE_DIR}/../..)

# "libhwloc.*" naming for MSVC and non-MSVC

configure_file(${TOPDIR}/contrib/windows/hwloc_config.h include/hwloc/autogen/config.h COPYONLY)

# Configure dynamically based on platform capabilities
include(CheckIncludeFile)
include(CheckSymbolExists)
include(CheckCSourceCompiles)

check_include_file("dirent.h" HAVE_DIRENT_H)
check_include_file("unistd.h" HAVE_UNISTD_H)
check_include_file("malloc.h" HAVE_MALLOC_H)
check_include_file("memory.h" HAVE_MEMORY_H)

check_symbol_exists(mkstemp "stdlib.h" HAVE_MKSTEMP)
check_symbol_exists(memalign "malloc.h" HAVE_MEMALIGN)

check_symbol_exists(strncasecmp "strings.h" HAVE_STRNCASECMP)
if(MSVC AND HAVE_STRNCASECMP)
    set(hwloc_strncasecmp 1)
    set(hwloc_strncasecmp_fcn strncasecmp)
else()
    set(hwloc_strncasecmp 0)
    set(hwloc_strncasecmp_fcn strncmp)
endif()


set(SIZEOF_VOID_P ${CMAKE_SIZEOF_VOID_P})
# disable x86 entirely by default
set(HWLOC_X86_32_ARCH)
set(HWLOC_X86_64_ARCH)
set(HWLOC_HAVE_X86_CPUID 1)
if (CMAKE_SYSTEM_PROCESSOR MATCHES "(^AMD64$|^x86_64$)")
    # "AMD64" on Windows, "x86_64" on Linux
    set(HWLOC_X86_64_ARCH 1)
elseif (CMAKE_SYSTEM_PROCESSOR MATCHES "(^x86$|i.86)")
    # "x86" on Windows, "i.86" on Linux
    set(HWLOC_X86_32_ARCH 1)
else()
    set(HWLOC_HAVE_X86_CPUID 0)
endif()

check_c_source_compiles("int main(void) {int cpuinfo[4]; __cpuidex(cpuinfo,0,0); return 0;}"
HWLOC_HAVE_MSVC_CPUIDEX
)

# the following lines are disabled until we are sure they are safe with old build environmentx
# - snprintf() returned broken values in the past, hwloc detects it during configure (see 7a4ee26510c06b55fc04aaccbfa18d0ca3b87198)
#   set(HAVE_DECL_SNPRINTF 1)
# - strtoull() had some issues in the past (see 9559bd08b79ef63dce45df87fb7f875b73ecb512)
#   set(HAVE_DECL_STRTOULL 1)

# --- optional external libraries
set(HWLOC_HAVE_LIBXML2)
if(HWLOC_WITH_LIBXML2)
    find_package(LibXml2 REQUIRED)
    set(HWLOC_HAVE_LIBXML2 1)
endif()

set(HWLOC_HAVE_OPENCL)
if(HWLOC_WITH_OPENCL)
    find_package(OpenCL REQUIRED)
    set(HWLOC_HAVE_OPENCL 1)
endif()

set(HAVE_CUDA)
set(HAVE_CUDA_H)
set(HAVE_CUDA_RUNTIME_API_H)
set(HWLOC_HAVE_CUDART)
if(HWLOC_WITH_CUDA)
    find_package(CUDAToolkit REQUIRED)
    set(HAVE_CUDA 1)
    set(HAVE_CUDA_H 1)
    set(HAVE_CUDA_RUNTIME_API_H 1)
    set(HWLOC_HAVE_CUDART 1)
endif()

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/private_config.h.in include/private/autogen/config.h)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/static-components.h.in include/static-components.h)

# Library

add_compile_definitions($<$<CONFIG:DEBUG>:HWLOC_DEBUG=1>)

# FIXME dll soname
add_library(hwloc
    ${TOPDIR}/hwloc/topology.c
    ${TOPDIR}/hwloc/traversal.c
    ${TOPDIR}/hwloc/distances.c
    ${TOPDIR}/hwloc/memattrs.c
    ${TOPDIR}/hwloc/cpukinds.c
    ${TOPDIR}/hwloc/components.c
    ${TOPDIR}/hwloc/bind.c
    ${TOPDIR}/hwloc/bitmap.c
    ${TOPDIR}/hwloc/pci-common.c
    ${TOPDIR}/hwloc/diff.c
    ${TOPDIR}/hwloc/shmem.c
    ${TOPDIR}/hwloc/misc.c
    ${TOPDIR}/hwloc/base64.c
    ${TOPDIR}/hwloc/topology-noos.c
    ${TOPDIR}/hwloc/topology-synthetic.c
    ${TOPDIR}/hwloc/topology-xml.c
    ${TOPDIR}/hwloc/topology-xml-nolibxml.c
    ${TOPDIR}/hwloc/topology-windows.c
    ${TOPDIR}/hwloc/topology-x86.c
    $<$<BOOL:${HWLOC_HAVE_LIBXML2}>:${TOPDIR}/hwloc/topology-xml-libxml.c>
    $<$<BOOL:${HWLOC_HAVE_OPENCL}>:${TOPDIR}/hwloc/topology-opencl.c>
    $<$<BOOL:${HAVE_CUDA}>:${TOPDIR}/hwloc/topology-cuda.c>
)

target_link_libraries(hwloc PRIVATE
    $<$<BOOL:${HWLOC_HAVE_LIBXML2}>:LibXml2::LibXml2>
    $<$<BOOL:${HWLOC_HAVE_OPENCL}>:OpenCL::OpenCL>
    "$<$<BOOL:${HAVE_CUDA}>:CUDA::cudart;CUDA::cuda_driver>"
)

if(HWLOC_BUILD_SHARED_LIBS)
    target_compile_definitions(hwloc PRIVATE $<$<BOOL:${MSVC}>:_USRDLL>)
endif()

target_include_directories(hwloc PUBLIC
    "$<BUILD_INTERFACE:${TOPDIR}/include;${CMAKE_CURRENT_BINARY_DIR}/include>"
    $<INSTALL_INTERFACE:include>
)

# Tools under utils/hwloc

if(NOT HWLOC_SKIP_TOOLS)

    set(TOOLS
        hwloc-bind
        hwloc-calc
        hwloc-diff
        hwloc-distrib
        hwloc-gather-cpuid
        hwloc-info
        hwloc-patch
    )

    foreach(tool IN ITEMS ${TOOLS})
        add_executable(${tool}
            ${TOPDIR}/utils/hwloc/${tool}.c)
        target_link_libraries(${tool} PRIVATE hwloc)
    endforeach(tool)

endif()

# lstopo

if(NOT HWLOC_SKIP_LSTOPO)

    set(LSTOPOS
        lstopo-no-graphics
        lstopo
        lstopo-win
    )

    set(LSTOPO_COMMON_SOURCES
        ${TOPDIR}/utils/lstopo/lstopo.c
        ${TOPDIR}/utils/lstopo/lstopo-draw.c
        ${TOPDIR}/utils/lstopo/lstopo-tikz.c
        ${TOPDIR}/utils/lstopo/lstopo-fig.c
        ${TOPDIR}/utils/lstopo/lstopo-svg.c
        ${TOPDIR}/utils/lstopo/lstopo-ascii.c
        ${TOPDIR}/utils/lstopo/lstopo-text.c
        ${TOPDIR}/utils/lstopo/lstopo-xml.c
        ${TOPDIR}/utils/hwloc/common-ps.c
    )

    add_executable(lstopo-no-graphics
        ${LSTOPO_COMMON_SOURCES}
    )
    target_link_libraries(lstopo-no-graphics PRIVATE hwloc)

    add_executable(lstopo
        ${LSTOPO_COMMON_SOURCES}
        ${TOPDIR}/utils/lstopo/lstopo-windows.c
    )
    target_compile_definitions(lstopo PRIVATE LSTOPO_HAVE_GRAPHICS)

    add_executable(lstopo-win WIN32
        ${LSTOPO_COMMON_SOURCES}
        ${TOPDIR}/utils/lstopo/lstopo-windows.c
    )
    target_compile_definitions(lstopo-win PRIVATE LSTOPO_HAVE_GRAPHICS)
    target_link_options(lstopo-win PRIVATE "$<$<BOOL:${MSVC}>:/subsystem:windows;/entry:mainCRTStartup>")

    foreach(tool IN ITEMS ${LSTOPOS})
        target_include_directories(${tool} PRIVATE ${TOPDIR}/utils/hwloc)
        target_link_libraries(${tool} PRIVATE hwloc)
    endforeach(tool)

endif()

# Misc

foreach(target IN ITEMS hwloc ${TOOLS} ${LSTOPOS})
    target_compile_definitions(${target} PRIVATE $<$<BOOL:${MSVC}>:_CRT_SECURE_NO_WARNINGS>)
endforeach(target)

# Install

install(TARGETS hwloc)

if(NOT HWLOC_SKIP_TOOLS)
    install(TARGETS ${TOOLS})
endif()

if(NOT HWLOC_SKIP_LSTOPO)
    install(TARGETS ${LSTOPOS})
endif()

if(NOT HWLOC_SKIP_INCLUDES)
    install(FILES ${TOPDIR}/include/hwloc.h TYPE INCLUDE)
    install(DIRECTORY ${TOPDIR}/include/hwloc TYPE INCLUDE FILES_MATCHING PATTERN "*.h")
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/include/hwloc/autogen/config.h DESTINATION include/hwloc/autogen)
endif()

if(MSVC AND BUILD_SHARED_LIBS)
  install(FILES $<TARGET_PDB_FILE:hwloc>
    TYPE BIN
    OPTIONAL)
endif()

# Testing

if(HWLOC_ENABLE_TESTING)
    add_subdirectory(${TOPDIR}/tests/hwloc ${CMAKE_CURRENT_BINARY_DIR}/tests/hwloc)
endif()
