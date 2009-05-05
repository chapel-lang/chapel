#
# $Id: Makefile.mak,v 1.6 1999/03/15 19:05:13 pvmsrc Exp $
#

#*************************************************************#
#**                                                         **#
#**      Nmake file for libfpvm.lib                        	**#
#**                   libfpvm.lib                           **#
#**                                                         **#
#**                                                         **#
#*************************************************************#

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE
NULL=nul
!ENDIF

!include $(PVM_ROOT)\conf\$(PVM_ARCH).def

# PVM_ROOT is set during installation in WIN32.def !

SDIR	=	$(PVM_ROOT)\libfpvm\win32

LOBS    =       $(PVM_ARCH)\pvmfaddhost.obj \
                        $(PVM_ARCH)\pvmfaddmhf.obj \
                        $(PVM_ARCH)\pvmfarchcode.obj \
                        $(PVM_ARCH)\pvmfbarrier.obj \
                        $(PVM_ARCH)\pvmfbcast.obj \
                        $(PVM_ARCH)\pvmfbufinfo.obj \
                        $(PVM_ARCH)\pvmfcatchout.obj \
                        $(PVM_ARCH)\pvmfconfig.obj \
                        $(PVM_ARCH)\pvmfdelhost.obj \
                        $(PVM_ARCH)\pvmfdelinfo.obj \
                        $(PVM_ARCH)\pvmfdelmhf.obj \
                        $(PVM_ARCH)\pvmfexit.obj \
                        $(PVM_ARCH)\pvmffreebuf.obj \
                        $(PVM_ARCH)\pvmffreectx.obj \
                        $(PVM_ARCH)\pvmffrzgrp.obj \
                        $(PVM_ARCH)\pvmfgather.obj \
                        $(PVM_ARCH)\pvmfgetctx.obj \
                        $(PVM_ARCH)\pvmfgetinfo.obj \
                        $(PVM_ARCH)\pvmfgetinst.obj \
                        $(PVM_ARCH)\pvmfgetopt.obj \
                        $(PVM_ARCH)\pvmfgetrbuf.obj \
                        $(PVM_ARCH)\pvmfgetsbuf.obj \
                        $(PVM_ARCH)\pvmfgettid.obj \
                        $(PVM_ARCH)\pvmfgsize.obj \
                        $(PVM_ARCH)\pvmfhalt.obj \
                        $(PVM_ARCH)\pvmfhostsync.obj \
                        $(PVM_ARCH)\pvmfinitsend.obj \
                        $(PVM_ARCH)\pvmfjoingrp.obj \
                        $(PVM_ARCH)\pvmfkill.obj \
                        $(PVM_ARCH)\pvmflvgrp.obj \
                        $(PVM_ARCH)\pvmfmcast.obj \
                        $(PVM_ARCH)\pvmfmkbuf.obj \
                        $(PVM_ARCH)\pvmfmstat.obj \
                        $(PVM_ARCH)\pvmfmytid.obj \
                        $(PVM_ARCH)\pvmfnewctx.obj \
                        $(PVM_ARCH)\pvmfnotify.obj \
                        $(PVM_ARCH)\pvmfnrecv.obj \
                        $(PVM_ARCH)\pvmfpack.obj \
                        $(PVM_ARCH)\pvmfparent.obj \
                        $(PVM_ARCH)\pvmfperror.obj \
                        $(PVM_ARCH)\pvmfprecv.obj \
                        $(PVM_ARCH)\pvmfprobe.obj \
                        $(PVM_ARCH)\pvmfpsend.obj \
                        $(PVM_ARCH)\pvmfpstat.obj \
                        $(PVM_ARCH)\pvmfputinfo.obj \
                        $(PVM_ARCH)\pvmfrecv.obj \
                        $(PVM_ARCH)\pvmfrecvinfo.obj \
                        $(PVM_ARCH)\pvmfreduce.obj \
                        $(PVM_ARCH)\pvmfscatter.obj \
                        $(PVM_ARCH)\pvmfsend.obj \
                        $(PVM_ARCH)\pvmfsendsig.obj \
                        $(PVM_ARCH)\pvmfsetctx.obj \
                        $(PVM_ARCH)\pvmfsetopt.obj \
                        $(PVM_ARCH)\pvmfsetrbuf.obj \
                        $(PVM_ARCH)\pvmfsetsbuf.obj \
                        $(PVM_ARCH)\pvmfsiblings.obj \
                        $(PVM_ARCH)\pvmfsleep.obj \
                        $(PVM_ARCH)\pvmfspawn.obj \
                        $(PVM_ARCH)\pvmfstartpvmd.obj \
                        $(PVM_ARCH)\pvmftasks.obj \
                        $(PVM_ARCH)\pvmftidtoh.obj \
                        $(PVM_ARCH)\pvmftrecv.obj \
                        $(PVM_ARCH)\pvmfunpack.obj \


