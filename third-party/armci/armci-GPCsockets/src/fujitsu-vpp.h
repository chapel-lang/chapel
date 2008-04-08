/* This file "fujitsu-vpp.h" #defines common set of macros used in
 * the Global Array and TCGMSG-MPI ports to Fujitsu VX and VPP systems.
 *
 * Author:       Jarek Nieplocha <j_nieplocha@pnl.gov>
 * Organization: Pacific Northwest National Laboratory, Richland, WA 99552
 * Date:         08.21.1997
 * History:      11.27.1997 added optimized 2-D array copy macros
 *               03.10.2000 Both strided operations made optional
 *
 * Notes:
 *     1. This file contains calls to an undisclosed Fujitsu MPlib library and
 *        is NOT intended for public distribution.
 */

#ifndef _FUJITSU_VPP_H
#define _FUJITSU_VPP_H

#ifdef __sparc
   /* the AP3000 version is limited */
#  include "/opt/FSUNaprun/include/vpp_mplib.h"
#  define VPP_NbWrite VPP_Write 
#else
   /* specify full path to MPlib include file (here is my account on Fecit VX)*/
#  include "/home/jniep/include/mplib.h"
#endif

#define TOTAL_NUM_SEM 128          /* number of MPlib semaphores per node */
#define NUM_SEM  16                /* we will use only that+1 many semaphores */
#define SEM_BASE (TOTAL_NUM_SEM-NUM_SEM-2)
#define NXTV_SEM SEM_BASE+NUM_SEM+1  /*nxtval server semaphore in tcgmsg-mpi */

#define MPLIB_TERMINATE {\
        fflush(stdout);sleep(1);\
        fprintf(stderr,"%d: MPlib call failed: %s in line %d compiled on %s\n",\
                VPP_MyPe(),__FILE__,__LINE__,__DATE__);\
        fprintf(stdout,"%d: MPlib call failed: %s in line %d compiled on %s\n",\
                VPP_MyPe(),__FILE__,__LINE__,__DATE__);\
        fflush(stdout); sleep(1);\
        VPP_Abort();\
}

#define NATIVE_LOCK0(proc,mtx) if(VPP_SemWait((PROC)(proc),(mtx)))MPLIB_TERMINATE
#define NATIVE_UNLOCK0(proc,mtx) if(VPP_SemPost((PROC)(proc),(mtx)))MPLIB_TERMINATE

#define NAT_LOCK(proc) if(VPP_SemWait((PROC)(proc),SEM_BASE))MPLIB_TERMINATE
#define NAT_UNLOCK(proc) if(VPP_SemPost((PROC)(proc),SEM_BASE))MPLIB_TERMINATE

#define NATIVE_BARRIER VPP_Barrier
#define NATIVE_BARRIER__() MPI_Barrier(MPI_COMM_WORLD)


/**************************** MEMORY COPY macros ************************/
/*copy n words from remote memory (proc) at (src) to local memory (dst) */
#define CopyElemFrom(src,dst,n,proc) \
        if(VPP_Read((PROC)(proc),(ADDRP) (src), (ADDRP)(dst), n*sizeof(long)))\
        MPLIB_TERMINATE

/*copy n words to remote memory (proc) at (dst) from local memory (src) */
#define CopyElemTo(src,dst,n,proc)   \
        if(VPP_Write((PROC)(proc),(ADDRP)(dst),(ADDRP) (src),n*sizeof(long)))\
        MPLIB_TERMINATE

/*copy n bytes from remote memory (proc) at (src) to local memory (dst) */
#define CopyFrom(src,dst,n,proc) \
        if(VPP_Read((PROC)(proc),(ADDRP) (src), (ADDRP)(dst), n))\
        MPLIB_TERMINATE

/*copy n bytes o remote memory (proc) at (dst) from local memory (src) */
#define CopyTo(src,dst,n,proc)   \
        if(VPP_Write((PROC)(proc),(ADDRP)(dst),(ADDRP) (src),n ))\
        MPLIB_TERMINATE

/* Memory copy for 2-dimensional array patches between local and remote memory: 
 * we use nonblocking read/write operations followed by a blocking read/write
 * of 1 byte to force completion of outstanding nonblocking operations.
 * This works because of the "in-order" rule for remote memory operations.
 * This code is using byte- rather than word- interface.
 */

