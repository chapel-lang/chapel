
/* $Id: trcproto.h,v 4.4 2009/01/30 21:23:58 pvmsrc Exp $ */

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


/* Tracer Library Routine Prototypes */


TRC_TRIE		trc_create_triestack __ProtoGlarp__(( void ));


void			*trc_lookup_trie __ProtoGlarp__(( TRC_TRIE, char * ));


TRC_DID			trc_create_did __ProtoGlarp__(( void ));
TRC_DID			trc_get_did __ProtoGlarp__(( TRC_HOST, int ));


TRC_ID			trc_create_tracer_id __ProtoGlarp__(( void ));
TRC_ID			trc_get_tracer_id __ProtoGlarp__(( void ));


TRC_DATADESC	trc_create_datadesc __ProtoGlarp__(( void ));


TRC_TEVDESC_INFO	trc_create_tevdesc_info __ProtoGlarp__(( void ));


TRC_TEVDESC		trc_create_tevdesc __ProtoGlarp__(( void ));

TRC_TEVDESC		trc_get_tevdesc __ProtoGlarp__(( TRC_TEVTASK,
					TRC_HOST, int, int ));

TRC_TEVDESC		trc_make_tevdesc __ProtoGlarp__(( TRC_ID,
					int, int, char *, TRC_TEVDESC_INFO, int ));

TRC_TEVDESC		trc_read_descriptor __ProtoGlarp__(( TRC_ID,
					int, char *, int, int ));


TRC_TEVREC		trc_create_tevrec __ProtoGlarp__(( void ));
TRC_TEVREC		trc_get_tevrec __ProtoGlarp__(( TRC_TEVREC, char * ));


TRC_VALUE		trc_make_value __ProtoGlarp__(( int, int ));
TRC_VALUE		trc_copy_value __ProtoGlarp__(( TRC_VALUE, int, int ));


TRC_TEVTASK		trc_create_tevtask __ProtoGlarp__(( void ));
TRC_TEVTASK		trc_get_tevtask_tid __ProtoGlarp__(( TRC_ID, int ));


TRC_HOST		trc_create_host __ProtoGlarp__(( void ));
TRC_HOST		trc_get_host_name __ProtoGlarp__(( char * ));
TRC_HOST		trc_get_host_tid __ProtoGlarp__(( int ));
TRC_HOST		trc_get_host_tid_last __ProtoGlarp__(( int,
					TRC_HOST * ));


void			trc_check_for_dead_host __ProtoGlarp__(( TRC_ID,
					TRC_TEVTASK ));

void			trc_check_listsize __ProtoGlarp__(( void ***, int *,
					int ));

void			trc_cleanup_dead_hosts __ProtoGlarp__(( void ));

void			trc_define_special_descriptors __ProtoGlarp__(( void ));

void			trc_dump_old_pvm_event_fmt __ProtoGlarp__(( TRC_ID,
					int ));

void			trc_dump_old_sddf_headers __ProtoGlarp__(( void ));

void			trc_dump_tevdesc __ProtoGlarp__(( TRC_ID,
					TRC_TEVDESC, int ));

void			trc_dump_trace_header __ProtoGlarp__(( TRC_ID ));

void			trc_dump_trace_str __ProtoGlarp__(( TRC_ID,
					char *, int ));

void			trc_end_trace __ProtoGlarp__(( TRC_ID ));

void			trc_free_datadesc __ProtoGlarp__(( TRC_DATADESC * ));

void			trc_free_did __ProtoGlarp__(( TRC_DID * ));

void			trc_free_event_descriptors __ProtoGlarp__(( TRC_TRIE ));

void			trc_free_host __ProtoGlarp__(( TRC_HOST * ));

void			trc_free_tevdesc_info __ProtoGlarp__((
					TRC_TEVDESC_INFO * ));

void			trc_free_tevrec __ProtoGlarp__(( TRC_TEVREC * ));

void			trc_free_tevtask __ProtoGlarp__(( TRC_TEVTASK * ));

void			trc_free_tevtask_list __ProtoGlarp__(( TRC_TEVTASK * ));

void			trc_free_tracer_id __ProtoGlarp__(( TRC_ID * ));

void			trc_free_triestack __ProtoGlarp__(( TRC_TRIE * ));

void			trc_init_did_trie __ProtoGlarp__(( void ));

void			trc_init_old_events __ProtoGlarp__(( void ));

void			trc_initialize_hosts __ProtoGlarp__(( TRC_ID ));

void			trc_memcheck __ProtoGlarp__(( void *, char * ));

void			trc_print_tmask __ProtoGlarp__(( Pvmtmask ));

void			trc_remove_tevdesc __ProtoGlarp__(( TRC_TEVDESC *,
					int ));

void			trc_reset_old_descriptors __ProtoGlarp__(( void ));

void			trc_set_didlist __ProtoGlarp__(( TRC_HOST, TRC_DID ));

void			trc_set_tevlist __ProtoGlarp__(( TRC_TEVTASK,
					TRC_HOST, TRC_TEVDESC ));

void			trc_set_tracing_codes __ProtoGlarp__(( TRC_ID ));

void			trc_status_msg __ProtoGlarp__(( TRC_ID, char * ));

void			trc_store_data_values __ProtoGlarp__(( TRC_ID,
					TRC_TEVDESC, int, int ));

