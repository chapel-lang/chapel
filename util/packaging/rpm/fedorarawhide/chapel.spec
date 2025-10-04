Name:           chapel
Version:        2.6.0
%global shortversion 2.6
Release:        %{autorelease}
Summary:        A Productive Parallel Programming Language

# Most code is under Apache-2.0
# Files under both Apache-2.0 and MIT:
#  compiler/adt/vec.cpp
#  compiler/backend/beautify.cpp
#  compiler/include/arg.h
#  compiler/include/beautify.h
#  compiler/include/list.h
#  compiler/include/map.h
#  compiler/include/vec.h
#  compiler/main/arg.cpp
# Bison-generated files are licensed GPL-3.0-or-later WITH Bison-exception-2.2:
#  frontend/lib/parsing/bison-chpl-lib.cpp
#  frontend/lib/parsing/bison-chpl-lib.h

License:        Apache-2.0 AND (GPL-3.0-or-later WITH Bison-exception-2.2) AND (Apache-2.0 AND MIT)
URL:            https://chapel-lang.org
Source:         https://github.com/chapel-lang/chapel/archive/%{version}/chapel-%{version}.tar.gz
# Unsupported architecture
ExcludeArch:    %{ix86}

BuildRequires:  bash
BuildRequires:  chrpath
BuildRequires:  clang20-devel
BuildRequires:  cmake
BuildRequires:  diffutils
BuildRequires:  gawk
BuildRequires:  git-core
BuildRequires:  gcc
BuildRequires:  gcc-c++
BuildRequires:  gmp-devel
BuildRequires:  hwloc-devel
# Full system support on host requires bundling
#BuildRequires:  jemalloc-devel
BuildRequires:  libfabric-devel
BuildRequires:  libunwind-devel
BuildRequires:  llvm20-devel
BuildRequires:  m4
BuildRequires:  make
BuildRequires:  mimalloc-devel
BuildRequires:  pmix-devel
BuildRequires:  perl
BuildRequires:  python3-devel
# Modified version is bundled
#BuildRequires:  re2-devel
BuildRequires:  stb_image-devel
BuildRequires:  stb_image_write-devel
BuildRequires:  which
# Not available on fedora rawhide
#BuildRequires:  whereami
# Documentation
BuildRequires:  python3dist(argcomplete)
BuildRequires:  python3dist(babel)
BuildRequires:  python3dist(breathe)
BuildRequires:  python3dist(docutils)
BuildRequires:  python3dist(filelock)
BuildRequires:  python3dist(jinja2)
BuildRequires:  python3dist(junitparser)
BuildRequires:  python3dist(markupsafe)
BuildRequires:  python3dist(pycparser)
BuildRequires:  python3dist(pycparserext)
BuildRequires:  python3dist(pygments)
BuildRequires:  python3dist(pytest)
BuildRequires:  python3dist(pytest-lsp)
BuildRequires:  python3dist(pyyaml)
# Not yet packaged
#BuildRequires:  python3dist(scspell3k)
BuildRequires:  python3dist(setuptools)
BuildRequires:  python3dist(sphinx)
BuildRequires:  python3dist(sphinx-rtd-theme)
BuildRequires:  python3dist(sphinxcontrib-chapeldomain)
BuildRequires:  python3dist(urllib3)
Requires: stb_image-devel
Requires: stb_image_write-devel
#Requires:  whereami
Provides: bundled(re2)

%description
Chapel is a modern programming language designed for productive parallel
computing at scale. Chapel's design and implementation have been undertaken
with portability in mind, permitting Chapel to run on multicore desktops and
laptops, commodity clusters, and the cloud, in addition to the high-end
supercomputers for which it was originally undertaken.

%prep
%autosetup -p1

# Use package in Fedora
rm modules/packages/ImageHelper/stb/README
rm modules/packages/ImageHelper/stb/stb_image.h
rm modules/packages/ImageHelper/stb/stb_image_write.h
ln -s %{_includedir}/stb/stb_image.h modules/packages/ImageHelper/stb/stb_image.h
ln -s %{_includedir}/stb/stb_image_write.h modules/packages/ImageHelper/stb/stb_image_write.h

