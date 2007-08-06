/* begin_generated_IBM_copyright_prolog                             */
/*                                                                  */
/* This is an automatically generated copyright prolog.             */
/* After initializing,  DO NOT MODIFY OR MOVE                       */
/* ---------------------------------------------------------------- */
/* Product(s):                                                      */
/* 5733-BG1                                                         */
/*                                                                  */
/* (C)Copyright IBM Corp. 2004, 2006                                */
/* All rights reserved.                                             */
/* US Government Users Restricted Rights -                          */
/* Use, duplication or disclosure restricted                        */
/* by GSA ADP Schedule Contract with IBM Corp.                      */
/*                                                                  */
/* Licensed Materials-Property of IBM                               */
/* --------------------------------------------------------------   */
/* end_generated_IBM_copyright_prolog                               */
/********************************************************************/

/*!
 * \file bgml.h
 * \brief Common external message layer interface.
 */
#ifndef __bgml_h__
#define __bgml_h__

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

  /**
   * \brief Message layer operation types
   */

  typedef enum
  {
    BGML_UNDEFINED_OP = -1,
    BGML_NOOP,
    BGML_MAX,
    BGML_MIN,
    BGML_SUM,
    BGML_PROD,
    BGML_LAND,
    BGML_LOR,
    BGML_LXOR,
    BGML_BAND,
    BGML_BOR,
    BGML_BXOR,
    BGML_MAXLOC,
    BGML_MINLOC,
    BGML_OP_COUNT
  }
  BGML_Op;

  /**
   * \brief Message layer data types
   */

  typedef enum
  {
    /* Standard/Primative DT's */
    BGML_UNDEFINED_DT = -1,
    BGML_SIGNED_CHAR,
    BGML_UNSIGNED_CHAR,
    BGML_SIGNED_SHORT,
    BGML_UNSIGNED_SHORT,
    BGML_SIGNED_INT,
    BGML_UNSIGNED_INT,
    BGML_SIGNED_LONG,
    BGML_UNSIGNED_LONG,
    BGML_FLOAT,
    BGML_DOUBLE,
    BGML_LONG_DOUBLE,
    BGML_LOGICAL,
    BGML_SINGLE_COMPLEX,
    BGML_DOUBLE_COMPLEX,
    /* Max/Minloc DT's */
    BGML_2INT,
    BGML_SHORT_INT,
    BGML_FLOAT_INT,
    BGML_DOUBLE_INT,
    BGML_2REAL,
    BGML_2DOUBLE_PRECISION,
    BGML_DT_COUNT
  }
  BGML_Dt;

  /* ********************************************************************* */
  /*                                                                       */
  /*               Generic: messager object                                */
  /*                                                                       */
  /* ********************************************************************* */

  /**
   * \brief Common BlueGene quad datatype (4 words, 16 bytes)
   */

  typedef struct
  {
    unsigned w0; /**< Word[0] */
    unsigned w1; /**< Word[1] */
    unsigned w2; /**< Word[2] */
    unsigned w3; /**< Word[3] */
  }
  BGQuad __attribute__ ((__aligned__ (16)));

  /**
   * \brief completion callback information descriptor. Mostly used to
   * signal completion of a particular operation to the world outside
   * the messaging library.
   */

  typedef struct BGML_Callback_t
  {
    void (*function) (void *);          /**< actual function to call */
    void *clientdata;                   /**< argument to function */
  }
  BGML_Callback_t;

  /**
   * \brief Initialize messager singleton. Not thread safe. This
   * function can be called multiple times.
   * \return 1 when called the first time; 0 for every subsequent call.
   */

  unsigned BGML_Messager_Init ();

  /**
   * \brief Finalize messager singleton. Not thread safe. This
   * function can be called multiple times.
   * \return 1 when called the first time; 0 for every subsequent call.
   */

  unsigned BGML_Messager_Finalize ();

  /*!
   * \brief Enter a critical section
   */
  
  inline void BGML_CriticalSection_enter ();

  /*!
   * \brief Exit a critical section
   */
  
  inline void BGML_CriticalSection_exit ();
  
  /** \brief Messager advance loop. Thread safe. */

  unsigned BGML_Messager_advance ();

  /** \brief return linear rank of current node. Reentrant. */

  unsigned BGML_Messager_rank ();

  /** \brief return number of processors running current job. Reentrant. */

  unsigned BGML_Messager_size ();

  /*! \brief return the geometry of the block. */
  void BGML_Messager_geometry (unsigned *x, unsigned *y, unsigned *z);

  /**
   * \brief map a torus coordinate to a linear rank. Thread safe.
   * \param x: torus x coordinate
   * \param y: torus y coordinate
   * \param z: torus z coordinate
   * \param t: torus t coordinate
   * \returns: linear rank or -1 if node (x,y,z,t) is not part of current job
   */

  int BGML_Messager_torus2rank (int x, int y, int z, int t);

  /**
   * \brief find torus coordinates for a linear rank. Thread safe.
   * \param rank: linear rank of a node
   * \param x: returns torus x coordinate of node
   * \param y: returns torus y coordinate of node
   * \param z: returns torus z coordinate of node
   * \param t: returns torus t coordinate of node
   * \returns 0 if successful; -1 if rank parameter outside of acceptable
   * range.
   */

  int BGML_Messager_rank2torus (int rank, int *x, int *y, int *z, int *t);

