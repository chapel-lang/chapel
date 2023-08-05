if !HWLOC_HAVE_DARWIN
all-local: needed useless
else HWLOC_HAVE_DARWIN
# Do nothing on Darwin because their default compiler doesn't like our gcc/cpp lines
all-local:
endif HWLOC_HAVE_DARWIN

TEST_CPPFLAGS =
if HWLOC_HAVE_LINUX
TEST_CPPFLAGS += -DHWLOC_TEST_RENAME_LINUX=1
endif HWLOC_HAVE_LINUX
if HWLOC_HAVE_LINUX_LIBNUMA
TEST_CPPFLAGS += -DHWLOC_TEST_RENAME_LINUX_LIBNUMA=1
endif HWLOC_HAVE_LINUX_LIBNUMA
if HWLOC_HAVE_SCHED_SETAFFINITY
TEST_CPPFLAGS += -DHWLOC_TEST_RENAME_GLIBC_SCHED=1
endif HWLOC_HAVE_SCHED_SETAFFINITY
if HWLOC_HAVE_LIBIBVERBS
TEST_CPPFLAGS += -DHWLOC_TEST_RENAME_OPENFABRICS_VERBS=1
endif HWLOC_HAVE_LIBIBVERBS
if HWLOC_HAVE_OPENCL
TEST_CPPFLAGS += -DHWLOC_TEST_RENAME_OPENCL=1 $(HWLOC_OPENCL_CFLAGS) $(HWLOC_OPENCL_CPPFLAGS)
endif HWLOC_HAVE_OPENCL
if HWLOC_HAVE_CUDA
TEST_CPPFLAGS += -DHWLOC_TEST_RENAME_CUDA=1 $(HWLOC_CUDA_CFLAGS) $(HWLOC_CUDA_CPPFLAGS)
endif HWLOC_HAVE_CUDA
if HWLOC_HAVE_CUDART
TEST_CPPFLAGS += -DHWLOC_TEST_RENAME_CUDART=1 $(HWLOC_CUDART_CFLAGS) $(HWLOC_CUDART_CPPFLAGS)
endif HWLOC_HAVE_CUDART
if HWLOC_HAVE_NVML
TEST_CPPFLAGS += -DHWLOC_TEST_RENAME_NVML=1 $(HWLOC_NVML_CFLAGS) $(HWLOC_NVML_CPPFLAGS)
endif HWLOC_HAVE_NVML
if HWLOC_HAVE_RSMI
TEST_CPPFLAGS += -DHWLOC_TEST_RENAME_RSMI=1 $(HWLOC_RSMI_CFLAGS) $(HWLOC_RSMI_CPPFLAGS)
endif HWLOC_HAVE_RSMI
if HWLOC_HAVE_LEVELZERO
TEST_CPPFLAGS += -DHWLOC_TEST_RENAME_LEVELZERO=1 $(HWLOC_LEVELZERO_CFLAGS) $(HWLOC_LEVELZERO_CPPFLAGS)
endif HWLOC_HAVE_LEVELZERO

dirs:
	mkdir -p include/hwloc/autogen include/private/autogen

include/hwloc/autogen/config.h: dirs $(HWLOC_top_builddir)/include/hwloc/autogen/config.h
	sed	-e 's/^#define HWLOC_SYM_TRANSFORM 0/#define HWLOC_SYM_TRANSFORM 1/'                    \
		-e 's/^#define HWLOC_SYM_PREFIX hwloc_/#define HWLOC_SYM_PREFIX foobar_/'               \
		-e 's/^#define HWLOC_SYM_PREFIX_CAPS HWLOC_/#define HWLOC_SYM_PREFIX_CAPS FOOBAR_/'     \
		$(HWLOC_top_builddir)/include/hwloc/autogen/config.h > include/hwloc/autogen/config.h

include/private/autogen/config.h: dirs $(HWLOC_top_builddir)/include/private/autogen/config.h
	sed	-e 's/^#define HWLOC_SYM_TRANSFORM 0/#define HWLOC_SYM_TRANSFORM 1/'                    \
		-e 's/^#define HWLOC_SYM_PREFIX hwloc_/#define HWLOC_SYM_PREFIX foobar_/'               \
		-e 's/^#define HWLOC_SYM_PREFIX_CAPS HWLOC_/#define HWLOC_SYM_PREFIX_CAPS FOOBAR_/'     \
		$(HWLOC_top_builddir)/include/private/autogen/config.h > include/private/autogen/config.h

useless:
	@echo "# Finding useless renaming"
	@if grep '^#define' $(HWLOC_top_srcdir)/include/hwloc/rename.h \
		| awk '{print $$2}' \
		| egrep -v '(HWLOC_RENAME_H|HWLOC_MUNGE_NAME|HWLOC_NAME)' \
		| while read name ; do \
			grep $$name -rwH $(HWLOC_top_srcdir)/include/ \
			| grep -v rename.h: >/dev/null \
			|| echo $$name; \
			done \
	 | grep '' ; then false ; else true ; fi
	@echo "# done"

cpp.out: include/hwloc/autogen/config.h include/private/autogen/config.h
	@echo "# Checking for warnings/errors during renaming"
	@echo "# using $(TEST_CPPFLAGS)"
	@$(CPP) -Werror $(CPPFLAGS) $(TEST_CPPFLAGS) -Iinclude -I$(HWLOC_top_srcdir)/include -I$(HWLOC_top_builddir)/include $(srcdir)/main.c >$@

# hwloc_uint64_t is ignored because it's always a typedef.
# we have to manually concat a##b because some cpp let gcc do it (at least cpp 4.2.1 on OpenBSD 5.2).
needed: cpp.out
	@echo "# Finding needed renaming"
	@if cat cpp.out \
		| sed -e 's/foobar_ ##   hwloc_ ## /foobar_hwloc_/g' \
		| sed -e 's/FOOBAR_ ##   hwloc_ ## /FOOBAR_hwloc_/g' \
		| grep -vw HWLOC_DEBUG_VERBOSE \
		| grep -vw hwloc_uint64_t \
		| egrep -i '(^| |\*)hwloc_' ; then false ; else true ; fi
	@echo "# done"

clean-local:
	rm -rf include

EXTRA_DIST	=	main.c
