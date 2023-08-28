#line 30 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
/* LocaleModelHelpMem.chpl:30 */
#line 30 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
static void chpl__init_LocaleModelHelpMem(int64_t _ln_chpl,
#line 30 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
                                          int32_t _fn_chpl) {
#line 30 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  _ref_int32_t_chpl refIndentLevel_chpl = NULL;
#line 30 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  if (chpl__init_LocaleModelHelpMem_p) /* ZLINE: 30 /Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl */
#line 30 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  {
#line 30 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
    goto _exit_chpl__init_LocaleModelHelpMem_chpl;
#line 30 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  }
#line 30 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  printModuleInit_chpl("%*s\n", "LocaleModelHelpMem", INT64(18), _ln_chpl, _fn_chpl);
#line 30 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  refIndentLevel_chpl = &moduleInitLevel_chpl;
#line 30 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  *(refIndentLevel_chpl) += INT64(1);
#line 30 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  chpl__init_LocaleModelHelpMem_p = UINT8(true);
#line 30 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  {
#line 30 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
    chpl__init_CTypes(_ln_chpl, _fn_chpl);
#line 30 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  }
#line 30 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  *(refIndentLevel_chpl) -= INT64(1);
#line 30 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  _exit_chpl__init_LocaleModelHelpMem_chpl:;
#line 30 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  return;
#line 30 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
}

#line 53 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
/* LocaleModelHelpMem.chpl:53 */
#line 53 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
static void * chpl_here_alloc(int64_t size_chpl9,
#line 53 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
                              int16_t md_chpl,
#line 53 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
                              int64_t _ln_chpl,
#line 53 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
                              int32_t _fn_chpl) {
#line 57 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  _owned_IllegalArgumentError_chpl2 call_tmp_chpl6;
#line 57 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  _owned_IllegalArgumentError_chpl2 ret_tmp_chpl;
#line 57 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  _ref__owned_IllegalArgumentError_chpl i_arg_chpl = NULL;
#line 57 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  IllegalArgumentError_chpl call_tmp_chpl7 = NULL;
#line 57 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  string_chpl call_tmp_chpl8;
#line 57 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  Error_chpl coerce_tmp_chpl3 = NULL;
#line 57 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  string_chpl ret_tmp_chpl2;
#line 57 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 57 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  uint64_t call_tmp_chpl9;
#line 57 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  int16_t call_tmp_chpl10;
#line 57 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  c_ptr_void_chpl call_tmp_chpl11 = NULL;
#line 57 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  chpl_checkValue7(size_chpl9, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 57 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  call_tmp_chpl6 = ret_tmp_chpl;
#line 57 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  i_arg_chpl = &call_tmp_chpl6;
#line 57 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  call_tmp_chpl7 = borrow_chpl2(i_arg_chpl);
#line 57 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  if (((RootClass_chpl)(call_tmp_chpl7)) != nil) /* ZLINE: 57 /Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl */
#line 57 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  {
#line 57 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
    coerce_tmp_chpl3 = ((Error_chpl)(call_tmp_chpl7));
#line 57 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
    chpl_check_nil(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 57 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
    ((void(*)(Error_chpl,
#line 57 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
       string_chpl *,
#line 57 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
       int64_t,
#line 57 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
       int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(coerce_tmp_chpl3))->chpl__cid) + INT64(1))])(coerce_tmp_chpl3, &ret_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 57 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
    call_tmp_chpl8 = ret_tmp_chpl2;
#line 57 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
    safeCastCheckHalt_chpl(&call_tmp_chpl8, _ln_chpl, _fn_chpl);
#line 57 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
    i_x_chpl = &call_tmp_chpl8;
#line 57 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
    deinit_chpl95(i_x_chpl, _ln_chpl, _fn_chpl);
#line 57 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  }
#line 57 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  chpl__autoDestroy62(&call_tmp_chpl6, _ln_chpl, _fn_chpl);
#line 57 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  call_tmp_chpl9 = ((uint64_t)(size_chpl9));
#line 57 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  call_tmp_chpl10 = chpl_memhook_md_num();
#line 57 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  call_tmp_chpl11 = chpl_mem_alloc(call_tmp_chpl9, ((int16_t)((md_chpl + call_tmp_chpl10))), _ln_chpl, _fn_chpl);
#line 53 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  return call_tmp_chpl11;
#line 53 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
}

