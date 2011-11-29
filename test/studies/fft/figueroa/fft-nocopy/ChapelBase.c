/* ChapelBase.chpl:1 */
void __init_ChapelBase(int32_t _ln, _string _fn) {
  chpl_bool T1;
  int32_t T5;
  chpl_bool T2;
  chpl_bool T3;
  int32_t T4;
  _string T6;
  int32_t T7;
  int32_t T11;
  chpl_bool T8;
  chpl_bool T9;
  int32_t T10;
  _string T12;
  int32_t T13;
  int32_t T14;
  chpl_bool T15;
  chpl_bool T16;
  _string T17;
  _string T18;
  _string T19;
  _string T20;
  _string T21;
  chpl_bool T22;
  _string T23;
  _string T24;
  T1 = (!__run_ChapelBase_firsttime0);
  if (T1) {
    goto _end___init_ChapelBase;
  }
  __run_ChapelBase_firsttime0 = false;
  T2 = _config_has_value("numLocales", "ChapelBase");
  T3 = (!T2);
  if (T3) {
    T4 = _chpl_comm_default_num_locales();
    T5 = T4;
  } else {
    T6 = _config_get_value("numLocales", "ChapelBase");
    T7 = _string_to_int32_t(T6, _ln, _fn);
    T5 = T7;
  }
  numLocales = T5;
  T8 = _config_has_value("maxThreads", "ChapelBase");
  T9 = (!T8);
  if (T9) {
    T10 = chpl_maxThreads();
    T11 = T10;
  } else {
    T12 = _config_get_value("maxThreads", "ChapelBase");
    T13 = _string_to_int32_t(T12, _ln, _fn);
    T11 = T13;
  }
  maxThreads = T11;
  T14 = chpl_maxThreadsLimit();
  maxThreadsLimit = T14;
  T15 = (maxThreadsLimit!=0);
  if (T15) {
    T16 = (maxThreads>maxThreadsLimit);
    if (T16) {
      T17 = int32_t_to_string(maxThreads);
      T18 = string_concat("specified value of ", T17, _ln, _fn);
      T19 = string_concat(T18, " for maxThreads is too high; limit is ", _ln, _fn);
      T20 = int32_t_to_string(maxThreadsLimit);
      T21 = string_concat(T19, T20, _ln, _fn);
      chpl_warning(T21, _ln, _fn);
    } else {
      T22 = (maxThreads==0);
      if (T22) {
        T23 = int32_t_to_string(maxThreadsLimit);
        T24 = string_concat("maxThreads is unbounded; however, the limit is ", T23, _ln, _fn);
        chpl_warning(T24, _ln, _fn);
      }
    }
  }
  _end___init_ChapelBase:;
  return;
}

/* ChapelBase.chpl:565 */
void init_elts(_ddata_locale x, int32_t s, int32_t _ln, _string _fn) {
  _ddata_locale T1 = NULL;
  range_int32_t_bounded_0 T2;
  range_int32_t_bounded_0 T3;
  int32_t T4;
  int32_t T6;
  int32_t T5;
  int32_t T8;
  int32_t T7;
  _ref_int32_t T9 = NULL;
  locale T10 = NULL;
  int32_t T11;
  T1 = x;
  T2 = _build_range(1, s);
  T3 = _copy(&(T2));
  T4 = 0;
  T5 = (T3._low);
  T6 = T5;
  T7 = (T3._high);
  T8 = T7;
  for (T4 = T6; T4 <= T8; T4 += 1) {
    T9 = &(T4);
    T10 = ((locale)(nil));
    T11 = (T4-1);
    _ARRAY_SET(T1, T11, T10);
  }
  return;
}

/* ChapelBase.chpl:565 */
void _init_elts_333870(_ddata_int32_t x, int32_t s) {
  _ddata_int32_t T1 = NULL;
  range_int32_t_bounded_0 T2;
  range_int32_t_bounded_0 T3;
  int32_t T4;
  int32_t T6;
  int32_t T5;
  int32_t T8;
  int32_t T7;
  _ref_int32_t T9 = NULL;
  int32_t T10;
  T1 = x;
  T2 = _build_range(1, s);
  T3 = _copy(&(T2));
  T4 = 0;
  T5 = (T3._low);
  T6 = T5;
  T7 = (T3._high);
  T8 = T7;
  for (T4 = T6; T4 <= T8; T4 += 1) {
    T9 = &(T4);
    T10 = (T4-1);
    _ARRAY_SET(T1, T10, 0);
  }
  return;
}

