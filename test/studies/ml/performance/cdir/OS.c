#line 36 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
/* OS.chpl:36 */
#line 36 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
static void chpl__init_OS(int64_t _ln_chpl,
#line 36 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                          int32_t _fn_chpl) {
#line 36 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _ref_int32_t_chpl refIndentLevel_chpl = NULL;
#line 36 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  if (chpl__init_OS_p) /* ZLINE: 36 /Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl */
#line 36 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  {
#line 36 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    goto _exit_chpl__init_OS_chpl;
#line 36 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  }
#line 36 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  printModuleInit_chpl("%*s\n", "OS", INT64(2), _ln_chpl, _fn_chpl);
#line 36 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  refIndentLevel_chpl = &moduleInitLevel_chpl;
#line 36 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  *(refIndentLevel_chpl) += INT64(1);
#line 36 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl__init_OS_p = UINT8(true);
#line 36 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  {
#line 36 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    chpl__init_CTypes(_ln_chpl, _fn_chpl);
#line 36 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    chpl__init_POSIX(_ln_chpl, _fn_chpl);
#line 36 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  }
#line 36 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  *(refIndentLevel_chpl) -= INT64(1);
#line 36 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _exit_chpl__init_OS_chpl:;
#line 36 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  return;
#line 36 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
}

#line 1124 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
/* OS.chpl:1124 */
#line 1124 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
static void chpl__auto_destroy_SystemError(SystemError_chpl this_chpl31,
#line 1124 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                                           int64_t _ln_chpl,
#line 1124 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                                           int32_t _fn_chpl) {
#line 1126 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _ref_string_chpl _field_destructor_tmp__chpl = NULL;
#line 1124 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  Error_chpl _parent_destructor_tmp__chpl = NULL;
#line 1124 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _ref_string_chpl _field_destructor_tmp__chpl2 = NULL;
#line 1126 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 1126 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _field_destructor_tmp__chpl = &((this_chpl31)->details);
#line 1126 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  deinit_chpl95(_field_destructor_tmp__chpl, _ln_chpl, _fn_chpl);
#line 1124 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _parent_destructor_tmp__chpl = ((Error_chpl)(this_chpl31));
#line 1124 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(_parent_destructor_tmp__chpl, _ln_chpl, _fn_chpl);
#line 1124 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _field_destructor_tmp__chpl2 = &((_parent_destructor_tmp__chpl)->_msg);
#line 1124 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  deinit_chpl95(_field_destructor_tmp__chpl2, _ln_chpl, _fn_chpl);
#line 1124 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  return;
#line 1124 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
}

#line 1128 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
/* OS.chpl:1128 */
#line 1128 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
static SystemError_chpl _new_chpl67(syserr err_chpl8,
#line 1128 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                                    string_chpl * details_chpl,
#line 1128 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                                    int64_t _ln_chpl,
#line 1128 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                                    int32_t _fn_chpl) {
#line 1128 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  SystemError_chpl initTemp_chpl = NULL;
#line 1128 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  c_ptr_void_chpl cast_tmp_chpl = NULL;
#line 1128 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  Error_chpl super_tmp_chpl = NULL;
#line 1128 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  Error_chpl init_coerce_tmp_chpl = NULL;
#line 1128 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  string_chpl _msg_chpl;
#line 1128 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  string_chpl details_chpl2;
#line 1128 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  cast_tmp_chpl = chpl_here_alloc(sizeof(chpl_SystemError_chpl_object), INT16(26), _ln_chpl, _fn_chpl);
#line 1128 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  initTemp_chpl = ((SystemError_chpl)(cast_tmp_chpl));
#line 1128 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(initTemp_chpl))->chpl__cid = chpl__cid_SystemError_chpl;
#line 1128 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(initTemp_chpl, _ln_chpl, _fn_chpl);
#line 1128 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  super_tmp_chpl = &((initTemp_chpl)->super);
#line 1128 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 1128 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(&((super_tmp_chpl)->super)))->chpl__cid = chpl__cid_RootClass_chpl;
#line 1128 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(((RootClass_chpl)(super_tmp_chpl))))->chpl__cid = chpl__cid_RootClass_chpl;
#line 1128 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  init_coerce_tmp_chpl = ((Error_chpl)(nil));
#line 1128 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 1128 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  (super_tmp_chpl)->_next = init_coerce_tmp_chpl;
#line 1128 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 1128 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  (super_tmp_chpl)->thrownLine = INT64(0);
#line 1128 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 1128 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  (super_tmp_chpl)->thrownFileId = INT32(0);
#line 1128 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  init_chpl194(&_msg_chpl, _ln_chpl, _fn_chpl);
#line 1128 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 1128 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  (super_tmp_chpl)->_msg = _msg_chpl;
#line 1128 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 1128 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  (super_tmp_chpl)->_hasThrowInfo = UINT8(false);
#line 1128 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(super_tmp_chpl))->chpl__cid = chpl__cid_Error_chpl;
#line 1128 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(((Error_chpl)(initTemp_chpl))))->chpl__cid = chpl__cid_Error_chpl;
#line 1128 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(initTemp_chpl, _ln_chpl, _fn_chpl);
#line 1128 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  (initTemp_chpl)->err = err_chpl8;
#line 1128 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  init_ASSIGN__chpl25(&details_chpl2, details_chpl, _ln_chpl, _fn_chpl);
#line 1128 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(initTemp_chpl, _ln_chpl, _fn_chpl);
#line 1128 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  (initTemp_chpl)->details = details_chpl2;
#line 1128 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(initTemp_chpl))->chpl__cid = chpl__cid_SystemError_chpl;
#line 1128 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  return initTemp_chpl;
#line 1128 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
}

#line 1137 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
/* OS.chpl:1137 */
#line 1137 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
static void message_chpl7(SystemError_chpl this_chpl31,
#line 1137 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                          string_chpl * _retArg_chpl,
#line 1137 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                          int64_t _ln_chpl,
#line 1137 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                          int32_t _fn_chpl) {
#line 1137 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  string_chpl ret_chpl;
#line 1139 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  c_ptrConst_int8_t_chpl call_tmp_chpl6 = NULL;
#line 1139 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  syserr coerce_tmp_chpl3;
#line 1139 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  int32_t _formal_tmp_out_err_in_strerror_chpl;
#line 1140 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  string_chpl err_msg_chpl;
#line 1141 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  Error_chpl error_chpl = NULL;
#line 1142 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  string_chpl call_tmp_chpl7;
#line 1142 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  int64_t call_tmp_chpl8;
#line 1142 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _owned_IllegalArgumentError_chpl2 call_tmp_chpl9;
#line 1142 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _owned_IllegalArgumentError_chpl2 ret_tmp_chpl;
#line 1142 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _ref__owned_IllegalArgumentError_chpl i_arg_chpl = NULL;
#line 1142 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  IllegalArgumentError_chpl call_tmp_chpl10 = NULL;
#line 1142 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  string_chpl call_tmp_chpl11;
#line 1142 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  Error_chpl coerce_tmp_chpl4 = NULL;
#line 1142 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  string_chpl ret_tmp_chpl2;
#line 1142 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 1142 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  string_chpl ret_tmp_chpl3;
#line 1145 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _ref_string_chpl call_tmp_chpl12 = NULL;
#line 1146 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _ref_string_chpl call_tmp_chpl13 = NULL;
#line 1146 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  string_chpl call_tmp_chpl14;
#line 1146 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  string_chpl ret_tmp_chpl4;
#line 1146 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  string_chpl call_tmp_chpl15;
#line 1146 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  string_chpl ret_tmp_chpl5;
#line 1146 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _ref_string_chpl i_x_chpl2 = NULL;
#line 1146 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _ref_string_chpl i_x_chpl3 = NULL;
#line 1139 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 1139 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  coerce_tmp_chpl3 = (this_chpl31)->err;
#line 1139 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  call_tmp_chpl6 = sys_strerror_syserr_str(coerce_tmp_chpl3, &_formal_tmp_out_err_in_strerror_chpl);
#line 1141 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  error_chpl = NULL;
#line 1142 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  call_tmp_chpl8 = string_length_bytes(call_tmp_chpl6);
#line 1142 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_checkValue2(call_tmp_chpl8, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 1142 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  call_tmp_chpl9 = ret_tmp_chpl;
#line 1142 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  i_arg_chpl = &call_tmp_chpl9;
#line 1142 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  call_tmp_chpl10 = borrow_chpl2(i_arg_chpl);
#line 1142 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  if (((RootClass_chpl)(call_tmp_chpl10)) != nil) /* ZLINE: 1142 /Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl */
#line 1142 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  {
#line 1142 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    coerce_tmp_chpl4 = ((Error_chpl)(call_tmp_chpl10));
#line 1142 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    chpl_check_nil(coerce_tmp_chpl4, _ln_chpl, _fn_chpl);
#line 1142 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    ((void(*)(Error_chpl,
#line 1142 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
       string_chpl *,
#line 1142 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
       int64_t,
#line 1142 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
       int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(coerce_tmp_chpl4))->chpl__cid) + INT64(1))])(coerce_tmp_chpl4, &ret_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 1142 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    call_tmp_chpl11 = ret_tmp_chpl2;
#line 1142 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    safeCastCheckHalt_chpl(&call_tmp_chpl11, _ln_chpl, _fn_chpl);
#line 1142 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    i_x_chpl = &call_tmp_chpl11;
#line 1142 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    deinit_chpl95(i_x_chpl, _ln_chpl, _fn_chpl);
#line 1142 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  }
#line 1142 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl__autoDestroy62(&call_tmp_chpl9, _ln_chpl, _fn_chpl);
#line 1142 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  if (error_chpl != nil) /* ZLINE: 1142 /Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl */
#line 1142 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  {
#line 1142 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    goto handler_chpl;
#line 1142 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  }
#line 1142 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  createAdoptingBuffer_chpl(call_tmp_chpl6, call_tmp_chpl8, &error_chpl, &ret_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 1142 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  call_tmp_chpl7 = ret_tmp_chpl3;
#line 1142 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  if (error_chpl != nil) /* ZLINE: 1142 /Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl */
#line 1142 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  {
#line 1142 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    goto handler_chpl;
#line 1142 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  }
#line 1142 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  err_msg_chpl = call_tmp_chpl7;
#line 1142 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  handler_chpl:;
#line 1141 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  if (error_chpl != nil) /* ZLINE: 1141 /Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl */
#line 1141 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  {
#line 1141 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    chpl_uncaught_error(error_chpl, _ln_chpl, _fn_chpl);
#line 1141 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  }
#line 1145 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 1145 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  call_tmp_chpl12 = &((this_chpl31)->details);
#line 1145 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  if (! ((call_tmp_chpl12)->buffLen == INT64(0))) /* ZLINE: 1145 /Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl */
#line 1145 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  {
#line 1146 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 1146 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    call_tmp_chpl13 = &((this_chpl31)->details);
#line 1146 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    chpl___PLUS_(&_str_literal_734_chpl, call_tmp_chpl13, &ret_tmp_chpl4, _ln_chpl, _fn_chpl);
#line 1146 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    call_tmp_chpl14 = ret_tmp_chpl4;
#line 1146 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    chpl___PLUS_(&call_tmp_chpl14, &_str_literal_415_chpl, &ret_tmp_chpl5, _ln_chpl, _fn_chpl);
#line 1146 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    call_tmp_chpl15 = ret_tmp_chpl5;
#line 1146 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    chpl___PLUS__ASSIGN_(&err_msg_chpl, &call_tmp_chpl15, _ln_chpl, _fn_chpl);
#line 1146 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    i_x_chpl2 = &call_tmp_chpl15;
#line 1146 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    deinit_chpl95(i_x_chpl2, _ln_chpl, _fn_chpl);
#line 1146 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    i_x_chpl3 = &call_tmp_chpl14;
#line 1146 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    deinit_chpl95(i_x_chpl3, _ln_chpl, _fn_chpl);
#line 1146 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  }
#line 1148 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ret_chpl = err_msg_chpl;
#line 1137 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  *(_retArg_chpl) = ret_chpl;
#line 1137 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  return;
#line 1137 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
}

#line 1160 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
/* OS.chpl:1160 */
#line 1160 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
static void createSystemOrChplError_chpl(syserr err_chpl8,
#line 1160 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                                         string_chpl * details_chpl,
#line 1160 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                                         _owned_Error_chpl * _retArg_chpl,
#line 1160 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                                         int64_t _ln_chpl,
#line 1160 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                                         int32_t _fn_chpl) {
#line 1160 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _owned_Error_chpl ret_chpl;
#line 1163 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  c_ptrConst_int8_t_chpl call_tmp_chpl6 = NULL;
#line 1163 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  int32_t _formal_tmp_out_err_in_strerror_chpl;
#line 1164 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  string_chpl err_msg_chpl;
#line 1165 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  Error_chpl error_chpl = NULL;
#line 1166 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  string_chpl call_tmp_chpl7;
#line 1166 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  int64_t call_tmp_chpl8;
#line 1166 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _owned_IllegalArgumentError_chpl2 call_tmp_chpl9;
#line 1166 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _owned_IllegalArgumentError_chpl2 ret_tmp_chpl;
#line 1166 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _ref__owned_IllegalArgumentError_chpl i_arg_chpl = NULL;
#line 1166 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  IllegalArgumentError_chpl call_tmp_chpl10 = NULL;
#line 1166 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  string_chpl call_tmp_chpl11;
#line 1166 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  Error_chpl coerce_tmp_chpl3 = NULL;
#line 1166 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  string_chpl ret_tmp_chpl2;
#line 1166 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 1166 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  string_chpl ret_tmp_chpl3;
#line 1171 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  int32_t call_tmp_chpl12;
#line 1170 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  int32_t call_tmp_chpl13;
#line 1171 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _owned_EofError_chpl call_tmp_chpl14;
#line 1171 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  EofError_chpl new_temp_chpl = NULL;
#line 1171 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _owned_EofError_chpl initTemp_chpl;
#line 1171 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _ref_string_chpl i_x_chpl2 = NULL;
#line 1172 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  int32_t call_tmp_chpl15;
#line 1170 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  int32_t call_tmp_chpl16;
#line 1172 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _owned_UnexpectedEofError_chpl call_tmp_chpl17;
#line 1172 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  UnexpectedEofError_chpl new_temp_chpl2 = NULL;
#line 1172 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _owned_UnexpectedEofError_chpl initTemp_chpl2;
#line 1172 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _ref_string_chpl i_x_chpl3 = NULL;
#line 1173 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  int32_t call_tmp_chpl18;
#line 1170 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  int32_t call_tmp_chpl19;
#line 1173 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _owned_BadFormatError_chpl call_tmp_chpl20;
#line 1173 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  BadFormatError_chpl new_temp_chpl3 = NULL;
#line 1173 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _owned_BadFormatError_chpl initTemp_chpl3;
#line 1173 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _ref_string_chpl i_x_chpl4 = NULL;
#line 1174 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _owned_SystemError_chpl call_tmp_chpl21;
#line 1174 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _owned_SystemError_chpl ret_tmp_chpl4;
#line 1174 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _ref_string_chpl i_x_chpl5 = NULL;
#line 1163 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  call_tmp_chpl6 = sys_strerror_syserr_str(err_chpl8, &_formal_tmp_out_err_in_strerror_chpl);
#line 1165 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  error_chpl = NULL;
#line 1166 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  call_tmp_chpl8 = string_length_bytes(call_tmp_chpl6);
#line 1166 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_checkValue2(call_tmp_chpl8, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 1166 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  call_tmp_chpl9 = ret_tmp_chpl;
#line 1166 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  i_arg_chpl = &call_tmp_chpl9;
#line 1166 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  call_tmp_chpl10 = borrow_chpl2(i_arg_chpl);
#line 1166 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  if (((RootClass_chpl)(call_tmp_chpl10)) != nil) /* ZLINE: 1166 /Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl */
#line 1166 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  {
#line 1166 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    coerce_tmp_chpl3 = ((Error_chpl)(call_tmp_chpl10));
#line 1166 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    chpl_check_nil(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 1166 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    ((void(*)(Error_chpl,
#line 1166 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
       string_chpl *,
#line 1166 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
       int64_t,
#line 1166 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
       int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(coerce_tmp_chpl3))->chpl__cid) + INT64(1))])(coerce_tmp_chpl3, &ret_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 1166 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    call_tmp_chpl11 = ret_tmp_chpl2;
#line 1166 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    safeCastCheckHalt_chpl(&call_tmp_chpl11, _ln_chpl, _fn_chpl);
#line 1166 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    i_x_chpl = &call_tmp_chpl11;
#line 1166 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    deinit_chpl95(i_x_chpl, _ln_chpl, _fn_chpl);
#line 1166 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  }
#line 1166 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl__autoDestroy62(&call_tmp_chpl9, _ln_chpl, _fn_chpl);
#line 1166 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  if (error_chpl != nil) /* ZLINE: 1166 /Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl */
#line 1166 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  {
#line 1166 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    goto handler_chpl;
#line 1166 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  }
#line 1166 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  createAdoptingBuffer_chpl(call_tmp_chpl6, call_tmp_chpl8, &error_chpl, &ret_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 1166 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  call_tmp_chpl7 = ret_tmp_chpl3;
#line 1166 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  if (error_chpl != nil) /* ZLINE: 1166 /Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl */
#line 1166 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  {
#line 1166 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    goto handler_chpl;
#line 1166 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  }
#line 1166 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  err_msg_chpl = call_tmp_chpl7;
#line 1166 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  handler_chpl:;
#line 1165 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  if (error_chpl != nil) /* ZLINE: 1165 /Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl */
#line 1165 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  {
#line 1165 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    chpl_uncaught_error(error_chpl, _ln_chpl, _fn_chpl);
#line 1165 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  }
#line 1171 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  call_tmp_chpl12 = chpl_macro_int_EEOF();
#line 1170 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  call_tmp_chpl13 = qio_err_to_int(err_chpl8);
#line 1170 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  if (call_tmp_chpl13 == call_tmp_chpl12) /* ZLINE: 1171 /Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl */
#line 1170 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  {
#line 1171 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    new_temp_chpl = _new_chpl83(details_chpl, &err_msg_chpl, _ln_chpl, _fn_chpl);
#line 1171 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    init_chpl174(&initTemp_chpl, new_temp_chpl);
#line 1171 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    call_tmp_chpl14 = initTemp_chpl;
#line 1171 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    init_ASSIGN__chpl18(&ret_chpl, &call_tmp_chpl14, _ln_chpl, _fn_chpl);
#line 1171 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    chpl__autoDestroy53(&call_tmp_chpl14, _ln_chpl, _fn_chpl);
#line 1171 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    i_x_chpl2 = &err_msg_chpl;
#line 1171 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    deinit_chpl95(i_x_chpl2, _ln_chpl, _fn_chpl);
#line 1171 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    goto _end_createSystemOrChplError_chpl;
#line 1171 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  } else /* ZLINE: 1170 /Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl */
#line 1171 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  {
#line 1172 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    call_tmp_chpl15 = chpl_macro_int_ESHORT();
#line 1170 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    call_tmp_chpl16 = qio_err_to_int(err_chpl8);
#line 1170 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    if (call_tmp_chpl16 == call_tmp_chpl15) /* ZLINE: 1172 /Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl */
#line 1170 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    {
#line 1172 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
      new_temp_chpl2 = _new_chpl84(details_chpl, &err_msg_chpl, _ln_chpl, _fn_chpl);
#line 1172 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
      init_chpl173(&initTemp_chpl2, new_temp_chpl2);
#line 1172 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
      call_tmp_chpl17 = initTemp_chpl2;
#line 1172 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
      init_ASSIGN__chpl19(&ret_chpl, &call_tmp_chpl17, _ln_chpl, _fn_chpl);
#line 1172 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
      chpl__autoDestroy52(&call_tmp_chpl17, _ln_chpl, _fn_chpl);
#line 1172 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
      i_x_chpl3 = &err_msg_chpl;
#line 1172 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
      deinit_chpl95(i_x_chpl3, _ln_chpl, _fn_chpl);
#line 1172 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
      goto _end_createSystemOrChplError_chpl;
#line 1172 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    } else /* ZLINE: 1170 /Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl */
#line 1172 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    {
#line 1173 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
      call_tmp_chpl18 = chpl_macro_int_EFORMAT();
#line 1170 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
      call_tmp_chpl19 = qio_err_to_int(err_chpl8);
#line 1170 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
      if (call_tmp_chpl19 == call_tmp_chpl18) /* ZLINE: 1173 /Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl */
#line 1170 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
      {
#line 1173 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
        new_temp_chpl3 = _new_chpl85(details_chpl, &err_msg_chpl, _ln_chpl, _fn_chpl);
#line 1173 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
        init_chpl178(&initTemp_chpl3, new_temp_chpl3);
#line 1173 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
        call_tmp_chpl20 = initTemp_chpl3;
#line 1173 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
        init_ASSIGN__chpl20(&ret_chpl, &call_tmp_chpl20, _ln_chpl, _fn_chpl);
#line 1173 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
        chpl__autoDestroy51(&call_tmp_chpl20, _ln_chpl, _fn_chpl);
#line 1173 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
        i_x_chpl4 = &err_msg_chpl;
#line 1173 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
        deinit_chpl95(i_x_chpl4, _ln_chpl, _fn_chpl);
#line 1173 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
        goto _end_createSystemOrChplError_chpl;
#line 1173 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
      } else /* ZLINE: 1174 /Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl */
#line 1173 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
      {
#line 1174 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
        createSystemError_chpl(err_chpl8, details_chpl, &ret_tmp_chpl4, _ln_chpl, _fn_chpl);
#line 1174 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
        call_tmp_chpl21 = ret_tmp_chpl4;
#line 1174 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
        init_ASSIGN__chpl21(&ret_chpl, &call_tmp_chpl21, _ln_chpl, _fn_chpl);
#line 1174 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
        chpl__autoDestroy54(&call_tmp_chpl21, _ln_chpl, _fn_chpl);
#line 1174 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
        i_x_chpl5 = &err_msg_chpl;
#line 1174 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
        deinit_chpl95(i_x_chpl5, _ln_chpl, _fn_chpl);
#line 1174 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
        goto _end_createSystemOrChplError_chpl;
#line 1174 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
      }
#line 1174 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    }
#line 1174 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  }
#line 1174 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _end_createSystemOrChplError_chpl:;
#line 1160 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  *(_retArg_chpl) = ret_chpl;
#line 1160 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  return;
#line 1160 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
}

#line 1184 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
/* OS.chpl:1184 */
#line 1184 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
static void createSystemOrChplError_chpl2(int64_t err_chpl8,
#line 1184 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                                          string_chpl * details_chpl,
#line 1184 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                                          _owned_Error_chpl * _retArg_chpl,
#line 1184 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                                          int64_t _ln_chpl,
#line 1184 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                                          int32_t _fn_chpl) {
#line 1184 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _owned_Error_chpl ret_chpl;
#line 1185 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  syserr call_tmp_chpl6;
#line 1185 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _owned_Error_chpl call_tmp_chpl7;
#line 1185 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _owned_Error_chpl ret_tmp_chpl;
#line 1185 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  call_tmp_chpl6 = qio_int_to_err(((int32_t)(err_chpl8)));
#line 1185 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  createSystemOrChplError_chpl(call_tmp_chpl6, details_chpl, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 1185 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  call_tmp_chpl7 = ret_tmp_chpl;
#line 1185 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ret_chpl = call_tmp_chpl7;
#line 1184 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  *(_retArg_chpl) = ret_chpl;
#line 1184 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  return;
#line 1184 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
}

