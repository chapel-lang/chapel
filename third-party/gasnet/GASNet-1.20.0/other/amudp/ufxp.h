/*   $Source: /var/local/cvs/gasnet/other/amudp/ufxp.h,v $
 *     $Date: 2004/08/26 04:53:50 $
 * $Revision: 1.2 $
 * Description: 
 * Copyright 2004, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */
#ifndef UFXP_H
#define UFXP_H

#include "ueth.h"

#define	ufxp_addr_t			ueth_addr_t
#define	ufxp_header_t			ueth_header_t
#define	ufxp_addr_hook_t		ueth_addr_hook_t

#define	UFXP_HEADERLENGTH		UETH_HEADERLENGTH
#define	UFXP_MAXPACKETSIZE		UETH_MAXPACKETSIZE
#define	UFXP_ALIGNMENT			UETH_ALIGNMENT

#define UFXP_OK				UETH_OK
#define UFXP_ERR_TIMEDOUT		UETH_ERR_TIMEDOUT
#define UFXP_ERR_MEMRESOURCE		UETH_ERR_MEMRESOURCE
#define UFXP_ERR_NICRESOURCE		UETH_ERR_NICRESOURCE
#define	UFXP_ERR_NOTINIT		UETH_ERR_NOTINIT

#define	ufxp_getcputime			ueth_getcputime
#define	ufxp_getustime			ueth_getustime

#define	ufxp_init			ueth_init
#define	ufxp_allocatepool		ueth_allocatepool
#define	ufxp_getaddress			ueth_getaddress
#define	ufxp_setaddresshook		ueth_setaddresshook
#define	ufxp_set_packet_destination	ueth_set_packet_destination
#define	ufxp_send_preset		ueth_send_preset
#define	ufxp_send			ueth_send
#define ufxp_query_send			ueth_query_send
#define	ufxp_cancel_send		ueth_cancel_send
#define	ufxp_recv			ueth_recv
#define	ufxp_freerxbuf			ueth_freerxbuf
#define	ufxp_terminate			ueth_terminate

#endif
