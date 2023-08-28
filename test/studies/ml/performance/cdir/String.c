#line 43 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
/* String.chpl:43 */
#line 43 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
static void chpl__init_String(int64_t _ln_chpl,
#line 43 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
                              int32_t _fn_chpl) {
#line 43 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _ref_int32_t_chpl refIndentLevel_chpl = NULL;
#line 43 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  if (chpl__init_String_p) /* ZLINE: 43 /Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl */
#line 43 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  {
#line 43 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    goto _exit_chpl__init_String_chpl;
#line 43 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  }
#line 43 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  printModuleInit_chpl("%*s\n", "String", INT64(6), _ln_chpl, _fn_chpl);
#line 43 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  refIndentLevel_chpl = &moduleInitLevel_chpl;
#line 43 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  *(refIndentLevel_chpl) += INT64(1);
#line 43 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  chpl__init_String_p = UINT8(true);
#line 43 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  {
#line 43 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    chpl__init_CTypes(_ln_chpl, _fn_chpl);
#line 43 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    chpl__init_ByteBufferHelpers(_ln_chpl, _fn_chpl);
#line 43 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    chpl__init_BytesStringCommon(_ln_chpl, _fn_chpl);
#line 43 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    chpl__init_OS(_ln_chpl, _fn_chpl);
#line 43 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    chpl__init_StringCasts(_ln_chpl, _fn_chpl);
#line 43 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    chpl__init_NVStringFactory(_ln_chpl, _fn_chpl);
#line 43 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    chpl__init_POSIX(_ln_chpl, _fn_chpl);
#line 43 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  }
#line 43 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  *(refIndentLevel_chpl) -= INT64(1);
#line 43 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _exit_chpl__init_String_chpl:;
#line 43 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  return;
#line 43 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
}

#line 83 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
/* String.chpl:83 */
#line 83 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
static void init_chpl192(byteIndex_chpl * this_chpl31) {
#line 83 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  (this_chpl31)->_bindex = INT64(0);
#line 83 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  return;
#line 83 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
}

#line 88 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
/* String.chpl:88 */
#line 88 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
static void init_ASSIGN__chpl24(byteIndex_chpl * this_chpl31,
#line 88 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
                                int64_t i_chpl) {
#line 88 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  (this_chpl31)->_bindex = i_chpl;
#line 88 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  return;
#line 88 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
}

#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
/* String.chpl:94 */
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
static void chpl___COLON_18(byteIndex_chpl * val_chpl,
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
                            string_chpl * _retArg_chpl,
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
                            int64_t _ln_chpl,
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
                            int32_t _fn_chpl) {
#line 95 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  string_chpl call_tmp_chpl6;
#line 95 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  string_chpl ret_tmp_chpl;
#line 95 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  chpl___COLON_25((val_chpl)->_bindex, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 95 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  call_tmp_chpl6 = ret_tmp_chpl;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  *(_retArg_chpl) = call_tmp_chpl6;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  return;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
}

#line 106 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
/* String.chpl:106 */
#line 106 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
static void init_chpl193(codepointIndex_chpl * this_chpl31) {
#line 106 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  (this_chpl31)->_cpindex = INT64(0);
#line 106 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  return;
#line 106 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
}

#line 123 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
/* String.chpl:123 */
#line 123 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
static void chpl_build_bounded_range7(byteIndex_chpl * low_chpl2,
#line 123 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
                                      byteIndex_chpl * high_chpl,
#line 123 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
                                      range_byteIndex_both_one_chpl * _retArg_chpl) {
#line 124 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  range_byteIndex_both_one_chpl call_tmp_chpl6;
#line 124 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  range_byteIndex_both_one_chpl initTemp_chpl;
#line 124 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  init_chpl106(&initTemp_chpl, low_chpl2, high_chpl);
#line 124 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  call_tmp_chpl6 = initTemp_chpl;
#line 123 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  *(_retArg_chpl) = call_tmp_chpl6;
#line 123 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  return;
#line 123 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
}

#line 128 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
/* String.chpl:128 */
#line 128 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
static void chpl_build_low_bounded_range3(byteIndex_chpl * low_chpl2,
#line 128 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
                                          range_byteIndex_low_one_chpl * _retArg_chpl) {
#line 129 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  range_byteIndex_low_one_chpl call_tmp_chpl6;
#line 129 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  range_byteIndex_low_one_chpl initTemp_chpl;
#line 129 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  init_chpl110(&initTemp_chpl, low_chpl2);
#line 129 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  call_tmp_chpl6 = initTemp_chpl;
#line 128 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  *(_retArg_chpl) = call_tmp_chpl6;
#line 128 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  return;
#line 128 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
}

#line 133 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
/* String.chpl:133 */
#line 133 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
static void chpl_build_high_bounded_range(byteIndex_chpl * high_chpl,
#line 133 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
                                          range_byteIndex_high_one_chpl * _retArg_chpl) {
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  range_byteIndex_high_one_chpl call_tmp_chpl6;
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  range_byteIndex_high_one_chpl initTemp_chpl;
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  init_chpl111(&initTemp_chpl, high_chpl);
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  call_tmp_chpl6 = initTemp_chpl;
#line 133 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  *(_retArg_chpl) = call_tmp_chpl6;
#line 133 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  return;
#line 133 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
}

#line 352 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
/* String.chpl:352 */
#line 352 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
static int64_t validateEncoding_chpl(uint8_t * buf_chpl,
#line 352 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
                                     int64_t len_chpl,
#line 352 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
                                     Error_chpl * error_out_chpl,
#line 352 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
                                     int64_t _ln_chpl,
#line 352 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
                                     int32_t _fn_chpl) {
#line 352 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  int64_t ret_chpl;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  int64_t numCodepoints_chpl;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  int32_t call_tmp_chpl6;
#line 358 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _owned_DecodeError_chpl call_tmp_chpl7;
#line 358 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  DecodeError_chpl new_temp_chpl = NULL;
#line 358 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _owned_DecodeError_chpl initTemp_chpl;
#line 358 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  Error_chpl call_tmp_chpl8 = NULL;
#line 358 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _owned_Error_chpl coerce_tmp_chpl3;
#line 358 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _owned_Error_chpl ret_tmp_chpl;
#line 358 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _ref__owned_DecodeError_chpl i_x_chpl = NULL;
#line 358 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _ref__owned_Error_chpl2 i__retArg_chpl = NULL;
#line 358 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _owned_DecodeError_chpl _formal_tmp_x_chpl;
#line 358 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _owned_Error_chpl ret_chpl2;
#line 358 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _ref__owned_DecodeError_chpl i_this_chpl = NULL;
#line 358 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  Error_chpl call_tmp_chpl9 = NULL;
#line 358 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _ref__owned_DecodeError_chpl i_this_chpl2 = NULL;
#line 358 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _owned_Error_chpl call_tmp_chpl10;
#line 358 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _owned_Error_chpl initTemp_chpl2;
#line 358 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _owned_Error_chpl _formal_tmp_in_err_chpl;
#line 355 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  numCodepoints_chpl = INT64(0);
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  call_tmp_chpl6 = chpl_enc_validate_buf(buf_chpl, len_chpl, &numCodepoints_chpl);
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  if (call_tmp_chpl6 != ((int32_t)(INT64(0)))) /* ZLINE: 357 /Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl */
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  {
#line 358 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    new_temp_chpl = _new_chpl58(_ln_chpl, _fn_chpl);
#line 358 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    init_chpl183(&initTemp_chpl, new_temp_chpl);
#line 358 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    call_tmp_chpl7 = initTemp_chpl;
#line 358 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    i_x_chpl = &call_tmp_chpl7;
#line 358 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    i__retArg_chpl = &ret_tmp_chpl;
#line 358 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    _formal_tmp_x_chpl = *(i_x_chpl);
#line 358 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    i_this_chpl = &_formal_tmp_x_chpl;
#line 358 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    call_tmp_chpl9 = ((Error_chpl)((i_this_chpl)->chpl_p));
#line 358 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    i_this_chpl2 = &_formal_tmp_x_chpl;
#line 358 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    (i_this_chpl2)->chpl_p = ((DecodeError_chpl)(nil));
#line 358 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    if (((RootClass_chpl)(call_tmp_chpl9)) == nil) /* ZLINE: 358 /Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl */
#line 358 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    {
#line 358 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      nilCheckHalt_chpl(&_str_literal_52_chpl, _ln_chpl, _fn_chpl);
#line 358 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    }
#line 358 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    init_chpl188(&initTemp_chpl2, call_tmp_chpl9);
#line 358 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    call_tmp_chpl10 = initTemp_chpl2;
#line 358 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    ret_chpl2 = call_tmp_chpl10;
#line 358 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    chpl__autoDestroy55(&_formal_tmp_x_chpl, _ln_chpl, _fn_chpl);
#line 358 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    *(i__retArg_chpl) = ret_chpl2;
#line 358 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    coerce_tmp_chpl3 = ret_tmp_chpl;
#line 358 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    _formal_tmp_in_err_chpl = coerce_tmp_chpl3;
#line 358 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    call_tmp_chpl8 = chpl_fix_thrown_error(&_formal_tmp_in_err_chpl, _ln_chpl, _fn_chpl);
#line 358 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    *(error_out_chpl) = call_tmp_chpl8;
#line 358 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    goto _end_validateEncoding_chpl;
#line 358 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  }
#line 361 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  ret_chpl = numCodepoints_chpl;
#line 361 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _end_validateEncoding_chpl:;
#line 352 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  return ret_chpl;
#line 352 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
}

#line 472 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
/* String.chpl:472 */
#line 472 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
static string_chpl chpl_createStringWithLiteral(c_string_rehook buffer_chpl,
#line 472 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
                                                int64_t offset_chpl,
#line 472 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
                                                c_string_rehook x_chpl,
#line 472 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
                                                int64_t length_chpl,
#line 472 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
                                                int64_t numCodepoints_chpl,
#line 472 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
                                                int64_t _ln_chpl,
#line 472 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
                                                int32_t _fn_chpl) {
#line 478 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  c_ptr_uint8_t_chpl call_tmp_chpl6 = NULL;
#line 478 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  c_ptr_uint8_t_chpl buf_chpl = NULL;
#line 479 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  c_ptr_uint8_t_chpl call_tmp_chpl7 = NULL;
#line 481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  c_ptr_void_chpl call_tmp_chpl8 = NULL;
#line 481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  c_ptr_void_chpl call_tmp_chpl9 = NULL;
#line 481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _owned_IllegalArgumentError_chpl2 call_tmp_chpl10;
#line 481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _owned_IllegalArgumentError_chpl2 ret_tmp_chpl;
#line 481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _ref__owned_IllegalArgumentError_chpl i_arg_chpl = NULL;
#line 481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  IllegalArgumentError_chpl call_tmp_chpl11 = NULL;
#line 481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  string_chpl call_tmp_chpl12;
#line 481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  Error_chpl coerce_tmp_chpl3 = NULL;
#line 481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  string_chpl ret_tmp_chpl2;
#line 481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 482 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  c_ptr_void_chpl call_tmp_chpl13 = NULL;
#line 482 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  c_ptr_void_chpl call_tmp_chpl14 = NULL;
#line 482 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _owned_IllegalArgumentError_chpl2 call_tmp_chpl15;
#line 482 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _owned_IllegalArgumentError_chpl2 ret_tmp_chpl3;
#line 482 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _ref__owned_IllegalArgumentError_chpl i_arg_chpl2 = NULL;
#line 482 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  IllegalArgumentError_chpl call_tmp_chpl16 = NULL;
#line 482 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  string_chpl call_tmp_chpl17;
#line 482 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  Error_chpl coerce_tmp_chpl4 = NULL;
#line 482 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  string_chpl ret_tmp_chpl4;
#line 482 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _ref_string_chpl i_x_chpl2 = NULL;
#line 484 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _ref_uint8_t_chpl call_tmp_chpl18 = NULL;
#line 491 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  int64_t call_tmp_chpl19;
#line 489 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  string_chpl ret_chpl;
#line 489 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  string_chpl ret_chpl2;
#line 478 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  call_tmp_chpl6 = ((c_ptr_uint8_t_chpl)(((c_ptr_void_chpl)(buffer_chpl))));
#line 478 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  buf_chpl = call_tmp_chpl6;
#line 479 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  call_tmp_chpl7 = (call_tmp_chpl6 + offset_chpl);
#line 479 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  buf_chpl = call_tmp_chpl7;
#line 481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  call_tmp_chpl8 = ((c_ptr_void_chpl)(call_tmp_chpl7));
#line 481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  call_tmp_chpl9 = ((c_ptr_void_chpl)(x_chpl));
#line 481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  chpl_checkValue7(length_chpl, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  call_tmp_chpl10 = ret_tmp_chpl;
#line 481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  i_arg_chpl = &call_tmp_chpl10;
#line 481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  call_tmp_chpl11 = borrow_chpl2(i_arg_chpl);
#line 481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  if (((RootClass_chpl)(call_tmp_chpl11)) != nil) /* ZLINE: 481 /Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl */
#line 481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  {
#line 481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    coerce_tmp_chpl3 = ((Error_chpl)(call_tmp_chpl11));
#line 481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    chpl_check_nil(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    ((void(*)(Error_chpl,
#line 481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
       string_chpl *,
#line 481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
       int64_t,
#line 481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
       int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(coerce_tmp_chpl3))->chpl__cid) + INT64(1))])(coerce_tmp_chpl3, &ret_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    call_tmp_chpl12 = ret_tmp_chpl2;
#line 481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    safeCastCheckHalt_chpl(&call_tmp_chpl12, _ln_chpl, _fn_chpl);
#line 481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    i_x_chpl = &call_tmp_chpl12;
#line 481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    deinit_chpl95(i_x_chpl, _ln_chpl, _fn_chpl);
#line 481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  }
#line 481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  chpl__autoDestroy62(&call_tmp_chpl10, _ln_chpl, _fn_chpl);
#line 481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  memcpy(call_tmp_chpl8, call_tmp_chpl9, ((uint64_t)(length_chpl)));
#line 482 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  call_tmp_chpl13 = ((c_ptr_void_chpl)(call_tmp_chpl7));
#line 482 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  call_tmp_chpl14 = ((c_ptr_void_chpl)(x_chpl));
#line 482 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  chpl_checkValue7(length_chpl, &ret_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 482 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  call_tmp_chpl15 = ret_tmp_chpl3;
#line 482 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  i_arg_chpl2 = &call_tmp_chpl15;
#line 482 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  call_tmp_chpl16 = borrow_chpl2(i_arg_chpl2);
#line 482 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  if (((RootClass_chpl)(call_tmp_chpl16)) != nil) /* ZLINE: 482 /Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl */
#line 482 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  {
#line 482 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    coerce_tmp_chpl4 = ((Error_chpl)(call_tmp_chpl16));
#line 482 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    chpl_check_nil(coerce_tmp_chpl4, _ln_chpl, _fn_chpl);
#line 482 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    ((void(*)(Error_chpl,
#line 482 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
       string_chpl *,
#line 482 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
       int64_t,
#line 482 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
       int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(coerce_tmp_chpl4))->chpl__cid) + INT64(1))])(coerce_tmp_chpl4, &ret_tmp_chpl4, _ln_chpl, _fn_chpl);
#line 482 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    call_tmp_chpl17 = ret_tmp_chpl4;
#line 482 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    safeCastCheckHalt_chpl(&call_tmp_chpl17, _ln_chpl, _fn_chpl);
#line 482 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    i_x_chpl2 = &call_tmp_chpl17;
#line 482 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    deinit_chpl95(i_x_chpl2, _ln_chpl, _fn_chpl);
#line 482 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  }
#line 482 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  chpl__autoDestroy62(&call_tmp_chpl15, _ln_chpl, _fn_chpl);
#line 482 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  memcpy(call_tmp_chpl13, call_tmp_chpl14, ((uint64_t)(length_chpl)));
#line 484 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  call_tmp_chpl18 = (buf_chpl + length_chpl);
#line 484 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  *(call_tmp_chpl18) = ((uint8_t)(INT64(0)));
#line 491 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  call_tmp_chpl19 = (length_chpl + INT64(1));
#line 489 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  init_chpl194(&ret_chpl2, _ln_chpl, _fn_chpl);
#line 489 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  initWithBorrowedBuffer_chpl2(&ret_chpl2, call_tmp_chpl7, length_chpl, call_tmp_chpl19);
#line 489 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  (&ret_chpl2)->cachedNumCodepoints = numCodepoints_chpl;
#line 489 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  ret_chpl = ret_chpl2;
#line 472 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  return ret_chpl;
#line 472 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
}

#line 639 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
/* String.chpl:639 */
#line 639 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
static void createAdoptingBuffer_chpl(int8_t * x_chpl,
#line 639 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
                                      int64_t length_chpl,
#line 639 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
                                      Error_chpl * error_out_chpl,
#line 639 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
                                      string_chpl * _retArg_chpl,
#line 639 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
                                      int64_t _ln_chpl,
#line 639 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
                                      int32_t _fn_chpl) {
#line 639 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  string_chpl ret_chpl;
#line 641 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  c_ptr_uint8_t_chpl call_tmp_chpl6 = NULL;
#line 643 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  int64_t call_tmp_chpl7;
#line 641 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  Error_chpl error_chpl = NULL;
#line 641 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  string_chpl ret_chpl2;
#line 641 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  string_chpl ret_chpl3;
#line 641 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  int64_t call_tmp_chpl8;
#line 641 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  Error_chpl error_chpl2 = NULL;
#line 641 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 641 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _ref_string_chpl i_x_chpl2 = NULL;
#line 641 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  call_tmp_chpl6 = ((c_ptr_uint8_t_chpl)(x_chpl));
#line 643 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  call_tmp_chpl7 = (length_chpl + INT64(1));
#line 641 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  error_chpl = NULL;
#line 641 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  init_chpl194(&ret_chpl3, _ln_chpl, _fn_chpl);
#line 641 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  error_chpl2 = NULL;
#line 641 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  call_tmp_chpl8 = validateEncoding_chpl(call_tmp_chpl6, length_chpl, &error_chpl2, _ln_chpl, _fn_chpl);
#line 641 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  if (error_chpl2 != nil) /* ZLINE: 641 /Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl */
#line 641 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  {
#line 641 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    error_chpl = error_chpl2;
#line 641 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    i_x_chpl = &ret_chpl3;
#line 641 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    deinit_chpl95(i_x_chpl, _ln_chpl, _fn_chpl);
#line 641 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    goto _end_createAdoptingBuffer_chpl;
#line 641 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  }
#line 641 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  (&ret_chpl3)->cachedNumCodepoints = call_tmp_chpl8;
#line 641 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  i_x_chpl2 = &ret_chpl3;
#line 641 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  (i_x_chpl2)->isOwned = UINT8(true);
#line 641 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  (i_x_chpl2)->buff = call_tmp_chpl6;
#line 641 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  (i_x_chpl2)->buffSize = call_tmp_chpl7;
#line 641 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  (i_x_chpl2)->buffLen = length_chpl;
#line 641 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  ret_chpl2 = ret_chpl3;
#line 641 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _end_createAdoptingBuffer_chpl:;
#line 641 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  if (error_chpl != nil) /* ZLINE: 641 /Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl */
#line 641 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  {
#line 641 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    *(error_out_chpl) = error_chpl;
#line 641 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    goto _endcreateAdoptingBuffer_chpl;
#line 641 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  }
#line 641 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  ret_chpl = ret_chpl2;
#line 641 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _endcreateAdoptingBuffer_chpl:;
#line 639 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  *(_retArg_chpl) = ret_chpl;
#line 639 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  return;
#line 639 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
}

