/* ARMCI header file */
#ifndef _ARMCI_H
#define _ARMCI_H   

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif


extern int armci_sameclusnode(int proc);

typedef struct {
    void **src_ptr_array;
    void **dst_ptr_array;
    int  ptr_array_len;
    int bytes;
} armci_giov_t;
typedef long armci_size_t;
extern int armci_notify(int proc);
extern int armci_notify_wait(int proc,int *pval);
extern int ARMCI_Init(void);    /* initialize ARMCI */
extern void ARMCI_Barrier(void);    /* ARMCI Barrier*/

extern int ARMCI_Put(void *src, void* dst, int bytes, int proc);
extern int ARMCI_Put_flag(void *src, void* dst,int bytes,int *f,int v,int proc);

#define ARMCI_Put1(_s,_d,_b,_p) memcpy(_d,_s,_b), 0

extern int ARMCI_PutS(          /* strided put */
                void *src_ptr,        /* pointer to 1st segment at source*/ 
		int src_stride_arr[], /* array of strides at source */
		void* dst_ptr,        /* pointer to 1st segment at destination*/
		int dst_stride_arr[], /* array of strides at destination */
		int count[],          /* number of units at each stride level count[0]=bytes */
		int stride_levels,    /* number of stride levels */
                int proc	      /* remote process(or) ID */
                );

extern int ARMCI_PutS_flag_dir(       /* put with flag that uses direct put */
                void *src_ptr,        /* pointer to 1st segment at source*/
                int src_stride_arr[], /* array of strides at source */
                void* dst_ptr,        /* pointer to 1st segment at destination*/
                int dst_stride_arr[], /* array of strides at destination */
                int count[],          /* number of segments at each stride 
                                         levels: count[0]=bytes*/
                int stride_levels,    /* number of stride levels */
                int *flag,            /* pointer to remote flag */
                int val,              /* value to set flag upon completion of
                                         data transfer */
                int proc              /* remote process(or) ID */
                );

extern int ARMCI_PutS_flag(
                void *src_ptr,        /* pointer to 1st segment at source*/
                int src_stride_arr[], /* array of strides at source */
                void* dst_ptr,        /* pointer to 1st segment at destination*/
                int dst_stride_arr[], /* array of strides at destination */
                int count[],          /* number of segments at each stride 
                                         levels: count[0]=bytes*/
                int stride_levels,    /* number of stride levels */
                int *flag,            /* pointer to remote flag */
                int val,              /* value to set flag upon completion of
                                         data transfer */
                int proc              /* remote process(or) ID */
                );

extern int ARMCI_AccS(                /* strided accumulate */
                int  optype,          /* operation */
                void *scale,          /* scale factor x += scale*y */
                void *src_ptr,        /* pointer to 1st segment at source*/ 
		int src_stride_arr[], /* array of strides at source */
		void* dst_ptr,        /* pointer to 1st segment at destination*/
		int dst_stride_arr[], /* array of strides at destination */
		int count[],          /* number of units at each stride level count[0]=bytes */
		int stride_levels,    /* number of stride levels */
                int proc	      /* remote process(or) ID */
                );


#ifdef __crayx1__
#define ARMCI_Get(_s,_d,_b,_p) memcpy(_d,_s,_b), 0
#else
extern int ARMCI_Get(void *src, void* dst, int bytes, int proc);
#endif

extern int ARMCI_GetS(          /* strided get */
                void *src_ptr,        /* pointer to 1st segment at source*/ 
		int src_stride_arr[], /* array of strides at source */
		void* dst_ptr,        /* pointer to 1st segment at destination*/
		int dst_stride_arr[], /* array of strides at destination */
		int count[],          /* number of units at each stride level count[0]=bytes */
		int stride_levels,    /* number of stride levels */
                int proc	      /* remote process(or) ID */
                );

extern int ARMCI_GetV( armci_giov_t darr[], /* descriptor array */
                int len,  /* length of descriptor array */
                int proc  /* remote process(or) ID */
              );

