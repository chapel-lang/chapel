#*****************************************************************************#
# 
# makefile
# for libpvm3.a, pvmd3
#
#*****************************************************************************#

include $(PVM_ROOT_U)/conf/win32cygwin.def

PVM_LIB = $(PVM_ROOT_U)/lib/$(PVM_ARCH)

XDR_H = $(PVM_ROOT_U)/xdr/types.h $(PVM_ROOT_U)/xdr/xdr.h

LOBJ = $(PVM_ARCH)/global.o \
	$(PVM_ARCH)/imalloc.o \
	$(PVM_ARCH)/lpvm.o \
	$(PVM_ARCH)/lpvmcat.o \
	$(PVM_ARCH)/lpvmgen.o \
	$(PVM_ARCH)/lpvmglob.o \
	$(PVM_ARCH)/lpvmpack.o \
	$(PVM_ARCH)/pmsg.o \
	$(PVM_ARCH)/pvmalloc.o \
	$(PVM_ARCH)/pvmcruft.o \
	$(PVM_ARCH)/pvmdabuf.o \
	$(PVM_ARCH)/pvmerr.o \
	$(PVM_ARCH)/pvmfrag.o \
	$(PVM_ARCH)/tev.o \
	$(PVM_ARCH)/waitc.o \
	$(PVM_ARCH)/xdr.o \
	$(PVM_ARCH)/xdr_floa.o \
	$(PVM_ARCH)/xdr_mem.o

DOBJ = $(PVM_ARCH)/ddpro.o \
	$(PVM_ARCH)/global.o \
	$(PVM_ARCH)/host.o \
	$(PVM_ARCH)/hoster.o \
	$(PVM_ARCH)/imalloc.o \
	$(PVM_ARCH)/msgbox.o \
	$(PVM_ARCH)/pkt.o \
	$(PVM_ARCH)/pmsg.o \
	$(PVM_ARCH)/pvmalloc.o \
	$(PVM_ARCH)/pvmcruft.o \
	$(PVM_ARCH)/pvmd.o \
	$(PVM_ARCH)/pvmdabuf.o \
	$(PVM_ARCH)/pvmdpack.o \
	$(PVM_ARCH)/pvmdtev.o \
	$(PVM_ARCH)/pvmdunix.o \
	$(PVM_ARCH)/pvmerr.o \
	$(PVM_ARCH)/pvmfrag.o \
	$(PVM_ARCH)/pvmlog.o \
	$(PVM_ARCH)/pvmregex.o \
	$(PVM_ARCH)/regex.o \
	$(PVM_ARCH)/sdpro.o \
	$(PVM_ARCH)/task.o \
	$(PVM_ARCH)/tdpro.o \
	$(PVM_ARCH)/waitc.o \
	$(PVM_ARCH)/xdr.o \
	$(PVM_ARCH)/xdr_mem.o \
	$(PVM_ARCH)/xdr_floa.o

all: $(LIB) $(PVM_LIB)/pvmd3.exe

$(PVM_LIB)/pvmd3.exe: $(DOBJ) 
	$(CC) -o  $(PVM_LIB)/pvmd3 $(DOBJ) $(LINK_FLAGS)

$(LIB): $(LOBJ)
	ar -rs $(LIB) $(LOBJ)

$(PVM_ARCH)/ddpro.o:    ddpro.c $(XDR_H) global.h
	$(CC) $(CFLAGS) $(CDEBUG) \
	 -o $(PVM_ARCH)/ddpro.o ddpro.c

$(PVM_ARCH)/global.o:   global.c $(XDR_H) global.h
	$(CC) $(CFLAGS) $(CDEBUG) \
	 -o $(PVM_ARCH)/global.o global.c

$(PVM_ARCH)/host.o:     host.c $(XDR_H) global.h
	$(CC) $(CFLAGS) $(CDEBUG) \
	 -o $(PVM_ARCH)/host.o host.c

$(PVM_ARCH)/hoster.o:     hoster.c $(XDR_H) global.h
	$(CC) $(CFLAGS) $(CDEBUG) \
	 -o $(PVM_ARCH)/hoster.o hoster.c

$(PVM_ARCH)/imalloc.o:  imalloc.c
	$(CC) $(CFLAGS) $(CDEBUG) \
	 -o $(PVM_ARCH)/imalloc.o imalloc.c

$(PVM_ARCH)/lpvm.o:     lpvm.c $(XDR_H) global.h
	$(CC) $(CFLAGS) $(CDEBUG) \
	 -o $(PVM_ARCH)/lpvm.o lpvm.c

$(PVM_ARCH)/lpvmcat.o:  lpvmcat.c $(XDR_H) global.h
	$(CC) $(CFLAGS) $(CDEBUG) \
	 -o $(PVM_ARCH)/lpvmcat.o lpvmcat.c

$(PVM_ARCH)/lpvmgen.o:  lpvmgen.c $(XDR_H) global.h
	$(CC) $(CFLAGS) $(CDEBUG) \
	 -o $(PVM_ARCH)/lpvmgen.o lpvmgen.c

$(PVM_ARCH)/lpvmglob.o: lpvmglob.c $(XDR_H) global.h
	$(CC) $(CFLAGS) $(CDEBUG) \
	 -o $(PVM_ARCH)/lpvmglob.o lpvmglob.c

$(PVM_ARCH)/lpvmpack.o: lpvmpack.c $(XDR_H) global.h
	$(CC) $(CFLAGS) $(CDEBUG) \
	 -o $(PVM_ARCH)/lpvmpack.o lpvmpack.c

