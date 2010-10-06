/*   $Source: /var/local/cvs/gasnet/sci-conduit/gasnet_core_internal.h,v $
 *     $Date: 2009/09/18 23:33:42 $
 * $Revision: 1.16 $
 * Description: GASNet sci conduit header for internal definitions in Core API
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 *				   Hung-Hsun Su <su@hcs.ufl.edu>
 *				   Burt Gordon <gordon@hcs.ufl.edu>
 * Terms of use are as specified in license.txt
 */

#ifndef _GASNET_CORE_INTERNAL_H
#define _GASNET_CORE_INTERNAL_H

#include <gasnet_internal.h>

/*  SCI conduit specific headers */
#ifndef _SISCI_HEADERS
#define _SISCI_HEADERS
#include "sisci_types.h"
#include "sisci_api.h"
#include "sisci_error.h"
#endif

/*Necessary use of some C definitions*/
#include <sys/types.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/********************************************************
					Data Structure
********************************************************/

/*  SCI conduit specific data structures */
typedef struct
{
	gasnet_node_t source_id;
	uint8_t msg_number;
} gasnetc_sci_token_t;

/* NOTE: if any change is made to the header, remember to make change to the gasnetc_get_header_size () which dictates how many bytes needs to be transfer over the network */
typedef struct
{
        uint16_t header; /*  handler (8 bits) + msg type (1 bit, request/reply) + AM type (2 bits) + num_arg (5 bits) */
                         /*  msg type: 0 = request; 1 = reply; */
                         /*  AM type: 0 = short; 1 = medium; 2 = long; 3 = control (basic return msg to free mls); */
        uint16_t header_size; /* only 1 byte is needed as the longest header is < 80 */
	uint32_t payload_size;
	gasnet_handlerarg_t args[16];
} gasnetc_ShortMedium_header_t;

typedef struct
{
	uint16_t header;
        uint16_t header_size;
	uint32_t payload_size;
	void * payload;
	gasnet_handlerarg_t args[16];
} gasnetc_Long_header_t;              /*  Max size for short/medium header = 2 (header) + 2 (pedding) + 4 (payload_size) + 4 (payload address) + 16 * 4 (arguments) = 72 */

typedef uint8_t bool;

typedef void (*gasnetc_handler_short) (gasnet_token_t token, ...);
typedef void (*gasnetc_handler_mediumlong)(gasnet_token_t token, void *buf, size_t nbytes, ...);

/********************************************************
					Constants
********************************************************/

/*  whether or not to use spin-locking for HSL's */
#define GASNETC_HSL_SPINLOCK 1
/* ------------------------------------------------------------------------------------ */
#define GASNETC_SCISAFE(fncall) do {                                      \
   fncall;                                                                \
   if_pf (error != SCI_ERR_OK)                                            \
      gasneti_fatalerror ("(%d) %s failed at %s:%i - Error code: 0x%x\n", \
       gasneti_mynode, #fncall, __FILE__, __LINE__, error);               \
 } while (0)

/* ------------------------------------------------------------------------------------ */
#define GASNETC_HANDLER_BASE  1 /* reserve 1-63 for the core API */
#define _hidx_gasnetc_auxseg_reqh             (GASNETC_HANDLER_BASE+0)
/* add new core API handlers here and to the bottom of gasnet_core.c */

/* ------------------------------------------------------------------------------------ */
/* AM category (recommended impl if supporting PSHM) */
typedef enum {
  gasnetc_Short=0,
  gasnetc_Medium=1,
  gasnetc_Long=2
} gasnetc_category_t;

/*  SCI conduit specific constants */
#define GASNETC_SCI_NO_CALLBACK				NULL
#define GASNETC_SCI_NO_FLAGS				0
#define GASNETC_SCI_FILE 					"gasnet_nodes.sci"
#define	GASNETC_SCI_FAST_SEG				1048576
#define GASNETC_SCI_TRUE					1
#define GASNETC_SCI_FALSE					0
#define GASNETC_SCI_MAX_REQUEST_MSG			2
#define GASNETC_SCI_MAX_HANDLER_NUMBER		256
#if PLATFORM_ARCH_64
	#define GASNETC_SCI_COMMAND_MESSAGE_SIZE	(2048+80) /*  max medium payload size + size of the longest header -- 64-bit machines*/
