#$Id$
           FC = f77
           CC = cc
           AR = ar
           AS = as
       RANLIB = echo
          CPP = /usr/lib/cpp
        SHELL = /bin/sh
           MV = /bin/mv
           RM = /bin/rm
      RMFLAGS = -r
      INSTALL = @echo
    MAKEFLAGS = -j 1                                                            
      ARFLAGS = rcv
        MKDIR = mkdir
       LINK.f = $(FLD)
       LINK.c = $(CLD)
 GLOB_DEFINES = -D$(TARGET)
          CLD = $(CC)
          _FC = $(notdir $(FC))
          _CC = $(notdir $(CC))
      COPT_NO = -g
      FOPT_NO = -g

# enable -Wall when using GNU compilers
ifdef USE_FULL_WARNINGS
   WALL = -Wall
endif

#-------------------------- IBM BlueGene -----------------------------
ifeq ($(TARGET), BGL)
ifdef BGCOMPILERS
           FC     = $(BGCOMPILERS)/powerpc-bgl-blrts-gnu-g77
           CC     = $(BGCOMPILERS)/powerpc-bgl-blrts-gnu-gcc -g
           AR     = $(BGCOMPILERS)/powerpc-bgl-blrts-gnu-ar
           AS     = $(BGCOMPILERS)/powerpc-bgl-blrts-gnu-as
           CPP    = $(BGCOMPILERS)/powerpc-bgl-blrts-gnu-cpp
           RANLIB = $(BGCOMPILERS)/powerpc-bgl-blrts-gnu-ranlib
else
           CC     = mpicc
endif

ifeq ($(_FC),blrts_xlf90)
           XLFDEFINED =1
           FOPT_REN +=   -qfixed
endif
ifeq ($(_FC),blrts_xlf)
           XLFDEFINED =1
endif

ifdef XLFDEFINED
           FOPT_REN += -qfixed
           COPT_REN = -DNOUNDERSCORE
	   FOPT = -O0
endif

	   GLOB_DEFINES+= -DBLRTS
	   INCLUDES += -I$(BGDRIVER)/bglsys/include
	   COPT = -O0
endif
#-------------------------- Cygwin/Cygnus: GNU on Windows ------------
ifeq ($(TARGET),CYGNUS) 
           FC = g77
           CC = gcc
     COPT_REN = -malign-double
 GLOB_DEFINES+= -DLINUX
endif

ifeq ($(TARGET),CYGWIN)
           FC = g77
           CC = gcc
 GLOB_DEFINES+= -DSHMEM -DMMAP -DSYSV
     COPT_REN = -malign-double
     EXTRA_OBJ = winshmem.o signaltrap.o shmalloc.o
endif

#-------------------------- Mac X ------------
ifeq ($(TARGET),MACX)
       RANLIB = ranlib
           FC = g77
           CC = gcc
 GLOB_DEFINES+= -DSHMEM -DMMAP -DDATA_SERVER
     EXTRA_OBJ = winshmem.o signaltrap.o shmalloc.o dataserv.o spawn.o \
                 dataserv.o sockets.o request.o ds-shared.o buffers.o async.o

ifneq (,$(findstring mpif,$(_FC)))
	_FC = $(shell $(FC) -v 2>&1 | awk ' /g77 version/ { print "g77"; exit }; /xlf/ {print "xlf"; exit }; /Pro Fortran/ {print "absoft"; exit }' )
endif
ifneq (,$(findstring mpicc,$(_CC)))
        _CC = $(shell $(CC) -v 2>&1 | awk ' /gcc version/ { print "gcc" ; exit  }; /Pro Fortran/ {print "absoft"; exit }' ) 
endif
ifeq ($(FC),xlf)      
     FOPT_REN = -q32  -qextname
GLOB_DEFINES += -DXLFLINUX -DEXTNAME
endif
ifeq ($(CC),xlc)      
   EXTRA_OBJ += tas.o
