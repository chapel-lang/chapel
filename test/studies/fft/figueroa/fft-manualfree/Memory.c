/* Memory.chpl:1 */
void __init_Memory(void) {
  chpl_bool T1;
  T1 = (!__run_Memory_firsttime20);
  if (T1) {
    goto _end___init_Memory;
  }
  __run_Memory_firsttime20 = false;
  _end___init_Memory:;
  return;
}

/* Memory.chpl:3 */
int64_t physicalMemory(locale _this_376035, MemUnits unit, int32_t _ln, _string _fn) {
  int64_t T25;
  uint64_t T1;
  int64_t T2;
  int32_t T3;
  int32_t T4;
  chpl_bool T5;
  int64_t T6;
  int32_t T7;
  int32_t T8;
  chpl_bool T9;
  int64_t T10;
  int64_t T11;
  int64_t T12;
  int32_t T13;
  int32_t T14;
  chpl_bool T15;
  int64_t T16;
  int64_t T17;
  int64_t T18;
  int32_t T19;
  int32_t T20;
  chpl_bool T21;
  int64_t T22;
  int64_t T23;
  int64_t T24;
  T1 = chpl_bytesPerLocale();
  T2 = 0;
  T3 = ((int32_t)(unit));
  T4 = ((int32_t)(MemUnits_Bytes));
  T5 = (T3==T4);
  if (T5) {
    T6 = ((int64_t)(T1));
    T2 = T6;
  } else {
    T7 = ((int32_t)(unit));
    T8 = ((int32_t)(MemUnits_KB));
    T9 = (T7==T8);
    if (T9) {
      T10 = ((int64_t)(T1));
      T11 = ((int64_t)(1024));
      T12 = (T10/T11);
      T2 = T12;
    } else {
      T13 = ((int32_t)(unit));
      T14 = ((int32_t)(MemUnits_MB));
      T15 = (T13==T14);
      if (T15) {
        T16 = ((int64_t)(T1));
        T17 = ((int64_t)(1048576));
        T18 = (T16/T17);
        T2 = T18;
      } else {
        T19 = ((int32_t)(unit));
        T20 = ((int32_t)(MemUnits_GB));
        T21 = (T19==T20);
        if (T21) {
          T22 = ((int64_t)(T1));
          T23 = ((int64_t)(1073741824));
          T24 = (T22/T23);
          T2 = T24;
        }
      }
    }
  }
  T25 = T2;
  return T25;
}