/* ChapelBase.chpl:565 */
void _init_elts_462899(_ddata__complex128 x, int32_t s) {
  _ddata__complex128 T1 = NULL;
  range_int32_t_bounded_0 T2;
  range_int32_t_bounded_0 T3;
  int32_t T4;
  int32_t T6;
  int32_t T5;
  int32_t T8;
  int32_t T7;
  _ref_int32_t T9 = NULL;
  _complex128 T10;
  int32_t T11;
  T1 = x;
  T2 = _build_range(1, s);
  T3 = _copy(&(T2));
  T4 = 0;
  T5 = (T3._low);
  T6 = T5;
  T7 = (T3._high);
  T8 = T7;
  for (T4 = T6; T4 <= T8; T4 += 1) {
    T9 = &(T4);
    T10.re = 0.0;
    T10.im = 0.0;
    T11 = (T4-1);
    _ARRAY_SET(T1, T11, T10);
  }
  return;
}

/* ChapelBase.chpl:574 */
_ddata__complex128 __construct__ddata_449871(int32_t size, _ddata__complex128 meme, int32_t _ln, _string _fn) {
  _ddata__complex128 T2 = NULL;
  chpl_bool T1;
  object T3 = NULL;
  T1 = (nil==meme);
  if (T1) {
    T2 = (_ddata__complex128)chpl_alloc(sizeof(__ddata__complex128), "instance of class _ddata", _ln, _fn);
    ((object)T2)->_cid = _e__ddata__complex128;
  } else {
    T2 = meme;
  }
  T3 = (&(T2->super));
  _construct_object(T3, _ln, _fn);
  T2->size = size;
  return T2;
}

/* ChapelBase.chpl:574 */
_ddata_locale _construct__ddata(int32_t size, _ddata_locale meme, int32_t _ln, _string _fn) {
  _ddata_locale T2 = NULL;
  chpl_bool T1;
  object T3 = NULL;
  T1 = (nil==meme);
  if (T1) {
    T2 = (_ddata_locale)chpl_alloc(sizeof(__ddata_locale), "instance of class _ddata", _ln, _fn);
    ((object)T2)->_cid = _e__ddata_locale;
  } else {
    T2 = meme;
  }
  T3 = (&(T2->super));
  _construct_object(T3, _ln, _fn);
  T2->size = size;
  return T2;
}

/* ChapelBase.chpl:574 */
_ddata_int32_t __construct__ddata_324090(int32_t size, _ddata_int32_t meme, int32_t _ln, _string _fn) {
  _ddata_int32_t T2 = NULL;
  chpl_bool T1;
  object T3 = NULL;
  T1 = (nil==meme);
  if (T1) {
    T2 = (_ddata_int32_t)chpl_alloc(sizeof(__ddata_int32_t), "instance of class _ddata", _ln, _fn);
    ((object)T2)->_cid = _e__ddata_int32_t;
  } else {
    T2 = meme;
  }
  T3 = (&(T2->super));
  _construct_object(T3, _ln, _fn);
  T2->size = size;
  return T2;
}

/* ChapelBase.chpl:645 */
_syncvar_chpl_bool __construct__syncvar_363728(chpl_bool value, int32_t _ln, _string _fn) {
  _syncvar_chpl_bool T1 = NULL;
  T1 = (_syncvar_chpl_bool)chpl_alloc(sizeof(__syncvar_chpl_bool), "instance of class _syncvar", _ln, _fn);
  T1->value = value;
  _initialize_681110(T1);
  return T1;
}

/* ChapelBase.chpl:645 */
_syncvar_int64_t __construct__syncvar_363511(int64_t value, int32_t _ln, _string _fn) {
  _syncvar_int64_t T1 = NULL;
  T1 = (_syncvar_int64_t)chpl_alloc(sizeof(__syncvar_int64_t), "instance of class _syncvar", _ln, _fn);
  T1->value = value;
  _initialize_681011(T1);
  return T1;
}

