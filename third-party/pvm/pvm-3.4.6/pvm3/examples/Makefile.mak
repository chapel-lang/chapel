#
# $Id: Makefile.mak,v 1.11 1999/03/12 17:55:28 pvmsrc Exp $
#

#*************************************************************#
#**                                                         **#
#**      PVM Examples:                                      **#
#**             c:    hello.exe hello_other.exe             **#
#**               spmd.exe master1 slave1                   **#
#**                   timing mbox bwtest                    **#
#**             f:    spmdf hit                             **#
#**                   gexample ms-sl                        **#
#*************************************************************#

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE
NULL=nul
!ENDIF

!include $(PVM_ROOT)\conf\$(PVM_ARCH).def

SDIR		=	$(PVM_ROOT)\examples

CPROGS		=	gexample hello hello_other master1 slave1 spmd \
				timing timing_slave mbox bwtest

FPROGS		=	fmaster1 fslave1 fspmd hitc hitc_slave

CTXPROGS	=	inherita inheritb inherit1 inherit2 inherit3

MBPROGS		=	task0 task1 task_end rme lmbi gmbi

MHFPROGS	=	mhf_server mhf_tickle

LIB		=	$(PVM_ROOT)\lib\$(PVM_ARCH)\libpvm3.lib

GLIB		=	$(PVM_ROOT)\lib\$(PVM_ARCH)\libgpvm3.lib

FLIB		=       $(PVM_ROOT)\libfpvm\$(PVM_ARCH)\libfpvm3.lib

FLIBPATH	=	/LIBPATH:$(PVM_FLIBDIR) \
				/LIBPATH:$(PVM_CLIBDIR)

default:	bin hello_other hello

all:	bin c-all f-all

c-all:	bin $(CPROGS) $(CTXPROGS) $(MBPROGS) $(MHFPROGS)

f-all:	bin $(FPROGS)


###############################################################
# C
###############################################################

bin:
	if not exist "$(PVM_ARCH)\$(NULL)" mkdir "$(PVM_ARCH)"
	if not exist "$(PVM_ROOT)\bin\$(NULL)" mkdir "$(PVM_ROOT)\bin"
	if not exist "$(PVM_ROOT)\bin\$(PVM_ARCH)\$(NULL)" \
		mkdir "$(PVM_ROOT)\bin\$(PVM_ARCH)"

timing:  $(PVM_ARCH)\timing.obj
	$(linkexe) $(conflags) \
		$(OUTBIN)$(PVM_ROOT)\bin\$(PVM_ARCH)\timing.exe \
		$(PVM_ARCH)\timing.obj $(LIB) $(GLIB) $(link_flags)

timing_slave:  $(PVM_ARCH)\timing_slave.obj
	$(linkexe) $(conflags) \
		$(OUTBIN)$(PVM_ROOT)\bin\$(PVM_ARCH)\timing_slave.exe \
		$(PVM_ARCH)\timing_slave.obj $(LIB) $(GLIB) $(link_flags)

gexample:  $(PVM_ARCH)\gexample.obj
	$(linkexe) $(conflags) \
		$(OUTBIN)$(PVM_ROOT)\bin\$(PVM_ARCH)\gexample.exe \
		$(PVM_ARCH)\gexample.obj $(LIB) $(GLIB) $(link_flags)

hello:  $(PVM_ARCH)\hello.obj 
	$(linkexe) $(conflags) \
		$(OUTBIN)$(PVM_ROOT)\bin\$(PVM_ARCH)\hello.exe \
		$(PVM_ARCH)\hello.obj $(LIB) $(link_flags)
 
hello_other:  $(PVM_ARCH)\hello_other.obj
	$(linkexe) $(conflags) \
		$(OUTBIN)$(PVM_ROOT)\bin\$(PVM_ARCH)\hello_other.exe \
		$(PVM_ARCH)\hello_other.obj $(LIB) $(link_flags)

mbox:  $(PVM_ARCH)\mbox.obj
	 $(linkexe) $(conflags) \
		$(OUTBIN)$(PVM_ROOT)\bin\$(PVM_ARCH)\mbox.exe \
		$(PVM_ARCH)\mbox.obj $(LIB) $(link_flags)

bwtest:  $(PVM_ARCH)\bwtest.obj
	$(linkexe) $(conflags) \
		$(OUTBIN)$(PVM_ROOT)\bin\$(PVM_ARCH)\bwtest.exe \
		$(PVM_ARCH)\bwtest.obj $(LIB) $(GLIB) $(link_flags)

