#ifndef MAESTRO_SCHED_H
#define MAESTRO_SCHED_H

enum trigger_type{
  MTT_CORE = 1,
  MTT_SOCKET = 2,
  MTT_SYSTEM = 3,
  MTT_OTHER = 0
};

enum trigger_action{
  MTA_LOWER_STREAM_COUNT = 1,
  MTA_RAISE_STREAM_COUNT = 2,
  MTA_OTHER = 0
};


void maestro_sched(enum trigger_type type, enum trigger_action action, int val);
int maestro_allowed_workers(void);
int maestro_current_workers(int val);
int64_t maestro_size(void);

#endif