endif
ifneq ($(_FC),xlf)
ifneq ($(_FC),g77)
	_FC = $(shell $(FC) -v 2>&1 | awk ' /Pro Fortran/ {print "absoft"; exit }' )
endif
endif
ifneq ($(_CC),xlc)      
ifneq ($(_CC),gcc)      
   EXTRA_OBJ += tas.o
endif
endif
ifeq ($(_FC),absoft)      
     FOPT_REN = -f -N15
endif

endif

#-------------------------- INTERIX 2.2.5 on Windows ------------
ifeq ($(TARGET),INTERIX) 
           FC = g77
           CC = gcc
     COPT_REN = -malign-double
endif
 
#------------------------------- Linux -------------------------------
ifeq ($(TARGET),LINUX)
       RANLIB = ranlib
           FC = g77
           CC = gcc
         _CPU = $(shell uname -m |\
                 awk ' /sparc/ { print "sparc" };/ppc/{ print "ppc"};\
                     /i686/{ print "686" }; /i*86&&^i686/ { print "x86" } ' )

ifneq (,$(findstring mpif,$(_FC)))
         _FC = $(shell $(FC) -v 2>&1 | awk ' /g77 version/ { print "g77"; exit }; /pgf/ { apgfcount++}; END {if(apgfcount)print "pgf77"} ; / frt / { print "frt" ; exit }' )
endif
ifneq (,$(findstring mpicc,$(_CC)))
         _CC = $(shell $(CC) -v 2>&1 | awk ' /gcc version/ {agcccount++}; END {if(agcccount)print "gcc"} ' )
endif
#
ifeq ($(ARMCI_NETWORK), LAPI)
   CC     = mpcc
   LINK.f = mpcc -lc -lxlf -lxlf90 -lm
endif
#
ifeq ($(_CPU),ppc)
    CDEFS += -DPPC
endif
ifeq ($(_CPU),x86)
    ifneq ($(ARMCI_NETWORK),MELLANOX)
       ifneq ($(ARMCI_NETWORK),OPENIB)
          OPT_ALIGN = -malign-double
       endif
    endif
endif
ifeq ($(_CPU),686)
    ifneq ($(ARMCI_NETWORK),MELLANOX)
       OPT_ALIGN = -malign-double -march=pentiumpro
    else
       OPT_ALIGN = -march=pentiumpro 
    endif
    ifeq ($(ARMCI_NETWORK),OPENIB)
        OPT_ALIGN = -march=pentiumpro
    endif
    CDEFS += -DCOPY686
    EXTRA_OBJ += x86copy.o
endif
ifeq ($(_CPU),786)
    ifneq ($(ARMCI_NETWORK),MELLANOX)
       OPT_ALIGN = -malign-double -march=pentiumpro
    else
       OPT_ALIGN = -march=pentiumpro 
    endif
    ifeq ($(ARMCI_NETWORK),OPENIB)
        OPT_ALIGN = -march=pentiumpro
    endif
#   CDEFS += -DCOPY686
    EXTRA_OBJ += x86copy.o
endif
ifeq ($(_CC),icc)
    ifeq ($(COPT),-O)
        COPT = -O3
        COPT_REN = -prefetch 
    endif
endif
ifeq ($(_CC),xlc)
    COPT_REN = -q32  -qlanglvl=extended
endif
ifeq ($(_CC),gcc)
    ifeq ($(COPT),-O)
       COPT = -O2 -finline-functions -funroll-loops
       COPT_REN += $(WALL) $(OPT_ALIGN)
    endif
else
    EXTRA_OBJ += tas.o
endif
#
# GNU compilers - g77
ifeq ($(_FC),g77)
    ifeq ($(FOPT),-O)
       FOPT = -O3
       FOPT_REN += -funroll-loops -fomit-frame-pointer $(OPT_ALIGN)
    endif
else
#  PGI fortran compiler on intel
   ifneq (,$(findstring pgf,$(_FC)))
       FOPT_REN = -Mvect  -Munroll -Mdalign -Minform,warn -Mnolist -Minfo=loop -Munixlogical
       ifeq ($(_CPU),686)
          FOPT_REN +=  -tp p6
       endif
   endif