all:	libfpvm3.dll libfpvm3.lib

libfpvm3.lib:  $(LOBS) $(PVM_ARCH)\ftocstr.obj
        $(linklib) $(libspec) $(conflags) \
		$(OUTBIN)$(PVM_ROOT)\libfpvm\$(PVM_ARCH)\libfpvm3.lib \
                $(LOBS) $(PVM_ARCH)\ftocstr.obj
        copy $(PVM_ARCH)\libfpvm3.lib $(PVM_ROOT)\lib\$(PVM_ARCH)

                


libfpvm3.dll:  $(LOBS) $(PVM_ARCH)\ftocstr.obj
        $(linkdll) $(dllspec) $(conflags) \
		$(OUTBIN)$(PVM_ROOT)\libfpvm\$(PVM_ARCH)\libfpvm3.dll \
                $(LOBS) $(PVM_ARCH)\ftocstr.obj \
                $(PVM_CLIBDIR)\oldnames.lib \
                $(PVM_CLIBDIR)\kernel32.lib \
                $(PVM_CLIBDIR)\libcpmtd.lib \
                $(PVM_CLIBDIR)\wsock32.lib \
                $(PVM_CLIBDIR)\libc.lib \
                $(PVM_CLIBDIR)\gdi32.lib $(PVM_CLIBDIR)\winspool.lib \
                $(PVM_CLIBDIR)\comdlg32.lib \
                $(PVM_CLIBDIR)\advapi32.lib \
                $(PVM_CLIBDIR)\shell32.lib  \
                $(PVM_CLIBDIR)\ole32.lib \
                $(PVM_CLIBDIR)\oleaut32.lib \
                $(PVM_CLIBDIR)\uuid.lib \
                $(PVM_CLIBDIR)\oldnames.lib \
		..\lib\WIN32\libpvm3.lib \
		..\lib\WIN32\libgpvm3.lib \
		/DEBUG /nodefaultlib:"LIBCMTD.LIB"
        copy $(PVM_ARCH)\libfpvm3.dll $(PVM_ROOT)\lib\$(PVM_ARCH)
	
		

$(PVM_ARCH)\ftocstr.obj:  $(SDIR)\ftocstr.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
                $(OUT)$(PVM_ARCH)\ftocstr.obj $(SDIR)\ftocstr.c
$(PVM_ARCH)\pvmfaddhost.obj:  $(SDIR)\pvmfaddhost.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
                $(OUT)$(PVM_ARCH)\pvmfaddhost.obj $(SDIR)\pvmfaddhost.c
$(PVM_ARCH)\pvmfaddmhf.obj:  $(SDIR)\pvmfaddmhf.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
                $(OUT)$(PVM_ARCH)\pvmfaddmhf.obj $(SDIR)\pvmfaddmhf.c
$(PVM_ARCH)\pvmfarchcode.obj:  $(SDIR)\pvmfarchcode.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
                $(OUT)$(PVM_ARCH)\pvmfarchcode.obj $(SDIR)\pvmfarchcode.c
$(PVM_ARCH)\pvmfbarrier.obj:  $(SDIR)\pvmfbarrier.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
                $(OUT)$(PVM_ARCH)\pvmfbarrier.obj $(SDIR)\pvmfbarrier.c
$(PVM_ARCH)\pvmfbcast.obj:  $(SDIR)\pvmfbcast.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
                $(OUT)$(PVM_ARCH)\pvmfbcast.obj $(SDIR)\pvmfbcast.c
$(PVM_ARCH)\pvmfbufinfo.obj:  $(SDIR)\pvmfbufinfo.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
                $(OUT)$(PVM_ARCH)\pvmfbufinfo.obj $(SDIR)\pvmfbufinfo.c
$(PVM_ARCH)\pvmfcatchout.obj:  $(SDIR)\pvmfcatchout.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
                $(OUT)$(PVM_ARCH)\pvmfcatchout.obj $(SDIR)\pvmfcatchout.c
