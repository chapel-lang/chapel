#line 34 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
/* Errors.chpl:34 */
#line 34 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
static void chpl__init_Errors(int64_t _ln_chpl,
#line 34 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                              int32_t _fn_chpl) {
#line 34 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_int32_t_chpl refIndentLevel_chpl = NULL;
#line 34 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  if (chpl__init_Errors_p) /* ZLINE: 34 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 34 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  {
#line 34 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    goto _exit_chpl__init_Errors_chpl;
#line 34 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  }
#line 34 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  printModuleInit_chpl("%*s\n", "Errors", INT64(6), _ln_chpl, _fn_chpl);
#line 34 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  refIndentLevel_chpl = &moduleInitLevel_chpl;
#line 34 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  *(refIndentLevel_chpl) += INT64(1);
#line 34 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl__init_Errors_p = UINT8(true);
#line 34 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  {
#line 34 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    chpl__init_ChapelLocks(_ln_chpl, _fn_chpl);
#line 34 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    chpl__init_CTypes(_ln_chpl, _fn_chpl);
#line 34 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  }
#line 34 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  *(refIndentLevel_chpl) -= INT64(1);
#line 34 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _exit_chpl__init_Errors_chpl:;
#line 34 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  return;
#line 34 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
}

#line 42 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
/* Errors.chpl:42 */
#line 42 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
static void chpl__auto_destroy_Error(Error_chpl this_chpl31,
#line 42 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                                     int64_t _ln_chpl,
#line 42 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                                     int32_t _fn_chpl) {
#line 53 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl _field_destructor_tmp__chpl = NULL;
#line 53 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 53 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _field_destructor_tmp__chpl = &((this_chpl31)->_msg);
#line 53 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  deinit_chpl95(_field_destructor_tmp__chpl, _ln_chpl, _fn_chpl);
#line 42 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  return;
#line 42 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
}

#line 64 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
/* Errors.chpl:64 */
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
static Error_chpl _new_chpl56(string_chpl * msg_chpl,
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                              int64_t _ln_chpl,
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                              int32_t _fn_chpl) {
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  Error_chpl initTemp_chpl = NULL;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  c_ptr_void_chpl cast_tmp_chpl = NULL;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  Error_chpl init_coerce_tmp_chpl = NULL;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl _msg_chpl;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  cast_tmp_chpl = chpl_here_alloc(sizeof(chpl_Error_chpl_object), INT16(234), _ln_chpl, _fn_chpl);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  initTemp_chpl = ((Error_chpl)(cast_tmp_chpl));
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  ((RootClass_chpl)(initTemp_chpl))->chpl__cid = chpl__cid_Error_chpl;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_check_nil(initTemp_chpl, _ln_chpl, _fn_chpl);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  ((RootClass_chpl)(&((initTemp_chpl)->super)))->chpl__cid = chpl__cid_RootClass_chpl;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  ((RootClass_chpl)(((RootClass_chpl)(initTemp_chpl))))->chpl__cid = chpl__cid_RootClass_chpl;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  init_coerce_tmp_chpl = ((Error_chpl)(nil));
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_check_nil(initTemp_chpl, _ln_chpl, _fn_chpl);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  (initTemp_chpl)->_next = init_coerce_tmp_chpl;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_check_nil(initTemp_chpl, _ln_chpl, _fn_chpl);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  (initTemp_chpl)->thrownLine = INT64(0);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_check_nil(initTemp_chpl, _ln_chpl, _fn_chpl);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  (initTemp_chpl)->thrownFileId = INT32(0);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  init_ASSIGN__chpl25(&_msg_chpl, msg_chpl, _ln_chpl, _fn_chpl);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_check_nil(initTemp_chpl, _ln_chpl, _fn_chpl);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  (initTemp_chpl)->_msg = _msg_chpl;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_check_nil(initTemp_chpl, _ln_chpl, _fn_chpl);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  (initTemp_chpl)->_hasThrowInfo = UINT8(false);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  ((RootClass_chpl)(initTemp_chpl))->chpl__cid = chpl__cid_Error_chpl;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  return initTemp_chpl;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
}

#line 73 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
/* Errors.chpl:73 */
#line 73 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
static void message_chpl(Error_chpl this_chpl31,
#line 73 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                         string_chpl * _retArg_chpl,
#line 73 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                         int64_t _ln_chpl,
#line 73 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                         int32_t _fn_chpl) {
#line 74 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl call_tmp_chpl6 = NULL;
#line 74 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_chpl;
#line 74 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl tmp_chpl;
#line 74 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 74 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  call_tmp_chpl6 = &((this_chpl31)->_msg);
#line 74 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  init_ASSIGN__chpl25(&tmp_chpl, call_tmp_chpl6, _ln_chpl, _fn_chpl);
#line 74 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  ret_chpl = tmp_chpl;
#line 73 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  *(_retArg_chpl) = ret_chpl;
#line 73 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  return;
#line 73 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
}

#line 82 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
/* Errors.chpl:82 */
#line 82 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
static void chpl__auto_destroy_NilThrownError(NilThrownError_chpl this_chpl31,
#line 82 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                                              int64_t _ln_chpl,
#line 82 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                                              int32_t _fn_chpl) {
#line 82 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  Error_chpl _parent_destructor_tmp__chpl = NULL;
#line 82 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl _field_destructor_tmp__chpl = NULL;
#line 82 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _parent_destructor_tmp__chpl = ((Error_chpl)(this_chpl31));
#line 82 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_check_nil(_parent_destructor_tmp__chpl, _ln_chpl, _fn_chpl);
#line 82 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _field_destructor_tmp__chpl = &((_parent_destructor_tmp__chpl)->_msg);
#line 82 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  deinit_chpl95(_field_destructor_tmp__chpl, _ln_chpl, _fn_chpl);
#line 82 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  return;
#line 82 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
}

#line 82 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
/* Errors.chpl:82 */
#line 82 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
static NilThrownError_chpl _new_chpl57(int64_t _ln_chpl,
#line 82 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                                       int32_t _fn_chpl) {
#line 82 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  NilThrownError_chpl initTemp_chpl = NULL;
#line 82 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  c_ptr_void_chpl cast_tmp_chpl = NULL;
#line 82 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  Error_chpl super_tmp_chpl = NULL;
#line 82 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  Error_chpl init_coerce_tmp_chpl = NULL;
#line 82 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl _msg_chpl;
#line 82 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  cast_tmp_chpl = chpl_here_alloc(sizeof(chpl_NilThrownError_chpl_object), INT16(2), _ln_chpl, _fn_chpl);
#line 82 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  initTemp_chpl = ((NilThrownError_chpl)(cast_tmp_chpl));
#line 82 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  ((RootClass_chpl)(initTemp_chpl))->chpl__cid = chpl__cid_NilThrownError_chpl;
#line 82 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_check_nil(initTemp_chpl, _ln_chpl, _fn_chpl);
#line 82 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  super_tmp_chpl = &((initTemp_chpl)->super);
#line 82 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 82 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  ((RootClass_chpl)(&((super_tmp_chpl)->super)))->chpl__cid = chpl__cid_RootClass_chpl;
#line 82 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  ((RootClass_chpl)(((RootClass_chpl)(super_tmp_chpl))))->chpl__cid = chpl__cid_RootClass_chpl;
#line 82 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  init_coerce_tmp_chpl = ((Error_chpl)(nil));
#line 82 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 82 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  (super_tmp_chpl)->_next = init_coerce_tmp_chpl;
#line 82 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 82 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  (super_tmp_chpl)->thrownLine = INT64(0);
#line 82 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 82 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  (super_tmp_chpl)->thrownFileId = INT32(0);
#line 82 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  init_chpl194(&_msg_chpl, _ln_chpl, _fn_chpl);
#line 82 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 82 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  (super_tmp_chpl)->_msg = _msg_chpl;
#line 82 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 82 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  (super_tmp_chpl)->_hasThrowInfo = UINT8(false);
#line 82 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  ((RootClass_chpl)(super_tmp_chpl))->chpl__cid = chpl__cid_Error_chpl;
#line 82 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  ((RootClass_chpl)(((Error_chpl)(initTemp_chpl))))->chpl__cid = chpl__cid_Error_chpl;
#line 82 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  ((RootClass_chpl)(initTemp_chpl))->chpl__cid = chpl__cid_NilThrownError_chpl;
#line 82 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  return initTemp_chpl;
#line 82 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
}

#line 84 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
/* Errors.chpl:84 */
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
static void message_chpl2(NilThrownError_chpl this_chpl31,
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                          string_chpl * _retArg_chpl,
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                          int64_t _ln_chpl,
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                          int32_t _fn_chpl) {
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl inlineImm_chpl;
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_chpl;
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl tmp_chpl;
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  inlineImm_chpl = _str_literal_144_chpl;
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  i_x_chpl = &inlineImm_chpl;
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  init_ASSIGN__chpl25(&tmp_chpl, i_x_chpl, _ln_chpl, _fn_chpl);
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  ret_chpl = tmp_chpl;
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  *(_retArg_chpl) = ret_chpl;
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  return;
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
}

#line 89 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
/* Errors.chpl:89 */
#line 89 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
static void chpl__auto_destroy_NilClassError(NilClassError_chpl this_chpl31,
#line 89 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                                             int64_t _ln_chpl,
#line 89 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                                             int32_t _fn_chpl) {
#line 89 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  Error_chpl _parent_destructor_tmp__chpl = NULL;
#line 89 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl _field_destructor_tmp__chpl = NULL;
#line 89 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _parent_destructor_tmp__chpl = ((Error_chpl)(this_chpl31));
#line 89 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_check_nil(_parent_destructor_tmp__chpl, _ln_chpl, _fn_chpl);
#line 89 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _field_destructor_tmp__chpl = &((_parent_destructor_tmp__chpl)->_msg);
#line 89 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  deinit_chpl95(_field_destructor_tmp__chpl, _ln_chpl, _fn_chpl);
#line 89 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  return;
#line 89 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
}

#line 91 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
/* Errors.chpl:91 */
#line 91 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
static void message_chpl3(NilClassError_chpl this_chpl31,
#line 91 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                          string_chpl * _retArg_chpl,
#line 91 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                          int64_t _ln_chpl,
#line 91 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                          int32_t _fn_chpl) {
#line 92 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl inlineImm_chpl;
#line 92 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 92 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_chpl;
#line 92 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl tmp_chpl;
#line 92 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  inlineImm_chpl = _str_literal_145_chpl;
#line 92 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  i_x_chpl = &inlineImm_chpl;
#line 92 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  init_ASSIGN__chpl25(&tmp_chpl, i_x_chpl, _ln_chpl, _fn_chpl);
#line 92 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  ret_chpl = tmp_chpl;
#line 91 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  *(_retArg_chpl) = ret_chpl;
#line 91 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  return;
#line 91 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
}

#line 109 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
/* Errors.chpl:109 */
#line 109 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
static void chpl__auto_destroy_DecodeError(DecodeError_chpl this_chpl31,
#line 109 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                                           int64_t _ln_chpl,
#line 109 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                                           int32_t _fn_chpl) {
#line 109 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  Error_chpl _parent_destructor_tmp__chpl = NULL;
#line 109 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl _field_destructor_tmp__chpl = NULL;
#line 109 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _parent_destructor_tmp__chpl = ((Error_chpl)(this_chpl31));
#line 109 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_check_nil(_parent_destructor_tmp__chpl, _ln_chpl, _fn_chpl);
#line 109 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _field_destructor_tmp__chpl = &((_parent_destructor_tmp__chpl)->_msg);
#line 109 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  deinit_chpl95(_field_destructor_tmp__chpl, _ln_chpl, _fn_chpl);
#line 109 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  return;
#line 109 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
}

#line 109 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
/* Errors.chpl:109 */
#line 109 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
static DecodeError_chpl _new_chpl58(int64_t _ln_chpl,
#line 109 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                                    int32_t _fn_chpl) {
#line 109 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  DecodeError_chpl initTemp_chpl = NULL;
#line 109 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  c_ptr_void_chpl cast_tmp_chpl = NULL;
#line 109 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  Error_chpl super_tmp_chpl = NULL;
#line 109 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  Error_chpl init_coerce_tmp_chpl = NULL;
#line 109 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl _msg_chpl;
#line 109 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  cast_tmp_chpl = chpl_here_alloc(sizeof(chpl_DecodeError_chpl_object), INT16(3), _ln_chpl, _fn_chpl);
#line 109 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  initTemp_chpl = ((DecodeError_chpl)(cast_tmp_chpl));
#line 109 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  ((RootClass_chpl)(initTemp_chpl))->chpl__cid = chpl__cid_DecodeError_chpl;
#line 109 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_check_nil(initTemp_chpl, _ln_chpl, _fn_chpl);
#line 109 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  super_tmp_chpl = &((initTemp_chpl)->super);
#line 109 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 109 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  ((RootClass_chpl)(&((super_tmp_chpl)->super)))->chpl__cid = chpl__cid_RootClass_chpl;
#line 109 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  ((RootClass_chpl)(((RootClass_chpl)(super_tmp_chpl))))->chpl__cid = chpl__cid_RootClass_chpl;
#line 109 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  init_coerce_tmp_chpl = ((Error_chpl)(nil));
#line 109 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 109 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  (super_tmp_chpl)->_next = init_coerce_tmp_chpl;
#line 109 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 109 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  (super_tmp_chpl)->thrownLine = INT64(0);
#line 109 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 109 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  (super_tmp_chpl)->thrownFileId = INT32(0);
#line 109 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  init_chpl194(&_msg_chpl, _ln_chpl, _fn_chpl);
#line 109 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 109 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  (super_tmp_chpl)->_msg = _msg_chpl;
#line 109 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 109 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  (super_tmp_chpl)->_hasThrowInfo = UINT8(false);
#line 109 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  ((RootClass_chpl)(super_tmp_chpl))->chpl__cid = chpl__cid_Error_chpl;
#line 109 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  ((RootClass_chpl)(((Error_chpl)(initTemp_chpl))))->chpl__cid = chpl__cid_Error_chpl;
#line 109 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  ((RootClass_chpl)(initTemp_chpl))->chpl__cid = chpl__cid_DecodeError_chpl;
#line 109 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  return initTemp_chpl;
#line 109 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
}

#line 112 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
/* Errors.chpl:112 */
#line 112 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
static void message_chpl4(DecodeError_chpl this_chpl31,
#line 112 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                          string_chpl * _retArg_chpl,
#line 112 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                          int64_t _ln_chpl,
#line 112 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                          int32_t _fn_chpl) {
#line 113 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl inlineImm_chpl;
#line 113 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 113 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_chpl;
#line 113 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl tmp_chpl;
#line 113 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  inlineImm_chpl = _str_literal_147_chpl;
#line 113 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  i_x_chpl = &inlineImm_chpl;
#line 113 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  init_ASSIGN__chpl25(&tmp_chpl, i_x_chpl, _ln_chpl, _fn_chpl);
#line 113 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  ret_chpl = tmp_chpl;
#line 112 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  *(_retArg_chpl) = ret_chpl;
#line 112 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  return;
#line 112 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
}

#line 117 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
/* Errors.chpl:117 */
#line 117 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
static void chpl__auto_destroy_IllegalArgumentError(IllegalArgumentError_chpl this_chpl31,
#line 117 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                                                    int64_t _ln_chpl,
#line 117 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                                                    int32_t _fn_chpl) {
#line 117 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  Error_chpl _parent_destructor_tmp__chpl = NULL;
#line 117 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl _field_destructor_tmp__chpl = NULL;
#line 117 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _parent_destructor_tmp__chpl = ((Error_chpl)(this_chpl31));
#line 117 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_check_nil(_parent_destructor_tmp__chpl, _ln_chpl, _fn_chpl);
#line 117 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _field_destructor_tmp__chpl = &((_parent_destructor_tmp__chpl)->_msg);
#line 117 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  deinit_chpl95(_field_destructor_tmp__chpl, _ln_chpl, _fn_chpl);
#line 117 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  return;
#line 117 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
}

#line 120 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
/* Errors.chpl:120 */
#line 120 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
static IllegalArgumentError_chpl _new_chpl59(string_chpl * msg_chpl,
#line 120 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                                             int64_t _ln_chpl,
#line 120 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                                             int32_t _fn_chpl) {
#line 120 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  IllegalArgumentError_chpl initTemp_chpl = NULL;
#line 120 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  c_ptr_void_chpl cast_tmp_chpl = NULL;
#line 120 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  Error_chpl super_tmp_chpl = NULL;
#line 120 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  Error_chpl init_coerce_tmp_chpl = NULL;
#line 120 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl _msg_chpl;
#line 120 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  cast_tmp_chpl = chpl_here_alloc(sizeof(chpl_IllegalArgumentError_chpl_object), INT16(1), _ln_chpl, _fn_chpl);
#line 120 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  initTemp_chpl = ((IllegalArgumentError_chpl)(cast_tmp_chpl));
#line 120 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  ((RootClass_chpl)(initTemp_chpl))->chpl__cid = chpl__cid_IllegalArgumentError_chpl;
#line 120 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_check_nil(initTemp_chpl, _ln_chpl, _fn_chpl);
#line 120 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  super_tmp_chpl = &((initTemp_chpl)->super);
#line 120 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 120 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  ((RootClass_chpl)(&((super_tmp_chpl)->super)))->chpl__cid = chpl__cid_RootClass_chpl;
#line 120 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  ((RootClass_chpl)(((RootClass_chpl)(super_tmp_chpl))))->chpl__cid = chpl__cid_RootClass_chpl;
#line 120 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  init_coerce_tmp_chpl = ((Error_chpl)(nil));
#line 120 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 120 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  (super_tmp_chpl)->_next = init_coerce_tmp_chpl;
#line 120 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 120 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  (super_tmp_chpl)->thrownLine = INT64(0);
#line 120 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 120 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  (super_tmp_chpl)->thrownFileId = INT32(0);
#line 120 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  init_ASSIGN__chpl25(&_msg_chpl, msg_chpl, _ln_chpl, _fn_chpl);
#line 120 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 120 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  (super_tmp_chpl)->_msg = _msg_chpl;
#line 120 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 120 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  (super_tmp_chpl)->_hasThrowInfo = UINT8(false);
#line 120 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  ((RootClass_chpl)(super_tmp_chpl))->chpl__cid = chpl__cid_Error_chpl;
#line 120 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  ((RootClass_chpl)(((Error_chpl)(initTemp_chpl))))->chpl__cid = chpl__cid_Error_chpl;
#line 120 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  ((RootClass_chpl)(initTemp_chpl))->chpl__cid = chpl__cid_IllegalArgumentError_chpl;
#line 120 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  return initTemp_chpl;
#line 120 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
}

#line 151 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
/* Errors.chpl:151 */
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
static void chpl__auto_destroy_CodepointSplitError(CodepointSplitError_chpl this_chpl31,
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                                                   int64_t _ln_chpl,
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                                                   int32_t _fn_chpl) {
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  Error_chpl _parent_destructor_tmp__chpl = NULL;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl _field_destructor_tmp__chpl = NULL;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _parent_destructor_tmp__chpl = ((Error_chpl)(this_chpl31));
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_check_nil(_parent_destructor_tmp__chpl, _ln_chpl, _fn_chpl);
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _field_destructor_tmp__chpl = &((_parent_destructor_tmp__chpl)->_msg);
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  deinit_chpl95(_field_destructor_tmp__chpl, _ln_chpl, _fn_chpl);
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  return;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
}

#line 152 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
/* Errors.chpl:152 */
#line 152 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
static CodepointSplitError_chpl _new_chpl60(string_chpl * info_chpl,
#line 152 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                                            int64_t _ln_chpl,
#line 152 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                                            int32_t _fn_chpl) {
#line 152 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  CodepointSplitError_chpl initTemp_chpl = NULL;
#line 152 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  c_ptr_void_chpl cast_tmp_chpl = NULL;
#line 152 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  Error_chpl super_tmp_chpl = NULL;
#line 152 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  Error_chpl init_coerce_tmp_chpl = NULL;
#line 152 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl _msg_chpl;
#line 152 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  cast_tmp_chpl = chpl_here_alloc(sizeof(chpl_CodepointSplitError_chpl_object), INT16(36), _ln_chpl, _fn_chpl);
#line 152 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  initTemp_chpl = ((CodepointSplitError_chpl)(cast_tmp_chpl));
#line 152 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  ((RootClass_chpl)(initTemp_chpl))->chpl__cid = chpl__cid_CodepointSplitError_chpl;
#line 152 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_check_nil(initTemp_chpl, _ln_chpl, _fn_chpl);
#line 152 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  super_tmp_chpl = &((initTemp_chpl)->super);
#line 152 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 152 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  ((RootClass_chpl)(&((super_tmp_chpl)->super)))->chpl__cid = chpl__cid_RootClass_chpl;
#line 152 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  ((RootClass_chpl)(((RootClass_chpl)(super_tmp_chpl))))->chpl__cid = chpl__cid_RootClass_chpl;
#line 152 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  init_coerce_tmp_chpl = ((Error_chpl)(nil));
#line 152 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 152 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  (super_tmp_chpl)->_next = init_coerce_tmp_chpl;
#line 152 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 152 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  (super_tmp_chpl)->thrownLine = INT64(0);
#line 152 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 152 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  (super_tmp_chpl)->thrownFileId = INT32(0);
#line 152 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  init_ASSIGN__chpl25(&_msg_chpl, info_chpl, _ln_chpl, _fn_chpl);
#line 152 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 152 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  (super_tmp_chpl)->_msg = _msg_chpl;
#line 152 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 152 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  (super_tmp_chpl)->_hasThrowInfo = UINT8(false);
#line 152 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  ((RootClass_chpl)(super_tmp_chpl))->chpl__cid = chpl__cid_Error_chpl;
#line 152 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  ((RootClass_chpl)(((Error_chpl)(initTemp_chpl))))->chpl__cid = chpl__cid_Error_chpl;
#line 152 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  ((RootClass_chpl)(initTemp_chpl))->chpl__cid = chpl__cid_CodepointSplitError_chpl;
#line 152 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  return initTemp_chpl;
#line 152 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
}

#line 156 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
/* Errors.chpl:156 */
#line 156 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
static void message_chpl5(CodepointSplitError_chpl this_chpl31,
#line 156 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                          string_chpl * _retArg_chpl,
#line 156 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                          int64_t _ln_chpl,
#line 156 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                          int32_t _fn_chpl) {
#line 157 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  Error_chpl coerce_tmp_chpl3 = NULL;
#line 157 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl call_tmp_chpl6 = NULL;
#line 157 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl7;
#line 157 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl;
#line 157 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  coerce_tmp_chpl3 = ((Error_chpl)(this_chpl31));
#line 157 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_check_nil(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 157 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  call_tmp_chpl6 = &((coerce_tmp_chpl3)->_msg);
#line 157 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl___PLUS_(&_str_literal_153_chpl, call_tmp_chpl6, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 157 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  call_tmp_chpl7 = ret_tmp_chpl;
#line 156 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  *(_retArg_chpl) = call_tmp_chpl7;
#line 156 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  return;
#line 156 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
}

#line 178 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
/* Errors.chpl:178 */
#line 178 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
static void init_chpl195(chpl_TaskErrors * this_chpl31,
#line 178 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                         Error_chpl _head_chpl,
#line 178 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                         chpl_LocalSpinlock * _errorsLock_chpl) {
#line 180 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_LocalSpinlock _formal_tmp__errorsLock_chpl;
#line 180 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _formal_tmp__errorsLock_chpl = *(_errorsLock_chpl);
#line 179 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  (this_chpl31)->_head = _head_chpl;
#line 180 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  (this_chpl31)->_errorsLock = _formal_tmp__errorsLock_chpl;
#line 178 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  return;
#line 178 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
}

#line 182 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
/* Errors.chpl:182 */
#line 182 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
static void append_chpl(chpl_TaskErrors * this_chpl31,
#line 182 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                        Error_chpl err_chpl8,
#line 182 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                        int64_t _ln_chpl,
#line 182 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                        int32_t _fn_chpl) {
#line 184 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_chpl_LocalSpinlock_chpl call_tmp_chpl6 = NULL;
#line 184 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_bool tmp_chpl;
#line 184 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_AtomicBool_chpl call_tmp_chpl7 = NULL;
#line 184 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_atomic_bool_chpl call_tmp_chpl8 = NULL;
#line 184 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  memory_order ret_chpl;
#line 184 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_bool call_tmp_chpl9;
#line 184 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_bool tmp_chpl2;
#line 184 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_AtomicBool_chpl call_tmp_chpl10 = NULL;
#line 184 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_atomic_bool_chpl call_tmp_chpl11 = NULL;
#line 184 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  memory_order ret_chpl2;
#line 184 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_bool call_tmp_chpl12;
#line 184 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_AtomicBool_chpl call_tmp_chpl13 = NULL;
#line 184 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_atomic_bool_chpl call_tmp_chpl14 = NULL;
#line 184 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  memory_order ret_chpl3;
#line 184 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_bool call_tmp_chpl15;
#line 184 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_bool tmp_chpl3;
#line 184 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_AtomicBool_chpl call_tmp_chpl16 = NULL;
#line 184 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_atomic_bool_chpl call_tmp_chpl17 = NULL;
#line 184 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  memory_order ret_chpl4;
#line 184 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_bool call_tmp_chpl18;
#line 185 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  Error_chpl coerce_tmp_chpl3 = NULL;
#line 186 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  Error_chpl coerce_tmp_chpl4 = NULL;
#line 188 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_chpl_LocalSpinlock_chpl call_tmp_chpl19 = NULL;
#line 188 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_AtomicBool_chpl call_tmp_chpl20 = NULL;
#line 188 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_atomic_bool_chpl call_tmp_chpl21 = NULL;
#line 188 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  memory_order ret_chpl5;
#line 184 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  call_tmp_chpl6 = &((this_chpl31)->_errorsLock);
#line 184 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  call_tmp_chpl7 = &((call_tmp_chpl6)->l);
#line 184 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_rmem_consist_maybe_release2(_ln_chpl, _fn_chpl);
#line 184 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  call_tmp_chpl8 = &((call_tmp_chpl7)->_v);
#line 184 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  ret_chpl = memory_order_seq_cst;
#line 184 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  call_tmp_chpl9 = atomic_load_explicit_bool(call_tmp_chpl8, ret_chpl);
#line 184 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  if (call_tmp_chpl9) /* ZLINE: 184 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 184 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  {
#line 184 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    tmp_chpl2 = UINT8(true);
#line 184 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  } else /* ZLINE: 184 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 184 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  {
#line 184 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    call_tmp_chpl10 = &((call_tmp_chpl6)->l);
#line 184 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    chpl_rmem_consist_maybe_release5(_ln_chpl, _fn_chpl);
#line 184 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    chpl_rmem_consist_maybe_release5(_ln_chpl, _fn_chpl);
#line 184 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    call_tmp_chpl11 = &((call_tmp_chpl10)->_v);
#line 184 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    ret_chpl2 = memory_order_acquire;
#line 184 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    call_tmp_chpl12 = atomic_exchange_explicit_bool(call_tmp_chpl11, UINT8(true), ret_chpl2);
#line 184 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    chpl_rmem_consist_maybe_acquire5(_ln_chpl, _fn_chpl);
#line 184 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    tmp_chpl2 = call_tmp_chpl12;
#line 184 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  }
#line 184 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  tmp_chpl = tmp_chpl2;
#line 184 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  while (tmp_chpl) {
#line 184 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    chpl_task_yield();
#line 184 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    call_tmp_chpl13 = &((call_tmp_chpl6)->l);
#line 184 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    chpl_rmem_consist_maybe_release2(_ln_chpl, _fn_chpl);
#line 184 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    call_tmp_chpl14 = &((call_tmp_chpl13)->_v);
#line 184 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    ret_chpl3 = memory_order_seq_cst;
#line 184 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    call_tmp_chpl15 = atomic_load_explicit_bool(call_tmp_chpl14, ret_chpl3);
#line 184 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    if (call_tmp_chpl15) /* ZLINE: 184 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 184 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    {
#line 184 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      tmp_chpl3 = UINT8(true);
#line 184 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    } else /* ZLINE: 184 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 184 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    {
#line 184 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl16 = &((call_tmp_chpl6)->l);
#line 184 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl_rmem_consist_maybe_release5(_ln_chpl, _fn_chpl);
#line 184 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl_rmem_consist_maybe_release5(_ln_chpl, _fn_chpl);
#line 184 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl17 = &((call_tmp_chpl16)->_v);
#line 184 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      ret_chpl4 = memory_order_acquire;
#line 184 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl18 = atomic_exchange_explicit_bool(call_tmp_chpl17, UINT8(true), ret_chpl4);
#line 184 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl_rmem_consist_maybe_acquire5(_ln_chpl, _fn_chpl);
#line 184 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      tmp_chpl3 = call_tmp_chpl18;
#line 184 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    }
#line 184 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    tmp_chpl = tmp_chpl3;
#line 184 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  }
#line 185 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  coerce_tmp_chpl3 = (this_chpl31)->_head;
#line 186 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  coerce_tmp_chpl4 = err_chpl8;
#line 186 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_check_nil(coerce_tmp_chpl4, _ln_chpl, _fn_chpl);
#line 186 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  (coerce_tmp_chpl4)->_next = coerce_tmp_chpl3;
#line 187 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  (this_chpl31)->_head = err_chpl8;
#line 188 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  call_tmp_chpl19 = &((this_chpl31)->_errorsLock);
#line 188 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  call_tmp_chpl20 = &((call_tmp_chpl19)->l);
#line 188 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_rmem_consist_maybe_release4(_ln_chpl, _fn_chpl);
#line 188 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_rmem_consist_maybe_release4(_ln_chpl, _fn_chpl);
#line 188 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  call_tmp_chpl21 = &((call_tmp_chpl20)->_v);
#line 188 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  ret_chpl5 = memory_order_release;
#line 188 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  atomic_store_explicit_bool(call_tmp_chpl21, UINT8(false), ret_chpl5);
#line 188 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_rmem_consist_maybe_acquire4(_ln_chpl, _fn_chpl);
#line 188 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_rmem_consist_maybe_acquire4(_ln_chpl, _fn_chpl);
#line 182 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  return;
#line 182 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
}

#line 192 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
/* Errors.chpl:192 */
#line 192 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
static chpl_bool empty_chpl(chpl_TaskErrors * this_chpl31,
#line 192 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                            int64_t _ln_chpl,
#line 192 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                            int32_t _fn_chpl) {
#line 193 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_bool call_tmp_chpl6;
#line 193 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  call_tmp_chpl6 = (((RootClass_chpl)((this_chpl31)->_head)) == nil);
#line 192 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  return call_tmp_chpl6;
#line 192 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
}