extern int ARMCI_PutV( armci_giov_t darr[], /* descriptor array */
                int len,  /* length of descriptor array */
                int proc  /* remote process(or) ID */
              );

extern int ARMCI_AccV( int op,       /* operation code */
                void *scale,         /* scaling factor for accumulate */
                armci_giov_t darr[], /* descriptor array */
                int len,             /* length of descriptor array */
                int proc             /* remote process(or) ID */
              );

extern int ARMCI_PutValueInt(int src,     /* value in a register to put     */
			     void *dst,   /* dest starting addr to put data */
			     int proc     /* remote process (or) ID         */
			     );

extern int ARMCI_PutValueLong(long src,   /* value in a register to put     */
			      void *dst,  /* dest starting addr to put data */
			      int proc    /* remote process (or) ID         */
			      );

extern int ARMCI_PutValueFloat(float src, /* value in a register to put     */
			       void *dst, /* dest starting addr to put data */
			       int proc   /* remote process (or) ID         */
			       );

extern int ARMCI_PutValueDouble(double src,/* value in a register to put     */
				void *dst, /* dest starting addr to put data */
				int proc   /* remote process (or) ID         */
				);

extern int ARMCI_GetValueInt(void *src, int proc);
extern long ARMCI_GetValueLong(void *src, int proc);
extern float ARMCI_GetValueFloat(void *src, int proc);     
extern double ARMCI_GetValueDouble(void *src, int proc);     


extern int ARMCI_Malloc(void* ptr_arr[], armci_size_t bytes);
extern int ARMCI_Free(void *ptr);
extern void* ARMCI_Malloc_local(armci_size_t bytes);
extern int ARMCI_Free_local(void *ptr);
extern int ARMCI_Same_node(int proc);

extern void ARMCI_Finalize();    /* terminate ARMCI */
extern void ARMCI_Error(char *msg, int code);
extern void ARMCI_Fence(int proc);
extern void ARMCI_AllFence(void);
extern int  ARMCI_Rmw(int op, int *ploc, int *prem, int extra, int proc);
extern void ARMCI_Cleanup(void);
extern int ARMCI_Create_mutexes(int num);
extern int ARMCI_Destroy_mutexes(void);
extern void ARMCI_Lock(int mutex, int proc);
extern void ARMCI_Unlock(int mutex, int proc);
extern void ARMCI_Set_shm_limit(unsigned long shmemlimit);
extern int ARMCI_Uses_shm();
extern void ARMCI_Copy(void *src, void *dst, int n);

#define FAIL  -1
#define FAIL2 -2
#define FAIL3 -3
#define FAIL4 -4
#define FAIL5 -5
#define FAIL6 -6
#define FAIL7 -7
#define FAIL8 -8

#define ARMCI_SWAP 10
#define ARMCI_SWAP_LONG 11
#define ARMCI_FETCH_AND_ADD 12
#define ARMCI_FETCH_AND_ADD_LONG 13

#define ARMCI_ACC_OFF 36
#define ARMCI_ACC_INT (ARMCI_ACC_OFF + 1)
#define ARMCI_ACC_DBL (ARMCI_ACC_OFF + 2)
#define ARMCI_ACC_FLT (ARMCI_ACC_OFF + 3)
#define ARMCI_ACC_CPL (ARMCI_ACC_OFF + 4)
#define ARMCI_ACC_DCP (ARMCI_ACC_OFF + 5)
#define ARMCI_ACC_LNG (ARMCI_ACC_OFF + 6)

#define ARMCI_MAX_STRIDE_LEVEL 8

#ifdef BGML     
#define ARMCI_CRITICAL_SECTION_ENTER() BGML_CriticalSection_enter();
#define ARMCI_CRITICAL_SECTION_EXIT()  BGML_CriticalSection_exit();
#else
#define ARMCI_CRITICAL_SECTION_ENTER()
#define ARMCI_CRITICAL_SECTION_EXIT()     
#endif    

