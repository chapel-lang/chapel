/*   $Source: /var/local/cvs/gasnet/other/ammpi/ammpi_spmd.h,v $
 *     $Date: 2009/03/29 03:53:22 $
 * $Revision: 1.15 $
 * Description: AMMPI Header for SPMD interface
 * Copyright 2000, Dan Bonachea <bonachea@cs.berkeley.edu>
 */

#ifndef __AMMPI_SPMD_H
#define __AMMPI_SPMD_H

#include <ammpi.h>

AMMPI_BEGIN_EXTERNC

#undef  AMMPI_SPMDStartup
#define AMMPI_SPMDStartup _CONCAT(AMMPI_SPMDStartup_AMMPI,AMMPI_DEBUG_CONFIG)

/* ------------------------------------------------------------------------------------ */
/* AMMPI SPMD Entry Points */

typedef int (*spawnfn_t)(int nproc, int argc, char **argv);
/* return non-zero if successful */


extern int AMMPI_SPMDStartup(int *argc, char ***argv,
                             int networkdepth, 
                             uint64_t *networkpid,
                             eb_t *eb, ep_t *ep); 
  /* should be always be called by program to initialize parallel job before parsing command line arguments
   *  must be called collectively by all worker processes
   * On worker processors, this call will modify the argc/argv params, call AM_Init,
   * and then return a bundle and endpoint properly configured for use with the SPMD job 
   * (translation table will be filled in, AM_SetExpectedResources called, and each worker given a unique tag)
   * worker processors should setup handler table for the endpoint and call SPMDBarrier before starting communication
   * Arguments:
   *  argc/argv - should be the unchanged ones passed to main, may return changed
   *  networkdepth - desired network depth hint (0 for default) (ignored on workers)
   *  networkpid - returns a globally unique pid for this job which is identical on all works (can be NULL for don't care) (ignored on master)
   *  eb, ep - variables to receive newly allocated bundle and endpoint on workers (ignored on master)
   */
extern int AMMPI_SPMDExit(int exitcode); 
  /* terminate the parallel job with given exit code (also handles AM_Terminate)
   */

extern int AMMPI_SPMDSetExitCallback(void (*fp)(int));
  /* register a function to be called when AMMPI_SPMDExit is called by any node
   * exit code is passed
   */
extern void (*AMMPI_SPMDkillmyprocess)(int);
  /* function used to finally kill the process (_exit by default) */

extern int AMMPI_SPMDSetThreadMode(int usingthreads, const char **provided_level, int *argc, char ***argv);
  /* attempt to request thread support level indicated by usingthreads from MPI-2 
   * returns non-zero on success or zero on failure, 
   * and in all cases returns the reported suport level in *provided_level
   * must be called before AMMPI_SPMDStartup 
   */

extern int AMMPI_SPMDIsWorker(char **argv); 
  /* given the initial command line arguments, determine whether this process is a 
   * worker process created by the AMMPI SPMD job startup API
   */
extern int AMMPI_SPMDNumProcs(void); /* return the number of processors in the parallel job */
extern int AMMPI_SPMDMyProc(void);   /* return a zero-based unique identifier of this processor in the parallel job */

extern int AMMPI_SPMDBarrier(void); 
/* block until all SPMD processors call this function, 
 * and poll the SPMD endpoint while waiting
 * a slow, but functional barrier that is advisable to call after setting up handlers
 * but before making transport calls, to prevent returned messages due to races 
 */

extern int AMMPI_SPMDAllGather(void *source, void *dest, size_t len);
extern int AMMPI_SPMDBroadcast(void *buf, size_t len, int rootid);
/* expose the MPI_AllGather & MPI_Bcast functions which can be useful for bootstrapping 
   AMMPI_SPMDAllGather: gather len bytes from source buf on each node, concatenate them and write 
   them into the dest buffer (which must have length len*numnodes) in rank order
   AMMPI_SPMDBroadcast: broadcast len bytes from buf on node rootid to buf on all nodes
   may only be used after startup - beware, these functions block without polling
*/

/* ------------------------------------------------------------------------------------ */
/* standardized AM-2 extensions */

#define AMX_SPMDgetenvMaster      getenv
#define AMX_SPMDBarrier           AMMPI_SPMDBarrier
#define AMX_SPMDNumProcs          AMMPI_SPMDNumProcs
#define AMX_SPMDMyProc            AMMPI_SPMDMyProc
#define AMX_SPMDExit              AMMPI_SPMDExit
#define AMX_SPMDSetExitCallback   AMMPI_SPMDSetExitCallback          
#define AMX_SPMDkillmyprocess     AMMPI_SPMDkillmyprocess
#define AMX_SPMDIsWorker(x)       (1)
#define AMX_SPMDAllGather         AMMPI_SPMDAllGather

#define AMX_SPMDStartup(pargc, pargv, networkdepth, pnetworkpid, peb, pep) \
      AMMPI_SPMDStartup((pargc), (pargv), (networkdepth), (pnetworkpid), (peb), (pep))

AMMPI_END_EXTERNC

#endif
