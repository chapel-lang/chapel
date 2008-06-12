/* ChapelRange.chpl:1 */
void __init_ChapelRange(void) {
  chpl_bool T1;
  T1 = (!__run_ChapelRange_firsttime6);
  if (T1) {
    goto _end___init_ChapelRange;
  }
  __run_ChapelRange_firsttime6 = false;
  _end___init_ChapelRange:;
  return;
}

/* ChapelRange.chpl:9 */
range_int32_t_bounded_0 _copy(range_int32_t_bounded_0* const x) {
  range_int32_t_bounded_0 T5;
  int32_t T1;
  int32_t T2;
  int32_t T3;
  range_int32_t_bounded_0 T4;
  T1 = ((*x)._low);
  T2 = ((*x)._high);
  T3 = ((*x)._stride);
  T4 = _construct_range(BoundedRangeType_bounded, false, T1, T2, T3);
  T5 = T4;
  return T5;
}

/* ChapelRange.chpl:9 */
range_int64_t_bounded_0 __copy_429953(range_int64_t_bounded_0* const x) {
  range_int64_t_bounded_0 T5;
  int64_t T1;
  int64_t T2;
  int32_t T3;
  range_int64_t_bounded_0 T4;
  T1 = ((*x)._low);
  T2 = ((*x)._high);
  T3 = ((*x)._stride);
  T4 = __construct_range_428854(BoundedRangeType_bounded, false, T1, T2, T3);
  T5 = T4;
  return T5;
}

/* ChapelRange.chpl:9 */
range_int32_t_bounded_1 __copy_536668(range_int32_t_bounded_1* const x) {
  range_int32_t_bounded_1 T5;
  int32_t T1;
  int32_t T2;
  int32_t T3;
  range_int32_t_bounded_1 T4;
  T1 = ((*x)._low);
  T2 = ((*x)._high);
  T3 = ((*x)._stride);
  T4 = __construct_range_536229(BoundedRangeType_bounded, true, T1, T2, T3);
  T5 = T4;
  return T5;
}

/* ChapelRange.chpl:9 */
range_int64_t_bounded_1 __copy_547955(range_int64_t_bounded_1* const x) {
  range_int64_t_bounded_1 T5;
  int64_t T1;
  int64_t T2;
  int32_t T3;
  range_int64_t_bounded_1 T4;
  T1 = ((*x)._low);
  T2 = ((*x)._high);
  T3 = ((*x)._stride);
  T4 = __construct_range_546991(BoundedRangeType_bounded, true, T1, T2, T3);
  T5 = T4;
  return T5;
}

/* ChapelRange.chpl:9 */
range_int32_t_boundedLow_0 __copy_562043(range_int32_t_boundedLow_0* const x) {
  range_int32_t_boundedLow_0 T5;
  int32_t T1;
  int32_t T2;
  int32_t T3;
  range_int32_t_boundedLow_0 T4;
  T1 = ((*x)._low);
  T2 = ((*x)._high);
  T3 = ((*x)._stride);
  T4 = __construct_range_561334(BoundedRangeType_boundedLow, false, T1, T2, T3);
  T5 = T4;
  return T5;
}

/* ChapelRange.chpl:9 */
range_int32_t_boundedLow_0 __construct_range_561334(BoundedRangeType boundedType, chpl_bool stridable, int32_t _low, int32_t _high, int32_t _stride) {
  range_int32_t_boundedLow_0 T1;
  T1._low = _low;
  T1._high = _high;
  T1._stride = _stride;
  return T1;
}

/* ChapelRange.chpl:9 */
range_int32_t_bounded_0 _construct_range(BoundedRangeType boundedType, chpl_bool stridable, int32_t _low, int32_t _high, int32_t _stride) {
  range_int32_t_bounded_0 T1;
  T1._low = _low;
  T1._high = _high;
  T1._stride = _stride;
  return T1;
}

/* ChapelRange.chpl:9 */
range_int64_t_bounded_0 __construct_range_428854(BoundedRangeType boundedType, chpl_bool stridable, int64_t _low, int64_t _high, int32_t _stride) {
  range_int64_t_bounded_0 T1;
  T1._low = _low;
  T1._high = _high;
  T1._stride = _stride;
  return T1;
}

/* ChapelRange.chpl:9 */
range_int32_t_bounded_1 __construct_range_536229(BoundedRangeType boundedType, chpl_bool stridable, int32_t _low, int32_t _high, int32_t _stride) {
  range_int32_t_bounded_1 T1;
  T1._low = _low;
  T1._high = _high;
  T1._stride = _stride;
  return T1;
}

/* ChapelRange.chpl:9 */
range_int64_t_bounded_1 __construct_range_546991(BoundedRangeType boundedType, chpl_bool stridable, int64_t _low, int64_t _high, int32_t _stride) {
  range_int64_t_bounded_1 T1;
  T1._low = _low;
  T1._high = _high;
  T1._stride = _stride;
  return T1;
}

/* ChapelRange.chpl:30 */
range_int32_t_bounded_0 _build_range(int32_t low, int32_t high) {
  range_int32_t_bounded_0 T3;
  range_int32_t_bounded_0 T2;
  range_int32_t_bounded_0 T1;
  T1._low = 0;
  T1._high = 0;
  T1._stride = 0;
  T1._low = low;
  T1._high = high;
  T1._stride = 1;
  T2 = _construct_range(BoundedRangeType_bounded, false, low, high, 1);
  T3 = T2;
  return T3;
}

/* ChapelRange.chpl:34 */
range_int64_t_bounded_0 __build_range_25268(int64_t low, int64_t high) {
  range_int64_t_bounded_0 T3;
  range_int64_t_bounded_0 T2;
  range_int64_t_bounded_0 T1;
  T1._low = 0;
  T1._high = 0;
  T1._stride = 0;
  T1._low = low;
  T1._high = high;
  T1._stride = 1;
  T2 = __construct_range_428854(BoundedRangeType_bounded, false, low, high, 1);
  T3 = T2;
  return T3;
}

/* ChapelRange.chpl:43 */
range_int32_t_boundedLow_0 __build_range_561135(BoundedRangeType bt, int32_t bound) {
  range_int32_t_boundedLow_0 T3;
  range_int32_t_boundedLow_0 T2;
  range_int32_t_boundedLow_0 T1;
  T1._low = 0;
  T1._high = 0;
  T1._stride = 0;
  T1._low = bound;
  T1._high = bound;
  T1._stride = 1;
  T2 = __construct_range_561334(BoundedRangeType_boundedLow, false, bound, bound, 1);
  T3 = T2;
  return T3;
}

/* ChapelRange.chpl:58 */
range_int32_t_bounded_0 __build_open_interval_upper_570090(range_int32_t_bounded_0* const r) {
  range_int32_t_bounded_0 T1;
  range_int32_t_bounded_0 T7;
  int32_t T2;
  int32_t T3;
  int32_t T4;
  range_int32_t_bounded_0 T6;
  range_int32_t_bounded_0 T5;
  T1 = _copy(&((*r)));
  T2 = (T1._low);
  T3 = (T1._high);
  T4 = (T3-1);
  T5._low = 0;
  T5._high = 0;
  T5._stride = 0;
  T5._low = T2;
  T5._high = T4;
  T5._stride = 1;
  T6 = _construct_range(BoundedRangeType_bounded, false, T2, T4, 1);
  T7 = T6;
  return T7;
}