$(PVM_ARCH)\pvmfconfig.obj:  $(SDIR)\pvmfconfig.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
                $(OUT)$(PVM_ARCH)\pvmfconfig.obj $(SDIR)\pvmfconfig.c
$(PVM_ARCH)\pvmfdelhost.obj:  $(SDIR)\pvmfdelhost.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
                $(OUT)$(PVM_ARCH)\pvmfdelhost.obj $(SDIR)\pvmfdelhost.c
$(PVM_ARCH)\pvmfdelinfo.obj:  $(SDIR)\pvmfdelinfo.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
                $(OUT)$(PVM_ARCH)\pvmfdelinfo.obj $(SDIR)\pvmfdelinfo.c
$(PVM_ARCH)\pvmfdelmhf.obj:  $(SDIR)\pvmfdelmhf.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
                $(OUT)$(PVM_ARCH)\pvmfdelmhf.obj  $(SDIR)\pvmfdelmhf.c
$(PVM_ARCH)\pvmfexit.obj:  $(SDIR)\pvmfexit.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
                $(OUT)$(PVM_ARCH)\pvmfexit.obj $(SDIR)\pvmfexit.c
$(PVM_ARCH)\pvmffreebuf.obj:  $(SDIR)\pvmffreebuf.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
                $(OUT)$(PVM_ARCH)\pvmffreebuf.obj $(SDIR)\pvmffreebuf.c
$(PVM_ARCH)\pvmffreectx.obj:  $(SDIR)\pvmffreectx.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
                $(OUT)$(PVM_ARCH)\pvmffreectx.obj  $(SDIR)\pvmffreectx.c
$(PVM_ARCH)\pvmffrzgrp.obj:  $(SDIR)\pvmffrzgrp.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
                $(OUT)$(PVM_ARCH)\pvmffrzgrp.obj $(SDIR)\pvmffrzgrp.c
$(PVM_ARCH)\pvmfgather.obj:  $(SDIR)\pvmfgather.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
                $(OUT)$(PVM_ARCH)\pvmfgather.obj $(SDIR)\pvmfgather.c
$(PVM_ARCH)\pvmfgetctx.obj:  $(SDIR)\pvmfgetctx.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
                $(OUT)$(PVM_ARCH)\pvmfgetctx.obj  $(SDIR)\pvmfgetctx.c
$(PVM_ARCH)\pvmfgetinfo.obj:  $(SDIR)\pvmfgetinfo.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
                $(OUT)$(PVM_ARCH)\pvmfgetinfo.obj  $(SDIR)\pvmfgetinfo.c
$(PVM_ARCH)\pvmfgetinst.obj:  $(SDIR)\pvmfgetinst.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
                $(OUT)$(PVM_ARCH)\pvmfgetinst.obj $(SDIR)\pvmfgetinst.c
$(PVM_ARCH)\pvmfgetopt.obj:  $(SDIR)\pvmfgetopt.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
                $(OUT)$(PVM_ARCH)\pvmfgetopt.obj $(SDIR)\pvmfgetopt.c
$(PVM_ARCH)\pvmfgetrbuf.obj:  $(SDIR)\pvmfgetrbuf.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
                $(OUT)$(PVM_ARCH)\pvmfgetrbuf.obj $(SDIR)\pvmfgetrbuf.c
$(PVM_ARCH)\pvmfgetsbuf.obj:  $(SDIR)\pvmfgetsbuf.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
                $(OUT)$(PVM_ARCH)\pvmfgetsbuf.obj $(SDIR)\pvmfgetsbuf.c
$(PVM_ARCH)\pvmfgettid.obj:  $(SDIR)\pvmfgettid.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
                $(OUT)$(PVM_ARCH)\pvmfgettid.obj $(SDIR)\pvmfgettid.c
$(PVM_ARCH)\pvmfgsize.obj:  $(SDIR)\pvmfgsize.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
                $(OUT)$(PVM_ARCH)\pvmfgsize.obj $(SDIR)\pvmfgsize.c
$(PVM_ARCH)\pvmfhalt.obj:  $(SDIR)\pvmfhalt.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
                $(OUT)$(PVM_ARCH)\pvmfhalt.obj $(SDIR)\pvmfhalt.c
