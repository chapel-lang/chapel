#
# Makefile: builds Chapel runtime
#

RUNTIME_ROOT = .
RUNTIME_SUBDIR = .


#
# include standard header for runtime
#
include $(RUNTIME_ROOT)/make/Makefile.runtime.head


#
# subdirectories and false subdirectory-oriented targets to force recursion
#
SUBDIRS = \
	src \
	comm-$(CHPL_MAKE_COMM) \
	launch-$(CHPL_MAKE_LAUNCHER) \
	threads-$(CHPL_MAKE_THREADS) \


#
# include source subdirectories here
#
include src/Makefile.include
include threads-$(CHPL_MAKE_THREADS)/Makefile.include
include comm-$(CHPL_MAKE_COMM)/Makefile.include
include launch-$(CHPL_MAKE_LAUNCHER)/Makefile.include

SVN_SRCS = 

RUNTIME_OBJS = \
	$(COMMON_OBJS) \
	$(THREADS_OBJS) \
	$(COMM_OBJS) \

LAUNCH_LIB_OBJS = \
	$(COMMON_LAUNCHER_OBJS) \
	$(COMM_LAUNCHER_OBJS) \
	$(LAUNCHER_OBJS) \


RUNTIME_DIR = ../lib/$(RUNTIME_UNIQUE_SUBDIR)
RUNTIME_LIB = $(RUNTIME_DIR)/libchpl.a
RUNTIME_DIR_TIMESTAMP = $(RUNTIME_DIR)/.timestamp

LAUNCHER_DIR = ../lib/$(LAUNCHER_UNIQUE_SUBDIR)
LAUNCHER_LIB = $(LAUNCHER_DIR)/libchpllaunch.a
LAUNCHER_DIR_TIMESTAMP = $(LAUNCHER_DIR)/.timestamp

ifneq ($(CHPL_MAKE_LAUNCHER),none)
LAUNCHER_TARGETS = $(LAUNCHER_LIB) $(LAUNCHER_DIR)/main_launcher.o
endif

TARGETS = \
	$(RUNTIME_LIB) \
	$(RUNTIME_DIR)/main.o \
	$(LAUNCHER_TARGETS) \

CLEAN_TARGS += $(TARGETS)

#
# main rules
#

include $(RUNTIME_ROOT)/make/Makefile.runtime.rules


#
# target-based rules
#

$(RUNTIME_LIB): $(RUNTIME_OBJS) $(RUNTIME_DIR_TIMESTAMP)
	$(AR) -rc $@ $(RUNTIME_OBJS)
	$(RANLIB) $@
	-@(which $(CHPL_TAGS_UTIL) > /dev/null 2>&1 && echo "Updating TAGS..." && $(CHPL_TAGS_UTIL) $(CHPL_TAGS_FLAGS) $(ALL_SRCS) $(CURDIR)/include/*.h $(CURDIR)/include/threads-$(CHPL_MAKE_THREADS)/*.h $(CURDIR)/include/comm-$(CHPL_MAKE_COMM)/*.h) || echo "$(CHPL_TAGS_UTIL) not available"

$(RUNTIME_DIR)/main.o: src/$(RUNTIME_OBJDIR)/main.o
	cp $< $@

$(RUNTIME_DIR_TIMESTAMP):
	mkdir -p $(RUNTIME_DIR)
	touch $(RUNTIME_DIR_TIMESTAMP)

#
# launcher rules
#
$(LAUNCHER_LIB): $(LAUNCH_LIB_OBJS) $(LAUNCHER_DIR_TIMESTAMP)
	$(AR) -rc $@ $(LAUNCH_LIB_OBJS)
	$(RANLIB) $@

$(LAUNCHER_DIR)/main_launcher.o: src/$(LAUNCHER_OBJDIR)/main_launcher.o
	cp $< $@

$(LAUNCHER_DIR_TIMESTAMP):
	mkdir -p $(LAUNCHER_DIR)
	touch $(LAUNCHER_DIR_TIMESTAMP)


#
# include standard footer for runtime
#
include $(RUNTIME_ROOT)/make/Makefile.runtime.foot