#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
/* Errors.chpl:225 */
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
static TaskErrors_chpl _new_chpl61(chpl_TaskErrors * group_chpl,
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                                   int64_t _ln_chpl,
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                                   int32_t _fn_chpl) {
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  TaskErrors_chpl initTemp_chpl = NULL;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  c_ptr_void_chpl cast_tmp_chpl = NULL;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  Error_chpl super_tmp_chpl = NULL;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  Error_chpl init_coerce_tmp_chpl = NULL;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl _msg_chpl;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  Error_chpl head_chpl = NULL;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  Error_chpl coerce_tmp_chpl3 = NULL;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  c_ptr__owned_Error_chpl errorsArray_chpl = NULL;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  Error_chpl cur_chpl = NULL;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int64_t n_chpl;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_bool tmp_chpl;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  Error_chpl coerce_tmp_chpl4 = NULL;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  Error_chpl curnext_chpl = NULL;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  TaskErrors_chpl _formal_type_tmp__chpl = NULL;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  TaskErrors_chpl ret_chpl = NULL;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_int64_t_chpl i_lhs_chpl = NULL;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  TaskErrors_chpl _ic__F0_this_chpl = NULL;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  TaskErrors_chpl this_chpl31 = NULL;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int64_t coerce_tmp_chpl5;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int64_t tmp_chpl2;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int64_t _ic__F1_high_chpl;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int64_t i_chpl;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  TaskErrors_chpl this_chpl32 = NULL;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  c_ptr__owned_Error_chpl coerce_tmp_chpl6 = NULL;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref__owned_Error_chpl call_tmp_chpl6 = NULL;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  Error_chpl coerce_tmp_chpl7 = NULL;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  TaskErrors_chpl this_chpl33 = NULL;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  c_ptr__owned_Error_chpl coerce_tmp_chpl8 = NULL;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref__owned_Error_chpl call_tmp_chpl7 = NULL;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  Error_chpl coerce_tmp_chpl9 = NULL;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_int64_t_chpl i_lhs_chpl2 = NULL;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  uint64_t call_tmp_chpl8;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  uint64_t call_tmp_chpl9;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  uint64_t call_tmp_chpl10;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_bool tmp_chpl3;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  c_ptr_void_chpl ptr_chpl = NULL;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  c_ptr_void_chpl init_coerce_tmp_chpl2 = NULL;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int16_t call_tmp_chpl11;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  c_ptr_void_chpl call_tmp_chpl12 = NULL;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  uint64_t call_tmp_chpl13;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int64_t call_tmp_chpl14;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int64_t call_tmp_chpl15;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int64_t p_chpl;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl16;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl17;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl2;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl18;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl3;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl19;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl4;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl20;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl5;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl21;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl6;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl22;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl7;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl23;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl8;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl24;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl9;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl msg_chpl;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl2 = NULL;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl3 = NULL;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl4 = NULL;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl5 = NULL;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl6 = NULL;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl7 = NULL;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl8 = NULL;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl9 = NULL;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl25;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl10;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl26;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl11;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl27;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl12;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl28;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl13;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl29;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl14;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl30;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl15;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl31;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl16;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl32;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl17;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl33;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl18;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl34;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl19;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl msg_chpl2;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl10 = NULL;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl11 = NULL;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl12 = NULL;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl13 = NULL;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl14 = NULL;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl15 = NULL;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl16 = NULL;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl17 = NULL;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl18 = NULL;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl19 = NULL;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_int64_t_chpl i_lhs_chpl3 = NULL;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl35;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl20;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl36;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl21;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl37;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl22;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl38;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl23;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl39;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl24;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl40;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl25;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl41;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl26;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl42;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl27;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl43;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl28;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl msg_chpl3;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl20 = NULL;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl21 = NULL;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl22 = NULL;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl23 = NULL;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl24 = NULL;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl25 = NULL;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl26 = NULL;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl27 = NULL;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl28 = NULL;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl44;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl29;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl45;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl30;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl46;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl31;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl47;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl32;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl48;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl33;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl49;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl34;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl50;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl35;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl51;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl36;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl52;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl37;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl53;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl38;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl msg_chpl4;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl29 = NULL;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl30 = NULL;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl31 = NULL;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl32 = NULL;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl33 = NULL;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl34 = NULL;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl35 = NULL;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl36 = NULL;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl37 = NULL;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl38 = NULL;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl54;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl39;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl55;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl40;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl56;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl41;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl57;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl42;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl58;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl43;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl59;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl44;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl60;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl45;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl61;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl46;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl62;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl47;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl msg_chpl5;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl39 = NULL;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl40 = NULL;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl41 = NULL;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl42 = NULL;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl43 = NULL;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl44 = NULL;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl45 = NULL;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl46 = NULL;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl47 = NULL;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl63;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl48;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl64;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl49;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl65;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl50;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl66;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl51;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl67;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl52;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl68;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl53;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl69;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl54;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl70;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl55;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl71;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl56;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl72;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl57;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl msg_chpl6;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl48 = NULL;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl49 = NULL;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl50 = NULL;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl51 = NULL;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl52 = NULL;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl53 = NULL;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl54 = NULL;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl55 = NULL;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl56 = NULL;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl57 = NULL;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  uint64_t call_tmp_chpl73;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int16_t call_tmp_chpl74;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  c_ptr_void_chpl call_tmp_chpl75 = NULL;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int64_t inlineImm_chpl;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _owned_IllegalArgumentError_chpl2 call_tmp_chpl76;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _owned_IllegalArgumentError_chpl2 ret_tmp_chpl58;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref__owned_IllegalArgumentError_chpl i_arg_chpl = NULL;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  IllegalArgumentError_chpl call_tmp_chpl77 = NULL;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl78;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  Error_chpl coerce_tmp_chpl10 = NULL;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl59;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl58 = NULL;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  c_ptr__owned_Error_chpl call_tmp_chpl79 = NULL;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int64_t idx_chpl;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_bool tmp_chpl4;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  Error_chpl coerce_tmp_chpl11 = NULL;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  Error_chpl curnext_chpl2 = NULL;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  Error_chpl coerce_tmp_chpl12 = NULL;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  Error_chpl tmp_chpl5 = NULL;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  TaskErrors_chpl _formal_type_tmp__chpl2 = NULL;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  TaskErrors_chpl ret_chpl2 = NULL;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  TaskErrors_chpl asTaskErr_chpl = NULL;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  c_ptr__owned_Error_chpl coerce_tmp_chpl13 = NULL;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref__owned_Error_chpl call_tmp_chpl80 = NULL;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _owned_Error_chpl call_tmp_chpl81;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _owned_Error_chpl ret_tmp_chpl60;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref__owned_Error_chpl2 i__retArg_chpl = NULL;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _owned_Error_chpl ret_chpl3;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _owned_Error_chpl call_tmp_chpl82;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _owned_Error_chpl initTemp_chpl2;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_int64_t_chpl i_lhs_chpl4 = NULL;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  TaskErrors_chpl _ic__F0_this_chpl2 = NULL;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  TaskErrors_chpl this_chpl34 = NULL;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int64_t coerce_tmp_chpl14;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int64_t inlineImm_chpl2;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int64_t tmp_chpl6;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int64_t _ic__F1_high_chpl2;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int64_t i_chpl2;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  TaskErrors_chpl this_chpl35 = NULL;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  c_ptr__owned_Error_chpl coerce_tmp_chpl15 = NULL;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref__owned_Error_chpl call_tmp_chpl83 = NULL;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  Error_chpl coerce_tmp_chpl16 = NULL;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  TaskErrors_chpl this_chpl36 = NULL;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  c_ptr__owned_Error_chpl coerce_tmp_chpl17 = NULL;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref__owned_Error_chpl call_tmp_chpl84 = NULL;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  Error_chpl coerce_tmp_chpl18 = NULL;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  c_ptr__owned_Error_chpl coerce_tmp_chpl19 = NULL;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref__owned_Error_chpl call_tmp_chpl85 = NULL;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_int64_t_chpl i_lhs_chpl5 = NULL;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int32_t chpl_macro_tmp_11;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int32_t chpl_macro_tmp_12;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  cast_tmp_chpl = chpl_here_alloc(sizeof(chpl_TaskErrors_chpl_object), INT16(43), _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  initTemp_chpl = ((TaskErrors_chpl)(cast_tmp_chpl));
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  ((RootClass_chpl)(initTemp_chpl))->chpl__cid = chpl__cid_TaskErrors_chpl;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_check_nil(initTemp_chpl, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  super_tmp_chpl = &((initTemp_chpl)->super);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  ((RootClass_chpl)(&((super_tmp_chpl)->super)))->chpl__cid = chpl__cid_RootClass_chpl;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  ((RootClass_chpl)(((RootClass_chpl)(super_tmp_chpl))))->chpl__cid = chpl__cid_RootClass_chpl;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  init_coerce_tmp_chpl = ((Error_chpl)(nil));
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  (super_tmp_chpl)->_next = init_coerce_tmp_chpl;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  (super_tmp_chpl)->thrownLine = INT64(0);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  (super_tmp_chpl)->thrownFileId = INT32(0);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  init_chpl194(&_msg_chpl, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  (super_tmp_chpl)->_msg = _msg_chpl;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  (super_tmp_chpl)->_hasThrowInfo = UINT8(false);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  ((RootClass_chpl)(super_tmp_chpl))->chpl__cid = chpl__cid_Error_chpl;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  ((RootClass_chpl)(((Error_chpl)(initTemp_chpl))))->chpl__cid = chpl__cid_Error_chpl;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  coerce_tmp_chpl3 = (group_chpl)->_head;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  head_chpl = coerce_tmp_chpl3;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  (group_chpl)->_head = ((Error_chpl)(nil));
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_check_nil(initTemp_chpl, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  (initTemp_chpl)->nErrors = INT64(0);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  errorsArray_chpl = ((c_ptr__owned_Error_chpl)(nil));
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_check_nil(initTemp_chpl, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  (initTemp_chpl)->errorsArray = errorsArray_chpl;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  ((RootClass_chpl)(initTemp_chpl))->chpl__cid = chpl__cid_TaskErrors_chpl;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  n_chpl = INT64(0);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  cur_chpl = coerce_tmp_chpl3;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  tmp_chpl = (((RootClass_chpl)(coerce_tmp_chpl3)) != nil);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  while (tmp_chpl) {
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    if (((RootClass_chpl)(cur_chpl)) == nil) /* ZLINE: 225 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    {
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      nilCheckHalt_chpl(&_str_literal_52_chpl, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    }
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    coerce_tmp_chpl4 = cur_chpl;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    chpl_check_nil(coerce_tmp_chpl4, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    curnext_chpl = (coerce_tmp_chpl4)->_next;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    if (((RootClass_chpl)(cur_chpl)) == nil) /* ZLINE: 225 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    {
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      ret_chpl = ((TaskErrors_chpl)(nil));
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      goto _end__COLON__chpl;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    }
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    chpl_macro_tmp_11 = ((RootClass_chpl)(cur_chpl))->chpl__cid;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    ret_chpl = ( (((chpl__cid_TaskErrors_chpl <= chpl_macro_tmp_11) && (chpl_macro_tmp_11 <= chpl_subclass_max_id[chpl__cid_TaskErrors_chpl])))?(((TaskErrors_chpl)(cur_chpl))):(((TaskErrors_chpl)(NULL))) );
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    _end__COLON__chpl:;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    if (((RootClass_chpl)(ret_chpl)) == nil) /* ZLINE: 225 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    {
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      i_lhs_chpl = &n_chpl;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      *(i_lhs_chpl) += INT64(1);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    } else /* ZLINE: 225 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    {
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      _ic__F0_this_chpl = ret_chpl;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      this_chpl31 = ret_chpl;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      coerce_tmp_chpl5 = (this_chpl31)->nErrors;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      if (coerce_tmp_chpl5 < INT64(0)) /* ZLINE: 225 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      {
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        boundsCheckHalt_chpl(&_str_literal_580_chpl, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      }
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      if (coerce_tmp_chpl5 == INT64(0)) /* ZLINE: 225 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      {
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        tmp_chpl2 = ((int64_t)(((uint64_t)((((uint64_t)(INT64(0))) - UINT64(1))))));
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      } else /* ZLINE: 225 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      {
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        tmp_chpl2 = ((int64_t)((INT64(0) + ((int64_t)((coerce_tmp_chpl5 - INT64(1)))))));
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      }
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      _ic__F1_high_chpl = tmp_chpl2;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      i_chpl = INT64(0);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl_checkIfRangeIterWillOverflow(INT64(0), tmp_chpl2, INT64(1), INT64(0), tmp_chpl2, UINT8(true), _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      this_chpl32 = _ic__F0_this_chpl;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl_check_nil(this_chpl32, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      coerce_tmp_chpl6 = (this_chpl32)->errorsArray;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      for (i_chpl = INT64(0); ((i_chpl <= _ic__F1_high_chpl)); i_chpl += INT64(1)) {
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        call_tmp_chpl6 = (coerce_tmp_chpl6 + i_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        coerce_tmp_chpl7 = borrow_chpl3(call_tmp_chpl6);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        if (((RootClass_chpl)(coerce_tmp_chpl7)) != nil) /* ZLINE: 225 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        {
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          this_chpl33 = _ic__F0_this_chpl;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          chpl_check_nil(this_chpl33, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          coerce_tmp_chpl8 = (this_chpl33)->errorsArray;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          call_tmp_chpl7 = (coerce_tmp_chpl8 + i_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          coerce_tmp_chpl9 = borrow_chpl3(call_tmp_chpl7);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          if (((RootClass_chpl)(coerce_tmp_chpl9)) != nil) /* ZLINE: 225 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          {
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            i_lhs_chpl2 = &n_chpl;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            *(i_lhs_chpl2) += INT64(1);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          }
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        }
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      }
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    }
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    cur_chpl = curnext_chpl;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    tmp_chpl = (((RootClass_chpl)(curnext_chpl)) != nil);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  }
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_check_nil(initTemp_chpl, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  (initTemp_chpl)->nErrors = n_chpl;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  call_tmp_chpl8 = ((uint64_t)(n_chpl));
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  call_tmp_chpl9 = sizeof(_owned_Error_chpl2);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  call_tmp_chpl10 = (call_tmp_chpl8 * call_tmp_chpl9);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  if (INT64(0) < INT64(0)) /* ZLINE: 225 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  {
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    tmp_chpl3 = UINT8(true);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  } else /* ZLINE: 225 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  {
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    tmp_chpl3 = (UINT64(0) != ((uint64_t)(INT64(0))));
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  }
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  init_coerce_tmp_chpl2 = ((c_ptr_void_chpl)(nil));
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  ptr_chpl = init_coerce_tmp_chpl2;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  if (! tmp_chpl3) /* ZLINE: 225 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  {
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    {
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl11 = offset_ARRAY_ELEMENTS_chpl();
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl12 = chpl_here_calloc(call_tmp_chpl10, INT64(1), call_tmp_chpl11, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      ptr_chpl = call_tmp_chpl12;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    }
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  } else /* ZLINE: 225 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  {
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    call_tmp_chpl13 = chpl_bitops_clz_64(UINT64(0));
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    call_tmp_chpl14 = (INT64(63) - ((int64_t)(call_tmp_chpl13)));
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    if (INT64(1) == INT64(0)) /* ZLINE: 225 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    {
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      halt_chpl(&_str_literal_30_chpl, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    }
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    call_tmp_chpl15 = (call_tmp_chpl14 / INT64(1));
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    p_chpl = call_tmp_chpl15;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    if (call_tmp_chpl15 < INT64(0)) /* ZLINE: 225 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    {
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl___COLON_24(UINT64(1), &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl16 = ret_tmp_chpl;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl___PLUS_(&_str_literal_45_chpl, &call_tmp_chpl16, &ret_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl17 = ret_tmp_chpl2;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl___PLUS_(&call_tmp_chpl17, &_str_literal_46_chpl, &ret_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl18 = ret_tmp_chpl3;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl___COLON_25(call_tmp_chpl15, &ret_tmp_chpl4, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl19 = ret_tmp_chpl4;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl___PLUS_(&call_tmp_chpl18, &call_tmp_chpl19, &ret_tmp_chpl5, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl20 = ret_tmp_chpl5;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl___PLUS_(&call_tmp_chpl20, &_str_literal_47_chpl, &ret_tmp_chpl6, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl21 = ret_tmp_chpl6;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl___COLON_25(call_tmp_chpl15, &ret_tmp_chpl7, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl22 = ret_tmp_chpl7;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl___PLUS_(&call_tmp_chpl21, &call_tmp_chpl22, &ret_tmp_chpl8, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl23 = ret_tmp_chpl8;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl___PLUS_(&call_tmp_chpl23, &_str_literal_48_chpl, &ret_tmp_chpl9, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl24 = ret_tmp_chpl9;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      msg_chpl = call_tmp_chpl24;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      i_x_chpl = &call_tmp_chpl23;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      deinit_chpl95(i_x_chpl, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      i_x_chpl2 = &call_tmp_chpl22;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      deinit_chpl95(i_x_chpl2, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      i_x_chpl3 = &call_tmp_chpl21;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      deinit_chpl95(i_x_chpl3, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      i_x_chpl4 = &call_tmp_chpl20;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      deinit_chpl95(i_x_chpl4, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      i_x_chpl5 = &call_tmp_chpl19;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      deinit_chpl95(i_x_chpl5, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      i_x_chpl6 = &call_tmp_chpl18;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      deinit_chpl95(i_x_chpl6, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      i_x_chpl7 = &call_tmp_chpl17;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      deinit_chpl95(i_x_chpl7, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      i_x_chpl8 = &call_tmp_chpl16;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      deinit_chpl95(i_x_chpl8, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      boundsCheckHalt_chpl(&msg_chpl, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      i_x_chpl9 = &msg_chpl;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      deinit_chpl95(i_x_chpl9, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    } else /* ZLINE: 225 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    if (call_tmp_chpl15 >= INT64(64)) /* ZLINE: 225 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    {
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl___COLON_24(UINT64(1), &ret_tmp_chpl10, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl25 = ret_tmp_chpl10;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl___PLUS_(&_str_literal_45_chpl, &call_tmp_chpl25, &ret_tmp_chpl11, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl26 = ret_tmp_chpl11;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl___PLUS_(&call_tmp_chpl26, &_str_literal_46_chpl, &ret_tmp_chpl12, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl27 = ret_tmp_chpl12;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl___COLON_25(call_tmp_chpl15, &ret_tmp_chpl13, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl28 = ret_tmp_chpl13;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl___PLUS_(&call_tmp_chpl27, &call_tmp_chpl28, &ret_tmp_chpl14, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl29 = ret_tmp_chpl14;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl___PLUS_(&call_tmp_chpl29, &_str_literal_47_chpl, &ret_tmp_chpl15, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl30 = ret_tmp_chpl15;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl___COLON_25(call_tmp_chpl15, &ret_tmp_chpl16, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl31 = ret_tmp_chpl16;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl___PLUS_(&call_tmp_chpl30, &call_tmp_chpl31, &ret_tmp_chpl17, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl32 = ret_tmp_chpl17;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl___PLUS_(&call_tmp_chpl32, &_str_literal_50_chpl, &ret_tmp_chpl18, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl33 = ret_tmp_chpl18;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl___PLUS_(&call_tmp_chpl33, &_str_literal_3965_chpl, &ret_tmp_chpl19, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl34 = ret_tmp_chpl19;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      msg_chpl2 = call_tmp_chpl34;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      i_x_chpl10 = &call_tmp_chpl33;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      deinit_chpl95(i_x_chpl10, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      i_x_chpl11 = &call_tmp_chpl32;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      deinit_chpl95(i_x_chpl11, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      i_x_chpl12 = &call_tmp_chpl31;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      deinit_chpl95(i_x_chpl12, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      i_x_chpl13 = &call_tmp_chpl30;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      deinit_chpl95(i_x_chpl13, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      i_x_chpl14 = &call_tmp_chpl29;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      deinit_chpl95(i_x_chpl14, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      i_x_chpl15 = &call_tmp_chpl28;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      deinit_chpl95(i_x_chpl15, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      i_x_chpl16 = &call_tmp_chpl27;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      deinit_chpl95(i_x_chpl16, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      i_x_chpl17 = &call_tmp_chpl26;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      deinit_chpl95(i_x_chpl17, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      i_x_chpl18 = &call_tmp_chpl25;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      deinit_chpl95(i_x_chpl18, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      boundsCheckHalt_chpl(&msg_chpl2, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      i_x_chpl19 = &msg_chpl2;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      deinit_chpl95(i_x_chpl19, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    }
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    if (((uint64_t)((UINT64(1) << call_tmp_chpl15))) < UINT64(0)) /* ZLINE: 225 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    {
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      i_lhs_chpl3 = &p_chpl;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      *(i_lhs_chpl3) += INT64(1);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    }
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    if (p_chpl < INT64(0)) /* ZLINE: 225 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    {
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl___COLON_24(UINT64(1), &ret_tmp_chpl20, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl35 = ret_tmp_chpl20;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl___PLUS_(&_str_literal_45_chpl, &call_tmp_chpl35, &ret_tmp_chpl21, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl36 = ret_tmp_chpl21;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl___PLUS_(&call_tmp_chpl36, &_str_literal_46_chpl, &ret_tmp_chpl22, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl37 = ret_tmp_chpl22;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl___COLON_25(p_chpl, &ret_tmp_chpl23, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl38 = ret_tmp_chpl23;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl___PLUS_(&call_tmp_chpl37, &call_tmp_chpl38, &ret_tmp_chpl24, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl39 = ret_tmp_chpl24;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl___PLUS_(&call_tmp_chpl39, &_str_literal_47_chpl, &ret_tmp_chpl25, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl40 = ret_tmp_chpl25;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl___COLON_25(p_chpl, &ret_tmp_chpl26, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl41 = ret_tmp_chpl26;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl___PLUS_(&call_tmp_chpl40, &call_tmp_chpl41, &ret_tmp_chpl27, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl42 = ret_tmp_chpl27;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl___PLUS_(&call_tmp_chpl42, &_str_literal_48_chpl, &ret_tmp_chpl28, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl43 = ret_tmp_chpl28;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      msg_chpl3 = call_tmp_chpl43;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      i_x_chpl20 = &call_tmp_chpl42;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      deinit_chpl95(i_x_chpl20, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      i_x_chpl21 = &call_tmp_chpl41;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      deinit_chpl95(i_x_chpl21, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      i_x_chpl22 = &call_tmp_chpl40;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      deinit_chpl95(i_x_chpl22, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      i_x_chpl23 = &call_tmp_chpl39;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      deinit_chpl95(i_x_chpl23, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      i_x_chpl24 = &call_tmp_chpl38;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      deinit_chpl95(i_x_chpl24, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      i_x_chpl25 = &call_tmp_chpl37;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      deinit_chpl95(i_x_chpl25, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      i_x_chpl26 = &call_tmp_chpl36;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      deinit_chpl95(i_x_chpl26, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      i_x_chpl27 = &call_tmp_chpl35;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      deinit_chpl95(i_x_chpl27, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      boundsCheckHalt_chpl(&msg_chpl3, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      i_x_chpl28 = &msg_chpl3;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      deinit_chpl95(i_x_chpl28, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    } else /* ZLINE: 225 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    if (p_chpl >= INT64(64)) /* ZLINE: 225 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    {
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl___COLON_24(UINT64(1), &ret_tmp_chpl29, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl44 = ret_tmp_chpl29;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl___PLUS_(&_str_literal_45_chpl, &call_tmp_chpl44, &ret_tmp_chpl30, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl45 = ret_tmp_chpl30;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl___PLUS_(&call_tmp_chpl45, &_str_literal_46_chpl, &ret_tmp_chpl31, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl46 = ret_tmp_chpl31;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl___COLON_25(p_chpl, &ret_tmp_chpl32, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl47 = ret_tmp_chpl32;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl___PLUS_(&call_tmp_chpl46, &call_tmp_chpl47, &ret_tmp_chpl33, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl48 = ret_tmp_chpl33;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl___PLUS_(&call_tmp_chpl48, &_str_literal_47_chpl, &ret_tmp_chpl34, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl49 = ret_tmp_chpl34;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl___COLON_25(p_chpl, &ret_tmp_chpl35, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl50 = ret_tmp_chpl35;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl___PLUS_(&call_tmp_chpl49, &call_tmp_chpl50, &ret_tmp_chpl36, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl51 = ret_tmp_chpl36;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl___PLUS_(&call_tmp_chpl51, &_str_literal_50_chpl, &ret_tmp_chpl37, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl52 = ret_tmp_chpl37;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl___PLUS_(&call_tmp_chpl52, &_str_literal_3965_chpl, &ret_tmp_chpl38, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl53 = ret_tmp_chpl38;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      msg_chpl4 = call_tmp_chpl53;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      i_x_chpl29 = &call_tmp_chpl52;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      deinit_chpl95(i_x_chpl29, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      i_x_chpl30 = &call_tmp_chpl51;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      deinit_chpl95(i_x_chpl30, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      i_x_chpl31 = &call_tmp_chpl50;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      deinit_chpl95(i_x_chpl31, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      i_x_chpl32 = &call_tmp_chpl49;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      deinit_chpl95(i_x_chpl32, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      i_x_chpl33 = &call_tmp_chpl48;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      deinit_chpl95(i_x_chpl33, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      i_x_chpl34 = &call_tmp_chpl47;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      deinit_chpl95(i_x_chpl34, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      i_x_chpl35 = &call_tmp_chpl46;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      deinit_chpl95(i_x_chpl35, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      i_x_chpl36 = &call_tmp_chpl45;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      deinit_chpl95(i_x_chpl36, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      i_x_chpl37 = &call_tmp_chpl44;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      deinit_chpl95(i_x_chpl37, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      boundsCheckHalt_chpl(&msg_chpl4, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      i_x_chpl38 = &msg_chpl4;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      deinit_chpl95(i_x_chpl38, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    }
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    assert_chpl((UINT64(0) <= ((uint64_t)((UINT64(1) << p_chpl)))), _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    if (p_chpl < INT64(0)) /* ZLINE: 225 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    {
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl___COLON_24(UINT64(1), &ret_tmp_chpl39, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl54 = ret_tmp_chpl39;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl___PLUS_(&_str_literal_45_chpl, &call_tmp_chpl54, &ret_tmp_chpl40, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl55 = ret_tmp_chpl40;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl___PLUS_(&call_tmp_chpl55, &_str_literal_46_chpl, &ret_tmp_chpl41, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl56 = ret_tmp_chpl41;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl___COLON_25(p_chpl, &ret_tmp_chpl42, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl57 = ret_tmp_chpl42;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl___PLUS_(&call_tmp_chpl56, &call_tmp_chpl57, &ret_tmp_chpl43, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl58 = ret_tmp_chpl43;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl___PLUS_(&call_tmp_chpl58, &_str_literal_47_chpl, &ret_tmp_chpl44, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl59 = ret_tmp_chpl44;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl___COLON_25(p_chpl, &ret_tmp_chpl45, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl60 = ret_tmp_chpl45;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl___PLUS_(&call_tmp_chpl59, &call_tmp_chpl60, &ret_tmp_chpl46, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl61 = ret_tmp_chpl46;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl___PLUS_(&call_tmp_chpl61, &_str_literal_48_chpl, &ret_tmp_chpl47, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl62 = ret_tmp_chpl47;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      msg_chpl5 = call_tmp_chpl62;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      i_x_chpl39 = &call_tmp_chpl61;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      deinit_chpl95(i_x_chpl39, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      i_x_chpl40 = &call_tmp_chpl60;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      deinit_chpl95(i_x_chpl40, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      i_x_chpl41 = &call_tmp_chpl59;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      deinit_chpl95(i_x_chpl41, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      i_x_chpl42 = &call_tmp_chpl58;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      deinit_chpl95(i_x_chpl42, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      i_x_chpl43 = &call_tmp_chpl57;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      deinit_chpl95(i_x_chpl43, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      i_x_chpl44 = &call_tmp_chpl56;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      deinit_chpl95(i_x_chpl44, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      i_x_chpl45 = &call_tmp_chpl55;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      deinit_chpl95(i_x_chpl45, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      i_x_chpl46 = &call_tmp_chpl54;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      deinit_chpl95(i_x_chpl46, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      boundsCheckHalt_chpl(&msg_chpl5, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      i_x_chpl47 = &msg_chpl5;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      deinit_chpl95(i_x_chpl47, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    } else /* ZLINE: 225 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    if (p_chpl >= INT64(64)) /* ZLINE: 225 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    {
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl___COLON_24(UINT64(1), &ret_tmp_chpl48, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl63 = ret_tmp_chpl48;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl___PLUS_(&_str_literal_45_chpl, &call_tmp_chpl63, &ret_tmp_chpl49, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl64 = ret_tmp_chpl49;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl___PLUS_(&call_tmp_chpl64, &_str_literal_46_chpl, &ret_tmp_chpl50, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl65 = ret_tmp_chpl50;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl___COLON_25(p_chpl, &ret_tmp_chpl51, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl66 = ret_tmp_chpl51;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl___PLUS_(&call_tmp_chpl65, &call_tmp_chpl66, &ret_tmp_chpl52, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl67 = ret_tmp_chpl52;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl___PLUS_(&call_tmp_chpl67, &_str_literal_47_chpl, &ret_tmp_chpl53, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl68 = ret_tmp_chpl53;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl___COLON_25(p_chpl, &ret_tmp_chpl54, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl69 = ret_tmp_chpl54;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl___PLUS_(&call_tmp_chpl68, &call_tmp_chpl69, &ret_tmp_chpl55, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl70 = ret_tmp_chpl55;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl___PLUS_(&call_tmp_chpl70, &_str_literal_50_chpl, &ret_tmp_chpl56, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl71 = ret_tmp_chpl56;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl___PLUS_(&call_tmp_chpl71, &_str_literal_3965_chpl, &ret_tmp_chpl57, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl72 = ret_tmp_chpl57;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      msg_chpl6 = call_tmp_chpl72;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      i_x_chpl48 = &call_tmp_chpl71;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      deinit_chpl95(i_x_chpl48, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      i_x_chpl49 = &call_tmp_chpl70;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      deinit_chpl95(i_x_chpl49, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      i_x_chpl50 = &call_tmp_chpl69;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      deinit_chpl95(i_x_chpl50, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      i_x_chpl51 = &call_tmp_chpl68;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      deinit_chpl95(i_x_chpl51, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      i_x_chpl52 = &call_tmp_chpl67;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      deinit_chpl95(i_x_chpl52, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      i_x_chpl53 = &call_tmp_chpl66;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      deinit_chpl95(i_x_chpl53, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      i_x_chpl54 = &call_tmp_chpl65;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      deinit_chpl95(i_x_chpl54, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      i_x_chpl55 = &call_tmp_chpl64;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      deinit_chpl95(i_x_chpl55, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      i_x_chpl56 = &call_tmp_chpl63;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      deinit_chpl95(i_x_chpl56, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      boundsCheckHalt_chpl(&msg_chpl6, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      i_x_chpl57 = &msg_chpl6;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      deinit_chpl95(i_x_chpl57, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    }
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    if (UINT64(0) != ((uint64_t)((UINT64(1) << p_chpl)))) /* ZLINE: 225 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    {
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      halt_chpl15(&_str_literal_2423_chpl, UINT64(0), _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    }
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    call_tmp_chpl73 = sizeof(c_ptr_void_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    if (UINT64(0) < call_tmp_chpl73) /* ZLINE: 225 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    {
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      halt_chpl(&_str_literal_2424_chpl, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    }
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    call_tmp_chpl74 = offset_ARRAY_ELEMENTS_chpl();
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    call_tmp_chpl75 = chpl_here_aligned_alloc(UINT64(0), call_tmp_chpl10, call_tmp_chpl74, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    ptr_chpl = call_tmp_chpl75;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    {
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      inlineImm_chpl = INT64(0);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl_checkValue6(INT64(0), &ret_tmp_chpl58, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl76 = ret_tmp_chpl58;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      i_arg_chpl = &call_tmp_chpl76;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl77 = borrow_chpl2(i_arg_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      if (((RootClass_chpl)(call_tmp_chpl77)) != nil) /* ZLINE: 225 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      {
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        coerce_tmp_chpl10 = ((Error_chpl)(call_tmp_chpl77));
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        chpl_check_nil(coerce_tmp_chpl10, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        ((void(*)(Error_chpl,
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
           string_chpl *,
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
           int64_t,
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
           int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(coerce_tmp_chpl10))->chpl__cid) + INT64(1))])(coerce_tmp_chpl10, &ret_tmp_chpl59, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        call_tmp_chpl78 = ret_tmp_chpl59;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        safeCastCheckHalt_chpl(&call_tmp_chpl78, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        i_x_chpl58 = &call_tmp_chpl78;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        deinit_chpl95(i_x_chpl58, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      }
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl__autoDestroy62(&call_tmp_chpl76, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      memset(call_tmp_chpl75, ((int32_t)(inlineImm_chpl)), call_tmp_chpl10);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    }
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  }
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  call_tmp_chpl79 = ((c_ptr__owned_Error_chpl)(ptr_chpl));
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_check_nil(initTemp_chpl, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  (initTemp_chpl)->errorsArray = call_tmp_chpl79;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  idx_chpl = INT64(0);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  cur_chpl = head_chpl;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  tmp_chpl4 = (((RootClass_chpl)(head_chpl)) != nil);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  while (tmp_chpl4) {
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    if (((RootClass_chpl)(cur_chpl)) == nil) /* ZLINE: 225 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    {
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      nilCheckHalt_chpl(&_str_literal_52_chpl, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    }
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    coerce_tmp_chpl11 = cur_chpl;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    chpl_check_nil(coerce_tmp_chpl11, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    curnext_chpl2 = (coerce_tmp_chpl11)->_next;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    if (((RootClass_chpl)(cur_chpl)) == nil) /* ZLINE: 225 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    {
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      nilCheckHalt_chpl(&_str_literal_52_chpl, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    }
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    coerce_tmp_chpl12 = cur_chpl;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    tmp_chpl5 = ((Error_chpl)(nil));
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    chpl_check_nil(coerce_tmp_chpl12, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    (coerce_tmp_chpl12)->_next = tmp_chpl5;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    if (((RootClass_chpl)(cur_chpl)) == nil) /* ZLINE: 225 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    {
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      ret_chpl2 = ((TaskErrors_chpl)(nil));
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      goto _end__COLON__chpl2;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    }
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    chpl_macro_tmp_12 = ((RootClass_chpl)(cur_chpl))->chpl__cid;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    ret_chpl2 = ( (((chpl__cid_TaskErrors_chpl <= chpl_macro_tmp_12) && (chpl_macro_tmp_12 <= chpl_subclass_max_id[chpl__cid_TaskErrors_chpl])))?(((TaskErrors_chpl)(cur_chpl))):(((TaskErrors_chpl)(NULL))) );
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    _end__COLON__chpl2:;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    asTaskErr_chpl = ret_chpl2;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    if (((RootClass_chpl)(ret_chpl2)) == nil) /* ZLINE: 225 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    {
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl_check_nil(initTemp_chpl, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      coerce_tmp_chpl13 = (initTemp_chpl)->errorsArray;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl80 = (coerce_tmp_chpl13 + idx_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      if (((RootClass_chpl)(cur_chpl)) == nil) /* ZLINE: 225 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      {
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        nilCheckHalt_chpl(&_str_literal_52_chpl, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      }
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      i__retArg_chpl = &ret_tmp_chpl60;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      init_chpl188(&initTemp_chpl2, cur_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl82 = initTemp_chpl2;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      ret_chpl3 = call_tmp_chpl82;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      *(i__retArg_chpl) = ret_chpl3;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl81 = ret_tmp_chpl60;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl___ASSIGN_11(call_tmp_chpl80, &call_tmp_chpl81, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl__autoDestroy57(&call_tmp_chpl81, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      i_lhs_chpl4 = &idx_chpl;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      *(i_lhs_chpl4) += INT64(1);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    } else /* ZLINE: 225 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    {
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      _ic__F0_this_chpl2 = ret_chpl2;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      this_chpl34 = ret_chpl2;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl_check_nil(this_chpl34, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      coerce_tmp_chpl14 = (this_chpl34)->nErrors;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      inlineImm_chpl2 = INT64(0);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      if (coerce_tmp_chpl14 < INT64(0)) /* ZLINE: 225 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      {
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        boundsCheckHalt_chpl(&_str_literal_580_chpl, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      }
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      if (coerce_tmp_chpl14 == INT64(0)) /* ZLINE: 225 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      {
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        tmp_chpl6 = ((int64_t)(((uint64_t)((((uint64_t)(inlineImm_chpl2)) - UINT64(1))))));
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      } else /* ZLINE: 225 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      {
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        tmp_chpl6 = ((int64_t)((inlineImm_chpl2 + ((int64_t)((coerce_tmp_chpl14 - INT64(1)))))));
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      }
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      _ic__F1_high_chpl2 = tmp_chpl6;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      i_chpl2 = INT64(0);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl_checkIfRangeIterWillOverflow(inlineImm_chpl2, tmp_chpl6, INT64(1), inlineImm_chpl2, tmp_chpl6, UINT8(true), _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      this_chpl35 = _ic__F0_this_chpl2;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl_check_nil(this_chpl35, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      coerce_tmp_chpl15 = (this_chpl35)->errorsArray;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      for (i_chpl2 = inlineImm_chpl2; ((i_chpl2 <= _ic__F1_high_chpl2)); i_chpl2 += INT64(1)) {
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        call_tmp_chpl83 = (coerce_tmp_chpl15 + i_chpl2);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        coerce_tmp_chpl16 = borrow_chpl3(call_tmp_chpl83);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        if (((RootClass_chpl)(coerce_tmp_chpl16)) != nil) /* ZLINE: 225 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        {
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          this_chpl36 = _ic__F0_this_chpl2;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          chpl_check_nil(this_chpl36, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          coerce_tmp_chpl17 = (this_chpl36)->errorsArray;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          call_tmp_chpl84 = (coerce_tmp_chpl17 + i_chpl2);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          coerce_tmp_chpl18 = borrow_chpl3(call_tmp_chpl84);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          if (((RootClass_chpl)(coerce_tmp_chpl18)) != nil) /* ZLINE: 225 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          {
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            chpl_check_nil(initTemp_chpl, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            coerce_tmp_chpl19 = (initTemp_chpl)->errorsArray;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            call_tmp_chpl85 = (coerce_tmp_chpl19 + idx_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            chpl___ASSIGN_10(call_tmp_chpl85, call_tmp_chpl84, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            i_lhs_chpl5 = &idx_chpl;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            *(i_lhs_chpl5) += INT64(1);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          }
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        }
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      }
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl__delete40(asTaskErr_chpl, _ln_chpl, _fn_chpl);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    }
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    cur_chpl = curnext_chpl2;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    tmp_chpl4 = (((RootClass_chpl)(curnext_chpl2)) != nil);
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  }
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  return initTemp_chpl;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
}

#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
/* Errors.chpl:280 */
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
static TaskErrors_chpl _new_chpl62(Error_chpl err_chpl8,
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                                   int64_t _ln_chpl,
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                                   int32_t _fn_chpl) {
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  TaskErrors_chpl initTemp_chpl = NULL;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  c_ptr_void_chpl cast_tmp_chpl = NULL;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  Error_chpl super_tmp_chpl = NULL;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  Error_chpl init_coerce_tmp_chpl = NULL;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl _msg_chpl;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  uint64_t coerce_tmp_chpl3;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  uint64_t call_tmp_chpl6;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  uint64_t call_tmp_chpl7;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_bool tmp_chpl;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  c_ptr_void_chpl ptr_chpl = NULL;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  c_ptr_void_chpl init_coerce_tmp_chpl2 = NULL;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int16_t call_tmp_chpl8;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  c_ptr_void_chpl call_tmp_chpl9 = NULL;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  uint64_t call_tmp_chpl10;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int64_t call_tmp_chpl11;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int64_t call_tmp_chpl12;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int64_t p_chpl;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl13;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl14;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl2;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl15;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl3;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl16;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl4;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl17;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl5;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl18;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl6;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl19;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl7;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl20;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl8;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl21;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl9;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl msg_chpl;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl2 = NULL;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl3 = NULL;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl4 = NULL;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl5 = NULL;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl6 = NULL;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl7 = NULL;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl8 = NULL;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl9 = NULL;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl22;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl10;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl23;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl11;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl24;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl12;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl25;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl13;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl26;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl14;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl27;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl15;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl28;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl16;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl29;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl17;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl30;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl18;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl31;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl19;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl msg_chpl2;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl10 = NULL;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl11 = NULL;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl12 = NULL;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl13 = NULL;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl14 = NULL;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl15 = NULL;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl16 = NULL;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl17 = NULL;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl18 = NULL;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl19 = NULL;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_int64_t_chpl i_lhs_chpl = NULL;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl32;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl20;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl33;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl21;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl34;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl22;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl35;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl23;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl36;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl24;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl37;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl25;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl38;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl26;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl39;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl27;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl40;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl28;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl msg_chpl3;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl20 = NULL;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl21 = NULL;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl22 = NULL;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl23 = NULL;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl24 = NULL;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl25 = NULL;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl26 = NULL;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl27 = NULL;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl28 = NULL;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl41;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl29;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl42;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl30;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl43;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl31;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl44;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl32;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl45;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl33;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl46;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl34;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl47;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl35;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl48;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl36;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl49;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl37;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl50;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl38;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl msg_chpl4;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl29 = NULL;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl30 = NULL;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl31 = NULL;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl32 = NULL;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl33 = NULL;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl34 = NULL;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl35 = NULL;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl36 = NULL;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl37 = NULL;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl38 = NULL;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl51;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl39;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl52;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl40;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl53;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl41;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl54;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl42;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl55;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl43;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl56;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl44;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl57;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl45;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl58;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl46;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl59;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl47;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl msg_chpl5;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl39 = NULL;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl40 = NULL;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl41 = NULL;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl42 = NULL;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl43 = NULL;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl44 = NULL;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl45 = NULL;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl46 = NULL;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl47 = NULL;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl60;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl48;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl61;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl49;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl62;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl50;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl63;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl51;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl64;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl52;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl65;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl53;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl66;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl54;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl67;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl55;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl68;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl56;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl69;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl57;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl msg_chpl6;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl48 = NULL;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl49 = NULL;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl50 = NULL;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl51 = NULL;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl52 = NULL;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl53 = NULL;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl54 = NULL;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl55 = NULL;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl56 = NULL;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl57 = NULL;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  uint64_t call_tmp_chpl70;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int16_t call_tmp_chpl71;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  c_ptr_void_chpl call_tmp_chpl72 = NULL;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int64_t inlineImm_chpl;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _owned_IllegalArgumentError_chpl2 call_tmp_chpl73;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _owned_IllegalArgumentError_chpl2 ret_tmp_chpl58;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref__owned_IllegalArgumentError_chpl i_arg_chpl = NULL;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  IllegalArgumentError_chpl call_tmp_chpl74 = NULL;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl75;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  Error_chpl coerce_tmp_chpl4 = NULL;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl59;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl58 = NULL;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  c_ptr__owned_Error_chpl call_tmp_chpl76 = NULL;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  Error_chpl coerce_tmp_chpl5 = NULL;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  Error_chpl tmp_chpl2 = NULL;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  c_ptr__owned_Error_chpl coerce_tmp_chpl6 = NULL;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int64_t inlineImm_chpl2;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref__owned_Error_chpl call_tmp_chpl77 = NULL;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _owned_Error_chpl call_tmp_chpl78;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _owned_Error_chpl ret_tmp_chpl60;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref__owned_Error_chpl2 i__retArg_chpl = NULL;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _owned_Error_chpl ret_chpl;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _owned_Error_chpl call_tmp_chpl79;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _owned_Error_chpl initTemp_chpl2;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  cast_tmp_chpl = chpl_here_alloc(sizeof(chpl_TaskErrors_chpl_object), INT16(43), _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  initTemp_chpl = ((TaskErrors_chpl)(cast_tmp_chpl));
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  ((RootClass_chpl)(initTemp_chpl))->chpl__cid = chpl__cid_TaskErrors_chpl;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_check_nil(initTemp_chpl, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  super_tmp_chpl = &((initTemp_chpl)->super);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  ((RootClass_chpl)(&((super_tmp_chpl)->super)))->chpl__cid = chpl__cid_RootClass_chpl;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  ((RootClass_chpl)(((RootClass_chpl)(super_tmp_chpl))))->chpl__cid = chpl__cid_RootClass_chpl;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  init_coerce_tmp_chpl = ((Error_chpl)(nil));
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  (super_tmp_chpl)->_next = init_coerce_tmp_chpl;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  (super_tmp_chpl)->thrownLine = INT64(0);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  (super_tmp_chpl)->thrownFileId = INT32(0);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  init_chpl194(&_msg_chpl, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  (super_tmp_chpl)->_msg = _msg_chpl;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  (super_tmp_chpl)->_hasThrowInfo = UINT8(false);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  ((RootClass_chpl)(super_tmp_chpl))->chpl__cid = chpl__cid_Error_chpl;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  ((RootClass_chpl)(((Error_chpl)(initTemp_chpl))))->chpl__cid = chpl__cid_Error_chpl;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_check_nil(initTemp_chpl, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  (initTemp_chpl)->nErrors = INT64(1);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  coerce_tmp_chpl3 = ((uint64_t)(INT64(1)));
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  call_tmp_chpl6 = sizeof(_owned_Error_chpl2);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  call_tmp_chpl7 = (coerce_tmp_chpl3 * call_tmp_chpl6);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  if (INT64(0) < INT64(0)) /* ZLINE: 280 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  {
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    tmp_chpl = UINT8(true);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  } else /* ZLINE: 280 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  {
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    tmp_chpl = (UINT64(0) != ((uint64_t)(INT64(0))));
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  }
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  init_coerce_tmp_chpl2 = ((c_ptr_void_chpl)(nil));
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  ptr_chpl = init_coerce_tmp_chpl2;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  if (! tmp_chpl) /* ZLINE: 280 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  {
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    {
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl8 = offset_ARRAY_ELEMENTS_chpl();
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl9 = chpl_here_calloc(call_tmp_chpl7, INT64(1), call_tmp_chpl8, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      ptr_chpl = call_tmp_chpl9;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    }
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  } else /* ZLINE: 280 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  {
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    call_tmp_chpl10 = chpl_bitops_clz_64(UINT64(0));
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    call_tmp_chpl11 = (INT64(63) - ((int64_t)(call_tmp_chpl10)));
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    if (INT64(1) == INT64(0)) /* ZLINE: 280 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    {
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      halt_chpl(&_str_literal_30_chpl, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    }
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    call_tmp_chpl12 = (call_tmp_chpl11 / INT64(1));
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    p_chpl = call_tmp_chpl12;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    if (call_tmp_chpl12 < INT64(0)) /* ZLINE: 280 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    {
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl___COLON_24(UINT64(1), &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl13 = ret_tmp_chpl;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl___PLUS_(&_str_literal_45_chpl, &call_tmp_chpl13, &ret_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl14 = ret_tmp_chpl2;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl___PLUS_(&call_tmp_chpl14, &_str_literal_46_chpl, &ret_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl15 = ret_tmp_chpl3;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl___COLON_25(call_tmp_chpl12, &ret_tmp_chpl4, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl16 = ret_tmp_chpl4;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl___PLUS_(&call_tmp_chpl15, &call_tmp_chpl16, &ret_tmp_chpl5, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl17 = ret_tmp_chpl5;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl___PLUS_(&call_tmp_chpl17, &_str_literal_47_chpl, &ret_tmp_chpl6, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl18 = ret_tmp_chpl6;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl___COLON_25(call_tmp_chpl12, &ret_tmp_chpl7, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl19 = ret_tmp_chpl7;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl___PLUS_(&call_tmp_chpl18, &call_tmp_chpl19, &ret_tmp_chpl8, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl20 = ret_tmp_chpl8;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl___PLUS_(&call_tmp_chpl20, &_str_literal_48_chpl, &ret_tmp_chpl9, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl21 = ret_tmp_chpl9;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      msg_chpl = call_tmp_chpl21;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      i_x_chpl = &call_tmp_chpl20;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      deinit_chpl95(i_x_chpl, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      i_x_chpl2 = &call_tmp_chpl19;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      deinit_chpl95(i_x_chpl2, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      i_x_chpl3 = &call_tmp_chpl18;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      deinit_chpl95(i_x_chpl3, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      i_x_chpl4 = &call_tmp_chpl17;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      deinit_chpl95(i_x_chpl4, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      i_x_chpl5 = &call_tmp_chpl16;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      deinit_chpl95(i_x_chpl5, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      i_x_chpl6 = &call_tmp_chpl15;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      deinit_chpl95(i_x_chpl6, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      i_x_chpl7 = &call_tmp_chpl14;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      deinit_chpl95(i_x_chpl7, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      i_x_chpl8 = &call_tmp_chpl13;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      deinit_chpl95(i_x_chpl8, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      boundsCheckHalt_chpl(&msg_chpl, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      i_x_chpl9 = &msg_chpl;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      deinit_chpl95(i_x_chpl9, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    } else /* ZLINE: 280 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    if (call_tmp_chpl12 >= INT64(64)) /* ZLINE: 280 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    {
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl___COLON_24(UINT64(1), &ret_tmp_chpl10, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl22 = ret_tmp_chpl10;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl___PLUS_(&_str_literal_45_chpl, &call_tmp_chpl22, &ret_tmp_chpl11, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl23 = ret_tmp_chpl11;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl___PLUS_(&call_tmp_chpl23, &_str_literal_46_chpl, &ret_tmp_chpl12, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl24 = ret_tmp_chpl12;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl___COLON_25(call_tmp_chpl12, &ret_tmp_chpl13, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl25 = ret_tmp_chpl13;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl___PLUS_(&call_tmp_chpl24, &call_tmp_chpl25, &ret_tmp_chpl14, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl26 = ret_tmp_chpl14;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl___PLUS_(&call_tmp_chpl26, &_str_literal_47_chpl, &ret_tmp_chpl15, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl27 = ret_tmp_chpl15;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl___COLON_25(call_tmp_chpl12, &ret_tmp_chpl16, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl28 = ret_tmp_chpl16;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl___PLUS_(&call_tmp_chpl27, &call_tmp_chpl28, &ret_tmp_chpl17, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl29 = ret_tmp_chpl17;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl___PLUS_(&call_tmp_chpl29, &_str_literal_50_chpl, &ret_tmp_chpl18, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl30 = ret_tmp_chpl18;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl___PLUS_(&call_tmp_chpl30, &_str_literal_3965_chpl, &ret_tmp_chpl19, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl31 = ret_tmp_chpl19;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      msg_chpl2 = call_tmp_chpl31;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      i_x_chpl10 = &call_tmp_chpl30;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      deinit_chpl95(i_x_chpl10, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      i_x_chpl11 = &call_tmp_chpl29;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      deinit_chpl95(i_x_chpl11, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      i_x_chpl12 = &call_tmp_chpl28;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      deinit_chpl95(i_x_chpl12, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      i_x_chpl13 = &call_tmp_chpl27;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      deinit_chpl95(i_x_chpl13, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      i_x_chpl14 = &call_tmp_chpl26;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      deinit_chpl95(i_x_chpl14, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      i_x_chpl15 = &call_tmp_chpl25;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      deinit_chpl95(i_x_chpl15, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      i_x_chpl16 = &call_tmp_chpl24;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      deinit_chpl95(i_x_chpl16, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      i_x_chpl17 = &call_tmp_chpl23;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      deinit_chpl95(i_x_chpl17, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      i_x_chpl18 = &call_tmp_chpl22;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      deinit_chpl95(i_x_chpl18, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      boundsCheckHalt_chpl(&msg_chpl2, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      i_x_chpl19 = &msg_chpl2;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      deinit_chpl95(i_x_chpl19, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    }
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    if (((uint64_t)((UINT64(1) << call_tmp_chpl12))) < UINT64(0)) /* ZLINE: 280 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    {
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      i_lhs_chpl = &p_chpl;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      *(i_lhs_chpl) += INT64(1);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    }
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    if (p_chpl < INT64(0)) /* ZLINE: 280 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    {
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl___COLON_24(UINT64(1), &ret_tmp_chpl20, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl32 = ret_tmp_chpl20;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl___PLUS_(&_str_literal_45_chpl, &call_tmp_chpl32, &ret_tmp_chpl21, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl33 = ret_tmp_chpl21;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl___PLUS_(&call_tmp_chpl33, &_str_literal_46_chpl, &ret_tmp_chpl22, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl34 = ret_tmp_chpl22;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl___COLON_25(p_chpl, &ret_tmp_chpl23, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl35 = ret_tmp_chpl23;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl___PLUS_(&call_tmp_chpl34, &call_tmp_chpl35, &ret_tmp_chpl24, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl36 = ret_tmp_chpl24;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl___PLUS_(&call_tmp_chpl36, &_str_literal_47_chpl, &ret_tmp_chpl25, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl37 = ret_tmp_chpl25;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl___COLON_25(p_chpl, &ret_tmp_chpl26, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl38 = ret_tmp_chpl26;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl___PLUS_(&call_tmp_chpl37, &call_tmp_chpl38, &ret_tmp_chpl27, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl39 = ret_tmp_chpl27;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl___PLUS_(&call_tmp_chpl39, &_str_literal_48_chpl, &ret_tmp_chpl28, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl40 = ret_tmp_chpl28;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      msg_chpl3 = call_tmp_chpl40;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      i_x_chpl20 = &call_tmp_chpl39;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      deinit_chpl95(i_x_chpl20, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      i_x_chpl21 = &call_tmp_chpl38;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      deinit_chpl95(i_x_chpl21, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      i_x_chpl22 = &call_tmp_chpl37;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      deinit_chpl95(i_x_chpl22, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      i_x_chpl23 = &call_tmp_chpl36;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      deinit_chpl95(i_x_chpl23, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      i_x_chpl24 = &call_tmp_chpl35;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      deinit_chpl95(i_x_chpl24, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      i_x_chpl25 = &call_tmp_chpl34;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      deinit_chpl95(i_x_chpl25, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      i_x_chpl26 = &call_tmp_chpl33;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      deinit_chpl95(i_x_chpl26, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      i_x_chpl27 = &call_tmp_chpl32;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      deinit_chpl95(i_x_chpl27, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      boundsCheckHalt_chpl(&msg_chpl3, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      i_x_chpl28 = &msg_chpl3;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      deinit_chpl95(i_x_chpl28, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    } else /* ZLINE: 280 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    if (p_chpl >= INT64(64)) /* ZLINE: 280 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    {
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl___COLON_24(UINT64(1), &ret_tmp_chpl29, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl41 = ret_tmp_chpl29;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl___PLUS_(&_str_literal_45_chpl, &call_tmp_chpl41, &ret_tmp_chpl30, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl42 = ret_tmp_chpl30;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl___PLUS_(&call_tmp_chpl42, &_str_literal_46_chpl, &ret_tmp_chpl31, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl43 = ret_tmp_chpl31;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl___COLON_25(p_chpl, &ret_tmp_chpl32, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl44 = ret_tmp_chpl32;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl___PLUS_(&call_tmp_chpl43, &call_tmp_chpl44, &ret_tmp_chpl33, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl45 = ret_tmp_chpl33;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl___PLUS_(&call_tmp_chpl45, &_str_literal_47_chpl, &ret_tmp_chpl34, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl46 = ret_tmp_chpl34;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl___COLON_25(p_chpl, &ret_tmp_chpl35, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl47 = ret_tmp_chpl35;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl___PLUS_(&call_tmp_chpl46, &call_tmp_chpl47, &ret_tmp_chpl36, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl48 = ret_tmp_chpl36;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl___PLUS_(&call_tmp_chpl48, &_str_literal_50_chpl, &ret_tmp_chpl37, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl49 = ret_tmp_chpl37;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl___PLUS_(&call_tmp_chpl49, &_str_literal_3965_chpl, &ret_tmp_chpl38, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl50 = ret_tmp_chpl38;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      msg_chpl4 = call_tmp_chpl50;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      i_x_chpl29 = &call_tmp_chpl49;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      deinit_chpl95(i_x_chpl29, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      i_x_chpl30 = &call_tmp_chpl48;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      deinit_chpl95(i_x_chpl30, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      i_x_chpl31 = &call_tmp_chpl47;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      deinit_chpl95(i_x_chpl31, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      i_x_chpl32 = &call_tmp_chpl46;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      deinit_chpl95(i_x_chpl32, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      i_x_chpl33 = &call_tmp_chpl45;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      deinit_chpl95(i_x_chpl33, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      i_x_chpl34 = &call_tmp_chpl44;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      deinit_chpl95(i_x_chpl34, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      i_x_chpl35 = &call_tmp_chpl43;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      deinit_chpl95(i_x_chpl35, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      i_x_chpl36 = &call_tmp_chpl42;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      deinit_chpl95(i_x_chpl36, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      i_x_chpl37 = &call_tmp_chpl41;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      deinit_chpl95(i_x_chpl37, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      boundsCheckHalt_chpl(&msg_chpl4, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      i_x_chpl38 = &msg_chpl4;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      deinit_chpl95(i_x_chpl38, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    }
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    assert_chpl((UINT64(0) <= ((uint64_t)((UINT64(1) << p_chpl)))), _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    if (p_chpl < INT64(0)) /* ZLINE: 280 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    {
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl___COLON_24(UINT64(1), &ret_tmp_chpl39, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl51 = ret_tmp_chpl39;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl___PLUS_(&_str_literal_45_chpl, &call_tmp_chpl51, &ret_tmp_chpl40, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl52 = ret_tmp_chpl40;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl___PLUS_(&call_tmp_chpl52, &_str_literal_46_chpl, &ret_tmp_chpl41, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl53 = ret_tmp_chpl41;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl___COLON_25(p_chpl, &ret_tmp_chpl42, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl54 = ret_tmp_chpl42;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl___PLUS_(&call_tmp_chpl53, &call_tmp_chpl54, &ret_tmp_chpl43, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl55 = ret_tmp_chpl43;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl___PLUS_(&call_tmp_chpl55, &_str_literal_47_chpl, &ret_tmp_chpl44, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl56 = ret_tmp_chpl44;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl___COLON_25(p_chpl, &ret_tmp_chpl45, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl57 = ret_tmp_chpl45;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl___PLUS_(&call_tmp_chpl56, &call_tmp_chpl57, &ret_tmp_chpl46, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl58 = ret_tmp_chpl46;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl___PLUS_(&call_tmp_chpl58, &_str_literal_48_chpl, &ret_tmp_chpl47, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl59 = ret_tmp_chpl47;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      msg_chpl5 = call_tmp_chpl59;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      i_x_chpl39 = &call_tmp_chpl58;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      deinit_chpl95(i_x_chpl39, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      i_x_chpl40 = &call_tmp_chpl57;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      deinit_chpl95(i_x_chpl40, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      i_x_chpl41 = &call_tmp_chpl56;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      deinit_chpl95(i_x_chpl41, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      i_x_chpl42 = &call_tmp_chpl55;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      deinit_chpl95(i_x_chpl42, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      i_x_chpl43 = &call_tmp_chpl54;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      deinit_chpl95(i_x_chpl43, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      i_x_chpl44 = &call_tmp_chpl53;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      deinit_chpl95(i_x_chpl44, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      i_x_chpl45 = &call_tmp_chpl52;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      deinit_chpl95(i_x_chpl45, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      i_x_chpl46 = &call_tmp_chpl51;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      deinit_chpl95(i_x_chpl46, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      boundsCheckHalt_chpl(&msg_chpl5, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      i_x_chpl47 = &msg_chpl5;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      deinit_chpl95(i_x_chpl47, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    } else /* ZLINE: 280 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    if (p_chpl >= INT64(64)) /* ZLINE: 280 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    {
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl___COLON_24(UINT64(1), &ret_tmp_chpl48, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl60 = ret_tmp_chpl48;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl___PLUS_(&_str_literal_45_chpl, &call_tmp_chpl60, &ret_tmp_chpl49, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl61 = ret_tmp_chpl49;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl___PLUS_(&call_tmp_chpl61, &_str_literal_46_chpl, &ret_tmp_chpl50, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl62 = ret_tmp_chpl50;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl___COLON_25(p_chpl, &ret_tmp_chpl51, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl63 = ret_tmp_chpl51;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl___PLUS_(&call_tmp_chpl62, &call_tmp_chpl63, &ret_tmp_chpl52, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl64 = ret_tmp_chpl52;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl___PLUS_(&call_tmp_chpl64, &_str_literal_47_chpl, &ret_tmp_chpl53, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl65 = ret_tmp_chpl53;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl___COLON_25(p_chpl, &ret_tmp_chpl54, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl66 = ret_tmp_chpl54;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl___PLUS_(&call_tmp_chpl65, &call_tmp_chpl66, &ret_tmp_chpl55, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl67 = ret_tmp_chpl55;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl___PLUS_(&call_tmp_chpl67, &_str_literal_50_chpl, &ret_tmp_chpl56, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl68 = ret_tmp_chpl56;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl___PLUS_(&call_tmp_chpl68, &_str_literal_3965_chpl, &ret_tmp_chpl57, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl69 = ret_tmp_chpl57;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      msg_chpl6 = call_tmp_chpl69;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      i_x_chpl48 = &call_tmp_chpl68;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      deinit_chpl95(i_x_chpl48, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      i_x_chpl49 = &call_tmp_chpl67;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      deinit_chpl95(i_x_chpl49, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      i_x_chpl50 = &call_tmp_chpl66;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      deinit_chpl95(i_x_chpl50, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      i_x_chpl51 = &call_tmp_chpl65;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      deinit_chpl95(i_x_chpl51, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      i_x_chpl52 = &call_tmp_chpl64;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      deinit_chpl95(i_x_chpl52, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      i_x_chpl53 = &call_tmp_chpl63;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      deinit_chpl95(i_x_chpl53, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      i_x_chpl54 = &call_tmp_chpl62;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      deinit_chpl95(i_x_chpl54, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      i_x_chpl55 = &call_tmp_chpl61;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      deinit_chpl95(i_x_chpl55, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      i_x_chpl56 = &call_tmp_chpl60;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      deinit_chpl95(i_x_chpl56, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      boundsCheckHalt_chpl(&msg_chpl6, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      i_x_chpl57 = &msg_chpl6;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      deinit_chpl95(i_x_chpl57, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    }
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    if (UINT64(0) != ((uint64_t)((UINT64(1) << p_chpl)))) /* ZLINE: 280 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    {
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      halt_chpl15(&_str_literal_2423_chpl, UINT64(0), _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    }
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    call_tmp_chpl70 = sizeof(c_ptr_void_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    if (UINT64(0) < call_tmp_chpl70) /* ZLINE: 280 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    {
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      halt_chpl(&_str_literal_2424_chpl, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    }
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    call_tmp_chpl71 = offset_ARRAY_ELEMENTS_chpl();
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    call_tmp_chpl72 = chpl_here_aligned_alloc(UINT64(0), call_tmp_chpl7, call_tmp_chpl71, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    ptr_chpl = call_tmp_chpl72;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    {
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      inlineImm_chpl = INT64(0);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl_checkValue6(INT64(0), &ret_tmp_chpl58, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl73 = ret_tmp_chpl58;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      i_arg_chpl = &call_tmp_chpl73;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl74 = borrow_chpl2(i_arg_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      if (((RootClass_chpl)(call_tmp_chpl74)) != nil) /* ZLINE: 280 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      {
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        coerce_tmp_chpl4 = ((Error_chpl)(call_tmp_chpl74));
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        chpl_check_nil(coerce_tmp_chpl4, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        ((void(*)(Error_chpl,
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
           string_chpl *,
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
           int64_t,
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
           int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(coerce_tmp_chpl4))->chpl__cid) + INT64(1))])(coerce_tmp_chpl4, &ret_tmp_chpl59, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        call_tmp_chpl75 = ret_tmp_chpl59;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        safeCastCheckHalt_chpl(&call_tmp_chpl75, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        i_x_chpl58 = &call_tmp_chpl75;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        deinit_chpl95(i_x_chpl58, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      }
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl__autoDestroy62(&call_tmp_chpl73, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      memset(call_tmp_chpl72, ((int32_t)(inlineImm_chpl)), call_tmp_chpl7);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    }
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  }
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  call_tmp_chpl76 = ((c_ptr__owned_Error_chpl)(ptr_chpl));
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_check_nil(initTemp_chpl, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  (initTemp_chpl)->errorsArray = call_tmp_chpl76;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  ((RootClass_chpl)(initTemp_chpl))->chpl__cid = chpl__cid_TaskErrors_chpl;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  coerce_tmp_chpl5 = err_chpl8;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  tmp_chpl2 = ((Error_chpl)(nil));
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_check_nil(coerce_tmp_chpl5, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  (coerce_tmp_chpl5)->_next = tmp_chpl2;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_check_nil(initTemp_chpl, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  coerce_tmp_chpl6 = (initTemp_chpl)->errorsArray;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  inlineImm_chpl2 = INT64(0);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  call_tmp_chpl77 = (coerce_tmp_chpl6 + inlineImm_chpl2);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  i__retArg_chpl = &ret_tmp_chpl60;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  init_chpl188(&initTemp_chpl2, err_chpl8);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  call_tmp_chpl79 = initTemp_chpl2;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  ret_chpl = call_tmp_chpl79;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  *(i__retArg_chpl) = ret_chpl;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  call_tmp_chpl78 = ret_tmp_chpl60;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl___ASSIGN_11(call_tmp_chpl77, &call_tmp_chpl78, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl__autoDestroy57(&call_tmp_chpl78, _ln_chpl, _fn_chpl);
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  return initTemp_chpl;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
}

#line 294 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
/* Errors.chpl:294 */
#line 294 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
static void deinit_chpl96(TaskErrors_chpl this_chpl31,
#line 294 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                          int64_t _ln_chpl,
#line 294 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                          int32_t _fn_chpl) {
#line 296 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int64_t coerce_tmp_chpl3;
#line 296 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int64_t tmp_chpl;
#line 296 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int64_t _ic__F1_high_chpl;
#line 296 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int64_t i_chpl;
#line 297 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  c_ptr__owned_Error_chpl coerce_tmp_chpl4 = NULL;
#line 297 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref__owned_Error_chpl call_tmp_chpl6 = NULL;
#line 294 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  Error_chpl _parent_destructor_tmp__chpl = NULL;
#line 294 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl _field_destructor_tmp__chpl = NULL;
#line 295 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 295 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  if ((this_chpl31)->errorsArray != nil) /* ZLINE: 295 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 295 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  {
#line 296 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 296 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    coerce_tmp_chpl3 = (this_chpl31)->nErrors;
#line 296 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    if (coerce_tmp_chpl3 < INT64(0)) /* ZLINE: 296 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 296 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    {
#line 296 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      boundsCheckHalt_chpl(&_str_literal_580_chpl, _ln_chpl, _fn_chpl);
#line 296 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    }
#line 296 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    if (coerce_tmp_chpl3 == INT64(0)) /* ZLINE: 296 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 296 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    {
#line 296 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      tmp_chpl = ((int64_t)(((uint64_t)((((uint64_t)(INT64(0))) - UINT64(1))))));
#line 296 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    } else /* ZLINE: 296 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 296 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    {
#line 296 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      tmp_chpl = ((int64_t)((INT64(0) + ((int64_t)((coerce_tmp_chpl3 - INT64(1)))))));
#line 296 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    }
#line 296 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    _ic__F1_high_chpl = tmp_chpl;
#line 296 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    i_chpl = INT64(0);
#line 296 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    chpl_checkIfRangeIterWillOverflow(INT64(0), tmp_chpl, INT64(1), INT64(0), tmp_chpl, UINT8(true), _ln_chpl, _fn_chpl);
#line 297 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 297 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    coerce_tmp_chpl4 = (this_chpl31)->errorsArray;
#line 296 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    for (i_chpl = INT64(0); ((i_chpl <= _ic__F1_high_chpl)); i_chpl += INT64(1)) {
#line 297 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl6 = (coerce_tmp_chpl4 + i_chpl);
#line 297 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl___ASSIGN_13(call_tmp_chpl6, nil, _ln_chpl, _fn_chpl);
#line 297 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    }
#line 299 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 299 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    chpl_here_free(((c_ptr_void_chpl)((this_chpl31)->errorsArray)), _ln_chpl, _fn_chpl);
#line 299 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  }
#line 294 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _parent_destructor_tmp__chpl = ((Error_chpl)(this_chpl31));
#line 294 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_check_nil(_parent_destructor_tmp__chpl, _ln_chpl, _fn_chpl);
#line 294 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _field_destructor_tmp__chpl = &((_parent_destructor_tmp__chpl)->_msg);
#line 294 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  deinit_chpl95(_field_destructor_tmp__chpl, _ln_chpl, _fn_chpl);
#line 294 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  return;
#line 294 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
}

#line 345 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
/* Errors.chpl:345 */
#line 345 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
static void message_chpl6(TaskErrors_chpl this_chpl31,
#line 345 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                          string_chpl * _retArg_chpl,
#line 345 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                          int64_t _ln_chpl,
#line 345 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                          int32_t _fn_chpl) {
#line 345 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_chpl;
#line 346 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int64_t n_chpl;
#line 348 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl minMsg_chpl;
#line 349 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl maxMsg_chpl;
#line 350 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  Error_chpl first_chpl2 = NULL;
#line 351 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  Error_chpl last_chpl = NULL;
#line 353 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  TaskErrors_chpl _ic__F0_this_chpl = NULL;
#line 353 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  TaskErrors_chpl this_chpl32 = NULL;
#line 353 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int64_t coerce_tmp_chpl3;
#line 353 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int64_t tmp_chpl;
#line 353 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int64_t _ic__F1_high_chpl;
#line 353 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int64_t i_chpl;
#line 353 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  TaskErrors_chpl this_chpl33 = NULL;
#line 353 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  c_ptr__owned_Error_chpl coerce_tmp_chpl4 = NULL;
#line 353 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref__owned_Error_chpl call_tmp_chpl6 = NULL;
#line 353 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  Error_chpl coerce_tmp_chpl5 = NULL;
#line 353 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  TaskErrors_chpl this_chpl34 = NULL;
#line 353 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  c_ptr__owned_Error_chpl coerce_tmp_chpl6 = NULL;
#line 353 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref__owned_Error_chpl call_tmp_chpl7 = NULL;
#line 354 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  Error_chpl coerce_tmp_chpl7 = NULL;
#line 354 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  Error_chpl e_chpl = NULL;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_bool tmp_chpl2;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_bool tmp_chpl3;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl8;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  c_ptr_uint8_t_chpl coerce_tmp_chpl8 = NULL;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int64_t coerce_tmp_chpl9;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int32_t coerce_tmp_chpl10;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  c_ptr_uint8_t_chpl coerce_tmp_chpl11 = NULL;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int64_t coerce_tmp_chpl12;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int32_t coerce_tmp_chpl13;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int64_t ret_chpl2;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int32_t call_tmp_chpl9;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_bool tmp_chpl4;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int32_t call_tmp_chpl10;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int64_t ret_chpl3;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int64_t tmp_chpl5;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int64_t size_chpl9;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _owned_IllegalArgumentError_chpl2 call_tmp_chpl11;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _owned_IllegalArgumentError_chpl2 ret_tmp_chpl2;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref__owned_IllegalArgumentError_chpl i_arg_chpl = NULL;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  IllegalArgumentError_chpl call_tmp_chpl12 = NULL;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl13;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  Error_chpl coerce_tmp_chpl14 = NULL;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl3;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int32_t call_tmp_chpl14;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _owned_IllegalArgumentError_chpl2 call_tmp_chpl15;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _owned_IllegalArgumentError_chpl2 ret_tmp_chpl4;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref__owned_IllegalArgumentError_chpl i_arg_chpl2 = NULL;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  IllegalArgumentError_chpl call_tmp_chpl16 = NULL;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl17;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  Error_chpl coerce_tmp_chpl15 = NULL;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl5;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl2 = NULL;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int64_t call_tmp_chpl18;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int32_t call_tmp_chpl19;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_bool tmp_chpl6;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int32_t call_tmp_chpl20;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  c_ptr_uint8_t_chpl call_tmp_chpl21 = NULL;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int64_t ret_chpl4;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int64_t tmp_chpl7;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int64_t size_chpl10;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _owned_IllegalArgumentError_chpl2 call_tmp_chpl22;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _owned_IllegalArgumentError_chpl2 ret_tmp_chpl6;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref__owned_IllegalArgumentError_chpl i_arg_chpl3 = NULL;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  IllegalArgumentError_chpl call_tmp_chpl23 = NULL;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl24;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  Error_chpl coerce_tmp_chpl16 = NULL;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl7;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl3 = NULL;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int32_t call_tmp_chpl25;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _owned_IllegalArgumentError_chpl2 call_tmp_chpl26;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _owned_IllegalArgumentError_chpl2 ret_tmp_chpl8;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref__owned_IllegalArgumentError_chpl i_arg_chpl4 = NULL;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  IllegalArgumentError_chpl call_tmp_chpl27 = NULL;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl28;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  Error_chpl coerce_tmp_chpl17 = NULL;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl9;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl4 = NULL;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int64_t call_tmp_chpl29;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int32_t call_tmp_chpl30;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_bool tmp_chpl8;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int32_t call_tmp_chpl31;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  c_ptr_uint8_t_chpl call_tmp_chpl32 = NULL;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int64_t ret_chpl5;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int64_t tmp_chpl9;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int64_t size_chpl11;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _owned_IllegalArgumentError_chpl2 call_tmp_chpl33;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _owned_IllegalArgumentError_chpl2 ret_tmp_chpl10;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref__owned_IllegalArgumentError_chpl i_arg_chpl5 = NULL;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  IllegalArgumentError_chpl call_tmp_chpl34 = NULL;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl35;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  Error_chpl coerce_tmp_chpl18 = NULL;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl11;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl5 = NULL;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int32_t call_tmp_chpl36;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _owned_IllegalArgumentError_chpl2 call_tmp_chpl37;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _owned_IllegalArgumentError_chpl2 ret_tmp_chpl12;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref__owned_IllegalArgumentError_chpl i_arg_chpl6 = NULL;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  IllegalArgumentError_chpl call_tmp_chpl38 = NULL;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl39;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  Error_chpl coerce_tmp_chpl19 = NULL;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl13;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl6 = NULL;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int64_t call_tmp_chpl40;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  c_ptr_uint8_t_chpl call_tmp_chpl41 = NULL;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  c_ptr_uint8_t_chpl call_tmp_chpl42 = NULL;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int64_t ret_chpl6;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int64_t tmp_chpl10;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int64_t size_chpl12;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _owned_IllegalArgumentError_chpl2 call_tmp_chpl43;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _owned_IllegalArgumentError_chpl2 ret_tmp_chpl14;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref__owned_IllegalArgumentError_chpl i_arg_chpl7 = NULL;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  IllegalArgumentError_chpl call_tmp_chpl44 = NULL;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl45;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  Error_chpl coerce_tmp_chpl20 = NULL;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl15;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl7 = NULL;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int32_t call_tmp_chpl46;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _owned_IllegalArgumentError_chpl2 call_tmp_chpl47;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _owned_IllegalArgumentError_chpl2 ret_tmp_chpl16;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref__owned_IllegalArgumentError_chpl i_arg_chpl8 = NULL;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  IllegalArgumentError_chpl call_tmp_chpl48 = NULL;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl49;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  Error_chpl coerce_tmp_chpl21 = NULL;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl17;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl8 = NULL;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int64_t call_tmp_chpl50;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl9 = NULL;
#line 356 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl51;
#line 356 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl18;
#line 356 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl10 = NULL;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_bool tmp_chpl11;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_bool tmp_chpl12;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl52;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl19;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  c_ptr_uint8_t_chpl coerce_tmp_chpl22 = NULL;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int64_t coerce_tmp_chpl23;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int32_t coerce_tmp_chpl24;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  c_ptr_uint8_t_chpl coerce_tmp_chpl25 = NULL;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int64_t coerce_tmp_chpl26;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int32_t coerce_tmp_chpl27;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int64_t ret_chpl7;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int32_t call_tmp_chpl53;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_bool tmp_chpl13;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int32_t call_tmp_chpl54;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int64_t ret_chpl8;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int64_t tmp_chpl14;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int64_t size_chpl13;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _owned_IllegalArgumentError_chpl2 call_tmp_chpl55;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _owned_IllegalArgumentError_chpl2 ret_tmp_chpl20;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref__owned_IllegalArgumentError_chpl i_arg_chpl9 = NULL;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  IllegalArgumentError_chpl call_tmp_chpl56 = NULL;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl57;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  Error_chpl coerce_tmp_chpl28 = NULL;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl21;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl11 = NULL;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int32_t call_tmp_chpl58;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _owned_IllegalArgumentError_chpl2 call_tmp_chpl59;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _owned_IllegalArgumentError_chpl2 ret_tmp_chpl22;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref__owned_IllegalArgumentError_chpl i_arg_chpl10 = NULL;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  IllegalArgumentError_chpl call_tmp_chpl60 = NULL;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl61;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  Error_chpl coerce_tmp_chpl29 = NULL;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl23;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl12 = NULL;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int64_t call_tmp_chpl62;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int32_t call_tmp_chpl63;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_bool tmp_chpl15;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int32_t call_tmp_chpl64;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  c_ptr_uint8_t_chpl call_tmp_chpl65 = NULL;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int64_t ret_chpl9;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int64_t tmp_chpl16;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int64_t size_chpl14;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _owned_IllegalArgumentError_chpl2 call_tmp_chpl66;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _owned_IllegalArgumentError_chpl2 ret_tmp_chpl24;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref__owned_IllegalArgumentError_chpl i_arg_chpl11 = NULL;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  IllegalArgumentError_chpl call_tmp_chpl67 = NULL;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl68;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  Error_chpl coerce_tmp_chpl30 = NULL;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl25;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl13 = NULL;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int32_t call_tmp_chpl69;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _owned_IllegalArgumentError_chpl2 call_tmp_chpl70;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _owned_IllegalArgumentError_chpl2 ret_tmp_chpl26;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref__owned_IllegalArgumentError_chpl i_arg_chpl12 = NULL;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  IllegalArgumentError_chpl call_tmp_chpl71 = NULL;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl72;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  Error_chpl coerce_tmp_chpl31 = NULL;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl27;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl14 = NULL;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int64_t call_tmp_chpl73;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int32_t call_tmp_chpl74;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_bool tmp_chpl17;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int32_t call_tmp_chpl75;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  c_ptr_uint8_t_chpl call_tmp_chpl76 = NULL;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int64_t ret_chpl10;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int64_t tmp_chpl18;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int64_t size_chpl15;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _owned_IllegalArgumentError_chpl2 call_tmp_chpl77;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _owned_IllegalArgumentError_chpl2 ret_tmp_chpl28;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref__owned_IllegalArgumentError_chpl i_arg_chpl13 = NULL;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  IllegalArgumentError_chpl call_tmp_chpl78 = NULL;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl79;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  Error_chpl coerce_tmp_chpl32 = NULL;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl29;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl15 = NULL;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int32_t call_tmp_chpl80;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _owned_IllegalArgumentError_chpl2 call_tmp_chpl81;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _owned_IllegalArgumentError_chpl2 ret_tmp_chpl30;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref__owned_IllegalArgumentError_chpl i_arg_chpl14 = NULL;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  IllegalArgumentError_chpl call_tmp_chpl82 = NULL;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl83;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  Error_chpl coerce_tmp_chpl33 = NULL;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl31;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl16 = NULL;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int64_t call_tmp_chpl84;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  c_ptr_uint8_t_chpl call_tmp_chpl85 = NULL;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  c_ptr_uint8_t_chpl call_tmp_chpl86 = NULL;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int64_t ret_chpl11;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int64_t tmp_chpl19;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int64_t size_chpl16;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _owned_IllegalArgumentError_chpl2 call_tmp_chpl87;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _owned_IllegalArgumentError_chpl2 ret_tmp_chpl32;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref__owned_IllegalArgumentError_chpl i_arg_chpl15 = NULL;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  IllegalArgumentError_chpl call_tmp_chpl88 = NULL;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl89;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  Error_chpl coerce_tmp_chpl34 = NULL;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl33;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl17 = NULL;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int32_t call_tmp_chpl90;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _owned_IllegalArgumentError_chpl2 call_tmp_chpl91;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _owned_IllegalArgumentError_chpl2 ret_tmp_chpl34;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref__owned_IllegalArgumentError_chpl i_arg_chpl16 = NULL;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  IllegalArgumentError_chpl call_tmp_chpl92 = NULL;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl93;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  Error_chpl coerce_tmp_chpl35 = NULL;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl35;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl18 = NULL;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int64_t call_tmp_chpl94;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl19 = NULL;
#line 358 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl95;
#line 358 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl36;
#line 358 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl20 = NULL;
#line 360 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_int64_t_chpl i_lhs_chpl = NULL;
#line 365 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  TaskErrors_chpl _ic__F0_this_chpl2 = NULL;
#line 365 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  TaskErrors_chpl this_chpl35 = NULL;
#line 365 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int64_t coerce_tmp_chpl36;
#line 365 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int64_t tmp_chpl20;
#line 365 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int64_t _ic__F1_high_chpl2;
#line 365 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int64_t i_chpl2;
#line 365 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  TaskErrors_chpl this_chpl36 = NULL;
#line 365 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  c_ptr__owned_Error_chpl coerce_tmp_chpl37 = NULL;
#line 365 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref__owned_Error_chpl call_tmp_chpl96 = NULL;
#line 365 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  Error_chpl coerce_tmp_chpl38 = NULL;
#line 365 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  TaskErrors_chpl this_chpl37 = NULL;
#line 365 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  c_ptr__owned_Error_chpl coerce_tmp_chpl39 = NULL;
#line 365 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref__owned_Error_chpl call_tmp_chpl97 = NULL;
#line 366 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  Error_chpl coerce_tmp_chpl40 = NULL;
#line 366 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  Error_chpl e_chpl2 = NULL;
#line 367 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl98;
#line 367 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl37;
#line 367 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_bool call_tmp_chpl99;
#line 367 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl21 = NULL;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  c_ptr_uint8_t_chpl coerce_tmp_chpl41 = NULL;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int64_t coerce_tmp_chpl42;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int32_t coerce_tmp_chpl43;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  c_ptr_uint8_t_chpl coerce_tmp_chpl44 = NULL;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int64_t coerce_tmp_chpl45;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int32_t coerce_tmp_chpl46;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int64_t ret_chpl12;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int32_t call_tmp_chpl100;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_bool tmp_chpl21;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int32_t call_tmp_chpl101;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int64_t ret_chpl13;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int64_t tmp_chpl22;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int64_t size_chpl17;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _owned_IllegalArgumentError_chpl2 call_tmp_chpl102;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _owned_IllegalArgumentError_chpl2 ret_tmp_chpl38;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref__owned_IllegalArgumentError_chpl i_arg_chpl17 = NULL;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  IllegalArgumentError_chpl call_tmp_chpl103 = NULL;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl104;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  Error_chpl coerce_tmp_chpl47 = NULL;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl39;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl22 = NULL;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int32_t call_tmp_chpl105;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _owned_IllegalArgumentError_chpl2 call_tmp_chpl106;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _owned_IllegalArgumentError_chpl2 ret_tmp_chpl40;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref__owned_IllegalArgumentError_chpl i_arg_chpl18 = NULL;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  IllegalArgumentError_chpl call_tmp_chpl107 = NULL;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl108;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  Error_chpl coerce_tmp_chpl48 = NULL;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl41;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl23 = NULL;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int64_t call_tmp_chpl109;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int32_t call_tmp_chpl110;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_bool tmp_chpl23;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int32_t call_tmp_chpl111;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  c_ptr_uint8_t_chpl call_tmp_chpl112 = NULL;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int64_t ret_chpl14;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int64_t tmp_chpl24;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int64_t size_chpl18;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _owned_IllegalArgumentError_chpl2 call_tmp_chpl113;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _owned_IllegalArgumentError_chpl2 ret_tmp_chpl42;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref__owned_IllegalArgumentError_chpl i_arg_chpl19 = NULL;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  IllegalArgumentError_chpl call_tmp_chpl114 = NULL;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl115;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  Error_chpl coerce_tmp_chpl49 = NULL;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl43;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl24 = NULL;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int32_t call_tmp_chpl116;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _owned_IllegalArgumentError_chpl2 call_tmp_chpl117;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _owned_IllegalArgumentError_chpl2 ret_tmp_chpl44;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref__owned_IllegalArgumentError_chpl i_arg_chpl20 = NULL;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  IllegalArgumentError_chpl call_tmp_chpl118 = NULL;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl119;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  Error_chpl coerce_tmp_chpl50 = NULL;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl45;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl25 = NULL;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int64_t call_tmp_chpl120;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int32_t call_tmp_chpl121;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_bool tmp_chpl25;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int32_t call_tmp_chpl122;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  c_ptr_uint8_t_chpl call_tmp_chpl123 = NULL;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int64_t ret_chpl15;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int64_t tmp_chpl26;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int64_t size_chpl19;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _owned_IllegalArgumentError_chpl2 call_tmp_chpl124;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _owned_IllegalArgumentError_chpl2 ret_tmp_chpl46;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref__owned_IllegalArgumentError_chpl i_arg_chpl21 = NULL;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  IllegalArgumentError_chpl call_tmp_chpl125 = NULL;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl126;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  Error_chpl coerce_tmp_chpl51 = NULL;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl47;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl26 = NULL;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int32_t call_tmp_chpl127;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _owned_IllegalArgumentError_chpl2 call_tmp_chpl128;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _owned_IllegalArgumentError_chpl2 ret_tmp_chpl48;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref__owned_IllegalArgumentError_chpl i_arg_chpl22 = NULL;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  IllegalArgumentError_chpl call_tmp_chpl129 = NULL;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl130;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  Error_chpl coerce_tmp_chpl52 = NULL;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl49;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl27 = NULL;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int64_t call_tmp_chpl131;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  c_ptr_uint8_t_chpl call_tmp_chpl132 = NULL;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  c_ptr_uint8_t_chpl call_tmp_chpl133 = NULL;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int64_t ret_chpl16;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int64_t tmp_chpl27;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int64_t size_chpl20;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _owned_IllegalArgumentError_chpl2 call_tmp_chpl134;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _owned_IllegalArgumentError_chpl2 ret_tmp_chpl50;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref__owned_IllegalArgumentError_chpl i_arg_chpl23 = NULL;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  IllegalArgumentError_chpl call_tmp_chpl135 = NULL;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl136;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  Error_chpl coerce_tmp_chpl53 = NULL;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl51;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl28 = NULL;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int32_t call_tmp_chpl137;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _owned_IllegalArgumentError_chpl2 call_tmp_chpl138;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _owned_IllegalArgumentError_chpl2 ret_tmp_chpl52;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref__owned_IllegalArgumentError_chpl i_arg_chpl24 = NULL;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  IllegalArgumentError_chpl call_tmp_chpl139 = NULL;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl140;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  Error_chpl coerce_tmp_chpl54 = NULL;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl53;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl29 = NULL;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int64_t call_tmp_chpl141;
#line 374 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  TaskErrors_chpl _ic__F0_this_chpl3 = NULL;
#line 374 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  TaskErrors_chpl this_chpl38 = NULL;
#line 374 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int64_t coerce_tmp_chpl55;
#line 374 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int64_t tmp_chpl28;
#line 374 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int64_t _ic__F1_high_chpl3;
#line 374 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int64_t i_chpl3;
#line 374 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  TaskErrors_chpl this_chpl39 = NULL;
#line 374 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  c_ptr__owned_Error_chpl coerce_tmp_chpl56 = NULL;
#line 374 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref__owned_Error_chpl call_tmp_chpl142 = NULL;
#line 374 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  Error_chpl coerce_tmp_chpl57 = NULL;
#line 374 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  TaskErrors_chpl this_chpl40 = NULL;
#line 374 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  c_ptr__owned_Error_chpl coerce_tmp_chpl58 = NULL;
#line 374 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref__owned_Error_chpl call_tmp_chpl143 = NULL;
#line 375 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  Error_chpl coerce_tmp_chpl59 = NULL;
#line 375 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  Error_chpl e_chpl3 = NULL;
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl144;
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl54;
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_bool call_tmp_chpl145;
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl30 = NULL;
#line 383 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl146;
#line 383 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl55;
#line 383 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl147;
#line 383 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl56;
#line 383 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_chpl17;
#line 383 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl31 = NULL;
#line 385 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_bool tmp_chpl29;
#line 385 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_bool tmp_chpl30;
#line 386 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl148;
#line 386 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl57;
#line 386 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl149;
#line 386 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl58;
#line 386 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl150;
#line 386 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl59;
#line 386 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl151;
#line 386 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl60;
#line 386 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl32 = NULL;
#line 386 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl33 = NULL;
#line 386 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl34 = NULL;
#line 386 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl35 = NULL;
#line 388 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl152;
#line 388 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl61;
#line 388 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl36 = NULL;
#line 390 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl37 = NULL;
#line 390 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl38 = NULL;
#line 346 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  n_chpl = INT64(0);
#line 348 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  init_chpl194(&minMsg_chpl, _ln_chpl, _fn_chpl);
#line 349 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  init_chpl194(&maxMsg_chpl, _ln_chpl, _fn_chpl);
#line 350 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  first_chpl2 = ((Error_chpl)(nil));
#line 351 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  last_chpl = ((Error_chpl)(nil));
#line 353 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ic__F0_this_chpl = this_chpl31;
#line 353 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  this_chpl32 = this_chpl31;
#line 353 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_check_nil(this_chpl32, _ln_chpl, _fn_chpl);
#line 353 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  coerce_tmp_chpl3 = (this_chpl32)->nErrors;
#line 353 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  if (coerce_tmp_chpl3 < INT64(0)) /* ZLINE: 353 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 353 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  {
#line 353 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    boundsCheckHalt_chpl(&_str_literal_580_chpl, _ln_chpl, _fn_chpl);
#line 353 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  }
#line 353 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  if (coerce_tmp_chpl3 == INT64(0)) /* ZLINE: 353 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 353 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  {
#line 353 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    tmp_chpl = ((int64_t)(((uint64_t)((((uint64_t)(INT64(0))) - UINT64(1))))));
#line 353 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  } else /* ZLINE: 353 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 353 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  {
#line 353 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    tmp_chpl = ((int64_t)((INT64(0) + ((int64_t)((coerce_tmp_chpl3 - INT64(1)))))));
#line 353 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  }
#line 353 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ic__F1_high_chpl = tmp_chpl;
#line 353 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  i_chpl = INT64(0);
#line 353 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_checkIfRangeIterWillOverflow(INT64(0), tmp_chpl, INT64(1), INT64(0), tmp_chpl, UINT8(true), _ln_chpl, _fn_chpl);
#line 353 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  this_chpl33 = _ic__F0_this_chpl;
#line 353 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_check_nil(this_chpl33, _ln_chpl, _fn_chpl);
#line 353 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  coerce_tmp_chpl4 = (this_chpl33)->errorsArray;
#line 353 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  for (i_chpl = INT64(0); ((i_chpl <= _ic__F1_high_chpl)); i_chpl += INT64(1)) {
#line 353 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    call_tmp_chpl6 = (coerce_tmp_chpl4 + i_chpl);
#line 353 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    coerce_tmp_chpl5 = borrow_chpl3(call_tmp_chpl6);
#line 353 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    if (((RootClass_chpl)(coerce_tmp_chpl5)) != nil) /* ZLINE: 353 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 353 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    {
#line 353 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      this_chpl34 = _ic__F0_this_chpl;
#line 353 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl_check_nil(this_chpl34, _ln_chpl, _fn_chpl);
#line 353 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      coerce_tmp_chpl6 = (this_chpl34)->errorsArray;
#line 353 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl7 = (coerce_tmp_chpl6 + i_chpl);
#line 354 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      if (((RootClass_chpl)((call_tmp_chpl7)->chpl_p)) == nil) /* ZLINE: 354 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 354 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      {
#line 354 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        nilCheckHalt_chpl(&_str_literal_52_chpl, _ln_chpl, _fn_chpl);
#line 354 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      }
#line 354 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      coerce_tmp_chpl7 = (call_tmp_chpl7)->chpl_p;
#line 354 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      e_chpl = coerce_tmp_chpl7;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      tmp_chpl2 = chpl___EQUALS_3(&minMsg_chpl, &_str_literal_163_chpl, _ln_chpl, _fn_chpl);
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      if (tmp_chpl2) /* ZLINE: 355 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      {
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        tmp_chpl3 = UINT8(true);
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      } else /* ZLINE: 355 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      {
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        chpl_check_nil(coerce_tmp_chpl7, _ln_chpl, _fn_chpl);
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        ((void(*)(Error_chpl,
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
           string_chpl *,
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
           int64_t,
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
           int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(coerce_tmp_chpl7))->chpl__cid) + INT64(1))])(coerce_tmp_chpl7, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        call_tmp_chpl8 = ret_tmp_chpl;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        coerce_tmp_chpl8 = (&call_tmp_chpl8)->buff;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        coerce_tmp_chpl9 = (&call_tmp_chpl8)->buffLen;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        coerce_tmp_chpl10 = (&call_tmp_chpl8)->locale_id;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        coerce_tmp_chpl11 = (&minMsg_chpl)->buff;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        coerce_tmp_chpl12 = (&minMsg_chpl)->buffLen;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        coerce_tmp_chpl13 = (&minMsg_chpl)->locale_id;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        call_tmp_chpl9 = get_chpl_nodeID();
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        if (coerce_tmp_chpl10 == call_tmp_chpl9) /* ZLINE: 355 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        {
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          call_tmp_chpl10 = get_chpl_nodeID();
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          tmp_chpl4 = (coerce_tmp_chpl13 == call_tmp_chpl10);
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        } else /* ZLINE: 355 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        {
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          tmp_chpl4 = UINT8(false);
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        }
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        if (tmp_chpl4) /* ZLINE: 355 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        {
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          if (coerce_tmp_chpl9 < coerce_tmp_chpl12) /* ZLINE: 355 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          {
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            tmp_chpl5 = coerce_tmp_chpl9;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          } else /* ZLINE: 355 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          {
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            tmp_chpl5 = coerce_tmp_chpl12;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          }
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          size_chpl9 = tmp_chpl5;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          chpl_checkValue7(tmp_chpl5, &ret_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          call_tmp_chpl11 = ret_tmp_chpl2;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          i_arg_chpl = &call_tmp_chpl11;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          call_tmp_chpl12 = borrow_chpl2(i_arg_chpl);
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          if (((RootClass_chpl)(call_tmp_chpl12)) != nil) /* ZLINE: 355 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          {
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            coerce_tmp_chpl14 = ((Error_chpl)(call_tmp_chpl12));
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            chpl_check_nil(coerce_tmp_chpl14, _ln_chpl, _fn_chpl);
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            ((void(*)(Error_chpl,
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
               string_chpl *,
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
               int64_t,
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
               int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(coerce_tmp_chpl14))->chpl__cid) + INT64(1))])(coerce_tmp_chpl14, &ret_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            call_tmp_chpl13 = ret_tmp_chpl3;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            safeCastCheckHalt_chpl(&call_tmp_chpl13, _ln_chpl, _fn_chpl);
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            i_x_chpl = &call_tmp_chpl13;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            deinit_chpl95(i_x_chpl, _ln_chpl, _fn_chpl);
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          }
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          chpl__autoDestroy62(&call_tmp_chpl11, _ln_chpl, _fn_chpl);
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          call_tmp_chpl14 = memcmp(((c_ptr_void_chpl)(coerce_tmp_chpl8)), ((c_ptr_void_chpl)(coerce_tmp_chpl11)), ((uint64_t)(size_chpl9)));
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          chpl_checkValue4(call_tmp_chpl14, &ret_tmp_chpl4, _ln_chpl, _fn_chpl);
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          call_tmp_chpl15 = ret_tmp_chpl4;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          i_arg_chpl2 = &call_tmp_chpl15;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          call_tmp_chpl16 = borrow_chpl2(i_arg_chpl2);
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          if (((RootClass_chpl)(call_tmp_chpl16)) != nil) /* ZLINE: 355 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          {
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            coerce_tmp_chpl15 = ((Error_chpl)(call_tmp_chpl16));
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            chpl_check_nil(coerce_tmp_chpl15, _ln_chpl, _fn_chpl);
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            ((void(*)(Error_chpl,
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
               string_chpl *,
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
               int64_t,
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
               int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(coerce_tmp_chpl15))->chpl__cid) + INT64(1))])(coerce_tmp_chpl15, &ret_tmp_chpl5, _ln_chpl, _fn_chpl);
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            call_tmp_chpl17 = ret_tmp_chpl5;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            safeCastCheckHalt_chpl(&call_tmp_chpl17, _ln_chpl, _fn_chpl);
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            i_x_chpl2 = &call_tmp_chpl17;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            deinit_chpl95(i_x_chpl2, _ln_chpl, _fn_chpl);
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          }
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          chpl__autoDestroy62(&call_tmp_chpl15, _ln_chpl, _fn_chpl);
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          call_tmp_chpl18 = ((int64_t)(call_tmp_chpl14));
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          if (call_tmp_chpl18 == INT64(0)) /* ZLINE: 355 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          {
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            if (size_chpl9 < coerce_tmp_chpl9) /* ZLINE: 355 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            {
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              ret_chpl3 = INT64(1);
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              goto _end__strcmp_local_chpl;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            }
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            if (size_chpl9 < coerce_tmp_chpl12) /* ZLINE: 355 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            {
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              ret_chpl3 = INT64(-1);
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              goto _end__strcmp_local_chpl;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            }
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          }
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          ret_chpl3 = call_tmp_chpl18;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          _end__strcmp_local_chpl:;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          ret_chpl2 = ret_chpl3;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          goto _end__strcmp_chpl;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        } else /* ZLINE: 355 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        {
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          call_tmp_chpl19 = get_chpl_nodeID();
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          if (coerce_tmp_chpl10 != call_tmp_chpl19) /* ZLINE: 355 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          {
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            call_tmp_chpl20 = get_chpl_nodeID();
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            tmp_chpl6 = (coerce_tmp_chpl13 == call_tmp_chpl20);
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          } else /* ZLINE: 355 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          {
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            tmp_chpl6 = UINT8(false);
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          }
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          if (tmp_chpl6) /* ZLINE: 355 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          {
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            call_tmp_chpl21 = bufferCopyRemote_chpl(((int64_t)(coerce_tmp_chpl10)), coerce_tmp_chpl8, coerce_tmp_chpl9, _ln_chpl, _fn_chpl);
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            if (coerce_tmp_chpl9 < coerce_tmp_chpl12) /* ZLINE: 355 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            {
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              tmp_chpl7 = coerce_tmp_chpl9;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            } else /* ZLINE: 355 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            {
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              tmp_chpl7 = coerce_tmp_chpl12;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            }
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            size_chpl10 = tmp_chpl7;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            chpl_checkValue7(tmp_chpl7, &ret_tmp_chpl6, _ln_chpl, _fn_chpl);
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            call_tmp_chpl22 = ret_tmp_chpl6;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            i_arg_chpl3 = &call_tmp_chpl22;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            call_tmp_chpl23 = borrow_chpl2(i_arg_chpl3);
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            if (((RootClass_chpl)(call_tmp_chpl23)) != nil) /* ZLINE: 355 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            {
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              coerce_tmp_chpl16 = ((Error_chpl)(call_tmp_chpl23));
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              chpl_check_nil(coerce_tmp_chpl16, _ln_chpl, _fn_chpl);
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              ((void(*)(Error_chpl,
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                 string_chpl *,
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                 int64_t,
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                 int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(coerce_tmp_chpl16))->chpl__cid) + INT64(1))])(coerce_tmp_chpl16, &ret_tmp_chpl7, _ln_chpl, _fn_chpl);
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              call_tmp_chpl24 = ret_tmp_chpl7;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              safeCastCheckHalt_chpl(&call_tmp_chpl24, _ln_chpl, _fn_chpl);
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              i_x_chpl3 = &call_tmp_chpl24;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              deinit_chpl95(i_x_chpl3, _ln_chpl, _fn_chpl);
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            }
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            chpl__autoDestroy62(&call_tmp_chpl22, _ln_chpl, _fn_chpl);
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            call_tmp_chpl25 = memcmp(((c_ptr_void_chpl)(call_tmp_chpl21)), ((c_ptr_void_chpl)(coerce_tmp_chpl11)), ((uint64_t)(size_chpl10)));
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            chpl_checkValue4(call_tmp_chpl25, &ret_tmp_chpl8, _ln_chpl, _fn_chpl);
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            call_tmp_chpl26 = ret_tmp_chpl8;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            i_arg_chpl4 = &call_tmp_chpl26;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            call_tmp_chpl27 = borrow_chpl2(i_arg_chpl4);
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            if (((RootClass_chpl)(call_tmp_chpl27)) != nil) /* ZLINE: 355 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            {
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              coerce_tmp_chpl17 = ((Error_chpl)(call_tmp_chpl27));
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              chpl_check_nil(coerce_tmp_chpl17, _ln_chpl, _fn_chpl);
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              ((void(*)(Error_chpl,
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                 string_chpl *,
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                 int64_t,
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                 int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(coerce_tmp_chpl17))->chpl__cid) + INT64(1))])(coerce_tmp_chpl17, &ret_tmp_chpl9, _ln_chpl, _fn_chpl);
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              call_tmp_chpl28 = ret_tmp_chpl9;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              safeCastCheckHalt_chpl(&call_tmp_chpl28, _ln_chpl, _fn_chpl);
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              i_x_chpl4 = &call_tmp_chpl28;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              deinit_chpl95(i_x_chpl4, _ln_chpl, _fn_chpl);
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            }
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            chpl__autoDestroy62(&call_tmp_chpl26, _ln_chpl, _fn_chpl);
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            call_tmp_chpl29 = ((int64_t)(call_tmp_chpl25));
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            if (call_tmp_chpl29 == INT64(0)) /* ZLINE: 355 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            {
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              if (size_chpl10 < coerce_tmp_chpl9) /* ZLINE: 355 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              {
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                ret_chpl4 = INT64(1);
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                goto _end__strcmp_local_chpl2;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              }
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              if (size_chpl10 < coerce_tmp_chpl12) /* ZLINE: 355 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              {
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                ret_chpl4 = INT64(-1);
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                goto _end__strcmp_local_chpl2;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              }
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            }
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            ret_chpl4 = call_tmp_chpl29;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            _end__strcmp_local_chpl2:;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            chpl_here_free(((c_ptr_void_chpl)(call_tmp_chpl21)), _ln_chpl, _fn_chpl);
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            ret_chpl2 = ret_chpl4;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            goto _end__strcmp_chpl;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          } else /* ZLINE: 355 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          {
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            call_tmp_chpl30 = get_chpl_nodeID();
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            if (coerce_tmp_chpl10 == call_tmp_chpl30) /* ZLINE: 355 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            {
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              call_tmp_chpl31 = get_chpl_nodeID();
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              tmp_chpl8 = (coerce_tmp_chpl13 != call_tmp_chpl31);
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            } else /* ZLINE: 355 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            {
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              tmp_chpl8 = UINT8(false);
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            }
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            if (tmp_chpl8) /* ZLINE: 355 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            {
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              call_tmp_chpl32 = bufferCopyRemote_chpl(((int64_t)(coerce_tmp_chpl13)), coerce_tmp_chpl11, coerce_tmp_chpl12, _ln_chpl, _fn_chpl);
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              if (coerce_tmp_chpl9 < coerce_tmp_chpl12) /* ZLINE: 355 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              {
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                tmp_chpl9 = coerce_tmp_chpl9;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              } else /* ZLINE: 355 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              {
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                tmp_chpl9 = coerce_tmp_chpl12;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              }
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              size_chpl11 = tmp_chpl9;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              chpl_checkValue7(tmp_chpl9, &ret_tmp_chpl10, _ln_chpl, _fn_chpl);
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              call_tmp_chpl33 = ret_tmp_chpl10;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              i_arg_chpl5 = &call_tmp_chpl33;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              call_tmp_chpl34 = borrow_chpl2(i_arg_chpl5);
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              if (((RootClass_chpl)(call_tmp_chpl34)) != nil) /* ZLINE: 355 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              {
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                coerce_tmp_chpl18 = ((Error_chpl)(call_tmp_chpl34));
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                chpl_check_nil(coerce_tmp_chpl18, _ln_chpl, _fn_chpl);
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                ((void(*)(Error_chpl,
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                   string_chpl *,
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                   int64_t,
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                   int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(coerce_tmp_chpl18))->chpl__cid) + INT64(1))])(coerce_tmp_chpl18, &ret_tmp_chpl11, _ln_chpl, _fn_chpl);
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                call_tmp_chpl35 = ret_tmp_chpl11;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                safeCastCheckHalt_chpl(&call_tmp_chpl35, _ln_chpl, _fn_chpl);
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                i_x_chpl5 = &call_tmp_chpl35;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                deinit_chpl95(i_x_chpl5, _ln_chpl, _fn_chpl);
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              }
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              chpl__autoDestroy62(&call_tmp_chpl33, _ln_chpl, _fn_chpl);
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              call_tmp_chpl36 = memcmp(((c_ptr_void_chpl)(coerce_tmp_chpl8)), ((c_ptr_void_chpl)(call_tmp_chpl32)), ((uint64_t)(size_chpl11)));
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              chpl_checkValue4(call_tmp_chpl36, &ret_tmp_chpl12, _ln_chpl, _fn_chpl);
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              call_tmp_chpl37 = ret_tmp_chpl12;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              i_arg_chpl6 = &call_tmp_chpl37;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              call_tmp_chpl38 = borrow_chpl2(i_arg_chpl6);
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              if (((RootClass_chpl)(call_tmp_chpl38)) != nil) /* ZLINE: 355 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              {
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                coerce_tmp_chpl19 = ((Error_chpl)(call_tmp_chpl38));
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                chpl_check_nil(coerce_tmp_chpl19, _ln_chpl, _fn_chpl);
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                ((void(*)(Error_chpl,
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                   string_chpl *,
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                   int64_t,
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                   int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(coerce_tmp_chpl19))->chpl__cid) + INT64(1))])(coerce_tmp_chpl19, &ret_tmp_chpl13, _ln_chpl, _fn_chpl);
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                call_tmp_chpl39 = ret_tmp_chpl13;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                safeCastCheckHalt_chpl(&call_tmp_chpl39, _ln_chpl, _fn_chpl);
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                i_x_chpl6 = &call_tmp_chpl39;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                deinit_chpl95(i_x_chpl6, _ln_chpl, _fn_chpl);
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              }
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              chpl__autoDestroy62(&call_tmp_chpl37, _ln_chpl, _fn_chpl);
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              call_tmp_chpl40 = ((int64_t)(call_tmp_chpl36));
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              if (call_tmp_chpl40 == INT64(0)) /* ZLINE: 355 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              {
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                if (size_chpl11 < coerce_tmp_chpl9) /* ZLINE: 355 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                {
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                  ret_chpl5 = INT64(1);
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                  goto _end__strcmp_local_chpl3;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                }
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                if (size_chpl11 < coerce_tmp_chpl12) /* ZLINE: 355 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                {
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                  ret_chpl5 = INT64(-1);
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                  goto _end__strcmp_local_chpl3;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                }
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              }
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              ret_chpl5 = call_tmp_chpl40;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              _end__strcmp_local_chpl3:;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              chpl_here_free(((c_ptr_void_chpl)(call_tmp_chpl32)), _ln_chpl, _fn_chpl);
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              ret_chpl2 = ret_chpl5;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              goto _end__strcmp_chpl;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            } else /* ZLINE: 355 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            {
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              call_tmp_chpl41 = bufferCopyRemote_chpl(((int64_t)(coerce_tmp_chpl10)), coerce_tmp_chpl8, coerce_tmp_chpl9, _ln_chpl, _fn_chpl);
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              call_tmp_chpl42 = bufferCopyRemote_chpl(((int64_t)(coerce_tmp_chpl13)), coerce_tmp_chpl11, coerce_tmp_chpl12, _ln_chpl, _fn_chpl);
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              if (coerce_tmp_chpl9 < coerce_tmp_chpl12) /* ZLINE: 355 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              {
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                tmp_chpl10 = coerce_tmp_chpl9;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              } else /* ZLINE: 355 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              {
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                tmp_chpl10 = coerce_tmp_chpl12;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              }
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              size_chpl12 = tmp_chpl10;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              chpl_checkValue7(tmp_chpl10, &ret_tmp_chpl14, _ln_chpl, _fn_chpl);
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              call_tmp_chpl43 = ret_tmp_chpl14;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              i_arg_chpl7 = &call_tmp_chpl43;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              call_tmp_chpl44 = borrow_chpl2(i_arg_chpl7);
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              if (((RootClass_chpl)(call_tmp_chpl44)) != nil) /* ZLINE: 355 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              {
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                coerce_tmp_chpl20 = ((Error_chpl)(call_tmp_chpl44));
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                chpl_check_nil(coerce_tmp_chpl20, _ln_chpl, _fn_chpl);
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                ((void(*)(Error_chpl,
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                   string_chpl *,
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                   int64_t,
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                   int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(coerce_tmp_chpl20))->chpl__cid) + INT64(1))])(coerce_tmp_chpl20, &ret_tmp_chpl15, _ln_chpl, _fn_chpl);
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                call_tmp_chpl45 = ret_tmp_chpl15;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                safeCastCheckHalt_chpl(&call_tmp_chpl45, _ln_chpl, _fn_chpl);
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                i_x_chpl7 = &call_tmp_chpl45;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                deinit_chpl95(i_x_chpl7, _ln_chpl, _fn_chpl);
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              }
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              chpl__autoDestroy62(&call_tmp_chpl43, _ln_chpl, _fn_chpl);
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              call_tmp_chpl46 = memcmp(((c_ptr_void_chpl)(call_tmp_chpl41)), ((c_ptr_void_chpl)(call_tmp_chpl42)), ((uint64_t)(size_chpl12)));
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              chpl_checkValue4(call_tmp_chpl46, &ret_tmp_chpl16, _ln_chpl, _fn_chpl);
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              call_tmp_chpl47 = ret_tmp_chpl16;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              i_arg_chpl8 = &call_tmp_chpl47;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              call_tmp_chpl48 = borrow_chpl2(i_arg_chpl8);
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              if (((RootClass_chpl)(call_tmp_chpl48)) != nil) /* ZLINE: 355 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              {
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                coerce_tmp_chpl21 = ((Error_chpl)(call_tmp_chpl48));
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                chpl_check_nil(coerce_tmp_chpl21, _ln_chpl, _fn_chpl);
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                ((void(*)(Error_chpl,
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                   string_chpl *,
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                   int64_t,
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                   int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(coerce_tmp_chpl21))->chpl__cid) + INT64(1))])(coerce_tmp_chpl21, &ret_tmp_chpl17, _ln_chpl, _fn_chpl);
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                call_tmp_chpl49 = ret_tmp_chpl17;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                safeCastCheckHalt_chpl(&call_tmp_chpl49, _ln_chpl, _fn_chpl);
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                i_x_chpl8 = &call_tmp_chpl49;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                deinit_chpl95(i_x_chpl8, _ln_chpl, _fn_chpl);
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              }
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              chpl__autoDestroy62(&call_tmp_chpl47, _ln_chpl, _fn_chpl);
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              call_tmp_chpl50 = ((int64_t)(call_tmp_chpl46));
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              if (call_tmp_chpl50 == INT64(0)) /* ZLINE: 355 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              {
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                if (size_chpl12 < coerce_tmp_chpl9) /* ZLINE: 355 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                {
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                  ret_chpl6 = INT64(1);
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                  goto _end__strcmp_local_chpl4;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                }
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                if (size_chpl12 < coerce_tmp_chpl12) /* ZLINE: 355 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                {
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                  ret_chpl6 = INT64(-1);
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                  goto _end__strcmp_local_chpl4;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                }
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              }
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              ret_chpl6 = call_tmp_chpl50;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              _end__strcmp_local_chpl4:;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              chpl_here_free(((c_ptr_void_chpl)(call_tmp_chpl41)), _ln_chpl, _fn_chpl);
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              chpl_here_free(((c_ptr_void_chpl)(call_tmp_chpl42)), _ln_chpl, _fn_chpl);
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              ret_chpl2 = ret_chpl6;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              goto _end__strcmp_chpl;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            }
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          }
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        }
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        _end__strcmp_chpl:;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        tmp_chpl3 = (ret_chpl2 < INT64(0));
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        i_x_chpl9 = &call_tmp_chpl8;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        deinit_chpl95(i_x_chpl9, _ln_chpl, _fn_chpl);
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      }
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      if (tmp_chpl3) /* ZLINE: 355 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      {
#line 356 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        chpl_check_nil(e_chpl, _ln_chpl, _fn_chpl);
#line 356 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        ((void(*)(Error_chpl,
#line 356 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
           string_chpl *,
#line 356 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
           int64_t,
#line 356 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
           int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(e_chpl))->chpl__cid) + INT64(1))])(e_chpl, &ret_tmp_chpl18, _ln_chpl, _fn_chpl);
#line 356 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        call_tmp_chpl51 = ret_tmp_chpl18;
#line 356 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        chpl___ASSIGN_16(&minMsg_chpl, &call_tmp_chpl51, _ln_chpl, _fn_chpl);
#line 356 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        i_x_chpl10 = &call_tmp_chpl51;
#line 356 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        deinit_chpl95(i_x_chpl10, _ln_chpl, _fn_chpl);
#line 356 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      }
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      tmp_chpl11 = chpl___EQUALS_3(&maxMsg_chpl, &_str_literal_163_chpl, _ln_chpl, _fn_chpl);
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      if (tmp_chpl11) /* ZLINE: 357 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      {
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        tmp_chpl12 = UINT8(true);
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      } else /* ZLINE: 357 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      {
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        chpl_check_nil(e_chpl, _ln_chpl, _fn_chpl);
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        ((void(*)(Error_chpl,
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
           string_chpl *,
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
           int64_t,
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
           int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(e_chpl))->chpl__cid) + INT64(1))])(e_chpl, &ret_tmp_chpl19, _ln_chpl, _fn_chpl);
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        call_tmp_chpl52 = ret_tmp_chpl19;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        coerce_tmp_chpl22 = (&call_tmp_chpl52)->buff;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        coerce_tmp_chpl23 = (&call_tmp_chpl52)->buffLen;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        coerce_tmp_chpl24 = (&call_tmp_chpl52)->locale_id;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        coerce_tmp_chpl25 = (&maxMsg_chpl)->buff;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        coerce_tmp_chpl26 = (&maxMsg_chpl)->buffLen;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        coerce_tmp_chpl27 = (&maxMsg_chpl)->locale_id;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        call_tmp_chpl53 = get_chpl_nodeID();
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        if (coerce_tmp_chpl24 == call_tmp_chpl53) /* ZLINE: 357 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        {
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          call_tmp_chpl54 = get_chpl_nodeID();
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          tmp_chpl13 = (coerce_tmp_chpl27 == call_tmp_chpl54);
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        } else /* ZLINE: 357 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        {
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          tmp_chpl13 = UINT8(false);
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        }
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        if (tmp_chpl13) /* ZLINE: 357 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        {
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          if (coerce_tmp_chpl23 < coerce_tmp_chpl26) /* ZLINE: 357 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          {
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            tmp_chpl14 = coerce_tmp_chpl23;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          } else /* ZLINE: 357 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          {
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            tmp_chpl14 = coerce_tmp_chpl26;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          }
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          size_chpl13 = tmp_chpl14;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          chpl_checkValue7(tmp_chpl14, &ret_tmp_chpl20, _ln_chpl, _fn_chpl);
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          call_tmp_chpl55 = ret_tmp_chpl20;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          i_arg_chpl9 = &call_tmp_chpl55;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          call_tmp_chpl56 = borrow_chpl2(i_arg_chpl9);
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          if (((RootClass_chpl)(call_tmp_chpl56)) != nil) /* ZLINE: 357 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          {
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            coerce_tmp_chpl28 = ((Error_chpl)(call_tmp_chpl56));
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            chpl_check_nil(coerce_tmp_chpl28, _ln_chpl, _fn_chpl);
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            ((void(*)(Error_chpl,
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
               string_chpl *,
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
               int64_t,
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
               int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(coerce_tmp_chpl28))->chpl__cid) + INT64(1))])(coerce_tmp_chpl28, &ret_tmp_chpl21, _ln_chpl, _fn_chpl);
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            call_tmp_chpl57 = ret_tmp_chpl21;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            safeCastCheckHalt_chpl(&call_tmp_chpl57, _ln_chpl, _fn_chpl);
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            i_x_chpl11 = &call_tmp_chpl57;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            deinit_chpl95(i_x_chpl11, _ln_chpl, _fn_chpl);
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          }
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          chpl__autoDestroy62(&call_tmp_chpl55, _ln_chpl, _fn_chpl);
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          call_tmp_chpl58 = memcmp(((c_ptr_void_chpl)(coerce_tmp_chpl22)), ((c_ptr_void_chpl)(coerce_tmp_chpl25)), ((uint64_t)(size_chpl13)));
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          chpl_checkValue4(call_tmp_chpl58, &ret_tmp_chpl22, _ln_chpl, _fn_chpl);
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          call_tmp_chpl59 = ret_tmp_chpl22;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          i_arg_chpl10 = &call_tmp_chpl59;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          call_tmp_chpl60 = borrow_chpl2(i_arg_chpl10);
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          if (((RootClass_chpl)(call_tmp_chpl60)) != nil) /* ZLINE: 357 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          {
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            coerce_tmp_chpl29 = ((Error_chpl)(call_tmp_chpl60));
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            chpl_check_nil(coerce_tmp_chpl29, _ln_chpl, _fn_chpl);
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            ((void(*)(Error_chpl,
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
               string_chpl *,
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
               int64_t,
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
               int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(coerce_tmp_chpl29))->chpl__cid) + INT64(1))])(coerce_tmp_chpl29, &ret_tmp_chpl23, _ln_chpl, _fn_chpl);
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            call_tmp_chpl61 = ret_tmp_chpl23;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            safeCastCheckHalt_chpl(&call_tmp_chpl61, _ln_chpl, _fn_chpl);
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            i_x_chpl12 = &call_tmp_chpl61;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            deinit_chpl95(i_x_chpl12, _ln_chpl, _fn_chpl);
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          }
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          chpl__autoDestroy62(&call_tmp_chpl59, _ln_chpl, _fn_chpl);
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          call_tmp_chpl62 = ((int64_t)(call_tmp_chpl58));
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          if (call_tmp_chpl62 == INT64(0)) /* ZLINE: 357 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          {
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            if (size_chpl13 < coerce_tmp_chpl23) /* ZLINE: 357 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            {
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              ret_chpl8 = INT64(1);
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              goto _end__strcmp_local_chpl5;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            }
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            if (size_chpl13 < coerce_tmp_chpl26) /* ZLINE: 357 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            {
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              ret_chpl8 = INT64(-1);
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              goto _end__strcmp_local_chpl5;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            }
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          }
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          ret_chpl8 = call_tmp_chpl62;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          _end__strcmp_local_chpl5:;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          ret_chpl7 = ret_chpl8;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          goto _end__strcmp_chpl2;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        } else /* ZLINE: 357 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        {
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          call_tmp_chpl63 = get_chpl_nodeID();
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          if (coerce_tmp_chpl24 != call_tmp_chpl63) /* ZLINE: 357 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          {
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            call_tmp_chpl64 = get_chpl_nodeID();
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            tmp_chpl15 = (coerce_tmp_chpl27 == call_tmp_chpl64);
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          } else /* ZLINE: 357 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          {
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            tmp_chpl15 = UINT8(false);
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          }
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          if (tmp_chpl15) /* ZLINE: 357 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          {
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            call_tmp_chpl65 = bufferCopyRemote_chpl(((int64_t)(coerce_tmp_chpl24)), coerce_tmp_chpl22, coerce_tmp_chpl23, _ln_chpl, _fn_chpl);
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            if (coerce_tmp_chpl23 < coerce_tmp_chpl26) /* ZLINE: 357 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            {
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              tmp_chpl16 = coerce_tmp_chpl23;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            } else /* ZLINE: 357 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            {
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              tmp_chpl16 = coerce_tmp_chpl26;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            }
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            size_chpl14 = tmp_chpl16;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            chpl_checkValue7(tmp_chpl16, &ret_tmp_chpl24, _ln_chpl, _fn_chpl);
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            call_tmp_chpl66 = ret_tmp_chpl24;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            i_arg_chpl11 = &call_tmp_chpl66;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            call_tmp_chpl67 = borrow_chpl2(i_arg_chpl11);
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            if (((RootClass_chpl)(call_tmp_chpl67)) != nil) /* ZLINE: 357 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            {
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              coerce_tmp_chpl30 = ((Error_chpl)(call_tmp_chpl67));
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              chpl_check_nil(coerce_tmp_chpl30, _ln_chpl, _fn_chpl);
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              ((void(*)(Error_chpl,
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                 string_chpl *,
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                 int64_t,
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                 int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(coerce_tmp_chpl30))->chpl__cid) + INT64(1))])(coerce_tmp_chpl30, &ret_tmp_chpl25, _ln_chpl, _fn_chpl);
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              call_tmp_chpl68 = ret_tmp_chpl25;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              safeCastCheckHalt_chpl(&call_tmp_chpl68, _ln_chpl, _fn_chpl);
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              i_x_chpl13 = &call_tmp_chpl68;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              deinit_chpl95(i_x_chpl13, _ln_chpl, _fn_chpl);
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            }
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            chpl__autoDestroy62(&call_tmp_chpl66, _ln_chpl, _fn_chpl);
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            call_tmp_chpl69 = memcmp(((c_ptr_void_chpl)(call_tmp_chpl65)), ((c_ptr_void_chpl)(coerce_tmp_chpl25)), ((uint64_t)(size_chpl14)));
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            chpl_checkValue4(call_tmp_chpl69, &ret_tmp_chpl26, _ln_chpl, _fn_chpl);
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            call_tmp_chpl70 = ret_tmp_chpl26;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            i_arg_chpl12 = &call_tmp_chpl70;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            call_tmp_chpl71 = borrow_chpl2(i_arg_chpl12);
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            if (((RootClass_chpl)(call_tmp_chpl71)) != nil) /* ZLINE: 357 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            {
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              coerce_tmp_chpl31 = ((Error_chpl)(call_tmp_chpl71));
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              chpl_check_nil(coerce_tmp_chpl31, _ln_chpl, _fn_chpl);
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              ((void(*)(Error_chpl,
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                 string_chpl *,
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                 int64_t,
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                 int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(coerce_tmp_chpl31))->chpl__cid) + INT64(1))])(coerce_tmp_chpl31, &ret_tmp_chpl27, _ln_chpl, _fn_chpl);
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              call_tmp_chpl72 = ret_tmp_chpl27;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              safeCastCheckHalt_chpl(&call_tmp_chpl72, _ln_chpl, _fn_chpl);
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              i_x_chpl14 = &call_tmp_chpl72;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              deinit_chpl95(i_x_chpl14, _ln_chpl, _fn_chpl);
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            }
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            chpl__autoDestroy62(&call_tmp_chpl70, _ln_chpl, _fn_chpl);
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            call_tmp_chpl73 = ((int64_t)(call_tmp_chpl69));
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            if (call_tmp_chpl73 == INT64(0)) /* ZLINE: 357 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            {
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              if (size_chpl14 < coerce_tmp_chpl23) /* ZLINE: 357 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              {
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                ret_chpl9 = INT64(1);
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                goto _end__strcmp_local_chpl6;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              }
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              if (size_chpl14 < coerce_tmp_chpl26) /* ZLINE: 357 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              {
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                ret_chpl9 = INT64(-1);
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                goto _end__strcmp_local_chpl6;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              }
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            }
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            ret_chpl9 = call_tmp_chpl73;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            _end__strcmp_local_chpl6:;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            chpl_here_free(((c_ptr_void_chpl)(call_tmp_chpl65)), _ln_chpl, _fn_chpl);
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            ret_chpl7 = ret_chpl9;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            goto _end__strcmp_chpl2;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          } else /* ZLINE: 357 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          {
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            call_tmp_chpl74 = get_chpl_nodeID();
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            if (coerce_tmp_chpl24 == call_tmp_chpl74) /* ZLINE: 357 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            {
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              call_tmp_chpl75 = get_chpl_nodeID();
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              tmp_chpl17 = (coerce_tmp_chpl27 != call_tmp_chpl75);
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            } else /* ZLINE: 357 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            {
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              tmp_chpl17 = UINT8(false);
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            }
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            if (tmp_chpl17) /* ZLINE: 357 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            {
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              call_tmp_chpl76 = bufferCopyRemote_chpl(((int64_t)(coerce_tmp_chpl27)), coerce_tmp_chpl25, coerce_tmp_chpl26, _ln_chpl, _fn_chpl);
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              if (coerce_tmp_chpl23 < coerce_tmp_chpl26) /* ZLINE: 357 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              {
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                tmp_chpl18 = coerce_tmp_chpl23;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              } else /* ZLINE: 357 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              {
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                tmp_chpl18 = coerce_tmp_chpl26;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              }
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              size_chpl15 = tmp_chpl18;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              chpl_checkValue7(tmp_chpl18, &ret_tmp_chpl28, _ln_chpl, _fn_chpl);
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              call_tmp_chpl77 = ret_tmp_chpl28;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              i_arg_chpl13 = &call_tmp_chpl77;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              call_tmp_chpl78 = borrow_chpl2(i_arg_chpl13);
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              if (((RootClass_chpl)(call_tmp_chpl78)) != nil) /* ZLINE: 357 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              {
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                coerce_tmp_chpl32 = ((Error_chpl)(call_tmp_chpl78));
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                chpl_check_nil(coerce_tmp_chpl32, _ln_chpl, _fn_chpl);
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                ((void(*)(Error_chpl,
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                   string_chpl *,
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                   int64_t,
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                   int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(coerce_tmp_chpl32))->chpl__cid) + INT64(1))])(coerce_tmp_chpl32, &ret_tmp_chpl29, _ln_chpl, _fn_chpl);
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                call_tmp_chpl79 = ret_tmp_chpl29;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                safeCastCheckHalt_chpl(&call_tmp_chpl79, _ln_chpl, _fn_chpl);
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                i_x_chpl15 = &call_tmp_chpl79;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                deinit_chpl95(i_x_chpl15, _ln_chpl, _fn_chpl);
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              }
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              chpl__autoDestroy62(&call_tmp_chpl77, _ln_chpl, _fn_chpl);
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              call_tmp_chpl80 = memcmp(((c_ptr_void_chpl)(coerce_tmp_chpl22)), ((c_ptr_void_chpl)(call_tmp_chpl76)), ((uint64_t)(size_chpl15)));
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              chpl_checkValue4(call_tmp_chpl80, &ret_tmp_chpl30, _ln_chpl, _fn_chpl);
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              call_tmp_chpl81 = ret_tmp_chpl30;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              i_arg_chpl14 = &call_tmp_chpl81;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              call_tmp_chpl82 = borrow_chpl2(i_arg_chpl14);
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              if (((RootClass_chpl)(call_tmp_chpl82)) != nil) /* ZLINE: 357 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              {
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                coerce_tmp_chpl33 = ((Error_chpl)(call_tmp_chpl82));
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                chpl_check_nil(coerce_tmp_chpl33, _ln_chpl, _fn_chpl);
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                ((void(*)(Error_chpl,
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                   string_chpl *,
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                   int64_t,
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                   int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(coerce_tmp_chpl33))->chpl__cid) + INT64(1))])(coerce_tmp_chpl33, &ret_tmp_chpl31, _ln_chpl, _fn_chpl);
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                call_tmp_chpl83 = ret_tmp_chpl31;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                safeCastCheckHalt_chpl(&call_tmp_chpl83, _ln_chpl, _fn_chpl);
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                i_x_chpl16 = &call_tmp_chpl83;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                deinit_chpl95(i_x_chpl16, _ln_chpl, _fn_chpl);
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              }
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              chpl__autoDestroy62(&call_tmp_chpl81, _ln_chpl, _fn_chpl);
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              call_tmp_chpl84 = ((int64_t)(call_tmp_chpl80));
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              if (call_tmp_chpl84 == INT64(0)) /* ZLINE: 357 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              {
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                if (size_chpl15 < coerce_tmp_chpl23) /* ZLINE: 357 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                {
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                  ret_chpl10 = INT64(1);
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                  goto _end__strcmp_local_chpl7;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                }
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                if (size_chpl15 < coerce_tmp_chpl26) /* ZLINE: 357 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                {
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                  ret_chpl10 = INT64(-1);
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                  goto _end__strcmp_local_chpl7;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                }
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              }
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              ret_chpl10 = call_tmp_chpl84;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              _end__strcmp_local_chpl7:;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              chpl_here_free(((c_ptr_void_chpl)(call_tmp_chpl76)), _ln_chpl, _fn_chpl);
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              ret_chpl7 = ret_chpl10;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              goto _end__strcmp_chpl2;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            } else /* ZLINE: 357 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            {
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              call_tmp_chpl85 = bufferCopyRemote_chpl(((int64_t)(coerce_tmp_chpl24)), coerce_tmp_chpl22, coerce_tmp_chpl23, _ln_chpl, _fn_chpl);
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              call_tmp_chpl86 = bufferCopyRemote_chpl(((int64_t)(coerce_tmp_chpl27)), coerce_tmp_chpl25, coerce_tmp_chpl26, _ln_chpl, _fn_chpl);
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              if (coerce_tmp_chpl23 < coerce_tmp_chpl26) /* ZLINE: 357 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              {
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                tmp_chpl19 = coerce_tmp_chpl23;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              } else /* ZLINE: 357 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              {
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                tmp_chpl19 = coerce_tmp_chpl26;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              }
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              size_chpl16 = tmp_chpl19;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              chpl_checkValue7(tmp_chpl19, &ret_tmp_chpl32, _ln_chpl, _fn_chpl);
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              call_tmp_chpl87 = ret_tmp_chpl32;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              i_arg_chpl15 = &call_tmp_chpl87;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              call_tmp_chpl88 = borrow_chpl2(i_arg_chpl15);
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              if (((RootClass_chpl)(call_tmp_chpl88)) != nil) /* ZLINE: 357 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              {
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                coerce_tmp_chpl34 = ((Error_chpl)(call_tmp_chpl88));
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                chpl_check_nil(coerce_tmp_chpl34, _ln_chpl, _fn_chpl);
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                ((void(*)(Error_chpl,
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                   string_chpl *,
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                   int64_t,
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                   int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(coerce_tmp_chpl34))->chpl__cid) + INT64(1))])(coerce_tmp_chpl34, &ret_tmp_chpl33, _ln_chpl, _fn_chpl);
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                call_tmp_chpl89 = ret_tmp_chpl33;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                safeCastCheckHalt_chpl(&call_tmp_chpl89, _ln_chpl, _fn_chpl);
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                i_x_chpl17 = &call_tmp_chpl89;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                deinit_chpl95(i_x_chpl17, _ln_chpl, _fn_chpl);
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              }
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              chpl__autoDestroy62(&call_tmp_chpl87, _ln_chpl, _fn_chpl);
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              call_tmp_chpl90 = memcmp(((c_ptr_void_chpl)(call_tmp_chpl85)), ((c_ptr_void_chpl)(call_tmp_chpl86)), ((uint64_t)(size_chpl16)));
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              chpl_checkValue4(call_tmp_chpl90, &ret_tmp_chpl34, _ln_chpl, _fn_chpl);
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              call_tmp_chpl91 = ret_tmp_chpl34;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              i_arg_chpl16 = &call_tmp_chpl91;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              call_tmp_chpl92 = borrow_chpl2(i_arg_chpl16);
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              if (((RootClass_chpl)(call_tmp_chpl92)) != nil) /* ZLINE: 357 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              {
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                coerce_tmp_chpl35 = ((Error_chpl)(call_tmp_chpl92));
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                chpl_check_nil(coerce_tmp_chpl35, _ln_chpl, _fn_chpl);
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                ((void(*)(Error_chpl,
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                   string_chpl *,
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                   int64_t,
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                   int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(coerce_tmp_chpl35))->chpl__cid) + INT64(1))])(coerce_tmp_chpl35, &ret_tmp_chpl35, _ln_chpl, _fn_chpl);
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                call_tmp_chpl93 = ret_tmp_chpl35;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                safeCastCheckHalt_chpl(&call_tmp_chpl93, _ln_chpl, _fn_chpl);
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                i_x_chpl18 = &call_tmp_chpl93;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                deinit_chpl95(i_x_chpl18, _ln_chpl, _fn_chpl);
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              }
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              chpl__autoDestroy62(&call_tmp_chpl91, _ln_chpl, _fn_chpl);
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              call_tmp_chpl94 = ((int64_t)(call_tmp_chpl90));
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              if (call_tmp_chpl94 == INT64(0)) /* ZLINE: 357 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              {
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                if (size_chpl16 < coerce_tmp_chpl23) /* ZLINE: 357 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                {
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                  ret_chpl11 = INT64(1);
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                  goto _end__strcmp_local_chpl8;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                }
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                if (size_chpl16 < coerce_tmp_chpl26) /* ZLINE: 357 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                {
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                  ret_chpl11 = INT64(-1);
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                  goto _end__strcmp_local_chpl8;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                }
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              }
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              ret_chpl11 = call_tmp_chpl94;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              _end__strcmp_local_chpl8:;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              chpl_here_free(((c_ptr_void_chpl)(call_tmp_chpl85)), _ln_chpl, _fn_chpl);
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              chpl_here_free(((c_ptr_void_chpl)(call_tmp_chpl86)), _ln_chpl, _fn_chpl);
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              ret_chpl7 = ret_chpl11;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
              goto _end__strcmp_chpl2;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            }
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          }
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        }
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        _end__strcmp_chpl2:;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        tmp_chpl12 = (ret_chpl7 > INT64(0));
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        i_x_chpl19 = &call_tmp_chpl52;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        deinit_chpl95(i_x_chpl19, _ln_chpl, _fn_chpl);
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      }
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      if (tmp_chpl12) /* ZLINE: 357 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      {
#line 358 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        chpl_check_nil(e_chpl, _ln_chpl, _fn_chpl);
#line 358 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        ((void(*)(Error_chpl,
#line 358 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
           string_chpl *,
#line 358 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
           int64_t,
#line 358 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
           int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(e_chpl))->chpl__cid) + INT64(1))])(e_chpl, &ret_tmp_chpl36, _ln_chpl, _fn_chpl);
#line 358 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        call_tmp_chpl95 = ret_tmp_chpl36;
#line 358 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        chpl___ASSIGN_16(&maxMsg_chpl, &call_tmp_chpl95, _ln_chpl, _fn_chpl);
#line 358 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        i_x_chpl20 = &call_tmp_chpl95;
#line 358 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        deinit_chpl95(i_x_chpl20, _ln_chpl, _fn_chpl);
#line 358 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      }
#line 360 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      i_lhs_chpl = &n_chpl;
#line 360 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      *(i_lhs_chpl) += INT64(1);
#line 360 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    }
#line 360 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  }
#line 365 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ic__F0_this_chpl2 = this_chpl31;
#line 365 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  this_chpl35 = this_chpl31;
#line 365 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_check_nil(this_chpl35, _ln_chpl, _fn_chpl);
#line 365 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  coerce_tmp_chpl36 = (this_chpl35)->nErrors;
#line 365 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  if (coerce_tmp_chpl36 < INT64(0)) /* ZLINE: 365 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 365 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  {
#line 365 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    boundsCheckHalt_chpl(&_str_literal_580_chpl, _ln_chpl, _fn_chpl);
#line 365 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  }
#line 365 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  if (coerce_tmp_chpl36 == INT64(0)) /* ZLINE: 365 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 365 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  {
#line 365 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    tmp_chpl20 = ((int64_t)(((uint64_t)((((uint64_t)(INT64(0))) - UINT64(1))))));
#line 365 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  } else /* ZLINE: 365 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 365 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  {
#line 365 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    tmp_chpl20 = ((int64_t)((INT64(0) + ((int64_t)((coerce_tmp_chpl36 - INT64(1)))))));
#line 365 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  }
#line 365 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ic__F1_high_chpl2 = tmp_chpl20;
#line 365 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  i_chpl2 = INT64(0);
#line 365 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_checkIfRangeIterWillOverflow(INT64(0), tmp_chpl20, INT64(1), INT64(0), tmp_chpl20, UINT8(true), _ln_chpl, _fn_chpl);
#line 365 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  this_chpl36 = _ic__F0_this_chpl2;
#line 365 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_check_nil(this_chpl36, _ln_chpl, _fn_chpl);
#line 365 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  coerce_tmp_chpl37 = (this_chpl36)->errorsArray;
#line 365 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  for (i_chpl2 = INT64(0); ((i_chpl2 <= _ic__F1_high_chpl2)); i_chpl2 += INT64(1)) {
#line 365 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    call_tmp_chpl96 = (coerce_tmp_chpl37 + i_chpl2);
#line 365 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    coerce_tmp_chpl38 = borrow_chpl3(call_tmp_chpl96);
#line 365 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    if (((RootClass_chpl)(coerce_tmp_chpl38)) != nil) /* ZLINE: 365 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 365 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    {
#line 365 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      this_chpl37 = _ic__F0_this_chpl2;
#line 365 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl_check_nil(this_chpl37, _ln_chpl, _fn_chpl);
#line 365 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      coerce_tmp_chpl39 = (this_chpl37)->errorsArray;
#line 365 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl97 = (coerce_tmp_chpl39 + i_chpl2);
#line 366 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      if (((RootClass_chpl)((call_tmp_chpl97)->chpl_p)) == nil) /* ZLINE: 366 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 366 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      {
#line 366 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        nilCheckHalt_chpl(&_str_literal_52_chpl, _ln_chpl, _fn_chpl);
#line 366 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      }
#line 366 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      coerce_tmp_chpl40 = (call_tmp_chpl97)->chpl_p;
#line 366 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      e_chpl2 = coerce_tmp_chpl40;
#line 367 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl_check_nil(coerce_tmp_chpl40, _ln_chpl, _fn_chpl);
#line 367 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      ((void(*)(Error_chpl,
#line 367 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
         string_chpl *,
#line 367 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
         int64_t,
#line 367 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
         int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(coerce_tmp_chpl40))->chpl__cid) + INT64(1))])(coerce_tmp_chpl40, &ret_tmp_chpl37, _ln_chpl, _fn_chpl);
#line 367 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl98 = ret_tmp_chpl37;
#line 367 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl99 = chpl___EQUALS_3(&call_tmp_chpl98, &minMsg_chpl, _ln_chpl, _fn_chpl);
#line 367 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      if (call_tmp_chpl99) /* ZLINE: 367 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 367 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      {
#line 368 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        if (((RootClass_chpl)(first_chpl2)) == nil) /* ZLINE: 368 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 368 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        {
#line 369 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          first_chpl2 = e_chpl2;
#line 369 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        }
#line 370 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        last_chpl = e_chpl2;
#line 370 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      }
#line 367 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      i_x_chpl21 = &call_tmp_chpl98;
#line 367 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      deinit_chpl95(i_x_chpl21, _ln_chpl, _fn_chpl);
#line 367 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    }
#line 367 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  }
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  coerce_tmp_chpl41 = (&minMsg_chpl)->buff;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  coerce_tmp_chpl42 = (&minMsg_chpl)->buffLen;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  coerce_tmp_chpl43 = (&minMsg_chpl)->locale_id;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  coerce_tmp_chpl44 = (&maxMsg_chpl)->buff;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  coerce_tmp_chpl45 = (&maxMsg_chpl)->buffLen;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  coerce_tmp_chpl46 = (&maxMsg_chpl)->locale_id;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  call_tmp_chpl100 = get_chpl_nodeID();
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  if (coerce_tmp_chpl43 == call_tmp_chpl100) /* ZLINE: 373 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  {
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    call_tmp_chpl101 = get_chpl_nodeID();
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    tmp_chpl21 = (coerce_tmp_chpl46 == call_tmp_chpl101);
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  } else /* ZLINE: 373 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  {
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    tmp_chpl21 = UINT8(false);
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  }
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  if (tmp_chpl21) /* ZLINE: 373 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  {
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    if (coerce_tmp_chpl42 < coerce_tmp_chpl45) /* ZLINE: 373 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    {
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      tmp_chpl22 = coerce_tmp_chpl42;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    } else /* ZLINE: 373 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    {
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      tmp_chpl22 = coerce_tmp_chpl45;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    }
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    size_chpl17 = tmp_chpl22;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    chpl_checkValue7(tmp_chpl22, &ret_tmp_chpl38, _ln_chpl, _fn_chpl);
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    call_tmp_chpl102 = ret_tmp_chpl38;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    i_arg_chpl17 = &call_tmp_chpl102;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    call_tmp_chpl103 = borrow_chpl2(i_arg_chpl17);
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    if (((RootClass_chpl)(call_tmp_chpl103)) != nil) /* ZLINE: 373 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    {
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      coerce_tmp_chpl47 = ((Error_chpl)(call_tmp_chpl103));
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl_check_nil(coerce_tmp_chpl47, _ln_chpl, _fn_chpl);
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      ((void(*)(Error_chpl,
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
         string_chpl *,
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
         int64_t,
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
         int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(coerce_tmp_chpl47))->chpl__cid) + INT64(1))])(coerce_tmp_chpl47, &ret_tmp_chpl39, _ln_chpl, _fn_chpl);
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl104 = ret_tmp_chpl39;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      safeCastCheckHalt_chpl(&call_tmp_chpl104, _ln_chpl, _fn_chpl);
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      i_x_chpl22 = &call_tmp_chpl104;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      deinit_chpl95(i_x_chpl22, _ln_chpl, _fn_chpl);
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    }
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    chpl__autoDestroy62(&call_tmp_chpl102, _ln_chpl, _fn_chpl);
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    call_tmp_chpl105 = memcmp(((c_ptr_void_chpl)(coerce_tmp_chpl41)), ((c_ptr_void_chpl)(coerce_tmp_chpl44)), ((uint64_t)(size_chpl17)));
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    chpl_checkValue4(call_tmp_chpl105, &ret_tmp_chpl40, _ln_chpl, _fn_chpl);
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    call_tmp_chpl106 = ret_tmp_chpl40;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    i_arg_chpl18 = &call_tmp_chpl106;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    call_tmp_chpl107 = borrow_chpl2(i_arg_chpl18);
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    if (((RootClass_chpl)(call_tmp_chpl107)) != nil) /* ZLINE: 373 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    {
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      coerce_tmp_chpl48 = ((Error_chpl)(call_tmp_chpl107));
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl_check_nil(coerce_tmp_chpl48, _ln_chpl, _fn_chpl);
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      ((void(*)(Error_chpl,
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
         string_chpl *,
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
         int64_t,
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
         int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(coerce_tmp_chpl48))->chpl__cid) + INT64(1))])(coerce_tmp_chpl48, &ret_tmp_chpl41, _ln_chpl, _fn_chpl);
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl108 = ret_tmp_chpl41;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      safeCastCheckHalt_chpl(&call_tmp_chpl108, _ln_chpl, _fn_chpl);
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      i_x_chpl23 = &call_tmp_chpl108;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      deinit_chpl95(i_x_chpl23, _ln_chpl, _fn_chpl);
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    }
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    chpl__autoDestroy62(&call_tmp_chpl106, _ln_chpl, _fn_chpl);
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    call_tmp_chpl109 = ((int64_t)(call_tmp_chpl105));
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    if (call_tmp_chpl109 == INT64(0)) /* ZLINE: 373 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    {
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      if (size_chpl17 < coerce_tmp_chpl42) /* ZLINE: 373 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      {
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        ret_chpl13 = INT64(1);
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        goto _end__strcmp_local_chpl9;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      }
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      if (size_chpl17 < coerce_tmp_chpl45) /* ZLINE: 373 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      {
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        ret_chpl13 = INT64(-1);
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        goto _end__strcmp_local_chpl9;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      }
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    }
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    ret_chpl13 = call_tmp_chpl109;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    _end__strcmp_local_chpl9:;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    ret_chpl12 = ret_chpl13;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    goto _end__strcmp_chpl3;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  } else /* ZLINE: 373 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  {
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    call_tmp_chpl110 = get_chpl_nodeID();
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    if (coerce_tmp_chpl43 != call_tmp_chpl110) /* ZLINE: 373 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    {
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl111 = get_chpl_nodeID();
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      tmp_chpl23 = (coerce_tmp_chpl46 == call_tmp_chpl111);
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    } else /* ZLINE: 373 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    {
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      tmp_chpl23 = UINT8(false);
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    }
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    if (tmp_chpl23) /* ZLINE: 373 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    {
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl112 = bufferCopyRemote_chpl(((int64_t)(coerce_tmp_chpl43)), coerce_tmp_chpl41, coerce_tmp_chpl42, _ln_chpl, _fn_chpl);
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      if (coerce_tmp_chpl42 < coerce_tmp_chpl45) /* ZLINE: 373 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      {
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        tmp_chpl24 = coerce_tmp_chpl42;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      } else /* ZLINE: 373 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      {
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        tmp_chpl24 = coerce_tmp_chpl45;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      }
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      size_chpl18 = tmp_chpl24;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl_checkValue7(tmp_chpl24, &ret_tmp_chpl42, _ln_chpl, _fn_chpl);
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl113 = ret_tmp_chpl42;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      i_arg_chpl19 = &call_tmp_chpl113;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl114 = borrow_chpl2(i_arg_chpl19);
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      if (((RootClass_chpl)(call_tmp_chpl114)) != nil) /* ZLINE: 373 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      {
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        coerce_tmp_chpl49 = ((Error_chpl)(call_tmp_chpl114));
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        chpl_check_nil(coerce_tmp_chpl49, _ln_chpl, _fn_chpl);
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        ((void(*)(Error_chpl,
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
           string_chpl *,
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
           int64_t,
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
           int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(coerce_tmp_chpl49))->chpl__cid) + INT64(1))])(coerce_tmp_chpl49, &ret_tmp_chpl43, _ln_chpl, _fn_chpl);
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        call_tmp_chpl115 = ret_tmp_chpl43;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        safeCastCheckHalt_chpl(&call_tmp_chpl115, _ln_chpl, _fn_chpl);
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        i_x_chpl24 = &call_tmp_chpl115;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        deinit_chpl95(i_x_chpl24, _ln_chpl, _fn_chpl);
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      }
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl__autoDestroy62(&call_tmp_chpl113, _ln_chpl, _fn_chpl);
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl116 = memcmp(((c_ptr_void_chpl)(call_tmp_chpl112)), ((c_ptr_void_chpl)(coerce_tmp_chpl44)), ((uint64_t)(size_chpl18)));
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl_checkValue4(call_tmp_chpl116, &ret_tmp_chpl44, _ln_chpl, _fn_chpl);
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl117 = ret_tmp_chpl44;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      i_arg_chpl20 = &call_tmp_chpl117;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl118 = borrow_chpl2(i_arg_chpl20);
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      if (((RootClass_chpl)(call_tmp_chpl118)) != nil) /* ZLINE: 373 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      {
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        coerce_tmp_chpl50 = ((Error_chpl)(call_tmp_chpl118));
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        chpl_check_nil(coerce_tmp_chpl50, _ln_chpl, _fn_chpl);
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        ((void(*)(Error_chpl,
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
           string_chpl *,
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
           int64_t,
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
           int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(coerce_tmp_chpl50))->chpl__cid) + INT64(1))])(coerce_tmp_chpl50, &ret_tmp_chpl45, _ln_chpl, _fn_chpl);
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        call_tmp_chpl119 = ret_tmp_chpl45;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        safeCastCheckHalt_chpl(&call_tmp_chpl119, _ln_chpl, _fn_chpl);
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        i_x_chpl25 = &call_tmp_chpl119;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        deinit_chpl95(i_x_chpl25, _ln_chpl, _fn_chpl);
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      }
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl__autoDestroy62(&call_tmp_chpl117, _ln_chpl, _fn_chpl);
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl120 = ((int64_t)(call_tmp_chpl116));
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      if (call_tmp_chpl120 == INT64(0)) /* ZLINE: 373 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      {
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        if (size_chpl18 < coerce_tmp_chpl42) /* ZLINE: 373 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        {
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          ret_chpl14 = INT64(1);
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          goto _end__strcmp_local_chpl10;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        }
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        if (size_chpl18 < coerce_tmp_chpl45) /* ZLINE: 373 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        {
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          ret_chpl14 = INT64(-1);
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          goto _end__strcmp_local_chpl10;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        }
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      }
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      ret_chpl14 = call_tmp_chpl120;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      _end__strcmp_local_chpl10:;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      chpl_here_free(((c_ptr_void_chpl)(call_tmp_chpl112)), _ln_chpl, _fn_chpl);
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      ret_chpl12 = ret_chpl14;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      goto _end__strcmp_chpl3;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    } else /* ZLINE: 373 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    {
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl121 = get_chpl_nodeID();
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      if (coerce_tmp_chpl43 == call_tmp_chpl121) /* ZLINE: 373 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      {
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        call_tmp_chpl122 = get_chpl_nodeID();
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        tmp_chpl25 = (coerce_tmp_chpl46 != call_tmp_chpl122);
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      } else /* ZLINE: 373 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      {
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        tmp_chpl25 = UINT8(false);
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      }
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      if (tmp_chpl25) /* ZLINE: 373 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      {
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        call_tmp_chpl123 = bufferCopyRemote_chpl(((int64_t)(coerce_tmp_chpl46)), coerce_tmp_chpl44, coerce_tmp_chpl45, _ln_chpl, _fn_chpl);
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        if (coerce_tmp_chpl42 < coerce_tmp_chpl45) /* ZLINE: 373 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        {
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          tmp_chpl26 = coerce_tmp_chpl42;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        } else /* ZLINE: 373 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        {
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          tmp_chpl26 = coerce_tmp_chpl45;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        }
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        size_chpl19 = tmp_chpl26;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        chpl_checkValue7(tmp_chpl26, &ret_tmp_chpl46, _ln_chpl, _fn_chpl);
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        call_tmp_chpl124 = ret_tmp_chpl46;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        i_arg_chpl21 = &call_tmp_chpl124;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        call_tmp_chpl125 = borrow_chpl2(i_arg_chpl21);
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        if (((RootClass_chpl)(call_tmp_chpl125)) != nil) /* ZLINE: 373 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        {
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          coerce_tmp_chpl51 = ((Error_chpl)(call_tmp_chpl125));
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          chpl_check_nil(coerce_tmp_chpl51, _ln_chpl, _fn_chpl);
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          ((void(*)(Error_chpl,
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
             string_chpl *,
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
             int64_t,
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
             int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(coerce_tmp_chpl51))->chpl__cid) + INT64(1))])(coerce_tmp_chpl51, &ret_tmp_chpl47, _ln_chpl, _fn_chpl);
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          call_tmp_chpl126 = ret_tmp_chpl47;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          safeCastCheckHalt_chpl(&call_tmp_chpl126, _ln_chpl, _fn_chpl);
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          i_x_chpl26 = &call_tmp_chpl126;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          deinit_chpl95(i_x_chpl26, _ln_chpl, _fn_chpl);
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        }
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        chpl__autoDestroy62(&call_tmp_chpl124, _ln_chpl, _fn_chpl);
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        call_tmp_chpl127 = memcmp(((c_ptr_void_chpl)(coerce_tmp_chpl41)), ((c_ptr_void_chpl)(call_tmp_chpl123)), ((uint64_t)(size_chpl19)));
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        chpl_checkValue4(call_tmp_chpl127, &ret_tmp_chpl48, _ln_chpl, _fn_chpl);
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        call_tmp_chpl128 = ret_tmp_chpl48;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        i_arg_chpl22 = &call_tmp_chpl128;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        call_tmp_chpl129 = borrow_chpl2(i_arg_chpl22);
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        if (((RootClass_chpl)(call_tmp_chpl129)) != nil) /* ZLINE: 373 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        {
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          coerce_tmp_chpl52 = ((Error_chpl)(call_tmp_chpl129));
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          chpl_check_nil(coerce_tmp_chpl52, _ln_chpl, _fn_chpl);
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          ((void(*)(Error_chpl,
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
             string_chpl *,
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
             int64_t,
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
             int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(coerce_tmp_chpl52))->chpl__cid) + INT64(1))])(coerce_tmp_chpl52, &ret_tmp_chpl49, _ln_chpl, _fn_chpl);
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          call_tmp_chpl130 = ret_tmp_chpl49;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          safeCastCheckHalt_chpl(&call_tmp_chpl130, _ln_chpl, _fn_chpl);
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          i_x_chpl27 = &call_tmp_chpl130;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          deinit_chpl95(i_x_chpl27, _ln_chpl, _fn_chpl);
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        }
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        chpl__autoDestroy62(&call_tmp_chpl128, _ln_chpl, _fn_chpl);
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        call_tmp_chpl131 = ((int64_t)(call_tmp_chpl127));
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        if (call_tmp_chpl131 == INT64(0)) /* ZLINE: 373 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        {
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          if (size_chpl19 < coerce_tmp_chpl42) /* ZLINE: 373 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          {
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            ret_chpl15 = INT64(1);
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            goto _end__strcmp_local_chpl11;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          }
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          if (size_chpl19 < coerce_tmp_chpl45) /* ZLINE: 373 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          {
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            ret_chpl15 = INT64(-1);
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            goto _end__strcmp_local_chpl11;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          }
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        }
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        ret_chpl15 = call_tmp_chpl131;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        _end__strcmp_local_chpl11:;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        chpl_here_free(((c_ptr_void_chpl)(call_tmp_chpl123)), _ln_chpl, _fn_chpl);
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        ret_chpl12 = ret_chpl15;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        goto _end__strcmp_chpl3;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      } else /* ZLINE: 373 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      {
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        call_tmp_chpl132 = bufferCopyRemote_chpl(((int64_t)(coerce_tmp_chpl43)), coerce_tmp_chpl41, coerce_tmp_chpl42, _ln_chpl, _fn_chpl);
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        call_tmp_chpl133 = bufferCopyRemote_chpl(((int64_t)(coerce_tmp_chpl46)), coerce_tmp_chpl44, coerce_tmp_chpl45, _ln_chpl, _fn_chpl);
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        if (coerce_tmp_chpl42 < coerce_tmp_chpl45) /* ZLINE: 373 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        {
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          tmp_chpl27 = coerce_tmp_chpl42;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        } else /* ZLINE: 373 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        {
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          tmp_chpl27 = coerce_tmp_chpl45;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        }
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        size_chpl20 = tmp_chpl27;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        chpl_checkValue7(tmp_chpl27, &ret_tmp_chpl50, _ln_chpl, _fn_chpl);
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        call_tmp_chpl134 = ret_tmp_chpl50;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        i_arg_chpl23 = &call_tmp_chpl134;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        call_tmp_chpl135 = borrow_chpl2(i_arg_chpl23);
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        if (((RootClass_chpl)(call_tmp_chpl135)) != nil) /* ZLINE: 373 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        {
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          coerce_tmp_chpl53 = ((Error_chpl)(call_tmp_chpl135));
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          chpl_check_nil(coerce_tmp_chpl53, _ln_chpl, _fn_chpl);
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          ((void(*)(Error_chpl,
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
             string_chpl *,
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
             int64_t,
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
             int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(coerce_tmp_chpl53))->chpl__cid) + INT64(1))])(coerce_tmp_chpl53, &ret_tmp_chpl51, _ln_chpl, _fn_chpl);
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          call_tmp_chpl136 = ret_tmp_chpl51;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          safeCastCheckHalt_chpl(&call_tmp_chpl136, _ln_chpl, _fn_chpl);
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          i_x_chpl28 = &call_tmp_chpl136;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          deinit_chpl95(i_x_chpl28, _ln_chpl, _fn_chpl);
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        }
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        chpl__autoDestroy62(&call_tmp_chpl134, _ln_chpl, _fn_chpl);
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        call_tmp_chpl137 = memcmp(((c_ptr_void_chpl)(call_tmp_chpl132)), ((c_ptr_void_chpl)(call_tmp_chpl133)), ((uint64_t)(size_chpl20)));
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        chpl_checkValue4(call_tmp_chpl137, &ret_tmp_chpl52, _ln_chpl, _fn_chpl);
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        call_tmp_chpl138 = ret_tmp_chpl52;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        i_arg_chpl24 = &call_tmp_chpl138;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        call_tmp_chpl139 = borrow_chpl2(i_arg_chpl24);
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        if (((RootClass_chpl)(call_tmp_chpl139)) != nil) /* ZLINE: 373 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        {
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          coerce_tmp_chpl54 = ((Error_chpl)(call_tmp_chpl139));
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          chpl_check_nil(coerce_tmp_chpl54, _ln_chpl, _fn_chpl);
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          ((void(*)(Error_chpl,
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
             string_chpl *,
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
             int64_t,
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
             int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(coerce_tmp_chpl54))->chpl__cid) + INT64(1))])(coerce_tmp_chpl54, &ret_tmp_chpl53, _ln_chpl, _fn_chpl);
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          call_tmp_chpl140 = ret_tmp_chpl53;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          safeCastCheckHalt_chpl(&call_tmp_chpl140, _ln_chpl, _fn_chpl);
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          i_x_chpl29 = &call_tmp_chpl140;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          deinit_chpl95(i_x_chpl29, _ln_chpl, _fn_chpl);
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        }
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        chpl__autoDestroy62(&call_tmp_chpl138, _ln_chpl, _fn_chpl);
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        call_tmp_chpl141 = ((int64_t)(call_tmp_chpl137));
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        if (call_tmp_chpl141 == INT64(0)) /* ZLINE: 373 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        {
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          if (size_chpl20 < coerce_tmp_chpl42) /* ZLINE: 373 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          {
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            ret_chpl16 = INT64(1);
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            goto _end__strcmp_local_chpl12;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          }
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          if (size_chpl20 < coerce_tmp_chpl45) /* ZLINE: 373 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          {
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            ret_chpl16 = INT64(-1);
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
            goto _end__strcmp_local_chpl12;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          }
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        }
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        ret_chpl16 = call_tmp_chpl141;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        _end__strcmp_local_chpl12:;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        chpl_here_free(((c_ptr_void_chpl)(call_tmp_chpl132)), _ln_chpl, _fn_chpl);
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        chpl_here_free(((c_ptr_void_chpl)(call_tmp_chpl133)), _ln_chpl, _fn_chpl);
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        ret_chpl12 = ret_chpl16;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        goto _end__strcmp_chpl3;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      }
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    }
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  }
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _end__strcmp_chpl3:;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  if (! (ret_chpl12 == INT64(0))) /* ZLINE: 373 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  {
#line 374 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    _ic__F0_this_chpl3 = this_chpl31;
#line 374 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    this_chpl38 = this_chpl31;
#line 374 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    chpl_check_nil(this_chpl38, _ln_chpl, _fn_chpl);
#line 374 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    coerce_tmp_chpl55 = (this_chpl38)->nErrors;
#line 374 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    if (coerce_tmp_chpl55 < INT64(0)) /* ZLINE: 374 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 374 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    {
#line 374 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      boundsCheckHalt_chpl(&_str_literal_580_chpl, _ln_chpl, _fn_chpl);
#line 374 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    }
#line 374 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    if (coerce_tmp_chpl55 == INT64(0)) /* ZLINE: 374 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 374 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    {
#line 374 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      tmp_chpl28 = ((int64_t)(((uint64_t)((((uint64_t)(INT64(0))) - UINT64(1))))));
#line 374 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    } else /* ZLINE: 374 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 374 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    {
#line 374 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      tmp_chpl28 = ((int64_t)((INT64(0) + ((int64_t)((coerce_tmp_chpl55 - INT64(1)))))));
#line 374 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    }
#line 374 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    _ic__F1_high_chpl3 = tmp_chpl28;
#line 374 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    i_chpl3 = INT64(0);
#line 374 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    chpl_checkIfRangeIterWillOverflow(INT64(0), tmp_chpl28, INT64(1), INT64(0), tmp_chpl28, UINT8(true), _ln_chpl, _fn_chpl);
#line 374 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    this_chpl39 = _ic__F0_this_chpl3;
#line 374 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    chpl_check_nil(this_chpl39, _ln_chpl, _fn_chpl);
#line 374 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    coerce_tmp_chpl56 = (this_chpl39)->errorsArray;
#line 374 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    for (i_chpl3 = INT64(0); ((i_chpl3 <= _ic__F1_high_chpl3)); i_chpl3 += INT64(1)) {
#line 374 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      call_tmp_chpl142 = (coerce_tmp_chpl56 + i_chpl3);
#line 374 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      coerce_tmp_chpl57 = borrow_chpl3(call_tmp_chpl142);
#line 374 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      if (((RootClass_chpl)(coerce_tmp_chpl57)) != nil) /* ZLINE: 374 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 374 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      {
#line 374 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        this_chpl40 = _ic__F0_this_chpl3;
#line 374 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        chpl_check_nil(this_chpl40, _ln_chpl, _fn_chpl);
#line 374 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        coerce_tmp_chpl58 = (this_chpl40)->errorsArray;
#line 374 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        call_tmp_chpl143 = (coerce_tmp_chpl58 + i_chpl3);
#line 375 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        if (((RootClass_chpl)((call_tmp_chpl143)->chpl_p)) == nil) /* ZLINE: 375 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 375 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        {
#line 375 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          nilCheckHalt_chpl(&_str_literal_52_chpl, _ln_chpl, _fn_chpl);
#line 375 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        }
#line 375 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        coerce_tmp_chpl59 = (call_tmp_chpl143)->chpl_p;
#line 375 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        e_chpl3 = coerce_tmp_chpl59;
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        chpl_check_nil(coerce_tmp_chpl59, _ln_chpl, _fn_chpl);
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        ((void(*)(Error_chpl,
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
           string_chpl *,
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
           int64_t,
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
           int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(coerce_tmp_chpl59))->chpl__cid) + INT64(1))])(coerce_tmp_chpl59, &ret_tmp_chpl54, _ln_chpl, _fn_chpl);
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        call_tmp_chpl144 = ret_tmp_chpl54;
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        call_tmp_chpl145 = chpl___EQUALS_3(&call_tmp_chpl144, &maxMsg_chpl, _ln_chpl, _fn_chpl);
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        if (call_tmp_chpl145) /* ZLINE: 376 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        {
#line 377 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
          last_chpl = e_chpl3;
#line 377 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        }
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        i_x_chpl30 = &call_tmp_chpl144;
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
        deinit_chpl95(i_x_chpl30, _ln_chpl, _fn_chpl);
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      }
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    }
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  }
#line 383 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl___COLON_25(n_chpl, &ret_tmp_chpl55, _ln_chpl, _fn_chpl);
#line 383 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  call_tmp_chpl146 = ret_tmp_chpl55;
#line 383 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl___PLUS_(&call_tmp_chpl146, &_str_literal_164_chpl, &ret_tmp_chpl56, _ln_chpl, _fn_chpl);
#line 383 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  call_tmp_chpl147 = ret_tmp_chpl56;
#line 383 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  ret_chpl17 = call_tmp_chpl147;
#line 383 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  i_x_chpl31 = &call_tmp_chpl146;
#line 383 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  deinit_chpl95(i_x_chpl31, _ln_chpl, _fn_chpl);
#line 385 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  if (((RootClass_chpl)(first_chpl2)) != nil) /* ZLINE: 385 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 385 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  {
#line 385 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    tmp_chpl29 = (((RootClass_chpl)(last_chpl)) != nil);
#line 385 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  } else /* ZLINE: 385 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 385 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  {
#line 385 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    tmp_chpl29 = UINT8(false);
#line 385 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  }
#line 385 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  if (tmp_chpl29) /* ZLINE: 385 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 385 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  {
#line 385 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    tmp_chpl30 = (((RootClass_chpl)(first_chpl2)) != ((RootClass_chpl)(last_chpl)));
#line 385 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  } else /* ZLINE: 385 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 385 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  {
#line 385 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    tmp_chpl30 = UINT8(false);
#line 385 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  }
#line 385 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  if (tmp_chpl30) /* ZLINE: 385 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 385 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  {
#line 386 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    if (((RootClass_chpl)(first_chpl2)) == nil) /* ZLINE: 386 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 386 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    {
#line 386 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      nilCheckHalt_chpl(&_str_literal_52_chpl, _ln_chpl, _fn_chpl);
#line 386 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    }
#line 386 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    chpl_describe_error(first_chpl2, &ret_tmp_chpl57, _ln_chpl, _fn_chpl);
#line 386 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    call_tmp_chpl148 = ret_tmp_chpl57;
#line 386 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    chpl___PLUS_(&call_tmp_chpl148, &_str_literal_165_chpl, &ret_tmp_chpl58, _ln_chpl, _fn_chpl);
#line 386 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    call_tmp_chpl149 = ret_tmp_chpl58;
#line 386 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    if (((RootClass_chpl)(last_chpl)) == nil) /* ZLINE: 386 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 386 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    {
#line 386 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      nilCheckHalt_chpl(&_str_literal_52_chpl, _ln_chpl, _fn_chpl);
#line 386 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    }
#line 386 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    chpl_describe_error(last_chpl, &ret_tmp_chpl59, _ln_chpl, _fn_chpl);
#line 386 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    call_tmp_chpl150 = ret_tmp_chpl59;
#line 386 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    chpl___PLUS_(&call_tmp_chpl149, &call_tmp_chpl150, &ret_tmp_chpl60, _ln_chpl, _fn_chpl);
#line 386 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    call_tmp_chpl151 = ret_tmp_chpl60;
#line 386 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    chpl___PLUS__ASSIGN_(&ret_chpl17, &call_tmp_chpl151, _ln_chpl, _fn_chpl);
#line 386 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    i_x_chpl32 = &call_tmp_chpl151;
#line 386 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    deinit_chpl95(i_x_chpl32, _ln_chpl, _fn_chpl);
#line 386 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    i_x_chpl33 = &call_tmp_chpl150;
#line 386 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    deinit_chpl95(i_x_chpl33, _ln_chpl, _fn_chpl);
#line 386 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    i_x_chpl34 = &call_tmp_chpl149;
#line 386 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    deinit_chpl95(i_x_chpl34, _ln_chpl, _fn_chpl);
#line 386 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    i_x_chpl35 = &call_tmp_chpl148;
#line 386 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    deinit_chpl95(i_x_chpl35, _ln_chpl, _fn_chpl);
#line 386 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  } else /* ZLINE: 387 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 386 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  if (((RootClass_chpl)(first_chpl2)) != nil) /* ZLINE: 387 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 386 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  {
#line 388 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    chpl_describe_error(first_chpl2, &ret_tmp_chpl61, _ln_chpl, _fn_chpl);
#line 388 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    call_tmp_chpl152 = ret_tmp_chpl61;
#line 388 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    chpl___PLUS__ASSIGN_(&ret_chpl17, &call_tmp_chpl152, _ln_chpl, _fn_chpl);
#line 388 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    i_x_chpl36 = &call_tmp_chpl152;
#line 388 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    deinit_chpl95(i_x_chpl36, _ln_chpl, _fn_chpl);
#line 388 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  }
#line 390 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  ret_chpl = ret_chpl17;
#line 390 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  i_x_chpl37 = &maxMsg_chpl;
#line 390 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  deinit_chpl95(i_x_chpl37, _ln_chpl, _fn_chpl);
#line 390 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  i_x_chpl38 = &minMsg_chpl;
#line 390 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  deinit_chpl95(i_x_chpl38, _ln_chpl, _fn_chpl);
#line 345 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  *(_retArg_chpl) = ret_chpl;
#line 345 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  return;
#line 345 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
}

#line 423 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
/* Errors.chpl:423 */
#line 423 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
static void chpl_error_type_name(Error_chpl err_chpl8,
#line 423 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                                 string_chpl * _retArg_chpl,
#line 423 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                                 int64_t _ln_chpl,
#line 423 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                                 int32_t _fn_chpl) {
#line 426 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl nameS_chpl;
#line 427 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  Error_chpl error_chpl = NULL;
#line 428 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  c_ptrConst_int8_t_chpl call_tmp_chpl6 = NULL;
#line 428 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int64_t call_tmp_chpl7;
#line 428 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _owned_IllegalArgumentError_chpl2 call_tmp_chpl8;
#line 428 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _owned_IllegalArgumentError_chpl2 ret_tmp_chpl;
#line 428 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref__owned_IllegalArgumentError_chpl i_arg_chpl = NULL;
#line 428 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  IllegalArgumentError_chpl call_tmp_chpl9 = NULL;
#line 428 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl10;
#line 428 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  Error_chpl coerce_tmp_chpl3 = NULL;
#line 428 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl2;
#line 428 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 428 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int64_t call_tmp_chpl11;
#line 428 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_chpl;
#line 428 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl12;
#line 428 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  Error_chpl error_chpl2 = NULL;
#line 428 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl3;
#line 424 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_check_nil(err_chpl8, _ln_chpl, _fn_chpl);
#line 427 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  error_chpl = NULL;
#line 428 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  call_tmp_chpl6 = ((c_ptrConst_int8_t_chpl)(chpl_classNames[((RootClass_chpl)(err_chpl8))->chpl__cid]));
#line 428 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  call_tmp_chpl7 = string_length_bytes(call_tmp_chpl6);
#line 428 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_checkValue2(call_tmp_chpl7, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 428 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  call_tmp_chpl8 = ret_tmp_chpl;
#line 428 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  i_arg_chpl = &call_tmp_chpl8;
#line 428 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  call_tmp_chpl9 = borrow_chpl2(i_arg_chpl);
#line 428 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  if (((RootClass_chpl)(call_tmp_chpl9)) != nil) /* ZLINE: 428 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 428 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  {
#line 428 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    coerce_tmp_chpl3 = ((Error_chpl)(call_tmp_chpl9));
#line 428 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    chpl_check_nil(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 428 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    ((void(*)(Error_chpl,
#line 428 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
       string_chpl *,
#line 428 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
       int64_t,
#line 428 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
       int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(coerce_tmp_chpl3))->chpl__cid) + INT64(1))])(coerce_tmp_chpl3, &ret_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 428 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    call_tmp_chpl10 = ret_tmp_chpl2;
#line 428 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    safeCastCheckHalt_chpl(&call_tmp_chpl10, _ln_chpl, _fn_chpl);
#line 428 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    i_x_chpl = &call_tmp_chpl10;
#line 428 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    deinit_chpl95(i_x_chpl, _ln_chpl, _fn_chpl);
#line 428 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  }
#line 428 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl__autoDestroy62(&call_tmp_chpl8, _ln_chpl, _fn_chpl);
#line 428 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  call_tmp_chpl11 = call_tmp_chpl7;
#line 428 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  if (error_chpl != nil) /* ZLINE: 428 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 428 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  {
#line 428 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    goto handler_chpl;
#line 428 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  }
#line 428 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  error_chpl2 = NULL;
#line 428 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  createCopyingBuffer_chpl(((c_ptr_uint8_t_chpl)(call_tmp_chpl6)), call_tmp_chpl11, ((int64_t)((call_tmp_chpl11 + INT64(1)))), decodePolicy_chpl_strict_chpl, &error_chpl2, &ret_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 428 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  call_tmp_chpl12 = ret_tmp_chpl3;
#line 428 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  if (error_chpl2 != nil) /* ZLINE: 428 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 428 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  {
#line 428 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    error_chpl = error_chpl2;
#line 428 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    goto _endcreateCopyingBuffer_chpl;
#line 428 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  }
#line 428 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  ret_chpl = call_tmp_chpl12;
#line 428 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _endcreateCopyingBuffer_chpl:;
#line 428 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  if (error_chpl != nil) /* ZLINE: 428 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 428 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  {
#line 428 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    goto handler_chpl;
#line 428 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  }
#line 428 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  nameS_chpl = ret_chpl;
#line 428 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  handler_chpl:;
#line 427 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  if (error_chpl != nil) /* ZLINE: 427 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 427 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  {
#line 427 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    chpl_uncaught_error(error_chpl, _ln_chpl, _fn_chpl);
#line 427 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  }
#line 423 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  *(_retArg_chpl) = nameS_chpl;
#line 423 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  return;
#line 423 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
}

#line 432 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
/* Errors.chpl:432 */
#line 432 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
static void chpl_describe_error(Error_chpl err_chpl8,
#line 432 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                                string_chpl * _retArg_chpl,
#line 432 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                                int64_t _ln_chpl,
#line 432 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                                int32_t _fn_chpl) {
#line 433 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl6;
#line 433 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl;
#line 433 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl nameS_chpl;
#line 435 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl7;
#line 435 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl2;
#line 435 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl8;
#line 435 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl3;
#line 435 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl9;
#line 435 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl4;
#line 435 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 435 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl2 = NULL;
#line 437 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl3 = NULL;
#line 433 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_error_type_name(err_chpl8, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 433 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  call_tmp_chpl6 = ret_tmp_chpl;
#line 433 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  nameS_chpl = call_tmp_chpl6;
#line 435 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl___PLUS_(&nameS_chpl, &_str_literal_167_chpl, &ret_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 435 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  call_tmp_chpl7 = ret_tmp_chpl2;
#line 435 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_check_nil(err_chpl8, _ln_chpl, _fn_chpl);
#line 435 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  ((void(*)(Error_chpl,
#line 435 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
     string_chpl *,
#line 435 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
     int64_t,
#line 435 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
     int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(err_chpl8))->chpl__cid) + INT64(1))])(err_chpl8, &ret_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 435 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  call_tmp_chpl8 = ret_tmp_chpl3;
#line 435 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl___PLUS_(&call_tmp_chpl7, &call_tmp_chpl8, &ret_tmp_chpl4, _ln_chpl, _fn_chpl);
#line 435 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  call_tmp_chpl9 = ret_tmp_chpl4;
#line 435 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  i_x_chpl = &call_tmp_chpl8;
#line 435 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  deinit_chpl95(i_x_chpl, _ln_chpl, _fn_chpl);
#line 435 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  i_x_chpl2 = &call_tmp_chpl7;
#line 435 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  deinit_chpl95(i_x_chpl2, _ln_chpl, _fn_chpl);
#line 437 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  i_x_chpl3 = &nameS_chpl;
#line 437 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  deinit_chpl95(i_x_chpl3, _ln_chpl, _fn_chpl);
#line 432 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  *(_retArg_chpl) = call_tmp_chpl9;
#line 432 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  return;
#line 432 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
}

#line 442 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
/* Errors.chpl:442 */
#line 442 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
static Error_chpl chpl_do_fix_thrown_error(Error_chpl err_chpl8,
#line 442 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                                           int64_t _ln_chpl,
#line 442 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                                           int32_t _fn_chpl) {
#line 444 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  Error_chpl fixErr_chpl = NULL;
#line 446 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  NilThrownError_chpl new_temp_chpl = NULL;
#line 448 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int64_t call_tmp_chpl6;
#line 449 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int32_t call_tmp_chpl7;
#line 455 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  Error_chpl coerce_tmp_chpl3 = NULL;
#line 456 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  Error_chpl coerce_tmp_chpl4 = NULL;
#line 457 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  Error_chpl coerce_tmp_chpl5 = NULL;
#line 458 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  Error_chpl coerce_tmp_chpl6 = NULL;
#line 444 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  fixErr_chpl = err_chpl8;
#line 445 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  if (((RootClass_chpl)(err_chpl8)) == nil) /* ZLINE: 445 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 445 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  {
#line 446 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    new_temp_chpl = _new_chpl57(_ln_chpl, _fn_chpl);
#line 446 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    fixErr_chpl = ((Error_chpl)(new_temp_chpl));
#line 446 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  }
#line 448 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  call_tmp_chpl6 = _ln_chpl;
#line 449 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  call_tmp_chpl7 = _fn_chpl;
#line 455 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  if (((RootClass_chpl)(fixErr_chpl)) == nil) /* ZLINE: 455 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 455 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  {
#line 455 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    nilCheckHalt_chpl(&_str_literal_52_chpl, _ln_chpl, _fn_chpl);
#line 455 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  }
#line 455 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  coerce_tmp_chpl3 = fixErr_chpl;
#line 455 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_check_nil(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 455 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  if (! (coerce_tmp_chpl3)->_hasThrowInfo) /* ZLINE: 455 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 455 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  {
#line 456 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    if (((RootClass_chpl)(fixErr_chpl)) == nil) /* ZLINE: 456 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 456 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    {
#line 456 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      nilCheckHalt_chpl(&_str_literal_52_chpl, _ln_chpl, _fn_chpl);
#line 456 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    }
#line 456 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    coerce_tmp_chpl4 = fixErr_chpl;
#line 456 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    chpl_check_nil(coerce_tmp_chpl4, _ln_chpl, _fn_chpl);
#line 456 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    (coerce_tmp_chpl4)->_hasThrowInfo = UINT8(true);
#line 457 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    if (((RootClass_chpl)(fixErr_chpl)) == nil) /* ZLINE: 457 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 457 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    {
#line 457 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      nilCheckHalt_chpl(&_str_literal_52_chpl, _ln_chpl, _fn_chpl);
#line 457 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    }
#line 457 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    coerce_tmp_chpl5 = fixErr_chpl;
#line 457 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    chpl_check_nil(coerce_tmp_chpl5, _ln_chpl, _fn_chpl);
#line 457 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    (coerce_tmp_chpl5)->thrownLine = call_tmp_chpl6;
#line 458 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    if (((RootClass_chpl)(fixErr_chpl)) == nil) /* ZLINE: 458 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 458 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    {
#line 458 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
      nilCheckHalt_chpl(&_str_literal_52_chpl, _ln_chpl, _fn_chpl);
#line 458 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    }
#line 458 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    coerce_tmp_chpl6 = fixErr_chpl;
#line 458 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    chpl_check_nil(coerce_tmp_chpl6, _ln_chpl, _fn_chpl);
#line 458 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    (coerce_tmp_chpl6)->thrownFileId = call_tmp_chpl7;
#line 458 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  }
#line 442 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  return fixErr_chpl;
#line 442 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
}

#line 475 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
/* Errors.chpl:475 */
#line 475 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
static Error_chpl chpl_fix_thrown_error(_owned_Error_chpl * err_chpl8,
#line 475 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                                        int64_t _ln_chpl,
#line 475 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                                        int32_t _fn_chpl) {
#line 475 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _owned_Error_chpl _formal_tmp_err_chpl;
#line 476 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref__owned_Error_chpl2 i_obj_chpl = NULL;
#line 476 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  Error_chpl coerce_tmp_chpl3 = NULL;
#line 476 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  Error_chpl call_tmp_chpl6 = NULL;
#line 475 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _formal_tmp_err_chpl = *(err_chpl8);
#line 476 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  i_obj_chpl = &_formal_tmp_err_chpl;
#line 476 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  coerce_tmp_chpl3 = (i_obj_chpl)->chpl_p;
#line 476 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  (i_obj_chpl)->chpl_p = ((Error_chpl)(nil));
#line 476 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  if (((RootClass_chpl)(coerce_tmp_chpl3)) == nil) /* ZLINE: 476 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 476 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  {
#line 476 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    nilCheckHalt_chpl(&_str_literal_52_chpl, _ln_chpl, _fn_chpl);
#line 476 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  }
#line 476 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  call_tmp_chpl6 = chpl_do_fix_thrown_error(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 475 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl__autoDestroy57(&_formal_tmp_err_chpl, _ln_chpl, _fn_chpl);
#line 475 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  return call_tmp_chpl6;
#line 475 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
}

#line 506 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
/* Errors.chpl:506 */
#line 506 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
static void chpl_delete_error(Error_chpl err_chpl8,
#line 506 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                              int64_t _ln_chpl,
#line 506 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                              int32_t _fn_chpl) {
#line 507 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  if (((RootClass_chpl)(err_chpl8)) != nil) /* ZLINE: 507 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 507 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  {
#line 507 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    chpl__delete74(err_chpl8, _ln_chpl, _fn_chpl);
#line 507 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  }
#line 506 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  return;
#line 506 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
}

#line 512 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
/* Errors.chpl:512 */
#line 512 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
static void chpl_uncaught_error(Error_chpl err_chpl8,
#line 512 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                                int64_t _ln_chpl,
#line 512 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                                int32_t _fn_chpl) {
#line 517 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl myFileS_chpl;
#line 518 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  Error_chpl error_chpl = NULL;
#line 519 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  c_ptrConst_int8_t_chpl call_tmp_chpl6 = NULL;
#line 519 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int64_t call_tmp_chpl7;
#line 519 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _owned_IllegalArgumentError_chpl2 call_tmp_chpl8;
#line 519 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _owned_IllegalArgumentError_chpl2 ret_tmp_chpl;
#line 519 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref__owned_IllegalArgumentError_chpl i_arg_chpl = NULL;
#line 519 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  IllegalArgumentError_chpl call_tmp_chpl9 = NULL;
#line 519 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl10;
#line 519 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  Error_chpl coerce_tmp_chpl3 = NULL;
#line 519 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl2;
#line 519 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 519 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int64_t call_tmp_chpl11;
#line 519 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_chpl;
#line 519 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl12;
#line 519 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  Error_chpl error_chpl2 = NULL;
#line 519 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl3;
#line 521 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int64_t call_tmp_chpl13;
#line 524 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  Error_chpl coerce_tmp_chpl4 = NULL;
#line 524 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_int32_t_chpl call_tmp_chpl14 = NULL;
#line 525 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl thrownFileS_chpl;
#line 526 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  Error_chpl error_chpl3 = NULL;
#line 527 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  c_ptrConst_int8_t_chpl call_tmp_chpl15 = NULL;
#line 527 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int64_t call_tmp_chpl16;
#line 527 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _owned_IllegalArgumentError_chpl2 call_tmp_chpl17;
#line 527 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _owned_IllegalArgumentError_chpl2 ret_tmp_chpl4;
#line 527 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref__owned_IllegalArgumentError_chpl i_arg_chpl2 = NULL;
#line 527 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  IllegalArgumentError_chpl call_tmp_chpl18 = NULL;
#line 527 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl19;
#line 527 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  Error_chpl coerce_tmp_chpl5 = NULL;
#line 527 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl5;
#line 527 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl2 = NULL;
#line 527 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int64_t call_tmp_chpl20;
#line 527 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_chpl2;
#line 527 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl21;
#line 527 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  Error_chpl error_chpl4 = NULL;
#line 527 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl6;
#line 529 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  Error_chpl coerce_tmp_chpl6 = NULL;
#line 529 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int64_t coerce_tmp_chpl7;
#line 531 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl22;
#line 531 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl7;
#line 531 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl23;
#line 531 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl8;
#line 531 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl24;
#line 531 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl9;
#line 531 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl25;
#line 531 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl10;
#line 531 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl26;
#line 531 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl11;
#line 532 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl27;
#line 532 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl12;
#line 531 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl28;
#line 531 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl13;
#line 531 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl29;
#line 531 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl14;
#line 531 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl30;
#line 531 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl15;
#line 531 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl31;
#line 531 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl16;
#line 531 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl32;
#line 531 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl17;
#line 533 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl33;
#line 533 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl18;
#line 531 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl34;
#line 531 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl19;
#line 531 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl35;
#line 531 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl20;
#line 531 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl s_chpl;
#line 531 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl3 = NULL;
#line 531 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl4 = NULL;
#line 531 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl5 = NULL;
#line 531 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl6 = NULL;
#line 531 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl7 = NULL;
#line 531 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl8 = NULL;
#line 531 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl9 = NULL;
#line 531 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl10 = NULL;
#line 531 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl11 = NULL;
#line 531 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl12 = NULL;
#line 531 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl13 = NULL;
#line 531 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl14 = NULL;
#line 531 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl15 = NULL;
#line 512 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl16 = NULL;
#line 512 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl17 = NULL;
#line 512 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl18 = NULL;
#line 518 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  error_chpl = NULL;
#line 519 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  call_tmp_chpl6 = ((c_ptrConst_int8_t_chpl)(chpl_lookupFilename(_fn_chpl)));
#line 519 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  call_tmp_chpl7 = string_length_bytes(call_tmp_chpl6);
#line 519 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_checkValue2(call_tmp_chpl7, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 519 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  call_tmp_chpl8 = ret_tmp_chpl;
#line 519 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  i_arg_chpl = &call_tmp_chpl8;
#line 519 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  call_tmp_chpl9 = borrow_chpl2(i_arg_chpl);
#line 519 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  if (((RootClass_chpl)(call_tmp_chpl9)) != nil) /* ZLINE: 519 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 519 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  {
#line 519 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    coerce_tmp_chpl3 = ((Error_chpl)(call_tmp_chpl9));
#line 519 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    chpl_check_nil(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 519 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    ((void(*)(Error_chpl,
#line 519 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
       string_chpl *,
#line 519 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
       int64_t,
#line 519 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
       int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(coerce_tmp_chpl3))->chpl__cid) + INT64(1))])(coerce_tmp_chpl3, &ret_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 519 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    call_tmp_chpl10 = ret_tmp_chpl2;
#line 519 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    safeCastCheckHalt_chpl(&call_tmp_chpl10, _ln_chpl, _fn_chpl);
#line 519 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    i_x_chpl = &call_tmp_chpl10;
#line 519 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    deinit_chpl95(i_x_chpl, _ln_chpl, _fn_chpl);
#line 519 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  }
#line 519 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl__autoDestroy62(&call_tmp_chpl8, _ln_chpl, _fn_chpl);
#line 519 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  call_tmp_chpl11 = call_tmp_chpl7;
#line 519 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  if (error_chpl != nil) /* ZLINE: 519 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 519 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  {
#line 519 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    goto handler_chpl;
#line 519 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  }
#line 519 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  error_chpl2 = NULL;
#line 519 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  createCopyingBuffer_chpl(((c_ptr_uint8_t_chpl)(call_tmp_chpl6)), call_tmp_chpl11, ((int64_t)((call_tmp_chpl11 + INT64(1)))), decodePolicy_chpl_strict_chpl, &error_chpl2, &ret_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 519 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  call_tmp_chpl12 = ret_tmp_chpl3;
#line 519 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  if (error_chpl2 != nil) /* ZLINE: 519 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 519 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  {
#line 519 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    error_chpl = error_chpl2;
#line 519 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    goto _endcreateCopyingBuffer_chpl;
#line 519 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  }
#line 519 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  ret_chpl = call_tmp_chpl12;
#line 519 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _endcreateCopyingBuffer_chpl:;
#line 519 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  if (error_chpl != nil) /* ZLINE: 519 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 519 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  {
#line 519 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    goto handler_chpl;
#line 519 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  }
#line 519 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  myFileS_chpl = ret_chpl;
#line 519 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  handler_chpl:;
#line 518 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  if (error_chpl != nil) /* ZLINE: 518 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 518 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  {
#line 518 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    chpl_uncaught_error(error_chpl, _ln_chpl, _fn_chpl);
#line 518 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  }
#line 521 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  call_tmp_chpl13 = _ln_chpl;
#line 524 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  coerce_tmp_chpl4 = err_chpl8;
#line 524 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_check_nil(coerce_tmp_chpl4, _ln_chpl, _fn_chpl);
#line 524 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  call_tmp_chpl14 = &((coerce_tmp_chpl4)->thrownFileId);
#line 526 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  error_chpl3 = NULL;
#line 527 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  call_tmp_chpl15 = ((c_ptrConst_int8_t_chpl)(chpl_lookupFilename(*(call_tmp_chpl14))));
#line 527 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  call_tmp_chpl16 = string_length_bytes(call_tmp_chpl15);
#line 527 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_checkValue2(call_tmp_chpl16, &ret_tmp_chpl4, _ln_chpl, _fn_chpl);
#line 527 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  call_tmp_chpl17 = ret_tmp_chpl4;
#line 527 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  i_arg_chpl2 = &call_tmp_chpl17;
#line 527 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  call_tmp_chpl18 = borrow_chpl2(i_arg_chpl2);
#line 527 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  if (((RootClass_chpl)(call_tmp_chpl18)) != nil) /* ZLINE: 527 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 527 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  {
#line 527 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    coerce_tmp_chpl5 = ((Error_chpl)(call_tmp_chpl18));
#line 527 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    chpl_check_nil(coerce_tmp_chpl5, _ln_chpl, _fn_chpl);
#line 527 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    ((void(*)(Error_chpl,
#line 527 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
       string_chpl *,
#line 527 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
       int64_t,
#line 527 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
       int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(coerce_tmp_chpl5))->chpl__cid) + INT64(1))])(coerce_tmp_chpl5, &ret_tmp_chpl5, _ln_chpl, _fn_chpl);