master1:  $(PVM_ARCH)\master1.obj
	$(linkexe) $(conflags) \
		$(OUTBIN)$(PVM_ROOT)\bin\$(PVM_ARCH)\master1.exe \
		$(PVM_ARCH)\master1.obj $(LIB) $(link_flags)

slave1:  $(PVM_ARCH)\slave1.obj
	$(linkexe) $(conflags) \
		$(OUTBIN)$(PVM_ROOT)\bin\$(PVM_ARCH)\slave1.exe \
		$(PVM_ARCH)\slave1.obj $(LIB) $(link_flags)

spmd:  $(PVM_ARCH)\spmd.obj
	$(linkexe) $(conflags) \
		$(OUTBIN)$(PVM_ROOT)\bin\$(PVM_ARCH)\spmd.exe \
		$(PVM_ARCH)\spmd.obj $(LIB) $(GLIB) $(link_flags)

inherita:  $(PVM_ARCH)\inherita.obj
	$(linkexe) $(conflags) \
		$(OUTBIN)$(PVM_ROOT)\bin\$(PVM_ARCH)\inherita.exe \
		$(PVM_ARCH)\inherita.obj $(LIB) $(link_flags)

inheritb:  $(PVM_ARCH)\inheritb.obj
	$(linkexe) $(conflags) \
		$(OUTBIN)$(PVM_ROOT)\bin\$(PVM_ARCH)\inheritb.exe \
		$(PVM_ARCH)\inheritb.obj $(LIB) $(link_flags)

inherit1:  $(PVM_ARCH)\inherit1.obj
	$(linkexe) $(conflags) \
		$(OUTBIN)$(PVM_ROOT)\bin\$(PVM_ARCH)\inherit1.exe \
		$(PVM_ARCH)\inherit1.obj $(LIB) $(link_flags)

inherit2:  $(PVM_ARCH)\inherit2.obj
	$(linkexe) $(conflags) \
		$(OUTBIN)$(PVM_ROOT)\bin\$(PVM_ARCH)\inherit2.exe \
		$(PVM_ARCH)\inherit2.obj $(LIB) $(link_flags)

inherit3:  $(PVM_ARCH)\inherit3.obj
	$(linkexe) $(conflags) \
		$(OUTBIN)$(PVM_ROOT)\bin\$(PVM_ARCH)\inherit3.exe \
		$(PVM_ARCH)\inherit3.obj $(LIB) $(link_flags)

task0:  $(PVM_ARCH)\task0.obj $(PVM_ARCH)\taskf.obj 
	$(linkexe) $(conflags) \
		$(OUTBIN)$(PVM_ROOT)\bin\$(PVM_ARCH)\task0.exe \
		$(PVM_ARCH)\task0.obj $(PVM_ARCH)\taskf.obj $(LIB) $(link_flags)

task1:  $(PVM_ARCH)\task1.obj $(PVM_ARCH)\taskf.obj 
	$(linkexe) $(conflags) \
		$(OUTBIN)$(PVM_ROOT)\bin\$(PVM_ARCH)\task1.exe \
		$(PVM_ARCH)\task1.obj $(PVM_ARCH)\taskf.obj $(LIB) $(link_flags)

task_end:  $(PVM_ARCH)\task_end.obj
	$(linkexe) $(conflags) \
		$(OUTBIN)$(PVM_ROOT)\bin\$(PVM_ARCH)\task_end.exe \
		$(PVM_ARCH)\task_end.obj $(LIB) $(link_flags)

rme:  $(PVM_ARCH)\rme.obj
	$(linkexe) $(conflags) \
		$(OUTBIN)$(PVM_ROOT)\bin\$(PVM_ARCH)\rme.exe \
		$(PVM_ARCH)\rme.obj $(LIB) $(link_flags)

lmbi:  $(PVM_ARCH)\lmbi.obj $(PVM_ARCH)\taskf.obj 
	$(linkexe) $(conflags) \
		$(OUTBIN)$(PVM_ROOT)\bin\$(PVM_ARCH)\lmbi.exe \
		$(PVM_ARCH)\lmbi.obj $(PVM_ARCH)\taskf.obj $(LIB) $(link_flags)

gmbi:  $(PVM_ARCH)\gmbi.obj $(PVM_ARCH)\taskf.obj 
	$(linkexe) $(conflags) \
		$(OUTBIN)$(PVM_ROOT)\bin\$(PVM_ARCH)\gmbi.exe \
		$(PVM_ARCH)\gmbi.obj $(PVM_ARCH)\taskf.obj  $(LIB) $(link_flags)

