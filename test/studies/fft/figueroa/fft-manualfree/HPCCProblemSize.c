/* probSize.chpl:1 */
void __init_HPCCProblemSize(void) {
  chpl_bool T1;
  int32_t T4;
  chpl_bool T2;
  chpl_bool T3;
  _string T5;
  int32_t T6;
  T1 = (!__run_HPCCProblemSize_firsttime25);
  if (T1) {
    goto _end___init_HPCCProblemSize;
  }
  __run_HPCCProblemSize_firsttime25 = false;
  if (__run_Memory_firsttime20) {
    __init_Memory();
  }
  __run_Memory_firsttime20 = false;
  if (__run_Types_firsttime14) {
    __init_Types();
  }
  __run_Types_firsttime14 = false;
  T2 = _config_has_value("memRatio", "HPCCProblemSize");
  T3 = (!T2);
  if (T3) {
    T4 = 4;
  } else {
    T5 = _config_get_value("memRatio", "HPCCProblemSize");
    T6 = _string_to_int32_t(T5, 0, "<command line setting of 'memRatio'>");
    T4 = T6;
  }
  memRatio = T4;
  _end___init_HPCCProblemSize:;
  return;
}

/* probSize.chpl:6 */
int64_t computeProblemSize(int32_t numArrays, chpl_bool returnLog2) {
  int64_t T33;
  _ic_physicalMemory__ref__array_int32_t_locale_1_SingleLocaleArithmeticArray_locale_int32_t_0_1_0 T1 = NULL;
  _sum_int64_t T3 = NULL;
  _sum_int64_t T2 = NULL;
  int64_t T4;
  int64_t T5;
  int64_t T6;
  int32_t T7;
  int64_t T10;
  int64_t T8;
  int64_t T9;
  int32_t T12;
  int32_t T11;
  int32_t T13;
  int64_t T14;
  int64_t T15;
  int64_t T16;
  chpl_bool T17;
  int64_t T18;
  int64_t T19;
  int32_t T20;
  _ic_physicalMemory__ref__array_int32_t_locale_1_SingleLocaleArithmeticArray_locale_int32_t_0_1_0 T21 = NULL;
  _min_int64_t T24 = NULL;
  _min_int64_t T22 = NULL;
  int64_t T23;
  int64_t T25;
  int64_t T26;
  int64_t T27;
  int64_t T28;
  int64_t T29;
  chpl_bool T30;
  int64_t T32;
  int64_t T31;
  T1 = (_ic_physicalMemory__ref__array_int32_t_locale_1_SingleLocaleArithmeticArray_locale_int32_t_0_1_0)chpl_alloc(sizeof(__ic_physicalMemory__ref__array_int32_t_locale_1_SingleLocaleArithmeticArray_locale_int32_t_0_1_0), "iterator class", 7, "test/release/examples/hpcc/probSize.chpl");
  T1->_0_this = Locales;
  T1->_1_unit = MemUnits_Bytes;
  T1->_2__formal_tmp_unit = MemUnits_Bytes;
  T1->_3__ret_physicalMemory = 0;
  T1->_4_this = nil;
  T1->_5_i = 0;
  T1->_6__tmp = 0;
  T1->_7__tmp = 0;
  T2 = (_sum_int64_t)chpl_alloc(sizeof(__sum_int64_t), "instance of class _unknown", 7, "test/release/examples/hpcc/probSize.chpl");
  ((object)T2)->_cid = _e__sum_int64_t;
  T2->value = 0;
  _sum_type();
  T2->value = 0;
  T3 = _construct__sum(0, T2, 7, "test/release/examples/hpcc/probSize.chpl");
  T4 = _reduce(T3, T1, 7, "test/release/examples/hpcc/probSize.chpl");
  T5 = ((int64_t)(memRatio));
  T6 = (T4/T5);
  T7 = (numArrays*16);
  T8 = ((int64_t)(T7));
  T9 = (T6/T8);
  T10 = T9;
  T11 = _log2_76322(T9, 13, "test/release/examples/hpcc/probSize.chpl");
  T12 = T11;
  if (returnLog2) {
    T13 = pow(2, T11);
    T14 = ((int64_t)(T13));
    T10 = T14;
    T15 = ((int64_t)(T7));
    T16 = (T14*T15);
    T17 = (T16<=T6);
    if (T17) {
      T18 = ((int64_t)(2));
      T19 = (T14*T18);
      T10 = T19;
      T20 = (T11+1);
      T12 = T20;
    }
  }
  T21 = (_ic_physicalMemory__ref__array_int32_t_locale_1_SingleLocaleArithmeticArray_locale_int32_t_0_1_0)chpl_alloc(sizeof(__ic_physicalMemory__ref__array_int32_t_locale_1_SingleLocaleArithmeticArray_locale_int32_t_0_1_0), "iterator class", 22, "test/release/examples/hpcc/probSize.chpl");
  T21->_0_this = Locales;
  T21->_1_unit = MemUnits_Bytes;
  T21->_2__formal_tmp_unit = MemUnits_Bytes;
  T21->_3__ret_physicalMemory = 0;
  T21->_4_this = nil;
  T21->_5_i = 0;
  T21->_6__tmp = 0;
  T21->_7__tmp = 0;
  T22 = (_min_int64_t)chpl_alloc(sizeof(__min_int64_t), "instance of class _unknown", 22, "test/release/examples/hpcc/probSize.chpl");
  ((object)T22)->_cid = _e__min_int64_t;
  T22->value = 0;
  T23 = max();
  T22->value = T23;
  T24 = _construct__min(T23, T22, 22, "test/release/examples/hpcc/probSize.chpl");
  T25 = __reduce_396373(T24, T21, 22, "test/release/examples/hpcc/probSize.chpl");
  T26 = ((int64_t)(T7));
  T27 = (T10*T26);
  T28 = ((int64_t)(numLocales));
  T29 = (T27/T28);
  T30 = (T29>T25);
  if (T30) {
    halt("System is too heterogeneous: blocked data won't fit into memory", 24, "test/release/examples/hpcc/probSize.chpl");
  }
  if (returnLog2) {
    T31 = ((int64_t)(T12));
    T32 = T31;
  } else {
    T32 = T10;
  }
  T33 = T32;
  return T33;
}