#line 527 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    call_tmp_chpl19 = ret_tmp_chpl5;
#line 527 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    safeCastCheckHalt_chpl(&call_tmp_chpl19, _ln_chpl, _fn_chpl);
#line 527 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    i_x_chpl2 = &call_tmp_chpl19;
#line 527 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    deinit_chpl95(i_x_chpl2, _ln_chpl, _fn_chpl);
#line 527 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  }
#line 527 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl__autoDestroy62(&call_tmp_chpl17, _ln_chpl, _fn_chpl);
#line 527 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  call_tmp_chpl20 = call_tmp_chpl16;
#line 527 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  if (error_chpl3 != nil) /* ZLINE: 527 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 527 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  {
#line 527 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    goto handler_chpl2;
#line 527 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  }
#line 527 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  error_chpl4 = NULL;
#line 527 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  createCopyingBuffer_chpl(((c_ptr_uint8_t_chpl)(call_tmp_chpl15)), call_tmp_chpl20, ((int64_t)((call_tmp_chpl20 + INT64(1)))), decodePolicy_chpl_strict_chpl, &error_chpl4, &ret_tmp_chpl6, _ln_chpl, _fn_chpl);
#line 527 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  call_tmp_chpl21 = ret_tmp_chpl6;
#line 527 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  if (error_chpl4 != nil) /* ZLINE: 527 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 527 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  {
#line 527 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    error_chpl3 = error_chpl4;
#line 527 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    goto _endcreateCopyingBuffer_chpl2;
#line 527 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  }
#line 527 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  ret_chpl2 = call_tmp_chpl21;
#line 527 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _endcreateCopyingBuffer_chpl2:;
#line 527 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  if (error_chpl3 != nil) /* ZLINE: 527 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 527 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  {
#line 527 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    goto handler_chpl2;
#line 527 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  }
#line 527 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  thrownFileS_chpl = ret_chpl2;
#line 527 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  handler_chpl2:;
#line 526 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  if (error_chpl3 != nil) /* ZLINE: 526 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 526 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  {
#line 526 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    chpl_uncaught_error(error_chpl3, _ln_chpl, _fn_chpl);
#line 526 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  }
#line 529 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  coerce_tmp_chpl6 = err_chpl8;
#line 529 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_check_nil(coerce_tmp_chpl6, _ln_chpl, _fn_chpl);
#line 529 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  coerce_tmp_chpl7 = (coerce_tmp_chpl6)->thrownLine;
#line 531 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_describe_error(err_chpl8, &ret_tmp_chpl7, _ln_chpl, _fn_chpl);
#line 531 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  call_tmp_chpl22 = ret_tmp_chpl7;
#line 531 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl___PLUS_(&_str_literal_176_chpl, &call_tmp_chpl22, &ret_tmp_chpl8, _ln_chpl, _fn_chpl);
#line 531 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  call_tmp_chpl23 = ret_tmp_chpl8;
#line 531 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl___PLUS_(&call_tmp_chpl23, &_str_literal_177_chpl, &ret_tmp_chpl9, _ln_chpl, _fn_chpl);
#line 531 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  call_tmp_chpl24 = ret_tmp_chpl9;
#line 531 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl___PLUS_(&call_tmp_chpl24, &thrownFileS_chpl, &ret_tmp_chpl10, _ln_chpl, _fn_chpl);
#line 531 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  call_tmp_chpl25 = ret_tmp_chpl10;
#line 531 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl___PLUS_(&call_tmp_chpl25, &_str_literal_178_chpl, &ret_tmp_chpl11, _ln_chpl, _fn_chpl);
#line 531 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  call_tmp_chpl26 = ret_tmp_chpl11;
#line 532 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl___COLON_25(coerce_tmp_chpl7, &ret_tmp_chpl12, _ln_chpl, _fn_chpl);
#line 532 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  call_tmp_chpl27 = ret_tmp_chpl12;
#line 531 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl___PLUS_(&call_tmp_chpl26, &call_tmp_chpl27, &ret_tmp_chpl13, _ln_chpl, _fn_chpl);
#line 531 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  call_tmp_chpl28 = ret_tmp_chpl13;
#line 531 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl___PLUS_(&call_tmp_chpl28, &_str_literal_179_chpl, &ret_tmp_chpl14, _ln_chpl, _fn_chpl);
#line 531 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  call_tmp_chpl29 = ret_tmp_chpl14;
#line 531 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl___PLUS_(&call_tmp_chpl29, &_str_literal_177_chpl, &ret_tmp_chpl15, _ln_chpl, _fn_chpl);
#line 531 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  call_tmp_chpl30 = ret_tmp_chpl15;
#line 531 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl___PLUS_(&call_tmp_chpl30, &myFileS_chpl, &ret_tmp_chpl16, _ln_chpl, _fn_chpl);
#line 531 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  call_tmp_chpl31 = ret_tmp_chpl16;
#line 531 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl___PLUS_(&call_tmp_chpl31, &_str_literal_178_chpl, &ret_tmp_chpl17, _ln_chpl, _fn_chpl);
#line 531 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  call_tmp_chpl32 = ret_tmp_chpl17;
#line 533 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl___COLON_25(call_tmp_chpl13, &ret_tmp_chpl18, _ln_chpl, _fn_chpl);
#line 533 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  call_tmp_chpl33 = ret_tmp_chpl18;
#line 531 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl___PLUS_(&call_tmp_chpl32, &call_tmp_chpl33, &ret_tmp_chpl19, _ln_chpl, _fn_chpl);
#line 531 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  call_tmp_chpl34 = ret_tmp_chpl19;
#line 531 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl___PLUS_(&call_tmp_chpl34, &_str_literal_180_chpl, &ret_tmp_chpl20, _ln_chpl, _fn_chpl);
#line 531 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  call_tmp_chpl35 = ret_tmp_chpl20;
#line 531 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  s_chpl = call_tmp_chpl35;
#line 531 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  i_x_chpl3 = &call_tmp_chpl34;
#line 531 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  deinit_chpl95(i_x_chpl3, _ln_chpl, _fn_chpl);
#line 531 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  i_x_chpl4 = &call_tmp_chpl33;
#line 531 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  deinit_chpl95(i_x_chpl4, _ln_chpl, _fn_chpl);
#line 531 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  i_x_chpl5 = &call_tmp_chpl32;
#line 531 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  deinit_chpl95(i_x_chpl5, _ln_chpl, _fn_chpl);
#line 531 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  i_x_chpl6 = &call_tmp_chpl31;
#line 531 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  deinit_chpl95(i_x_chpl6, _ln_chpl, _fn_chpl);
#line 531 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  i_x_chpl7 = &call_tmp_chpl30;
#line 531 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  deinit_chpl95(i_x_chpl7, _ln_chpl, _fn_chpl);
#line 531 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  i_x_chpl8 = &call_tmp_chpl29;
#line 531 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  deinit_chpl95(i_x_chpl8, _ln_chpl, _fn_chpl);
#line 531 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  i_x_chpl9 = &call_tmp_chpl28;
#line 531 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  deinit_chpl95(i_x_chpl9, _ln_chpl, _fn_chpl);
#line 531 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  i_x_chpl10 = &call_tmp_chpl27;
#line 531 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  deinit_chpl95(i_x_chpl10, _ln_chpl, _fn_chpl);
#line 531 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  i_x_chpl11 = &call_tmp_chpl26;
#line 531 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  deinit_chpl95(i_x_chpl11, _ln_chpl, _fn_chpl);
#line 531 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  i_x_chpl12 = &call_tmp_chpl25;
#line 531 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  deinit_chpl95(i_x_chpl12, _ln_chpl, _fn_chpl);
#line 531 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  i_x_chpl13 = &call_tmp_chpl24;
#line 531 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  deinit_chpl95(i_x_chpl13, _ln_chpl, _fn_chpl);
#line 531 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  i_x_chpl14 = &call_tmp_chpl23;
#line 531 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  deinit_chpl95(i_x_chpl14, _ln_chpl, _fn_chpl);
#line 531 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  i_x_chpl15 = &call_tmp_chpl22;
#line 531 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  deinit_chpl95(i_x_chpl15, _ln_chpl, _fn_chpl);
#line 534 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_error_preformatted(((c_ptrConst_int8_t_chpl)((&s_chpl)->buff)));
#line 512 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  i_x_chpl16 = &s_chpl;
#line 512 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  deinit_chpl95(i_x_chpl16, _ln_chpl, _fn_chpl);
#line 512 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  i_x_chpl17 = &thrownFileS_chpl;
#line 512 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  deinit_chpl95(i_x_chpl17, _ln_chpl, _fn_chpl);
#line 512 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  i_x_chpl18 = &myFileS_chpl;
#line 512 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  deinit_chpl95(i_x_chpl18, _ln_chpl, _fn_chpl);
#line 512 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  return;
#line 512 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
}