/* ChapelRange.chpl:58 */
range_int64_t_bounded_0 _build_open_interval_upper(range_int64_t_bounded_0* const r, int32_t _ln, _string _fn) {
  range_int64_t_bounded_0 T1;
  range_int64_t_bounded_0 T8;
  int64_t T2;
  int64_t T3;
  int64_t T4;
  int64_t T5;
  range_int64_t_bounded_0 T7;
  range_int64_t_bounded_0 T6;
  T1 = __copy_429953(&((*r)));
  T2 = (T1._low);
  T3 = (T1._high);
  T4 = ((int64_t)(1));
  T5 = (T3-T4);
  T6._low = 0;
  T6._high = 0;
  T6._stride = 0;
  T6._low = T2;
  T6._high = T5;
  T6._stride = 1;
  T7 = __construct_range_428854(BoundedRangeType_bounded, false, T2, T5, 1);
  T8 = T7;
  return T8;
}

/* ChapelRange.chpl:65 */
range_int64_t_bounded_1 _by_549212(range_int64_t_bounded_0* const r, int32_t i, int32_t _ln, _string _fn) {
  range_int64_t_bounded_0 T1;
  range_int64_t_bounded_1 T12;
  chpl_bool T2;
  range_int64_t_bounded_1 T8;
  int64_t T3;
  int64_t T4;
  int32_t T5;
  int32_t T6;
  range_int64_t_bounded_1 T7;
  int64_t T9;
  int64_t T10;
  chpl_bool T11;
  int32_t T13;
  chpl_bool T14;
  int64_t T15;
  _ref_range_int64_t_bounded_1 T16 = NULL;
  int64_t T17;
  _ref_range_int64_t_bounded_1 T18 = NULL;
  T1 = __copy_429953(&((*r)));
  T2 = (i==0);
  if (T2) {
    halt("range cannot be strided by zero", _ln, _fn);
  }
  T3 = (T1._low);
  T4 = (T1._high);
  T5 = (T1._stride);
  T6 = (T5*i);
  T7 = __construct_range_546991(BoundedRangeType_bounded, true, T3, T4, T6);
  T8 = __copy_547955(&(T7));
  T9 = (T1._low);
  T10 = (T1._high);
  T11 = (T9>T10);
  if (T11) {
    T12 = T8;
    goto _end_by;
  }
  T13 = (T8._stride);
  T14 = (T13<0);
  if (T14) {
    T15 = (T8._high);
    T16 = &(T8);
    __alignLow_549607(T16, T15, _ln, _fn);
  } else {
    T17 = (T8._low);
    T18 = &(T8);
    __alignHigh_550118(T18, T17, _ln, _fn);
  }
  T12 = T8;
  _end_by:;
  return T12;
}

/* ChapelRange.chpl:65 */
range_int32_t_bounded_1 by(range_int32_t_bounded_0* const r, int32_t i, int32_t _ln, _string _fn) {
  range_int32_t_bounded_0 T1;
  range_int32_t_bounded_1 T12;
  chpl_bool T2;
  range_int32_t_bounded_1 T8;
  int32_t T3;
  int32_t T4;
  int32_t T5;
  int32_t T6;
  range_int32_t_bounded_1 T7;
  int32_t T9;
  int32_t T10;
  chpl_bool T11;
  int32_t T13;
  chpl_bool T14;
  int32_t T15;
  _ref_range_int32_t_bounded_1 T16 = NULL;
  int32_t T17;
  _ref_range_int32_t_bounded_1 T18 = NULL;
  T1 = _copy(&((*r)));
  T2 = (i==0);
  if (T2) {
    halt("range cannot be strided by zero", _ln, _fn);
  }
  T3 = (T1._low);
  T4 = (T1._high);
  T5 = (T1._stride);
  T6 = (T5*i);
  T7 = __construct_range_536229(BoundedRangeType_bounded, true, T3, T4, T6);
  T8 = __copy_536668(&(T7));
  T9 = (T1._low);
  T10 = (T1._high);
  T11 = (T9>T10);
  if (T11) {
    T12 = T8;
    goto _end_by;
  }
  T13 = (T8._stride);
  T14 = (T13<0);
  if (T14) {
    T15 = (T8._high);
    T16 = &(T8);
    _alignLow(T16, T15);
  } else {
    T17 = (T8._low);
    T18 = &(T8);
    _alignHigh(T18, T17);
  }
  T12 = T8;
  _end_by:;
  return T12;
}

/* ChapelRange.chpl:101 */
void __alignLow_549607(_ref_range_int64_t_bounded_1 _this_549612, int64_t alignment, int32_t _ln, _string _fn) {
  int32_t T1;
  int32_t T4;
  chpl_bool T2;
  int32_t T3;
  int64_t T5;
  int64_t T6;
  int64_t T7;
  int64_t T11;
  int64_t T8;
  chpl_bool T9;
  int64_t T10;
  int64_t T12;
  int64_t T13;
  chpl_bool T14;
  int64_t T15;
  int64_t T16;
  int64_t T17;
  chpl_bool T18;
  int64_t T19;
  int64_t T20;
  int64_t T21;
  int64_t T22;
  int64_t T23;
  T1 = ((*_this_549612)._stride);
  T2 = (T1<0);
  if (T2) {
    T3 = (-T1);
    T4 = T3;
  } else {
    T4 = T1;
  }
  T5 = ((int64_t)(T4));
  T6 = ((*_this_549612)._low);
  T7 = (T6-alignment);
  T8 = ((int64_t)(0));
  T9 = (T7<T8);
  if (T9) {
    T10 = (-T7);
    T11 = T10;
  } else {
    T11 = T7;
  }
  T12 = (T11%T5);
  T13 = ((int64_t)(0));
  T14 = (T12!=T13);
  if (T14) {
    T15 = ((*_this_549612)._low);
    T16 = (T15-alignment);
    T17 = ((int64_t)(0));
    T18 = (T16<T17);
    if (T18) {
      T19 = ((*_this_549612)._low);
      T20 = (T19+T12);
      (*_this_549612)._low = T20;
    } else {
      T21 = (T5-T12);
      T22 = ((*_this_549612)._low);
      T23 = (T22+T21);
      (*_this_549612)._low = T23;
    }
  }
  return;
}