#line 838 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
/* String.chpl:838 */
#line 838 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
static void createCopyingBuffer_chpl(uint8_t * x_chpl,
#line 838 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
                                     int64_t length_chpl,
#line 838 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
                                     int64_t size_chpl9,
#line 838 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
                                     decodePolicy_chpl policy_chpl,
#line 838 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
                                     Error_chpl * error_out_chpl,
#line 838 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
                                     string_chpl * _retArg_chpl,
#line 838 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
                                     int64_t _ln_chpl,
#line 838 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
                                     int32_t _fn_chpl) {
#line 838 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  string_chpl ret_chpl;
#line 848 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  string_chpl call_tmp_chpl6;
#line 848 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  Error_chpl error_chpl = NULL;
#line 848 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  string_chpl ret_tmp_chpl;
#line 848 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  error_chpl = NULL;
#line 848 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  decodeByteBuffer_chpl(x_chpl, length_chpl, policy_chpl, &error_chpl, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 848 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  call_tmp_chpl6 = ret_tmp_chpl;
#line 848 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  if (error_chpl != nil) /* ZLINE: 848 /Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl */
#line 848 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  {
#line 848 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    *(error_out_chpl) = error_chpl;
#line 848 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    goto _endcreateCopyingBuffer_chpl;
#line 848 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  }
#line 848 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  ret_chpl = call_tmp_chpl6;
#line 848 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _endcreateCopyingBuffer_chpl:;
#line 838 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  *(_retArg_chpl) = ret_chpl;
#line 838 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  return;
#line 838 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
}

#line 926 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
/* String.chpl:926 */
#line 926 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
static void init_chpl194(string_chpl * this_chpl31,
#line 926 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
                         int64_t _ln_chpl,
#line 926 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
                         int32_t _fn_chpl) {
#line 926 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  c_ptr_uint8_t_chpl init_coerce_tmp_chpl = NULL;
#line 923 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  int32_t call_tmp_chpl6;
#line 926 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  (this_chpl31)->buffLen = INT64(0);
#line 926 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  (this_chpl31)->buffSize = INT64(0);
#line 926 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  (this_chpl31)->cachedNumCodepoints = INT64(0);
#line 926 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  init_coerce_tmp_chpl = ((c_ptr_uint8_t_chpl)(nil));
#line 926 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  (this_chpl31)->buff = init_coerce_tmp_chpl;
#line 926 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  (this_chpl31)->isOwned = UINT8(true);
#line 926 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  (this_chpl31)->hasEscapes = UINT8(false);
#line 923 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  call_tmp_chpl6 = get_chpl_nodeID();
#line 926 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  (this_chpl31)->locale_id = call_tmp_chpl6;
#line 926 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  return;
#line 926 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
}

#line 930 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
/* String.chpl:930 */
#line 930 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
static void init_ASSIGN__chpl25(string_chpl * this_chpl31,
#line 930 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
                                string_chpl * s_chpl,
#line 930 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
                                int64_t _ln_chpl,
#line 930 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
                                int32_t _fn_chpl) {
#line 931 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  c_ptr_uint8_t_chpl init_coerce_tmp_chpl = NULL;
#line 923 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  int32_t call_tmp_chpl6;
#line 932 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  int64_t ret_chpl;
#line 932 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  int64_t otherLen_chpl;
#line 932 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  c_ptr_uint8_t_chpl coerce_tmp_chpl3 = NULL;
#line 932 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  int64_t call_tmp_chpl7;
#line 932 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  int64_t tmp_chpl;
#line 932 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  int16_t call_tmp_chpl8;
#line 932 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  c_ptr_void_chpl cast_tmp_chpl = NULL;
#line 932 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  c_ptr_uint8_t_chpl call_tmp_chpl9 = NULL;
#line 932 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  c_ptr_uint8_t_chpl dst_chpl = NULL;
#line 932 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  c_ptr_uint8_t_chpl call_tmp_chpl10 = NULL;
#line 932 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  c_ptr_uint8_t_chpl call_tmp_chpl11 = NULL;
#line 932 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _owned_IllegalArgumentError_chpl2 call_tmp_chpl12;
#line 932 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _owned_IllegalArgumentError_chpl2 ret_tmp_chpl;
#line 932 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _ref__owned_IllegalArgumentError_chpl i_arg_chpl = NULL;
#line 932 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  IllegalArgumentError_chpl call_tmp_chpl13 = NULL;
#line 932 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  string_chpl call_tmp_chpl14;
#line 932 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  Error_chpl coerce_tmp_chpl4 = NULL;
#line 932 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  string_chpl ret_tmp_chpl2;
#line 932 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 932 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _ref_uint8_t_chpl call_tmp_chpl15 = NULL;
#line 932 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  c_ptr_uint8_t_chpl coerce_tmp_chpl5 = NULL;
#line 932 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  int64_t coerce_tmp_chpl6;
#line 932 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _ref_uint8_t_chpl call_tmp_chpl16 = NULL;
#line 931 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  (this_chpl31)->buffLen = INT64(0);
#line 931 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  (this_chpl31)->buffSize = INT64(0);
#line 931 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  (this_chpl31)->cachedNumCodepoints = INT64(0);
#line 931 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  init_coerce_tmp_chpl = ((c_ptr_uint8_t_chpl)(nil));
#line 931 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  (this_chpl31)->buff = init_coerce_tmp_chpl;
#line 931 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  (this_chpl31)->isOwned = UINT8(true);
#line 931 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  (this_chpl31)->hasEscapes = UINT8(false);
#line 923 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  call_tmp_chpl6 = get_chpl_nodeID();
#line 931 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  (this_chpl31)->locale_id = call_tmp_chpl6;
#line 932 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  get_chpl_nodeID();
#line 932 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  ret_chpl = (s_chpl)->buffLen;
#line 932 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  otherLen_chpl = ret_chpl;
#line 932 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  (this_chpl31)->isOwned = UINT8(true);
#line 932 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  (this_chpl31)->hasEscapes = (s_chpl)->hasEscapes;
#line 932 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  (this_chpl31)->cachedNumCodepoints = (s_chpl)->cachedNumCodepoints;
#line 932 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  if (ret_chpl > INT64(0)) /* ZLINE: 932 /Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl */
#line 932 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  {
#line 932 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    (this_chpl31)->buffLen = ret_chpl;
#line 932 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    coerce_tmp_chpl3 = (s_chpl)->buff;
#line 932 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    call_tmp_chpl7 = chpl_here_good_alloc_size(((int64_t)((ret_chpl + INT64(1)))), _ln_chpl, _fn_chpl);
#line 932 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    if (call_tmp_chpl7 > INT64(0)) /* ZLINE: 932 /Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl */
#line 932 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    {
#line 932 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      tmp_chpl = call_tmp_chpl7;
#line 932 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    } else /* ZLINE: 932 /Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl */
#line 932 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    {
#line 932 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      tmp_chpl = INT64(0);
#line 932 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    }
#line 932 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    call_tmp_chpl8 = offset_STR_COPY_DATA_chpl();
#line 932 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    cast_tmp_chpl = chpl_here_alloc(tmp_chpl, call_tmp_chpl8, _ln_chpl, _fn_chpl);
#line 932 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    call_tmp_chpl9 = ((c_ptr_uint8_t_chpl)(cast_tmp_chpl));
#line 932 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    dst_chpl = call_tmp_chpl9;
#line 932 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    call_tmp_chpl10 = (call_tmp_chpl9 + INT64(0));
#line 932 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    call_tmp_chpl11 = (coerce_tmp_chpl3 + INT64(0));
#line 932 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    chpl_checkValue7(ret_chpl, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 932 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    call_tmp_chpl12 = ret_tmp_chpl;
#line 932 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    i_arg_chpl = &call_tmp_chpl12;
#line 932 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    call_tmp_chpl13 = borrow_chpl2(i_arg_chpl);
#line 932 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    if (((RootClass_chpl)(call_tmp_chpl13)) != nil) /* ZLINE: 932 /Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl */
#line 932 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    {
#line 932 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      coerce_tmp_chpl4 = ((Error_chpl)(call_tmp_chpl13));
#line 932 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      chpl_check_nil(coerce_tmp_chpl4, _ln_chpl, _fn_chpl);
#line 932 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      ((void(*)(Error_chpl,
#line 932 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
         string_chpl *,
#line 932 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
         int64_t,
#line 932 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
         int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(coerce_tmp_chpl4))->chpl__cid) + INT64(1))])(coerce_tmp_chpl4, &ret_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 932 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      call_tmp_chpl14 = ret_tmp_chpl2;
#line 932 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      safeCastCheckHalt_chpl(&call_tmp_chpl14, _ln_chpl, _fn_chpl);
#line 932 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      i_x_chpl = &call_tmp_chpl14;
#line 932 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      deinit_chpl95(i_x_chpl, _ln_chpl, _fn_chpl);
#line 932 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    }
#line 932 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    chpl__autoDestroy62(&call_tmp_chpl12, _ln_chpl, _fn_chpl);
#line 932 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    memcpy(((c_ptr_void_chpl)(call_tmp_chpl10)), ((c_ptr_void_chpl)(call_tmp_chpl11)), ((uint64_t)(otherLen_chpl)));
#line 932 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    call_tmp_chpl15 = (dst_chpl + otherLen_chpl);
#line 932 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    *(call_tmp_chpl15) = ((uint8_t)(INT64(0)));
#line 932 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    (this_chpl31)->buff = call_tmp_chpl9;
#line 932 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    coerce_tmp_chpl5 = (this_chpl31)->buff;
#line 932 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    coerce_tmp_chpl6 = (this_chpl31)->buffLen;
#line 932 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    call_tmp_chpl16 = (coerce_tmp_chpl5 + coerce_tmp_chpl6);
#line 932 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    *(call_tmp_chpl16) = ((uint8_t)(INT64(0)));
#line 932 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    (this_chpl31)->buffSize = tmp_chpl;
#line 932 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  }
#line 930 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  return;
#line 930 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
}

#line 935 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
/* String.chpl:935 */
#line 935 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
static void deinit_chpl95(string_chpl * this_chpl31,
#line 935 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
                          int64_t _ln_chpl,
#line 935 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
                          int32_t _fn_chpl) {
#line 939 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  chpl_bool tmp_chpl;
#line 939 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  if ((this_chpl31)->isOwned) /* ZLINE: 939 /Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl */
#line 939 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  {
#line 939 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    tmp_chpl = ((this_chpl31)->buff != nil);
#line 939 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  } else /* ZLINE: 939 /Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl */
#line 939 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  {
#line 939 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    tmp_chpl = UINT8(false);
#line 939 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  }
#line 939 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  if (tmp_chpl) /* ZLINE: 939 /Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl */
#line 939 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  {
#line 941 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    chpl_buildLocaleID((this_chpl31)->locale_id, c_sublocid_any, _ln_chpl, _fn_chpl);
#line 942 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    chpl_here_free(((c_ptr_void_chpl)((this_chpl31)->buff)), _ln_chpl, _fn_chpl);
#line 942 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  }
#line 935 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  return;
#line 935 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
}

#line 992 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
/* String.chpl:992 */
#line 992 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
static void _cpIndexLenHelpNoAdjustment_chpl(string_chpl * this_chpl31,
#line 992 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
                                             int64_t * start_chpl2,
#line 992 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
                                             _tuple_3_int32_t_byteIndex_int64_t_chpl * _retArg_chpl,
#line 992 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
                                             int64_t _ln_chpl,
#line 992 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
                                             int32_t _fn_chpl) {
#line 992 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _tuple_3_int32_t_byteIndex_int64_t_chpl ret_chpl;
#line 999 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  int64_t coerce_tmp_chpl3;
#line 1001 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  int64_t ret_chpl2;
#line 1001 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  int64_t ret_chpl3;
#line 1003 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  c_ptr_uint8_t_chpl coerce_tmp_chpl4 = NULL;
#line 1003 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _ref_uint8_t_chpl call_tmp_chpl6 = NULL;
#line 1003 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  int32_t call_tmp_chpl7;
#line 1003 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  byteIndex_chpl call_tmp_chpl8;
#line 1003 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  byteIndex_chpl ret_chpl4;
#line 1003 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  byteIndex_chpl cpi_chpl;
#line 1003 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _tuple_3_int32_t__ref_byteIndex_int64_t_chpl call_tmp_chpl9;
#line 1003 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _tuple_3_int32_t__ref_byteIndex_int64_t_chpl ret_tmp_chpl;
#line 1003 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _ref_byteIndex_chpl i__e1_x_chpl = NULL;
#line 1003 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _ref_byteIndex_chpl copy_ret_tmp_x1_chpl = NULL;
#line 1003 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  int64_t copy_ret_tmp_x2_chpl;
#line 1003 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _tuple_3_int32_t_byteIndex_int64_t_chpl ret_chpl5;
#line 1003 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  byteIndex_chpl elt_x1_chpl;
#line 1007 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  int32_t call_tmp_x1_chpl;
#line 1007 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  int32_t call_tmp_x2_chpl;
#line 1007 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _tuple_3_syserr_int32_t_int32_t_chpl ret_tmp_chpl2;
#line 1012 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  byteIndex_chpl call_tmp_chpl10;
#line 1012 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  byteIndex_chpl ret_chpl6;
#line 1012 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  byteIndex_chpl cpi_chpl2;
#line 1012 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  int64_t call_tmp_chpl11;
#line 1012 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _tuple_3_int32_t__ref_byteIndex_int64_t_chpl call_tmp_chpl12;
#line 1012 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _tuple_3_int32_t__ref_byteIndex_int64_t_chpl ret_tmp_chpl3;
#line 1012 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _ref_byteIndex_chpl i__e1_x_chpl2 = NULL;
#line 1012 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _ref_byteIndex_chpl copy_ret_tmp_x1_chpl2 = NULL;
#line 1012 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  int64_t copy_ret_tmp_x2_chpl2;
#line 1012 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _tuple_3_int32_t_byteIndex_int64_t_chpl ret_chpl7;
#line 1012 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  byteIndex_chpl elt_x1_chpl2;
#line 999 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  coerce_tmp_chpl3 = *(start_chpl2);
#line 1001 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  ret_chpl2 = (this_chpl31)->cachedNumCodepoints;
#line 1001 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  ret_chpl3 = (this_chpl31)->buffLen;
#line 1001 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  if (ret_chpl2 == ret_chpl3) /* ZLINE: 1001 /Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl */
#line 1001 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  {
#line 1002 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    *(start_chpl2) += INT64(1);
#line 1003 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    coerce_tmp_chpl4 = (this_chpl31)->buff;
#line 1003 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    call_tmp_chpl6 = (coerce_tmp_chpl4 + coerce_tmp_chpl3);
#line 1003 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    call_tmp_chpl7 = ((int32_t)(*(call_tmp_chpl6)));
#line 1003 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    init_chpl192(&cpi_chpl);
#line 1003 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    (&cpi_chpl)->_bindex = coerce_tmp_chpl3;
#line 1003 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    ret_chpl4 = cpi_chpl;
#line 1003 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    call_tmp_chpl8 = ret_chpl4;
#line 1003 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    i__e1_x_chpl = &call_tmp_chpl8;
#line 1003 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    (&ret_tmp_chpl)->x0 = call_tmp_chpl7;
#line 1003 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    (&ret_tmp_chpl)->x1 = i__e1_x_chpl;
#line 1003 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    (&ret_tmp_chpl)->x2 = INT64(1);
#line 1003 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    call_tmp_chpl9 = ret_tmp_chpl;
#line 1003 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    copy_ret_tmp_x1_chpl = (&call_tmp_chpl9)->x1;
#line 1003 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    copy_ret_tmp_x2_chpl = (&call_tmp_chpl9)->x2;
#line 1003 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    elt_x1_chpl = *(copy_ret_tmp_x1_chpl);
#line 1003 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    (&ret_chpl5)->x0 = (&call_tmp_chpl9)->x0;
#line 1003 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    (&ret_chpl5)->x1 = elt_x1_chpl;
#line 1003 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    (&ret_chpl5)->x2 = copy_ret_tmp_x2_chpl;
#line 1003 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    ret_chpl = ret_chpl5;
#line 1003 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    goto _end__cpIndexLenHelpNoAdjustment_chpl;
#line 1003 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  } else /* ZLINE: 1006 /Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl */
#line 1003 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  {
#line 1007 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    decodeHelp_chpl((this_chpl31)->buff, (this_chpl31)->buffLen, coerce_tmp_chpl3, UINT8(true), &ret_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 1007 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    call_tmp_x1_chpl = (&ret_tmp_chpl2)->x1;
#line 1007 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    call_tmp_x2_chpl = (&ret_tmp_chpl2)->x2;
#line 1011 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    *(start_chpl2) += ((int64_t)(call_tmp_x2_chpl));
#line 1012 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    init_chpl192(&cpi_chpl2);
#line 1012 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    (&cpi_chpl2)->_bindex = coerce_tmp_chpl3;
#line 1012 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    ret_chpl6 = cpi_chpl2;
#line 1012 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    call_tmp_chpl10 = ret_chpl6;
#line 1012 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    call_tmp_chpl11 = ((int64_t)(call_tmp_x2_chpl));
#line 1012 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    i__e1_x_chpl2 = &call_tmp_chpl10;
#line 1012 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    (&ret_tmp_chpl3)->x0 = call_tmp_x1_chpl;
#line 1012 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    (&ret_tmp_chpl3)->x1 = i__e1_x_chpl2;
#line 1012 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    (&ret_tmp_chpl3)->x2 = call_tmp_chpl11;
#line 1012 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    call_tmp_chpl12 = ret_tmp_chpl3;
#line 1012 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    copy_ret_tmp_x1_chpl2 = (&call_tmp_chpl12)->x1;
#line 1012 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    copy_ret_tmp_x2_chpl2 = (&call_tmp_chpl12)->x2;
#line 1012 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    elt_x1_chpl2 = *(copy_ret_tmp_x1_chpl2);
#line 1012 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    (&ret_chpl7)->x0 = (&call_tmp_chpl12)->x0;
#line 1012 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    (&ret_chpl7)->x1 = elt_x1_chpl2;
#line 1012 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    (&ret_chpl7)->x2 = copy_ret_tmp_x2_chpl2;
#line 1012 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    ret_chpl = ret_chpl7;
#line 1012 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    goto _end__cpIndexLenHelpNoAdjustment_chpl;
#line 1012 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  }
#line 1012 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _end__cpIndexLenHelpNoAdjustment_chpl:;
#line 992 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  *(_retArg_chpl) = ret_chpl;
#line 992 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  return;
#line 992 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
}