#define     BGML_MESSAGER_VNMODE 0      /**< virtual node mode */
#define     BGML_MESSAGER_SMMODE 1      /**< shared memory mode */
#define     BGML_MESSAGER_CPMODE 2      /**< non-coherent coprocessor mode */

  /**
   * \brief This should provide information about whether we
   * are in shared memory/coprocessor mode or virtual node mode
   * \todo incomplete!!!
   */

  int BGML_Messager_mode ();

#define     BGML_MESSAGER_2S       1   /**< two-sided (MPI) communication */
#define     BGML_MESSAGER_1S       2   /**< one-sided communication */
#define     BGML_MESSAGER_TR       4   /**< torus collectives */
#define     BGML_MESSAGER_TSC      8   /**< torus collectives */
#define     BGML_MESSAGER_TSRC    16   /**< rectangular torus collectives */
#define     BGML_MESSAGER_GI      32   /**< global interrupt barrier */

  /**
   * \brief Provides list of available subsets in messaging library.
   * \return a bit mask of available subsystems.
   */

  unsigned long long BGML_Messager_available ();

  /* ********************************************************************* */
  /*                                                                       */
  /*  2-sided point-to-point communication: callbacks & initialization     */
  /*                                                                       */
  /* ********************************************************************* */

  /**
   * \brief Opaque data type that holds all 2-sided message requests
   * (send & receive).  We allocate 0.5 KBytes of space to hold the
   * send/receive object's state.  The external API does not reveal the
   * internal structure of the object. Implementations should verify
   * that send and receive objects need less space than sizeof(BG_2S_t).
   */

  typedef BGQuad BG2S_t[32];

  /**
   * \brief Pointer to the 2-sided message opaque data type.
   */

  typedef BG2S_t *BG2S_p;

  /**
   * \brief callback invoked when a long message arrives to destination.
   */

  typedef
    BG2S_t *(*cb_BG2S_Recv) (const BGQuad * msginfo,
                             unsigned senderrank,
                             const unsigned sndlen,
                             unsigned *rcvlen,
                             char **rcvbuf, BGML_Callback_t * const cb_info);

  /**
   * \brief callback invoked when a short message arrives to destination.
   */

  typedef
    void (*cb_BG2S_RecvShort) (const BGQuad * msginfo,
                               unsigned senderrank,
                               const char *sndbuf, const unsigned sndlen);

  /**
   * \brief callback invoked when a control message arrives to destination.
   */

  typedef
    void (*cb_BG2S_Control) (unsigned senderrank,
                             const char *sndbuf, const unsigned sndlen);


  /**
   *  \brief Initialize 2-sided communication. Cannot send 2-sided messages
   * before this function has been invoked. Not thread safe.
   * \param cb_recvshort: callback invoked for short messages
   * \param cb_recvlong: callback invoked for long messages
   * \param cb_control: callback invoked for control messages
   * !!! open question: should we allocate explicit space for the protocols?
   */

  void BG2S_Configure (cb_BG2S_RecvShort cb_recvshort,
                       cb_BG2S_Recv cb_recvlong, cb_BG2S_Control cb_control);

  /* ********************************************************************* */
  /*                                                                       */
  /*  2-sided point-to-point communication: message sending primitives     */
  /*                                                                       */
  /* ********************************************************************* */

  /**
   * \brief Send a message. Thread safe if invoked on different
   * messages. Until the send invokes the callback, it is illegal to
   * send it again, reset it or touch the attached send buffer.
   *
   * \param request: place to keep per-message information, such as
   * location in send queue, how many bytes have been sent etc.
   * \param cb_info: function to call when send is *locally* complete
   * \param msginfo: a "quad" of (to messager, opaque) MPI header information.
   * \param sndbuf: send data buffer.
   * \param sndlen: send data length (in bytes).
   * \param destrank: destination rank.
   */

  void BG2S_Send (BG2S_t * request,
                  const BGML_Callback_t * cb_info,
                  const BGQuad * msginfo,
                  const char *sndbuf, unsigned sndlen, unsigned destrank);

  /**
   * \brief Create a send request that can be sent later. Reentrant.
   */

  void BG2S_Create (BG2S_t * request,
                    const BGML_Callback_t * cb_info,
                    const BGQuad * msginfo,
                    const char *sndbuf, unsigned sndlen, unsigned destrank);

  /**
   * \brief reset an existing send request. Set byte counters to zero.
   * This allows a request to be sent multiple times. Reentrant.
   */

  void BG2S_Reset (BG2S_t * request);

  /**
   * \brief post a request (i.e. start sending it).
   * Until the send invokes its callback, it is illegal to reset it
   * or touch the attached send buffer. Thread safe.
   */

  void BG2S_Start (BG2S_t * sender);

  /**
   * \brief control message send. message length should be < 200 bytes.
   */

  void BG2S_Control (BG2S_t * request,
                     const BGML_Callback_t * cb_info,
                     const char *sndbuf, unsigned sndlen, unsigned destrank);

  /* ********************************************************************* */
  /*                                                                       */
  /*     one-sided communication (torus)                                   */
  /*                                                                       */
  /* ********************************************************************* */

  /**
   * \brief Opaque data type that holds all 1-sided message requests.
   * The external API does not reveal the internal structure of the object.
   */

  typedef BGQuad BG1S_t[32];
  
  /**
   * \brief Generalized I/O vector data type/
   */
  
  typedef struct 
  {
    void **src_ptr_ar;  /**< Source starting addresses of each data segment. */
    void **dst_ptr_ar;  /**< Destination starting addresses of each data segment. */
    int ptr_ar_len;     /**< Number of data segment. */
    int bytes;          /**< The length of each segment in bytes. */
  } BGML_giov_t;
  
  /**
   * \brief execute a one-sided \b put to a destination node. Takes the
   * combination of base and srcbuf processed by the translation function
   * and returns the addressed buffer to the originator of request.
   *
   * \param[in] request      Data structure holding state of the \b put
   * \param[in] dstrank      Data destination rank
   * \param[in] srcbuf       Send (source) buffer on the local node
   * \param[in] base         Base address of receive (destination) buffer on the remote node
   * \param[in] dstbuf       Receive (destination) buffer on the remote node
   * \param[in] bytes        Number of bytes to transfer
   * \param[in] cb_done      Callback to invoke when message has completed
   * \param[in] isconsistent Nonzero if consistency is required
   */

  int BG1S_Memput (BG1S_t *request,
                   unsigned dstrank,
                   const char *srcbuf,
                   const unsigned base,
                   char *dstbuf, 
                   unsigned bytes, 
                   const BGML_Callback_t * cb_done,
                   unsigned isconsistent);

  /**
   * \brief execute a one-sided \b vector \b put to a destination node. Takes the
   * combination of srcbase and srcbuf processed by the translation function
   * and returns the addressed buffer to the originator of request.
   *
   * \param[in] request      Data structure holding state of the \b put
   * \param[in] dstrank      Data destination rank
   * \param[in] giov_count   Number of generalized IO vector elements describing data to put to the remote node
   * \param[in] giov_array   Array of generalized IO vectors
   * \param[in] base         Base address of receive (destination) buffers on the remote node
   * \param[in] cb_done      Callback to invoke when message has completed
   * \param[in] isconsistent Nonzero if consistency is required
   */
  
  int BG1S_MemputV (BG1S_t *request,
                    unsigned dstrank,
                    unsigned giov_count,
                    BGML_giov_t *giov_array,
                    const unsigned base,
                    const BGML_Callback_t * cb_done,
                    unsigned isconsistent);
  
  /**
   * \brief execute a one-sided \b strided \b put to a destination node. Takes the
   * combination of srcbase and srcbuf processed by the translation function
   * and returns the addressed buffer to the originator of request.
   *
   * \param[in] request        Data structure holding state of the \b put
   * \param[in] dstrank        Data destination rank
   * \param[in] src_ptr        Pointer to 1st segment at source
   * \param[in] src_stride_arr Array of strides at source
   * \param[in] src_ptr        Pointer to 1st segment at destination
   * \param[in] src_stride_arr Array of strides at destination
   * \param[in] seg_count      Number of segments at each stride levels: count[0]=bytes
   * \param[in] stride_levels  Number of stride levels
   * \param[in] base           Base address of receive (destination) buffers on the remote node
   * \param[in] cb_done        Callback to invoke when message has completed
   * \param[in] isconsistent   Nonzero if consistency is required
   */
  
  int BG1S_MemputS (BG1S_t *request,
                    unsigned dstrank,
                    void *src_ptr,  
                    int *src_stride_arr,
                    void *dst_ptr,       
                    int *dst_stride_arr,
                    int *seg_count, 
                    int stride_levels,   
                    const unsigned base,
                    const BGML_Callback_t * cb_done,
                    unsigned isconsistent);
  
  /**
   * \brief execute a one-sided \b get from a remote node.
   *
   * \param[in] request      Data structure holding state of the \b get
   * \param[in] srcrank      Data source rank
   * \param[in] dstbuf       Receive (destination) buffer on the local node
   * \param[in] base         Base address of send (source) buffer on the remote node
   * \param[in] srcbuf       Send (source) buffer on the remote node
   * \param[in] bytes        Number of bytes to transfer
   * \param[in] cb_done      Callback to invoke when message has completed
   * \param[in] isconsistent Nonzero if consistency is required
   */

  int BG1S_Memget (BG1S_t *request,
                   unsigned srcrank,
                   char *dstbuf,
                   const unsigned base,
                   const char *srcbuf,
                   unsigned bytes, 
                   const BGML_Callback_t * cb_done,
                   unsigned isconsistent);
    
  /**
   * \brief execute a one-sided \b vector \b get from a remote node.
   *
   * \param[in] request      Data structure holding state of the \b get
   * \param[in] srcrank      Data source rank
   * \param[in] giov_count   Number of generalized IO vector elements describing data to get from the remote node
   * \param[in] giov_array   Array of generalized IO vectors
   * \param[in] base         Base address of send (source) buffer on the remote node
   * \param[in] cb_done      Callback to invoke when message has completed
   * \param[in] isconsistent Nonzero if consistency is required
   */
  
  int BG1S_MemgetV (BG1S_t *request,
                    unsigned srcrank,
                    unsigned giov_count,
                    BGML_giov_t *giov_array,
                    const unsigned base,
                    const BGML_Callback_t * cb_done,
                    unsigned isconsistent);

  /**
   * \brief execute a one-sided \b strided \b get from a remote node.
   *
   * \param[in] request        Data structure holding state of the \b get
   * \param[in] srcrank        Data source rank
   * \param[in] src_ptr        Pointer to 1st segment at source
   * \param[in] src_stride_arr Array of strides at source
   * \param[in] src_ptr        Pointer to 1st segment at destination
   * \param[in] src_stride_arr Array of strides at destination
   * \param[in] seg_count      Number of segments at each stride levels: count[0]=bytes
   * \param[in] stride_levels  Number of stride levels
   * \param[in] base           Base address of send (source) buffer on the remote node
   * \param[in] cb_done        Callback to invoke when message has completed
   * \param[in] isconsistent   Nonzero if consistency is required
   */
  
  int BG1S_MemgetS (BG1S_t *request,
                    unsigned srcrank,
                    void *src_ptr,  
                    int *src_stride_arr,
                    void *dst_ptr,       
                    int *dst_stride_arr,
                    int *seg_count, 
                    int stride_levels,   
                    const unsigned base,
                    const BGML_Callback_t * cb_done,
                    unsigned isconsistent);
  
  /**
   * \brief Register a callback function to be invoked when all outstanding operations 
   *        on this node to the remote node have completed accessing local memory.
   *
   * \param[in] rank         remote node
   * \param[in] cb_done      function to invoke when local memory access has completed
   * \param[in] clientdata   clientdata referenced when \b cb_done is invoked
   *
   * \example
   * \code
   * void wait_callback (void * clientdata)
   * {
   *   unsigned * done = (unsigned *) clientdata;
   *   *done = 1;
   * }
   *
   * void wait_proc (unsigned rank)
   * {
   *   unsigned done = 0;
   *   BGML_Callback_t cb_wait_proc = {wait_callback, &done};
   *   if (BG1S_WaitProc (rank, &cb_wait_proc))
   *   {
   *     BGML_CriticalSection_enter();
   *     while (!done) BGML_Messager_advance ();
   *     BGML_CriticalSection_exit();
   *   }
   * }
   * \endcode
   */

  int BG1S_WaitProc (unsigned rank, const BGML_Callback_t * cb_done);


  /**
   * \brief Register a callback function to be invoked when all outstanding operations 
   *        on this node have completed accessing local memory.
   *
   * \param[in] cb_done      function to invoke when local memory access has completed
   * \param[in] clientdata   clientdata referenced when \b cb_done is invoked
   *
   *
   * \example
   * \code
   * void wait_callback (void * clientdata)
   * {
   *   unsigned * done = (unsigned *) clientdata;
   *   *done = 1;
   * }
   *
   * void wait_all ()
   * {
   *   unsigned done = 0;
   *   BGML_Callback_t cb_wait_all  = {wait_callback, &done};
   *   if (BG1S_WaitAll (&cb_wait_all))
   *   {
   *     BGML_CriticalSection_enter();
   *     while (!done) BGML_Messager_advance ();
   *     BGML_CriticalSection_exit();
   *   }
   * }
   * \endcode
   */

  int BG1S_WaitAll (const BGML_Callback_t * cb_done);

 
  /**
   * \brief Execute a \b read-modify-write operation.
   *
   * \param[in] request      data structure holding state of the \b rmw
   * \param[in] rank         remote rank
   * \param[in] base         base address of destination buffer
   * \param[in] dstbuf       destination (remote) buffer
   * \param[in] input        input (local) buffer
   * \param[in] output       output (local) buffer
   * \param[in] operation    operation to perfrom on remote node
   * \param[in] datatype     data type of memory at the destination, input, and output buffers
   * \param[in] cb_done      function to invoke when reception of all data has been
   *                         acknowledged by the remote node.
   * \param[in] clientdata   clientdata referenced when \b cb_done is invoked
   * \param[in] isconsistent nonzero if consistency is required
   */

  int BG1S_rmw (BG1S_t *request,
                unsigned rank,
                const unsigned base,
                void *dstbuf,
                const void *input,
                void *output,
                BGML_Op operation, 
                BGML_Dt datatype, 
                const BGML_Callback_t * cb_done,
                unsigned isconsistent);


  /**
   * \brief Execute an \b accumulate operation.
   *
   * The accumulate function is dependent on the data type, operand and operations.
   * <code>dst[i] = (src[i] op1 operand) op2 dst[i]</code>
   * Takes the
   * combination of sndbase and sndbuf processed by the translation function
   * and returns the addressed buffer to the originator of request.
   *
   * \param[in] request      Data structure holding state of the \b accumulate
   * \param[in] rank         Remote processor rank
   * \param[in] srcbuf       Source (local) buffer
   * \param[in] operand      Operand (local) buffer
   * \param[in] base         Base address of destination buffer
   * \param[in] dstbuf       Destination (remote) buffer
   * \param[in] count        Number of elements to accumulate
   * \param[in] datatype     Element data type
   * \param[in] op1          Operation to transform data as it is sent from the local node
   * \param[in] op2          Operation to transform data as it is received at the remote node
   * \param[in] cb_done      Callback to invoke when message has completed
   * \param[in] isconsistent Nonzero if consistency is required
   */

  int BG1S_Accumulate (BG1S_t *request,
                       unsigned rank,
                       const char *srcbuf,
                       const char *operand,
                       const unsigned dstbase,
                       char *dstbuf,
                       unsigned count,
                       BGML_Dt datatype,
                       BGML_Op op1,
                       BGML_Op op2,
                       const BGML_Callback_t * cb_done,
                       unsigned isconsistent);

  /**
   * \brief Execute a \b vector \b accumulate operation.
   *
   * The accumulate function is dependent on the data type, operand and operations.
   * <code>dst[i] = (src[i] op1 operand) op2 dst[i]</code>
   * Takes the
   * combination of sndbase and sndbuf processed by the translation function
   * and returns the addressed buffer to the originator of request.
   *
   * \param[in] request      Data structure holding state of the \b put
   * \param[in] rank         Remote processor rank
   * \param[in] giov_count   Number of generalized IO vector elements describing data to accumulate on the remote node
   * \param[in] giov_array   Array of generalized IO vectors
   * \param[in] operand      Operand (local) buffer
   * \param[in] base         Base address of receive (destination) buffers on the remote node
   * \param[in] datatype     Element data type
   * \param[in] op1          Operation to transform data as it is sent from the local node
   * \param[in] op2          Operation to transform data as it is received at the remote node
   * \param[in] cb_done      Callback to invoke when message has completed
   * \param[in] isconsistent Nonzero if consistency is required
   */

  int BG1S_AccumulateV (BG1S_t *request,
                        unsigned rank,
                        unsigned giov_count,
                        BGML_giov_t *giov_array,
                        const char *operand,
                        const unsigned base,
                        BGML_Dt datatype,
                        BGML_Op op1,
                        BGML_Op op2,
                        const BGML_Callback_t * cb_done,
                        unsigned isconsistent);
  
  /**
   * \brief Execute a \b strided \b accumulate operation.
   *
   * The accumulate function is dependent on the data type, operand and operations.
   * <code>dst[i] = (src[i] op1 operand) op2 dst[i]</code>
   * Takes the
   * combination of sndbase and sndbuf processed by the translation function
   * and returns the addressed buffer to the originator of request.
   *
   * \param[in] request        Data structure holding state of the \b put
   * \param[in] rank           Remote processor rank
   * \param[in] src_ptr        Pointer to 1st segment at source
   * \param[in] src_stride_arr Array of strides at source
   * \param[in] src_ptr        Pointer to 1st segment at destination
   * \param[in] src_stride_arr Array of strides at destination
   * \param[in] seg_count      Number of segments at each stride levels: count[0]=bytes
   * \param[in] stride_levels  Number of stride levels
   * \param[in] operand        Operand (local) buffer
   * \param[in] base           Base address of receive (destination) buffers on the remote node
   * \param[in] datatype       Element data type
   * \param[in] op1            Operation to transform data as it is sent from the local node
   * \param[in] op2            Operation to transform data as it is received at the remote node
   * \param[in] cb_done        Callback to invoke when message has completed
   * \param[in] isconsistent   Nonzero if consistency is required
   */
  
  int BG1S_AccumulateS (BG1S_t *request,
                        unsigned rank,
                        void *src_ptr,  
                        int *src_stride_arr,
                        void *dst_ptr,       
                        int *dst_stride_arr,
                        int *seg_count, 
                        int stride_levels,   
                        const char *operand,
                        const unsigned base,
                        BGML_Dt datatype,
                        BGML_Op op1,
                        BGML_Op op2,
                        const BGML_Callback_t * cb_done,
                        unsigned isconsistent);
  
  
  /**
   * \brief Acquire a lock resource.
   *
   * The lock request sends an opaque \c lockinfo data structure to the target
   * node which returns an opaque \c keyinfo data structure to the origin node.
   * This \c keyinfo information is used to unlock the resource.
   *
   * The implementation of the lock and unlock operations is dependent on the 
   * registered \c cb_lock and \c cb_unlock callback functions.
   *
   * \see BG1S_Unlock
   *
   * \param[in]  request        Data structure holding state of the message
   * \param[in]  rank           Remote processor rank
   * \param[in]  lockinfo       Pointer to opaque lock information
   * \param[out] keyinfo        Pointer to opaque key information
   * \param[in]  cb_done        Callback to invoke when message has completed
   * \param[in]  isconsistent   Nonzero if consistency is required
   */
  
  int BG1S_Lock (BG1S_t *request,
                 unsigned rank,
                 const BGQuad * lockinfo,
                 const BGQuad * keyinfo,
                 const BGML_Callback_t * cb_done,
                 unsigned isconsistent);
  
  /**
   * \brief Release a previously acquired lock resource.
   *
   * The unlock request sends an opaque \c keyinfo data structure to the target
   * node which unlocks the resource and allows the next lock request to proceed.
   *
   * The implementation of the lock and unlock operations is dependent on the 
   * registered \c cb_lock and \c cb_unlock callback functions.
   *
   * \see BG1S_Lock
   *
   * \param[in]  request        Data structure holding state of the message
   * \param[in]  rank           Remote processor rank
   * \param[out] keyinfo        Pointer to opaque key information
   * \param[in]  cb_done        Callback to invoke when message has completed
   * \param[in]  isconsistent   Nonzero if consistency is required
   */
  
  int BG1S_Unlock (BG1S_t *request,
                   unsigned rank,
                   const BGQuad * keyinfo,
                   const BGML_Callback_t * cb_done,
                   unsigned isconsistent);
  
  
  /**
   * \brief One-sided communication consistency models.
   */

  typedef enum
  {
    BG1S_ConsistencyModel_Relaxed    = 0,  /*!< Message are immediately sent */
    BG1S_ConsistencyModel_Weak       = 1,  /*!< Messages to the same node preserve order. */
    BG1S_ConsistencyModel_Sequential = 2,  /*!< Messages to all nodes preserve order. */
  }
  BG1S_ConsistencyModel;

  /**
   * \brief One-sided communcation configuration.
   */
  
  typedef struct
  {
    /** Callback function that is invoked to allocate memory. The first parameter 
     * is the number of bytes to allocate and the second parameter is the
     * \c cb_alloc clientdata. */
    void *(*cb_alloc) (unsigned, void *); 
    
    /** Clientdata for the \c cb_alloc callback function */
    void * cb_alloc_clientdata;
    
    /** Callback function that is invoked to free memory previously allocated
     * by the \c cb_alloc function. The first parameter is the address of the
     * memory to free and the second parameter is the \c cb_free clientdata. */
    void (*cb_free) (void *, void *);
    
    /** Clientdata for the \c cb_free callback function */
    void * cb_free_clientdata;
    
    /** Callback function that is invoked to map a raw address and base 
     * value to a memory location. The first parameter is the raw address, 
     * the second parameter is the base value, and the third parameter is the 
     * \c cb_map clientdata. */
    void *(*cb_map) (void *, unsigned, void *);
    
    /** Clientdata for the \c cb_map callback function */
    void * cb_map_clientdata;
    
    /** Callback function that is invoked on the target node when a message
     * is received and when the message completed. 
     * Parameters: 
     *   address
     *   number of bytes
     *   peer (origin) rank
     *   message identifier
     *   message complete flag
     *   \c cb_notify clientdata
     */
    void (*cb_notify) (void *, unsigned, unsigned, unsigned, unsigned, void *);
    
    /** Clientdata for the \c cb_notify callback function */
    void * cb_notify_clientdata;
    
    /** Callback function that is invoked to acquire a lock resource.
     * Parameters:
     *   Lock request information [in]
     *   \c cb_lock clientdata [in]
     *   Lock identifier -OR- Lock key [out] 
     * Returns:
     *   \c 0 if the lock was not acquired
     */
    unsigned (*cb_lock) (BGQuad *, void *, BGQuad *);
    
    /** Clientdata for the \c cb_lock callback function */
    void * cb_lock_clientdata;
    
    /** Callback function that is invoked to release a lock resource.
     * Parameters:
     *   Lock key information [in]
     *   \c cb_unlock clientdata [in]
     * Returns:
     *   Lock identifier associated with this key information
     */
    BGQuad (*cb_unlock) (BGQuad *, void *);
    
    /** Clientdata for the \c cb_lock callback function */
    void * cb_unlock_clientdata;
    
    
    /** One-sided communication consistency model */
    BG1S_ConsistencyModel consistency;
  }
  BG1S_Configuration_t;

  
  /**
   * \brief Configure one-sided communication.
   *
   * \param[in] configuration If \c NULL the configuration is not updated.
   *
   * \return The current configuration.
   */

  BG1S_Configuration_t BG1S_Configure (BG1S_Configuration_t * configuration);

  
  /**
   * \brief Returns an elapsed time on the calling processor.
   *
   * \return Time in seconds since an arbitrary time in the past. 
   */
   
  double BGML_Timer();
   

  /* ********************************************************************* */
  /*                                                                       */
  /*      Collective communication on tree and GI network                  */
  /*                                                                       */
  /* ********************************************************************* */

  /**
   * \brief Opaque data type that holds GI message requests.
   * The external API does not reveal the internal structure of the object.
   */

  typedef BGQuad BGGI_t[32];

  /**
   * \brief global interrupt barrier
   *
   * \note Assert if global interrupts are not available.
   *
   * \note do we need a non-blocking version? probably yes.
   */

  void BGGI_Barrier ();

  /**
   * \brief tree barrier
   * \param pclass: class route used for operation
   */

  void BGTr_Barrier (int pclass);

  /**
   * \brief start tree barrier operation
   * \param cb_info ?
   * \param pclass: class route used for operation
   */

  void BGTr_Barrier_start (const BGML_Callback_t * cb_info, int pclass);


  /**
   * \brief tree broadcast
   * \param root: ?
   * \param buffer: broadcast data buffer
   * \param nbytes: number of bytes broadcast
   * \param pclass: class route used for operation
   */

  void BGTr_Bcast (int root, void *buffer, int nbytes, int pclass);

  /**
   * \brief start tree broadcast operation
   * \param cb_info ?
   * \param buffer broadcast data buffer
   * \param nbytes number of bytes broadcast
   * \param pclass class route used for operation
   */

  void BGTr_Bcast_start (const BGML_Callback_t * cb_info,
                         void *buffer, int nbytes, int pclass);

  /**
   * \brief tree allreduce
   * \param sbuffer: send buffer holding non-reduced input data
   * \param rbuffer: receive buffer, holds results
   * \param count: number of entries in buffer
   * \param dt: data type of operation
   * \param op: reduce operation
   * \param root: -1 if allreduce, root if reduction
   * \param pclass: class route used for operation
   */

  void BGTr_Allreduce (const void *sbuffer,
                       void *rbuffer,
                       unsigned count,
                       BGML_Dt dt, BGML_Op op, int root, unsigned pclass);

  /**
   * \brief start tree allreduce operation
   * \param cb_info ?
   * \param sbuffer: send buffer holding non-reduced input data
   * \param rbuffer: receive buffer, holds results
   * \param count: number of entries in buffer
   * \param dt: data type of operation
   * \param op: reduce operation
   * \param root: ?
   * \param pclass: class route used for operation
   */

  void BGTr_Allreduce_start (const BGML_Callback_t * cb_info,
                             const void *sbuffer,
                             void *rbuffer,
                             unsigned count,
                             BGML_Dt dt,
                             BGML_Op op, int root, unsigned pclass);

  /**
   * \brief Configure the tree network.
   */

  void BGTr_Configure (unsigned use_coprocessor,
                       unsigned protocolTreshold, unsigned highPrecision);

  /* ********************************************************************* */
  /*                                                                       */
  /*        Collective communication on torus                              */
  /*                                                                       */
  /* ********************************************************************* */

  /**
   * \brief Opaque data type for torus collective barrier communication.
   */

  typedef BGQuad BGTsC_Barrier_t[64];

  /**
   * \brief Opaque data type for torus collective alltoallv communication.
   */

  typedef BGQuad BGTsC_Alltoallv_t[1];

  /**
   * \brief Initialize a general purpose barrier on the torus.
   * \param commID: communicator ID, 0 <= ID < N (what is N?)
   * \param numproc: number of processors participating in barrier
   * \param ranks: [SORTED] list of all ranks participating in barrier.
   * Every process initializing a barrier has to specify the same list.
   * No checking is done to ensure the compatibility of the lists.
   */

  void BGTsC_Barrier_Init (BGTsC_Barrier_t * request,
                           unsigned commID,
                           unsigned numproc, unsigned *ranks);

  /**
   * \brief Start execution of a general purpose barrier.
   *
   * Calling this function while a barrier is underway results in
   * undefined behavior.
   *
   * \param commID: communicator ID, 0 <= ID < N (what is N?)
   * \param cb ?
   */

  void BGTsC_Barrier_Start (unsigned commID, const BGML_Callback_t * cb);

  /**
   * \brief Execute a general purpose barrier.
   *
   * \param commID: communicator ID, 0 <= ID < N (what is N?)
   */
  void BGTsC_Barrier (unsigned commID);

  /**
   * \brief Torus alltoallv
   * \param request
   * \param cb
   * \param commID
   * \param numproc
   * \param ranks
   * \param sndbuf
   * \param sndlens
   * \param sdispls
   * \param rcvbuf
   * \param rcvlens
   * \param rdispls
   * \param sndcounters
   * \param rcvcounters
   */

  void BGTsC_Alltoallv_start (BGTsC_Alltoallv_t * request,
                              const BGML_Callback_t * cb,
                              unsigned commID,
                              const unsigned numproc,
                              const unsigned *ranks,
                              const char *sndbuf,
                              const unsigned *sndlens,
                              const unsigned *sdispls,
                              const char *rcvbuf,
                              const unsigned *rcvlens,
                              const unsigned *rdispls,
                              unsigned *sndcounters,
                              unsigned *rcvcounters);

  /* ********************************************************************* */
  /*                                                                       */
  /*       Collectives on rectangular communicators on torus.              */
  /*                                                                       */
  /* ********************************************************************* */

  /**
   * \brief Opaque data type for torus rectangular barrier communicator.
   */

  typedef BGQuad BGTsRC_Barrier_t[1];

  /**
   * \brief Opaque data type for torus rectangular broadcast communicator.
   */

  typedef BGQuad BGTsRC_Bcast_t[1];

  /**
   * \brief Opaque data type for torus rectangular allreduce communicator.
   */

  typedef BGQuad BGTsRC_Allreduce_t[1];

  /**
   * \brief Torus rectangular communicator geometry
   */

  typedef struct BGTsRC_Geometry_t
  {
    unsigned x0;      /**< lower left corner, x coordinate */
    unsigned y0;      /**< lower left corner, y coordinate */
    unsigned z0;      /**< lower left corner, z coordinate */
    unsigned t0;      /**< lower left corner, t coordinate */
    unsigned xs;      /**< x dimension */
    unsigned ys;      /**< y dimension */
    unsigned zs;      /**< z dimension */
    unsigned ts;      /**< t dimension */
    unsigned xr;      /**< x root (for bcast, reduce) */
    unsigned yr;      /**< y root (for bcast, reduce) */
    unsigned zr;      /**< z root (for bcast, reduce) */
    unsigned tr;      /**< t root (for bcast, reduce) */
  }
  BGTsRC_Geometry_t;

  /**
   * \brief Start execution of a torus rectangular communicator barrier.
   *
   * \param request ?
   * \param cb_info ?
   * \param commID: communicator ID, 0 <= ID < N (what is N?)
   * \param geometry ?
   */

  void BGTsRC_Barrier_start (BGTsRC_Barrier_t * request,
                             const BGML_Callback_t * cb_info,
                             unsigned commID,
                             const BGTsRC_Geometry_t * geometry);

  /**
   * \brief Start execution of a torus rectangular communicator broadcast.
   *
   * \param request ?
   * \param cb_info ?
   * \param commID: communicator ID, 0 <= ID < N (what is N?)
   * \param geometry ?
   * \param root ?
   * \param buffer ?
   * \param buflen ?
   */

  void BGTsRC_Bcast_start (BGTsRC_Bcast_t * request,
                           const BGML_Callback_t * cb_info,
                           unsigned commID,
                           const BGTsRC_Geometry_t * geometry,
                           unsigned root, char *buffer, unsigned buflen);

  /**
   * \brief Start execution of a torus rectangular communicator alltoallv.
   *
   * \param request ?
   * \param cb_info ?
   * \param commID: communicator ID, 0 <= ID < N (what is N?)
   * \param geometry ?
   * \param sbuffer ?
   * \param rbuffer ?
   * \param count ?
   * \param dt ?
   * \param op ?
   */

  void BGTsRC_Allreduce_start (BGTsRC_Allreduce_t * request,
                               const BGML_Callback_t * cb_info,
                               unsigned commID,
                               const BGTsRC_Geometry_t * geometry,
                               const void *sbuffer,
                               void *rbuffer,
                               unsigned count, BGML_Dt dt, BGML_Op op);


  
  
  /* ********************************************************************* */
  /*                                                                       */
  /*          Data types and operation types                               */
  /*                                                                       */
  /* ********************************************************************* */

  /**
   * \brief Structure for the \b BGML_SINGLE_COMPLEX datatype
   * \see BGML_Dt
   */
  typedef struct cmplx64_t
  {
    float real;         /**< Real part of the complex number */
    float imaginary;    /**< Imaginary part of the complex number */
  } cmplx64_t;

  /**
   * \brief Structure for the \b BGML_DOUBLE_COMPLEX datatype
   * \see BGML_Dt
   */
  typedef struct cmplx128_t
  {
    double real;        /**< Real part of the complex number */
    double imaginary;   /**< Imaginary part of the complex number */
  } cmplx128_t;

  /**
   * \brief Structure for the \b BGML_2INT datatype
   * \see BGML_Dt
   */
  typedef struct int32int32_t
  {
    int32_t primary;   /**< Primary part */
    int32_t secondary; /**< Secondary part */
  } int32int32_t;

  /**
   * \brief Structure for the \b BGML_SHORT_INT datatype
   * \see BGML_Dt
   */
  typedef struct int16int32_t
  {
    int16_t primary;   /**< Primary part */
    int32_t secondary; /**< Secondary part */
  } int16int32_t;

  /**
   * \brief Structure for the \b BGML_FLOAT_INT datatype
   * \see BGML_Dt
   */
  typedef struct fp32int32_t
  {
    float primary;     /**< Primary part */
    int32_t secondary; /**< Secondary part */
  } fp32int32_t;

  /**
   * \brief Structure for the \b BGML_DOUBLE_INT datatype
   * \see BGML_Dt
   */
  typedef struct fp64int32_t
  {
    double primary;    /**< Primary part */
    int32_t secondary; /**< Secondary part */
  } fp64int32_t;

  /**
   * \brief Structure for the \b BGML_2REAL datatype
   * \see BGML_Dt
   */
  typedef struct fp32fp32_t
  {
    float primary;    /**< Primary part */
    float secondary;  /**< Secondary part */
  } fp32fp32_t;

  /**
   * \brief Structure for the \b BGML_2DOUBLE_PRECISION datatype
   * \see BGML_Dt
   */
  typedef struct fp64fp64_t
  {
    double primary;   /**< Primary part */
    double secondary; /**< Secondary part */
  } fp64fp64_t;

  /*!
   * \brief Array to quickly look up the data size of a data type.
   */
 
  extern unsigned BGML_DatatypeSize[];
  
#ifdef __cplusplus
};
#endif

#endif
