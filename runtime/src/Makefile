
CHAPEL_ROOT = ../..
include $(CHAPEL_ROOT)/make/Makefile.base

CC = gcc
CFLAGS += -I../include

CHPL_RT_SRCS = \
	chplio.c
CHPL_RT_OBJS = $(CHPL_RT_SRCS:%.c=$(PLATFORM)/%.o)
CHPL_RT_LIB = $(PLATFORM)/libchpl.a

CVS_DEPENDS = ./$(PLATFORM)/Makefile.cvs.depend

TARGETS = $(CHPL_RT_LIB)
CVS_SRCS = $(CHPL_RT_SRCS) main.c

all: $(CHPL_RT_LIB) $(PLATFORM)/main.o

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
	  $(CCDEP) -MM -MG $(CFLAGS) $$file >> $(CVS_DEPENDS); \
        done	

$(CHPL_RT_LIB): $(CHPL_RT_OBJS)
	$(AR) -rc $@ $(CHPL_RT_OBJS)

$(PLATFORM)/%.o: %.c $(PLATFORM_DIR)
	$(CC) -c $(CFLAGS) -o $@ $<

-include $(CVS_DEPENDS)
