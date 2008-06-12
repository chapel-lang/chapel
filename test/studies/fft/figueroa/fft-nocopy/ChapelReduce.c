/* ChapelReduce.chpl:1 */
void __init_ChapelReduce(void) {
  chpl_bool T1;
  T1 = (!__run_ChapelReduce_firsttime5);
  if (T1) {
    goto _end___init_ChapelReduce;
  }
  __run_ChapelReduce_firsttime5 = false;
  if (__run_List_firsttime16) {
    __init_List();
  }
  __run_List_firsttime16 = false;
  _end___init_ChapelReduce:;
  return;
}

/* ChapelReduce.chpl:2 */
int64_t __reduce_396373(_min_int64_t r, _ic_physicalMemory__ref__array_int32_t_locale_1_SingleLocaleArithmeticArray_locale_int32_t_0_1_0 s, int32_t _ln, _string _fn) {
  _min_int64_t T1 = NULL;
  int64_t T26;
  _array_int32_t_locale_1_SingleLocaleArithmeticArray_locale_int32_t_0_1_0 T2;
  MemUnits T3;
  MemUnits T4;
  SingleLocaleArithmeticArray_locale_int32_t_0_1_0 T5 = NULL;
  SingleLocaleArithmeticArray_locale_int32_t_0_1_0 T6 = NULL;
  SingleLocaleArithmeticDomain_1_int32_t_0 T7 = NULL;
  range_int32_t_bounded_0 T9;
  _tuple_1_range_int32_t_bounded_0 T8;
  range_int32_t_bounded_0 T10;
  int32_t T11;
  int32_t T13;
  int32_t T12;
  int32_t T15;
  int32_t T14;
  _ref_int32_t T16 = NULL;
  int32_t T17;
  int32_t T18;
  int32_t T19;
  int32_t T20;
  _ddata_locale T21 = NULL;
  _ref_locale T22 = NULL;
  locale T23 = NULL;
  int64_t T24;
  int64_t T25;
  T1 = r;
  T2 = (s->_0_this);
  T3 = (s->_1_unit);
  T4 = T3;
  T5 = (T2._value);
  T6 = T5;
  T7 = (T5->dom);
  T8 = (T7->ranges);
  T9 = (T8.x1);
  T10 = _copy(&(T9));
  T11 = 0;
  T12 = (T10._low);
  T13 = T12;
  T14 = (T10._high);
  T15 = T14;
  for (T11 = T13; T11 <= T15; T11 += 1) {
    T16 = &(T11);
    T17 = (T6->origin);
    T18 = (T17+T11);
    T19 = (T6->factoredOffs);
    T20 = (T18-T19);
    T21 = (T6->data);
    T22 = _ARRAY_GET(T21, T20);
    T23 = *(T22);
    T24 = physicalMemory(T23, T4, _ln, _fn);
    _accumulate_396519(T1, T24);
  }
  T25 = _generate_397007(T1);
  T26 = T25;
  return T26;
}

