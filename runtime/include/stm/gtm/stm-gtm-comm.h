#ifndef _stm_gtm_comm_gasnet_h_
#define _stm_gtm_comm_gasnet_h_

typedef struct {
  int32_t txid;         // transaction id
  int32_t txlocale;     // locale which issued the topmost stm_begin
  int32_t caller;       // locale from which this request originated
  int32_t txstatus;     // transaction's status
  int* commstatus;      // flag that signals end of comm operation
} tx_generic_t;

typedef struct {
  int32_t txid;
  int32_t txlocale;
  int32_t caller;
  int32_t txstatus;
  int* commstatus;
  size_t datasize;      
  void* remaddr;        // source address (on remote locale) 
  void* retaddr;        // dest address (on caller locale)
} tx_get_t;

typedef struct { 
  int* commstatus;
  size_t datasize;
  void* retaddr; 
  char data[0];        // data returned by get 
} tx_getdata_t;

typedef struct {
  int32_t txid;
  int32_t txlocale;
  int32_t caller;
  int32_t txstatus;
  int* commstatus;
  size_t datasize;
  void* remaddr;        // dest address (on remote locale) 
  char data[0];        // data to store (placement on top for alignment)
} tx_put_t;

typedef struct {
  void* txdesc;
  int32_t txid;
  int32_t txlocale;
  int32_t caller;
  int32_t txstatus;
  int* commstatus;
  chpl_fn_int_t fid;
  size_t argsize;        
  char arg[0];         // list of arguments to fork function
} tx_fork_t;

typedef struct {
  void* txdesc;
  int* commstatus;
  int status;
  int32_t numremlocales;
  int32_t* remlocales;
} tx_forksignal_t;

#endif