/* ChapelRange.chpl:101 */
void __alignLow_667658(_ref_range_int32_t_bounded_0 _this_667663, int32_t alignment) {
  int32_t T1;
  int32_t T4;
  chpl_bool T2;
  int32_t T3;
  int32_t T5;
  int32_t T6;
  int32_t T9;
  chpl_bool T7;
  int32_t T8;
  int32_t T10;
  chpl_bool T11;
  int32_t T12;
  int32_t T13;
  chpl_bool T14;
  int32_t T15;
  int32_t T16;
  int32_t T17;
  int32_t T18;
  int32_t T19;
  T1 = ((*_this_667663)._stride);
  T2 = (T1<0);
  if (T2) {
    T3 = (-T1);
    T4 = T3;
  } else {
    T4 = T1;
  }
  T5 = ((*_this_667663)._low);
  T6 = (T5-alignment);
  T7 = (T6<0);
  if (T7) {
    T8 = (-T6);
    T9 = T8;
  } else {
    T9 = T6;
  }
  T10 = (T9%T4);
  T11 = (T10!=0);
  if (T11) {
    T12 = ((*_this_667663)._low);
    T13 = (T12-alignment);
    T14 = (T13<0);
    if (T14) {
      T15 = ((*_this_667663)._low);
      T16 = (T15+T10);
      (*_this_667663)._low = T16;
    } else {
      T17 = (T4-T10);
      T18 = ((*_this_667663)._low);
      T19 = (T18+T17);
      (*_this_667663)._low = T19;
    }
  }
  return;
}

/* ChapelRange.chpl:101 */
void __alignLow_677946(_ref_range_int64_t_bounded_0 _this_677951, int64_t alignment, int32_t _ln, _string _fn) {
  int32_t T1;
  int32_t T4;
  chpl_bool T2;
  int32_t T3;
  int64_t T5;
  int64_t T6;
  int64_t T7;
  int64_t T11;
  int64_t T8;
  chpl_bool T9;
  int64_t T10;
  int64_t T12;
  int64_t T13;
  chpl_bool T14;
  int64_t T15;
  int64_t T16;
  int64_t T17;
  chpl_bool T18;
  int64_t T19;
  int64_t T20;
  int64_t T21;
  int64_t T22;
  int64_t T23;
  T1 = ((*_this_677951)._stride);
  T2 = (T1<0);
  if (T2) {
    T3 = (-T1);
    T4 = T3;
  } else {
    T4 = T1;
  }
  T5 = ((int64_t)(T4));
  T6 = ((*_this_677951)._low);
  T7 = (T6-alignment);
  T8 = ((int64_t)(0));
  T9 = (T7<T8);
  if (T9) {
    T10 = (-T7);
    T11 = T10;
  } else {
    T11 = T7;
  }
  T12 = (T11%T5);
  T13 = ((int64_t)(0));
  T14 = (T12!=T13);
  if (T14) {
    T15 = ((*_this_677951)._low);
    T16 = (T15-alignment);
    T17 = ((int64_t)(0));
    T18 = (T16<T17);
    if (T18) {
      T19 = ((*_this_677951)._low);
      T20 = (T19+T12);
      (*_this_677951)._low = T20;
    } else {
      T21 = (T5-T12);
      T22 = ((*_this_677951)._low);
      T23 = (T22+T21);
      (*_this_677951)._low = T23;
    }
  }
  return;
}

/* ChapelRange.chpl:101 */
void _alignLow(_ref_range_int32_t_bounded_1 _this_537146, int32_t alignment) {
  int32_t T1;
  int32_t T4;
  chpl_bool T2;
  int32_t T3;
  int32_t T5;
  int32_t T6;
  int32_t T9;
  chpl_bool T7;
  int32_t T8;
  int32_t T10;
  chpl_bool T11;
  int32_t T12;
  int32_t T13;
  chpl_bool T14;
  int32_t T15;
  int32_t T16;
  int32_t T17;
  int32_t T18;
  int32_t T19;
  T1 = ((*_this_537146)._stride);
  T2 = (T1<0);
  if (T2) {
    T3 = (-T1);
    T4 = T3;
  } else {
    T4 = T1;
  }
  T5 = ((*_this_537146)._low);
  T6 = (T5-alignment);
  T7 = (T6<0);
  if (T7) {
    T8 = (-T6);
    T9 = T8;
  } else {
    T9 = T6;
  }
  T10 = (T9%T4);
  T11 = (T10!=0);
  if (T11) {
    T12 = ((*_this_537146)._low);
    T13 = (T12-alignment);
    T14 = (T13<0);
    if (T14) {
      T15 = ((*_this_537146)._low);
      T16 = (T15+T10);
      (*_this_537146)._low = T16;
    } else {
      T17 = (T4-T10);
      T18 = ((*_this_537146)._low);
      T19 = (T18+T17);
      (*_this_537146)._low = T19;
    }
  }
  return;
}

/* ChapelRange.chpl:115 */
void __alignHigh_668116(_ref_range_int32_t_bounded_0 _this_668121, int32_t alignment) {
  int32_t T1;
  int32_t T4;
  chpl_bool T2;
  int32_t T3;
  int32_t T5;
  int32_t T6;
  int32_t T9;
  chpl_bool T7;
  int32_t T8;
  int32_t T10;
  chpl_bool T11;
  int32_t T12;
  int32_t T13;
  chpl_bool T14;
  int32_t T15;
  int32_t T16;
  int32_t T17;
  int32_t T18;
  int32_t T19;
  T1 = ((*_this_668121)._stride);
  T2 = (T1<0);
  if (T2) {
    T3 = (-T1);
    T4 = T3;
  } else {
    T4 = T1;
  }
  T5 = ((*_this_668121)._high);
  T6 = (T5-alignment);
  T7 = (T6<0);
  if (T7) {
    T8 = (-T6);
    T9 = T8;
  } else {
    T9 = T6;
  }
  T10 = (T9%T4);
  T11 = (T10!=0);
  if (T11) {
    T12 = ((*_this_668121)._high);
    T13 = (T12-alignment);
    T14 = (T13>0);
    if (T14) {
      T15 = ((*_this_668121)._high);
      T16 = (T15-T10);
      (*_this_668121)._high = T16;
    } else {
      T17 = (T4-T10);
      T18 = ((*_this_668121)._high);
      T19 = (T18-T17);
      (*_this_668121)._high = T19;
    }
  }
  return;
}

/* ChapelRange.chpl:115 */
void __alignHigh_678404(_ref_range_int64_t_bounded_0 _this_678409, int64_t alignment, int32_t _ln, _string _fn) {
  int32_t T1;
  int32_t T4;
  chpl_bool T2;
  int32_t T3;
  int64_t T5;
  int64_t T6;
  int64_t T7;
  int64_t T11;
  int64_t T8;
  chpl_bool T9;
  int64_t T10;
  int64_t T12;
  int64_t T13;
  chpl_bool T14;
  int64_t T15;
  int64_t T16;
  int64_t T17;
  chpl_bool T18;
  int64_t T19;
  int64_t T20;
  int64_t T21;
  int64_t T22;
  int64_t T23;
  T1 = ((*_this_678409)._stride);
  T2 = (T1<0);
  if (T2) {
    T3 = (-T1);
    T4 = T3;
  } else {
    T4 = T1;
  }
  T5 = ((int64_t)(T4));
  T6 = ((*_this_678409)._high);
  T7 = (T6-alignment);
  T8 = ((int64_t)(0));
  T9 = (T7<T8);
  if (T9) {
    T10 = (-T7);
    T11 = T10;
  } else {
    T11 = T7;
  }
  T12 = (T11%T5);
  T13 = ((int64_t)(0));
  T14 = (T12!=T13);
  if (T14) {
    T15 = ((*_this_678409)._high);
    T16 = (T15-alignment);
    T17 = ((int64_t)(0));
    T18 = (T16>T17);
    if (T18) {
      T19 = ((*_this_678409)._high);
      T20 = (T19-T12);
      (*_this_678409)._high = T20;
    } else {
      T21 = (T5-T12);
      T22 = ((*_this_678409)._high);
      T23 = (T22-T21);
      (*_this_678409)._high = T23;
    }
  }
  return;
}