mhf_server:  $(PVM_ARCH)\mhf_server.obj
	$(linkexe) $(conflags) \
		$(OUTBIN)$(PVM_ROOT)\bin\$(PVM_ARCH)\mhf_server.exe \
		$(PVM_ARCH)\mhf_server.obj $(LIB) $(link_flags)

mhf_tickle:  $(PVM_ARCH)\mhf_tickle.obj
	$(linkexe) $(conflags) \
		$(OUTBIN)$(PVM_ROOT)\bin\$(PVM_ARCH)\mhf_tickle.exe \
		$(PVM_ARCH)\mhf_tickle.obj $(LIB) $(link_flags)


#############
# C OBJS
#############

$(PVM_ARCH)\mbox.obj:  $(SDIR)\mbox.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
		$(OUT)$(PVM_ARCH)\mbox.obj $(SDIR)\mbox.c

$(PVM_ARCH)\bwtest.obj:  $(SDIR)\bwtest.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
		$(OUT)$(PVM_ARCH)\bwtest.obj $(SDIR)\bwtest.c

$(PVM_ARCH)\timing.obj:  $(SDIR)\timing.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
		$(OUT)$(PVM_ARCH)\timing.obj $(SDIR)\timing.c

$(PVM_ARCH)\timing_slave.obj:  $(SDIR)\timing_slave.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
		$(OUT)$(PVM_ARCH)\timing_slave.obj $(SDIR)\timing_slave.c

$(PVM_ARCH)\gexample.obj:  $(SDIR)\gexample.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
		$(OUT)$(PVM_ARCH)\gexample.obj $(SDIR)\gexample.c

$(PVM_ARCH)\hello.obj:  $(SDIR)\hello.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
		$(OUT)$(PVM_ARCH)\hello.obj $(SDIR)\hello.c

$(PVM_ARCH)\hello_other.obj:  $(SDIR)\hello_other.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
		$(OUT)$(PVM_ARCH)\hello_other.obj $(SDIR)\hello_other.c

$(PVM_ARCH)\master1.obj:  $(SDIR)\master1.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
		$(OUT)$(PVM_ARCH)\master1.obj $(SDIR)\master1.c

$(PVM_ARCH)\slave1.obj:  $(SDIR)\slave1.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
		$(OUT)$(PVM_ARCH)\slave1.obj $(SDIR)\slave1.c

$(PVM_ARCH)\spmd.obj:  $(SDIR)\spmd.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
		$(OUT)$(PVM_ARCH)\spmd.obj $(SDIR)\spmd.c

$(PVM_ARCH)\taskf.obj:  $(SDIR)\taskf.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
		$(OUT)$(PVM_ARCH)\taskf.obj $(SDIR)\taskf.c

$(PVM_ARCH)\inherita.obj:  $(SDIR)\inherita.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
		$(OUT)$(PVM_ARCH)\inherita.obj $(SDIR)\inherita.c

$(PVM_ARCH)\inheritb.obj:  $(SDIR)\inheritb.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
		$(OUT)$(PVM_ARCH)\inheritb.obj $(SDIR)\inheritb.c

$(PVM_ARCH)\inherit1.obj:  $(SDIR)\inherit1.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
		$(OUT)$(PVM_ARCH)\inherit1.obj $(SDIR)\inherit1.c

$(PVM_ARCH)\inherit2.obj:  $(SDIR)\inherit2.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
		$(OUT)$(PVM_ARCH)\inherit2.obj $(SDIR)\inherit2.c

$(PVM_ARCH)\inherit3.obj:  $(SDIR)\inherit3.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
		$(OUT)$(PVM_ARCH)\inherit3.obj $(SDIR)\inherit3.c

$(PVM_ARCH)\task0.obj:  $(SDIR)\task0.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
		$(OUT)$(PVM_ARCH)\task0.obj $(SDIR)\task0.c

$(PVM_ARCH)\task1.obj:  $(SDIR)\task1.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
		$(OUT)$(PVM_ARCH)\task1.obj $(SDIR)\task1.c

$(PVM_ARCH)\task_end.obj:  $(SDIR)\task_end.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
		$(OUT)$(PVM_ARCH)\task_end.obj $(SDIR)\task_end.c

$(PVM_ARCH)\rme.obj:  $(SDIR)\rme.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
		$(OUT)$(PVM_ARCH)\rme.obj $(SDIR)\rme.c

$(PVM_ARCH)\lmbi.obj:  $(SDIR)\lmbi.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
		$(OUT)$(PVM_ARCH)\lmbi.obj $(SDIR)\lmbi.c