/************ locality information **********************************************/
typedef int armci_domain_t;
#define ARMCI_DOMAIN_SMP 0        /* SMP node domain for armci_domain_XXX calls */
extern int armci_domain_nprocs(armci_domain_t domain, int id);
extern int armci_domain_id(armci_domain_t domain, int glob_proc_id);
extern int armci_domain_glob_proc_id(armci_domain_t domain, int id, int loc_proc_id);
extern int armci_domain_my_id(armci_domain_t domain);
extern int armci_domain_count(armci_domain_t domain);
extern int armci_domain_same_id(armci_domain_t domain, int proc);


/* PVM group
 * On CrayT3E: the default group is the global group which is (char *)NULL
 *             It is the only working group.
 * On Workstations: the default group is "mp_working_group". User can set
 *                  the group name by calling the ARMCI_PVM_init (defined
 *                  in message.c) and passing the group name to the library.
 */

extern char *mp_group_name;

/*********************stuff for non-blocking API******************************/
/*\ the request structure for non-blocking api. 
\*/
typedef struct{
#ifdef BGML 
    int data[4]; /* tag, bufid, agg_flag, op, proc */
    double dummy[72]; /* bg1s_t, count, extra */
#else
    int data[4];
#if defined(_AIX) 
#   if defined(__64BIT__)
    double dummy[27]; /*lapi_cntr_t is 200 bytes, using 216 just to be safe*/ 
#   else
    double dummy[24]; /*lapi_cntr_t is 148 bytes, using 166 just to be safe*/ 
#   endif
#elif defined(ALLOW_PIN)
    void *dummy[2];/*2 cause itshould be aligned after we cast hdl_t to ihdl_t*/
#else
    double dummy;
#endif
#endif
} armci_hdl_t;

#define armci_req_t armci_hdl_t

#ifdef MPI
typedef struct {
    double dummy[8];
} ARMCI_Group;
 
void ARMCI_Group_create(int n, int *pid_list, ARMCI_Group *group);
int ARMCI_Group_rank(ARMCI_Group *group, int *rank);
void ARMCI_Group_size(ARMCI_Group *group, int *size);
int ARMCI_Malloc_group(void *ptr_arr[], armci_size_t bytes,ARMCI_Group *group);
int ARMCI_Free_group(void *ptr, ARMCI_Group *group);
#endif

extern int ARMCI_NbPut(void *src, void* dst, int bytes, int proc,armci_hdl_t* nb_handle);

extern int ARMCI_NbPutS(          /* strided put */
                void *src_ptr,        /* pointer to 1st segment at source*/ 
		int src_stride_arr[], /* array of strides at source */
		void* dst_ptr,        /* pointer to 1st segment at destination*/
		int dst_stride_arr[], /* array of strides at destination */
		int count[],          /* number of units at each stride level count[0]=bytes */
		int stride_levels,    /* number of stride levels */
                int proc,	      /* remote process(or) ID */
                armci_hdl_t* nb_handle /*armci_non-blocking request handle*/
                );

extern int ARMCI_NbAccS(                /* strided accumulate */
                int  optype,          /* operation */
                void *scale,          /* scale factor x += scale*y */
                void *src_ptr,        /* pointer to 1st segment at source*/ 
		int src_stride_arr[], /* array of strides at source */
		void* dst_ptr,        /* pointer to 1st segment at destination*/
		int dst_stride_arr[], /* array of strides at destination */
		int count[],          /* number of units at each stride level count[0]=bytes */
		int stride_levels,    /* number of stride levels */
                int proc,	      /* remote process(or) ID */
                armci_hdl_t* nb_handle /*armci_non-blocking request handle*/
                );

extern int ARMCI_NbGet(void *src, void* dst, int bytes, int proc,armci_hdl_t* nb_handle);