/* ChapelReduce.chpl:2 */
int64_t _reduce(_sum_int64_t r, _ic_physicalMemory__ref__array_int32_t_locale_1_SingleLocaleArithmeticArray_locale_int32_t_0_1_0 s, int32_t _ln, _string _fn) {
  _sum_int64_t T1 = NULL;
  int64_t T26;
  _array_int32_t_locale_1_SingleLocaleArithmeticArray_locale_int32_t_0_1_0 T2;
  MemUnits T3;
  MemUnits T4;
  SingleLocaleArithmeticArray_locale_int32_t_0_1_0 T5 = NULL;
  SingleLocaleArithmeticArray_locale_int32_t_0_1_0 T6 = NULL;
  SingleLocaleArithmeticDomain_1_int32_t_0 T7 = NULL;
  range_int32_t_bounded_0 T9;
  _tuple_1_range_int32_t_bounded_0 T8;
  range_int32_t_bounded_0 T10;
  int32_t T11;
  int32_t T13;
  int32_t T12;
  int32_t T15;
  int32_t T14;
  _ref_int32_t T16 = NULL;
  int32_t T17;
  int32_t T18;
  int32_t T19;
  int32_t T20;
  _ddata_locale T21 = NULL;
  _ref_locale T22 = NULL;
  locale T23 = NULL;
  int64_t T24;
  int64_t T25;
  T1 = r;
  T2 = (s->_0_this);
  T3 = (s->_1_unit);
  T4 = T3;
  T5 = (T2._value);
  T6 = T5;
  T7 = (T5->dom);
  T8 = (T7->ranges);
  T9 = (T8.x1);
  T10 = _copy(&(T9));
  T11 = 0;
  T12 = (T10._low);
  T13 = T12;
  T14 = (T10._high);
  T15 = T14;
  for (T11 = T13; T11 <= T15; T11 += 1) {
    T16 = &(T11);
    T17 = (T6->origin);
    T18 = (T17+T11);
    T19 = (T6->factoredOffs);
    T20 = (T18-T19);
    T21 = (T6->data);
    T22 = _ARRAY_GET(T21, T20);
    T23 = *(T22);
    T24 = physicalMemory(T23, T4, _ln, _fn);
    accumulate(T1, T24);
  }
  T25 = generate(T1);
  T26 = T25;
  return T26;
}

