#ifndef _ARMCI_ELAN_H_
#define _ARMCI_ELAN_H_

#include <elan/elan.h>

#define SERV_BUF_IDX_T long
extern void armcill_getbufidx(int size, int proc, SERV_BUF_IDX_T *bidx);
extern void armcill_clearbuf(ELAN_EVENT** handle);
extern int armcill_testbuf(ELAN_EVENT** handle);
extern void armci_elan_put_with_tracknotify(char *src,char *dst,int n,int proc,ELAN_EVENT** phandle);

typedef int ops_t;
extern ops_t *rdma_ops_pending_ar;
typedef struct {
    void *data_ptr;         /* pointer where the data should go */
    long ack;               /* header ack */
    void *ack_ptr;          /* pointer where the data should go */
} msg_tag_t;

#define GET_SEND_BUFFER _armci_buf_get
#define FREE_SEND_BUFFER _armci_buf_release
#define COMPLETE_HANDLE _armci_buf_complete_nb_request
#define TEST_HANDLE _armci_buf_test_nb_request
#define SET_BUF_TAG _armci_buf_set_tag
#define INIT_SENDBUF_INFO(_hdl,_buf,_op,_proc) _armci_buf_set_tag(_buf,(_hdl)->tag,0)

#define BUF_ALLOCATE(_size) malloc(_size)
#define NB_CMPL_T ELAN_EVENT*
#define BUF_EXTRA_FIELD_T  ELAN_EVENT*
#define CLEAR_HNDL_FIELD(_x) _x=NULL

#define INIT_SEND_BUF(_field,_snd,_rcv) _field=NULL
#define CLEAR_SEND_BUF_FIELD(_field, _s, _r,_t,_o) armcill_clearbuf(&(_field)) 
#define CLEAR_SEND_BUF_FIELD_(_field, _s, _r,_t,_o) \
        elan_wait((_field),elan_base->waitType);  \
        if(!((request_header_t*)(&(_field)+1))->inbuf) while(((request_header_t*)(&(_field)+1))->tag) armci_util_spin(100,&(_field)+1); else
#define TEST_SEND_BUF_FIELD(_field, _s, _r,_t,_o,_ret) *_ret=armcill_testbuf(&_field)

#define LONG_PUT_THRESHOLD 128
#define PACKPUT 32

#define LONG_GET_THRESHOLD 128
#define PACKGET 32

#endif
