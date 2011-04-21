dnl -*- autoconf -*-
dnl
dnl Copyright (c) 2008 Sandia Corporation

m4_include([config/ax_c_restrict.m4])
m4_include([config/ax_create_stdint_h.m4])
m4_include([config/ax_check_page_aligned_malloc.m4])
m4_include([config/ax_check_16aligned_malloc.m4])
m4_include([config/ac_compile_check_sizeof.m4])
dnl Only include this if we're using an old Autoconf.  Remove when we
dnl finally drop support for AC 2.59
m4_if(m4_version_compare(m4_defn([m4_PACKAGE_VERSION]), [2.60]), -1,
      [m4_include([config/ac_prog_cc_c99.m4])
       m4_include([config/ac_use_system_extensions.m4])])

m4_include([config/qthread_check_makecontext.m4])
m4_include([config/qthread_check_ucstack_ssflags.m4])
m4_include([config/qthread_check_sst.m4])
m4_include([config/qthread_check_attributes.m4])
m4_include([config/qthread_check_altix_timer.m4])
m4_include([config/qthread_check_assembly.m4])
m4_include([config/qthread_check_atomics.m4])
m4_include([config/qthread_check_working_valloc.m4])
m4_include([config/qthread_builtin_expect.m4])
m4_include([config/qthread_hash_maps.m4])
m4_include([config/qthread_check_qsort.m4])
m4_include([config/qthread_detect_compiler_type.m4])
m4_include([config/qthread_check_swapcontext.m4])
m4_include([config/qthread_vamacros.m4])
m4_include([config/qthread_check_bitfield_order.m4])
m4_include([config/qthread_check_FUNCTION.m4])
dnl Topology interfaces
m4_include([config/qthread_check_libnuma.m4])
m4_include([config/qthread_check_plpa.m4])
m4_include([config/qthread_check_liblgrp.m4])
m4_include([config/qthread_check_hwloc.m4])
m4_include([config/qthread_check_machtopo.m4])
m4_include([config/qthread_check_tiletopo.m4])