/* probSize.chpl:30 */
void printProblemSize(int32_t numArrays, int64_t problemSize) {
  int64_t T1;
  int64_t T2;
  _real64 T3;
  _real64 T4;
  _real64 T5;
  _real64 T6;
  _real64 T7;
  int32_t T8;
  int64_t T9;
  int64_t T10;
  int64_t T11;
  chpl_bool T12;
  T1 = ((int64_t)(16));
  T2 = (problemSize*T1);
  T3 = ((_real64)(T2));
  T4 = ((_real64)(numArrays));
  T5 = (T4*T3);
  T6 = ((_real64)(1073741824));
  T7 = (T5/T6);
  T8 = _log2_76322(problemSize, 33, "test/release/examples/hpcc/probSize.chpl");
  T9 = ((int64_t)(T8));
  _write_421278("Problem size = ", problemSize, 35, "test/release/examples/hpcc/probSize.chpl");
  T10 = ((int64_t)(2));
  T11 = pow(T10, T9);
  T12 = (T11==problemSize);
  if (T12) {
    _write_424291(" (2**", T9, ")", 37, "test/release/examples/hpcc/probSize.chpl");
  }
  _writeln_18890(39, "test/release/examples/hpcc/probSize.chpl");
  _writeln_425379("Bytes per array = ", T2, 40, "test/release/examples/hpcc/probSize.chpl");
  _writeln_425807("Total memory required (GB) = ", T7, 41, "test/release/examples/hpcc/probSize.chpl");
  return;
}

