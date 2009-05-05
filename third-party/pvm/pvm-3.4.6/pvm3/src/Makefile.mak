#*************************************************************#
#
# $Id: Makefile.mak,v 1.11 1999/12/10 21:30:25 pvmsrc Exp $
#
#  (N)make file for the daemon and pvm library
#
#  PVM_ARCH   = the official pvm-name of your processor
#  ARCHCFLAGS = special cc flags
#  ARCHLIB    = special libs needed for daemon
#

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=
!ENDIF 

!include $(PVM_ROOT)\conf\$(PVM_ARCH).def

SDIR	=	$(PVM_ROOT)\src

# "ddpro.c" "host.c" "imalloc.c" "msgbox.c" "pkt.c" "pmsg.c"
# "pvmalloc.c" "pvmcruft.c" "pvmd.c" "pvmdabuf.c" "pvmdpack.c"
# "pvmdunix.c" "pvmerr.c" "pvmfrag.c" "pvmlog.c" "pvmwin.c" "sdpro.c"
# "task.c" "tdpro.c" "waitc.c" "pvmdtev.c" "regex\pvmregex.c"
# "regex\regex.c" "global.c"
# "..\xdr\xdr.c" "..\xdr\xdr_mem.c" "..\xdr\xdr_floa.c"

#
# GNU Regex Defines - adjust if needed
#

REGEXOBJS	=
REGEXOBJS	=	$(PVM_ARCH)\pvmregex.obj $(PVM_ARCH)\regex.obj

#
# PVM Daemon & Library Objects
#

DOBJ	=	$(PVM_ARCH)\ddpro.obj $(PVM_ARCH)\host.obj \
			$(PVM_ARCH)\imalloc.obj $(PVM_ARCH)\msgbox.obj \
			$(PVM_ARCH)\pkt.obj $(PVM_ARCH)\pmsg.obj \
			$(PVM_ARCH)\pvmalloc.obj $(PVM_ARCH)\pvmcruft.obj \
			$(PVM_ARCH)\pvmd.obj $(PVM_ARCH)\pvmdabuf.obj \
			$(PVM_ARCH)\pvmdpack.obj $(PVM_ARCH)\pvmdunix.obj \
			$(PVM_ARCH)\pvmerr.obj $(PVM_ARCH)\pvmfrag.obj \
			$(PVM_ARCH)\pvmlog.obj $(PVM_ARCH)\pvmwin.obj \
			$(PVM_ARCH)\sdpro.obj $(PVM_ARCH)\task.obj \
			$(PVM_ARCH)\tdpro.obj $(PVM_ARCH)\waitc.obj \
			$(PVM_ARCH)\pvmdtev.obj $(PVM_ARCH)\global.obj \
			$(PVM_ARCH)\xdr.obj $(PVM_ARCH)\xdr_mem.obj \
			$(PVM_ARCH)\xdr_floa.obj $(REGEXOBJS)

LOBJ	=	$(PVM_ARCH)\imalloc.obj $(PVM_ARCH)\lpvm.obj \
			$(PVM_ARCH)\lpvmcat.obj $(PVM_ARCH)\lpvmgen.obj \
			$(PVM_ARCH)\tev.obj $(PVM_ARCH)\lpvmpack.obj \
			$(PVM_ARCH)\pvmalloc.obj $(PVM_ARCH)\pmsg.obj \
			$(PVM_ARCH)\pvmcruft.obj $(PVM_ARCH)\pvmdabuf.obj \
			$(PVM_ARCH)\pvmfrag.obj $(PVM_ARCH)\global.obj \
			$(PVM_ARCH)\pvmerr.obj $(PVM_ARCH)\lpvmglob.obj \
			$(PVM_ARCH)\pvmwin.obj $(PVM_ARCH)\xdr.obj \
			$(PVM_ARCH)\xdr_floa.obj $(PVM_ARCH)\xdr_mem.obj \
			$(PVM_ARCH)\waitc.obj \
			$(PVM_ARCH)\pvmdpack.obj

all:	paths libpvm3.lib $(pvmd3exe) $(libpvm3dll)

paths:
	if not exist "..\lib\$(PVM_ARCH)\$(NULL)" mkdir "..\lib\$(PVM_ARCH)"
	if not exist "..\bin\$(PVM_ARCH)\$(NULL)" mkdir "..\bin\$(PVM_ARCH)"
	if not exist "$(PVM_ARCH)\$(NULL)"   mkdir "$(PVM_ARCH)"

libpvm3.lib:  $(LOBJ)
	$(linklib) $(conflags) \
		$(OUTLIB)$(PVM_ROOT)\lib\$(PVM_ARCH)\libpvm3.lib $(LOBJ)

