#*****************************************************************************#
# 
# makefile
# for PVM Examples:
#	c:    hello hello_other spmd master1 slave1 timing mbox bwtest
#	f:    spmdf hit gexample ms-sl
#
#*****************************************************************************#

include $(PVM_ROOT_U)/conf/win32cygwin.def

PVM_BIN = $(PVM_ROOT_U)/bin/$(PVM_ARCH)

SDIR = $(PVM_ROOT_U)/examples

CPROGS = $(PVM_BIN)/gexample \
	  $(PVM_BIN)/hello \
	  $(PVM_BIN)/hello_other \
	  $(PVM_BIN)/master1 \
	  $(PVM_BIN)/slave1 \
	  $(PVM_BIN)/spmd \
	  $(PVM_BIN)/timing \
	  $(PVM_BIN)/timing_slave \
	  $(PVM_BIN)/mbox \
	  $(PVM_BIN)/bwtest

CTXPROGS = $(PVM_BIN)/inherita \
	    $(PVM_BIN)/inheritb \
	    $(PVM_BIN)/inherit1 \
	    $(PVM_BIN)/inherit2 \
	    $(PVM_BIN)/inherit3

MBPROGS = $(PVM_BIN)/task0 \
	   $(PVM_BIN)/task1 \
	   $(PVM_BIN)/task_end \
	   $(PVM_BIN)/rme \
	   $(PVM_BIN)/lmbi \
	   $(PVM_BIN)/gmbi

MHFPROGS = $(PVM_BIN)/mhf_server \
	    $(PVM_BIN)/mhf_tickle

default:	$(PVM_BIN)/hello_other $(PVM_BIN)/hello

all:	c-all # $(MHFPROGS)

c-all:	$(CPROGS) $(CTXPROGS) $(MBPROGS) # $(MHFPROGS)

###############################################################
# C
###############################################################

$(PVM_BIN)/timing:  $(PVM_ARCH)/timing.o $(LIB) $(GLIB)
	$(CC) -o $(PVM_BIN)/timing $(PVM_ARCH)/timing.o \
	 $(LIB) $(GLIB) $(LINK_FLAGS)

$(PVM_BIN)/timing_slave:  $(PVM_ARCH)/timing_slave.o $(LIB) $(GLIB)
	$(CC) -o $(PVM_BIN)/timing_slave $(PVM_ARCH)/timing_slave.o \
	 $(LIB) $(GLIB) $(LINK_FLAGS)

$(PVM_BIN)/gexample:  $(PVM_ARCH)/gexample.o $(LIB) $(GLIB)
	$(CC) -o $(PVM_BIN)/gexample $(PVM_ARCH)/gexample.o \
	 $(LIB) $(GLIB) $(LINK_FLAGS)

$(PVM_BIN)/hello:  $(PVM_ARCH)/hello.o  $(LIB)
	$(CC) -o $(PVM_BIN)/hello $(PVM_ARCH)/hello.o \
	 $(LIB) $(LINK_FLAGS)

$(PVM_BIN)/hello_other:  $(PVM_ARCH)/hello_other.o $(LIB)
	$(CC) -o $(PVM_BIN)/hello_other $(PVM_ARCH)/hello_other.o \
	 $(LIB) $(LINK_FLAGS)

$(PVM_BIN)/mbox:  $(PVM_ARCH)/mbox.o $(LIB)
	$(CC) -o $(PVM_BIN)/mbox $(PVM_ARCH)/mbox.o \
	 $(LIB) $(LINK_FLAGS)

$(PVM_BIN)/bwtest:  $(PVM_ARCH)/bwtest.o $(LIB) $(GLIB)
	$(CC) -o $(PVM_BIN)/bwtest $(PVM_ARCH)/bwtest.o \
	 $(LIB) $(GLIB) $(LINK_FLAGS)

$(PVM_BIN)/master1:  $(PVM_ARCH)/master1.o $(LIB)
	$(CC) -o $(PVM_BIN)/master1 $(PVM_ARCH)/master1.o \
	 $(LIB) $(LINK_FLAGS)

$(PVM_BIN)/slave1:  $(PVM_ARCH)/slave1.o $(LIB)
	$(CC) -o $(PVM_BIN)/slave1 $(PVM_ARCH)/slave1.o \
	 $(LIB) $(LINK_FLAGS)

$(PVM_BIN)/spmd:  $(PVM_ARCH)/spmd.o $(LIB) $(GLIB)
	$(CC) -o $(PVM_BIN)/spmd $(PVM_ARCH)/spmd.o \
	 $(LIB) $(GLIB) $(LINK_FLAGS)

$(PVM_BIN)/inherita:  $(PVM_ARCH)/inherita.o $(LIB)
	$(CC) -o $(PVM_BIN)/inherita $(PVM_ARCH)/inherita.o \
	 $(LIB) $(LINK_FLAGS)