#line 541 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
/* Errors.chpl:541 */
#line 541 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
static void chpl_propagate_error(Error_chpl err_chpl8,
#line 541 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                                 int64_t _ln_chpl,
#line 541 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                                 int32_t _fn_chpl) {
#line 542 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_uncaught_error(err_chpl8, _ln_chpl, _fn_chpl);
#line 541 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  return;
#line 541 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
}

#line 548 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
/* Errors.chpl:548 */
#line 548 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
static Error_chpl chpl_forall_error(Error_chpl err_chpl8,
#line 548 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                                    int64_t _ln_chpl,
#line 548 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                                    int32_t _fn_chpl) {
#line 548 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  Error_chpl ret_chpl = NULL;
#line 549 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  TaskErrors_chpl _formal_type_tmp__chpl = NULL;
#line 549 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  TaskErrors_chpl ret_chpl2 = NULL;
#line 552 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  TaskErrors_chpl new_temp_chpl = NULL;
#line 552 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  int32_t chpl_macro_tmp_13;
#line 549 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  if (((RootClass_chpl)(err_chpl8)) == nil) /* ZLINE: 549 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 549 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  {
#line 549 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    ret_chpl2 = ((TaskErrors_chpl)(nil));
#line 549 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    goto _end__COLON__chpl;
#line 549 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  }
#line 549 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_macro_tmp_13 = ((RootClass_chpl)(err_chpl8))->chpl__cid;
#line 549 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  ret_chpl2 = ( (((chpl__cid_TaskErrors_chpl <= chpl_macro_tmp_13) && (chpl_macro_tmp_13 <= chpl_subclass_max_id[chpl__cid_TaskErrors_chpl])))?(((TaskErrors_chpl)(err_chpl8))):(((TaskErrors_chpl)(NULL))) );
#line 549 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _end__COLON__chpl:;
#line 549 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  if (((RootClass_chpl)(ret_chpl2)) != nil) /* ZLINE: 549 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 549 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  {
#line 550 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    ret_chpl = err_chpl8;
#line 550 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    goto _end_chpl_forall_error_chpl;
#line 550 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  }
#line 552 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  new_temp_chpl = _new_chpl62(err_chpl8, _ln_chpl, _fn_chpl);
#line 552 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  ret_chpl = ((Error_chpl)(new_temp_chpl));
#line 552 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _end_chpl_forall_error_chpl:;
#line 548 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  return ret_chpl;
#line 548 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
}

