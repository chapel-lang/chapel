CHAPEL_ROOT = ../../..
CHPL_MAKE_HOST_TARGET = --target
include $(CHAPEL_ROOT)/make/Makefile.base

HEAPMM_BUILD_DIR = $(MEM_HEAPMM_DIR)/build/$(CHPL_MAKE_TARGET_PLATFORM)-$(CHPL_MAKE_COMPILER)

HEAPMM_SRCS = hmm_alloc.c      \
              hmm_largest.c    \
              hmm_base.c       \
              hmm_resize.c     \
              hmm_dflt_abort.c \
              hmm_shrink.c     \
              hmm_grow.c       \
              hmm_true.c

HEAPMM_OBJS = $(HEAPMM_SRCS:%.c=$(HEAPMM_BUILD_DIR)/%.o)

HEAPMM_CFLAGS += -O3

.PHONY: all
all: $(MEM_HEAPMM_LIB_DIR)/libheapmm.a

$(MEM_HEAPMM_LIB_DIR)/libheapmm.a: $(HEAPMM_OBJS) $(MEM_HEAPMM_LIB_DIR)/.timestamp
	$(AR) -rc $@ $(HEAPMM_OBJS)
	$(RANLIB) $@

$(HEAPMM_BUILD_DIR)/%.o: $(HEAPMM_SRCS) $(HEAPMM_BUILD_DIR)/.timestamp
	$(CC) -I$(MEM_HEAPMM_INCLUDE_DIR) $(HEAPMM_CFLAGS) -c $*.c -o $@


%/.timestamp:
	@mkdir -p $*
	@-touch $*/.timestamp

clean:
	rm -f $(HEAPMM_OBJS) $(MEM_HEAPMM_LIB_DIR)/libheapmm.a