$(PVM_ARCH)\pvmfhostsync.obj:  $(SDIR)\pvmfhostsync.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
                $(OUT)$(PVM_ARCH)\pvmfhostsync.obj $(SDIR)\pvmfhostsync.c
$(PVM_ARCH)\pvmfinitsend.obj:  $(SDIR)\pvmfinitsend.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
                $(OUT)$(PVM_ARCH)\pvmfinitsend.obj $(SDIR)\pvmfinitsend.c
$(PVM_ARCH)\pvmfjoingrp.obj:  $(SDIR)\pvmfjoingrp.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
                $(OUT)$(PVM_ARCH)\pvmfjoingrp.obj $(SDIR)\pvmfjoingrp.c
$(PVM_ARCH)\pvmfkill.obj:  $(SDIR)\pvmfkill.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
                $(OUT)$(PVM_ARCH)\pvmfkill.obj $(SDIR)\pvmfkill.c
$(PVM_ARCH)\pvmflvgrp.obj:  $(SDIR)\pvmflvgrp.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
                $(OUT)$(PVM_ARCH)\pvmflvgrp.obj $(SDIR)\pvmflvgrp.c
$(PVM_ARCH)\pvmfmcast.obj:  $(SDIR)\pvmfmcast.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
                $(OUT)$(PVM_ARCH)\pvmfmcast.obj $(SDIR)\pvmfmcast.c
$(PVM_ARCH)\pvmfmkbuf.obj:  $(SDIR)\pvmfmkbuf.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
                $(OUT)$(PVM_ARCH)\pvmfmkbuf.obj $(SDIR)\pvmfmkbuf.c
$(PVM_ARCH)\pvmfmstat.obj:  $(SDIR)\pvmfmstat.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
                $(OUT)$(PVM_ARCH)\pvmfmstat.obj $(SDIR)\pvmfmstat.c
$(PVM_ARCH)\pvmfmytid.obj:  $(SDIR)\pvmfmytid.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
                $(OUT)$(PVM_ARCH)\pvmfmytid.obj $(SDIR)\pvmfmytid.c
$(PVM_ARCH)\pvmfnewctx.obj:  $(SDIR)\pvmfnewctx.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
                $(OUT)$(PVM_ARCH)\pvmfnewctx.obj  $(SDIR)\pvmfnewctx.c
$(PVM_ARCH)\pvmfnotify.obj:  $(SDIR)\pvmfnotify.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
                $(OUT)$(PVM_ARCH)\pvmfnotify.obj $(SDIR)\pvmfnotify.c
$(PVM_ARCH)\pvmfnrecv.obj:  $(SDIR)\pvmfnrecv.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
                $(OUT)$(PVM_ARCH)\pvmfnrecv.obj $(SDIR)\pvmfnrecv.c
$(PVM_ARCH)\pvmfpack.obj:  $(SDIR)\pvmfpack.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
                $(OUT)$(PVM_ARCH)\pvmfpack.obj $(SDIR)\pvmfpack.c
$(PVM_ARCH)\pvmfparent.obj:  $(SDIR)\pvmfparent.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
                $(OUT)$(PVM_ARCH)\pvmfparent.obj $(SDIR)\pvmfparent.c
$(PVM_ARCH)\pvmfperror.obj:  $(SDIR)\pvmfperror.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
                $(OUT)$(PVM_ARCH)\pvmfperror.obj $(SDIR)\pvmfperror.c
$(PVM_ARCH)\pvmfprecv.obj:  $(SDIR)\pvmfprecv.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
                $(OUT)$(PVM_ARCH)\pvmfprecv.obj $(SDIR)\pvmfprecv.c
$(PVM_ARCH)\pvmfprobe.obj:  $(SDIR)\pvmfprobe.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
                $(OUT)$(PVM_ARCH)\pvmfprobe.obj $(SDIR)\pvmfprobe.c
$(PVM_ARCH)\pvmfpsend.obj:  $(SDIR)\pvmfpsend.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
                $(OUT)$(PVM_ARCH)\pvmfpsend.obj $(SDIR)\pvmfpsend.c
$(PVM_ARCH)\pvmfpstat.obj:  $(SDIR)\pvmfpstat.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
                $(OUT)$(PVM_ARCH)\pvmfpstat.obj $(SDIR)\pvmfpstat.c
$(PVM_ARCH)\pvmfputinfo.obj:  $(SDIR)\pvmfputinfo.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
                $(OUT)$(PVM_ARCH)\pvmfputinfo.obj  $(SDIR)\pvmfputinfo.c
