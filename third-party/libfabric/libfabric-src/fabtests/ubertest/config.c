/*
 * Copyright (c) 2017 Intel Corporation.  All rights reserved.
 *
 * This software is available to you under the BSD license below:
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

#include <stdio.h>
#include <string.h>

#include "fabtest.h"
#include "jsmn.h"


#define FT_CAP_MSG	FI_MSG | FI_SEND | FI_RECV
#define FT_CAP_TAGGED	FI_TAGGED | FI_SEND | FI_RECV
#define FT_CAP_RMA	FI_RMA | FI_READ | FI_WRITE | FI_REMOTE_READ | FI_REMOTE_WRITE
#define FT_CAP_ATOMIC	FI_ATOMICS | FI_READ | FI_WRITE | FI_REMOTE_READ | FI_REMOTE_WRITE


struct key_t {
	char *str;
	size_t offset;
	enum { VAL_STRING, VAL_NUM } val_type;
	int val_size;
};

static struct ft_series test_series;

size_t sm_size_array[] = {
		1 << 0,
		1 << 1,
		1 << 2, (1 << 2) + (1 << 1),
		1 << 3, (1 << 3) + (1 << 2),
		1 << 4, (1 << 4) + (1 << 3),
		1 << 5, (1 << 5) + (1 << 4),
		1 << 6, (1 << 6) + (1 << 5),
		1 << 7, (1 << 7) + (1 << 6),
		1 << 8
};
const unsigned int sm_size_cnt = (sizeof sm_size_array / sizeof sm_size_array[0]);

size_t med_size_array[] = {
		1 <<  4,
		1 <<  5,
		1 <<  6,
		1 <<  7, (1 <<  7) + (1 <<  6),
		1 <<  8, (1 <<  8) + (1 <<  7),
		1 <<  9, (1 <<  9) + (1 <<  8),
		1 << 10, (1 << 10) + (1 <<  9),
		1 << 11, (1 << 11) + (1 << 10),
		1 << 12, (1 << 12) + (1 << 11),
		1 << 13, (1 << 13) + (1 << 12),
		1 << 14
};
const unsigned int med_size_cnt = (sizeof med_size_array / sizeof med_size_array[0]);

size_t lg_size_array[] = {
		1 <<  4,
		1 <<  5,
		1 <<  6,
		1 <<  7, (1 <<  7) + (1 <<  6),
		1 <<  8, (1 <<  8) + (1 <<  7),
		1 <<  9, (1 <<  9) + (1 <<  8),
		1 << 10, (1 << 10) + (1 <<  9),
		1 << 11, (1 << 11) + (1 << 10),
		1 << 12, (1 << 12) + (1 << 11),
		1 << 13, (1 << 13) + (1 << 12),
		1 << 14, (1 << 14) + (1 << 13),
		1 << 15, (1 << 15) + (1 << 14),
		1 << 16, (1 << 16) + (1 << 15),
		1 << 17, (1 << 17) + (1 << 16),
		1 << 18, (1 << 18) + (1 << 17),
		1 << 19, (1 << 19) + (1 << 18),
		1 << 20, (1 << 20) + (1 << 19),
		1 << 21, (1 << 21) + (1 << 20),
		1 << 22, (1 << 22) + (1 << 21),
};
const unsigned int lg_size_cnt = (sizeof lg_size_array / sizeof lg_size_array[0]);

static struct key_t keys[] = {
	{
		.str = "node",
		.offset = offsetof(struct ft_set, node),
		.val_type = VAL_STRING,
	},
	{
		.str = "service",
		.offset = offsetof(struct ft_set, service),
		.val_type = VAL_STRING,
	},
	{
		.str = "prov_name",
		.offset = offsetof(struct ft_set, prov_name),
		.val_type = VAL_STRING,
	},
	{
		.str = "test_type",
		.offset = offsetof(struct ft_set, test_type),
		.val_type = VAL_NUM,
		.val_size = sizeof(((struct ft_set *)0)->test_type) / FT_MAX_TEST,
	},
	{
		.str = "class_function",
		.offset = offsetof(struct ft_set, class_function),
		.val_type = VAL_NUM,
		.val_size = sizeof(((struct ft_set *)0)->class_function) / FT_MAX_FUNCTIONS,
	},
	{
		.str = "ep_type",
		.offset = offsetof(struct ft_set, ep_type),
		.val_type = VAL_NUM,
		.val_size = sizeof(((struct ft_set *)0)->ep_type) / FT_MAX_EP_TYPES,
	},
	{
		.str = "av_type",
		.offset = offsetof(struct ft_set, av_type),
		.val_type = VAL_NUM,
		.val_size = sizeof(((struct ft_set *)0)->av_type) / FT_MAX_AV_TYPES,
	},
	{
		.str = "comp_type",
		.offset = offsetof(struct ft_set, comp_type),
		.val_type = VAL_NUM,
		.val_size = sizeof(((struct ft_set *)0)->comp_type) / FT_MAX_COMP,
	},
	{
		.str = "eq_wait_obj",
		.offset = offsetof(struct ft_set, eq_wait_obj),
		.val_type = VAL_NUM,
		.val_size = sizeof(((struct ft_set *)0)->eq_wait_obj) / FT_MAX_WAIT_OBJ,
	},
	{
		.str = "cq_wait_obj",
		.offset = offsetof(struct ft_set, cq_wait_obj),
		.val_type = VAL_NUM,
		.val_size = sizeof(((struct ft_set *)0)->cq_wait_obj) / FT_MAX_WAIT_OBJ,
	},
	{
		.str = "cntr_wait_obj",
		.offset = offsetof(struct ft_set, cntr_wait_obj),
		.val_type = VAL_NUM,
		.val_size = sizeof(((struct ft_set *)0)->cntr_wait_obj) / FT_MAX_WAIT_OBJ,
	},
	{
		.str = "op",
		.offset = offsetof(struct ft_set, op),
		.val_type = VAL_NUM,
		.val_size = sizeof(((struct ft_set *)0)->op) / FI_ATOMIC_OP_LAST,
	},
	{
		.str = "datatype",
		.offset = offsetof(struct ft_set, datatype),
		.val_type = VAL_NUM,
		.val_size = sizeof(((struct ft_set *)0)->datatype) / FI_DATATYPE_LAST,
	},
	{
		.str = "mode",
		.offset = offsetof(struct ft_set, mode),
		.val_type = VAL_NUM,
		.val_size = sizeof(((struct ft_set *)0)->mode) / FT_MAX_PROV_MODES,
	},
	{
		.str = "test_class",
		.offset = offsetof(struct ft_set, test_class),
		.val_type = VAL_NUM,
		.val_size = sizeof(((struct ft_set *)0)->test_class) / FT_MAX_CLASS,
	},
	{
		.str = "constant_caps",
		.offset = offsetof(struct ft_set, constant_caps),
		.val_type = VAL_NUM,
		.val_size = sizeof(((struct ft_set *)0)->constant_caps) / FT_MAX_CAPS,
	},
	{
		.str = "rx_cq_bind_flags",
		.offset = offsetof(struct ft_set, rx_cq_bind_flags),
		.val_type = VAL_NUM,
		.val_size = sizeof(((struct ft_set *)0)->rx_cq_bind_flags) / FT_MAX_FLAGS,
	},
	{
		.str = "tx_cq_bind_flags",
		.offset = offsetof(struct ft_set, tx_cq_bind_flags),
		.val_type = VAL_NUM,
		.val_size = sizeof(((struct ft_set *)0)->tx_cq_bind_flags) / FT_MAX_FLAGS,
	},
	{
		.str = "rx_op_flags",
		.offset = offsetof(struct ft_set, rx_op_flags),
		.val_type = VAL_NUM,
		.val_size = sizeof(((struct ft_set *)0)->rx_op_flags) / FT_MAX_FLAGS,
	},
	{
		.str = "tx_op_flags",
		.offset = offsetof(struct ft_set, tx_op_flags),
		.val_type = VAL_NUM,
		.val_size = sizeof(((struct ft_set *)0)->tx_op_flags) / FT_MAX_FLAGS,
	},
	{
		.str = "test_flags",
		.offset = offsetof(struct ft_set, test_flags),
		.val_type = VAL_NUM,
		.val_size = sizeof(((struct ft_set *)0)->test_flags),
	},
	{
		.str = "msg_flags",
		.offset = offsetof(struct ft_set, msg_flags),
		.val_type = VAL_NUM,
		.val_size = sizeof(((struct ft_set *)0)->msg_flags),
	},
	{
		.str = "mr_mode",
		.offset = offsetof(struct ft_set, mr_mode),
		.val_type = VAL_NUM,
		.val_size = sizeof(((struct ft_set *)0)->mr_mode) / FT_MAX_MR_MODES,
	},
	{
		.str = "progress",
		.offset = offsetof(struct ft_set, progress),
		.val_type = VAL_NUM,
		.val_size = sizeof(((struct ft_set *)0)->progress) / FT_MAX_PROGRESS,
	},
	{
		.str = "threading",
		.offset = offsetof(struct ft_set, threading),
		.val_type = VAL_NUM,
		.val_size = sizeof(((struct ft_set *)0)->threading) / FT_MAX_THREADING,
	},
};

static int ft_parse_num(char *str, int len, struct key_t *key, void *buf)
{
	if (!strncmp(key->str, "test_type", strlen("test_type"))) {
		TEST_ENUM_SET_N_RETURN(str, len, FT_TEST_LATENCY, enum ft_test_type, buf);
		TEST_ENUM_SET_N_RETURN(str, len, FT_TEST_BANDWIDTH, enum ft_test_type, buf);
		TEST_ENUM_SET_N_RETURN(str, len, FT_TEST_UNIT, enum ft_test_type, buf);
		FT_ERR("Unknown test_type");
	} else if (!strncmp(key->str, "class_function", strlen("class_function"))) {
		TEST_ENUM_SET_N_RETURN(str, len, FT_FUNC_SEND, enum ft_class_function, buf);
		TEST_ENUM_SET_N_RETURN(str, len, FT_FUNC_SENDV, enum ft_class_function, buf);
		TEST_ENUM_SET_N_RETURN(str, len, FT_FUNC_SENDMSG, enum ft_class_function, buf);
		TEST_ENUM_SET_N_RETURN(str, len, FT_FUNC_INJECT, enum ft_class_function, buf);
		TEST_ENUM_SET_N_RETURN(str, len, FT_FUNC_INJECTDATA, enum ft_class_function, buf);
		TEST_ENUM_SET_N_RETURN(str, len, FT_FUNC_SENDDATA, enum ft_class_function, buf);

		TEST_ENUM_SET_N_RETURN(str, len, FT_FUNC_WRITE, enum ft_class_function, buf);
		TEST_ENUM_SET_N_RETURN(str, len, FT_FUNC_WRITEV, enum ft_class_function, buf);
		TEST_ENUM_SET_N_RETURN(str, len, FT_FUNC_WRITEMSG, enum ft_class_function, buf);
		TEST_ENUM_SET_N_RETURN(str, len, FT_FUNC_WRITEDATA, enum ft_class_function, buf);
		TEST_ENUM_SET_N_RETURN(str, len, FT_FUNC_INJECT_WRITE, enum ft_class_function, buf);
		TEST_ENUM_SET_N_RETURN(str, len, FT_FUNC_INJECT_WRITEDATA, enum ft_class_function, buf);

		TEST_ENUM_SET_N_RETURN(str, len, FT_FUNC_READ, enum ft_class_function, buf);
		TEST_ENUM_SET_N_RETURN(str, len, FT_FUNC_READV, enum ft_class_function, buf);
		TEST_ENUM_SET_N_RETURN(str, len, FT_FUNC_READMSG, enum ft_class_function, buf);

		TEST_ENUM_SET_N_RETURN(str, len, FT_FUNC_ATOMIC, enum ft_class_function, buf);
		TEST_ENUM_SET_N_RETURN(str, len, FT_FUNC_ATOMICV, enum ft_class_function, buf);
		TEST_ENUM_SET_N_RETURN(str, len, FT_FUNC_ATOMICMSG, enum ft_class_function, buf);
		TEST_ENUM_SET_N_RETURN(str, len, FT_FUNC_INJECT_ATOMIC, enum ft_class_function, buf);
		TEST_ENUM_SET_N_RETURN(str, len, FT_FUNC_FETCH_ATOMIC, enum ft_class_function, buf);
		TEST_ENUM_SET_N_RETURN(str, len, FT_FUNC_FETCH_ATOMICV, enum ft_class_function, buf);
		TEST_ENUM_SET_N_RETURN(str, len, FT_FUNC_FETCH_ATOMICMSG, enum ft_class_function, buf);
		TEST_ENUM_SET_N_RETURN(str, len, FT_FUNC_COMPARE_ATOMIC, enum ft_class_function, buf);
		TEST_ENUM_SET_N_RETURN(str, len, FT_FUNC_COMPARE_ATOMICV, enum ft_class_function, buf);
		TEST_ENUM_SET_N_RETURN(str, len, FT_FUNC_COMPARE_ATOMICMSG, enum ft_class_function, buf);

		FT_ERR("Unknown class_function");
	} else if (!strncmp(key->str, "ep_type", strlen("ep_type"))) {
		TEST_ENUM_SET_N_RETURN(str, len, FI_EP_MSG, enum fi_ep_type, buf);
		TEST_ENUM_SET_N_RETURN(str, len, FI_EP_DGRAM, enum fi_ep_type, buf);
		TEST_ENUM_SET_N_RETURN(str, len, FI_EP_RDM, enum fi_ep_type, buf);
		FT_ERR("Unknown ep_type");
	} else if (!strncmp(key->str, "av_type", strlen("av_type"))) {
		TEST_ENUM_SET_N_RETURN(str, len, FI_AV_MAP, enum fi_av_type, buf);
		TEST_ENUM_SET_N_RETURN(str, len, FI_AV_TABLE, enum fi_av_type, buf);
		FT_ERR("Unknown av_type");
	} else if (!strncmp(key->str, "test_class", strlen("test_class"))) {
		TEST_SET_N_RETURN(str, len, "FT_CAP_MSG", FT_CAP_MSG, uint64_t, buf);
		TEST_SET_N_RETURN(str, len, "FT_CAP_TAGGED", FT_CAP_TAGGED, uint64_t, buf);
		TEST_SET_N_RETURN(str, len, "FT_CAP_RMA", FT_CAP_RMA, uint64_t, buf);
		TEST_SET_N_RETURN(str, len, "FT_CAP_ATOMIC", FT_CAP_ATOMIC, uint64_t, buf);
		FT_ERR("Unknown test class");
	} else if (!strncmp(key->str, "eq_wait_obj", strlen("eq_wait_obj")) ||
		!strncmp(key->str, "cq_wait_obj", strlen("cq_wait_obj")) ||
		!strncmp(key->str, "cntr_wait_obj", strlen("cntr_wait_obj"))) {
		TEST_ENUM_SET_N_RETURN(str, len, FI_WAIT_NONE, enum fi_wait_obj, buf);
		TEST_ENUM_SET_N_RETURN(str, len, FI_WAIT_UNSPEC, enum fi_wait_obj, buf);
		TEST_ENUM_SET_N_RETURN(str, len, FI_WAIT_FD, enum fi_wait_obj, buf);
		TEST_ENUM_SET_N_RETURN(str, len, FI_WAIT_MUTEX_COND, enum fi_wait_obj, buf);
		FT_ERR("Unknown wait_obj");
	} else if (!strncmp(key->str, "op", strlen("op"))) {
		TEST_ENUM_SET_N_RETURN(str, len, FI_MIN, enum fi_op, buf);
		TEST_ENUM_SET_N_RETURN(str, len, FI_MAX, enum fi_op, buf);
		TEST_ENUM_SET_N_RETURN(str, len, FI_SUM, enum fi_op, buf);
		TEST_ENUM_SET_N_RETURN(str, len, FI_PROD, enum fi_op, buf);
		TEST_ENUM_SET_N_RETURN(str, len, FI_LOR, enum fi_op, buf);
		TEST_ENUM_SET_N_RETURN(str, len, FI_LAND, enum fi_op, buf);
		TEST_ENUM_SET_N_RETURN(str, len, FI_BOR, enum fi_op, buf);
		TEST_ENUM_SET_N_RETURN(str, len, FI_BAND, enum fi_op, buf);
		TEST_ENUM_SET_N_RETURN(str, len, FI_LXOR, enum fi_op, buf);
		TEST_ENUM_SET_N_RETURN(str, len, FI_BXOR, enum fi_op, buf);
		TEST_ENUM_SET_N_RETURN(str, len, FI_ATOMIC_READ, enum fi_op, buf);
		TEST_ENUM_SET_N_RETURN(str, len, FI_ATOMIC_WRITE, enum fi_op, buf);
		TEST_ENUM_SET_N_RETURN(str, len, FI_CSWAP, enum fi_op, buf);
		TEST_ENUM_SET_N_RETURN(str, len, FI_CSWAP_NE, enum fi_op, buf);
		TEST_ENUM_SET_N_RETURN(str, len, FI_CSWAP_LE, enum fi_op, buf);
		TEST_ENUM_SET_N_RETURN(str, len, FI_CSWAP_LT, enum fi_op, buf);
		TEST_ENUM_SET_N_RETURN(str, len, FI_CSWAP_GE, enum fi_op, buf);
		TEST_ENUM_SET_N_RETURN(str, len, FI_CSWAP_GT, enum fi_op, buf);
		TEST_ENUM_SET_N_RETURN(str, len, FI_MSWAP, enum fi_op, buf);
		FT_ERR("Unknown op");
	} else if (!strncmp(key->str, "datatype", strlen("datatype"))) {
		TEST_ENUM_SET_N_RETURN(str, len, FI_INT8, enum fi_datatype, buf);
		TEST_ENUM_SET_N_RETURN(str, len, FI_UINT8, enum fi_datatype, buf);
		TEST_ENUM_SET_N_RETURN(str, len, FI_INT16, enum fi_datatype, buf);
		TEST_ENUM_SET_N_RETURN(str, len, FI_UINT16, enum fi_datatype, buf);
		TEST_ENUM_SET_N_RETURN(str, len, FI_INT32, enum fi_datatype, buf);
		TEST_ENUM_SET_N_RETURN(str, len, FI_UINT32, enum fi_datatype, buf);
		TEST_ENUM_SET_N_RETURN(str, len, FI_INT64, enum fi_datatype, buf);
		TEST_ENUM_SET_N_RETURN(str, len, FI_UINT64, enum fi_datatype, buf);
		TEST_ENUM_SET_N_RETURN(str, len, FI_FLOAT, enum fi_datatype, buf);
		TEST_ENUM_SET_N_RETURN(str, len, FI_DOUBLE, enum fi_datatype, buf);
		TEST_ENUM_SET_N_RETURN(str, len, FI_FLOAT_COMPLEX, enum fi_datatype, buf);
		TEST_ENUM_SET_N_RETURN(str, len, FI_DOUBLE_COMPLEX, enum fi_datatype, buf);
		TEST_ENUM_SET_N_RETURN(str, len, FI_LONG_DOUBLE, enum fi_datatype, buf);
		TEST_ENUM_SET_N_RETURN(str, len, FI_LONG_DOUBLE_COMPLEX, enum fi_datatype, buf);
		FT_ERR("Unknown datatype");
	} else if (!strncmp(key->str, "msg_flags", strlen("msg_flags"))) {
		TEST_ENUM_SET_N_RETURN(str, len, FI_REMOTE_CQ_DATA, uint64_t, buf);
		TEST_ENUM_SET_N_RETURN(str, len, FI_COMPLETION, uint64_t, buf);
		FT_ERR("Unknown message flag");
	} else if (!strncmp(key->str, "mr_mode", strlen("mr_mode"))) {
		TEST_ENUM_SET_N_RETURN(str, len, FI_MR_LOCAL, uint64_t, buf);
		TEST_ENUM_SET_N_RETURN(str, len, FI_MR_VIRT_ADDR, uint64_t, buf);
		TEST_ENUM_SET_N_RETURN(str, len, FI_MR_ALLOCATED, uint64_t, buf);
		TEST_ENUM_SET_N_RETURN(str, len, FI_MR_PROV_KEY, uint64_t, buf);
		FT_ERR("Unknown MR mode");
	} else if (!strncmp(key->str, "progress", strlen("progress"))) {
		TEST_ENUM_SET_N_RETURN(str, len, FI_PROGRESS_MANUAL, int, buf);
		TEST_ENUM_SET_N_RETURN(str, len, FI_PROGRESS_AUTO, int, buf);
		TEST_ENUM_SET_N_RETURN(str, len, FI_PROGRESS_UNSPEC, int, buf);
		FT_ERR("Unknown progress mode");
	} else if (!strncmp(key->str, "threading", strlen("threading"))) {
		TEST_ENUM_SET_N_RETURN(str, len, FI_THREAD_UNSPEC, int, buf);
		TEST_ENUM_SET_N_RETURN(str, len, FI_THREAD_SAFE, int, buf);
		TEST_ENUM_SET_N_RETURN(str, len, FI_THREAD_FID, int, buf);
		TEST_ENUM_SET_N_RETURN(str, len, FI_THREAD_DOMAIN, int, buf);
		TEST_ENUM_SET_N_RETURN(str, len, FI_THREAD_COMPLETION, int, buf);
		TEST_ENUM_SET_N_RETURN(str, len, FI_THREAD_ENDPOINT, int, buf);
		FT_ERR("Unknown threading level");
	} else if (!strncmp(key->str, "constant_caps", strlen("constant_caps"))) {
		TEST_ENUM_SET_N_RETURN(str, len, FI_RMA, uint64_t, buf);
		TEST_ENUM_SET_N_RETURN(str, len, FI_MSG, uint64_t, buf);
		TEST_ENUM_SET_N_RETURN(str, len, FI_SEND, uint64_t, buf);
		TEST_ENUM_SET_N_RETURN(str, len, FI_RECV, uint64_t, buf);
		TEST_ENUM_SET_N_RETURN(str, len, FI_READ, uint64_t, buf);
		TEST_ENUM_SET_N_RETURN(str, len, FI_WRITE, uint64_t, buf);
		TEST_ENUM_SET_N_RETURN(str, len, FI_REMOTE_READ, uint64_t, buf);
		TEST_ENUM_SET_N_RETURN(str, len, FI_REMOTE_WRITE, uint64_t, buf);
		TEST_ENUM_SET_N_RETURN(str, len, FI_TAGGED, uint64_t, buf);
		TEST_ENUM_SET_N_RETURN(str, len, FI_DIRECTED_RECV, uint64_t, buf);
		FT_ERR("Unknown caps");
	} else if (!strncmp(key->str, "rx_cq_bind_flags", strlen("rx_cq_bind_flags"))) {
		TEST_ENUM_SET_N_RETURN(str, len, FI_SELECTIVE_COMPLETION, uint64_t, buf);
		FT_ERR("Unknown rx_cq_bind_flags");
	} else if (!strncmp(key->str, "tx_cq_bind_flags", strlen("tx_cq_bind_flags"))) {
		TEST_ENUM_SET_N_RETURN(str, len, FI_SELECTIVE_COMPLETION, uint64_t, buf);
		FT_ERR("Unknown tx_cq_bind_flags");
	} else if (!strncmp(key->str, "rx_op_flags", strlen("rx_op_flags"))) {
		TEST_ENUM_SET_N_RETURN(str, len, FI_COMPLETION, uint64_t, buf);
		FT_ERR("Unknown rx_op_flags");
	} else if (!strncmp(key->str, "tx_op_flags", strlen("tx_op_flags"))) {
		TEST_ENUM_SET_N_RETURN(str, len, FI_COMPLETION, uint64_t, buf);
		FT_ERR("Unknown tx_op_flags");
	} else if (!strncmp(key->str, "comp_type", strlen("comp_type"))) {
		TEST_ENUM_SET_N_RETURN(str, len, FT_COMP_QUEUE, enum ft_comp_type, buf);
		TEST_ENUM_SET_N_RETURN(str, len, FT_COMP_CNTR, enum ft_comp_type, buf);
		TEST_ENUM_SET_N_RETURN(str, len, FT_COMP_ALL, enum ft_comp_type, buf);
		FT_ERR("Unknown comp_type");
	} else if (!strncmp(key->str, "mode", strlen("mode"))) {
		TEST_ENUM_SET_N_RETURN(str, len, FI_CONTEXT, uint64_t, buf);
		TEST_ENUM_SET_N_RETURN(str, len, FI_RX_CQ_DATA, uint64_t, buf);
		FT_ERR("Unsupported mode bit");
	} else if (!strncmp(key->str, "test_flags", strlen("test_flags"))) {
		TEST_SET_N_RETURN(str, len, "FT_FLAG_QUICKTEST", FT_FLAG_QUICKTEST, uint64_t, buf);
	} else {
		FT_ERR("Unknown test configuration key");
	}

	return -1;
}

static int ft_parse_key_val(char *config, jsmntok_t *token, char *test_set)
{
	int i, parsed = 0;
	jsmntok_t *key_token = token;
	jsmntok_t *val_token = token + 1;
	struct key_t *key = NULL;
	int size = 0;

	if (!strncmp(config + key_token->start, "#", 1)) {
		parsed += 2;
		return parsed;
	}

	for (i = 0; i < sizeof(keys) / sizeof(keys[0]); i++) {
		if (!strncmp(config + key_token->start, keys[i].str, strlen(keys[i].str))) {
			key = &keys[i];
			parsed++;
			break;
		}
	}

	if (!key) {
		FT_ERR("Unknown key");
		return -1;
	}

	switch(val_token->type) {
	case JSMN_PRIMITIVE:
	case JSMN_STRING:
		size = 1;
		break;
	case JSMN_ARRAY:
		size = val_token->size;
		val_token++;
		parsed++;
		break;
	default:
		FT_ERR("[jsmn] Unknown token type");
		return -1;
	}

	for (i = 0; i < size; i++) {
		switch(key->val_type) {
		case VAL_STRING:
			memcpy(test_set + key->offset + key->val_size * i,
					config + val_token[i].start,
					val_token[i].end - val_token[i].start);
			break;
		case VAL_NUM:
			if (ft_parse_num(config + val_token[i].start,
					val_token[i].end - val_token[i].start, key,
					test_set + key->offset + key->val_size * i) < 0)
				return -1;
			break;
		default:
			FT_ERR("Invalid key->val_type");
			return -1;
		}
		parsed++;
	}

	return parsed;
}

static int ft_parse_config(char *config, int size,
		struct ft_set **test_sets_out, int *nsets)
{
	struct ft_set *test_sets;
	jsmn_parser parser;
	jsmntok_t *tokens;
	int num_tokens, num_tokens_parsed;
	int i, ret, ts_count, ts_index;

	jsmn_init(&parser);
	num_tokens = jsmn_parse(&parser, config, size, NULL, 0);
	if (num_tokens <= 0)
		return 1;

	tokens = malloc(sizeof(jsmntok_t) * num_tokens);
	if (!tokens)
		return 1;

	/* jsmn parser returns a list of JSON tokens (jsmntok_t)
	 * e.g. JSMN_OBJECT
	 * 	JSMN_STRING : <key>
	 * 	JSMN_STRING : <value>
	 * 	JSMN_STRING : <key>
	 * 	JSMN_ARRAY  : <value: array with 2 elements>
	 * 	JSMN_STRING
	 * 	JSMN_STRING
	 * 	JSMN_STRING : <key>
	 * 	JSMN_STRING : <value>
	 * In our case, JSMN_OBJECT would represent a ft_set structure. The rest
	 * of the tokens would be treated as key-value pairs. The first JSMN_STRING
	 * would represent a key and the next would represent a value. A value
	 * can also be an array. jsmntok_t.size would represent the length of
	 * the array.
	 */
	jsmn_init(&parser);
	ret = jsmn_parse(&parser, config, size, tokens, num_tokens);
	if (ret < 0) {
		switch (ret) {
		case JSMN_ERROR_INVAL:
			FT_ERR("[jsmn] bad token, JSON string is corrupted!");
			break;
		case JSMN_ERROR_NOMEM:
			FT_ERR("[jsmn] not enough tokens, JSON string is too large!");
			break;
		case JSMN_ERROR_PART:
			FT_ERR("[jsmn] JSON string is too short, expecting more JSON data!");
			break;
		default:
			FT_ERR("[jsmn] Unknown error!");
			break;
		}
		goto err1;
	}

	if (ret != num_tokens) {
		FT_ERR("[jsmn] Expected # of tokens: %d, Got: %d", num_tokens, ret);
		goto err1;
	}

	for (i = 0, ts_count = 0; i < num_tokens; i++) {
		if (tokens[i].type == JSMN_OBJECT)
			ts_count++;
	}

	test_sets = calloc(ts_count, sizeof(struct ft_set));

	for (i = 0, ts_index = -1; i < num_tokens;) {
		switch (tokens[i].type) {
		case JSMN_OBJECT:
			ts_index++;
			i++;
			break;
		case JSMN_PRIMITIVE:
		case JSMN_STRING:
			num_tokens_parsed = ft_parse_key_val(config, &tokens[i],
					(char *)(test_sets + ts_index));
		        if (num_tokens_parsed <= 0)	{
				FT_ERR("Error parsing config!");
				goto err2;
			}
			i += num_tokens_parsed;
			break;
		default:
			FT_ERR("[jsmn] Unknown token!");
			goto err2;
		}
	}

	*test_sets_out = test_sets;
	*nsets = ts_count;

	free(tokens);
	return 0;