#  Intel fortran compiler (<= version 7)
   ifneq (,$(findstring ifc,$(_FC)))
       FOPT=-O4 -prefetch -unroll -ip
       ifeq ($(_CPU),k7)
          FOPT_REN = -xM 
       endif
       ifeq ($(_CPU),686)
          FOPT_REN = -xK -tpp6
       endif
       ifeq ($(_CPU),786)
          FOPT_REN = -xW -tpp7
       endif
   endif
#  Intel fortran compiler (>= version 8)
   ifneq (,$(findstring ifort,$(_FC)))
       FOPT=-O4 -prefetch -unroll -ip
       ifeq ($(_CPU),k7)
          FOPT_REN = -xM 
       endif
       ifeq ($(_CPU),686)
          FOPT_REN = -xK -tpp6
       endif
       ifeq ($(_CPU),786)
          FOPT_REN = -xW -tpp7
       endif
       ifeq ($(LINK.c),$(FC))
          CLD_REN += -nofor_main
       endif
   endif

#  IBM fortran compiler
   ifeq ($(_CC),xlc)
       COPT_REN = -q32  -qlanglvl=extended
       CDEFS += -DXLCLINUX
   endif
   ifeq ($(_FC),xlf)
       FOPT_REN = -q32  -qEXTNAME
   endif

#  Fujitsu Compilers
   ifeq ($(_CC),mpifcc)
      _CC = fcc
   endif
   ifeq ($(_CC),fcc)
      COPT = -Kfast
   endif
   ifeq ($(_FC),mpifrt)
      _FC = frt
   endif
   ifeq ($(_FC),frt)
      FOPT = -Kfast
      FOPT_REN += -X9 -Am
   endif

#  GNU g95 Compiler
  ifeq ($(_FC),g95)
     FOPT_REN += -i4
     FOPT_REN += -fno-second-underscore -ffixed-form -ffixed-line-length-80
     FLD_REN=
  endif
endif

endif # end of LINUX

#------------------------------- Linux64 -------------------------------
#
# Linux 64-bit on DEC/Compaq Alpha with DEC compilers
# Linux 64-bit on Itanium with Intel compilers
# Linux 64-bit on Optern with GNU compilers
ifeq ($(TARGET),LINUX64)
   GLOB_DEFINES += -DLINUX
         _CPU = $(shell uname -m)

ifneq (,$(findstring mpicc,$(_CC)))
         _CC = $(shell $(CC) -v 2>&1 | awk ' /gcc version/ { print "gcc" ; exit  } ' )
endif

#
#-----------------------------------
# LINUX 64 CPU Specific Setup: IA64
#-----------------------------------
# IA64, only Intel fortran compiler supported 
ifeq  ($(_CPU),ia64)
     FC=efc
     CC=gcc
  _SGIALTIX= $(shell if [ -r /proc/sgi_sn/system_serial_number ]; then /bin/echo Y; fi)
  ifeq ($(_SGIALTIX),Y)
   GLOB_DEFINES += -DSGIALTIX
  endif
  ifeq ($(_FC),sgif90)
     FOPT_REN = -macro_expand 
  endif

  # Intel Compiler version <= 7
  ifeq ($(_FC),efc)
     _IFCV8= $(shell efc -v  2>&1|egrep "Version "|head -1|awk ' /8\./  {print "Y";exit}; /9./ {print "Y"; exit}')
     ifeq ($(_IFCV8),Y)
        FC = ifort
     endif
     GLOB_DEFINES += -DIFCLINUX
     FOPT_REN= -w -cm -w90 #-align 
