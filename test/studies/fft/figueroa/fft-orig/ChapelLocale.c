/* ChapelLocale.chpl:1 */
void __init_ChapelLocale(int32_t _ln, _string _fn) {
  chpl_bool T1;
  _domain_1_SingleLocaleArithmeticDomain_1_int32_t_0 T6;
  SingleLocaleDistribution T2 = NULL;
  _domain_1_SingleLocaleArithmeticDomain_1_int32_t_0 T5;
  _domain_1_SingleLocaleArithmeticDomain_1_int32_t_0 T3;
  SingleLocaleArithmeticDomain_1_int32_t_0 T4 = NULL;
  _domain_1_SingleLocaleArithmeticDomain_1_int32_t_0 T9;
  int32_t T7;
  range_int32_t_bounded_0 T8;
  locale T10 = NULL;
  _array_int32_t_locale_1_SingleLocaleArithmeticArray_locale_int32_t_0_1_0 T13;
  _domain_1_SingleLocaleArithmeticDomain_1_int32_t_0 T11;
  _ArrayTypeInfo T12;
  _domain_1_SingleLocaleArithmeticDomain_1_int32_t_0 T15;
  _array_int32_t_locale_1_SingleLocaleArithmeticArray_locale_int32_t_0_1_0 T14;
  int32_t T22;
  _domain_1_SingleLocaleArithmeticDomain_1_int32_t_0 T16;
  SingleLocaleArithmeticDomain_1_int32_t_0 T17 = NULL;
  _tuple_1_range_int32_t_bounded_0 T18;
  range_int32_t_bounded_0 T19;
  range_int32_t_bounded_0 T20;
  int32_t T21;
  SingleLocaleArithmeticDomain_1_int32_t_0 T23 = NULL;
  range_int32_t_bounded_0 T25;
  _tuple_1_range_int32_t_bounded_0 T24;
  range_int32_t_bounded_0 T26;
  int32_t T27;
  int32_t T29;
  int32_t T28;
  int32_t T31;
  int32_t T30;
  _ref_int32_t T32 = NULL;
  locale T35 = NULL;
  _ref_int32_t T34 = NULL;
  int32_t T33;
  SingleLocaleArithmeticArray_locale_int32_t_0_1_0 T36 = NULL;
  int32_t T37;
  int32_t T38;
  int32_t T39;
  int32_t T40;
  _ddata_locale T41 = NULL;
  _ref_locale T42 = NULL;
  int32_t T43;
  T1 = (!__run_ChapelLocale_firsttime2);
  if (T1) {
    goto _end___init_ChapelLocale;
  }
  __run_ChapelLocale_firsttime2 = false;
  T2 = (SingleLocaleDistribution)chpl_alloc(sizeof(_SingleLocaleDistribution), "instance of class _unknown", _ln, _fn);
  ((object)T2)->_cid = _e_SingleLocaleDistribution;
  _construct_SingleLocaleDistribution(T2, _ln, _fn);
  T3._value = nil;
  T4 = ((SingleLocaleArithmeticDomain_1_int32_t_0)(nil));
  T3._value = T4;
  T5 = _construct__domain(1, T4, _ln, _fn);
  T6 = T5;
  T7 = (numLocales-1);
  T8 = _build_range(0, T7);
  T9 = _build_domain(&(T8), _ln, _fn);
  T6 = __EQUAL__252504(&(T6), &(T9), _ln, _fn);
  LocaleSpace = T6;
  doneCreatingLocales = false;
  T10 = ((locale)(nil));
  _here = T10;
  T11 = __build_domain_277339(&(LocaleSpace));
  T12 = _build_array_type(&(T11));
  T13 = __init_306845(&(T12), _ln, _fn);
  T14 = T13;
  T15 = _dom(&(T14), _ln, _fn);
  T16 = __build_domain_277339(&(LocaleSpace));
  T17 = (T16._value);
  T18 = (T17->ranges);
  T19 = (T18.x1);
  T20 = _copy(&(T19));
  T21 = (T20._low);
  T22 = T21;
  T23 = (T15._value);
  T24 = (T23->ranges);
  T25 = (T24.x1);
  T26 = _copy(&(T25));
  T27 = 0;
  T28 = (T26._low);
  T29 = T28;
  T30 = (T26._high);
  T31 = T30;
  for (T27 = T29; T27 <= T31; T27 += 1) {
    T32 = &(T27);
    T33 = T22;
    T34 = &(T33);
    T35 = chpl_setupLocale(T33, _ln, _fn);
    T36 = (T13._value);
    T37 = (T36->origin);
    T38 = (T37+T27);
    T39 = (T36->factoredOffs);
    T40 = (T38-T39);
    T41 = (T36->data);
    T42 = _ARRAY_GET(T41, T40);
    (*T42) = T35;
    T43 = (T33+1);
    T22 = T43;
  }
  Locales = T13;
  doneCreatingLocales = true;
  _end___init_ChapelLocale:;
  return;
}

/* ChapelLocale.chpl:10 */
locale _construct_locale(int32_t chpl_id, locale meme, int32_t _ln, _string _fn) {
  locale T2 = NULL;
  chpl_bool T1;
  object T3 = NULL;
  T1 = (nil==meme);
  if (T1) {
    T2 = (locale)chpl_alloc(sizeof(_locale), "instance of class locale", _ln, _fn);
    ((object)T2)->_cid = _e_locale;
  } else {
    T2 = meme;
  }
  T3 = (&(T2->super));
  _construct_object(T3, _ln, _fn);
  T2->chpl_id = chpl_id;
  return T2;
}

/* ChapelLocale.chpl:13 */
locale _locale_16521(int32_t id, int32_t _ln, _string _fn) {
  locale T3 = NULL;
  locale T2 = NULL;
  locale T1 = NULL;
  chpl_bool T4;
  T1 = (locale)chpl_alloc(sizeof(_locale), "instance of class _unknown", _ln, _fn);
  ((object)T1)->_cid = _e_locale;
  T1->chpl_id = 0;
  T1->chpl_id = 0;
  T2 = _construct_locale(0, T1, _ln, _fn);
  T3 = T2;
  T4 = doneCreatingLocales;
  if (T4) {
    halt("locales cannot be created", _ln, _fn);
  }
  T3->chpl_id = id;
  return T3;
}

/* ChapelLocale.chpl:33 */
locale chpl_setupLocale(int32_t id, int32_t _ln, _string _fn) {
  locale T2 = NULL;
  locale T1 = NULL;
  T1 = _locale_16521(id, _ln, _fn);
  _here = T1;
  T2 = T1;
  return T2;
}

