CHAPEL_ROOT = ../../..
CHPL_MAKE_HOST_TARGET = --target
include $(CHAPEL_ROOT)/make/Makefile.base

HMM_BUILD_DIR = $(MEM_HMM_DIR)/build/$(CHPL_MAKE_TARGET_PLATFORM)-$(CHPL_MAKE_COMPILER)

HMM_SRCS = hmm_alloc.c      \
           hmm_largest.c    \
           hmm_base.c       \
           hmm_resize.c     \
           hmm_dflt_abort.c \
           hmm_shrink.c     \
           hmm_grow.c       \
           hmm_true.c

HMM_OBJS = $(HMM_SRCS:%.c=$(HMM_BUILD_DIR)/%.o)

HMM_CFLAGS += -O3

.PHONY: all
all: $(MEM_HMM_LIB_DIR)/libhmm.a

$(MEM_HMM_LIB_DIR)/libhmm.a: $(HMM_OBJS) $(MEM_HMM_LIB_DIR)/.timestamp
	$(AR) -rc $@ $(HMM_OBJS)
	$(RANLIB) $@

$(HMM_BUILD_DIR)/%.o: $(HMM_SRCS) $(HMM_BUILD_DIR)/.timestamp
	$(CC) -I$(MEM_HMM_INCLUDE_DIR) $(HMM_CFLAGS) -c $*.c -o $@


%/.timestamp:
	@mkdir -p $*
	@-touch $*/.timestamp

clean:
	rm -f $(HMM_OBJS) $(MEM_HMM_LIB_DIR)/libhmm.a

