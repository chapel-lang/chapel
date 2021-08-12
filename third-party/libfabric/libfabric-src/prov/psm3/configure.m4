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

	 PSM_HAL_CNT=1
	 PSM_HAL_INST=gen1

	 psm3_happy=1
	 AS_IF([test x"$enable_psm3" != x"no"],
	       [
		FI_CHECK_PACKAGE([psm3_rt],
		                 [sys/mman.h],
		                 [rt],
		                 [shm_open],
		                 [],
		                 [$psm3_PREFIX],
		                 [$psm3_LIBDIR],
		                 [],
		                 [psm3_happy=0])
ifelse('
		FI_CHECK_PACKAGE([psm3_dl],
		                 [dlfcn.h],
		                 [dl],
		                 [dlopen],
		                 [],
		                 [$psm3_PREFIX],
		                 [$psm3_LIBDIR],
		                 [psm3_dl_happy=1],
		                 [psm3_happy=0])
')dnl
		FI_CHECK_PACKAGE([psm3_numa],
		                 [numa.h],
		                 [numa],
		                 [numa_node_of_cpu],
		                 [],
		                 [$psm3_PREFIX],
		                 [$psm3_LIBDIR],
		                 [psm3_numa_happy=1],
		                 [psm3_happy=0])

		FI_CHECK_PACKAGE([psm3_ibv],
		                 [infiniband/verbs.h],
		                 [ibverbs],
		                 [ibv_get_device_list],
		                 [],
		                 [$psm3_PREFIX],
		                 [$psm3_LIBDIR],
		                 [psm3_ibv_happy=1],
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
				AC_MSG_NOTICE([psm3 requires minimum of avx instruction set to build])
			])
		CFLAGS=$save_CFLAGS

		AC_MSG_CHECKING([for -mavx support])
		save_CFLAGS=$CFLAGS
		CFLAGS="$PSM3_STRIP_OPTFLAGS -mavx -O0"
		AC_LINK_IFELSE(
			[AC_LANG_PROGRAM(
				[[#include <immintrin.h>]],
				[[unsigned long long _a[4] = {1ULL,2ULL,3ULL,4ULL};
				  __m256i vA = _mm256_loadu_si256((__m256i *)_a);
				  __m256i vB;
				  _mm256_store_si256(&vB, vA);
				  return 0;]])
			],[
				AC_MSG_RESULT([yes])
				PSM3_ARCH_CFLAGS="-mavx"
			],[
				psm3_happy=0
				AC_MSG_RESULT([no])
				AC_MSG_NOTICE([psm3 requires minimum of avx instruction set to build])
			])
		CFLAGS=$save_CFLAGS

		AC_MSG_CHECKING([for -mavx2 support])
		save_CFLAGS=$CFLAGS
		CFLAGS="$PSM3_STRIP_OPTFLAGS -mavx2 -O0"
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
				PSM3_ARCH_CFLAGS="-mavx2"
			],[
				AC_MSG_RESULT([no])
			])
		CFLAGS=$save_CFLAGS

		AS_IF([test x$with_psm3_rv = xno],
		      [psm3_CPPFLAGS="$psm3_CPPFLAGS -URNDV_MOD"],
		      [
			AS_IF([test "x$with_psm3_rv" = "x"],
			      [
				psm3_rv_check=1
				with_psm3_rv=/usr/include
			      ],[psm3_rv_check=0])
			psm3_rv_old_header=0
			save_CPPFLAGS=$CPPFLAGS
			CPPFLAGS="$CPPFLAGS -I$with_psm3_rv"
			dnl Check for /usr/include/rdma/rv_user_ioctls.h first
			_FI_CHECK_PACKAGE_HEADER([psm3_rv],
			                         [rdma/rv_user_ioctls.h],
			                         [],
			                         [psm3_rv_happy=1],
			                         [psm3_rv_happy=0])

			AS_IF([test $psm3_rv_happy -eq 0], [
				AS_IF([test "$psm3_rv_check" -eq 1],
				      [with_psm3_rv=/usr/include/uapi])
				CPPFLAGS="$save_CPPFLAGS -I$with_psm3_rv"
				_FI_CHECK_PACKAGE_HEADER([psm3_rv],
				                         [rv/rv_user_ioctls.h],
				                         [],
				                         [psm3_rv_happy=1
				                          psm3_rv_old_header=1],
				                         [psm3_rv_happy=0])
			      ])
			CPPFLAGS=$save_CPPFLAGS
			AS_IF([test "$psm3_rv_happy" -eq 0],
			      [
				AS_IF([test "$psm3_rv_check" -eq 0], [
					psm3_happy=0
					AC_MSG_ERROR([RV Module headers requested but rv_user_ioctls.h not found.])
				])
				psm3_CPPFLAGS="$psm3_CPPFLAGS -URNDV_MOD"
			      ],[
				psm3_CPPFLAGS="$psm3_CPPFLAGS -DRNDV_MOD -I$with_psm3_rv"
				AS_IF([test "$psm3_rv_old_header" -eq 1],
				      [psm3_CPPFLAGS="$psm3_CPPFLAGS -DHAVE_OLD_RV_HEADER"])
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
		AS_IF([test $psm3_happy -eq 1], [
			AC_CONFIG_FILES([prov/psm3/psm3/psm2_hal_inlines_i.h \
		                 prov/psm3/psm3/psm2_hal_inlines_d.h \
		                 prov/psm3/src/psm3_revision.c])
		])
	       ],[psm3_happy=0])

	 AS_IF([test $psm3_happy -eq 1], [$1], [$2])

	 psm3_CFLAGS="$PSM3_ARCH_CFLAGS"
	 psm3_CPPFLAGS="$psm3_CPPFLAGS $psm3_rt_CPPFLAGS $psm3_dl_CPPFLAGS $psm3_numa_CPPFLAGS $psm3_ibv_CPPFLAGS"
	 psm3_LDFLAGS="$psm3_LDFLAGS $psm3_rt_LDFLAGS $psm3_dl_LDFLAGS $psm3_numa_LDFLAGS $psm3_ibv_LDFLAGS"
	 psm3_LIBS="$psm3_LIBS $psm3_rt_LIBS $psm3_dl_LIBS $psm3_numa_LIBS $psm3_ibv_LIBS"
	 AC_SUBST(psm3_CFLAGS)
	 AC_SUBST(psm3_CPPFLAGS)
	 AC_SUBST(psm3_LDFLAGS)
	 AC_SUBST(psm3_LIBS)
	 AC_SUBST(PSM_HAL_CNT)
	 AC_SUBST(PSM_HAL_INST)

])

AC_ARG_WITH([psm3-rv],
            [AS_HELP_STRING([--with-psm3-rv],
                            [Enable RV module use @<:@default=check@:>@])])
