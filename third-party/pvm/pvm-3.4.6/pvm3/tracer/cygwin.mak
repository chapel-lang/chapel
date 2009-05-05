#*****************************************************************************#
# 
# makefile
# for the PVM tracer
#
#*****************************************************************************#

include $(PVM_ROOT_U)/conf/win32cygwin.def

LIBS = $(PVM_ARCH)/libpvmtracer.a $(LIB)

OBJS = $(PVM_ARCH)/cmd.o \
	$(PVM_ARCH)/trcglob.o \
	$(PVM_ARCH)/tracer.o

LIBOBJS	= $(PVM_ARCH)/trcmess.o \
	   $(PVM_ARCH)/trcfile.o \
	   $(PVM_ARCH)/trccompat.o \
	   $(PVM_ARCH)/trccompatglob.o \
	   $(PVM_ARCH)/trcutil.o \
	   $(PVM_ARCH)/trclib.o

all:	$(PVM_ARCH)/libpvmtracer.a $(PVM_ARCH)/tracer.exe

$(PVM_ARCH)/libpvmtracer.a:  $(LIBOBJS)
	ar -rs $(PVM_ARCH)/libpvmtracer.a $(LIBOBJS)

$(PVM_ARCH)/tracer.exe: $(OBJS) $(PVM_ARCH)/libpvmtracer.a
	$(CC) -o $(PVM_ARCH)/tracer $(OBJS) $(LIBS) $(LINK_FLAGS)

$(PVM_ARCH)/cmd.o: cmd.c
	$(CC) $(CFLAGS) $(CDEBUG) \
	 -o $(PVM_ARCH)/cmd.o cmd.c

$(PVM_ARCH)/trcglob.o: trcglob.c
	$(CC) $(CFLAGS) $(CDEBUG) \
	 -o $(PVM_ARCH)/trcglob.o trcglob.c

$(PVM_ARCH)/tracer.o: tracer.c
	$(CC) $(CFLAGS) $(CDEBUG) \
	 -o $(PVM_ARCH)/tracer.o tracer.c

# LIB

$(PVM_ARCH)/trcmess.o: trcmess.c
	$(CC) $(CFLAGS) $(CDEBUG) \
	 -o $(PVM_ARCH)/trcmess.o trcmess.c

$(PVM_ARCH)/trcfile.o: trcfile.c
	$(CC) $(CFLAGS) $(CDEBUG) \
	 -o $(PVM_ARCH)/trcfile.o trcfile.c

$(PVM_ARCH)/trccompat.o: trccompat.c
	$(CC) $(CFLAGS) $(CDEBUG) \
	 -o $(PVM_ARCH)/trccompat.o trccompat.c

$(PVM_ARCH)/trccompatglob.o: trccompatglob.c
	$(CC) $(CFLAGS) $(CDEBUG) \
	 -o $(PVM_ARCH)/trccompatglob.o trccompatglob.c

$(PVM_ARCH)/trcutil.o: trcutil.c
	$(CC) $(CFLAGS) $(CDEBUG) \
	 -o $(PVM_ARCH)/trcutil.o trcutil.c

$(PVM_ARCH)/trclib.o: trclib.c
	$(CC) $(CFLAGS) $(CDEBUG) \
	 -o $(PVM_ARCH)/trclib.o trclib.c

# Clean up everything but the .EXEs
clean :
	-rm -f $(PVM_ARCH)/*.o