/* ChapelRange.chpl:115 */
void _alignHigh(_ref_range_int32_t_bounded_1 _this_537684, int32_t alignment) {
  int32_t T1;
  int32_t T4;
  chpl_bool T2;
  int32_t T3;
  int32_t T5;
  int32_t T6;
  int32_t T9;
  chpl_bool T7;
  int32_t T8;
  int32_t T10;
  chpl_bool T11;
  int32_t T12;
  int32_t T13;
  chpl_bool T14;
  int32_t T15;
  int32_t T16;
  int32_t T17;
  int32_t T18;
  int32_t T19;
  T1 = ((*_this_537684)._stride);
  T2 = (T1<0);
  if (T2) {
    T3 = (-T1);
    T4 = T3;
  } else {
    T4 = T1;
  }
  T5 = ((*_this_537684)._high);
  T6 = (T5-alignment);
  T7 = (T6<0);
  if (T7) {
    T8 = (-T6);
    T9 = T8;
  } else {
    T9 = T6;
  }
  T10 = (T9%T4);
  T11 = (T10!=0);
  if (T11) {
    T12 = ((*_this_537684)._high);
    T13 = (T12-alignment);
    T14 = (T13>0);
    if (T14) {
      T15 = ((*_this_537684)._high);
      T16 = (T15-T10);
      (*_this_537684)._high = T16;
    } else {
      T17 = (T4-T10);
      T18 = ((*_this_537684)._high);
      T19 = (T18-T17);
      (*_this_537684)._high = T19;
    }
  }
  return;
}

/* ChapelRange.chpl:115 */
void __alignHigh_550118(_ref_range_int64_t_bounded_1 _this_550123, int64_t alignment, int32_t _ln, _string _fn) {
  int32_t T1;
  int32_t T4;
  chpl_bool T2;
  int32_t T3;
  int64_t T5;
  int64_t T6;
  int64_t T7;
  int64_t T11;
  int64_t T8;
  chpl_bool T9;
  int64_t T10;
  int64_t T12;
  int64_t T13;
  chpl_bool T14;
  int64_t T15;
  int64_t T16;
  int64_t T17;
  chpl_bool T18;
  int64_t T19;
  int64_t T20;
  int64_t T21;
  int64_t T22;
  int64_t T23;
  T1 = ((*_this_550123)._stride);
  T2 = (T1<0);
  if (T2) {
    T3 = (-T1);
    T4 = T3;
  } else {
    T4 = T1;
  }
  T5 = ((int64_t)(T4));
  T6 = ((*_this_550123)._high);
  T7 = (T6-alignment);
  T8 = ((int64_t)(0));
  T9 = (T7<T8);
  if (T9) {
    T10 = (-T7);
    T11 = T10;
  } else {
    T11 = T7;
  }
  T12 = (T11%T5);
  T13 = ((int64_t)(0));
  T14 = (T12!=T13);
  if (T14) {
    T15 = ((*_this_550123)._high);
    T16 = (T15-alignment);
    T17 = ((int64_t)(0));
    T18 = (T16>T17);
    if (T18) {
      T19 = ((*_this_550123)._high);
      T20 = (T19-T12);
      (*_this_550123)._high = T20;
    } else {
      T21 = (T5-T12);
      T22 = ((*_this_550123)._high);
      T23 = (T22-T21);
      (*_this_550123)._high = T23;
    }
  }
  return;
}

/* ChapelRange.chpl:129 */
range_int64_t_bounded_0 __EQUAL__444455(range_int64_t_bounded_0* const r1, range_int64_t_bounded_0* const r2) {
  range_int64_t_bounded_0 T4;
  int64_t T1;
  int64_t T2;
  int32_t T3;
  T1 = ((*r2)._low);
  (*r1)._low = T1;
  T2 = ((*r2)._high);
  (*r1)._high = T2;
  T3 = ((*r2)._stride);
  (*r1)._stride = T3;
  T4 = (*r1);
  return T4;
}

/* ChapelRange.chpl:129 */
range_int64_t_bounded_1 __EQUAL__626708(range_int64_t_bounded_1* const r1, range_int32_t_bounded_1* const r2, int32_t _ln, _string _fn) {
  range_int64_t_bounded_1 T6;
  int32_t T1;
  int64_t T2;
  int32_t T3;
  int64_t T4;
  int32_t T5;
  T1 = ((*r2)._low);
  T2 = ((int64_t)(T1));
  (*r1)._low = T2;
  T3 = ((*r2)._high);
  T4 = ((int64_t)(T3));
  (*r1)._high = T4;
  T5 = ((*r2)._stride);
  (*r1)._stride = T5;
  T6 = (*r1);
  return T6;
}

/* ChapelRange.chpl:129 */
range_int64_t_bounded_1 __EQUAL__550911(range_int64_t_bounded_1* const r1, range_int64_t_bounded_1* const r2) {
  range_int64_t_bounded_1 T4;
  int64_t T1;
  int64_t T2;
  int32_t T3;
  T1 = ((*r2)._low);
  (*r1)._low = T1;
  T2 = ((*r2)._high);
  (*r1)._high = T2;
  T3 = ((*r2)._stride);
  (*r1)._stride = T3;
  T4 = (*r1);
  return T4;
}

/* ChapelRange.chpl:129 */
range_int32_t_bounded_0 _EQUAL_(range_int32_t_bounded_0* const r1, range_int32_t_bounded_0* const r2) {
  range_int32_t_bounded_0 T4;
  int32_t T1;
  int32_t T2;
  int32_t T3;
  T1 = ((*r2)._low);
  (*r1)._low = T1;
  T2 = ((*r2)._high);
  (*r1)._high = T2;
  T3 = ((*r2)._stride);
  (*r1)._stride = T3;
  T4 = (*r1);
  return T4;
}