#     for IA64 only. gcc 3.x cannot find the symbols modsi3 and divsi3 in IA64.
#     lib1funs-ia64 includes these symbols.
#     EXTRA_OBJ += funcs-ia64.o
     ifeq ($(FOPT),-O)
        FOPT =  -O3 -hlo -ftz -pad
     endif
  endif

  # Intel Compiler version >= 8
  ifeq ($(_FC),ifort)
     GLOB_DEFINES += -DIFCLINUX
     FOPT_REN= -w -cm -w90 #-align 
     ifeq ($(FOPT),-O)
        FOPT =  -O3 -hlo -ftz -pad
     endif
  endif

  ifeq ($(_CC),gcc)
      COPT_NO = -g -O0
      COPT= -O0 -g
      COPT_REN += $(WALL)
  endif
  ifeq ($(_CC),ecc)
     COPT_REN= -w1 #-fno-alias    
  endif
  ifeq ($(_CC),icc)
     COPT_REN= -w1 #-fno-alias    
  endif 

# Fujitsu Compilers
  ifeq ($(_CC),mpifcc)
      _CC = fcc
  endif
  ifeq ($(_CC),fcc)
     COPT = -Kfast
  endif
  ifeq ($(_FC),mpifrt)
     _FC = frt
  endif
  ifeq ($(_FC),frt)
     FOPT = -Kfast
     FOPT_REN += -X9 -Am
  endif

  GLOB_DEFINES += -DNEED_MEM_SYNC
endif

#-----------------------------------
# LINUX 64 CPU Specific Setup: Alpha
#-----------------------------------
ifeq  ($(_CPU),alpha)
     FC = fort
     CC = ccc
     FOPT_REN = -assume no2underscore -fpe3 -check nooverflow
     FOPT_REN+= -assume accuracy_sensitive -check nopower -check nounderflow
endif

#
#-------------------------------------
# LINUX 64 CPU Specific Setup: Opteron
#-------------------------------------
ifeq  ($(_CPU),x86_64)
  _FC = $(shell $(FC) -v 2>&1 | awk ' /g77 version/ { print "g77"; exit }; /gcc version/ { print "g77"; exit }; /Path/ { print "pathf90" ; exit }; /efc/ { print "efc" ; exit }; /pgf90/ { pgf90count++}; /pgf77/ { pgf77count++}; END {if(pgf77count)print "pgf77" ; if(pgf90count)print "pgf90"} ')
  ifeq ($(_FC),pgf90)
     FOPT_REN= -fast -Mdalign -tp k8-64 -O3
  endif
  _FC = $(shell $(FC) -v 2>&1 | awk ' /g77 version/ { print "g77"; exit }; /gcc version/ { print "g77"; exit }; /Path/ { print "pathf90" ; exit }; /efc/ { print "efc" ; exit }; /ifc/ { print "ifort" ; exit };/ifort/ { print "ifort" ; exit }; /pgf90/ { apgf90count++}; /pgf77/ { apgf77count++}; END {if(apgf77count)print "pgf77" ; if(apgf90count)print "pgf90"} ')
  ifeq ($(_FC),g77)
        FOPT_REN  += -fstrength-reduce -mfpmath=sse 
  endif
  ifeq ($(_FC),ifort)
        FOPT_REN  += -quiet -O3  -w -cm -xW -tpp7
  endif
  ifeq ($(_FC),pathf90)
        FOPT_REN  = -O3 -OPT:Ofast 
  endif

  ifeq ($(ARMCI_NETWORK), LAPI)
     CC     = mpcc
     LINK.f = mpcc -m64 -lm
     GLOB_DEFINES += -DLAPI -DLAPI64 -DXLCLINUX
     COPT_REN = -m64
  endif

endif

#
#-------------------------------------
# LINUX 64 CPU Specific Setup: power4
#-------------------------------------
ifeq  ($(_CPU),ppc64)
  FC=xlf
  # CC=/opt/cross/bin/powerpc64-linux-gcc
  CC=gcc
  GLOB_DEFINES += -DNEED_MEM_SYNC
  COPT=-O3
  XLC_OPT = -q64 -qlanglvl=extended -qinline=100 -qstrict -qarch=auto -qtune=auto
