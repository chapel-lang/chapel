CHAPEL_ROOT = ../..
include $(CHAPEL_ROOT)/make/Makefile.base
include $(CHAPEL_ROOT)/runtime/etc/Makefile.macros

CC = gcc
CFLAGS += -I../include/$(PLATFORM) -I../include

CFLAGS += -Wall -Wmissing-declarations -Wmissing-prototypes -Wstrict-prototypes
CFLAGS += $(ONLYCFLAGS)

CHPL_RT_SRCS = \
	arg.c \
	array.c \
	chplio.c \
	chplmem.c \
	chplmem_tests.c \
	chpltypes.c \
	config.c \
	domain.c \
	error.c \

CHPL_RT_OBJS = $(CHPL_RT_SRCS:%.c=$(PLATFORM)/%.o)
CHPL_RT_LIB = $(PLATFORM)/libchpl.a

CVS_DEPENDS = ./$(PLATFORM)/Makefile.cvs.depend

TARGETS = $(CHPL_RT_LIB)
CVS_SRCS = $(CHPL_RT_SRCS) main.c

all: $(CHPL_RT_LIB) $(PLATFORM)/main.o
	@if [ `grep "chplrt.h" $(CVS_SRCS) | wc -l` -ne `ls $(CVS_SRCS) | wc -l` ]; then echo "PROBLEM:  There's a runtime source file that does not include 'chplrt.h'."; exit 1; fi

clean:
	rm -f ./$(PLATFORM)/*.o $(TARGETS)

cleandeps:
	rm -f $(CVS_DEPENDS)

clobber: clean
	rm -f *~
	rm -rf ./$(PLATFORM)

depend: $(PLATFORM_DIR)
	@echo Updating dependences...
	@echo > $(CVS_DEPENDS)
	@for file in $(CVS_SRCS); do \
	  $(ECHO) -n $(PLATFORM)/ >> $(CVS_DEPENDS); \
	  $(CC) -MM -MG $(CFLAGS) $$file | grep -v \^\# >> $(CVS_DEPENDS); \
        done	

$(CHPL_RT_LIB): $(CHPL_RT_OBJS)
	$(AR) -rc $@ $(CHPL_RT_OBJS)
	@echo "Updating TAGS..."
	@etags -o ../TAGS $(CHPL_RT_SRCS) ../include/*.h

$(PLATFORM)/%.o: %.c $(PLATFORM_DIR)
	$(CC) -c $(CFLAGS) -o $@ $<

-include $(CVS_DEPENDS)