#line 1182 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
/* String.chpl:1182 */
#line 1182 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
static int64_t doSearchUTF8_chpl(string_chpl * this_chpl31,
#line 1182 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
                                 string_chpl * pattern_chpl,
#line 1182 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
                                 range_byteIndex_low_one_chpl indices_chpl5,
#line 1182 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
                                 int64_t _ln_chpl,
#line 1182 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
                                 int32_t _fn_chpl) {
#line 1190 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  int64_t localRet_chpl;
#line 1191 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  int64_t nLen_chpl;
#line 1191 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  int64_t coerce_tmp_chpl3;
#line 1192 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _tuple_2_range_int64_t_both_one_int64_t_chpl call_tmp_chpl6;
#line 1192 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  Error_chpl error_chpl = NULL;
#line 1192 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _tuple_2_range_int64_t_both_one_int64_t_chpl ret_tmp_chpl;
#line 1192 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  range_int64_t_both_one_chpl tmp_x0_chpl;
#line 1192 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  range_int64_t_both_one_chpl view_chpl;
#line 1193 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  int64_t call_tmp_chpl7;
#line 1237 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  string_chpl ret_chpl;
#line 1237 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  string_chpl ret_chpl2;
#line 1237 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  string_chpl localPattern_chpl;
#line 1241 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  int64_t call_tmp_chpl8;
#line 1242 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  range_int64_t_both_one_chpl tmp_chpl;
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  range_int64_t_low_one_chpl call_tmp_chpl9;
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  range_int64_t_low_one_chpl ret_tmp_chpl2;
#line 1242 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl3;
#line 1246 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  range_int64_t_both_one_chpl _ic__F0_this_chpl;
#line 1246 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  int64_t i_chpl;
#line 1246 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  range_int64_t_both_one_chpl this_chpl32;
#line 1246 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  range_int64_t_both_one_chpl this_chpl33;
#line 1246 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  int64_t end_chpl;
#line 1248 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  chpl_bool call_tmp_chpl10;
#line 1248 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  chpl_bool call_tmp_chpl11;
#line 1246 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  int64_t i_chpl2;
#line 1248 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  int64_t tmp_chpl2;
#line 1248 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  int64_t _ic__F1_high_chpl;
#line 1248 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  int64_t i_chpl3;
#line 1249 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  int64_t call_tmp_chpl12;
#line 1250 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  c_ptr_uint8_t_chpl coerce_tmp_chpl4 = NULL;
#line 1250 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _ref_uint8_t_chpl call_tmp_chpl13 = NULL;
#line 1250 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  c_ptr_uint8_t_chpl coerce_tmp_chpl5 = NULL;
#line 1250 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _ref_uint8_t_chpl call_tmp_chpl14 = NULL;
#line 1256 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  int64_t call_tmp_chpl15;
#line 1246 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 1187 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  chpl_buildLocaleID((this_chpl31)->locale_id, c_sublocid_any, _ln_chpl, _fn_chpl);
#line 1190 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  localRet_chpl = INT64(-2);
#line 1191 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  coerce_tmp_chpl3 = (pattern_chpl)->buffLen;
#line 1191 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  nLen_chpl = coerce_tmp_chpl3;
#line 1192 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  error_chpl = NULL;
#line 1192 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  getView_chpl2(this_chpl31, indices_chpl5, &error_chpl, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 1192 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  call_tmp_chpl6 = ret_tmp_chpl;
#line 1192 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  if (error_chpl != nil) /* ZLINE: 1192 /Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl */
#line 1192 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  {
#line 1192 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    chpl_uncaught_error(error_chpl, _ln_chpl, _fn_chpl);
#line 1192 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  }
#line 1192 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  tmp_x0_chpl = (&call_tmp_chpl6)->x0;
#line 1192 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  view_chpl = tmp_x0_chpl;
#line 1193 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  call_tmp_chpl7 = size_chpl8(tmp_x0_chpl, _ln_chpl, _fn_chpl);
#line 1220 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  if (coerce_tmp_chpl3 == INT64(0)) /* ZLINE: 1220 /Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl */
#line 1220 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  {
#line 1222 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    localRet_chpl = INT64(-1);
#line 1222 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  }
#line 1231 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  if (coerce_tmp_chpl3 > call_tmp_chpl7) /* ZLINE: 1231 /Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl */
#line 1231 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  {
#line 1232 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    localRet_chpl = INT64(-1);
#line 1232 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  }
#line 1235 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  if (localRet_chpl == INT64(-2)) /* ZLINE: 1235 /Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl */
#line 1235 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  {
#line 1236 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    localRet_chpl = INT64(-1);
#line 1237 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    init_chpl194(&ret_chpl2, _ln_chpl, _fn_chpl);
#line 1237 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    (&ret_chpl2)->cachedNumCodepoints = (pattern_chpl)->cachedNumCodepoints;
#line 1237 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    initWithBorrowedBuffer_chpl(&ret_chpl2, pattern_chpl, _ln_chpl, _fn_chpl);
#line 1237 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    ret_chpl = ret_chpl2;
#line 1237 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    localPattern_chpl = ret_chpl;
#line 1241 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    call_tmp_chpl8 = (((int64_t)((call_tmp_chpl7 - coerce_tmp_chpl3))) + INT64(1));
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    chpl_build_low_bounded_range2(INT64(0), &ret_tmp_chpl2);
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    call_tmp_chpl9 = ret_tmp_chpl2;
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    chpl___POUND_3(call_tmp_chpl9, call_tmp_chpl8, &ret_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 1242 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    tmp_chpl = ret_tmp_chpl3;
#line 1246 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    _ic__F0_this_chpl = tmp_chpl;
#line 1246 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    checkIfIterWillOverflow_chpl(tmp_chpl, UINT8(true), _ln_chpl, _fn_chpl);
#line 1246 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    i_chpl = INT64(0);
#line 1246 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    this_chpl32 = _ic__F0_this_chpl;
#line 1246 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    this_chpl33 = _ic__F0_this_chpl;
#line 1246 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    end_chpl = (&this_chpl33)->_high;
#line 1248 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    call_tmp_chpl10 = (nLen_chpl < INT64(0));
#line 1248 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    call_tmp_chpl11 = (nLen_chpl == INT64(0));
#line 1246 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    for (i_chpl = (&this_chpl32)->_low; ((i_chpl <= end_chpl)); i_chpl += INT64(1)) {
#line 1246 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      i_chpl2 = i_chpl;
#line 1248 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      if (call_tmp_chpl10) /* ZLINE: 1248 /Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl */
#line 1248 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      {
#line 1248 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        boundsCheckHalt_chpl(&_str_literal_580_chpl, _ln_chpl, _fn_chpl);
#line 1248 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      }
#line 1248 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      if (call_tmp_chpl11) /* ZLINE: 1248 /Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl */
#line 1248 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      {
#line 1248 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        tmp_chpl2 = ((int64_t)(((uint64_t)((((uint64_t)(INT64(0))) - UINT64(1))))));
#line 1248 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      } else /* ZLINE: 1248 /Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl */
#line 1248 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      {
#line 1248 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        tmp_chpl2 = ((int64_t)((INT64(0) + ((int64_t)((nLen_chpl - INT64(1)))))));
#line 1248 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      }
#line 1248 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      _ic__F1_high_chpl = tmp_chpl2;
#line 1248 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      i_chpl3 = INT64(0);
#line 1248 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      chpl_checkIfRangeIterWillOverflow(INT64(0), tmp_chpl2, INT64(1), INT64(0), tmp_chpl2, UINT8(true), _ln_chpl, _fn_chpl);
#line 1248 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      for (i_chpl3 = INT64(0); ((i_chpl3 <= _ic__F1_high_chpl)); i_chpl3 += INT64(1)) {
#line 1249 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        call_tmp_chpl12 = orderToIndex_chpl2(view_chpl, ((int64_t)((i_chpl2 + i_chpl3))), _ln_chpl, _fn_chpl);
#line 1250 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        coerce_tmp_chpl4 = (this_chpl31)->buff;
#line 1250 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        call_tmp_chpl13 = (coerce_tmp_chpl4 + call_tmp_chpl12);
#line 1250 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        coerce_tmp_chpl5 = (&localPattern_chpl)->buff;
#line 1250 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        call_tmp_chpl14 = (coerce_tmp_chpl5 + i_chpl3);
#line 1250 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        if (*(call_tmp_chpl13) != *(call_tmp_chpl14)) /* ZLINE: 1250 /Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl */
#line 1250 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        {
#line 1250 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
          goto _breakLabel_chpl;
#line 1250 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        }
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        if (i_chpl3 == ((int64_t)((nLen_chpl - INT64(1))))) /* ZLINE: 1252 /Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl */
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        {
#line 1256 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
          call_tmp_chpl15 = orderToIndex_chpl2(view_chpl, i_chpl2, _ln_chpl, _fn_chpl);
#line 1256 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
          localRet_chpl = call_tmp_chpl15;
#line 1256 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        }
#line 1256 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      }
#line 1256 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      _breakLabel_chpl:;
#line 1260 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      if (localRet_chpl != INT64(-1)) /* ZLINE: 1260 /Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl */
#line 1260 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      {
#line 1260 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        goto _breakLabel_chpl2;
#line 1260 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      }
#line 1260 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    }
#line 1260 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    _breakLabel_chpl2:;
#line 1246 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    i_x_chpl = &localPattern_chpl;
#line 1246 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    deinit_chpl95(i_x_chpl, _ln_chpl, _fn_chpl);
#line 1246 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  }
#line 1182 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  return localRet_chpl;
#line 1182 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
}

#line 1393 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
/* String.chpl:1393 */
#line 1393 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
static void encode_chpl(string_chpl * this_chpl31,
#line 1393 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
                        encodePolicy_chpl policy_chpl,
#line 1393 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
                        bytes_chpl * _retArg_chpl,
#line 1393 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
                        int64_t _ln_chpl,
#line 1393 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
                        int32_t _fn_chpl) {
#line 1393 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  bytes_chpl ret_chpl;
#line 1394 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  string_chpl ret_chpl2;
#line 1394 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  string_chpl ret_chpl3;
#line 1394 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  string_chpl localThis_chpl;
#line 1396 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  chpl_bool tmp_chpl;
#line 1396 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  int64_t ret_chpl4;
#line 1396 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  int64_t ret_chpl5;
#line 1397 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  int64_t ret_chpl6;
#line 1397 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  c_ptr_uint8_t_chpl coerce_tmp_chpl3 = NULL;
#line 1397 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  int64_t call_tmp_chpl6;
#line 1397 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  bytes_chpl ret_chpl7;
#line 1397 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  bytes_chpl ret_chpl8;
#line 1397 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 1400 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  int64_t call_tmp_chpl7;
#line 1400 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  int64_t tmp_chpl2;
#line 1400 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  int16_t call_tmp_chpl8;
#line 1400 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  c_ptr_void_chpl cast_tmp_chpl = NULL;
#line 1400 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  c_ptr_uint8_t_chpl buff_chpl = NULL;
#line 1400 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  int64_t size_chpl9;
#line 1402 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  int64_t readIdx_chpl;
#line 1403 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  int64_t writeIdx_chpl;
#line 1404 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  chpl_bool tmp_chpl3;
#line 1405 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  c_ptr_uint8_t_chpl call_tmp_chpl9 = NULL;
#line 1406 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  syserr call_tmp_x0_chpl;
#line 1406 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  int32_t call_tmp_x1_chpl;
#line 1406 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  int32_t call_tmp_x2_chpl;
#line 1406 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _tuple_3_syserr_int32_t_int32_t_chpl ret_tmp_chpl;
#line 1410 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  chpl_bool tmp_chpl4;
#line 1411 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _ref_uint8_t_chpl call_tmp_chpl10 = NULL;
#line 1412 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _ref_int64_t_chpl i_lhs_chpl = NULL;
#line 1414 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  int32_t call_tmp_chpl11;
#line 1419 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  int64_t coerce_tmp_chpl4;
#line 1419 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  c_ptr_uint8_t_chpl call_tmp_chpl12 = NULL;
#line 1419 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  c_ptr_uint8_t_chpl call_tmp_chpl13 = NULL;
#line 1419 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _owned_IllegalArgumentError_chpl2 call_tmp_chpl14;
#line 1419 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _owned_IllegalArgumentError_chpl2 ret_tmp_chpl2;
#line 1419 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _ref__owned_IllegalArgumentError_chpl i_arg_chpl = NULL;
#line 1419 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  IllegalArgumentError_chpl call_tmp_chpl15 = NULL;
#line 1419 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  string_chpl call_tmp_chpl16;
#line 1419 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  Error_chpl coerce_tmp_chpl5 = NULL;
#line 1419 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  string_chpl ret_tmp_chpl3;
#line 1419 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _ref_string_chpl i_x_chpl2 = NULL;
#line 1420 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _ref_int64_t_chpl i_lhs_chpl2 = NULL;
#line 1423 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  int64_t coerce_tmp_chpl6;
#line 1423 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  c_ptr_uint8_t_chpl call_tmp_chpl17 = NULL;
#line 1423 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  c_ptr_uint8_t_chpl call_tmp_chpl18 = NULL;
#line 1423 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _owned_IllegalArgumentError_chpl2 call_tmp_chpl19;
#line 1423 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _owned_IllegalArgumentError_chpl2 ret_tmp_chpl4;
#line 1423 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _ref__owned_IllegalArgumentError_chpl i_arg_chpl2 = NULL;
#line 1423 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  IllegalArgumentError_chpl call_tmp_chpl20 = NULL;
#line 1423 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  string_chpl call_tmp_chpl21;
#line 1423 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  Error_chpl coerce_tmp_chpl7 = NULL;
#line 1423 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  string_chpl ret_tmp_chpl5;
#line 1423 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _ref_string_chpl i_x_chpl3 = NULL;
#line 1424 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _ref_int64_t_chpl i_lhs_chpl3 = NULL;
#line 1426 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _ref_int64_t_chpl i_lhs_chpl4 = NULL;
#line 1428 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _ref_uint8_t_chpl call_tmp_chpl22 = NULL;
#line 1429 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  bytes_chpl ret_chpl9;
#line 1429 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  bytes_chpl ret_chpl10;
#line 1429 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _ref_bytes_chpl i_x_chpl4 = NULL;
#line 1429 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _ref_string_chpl i_x_chpl5 = NULL;
#line 1394 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  init_chpl194(&ret_chpl3, _ln_chpl, _fn_chpl);
#line 1394 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  (&ret_chpl3)->cachedNumCodepoints = (this_chpl31)->cachedNumCodepoints;
#line 1394 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  initWithBorrowedBuffer_chpl(&ret_chpl3, this_chpl31, _ln_chpl, _fn_chpl);
#line 1394 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  ret_chpl2 = ret_chpl3;
#line 1394 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  localThis_chpl = ret_chpl2;
#line 1396 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  if (policy_chpl == encodePolicy_chpl_pass_chpl) /* ZLINE: 1396 /Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl */
#line 1396 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  {
#line 1396 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    tmp_chpl = UINT8(true);
#line 1396 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  } else /* ZLINE: 1396 /Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl */
#line 1396 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  {
#line 1396 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    ret_chpl4 = (this_chpl31)->cachedNumCodepoints;
#line 1396 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    ret_chpl5 = (this_chpl31)->buffLen;
#line 1396 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    tmp_chpl = (ret_chpl4 == ret_chpl5);
#line 1396 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  }
#line 1396 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  if (tmp_chpl) /* ZLINE: 1396 /Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl */
#line 1396 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  {
#line 1397 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    ret_chpl6 = (&localThis_chpl)->buffLen;
#line 1397 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    coerce_tmp_chpl3 = (&localThis_chpl)->buff;
#line 1397 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    call_tmp_chpl6 = (ret_chpl6 + INT64(1));
#line 1397 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    init_chpl19(&ret_chpl8, _ln_chpl, _fn_chpl);
#line 1397 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    initWithNewBuffer_chpl(&ret_chpl8, coerce_tmp_chpl3, ret_chpl6, call_tmp_chpl6, _ln_chpl, _fn_chpl);
#line 1397 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    ret_chpl7 = ret_chpl8;
#line 1397 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    ret_chpl = ret_chpl7;
#line 1397 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    i_x_chpl = &localThis_chpl;
#line 1397 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    deinit_chpl95(i_x_chpl, _ln_chpl, _fn_chpl);
#line 1397 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    goto _end_encode_chpl;
#line 1397 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  } else /* ZLINE: 1399 /Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl */
#line 1397 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  {
#line 1400 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    call_tmp_chpl7 = chpl_here_good_alloc_size(((int64_t)(((&localThis_chpl)->buffLen + INT64(1)))), _ln_chpl, _fn_chpl);
#line 1400 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    if (call_tmp_chpl7 > INT64(0)) /* ZLINE: 1400 /Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl */
#line 1400 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    {
#line 1400 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      tmp_chpl2 = call_tmp_chpl7;
#line 1400 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    } else /* ZLINE: 1400 /Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl */
#line 1400 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    {
#line 1400 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      tmp_chpl2 = INT64(0);
#line 1400 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    }
#line 1400 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    call_tmp_chpl8 = offset_STR_COPY_DATA_chpl();
#line 1400 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    cast_tmp_chpl = chpl_here_alloc(tmp_chpl2, call_tmp_chpl8, _ln_chpl, _fn_chpl);
#line 1400 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    buff_chpl = ((c_ptr_uint8_t_chpl)(cast_tmp_chpl));
#line 1400 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    size_chpl9 = tmp_chpl2;
#line 1402 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    readIdx_chpl = INT64(0);
#line 1403 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    writeIdx_chpl = INT64(0);
#line 1404 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    tmp_chpl3 = (INT64(0) < (&localThis_chpl)->buffLen);
#line 1404 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    while (tmp_chpl3) {
#line 1405 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      call_tmp_chpl9 = ((&localThis_chpl)->buff + readIdx_chpl);
#line 1406 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      decodeHelp_chpl((&localThis_chpl)->buff, (&localThis_chpl)->buffLen, readIdx_chpl, UINT8(true), &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 1406 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      call_tmp_x0_chpl = (&ret_tmp_chpl)->x0;
#line 1406 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      call_tmp_x1_chpl = (&ret_tmp_chpl)->x1;
#line 1406 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      call_tmp_x2_chpl = (&ret_tmp_chpl)->x2;
#line 1410 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      if (INT32(56448) <= call_tmp_x1_chpl) /* ZLINE: 1410 /Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl */
#line 1410 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      {
#line 1410 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        tmp_chpl4 = (call_tmp_x1_chpl <= ((int32_t)(INT64(56575))));
#line 1410 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      } else /* ZLINE: 1410 /Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl */
#line 1410 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      {
#line 1410 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        tmp_chpl4 = UINT8(false);
#line 1410 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      }
#line 1410 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      if (tmp_chpl4) /* ZLINE: 1410 /Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl */
#line 1410 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      {
#line 1411 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        call_tmp_chpl10 = (buff_chpl + writeIdx_chpl);
#line 1411 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        *(call_tmp_chpl10) = ((uint8_t)(((int32_t)((call_tmp_x1_chpl - ((int32_t)(INT64(56320))))))));
#line 1412 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        i_lhs_chpl = &writeIdx_chpl;
#line 1412 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        *(i_lhs_chpl) += INT64(1);
#line 1412 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      } else /* ZLINE: 1410 /Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl */
#line 1412 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      {
#line 1414 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        call_tmp_chpl11 = qio_err_to_int(call_tmp_x0_chpl);
#line 1414 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        if (! (call_tmp_chpl11 == ((int32_t)(INT64(0))))) /* ZLINE: 1414 /Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl */
#line 1414 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        {
#line 1419 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
          coerce_tmp_chpl4 = ((int64_t)(call_tmp_x2_chpl));
#line 1419 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
          call_tmp_chpl12 = ((buff_chpl + writeIdx_chpl) + INT64(0));
#line 1419 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
          call_tmp_chpl13 = (call_tmp_chpl9 + INT64(0));
#line 1419 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
          chpl_checkValue7(coerce_tmp_chpl4, &ret_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 1419 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
          call_tmp_chpl14 = ret_tmp_chpl2;
#line 1419 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
          i_arg_chpl = &call_tmp_chpl14;
#line 1419 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
          call_tmp_chpl15 = borrow_chpl2(i_arg_chpl);
#line 1419 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
          if (((RootClass_chpl)(call_tmp_chpl15)) != nil) /* ZLINE: 1419 /Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl */
#line 1419 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
          {
#line 1419 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
            coerce_tmp_chpl5 = ((Error_chpl)(call_tmp_chpl15));
#line 1419 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
            chpl_check_nil(coerce_tmp_chpl5, _ln_chpl, _fn_chpl);
#line 1419 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
            ((void(*)(Error_chpl,
#line 1419 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
               string_chpl *,
#line 1419 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
               int64_t,
#line 1419 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
               int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(coerce_tmp_chpl5))->chpl__cid) + INT64(1))])(coerce_tmp_chpl5, &ret_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 1419 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
            call_tmp_chpl16 = ret_tmp_chpl3;
#line 1419 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
            safeCastCheckHalt_chpl(&call_tmp_chpl16, _ln_chpl, _fn_chpl);
#line 1419 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
            i_x_chpl2 = &call_tmp_chpl16;
#line 1419 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
            deinit_chpl95(i_x_chpl2, _ln_chpl, _fn_chpl);
#line 1419 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
          }
#line 1419 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
          chpl__autoDestroy62(&call_tmp_chpl14, _ln_chpl, _fn_chpl);
#line 1419 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
          memcpy(((c_ptr_void_chpl)(call_tmp_chpl12)), ((c_ptr_void_chpl)(call_tmp_chpl13)), ((uint64_t)(coerce_tmp_chpl4)));
#line 1420 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
          i_lhs_chpl2 = &writeIdx_chpl;
#line 1420 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
          *(i_lhs_chpl2) += ((int64_t)(call_tmp_x2_chpl));
#line 1420 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        } else /* ZLINE: 1422 /Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl */
#line 1420 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        {
#line 1423 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
          coerce_tmp_chpl6 = ((int64_t)(call_tmp_x2_chpl));
#line 1423 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
          call_tmp_chpl17 = ((buff_chpl + writeIdx_chpl) + INT64(0));
#line 1423 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
          call_tmp_chpl18 = (call_tmp_chpl9 + INT64(0));
#line 1423 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
          chpl_checkValue7(coerce_tmp_chpl6, &ret_tmp_chpl4, _ln_chpl, _fn_chpl);
#line 1423 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
          call_tmp_chpl19 = ret_tmp_chpl4;
#line 1423 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
          i_arg_chpl2 = &call_tmp_chpl19;
#line 1423 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
          call_tmp_chpl20 = borrow_chpl2(i_arg_chpl2);
#line 1423 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
          if (((RootClass_chpl)(call_tmp_chpl20)) != nil) /* ZLINE: 1423 /Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl */
#line 1423 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
          {
#line 1423 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
            coerce_tmp_chpl7 = ((Error_chpl)(call_tmp_chpl20));
#line 1423 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
            chpl_check_nil(coerce_tmp_chpl7, _ln_chpl, _fn_chpl);
#line 1423 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
            ((void(*)(Error_chpl,
#line 1423 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
               string_chpl *,
#line 1423 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
               int64_t,
#line 1423 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
               int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(coerce_tmp_chpl7))->chpl__cid) + INT64(1))])(coerce_tmp_chpl7, &ret_tmp_chpl5, _ln_chpl, _fn_chpl);
#line 1423 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
            call_tmp_chpl21 = ret_tmp_chpl5;
#line 1423 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
            safeCastCheckHalt_chpl(&call_tmp_chpl21, _ln_chpl, _fn_chpl);
#line 1423 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
            i_x_chpl3 = &call_tmp_chpl21;
#line 1423 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
            deinit_chpl95(i_x_chpl3, _ln_chpl, _fn_chpl);
#line 1423 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
          }
#line 1423 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
          chpl__autoDestroy62(&call_tmp_chpl19, _ln_chpl, _fn_chpl);
#line 1423 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
          memcpy(((c_ptr_void_chpl)(call_tmp_chpl17)), ((c_ptr_void_chpl)(call_tmp_chpl18)), ((uint64_t)(coerce_tmp_chpl6)));
#line 1424 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
          i_lhs_chpl3 = &writeIdx_chpl;
#line 1424 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
          *(i_lhs_chpl3) += ((int64_t)(call_tmp_x2_chpl));
#line 1424 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        }
#line 1424 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      }
#line 1426 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      i_lhs_chpl4 = &readIdx_chpl;
#line 1426 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      *(i_lhs_chpl4) += ((int64_t)(call_tmp_x2_chpl));
#line 1404 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      tmp_chpl3 = (readIdx_chpl < (&localThis_chpl)->buffLen);
#line 1404 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    }
#line 1428 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    call_tmp_chpl22 = (buff_chpl + writeIdx_chpl);
#line 1428 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    *(call_tmp_chpl22) = ((uint8_t)(INT64(0)));
#line 1429 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    init_chpl19(&ret_chpl10, _ln_chpl, _fn_chpl);
#line 1429 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    i_x_chpl4 = &ret_chpl10;
#line 1429 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    (i_x_chpl4)->isOwned = UINT8(true);
#line 1429 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    (i_x_chpl4)->buff = buff_chpl;
#line 1429 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    (i_x_chpl4)->buffSize = size_chpl9;
#line 1429 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    (i_x_chpl4)->buffLen = writeIdx_chpl;
#line 1429 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    ret_chpl9 = ret_chpl10;
#line 1429 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    ret_chpl = ret_chpl9;
#line 1429 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    i_x_chpl5 = &localThis_chpl;
#line 1429 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    deinit_chpl95(i_x_chpl5, _ln_chpl, _fn_chpl);
#line 1429 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    goto _end_encode_chpl;
#line 1429 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  }
#line 1429 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _end_encode_chpl:;
#line 1393 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  *(_retArg_chpl) = ret_chpl;
#line 1393 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  return;
#line 1393 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
}