#ifdef VPP_STRIDED_WRITE
#  define CopyPatchTo(src, ld_src, dst, ld_dst, blocks, bytes, proc){\
        if(VPP_WriteBothStrided((PROC)(proc), (ADDRP)dst, (ADDRP)src,\
                    bytes, ld_dst, ld_src, bytes*blocks))MPLIB_TERMINATE; }
#else
#  define CopyPatchTo(src, ld_src, dst, ld_dst, blocks, bytes, proc){\
        int _iii, _stat=0, _bytes2copy=1;\
        char *ps=(char*)src, *pd=(char*)dst;\
        if((blocks)>1)for (_iii=0;_iii<(blocks);_iii++){\
             _stat += VPP_NbWrite((PROC)(proc),(ADDRP)pd,(ADDRP)ps,(bytes));\
             ps += (ld_src);\
             pd += (ld_dst);\
        }else _bytes2copy=(bytes);\
        _stat += VPP_Write((PROC)(proc),(ADDRP)(dst),(ADDRP)(src),_bytes2copy);\
        if(_stat)MPLIB_TERMINATE; }
#endif

#ifdef VPP_STRIDED_READ
#  define CopyPatchFrom(src, ld_src, dst, ld_dst, blocks, _bytes, proc){\
        if(VPP_ReadBothStrided((PROC)(proc), (ADDRP)src, (ADDRP)dst,\
                    _bytes, ld_src, ld_dst, _bytes*blocks))MPLIB_TERMINATE;}
#else
#  define CopyPatchFrom(src, ld_src, dst, ld_dst, blocks, _bytes, proc){\
        int _iii, _stat=0, _bytes2copy=1;\
        char *ps=(char*)src, *pd=(char*)dst;\
        if((blocks)>1)for (_iii=0;_iii<(blocks);_iii++){\
             _stat += VPP_Read((PROC)(proc),(ADDRP)ps,(ADDRP)pd,(_bytes));\
             ps += (ld_src);\
             pd += (ld_dst);\
        }else _bytes2copy=(_bytes);\
        _stat += VPP_Read((PROC)(proc),(ADDRP)(src),(ADDRP)(dst),_bytes2copy);\
        if(_stat)MPLIB_TERMINATE; }
#endif

#if 0
#define MAX_IDS 10
static DRWD *_id[MAX_IDS];

#define CopyPatchFromXX(src, ld_src, dst, ld_dst, blocks, _bytes, proc){\
        int _iii, _stat=0, _idx;\
        char *ps=(char*)src, *pd=(char*)dst;\
printf("lds=%d ldd=%d bl=%d bytes=%d proc=%d\n",ld_src,ld_dst,blocks, _bytes, proc); fflush(stdout);\
        if((blocks)>1){\
          for (_iii=0; _iii<(blocks); _iii++){\
             _idx = _iii%MAX_IDS;\
             if(_iii >= MAX_IDS)_stat += VPP_ReadDone(_id[_idx]);\
             _id[_idx]=VPP_StartRead((PROC)(proc),(ADDRP)ps,(ADDRP)pd,(_bytes));\
             ps += (ld_src);\
             pd += (ld_dst);\
          }\
          for (_idx=0; _idx < (blocks) && _idx < MAX_IDS; _idx++){\
              _stat += VPP_ReadDone(_id[_idx]);\
          }\
        }else _stat = VPP_Read((PROC)(proc),(ADDRP)(src),(ADDRP)(dst),(_bytes));\
        if(_stat)MPLIB_TERMINATE;\
}

#define CopyPatchFromYY(src, ld_src, dst, ld_dst, blocks, _bytes, proc){\
        int _iii, _stat=0, _bytes2copy=1;\
        char *ps=(char*)src, *pd=(char*)dst;\
        if((blocks)>1) {   for (_iii=0;_iii<(blocks);_iii++){\
             _id[0]= VPP_StartRead((PROC)(proc),(ADDRP)ps,(ADDRP)pd,(_bytes));\
             ps += (ld_src);\
             pd += (ld_dst);\
          }\
          _stat += VPP_ReadDone(_id[0]);\
        }else _bytes2copy=(_bytes);\
        _stat += VPP_Read((PROC)(proc),(ADDRP)(src),(ADDRP)(dst),_bytes2copy);\
        if(_stat)MPLIB_TERMINATE;\
}
#endif


#endif
