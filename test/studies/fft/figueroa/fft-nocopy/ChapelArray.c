/* ChapelArray.chpl:1 */
void __init_ChapelArray(void) {
  chpl_bool T1;
  T1 = (!__run_ChapelArray_firsttime7);
  if (T1) {
    goto _end___init_ChapelArray;
  }
  __run_ChapelArray_firsttime7 = false;
  _end___init_ChapelArray:;
  return;
}

/* ChapelArray.chpl:23 */
_ArrayTypeInfo _build_array_type(_domain_1_SingleLocaleArithmeticDomain_1_int32_t_0* const dom) {
  SingleLocaleArithmeticDomain_1_int32_t_0 T1 = NULL;
  _ArrayTypeInfo T2;
  T1 = ((*dom)._value);
  T2.dom = T1;
  return T2;
}

/* ChapelArray.chpl:23 */
__ArrayTypeInfo_700613 __build_array_type_447288(_domain_1_SingleLocaleArithmeticDomain_1_int64_t_0* const dom) {
  SingleLocaleArithmeticDomain_1_int64_t_0 T1 = NULL;
  __ArrayTypeInfo_700613 T2;
  T1 = ((*dom)._value);
  T2.dom = T1;
  return T2;
}

/* ChapelArray.chpl:23 */
__ArrayTypeInfo_700380 __build_array_type_322184(_domain_1_SingleLocaleArithmeticDomain_1_int32_t_0* const dom) {
  SingleLocaleArithmeticDomain_1_int32_t_0 T1 = NULL;
  __ArrayTypeInfo_700380 T2;
  T1 = ((*dom)._value);
  T2.dom = T1;
  return T2;
}

/* ChapelArray.chpl:29 */
_domain_1_SingleLocaleArithmeticDomain_1_int32_t_0 __build_domain_277339(_domain_1_SingleLocaleArithmeticDomain_1_int32_t_0* const x) {
  _domain_1_SingleLocaleArithmeticDomain_1_int32_t_0 T1;
  _domain_1_SingleLocaleArithmeticDomain_1_int32_t_0 T2;
  T1 = (*x);
  T2 = T1;
  return T2;
}

/* ChapelArray.chpl:29 */
_domain_1_SingleLocaleArithmeticDomain_1_int64_t_0 __build_domain_447257(_domain_1_SingleLocaleArithmeticDomain_1_int64_t_0* const x) {
  _domain_1_SingleLocaleArithmeticDomain_1_int64_t_0 T1;
  _domain_1_SingleLocaleArithmeticDomain_1_int64_t_0 T2;
  T1 = (*x);
  T2 = T1;
  return T2;
}

/* ChapelArray.chpl:32 */
_domain_1_SingleLocaleArithmeticDomain_1_int64_t_0 __build_domain_442256(range_int64_t_bounded_0* const _e0_ranges, int32_t _ln, _string _fn) {
  range_int64_t_bounded_0 T1;
  _tuple_1_range_int64_t_bounded_0 T2;
  _domain_1_SingleLocaleArithmeticDomain_1_int64_t_0 T8;
  SingleLocaleDistribution T3 = NULL;
  _domain_1_SingleLocaleArithmeticDomain_1_int64_t_0 T6;
  _domain_1_SingleLocaleArithmeticDomain_1_int64_t_0 T4;
  SingleLocaleArithmeticDomain_1_int64_t_0 T5 = NULL;
  _domain_1_SingleLocaleArithmeticDomain_1_int64_t_0 T7;
  T1 = __copy_429953(&((*_e0_ranges)));
  T2.x1 = T1;
  T3 = (SingleLocaleDistribution)chpl_alloc(sizeof(_SingleLocaleDistribution), "instance of class _unknown", _ln, _fn);
  ((object)T3)->_cid = _e_SingleLocaleDistribution;
  _construct_SingleLocaleDistribution(T3, _ln, _fn);
  T4._value = nil;
  T5 = ((SingleLocaleArithmeticDomain_1_int64_t_0)(nil));
  T4._value = T5;
  T6 = __construct__domain_430768(1, T5, _ln, _fn);
  T7 = T6;
  _setIndices_443067(&(T7), &(T2));
  T8 = T6;
  return T8;
}

/* ChapelArray.chpl:32 */
_domain_1_SingleLocaleArithmeticDomain_1_int32_t_0 _build_domain(range_int32_t_bounded_0* const _e0_ranges, int32_t _ln, _string _fn) {
  range_int32_t_bounded_0 T1;
  _tuple_1_range_int32_t_bounded_0 T2;
  _domain_1_SingleLocaleArithmeticDomain_1_int32_t_0 T8;
  //SingleLocaleDistribution T3 = NULL;
  _domain_1_SingleLocaleArithmeticDomain_1_int32_t_0 T6;
  _domain_1_SingleLocaleArithmeticDomain_1_int32_t_0 T4;
  SingleLocaleArithmeticDomain_1_int32_t_0 T5 = NULL;
  _domain_1_SingleLocaleArithmeticDomain_1_int32_t_0 T7;
  T1 = _copy(&((*_e0_ranges)));
  T2.x1 = T1;
  //T3 = (SingleLocaleDistribution)chpl_alloc(sizeof(_SingleLocaleDistribution), "instance of class _unknown", _ln, _fn);
  //((object)T3)->_cid = _e_SingleLocaleDistribution;
  //_construct_SingleLocaleDistribution(T3, _ln, _fn);
  T4._value = nil;
  T5 = ((SingleLocaleArithmeticDomain_1_int32_t_0)(nil));
  T4._value = T5;
  T6 = _construct__domain(1, T5, _ln, _fn);
  T7 = T6;
  setIndices(&(T7), &(T2));
  T8 = T6;
  return T8;
}

/* ChapelArray.chpl:41 */
_domain_1_SingleLocaleArithmeticDomain_1_int32_t_0 _wrapDomain(SingleLocaleArithmeticDomain_1_int32_t_0 d, int32_t _ln, _string _fn) {
  _domain_1_SingleLocaleArithmeticDomain_1_int32_t_0 T2;
  _domain_1_SingleLocaleArithmeticDomain_1_int32_t_0 T1;
  T1 = _construct__domain(1, d, _ln, _fn);
  T2 = T1;
  return T2;
}

/* ChapelArray.chpl:41 */
_domain_1_SingleLocaleArithmeticDomain_1_int64_t_0 __wrapDomain_470260(SingleLocaleArithmeticDomain_1_int64_t_0 d, int32_t _ln, _string _fn) {
  _domain_1_SingleLocaleArithmeticDomain_1_int64_t_0 T2;
  _domain_1_SingleLocaleArithmeticDomain_1_int64_t_0 T1;
  T1 = __construct__domain_430768(1, d, _ln, _fn);
  T2 = T1;
  return T2;
}

