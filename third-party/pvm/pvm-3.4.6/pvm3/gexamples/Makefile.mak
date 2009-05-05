#
# $Id: Makefile.mak,v 1.3 1999/03/15 21:50:30 pvmsrc Exp $
#

#*************************************************************#
#**                                                         **#
#**      PVM Group Examples:                                **#
#**             c:    joinleave, tst, gs, ge, gexamp, thb,  **#
#**                   tnb, tjl, tjf, trsg                   **#
#**                                                         **#
#**             f: frsg                                     **#
#*************************************************************#

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE
NULL=nul
!ENDIF

!include $(PVM_ROOT)\conf\$(PVM_ARCH).def

SDIR		=	$(PVM_ROOT)\gexamples

CPROGS		=	joinleave tst gs ge gexamp thb tnb tjl tjf trsg

FPROGS		=	frsg

LIB		=	$(PVM_ROOT)\lib\$(PVM_ARCH)\libpvm3.lib

GLIB		=	$(PVM_ROOT)\lib\$(PVM_ARCH)\libgpvm3.lib

FLIB		=       $(PVM_ROOT)\libfpvm\$(PVM_ARCH)\libfpvm3.lib

FLIBPATH	=	/LIBPATH:$(PVM_FLIBDIR) \
				/LIBPATH:$(PVM_CLIBDIR)

default:	all

all:	bin $(CPROGS) $(FPROGS)

c-all:	bin $(CPROGS) 

f-all:	bin $(FPROGS)


###############################################################
# C
###############################################################

bin:
	if not exist "$(PVM_ARCH)\$(NULL)" mkdir "$(PVM_ARCH)"
	if not exist "$(PVM_ROOT)\bin\$(NULL)" mkdir "$(PVM_ROOT)\bin"
	if not exist "$(PVM_ROOT)\bin\$(PVM_ARCH)\$(NULL)" \
		mkdir "$(PVM_ROOT)\bin\$(PVM_ARCH)"


joinleave: $(PVM_ARCH)/joinleave.obj
	$(linkexe) $(conflags) \
		$(OUTBIN)$(PVM_ROOT)\bin\$(PVM_ARCH)\joinleave.exe \
		$(PVM_ARCH)\joinleave.obj $(LIB) $(GLIB) $(link_flags)

tst: $(PVM_ARCH)/tst.obj
	$(linkexe) $(conflags) \
		$(OUTBIN)$(PVM_ROOT)\bin\$(PVM_ARCH)\tst.exe \
		$(PVM_ARCH)\tst.obj $(LIB) $(GLIB) $(link_flags)

gs: $(PVM_ARCH)/gs.obj
	$(linkexe) $(conflags) \
		$(OUTBIN)$(PVM_ROOT)\bin\$(PVM_ARCH)\gs.exe \
		$(PVM_ARCH)\gs.obj $(LIB) $(GLIB) $(link_flags)

ge: $(PVM_ARCH)/ge.obj
	$(linkexe) $(conflags) \
		$(OUTBIN)$(PVM_ROOT)\bin\$(PVM_ARCH)\ge.exe \
		$(PVM_ARCH)\ge.obj $(LIB) $(GLIB) $(link_flags)

gexamp: $(PVM_ARCH)/gexamp.obj
	$(linkexe) $(conflags) \
		$(OUTBIN)$(PVM_ROOT)\bin\$(PVM_ARCH)\gexamp.exe \
		$(PVM_ARCH)\gexamp.obj $(LIB) $(GLIB) $(link_flags)

thb: $(PVM_ARCH)/thb.obj
	$(linkexe) $(conflags) \
		$(OUTBIN)$(PVM_ROOT)\bin\$(PVM_ARCH)\thb.exe \
		$(PVM_ARCH)\thb.obj $(LIB) $(GLIB) $(link_flags)

tnb: $(PVM_ARCH)/tnb.obj
	$(linkexe) $(conflags) \
		$(OUTBIN)$(PVM_ROOT)\bin\$(PVM_ARCH)\tnb.exe \
		$(PVM_ARCH)\tnb.obj $(LIB) $(GLIB) $(link_flags)

tjl: $(PVM_ARCH)/tjl.obj
	$(linkexe) $(conflags) \
		$(OUTBIN)$(PVM_ROOT)\bin\$(PVM_ARCH)\tjl.exe \
		$(PVM_ARCH)\tjl.obj $(LIB) $(GLIB) $(link_flags)

tjf: $(PVM_ARCH)/tjf.obj
	$(linkexe) $(conflags) \
		$(OUTBIN)$(PVM_ROOT)\bin\$(PVM_ARCH)\tjf.exe \
		$(PVM_ARCH)\tjf.obj $(LIB) $(GLIB) $(link_flags)

