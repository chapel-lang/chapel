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
	threads-$(CHPL_THREADS) \


#
# include source subdirectories here
#
include src/Makefile.include
include threads-$(CHPL_THREADS)/Makefile.include

CVS_SRCS = 

RUNTIME_OBJS = \
	$(COMMON_OBJS) \
	$(THREADS_OBJS)

RUNTIME_DIR = ../lib/$(CHPL_MAKE_PLATFORM)
RUNTIME_LIB = $(RUNTIME_DIR)/libchpl.a

TARGETS = $(RUNTIME_DIR)/main.o $(RUNTIME_LIB)

CLEAN_TARGS += $(TARGETS)

#
# main rules
#

include $(RUNTIME_ROOT)/make/Makefile.runtime.rules


#
# target-based rules
#

$(RUNTIME_LIB): $(RUNTIME_OBJS)
	$(AR) -rc $@ $(RUNTIME_OBJS)
	$(RANLIB) $@
	-@(which etags > /dev/null 2>&1 && echo "Updating TAGS..." && etags -o ../TAGS $(RUNTIME_SRCS) include/*.h include/threads-$(CHPL_THREADS)/*.h) || echo "etags not available"

$(RUNTIME_DIR)/main.o: src/$(CHPL_MAKE_PLATFORM)/main.o
	@make $(RUNTIME_DIR)
	cp $< $@

$(RUNTIME_DIR):
	mkdir -p $@

#
# include standard footer for runtime
#
include $(RUNTIME_ROOT)/make/Makefile.runtime.foot