#line 1197 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
/* OS.chpl:1197 */
#line 1197 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
static void createSystemError_chpl(syserr err_chpl8,
#line 1197 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                                   string_chpl * details_chpl,
#line 1197 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                                   _owned_SystemError_chpl * _retArg_chpl,
#line 1197 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                                   int64_t _ln_chpl,
#line 1197 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                                   int32_t _fn_chpl) {
#line 1197 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _owned_SystemError_chpl ret_chpl;
#line 1198 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  int32_t call_tmp_chpl6;
#line 1198 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_bool tmp_chpl;
#line 1198 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  int32_t call_tmp_chpl7;
#line 1198 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_bool tmp_chpl2;
#line 1198 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  int32_t call_tmp_chpl8;
#line 1198 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_bool tmp_chpl3;
#line 1198 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  int32_t call_tmp_chpl9;
#line 1199 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _owned_BlockingIoError_chpl call_tmp_chpl10;
#line 1199 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  BlockingIoError_chpl new_temp_chpl = NULL;
#line 1199 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _owned_BlockingIoError_chpl initTemp_chpl;
#line 1200 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  int32_t call_tmp_chpl11;
#line 1201 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _owned_ChildProcessError_chpl call_tmp_chpl12;
#line 1201 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ChildProcessError_chpl new_temp_chpl2 = NULL;
#line 1201 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _owned_ChildProcessError_chpl initTemp_chpl2;
#line 1202 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  int32_t call_tmp_chpl13;
#line 1203 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _owned_BrokenPipeError_chpl call_tmp_chpl14;
#line 1203 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  BrokenPipeError_chpl new_temp_chpl3 = NULL;
#line 1203 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _owned_BrokenPipeError_chpl initTemp_chpl3;
#line 1204 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  int32_t call_tmp_chpl15;
#line 1205 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _owned_ConnectionAbortedError_chpl call_tmp_chpl16;
#line 1205 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ConnectionAbortedError_chpl new_temp_chpl4 = NULL;
#line 1205 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _owned_ConnectionAbortedError_chpl initTemp_chpl4;
#line 1206 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  int32_t call_tmp_chpl17;
#line 1207 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _owned_ConnectionRefusedError_chpl call_tmp_chpl18;
#line 1207 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ConnectionRefusedError_chpl new_temp_chpl5 = NULL;
#line 1207 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _owned_ConnectionRefusedError_chpl initTemp_chpl5;
#line 1208 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  int32_t call_tmp_chpl19;
#line 1209 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _owned_ConnectionResetError_chpl call_tmp_chpl20;
#line 1209 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ConnectionResetError_chpl new_temp_chpl6 = NULL;
#line 1209 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _owned_ConnectionResetError_chpl initTemp_chpl6;
#line 1210 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  int32_t call_tmp_chpl21;
#line 1211 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _owned_FileExistsError_chpl call_tmp_chpl22;
#line 1211 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  FileExistsError_chpl new_temp_chpl7 = NULL;
#line 1211 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _owned_FileExistsError_chpl initTemp_chpl7;
#line 1212 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  int32_t call_tmp_chpl23;
#line 1213 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _owned_FileNotFoundError_chpl call_tmp_chpl24;
#line 1213 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  FileNotFoundError_chpl new_temp_chpl8 = NULL;
#line 1213 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _owned_FileNotFoundError_chpl initTemp_chpl8;
#line 1214 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  int32_t call_tmp_chpl25;
#line 1215 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _owned_InterruptedError_chpl call_tmp_chpl26;
#line 1215 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  InterruptedError_chpl new_temp_chpl9 = NULL;
#line 1215 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _owned_InterruptedError_chpl initTemp_chpl9;
#line 1216 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  int32_t call_tmp_chpl27;
#line 1217 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _owned_IsADirectoryError_chpl call_tmp_chpl28;
#line 1217 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  IsADirectoryError_chpl new_temp_chpl10 = NULL;
#line 1217 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _owned_IsADirectoryError_chpl initTemp_chpl10;
#line 1218 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  int32_t call_tmp_chpl29;
#line 1219 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _owned_NotADirectoryError_chpl call_tmp_chpl30;
#line 1219 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  NotADirectoryError_chpl new_temp_chpl11 = NULL;
#line 1219 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _owned_NotADirectoryError_chpl initTemp_chpl11;
#line 1220 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  int32_t call_tmp_chpl31;
#line 1220 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_bool tmp_chpl4;
#line 1220 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  int32_t call_tmp_chpl32;
#line 1221 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _owned_PermissionError_chpl call_tmp_chpl33;
#line 1221 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  PermissionError_chpl new_temp_chpl12 = NULL;
#line 1221 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _owned_PermissionError_chpl initTemp_chpl12;
#line 1222 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  int32_t call_tmp_chpl34;
#line 1223 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _owned_ProcessLookupError_chpl call_tmp_chpl35;
#line 1223 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ProcessLookupError_chpl new_temp_chpl13 = NULL;
#line 1223 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _owned_ProcessLookupError_chpl initTemp_chpl13;
#line 1224 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  int32_t call_tmp_chpl36;
#line 1225 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _owned_TimeoutError_chpl call_tmp_chpl37;
#line 1225 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  TimeoutError_chpl new_temp_chpl14 = NULL;
#line 1225 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _owned_TimeoutError_chpl initTemp_chpl14;
#line 1226 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  int32_t call_tmp_chpl38;
#line 1227 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _owned_IoError_chpl call_tmp_chpl39;
#line 1227 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  IoError_chpl new_temp_chpl15 = NULL;
#line 1227 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _owned_IoError_chpl initTemp_chpl15;
#line 1230 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _owned_SystemError_chpl call_tmp_chpl40;
#line 1230 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  SystemError_chpl new_temp_chpl16 = NULL;
#line 1230 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _owned_SystemError_chpl initTemp_chpl16;
#line 1198 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  call_tmp_chpl6 = qio_err_to_int(err_chpl8);
#line 1198 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  if (call_tmp_chpl6 == EAGAIN) /* ZLINE: 1198 /Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl */
#line 1198 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  {
#line 1198 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    tmp_chpl = UINT8(true);
#line 1198 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  } else /* ZLINE: 1198 /Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl */
#line 1198 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  {
#line 1198 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    call_tmp_chpl7 = qio_err_to_int(err_chpl8);
#line 1198 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    tmp_chpl = (call_tmp_chpl7 == EALREADY);
#line 1198 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  }
#line 1198 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  if (tmp_chpl) /* ZLINE: 1198 /Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl */
#line 1198 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  {
#line 1198 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    tmp_chpl2 = UINT8(true);
#line 1198 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  } else /* ZLINE: 1198 /Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl */
#line 1198 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  {
#line 1198 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    call_tmp_chpl8 = qio_err_to_int(err_chpl8);
#line 1198 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    tmp_chpl2 = (call_tmp_chpl8 == EWOULDBLOCK);
#line 1198 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  }
#line 1198 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  if (tmp_chpl2) /* ZLINE: 1198 /Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl */
#line 1198 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  {
#line 1198 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    tmp_chpl3 = UINT8(true);
#line 1198 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  } else /* ZLINE: 1198 /Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl */
#line 1198 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  {
#line 1198 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    call_tmp_chpl9 = qio_err_to_int(err_chpl8);
#line 1198 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    tmp_chpl3 = (call_tmp_chpl9 == EINPROGRESS);
#line 1198 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  }
#line 1198 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  if (tmp_chpl3) /* ZLINE: 1198 /Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl */
#line 1198 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  {
#line 1199 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    new_temp_chpl = _new_chpl68(details_chpl, err_chpl8, _ln_chpl, _fn_chpl);
#line 1199 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    init_chpl180(&initTemp_chpl, new_temp_chpl);
#line 1199 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    call_tmp_chpl10 = initTemp_chpl;
#line 1199 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    init_ASSIGN__chpl10(&ret_chpl, &call_tmp_chpl10, _ln_chpl, _fn_chpl);
#line 1199 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    chpl__autoDestroy50(&call_tmp_chpl10, _ln_chpl, _fn_chpl);
#line 1199 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    goto _end_createSystemError_chpl;
#line 1199 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  } else /* ZLINE: 1198 /Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl */
#line 1199 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  {
#line 1200 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    call_tmp_chpl11 = qio_err_to_int(err_chpl8);
#line 1200 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    if (call_tmp_chpl11 == ECHILD) /* ZLINE: 1200 /Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl */
#line 1200 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    {
#line 1201 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
      new_temp_chpl2 = _new_chpl69(details_chpl, err_chpl8, _ln_chpl, _fn_chpl);
#line 1201 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
      init_chpl184(&initTemp_chpl2, new_temp_chpl2);
#line 1201 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
      call_tmp_chpl12 = initTemp_chpl2;
#line 1201 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
      init_ASSIGN__chpl9(&ret_chpl, &call_tmp_chpl12, _ln_chpl, _fn_chpl);
#line 1201 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
      chpl__autoDestroy49(&call_tmp_chpl12, _ln_chpl, _fn_chpl);
#line 1201 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
      goto _end_createSystemError_chpl;
#line 1201 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    } else /* ZLINE: 1200 /Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl */
#line 1201 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    {
#line 1202 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
      call_tmp_chpl13 = qio_err_to_int(err_chpl8);
#line 1202 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
      if (call_tmp_chpl13 == EPIPE) /* ZLINE: 1202 /Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl */
#line 1202 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
      {
#line 1203 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
        new_temp_chpl3 = _new_chpl70(details_chpl, err_chpl8, _ln_chpl, _fn_chpl);
#line 1203 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
        init_chpl185(&initTemp_chpl3, new_temp_chpl3);
#line 1203 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
        call_tmp_chpl14 = initTemp_chpl3;
#line 1203 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
        init_ASSIGN__chpl6(&ret_chpl, &call_tmp_chpl14, _ln_chpl, _fn_chpl);
#line 1203 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
        chpl__autoDestroy48(&call_tmp_chpl14, _ln_chpl, _fn_chpl);
#line 1203 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
        goto _end_createSystemError_chpl;
#line 1203 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
      } else /* ZLINE: 1202 /Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl */
#line 1203 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
      {
#line 1204 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
        call_tmp_chpl15 = qio_err_to_int(err_chpl8);
#line 1204 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
        if (call_tmp_chpl15 == ECONNABORTED) /* ZLINE: 1204 /Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl */
#line 1204 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
        {
#line 1205 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
          new_temp_chpl4 = _new_chpl71(details_chpl, err_chpl8, _ln_chpl, _fn_chpl);
#line 1205 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
          init_chpl165(&initTemp_chpl4, new_temp_chpl4);
#line 1205 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
          call_tmp_chpl16 = initTemp_chpl4;
#line 1205 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
          init_ASSIGN__chpl4(&ret_chpl, &call_tmp_chpl16, _ln_chpl, _fn_chpl);
#line 1205 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
          chpl__autoDestroy47(&call_tmp_chpl16, _ln_chpl, _fn_chpl);
#line 1205 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
          goto _end_createSystemError_chpl;
#line 1205 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
        } else /* ZLINE: 1204 /Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl */
#line 1205 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
        {
#line 1206 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
          call_tmp_chpl17 = qio_err_to_int(err_chpl8);
#line 1206 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
          if (call_tmp_chpl17 == ECONNREFUSED) /* ZLINE: 1206 /Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl */
#line 1206 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
          {
#line 1207 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
            new_temp_chpl5 = _new_chpl72(details_chpl, err_chpl8, _ln_chpl, _fn_chpl);
#line 1207 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
            init_chpl175(&initTemp_chpl5, new_temp_chpl5);
#line 1207 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
            call_tmp_chpl18 = initTemp_chpl5;
#line 1207 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
            init_ASSIGN__chpl3(&ret_chpl, &call_tmp_chpl18, _ln_chpl, _fn_chpl);
#line 1207 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
            chpl__autoDestroy46(&call_tmp_chpl18, _ln_chpl, _fn_chpl);
#line 1207 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
            goto _end_createSystemError_chpl;
#line 1207 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
          } else /* ZLINE: 1206 /Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl */
#line 1207 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
          {
#line 1208 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
            call_tmp_chpl19 = qio_err_to_int(err_chpl8);
#line 1208 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
            if (call_tmp_chpl19 == ECONNRESET) /* ZLINE: 1208 /Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl */
#line 1208 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
            {
#line 1209 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
              new_temp_chpl6 = _new_chpl73(details_chpl, err_chpl8, _ln_chpl, _fn_chpl);
#line 1209 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
              init_chpl172(&initTemp_chpl6, new_temp_chpl6);
#line 1209 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
              call_tmp_chpl20 = initTemp_chpl6;
#line 1209 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
              init_ASSIGN__chpl2(&ret_chpl, &call_tmp_chpl20, _ln_chpl, _fn_chpl);
#line 1209 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
              chpl__autoDestroy45(&call_tmp_chpl20, _ln_chpl, _fn_chpl);
#line 1209 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
              goto _end_createSystemError_chpl;
#line 1209 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
            } else /* ZLINE: 1208 /Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl */
#line 1209 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
            {
#line 1210 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
              call_tmp_chpl21 = qio_err_to_int(err_chpl8);
#line 1210 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
              if (call_tmp_chpl21 == EEXIST) /* ZLINE: 1210 /Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl */
#line 1210 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
              {
#line 1211 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                new_temp_chpl7 = _new_chpl74(details_chpl, err_chpl8, _ln_chpl, _fn_chpl);
#line 1211 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                init_chpl170(&initTemp_chpl7, new_temp_chpl7);
#line 1211 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                call_tmp_chpl22 = initTemp_chpl7;
#line 1211 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                init_ASSIGN__chpl5(&ret_chpl, &call_tmp_chpl22, _ln_chpl, _fn_chpl);
#line 1211 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                chpl__autoDestroy44(&call_tmp_chpl22, _ln_chpl, _fn_chpl);
#line 1211 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                goto _end_createSystemError_chpl;
#line 1211 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
              } else /* ZLINE: 1210 /Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl */
#line 1211 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
              {
#line 1212 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                call_tmp_chpl23 = qio_err_to_int(err_chpl8);
#line 1212 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                if (call_tmp_chpl23 == ENOENT) /* ZLINE: 1212 /Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl */
#line 1212 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                {
#line 1213 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                  new_temp_chpl8 = _new_chpl75(details_chpl, err_chpl8, _ln_chpl, _fn_chpl);
#line 1213 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                  init_chpl169(&initTemp_chpl8, new_temp_chpl8);
#line 1213 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                  call_tmp_chpl24 = initTemp_chpl8;
#line 1213 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                  init_ASSIGN__chpl7(&ret_chpl, &call_tmp_chpl24, _ln_chpl, _fn_chpl);
#line 1213 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                  chpl__autoDestroy43(&call_tmp_chpl24, _ln_chpl, _fn_chpl);
#line 1213 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                  goto _end_createSystemError_chpl;
#line 1213 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                } else /* ZLINE: 1212 /Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl */
#line 1213 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                {
#line 1214 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                  call_tmp_chpl25 = qio_err_to_int(err_chpl8);
#line 1214 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                  if (call_tmp_chpl25 == EINTR) /* ZLINE: 1214 /Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl */
#line 1214 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                  {
#line 1215 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                    new_temp_chpl9 = _new_chpl76(details_chpl, err_chpl8, _ln_chpl, _fn_chpl);
#line 1215 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                    init_chpl168(&initTemp_chpl9, new_temp_chpl9);
#line 1215 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                    call_tmp_chpl26 = initTemp_chpl9;
#line 1215 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                    init_ASSIGN__chpl8(&ret_chpl, &call_tmp_chpl26, _ln_chpl, _fn_chpl);
#line 1215 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                    chpl__autoDestroy42(&call_tmp_chpl26, _ln_chpl, _fn_chpl);
#line 1215 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                    goto _end_createSystemError_chpl;
#line 1215 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                  } else /* ZLINE: 1214 /Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl */
#line 1215 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                  {
#line 1216 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                    call_tmp_chpl27 = qio_err_to_int(err_chpl8);
#line 1216 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                    if (call_tmp_chpl27 == EISDIR) /* ZLINE: 1216 /Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl */
#line 1216 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                    {
#line 1217 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                      new_temp_chpl10 = _new_chpl77(details_chpl, err_chpl8, _ln_chpl, _fn_chpl);
#line 1217 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                      init_chpl167(&initTemp_chpl10, new_temp_chpl10);
#line 1217 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                      call_tmp_chpl28 = initTemp_chpl10;
#line 1217 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                      init_ASSIGN__chpl11(&ret_chpl, &call_tmp_chpl28, _ln_chpl, _fn_chpl);
#line 1217 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                      chpl__autoDestroy41(&call_tmp_chpl28, _ln_chpl, _fn_chpl);
#line 1217 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                      goto _end_createSystemError_chpl;
#line 1217 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                    } else /* ZLINE: 1216 /Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl */
#line 1217 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                    {
#line 1218 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                      call_tmp_chpl29 = qio_err_to_int(err_chpl8);
#line 1218 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                      if (call_tmp_chpl29 == ENOTDIR) /* ZLINE: 1218 /Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl */
#line 1218 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                      {
#line 1219 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                        new_temp_chpl11 = _new_chpl78(details_chpl, err_chpl8, _ln_chpl, _fn_chpl);
#line 1219 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                        init_chpl164(&initTemp_chpl11, new_temp_chpl11);
#line 1219 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                        call_tmp_chpl30 = initTemp_chpl11;
#line 1219 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                        init_ASSIGN__chpl12(&ret_chpl, &call_tmp_chpl30, _ln_chpl, _fn_chpl);
#line 1219 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                        chpl__autoDestroy40(&call_tmp_chpl30, _ln_chpl, _fn_chpl);
#line 1219 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                        goto _end_createSystemError_chpl;
#line 1219 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                      } else /* ZLINE: 1218 /Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl */
#line 1219 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                      {
#line 1220 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                        call_tmp_chpl31 = qio_err_to_int(err_chpl8);
#line 1220 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                        if (call_tmp_chpl31 == EACCES) /* ZLINE: 1220 /Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl */
#line 1220 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                        {
#line 1220 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                          tmp_chpl4 = UINT8(true);
#line 1220 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                        } else /* ZLINE: 1220 /Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl */
#line 1220 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                        {
#line 1220 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                          call_tmp_chpl32 = qio_err_to_int(err_chpl8);
#line 1220 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                          tmp_chpl4 = (call_tmp_chpl32 == EPERM);
#line 1220 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                        }
#line 1220 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                        if (tmp_chpl4) /* ZLINE: 1220 /Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl */
#line 1220 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                        {
#line 1221 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                          new_temp_chpl12 = _new_chpl79(details_chpl, err_chpl8, _ln_chpl, _fn_chpl);
#line 1221 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                          init_chpl189(&initTemp_chpl12, new_temp_chpl12);
#line 1221 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                          call_tmp_chpl33 = initTemp_chpl12;
#line 1221 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                          init_ASSIGN__chpl13(&ret_chpl, &call_tmp_chpl33, _ln_chpl, _fn_chpl);
#line 1221 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                          chpl__autoDestroy39(&call_tmp_chpl33, _ln_chpl, _fn_chpl);
#line 1221 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                          goto _end_createSystemError_chpl;
#line 1221 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                        } else /* ZLINE: 1220 /Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl */
#line 1221 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                        {
#line 1222 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                          call_tmp_chpl34 = qio_err_to_int(err_chpl8);
#line 1222 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                          if (call_tmp_chpl34 == ESRCH) /* ZLINE: 1222 /Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl */
#line 1222 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                          {
#line 1223 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                            new_temp_chpl13 = _new_chpl80(details_chpl, err_chpl8, _ln_chpl, _fn_chpl);
#line 1223 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                            init_chpl187(&initTemp_chpl13, new_temp_chpl13);
#line 1223 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                            call_tmp_chpl35 = initTemp_chpl13;
#line 1223 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                            init_ASSIGN__chpl14(&ret_chpl, &call_tmp_chpl35, _ln_chpl, _fn_chpl);
#line 1223 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                            chpl__autoDestroy37(&call_tmp_chpl35, _ln_chpl, _fn_chpl);
#line 1223 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                            goto _end_createSystemError_chpl;
#line 1223 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                          } else /* ZLINE: 1222 /Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl */
#line 1223 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                          {
#line 1224 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                            call_tmp_chpl36 = qio_err_to_int(err_chpl8);
#line 1224 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                            if (call_tmp_chpl36 == ETIMEDOUT) /* ZLINE: 1224 /Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl */
#line 1224 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                            {
#line 1225 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                              new_temp_chpl14 = _new_chpl81(details_chpl, err_chpl8, _ln_chpl, _fn_chpl);
#line 1225 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                              init_chpl181(&initTemp_chpl14, new_temp_chpl14);
#line 1225 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                              call_tmp_chpl37 = initTemp_chpl14;
#line 1225 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                              init_ASSIGN__chpl15(&ret_chpl, &call_tmp_chpl37, _ln_chpl, _fn_chpl);
#line 1225 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                              chpl__autoDestroy35(&call_tmp_chpl37, _ln_chpl, _fn_chpl);
#line 1225 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                              goto _end_createSystemError_chpl;
#line 1225 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                            } else /* ZLINE: 1224 /Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl */
#line 1225 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                            {
#line 1226 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                              call_tmp_chpl38 = qio_err_to_int(err_chpl8);
#line 1226 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                              if (call_tmp_chpl38 == EIO) /* ZLINE: 1226 /Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl */
#line 1226 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                              {
#line 1227 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                                new_temp_chpl15 = _new_chpl82(err_chpl8, details_chpl, _ln_chpl, _fn_chpl);
#line 1227 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                                init_chpl176(&initTemp_chpl15, new_temp_chpl15);
#line 1227 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                                call_tmp_chpl39 = initTemp_chpl15;
#line 1227 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                                init_ASSIGN__chpl16(&ret_chpl, &call_tmp_chpl39, _ln_chpl, _fn_chpl);
#line 1227 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                                chpl__autoDestroy38(&call_tmp_chpl39, _ln_chpl, _fn_chpl);
#line 1227 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                                goto _end_createSystemError_chpl;
#line 1227 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                              }
#line 1227 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                            }
#line 1227 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                          }
#line 1227 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                        }
#line 1227 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                      }
#line 1227 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                    }
#line 1227 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                  }
#line 1227 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                }
#line 1227 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
              }
#line 1227 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
            }
#line 1227 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
          }
#line 1227 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
        }
#line 1227 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
      }
#line 1227 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    }
#line 1227 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  }
#line 1230 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  new_temp_chpl16 = _new_chpl67(err_chpl8, details_chpl, _ln_chpl, _fn_chpl);
#line 1230 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  init_chpl186(&initTemp_chpl16, new_temp_chpl16);
#line 1230 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  call_tmp_chpl40 = initTemp_chpl16;
#line 1230 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ret_chpl = call_tmp_chpl40;
#line 1230 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _end_createSystemError_chpl:;
#line 1197 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  *(_retArg_chpl) = ret_chpl;
#line 1197 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  return;
#line 1197 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
}

