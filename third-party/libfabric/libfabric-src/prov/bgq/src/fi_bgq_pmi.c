/*
 * Copyright (C) 2016 by Argonne National Laboratory.
 *
 * This software is available to you under a choice of one of two
 * licenses.  You may choose to be licensed under the terms of the GNU
 * General Public License (GPL) Version 2, available from the file
 * COPYING in the main directory of this source tree, or the
 * BSD license below:
 *
 *     Redistribution and use in source and binary forms, with or
 *     without modification, are permitted provided that the following
 *     conditions are met:
 *
 *      - Redistributions of source code must retain the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer.
 *
 *      - Redistributions in binary form must reproduce the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer in the documentation and/or other materials
 *        provided with the distribution.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include <stdlib.h>

#define PMI_TRUE			(1)
#define PMI_FALSE			(0)

#define PMI_SUCCESS			0
#define PMI_FAIL			-1
#define PMI_ERR_INIT			1
#define PMI_ERR_NOMEM			2
#define PMI_ERR_INVALID_ARG		3
#define PMI_ERR_INVALID_KEY		4
#define PMI_ERR_INVALID_KEY_LENGTH	5
#define PMI_ERR_INVALID_VAL		6
#define PMI_ERR_INVALID_VAL_LENGTH	7
#define PMI_ERR_INVALID_LENGTH		8
#define PMI_ERR_INVALID_NUM_ARGS	9
#define PMI_ERR_INVALID_ARGS		10
#define PMI_ERR_INVALID_NUM_PARSED	11
#define PMI_ERR_INVALID_KEYVALP		12
#define PMI_ERR_INVALID_SIZE		13

#define BGQ_PMI_NAME_MAXLEN		128
#define BGQ_PMI_KEY_MAXLEN		128
#define BGQ_PMI_VALUE_MAXLEN		128
#define BGQ_SINGLE_ENTRY_NODE_BLOCK_MAX_LEN		16

#include "rdma/bgq/fi_bgq_spi.h"

static inline void node_block_write(char *value, char *current_node_block_value, int current_block_id, int current_node_block_count, int proc_count) {
	sprintf(current_node_block_value,",(%d,%d,%d)",current_block_id,current_node_block_count,proc_count);
	strcat(value,current_node_block_value);
}

static inline void *
convert_virtual_address_to_global_virtual_address (void * vaddr, size_t len)
{
	uint64_t paddr = 0;
	void * global_vaddr = NULL;
	uint32_t cnk_rc __attribute__ ((unused));

	Kernel_MemoryRegion_t cnk_mr;
	cnk_rc = Kernel_CreateMemoryRegion(&cnk_mr, (void *)vaddr, len);
	assert(cnk_rc == 0);

	paddr = (uint64_t)cnk_mr.BasePa + ((uint64_t)vaddr - (uint64_t)cnk_mr.BaseVa);

	cnk_rc = Kernel_Physical2GlobalVirtual((void *)paddr, &global_vaddr);
	assert(cnk_rc == 0);

	return global_vaddr;
}

static int pmi_rank = INT_MAX;
static int pmi_size = INT_MAX;
static int pmi_local_size = INT_MAX;
static int bgq_pmi_value_maxlen;
static uint32_t my_bgq_rank_node_id;

uint32_t *bgq_node_list;

int PMI_Init (int *spawned)
{
	if (!spawned) return PMI_ERR_INVALID_ARG;
	*spawned = PMI_FALSE;

	if (pmi_rank != INT_MAX) return PMI_FAIL;
	if (pmi_size != INT_MAX) return PMI_FAIL;

	Personality_t pers;
	int rc = 0;
	rc = Kernel_GetPersonality(&pers, sizeof(pers));
	if (rc) return PMI_FAIL;

	/* calculate the maximum number of ranks from the torus dimensions */
	Personality_Networks_t *net = &pers.Network_Config;
	uint32_t max_ranks = net->Anodes * net->Bnodes * net->Cnodes *
		net->Dnodes * net->Enodes * Kernel_ProcessCount();
	uint64_t numentries = 0;

	BG_CoordinateMapping_t mapping[max_ranks];
	rc = Kernel_RanksToCoords(sizeof(mapping), mapping, &numentries);
	bgq_node_list = (uint32_t *) malloc(sizeof(uint32_t) * max_ranks);
	uint32_t tcoord32bitmask = 0xFFFFFFC0;
	uint32_t origcoord;

	/* while populating the rank map also determine how many local ranks on
	   my node - pmi_local_size */
	BG_CoordinateMapping_t my_bgq_coords;
	my_bgq_coords.e = net->Ecoord;
	my_bgq_coords.reserved = mapping[0].reserved;
	my_bgq_coords.a = net->Acoord;
	my_bgq_coords.b = net->Bcoord;
	my_bgq_coords.c = net->Ccoord;
	my_bgq_coords.d = net->Dcoord;
	my_bgq_coords.t = 0;

	memcpy(&my_bgq_rank_node_id,&my_bgq_coords,sizeof(BG_CoordinateMapping_t));
	pmi_local_size = 0;

	int i;
	for (i=0;i<numentries;i++) {
		memcpy(&origcoord, &(mapping[i]),sizeof(BG_CoordinateMapping_t));
		bgq_node_list[i]  = origcoord & tcoord32bitmask;
		if (my_bgq_rank_node_id == bgq_node_list[i])
			pmi_local_size++;
	}
	pmi_size = (int) numentries;
	pmi_rank = (int) Kernel_GetRank();
	// the largest possible key value is the number of nodes * the space for a node block def
	bgq_pmi_value_maxlen = net->Anodes * net->Bnodes * net->Cnodes *
		net->Dnodes * net->Enodes * BGQ_SINGLE_ENTRY_NODE_BLOCK_MAX_LEN;
	if (bgq_pmi_value_maxlen < BGQ_PMI_VALUE_MAXLEN)
		bgq_pmi_value_maxlen = BGQ_PMI_VALUE_MAXLEN;
	return PMI_SUCCESS;
}