/* ChapelRange.chpl:146 */
range_int64_t_bounded_1 _this_574915(_ref_range_int64_t_bounded_0 _this_574920, range_int64_t_bounded_1* const other, int32_t _ln, _string _fn) {
  range_int64_t_bounded_1 T1;
  range_int64_t_bounded_1 T56;
  int64_t T2;
  int64_t T3;
  int32_t T4;
  int32_t T7;
  chpl_bool T5;
  int32_t T6;
  int64_t T10;
  int32_t T8;
  chpl_bool T9;
  int64_t T11;
  int64_t T12;
  int32_t T13;
  int32_t T16;
  chpl_bool T14;
  int32_t T15;
  int64_t T19;
  int32_t T17;
  chpl_bool T18;
  _tuple_2_int32_t_int32_t T20;
  int32_t T21;
  int32_t T22;
  range_int64_t_bounded_1 T30;
  int64_t T24;
  chpl_bool T23;
  int64_t T26;
  chpl_bool T25;
  int32_t T27;
  int32_t T28;
  range_int64_t_bounded_1 T29;
  int64_t T31;
  int64_t T35;
  int64_t T32;
  chpl_bool T33;
  int64_t T34;
  int64_t T36;
  int64_t T37;
  int64_t T38;
  chpl_bool T39;
  int64_t T40;
  int64_t T41;
  int32_t T42;
  chpl_bool T43;
  int32_t T44;
  int32_t T45;
  int64_t T46;
  int64_t T47;
  int64_t T48;
  int64_t T49;
  int64_t T50;
  int64_t T51;
  int64_t T52;
  int64_t T53;
  _ref_range_int64_t_bounded_1 T54 = NULL;
  _ref_range_int64_t_bounded_1 T55 = NULL;
  T1 = __copy_547955(&((*other)));
  T2 = ((*_this_574920)._low);
  T3 = ((*_this_574920)._high);
  T4 = ((*_this_574920)._stride);
  T5 = (T4<0);
  if (T5) {
    T6 = (-T4);
    T7 = T6;
  } else {
    T7 = T4;
  }
  T8 = ((*_this_574920)._stride);
  T9 = (T8<0);
  if (T9) {
    T10 = T3;
  } else {
    T10 = T2;
  }
  T11 = (T1._low);
  T12 = (T1._high);
  T13 = (T1._stride);
  T14 = (T13<0);
  if (T14) {
    T15 = (-T13);
    T16 = T15;
  } else {
    T16 = T13;
  }
  T17 = ((*_this_574920)._stride);
  T18 = (T17<0);
  if (T18) {
    T19 = T12;
  } else {
    T19 = T11;
  }
  T20 = extendedEuclid(T7, T16);
  T21 = (T20.x1);
  T22 = (T20.x2);
  T23 = (T2>T11);
  if (T23) {
    T24 = T2;
  } else {
    T24 = T11;
  }
  T25 = (T3<T12);
  if (T25) {
    T26 = T3;
  } else {
    T26 = T12;
  }
  T27 = (T7*T16);
  T28 = (T27/T21);
  T29 = __construct_range_546991(BoundedRangeType_bounded, true, T24, T26, T28);
  T30 = __copy_547955(&(T29));
  T31 = (T2-T11);
  T32 = ((int64_t)(0));
  T33 = (T31<T32);
  if (T33) {
    T34 = (-T31);
    T35 = T34;
  } else {
    T35 = T31;
  }
  T36 = ((int64_t)(T21));
  T37 = (T35%T36);
  T38 = ((int64_t)(0));
  T39 = (T37!=T38);
  if (T39) {
    T40 = (T30._high);
    T41 = (T30._low);
    T30._low = T40;
    T30._high = T41;
  } else {
    T42 = (T1._stride);
    T43 = (T42<0);
    if (T43) {
      T44 = (T30._stride);
      T45 = (-T44);
      T30._stride = T45;
    }
    T46 = (T19-T10);
    T47 = ((int64_t)(T22));
    T48 = (T46*T47);
    T49 = ((int64_t)(T7));
    T50 = (T48*T49);
    T51 = ((int64_t)(T21));
    T52 = (T50/T51);
    T53 = (T10+T52);
    T54 = &(T30);
    __alignLow_549607(T54, T53, _ln, _fn);
    T55 = &(T30);
    __alignHigh_550118(T55, T53, _ln, _fn);
  }
  T56 = T30;
  return T56;
}

/* ChapelRange.chpl:146 */
range_int32_t_bounded_0 _this_665070(_ref_range_int32_t_bounded_0 _this_665075, range_int32_t_bounded_0* const other) {
  range_int32_t_bounded_0 T1;
  range_int32_t_bounded_0 T50;
  int32_t T2;
  int32_t T3;
  int32_t T4;
  int32_t T7;
  chpl_bool T5;
  int32_t T6;
  int32_t T10;
  int32_t T8;
  chpl_bool T9;
  int32_t T11;
  int32_t T12;
  int32_t T13;
  int32_t T16;
  chpl_bool T14;
  int32_t T15;
  int32_t T19;
  int32_t T17;
  chpl_bool T18;
  _tuple_2_int32_t_int32_t T20;
  int32_t T21;
  int32_t T22;
  range_int32_t_bounded_0 T30;
  int32_t T24;
  chpl_bool T23;
  int32_t T26;
  chpl_bool T25;
  int32_t T27;
  int32_t T28;
  range_int32_t_bounded_0 T29;
  int32_t T31;
  int32_t T34;
  chpl_bool T32;
  int32_t T33;
  int32_t T35;
  chpl_bool T36;
  int32_t T37;
  int32_t T38;
  int32_t T39;
  chpl_bool T40;
  int32_t T41;
  int32_t T42;
  int32_t T43;
  int32_t T44;
  int32_t T45;
  int32_t T46;
  int32_t T47;
  _ref_range_int32_t_bounded_0 T48 = NULL;
  _ref_range_int32_t_bounded_0 T49 = NULL;
  T1 = _copy(&((*other)));
  T2 = ((*_this_665075)._low);
  T3 = ((*_this_665075)._high);
  T4 = ((*_this_665075)._stride);
  T5 = (T4<0);
  if (T5) {
    T6 = (-T4);
    T7 = T6;
  } else {
    T7 = T4;
  }
  T8 = ((*_this_665075)._stride);
  T9 = (T8<0);
  if (T9) {
    T10 = T3;
  } else {
    T10 = T2;
  }
  T11 = (T1._low);
  T12 = (T1._high);
  T13 = (T1._stride);
  T14 = (T13<0);
  if (T14) {
    T15 = (-T13);
    T16 = T15;
  } else {
    T16 = T13;
  }
  T17 = ((*_this_665075)._stride);
  T18 = (T17<0);
  if (T18) {
    T19 = T12;
  } else {
    T19 = T11;
  }
  T20 = _extendedEuclid_665444(T7, T16);
  T21 = (T20.x1);
  T22 = (T20.x2);
  T23 = (T2>T11);
  if (T23) {
    T24 = T2;
  } else {
    T24 = T11;
  }
  T25 = (T3<T12);
  if (T25) {
    T26 = T3;
  } else {
    T26 = T12;
  }
  T27 = (T7*T16);
  T28 = (T27/T21);
  T29 = _construct_range(BoundedRangeType_bounded, false, T24, T26, T28);
  T30 = _copy(&(T29));
  T31 = (T2-T11);
  T32 = (T31<0);
  if (T32) {
    T33 = (-T31);
    T34 = T33;
  } else {
    T34 = T31;
  }
  T35 = (T34%T21);
  T36 = (T35!=0);
  if (T36) {
    T37 = (T30._high);
    T38 = (T30._low);
    T30._low = T37;
    T30._high = T38;
  } else {
    T39 = (T1._stride);
    T40 = (T39<0);
    if (T40) {
      T41 = (T30._stride);
      T42 = (-T41);
      T30._stride = T42;
    }
    T43 = (T19-T10);
    T44 = (T43*T22);
    T45 = (T44*T7);
    T46 = (T45/T21);
    T47 = (T10+T46);
    T48 = &(T30);
    __alignLow_667658(T48, T47);
    T49 = &(T30);
    __alignHigh_668116(T49, T47);
  }
  T50 = T30;
  return T50;
}