$(PVM_ARCH)/msgbox.o:   msgbox.c $(XDR_H) global.h
	$(CC) $(CFLAGS) $(CDEBUG) \
	 -o $(PVM_ARCH)/msgbox.o msgbox.c

$(PVM_ARCH)/pkt.o:      pkt.c 
	$(CC) $(CFLAGS) $(CDEBUG) \
	 -o $(PVM_ARCH)/pkt.o pkt.c

$(PVM_ARCH)/pmsg.o:     pmsg.c $(XDR_H) global.h
	$(CC) $(CFLAGS) $(CDEBUG) \
	 -o $(PVM_ARCH)/pmsg.o pmsg.c

$(PVM_ARCH)/pvmalloc.o: pvmalloc.c 
	$(CC) $(CFLAGS) $(CDEBUG) \
	 -o $(PVM_ARCH)/pvmalloc.o pvmalloc.c

$(PVM_ARCH)/pvmcruft.o: pvmcruft.c global.h
	$(CC) $(CFLAGS) $(CDEBUG) \
	 -o $(PVM_ARCH)/pvmcruft.o pvmcruft.c

$(PVM_ARCH)/pvmd.o:     pvmd.c $(XDR_H) global.h
	$(CC) $(CFLAGS) $(CDEBUG) \
	 -o $(PVM_ARCH)/pvmd.o pvmd.c

$(PVM_ARCH)/pvmdabuf.o: pvmdabuf.c
	$(CC) $(CFLAGS) $(CDEBUG) \
	 -o $(PVM_ARCH)/pvmdabuf.o pvmdabuf.c

$(PVM_ARCH)/pvmdpack.o: pvmdpack.c $(XDR_H) global.h
	$(CC) $(CFLAGS) $(CDEBUG) \
	 -o $(PVM_ARCH)/pvmdpack.o pvmdpack.c

$(PVM_ARCH)/pvmdtev.o: pvmdtev.c $(XDR_H) global.h
	$(CC) $(CFLAGS) $(CDEBUG) \
	 -o $(PVM_ARCH)/pvmdtev.o pvmdtev.c

$(PVM_ARCH)/pvmdunix.o:pvmdunix.c $(XDR_H) global.h
	$(CC) $(CFLAGS) $(CDEBUG) \
	 -o $(PVM_ARCH)/pvmdunix.o pvmdunix.c

$(PVM_ARCH)/pvmerr.o:   pvmerr.c global.h
	$(CC) $(CFLAGS) $(CDEBUG) \
	 -o $(PVM_ARCH)/pvmerr.o pvmerr.c

$(PVM_ARCH)/pvmfrag.o:  pvmfrag.c 
	$(CC) $(CFLAGS) $(CDEBUG) \
	 -o $(PVM_ARCH)/pvmfrag.o pvmfrag.c

$(PVM_ARCH)/pvmlog.o:   pvmlog.c 
	$(CC) $(CFLAGS) $(CDEBUG) \
	 -o $(PVM_ARCH)/pvmlog.o pvmlog.c

$(PVM_ARCH)/pvmregex.o: regex/pvmregex.c
	$(CC) $(CFLAGS) $(CDEBUG) \
	 -o $(PVM_ARCH)/pvmregex.o regex/pvmregex.c

$(PVM_ARCH)/regex.o:    regex/regex.c
	$(CC) $(CFLAGS) $(CDEBUG) \
	 -o $(PVM_ARCH)/regex.o regex/regex.c

$(PVM_ARCH)/sdpro.o:    sdpro.c $(XDR_H) global.h
	$(CC) $(CFLAGS) $(CDEBUG) \
	 -o $(PVM_ARCH)/sdpro.o sdpro.c

$(PVM_ARCH)/task.o:     task.c $(XDR_H) global.h
	$(CC) $(CFLAGS) $(CDEBUG) \
	 -o $(PVM_ARCH)/task.o task.c

$(PVM_ARCH)/tdpro.o:    tdpro.c $(XDR_H) global.h
	$(CC) $(CFLAGS) $(CDEBUG) \
	 -o $(PVM_ARCH)/tdpro.o tdpro.c

$(PVM_ARCH)/tev.o:      tev.c $(XDR_H) global.h
	$(CC) $(CFLAGS) $(CDEBUG) \
	 -o $(PVM_ARCH)/tev.o tev.c

$(PVM_ARCH)/waitc.o:    waitc.c $(XDR_H) global.h
	$(CC) $(CFLAGS) $(CDEBUG) \
	 -o $(PVM_ARCH)/waitc.o waitc.c

$(PVM_ARCH)/xdr.o:      $(PVM_ROOT_U)/xdr/xdr.c $(XDR_H)
	$(CC) $(CFLAGS) $(CDEBUG) \
	 -o $(PVM_ARCH)/xdr.o $(PVM_ROOT_U)/xdr/xdr.c

$(PVM_ARCH)/xdr_floa.o: $(PVM_ROOT_U)/xdr/xdr_floa.c $(XDR_H)
	$(CC) $(CFLAGS) $(CDEBUG) \
	 -o $(PVM_ARCH)/xdr_floa.o $(PVM_ROOT_U)/xdr/xdr_floa.c

$(PVM_ARCH)/xdr_mem.o:  $(PVM_ROOT_U)/xdr/xdr_mem.c $(XDR_H)
	$(CC) $(CFLAGS) $(CDEBUG) \
	 -o $(PVM_ARCH)/xdr_mem.o $(PVM_ROOT_U)/xdr/xdr_mem.c

# Clean up everything but the .EXEs
clean :
	-rm -f $(PVM_ARCH)/*.o