err2:
	free(test_sets);
err1:
	free(tokens);
	return 1;
}

struct ft_series *fts_load(char *filename)
{
	int nsets = 0;
	char *config;
	FILE *fp;

	if (filename) {
		int size;
		struct ft_set *test_sets = NULL;

		fp = fopen(filename, "rb");
		if (!fp) {
			FT_ERR("Unable to open config file: %s\n", filename);
			return NULL;
		}

		fseek(fp, 0, SEEK_END);
		size = ftell(fp);
		if (size < 0) {
			FT_ERR("ftell error");
			goto err1;
		}
		fseek(fp, 0, SEEK_SET);

		config = malloc(size + 1);
		if (!config) {
			FT_ERR("Unable to allocate memory");
			goto err1;
		}

		if (fread(config, size, 1, fp) != 1) {
			FT_ERR("Error reading config file");
			goto err2;
		}

		config[size] = 0;

		if (ft_parse_config(config, size, &test_sets, &nsets)) {
			FT_ERR("Unable to parse file");
			goto err2;
		}

		test_series.sets = test_sets;
		test_series.nsets = nsets;
		free(config);
		fclose(fp);
	} else {
		printf("Test config file required.\n");
		exit(1);
	}

	for (fts_start(&test_series, 0); !fts_end(&test_series, 0);
	     fts_next(&test_series))
		test_series.test_count++;
	fts_start(&test_series, 0);