#line 568 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
/* Errors.chpl:568 */
#line 568 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
static void chpl_enum_cast_error(uint8_t casted_chpl,
#line 568 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                                 string_chpl * enumName_chpl,
#line 568 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                                 Error_chpl * error_out_chpl,
#line 568 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                                 int64_t _ln_chpl,
#line 568 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                                 int32_t _fn_chpl) {
#line 569 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl6;
#line 569 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl;
#line 569 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl7;
#line 569 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl2;
#line 569 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl8;
#line 569 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl3;
#line 569 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl9;
#line 569 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl4;
#line 569 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl10;
#line 569 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl5;
#line 569 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _owned_IllegalArgumentError_chpl call_tmp_chpl11;
#line 569 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  IllegalArgumentError_chpl new_temp_chpl = NULL;
#line 569 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _owned_IllegalArgumentError_chpl initTemp_chpl;
#line 569 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  Error_chpl call_tmp_chpl12 = NULL;
#line 569 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _owned_Error_chpl coerce_tmp_chpl3;
#line 569 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _owned_Error_chpl ret_tmp_chpl6;
#line 569 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref__owned_IllegalArgumentError_chpl2 i_x_chpl = NULL;
#line 569 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref__owned_Error_chpl2 i__retArg_chpl = NULL;
#line 569 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _owned_IllegalArgumentError_chpl _formal_tmp_x_chpl;
#line 569 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _owned_Error_chpl ret_chpl;
#line 569 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref__owned_IllegalArgumentError_chpl2 i_this_chpl = NULL;
#line 569 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  Error_chpl call_tmp_chpl13 = NULL;
#line 569 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref__owned_IllegalArgumentError_chpl2 i_this_chpl2 = NULL;
#line 569 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _owned_Error_chpl call_tmp_chpl14;
#line 569 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _owned_Error_chpl initTemp_chpl2;
#line 569 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _owned_Error_chpl _formal_tmp_in_err_chpl;
#line 569 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl2 = NULL;
#line 569 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl3 = NULL;
#line 569 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl4 = NULL;
#line 569 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl5 = NULL;
#line 569 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl6 = NULL;
#line 569 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl___COLON_23(casted_chpl, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 569 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  call_tmp_chpl6 = ret_tmp_chpl;
#line 569 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl___PLUS_(&_str_literal_187_chpl, &call_tmp_chpl6, &ret_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 569 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  call_tmp_chpl7 = ret_tmp_chpl2;
#line 569 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl___PLUS_(&call_tmp_chpl7, &_str_literal_185_chpl, &ret_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 569 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  call_tmp_chpl8 = ret_tmp_chpl3;
#line 569 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl___PLUS_(&call_tmp_chpl8, enumName_chpl, &ret_tmp_chpl4, _ln_chpl, _fn_chpl);
#line 569 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  call_tmp_chpl9 = ret_tmp_chpl4;
#line 569 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl___PLUS_(&call_tmp_chpl9, &_str_literal_183_chpl, &ret_tmp_chpl5, _ln_chpl, _fn_chpl);
#line 569 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  call_tmp_chpl10 = ret_tmp_chpl5;
#line 569 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  new_temp_chpl = _new_chpl59(&call_tmp_chpl10, _ln_chpl, _fn_chpl);
#line 569 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  init_chpl179(&initTemp_chpl, new_temp_chpl);
#line 569 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  call_tmp_chpl11 = initTemp_chpl;
#line 569 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  i_x_chpl = &call_tmp_chpl11;
#line 569 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  i__retArg_chpl = &ret_tmp_chpl6;
#line 569 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _formal_tmp_x_chpl = *(i_x_chpl);
#line 569 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  i_this_chpl = &_formal_tmp_x_chpl;
#line 569 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  call_tmp_chpl13 = ((Error_chpl)((i_this_chpl)->chpl_p));
#line 569 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  i_this_chpl2 = &_formal_tmp_x_chpl;
#line 569 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  (i_this_chpl2)->chpl_p = ((IllegalArgumentError_chpl)(nil));
#line 569 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  if (((RootClass_chpl)(call_tmp_chpl13)) == nil) /* ZLINE: 569 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 569 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  {
#line 569 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    nilCheckHalt_chpl(&_str_literal_52_chpl, _ln_chpl, _fn_chpl);
#line 569 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  }
#line 569 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  init_chpl188(&initTemp_chpl2, call_tmp_chpl13);
#line 569 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  call_tmp_chpl14 = initTemp_chpl2;
#line 569 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  ret_chpl = call_tmp_chpl14;
#line 569 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl__autoDestroy59(&_formal_tmp_x_chpl, _ln_chpl, _fn_chpl);
#line 569 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  *(i__retArg_chpl) = ret_chpl;
#line 569 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  coerce_tmp_chpl3 = ret_tmp_chpl6;
#line 569 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _formal_tmp_in_err_chpl = coerce_tmp_chpl3;
#line 569 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  call_tmp_chpl12 = chpl_fix_thrown_error(&_formal_tmp_in_err_chpl, _ln_chpl, _fn_chpl);
#line 569 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  *(error_out_chpl) = call_tmp_chpl12;
#line 569 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  i_x_chpl2 = &call_tmp_chpl10;
#line 569 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  deinit_chpl95(i_x_chpl2, _ln_chpl, _fn_chpl);
#line 569 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  i_x_chpl3 = &call_tmp_chpl9;
#line 569 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  deinit_chpl95(i_x_chpl3, _ln_chpl, _fn_chpl);
#line 569 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  i_x_chpl4 = &call_tmp_chpl8;
#line 569 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  deinit_chpl95(i_x_chpl4, _ln_chpl, _fn_chpl);
#line 569 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  i_x_chpl5 = &call_tmp_chpl7;
#line 569 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  deinit_chpl95(i_x_chpl5, _ln_chpl, _fn_chpl);
#line 569 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  i_x_chpl6 = &call_tmp_chpl6;
#line 569 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  deinit_chpl95(i_x_chpl6, _ln_chpl, _fn_chpl);
#line 568 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  return;
#line 568 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
}