#line 1506 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
/* String.chpl:1506 */
#line 1506 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
static void advance_chpl19(_ic_chpl_bytes__ref_string_chpl this_chpl31,
#line 1506 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
                           int64_t _ln_chpl,
#line 1506 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
                           int32_t _fn_chpl) {
#line 1506 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  int64_t more_chpl;
#line 1506 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _ref_string_chpl tmp_chpl = NULL;
#line 1507 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  string_chpl ret_chpl;
#line 1507 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  string_chpl ret_chpl2;
#line 1506 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _ref_string_chpl tmp_chpl2 = NULL;
#line 1509 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  int64_t coerce_tmp_chpl3;
#line 1509 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _ic_chpl_direct_counted_range_iter_chpl _ic__chpl = NULL;
#line 1509 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  c_ptr_void_chpl cast_tmp_chpl = NULL;
#line 1509 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  int64_t low_chpl2;
#line 1506 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _ref__ic_chpl_direct_counted_range_iter_chpl tmp_chpl3 = NULL;
#line 1509 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  int64_t count_chpl;
#line 1506 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _ref__ic_chpl_direct_counted_range_iter_chpl tmp_chpl4 = NULL;
#line 1509 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _ic_chpl_direct_counted_range_iter_helper_chpl _ic__chpl2 = NULL;
#line 1509 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  c_ptr_void_chpl cast_tmp_chpl2 = NULL;
#line 1506 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _ref__ic_chpl_direct_counted_range_iter_helper_chpl tmp_chpl5 = NULL;
#line 1509 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  int64_t low_chpl3;
#line 1506 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _ref__ic_chpl_direct_counted_range_iter_helper_chpl tmp_chpl6 = NULL;
#line 1509 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  int64_t tmp_chpl7;
#line 1506 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _ref__ic_chpl_direct_counted_range_iter_helper_chpl tmp_chpl8 = NULL;
#line 1506 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _ref__ic_chpl_direct_counted_range_iter_helper_chpl tmp_chpl9 = NULL;
#line 1506 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _ref__ic_chpl_direct_counted_range_iter_helper_chpl tmp_chpl10 = NULL;
#line 1509 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  int64_t call_tmp_chpl6;
#line 1506 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _ref__ic_chpl_direct_counted_range_iter_helper_chpl tmp_chpl11 = NULL;
#line 1509 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _ic_chpl_direct_param_stride_range_iter_chpl _ic__chpl3 = NULL;
#line 1509 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  c_ptr_void_chpl cast_tmp_chpl3 = NULL;
#line 1509 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  int64_t low_chpl4;
#line 1506 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _ref__ic_chpl_direct_param_stride_range_iter_chpl tmp_chpl12 = NULL;
#line 1509 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  int64_t high_chpl;
#line 1506 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _ref__ic_chpl_direct_param_stride_range_iter_chpl tmp_chpl13 = NULL;
#line 1509 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  int64_t low_chpl5;
#line 1506 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _ref__ic_chpl_direct_param_stride_range_iter_chpl tmp_chpl14 = NULL;
#line 1506 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _ref__ic_chpl_direct_param_stride_range_iter_chpl tmp_chpl15 = NULL;
#line 1506 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  int64_t tmp_chpl16;
#line 1506 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _ref_string_chpl tmp_chpl17 = NULL;
#line 1510 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  c_ptr_uint8_t_chpl coerce_tmp_chpl4 = NULL;
#line 1510 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _ref_uint8_t_chpl call_tmp_chpl7 = NULL;
#line 1506 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  uint8_t tmp_chpl18;
#line 1506 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _ref_string_chpl tmp_chpl19 = NULL;
#line 1506 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  int64_t tmp_chpl20;
#line 1506 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _ref__ic_chpl_direct_param_stride_range_iter_chpl tmp_chpl21 = NULL;
#line 1506 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  int64_t tmp_chpl22;
#line 1509 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  int64_t high_chpl2;
#line 1506 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _ref__ic_chpl_direct_param_stride_range_iter_chpl tmp_chpl23 = NULL;
#line 1506 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  int64_t tmp_chpl24;
#line 1506 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _ref_string_chpl tmp_chpl25 = NULL;
#line 1506 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 1506 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  more_chpl = (this_chpl31)->more;
#line 1506 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  if (more_chpl == INT64(2)) /* ZLINE: 1506 /Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl */
#line 1506 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  {
#line 1506 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    goto _jump_2_chpl;
#line 1506 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  }
#line 1506 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  if (more_chpl == INT64(-2)) /* ZLINE: 1506 /Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl */
#line 1506 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  {
#line 1506 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    goto _jump_break_2_chpl;
#line 1506 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  }
#line 1506 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 1506 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  tmp_chpl = (this_chpl31)->F0_this;
#line 1507 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  init_chpl194(&ret_chpl2, _ln_chpl, _fn_chpl);
#line 1507 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  (&ret_chpl2)->cachedNumCodepoints = (tmp_chpl)->cachedNumCodepoints;
#line 1507 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  initWithBorrowedBuffer_chpl(&ret_chpl2, tmp_chpl, _ln_chpl, _fn_chpl);
#line 1507 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  ret_chpl = ret_chpl2;
#line 1506 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 1506 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  (this_chpl31)->F1_localThis = ret_chpl;
#line 1506 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 1506 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  tmp_chpl2 = &((this_chpl31)->F1_localThis);
#line 1509 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  coerce_tmp_chpl3 = (tmp_chpl2)->buffLen;
#line 1509 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  cast_tmp_chpl = chpl_here_alloc(sizeof(chpl__ic_chpl_direct_counted_range_iter_chpl_object), INT16(32), _ln_chpl, _fn_chpl);
#line 1509 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _ic__chpl = ((_ic_chpl_direct_counted_range_iter_chpl)(cast_tmp_chpl));
#line 1509 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  ((RootClass_chpl)(_ic__chpl))->chpl__cid = chpl__cid__ic_chpl_direct_counted_range_iter_chpl;
#line 1509 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  chpl_check_nil(_ic__chpl, _ln_chpl, _fn_chpl);
#line 1509 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  (_ic__chpl)->more = INT64(1);
#line 1509 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  chpl_check_nil(_ic__chpl, _ln_chpl, _fn_chpl);
#line 1509 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  (_ic__chpl)->F0_low = INT64(0);
#line 1509 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  chpl_check_nil(_ic__chpl, _ln_chpl, _fn_chpl);
#line 1509 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  (_ic__chpl)->F1_count = coerce_tmp_chpl3;
#line 1506 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 1506 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  (this_chpl31)->F2__iterator = _ic__chpl;
#line 1506 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 1506 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  tmp_chpl3 = &((this_chpl31)->F2__iterator);
#line 1509 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  chpl_check_nil(tmp_chpl3, _ln_chpl, _fn_chpl);
#line 1509 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  low_chpl2 = (*(tmp_chpl3))->F0_low;
#line 1506 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 1506 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  tmp_chpl4 = &((this_chpl31)->F2__iterator);
#line 1509 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  chpl_check_nil(tmp_chpl4, _ln_chpl, _fn_chpl);
#line 1509 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  count_chpl = (*(tmp_chpl4))->F1_count;
#line 1509 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  cast_tmp_chpl2 = chpl_here_alloc(sizeof(chpl__ic_chpl_direct_counted_range_iter_helper_chpl_object), INT16(31), _ln_chpl, _fn_chpl);
#line 1509 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _ic__chpl2 = ((_ic_chpl_direct_counted_range_iter_helper_chpl)(cast_tmp_chpl2));
#line 1509 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  ((RootClass_chpl)(_ic__chpl2))->chpl__cid = chpl__cid__ic_chpl_direct_counted_range_iter_helper_chpl;
#line 1509 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  chpl_check_nil(_ic__chpl2, _ln_chpl, _fn_chpl);
#line 1509 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  (_ic__chpl2)->more = INT64(1);
#line 1509 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  chpl_check_nil(_ic__chpl2, _ln_chpl, _fn_chpl);
#line 1509 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  (_ic__chpl2)->F0_low = low_chpl2;
#line 1509 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  chpl_check_nil(_ic__chpl2, _ln_chpl, _fn_chpl);
#line 1509 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  (_ic__chpl2)->F1_count = count_chpl;
#line 1506 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 1506 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  (this_chpl31)->F3__iterator = _ic__chpl2;
#line 1506 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 1506 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  tmp_chpl5 = &((this_chpl31)->F3__iterator);
#line 1509 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  chpl_check_nil(tmp_chpl5, _ln_chpl, _fn_chpl);
#line 1509 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  if ((*(tmp_chpl5))->F1_count < INT64(0)) /* ZLINE: 1509 /Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl */
#line 1509 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  {
#line 1509 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    boundsCheckHalt_chpl(&_str_literal_580_chpl, _ln_chpl, _fn_chpl);
#line 1509 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  }
#line 1506 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 1506 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  tmp_chpl6 = &((this_chpl31)->F3__iterator);
#line 1509 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  chpl_check_nil(tmp_chpl6, _ln_chpl, _fn_chpl);
#line 1509 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  low_chpl3 = (*(tmp_chpl6))->F0_low;
#line 1506 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 1506 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  tmp_chpl8 = &((this_chpl31)->F3__iterator);
#line 1509 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  chpl_check_nil(tmp_chpl8, _ln_chpl, _fn_chpl);
#line 1509 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  if ((*(tmp_chpl8))->F1_count == INT64(0)) /* ZLINE: 1509 /Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl */
#line 1509 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  {
#line 1506 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 1506 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    tmp_chpl9 = &((this_chpl31)->F3__iterator);
#line 1509 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    chpl_check_nil(tmp_chpl9, _ln_chpl, _fn_chpl);
#line 1509 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    tmp_chpl7 = ((int64_t)(((uint64_t)((((uint64_t)((*(tmp_chpl9))->F0_low)) - UINT64(1))))));
#line 1509 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  } else /* ZLINE: 1509 /Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl */
#line 1509 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  {
#line 1506 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 1506 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    tmp_chpl10 = &((this_chpl31)->F3__iterator);
#line 1509 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    chpl_check_nil(tmp_chpl10, _ln_chpl, _fn_chpl);
#line 1509 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    call_tmp_chpl6 = ((*(tmp_chpl10))->F1_count - INT64(1));
#line 1506 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 1506 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    tmp_chpl11 = &((this_chpl31)->F3__iterator);
#line 1509 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    chpl_check_nil(tmp_chpl11, _ln_chpl, _fn_chpl);
#line 1509 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    tmp_chpl7 = ((int64_t)(((*(tmp_chpl11))->F0_low + call_tmp_chpl6)));
#line 1509 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  }
#line 1509 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  cast_tmp_chpl3 = chpl_here_alloc(sizeof(chpl__ic_chpl_direct_param_stride_range_iter_chpl_object), INT16(4), _ln_chpl, _fn_chpl);
#line 1509 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _ic__chpl3 = ((_ic_chpl_direct_param_stride_range_iter_chpl)(cast_tmp_chpl3));
#line 1509 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  ((RootClass_chpl)(_ic__chpl3))->chpl__cid = chpl__cid__ic_chpl_direct_param_stride_range_iter_chpl;
#line 1509 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  chpl_check_nil(_ic__chpl3, _ln_chpl, _fn_chpl);
#line 1509 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  (_ic__chpl3)->more = INT64(1);
#line 1509 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  chpl_check_nil(_ic__chpl3, _ln_chpl, _fn_chpl);
#line 1509 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  (_ic__chpl3)->F0_low = low_chpl3;
#line 1509 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  chpl_check_nil(_ic__chpl3, _ln_chpl, _fn_chpl);
#line 1509 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  (_ic__chpl3)->F1_high = tmp_chpl7;
#line 1506 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 1506 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  (this_chpl31)->F4__iterator = _ic__chpl3;
#line 1506 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 1506 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  (this_chpl31)->F5_i = INT64(0);
#line 1506 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 1506 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  tmp_chpl12 = &((this_chpl31)->F4__iterator);
#line 1509 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  chpl_check_nil(tmp_chpl12, _ln_chpl, _fn_chpl);
#line 1509 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  low_chpl4 = (*(tmp_chpl12))->F0_low;
#line 1506 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 1506 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  tmp_chpl13 = &((this_chpl31)->F4__iterator);
#line 1509 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  chpl_check_nil(tmp_chpl13, _ln_chpl, _fn_chpl);
#line 1509 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  high_chpl = (*(tmp_chpl13))->F1_high;
#line 1506 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 1506 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  tmp_chpl14 = &((this_chpl31)->F4__iterator);
#line 1509 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  chpl_check_nil(tmp_chpl14, _ln_chpl, _fn_chpl);
#line 1509 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  low_chpl5 = (*(tmp_chpl14))->F0_low;
#line 1506 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 1506 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  tmp_chpl15 = &((this_chpl31)->F4__iterator);
#line 1509 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  chpl_check_nil(tmp_chpl15, _ln_chpl, _fn_chpl);
#line 1509 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  chpl_checkIfRangeIterWillOverflow(low_chpl5, (*(tmp_chpl15))->F1_high, INT64(1), low_chpl4, high_chpl, UINT8(true), _ln_chpl, _fn_chpl);
#line 1509 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  for (chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl),tmp_chpl21 = &((this_chpl31)->F4__iterator),chpl_check_nil(tmp_chpl21, _ln_chpl, _fn_chpl),tmp_chpl20 = (*(tmp_chpl21))->F0_low,chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl),(this_chpl31)->F6_low = tmp_chpl20,chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl),tmp_chpl22 = (this_chpl31)->F6_low,chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl),(this_chpl31)->F5_i = tmp_chpl22; (chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl),tmp_chpl23 = &((this_chpl31)->F4__iterator),chpl_check_nil(tmp_chpl23, _ln_chpl, _fn_chpl),high_chpl2 = (*(tmp_chpl23))->F1_high,chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl),((this_chpl31)->F5_i <= high_chpl2)); chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl),tmp_chpl24 = (this_chpl31)->F5_i,tmp_chpl24 += INT64(1),chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl),(this_chpl31)->F5_i = tmp_chpl24) {
#line 1506 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 1506 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    tmp_chpl16 = (this_chpl31)->F5_i;
#line 1506 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 1506 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    tmp_chpl17 = &((this_chpl31)->F1_localThis);
#line 1510 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    coerce_tmp_chpl4 = (tmp_chpl17)->buff;
#line 1510 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    call_tmp_chpl7 = (coerce_tmp_chpl4 + tmp_chpl16);
#line 1510 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    tmp_chpl18 = *(call_tmp_chpl7);
#line 1506 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 1506 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    (this_chpl31)->value = tmp_chpl18;
#line 1506 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 1506 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    (this_chpl31)->more = INT64(2);
#line 1506 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    goto _end_chpl;
#line 1506 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    _jump_break_2_chpl:;
#line 1506 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 1509 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    chpl_here_free(((c_ptr_void_chpl)((this_chpl31)->F4__iterator)), _ln_chpl, _fn_chpl);
#line 1506 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 1509 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    chpl_here_free(((c_ptr_void_chpl)((this_chpl31)->F3__iterator)), _ln_chpl, _fn_chpl);
#line 1506 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 1509 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    chpl_here_free(((c_ptr_void_chpl)((this_chpl31)->F2__iterator)), _ln_chpl, _fn_chpl);
#line 1506 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 1506 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    tmp_chpl19 = &((this_chpl31)->F1_localThis);
#line 1510 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    deinit_chpl95(tmp_chpl19, _ln_chpl, _fn_chpl);
#line 1510 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    goto _endchpl_bytes_chpl;
#line 1510 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    _jump_2_chpl:;
#line 1510 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  }
#line 1506 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 1509 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  chpl_here_free(((c_ptr_void_chpl)((this_chpl31)->F4__iterator)), _ln_chpl, _fn_chpl);
#line 1506 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 1509 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  chpl_here_free(((c_ptr_void_chpl)((this_chpl31)->F3__iterator)), _ln_chpl, _fn_chpl);
#line 1506 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 1509 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  chpl_here_free(((c_ptr_void_chpl)((this_chpl31)->F2__iterator)), _ln_chpl, _fn_chpl);
#line 1506 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 1506 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  tmp_chpl25 = &((this_chpl31)->F1_localThis);
#line 1510 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  deinit_chpl95(tmp_chpl25, _ln_chpl, _fn_chpl);
#line 1510 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _endchpl_bytes_chpl:;
#line 1506 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 1506 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  (this_chpl31)->more = INT64(0);
#line 1506 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _end_chpl:;
#line 1506 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  return;
#line 1506 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
}