$(PVM_ARCH)\pvmftrecv.obj:  $(SDIR)\pvmftrecv.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
                $(OUT)$(PVM_ARCH)\pvmftrecv.obj $(SDIR)\pvmftrecv.c
$(PVM_ARCH)\pvmfrecvinfo.obj:  $(SDIR)\pvmfrecvinfo.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
                $(OUT)$(PVM_ARCH)\pvmfrecvinfo.obj  $(SDIR)\pvmfrecvinfo.c
$(PVM_ARCH)\pvmfreduce.obj:  $(SDIR)\pvmfreduce.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
                $(OUT)$(PVM_ARCH)\pvmfreduce.obj $(SDIR)\pvmfreduce.c
$(PVM_ARCH)\pvmfscatter.obj:  $(SDIR)\pvmfscatter.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
                $(OUT)$(PVM_ARCH)\pvmfscatter.obj $(SDIR)\pvmfscatter.c
$(PVM_ARCH)\pvmfsend.obj:  $(SDIR)\pvmfsend.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
                $(OUT)$(PVM_ARCH)\pvmfsend.obj $(SDIR)\pvmfsend.c
$(PVM_ARCH)\pvmfsendsig.obj:  $(SDIR)\pvmfsendsig.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
                $(OUT)$(PVM_ARCH)\pvmfsendsig.obj $(SDIR)\pvmfsendsig.c
$(PVM_ARCH)\pvmfsetctx.obj:  $(SDIR)\pvmfsetctx.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
                $(OUT)$(PVM_ARCH)\pvmfsetctx.obj  $(SDIR)\pvmfsetctx.c
$(PVM_ARCH)\pvmfsetopt.obj:  $(SDIR)\pvmfsetopt.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
                $(OUT)$(PVM_ARCH)\pvmfsetopt.obj $(SDIR)\pvmfsetopt.c
$(PVM_ARCH)\pvmfsetrbuf.obj:  $(SDIR)\pvmfsetrbuf.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
                $(OUT)$(PVM_ARCH)\pvmfsetrbuf.obj $(SDIR)\pvmfsetrbuf.c
$(PVM_ARCH)\pvmfsetsbuf.obj:  $(SDIR)\pvmfsetsbuf.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
                $(OUT)$(PVM_ARCH)\pvmfsetsbuf.obj $(SDIR)\pvmfsetsbuf.c
$(PVM_ARCH)\pvmfsiblings.obj:  $(SDIR)\pvmfsiblings.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
                $(OUT)$(PVM_ARCH)\pvmfsiblings.obj $(SDIR)\pvmfsiblings.c
$(PVM_ARCH)\pvmfsleep.obj:  $(SDIR)\pvmfsleep.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
                $(OUT)$(PVM_ARCH)\pvmfsleep.obj $(SDIR)\pvmfsleep.c
$(PVM_ARCH)\pvmfspawn.obj:  $(SDIR)\pvmfspawn.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
                $(OUT)$(PVM_ARCH)\pvmfspawn.obj $(SDIR)\pvmfspawn.c
$(PVM_ARCH)\pvmfstartpvmd.obj:  $(SDIR)\pvmfstartpvmd.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
                $(OUT)$(PVM_ARCH)\pvmfstartpvmd.obj $(SDIR)\pvmfstartpvmd.c
$(PVM_ARCH)\pvmftasks.obj:  $(SDIR)\pvmftasks.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
                $(OUT)$(PVM_ARCH)\pvmftasks.obj $(SDIR)\pvmftasks.c
$(PVM_ARCH)\pvmftidtoh.obj:  $(SDIR)\pvmftidtoh.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
                $(OUT)$(PVM_ARCH)\pvmftidtoh.obj $(SDIR)\pvmftidtoh.c
$(PVM_ARCH)\pvmfrecv.obj:  $(SDIR)\pvmfrecv.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
                $(OUT)$(PVM_ARCH)\pvmfrecv.obj $(SDIR)\pvmfrecv.c
$(PVM_ARCH)\pvmfunpack.obj:  $(SDIR)\pvmfunpack.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
                $(OUT)$(PVM_ARCH)\pvmfunpack.obj $(SDIR)\pvmfunpack.c


# Clean up everything but the .EXEs
clean:
	cd WIN32
	-del *.obj
	-del *.pdb
	-del lib*.*


