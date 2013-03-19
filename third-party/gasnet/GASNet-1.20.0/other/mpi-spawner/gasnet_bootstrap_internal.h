/*   $Source: /var/local/cvs/gasnet/other/mpi-spawner/gasnet_bootstrap_internal.h,v $
 *     $Date: 2005/07/03 14:33:39 $
 * $Revision: 1.3 $
 * Description: GASNet conduit-independent mpi-based spawner (prototypes)
 * Copyright 2005, The Regents of the University of California
 * Terms of use are as specified in license.txt
 */

#ifndef _GASNET_BOOTSTRAP_INTERNAL_MPI_H
#define _GASNET_BOOTSTRAP_INTERNAL_MPI_H

extern void gasneti_bootstrapInit_mpi(int *argc_p, char ***argv_p,
                                      gasnet_node_t *nodes_p, gasnet_node_t *mynode_p);
extern void gasneti_bootstrapFini_mpi(void);
extern void gasneti_bootstrapAbort_mpi(int exitcode) GASNETI_NORETURN;
extern void gasneti_bootstrapBarrier_mpi(void);
extern void gasneti_bootstrapExchange_mpi(void *src, size_t len, void *dest);
extern void gasneti_bootstrapAlltoall_mpi(void *src, size_t len, void *dest);
extern void gasneti_bootstrapBroadcast_mpi(void *src, size_t len, void *dest, int rootnode);

#endif