$(PVM_ARCH)\gmbi.obj:  $(SDIR)\gmbi.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
		$(OUT)$(PVM_ARCH)\gmbi.obj $(SDIR)\gmbi.c

$(PVM_ARCH)\mhf_server.obj:  $(SDIR)\mhf_server.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
		$(OUT)$(PVM_ARCH)\mhf_server.obj $(SDIR)\mhf_server.c

$(PVM_ARCH)\mhf_tickle.obj:  $(SDIR)\mhf_tickle.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
		$(OUT)$(PVM_ARCH)\mhf_tickle.obj $(SDIR)\mhf_tickle.c


#############################################################
# FORTRAN
#############################################################

fspmd:  $(PVM_ARCH)\spmdf.obj
	$(linkexe) $(conflags) $(FLIBPATH) \
		$(OUTBIN)$(PVM_ROOT)\bin\$(PVM_ARCH)\spmdf.exe \
		$(PVM_ARCH)\spmdf.obj $(LIB) $(GLIB) $(FLIB) $(link_flags)

fmaster1:  $(PVM_ARCH)\master1f.obj
	$(linkexe) $(conflags) $(FLIBPATH) \
		$(OUTBIN)$(PVM_ROOT)\bin\$(PVM_ARCH)\fmaster1.exe \
		$(PVM_ARCH)\master1f.obj $(LIB) $(GLIB) $(FLIB) $(link_flags) 

fslave1:  $(PVM_ARCH)\slave1f.obj
	$(linkexe) $(conflags) $(FLIBPATH) \
		$(OUTBIN)$(PVM_ROOT)\bin\$(PVM_ARCH)\fslave1.exe \
		$(PVM_ARCH)\slave1f.obj $(LIB) $(GLIB) $(FLIB) $(link_flags)

fgexample:  $(PVM_ARCH)\gexamplef.obj
	$(linkexe) $(conflags) $(FLIBPATH) \
		$(OUTBIN)$(PVM_ROOT)\bin\$(PVM_ARCH)\fgexample.exe \
		$(PVM_ARCH)\gexamplef.obj $(LIB) $(GLIB) $(FLIB) $(link_flags)

hitc:  $(PVM_ARCH)\hitc.obj
	$(linkexe) $(conflags) $(FLIBPATH) \
		$(OUTBIN)$(PVM_ROOT)\bin\$(PVM_ARCH)\hitc.exe \
		$(PVM_ARCH)\hitc.obj $(LIB) $(GLIB) $(FLIB) $(link_flags)

hitc_slave:  $(PVM_ARCH)\hitc_slave.obj
	$(linkexe) $(conflags) $(FLIBPATH) \
		$(OUTBIN)$(PVM_ROOT)\bin\$(PVM_ARCH)\hitc_slave.exe \
		$(PVM_ARCH)\hitc_slave.obj $(LIB) $(GLIB) $(FLIB) $(link_flags)


##############
# FORTRAN OBJS
##############

$(PVM_ARCH)\hitc.obj:  $(SDIR)\hitc.f
	$(fc) $(cdebug) /Fo$(PVM_ARCH)\hitc.obj $(fflags) $(cvars) \
		$(SDIR)\hitc.f

$(PVM_ARCH)\hitc_slave.obj:  $(SDIR)\hitc_slave.f
	$(fc) $(cdebug) /Fo$(PVM_ARCH)\hitc_slave.obj $(fflags) $(cvars) \
		$(SDIR)\hitc_slave.f

$(PVM_ARCH)\spmdf.obj:  $(SDIR)\spmd.f
	$(fc) $(cdebug) /Fo$(PVM_ARCH)\spmdf.obj $(fflags) $(cvars) \
		$(SDIR)\spmd.f

$(PVM_ARCH)\gexamplef.obj:  $(SDIR)\gexample.f
	$(fc) $(cdebug) /Fo$(PVM_ARCH)\gexamplef.obj $(fflags) $(cvars) \
		$(SDIR)\gexample.f

$(PVM_ARCH)\slave1f.obj:  $(SDIR)\slave1.f
	$(fc) $(cdebug) /Fo$(PVM_ARCH)\slave1f.obj $(fflags) $(cvars) \
		$(SDIR)\slave1.f

$(PVM_ARCH)\master1f.obj:  $(SDIR)\master1.f
	$(fc) $(cdebug) /Fo$(PVM_ARCH)\master1f.obj $(fflags) $(cvars) \
		$(SDIR)\master1.f

