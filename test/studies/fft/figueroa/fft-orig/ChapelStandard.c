/* ChapelStandard.chpl:1 */
void __init_ChapelStandard(int32_t _ln, _string _fn) {
  chpl_bool T1;
  T1 = (!__run_ChapelStandard_firsttime1);
  if (T1) {
    goto _end___init_ChapelStandard;
  }
  __run_ChapelStandard_firsttime1 = false;
  if (__run_ChapelBase_firsttime0) {
    __init_ChapelBase(_ln, _fn);
  }
  __run_ChapelBase_firsttime0 = false;
  if (__run_ChapelIO_firsttime3) {
    __init_ChapelIO(_ln, _fn);
  }
  __run_ChapelIO_firsttime3 = false;
  if (__run_ChapelTuple_firsttime4) {
    __init_ChapelTuple();
  }
  __run_ChapelTuple_firsttime4 = false;
  if (__run_ChapelRange_firsttime6) {
    __init_ChapelRange();
  }
  __run_ChapelRange_firsttime6 = false;
  if (__run_ChapelReduce_firsttime5) {
    __init_ChapelReduce();
  }
  __run_ChapelReduce_firsttime5 = false;
  if (__run_ChapelArray_firsttime7) {
    __init_ChapelArray();
  }
  __run_ChapelArray_firsttime7 = false;
  if (__run_ChapelLocale_firsttime2) {
    __init_ChapelLocale(_ln, _fn);
  }
  __run_ChapelLocale_firsttime2 = false;
  if (__run_SingleLocaleArithmetic_firsttime9) {
    __init_SingleLocaleArithmetic(_ln, _fn);
  }
  __run_SingleLocaleArithmetic_firsttime9 = false;
  if (__run_SingleLocaleAssociative_firsttime10) {
    __init_SingleLocaleAssociative(_ln, _fn);
  }
  __run_SingleLocaleAssociative_firsttime10 = false;
  if (__run_SingleLocaleSparse_firsttime11) {
    __init_SingleLocaleSparse();
  }
  __run_SingleLocaleSparse_firsttime11 = false;
  if (__run_SingleLocaleEnum_firsttime12) {
    __init_SingleLocaleEnum();
  }
  __run_SingleLocaleEnum_firsttime12 = false;
  if (__run_SingleLocaleOpaque_firsttime13) {
    __init_SingleLocaleOpaque();
  }
  __run_SingleLocaleOpaque_firsttime13 = false;
  if (__run_ChapelUtil_firsttime8) {
    __init_ChapelUtil();
  }
  __run_ChapelUtil_firsttime8 = false;
  if (__run_Types_firsttime14) {
    __init_Types();
  }
  __run_Types_firsttime14 = false;
  if (__run_Math_firsttime15) {
    __init_Math();
  }
  __run_Math_firsttime15 = false;
  _end___init_ChapelStandard:;
  return;
}