extern int ARMCI_NbGetS(          /* strided get */
                void *src_ptr,        /* pointer to 1st segment at source*/ 
		int src_stride_arr[], /* array of strides at source */
		void* dst_ptr,        /* pointer to 1st segment at destination*/
		int dst_stride_arr[], /* array of strides at destination */
		int count[],          /* number of units at each stride level count[0]=bytes */
		int stride_levels,    /* number of stride levels */
                int proc,	      /* remote process(or) ID */
                armci_hdl_t* nb_handler/*armci_non-blocking request handle*/
                );

extern int ARMCI_NbGetV( armci_giov_t darr[], /* descriptor array */
                int len,  /* length of descriptor array */
                int proc,  /* remote process(or) ID */
                armci_hdl_t* nb_handle /*armci_non-blocking request handle*/
              );

extern int ARMCI_NbPutV( armci_giov_t darr[], /* descriptor array */
                int len,  /* length of descriptor array */
                int proc,  /* remote process(or) ID */
                armci_hdl_t* nb_handle /*armci_non-blocking request handle*/
              );

extern int ARMCI_NbAccV( int op,       /* operation code */
                void *scale,         /* scaling factor for accumulate */
                armci_giov_t darr[], /* descriptor array */
                int len,             /* length of descriptor array */
                int proc,             /* remote process(or) ID */
                armci_hdl_t* nb_handle /*armci_non-blocking request handle*/
              );

extern int ARMCI_NbPutValueInt(int src,   /* value in a register to put     */
			       void *dst, /* dest starting addr to put data */
			       int proc,  /* remote process (or) ID         */
			       armci_hdl_t* nb_handle /*armci_non-blocking 
						       request handle       */
			       );

extern int ARMCI_NbPutValueLong(long src,  /* value in a register to put     */
				void *dst, /* dest starting addr to put data */
				int proc,  /* remote process (or) ID         */
				armci_hdl_t* nb_handle /*armci_non-blocking 
							request handle       */
				);

extern int ARMCI_NbPutValueFloat(float src,/* value in a register to put     */
				 void *dst,/* dest starting addr to put data */
				 int proc, /* remote process (or) ID         */
				 armci_hdl_t* nb_handle /*armci_non-blocking 
							 request handle      */
				 );

extern int ARMCI_NbPutValueDouble(double src,/* value in a register to put   */
				  void *dst,/* dest starting addr to put data*/
				  int proc,   /* remote process (or) ID      */
				  armci_hdl_t* nb_handle /*armci_non-blocking 
							  request handle     */
				  );

extern int ARMCI_Wait(armci_hdl_t* nb_handle); /*non-blocking request handle*/

extern int ARMCI_Test(armci_hdl_t* nb_handle); /*non-blocking request handle*/

extern int ARMCI_WaitAll (void);

extern int ARMCI_WaitProc (int proc);

extern void ARMCI_SET_AGGREGATE_HANDLE(armci_hdl_t* nb_handle);

extern void ARMCI_UNSET_AGGREGATE_HANDLE(armci_hdl_t* nb_handle);

#define ARMCI_INIT_HANDLE(hdl) do {((double *)((hdl)->data))[0]=0; \
  ((double *)((hdl)->data))[1]=0; }while(0)

/* ------------------- ARMCI Checkpointing/Recovery ----------------- */
#ifdef DO_CKPT
typedef struct {
        void **ptr_arr;
        size_t *sz;
        int *saveonce;
        int count;
}armci_ckpt_ds_t;
void ARMCI_Ckpt_create_ds(armci_ckpt_ds_t *ckptds, int count);
int ARMCI_Ckpt_init(char *filename, ARMCI_Group *grp, int savestack, int saveheap, armci_ckpt_ds_t *ckptds);
int ARMCI_Ckpt(int rid);
void ARMCI_Ckpt_finalize(int rid);
#endif
/* ------------------------------------------------------------------ */

#ifdef MPI
ARMCI_Group * ARMCI_Get_world_group();
#ifdef DO_CKPT
ARMCI_Group * ARMCI_Get_ft_group();
#endif
#endif

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif /* _ARMCI_H */


