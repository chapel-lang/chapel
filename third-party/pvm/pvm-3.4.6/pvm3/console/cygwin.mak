#*****************************************************************************#
# 
# makefile
# for the PVM console: pvm.exe
#
#*****************************************************************************#

include $(PVM_ROOT_U)/conf/win32cygwin.def

PVM_CONSOLE = $(PVM_ARCH)

OBJS = $(PVM_ARCH)/cmds.o \
	$(PVM_ARCH)/cons.o \
	$(PVM_ARCH)/job.o \
	$(PVM_ARCH)/trc.o

LIBS = $(LIB) \
	$(PVM_ROOT_U)/tracer/$(PVM_ARCH)/libpvmtracer.a

LINK_FLAGS = -lreadline

all:  $(PVM_CONSOLE)/pvm.exe

#

$(PVM_CONSOLE)/pvm.exe: $(OBJS) $(LIBS)
	$(CC) -o $(PVM_CONSOLE)/pvm $(OBJS) $(LIBS) $(LINK_FLAGS)

$(PVM_ARCH)/cmds.o:  $(PVM_ROOT_U)/console/cmds.c
	$(CC) $(CFLAGS) $(CDEBUG) \
	 -o $(PVM_ROOT_U)/console/$(PVM_ARCH)/cmds.o $(PVM_ROOT_U)/console/cmds.c

$(PVM_ARCH)/cons.o:  $(PVM_ROOT_U)/console/cons.c
	$(CC) $(CFLAGS) $(CDEBUG) \
	 -o $(PVM_ROOT_U)/console/$(PVM_ARCH)/cons.o $(PVM_ROOT_U)/console/cons.c

$(PVM_ARCH)/trc.o:  $(PVM_ROOT_U)/console/trc.c
	$(CC) $(CFLAGS) $(CDEBUG) \
	 -o $(PVM_ROOT_U)/console/$(PVM_ARCH)/trc.o $(PVM_ROOT_U)/console/trc.c

$(PVM_ARCH)/job.o:  $(PVM_ROOT_U)/console/job.c
	$(CC) $(CFLAGS) $(CDEBUG) \
	 -o $(PVM_ROOT_U)/console/$(PVM_ARCH)/job.o $(PVM_ROOT_U)/console/job.c

# Clean up everything but the .EXEs

clean:
	-rm -f $(PVM_ARCH)/*.o