# Clean all Fortran example codes - .OBJ and .EXE
tidy-f:
	-del $(PVM_ARCH)\spmdf.obj
	-del $(PVM_ARCH)\master1f.obj
	-del $(PVM_ARCH)\slave1f.obj
	-del $(PVM_ARCH)\gexamplef.obj
	-del $(PVM_ARCH)\hitc.obj
	-del $(PVM_ARCH)\hitc_slave.obj

	-del $(PVM_ROOT)\bin\$(PVM_ARCH)\spmdf.exe
	-del $(PVM_ROOT)\bin\$(PVM_ARCH)\fmaster1.exe
	-del $(PVM_ROOT)\bin\$(PVM_ARCH)\fslave1.exe
	-del $(PVM_ROOT)\bin\$(PVM_ARCH)\fgexample.exe
	-del $(PVM_ROOT)\bin\$(PVM_ARCH)\hitc.exe
	-del $(PVM_ROOT)\bin\$(PVM_ARCH)\hitc_slave.exe

# Clean all C example codes - .OBJ and .EXE
tidy-c:
	-del $(PVM_ARCH)\timing.obj
	-del $(PVM_ARCH)\timing_slave.obj
	-del $(PVM_ARCH)\gexample.obj
	-del $(PVM_ARCH)\hello.obj 
	-del $(PVM_ARCH)\hello_other.obj
	-del $(PVM_ARCH)\mbox.obj
	-del $(PVM_ARCH)\bwtest.obj
	-del $(PVM_ARCH)\master1.obj
	-del $(PVM_ARCH)\slave1.obj
	-del $(PVM_ARCH)\spmd.obj
	-del $(PVM_ARCH)\inherita.obj
	-del $(PVM_ARCH)\inheritb.obj
	-del $(PVM_ARCH)\inherit1.obj
	-del $(PVM_ARCH)\inherit2.obj
	-del $(PVM_ARCH)\inherit3.obj
	-del $(PVM_ARCH)\task0.obj
	-del $(PVM_ARCH)\taskf.obj 
	-del $(PVM_ARCH)\task1.obj
	-del $(PVM_ARCH)\task_end.obj
	-del $(PVM_ARCH)\rme.obj
	-del $(PVM_ARCH)\lmbi.obj
	-del $(PVM_ARCH)\gmbi.obj
	-del $(PVM_ARCH)\mhf_server.obj
	-del $(PVM_ARCH)\mhf_tickle.obj

	-del $(PVM_ROOT)\bin\$(PVM_ARCH)\timing.exe
	-del $(PVM_ROOT)\bin\$(PVM_ARCH)\timing_slave.exe
	-del $(PVM_ROOT)\bin\$(PVM_ARCH)\gexample.exe
	-del $(PVM_ROOT)\bin\$(PVM_ARCH)\hello.exe
	-del $(PVM_ROOT)\bin\$(PVM_ARCH)\hello_other.exe
	-del $(PVM_ROOT)\bin\$(PVM_ARCH)\mbox.exe
	-del $(PVM_ROOT)\bin\$(PVM_ARCH)\bwtest.exe
	-del $(PVM_ROOT)\bin\$(PVM_ARCH)\master1.exe
	-del $(PVM_ROOT)\bin\$(PVM_ARCH)\slave1.exe
	-del $(PVM_ROOT)\bin\$(PVM_ARCH)\spmd.exe
	-del $(PVM_ROOT)\bin\$(PVM_ARCH)\inherita.exe	
	-del $(PVM_ROOT)\bin\$(PVM_ARCH)\inheritb.exe
	-del $(PVM_ROOT)\bin\$(PVM_ARCH)\inherit1.exe
	-del $(PVM_ROOT)\bin\$(PVM_ARCH)\inherit2.exe
	-del $(PVM_ROOT)\bin\$(PVM_ARCH)\inherit3.exe
	-del $(PVM_ROOT)\bin\$(PVM_ARCH)\task0.exe
	-del $(PVM_ROOT)\bin\$(PVM_ARCH)\task1.exe
	-del $(PVM_ROOT)\bin\$(PVM_ARCH)\task_end.exe
	-del $(PVM_ROOT)\bin\$(PVM_ARCH)\rme.exe
	-del $(PVM_ROOT)\bin\$(PVM_ARCH)\lmbi.exe
	-del $(PVM_ROOT)\bin\$(PVM_ARCH)\gmbi.exe
	-del $(PVM_ROOT)\bin\$(PVM_ARCH)\mhf_server.exe
	-del $(PVM_ROOT)\bin\$(PVM_ARCH)\mhf_tickle.exe

# Clean up everything but the .EXEs
clean:
	-del *.obj
	-del *.pdb