/* ChapelArray.chpl:131 */
_domain_1_SingleLocaleArithmeticDomain_1_int32_t_0 _construct__domain(int32_t rank, SingleLocaleArithmeticDomain_1_int32_t_0 _value, int32_t _ln, _string _fn) {
  _domain_1_SingleLocaleArithmeticDomain_1_int32_t_0 T4;
  _domain_1_SingleLocaleArithmeticDomain_1_int32_t_0 T1;
  _domain_1_SingleLocaleArithmeticDomain_1_int32_t_0 T3;
  _domain_1_SingleLocaleArithmeticDomain_1_int32_t_0 T2;
  T1._value = _value;
  T2 = T1;
  T3 = _initialize_234797(&(T2), _ln, _fn);
  T4 = T3;
  return T4;
}

/* ChapelArray.chpl:131 */
_domain_1_SingleLocaleArithmeticDomain_1_int64_t_0 __construct__domain_430768(int32_t rank, SingleLocaleArithmeticDomain_1_int64_t_0 _value, int32_t _ln, _string _fn) {
  _domain_1_SingleLocaleArithmeticDomain_1_int64_t_0 T4;
  _domain_1_SingleLocaleArithmeticDomain_1_int64_t_0 T1;
  _domain_1_SingleLocaleArithmeticDomain_1_int64_t_0 T3;
  _domain_1_SingleLocaleArithmeticDomain_1_int64_t_0 T2;
  T1._value = _value;
  T2 = T1;
  T3 = _initialize_437951(&(T2), _ln, _fn);
  T4 = T3;
  return T4;
}

/* ChapelArray.chpl:131 */
_domain_1_SingleLocaleArithmeticDomain_1_int64_t_1 __construct__domain_551628(int32_t rank, SingleLocaleArithmeticDomain_1_int64_t_1 _value, int32_t _ln, _string _fn) {
  _domain_1_SingleLocaleArithmeticDomain_1_int64_t_1 T4;
  _domain_1_SingleLocaleArithmeticDomain_1_int64_t_1 T1;
  _domain_1_SingleLocaleArithmeticDomain_1_int64_t_1 T3;
  _domain_1_SingleLocaleArithmeticDomain_1_int64_t_1 T2;
  T1._value = _value;
  T2 = T1;
  T3 = _initialize_558716(&(T2), _ln, _fn);
  T4 = T3;
  return T4;
}

/* ChapelArray.chpl:136 */
_domain_1_SingleLocaleArithmeticDomain_1_int64_t_0 _initialize_437951(_domain_1_SingleLocaleArithmeticDomain_1_int64_t_0* const _this_437955, int32_t _ln, _string _fn) {
  _domain_1_SingleLocaleArithmeticDomain_1_int64_t_0 T6;
  SingleLocaleArithmeticDomain_1_int64_t_0 T1 = NULL;
  object T2 = NULL;
  chpl_bool T3;
  SingleLocaleArithmeticDomain_1_int64_t_0 T5 = NULL;
  SingleLocaleArithmeticDomain_1_int64_t_0 T4 = NULL;
  T1 = ((*_this_437955)._value);
  T2 = ((object)(T1));
  T3 = (T2==nil);
  if (T3) {
    T4 = ((*_this_437955)._value);
    T5 = _buildEmptyDomain_438927(T4, _ln, _fn);
    (*_this_437955)._value = T5;
  }
  T6 = (*_this_437955);
  return T6;
}

/* ChapelArray.chpl:136 */
_domain_1_SingleLocaleArithmeticDomain_1_int64_t_1 _initialize_558716(_domain_1_SingleLocaleArithmeticDomain_1_int64_t_1* const _this_558720, int32_t _ln, _string _fn) {
  _domain_1_SingleLocaleArithmeticDomain_1_int64_t_1 T6;
  SingleLocaleArithmeticDomain_1_int64_t_1 T1 = NULL;
  object T2 = NULL;
  chpl_bool T3;
  SingleLocaleArithmeticDomain_1_int64_t_1 T5 = NULL;
  SingleLocaleArithmeticDomain_1_int64_t_1 T4 = NULL;
  T1 = ((*_this_558720)._value);
  T2 = ((object)(T1));
  T3 = (T2==nil);
  if (T3) {
    T4 = ((*_this_558720)._value);
    T5 = _buildEmptyDomain_560120(T4, _ln, _fn);
    (*_this_558720)._value = T5;
  }
  T6 = (*_this_558720);
  return T6;
}

/* ChapelArray.chpl:136 */
_domain_1_SingleLocaleArithmeticDomain_1_int32_t_0 _initialize_234797(_domain_1_SingleLocaleArithmeticDomain_1_int32_t_0* const _this_234801, int32_t _ln, _string _fn) {
  _domain_1_SingleLocaleArithmeticDomain_1_int32_t_0 T6;
  SingleLocaleArithmeticDomain_1_int32_t_0 T1 = NULL;
  object T2 = NULL;
  chpl_bool T3;
  SingleLocaleArithmeticDomain_1_int32_t_0 T5 = NULL;
  SingleLocaleArithmeticDomain_1_int32_t_0 T4 = NULL;
  T1 = ((*_this_234801)._value);
  T2 = ((object)(T1));
  T3 = (T2==nil);
  if (T3) {
    T4 = ((*_this_234801)._value);
    T5 = buildEmptyDomain(T4, _ln, _fn);
    (*_this_234801)._value = T5;
  }
  T6 = (*_this_234801);
  return T6;
}

/* ChapelArray.chpl:148 */
_domain_1_SingleLocaleArithmeticDomain_1_int64_t_1 _this_622027(_domain_1_SingleLocaleArithmeticDomain_1_int64_t_0* const _this_622033, range_int32_t_bounded_1* const _e0_ranges, int32_t _ln, _string _fn) {
  range_int32_t_bounded_1 T1;
  _domain_1_SingleLocaleArithmeticDomain_1_int64_t_1 T7;
  range_int32_t_bounded_1 T2;
  _tuple_1_range_int32_t_bounded_1 T3;
  SingleLocaleArithmeticDomain_1_int64_t_1 T5 = NULL;
  SingleLocaleArithmeticDomain_1_int64_t_0 T4 = NULL;
  _domain_1_SingleLocaleArithmeticDomain_1_int64_t_1 T6;
  T1 = __copy_536668(&((*_e0_ranges)));
  T2 = __copy_536668(&(T1));
  T3.x1 = T2;
  T4 = ((*_this_622033)._value);
  T5 = _slice_622594(T4, true, &(T3), _ln, _fn);
  T6 = __construct__domain_551628(1, T5, _ln, _fn);
  T7 = T6;
  return T7;
}