#line 1242 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
/* OS.chpl:1242 */
#line 1242 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
static void createSystemError_chpl2(int64_t err_chpl8,
#line 1242 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                                    string_chpl * details_chpl,
#line 1242 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                                    _owned_SystemError_chpl * _retArg_chpl,
#line 1242 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                                    int64_t _ln_chpl,
#line 1242 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                                    int32_t _fn_chpl) {
#line 1242 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _owned_SystemError_chpl ret_chpl;
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  syserr call_tmp_chpl6;
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _owned_SystemError_chpl call_tmp_chpl7;
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _owned_SystemError_chpl ret_tmp_chpl;
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  call_tmp_chpl6 = qio_int_to_err(((int32_t)(err_chpl8)));
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  createSystemError_chpl(call_tmp_chpl6, details_chpl, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  call_tmp_chpl7 = ret_tmp_chpl;
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ret_chpl = call_tmp_chpl7;
#line 1242 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  *(_retArg_chpl) = ret_chpl;
#line 1242 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  return;
#line 1242 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
}

#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
/* OS.chpl:1252 */
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
static void chpl__auto_destroy_BlockingIoError(BlockingIoError_chpl this_chpl31,
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                                               int64_t _ln_chpl,
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                                               int32_t _fn_chpl) {
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  SystemError_chpl _parent_destructor_tmp__chpl = NULL;
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _ref_string_chpl _field_destructor_tmp__chpl = NULL;
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  Error_chpl _parent_destructor_tmp__chpl2 = NULL;
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _ref_string_chpl _field_destructor_tmp__chpl2 = NULL;
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _parent_destructor_tmp__chpl = ((SystemError_chpl)(this_chpl31));
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(_parent_destructor_tmp__chpl, _ln_chpl, _fn_chpl);
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _field_destructor_tmp__chpl = &((_parent_destructor_tmp__chpl)->details);
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  deinit_chpl95(_field_destructor_tmp__chpl, _ln_chpl, _fn_chpl);
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _parent_destructor_tmp__chpl2 = ((Error_chpl)(_parent_destructor_tmp__chpl));
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(_parent_destructor_tmp__chpl2, _ln_chpl, _fn_chpl);
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _field_destructor_tmp__chpl2 = &((_parent_destructor_tmp__chpl2)->_msg);
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  deinit_chpl95(_field_destructor_tmp__chpl2, _ln_chpl, _fn_chpl);
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  return;
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
}

#line 1253 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
/* OS.chpl:1253 */
#line 1253 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
static BlockingIoError_chpl _new_chpl68(string_chpl * details_chpl,
#line 1253 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                                        syserr err_chpl8,
#line 1253 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                                        int64_t _ln_chpl,
#line 1253 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                                        int32_t _fn_chpl) {
#line 1253 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  BlockingIoError_chpl initTemp_chpl = NULL;
#line 1253 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  c_ptr_void_chpl cast_tmp_chpl = NULL;
#line 1253 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  SystemError_chpl super_tmp_chpl = NULL;
#line 1253 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  Error_chpl super_tmp_chpl2 = NULL;
#line 1253 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  Error_chpl init_coerce_tmp_chpl = NULL;
#line 1253 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  string_chpl _msg_chpl;
#line 1253 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  string_chpl details_chpl2;
#line 1253 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  cast_tmp_chpl = chpl_here_alloc(sizeof(chpl_BlockingIoError_chpl_object), INT16(11), _ln_chpl, _fn_chpl);
#line 1253 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  initTemp_chpl = ((BlockingIoError_chpl)(cast_tmp_chpl));
#line 1253 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(initTemp_chpl))->chpl__cid = chpl__cid_BlockingIoError_chpl;
#line 1253 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(initTemp_chpl, _ln_chpl, _fn_chpl);
#line 1253 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  super_tmp_chpl = &((initTemp_chpl)->super);
#line 1253 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 1253 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  super_tmp_chpl2 = &((super_tmp_chpl)->super);
#line 1253 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 1253 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(&((super_tmp_chpl2)->super)))->chpl__cid = chpl__cid_RootClass_chpl;
#line 1253 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(((RootClass_chpl)(super_tmp_chpl2))))->chpl__cid = chpl__cid_RootClass_chpl;
#line 1253 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  init_coerce_tmp_chpl = ((Error_chpl)(nil));
#line 1253 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 1253 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  (super_tmp_chpl2)->_next = init_coerce_tmp_chpl;
#line 1253 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 1253 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  (super_tmp_chpl2)->thrownLine = INT64(0);
#line 1253 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 1253 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  (super_tmp_chpl2)->thrownFileId = INT32(0);
#line 1253 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  init_chpl194(&_msg_chpl, _ln_chpl, _fn_chpl);
#line 1253 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 1253 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  (super_tmp_chpl2)->_msg = _msg_chpl;
#line 1253 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 1253 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  (super_tmp_chpl2)->_hasThrowInfo = UINT8(false);
#line 1253 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(super_tmp_chpl2))->chpl__cid = chpl__cid_Error_chpl;
#line 1253 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(((Error_chpl)(super_tmp_chpl))))->chpl__cid = chpl__cid_Error_chpl;
#line 1253 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 1253 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  (super_tmp_chpl)->err = err_chpl8;
#line 1253 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  init_ASSIGN__chpl25(&details_chpl2, details_chpl, _ln_chpl, _fn_chpl);
#line 1253 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 1253 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  (super_tmp_chpl)->details = details_chpl2;
#line 1253 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(super_tmp_chpl))->chpl__cid = chpl__cid_SystemError_chpl;
#line 1253 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(((SystemError_chpl)(initTemp_chpl))))->chpl__cid = chpl__cid_SystemError_chpl;
#line 1253 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(initTemp_chpl))->chpl__cid = chpl__cid_BlockingIoError_chpl;
#line 1253 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  return initTemp_chpl;
#line 1253 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
}

#line 1262 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
/* OS.chpl:1262 */
#line 1262 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
static void chpl__auto_destroy_ChildProcessError(ChildProcessError_chpl this_chpl31,
#line 1262 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                                                 int64_t _ln_chpl,
#line 1262 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                                                 int32_t _fn_chpl) {
#line 1262 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  SystemError_chpl _parent_destructor_tmp__chpl = NULL;
#line 1262 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _ref_string_chpl _field_destructor_tmp__chpl = NULL;
#line 1262 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  Error_chpl _parent_destructor_tmp__chpl2 = NULL;
#line 1262 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _ref_string_chpl _field_destructor_tmp__chpl2 = NULL;
#line 1262 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _parent_destructor_tmp__chpl = ((SystemError_chpl)(this_chpl31));
#line 1262 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(_parent_destructor_tmp__chpl, _ln_chpl, _fn_chpl);
#line 1262 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _field_destructor_tmp__chpl = &((_parent_destructor_tmp__chpl)->details);
#line 1262 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  deinit_chpl95(_field_destructor_tmp__chpl, _ln_chpl, _fn_chpl);
#line 1262 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _parent_destructor_tmp__chpl2 = ((Error_chpl)(_parent_destructor_tmp__chpl));
#line 1262 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(_parent_destructor_tmp__chpl2, _ln_chpl, _fn_chpl);
#line 1262 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _field_destructor_tmp__chpl2 = &((_parent_destructor_tmp__chpl2)->_msg);
#line 1262 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  deinit_chpl95(_field_destructor_tmp__chpl2, _ln_chpl, _fn_chpl);
#line 1262 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  return;
#line 1262 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
}

#line 1263 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
/* OS.chpl:1263 */
#line 1263 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
static ChildProcessError_chpl _new_chpl69(string_chpl * details_chpl,
#line 1263 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                                          syserr err_chpl8,
#line 1263 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                                          int64_t _ln_chpl,
#line 1263 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                                          int32_t _fn_chpl) {
#line 1263 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ChildProcessError_chpl initTemp_chpl = NULL;
#line 1263 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  c_ptr_void_chpl cast_tmp_chpl = NULL;
#line 1263 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  SystemError_chpl super_tmp_chpl = NULL;
#line 1263 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  Error_chpl super_tmp_chpl2 = NULL;
#line 1263 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  Error_chpl init_coerce_tmp_chpl = NULL;
#line 1263 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  string_chpl _msg_chpl;
#line 1263 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  string_chpl details_chpl2;
#line 1263 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  cast_tmp_chpl = chpl_here_alloc(sizeof(chpl_ChildProcessError_chpl_object), INT16(12), _ln_chpl, _fn_chpl);
#line 1263 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  initTemp_chpl = ((ChildProcessError_chpl)(cast_tmp_chpl));
#line 1263 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(initTemp_chpl))->chpl__cid = chpl__cid_ChildProcessError_chpl;
#line 1263 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(initTemp_chpl, _ln_chpl, _fn_chpl);
#line 1263 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  super_tmp_chpl = &((initTemp_chpl)->super);
#line 1263 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 1263 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  super_tmp_chpl2 = &((super_tmp_chpl)->super);
#line 1263 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 1263 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(&((super_tmp_chpl2)->super)))->chpl__cid = chpl__cid_RootClass_chpl;
#line 1263 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(((RootClass_chpl)(super_tmp_chpl2))))->chpl__cid = chpl__cid_RootClass_chpl;
#line 1263 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  init_coerce_tmp_chpl = ((Error_chpl)(nil));
#line 1263 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 1263 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  (super_tmp_chpl2)->_next = init_coerce_tmp_chpl;
#line 1263 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 1263 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  (super_tmp_chpl2)->thrownLine = INT64(0);
#line 1263 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 1263 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  (super_tmp_chpl2)->thrownFileId = INT32(0);
#line 1263 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  init_chpl194(&_msg_chpl, _ln_chpl, _fn_chpl);
#line 1263 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 1263 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  (super_tmp_chpl2)->_msg = _msg_chpl;
#line 1263 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 1263 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  (super_tmp_chpl2)->_hasThrowInfo = UINT8(false);
#line 1263 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(super_tmp_chpl2))->chpl__cid = chpl__cid_Error_chpl;
#line 1263 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(((Error_chpl)(super_tmp_chpl))))->chpl__cid = chpl__cid_Error_chpl;
#line 1263 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 1263 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  (super_tmp_chpl)->err = err_chpl8;
#line 1263 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  init_ASSIGN__chpl25(&details_chpl2, details_chpl, _ln_chpl, _fn_chpl);
#line 1263 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 1263 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  (super_tmp_chpl)->details = details_chpl2;
#line 1263 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(super_tmp_chpl))->chpl__cid = chpl__cid_SystemError_chpl;
#line 1263 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(((SystemError_chpl)(initTemp_chpl))))->chpl__cid = chpl__cid_SystemError_chpl;
#line 1263 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(initTemp_chpl))->chpl__cid = chpl__cid_ChildProcessError_chpl;
#line 1263 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  return initTemp_chpl;
#line 1263 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
}

#line 1272 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
/* OS.chpl:1272 */
#line 1272 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
static void chpl__auto_destroy_ConnectionError(ConnectionError_chpl this_chpl31,
#line 1272 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                                               int64_t _ln_chpl,
#line 1272 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                                               int32_t _fn_chpl) {
#line 1272 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  SystemError_chpl _parent_destructor_tmp__chpl = NULL;
#line 1272 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _ref_string_chpl _field_destructor_tmp__chpl = NULL;
#line 1272 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  Error_chpl _parent_destructor_tmp__chpl2 = NULL;
#line 1272 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _ref_string_chpl _field_destructor_tmp__chpl2 = NULL;
#line 1272 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _parent_destructor_tmp__chpl = ((SystemError_chpl)(this_chpl31));
#line 1272 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(_parent_destructor_tmp__chpl, _ln_chpl, _fn_chpl);
#line 1272 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _field_destructor_tmp__chpl = &((_parent_destructor_tmp__chpl)->details);
#line 1272 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  deinit_chpl95(_field_destructor_tmp__chpl, _ln_chpl, _fn_chpl);
#line 1272 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _parent_destructor_tmp__chpl2 = ((Error_chpl)(_parent_destructor_tmp__chpl));
#line 1272 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(_parent_destructor_tmp__chpl2, _ln_chpl, _fn_chpl);
#line 1272 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _field_destructor_tmp__chpl2 = &((_parent_destructor_tmp__chpl2)->_msg);
#line 1272 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  deinit_chpl95(_field_destructor_tmp__chpl2, _ln_chpl, _fn_chpl);
#line 1272 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  return;
#line 1272 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
}

#line 1282 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
/* OS.chpl:1282 */
#line 1282 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
static void chpl__auto_destroy_BrokenPipeError(BrokenPipeError_chpl this_chpl31,
#line 1282 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                                               int64_t _ln_chpl,
#line 1282 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                                               int32_t _fn_chpl) {
#line 1282 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  SystemError_chpl _parent_destructor_tmp__chpl = NULL;
#line 1282 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _ref_string_chpl _field_destructor_tmp__chpl = NULL;
#line 1282 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  Error_chpl _parent_destructor_tmp__chpl2 = NULL;
#line 1282 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _ref_string_chpl _field_destructor_tmp__chpl2 = NULL;
#line 1282 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _parent_destructor_tmp__chpl = ((SystemError_chpl)(((ConnectionError_chpl)(this_chpl31))));
#line 1282 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(_parent_destructor_tmp__chpl, _ln_chpl, _fn_chpl);
#line 1282 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _field_destructor_tmp__chpl = &((_parent_destructor_tmp__chpl)->details);
#line 1282 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  deinit_chpl95(_field_destructor_tmp__chpl, _ln_chpl, _fn_chpl);
#line 1282 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _parent_destructor_tmp__chpl2 = ((Error_chpl)(_parent_destructor_tmp__chpl));
#line 1282 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(_parent_destructor_tmp__chpl2, _ln_chpl, _fn_chpl);
#line 1282 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _field_destructor_tmp__chpl2 = &((_parent_destructor_tmp__chpl2)->_msg);
#line 1282 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  deinit_chpl95(_field_destructor_tmp__chpl2, _ln_chpl, _fn_chpl);
#line 1282 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  return;
#line 1282 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
}

#line 1283 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
/* OS.chpl:1283 */
#line 1283 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
static BrokenPipeError_chpl _new_chpl70(string_chpl * details_chpl,
#line 1283 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                                        syserr err_chpl8,
#line 1283 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                                        int64_t _ln_chpl,
#line 1283 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                                        int32_t _fn_chpl) {
#line 1283 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  BrokenPipeError_chpl initTemp_chpl = NULL;
#line 1283 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  c_ptr_void_chpl cast_tmp_chpl = NULL;
#line 1283 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ConnectionError_chpl super_tmp_chpl = NULL;
#line 1283 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  SystemError_chpl super_tmp_chpl2 = NULL;
#line 1283 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  Error_chpl super_tmp_chpl3 = NULL;
#line 1283 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  Error_chpl init_coerce_tmp_chpl = NULL;
#line 1283 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  string_chpl _msg_chpl;
#line 1283 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  string_chpl details_chpl2;
#line 1283 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  cast_tmp_chpl = chpl_here_alloc(sizeof(chpl_BrokenPipeError_chpl_object), INT16(13), _ln_chpl, _fn_chpl);
#line 1283 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  initTemp_chpl = ((BrokenPipeError_chpl)(cast_tmp_chpl));
#line 1283 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(initTemp_chpl))->chpl__cid = chpl__cid_BrokenPipeError_chpl;
#line 1283 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(initTemp_chpl, _ln_chpl, _fn_chpl);
#line 1283 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  super_tmp_chpl = &((initTemp_chpl)->super);
#line 1283 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 1283 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  super_tmp_chpl2 = &((super_tmp_chpl)->super);
#line 1283 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 1283 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  super_tmp_chpl3 = &((super_tmp_chpl2)->super);
#line 1283 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 1283 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(&((super_tmp_chpl3)->super)))->chpl__cid = chpl__cid_RootClass_chpl;
#line 1283 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(((RootClass_chpl)(super_tmp_chpl3))))->chpl__cid = chpl__cid_RootClass_chpl;
#line 1283 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  init_coerce_tmp_chpl = ((Error_chpl)(nil));
#line 1283 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 1283 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  (super_tmp_chpl3)->_next = init_coerce_tmp_chpl;
#line 1283 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 1283 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  (super_tmp_chpl3)->thrownLine = INT64(0);
#line 1283 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 1283 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  (super_tmp_chpl3)->thrownFileId = INT32(0);
#line 1283 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  init_chpl194(&_msg_chpl, _ln_chpl, _fn_chpl);
#line 1283 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 1283 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  (super_tmp_chpl3)->_msg = _msg_chpl;
#line 1283 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 1283 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  (super_tmp_chpl3)->_hasThrowInfo = UINT8(false);
#line 1283 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(super_tmp_chpl3))->chpl__cid = chpl__cid_Error_chpl;
#line 1283 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(((Error_chpl)(super_tmp_chpl2))))->chpl__cid = chpl__cid_Error_chpl;
#line 1283 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 1283 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  (super_tmp_chpl2)->err = err_chpl8;
#line 1283 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  init_ASSIGN__chpl25(&details_chpl2, details_chpl, _ln_chpl, _fn_chpl);
#line 1283 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 1283 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  (super_tmp_chpl2)->details = details_chpl2;
#line 1283 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(super_tmp_chpl2))->chpl__cid = chpl__cid_SystemError_chpl;
#line 1283 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(((SystemError_chpl)(super_tmp_chpl))))->chpl__cid = chpl__cid_SystemError_chpl;
#line 1283 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(super_tmp_chpl))->chpl__cid = chpl__cid_ConnectionError_chpl;
#line 1283 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(((ConnectionError_chpl)(initTemp_chpl))))->chpl__cid = chpl__cid_ConnectionError_chpl;
#line 1283 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(initTemp_chpl))->chpl__cid = chpl__cid_BrokenPipeError_chpl;
#line 1283 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  return initTemp_chpl;
#line 1283 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
}

#line 1292 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
/* OS.chpl:1292 */
#line 1292 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
static void chpl__auto_destroy_ConnectionAbortedError(ConnectionAbortedError_chpl this_chpl31,
#line 1292 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                                                      int64_t _ln_chpl,
#line 1292 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                                                      int32_t _fn_chpl) {
#line 1292 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  SystemError_chpl _parent_destructor_tmp__chpl = NULL;
#line 1292 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _ref_string_chpl _field_destructor_tmp__chpl = NULL;
#line 1292 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  Error_chpl _parent_destructor_tmp__chpl2 = NULL;
#line 1292 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _ref_string_chpl _field_destructor_tmp__chpl2 = NULL;
#line 1292 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _parent_destructor_tmp__chpl = ((SystemError_chpl)(((ConnectionError_chpl)(this_chpl31))));
#line 1292 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(_parent_destructor_tmp__chpl, _ln_chpl, _fn_chpl);
#line 1292 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _field_destructor_tmp__chpl = &((_parent_destructor_tmp__chpl)->details);
#line 1292 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  deinit_chpl95(_field_destructor_tmp__chpl, _ln_chpl, _fn_chpl);
#line 1292 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _parent_destructor_tmp__chpl2 = ((Error_chpl)(_parent_destructor_tmp__chpl));
#line 1292 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(_parent_destructor_tmp__chpl2, _ln_chpl, _fn_chpl);
#line 1292 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _field_destructor_tmp__chpl2 = &((_parent_destructor_tmp__chpl2)->_msg);
#line 1292 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  deinit_chpl95(_field_destructor_tmp__chpl2, _ln_chpl, _fn_chpl);
#line 1292 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  return;
#line 1292 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
}

#line 1293 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
/* OS.chpl:1293 */
#line 1293 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
static ConnectionAbortedError_chpl _new_chpl71(string_chpl * details_chpl,
#line 1293 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                                               syserr err_chpl8,
#line 1293 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                                               int64_t _ln_chpl,
#line 1293 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                                               int32_t _fn_chpl) {
#line 1293 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ConnectionAbortedError_chpl initTemp_chpl = NULL;
#line 1293 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  c_ptr_void_chpl cast_tmp_chpl = NULL;
#line 1293 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ConnectionError_chpl super_tmp_chpl = NULL;
#line 1293 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  SystemError_chpl super_tmp_chpl2 = NULL;
#line 1293 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  Error_chpl super_tmp_chpl3 = NULL;
#line 1293 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  Error_chpl init_coerce_tmp_chpl = NULL;
#line 1293 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  string_chpl _msg_chpl;
#line 1293 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  string_chpl details_chpl2;
#line 1293 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  cast_tmp_chpl = chpl_here_alloc(sizeof(chpl_ConnectionAbortedError_chpl_object), INT16(14), _ln_chpl, _fn_chpl);
#line 1293 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  initTemp_chpl = ((ConnectionAbortedError_chpl)(cast_tmp_chpl));
#line 1293 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(initTemp_chpl))->chpl__cid = chpl__cid_ConnectionAbortedError_chpl;
#line 1293 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(initTemp_chpl, _ln_chpl, _fn_chpl);
#line 1293 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  super_tmp_chpl = &((initTemp_chpl)->super);
#line 1293 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 1293 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  super_tmp_chpl2 = &((super_tmp_chpl)->super);
#line 1293 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 1293 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  super_tmp_chpl3 = &((super_tmp_chpl2)->super);
#line 1293 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 1293 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(&((super_tmp_chpl3)->super)))->chpl__cid = chpl__cid_RootClass_chpl;
#line 1293 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(((RootClass_chpl)(super_tmp_chpl3))))->chpl__cid = chpl__cid_RootClass_chpl;
#line 1293 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  init_coerce_tmp_chpl = ((Error_chpl)(nil));
#line 1293 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 1293 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  (super_tmp_chpl3)->_next = init_coerce_tmp_chpl;
#line 1293 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 1293 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  (super_tmp_chpl3)->thrownLine = INT64(0);
#line 1293 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 1293 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  (super_tmp_chpl3)->thrownFileId = INT32(0);
#line 1293 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  init_chpl194(&_msg_chpl, _ln_chpl, _fn_chpl);
#line 1293 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 1293 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  (super_tmp_chpl3)->_msg = _msg_chpl;
#line 1293 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 1293 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  (super_tmp_chpl3)->_hasThrowInfo = UINT8(false);
#line 1293 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(super_tmp_chpl3))->chpl__cid = chpl__cid_Error_chpl;
#line 1293 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(((Error_chpl)(super_tmp_chpl2))))->chpl__cid = chpl__cid_Error_chpl;
#line 1293 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 1293 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  (super_tmp_chpl2)->err = err_chpl8;
#line 1293 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  init_ASSIGN__chpl25(&details_chpl2, details_chpl, _ln_chpl, _fn_chpl);
#line 1293 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 1293 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  (super_tmp_chpl2)->details = details_chpl2;
#line 1293 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(super_tmp_chpl2))->chpl__cid = chpl__cid_SystemError_chpl;
#line 1293 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(((SystemError_chpl)(super_tmp_chpl))))->chpl__cid = chpl__cid_SystemError_chpl;
#line 1293 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(super_tmp_chpl))->chpl__cid = chpl__cid_ConnectionError_chpl;
#line 1293 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(((ConnectionError_chpl)(initTemp_chpl))))->chpl__cid = chpl__cid_ConnectionError_chpl;
#line 1293 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(initTemp_chpl))->chpl__cid = chpl__cid_ConnectionAbortedError_chpl;
#line 1293 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  return initTemp_chpl;
#line 1293 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
}

#line 1302 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
/* OS.chpl:1302 */
#line 1302 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
static void chpl__auto_destroy_ConnectionRefusedError(ConnectionRefusedError_chpl this_chpl31,
#line 1302 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                                                      int64_t _ln_chpl,
#line 1302 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                                                      int32_t _fn_chpl) {
#line 1302 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  SystemError_chpl _parent_destructor_tmp__chpl = NULL;
#line 1302 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _ref_string_chpl _field_destructor_tmp__chpl = NULL;
#line 1302 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  Error_chpl _parent_destructor_tmp__chpl2 = NULL;
#line 1302 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _ref_string_chpl _field_destructor_tmp__chpl2 = NULL;
#line 1302 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _parent_destructor_tmp__chpl = ((SystemError_chpl)(((ConnectionError_chpl)(this_chpl31))));
#line 1302 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(_parent_destructor_tmp__chpl, _ln_chpl, _fn_chpl);
#line 1302 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _field_destructor_tmp__chpl = &((_parent_destructor_tmp__chpl)->details);
#line 1302 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  deinit_chpl95(_field_destructor_tmp__chpl, _ln_chpl, _fn_chpl);
#line 1302 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _parent_destructor_tmp__chpl2 = ((Error_chpl)(_parent_destructor_tmp__chpl));
#line 1302 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(_parent_destructor_tmp__chpl2, _ln_chpl, _fn_chpl);
#line 1302 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _field_destructor_tmp__chpl2 = &((_parent_destructor_tmp__chpl2)->_msg);
#line 1302 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  deinit_chpl95(_field_destructor_tmp__chpl2, _ln_chpl, _fn_chpl);
#line 1302 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  return;
#line 1302 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
}

#line 1303 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
/* OS.chpl:1303 */
#line 1303 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
static ConnectionRefusedError_chpl _new_chpl72(string_chpl * details_chpl,
#line 1303 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                                               syserr err_chpl8,
#line 1303 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                                               int64_t _ln_chpl,
#line 1303 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                                               int32_t _fn_chpl) {
#line 1303 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ConnectionRefusedError_chpl initTemp_chpl = NULL;
#line 1303 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  c_ptr_void_chpl cast_tmp_chpl = NULL;
#line 1303 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ConnectionError_chpl super_tmp_chpl = NULL;
#line 1303 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  SystemError_chpl super_tmp_chpl2 = NULL;
#line 1303 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  Error_chpl super_tmp_chpl3 = NULL;
#line 1303 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  Error_chpl init_coerce_tmp_chpl = NULL;
#line 1303 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  string_chpl _msg_chpl;
#line 1303 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  string_chpl details_chpl2;
#line 1303 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  cast_tmp_chpl = chpl_here_alloc(sizeof(chpl_ConnectionRefusedError_chpl_object), INT16(15), _ln_chpl, _fn_chpl);
#line 1303 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  initTemp_chpl = ((ConnectionRefusedError_chpl)(cast_tmp_chpl));
#line 1303 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(initTemp_chpl))->chpl__cid = chpl__cid_ConnectionRefusedError_chpl;
#line 1303 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(initTemp_chpl, _ln_chpl, _fn_chpl);
#line 1303 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  super_tmp_chpl = &((initTemp_chpl)->super);
#line 1303 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 1303 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  super_tmp_chpl2 = &((super_tmp_chpl)->super);
#line 1303 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 1303 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  super_tmp_chpl3 = &((super_tmp_chpl2)->super);
#line 1303 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 1303 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(&((super_tmp_chpl3)->super)))->chpl__cid = chpl__cid_RootClass_chpl;
#line 1303 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(((RootClass_chpl)(super_tmp_chpl3))))->chpl__cid = chpl__cid_RootClass_chpl;
#line 1303 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  init_coerce_tmp_chpl = ((Error_chpl)(nil));
#line 1303 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 1303 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  (super_tmp_chpl3)->_next = init_coerce_tmp_chpl;
#line 1303 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 1303 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  (super_tmp_chpl3)->thrownLine = INT64(0);
#line 1303 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 1303 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  (super_tmp_chpl3)->thrownFileId = INT32(0);
#line 1303 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  init_chpl194(&_msg_chpl, _ln_chpl, _fn_chpl);
#line 1303 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 1303 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  (super_tmp_chpl3)->_msg = _msg_chpl;
#line 1303 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 1303 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  (super_tmp_chpl3)->_hasThrowInfo = UINT8(false);
#line 1303 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(super_tmp_chpl3))->chpl__cid = chpl__cid_Error_chpl;
#line 1303 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(((Error_chpl)(super_tmp_chpl2))))->chpl__cid = chpl__cid_Error_chpl;
#line 1303 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 1303 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  (super_tmp_chpl2)->err = err_chpl8;
#line 1303 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  init_ASSIGN__chpl25(&details_chpl2, details_chpl, _ln_chpl, _fn_chpl);
#line 1303 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 1303 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  (super_tmp_chpl2)->details = details_chpl2;
#line 1303 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(super_tmp_chpl2))->chpl__cid = chpl__cid_SystemError_chpl;
#line 1303 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(((SystemError_chpl)(super_tmp_chpl))))->chpl__cid = chpl__cid_SystemError_chpl;
#line 1303 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(super_tmp_chpl))->chpl__cid = chpl__cid_ConnectionError_chpl;
#line 1303 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(((ConnectionError_chpl)(initTemp_chpl))))->chpl__cid = chpl__cid_ConnectionError_chpl;
#line 1303 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(initTemp_chpl))->chpl__cid = chpl__cid_ConnectionRefusedError_chpl;
#line 1303 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  return initTemp_chpl;
#line 1303 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
}

#line 1312 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
/* OS.chpl:1312 */
#line 1312 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
static void chpl__auto_destroy_ConnectionResetError(ConnectionResetError_chpl this_chpl31,
#line 1312 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                                                    int64_t _ln_chpl,
#line 1312 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                                                    int32_t _fn_chpl) {
#line 1312 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  SystemError_chpl _parent_destructor_tmp__chpl = NULL;
#line 1312 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _ref_string_chpl _field_destructor_tmp__chpl = NULL;
#line 1312 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  Error_chpl _parent_destructor_tmp__chpl2 = NULL;
#line 1312 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _ref_string_chpl _field_destructor_tmp__chpl2 = NULL;
#line 1312 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _parent_destructor_tmp__chpl = ((SystemError_chpl)(((ConnectionError_chpl)(this_chpl31))));
#line 1312 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(_parent_destructor_tmp__chpl, _ln_chpl, _fn_chpl);
#line 1312 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _field_destructor_tmp__chpl = &((_parent_destructor_tmp__chpl)->details);
#line 1312 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  deinit_chpl95(_field_destructor_tmp__chpl, _ln_chpl, _fn_chpl);
#line 1312 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _parent_destructor_tmp__chpl2 = ((Error_chpl)(_parent_destructor_tmp__chpl));
#line 1312 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(_parent_destructor_tmp__chpl2, _ln_chpl, _fn_chpl);
#line 1312 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _field_destructor_tmp__chpl2 = &((_parent_destructor_tmp__chpl2)->_msg);
#line 1312 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  deinit_chpl95(_field_destructor_tmp__chpl2, _ln_chpl, _fn_chpl);
#line 1312 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  return;
#line 1312 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
}

#line 1313 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
/* OS.chpl:1313 */
#line 1313 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
static ConnectionResetError_chpl _new_chpl73(string_chpl * details_chpl,
#line 1313 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                                             syserr err_chpl8,
#line 1313 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                                             int64_t _ln_chpl,
#line 1313 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                                             int32_t _fn_chpl) {
#line 1313 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ConnectionResetError_chpl initTemp_chpl = NULL;
#line 1313 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  c_ptr_void_chpl cast_tmp_chpl = NULL;
#line 1313 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ConnectionError_chpl super_tmp_chpl = NULL;
#line 1313 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  SystemError_chpl super_tmp_chpl2 = NULL;
#line 1313 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  Error_chpl super_tmp_chpl3 = NULL;
#line 1313 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  Error_chpl init_coerce_tmp_chpl = NULL;
#line 1313 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  string_chpl _msg_chpl;
#line 1313 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  string_chpl details_chpl2;
#line 1313 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  cast_tmp_chpl = chpl_here_alloc(sizeof(chpl_ConnectionResetError_chpl_object), INT16(16), _ln_chpl, _fn_chpl);
#line 1313 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  initTemp_chpl = ((ConnectionResetError_chpl)(cast_tmp_chpl));
#line 1313 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(initTemp_chpl))->chpl__cid = chpl__cid_ConnectionResetError_chpl;
#line 1313 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(initTemp_chpl, _ln_chpl, _fn_chpl);
#line 1313 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  super_tmp_chpl = &((initTemp_chpl)->super);
#line 1313 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 1313 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  super_tmp_chpl2 = &((super_tmp_chpl)->super);
#line 1313 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 1313 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  super_tmp_chpl3 = &((super_tmp_chpl2)->super);
#line 1313 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 1313 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(&((super_tmp_chpl3)->super)))->chpl__cid = chpl__cid_RootClass_chpl;
#line 1313 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(((RootClass_chpl)(super_tmp_chpl3))))->chpl__cid = chpl__cid_RootClass_chpl;
#line 1313 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  init_coerce_tmp_chpl = ((Error_chpl)(nil));
#line 1313 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 1313 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  (super_tmp_chpl3)->_next = init_coerce_tmp_chpl;
#line 1313 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 1313 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  (super_tmp_chpl3)->thrownLine = INT64(0);
#line 1313 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 1313 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  (super_tmp_chpl3)->thrownFileId = INT32(0);
#line 1313 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  init_chpl194(&_msg_chpl, _ln_chpl, _fn_chpl);
#line 1313 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 1313 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  (super_tmp_chpl3)->_msg = _msg_chpl;
#line 1313 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 1313 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  (super_tmp_chpl3)->_hasThrowInfo = UINT8(false);
#line 1313 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(super_tmp_chpl3))->chpl__cid = chpl__cid_Error_chpl;
#line 1313 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(((Error_chpl)(super_tmp_chpl2))))->chpl__cid = chpl__cid_Error_chpl;
#line 1313 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 1313 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  (super_tmp_chpl2)->err = err_chpl8;
#line 1313 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  init_ASSIGN__chpl25(&details_chpl2, details_chpl, _ln_chpl, _fn_chpl);
#line 1313 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 1313 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  (super_tmp_chpl2)->details = details_chpl2;
#line 1313 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(super_tmp_chpl2))->chpl__cid = chpl__cid_SystemError_chpl;
#line 1313 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(((SystemError_chpl)(super_tmp_chpl))))->chpl__cid = chpl__cid_SystemError_chpl;
#line 1313 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(super_tmp_chpl))->chpl__cid = chpl__cid_ConnectionError_chpl;
#line 1313 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(((ConnectionError_chpl)(initTemp_chpl))))->chpl__cid = chpl__cid_ConnectionError_chpl;
#line 1313 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(initTemp_chpl))->chpl__cid = chpl__cid_ConnectionResetError_chpl;
#line 1313 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  return initTemp_chpl;
#line 1313 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
}

#line 1322 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
/* OS.chpl:1322 */
#line 1322 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
static void chpl__auto_destroy_FileExistsError(FileExistsError_chpl this_chpl31,
#line 1322 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                                               int64_t _ln_chpl,
#line 1322 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                                               int32_t _fn_chpl) {
#line 1322 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  SystemError_chpl _parent_destructor_tmp__chpl = NULL;
#line 1322 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _ref_string_chpl _field_destructor_tmp__chpl = NULL;
#line 1322 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  Error_chpl _parent_destructor_tmp__chpl2 = NULL;
#line 1322 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _ref_string_chpl _field_destructor_tmp__chpl2 = NULL;
#line 1322 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _parent_destructor_tmp__chpl = ((SystemError_chpl)(this_chpl31));
#line 1322 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(_parent_destructor_tmp__chpl, _ln_chpl, _fn_chpl);
#line 1322 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _field_destructor_tmp__chpl = &((_parent_destructor_tmp__chpl)->details);
#line 1322 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  deinit_chpl95(_field_destructor_tmp__chpl, _ln_chpl, _fn_chpl);
#line 1322 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _parent_destructor_tmp__chpl2 = ((Error_chpl)(_parent_destructor_tmp__chpl));
#line 1322 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(_parent_destructor_tmp__chpl2, _ln_chpl, _fn_chpl);
#line 1322 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _field_destructor_tmp__chpl2 = &((_parent_destructor_tmp__chpl2)->_msg);
#line 1322 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  deinit_chpl95(_field_destructor_tmp__chpl2, _ln_chpl, _fn_chpl);
#line 1322 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  return;
#line 1322 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
}

#line 1323 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
/* OS.chpl:1323 */
#line 1323 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
static FileExistsError_chpl _new_chpl74(string_chpl * details_chpl,
#line 1323 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                                        syserr err_chpl8,
#line 1323 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                                        int64_t _ln_chpl,
#line 1323 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                                        int32_t _fn_chpl) {
#line 1323 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  FileExistsError_chpl initTemp_chpl = NULL;
#line 1323 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  c_ptr_void_chpl cast_tmp_chpl = NULL;
#line 1323 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  SystemError_chpl super_tmp_chpl = NULL;
#line 1323 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  Error_chpl super_tmp_chpl2 = NULL;
#line 1323 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  Error_chpl init_coerce_tmp_chpl = NULL;
#line 1323 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  string_chpl _msg_chpl;
#line 1323 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  string_chpl details_chpl2;
#line 1323 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  cast_tmp_chpl = chpl_here_alloc(sizeof(chpl_FileExistsError_chpl_object), INT16(17), _ln_chpl, _fn_chpl);
#line 1323 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  initTemp_chpl = ((FileExistsError_chpl)(cast_tmp_chpl));
#line 1323 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(initTemp_chpl))->chpl__cid = chpl__cid_FileExistsError_chpl;
#line 1323 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(initTemp_chpl, _ln_chpl, _fn_chpl);
#line 1323 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  super_tmp_chpl = &((initTemp_chpl)->super);
#line 1323 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 1323 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  super_tmp_chpl2 = &((super_tmp_chpl)->super);
#line 1323 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 1323 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(&((super_tmp_chpl2)->super)))->chpl__cid = chpl__cid_RootClass_chpl;
#line 1323 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(((RootClass_chpl)(super_tmp_chpl2))))->chpl__cid = chpl__cid_RootClass_chpl;
#line 1323 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  init_coerce_tmp_chpl = ((Error_chpl)(nil));
#line 1323 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 1323 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  (super_tmp_chpl2)->_next = init_coerce_tmp_chpl;
#line 1323 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 1323 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  (super_tmp_chpl2)->thrownLine = INT64(0);
#line 1323 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 1323 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  (super_tmp_chpl2)->thrownFileId = INT32(0);
#line 1323 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  init_chpl194(&_msg_chpl, _ln_chpl, _fn_chpl);
#line 1323 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 1323 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  (super_tmp_chpl2)->_msg = _msg_chpl;
#line 1323 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 1323 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  (super_tmp_chpl2)->_hasThrowInfo = UINT8(false);
#line 1323 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(super_tmp_chpl2))->chpl__cid = chpl__cid_Error_chpl;
#line 1323 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(((Error_chpl)(super_tmp_chpl))))->chpl__cid = chpl__cid_Error_chpl;
#line 1323 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 1323 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  (super_tmp_chpl)->err = err_chpl8;
#line 1323 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  init_ASSIGN__chpl25(&details_chpl2, details_chpl, _ln_chpl, _fn_chpl);
#line 1323 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 1323 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  (super_tmp_chpl)->details = details_chpl2;
#line 1323 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(super_tmp_chpl))->chpl__cid = chpl__cid_SystemError_chpl;
#line 1323 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(((SystemError_chpl)(initTemp_chpl))))->chpl__cid = chpl__cid_SystemError_chpl;
#line 1323 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(initTemp_chpl))->chpl__cid = chpl__cid_FileExistsError_chpl;
#line 1323 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  return initTemp_chpl;
#line 1323 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
}

#line 1332 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
/* OS.chpl:1332 */
#line 1332 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
static void chpl__auto_destroy_FileNotFoundError(FileNotFoundError_chpl this_chpl31,
#line 1332 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                                                 int64_t _ln_chpl,
#line 1332 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                                                 int32_t _fn_chpl) {
#line 1332 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  SystemError_chpl _parent_destructor_tmp__chpl = NULL;
#line 1332 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _ref_string_chpl _field_destructor_tmp__chpl = NULL;
#line 1332 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  Error_chpl _parent_destructor_tmp__chpl2 = NULL;
#line 1332 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _ref_string_chpl _field_destructor_tmp__chpl2 = NULL;
#line 1332 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _parent_destructor_tmp__chpl = ((SystemError_chpl)(this_chpl31));
#line 1332 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(_parent_destructor_tmp__chpl, _ln_chpl, _fn_chpl);
#line 1332 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _field_destructor_tmp__chpl = &((_parent_destructor_tmp__chpl)->details);
#line 1332 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  deinit_chpl95(_field_destructor_tmp__chpl, _ln_chpl, _fn_chpl);
#line 1332 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _parent_destructor_tmp__chpl2 = ((Error_chpl)(_parent_destructor_tmp__chpl));
#line 1332 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(_parent_destructor_tmp__chpl2, _ln_chpl, _fn_chpl);
#line 1332 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _field_destructor_tmp__chpl2 = &((_parent_destructor_tmp__chpl2)->_msg);
#line 1332 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  deinit_chpl95(_field_destructor_tmp__chpl2, _ln_chpl, _fn_chpl);
#line 1332 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  return;
#line 1332 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
}

#line 1333 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
/* OS.chpl:1333 */
#line 1333 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
static FileNotFoundError_chpl _new_chpl75(string_chpl * details_chpl,
#line 1333 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                                          syserr err_chpl8,
#line 1333 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                                          int64_t _ln_chpl,
#line 1333 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                                          int32_t _fn_chpl) {
#line 1333 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  FileNotFoundError_chpl initTemp_chpl = NULL;
#line 1333 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  c_ptr_void_chpl cast_tmp_chpl = NULL;
#line 1333 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  SystemError_chpl super_tmp_chpl = NULL;
#line 1333 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  Error_chpl super_tmp_chpl2 = NULL;
#line 1333 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  Error_chpl init_coerce_tmp_chpl = NULL;
#line 1333 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  string_chpl _msg_chpl;
#line 1333 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  string_chpl details_chpl2;
#line 1333 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  cast_tmp_chpl = chpl_here_alloc(sizeof(chpl_FileNotFoundError_chpl_object), INT16(18), _ln_chpl, _fn_chpl);
#line 1333 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  initTemp_chpl = ((FileNotFoundError_chpl)(cast_tmp_chpl));
#line 1333 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(initTemp_chpl))->chpl__cid = chpl__cid_FileNotFoundError_chpl;
#line 1333 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(initTemp_chpl, _ln_chpl, _fn_chpl);
#line 1333 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  super_tmp_chpl = &((initTemp_chpl)->super);
#line 1333 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 1333 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  super_tmp_chpl2 = &((super_tmp_chpl)->super);
#line 1333 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 1333 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(&((super_tmp_chpl2)->super)))->chpl__cid = chpl__cid_RootClass_chpl;
#line 1333 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(((RootClass_chpl)(super_tmp_chpl2))))->chpl__cid = chpl__cid_RootClass_chpl;
#line 1333 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  init_coerce_tmp_chpl = ((Error_chpl)(nil));
#line 1333 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 1333 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  (super_tmp_chpl2)->_next = init_coerce_tmp_chpl;
#line 1333 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 1333 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  (super_tmp_chpl2)->thrownLine = INT64(0);
#line 1333 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 1333 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  (super_tmp_chpl2)->thrownFileId = INT32(0);
#line 1333 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  init_chpl194(&_msg_chpl, _ln_chpl, _fn_chpl);
#line 1333 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 1333 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  (super_tmp_chpl2)->_msg = _msg_chpl;
#line 1333 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 1333 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  (super_tmp_chpl2)->_hasThrowInfo = UINT8(false);
#line 1333 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(super_tmp_chpl2))->chpl__cid = chpl__cid_Error_chpl;
#line 1333 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(((Error_chpl)(super_tmp_chpl))))->chpl__cid = chpl__cid_Error_chpl;
#line 1333 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 1333 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  (super_tmp_chpl)->err = err_chpl8;
#line 1333 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  init_ASSIGN__chpl25(&details_chpl2, details_chpl, _ln_chpl, _fn_chpl);
#line 1333 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 1333 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  (super_tmp_chpl)->details = details_chpl2;
#line 1333 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(super_tmp_chpl))->chpl__cid = chpl__cid_SystemError_chpl;
#line 1333 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(((SystemError_chpl)(initTemp_chpl))))->chpl__cid = chpl__cid_SystemError_chpl;
#line 1333 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(initTemp_chpl))->chpl__cid = chpl__cid_FileNotFoundError_chpl;
#line 1333 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  return initTemp_chpl;
#line 1333 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
}

#line 1342 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
/* OS.chpl:1342 */
#line 1342 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
static void chpl__auto_destroy_InterruptedError(InterruptedError_chpl this_chpl31,
#line 1342 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                                                int64_t _ln_chpl,
#line 1342 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                                                int32_t _fn_chpl) {
#line 1342 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  SystemError_chpl _parent_destructor_tmp__chpl = NULL;
#line 1342 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _ref_string_chpl _field_destructor_tmp__chpl = NULL;
#line 1342 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  Error_chpl _parent_destructor_tmp__chpl2 = NULL;
#line 1342 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _ref_string_chpl _field_destructor_tmp__chpl2 = NULL;
#line 1342 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _parent_destructor_tmp__chpl = ((SystemError_chpl)(this_chpl31));
#line 1342 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(_parent_destructor_tmp__chpl, _ln_chpl, _fn_chpl);
#line 1342 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _field_destructor_tmp__chpl = &((_parent_destructor_tmp__chpl)->details);
#line 1342 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  deinit_chpl95(_field_destructor_tmp__chpl, _ln_chpl, _fn_chpl);
#line 1342 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _parent_destructor_tmp__chpl2 = ((Error_chpl)(_parent_destructor_tmp__chpl));
#line 1342 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(_parent_destructor_tmp__chpl2, _ln_chpl, _fn_chpl);
#line 1342 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _field_destructor_tmp__chpl2 = &((_parent_destructor_tmp__chpl2)->_msg);
#line 1342 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  deinit_chpl95(_field_destructor_tmp__chpl2, _ln_chpl, _fn_chpl);
#line 1342 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  return;
#line 1342 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
}

#line 1343 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
/* OS.chpl:1343 */
#line 1343 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
static InterruptedError_chpl _new_chpl76(string_chpl * details_chpl,
#line 1343 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                                         syserr err_chpl8,
#line 1343 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                                         int64_t _ln_chpl,
#line 1343 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                                         int32_t _fn_chpl) {
#line 1343 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  InterruptedError_chpl initTemp_chpl = NULL;
#line 1343 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  c_ptr_void_chpl cast_tmp_chpl = NULL;
#line 1343 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  SystemError_chpl super_tmp_chpl = NULL;
#line 1343 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  Error_chpl super_tmp_chpl2 = NULL;
#line 1343 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  Error_chpl init_coerce_tmp_chpl = NULL;
#line 1343 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  string_chpl _msg_chpl;
#line 1343 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  string_chpl details_chpl2;
#line 1343 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  cast_tmp_chpl = chpl_here_alloc(sizeof(chpl_InterruptedError_chpl_object), INT16(19), _ln_chpl, _fn_chpl);
#line 1343 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  initTemp_chpl = ((InterruptedError_chpl)(cast_tmp_chpl));
#line 1343 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(initTemp_chpl))->chpl__cid = chpl__cid_InterruptedError_chpl;
#line 1343 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(initTemp_chpl, _ln_chpl, _fn_chpl);
#line 1343 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  super_tmp_chpl = &((initTemp_chpl)->super);
#line 1343 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 1343 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  super_tmp_chpl2 = &((super_tmp_chpl)->super);
#line 1343 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 1343 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(&((super_tmp_chpl2)->super)))->chpl__cid = chpl__cid_RootClass_chpl;
#line 1343 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(((RootClass_chpl)(super_tmp_chpl2))))->chpl__cid = chpl__cid_RootClass_chpl;
#line 1343 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  init_coerce_tmp_chpl = ((Error_chpl)(nil));
#line 1343 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 1343 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  (super_tmp_chpl2)->_next = init_coerce_tmp_chpl;
#line 1343 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 1343 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  (super_tmp_chpl2)->thrownLine = INT64(0);
#line 1343 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 1343 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  (super_tmp_chpl2)->thrownFileId = INT32(0);
#line 1343 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  init_chpl194(&_msg_chpl, _ln_chpl, _fn_chpl);
#line 1343 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 1343 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  (super_tmp_chpl2)->_msg = _msg_chpl;
#line 1343 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 1343 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  (super_tmp_chpl2)->_hasThrowInfo = UINT8(false);
#line 1343 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(super_tmp_chpl2))->chpl__cid = chpl__cid_Error_chpl;
#line 1343 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(((Error_chpl)(super_tmp_chpl))))->chpl__cid = chpl__cid_Error_chpl;
#line 1343 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 1343 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  (super_tmp_chpl)->err = err_chpl8;
#line 1343 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  init_ASSIGN__chpl25(&details_chpl2, details_chpl, _ln_chpl, _fn_chpl);
#line 1343 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 1343 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  (super_tmp_chpl)->details = details_chpl2;
#line 1343 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(super_tmp_chpl))->chpl__cid = chpl__cid_SystemError_chpl;
#line 1343 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(((SystemError_chpl)(initTemp_chpl))))->chpl__cid = chpl__cid_SystemError_chpl;
#line 1343 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(initTemp_chpl))->chpl__cid = chpl__cid_InterruptedError_chpl;
#line 1343 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  return initTemp_chpl;
#line 1343 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
}

#line 1352 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
/* OS.chpl:1352 */
#line 1352 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
static void chpl__auto_destroy_IsADirectoryError(IsADirectoryError_chpl this_chpl31,
#line 1352 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                                                 int64_t _ln_chpl,
#line 1352 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                                                 int32_t _fn_chpl) {
#line 1352 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  SystemError_chpl _parent_destructor_tmp__chpl = NULL;
#line 1352 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _ref_string_chpl _field_destructor_tmp__chpl = NULL;
#line 1352 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  Error_chpl _parent_destructor_tmp__chpl2 = NULL;
#line 1352 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _ref_string_chpl _field_destructor_tmp__chpl2 = NULL;
#line 1352 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _parent_destructor_tmp__chpl = ((SystemError_chpl)(this_chpl31));
#line 1352 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(_parent_destructor_tmp__chpl, _ln_chpl, _fn_chpl);
#line 1352 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _field_destructor_tmp__chpl = &((_parent_destructor_tmp__chpl)->details);
#line 1352 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  deinit_chpl95(_field_destructor_tmp__chpl, _ln_chpl, _fn_chpl);
#line 1352 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _parent_destructor_tmp__chpl2 = ((Error_chpl)(_parent_destructor_tmp__chpl));
#line 1352 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(_parent_destructor_tmp__chpl2, _ln_chpl, _fn_chpl);
#line 1352 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _field_destructor_tmp__chpl2 = &((_parent_destructor_tmp__chpl2)->_msg);
#line 1352 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  deinit_chpl95(_field_destructor_tmp__chpl2, _ln_chpl, _fn_chpl);
#line 1352 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  return;
#line 1352 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
}

#line 1353 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
/* OS.chpl:1353 */
#line 1353 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
static IsADirectoryError_chpl _new_chpl77(string_chpl * details_chpl,
#line 1353 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                                          syserr err_chpl8,
#line 1353 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                                          int64_t _ln_chpl,
#line 1353 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                                          int32_t _fn_chpl) {
#line 1353 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  IsADirectoryError_chpl initTemp_chpl = NULL;
#line 1353 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  c_ptr_void_chpl cast_tmp_chpl = NULL;
#line 1353 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  SystemError_chpl super_tmp_chpl = NULL;
#line 1353 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  Error_chpl super_tmp_chpl2 = NULL;
#line 1353 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  Error_chpl init_coerce_tmp_chpl = NULL;
#line 1353 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  string_chpl _msg_chpl;
#line 1353 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  string_chpl details_chpl2;
#line 1353 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  cast_tmp_chpl = chpl_here_alloc(sizeof(chpl_IsADirectoryError_chpl_object), INT16(20), _ln_chpl, _fn_chpl);
#line 1353 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  initTemp_chpl = ((IsADirectoryError_chpl)(cast_tmp_chpl));
#line 1353 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(initTemp_chpl))->chpl__cid = chpl__cid_IsADirectoryError_chpl;
#line 1353 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(initTemp_chpl, _ln_chpl, _fn_chpl);
#line 1353 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  super_tmp_chpl = &((initTemp_chpl)->super);
#line 1353 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 1353 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  super_tmp_chpl2 = &((super_tmp_chpl)->super);
#line 1353 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 1353 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(&((super_tmp_chpl2)->super)))->chpl__cid = chpl__cid_RootClass_chpl;
#line 1353 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(((RootClass_chpl)(super_tmp_chpl2))))->chpl__cid = chpl__cid_RootClass_chpl;
#line 1353 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  init_coerce_tmp_chpl = ((Error_chpl)(nil));
#line 1353 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 1353 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  (super_tmp_chpl2)->_next = init_coerce_tmp_chpl;
#line 1353 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 1353 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  (super_tmp_chpl2)->thrownLine = INT64(0);
#line 1353 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 1353 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  (super_tmp_chpl2)->thrownFileId = INT32(0);
#line 1353 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  init_chpl194(&_msg_chpl, _ln_chpl, _fn_chpl);
#line 1353 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 1353 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  (super_tmp_chpl2)->_msg = _msg_chpl;
#line 1353 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 1353 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  (super_tmp_chpl2)->_hasThrowInfo = UINT8(false);
#line 1353 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(super_tmp_chpl2))->chpl__cid = chpl__cid_Error_chpl;
#line 1353 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(((Error_chpl)(super_tmp_chpl))))->chpl__cid = chpl__cid_Error_chpl;
#line 1353 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 1353 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  (super_tmp_chpl)->err = err_chpl8;
#line 1353 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  init_ASSIGN__chpl25(&details_chpl2, details_chpl, _ln_chpl, _fn_chpl);
#line 1353 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 1353 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  (super_tmp_chpl)->details = details_chpl2;
#line 1353 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(super_tmp_chpl))->chpl__cid = chpl__cid_SystemError_chpl;
#line 1353 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(((SystemError_chpl)(initTemp_chpl))))->chpl__cid = chpl__cid_SystemError_chpl;
#line 1353 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(initTemp_chpl))->chpl__cid = chpl__cid_IsADirectoryError_chpl;
#line 1353 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  return initTemp_chpl;
#line 1353 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
}

#line 1362 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
/* OS.chpl:1362 */
#line 1362 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
static void chpl__auto_destroy_NotADirectoryError(NotADirectoryError_chpl this_chpl31,
#line 1362 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                                                  int64_t _ln_chpl,
#line 1362 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                                                  int32_t _fn_chpl) {
#line 1362 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  SystemError_chpl _parent_destructor_tmp__chpl = NULL;
#line 1362 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _ref_string_chpl _field_destructor_tmp__chpl = NULL;
#line 1362 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  Error_chpl _parent_destructor_tmp__chpl2 = NULL;
#line 1362 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _ref_string_chpl _field_destructor_tmp__chpl2 = NULL;
#line 1362 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _parent_destructor_tmp__chpl = ((SystemError_chpl)(this_chpl31));
#line 1362 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(_parent_destructor_tmp__chpl, _ln_chpl, _fn_chpl);
#line 1362 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _field_destructor_tmp__chpl = &((_parent_destructor_tmp__chpl)->details);
#line 1362 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  deinit_chpl95(_field_destructor_tmp__chpl, _ln_chpl, _fn_chpl);
#line 1362 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _parent_destructor_tmp__chpl2 = ((Error_chpl)(_parent_destructor_tmp__chpl));
#line 1362 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(_parent_destructor_tmp__chpl2, _ln_chpl, _fn_chpl);
#line 1362 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _field_destructor_tmp__chpl2 = &((_parent_destructor_tmp__chpl2)->_msg);
#line 1362 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  deinit_chpl95(_field_destructor_tmp__chpl2, _ln_chpl, _fn_chpl);
#line 1362 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  return;
#line 1362 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
}

#line 1363 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
/* OS.chpl:1363 */
#line 1363 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
static NotADirectoryError_chpl _new_chpl78(string_chpl * details_chpl,
#line 1363 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                                           syserr err_chpl8,
#line 1363 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                                           int64_t _ln_chpl,
#line 1363 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                                           int32_t _fn_chpl) {
#line 1363 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  NotADirectoryError_chpl initTemp_chpl = NULL;
#line 1363 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  c_ptr_void_chpl cast_tmp_chpl = NULL;
#line 1363 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  SystemError_chpl super_tmp_chpl = NULL;
#line 1363 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  Error_chpl super_tmp_chpl2 = NULL;
#line 1363 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  Error_chpl init_coerce_tmp_chpl = NULL;
#line 1363 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  string_chpl _msg_chpl;
#line 1363 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  string_chpl details_chpl2;
#line 1363 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  cast_tmp_chpl = chpl_here_alloc(sizeof(chpl_NotADirectoryError_chpl_object), INT16(21), _ln_chpl, _fn_chpl);
#line 1363 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  initTemp_chpl = ((NotADirectoryError_chpl)(cast_tmp_chpl));
#line 1363 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(initTemp_chpl))->chpl__cid = chpl__cid_NotADirectoryError_chpl;
#line 1363 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(initTemp_chpl, _ln_chpl, _fn_chpl);
#line 1363 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  super_tmp_chpl = &((initTemp_chpl)->super);
#line 1363 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 1363 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  super_tmp_chpl2 = &((super_tmp_chpl)->super);
#line 1363 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 1363 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(&((super_tmp_chpl2)->super)))->chpl__cid = chpl__cid_RootClass_chpl;
#line 1363 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(((RootClass_chpl)(super_tmp_chpl2))))->chpl__cid = chpl__cid_RootClass_chpl;
#line 1363 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  init_coerce_tmp_chpl = ((Error_chpl)(nil));
#line 1363 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 1363 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  (super_tmp_chpl2)->_next = init_coerce_tmp_chpl;
#line 1363 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 1363 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  (super_tmp_chpl2)->thrownLine = INT64(0);
#line 1363 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 1363 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  (super_tmp_chpl2)->thrownFileId = INT32(0);
#line 1363 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  init_chpl194(&_msg_chpl, _ln_chpl, _fn_chpl);
#line 1363 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 1363 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  (super_tmp_chpl2)->_msg = _msg_chpl;
#line 1363 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 1363 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  (super_tmp_chpl2)->_hasThrowInfo = UINT8(false);
#line 1363 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(super_tmp_chpl2))->chpl__cid = chpl__cid_Error_chpl;
#line 1363 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(((Error_chpl)(super_tmp_chpl))))->chpl__cid = chpl__cid_Error_chpl;
#line 1363 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 1363 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  (super_tmp_chpl)->err = err_chpl8;
#line 1363 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  init_ASSIGN__chpl25(&details_chpl2, details_chpl, _ln_chpl, _fn_chpl);
#line 1363 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 1363 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  (super_tmp_chpl)->details = details_chpl2;
#line 1363 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(super_tmp_chpl))->chpl__cid = chpl__cid_SystemError_chpl;
#line 1363 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(((SystemError_chpl)(initTemp_chpl))))->chpl__cid = chpl__cid_SystemError_chpl;
#line 1363 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(initTemp_chpl))->chpl__cid = chpl__cid_NotADirectoryError_chpl;
#line 1363 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  return initTemp_chpl;
#line 1363 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
}

#line 1372 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
/* OS.chpl:1372 */
#line 1372 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
static void chpl__auto_destroy_PermissionError(PermissionError_chpl this_chpl31,
#line 1372 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                                               int64_t _ln_chpl,
#line 1372 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                                               int32_t _fn_chpl) {
#line 1372 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  SystemError_chpl _parent_destructor_tmp__chpl = NULL;
#line 1372 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _ref_string_chpl _field_destructor_tmp__chpl = NULL;
#line 1372 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  Error_chpl _parent_destructor_tmp__chpl2 = NULL;
#line 1372 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _ref_string_chpl _field_destructor_tmp__chpl2 = NULL;
#line 1372 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _parent_destructor_tmp__chpl = ((SystemError_chpl)(this_chpl31));
#line 1372 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(_parent_destructor_tmp__chpl, _ln_chpl, _fn_chpl);
#line 1372 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _field_destructor_tmp__chpl = &((_parent_destructor_tmp__chpl)->details);
#line 1372 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  deinit_chpl95(_field_destructor_tmp__chpl, _ln_chpl, _fn_chpl);
#line 1372 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _parent_destructor_tmp__chpl2 = ((Error_chpl)(_parent_destructor_tmp__chpl));
#line 1372 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(_parent_destructor_tmp__chpl2, _ln_chpl, _fn_chpl);
#line 1372 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _field_destructor_tmp__chpl2 = &((_parent_destructor_tmp__chpl2)->_msg);
#line 1372 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  deinit_chpl95(_field_destructor_tmp__chpl2, _ln_chpl, _fn_chpl);
#line 1372 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  return;
#line 1372 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
}

#line 1373 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
/* OS.chpl:1373 */
#line 1373 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
static PermissionError_chpl _new_chpl79(string_chpl * details_chpl,
#line 1373 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                                        syserr err_chpl8,
#line 1373 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                                        int64_t _ln_chpl,
#line 1373 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                                        int32_t _fn_chpl) {
#line 1373 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  PermissionError_chpl initTemp_chpl = NULL;
#line 1373 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  c_ptr_void_chpl cast_tmp_chpl = NULL;
#line 1373 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  SystemError_chpl super_tmp_chpl = NULL;
#line 1373 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  Error_chpl super_tmp_chpl2 = NULL;
#line 1373 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  Error_chpl init_coerce_tmp_chpl = NULL;
#line 1373 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  string_chpl _msg_chpl;
#line 1373 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  string_chpl details_chpl2;
#line 1373 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  cast_tmp_chpl = chpl_here_alloc(sizeof(chpl_PermissionError_chpl_object), INT16(22), _ln_chpl, _fn_chpl);
#line 1373 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  initTemp_chpl = ((PermissionError_chpl)(cast_tmp_chpl));
#line 1373 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(initTemp_chpl))->chpl__cid = chpl__cid_PermissionError_chpl;
#line 1373 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(initTemp_chpl, _ln_chpl, _fn_chpl);
#line 1373 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  super_tmp_chpl = &((initTemp_chpl)->super);
#line 1373 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 1373 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  super_tmp_chpl2 = &((super_tmp_chpl)->super);
#line 1373 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 1373 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(&((super_tmp_chpl2)->super)))->chpl__cid = chpl__cid_RootClass_chpl;
#line 1373 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(((RootClass_chpl)(super_tmp_chpl2))))->chpl__cid = chpl__cid_RootClass_chpl;
#line 1373 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  init_coerce_tmp_chpl = ((Error_chpl)(nil));
#line 1373 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 1373 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  (super_tmp_chpl2)->_next = init_coerce_tmp_chpl;
#line 1373 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 1373 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  (super_tmp_chpl2)->thrownLine = INT64(0);
#line 1373 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 1373 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  (super_tmp_chpl2)->thrownFileId = INT32(0);
#line 1373 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  init_chpl194(&_msg_chpl, _ln_chpl, _fn_chpl);
#line 1373 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 1373 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  (super_tmp_chpl2)->_msg = _msg_chpl;
#line 1373 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 1373 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  (super_tmp_chpl2)->_hasThrowInfo = UINT8(false);
#line 1373 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(super_tmp_chpl2))->chpl__cid = chpl__cid_Error_chpl;
#line 1373 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(((Error_chpl)(super_tmp_chpl))))->chpl__cid = chpl__cid_Error_chpl;
#line 1373 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 1373 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  (super_tmp_chpl)->err = err_chpl8;
#line 1373 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  init_ASSIGN__chpl25(&details_chpl2, details_chpl, _ln_chpl, _fn_chpl);
#line 1373 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 1373 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  (super_tmp_chpl)->details = details_chpl2;
#line 1373 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(super_tmp_chpl))->chpl__cid = chpl__cid_SystemError_chpl;
#line 1373 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(((SystemError_chpl)(initTemp_chpl))))->chpl__cid = chpl__cid_SystemError_chpl;
#line 1373 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(initTemp_chpl))->chpl__cid = chpl__cid_PermissionError_chpl;
#line 1373 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  return initTemp_chpl;
#line 1373 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
}

#line 1382 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
/* OS.chpl:1382 */
#line 1382 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
static void chpl__auto_destroy_ProcessLookupError(ProcessLookupError_chpl this_chpl31,
#line 1382 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                                                  int64_t _ln_chpl,
#line 1382 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                                                  int32_t _fn_chpl) {
#line 1382 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  SystemError_chpl _parent_destructor_tmp__chpl = NULL;
#line 1382 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _ref_string_chpl _field_destructor_tmp__chpl = NULL;
#line 1382 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  Error_chpl _parent_destructor_tmp__chpl2 = NULL;
#line 1382 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _ref_string_chpl _field_destructor_tmp__chpl2 = NULL;
#line 1382 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _parent_destructor_tmp__chpl = ((SystemError_chpl)(this_chpl31));
#line 1382 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(_parent_destructor_tmp__chpl, _ln_chpl, _fn_chpl);
#line 1382 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _field_destructor_tmp__chpl = &((_parent_destructor_tmp__chpl)->details);
#line 1382 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  deinit_chpl95(_field_destructor_tmp__chpl, _ln_chpl, _fn_chpl);
#line 1382 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _parent_destructor_tmp__chpl2 = ((Error_chpl)(_parent_destructor_tmp__chpl));
#line 1382 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(_parent_destructor_tmp__chpl2, _ln_chpl, _fn_chpl);
#line 1382 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _field_destructor_tmp__chpl2 = &((_parent_destructor_tmp__chpl2)->_msg);
#line 1382 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  deinit_chpl95(_field_destructor_tmp__chpl2, _ln_chpl, _fn_chpl);
#line 1382 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  return;
#line 1382 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
}

#line 1383 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
/* OS.chpl:1383 */
#line 1383 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
static ProcessLookupError_chpl _new_chpl80(string_chpl * details_chpl,
#line 1383 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                                           syserr err_chpl8,
#line 1383 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                                           int64_t _ln_chpl,
#line 1383 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                                           int32_t _fn_chpl) {
#line 1383 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ProcessLookupError_chpl initTemp_chpl = NULL;
#line 1383 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  c_ptr_void_chpl cast_tmp_chpl = NULL;
#line 1383 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  SystemError_chpl super_tmp_chpl = NULL;
#line 1383 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  Error_chpl super_tmp_chpl2 = NULL;
#line 1383 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  Error_chpl init_coerce_tmp_chpl = NULL;
#line 1383 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  string_chpl _msg_chpl;
#line 1383 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  string_chpl details_chpl2;
#line 1383 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  cast_tmp_chpl = chpl_here_alloc(sizeof(chpl_ProcessLookupError_chpl_object), INT16(23), _ln_chpl, _fn_chpl);
#line 1383 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  initTemp_chpl = ((ProcessLookupError_chpl)(cast_tmp_chpl));
#line 1383 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(initTemp_chpl))->chpl__cid = chpl__cid_ProcessLookupError_chpl;
#line 1383 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(initTemp_chpl, _ln_chpl, _fn_chpl);
#line 1383 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  super_tmp_chpl = &((initTemp_chpl)->super);
#line 1383 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 1383 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  super_tmp_chpl2 = &((super_tmp_chpl)->super);
#line 1383 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 1383 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(&((super_tmp_chpl2)->super)))->chpl__cid = chpl__cid_RootClass_chpl;
#line 1383 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(((RootClass_chpl)(super_tmp_chpl2))))->chpl__cid = chpl__cid_RootClass_chpl;
#line 1383 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  init_coerce_tmp_chpl = ((Error_chpl)(nil));
#line 1383 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 1383 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  (super_tmp_chpl2)->_next = init_coerce_tmp_chpl;
#line 1383 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 1383 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  (super_tmp_chpl2)->thrownLine = INT64(0);
#line 1383 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 1383 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  (super_tmp_chpl2)->thrownFileId = INT32(0);
#line 1383 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  init_chpl194(&_msg_chpl, _ln_chpl, _fn_chpl);
#line 1383 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 1383 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  (super_tmp_chpl2)->_msg = _msg_chpl;
#line 1383 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 1383 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  (super_tmp_chpl2)->_hasThrowInfo = UINT8(false);
#line 1383 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(super_tmp_chpl2))->chpl__cid = chpl__cid_Error_chpl;
#line 1383 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(((Error_chpl)(super_tmp_chpl))))->chpl__cid = chpl__cid_Error_chpl;
#line 1383 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 1383 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  (super_tmp_chpl)->err = err_chpl8;
#line 1383 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  init_ASSIGN__chpl25(&details_chpl2, details_chpl, _ln_chpl, _fn_chpl);
#line 1383 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 1383 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  (super_tmp_chpl)->details = details_chpl2;
#line 1383 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(super_tmp_chpl))->chpl__cid = chpl__cid_SystemError_chpl;
#line 1383 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(((SystemError_chpl)(initTemp_chpl))))->chpl__cid = chpl__cid_SystemError_chpl;
#line 1383 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(initTemp_chpl))->chpl__cid = chpl__cid_ProcessLookupError_chpl;
#line 1383 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  return initTemp_chpl;
#line 1383 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
}

#line 1392 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
/* OS.chpl:1392 */
#line 1392 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
static void chpl__auto_destroy_TimeoutError(TimeoutError_chpl this_chpl31,
#line 1392 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                                            int64_t _ln_chpl,
#line 1392 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                                            int32_t _fn_chpl) {
#line 1392 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  SystemError_chpl _parent_destructor_tmp__chpl = NULL;
#line 1392 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _ref_string_chpl _field_destructor_tmp__chpl = NULL;
#line 1392 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  Error_chpl _parent_destructor_tmp__chpl2 = NULL;
#line 1392 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _ref_string_chpl _field_destructor_tmp__chpl2 = NULL;
#line 1392 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _parent_destructor_tmp__chpl = ((SystemError_chpl)(this_chpl31));
#line 1392 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(_parent_destructor_tmp__chpl, _ln_chpl, _fn_chpl);
#line 1392 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _field_destructor_tmp__chpl = &((_parent_destructor_tmp__chpl)->details);
#line 1392 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  deinit_chpl95(_field_destructor_tmp__chpl, _ln_chpl, _fn_chpl);
#line 1392 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _parent_destructor_tmp__chpl2 = ((Error_chpl)(_parent_destructor_tmp__chpl));
#line 1392 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(_parent_destructor_tmp__chpl2, _ln_chpl, _fn_chpl);
#line 1392 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _field_destructor_tmp__chpl2 = &((_parent_destructor_tmp__chpl2)->_msg);
#line 1392 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  deinit_chpl95(_field_destructor_tmp__chpl2, _ln_chpl, _fn_chpl);
#line 1392 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  return;
#line 1392 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
}

#line 1393 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
/* OS.chpl:1393 */
#line 1393 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
static TimeoutError_chpl _new_chpl81(string_chpl * details_chpl,
#line 1393 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                                     syserr err_chpl8,
#line 1393 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                                     int64_t _ln_chpl,
#line 1393 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                                     int32_t _fn_chpl) {
#line 1393 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  TimeoutError_chpl initTemp_chpl = NULL;
#line 1393 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  c_ptr_void_chpl cast_tmp_chpl = NULL;
#line 1393 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  SystemError_chpl super_tmp_chpl = NULL;
#line 1393 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  Error_chpl super_tmp_chpl2 = NULL;
#line 1393 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  Error_chpl init_coerce_tmp_chpl = NULL;
#line 1393 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  string_chpl _msg_chpl;
#line 1393 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  string_chpl details_chpl2;
#line 1393 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  cast_tmp_chpl = chpl_here_alloc(sizeof(chpl_TimeoutError_chpl_object), INT16(24), _ln_chpl, _fn_chpl);
#line 1393 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  initTemp_chpl = ((TimeoutError_chpl)(cast_tmp_chpl));
#line 1393 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(initTemp_chpl))->chpl__cid = chpl__cid_TimeoutError_chpl;
#line 1393 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(initTemp_chpl, _ln_chpl, _fn_chpl);
#line 1393 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  super_tmp_chpl = &((initTemp_chpl)->super);
#line 1393 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 1393 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  super_tmp_chpl2 = &((super_tmp_chpl)->super);
#line 1393 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 1393 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(&((super_tmp_chpl2)->super)))->chpl__cid = chpl__cid_RootClass_chpl;
#line 1393 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(((RootClass_chpl)(super_tmp_chpl2))))->chpl__cid = chpl__cid_RootClass_chpl;
#line 1393 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  init_coerce_tmp_chpl = ((Error_chpl)(nil));
#line 1393 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 1393 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  (super_tmp_chpl2)->_next = init_coerce_tmp_chpl;
#line 1393 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 1393 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  (super_tmp_chpl2)->thrownLine = INT64(0);
#line 1393 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 1393 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  (super_tmp_chpl2)->thrownFileId = INT32(0);
#line 1393 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  init_chpl194(&_msg_chpl, _ln_chpl, _fn_chpl);
#line 1393 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 1393 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  (super_tmp_chpl2)->_msg = _msg_chpl;
#line 1393 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 1393 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  (super_tmp_chpl2)->_hasThrowInfo = UINT8(false);
#line 1393 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(super_tmp_chpl2))->chpl__cid = chpl__cid_Error_chpl;
#line 1393 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(((Error_chpl)(super_tmp_chpl))))->chpl__cid = chpl__cid_Error_chpl;
#line 1393 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 1393 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  (super_tmp_chpl)->err = err_chpl8;
#line 1393 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  init_ASSIGN__chpl25(&details_chpl2, details_chpl, _ln_chpl, _fn_chpl);
#line 1393 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 1393 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  (super_tmp_chpl)->details = details_chpl2;
#line 1393 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(super_tmp_chpl))->chpl__cid = chpl__cid_SystemError_chpl;
#line 1393 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(((SystemError_chpl)(initTemp_chpl))))->chpl__cid = chpl__cid_SystemError_chpl;
#line 1393 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(initTemp_chpl))->chpl__cid = chpl__cid_TimeoutError_chpl;
#line 1393 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  return initTemp_chpl;
#line 1393 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
}

#line 1402 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
/* OS.chpl:1402 */
#line 1402 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
static void chpl__auto_destroy_IoError(IoError_chpl this_chpl31,
#line 1402 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                                       int64_t _ln_chpl,
#line 1402 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                                       int32_t _fn_chpl) {
#line 1402 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  SystemError_chpl _parent_destructor_tmp__chpl = NULL;
#line 1402 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _ref_string_chpl _field_destructor_tmp__chpl = NULL;
#line 1402 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  Error_chpl _parent_destructor_tmp__chpl2 = NULL;
#line 1402 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _ref_string_chpl _field_destructor_tmp__chpl2 = NULL;
#line 1402 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _parent_destructor_tmp__chpl = ((SystemError_chpl)(this_chpl31));
#line 1402 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(_parent_destructor_tmp__chpl, _ln_chpl, _fn_chpl);
#line 1402 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _field_destructor_tmp__chpl = &((_parent_destructor_tmp__chpl)->details);
#line 1402 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  deinit_chpl95(_field_destructor_tmp__chpl, _ln_chpl, _fn_chpl);
#line 1402 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _parent_destructor_tmp__chpl2 = ((Error_chpl)(_parent_destructor_tmp__chpl));
#line 1402 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(_parent_destructor_tmp__chpl2, _ln_chpl, _fn_chpl);
#line 1402 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _field_destructor_tmp__chpl2 = &((_parent_destructor_tmp__chpl2)->_msg);
#line 1402 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  deinit_chpl95(_field_destructor_tmp__chpl2, _ln_chpl, _fn_chpl);
#line 1402 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  return;
#line 1402 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
}

#line 1403 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
/* OS.chpl:1403 */
#line 1403 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
static IoError_chpl _new_chpl82(syserr err_chpl8,
#line 1403 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                                string_chpl * details_chpl,
#line 1403 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                                int64_t _ln_chpl,
#line 1403 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                                int32_t _fn_chpl) {
#line 1403 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  IoError_chpl initTemp_chpl = NULL;
#line 1403 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  c_ptr_void_chpl cast_tmp_chpl = NULL;
#line 1403 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  SystemError_chpl super_tmp_chpl = NULL;
#line 1403 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  Error_chpl super_tmp_chpl2 = NULL;
#line 1403 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  Error_chpl init_coerce_tmp_chpl = NULL;
#line 1403 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  string_chpl _msg_chpl;
#line 1403 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  string_chpl details_chpl2;
#line 1403 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  cast_tmp_chpl = chpl_here_alloc(sizeof(chpl_IoError_chpl_object), INT16(25), _ln_chpl, _fn_chpl);
#line 1403 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  initTemp_chpl = ((IoError_chpl)(cast_tmp_chpl));
#line 1403 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(initTemp_chpl))->chpl__cid = chpl__cid_IoError_chpl;
#line 1403 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(initTemp_chpl, _ln_chpl, _fn_chpl);
#line 1403 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  super_tmp_chpl = &((initTemp_chpl)->super);
#line 1403 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 1403 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  super_tmp_chpl2 = &((super_tmp_chpl)->super);
#line 1403 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 1403 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(&((super_tmp_chpl2)->super)))->chpl__cid = chpl__cid_RootClass_chpl;
#line 1403 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(((RootClass_chpl)(super_tmp_chpl2))))->chpl__cid = chpl__cid_RootClass_chpl;
#line 1403 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  init_coerce_tmp_chpl = ((Error_chpl)(nil));
#line 1403 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 1403 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  (super_tmp_chpl2)->_next = init_coerce_tmp_chpl;
#line 1403 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 1403 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  (super_tmp_chpl2)->thrownLine = INT64(0);
#line 1403 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 1403 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  (super_tmp_chpl2)->thrownFileId = INT32(0);
#line 1403 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  init_chpl194(&_msg_chpl, _ln_chpl, _fn_chpl);
#line 1403 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 1403 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  (super_tmp_chpl2)->_msg = _msg_chpl;
#line 1403 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 1403 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  (super_tmp_chpl2)->_hasThrowInfo = UINT8(false);
#line 1403 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(super_tmp_chpl2))->chpl__cid = chpl__cid_Error_chpl;
#line 1403 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(((Error_chpl)(super_tmp_chpl))))->chpl__cid = chpl__cid_Error_chpl;
#line 1403 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 1403 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  (super_tmp_chpl)->err = err_chpl8;
#line 1403 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  init_ASSIGN__chpl25(&details_chpl2, details_chpl, _ln_chpl, _fn_chpl);
#line 1403 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 1403 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  (super_tmp_chpl)->details = details_chpl2;
#line 1403 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(super_tmp_chpl))->chpl__cid = chpl__cid_SystemError_chpl;
#line 1403 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(((SystemError_chpl)(initTemp_chpl))))->chpl__cid = chpl__cid_SystemError_chpl;
#line 1403 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(initTemp_chpl))->chpl__cid = chpl__cid_IoError_chpl;
#line 1403 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  return initTemp_chpl;
#line 1403 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
}

#line 1412 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
/* OS.chpl:1412 */
#line 1412 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
static void chpl__auto_destroy_EofError(EofError_chpl this_chpl31,
#line 1412 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                                        int64_t _ln_chpl,
#line 1412 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                                        int32_t _fn_chpl) {
#line 1413 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _ref_string_chpl _field_destructor_tmp__chpl = NULL;
#line 1412 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  Error_chpl _parent_destructor_tmp__chpl = NULL;
#line 1412 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _ref_string_chpl _field_destructor_tmp__chpl2 = NULL;
#line 1413 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 1413 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _field_destructor_tmp__chpl = &((this_chpl31)->details);
#line 1413 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  deinit_chpl95(_field_destructor_tmp__chpl, _ln_chpl, _fn_chpl);
#line 1412 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _parent_destructor_tmp__chpl = ((Error_chpl)(this_chpl31));
#line 1412 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(_parent_destructor_tmp__chpl, _ln_chpl, _fn_chpl);
#line 1412 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _field_destructor_tmp__chpl2 = &((_parent_destructor_tmp__chpl)->_msg);
#line 1412 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  deinit_chpl95(_field_destructor_tmp__chpl2, _ln_chpl, _fn_chpl);
#line 1412 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  return;
#line 1412 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
}

#line 1415 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
/* OS.chpl:1415 */
#line 1415 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
static EofError_chpl _new_chpl83(string_chpl * details_chpl,
#line 1415 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                                 string_chpl * err_msg_chpl,
#line 1415 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                                 int64_t _ln_chpl,
#line 1415 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                                 int32_t _fn_chpl) {
#line 1415 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  EofError_chpl initTemp_chpl = NULL;
#line 1415 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  c_ptr_void_chpl cast_tmp_chpl = NULL;
#line 1415 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  Error_chpl super_tmp_chpl = NULL;
#line 1415 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  Error_chpl init_coerce_tmp_chpl = NULL;
#line 1415 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  string_chpl _msg_chpl;
#line 1415 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  string_chpl details_chpl2;
#line 1415 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _ref_string_chpl call_tmp_chpl6 = NULL;
#line 1415 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  cast_tmp_chpl = chpl_here_alloc(sizeof(chpl_EofError_chpl_object), INT16(8), _ln_chpl, _fn_chpl);
#line 1415 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  initTemp_chpl = ((EofError_chpl)(cast_tmp_chpl));
#line 1415 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(initTemp_chpl))->chpl__cid = chpl__cid_EofError_chpl;
#line 1415 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(initTemp_chpl, _ln_chpl, _fn_chpl);
#line 1415 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  super_tmp_chpl = &((initTemp_chpl)->super);
#line 1415 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 1415 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(&((super_tmp_chpl)->super)))->chpl__cid = chpl__cid_RootClass_chpl;
#line 1415 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(((RootClass_chpl)(super_tmp_chpl))))->chpl__cid = chpl__cid_RootClass_chpl;
#line 1415 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  init_coerce_tmp_chpl = ((Error_chpl)(nil));
#line 1415 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 1415 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  (super_tmp_chpl)->_next = init_coerce_tmp_chpl;
#line 1415 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 1415 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  (super_tmp_chpl)->thrownLine = INT64(0);
#line 1415 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 1415 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  (super_tmp_chpl)->thrownFileId = INT32(0);
#line 1415 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  init_chpl194(&_msg_chpl, _ln_chpl, _fn_chpl);
#line 1415 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 1415 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  (super_tmp_chpl)->_msg = _msg_chpl;
#line 1415 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 1415 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  (super_tmp_chpl)->_hasThrowInfo = UINT8(false);
#line 1415 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(super_tmp_chpl))->chpl__cid = chpl__cid_Error_chpl;
#line 1415 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(((Error_chpl)(initTemp_chpl))))->chpl__cid = chpl__cid_Error_chpl;
#line 1415 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  init_ASSIGN__chpl25(&details_chpl2, details_chpl, _ln_chpl, _fn_chpl);
#line 1415 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(initTemp_chpl, _ln_chpl, _fn_chpl);
#line 1415 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  (initTemp_chpl)->details = details_chpl2;
#line 1415 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(initTemp_chpl, _ln_chpl, _fn_chpl);
#line 1415 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  call_tmp_chpl6 = &(((Error_chpl)(initTemp_chpl))->_msg);
#line 1415 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl___ASSIGN_16(call_tmp_chpl6, err_msg_chpl, _ln_chpl, _fn_chpl);
#line 1415 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(initTemp_chpl))->chpl__cid = chpl__cid_EofError_chpl;
#line 1415 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  return initTemp_chpl;
#line 1415 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
}

#line 1420 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
/* OS.chpl:1420 */
#line 1420 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
static void message_chpl8(EofError_chpl this_chpl31,
#line 1420 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                          string_chpl * _retArg_chpl,
#line 1420 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                          int64_t _ln_chpl,
#line 1420 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                          int32_t _fn_chpl) {
#line 1420 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  string_chpl ret_chpl;
#line 1421 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  string_chpl generatedMsg_chpl;
#line 1423 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  Error_chpl coerce_tmp_chpl3 = NULL;
#line 1423 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _ref_string_chpl call_tmp_chpl6 = NULL;
#line 1424 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  Error_chpl coerce_tmp_chpl4 = NULL;
#line 1424 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _ref_string_chpl call_tmp_chpl7 = NULL;
#line 1427 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  int32_t call_tmp_chpl8;
#line 1427 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  syserr call_tmp_chpl9;
#line 1429 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  c_ptrConst_int8_t_chpl call_tmp_chpl10 = NULL;
#line 1429 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  int32_t _formal_tmp_out_err_in_strerror_chpl;
#line 1430 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  string_chpl errorcode_msg_chpl;
#line 1431 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  Error_chpl error_chpl = NULL;
#line 1432 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  string_chpl call_tmp_chpl11;
#line 1432 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  int64_t call_tmp_chpl12;
#line 1432 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _owned_IllegalArgumentError_chpl2 call_tmp_chpl13;
#line 1432 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _owned_IllegalArgumentError_chpl2 ret_tmp_chpl;
#line 1432 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _ref__owned_IllegalArgumentError_chpl i_arg_chpl = NULL;
#line 1432 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  IllegalArgumentError_chpl call_tmp_chpl14 = NULL;
#line 1432 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  string_chpl call_tmp_chpl15;
#line 1432 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  Error_chpl coerce_tmp_chpl5 = NULL;
#line 1432 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  string_chpl ret_tmp_chpl2;
#line 1432 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 1432 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  string_chpl ret_tmp_chpl3;
#line 1434 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _ref_string_chpl i_x_chpl2 = NULL;
#line 1438 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _ref_string_chpl call_tmp_chpl16 = NULL;
#line 1439 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _ref_string_chpl call_tmp_chpl17 = NULL;
#line 1439 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  string_chpl call_tmp_chpl18;
#line 1439 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  string_chpl ret_tmp_chpl4;
#line 1439 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  string_chpl call_tmp_chpl19;
#line 1439 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  string_chpl ret_tmp_chpl5;
#line 1439 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _ref_string_chpl i_x_chpl3 = NULL;
#line 1439 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _ref_string_chpl i_x_chpl4 = NULL;
#line 1421 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  init_chpl194(&generatedMsg_chpl, _ln_chpl, _fn_chpl);
#line 1423 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  coerce_tmp_chpl3 = ((Error_chpl)(this_chpl31));
#line 1423 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 1423 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  call_tmp_chpl6 = &((coerce_tmp_chpl3)->_msg);
#line 1423 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  if (! ((call_tmp_chpl6)->buffLen == INT64(0))) /* ZLINE: 1423 /Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl */
#line 1423 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  {
#line 1424 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    coerce_tmp_chpl4 = ((Error_chpl)(this_chpl31));
#line 1424 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    chpl_check_nil(coerce_tmp_chpl4, _ln_chpl, _fn_chpl);
#line 1424 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    call_tmp_chpl7 = &((coerce_tmp_chpl4)->_msg);
#line 1424 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    chpl___PLUS__ASSIGN_(&generatedMsg_chpl, call_tmp_chpl7, _ln_chpl, _fn_chpl);
#line 1424 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  } else /* ZLINE: 1425 /Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl */
#line 1424 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  {
#line 1427 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    call_tmp_chpl8 = chpl_macro_int_EEOF();
#line 1427 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    call_tmp_chpl9 = qio_int_to_err(call_tmp_chpl8);
#line 1429 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    call_tmp_chpl10 = sys_strerror_syserr_str(call_tmp_chpl9, &_formal_tmp_out_err_in_strerror_chpl);
#line 1431 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    error_chpl = NULL;
#line 1432 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    call_tmp_chpl12 = string_length_bytes(call_tmp_chpl10);
#line 1432 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    chpl_checkValue2(call_tmp_chpl12, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 1432 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    call_tmp_chpl13 = ret_tmp_chpl;
#line 1432 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    i_arg_chpl = &call_tmp_chpl13;
#line 1432 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    call_tmp_chpl14 = borrow_chpl2(i_arg_chpl);
#line 1432 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    if (((RootClass_chpl)(call_tmp_chpl14)) != nil) /* ZLINE: 1432 /Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl */
#line 1432 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    {
#line 1432 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
      coerce_tmp_chpl5 = ((Error_chpl)(call_tmp_chpl14));
#line 1432 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
      chpl_check_nil(coerce_tmp_chpl5, _ln_chpl, _fn_chpl);
#line 1432 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
      ((void(*)(Error_chpl,
#line 1432 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
         string_chpl *,
#line 1432 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
         int64_t,
#line 1432 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
         int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(coerce_tmp_chpl5))->chpl__cid) + INT64(1))])(coerce_tmp_chpl5, &ret_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 1432 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
      call_tmp_chpl15 = ret_tmp_chpl2;
#line 1432 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
      safeCastCheckHalt_chpl(&call_tmp_chpl15, _ln_chpl, _fn_chpl);
#line 1432 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
      i_x_chpl = &call_tmp_chpl15;
#line 1432 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
      deinit_chpl95(i_x_chpl, _ln_chpl, _fn_chpl);
#line 1432 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    }
#line 1432 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    chpl__autoDestroy62(&call_tmp_chpl13, _ln_chpl, _fn_chpl);
#line 1432 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    if (error_chpl != nil) /* ZLINE: 1432 /Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl */
#line 1432 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    {
#line 1432 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
      goto handler_chpl;
#line 1432 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    }
#line 1432 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    createAdoptingBuffer_chpl(call_tmp_chpl10, call_tmp_chpl12, &error_chpl, &ret_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 1432 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    call_tmp_chpl11 = ret_tmp_chpl3;
#line 1432 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    if (error_chpl != nil) /* ZLINE: 1432 /Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl */
#line 1432 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    {
#line 1432 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
      goto handler_chpl;
#line 1432 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    }
#line 1432 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    errorcode_msg_chpl = call_tmp_chpl11;
#line 1432 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    handler_chpl:;
#line 1431 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    if (error_chpl != nil) /* ZLINE: 1431 /Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl */
#line 1431 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    {
#line 1431 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
      chpl_uncaught_error(error_chpl, _ln_chpl, _fn_chpl);
#line 1431 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    }
#line 1434 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    chpl___PLUS__ASSIGN_(&generatedMsg_chpl, &errorcode_msg_chpl, _ln_chpl, _fn_chpl);
#line 1434 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    i_x_chpl2 = &errorcode_msg_chpl;
#line 1434 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    deinit_chpl95(i_x_chpl2, _ln_chpl, _fn_chpl);
#line 1434 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  }
#line 1438 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 1438 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  call_tmp_chpl16 = &((this_chpl31)->details);
#line 1438 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  if (! ((call_tmp_chpl16)->buffLen == INT64(0))) /* ZLINE: 1438 /Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl */
#line 1438 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  {
#line 1439 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 1439 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    call_tmp_chpl17 = &((this_chpl31)->details);
#line 1439 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    chpl___PLUS_(&_str_literal_734_chpl, call_tmp_chpl17, &ret_tmp_chpl4, _ln_chpl, _fn_chpl);
#line 1439 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    call_tmp_chpl18 = ret_tmp_chpl4;
#line 1439 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    chpl___PLUS_(&call_tmp_chpl18, &_str_literal_415_chpl, &ret_tmp_chpl5, _ln_chpl, _fn_chpl);
#line 1439 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    call_tmp_chpl19 = ret_tmp_chpl5;
#line 1439 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    chpl___PLUS__ASSIGN_(&generatedMsg_chpl, &call_tmp_chpl19, _ln_chpl, _fn_chpl);
#line 1439 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    i_x_chpl3 = &call_tmp_chpl19;
#line 1439 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    deinit_chpl95(i_x_chpl3, _ln_chpl, _fn_chpl);
#line 1439 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    i_x_chpl4 = &call_tmp_chpl18;
#line 1439 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    deinit_chpl95(i_x_chpl4, _ln_chpl, _fn_chpl);
#line 1439 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  }
#line 1441 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ret_chpl = generatedMsg_chpl;
#line 1420 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  *(_retArg_chpl) = ret_chpl;
#line 1420 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  return;
#line 1420 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
}

