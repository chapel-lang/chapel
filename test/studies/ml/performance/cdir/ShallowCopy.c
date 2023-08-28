#line 1861 "/Users/iainmoncrief/Documents/chapel/modules/packages/Sort.chpl"
/* Sort.chpl:1861 */
#line 1861 "/Users/iainmoncrief/Documents/chapel/modules/packages/Sort.chpl"
static void chpl__init_ShallowCopy(int64_t _ln_chpl,
#line 1861 "/Users/iainmoncrief/Documents/chapel/modules/packages/Sort.chpl"
                                   int32_t _fn_chpl) {
#line 1861 "/Users/iainmoncrief/Documents/chapel/modules/packages/Sort.chpl"
  _ref_int32_t_chpl refIndentLevel_chpl = NULL;
#line 1861 "/Users/iainmoncrief/Documents/chapel/modules/packages/Sort.chpl"
  if (chpl__init_ShallowCopy_p) /* ZLINE: 1861 /Users/iainmoncrief/Documents/chapel/modules/packages/Sort.chpl */
#line 1861 "/Users/iainmoncrief/Documents/chapel/modules/packages/Sort.chpl"
  {
#line 1861 "/Users/iainmoncrief/Documents/chapel/modules/packages/Sort.chpl"
    goto _exit_chpl__init_ShallowCopy_chpl;
#line 1861 "/Users/iainmoncrief/Documents/chapel/modules/packages/Sort.chpl"
  }
#line 1861 "/Users/iainmoncrief/Documents/chapel/modules/packages/Sort.chpl"
  printModuleInit_chpl("%*s\n", "ShallowCopy", INT64(11), _ln_chpl, _fn_chpl);
#line 1861 "/Users/iainmoncrief/Documents/chapel/modules/packages/Sort.chpl"
  refIndentLevel_chpl = &moduleInitLevel_chpl;
#line 1861 "/Users/iainmoncrief/Documents/chapel/modules/packages/Sort.chpl"
  *(refIndentLevel_chpl) += INT64(1);
#line 1861 "/Users/iainmoncrief/Documents/chapel/modules/packages/Sort.chpl"
  chpl__init_ShallowCopy_p = UINT8(true);
#line 1861 "/Users/iainmoncrief/Documents/chapel/modules/packages/Sort.chpl"
  {
#line 1861 "/Users/iainmoncrief/Documents/chapel/modules/packages/Sort.chpl"
    chpl__init_Sort(_ln_chpl, _fn_chpl);
#line 1861 "/Users/iainmoncrief/Documents/chapel/modules/packages/Sort.chpl"
    chpl__init_CTypes(_ln_chpl, _fn_chpl);
#line 1861 "/Users/iainmoncrief/Documents/chapel/modules/packages/Sort.chpl"
    chpl__init_POSIX(_ln_chpl, _fn_chpl);
#line 1861 "/Users/iainmoncrief/Documents/chapel/modules/packages/Sort.chpl"
  }
#line 1861 "/Users/iainmoncrief/Documents/chapel/modules/packages/Sort.chpl"
  *(refIndentLevel_chpl) -= INT64(1);
#line 1861 "/Users/iainmoncrief/Documents/chapel/modules/packages/Sort.chpl"
  _exit_chpl__init_ShallowCopy_chpl:;
#line 1861 "/Users/iainmoncrief/Documents/chapel/modules/packages/Sort.chpl"
  return;
#line 1861 "/Users/iainmoncrief/Documents/chapel/modules/packages/Sort.chpl"
}

