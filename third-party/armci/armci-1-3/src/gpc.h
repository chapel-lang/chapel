#ifndef __GPCDEF
#if defined(ARMCI_ENABLE_GPC_CALLS)
#define __GPCDEF

#include "armci.h"

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif   

#define GPC_INIT 1
#define GPC_PROBE 2
#define GPC_WAIT 3
#define GPC_DONE 4
#define GPC_PENDING 5

typedef struct {
  int proc;
  armci_hdl_t ahdl;
}gpc_hdl_t;

/*  #define ARMCI_GPC_HLEN 1024 */
/*  #define ARMCI_GPC_DLEN 1024*1024 */
extern int ARMCI_Gpc_register( int (*func) ());
extern void ARMCI_Gpc_release(int handle); 
extern void * ARMCI_Gpc_translate(void *ptr, int proc, int from); 
extern void ARMCI_Gpc_lock(int proc); 
extern void ARMCI_Gpc_unlock(int proc); 
extern int ARMCI_Gpc_trylock(int proc); 
extern int ARMCI_Gpc_exec(int h,int p, void *hdr, int hlen, void *data,int dlen,
			  void *rhdr, int rhlen, void *rdata, int rdlen,
			  gpc_hdl_t* nbh);
extern int ARMCI_Get_gpc_hlen();
extern int ARMCI_Get_gpc_dlen();

extern void ARMCI_Gpc_init_handle(gpc_hdl_t *nbh);
extern void ARMCI_Gpc_wait(gpc_hdl_t *nbh);

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
#endif