#line 63 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
/* LocaleModelHelpMem.chpl:63 */
#line 63 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
static void * chpl_here_alloc2(uint64_t size_chpl9,
#line 63 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
                               int16_t md_chpl,
#line 63 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
                               int64_t _ln_chpl,
#line 63 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
                               int32_t _fn_chpl) {
#line 67 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  _owned_IllegalArgumentError_chpl2 call_tmp_chpl6;
#line 67 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  _owned_IllegalArgumentError_chpl2 ret_tmp_chpl;
#line 67 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  _ref__owned_IllegalArgumentError_chpl i_arg_chpl = NULL;
#line 67 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  IllegalArgumentError_chpl call_tmp_chpl7 = NULL;
#line 67 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  string_chpl call_tmp_chpl8;
#line 67 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  Error_chpl coerce_tmp_chpl3 = NULL;
#line 67 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  string_chpl ret_tmp_chpl2;
#line 67 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 67 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  uint64_t call_tmp_chpl9;
#line 67 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  int16_t call_tmp_chpl10;
#line 67 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  c_ptr_void_chpl call_tmp_chpl11 = NULL;
#line 67 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  chpl_checkValue5(size_chpl9, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 67 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  call_tmp_chpl6 = ret_tmp_chpl;
#line 67 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  i_arg_chpl = &call_tmp_chpl6;
#line 67 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  call_tmp_chpl7 = borrow_chpl2(i_arg_chpl);
#line 67 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  if (((RootClass_chpl)(call_tmp_chpl7)) != nil) /* ZLINE: 67 /Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl */
#line 67 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  {
#line 67 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
    coerce_tmp_chpl3 = ((Error_chpl)(call_tmp_chpl7));
#line 67 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
    chpl_check_nil(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 67 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
    ((void(*)(Error_chpl,
#line 67 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
       string_chpl *,
#line 67 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
       int64_t,
#line 67 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
       int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(coerce_tmp_chpl3))->chpl__cid) + INT64(1))])(coerce_tmp_chpl3, &ret_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 67 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
    call_tmp_chpl8 = ret_tmp_chpl2;
#line 67 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
    safeCastCheckHalt_chpl(&call_tmp_chpl8, _ln_chpl, _fn_chpl);
#line 67 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
    i_x_chpl = &call_tmp_chpl8;
#line 67 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
    deinit_chpl95(i_x_chpl, _ln_chpl, _fn_chpl);
#line 67 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  }
#line 67 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  chpl__autoDestroy62(&call_tmp_chpl6, _ln_chpl, _fn_chpl);
#line 67 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  call_tmp_chpl9 = size_chpl9;
#line 67 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  call_tmp_chpl10 = chpl_memhook_md_num();
#line 67 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  call_tmp_chpl11 = chpl_mem_alloc(call_tmp_chpl9, ((int16_t)((md_chpl + call_tmp_chpl10))), _ln_chpl, _fn_chpl);
#line 63 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  return call_tmp_chpl11;
#line 63 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
}

