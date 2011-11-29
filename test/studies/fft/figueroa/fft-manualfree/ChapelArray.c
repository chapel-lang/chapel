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
__ArrayTypeInfo_709895 __build_array_type_600051(_domain_1_SingleLocaleArithmeticDomain_1_int64_t_1* const dom) {
  SingleLocaleArithmeticDomain_1_int64_t_1 T1 = NULL;
  __ArrayTypeInfo_709895 T2;
  T1 = ((*dom)._value);
  T2.dom = T1;
  return T2;
}

/* ChapelArray.chpl:23 */
__ArrayTypeInfo_709668 __build_array_type_447288(_domain_1_SingleLocaleArithmeticDomain_1_int64_t_0* const dom) {
  SingleLocaleArithmeticDomain_1_int64_t_0 T1 = NULL;
  __ArrayTypeInfo_709668 T2;
  T1 = ((*dom)._value);
  T2.dom = T1;
  return T2;
}

/* ChapelArray.chpl:23 */
__ArrayTypeInfo_709435 __build_array_type_322184(_domain_1_SingleLocaleArithmeticDomain_1_int32_t_0* const dom) {
  SingleLocaleArithmeticDomain_1_int32_t_0 T1 = NULL;
  __ArrayTypeInfo_709435 T2;
  T1 = ((*dom)._value);
  T2.dom = T1;
  return T2;
}

/* ChapelArray.chpl:23 */
_ArrayTypeInfo _build_array_type(_domain_1_SingleLocaleArithmeticDomain_1_int32_t_0* const dom) {
  SingleLocaleArithmeticDomain_1_int32_t_0 T1 = NULL;
  _ArrayTypeInfo T2;
  T1 = ((*dom)._value);
  T2.dom = T1;
  return T2;
}

