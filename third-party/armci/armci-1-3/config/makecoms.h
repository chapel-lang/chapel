#provides definitions for protocols used by communication libraries
# COMM_DEFINES - identifies communication network protocol
# COMM_LIBS    - list of libs that need to be linked with 
# COMM_INCLUDES- include path to access communication protocol API
#
ifeq ($(ARMCI_NETWORK),BGML)
   COMM_DEFINES = -DBGML
   ifdef BGML_INCLUDE
      COMM_INCLUDES = -I$(BGML_INCLUDE)
   endif
   ifdef BGML_LIB
      COMM_LIBS = -L$(BGML_LIB)
   endif
   BGML_LIB_NAME= -lmsglayer.rts -lrts.rts -ldevices.rts
endif
ifeq ($(ARMCI_NETWORK),BGMLMPI)
   COMM_DEFINES = -DBGML
   ifdef BGMLMPI_INCLUDE
      COMM_INCLUDES = -I$(BGMLMPI_INCLUDE)
   endif
   ifdef BGMLMPI_LIB
      COMM_LIBS = -L$(BGMLMPI_LIB)
   endif
   BGMLMPI_LIB_NAME = -lmpich.rts -lfmpich_.rts -lmsglayer.rts -lrts.rts -ldevices.rts
   COMM_LIBS += $(BGMLMPI_LIB_NAME)
endif
ifeq ($(ARMCI_NETWORK),GM)
  COMM_DEFINES = -DGM
  ifdef GM_INCLUDE
    COMM_INCLUDES = -I$(GM_INCLUDE)
  endif
  ifdef GM_LIB
    COMM_LIBS = -L$(GM_LIB)
  endif
  GM_LIB_NAME = -lgm
  COMM_LIBS += $(GM_LIB_NAME) -lpthread
endif

ifeq ($(ARMCI_NETWORK),CRAY-SHMEM)
   COMM_DEFINES += -DCRAY_SHMEM -DNO_SHM
   ifeq ($(TARGET),CATAMOUNT)
      COMM_DEFINES += -DSHMEM_HANDLE_NOT_SUPPORTED
   endif
   COMM_LIBS += -lsma
endif

ifeq ($(ARMCI_NETWORK),PORTALS)

  ifndef PORTALS_NAL
    PORTALS_NAL = p3nal\_utcp
  endif

  COMM_INCLUDES = -I/usr/local/include
  COMM_LIBS = -L/usr/local/lib

  ifdef PORTALS_INCLUDE
    COMM_INCLUDES = -I$(PORTALS_INCLUDE)
  endif

  ifdef PORTALS_LIB
    COMM_LIBS = -L$(PORTALS_LIB)
  endif

  COMM_DEFINES = -DPORTALS -DP3_NAL=\<$(PORTALS_NAL)\.h\>
  ifndef PORTALS_LIB_NAMES
    PORTALS_NAL_STR = utcp
    PORTALS_LIB_NAMES = -lp3api -lp3lib -lp3rt -l$(PORTALS_NAL_STR)lib
  endif

#  COMM_LIBS += $(PORTALS_LIB_NAMES) 
   COMM_LIBS += -lsma 

endif

ifeq ($(ARMCI_NETWORK),VIA)
  COMM_DEFINES = -DVIA
  ifdef VIA_INCLUDE
    COMM_INCLUDES = -I$(VIA_INCLUDE)
  endif
  ifdef VIA_LIB
    COMM_LIBS = -L$(VIA_LIB)
  endif
  VIA_LIB_NAME = -lvipl
  COMM_LIBS += $(VIA_LIB_NAME)
endif

ifeq ($(ARMCI_NETWORK),OPENIB)
  COMM_DEFINES = -DOPENIB
  ifdef IB_INCLUDE
    COMM_INCLUDES = -I$(IB_INCLUDE)
  endif
  ifdef IB_LIB
    COMM_LIBS = -L$(IB_LIB)
  endif
  IB_LIB_NAME = -libverbs
  COMM_LIBS += $(IB_LIB_NAME)
endif

ifeq ($(ARMCI_NETWORK),MELLANOX)
  COMM_DEFINES = -DMELLANOX
  ifdef IB_INCLUDE
    COMM_INCLUDES = -I$(IB_INCLUDE)
  endif
  ifdef IB_LIB
    COMM_LIBS = -L$(IB_LIB)
  endif
  ifndef IB_LIB_NAME
    IB_LIB_NAME = -lvapi -lmosal -lmtl_common -lmpga -lpthread
  endif
  COMM_LIBS += $(IB_LIB_NAME)
endif

