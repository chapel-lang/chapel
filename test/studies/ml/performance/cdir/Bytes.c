#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/Bytes.chpl"
/* Bytes.chpl:26 */
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/Bytes.chpl"
static void chpl__init_Bytes(int64_t _ln_chpl,
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/Bytes.chpl"
                             int32_t _fn_chpl) {
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/Bytes.chpl"
  _ref_int32_t_chpl refIndentLevel_chpl = NULL;
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/Bytes.chpl"
  if (chpl__init_Bytes_p) /* ZLINE: 26 /Users/iainmoncrief/Documents/chapel/modules/internal/Bytes.chpl */
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/Bytes.chpl"
  {
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/Bytes.chpl"
    goto _exit_chpl__init_Bytes_chpl;
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/Bytes.chpl"
  }
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/Bytes.chpl"
  printModuleInit_chpl("%*s\n", "Bytes", INT64(5), _ln_chpl, _fn_chpl);
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/Bytes.chpl"
  refIndentLevel_chpl = &moduleInitLevel_chpl;
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/Bytes.chpl"
  *(refIndentLevel_chpl) += INT64(1);
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/Bytes.chpl"
  chpl__init_Bytes_p = UINT8(true);
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/Bytes.chpl"
  {
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/Bytes.chpl"
    chpl__init_ByteBufferHelpers(_ln_chpl, _fn_chpl);
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/Bytes.chpl"
    chpl__init_BytesStringCommon(_ln_chpl, _fn_chpl);
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/Bytes.chpl"
    chpl__init_CTypes(_ln_chpl, _fn_chpl);
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/Bytes.chpl"
    chpl__init_POSIX(_ln_chpl, _fn_chpl);
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/Bytes.chpl"
    chpl__init_String(_ln_chpl, _fn_chpl);
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/Bytes.chpl"
  }
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/Bytes.chpl"
  *(refIndentLevel_chpl) -= INT64(1);
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/Bytes.chpl"
  _exit_chpl__init_Bytes_chpl:;
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/Bytes.chpl"
  return;
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/Bytes.chpl"
}