#else
	#define GASNETC_SCI_COMMAND_MESSAGE_SIZE	(2048+76) /*  max medium payload size + size of the longest header*/
#endif
#define GASNETC_SCI_NUM_DMA_QUEUE			1
#define GASNETC_SCI_MAX_DMA_QUEUE_USAGE		1
#define GASNETC_SCI_REQUEST					0
#define GASNETC_SCI_REPLY					1
#define GASNETC_SCI_CONTROL					0
#define GASNETC_SCI_SHORT					1
#define GASNETC_SCI_MEDIUM					2
#define GASNETC_SCI_LONG					3
#define GASNETC_SCI_CONTROL_FLAG			10
#define GASNETC_SCI_SHORT_FLAG				11
#define GASNETC_SCI_MEDIUM_FLAG				12
#define GASNETC_SCI_LONG_FLAG				13
#define GASNETC_SCI_ONE_MB					1048576 /* define 1 MB */
#define GASNETC_SCI_TIMEOUT_SEC				20

/********************************************************
					Global Variables
********************************************************/
extern int gasnetc_bigphy_enable;
extern sci_desc_t			*gasnetc_sci_sd;
extern sci_desc_t			*gasnetc_sci_sd_gb;
extern sci_desc_t			*gasnetc_sci_sd_remote;
extern sci_desc_t			*gasnetc_sci_sd_long;
extern sci_local_segment_t		*gasnetc_sci_localSegment;
extern sci_remote_segment_t		*gasnetc_sci_remoteSegment_long;
extern unsigned int			gasnetc_sci_localAdapterNo;
extern sci_map_t			*gasnetc_sci_localMap;
extern sci_map_t			*gasnetc_sci_remoteMap;
extern sci_map_t			*gasnetc_sci_remoteMap_gb;
extern void 				**gasnetc_sci_global_ready;
extern void				*gasnetc_sci_handler_table[GASNETC_SCI_MAX_HANDLER_NUMBER];
extern bool				*gasnetc_sci_msg_loc_status;
extern bool 				*gasnetc_sci_msg_flag;
extern int				gasnetc_sci_internal_barrier_flag;
extern gasneti_mutex_t			gasnetc_sci_request_mutex;

/********************************************************
                                        BARRIER FUNCTIONS
********************************************************/

void gasnetc_sci_barrier_notify (int barrier_value);

int gasnetc_sci_barrier_try (int barrier_value);

int gasnetc_sci_barrier_wait (int barrier_value);

/********************************************************
					SCI SETUP FUNCTIONS
********************************************************/

/* the first step to exiting */
void gasnetc_sci_call_exit(unsigned int sig);

/*  BARRIER FUNCTION */
/*  Creates a temporary segment and connects to all other nodes' */
/*  temp segment, writes a 1 in their segment at index:gasneti_mynode, */
/*  then waits until everybody has written a 1 into all of our index spots */
/*  destroys the segment and continues. */
void gasnetc_sci_internal_Barrier(void);

/*  Parses all the SCI Ids and places them in gasnetc_sci_SCI_Ids. */
/*  The corresponding GASNet node ID is the index of the location of the SCI id in the */
/*  array. */
int gasnetc_parseSCIIds(FILE *node_info);

/*  Returns the Max_local_Segment size in bytes based on available */
/*  free mem on the system and writes the size to a global file */
void gasnetc_get_free_mem(void);

/*  Returns the maximum global segment size. This is the minimum of the segment */
/*  sizes available over the whole cluster. Return -1 if not all node information */
/*  is in the file, throws an error otherwise. */
void gasnetc_getSCIglobal_seg(void);