%build
export CHPL_HOST_COMPILER=gnu
export CHPL_TARGET_COMPILER=gnu
export CHPL_TARGET_CPU=unknown
export CHPL_LOCALE_MODEL=flat
export CHPL_TASKS=fifo
# gasnet not yet packaged
export CHPL_COMM=none
export CHPL_TARGET_MEM=cstdlib
#export CHPL_TARGET_JEMALLOC=system
export CHPL_HOST_MEM=cstdlib
#export CHPL_HOST_MIMALLOC=system
#export CHPL_HOST_JEMALLOC=system
#export CHPL_NETWORK_ATOMICS=ofi
export CHPL_GMP=system
export CHPL_HWLOC=system
# No option to use system version, Chapel
# specific changes not pushed upstream
export CHPL_RE2=bundled
export CHPL_AUX_FILESYS=none
export CHPL_LLVM=system
export CHPL_LLVM_SUPPORT=system
export CHPL_UNWIND=system
export CHPL_LIB_PIC=pic
./configure --prefix=%{_prefix}
%make_build


%install
export CHPL_HOST_COMPILER=gnu
export CHPL_TARGET_COMPILER=gnu
export CHPL_TARGET_CPU=unknown
export CHPL_LOCALE_MODEL=flat
export CHPL_TASKS=fifo
# gasnet not yet packaged
#export CHPL_COMM=none
export CHPL_TARGET_MEM=cstdlib
#export CHPL_HOST_MIMALLOC=system
#export CHPL_TARGET_JEMALLOC=system
export CHPL_HOST_MEM=cstdlib
#export CHPL_HOST_JEMALLOC=system
#export CHPL_NETWORK_ATOMICS=ofi
export CHPL_GMP=system
export CHPL_HWLOC=system
# No option to use system version, Chapel
# specific changes not pushed upstream
export CHPL_RE2=bundled
export CHPL_AUX_FILESYS=none
export CHPL_LLVM=system
export CHPL_LLVM_SUPPORT=system
export CHPL_UNWIND=system
export CHPL_LIB_PIC=pic
%make_install
mkdir -p  %{buildroot}%{_libdir}
mv %{buildroot}%{_prefix}/lib/chapel %{buildroot}%{_libdir}/chapel
mkdir -p %{buildroot}%{_libdir}/cmake
mv %{buildroot}%{_prefix}/lib/cmake/chpl %{buildroot}%{_libdir}/cmake/chpl
%ifarch ppc64le
chrpath --delete %{buildroot}%{_libdir}/chapel/%{shortversion}/runtime/lib/compiler/linux_ppc_le64-%{_arch}/libChplFrontendShared.so
%elifarch s390x
chrpath --delete %{buildroot}%{_libdir}/chapel/%{shortversion}/runtime/lib/compiler/linux32-%{_arch}/libChplFrontendShared.so
%else
chrpath --delete %{buildroot}%{_libdir}/chapel/%{shortversion}/runtime/lib/compiler/linux64-%{_arch}/libChplFrontendShared.so
%endif
# Remove development files
rm %{buildroot}%{_libdir}/chapel/%{shortversion}/runtime/include/.gitignore
rm %{buildroot}%{_datadir}/chapel/%{shortversion}/make/.gitignore
rm %{buildroot}%{_datadir}/chapel/%{shortversion}//modules/.gitignore
rm %{buildroot}%{_datadir}/chapel/%{shortversion}//third-party/chpl-venv/chpldoc-sphinx-project/.gitignore
rm %{buildroot}%{_datadir}/chapel/%{shortversion}/util/chplenv/.gitignore
%ifarch ppc64le
rm %{buildroot}%{_libdir}/chapel/%{shortversion}/third-party/re2/install/linux_ppc_le64-%{_arch}-unknown-gnu-pic/lib/pkgconfig/re2.pc
rm -r %{buildroot}%{_libdir}/chapel/%{shortversion}/third-party/re2/install/linux_ppc_le64-%{_arch}-unknown-gnu-pic/lib/pkgconfig/
%elifarch s390x
rm %{buildroot}%{_libdir}/chapel/%{shortversion}/third-party/re2/install/linux32-%{_arch}-unknown-gnu-pic/lib/pkgconfig/re2.pc
rm -r %{buildroot}%{_libdir}/chapel/%{shortversion}/third-party/re2/install/linux32-%{_arch}-unknown-gnu-pic/lib/pkgconfig/
%else
rm %{buildroot}%{_libdir}/chapel/%{shortversion}/third-party/re2/install/linux64-%{_arch}-unknown-gnu-pic/lib/pkgconfig/re2.pc
rm -r %{buildroot}%{_libdir}/chapel/%{shortversion}/third-party/re2/install/linux64-%{_arch}-unknown-gnu-pic/lib/pkgconfig/
%endif