void			trc_store_event_header __ProtoGlarp__(( TRC_ID,
					TRC_TEVDESC, int ));

void			trc_task_output __ProtoGlarp__(( TRC_ID,
					int, char * ));

void			trc_tracer_init __ProtoGlarp__(( void ));

void			trc_update_trace_time __ProtoGlarp__(( int, int ));

void			trc_write_host_add_event __ProtoGlarp__(( TRC_ID,
					TRC_HOST ));

void			trc_write_host_del_event __ProtoGlarp__(( TRC_ID,
					TRC_HOST, int ));

void			trc_write_host_sync_event __ProtoGlarp__(( TRC_ID,
					TRC_HOST ));

void			trc_write_old_host_add_event __ProtoGlarp__(( TRC_ID,
					TRC_HOST ));

void			trc_write_old_host_del_event __ProtoGlarp__(( TRC_ID,
					TRC_HOST, int ));

void			trc_write_old_host_sync_event __ProtoGlarp__(( TRC_ID,
					TRC_HOST ));

void			trc_write_old_output_event __ProtoGlarp__(( TRC_ID,
					int, char * ));

void			trc_write_output_event __ProtoGlarp__(( TRC_ID,
					int, char * ));


char			*trc_old_pvm_event_string __ProtoGlarp__(( TRC_ID,
					int ));

char			*trc_host_alias_str __ProtoGlarp__(( char * ));

char			*trc_tevrec_string __ProtoGlarp__(( TRC_TEVDESC,
					TRC_TEVREC, int, int, int ));

char			*trc_copy_str __ProtoGlarp__(( char * ));

char			*trc_date_str __ProtoGlarp__(( void ));

char			*trc_pad_num __ProtoGlarp__(( long, long ));


int				trc_add_tevdesc __ProtoGlarp__(( TRC_ID,
					TRC_TEVDESC *, int, int ));

int				trc_add_to_trie __ProtoGlarp__(( TRC_TRIE,
					char *, void * ));

int				trc_append_str __ProtoGlarp__(( char *, char *, int ));

int				trc_check_host_sync __ProtoGlarp__(( TRC_ID ));

int				trc_cmp_datadesc __ProtoGlarp__(( TRC_DATADESC,
					TRC_DATADESC ));

int				trc_cmp_did __ProtoGlarp__(( TRC_DID, TRC_DID ));

int				trc_cmp_tevdesc __ProtoGlarp__(( TRC_TEVDESC,
					TRC_TEVDESC ));

int				trc_compare __ProtoGlarp__(( char *, char * ));

int				trc_do_host_sync __ProtoGlarp__(( TRC_ID,
					TRC_HOST, int ));

int				trc_fdcheck __ProtoGlarp__(( int, char * ));

int				trc_filecheck __ProtoGlarp__(( FILE *, char * ));

int				trc_find_event_end_ret __ProtoGlarp__(( TRC_ID,
					char *, int ));

int				trc_find_event_str_ret __ProtoGlarp__(( TRC_ID,
					char *, char *, int ));

int				trc_find_event_end __ProtoGlarp__(( TRC_ID ));

int				trc_find_event_str __ProtoGlarp__(( TRC_ID, char * ));

int				trc_free_tevdesc __ProtoGlarp__(( TRC_TEVDESC * ));

int				trc_get_dt_str __ProtoGlarp__(( char * ));

int				trc_get_msg __ProtoGlarp__(( TRC_ID,
					int *, int *, int *, int * ));

int				trc_handle_host_add_notify __ProtoGlarp__(( TRC_ID,
					int *, int ));

int				trc_handle_host_del_notify __ProtoGlarp__(( TRC_ID,
					int ));

int				trc_handle_output_message __ProtoGlarp__(( TRC_ID ));

int				trc_handle_trace_message __ProtoGlarp__(( TRC_ID,
					int ));

int				trc_open_output_file __ProtoGlarp__(( TRC_ID ));

int				trc_process_old_trace_event __ProtoGlarp__(( TRC_ID,
					char * ));

int				trc_process_trace_event __ProtoGlarp__(( TRC_ID,
					char *, int, int ));

int				trc_read_trace_event __ProtoGlarp__(( TRC_ID, int * ));

int				trc_read_old_trace_str __ProtoGlarp__(( TRC_ID,
					char *, int, int ));

int				trc_recv_messages __ProtoGlarp__(( TRC_ID,
					int, int * ));

int				trc_reset_trace_file __ProtoGlarp__(( TRC_ID ));

int				trc_save_host_status_events __ProtoGlarp__(( TRC_ID ));

int				trc_set_output_file __ProtoGlarp__(( TRC_ID, char * ));

int				trc_set_trace_file __ProtoGlarp__(( TRC_ID, char * ));

int				trc_store_old_trace_event __ProtoGlarp__(( TRC_ID,
					int, int, int, int ));

int				trc_store_trace_event __ProtoGlarp__(( TRC_ID,
					TRC_TEVDESC, int, int ));

int				trc_tevtasks_alive __ProtoGlarp__(( TRC_ID ));

int				trc_tevtasks_dead __ProtoGlarp__(( TRC_ID ));

int				trc_update_host __ProtoGlarp__(( TRC_HOST *,
					struct pvmhostinfo *, int, int ));