#line 73 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
/* LocaleModelHelpMem.chpl:73 */
#line 73 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
static void * chpl_here_aligned_alloc(uint64_t alignment_chpl,
#line 73 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
                                      uint64_t size_chpl9,
#line 73 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
                                      int16_t md_chpl,
#line 73 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
                                      int64_t _ln_chpl,
#line 73 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
                                      int32_t _fn_chpl) {
#line 78 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  _owned_IllegalArgumentError_chpl2 call_tmp_chpl6;
#line 78 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  _owned_IllegalArgumentError_chpl2 ret_tmp_chpl;
#line 78 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  _ref__owned_IllegalArgumentError_chpl i_arg_chpl = NULL;
#line 78 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  IllegalArgumentError_chpl call_tmp_chpl7 = NULL;
#line 78 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  string_chpl call_tmp_chpl8;
#line 78 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  Error_chpl coerce_tmp_chpl3 = NULL;
#line 78 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  string_chpl ret_tmp_chpl2;
#line 78 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 78 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  uint64_t call_tmp_chpl9;
#line 79 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  _owned_IllegalArgumentError_chpl2 call_tmp_chpl10;
#line 79 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  _owned_IllegalArgumentError_chpl2 ret_tmp_chpl3;
#line 79 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  _ref__owned_IllegalArgumentError_chpl i_arg_chpl2 = NULL;
#line 79 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  IllegalArgumentError_chpl call_tmp_chpl11 = NULL;
#line 79 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  string_chpl call_tmp_chpl12;
#line 79 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  Error_chpl coerce_tmp_chpl4 = NULL;
#line 79 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  string_chpl ret_tmp_chpl4;
#line 79 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  _ref_string_chpl i_x_chpl2 = NULL;
#line 79 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  uint64_t call_tmp_chpl13;
#line 80 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  int16_t call_tmp_chpl14;
#line 78 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  c_ptr_void_chpl call_tmp_chpl15 = NULL;
#line 78 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  chpl_checkValue5(alignment_chpl, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 78 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  call_tmp_chpl6 = ret_tmp_chpl;
#line 78 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  i_arg_chpl = &call_tmp_chpl6;
#line 78 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  call_tmp_chpl7 = borrow_chpl2(i_arg_chpl);
#line 78 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  if (((RootClass_chpl)(call_tmp_chpl7)) != nil) /* ZLINE: 78 /Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl */
#line 78 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  {
#line 78 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
    coerce_tmp_chpl3 = ((Error_chpl)(call_tmp_chpl7));
#line 78 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
    chpl_check_nil(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 78 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
    ((void(*)(Error_chpl,
#line 78 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
       string_chpl *,
#line 78 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
       int64_t,
#line 78 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
       int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(coerce_tmp_chpl3))->chpl__cid) + INT64(1))])(coerce_tmp_chpl3, &ret_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 78 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
    call_tmp_chpl8 = ret_tmp_chpl2;
#line 78 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
    safeCastCheckHalt_chpl(&call_tmp_chpl8, _ln_chpl, _fn_chpl);
#line 78 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
    i_x_chpl = &call_tmp_chpl8;
#line 78 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
    deinit_chpl95(i_x_chpl, _ln_chpl, _fn_chpl);
#line 78 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  }
#line 78 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  chpl__autoDestroy62(&call_tmp_chpl6, _ln_chpl, _fn_chpl);
#line 78 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  call_tmp_chpl9 = alignment_chpl;
#line 79 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  chpl_checkValue5(size_chpl9, &ret_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 79 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  call_tmp_chpl10 = ret_tmp_chpl3;
#line 79 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  i_arg_chpl2 = &call_tmp_chpl10;
#line 79 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  call_tmp_chpl11 = borrow_chpl2(i_arg_chpl2);
#line 79 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  if (((RootClass_chpl)(call_tmp_chpl11)) != nil) /* ZLINE: 79 /Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl */
#line 79 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  {
#line 79 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
    coerce_tmp_chpl4 = ((Error_chpl)(call_tmp_chpl11));
#line 79 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
    chpl_check_nil(coerce_tmp_chpl4, _ln_chpl, _fn_chpl);
#line 79 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
    ((void(*)(Error_chpl,
#line 79 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
       string_chpl *,
#line 79 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
       int64_t,
#line 79 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
       int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(coerce_tmp_chpl4))->chpl__cid) + INT64(1))])(coerce_tmp_chpl4, &ret_tmp_chpl4, _ln_chpl, _fn_chpl);
#line 79 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
    call_tmp_chpl12 = ret_tmp_chpl4;
#line 79 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
    safeCastCheckHalt_chpl(&call_tmp_chpl12, _ln_chpl, _fn_chpl);
#line 79 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
    i_x_chpl2 = &call_tmp_chpl12;
#line 79 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
    deinit_chpl95(i_x_chpl2, _ln_chpl, _fn_chpl);
#line 79 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  }
#line 79 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  chpl__autoDestroy62(&call_tmp_chpl10, _ln_chpl, _fn_chpl);
#line 79 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  call_tmp_chpl13 = size_chpl9;
#line 80 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  call_tmp_chpl14 = chpl_memhook_md_num();
#line 78 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  call_tmp_chpl15 = chpl_mem_memalign(call_tmp_chpl9, call_tmp_chpl13, ((int16_t)((md_chpl + call_tmp_chpl14))), _ln_chpl, _fn_chpl);
#line 73 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  return call_tmp_chpl15;
#line 73 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
}