#line 1454 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
/* OS.chpl:1454 */
#line 1454 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
static void chpl__auto_destroy_UnexpectedEofError(UnexpectedEofError_chpl this_chpl31,
#line 1454 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                                                  int64_t _ln_chpl,
#line 1454 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                                                  int32_t _fn_chpl) {
#line 1455 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _ref_string_chpl _field_destructor_tmp__chpl = NULL;
#line 1454 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  Error_chpl _parent_destructor_tmp__chpl = NULL;
#line 1454 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _ref_string_chpl _field_destructor_tmp__chpl2 = NULL;
#line 1455 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 1455 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _field_destructor_tmp__chpl = &((this_chpl31)->details);
#line 1455 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  deinit_chpl95(_field_destructor_tmp__chpl, _ln_chpl, _fn_chpl);
#line 1454 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _parent_destructor_tmp__chpl = ((Error_chpl)(this_chpl31));
#line 1454 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(_parent_destructor_tmp__chpl, _ln_chpl, _fn_chpl);
#line 1454 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _field_destructor_tmp__chpl2 = &((_parent_destructor_tmp__chpl)->_msg);
#line 1454 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  deinit_chpl95(_field_destructor_tmp__chpl2, _ln_chpl, _fn_chpl);
#line 1454 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  return;
#line 1454 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
}