int PMI_Initialized (int *initialized)
{
	if (!initialized) return PMI_ERR_INVALID_ARG;

	return (pmi_size != INT_MAX && pmi_rank != INT_MAX) ? PMI_TRUE : PMI_FALSE;
}


int PMI_Finalize ()
{
	free(bgq_node_list);
	if (pmi_rank == INT_MAX) return PMI_ERR_INIT;
	if (pmi_size == INT_MAX) return PMI_ERR_INIT;

	pmi_rank = INT_MAX;
	pmi_size = INT_MAX;
	return PMI_SUCCESS;
}


int PMI_Get_size (int *size)
{
	if (pmi_size == INT_MAX) return PMI_ERR_INIT;

	if (!size) return PMI_ERR_INVALID_ARG;
	*size = pmi_size;
	return PMI_SUCCESS;
}


int PMI_Get_rank (int *rank)
{
	if (pmi_rank == INT_MAX) return PMI_ERR_INIT;

	if (!rank) return PMI_ERR_INVALID_ARG;
	*rank = pmi_rank;
	return PMI_SUCCESS;
}

int PMI_Get_universe_size (int *size)
{
	return PMI_Get_size(size);
}

int PMI_Get_appnum (int *appnum)
{
	if (pmi_rank == INT_MAX) return PMI_ERR_INIT;
	if (pmi_size == INT_MAX) return PMI_ERR_INIT;

	if (!appnum) return PMI_ERR_INVALID_ARG;
	*appnum = 0;
	return PMI_SUCCESS;
}

int PMI_Publish_name (const char service_name[], const char port[])
{
	return PMI_FAIL;
}

int PMI_Unpublish_name (const char service_name[])
{
	return PMI_FAIL;
}

int PMI_Lookup_name (const char service_name[], char port[])
{
	return PMI_FAIL;
}

int PMI_Barrier ()
{
//	abort();
//	return PMI_FAIL;
	return PMI_SUCCESS;
}

int PMI_Abort (int exit_code, const char error_msg[])
{
	abort();
	return PMI_SUCCESS;
}

int PMI_KVS_Get_my_name (char kvsname[], int length)
{
	/* obtain the name of the keyval space the local process group has access to */
	kvsname[0] = 0;
	return PMI_SUCCESS;
}