#line 1537 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
/* String.chpl:1537 */
#line 1537 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
static uint8_t byte_chpl(string_chpl * this_chpl31,
#line 1537 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
                         int64_t i_chpl,
#line 1537 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
                         int64_t _ln_chpl,
#line 1537 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
                         int32_t _fn_chpl) {
#line 1538 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  chpl_bool tmp_chpl;
#line 1539 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  int64_t ret_chpl;
#line 1540 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  c_ptr_uint8_t_chpl coerce_tmp_chpl3 = NULL;
#line 1540 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  uint8_t ret_chpl2;
#line 1540 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _ref_uint8_t_chpl call_tmp_chpl6 = NULL;
#line 1538 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  if (i_chpl < INT64(0)) /* ZLINE: 1538 /Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl */
#line 1538 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  {
#line 1538 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    tmp_chpl = UINT8(true);
#line 1538 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  } else /* ZLINE: 1538 /Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl */
#line 1538 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  {
#line 1538 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    tmp_chpl = (i_chpl >= (this_chpl31)->buffLen);
#line 1538 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  }
#line 1538 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  if (tmp_chpl) /* ZLINE: 1538 /Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl */
#line 1538 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  {
#line 1539 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    ret_chpl = (this_chpl31)->buffLen;
#line 1539 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    halt_chpl2(&_str_literal_218_chpl, i_chpl, &_str_literal_258_chpl, ret_chpl, &_str_literal_259_chpl, _ln_chpl, _fn_chpl);
#line 1539 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  }
#line 1540 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  coerce_tmp_chpl3 = (this_chpl31)->buff;
#line 1540 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  call_tmp_chpl6 = (coerce_tmp_chpl3 + i_chpl);
#line 1540 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  ret_chpl2 = *(call_tmp_chpl6);
#line 1537 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  return ret_chpl2;
#line 1537 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
}

#line 1638 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
/* String.chpl:1638 */
#line 1638 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
static void this_chpl30(string_chpl * this_chpl31,
#line 1638 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
                        codepointIndex_chpl * i_chpl,
#line 1638 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
                        string_chpl * _retArg_chpl,
#line 1638 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
                        int64_t _ln_chpl,
#line 1638 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
                        int32_t _fn_chpl) {
#line 1639 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  string_chpl call_tmp_chpl6;
#line 1639 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  string_chpl ret_tmp_chpl;
#line 1639 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  item_chpl(this_chpl31, i_chpl, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 1639 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  call_tmp_chpl6 = ret_tmp_chpl;
#line 1638 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  *(_retArg_chpl) = call_tmp_chpl6;
#line 1638 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  return;
#line 1638 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
}

#line 1658 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
/* String.chpl:1658 */
#line 1658 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
static void item_chpl(string_chpl * this_chpl31,
#line 1658 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
                      codepointIndex_chpl * i_chpl,
#line 1658 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
                      string_chpl * _retArg_chpl,
#line 1658 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
                      int64_t _ln_chpl,
#line 1658 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
                      int32_t _fn_chpl) {
#line 1658 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  string_chpl ret_chpl;
#line 1662 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  string_chpl inlineImm_chpl;
#line 1662 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 1662 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  string_chpl ret_chpl2;
#line 1662 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  string_chpl tmp_chpl;
#line 1663 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  int64_t ret_chpl3;
#line 1663 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  int64_t ret_chpl4;
#line 1664 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  int64_t ret_chpl5;
#line 1665 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  int64_t ret_chpl6;
#line 1665 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  int64_t ret_chpl7;
#line 1666 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  int64_t inlineImm_chpl2;
#line 1666 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  c_ptr_uint8_t_chpl call_tmp_chpl6 = NULL;
#line 1666 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  int64_t call_tmp_chpl7;
#line 1666 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  int64_t tmp_chpl2;
#line 1666 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  int16_t call_tmp_chpl8;
#line 1666 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  c_ptr_void_chpl cast_tmp_chpl = NULL;
#line 1666 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  c_ptr_uint8_t_chpl call_tmp_chpl9 = NULL;
#line 1666 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  c_ptr_uint8_t_chpl dst_chpl = NULL;
#line 1666 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  c_ptr_uint8_t_chpl call_tmp_chpl10 = NULL;
#line 1666 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  c_ptr_uint8_t_chpl call_tmp_chpl11 = NULL;
#line 1666 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _owned_IllegalArgumentError_chpl2 call_tmp_chpl12;
#line 1666 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _owned_IllegalArgumentError_chpl2 ret_tmp_chpl;
#line 1666 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _ref__owned_IllegalArgumentError_chpl i_arg_chpl = NULL;
#line 1666 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  IllegalArgumentError_chpl call_tmp_chpl13 = NULL;
#line 1666 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  string_chpl call_tmp_chpl14;
#line 1666 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  Error_chpl coerce_tmp_chpl3 = NULL;
#line 1666 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  string_chpl ret_tmp_chpl2;
#line 1666 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _ref_string_chpl i_x_chpl2 = NULL;
#line 1666 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _ref_uint8_t_chpl call_tmp_chpl15 = NULL;
#line 1668 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  string_chpl ret_chpl8;
#line 1668 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  string_chpl ret_chpl9;
#line 1668 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _ref_string_chpl i_x_chpl3 = NULL;
#line 1671 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  int64_t charCount_chpl;
#line 1672 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  byteIndex_chpl default_arg_start_chpl;
#line 1672 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  byteIndex_chpl ret_chpl10;
#line 1672 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  byteIndex_chpl cpi_chpl;
#line 1672 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _ref_byteIndex_chpl i_start_chpl = NULL;
#line 1672 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  string_chpl ret_chpl11;
#line 1672 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  string_chpl ret_chpl12;
#line 1672 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  string_chpl localThis_chpl;
#line 1672 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  int64_t call_tmp_chpl16;
#line 1672 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  int64_t i_chpl2;
#line 1672 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  chpl_bool tmp_chpl3;
#line 1672 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _tuple_3_int32_t_byteIndex_int64_t_chpl call_tmp_chpl17;
#line 1672 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _tuple_3_int32_t_byteIndex_int64_t_chpl ret_tmp_chpl3;
#line 1672 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  byteIndex_chpl _yieldedIndex_x1_chpl;
#line 1672 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  int64_t _yieldedIndex_x2_chpl;
#line 1672 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  byteIndex_chpl byteIdx_chpl;
#line 1672 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  int64_t nBytes_chpl;
#line 1674 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _tuple_2_c_ptr_uint8_t_int64_t_chpl call_tmp_chpl18;
#line 1674 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _tuple_2_c_ptr_uint8_t_int64_t_chpl ret_chpl13;
#line 1674 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  c_ptr_uint8_t_chpl call_tmp_chpl19 = NULL;
#line 1674 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  int64_t call_tmp_chpl20;
#line 1674 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  int64_t tmp_chpl4;
#line 1674 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  int16_t call_tmp_chpl21;
#line 1674 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  c_ptr_void_chpl cast_tmp_chpl2 = NULL;
#line 1674 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  c_ptr_uint8_t_chpl call_tmp_chpl22 = NULL;
#line 1674 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  c_ptr_uint8_t_chpl dst_chpl2 = NULL;
#line 1674 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  c_ptr_uint8_t_chpl call_tmp_chpl23 = NULL;
#line 1674 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  c_ptr_uint8_t_chpl call_tmp_chpl24 = NULL;
#line 1674 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _owned_IllegalArgumentError_chpl2 call_tmp_chpl25;
#line 1674 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _owned_IllegalArgumentError_chpl2 ret_tmp_chpl4;
#line 1674 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _ref__owned_IllegalArgumentError_chpl i_arg_chpl2 = NULL;
#line 1674 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  IllegalArgumentError_chpl call_tmp_chpl26 = NULL;
#line 1674 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  string_chpl call_tmp_chpl27;
#line 1674 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  Error_chpl coerce_tmp_chpl4 = NULL;
#line 1674 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  string_chpl ret_tmp_chpl5;
#line 1674 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _ref_string_chpl i_x_chpl4 = NULL;
#line 1674 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _ref_uint8_t_chpl call_tmp_chpl28 = NULL;
#line 1674 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  c_ptr_uint8_t_chpl tmp_x0_chpl = NULL;
#line 1674 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  int64_t tmp_x1_chpl;
#line 1676 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  string_chpl ret_chpl14;
#line 1676 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  string_chpl ret_chpl15;
#line 1676 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _ref_string_chpl i_x_chpl5 = NULL;
#line 1672 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _ref_string_chpl i_x_chpl6 = NULL;
#line 1678 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _ref_int64_t_chpl i_lhs_chpl = NULL;
#line 1672 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _ref_string_chpl i_x_chpl7 = NULL;
#line 1681 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  int64_t ret_chpl16;
#line 1681 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  int64_t ret_chpl17;
#line 1659 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  if ((i_chpl)->_cpindex < INT64(0)) /* ZLINE: 1659 /Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl */
#line 1659 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  {
#line 1660 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    halt_chpl3(&_str_literal_218_chpl, (i_chpl)->_cpindex, &_str_literal_262_chpl, _ln_chpl, _fn_chpl);
#line 1660 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  }
#line 1662 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  if ((this_chpl31)->buffLen == INT64(0)) /* ZLINE: 1662 /Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl */
#line 1662 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  {
#line 1662 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    inlineImm_chpl = _str_literal_163_chpl;
#line 1662 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    i_x_chpl = &inlineImm_chpl;
#line 1662 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    init_ASSIGN__chpl25(&tmp_chpl, i_x_chpl, _ln_chpl, _fn_chpl);
#line 1662 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    ret_chpl2 = tmp_chpl;
#line 1662 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    ret_chpl = ret_chpl2;
#line 1662 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    goto _end_item_chpl;
#line 1662 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  }
#line 1663 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  ret_chpl3 = (this_chpl31)->cachedNumCodepoints;
#line 1663 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  ret_chpl4 = (this_chpl31)->buffLen;
#line 1663 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  if (ret_chpl3 == ret_chpl4) /* ZLINE: 1663 /Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl */
#line 1663 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  {
#line 1664 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    ret_chpl5 = (this_chpl31)->buffLen;
#line 1664 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    if ((i_chpl)->_cpindex >= ret_chpl5) /* ZLINE: 1664 /Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl */
#line 1664 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    {
#line 1665 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      ret_chpl6 = (i_chpl)->_cpindex;
#line 1665 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      ret_chpl7 = (this_chpl31)->cachedNumCodepoints;
#line 1665 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      halt_chpl4(&_str_literal_218_chpl, ret_chpl6, &_str_literal_263_chpl, ret_chpl7, _ln_chpl, _fn_chpl);
#line 1665 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    }
#line 1666 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    inlineImm_chpl2 = INT64(1);
#line 1666 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    call_tmp_chpl6 = ((this_chpl31)->buff + (i_chpl)->_cpindex);
#line 1666 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    call_tmp_chpl7 = chpl_here_good_alloc_size(((int64_t)((INT64(1) + INT64(1)))), _ln_chpl, _fn_chpl);
#line 1666 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    if (call_tmp_chpl7 > INT64(0)) /* ZLINE: 1666 /Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl */
#line 1666 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    {
#line 1666 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      tmp_chpl2 = call_tmp_chpl7;
#line 1666 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    } else /* ZLINE: 1666 /Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl */
#line 1666 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    {
#line 1666 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      tmp_chpl2 = INT64(0);
#line 1666 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    }
#line 1666 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    call_tmp_chpl8 = offset_STR_COPY_DATA_chpl();
#line 1666 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    cast_tmp_chpl = chpl_here_alloc(tmp_chpl2, call_tmp_chpl8, _ln_chpl, _fn_chpl);
#line 1666 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    call_tmp_chpl9 = ((c_ptr_uint8_t_chpl)(cast_tmp_chpl));
#line 1666 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    dst_chpl = call_tmp_chpl9;
#line 1666 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    call_tmp_chpl10 = (call_tmp_chpl9 + INT64(0));
#line 1666 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    call_tmp_chpl11 = (call_tmp_chpl6 + INT64(0));
#line 1666 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    chpl_checkValue7(INT64(1), &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 1666 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    call_tmp_chpl12 = ret_tmp_chpl;
#line 1666 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    i_arg_chpl = &call_tmp_chpl12;
#line 1666 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    call_tmp_chpl13 = borrow_chpl2(i_arg_chpl);
#line 1666 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    if (((RootClass_chpl)(call_tmp_chpl13)) != nil) /* ZLINE: 1666 /Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl */
#line 1666 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    {
#line 1666 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      coerce_tmp_chpl3 = ((Error_chpl)(call_tmp_chpl13));
#line 1666 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      chpl_check_nil(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 1666 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      ((void(*)(Error_chpl,
#line 1666 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
         string_chpl *,
#line 1666 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
         int64_t,
#line 1666 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
         int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(coerce_tmp_chpl3))->chpl__cid) + INT64(1))])(coerce_tmp_chpl3, &ret_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 1666 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      call_tmp_chpl14 = ret_tmp_chpl2;
#line 1666 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      safeCastCheckHalt_chpl(&call_tmp_chpl14, _ln_chpl, _fn_chpl);
#line 1666 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      i_x_chpl2 = &call_tmp_chpl14;
#line 1666 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      deinit_chpl95(i_x_chpl2, _ln_chpl, _fn_chpl);
#line 1666 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    }
#line 1666 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    chpl__autoDestroy62(&call_tmp_chpl12, _ln_chpl, _fn_chpl);
#line 1666 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    memcpy(((c_ptr_void_chpl)(call_tmp_chpl10)), ((c_ptr_void_chpl)(call_tmp_chpl11)), ((uint64_t)(inlineImm_chpl2)));
#line 1666 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    call_tmp_chpl15 = (dst_chpl + inlineImm_chpl2);
#line 1666 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    *(call_tmp_chpl15) = ((uint8_t)(INT64(0)));
#line 1668 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    init_chpl194(&ret_chpl9, _ln_chpl, _fn_chpl);
#line 1668 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    i_x_chpl3 = &ret_chpl9;
#line 1668 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    (i_x_chpl3)->isOwned = UINT8(true);
#line 1668 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    (i_x_chpl3)->buff = call_tmp_chpl9;
#line 1668 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    (i_x_chpl3)->buffSize = tmp_chpl2;
#line 1668 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    (i_x_chpl3)->buffLen = INT64(1);
#line 1668 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    (&ret_chpl9)->cachedNumCodepoints = INT64(1);
#line 1668 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    ret_chpl8 = ret_chpl9;
#line 1668 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    ret_chpl = ret_chpl8;
#line 1668 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    goto _end_item_chpl;
#line 1668 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  } else /* ZLINE: 1670 /Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl */
#line 1668 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  {
#line 1671 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    charCount_chpl = INT64(0);
#line 1672 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    init_chpl192(&cpi_chpl);
#line 1672 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    (&cpi_chpl)->_bindex = INT64(0);
#line 1672 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    ret_chpl10 = cpi_chpl;
#line 1672 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    default_arg_start_chpl = ret_chpl10;
#line 1672 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    i_start_chpl = &default_arg_start_chpl;
#line 1672 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    init_chpl194(&ret_chpl12, _ln_chpl, _fn_chpl);
#line 1672 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    (&ret_chpl12)->cachedNumCodepoints = (this_chpl31)->cachedNumCodepoints;
#line 1672 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    initWithBorrowedBuffer_chpl(&ret_chpl12, this_chpl31, _ln_chpl, _fn_chpl);
#line 1672 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    ret_chpl11 = ret_chpl12;
#line 1672 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    localThis_chpl = ret_chpl11;
#line 1672 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    call_tmp_chpl16 = _findStartOfNextCodepointFromByte_chpl(&localThis_chpl, i_start_chpl, _ln_chpl, _fn_chpl);
#line 1672 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    i_chpl2 = call_tmp_chpl16;
#line 1672 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    tmp_chpl3 = (call_tmp_chpl16 < (&localThis_chpl)->buffLen);
#line 1672 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    while (tmp_chpl3) {
#line 1672 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      _cpIndexLenHelpNoAdjustment_chpl(&localThis_chpl, &i_chpl2, &ret_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 1672 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      call_tmp_chpl17 = ret_tmp_chpl3;
#line 1672 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      _yieldedIndex_x1_chpl = (&call_tmp_chpl17)->x1;
#line 1672 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      _yieldedIndex_x2_chpl = (&call_tmp_chpl17)->x2;
#line 1672 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      byteIdx_chpl = _yieldedIndex_x1_chpl;
#line 1672 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      nBytes_chpl = _yieldedIndex_x2_chpl;
#line 1673 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      if (charCount_chpl == (i_chpl)->_cpindex) /* ZLINE: 1673 /Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl */
#line 1673 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      {
#line 1674 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        call_tmp_chpl19 = ((this_chpl31)->buff + (&byteIdx_chpl)->_bindex);
#line 1674 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        call_tmp_chpl20 = chpl_here_good_alloc_size(((int64_t)((_yieldedIndex_x2_chpl + INT64(1)))), _ln_chpl, _fn_chpl);
#line 1674 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        if (call_tmp_chpl20 > INT64(0)) /* ZLINE: 1674 /Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl */
#line 1674 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        {
#line 1674 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
          tmp_chpl4 = call_tmp_chpl20;
#line 1674 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        } else /* ZLINE: 1674 /Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl */
#line 1674 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        {
#line 1674 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
          tmp_chpl4 = INT64(0);
#line 1674 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        }
#line 1674 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        call_tmp_chpl21 = offset_STR_COPY_DATA_chpl();
#line 1674 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        cast_tmp_chpl2 = chpl_here_alloc(tmp_chpl4, call_tmp_chpl21, _ln_chpl, _fn_chpl);
#line 1674 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        call_tmp_chpl22 = ((c_ptr_uint8_t_chpl)(cast_tmp_chpl2));
#line 1674 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        dst_chpl2 = call_tmp_chpl22;
#line 1674 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        call_tmp_chpl23 = (call_tmp_chpl22 + INT64(0));
#line 1674 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        call_tmp_chpl24 = (call_tmp_chpl19 + INT64(0));
#line 1674 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        chpl_checkValue7(_yieldedIndex_x2_chpl, &ret_tmp_chpl4, _ln_chpl, _fn_chpl);
#line 1674 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        call_tmp_chpl25 = ret_tmp_chpl4;
#line 1674 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        i_arg_chpl2 = &call_tmp_chpl25;
#line 1674 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        call_tmp_chpl26 = borrow_chpl2(i_arg_chpl2);
#line 1674 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        if (((RootClass_chpl)(call_tmp_chpl26)) != nil) /* ZLINE: 1674 /Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl */
#line 1674 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        {
#line 1674 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
          coerce_tmp_chpl4 = ((Error_chpl)(call_tmp_chpl26));
#line 1674 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
          chpl_check_nil(coerce_tmp_chpl4, _ln_chpl, _fn_chpl);
#line 1674 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
          ((void(*)(Error_chpl,
#line 1674 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
             string_chpl *,
#line 1674 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
             int64_t,
#line 1674 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
             int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(coerce_tmp_chpl4))->chpl__cid) + INT64(1))])(coerce_tmp_chpl4, &ret_tmp_chpl5, _ln_chpl, _fn_chpl);