#line 1457 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
/* OS.chpl:1457 */
#line 1457 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
static UnexpectedEofError_chpl _new_chpl84(string_chpl * details_chpl,
#line 1457 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                                           string_chpl * err_msg_chpl,
#line 1457 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                                           int64_t _ln_chpl,
#line 1457 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                                           int32_t _fn_chpl) {
#line 1457 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  UnexpectedEofError_chpl initTemp_chpl = NULL;
#line 1457 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  c_ptr_void_chpl cast_tmp_chpl = NULL;
#line 1457 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  Error_chpl super_tmp_chpl = NULL;
#line 1457 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  Error_chpl init_coerce_tmp_chpl = NULL;
#line 1457 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  string_chpl _msg_chpl;
#line 1457 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  string_chpl details_chpl2;
#line 1457 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _ref_string_chpl call_tmp_chpl6 = NULL;
#line 1457 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  cast_tmp_chpl = chpl_here_alloc(sizeof(chpl_UnexpectedEofError_chpl_object), INT16(9), _ln_chpl, _fn_chpl);
#line 1457 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  initTemp_chpl = ((UnexpectedEofError_chpl)(cast_tmp_chpl));
#line 1457 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(initTemp_chpl))->chpl__cid = chpl__cid_UnexpectedEofError_chpl;
#line 1457 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(initTemp_chpl, _ln_chpl, _fn_chpl);
#line 1457 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  super_tmp_chpl = &((initTemp_chpl)->super);
#line 1457 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 1457 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(&((super_tmp_chpl)->super)))->chpl__cid = chpl__cid_RootClass_chpl;
#line 1457 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(((RootClass_chpl)(super_tmp_chpl))))->chpl__cid = chpl__cid_RootClass_chpl;
#line 1457 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  init_coerce_tmp_chpl = ((Error_chpl)(nil));
#line 1457 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 1457 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  (super_tmp_chpl)->_next = init_coerce_tmp_chpl;
#line 1457 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 1457 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  (super_tmp_chpl)->thrownLine = INT64(0);
#line 1457 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 1457 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  (super_tmp_chpl)->thrownFileId = INT32(0);
#line 1457 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  init_chpl194(&_msg_chpl, _ln_chpl, _fn_chpl);
#line 1457 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 1457 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  (super_tmp_chpl)->_msg = _msg_chpl;
#line 1457 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 1457 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  (super_tmp_chpl)->_hasThrowInfo = UINT8(false);
#line 1457 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(super_tmp_chpl))->chpl__cid = chpl__cid_Error_chpl;
#line 1457 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(((Error_chpl)(initTemp_chpl))))->chpl__cid = chpl__cid_Error_chpl;
#line 1457 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  init_ASSIGN__chpl25(&details_chpl2, details_chpl, _ln_chpl, _fn_chpl);
#line 1457 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(initTemp_chpl, _ln_chpl, _fn_chpl);
#line 1457 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  (initTemp_chpl)->details = details_chpl2;
#line 1457 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(initTemp_chpl, _ln_chpl, _fn_chpl);
#line 1457 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  call_tmp_chpl6 = &(((Error_chpl)(initTemp_chpl))->_msg);
#line 1457 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl___ASSIGN_16(call_tmp_chpl6, err_msg_chpl, _ln_chpl, _fn_chpl);
#line 1457 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(initTemp_chpl))->chpl__cid = chpl__cid_UnexpectedEofError_chpl;
#line 1457 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  return initTemp_chpl;
#line 1457 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
}