	printf("Test configurations loaded: %d\n", test_series.test_count);
	return &test_series;

err2:
	free(config);
err1:
	fclose(fp);
	return NULL;
}

void fts_close(struct ft_series *series)
{
	free(series->sets);
}

void fts_start(struct ft_series *series, int index)
{
	series->cur_set = 0;
	series->cur_type = 0;
	series->cur_ep = 0;
	series->cur_av = 0;
	series->cur_comp = 0;
	series->cur_eq_wait_obj = 0;
	series->cur_cq_wait_obj = 0;
	series->cur_cntr_wait_obj = 0;
	series->cur_mode = 0;
	series->cur_class = 0;
	series->cur_progress = 0;
	series->cur_threading = 0;

	series->test_index = 1;
	if (index > 1) {
		for (; !fts_end(series, index - 1); fts_next(series))
			;
	}
}

int fts_info_is_valid(void)
{
	if (test_info.msg_flags && !is_msg_func(test_info.class_function))
		return 0;

	if (test_info.rx_cq_bind_flags & FI_SELECTIVE_COMPLETION) {
		if (!(test_info.rx_op_flags & FI_COMPLETION) &&
		    !(test_info.msg_flags & FI_COMPLETION))
			return 0;

		/* Skip RX selective completion if not using a counter
		 * - Hard to test (because of needed sync messages)
		 * - Not intended use case for FI_SELECTIVE_COMPLETION
		 */
		if (!ft_use_comp_cntr(test_info.comp_type))
			return 0;
	}

	if (test_info.test_class & (FI_MSG | FI_TAGGED) &&
	    !ft_check_rx_completion(test_info) &&
	    !ft_use_comp_cntr(test_info.comp_type))
		return 0;
	if (test_info.test_type == FT_TEST_UNIT &&
	    test_info.ep_type == FI_EP_DGRAM)
		return 0;

	return 1;
}

