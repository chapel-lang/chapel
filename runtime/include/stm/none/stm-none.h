#ifndef _chpl_cfg_stm_h_
#define _chpl_cfg_stm_h_

typedef int chpl_stm_tx_env_t;
#define chpl_stm_tx_set_env(txenv)

typedef struct __chpl_stm_tx_t {
  int32_t numremlocales;
  int32_t* remlocales;
  chpl_stm_stats_p counters; 
} chpl_stm_tx_t;

#endif 
