/* Types.chpl:1 */
void __init_Types(void) {
  chpl_bool T1;
  T1 = (!__run_Types_firsttime14);
  if (T1) {
    goto _end___init_Types;
  }
  __run_Types_firsttime14 = false;
  _end___init_Types:;
  return;
}

/* Types.chpl:33 */
_real64 min(void) {
  _real64 T2;
  _real64 T3;
  _real64 T1;
  T1 = MIN_FLOAT64;
  T3 = T1;
  return T3;
}

/* Types.chpl:47 */
int64_t max(void) {
  int64_t T2;
  int64_t T3;
  int64_t T1;
  T1 = MAX_INT64;
  T3 = T1;
  return T3;
}

