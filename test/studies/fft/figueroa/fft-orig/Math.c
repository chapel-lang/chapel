/* Math.chpl:1 */
void __init_Math(void) {
  chpl_bool T1;
  T1 = (!__run_Math_firsttime15);
  if (T1) {
    goto _end___init_Math;
  }
  __run_Math_firsttime15 = false;
  _end___init_Math:;
  return;
}

/* Math.chpl:78 */
int32_t _logBasePow2Help(int64_t val, int32_t baseLog2, int32_t _ln, _string _fn) {
  int32_t T1;
  int64_t T3;
  int64_t T2;
  int32_t T12;
  int32_t T4;
  chpl_bool T7;
  int64_t T5;
  chpl_bool T6;
  int64_t T8;
  int32_t T9;
  int64_t T10;
  chpl_bool T11;
  T1 = baseLog2;
  T2 = val;
  T3 = T2;
  T4 = -1;
  T5 = ((int64_t)(0));
  T6 = (T2!=T5);
  T7 = T6;
  while (T7) {
    T8 = (T3>>T1);
    T3 = T8;
    T9 = (T4+1);
    T4 = T9;
    T10 = ((int64_t)(0));
    T11 = (T8!=T10);
    T7 = T11;
  }
  T12 = T4;
  return T12;
}

/* Math.chpl:87 */
int32_t logBasePow2(int64_t val, int32_t baseLog2, int32_t _ln, _string _fn) {
  int64_t T1;
  int32_t T5;
  int64_t T2;
  chpl_bool T3;
  int32_t T4;
  T1 = val;
  T2 = ((int64_t)(1));
  T3 = (T1<T2);
  if (T3) {
    halt("Can't take the log() of a non-positive integer", _ln, _fn);
  }
  T4 = _logBasePow2Help(T1, baseLog2, _ln, _fn);
  T5 = T4;
  return T5;
}

/* Math.chpl:101 */
int32_t _log2_76322(int64_t val, int32_t _ln, _string _fn) {
  int64_t T1;
  int32_t T3;
  int32_t T2;
  T1 = val;
  T2 = logBasePow2(T1, 1, _ln, _fn);
  T3 = T2;
  return T3;
}

