/*The hitachi sr8000 header file*/
#include <hmpp/nalloc.h>
#include <hxb/combuf.h>
#include <hxb/combuf_node.h>
#include <hxb/combuf_returns.h>

#define PAGE_SIZE       0x1000
#define SMALL_MSG_SIZE PAGE_SIZE
#define SMALL_MSG_NUM armci_nproc
#define LARGE_MSG_SIZE 819200
#define STRIDED_GET_BUFLEN (200704-4096)*4 /*MSG_BUFLEN_SMALL-PAGE_SIZE*/
#define LARGE_MSG_NUM 1
#define SMALL_BUFS_PER_PROCESS 1
#define ROUND_UP_PAGE(size)  ((size + (PAGE_SIZE-1)) & ~(PAGE_SIZE-1))
#define NOOPTION        0
#define NOFLAG          0
#define NOATOMIC        0
#define LONG_GET_THRESHOLD 4096
#define LONG_GET_THRESHOLD_STRIDED 20000000
#define LONG_PUT_THRESHOLD 2047
#define MAX_CHUNKS_SHORT_GET 3

extern int armci_sr8krem_strided(int op, void* scale, int proc,
                       void *src_ptr, int src_stride_arr[],
                       void* dst_ptr, int dst_stride_arr[],
                       int count[], int stride_levels, int lockit);


typedef struct {
    void *data_ptr;         /* pointer where the data should go */
    long ack;               /* header ack */
} msg_tag_t;

extern int* clauth;


/*All the defined keys, note: keynum 1961 is already used in winshmem.c*/
#define BUF_KEY 2020L /*key for the client get and put buffers*/

#define SERVER_GENBUF_KEY 4040 /*server key for small and general bufs*/
#define SERVER_LARGE_GETBUF_KEY 16060 /*server keu for large getbuf*/

#define MUTEX_ARRAY_KEY 15050 /*common key for mutex array memory*/
#define CLIENT_SERV_COMMON_CB_KEY 3030 /*common key for cln-srv sharing info*/

#define CLIENT_SMALLBUF_KEY 8080/*client key for small buffers*/
#define CLIENT_DIRECTBUF_KEY 14040/*client key for large buffers*/
#define CLIENT_COUNTERS_KEY 18080

/*All the field numbers***/
#define CLIENT_SMALLBUF_FIELDNUM 1000
#define CLIENT_GETBUF_FIELDNUM 2
#define CLIENT_DIRECTBUF_FIELDNUM 2000
#define CLIENT_GET_DIRECTBUF_FIELDNUM 5000
#define CLIENT_PENDING_OP_FIELDNUM 6000
#define CLIENT_PENDING_SYN_FIELDNUM 7000
#define SERV_FIELD_NUM 3000
#define SERV_FIELD_NUM_FOR_LARGE_BUF 4000

extern char *_sr8k_armci_buf_get(int,int,int);
extern char *_sr8k_armci_buf_init(int);
extern void _sr8k_armci_buf_release(void *);
/***scope in tuning***/
#define LPUT 180000
#define LGET 450000
#define FLAGSIZE 8

/*stuff for get pipeline*/
#define PIPE_BUFSIZE  100*(8*2048 -128)
#define PIPE_MIN_BUFSIZE 2048
#define PIPE_MEDIUM_BUFSIZE (4*8192)

/**********definitions for using buffers.c************************/
extern int numofbuffers;
extern int _sr8k_buffer_index;
#define MSG_BUFLEN_SMALL 200704
#define BUFID_PAD_T double
#define BUF_ALLOCATE _sr8k_armci_buf_init
#define BUF_EXTRA_FIELD_T int
#define STORE_BUFID 1

#define GET_SEND_BUFFER _sr8k_armci_buf_get
#define  PASSBUFID(_id) _sr8k_buffer_index=_id;

#define FREE_SEND_BUFFER _sr8k_armci_buf_release

#define INIT_SEND_BUF(_fld,_snd,_rcv) _snd=0;_rcv=0
#define CLEAR_SEND_BUF_FIELD(_fld, _s, _r,_t) if((_fld)!=-1)(_fld)=-1;
/******************************************************************/

extern int *server_ready_flag;
extern int *client_ready_flag;


typedef struct {
	long bufptr;
	long cb_key;
	long bufdesc;
} rcv_field_info_t;

typedef struct {
	int small_buf_auth[SMALL_BUFS_PER_PROCESS];
	int small_buf_tcwd[SMALL_BUFS_PER_PROCESS];
	int large_buf_auth[LARGE_MSG_NUM*4];
	int large_buf_tcwd[LARGE_MSG_NUM*4];
	int get_auth;
	int get_tcwd;
	int server_large_getbuf_auth;
}client_auth_t;

typedef struct {
	int put_auths[SMALL_BUFS_PER_PROCESS];
	int put_tcwd[SMALL_BUFS_PER_PROCESS];
	int lbuf_put_auths[4];
	int get_auth;
	int get_tcwd;
	int clientgetbuf_auth;
	int clientgetbuf_tcwd;
	int clientputbuf_auth;
	int clientputbuf_gtcwd;
}server_auth_t;

extern rcv_field_info_t *serv_field_info;
extern rcv_field_info_t *client_rcv_field_info;
extern server_auth_t *server_auths;

