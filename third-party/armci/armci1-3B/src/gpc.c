/* $Id$ *****************************************************
  Prototype of Global Procedure Calls.
  July/03 JN - shared memory version  
  
*************************************************************/ 

#include <stdio.h>
#include "armcip.h"
#include "locks.h"
#include "gpc.h"

#define GPC_SLOTS 32 
#define GPC_OFFSET -100
static void *_table[GPC_SLOTS]={
(void*)0, (void*)0, (void*)0, (void*)0, (void*)0, (void*)0, (void*)0, (void*)0,
(void*)0, (void*)0, (void*)0, (void*)0, (void*)0, (void*)0, (void*)0, (void*)0,
(void*)0, (void*)0, (void*)0, (void*)0, (void*)0, (void*)0, (void*)0, (void*)0,
(void*)0, (void*)0, (void*)0, (void*)0, (void*)0, (void*)0, (void*)0, (void*)0};

#if (defined(GM) || defined(VAPI) || defined(QUADRICS)) && defined(ARMCI_ENABLE_GPC_CALLS)

/*\ callback functions must be registered -- user gets int handle back
\*/
int ARMCI_Gpc_register( int (*func) ())
{
  int handle =-1, candidate = 0;

  ARMCI_Barrier();
  do{
    if(!_table[candidate]){
      handle = candidate;
      _table[candidate]=func;
    }
    candidate++;
  }while(candidate < GPC_SLOTS && handle == -1);
  return(GPC_OFFSET-handle);
}

/*\ release/deassociate handle with previously registered callback function
\*/
void ARMCI_Gpc_release(int handle)
{
     int h = -handle + GPC_OFFSET;

     ARMCI_Barrier();
     if(h<0 || h >= GPC_SLOTS) armci_die("ARMCI_Gpc_release: bad handle",h);
     _table[h] = (void*)0;
}



/*\ Send Request to Execute callback function in a global address space 
 *  Arguments:
 *  f     - handle to the callback function
 *  p     - remote processor
 *  hdr   - header data - used to pack extra args for callback (local buffer) 
 *  hlen  - size of header data < ARMCI_GPC_HLEN
 *  data  - bulk data passed to callback (local buffer)
 *  dlen  - length of bulk data
 *  rhdr  - ptr to reply header (return args from callback)
 *  rhlen - length of buffer to store reply header < ARMCI_GPC_HLEN  
 *  rdata - ptr to where reply data from callback should be stored (local buf)
 *  rdlen - size of the buffer to store reply data  
 *  nbh   - nonblocking handle
 *  
\*/
int ARMCI_Gpc_exec(int h, int p, void  *hdr, int hlen,  void *data,  int dlen,
		   void *rhdr, int rhlen, void *rdata, int rdlen, gpc_hdl_t* nbh)
{
  int hnd = -h + GPC_OFFSET;
  int err = 0;
  armci_hdl_t *ahdl = (nbh ? &(nbh->ahdl): NULL);

  if(hnd <0 || hnd>= GPC_SLOTS) 
    err += fprintf(stderr, "ARMCI_Gpc_exec: bad callback handle %d: %d\n",hnd,GPC_SLOTS);
  if(!_table[hnd]) 
    err += fprintf(stderr, "ARMCI_Gpc_exec: NULL function %d",hnd);

  if(hlen<0 || hlen>=ARMCI_Gpc_get_hlen())
    err += fprintf(stderr, "ARMCI_Gpc_exec: Invalid send header size %d %d\n", hlen, ARMCI_Gpc_get_hlen());
  if(rhlen<0 || rhlen>=ARMCI_Gpc_get_hlen())
    err += fprintf(stderr, "ARMCI_Gpc_exec: Invalid recv header size %d %d\n", rhlen, ARMCI_Gpc_get_hlen());
  if(dlen<0 || dlen>=ARMCI_Gpc_get_dlen()) 
    err += fprintf(stderr, "ARMCI_Gpc_exec: Invalid send data size %d %d\n", dlen, ARMCI_Gpc_get_dlen());
  if(rdlen<0 || rdlen>=ARMCI_Gpc_get_dlen()) 
    err += fprintf(stderr, "ARMCI_Gpc_exec: Invalid recv data size %d %d\n", rdlen, ARMCI_Gpc_get_dlen());

  if(hlen>0 && hdr==NULL) 
    err += fprintf(stderr, "ARMCI_Gpc_exec: Null send header for non-zero header size %d\n", hlen);
  if(rhlen>0 && rhdr==NULL) 
    err += fprintf(stderr, "ARMCI_Gpc_exec: Null recv header for non-zero header size %d\n", rhlen);
  if(dlen>0 && data==NULL) 
    err += fprintf(stderr, "ARMCI_Gpc_exec: Null send data for non-zero data size %d\n", dlen);
  if(rdlen>0 && rdata==NULL) 
    err += fprintf(stderr, "ARMCI_Gpc_exec: Null recv data for non-zero header size %d\n", rdlen);

  if(p<0 || p >= armci_nproc)
    err += fprintf(stderr, "ARMCI_Gpc_exec: Invalid target processor id %d\n", p, armci_nproc);

  if(err)
    return FAIL;

  if(rhlen + rdlen == 0)
    armci_die("Zero reply header + data length not yet supported", 0);

  if(nbh)
    nbh->proc = p;
#if 1
  if(SAMECLUSNODE(p) && armci_nproc==1) {
    int rhsize, rdsize;
    int (*func)();

/*      fprintf(stderr, "%d:: armci gpc exec. SAMECLUSNODE\n", armci_me); */

    func = _table[hnd];
    if(func(p, armci_me, hdr, hlen, data, dlen, rhdr, rhlen, &rhsize,
	    rdata, rdlen, &rdsize, GPC_INIT) != GPC_DONE) {
      func(p, armci_me, hdr, hlen, data, dlen, rhdr, rhlen, &rhsize,
	   rdata, rdlen, &rdsize, GPC_WAIT);
    } 
#ifndef VAPI
    ARMCI_Fence(p);
#endif
    return 0;
  }
#endif

/*    fprintf(stderr, "%d:: armci gpc exec. invoking armci gpc\n", armci_me); */
  return armci_gpc(h, p, hdr, hlen,  data,  dlen,
		 rhdr, rhlen, rdata, rdlen, ahdl); 
}

