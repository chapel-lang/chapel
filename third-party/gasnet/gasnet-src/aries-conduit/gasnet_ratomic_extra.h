/*   $Source: bitbucket.org:berkeleylab/gasnet.git/aries-conduit/gasnet_ratomic_extra.h $
 * Description: GASNet Remote Atomics API Header (aries specific additions)
 * Copyright 2017, The Regents of the University of California
 * Terms of use are as specified in license.txt
 */

#ifndef _IN_GASNET_RATOMIC_H
  #error This file is not meant to be included directly- clients should include gasnet_ratomic.h
#endif

#ifndef _GASNET_RATOMIC_EXTRA_H
#define _GASNET_RATOMIC_EXTRA_H

#if !GASNETC_BUILD_GNIRATOMIC
#error "gasnet_ratomic_extra.h included erroneously"
#endif

#endif // _GASNET_RATOMIC_EXTRA_H