#line 86 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
/* LocaleModelHelpMem.chpl:86 */
#line 86 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
static void * chpl_here_calloc(uint64_t size_chpl9,
#line 86 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
                               int64_t number_chpl,
#line 86 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
                               int16_t md_chpl,
#line 86 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
                               int64_t _ln_chpl,
#line 86 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
                               int32_t _fn_chpl) {
#line 90 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  _owned_IllegalArgumentError_chpl2 call_tmp_chpl6;
#line 90 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  _owned_IllegalArgumentError_chpl2 ret_tmp_chpl;
#line 90 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  _ref__owned_IllegalArgumentError_chpl i_arg_chpl = NULL;
#line 90 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  IllegalArgumentError_chpl call_tmp_chpl7 = NULL;
#line 90 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  string_chpl call_tmp_chpl8;
#line 90 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  Error_chpl coerce_tmp_chpl3 = NULL;
#line 90 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  string_chpl ret_tmp_chpl2;
#line 90 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 90 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  uint64_t call_tmp_chpl9;
#line 90 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  _owned_IllegalArgumentError_chpl2 call_tmp_chpl10;
#line 90 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  _owned_IllegalArgumentError_chpl2 ret_tmp_chpl3;
#line 90 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  _ref__owned_IllegalArgumentError_chpl i_arg_chpl2 = NULL;
#line 90 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  IllegalArgumentError_chpl call_tmp_chpl11 = NULL;
#line 90 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  string_chpl call_tmp_chpl12;
#line 90 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  Error_chpl coerce_tmp_chpl4 = NULL;
#line 90 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  string_chpl ret_tmp_chpl4;
#line 90 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  _ref_string_chpl i_x_chpl2 = NULL;
#line 90 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  uint64_t call_tmp_chpl13;
#line 90 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  int16_t call_tmp_chpl14;
#line 90 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  c_ptr_void_chpl call_tmp_chpl15 = NULL;
#line 90 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  chpl_checkValue7(number_chpl, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 90 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  call_tmp_chpl6 = ret_tmp_chpl;
#line 90 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  i_arg_chpl = &call_tmp_chpl6;
#line 90 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  call_tmp_chpl7 = borrow_chpl2(i_arg_chpl);
#line 90 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  if (((RootClass_chpl)(call_tmp_chpl7)) != nil) /* ZLINE: 90 /Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl */
#line 90 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  {
#line 90 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
    coerce_tmp_chpl3 = ((Error_chpl)(call_tmp_chpl7));
#line 90 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
    chpl_check_nil(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 90 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
    ((void(*)(Error_chpl,
#line 90 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
       string_chpl *,
#line 90 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
       int64_t,
#line 90 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
       int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(coerce_tmp_chpl3))->chpl__cid) + INT64(1))])(coerce_tmp_chpl3, &ret_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 90 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
    call_tmp_chpl8 = ret_tmp_chpl2;
#line 90 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
    safeCastCheckHalt_chpl(&call_tmp_chpl8, _ln_chpl, _fn_chpl);
#line 90 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
    i_x_chpl = &call_tmp_chpl8;
#line 90 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
    deinit_chpl95(i_x_chpl, _ln_chpl, _fn_chpl);
#line 90 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  }
#line 90 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  chpl__autoDestroy62(&call_tmp_chpl6, _ln_chpl, _fn_chpl);
#line 90 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  call_tmp_chpl9 = ((uint64_t)(number_chpl));
#line 90 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  chpl_checkValue5(size_chpl9, &ret_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 90 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  call_tmp_chpl10 = ret_tmp_chpl3;
#line 90 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  i_arg_chpl2 = &call_tmp_chpl10;
#line 90 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  call_tmp_chpl11 = borrow_chpl2(i_arg_chpl2);
#line 90 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  if (((RootClass_chpl)(call_tmp_chpl11)) != nil) /* ZLINE: 90 /Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl */
#line 90 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  {
#line 90 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
    coerce_tmp_chpl4 = ((Error_chpl)(call_tmp_chpl11));
#line 90 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
    chpl_check_nil(coerce_tmp_chpl4, _ln_chpl, _fn_chpl);
#line 90 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
    ((void(*)(Error_chpl,
#line 90 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
       string_chpl *,
#line 90 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
       int64_t,
#line 90 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
       int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(coerce_tmp_chpl4))->chpl__cid) + INT64(1))])(coerce_tmp_chpl4, &ret_tmp_chpl4, _ln_chpl, _fn_chpl);
#line 90 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
    call_tmp_chpl12 = ret_tmp_chpl4;
#line 90 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
    safeCastCheckHalt_chpl(&call_tmp_chpl12, _ln_chpl, _fn_chpl);
#line 90 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
    i_x_chpl2 = &call_tmp_chpl12;
#line 90 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
    deinit_chpl95(i_x_chpl2, _ln_chpl, _fn_chpl);