/* ChapelReduce.chpl:2 */
_real64 __reduce_648116(_max__real64 r, _ic_sqrt s, int32_t _ln, _string _fn) {
  _max__real64 T1 = NULL;
  _real64 T109;
  _ic__PLUS_ T2 = NULL;
  _ic__ASTERISK__ASTERISK_ T3 = NULL;
  __ic__ASTERISK__ASTERISK__642456 T4 = NULL;
  _ic_im__ref__array_int64_t__complex128_1_SingleLocaleArithmeticArray__complex128_int64_t_0_1_0 T31 = NULL;
  SingleLocaleArithmeticArray__complex128_int64_t_0_1_0 T24 = NULL;
  int64_t T30;
  __ic__HYPHEN__641876 T5 = NULL;
  _ic_im__ref__array_int64_t__complex128_1_SingleLocaleArithmeticArray__complex128_int64_t_0_1_0 T6 = NULL;
  _ic_im__ref__array_int64_t__complex128_1_SingleLocaleArithmeticArray__complex128_int64_t_0_1_0 T7 = NULL;
  _ic_im__ref__array_int64_t__complex128_1_SingleLocaleArithmeticArray__complex128_int64_t_0_1_0 T8 = NULL;
  _array_int64_t__complex128_1_SingleLocaleArithmeticArray__complex128_int64_t_0_1_0 T9;
  SingleLocaleArithmeticArray__complex128_int64_t_0_1_0 T10 = NULL;
  int64_t T11;
  int64_t T12;
  int64_t T13;
  _array_int64_t__complex128_1_SingleLocaleArithmeticArray__complex128_int64_t_0_1_0 T14;
  SingleLocaleArithmeticArray__complex128_int64_t_0_1_0 T15 = NULL;
  SingleLocaleArithmeticDomain_1_int64_t_0 T16 = NULL;
  _tuple_1_range_int64_t_bounded_0 T17;
  range_int64_t_bounded_0 T18;
  range_int64_t_bounded_0 T19;
  int64_t T20;
  int64_t T21;
  _array_int64_t__complex128_1_SingleLocaleArithmeticArray__complex128_int64_t_0_1_0 T22;
  SingleLocaleArithmeticArray__complex128_int64_t_0_1_0 T23 = NULL;
  SingleLocaleArithmeticDomain_1_int64_t_0 T25 = NULL;
  _tuple_1_range_int64_t_bounded_0 T26;
  range_int64_t_bounded_0 T27;
  range_int64_t_bounded_0 T28;
  int64_t T29;
  _ic__HYPHEN_ T32 = NULL;
  _ic_re__ref__array_int64_t__complex128_1_SingleLocaleArithmeticArray__complex128_int64_t_0_1_0 T33 = NULL;
  _ic_re__ref__array_int64_t__complex128_1_SingleLocaleArithmeticArray__complex128_int64_t_0_1_0 T34 = NULL;
  SingleLocaleArithmeticArray__complex128_int64_t_0_1_0 T37 = NULL;
  int64_t T43;
  _array_int64_t__complex128_1_SingleLocaleArithmeticArray__complex128_int64_t_0_1_0 T35;
  SingleLocaleArithmeticArray__complex128_int64_t_0_1_0 T36 = NULL;
  SingleLocaleArithmeticDomain_1_int64_t_0 T38 = NULL;
  _tuple_1_range_int64_t_bounded_0 T39;
  range_int64_t_bounded_0 T40;
  range_int64_t_bounded_0 T41;
  int64_t T42;
  _array_int64_t__complex128_1_SingleLocaleArithmeticArray__complex128_int64_t_0_1_0 T44;
  SingleLocaleArithmeticArray__complex128_int64_t_0_1_0 T45 = NULL;
  SingleLocaleArithmeticArray__complex128_int64_t_0_1_0 T46 = NULL;
  SingleLocaleArithmeticDomain_1_int64_t_0 T47 = NULL;
  range_int64_t_bounded_0 T49;
  _tuple_1_range_int64_t_bounded_0 T48;
  range_int64_t_bounded_0 T50;
  int64_t T51;
  int64_t T53;
  int64_t T52;
  int64_t T55;
  int64_t T54;
  _ref_int64_t T56 = NULL;
  int64_t T57;
  int64_t T58;
  int64_t T59;
  int64_t T60;
  int32_t T61;
  _ddata__complex128 T62 = NULL;
  _ref__complex128 T63 = NULL;
  _ref_int64_t T65 = NULL;
  int64_t T64;
  _ref__real64 T73 = NULL;
  int64_t T66;
  int64_t T67;
  int64_t T68;
  int64_t T69;
  int32_t T70;
  _ddata__complex128 T71 = NULL;
  _ref__complex128 T72 = NULL;
  _real64 T74;
  _real64 T75;
  _real64 T76;
  _real64 T77;
  _ref_int64_t T79 = NULL;
  int64_t T78;
  int64_t T80;
  int64_t T81;
  int64_t T82;
  int64_t T83;
  int32_t T84;
  _ddata__complex128 T85 = NULL;
  _ref__complex128 T86 = NULL;
  _ref_int64_t T90 = NULL;
  int64_t T87;
  SingleLocaleArithmeticArray__complex128_int64_t_0_1_0 T88 = NULL;
  _ref__real64 T89 = NULL;
  int64_t T91;
  int64_t T92;
  int64_t T93;
  int64_t T94;
  int32_t T95;
  _ddata__complex128 T96 = NULL;
  _ref__complex128 T97 = NULL;
  _ref__real64 T98 = NULL;
  _real64 T99;
  _real64 T100;
  _real64 T101;
  _real64 T102;
  _real64 T103;
  _real64 T104;
  int64_t T106;
  int64_t T105;
  int64_t T107;
  _real64 T108;
  T1 = r;
  T2 = (s->_0_x);
  T3 = (T2->_0_a);
  T4 = (T2->_1_b);
  T5 = (T4->_0_a);
  T6 = (T5->_0_a);
  T7 = (T5->_1_b);
  T8 = (_ic_im__ref__array_int64_t__complex128_1_SingleLocaleArithmeticArray__complex128_int64_t_0_1_0)chpl_alloc(sizeof(__ic_im__ref__array_int64_t__complex128_1_SingleLocaleArithmeticArray__complex128_int64_t_0_1_0), "iterator class copy", _ln, _fn);
  T9 = (T7->_0_this);
  T8->_0_this = T9;
  T10 = (T7->_2_this);
  T8->_2_this = T10;
  T11 = (T7->_3_i);
  T8->_3_i = T11;
  T12 = (T7->_4__tmp);
  T8->_4__tmp = T12;
  T13 = (T7->_5__tmp);
  T8->_5__tmp = T13;
  T14 = (T8->_0_this);
  T15 = (T14._value);
  T8->_2_this = T15;
  T16 = (T15->dom);
  T17 = (T16->ranges);
  T18 = (T17.x1);
  T19 = __copy_429953(&(T18));
  T8->_3_i = 0;
  T20 = (T19._low);
  T8->_4__tmp = T20;
  T21 = (T19._high);
  T8->_5__tmp = T21;
  T8->_3_i = T20;
  T22 = (T6->_0_this);
  T23 = (T22._value);
  T24 = T23;
  T25 = (T23->dom);
  T26 = (T25->ranges);
  T27 = (T26.x1);
  T28 = __copy_429953(&(T27));
  T29 = (T28._low);
  T30 = T29;
  T31 = T8;
  T32 = (T3->_0_a);
  T33 = (T32->_0_a);
  T34 = (T32->_1_b);
  T35 = (T34->_0_this);
  T36 = (T35._value);
  T37 = T36;
  T38 = (T36->dom);
  T39 = (T38->ranges);
  T40 = (T39.x1);
  T41 = __copy_429953(&(T40));
  T42 = (T41._low);
  T43 = T42;
  T44 = (T33->_0_this);
  T45 = (T44._value);
  T46 = T45;
  T47 = (T45->dom);
  T48 = (T47->ranges);
  T49 = (T48.x1);
  T50 = __copy_429953(&(T49));
  T51 = 0;
  T52 = (T50._low);
  T53 = T52;
  T54 = (T50._high);
  T55 = T54;
  for (T51 = T53; T51 <= T55; T51 += 1) {
    T56 = &(T51);
    T57 = (T46->origin);
    T58 = (T57+T51);
    T59 = (T46->factoredOffs);
    T60 = (T58-T59);
    T61 = ((int32_t)(T60));
    T62 = (T46->data);
    T63 = _ARRAY_GET(T62, T61);
    T64 = T43;
    T65 = &(T64);
    T66 = (T37->origin);
    T67 = (T66+T64);
    T68 = (T37->factoredOffs);
    T69 = (T67-T68);
    T70 = ((int32_t)(T69));
    T71 = (T37->data);
    T72 = _ARRAY_GET(T71, T70);
    T73 = (&((*T72).re));
    T74 = ((*T63).re);
    T75 = *(T73);
    T76 = (T74-T75);
    T77 = (T76*T76);
    T78 = T30;
    T79 = &(T78);
    T80 = (T24->origin);
    T81 = (T80+T78);
    T82 = (T24->factoredOffs);
    T83 = (T81-T82);
    T84 = ((int32_t)(T83));
    T85 = (T24->data);
    T86 = _ARRAY_GET(T85, T84);
    T87 = (T31->_3_i);
    T88 = (T31->_2_this);
    T89 = (T31->_1__ret_im);
    T90 = &(T87);
    T31->_3_i = T87;
    T31->_3_i = T87;
    T91 = (T88->origin);
    T92 = (T91+T87);
    T93 = (T88->factoredOffs);
    T94 = (T92-T93);
    T95 = ((int32_t)(T94));
    T96 = (T88->data);
    T97 = _ARRAY_GET(T96, T95);
    T89 = (&((*T97).im));
    T31->_1__ret_im = T89;
    T98 = (T31->_1__ret_im);
    T99 = ((*T86).im);
    T100 = *(T98);
    T101 = (T99-T100);
    T102 = (T101*T101);
    T103 = (T77+T102);
    T104 = sqrt(T103);
    _accumulate_648307(T1, T104);
    T105 = (T31->_3_i);
    T105 = (T105+1);
    T31->_3_i = T105;
    T106 = (T78+1);
    T30 = T106;
    T107 = (T64+1);
    T43 = T107;
  }
  T108 = _generate_649547(T1);
  T109 = T108;
  return T109;
}

