#line 253 "/Users/iainmoncrief/Documents/chapel/modules/packages/Sort.chpl"
/* Sort.chpl:253 */
#line 253 "/Users/iainmoncrief/Documents/chapel/modules/packages/Sort.chpl"
static void chpl__init_Sort(int64_t _ln_chpl,
#line 253 "/Users/iainmoncrief/Documents/chapel/modules/packages/Sort.chpl"
                            int32_t _fn_chpl) {
#line 253 "/Users/iainmoncrief/Documents/chapel/modules/packages/Sort.chpl"
  _ref_int32_t_chpl refIndentLevel_chpl = NULL;
#line 266 "/Users/iainmoncrief/Documents/chapel/modules/packages/Sort.chpl"
  DefaultComparator_chpl call_tmp_chpl6;
#line 266 "/Users/iainmoncrief/Documents/chapel/modules/packages/Sort.chpl"
  DefaultComparator_chpl initTemp_chpl;
#line 277 "/Users/iainmoncrief/Documents/chapel/modules/packages/Sort.chpl"
  ReverseComparator_DefaultComparator_chpl call_tmp_chpl7;
#line 277 "/Users/iainmoncrief/Documents/chapel/modules/packages/Sort.chpl"
  ReverseComparator_DefaultComparator_chpl initTemp_chpl2;
#line 253 "/Users/iainmoncrief/Documents/chapel/modules/packages/Sort.chpl"
  if (chpl__init_Sort_p) /* ZLINE: 253 /Users/iainmoncrief/Documents/chapel/modules/packages/Sort.chpl */
#line 253 "/Users/iainmoncrief/Documents/chapel/modules/packages/Sort.chpl"
  {
#line 253 "/Users/iainmoncrief/Documents/chapel/modules/packages/Sort.chpl"
    goto _exit_chpl__init_Sort_chpl;
#line 253 "/Users/iainmoncrief/Documents/chapel/modules/packages/Sort.chpl"
  }
#line 253 "/Users/iainmoncrief/Documents/chapel/modules/packages/Sort.chpl"
  printModuleInit_chpl("%*s\n", "Sort", INT64(4), _ln_chpl, _fn_chpl);
#line 253 "/Users/iainmoncrief/Documents/chapel/modules/packages/Sort.chpl"
  refIndentLevel_chpl = &moduleInitLevel_chpl;
#line 253 "/Users/iainmoncrief/Documents/chapel/modules/packages/Sort.chpl"
  *(refIndentLevel_chpl) += INT64(1);
#line 253 "/Users/iainmoncrief/Documents/chapel/modules/packages/Sort.chpl"
  chpl__init_Sort_p = UINT8(true);
#line 253 "/Users/iainmoncrief/Documents/chapel/modules/packages/Sort.chpl"
  {
#line 253 "/Users/iainmoncrief/Documents/chapel/modules/packages/Sort.chpl"
    chpl__init_List(_ln_chpl, _fn_chpl);
#line 253 "/Users/iainmoncrief/Documents/chapel/modules/packages/Sort.chpl"
    chpl__init_Reflection(_ln_chpl, _fn_chpl);
#line 253 "/Users/iainmoncrief/Documents/chapel/modules/packages/Sort.chpl"
    chpl__init_CTypes(_ln_chpl, _fn_chpl);
#line 253 "/Users/iainmoncrief/Documents/chapel/modules/packages/Sort.chpl"
    chpl__init_POSIX(_ln_chpl, _fn_chpl);
#line 253 "/Users/iainmoncrief/Documents/chapel/modules/packages/Sort.chpl"
    chpl__init_MSBRadixSort(_ln_chpl, _fn_chpl);
#line 253 "/Users/iainmoncrief/Documents/chapel/modules/packages/Sort.chpl"
    chpl__init_QuickSort(_ln_chpl, _fn_chpl);
#line 253 "/Users/iainmoncrief/Documents/chapel/modules/packages/Sort.chpl"
  }
#line 253 "/Users/iainmoncrief/Documents/chapel/modules/packages/Sort.chpl"
  chpl_addModule("Sort", ((c_fn_ptr)(chpl__deinit_Sort)), _ln_chpl, _fn_chpl);
#line 266 "/Users/iainmoncrief/Documents/chapel/modules/packages/Sort.chpl"
  init_chpl223(&initTemp_chpl);
#line 266 "/Users/iainmoncrief/Documents/chapel/modules/packages/Sort.chpl"
  call_tmp_chpl6 = initTemp_chpl;
#line 266 "/Users/iainmoncrief/Documents/chapel/modules/packages/Sort.chpl"
  defaultComparator_chpl = call_tmp_chpl6;
#line 277 "/Users/iainmoncrief/Documents/chapel/modules/packages/Sort.chpl"
  init_chpl224(&initTemp_chpl2);
#line 277 "/Users/iainmoncrief/Documents/chapel/modules/packages/Sort.chpl"
  call_tmp_chpl7 = initTemp_chpl2;
#line 277 "/Users/iainmoncrief/Documents/chapel/modules/packages/Sort.chpl"
  reverseComparator_chpl = call_tmp_chpl7;
#line 253 "/Users/iainmoncrief/Documents/chapel/modules/packages/Sort.chpl"
  *(refIndentLevel_chpl) -= INT64(1);
#line 253 "/Users/iainmoncrief/Documents/chapel/modules/packages/Sort.chpl"
  _exit_chpl__init_Sort_chpl:;
#line 253 "/Users/iainmoncrief/Documents/chapel/modules/packages/Sort.chpl"
  return;
#line 253 "/Users/iainmoncrief/Documents/chapel/modules/packages/Sort.chpl"
}