ifeq ($(ARMCI_NETWORK),ELAN3)
  ifdef ELAN3_INCLUDE
        QUADRICS_INCLUDE = $(ELAN3_INCLUDE)
  endif
  ifdef ELAN3_LIB
        QUADRICS_LIB = $(ELAN3_LIB)
  endif
  ARMCI_NETWORK = QUADRICS
endif

ifeq ($(ARMCI_NETWORK),QUADRICS)
  COMM_DEFINES = -DQUADRICS
  ifdef QUADRICS_INCLUDE
    COMM_INCLUDES = -I$(QUADRICS_INCLUDE)
  else
    ifeq ($(TARGET),DECOSF)
       COMM_INCLUDES = -I/usr/opt/rms/include
    endif
  endif
  ifdef QUADRICS_LIB
    COMM_LIBS = -L$(QUADRICS_LIB) -Wl,-rpath-link -Wl,$(QUADRICS_LIB)
  else
    ifeq ($(TARGET),DECOSF)
      COMM_LIBS = -L/usr/opt/rms/lib
    endif
  endif
  QUADRICS_LIB_NAME = -lshmem -lelan -lpthread
  COMM_LIBS += $(QUADRICS_LIB_NAME)
endif

ifeq ($(ARMCI_NETWORK),ELAN4)
  ifdef ELAN4_INCLUDE
    COMM_INCLUDES = -I$(ELAN4_INCLUDE)
  else
    ifeq ($(TARGET),DECOSF)
       COMM_INCLUDES = -I/usr/opt/rms/include
    else
       COMM_INCLUDES = -I/usr/lib/qsnet/elan4/include
    endif
  endif
  ifdef ELAN4_LIB
    COMM_LIBS = -L$(ELAN4_LIB) -Wl,-rpath-link -Wl,$(ELAN4_LIB)
  else
    ifeq ($(TARGET),DECOSF)
      COMM_LIBS = -L/usr/opt/rms/lib
    else
      COMM_LIBS = -L/usr/lib/qsnet/elan4/lib
    endif
  endif
  COMM_DEFINES += -DDOELAN4  -DQUADRICS
  ELAN4_LIB_NAME = -lelan4 -lelan -lpthread
  COMM_LIBS += $(ELAN4_LIB_NAME)
endif
    
ifeq ($(TARGET),LINUX64)
# _ALTIX= $(shell /bin/rpm -q -i sgi-mpt  2>&1|egrep Reloc|awk ' /Rel/  {print "Y"}')
     _SGIALTIX= $(shell if [ -r /proc/sgi_sn/system_serial_number ]; then /bin/echo Y; fi)
ifeq ($(_SGIALTIX),Y)
  COMM_LIBS += -lsma
endif
endif

ifeq ($(ARMCI_NETWORK), LAPI)
  COMM_DEFINES = -DLAPI
  ifdef LAPI_INCLUDE
    COMM_INCLUDES = -I$(LAPI_INCLUDE)
  endif
  ifdef LAPI_LIB
    COMM_LIBS = -L$(LAPI_LIB)
  endif
  LAPI_LIB_NAME = 
  COMM_LIBS += $(LAPI_LIB_NAME)
endif

ifeq ($(TARGET),LAPI)
ifdef LAPI2
  COMM_DEFINES += -DLAPI2
  COMM_INCLUDES = -I/u2/d3h325/lapi_vector_beta
  COMM_LIBS += /u2/d3h325/lapi_vector_beta/liblapi_r_dbg.a
endif
endif
ifeq ($(TARGET),LAPI64)
   COMM_LIBS += $(LAPI64LIBS)
endif

ifeq ($(TARGET),SOLARIS)
#  need gethostbyname from -lucb under earlier versions of Solaris
   COMM_LIBS += $(shell uname -r |\
                awk -F. '{ if ( $$1 == 5 && $$2 < 6 )\
                print "/usr/ucblib/libucb.a" }')
   COMM_LIBS +=  -lsocket -lrpcsvc -lnsl
endif
ifeq ($(TARGET),SOLARIS64)
   COMM_LIBS +=  -lsocket -lrpcsvc -lnsl
endif

ifeq ($(TARGET),FUJITSU-VPP)
   COMM_LIBS = -lmp -lgen -lpx -lelf -Wl,-J,-P
endif

ifeq ($(TARGET),FUJITSU-VPP64)
   COMM_LIBS = -lmp -lgen -lpx -lelf -Wl,-J,-P
endif
   
ifeq ($(TARGET),FUJITSU-AP)
   COMM_LIBS = -L/opt/FSUNaprun/lib -lmpl -lelf -lgen
endif

ifeq ($(TARGET),CRAY-YMP)
   COMM_LIBS = $(LIBCM)
endif

