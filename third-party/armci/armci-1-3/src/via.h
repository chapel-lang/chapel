#ifndef _VIA_H_
#define _VIA_H_ 
extern int armci_long_buf_free, armci_long_buf_taken_srv;
extern void armci_via_wait_ack();
#define VIA_USES_RDMA 1
#define PIPE_BUFSIZE  (4096)
#define MAX_PIPELINE_CHUNKS 20
#define PIPE_MIN_BUFSIZE 1024 
#define PIPE_MEDIUM_BUFSIZE (2*1024)

/* we need buffer size to be 64-byte alligned */
#define EXTRA_MSG_BUFLEN_DBL  128 
#define EXTRA_MSG_BUFLEN  ((EXTRA_MSG_BUFLEN_DBL)<<3) 
#define VBUF_DLEN 64*1023
#define MSG_BUFLEN_DBL ((VBUF_DLEN)>>3)

#ifdef PIPE_BUFSIZE
#  define STRIDED_GET_BUFLEN_DBL 31*1024
#  define STRIDED_GET_BUFLEN (STRIDED_GET_BUFLEN_DBL<<3)
#  define MAX_BUFLEN (STRIDED_GET_BUFLEN+EXTRA_MSG_BUFLEN)
#else
#  define MAX_BUFLEN (MSG_BUFLEN+EXTRA_MSG_BUFLEN)
#endif

#define GET_SEND_BUFFER____(_size) \
        MessageSndBuffer;if(!armci_long_buf_free)armci_via_wait_ack()

#define GET_SEND_BUFFER _armci_buf_get
#define FREE_SEND_BUFFER _armci_buf_release
#define COMPLETE_HANDLE _armci_buf_complete_nb_request
#define TEST_HANDLE _armci_buf_test_nb_request
#define BALANCE_BUFFERS
#ifdef BALANCE_BUFFERS
#  define BALANCE_FACTOR 1.6
#  define BALANCE_BUFSIZE 25000
#endif

/*
typedef struct {
    char padd[128];
} buf_pad_t;
#define BUFID_PAD_T buf_pad_t
*/

typedef struct {
    char s[64];
    char r[64];
} armci_via_field_t;

#define BUF_EXTRA_FIELD_T armci_via_field_t 
#define CLEAR_SEND_BUF_FIELD(_field,_snd,_rcv,_to,_op) armci_via_complete_buf((armci_via_field_t *)(&(_field)),(_snd),(_rcv),(_to),(_op));_snd=0;_rcv=0;_to=0

#define TEST_SEND_BUF_FIELD(_field,_snd,_rcv,_to,_op,_ret) armci_via_test_buf((armci_via_field_t *)(&(_field)),(_snd),(_rcv),(_to),(_op),(_ret))

/*we have 3 protocols for get, 2 of them would now not post any recv descriptors
  hence would not require _rcv, third one, pinning protocol, would still post a 
  descriptor but waits for it outside of the regular complete_buf code. so, _rcv
  can be set to 0 safely
#define INIT_SEND_BUF(_field,_snd,_rcv) _snd=1;_rcv=1;if(operation==GET&&size>2*PIPE_MIN_BUFSIZE)_rcv=0
*/
#define INIT_SEND_BUF(_field,_snd,_rcv) _snd=1;_rcv=1;if(operation==GET)_rcv=0

extern char * armci_via_client_mem_alloc(int);
#define BUF_ALLOCATE(_size) armci_via_client_mem_alloc(_size)

#define CLIENT_BUF_BYPASS 
#define NEEDS_PINNING 
#define LONG_GET_THRESHOLD 10000
#define LONG_GET_THRESHOLD_STRIDED 20000000
#define _armci_bypass 1

extern void armci_via_complete_buf(armci_via_field_t *,int,int,int,int); 
extern int armci_pin_memory(void *ptr, int stride_arr[], int count[], int lev);
extern void armci_unpin_memory(void *ptr,int stride_arr[],int count[],int lev);
extern void armci_client_send_ack(int p, int success);
extern int armcill_server_wait_ack(int proc, int n);
extern void armcill_server_put(int proc, void* s, void *d, int len);

#endif