#line 1674 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
          call_tmp_chpl27 = ret_tmp_chpl5;
#line 1674 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
          safeCastCheckHalt_chpl(&call_tmp_chpl27, _ln_chpl, _fn_chpl);
#line 1674 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
          i_x_chpl4 = &call_tmp_chpl27;
#line 1674 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
          deinit_chpl95(i_x_chpl4, _ln_chpl, _fn_chpl);
#line 1674 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        }
#line 1674 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        chpl__autoDestroy62(&call_tmp_chpl25, _ln_chpl, _fn_chpl);
#line 1674 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        memcpy(((c_ptr_void_chpl)(call_tmp_chpl23)), ((c_ptr_void_chpl)(call_tmp_chpl24)), ((uint64_t)(nBytes_chpl)));
#line 1674 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        call_tmp_chpl28 = (dst_chpl2 + nBytes_chpl);
#line 1674 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        *(call_tmp_chpl28) = ((uint8_t)(INT64(0)));
#line 1674 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        (&ret_chpl13)->x0 = call_tmp_chpl22;
#line 1674 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        (&ret_chpl13)->x1 = tmp_chpl4;
#line 1674 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        call_tmp_chpl18 = ret_chpl13;
#line 1674 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        tmp_x0_chpl = (&call_tmp_chpl18)->x0;
#line 1674 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        tmp_x1_chpl = (&call_tmp_chpl18)->x1;
#line 1676 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        init_chpl194(&ret_chpl15, _ln_chpl, _fn_chpl);
#line 1676 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        i_x_chpl5 = &ret_chpl15;
#line 1676 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        (i_x_chpl5)->isOwned = UINT8(true);
#line 1676 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        (i_x_chpl5)->buff = tmp_x0_chpl;
#line 1676 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        (i_x_chpl5)->buffSize = tmp_x1_chpl;
#line 1676 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        (i_x_chpl5)->buffLen = nBytes_chpl;
#line 1676 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        (&ret_chpl15)->cachedNumCodepoints = INT64(1);
#line 1676 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        ret_chpl14 = ret_chpl15;
#line 1676 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        ret_chpl = ret_chpl14;
#line 1672 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        i_x_chpl6 = &localThis_chpl;
#line 1672 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        deinit_chpl95(i_x_chpl6, _ln_chpl, _fn_chpl);
#line 1676 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        goto _end_item_chpl;
#line 1676 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      }
#line 1678 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      i_lhs_chpl = &charCount_chpl;
#line 1678 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      *(i_lhs_chpl) += INT64(1);
#line 1672 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      tmp_chpl3 = (i_chpl2 < (&localThis_chpl)->buffLen);
#line 1672 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    }
#line 1672 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    i_x_chpl7 = &localThis_chpl;
#line 1672 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    deinit_chpl95(i_x_chpl7, _ln_chpl, _fn_chpl);
#line 1681 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    ret_chpl16 = (i_chpl)->_cpindex;
#line 1681 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    ret_chpl17 = (this_chpl31)->cachedNumCodepoints;
#line 1681 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    halt_chpl4(&_str_literal_218_chpl, ret_chpl16, &_str_literal_263_chpl, ret_chpl17, _ln_chpl, _fn_chpl);
#line 1681 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  }
#line 1681 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _end_item_chpl:;
#line 1658 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  *(_retArg_chpl) = ret_chpl;
#line 1658 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  return;
#line 1658 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
}