$(PVM_BIN)/inheritb:  $(PVM_ARCH)/inheritb.o $(LIB)
	$(CC) -o $(PVM_BIN)/inheritb $(PVM_ARCH)/inheritb.o \
	 $(LIB) $(LINK_FLAGS)

$(PVM_BIN)/inherit1:  $(PVM_ARCH)/inherit1.o $(LIB)
	$(CC) -o $(PVM_BIN)/inherit1 $(PVM_ARCH)/inherit1.o \
	 $(LIB) $(LINK_FLAGS)

$(PVM_BIN)/inherit2:  $(PVM_ARCH)/inherit2.o $(LIB)
	$(CC) -o $(PVM_BIN)/inherit2 $(PVM_ARCH)/inherit2.o \
	 $(LIB) $(LINK_FLAGS)

$(PVM_BIN)/inherit3:  $(PVM_ARCH)/inherit3.o $(LIB)
	$(CC) -o $(PVM_BIN)/inherit3 $(PVM_ARCH)/inherit3.o \
	 $(LIB) $(LINK_FLAGS)

$(PVM_BIN)/task0:  $(PVM_ARCH)/task0.o $(PVM_ARCH)/taskf.o $(LIB)
	$(CC) -o $(PVM_BIN)/task0 $(PVM_ARCH)/task0.o $(PVM_ARCH)/taskf.o \
	 $(LIB) $(LINK_FLAGS)

$(PVM_BIN)/task1:  $(PVM_ARCH)/task1.o $(PVM_ARCH)/taskf.o  $(LIB)
	$(CC) -o $(PVM_BIN)/task1 $(PVM_ARCH)/task1.o $(PVM_ARCH)/taskf.o \
	 $(LIB) $(LINK_FLAGS)

$(PVM_BIN)/task_end:  $(PVM_ARCH)/task_end.o $(LIB)
	$(CC) -o $(PVM_BIN)/task_end $(PVM_ARCH)/task_end.o \
	 $(LIB) $(LINK_FLAGS)

$(PVM_BIN)/rme:  $(PVM_ARCH)/rme.o $(LIB)
	$(CC) -o $(PVM_BIN)/rme $(PVM_ARCH)/rme.o \
	 $(LIB) $(LINK_FLAGS)

$(PVM_BIN)/lmbi:  $(PVM_ARCH)/lmbi.o $(PVM_ARCH)/taskf.o  $(LIB)
	$(CC) -o $(PVM_BIN)/lmbi $(PVM_ARCH)/lmbi.o $(PVM_ARCH)/taskf.o \
	 $(LIB) $(LINK_FLAGS)

$(PVM_BIN)/gmbi:  $(PVM_ARCH)/gmbi.o $(PVM_ARCH)/taskf.o $(LIB)
	$(CC) -o $(PVM_BIN)/gmbi $(PVM_ARCH)/gmbi.o $(PVM_ARCH)/taskf.o \
	 $(LIB) $(LINK_FLAGS)

$(PVM_BIN)/mhf_server:  $(PVM_ARCH)/mhf_server.o $(LIB)
	$(CC) -o $(PVM_BIN)/mhf_server $(PVM_ARCH)/mhf_server.o \
	 $(LIB) $(LINK_FLAGS)

$(PVM_BIN)/mhf_tickle:  $(PVM_ARCH)/mhf_tickle.o $(LIB)
	$(CC) -o $(PVM_BIN)/mhf_tickle $(PVM_ARCH)/mhf_tickle.o \
	 $(LIB) $(LINK_FLAGS)

#############
# C OBJS
#############

$(PVM_ARCH)/mbox.o:  $(SDIR)/mbox.c
	$(CC) $(CFLAGS) $(CDEBUG) \
	 -o $(PVM_ARCH)/mbox.o $(SDIR)/mbox.c

$(PVM_ARCH)/bwtest.o:  $(SDIR)/bwtest.c
	$(CC) $(CFLAGS) $(CDEBUG) \
	 -o $(PVM_ARCH)/bwtest.o $(SDIR)/bwtest.c

$(PVM_ARCH)/timing.o:  $(SDIR)/timing.c
	$(CC) $(CFLAGS) $(CDEBUG) \
	 -o $(PVM_ARCH)/timing.o $(SDIR)/timing.c

$(PVM_ARCH)/timing_slave.o:  $(SDIR)/timing_slave.c
	$(CC) $(CFLAGS) $(CDEBUG) \
	 -o $(PVM_ARCH)/timing_slave.o $(SDIR)/timing_slave.c

$(PVM_ARCH)/gexample.o:  $(SDIR)/gexample.c
	$(CC) $(CFLAGS) $(CDEBUG) \
	 -o $(PVM_ARCH)/gexample.o $(SDIR)/gexample.c