#line 135 "/Users/iainmoncrief/Documents/chapel/modules/internal/Bytes.chpl"
/* Bytes.chpl:135 */
#line 135 "/Users/iainmoncrief/Documents/chapel/modules/internal/Bytes.chpl"
static bytes_chpl chpl_createBytesWithLiteral(c_string_rehook buffer_chpl,
#line 135 "/Users/iainmoncrief/Documents/chapel/modules/internal/Bytes.chpl"
                                              int64_t offset_chpl,
#line 135 "/Users/iainmoncrief/Documents/chapel/modules/internal/Bytes.chpl"
                                              c_string_rehook x_chpl,
#line 135 "/Users/iainmoncrief/Documents/chapel/modules/internal/Bytes.chpl"
                                              int64_t length_chpl,
#line 135 "/Users/iainmoncrief/Documents/chapel/modules/internal/Bytes.chpl"
                                              int64_t _ln_chpl,
#line 135 "/Users/iainmoncrief/Documents/chapel/modules/internal/Bytes.chpl"
                                              int32_t _fn_chpl) {
#line 140 "/Users/iainmoncrief/Documents/chapel/modules/internal/Bytes.chpl"
  c_ptr_uint8_t_chpl call_tmp_chpl6 = NULL;
#line 140 "/Users/iainmoncrief/Documents/chapel/modules/internal/Bytes.chpl"
  c_ptr_uint8_t_chpl buf_chpl = NULL;
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/Bytes.chpl"
  c_ptr_uint8_t_chpl call_tmp_chpl7 = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/Bytes.chpl"
  c_ptr_void_chpl call_tmp_chpl8 = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/Bytes.chpl"
  c_ptr_void_chpl call_tmp_chpl9 = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/Bytes.chpl"
  _owned_IllegalArgumentError_chpl2 call_tmp_chpl10;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/Bytes.chpl"
  _owned_IllegalArgumentError_chpl2 ret_tmp_chpl;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/Bytes.chpl"
  _ref__owned_IllegalArgumentError_chpl i_arg_chpl = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/Bytes.chpl"
  IllegalArgumentError_chpl call_tmp_chpl11 = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/Bytes.chpl"
  string_chpl call_tmp_chpl12;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/Bytes.chpl"
  Error_chpl coerce_tmp_chpl3 = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/Bytes.chpl"
  string_chpl ret_tmp_chpl2;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/Bytes.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 145 "/Users/iainmoncrief/Documents/chapel/modules/internal/Bytes.chpl"
  _ref_uint8_t_chpl call_tmp_chpl13 = NULL;
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/Bytes.chpl"
  int64_t call_tmp_chpl14;
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/Bytes.chpl"
  bytes_chpl ret_chpl;
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/Bytes.chpl"
  bytes_chpl ret_chpl2;
#line 140 "/Users/iainmoncrief/Documents/chapel/modules/internal/Bytes.chpl"
  call_tmp_chpl6 = ((c_ptr_uint8_t_chpl)(((c_ptr_void_chpl)(buffer_chpl))));
#line 140 "/Users/iainmoncrief/Documents/chapel/modules/internal/Bytes.chpl"
  buf_chpl = call_tmp_chpl6;
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/Bytes.chpl"
  call_tmp_chpl7 = (call_tmp_chpl6 + offset_chpl);
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/Bytes.chpl"
  buf_chpl = call_tmp_chpl7;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/Bytes.chpl"
  call_tmp_chpl8 = ((c_ptr_void_chpl)(call_tmp_chpl7));
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/Bytes.chpl"
  call_tmp_chpl9 = ((c_ptr_void_chpl)(x_chpl));
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/Bytes.chpl"
  chpl_checkValue7(length_chpl, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/Bytes.chpl"
  call_tmp_chpl10 = ret_tmp_chpl;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/Bytes.chpl"
  i_arg_chpl = &call_tmp_chpl10;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/Bytes.chpl"
  call_tmp_chpl11 = borrow_chpl2(i_arg_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/Bytes.chpl"
  if (((RootClass_chpl)(call_tmp_chpl11)) != nil) /* ZLINE: 143 /Users/iainmoncrief/Documents/chapel/modules/internal/Bytes.chpl */
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/Bytes.chpl"
  {
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/Bytes.chpl"
    coerce_tmp_chpl3 = ((Error_chpl)(call_tmp_chpl11));
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/Bytes.chpl"
    chpl_check_nil(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/Bytes.chpl"
    ((void(*)(Error_chpl,
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/Bytes.chpl"
       string_chpl *,
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/Bytes.chpl"
       int64_t,
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/Bytes.chpl"
       int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(coerce_tmp_chpl3))->chpl__cid) + INT64(1))])(coerce_tmp_chpl3, &ret_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/Bytes.chpl"
    call_tmp_chpl12 = ret_tmp_chpl2;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/Bytes.chpl"
    safeCastCheckHalt_chpl(&call_tmp_chpl12, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/Bytes.chpl"
    i_x_chpl = &call_tmp_chpl12;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/Bytes.chpl"
    deinit_chpl95(i_x_chpl, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/Bytes.chpl"
  }
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/Bytes.chpl"
  chpl__autoDestroy62(&call_tmp_chpl10, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/Bytes.chpl"
  memcpy(call_tmp_chpl8, call_tmp_chpl9, ((uint64_t)(length_chpl)));
#line 145 "/Users/iainmoncrief/Documents/chapel/modules/internal/Bytes.chpl"
  call_tmp_chpl13 = (buf_chpl + length_chpl);
#line 145 "/Users/iainmoncrief/Documents/chapel/modules/internal/Bytes.chpl"
  *(call_tmp_chpl13) = ((uint8_t)(INT64(0)));
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/Bytes.chpl"
  call_tmp_chpl14 = (length_chpl + INT64(1));
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/Bytes.chpl"
  init_chpl19(&ret_chpl2, _ln_chpl, _fn_chpl);
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/Bytes.chpl"
  initWithBorrowedBuffer_chpl3(&ret_chpl2, call_tmp_chpl7, length_chpl, call_tmp_chpl14);
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/Bytes.chpl"
  ret_chpl = ret_chpl2;
#line 135 "/Users/iainmoncrief/Documents/chapel/modules/internal/Bytes.chpl"
  return ret_chpl;
#line 135 "/Users/iainmoncrief/Documents/chapel/modules/internal/Bytes.chpl"
}

#line 422 "/Users/iainmoncrief/Documents/chapel/modules/internal/Bytes.chpl"
/* Bytes.chpl:422 */
#line 422 "/Users/iainmoncrief/Documents/chapel/modules/internal/Bytes.chpl"
static void init_chpl19(bytes_chpl * this_chpl31,
#line 422 "/Users/iainmoncrief/Documents/chapel/modules/internal/Bytes.chpl"
                        int64_t _ln_chpl,
#line 422 "/Users/iainmoncrief/Documents/chapel/modules/internal/Bytes.chpl"
                        int32_t _fn_chpl) {
#line 422 "/Users/iainmoncrief/Documents/chapel/modules/internal/Bytes.chpl"
  c_ptr_uint8_t_chpl init_coerce_tmp_chpl = NULL;
#line 419 "/Users/iainmoncrief/Documents/chapel/modules/internal/Bytes.chpl"
  int32_t call_tmp_chpl6;
#line 422 "/Users/iainmoncrief/Documents/chapel/modules/internal/Bytes.chpl"
  (this_chpl31)->buffLen = INT64(0);
#line 422 "/Users/iainmoncrief/Documents/chapel/modules/internal/Bytes.chpl"
  (this_chpl31)->buffSize = INT64(0);
#line 422 "/Users/iainmoncrief/Documents/chapel/modules/internal/Bytes.chpl"
  init_coerce_tmp_chpl = ((c_ptr_uint8_t_chpl)(nil));
#line 422 "/Users/iainmoncrief/Documents/chapel/modules/internal/Bytes.chpl"
  (this_chpl31)->buff = init_coerce_tmp_chpl;
#line 422 "/Users/iainmoncrief/Documents/chapel/modules/internal/Bytes.chpl"
  (this_chpl31)->isOwned = UINT8(true);
#line 419 "/Users/iainmoncrief/Documents/chapel/modules/internal/Bytes.chpl"
  call_tmp_chpl6 = get_chpl_nodeID();
#line 422 "/Users/iainmoncrief/Documents/chapel/modules/internal/Bytes.chpl"
  (this_chpl31)->locale_id = call_tmp_chpl6;
#line 422 "/Users/iainmoncrief/Documents/chapel/modules/internal/Bytes.chpl"
  return;
#line 422 "/Users/iainmoncrief/Documents/chapel/modules/internal/Bytes.chpl"
}

#line 426 "/Users/iainmoncrief/Documents/chapel/modules/internal/Bytes.chpl"
/* Bytes.chpl:426 */
#line 426 "/Users/iainmoncrief/Documents/chapel/modules/internal/Bytes.chpl"
static void deinit_chpl3(bytes_chpl * this_chpl31,
#line 426 "/Users/iainmoncrief/Documents/chapel/modules/internal/Bytes.chpl"
                         int64_t _ln_chpl,
#line 426 "/Users/iainmoncrief/Documents/chapel/modules/internal/Bytes.chpl"
                         int32_t _fn_chpl) {
#line 427 "/Users/iainmoncrief/Documents/chapel/modules/internal/Bytes.chpl"
  chpl_bool tmp_chpl;
#line 427 "/Users/iainmoncrief/Documents/chapel/modules/internal/Bytes.chpl"
  if ((this_chpl31)->isOwned) /* ZLINE: 427 /Users/iainmoncrief/Documents/chapel/modules/internal/Bytes.chpl */
#line 427 "/Users/iainmoncrief/Documents/chapel/modules/internal/Bytes.chpl"
  {
#line 427 "/Users/iainmoncrief/Documents/chapel/modules/internal/Bytes.chpl"
    tmp_chpl = ((this_chpl31)->buff != nil);
#line 427 "/Users/iainmoncrief/Documents/chapel/modules/internal/Bytes.chpl"
  } else /* ZLINE: 427 /Users/iainmoncrief/Documents/chapel/modules/internal/Bytes.chpl */
#line 427 "/Users/iainmoncrief/Documents/chapel/modules/internal/Bytes.chpl"
  {
#line 427 "/Users/iainmoncrief/Documents/chapel/modules/internal/Bytes.chpl"
    tmp_chpl = UINT8(false);
#line 427 "/Users/iainmoncrief/Documents/chapel/modules/internal/Bytes.chpl"
  }
#line 427 "/Users/iainmoncrief/Documents/chapel/modules/internal/Bytes.chpl"
  if (tmp_chpl) /* ZLINE: 427 /Users/iainmoncrief/Documents/chapel/modules/internal/Bytes.chpl */
#line 427 "/Users/iainmoncrief/Documents/chapel/modules/internal/Bytes.chpl"
  {
#line 429 "/Users/iainmoncrief/Documents/chapel/modules/internal/Bytes.chpl"
    chpl_buildLocaleID((this_chpl31)->locale_id, c_sublocid_any, _ln_chpl, _fn_chpl);
#line 430 "/Users/iainmoncrief/Documents/chapel/modules/internal/Bytes.chpl"
    chpl_here_free(((c_ptr_void_chpl)((this_chpl31)->buff)), _ln_chpl, _fn_chpl);
#line 430 "/Users/iainmoncrief/Documents/chapel/modules/internal/Bytes.chpl"
  }
#line 426 "/Users/iainmoncrief/Documents/chapel/modules/internal/Bytes.chpl"
  return;
#line 426 "/Users/iainmoncrief/Documents/chapel/modules/internal/Bytes.chpl"
}

