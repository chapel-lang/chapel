all-local:
	@echo Existing targets: 'needed' and 'useless'

prepare:
	@echo
	@echo "###############################"
	@echo Enabling renaming in headers...
	@echo
	sed	-e '/^#define HWLOC_SYM_TRANSFORM 0/s/0/1/'			\
		-e '/^#define HWLOC_SYM_PREFIX hwloc_/s/hwloc_/foobar_/1'	\
		-e '/^#define HWLOC_SYM_PREFIX_CAPS HWLOC_/s/HWLOC_/FOOBAR_/2'	\
		-i $(abs_top_builddir)/include/hwloc/autogen/config.h $(abs_top_builddir)/include/private/autogen/config.h

needed: prepare
	@echo
	@echo "###########################"
	@echo Finding missing renaming...
	@echo
	cpp $(srcdir)/main.c -I $(abs_top_srcdir)/include -I $(abs_top_builddir)/include \
		| egrep -i '(^| |\*)hwloc_'

useless: prepare
	@echo
	@echo "###########################"
	@echo Finding useless renaming...
	@echo
	grep '^#define' $(abs_top_srcdir)/include/hwloc/rename.h \
		| awk '{print $$2}' \
		| egrep -v '(HWLOC_RENAME_H|HWLOC_MUNGE_NAME|HWLOC_NAME)' \
		| while read name ; do \
			grep $$name -rwH $(abs_top_srcdir)/include/ \
			| grep -v rename.h: | grep -v .svn >/dev/null \
			|| echo $$name; \
			done

EXTRA_DIST	=	main.c