/* ChapelArray.chpl:148 */
_domain_1_SingleLocaleArithmeticDomain_1_int64_t_0 _this_674622(_domain_1_SingleLocaleArithmeticDomain_1_int64_t_0* const _this_674628, range_int64_t_bounded_0* const _e0_ranges, int32_t _ln, _string _fn) {
  range_int64_t_bounded_0 T1;
  _domain_1_SingleLocaleArithmeticDomain_1_int64_t_0 T7;
  range_int64_t_bounded_0 T2;
  _tuple_1_range_int64_t_bounded_0 T3;
  SingleLocaleArithmeticDomain_1_int64_t_0 T5 = NULL;
  SingleLocaleArithmeticDomain_1_int64_t_0 T4 = NULL;
  _domain_1_SingleLocaleArithmeticDomain_1_int64_t_0 T6;
  T1 = __copy_429953(&((*_e0_ranges)));
  T2 = __copy_429953(&(T1));
  T3.x1 = T2;
  T4 = ((*_this_674628)._value);
  T5 = _slice_675366(T4, false, &(T3), _ln, _fn);
  T6 = __construct__domain_430768(1, T5, _ln, _fn);
  T7 = T6;
  return T7;
}

/* ChapelArray.chpl:148 */
_domain_1_SingleLocaleArithmeticDomain_1_int64_t_1 _this_573440(_domain_1_SingleLocaleArithmeticDomain_1_int64_t_0* const _this_573446, range_int64_t_bounded_1* const _e0_ranges, int32_t _ln, _string _fn) {
  range_int64_t_bounded_1 T1;
  _domain_1_SingleLocaleArithmeticDomain_1_int64_t_1 T7;
  range_int64_t_bounded_1 T2;
  _tuple_1_range_int64_t_bounded_1 T3;
  SingleLocaleArithmeticDomain_1_int64_t_1 T5 = NULL;
  SingleLocaleArithmeticDomain_1_int64_t_0 T4 = NULL;
  _domain_1_SingleLocaleArithmeticDomain_1_int64_t_1 T6;
  T1 = __copy_547955(&((*_e0_ranges)));
  T2 = __copy_547955(&(T1));
  T3.x1 = T2;
  T4 = ((*_this_573446)._value);
  T5 = slice(T4, true, &(T3), _ln, _fn);
  T6 = __construct__domain_551628(1, T5, _ln, _fn);
  T7 = T6;
  return T7;
}

/* ChapelArray.chpl:148 */
_domain_1_SingleLocaleArithmeticDomain_1_int32_t_0 _this_663900(_domain_1_SingleLocaleArithmeticDomain_1_int32_t_0* const _this_663906, range_int32_t_bounded_0* const _e0_ranges, int32_t _ln, _string _fn) {
  range_int32_t_bounded_0 T1;
  _domain_1_SingleLocaleArithmeticDomain_1_int32_t_0 T7;
  range_int32_t_bounded_0 T2;
  _tuple_1_range_int32_t_bounded_0 T3;
  SingleLocaleArithmeticDomain_1_int32_t_0 T5 = NULL;
  SingleLocaleArithmeticDomain_1_int32_t_0 T4 = NULL;
  _domain_1_SingleLocaleArithmeticDomain_1_int32_t_0 T6;
  T1 = _copy(&((*_e0_ranges)));
  T2 = _copy(&(T1));
  T3.x1 = T2;
  T4 = ((*_this_663906)._value);
  T5 = _slice_664786(T4, false, &(T3), _ln, _fn);
  T6 = _construct__domain(1, T5, _ln, _fn);
  T7 = T6;
  return T7;
}

/* ChapelArray.chpl:164 */
_array_int32_t_int32_t_1_SingleLocaleArithmeticArray_int32_t_int32_t_0_1_0 _buildArray_322332(_domain_1_SingleLocaleArithmeticDomain_1_int32_t_0* const _this_322336, int32_t _ln, _string _fn) {
  _array_int32_t_int32_t_1_SingleLocaleArithmeticArray_int32_t_int32_t_0_1_0 T8;
  SingleLocaleArithmeticArray_int32_t_int32_t_0_1_0 T2 = NULL;
  SingleLocaleArithmeticDomain_1_int32_t_0 T1 = NULL;
  SingleLocaleArithmeticDomain_1_int32_t_0 T3 = NULL;
  BaseDomain T4 = NULL;
  _ref_list_BaseArray T5 = NULL;
  BaseArray T6 = NULL;
  _array_int32_t_int32_t_1_SingleLocaleArithmeticArray_int32_t_int32_t_0_1_0 T7;
  T1 = ((*_this_322336)._value);
  T2 = _buildArray_322761(T1, _ln, _fn);
  T3 = ((*_this_322336)._value);
  T4 = ((BaseDomain)(T3));
  T5 = (&(T4->_arrs));
  T6 = ((BaseArray)(T2));
  //append(T5, T6, _ln, _fn);
  T7 = __construct__array_334753(1, T2);
  T8 = T7;
  return T8;
}

/* ChapelArray.chpl:164 */
_array_int64_t__complex128_1_SingleLocaleArithmeticArray__complex128_int64_t_0_1_0 _buildArray_447332(_domain_1_SingleLocaleArithmeticDomain_1_int64_t_0* const _this_447336, int32_t _ln, _string _fn) {
  _array_int64_t__complex128_1_SingleLocaleArithmeticArray__complex128_int64_t_0_1_0 T8;
  SingleLocaleArithmeticArray__complex128_int64_t_0_1_0 T2 = NULL;
  SingleLocaleArithmeticDomain_1_int64_t_0 T1 = NULL;
  SingleLocaleArithmeticDomain_1_int64_t_0 T3 = NULL;
  BaseDomain T4 = NULL;
  _ref_list_BaseArray T5 = NULL;
  BaseArray T6 = NULL;
  _array_int64_t__complex128_1_SingleLocaleArithmeticArray__complex128_int64_t_0_1_0 T7;
  T1 = ((*_this_447336)._value);
  T2 = _buildArray_447865(T1, _ln, _fn);
  T3 = ((*_this_447336)._value);
  T4 = ((BaseDomain)(T3));
  T5 = (&(T4->_arrs));
  T6 = ((BaseArray)(T2));
  //append(T5, T6, _ln, _fn);
  T7 = __construct__array_463782(1, T2);
  T8 = T7;
  return T8;
}

/* ChapelArray.chpl:164 */
_array_int32_t_locale_1_SingleLocaleArithmeticArray_locale_int32_t_0_1_0 buildArray(_domain_1_SingleLocaleArithmeticDomain_1_int32_t_0* const _this_277417, int32_t _ln, _string _fn) {
  _array_int32_t_locale_1_SingleLocaleArithmeticArray_locale_int32_t_0_1_0 T8;
  SingleLocaleArithmeticArray_locale_int32_t_0_1_0 T2 = NULL;
  SingleLocaleArithmeticDomain_1_int32_t_0 T1 = NULL;
  SingleLocaleArithmeticDomain_1_int32_t_0 T3 = NULL;
  BaseDomain T4 = NULL;
  _ref_list_BaseArray T5 = NULL;
  BaseArray T6 = NULL;
  _array_int32_t_locale_1_SingleLocaleArithmeticArray_locale_int32_t_0_1_0 T7;
  T1 = ((*_this_277417)._value);
  T2 = _buildArray_277946(T1, _ln, _fn);
  T3 = ((*_this_277417)._value);
  T4 = ((BaseDomain)(T3));
  T5 = (&(T4->_arrs));
  T6 = ((BaseArray)(T2));
  append(T5, T6, _ln, _fn);
  T7 = _construct__array(1, T2);
  T8 = T7;
  return T8;
}

