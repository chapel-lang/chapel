CHAPEL_ROOT = ../../..
CHPL_MAKE_HOST_TARGET = --target

include $(CHAPEL_ROOT)/make/Makefile.base

DLMALLOC_CFLAGS += -O3 $(CHPL_DLMALLOC_CFLAGS)

DLMALLOC_DEFINES = USE_LOCKS -DONLY_MSPACES -DMSPACES

DLMALLOC_SRCS = dlmalloc.c

DLMALLOC_OBJS = $(DLMALLOC_SRCS:%.c=$(DLMALLOC_BUILD_DIR)/%.o)

.PHONY: all
all: $(DLMALLOC_LIB_DIR)/libdlmalloc.a

$(DLMALLOC_LIB_DIR)/libdlmalloc.a: $(DLMALLOC_OBJS) $(DLMALLOC_LIB_DIR)/.timestamp
	$(AR) -rc $@ $(DLMALLOC_OBJS)
	$(RANLIB) $@

$(DLMALLOC_BUILD_DIR)/%.o: $(DLMALLOC_SRCS) $(DLMALLOC_BUILD_DIR)/.timestamp
	$(CC) -I$(DLMALLOC_INCLUDE_DIR) -D$(DLMALLOC_DEFINES) $(DLMALLOC_CFLAGS) -c $*.c -o $@


%/.timestamp:
	@mkdir -p $*
	@-touch $*/.timestamp