#line 606 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
/* Errors.chpl:606 */
#line 606 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
static void assert_chpl(chpl_bool test_chpl,
#line 606 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                        int64_t _ln_chpl,
#line 606 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                        int32_t _fn_chpl) {
#line 608 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl inlineImm_chpl;
#line 607 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  if (! test_chpl) /* ZLINE: 607 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 607 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  {
#line 608 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    inlineImm_chpl = _str_literal_195_chpl;
#line 608 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    chpl_error(((c_ptrConst_int8_t_chpl)((&inlineImm_chpl)->buff)), _ln_chpl, _fn_chpl);
#line 608 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  }
#line 606 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  return;
#line 606 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
}

#line 627 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
/* Errors.chpl:627 */
#line 627 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
static void assert_chpl2(chpl_bool test_chpl,
#line 627 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                         string_chpl * _e0_args_chpl,
#line 627 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                         int64_t _ln_chpl,
#line 627 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                         int32_t _fn_chpl) {
#line 629 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl6;
#line 629 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl;
#line 629 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl7;
#line 629 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl2;
#line 629 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl tmpstring_chpl;
#line 629 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl2 = NULL;
#line 628 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  if (! test_chpl) /* ZLINE: 628 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 628 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  {
#line 629 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    chpl_stringify_wrapper11(_e0_args_chpl, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 629 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    call_tmp_chpl6 = ret_tmp_chpl;
#line 629 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    chpl___PLUS_(&_str_literal_196_chpl, &call_tmp_chpl6, &ret_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 629 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    call_tmp_chpl7 = ret_tmp_chpl2;
#line 629 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    tmpstring_chpl = call_tmp_chpl7;
#line 629 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    i_x_chpl = &call_tmp_chpl6;
#line 629 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    deinit_chpl95(i_x_chpl, _ln_chpl, _fn_chpl);
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    chpl_error(((c_ptrConst_int8_t_chpl)((&tmpstring_chpl)->buff)), _ln_chpl, _fn_chpl);
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    i_x_chpl2 = &tmpstring_chpl;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    deinit_chpl95(i_x_chpl2, _ln_chpl, _fn_chpl);
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  }
#line 627 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  return;
#line 627 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
}