/* ChapelRange.chpl:146 */
range_int32_t_bounded_1 _this_622878(_ref_range_int64_t_bounded_0 _this_622883, range_int32_t_bounded_1* const other, int32_t _ln, _string _fn) {
  range_int32_t_bounded_1 T1;
  range_int32_t_bounded_1 T52;
  int64_t T2;
  int32_t T3;
  int64_t T4;
  int32_t T5;
  int32_t T6;
  int32_t T9;
  chpl_bool T7;
  int32_t T8;
  int32_t T12;
  int32_t T10;
  chpl_bool T11;
  int32_t T13;
  int32_t T14;
  int32_t T15;
  int32_t T18;
  chpl_bool T16;
  int32_t T17;
  int32_t T21;
  int32_t T19;
  chpl_bool T20;
  _tuple_2_int32_t_int32_t T22;
  int32_t T23;
  int32_t T24;
  range_int32_t_bounded_1 T32;
  int32_t T26;
  chpl_bool T25;
  int32_t T28;
  chpl_bool T27;
  int32_t T29;
  int32_t T30;
  range_int32_t_bounded_1 T31;
  int32_t T33;
  int32_t T36;
  chpl_bool T34;
  int32_t T35;
  int32_t T37;
  chpl_bool T38;
  int32_t T39;
  int32_t T40;
  int32_t T41;
  chpl_bool T42;
  int32_t T43;
  int32_t T44;
  int32_t T45;
  int32_t T46;
  int32_t T47;
  int32_t T48;
  int32_t T49;
  _ref_range_int32_t_bounded_1 T50 = NULL;
  _ref_range_int32_t_bounded_1 T51 = NULL;
  T1 = __copy_536668(&((*other)));
  T2 = ((*_this_622883)._low);
  T3 = ((int32_t)(T2));
  T4 = ((*_this_622883)._high);
  T5 = ((int32_t)(T4));
  T6 = ((*_this_622883)._stride);
  T7 = (T6<0);
  if (T7) {
    T8 = (-T6);
    T9 = T8;
  } else {
    T9 = T6;
  }
  T10 = ((*_this_622883)._stride);
  T11 = (T10<0);
  if (T11) {
    T12 = T5;
  } else {
    T12 = T3;
  }
  T13 = (T1._low);
  T14 = (T1._high);
  T15 = (T1._stride);
  T16 = (T15<0);
  if (T16) {
    T17 = (-T15);
    T18 = T17;
  } else {
    T18 = T15;
  }
  T19 = ((*_this_622883)._stride);
  T20 = (T19<0);
  if (T20) {
    T21 = T14;
  } else {
    T21 = T13;
  }
  T22 = _extendedEuclid_623252(T9, T18);
  T23 = (T22.x1);
  T24 = (T22.x2);
  T25 = (T3>T13);
  if (T25) {
    T26 = T3;
  } else {
    T26 = T13;
  }
  T27 = (T5<T14);
  if (T27) {
    T28 = T5;
  } else {
    T28 = T14;
  }
  T29 = (T9*T18);
  T30 = (T29/T23);
  T31 = __construct_range_536229(BoundedRangeType_bounded, true, T26, T28, T30);
  T32 = __copy_536668(&(T31));
  T33 = (T3-T13);
  T34 = (T33<0);
  if (T34) {
    T35 = (-T33);
    T36 = T35;
  } else {
    T36 = T33;
  }
  T37 = (T36%T23);
  T38 = (T37!=0);
  if (T38) {
    T39 = (T32._high);
    T40 = (T32._low);
    T32._low = T39;
    T32._high = T40;
  } else {
    T41 = (T1._stride);
    T42 = (T41<0);
    if (T42) {
      T43 = (T32._stride);
      T44 = (-T43);
      T32._stride = T44;
    }
    T45 = (T21-T12);
    T46 = (T45*T24);
    T47 = (T46*T9);
    T48 = (T47/T23);
    T49 = (T12+T48);
    T50 = &(T32);
    _alignLow(T50, T49);
    T51 = &(T32);
    _alignHigh(T51, T49);
  }
  T52 = T32;
  return T52;
}

/* ChapelRange.chpl:146 */
range_int64_t_bounded_0 _this_675650(_ref_range_int64_t_bounded_0 _this_675655, range_int64_t_bounded_0* const other, int32_t _ln, _string _fn) {
  range_int64_t_bounded_0 T1;
  range_int64_t_bounded_0 T56;
  int64_t T2;
  int64_t T3;
  int32_t T4;
  int32_t T7;
  chpl_bool T5;
  int32_t T6;
  int64_t T10;
  int32_t T8;
  chpl_bool T9;
  int64_t T11;
  int64_t T12;
  int32_t T13;
  int32_t T16;
  chpl_bool T14;
  int32_t T15;
  int64_t T19;
  int32_t T17;
  chpl_bool T18;
  _tuple_2_int32_t_int32_t T20;
  int32_t T21;
  int32_t T22;
  range_int64_t_bounded_0 T30;
  int64_t T24;
  chpl_bool T23;
  int64_t T26;
  chpl_bool T25;
  int32_t T27;
  int32_t T28;
  range_int64_t_bounded_0 T29;
  int64_t T31;
  int64_t T35;
  int64_t T32;
  chpl_bool T33;
  int64_t T34;
  int64_t T36;
  int64_t T37;
  int64_t T38;
  chpl_bool T39;
  int64_t T40;
  int64_t T41;
  int32_t T42;
  chpl_bool T43;
  int32_t T44;
  int32_t T45;
  int64_t T46;
  int64_t T47;
  int64_t T48;
  int64_t T49;
  int64_t T50;
  int64_t T51;
  int64_t T52;
  int64_t T53;
  _ref_range_int64_t_bounded_0 T54 = NULL;
  _ref_range_int64_t_bounded_0 T55 = NULL;
  T1 = __copy_429953(&((*other)));
  T2 = ((*_this_675655)._low);
  T3 = ((*_this_675655)._high);
  T4 = ((*_this_675655)._stride);
  T5 = (T4<0);
  if (T5) {
    T6 = (-T4);
    T7 = T6;
  } else {
    T7 = T4;
  }
  T8 = ((*_this_675655)._stride);
  T9 = (T8<0);
  if (T9) {
    T10 = T3;
  } else {
    T10 = T2;
  }
  T11 = (T1._low);
  T12 = (T1._high);
  T13 = (T1._stride);
  T14 = (T13<0);
  if (T14) {
    T15 = (-T13);
    T16 = T15;
  } else {
    T16 = T13;
  }
  T17 = ((*_this_675655)._stride);
  T18 = (T17<0);
  if (T18) {
    T19 = T12;
  } else {
    T19 = T11;
  }
  T20 = _extendedEuclid_676024(T7, T16);
  T21 = (T20.x1);
  T22 = (T20.x2);
  T23 = (T2>T11);
  if (T23) {
    T24 = T2;
  } else {
    T24 = T11;
  }
  T25 = (T3<T12);
  if (T25) {
    T26 = T3;
  } else {
    T26 = T12;
  }
  T27 = (T7*T16);
  T28 = (T27/T21);
  T29 = __construct_range_428854(BoundedRangeType_bounded, false, T24, T26, T28);
  T30 = __copy_429953(&(T29));
  T31 = (T2-T11);
  T32 = ((int64_t)(0));
  T33 = (T31<T32);
  if (T33) {
    T34 = (-T31);
    T35 = T34;
  } else {
    T35 = T31;
  }
  T36 = ((int64_t)(T21));
  T37 = (T35%T36);
  T38 = ((int64_t)(0));
  T39 = (T37!=T38);
  if (T39) {
    T40 = (T30._high);
    T41 = (T30._low);
    T30._low = T40;
    T30._high = T41;
  } else {
    T42 = (T1._stride);
    T43 = (T42<0);
    if (T43) {
      T44 = (T30._stride);
      T45 = (-T44);
      T30._stride = T45;
    }
    T46 = (T19-T10);
    T47 = ((int64_t)(T22));
    T48 = (T46*T47);
    T49 = ((int64_t)(T7));
    T50 = (T48*T49);
    T51 = ((int64_t)(T21));
    T52 = (T50/T51);
    T53 = (T10+T52);
    T54 = &(T30);
    __alignLow_677946(T54, T53, _ln, _fn);
    T55 = &(T30);
    __alignHigh_678404(T55, T53, _ln, _fn);
  }
  T56 = T30;
  return T56;
}

