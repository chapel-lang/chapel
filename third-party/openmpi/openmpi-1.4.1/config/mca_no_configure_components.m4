dnl
dnl $HEADER
dnl

dnl This file is automatically created by autogen.sh; it should not
dnl be edited by hand!!

m4_define([mca_backtrace_no_config_component_list], [])
m4_define([mca_backtrace_m4_config_component_list], [execinfo, printstack, darwin, none])
m4_define([mca_carto_no_config_component_list], [auto_detect, file])
m4_define([mca_carto_m4_config_component_list], [])
m4_define([mca_crs_no_config_component_list], [none])
m4_define([mca_crs_m4_config_component_list], [blcr, self])
m4_define([mca_installdirs_no_config_component_list], [])
m4_define([mca_installdirs_m4_config_component_list], [env, config, windows])
m4_define([mca_maffinity_no_config_component_list], [first_use])
m4_define([mca_maffinity_m4_config_component_list], [libnuma])
m4_define([mca_memchecker_no_config_component_list], [])
m4_define([mca_memchecker_m4_config_component_list], [valgrind])
m4_define([mca_memcpy_no_config_component_list], [])
m4_define([mca_memcpy_m4_config_component_list], [])
m4_define([mca_memory_no_config_component_list], [])
m4_define([mca_memory_m4_config_component_list], [ptmalloc2, malloc_solaris])
m4_define([mca_paffinity_no_config_component_list], [])
m4_define([mca_paffinity_m4_config_component_list], [linux, solaris, windows, darwin, test, posix])
m4_define([mca_timer_no_config_component_list], [])
m4_define([mca_timer_m4_config_component_list], [catamount, aix, altix, darwin, solaris, windows, linux])
m4_define([mca_opal_framework_list], [backtrace, carto, crs, installdirs, maffinity, memchecker, memcpy, memory, paffinity, timer])
m4_define([mca_errmgr_no_config_component_list], [default])
m4_define([mca_errmgr_m4_config_component_list], [])
m4_define([mca_ess_no_config_component_list], [])
m4_define([mca_ess_m4_config_component_list], [portals_utcp, cnos, alps, env, hnp, lsf, singleton, slurm, tool])
m4_define([mca_filem_no_config_component_list], [rsh])
m4_define([mca_filem_m4_config_component_list], [])
m4_define([mca_grpcomm_no_config_component_list], [])
m4_define([mca_grpcomm_m4_config_component_list], [cnos, bad, basic])
m4_define([mca_iof_no_config_component_list], [hnp, orted, tool])
m4_define([mca_iof_m4_config_component_list], [])
m4_define([mca_notifier_no_config_component_list], [])
m4_define([mca_notifier_m4_config_component_list], [syslog])
m4_define([mca_odls_no_config_component_list], [])
m4_define([mca_odls_m4_config_component_list], [default, process])
m4_define([mca_oob_no_config_component_list], [])
m4_define([mca_oob_m4_config_component_list], [tcp])
m4_define([mca_plm_no_config_component_list], [])
m4_define([mca_plm_m4_config_component_list], [alps, ccp, lsf, process, rsh, slurm, tm, xgrid])
m4_define([mca_ras_no_config_component_list], [])
m4_define([mca_ras_m4_config_component_list], [alps, ccp, gridengine, loadleveler, lsf, slurm, tm])
m4_define([mca_rmaps_no_config_component_list], [load_balance, rank_file, round_robin, seq])
m4_define([mca_rmaps_m4_config_component_list], [])
m4_define([mca_rml_no_config_component_list], [oob])
m4_define([mca_rml_m4_config_component_list], [ftrm])
m4_define([mca_routed_no_config_component_list], [binomial, direct, linear])
m4_define([mca_routed_m4_config_component_list], [])
m4_define([mca_snapc_no_config_component_list], [])
m4_define([mca_snapc_m4_config_component_list], [full])
m4_define([mca_orte_framework_list], [errmgr, ess, filem, grpcomm, iof, notifier, odls, oob, plm, ras, rmaps, rml, routed, snapc])
m4_define([mca_allocator_no_config_component_list], [basic, bucket])
m4_define([mca_allocator_m4_config_component_list], [])
m4_define([mca_bml_no_config_component_list], [r2])
m4_define([mca_bml_m4_config_component_list], [])
m4_define([mca_btl_no_config_component_list], [self, sm])
m4_define([mca_btl_m4_config_component_list], [elan, gm, mx, ofud, openib, portals, tcp, udapl])
m4_define([mca_coll_no_config_component_list], [basic, hierarch, inter, self, sm, sync, tuned])
m4_define([mca_coll_m4_config_component_list], [])
m4_define([mca_common_no_config_component_list], [sm])
m4_define([mca_common_m4_config_component_list], [mx, portals])
m4_define([mca_crcp_no_config_component_list], [])
m4_define([mca_crcp_m4_config_component_list], [bkmrk])
m4_define([mca_dpm_no_config_component_list], [orte])
m4_define([mca_dpm_m4_config_component_list], [])
m4_define([mca_io_no_config_component_list], [])
m4_define([mca_io_m4_config_component_list], [romio])
m4_define([mca_mpool_no_config_component_list], [fake, rdma, sm])
m4_define([mca_mpool_m4_config_component_list], [])
m4_define([mca_mtl_no_config_component_list], [])
m4_define([mca_mtl_m4_config_component_list], [mx, portals, psm])
m4_define([mca_osc_no_config_component_list], [pt2pt, rdma])
m4_define([mca_osc_m4_config_component_list], [])
m4_define([mca_vprotocol_no_config_component_list], [pessimist])
m4_define([mca_vprotocol_m4_config_component_list], [])
m4_define([mca_pml_no_config_component_list], [cm, csum, ob1])
m4_define([mca_pml_m4_config_component_list], [crcpw, v])
m4_define([mca_pubsub_no_config_component_list], [orte])
m4_define([mca_pubsub_m4_config_component_list], [])
m4_define([mca_rcache_no_config_component_list], [vma])
m4_define([mca_rcache_m4_config_component_list], [])
m4_define([mca_topo_no_config_component_list], [unity])
m4_define([mca_topo_m4_config_component_list], [])
m4_define([mca_ompi_framework_list], [allocator, bml, btl, coll, common, crcp, dpm, io, mpool, mtl, osc, pml, pubsub, rcache, topo])
m4_define([mca_project_list], [opal, orte, ompi])

