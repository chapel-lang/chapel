CHAPEL_ROOT = ../../..

include $(CHAPEL_ROOT)/make/Makefile.base

LEA_CFLAGS = -O3

LEA_BUILD_DIR = $(MEM_LEA_DIR)/build/$(CHPL_MAKE_TARGET_PLATFORM)-$(CHPL_MAKE_COMPILER)

LEA_DEFINES = USE_LOCKS -DONLY_MSPACES -DMSPACES

LEA_SRCS = dlmalloc.c

LEA_OBJS = $(LEA_SRCS:%.c=$(LEA_BUILD_DIR)/%.o)

.PHONY: all
all: $(MEM_LEA_LIB_DIR)/libdlmalloc.a

$(MEM_LEA_LIB_DIR)/libdlmalloc.a: $(LEA_OBJS) $(MEM_LEA_LIB_DIR)/.timestamp
	$(AR) -rc $@ $(LEA_OBJS)
	$(RANLIB) $@

$(LEA_BUILD_DIR)/%.o: $(LEA_SRCS) $(LEA_BUILD_DIR)/.timestamp
	$(CC) -I$(MEM_LEA_INCLUDE_DIR) -D$(LEA_DEFINES) $(LEA_CFLAGS) -c $*.c -o $@


%/.timestamp:
	@mkdir -p $*
	@-touch $*/.timestamp

clean:
	rm -f $(LEA_OBJS) $(MEM_LEA_LIB_DIR)/libdlmalloc.a