%py3_shebang_fix %{buildroot}%{_datadir}/chapel/%{shortversion}/util/chplenv/chpl_lib_pic.py
%py3_shebang_fix %{buildroot}%{_datadir}/chapel/%{shortversion}/util/chplenv/chpl_mimalloc.py

# Add executable permissions
chmod +x %{buildroot}%{_datadir}/chapel/%{shortversion}/util/chplenv/chpl_lib_pic.py
chmod +x %{buildroot}%{_datadir}/chapel/%{shortversion}/util/chplenv/chpl_mimalloc.py
chmod +x %{buildroot}%{_datadir}/chapel/%{shortversion}/util/config/run-in-venv-common.bash

rm %{buildroot}%{_datadir}/chapel/%{shortversion}/modules/packages/ImageHelper/stb/stb_image.h
rm %{buildroot}%{_datadir}/chapel/%{shortversion}/modules/packages/ImageHelper/stb/stb_image_write.h
ln -s %{_includedir}/stb/stb_image.h \
  %{buildroot}%{_datadir}/chapel/%{shortversion}/modules/packages/ImageHelper/stb/stb_image.h
ln -s %{_includedir}/stb/stb_image_write.h \
  %{buildroot}%{_datadir}/chapel/%{shortversion}/modules/packages/ImageHelper/stb/stb_image_write.h

%check
export CHPL_HOST_COMPILER=gnu
export CHPL_TARGET_COMPILER=gnu
export CHPL_TARGET_CPU=unknown
export CHPL_LOCALE_MODEL=flat
export CHPL_TASKS=fifo
# gasnet not yet packaged
export CHPL_COMM=none
export CHPL_TARGET_MEM=cstdlib
#export CHPL_TARGET_JEMALLOC=system
export CHPL_HOST_MEM=cstdlib
#export CHPL_HOST_MIMALLOC=system
#export CHPL_HOST_JEMALLOC=system
#export CHPL_NETWORK_ATOMICS=ofi
export CHPL_GMP=system
export CHPL_HWLOC=system
# No option to use system version, Chapel
# specific changes not pushed upstream
export CHPL_RE2=bundled
export CHPL_AUX_FILESYS=none
export CHPL_LLVM=system
export CHPL_LLVM_SUPPORT=system
export CHPL_UNWIND=system
export CHPL_LIB_PIC=pic
# On ppc64le and s390x check fails with
# /usr/include/bits/string_fortified.h:29:10: warning: ‘__builtin___memcpy_chk’ specified bound 18446744073709551584 exceeds maximum object size 9223372036854775807 [-Wstringop-overflow=]
%ifnarch s390x ppc64le
LD_LIBRARY_PATH=%{buildroot}/%{_libdir}/chapel/%{shortversion}/compiler/:${LD_LIBRARY_PATH} \
PATH=%{buildroot}/%{_bindir}:${PATH} \
make check
%endif

%files
%license LICENSE
%license LICENSE.chapel
%{_bindir}/chpl
%{_datadir}/chapel/
%{_libdir}/chapel/
%{_libdir}/cmake/chpl

%changelog
%autochangelog 
