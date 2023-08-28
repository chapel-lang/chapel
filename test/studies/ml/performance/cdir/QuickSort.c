#line 1074 "/Users/iainmoncrief/Documents/chapel/modules/packages/Sort.chpl"
/* Sort.chpl:1074 */
#line 1074 "/Users/iainmoncrief/Documents/chapel/modules/packages/Sort.chpl"
static void chpl__init_QuickSort(int64_t _ln_chpl,
#line 1074 "/Users/iainmoncrief/Documents/chapel/modules/packages/Sort.chpl"
                                 int32_t _fn_chpl) {
#line 1074 "/Users/iainmoncrief/Documents/chapel/modules/packages/Sort.chpl"
  _ref_int32_t_chpl refIndentLevel_chpl = NULL;
#line 1074 "/Users/iainmoncrief/Documents/chapel/modules/packages/Sort.chpl"
  if (chpl__init_QuickSort_p) /* ZLINE: 1074 /Users/iainmoncrief/Documents/chapel/modules/packages/Sort.chpl */
#line 1074 "/Users/iainmoncrief/Documents/chapel/modules/packages/Sort.chpl"
  {
#line 1074 "/Users/iainmoncrief/Documents/chapel/modules/packages/Sort.chpl"
    goto _exit_chpl__init_QuickSort_chpl;
#line 1074 "/Users/iainmoncrief/Documents/chapel/modules/packages/Sort.chpl"
  }
#line 1074 "/Users/iainmoncrief/Documents/chapel/modules/packages/Sort.chpl"
  printModuleInit_chpl("%*s\n", "QuickSort", INT64(9), _ln_chpl, _fn_chpl);
#line 1074 "/Users/iainmoncrief/Documents/chapel/modules/packages/Sort.chpl"
  refIndentLevel_chpl = &moduleInitLevel_chpl;
#line 1074 "/Users/iainmoncrief/Documents/chapel/modules/packages/Sort.chpl"
  *(refIndentLevel_chpl) += INT64(1);
#line 1074 "/Users/iainmoncrief/Documents/chapel/modules/packages/Sort.chpl"
  chpl__init_QuickSort_p = UINT8(true);
#line 1074 "/Users/iainmoncrief/Documents/chapel/modules/packages/Sort.chpl"
  {
#line 1074 "/Users/iainmoncrief/Documents/chapel/modules/packages/Sort.chpl"
    chpl__init_Sort(_ln_chpl, _fn_chpl);
#line 1074 "/Users/iainmoncrief/Documents/chapel/modules/packages/Sort.chpl"
    chpl__init_Sort(_ln_chpl, _fn_chpl);
#line 1074 "/Users/iainmoncrief/Documents/chapel/modules/packages/Sort.chpl"
    chpl__init_ShallowCopy(_ln_chpl, _fn_chpl);
#line 1074 "/Users/iainmoncrief/Documents/chapel/modules/packages/Sort.chpl"
    chpl__init_InsertionSort(_ln_chpl, _fn_chpl);
#line 1074 "/Users/iainmoncrief/Documents/chapel/modules/packages/Sort.chpl"
  }
#line 1074 "/Users/iainmoncrief/Documents/chapel/modules/packages/Sort.chpl"
  *(refIndentLevel_chpl) -= INT64(1);
#line 1074 "/Users/iainmoncrief/Documents/chapel/modules/packages/Sort.chpl"
  _exit_chpl__init_QuickSort_chpl:;
#line 1074 "/Users/iainmoncrief/Documents/chapel/modules/packages/Sort.chpl"
  return;
#line 1074 "/Users/iainmoncrief/Documents/chapel/modules/packages/Sort.chpl"
}

