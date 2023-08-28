#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
/* ChapelIOStringifyHelper.chpl:24 */
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
static void chpl__init_ChapelIOStringifyHelper(int64_t _ln_chpl,
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
                                               int32_t _fn_chpl) {
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  _ref_int32_t_chpl refIndentLevel_chpl = NULL;
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  if (chpl__init_ChapelIOStringifyHelper_p) /* ZLINE: 24 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl */
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  {
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
    goto _exit_chpl__init_ChapelIOStringifyHelper_chpl;
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  }
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  printModuleInit_chpl("%*s\n", "ChapelIOStringifyHelper", INT64(23), _ln_chpl, _fn_chpl);
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  refIndentLevel_chpl = &moduleInitLevel_chpl;
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  *(refIndentLevel_chpl) += INT64(1);
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  chpl__init_ChapelIOStringifyHelper_p = UINT8(true);
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  {
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
    chpl__init_CTypes(_ln_chpl, _fn_chpl);
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
    chpl__init_BytesStringCommon(_ln_chpl, _fn_chpl);
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  }
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  *(refIndentLevel_chpl) -= INT64(1);
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  _exit_chpl__init_ChapelIOStringifyHelper_chpl:;
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  return;
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
}

#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
/* ChapelIOStringifyHelper.chpl:70 */
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
static void stringify_simple_chpl15(string_chpl * _e0_args_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
                                    int64_t _e1_args_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
                                    string_chpl * _e2_args_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
                                    string_chpl * _retArg_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
                                    int64_t _ln_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
                                    int32_t _fn_chpl) {
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl ret_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl str_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl inlineImm_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl ret_chpl2;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl tmp_chpl;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl call_tmp_chpl6;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl ret_tmp_chpl;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  _ref_string_chpl i_x_chpl2 = NULL;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  inlineImm_chpl = _str_literal_163_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  i_x_chpl = &inlineImm_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  init_ASSIGN__chpl25(&tmp_chpl, i_x_chpl, _ln_chpl, _fn_chpl);
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  ret_chpl2 = tmp_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  str_chpl = ret_chpl2;
#line 80 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  chpl___PLUS__ASSIGN_(&str_chpl, _e0_args_chpl, _ln_chpl, _fn_chpl);
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  chpl___COLON_25(_e1_args_chpl, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  call_tmp_chpl6 = ret_tmp_chpl;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  chpl___PLUS__ASSIGN_(&str_chpl, &call_tmp_chpl6, _ln_chpl, _fn_chpl);
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  i_x_chpl2 = &call_tmp_chpl6;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  deinit_chpl95(i_x_chpl2, _ln_chpl, _fn_chpl);
#line 80 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  chpl___PLUS__ASSIGN_(&str_chpl, _e2_args_chpl, _ln_chpl, _fn_chpl);
#line 100 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  ret_chpl = str_chpl;
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  *(_retArg_chpl) = ret_chpl;
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  return;
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
}

#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
/* ChapelIOStringifyHelper.chpl:70 */
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
static void stringify_simple_chpl12(int8_t * _e0_args_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
                                    string_chpl * _retArg_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
                                    int64_t _ln_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
                                    int32_t _fn_chpl) {
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  c_ptrConst_int8_t_chpl elt_x0_chpl = NULL;
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl ret_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl str_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl inlineImm_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl ret_chpl2;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl tmp_chpl;
#line 83 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  Error_chpl error_chpl = NULL;
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl call_tmp_chpl6;
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  int64_t call_tmp_chpl7;
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  _owned_IllegalArgumentError_chpl2 call_tmp_chpl8;
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  _owned_IllegalArgumentError_chpl2 ret_tmp_chpl;
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  _ref__owned_IllegalArgumentError_chpl i_arg_chpl = NULL;
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  IllegalArgumentError_chpl call_tmp_chpl9 = NULL;
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl call_tmp_chpl10;
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  Error_chpl coerce_tmp_chpl3 = NULL;
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl ret_tmp_chpl2;
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  _ref_string_chpl i_x_chpl2 = NULL;
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  int64_t call_tmp_chpl11;
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl ret_chpl3;
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl call_tmp_chpl12;
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  Error_chpl error_chpl2 = NULL;
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl ret_tmp_chpl3;
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  _ref_string_chpl i_x_chpl3 = NULL;
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  elt_x0_chpl = ((c_ptrConst_int8_t_chpl)(((c_string_rehook)(_e0_args_chpl))));
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  inlineImm_chpl = _str_literal_163_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  i_x_chpl = &inlineImm_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  init_ASSIGN__chpl25(&tmp_chpl, i_x_chpl, _ln_chpl, _fn_chpl);
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  ret_chpl2 = tmp_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  str_chpl = ret_chpl2;
#line 83 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  error_chpl = NULL;
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  call_tmp_chpl7 = string_length_bytes(elt_x0_chpl);
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  chpl_checkValue2(call_tmp_chpl7, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  call_tmp_chpl8 = ret_tmp_chpl;
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  i_arg_chpl = &call_tmp_chpl8;
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  call_tmp_chpl9 = borrow_chpl2(i_arg_chpl);
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  if (((RootClass_chpl)(call_tmp_chpl9)) != nil) /* ZLINE: 84 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl */
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  {
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
    coerce_tmp_chpl3 = ((Error_chpl)(call_tmp_chpl9));
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
    chpl_check_nil(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
    ((void(*)(Error_chpl,
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
       string_chpl *,
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
       int64_t,
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
       int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(coerce_tmp_chpl3))->chpl__cid) + INT64(1))])(coerce_tmp_chpl3, &ret_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
    call_tmp_chpl10 = ret_tmp_chpl2;
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
    safeCastCheckHalt_chpl(&call_tmp_chpl10, _ln_chpl, _fn_chpl);
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
    i_x_chpl2 = &call_tmp_chpl10;
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
    deinit_chpl95(i_x_chpl2, _ln_chpl, _fn_chpl);
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  }
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  chpl__autoDestroy62(&call_tmp_chpl8, _ln_chpl, _fn_chpl);
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  call_tmp_chpl11 = call_tmp_chpl7;
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  if (error_chpl != nil) /* ZLINE: 84 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl */
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  {
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
    goto handler_chpl;
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  }
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  error_chpl2 = NULL;
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  createCopyingBuffer_chpl(((c_ptr_uint8_t_chpl)(elt_x0_chpl)), call_tmp_chpl11, ((int64_t)((call_tmp_chpl11 + INT64(1)))), decodePolicy_chpl_replace_chpl, &error_chpl2, &ret_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  call_tmp_chpl12 = ret_tmp_chpl3;
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  if (error_chpl2 != nil) /* ZLINE: 84 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl */
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  {
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
    error_chpl = error_chpl2;
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
    goto _endcreateCopyingBuffer_chpl;
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  }
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  ret_chpl3 = call_tmp_chpl12;
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  _endcreateCopyingBuffer_chpl:;
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  call_tmp_chpl6 = ret_chpl3;
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  if (error_chpl != nil) /* ZLINE: 84 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl */
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  {
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
    goto handler_chpl;
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  }
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  chpl___PLUS__ASSIGN_(&str_chpl, &call_tmp_chpl6, _ln_chpl, _fn_chpl);
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  i_x_chpl3 = &call_tmp_chpl6;
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  deinit_chpl95(i_x_chpl3, _ln_chpl, _fn_chpl);
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  handler_chpl:;
#line 83 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  if (error_chpl != nil) /* ZLINE: 83 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl */
#line 83 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  {
#line 83 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
    chpl_uncaught_error(error_chpl, _ln_chpl, _fn_chpl);
#line 83 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  }
#line 100 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  ret_chpl = str_chpl;
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  *(_retArg_chpl) = ret_chpl;
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  return;
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
}

#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
/* ChapelIOStringifyHelper.chpl:70 */
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
static void stringify_simple_chpl21(string_chpl * _e0_args_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
                                    string_chpl * _retArg_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
                                    int64_t _ln_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
                                    int32_t _fn_chpl) {
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl ret_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl str_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl inlineImm_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl ret_chpl2;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl tmp_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  inlineImm_chpl = _str_literal_163_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  i_x_chpl = &inlineImm_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  init_ASSIGN__chpl25(&tmp_chpl, i_x_chpl, _ln_chpl, _fn_chpl);
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  ret_chpl2 = tmp_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  str_chpl = ret_chpl2;
#line 80 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  chpl___PLUS__ASSIGN_(&str_chpl, _e0_args_chpl, _ln_chpl, _fn_chpl);
#line 100 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  ret_chpl = str_chpl;
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  *(_retArg_chpl) = ret_chpl;
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  return;
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
}

#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
/* ChapelIOStringifyHelper.chpl:70 */
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
static void stringify_simple_chpl9(int64_t _e0_args_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
                                   string_chpl * _retArg_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
                                   int64_t _ln_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
                                   int32_t _fn_chpl) {
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl ret_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl str_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl inlineImm_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl ret_chpl2;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl tmp_chpl;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl call_tmp_chpl6;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl ret_tmp_chpl;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  _ref_string_chpl i_x_chpl2 = NULL;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  inlineImm_chpl = _str_literal_163_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  i_x_chpl = &inlineImm_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  init_ASSIGN__chpl25(&tmp_chpl, i_x_chpl, _ln_chpl, _fn_chpl);
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  ret_chpl2 = tmp_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  str_chpl = ret_chpl2;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  chpl___COLON_25(_e0_args_chpl, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  call_tmp_chpl6 = ret_tmp_chpl;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  chpl___PLUS__ASSIGN_(&str_chpl, &call_tmp_chpl6, _ln_chpl, _fn_chpl);
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  i_x_chpl2 = &call_tmp_chpl6;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  deinit_chpl95(i_x_chpl2, _ln_chpl, _fn_chpl);
#line 100 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  ret_chpl = str_chpl;
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  *(_retArg_chpl) = ret_chpl;
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  return;
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
}

#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
/* ChapelIOStringifyHelper.chpl:70 */
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
static void stringify_simple_chpl20(string_chpl * _e0_args_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
                                    int64_t _e1_args_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
                                    string_chpl * _e2_args_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
                                    int64_t _e3_args_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
                                    string_chpl * _e4_args_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
                                    string_chpl * _retArg_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
                                    int64_t _ln_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
                                    int32_t _fn_chpl) {
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl ret_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl str_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl inlineImm_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl ret_chpl2;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl tmp_chpl;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl call_tmp_chpl6;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl ret_tmp_chpl;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  _ref_string_chpl i_x_chpl2 = NULL;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl call_tmp_chpl7;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl ret_tmp_chpl2;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  _ref_string_chpl i_x_chpl3 = NULL;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  inlineImm_chpl = _str_literal_163_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  i_x_chpl = &inlineImm_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  init_ASSIGN__chpl25(&tmp_chpl, i_x_chpl, _ln_chpl, _fn_chpl);
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  ret_chpl2 = tmp_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  str_chpl = ret_chpl2;
#line 80 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  chpl___PLUS__ASSIGN_(&str_chpl, _e0_args_chpl, _ln_chpl, _fn_chpl);
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  chpl___COLON_25(_e1_args_chpl, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  call_tmp_chpl6 = ret_tmp_chpl;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  chpl___PLUS__ASSIGN_(&str_chpl, &call_tmp_chpl6, _ln_chpl, _fn_chpl);
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  i_x_chpl2 = &call_tmp_chpl6;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  deinit_chpl95(i_x_chpl2, _ln_chpl, _fn_chpl);
#line 80 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  chpl___PLUS__ASSIGN_(&str_chpl, _e2_args_chpl, _ln_chpl, _fn_chpl);
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  chpl___COLON_25(_e3_args_chpl, &ret_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  call_tmp_chpl7 = ret_tmp_chpl2;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  chpl___PLUS__ASSIGN_(&str_chpl, &call_tmp_chpl7, _ln_chpl, _fn_chpl);
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  i_x_chpl3 = &call_tmp_chpl7;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  deinit_chpl95(i_x_chpl3, _ln_chpl, _fn_chpl);
#line 80 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  chpl___PLUS__ASSIGN_(&str_chpl, _e4_args_chpl, _ln_chpl, _fn_chpl);
#line 100 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  ret_chpl = str_chpl;
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  *(_retArg_chpl) = ret_chpl;
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  return;
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
}

#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
/* ChapelIOStringifyHelper.chpl:70 */
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
static void stringify_simple_chpl8(string_chpl * _e0_args_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
                                   string_chpl * _e1_args_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
                                   int64_t _e2_args_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
                                   string_chpl * _e3_args_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
                                   range_int64_t_both_one_chpl _e4_args_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
                                   string_chpl * _retArg_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
                                   int64_t _ln_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
                                   int32_t _fn_chpl) {
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl ret_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl str_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl inlineImm_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl ret_chpl2;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl tmp_chpl;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl call_tmp_chpl6;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl ret_tmp_chpl;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  _ref_string_chpl i_x_chpl2 = NULL;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl call_tmp_chpl7;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl ret_tmp_chpl2;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  _ref_string_chpl i_x_chpl3 = NULL;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  inlineImm_chpl = _str_literal_163_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  i_x_chpl = &inlineImm_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  init_ASSIGN__chpl25(&tmp_chpl, i_x_chpl, _ln_chpl, _fn_chpl);
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  ret_chpl2 = tmp_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  str_chpl = ret_chpl2;
#line 80 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  chpl___PLUS__ASSIGN_(&str_chpl, _e0_args_chpl, _ln_chpl, _fn_chpl);
#line 80 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  chpl___PLUS__ASSIGN_(&str_chpl, _e1_args_chpl, _ln_chpl, _fn_chpl);
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  chpl___COLON_25(_e2_args_chpl, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  call_tmp_chpl6 = ret_tmp_chpl;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  chpl___PLUS__ASSIGN_(&str_chpl, &call_tmp_chpl6, _ln_chpl, _fn_chpl);
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  i_x_chpl2 = &call_tmp_chpl6;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  deinit_chpl95(i_x_chpl2, _ln_chpl, _fn_chpl);
#line 80 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  chpl___PLUS__ASSIGN_(&str_chpl, _e3_args_chpl, _ln_chpl, _fn_chpl);
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  chpl___COLON_16(_e4_args_chpl, &ret_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  call_tmp_chpl7 = ret_tmp_chpl2;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  chpl___PLUS__ASSIGN_(&str_chpl, &call_tmp_chpl7, _ln_chpl, _fn_chpl);
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  i_x_chpl3 = &call_tmp_chpl7;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  deinit_chpl95(i_x_chpl3, _ln_chpl, _fn_chpl);
#line 100 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  ret_chpl = str_chpl;
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  *(_retArg_chpl) = ret_chpl;
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  return;
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
}

#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
/* ChapelIOStringifyHelper.chpl:70 */
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
static void stringify_simple_chpl(string_chpl * _e0_args_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
                                  string_chpl * _e1_args_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
                                  range_int64_t_both_one_chpl _e2_args_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
                                  string_chpl * _e3_args_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
                                  range_int64_t_both_one_chpl _e4_args_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
                                  string_chpl * _retArg_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
                                  int64_t _ln_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
                                  int32_t _fn_chpl) {
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl ret_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl str_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl inlineImm_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl ret_chpl2;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl tmp_chpl;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl call_tmp_chpl6;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl ret_tmp_chpl;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  _ref_string_chpl i_x_chpl2 = NULL;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl call_tmp_chpl7;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl ret_tmp_chpl2;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  _ref_string_chpl i_x_chpl3 = NULL;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  inlineImm_chpl = _str_literal_163_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  i_x_chpl = &inlineImm_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  init_ASSIGN__chpl25(&tmp_chpl, i_x_chpl, _ln_chpl, _fn_chpl);
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  ret_chpl2 = tmp_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  str_chpl = ret_chpl2;
#line 80 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  chpl___PLUS__ASSIGN_(&str_chpl, _e0_args_chpl, _ln_chpl, _fn_chpl);
#line 80 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  chpl___PLUS__ASSIGN_(&str_chpl, _e1_args_chpl, _ln_chpl, _fn_chpl);
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  chpl___COLON_16(_e2_args_chpl, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  call_tmp_chpl6 = ret_tmp_chpl;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  chpl___PLUS__ASSIGN_(&str_chpl, &call_tmp_chpl6, _ln_chpl, _fn_chpl);
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  i_x_chpl2 = &call_tmp_chpl6;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  deinit_chpl95(i_x_chpl2, _ln_chpl, _fn_chpl);
#line 80 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  chpl___PLUS__ASSIGN_(&str_chpl, _e3_args_chpl, _ln_chpl, _fn_chpl);
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  chpl___COLON_16(_e4_args_chpl, &ret_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  call_tmp_chpl7 = ret_tmp_chpl2;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  chpl___PLUS__ASSIGN_(&str_chpl, &call_tmp_chpl7, _ln_chpl, _fn_chpl);
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  i_x_chpl3 = &call_tmp_chpl7;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  deinit_chpl95(i_x_chpl3, _ln_chpl, _fn_chpl);
#line 100 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  ret_chpl = str_chpl;
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  *(_retArg_chpl) = ret_chpl;
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  return;
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
}

#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
/* ChapelIOStringifyHelper.chpl:70 */
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
static void stringify_simple_chpl17(string_chpl * _e0_args_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
                                    range_int64_t_both_one_chpl _e1_args_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
                                    string_chpl * _e2_args_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
                                    int64_t _e3_args_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
                                    string_chpl * _retArg_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
                                    int64_t _ln_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
                                    int32_t _fn_chpl) {
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl ret_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl str_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl inlineImm_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl ret_chpl2;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl tmp_chpl;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl call_tmp_chpl6;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl ret_tmp_chpl;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  _ref_string_chpl i_x_chpl2 = NULL;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl call_tmp_chpl7;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl ret_tmp_chpl2;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  _ref_string_chpl i_x_chpl3 = NULL;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  inlineImm_chpl = _str_literal_163_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  i_x_chpl = &inlineImm_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  init_ASSIGN__chpl25(&tmp_chpl, i_x_chpl, _ln_chpl, _fn_chpl);
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  ret_chpl2 = tmp_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  str_chpl = ret_chpl2;
#line 80 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  chpl___PLUS__ASSIGN_(&str_chpl, _e0_args_chpl, _ln_chpl, _fn_chpl);
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  chpl___COLON_16(_e1_args_chpl, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  call_tmp_chpl6 = ret_tmp_chpl;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  chpl___PLUS__ASSIGN_(&str_chpl, &call_tmp_chpl6, _ln_chpl, _fn_chpl);
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  i_x_chpl2 = &call_tmp_chpl6;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  deinit_chpl95(i_x_chpl2, _ln_chpl, _fn_chpl);
#line 80 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  chpl___PLUS__ASSIGN_(&str_chpl, _e2_args_chpl, _ln_chpl, _fn_chpl);
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  chpl___COLON_25(_e3_args_chpl, &ret_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  call_tmp_chpl7 = ret_tmp_chpl2;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  chpl___PLUS__ASSIGN_(&str_chpl, &call_tmp_chpl7, _ln_chpl, _fn_chpl);
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  i_x_chpl3 = &call_tmp_chpl7;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  deinit_chpl95(i_x_chpl3, _ln_chpl, _fn_chpl);
#line 100 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  ret_chpl = str_chpl;
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  *(_retArg_chpl) = ret_chpl;
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  return;
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
}

#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
/* ChapelIOStringifyHelper.chpl:70 */
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
static void stringify_simple_chpl4(string_chpl * _e0_args_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
                                   range_byteIndex_both_one_chpl _e1_args_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
                                   string_chpl * _e2_args_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
                                   int64_t _e3_args_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
                                   string_chpl * _retArg_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
                                   int64_t _ln_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
                                   int32_t _fn_chpl) {
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl ret_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl str_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl inlineImm_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl ret_chpl2;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl tmp_chpl;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl call_tmp_chpl6;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl ret_tmp_chpl;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  _ref_string_chpl i_x_chpl2 = NULL;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl call_tmp_chpl7;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl ret_tmp_chpl2;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  _ref_string_chpl i_x_chpl3 = NULL;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  inlineImm_chpl = _str_literal_163_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  i_x_chpl = &inlineImm_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  init_ASSIGN__chpl25(&tmp_chpl, i_x_chpl, _ln_chpl, _fn_chpl);
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  ret_chpl2 = tmp_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  str_chpl = ret_chpl2;
#line 80 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  chpl___PLUS__ASSIGN_(&str_chpl, _e0_args_chpl, _ln_chpl, _fn_chpl);
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  chpl___COLON_13(_e1_args_chpl, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  call_tmp_chpl6 = ret_tmp_chpl;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  chpl___PLUS__ASSIGN_(&str_chpl, &call_tmp_chpl6, _ln_chpl, _fn_chpl);
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  i_x_chpl2 = &call_tmp_chpl6;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  deinit_chpl95(i_x_chpl2, _ln_chpl, _fn_chpl);
#line 80 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  chpl___PLUS__ASSIGN_(&str_chpl, _e2_args_chpl, _ln_chpl, _fn_chpl);
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  chpl___COLON_25(_e3_args_chpl, &ret_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  call_tmp_chpl7 = ret_tmp_chpl2;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  chpl___PLUS__ASSIGN_(&str_chpl, &call_tmp_chpl7, _ln_chpl, _fn_chpl);
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  i_x_chpl3 = &call_tmp_chpl7;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  deinit_chpl95(i_x_chpl3, _ln_chpl, _fn_chpl);
#line 100 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  ret_chpl = str_chpl;
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  *(_retArg_chpl) = ret_chpl;
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  return;
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
}

#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
/* ChapelIOStringifyHelper.chpl:70 */
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
static void stringify_simple_chpl3(string_chpl * _e0_args_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
                                   int64_t _e1_args_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
                                   string_chpl * _e2_args_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
                                   int64_t _e3_args_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
                                   string_chpl * _retArg_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
                                   int64_t _ln_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
                                   int32_t _fn_chpl) {
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl ret_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl str_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl inlineImm_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl ret_chpl2;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl tmp_chpl;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl call_tmp_chpl6;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl ret_tmp_chpl;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  _ref_string_chpl i_x_chpl2 = NULL;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl call_tmp_chpl7;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl ret_tmp_chpl2;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  _ref_string_chpl i_x_chpl3 = NULL;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  inlineImm_chpl = _str_literal_163_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  i_x_chpl = &inlineImm_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  init_ASSIGN__chpl25(&tmp_chpl, i_x_chpl, _ln_chpl, _fn_chpl);
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  ret_chpl2 = tmp_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  str_chpl = ret_chpl2;
#line 80 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  chpl___PLUS__ASSIGN_(&str_chpl, _e0_args_chpl, _ln_chpl, _fn_chpl);
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  chpl___COLON_25(_e1_args_chpl, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  call_tmp_chpl6 = ret_tmp_chpl;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  chpl___PLUS__ASSIGN_(&str_chpl, &call_tmp_chpl6, _ln_chpl, _fn_chpl);
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  i_x_chpl2 = &call_tmp_chpl6;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  deinit_chpl95(i_x_chpl2, _ln_chpl, _fn_chpl);
#line 80 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  chpl___PLUS__ASSIGN_(&str_chpl, _e2_args_chpl, _ln_chpl, _fn_chpl);
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  chpl___COLON_25(_e3_args_chpl, &ret_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  call_tmp_chpl7 = ret_tmp_chpl2;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  chpl___PLUS__ASSIGN_(&str_chpl, &call_tmp_chpl7, _ln_chpl, _fn_chpl);
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  i_x_chpl3 = &call_tmp_chpl7;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  deinit_chpl95(i_x_chpl3, _ln_chpl, _fn_chpl);
#line 100 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  ret_chpl = str_chpl;
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  *(_retArg_chpl) = ret_chpl;
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  return;
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
}

#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
/* ChapelIOStringifyHelper.chpl:70 */
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
static void stringify_simple_chpl5(string_chpl * _e0_args_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
                                   range_int64_t_low_one_chpl _e1_args_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
                                   string_chpl * _e2_args_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
                                   int64_t _e3_args_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
                                   string_chpl * _retArg_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
                                   int64_t _ln_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
                                   int32_t _fn_chpl) {
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl ret_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl str_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl inlineImm_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl ret_chpl2;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl tmp_chpl;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl call_tmp_chpl6;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl ret_tmp_chpl;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  _ref_string_chpl i_x_chpl2 = NULL;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl call_tmp_chpl7;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl ret_tmp_chpl2;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  _ref_string_chpl i_x_chpl3 = NULL;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  inlineImm_chpl = _str_literal_163_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  i_x_chpl = &inlineImm_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  init_ASSIGN__chpl25(&tmp_chpl, i_x_chpl, _ln_chpl, _fn_chpl);
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  ret_chpl2 = tmp_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  str_chpl = ret_chpl2;
#line 80 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  chpl___PLUS__ASSIGN_(&str_chpl, _e0_args_chpl, _ln_chpl, _fn_chpl);
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  chpl___COLON_17(_e1_args_chpl, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  call_tmp_chpl6 = ret_tmp_chpl;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  chpl___PLUS__ASSIGN_(&str_chpl, &call_tmp_chpl6, _ln_chpl, _fn_chpl);
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  i_x_chpl2 = &call_tmp_chpl6;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  deinit_chpl95(i_x_chpl2, _ln_chpl, _fn_chpl);
#line 80 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  chpl___PLUS__ASSIGN_(&str_chpl, _e2_args_chpl, _ln_chpl, _fn_chpl);
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  chpl___COLON_25(_e3_args_chpl, &ret_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  call_tmp_chpl7 = ret_tmp_chpl2;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  chpl___PLUS__ASSIGN_(&str_chpl, &call_tmp_chpl7, _ln_chpl, _fn_chpl);
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  i_x_chpl3 = &call_tmp_chpl7;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  deinit_chpl95(i_x_chpl3, _ln_chpl, _fn_chpl);
#line 100 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  ret_chpl = str_chpl;
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  *(_retArg_chpl) = ret_chpl;
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  return;
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
}

#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
/* ChapelIOStringifyHelper.chpl:70 */
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
static void stringify_simple_chpl19(string_chpl * _e0_args_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
                                    range_byteIndex_low_one_chpl _e1_args_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
                                    string_chpl * _e2_args_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
                                    int64_t _e3_args_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
                                    string_chpl * _retArg_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
                                    int64_t _ln_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
                                    int32_t _fn_chpl) {
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl ret_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl str_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl inlineImm_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl ret_chpl2;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl tmp_chpl;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl call_tmp_chpl6;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl ret_tmp_chpl;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  _ref_string_chpl i_x_chpl2 = NULL;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl call_tmp_chpl7;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl ret_tmp_chpl2;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  _ref_string_chpl i_x_chpl3 = NULL;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  inlineImm_chpl = _str_literal_163_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  i_x_chpl = &inlineImm_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  init_ASSIGN__chpl25(&tmp_chpl, i_x_chpl, _ln_chpl, _fn_chpl);
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  ret_chpl2 = tmp_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  str_chpl = ret_chpl2;
#line 80 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  chpl___PLUS__ASSIGN_(&str_chpl, _e0_args_chpl, _ln_chpl, _fn_chpl);
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  chpl___COLON_12(_e1_args_chpl, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  call_tmp_chpl6 = ret_tmp_chpl;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  chpl___PLUS__ASSIGN_(&str_chpl, &call_tmp_chpl6, _ln_chpl, _fn_chpl);
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  i_x_chpl2 = &call_tmp_chpl6;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  deinit_chpl95(i_x_chpl2, _ln_chpl, _fn_chpl);
#line 80 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  chpl___PLUS__ASSIGN_(&str_chpl, _e2_args_chpl, _ln_chpl, _fn_chpl);
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  chpl___COLON_25(_e3_args_chpl, &ret_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  call_tmp_chpl7 = ret_tmp_chpl2;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  chpl___PLUS__ASSIGN_(&str_chpl, &call_tmp_chpl7, _ln_chpl, _fn_chpl);
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  i_x_chpl3 = &call_tmp_chpl7;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  deinit_chpl95(i_x_chpl3, _ln_chpl, _fn_chpl);
#line 100 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  ret_chpl = str_chpl;
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  *(_retArg_chpl) = ret_chpl;
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  return;
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
}

#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
/* ChapelIOStringifyHelper.chpl:70 */
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
static void stringify_simple_chpl18(string_chpl * _e0_args_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
                                    range_byteIndex_high_one_chpl _e1_args_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
                                    string_chpl * _e2_args_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
                                    int64_t _e3_args_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
                                    string_chpl * _retArg_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
                                    int64_t _ln_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
                                    int32_t _fn_chpl) {
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl ret_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl str_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl inlineImm_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl ret_chpl2;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl tmp_chpl;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl call_tmp_chpl6;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl ret_tmp_chpl;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  _ref_string_chpl i_x_chpl2 = NULL;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl call_tmp_chpl7;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl ret_tmp_chpl2;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  _ref_string_chpl i_x_chpl3 = NULL;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  inlineImm_chpl = _str_literal_163_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  i_x_chpl = &inlineImm_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  init_ASSIGN__chpl25(&tmp_chpl, i_x_chpl, _ln_chpl, _fn_chpl);
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  ret_chpl2 = tmp_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  str_chpl = ret_chpl2;
#line 80 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  chpl___PLUS__ASSIGN_(&str_chpl, _e0_args_chpl, _ln_chpl, _fn_chpl);
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  chpl___COLON_14(_e1_args_chpl, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  call_tmp_chpl6 = ret_tmp_chpl;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  chpl___PLUS__ASSIGN_(&str_chpl, &call_tmp_chpl6, _ln_chpl, _fn_chpl);
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  i_x_chpl2 = &call_tmp_chpl6;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  deinit_chpl95(i_x_chpl2, _ln_chpl, _fn_chpl);
#line 80 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  chpl___PLUS__ASSIGN_(&str_chpl, _e2_args_chpl, _ln_chpl, _fn_chpl);
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  chpl___COLON_25(_e3_args_chpl, &ret_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  call_tmp_chpl7 = ret_tmp_chpl2;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  chpl___PLUS__ASSIGN_(&str_chpl, &call_tmp_chpl7, _ln_chpl, _fn_chpl);
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  i_x_chpl3 = &call_tmp_chpl7;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  deinit_chpl95(i_x_chpl3, _ln_chpl, _fn_chpl);
#line 100 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  ret_chpl = str_chpl;
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  *(_retArg_chpl) = ret_chpl;
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  return;
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
}

#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
/* ChapelIOStringifyHelper.chpl:70 */
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
static void stringify_simple_chpl14(string_chpl * _e0_args_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
                                    string_chpl * _e1_args_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
                                    string_chpl * _e2_args_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
                                    string_chpl * _e3_args_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
                                    string_chpl * _retArg_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
                                    int64_t _ln_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
                                    int32_t _fn_chpl) {
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl ret_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl str_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl inlineImm_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl ret_chpl2;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl tmp_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  inlineImm_chpl = _str_literal_163_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  i_x_chpl = &inlineImm_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  init_ASSIGN__chpl25(&tmp_chpl, i_x_chpl, _ln_chpl, _fn_chpl);
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  ret_chpl2 = tmp_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  str_chpl = ret_chpl2;
#line 80 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  chpl___PLUS__ASSIGN_(&str_chpl, _e0_args_chpl, _ln_chpl, _fn_chpl);
#line 80 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  chpl___PLUS__ASSIGN_(&str_chpl, _e1_args_chpl, _ln_chpl, _fn_chpl);
#line 80 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  chpl___PLUS__ASSIGN_(&str_chpl, _e2_args_chpl, _ln_chpl, _fn_chpl);
#line 80 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  chpl___PLUS__ASSIGN_(&str_chpl, _e3_args_chpl, _ln_chpl, _fn_chpl);
#line 100 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  ret_chpl = str_chpl;
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  *(_retArg_chpl) = ret_chpl;
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  return;
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
}

#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
/* ChapelIOStringifyHelper.chpl:70 */
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
static void stringify_simple_chpl6(string_chpl * _e0_args_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
                                   uint64_t _e1_args_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
                                   string_chpl * _e2_args_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
                                   uint64_t _e3_args_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
                                   string_chpl * _retArg_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
                                   int64_t _ln_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
                                   int32_t _fn_chpl) {
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl ret_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl str_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl inlineImm_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl ret_chpl2;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl tmp_chpl;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl call_tmp_chpl6;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl ret_tmp_chpl;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  _ref_string_chpl i_x_chpl2 = NULL;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl call_tmp_chpl7;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl ret_tmp_chpl2;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  _ref_string_chpl i_x_chpl3 = NULL;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  inlineImm_chpl = _str_literal_163_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  i_x_chpl = &inlineImm_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  init_ASSIGN__chpl25(&tmp_chpl, i_x_chpl, _ln_chpl, _fn_chpl);
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  ret_chpl2 = tmp_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  str_chpl = ret_chpl2;
#line 80 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  chpl___PLUS__ASSIGN_(&str_chpl, _e0_args_chpl, _ln_chpl, _fn_chpl);
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  chpl___COLON_24(_e1_args_chpl, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  call_tmp_chpl6 = ret_tmp_chpl;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  chpl___PLUS__ASSIGN_(&str_chpl, &call_tmp_chpl6, _ln_chpl, _fn_chpl);
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  i_x_chpl2 = &call_tmp_chpl6;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  deinit_chpl95(i_x_chpl2, _ln_chpl, _fn_chpl);
#line 80 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  chpl___PLUS__ASSIGN_(&str_chpl, _e2_args_chpl, _ln_chpl, _fn_chpl);
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  chpl___COLON_24(_e3_args_chpl, &ret_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  call_tmp_chpl7 = ret_tmp_chpl2;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  chpl___PLUS__ASSIGN_(&str_chpl, &call_tmp_chpl7, _ln_chpl, _fn_chpl);
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  i_x_chpl3 = &call_tmp_chpl7;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  deinit_chpl95(i_x_chpl3, _ln_chpl, _fn_chpl);
#line 100 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  ret_chpl = str_chpl;
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  *(_retArg_chpl) = ret_chpl;
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  return;
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
}

#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
/* ChapelIOStringifyHelper.chpl:70 */
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
static void stringify_simple_chpl10(string_chpl * _e0_args_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
                                    int64_t _e1_args_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
                                    string_chpl * _e2_args_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
                                    range_int64_t_neither_one_chpl _e3_args_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
                                    string_chpl * _retArg_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
                                    int64_t _ln_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
                                    int32_t _fn_chpl) {
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl ret_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl str_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl inlineImm_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl ret_chpl2;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl tmp_chpl;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl call_tmp_chpl6;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl ret_tmp_chpl;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  _ref_string_chpl i_x_chpl2 = NULL;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl call_tmp_chpl7;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl ret_tmp_chpl2;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  _ref_string_chpl i_x_chpl3 = NULL;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  inlineImm_chpl = _str_literal_163_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  i_x_chpl = &inlineImm_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  init_ASSIGN__chpl25(&tmp_chpl, i_x_chpl, _ln_chpl, _fn_chpl);
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  ret_chpl2 = tmp_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  str_chpl = ret_chpl2;
#line 80 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  chpl___PLUS__ASSIGN_(&str_chpl, _e0_args_chpl, _ln_chpl, _fn_chpl);
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  chpl___COLON_25(_e1_args_chpl, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  call_tmp_chpl6 = ret_tmp_chpl;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  chpl___PLUS__ASSIGN_(&str_chpl, &call_tmp_chpl6, _ln_chpl, _fn_chpl);
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  i_x_chpl2 = &call_tmp_chpl6;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  deinit_chpl95(i_x_chpl2, _ln_chpl, _fn_chpl);
#line 80 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  chpl___PLUS__ASSIGN_(&str_chpl, _e2_args_chpl, _ln_chpl, _fn_chpl);
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  chpl___COLON_15(_e3_args_chpl, &ret_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  call_tmp_chpl7 = ret_tmp_chpl2;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  chpl___PLUS__ASSIGN_(&str_chpl, &call_tmp_chpl7, _ln_chpl, _fn_chpl);
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  i_x_chpl3 = &call_tmp_chpl7;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  deinit_chpl95(i_x_chpl3, _ln_chpl, _fn_chpl);
#line 100 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  ret_chpl = str_chpl;
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  *(_retArg_chpl) = ret_chpl;
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  return;
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
}

#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
/* ChapelIOStringifyHelper.chpl:70 */
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
static void stringify_simple_chpl16(string_chpl * _e0_args_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
                                    int64_t _e1_args_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
                                    string_chpl * _e2_args_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
                                    range_int64_t_both_one_chpl _e3_args_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
                                    string_chpl * _retArg_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
                                    int64_t _ln_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
                                    int32_t _fn_chpl) {
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl ret_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl str_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl inlineImm_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl ret_chpl2;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl tmp_chpl;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl call_tmp_chpl6;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl ret_tmp_chpl;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  _ref_string_chpl i_x_chpl2 = NULL;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl call_tmp_chpl7;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl ret_tmp_chpl2;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  _ref_string_chpl i_x_chpl3 = NULL;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  inlineImm_chpl = _str_literal_163_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  i_x_chpl = &inlineImm_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  init_ASSIGN__chpl25(&tmp_chpl, i_x_chpl, _ln_chpl, _fn_chpl);
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  ret_chpl2 = tmp_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  str_chpl = ret_chpl2;
#line 80 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  chpl___PLUS__ASSIGN_(&str_chpl, _e0_args_chpl, _ln_chpl, _fn_chpl);
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  chpl___COLON_25(_e1_args_chpl, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  call_tmp_chpl6 = ret_tmp_chpl;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  chpl___PLUS__ASSIGN_(&str_chpl, &call_tmp_chpl6, _ln_chpl, _fn_chpl);
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  i_x_chpl2 = &call_tmp_chpl6;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  deinit_chpl95(i_x_chpl2, _ln_chpl, _fn_chpl);
#line 80 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  chpl___PLUS__ASSIGN_(&str_chpl, _e2_args_chpl, _ln_chpl, _fn_chpl);
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  chpl___COLON_16(_e3_args_chpl, &ret_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  call_tmp_chpl7 = ret_tmp_chpl2;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  chpl___PLUS__ASSIGN_(&str_chpl, &call_tmp_chpl7, _ln_chpl, _fn_chpl);
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  i_x_chpl3 = &call_tmp_chpl7;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  deinit_chpl95(i_x_chpl3, _ln_chpl, _fn_chpl);
#line 100 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  ret_chpl = str_chpl;
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  *(_retArg_chpl) = ret_chpl;
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  return;
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
}

#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
/* ChapelIOStringifyHelper.chpl:70 */
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
static void stringify_simple_chpl7(string_chpl * _e0_args_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
                                   uint64_t _e1_args_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
                                   string_chpl * _retArg_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
                                   int64_t _ln_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
                                   int32_t _fn_chpl) {
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl ret_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl str_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl inlineImm_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl ret_chpl2;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl tmp_chpl;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl call_tmp_chpl6;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl ret_tmp_chpl;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  _ref_string_chpl i_x_chpl2 = NULL;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  inlineImm_chpl = _str_literal_163_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  i_x_chpl = &inlineImm_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  init_ASSIGN__chpl25(&tmp_chpl, i_x_chpl, _ln_chpl, _fn_chpl);
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  ret_chpl2 = tmp_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  str_chpl = ret_chpl2;
#line 80 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  chpl___PLUS__ASSIGN_(&str_chpl, _e0_args_chpl, _ln_chpl, _fn_chpl);
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  chpl___COLON_24(_e1_args_chpl, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  call_tmp_chpl6 = ret_tmp_chpl;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  chpl___PLUS__ASSIGN_(&str_chpl, &call_tmp_chpl6, _ln_chpl, _fn_chpl);
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  i_x_chpl2 = &call_tmp_chpl6;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  deinit_chpl95(i_x_chpl2, _ln_chpl, _fn_chpl);
#line 100 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  ret_chpl = str_chpl;
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  *(_retArg_chpl) = ret_chpl;
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  return;
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
}

#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
/* ChapelIOStringifyHelper.chpl:70 */
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
static void stringify_simple_chpl11(string_chpl * _e0_args_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
                                    int64_t _e1_args_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
                                    string_chpl * _retArg_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
                                    int64_t _ln_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
                                    int32_t _fn_chpl) {
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl ret_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl str_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl inlineImm_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl ret_chpl2;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl tmp_chpl;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl call_tmp_chpl6;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl ret_tmp_chpl;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  _ref_string_chpl i_x_chpl2 = NULL;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  inlineImm_chpl = _str_literal_163_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  i_x_chpl = &inlineImm_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  init_ASSIGN__chpl25(&tmp_chpl, i_x_chpl, _ln_chpl, _fn_chpl);
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  ret_chpl2 = tmp_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  str_chpl = ret_chpl2;
#line 80 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  chpl___PLUS__ASSIGN_(&str_chpl, _e0_args_chpl, _ln_chpl, _fn_chpl);
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  chpl___COLON_25(_e1_args_chpl, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  call_tmp_chpl6 = ret_tmp_chpl;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  chpl___PLUS__ASSIGN_(&str_chpl, &call_tmp_chpl6, _ln_chpl, _fn_chpl);
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  i_x_chpl2 = &call_tmp_chpl6;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  deinit_chpl95(i_x_chpl2, _ln_chpl, _fn_chpl);
#line 100 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  ret_chpl = str_chpl;
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  *(_retArg_chpl) = ret_chpl;
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  return;
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
}

#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
/* ChapelIOStringifyHelper.chpl:70 */
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
static void stringify_simple_chpl13(string_chpl * _e0_args_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
                                    string_chpl * _e1_args_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
                                    string_chpl * _e2_args_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
                                    string_chpl * _e3_args_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
                                    string_chpl * _e4_args_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
                                    string_chpl * _e5_args_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
                                    string_chpl * _e6_args_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
                                    string_chpl * _e7_args_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
                                    string_chpl * _e8_args_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
                                    string_chpl * _retArg_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
                                    int64_t _ln_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
                                    int32_t _fn_chpl) {
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  _tuple_9_star__ref_string_chpl args_chpl;
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  _tuple_9_star__ref_string_chpl this_chpl31;
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl ret_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl str_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl inlineImm_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl ret_chpl2;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl tmp_chpl;
#line 80 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  _ref_string_chpl call_tmp_chpl6 = NULL;
#line 80 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  _ref_string_chpl call_tmp_chpl7 = NULL;
#line 80 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  _ref_string_chpl call_tmp_chpl8 = NULL;
#line 80 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  _ref_string_chpl call_tmp_chpl9 = NULL;
#line 80 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  _ref_string_chpl call_tmp_chpl10 = NULL;
#line 80 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  _ref_string_chpl call_tmp_chpl11 = NULL;
#line 80 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  _ref_string_chpl call_tmp_chpl12 = NULL;
#line 80 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  _ref_string_chpl call_tmp_chpl13 = NULL;
#line 80 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  _ref_string_chpl call_tmp_chpl14 = NULL;
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  *(this_chpl31 + INT64(0)) = _e0_args_chpl;
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  *(this_chpl31 + INT64(1)) = _e1_args_chpl;
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  *(this_chpl31 + INT64(2)) = _e2_args_chpl;
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  *(this_chpl31 + INT64(3)) = _e3_args_chpl;
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  *(this_chpl31 + INT64(4)) = _e4_args_chpl;
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  *(this_chpl31 + INT64(5)) = _e5_args_chpl;
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  *(this_chpl31 + INT64(6)) = _e6_args_chpl;
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  *(this_chpl31 + INT64(7)) = _e7_args_chpl;
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  *(this_chpl31 + INT64(8)) = _e8_args_chpl;
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  memcpy(&args_chpl, &this_chpl31, sizeof(_tuple_9_star__ref_string_chpl));
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  inlineImm_chpl = _str_literal_163_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  i_x_chpl = &inlineImm_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  init_ASSIGN__chpl25(&tmp_chpl, i_x_chpl, _ln_chpl, _fn_chpl);
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  ret_chpl2 = tmp_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  str_chpl = ret_chpl2;
#line 80 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  call_tmp_chpl6 = *(args_chpl + INT64(0));
#line 80 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  chpl___PLUS__ASSIGN_(&str_chpl, call_tmp_chpl6, _ln_chpl, _fn_chpl);
#line 80 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  call_tmp_chpl7 = *(args_chpl + INT64(1));
#line 80 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  chpl___PLUS__ASSIGN_(&str_chpl, call_tmp_chpl7, _ln_chpl, _fn_chpl);
#line 80 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  call_tmp_chpl8 = *(args_chpl + INT64(2));
#line 80 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  chpl___PLUS__ASSIGN_(&str_chpl, call_tmp_chpl8, _ln_chpl, _fn_chpl);
#line 80 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  call_tmp_chpl9 = *(args_chpl + INT64(3));
#line 80 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  chpl___PLUS__ASSIGN_(&str_chpl, call_tmp_chpl9, _ln_chpl, _fn_chpl);
#line 80 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  call_tmp_chpl10 = *(args_chpl + INT64(4));
#line 80 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  chpl___PLUS__ASSIGN_(&str_chpl, call_tmp_chpl10, _ln_chpl, _fn_chpl);
#line 80 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  call_tmp_chpl11 = *(args_chpl + INT64(5));
#line 80 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  chpl___PLUS__ASSIGN_(&str_chpl, call_tmp_chpl11, _ln_chpl, _fn_chpl);
#line 80 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  call_tmp_chpl12 = *(args_chpl + INT64(6));
#line 80 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  chpl___PLUS__ASSIGN_(&str_chpl, call_tmp_chpl12, _ln_chpl, _fn_chpl);
#line 80 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  call_tmp_chpl13 = *(args_chpl + INT64(7));
#line 80 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  chpl___PLUS__ASSIGN_(&str_chpl, call_tmp_chpl13, _ln_chpl, _fn_chpl);
#line 80 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  call_tmp_chpl14 = *(args_chpl + INT64(8));
#line 80 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  chpl___PLUS__ASSIGN_(&str_chpl, call_tmp_chpl14, _ln_chpl, _fn_chpl);
#line 100 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  ret_chpl = str_chpl;
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  *(_retArg_chpl) = ret_chpl;
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  return;
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
}

#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
/* ChapelIOStringifyHelper.chpl:70 */
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
static void stringify_simple_chpl2(string_chpl * _e0_args_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
                                   string_chpl * _e1_args_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
                                   int64_t _e2_args_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
                                   string_chpl * _e3_args_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
                                   uint64_t _e4_args_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
                                   string_chpl * _e5_args_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
                                   uint64_t _e6_args_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
                                   string_chpl * _retArg_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
                                   int64_t _ln_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
                                   int32_t _fn_chpl) {
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl ret_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl str_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl inlineImm_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl ret_chpl2;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl tmp_chpl;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl call_tmp_chpl6;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl ret_tmp_chpl;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  _ref_string_chpl i_x_chpl2 = NULL;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl call_tmp_chpl7;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl ret_tmp_chpl2;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  _ref_string_chpl i_x_chpl3 = NULL;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl call_tmp_chpl8;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  string_chpl ret_tmp_chpl3;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  _ref_string_chpl i_x_chpl4 = NULL;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  inlineImm_chpl = _str_literal_163_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  i_x_chpl = &inlineImm_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  init_ASSIGN__chpl25(&tmp_chpl, i_x_chpl, _ln_chpl, _fn_chpl);
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  ret_chpl2 = tmp_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  str_chpl = ret_chpl2;
#line 80 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  chpl___PLUS__ASSIGN_(&str_chpl, _e0_args_chpl, _ln_chpl, _fn_chpl);
#line 80 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  chpl___PLUS__ASSIGN_(&str_chpl, _e1_args_chpl, _ln_chpl, _fn_chpl);
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  chpl___COLON_25(_e2_args_chpl, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  call_tmp_chpl6 = ret_tmp_chpl;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  chpl___PLUS__ASSIGN_(&str_chpl, &call_tmp_chpl6, _ln_chpl, _fn_chpl);
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  i_x_chpl2 = &call_tmp_chpl6;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  deinit_chpl95(i_x_chpl2, _ln_chpl, _fn_chpl);
#line 80 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  chpl___PLUS__ASSIGN_(&str_chpl, _e3_args_chpl, _ln_chpl, _fn_chpl);
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  chpl___COLON_24(_e4_args_chpl, &ret_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  call_tmp_chpl7 = ret_tmp_chpl2;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  chpl___PLUS__ASSIGN_(&str_chpl, &call_tmp_chpl7, _ln_chpl, _fn_chpl);
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  i_x_chpl3 = &call_tmp_chpl7;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  deinit_chpl95(i_x_chpl3, _ln_chpl, _fn_chpl);
#line 80 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  chpl___PLUS__ASSIGN_(&str_chpl, _e5_args_chpl, _ln_chpl, _fn_chpl);
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  chpl___COLON_24(_e6_args_chpl, &ret_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  call_tmp_chpl8 = ret_tmp_chpl3;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  chpl___PLUS__ASSIGN_(&str_chpl, &call_tmp_chpl8, _ln_chpl, _fn_chpl);
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  i_x_chpl4 = &call_tmp_chpl8;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  deinit_chpl95(i_x_chpl4, _ln_chpl, _fn_chpl);
#line 100 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  ret_chpl = str_chpl;
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  *(_retArg_chpl) = ret_chpl;
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
  return;
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelIOStringifyHelper.chpl"
}