/*  This uses the number given to it to create the segments needed by the command */
/*  region. It will leave an open spot for the gasnet segment space to be */
/*  the next to last segment. It will be created in GASNet Attach. */
void gasnetc_SCI_Create_Connections(void);

/*  Connects all the command regions and then all the global ready bytes */
/*  across all nodes. */
int gasnetc_SCI_connect_cmd(void);

/*  This function finds the total number of nodes that will run GASNet as well as */
/*  assign GASNet Node IDS to each SCI ID. Additionally, the total amount of free */
/*  memory on the system is determined and a percentage is used as the MAX_local_Seg */
/*  size. */
void gasnetc_SCIInit(void);

/*  Create the payload (GASNET segment) segment and set it available. */
void* gasnetc_create_gasnetc_sci_seg(uintptr_t *segsize, int index);

/*  This waits for all nodes to write their segment sizes and addresses to the */
/*  mailboxes. Then gets all the segment info and places int the segment array */
/*  info. */
void gasnetc_get_SegInfo(gasnet_seginfo_t* gasnetc_sci_seginfo, uintptr_t segsize,  void * segbase);

/********************************************************
                                Local/Remote segment Info
********************************************************/

/*  Return the memory address (ptr, virtual) to the control segment on the remote node */
GASNETI_INLINE(gasnetc_gr_get_addr)
void * gasnetc_gr_get_addr (gasnet_node_t RemoteID)
{
        return gasnetc_sci_global_ready[RemoteID];
}

/*  Return the remote map handler (SCI) created for the dedicated segment on the remote node */
GASNETI_INLINE(gasnetc_rs_get_rmap)
sci_map_t gasnetc_rs_get_rmap (gasnet_node_t RemoteID)
{
        return gasnetc_sci_remoteMap[RemoteID];
}

/*  Calculates the appropriate offset required based on the input address and the starting address of a remote payload segment */
GASNETI_INLINE(gasnetc_rs_get_offset)
int gasnetc_rs_get_offset (gasnet_node_t RemoteID, void * dest_addr)
{
        void * dest_base_addr = gasneti_seginfo[RemoteID].addr;
        if (dest_addr >= dest_base_addr)
        {
            int offset = (int) (((uint8_t *) dest_addr) - ((uint8_t *) dest_base_addr));
            if ((offset >= 0) && (offset <= gasneti_seginfo[RemoteID].size))
            {
                    return offset;
            }
            else
            {
                   gasneti_fatalerror ("(%d) request addr of %p is out of range from the base addr %p at node %d (size = %d)", gasneti_mynode, dest_addr, dest_base_addr, RemoteID, gasneti_seginfo[RemoteID].size);
            }
        }
        else
        {
            gasneti_fatalerror ("(%d) request addr of %p is smaller than the base addr %p at node %d", gasneti_mynode, dest_addr, dest_base_addr, RemoteID);
        }
}

/********************************************************
				Message Location Status
		-- Use by Local node to keep track of --
		--	command msgs to all other nodes   --
********************************************************/

/*  Check if the given location is free or not */
GASNETI_INLINE(gasnetc_mls_status)
bool gasnetc_mls_status (gasnet_node_t RemoteID, uint8_t msg_number)
{
        return gasnetc_sci_msg_loc_status[RemoteID * GASNETC_SCI_MAX_REQUEST_MSG * 2 + msg_number];
}

/*  Set a given message space to un-occupied (false) */
GASNETI_INLINE(gasnetc_mls_set)
void gasnetc_mls_set (gasnet_node_t RemoteID, uint8_t msg_number)
{
        if (RemoteID >= gasneti_nodes)
        {
            gasneti_fatalerror ("Error: node %d trying to send to node %d outside of system\n", gasneti_mynode, RemoteID);
        }
        if (msg_number >= GASNETC_SCI_MAX_REQUEST_MSG * 2)
        {
            gasneti_fatalerror ("Error: node %d trying to use mailbox %d, which is not allow\n", gasneti_mynode, msg_number);
        }
        if (gasnetc_sci_msg_loc_status[RemoteID * GASNETC_SCI_MAX_REQUEST_MSG * 2 + msg_number] == GASNETC_SCI_TRUE)
        {
            gasneti_fatalerror ("Error: node %d Attempt to set a msg location that is already set\n", gasneti_mynode);
        }
        else
        {
            gasnetc_sci_msg_loc_status[RemoteID * GASNETC_SCI_MAX_REQUEST_MSG * 2 + msg_number] = GASNETC_SCI_TRUE;
        }
}