/* ChapelArray.chpl:206 */
int64_t numIndices(_domain_1_SingleLocaleArithmeticDomain_1_int64_t_0* const _this_471799, int32_t _ln, _string _fn) {
  int64_t T3;
  int64_t T2;
  SingleLocaleArithmeticDomain_1_int64_t_0 T1 = NULL;
  T1 = ((*_this_471799)._value);
  T2 = _numIndices_471836(T1, _ln, _fn);
  T3 = T2;
  return T3;
}

/* ChapelArray.chpl:252 */
void _setIndices_443067(_domain_1_SingleLocaleArithmeticDomain_1_int64_t_0* const _this_443071, _tuple_1_range_int64_t_bounded_0* const x) {
  range_int64_t_bounded_0 T1;
  range_int64_t_bounded_0 T2;
  range_int64_t_bounded_0 T3;
  _tuple_1_range_int64_t_bounded_0 T4;
  SingleLocaleArithmeticDomain_1_int64_t_0 T5 = NULL;
  T1 = ((*x).x1);
  T2 = __copy_429953(&(T1));
  T3 = __copy_429953(&(T2));
  T4.x1 = T3;
  T5 = ((*_this_443071)._value);
  _setIndices_443294(T5, &(T4));
  return;
}

/* ChapelArray.chpl:252 */
void setIndices(_domain_1_SingleLocaleArithmeticDomain_1_int32_t_0* const _this_249241, _tuple_1_range_int32_t_bounded_0* const x) {
  range_int32_t_bounded_0 T1;
  range_int32_t_bounded_0 T2;
  range_int32_t_bounded_0 T3;
  _tuple_1_range_int32_t_bounded_0 T4;
  SingleLocaleArithmeticDomain_1_int32_t_0 T5 = NULL;
  T1 = ((*x).x1);
  T2 = _copy(&(T1));
  T3 = _copy(&(T2));
  T4.x1 = T3;
  T5 = ((*_this_249241)._value);
  _setIndices_249464(T5, &(T4));
  return;
}

/* ChapelArray.chpl:271 */
_domain_1_SingleLocaleArithmeticDomain_1_int64_t_0 __EQUAL__445182(_domain_1_SingleLocaleArithmeticDomain_1_int64_t_0* const a, _domain_1_SingleLocaleArithmeticDomain_1_int64_t_0* const b, int32_t _ln, _string _fn) {
  _domain_1_SingleLocaleArithmeticDomain_1_int64_t_0 T40;
  _domain_1_SingleLocaleArithmeticDomain_1_int64_t_0 T1;
  SingleLocaleArithmeticDomain_1_int64_t_0 T2 = NULL;
  BaseDomain T3 = NULL;
  list_BaseArray T4;
  list_BaseArray T5;
  listNode_BaseArray T7 = NULL;
  listNode_BaseArray T6 = NULL;
  chpl_bool T10;
  object T8 = NULL;
  chpl_bool T9;
  BaseArray T11 = NULL;
  _domain_1_SingleLocaleArithmeticDomain_1_int64_t_0 T12;
  chpl_bool T13;
  SingleLocaleArithmeticArray_locale_int32_t_0_1_0 T14 = NULL;
  _domain_1_SingleLocaleArithmeticDomain_1_int64_t_0 T15;
  chpl_bool T16;
  SingleLocaleArithmeticArray_int32_t_int32_t_0_1_0 T17 = NULL;
  _domain_1_SingleLocaleArithmeticDomain_1_int64_t_0 T18;
  chpl_bool T19;
  SingleLocaleArithmeticArray__complex128_int64_t_0_1_0 T20 = NULL;
  _domain_1_SingleLocaleArithmeticDomain_1_int64_t_0 T21;
  chpl_bool T22;
  SingleLocaleArithmeticArray__complex128_int64_t_0_1_1 T23 = NULL;
  _domain_1_SingleLocaleArithmeticDomain_1_int64_t_0 T24;
  chpl_bool T25;
  SingleLocaleArithmeticArray__complex128_int32_t_1_1_0 T26 = NULL;
  _domain_1_SingleLocaleArithmeticDomain_1_int64_t_0 T27;
  _domain_1_SingleLocaleArithmeticDomain_1_int64_t_0 T28;
  listNode_BaseArray T29 = NULL;
  object T30 = NULL;
  chpl_bool T31;
  _domain_1_SingleLocaleArithmeticDomain_1_int64_t_0 T32;
  _domain_1_SingleLocaleArithmeticDomain_1_int64_t_0 T33;
  range_int64_t_bounded_0 T36;
  _tuple_1_range_int64_t_bounded_0 T35;
  SingleLocaleArithmeticDomain_1_int64_t_0 T34 = NULL;
  range_int64_t_bounded_0 T37;
  _tuple_1_range_int64_t_bounded_0 T38;
  SingleLocaleArithmeticDomain_1_int64_t_0 T39 = NULL;
  T1 = (*a);
  T2 = (T1._value);
  T3 = ((BaseDomain)(T2));
  T4 = (T3->_arrs);
  T5 = __copy_225794(&(T4));
  T6 = (T5.first);
  T7 = T6;
  T8 = ((object)(T6));
  T9 = (T8!=nil);
  T10 = T9;
  while (T10) {
    T11 = (T7->data);
    T12 = (*b);
    T13 = (((object)T11)->_cid == _e_SingleLocaleArithmeticArray_locale_int32_t_0_1_0);
    if (T13) {
      T14 = ((SingleLocaleArithmeticArray_locale_int32_t_0_1_0)(T11));
      T15 = T12;
      _reallocate_671413(T14, &(T15), _ln, _fn);
    } else {
      T16 = (((object)T11)->_cid == _e_SingleLocaleArithmeticArray_int32_t_int32_t_0_1_0);
      if (T16) {
        T17 = ((SingleLocaleArithmeticArray_int32_t_int32_t_0_1_0)(T11));
        T18 = T12;
        _reallocate_671813(T17, &(T18), _ln, _fn);
      } else {
        T19 = (((object)T11)->_cid == _e_SingleLocaleArithmeticArray__complex128_int64_t_0_1_0);
        if (T19) {
          T20 = ((SingleLocaleArithmeticArray__complex128_int64_t_0_1_0)(T11));
          T21 = T12;
          _reallocate_672213(T20, &(T21), _ln, _fn);
        } else {
          T22 = (((object)T11)->_cid == _e_SingleLocaleArithmeticArray__complex128_int64_t_0_1_1);
          if (T22) {
            T23 = ((SingleLocaleArithmeticArray__complex128_int64_t_0_1_1)(T11));
            T24 = T12;
            _reallocate_678945(T23, &(T24), _ln, _fn);
          } else {
            T25 = (((object)T11)->_cid == _e_SingleLocaleArithmeticArray__complex128_int32_t_1_1_0);
            if (T25) {
              T26 = ((SingleLocaleArithmeticArray__complex128_int32_t_1_1_0)(T11));
              T27 = T12;
              _reallocate_679345(T26, &(T27), _ln, _fn);
            } else {
              T28 = T12;
              _reallocate_446561(T11, &(T28), _ln, _fn);
            }
          }
        }
      }
    }
    T29 = (T7->next);
    T7 = T29;
    T30 = ((object)(T29));
    T31 = (T30!=nil);
    T10 = T31;
  }
  T32 = (*a);
  T33 = (*b);
  T34 = (T33._value);
  T35 = _getIndices_447038(T34);
  T36 = (T35.x1);
  T37 = __copy_429953(&(T36));
  T38.x1 = T37;
  T39 = (T32._value);
  _setIndices_443294(T39, &(T38));
  T40 = (*a);
  return T40;
}