/* ChapelReduce.chpl:18 */
int64_t _sum_type(void) {
  int64_t T2;
  int64_t T1;
  T1 = (0+0);
  T2 = T1;
  return T2;
}

/* ChapelReduce.chpl:23 */
_sum_int64_t _construct__sum(int64_t value, _sum_int64_t meme, int32_t _ln, _string _fn) {
  _sum_int64_t T2 = NULL;
  chpl_bool T1;
  object T3 = NULL;
  T1 = (nil==meme);
  if (T1) {
    T2 = (_sum_int64_t)chpl_alloc(sizeof(__sum_int64_t), "instance of class _sum", _ln, _fn);
    ((object)T2)->_cid = _e__sum_int64_t;
  } else {
    T2 = meme;
  }
  T3 = (&(T2->super));
  _construct_object(T3, _ln, _fn);
  T2->value = value;
  return T2;
}

/* ChapelReduce.chpl:26 */
void accumulate(_sum_int64_t _this_390543, int64_t x) {
  int64_t T1;
  int64_t T2;
  T1 = (_this_390543->value);
  T2 = (T1+x);
  _this_390543->value = T2;
  return;
}

/* ChapelReduce.chpl:29 */
int64_t generate(_sum_int64_t _this_391518) {
  int64_t T2;
  int64_t T1;
  T1 = (_this_391518->value);
  T2 = T1;
  return T2;
}