_FC = $(shell $(FC) -v 2>&1 | awk ' /xlf/ { print "xlf"; exit }; /g77 version/ { print "g77"; exit };/gcc version 4/ { print "gfortran"; exit }; /gcc version/ { print "g77"; exit }; /ifc/ { print "ifort" ; exit }; /ifort/ { print "ifort" ; exit }; /efc/ { print "efc" ; exit }; /pgf90/ { pgf90count++}; /pgf77/ { pgf77count++}; /PathScale/ { pathf90count++}; END {if(pgf77count)print "pgf77" ; if(pgf90count)print "pgf90" ; if(pathf90count)print "pathf90"} ')

  ifeq ($(ARMCI_NETWORK), LAPI)
     CC     = mpcc
     LINK.f = mpcc -lc -lxlf -lxlf90 -lm
     GLOB_DEFINES += -DLAPI -DLAPI64 -DXLCLINUX
     COPT_REN = $(XLC_OPT)
  endif

  ifeq ($(_CC),xlc)
     GLOB_DEFINES += -DXLCLINUX
     COPT_REN = $(XLC_OPT)
  endif
  ifeq ($(_CC),gcc)
     COPT_REN +=  -m64 -funroll-loops
  endif
  ifeq ($(_FC),xlf)
     FOPT_REN = -q64 -qEXTNAME
     FOPT = -O4 -qarch=auto -qstrict
  endif
endif

   EXTRA_OBJ += tas.o
endif # end of LINUX64

#----------------------------- Fujitsu ------------------------------
ifeq ($(TARGET),FUJITSU-VPP)
           FC = frt
     FOPT_REN = -Sw -KA32
     COPT_REN = -x100 -KA32
 GLOB_DEFINES = -DFUJITSU -DVPP_STRIDED_READ -DVPP_STRIDED_WRITE
#   EXTRA_LIBS = /usr/local/lib/libmp.a -L/opt/tools/lib/ -lgen  -lpx -lelf -Wl,-J,-P
endif

ifeq ($(TARGET),FUJITSU-VPP64)
           FC = frt
     FOPT_REN = -Sw
     COPT_REN = -x100
 GLOB_DEFINES = -DFUJITSU -DFUJITSU64 
#disable if broken
 GLOB_DEFINES += -DVPP_STRIDED_READ -DVPP_STRIDED_WRITE
endif

#AP3000 running Solaris on Sparc
ifeq ($(TARGET),FUJITSU-AP)
           FC = frt
           CC = fcc
#     FOPT_REN = 
 GLOB_DEFINES = -DFUJITSU
endif

#---------------------------- Hitachi SR8000 ------------------------------
ifeq ($(TARGET),HITACHI)
           FC = f90 -hf77
           CC = mpicc
      ifeq ($(FOPT),-O)
         FOPT = -opt=ss -nopar
      endif
   EXTRA_OBJ += sr8k.o
endif

#------------- Sun sparc/x86/x64 Solaris or Fujitsu Sparc/Solaris --------------
ifeq ($(TARGET),SOLARIS)
  _SUN_PROC = $(shell /bin/uname -p)
  ifeq ($(_SUN_PROC),i386)
       _XARCH = -xarch=sse2
  else
       _XARCH =
  endif
  ifeq ($(_CC),mpifcc)
       _CC = fcc
  endif
  ifeq ($(_FC),mpifrt)
       _FC = frt
  endif
  ifeq ($(_FC),f77)
      FOPT_REN = -dalign $(_XARCH)
  endif
  ifeq ($(_CC),cc)
      COPT_REN = -dalign $(_XARCH)
  endif
  ifeq ($(_FC),frt)
      FOPT_REN = -fw -Kfast -KV8PFMADD
  endif
  ifeq ($(_CC),fcc)
      COPT_REN = -Kfast -KV8PFMADD -x0
      GLOB_DEFINES += -DSPARC64_GP
  endif