#line 90 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  }
#line 90 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  chpl__autoDestroy62(&call_tmp_chpl10, _ln_chpl, _fn_chpl);
#line 90 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  call_tmp_chpl13 = size_chpl9;
#line 90 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  call_tmp_chpl14 = chpl_memhook_md_num();
#line 90 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  call_tmp_chpl15 = chpl_mem_calloc(call_tmp_chpl9, call_tmp_chpl13, ((int16_t)((md_chpl + call_tmp_chpl14))), _ln_chpl, _fn_chpl);
#line 86 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  return call_tmp_chpl15;
#line 86 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
}

#line 95 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
/* LocaleModelHelpMem.chpl:95 */
#line 95 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
static void * chpl_here_realloc(void * ptr_chpl,
#line 95 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
                                int64_t size_chpl9,
#line 95 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
                                int16_t md_chpl,
#line 95 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
                                int64_t _ln_chpl,
#line 95 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
                                int32_t _fn_chpl) {
#line 99 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  _owned_IllegalArgumentError_chpl2 call_tmp_chpl6;
#line 99 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  _owned_IllegalArgumentError_chpl2 ret_tmp_chpl;
#line 99 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  _ref__owned_IllegalArgumentError_chpl i_arg_chpl = NULL;
#line 99 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  IllegalArgumentError_chpl call_tmp_chpl7 = NULL;
#line 99 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  string_chpl call_tmp_chpl8;
#line 99 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  Error_chpl coerce_tmp_chpl3 = NULL;
#line 99 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  string_chpl ret_tmp_chpl2;
#line 99 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 99 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  uint64_t call_tmp_chpl9;
#line 99 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  int16_t call_tmp_chpl10;
#line 99 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  c_ptr_void_chpl call_tmp_chpl11 = NULL;
#line 99 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  chpl_checkValue7(size_chpl9, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 99 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  call_tmp_chpl6 = ret_tmp_chpl;
#line 99 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  i_arg_chpl = &call_tmp_chpl6;
#line 99 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  call_tmp_chpl7 = borrow_chpl2(i_arg_chpl);
#line 99 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  if (((RootClass_chpl)(call_tmp_chpl7)) != nil) /* ZLINE: 99 /Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl */
#line 99 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  {
#line 99 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
    coerce_tmp_chpl3 = ((Error_chpl)(call_tmp_chpl7));
#line 99 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
    chpl_check_nil(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 99 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
    ((void(*)(Error_chpl,
#line 99 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
       string_chpl *,
#line 99 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
       int64_t,
#line 99 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
       int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(coerce_tmp_chpl3))->chpl__cid) + INT64(1))])(coerce_tmp_chpl3, &ret_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 99 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
    call_tmp_chpl8 = ret_tmp_chpl2;
#line 99 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
    safeCastCheckHalt_chpl(&call_tmp_chpl8, _ln_chpl, _fn_chpl);
#line 99 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
    i_x_chpl = &call_tmp_chpl8;
#line 99 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
    deinit_chpl95(i_x_chpl, _ln_chpl, _fn_chpl);
#line 99 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  }
#line 99 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  chpl__autoDestroy62(&call_tmp_chpl6, _ln_chpl, _fn_chpl);
#line 99 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  call_tmp_chpl9 = ((uint64_t)(size_chpl9));
#line 99 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  call_tmp_chpl10 = chpl_memhook_md_num();
#line 99 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  call_tmp_chpl11 = chpl_mem_realloc(ptr_chpl, call_tmp_chpl9, ((int16_t)((md_chpl + call_tmp_chpl10))), _ln_chpl, _fn_chpl);
#line 95 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  return call_tmp_chpl11;
#line 95 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
}