/*  Obtain a free request message location */
int gasnetc_mls_get_loc (gasnet_node_t dest_node_id);

/********************************************************
				  MSG Flag Management
********************************************************/
/*  return the status of message existense flag */
GASNETI_INLINE(gasnetc_msg_exist_flag_status)
bool gasnetc_msg_exist_flag_status (void)
{
        return gasnetc_sci_msg_flag[gasneti_nodes * GASNETC_SCI_MAX_REQUEST_MSG * 2];
}

/*  set the message existense flag to FALSE */
GASNETI_INLINE(gasnetc_msg_exist_flag_release)
void gasnetc_msg_exist_flag_release (void)
{
        gasnetc_sci_msg_flag[gasneti_nodes * GASNETC_SCI_MAX_REQUEST_MSG * 2] = GASNETC_SCI_FALSE;
}

/*  return the status of msg_flag */
GASNETI_INLINE(gasnetc_msg_flag_status)
bool gasnetc_msg_flag_status (gasnet_node_t node_id, uint8_t msg_number)
{
        return gasnetc_sci_msg_flag[node_id * GASNETC_SCI_MAX_REQUEST_MSG * 2 + msg_number];
}

/*  set the corresponding msg_flag to FALSE */
GASNETI_INLINE(gasnetc_msg_flag_release)
void gasnetc_msg_flag_release (gasnet_node_t node_id, uint8_t msg_number)
{
        gasnetc_sci_msg_flag[node_id * GASNETC_SCI_MAX_REQUEST_MSG * 2 + msg_number] = GASNETC_SCI_FALSE;
}

/*  Scans the MRFs to enqueue new messages */
void gasnetc_MRF_scan (void);

/********************************************************
			Command Segment Related Functions
********************************************************/

/*  Return the Handler # for the given message */
GASNETI_INLINE(gasnetc_get_msg_handler)
gasnet_handler_t gasnetc_get_msg_handler (uint16_t header)
{
        return ((gasnet_handler_t) (header>>8));
}

/*  Return the type (Request/Reply) for the given message */
GASNETI_INLINE(gasnetc_get_msg_type)
uint8_t gasnetc_get_msg_type (uint16_t header)
{
        return ((uint8_t) ((header>>7) & 1));
}

/*  Return the AM type (Short/Medium/Long) for the given message */
GASNETI_INLINE(gasnetc_get_AM_type)
uint8_t gasnetc_get_AM_type (uint16_t header)
{
        return ((uint8_t) ((header>>5) & 3));
}

/*  Return the # of argument for the given message */
GASNETI_INLINE(gasnetc_get_msg_num_arg)
uint8_t gasnetc_get_msg_num_arg (uint16_t header)
{
        return ((uint8_t) (header & 31));
}

/********************************************************
                        Command Segment Related Functions
********************************************************/

/*  Generate Short / Medium Message Header */
GASNETI_INLINE(gasnetc_construct_ShortMedium_command)
void gasnetc_construct_ShortMedium_command (gasnetc_ShortMedium_header_t *temp, gasnet_handler_t handler,
                                                                                        uint8_t msg_type, uint8_t AM_type, size_t size, uint8_t num_args, gasnet_handlerarg_t args[],
                                                                                        uint16_t header_size)
{
        int i;
        temp->header = (((((handler<<1) | msg_type)<<2) | AM_type)<<5) | num_args;
        temp->header_size = header_size;
        temp->payload_size = size;
        for (i = 0; i < num_args; i++)
        {
                temp->args[i] = args[i];
        }
}

