foreach(t api_version backends bind
  bitmap bitmap_compare_inclusion bitmap_first_last_weight bitmap_singlify bitmap_string
  distances get_area_memlocation get_cache_covering_cpuset get_closest_objs get_largest_objs_inside_cpuset
  get_last_cpu_location get_next_obj_covering_cpuset get_obj_below_array_by_type get_obj_covering_cpuset
  get_obj_inside_cpuset get_obj_with_same_locality get_shared_cache_covering_obj
  groups insert_misc iodevs is_thissystem list_components obj_infos object_userdata pci_backend synthetic
  topology_abi topology_allow topology_diff topology_dup topology_restrict type_depth type_sscanf
)
  add_executable(hwloc_${t} hwloc_${t}.c)
  target_link_libraries(hwloc_${t} PRIVATE hwloc)

  add_test(NAME ${t} COMMAND hwloc_${t} WORKING_DIRECTORY ${PROJECT_BINARY_DIR})
  set_tests_properties(${t} PROPERTIES TIMEOUT 10)
endforeach()

foreach(t cpuset_nodeset memattrs cpukinds gl windows_processor_groups)
  add_executable(${t} ${t}.c)
  target_link_libraries(${t} PRIVATE hwloc)

  add_test(NAME ${t} COMMAND ${t} WORKING_DIRECTORY ${PROJECT_BINARY_DIR})
  set_tests_properties(${t} PROPERTIES TIMEOUT 10)
endforeach()

add_executable(xmlbuffer xmlbuffer.c)
target_link_libraries(xmlbuffer PRIVATE hwloc)
add_test(NAME xmlbuffer00 COMMAND xmlbuffer 0 0 WORKING_DIRECTORY ${PROJECT_BINARY_DIR})
add_test(NAME xmlbuffer01 COMMAND xmlbuffer 0 1 WORKING_DIRECTORY ${PROJECT_BINARY_DIR})
add_test(NAME xmlbuffer10 COMMAND xmlbuffer 1 0 WORKING_DIRECTORY ${PROJECT_BINARY_DIR})
add_test(NAME xmlbuffer11 COMMAND xmlbuffer 1 1 WORKING_DIRECTORY ${PROJECT_BINARY_DIR})
set_tests_properties(xmlbuffer00 xmlbuffer01 xmlbuffer10 xmlbuffer11 PROPERTIES TIMEOUT 10)

target_compile_definitions(hwloc_type_sscanf PRIVATE XMLTESTDIR=\"${CMAKE_CURRENT_SOURCE_DIR}/xml/\")

set_tests_properties(get_obj_with_same_locality PROPERTIES ENVIRONMENT HWLOC_TOP_SRCDIR=${TOPDIR})

add_test(NAME lstopo-no-graphics:verbose COMMAND lstopo-no-graphics -v)
add_test(NAME lstopo-no-graphics:groups COMMAND lstopo-no-graphics --windows-processor-groups)
set_tests_properties(lstopo-no-graphics:verbose lstopo-no-graphics:groups PROPERTIES
DISABLED ${HWLOC_SKIP_LSTOPO}
LABELS exe
TIMEOUT 10
)

add_test(NAME hwloc-info:support COMMAND hwloc-info --support)
set_tests_properties(hwloc-info:support PROPERTIES
DISABLED ${HWLOC_SKIP_TOOLS}
LABELS exe
TIMEOUT 10
)


# --- optional tests

if(HWLOC_HAVE_OPENCL)
  add_executable(test_opencl opencl.c)
  target_link_libraries(test_opencl PRIVATE hwloc OpenCL::OpenCL)

  add_test(NAME OpenCL COMMAND test_opencl)
  set_tests_properties(OpenCL PROPERTIES TIMEOUT 10)
endif()

if(HAVE_CUDA)
  add_executable(test_cuda cuda.c)
  target_link_libraries(test_cuda PRIVATE hwloc CUDA::cudart CUDA::cuda_driver)

  add_test(NAME CUDA COMMAND test_cuda)
  set_tests_properties(CUDA PROPERTIES TIMEOUT 10)
endif()
