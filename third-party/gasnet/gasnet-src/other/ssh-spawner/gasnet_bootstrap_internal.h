/*   $Source: bitbucket.org:berkeleylab/gasnet.git/other/ssh-spawner/gasnet_bootstrap_internal.h $
 * Description: GASNet conduit-independent ssh-based spawner (prototypes)
 * Copyright 2005, The Regents of the University of California
 * Terms of use are as specified in license.txt
 */

#ifndef _GASNET_BOOTSTRAP_INTERNAL_SSH_H
#define _GASNET_BOOTSTRAP_INTERNAL_SSH_H

extern int  gasneti_bootstrapInit_ssh(int *argc_p, char ***argv_p,
                                      gasnet_node_t *nodes_p, gasnet_node_t *mynode_p);
extern void gasneti_bootstrapFini_ssh(void);
extern void gasneti_bootstrapAbort_ssh(int exitcode) GASNETI_NORETURN;
extern void gasneti_bootstrapBarrier_ssh(void);
extern void gasneti_bootstrapExchange_ssh(void *src, size_t len, void *dest);
extern void gasneti_bootstrapAlltoall_ssh(void *src, size_t len, void *dest);
extern void gasneti_bootstrapBroadcast_ssh(void *src, size_t len, void *dest, int rootnode);
extern void gasneti_bootstrapSNodeBroadcast_ssh(void *src, size_t len, void *dest, int rootnode);
extern void gasneti_bootstrapCleanup_ssh(void);

#if GASNET_BLCR
extern int gasneti_bootstrapPreCheckpoint_ssh(int fd);
extern int gasneti_bootstrapPostCheckpoint_ssh(int fd, int is_restart);
extern int gasneti_bootstrapRollback_ssh(const char *dir);
#endif

#endif