void fts_next(struct ft_series *series)
{
	struct ft_set *set;

	if (fts_end(series, 0))
		return;

	series->test_index++;
	set = &series->sets[series->cur_set];

	if (set->test_class[++series->cur_class])
		return;
	series->cur_class = 0;

	if (set->mode[++series->cur_mode])
		return;
	series->cur_mode = 0;

	if (set->class_function[++series->cur_func])
		return;
	series->cur_func = 0;

	if (set->op[++series->cur_op])
		return;
	series->cur_op = 0;

	if (set->datatype[++series->cur_datatype])
		return;
	series->cur_datatype = 0;

	if (set->comp_type[++series->cur_comp])
		return;
	series->cur_comp = 0;

	if (set->eq_wait_obj[++series->cur_eq_wait_obj])
		return;
	series->cur_eq_wait_obj = 0;

	if (set->cq_wait_obj[++series->cur_cq_wait_obj])
		return;
	series->cur_cq_wait_obj = 0;

	if (set->cntr_wait_obj[++series->cur_cntr_wait_obj])
		return;
	series->cur_cntr_wait_obj = 0;

	if (set->ep_type[series->cur_ep] == FI_EP_RDM ||
	    set->ep_type[series->cur_ep] == FI_EP_DGRAM) {
		if (set->av_type[++series->cur_av])
			return;
	}
	series->cur_av = 0;

	if (set->ep_type[++series->cur_ep])
		return;
	series->cur_ep = 0;

	if (set->test_type[++series->cur_type])
		return;
	series->cur_type = 0;

	if (set->progress[++series->cur_progress])
		return;
	series->cur_progress = 0;

	if (set->threading[++series->cur_threading])
		return;
	series->cur_threading = 0;

	series->cur_set++;
}