#line 627 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
/* Errors.chpl:627 */
#line 627 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
static void assert_chpl3(chpl_bool test_chpl,
#line 627 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                         string_chpl * _e0_args_chpl,
#line 627 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                         uint64_t _e1_args_chpl,
#line 627 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                         string_chpl * _e2_args_chpl,
#line 627 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                         uint64_t _e3_args_chpl,
#line 627 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                         int64_t _ln_chpl,
#line 627 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                         int32_t _fn_chpl) {
#line 629 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl6;
#line 629 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl;
#line 629 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl7;
#line 629 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl2;
#line 629 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl tmpstring_chpl;
#line 629 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl2 = NULL;
#line 628 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  if (! test_chpl) /* ZLINE: 628 /Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl */
#line 628 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  {
#line 629 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    chpl_stringify_wrapper14(_e0_args_chpl, _e1_args_chpl, _e2_args_chpl, _e3_args_chpl, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 629 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    call_tmp_chpl6 = ret_tmp_chpl;
#line 629 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    chpl___PLUS_(&_str_literal_196_chpl, &call_tmp_chpl6, &ret_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 629 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    call_tmp_chpl7 = ret_tmp_chpl2;
#line 629 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    tmpstring_chpl = call_tmp_chpl7;
#line 629 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    i_x_chpl = &call_tmp_chpl6;
#line 629 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    deinit_chpl95(i_x_chpl, _ln_chpl, _fn_chpl);
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    chpl_error(((c_ptrConst_int8_t_chpl)((&tmpstring_chpl)->buff)), _ln_chpl, _fn_chpl);
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    i_x_chpl2 = &tmpstring_chpl;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
    deinit_chpl95(i_x_chpl2, _ln_chpl, _fn_chpl);
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  }
#line 627 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  return;
#line 627 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
}