libpvm3.dll:  $(LOBJ)
	$(linkdll) $(conflags) \
		$(OUTDLL)$(PVM_ROOT)\lib\$(PVM_ARCH)\libpvm3.dll $(LOBJ) \
		$(link_flags)

visual_pvmd3.exe:  $(DOBJ)
	$(linkexe) $(conflags) \
		$(OUTBIN)$(PVM_ROOT)\lib\win32\pvmd3.exe \
		$(FILE) $(BEGIN) $(DOBJ) $(END) $(link_flags)	

$(PVM_ARCH)\imalloc.obj:  $(SDIR)\imalloc.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
		$(OUT)$(PVM_ROOT)\src\$(PVM_ARCH)\imalloc.obj $(SDIR)\imalloc.c
$(PVM_ARCH)\msgbox.obj:  $(SDIR)\msgbox.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
		$(OUT)$(PVM_ROOT)\src\$(PVM_ARCH)\msgbox.obj $(SDIR)\msgbox.c
$(PVM_ARCH)\pmsg.obj:  $(SDIR)\pmsg.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
		$(OUT)$(PVM_ROOT)\src\$(PVM_ARCH)\pmsg.obj $(SDIR)\pmsg.c
$(PVM_ARCH)\pkt.obj:  $(SDIR)\pkt.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
		$(OUT)$(PVM_ROOT)\src\$(PVM_ARCH)\pkt.obj $(SDIR)\pkt.c
$(PVM_ARCH)\pvmalloc.obj:  $(SDIR)\pvmalloc.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
		$(OUT)$(PVM_ROOT)\src\$(PVM_ARCH)\pvmalloc.obj \
		$(SDIR)\pvmalloc.c
$(PVM_ARCH)\pvmdpack.obj:  $(SDIR)\pvmdpack.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
		$(OUT)$(PVM_ROOT)\src\$(PVM_ARCH)\pvmdpack.obj \
		$(SDIR)\pvmdpack.c
$(PVM_ARCH)\pvmerr.obj:  $(SDIR)\pvmerr.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
		$(OUT)$(PVM_ROOT)\src\$(PVM_ARCH)\pvmerr.obj $(SDIR)\pvmerr.c
$(PVM_ARCH)\pvmlog.obj:  $(SDIR)\pvmlog.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
		$(OUT)$(PVM_ROOT)\src\$(PVM_ARCH)\pvmlog.obj $(SDIR)\pvmlog.c
$(PVM_ARCH)\pvmdabuf.obj:  $(SDIR)\pvmdabuf.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
		$(OUT)$(PVM_ROOT)\src\$(PVM_ARCH)\pvmdabuf.obj \
		$(SDIR)\pvmdabuf.c
$(PVM_ARCH)\host.obj:  $(SDIR)\host.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
		$(OUT)$(PVM_ROOT)\src\$(PVM_ARCH)\host.obj $(SDIR)\host.c
$(PVM_ARCH)\pvmfrag.obj:  $(SDIR)\pvmfrag.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
		$(OUT)$(PVM_ROOT)\src\$(PVM_ARCH)\pvmfrag.obj $(SDIR)\pvmfrag.c
$(PVM_ARCH)\lpvmglob.obj:  $(SDIR)\lpvmglob.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
		$(OUT)$(PVM_ROOT)\src\$(PVM_ARCH)\lpvmglob.obj \
		$(SDIR)\lpvmglob.c
$(PVM_ARCH)\pvmcruft.obj:  $(SDIR)\pvmcruft.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
		$(OUT)$(PVM_ROOT)\src\$(PVM_ARCH)\pvmcruft.obj \
		$(SDIR)\pvmcruft.c
$(PVM_ARCH)\pvmdunix.obj:  $(SDIR)\pvmdunix.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
		$(OUT)$(PVM_ROOT)\src\$(PVM_ARCH)\pvmdunix.obj \
		$(SDIR)\pvmdunix.c
$(PVM_ARCH)\pvmwin.obj:  $(SDIR)\pvmwin.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
		$(OUT)$(PVM_ROOT)\src\$(PVM_ARCH)\pvmwin.obj $(SDIR)\pvmwin.c
$(PVM_ARCH)\sdpro.obj:  $(SDIR)\sdpro.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
		$(OUT)$(PVM_ROOT)\src\$(PVM_ARCH)\sdpro.obj $(SDIR)\sdpro.c
$(PVM_ARCH)\global.obj:  $(SDIR)\global.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
		$(OUT)$(PVM_ROOT)\src\$(PVM_ARCH)\global.obj $(SDIR)\global.c