$(PVM_ARCH)/hello.o:  $(SDIR)/hello.c
	$(CC) $(CFLAGS) $(CDEBUG) \
	 -o $(PVM_ARCH)/hello.o $(SDIR)/hello.c

$(PVM_ARCH)/hello_other.o:  $(SDIR)/hello_other.c
	$(CC) $(CFLAGS) $(CDEBUG) \
	 -o $(PVM_ARCH)/hello_other.o $(SDIR)/hello_other.c

$(PVM_ARCH)/master1.o:  $(SDIR)/master1.c
	$(CC) $(CFLAGS) $(CDEBUG) \
	 -o $(PVM_ARCH)/master1.o $(SDIR)/master1.c

$(PVM_ARCH)/slave1.o:  $(SDIR)/slave1.c
	$(CC) $(CFLAGS) $(CDEBUG) \
	 -o $(PVM_ARCH)/slave1.o $(SDIR)/slave1.c

$(PVM_ARCH)/spmd.o:  $(SDIR)/spmd.c
	$(CC) $(CFLAGS) $(CDEBUG) \
	 -o $(PVM_ARCH)/spmd.o $(SDIR)/spmd.c

$(PVM_ARCH)/taskf.o:  $(SDIR)/taskf.c
	$(CC) $(CFLAGS) $(CDEBUG) \
	 -o $(PVM_ARCH)/taskf.o $(SDIR)/taskf.c

$(PVM_ARCH)/inherita.o:  $(SDIR)/inherita.c
	$(CC) $(CFLAGS) $(CDEBUG) \
	 -o $(PVM_ARCH)/inherita.o $(SDIR)/inherita.c

$(PVM_ARCH)/inheritb.o:  $(SDIR)/inheritb.c
	$(CC) $(CFLAGS) $(CDEBUG) \
	 -o $(PVM_ARCH)/inheritb.o $(SDIR)/inheritb.c

$(PVM_ARCH)/inherit1.o:  $(SDIR)/inherit1.c
	$(CC) $(CFLAGS) $(CDEBUG) \
	 -o $(PVM_ARCH)/inherit1.o $(SDIR)/inherit1.c

$(PVM_ARCH)/inherit2.o:  $(SDIR)/inherit2.c
	$(CC) $(CFLAGS) $(CDEBUG) \
	 -o $(PVM_ARCH)/inherit2.o $(SDIR)/inherit2.c

$(PVM_ARCH)/inherit3.o:  $(SDIR)/inherit3.c
	$(CC) $(CFLAGS) $(CDEBUG) \
	 -o $(PVM_ARCH)/inherit3.o $(SDIR)/inherit3.c

$(PVM_ARCH)/task0.o:  $(SDIR)/task0.c
	$(CC) $(CFLAGS) $(CDEBUG) \
	 -o $(PVM_ARCH)/task0.o $(SDIR)/task0.c

$(PVM_ARCH)/task1.o:  $(SDIR)/task1.c
	$(CC) $(CFLAGS) $(CDEBUG) \
	 -o $(PVM_ARCH)/task1.o $(SDIR)/task1.c

$(PVM_ARCH)/task_end.o:  $(SDIR)/task_end.c
	$(CC) $(CFLAGS) $(CDEBUG) \
	 -o $(PVM_ARCH)/task_end.o $(SDIR)/task_end.c

$(PVM_ARCH)/rme.o:  $(SDIR)/rme.c
	$(CC) $(CFLAGS) $(CDEBUG) \
	 -o $(PVM_ARCH)/rme.o $(SDIR)/rme.c

$(PVM_ARCH)/lmbi.o:  $(SDIR)/lmbi.c
	$(CC) $(CFLAGS) $(CDEBUG) \
	 -o $(PVM_ARCH)/lmbi.o $(SDIR)/lmbi.c

$(PVM_ARCH)/gmbi.o:  $(SDIR)/gmbi.c
	$(CC) $(CFLAGS) $(CDEBUG) \
	 -o $(PVM_ARCH)/gmbi.o $(SDIR)/gmbi.c

$(PVM_ARCH)/mhf_server.o:  $(SDIR)/mhf_server.c
	$(CC) $(CFLAGS) $(CDEBUG) \
	 -o $(PVM_ARCH)/mhf_server.o $(SDIR)/mhf_server.c

$(PVM_ARCH)/mhf_tickle.o:  $(SDIR)/mhf_tickle.c
	$(CC) $(CFLAGS) $(CDEBUG) \
	 -o $(PVM_ARCH)/mhf_tickle.o $(SDIR)/mhf_tickle.c

# Clean up everything but the .EXEs
clean:
	-rm -f $(PVM_ARCH)/*.o