#line 682 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
/* Errors.chpl:682 */
#line 682 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
static void compilerAssert_chpl(void) {
#line 682 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  return;
#line 682 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
}

#line 700 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
/* Errors.chpl:700 */
#line 700 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
static void compilerAssert_chpl2(void) {
#line 700 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  return;
#line 700 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
}

#line 749 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
/* Errors.chpl:749 */
#line 749 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
static void halt_chpl(string_chpl * msg_chpl,
#line 749 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                      int64_t _ln_chpl,
#line 749 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                      int32_t _fn_chpl) {
#line 750 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl6;
#line 750 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_chpl;
#line 750 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_chpl2;
#line 749 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 750 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  init_chpl194(&ret_chpl2, _ln_chpl, _fn_chpl);
#line 750 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  (&ret_chpl2)->cachedNumCodepoints = (msg_chpl)->cachedNumCodepoints;
#line 750 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  initWithBorrowedBuffer_chpl(&ret_chpl2, msg_chpl, _ln_chpl, _fn_chpl);
#line 750 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  ret_chpl = ret_chpl2;
#line 750 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  call_tmp_chpl6 = ret_chpl;
#line 750 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  halt_chpl17(((c_ptrConst_int8_t_chpl)((&call_tmp_chpl6)->buff)), _ln_chpl, _fn_chpl);
#line 749 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  i_x_chpl = &call_tmp_chpl6;
#line 749 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  deinit_chpl95(i_x_chpl, _ln_chpl, _fn_chpl);
#line 749 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  return;
#line 749 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
}

#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
/* Errors.chpl:763 */
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
static void halt_chpl17(int8_t * _e0_args_chpl,
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                        int64_t _ln_chpl,
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                        int32_t _fn_chpl) {
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl6;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl7;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl2;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl tmpstring_chpl;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl2 = NULL;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_stringify_wrapper17(_e0_args_chpl, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  call_tmp_chpl6 = ret_tmp_chpl;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl___PLUS_(&_str_literal_199_chpl, &call_tmp_chpl6, &ret_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  call_tmp_chpl7 = ret_tmp_chpl2;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  tmpstring_chpl = call_tmp_chpl7;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  i_x_chpl = &call_tmp_chpl6;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  deinit_chpl95(i_x_chpl, _ln_chpl, _fn_chpl);
#line 765 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_error(((c_ptrConst_int8_t_chpl)((&tmpstring_chpl)->buff)), _ln_chpl, _fn_chpl);
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  i_x_chpl2 = &tmpstring_chpl;
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  deinit_chpl95(i_x_chpl2, _ln_chpl, _fn_chpl);
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  return;
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
}

#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
/* Errors.chpl:763 */
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
static void halt_chpl14(int64_t _e0_args_chpl,
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                        int64_t _ln_chpl,
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                        int32_t _fn_chpl) {
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl6;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl7;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl2;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl tmpstring_chpl;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl2 = NULL;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_stringify_wrapper6(_e0_args_chpl, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  call_tmp_chpl6 = ret_tmp_chpl;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl___PLUS_(&_str_literal_199_chpl, &call_tmp_chpl6, &ret_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  call_tmp_chpl7 = ret_tmp_chpl2;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  tmpstring_chpl = call_tmp_chpl7;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  i_x_chpl = &call_tmp_chpl6;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  deinit_chpl95(i_x_chpl, _ln_chpl, _fn_chpl);
#line 765 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_error(((c_ptrConst_int8_t_chpl)((&tmpstring_chpl)->buff)), _ln_chpl, _fn_chpl);
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  i_x_chpl2 = &tmpstring_chpl;
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  deinit_chpl95(i_x_chpl2, _ln_chpl, _fn_chpl);
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  return;
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
}

#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
/* Errors.chpl:763 */
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
static void halt_chpl3(string_chpl * _e0_args_chpl,
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                       int64_t _e1_args_chpl,
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                       string_chpl * _e2_args_chpl,
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                       int64_t _ln_chpl,
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                       int32_t _fn_chpl) {
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl6;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl7;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl2;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl tmpstring_chpl;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl2 = NULL;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_stringify_wrapper7(_e0_args_chpl, _e1_args_chpl, _e2_args_chpl, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  call_tmp_chpl6 = ret_tmp_chpl;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl___PLUS_(&_str_literal_199_chpl, &call_tmp_chpl6, &ret_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  call_tmp_chpl7 = ret_tmp_chpl2;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  tmpstring_chpl = call_tmp_chpl7;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  i_x_chpl = &call_tmp_chpl6;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  deinit_chpl95(i_x_chpl, _ln_chpl, _fn_chpl);
#line 765 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_error(((c_ptrConst_int8_t_chpl)((&tmpstring_chpl)->buff)), _ln_chpl, _fn_chpl);
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  i_x_chpl2 = &tmpstring_chpl;
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  deinit_chpl95(i_x_chpl2, _ln_chpl, _fn_chpl);
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  return;
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
}

#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
/* Errors.chpl:763 */
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
static void halt_chpl12(string_chpl * _e0_args_chpl,
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                        ArrayInit_chpl _e1_args_chpl,
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                        string_chpl * _e2_args_chpl,
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                        int64_t _ln_chpl,
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                        int32_t _fn_chpl) {
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl6;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl7;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl2;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl tmpstring_chpl;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl2 = NULL;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_stringify_wrapper9(_e0_args_chpl, _e1_args_chpl, _e2_args_chpl, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  call_tmp_chpl6 = ret_tmp_chpl;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl___PLUS_(&_str_literal_199_chpl, &call_tmp_chpl6, &ret_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  call_tmp_chpl7 = ret_tmp_chpl2;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  tmpstring_chpl = call_tmp_chpl7;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  i_x_chpl = &call_tmp_chpl6;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  deinit_chpl95(i_x_chpl, _ln_chpl, _fn_chpl);
#line 765 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_error(((c_ptrConst_int8_t_chpl)((&tmpstring_chpl)->buff)), _ln_chpl, _fn_chpl);
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  i_x_chpl2 = &tmpstring_chpl;
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  deinit_chpl95(i_x_chpl2, _ln_chpl, _fn_chpl);
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  return;
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
}

#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
/* Errors.chpl:763 */
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
static void halt_chpl2(string_chpl * _e0_args_chpl,
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                       int64_t _e1_args_chpl,
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                       string_chpl * _e2_args_chpl,
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                       int64_t _e3_args_chpl,
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                       string_chpl * _e4_args_chpl,
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                       int64_t _ln_chpl,
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                       int32_t _fn_chpl) {
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl6;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl7;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl2;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl tmpstring_chpl;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl2 = NULL;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_stringify_wrapper16(_e0_args_chpl, _e1_args_chpl, _e2_args_chpl, _e3_args_chpl, _e4_args_chpl, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  call_tmp_chpl6 = ret_tmp_chpl;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl___PLUS_(&_str_literal_199_chpl, &call_tmp_chpl6, &ret_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  call_tmp_chpl7 = ret_tmp_chpl2;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  tmpstring_chpl = call_tmp_chpl7;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  i_x_chpl = &call_tmp_chpl6;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  deinit_chpl95(i_x_chpl, _ln_chpl, _fn_chpl);
#line 765 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_error(((c_ptrConst_int8_t_chpl)((&tmpstring_chpl)->buff)), _ln_chpl, _fn_chpl);
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  i_x_chpl2 = &tmpstring_chpl;
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  deinit_chpl95(i_x_chpl2, _ln_chpl, _fn_chpl);
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  return;
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
}

#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
/* Errors.chpl:763 */
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
static void halt_chpl16(string_chpl * _e0_args_chpl,
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                        string_chpl * _e1_args_chpl,
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                        int64_t _e2_args_chpl,
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                        string_chpl * _e3_args_chpl,
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                        range_int64_t_both_one_chpl _e4_args_chpl,
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                        int64_t _ln_chpl,
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                        int32_t _fn_chpl) {
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl6;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl7;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl2;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl tmpstring_chpl;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl2 = NULL;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_stringify_wrapper12(_e0_args_chpl, _e1_args_chpl, _e2_args_chpl, _e3_args_chpl, _e4_args_chpl, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  call_tmp_chpl6 = ret_tmp_chpl;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl___PLUS_(&_str_literal_199_chpl, &call_tmp_chpl6, &ret_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  call_tmp_chpl7 = ret_tmp_chpl2;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  tmpstring_chpl = call_tmp_chpl7;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  i_x_chpl = &call_tmp_chpl6;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  deinit_chpl95(i_x_chpl, _ln_chpl, _fn_chpl);
#line 765 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_error(((c_ptrConst_int8_t_chpl)((&tmpstring_chpl)->buff)), _ln_chpl, _fn_chpl);
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  i_x_chpl2 = &tmpstring_chpl;
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  deinit_chpl95(i_x_chpl2, _ln_chpl, _fn_chpl);
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  return;
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
}

#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
/* Errors.chpl:763 */
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
static void halt_chpl9(string_chpl * _e0_args_chpl,
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                       string_chpl * _e1_args_chpl,
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                       range_int64_t_both_one_chpl _e2_args_chpl,
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                       string_chpl * _e3_args_chpl,
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                       range_int64_t_both_one_chpl _e4_args_chpl,
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                       int64_t _ln_chpl,
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                       int32_t _fn_chpl) {
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl6;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl7;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl2;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl tmpstring_chpl;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl2 = NULL;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_stringify_wrapper10(_e0_args_chpl, _e1_args_chpl, _e2_args_chpl, _e3_args_chpl, _e4_args_chpl, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  call_tmp_chpl6 = ret_tmp_chpl;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl___PLUS_(&_str_literal_199_chpl, &call_tmp_chpl6, &ret_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  call_tmp_chpl7 = ret_tmp_chpl2;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  tmpstring_chpl = call_tmp_chpl7;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  i_x_chpl = &call_tmp_chpl6;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  deinit_chpl95(i_x_chpl, _ln_chpl, _fn_chpl);
#line 765 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_error(((c_ptrConst_int8_t_chpl)((&tmpstring_chpl)->buff)), _ln_chpl, _fn_chpl);
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  i_x_chpl2 = &tmpstring_chpl;
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  deinit_chpl95(i_x_chpl2, _ln_chpl, _fn_chpl);
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  return;
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
}

#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
/* Errors.chpl:763 */
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
static void halt_chpl7(string_chpl * _e0_args_chpl,
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                       range_int64_t_both_one_chpl _e1_args_chpl,
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                       string_chpl * _e2_args_chpl,
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                       int64_t _e3_args_chpl,
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                       int64_t _ln_chpl,
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                       int32_t _fn_chpl) {
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl6;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl7;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl2;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl tmpstring_chpl;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl2 = NULL;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_stringify_wrapper13(_e0_args_chpl, _e1_args_chpl, _e2_args_chpl, _e3_args_chpl, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  call_tmp_chpl6 = ret_tmp_chpl;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl___PLUS_(&_str_literal_199_chpl, &call_tmp_chpl6, &ret_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  call_tmp_chpl7 = ret_tmp_chpl2;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  tmpstring_chpl = call_tmp_chpl7;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  i_x_chpl = &call_tmp_chpl6;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  deinit_chpl95(i_x_chpl, _ln_chpl, _fn_chpl);
#line 765 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_error(((c_ptrConst_int8_t_chpl)((&tmpstring_chpl)->buff)), _ln_chpl, _fn_chpl);
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  i_x_chpl2 = &tmpstring_chpl;
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  deinit_chpl95(i_x_chpl2, _ln_chpl, _fn_chpl);
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  return;
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
}

#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
/* Errors.chpl:763 */
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
static void halt_chpl21(string_chpl * _e0_args_chpl,
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                        range_byteIndex_both_one_chpl _e1_args_chpl,
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                        string_chpl * _e2_args_chpl,
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                        int64_t _e3_args_chpl,
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                        int64_t _ln_chpl,
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                        int32_t _fn_chpl) {
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl6;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl7;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl2;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl tmpstring_chpl;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl2 = NULL;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_stringify_wrapper22(_e0_args_chpl, _e1_args_chpl, _e2_args_chpl, _e3_args_chpl, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  call_tmp_chpl6 = ret_tmp_chpl;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl___PLUS_(&_str_literal_199_chpl, &call_tmp_chpl6, &ret_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  call_tmp_chpl7 = ret_tmp_chpl2;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  tmpstring_chpl = call_tmp_chpl7;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  i_x_chpl = &call_tmp_chpl6;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  deinit_chpl95(i_x_chpl, _ln_chpl, _fn_chpl);
#line 765 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_error(((c_ptrConst_int8_t_chpl)((&tmpstring_chpl)->buff)), _ln_chpl, _fn_chpl);
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  i_x_chpl2 = &tmpstring_chpl;
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  deinit_chpl95(i_x_chpl2, _ln_chpl, _fn_chpl);
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  return;
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
}

#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
/* Errors.chpl:763 */
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
static void halt_chpl4(string_chpl * _e0_args_chpl,
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                       int64_t _e1_args_chpl,
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                       string_chpl * _e2_args_chpl,
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                       int64_t _e3_args_chpl,
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                       int64_t _ln_chpl,
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                       int32_t _fn_chpl) {
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl6;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl7;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl2;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl tmpstring_chpl;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl2 = NULL;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_stringify_wrapper8(_e0_args_chpl, _e1_args_chpl, _e2_args_chpl, _e3_args_chpl, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  call_tmp_chpl6 = ret_tmp_chpl;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl___PLUS_(&_str_literal_199_chpl, &call_tmp_chpl6, &ret_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  call_tmp_chpl7 = ret_tmp_chpl2;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  tmpstring_chpl = call_tmp_chpl7;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  i_x_chpl = &call_tmp_chpl6;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  deinit_chpl95(i_x_chpl, _ln_chpl, _fn_chpl);
#line 765 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_error(((c_ptrConst_int8_t_chpl)((&tmpstring_chpl)->buff)), _ln_chpl, _fn_chpl);
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  i_x_chpl2 = &tmpstring_chpl;
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  deinit_chpl95(i_x_chpl2, _ln_chpl, _fn_chpl);
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  return;
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
}

#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
/* Errors.chpl:763 */
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
static void halt_chpl6(string_chpl * _e0_args_chpl,
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                       range_int64_t_low_one_chpl _e1_args_chpl,
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                       string_chpl * _e2_args_chpl,
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                       int64_t _e3_args_chpl,
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                       int64_t _ln_chpl,
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                       int32_t _fn_chpl) {
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl6;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl7;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl2;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl tmpstring_chpl;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl2 = NULL;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_stringify_wrapper15(_e0_args_chpl, _e1_args_chpl, _e2_args_chpl, _e3_args_chpl, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  call_tmp_chpl6 = ret_tmp_chpl;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl___PLUS_(&_str_literal_199_chpl, &call_tmp_chpl6, &ret_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  call_tmp_chpl7 = ret_tmp_chpl2;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  tmpstring_chpl = call_tmp_chpl7;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  i_x_chpl = &call_tmp_chpl6;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  deinit_chpl95(i_x_chpl, _ln_chpl, _fn_chpl);
#line 765 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_error(((c_ptrConst_int8_t_chpl)((&tmpstring_chpl)->buff)), _ln_chpl, _fn_chpl);
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  i_x_chpl2 = &tmpstring_chpl;
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  deinit_chpl95(i_x_chpl2, _ln_chpl, _fn_chpl);
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  return;
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
}

#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
/* Errors.chpl:763 */
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
static void halt_chpl13(string_chpl * _e0_args_chpl,
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                        range_byteIndex_low_one_chpl _e1_args_chpl,
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                        string_chpl * _e2_args_chpl,
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                        int64_t _e3_args_chpl,
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                        int64_t _ln_chpl,
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                        int32_t _fn_chpl) {
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl6;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl7;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl2;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl tmpstring_chpl;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl2 = NULL;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_stringify_wrapper19(_e0_args_chpl, _e1_args_chpl, _e2_args_chpl, _e3_args_chpl, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  call_tmp_chpl6 = ret_tmp_chpl;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl___PLUS_(&_str_literal_199_chpl, &call_tmp_chpl6, &ret_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  call_tmp_chpl7 = ret_tmp_chpl2;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  tmpstring_chpl = call_tmp_chpl7;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  i_x_chpl = &call_tmp_chpl6;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  deinit_chpl95(i_x_chpl, _ln_chpl, _fn_chpl);
#line 765 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_error(((c_ptrConst_int8_t_chpl)((&tmpstring_chpl)->buff)), _ln_chpl, _fn_chpl);
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  i_x_chpl2 = &tmpstring_chpl;
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  deinit_chpl95(i_x_chpl2, _ln_chpl, _fn_chpl);
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  return;
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
}

#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
/* Errors.chpl:763 */
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
static void halt_chpl18(string_chpl * _e0_args_chpl,
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                        range_byteIndex_high_one_chpl _e1_args_chpl,
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                        string_chpl * _e2_args_chpl,
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                        int64_t _e3_args_chpl,
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                        int64_t _ln_chpl,
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                        int32_t _fn_chpl) {
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl6;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl7;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl2;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl tmpstring_chpl;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl2 = NULL;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_stringify_wrapper20(_e0_args_chpl, _e1_args_chpl, _e2_args_chpl, _e3_args_chpl, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  call_tmp_chpl6 = ret_tmp_chpl;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl___PLUS_(&_str_literal_199_chpl, &call_tmp_chpl6, &ret_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  call_tmp_chpl7 = ret_tmp_chpl2;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  tmpstring_chpl = call_tmp_chpl7;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  i_x_chpl = &call_tmp_chpl6;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  deinit_chpl95(i_x_chpl, _ln_chpl, _fn_chpl);
#line 765 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_error(((c_ptrConst_int8_t_chpl)((&tmpstring_chpl)->buff)), _ln_chpl, _fn_chpl);
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  i_x_chpl2 = &tmpstring_chpl;
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  deinit_chpl95(i_x_chpl2, _ln_chpl, _fn_chpl);
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  return;
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
}

#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
/* Errors.chpl:763 */
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
static void halt_chpl8(string_chpl * _e0_args_chpl,
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                       string_chpl * _e1_args_chpl,
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                       string_chpl * _e2_args_chpl,
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                       string_chpl * _e3_args_chpl,
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                       int64_t _ln_chpl,
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                       int32_t _fn_chpl) {
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl6;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl7;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl2;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl tmpstring_chpl;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl2 = NULL;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_stringify_wrapper(_e0_args_chpl, _e1_args_chpl, _e2_args_chpl, _e3_args_chpl, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  call_tmp_chpl6 = ret_tmp_chpl;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl___PLUS_(&_str_literal_199_chpl, &call_tmp_chpl6, &ret_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  call_tmp_chpl7 = ret_tmp_chpl2;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  tmpstring_chpl = call_tmp_chpl7;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  i_x_chpl = &call_tmp_chpl6;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  deinit_chpl95(i_x_chpl, _ln_chpl, _fn_chpl);
#line 765 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_error(((c_ptrConst_int8_t_chpl)((&tmpstring_chpl)->buff)), _ln_chpl, _fn_chpl);
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  i_x_chpl2 = &tmpstring_chpl;
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  deinit_chpl95(i_x_chpl2, _ln_chpl, _fn_chpl);
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  return;
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
}

#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
/* Errors.chpl:763 */
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
static void halt_chpl20(string_chpl * _e0_args_chpl,
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                        int64_t _e1_args_chpl,
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                        string_chpl * _e2_args_chpl,
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                        range_int64_t_neither_one_chpl _e3_args_chpl,
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                        int64_t _ln_chpl,
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                        int32_t _fn_chpl) {
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl6;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl7;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl2;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl tmpstring_chpl;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl2 = NULL;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_stringify_wrapper3(_e0_args_chpl, _e1_args_chpl, _e2_args_chpl, _e3_args_chpl, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  call_tmp_chpl6 = ret_tmp_chpl;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl___PLUS_(&_str_literal_199_chpl, &call_tmp_chpl6, &ret_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  call_tmp_chpl7 = ret_tmp_chpl2;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  tmpstring_chpl = call_tmp_chpl7;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  i_x_chpl = &call_tmp_chpl6;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  deinit_chpl95(i_x_chpl, _ln_chpl, _fn_chpl);
#line 765 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_error(((c_ptrConst_int8_t_chpl)((&tmpstring_chpl)->buff)), _ln_chpl, _fn_chpl);
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  i_x_chpl2 = &tmpstring_chpl;
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  deinit_chpl95(i_x_chpl2, _ln_chpl, _fn_chpl);
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  return;
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
}

#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
/* Errors.chpl:763 */
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
static void halt_chpl10(string_chpl * _e0_args_chpl,
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                        int64_t _e1_args_chpl,
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                        string_chpl * _e2_args_chpl,
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                        range_int64_t_both_one_chpl _e3_args_chpl,
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                        int64_t _ln_chpl,
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                        int32_t _fn_chpl) {
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl6;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl7;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl2;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl tmpstring_chpl;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl2 = NULL;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_stringify_wrapper2(_e0_args_chpl, _e1_args_chpl, _e2_args_chpl, _e3_args_chpl, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  call_tmp_chpl6 = ret_tmp_chpl;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl___PLUS_(&_str_literal_199_chpl, &call_tmp_chpl6, &ret_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  call_tmp_chpl7 = ret_tmp_chpl2;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  tmpstring_chpl = call_tmp_chpl7;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  i_x_chpl = &call_tmp_chpl6;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  deinit_chpl95(i_x_chpl, _ln_chpl, _fn_chpl);
#line 765 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_error(((c_ptrConst_int8_t_chpl)((&tmpstring_chpl)->buff)), _ln_chpl, _fn_chpl);
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  i_x_chpl2 = &tmpstring_chpl;
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  deinit_chpl95(i_x_chpl2, _ln_chpl, _fn_chpl);
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  return;
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
}

#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
/* Errors.chpl:763 */
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
static void halt_chpl15(string_chpl * _e0_args_chpl,
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                        uint64_t _e1_args_chpl,
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                        int64_t _ln_chpl,
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                        int32_t _fn_chpl) {
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl6;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl7;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl2;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl tmpstring_chpl;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl2 = NULL;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_stringify_wrapper21(_e0_args_chpl, _e1_args_chpl, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  call_tmp_chpl6 = ret_tmp_chpl;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl___PLUS_(&_str_literal_199_chpl, &call_tmp_chpl6, &ret_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  call_tmp_chpl7 = ret_tmp_chpl2;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  tmpstring_chpl = call_tmp_chpl7;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  i_x_chpl = &call_tmp_chpl6;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  deinit_chpl95(i_x_chpl, _ln_chpl, _fn_chpl);
#line 765 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_error(((c_ptrConst_int8_t_chpl)((&tmpstring_chpl)->buff)), _ln_chpl, _fn_chpl);
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  i_x_chpl2 = &tmpstring_chpl;
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  deinit_chpl95(i_x_chpl2, _ln_chpl, _fn_chpl);
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  return;
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
}

#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
/* Errors.chpl:763 */
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
static void halt_chpl5(string_chpl * _e0_args_chpl,
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                       int64_t _e1_args_chpl,
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                       int64_t _ln_chpl,
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                       int32_t _fn_chpl) {
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl6;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl7;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl2;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl tmpstring_chpl;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl2 = NULL;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_stringify_wrapper4(_e0_args_chpl, _e1_args_chpl, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  call_tmp_chpl6 = ret_tmp_chpl;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl___PLUS_(&_str_literal_199_chpl, &call_tmp_chpl6, &ret_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  call_tmp_chpl7 = ret_tmp_chpl2;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  tmpstring_chpl = call_tmp_chpl7;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  i_x_chpl = &call_tmp_chpl6;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  deinit_chpl95(i_x_chpl, _ln_chpl, _fn_chpl);
#line 765 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_error(((c_ptrConst_int8_t_chpl)((&tmpstring_chpl)->buff)), _ln_chpl, _fn_chpl);
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  i_x_chpl2 = &tmpstring_chpl;
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  deinit_chpl95(i_x_chpl2, _ln_chpl, _fn_chpl);
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  return;
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
}

#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
/* Errors.chpl:763 */
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
static void halt_chpl11(string_chpl * _e0_args_chpl,
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                        string_chpl * _e1_args_chpl,
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                        string_chpl * _e2_args_chpl,
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                        string_chpl * _e3_args_chpl,
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                        string_chpl * _e4_args_chpl,
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                        string_chpl * _e5_args_chpl,
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                        string_chpl * _e6_args_chpl,
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                        string_chpl * _e7_args_chpl,
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                        string_chpl * _e8_args_chpl,
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                        int64_t _ln_chpl,
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                        int32_t _fn_chpl) {
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl6;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl7;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl2;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl tmpstring_chpl;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl2 = NULL;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_stringify_wrapper18(_e0_args_chpl, _e1_args_chpl, _e2_args_chpl, _e3_args_chpl, _e4_args_chpl, _e5_args_chpl, _e6_args_chpl, _e7_args_chpl, _e8_args_chpl, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  call_tmp_chpl6 = ret_tmp_chpl;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl___PLUS_(&_str_literal_199_chpl, &call_tmp_chpl6, &ret_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  call_tmp_chpl7 = ret_tmp_chpl2;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  tmpstring_chpl = call_tmp_chpl7;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  i_x_chpl = &call_tmp_chpl6;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  deinit_chpl95(i_x_chpl, _ln_chpl, _fn_chpl);
#line 765 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_error(((c_ptrConst_int8_t_chpl)((&tmpstring_chpl)->buff)), _ln_chpl, _fn_chpl);
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  i_x_chpl2 = &tmpstring_chpl;
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  deinit_chpl95(i_x_chpl2, _ln_chpl, _fn_chpl);
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  return;
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
}

#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
/* Errors.chpl:763 */
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
static void halt_chpl19(string_chpl * _e0_args_chpl,
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                        string_chpl * _e1_args_chpl,
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                        int64_t _e2_args_chpl,
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                        string_chpl * _e3_args_chpl,
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                        uint64_t _e4_args_chpl,
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                        string_chpl * _e5_args_chpl,
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                        uint64_t _e6_args_chpl,
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                        int64_t _ln_chpl,
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                        int32_t _fn_chpl) {
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl6;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl7;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_tmp_chpl2;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl tmpstring_chpl;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl2 = NULL;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_stringify_wrapper5(_e0_args_chpl, _e1_args_chpl, _e2_args_chpl, _e3_args_chpl, _e4_args_chpl, _e5_args_chpl, _e6_args_chpl, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  call_tmp_chpl6 = ret_tmp_chpl;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl___PLUS_(&_str_literal_199_chpl, &call_tmp_chpl6, &ret_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  call_tmp_chpl7 = ret_tmp_chpl2;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  tmpstring_chpl = call_tmp_chpl7;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  i_x_chpl = &call_tmp_chpl6;
#line 764 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  deinit_chpl95(i_x_chpl, _ln_chpl, _fn_chpl);
#line 765 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_error(((c_ptrConst_int8_t_chpl)((&tmpstring_chpl)->buff)), _ln_chpl, _fn_chpl);
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  i_x_chpl2 = &tmpstring_chpl;
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  deinit_chpl95(i_x_chpl2, _ln_chpl, _fn_chpl);
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  return;
#line 763 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
}

#line 773 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
/* Errors.chpl:773 */
#line 773 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
static void warning_chpl(string_chpl * msg_chpl,
#line 773 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                         int64_t _ln_chpl,
#line 773 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
                         int32_t _fn_chpl) {
#line 774 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl call_tmp_chpl6;
#line 774 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_chpl;
#line 774 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  string_chpl ret_chpl2;
#line 773 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 774 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  init_chpl194(&ret_chpl2, _ln_chpl, _fn_chpl);
#line 774 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  (&ret_chpl2)->cachedNumCodepoints = (msg_chpl)->cachedNumCodepoints;
#line 774 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  initWithBorrowedBuffer_chpl(&ret_chpl2, msg_chpl, _ln_chpl, _fn_chpl);
#line 774 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  ret_chpl = ret_chpl2;
#line 774 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  call_tmp_chpl6 = ret_chpl;
#line 774 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  chpl_warning(((c_ptrConst_int8_t_chpl)((&call_tmp_chpl6)->buff)), _ln_chpl, _fn_chpl);
#line 773 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  i_x_chpl = &call_tmp_chpl6;
#line 773 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  deinit_chpl95(i_x_chpl, _ln_chpl, _fn_chpl);
#line 773 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
  return;
#line 773 "/Users/iainmoncrief/Documents/chapel/modules/standard/Errors.chpl"
}

