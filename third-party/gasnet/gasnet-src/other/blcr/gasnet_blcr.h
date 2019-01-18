/*   $Source: bitbucket.org:berkeleylab/gasnet.git/other/blcr/gasnet_blcr.h $
 * Description: header for internal definitions used in GASNet+BLCR implementation
 * Copyright 2016, Regents of the University of California
 * Terms of use are as specified in license.txt
 */

#ifndef _GASNET_BLCR_H
#define _GASNET_BLCR_H

#if !GASNET_BLCR
#error "gasnet_blcr.h included but BLCR support was not enabled"
#endif

GASNETI_BEGIN_EXTERNC

/* ------------------------------------------------------------------------------------ */
/* Conduit-independent support for BLCR-based checkpoint/restart
 */

extern uint64_t gasneti_checkpoint_guid;
extern uint32_t gasneti_checkpoint_sequence;
extern char *gasneti_checkpoint_jobdir;

extern uint64_t gasneti_checkpoint_mkguid(void);

extern void gasneti_checkpoint_init(gasneti_bootstrapBroadcastfn_t bcast_fn);

extern const char *gasneti_checkpoint_dir(const char *dir);
extern int gasneti_checkpoint_self(const char *dir);

/* Constituents of gasneti_checkpoint_self() allow conduit to include data before/after */
extern int gasneti_checkpoint_create(const char *dir);
extern int gasneti_checkpoint_write(int fd);

/* ------------------------------------------------------------------------------------ */
GASNETI_END_EXTERNC

#endif /* _GASNET_BLCR_H */
