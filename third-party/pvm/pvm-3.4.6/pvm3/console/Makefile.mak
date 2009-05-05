#
# $Id: Makefile.mak,v 1.8 1999/03/05 17:25:50 pvmsrc Exp $
#
#  Nmake file for the PVM console: pvm.exe
#
#  PVM_ARCH   = the official pvm-name of your processor
#  ARCHCFLAGS = special cc flags
#  ARCHLIB    = special libs needed for daemon
#

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE
NULL=nul
!ENDIF

!include $(PVM_ROOT)\conf\$(PVM_ARCH).def

OBJS	=	$(PVM_ARCH)\cmds.obj $(PVM_ARCH)\cons.obj \
			$(PVM_ARCH)\job.obj $(PVM_ARCH)\trc.obj

LIBS	=	$(PVM_ROOT)\lib\$(PVM_ARCH)\libpvm3.lib \
			$(PVM_ROOT)\tracer\$(PVM_ARCH)\libpvmtracer.lib

all:  paths pvm.exe

#

paths:
	if not exist $(PVM_ARCH) mkdir $(PVM_ARCH)
	if not exist "..\bin\$(PVM_ARCH)\$(NULL)" mkdir "..\bin\$(PVM_ARCH)"

pvm.exe:  $(OBJS)
	$(linkexe) $(conflags) \
		$(OUTBIN)$(PVM_ROOT)\lib\$(PVM_ARCH)\pvm.exe \
		$(OBJS) $(LIBS) $(link_flags)

$(PVM_ARCH)\cmds.obj:  $(PVM_ROOT)\console\cmds.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
		$(OUT)$(PVM_ROOT)\console\$(PVM_ARCH)\cmds.obj \
		$(PVM_ROOT)\console\cmds.c
$(PVM_ARCH)\cons.obj:  $(PVM_ROOT)\console\cons.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
		$(OUT)$(PVM_ROOT)\console\$(PVM_ARCH)\cons.obj \
		$(PVM_ROOT)\console\cons.c
$(PVM_ARCH)\trc.obj:  $(PVM_ROOT)\console\trc.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
		$(OUT)$(PVM_ROOT)\console\$(PVM_ARCH)\trc.obj \
		$(PVM_ROOT)\console\trc.c
$(PVM_ARCH)\job.obj:  $(PVM_ROOT)\console\job.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
		$(OUT)$(PVM_ROOT)\console\$(PVM_ARCH)\job.obj \
		$(PVM_ROOT)\console\job.c

# Clean up everything but the .EXEs

clean:
    -del $(PVM_ARCH)\*.obj
    -del $(PVM_ARCH)\*.pdb