/* ChapelReduce.chpl:42 */
_max__real64 _construct__max(_real64 value, _max__real64 meme, int32_t _ln, _string _fn) {
  _max__real64 T2 = NULL;
  chpl_bool T1;
  object T3 = NULL;
  T1 = (nil==meme);
  if (T1) {
    T2 = (_max__real64)chpl_alloc(sizeof(__max__real64), "instance of class _max", _ln, _fn);
    ((object)T2)->_cid = _e__max__real64;
  } else {
    T2 = meme;
  }
  T3 = (&(T2->super));
  _construct_object(T3, _ln, _fn);
  T2->value = value;
  return T2;
}

/* ChapelReduce.chpl:46 */
void _accumulate_648307(_max__real64 _this_648311, _real64 x) {
  _real64 T1;
  _real64 T3;
  chpl_bool T2;
  T1 = (_this_648311->value);
  T2 = (x>T1);
  if (T2) {
    T3 = x;
  } else {
    T3 = T1;
  }
  _this_648311->value = T3;
  return;
}

/* ChapelReduce.chpl:49 */
_real64 _generate_649547(_max__real64 _this_649551) {
  _real64 T2;
  _real64 T1;
  T1 = (_this_649551->value);
  T2 = T1;
  return T2;
}

/* ChapelReduce.chpl:52 */
_min_int64_t _construct__min(int64_t value, _min_int64_t meme, int32_t _ln, _string _fn) {
  _min_int64_t T2 = NULL;
  chpl_bool T1;
  object T3 = NULL;
  T1 = (nil==meme);
  if (T1) {
    T2 = (_min_int64_t)chpl_alloc(sizeof(__min_int64_t), "instance of class _min", _ln, _fn);
    ((object)T2)->_cid = _e__min_int64_t;
  } else {
    T2 = meme;
  }
  T3 = (&(T2->super));
  _construct_object(T3, _ln, _fn);
  T2->value = value;
  return T2;
}

/* ChapelReduce.chpl:56 */
void _accumulate_396519(_min_int64_t _this_396523, int64_t x) {
  int64_t T1;
  int64_t T3;
  chpl_bool T2;
  T1 = (_this_396523->value);
  T2 = (x<T1);
  if (T2) {
    T3 = x;
  } else {
    T3 = T1;
  }
  _this_396523->value = T3;
  return;
}

/* ChapelReduce.chpl:59 */
int64_t _generate_397007(_min_int64_t _this_397011) {
  int64_t T2;
  int64_t T1;
  T1 = (_this_397011->value);
  T2 = T1;
  return T2;
}

