/* Schedules.chpl:1 */
void __init_Schedules(void) {
  chpl_bool T1;
  T1 = (!__run_Schedules_firsttime21);
  if (T1) {
    goto _end___init_Schedules;
  }
  __run_Schedules_firsttime21 = false;
  _end___init_Schedules:;
  return;
}

