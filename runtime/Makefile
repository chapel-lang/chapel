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
	threads-$(CHPL_MAKE_THREADS) \
	comm-$(CHPL_MAKE_COMM) \


#
# include source subdirectories here
#
include src/Makefile.include
include threads-$(CHPL_MAKE_THREADS)/Makefile.include
include comm-$(CHPL_MAKE_COMM)/Makefile.include

CVS_SRCS = 

RUNTIME_OBJS = \
	$(COMMON_OBJS) \
	$(THREADS_OBJS) \
	$(COMM_OBJS) \

RUNTIME_DIR = ../lib/$(CHPL_MAKE_PLATFORM)/threads-$(CHPL_MAKE_THREADS)/comm-$(CHPL_MAKE_COMM)
RUNTIME_LIB = $(RUNTIME_DIR)/libchpl.a
RUNTIME_DIR_TIMESTAMP = $(RUNTIME_DIR)/timestamp

TARGETS = $(RUNTIME_LIB) $(RUNTIME_DIR)/main.o

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
	-@(which etags > /dev/null 2>&1 && echo "Updating TAGS..." && etags -o ../TAGS $(RUNTIME_SRCS) include/*.h include/threads-$(CHPL_MAKE_THREADS)/*.h include/comm-$(CHPL_MAKE_COMM)/*.h) || echo "etags not available"

$(RUNTIME_DIR)/main.o: src/$(CHPL_MAKE_PLATFORM)/threads-$(CHPL_MAKE_THREADS)/comm-$(CHPL_MAKE_COMM)/main.o
	cp $< $@

$(RUNTIME_DIR_TIMESTAMP):
	mkdir -p $(RUNTIME_DIR)
	touch $(RUNTIME_DIR_TIMESTAMP)

#
# include standard footer for runtime
#
include $(RUNTIME_ROOT)/make/Makefile.runtime.foot