#line 104 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
/* LocaleModelHelpMem.chpl:104 */
#line 104 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
static int64_t chpl_here_good_alloc_size(int64_t min_size_chpl,
#line 104 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
                                         int64_t _ln_chpl,
#line 104 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
                                         int32_t _fn_chpl) {
#line 108 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  _owned_IllegalArgumentError_chpl2 call_tmp_chpl6;
#line 108 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  _owned_IllegalArgumentError_chpl2 ret_tmp_chpl;
#line 108 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  _ref__owned_IllegalArgumentError_chpl i_arg_chpl = NULL;
#line 108 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  IllegalArgumentError_chpl call_tmp_chpl7 = NULL;
#line 108 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  string_chpl call_tmp_chpl8;
#line 108 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  Error_chpl coerce_tmp_chpl3 = NULL;
#line 108 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  string_chpl ret_tmp_chpl2;
#line 108 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 108 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  uint64_t call_tmp_chpl9;
#line 108 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  _owned_IllegalArgumentError_chpl2 call_tmp_chpl10;
#line 108 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  _owned_IllegalArgumentError_chpl2 ret_tmp_chpl3;
#line 108 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  _ref__owned_IllegalArgumentError_chpl i_arg_chpl2 = NULL;
#line 108 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  IllegalArgumentError_chpl call_tmp_chpl11 = NULL;
#line 108 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  string_chpl call_tmp_chpl12;
#line 108 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  Error_chpl coerce_tmp_chpl4 = NULL;
#line 108 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  string_chpl ret_tmp_chpl4;
#line 108 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  _ref_string_chpl i_x_chpl2 = NULL;
#line 108 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  int64_t call_tmp_chpl13;
#line 108 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  chpl_checkValue7(min_size_chpl, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 108 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  call_tmp_chpl6 = ret_tmp_chpl;
#line 108 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  i_arg_chpl = &call_tmp_chpl6;
#line 108 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  call_tmp_chpl7 = borrow_chpl2(i_arg_chpl);
#line 108 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  if (((RootClass_chpl)(call_tmp_chpl7)) != nil) /* ZLINE: 108 /Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl */
#line 108 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  {
#line 108 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
    coerce_tmp_chpl3 = ((Error_chpl)(call_tmp_chpl7));
#line 108 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
    chpl_check_nil(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 108 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
    ((void(*)(Error_chpl,
#line 108 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
       string_chpl *,
#line 108 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
       int64_t,
#line 108 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
       int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(coerce_tmp_chpl3))->chpl__cid) + INT64(1))])(coerce_tmp_chpl3, &ret_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 108 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
    call_tmp_chpl8 = ret_tmp_chpl2;
#line 108 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
    safeCastCheckHalt_chpl(&call_tmp_chpl8, _ln_chpl, _fn_chpl);
#line 108 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
    i_x_chpl = &call_tmp_chpl8;
#line 108 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
    deinit_chpl95(i_x_chpl, _ln_chpl, _fn_chpl);
#line 108 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  }
#line 108 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  chpl__autoDestroy62(&call_tmp_chpl6, _ln_chpl, _fn_chpl);
#line 108 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  call_tmp_chpl9 = chpl_mem_good_alloc_size(((uint64_t)(min_size_chpl)), _ln_chpl, _fn_chpl);
#line 108 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  chpl_checkValue3(call_tmp_chpl9, &ret_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 108 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  call_tmp_chpl10 = ret_tmp_chpl3;
#line 108 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  i_arg_chpl2 = &call_tmp_chpl10;
#line 108 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  call_tmp_chpl11 = borrow_chpl2(i_arg_chpl2);
#line 108 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  if (((RootClass_chpl)(call_tmp_chpl11)) != nil) /* ZLINE: 108 /Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl */
#line 108 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  {
#line 108 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
    coerce_tmp_chpl4 = ((Error_chpl)(call_tmp_chpl11));
#line 108 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
    chpl_check_nil(coerce_tmp_chpl4, _ln_chpl, _fn_chpl);
#line 108 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
    ((void(*)(Error_chpl,
#line 108 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
       string_chpl *,
#line 108 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
       int64_t,
#line 108 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
       int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(coerce_tmp_chpl4))->chpl__cid) + INT64(1))])(coerce_tmp_chpl4, &ret_tmp_chpl4, _ln_chpl, _fn_chpl);
#line 108 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
    call_tmp_chpl12 = ret_tmp_chpl4;
#line 108 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
    safeCastCheckHalt_chpl(&call_tmp_chpl12, _ln_chpl, _fn_chpl);
#line 108 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
    i_x_chpl2 = &call_tmp_chpl12;
#line 108 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
    deinit_chpl95(i_x_chpl2, _ln_chpl, _fn_chpl);
#line 108 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  }
#line 108 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  chpl__autoDestroy62(&call_tmp_chpl10, _ln_chpl, _fn_chpl);
#line 108 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  call_tmp_chpl13 = ((int64_t)(call_tmp_chpl9));
#line 104 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  return call_tmp_chpl13;
#line 104 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
}

#line 113 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
/* LocaleModelHelpMem.chpl:113 */
#line 113 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
static void chpl_here_free(void * ptr_chpl,
#line 113 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
                           int64_t _ln_chpl,
#line 113 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
                           int32_t _fn_chpl) {
#line 117 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  chpl_mem_free(ptr_chpl, _ln_chpl, _fn_chpl);
#line 113 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
  return;
#line 113 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpMem.chpl"
}