int armci_gpc_local_exec(int h, int to, int from, void *hdr,   int hlen,
		     void *data,  int dlen,
		     void *rhdr,  int rhlen, 
		     void *rdata, int rdlen, int rtype) {
  int rhsize, rdsize;
  int (*func)();
  int hnd = -h + GPC_OFFSET;
  
  if(hnd <0 || hnd>= GPC_SLOTS) 
    armci_die2("armci_gpc_local_exec: bad callback handle",hnd,GPC_SLOTS);
  if(!_table[hnd]) armci_die("armci_gpc_local_exec: NULL function",hnd);
  
  func = _table[hnd];

  if(!SAMECLUSNODE(to)) 
    armci_die("armci_gpc_local_exec: GPC call to a different node received!", armci_me);

/*    func(to, from, hdr, hlen, data, dlen, rhdr, rhlen, &rhsize, */
/*  	 rdata, rdlen, &rdsize);  */
/*    return 0; */
  return func(to, from, hdr, hlen, data, dlen, rhdr, rhlen, &rhsize,
	      rdata, rdlen, &rdsize, rtype); 
}

/*\
 *   This is a template for the callback function
 *   The arguments are passed as specified in ARMCI_Gpc_exec
 *   In addition,
 *      rhsize specifies the actual size of reply header data returned
 *      rdsize specifies the actual size of reply data returned
\*/
int example_func(int to, int from, void *hdr,   int hlen,
                                    void *data,  int dlen,
                                    void *rhdr,  int rhlen, int *rhsize,
                                    void *rdata, int rdlen, int *rdsize,
		 int rtype);
     
     
/*\
 *  Translate pointer to memory on processor "proc"
 *  to be used in a callback function send by processor "from"
\*/
void * ARMCI_Gpc_translate(void *ptr, int proc, int from)
{
return ptr;
}


/*\ acquire lock in a callback function executed in context of processor "proc"
\*/
void ARMCI_Gpc_lock(int proc)
{
#if defined(CLUSTER) && !defined(SGIALTIX)
    int lock = (proc-armci_clus_info[armci_clus_id(proc)].master)%NUM_LOCKS;
#else
    int lock = 0;
#endif
    NATIVE_LOCK(lock,proc);
}

/*\ try acquire lock in a callback function to be executed in context of
 *  processor "proc"
 *  return value: 1 - success
 *                0 - failure (already locked by another thread)
\*/
int ARMCI_Gpc_trylock(int proc)
{
armci_die("ARMCI_Gpc_trylock: not yet implemented",0);
return 0;
}

/*\ release lock in a callback function executed in context of processor "proc"
\*/
void ARMCI_Gpc_unlock(int proc)
{
#if defined(CLUSTER) && !defined(SGIALTIX)
    int lock = (proc-armci_clus_info[armci_clus_id(proc)].master)%NUM_LOCKS;
#else
    int lock = 0;
#endif
    NATIVE_UNLOCK(lock,proc);
}

void ARMCI_Gpc_init_handle(gpc_hdl_t *nbh) {
  nbh->proc = armci_me;
  ARMCI_INIT_HANDLE(&nbh->ahdl);
}

void ARMCI_Gpc_wait(gpc_hdl_t *nbh) {
  if(SAMECLUSNODE(nbh->proc))
    return;
  ARMCI_Wait(&nbh->ahdl);
}

void ARMCI_Gpc_test(gpc_hdl_t *nbh) {
  if(SAMECLUSNODE(nbh->proc))
    return;
  ARMCI_Test(&nbh->ahdl);
}

#define ARMCI_GPC_HLEN 65536
#define ARMCI_GPC_DLEN 65536
int ARMCI_Gpc_get_hlen() {
  return ARMCI_GPC_HLEN;
}

int ARMCI_Gpc_get_dlen() {
  return ARMCI_GPC_DLEN;
}

#endif