#line 1903 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
/* String.chpl:1903 */
#line 1903 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
static void strip_chpl(string_chpl * this_chpl31,
#line 1903 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
                       string_chpl * chars_chpl,
#line 1903 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
                       chpl_bool leading_chpl,
#line 1903 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
                       chpl_bool trailing_chpl,
#line 1903 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
                       string_chpl * _retArg_chpl,
#line 1903 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
                       int64_t _ln_chpl,
#line 1903 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
                       int32_t _fn_chpl) {
#line 1903 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  string_chpl ret_chpl;
#line 1905 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  int64_t ret_chpl2;
#line 1905 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  int64_t ret_chpl3;
#line 1906 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  string_chpl call_tmp_chpl6;
#line 1906 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  string_chpl ret_tmp_chpl;
#line 1908 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  string_chpl inlineImm_chpl;
#line 1908 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 1908 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  string_chpl ret_chpl4;
#line 1908 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  string_chpl tmp_chpl;
#line 1909 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  string_chpl ret_chpl5;
#line 1909 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  string_chpl tmp_chpl2;
#line 1911 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  string_chpl ret_chpl6;
#line 1911 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  string_chpl ret_chpl7;
#line 1911 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  string_chpl localThis_chpl;
#line 1912 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  string_chpl ret_chpl8;
#line 1912 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  string_chpl ret_chpl9;
#line 1912 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  string_chpl localChars_chpl;
#line 1914 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  byteIndex_chpl start_chpl2;
#line 1915 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  byteIndex_chpl end_chpl;
#line 1918 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  byteIndex_chpl default_arg_start_chpl;
#line 1918 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  byteIndex_chpl ret_chpl10;
#line 1918 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  byteIndex_chpl cpi_chpl;
#line 1918 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _ref_string_chpl i_this_chpl = NULL;
#line 1918 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _ref_byteIndex_chpl i_start_chpl = NULL;
#line 1918 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  string_chpl ret_chpl11;
#line 1918 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  string_chpl ret_chpl12;
#line 1918 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  string_chpl localThis_chpl2;
#line 1918 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  int64_t call_tmp_chpl7;
#line 1918 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  int64_t i_chpl;
#line 1918 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  chpl_bool tmp_chpl3;
#line 1918 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _tuple_3_int32_t_byteIndex_int64_t_chpl call_tmp_chpl8;
#line 1918 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _tuple_3_int32_t_byteIndex_int64_t_chpl ret_tmp_chpl2;
#line 1918 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  byteIndex_chpl _yieldedIndex_x1_chpl;
#line 1918 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  int32_t thisChar_chpl;
#line 1918 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  byteIndex_chpl i_chpl2;
#line 1918 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  int64_t nBytes_chpl;
#line 1919 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _ref_string_chpl i_this_chpl2 = NULL;
#line 1919 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  string_chpl ret_chpl13;
#line 1919 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  string_chpl ret_chpl14;
#line 1919 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  string_chpl localThis_chpl3;
#line 1919 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  int64_t i_chpl3;
#line 1919 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  chpl_bool tmp_chpl4;
#line 1919 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _tuple_3_int32_t_byteIndex_int64_t_chpl ret_tmp_chpl3;
#line 1921 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  byteIndex_chpl call_tmp_chpl9;
#line 1921 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  int64_t call_tmp_chpl10;
#line 1921 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  byteIndex_chpl ret_chpl15;
#line 1921 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  byteIndex_chpl cpi_chpl2;
#line 1919 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _ref_string_chpl i_x_chpl2 = NULL;
#line 1919 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _ref_string_chpl i_x_chpl3 = NULL;
#line 1918 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _ref_string_chpl i_x_chpl4 = NULL;
#line 1918 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _ref_string_chpl i_x_chpl5 = NULL;
#line 1935 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _ref_string_chpl i_this_chpl3 = NULL;
#line 1935 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _ref_byteIndex_chpl i_start_chpl2 = NULL;
#line 1935 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  string_chpl ret_chpl16;
#line 1935 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  string_chpl ret_chpl17;
#line 1935 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  string_chpl localThis_chpl4;
#line 1935 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  int64_t call_tmp_chpl11;
#line 1935 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  int64_t i_chpl4;
#line 1935 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  chpl_bool tmp_chpl5;
#line 1935 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _tuple_3_int32_t_byteIndex_int64_t_chpl call_tmp_chpl12;
#line 1935 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _tuple_3_int32_t_byteIndex_int64_t_chpl ret_tmp_chpl4;
#line 1935 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  byteIndex_chpl _yieldedIndex_x1_chpl2;
#line 1935 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  int32_t thisChar_chpl2;
#line 1935 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  byteIndex_chpl i_chpl5;
#line 1935 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  int64_t nBytes_chpl2;
#line 1936 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _ref_string_chpl i_this_chpl4 = NULL;
#line 1936 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  string_chpl ret_chpl18;
#line 1936 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  string_chpl ret_chpl19;
#line 1936 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  string_chpl localThis_chpl5;
#line 1936 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  int64_t i_chpl6;
#line 1936 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  chpl_bool tmp_chpl6;
#line 1936 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _tuple_3_int32_t_byteIndex_int64_t_chpl ret_tmp_chpl5;
#line 1936 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _ref_string_chpl i_x_chpl6 = NULL;
#line 1936 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _ref_string_chpl i_x_chpl7 = NULL;
#line 1942 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  byteIndex_chpl call_tmp_chpl13;
#line 1942 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  int64_t call_tmp_chpl14;
#line 1942 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  byteIndex_chpl ret_chpl20;
#line 1942 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  byteIndex_chpl cpi_chpl3;
#line 1942 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  byteIndex_chpl call_tmp_chpl15;
#line 1942 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  int64_t call_tmp_chpl16;
#line 1942 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  byteIndex_chpl ret_chpl21;
#line 1942 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  byteIndex_chpl cpi_chpl4;
#line 1935 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _ref_string_chpl i_x_chpl8 = NULL;
#line 1946 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  range_byteIndex_both_one_chpl call_tmp_chpl17;
#line 1946 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  range_byteIndex_both_one_chpl ret_tmp_chpl6;
#line 1946 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  Error_chpl error_chpl = NULL;
#line 1946 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _ref_string_chpl i_this_chpl5 = NULL;
#line 1946 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  string_chpl ret_chpl22;
#line 1946 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  string_chpl call_tmp_chpl18;
#line 1946 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  Error_chpl error_chpl2 = NULL;
#line 1946 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  string_chpl ret_tmp_chpl7;
#line 1946 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _ref_string_chpl i_x_chpl9 = NULL;
#line 1946 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _ref_string_chpl i_x_chpl10 = NULL;
#line 1905 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  ret_chpl2 = (this_chpl31)->cachedNumCodepoints;
#line 1905 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  ret_chpl3 = (this_chpl31)->buffLen;
#line 1905 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  if (ret_chpl2 == ret_chpl3) /* ZLINE: 1905 /Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl */
#line 1905 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  {
#line 1906 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    doStripNoEnc_chpl(this_chpl31, chars_chpl, leading_chpl, trailing_chpl, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 1906 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    call_tmp_chpl6 = ret_tmp_chpl;
#line 1906 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    ret_chpl = call_tmp_chpl6;
#line 1906 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    goto _end_strip_chpl;
#line 1906 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  } else /* ZLINE: 1907 /Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl */
#line 1906 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  {
#line 1908 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    if ((this_chpl31)->buffLen == INT64(0)) /* ZLINE: 1908 /Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl */
#line 1908 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    {
#line 1908 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      inlineImm_chpl = _str_literal_163_chpl;
#line 1908 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      i_x_chpl = &inlineImm_chpl;
#line 1908 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      init_ASSIGN__chpl25(&tmp_chpl, i_x_chpl, _ln_chpl, _fn_chpl);
#line 1908 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      ret_chpl4 = tmp_chpl;
#line 1908 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      ret_chpl = ret_chpl4;
#line 1908 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      goto _end_strip_chpl;
#line 1908 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    }
#line 1909 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    if ((chars_chpl)->buffLen == INT64(0)) /* ZLINE: 1909 /Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl */
#line 1909 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    {
#line 1909 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      init_ASSIGN__chpl25(&tmp_chpl2, this_chpl31, _ln_chpl, _fn_chpl);
#line 1909 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      ret_chpl5 = tmp_chpl2;
#line 1909 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      ret_chpl = ret_chpl5;
#line 1909 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      goto _end_strip_chpl;
#line 1909 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    }
#line 1911 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    init_chpl194(&ret_chpl7, _ln_chpl, _fn_chpl);
#line 1911 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    (&ret_chpl7)->cachedNumCodepoints = (this_chpl31)->cachedNumCodepoints;
#line 1911 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    initWithBorrowedBuffer_chpl(&ret_chpl7, this_chpl31, _ln_chpl, _fn_chpl);
#line 1911 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    ret_chpl6 = ret_chpl7;
#line 1911 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    localThis_chpl = ret_chpl6;
#line 1912 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    init_chpl194(&ret_chpl9, _ln_chpl, _fn_chpl);
#line 1912 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    (&ret_chpl9)->cachedNumCodepoints = (chars_chpl)->cachedNumCodepoints;
#line 1912 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    initWithBorrowedBuffer_chpl(&ret_chpl9, chars_chpl, _ln_chpl, _fn_chpl);
#line 1912 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    ret_chpl8 = ret_chpl9;
#line 1912 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    localChars_chpl = ret_chpl8;
#line 1914 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    init_ASSIGN__chpl24(&start_chpl2, INT64(0));
#line 1915 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    init_ASSIGN__chpl24(&end_chpl, ((int64_t)(((&localThis_chpl)->buffLen - INT64(1)))));
#line 1917 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    if (leading_chpl) /* ZLINE: 1917 /Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl */
#line 1917 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    {
#line 1918 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      init_chpl192(&cpi_chpl);
#line 1918 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      (&cpi_chpl)->_bindex = INT64(0);
#line 1918 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      ret_chpl10 = cpi_chpl;
#line 1918 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      default_arg_start_chpl = ret_chpl10;
#line 1918 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      i_this_chpl = &localThis_chpl;
#line 1918 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      i_start_chpl = &default_arg_start_chpl;
#line 1918 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      init_chpl194(&ret_chpl12, _ln_chpl, _fn_chpl);
#line 1918 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      (&ret_chpl12)->cachedNumCodepoints = (&localThis_chpl)->cachedNumCodepoints;
#line 1918 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      initWithBorrowedBuffer_chpl(&ret_chpl12, i_this_chpl, _ln_chpl, _fn_chpl);
#line 1918 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      ret_chpl11 = ret_chpl12;
#line 1918 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      localThis_chpl2 = ret_chpl11;
#line 1918 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      call_tmp_chpl7 = _findStartOfNextCodepointFromByte_chpl(&localThis_chpl2, i_start_chpl, _ln_chpl, _fn_chpl);
#line 1918 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      i_chpl = call_tmp_chpl7;
#line 1918 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      tmp_chpl3 = (call_tmp_chpl7 < (&localThis_chpl2)->buffLen);
#line 1918 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      while (tmp_chpl3) {
#line 1918 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        _cpIndexLenHelpNoAdjustment_chpl(&localThis_chpl2, &i_chpl, &ret_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 1918 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        call_tmp_chpl8 = ret_tmp_chpl2;
#line 1918 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        _yieldedIndex_x1_chpl = (&call_tmp_chpl8)->x1;
#line 1918 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        thisChar_chpl = (&call_tmp_chpl8)->x0;
#line 1918 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        i_chpl2 = _yieldedIndex_x1_chpl;
#line 1918 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        nBytes_chpl = (&call_tmp_chpl8)->x2;
#line 1919 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        i_this_chpl2 = &localChars_chpl;
#line 1919 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        init_chpl194(&ret_chpl14, _ln_chpl, _fn_chpl);
#line 1919 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        (&ret_chpl14)->cachedNumCodepoints = (&localChars_chpl)->cachedNumCodepoints;
#line 1919 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        initWithBorrowedBuffer_chpl(&ret_chpl14, i_this_chpl2, _ln_chpl, _fn_chpl);
#line 1919 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        ret_chpl13 = ret_chpl14;
#line 1919 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        localThis_chpl3 = ret_chpl13;
#line 1919 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        i_chpl3 = INT64(0);
#line 1919 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        tmp_chpl4 = (INT64(0) < (&localThis_chpl3)->buffLen);
#line 1919 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        while (tmp_chpl4) {
#line 1919 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
          _cpIndexLenHelpNoAdjustment_chpl(&localThis_chpl3, &i_chpl3, &ret_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 1920 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
          if (thisChar_chpl == (&ret_tmp_chpl3)->x0) /* ZLINE: 1920 /Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl */
#line 1920 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
          {
#line 1921 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
            call_tmp_chpl10 = ((&i_chpl2)->_bindex + nBytes_chpl);
#line 1921 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
            init_chpl192(&cpi_chpl2);
#line 1921 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
            (&cpi_chpl2)->_bindex = call_tmp_chpl10;
#line 1921 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
            ret_chpl15 = cpi_chpl2;
#line 1921 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
            call_tmp_chpl9 = ret_chpl15;
#line 1921 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
            chpl___ASSIGN_15(&start_chpl2, &call_tmp_chpl9);
#line 1919 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
            i_x_chpl2 = &localThis_chpl3;
#line 1919 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
            deinit_chpl95(i_x_chpl2, _ln_chpl, _fn_chpl);
#line 1922 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
            goto _continueLabel_chpl;
#line 1922 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
          }
#line 1919 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
          tmp_chpl4 = (i_chpl3 < (&localThis_chpl3)->buffLen);
#line 1919 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        }
#line 1919 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        i_x_chpl3 = &localThis_chpl3;
#line 1919 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        deinit_chpl95(i_x_chpl3, _ln_chpl, _fn_chpl);
#line 1918 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        i_x_chpl4 = &localThis_chpl2;
#line 1918 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        deinit_chpl95(i_x_chpl4, _ln_chpl, _fn_chpl);
#line 1925 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        goto _breakLabel_chpl;
#line 1925 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        _continueLabel_chpl:;
#line 1918 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        tmp_chpl3 = (i_chpl < (&localThis_chpl2)->buffLen);
#line 1918 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      }
#line 1918 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      i_x_chpl5 = &localThis_chpl2;
#line 1918 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      deinit_chpl95(i_x_chpl5, _ln_chpl, _fn_chpl);
#line 1918 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      _breakLabel_chpl:;
#line 1918 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    }
#line 1929 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    if (trailing_chpl) /* ZLINE: 1929 /Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl */
#line 1929 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    {
#line 1934 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      chpl___ASSIGN_14(&end_chpl, INT64(-1));
#line 1935 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      i_this_chpl3 = &localThis_chpl;
#line 1935 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      i_start_chpl2 = &start_chpl2;
#line 1935 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      init_chpl194(&ret_chpl17, _ln_chpl, _fn_chpl);
#line 1935 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      (&ret_chpl17)->cachedNumCodepoints = (&localThis_chpl)->cachedNumCodepoints;
#line 1935 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      initWithBorrowedBuffer_chpl(&ret_chpl17, i_this_chpl3, _ln_chpl, _fn_chpl);
#line 1935 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      ret_chpl16 = ret_chpl17;
#line 1935 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      localThis_chpl4 = ret_chpl16;
#line 1935 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      call_tmp_chpl11 = _findStartOfNextCodepointFromByte_chpl(&localThis_chpl4, i_start_chpl2, _ln_chpl, _fn_chpl);
#line 1935 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      i_chpl4 = call_tmp_chpl11;
#line 1935 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      tmp_chpl5 = (call_tmp_chpl11 < (&localThis_chpl4)->buffLen);
#line 1935 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      while (tmp_chpl5) {
#line 1935 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        _cpIndexLenHelpNoAdjustment_chpl(&localThis_chpl4, &i_chpl4, &ret_tmp_chpl4, _ln_chpl, _fn_chpl);
#line 1935 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        call_tmp_chpl12 = ret_tmp_chpl4;
#line 1935 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        _yieldedIndex_x1_chpl2 = (&call_tmp_chpl12)->x1;
#line 1935 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        thisChar_chpl2 = (&call_tmp_chpl12)->x0;
#line 1935 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        i_chpl5 = _yieldedIndex_x1_chpl2;
#line 1935 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        nBytes_chpl2 = (&call_tmp_chpl12)->x2;
#line 1936 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        i_this_chpl4 = &localChars_chpl;
#line 1936 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        init_chpl194(&ret_chpl19, _ln_chpl, _fn_chpl);
#line 1936 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        (&ret_chpl19)->cachedNumCodepoints = (&localChars_chpl)->cachedNumCodepoints;
#line 1936 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        initWithBorrowedBuffer_chpl(&ret_chpl19, i_this_chpl4, _ln_chpl, _fn_chpl);
#line 1936 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        ret_chpl18 = ret_chpl19;
#line 1936 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        localThis_chpl5 = ret_chpl18;
#line 1936 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        i_chpl6 = INT64(0);
#line 1936 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        tmp_chpl6 = (INT64(0) < (&localThis_chpl5)->buffLen);
#line 1936 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        while (tmp_chpl6) {
#line 1936 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
          _cpIndexLenHelpNoAdjustment_chpl(&localThis_chpl5, &i_chpl6, &ret_tmp_chpl5, _ln_chpl, _fn_chpl);
#line 1937 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
          if (thisChar_chpl2 == (&ret_tmp_chpl5)->x0) /* ZLINE: 1937 /Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl */
#line 1937 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
          {
#line 1936 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
            i_x_chpl6 = &localThis_chpl5;
#line 1936 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
            deinit_chpl95(i_x_chpl6, _ln_chpl, _fn_chpl);
#line 1938 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
            goto _continueLabel_chpl2;
#line 1938 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
          }
#line 1936 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
          tmp_chpl6 = (i_chpl6 < (&localThis_chpl5)->buffLen);
#line 1936 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        }
#line 1936 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        i_x_chpl7 = &localThis_chpl5;
#line 1936 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        deinit_chpl95(i_x_chpl7, _ln_chpl, _fn_chpl);
#line 1942 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        call_tmp_chpl14 = ((&i_chpl5)->_bindex + nBytes_chpl2);
#line 1942 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        init_chpl192(&cpi_chpl3);
#line 1942 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        (&cpi_chpl3)->_bindex = call_tmp_chpl14;
#line 1942 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        ret_chpl20 = cpi_chpl3;
#line 1942 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        call_tmp_chpl13 = ret_chpl20;
#line 1942 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        call_tmp_chpl16 = ((&call_tmp_chpl13)->_bindex - INT64(1));
#line 1942 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        init_chpl192(&cpi_chpl4);
#line 1942 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        (&cpi_chpl4)->_bindex = call_tmp_chpl16;
#line 1942 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        ret_chpl21 = cpi_chpl4;
#line 1942 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        call_tmp_chpl15 = ret_chpl21;
#line 1942 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        chpl___ASSIGN_15(&end_chpl, &call_tmp_chpl15);
#line 1942 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        _continueLabel_chpl2:;
#line 1935 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        tmp_chpl5 = (i_chpl4 < (&localThis_chpl4)->buffLen);
#line 1935 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      }
#line 1935 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      i_x_chpl8 = &localThis_chpl4;
#line 1935 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      deinit_chpl95(i_x_chpl8, _ln_chpl, _fn_chpl);
#line 1935 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    }
#line 1946 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    chpl_build_bounded_range7(&start_chpl2, &end_chpl, &ret_tmp_chpl6);
#line 1946 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    call_tmp_chpl17 = ret_tmp_chpl6;
#line 1946 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    error_chpl = NULL;
#line 1946 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    i_this_chpl5 = &localThis_chpl;
#line 1946 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    error_chpl2 = NULL;
#line 1946 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    getSlice_chpl3(i_this_chpl5, call_tmp_chpl17, &error_chpl2, &ret_tmp_chpl7, _ln_chpl, _fn_chpl);
#line 1946 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    call_tmp_chpl18 = ret_tmp_chpl7;
#line 1946 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    if (error_chpl2 != nil) /* ZLINE: 1946 /Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl */
#line 1946 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    {
#line 1946 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      error_chpl = error_chpl2;
#line 1946 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      goto _endthis_chpl;
#line 1946 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    }
#line 1946 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    ret_chpl22 = call_tmp_chpl18;
#line 1946 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    _endthis_chpl:;
#line 1946 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    if (error_chpl != nil) /* ZLINE: 1946 /Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl */
#line 1946 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    {
#line 1946 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      chpl_uncaught_error(error_chpl, _ln_chpl, _fn_chpl);
#line 1946 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    }
#line 1946 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    ret_chpl = ret_chpl22;
#line 1946 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    i_x_chpl9 = &localChars_chpl;
#line 1946 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    deinit_chpl95(i_x_chpl9, _ln_chpl, _fn_chpl);
#line 1946 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    i_x_chpl10 = &localThis_chpl;
#line 1946 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    deinit_chpl95(i_x_chpl10, _ln_chpl, _fn_chpl);
#line 1946 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    goto _end_strip_chpl;
#line 1946 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  }
#line 1946 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _end_strip_chpl:;
#line 1903 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  *(_retArg_chpl) = ret_chpl;
#line 1903 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  return;
#line 1903 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
}

#line 2291 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
/* String.chpl:2291 */
#line 2291 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
static void chpl___ASSIGN_14(byteIndex_chpl * lhs_chpl,
#line 2291 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
                             int64_t rhs_chpl) {
#line 2292 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  (lhs_chpl)->_bindex = rhs_chpl;
#line 2291 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  return;
#line 2291 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
}

#line 2295 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
/* String.chpl:2295 */
#line 2295 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
static void chpl___ASSIGN_15(byteIndex_chpl * lhs_chpl,
#line 2295 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
                             byteIndex_chpl * rhs_chpl) {
#line 2295 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  *(lhs_chpl) = *(rhs_chpl);
#line 2295 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  return;
#line 2295 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
}

#line 2316 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
/* String.chpl:2316 */
#line 2316 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
static void chpl___ASSIGN_16(string_chpl * lhs_chpl,
#line 2316 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
                             string_chpl * rhs_chpl,
#line 2316 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
                             int64_t _ln_chpl,
#line 2316 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
                             int32_t _fn_chpl) {
#line 2317 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  doAssign_chpl(lhs_chpl, rhs_chpl, _ln_chpl, _fn_chpl);
#line 2316 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  return;
#line 2316 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
}

#line 2326 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
/* String.chpl:2326 */
#line 2326 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
static void chpl___PLUS_(string_chpl * s0_chpl,
#line 2326 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
                         string_chpl * s1_chpl,
#line 2326 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
                         string_chpl * _retArg_chpl,
#line 2326 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
                         int64_t _ln_chpl,
#line 2326 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
                         int32_t _fn_chpl) {
#line 2327 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  string_chpl call_tmp_chpl6;
#line 2327 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  string_chpl ret_tmp_chpl;
#line 2327 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  doConcat_chpl(s0_chpl, s1_chpl, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 2327 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  call_tmp_chpl6 = ret_tmp_chpl;
#line 2326 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  *(_retArg_chpl) = call_tmp_chpl6;
#line 2326 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  return;
#line 2326 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
}

#line 2445 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
/* String.chpl:2445 */
#line 2445 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
static void chpl___PLUS__ASSIGN_(string_chpl * lhs_chpl,
#line 2445 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
                                 string_chpl * rhs_chpl,
#line 2445 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
                                 int64_t _ln_chpl,
#line 2445 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
                                 int32_t _fn_chpl) {
#line 2446 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  doAppend_chpl(lhs_chpl, rhs_chpl, _ln_chpl, _fn_chpl);
#line 2445 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  return;
#line 2445 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
}

#line 2466 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
/* String.chpl:2466 */
#line 2466 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
static chpl_bool chpl___EQUALS_3(string_chpl * a_chpl,
#line 2466 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
                                 string_chpl * b_chpl,
#line 2466 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
                                 int64_t _ln_chpl,
#line 2466 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
                                 int32_t _fn_chpl) {
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  c_ptr_uint8_t_chpl coerce_tmp_chpl3 = NULL;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  int64_t coerce_tmp_chpl4;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  int32_t coerce_tmp_chpl5;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  c_ptr_uint8_t_chpl coerce_tmp_chpl6 = NULL;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  int64_t coerce_tmp_chpl7;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  int32_t coerce_tmp_chpl8;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  int64_t ret_chpl;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  int32_t call_tmp_chpl6;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  chpl_bool tmp_chpl;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  int32_t call_tmp_chpl7;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  int64_t ret_chpl2;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  int64_t tmp_chpl2;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  int64_t size_chpl9;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _owned_IllegalArgumentError_chpl2 call_tmp_chpl8;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _owned_IllegalArgumentError_chpl2 ret_tmp_chpl;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _ref__owned_IllegalArgumentError_chpl i_arg_chpl = NULL;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  IllegalArgumentError_chpl call_tmp_chpl9 = NULL;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  string_chpl call_tmp_chpl10;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  Error_chpl coerce_tmp_chpl9 = NULL;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  string_chpl ret_tmp_chpl2;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  int32_t call_tmp_chpl11;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _owned_IllegalArgumentError_chpl2 call_tmp_chpl12;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _owned_IllegalArgumentError_chpl2 ret_tmp_chpl3;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _ref__owned_IllegalArgumentError_chpl i_arg_chpl2 = NULL;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  IllegalArgumentError_chpl call_tmp_chpl13 = NULL;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  string_chpl call_tmp_chpl14;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  Error_chpl coerce_tmp_chpl10 = NULL;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  string_chpl ret_tmp_chpl4;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _ref_string_chpl i_x_chpl2 = NULL;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  int64_t call_tmp_chpl15;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  int32_t call_tmp_chpl16;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  chpl_bool tmp_chpl3;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  int32_t call_tmp_chpl17;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  c_ptr_uint8_t_chpl call_tmp_chpl18 = NULL;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  int64_t ret_chpl3;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  int64_t tmp_chpl4;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  int64_t size_chpl10;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _owned_IllegalArgumentError_chpl2 call_tmp_chpl19;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _owned_IllegalArgumentError_chpl2 ret_tmp_chpl5;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _ref__owned_IllegalArgumentError_chpl i_arg_chpl3 = NULL;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  IllegalArgumentError_chpl call_tmp_chpl20 = NULL;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  string_chpl call_tmp_chpl21;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  Error_chpl coerce_tmp_chpl11 = NULL;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  string_chpl ret_tmp_chpl6;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _ref_string_chpl i_x_chpl3 = NULL;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  int32_t call_tmp_chpl22;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _owned_IllegalArgumentError_chpl2 call_tmp_chpl23;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _owned_IllegalArgumentError_chpl2 ret_tmp_chpl7;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _ref__owned_IllegalArgumentError_chpl i_arg_chpl4 = NULL;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  IllegalArgumentError_chpl call_tmp_chpl24 = NULL;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  string_chpl call_tmp_chpl25;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  Error_chpl coerce_tmp_chpl12 = NULL;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  string_chpl ret_tmp_chpl8;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _ref_string_chpl i_x_chpl4 = NULL;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  int64_t call_tmp_chpl26;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  int32_t call_tmp_chpl27;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  chpl_bool tmp_chpl5;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  int32_t call_tmp_chpl28;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  c_ptr_uint8_t_chpl call_tmp_chpl29 = NULL;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  int64_t ret_chpl4;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  int64_t tmp_chpl6;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  int64_t size_chpl11;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _owned_IllegalArgumentError_chpl2 call_tmp_chpl30;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _owned_IllegalArgumentError_chpl2 ret_tmp_chpl9;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _ref__owned_IllegalArgumentError_chpl i_arg_chpl5 = NULL;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  IllegalArgumentError_chpl call_tmp_chpl31 = NULL;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  string_chpl call_tmp_chpl32;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  Error_chpl coerce_tmp_chpl13 = NULL;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  string_chpl ret_tmp_chpl10;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _ref_string_chpl i_x_chpl5 = NULL;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  int32_t call_tmp_chpl33;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _owned_IllegalArgumentError_chpl2 call_tmp_chpl34;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _owned_IllegalArgumentError_chpl2 ret_tmp_chpl11;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _ref__owned_IllegalArgumentError_chpl i_arg_chpl6 = NULL;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  IllegalArgumentError_chpl call_tmp_chpl35 = NULL;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  string_chpl call_tmp_chpl36;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  Error_chpl coerce_tmp_chpl14 = NULL;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  string_chpl ret_tmp_chpl12;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _ref_string_chpl i_x_chpl6 = NULL;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  int64_t call_tmp_chpl37;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  c_ptr_uint8_t_chpl call_tmp_chpl38 = NULL;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  c_ptr_uint8_t_chpl call_tmp_chpl39 = NULL;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  int64_t ret_chpl5;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  int64_t tmp_chpl7;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  int64_t size_chpl12;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _owned_IllegalArgumentError_chpl2 call_tmp_chpl40;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _owned_IllegalArgumentError_chpl2 ret_tmp_chpl13;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _ref__owned_IllegalArgumentError_chpl i_arg_chpl7 = NULL;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  IllegalArgumentError_chpl call_tmp_chpl41 = NULL;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  string_chpl call_tmp_chpl42;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  Error_chpl coerce_tmp_chpl15 = NULL;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  string_chpl ret_tmp_chpl14;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _ref_string_chpl i_x_chpl7 = NULL;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  int32_t call_tmp_chpl43;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _owned_IllegalArgumentError_chpl2 call_tmp_chpl44;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _owned_IllegalArgumentError_chpl2 ret_tmp_chpl15;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _ref__owned_IllegalArgumentError_chpl i_arg_chpl8 = NULL;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  IllegalArgumentError_chpl call_tmp_chpl45 = NULL;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  string_chpl call_tmp_chpl46;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  Error_chpl coerce_tmp_chpl16 = NULL;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  string_chpl ret_tmp_chpl16;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _ref_string_chpl i_x_chpl8 = NULL;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  int64_t call_tmp_chpl47;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  chpl_bool call_tmp_chpl48;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  coerce_tmp_chpl3 = (a_chpl)->buff;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  coerce_tmp_chpl4 = (a_chpl)->buffLen;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  coerce_tmp_chpl5 = (a_chpl)->locale_id;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  coerce_tmp_chpl6 = (b_chpl)->buff;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  coerce_tmp_chpl7 = (b_chpl)->buffLen;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  coerce_tmp_chpl8 = (b_chpl)->locale_id;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  call_tmp_chpl6 = get_chpl_nodeID();
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  if (coerce_tmp_chpl5 == call_tmp_chpl6) /* ZLINE: 2481 /Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl */
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  {
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    call_tmp_chpl7 = get_chpl_nodeID();
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    tmp_chpl = (coerce_tmp_chpl8 == call_tmp_chpl7);
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  } else /* ZLINE: 2481 /Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl */
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  {
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    tmp_chpl = UINT8(false);
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  }
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  if (tmp_chpl) /* ZLINE: 2481 /Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl */
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  {
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    if (coerce_tmp_chpl4 < coerce_tmp_chpl7) /* ZLINE: 2481 /Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl */
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    {
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      tmp_chpl2 = coerce_tmp_chpl4;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    } else /* ZLINE: 2481 /Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl */
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    {
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      tmp_chpl2 = coerce_tmp_chpl7;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    }
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    size_chpl9 = tmp_chpl2;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    chpl_checkValue7(tmp_chpl2, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    call_tmp_chpl8 = ret_tmp_chpl;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    i_arg_chpl = &call_tmp_chpl8;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    call_tmp_chpl9 = borrow_chpl2(i_arg_chpl);
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    if (((RootClass_chpl)(call_tmp_chpl9)) != nil) /* ZLINE: 2481 /Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl */
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    {
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      coerce_tmp_chpl9 = ((Error_chpl)(call_tmp_chpl9));
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      chpl_check_nil(coerce_tmp_chpl9, _ln_chpl, _fn_chpl);
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      ((void(*)(Error_chpl,
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
         string_chpl *,
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
         int64_t,
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
         int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(coerce_tmp_chpl9))->chpl__cid) + INT64(1))])(coerce_tmp_chpl9, &ret_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      call_tmp_chpl10 = ret_tmp_chpl2;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      safeCastCheckHalt_chpl(&call_tmp_chpl10, _ln_chpl, _fn_chpl);
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      i_x_chpl = &call_tmp_chpl10;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      deinit_chpl95(i_x_chpl, _ln_chpl, _fn_chpl);
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    }
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    chpl__autoDestroy62(&call_tmp_chpl8, _ln_chpl, _fn_chpl);
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    call_tmp_chpl11 = memcmp(((c_ptr_void_chpl)(coerce_tmp_chpl3)), ((c_ptr_void_chpl)(coerce_tmp_chpl6)), ((uint64_t)(size_chpl9)));
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    chpl_checkValue4(call_tmp_chpl11, &ret_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    call_tmp_chpl12 = ret_tmp_chpl3;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    i_arg_chpl2 = &call_tmp_chpl12;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    call_tmp_chpl13 = borrow_chpl2(i_arg_chpl2);
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    if (((RootClass_chpl)(call_tmp_chpl13)) != nil) /* ZLINE: 2481 /Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl */
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    {
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      coerce_tmp_chpl10 = ((Error_chpl)(call_tmp_chpl13));
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      chpl_check_nil(coerce_tmp_chpl10, _ln_chpl, _fn_chpl);
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      ((void(*)(Error_chpl,
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
         string_chpl *,
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
         int64_t,
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
         int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(coerce_tmp_chpl10))->chpl__cid) + INT64(1))])(coerce_tmp_chpl10, &ret_tmp_chpl4, _ln_chpl, _fn_chpl);
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      call_tmp_chpl14 = ret_tmp_chpl4;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      safeCastCheckHalt_chpl(&call_tmp_chpl14, _ln_chpl, _fn_chpl);
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      i_x_chpl2 = &call_tmp_chpl14;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      deinit_chpl95(i_x_chpl2, _ln_chpl, _fn_chpl);
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    }
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    chpl__autoDestroy62(&call_tmp_chpl12, _ln_chpl, _fn_chpl);
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    call_tmp_chpl15 = ((int64_t)(call_tmp_chpl11));
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    if (call_tmp_chpl15 == INT64(0)) /* ZLINE: 2481 /Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl */
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    {
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      if (size_chpl9 < coerce_tmp_chpl4) /* ZLINE: 2481 /Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl */
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      {
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        ret_chpl2 = INT64(1);
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        goto _end__strcmp_local_chpl;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      }
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      if (size_chpl9 < coerce_tmp_chpl7) /* ZLINE: 2481 /Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl */
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      {
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        ret_chpl2 = INT64(-1);
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        goto _end__strcmp_local_chpl;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      }
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    }
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    ret_chpl2 = call_tmp_chpl15;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    _end__strcmp_local_chpl:;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    ret_chpl = ret_chpl2;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    goto _end__strcmp_chpl;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  } else /* ZLINE: 2481 /Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl */
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  {
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    call_tmp_chpl16 = get_chpl_nodeID();
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    if (coerce_tmp_chpl5 != call_tmp_chpl16) /* ZLINE: 2481 /Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl */
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    {
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      call_tmp_chpl17 = get_chpl_nodeID();
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      tmp_chpl3 = (coerce_tmp_chpl8 == call_tmp_chpl17);
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    } else /* ZLINE: 2481 /Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl */
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    {
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      tmp_chpl3 = UINT8(false);
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    }
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    if (tmp_chpl3) /* ZLINE: 2481 /Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl */
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    {
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      call_tmp_chpl18 = bufferCopyRemote_chpl(((int64_t)(coerce_tmp_chpl5)), coerce_tmp_chpl3, coerce_tmp_chpl4, _ln_chpl, _fn_chpl);
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      if (coerce_tmp_chpl4 < coerce_tmp_chpl7) /* ZLINE: 2481 /Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl */
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      {
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        tmp_chpl4 = coerce_tmp_chpl4;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      } else /* ZLINE: 2481 /Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl */
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      {
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        tmp_chpl4 = coerce_tmp_chpl7;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      }
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      size_chpl10 = tmp_chpl4;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      chpl_checkValue7(tmp_chpl4, &ret_tmp_chpl5, _ln_chpl, _fn_chpl);
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      call_tmp_chpl19 = ret_tmp_chpl5;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      i_arg_chpl3 = &call_tmp_chpl19;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      call_tmp_chpl20 = borrow_chpl2(i_arg_chpl3);
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      if (((RootClass_chpl)(call_tmp_chpl20)) != nil) /* ZLINE: 2481 /Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl */
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      {
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        coerce_tmp_chpl11 = ((Error_chpl)(call_tmp_chpl20));
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        chpl_check_nil(coerce_tmp_chpl11, _ln_chpl, _fn_chpl);
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        ((void(*)(Error_chpl,
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
           string_chpl *,
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
           int64_t,
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
           int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(coerce_tmp_chpl11))->chpl__cid) + INT64(1))])(coerce_tmp_chpl11, &ret_tmp_chpl6, _ln_chpl, _fn_chpl);
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        call_tmp_chpl21 = ret_tmp_chpl6;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        safeCastCheckHalt_chpl(&call_tmp_chpl21, _ln_chpl, _fn_chpl);
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        i_x_chpl3 = &call_tmp_chpl21;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        deinit_chpl95(i_x_chpl3, _ln_chpl, _fn_chpl);
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      }
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      chpl__autoDestroy62(&call_tmp_chpl19, _ln_chpl, _fn_chpl);
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      call_tmp_chpl22 = memcmp(((c_ptr_void_chpl)(call_tmp_chpl18)), ((c_ptr_void_chpl)(coerce_tmp_chpl6)), ((uint64_t)(size_chpl10)));
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      chpl_checkValue4(call_tmp_chpl22, &ret_tmp_chpl7, _ln_chpl, _fn_chpl);
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      call_tmp_chpl23 = ret_tmp_chpl7;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      i_arg_chpl4 = &call_tmp_chpl23;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      call_tmp_chpl24 = borrow_chpl2(i_arg_chpl4);
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      if (((RootClass_chpl)(call_tmp_chpl24)) != nil) /* ZLINE: 2481 /Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl */
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      {
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        coerce_tmp_chpl12 = ((Error_chpl)(call_tmp_chpl24));
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        chpl_check_nil(coerce_tmp_chpl12, _ln_chpl, _fn_chpl);
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        ((void(*)(Error_chpl,
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
           string_chpl *,
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
           int64_t,
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
           int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(coerce_tmp_chpl12))->chpl__cid) + INT64(1))])(coerce_tmp_chpl12, &ret_tmp_chpl8, _ln_chpl, _fn_chpl);
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        call_tmp_chpl25 = ret_tmp_chpl8;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        safeCastCheckHalt_chpl(&call_tmp_chpl25, _ln_chpl, _fn_chpl);
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        i_x_chpl4 = &call_tmp_chpl25;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        deinit_chpl95(i_x_chpl4, _ln_chpl, _fn_chpl);
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      }
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      chpl__autoDestroy62(&call_tmp_chpl23, _ln_chpl, _fn_chpl);
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      call_tmp_chpl26 = ((int64_t)(call_tmp_chpl22));
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      if (call_tmp_chpl26 == INT64(0)) /* ZLINE: 2481 /Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl */
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      {
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        if (size_chpl10 < coerce_tmp_chpl4) /* ZLINE: 2481 /Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl */
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        {
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
          ret_chpl3 = INT64(1);
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
          goto _end__strcmp_local_chpl2;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        }
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        if (size_chpl10 < coerce_tmp_chpl7) /* ZLINE: 2481 /Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl */
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        {
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
          ret_chpl3 = INT64(-1);
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
          goto _end__strcmp_local_chpl2;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        }
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      }
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      ret_chpl3 = call_tmp_chpl26;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      _end__strcmp_local_chpl2:;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      chpl_here_free(((c_ptr_void_chpl)(call_tmp_chpl18)), _ln_chpl, _fn_chpl);
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      ret_chpl = ret_chpl3;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      goto _end__strcmp_chpl;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    } else /* ZLINE: 2481 /Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl */
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    {
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      call_tmp_chpl27 = get_chpl_nodeID();
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      if (coerce_tmp_chpl5 == call_tmp_chpl27) /* ZLINE: 2481 /Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl */
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      {
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        call_tmp_chpl28 = get_chpl_nodeID();
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        tmp_chpl5 = (coerce_tmp_chpl8 != call_tmp_chpl28);
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      } else /* ZLINE: 2481 /Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl */
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      {
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        tmp_chpl5 = UINT8(false);
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      }
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      if (tmp_chpl5) /* ZLINE: 2481 /Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl */
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      {
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        call_tmp_chpl29 = bufferCopyRemote_chpl(((int64_t)(coerce_tmp_chpl8)), coerce_tmp_chpl6, coerce_tmp_chpl7, _ln_chpl, _fn_chpl);
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        if (coerce_tmp_chpl4 < coerce_tmp_chpl7) /* ZLINE: 2481 /Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl */
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        {
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
          tmp_chpl6 = coerce_tmp_chpl4;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        } else /* ZLINE: 2481 /Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl */
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        {
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
          tmp_chpl6 = coerce_tmp_chpl7;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        }
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        size_chpl11 = tmp_chpl6;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        chpl_checkValue7(tmp_chpl6, &ret_tmp_chpl9, _ln_chpl, _fn_chpl);
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        call_tmp_chpl30 = ret_tmp_chpl9;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        i_arg_chpl5 = &call_tmp_chpl30;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        call_tmp_chpl31 = borrow_chpl2(i_arg_chpl5);
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        if (((RootClass_chpl)(call_tmp_chpl31)) != nil) /* ZLINE: 2481 /Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl */
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        {
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
          coerce_tmp_chpl13 = ((Error_chpl)(call_tmp_chpl31));
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
          chpl_check_nil(coerce_tmp_chpl13, _ln_chpl, _fn_chpl);
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
          ((void(*)(Error_chpl,
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
             string_chpl *,
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
             int64_t,
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
             int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(coerce_tmp_chpl13))->chpl__cid) + INT64(1))])(coerce_tmp_chpl13, &ret_tmp_chpl10, _ln_chpl, _fn_chpl);
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
          call_tmp_chpl32 = ret_tmp_chpl10;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
          safeCastCheckHalt_chpl(&call_tmp_chpl32, _ln_chpl, _fn_chpl);
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
          i_x_chpl5 = &call_tmp_chpl32;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
          deinit_chpl95(i_x_chpl5, _ln_chpl, _fn_chpl);
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        }
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        chpl__autoDestroy62(&call_tmp_chpl30, _ln_chpl, _fn_chpl);
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        call_tmp_chpl33 = memcmp(((c_ptr_void_chpl)(coerce_tmp_chpl3)), ((c_ptr_void_chpl)(call_tmp_chpl29)), ((uint64_t)(size_chpl11)));
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        chpl_checkValue4(call_tmp_chpl33, &ret_tmp_chpl11, _ln_chpl, _fn_chpl);
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        call_tmp_chpl34 = ret_tmp_chpl11;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        i_arg_chpl6 = &call_tmp_chpl34;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        call_tmp_chpl35 = borrow_chpl2(i_arg_chpl6);
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        if (((RootClass_chpl)(call_tmp_chpl35)) != nil) /* ZLINE: 2481 /Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl */
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        {
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
          coerce_tmp_chpl14 = ((Error_chpl)(call_tmp_chpl35));
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
          chpl_check_nil(coerce_tmp_chpl14, _ln_chpl, _fn_chpl);
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
          ((void(*)(Error_chpl,
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
             string_chpl *,
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
             int64_t,
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
             int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(coerce_tmp_chpl14))->chpl__cid) + INT64(1))])(coerce_tmp_chpl14, &ret_tmp_chpl12, _ln_chpl, _fn_chpl);
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
          call_tmp_chpl36 = ret_tmp_chpl12;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
          safeCastCheckHalt_chpl(&call_tmp_chpl36, _ln_chpl, _fn_chpl);
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
          i_x_chpl6 = &call_tmp_chpl36;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
          deinit_chpl95(i_x_chpl6, _ln_chpl, _fn_chpl);
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        }
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        chpl__autoDestroy62(&call_tmp_chpl34, _ln_chpl, _fn_chpl);
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        call_tmp_chpl37 = ((int64_t)(call_tmp_chpl33));
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        if (call_tmp_chpl37 == INT64(0)) /* ZLINE: 2481 /Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl */
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        {
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
          if (size_chpl11 < coerce_tmp_chpl4) /* ZLINE: 2481 /Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl */
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
          {
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
            ret_chpl4 = INT64(1);
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
            goto _end__strcmp_local_chpl3;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
          }
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
          if (size_chpl11 < coerce_tmp_chpl7) /* ZLINE: 2481 /Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl */
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
          {
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
            ret_chpl4 = INT64(-1);
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
            goto _end__strcmp_local_chpl3;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
          }
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        }
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        ret_chpl4 = call_tmp_chpl37;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        _end__strcmp_local_chpl3:;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        chpl_here_free(((c_ptr_void_chpl)(call_tmp_chpl29)), _ln_chpl, _fn_chpl);
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        ret_chpl = ret_chpl4;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        goto _end__strcmp_chpl;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      } else /* ZLINE: 2481 /Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl */
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      {
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        call_tmp_chpl38 = bufferCopyRemote_chpl(((int64_t)(coerce_tmp_chpl5)), coerce_tmp_chpl3, coerce_tmp_chpl4, _ln_chpl, _fn_chpl);
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        call_tmp_chpl39 = bufferCopyRemote_chpl(((int64_t)(coerce_tmp_chpl8)), coerce_tmp_chpl6, coerce_tmp_chpl7, _ln_chpl, _fn_chpl);
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        if (coerce_tmp_chpl4 < coerce_tmp_chpl7) /* ZLINE: 2481 /Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl */
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        {
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
          tmp_chpl7 = coerce_tmp_chpl4;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        } else /* ZLINE: 2481 /Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl */
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        {
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
          tmp_chpl7 = coerce_tmp_chpl7;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        }
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        size_chpl12 = tmp_chpl7;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        chpl_checkValue7(tmp_chpl7, &ret_tmp_chpl13, _ln_chpl, _fn_chpl);
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        call_tmp_chpl40 = ret_tmp_chpl13;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        i_arg_chpl7 = &call_tmp_chpl40;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        call_tmp_chpl41 = borrow_chpl2(i_arg_chpl7);
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        if (((RootClass_chpl)(call_tmp_chpl41)) != nil) /* ZLINE: 2481 /Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl */
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        {
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
          coerce_tmp_chpl15 = ((Error_chpl)(call_tmp_chpl41));
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
          chpl_check_nil(coerce_tmp_chpl15, _ln_chpl, _fn_chpl);
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
          ((void(*)(Error_chpl,
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
             string_chpl *,
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
             int64_t,
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
             int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(coerce_tmp_chpl15))->chpl__cid) + INT64(1))])(coerce_tmp_chpl15, &ret_tmp_chpl14, _ln_chpl, _fn_chpl);
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
          call_tmp_chpl42 = ret_tmp_chpl14;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
          safeCastCheckHalt_chpl(&call_tmp_chpl42, _ln_chpl, _fn_chpl);
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
          i_x_chpl7 = &call_tmp_chpl42;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
          deinit_chpl95(i_x_chpl7, _ln_chpl, _fn_chpl);
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        }
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        chpl__autoDestroy62(&call_tmp_chpl40, _ln_chpl, _fn_chpl);
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        call_tmp_chpl43 = memcmp(((c_ptr_void_chpl)(call_tmp_chpl38)), ((c_ptr_void_chpl)(call_tmp_chpl39)), ((uint64_t)(size_chpl12)));
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        chpl_checkValue4(call_tmp_chpl43, &ret_tmp_chpl15, _ln_chpl, _fn_chpl);
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        call_tmp_chpl44 = ret_tmp_chpl15;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        i_arg_chpl8 = &call_tmp_chpl44;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        call_tmp_chpl45 = borrow_chpl2(i_arg_chpl8);
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        if (((RootClass_chpl)(call_tmp_chpl45)) != nil) /* ZLINE: 2481 /Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl */
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        {
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
          coerce_tmp_chpl16 = ((Error_chpl)(call_tmp_chpl45));
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
          chpl_check_nil(coerce_tmp_chpl16, _ln_chpl, _fn_chpl);
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
          ((void(*)(Error_chpl,
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
             string_chpl *,
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
             int64_t,
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
             int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(coerce_tmp_chpl16))->chpl__cid) + INT64(1))])(coerce_tmp_chpl16, &ret_tmp_chpl16, _ln_chpl, _fn_chpl);
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
          call_tmp_chpl46 = ret_tmp_chpl16;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
          safeCastCheckHalt_chpl(&call_tmp_chpl46, _ln_chpl, _fn_chpl);
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
          i_x_chpl8 = &call_tmp_chpl46;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
          deinit_chpl95(i_x_chpl8, _ln_chpl, _fn_chpl);
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        }
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        chpl__autoDestroy62(&call_tmp_chpl44, _ln_chpl, _fn_chpl);
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        call_tmp_chpl47 = ((int64_t)(call_tmp_chpl43));
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        if (call_tmp_chpl47 == INT64(0)) /* ZLINE: 2481 /Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl */
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        {
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
          if (size_chpl12 < coerce_tmp_chpl4) /* ZLINE: 2481 /Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl */
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
          {
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
            ret_chpl5 = INT64(1);
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
            goto _end__strcmp_local_chpl4;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
          }
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
          if (size_chpl12 < coerce_tmp_chpl7) /* ZLINE: 2481 /Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl */
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
          {
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
            ret_chpl5 = INT64(-1);
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
            goto _end__strcmp_local_chpl4;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
          }
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        }
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        ret_chpl5 = call_tmp_chpl47;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        _end__strcmp_local_chpl4:;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        chpl_here_free(((c_ptr_void_chpl)(call_tmp_chpl38)), _ln_chpl, _fn_chpl);
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        chpl_here_free(((c_ptr_void_chpl)(call_tmp_chpl39)), _ln_chpl, _fn_chpl);
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        ret_chpl = ret_chpl5;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
        goto _end__strcmp_chpl;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
      }
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    }
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  }
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _end__strcmp_chpl:;
#line 2481 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  call_tmp_chpl48 = (ret_chpl == INT64(0));
#line 2466 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  return call_tmp_chpl48;
#line 2466 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
}