/* ChapelArray.chpl:271 */
_domain_1_SingleLocaleArithmeticDomain_1_int32_t_0 __EQUAL__252504(_domain_1_SingleLocaleArithmeticDomain_1_int32_t_0* const a, _domain_1_SingleLocaleArithmeticDomain_1_int32_t_0* const b, int32_t _ln, _string _fn) {
  _domain_1_SingleLocaleArithmeticDomain_1_int32_t_0 T40;
  _domain_1_SingleLocaleArithmeticDomain_1_int32_t_0 T1;
  SingleLocaleArithmeticDomain_1_int32_t_0 T2 = NULL;
  BaseDomain T3 = NULL;
  list_BaseArray T4;
  list_BaseArray T5;
  listNode_BaseArray T7 = NULL;
  listNode_BaseArray T6 = NULL;
  chpl_bool T10;
  object T8 = NULL;
  chpl_bool T9;
  BaseArray T11 = NULL;
  _domain_1_SingleLocaleArithmeticDomain_1_int32_t_0 T12;
  chpl_bool T13;
  SingleLocaleArithmeticArray_locale_int32_t_0_1_0 T14 = NULL;
  _domain_1_SingleLocaleArithmeticDomain_1_int32_t_0 T15;
  chpl_bool T16;
  SingleLocaleArithmeticArray_int32_t_int32_t_0_1_0 T17 = NULL;
  _domain_1_SingleLocaleArithmeticDomain_1_int32_t_0 T18;
  chpl_bool T19;
  SingleLocaleArithmeticArray__complex128_int64_t_0_1_0 T20 = NULL;
  _domain_1_SingleLocaleArithmeticDomain_1_int32_t_0 T21;
  chpl_bool T22;
  SingleLocaleArithmeticArray__complex128_int64_t_0_1_1 T23 = NULL;
  _domain_1_SingleLocaleArithmeticDomain_1_int32_t_0 T24;
  chpl_bool T25;
  SingleLocaleArithmeticArray__complex128_int32_t_1_1_0 T26 = NULL;
  _domain_1_SingleLocaleArithmeticDomain_1_int32_t_0 T27;
  _domain_1_SingleLocaleArithmeticDomain_1_int32_t_0 T28;
  listNode_BaseArray T29 = NULL;
  object T30 = NULL;
  chpl_bool T31;
  _domain_1_SingleLocaleArithmeticDomain_1_int32_t_0 T32;
  _domain_1_SingleLocaleArithmeticDomain_1_int32_t_0 T33;
  range_int32_t_bounded_0 T36;
  _tuple_1_range_int32_t_bounded_0 T35;
  SingleLocaleArithmeticDomain_1_int32_t_0 T34 = NULL;
  range_int32_t_bounded_0 T37;
  _tuple_1_range_int32_t_bounded_0 T38;
  SingleLocaleArithmeticDomain_1_int32_t_0 T39 = NULL;
  T1 = (*a);
  T2 = (T1._value);
  T3 = ((BaseDomain)(T2));
  T4 = (T3->_arrs);
  T5 = __copy_225794(&(T4));
  T6 = (T5.first);
  T7 = T6;
  T8 = ((object)(T6));
  T9 = (T8!=nil);
  T10 = T9;
  while (T10) {
    T11 = (T7->data);
    T12 = (*b);
    T13 = (((object)T11)->_cid == _e_SingleLocaleArithmeticArray_locale_int32_t_0_1_0);
    if (T13) {
      T14 = ((SingleLocaleArithmeticArray_locale_int32_t_0_1_0)(T11));
      T15 = T12;
      _reallocate_661407(T14, &(T15), _ln, _fn);
    } else {
      T16 = (((object)T11)->_cid == _e_SingleLocaleArithmeticArray_int32_t_int32_t_0_1_0);
      if (T16) {
        T17 = ((SingleLocaleArithmeticArray_int32_t_int32_t_0_1_0)(T11));
        T18 = T12;
        _reallocate_668928(T17, &(T18), _ln, _fn);
      } else {
        T19 = (((object)T11)->_cid == _e_SingleLocaleArithmeticArray__complex128_int64_t_0_1_0);
        if (T19) {
          T20 = ((SingleLocaleArithmeticArray__complex128_int64_t_0_1_0)(T11));
          T21 = T12;
          _reallocate_669828(T20, &(T21), _ln, _fn);
        } else {
          T22 = (((object)T11)->_cid == _e_SingleLocaleArithmeticArray__complex128_int64_t_0_1_1);
          if (T22) {
            T23 = ((SingleLocaleArithmeticArray__complex128_int64_t_0_1_1)(T11));
            T24 = T12;
            _reallocate_670228(T23, &(T24), _ln, _fn);
          } else {
            T25 = (((object)T11)->_cid == _e_SingleLocaleArithmeticArray__complex128_int32_t_1_1_0);
            if (T25) {
              T26 = ((SingleLocaleArithmeticArray__complex128_int32_t_1_1_0)(T11));
              T27 = T12;
              _reallocate_670628(T26, &(T27), _ln, _fn);
            } else {
              T28 = T12;
              reallocate(T11, &(T28), _ln, _fn);
            }
          }
        }
      }
    }
    T29 = (T7->next);
    T7 = T29;
    T30 = ((object)(T29));
    T31 = (T30!=nil);
    T10 = T31;
  }
  T32 = (*a);
  T33 = (*b);
  T34 = (T33._value);
  T35 = getIndices(T34);
  T36 = (T35.x1);
  T37 = _copy(&(T36));
  T38.x1 = T37;
  T39 = (T32._value);
  _setIndices_249464(T39, &(T38));
  T40 = (*a);
  return T40;
}

/* ChapelArray.chpl:285 */
_domain_1_SingleLocaleArithmeticDomain_1_int64_t_0 __EQUAL__441768(_domain_1_SingleLocaleArithmeticDomain_1_int64_t_0* const d, range_int64_t_bounded_0* const r, int32_t _ln, _string _fn) {
  _domain_1_SingleLocaleArithmeticDomain_1_int64_t_0 T2;
  _domain_1_SingleLocaleArithmeticDomain_1_int64_t_0 T1;
  T1 = __build_domain_442256(&((*r)), _ln, _fn);
  (*d) = __EQUAL__445182(&((*d)), &(T1), _ln, _fn);
  T2 = (*d);
  return T2;
}