/* ChapelArray.chpl:29 */
_domain_1_SingleLocaleArithmeticDomain_1_int64_t_1 __build_domain_600019(_domain_1_SingleLocaleArithmeticDomain_1_int64_t_1* const x) {
  _domain_1_SingleLocaleArithmeticDomain_1_int64_t_1 T1;
  _domain_1_SingleLocaleArithmeticDomain_1_int64_t_1 T2;
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

/* ChapelArray.chpl:29 */
_domain_1_SingleLocaleArithmeticDomain_1_int32_t_0 __build_domain_277339(_domain_1_SingleLocaleArithmeticDomain_1_int32_t_0* const x) {
  _domain_1_SingleLocaleArithmeticDomain_1_int32_t_0 T1;
  _domain_1_SingleLocaleArithmeticDomain_1_int32_t_0 T2;
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
_domain_1_SingleLocaleArithmeticDomain_1_int64_t_0 __wrapDomain_470260(SingleLocaleArithmeticDomain_1_int64_t_0 d, int32_t _ln, _string _fn) {
  _domain_1_SingleLocaleArithmeticDomain_1_int64_t_0 T2;
  _domain_1_SingleLocaleArithmeticDomain_1_int64_t_0 T1;
  T1 = __construct__domain_430768(1, d, _ln, _fn);
  T2 = T1;
  return T2;
}

/* ChapelArray.chpl:41 */
_domain_1_SingleLocaleArithmeticDomain_1_int64_t_1 __wrapDomain_598668(SingleLocaleArithmeticDomain_1_int64_t_1 d, int32_t _ln, _string _fn) {
  _domain_1_SingleLocaleArithmeticDomain_1_int64_t_1 T2;
  _domain_1_SingleLocaleArithmeticDomain_1_int64_t_1 T1;
  T1 = __construct__domain_551628(1, d, _ln, _fn);
  T2 = T1;
  return T2;
}

/* ChapelArray.chpl:41 */
_domain_1_SingleLocaleArithmeticDomain_1_int32_t_0 _wrapDomain(SingleLocaleArithmeticDomain_1_int32_t_0 d, int32_t _ln, _string _fn) {
  _domain_1_SingleLocaleArithmeticDomain_1_int32_t_0 T2;
  _domain_1_SingleLocaleArithmeticDomain_1_int32_t_0 T1;
  T1 = _construct__domain(1, d, _ln, _fn);
  T2 = T1;
  return T2;
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
_domain_1_SingleLocaleArithmeticDomain_1_int64_t_0 _this_686183(_domain_1_SingleLocaleArithmeticDomain_1_int64_t_0* const _this_686189, range_int64_t_bounded_0* const _e0_ranges, int32_t _ln, _string _fn) {
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
  T4 = ((*_this_686189)._value);
  T5 = _slice_686927(T4, false, &(T3), _ln, _fn);
  T6 = __construct__domain_430768(1, T5, _ln, _fn);
  T7 = T6;
  return T7;
}

/* ChapelArray.chpl:148 */
_domain_1_SingleLocaleArithmeticDomain_1_int32_t_0 _this_675461(_domain_1_SingleLocaleArithmeticDomain_1_int32_t_0* const _this_675467, range_int32_t_bounded_0* const _e0_ranges, int32_t _ln, _string _fn) {
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
  T4 = ((*_this_675467)._value);
  T5 = _slice_676347(T4, false, &(T3), _ln, _fn);
  T6 = _construct__domain(1, T5, _ln, _fn);
  T7 = T6;
  return T7;
}

/* ChapelArray.chpl:148 */
_domain_1_SingleLocaleArithmeticDomain_1_int64_t_1 _this_633639(_domain_1_SingleLocaleArithmeticDomain_1_int64_t_0* const _this_633645, range_int32_t_bounded_1* const _e0_ranges, int32_t _ln, _string _fn) {
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
  T4 = ((*_this_633645)._value);
  T5 = _slice_634206(T4, true, &(T3), _ln, _fn);
  T6 = __construct__domain_551628(1, T5, _ln, _fn);
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
_array_int64_t__complex128_1_SingleLocaleArithmeticArray__complex128_int64_t_0_1_1 _buildArray_598729(_domain_1_SingleLocaleArithmeticDomain_1_int64_t_1* const _this_598733, int32_t _ln, _string _fn) {
  _array_int64_t__complex128_1_SingleLocaleArithmeticArray__complex128_int64_t_0_1_1 T8;
  SingleLocaleArithmeticArray__complex128_int64_t_0_1_1 T2 = NULL;
  SingleLocaleArithmeticDomain_1_int64_t_1 T1 = NULL;
  SingleLocaleArithmeticDomain_1_int64_t_1 T3 = NULL;
  BaseDomain T4 = NULL;
  _ref_list_BaseArray T5 = NULL;
  BaseArray T6 = NULL;
  _array_int64_t__complex128_1_SingleLocaleArithmeticArray__complex128_int64_t_0_1_1 T7;
  T1 = ((*_this_598733)._value);
  T2 = _buildArray_600097(T1, _ln, _fn);
  T3 = ((*_this_598733)._value);
  T4 = ((BaseDomain)(T3));
  T5 = (&(T4->_arrs));
  T6 = ((BaseArray)(T2));
  //append(T5, T6, _ln, _fn);
  T7 = __construct__array_591640(1, T2);
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
  append(T5, T6, _ln, _fn);
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
      _reallocate_672968(T14, &(T15), _ln, _fn);
    } else {
      T16 = (((object)T11)->_cid == _e_SingleLocaleArithmeticArray_int32_t_int32_t_0_1_0);
      if (T16) {
        T17 = ((SingleLocaleArithmeticArray_int32_t_int32_t_0_1_0)(T11));
        T18 = T12;
        _reallocate_680489(T17, &(T18), _ln, _fn);
      } else {
        T19 = (((object)T11)->_cid == _e_SingleLocaleArithmeticArray__complex128_int64_t_0_1_0);
        if (T19) {
          T20 = ((SingleLocaleArithmeticArray__complex128_int64_t_0_1_0)(T11));
          T21 = T12;
          _reallocate_681389(T20, &(T21), _ln, _fn);
        } else {
          T22 = (((object)T11)->_cid == _e_SingleLocaleArithmeticArray__complex128_int64_t_0_1_1);
          if (T22) {
            T23 = ((SingleLocaleArithmeticArray__complex128_int64_t_0_1_1)(T11));
            T24 = T12;
            _reallocate_681789(T23, &(T24), _ln, _fn);
          } else {
            T25 = (((object)T11)->_cid == _e_SingleLocaleArithmeticArray__complex128_int32_t_1_1_0);
            if (T25) {
              T26 = ((SingleLocaleArithmeticArray__complex128_int32_t_1_1_0)(T11));
              T27 = T12;
              _reallocate_682189(T26, &(T27), _ln, _fn);
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
      _reallocate_682974(T14, &(T15), _ln, _fn);
    } else {
      T16 = (((object)T11)->_cid == _e_SingleLocaleArithmeticArray_int32_t_int32_t_0_1_0);
      if (T16) {
        T17 = ((SingleLocaleArithmeticArray_int32_t_int32_t_0_1_0)(T11));
        T18 = T12;
        _reallocate_683374(T17, &(T18), _ln, _fn);
      } else {
        T19 = (((object)T11)->_cid == _e_SingleLocaleArithmeticArray__complex128_int64_t_0_1_0);
        if (T19) {
          T20 = ((SingleLocaleArithmeticArray__complex128_int64_t_0_1_0)(T11));
          T21 = T12;
          _reallocate_683774(T20, &(T21), _ln, _fn);
        } else {
          T22 = (((object)T11)->_cid == _e_SingleLocaleArithmeticArray__complex128_int64_t_0_1_1);
          if (T22) {
            T23 = ((SingleLocaleArithmeticArray__complex128_int64_t_0_1_1)(T11));
            T24 = T12;
            _reallocate_690506(T23, &(T24), _ln, _fn);
          } else {
            T25 = (((object)T11)->_cid == _e_SingleLocaleArithmeticArray__complex128_int32_t_1_1_0);
            if (T25) {
              T26 = ((SingleLocaleArithmeticArray__complex128_int32_t_1_1_0)(T11));
              T27 = T12;
              _reallocate_690906(T26, &(T27), _ln, _fn);
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
_array_int64_t__complex128_1_SingleLocaleArithmeticArray__complex128_int64_t_0_1_0 __construct__array_463782(int32_t rank, SingleLocaleArithmeticArray__complex128_int64_t_0_1_0 _value) {
  _array_int64_t__complex128_1_SingleLocaleArithmeticArray__complex128_int64_t_0_1_0 T1;
  T1._value = _value;
  return T1;
}

/* ChapelArray.chpl:321 */
_array_int32_t_locale_1_SingleLocaleArithmeticArray_locale_int32_t_0_1_0 _construct__array(int32_t rank, SingleLocaleArithmeticArray_locale_int32_t_0_1_0 _value) {
  _array_int32_t_locale_1_SingleLocaleArithmeticArray_locale_int32_t_0_1_0 T1;
  T1._value = _value;
  return T1;
}

/* ChapelArray.chpl:321 */
_array_int32_t__complex128_1_SingleLocaleArithmeticArray__complex128_int32_t_1_1_0 __construct__array_617491(int32_t rank, SingleLocaleArithmeticArray__complex128_int32_t_1_1_0 _value) {
  _array_int32_t__complex128_1_SingleLocaleArithmeticArray__complex128_int32_t_1_1_0 T1;
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
_array_int32_t_int32_t_1_SingleLocaleArithmeticArray_int32_t_int32_t_0_1_0 __construct__array_334753(int32_t rank, SingleLocaleArithmeticArray_int32_t_int32_t_0_1_0 _value) {
  _array_int32_t_int32_t_1_SingleLocaleArithmeticArray_int32_t_int32_t_0_1_0 T1;
  T1._value = _value;
  return T1;
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
_domain_1_SingleLocaleArithmeticDomain_1_int64_t_1 __dom_599569(_array_int64_t__complex128_1_SingleLocaleArithmeticArray__complex128_int64_t_0_1_1* const _this_599573, int32_t _ln, _string _fn) {
  _domain_1_SingleLocaleArithmeticDomain_1_int64_t_1 T4;
  SingleLocaleArithmeticArray__complex128_int64_t_0_1_1 T1 = NULL;
  SingleLocaleArithmeticDomain_1_int64_t_1 T2 = NULL;
  _domain_1_SingleLocaleArithmeticDomain_1_int64_t_1 T3;
  T1 = ((*_this_599573)._value);
  T2 = (T1->dom);
  T3 = __construct__domain_551628(1, T2, _ln, _fn);
  T4 = T3;
  return T4;
}

/* ChapelArray.chpl:357 */
_array_int64_t__complex128_1_SingleLocaleArithmeticArray__complex128_int64_t_0_1_1 _this_572389(_array_int64_t__complex128_1_SingleLocaleArithmeticArray__complex128_int64_t_0_1_0* const _this_572395, range_int64_t_bounded_1* const _e0_ranges, int32_t _ln, _string _fn) {
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
  T2 = __dom_470951(&((*_this_572395)), _ln, _fn);
  T3 = T2;
  T4 = _this_573440(&(T3), &(T1), _ln, _fn);
  T5 = (T4._value);
  T6 = ((*_this_572395)._value);
  T7 = _slice_585498(T6, T5, _ln, _fn);
  T8 = __construct__array_591640(1, T7);
  T9 = T8;
  return T9;
}

/* ChapelArray.chpl:357 */
_array_int64_t__complex128_1_SingleLocaleArithmeticArray__complex128_int64_t_0_1_1 _this_632341(_array_int64_t__complex128_1_SingleLocaleArithmeticArray__complex128_int64_t_0_1_0* const _this_632347, range_int32_t_bounded_1* const _e0_ranges, int32_t _ln, _string _fn) {
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
  T2 = __dom_470951(&((*_this_632347)), _ln, _fn);
  T3 = T2;
  T4 = _this_633639(&(T3), &(T1), _ln, _fn);
  T5 = (T4._value);
  T6 = ((*_this_632347)._value);
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
_array_int32_t__complex128_1_SingleLocaleArithmeticArray__complex128_int32_t_1_1_0 reindex(_array_int64_t__complex128_1_SingleLocaleArithmeticArray__complex128_int64_t_0_1_1* const _this_611251, _domain_1_SingleLocaleArithmeticDomain_1_int32_t_0* const d, int32_t _ln, _string _fn) {
  _domain_1_SingleLocaleArithmeticDomain_1_int32_t_0 T1;
  _array_int32_t__complex128_1_SingleLocaleArithmeticArray__complex128_int32_t_1_1_0 T6;
  SingleLocaleArithmeticDomain_1_int32_t_0 T2 = NULL;
  SingleLocaleArithmeticArray__complex128_int32_t_1_1_0 T4 = NULL;
  SingleLocaleArithmeticArray__complex128_int64_t_0_1_1 T3 = NULL;
  _array_int32_t__complex128_1_SingleLocaleArithmeticArray__complex128_int32_t_1_1_0 T5;
  T1 = (*d);
  T2 = (T1._value);
  T3 = ((*_this_611251)._value);
  T4 = _reindex_611480(T3, T2, _ln, _fn);
  T5 = __construct__array_617491(1, T4);
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

/* ChapelArray.chpl:431 */
_array_int64_t__complex128_1_SingleLocaleArithmeticArray__complex128_int64_t_0_1_1 __copy_599498(_array_int64_t__complex128_1_SingleLocaleArithmeticArray__complex128_int64_t_0_1_1* const a, int32_t _ln, _string _fn) {
  _array_int64_t__complex128_1_SingleLocaleArithmeticArray__complex128_int64_t_0_1_1 T88;
  _array_int64_t__complex128_1_SingleLocaleArithmeticArray__complex128_int64_t_0_1_1 T5;
  _domain_1_SingleLocaleArithmeticDomain_1_int64_t_1 T2;
  _array_int64_t__complex128_1_SingleLocaleArithmeticArray__complex128_int64_t_0_1_1 T1;
  _domain_1_SingleLocaleArithmeticDomain_1_int64_t_1 T3;
  __ArrayTypeInfo_709895 T4;
  _array_int64_t__complex128_1_SingleLocaleArithmeticArray__complex128_int64_t_0_1_1 T6;
  _domain_1_SingleLocaleArithmeticDomain_1_int64_t_1 T8;
  _array_int64_t__complex128_1_SingleLocaleArithmeticArray__complex128_int64_t_0_1_1 T7;
  _array_int64_t__complex128_1_SingleLocaleArithmeticArray__complex128_int64_t_0_1_1 T9;
  SingleLocaleArithmeticArray__complex128_int64_t_0_1_1 T11 = NULL;
  range_int64_t_bounded_1 T16;
  int64_t T22;
  SingleLocaleArithmeticArray__complex128_int64_t_0_1_1 T10 = NULL;
  SingleLocaleArithmeticDomain_1_int64_t_1 T12 = NULL;
  _tuple_1_range_int64_t_bounded_1 T13;
  range_int64_t_bounded_1 T14;
  range_int64_t_bounded_1 T15;
  int64_t T20;
  int32_t T17;
  chpl_bool T18;
  int64_t T19;
  int64_t T21;
  int64_t T23;
  chpl_bool T24;
  SingleLocaleArithmeticDomain_1_int64_t_1 T25 = NULL;
  range_int64_t_bounded_1 T27;
  _tuple_1_range_int64_t_bounded_1 T26;
  range_int64_t_bounded_1 T28;
  range_int64_t_bounded_1 T29;
  int64_t T35;
  int64_t T33;
  int32_t T30;
  chpl_bool T31;
  int64_t T32;
  int64_t T34;
  chpl_bool T41;
  int64_t T36;
  chpl_bool T37;
  chpl_bool T40;
  int64_t T38;
  chpl_bool T39;
  _ref__complex128 T57 = NULL;
  int64_t T42;
  _tuple_1_int64_t T43;
  int64_t T44;
  int64_t T45;
  _tuple_1_int64_t T46;
  int64_t T47;
  int64_t T48;
  _tuple_1_int32_t T49;
  int32_t T50;
  int64_t T51;
  int64_t T52;
  int64_t T53;
  int32_t T54;
  _ddata__complex128 T55 = NULL;
  _ref__complex128 T56 = NULL;
  SingleLocaleArithmeticArray__complex128_int64_t_0_1_1 T58 = NULL;
  int64_t T59;
  _tuple_1_int64_t T60;
  int64_t T61;
  int64_t T62;
  _tuple_1_int64_t T63;
  int64_t T64;
  int64_t T65;
  _tuple_1_int32_t T66;
  int32_t T67;
  int64_t T68;
  int64_t T69;
  int64_t T70;
  int32_t T71;
  _ddata__complex128 T72 = NULL;
  _ref__complex128 T73 = NULL;
  _complex128 T74;
  int32_t T75;
  int64_t T76;
  int64_t T77;
  int64_t T78;
  chpl_bool T79;
  int32_t T80;
  int64_t T81;
  int64_t T82;
  int64_t T83;
  chpl_bool T84;
  chpl_bool T87;
  int64_t T85;
  chpl_bool T86;
  T1 = (*a);
  T2 = __dom_599569(&(T1), _ln, _fn);
  T3 = __build_domain_600019(&(T2));
  T4 = __build_array_type_600051(&(T3));
  T5 = __init_598627(&(T4), _ln, _fn);
  T6 = T5;
  T7 = T5;
  T8 = __dom_599569(&(T7), _ln, _fn);
  T9 = (*a);
  T10 = (T9._value);
  T11 = T10;
  T12 = (T10->dom);
  T13 = (T12->ranges);
  T14 = (T13.x1);
  T15 = __copy_547955(&(T14));
  T16 = T15;
  T17 = (T15._stride);
  T18 = (T17>0);
  if (T18) {
    T19 = (T15._low);
    T20 = T19;
  } else {
    T21 = (T15._high);
    T20 = T21;
  }
  T22 = T20;
  T23 = (T15._low);
  T24 = (T23<=T20);
  if (T24) {
  } else {
  }
  T25 = (T8._value);
  T26 = (T25->ranges);
  T27 = (T26.x1);
  T28 = __copy_547955(&(T27));
  T29 = T28;
  T30 = (T28._stride);
  T31 = (T30>0);
  if (T31) {
    T32 = (T28._low);
    T33 = T32;
  } else {
    T34 = (T28._high);
    T33 = T34;
  }
  T35 = T33;
  T36 = (T28._low);
  T37 = (T36<=T33);
  if (T37) {
    T38 = (T28._high);
    T39 = (T33<=T38);
    T40 = T39;
  } else {
    T40 = false;
  }
  T41 = T40;
  while (T41) {
    T42 = (T11->origin);
    T43 = (T11->off);
    T44 = (T43.x1);
    T45 = (T22-T44);
    T46 = (T11->blk);
    T47 = (T46.x1);
    T48 = (T45*T47);
    T49 = (T11->str);
    T50 = (T49.x1);
    T51 = ((int64_t)(T50));
    T52 = (T48/T51);
    T53 = (T42+T52);
    T54 = ((int32_t)(T53));
    T55 = (T11->data);
    T56 = _ARRAY_GET(T55, T54);
    T57 = T56;
    T58 = (T6._value);
    T59 = (T58->origin);
    T60 = (T58->off);
    T61 = (T60.x1);
    T62 = (T35-T61);
    T63 = (T58->blk);
    T64 = (T63.x1);
    T65 = (T62*T64);
    T66 = (T58->str);
    T67 = (T66.x1);
    T68 = ((int64_t)(T67));
    T69 = (T65/T68);
    T70 = (T59+T69);
    T71 = ((int32_t)(T70));
    T72 = (T58->data);
    T73 = _ARRAY_GET(T72, T71);
    T74 = *(T57);
    (*T73) = T74;
    T75 = (T16._stride);
    T76 = ((int64_t)(T75));
    T77 = (T22+T76);
    T22 = T77;
    T78 = (T16._low);
    T79 = (T78<=T77);
    if (T79) {
    } else {
    }
    T80 = (T29._stride);
    T81 = ((int64_t)(T80));
    T82 = (T35+T81);
    T35 = T82;
    T83 = (T29._low);
    T84 = (T83<=T82);
    if (T84) {
      T85 = (T29._high);
      T86 = (T82<=T85);
      T87 = T86;
    } else {
      T87 = false;
    }
    T41 = T87;
  }
  T88 = T6;
  return T88;
}

/* ChapelArray.chpl:445 */
_array_int64_t__complex128_1_SingleLocaleArithmeticArray__complex128_int64_t_0_1_0 __init_470219(__ArrayTypeInfo_709668* const t, int32_t _ln, _string _fn) {
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
_array_int32_t_int32_t_1_SingleLocaleArithmeticArray_int32_t_int32_t_0_1_0 __init_341190(__ArrayTypeInfo_709435* const t, int32_t _ln, _string _fn) {
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
_array_int64_t__complex128_1_SingleLocaleArithmeticArray__complex128_int64_t_0_1_1 __init_598627(__ArrayTypeInfo_709895* const t, int32_t _ln, _string _fn) {
  _array_int64_t__complex128_1_SingleLocaleArithmeticArray__complex128_int64_t_0_1_1 T5;
  _array_int64_t__complex128_1_SingleLocaleArithmeticArray__complex128_int64_t_0_1_1 T4;
  SingleLocaleArithmeticDomain_1_int64_t_1 T1 = NULL;
  _domain_1_SingleLocaleArithmeticDomain_1_int64_t_1 T2;
  _domain_1_SingleLocaleArithmeticDomain_1_int64_t_1 T3;
  T1 = ((*t).dom);
  T2 = __wrapDomain_598668(T1, _ln, _fn);
  T3 = T2;
  T4 = _buildArray_598729(&(T3), _ln, _fn);
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

