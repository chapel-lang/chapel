#ifndef _stm_gtm_cmgr_h_
#define _stm_gtm_cmgr_h_

#define MIN_BACKOFF (1UL << 10)
#define MAX_BACKOFF (1UL << 32)

enum {GTM_CMGR_RETRY = 0, GTM_CMGR_BACKOFF};

typedef struct __cmgr_t {
  int32_t policy;
  unsigned long backoff;
} cmgr_t;

void gtm_tx_create_cmgr(void* txptr);
void gtm_tx_abort_cmgr(void* txptr);

#endif