dnl List all the no-configure components that we found, and AC_DEFINE
dnl their versions

AC_DEFUN([MCA_NO_CONFIG_CONFIG_FILES],[

AC_CONFIG_FILES(opal/mca/backtrace/Makefile)
dnl ----------------------------------------------------------------

dnl m4-configure component: 
dnl    opal/mca/backtrace/darwin

AC_CONFIG_FILES([opal/mca/backtrace/darwin/Makefile])
dnl ----------------------------------------------------------------

dnl m4-configure component: 
dnl    opal/mca/backtrace/execinfo

AC_CONFIG_FILES([opal/mca/backtrace/execinfo/Makefile])
dnl ----------------------------------------------------------------

dnl m4-configure component: 
dnl    opal/mca/backtrace/none

AC_CONFIG_FILES([opal/mca/backtrace/none/Makefile])
dnl ----------------------------------------------------------------

dnl m4-configure component: 
dnl    opal/mca/backtrace/printstack

AC_CONFIG_FILES([opal/mca/backtrace/printstack/Makefile])
AC_CONFIG_FILES(opal/mca/carto/Makefile)
dnl ----------------------------------------------------------------

dnl No-configure component: 
dnl    opal/mca/carto/auto_detect

AC_CONFIG_FILES([opal/mca/carto/auto_detect/Makefile])
dnl ----------------------------------------------------------------

dnl No-configure component: 
dnl    opal/mca/carto/file

AC_CONFIG_FILES([opal/mca/carto/file/Makefile])
AC_CONFIG_FILES(opal/mca/crs/Makefile)
dnl ----------------------------------------------------------------

dnl m4-configure component: 
dnl    opal/mca/crs/blcr

AC_CONFIG_FILES([opal/mca/crs/blcr/Makefile])
dnl ----------------------------------------------------------------

dnl No-configure component: 
dnl    opal/mca/crs/none

AC_CONFIG_FILES([opal/mca/crs/none/Makefile])
dnl ----------------------------------------------------------------

dnl m4-configure component: 
dnl    opal/mca/crs/self

AC_CONFIG_FILES([opal/mca/crs/self/Makefile])
AC_CONFIG_FILES(opal/mca/installdirs/Makefile)
dnl ----------------------------------------------------------------

dnl m4-configure component: 
dnl    opal/mca/installdirs/config

AC_CONFIG_FILES([opal/mca/installdirs/config/Makefile])
dnl ----------------------------------------------------------------

dnl m4-configure component: 
dnl    opal/mca/installdirs/env

AC_CONFIG_FILES([opal/mca/installdirs/env/Makefile])
dnl ----------------------------------------------------------------

dnl m4-configure component: 
dnl    opal/mca/installdirs/windows

AC_CONFIG_FILES([opal/mca/installdirs/windows/Makefile])
AC_CONFIG_FILES(opal/mca/maffinity/Makefile)
dnl ----------------------------------------------------------------

dnl No-configure component: 
dnl    opal/mca/maffinity/first_use

AC_CONFIG_FILES([opal/mca/maffinity/first_use/Makefile])
dnl ----------------------------------------------------------------

dnl m4-configure component: 
dnl    opal/mca/maffinity/libnuma

AC_CONFIG_FILES([opal/mca/maffinity/libnuma/Makefile])
AC_CONFIG_FILES(opal/mca/memchecker/Makefile)
dnl ----------------------------------------------------------------

dnl m4-configure component: 
dnl    opal/mca/memchecker/valgrind

AC_CONFIG_FILES([opal/mca/memchecker/valgrind/Makefile])
AC_CONFIG_FILES(opal/mca/memcpy/Makefile)
AC_CONFIG_FILES(opal/mca/memory/Makefile)
dnl ----------------------------------------------------------------

dnl m4-configure component: 
dnl    opal/mca/memory/malloc_solaris

AC_CONFIG_FILES([opal/mca/memory/malloc_solaris/Makefile])
dnl ----------------------------------------------------------------

dnl m4-configure component: 
dnl    opal/mca/memory/ptmalloc2

AC_CONFIG_FILES([opal/mca/memory/ptmalloc2/Makefile])
AC_CONFIG_FILES(opal/mca/paffinity/Makefile)
dnl ----------------------------------------------------------------

dnl m4-configure component: 
dnl    opal/mca/paffinity/darwin

AC_CONFIG_FILES([opal/mca/paffinity/darwin/Makefile])
dnl ----------------------------------------------------------------

dnl m4-configure component: 
dnl    opal/mca/paffinity/linux

AC_CONFIG_FILES([opal/mca/paffinity/linux/Makefile])
dnl ----------------------------------------------------------------

dnl m4-configure component: 
dnl    opal/mca/paffinity/posix

AC_CONFIG_FILES([opal/mca/paffinity/posix/Makefile])
dnl ----------------------------------------------------------------

dnl m4-configure component: 
dnl    opal/mca/paffinity/solaris

AC_CONFIG_FILES([opal/mca/paffinity/solaris/Makefile])
dnl ----------------------------------------------------------------

dnl m4-configure component: 
dnl    opal/mca/paffinity/test

AC_CONFIG_FILES([opal/mca/paffinity/test/Makefile])
dnl ----------------------------------------------------------------

dnl m4-configure component: 
dnl    opal/mca/paffinity/windows

AC_CONFIG_FILES([opal/mca/paffinity/windows/Makefile])
AC_CONFIG_FILES(opal/mca/timer/Makefile)
dnl ----------------------------------------------------------------

dnl m4-configure component: 
dnl    opal/mca/timer/aix

AC_CONFIG_FILES([opal/mca/timer/aix/Makefile])
dnl ----------------------------------------------------------------

dnl m4-configure component: 
dnl    opal/mca/timer/altix

AC_CONFIG_FILES([opal/mca/timer/altix/Makefile])
dnl ----------------------------------------------------------------

dnl m4-configure component: 
dnl    opal/mca/timer/catamount

AC_CONFIG_FILES([opal/mca/timer/catamount/Makefile])
dnl ----------------------------------------------------------------

dnl m4-configure component: 
dnl    opal/mca/timer/darwin

AC_CONFIG_FILES([opal/mca/timer/darwin/Makefile])
dnl ----------------------------------------------------------------

dnl m4-configure component: 
dnl    opal/mca/timer/linux

AC_CONFIG_FILES([opal/mca/timer/linux/Makefile])
dnl ----------------------------------------------------------------

dnl m4-configure component: 
dnl    opal/mca/timer/solaris

AC_CONFIG_FILES([opal/mca/timer/solaris/Makefile])
dnl ----------------------------------------------------------------

dnl m4-configure component: 
dnl    opal/mca/timer/windows

AC_CONFIG_FILES([opal/mca/timer/windows/Makefile])
AC_CONFIG_FILES(orte/mca/errmgr/Makefile)
dnl ----------------------------------------------------------------

dnl No-configure component: 
dnl    orte/mca/errmgr/default

AC_CONFIG_FILES([orte/mca/errmgr/default/Makefile])
AC_CONFIG_FILES(orte/mca/ess/Makefile)
dnl ----------------------------------------------------------------

dnl m4-configure component: 
dnl    orte/mca/ess/alps

AC_CONFIG_FILES([orte/mca/ess/alps/Makefile])
dnl ----------------------------------------------------------------

dnl m4-configure component: 
dnl    orte/mca/ess/cnos

AC_CONFIG_FILES([orte/mca/ess/cnos/Makefile])
dnl ----------------------------------------------------------------

dnl m4-configure component: 
dnl    orte/mca/ess/env

AC_CONFIG_FILES([orte/mca/ess/env/Makefile])
dnl ----------------------------------------------------------------

dnl m4-configure component: 
dnl    orte/mca/ess/hnp

AC_CONFIG_FILES([orte/mca/ess/hnp/Makefile])
dnl ----------------------------------------------------------------

dnl m4-configure component: 
dnl    orte/mca/ess/lsf

AC_CONFIG_FILES([orte/mca/ess/lsf/Makefile])
dnl ----------------------------------------------------------------

dnl m4-configure component: 
dnl    orte/mca/ess/portals_utcp

AC_CONFIG_FILES([orte/mca/ess/portals_utcp/Makefile])
dnl ----------------------------------------------------------------

dnl m4-configure component: 
dnl    orte/mca/ess/singleton

AC_CONFIG_FILES([orte/mca/ess/singleton/Makefile])
dnl ----------------------------------------------------------------

dnl m4-configure component: 
dnl    orte/mca/ess/slurm

AC_CONFIG_FILES([orte/mca/ess/slurm/Makefile])
dnl ----------------------------------------------------------------

dnl m4-configure component: 
dnl    orte/mca/ess/tool

AC_CONFIG_FILES([orte/mca/ess/tool/Makefile])
AC_CONFIG_FILES(orte/mca/filem/Makefile)
dnl ----------------------------------------------------------------

dnl No-configure component: 
dnl    orte/mca/filem/rsh

AC_CONFIG_FILES([orte/mca/filem/rsh/Makefile])
AC_CONFIG_FILES(orte/mca/grpcomm/Makefile)
dnl ----------------------------------------------------------------

dnl m4-configure component: 
dnl    orte/mca/grpcomm/bad

AC_CONFIG_FILES([orte/mca/grpcomm/bad/Makefile])
dnl ----------------------------------------------------------------

dnl m4-configure component: 
dnl    orte/mca/grpcomm/basic

AC_CONFIG_FILES([orte/mca/grpcomm/basic/Makefile])
dnl ----------------------------------------------------------------

dnl m4-configure component: 
dnl    orte/mca/grpcomm/cnos

AC_CONFIG_FILES([orte/mca/grpcomm/cnos/Makefile])
AC_CONFIG_FILES(orte/mca/iof/Makefile)
dnl ----------------------------------------------------------------

dnl No-configure component: 
dnl    orte/mca/iof/hnp

AC_CONFIG_FILES([orte/mca/iof/hnp/Makefile])
dnl ----------------------------------------------------------------

dnl No-configure component: 
dnl    orte/mca/iof/orted

AC_CONFIG_FILES([orte/mca/iof/orted/Makefile])
dnl ----------------------------------------------------------------

dnl No-configure component: 
dnl    orte/mca/iof/tool

AC_CONFIG_FILES([orte/mca/iof/tool/Makefile])
AC_CONFIG_FILES(orte/mca/notifier/Makefile)
dnl ----------------------------------------------------------------

dnl m4-configure component: 
dnl    orte/mca/notifier/syslog

AC_CONFIG_FILES([orte/mca/notifier/syslog/Makefile])
AC_CONFIG_FILES(orte/mca/odls/Makefile)
dnl ----------------------------------------------------------------

dnl m4-configure component: 
dnl    orte/mca/odls/default

AC_CONFIG_FILES([orte/mca/odls/default/Makefile])
dnl ----------------------------------------------------------------

dnl m4-configure component: 
dnl    orte/mca/odls/process

AC_CONFIG_FILES([orte/mca/odls/process/Makefile])
AC_CONFIG_FILES(orte/mca/oob/Makefile)
dnl ----------------------------------------------------------------

dnl m4-configure component: 
dnl    orte/mca/oob/tcp

AC_CONFIG_FILES([orte/mca/oob/tcp/Makefile])
AC_CONFIG_FILES(orte/mca/plm/Makefile)
dnl ----------------------------------------------------------------

dnl m4-configure component: 
dnl    orte/mca/plm/alps

AC_CONFIG_FILES([orte/mca/plm/alps/Makefile])
dnl ----------------------------------------------------------------

dnl m4-configure component: 
dnl    orte/mca/plm/ccp

AC_CONFIG_FILES([orte/mca/plm/ccp/Makefile])
dnl ----------------------------------------------------------------

dnl m4-configure component: 
dnl    orte/mca/plm/lsf

AC_CONFIG_FILES([orte/mca/plm/lsf/Makefile])
dnl ----------------------------------------------------------------

dnl m4-configure component: 
dnl    orte/mca/plm/process

AC_CONFIG_FILES([orte/mca/plm/process/Makefile])
dnl ----------------------------------------------------------------

dnl m4-configure component: 
dnl    orte/mca/plm/rsh

AC_CONFIG_FILES([orte/mca/plm/rsh/Makefile])
dnl ----------------------------------------------------------------

dnl m4-configure component: 
dnl    orte/mca/plm/slurm

AC_CONFIG_FILES([orte/mca/plm/slurm/Makefile])
dnl ----------------------------------------------------------------

dnl m4-configure component: 
dnl    orte/mca/plm/tm

AC_CONFIG_FILES([orte/mca/plm/tm/Makefile])
dnl ----------------------------------------------------------------

dnl m4-configure component: 
dnl    orte/mca/plm/xgrid

AC_CONFIG_FILES([orte/mca/plm/xgrid/Makefile])
AC_CONFIG_FILES(orte/mca/ras/Makefile)
dnl ----------------------------------------------------------------

dnl m4-configure component: 
dnl    orte/mca/ras/alps

AC_CONFIG_FILES([orte/mca/ras/alps/Makefile])
dnl ----------------------------------------------------------------

dnl m4-configure component: 
dnl    orte/mca/ras/ccp

AC_CONFIG_FILES([orte/mca/ras/ccp/Makefile])
dnl ----------------------------------------------------------------

dnl m4-configure component: 
dnl    orte/mca/ras/gridengine

AC_CONFIG_FILES([orte/mca/ras/gridengine/Makefile])
dnl ----------------------------------------------------------------

dnl m4-configure component: 
dnl    orte/mca/ras/loadleveler

AC_CONFIG_FILES([orte/mca/ras/loadleveler/Makefile])
dnl ----------------------------------------------------------------

dnl m4-configure component: 
dnl    orte/mca/ras/lsf

AC_CONFIG_FILES([orte/mca/ras/lsf/Makefile])
dnl ----------------------------------------------------------------

dnl m4-configure component: 
dnl    orte/mca/ras/slurm

AC_CONFIG_FILES([orte/mca/ras/slurm/Makefile])
dnl ----------------------------------------------------------------

dnl m4-configure component: 
dnl    orte/mca/ras/tm

AC_CONFIG_FILES([orte/mca/ras/tm/Makefile])
AC_CONFIG_FILES(orte/mca/rmaps/Makefile)
dnl ----------------------------------------------------------------

dnl No-configure component: 
dnl    orte/mca/rmaps/load_balance

AC_CONFIG_FILES([orte/mca/rmaps/load_balance/Makefile])
dnl ----------------------------------------------------------------

dnl No-configure component: 
dnl    orte/mca/rmaps/rank_file

AC_CONFIG_FILES([orte/mca/rmaps/rank_file/Makefile])
dnl ----------------------------------------------------------------

dnl No-configure component: 
dnl    orte/mca/rmaps/round_robin

AC_CONFIG_FILES([orte/mca/rmaps/round_robin/Makefile])
dnl ----------------------------------------------------------------

dnl No-configure component: 
dnl    orte/mca/rmaps/seq

AC_CONFIG_FILES([orte/mca/rmaps/seq/Makefile])
AC_CONFIG_FILES(orte/mca/rml/Makefile)
dnl ----------------------------------------------------------------

dnl m4-configure component: 
dnl    orte/mca/rml/ftrm

AC_CONFIG_FILES([orte/mca/rml/ftrm/Makefile])
dnl ----------------------------------------------------------------

dnl No-configure component: 
dnl    orte/mca/rml/oob

AC_CONFIG_FILES([orte/mca/rml/oob/Makefile])
AC_CONFIG_FILES(orte/mca/routed/Makefile)
dnl ----------------------------------------------------------------

dnl No-configure component: 
dnl    orte/mca/routed/binomial

AC_CONFIG_FILES([orte/mca/routed/binomial/Makefile])
dnl ----------------------------------------------------------------

dnl No-configure component: 
dnl    orte/mca/routed/direct

AC_CONFIG_FILES([orte/mca/routed/direct/Makefile])
dnl ----------------------------------------------------------------

dnl No-configure component: 
dnl    orte/mca/routed/linear

AC_CONFIG_FILES([orte/mca/routed/linear/Makefile])
AC_CONFIG_FILES(orte/mca/snapc/Makefile)
dnl ----------------------------------------------------------------

dnl m4-configure component: 
dnl    orte/mca/snapc/full

AC_CONFIG_FILES([orte/mca/snapc/full/Makefile])
AC_CONFIG_FILES(ompi/mca/allocator/Makefile)
dnl ----------------------------------------------------------------

dnl No-configure component: 
dnl    ompi/mca/allocator/basic

AC_CONFIG_FILES([ompi/mca/allocator/basic/Makefile])
dnl ----------------------------------------------------------------

dnl No-configure component: 
dnl    ompi/mca/allocator/bucket

AC_CONFIG_FILES([ompi/mca/allocator/bucket/Makefile])
AC_CONFIG_FILES(ompi/mca/bml/Makefile)
dnl ----------------------------------------------------------------

dnl No-configure component: 
dnl    ompi/mca/bml/r2

AC_CONFIG_FILES([ompi/mca/bml/r2/Makefile])
AC_CONFIG_FILES(ompi/mca/btl/Makefile)
dnl ----------------------------------------------------------------

dnl m4-configure component: 
dnl    ompi/mca/btl/elan

AC_CONFIG_FILES([ompi/mca/btl/elan/Makefile])
dnl ----------------------------------------------------------------

dnl m4-configure component: 
dnl    ompi/mca/btl/gm

AC_CONFIG_FILES([ompi/mca/btl/gm/Makefile])
dnl ----------------------------------------------------------------

dnl m4-configure component: 
dnl    ompi/mca/btl/mx

AC_CONFIG_FILES([ompi/mca/btl/mx/Makefile])
dnl ----------------------------------------------------------------

dnl m4-configure component: 
dnl    ompi/mca/btl/ofud

AC_CONFIG_FILES([ompi/mca/btl/ofud/Makefile])
dnl ----------------------------------------------------------------

dnl m4-configure component: 
dnl    ompi/mca/btl/openib

AC_CONFIG_FILES([ompi/mca/btl/openib/Makefile])
dnl ----------------------------------------------------------------

dnl m4-configure component: 
dnl    ompi/mca/btl/portals

AC_CONFIG_FILES([ompi/mca/btl/portals/Makefile])
dnl ----------------------------------------------------------------

dnl No-configure component: 
dnl    ompi/mca/btl/self

AC_CONFIG_FILES([ompi/mca/btl/self/Makefile])
dnl ----------------------------------------------------------------

dnl No-configure component: 
dnl    ompi/mca/btl/sm

AC_CONFIG_FILES([ompi/mca/btl/sm/Makefile])
dnl ----------------------------------------------------------------

dnl m4-configure component: 
dnl    ompi/mca/btl/tcp

AC_CONFIG_FILES([ompi/mca/btl/tcp/Makefile])
dnl ----------------------------------------------------------------

dnl m4-configure component: 
dnl    ompi/mca/btl/udapl

AC_CONFIG_FILES([ompi/mca/btl/udapl/Makefile])
AC_CONFIG_FILES(ompi/mca/coll/Makefile)
dnl ----------------------------------------------------------------

dnl No-configure component: 
dnl    ompi/mca/coll/basic

AC_CONFIG_FILES([ompi/mca/coll/basic/Makefile])
dnl ----------------------------------------------------------------

dnl No-configure component: 
dnl    ompi/mca/coll/hierarch

AC_CONFIG_FILES([ompi/mca/coll/hierarch/Makefile])
dnl ----------------------------------------------------------------

dnl No-configure component: 
dnl    ompi/mca/coll/inter

AC_CONFIG_FILES([ompi/mca/coll/inter/Makefile])
dnl ----------------------------------------------------------------

dnl No-configure component: 
dnl    ompi/mca/coll/self

AC_CONFIG_FILES([ompi/mca/coll/self/Makefile])
dnl ----------------------------------------------------------------

dnl No-configure component: 
dnl    ompi/mca/coll/sm

AC_CONFIG_FILES([ompi/mca/coll/sm/Makefile])
dnl ----------------------------------------------------------------

dnl No-configure component: 
dnl    ompi/mca/coll/sync

AC_CONFIG_FILES([ompi/mca/coll/sync/Makefile])
dnl ----------------------------------------------------------------

dnl No-configure component: 
dnl    ompi/mca/coll/tuned

AC_CONFIG_FILES([ompi/mca/coll/tuned/Makefile])
AC_CONFIG_FILES(ompi/mca/common/Makefile)
dnl ----------------------------------------------------------------

dnl m4-configure component: 
dnl    ompi/mca/common/mx

AC_CONFIG_FILES([ompi/mca/common/mx/Makefile])
dnl ----------------------------------------------------------------

dnl m4-configure component: 
dnl    ompi/mca/common/portals

AC_CONFIG_FILES([ompi/mca/common/portals/Makefile])
dnl ----------------------------------------------------------------

dnl No-configure component: 
dnl    ompi/mca/common/sm

AC_CONFIG_FILES([ompi/mca/common/sm/Makefile])
AC_CONFIG_FILES(ompi/mca/crcp/Makefile)
dnl ----------------------------------------------------------------

dnl m4-configure component: 
dnl    ompi/mca/crcp/bkmrk

AC_CONFIG_FILES([ompi/mca/crcp/bkmrk/Makefile])
AC_CONFIG_FILES(ompi/mca/dpm/Makefile)
dnl ----------------------------------------------------------------

dnl No-configure component: 
dnl    ompi/mca/dpm/orte

AC_CONFIG_FILES([ompi/mca/dpm/orte/Makefile])
AC_CONFIG_FILES(ompi/mca/io/Makefile)
dnl ----------------------------------------------------------------

dnl m4-configure component: 
dnl    ompi/mca/io/romio

AC_CONFIG_FILES([ompi/mca/io/romio/Makefile])
AC_CONFIG_FILES(ompi/mca/mpool/Makefile)
dnl ----------------------------------------------------------------

dnl No-configure component: 
dnl    ompi/mca/mpool/fake

AC_CONFIG_FILES([ompi/mca/mpool/fake/Makefile])
dnl ----------------------------------------------------------------

dnl No-configure component: 
dnl    ompi/mca/mpool/rdma

AC_CONFIG_FILES([ompi/mca/mpool/rdma/Makefile])
dnl ----------------------------------------------------------------

dnl No-configure component: 
dnl    ompi/mca/mpool/sm

AC_CONFIG_FILES([ompi/mca/mpool/sm/Makefile])
AC_CONFIG_FILES(ompi/mca/mtl/Makefile)
dnl ----------------------------------------------------------------

dnl m4-configure component: 
dnl    ompi/mca/mtl/mx

AC_CONFIG_FILES([ompi/mca/mtl/mx/Makefile])
dnl ----------------------------------------------------------------

dnl m4-configure component: 
dnl    ompi/mca/mtl/portals

AC_CONFIG_FILES([ompi/mca/mtl/portals/Makefile])
dnl ----------------------------------------------------------------

dnl m4-configure component: 
dnl    ompi/mca/mtl/psm

AC_CONFIG_FILES([ompi/mca/mtl/psm/Makefile])
AC_CONFIG_FILES(ompi/mca/osc/Makefile)
dnl ----------------------------------------------------------------

dnl No-configure component: 
dnl    ompi/mca/osc/pt2pt

AC_CONFIG_FILES([ompi/mca/osc/pt2pt/Makefile])
dnl ----------------------------------------------------------------

dnl No-configure component: 
dnl    ompi/mca/osc/rdma

AC_CONFIG_FILES([ompi/mca/osc/rdma/Makefile])
AC_CONFIG_FILES(ompi/mca/pml/Makefile)
dnl ----------------------------------------------------------------

dnl No-configure component: 
dnl    ompi/mca/pml/cm

AC_CONFIG_FILES([ompi/mca/pml/cm/Makefile])
dnl ----------------------------------------------------------------

dnl m4-configure component: 
dnl    ompi/mca/pml/crcpw

AC_CONFIG_FILES([ompi/mca/pml/crcpw/Makefile])
dnl ----------------------------------------------------------------

dnl No-configure component: 
dnl    ompi/mca/pml/csum

AC_CONFIG_FILES([ompi/mca/pml/csum/Makefile])
dnl ----------------------------------------------------------------

dnl No-configure component: 
dnl    ompi/mca/pml/ob1

AC_CONFIG_FILES([ompi/mca/pml/ob1/Makefile])
AC_CONFIG_FILES(ompi/mca/pml/v/mca/vprotocol/Makefile)
dnl ----------------------------------------------------------------

dnl No-configure component: 
dnl    ompi/mca/pml/v/mca/vprotocol/pessimist

AC_CONFIG_FILES([ompi/mca/pml/v/mca/vprotocol/pessimist/Makefile])
dnl ----------------------------------------------------------------

dnl m4-configure component: 
dnl    ompi/mca/pml/v

AC_CONFIG_FILES([ompi/mca/pml/v/Makefile])
AC_CONFIG_FILES(ompi/mca/pubsub/Makefile)
dnl ----------------------------------------------------------------

dnl No-configure component: 
dnl    ompi/mca/pubsub/orte

AC_CONFIG_FILES([ompi/mca/pubsub/orte/Makefile])
AC_CONFIG_FILES(ompi/mca/rcache/Makefile)
dnl ----------------------------------------------------------------

dnl No-configure component: 
dnl    ompi/mca/rcache/vma

AC_CONFIG_FILES([ompi/mca/rcache/vma/Makefile])
AC_CONFIG_FILES(ompi/mca/topo/Makefile)
dnl ----------------------------------------------------------------

dnl No-configure component: 
dnl    ompi/mca/topo/unity

AC_CONFIG_FILES([ompi/mca/topo/unity/Makefile])
])dnl