/* ChapelRange.chpl:170 */
_tuple_2_int32_t_int32_t _extendedEuclid_676024(int32_t u, int32_t v) {
  _tuple_2_int32_t_int32_t T15;
  int32_t T1;
  int32_t T2;
  int32_t T3;
  int32_t T4;
  chpl_bool T6;
  chpl_bool T5;
  int32_t T12;
  int32_t T13;
  int32_t T7;
  int32_t T8;
  int32_t T9;
  int32_t T10;
  int32_t T11;
  chpl_bool T14;
  T1 = 1;
  T2 = u;
  T3 = 0;
  T4 = v;
  T5 = (v!=0);
  T6 = T5;
  while (T6) {
    T7 = (T2/T4);
    T8 = (T4*T7);
    T9 = (T3*T7);
    T10 = (T2-T8);
    T11 = (T1-T9);
    T12 = T3;
    T13 = T4;
    T3 = T11;
    T4 = T10;
    T1 = T12;
    T2 = T13;
    T14 = (T10!=0);
    T6 = T14;
  }
  T15.x1 = T2;
  T15.x2 = T1;
  return T15;
}

/* ChapelRange.chpl:170 */
_tuple_2_int32_t_int32_t extendedEuclid(int32_t u, int32_t v) {
  _tuple_2_int32_t_int32_t T15;
  int32_t T1;
  int32_t T2;
  int32_t T3;
  int32_t T4;
  chpl_bool T6;
  chpl_bool T5;
  int32_t T12;
  int32_t T13;
  int32_t T7;
  int32_t T8;
  int32_t T9;
  int32_t T10;
  int32_t T11;
  chpl_bool T14;
  T1 = 1;
  T2 = u;
  T3 = 0;
  T4 = v;
  T5 = (v!=0);
  T6 = T5;
  while (T6) {
    T7 = (T2/T4);
    T8 = (T4*T7);
    T9 = (T3*T7);
    T10 = (T2-T8);
    T11 = (T1-T9);
    T12 = T3;
    T13 = T4;
    T3 = T11;
    T4 = T10;
    T1 = T12;
    T2 = T13;
    T14 = (T10!=0);
    T6 = T14;
  }
  T15.x1 = T2;
  T15.x2 = T1;
  return T15;
}

/* ChapelRange.chpl:170 */
_tuple_2_int32_t_int32_t _extendedEuclid_623252(int32_t u, int32_t v) {
  _tuple_2_int32_t_int32_t T15;
  int32_t T1;
  int32_t T2;
  int32_t T3;
  int32_t T4;
  chpl_bool T6;
  chpl_bool T5;
  int32_t T12;
  int32_t T13;
  int32_t T7;
  int32_t T8;
  int32_t T9;
  int32_t T10;
  int32_t T11;
  chpl_bool T14;
  T1 = 1;
  T2 = u;
  T3 = 0;
  T4 = v;
  T5 = (v!=0);
  T6 = T5;
  while (T6) {
    T7 = (T2/T4);
    T8 = (T4*T7);
    T9 = (T3*T7);
    T10 = (T2-T8);
    T11 = (T1-T9);
    T12 = T3;
    T13 = T4;
    T3 = T11;
    T4 = T10;
    T1 = T12;
    T2 = T13;
    T14 = (T10!=0);
    T6 = T14;
  }
  T15.x1 = T2;
  T15.x2 = T1;
  return T15;
}

/* ChapelRange.chpl:170 */
_tuple_2_int32_t_int32_t _extendedEuclid_665444(int32_t u, int32_t v) {
  _tuple_2_int32_t_int32_t T15;
  int32_t T1;
  int32_t T2;
  int32_t T3;
  int32_t T4;
  chpl_bool T6;
  chpl_bool T5;
  int32_t T12;
  int32_t T13;
  int32_t T7;
  int32_t T8;
  int32_t T9;
  int32_t T10;
  int32_t T11;
  chpl_bool T14;
  T1 = 1;
  T2 = u;
  T3 = 0;
  T4 = v;
  T5 = (v!=0);
  T6 = T5;
  while (T6) {
    T7 = (T2/T4);
    T8 = (T4*T7);
    T9 = (T3*T7);
    T10 = (T2-T8);
    T11 = (T1-T9);
    T12 = T3;
    T13 = T4;
    T3 = T11;
    T4 = T10;
    T1 = T12;
    T2 = T13;
    T14 = (T10!=0);
    T6 = T14;
  }
  T15.x1 = T2;
  T15.x2 = T1;
  return T15;
}

/* ChapelRange.chpl:263 */
int64_t _length_459250(_ref_range_int64_t_bounded_0 _this_459255, int32_t _ln, _string _fn) {
  int64_t T24;
  int64_t T11;
  int32_t T1;
  chpl_bool T2;
  int64_t T3;
  int64_t T4;
  int64_t T5;
  int32_t T6;
  int64_t T7;
  int64_t T8;
  int64_t T9;
  int64_t T10;
  int64_t T12;
  int64_t T13;
  int64_t T14;
  int32_t T15;
  int64_t T16;
  int64_t T17;
  int64_t T18;
  int64_t T19;
  int64_t T23;
  int64_t T20;
  chpl_bool T21;
  int64_t T22;
  T1 = ((*_this_459255)._stride);
  T2 = (T1>0);
  if (T2) {
    T3 = ((*_this_459255)._high);
    T4 = ((*_this_459255)._low);
    T5 = (T3-T4);
    T6 = ((*_this_459255)._stride);
    T7 = ((int64_t)(T6));
    T8 = (T5/T7);
    T9 = ((int64_t)(1));
    T10 = (T8+T9);
    T11 = T10;
  } else {
    T12 = ((*_this_459255)._low);
    T13 = ((*_this_459255)._high);
    T14 = (T12-T13);
    T15 = ((*_this_459255)._stride);
    T16 = ((int64_t)(T15));
    T17 = (T14/T16);
    T18 = ((int64_t)(1));
    T19 = (T17+T18);
    T11 = T19;
  }
  T20 = ((int64_t)(0));
  T21 = (T11<T20);
  if (T21) {
    T22 = ((int64_t)(0));
    T23 = T22;
  } else {
    T23 = T11;
  }
  T24 = T23;
  return T24;
}