int fts_end(struct ft_series *series, int index)
{
	return (series->cur_set >= series->nsets) ||
		((index > 0) && (series->test_index > index));
}

void fts_cur_info(struct ft_series *series, struct ft_info *info)
{
	static struct ft_set *set;
	int i = 0;

	memset(info, 0, sizeof *info);
	if (series->cur_set >= series->nsets)
		return;

	set = &series->sets[series->cur_set];
	info->test_type = set->test_type[series->cur_type];
	info->test_index = series->test_index;
	info->class_function = set->class_function[series->cur_func];
	info->msg_flags = set->msg_flags;
	info->op = set->op[series->cur_op];
	info->datatype = set->datatype[series->cur_datatype];
	info->test_flags = set->test_flags;
	info->test_class = set->test_class[series->cur_class];
	info->progress = set->progress[series->cur_progress];
	info->threading = set->threading[series->cur_threading];

	if (info->test_class) {
		info->caps = set->test_class[series->cur_class];
		if (info->caps & (FT_CAP_RMA | FT_CAP_ATOMIC))
			info->caps |= FT_CAP_MSG;
	}

	if (set->constant_caps[0]) {
		i = 0;
		while (set->constant_caps[i])
			info->caps |= set->constant_caps[i++];
	}
	if (set->mr_mode[0]) {
		i = 0;
		while (set->mr_mode[i])
			info->mr_mode |= set->mr_mode[i++];
	}
	if (set->rx_cq_bind_flags[0]) {
		i = 0;
		while (set->rx_cq_bind_flags[i])
			info->rx_cq_bind_flags |= set->rx_cq_bind_flags[i++];
	}
	if (set->tx_cq_bind_flags[0]) {
		i = 0;
		while (set->tx_cq_bind_flags[i])
			info->tx_cq_bind_flags |= set->tx_cq_bind_flags[i++];
	}
	if (set->rx_op_flags[0]) {
		i = 0;
		while (set->rx_op_flags[i])
			info->rx_op_flags |= set->rx_op_flags[i++];
	}
	if (set->tx_op_flags[0]) {
		i = 0;
		while (set->tx_op_flags[i])
			info->tx_op_flags |= set->tx_op_flags[i++];
	}
	if (set->mode[0]) {
		i = 0;
		while (set->mode[i])
			info->mode |= set->mode[i++];
	}

	info->ep_type = set->ep_type[series->cur_ep];
	info->av_type = set->av_type[series->cur_av];
	if (set->comp_type[0])
		info->comp_type = set->comp_type[series->cur_comp];
	else
		info->comp_type = FT_COMP_QUEUE;

	if (info->caps & (FT_CAP_RMA | FT_CAP_ATOMIC) &&
		(ft_use_comp_cntr(info->comp_type)))
		info->caps |= FI_RMA_EVENT;
	info->eq_wait_obj = set->eq_wait_obj[series->cur_eq_wait_obj];
	info->cntr_wait_obj = set->cntr_wait_obj[series->cur_cntr_wait_obj];

	if (set->node[0])
		strncpy(info->node, set->node, sizeof(info->node));
	else if (opts.dst_addr)
		strncpy(info->node, opts.dst_addr, sizeof(info->node));
	info->node[sizeof(info->node) - 1] = '\0';

	if (set->service[0])
		strncpy(info->service, set->service, sizeof(info->service));
	else if (opts.dst_port)
		strncpy(info->service, opts.dst_port, sizeof(info->service));
	info->service[sizeof(info->service) - 1] = '\0';

	strncpy(info->prov_name, set->prov_name, sizeof(info->prov_name));
	info->prov_name[sizeof(info->prov_name) - 1] = '\0';
}