trsg: $(PVM_ARCH)/trsg.obj
	$(linkexe) $(conflags) \
		$(OUTBIN)$(PVM_ROOT)\bin\$(PVM_ARCH)\trsg.exe \
		$(PVM_ARCH)\trsg.obj $(LIB) $(GLIB) $(link_flags)

#############
# C OBJS
#############

$(PVM_ARCH)\joinleave.obj:  $(SDIR)\joinleave.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
		$(OUT)$(PVM_ARCH)\joinleave.obj $(SDIR)\joinleave.c

$(PVM_ARCH)\tst.obj:  $(SDIR)\tst.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
		$(OUT)$(PVM_ARCH)\tst.obj $(SDIR)\tst.c

$(PVM_ARCH)\gs.obj:  $(SDIR)\gs.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
		$(OUT)$(PVM_ARCH)\gs.obj $(SDIR)\gs.c

$(PVM_ARCH)\ge.obj:  $(SDIR)\ge.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
		$(OUT)$(PVM_ARCH)\ge.obj $(SDIR)\ge.c

$(PVM_ARCH)\gexamp.obj:  $(SDIR)\gexamp.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
		$(OUT)$(PVM_ARCH)\gexamp.obj $(SDIR)\gexamp.c

$(PVM_ARCH)\thb.obj:  $(SDIR)\thb.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
		$(OUT)$(PVM_ARCH)\thb.obj $(SDIR)\thb.c

$(PVM_ARCH)\tnb.obj:  $(SDIR)\tnb.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
		$(OUT)$(PVM_ARCH)\tnb.obj $(SDIR)\tnb.c

$(PVM_ARCH)\tjl.obj:  $(SDIR)\tjl.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
		$(OUT)$(PVM_ARCH)\tjl.obj $(SDIR)\tjl.c

$(PVM_ARCH)\tjf.obj:  $(SDIR)\tjf.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
		$(OUT)$(PVM_ARCH)\tjf.obj $(SDIR)\tjf.c

$(PVM_ARCH)\trsg.obj:  $(SDIR)\trsg.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
		$(OUT)$(PVM_ARCH)\trsg.obj $(SDIR)\trsg.c


#############################################################
# FORTRAN
#############################################################

frsg:  $(PVM_ARCH)\frsg.obj
	$(linkexe) $(conflags) $(FLIBPATH) \
		$(OUTBIN)$(PVM_ROOT)\bin\$(PVM_ARCH)\frsg.exe \
		$(PVM_ARCH)\frsg.obj $(LIB) $(GLIB) $(FLIB) $(link_flags)


##############
# FORTRAN OBJS
##############

$(PVM_ARCH)\frsg.obj:  $(SDIR)\frsg.f
	$(fc) $(cdebug) /Fo$(PVM_ARCH)\frsg.obj $(fflags) $(cvars) \
		$(SDIR)\frsg.f


# Clean all Fortran example codes - .OBJ and .EXE
tidy-f:
	-del $(PVM_ARCH)\frsg.obj

	-del $(PVM_ROOT)\bin\$(PVM_ARCH)\frsg.exe

# Clean all C example codes - .OBJ and .EXE
tidy-c:
	-del $(PVM_ARCH)\joinleave.obj
	-del $(PVM_ARCH)\tst.obj
	-del $(PVM_ARCH)\gs.obj
	-del $(PVM_ARCH)\ge.obj
	-del $(PVM_ARCH)\gexamp.obj
	-del $(PVM_ARCH)\thb.obj
	-del $(PVM_ARCH)\tnb.obj
	-del $(PVM_ARCH)\tjl.obj
	-del $(PVM_ARCH)\tjf.obj
	-del $(PVM_ARCH)\trsg.obj

	-del $(PVM_ROOT)\bin\$(PVM_ARCH)\joinleave.exe
	-del $(PVM_ROOT)\bin\$(PVM_ARCH)\tst.exe 
	-del $(PVM_ROOT)\bin\$(PVM_ARCH)\gs.exe 
	-del $(PVM_ROOT)\bin\$(PVM_ARCH)\ge.exe 
	-del $(PVM_ROOT)\bin\$(PVM_ARCH)\gexamp.exe 
	-del $(PVM_ROOT)\bin\$(PVM_ARCH)\thb.exe 
	-del $(PVM_ROOT)\bin\$(PVM_ARCH)\tnb.exe 
	-del $(PVM_ROOT)\bin\$(PVM_ARCH)\tjl.exe 
	-del $(PVM_ROOT)\bin\$(PVM_ARCH)\tjf.exe 
	-del $(PVM_ROOT)\bin\$(PVM_ARCH)\trsg.exe 


# Clean up everything but the .EXEs
clean:
	-del *.obj
	-del *.pdb