endif
#
# 64-bit
ifeq ($(TARGET),SOLARIS64)
#
  _SUN_PROC = $(shell /bin/uname -p)
  ifeq ($(_SUN_PROC),i386)
       _XARCH = -xarch=amd64
  else
       _XARCH = -xarch=v9
  endif
  ifeq ($(_CC),mpifcc)
       _CC = fcc
  endif
  ifeq ($(_FC),mpifrt)
       _FC = frt
  endif
  ifeq ($(_FC),frt)
     FOPT_REN = -fw -Kfast -KV9FMADD
  else
     FOPT_REN = -dalign $(_XARCH)
  endif
  ifeq ($(_CC),fcc)
     COPT_REN = -Kfast -KV9FMADD -x0
     GLOB_DEFINES += -DSPARC64_GP
  else
     COPT_REN = -dalign $(_XARCH)
  endif
#
 GLOB_DEFINES += -DSOLARIS
endif
#
#
#obsolete SunOS 4.X
ifeq ($(TARGET),SUN)
           CC = gcc
     FOPT_REN = -Nl100
       RANLIB = ranlib
endif

#----------------------------- HP/Convex ------------------------------
ifeq ($(TARGET),HPUX)
           FC = fort77
           AS = cc -c
    ifeq ($(FOPT),-O)
         FOPT = -O3
         FOPT += $(shell uname -m |\
		 awk -F/ '{ if ( $$2 > 799 ) print "+Odataprefetch" }')
    endif
     FOPT_REN = +ppu
     COPT_REN = -Ae
       CDEFS += -DEXTNAME
#   EXTRA_OBJ = tas-parisc.o
endif
#
ifeq ($(TARGET),HPUX64)
         _CPU = $(shell uname -m)
           FC = f90
           AS = cc -c
     FOPT_REN = +ppu
     COPT_REN = -Ae 
ifeq  ($(_CPU),ia64)
     FOPT_REN = +DD64
     COPT_REN = +DD64
else
     FOPT_REN += +DA2.0W 
     COPT_REN += +DA2.0W 
    ifeq ($(FOPT),-O)
         FOPT = -O3 +Odataprefetch +Ofastaccess
    endif
endif
       CDEFS += -DEXTNAME
GLOB_DEFINES += -DHPUX
endif
#
#
ifeq ($(TARGET),CONVEX-SPP)
           FC = fc
          CPP = /lib/cpp
    ifeq ($(FOPT),-O)
         FOPT = -O1
    endif
    ifeq ($(FOPT),-g)
         FOPT = -no
    endif
    ifeq ($(COPT),-g)
         COPT = -no
    endif
    COPT_REN  = -or none
     FOPT_REN = -ppu -or none
 GLOB_DEFINES = -DCONVEX
endif

#----------------------------- SGI ---------------------------------
ifeq ($(TARGET),SGI)
#    COPT_REN = -32
#    FOPT_REN = -32
    SGI = yes
endif

ifeq ($(TARGET),SGI_N32)
    COPT_REN = -n32
    FOPT_REN = -n32
    SGI = yes
endif

ifeq ($(TARGET),SGI64)
    COPT_REN = -64
    FOPT_REN = -align64 -64
    SGI = yes
endif

ifeq ($(TARGET),SGITFP)
    COPT_REN = -64
    FOPT_REN = -align64 -64
GLOB_DEFINES += -DSGI64
    SGI = yes
endif

ifdef SGI
GLOB_DEFINES += -DSGI
# optimization flags for R10000 (IP28)
  FOPT_R10K = -TENV:X=1 -WK,-so=1,-o=1,-r=3,-dr=AKC
# optimization flags for R8000 (IP21)
  FOPT_R8K = -TENV:X=3 -WK,-so=1,-o=1,-r=3,-dr=AKC
    ifeq ($(FOPT),-O)
         FOPT = -O3
    endif

#CPU specific compiler flags
ifneq ($(TARGET_CPU),R4000)
    COPT_REN += -mips4
    FOPT_REN += -mips4
