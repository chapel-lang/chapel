#ifndef _stm_gtm_stats_h_
#define _stm_gtm_stats_h_

void gtm_stats_init(void);
void gtm_stats_exit(void);

void gtm_stats_tx_abort(_real64);
void gtm_stats_tx_comm_abort(_real64);

void gtm_stats_tx_commitPh1(_real64);
void gtm_stats_tx_comm_commitPh1(_real64);

void gtm_stats_tx_commitPh2(_real64);
void gtm_stats_tx_comm_commitPh2(_real64);

void gtm_stats_tx_load(_real64);
void gtm_stats_tx_get(_real64);
void gtm_stats_tx_comm_get(_real64);

void gtm_stats_tx_store(_real64);
void gtm_stats_tx_put(_real64);
void gtm_stats_tx_comm_put(_real64);

void gtm_stats_tx_fork(_real64);
void gtm_stats_tx_comm_fork(_real64);

#endif
