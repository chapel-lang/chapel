dnl Configury specific to the libfabric PSM3 provider

dnl Called to configure this provider
dnl
dnl Arguments:
dnl
dnl $1: action if configured successfully
dnl $2: action if not configured successfully
dnl
AC_DEFUN([FI_PSM3_CONFIGURE],[
     # Determine if we can support the psm3 provider
     psm3_ARCH=$host_cpu
     AM_CONDITIONAL([HAVE_PSM3_X86_64], [test x$psm3_ARCH = xx86_64])
     AC_SUBST([HAVE_PSM3_X86_64])
     AC_SUBST([psm3_ARCH])

     enable_psm3_src=yes
     AM_CONDITIONAL([HAVE_PSM3_SRC], [test "x$enable_psm3_src" != "xno"], [build PSM3 src into provider])
     AC_DEFINE([HAVE_PSM3_SRC], [1], [PSM3 source is built-in])

     PSM3_HAL_INST=""
     PSM3_HAL_CNT=0

     psm3_happy=1
     AS_IF([test x"$enable_psm3" != x"no"],
           [
        AS_IF([test "x$enable_psm3_verbs" = "xyes"],
              [
            PSM3_HAL_INST="$PSM3_HAL_INST verbs"
            PSM3_HAL_CNT=$((PSM3_HAL_CNT+1))
            psm3_CPPFLAGS="$psm3_CPPFLAGS -DPSM_VERBS"
              ])
        AS_IF([test "x$enable_psm3_sockets" = "xyes"],
              [
            PSM3_HAL_INST="$PSM3_HAL_INST sockets"
            PSM3_HAL_CNT=$((PSM3_HAL_CNT+1))
            psm3_CPPFLAGS="$psm3_CPPFLAGS -DPSM_SOCKETS"
              ])
        PSM3_HAL_INST=${PSM3_HAL_INST# }

        AS_IF([test "$PSM3_HAL_CNT" -le 0],
              [
            psm3_happy=0
            AC_MSG_ERROR([No HALs enable. Must enable at least one HAL])
              ])

        AS_IF([test "x$enable_psm3_udp" = "xyes"],
              [
            AS_IF([test "x$enable_psm3_sockets" = "xyes"],
                  [psm3_CPPFLAGS="$psm3_CPPFLAGS -DUSE_UDP"],
                  [AC_MSG_ERROR([UDP requires Scokets HAL active])])
              ])
        dnl RC requires the Verbs HAL, so if Verbs HAL is disabled, then this should default to match verbs HAL
        AS_IF([test "x$enable_psm3_rc" = "xcheck"],
              [enable_psm3_rc=$enable_psm3_verbs])
        AS_IF([test "x$enable_psm3_rc" = "xyes"],
              [
            AS_IF([test "x$enable_psm3_verbs" = "xyes"],
                  [psm3_CPPFLAGS="$psm3_CPPFLAGS -DUSE_RC"],
                  [AC_MSG_ERROR([User RC QPs requires Verbs HAL active])])
              ])

        FI_CHECK_PACKAGE([psm3_rt],
                         [sys/mman.h],
                         [rt],
                         [shm_open],
                         [],
                         [$psm3_PREFIX],
                         [$psm3_LIBDIR],
                         [],
                         [psm3_happy=0])

        FI_CHECK_PACKAGE([psm3_numa],
                         [numa.h],
                         [numa],
                         [numa_node_of_cpu],
                         [],
                         [$psm3_PREFIX],
                         [$psm3_LIBDIR],
                         [psm3_numa_happy=1],
                         [psm3_happy=0])

        AS_IF([test "x$enable_psm3_verbs" = "xyes"],
              [
            FI_CHECK_PACKAGE([psm3_ibv],
                             [infiniband/verbs.h],
                             [ibverbs],
                             [ibv_get_device_list],
                             [],
                             [$psm3_PREFIX],
                             [$psm3_LIBDIR],
                             [psm3_ibv_happy=1],
                             [psm3_happy=0])
              ])

        FI_CHECK_PACKAGE([psm3_uuid],
                         [uuid/uuid.h],
                         [uuid],
                         [uuid_parse],
                         [],
                         [$psm3_PREFIX],
                         [$psm3_LIBDIR],
                         [psm3_uuid_happy=1],
                         [psm3_happy=0])

        AC_MSG_CHECKING([for -msse4.2 support])

        dnl Strip other optflags to avoid conflicts when checking for instruction sets
        FI_STRIP_OPTFLAGS($CFLAGS)
        PSM3_STRIP_OPTFLAGS="$s_result"

        save_CFLAGS=$CFLAGS
        CFLAGS="$PSM3_STRIP_OPTFLAGS -msse4.2 -O0"
        AC_LINK_IFELSE(
            [AC_LANG_PROGRAM(
                [[#include <nmmintrin.h>]],
                [[unsigned int crc = 0;
                  crc = _mm_crc32_u32(crc, 0);
                  return crc == 0;]])
            ],[
                AC_MSG_RESULT([yes])
                PSM3_ARCH_CFLAGS="-msse4.2"
            ],[
                psm3_happy=0
                AC_MSG_RESULT([no])
                AC_MSG_NOTICE([psm3 requires minimum of sse4.2 instruction set to build])
            ])
        CFLAGS=$save_CFLAGS

        AC_MSG_CHECKING([for -mavx2 support (recommended)])
        save_CFLAGS=$CFLAGS
        CFLAGS="$PSM3_STRIP_OPTFLAGS -O0"
        AC_LINK_IFELSE(
            [AC_LANG_PROGRAM(
                [[#include <immintrin.h>]],
                [[unsigned long long _a[4] = {1ULL,2ULL,3ULL,4ULL};
                  __m256i vA = _mm256_loadu_si256((__m256i *)_a);
                  __m256i vB = _mm256_add_epi64(vA, vA);
                  (void)vB;
                  return 0;]])
            ],[
                AC_MSG_RESULT([yes])
            ],[
                AC_MSG_RESULT([no])
                AC_MSG_NOTICE([psm3 recommends minimum of avx2 instruction set for best performance])
            ])
        CFLAGS=$save_CFLAGS

        AC_PROG_GREP()

        AC_MSG_CHECKING([for -Wno-address-of-packed-member support])
        save_CFLAGS="$CFLAGS"
        dnl - Invalid flag will only show on failure
        CFLAGS="-Wno-address-of-packed-member -Werror"
        AC_COMPILE_IFELSE(
            [AC_LANG_PROGRAM([#error "Error on purpose"])], [],
            [
                dnl - Check if flag is in error message
                if $GREP 'Wno-address-of-packed-member' conftest.err >/dev/null; then
                    AC_MSG_RESULT(no)
                else
                    AC_MSG_RESULT(yes)
                    psm3_CFLAGS="$psm3_CFLAGS -Wno-address-of-packed-member"
                fi
            ])
        CFLAGS=$save_CFLAGS

        have_libcuda=0
        AS_IF([test $have_cuda -eq 1 && test $cuda_dlopen -eq 1], [have_libcuda=1])
        AS_IF([test $have_libcuda -eq 1],
              [psm3_CPPFLAGS="$psm3_CPPFLAGS -DPSM_CUDA -DNVIDIA_GPU_DIRECT"])
        AC_DEFINE_UNQUOTED([PSM3_CUDA], [$have_libcuda], [Whether we have CUDA runtime or not])

        have_oneapi_ze=0
        AS_IF([test $have_ze -eq 1 && test $ze_dlopen -eq 1], [have_oneapi_ze=1])
        AS_IF([test $have_oneapi_ze -eq 1],
              [psm3_CPPFLAGS="$psm3_CPPFLAGS -DPSM_ONEAPI -DINTEL_GPU_DIRECT"])
        AS_IF([test $have_drm -eq 1], [psm3_CPPFLAGS="$psm3_CPPFLAGS -DHAVE_DRM"])
        AS_IF([test $have_libdrm -eq 1], [psm3_CPPFLAGS="$psm3_CPPFLAGS -DHAVE_LIBDRM"])
        AS_IF([test $have_oneapi_ze -eq 1],
              [
                  AC_MSG_CHECKING([for pidfd support])
                  AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
                          #include <sys/types.h>
                          #include <unistd.h>
                          #include <sys/syscall.h>
                       ]],
                       [[
                          int fd;
                          int pidfd;
                          int rem_fd;
                          fd = syscall(__NR_pidfd_getfd, pidfd, rem_fd, 0);
                       ]])
                     ],
                     [
                         AC_MSG_RESULT([yes])
                         psm3_CPPFLAGS="$psm3_CPPFLAGS -DPSM_HAVE_PIDFD"
                     ],
                     [AC_MSG_RESULT([no])])
              ])
        AC_DEFINE_UNQUOTED([PSM3_ONEAPI], [$have_oneapi_ze], [Whether we have oneAPI Level-Zero runtime or not])

        AS_IF([test "$have_oneapi_ze" = "1"],
            [
                save_LDFLAGS="$LDFLAGS"
                LDFLAGS="$LDFLAGS -lze_loader"
                AC_MSG_CHECKING([for zeMemPutIpcHandle support in level-zero])
                AC_LINK_IFELSE(
                    [AC_LANG_PROGRAM([[
                            #include <level_zero/ze_api.h>
                        ]],[[
                            ze_context_handle_t hContext = NULL;
                            ze_ipc_mem_handle_t handle;
                            (void)zeMemPutIpcHandle(hContext, handle);
                        ]])
                    ],[
                        AC_MSG_RESULT(yes)
                        psm3_CPPFLAGS="$psm3_CPPFLAGS -DPSM_HAVE_ONEAPI_ZE_PUT_IPCHANDLE"
                        LDFLAGS="$save_LDFLAGS"
                    ],[
                        AC_MSG_RESULT(no)
                        LDFLAGS="$save_LDFLAGS"
                    ])
            ])


        AS_IF([test x$with_psm3_rv != xno],
              [
            AS_IF([test "x$with_psm3_rv" = "x"],
                  [
                psm3_rv_check=1
                with_psm3_rv=/usr/include
                  ],[psm3_rv_check=0])
            save_CPPFLAGS=$CPPFLAGS
            CPPFLAGS="$CPPFLAGS -I$with_psm3_rv"
            _FI_CHECK_PACKAGE_HEADER([psm3_rv],
                                     [rdma/rv_user_ioctls.h],
                                     [],
                                     [psm3_rv_happy=1],
                                     [psm3_rv_happy=0])

            CPPFLAGS=$save_CPPFLAGS
            AS_IF([test "$psm3_rv_happy" -eq 0],
                  [
                AS_IF([test "$psm3_rv_check" -eq 0], [
                    psm3_happy=0
                    AC_MSG_ERROR([RV Module headers requested but rv_user_ioctls.h not found.])
                      ])
                  ],[
                AS_IF([test "$psm3_rv_check" -eq 1],
                      [
                    dnl Include under any of the following cases
                    dnl  - Verbs
                    dnl  - Sockets and CUDA
                    AS_IF([test "x$enable_psm3_sockets" = "xyes" && test "$have_libcuda" -eq 1],
                          [psm3_sockets_with_cuda=1],
                          [psm3_sockets_with_cuda=0])
                    AS_IF([test "x$enable_psm3_verbs" = "xyes" || test "$psm3_sockets_with_cuda" -eq 1],
                          [
                        psm3_CPPFLAGS="$psm3_CPPFLAGS -DRNDV_MOD -I$with_psm3_rv"
                          ],[
                        AC_MSG_NOTICE([RV Module headers found, but not included, as current config does not support RV])
                        psm3_rv_happy=0
                          ])
                      ],[
                    psm3_CPPFLAGS="$psm3_CPPFLAGS -DRNDV_MOD -I$with_psm3_rv"
                      ])
                  ])
            AS_IF([test "$psm3_rv_happy" -eq 1], [
                AC_MSG_CHECKING([for RV support for ring.overflow_cnt])
                AC_COMPILE_IFELSE(
                    [AC_LANG_PROGRAM(
                        [[#include <sys/types.h>
                          #include <stdint.h>
                          #include <rdma/rv_user_ioctls.h>]],
                        [[struct rv_ring_header ring;
                          ring.overflow_cnt=0;
                          (void)ring;
                          return 0;]])
                    ],[
                        AC_MSG_RESULT(yes)
                    ],[
                        AC_MSG_RESULT(no)
                        psm3_CPPFLAGS="$psm3_CPPFLAGS -DHAVE_NO_PSM3_RV_OVERFLOW_CNT"
                    ])
                  ])

              ])
        AS_IF([test "x$enable_psm3_dsa" != "xno"],
              [
            _FI_CHECK_PACKAGE_HEADER([psm3_dsa],
                                     [linux/idxd.h],
                                     [],
                                     [psm3_dsa_found=1],
                                     [psm3_dsa_found=0])
            AS_IF([test $psm3_dsa_found -ne 1 && test "x$enable_psm3_dsa" == "xyes"],
                  [
                psm3_happy=0
                AC_MSG_ERROR([DSA Support requested but linux/idxd.h not found.])
                  ])
            AS_IF([test "$psm3_dsa_found" -eq 1],
                  [
                AC_MSG_CHECKING([for full Intel DSA support])
                AC_COMPILE_IFELSE(
                    [AC_LANG_PROGRAM(
                        [[#include <linux/idxd.h>]],[[
                        struct dsa_hw_desc desc = {};
                        struct dsa_completion_record c = {};
                        desc.opcode = DSA_OPCODE_MEMMOVE;
                        desc.flags = IDXD_OP_FLAG_CRAV | IDXD_OP_FLAG_RCR | IDXD_OP_FLAG_CC
                            | IDXD_OP_FLAG_BOF;
                        desc.xfer_size = 0;
                        desc.src_addr = 0;
                        desc.dst_addr = 0;
                        desc.completion_addr = (uintptr_t)&c;
                        while (c.status == DSA_COMP_NONE) ;
                        if (c.status != DSA_COMP_SUCCESS) {
                            if ((c.status & DSA_COMP_STATUS_MASK) != DSA_COMP_PAGE_FAULT_NOBOF) {
                            return -1;
                            } else  if (c.status & DSA_COMP_STATUS_WRITE) {
                            return 0;
                            }
                        }
                        return 0;
                        ]])
                    ],[
                        AC_MSG_RESULT(yes)
                        psm3_CPPFLAGS="$psm3_CPPFLAGS -DPSM_DSA"
                    ],[
                        AC_MSG_RESULT(no)
                        AS_IF([test "x$enable_psm3_dsa" == "xyes"],
                              [
                            psm3_happy=0
                            AC_MSG_ERROR([Missing full DSA Support in linux/idxd.h.])
                              ])
                    ])
                  ])
              ])
        dnl RDMA_READ requires RC, so if RC is disabled, then this should default to match RC
        AS_IF([test "x$enable_psm3_rdma_read" = "xcheck"],
              [enable_psm3_rdma_read=$enable_psm3_rc])
        AS_IF([test "x$enable_psm3_rdma_read" = "xyes"],
              [
            AS_IF([test "x$enable_psm3_rc" = "xyes"],
                  [psm3_CPPFLAGS="$psm3_CPPFLAGS -DUSE_RDMA_READ"],
                  [AC_MSG_ERROR([RDMA_READ requires User RC QPs active])])
              ])
        AS_IF([test "x$enable_psm3_umr_cache" != "xno"],
              [
            # have_uffd is set in configure.ac
            AS_IF([test $have_uffd -ne 1 && test "x$enable_psm3_umr_cache" == "xyes"],
                  [
                psm3_happy=0
                AC_MSG_ERROR([UMR Support requested but linux/userfaultfd.h not found.])
                  ])
            AS_IF([test $have_uffd -eq 1],
                  [
                psm3_CPPFLAGS="$psm3_CPPFLAGS -DUMR_CACHE"
                  ])
        ])

        AS_IF([test "x$enable_psm3_hwloc" != "xno"],
              [
                FI_CHECK_PACKAGE([psm3_hwloc],
                                 [hwloc.h],
                                 [hwloc],
                                 [hwloc_topology_init],
                                 [],
                                 [$psm3_PREFIX],
                                 [$psm3_LIBDIR],
                                 [psm3_hwloc_found=1],
                                 [psm3_hwloc_found=0])
                AS_IF([test $psm3_hwloc_found -ne 1 && test "x$enable_psm3_hwloc" == "xyes"],
                      [
                        psm3_happy=0
                        AC_MSG_ERROR([hwloc Support requested but hwloc headers and/or library not found.])
                      ])
                AS_IF([test "$psm3_hwloc_found" -eq 1],
                      [
                        psm3_CPPFLAGS="$psm3_CPPFLAGS -DPSM_USE_HWLOC"
                      ])
               ])

        AS_IF([test $psm3_happy -eq 1], [
            AC_CONFIG_FILES([prov/psm3/psm3/psm2_hal_inlines_i.h \
                 prov/psm3/psm3/psm2_hal_inlines_d.h \
                 prov/psm3/src/psm3_revision.c])
        ])
           ],[psm3_happy=0])

     AS_IF([test $psm3_happy -eq 1], [$1], [$2])

     psm3_ARCH_CFLAGS="$PSM3_ARCH_CFLAGS"
     psm3_CPPFLAGS="$psm3_CPPFLAGS $psm3_rt_CPPFLAGS $psm3_dl_CPPFLAGS $psm3_numa_CPPFLAGS $psm3_ibv_CPPFLAGS $psm3_uuid_CPPFLAGS $psm3_hwloc_CPPFLAGS"
     psm3_LDFLAGS="$psm3_LDFLAGS $psm3_rt_LDFLAGS $psm3_dl_LDFLAGS $psm3_numa_LDFLAGS $psm3_ibv_LDFLAGS $psm3_uuid_LDFLAGS $psm3_hwloc_LDFLAGS"
     psm3_LIBS="$psm3_LIBS $psm3_rt_LIBS $psm3_dl_LIBS $psm3_numa_LIBS $psm3_ibv_LIBS $psm3_uuid_LIBS $psm3_hwloc_LIBS"
     AC_SUBST(psm3_CFLAGS)
     AC_SUBST(psm3_ARCH_CFLAGS)
     AC_SUBST(psm3_CPPFLAGS)
     AC_SUBST(psm3_LDFLAGS)
     AC_SUBST(psm3_LIBS)
     AC_SUBST(PSM3_HAL_CNT)
     AC_SUBST(PSM3_HAL_INST)

     PSM3_IEFS_VERSION=m4_normalize(m4_esyscmd([cat prov/psm3/VERSION]))
     AC_SUBST(PSM3_IEFS_VERSION)
     PSM3_PROV_VER_MAJOR=$(echo "${PSM3_IEFS_VERSION}" | cut -d'_' -f1)
     PSM3_PROV_VER_MINOR=$(echo "${PSM3_IEFS_VERSION}" | cut -d'_' -f2)
     PSM3_PROV_VER_MAINT=$(echo "${PSM3_IEFS_VERSION}" | cut -d'_' -f3)
     PSM3_PROV_VER_PATCH=$(echo "${PSM3_IEFS_VERSION}" | cut -d'_' -f4)
     AC_SUBST(PSM3_PROV_VER_MAJOR)
     AC_SUBST(PSM3_PROV_VER_MINOR)
     AC_SUBST(PSM3_PROV_VER_MAINT)
     AC_SUBST(PSM3_PROV_VER_PATCH)

     AC_SUBST(PSM3_BUILD_TIMESTAMP, ["<Unknown>"])
     AC_SUBST(PSM3_SRC_CHECKSUM, ["<Unknown>"])
     AC_SUBST(PSM3_GIT_HASH, ["<Unknown>"])

])

AC_ARG_WITH([psm3-rv],
    [AS_HELP_STRING([--with-psm3-rv],
        [Enable RV module use @<:@default=check@:>@])])
dnl ------------ HALs
AC_ARG_ENABLE([psm3-verbs],
    [AS_HELP_STRING([--enable-psm3-verbs],
        [Enable PSM3 support on Verbs HAL (UD QPs) @<:@default=yes@:>@])],
    [],
    [enable_psm3_verbs=yes])
AC_ARG_ENABLE([psm3-sockets],
    [AS_HELP_STRING([--enable-psm3-sockets],
        [Enable PSM3 support on Scokets HAL (TCP) @<:@default=yes@:>@])],
    [],
    [enable_psm3_sockets=yes])

dnl ------------- HAL Extensions
AC_ARG_ENABLE([psm3-udp],
    [AS_HELP_STRING([--enable-psm3-udp],
        [Enable UDP on applicable HALs @<:@default=no@:>@])],
    [],
    [enable_psm3_udp=no])
AC_ARG_ENABLE([psm3-rc],
    [AS_HELP_STRING([--enable-psm3-rc],
        [EXPERIMENTAL: Enable User Space RC QPs on applicable HALs @<:@default=check [check means match --enable-psm3-verbs option]@:>@])],
    [],
    [enable_psm3_rc=check])
dnl ------------- Extra Features
AC_ARG_ENABLE([psm3-dsa],
    [AS_HELP_STRING([--enable-psm3-dsa],
        [Enable support for Intel Data Streaming Accelerator (DSA) @<:@default=check@:>@])],
    [],
    [enable_psm3_dsa=check])
AC_ARG_ENABLE([psm3-rdma-read],
    [AS_HELP_STRING([--enable-psm3-rdma-read],
        [Enable RDMA_READ support @<:@default=check [check means match --enable-psm3-rc]@:>@])],
    [],
    [enable_psm3_rdma_read=check])
AC_ARG_ENABLE([psm3-umr-cache],
    [AS_HELP_STRING([--enable-psm3-umr-cache],
        [Enable support for Userspace Memory Region (UMR) Caching @<:@default=check@:>@])],
    [],
    [enable_psm3_umr_cache=check])
AC_ARG_ENABLE([psm3-hwloc],
    [AS_HELP_STRING([--enable-psm3-hwloc],
        [Enable PSM3 use of hwloc for NIC affinity selections @<:@default=check@:>@])],
    [],
    [enable_psm3_hwloc=check])
dnl vim: set ts=4 sw=4 tw=0 et :