#line 1462 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
/* OS.chpl:1462 */
#line 1462 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
static void message_chpl9(UnexpectedEofError_chpl this_chpl31,
#line 1462 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                          string_chpl * _retArg_chpl,
#line 1462 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                          int64_t _ln_chpl,
#line 1462 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                          int32_t _fn_chpl) {
#line 1462 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  string_chpl ret_chpl;
#line 1463 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  string_chpl generatedMsg_chpl;
#line 1465 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  Error_chpl coerce_tmp_chpl3 = NULL;
#line 1465 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _ref_string_chpl call_tmp_chpl6 = NULL;
#line 1466 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  Error_chpl coerce_tmp_chpl4 = NULL;
#line 1466 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _ref_string_chpl call_tmp_chpl7 = NULL;
#line 1469 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  int32_t call_tmp_chpl8;
#line 1469 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  syserr call_tmp_chpl9;
#line 1471 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  c_ptrConst_int8_t_chpl call_tmp_chpl10 = NULL;
#line 1471 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  int32_t _formal_tmp_out_err_in_strerror_chpl;
#line 1472 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  string_chpl errorcode_msg_chpl;
#line 1473 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  Error_chpl error_chpl = NULL;
#line 1474 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  string_chpl call_tmp_chpl11;
#line 1474 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  int64_t call_tmp_chpl12;
#line 1474 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _owned_IllegalArgumentError_chpl2 call_tmp_chpl13;
#line 1474 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _owned_IllegalArgumentError_chpl2 ret_tmp_chpl;
#line 1474 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _ref__owned_IllegalArgumentError_chpl i_arg_chpl = NULL;
#line 1474 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  IllegalArgumentError_chpl call_tmp_chpl14 = NULL;
#line 1474 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  string_chpl call_tmp_chpl15;
#line 1474 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  Error_chpl coerce_tmp_chpl5 = NULL;
#line 1474 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  string_chpl ret_tmp_chpl2;
#line 1474 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 1474 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  string_chpl ret_tmp_chpl3;
#line 1476 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _ref_string_chpl i_x_chpl2 = NULL;
#line 1480 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _ref_string_chpl call_tmp_chpl16 = NULL;
#line 1481 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _ref_string_chpl call_tmp_chpl17 = NULL;
#line 1481 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  string_chpl call_tmp_chpl18;
#line 1481 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  string_chpl ret_tmp_chpl4;
#line 1481 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  string_chpl call_tmp_chpl19;
#line 1481 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  string_chpl ret_tmp_chpl5;
#line 1481 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _ref_string_chpl i_x_chpl3 = NULL;
#line 1481 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _ref_string_chpl i_x_chpl4 = NULL;
#line 1463 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  init_chpl194(&generatedMsg_chpl, _ln_chpl, _fn_chpl);
#line 1465 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  coerce_tmp_chpl3 = ((Error_chpl)(this_chpl31));
#line 1465 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 1465 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  call_tmp_chpl6 = &((coerce_tmp_chpl3)->_msg);
#line 1465 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  if (! ((call_tmp_chpl6)->buffLen == INT64(0))) /* ZLINE: 1465 /Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl */
#line 1465 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  {
#line 1466 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    coerce_tmp_chpl4 = ((Error_chpl)(this_chpl31));
#line 1466 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    chpl_check_nil(coerce_tmp_chpl4, _ln_chpl, _fn_chpl);
#line 1466 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    call_tmp_chpl7 = &((coerce_tmp_chpl4)->_msg);
#line 1466 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    chpl___PLUS__ASSIGN_(&generatedMsg_chpl, call_tmp_chpl7, _ln_chpl, _fn_chpl);
#line 1466 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  } else /* ZLINE: 1467 /Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl */
#line 1466 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  {
#line 1469 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    call_tmp_chpl8 = chpl_macro_int_ESHORT();
#line 1469 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    call_tmp_chpl9 = qio_int_to_err(call_tmp_chpl8);
#line 1471 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    call_tmp_chpl10 = sys_strerror_syserr_str(call_tmp_chpl9, &_formal_tmp_out_err_in_strerror_chpl);
#line 1473 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    error_chpl = NULL;
#line 1474 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    call_tmp_chpl12 = string_length_bytes(call_tmp_chpl10);
#line 1474 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    chpl_checkValue2(call_tmp_chpl12, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 1474 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    call_tmp_chpl13 = ret_tmp_chpl;
#line 1474 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    i_arg_chpl = &call_tmp_chpl13;
#line 1474 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    call_tmp_chpl14 = borrow_chpl2(i_arg_chpl);
#line 1474 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    if (((RootClass_chpl)(call_tmp_chpl14)) != nil) /* ZLINE: 1474 /Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl */
#line 1474 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    {
#line 1474 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
      coerce_tmp_chpl5 = ((Error_chpl)(call_tmp_chpl14));
#line 1474 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
      chpl_check_nil(coerce_tmp_chpl5, _ln_chpl, _fn_chpl);
#line 1474 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
      ((void(*)(Error_chpl,
#line 1474 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
         string_chpl *,
#line 1474 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
         int64_t,
#line 1474 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
         int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(coerce_tmp_chpl5))->chpl__cid) + INT64(1))])(coerce_tmp_chpl5, &ret_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 1474 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
      call_tmp_chpl15 = ret_tmp_chpl2;
#line 1474 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
      safeCastCheckHalt_chpl(&call_tmp_chpl15, _ln_chpl, _fn_chpl);
#line 1474 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
      i_x_chpl = &call_tmp_chpl15;
#line 1474 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
      deinit_chpl95(i_x_chpl, _ln_chpl, _fn_chpl);
#line 1474 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    }
#line 1474 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    chpl__autoDestroy62(&call_tmp_chpl13, _ln_chpl, _fn_chpl);
#line 1474 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    if (error_chpl != nil) /* ZLINE: 1474 /Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl */
#line 1474 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    {
#line 1474 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
      goto handler_chpl;
#line 1474 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    }
#line 1474 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    createAdoptingBuffer_chpl(call_tmp_chpl10, call_tmp_chpl12, &error_chpl, &ret_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 1474 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    call_tmp_chpl11 = ret_tmp_chpl3;
#line 1474 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    if (error_chpl != nil) /* ZLINE: 1474 /Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl */
#line 1474 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    {
#line 1474 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
      goto handler_chpl;
#line 1474 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    }
#line 1474 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    errorcode_msg_chpl = call_tmp_chpl11;
#line 1474 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    handler_chpl:;
#line 1473 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    if (error_chpl != nil) /* ZLINE: 1473 /Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl */
#line 1473 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    {
#line 1473 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
      chpl_uncaught_error(error_chpl, _ln_chpl, _fn_chpl);
#line 1473 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    }
#line 1476 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    chpl___PLUS__ASSIGN_(&generatedMsg_chpl, &errorcode_msg_chpl, _ln_chpl, _fn_chpl);
#line 1476 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    i_x_chpl2 = &errorcode_msg_chpl;
#line 1476 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    deinit_chpl95(i_x_chpl2, _ln_chpl, _fn_chpl);
#line 1476 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  }
#line 1480 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 1480 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  call_tmp_chpl16 = &((this_chpl31)->details);
#line 1480 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  if (! ((call_tmp_chpl16)->buffLen == INT64(0))) /* ZLINE: 1480 /Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl */
#line 1480 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  {
#line 1481 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 1481 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    call_tmp_chpl17 = &((this_chpl31)->details);
#line 1481 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    chpl___PLUS_(&_str_literal_734_chpl, call_tmp_chpl17, &ret_tmp_chpl4, _ln_chpl, _fn_chpl);
#line 1481 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    call_tmp_chpl18 = ret_tmp_chpl4;
#line 1481 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    chpl___PLUS_(&call_tmp_chpl18, &_str_literal_415_chpl, &ret_tmp_chpl5, _ln_chpl, _fn_chpl);
#line 1481 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    call_tmp_chpl19 = ret_tmp_chpl5;
#line 1481 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    chpl___PLUS__ASSIGN_(&generatedMsg_chpl, &call_tmp_chpl19, _ln_chpl, _fn_chpl);
#line 1481 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    i_x_chpl3 = &call_tmp_chpl19;
#line 1481 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    deinit_chpl95(i_x_chpl3, _ln_chpl, _fn_chpl);
#line 1481 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    i_x_chpl4 = &call_tmp_chpl18;
#line 1481 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    deinit_chpl95(i_x_chpl4, _ln_chpl, _fn_chpl);
#line 1481 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  }
#line 1483 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ret_chpl = generatedMsg_chpl;
#line 1462 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  *(_retArg_chpl) = ret_chpl;
#line 1462 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  return;
#line 1462 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
}