int PMI_KVS_Get_name_length_max (int *length)
{
	/* obtain the length necessary to store a kvsname */
	if (!length) return PMI_ERR_INVALID_ARG;
	*length = BGQ_PMI_NAME_MAXLEN;

	return PMI_SUCCESS;
}

int PMI_KVS_Get_key_length_max (int *length)
{
	/* obtain the length necessary to store a key */
	if (!length) return PMI_ERR_INVALID_ARG;
	*length = BGQ_PMI_KEY_MAXLEN;

	return PMI_SUCCESS;
}

int PMI_KVS_Get_value_length_max (int *length)
{
	/* obtain the length necessary to store a value */
	if (!length) return PMI_ERR_INVALID_ARG;
	*length = bgq_pmi_value_maxlen;

	return PMI_SUCCESS;
}


int PMI_KVS_Put (const char kvsname[], const char key[], const char value[])
{
	/* unimplemented put -- we are not really maintaining a keyval space */
	return PMI_SUCCESS;
}

int PMI_KVS_Commit (const char kvsname[])
{
	/* unimplemented commit -- we are not really maintaining a keyval space */
	return PMI_SUCCESS;
}

int PMI_KVS_Get (const char kvsname[], const char key[], char value[], int length)
{
	/* get a key/value pair from a keyval space */
	if (0 == strncmp(key, "PMI_local_size", 14)) {
		snprintf(value, length, "%d", pmi_local_size);
		return PMI_SUCCESS;
	}
	else if (0 == strncmp(key, "PMI_process_mapping", 19)) {

		/* Build the pmi node block list from the bgq node list computed
		   in the PMI_Init. */
		char current_node_block_value[BGQ_SINGLE_ENTRY_NODE_BLOCK_MAX_LEN*2];
		strcpy(value,"(vector");

		/* These are the variables holding values written out as a tuple
		   for a node block. */
		int current_block_id = 0;
		int current_proc_count = 0;
		int prev_proc_count = -1;
		int current_node_block_count = 1;

		/* This is the current node being analyzed. */
		uint32_t current_node_id = bgq_node_list[0];

		int i;
		for (i=0;i<pmi_size;i++) {
			if (bgq_node_list[i] == current_node_id) {
				// count the procs on this node
				current_proc_count++;
			}
			else {
				/* proc counting done for previous node, moving on to next node in the list. */
				if (current_node_id == bgq_node_list[0]) {
					/* Special case for first node. */
					current_node_id = bgq_node_list[i];
					prev_proc_count = current_proc_count;
					current_proc_count = 1;
					current_node_block_count = 1;
				}
				else {
					current_node_id = bgq_node_list[i];
					if (prev_proc_count == current_proc_count) {
						/* This node has same number of procs as last node just increment
						   the node block count. */
						current_node_block_count++;
						current_proc_count = 1;
					}
					else {
						/* new node with different proc count than previous - write out the previous
						   node block and advance to next node. */
						node_block_write(value, current_node_block_value, current_block_id, current_node_block_count, prev_proc_count);
						prev_proc_count = current_proc_count;
						current_proc_count = 1;
						current_block_id += current_node_block_count;
						current_node_block_count = 1;
						current_node_id = bgq_node_list[i];
					}
				}
			}
		}
		/* After the bgq node list loop write out last node blocks. */
		if (current_node_id == bgq_node_list[0]) {
			/* If only 1 node this is simple. */
			node_block_write(value, current_node_block_value, current_block_id, 1, current_proc_count);
		}
		else {
			if (prev_proc_count == current_proc_count) {
				/* Loop ended with last two nodes having equal number of procs. */
				current_node_block_count++;
				node_block_write(value, current_node_block_value, current_block_id, current_node_block_count, prev_proc_count);
			}
			else {
				/* Loop ended with last node having different number of procs than
				   second-to-last node. */
				node_block_write(value, current_node_block_value, current_block_id, current_node_block_count, prev_proc_count);
				current_block_id += current_node_block_count;
				node_block_write(value, current_node_block_value, current_block_id, 1, current_proc_count);
			}
		}
		// add closing bracket to value
		strcat(value,")");
		return PMI_SUCCESS;
	}
	else
		return PMI_FAIL;

}
