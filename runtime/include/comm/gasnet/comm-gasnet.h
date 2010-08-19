//
// The following macro is from the GASNet test.h distribution
// SVNFIXME: put this in the right place
//

#define GASNET_Safe(fncall) do {					\
    int _retval;							\
    if ((_retval = fncall) != GASNET_OK) {				\
      fprintf(stderr, "ERROR calling: %s\n"				\
	      " at: %s:%i\n"						\
	      " error: %s (%s)\n",					\
	      #fncall, __FILE__, __LINE__,				\
	      gasnet_ErrorName(_retval), gasnet_ErrorDesc(_retval));    \
      fflush(stderr);                                                   \
      gasnet_exit(_retval);                                             \
    }                                                                   \
  } while(0)


//
// AM functions
//
#define FORK          128 // synchronous fork
#define FORK_LARGE    129 // synchronous fork with a huge argument
#define FORK_NB       130 // non-blocking fork 
#define FORK_NB_LARGE 131 // non-blocking fork with a huge argument
#define FORK_FAST     132 // run the function in the handler (use with care)
#define SIGNAL        133 // ack of synchronous fork
#define PRIV_BCAST    134 // put data at addr (used for private broadcast)
#define PRIV_BCAST_LARGE 135 // put data at addr (used for private broadcast)
#define FREE          136 // free data at addr
#define EXIT_ANY      137 // free data at addr

void AM_fork(gasnet_token_t token, void* buf, size_t nbytes);
void AM_fork_large(gasnet_token_t token, void* buf, size_t nbytes);
void AM_fork_nb(gasnet_token_t token, void* buf, size_t nbytes);
void AM_fork_nb_large(gasnet_token_t token, void* buf, size_t nbytes);
void AM_fork_fast(gasnet_token_t token, void* buf, size_t nbytes);
void AM_signal(gasnet_token_t token, void* buf, size_t nbytes);
void AM_priv_bcast(gasnet_token_t token, void* buf, size_t nbytes);
void AM_priv_bcast_large(gasnet_token_t token, void* buf, size_t nbytes);
void AM_free(gasnet_token_t token, void* buf, size_t nbytes);
void AM_exit_any(gasnet_token_t token, void* buf, size_t nbytes);


#ifdef CHPL_STM_GTM

//
// STM AM functions
//
#define TX_ABORT        150
#define TX_COMMITPH1    151
#define TX_COMMITPH2    152
#define TX_GET          153
#define TX_GETDATA      154
#define TX_PUT          155
#define TX_FORK         156
#define TX_FORK_FAST    157
#define TX_SIGNAL       158

void AM_tx_abort(gasnet_token_t token, void* buf, size_t nbytes);
void AM_tx_commitPh1(gasnet_token_t token, void* buf, size_t nbytes);
void AM_tx_commitPh2(gasnet_token_t token, void* buf, size_t nbytes);
void AM_tx_getdata (gasnet_token_t token, void* buf, size_t nbytes, int status);
void AM_tx_get (gasnet_token_t token, void* buf, size_t nbytes);
void AM_tx_put (gasnet_token_t token, void* buf, size_t nbytes);
void AM_tx_fork(gasnet_token_t token, void* buf, size_t nbytes);
void AM_tx_signal(gasnet_token_t token, void* buf, size_t nbytes, int status);

void AM_tx_abort_fast(gasnet_token_t token, void* buf, size_t nbytes);
void AM_tx_commitPh1_fast(gasnet_token_t token, void* buf, size_t nbytes);
void AM_tx_commitPh2_fast(gasnet_token_t token, void* buf, size_t nbytes);
void AM_tx_getdata_fast(gasnet_token_t token, void* buf, size_t nbytes, int status);
void AM_tx_get_fast(gasnet_token_t token, void* buf, size_t nbytes);
void AM_tx_put_fast(gasnet_token_t token, void* buf, size_t nbytes);
void AM_tx_fork_fast(gasnet_token_t token, void* buf, size_t nbytes);

#if STM_GTM_NOFAST

 static gasnet_handlerentry_t ftable[] = {
  {FORK,          AM_fork},
  {FORK_LARGE,    AM_fork_large},
  {FORK_NB,       AM_fork_nb},
  {FORK_NB_LARGE, AM_fork_nb_large},
  {FORK_FAST,     AM_fork_fast},
  {SIGNAL,        AM_signal},
  {PRIV_BCAST,    AM_priv_bcast},
  {PRIV_BCAST_LARGE, AM_priv_bcast_large},
  {FREE,          AM_free},
  {EXIT_ANY,      AM_exit_any},
  {TX_ABORT,      AM_tx_abort},
  {TX_COMMITPH1,  AM_tx_commitPh1},
  {TX_COMMITPH2,  AM_tx_commitPh2},
  {TX_GET,        AM_tx_get},
  {TX_GETDATA,    AM_tx_getdata},
  {TX_PUT,        AM_tx_put},
  {TX_FORK,       AM_tx_fork},
  {TX_FORK_FAST,  AM_tx_fork_fast},
  {TX_SIGNAL,     AM_tx_signal}
};

#else 

static gasnet_handlerentry_t ftable[] = {
  {FORK,          AM_fork},
  {FORK_LARGE,    AM_fork_large},
  {FORK_NB,       AM_fork_nb},
  {FORK_NB_LARGE, AM_fork_nb_large},
  {FORK_FAST,     AM_fork_fast},
  {SIGNAL,        AM_signal},
  {PRIV_BCAST,    AM_priv_bcast},
  {PRIV_BCAST_LARGE, AM_priv_bcast_large},
  {FREE,          AM_free},
  {EXIT_ANY,      AM_exit_any},
  {TX_ABORT,      AM_tx_abort_fast},
  {TX_COMMITPH1,  AM_tx_commitPh1_fast},
  {TX_COMMITPH2,  AM_tx_commitPh2_fast},
  {TX_GET,        AM_tx_get_fast},
  {TX_GETDATA,    AM_tx_getdata},
  {TX_PUT,        AM_tx_put_fast},
  {TX_FORK,       AM_tx_fork},
  {TX_FORK_FAST,  AM_tx_fork_fast},
  {TX_SIGNAL,     AM_tx_signal}, 
};

#endif 

#else

static gasnet_handlerentry_t ftable[] = {
  {FORK,          AM_fork},
  {FORK_LARGE,    AM_fork_large},
  {FORK_NB,       AM_fork_nb},
  {FORK_NB_LARGE, AM_fork_nb_large},
  {FORK_FAST,     AM_fork_fast},
  {SIGNAL,        AM_signal},
  {PRIV_BCAST,    AM_priv_bcast},
  {PRIV_BCAST_LARGE, AM_priv_bcast_large},
  {FREE,          AM_free},
  {EXIT_ANY,      AM_exit_any}
};

#endif
