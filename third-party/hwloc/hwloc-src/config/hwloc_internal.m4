dnl -*- Autoconf -*-
dnl
dnl Copyright © 2010-2024 Inria.  All rights reserved.
dnl Copyright © 2009, 2011 Université Bordeaux
dnl Copyright © 2004-2005 The Trustees of Indiana University and Indiana
dnl                         University Research and Technology
dnl                         Corporation.  All rights reserved.
dnl Copyright © 2004-2005 The Regents of the University of California.
dnl                         All rights reserved.
dnl Copyright © 2004-2008 High Performance Computing Center Stuttgart,
dnl                         University of Stuttgart.  All rights reserved.
dnl Copyright © 2006-2014 Cisco Systems, Inc.  All rights reserved.
dnl
dnl See COPYING in top-level directory.

#-----------------------------------------------------------------------

# Probably only ever invoked by hwloc's configure.ac
AC_DEFUN([HWLOC_BUILD_STANDALONE],[
    hwloc_mode=standalone
])dnl

#-----------------------------------------------------------------------

# Probably only ever invoked by hwloc's configure.ac
AC_DEFUN([HWLOC_DEFINE_ARGS],[
    # Embedded mode, or standalone?
    AC_ARG_ENABLE([embedded-mode],
                    AS_HELP_STRING([--enable-embedded-mode],
                                   [Using --enable-embedded-mode puts the HWLOC into "embedded" mode.  The default is --disable-embedded-mode, meaning that the HWLOC is in "standalone" mode.]))

    # Change the symbol prefix?
    AC_ARG_WITH([hwloc-symbol-prefix],
                AS_HELP_STRING([--with-hwloc-symbol-prefix=STRING],
                               [STRING can be any valid C symbol name.  It will be prefixed to all public HWLOC symbols.  Default: "" (no prefix)]))

    # For the windows build
    AC_ARG_VAR([HWLOC_MS_LIB], [Path to Microsoft's Visual Studio `lib' tool])

    # Debug mode?
    AC_ARG_ENABLE([debug],
                  AS_HELP_STRING([--enable-debug],
                                 [Using --enable-debug enables various hwloc maintainer-level debugging controls.  This option is not recomended for end users.]))

    # Doxygen?
    AC_ARG_ENABLE([doxygen],
        [AS_HELP_STRING([--enable-doxygen],
                        [enable support for building Doxygen documentation (note that this option is ONLY relevant in developer builds; Doxygen documentation is pre-built for tarball builds and this option is therefore ignored)])])

    # Building the README
    AC_ARG_ENABLE([readme],
        [AS_HELP_STRING([--disable-readme],
                        [disable the updating of the top-level README file from the HTML documentation index])])

    # Picky?
    AC_ARG_ENABLE(picky,
                  AS_HELP_STRING([--disable-picky],
                                 [When in developer checkouts of hwloc and compiling with gcc, the default is to enable maximum compiler pickyness.  Using --disable-picky or --enable-picky overrides any default setting]))

    # Cairo?
    AC_ARG_ENABLE([cairo],
                  AS_HELP_STRING([--disable-cairo],
                                 [Disable the Cairo back-end of hwloc's lstopo command]))

    # CPUID
    AC_ARG_ENABLE([cpuid],
		  AS_HELP_STRING([--disable-cpuid],
				 [Disable the cpuid-based architecture specific support (x86 component)]))

    # XML using libxml2?
    AC_ARG_ENABLE([libxml2],
                  AS_HELP_STRING([--disable-libxml2],
		                 [Do not use libxml2 for XML support, use a custom minimalistic support]))

    # I/O?
    AC_ARG_ENABLE([io],
                  AS_HELP_STRING([--disable-io],
                                 [Disable I/O discovery build entirely (PCI, LinuxIO, CUDA, OpenCL, NVML, RSMI, LevelZero, GL) instead of only disabling it at runtime by default]))

    # PCI?
    AC_ARG_ENABLE([pci],
                  AS_HELP_STRING([--disable-pci],
                                 [Disable the PCI device discovery build (instead of only disabling PCI at runtime by default)]))
    # 32bits_pci_domain?
    AC_ARG_ENABLE([32bits-pci-domain],
                  AS_HELP_STRING([--enable-32bits-pci-domain],
                                 [Enable 32 bits PCI domains (domains > 16bits are ignored by default). WARNING: This breaks the library ABI, don't enable unless really needed.]))

    # OpenCL?
    AC_ARG_ENABLE([opencl],
                  AS_HELP_STRING([--disable-opencl],
                                 [Disable the OpenCL device discovery build (instead of only disabling OpenCL at runtime by default)]))

    # CUDA?
    AC_ARG_ENABLE([cuda],
                  AS_HELP_STRING([--disable-cuda],
                                 [Disable the CUDA device discovery build using libcudart (instead of only disabling CUDA at runtime by default)]))

    # NVML?
    AC_ARG_ENABLE([nvml],
                  AS_HELP_STRING([--disable-nvml],
                                 [Disable the NVML device discovery build (instead of only disabling NVML at runtime by default)]))

    # CUDA version (for using its pkg-config cuda-x.y.pc)
    AC_ARG_WITH([cuda-version],
                AS_HELP_STRING([--with-cuda-version=<version>],
		               [Specify the CUDA version (e.g. 11.2) for selecting the appropriate pkg-config file]))
    AC_ARG_VAR([CUDA_VERSION], [The CUDA version (e.g. 11.2) for selecting the appropriate pkg-config file])
    # CUDA install path (and NVML and OpenCL)
    AC_ARG_WITH([cuda],
                AS_HELP_STRING([--with-cuda=<dir>],
                               [Specify the CUDA installation directory, used for NVIDIA NVML and OpenCL too. If a non-existent directory is given, all dependencies installed by CUDA are disabled (CUDA, NVML and NVIDIA OpenCL).]))

    # RSMI?
    AC_ARG_ENABLE([rsmi],
                  AS_HELP_STRING([--disable-rsmi],
                                 [Disable the ROCm SMI device discovery]))
    # ROCm version (for selecting /opt/rocm-x.y/)
    AC_ARG_WITH([rocm-version],
                AS_HELP_STRING([--with-rocm-version=<version>],
		               [Specify the ROCm version (e.g. 4.2.0) for selecting the default ROCm installation path (e.g. /opt/rocm-4.2.0)]))
    AC_ARG_VAR([ROCM_VERSION], [The ROCm version (e.g. 4.2.0) for selecting the default ROCm installation path (e.g. /opt/rocm-4.2.0)])
    # ROCm install path
    AC_ARG_WITH([rocm],
                AS_HELP_STRING([--with-rocm=<dir>],
		               [Specify the ROCm installation directory]))

    # LevelZero
    AC_ARG_ENABLE([levelzero],
                  AS_HELP_STRING([--disable-levelzero],
                                 [Disable the oneAPI Level Zero device discovery]))

    # GL/Display
    AC_ARG_ENABLE([gl],
		  AS_HELP_STRING([--disable-gl],
				 [Disable the GL display device discovery (instead of only disabling GL at runtime by default)]))

    # LibUdev
    AC_ARG_ENABLE([libudev],
                  AS_HELP_STRING([--disable-libudev],
                                 [Disable the Linux libudev]))

    # Plugins
    AC_ARG_ENABLE([plugins],
                  AS_HELP_STRING([--enable-plugins=name,...],
                                 [Build the given components as dynamically-loaded plugins]))

    AC_ARG_WITH([hwloc-plugins-path],
		AS_HELP_STRING([--with-hwloc-plugins-path=dir:...],
                               [Colon-separated list of plugin directories. Default: "$prefix/lib/hwloc". Plugins will be installed in the first directory. They will be loaded from all of them, in order.]),
		[HWLOC_PLUGINS_PATH="$with_hwloc_plugins_path"],
		[HWLOC_PLUGINS_PATH="\$(libdir)/hwloc"])

    # Look for dlopen
    # Not --disable-dlopen because $enable_dlopen is already used/set
    AC_ARG_ENABLE([plugin-dlopen],
                  AS_HELP_STRING([--disable-plugin-dlopen],
                                 [Do not use dlopen for loading plugins.]))
    # Look for ltdl
    # Not --disable-ltdl for consistency wrt dlopen above
    AC_ARG_ENABLE([plugin-ltdl],
                  AS_HELP_STRING([--disable-plugin-ltdl],
                                 [Do not use ltdl for loading plugins.]))

])dnl

#-----------------------------------------------------------------------

dnl We only build documentation if this is a developer checkout.
dnl Distribution tarballs just install pre-built documentation that was
dnl included in the tarball.

# Probably only ever invoked by hwloc's configure.ac
AC_DEFUN([HWLOC_SETUP_DOCS],[
    cat <<EOF

###
### Configuring hwloc documentation
###
EOF

    AC_MSG_CHECKING([if this is a developer build])
    AS_IF([test ! -e "$srcdir/.git"],
          [AC_MSG_RESULT([no (doxygen generation is optional)])
	   test "x$enable_doxygen" = x && enable_doxygen=no],
          [AC_MSG_RESULT([yes])
	   test "x$enable_doxygen" = x && enable_doxygen=yes])

    # Generating the doxygen output requires a few tools.  If we
    # don't have all of them, refuse the build the docs.
    AC_ARG_VAR([DOXYGEN], [Location of the doxygen program (required for building the hwloc doxygen documentation)])
    AC_PATH_TOOL([DOXYGEN], [doxygen])
    HWLOC_DOXYGEN_VERSION=`doxygen --version 2> /dev/null`

    AC_ARG_VAR([PDFLATEX], [Location of the pdflatex program (required for building the hwloc doxygen documentation)])
    AC_PATH_TOOL([PDFLATEX], [pdflatex])

    AC_ARG_VAR([MAKEINDEX], [Location of the makeindex program (required for building the hwloc doxygen documentation)])
    AC_PATH_TOOL([MAKEINDEX], [makeindex])

    AC_ARG_VAR([FIG2DEV], [Location of the fig2dev program (required for building the hwloc doxygen documentation)])
    AC_PATH_TOOL([FIG2DEV], [fig2dev])

    AC_ARG_VAR([GS], [Location of the gs program (required for building the hwloc doxygen documentation)])
    AC_PATH_TOOL([GS], [gs])

    AC_ARG_VAR([EPSTOPDF], [Location of the epstopdf program (required for building the hwloc doxygen documentation)])
    AC_PATH_TOOL([EPSTOPDF], [epstopdf])

    AC_MSG_CHECKING([if can build doxygen docs])
    AS_IF([test "x$DOXYGEN" != "x" -a "x$PDFLATEX" != "x" -a "x$MAKEINDEX" != "x" -a "x$FIG2DEV" != "x" -a "x$GS" != "x" -a "x$EPSTOPDF" != "x"],
                 [hwloc_generate_doxs=yes], [hwloc_generate_doxs=no])
    AC_MSG_RESULT([$hwloc_generate_doxs])
    AS_IF([test "x$hwloc_generate_doxs" = xyes -a "x$HWLOC_DOXYGEN_VERSION" = x1.6.2],
                 [hwloc_generate_doxs="no"; AC_MSG_WARN([doxygen 1.6.2 has broken short name support, disabling])])
    AS_IF([test "x$hwloc_generate_doxs" = xyes -a "x$HWLOC_DOXYGEN_VERSION" = x1.8.16 -a "$HWLOC_top_builddir" = "$HWLOC_top_srcdir"],
                 [hwloc_generate_doxs="no"; AC_MSG_WARN([doxygen 1.8.16 fails when building inside the source-tree, disabling])])

    AC_REQUIRE([AC_PROG_SED])

    AS_IF([test "x$enable_readme" != xno], [
      # Making the top-level README requires w3m or lynx.
      AC_ARG_VAR([W3M], [Location of the w3m program (required to building the top-level hwloc README file)])
      AC_PATH_TOOL([W3M], [w3m])
      AC_ARG_VAR([LYNX], [Location of the lynx program (required to building the top-level hwloc README file)])
      AC_PATH_TOOL([LYNX], [lynx])

      AC_MSG_CHECKING([if can build top-level README])
      AS_IF([test "x$W3M" != "x"],
            [hwloc_generate_readme=yes
             HWLOC_W3_GENERATOR=$W3M],
            [AS_IF([test "x$LYNX" != "x"],
                   [hwloc_generate_readme=yes
                    HWLOC_W3_GENERATOR="$LYNX -dump -nolist"],
                   [hwloc_generate_readme=no])])
      AC_SUBST(HWLOC_W3_GENERATOR)
      AC_MSG_RESULT([$hwloc_generate_readme])
    ], [
      hwloc_generate_readme=no
    ])
    AC_MSG_CHECKING([if will build top-level README])
    AC_MSG_RESULT([$hwloc_generate_readme])

    # If any one of the above tools is missing, we will refuse to make dist.
    AC_MSG_CHECKING([if will build doxygen docs])
    AS_IF([test "x$hwloc_generate_doxs" = "xyes" -a "x$enable_doxygen" != "xno"],
          [], [hwloc_generate_doxs=no])
    AC_MSG_RESULT([$hwloc_generate_doxs])

    # See if we want to install the doxygen docs
    AC_MSG_CHECKING([if will install doxygen docs])
    AS_IF([test "x$hwloc_generate_doxs" = "xyes" -o \
	    -f "$srcdir/doc/doxygen-doc/man/man3/hwloc_distrib.3" -a \
    	    -f "$srcdir/doc/doxygen-doc/hwloc-a4.pdf" -a \
    	    -f "$srcdir/doc/doxygen-doc/hwloc-letter.pdf"],
          [hwloc_install_doxs=yes],
          [hwloc_install_doxs=no])
    AC_MSG_RESULT([$hwloc_install_doxs])

    # Generate some files for the docs
    AC_CONFIG_FILES(
        hwloc_config_prefix[doc/Makefile]
        hwloc_config_prefix[doc/examples/Makefile]
        hwloc_config_prefix[doc/doxygen-config.cfg])
])

#-----------------------------------------------------------------------

# Probably only ever invoked by hwloc's configure.ac
AC_DEFUN([HWLOC_SETUP_UTILS],[
    cat <<EOF

###
### Configuring hwloc command line utilities
###
EOF

    AC_REQUIRE([AC_PROG_SED])

    # runstatedir only supported in autoconf >= 2.70 and in some backports
    if test "x${runstatedir}" != "x"; then
      HWLOC_runstatedir=${runstatedir}
    else
      HWLOC_runstatedir='${localstatedir}/run'
    fi
    AC_SUBST([HWLOC_runstatedir])

    # X11 support
    AC_PATH_XTRA

    CPPFLAGS_save=$CPPFLAGS
    LIBS_save=$LIBS

    CPPFLAGS="$CPPFLAGS $X_CFLAGS"
    LIBS="$LIBS $X_PRE_LIBS $X_LIBS $X_EXTRA_LIBS"
    AC_CHECK_HEADERS([X11/Xlib.h],
        [AC_CHECK_LIB([X11], [XOpenDisplay],
            [ AC_CHECK_HEADERS([X11/Xutil.h],
                [AC_CHECK_HEADERS([X11/keysym.h],
                    [AC_DEFINE([HWLOC_HAVE_X11_KEYSYM], [1], [Define to 1 if X11 headers including Xutil.h and keysym.h are available.])
                     hwloc_x11_keysym_happy=yes
                     HWLOC_X11_CPPFLAGS="$X_CFLAGS"
                     AC_SUBST([HWLOC_X11_CPPFLAGS])
                     HWLOC_X11_LIBS="$X_PRE_LIBS $X_LIBS -lX11 $X_EXTRA_LIBS"
                     AC_SUBST([HWLOC_X11_LIBS])])
                ], [], [#include <X11/Xlib.h>])
            ])
         ])
    CPPFLAGS=$CPPFLAGS_save
    LIBS=$LIBS_save

    # Cairo support
    hwloc_cairo_happy=no
    if test "x$enable_cairo" != "xno"; then
      HWLOC_PKG_CHECK_MODULES([CAIRO], [cairo], [cairo_fill], [cairo.h],
                              [hwloc_cairo_happy=yes],
                              [hwloc_cairo_happy=no])
    fi

    if test "x$hwloc_cairo_happy" = "xyes"; then
        AC_DEFINE([HWLOC_HAVE_CAIRO], [1], [Define to 1 if you have the `cairo' library.])
        AC_MSG_CHECKING([whether lstopo Cairo/X11 interactive graphical output is supported])
        if test "x$hwloc_x11_keysym_happy" = xyes; then
          save_CPPFLAGS="$CPPFLAGS"
          CPPFLAGS="$CPPFLAGS $HWLOC_CAIRO_CFLAGS $HWLOC_X11_CPPFLAGS"
          AC_PREPROC_IFELSE([
            AC_LANG_PROGRAM([[
              #include <cairo.h>
            ]], [[
              #ifndef CAIRO_HAS_XLIB_SURFACE
              #error
              #endif
            ]])
          ], [
            AC_MSG_RESULT([yes])
            lstopo_have_x11=yes
            AC_DEFINE([LSTOPO_HAVE_X11], 1, [Define if lstopo Cairo/X11 interactive graphical output is supported])
          ], [
            AC_MSG_RESULT([no (missing CAIRO_HAS_XLIB_SURFACE)])
          ])
          CPPFLAGS="$save_CPPFLAGS"
        else
          AC_MSG_RESULT([no (missing X11)])
        fi
    else
        AS_IF([test "$enable_cairo" = "yes"],
              [AC_MSG_WARN([--enable-cairo requested, but Cairo/X11 support was not found])
               AC_MSG_ERROR([Cannot continue])])
    fi

    AC_CHECK_TYPES([wchar_t], [
      AC_CHECK_FUNCS([putwc])
    ], [], [[#include <wchar.h>]])

    HWLOC_XML_LOCALIZED=1
    AC_CHECK_HEADERS([locale.h xlocale.h], [
      AC_CHECK_FUNCS([setlocale])
      AC_CHECK_FUNCS([uselocale], [HWLOC_XML_LOCALIZED=0])
    ])
    AC_SUBST([HWLOC_XML_LOCALIZED])
    AC_CHECK_HEADERS([langinfo.h], [
      AC_CHECK_FUNCS([nl_langinfo])
    ])
    hwloc_old_LIBS="$LIBS"
    chosen_curses=""
    for curses in ncurses curses
    do
      for lib in "" -l${curses}w -l$curses -ltinfo -ltermcap
      do
        AC_MSG_CHECKING(termcap support using $curses and $lib)
        LIBS="$hwloc_old_LIBS $lib"
        AC_LINK_IFELSE([AC_LANG_PROGRAM([[
#include <$curses.h>
#include <term.h>
]], [[tparm(NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0)]])], [
          AC_MSG_RESULT(yes)
          AC_SUBST([HWLOC_TERMCAP_LIBS], ["$LIBS"])
          AC_DEFINE([HWLOC_HAVE_LIBTERMCAP], [1],
                    [Define to 1 if you have a library providing the termcap interface])
          chosen_curses=$curses
        ], [
          AC_MSG_RESULT(no)
        ])
        test "x$chosen_curses" != "x" && break
      done
      test "x$chosen_curses" != "x" && break
    done
    if test "$chosen_curses" = ncurses
    then
      AC_DEFINE([HWLOC_USE_NCURSES], [1], [Define to 1 if ncurses works, preferred over curses])
    fi
    LIBS="$hwloc_old_LIBS"
    unset hwloc_old_LIBS

    _HWLOC_CHECK_DIFF_U
    _HWLOC_CHECK_DIFF_W

    # Solaris needs -lsocket for socket/bind/... in hwloc-ps
    AC_CHECK_DECLS([bind], [
      AC_CHECK_LIB([socket], [bind],
                   [need_libsocket=yes])
    ], [], [[#include <sys/socket.h>]])
    if test x$need_libsocket = xyes; then
      # keep -lsocket first in case there's also -lnsl which cannot be before -lsocket
      HWLOC_PS_LIBS="-lsocket $HWLOC_PS_LIBS"
    fi
    AC_SUBST(HWLOC_PS_LIBS)

    AC_CHECK_FUNCS([isatty])
    AC_CHECK_FUNCS([tcgetpgrp])

    AC_CHECK_HEADERS([time.h], [
      AC_CHECK_FUNCS([clock_gettime])
    ])

    # Only generate this if we're building the utilities
    AC_CONFIG_FILES(
        hwloc_config_prefix[utils/Makefile]
        hwloc_config_prefix[utils/hwloc/Makefile]
        hwloc_config_prefix[utils/lstopo/Makefile]
        hwloc_config_prefix[hwloc.pc]
   )
])dnl

#-----------------------------------------------------------------------

# Probably only ever invoked by hwloc's configure.ac
AC_DEFUN([HWLOC_SETUP_TESTS],[
    cat <<EOF

###
### Configuring tests
###
EOF

    AC_CHECK_LIB([pthread], [pthread_self], [hwloc_have_pthread=yes])

    HWLOC_PKG_CHECK_MODULES([NUMA], [numa], [numa_available], [numa.h],
                            [hwloc_have_linux_libnuma=yes],
			    [
			     # libnuma didn't have a .pc before 2.0.12, look for it manually.
			     AC_CHECK_LIB([numa], [numa_available], [
				# and make sure this old release has at least numa_bitmask_alloc() for our tests
			        AC_CHECK_DECL([numa_bitmask_alloc],
				              [hwloc_have_linux_libnuma=yes
					       HWLOC_NUMA_LIBS=-lnuma
					      ],
					      [],
					      [#include <numa.h>])
			     ])])

    AC_CHECK_HEADERS([stdlib.h], [
      AC_CHECK_FUNCS([mkstemp])
    ])

    AC_CHECK_HEADERS([infiniband/verbs.h], [
      AC_CHECK_LIB([ibverbs], [ibv_open_device],
                   [AC_DEFINE([HAVE_LIBIBVERBS], 1, [Define to 1 if we have -libverbs])
                    hwloc_have_libibverbs=yes
		    HWLOC_IBVERBS_LIBS=-libverbs
		    AC_SUBST(HWLOC_IBVERBS_LIBS)
		   ])
    ])

    AC_PATH_PROG([BASH], [bash])

    AC_CHECK_PROGS(XMLLINT, [xmllint])

    AC_CHECK_PROGS(BUNZIPP, bunzip2, false)

    AC_MSG_CHECKING(if CXX works)
    AC_LANG_PUSH([C++])
    AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#include <iostream>
using namespace std;
int foo(void) {
  cout << "test" << endl;
  return 0;
}
	]])], [hwloc_have_cxx=yes], [hwloc_have_cxx=no])
    AC_LANG_POP([C++])
    AC_MSG_RESULT([$hwloc_have_cxx])

    _HWLOC_CHECK_DIFF_U

    # Only generate these files if we're making the tests
    AC_CONFIG_FILES(
        hwloc_config_prefix[tests/Makefile]
        hwloc_config_prefix[tests/hwloc/Makefile]
        hwloc_config_prefix[tests/hwloc/linux/Makefile]
        hwloc_config_prefix[tests/hwloc/linux/allowed/Makefile]
        hwloc_config_prefix[tests/hwloc/linux/gather/Makefile]
        hwloc_config_prefix[tests/hwloc/x86/Makefile]
        hwloc_config_prefix[tests/hwloc/x86+linux/Makefile]
        hwloc_config_prefix[tests/hwloc/xml/Makefile]
        hwloc_config_prefix[tests/hwloc/ports/Makefile]
        hwloc_config_prefix[tests/hwloc/rename/Makefile]
        hwloc_config_prefix[tests/hwloc/linux/allowed/test-topology.sh]
        hwloc_config_prefix[tests/hwloc/linux/gather/test-gather-topology.sh]
        hwloc_config_prefix[tests/hwloc/linux/test-topology.sh]
        hwloc_config_prefix[tests/hwloc/x86/test-topology.sh]
        hwloc_config_prefix[tests/hwloc/x86+linux/test-topology.sh]
        hwloc_config_prefix[tests/hwloc/xml/test-topology.sh]
        hwloc_config_prefix[tests/hwloc/wrapper.sh]
        hwloc_config_prefix[utils/hwloc/hwloc-compress-dir]
        hwloc_config_prefix[utils/hwloc/hwloc-gather-topology]
        hwloc_config_prefix[utils/hwloc/test-hwloc-annotate.sh]
        hwloc_config_prefix[utils/hwloc/test-hwloc-calc.sh]
        hwloc_config_prefix[utils/hwloc/test-hwloc-compress-dir.sh]
        hwloc_config_prefix[utils/hwloc/test-hwloc-diffpatch.sh]
        hwloc_config_prefix[utils/hwloc/test-hwloc-distrib.sh]
        hwloc_config_prefix[utils/hwloc/test-hwloc-info.sh]
        hwloc_config_prefix[utils/hwloc/test-build-custom-topology.sh]
        hwloc_config_prefix[utils/hwloc/test-fake-plugin.sh]
        hwloc_config_prefix[utils/hwloc/test-parsing-flags.sh]
        hwloc_config_prefix[utils/hwloc/test-hwloc-dump-hwdata/Makefile]
        hwloc_config_prefix[utils/hwloc/test-hwloc-dump-hwdata/test-hwloc-dump-hwdata.sh]
        hwloc_config_prefix[utils/lstopo/test-lstopo.sh]
        hwloc_config_prefix[utils/lstopo/test-lstopo-shmem.sh]
        hwloc_config_prefix[contrib/hwloc-ps.www/Makefile]
        hwloc_config_prefix[contrib/systemd/Makefile]
        hwloc_config_prefix[contrib/completion/Makefile]
        hwloc_config_prefix[contrib/completion/test-bash-completion.sh]
        hwloc_config_prefix[contrib/misc/Makefile]
        hwloc_config_prefix[contrib/windows/Makefile]
    )

    AC_CONFIG_COMMANDS([chmoding-scripts], [chmod +x] \
      hwloc_config_prefix[tests/hwloc/linux/test-topology.sh] \
      hwloc_config_prefix[tests/hwloc/x86/test-topology.sh] \
      hwloc_config_prefix[tests/hwloc/x86+linux/test-topology.sh] \
      hwloc_config_prefix[tests/hwloc/xml/test-topology.sh] \
      hwloc_config_prefix[tests/hwloc/linux/allowed/test-topology.sh] \
      hwloc_config_prefix[tests/hwloc/linux/gather/test-gather-topology.sh] \
      hwloc_config_prefix[tests/hwloc/wrapper.sh] \
      hwloc_config_prefix[utils/hwloc/hwloc-compress-dir] \
      hwloc_config_prefix[utils/hwloc/hwloc-gather-topology] \
      hwloc_config_prefix[utils/hwloc/test-hwloc-annotate.sh] \
      hwloc_config_prefix[utils/hwloc/test-hwloc-calc.sh] \
      hwloc_config_prefix[utils/hwloc/test-hwloc-compress-dir.sh] \
      hwloc_config_prefix[utils/hwloc/test-hwloc-diffpatch.sh] \
      hwloc_config_prefix[utils/hwloc/test-hwloc-distrib.sh] \
      hwloc_config_prefix[utils/hwloc/test-hwloc-info.sh] \
      hwloc_config_prefix[utils/hwloc/test-build-custom-topology.sh] \
      hwloc_config_prefix[utils/hwloc/test-fake-plugin.sh] \
      hwloc_config_prefix[utils/hwloc/test-parsing-flags.sh] \
      hwloc_config_prefix[utils/hwloc/test-hwloc-dump-hwdata/test-hwloc-dump-hwdata.sh] \
      hwloc_config_prefix[utils/lstopo/test-lstopo.sh] \
      hwloc_config_prefix[utils/lstopo/test-lstopo-shmem.sh] \
      hwloc_config_prefix[contrib/completion/test-bash-completion.sh])

    # These links are only needed in standalone mode.  It would
    # be nice to m4 foreach this somehow, but whenever I tried
    # it, I got obscure "invalid tag" errors from
    # AC_CONFIG_LINKS.  :-\ Since these tests are only run when
    # built in standalone mode, only generate them in
    # standalone mode.
    AC_CONFIG_LINKS(
	hwloc_config_prefix[tests/hwloc/ports/topology-solaris.c]:hwloc_config_prefix[hwloc/topology-solaris.c]
	hwloc_config_prefix[tests/hwloc/ports/topology-solaris-chiptype.c]:hwloc_config_prefix[hwloc/topology-solaris-chiptype.c]
	hwloc_config_prefix[tests/hwloc/ports/topology-aix.c]:hwloc_config_prefix[hwloc/topology-aix.c]
	hwloc_config_prefix[tests/hwloc/ports/topology-windows.c]:hwloc_config_prefix[hwloc/topology-windows.c]
	hwloc_config_prefix[tests/hwloc/ports/topology-darwin.c]:hwloc_config_prefix[hwloc/topology-darwin.c]
	hwloc_config_prefix[tests/hwloc/ports/topology-freebsd.c]:hwloc_config_prefix[hwloc/topology-freebsd.c]
	hwloc_config_prefix[tests/hwloc/ports/topology-netbsd.c]:hwloc_config_prefix[hwloc/topology-netbsd.c]
	hwloc_config_prefix[tests/hwloc/ports/topology-hpux.c]:hwloc_config_prefix[hwloc/topology-hpux.c]
	hwloc_config_prefix[tests/hwloc/ports/topology-bgq.c]:hwloc_config_prefix[hwloc/topology-bgq.c]
	hwloc_config_prefix[tests/hwloc/ports/topology-opencl.c]:hwloc_config_prefix[hwloc/topology-opencl.c]
	hwloc_config_prefix[tests/hwloc/ports/topology-cuda.c]:hwloc_config_prefix[hwloc/topology-cuda.c]
	hwloc_config_prefix[tests/hwloc/ports/topology-nvml.c]:hwloc_config_prefix[hwloc/topology-nvml.c]
	hwloc_config_prefix[tests/hwloc/ports/topology-rsmi.c]:hwloc_config_prefix[hwloc/topology-rsmi.c]
	hwloc_config_prefix[tests/hwloc/ports/topology-levelzero.c]:hwloc_config_prefix[hwloc/topology-levelzero.c]
	hwloc_config_prefix[tests/hwloc/ports/topology-gl.c]:hwloc_config_prefix[hwloc/topology-gl.c]
	hwloc_config_prefix[tests/hwloc/ports/lstopo-windows.c]:hwloc_config_prefix[utils/lstopo/lstopo-windows.c]
        hwloc_config_prefix[tests/hwloc/ports/lstopo-android.c]:hwloc_config_prefix[utils/lstopo/lstopo-android.c])
    ])
])dnl

#-----------------------------------------------------------------------

AC_DEFUN([_HWLOC_PROG_DIFF], [
  AC_ARG_VAR(DIFF, [diff tool])
  AC_PATH_PROG([DIFF], [diff])
])

AC_DEFUN([_HWLOC_CHECK_DIFF_U], [
  AC_REQUIRE([_HWLOC_PROG_DIFF])
  AC_MSG_CHECKING([whether diff accepts -u])
  if $DIFF -u /dev/null /dev/null 2> /dev/null
  then
    AC_MSG_RESULT([yes])
    HWLOC_DIFF_U="-u"
  else
    AC_MSG_RESULT([no])
    HWLOC_DIFF_U=""
  fi
  AC_SUBST([HWLOC_DIFF_U])
])

AC_DEFUN([_HWLOC_CHECK_DIFF_W], [
  AC_REQUIRE([_HWLOC_PROG_DIFF])
  AC_MSG_CHECKING([whether diff accepts -w])
  if $DIFF -w /dev/null /dev/null 2> /dev/null
  then
    AC_MSG_RESULT([yes])
    HWLOC_DIFF_W="-w"
  else
    AC_MSG_RESULT([no])
    HWLOC_DIFF_W=""
  fi
  AC_SUBST([HWLOC_DIFF_W])
])