#line 253 "/Users/iainmoncrief/Documents/chapel/modules/packages/Sort.chpl"
/* Sort.chpl:253 */
#line 253 "/Users/iainmoncrief/Documents/chapel/modules/packages/Sort.chpl"
static void chpl__deinit_Sort(void) {
#line 253 "/Users/iainmoncrief/Documents/chapel/modules/packages/Sort.chpl"
  return;
#line 253 "/Users/iainmoncrief/Documents/chapel/modules/packages/Sort.chpl"
}

#line 3530 "/Users/iainmoncrief/Documents/chapel/modules/packages/Sort.chpl"
/* Sort.chpl:3530 */
#line 3530 "/Users/iainmoncrief/Documents/chapel/modules/packages/Sort.chpl"
static void init_chpl223(DefaultComparator_chpl * this_chpl31) {
#line 3530 "/Users/iainmoncrief/Documents/chapel/modules/packages/Sort.chpl"
  return;
#line 3530 "/Users/iainmoncrief/Documents/chapel/modules/packages/Sort.chpl"
}

#line 3530 "/Users/iainmoncrief/Documents/chapel/modules/packages/Sort.chpl"
/* Sort.chpl:3530 */
#line 3530 "/Users/iainmoncrief/Documents/chapel/modules/packages/Sort.chpl"
static void init_ASSIGN__chpl26(DefaultComparator_chpl * this_chpl31,
#line 3530 "/Users/iainmoncrief/Documents/chapel/modules/packages/Sort.chpl"
                                DefaultComparator_chpl * other_chpl) {
#line 3530 "/Users/iainmoncrief/Documents/chapel/modules/packages/Sort.chpl"
  return;
#line 3530 "/Users/iainmoncrief/Documents/chapel/modules/packages/Sort.chpl"
}

#line 3688 "/Users/iainmoncrief/Documents/chapel/modules/packages/Sort.chpl"
/* Sort.chpl:3688 */
#line 3688 "/Users/iainmoncrief/Documents/chapel/modules/packages/Sort.chpl"
static void init_chpl224(ReverseComparator_DefaultComparator_chpl * this_chpl31) {
#line 3689 "/Users/iainmoncrief/Documents/chapel/modules/packages/Sort.chpl"
  DefaultComparator_chpl local_defaultComparator_chpl;
#line 3689 "/Users/iainmoncrief/Documents/chapel/modules/packages/Sort.chpl"
  DefaultComparator_chpl comparator_chpl;
#line 3689 "/Users/iainmoncrief/Documents/chapel/modules/packages/Sort.chpl"
  local_defaultComparator_chpl = defaultComparator_chpl;
#line 3689 "/Users/iainmoncrief/Documents/chapel/modules/packages/Sort.chpl"
  init_ASSIGN__chpl26(&comparator_chpl, &local_defaultComparator_chpl);
#line 3689 "/Users/iainmoncrief/Documents/chapel/modules/packages/Sort.chpl"
  (this_chpl31)->comparator = comparator_chpl;
#line 3688 "/Users/iainmoncrief/Documents/chapel/modules/packages/Sort.chpl"
  return;
#line 3688 "/Users/iainmoncrief/Documents/chapel/modules/packages/Sort.chpl"
}

