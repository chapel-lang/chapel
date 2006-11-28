CHAPEL_ROOT = ../..
DEBUG = 1
include $(CHAPEL_ROOT)/make/Makefile.base

RUNTIME_CFLAGS += -I../include/$(PLATFORM) -I../include

CHPL_RT_SRCS = \
	arg.c \
	chplio.c \
	chplmem.c \
	chplmem_tests.c \
	chplsys.c \
	chplthreads.c \
	chpltypes.c \
	config.c \
	error.c \

CHPL_RT_OBJS = $(CHPL_RT_SRCS:%.c=$(PLATFORM)/%.o)
CHPL_RT_DIR = ../../lib/$(PLATFORM)
CHPL_RT_LIB = $(CHPL_RT_DIR)/libchpl.a

CVS_DEPENDS = ./$(PLATFORM)/Makefile.cvs.depend

TARGETS = $(CHPL_RT_LIB) $(CHPL_RT_DIR)/main.o
CVS_SRCS = $(CHPL_RT_SRCS) main.c

default: all

all: $(CHPL_RT_DIR) $(TARGETS)
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
	@rm -f $(CVS_DEPENDS).tmp
	@echo > $(CVS_DEPENDS).tmp
	@for file in $(CVS_SRCS); do \
	  $(ECHO) -n $(PLATFORM)/ >> $(CVS_DEPENDS).tmp; \
	  $(CMAKEDEPEND) $(RUNTIME_CFLAGS) $$file | grep -v \^\# >> $(CVS_DEPENDS).tmp; \
        done	
	@mv -f $(CVS_DEPENDS).tmp $(CVS_DEPENDS)

$(CHPL_RT_LIB): $(CHPL_RT_OBJS)
	$(AR) -rc $@ $(CHPL_RT_OBJS)
	@echo "Updating TAGS..."
	@etags -o ../TAGS $(CHPL_RT_SRCS) ../include/*.h

$(CHPL_RT_DIR)/main.o: $(PLATFORM)/main.o
	cp $< $@

$(CHPL_RT_DIR):
	mkdir -p $@


$(PLATFORM)/%.o: %.c $(PLATFORM_DIR)
	$(CC) -c $(RUNTIME_CFLAGS) -o $@ $<


-include $(CVS_DEPENDS)