#line 1491 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
/* OS.chpl:1491 */
#line 1491 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
static void chpl__auto_destroy_BadFormatError(BadFormatError_chpl this_chpl31,
#line 1491 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                                              int64_t _ln_chpl,
#line 1491 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                                              int32_t _fn_chpl) {
#line 1492 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _ref_string_chpl _field_destructor_tmp__chpl = NULL;
#line 1491 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  Error_chpl _parent_destructor_tmp__chpl = NULL;
#line 1491 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _ref_string_chpl _field_destructor_tmp__chpl2 = NULL;
#line 1492 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 1492 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _field_destructor_tmp__chpl = &((this_chpl31)->details);
#line 1492 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  deinit_chpl95(_field_destructor_tmp__chpl, _ln_chpl, _fn_chpl);
#line 1491 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _parent_destructor_tmp__chpl = ((Error_chpl)(this_chpl31));
#line 1491 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(_parent_destructor_tmp__chpl, _ln_chpl, _fn_chpl);
#line 1491 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _field_destructor_tmp__chpl2 = &((_parent_destructor_tmp__chpl)->_msg);
#line 1491 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  deinit_chpl95(_field_destructor_tmp__chpl2, _ln_chpl, _fn_chpl);
#line 1491 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  return;
#line 1491 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
}

#line 1494 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
/* OS.chpl:1494 */
#line 1494 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
static BadFormatError_chpl _new_chpl85(string_chpl * details_chpl,
#line 1494 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                                       string_chpl * err_msg_chpl,
#line 1494 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                                       int64_t _ln_chpl,
#line 1494 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                                       int32_t _fn_chpl) {
#line 1494 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  BadFormatError_chpl initTemp_chpl = NULL;
#line 1494 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  c_ptr_void_chpl cast_tmp_chpl = NULL;
#line 1494 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  Error_chpl super_tmp_chpl = NULL;
#line 1494 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  Error_chpl init_coerce_tmp_chpl = NULL;
#line 1494 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  string_chpl _msg_chpl;
#line 1494 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  string_chpl details_chpl2;
#line 1494 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _ref_string_chpl call_tmp_chpl6 = NULL;
#line 1494 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  cast_tmp_chpl = chpl_here_alloc(sizeof(chpl_BadFormatError_chpl_object), INT16(10), _ln_chpl, _fn_chpl);
#line 1494 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  initTemp_chpl = ((BadFormatError_chpl)(cast_tmp_chpl));
#line 1494 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(initTemp_chpl))->chpl__cid = chpl__cid_BadFormatError_chpl;
#line 1494 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(initTemp_chpl, _ln_chpl, _fn_chpl);
#line 1494 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  super_tmp_chpl = &((initTemp_chpl)->super);
#line 1494 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 1494 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(&((super_tmp_chpl)->super)))->chpl__cid = chpl__cid_RootClass_chpl;
#line 1494 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(((RootClass_chpl)(super_tmp_chpl))))->chpl__cid = chpl__cid_RootClass_chpl;
#line 1494 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  init_coerce_tmp_chpl = ((Error_chpl)(nil));
#line 1494 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 1494 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  (super_tmp_chpl)->_next = init_coerce_tmp_chpl;
#line 1494 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 1494 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  (super_tmp_chpl)->thrownLine = INT64(0);
#line 1494 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 1494 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  (super_tmp_chpl)->thrownFileId = INT32(0);
#line 1494 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  init_chpl194(&_msg_chpl, _ln_chpl, _fn_chpl);
#line 1494 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 1494 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  (super_tmp_chpl)->_msg = _msg_chpl;
#line 1494 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 1494 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  (super_tmp_chpl)->_hasThrowInfo = UINT8(false);
#line 1494 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(super_tmp_chpl))->chpl__cid = chpl__cid_Error_chpl;
#line 1494 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(((Error_chpl)(initTemp_chpl))))->chpl__cid = chpl__cid_Error_chpl;
#line 1494 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  init_ASSIGN__chpl25(&details_chpl2, details_chpl, _ln_chpl, _fn_chpl);
#line 1494 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(initTemp_chpl, _ln_chpl, _fn_chpl);
#line 1494 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  (initTemp_chpl)->details = details_chpl2;
#line 1494 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(initTemp_chpl, _ln_chpl, _fn_chpl);
#line 1494 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  call_tmp_chpl6 = &(((Error_chpl)(initTemp_chpl))->_msg);
#line 1494 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl___ASSIGN_16(call_tmp_chpl6, err_msg_chpl, _ln_chpl, _fn_chpl);
#line 1494 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ((RootClass_chpl)(initTemp_chpl))->chpl__cid = chpl__cid_BadFormatError_chpl;
#line 1494 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  return initTemp_chpl;
#line 1494 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
}

#line 1499 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
/* OS.chpl:1499 */
#line 1499 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
static void message_chpl10(BadFormatError_chpl this_chpl31,
#line 1499 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                           string_chpl * _retArg_chpl,
#line 1499 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                           int64_t _ln_chpl,
#line 1499 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                           int32_t _fn_chpl) {
#line 1499 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  string_chpl ret_chpl;
#line 1500 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  string_chpl generatedMsg_chpl;
#line 1502 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  Error_chpl coerce_tmp_chpl3 = NULL;
#line 1502 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _ref_string_chpl call_tmp_chpl6 = NULL;
#line 1503 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  Error_chpl coerce_tmp_chpl4 = NULL;
#line 1503 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _ref_string_chpl call_tmp_chpl7 = NULL;
#line 1506 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  int32_t call_tmp_chpl8;
#line 1506 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  syserr call_tmp_chpl9;
#line 1508 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  c_ptrConst_int8_t_chpl call_tmp_chpl10 = NULL;
#line 1508 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  int32_t _formal_tmp_out_err_in_strerror_chpl;
#line 1509 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  string_chpl errorcode_msg_chpl;
#line 1510 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  Error_chpl error_chpl = NULL;
#line 1511 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  string_chpl call_tmp_chpl11;
#line 1511 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  int64_t call_tmp_chpl12;
#line 1511 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _owned_IllegalArgumentError_chpl2 call_tmp_chpl13;
#line 1511 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _owned_IllegalArgumentError_chpl2 ret_tmp_chpl;
#line 1511 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _ref__owned_IllegalArgumentError_chpl i_arg_chpl = NULL;
#line 1511 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  IllegalArgumentError_chpl call_tmp_chpl14 = NULL;
#line 1511 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  string_chpl call_tmp_chpl15;
#line 1511 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  Error_chpl coerce_tmp_chpl5 = NULL;
#line 1511 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  string_chpl ret_tmp_chpl2;
#line 1511 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 1511 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  string_chpl ret_tmp_chpl3;
#line 1513 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _ref_string_chpl i_x_chpl2 = NULL;
#line 1517 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _ref_string_chpl call_tmp_chpl16 = NULL;
#line 1518 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _ref_string_chpl call_tmp_chpl17 = NULL;
#line 1518 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  string_chpl call_tmp_chpl18;
#line 1518 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  string_chpl ret_tmp_chpl4;
#line 1518 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  string_chpl call_tmp_chpl19;
#line 1518 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  string_chpl ret_tmp_chpl5;
#line 1518 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _ref_string_chpl i_x_chpl3 = NULL;
#line 1518 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _ref_string_chpl i_x_chpl4 = NULL;
#line 1500 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  init_chpl194(&generatedMsg_chpl, _ln_chpl, _fn_chpl);
#line 1502 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  coerce_tmp_chpl3 = ((Error_chpl)(this_chpl31));
#line 1502 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 1502 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  call_tmp_chpl6 = &((coerce_tmp_chpl3)->_msg);
#line 1502 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  if (! ((call_tmp_chpl6)->buffLen == INT64(0))) /* ZLINE: 1502 /Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl */
#line 1502 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  {
#line 1503 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    coerce_tmp_chpl4 = ((Error_chpl)(this_chpl31));
#line 1503 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    chpl_check_nil(coerce_tmp_chpl4, _ln_chpl, _fn_chpl);
#line 1503 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    call_tmp_chpl7 = &((coerce_tmp_chpl4)->_msg);
#line 1503 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    chpl___PLUS__ASSIGN_(&generatedMsg_chpl, call_tmp_chpl7, _ln_chpl, _fn_chpl);
#line 1503 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  } else /* ZLINE: 1504 /Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl */
#line 1503 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  {
#line 1506 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    call_tmp_chpl8 = chpl_macro_int_EFORMAT();
#line 1506 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    call_tmp_chpl9 = qio_int_to_err(call_tmp_chpl8);
#line 1508 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    call_tmp_chpl10 = sys_strerror_syserr_str(call_tmp_chpl9, &_formal_tmp_out_err_in_strerror_chpl);
#line 1510 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    error_chpl = NULL;
#line 1511 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    call_tmp_chpl12 = string_length_bytes(call_tmp_chpl10);
#line 1511 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    chpl_checkValue2(call_tmp_chpl12, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 1511 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    call_tmp_chpl13 = ret_tmp_chpl;
#line 1511 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    i_arg_chpl = &call_tmp_chpl13;
#line 1511 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    call_tmp_chpl14 = borrow_chpl2(i_arg_chpl);
#line 1511 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    if (((RootClass_chpl)(call_tmp_chpl14)) != nil) /* ZLINE: 1511 /Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl */
#line 1511 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    {
#line 1511 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
      coerce_tmp_chpl5 = ((Error_chpl)(call_tmp_chpl14));
#line 1511 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
      chpl_check_nil(coerce_tmp_chpl5, _ln_chpl, _fn_chpl);
#line 1511 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
      ((void(*)(Error_chpl,
#line 1511 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
         string_chpl *,
#line 1511 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
         int64_t,
#line 1511 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
         int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(coerce_tmp_chpl5))->chpl__cid) + INT64(1))])(coerce_tmp_chpl5, &ret_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 1511 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
      call_tmp_chpl15 = ret_tmp_chpl2;
#line 1511 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
      safeCastCheckHalt_chpl(&call_tmp_chpl15, _ln_chpl, _fn_chpl);
#line 1511 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
      i_x_chpl = &call_tmp_chpl15;
#line 1511 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
      deinit_chpl95(i_x_chpl, _ln_chpl, _fn_chpl);
#line 1511 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    }
#line 1511 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    chpl__autoDestroy62(&call_tmp_chpl13, _ln_chpl, _fn_chpl);
#line 1511 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    if (error_chpl != nil) /* ZLINE: 1511 /Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl */
#line 1511 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    {
#line 1511 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
      goto handler_chpl;
#line 1511 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    }
#line 1511 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    createAdoptingBuffer_chpl(call_tmp_chpl10, call_tmp_chpl12, &error_chpl, &ret_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 1511 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    call_tmp_chpl11 = ret_tmp_chpl3;
#line 1511 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    if (error_chpl != nil) /* ZLINE: 1511 /Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl */
#line 1511 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    {
#line 1511 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
      goto handler_chpl;
#line 1511 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    }
#line 1511 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    errorcode_msg_chpl = call_tmp_chpl11;
#line 1511 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    handler_chpl:;
#line 1510 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    if (error_chpl != nil) /* ZLINE: 1510 /Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl */
#line 1510 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    {
#line 1510 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
      chpl_uncaught_error(error_chpl, _ln_chpl, _fn_chpl);
#line 1510 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    }
#line 1513 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    chpl___PLUS__ASSIGN_(&generatedMsg_chpl, &errorcode_msg_chpl, _ln_chpl, _fn_chpl);
#line 1513 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    i_x_chpl2 = &errorcode_msg_chpl;
#line 1513 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    deinit_chpl95(i_x_chpl2, _ln_chpl, _fn_chpl);
#line 1513 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  }
#line 1517 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 1517 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  call_tmp_chpl16 = &((this_chpl31)->details);
#line 1517 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  if (! ((call_tmp_chpl16)->buffLen == INT64(0))) /* ZLINE: 1517 /Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl */
#line 1517 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  {
#line 1518 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 1518 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    call_tmp_chpl17 = &((this_chpl31)->details);
#line 1518 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    chpl___PLUS_(&_str_literal_734_chpl, call_tmp_chpl17, &ret_tmp_chpl4, _ln_chpl, _fn_chpl);
#line 1518 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    call_tmp_chpl18 = ret_tmp_chpl4;
#line 1518 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    chpl___PLUS_(&call_tmp_chpl18, &_str_literal_415_chpl, &ret_tmp_chpl5, _ln_chpl, _fn_chpl);
#line 1518 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    call_tmp_chpl19 = ret_tmp_chpl5;
#line 1518 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    chpl___PLUS__ASSIGN_(&generatedMsg_chpl, &call_tmp_chpl19, _ln_chpl, _fn_chpl);
#line 1518 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    i_x_chpl3 = &call_tmp_chpl19;
#line 1518 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    deinit_chpl95(i_x_chpl3, _ln_chpl, _fn_chpl);
#line 1518 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    i_x_chpl4 = &call_tmp_chpl18;
#line 1518 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    deinit_chpl95(i_x_chpl4, _ln_chpl, _fn_chpl);
#line 1518 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  }
#line 1520 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ret_chpl = generatedMsg_chpl;
#line 1499 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  *(_retArg_chpl) = ret_chpl;
#line 1499 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  return;
#line 1499 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
}