$(PVM_ARCH)\task.obj:  $(SDIR)\task.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
		$(OUT)$(PVM_ROOT)\src\$(PVM_ARCH)\task.obj $(SDIR)\task.c
$(PVM_ARCH)\tdpro.obj:  $(SDIR)\tdpro.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
		$(OUT)$(PVM_ROOT)\src\$(PVM_ARCH)\tdpro.obj $(SDIR)\tdpro.c
$(PVM_ARCH)\ddpro.obj:  $(SDIR)\ddpro.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
		$(OUT)$(PVM_ROOT)\src\$(PVM_ARCH)\ddpro.obj $(SDIR)\ddpro.c
$(PVM_ARCH)\pvmdtev.obj:  $(SDIR)\pvmdtev.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
		$(OUT)$(PVM_ROOT)\src\$(PVM_ARCH)\pvmdtev.obj $(SDIR)\pvmdtev.c
$(PVM_ARCH)\pvmd.obj:  $(SDIR)\pvmd.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
		$(OUT)$(PVM_ROOT)\src\$(PVM_ARCH)\pvmd.obj $(SDIR)\pvmd.c
$(PVM_ARCH)\pvmregex.obj:  $(SDIR)\regex\pvmregex.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
		$(OUT)$(PVM_ROOT)\src\$(PVM_ARCH)\pvmregex.obj \
		$(SDIR)\regex\pvmregex.c
$(PVM_ARCH)\regex.obj:  $(SDIR)\regex\regex.c
	$(cc) $(cdebug) $(cflags) $(cvars) -DREGEX_MALLOC -DHAVE_STRING_H \
		$(OUT)$(PVM_ROOT)\src\$(PVM_ARCH)\regex.obj \
		$(SDIR)\regex\regex.c
$(PVM_ARCH)\waitc.obj:  $(SDIR)\waitc.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
		$(OUT)$(PVM_ROOT)\src\$(PVM_ARCH)\waitc.obj $(SDIR)\waitc.c
$(PVM_ARCH)\tev.obj:  $(SDIR)\tev.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
		$(OUT)$(PVM_ROOT)\src\$(PVM_ARCH)\tev.obj $(SDIR)\tev.c
$(PVM_ARCH)\lpvm.obj:  $(SDIR)\lpvm.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
		$(OUT)$(PVM_ROOT)\src\$(PVM_ARCH)\lpvm.obj $(SDIR)\lpvm.c
$(PVM_ARCH)\lpvmcat.obj:  $(SDIR)\lpvmcat.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
		$(OUT)$(PVM_ROOT)\src\$(PVM_ARCH)\lpvmcat.obj $(SDIR)\lpvmcat.c
$(PVM_ARCH)\lpvmgen.obj:  $(SDIR)\lpvmgen.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
		$(OUT)$(PVM_ROOT)\src\$(PVM_ARCH)\lpvmgen.obj $(SDIR)\lpvmgen.c
$(PVM_ARCH)\lpvmpack.obj:  $(SDIR)\lpvmpack.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
		$(OUT)$(PVM_ROOT)\src\$(PVM_ARCH)\lpvmpack.obj \
		$(SDIR)\lpvmpack.c
$(PVM_ARCH)\xdr_mem.obj:  $(PVM_ROOT)\xdr\xdr_mem.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
		$(OUT)$(PVM_ROOT)\src\$(PVM_ARCH)\xdr_mem.obj \
		$(PVM_ROOT)\xdr\xdr_mem.c
$(PVM_ARCH)\xdr_floa.obj:  $(PVM_ROOT)\xdr\xdr_floa.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
		$(OUT)$(PVM_ROOT)\src\$(PVM_ARCH)\xdr_floa.obj \
		$(PVM_ROOT)\xdr\xdr_floa.c
$(PVM_ARCH)\xdr.obj:  $(PVM_ROOT)\xdr\xdr.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
		$(OUT)$(PVM_ROOT)\src\$(PVM_ARCH)\xdr.obj $(PVM_ROOT)\xdr\xdr.c
$(PVM_ARCH)\pvmarchc.obj:  $(SDIR)\pvmarchc.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
		$(OUT)$(PVM_ROOT)\src\$(PVM_ARCH)\pvmarchc.obj \
		$(SDIR)\pvmarchc.c
$(PVM_ARCH)\pvmumbuf.obj:  $(SDIR)\pvmumbuf.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
		$(OUT)$(PVM_ROOT)\src\$(PVM_ARCH)\pvmumbuf.obj \
		$(SDIR)\pvmumbuf.c

# Clean up everything but the .EXEs
clean :
	-del $(PVM_ARCH)\*.obj

