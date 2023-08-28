#line 682 "/Users/iainmoncrief/Documents/chapel/modules/packages/Sort.chpl"
/* Sort.chpl:682 */
#line 682 "/Users/iainmoncrief/Documents/chapel/modules/packages/Sort.chpl"
static void chpl__init_InsertionSort(int64_t _ln_chpl,
#line 682 "/Users/iainmoncrief/Documents/chapel/modules/packages/Sort.chpl"
                                     int32_t _fn_chpl) {
#line 682 "/Users/iainmoncrief/Documents/chapel/modules/packages/Sort.chpl"
  _ref_int32_t_chpl refIndentLevel_chpl = NULL;
#line 682 "/Users/iainmoncrief/Documents/chapel/modules/packages/Sort.chpl"
  if (chpl__init_InsertionSort_p) /* ZLINE: 682 /Users/iainmoncrief/Documents/chapel/modules/packages/Sort.chpl */
#line 682 "/Users/iainmoncrief/Documents/chapel/modules/packages/Sort.chpl"
  {
#line 682 "/Users/iainmoncrief/Documents/chapel/modules/packages/Sort.chpl"
    goto _exit_chpl__init_InsertionSort_chpl;
#line 682 "/Users/iainmoncrief/Documents/chapel/modules/packages/Sort.chpl"
  }
#line 682 "/Users/iainmoncrief/Documents/chapel/modules/packages/Sort.chpl"
  printModuleInit_chpl("%*s\n", "InsertionSort", INT64(13), _ln_chpl, _fn_chpl);
#line 682 "/Users/iainmoncrief/Documents/chapel/modules/packages/Sort.chpl"
  refIndentLevel_chpl = &moduleInitLevel_chpl;
#line 682 "/Users/iainmoncrief/Documents/chapel/modules/packages/Sort.chpl"
  *(refIndentLevel_chpl) += INT64(1);
#line 682 "/Users/iainmoncrief/Documents/chapel/modules/packages/Sort.chpl"
  chpl__init_InsertionSort_p = UINT8(true);
#line 682 "/Users/iainmoncrief/Documents/chapel/modules/packages/Sort.chpl"
  {
#line 682 "/Users/iainmoncrief/Documents/chapel/modules/packages/Sort.chpl"
    chpl__init_Sort(_ln_chpl, _fn_chpl);
#line 682 "/Users/iainmoncrief/Documents/chapel/modules/packages/Sort.chpl"
    chpl__init_Sort(_ln_chpl, _fn_chpl);
#line 682 "/Users/iainmoncrief/Documents/chapel/modules/packages/Sort.chpl"
    chpl__init_ShallowCopy(_ln_chpl, _fn_chpl);
#line 682 "/Users/iainmoncrief/Documents/chapel/modules/packages/Sort.chpl"
  }
#line 682 "/Users/iainmoncrief/Documents/chapel/modules/packages/Sort.chpl"
  *(refIndentLevel_chpl) -= INT64(1);
#line 682 "/Users/iainmoncrief/Documents/chapel/modules/packages/Sort.chpl"
  _exit_chpl__init_InsertionSort_chpl:;
#line 682 "/Users/iainmoncrief/Documents/chapel/modules/packages/Sort.chpl"
  return;
#line 682 "/Users/iainmoncrief/Documents/chapel/modules/packages/Sort.chpl"
}