#line 1552 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
/* OS.chpl:1552 */
#line 1552 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
static void quote_string_chpl(string_chpl * s_chpl,
#line 1552 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                              int64_t len_chpl,
#line 1552 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                              string_chpl * _retArg_chpl,
#line 1552 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                              int64_t _ln_chpl,
#line 1552 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                              int32_t _fn_chpl) {
#line 1552 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  string_chpl ret_chpl;
#line 1559 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  c_ptrConst_int8_t_chpl ret_chpl2 = NULL;
#line 1563 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  string_chpl call_tmp_chpl6;
#line 1563 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  string_chpl ret_chpl3;
#line 1563 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  string_chpl ret_chpl4;
#line 1561 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  syserr call_tmp_chpl7;
#line 1561 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 1567 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  Error_chpl error_chpl = NULL;
#line 1568 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  int32_t call_tmp_chpl8;
#line 1568 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  c_ptrConst_int8_t_chpl call_tmp_chpl9 = NULL;
#line 1568 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  string_chpl call_tmp_chpl10;
#line 1568 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  int64_t call_tmp_chpl11;
#line 1568 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _owned_IllegalArgumentError_chpl2 call_tmp_chpl12;
#line 1568 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _owned_IllegalArgumentError_chpl2 ret_tmp_chpl;
#line 1568 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _ref__owned_IllegalArgumentError_chpl i_arg_chpl = NULL;
#line 1568 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  IllegalArgumentError_chpl call_tmp_chpl13 = NULL;
#line 1568 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  string_chpl call_tmp_chpl14;
#line 1568 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  Error_chpl coerce_tmp_chpl3 = NULL;
#line 1568 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  string_chpl ret_tmp_chpl2;
#line 1568 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _ref_string_chpl i_x_chpl2 = NULL;
#line 1568 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  string_chpl ret_tmp_chpl3;
#line 1569 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  string_chpl call_tmp_chpl15;
#line 1569 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  int64_t call_tmp_chpl16;
#line 1569 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _owned_IllegalArgumentError_chpl2 call_tmp_chpl17;
#line 1569 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _owned_IllegalArgumentError_chpl2 ret_tmp_chpl4;
#line 1569 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _ref__owned_IllegalArgumentError_chpl i_arg_chpl2 = NULL;
#line 1569 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  IllegalArgumentError_chpl call_tmp_chpl18 = NULL;
#line 1569 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  string_chpl call_tmp_chpl19;
#line 1569 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  Error_chpl coerce_tmp_chpl4 = NULL;
#line 1569 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  string_chpl ret_tmp_chpl5;
#line 1569 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _ref_string_chpl i_x_chpl3 = NULL;
#line 1569 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  string_chpl ret_tmp_chpl6;
#line 1559 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ret_chpl2 = ((c_ptrConst_int8_t_chpl)(nil));
#line 1563 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  init_chpl194(&ret_chpl4, _ln_chpl, _fn_chpl);
#line 1563 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  (&ret_chpl4)->cachedNumCodepoints = (s_chpl)->cachedNumCodepoints;
#line 1563 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  initWithBorrowedBuffer_chpl(&ret_chpl4, s_chpl, _ln_chpl, _fn_chpl);
#line 1563 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ret_chpl3 = ret_chpl4;
#line 1563 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  call_tmp_chpl6 = ret_chpl3;
#line 1561 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  call_tmp_chpl7 = qio_quote_string(UINT8(34), UINT8(34), QIO_STRING_FORMAT_CHPL, ((c_ptrConst_int8_t_chpl)((&call_tmp_chpl6)->buff)), len_chpl, &ret_chpl2, nil);
#line 1561 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  i_x_chpl = &call_tmp_chpl6;
#line 1561 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  deinit_chpl95(i_x_chpl, _ln_chpl, _fn_chpl);
#line 1567 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  error_chpl = NULL;
#line 1568 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  call_tmp_chpl8 = qio_err_iserr(call_tmp_chpl7);
#line 1568 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  if (call_tmp_chpl8 != INT32(0)) /* ZLINE: 1568 /Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl */
#line 1568 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  {
#line 1568 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    call_tmp_chpl9 = qio_strdup("<error>");
#line 1568 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    call_tmp_chpl11 = string_length_bytes(call_tmp_chpl9);
#line 1568 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    chpl_checkValue2(call_tmp_chpl11, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 1568 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    call_tmp_chpl12 = ret_tmp_chpl;
#line 1568 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    i_arg_chpl = &call_tmp_chpl12;
#line 1568 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    call_tmp_chpl13 = borrow_chpl2(i_arg_chpl);
#line 1568 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    if (((RootClass_chpl)(call_tmp_chpl13)) != nil) /* ZLINE: 1568 /Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl */
#line 1568 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    {
#line 1568 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
      coerce_tmp_chpl3 = ((Error_chpl)(call_tmp_chpl13));
#line 1568 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
      chpl_check_nil(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 1568 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
      ((void(*)(Error_chpl,
#line 1568 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
         string_chpl *,
#line 1568 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
         int64_t,
#line 1568 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
         int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(coerce_tmp_chpl3))->chpl__cid) + INT64(1))])(coerce_tmp_chpl3, &ret_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 1568 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
      call_tmp_chpl14 = ret_tmp_chpl2;
#line 1568 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
      safeCastCheckHalt_chpl(&call_tmp_chpl14, _ln_chpl, _fn_chpl);
#line 1568 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
      i_x_chpl2 = &call_tmp_chpl14;
#line 1568 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
      deinit_chpl95(i_x_chpl2, _ln_chpl, _fn_chpl);
#line 1568 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    }
#line 1568 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    chpl__autoDestroy62(&call_tmp_chpl12, _ln_chpl, _fn_chpl);
#line 1568 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    if (error_chpl != nil) /* ZLINE: 1568 /Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl */
#line 1568 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    {
#line 1568 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
      goto handler_chpl;
#line 1568 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    }
#line 1568 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    createAdoptingBuffer_chpl(call_tmp_chpl9, call_tmp_chpl11, &error_chpl, &ret_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 1568 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    call_tmp_chpl10 = ret_tmp_chpl3;
#line 1568 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    if (error_chpl != nil) /* ZLINE: 1568 /Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl */
#line 1568 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    {
#line 1568 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
      goto handler_chpl;
#line 1568 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    }
#line 1568 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    ret_chpl = call_tmp_chpl10;
#line 1568 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    goto _end_quote_string_chpl;
#line 1568 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  }
#line 1569 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  call_tmp_chpl16 = string_length_bytes(ret_chpl2);
#line 1569 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl_checkValue2(call_tmp_chpl16, &ret_tmp_chpl4, _ln_chpl, _fn_chpl);
#line 1569 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  call_tmp_chpl17 = ret_tmp_chpl4;
#line 1569 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  i_arg_chpl2 = &call_tmp_chpl17;
#line 1569 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  call_tmp_chpl18 = borrow_chpl2(i_arg_chpl2);
#line 1569 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  if (((RootClass_chpl)(call_tmp_chpl18)) != nil) /* ZLINE: 1569 /Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl */
#line 1569 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  {
#line 1569 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    coerce_tmp_chpl4 = ((Error_chpl)(call_tmp_chpl18));
#line 1569 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    chpl_check_nil(coerce_tmp_chpl4, _ln_chpl, _fn_chpl);
#line 1569 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    ((void(*)(Error_chpl,
#line 1569 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
       string_chpl *,
#line 1569 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
       int64_t,
#line 1569 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
       int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(coerce_tmp_chpl4))->chpl__cid) + INT64(1))])(coerce_tmp_chpl4, &ret_tmp_chpl5, _ln_chpl, _fn_chpl);
#line 1569 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    call_tmp_chpl19 = ret_tmp_chpl5;
#line 1569 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    safeCastCheckHalt_chpl(&call_tmp_chpl19, _ln_chpl, _fn_chpl);
#line 1569 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    i_x_chpl3 = &call_tmp_chpl19;
#line 1569 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    deinit_chpl95(i_x_chpl3, _ln_chpl, _fn_chpl);
#line 1569 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  }
#line 1569 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl__autoDestroy62(&call_tmp_chpl17, _ln_chpl, _fn_chpl);
#line 1569 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  if (error_chpl != nil) /* ZLINE: 1569 /Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl */
#line 1569 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  {
#line 1569 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    goto handler_chpl;
#line 1569 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  }
#line 1569 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  createAdoptingBuffer_chpl(ret_chpl2, call_tmp_chpl16, &error_chpl, &ret_tmp_chpl6, _ln_chpl, _fn_chpl);
#line 1569 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  call_tmp_chpl15 = ret_tmp_chpl6;
#line 1569 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  if (error_chpl != nil) /* ZLINE: 1569 /Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl */
#line 1569 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  {
#line 1569 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    goto handler_chpl;
#line 1569 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  }
#line 1569 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  ret_chpl = call_tmp_chpl15;
#line 1569 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  goto _end_quote_string_chpl;
#line 1569 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  handler_chpl:;
#line 1567 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  if (error_chpl != nil) /* ZLINE: 1567 /Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl */
#line 1567 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  {
#line 1567 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    chpl_uncaught_error(error_chpl, _ln_chpl, _fn_chpl);
#line 1567 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  }
#line 1567 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _end_quote_string_chpl:;
#line 1552 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  *(_retArg_chpl) = ret_chpl;
#line 1552 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  return;
#line 1552 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
}

#line 1587 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
/* OS.chpl:1587 */
#line 1587 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
static void ioerror_chpl(syserr error_chpl,
#line 1587 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                         string_chpl * msg_chpl,
#line 1587 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                         string_chpl * path_chpl,
#line 1587 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                         int64_t offset_chpl,
#line 1587 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                         Error_chpl * error_out_chpl,
#line 1587 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                         int64_t _ln_chpl,
#line 1587 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                         int32_t _fn_chpl) {
#line 1589 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  int32_t call_tmp_chpl6;
#line 1590 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  int64_t ret_chpl;
#line 1590 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  string_chpl call_tmp_chpl7;
#line 1590 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  string_chpl ret_tmp_chpl;
#line 1590 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  string_chpl quotedpath_chpl;
#line 1591 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  string_chpl call_tmp_chpl8;
#line 1591 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  string_chpl ret_tmp_chpl2;
#line 1591 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  string_chpl call_tmp_chpl9;
#line 1591 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  string_chpl ret_tmp_chpl3;
#line 1591 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  string_chpl call_tmp_chpl10;
#line 1591 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  string_chpl ret_tmp_chpl4;
#line 1592 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  string_chpl call_tmp_chpl11;
#line 1592 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  string_chpl ret_tmp_chpl5;
#line 1591 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  string_chpl call_tmp_chpl12;
#line 1591 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  string_chpl ret_tmp_chpl6;
#line 1591 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  string_chpl details_chpl;
#line 1591 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 1591 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _ref_string_chpl i_x_chpl2 = NULL;
#line 1591 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _ref_string_chpl i_x_chpl3 = NULL;
#line 1591 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _ref_string_chpl i_x_chpl4 = NULL;
#line 1593 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _owned_Error_chpl call_tmp_chpl13;
#line 1593 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _owned_Error_chpl ret_tmp_chpl7;
#line 1593 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  Error_chpl call_tmp_chpl14 = NULL;
#line 1593 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _ref_string_chpl i_x_chpl5 = NULL;
#line 1593 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _ref_string_chpl i_x_chpl6 = NULL;
#line 1589 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  call_tmp_chpl6 = qio_err_iserr(error_chpl);
#line 1589 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  if (call_tmp_chpl6 != INT32(0)) /* ZLINE: 1589 /Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl */
#line 1589 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  {
#line 1590 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    ret_chpl = (path_chpl)->buffLen;
#line 1590 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    quote_string_chpl(path_chpl, ret_chpl, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 1590 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    call_tmp_chpl7 = ret_tmp_chpl;
#line 1590 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    quotedpath_chpl = call_tmp_chpl7;
#line 1591 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    chpl___PLUS_(msg_chpl, &_str_literal_2383_chpl, &ret_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 1591 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    call_tmp_chpl8 = ret_tmp_chpl2;
#line 1591 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    chpl___PLUS_(&call_tmp_chpl8, &quotedpath_chpl, &ret_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 1591 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    call_tmp_chpl9 = ret_tmp_chpl3;
#line 1591 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    chpl___PLUS_(&call_tmp_chpl9, &_str_literal_2384_chpl, &ret_tmp_chpl4, _ln_chpl, _fn_chpl);
#line 1591 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    call_tmp_chpl10 = ret_tmp_chpl4;
#line 1592 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    chpl___COLON_25(offset_chpl, &ret_tmp_chpl5, _ln_chpl, _fn_chpl);
#line 1592 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    call_tmp_chpl11 = ret_tmp_chpl5;
#line 1591 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    chpl___PLUS_(&call_tmp_chpl10, &call_tmp_chpl11, &ret_tmp_chpl6, _ln_chpl, _fn_chpl);
#line 1591 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    call_tmp_chpl12 = ret_tmp_chpl6;
#line 1591 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    details_chpl = call_tmp_chpl12;
#line 1591 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    i_x_chpl = &call_tmp_chpl11;
#line 1591 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    deinit_chpl95(i_x_chpl, _ln_chpl, _fn_chpl);
#line 1591 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    i_x_chpl2 = &call_tmp_chpl10;
#line 1591 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    deinit_chpl95(i_x_chpl2, _ln_chpl, _fn_chpl);
#line 1591 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    i_x_chpl3 = &call_tmp_chpl9;
#line 1591 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    deinit_chpl95(i_x_chpl3, _ln_chpl, _fn_chpl);
#line 1591 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    i_x_chpl4 = &call_tmp_chpl8;
#line 1591 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    deinit_chpl95(i_x_chpl4, _ln_chpl, _fn_chpl);
#line 1593 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    createSystemOrChplError_chpl(error_chpl, &details_chpl, &ret_tmp_chpl7, _ln_chpl, _fn_chpl);
#line 1593 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    call_tmp_chpl13 = ret_tmp_chpl7;
#line 1593 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    call_tmp_chpl14 = chpl_fix_thrown_error(&call_tmp_chpl13, _ln_chpl, _fn_chpl);
#line 1593 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    *(error_out_chpl) = call_tmp_chpl14;
#line 1593 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    i_x_chpl5 = &details_chpl;
#line 1593 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    deinit_chpl95(i_x_chpl5, _ln_chpl, _fn_chpl);
#line 1593 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    i_x_chpl6 = &quotedpath_chpl;
#line 1593 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    deinit_chpl95(i_x_chpl6, _ln_chpl, _fn_chpl);
#line 1593 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    goto _endioerror_chpl;
#line 1593 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  }
#line 1593 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _endioerror_chpl:;
#line 1587 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  return;
#line 1587 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
}

#line 1600 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
/* OS.chpl:1600 */
#line 1600 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
static void ioerror_chpl2(syserr error_chpl,
#line 1600 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                          string_chpl * msg_chpl,
#line 1600 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                          string_chpl * path_chpl,
#line 1600 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                          Error_chpl * error_out_chpl,
#line 1600 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                          int64_t _ln_chpl,
#line 1600 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                          int32_t _fn_chpl) {
#line 1602 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  int32_t call_tmp_chpl6;
#line 1603 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  int64_t ret_chpl;
#line 1603 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  string_chpl call_tmp_chpl7;
#line 1603 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  string_chpl ret_tmp_chpl;
#line 1603 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  string_chpl quotedpath_chpl;
#line 1604 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  string_chpl call_tmp_chpl8;
#line 1604 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  string_chpl ret_tmp_chpl2;
#line 1604 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  string_chpl call_tmp_chpl9;
#line 1604 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  string_chpl ret_tmp_chpl3;
#line 1604 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  string_chpl details_chpl;
#line 1604 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 1605 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _owned_Error_chpl call_tmp_chpl10;
#line 1605 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _owned_Error_chpl ret_tmp_chpl4;
#line 1605 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  Error_chpl call_tmp_chpl11 = NULL;
#line 1605 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _ref_string_chpl i_x_chpl2 = NULL;
#line 1605 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _ref_string_chpl i_x_chpl3 = NULL;
#line 1602 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  call_tmp_chpl6 = qio_err_iserr(error_chpl);
#line 1602 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  if (call_tmp_chpl6 != INT32(0)) /* ZLINE: 1602 /Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl */
#line 1602 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  {
#line 1603 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    ret_chpl = (path_chpl)->buffLen;
#line 1603 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    quote_string_chpl(path_chpl, ret_chpl, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 1603 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    call_tmp_chpl7 = ret_tmp_chpl;
#line 1603 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    quotedpath_chpl = call_tmp_chpl7;
#line 1604 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    chpl___PLUS_(msg_chpl, &_str_literal_2383_chpl, &ret_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 1604 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    call_tmp_chpl8 = ret_tmp_chpl2;
#line 1604 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    chpl___PLUS_(&call_tmp_chpl8, &quotedpath_chpl, &ret_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 1604 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    call_tmp_chpl9 = ret_tmp_chpl3;
#line 1604 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    details_chpl = call_tmp_chpl9;
#line 1604 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    i_x_chpl = &call_tmp_chpl8;
#line 1604 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    deinit_chpl95(i_x_chpl, _ln_chpl, _fn_chpl);
#line 1605 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    createSystemOrChplError_chpl(error_chpl, &details_chpl, &ret_tmp_chpl4, _ln_chpl, _fn_chpl);
#line 1605 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    call_tmp_chpl10 = ret_tmp_chpl4;
#line 1605 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    call_tmp_chpl11 = chpl_fix_thrown_error(&call_tmp_chpl10, _ln_chpl, _fn_chpl);
#line 1605 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    *(error_out_chpl) = call_tmp_chpl11;
#line 1605 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    i_x_chpl2 = &details_chpl;
#line 1605 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    deinit_chpl95(i_x_chpl2, _ln_chpl, _fn_chpl);
#line 1605 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    i_x_chpl3 = &quotedpath_chpl;
#line 1605 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    deinit_chpl95(i_x_chpl3, _ln_chpl, _fn_chpl);
#line 1605 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    goto _endioerror_chpl;
#line 1605 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  }
#line 1605 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _endioerror_chpl:;
#line 1600 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  return;
#line 1600 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
}

#line 1612 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
/* OS.chpl:1612 */
#line 1612 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
static void ioerror_chpl3(syserr error_chpl,
#line 1612 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                          string_chpl * msg_chpl,
#line 1612 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                          Error_chpl * error_out_chpl,
#line 1612 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                          int64_t _ln_chpl,
#line 1612 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                          int32_t _fn_chpl) {
#line 1614 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  int32_t call_tmp_chpl6;
#line 1614 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _owned_Error_chpl call_tmp_chpl7;
#line 1614 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _owned_Error_chpl ret_tmp_chpl;
#line 1614 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  Error_chpl call_tmp_chpl8 = NULL;
#line 1614 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  call_tmp_chpl6 = qio_err_iserr(error_chpl);
#line 1614 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  if (call_tmp_chpl6 != INT32(0)) /* ZLINE: 1614 /Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl */
#line 1614 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  {
#line 1614 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    createSystemOrChplError_chpl(error_chpl, msg_chpl, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 1614 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    call_tmp_chpl7 = ret_tmp_chpl;
#line 1614 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    call_tmp_chpl8 = chpl_fix_thrown_error(&call_tmp_chpl7, _ln_chpl, _fn_chpl);
#line 1614 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    *(error_out_chpl) = call_tmp_chpl8;
#line 1614 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    goto _endioerror_chpl;
#line 1614 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  }
#line 1614 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _endioerror_chpl:;
#line 1612 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  return;
#line 1612 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
}