/* ChapelArray.chpl:314 */
_domain_1_SingleLocaleArithmeticDomain_1_int64_t_1 _by_545839(_domain_1_SingleLocaleArithmeticDomain_1_int64_t_0* const a, int32_t b, int32_t _ln, _string _fn) {
  _domain_1_SingleLocaleArithmeticDomain_1_int64_t_0 T1;
  _domain_1_SingleLocaleArithmeticDomain_1_int64_t_1 T5;
  SingleLocaleArithmeticDomain_1_int64_t_1 T3 = NULL;
  SingleLocaleArithmeticDomain_1_int64_t_0 T2 = NULL;
  _domain_1_SingleLocaleArithmeticDomain_1_int64_t_1 T4;
  T1 = (*a);
  T2 = (T1._value);
  T3 = strideBy(T2, b, _ln, _fn);
  T4 = __construct__domain_551628(1, T3, _ln, _fn);
  T5 = T4;
  return T5;
}

/* ChapelArray.chpl:321 */
_array_int32_t_locale_1_SingleLocaleArithmeticArray_locale_int32_t_0_1_0 _construct__array(int32_t rank, SingleLocaleArithmeticArray_locale_int32_t_0_1_0 _value) {
  _array_int32_t_locale_1_SingleLocaleArithmeticArray_locale_int32_t_0_1_0 T1;
  T1._value = _value;
  return T1;
}

/* ChapelArray.chpl:321 */
_array_int32_t_int32_t_1_SingleLocaleArithmeticArray_int32_t_int32_t_0_1_0 __construct__array_334753(int32_t rank, SingleLocaleArithmeticArray_int32_t_int32_t_0_1_0 _value) {
  _array_int32_t_int32_t_1_SingleLocaleArithmeticArray_int32_t_int32_t_0_1_0 T1;
  T1._value = _value;
  return T1;
}

/* ChapelArray.chpl:321 */
_array_int64_t__complex128_1_SingleLocaleArithmeticArray__complex128_int64_t_0_1_0 __construct__array_463782(int32_t rank, SingleLocaleArithmeticArray__complex128_int64_t_0_1_0 _value) {
  _array_int64_t__complex128_1_SingleLocaleArithmeticArray__complex128_int64_t_0_1_0 T1;
  T1._value = _value;
  return T1;
}

/* ChapelArray.chpl:321 */
_array_int64_t__complex128_1_SingleLocaleArithmeticArray__complex128_int64_t_0_1_1 __construct__array_591640(int32_t rank, SingleLocaleArithmeticArray__complex128_int64_t_0_1_1 _value) {
  _array_int64_t__complex128_1_SingleLocaleArithmeticArray__complex128_int64_t_0_1_1 T1;
  T1._value = _value;
  return T1;
}

/* ChapelArray.chpl:321 */
_array_int32_t__complex128_1_SingleLocaleArithmeticArray__complex128_int32_t_1_1_0 __construct__array_605911(int32_t rank, SingleLocaleArithmeticArray__complex128_int32_t_1_1_0 _value) {
  _array_int32_t__complex128_1_SingleLocaleArithmeticArray__complex128_int32_t_1_1_0 T1;
  T1._value = _value;
  return T1;
}

/* ChapelArray.chpl:328 */
_domain_1_SingleLocaleArithmeticDomain_1_int32_t_0 _dom(_array_int32_t_locale_1_SingleLocaleArithmeticArray_locale_int32_t_0_1_0* const _this_309714, int32_t _ln, _string _fn) {
  _domain_1_SingleLocaleArithmeticDomain_1_int32_t_0 T4;
  SingleLocaleArithmeticArray_locale_int32_t_0_1_0 T1 = NULL;
  SingleLocaleArithmeticDomain_1_int32_t_0 T2 = NULL;
  _domain_1_SingleLocaleArithmeticDomain_1_int32_t_0 T3;
  T1 = ((*_this_309714)._value);
  T2 = (T1->dom);
  T3 = _construct__domain(1, T2, _ln, _fn);
  T4 = T3;
  return T4;
}

/* ChapelArray.chpl:328 */
_domain_1_SingleLocaleArithmeticDomain_1_int64_t_0 __dom_470951(_array_int64_t__complex128_1_SingleLocaleArithmeticArray__complex128_int64_t_0_1_0* const _this_470955, int32_t _ln, _string _fn) {
  _domain_1_SingleLocaleArithmeticDomain_1_int64_t_0 T4;
  SingleLocaleArithmeticArray__complex128_int64_t_0_1_0 T1 = NULL;
  SingleLocaleArithmeticDomain_1_int64_t_0 T2 = NULL;
  _domain_1_SingleLocaleArithmeticDomain_1_int64_t_0 T3;
  T1 = ((*_this_470955)._value);
  T2 = (T1->dom);
  T3 = __construct__domain_430768(1, T2, _ln, _fn);
  T4 = T3;
  return T4;
}

/* ChapelArray.chpl:357 */
_array_int64_t__complex128_1_SingleLocaleArithmeticArray__complex128_int64_t_0_1_1 _this_621845(_array_int64_t__complex128_1_SingleLocaleArithmeticArray__complex128_int64_t_0_1_0* const _this_621851, range_int32_t_bounded_1* const _e0_ranges, int32_t _ln, _string _fn) {
  range_int32_t_bounded_1 T1;
  _array_int64_t__complex128_1_SingleLocaleArithmeticArray__complex128_int64_t_0_1_1 T9;
  _domain_1_SingleLocaleArithmeticDomain_1_int64_t_0 T2;
  _domain_1_SingleLocaleArithmeticDomain_1_int64_t_1 T4;
  _domain_1_SingleLocaleArithmeticDomain_1_int64_t_0 T3;
  SingleLocaleArithmeticDomain_1_int64_t_1 T5 = NULL;
  SingleLocaleArithmeticArray__complex128_int64_t_0_1_1 T7 = NULL;
  SingleLocaleArithmeticArray__complex128_int64_t_0_1_0 T6 = NULL;
  _array_int64_t__complex128_1_SingleLocaleArithmeticArray__complex128_int64_t_0_1_1 T8;
  T1 = __copy_536668(&((*_e0_ranges)));
  T2 = __dom_470951(&((*_this_621851)), _ln, _fn);
  T3 = T2;
  T4 = _this_622027(&(T3), &(T1), _ln, _fn);
  T5 = (T4._value);
  T6 = ((*_this_621851)._value);
  T7 = _slice_585498(T6, T5, _ln, _fn);
  T8 = __construct__array_591640(1, T7);
  T9 = T8;
  return T9;
}

