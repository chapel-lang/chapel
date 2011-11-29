/* Time.chpl:1 */
void __init_Time(void) {
  chpl_bool T1;
  T1 = (!__run_Time_firsttime17);
  if (T1) {
    goto _end___init_Time;
  }
  __run_Time_firsttime17 = false;
  _end___init_Time:;
  return;
}

/* Time.chpl:4 */
_real64 getCurrentTime(TimeUnits unit, int32_t _ln, _string _fn) {
  _real64 T3;
  _real64 T1;
  _real64 T2;
  T1 = _now_time();
  T2 = _convert_microseconds(unit, T1, _ln, _fn);
  T3 = T2;
  return T3;
}

/* Time.chpl:69 */
_real64 _convert_microseconds(TimeUnits unit, _real64 us, int32_t _ln, _string _fn) {
  _real64 T4;
  int32_t T1;
  int32_t T2;
  chpl_bool T3;
  int32_t T5;
  int32_t T6;
  chpl_bool T7;
  _real64 T8;
  int32_t T9;
  int32_t T10;
  chpl_bool T11;
  _real64 T12;
  int32_t T13;
  int32_t T14;
  chpl_bool T15;
  _real64 T16;
  int32_t T17;
  int32_t T18;
  chpl_bool T19;
  _real64 T20;
  _real64 T21;
  T1 = ((int32_t)(unit));
  T2 = ((int32_t)(TimeUnits_microseconds));
  T3 = (T1==T2);
  if (T3) {
    T4 = us;
    goto _end__convert_microseconds;
  } else {
    T5 = ((int32_t)(unit));
    T6 = ((int32_t)(TimeUnits_milliseconds));
    T7 = (T5==T6);
    if (T7) {
      T8 = (us/1.0e+3);
      T4 = T8;
      goto _end__convert_microseconds;
    } else {
      T9 = ((int32_t)(unit));
      T10 = ((int32_t)(TimeUnits_seconds));
      T11 = (T9==T10);
      if (T11) {
        T12 = (us/1.0e+6);
        T4 = T12;
        goto _end__convert_microseconds;
      } else {
        T13 = ((int32_t)(unit));
        T14 = ((int32_t)(TimeUnits_minutes));
        T15 = (T13==T14);
        if (T15) {
          T16 = (us/60.0e+6);
          T4 = T16;
          goto _end__convert_microseconds;
        } else {
          T17 = ((int32_t)(unit));
          T18 = ((int32_t)(TimeUnits_hours));
          T19 = (T17==T18);
          if (T19) {
            T20 = (us/3600.0e+6);
            T4 = T20;
            goto _end__convert_microseconds;
          }
        }
      }
    }
  }
  halt("internal error in module Time", _ln, _fn);
  T21 = (-1.0);
  T4 = T21;
  _end__convert_microseconds:;
  return T4;
}