/*  Generate Long Message Header */
GASNETI_INLINE(gasnetc_construct_Long_command)
void gasnetc_construct_Long_command (gasnetc_Long_header_t *temp, gasnet_handler_t handler, uint8_t msg_type, void *payload,
                                                                         size_t size, uint8_t num_args, gasnet_handlerarg_t args[],
                                                                         uint16_t header_size)
{
        int i;
        temp->header = (((((handler<<1) | msg_type)<<2) | GASNETC_SCI_LONG)<<5) | num_args;
        temp->header_size = header_size;
        temp->payload_size = size;
        temp->payload = payload;
        for (i = 0; i < num_args; i++)
        {
                temp->args[i] = args[i];
        }
}

/********************************************************
                                    AM Transfer Functions
********************************************************/

GASNETI_INLINE(gasnetc_send_unaligned_offset_calculation)
void gasnetc_send_unaligned_offset_calculation (void *base_addr, void *dest_addr, size_t dest_size, uint8_t *start_unaligned_bytes, uint8_t *right_unaligned_bytes)
{
       uint8_t alignment_value = 64;
       uint8_t start_offset = (((uint8_t *)dest_addr - (uint8_t *)base_addr) & (alignment_value - 1));
       if (start_offset == 0)
       {
            *start_unaligned_bytes = 0;
       }
       else
       {
            *start_unaligned_bytes = alignment_value - start_offset;
       }
       *right_unaligned_bytes = (dest_size - *start_unaligned_bytes) & (alignment_value - 1);
}

/*  Calculate the actual header size */
GASNETI_INLINE(gasnetc_get_header_size)
int gasnetc_get_header_size (uint8_t AM_type, int num_arg)
{
        /* AM header size calculation include pedding of 2 bytes to uint16_t header*/
        switch (AM_type)
        {
              case GASNETC_SCI_SHORT:
              case GASNETC_SCI_MEDIUM:  return (8 + num_arg * (sizeof(gasnet_handlerarg_t)));
              case GASNETC_SCI_LONG:    return (8 + sizeof(void *) + num_arg * (sizeof(gasnet_handlerarg_t)));
              default:                  gasneti_fatalerror("(%d) bad type input to calculate header_size", gasneti_mynode);
        }
}

/*  0 copy 2 transfer SM transfer */
int gasnetc_SM_transfer (gasnet_node_t dest, uint8_t msg_number, uint8_t msg_type, uint8_t AM_type, gasnet_handler_t handler,
						int numargs, gasnet_handlerarg_t args[], void *payload, size_t segment_size,
						void *long_payload);

/*  SM Request */
GASNETI_INLINE(gasnetc_SM_request)
int gasnetc_SM_request (gasnet_node_t dest, uint8_t AM_type, gasnet_handler_t handler,
                                                int numargs, gasnet_handlerarg_t args[], void *payload_source_addr, size_t nbytes,
                                                void *payload_dest_addr)
{
        int retval;
        int msg_location;
        gasneti_mutex_lock(&gasnetc_sci_request_mutex);
        msg_location = gasnetc_mls_get_loc (dest);
        retval = gasnetc_SM_transfer (dest, msg_location, GASNETC_SCI_REQUEST, AM_type, handler, numargs, args, payload_source_addr, nbytes, payload_dest_addr);
        gasneti_mutex_unlock(&gasnetc_sci_request_mutex);
        return retval;
}

/********************************************************
		Long AM Initialization/Transfer Functions
********************************************************/

/*  Allocates space for local dma queues */
int gasnetc_create_dma_queues (void);

/*  Remove the previously allocated local dma queues */
int gasnetc_remove_dma_queues (void);

/*  Transfer the Long message using the DMA transfer method */
int gasnetc_DMA_write (gasnet_node_t dest, void *source_addr, size_t nbytes, void *dest_addr);

/********************************************************
		Environment Setup/Remove Functions
********************************************************/

/*  Initialize necessary system variables */
void gasnetc_setup_env (void);

/*  Remove system variables */
void gasnetc_free_env (void);

#endif