/* ChapelArray.chpl:357 */
_array_int64_t__complex128_1_SingleLocaleArithmeticArray__complex128_int64_t_0_1_1 _this_573174(_array_int64_t__complex128_1_SingleLocaleArithmeticArray__complex128_int64_t_0_1_0* const _this_573180, range_int64_t_bounded_1* const _e0_ranges, int32_t _ln, _string _fn) {
  range_int64_t_bounded_1 T1;
  _array_int64_t__complex128_1_SingleLocaleArithmeticArray__complex128_int64_t_0_1_1 T9;
  _domain_1_SingleLocaleArithmeticDomain_1_int64_t_0 T2;
  _domain_1_SingleLocaleArithmeticDomain_1_int64_t_1 T4;
  _domain_1_SingleLocaleArithmeticDomain_1_int64_t_0 T3;
  SingleLocaleArithmeticDomain_1_int64_t_1 T5 = NULL;
  SingleLocaleArithmeticArray__complex128_int64_t_0_1_1 T7 = NULL;
  SingleLocaleArithmeticArray__complex128_int64_t_0_1_0 T6 = NULL;
  _array_int64_t__complex128_1_SingleLocaleArithmeticArray__complex128_int64_t_0_1_1 T8;
  T1 = __copy_547955(&((*_e0_ranges)));
  T2 = __dom_470951(&((*_this_573180)), _ln, _fn);
  T3 = T2;
  T4 = _this_573440(&(T3), &(T1), _ln, _fn);
  T5 = (T4._value);
  T6 = ((*_this_573180)._value);
  T7 = _slice_585498(T6, T5, _ln, _fn);
  T8 = __construct__array_591640(1, T7);
  T9 = T8;
  return T9;
}

/* ChapelArray.chpl:377 */
int64_t numElements(_array_int64_t__complex128_1_SingleLocaleArithmeticArray__complex128_int64_t_0_1_0* const _this_470915, int32_t _ln, _string _fn) {
  int64_t T4;
  _domain_1_SingleLocaleArithmeticDomain_1_int64_t_0 T1;
  int64_t T3;
  _domain_1_SingleLocaleArithmeticDomain_1_int64_t_0 T2;
  T1 = __dom_470951(&((*_this_470915)), _ln, _fn);
  T2 = T1;
  T3 = numIndices(&(T2), _ln, _fn);
  T4 = T3;
  return T4;
}

/* ChapelArray.chpl:379 */
_array_int32_t__complex128_1_SingleLocaleArithmeticArray__complex128_int32_t_1_1_0 reindex(_array_int64_t__complex128_1_SingleLocaleArithmeticArray__complex128_int64_t_0_1_1* const _this_599555, _domain_1_SingleLocaleArithmeticDomain_1_int32_t_0* const d, int32_t _ln, _string _fn) {
  _domain_1_SingleLocaleArithmeticDomain_1_int32_t_0 T1;
  _array_int32_t__complex128_1_SingleLocaleArithmeticArray__complex128_int32_t_1_1_0 T6;
  SingleLocaleArithmeticDomain_1_int32_t_0 T2 = NULL;
  SingleLocaleArithmeticArray__complex128_int32_t_1_1_0 T4 = NULL;
  SingleLocaleArithmeticArray__complex128_int64_t_0_1_1 T3 = NULL;
  _array_int32_t__complex128_1_SingleLocaleArithmeticArray__complex128_int32_t_1_1_0 T5;
  T1 = (*d);
  T2 = (T1._value);
  T3 = ((*_this_599555)._value);
  T4 = _reindex_599900(T3, T2, _ln, _fn);
  T5 = __construct__array_605911(1, T4);
  T6 = T5;
  return T6;
}

/* ChapelArray.chpl:410 */
_array_int32_t_int32_t_1_SingleLocaleArithmeticArray_int32_t_int32_t_0_1_0 __EQUAL__344534(_array_int32_t_int32_t_1_SingleLocaleArithmeticArray_int32_t_int32_t_0_1_0* const a, _tuple_27_int32_t_int32_t_int32_t_int32_t_int32_t_int32_t_int32_t_int32_t_int32_t_int32_t_int32_t_int32_t_int32_t_int32_t_int32_t_int32_t_int32_t_int32_t_int32_t_int32_t_int32_t_int32_t_int32_t_int32_t_int32_t_int32_t_int32_t* const b, int32_t _ln, _string _fn) {
  _array_int32_t_int32_t_1_SingleLocaleArithmeticArray_int32_t_int32_t_0_1_0 T31;
  _array_int32_t_int32_t_1_SingleLocaleArithmeticArray_int32_t_int32_t_0_1_0 T1;
  int32_t T2;
  int32_t T3;
  int32_t T4;
  int32_t T5;
  int32_t T6;
  int32_t T7;
  int32_t T8;
  int32_t T9;
  int32_t T10;
  int32_t T11;
  int32_t T12;
  int32_t T13;
  int32_t T14;
  int32_t T15;
  int32_t T16;
  int32_t T17;
  int32_t T18;
  int32_t T19;
  int32_t T20;
  int32_t T21;
  int32_t T22;
  int32_t T23;
  int32_t T24;
  int32_t T25;
  int32_t T26;
  int32_t T27;
  int32_t T28;
  _tuple_27_int32_t_int32_t_int32_t_int32_t_int32_t_int32_t_int32_t_int32_t_int32_t_int32_t_int32_t_int32_t_int32_t_int32_t_int32_t_int32_t_int32_t_int32_t_int32_t_int32_t_int32_t_int32_t_int32_t_int32_t_int32_t_int32_t_int32_t T29;
  SingleLocaleArithmeticArray_int32_t_int32_t_0_1_0 T30 = NULL;
  T1 = (*a);
  T2 = ((*b).x1);
  T3 = ((*b).x2);
  T4 = ((*b).x3);
  T5 = ((*b).x4);
  T6 = ((*b).x5);
  T7 = ((*b).x6);
  T8 = ((*b).x7);
  T9 = ((*b).x8);
  T10 = ((*b).x9);
  T11 = ((*b).x10);
  T12 = ((*b).x11);
  T13 = ((*b).x12);
  T14 = ((*b).x13);
  T15 = ((*b).x14);
  T16 = ((*b).x15);
  T17 = ((*b).x16);
  T18 = ((*b).x17);
  T19 = ((*b).x18);
  T20 = ((*b).x19);
  T21 = ((*b).x20);
  T22 = ((*b).x21);
  T23 = ((*b).x22);
  T24 = ((*b).x23);
  T25 = ((*b).x24);
  T26 = ((*b).x25);
  T27 = ((*b).x26);
  T28 = ((*b).x27);
  T29.x1 = T2;
  T29.x2 = T3;
  T29.x3 = T4;
  T29.x4 = T5;
  T29.x5 = T6;
  T29.x6 = T7;
  T29.x7 = T8;
  T29.x8 = T9;
  T29.x9 = T10;
  T29.x10 = T11;
  T29.x11 = T12;
  T29.x12 = T13;
  T29.x13 = T14;
  T29.x14 = T15;
  T29.x15 = T16;
  T29.x16 = T17;
  T29.x17 = T18;
  T29.x18 = T19;
  T29.x19 = T20;
  T29.x20 = T21;
  T29.x21 = T22;
  T29.x22 = T23;
  T29.x23 = T24;
  T29.x24 = T25;
  T29.x25 = T26;
  T29.x26 = T27;
  T29.x27 = T28;
  T30 = (T1._value);
  tupleInit(T30, &(T29), _ln, _fn);
  T31 = (*a);
  return T31;
}