else
     COPT_REN = -32
     FOPT_REN = -32
endif

ifdef TARGET_CPU

ifeq ($(TARGET_CPU),R10000)
 FOPT_REN += $(FOPT_R10K)
endif
ifeq ($(TARGET_CPU),R8000)
 FOPT_REN += $(FOPT_R8K)
endif

else
    FOPT_REN += $(FOPT_R10K)
endif

endif

#----------------------------- DEC/Compaq ---------------------------------
ifeq ($(TARGET),DECOSF)
USE_REGION = $(shell /sbin/sysconfig -s | awk ' /shmem/  { print "yes" } ' )
          CLD = cc
     FOPT_REN = -fpe2 -check nounderflow -check nopower -check nooverflow
     COPT_REN += -D_POSIX_PII_SOCKET
ifeq ($(USE_REGION),yes)
     COPT_REN += -DREGION_ALLOC
     EXTRA_OBJ = alloc.o
endif
endif
#------------------------------- Crays ------------------------------------

# YMP, J90, ... PVP XT3
#
ifeq ($(TARGET),CRAY-YMP)
     COPT_REN = -htaskprivate $(LIBCM)
           FC = f90
 GLOB_DEFINES = -DCRAY_YMP
     FOPT_REN = -dp -ataskcommon $(LIBCM)
         CRAY = yes
endif

ifeq ($(TARGET),CRAY-SV1)
     COPT_REN = -htaskprivate $(LIBCM)
           FC = f90
 GLOB_DEFINES = -DCRAY_YMP -DCRAY_SV1
     FOPT_REN = -dp -ataskcommon $(LIBCM)
         CRAY = yes
endif

ifeq ($(TARGET),cray-sv2)
           FC = ftn
 GLOB_DEFINES = -DCRAY_YMP
     ifeq ($(FOPT), -O)
#        FOPT = -O vector3,msgs,negmsgs -rm
         FOPT = -O vector3
     endif
     ifeq ($(COPT), -O)
         COPT = -O -h inline2
     endif
     ifdef USE_SSP
       FOPT_REN += -O ssp
       COPT_REN += -h ssp
     endif

#    COPT_REN = -h report=imsvf
#         CRAY = yes
endif

ifeq ($(TARGET),CRAY-T3D)
           FC = cf77
 GLOB_DEFINES = -DCRAY_T3D
         CRAY = yes
endif


ifeq ($(TARGET),CRAY-T3E)
           FC = f90
     FOPT_REN = -dp
 GLOB_DEFINES = -DCRAY_T3E
         CRAY = yes
endif

ifdef CRAY
     ifeq ($(FOPT), -O)
         FOPT = -O2
     endif
     ifeq ($(COPT), -O)
         COPT = -O1 -hinline3
     endif
endif

ifeq ($(TARGET),CATAMOUNT)
           FC = ftn
           CC = cc
#  _FC = $(shell $(FC) -v 2>&1 | awk ' /g77 version/ { print "g77"; exit }; /gcc version/ { print "g77"; exit }; /pgf90/ { pgf90count++}; END if(pgf90count)print "pgf90" ')
#  ifeq ($(_FC),pgf90)
#           FOPT_REN= -fast -Mdalign -tp k8-64 -O3
#  endif
#  ifeq ($(_FC),g77)
#     FOPT_REN = -O3 -fno-second-underscore
#  endif

 FOPT_REN = -O3

 GLOB_DEFINES = -DXT3 -DCATAMOUNT
   EXTRA_OBJ += tas.o
endif 

#done CRAY systems
#................................. NEC SX-6 .................................
ifeq ($(TARGET),NEC)
#
     FC = f90
#    on SX-6 we must use c++ compiler and cc on SX-5
     CC = c++
     ifeq ($(FOPT), -O)
         FOPT = -V -Cvopt -Wf"-pvctl nomsg noassume vwork=stack"
     endif
     ifeq ($(COPT), -O)
         COPT = -O nomsg
     endif
     EXTRA_OBJ = tas-sx.o
     COPT_REN = -size_t64 -Nv -Xa
     ASFLAGS = -h size_t64
     EXTRA_LIBS += -f90libew
