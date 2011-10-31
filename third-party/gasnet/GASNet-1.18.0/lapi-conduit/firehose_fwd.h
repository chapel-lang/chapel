/*   $Source: /var/local/cvs/gasnet/lapi-conduit/firehose_fwd.h,v $
 *     $Date: 2008/03/05 23:54:24 $
 * $Revision: 1.3 $
 * Description: Firehose forward declarations
 * Copyright 2004, Christian Bell <csbell@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */
#include <inttypes.h>
#include <stdlib.h>
#include <lapi.h>

/* Exactly one one of the next two firehose impementations must be defined */
#define FIREHOSE_REGION

#ifdef FIREHOSE_REGION
/* Define the next preprocessor directive to allow firehose clients to
 * attach a client type in opaque firehose_region_t.  It can only be
 * defined within the context of FIREHOSE_REGION. */
#define FIREHOSE_CLIENT_T

#ifdef FIREHOSE_CLIENT_T
typedef
struct _firehose_client_t {
	lapi_user_pvo_t	pvo;	
}
firehose_client_t;
#endif

#endif

/* Remote and completion callbacks
 *
 * Define the next preprocessor directive to allow firehose completion
 * handlers to run within an AM Handler context */
#undef FIREHOSE_COMPLETION_IN_HANDLER

/* Define the next preprocessor directive to allow remote firehose
 * callbacks to run within an AM Handler context */
#define FIREHOSE_REMOTE_CALLBACK_IN_HANDLER

/* Remote callback arguments type
 *
 * This type is passed to the callback that can be optionally invoked
 * on the remote node when the firehose library has completed a
 * firehose move on the requested node (see 'Firehose Remote Pin'
 * documentation in firehose.h).
 *
 * This type must be defined, even if it is not used by the client.
 *
 */
typedef struct _firehose_remotecallback_args_t {
	uintptr_t	local_addr;
	uintptr_t	remote_addr;
	size_t		nbytes;
}
firehose_remotecallback_args_t;

/* Connection-oriented pinning networks
 *
 * Some networks actually need to connect/disconnect to remote
 * segments through their own API calls.  For these networks, the
 * client must execute additional function calls for regions to be
 * pinned and unpinned.
 *
 * The firehose interface defines two preprocessor directives:
 *   FIREHOSE_BIND_CALLBACK allows a client to bind to a remote region
 *   			    that has been pinned in a network-specific
 *   			    manner;
 *
 *   FIREHOSE_UNBIND_CALLBACK allows a client to unbind from a remote
 *                            region in a network-specific manner
 *                            before it is effectively unpinned;
 */

/* Define the next preprocessor directive to allow the client to bind
 * to newly pinned regions once a move request completes locally.
 *
 * If active, this callback runs with the newly pinned regions once
 * the node initiating the move request receives the target's node's
 * reply. 
 */
#undef FIREHOSE_BIND_CALLBACK

/* Define the next preprocessor directive to allow the client to
 * unbind to regions locally once the firehose interface selects the
 * region for unpinning before any unpin messages are sent.
 *
 * If active, this callback runs with the regions selected for
 * unpinning and the target node the regions were mapped to.
 */
#undef FIREHOSE_UNBIND_CALLBACK

/* Strict export pinning networks
 *
 * Most networks work under the assumption that a pinned region of
 * memory is memory accessible to every node on the cluster (possibly
 * through a key or no key at all).  Some networks may require control
 * over who the region is exported to.  These networks may want to
 * define one of the following two preprocessor directives:
 *    FIREHOSE_EXPORT_CALLBACK allows a client to export a local
 *			       region for remote access in a
 *			       network-specific manner;
 *
 *    FIREHOSE_UNEXPORT_CALLBACK allows a client to unexport a local
 *				 region for remote access in a
 *				 network-specific manner;
 */

/* Define the next preprocessor directive to allow the client to
 * export a region for remote access once a move request is received
 * locally.
 *
 * If active, this callback runs with the local regions requested for
 * pinning by a remote node.  These regions will be pinned before the
 * callback but may already be exported to other nodes.
 */
#undef FIREHOSE_EXPORT_CALLBACK

/* Define the next preprocessor directive to allow the client to
 * unexport a region for remote access once a move request is received
 * locally.  An unexport call typically balances an export call on the
 * same region.
 *
 * If active, this callback runs with the local regions requested for
 * unpinning by a remote node.  This callback is run prior to
 * unpinning the local region (although the local region may not be
 * subsequently unpinned if it has been exported to other nodes).
 */
#undef FIREHOSE_UNEXPORT_CALLBACK

