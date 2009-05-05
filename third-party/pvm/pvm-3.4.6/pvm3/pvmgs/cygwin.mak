#*****************************************************************************#
# 
# makefile
# for libgpvm3.a, pvmgs
#
#*****************************************************************************#

include $(PVM_ROOT_U)/conf/win32cygwin.def

LOBJ = $(PVM_ARCH)/pvmgsu_aux.o \
	$(PVM_ARCH)/pvmgsu_core.o \
	$(PVM_ARCH)/pvmgs_func.o

PVM_BIN = $(PVM_ROOT_U)/bin/$(PVM_ARCH)

all: $(GLIB) $(PVM_BIN)/pvmgs.exe

$(PVM_BIN)/pvmgs.exe: $(PVM_ARCH)/pvmgs_core.o $(PVM_ARCH)/pvmgs_func.o
	$(CC) -o $(PVM_BIN)/pvmgs $(PVM_ARCH)/pvmgs_core.o \
	 $(PVM_ARCH)/pvmgs_func.o $(LIB) $(LINK_FLAGS)

$(GLIB): $(LOBJ)
	ar -rs $(GLIB) $(LOBJ)

$(PVM_ARCH)/pvmgsu_aux.o:  pvmgsu_aux.c
	$(CC) $(CFLAGS) $(CDEBUG) \
	 -o $(PVM_ARCH)/pvmgsu_aux.o pvmgsu_aux.c

$(PVM_ARCH)/pvmgsu_core.o: pvmgsu_core.c
	$(CC) $(CFLAGS) $(CDEBUG) \
	 -o $(PVM_ARCH)/pvmgsu_core.o pvmgsu_core.c

$(PVM_ARCH)/pvmgs_func.o: pvmgs_func.c
	$(CC) $(CFLAGS) $(CDEBUG) \
	 -o $(PVM_ARCH)/pvmgs_func.o pvmgs_func.c

$(PVM_ARCH)/pvmgs_core.o: pvmgs_core.c
	$(CC) $(CFLAGS) $(CDEBUG) \
	 -o $(PVM_ARCH)/pvmgs_core.o pvmgs_core.c

# Clean up everything but the .EXEs
clean :
	-rm -f $(PVM_ARCH)/*.o

