/*   $Source: bitbucket.org:berkeleylab/gasnet.git/other/pmi-spawner/gasnet_bootstrap_internal.h $
 * Description: GASNet conduit-independent pmi-based spawner (prototypes)
 * Copyright 2013, The Regents of the University of California
 * Terms of use are as specified in license.txt
 */

#ifndef _GASNET_BOOTSTRAP_INTERNAL_PMI_H
#define _GASNET_BOOTSTRAP_INTERNAL_PMI_H

extern int  gasneti_bootstrapInit_pmi(int *argc_p, char ***argv_p,
                                      gasnet_node_t *nodes_p, gasnet_node_t *mynode_p);
extern void gasneti_bootstrapFini_pmi(void);
extern void gasneti_bootstrapAbort_pmi(int exitcode) GASNETI_NORETURN;
extern void gasneti_bootstrapBarrier_pmi(void);
extern void gasneti_bootstrapExchange_pmi(void *src, size_t len, void *dest);
extern void gasneti_bootstrapAlltoall_pmi(void *src, size_t len, void *dest);
extern void gasneti_bootstrapBroadcast_pmi(void *src, size_t len, void *dest, int rootnode);
extern void gasneti_bootstrapCleanup_pmi(void);

#endif