/* ChapelBase.chpl:645 */
_syncvar_uint64_t _construct__syncvar(uint64_t value, int32_t _ln, _string _fn) {
  _syncvar_uint64_t T1 = NULL;
  T1 = (_syncvar_uint64_t)chpl_alloc(sizeof(__syncvar_uint64_t), "instance of class _syncvar", _ln, _fn);
  T1->value = value;
  _initialize_679792(T1);
  return T1;
}

/* ChapelBase.chpl:652 */
void _initialize_681110(_syncvar_chpl_bool _this_681114) {
  chpl_init_sync_aux(&((_this_681114)->sync_aux));
  return;
}

/* ChapelBase.chpl:652 */
void _initialize_679792(_syncvar_uint64_t _this_679796) {
  chpl_init_sync_aux(&((_this_679796)->sync_aux));
  return;
}

/* ChapelBase.chpl:652 */
void _initialize_681011(_syncvar_int64_t _this_681015) {
  chpl_init_sync_aux(&((_this_681015)->sync_aux));
  return;
}

/* ChapelBase.chpl:665 */
_syncvar_chpl_bool __pass_374812(_syncvar_chpl_bool sv) {
  _syncvar_chpl_bool T1 = NULL;
  T1 = sv;
  return T1;
}

/* ChapelBase.chpl:665 */
_syncvar_int64_t __pass_374834(_syncvar_int64_t sv) {
  _syncvar_int64_t T1 = NULL;
  T1 = sv;
  return T1;
}

/* ChapelBase.chpl:665 */
_syncvar_uint64_t _pass(_syncvar_uint64_t sv) {
  _syncvar_uint64_t T1 = NULL;
  T1 = sv;
  return T1;
}

/* ChapelBase.chpl:685 */
chpl_bool readFE(_syncvar_chpl_bool _this_651658, int32_t _ln, _string _fn) {
  chpl_bool T2;
  chpl_bool T1;
  chpl_sync_wait_full_and_lock(&((_this_651658)->sync_aux), _ln, _fn);
  T1 = (_this_651658->value);
  chpl_sync_mark_and_signal_empty(&((_this_651658)->sync_aux));
  T2 = T1;
  return T2;
}

/* ChapelBase.chpl:719 */
uint64_t readXX(_syncvar_uint64_t _this_660108) {
  uint64_t T2;
  uint64_t T1;
  chpl_sync_lock(&((_this_660108)->sync_aux));
  T1 = (_this_660108->value);
  chpl_sync_unlock(&((_this_660108)->sync_aux));
  T2 = T1;
  return T2;
}

/* ChapelBase.chpl:736 */
void _writeEF_661172(_syncvar_uint64_t _this_661177, uint64_t val, int32_t _ln, _string _fn) {
  chpl_sync_wait_empty_and_lock(&((_this_661177)->sync_aux), _ln, _fn);
  _this_661177->value = val;
  chpl_sync_mark_and_signal_full(&((_this_661177)->sync_aux));
  return;
}

/* ChapelBase.chpl:736 */
void writeEF(_syncvar_int64_t _this_370323, int64_t val, int32_t _ln, _string _fn) {
  chpl_sync_wait_empty_and_lock(&((_this_370323)->sync_aux), _ln, _fn);
  _this_370323->value = val;
  chpl_sync_mark_and_signal_full(&((_this_370323)->sync_aux));
  return;
}

/* ChapelBase.chpl:736 */
void _writeEF_373453(_syncvar_chpl_bool _this_373458, chpl_bool val, int32_t _ln, _string _fn) {
  chpl_sync_wait_empty_and_lock(&((_this_373458)->sync_aux), _ln, _fn);
  _this_373458->value = val;
  chpl_sync_mark_and_signal_full(&((_this_373458)->sync_aux));
  return;
}

/* ChapelBase.chpl:748 */
_syncvar_uint64_t __EQUAL__661104(_syncvar_uint64_t sv, uint64_t val, int32_t _ln, _string _fn) {
  _syncvar_uint64_t T1 = NULL;
  _writeEF_661172(sv, val, _ln, _fn);
  T1 = sv;
  return T1;
}