#SX5
#    COPT = -O nomsg -hnovector,nomulti -pvctl,nomsg
#    FOPT = -Cvopt -Wf"-pvctl nomsg"
#    COPT_REN = -hsize_t64
#    EXTRA_LIBS += -li90sxe
endif

#................................. IBM SP and workstations ...................

# IBM SP with LAPI: 32- and 64-bit versions
ifeq ($(TARGET),LAPI)
         IBM_ = 1
         LAPI_= 1
endif
ifeq ($(TARGET),LAPI64)
       IBM64_ = 1
         LAPI_= 1
GLOB_DEFINES += -DLAPI -DIBM64
endif
#
# IBM RS/6000 under AIX
ifeq ($(TARGET),IBM)
        IBM_  = 1
endif
ifeq ($(TARGET),IBM64)
      IBM64_  = 1
endif
#
ifdef LAPI_
          CC  = mpcc_r
      LINK.f  = mpcc_r -lc_r -lxlf -lxlf90 -lm
    EXTRA_OBJ = lapi.o request.o buffers.o
GLOB_DEFINES += -DSP
endif
#
ifdef IBM64_
        IBM_  = 1
     FOPT_REN = -q64
     COPT_REN = -q64
      ARFLAGS = -rcv -X 64
endif
#
ifdef IBM_
     ifeq ($(FOPT), -O)
         FOPT = -O4 -qarch=auto -qstrict
     else
#        without this flag xlf_r creates nonreentrant code
         FOPT_REN += -qnosave
     endif
     ifeq ($(COPT), -O)
         COPT = -O3 -qinline=100 -qstrict -qarch=auto -qtune=auto
     endif
       CDEFS += -DEXTNAME
           FC = xlf
GLOB_DEFINES += -DAIX
          _CPU = $(shell lsattr -El `lsdev -C -c processor -F name | head -1` | awk ' /POWER4/ { print "PWR4" }; /POWER5/ { print "PWR4" }')
endif
#
ifeq ($(_CPU),PWR4)
GLOB_DEFINES += -DNEED_MEM_SYNC
endif
#
#...................... common definitions .......................
#

#alternative malloc library
ifdef ALT_MALLOC
EXTRA_LIBS += $(ALT_MALLOC)
endif

#get rid of 2nd underscore under g77
ifeq ($(_FC),g77)
     FOPT_REN += -fno-second-underscore
endif
ifeq ($(_FC),pathf90)
     FOPT_REN += -fno-second-underscore
endif


       DEFINES = $(GLOB_DEFINES) $(LIB_DEFINES)

#Fujitsu fortran compiler requires -Wp prefix for cpp symbols
ifeq ($(TARGET),FUJITSU-VPP)
       comma:= ,
       empty:=
       space:= $(empty) $(empty)
       FDEFINES_0 = $(strip  $(DEFINES))
       FDEFINES = -Wp,$(subst $(space),$(comma),$(FDEFINES_0))
else
       FDEFINES = $(DEFINES)
endif

       INCLUDES += $(LIB_INCLUDES)
       CPP_FLAGS += $(INCLUDES) $(FDEFINES)

ifdef FRAGILE
       FOPT= $(FOPT_NO)
       COPT= $(COPT_NO)
endif

       FFLAGS = $(FOPT) $(FOPT_REN)
       CFLAGS = $(COPT) $(COPT_REN) $(INCLUDES) $(DEFINES) $(CDEFS) $(LIB_CDEFS)
       CFLAGS := $(strip $(CFLAGS))
       FFLAGS := $(strip $(FFLAGS))

ifeq (CRAY,$(findstring CRAY,$(TARGET)))
%.o:    %.f
	$(FC) -c $(FFLAGS) $*.f
endif

%.o:    %.gcc
	gcc -x c -c $*.gcc
