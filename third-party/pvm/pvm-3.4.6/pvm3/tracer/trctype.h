
/* $Id: trctype.h,v 4.3 1997/12/19 21:19:54 pvmsrc Exp $ */

/*
 *         Tracer version 1.0:  A Trace File Generator for PVM
 *           Oak Ridge National Laboratory, Oak Ridge TN.
 *           Authors:  James Arthur Kohl and G. A. Geist
 *                   (C) 1994 All Rights Reserved
 *
 *                              NOTICE
 *
 * Permission to use, copy, modify, and distribute this software and
 * its documentation for any purpose and without fee is hereby granted
 * provided that the above copyright notice appear in all copies and
 * that both the copyright notice and this permission notice appear
 * in supporting documentation.
 *
 * Neither the Institution, Oak Ridge National Laboratory, nor the
 * Authors make any representations about the suitability of this
 * software for any purpose.  This software is provided ``as is''
 * without express or implied warranty.
 *
 * Tracer was funded by the U.S. Department of Energy.
 */


/* Host Name Macros */

#define TRC_LOCALHOST( _name ) \
\
	( TRC_HOST_NAME != NULL && TRC_HOST_ALIAS != NULL \
		&& ( !strcmp( TRC_HOST_NAME, _name ) \
			|| !strcmp( TRC_HOST_ALIAS, _name ) \
		 	|| ( TRC_TMP_HOST = trc_host_alias_str( _name ), \
				TRC_TMP_CMP = !strcmp( TRC_HOST_NAME, TRC_TMP_HOST ), \
				free( TRC_TMP_HOST ), \
				TRC_TMP_CMP ) ) ) \
\

#define TRC_HOST_COMPARE( _host, _name ) \
\
	 ( !strcmp( (_host)->name, _name ) \
		 || !strcmp( (_host)->alias, _name ) \
		 || ( TRC_TMP_HOST = trc_host_alias_str( _name ), \
			TRC_TMP_CMP = !strcmp( (_host)->name, TRC_TMP_HOST ), \
			free( TRC_TMP_HOST ), \
			TRC_TMP_CMP ) ) \
\


/* Group Task Check Macro */

#define TRC_GROUPTASK( _name ) \
	( trc_compare( "pvmg", _name ) || trc_compare( "pvmcg", _name ) )


/* Trace Event Record Access Macros */

#define TRC_GET_TEVREC( _tr, _trptr, _did ) \
\
	( (_trptr) = trc_get_tevrec( _tr, _did ), \
		(_trptr) != NULL ) \
\

#define TRC_GET_TEVREC_VALUE( _tr, _did, _dtype ) \
\
	( (TRC_TMP_TR = trc_get_tevrec( _tr, _did )) != NULL ? \
		TRC_VALUE_OF( TRC_TMP_TR->value, _dtype ) : (_dtype) -1 ) \
\

#define TRC_ARR_VALUE_OF( _ptr, _dtype, _index ) \
	( ((_dtype *) (_ptr))[_index] )

#define TRC_VALUE_OF( _ptr, _dtype )    ( *( (_dtype *) (_ptr) ) )


/* Host Life Status Constants */

#define TRC_NOT_IN_PVM    0
#define TRC_IN_PVM        1


/* TEV Type Constants */

#define TRC_ENTRY_TEV    0
#define TRC_EXIT_TEV     1
#define TRC_IGNORE_TEV   2
#define TRC_MAX_TEV      3


/* Trace Message Status Constants */

#define TRC_MSG_STATUS_NONE         0
#define TRC_MSG_STATUS_ACTIVE       1
#define TRC_MSG_STATUS_MORE         2
#define TRC_MSG_STATUS_OVERWRITE    4
#define TRC_MSG_STATUS_CLEANUP      8


/* General Constants */

#define TRC_FALSE    0
#define TRC_TRUE     1


/* Tracer Data Structures & Types */


typedef void (*trc_vfp)();


struct trc_trie_struct
{
	int valid;
	char *str;
	void *value;
	/* TRC_TRIE */ struct trc_trie_struct *next;
};

typedef struct trc_trie_struct *TRC_TRIE;


struct trc_did_struct
{
	int id;
	char *name;
	char *desc;
	/* TRC_DID */ struct trc_did_struct *next;
};

typedef struct trc_did_struct *TRC_DID;


typedef void *TRC_VALUE;


struct trc_datadesc_struct
{
	TRC_DID did;
	int dt;
	int array;
	TRC_VALUE data;
	int num;
	/* TRC_DATADESC */ struct trc_datadesc_struct *next;
};

typedef struct trc_datadesc_struct *TRC_DATADESC;


struct trc_tevdesc_struct
{
	char *name;
	int eid;
	int entry_exit;
	int index;
	int hid;
	int dump;
	TRC_DATADESC ddesc;
	int refcount;
	/* TRC_TEVDESC */ struct trc_tevdesc_struct *next;
};

typedef struct trc_tevdesc_struct *TRC_TEVDESC;


struct trc_tevdesc_info_struct
{
	int id;
	int dt;
	int array;
};

typedef struct trc_tevdesc_info_struct TRC_TEVDESC_INFO_STRUCT;
typedef TRC_TEVDESC_INFO_STRUCT *TRC_TEVDESC_INFO;


struct trc_tevtask_struct
{
	int tid;
	int tevstatus;
	int outstatus;
	void **tevlist;
	int tevsize;
	char *output;
	/* TRC_TEVTASK */ struct trc_tevtask_struct *next;
};

typedef struct trc_tevtask_struct *TRC_TEVTASK;


struct trc_tevrec_struct
{
	TRC_DATADESC ddesc;
	TRC_VALUE value;
	int num;
	/* TRC_TEVREC */ struct trc_tevrec_struct *next;
};

typedef struct trc_tevrec_struct *TRC_TEVREC;


struct trc_host_struct
{
	char *name;
	char *alias;
	char *refname;
	char *arch;
	struct timeval delta;
	struct timeval last_time;
	int pvmd_tid;
	int speed;
	int in_pvm;

	/* Tracing Info */
	void **tevlist;
	int tevsize;
	void **didlist;
	int didsize;

	/* Extension Structure for Drawing, Etc. */
	void *ext;

	/* TRC_HOST */ struct trc_host_struct *next;
};

typedef struct trc_host_struct *TRC_HOST;


struct trc_id_struct
{
	int complete;
	int group_tasks;
	int event_ctx;
	int event_tag;
	int output_ctx;
	int output_tag;
	TRC_TEVTASK tevtask_list;
	char *trace_file;
	FILE *trace_out;
	FILE *trace_in;
	char *output_file;
	FILE *output_fp;
	trc_vfp status_msg;
	trc_vfp event_dump_hdr;
	trc_vfp output_dump_hdr;
	trc_vfp handle_host_add_notify;
	trc_vfp handle_host_del_notify;
	trc_vfp handle_descriptor;
	trc_vfp handle_old_descriptor;
	trc_vfp handle_event;
	trc_vfp handle_old_event;
	trc_vfp handle_command;
	trc_vfp handle_comment;
	/* TRC_ID */ struct trc_id_struct *next;
};

typedef struct trc_id_struct *TRC_ID;