/* ChapelBase.chpl:748 */
_syncvar_chpl_bool __EQUAL__373416(_syncvar_chpl_bool sv, chpl_bool val, int32_t _ln, _string _fn) {
  _syncvar_chpl_bool T1 = NULL;
  _writeEF_373453(sv, val, _ln, _fn);
  T1 = sv;
  return T1;
}

/* ChapelBase.chpl:748 */
_syncvar_int64_t __EQUAL__364076(_syncvar_int64_t sv, int64_t val, int32_t _ln, _string _fn) {
  _syncvar_int64_t T1 = NULL;
  writeEF(sv, val, _ln, _fn);
  T1 = sv;
  return T1;
}

/* ChapelBase.chpl:780 */
void reset(_syncvar_uint64_t _this_661279) {
  chpl_sync_lock(&((_this_661279)->sync_aux));
  _this_661279->value = UINT64(0);
  chpl_sync_mark_and_signal_empty(&((_this_661279)->sync_aux));
  return;
}

/* ChapelBase.chpl:794 */
chpl_bool isFull(_syncvar_uint64_t _this_658908) {
  chpl_bool T2;
  chpl_bool T1;
  T1 = chpl_sync_is_full(&((_this_658908)->value), &((_this_658908)->sync_aux), false);
  T2 = T1;
  return T2;
}

/* ChapelBase.chpl:909 */
_EndCount _construct__EndCount(_syncvar_int64_t i, _syncvar_chpl_bool b, _EndCount meme, int32_t _ln, _string _fn) {
  _syncvar_chpl_bool T1 = NULL;
  _syncvar_int64_t T2 = NULL;
  _EndCount T4 = NULL;
  chpl_bool T3;
  object T5 = NULL;
  T1 = __pass_374812(b);
  T2 = __pass_374834(i);
  T3 = (nil==meme);
  if (T3) {
    T4 = (_EndCount)chpl_alloc(sizeof(__EndCount), "instance of class _EndCount", _ln, _fn);
    ((object)T4)->_cid = _e__EndCount;
  } else {
    T4 = meme;
  }
  T5 = (&(T4->super));
  _construct_object(T5, _ln, _fn);
  T4->i = T2;
  T4->b = T1;
  return T4;
}

/* ChapelBase.chpl:914 */
_EndCount _endCountAlloc(int32_t _ln, _string _fn) {
  _EndCount T8 = NULL;
  _EndCount T7 = NULL;
  _EndCount T1 = NULL;
  _syncvar_int64_t T2 = NULL;
  _syncvar_int64_t T4 = NULL;
  int64_t T3;
  _syncvar_chpl_bool T5 = NULL;
  _syncvar_chpl_bool T6 = NULL;
  T1 = (_EndCount)chpl_alloc(sizeof(__EndCount), "instance of class _unknown", _ln, _fn);
  ((object)T1)->_cid = _e__EndCount;
  T2 = __construct__syncvar_363511(0, _ln, _fn);
  T3 = ((int64_t)(0));
  T4 = __EQUAL__364076(T2, T3, _ln, _fn);
  T1->i = T4;
  T5 = __construct__syncvar_363728(false, _ln, _fn);
  T6 = __EQUAL__373416(T5, true, _ln, _fn);
  T1->b = T6;
  T7 = _construct__EndCount(T4, T6, T1, _ln, _fn);
  T8 = T7;
  return T8;
}

/* ChapelBase.chpl:930 */
void _waitEndCount(_EndCount e, int32_t _ln, _string _fn) {
  _syncvar_chpl_bool T1 = NULL;
  T1 = (e->b);
  readFE(T1, _ln, _fn);
  return;
}

/* ChapelBase.chpl:944 */
void __waitEndCount_11440(_EndCount _endCount, int32_t _ln, _string _fn) {
  _waitEndCount(_endCount, _ln, _fn);
  return;
}

/* ChapelBase.chpl:1137 */
void _cfor_inc(_ref_int32_t i, int32_t s) {
  int32_t T1;
  T1 = *(i);
  (*i) = T1;
  return;
}

/* ChapelBase.chpl:1137 */
void __cfor_inc_491492(_ref_int64_t i, int32_t s) {
  int64_t T1;
  T1 = *(i);
  (*i) = T1;
  return;
}

