/* SingleLocaleAssociative.chpl:1 */
void __init_SingleLocaleAssociative(int32_t _ln, _string _fn) {
  chpl_bool T1;
  _array_int32_t_int32_t_1_SingleLocaleArithmeticArray_int32_t_int32_t_0_1_0 T5;
  range_int32_t_bounded_0 T2;
  _domain_1_SingleLocaleArithmeticDomain_1_int32_t_0 T3;
  __ArrayTypeInfo_700380 T4;
  _tuple_27_int32_t_int32_t_int32_t_int32_t_int32_t_int32_t_int32_t_int32_t_int32_t_int32_t_int32_t_int32_t_int32_t_int32_t_int32_t_int32_t_int32_t_int32_t_int32_t_int32_t_int32_t_int32_t_int32_t_int32_t_int32_t_int32_t_int32_t T6;
  T1 = (!__run_SingleLocaleAssociative_firsttime10);
  if (T1) {
    goto _end___init_SingleLocaleAssociative;
  }
  __run_SingleLocaleAssociative_firsttime10 = false;
  if (__run_Sort_firsttime22) {
    __init_Sort();
  }
  __run_Sort_firsttime22 = false;
  T2 = _build_range(0, 26);
  T3 = _build_domain(&(T2), _ln, _fn);
  T4 = __build_array_type_322184(&(T3));
  T5 = __init_341190(&(T4), _ln, _fn);
  T6.x1 = 23;
  T6.x2 = 53;
  T6.x3 = 97;
  T6.x4 = 193;
  T6.x5 = 389;
  T6.x6 = 769;
  T6.x7 = 1543;
  T6.x8 = 3079;
  T6.x9 = 6151;
  T6.x10 = 12289;
  T6.x11 = 24593;
  T6.x12 = 49157;
  T6.x13 = 98317;
  T6.x14 = 196613;
  T6.x15 = 393241;
  T6.x16 = 786433;
  T6.x17 = 1572869;
  T6.x18 = 3145739;
  T6.x19 = 6291469;
  T6.x20 = 12582917;
  T6.x21 = 25165843;
  T6.x22 = 50331653;
  T6.x23 = 100663319;
  T6.x24 = 201326611;
  T6.x25 = 402653189;
  T6.x26 = 805306457;
  T6.x27 = 1610612741;
  T5 = __EQUAL__344534(&(T5), &(T6), _ln, _fn);
  chpl_primes = T5;
  _end___init_SingleLocaleAssociative:;
  return;
}