/* ChapelRange.chpl:263 */
int64_t _length_588976(_ref_range_int64_t_bounded_1 _this_588981, int32_t _ln, _string _fn) {
  int64_t T24;
  int64_t T11;
  int32_t T1;
  chpl_bool T2;
  int64_t T3;
  int64_t T4;
  int64_t T5;
  int32_t T6;
  int64_t T7;
  int64_t T8;
  int64_t T9;
  int64_t T10;
  int64_t T12;
  int64_t T13;
  int64_t T14;
  int32_t T15;
  int64_t T16;
  int64_t T17;
  int64_t T18;
  int64_t T19;
  int64_t T23;
  int64_t T20;
  chpl_bool T21;
  int64_t T22;
  T1 = ((*_this_588981)._stride);
  T2 = (T1>0);
  if (T2) {
    T3 = ((*_this_588981)._high);
    T4 = ((*_this_588981)._low);
    T5 = (T3-T4);
    T6 = ((*_this_588981)._stride);
    T7 = ((int64_t)(T6));
    T8 = (T5/T7);
    T9 = ((int64_t)(1));
    T10 = (T8+T9);
    T11 = T10;
  } else {
    T12 = ((*_this_588981)._low);
    T13 = ((*_this_588981)._high);
    T14 = (T12-T13);
    T15 = ((*_this_588981)._stride);
    T16 = ((int64_t)(T15));
    T17 = (T14/T16);
    T18 = ((int64_t)(1));
    T19 = (T17+T18);
    T11 = T19;
  }
  T20 = ((int64_t)(0));
  T21 = (T11<T20);
  if (T21) {
    T22 = ((int64_t)(0));
    T23 = T22;
  } else {
    T23 = T11;
  }
  T24 = T23;
  return T24;
}

/* ChapelRange.chpl:263 */
int32_t length(_ref_range_int32_t_bounded_0 _this_293860) {
  int32_t T18;
  int32_t T9;
  int32_t T1;
  chpl_bool T2;
  int32_t T3;
  int32_t T4;
  int32_t T5;
  int32_t T6;
  int32_t T7;
  int32_t T8;
  int32_t T10;
  int32_t T11;
  int32_t T12;
  int32_t T13;
  int32_t T14;
  int32_t T15;
  int32_t T17;
  chpl_bool T16;
  T1 = ((*_this_293860)._stride);
  T2 = (T1>0);
  if (T2) {
    T3 = ((*_this_293860)._high);
    T4 = ((*_this_293860)._low);
    T5 = (T3-T4);
    T6 = ((*_this_293860)._stride);
    T7 = (T5/T6);
    T8 = (T7+1);
    T9 = T8;
  } else {
    T10 = ((*_this_293860)._low);
    T11 = ((*_this_293860)._high);
    T12 = (T10-T11);
    T13 = ((*_this_293860)._stride);
    T14 = (T12/T13);
    T15 = (T14+1);
    T9 = T15;
  }
  T16 = (T9<0);
  if (T16) {
    T17 = 0;
  } else {
    T17 = T9;
  }
  T18 = T17;
  return T18;
}

/* ChapelRange.chpl:391 */
range_int64_t_bounded_1 __PLUS__570611(range_int32_t_bounded_1* const r, int64_t i, int32_t _ln, _string _fn) {
  range_int32_t_bounded_1 T1;
  range_int64_t_bounded_1 T10;
  int32_t T2;
  int64_t T3;
  int64_t T4;
  int32_t T5;
  int64_t T6;
  int64_t T7;
  int32_t T8;
  range_int64_t_bounded_1 T9;
  T1 = __copy_536668(&((*r)));
  T2 = (T1._low);
  T3 = ((int64_t)(T2));
  T4 = (T3+i);
  T5 = (T1._high);
  T6 = ((int64_t)(T5));
  T7 = (T6+i);
  T8 = (T1._stride);
  T9 = __construct_range_546991(BoundedRangeType_bounded, true, T4, T7, T8);
  T10 = T9;
  return T10;
}

/* ChapelRange.chpl:391 */
range_int32_t_bounded_1 __PLUS__619001(range_int32_t_bounded_1* const r, int32_t i) {
  range_int32_t_bounded_1 T1;
  range_int32_t_bounded_1 T8;
  int32_t T2;
  int32_t T3;
  int32_t T4;
  int32_t T5;
  int32_t T6;
  range_int32_t_bounded_1 T7;
  T1 = __copy_536668(&((*r)));
  T2 = (T1._low);
  T3 = (T2+i);
  T4 = (T1._high);
  T5 = (T4+i);
  T6 = (T1._stride);
  T7 = __construct_range_536229(BoundedRangeType_bounded, true, T3, T5, T6);
  T8 = T7;
  return T8;
}

/* ChapelRange.chpl:397 */
range_int32_t_bounded_1 _ASTERISK_(range_int32_t_bounded_0* const r, int32_t i, int32_t _ln, _string _fn) {
  range_int32_t_bounded_0 T1;
  range_int32_t_bounded_1 T10;
  chpl_bool T2;
  int32_t T3;
  int32_t T4;
  int32_t T5;
  int32_t T6;
  int32_t T7;
  int32_t T8;
  range_int32_t_bounded_1 T9;
  T1 = _copy(&((*r)));
  T2 = (i==0);
  if (T2) {
    halt("multiplication of range by zero", _ln, _fn);
  }
  T3 = (T1._low);
  T4 = (T3*i);
  T5 = (T1._high);
  T6 = (T5*i);
  T7 = (T1._stride);
  T8 = (T7*i);
  T9 = __construct_range_536229(BoundedRangeType_bounded, true, T4, T6, T8);
  T10 = T9;
  return T10;
}

/* ChapelRange.chpl:406 */
range_int64_t_bounded_0 _PLUS_(int64_t i, range_int32_t_bounded_0* const r, int32_t _ln, _string _fn) {
  range_int32_t_bounded_0 T1;
  range_int64_t_bounded_0 T10;
  int32_t T2;
  int64_t T3;
  int64_t T4;
  int32_t T5;
  int64_t T6;
  int64_t T7;
  int32_t T8;
  range_int64_t_bounded_0 T9;
  T1 = _copy(&((*r)));
  T2 = (T1._low);
  T3 = ((int64_t)(T2));
  T4 = (i+T3);
  T5 = (T1._high);
  T6 = ((int64_t)(T5));
  T7 = (i+T6);
  T8 = (T1._stride);
  T9 = __construct_range_428854(BoundedRangeType_bounded, false, T4, T7, T8);
  T10 = T9;
  return T10;
}