/* ChapelArray.chpl:445 */
_array_int32_t_locale_1_SingleLocaleArithmeticArray_locale_int32_t_0_1_0 __init_306845(_ArrayTypeInfo* const t, int32_t _ln, _string _fn) {
  _array_int32_t_locale_1_SingleLocaleArithmeticArray_locale_int32_t_0_1_0 T5;
  _array_int32_t_locale_1_SingleLocaleArithmeticArray_locale_int32_t_0_1_0 T4;
  SingleLocaleArithmeticDomain_1_int32_t_0 T1 = NULL;
  _domain_1_SingleLocaleArithmeticDomain_1_int32_t_0 T2;
  _domain_1_SingleLocaleArithmeticDomain_1_int32_t_0 T3;
  T1 = ((*t).dom);
  T2 = _wrapDomain(T1, _ln, _fn);
  T3 = T2;
  T4 = buildArray(&(T3), _ln, _fn);
  T5 = T4;
  return T5;
}

/* ChapelArray.chpl:445 */
_array_int32_t_int32_t_1_SingleLocaleArithmeticArray_int32_t_int32_t_0_1_0 __init_341190(__ArrayTypeInfo_700380* const t, int32_t _ln, _string _fn) {
  _array_int32_t_int32_t_1_SingleLocaleArithmeticArray_int32_t_int32_t_0_1_0 T5;
  _array_int32_t_int32_t_1_SingleLocaleArithmeticArray_int32_t_int32_t_0_1_0 T4;
  SingleLocaleArithmeticDomain_1_int32_t_0 T1 = NULL;
  _domain_1_SingleLocaleArithmeticDomain_1_int32_t_0 T2;
  _domain_1_SingleLocaleArithmeticDomain_1_int32_t_0 T3;
  T1 = ((*t).dom);
  T2 = _wrapDomain(T1, _ln, _fn);
  T3 = T2;
  T4 = _buildArray_322332(&(T3), _ln, _fn);
  T5 = T4;
  return T5;
}

/* ChapelArray.chpl:445 */
_array_int64_t__complex128_1_SingleLocaleArithmeticArray__complex128_int64_t_0_1_0 __init_470219(__ArrayTypeInfo_700613* const t, int32_t _ln, _string _fn) {
  _array_int64_t__complex128_1_SingleLocaleArithmeticArray__complex128_int64_t_0_1_0 T5;
  _array_int64_t__complex128_1_SingleLocaleArithmeticArray__complex128_int64_t_0_1_0 T4;
  SingleLocaleArithmeticDomain_1_int64_t_0 T1 = NULL;
  _domain_1_SingleLocaleArithmeticDomain_1_int64_t_0 T2;
  _domain_1_SingleLocaleArithmeticDomain_1_int64_t_0 T3;
  T1 = ((*t).dom);
  T2 = __wrapDomain_470260(T1, _ln, _fn);
  T3 = T2;
  T4 = _buildArray_447332(&(T3), _ln, _fn);
  T5 = T4;
  return T5;
}

/* ChapelArray.chpl:449 */
void _writeThis_530684(_array_int64_t__complex128_1_SingleLocaleArithmeticArray__complex128_int64_t_0_1_0* const _this_530689, Writer f, int32_t _ln, _string _fn) {
  SingleLocaleArithmeticArray__complex128_int64_t_0_1_0 T1 = NULL;
  T1 = ((*_this_530689)._value);
  _write_530783(f, T1, _ln, _fn);
  return;
}

/* ChapelArray.chpl:455 */
BaseArray _construct_BaseArray(BaseArray meme, int32_t _ln, _string _fn) {
  BaseArray T2 = NULL;
  chpl_bool T1;
  object T3 = NULL;
  T1 = (nil==meme);
  if (T1) {
    T2 = (BaseArray)chpl_alloc(sizeof(_BaseArray), "instance of class BaseArray", _ln, _fn);
    ((object)T2)->_cid = _e_BaseArray;
  } else {
    T2 = meme;
  }
  T3 = (&(T2->super));
  _construct_object(T3, _ln, _fn);
  return T2;
}

/* ChapelArray.chpl:456 */
void reallocate(BaseArray _this_261714, _domain_1_SingleLocaleArithmeticDomain_1_int32_t_0* const d, int32_t _ln, _string _fn) {
  halt("reallocating not support for this array type", _ln, _fn);
  return;
}

/* ChapelArray.chpl:456 */
void _reallocate_446561(BaseArray _this_446565, _domain_1_SingleLocaleArithmeticDomain_1_int64_t_0* const d, int32_t _ln, _string _fn) {
  halt("reallocating not support for this array type", _ln, _fn);
  return;
}

/* ChapelArray.chpl:489 */
BaseDomain _construct_BaseDomain(list_BaseArray* const _arrs, BaseDomain meme, int32_t _ln, _string _fn) {
  list_BaseArray T1;
  BaseDomain T3 = NULL;
  chpl_bool T2;
  object T4 = NULL;
  T1 = __copy_225794(&((*_arrs)));
  T2 = (nil==meme);
  if (T2) {
    T3 = (BaseDomain)chpl_alloc(sizeof(_BaseDomain), "instance of class BaseDomain", _ln, _fn);
    ((object)T3)->_cid = _e_BaseDomain;
  } else {
    T3 = meme;
  }
  T4 = (&(T3->super));
  _construct_object(T4, _ln, _fn);
  T3->_arrs = T1;
  return T3;
}

/* ChapelArray.chpl:515 */
BaseArithmeticDomain _construct_BaseArithmeticDomain(list_BaseArray* const _arrs, BaseArithmeticDomain meme, int32_t _ln, _string _fn) {
  list_BaseArray T1;
  BaseArithmeticDomain T3 = NULL;
  chpl_bool T2;
  BaseDomain T4 = NULL;
  T1 = __copy_225794(&((*_arrs)));
  T2 = (nil==meme);
  if (T2) {
    T3 = (BaseArithmeticDomain)chpl_alloc(sizeof(_BaseArithmeticDomain), "instance of class BaseArithmeticDomain", _ln, _fn);
    ((object)T3)->_cid = _e_BaseArithmeticDomain;
  } else {
    T3 = meme;
  }
  T4 = (&(T3->super));
  _construct_BaseDomain(&(T1), T4, _ln, _fn);
  return T3;
}

/* ChapelArray.chpl:609 */
SingleLocaleDistribution distributed_warning(SingleLocaleDistribution d, int32_t _ln, _string _fn) {
  SingleLocaleDistribution T2 = NULL;
  chpl_bool T1;
  T1 = (numLocales>1);
  if (T1) {
    chpl_warning("'distributed' domains/arrays are not yet distributed across multiple locales", _ln, _fn);
  }
  T2 = d;
  return T2;
}

