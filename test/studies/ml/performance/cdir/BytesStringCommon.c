#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
/* BytesStringCommon.chpl:21 */
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
static void chpl__init_BytesStringCommon(int64_t _ln_chpl,
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                                         int32_t _fn_chpl) {
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref_int32_t_chpl refIndentLevel_chpl = NULL;
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  if (chpl__init_BytesStringCommon_p) /* ZLINE: 21 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  {
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    goto _exit_chpl__init_BytesStringCommon_chpl;
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  }
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  printModuleInit_chpl("%*s\n", "BytesStringCommon", INT64(17), _ln_chpl, _fn_chpl);
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  refIndentLevel_chpl = &moduleInitLevel_chpl;
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  *(refIndentLevel_chpl) += INT64(1);
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl__init_BytesStringCommon_p = UINT8(true);
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  {
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    chpl__init_CTypes(_ln_chpl, _fn_chpl);
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    chpl__init_ByteBufferHelpers(_ln_chpl, _fn_chpl);
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    chpl__init_NVStringFactory(_ln_chpl, _fn_chpl);
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    chpl__init_OS(_ln_chpl, _fn_chpl);
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    chpl__init_Bytes(_ln_chpl, _fn_chpl);
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    chpl__init_String(_ln_chpl, _fn_chpl);
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  }
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  *(refIndentLevel_chpl) -= INT64(1);
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _exit_chpl__init_BytesStringCommon_chpl:;
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  return;
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
}

#line 109 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
/* BytesStringCommon.chpl:109 */
#line 109 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
static void decodeByteBuffer_chpl(uint8_t * buff_chpl,
#line 109 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                                  int64_t length_chpl,
#line 109 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                                  decodePolicy_chpl policy_chpl,
#line 109 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                                  Error_chpl * error_out_chpl,
#line 109 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                                  string_chpl * _retArg_chpl,
#line 109 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                                  int64_t _ln_chpl,
#line 109 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                                  int32_t _fn_chpl) {
#line 109 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl ret_chpl;
#line 117 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl inlineImm_chpl;
#line 117 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 117 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl ret_chpl2;
#line 117 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl tmp_chpl;
#line 123 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t call_tmp_chpl6;
#line 123 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t tmp_chpl2;
#line 123 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int16_t call_tmp_chpl7;
#line 123 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  c_ptr_void_chpl cast_tmp_chpl = NULL;
#line 123 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  c_ptr_uint8_t_chpl newBuff_chpl = NULL;
#line 123 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t allocSize_chpl;
#line 124 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl_bool hasEscapes_chpl;
#line 125 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t numCodepoints_chpl;
#line 127 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t expectedSize_chpl;
#line 129 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t thisIdx_chpl;
#line 130 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t decodedIdx_chpl;
#line 131 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl_bool tmp_chpl3;
#line 132 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  syserr call_tmp_x0_chpl;
#line 132 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int32_t call_tmp_x2_chpl;
#line 132 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _tuple_3_syserr_int32_t_int32_t_chpl ret_tmp_chpl;
#line 135 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  c_ptr_uint8_t_chpl call_tmp_chpl8 = NULL;
#line 137 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref_int64_t_chpl i_lhs_chpl = NULL;
#line 139 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int32_t call_tmp_chpl9;
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _owned_DecodeError_chpl call_tmp_chpl10;
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  DecodeError_chpl new_temp_chpl = NULL;
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _owned_DecodeError_chpl initTemp_chpl;
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  Error_chpl call_tmp_chpl11 = NULL;
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _owned_Error_chpl coerce_tmp_chpl3;
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _owned_Error_chpl ret_tmp_chpl2;
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref__owned_DecodeError_chpl i_x_chpl2 = NULL;
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref__owned_Error_chpl2 i__retArg_chpl = NULL;
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _owned_DecodeError_chpl _formal_tmp_x_chpl;
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _owned_Error_chpl ret_chpl3;
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref__owned_DecodeError_chpl i_this_chpl = NULL;
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  Error_chpl call_tmp_chpl12 = NULL;
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref__owned_DecodeError_chpl i_this_chpl2 = NULL;
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _owned_Error_chpl call_tmp_chpl13;
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _owned_Error_chpl initTemp_chpl2;
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _owned_Error_chpl _formal_tmp_in_err_chpl;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int32_t nInvalidBytes_chpl;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int32_t tmp_chpl4;
#line 152 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref_int64_t_chpl i_lhs_chpl2 = NULL;
#line 162 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref_int64_t_chpl i_lhs_chpl3 = NULL;
#line 163 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  c_ptr_uint8_t_chpl tmp_x0_chpl = NULL;
#line 163 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t tmp_x1_chpl;
#line 163 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _tuple_2_c_ptr_uint8_t_int64_t_chpl ret_tmp_chpl3;
#line 168 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref_int64_t_chpl i_lhs_chpl4 = NULL;
#line 175 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref_int64_t_chpl i_lhs_chpl5 = NULL;
#line 176 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t tmp_x1_chpl2;
#line 176 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _tuple_2_c_ptr_uint8_t_int64_t_chpl ret_tmp_chpl4;
#line 178 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t tmp_chpl5;
#line 178 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t _ic__F1_high_chpl;
#line 178 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t i_chpl;
#line 180 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t call_tmp_chpl14;
#line 180 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref_uint8_t_chpl call_tmp_chpl15 = NULL;
#line 181 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref_int64_t_chpl i_lhs_chpl6 = NULL;
#line 187 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref_int64_t_chpl i_lhs_chpl7 = NULL;
#line 196 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t coerce_tmp_chpl4;
#line 196 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  c_ptr_uint8_t_chpl call_tmp_chpl16 = NULL;
#line 196 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  c_ptr_uint8_t_chpl call_tmp_chpl17 = NULL;
#line 196 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _owned_IllegalArgumentError_chpl2 call_tmp_chpl18;
#line 196 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _owned_IllegalArgumentError_chpl2 ret_tmp_chpl5;
#line 196 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref__owned_IllegalArgumentError_chpl i_arg_chpl = NULL;
#line 196 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  IllegalArgumentError_chpl call_tmp_chpl19 = NULL;
#line 196 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl call_tmp_chpl20;
#line 196 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  Error_chpl coerce_tmp_chpl5 = NULL;
#line 196 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl ret_tmp_chpl6;
#line 196 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref_string_chpl i_x_chpl3 = NULL;
#line 198 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref_int64_t_chpl i_lhs_chpl8 = NULL;
#line 199 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref_int64_t_chpl i_lhs_chpl9 = NULL;
#line 203 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref_uint8_t_chpl call_tmp_chpl21 = NULL;
#line 204 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl ret_chpl4;
#line 204 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl ret_chpl5;
#line 204 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref_string_chpl i_x_chpl4 = NULL;
#line 204 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl ret_chpl6;
#line 117 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  if (length_chpl == INT64(0)) /* ZLINE: 117 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 117 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  {
#line 117 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    inlineImm_chpl = _str_literal_163_chpl;
#line 117 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    i_x_chpl = &inlineImm_chpl;
#line 117 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    init_ASSIGN__chpl25(&tmp_chpl, i_x_chpl, _ln_chpl, _fn_chpl);
#line 117 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    ret_chpl2 = tmp_chpl;
#line 117 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    ret_chpl = ret_chpl2;
#line 117 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    goto _end_decodeByteBuffer_chpl;
#line 117 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  }
#line 123 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl6 = chpl_here_good_alloc_size(((int64_t)((length_chpl + INT64(1)))), _ln_chpl, _fn_chpl);
#line 123 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  if (call_tmp_chpl6 > INT64(0)) /* ZLINE: 123 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 123 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  {
#line 123 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    tmp_chpl2 = call_tmp_chpl6;
#line 123 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  } else /* ZLINE: 123 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 123 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  {
#line 123 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    tmp_chpl2 = INT64(0);
#line 123 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  }
#line 123 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl7 = offset_STR_COPY_DATA_chpl();
#line 123 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  cast_tmp_chpl = chpl_here_alloc(tmp_chpl2, call_tmp_chpl7, _ln_chpl, _fn_chpl);
#line 123 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  newBuff_chpl = ((c_ptr_uint8_t_chpl)(cast_tmp_chpl));
#line 123 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  allocSize_chpl = tmp_chpl2;
#line 124 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  hasEscapes_chpl = UINT8(false);
#line 125 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  numCodepoints_chpl = INT64(0);
#line 127 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  expectedSize_chpl = tmp_chpl2;
#line 129 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  thisIdx_chpl = INT64(0);
#line 130 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  decodedIdx_chpl = INT64(0);
#line 131 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  tmp_chpl3 = (INT64(0) < length_chpl);
#line 131 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  while (tmp_chpl3) {
#line 132 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    decodeHelp_chpl(buff_chpl, length_chpl, thisIdx_chpl, UINT8(false), &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 132 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_x0_chpl = (&ret_tmp_chpl)->x0;
#line 132 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_x2_chpl = (&ret_tmp_chpl)->x2;
#line 135 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl8 = (buff_chpl + thisIdx_chpl);
#line 137 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    i_lhs_chpl = &numCodepoints_chpl;
#line 137 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    *(i_lhs_chpl) += INT64(1);
#line 139 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl9 = qio_err_to_int(call_tmp_x0_chpl);
#line 139 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    if (! (call_tmp_chpl9 == ((int32_t)(INT64(0))))) /* ZLINE: 139 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 139 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    {
#line 140 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      if (policy_chpl == decodePolicy_chpl_strict_chpl) /* ZLINE: 140 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 140 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      {
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        chpl_here_free(((c_ptr_void_chpl)(newBuff_chpl)), _ln_chpl, _fn_chpl);
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        new_temp_chpl = _new_chpl58(_ln_chpl, _fn_chpl);
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        init_chpl183(&initTemp_chpl, new_temp_chpl);
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        call_tmp_chpl10 = initTemp_chpl;
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        i_x_chpl2 = &call_tmp_chpl10;
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        i__retArg_chpl = &ret_tmp_chpl2;
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        _formal_tmp_x_chpl = *(i_x_chpl2);
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        i_this_chpl = &_formal_tmp_x_chpl;
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        call_tmp_chpl12 = ((Error_chpl)((i_this_chpl)->chpl_p));
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        i_this_chpl2 = &_formal_tmp_x_chpl;
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        (i_this_chpl2)->chpl_p = ((DecodeError_chpl)(nil));
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        if (((RootClass_chpl)(call_tmp_chpl12)) == nil) /* ZLINE: 142 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        {
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          nilCheckHalt_chpl(&_str_literal_52_chpl, _ln_chpl, _fn_chpl);
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        }
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        init_chpl188(&initTemp_chpl2, call_tmp_chpl12);
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        call_tmp_chpl13 = initTemp_chpl2;
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        ret_chpl3 = call_tmp_chpl13;
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        chpl__autoDestroy55(&_formal_tmp_x_chpl, _ln_chpl, _fn_chpl);
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        *(i__retArg_chpl) = ret_chpl3;
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        coerce_tmp_chpl3 = ret_tmp_chpl2;
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        _formal_tmp_in_err_chpl = coerce_tmp_chpl3;
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        call_tmp_chpl11 = chpl_fix_thrown_error(&_formal_tmp_in_err_chpl, _ln_chpl, _fn_chpl);
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        *(error_out_chpl) = call_tmp_chpl11;
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        goto _end_decodeByteBuffer_chpl;
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      } else /* ZLINE: 144 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      {
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        if (call_tmp_x2_chpl == ((int32_t)(INT64(1)))) /* ZLINE: 151 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        {
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          tmp_chpl4 = call_tmp_x2_chpl;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        } else /* ZLINE: 151 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        {
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          tmp_chpl4 = ((int32_t)((call_tmp_x2_chpl - ((int32_t)(INT64(1))))));
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        }
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        nInvalidBytes_chpl = tmp_chpl4;
#line 152 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        i_lhs_chpl2 = &thisIdx_chpl;
#line 152 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        *(i_lhs_chpl2) += ((int64_t)(tmp_chpl4));
#line 154 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        if (policy_chpl == decodePolicy_chpl_replace_chpl) /* ZLINE: 154 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 154 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        {
#line 162 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          i_lhs_chpl3 = &expectedSize_chpl;
#line 162 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          *(i_lhs_chpl3) += ((int64_t)(((int32_t)((((int32_t)(INT64(3))) - tmp_chpl4)))));
#line 163 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          bufferEnsureSize_chpl(newBuff_chpl, allocSize_chpl, expectedSize_chpl, &ret_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 163 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          tmp_x0_chpl = (&ret_tmp_chpl3)->x0;
#line 163 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          tmp_x1_chpl = (&ret_tmp_chpl3)->x1;
#line 163 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          newBuff_chpl = tmp_x0_chpl;
#line 163 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          allocSize_chpl = tmp_x1_chpl;
#line 166 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          qio_encode_char_buf(((c_ptr_void_chpl)((tmp_x0_chpl + decodedIdx_chpl))), INT32(65533));
#line 168 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          i_lhs_chpl4 = &decodedIdx_chpl;
#line 168 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          *(i_lhs_chpl4) += INT64(3);
#line 168 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        } else /* ZLINE: 170 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 168 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        if (policy_chpl == decodePolicy_chpl_escape_chpl) /* ZLINE: 170 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 168 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        {
#line 172 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          hasEscapes_chpl = UINT8(true);
#line 175 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          i_lhs_chpl5 = &expectedSize_chpl;
#line 175 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          *(i_lhs_chpl5) += ((int64_t)(((int32_t)((((int32_t)(INT64(2))) * tmp_chpl4)))));
#line 176 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          bufferEnsureSize_chpl(newBuff_chpl, allocSize_chpl, expectedSize_chpl, &ret_tmp_chpl4, _ln_chpl, _fn_chpl);
#line 176 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          tmp_x1_chpl2 = (&ret_tmp_chpl4)->x1;
#line 176 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          newBuff_chpl = (&ret_tmp_chpl4)->x0;
#line 176 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          allocSize_chpl = tmp_x1_chpl2;
#line 178 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          if (tmp_chpl4 < INT32(0)) /* ZLINE: 178 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 178 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          {
#line 178 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
            boundsCheckHalt_chpl(&_str_literal_580_chpl, _ln_chpl, _fn_chpl);
#line 178 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          }
#line 178 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          if (tmp_chpl4 == ((int32_t)(INT64(0)))) /* ZLINE: 178 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 178 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          {
#line 178 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
            tmp_chpl5 = ((int64_t)(((uint64_t)((((uint64_t)(INT64(0))) - UINT64(1))))));
#line 178 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          } else /* ZLINE: 178 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 178 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          {
#line 178 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
            tmp_chpl5 = ((int64_t)((INT64(0) + ((int64_t)((((int64_t)(tmp_chpl4)) - INT64(1)))))));
#line 178 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          }
#line 178 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          _ic__F1_high_chpl = tmp_chpl5;
#line 178 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          i_chpl = INT64(0);
#line 178 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          chpl_checkIfRangeIterWillOverflow(INT64(0), tmp_chpl5, INT64(1), INT64(0), tmp_chpl5, UINT8(true), _ln_chpl, _fn_chpl);
#line 178 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          for (i_chpl = INT64(0); ((i_chpl <= _ic__F1_high_chpl)); i_chpl += INT64(1)) {
#line 180 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
            call_tmp_chpl14 = (((int64_t)((thisIdx_chpl - ((int64_t)(nInvalidBytes_chpl))))) + i_chpl);
#line 180 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
            call_tmp_chpl15 = (buff_chpl + call_tmp_chpl14);
#line 179 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
            qio_encode_char_buf(((c_ptr_void_chpl)((newBuff_chpl + decodedIdx_chpl))), ((int32_t)((((int32_t)(INT64(56320))) + ((int32_t)(*(call_tmp_chpl15)))))));
#line 181 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
            i_lhs_chpl6 = &decodedIdx_chpl;
#line 181 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
            *(i_lhs_chpl6) += INT64(3);
#line 181 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          }
#line 187 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          i_lhs_chpl7 = &numCodepoints_chpl;
#line 187 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          *(i_lhs_chpl7) += ((int64_t)(((int32_t)((nInvalidBytes_chpl - ((int32_t)(INT64(1))))))));
#line 187 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        }
#line 187 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      }
#line 187 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    } else /* ZLINE: 194 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 187 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    {
#line 196 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      coerce_tmp_chpl4 = ((int64_t)(call_tmp_x2_chpl));
#line 196 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      call_tmp_chpl16 = (newBuff_chpl + decodedIdx_chpl);
#line 196 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      call_tmp_chpl17 = (call_tmp_chpl8 + INT64(0));
#line 196 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      chpl_checkValue7(coerce_tmp_chpl4, &ret_tmp_chpl5, _ln_chpl, _fn_chpl);
#line 196 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      call_tmp_chpl18 = ret_tmp_chpl5;
#line 196 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      i_arg_chpl = &call_tmp_chpl18;
#line 196 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      call_tmp_chpl19 = borrow_chpl2(i_arg_chpl);
#line 196 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      if (((RootClass_chpl)(call_tmp_chpl19)) != nil) /* ZLINE: 196 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 196 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      {
#line 196 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        coerce_tmp_chpl5 = ((Error_chpl)(call_tmp_chpl19));
#line 196 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        chpl_check_nil(coerce_tmp_chpl5, _ln_chpl, _fn_chpl);
#line 196 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        ((void(*)(Error_chpl,
#line 196 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
           string_chpl *,
#line 196 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
           int64_t,
#line 196 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
           int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(coerce_tmp_chpl5))->chpl__cid) + INT64(1))])(coerce_tmp_chpl5, &ret_tmp_chpl6, _ln_chpl, _fn_chpl);
#line 196 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        call_tmp_chpl20 = ret_tmp_chpl6;
#line 196 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        safeCastCheckHalt_chpl(&call_tmp_chpl20, _ln_chpl, _fn_chpl);
#line 196 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        i_x_chpl3 = &call_tmp_chpl20;
#line 196 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        deinit_chpl95(i_x_chpl3, _ln_chpl, _fn_chpl);
#line 196 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      }
#line 196 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      chpl__autoDestroy62(&call_tmp_chpl18, _ln_chpl, _fn_chpl);
#line 196 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      memcpy(((c_ptr_void_chpl)(call_tmp_chpl16)), ((c_ptr_void_chpl)(call_tmp_chpl17)), ((uint64_t)(coerce_tmp_chpl4)));
#line 198 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      i_lhs_chpl8 = &thisIdx_chpl;
#line 198 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      *(i_lhs_chpl8) += ((int64_t)(call_tmp_x2_chpl));
#line 199 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      i_lhs_chpl9 = &decodedIdx_chpl;
#line 199 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      *(i_lhs_chpl9) += ((int64_t)(call_tmp_x2_chpl));
#line 199 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    }
#line 131 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    tmp_chpl3 = (thisIdx_chpl < length_chpl);
#line 131 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  }
#line 203 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl21 = (newBuff_chpl + decodedIdx_chpl);
#line 203 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  *(call_tmp_chpl21) = ((uint8_t)(INT64(0)));
#line 204 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  init_chpl194(&ret_chpl5, _ln_chpl, _fn_chpl);
#line 204 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  i_x_chpl4 = &ret_chpl5;
#line 204 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  (i_x_chpl4)->isOwned = UINT8(true);
#line 204 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  (i_x_chpl4)->buff = newBuff_chpl;
#line 204 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  (i_x_chpl4)->buffSize = allocSize_chpl;
#line 204 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  (i_x_chpl4)->buffLen = decodedIdx_chpl;
#line 204 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  (&ret_chpl5)->cachedNumCodepoints = numCodepoints_chpl;
#line 204 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  ret_chpl4 = ret_chpl5;
#line 204 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  ret_chpl6 = ret_chpl4;
#line 208 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  (&ret_chpl6)->hasEscapes = hasEscapes_chpl;
#line 209 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  ret_chpl = ret_chpl6;
#line 209 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _end_decodeByteBuffer_chpl:;
#line 109 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  *(_retArg_chpl) = ret_chpl;
#line 109 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  return;
#line 109 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
}

#line 232 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
/* BytesStringCommon.chpl:232 */
#line 232 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
static void decodeHelp_chpl(uint8_t * buff_chpl,
#line 232 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                            int64_t buffLen_chpl,
#line 232 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                            int64_t offset_chpl,
#line 232 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                            chpl_bool allowEsc_chpl,
#line 232 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                            _tuple_3_syserr_int32_t_int32_t_chpl * _retArg_chpl,
#line 232 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                            int64_t _ln_chpl,
#line 232 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                            int32_t _fn_chpl) {
#line 247 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int32_t chr_chpl;
#line 248 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int32_t nBytes_chpl;
#line 249 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int32_t call_tmp_chpl6;
#line 250 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  c_ptr_uint8_t_chpl call_tmp_chpl7 = NULL;
#line 251 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t call_tmp_chpl8;
#line 252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  syserr decodeRet_chpl;
#line 254 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  syserr call_tmp_chpl9;
#line 258 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  syserr call_tmp_chpl10;
#line 262 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _tuple_3_syserr_int32_t_int32_t_chpl ret_tmp_chpl;
#line 262 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int32_t this_x1_chpl;
#line 262 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int32_t this_x2_chpl;
#line 247 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chr_chpl = INT32(0);
#line 248 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  nBytes_chpl = INT32(0);
#line 249 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl6 = ((int32_t)(offset_chpl));
#line 250 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl7 = (buff_chpl + call_tmp_chpl6);
#line 251 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl8 = (buffLen_chpl - ((int64_t)(call_tmp_chpl6)));
#line 253 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  if (allowEsc_chpl) /* ZLINE: 253 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 253 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  {
#line 254 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl9 = qio_decode_char_buf_esc(&chr_chpl, &nBytes_chpl, call_tmp_chpl7, call_tmp_chpl8);
#line 254 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    decodeRet_chpl = call_tmp_chpl9;
#line 254 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  } else /* ZLINE: 253 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 254 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  {
#line 258 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl10 = qio_decode_char_buf(&chr_chpl, &nBytes_chpl, call_tmp_chpl7, call_tmp_chpl8);
#line 258 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    decodeRet_chpl = call_tmp_chpl10;
#line 258 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  }
#line 262 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  this_x1_chpl = chr_chpl;
#line 262 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  this_x2_chpl = nBytes_chpl;
#line 262 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  (&ret_tmp_chpl)->x0 = decodeRet_chpl;
#line 262 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  (&ret_tmp_chpl)->x1 = this_x1_chpl;
#line 262 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  (&ret_tmp_chpl)->x2 = this_x2_chpl;
#line 232 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  *(_retArg_chpl) = ret_tmp_chpl;
#line 232 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  return;
#line 232 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
}

#line 265 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
/* BytesStringCommon.chpl:265 */
#line 265 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
static void initWithBorrowedBuffer_chpl(string_chpl * x_chpl,
#line 265 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                                        string_chpl * other_chpl,
#line 265 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                                        int64_t _ln_chpl,
#line 265 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                                        int32_t _fn_chpl) {
#line 271 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int32_t call_tmp_chpl6;
#line 271 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl_bool call_tmp_chpl7;
#line 272 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t ret_chpl;
#line 279 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  c_ptr_uint8_t_chpl call_tmp_chpl8 = NULL;
#line 268 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  (x_chpl)->isOwned = UINT8(false);
#line 269 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  (x_chpl)->hasEscapes = (other_chpl)->hasEscapes;
#line 271 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl6 = get_chpl_nodeID();
#line 271 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl7 = ((other_chpl)->locale_id != call_tmp_chpl6);
#line 272 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  ret_chpl = (other_chpl)->buffLen;
#line 274 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  if (ret_chpl > INT64(0)) /* ZLINE: 274 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 274 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  {
#line 275 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    (x_chpl)->buffLen = ret_chpl;
#line 276 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    if (call_tmp_chpl7) /* ZLINE: 276 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 276 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    {
#line 278 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      (x_chpl)->isOwned = UINT8(true);
#line 279 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      call_tmp_chpl8 = bufferCopyRemote_chpl(((int64_t)((other_chpl)->locale_id)), (other_chpl)->buff, ret_chpl, _ln_chpl, _fn_chpl);
#line 279 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      (x_chpl)->buff = call_tmp_chpl8;
#line 280 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      (x_chpl)->buffSize = ((int64_t)((ret_chpl + INT64(1))));
#line 281 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      (x_chpl)->cachedNumCodepoints = (other_chpl)->cachedNumCodepoints;
#line 281 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    } else /* ZLINE: 283 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 281 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    {
#line 285 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      (x_chpl)->buff = (other_chpl)->buff;
#line 286 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      (x_chpl)->buffSize = (other_chpl)->buffSize;
#line 287 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      (x_chpl)->cachedNumCodepoints = (other_chpl)->cachedNumCodepoints;
#line 287 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    }
#line 287 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  }
#line 265 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  return;
#line 265 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
}

#line 292 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
/* BytesStringCommon.chpl:292 */
#line 292 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
static void initWithBorrowedBuffer_chpl3(bytes_chpl * x_chpl,
#line 292 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                                         uint8_t * other_chpl,
#line 292 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                                         int64_t length_chpl,
#line 292 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                                         int64_t size_chpl9) {
#line 295 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  (x_chpl)->isOwned = UINT8(false);
#line 299 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  (x_chpl)->buff = other_chpl;
#line 300 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  (x_chpl)->buffSize = size_chpl9;
#line 301 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  (x_chpl)->buffLen = length_chpl;
#line 292 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  return;
#line 292 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
}

#line 292 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
/* BytesStringCommon.chpl:292 */
#line 292 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
static void initWithBorrowedBuffer_chpl2(string_chpl * x_chpl,
#line 292 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                                         uint8_t * other_chpl,
#line 292 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                                         int64_t length_chpl,
#line 292 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                                         int64_t size_chpl9) {
#line 295 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  (x_chpl)->isOwned = UINT8(false);
#line 299 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  (x_chpl)->buff = other_chpl;
#line 300 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  (x_chpl)->buffSize = size_chpl9;
#line 301 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  (x_chpl)->buffLen = length_chpl;
#line 292 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  return;
#line 292 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
}

#line 342 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
/* BytesStringCommon.chpl:342 */
#line 342 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
static void initWithNewBuffer_chpl(bytes_chpl * x_chpl,
#line 342 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                                   uint8_t * other_chpl,
#line 342 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                                   int64_t length_chpl,
#line 342 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                                   int64_t size_chpl9,
#line 342 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                                   int64_t _ln_chpl,
#line 342 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                                   int32_t _fn_chpl) {
#line 345 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t otherLen_chpl;
#line 350 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _tuple_2_c_ptr_uint8_t_int64_t_chpl call_tmp_chpl6;
#line 350 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _tuple_2_c_ptr_uint8_t_int64_t_chpl ret_chpl;
#line 350 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t call_tmp_chpl7;
#line 350 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t tmp_chpl;
#line 350 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int16_t call_tmp_chpl8;
#line 350 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  c_ptr_void_chpl cast_tmp_chpl = NULL;
#line 350 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  c_ptr_uint8_t_chpl call_tmp_chpl9 = NULL;
#line 350 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  c_ptr_uint8_t_chpl dst_chpl = NULL;
#line 350 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  c_ptr_uint8_t_chpl call_tmp_chpl10 = NULL;
#line 350 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  c_ptr_uint8_t_chpl call_tmp_chpl11 = NULL;
#line 350 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _owned_IllegalArgumentError_chpl2 call_tmp_chpl12;
#line 350 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _owned_IllegalArgumentError_chpl2 ret_tmp_chpl;
#line 350 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref__owned_IllegalArgumentError_chpl i_arg_chpl = NULL;
#line 350 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  IllegalArgumentError_chpl call_tmp_chpl13 = NULL;
#line 350 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl call_tmp_chpl14;
#line 350 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  Error_chpl coerce_tmp_chpl3 = NULL;
#line 350 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl ret_tmp_chpl2;
#line 350 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 350 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref_uint8_t_chpl call_tmp_chpl15 = NULL;
#line 350 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t tmp_x1_chpl;
#line 353 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  c_ptr_uint8_t_chpl coerce_tmp_chpl4 = NULL;
#line 353 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t coerce_tmp_chpl5;
#line 353 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref_uint8_t_chpl call_tmp_chpl16 = NULL;
#line 345 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  otherLen_chpl = length_chpl;
#line 346 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  (x_chpl)->isOwned = UINT8(true);
#line 348 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  if (length_chpl > INT64(0)) /* ZLINE: 348 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 348 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  {
#line 350 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl7 = chpl_here_good_alloc_size(((int64_t)((length_chpl + INT64(1)))), _ln_chpl, _fn_chpl);
#line 350 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    if (call_tmp_chpl7 > INT64(0)) /* ZLINE: 350 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 350 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    {
#line 350 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      tmp_chpl = call_tmp_chpl7;
#line 350 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    } else /* ZLINE: 350 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 350 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    {
#line 350 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      tmp_chpl = INT64(0);
#line 350 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    }
#line 350 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl8 = offset_STR_COPY_DATA_chpl();
#line 350 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    cast_tmp_chpl = chpl_here_alloc(tmp_chpl, call_tmp_chpl8, _ln_chpl, _fn_chpl);
#line 350 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl9 = ((c_ptr_uint8_t_chpl)(cast_tmp_chpl));
#line 350 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    dst_chpl = call_tmp_chpl9;
#line 350 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl10 = (call_tmp_chpl9 + INT64(0));
#line 350 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl11 = (other_chpl + INT64(0));
#line 350 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    chpl_checkValue7(length_chpl, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 350 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl12 = ret_tmp_chpl;
#line 350 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    i_arg_chpl = &call_tmp_chpl12;
#line 350 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl13 = borrow_chpl2(i_arg_chpl);
#line 350 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    if (((RootClass_chpl)(call_tmp_chpl13)) != nil) /* ZLINE: 350 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 350 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    {
#line 350 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      coerce_tmp_chpl3 = ((Error_chpl)(call_tmp_chpl13));
#line 350 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      chpl_check_nil(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 350 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      ((void(*)(Error_chpl,
#line 350 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
         string_chpl *,
#line 350 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
         int64_t,
#line 350 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
         int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(coerce_tmp_chpl3))->chpl__cid) + INT64(1))])(coerce_tmp_chpl3, &ret_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 350 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      call_tmp_chpl14 = ret_tmp_chpl2;
#line 350 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      safeCastCheckHalt_chpl(&call_tmp_chpl14, _ln_chpl, _fn_chpl);
#line 350 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      i_x_chpl = &call_tmp_chpl14;
#line 350 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      deinit_chpl95(i_x_chpl, _ln_chpl, _fn_chpl);
#line 350 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    }
#line 350 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    chpl__autoDestroy62(&call_tmp_chpl12, _ln_chpl, _fn_chpl);
#line 350 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    memcpy(((c_ptr_void_chpl)(call_tmp_chpl10)), ((c_ptr_void_chpl)(call_tmp_chpl11)), ((uint64_t)(otherLen_chpl)));
#line 350 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl15 = (dst_chpl + otherLen_chpl);
#line 350 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    *(call_tmp_chpl15) = ((uint8_t)(INT64(0)));
#line 350 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    (&ret_chpl)->x0 = call_tmp_chpl9;
#line 350 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    (&ret_chpl)->x1 = tmp_chpl;
#line 350 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl6 = ret_chpl;
#line 350 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    tmp_x1_chpl = (&call_tmp_chpl6)->x1;
#line 351 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    (x_chpl)->buff = (&call_tmp_chpl6)->x0;
#line 352 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    (x_chpl)->buffLen = otherLen_chpl;
#line 353 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    coerce_tmp_chpl4 = (x_chpl)->buff;
#line 353 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    coerce_tmp_chpl5 = (x_chpl)->buffLen;
#line 353 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl16 = (coerce_tmp_chpl4 + coerce_tmp_chpl5);
#line 353 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    *(call_tmp_chpl16) = ((uint8_t)(INT64(0)));
#line 354 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    (x_chpl)->buffSize = tmp_x1_chpl;
#line 354 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  }
#line 342 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  return;
#line 342 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
}

#line 367 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
/* BytesStringCommon.chpl:367 */
#line 367 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
static void getView_chpl6(string_chpl * x_chpl,
#line 367 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                          range_int64_t_both_one_chpl r_chpl,
#line 367 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                          Error_chpl * error_out_chpl,
#line 367 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                          _tuple_2_range_int64_t_both_one_int64_t_chpl * _retArg_chpl,
#line 367 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                          int64_t _ln_chpl,
#line 367 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                          int32_t _fn_chpl) {
#line 367 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _tuple_2_range_int64_t_both_one_int64_t_chpl ret_chpl;
#line 415 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t ret_chpl2;
#line 415 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t ret_chpl3;
#line 416 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _tuple_2_range_int64_t_both_one_int64_t_chpl call_tmp_chpl6;
#line 416 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _tuple_2_range_int64_t_both_one_int64_t_chpl ret_tmp_chpl;
#line 428 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t ret_chpl4;
#line 428 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t call_tmp_chpl7;
#line 428 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl8;
#line 428 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl2;
#line 428 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl_bool call_tmp_chpl9;
#line 428 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t ret_chpl5;
#line 428 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t call_tmp_chpl10;
#line 428 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl11;
#line 428 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl3;
#line 428 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t coerce_tmp_chpl3;
#line 428 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _tuple_2_range_int64_t_both_one_int64_t_chpl ret_tmp_chpl4;
#line 432 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl intR_chpl;
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl12;
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t ret_chpl6;
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t call_tmp_chpl13;
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl14;
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl5;
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl boundedOther_chpl;
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl tmp_chpl;
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl initTemp_chpl;
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  uint64_t call_tmp_chpl15;
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl_bool tmp_chpl2;
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl_bool tmp_chpl3;
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl_bool ret_chpl7;
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  uint64_t call_tmp_chpl16;
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl_bool tmp_chpl4;
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  uint64_t call_tmp_chpl17;
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl_bool tmp_chpl5;
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl18;
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl6;
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl slice_chpl;
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl19;
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl7;
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl_bool call_tmp_chpl20;
#line 435 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t ret_chpl8;
#line 440 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t cpCount_chpl;
#line 441 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t cpIdxLow_chpl;
#line 441 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t tmp_chpl6;
#line 444 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t cpIdxHigh_chpl;
#line 445 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t ret_chpl9;
#line 448 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t byteLow_chpl;
#line 449 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t byteHigh_chpl;
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  byteIndex_chpl default_arg_start_chpl;
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  byteIndex_chpl ret_chpl10;
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  byteIndex_chpl cpi_chpl;
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl ret_chpl11;
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl ret_chpl12;
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl localThis_chpl;
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t ret_chpl13;
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t i_chpl;
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl_bool tmp_chpl7;
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl_bool tmp_chpl8;
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  c_ptr_uint8_t_chpl coerce_tmp_chpl4 = NULL;
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref_uint8_t_chpl call_tmp_chpl21 = NULL;
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref_int64_t_chpl i_lhs_chpl = NULL;
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl_bool tmp_chpl9;
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  c_ptr_uint8_t_chpl coerce_tmp_chpl5 = NULL;
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref_uint8_t_chpl call_tmp_chpl22 = NULL;
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl_bool tmp_chpl10;
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t call_tmp_chpl23;
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t j_chpl;
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl_bool tmp_chpl11;
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl_bool tmp_chpl12;
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  c_ptr_uint8_t_chpl coerce_tmp_chpl6 = NULL;
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref_uint8_t_chpl call_tmp_chpl24 = NULL;
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref_int64_t_chpl i_lhs_chpl2 = NULL;
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl_bool tmp_chpl13;
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  c_ptr_uint8_t_chpl coerce_tmp_chpl7 = NULL;
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref_uint8_t_chpl call_tmp_chpl25 = NULL;
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  byteIndex_chpl ret_chpl14;
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  byteIndex_chpl cpi_chpl2;
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t call_tmp_chpl26;
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  byteIndex_chpl i_chpl2;
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 462 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref_int64_t_chpl i_lhs_chpl3 = NULL;
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref_string_chpl i_x_chpl2 = NULL;
#line 465 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl27;
#line 465 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl8;
#line 465 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t call_tmp_chpl28;
#line 465 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _tuple_2_range_int64_t_both_one_int64_t_chpl ret_tmp_chpl9;
#line 415 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  ret_chpl2 = (x_chpl)->cachedNumCodepoints;
#line 415 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  ret_chpl3 = (x_chpl)->buffLen;
#line 415 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  if (ret_chpl2 == ret_chpl3) /* ZLINE: 415 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 415 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  {
#line 416 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    simpleCaseHelper_chpl6(x_chpl, r_chpl, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 416 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl6 = ret_tmp_chpl;
#line 416 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    ret_chpl = call_tmp_chpl6;
#line 416 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    goto _end_getView_chpl;
#line 416 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  } else /* ZLINE: 418 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 416 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  {
#line 428 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    ret_chpl4 = (x_chpl)->cachedNumCodepoints;
#line 428 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl7 = chpl__nudgeHighBound(ret_chpl4);
#line 428 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    chpl_build_bounded_range6(INT64(0), call_tmp_chpl7, &ret_tmp_chpl2);
#line 428 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl8 = ret_tmp_chpl2;
#line 428 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl9 = chpl___EQUALS_2(r_chpl, call_tmp_chpl8, _ln_chpl, _fn_chpl);
#line 428 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    if (call_tmp_chpl9) /* ZLINE: 428 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 428 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    {
#line 428 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      ret_chpl5 = (x_chpl)->buffLen;
#line 428 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      call_tmp_chpl10 = chpl__nudgeHighBound(ret_chpl5);
#line 428 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      chpl_build_bounded_range6(INT64(0), call_tmp_chpl10, &ret_tmp_chpl3);
#line 428 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      call_tmp_chpl11 = ret_tmp_chpl3;
#line 428 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      coerce_tmp_chpl3 = (x_chpl)->cachedNumCodepoints;
#line 428 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      (&ret_tmp_chpl4)->x0 = call_tmp_chpl11;
#line 428 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      (&ret_tmp_chpl4)->x1 = coerce_tmp_chpl3;
#line 428 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      ret_chpl = ret_tmp_chpl4;
#line 428 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      goto _end_getView_chpl;
#line 428 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    }
#line 432 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    intR_chpl = r_chpl;
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    ret_chpl6 = (x_chpl)->cachedNumCodepoints;
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl13 = chpl__nudgeHighBound(ret_chpl6);
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    chpl_build_bounded_range6(INT64(0), call_tmp_chpl13, &ret_tmp_chpl5);
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl14 = ret_tmp_chpl5;
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl12 = call_tmp_chpl14;
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    init_chpl115(&initTemp_chpl, (&intR_chpl)->_low, (&intR_chpl)->_high);
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    tmp_chpl = initTemp_chpl;
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    boundedOther_chpl = tmp_chpl;
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl15 = sizeAs_chpl36(tmp_chpl, _ln_chpl, _fn_chpl);
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    if (! (INT64(0) < INT64(0))) /* ZLINE: 434 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    {
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      tmp_chpl2 = (call_tmp_chpl15 == ((uint64_t)(INT64(0))));
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    } else /* ZLINE: 434 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    {
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      tmp_chpl2 = UINT8(false);
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    }
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    if (tmp_chpl2) /* ZLINE: 434 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    {
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      tmp_chpl3 = UINT8(true);
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    } else /* ZLINE: 434 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    {
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      call_tmp_chpl16 = sizeAs_chpl36(call_tmp_chpl14, _ln_chpl, _fn_chpl);
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      if (! (INT64(0) < INT64(0))) /* ZLINE: 434 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      {
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        tmp_chpl4 = (call_tmp_chpl16 == ((uint64_t)(INT64(0))));
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      } else /* ZLINE: 434 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      {
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        tmp_chpl4 = UINT8(false);
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      }
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      if (tmp_chpl4) /* ZLINE: 434 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      {
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        call_tmp_chpl17 = sizeAs_chpl36(boundedOther_chpl, _ln_chpl, _fn_chpl);
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        if (! (INT64(0) < INT64(0))) /* ZLINE: 434 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        {
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          tmp_chpl5 = (call_tmp_chpl17 == ((uint64_t)(INT64(0))));
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        } else /* ZLINE: 434 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        {
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          tmp_chpl5 = UINT8(false);
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        }
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        ret_chpl7 = tmp_chpl5;
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        goto _end_contains_chpl;
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      }
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      chpl_slice5(call_tmp_chpl12, boundedOther_chpl, &ret_tmp_chpl6);
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      call_tmp_chpl18 = ret_tmp_chpl6;
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      slice_chpl = call_tmp_chpl18;
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      adjustedSlice_chpl(&slice_chpl, &ret_tmp_chpl7);
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      call_tmp_chpl19 = ret_tmp_chpl7;
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      call_tmp_chpl20 = chpl___EQUALS_2(boundedOther_chpl, call_tmp_chpl19, _ln_chpl, _fn_chpl);
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      ret_chpl7 = call_tmp_chpl20;
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      _end_contains_chpl:;
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      tmp_chpl3 = ret_chpl7;
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    }
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    if (! tmp_chpl3) /* ZLINE: 434 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    {
#line 435 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      ret_chpl8 = (x_chpl)->cachedNumCodepoints;
#line 435 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      halt_chpl7(&_str_literal_1319_chpl, r_chpl, &_str_literal_263_chpl, ret_chpl8, _ln_chpl, _fn_chpl);
#line 435 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    }
#line 440 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    cpCount_chpl = INT64(0);
#line 441 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    if ((&intR_chpl)->_low >= INT64(0)) /* ZLINE: 441 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 441 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    {
#line 441 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      tmp_chpl6 = (&intR_chpl)->_low;
#line 441 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    } else /* ZLINE: 441 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 441 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    {
#line 441 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      tmp_chpl6 = INT64(0);
#line 441 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    }
#line 441 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    cpIdxLow_chpl = tmp_chpl6;
#line 445 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    ret_chpl9 = (&intR_chpl)->_high;
#line 444 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    cpIdxHigh_chpl = ret_chpl9;
#line 448 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    byteLow_chpl = (x_chpl)->buffLen;
#line 449 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    byteHigh_chpl = ((int64_t)(((x_chpl)->buffLen - INT64(1))));
#line 451 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    if (ret_chpl9 >= INT64(0)) /* ZLINE: 451 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 451 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    {
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      init_chpl192(&cpi_chpl);
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      (&cpi_chpl)->_bindex = INT64(0);
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      ret_chpl10 = cpi_chpl;
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      default_arg_start_chpl = ret_chpl10;
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      init_chpl194(&ret_chpl12, _ln_chpl, _fn_chpl);
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      (&ret_chpl12)->cachedNumCodepoints = (x_chpl)->cachedNumCodepoints;
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      initWithBorrowedBuffer_chpl(&ret_chpl12, x_chpl, _ln_chpl, _fn_chpl);
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      ret_chpl11 = ret_chpl12;
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      localThis_chpl = ret_chpl11;
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      ret_chpl13 = (&default_arg_start_chpl)->_bindex;
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      i_chpl = ret_chpl13;
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      if (ret_chpl13 > INT64(0)) /* ZLINE: 452 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      {
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        if (ret_chpl13 < (&localThis_chpl)->buffLen) /* ZLINE: 452 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        {
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          coerce_tmp_chpl4 = (&localThis_chpl)->buff;
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          call_tmp_chpl21 = (coerce_tmp_chpl4 + ret_chpl13);
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          tmp_chpl8 = (! ((*(call_tmp_chpl21) & ((uint8_t)(INT64(192)))) != ((uint8_t)(INT64(128)))));
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        } else /* ZLINE: 452 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        {
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          tmp_chpl8 = UINT8(false);
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        }
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        tmp_chpl7 = tmp_chpl8;
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        while (tmp_chpl7) {
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          i_lhs_chpl = &i_chpl;
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          *(i_lhs_chpl) += INT64(1);
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          if (i_chpl < (&localThis_chpl)->buffLen) /* ZLINE: 452 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          {
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
            coerce_tmp_chpl5 = (&localThis_chpl)->buff;
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
            call_tmp_chpl22 = (coerce_tmp_chpl5 + i_chpl);
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
            tmp_chpl9 = (! ((*(call_tmp_chpl22) & ((uint8_t)(INT64(192)))) != ((uint8_t)(INT64(128)))));
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          } else /* ZLINE: 452 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          {
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
            tmp_chpl9 = UINT8(false);
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          }
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          tmp_chpl7 = tmp_chpl9;
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        }
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      }
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      tmp_chpl10 = (i_chpl < (&localThis_chpl)->buffLen);
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      while (tmp_chpl10) {
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        call_tmp_chpl23 = (i_chpl + INT64(1));
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        j_chpl = call_tmp_chpl23;
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        if (call_tmp_chpl23 < (&localThis_chpl)->buffLen) /* ZLINE: 452 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        {
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          coerce_tmp_chpl6 = (&localThis_chpl)->buff;
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          call_tmp_chpl24 = (coerce_tmp_chpl6 + call_tmp_chpl23);
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          tmp_chpl12 = (! ((*(call_tmp_chpl24) & ((uint8_t)(INT64(192)))) != ((uint8_t)(INT64(128)))));
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        } else /* ZLINE: 452 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        {
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          tmp_chpl12 = UINT8(false);
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        }
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        tmp_chpl11 = tmp_chpl12;
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        while (tmp_chpl11) {
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          i_lhs_chpl2 = &j_chpl;
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          *(i_lhs_chpl2) += INT64(1);
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          if (j_chpl < (&localThis_chpl)->buffLen) /* ZLINE: 452 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          {
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
            coerce_tmp_chpl7 = (&localThis_chpl)->buff;
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
            call_tmp_chpl25 = (coerce_tmp_chpl7 + j_chpl);
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
            tmp_chpl13 = (! ((*(call_tmp_chpl25) & ((uint8_t)(INT64(192)))) != ((uint8_t)(INT64(128)))));
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          } else /* ZLINE: 452 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          {
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
            tmp_chpl13 = UINT8(false);
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          }
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          tmp_chpl11 = tmp_chpl13;
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        }
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        init_chpl192(&cpi_chpl2);
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        (&cpi_chpl2)->_bindex = i_chpl;
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        ret_chpl14 = cpi_chpl2;
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        call_tmp_chpl26 = (j_chpl - i_chpl);
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        i_chpl2 = ret_chpl14;
#line 453 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        if (cpCount_chpl == cpIdxLow_chpl) /* ZLINE: 453 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 453 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        {
#line 454 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          byteLow_chpl = (&i_chpl2)->_bindex;
#line 454 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        }
#line 458 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        if (cpCount_chpl == cpIdxHigh_chpl) /* ZLINE: 458 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 458 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        {
#line 459 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          byteHigh_chpl = ((int64_t)((((int64_t)(((&i_chpl2)->_bindex + call_tmp_chpl26))) - INT64(1))));
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          i_x_chpl = &localThis_chpl;
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          deinit_chpl95(i_x_chpl, _ln_chpl, _fn_chpl);
#line 460 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          goto _breakLabel_chpl;
#line 460 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        }
#line 462 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        i_lhs_chpl3 = &cpCount_chpl;
#line 462 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        *(i_lhs_chpl3) += INT64(1);
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        i_chpl = j_chpl;
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        tmp_chpl10 = (i_chpl < (&localThis_chpl)->buffLen);
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      }
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      i_x_chpl2 = &localThis_chpl;
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      deinit_chpl95(i_x_chpl2, _ln_chpl, _fn_chpl);
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      _breakLabel_chpl:;
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    }
#line 465 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    chpl_build_bounded_range6(byteLow_chpl, byteHigh_chpl, &ret_tmp_chpl8);
#line 465 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl27 = ret_tmp_chpl8;
#line 465 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl28 = (((int64_t)((cpIdxHigh_chpl - cpIdxLow_chpl))) + INT64(1));
#line 465 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    (&ret_tmp_chpl9)->x0 = call_tmp_chpl27;
#line 465 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    (&ret_tmp_chpl9)->x1 = call_tmp_chpl28;
#line 465 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    ret_chpl = ret_tmp_chpl9;
#line 465 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    goto _end_getView_chpl;
#line 465 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  }
#line 465 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _end_getView_chpl:;
#line 367 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  *(_retArg_chpl) = ret_chpl;
#line 367 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  return;
#line 367 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
}

#line 367 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
/* BytesStringCommon.chpl:367 */
#line 367 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
static void getView_chpl5(string_chpl * x_chpl,
#line 367 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                          range_byteIndex_both_one_chpl r_chpl,
#line 367 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                          Error_chpl * error_out_chpl,
#line 367 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                          _tuple_2_range_int64_t_both_one_int64_t_chpl * _retArg_chpl,
#line 367 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                          int64_t _ln_chpl,
#line 367 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                          int32_t _fn_chpl) {
#line 367 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _tuple_2_range_int64_t_both_one_int64_t_chpl ret_chpl;
#line 397 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl6;
#line 397 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t ret_chpl2;
#line 397 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t call_tmp_chpl7;
#line 397 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl8;
#line 397 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl;
#line 397 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  byteIndex_chpl call_tmp_chpl9;
#line 397 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t coerce_tmp_chpl3;
#line 397 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  byteIndex_chpl ret_chpl3;
#line 397 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  byteIndex_chpl cpi_chpl;
#line 397 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t ret_chpl4;
#line 397 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl_bool ret_chpl5;
#line 396 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl_bool tmp_chpl;
#line 398 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  byteIndex_chpl call_tmp_chpl10;
#line 398 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t coerce_tmp_chpl4;
#line 398 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  byteIndex_chpl ret_chpl6;
#line 398 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  byteIndex_chpl cpi_chpl2;
#line 398 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  uint8_t call_tmp_chpl11;
#line 401 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  byteIndex_chpl call_tmp_chpl12;
#line 401 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t coerce_tmp_chpl5;
#line 401 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  byteIndex_chpl ret_chpl7;
#line 401 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  byteIndex_chpl cpi_chpl3;
#line 401 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl call_tmp_chpl13;
#line 401 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl ret_tmp_chpl2;
#line 400 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl call_tmp_chpl14;
#line 400 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl ret_tmp_chpl3;
#line 400 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl call_tmp_chpl15;
#line 400 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl ret_tmp_chpl4;
#line 399 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _owned_CodepointSplitError_chpl call_tmp_chpl16;
#line 399 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  CodepointSplitError_chpl new_temp_chpl = NULL;
#line 399 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _owned_CodepointSplitError_chpl initTemp_chpl;
#line 399 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  Error_chpl call_tmp_chpl17 = NULL;
#line 399 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _owned_Error_chpl coerce_tmp_chpl6;
#line 399 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _owned_Error_chpl ret_tmp_chpl5;
#line 399 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref__owned_CodepointSplitError_chpl i_x_chpl = NULL;
#line 399 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref__owned_Error_chpl2 i__retArg_chpl = NULL;
#line 399 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _owned_CodepointSplitError_chpl _formal_tmp_x_chpl;
#line 399 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _owned_Error_chpl ret_chpl8;
#line 399 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref__owned_CodepointSplitError_chpl i_this_chpl = NULL;
#line 399 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  Error_chpl call_tmp_chpl18 = NULL;
#line 399 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref__owned_CodepointSplitError_chpl i_this_chpl2 = NULL;
#line 399 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _owned_Error_chpl call_tmp_chpl19;
#line 399 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _owned_Error_chpl initTemp_chpl2;
#line 399 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _owned_Error_chpl _formal_tmp_in_err_chpl;
#line 399 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref_string_chpl i_x_chpl2 = NULL;
#line 399 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref_string_chpl i_x_chpl3 = NULL;
#line 399 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref_string_chpl i_x_chpl4 = NULL;
#line 406 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl20;
#line 406 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t ret_chpl9;
#line 406 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t call_tmp_chpl21;
#line 406 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl22;
#line 406 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl6;
#line 406 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  byteIndex_chpl call_tmp_chpl23;
#line 406 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t coerce_tmp_chpl7;
#line 406 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  byteIndex_chpl ret_chpl10;
#line 406 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  byteIndex_chpl cpi_chpl4;
#line 406 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t call_tmp_chpl24;
#line 406 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl_bool ret_chpl11;
#line 405 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl_bool tmp_chpl2;
#line 407 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  byteIndex_chpl call_tmp_chpl25;
#line 407 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t coerce_tmp_chpl8;
#line 407 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  byteIndex_chpl ret_chpl12;
#line 407 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  byteIndex_chpl cpi_chpl5;
#line 407 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  uint8_t call_tmp_chpl26;
#line 410 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  byteIndex_chpl call_tmp_chpl27;
#line 410 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t coerce_tmp_chpl9;
#line 410 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  byteIndex_chpl ret_chpl13;
#line 410 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  byteIndex_chpl cpi_chpl6;
#line 410 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl call_tmp_chpl28;
#line 410 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl ret_tmp_chpl7;
#line 409 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl call_tmp_chpl29;
#line 409 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl ret_tmp_chpl8;
#line 409 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl call_tmp_chpl30;
#line 409 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl ret_tmp_chpl9;
#line 408 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _owned_CodepointSplitError_chpl call_tmp_chpl31;
#line 408 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  CodepointSplitError_chpl new_temp_chpl2 = NULL;
#line 408 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _owned_CodepointSplitError_chpl initTemp_chpl3;
#line 408 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  Error_chpl call_tmp_chpl32 = NULL;
#line 408 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _owned_Error_chpl coerce_tmp_chpl10;
#line 408 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _owned_Error_chpl ret_tmp_chpl10;
#line 408 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref__owned_CodepointSplitError_chpl i_x_chpl5 = NULL;
#line 408 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref__owned_Error_chpl2 i__retArg_chpl2 = NULL;
#line 408 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _owned_CodepointSplitError_chpl _formal_tmp_x_chpl2;
#line 408 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _owned_Error_chpl ret_chpl14;
#line 408 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref__owned_CodepointSplitError_chpl i_this_chpl3 = NULL;
#line 408 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  Error_chpl call_tmp_chpl33 = NULL;
#line 408 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref__owned_CodepointSplitError_chpl i_this_chpl4 = NULL;
#line 408 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _owned_Error_chpl call_tmp_chpl34;
#line 408 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _owned_Error_chpl initTemp_chpl4;
#line 408 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _owned_Error_chpl _formal_tmp_in_err_chpl2;
#line 408 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref_string_chpl i_x_chpl6 = NULL;
#line 408 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref_string_chpl i_x_chpl7 = NULL;
#line 408 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref_string_chpl i_x_chpl8 = NULL;
#line 413 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _tuple_2_range_int64_t_both_one_int64_t_chpl call_tmp_chpl35;
#line 413 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _tuple_2_range_int64_t_both_one_int64_t_chpl ret_tmp_chpl11;
#line 397 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  ret_chpl2 = (x_chpl)->buffLen;
#line 397 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl7 = chpl__nudgeHighBound(ret_chpl2);
#line 397 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl_build_bounded_range6(INT64(0), call_tmp_chpl7, &ret_tmp_chpl);
#line 397 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl8 = ret_tmp_chpl;
#line 397 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl6 = call_tmp_chpl8;
#line 397 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  coerce_tmp_chpl3 = (&r_chpl)->_low;
#line 397 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  init_chpl192(&cpi_chpl);
#line 397 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  (&cpi_chpl)->_bindex = coerce_tmp_chpl3;
#line 397 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  ret_chpl3 = cpi_chpl;
#line 397 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl9 = ret_chpl3;
#line 397 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  ret_chpl4 = (&call_tmp_chpl9)->_bindex;
#line 397 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  if (ret_chpl4 > (&call_tmp_chpl6)->_high) /* ZLINE: 397 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 397 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  {
#line 397 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    ret_chpl5 = UINT8(false);
#line 397 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    goto _end_contains_chpl;
#line 397 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  }
#line 397 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  if (ret_chpl4 < (&call_tmp_chpl6)->_low) /* ZLINE: 397 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 397 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  {
#line 397 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    ret_chpl5 = UINT8(false);
#line 397 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    goto _end_contains_chpl;
#line 397 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  }
#line 397 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  ret_chpl5 = UINT8(true);
#line 397 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _end_contains_chpl:;
#line 396 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  if (ret_chpl5) /* ZLINE: 396 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 396 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  {
#line 398 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    coerce_tmp_chpl4 = (&r_chpl)->_low;
#line 398 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    init_chpl192(&cpi_chpl2);
#line 398 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    (&cpi_chpl2)->_bindex = coerce_tmp_chpl4;
#line 398 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    ret_chpl6 = cpi_chpl2;
#line 398 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl10 = ret_chpl6;
#line 398 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl11 = byte_chpl(x_chpl, (&call_tmp_chpl10)->_bindex, _ln_chpl, _fn_chpl);
#line 396 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    tmp_chpl = (! ((call_tmp_chpl11 & ((uint8_t)(INT64(192)))) != ((uint8_t)(INT64(128)))));
#line 396 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  } else /* ZLINE: 396 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 396 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  {
#line 396 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    tmp_chpl = UINT8(false);
#line 396 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  }
#line 396 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  if (tmp_chpl) /* ZLINE: 398 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 396 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  {
#line 401 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    coerce_tmp_chpl5 = (&r_chpl)->_low;
#line 401 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    init_chpl192(&cpi_chpl3);
#line 401 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    (&cpi_chpl3)->_bindex = coerce_tmp_chpl5;
#line 401 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    ret_chpl7 = cpi_chpl3;
#line 401 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl12 = ret_chpl7;
#line 401 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    chpl___COLON_18(&call_tmp_chpl12, &ret_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 401 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl13 = ret_tmp_chpl2;
#line 400 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    chpl___PLUS_(&_str_literal_4042_chpl, &call_tmp_chpl13, &ret_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 400 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl14 = ret_tmp_chpl3;
#line 400 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    chpl___PLUS_(&call_tmp_chpl14, &_str_literal_1324_chpl, &ret_tmp_chpl4, _ln_chpl, _fn_chpl);
#line 400 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl15 = ret_tmp_chpl4;
#line 399 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    new_temp_chpl = _new_chpl60(&call_tmp_chpl15, _ln_chpl, _fn_chpl);
#line 399 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    init_chpl171(&initTemp_chpl, new_temp_chpl);
#line 399 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl16 = initTemp_chpl;
#line 399 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    i_x_chpl = &call_tmp_chpl16;
#line 399 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    i__retArg_chpl = &ret_tmp_chpl5;
#line 399 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    _formal_tmp_x_chpl = *(i_x_chpl);
#line 399 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    i_this_chpl = &_formal_tmp_x_chpl;
#line 399 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl18 = ((Error_chpl)((i_this_chpl)->chpl_p));
#line 399 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    i_this_chpl2 = &_formal_tmp_x_chpl;
#line 399 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    (i_this_chpl2)->chpl_p = ((CodepointSplitError_chpl)(nil));
#line 399 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    if (((RootClass_chpl)(call_tmp_chpl18)) == nil) /* ZLINE: 399 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 399 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    {
#line 399 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      nilCheckHalt_chpl(&_str_literal_52_chpl, _ln_chpl, _fn_chpl);
#line 399 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    }
#line 399 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    init_chpl188(&initTemp_chpl2, call_tmp_chpl18);
#line 399 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl19 = initTemp_chpl2;
#line 399 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    ret_chpl8 = call_tmp_chpl19;
#line 399 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    chpl__autoDestroy36(&_formal_tmp_x_chpl, _ln_chpl, _fn_chpl);
#line 399 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    *(i__retArg_chpl) = ret_chpl8;
#line 399 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    coerce_tmp_chpl6 = ret_tmp_chpl5;
#line 399 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    _formal_tmp_in_err_chpl = coerce_tmp_chpl6;
#line 399 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl17 = chpl_fix_thrown_error(&_formal_tmp_in_err_chpl, _ln_chpl, _fn_chpl);
#line 399 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    *(error_out_chpl) = call_tmp_chpl17;
#line 399 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    i_x_chpl2 = &call_tmp_chpl15;
#line 399 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    deinit_chpl95(i_x_chpl2, _ln_chpl, _fn_chpl);
#line 399 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    i_x_chpl3 = &call_tmp_chpl14;
#line 399 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    deinit_chpl95(i_x_chpl3, _ln_chpl, _fn_chpl);
#line 399 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    i_x_chpl4 = &call_tmp_chpl13;
#line 399 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    deinit_chpl95(i_x_chpl4, _ln_chpl, _fn_chpl);
#line 399 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    goto _end_getView_chpl;
#line 399 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  }
#line 406 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  ret_chpl9 = (x_chpl)->buffLen;
#line 406 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl21 = chpl__nudgeHighBound(ret_chpl9);
#line 406 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl_build_bounded_range6(INT64(0), call_tmp_chpl21, &ret_tmp_chpl6);
#line 406 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl22 = ret_tmp_chpl6;
#line 406 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl20 = call_tmp_chpl22;
#line 406 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  coerce_tmp_chpl7 = (&r_chpl)->_high;
#line 406 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  init_chpl192(&cpi_chpl4);
#line 406 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  (&cpi_chpl4)->_bindex = coerce_tmp_chpl7;
#line 406 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  ret_chpl10 = cpi_chpl4;
#line 406 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl23 = ret_chpl10;
#line 406 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl24 = ((&call_tmp_chpl23)->_bindex + INT64(1));
#line 406 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  if (call_tmp_chpl24 > (&call_tmp_chpl20)->_high) /* ZLINE: 406 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 406 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  {
#line 406 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    ret_chpl11 = UINT8(false);
#line 406 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    goto _end_contains_chpl2;
#line 406 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  }
#line 406 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  if (call_tmp_chpl24 < (&call_tmp_chpl20)->_low) /* ZLINE: 406 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 406 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  {
#line 406 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    ret_chpl11 = UINT8(false);
#line 406 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    goto _end_contains_chpl2;
#line 406 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  }
#line 406 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  ret_chpl11 = UINT8(true);
#line 406 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _end_contains_chpl2:;
#line 405 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  if (ret_chpl11) /* ZLINE: 405 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 405 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  {
#line 407 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    coerce_tmp_chpl8 = (&r_chpl)->_high;
#line 407 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    init_chpl192(&cpi_chpl5);
#line 407 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    (&cpi_chpl5)->_bindex = coerce_tmp_chpl8;
#line 407 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    ret_chpl12 = cpi_chpl5;
#line 407 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl25 = ret_chpl12;
#line 407 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl26 = byte_chpl(x_chpl, ((int64_t)(((&call_tmp_chpl25)->_bindex + INT64(1)))), _ln_chpl, _fn_chpl);
#line 405 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    tmp_chpl2 = (! ((call_tmp_chpl26 & ((uint8_t)(INT64(192)))) != ((uint8_t)(INT64(128)))));
#line 405 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  } else /* ZLINE: 405 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 405 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  {
#line 405 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    tmp_chpl2 = UINT8(false);
#line 405 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  }
#line 405 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  if (tmp_chpl2) /* ZLINE: 407 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 405 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  {
#line 410 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    coerce_tmp_chpl9 = (&r_chpl)->_high;
#line 410 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    init_chpl192(&cpi_chpl6);
#line 410 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    (&cpi_chpl6)->_bindex = coerce_tmp_chpl9;
#line 410 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    ret_chpl13 = cpi_chpl6;
#line 410 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl27 = ret_chpl13;
#line 410 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    chpl___COLON_18(&call_tmp_chpl27, &ret_tmp_chpl7, _ln_chpl, _fn_chpl);
#line 410 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl28 = ret_tmp_chpl7;
#line 409 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    chpl___PLUS_(&_str_literal_4044_chpl, &call_tmp_chpl28, &ret_tmp_chpl8, _ln_chpl, _fn_chpl);
#line 409 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl29 = ret_tmp_chpl8;
#line 409 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    chpl___PLUS_(&call_tmp_chpl29, &_str_literal_1324_chpl, &ret_tmp_chpl9, _ln_chpl, _fn_chpl);
#line 409 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl30 = ret_tmp_chpl9;
#line 408 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    new_temp_chpl2 = _new_chpl60(&call_tmp_chpl30, _ln_chpl, _fn_chpl);
#line 408 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    init_chpl171(&initTemp_chpl3, new_temp_chpl2);
#line 408 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl31 = initTemp_chpl3;
#line 408 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    i_x_chpl5 = &call_tmp_chpl31;
#line 408 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    i__retArg_chpl2 = &ret_tmp_chpl10;
#line 408 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    _formal_tmp_x_chpl2 = *(i_x_chpl5);
#line 408 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    i_this_chpl3 = &_formal_tmp_x_chpl2;
#line 408 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl33 = ((Error_chpl)((i_this_chpl3)->chpl_p));
#line 408 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    i_this_chpl4 = &_formal_tmp_x_chpl2;
#line 408 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    (i_this_chpl4)->chpl_p = ((CodepointSplitError_chpl)(nil));
#line 408 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    if (((RootClass_chpl)(call_tmp_chpl33)) == nil) /* ZLINE: 408 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 408 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    {
#line 408 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      nilCheckHalt_chpl(&_str_literal_52_chpl, _ln_chpl, _fn_chpl);
#line 408 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    }
#line 408 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    init_chpl188(&initTemp_chpl4, call_tmp_chpl33);
#line 408 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl34 = initTemp_chpl4;
#line 408 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    ret_chpl14 = call_tmp_chpl34;
#line 408 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    chpl__autoDestroy36(&_formal_tmp_x_chpl2, _ln_chpl, _fn_chpl);
#line 408 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    *(i__retArg_chpl2) = ret_chpl14;
#line 408 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    coerce_tmp_chpl10 = ret_tmp_chpl10;
#line 408 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    _formal_tmp_in_err_chpl2 = coerce_tmp_chpl10;
#line 408 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl32 = chpl_fix_thrown_error(&_formal_tmp_in_err_chpl2, _ln_chpl, _fn_chpl);
#line 408 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    *(error_out_chpl) = call_tmp_chpl32;
#line 408 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    i_x_chpl6 = &call_tmp_chpl30;
#line 408 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    deinit_chpl95(i_x_chpl6, _ln_chpl, _fn_chpl);
#line 408 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    i_x_chpl7 = &call_tmp_chpl29;
#line 408 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    deinit_chpl95(i_x_chpl7, _ln_chpl, _fn_chpl);
#line 408 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    i_x_chpl8 = &call_tmp_chpl28;
#line 408 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    deinit_chpl95(i_x_chpl8, _ln_chpl, _fn_chpl);
#line 408 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    goto _end_getView_chpl;
#line 408 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  }
#line 413 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  simpleCaseHelper_chpl5(x_chpl, r_chpl, &ret_tmp_chpl11, _ln_chpl, _fn_chpl);
#line 413 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl35 = ret_tmp_chpl11;
#line 413 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  ret_chpl = call_tmp_chpl35;
#line 413 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _end_getView_chpl:;
#line 367 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  *(_retArg_chpl) = ret_chpl;
#line 367 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  return;
#line 367 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
}

#line 367 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
/* BytesStringCommon.chpl:367 */
#line 367 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
static void getView_chpl4(string_chpl * x_chpl,
#line 367 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                          range_int64_t_low_one_chpl r_chpl,
#line 367 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                          Error_chpl * error_out_chpl,
#line 367 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                          _tuple_2_range_int64_t_both_one_int64_t_chpl * _retArg_chpl,
#line 367 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                          int64_t _ln_chpl,
#line 367 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                          int32_t _fn_chpl) {
#line 367 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _tuple_2_range_int64_t_both_one_int64_t_chpl ret_chpl;
#line 415 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t ret_chpl2;
#line 415 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t ret_chpl3;
#line 416 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _tuple_2_range_int64_t_both_one_int64_t_chpl call_tmp_chpl6;
#line 416 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _tuple_2_range_int64_t_both_one_int64_t_chpl ret_tmp_chpl;
#line 428 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t ret_chpl4;
#line 428 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t call_tmp_chpl7;
#line 428 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl2;
#line 432 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_low_one_chpl intR_chpl;
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl8;
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t ret_chpl5;
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t call_tmp_chpl9;
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl10;
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl3;
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl boundedOther_chpl;
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl tmp_chpl;
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl initTemp_chpl;
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  uint64_t call_tmp_chpl11;
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl_bool tmp_chpl2;
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl_bool tmp_chpl3;
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl_bool ret_chpl6;
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  uint64_t call_tmp_chpl12;
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl_bool tmp_chpl4;
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  uint64_t call_tmp_chpl13;
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl_bool tmp_chpl5;
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl14;
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl4;
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl slice_chpl;
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl15;
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl5;
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl_bool call_tmp_chpl16;
#line 435 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t ret_chpl7;
#line 440 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t cpCount_chpl;
#line 441 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t cpIdxLow_chpl;
#line 441 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t tmp_chpl6;
#line 444 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t cpIdxHigh_chpl;
#line 446 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t call_tmp_chpl17;
#line 448 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t byteLow_chpl;
#line 449 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t byteHigh_chpl;
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  byteIndex_chpl default_arg_start_chpl;
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  byteIndex_chpl ret_chpl8;
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  byteIndex_chpl cpi_chpl;
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl ret_chpl9;
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl ret_chpl10;
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl localThis_chpl;
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t ret_chpl11;
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t i_chpl;
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl_bool tmp_chpl7;
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl_bool tmp_chpl8;
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  c_ptr_uint8_t_chpl coerce_tmp_chpl3 = NULL;
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref_uint8_t_chpl call_tmp_chpl18 = NULL;
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref_int64_t_chpl i_lhs_chpl = NULL;
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl_bool tmp_chpl9;
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  c_ptr_uint8_t_chpl coerce_tmp_chpl4 = NULL;
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref_uint8_t_chpl call_tmp_chpl19 = NULL;
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl_bool tmp_chpl10;
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t call_tmp_chpl20;
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t j_chpl;
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl_bool tmp_chpl11;
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl_bool tmp_chpl12;
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  c_ptr_uint8_t_chpl coerce_tmp_chpl5 = NULL;
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref_uint8_t_chpl call_tmp_chpl21 = NULL;
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref_int64_t_chpl i_lhs_chpl2 = NULL;
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl_bool tmp_chpl13;
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  c_ptr_uint8_t_chpl coerce_tmp_chpl6 = NULL;
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref_uint8_t_chpl call_tmp_chpl22 = NULL;
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  byteIndex_chpl ret_chpl12;
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  byteIndex_chpl cpi_chpl2;
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t call_tmp_chpl23;
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  byteIndex_chpl i_chpl2;
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref_string_chpl i_x_chpl2 = NULL;
#line 462 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref_int64_t_chpl i_lhs_chpl3 = NULL;
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref_string_chpl i_x_chpl3 = NULL;
#line 465 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl24;
#line 465 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl6;
#line 465 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t call_tmp_chpl25;
#line 465 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _tuple_2_range_int64_t_both_one_int64_t_chpl ret_tmp_chpl7;
#line 415 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  ret_chpl2 = (x_chpl)->cachedNumCodepoints;
#line 415 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  ret_chpl3 = (x_chpl)->buffLen;
#line 415 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  if (ret_chpl2 == ret_chpl3) /* ZLINE: 415 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 415 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  {
#line 416 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    simpleCaseHelper_chpl4(x_chpl, r_chpl, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 416 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl6 = ret_tmp_chpl;
#line 416 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    ret_chpl = call_tmp_chpl6;
#line 416 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    goto _end_getView_chpl;
#line 416 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  } else /* ZLINE: 418 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 416 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  {
#line 428 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    ret_chpl4 = (x_chpl)->cachedNumCodepoints;
#line 428 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl7 = chpl__nudgeHighBound(ret_chpl4);
#line 428 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    chpl_build_bounded_range6(INT64(0), call_tmp_chpl7, &ret_tmp_chpl2);
#line 432 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    intR_chpl = r_chpl;
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    ret_chpl5 = (x_chpl)->cachedNumCodepoints;
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl9 = chpl__nudgeHighBound(ret_chpl5);
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    chpl_build_bounded_range6(INT64(0), call_tmp_chpl9, &ret_tmp_chpl3);
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl10 = ret_tmp_chpl3;
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl8 = call_tmp_chpl10;
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    init_chpl115(&initTemp_chpl, (&intR_chpl)->_low, (&call_tmp_chpl8)->_high);
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    tmp_chpl = initTemp_chpl;
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    boundedOther_chpl = tmp_chpl;
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl11 = sizeAs_chpl36(tmp_chpl, _ln_chpl, _fn_chpl);
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    if (! (INT64(0) < INT64(0))) /* ZLINE: 434 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    {
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      tmp_chpl2 = (call_tmp_chpl11 == ((uint64_t)(INT64(0))));
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    } else /* ZLINE: 434 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    {
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      tmp_chpl2 = UINT8(false);
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    }
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    if (tmp_chpl2) /* ZLINE: 434 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    {
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      tmp_chpl3 = UINT8(true);
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    } else /* ZLINE: 434 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    {
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      call_tmp_chpl12 = sizeAs_chpl36(call_tmp_chpl10, _ln_chpl, _fn_chpl);
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      if (! (INT64(0) < INT64(0))) /* ZLINE: 434 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      {
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        tmp_chpl4 = (call_tmp_chpl12 == ((uint64_t)(INT64(0))));
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      } else /* ZLINE: 434 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      {
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        tmp_chpl4 = UINT8(false);
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      }
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      if (tmp_chpl4) /* ZLINE: 434 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      {
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        call_tmp_chpl13 = sizeAs_chpl36(boundedOther_chpl, _ln_chpl, _fn_chpl);
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        if (! (INT64(0) < INT64(0))) /* ZLINE: 434 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        {
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          tmp_chpl5 = (call_tmp_chpl13 == ((uint64_t)(INT64(0))));
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        } else /* ZLINE: 434 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        {
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          tmp_chpl5 = UINT8(false);
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        }
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        ret_chpl6 = tmp_chpl5;
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        goto _end_contains_chpl;
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      }
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      chpl_slice5(call_tmp_chpl8, boundedOther_chpl, &ret_tmp_chpl4);
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      call_tmp_chpl14 = ret_tmp_chpl4;
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      slice_chpl = call_tmp_chpl14;
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      adjustedSlice_chpl(&slice_chpl, &ret_tmp_chpl5);
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      call_tmp_chpl15 = ret_tmp_chpl5;
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      call_tmp_chpl16 = chpl___EQUALS_2(boundedOther_chpl, call_tmp_chpl15, _ln_chpl, _fn_chpl);
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      ret_chpl6 = call_tmp_chpl16;
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      _end_contains_chpl:;
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      tmp_chpl3 = ret_chpl6;
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    }
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    if (! tmp_chpl3) /* ZLINE: 434 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    {
#line 435 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      ret_chpl7 = (x_chpl)->cachedNumCodepoints;
#line 435 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      halt_chpl6(&_str_literal_1319_chpl, r_chpl, &_str_literal_263_chpl, ret_chpl7, _ln_chpl, _fn_chpl);
#line 435 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    }
#line 440 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    cpCount_chpl = INT64(0);
#line 441 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    if ((&intR_chpl)->_low >= INT64(0)) /* ZLINE: 441 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 441 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    {
#line 441 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      tmp_chpl6 = (&intR_chpl)->_low;
#line 441 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    } else /* ZLINE: 441 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 441 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    {
#line 441 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      tmp_chpl6 = INT64(0);
#line 441 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    }
#line 441 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    cpIdxLow_chpl = tmp_chpl6;
#line 446 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl17 = ((x_chpl)->buffLen - INT64(1));
#line 444 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    cpIdxHigh_chpl = call_tmp_chpl17;
#line 448 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    byteLow_chpl = (x_chpl)->buffLen;
#line 449 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    byteHigh_chpl = ((int64_t)(((x_chpl)->buffLen - INT64(1))));
#line 451 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    if (call_tmp_chpl17 >= INT64(0)) /* ZLINE: 451 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 451 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    {
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      init_chpl192(&cpi_chpl);
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      (&cpi_chpl)->_bindex = INT64(0);
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      ret_chpl8 = cpi_chpl;
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      default_arg_start_chpl = ret_chpl8;
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      init_chpl194(&ret_chpl10, _ln_chpl, _fn_chpl);
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      (&ret_chpl10)->cachedNumCodepoints = (x_chpl)->cachedNumCodepoints;
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      initWithBorrowedBuffer_chpl(&ret_chpl10, x_chpl, _ln_chpl, _fn_chpl);
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      ret_chpl9 = ret_chpl10;
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      localThis_chpl = ret_chpl9;
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      ret_chpl11 = (&default_arg_start_chpl)->_bindex;
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      i_chpl = ret_chpl11;
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      if (ret_chpl11 > INT64(0)) /* ZLINE: 452 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      {
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        if (ret_chpl11 < (&localThis_chpl)->buffLen) /* ZLINE: 452 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        {
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          coerce_tmp_chpl3 = (&localThis_chpl)->buff;
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          call_tmp_chpl18 = (coerce_tmp_chpl3 + ret_chpl11);
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          tmp_chpl8 = (! ((*(call_tmp_chpl18) & ((uint8_t)(INT64(192)))) != ((uint8_t)(INT64(128)))));
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        } else /* ZLINE: 452 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        {
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          tmp_chpl8 = UINT8(false);
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        }
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        tmp_chpl7 = tmp_chpl8;
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        while (tmp_chpl7) {
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          i_lhs_chpl = &i_chpl;
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          *(i_lhs_chpl) += INT64(1);
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          if (i_chpl < (&localThis_chpl)->buffLen) /* ZLINE: 452 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          {
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
            coerce_tmp_chpl4 = (&localThis_chpl)->buff;
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
            call_tmp_chpl19 = (coerce_tmp_chpl4 + i_chpl);
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
            tmp_chpl9 = (! ((*(call_tmp_chpl19) & ((uint8_t)(INT64(192)))) != ((uint8_t)(INT64(128)))));
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          } else /* ZLINE: 452 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          {
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
            tmp_chpl9 = UINT8(false);
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          }
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          tmp_chpl7 = tmp_chpl9;
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        }
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      }
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      tmp_chpl10 = (i_chpl < (&localThis_chpl)->buffLen);
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      while (tmp_chpl10) {
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        call_tmp_chpl20 = (i_chpl + INT64(1));
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        j_chpl = call_tmp_chpl20;
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        if (call_tmp_chpl20 < (&localThis_chpl)->buffLen) /* ZLINE: 452 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        {
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          coerce_tmp_chpl5 = (&localThis_chpl)->buff;
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          call_tmp_chpl21 = (coerce_tmp_chpl5 + call_tmp_chpl20);
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          tmp_chpl12 = (! ((*(call_tmp_chpl21) & ((uint8_t)(INT64(192)))) != ((uint8_t)(INT64(128)))));
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        } else /* ZLINE: 452 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        {
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          tmp_chpl12 = UINT8(false);
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        }
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        tmp_chpl11 = tmp_chpl12;
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        while (tmp_chpl11) {
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          i_lhs_chpl2 = &j_chpl;
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          *(i_lhs_chpl2) += INT64(1);
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          if (j_chpl < (&localThis_chpl)->buffLen) /* ZLINE: 452 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          {
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
            coerce_tmp_chpl6 = (&localThis_chpl)->buff;
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
            call_tmp_chpl22 = (coerce_tmp_chpl6 + j_chpl);
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
            tmp_chpl13 = (! ((*(call_tmp_chpl22) & ((uint8_t)(INT64(192)))) != ((uint8_t)(INT64(128)))));
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          } else /* ZLINE: 452 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          {
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
            tmp_chpl13 = UINT8(false);
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          }
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          tmp_chpl11 = tmp_chpl13;
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        }
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        init_chpl192(&cpi_chpl2);
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        (&cpi_chpl2)->_bindex = i_chpl;
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        ret_chpl12 = cpi_chpl2;
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        call_tmp_chpl23 = (j_chpl - i_chpl);
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        i_chpl2 = ret_chpl12;
#line 453 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        if (cpCount_chpl == cpIdxLow_chpl) /* ZLINE: 453 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 453 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        {
#line 454 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          byteLow_chpl = (&i_chpl2)->_bindex;
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          i_x_chpl = &localThis_chpl;
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          deinit_chpl95(i_x_chpl, _ln_chpl, _fn_chpl);
#line 456 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          goto _breakLabel_chpl;
#line 456 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        }
#line 458 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        if (cpCount_chpl == cpIdxHigh_chpl) /* ZLINE: 458 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 458 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        {
#line 459 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          byteHigh_chpl = ((int64_t)((((int64_t)(((&i_chpl2)->_bindex + call_tmp_chpl23))) - INT64(1))));
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          i_x_chpl2 = &localThis_chpl;
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          deinit_chpl95(i_x_chpl2, _ln_chpl, _fn_chpl);
#line 460 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          goto _breakLabel_chpl;
#line 460 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        }
#line 462 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        i_lhs_chpl3 = &cpCount_chpl;
#line 462 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        *(i_lhs_chpl3) += INT64(1);
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        i_chpl = j_chpl;
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        tmp_chpl10 = (i_chpl < (&localThis_chpl)->buffLen);
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      }
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      i_x_chpl3 = &localThis_chpl;
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      deinit_chpl95(i_x_chpl3, _ln_chpl, _fn_chpl);
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      _breakLabel_chpl:;
#line 452 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    }
#line 465 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    chpl_build_bounded_range6(byteLow_chpl, byteHigh_chpl, &ret_tmp_chpl6);
#line 465 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl24 = ret_tmp_chpl6;
#line 465 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl25 = (((int64_t)((cpIdxHigh_chpl - cpIdxLow_chpl))) + INT64(1));
#line 465 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    (&ret_tmp_chpl7)->x0 = call_tmp_chpl24;
#line 465 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    (&ret_tmp_chpl7)->x1 = call_tmp_chpl25;
#line 465 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    ret_chpl = ret_tmp_chpl7;
#line 465 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    goto _end_getView_chpl;
#line 465 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  }
#line 465 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _end_getView_chpl:;
#line 367 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  *(_retArg_chpl) = ret_chpl;
#line 367 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  return;
#line 367 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
}

#line 367 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
/* BytesStringCommon.chpl:367 */
#line 367 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
static void getView_chpl3(string_chpl * x_chpl,
#line 367 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                          range_byteIndex_low_one_chpl r_chpl,
#line 367 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                          Error_chpl * error_out_chpl,
#line 367 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                          _tuple_2_range_int64_t_both_one_int64_t_chpl * _retArg_chpl,
#line 367 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                          int64_t _ln_chpl,
#line 367 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                          int32_t _fn_chpl) {
#line 367 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _tuple_2_range_int64_t_both_one_int64_t_chpl ret_chpl;
#line 397 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl6;
#line 397 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t ret_chpl2;
#line 397 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t call_tmp_chpl7;
#line 397 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl8;
#line 397 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl;
#line 397 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  byteIndex_chpl call_tmp_chpl9;
#line 397 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t coerce_tmp_chpl3;
#line 397 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  byteIndex_chpl ret_chpl3;
#line 397 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  byteIndex_chpl cpi_chpl;
#line 397 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t ret_chpl4;
#line 397 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl_bool ret_chpl5;
#line 396 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl_bool tmp_chpl;
#line 398 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  byteIndex_chpl call_tmp_chpl10;
#line 398 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t coerce_tmp_chpl4;
#line 398 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  byteIndex_chpl ret_chpl6;
#line 398 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  byteIndex_chpl cpi_chpl2;
#line 398 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  uint8_t call_tmp_chpl11;
#line 401 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  byteIndex_chpl call_tmp_chpl12;
#line 401 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t coerce_tmp_chpl5;
#line 401 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  byteIndex_chpl ret_chpl7;
#line 401 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  byteIndex_chpl cpi_chpl3;
#line 401 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl call_tmp_chpl13;
#line 401 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl ret_tmp_chpl2;
#line 400 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl call_tmp_chpl14;
#line 400 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl ret_tmp_chpl3;
#line 400 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl call_tmp_chpl15;
#line 400 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl ret_tmp_chpl4;
#line 399 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _owned_CodepointSplitError_chpl call_tmp_chpl16;
#line 399 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  CodepointSplitError_chpl new_temp_chpl = NULL;
#line 399 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _owned_CodepointSplitError_chpl initTemp_chpl;
#line 399 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  Error_chpl call_tmp_chpl17 = NULL;
#line 399 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _owned_Error_chpl coerce_tmp_chpl6;
#line 399 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _owned_Error_chpl ret_tmp_chpl5;
#line 399 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref__owned_CodepointSplitError_chpl i_x_chpl = NULL;
#line 399 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref__owned_Error_chpl2 i__retArg_chpl = NULL;
#line 399 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _owned_CodepointSplitError_chpl _formal_tmp_x_chpl;
#line 399 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _owned_Error_chpl ret_chpl8;
#line 399 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref__owned_CodepointSplitError_chpl i_this_chpl = NULL;
#line 399 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  Error_chpl call_tmp_chpl18 = NULL;
#line 399 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref__owned_CodepointSplitError_chpl i_this_chpl2 = NULL;
#line 399 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _owned_Error_chpl call_tmp_chpl19;
#line 399 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _owned_Error_chpl initTemp_chpl2;
#line 399 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _owned_Error_chpl _formal_tmp_in_err_chpl;
#line 399 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref_string_chpl i_x_chpl2 = NULL;
#line 399 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref_string_chpl i_x_chpl3 = NULL;
#line 399 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref_string_chpl i_x_chpl4 = NULL;
#line 413 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _tuple_2_range_int64_t_both_one_int64_t_chpl call_tmp_chpl20;
#line 413 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _tuple_2_range_int64_t_both_one_int64_t_chpl ret_tmp_chpl6;
#line 397 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  ret_chpl2 = (x_chpl)->buffLen;
#line 397 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl7 = chpl__nudgeHighBound(ret_chpl2);
#line 397 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl_build_bounded_range6(INT64(0), call_tmp_chpl7, &ret_tmp_chpl);
#line 397 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl8 = ret_tmp_chpl;
#line 397 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl6 = call_tmp_chpl8;
#line 397 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  coerce_tmp_chpl3 = (&r_chpl)->_low;
#line 397 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  init_chpl192(&cpi_chpl);
#line 397 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  (&cpi_chpl)->_bindex = coerce_tmp_chpl3;
#line 397 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  ret_chpl3 = cpi_chpl;
#line 397 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl9 = ret_chpl3;
#line 397 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  ret_chpl4 = (&call_tmp_chpl9)->_bindex;
#line 397 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  if (ret_chpl4 > (&call_tmp_chpl6)->_high) /* ZLINE: 397 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 397 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  {
#line 397 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    ret_chpl5 = UINT8(false);
#line 397 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    goto _end_contains_chpl;
#line 397 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  }
#line 397 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  if (ret_chpl4 < (&call_tmp_chpl6)->_low) /* ZLINE: 397 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 397 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  {
#line 397 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    ret_chpl5 = UINT8(false);
#line 397 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    goto _end_contains_chpl;
#line 397 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  }
#line 397 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  ret_chpl5 = UINT8(true);
#line 397 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _end_contains_chpl:;
#line 396 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  if (ret_chpl5) /* ZLINE: 396 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 396 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  {
#line 398 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    coerce_tmp_chpl4 = (&r_chpl)->_low;
#line 398 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    init_chpl192(&cpi_chpl2);
#line 398 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    (&cpi_chpl2)->_bindex = coerce_tmp_chpl4;
#line 398 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    ret_chpl6 = cpi_chpl2;
#line 398 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl10 = ret_chpl6;
#line 398 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl11 = byte_chpl(x_chpl, (&call_tmp_chpl10)->_bindex, _ln_chpl, _fn_chpl);
#line 396 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    tmp_chpl = (! ((call_tmp_chpl11 & ((uint8_t)(INT64(192)))) != ((uint8_t)(INT64(128)))));
#line 396 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  } else /* ZLINE: 396 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 396 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  {
#line 396 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    tmp_chpl = UINT8(false);
#line 396 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  }
#line 396 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  if (tmp_chpl) /* ZLINE: 398 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 396 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  {
#line 401 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    coerce_tmp_chpl5 = (&r_chpl)->_low;
#line 401 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    init_chpl192(&cpi_chpl3);
#line 401 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    (&cpi_chpl3)->_bindex = coerce_tmp_chpl5;
#line 401 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    ret_chpl7 = cpi_chpl3;
#line 401 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl12 = ret_chpl7;
#line 401 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    chpl___COLON_18(&call_tmp_chpl12, &ret_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 401 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl13 = ret_tmp_chpl2;
#line 400 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    chpl___PLUS_(&_str_literal_4042_chpl, &call_tmp_chpl13, &ret_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 400 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl14 = ret_tmp_chpl3;
#line 400 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    chpl___PLUS_(&call_tmp_chpl14, &_str_literal_1324_chpl, &ret_tmp_chpl4, _ln_chpl, _fn_chpl);
#line 400 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl15 = ret_tmp_chpl4;
#line 399 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    new_temp_chpl = _new_chpl60(&call_tmp_chpl15, _ln_chpl, _fn_chpl);
#line 399 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    init_chpl171(&initTemp_chpl, new_temp_chpl);
#line 399 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl16 = initTemp_chpl;
#line 399 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    i_x_chpl = &call_tmp_chpl16;
#line 399 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    i__retArg_chpl = &ret_tmp_chpl5;
#line 399 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    _formal_tmp_x_chpl = *(i_x_chpl);
#line 399 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    i_this_chpl = &_formal_tmp_x_chpl;
#line 399 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl18 = ((Error_chpl)((i_this_chpl)->chpl_p));
#line 399 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    i_this_chpl2 = &_formal_tmp_x_chpl;
#line 399 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    (i_this_chpl2)->chpl_p = ((CodepointSplitError_chpl)(nil));
#line 399 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    if (((RootClass_chpl)(call_tmp_chpl18)) == nil) /* ZLINE: 399 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 399 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    {
#line 399 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      nilCheckHalt_chpl(&_str_literal_52_chpl, _ln_chpl, _fn_chpl);
#line 399 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    }
#line 399 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    init_chpl188(&initTemp_chpl2, call_tmp_chpl18);
#line 399 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl19 = initTemp_chpl2;
#line 399 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    ret_chpl8 = call_tmp_chpl19;
#line 399 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    chpl__autoDestroy36(&_formal_tmp_x_chpl, _ln_chpl, _fn_chpl);
#line 399 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    *(i__retArg_chpl) = ret_chpl8;
#line 399 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    coerce_tmp_chpl6 = ret_tmp_chpl5;
#line 399 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    _formal_tmp_in_err_chpl = coerce_tmp_chpl6;
#line 399 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl17 = chpl_fix_thrown_error(&_formal_tmp_in_err_chpl, _ln_chpl, _fn_chpl);
#line 399 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    *(error_out_chpl) = call_tmp_chpl17;
#line 399 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    i_x_chpl2 = &call_tmp_chpl15;
#line 399 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    deinit_chpl95(i_x_chpl2, _ln_chpl, _fn_chpl);
#line 399 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    i_x_chpl3 = &call_tmp_chpl14;
#line 399 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    deinit_chpl95(i_x_chpl3, _ln_chpl, _fn_chpl);
#line 399 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    i_x_chpl4 = &call_tmp_chpl13;
#line 399 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    deinit_chpl95(i_x_chpl4, _ln_chpl, _fn_chpl);
#line 399 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    goto _end_getView_chpl;
#line 399 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  }
#line 413 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  simpleCaseHelper_chpl3(x_chpl, r_chpl, &ret_tmp_chpl6, _ln_chpl, _fn_chpl);
#line 413 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl20 = ret_tmp_chpl6;
#line 413 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  ret_chpl = call_tmp_chpl20;
#line 413 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _end_getView_chpl:;
#line 367 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  *(_retArg_chpl) = ret_chpl;
#line 367 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  return;
#line 367 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
}

#line 367 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
/* BytesStringCommon.chpl:367 */
#line 367 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
static void getView_chpl2(string_chpl * x_chpl,
#line 367 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                          range_byteIndex_low_one_chpl r_chpl,
#line 367 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                          Error_chpl * error_out_chpl,
#line 367 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                          _tuple_2_range_int64_t_both_one_int64_t_chpl * _retArg_chpl,
#line 367 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                          int64_t _ln_chpl,
#line 367 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                          int32_t _fn_chpl) {
#line 413 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _tuple_2_range_int64_t_both_one_int64_t_chpl call_tmp_chpl6;
#line 413 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _tuple_2_range_int64_t_both_one_int64_t_chpl ret_tmp_chpl;
#line 413 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  simpleCaseHelper_chpl2(x_chpl, r_chpl, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 413 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl6 = ret_tmp_chpl;
#line 367 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  *(_retArg_chpl) = call_tmp_chpl6;
#line 367 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  return;
#line 367 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
}

#line 367 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
/* BytesStringCommon.chpl:367 */
#line 367 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
static void getView_chpl(string_chpl * x_chpl,
#line 367 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                         range_byteIndex_high_one_chpl r_chpl,
#line 367 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                         Error_chpl * error_out_chpl,
#line 367 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                         _tuple_2_range_int64_t_both_one_int64_t_chpl * _retArg_chpl,
#line 367 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                         int64_t _ln_chpl,
#line 367 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                         int32_t _fn_chpl) {
#line 367 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _tuple_2_range_int64_t_both_one_int64_t_chpl ret_chpl;
#line 406 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl6;
#line 406 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t ret_chpl2;
#line 406 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t call_tmp_chpl7;
#line 406 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl8;
#line 406 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl;
#line 406 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  byteIndex_chpl call_tmp_chpl9;
#line 406 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t coerce_tmp_chpl3;
#line 406 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  byteIndex_chpl ret_chpl3;
#line 406 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  byteIndex_chpl cpi_chpl;
#line 406 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t call_tmp_chpl10;
#line 406 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl_bool ret_chpl4;
#line 405 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl_bool tmp_chpl;
#line 407 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  byteIndex_chpl call_tmp_chpl11;
#line 407 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t coerce_tmp_chpl4;
#line 407 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  byteIndex_chpl ret_chpl5;
#line 407 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  byteIndex_chpl cpi_chpl2;
#line 407 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  uint8_t call_tmp_chpl12;
#line 410 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  byteIndex_chpl call_tmp_chpl13;
#line 410 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t coerce_tmp_chpl5;
#line 410 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  byteIndex_chpl ret_chpl6;
#line 410 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  byteIndex_chpl cpi_chpl3;
#line 410 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl call_tmp_chpl14;
#line 410 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl ret_tmp_chpl2;
#line 409 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl call_tmp_chpl15;
#line 409 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl ret_tmp_chpl3;
#line 409 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl call_tmp_chpl16;
#line 409 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl ret_tmp_chpl4;
#line 408 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _owned_CodepointSplitError_chpl call_tmp_chpl17;
#line 408 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  CodepointSplitError_chpl new_temp_chpl = NULL;
#line 408 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _owned_CodepointSplitError_chpl initTemp_chpl;
#line 408 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  Error_chpl call_tmp_chpl18 = NULL;
#line 408 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _owned_Error_chpl coerce_tmp_chpl6;
#line 408 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _owned_Error_chpl ret_tmp_chpl5;
#line 408 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref__owned_CodepointSplitError_chpl i_x_chpl = NULL;
#line 408 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref__owned_Error_chpl2 i__retArg_chpl = NULL;
#line 408 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _owned_CodepointSplitError_chpl _formal_tmp_x_chpl;
#line 408 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _owned_Error_chpl ret_chpl7;
#line 408 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref__owned_CodepointSplitError_chpl i_this_chpl = NULL;
#line 408 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  Error_chpl call_tmp_chpl19 = NULL;
#line 408 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref__owned_CodepointSplitError_chpl i_this_chpl2 = NULL;
#line 408 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _owned_Error_chpl call_tmp_chpl20;
#line 408 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _owned_Error_chpl initTemp_chpl2;
#line 408 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _owned_Error_chpl _formal_tmp_in_err_chpl;
#line 408 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref_string_chpl i_x_chpl2 = NULL;
#line 408 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref_string_chpl i_x_chpl3 = NULL;
#line 408 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref_string_chpl i_x_chpl4 = NULL;
#line 413 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _tuple_2_range_int64_t_both_one_int64_t_chpl call_tmp_chpl21;
#line 413 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _tuple_2_range_int64_t_both_one_int64_t_chpl ret_tmp_chpl6;
#line 406 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  ret_chpl2 = (x_chpl)->buffLen;
#line 406 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl7 = chpl__nudgeHighBound(ret_chpl2);
#line 406 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl_build_bounded_range6(INT64(0), call_tmp_chpl7, &ret_tmp_chpl);
#line 406 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl8 = ret_tmp_chpl;
#line 406 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl6 = call_tmp_chpl8;
#line 406 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  coerce_tmp_chpl3 = (&r_chpl)->_high;
#line 406 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  init_chpl192(&cpi_chpl);
#line 406 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  (&cpi_chpl)->_bindex = coerce_tmp_chpl3;
#line 406 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  ret_chpl3 = cpi_chpl;
#line 406 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl9 = ret_chpl3;
#line 406 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl10 = ((&call_tmp_chpl9)->_bindex + INT64(1));
#line 406 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  if (call_tmp_chpl10 > (&call_tmp_chpl6)->_high) /* ZLINE: 406 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 406 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  {
#line 406 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    ret_chpl4 = UINT8(false);
#line 406 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    goto _end_contains_chpl;
#line 406 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  }
#line 406 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  if (call_tmp_chpl10 < (&call_tmp_chpl6)->_low) /* ZLINE: 406 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 406 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  {
#line 406 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    ret_chpl4 = UINT8(false);
#line 406 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    goto _end_contains_chpl;
#line 406 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  }
#line 406 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  ret_chpl4 = UINT8(true);
#line 406 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _end_contains_chpl:;
#line 405 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  if (ret_chpl4) /* ZLINE: 405 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 405 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  {
#line 407 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    coerce_tmp_chpl4 = (&r_chpl)->_high;
#line 407 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    init_chpl192(&cpi_chpl2);
#line 407 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    (&cpi_chpl2)->_bindex = coerce_tmp_chpl4;
#line 407 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    ret_chpl5 = cpi_chpl2;
#line 407 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl11 = ret_chpl5;
#line 407 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl12 = byte_chpl(x_chpl, ((int64_t)(((&call_tmp_chpl11)->_bindex + INT64(1)))), _ln_chpl, _fn_chpl);
#line 405 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    tmp_chpl = (! ((call_tmp_chpl12 & ((uint8_t)(INT64(192)))) != ((uint8_t)(INT64(128)))));
#line 405 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  } else /* ZLINE: 405 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 405 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  {
#line 405 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    tmp_chpl = UINT8(false);
#line 405 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  }
#line 405 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  if (tmp_chpl) /* ZLINE: 407 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 405 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  {
#line 410 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    coerce_tmp_chpl5 = (&r_chpl)->_high;
#line 410 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    init_chpl192(&cpi_chpl3);
#line 410 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    (&cpi_chpl3)->_bindex = coerce_tmp_chpl5;
#line 410 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    ret_chpl6 = cpi_chpl3;
#line 410 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl13 = ret_chpl6;
#line 410 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    chpl___COLON_18(&call_tmp_chpl13, &ret_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 410 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl14 = ret_tmp_chpl2;
#line 409 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    chpl___PLUS_(&_str_literal_4044_chpl, &call_tmp_chpl14, &ret_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 409 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl15 = ret_tmp_chpl3;
#line 409 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    chpl___PLUS_(&call_tmp_chpl15, &_str_literal_1324_chpl, &ret_tmp_chpl4, _ln_chpl, _fn_chpl);
#line 409 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl16 = ret_tmp_chpl4;
#line 408 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    new_temp_chpl = _new_chpl60(&call_tmp_chpl16, _ln_chpl, _fn_chpl);
#line 408 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    init_chpl171(&initTemp_chpl, new_temp_chpl);
#line 408 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl17 = initTemp_chpl;
#line 408 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    i_x_chpl = &call_tmp_chpl17;
#line 408 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    i__retArg_chpl = &ret_tmp_chpl5;
#line 408 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    _formal_tmp_x_chpl = *(i_x_chpl);
#line 408 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    i_this_chpl = &_formal_tmp_x_chpl;
#line 408 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl19 = ((Error_chpl)((i_this_chpl)->chpl_p));
#line 408 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    i_this_chpl2 = &_formal_tmp_x_chpl;
#line 408 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    (i_this_chpl2)->chpl_p = ((CodepointSplitError_chpl)(nil));
#line 408 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    if (((RootClass_chpl)(call_tmp_chpl19)) == nil) /* ZLINE: 408 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 408 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    {
#line 408 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      nilCheckHalt_chpl(&_str_literal_52_chpl, _ln_chpl, _fn_chpl);
#line 408 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    }
#line 408 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    init_chpl188(&initTemp_chpl2, call_tmp_chpl19);
#line 408 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl20 = initTemp_chpl2;
#line 408 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    ret_chpl7 = call_tmp_chpl20;
#line 408 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    chpl__autoDestroy36(&_formal_tmp_x_chpl, _ln_chpl, _fn_chpl);
#line 408 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    *(i__retArg_chpl) = ret_chpl7;
#line 408 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    coerce_tmp_chpl6 = ret_tmp_chpl5;
#line 408 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    _formal_tmp_in_err_chpl = coerce_tmp_chpl6;
#line 408 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl18 = chpl_fix_thrown_error(&_formal_tmp_in_err_chpl, _ln_chpl, _fn_chpl);
#line 408 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    *(error_out_chpl) = call_tmp_chpl18;
#line 408 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    i_x_chpl2 = &call_tmp_chpl16;
#line 408 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    deinit_chpl95(i_x_chpl2, _ln_chpl, _fn_chpl);
#line 408 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    i_x_chpl3 = &call_tmp_chpl15;
#line 408 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    deinit_chpl95(i_x_chpl3, _ln_chpl, _fn_chpl);
#line 408 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    i_x_chpl4 = &call_tmp_chpl14;
#line 408 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    deinit_chpl95(i_x_chpl4, _ln_chpl, _fn_chpl);
#line 408 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    goto _end_getView_chpl;
#line 408 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  }
#line 413 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  simpleCaseHelper_chpl(x_chpl, r_chpl, &ret_tmp_chpl6, _ln_chpl, _fn_chpl);
#line 413 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl21 = ret_tmp_chpl6;
#line 413 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  ret_chpl = call_tmp_chpl21;
#line 413 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _end_getView_chpl:;
#line 367 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  *(_retArg_chpl) = ret_chpl;
#line 367 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  return;
#line 367 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
}

#line 373 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
/* BytesStringCommon.chpl:373 */
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
static void simpleCaseHelper_chpl(string_chpl * x_chpl,
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                                  range_byteIndex_high_one_chpl r_chpl,
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                                  _tuple_2_range_int64_t_both_one_int64_t_chpl * _retArg_chpl,
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                                  int64_t _ln_chpl,
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                                  int32_t _fn_chpl) {
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_high_one_chpl call_tmp_chpl6;
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_high_one_chpl ret_tmp_chpl;
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_high_one_chpl intR_chpl;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl7;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t ret_chpl;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t call_tmp_chpl8;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl9;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl2;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl boundedOther_chpl;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl tmp_chpl;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl initTemp_chpl;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  uint64_t call_tmp_chpl10;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl_bool tmp_chpl2;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl_bool tmp_chpl3;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl_bool ret_chpl2;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  uint64_t call_tmp_chpl11;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl_bool tmp_chpl4;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  uint64_t call_tmp_chpl12;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl_bool tmp_chpl5;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl13;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl3;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl slice_chpl;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl14;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl4;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl_bool call_tmp_chpl15;
#line 380 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t ret_chpl3;
#line 384 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t ret_chpl4;
#line 384 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t call_tmp_chpl16;
#line 384 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl17;
#line 384 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl5;
#line 384 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl18;
#line 384 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl6;
#line 384 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _tuple_2_range_int64_t_both_one_int64_t_chpl ret_tmp_chpl7;
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl___COLON_10(r_chpl, &ret_tmp_chpl);
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl6 = ret_tmp_chpl;
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  intR_chpl = call_tmp_chpl6;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  ret_chpl = (x_chpl)->buffLen;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl8 = chpl__nudgeHighBound(ret_chpl);
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl_build_bounded_range6(INT64(0), call_tmp_chpl8, &ret_tmp_chpl2);
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl9 = ret_tmp_chpl2;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl7 = call_tmp_chpl9;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  init_chpl115(&initTemp_chpl, (&call_tmp_chpl7)->_low, (&intR_chpl)->_high);
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  tmp_chpl = initTemp_chpl;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  boundedOther_chpl = tmp_chpl;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl10 = sizeAs_chpl36(tmp_chpl, _ln_chpl, _fn_chpl);
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  if (! (INT64(0) < INT64(0))) /* ZLINE: 378 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  {
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    tmp_chpl2 = (call_tmp_chpl10 == ((uint64_t)(INT64(0))));
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  } else /* ZLINE: 378 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  {
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    tmp_chpl2 = UINT8(false);
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  }
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  if (tmp_chpl2) /* ZLINE: 378 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  {
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    tmp_chpl3 = UINT8(true);
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  } else /* ZLINE: 378 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  {
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl11 = sizeAs_chpl36(call_tmp_chpl9, _ln_chpl, _fn_chpl);
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    if (! (INT64(0) < INT64(0))) /* ZLINE: 378 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    {
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      tmp_chpl4 = (call_tmp_chpl11 == ((uint64_t)(INT64(0))));
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    } else /* ZLINE: 378 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    {
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      tmp_chpl4 = UINT8(false);
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    }
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    if (tmp_chpl4) /* ZLINE: 378 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    {
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      call_tmp_chpl12 = sizeAs_chpl36(boundedOther_chpl, _ln_chpl, _fn_chpl);
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      if (! (INT64(0) < INT64(0))) /* ZLINE: 378 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      {
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        tmp_chpl5 = (call_tmp_chpl12 == ((uint64_t)(INT64(0))));
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      } else /* ZLINE: 378 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      {
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        tmp_chpl5 = UINT8(false);
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      }
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      ret_chpl2 = tmp_chpl5;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      goto _end_contains_chpl;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    }
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    chpl_slice5(call_tmp_chpl7, boundedOther_chpl, &ret_tmp_chpl3);
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl13 = ret_tmp_chpl3;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    slice_chpl = call_tmp_chpl13;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    adjustedSlice_chpl(&slice_chpl, &ret_tmp_chpl4);
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl14 = ret_tmp_chpl4;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl15 = chpl___EQUALS_2(boundedOther_chpl, call_tmp_chpl14, _ln_chpl, _fn_chpl);
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    ret_chpl2 = call_tmp_chpl15;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    _end_contains_chpl:;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    tmp_chpl3 = ret_chpl2;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  }
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  if (! tmp_chpl3) /* ZLINE: 378 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  {
#line 380 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    ret_chpl3 = (x_chpl)->buffLen;
#line 379 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    halt_chpl18(&_str_literal_1319_chpl, r_chpl, &_str_literal_263_chpl, ret_chpl3, _ln_chpl, _fn_chpl);
#line 379 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  }
#line 384 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  ret_chpl4 = (x_chpl)->buffLen;
#line 384 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl16 = chpl__nudgeHighBound(ret_chpl4);
#line 384 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl_build_bounded_range6(INT64(0), call_tmp_chpl16, &ret_tmp_chpl5);
#line 384 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl17 = ret_tmp_chpl5;
#line 384 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl_slice4(call_tmp_chpl6, call_tmp_chpl17, &ret_tmp_chpl6);
#line 384 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl18 = ret_tmp_chpl6;
#line 384 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  (&ret_tmp_chpl7)->x0 = call_tmp_chpl18;
#line 384 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  (&ret_tmp_chpl7)->x1 = INT64(-1);
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  *(_retArg_chpl) = ret_tmp_chpl7;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  return;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
}

#line 373 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
/* BytesStringCommon.chpl:373 */
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
static void simpleCaseHelper_chpl6(string_chpl * x_chpl,
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                                   range_int64_t_both_one_chpl r_chpl,
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                                   _tuple_2_range_int64_t_both_one_int64_t_chpl * _retArg_chpl,
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                                   int64_t _ln_chpl,
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                                   int32_t _fn_chpl) {
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl intR_chpl;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl6;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t ret_chpl;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t call_tmp_chpl7;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl8;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl boundedOther_chpl;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl tmp_chpl;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl initTemp_chpl;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  uint64_t call_tmp_chpl9;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl_bool tmp_chpl2;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl_bool tmp_chpl3;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl_bool ret_chpl2;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  uint64_t call_tmp_chpl10;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl_bool tmp_chpl4;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  uint64_t call_tmp_chpl11;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl_bool tmp_chpl5;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl12;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl2;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl slice_chpl;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl13;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl3;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl_bool call_tmp_chpl14;
#line 380 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t ret_chpl3;
#line 387 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t ret_chpl4;
#line 387 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t call_tmp_chpl15;
#line 387 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl16;
#line 387 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl4;
#line 387 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl17;
#line 387 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl5;
#line 387 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl retRange_chpl;
#line 388 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t call_tmp_chpl18;
#line 388 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _tuple_2_range_int64_t_both_one_int64_t_chpl ret_tmp_chpl6;
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  intR_chpl = r_chpl;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  ret_chpl = (x_chpl)->buffLen;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl7 = chpl__nudgeHighBound(ret_chpl);
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl_build_bounded_range6(INT64(0), call_tmp_chpl7, &ret_tmp_chpl);
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl8 = ret_tmp_chpl;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl6 = call_tmp_chpl8;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  init_chpl115(&initTemp_chpl, (&intR_chpl)->_low, (&intR_chpl)->_high);
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  tmp_chpl = initTemp_chpl;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  boundedOther_chpl = tmp_chpl;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl9 = sizeAs_chpl36(tmp_chpl, _ln_chpl, _fn_chpl);
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  if (! (INT64(0) < INT64(0))) /* ZLINE: 378 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  {
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    tmp_chpl2 = (call_tmp_chpl9 == ((uint64_t)(INT64(0))));
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  } else /* ZLINE: 378 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  {
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    tmp_chpl2 = UINT8(false);
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  }
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  if (tmp_chpl2) /* ZLINE: 378 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  {
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    tmp_chpl3 = UINT8(true);
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  } else /* ZLINE: 378 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  {
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl10 = sizeAs_chpl36(call_tmp_chpl8, _ln_chpl, _fn_chpl);
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    if (! (INT64(0) < INT64(0))) /* ZLINE: 378 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    {
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      tmp_chpl4 = (call_tmp_chpl10 == ((uint64_t)(INT64(0))));
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    } else /* ZLINE: 378 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    {
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      tmp_chpl4 = UINT8(false);
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    }
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    if (tmp_chpl4) /* ZLINE: 378 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    {
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      call_tmp_chpl11 = sizeAs_chpl36(boundedOther_chpl, _ln_chpl, _fn_chpl);
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      if (! (INT64(0) < INT64(0))) /* ZLINE: 378 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      {
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        tmp_chpl5 = (call_tmp_chpl11 == ((uint64_t)(INT64(0))));
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      } else /* ZLINE: 378 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      {
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        tmp_chpl5 = UINT8(false);
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      }
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      ret_chpl2 = tmp_chpl5;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      goto _end_contains_chpl;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    }
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    chpl_slice5(call_tmp_chpl6, boundedOther_chpl, &ret_tmp_chpl2);
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl12 = ret_tmp_chpl2;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    slice_chpl = call_tmp_chpl12;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    adjustedSlice_chpl(&slice_chpl, &ret_tmp_chpl3);
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl13 = ret_tmp_chpl3;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl14 = chpl___EQUALS_2(boundedOther_chpl, call_tmp_chpl13, _ln_chpl, _fn_chpl);
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    ret_chpl2 = call_tmp_chpl14;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    _end_contains_chpl:;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    tmp_chpl3 = ret_chpl2;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  }
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  if (! tmp_chpl3) /* ZLINE: 378 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  {
#line 380 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    ret_chpl3 = (x_chpl)->buffLen;
#line 379 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    halt_chpl7(&_str_literal_1319_chpl, r_chpl, &_str_literal_263_chpl, ret_chpl3, _ln_chpl, _fn_chpl);
#line 379 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  }
#line 387 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  ret_chpl4 = (x_chpl)->buffLen;
#line 387 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl15 = chpl__nudgeHighBound(ret_chpl4);
#line 387 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl_build_bounded_range6(INT64(0), call_tmp_chpl15, &ret_tmp_chpl4);
#line 387 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl16 = ret_tmp_chpl4;
#line 387 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl_slice3(r_chpl, call_tmp_chpl16, &ret_tmp_chpl5);
#line 387 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl17 = ret_tmp_chpl5;
#line 387 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  retRange_chpl = call_tmp_chpl17;
#line 388 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl18 = size_chpl8(call_tmp_chpl17, _ln_chpl, _fn_chpl);
#line 388 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  (&ret_tmp_chpl6)->x0 = retRange_chpl;
#line 388 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  (&ret_tmp_chpl6)->x1 = call_tmp_chpl18;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  *(_retArg_chpl) = ret_tmp_chpl6;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  return;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
}

#line 373 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
/* BytesStringCommon.chpl:373 */
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
static void simpleCaseHelper_chpl5(string_chpl * x_chpl,
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                                   range_byteIndex_both_one_chpl r_chpl,
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                                   _tuple_2_range_int64_t_both_one_int64_t_chpl * _retArg_chpl,
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                                   int64_t _ln_chpl,
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                                   int32_t _fn_chpl) {
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl6;
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl;
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl intR_chpl;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl7;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t ret_chpl;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t call_tmp_chpl8;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl9;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl2;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl boundedOther_chpl;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl tmp_chpl;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl initTemp_chpl;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  uint64_t call_tmp_chpl10;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl_bool tmp_chpl2;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl_bool tmp_chpl3;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl_bool ret_chpl2;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  uint64_t call_tmp_chpl11;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl_bool tmp_chpl4;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  uint64_t call_tmp_chpl12;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl_bool tmp_chpl5;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl13;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl3;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl slice_chpl;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl14;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl4;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl_bool call_tmp_chpl15;
#line 380 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t ret_chpl3;
#line 384 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t ret_chpl4;
#line 384 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t call_tmp_chpl16;
#line 384 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl17;
#line 384 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl5;
#line 384 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl18;
#line 384 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl6;
#line 384 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _tuple_2_range_int64_t_both_one_int64_t_chpl ret_tmp_chpl7;
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl___COLON_11(r_chpl, &ret_tmp_chpl);
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl6 = ret_tmp_chpl;
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  intR_chpl = call_tmp_chpl6;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  ret_chpl = (x_chpl)->buffLen;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl8 = chpl__nudgeHighBound(ret_chpl);
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl_build_bounded_range6(INT64(0), call_tmp_chpl8, &ret_tmp_chpl2);
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl9 = ret_tmp_chpl2;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl7 = call_tmp_chpl9;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  init_chpl115(&initTemp_chpl, (&intR_chpl)->_low, (&intR_chpl)->_high);
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  tmp_chpl = initTemp_chpl;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  boundedOther_chpl = tmp_chpl;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl10 = sizeAs_chpl36(tmp_chpl, _ln_chpl, _fn_chpl);
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  if (! (INT64(0) < INT64(0))) /* ZLINE: 378 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  {
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    tmp_chpl2 = (call_tmp_chpl10 == ((uint64_t)(INT64(0))));
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  } else /* ZLINE: 378 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  {
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    tmp_chpl2 = UINT8(false);
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  }
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  if (tmp_chpl2) /* ZLINE: 378 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  {
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    tmp_chpl3 = UINT8(true);
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  } else /* ZLINE: 378 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  {
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl11 = sizeAs_chpl36(call_tmp_chpl9, _ln_chpl, _fn_chpl);
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    if (! (INT64(0) < INT64(0))) /* ZLINE: 378 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    {
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      tmp_chpl4 = (call_tmp_chpl11 == ((uint64_t)(INT64(0))));
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    } else /* ZLINE: 378 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    {
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      tmp_chpl4 = UINT8(false);
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    }
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    if (tmp_chpl4) /* ZLINE: 378 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    {
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      call_tmp_chpl12 = sizeAs_chpl36(boundedOther_chpl, _ln_chpl, _fn_chpl);
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      if (! (INT64(0) < INT64(0))) /* ZLINE: 378 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      {
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        tmp_chpl5 = (call_tmp_chpl12 == ((uint64_t)(INT64(0))));
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      } else /* ZLINE: 378 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      {
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        tmp_chpl5 = UINT8(false);
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      }
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      ret_chpl2 = tmp_chpl5;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      goto _end_contains_chpl;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    }
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    chpl_slice5(call_tmp_chpl7, boundedOther_chpl, &ret_tmp_chpl3);
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl13 = ret_tmp_chpl3;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    slice_chpl = call_tmp_chpl13;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    adjustedSlice_chpl(&slice_chpl, &ret_tmp_chpl4);
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl14 = ret_tmp_chpl4;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl15 = chpl___EQUALS_2(boundedOther_chpl, call_tmp_chpl14, _ln_chpl, _fn_chpl);
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    ret_chpl2 = call_tmp_chpl15;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    _end_contains_chpl:;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    tmp_chpl3 = ret_chpl2;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  }
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  if (! tmp_chpl3) /* ZLINE: 378 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  {
#line 380 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    ret_chpl3 = (x_chpl)->buffLen;
#line 379 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    halt_chpl21(&_str_literal_1319_chpl, r_chpl, &_str_literal_263_chpl, ret_chpl3, _ln_chpl, _fn_chpl);
#line 379 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  }
#line 384 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  ret_chpl4 = (x_chpl)->buffLen;
#line 384 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl16 = chpl__nudgeHighBound(ret_chpl4);
#line 384 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl_build_bounded_range6(INT64(0), call_tmp_chpl16, &ret_tmp_chpl5);
#line 384 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl17 = ret_tmp_chpl5;
#line 384 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl_slice3(call_tmp_chpl6, call_tmp_chpl17, &ret_tmp_chpl6);
#line 384 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl18 = ret_tmp_chpl6;
#line 384 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  (&ret_tmp_chpl7)->x0 = call_tmp_chpl18;
#line 384 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  (&ret_tmp_chpl7)->x1 = INT64(-1);
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  *(_retArg_chpl) = ret_tmp_chpl7;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  return;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
}

#line 373 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
/* BytesStringCommon.chpl:373 */
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
static void simpleCaseHelper_chpl4(string_chpl * x_chpl,
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                                   range_int64_t_low_one_chpl r_chpl,
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                                   _tuple_2_range_int64_t_both_one_int64_t_chpl * _retArg_chpl,
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                                   int64_t _ln_chpl,
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                                   int32_t _fn_chpl) {
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_low_one_chpl intR_chpl;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl6;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t ret_chpl;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t call_tmp_chpl7;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl8;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl boundedOther_chpl;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl tmp_chpl;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl initTemp_chpl;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  uint64_t call_tmp_chpl9;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl_bool tmp_chpl2;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl_bool tmp_chpl3;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl_bool ret_chpl2;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  uint64_t call_tmp_chpl10;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl_bool tmp_chpl4;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  uint64_t call_tmp_chpl11;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl_bool tmp_chpl5;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl12;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl2;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl slice_chpl;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl13;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl3;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl_bool call_tmp_chpl14;
#line 380 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t ret_chpl3;
#line 387 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t ret_chpl4;
#line 387 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t call_tmp_chpl15;
#line 387 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl16;
#line 387 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl4;
#line 387 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl17;
#line 387 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl5;
#line 387 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl retRange_chpl;
#line 388 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t call_tmp_chpl18;
#line 388 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _tuple_2_range_int64_t_both_one_int64_t_chpl ret_tmp_chpl6;
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  intR_chpl = r_chpl;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  ret_chpl = (x_chpl)->buffLen;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl7 = chpl__nudgeHighBound(ret_chpl);
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl_build_bounded_range6(INT64(0), call_tmp_chpl7, &ret_tmp_chpl);
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl8 = ret_tmp_chpl;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl6 = call_tmp_chpl8;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  init_chpl115(&initTemp_chpl, (&intR_chpl)->_low, (&call_tmp_chpl6)->_high);
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  tmp_chpl = initTemp_chpl;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  boundedOther_chpl = tmp_chpl;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl9 = sizeAs_chpl36(tmp_chpl, _ln_chpl, _fn_chpl);
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  if (! (INT64(0) < INT64(0))) /* ZLINE: 378 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  {
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    tmp_chpl2 = (call_tmp_chpl9 == ((uint64_t)(INT64(0))));
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  } else /* ZLINE: 378 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  {
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    tmp_chpl2 = UINT8(false);
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  }
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  if (tmp_chpl2) /* ZLINE: 378 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  {
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    tmp_chpl3 = UINT8(true);
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  } else /* ZLINE: 378 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  {
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl10 = sizeAs_chpl36(call_tmp_chpl8, _ln_chpl, _fn_chpl);
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    if (! (INT64(0) < INT64(0))) /* ZLINE: 378 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    {
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      tmp_chpl4 = (call_tmp_chpl10 == ((uint64_t)(INT64(0))));
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    } else /* ZLINE: 378 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    {
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      tmp_chpl4 = UINT8(false);
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    }
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    if (tmp_chpl4) /* ZLINE: 378 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    {
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      call_tmp_chpl11 = sizeAs_chpl36(boundedOther_chpl, _ln_chpl, _fn_chpl);
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      if (! (INT64(0) < INT64(0))) /* ZLINE: 378 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      {
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        tmp_chpl5 = (call_tmp_chpl11 == ((uint64_t)(INT64(0))));
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      } else /* ZLINE: 378 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      {
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        tmp_chpl5 = UINT8(false);
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      }
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      ret_chpl2 = tmp_chpl5;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      goto _end_contains_chpl;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    }
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    chpl_slice5(call_tmp_chpl6, boundedOther_chpl, &ret_tmp_chpl2);
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl12 = ret_tmp_chpl2;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    slice_chpl = call_tmp_chpl12;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    adjustedSlice_chpl(&slice_chpl, &ret_tmp_chpl3);
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl13 = ret_tmp_chpl3;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl14 = chpl___EQUALS_2(boundedOther_chpl, call_tmp_chpl13, _ln_chpl, _fn_chpl);
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    ret_chpl2 = call_tmp_chpl14;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    _end_contains_chpl:;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    tmp_chpl3 = ret_chpl2;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  }
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  if (! tmp_chpl3) /* ZLINE: 378 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  {
#line 380 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    ret_chpl3 = (x_chpl)->buffLen;
#line 379 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    halt_chpl6(&_str_literal_1319_chpl, r_chpl, &_str_literal_263_chpl, ret_chpl3, _ln_chpl, _fn_chpl);
#line 379 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  }
#line 387 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  ret_chpl4 = (x_chpl)->buffLen;
#line 387 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl15 = chpl__nudgeHighBound(ret_chpl4);
#line 387 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl_build_bounded_range6(INT64(0), call_tmp_chpl15, &ret_tmp_chpl4);
#line 387 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl16 = ret_tmp_chpl4;
#line 387 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl_slice(r_chpl, call_tmp_chpl16, &ret_tmp_chpl5);
#line 387 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl17 = ret_tmp_chpl5;
#line 387 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  retRange_chpl = call_tmp_chpl17;
#line 388 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl18 = size_chpl8(call_tmp_chpl17, _ln_chpl, _fn_chpl);
#line 388 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  (&ret_tmp_chpl6)->x0 = retRange_chpl;
#line 388 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  (&ret_tmp_chpl6)->x1 = call_tmp_chpl18;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  *(_retArg_chpl) = ret_tmp_chpl6;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  return;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
}

#line 373 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
/* BytesStringCommon.chpl:373 */
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
static void simpleCaseHelper_chpl3(string_chpl * x_chpl,
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                                   range_byteIndex_low_one_chpl r_chpl,
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                                   _tuple_2_range_int64_t_both_one_int64_t_chpl * _retArg_chpl,
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                                   int64_t _ln_chpl,
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                                   int32_t _fn_chpl) {
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_low_one_chpl call_tmp_chpl6;
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_low_one_chpl ret_tmp_chpl;
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_low_one_chpl intR_chpl;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl7;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t ret_chpl;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t call_tmp_chpl8;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl9;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl2;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl boundedOther_chpl;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl tmp_chpl;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl initTemp_chpl;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  uint64_t call_tmp_chpl10;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl_bool tmp_chpl2;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl_bool tmp_chpl3;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl_bool ret_chpl2;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  uint64_t call_tmp_chpl11;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl_bool tmp_chpl4;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  uint64_t call_tmp_chpl12;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl_bool tmp_chpl5;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl13;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl3;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl slice_chpl;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl14;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl4;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl_bool call_tmp_chpl15;
#line 380 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t ret_chpl3;
#line 384 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t ret_chpl4;
#line 384 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t call_tmp_chpl16;
#line 384 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl17;
#line 384 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl5;
#line 384 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl18;
#line 384 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl6;
#line 384 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _tuple_2_range_int64_t_both_one_int64_t_chpl ret_tmp_chpl7;
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl___COLON_9(r_chpl, &ret_tmp_chpl);
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl6 = ret_tmp_chpl;
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  intR_chpl = call_tmp_chpl6;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  ret_chpl = (x_chpl)->buffLen;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl8 = chpl__nudgeHighBound(ret_chpl);
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl_build_bounded_range6(INT64(0), call_tmp_chpl8, &ret_tmp_chpl2);
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl9 = ret_tmp_chpl2;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl7 = call_tmp_chpl9;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  init_chpl115(&initTemp_chpl, (&intR_chpl)->_low, (&call_tmp_chpl7)->_high);
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  tmp_chpl = initTemp_chpl;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  boundedOther_chpl = tmp_chpl;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl10 = sizeAs_chpl36(tmp_chpl, _ln_chpl, _fn_chpl);
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  if (! (INT64(0) < INT64(0))) /* ZLINE: 378 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  {
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    tmp_chpl2 = (call_tmp_chpl10 == ((uint64_t)(INT64(0))));
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  } else /* ZLINE: 378 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  {
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    tmp_chpl2 = UINT8(false);
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  }
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  if (tmp_chpl2) /* ZLINE: 378 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  {
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    tmp_chpl3 = UINT8(true);
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  } else /* ZLINE: 378 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  {
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl11 = sizeAs_chpl36(call_tmp_chpl9, _ln_chpl, _fn_chpl);
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    if (! (INT64(0) < INT64(0))) /* ZLINE: 378 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    {
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      tmp_chpl4 = (call_tmp_chpl11 == ((uint64_t)(INT64(0))));
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    } else /* ZLINE: 378 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    {
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      tmp_chpl4 = UINT8(false);
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    }
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    if (tmp_chpl4) /* ZLINE: 378 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    {
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      call_tmp_chpl12 = sizeAs_chpl36(boundedOther_chpl, _ln_chpl, _fn_chpl);
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      if (! (INT64(0) < INT64(0))) /* ZLINE: 378 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      {
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        tmp_chpl5 = (call_tmp_chpl12 == ((uint64_t)(INT64(0))));
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      } else /* ZLINE: 378 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      {
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        tmp_chpl5 = UINT8(false);
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      }
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      ret_chpl2 = tmp_chpl5;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      goto _end_contains_chpl;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    }
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    chpl_slice5(call_tmp_chpl7, boundedOther_chpl, &ret_tmp_chpl3);
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl13 = ret_tmp_chpl3;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    slice_chpl = call_tmp_chpl13;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    adjustedSlice_chpl(&slice_chpl, &ret_tmp_chpl4);
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl14 = ret_tmp_chpl4;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl15 = chpl___EQUALS_2(boundedOther_chpl, call_tmp_chpl14, _ln_chpl, _fn_chpl);
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    ret_chpl2 = call_tmp_chpl15;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    _end_contains_chpl:;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    tmp_chpl3 = ret_chpl2;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  }
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  if (! tmp_chpl3) /* ZLINE: 378 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  {
#line 380 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    ret_chpl3 = (x_chpl)->buffLen;
#line 379 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    halt_chpl13(&_str_literal_1319_chpl, r_chpl, &_str_literal_263_chpl, ret_chpl3, _ln_chpl, _fn_chpl);
#line 379 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  }
#line 384 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  ret_chpl4 = (x_chpl)->buffLen;
#line 384 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl16 = chpl__nudgeHighBound(ret_chpl4);
#line 384 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl_build_bounded_range6(INT64(0), call_tmp_chpl16, &ret_tmp_chpl5);
#line 384 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl17 = ret_tmp_chpl5;
#line 384 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl_slice(call_tmp_chpl6, call_tmp_chpl17, &ret_tmp_chpl6);
#line 384 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl18 = ret_tmp_chpl6;
#line 384 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  (&ret_tmp_chpl7)->x0 = call_tmp_chpl18;
#line 384 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  (&ret_tmp_chpl7)->x1 = INT64(-1);
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  *(_retArg_chpl) = ret_tmp_chpl7;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  return;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
}

#line 373 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
/* BytesStringCommon.chpl:373 */
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
static void simpleCaseHelper_chpl2(string_chpl * x_chpl,
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                                   range_byteIndex_low_one_chpl r_chpl,
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                                   _tuple_2_range_int64_t_both_one_int64_t_chpl * _retArg_chpl,
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                                   int64_t _ln_chpl,
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                                   int32_t _fn_chpl) {
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_low_one_chpl call_tmp_chpl6;
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_low_one_chpl ret_tmp_chpl;
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_low_one_chpl intR_chpl;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl7;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t ret_chpl;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t call_tmp_chpl8;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl9;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl2;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl boundedOther_chpl;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl tmp_chpl;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl initTemp_chpl;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  uint64_t call_tmp_chpl10;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl_bool tmp_chpl2;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl_bool tmp_chpl3;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl_bool ret_chpl2;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  uint64_t call_tmp_chpl11;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl_bool tmp_chpl4;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  uint64_t call_tmp_chpl12;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl_bool tmp_chpl5;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl13;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl3;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl slice_chpl;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl14;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl4;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl_bool call_tmp_chpl15;
#line 380 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t ret_chpl3;
#line 384 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t ret_chpl4;
#line 384 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t call_tmp_chpl16;
#line 384 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl17;
#line 384 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl5;
#line 384 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl18;
#line 384 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl6;
#line 384 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _tuple_2_range_int64_t_both_one_int64_t_chpl ret_tmp_chpl7;
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl___COLON_9(r_chpl, &ret_tmp_chpl);
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl6 = ret_tmp_chpl;
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  intR_chpl = call_tmp_chpl6;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  ret_chpl = (x_chpl)->buffLen;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl8 = chpl__nudgeHighBound(ret_chpl);
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl_build_bounded_range6(INT64(0), call_tmp_chpl8, &ret_tmp_chpl2);
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl9 = ret_tmp_chpl2;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl7 = call_tmp_chpl9;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  init_chpl115(&initTemp_chpl, (&intR_chpl)->_low, (&call_tmp_chpl7)->_high);
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  tmp_chpl = initTemp_chpl;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  boundedOther_chpl = tmp_chpl;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl10 = sizeAs_chpl36(tmp_chpl, _ln_chpl, _fn_chpl);
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  if (! (INT64(0) < INT64(0))) /* ZLINE: 378 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  {
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    tmp_chpl2 = (call_tmp_chpl10 == ((uint64_t)(INT64(0))));
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  } else /* ZLINE: 378 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  {
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    tmp_chpl2 = UINT8(false);
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  }
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  if (tmp_chpl2) /* ZLINE: 378 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  {
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    tmp_chpl3 = UINT8(true);
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  } else /* ZLINE: 378 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  {
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl11 = sizeAs_chpl36(call_tmp_chpl9, _ln_chpl, _fn_chpl);
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    if (! (INT64(0) < INT64(0))) /* ZLINE: 378 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    {
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      tmp_chpl4 = (call_tmp_chpl11 == ((uint64_t)(INT64(0))));
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    } else /* ZLINE: 378 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    {
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      tmp_chpl4 = UINT8(false);
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    }
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    if (tmp_chpl4) /* ZLINE: 378 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    {
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      call_tmp_chpl12 = sizeAs_chpl36(boundedOther_chpl, _ln_chpl, _fn_chpl);
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      if (! (INT64(0) < INT64(0))) /* ZLINE: 378 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      {
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        tmp_chpl5 = (call_tmp_chpl12 == ((uint64_t)(INT64(0))));
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      } else /* ZLINE: 378 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      {
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        tmp_chpl5 = UINT8(false);
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      }
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      ret_chpl2 = tmp_chpl5;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      goto _end_contains_chpl;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    }
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    chpl_slice5(call_tmp_chpl7, boundedOther_chpl, &ret_tmp_chpl3);
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl13 = ret_tmp_chpl3;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    slice_chpl = call_tmp_chpl13;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    adjustedSlice_chpl(&slice_chpl, &ret_tmp_chpl4);
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl14 = ret_tmp_chpl4;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl15 = chpl___EQUALS_2(boundedOther_chpl, call_tmp_chpl14, _ln_chpl, _fn_chpl);
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    ret_chpl2 = call_tmp_chpl15;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    _end_contains_chpl:;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    tmp_chpl3 = ret_chpl2;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  }
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  if (! tmp_chpl3) /* ZLINE: 378 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  {
#line 380 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    ret_chpl3 = (x_chpl)->buffLen;
#line 379 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    halt_chpl13(&_str_literal_1319_chpl, r_chpl, &_str_literal_263_chpl, ret_chpl3, _ln_chpl, _fn_chpl);
#line 379 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  }
#line 384 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  ret_chpl4 = (x_chpl)->buffLen;
#line 384 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl16 = chpl__nudgeHighBound(ret_chpl4);
#line 384 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl_build_bounded_range6(INT64(0), call_tmp_chpl16, &ret_tmp_chpl5);
#line 384 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl17 = ret_tmp_chpl5;
#line 384 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl_slice(call_tmp_chpl6, call_tmp_chpl17, &ret_tmp_chpl6);
#line 384 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl18 = ret_tmp_chpl6;
#line 384 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  (&ret_tmp_chpl7)->x0 = call_tmp_chpl18;
#line 384 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  (&ret_tmp_chpl7)->x1 = INT64(-1);
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  *(_retArg_chpl) = ret_tmp_chpl7;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  return;
#line 373 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
}

#line 470 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
/* BytesStringCommon.chpl:470 */
#line 470 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
static void getSlice_chpl3(string_chpl * x_chpl,
#line 470 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                           range_byteIndex_both_one_chpl r_chpl,
#line 470 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                           Error_chpl * error_out_chpl,
#line 470 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                           string_chpl * _retArg_chpl,
#line 470 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                           int64_t _ln_chpl,
#line 470 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                           int32_t _fn_chpl) {
#line 470 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl ret_chpl;
#line 474 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl ret_chpl2;
#line 478 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _tuple_2_range_int64_t_both_one_int64_t_chpl call_tmp_chpl6;
#line 478 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  Error_chpl error_chpl = NULL;
#line 478 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _tuple_2_range_int64_t_both_one_int64_t_chpl ret_tmp_chpl;
#line 478 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl tmp_x0_chpl;
#line 478 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t tmp_x1_chpl;
#line 478 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl r2_chpl;
#line 479 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t call_tmp_chpl7;
#line 480 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl ret_chpl3;
#line 484 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  c_ptr_uint8_t_chpl buff_chpl = NULL;
#line 491 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t call_tmp_chpl8;
#line 491 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t copyLen_chpl;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _tuple_2_c_ptr_uint8_t_int64_t_chpl call_tmp_chpl9;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _tuple_2_c_ptr_uint8_t_int64_t_chpl ret_chpl4;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  c_ptr_uint8_t_chpl call_tmp_chpl10 = NULL;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t call_tmp_chpl11;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t tmp_chpl;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int16_t call_tmp_chpl12;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  c_ptr_void_chpl cast_tmp_chpl = NULL;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  c_ptr_uint8_t_chpl call_tmp_chpl13 = NULL;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  c_ptr_uint8_t_chpl dst_chpl = NULL;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  c_ptr_uint8_t_chpl call_tmp_chpl14 = NULL;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  c_ptr_uint8_t_chpl call_tmp_chpl15 = NULL;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _owned_IllegalArgumentError_chpl2 call_tmp_chpl16;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _owned_IllegalArgumentError_chpl2 ret_tmp_chpl2;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref__owned_IllegalArgumentError_chpl i_arg_chpl = NULL;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  IllegalArgumentError_chpl call_tmp_chpl17 = NULL;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl call_tmp_chpl18;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  Error_chpl coerce_tmp_chpl3 = NULL;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl ret_tmp_chpl3;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref_uint8_t_chpl call_tmp_chpl19 = NULL;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  c_ptr_uint8_t_chpl tmp_x0_chpl2 = NULL;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t tmp_x1_chpl2;
#line 508 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t call_tmp_chpl20;
#line 509 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref_uint8_t_chpl call_tmp_chpl21 = NULL;
#line 512 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t numCodepoints_chpl;
#line 514 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t call_tmp_chpl22;
#line 516 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl ret_chpl5;
#line 516 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl ret_chpl6;
#line 516 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref_string_chpl i_x_chpl2 = NULL;
#line 473 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  if ((x_chpl)->buffLen == INT64(0)) /* ZLINE: 473 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 473 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  {
#line 474 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    init_chpl194(&ret_chpl2, _ln_chpl, _fn_chpl);
#line 475 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    ret_chpl = ret_chpl2;
#line 475 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    goto _end_getSlice_chpl;
#line 475 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  }
#line 478 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  error_chpl = NULL;
#line 478 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  getView_chpl5(x_chpl, r_chpl, &error_chpl, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 478 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl6 = ret_tmp_chpl;
#line 478 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  if (error_chpl != nil) /* ZLINE: 478 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 478 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  {
#line 478 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    *(error_out_chpl) = error_chpl;
#line 478 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    goto _end_getSlice_chpl;
#line 478 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  }
#line 478 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  tmp_x0_chpl = (&call_tmp_chpl6)->x0;
#line 478 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  tmp_x1_chpl = (&call_tmp_chpl6)->x1;
#line 478 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  r2_chpl = tmp_x0_chpl;
#line 479 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl7 = size_chpl8(tmp_x0_chpl, _ln_chpl, _fn_chpl);
#line 479 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  if (call_tmp_chpl7 <= INT64(0)) /* ZLINE: 479 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 479 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  {
#line 480 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    init_chpl194(&ret_chpl3, _ln_chpl, _fn_chpl);
#line 481 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    ret_chpl = ret_chpl3;
#line 481 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    goto _end_getSlice_chpl;
#line 481 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  }
#line 491 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl8 = (((int64_t)(((&r2_chpl)->_high - (&r2_chpl)->_low))) + INT64(1));
#line 491 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  copyLen_chpl = call_tmp_chpl8;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl10 = ((x_chpl)->buff + (&r2_chpl)->_low);
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl11 = chpl_here_good_alloc_size(((int64_t)((call_tmp_chpl8 + INT64(1)))), _ln_chpl, _fn_chpl);
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  if (call_tmp_chpl11 > INT64(0)) /* ZLINE: 492 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  {
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    tmp_chpl = call_tmp_chpl11;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  } else /* ZLINE: 492 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  {
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    tmp_chpl = INT64(0);
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  }
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl12 = offset_STR_COPY_DATA_chpl();
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  cast_tmp_chpl = chpl_here_alloc(tmp_chpl, call_tmp_chpl12, _ln_chpl, _fn_chpl);
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl13 = ((c_ptr_uint8_t_chpl)(cast_tmp_chpl));
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  dst_chpl = call_tmp_chpl13;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl14 = (call_tmp_chpl13 + INT64(0));
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl15 = (call_tmp_chpl10 + INT64(0));
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl_checkValue7(call_tmp_chpl8, &ret_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl16 = ret_tmp_chpl2;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  i_arg_chpl = &call_tmp_chpl16;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl17 = borrow_chpl2(i_arg_chpl);
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  if (((RootClass_chpl)(call_tmp_chpl17)) != nil) /* ZLINE: 492 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  {
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    coerce_tmp_chpl3 = ((Error_chpl)(call_tmp_chpl17));
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    chpl_check_nil(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    ((void(*)(Error_chpl,
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
       string_chpl *,
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
       int64_t,
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
       int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(coerce_tmp_chpl3))->chpl__cid) + INT64(1))])(coerce_tmp_chpl3, &ret_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl18 = ret_tmp_chpl3;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    safeCastCheckHalt_chpl(&call_tmp_chpl18, _ln_chpl, _fn_chpl);
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    i_x_chpl = &call_tmp_chpl18;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    deinit_chpl95(i_x_chpl, _ln_chpl, _fn_chpl);
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  }
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl__autoDestroy62(&call_tmp_chpl16, _ln_chpl, _fn_chpl);
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  memcpy(((c_ptr_void_chpl)(call_tmp_chpl14)), ((c_ptr_void_chpl)(call_tmp_chpl15)), ((uint64_t)(copyLen_chpl)));
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl19 = (dst_chpl + copyLen_chpl);
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  *(call_tmp_chpl19) = ((uint8_t)(INT64(0)));
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  (&ret_chpl4)->x0 = call_tmp_chpl13;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  (&ret_chpl4)->x1 = tmp_chpl;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl9 = ret_chpl4;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  tmp_x0_chpl2 = (&call_tmp_chpl9)->x0;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  tmp_x1_chpl2 = (&call_tmp_chpl9)->x1;
#line 495 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  buff_chpl = tmp_x0_chpl2;
#line 508 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl20 = size_chpl8(r2_chpl, _ln_chpl, _fn_chpl);
#line 509 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl21 = (buff_chpl + call_tmp_chpl20);
#line 509 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  *(call_tmp_chpl21) = ((uint8_t)(INT64(0)));
#line 512 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  numCodepoints_chpl = tmp_x1_chpl;
#line 513 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  if (tmp_x1_chpl == INT64(-1)) /* ZLINE: 513 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 513 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  {
#line 514 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl22 = countNumCodepoints_chpl(tmp_x0_chpl2, call_tmp_chpl20, _ln_chpl, _fn_chpl);
#line 514 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    numCodepoints_chpl = call_tmp_chpl22;
#line 514 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  }
#line 516 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  init_chpl194(&ret_chpl6, _ln_chpl, _fn_chpl);
#line 516 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  i_x_chpl2 = &ret_chpl6;
#line 516 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  (i_x_chpl2)->isOwned = UINT8(true);
#line 516 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  (i_x_chpl2)->buff = tmp_x0_chpl2;
#line 516 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  (i_x_chpl2)->buffSize = tmp_x1_chpl2;
#line 516 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  (i_x_chpl2)->buffLen = call_tmp_chpl20;
#line 516 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  (&ret_chpl6)->cachedNumCodepoints = numCodepoints_chpl;
#line 516 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  ret_chpl5 = ret_chpl6;
#line 516 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  ret_chpl = ret_chpl5;
#line 516 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _end_getSlice_chpl:;
#line 470 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  *(_retArg_chpl) = ret_chpl;
#line 470 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  return;
#line 470 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
}

#line 470 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
/* BytesStringCommon.chpl:470 */
#line 470 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
static void getSlice_chpl4(string_chpl * x_chpl,
#line 470 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                           range_int64_t_low_one_chpl r_chpl,
#line 470 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                           Error_chpl * error_out_chpl,
#line 470 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                           string_chpl * _retArg_chpl,
#line 470 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                           int64_t _ln_chpl,
#line 470 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                           int32_t _fn_chpl) {
#line 470 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl ret_chpl;
#line 474 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl ret_chpl2;
#line 478 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _tuple_2_range_int64_t_both_one_int64_t_chpl call_tmp_chpl6;
#line 478 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  Error_chpl error_chpl = NULL;
#line 478 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _tuple_2_range_int64_t_both_one_int64_t_chpl ret_tmp_chpl;
#line 478 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl tmp_x0_chpl;
#line 478 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t tmp_x1_chpl;
#line 478 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl r2_chpl;
#line 479 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t call_tmp_chpl7;
#line 480 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl ret_chpl3;
#line 484 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  c_ptr_uint8_t_chpl buff_chpl = NULL;
#line 491 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t call_tmp_chpl8;
#line 491 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t copyLen_chpl;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _tuple_2_c_ptr_uint8_t_int64_t_chpl call_tmp_chpl9;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _tuple_2_c_ptr_uint8_t_int64_t_chpl ret_chpl4;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  c_ptr_uint8_t_chpl call_tmp_chpl10 = NULL;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t call_tmp_chpl11;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t tmp_chpl;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int16_t call_tmp_chpl12;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  c_ptr_void_chpl cast_tmp_chpl = NULL;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  c_ptr_uint8_t_chpl call_tmp_chpl13 = NULL;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  c_ptr_uint8_t_chpl dst_chpl = NULL;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  c_ptr_uint8_t_chpl call_tmp_chpl14 = NULL;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  c_ptr_uint8_t_chpl call_tmp_chpl15 = NULL;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _owned_IllegalArgumentError_chpl2 call_tmp_chpl16;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _owned_IllegalArgumentError_chpl2 ret_tmp_chpl2;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref__owned_IllegalArgumentError_chpl i_arg_chpl = NULL;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  IllegalArgumentError_chpl call_tmp_chpl17 = NULL;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl call_tmp_chpl18;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  Error_chpl coerce_tmp_chpl3 = NULL;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl ret_tmp_chpl3;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref_uint8_t_chpl call_tmp_chpl19 = NULL;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  c_ptr_uint8_t_chpl tmp_x0_chpl2 = NULL;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t tmp_x1_chpl2;
#line 508 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t call_tmp_chpl20;
#line 509 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref_uint8_t_chpl call_tmp_chpl21 = NULL;
#line 512 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t numCodepoints_chpl;
#line 514 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t call_tmp_chpl22;
#line 516 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl ret_chpl5;
#line 516 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl ret_chpl6;
#line 516 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref_string_chpl i_x_chpl2 = NULL;
#line 473 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  if ((x_chpl)->buffLen == INT64(0)) /* ZLINE: 473 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 473 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  {
#line 474 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    init_chpl194(&ret_chpl2, _ln_chpl, _fn_chpl);
#line 475 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    ret_chpl = ret_chpl2;
#line 475 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    goto _end_getSlice_chpl;
#line 475 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  }
#line 478 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  error_chpl = NULL;
#line 478 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  getView_chpl4(x_chpl, r_chpl, &error_chpl, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 478 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl6 = ret_tmp_chpl;
#line 478 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  if (error_chpl != nil) /* ZLINE: 478 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 478 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  {
#line 478 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    *(error_out_chpl) = error_chpl;
#line 478 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    goto _end_getSlice_chpl;
#line 478 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  }
#line 478 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  tmp_x0_chpl = (&call_tmp_chpl6)->x0;
#line 478 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  tmp_x1_chpl = (&call_tmp_chpl6)->x1;
#line 478 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  r2_chpl = tmp_x0_chpl;
#line 479 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl7 = size_chpl8(tmp_x0_chpl, _ln_chpl, _fn_chpl);
#line 479 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  if (call_tmp_chpl7 <= INT64(0)) /* ZLINE: 479 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 479 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  {
#line 480 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    init_chpl194(&ret_chpl3, _ln_chpl, _fn_chpl);
#line 481 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    ret_chpl = ret_chpl3;
#line 481 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    goto _end_getSlice_chpl;
#line 481 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  }
#line 491 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl8 = (((int64_t)(((&r2_chpl)->_high - (&r2_chpl)->_low))) + INT64(1));
#line 491 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  copyLen_chpl = call_tmp_chpl8;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl10 = ((x_chpl)->buff + (&r2_chpl)->_low);
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl11 = chpl_here_good_alloc_size(((int64_t)((call_tmp_chpl8 + INT64(1)))), _ln_chpl, _fn_chpl);
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  if (call_tmp_chpl11 > INT64(0)) /* ZLINE: 492 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  {
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    tmp_chpl = call_tmp_chpl11;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  } else /* ZLINE: 492 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  {
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    tmp_chpl = INT64(0);
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  }
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl12 = offset_STR_COPY_DATA_chpl();
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  cast_tmp_chpl = chpl_here_alloc(tmp_chpl, call_tmp_chpl12, _ln_chpl, _fn_chpl);
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl13 = ((c_ptr_uint8_t_chpl)(cast_tmp_chpl));
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  dst_chpl = call_tmp_chpl13;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl14 = (call_tmp_chpl13 + INT64(0));
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl15 = (call_tmp_chpl10 + INT64(0));
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl_checkValue7(call_tmp_chpl8, &ret_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl16 = ret_tmp_chpl2;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  i_arg_chpl = &call_tmp_chpl16;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl17 = borrow_chpl2(i_arg_chpl);
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  if (((RootClass_chpl)(call_tmp_chpl17)) != nil) /* ZLINE: 492 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  {
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    coerce_tmp_chpl3 = ((Error_chpl)(call_tmp_chpl17));
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    chpl_check_nil(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    ((void(*)(Error_chpl,
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
       string_chpl *,
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
       int64_t,
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
       int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(coerce_tmp_chpl3))->chpl__cid) + INT64(1))])(coerce_tmp_chpl3, &ret_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl18 = ret_tmp_chpl3;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    safeCastCheckHalt_chpl(&call_tmp_chpl18, _ln_chpl, _fn_chpl);
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    i_x_chpl = &call_tmp_chpl18;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    deinit_chpl95(i_x_chpl, _ln_chpl, _fn_chpl);
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  }
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl__autoDestroy62(&call_tmp_chpl16, _ln_chpl, _fn_chpl);
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  memcpy(((c_ptr_void_chpl)(call_tmp_chpl14)), ((c_ptr_void_chpl)(call_tmp_chpl15)), ((uint64_t)(copyLen_chpl)));
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl19 = (dst_chpl + copyLen_chpl);
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  *(call_tmp_chpl19) = ((uint8_t)(INT64(0)));
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  (&ret_chpl4)->x0 = call_tmp_chpl13;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  (&ret_chpl4)->x1 = tmp_chpl;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl9 = ret_chpl4;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  tmp_x0_chpl2 = (&call_tmp_chpl9)->x0;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  tmp_x1_chpl2 = (&call_tmp_chpl9)->x1;
#line 495 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  buff_chpl = tmp_x0_chpl2;
#line 508 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl20 = size_chpl8(r2_chpl, _ln_chpl, _fn_chpl);
#line 509 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl21 = (buff_chpl + call_tmp_chpl20);
#line 509 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  *(call_tmp_chpl21) = ((uint8_t)(INT64(0)));
#line 512 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  numCodepoints_chpl = tmp_x1_chpl;
#line 513 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  if (tmp_x1_chpl == INT64(-1)) /* ZLINE: 513 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 513 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  {
#line 514 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl22 = countNumCodepoints_chpl(tmp_x0_chpl2, call_tmp_chpl20, _ln_chpl, _fn_chpl);
#line 514 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    numCodepoints_chpl = call_tmp_chpl22;
#line 514 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  }
#line 516 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  init_chpl194(&ret_chpl6, _ln_chpl, _fn_chpl);
#line 516 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  i_x_chpl2 = &ret_chpl6;
#line 516 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  (i_x_chpl2)->isOwned = UINT8(true);
#line 516 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  (i_x_chpl2)->buff = tmp_x0_chpl2;
#line 516 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  (i_x_chpl2)->buffSize = tmp_x1_chpl2;
#line 516 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  (i_x_chpl2)->buffLen = call_tmp_chpl20;
#line 516 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  (&ret_chpl6)->cachedNumCodepoints = numCodepoints_chpl;
#line 516 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  ret_chpl5 = ret_chpl6;
#line 516 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  ret_chpl = ret_chpl5;
#line 516 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _end_getSlice_chpl:;
#line 470 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  *(_retArg_chpl) = ret_chpl;
#line 470 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  return;
#line 470 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
}

#line 470 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
/* BytesStringCommon.chpl:470 */
#line 470 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
static void getSlice_chpl5(string_chpl * x_chpl,
#line 470 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                           range_byteIndex_high_one_chpl r_chpl,
#line 470 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                           Error_chpl * error_out_chpl,
#line 470 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                           string_chpl * _retArg_chpl,
#line 470 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                           int64_t _ln_chpl,
#line 470 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                           int32_t _fn_chpl) {
#line 470 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl ret_chpl;
#line 474 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl ret_chpl2;
#line 478 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _tuple_2_range_int64_t_both_one_int64_t_chpl call_tmp_chpl6;
#line 478 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  Error_chpl error_chpl = NULL;
#line 478 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _tuple_2_range_int64_t_both_one_int64_t_chpl ret_tmp_chpl;
#line 478 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl tmp_x0_chpl;
#line 478 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t tmp_x1_chpl;
#line 478 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl r2_chpl;
#line 479 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t call_tmp_chpl7;
#line 480 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl ret_chpl3;
#line 484 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  c_ptr_uint8_t_chpl buff_chpl = NULL;
#line 491 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t call_tmp_chpl8;
#line 491 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t copyLen_chpl;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _tuple_2_c_ptr_uint8_t_int64_t_chpl call_tmp_chpl9;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _tuple_2_c_ptr_uint8_t_int64_t_chpl ret_chpl4;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  c_ptr_uint8_t_chpl call_tmp_chpl10 = NULL;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t call_tmp_chpl11;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t tmp_chpl;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int16_t call_tmp_chpl12;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  c_ptr_void_chpl cast_tmp_chpl = NULL;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  c_ptr_uint8_t_chpl call_tmp_chpl13 = NULL;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  c_ptr_uint8_t_chpl dst_chpl = NULL;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  c_ptr_uint8_t_chpl call_tmp_chpl14 = NULL;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  c_ptr_uint8_t_chpl call_tmp_chpl15 = NULL;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _owned_IllegalArgumentError_chpl2 call_tmp_chpl16;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _owned_IllegalArgumentError_chpl2 ret_tmp_chpl2;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref__owned_IllegalArgumentError_chpl i_arg_chpl = NULL;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  IllegalArgumentError_chpl call_tmp_chpl17 = NULL;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl call_tmp_chpl18;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  Error_chpl coerce_tmp_chpl3 = NULL;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl ret_tmp_chpl3;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref_uint8_t_chpl call_tmp_chpl19 = NULL;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  c_ptr_uint8_t_chpl tmp_x0_chpl2 = NULL;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t tmp_x1_chpl2;
#line 508 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t call_tmp_chpl20;
#line 509 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref_uint8_t_chpl call_tmp_chpl21 = NULL;
#line 512 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t numCodepoints_chpl;
#line 514 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t call_tmp_chpl22;
#line 516 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl ret_chpl5;
#line 516 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl ret_chpl6;
#line 516 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref_string_chpl i_x_chpl2 = NULL;
#line 473 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  if ((x_chpl)->buffLen == INT64(0)) /* ZLINE: 473 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 473 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  {
#line 474 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    init_chpl194(&ret_chpl2, _ln_chpl, _fn_chpl);
#line 475 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    ret_chpl = ret_chpl2;
#line 475 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    goto _end_getSlice_chpl;
#line 475 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  }
#line 478 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  error_chpl = NULL;
#line 478 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  getView_chpl(x_chpl, r_chpl, &error_chpl, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 478 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl6 = ret_tmp_chpl;
#line 478 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  if (error_chpl != nil) /* ZLINE: 478 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 478 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  {
#line 478 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    *(error_out_chpl) = error_chpl;
#line 478 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    goto _end_getSlice_chpl;
#line 478 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  }
#line 478 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  tmp_x0_chpl = (&call_tmp_chpl6)->x0;
#line 478 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  tmp_x1_chpl = (&call_tmp_chpl6)->x1;
#line 478 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  r2_chpl = tmp_x0_chpl;
#line 479 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl7 = size_chpl8(tmp_x0_chpl, _ln_chpl, _fn_chpl);
#line 479 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  if (call_tmp_chpl7 <= INT64(0)) /* ZLINE: 479 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 479 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  {
#line 480 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    init_chpl194(&ret_chpl3, _ln_chpl, _fn_chpl);
#line 481 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    ret_chpl = ret_chpl3;
#line 481 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    goto _end_getSlice_chpl;
#line 481 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  }
#line 491 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl8 = (((int64_t)(((&r2_chpl)->_high - (&r2_chpl)->_low))) + INT64(1));
#line 491 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  copyLen_chpl = call_tmp_chpl8;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl10 = ((x_chpl)->buff + (&r2_chpl)->_low);
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl11 = chpl_here_good_alloc_size(((int64_t)((call_tmp_chpl8 + INT64(1)))), _ln_chpl, _fn_chpl);
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  if (call_tmp_chpl11 > INT64(0)) /* ZLINE: 492 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  {
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    tmp_chpl = call_tmp_chpl11;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  } else /* ZLINE: 492 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  {
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    tmp_chpl = INT64(0);
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  }
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl12 = offset_STR_COPY_DATA_chpl();
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  cast_tmp_chpl = chpl_here_alloc(tmp_chpl, call_tmp_chpl12, _ln_chpl, _fn_chpl);
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl13 = ((c_ptr_uint8_t_chpl)(cast_tmp_chpl));
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  dst_chpl = call_tmp_chpl13;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl14 = (call_tmp_chpl13 + INT64(0));
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl15 = (call_tmp_chpl10 + INT64(0));
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl_checkValue7(call_tmp_chpl8, &ret_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl16 = ret_tmp_chpl2;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  i_arg_chpl = &call_tmp_chpl16;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl17 = borrow_chpl2(i_arg_chpl);
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  if (((RootClass_chpl)(call_tmp_chpl17)) != nil) /* ZLINE: 492 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  {
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    coerce_tmp_chpl3 = ((Error_chpl)(call_tmp_chpl17));
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    chpl_check_nil(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    ((void(*)(Error_chpl,
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
       string_chpl *,
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
       int64_t,
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
       int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(coerce_tmp_chpl3))->chpl__cid) + INT64(1))])(coerce_tmp_chpl3, &ret_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl18 = ret_tmp_chpl3;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    safeCastCheckHalt_chpl(&call_tmp_chpl18, _ln_chpl, _fn_chpl);
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    i_x_chpl = &call_tmp_chpl18;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    deinit_chpl95(i_x_chpl, _ln_chpl, _fn_chpl);
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  }
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl__autoDestroy62(&call_tmp_chpl16, _ln_chpl, _fn_chpl);
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  memcpy(((c_ptr_void_chpl)(call_tmp_chpl14)), ((c_ptr_void_chpl)(call_tmp_chpl15)), ((uint64_t)(copyLen_chpl)));
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl19 = (dst_chpl + copyLen_chpl);
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  *(call_tmp_chpl19) = ((uint8_t)(INT64(0)));
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  (&ret_chpl4)->x0 = call_tmp_chpl13;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  (&ret_chpl4)->x1 = tmp_chpl;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl9 = ret_chpl4;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  tmp_x0_chpl2 = (&call_tmp_chpl9)->x0;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  tmp_x1_chpl2 = (&call_tmp_chpl9)->x1;
#line 495 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  buff_chpl = tmp_x0_chpl2;
#line 508 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl20 = size_chpl8(r2_chpl, _ln_chpl, _fn_chpl);
#line 509 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl21 = (buff_chpl + call_tmp_chpl20);
#line 509 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  *(call_tmp_chpl21) = ((uint8_t)(INT64(0)));
#line 512 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  numCodepoints_chpl = tmp_x1_chpl;
#line 513 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  if (tmp_x1_chpl == INT64(-1)) /* ZLINE: 513 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 513 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  {
#line 514 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl22 = countNumCodepoints_chpl(tmp_x0_chpl2, call_tmp_chpl20, _ln_chpl, _fn_chpl);
#line 514 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    numCodepoints_chpl = call_tmp_chpl22;
#line 514 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  }
#line 516 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  init_chpl194(&ret_chpl6, _ln_chpl, _fn_chpl);
#line 516 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  i_x_chpl2 = &ret_chpl6;
#line 516 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  (i_x_chpl2)->isOwned = UINT8(true);
#line 516 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  (i_x_chpl2)->buff = tmp_x0_chpl2;
#line 516 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  (i_x_chpl2)->buffSize = tmp_x1_chpl2;
#line 516 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  (i_x_chpl2)->buffLen = call_tmp_chpl20;
#line 516 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  (&ret_chpl6)->cachedNumCodepoints = numCodepoints_chpl;
#line 516 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  ret_chpl5 = ret_chpl6;
#line 516 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  ret_chpl = ret_chpl5;
#line 516 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _end_getSlice_chpl:;
#line 470 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  *(_retArg_chpl) = ret_chpl;
#line 470 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  return;
#line 470 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
}

#line 470 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
/* BytesStringCommon.chpl:470 */
#line 470 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
static void getSlice_chpl(string_chpl * x_chpl,
#line 470 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                          range_byteIndex_low_one_chpl r_chpl,
#line 470 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                          Error_chpl * error_out_chpl,
#line 470 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                          string_chpl * _retArg_chpl,
#line 470 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                          int64_t _ln_chpl,
#line 470 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                          int32_t _fn_chpl) {
#line 470 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl ret_chpl;
#line 474 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl ret_chpl2;
#line 478 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _tuple_2_range_int64_t_both_one_int64_t_chpl call_tmp_chpl6;
#line 478 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  Error_chpl error_chpl = NULL;
#line 478 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _tuple_2_range_int64_t_both_one_int64_t_chpl ret_tmp_chpl;
#line 478 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl tmp_x0_chpl;
#line 478 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t tmp_x1_chpl;
#line 478 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl r2_chpl;
#line 479 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t call_tmp_chpl7;
#line 480 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl ret_chpl3;
#line 484 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  c_ptr_uint8_t_chpl buff_chpl = NULL;
#line 491 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t call_tmp_chpl8;
#line 491 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t copyLen_chpl;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _tuple_2_c_ptr_uint8_t_int64_t_chpl call_tmp_chpl9;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _tuple_2_c_ptr_uint8_t_int64_t_chpl ret_chpl4;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  c_ptr_uint8_t_chpl call_tmp_chpl10 = NULL;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t call_tmp_chpl11;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t tmp_chpl;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int16_t call_tmp_chpl12;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  c_ptr_void_chpl cast_tmp_chpl = NULL;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  c_ptr_uint8_t_chpl call_tmp_chpl13 = NULL;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  c_ptr_uint8_t_chpl dst_chpl = NULL;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  c_ptr_uint8_t_chpl call_tmp_chpl14 = NULL;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  c_ptr_uint8_t_chpl call_tmp_chpl15 = NULL;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _owned_IllegalArgumentError_chpl2 call_tmp_chpl16;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _owned_IllegalArgumentError_chpl2 ret_tmp_chpl2;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref__owned_IllegalArgumentError_chpl i_arg_chpl = NULL;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  IllegalArgumentError_chpl call_tmp_chpl17 = NULL;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl call_tmp_chpl18;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  Error_chpl coerce_tmp_chpl3 = NULL;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl ret_tmp_chpl3;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref_uint8_t_chpl call_tmp_chpl19 = NULL;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  c_ptr_uint8_t_chpl tmp_x0_chpl2 = NULL;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t tmp_x1_chpl2;
#line 508 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t call_tmp_chpl20;
#line 509 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref_uint8_t_chpl call_tmp_chpl21 = NULL;
#line 512 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t numCodepoints_chpl;
#line 514 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t call_tmp_chpl22;
#line 516 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl ret_chpl5;
#line 516 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl ret_chpl6;
#line 516 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref_string_chpl i_x_chpl2 = NULL;
#line 473 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  if ((x_chpl)->buffLen == INT64(0)) /* ZLINE: 473 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 473 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  {
#line 474 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    init_chpl194(&ret_chpl2, _ln_chpl, _fn_chpl);
#line 475 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    ret_chpl = ret_chpl2;
#line 475 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    goto _end_getSlice_chpl;
#line 475 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  }
#line 478 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  error_chpl = NULL;
#line 478 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  getView_chpl3(x_chpl, r_chpl, &error_chpl, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 478 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl6 = ret_tmp_chpl;
#line 478 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  if (error_chpl != nil) /* ZLINE: 478 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 478 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  {
#line 478 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    *(error_out_chpl) = error_chpl;
#line 478 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    goto _end_getSlice_chpl;
#line 478 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  }
#line 478 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  tmp_x0_chpl = (&call_tmp_chpl6)->x0;
#line 478 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  tmp_x1_chpl = (&call_tmp_chpl6)->x1;
#line 478 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  r2_chpl = tmp_x0_chpl;
#line 479 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl7 = size_chpl8(tmp_x0_chpl, _ln_chpl, _fn_chpl);
#line 479 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  if (call_tmp_chpl7 <= INT64(0)) /* ZLINE: 479 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 479 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  {
#line 480 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    init_chpl194(&ret_chpl3, _ln_chpl, _fn_chpl);
#line 481 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    ret_chpl = ret_chpl3;
#line 481 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    goto _end_getSlice_chpl;
#line 481 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  }
#line 491 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl8 = (((int64_t)(((&r2_chpl)->_high - (&r2_chpl)->_low))) + INT64(1));
#line 491 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  copyLen_chpl = call_tmp_chpl8;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl10 = ((x_chpl)->buff + (&r2_chpl)->_low);
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl11 = chpl_here_good_alloc_size(((int64_t)((call_tmp_chpl8 + INT64(1)))), _ln_chpl, _fn_chpl);
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  if (call_tmp_chpl11 > INT64(0)) /* ZLINE: 492 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  {
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    tmp_chpl = call_tmp_chpl11;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  } else /* ZLINE: 492 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  {
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    tmp_chpl = INT64(0);
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  }
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl12 = offset_STR_COPY_DATA_chpl();
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  cast_tmp_chpl = chpl_here_alloc(tmp_chpl, call_tmp_chpl12, _ln_chpl, _fn_chpl);
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl13 = ((c_ptr_uint8_t_chpl)(cast_tmp_chpl));
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  dst_chpl = call_tmp_chpl13;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl14 = (call_tmp_chpl13 + INT64(0));
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl15 = (call_tmp_chpl10 + INT64(0));
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl_checkValue7(call_tmp_chpl8, &ret_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl16 = ret_tmp_chpl2;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  i_arg_chpl = &call_tmp_chpl16;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl17 = borrow_chpl2(i_arg_chpl);
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  if (((RootClass_chpl)(call_tmp_chpl17)) != nil) /* ZLINE: 492 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  {
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    coerce_tmp_chpl3 = ((Error_chpl)(call_tmp_chpl17));
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    chpl_check_nil(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    ((void(*)(Error_chpl,
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
       string_chpl *,
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
       int64_t,
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
       int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(coerce_tmp_chpl3))->chpl__cid) + INT64(1))])(coerce_tmp_chpl3, &ret_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl18 = ret_tmp_chpl3;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    safeCastCheckHalt_chpl(&call_tmp_chpl18, _ln_chpl, _fn_chpl);
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    i_x_chpl = &call_tmp_chpl18;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    deinit_chpl95(i_x_chpl, _ln_chpl, _fn_chpl);
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  }
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl__autoDestroy62(&call_tmp_chpl16, _ln_chpl, _fn_chpl);
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  memcpy(((c_ptr_void_chpl)(call_tmp_chpl14)), ((c_ptr_void_chpl)(call_tmp_chpl15)), ((uint64_t)(copyLen_chpl)));
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl19 = (dst_chpl + copyLen_chpl);
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  *(call_tmp_chpl19) = ((uint8_t)(INT64(0)));
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  (&ret_chpl4)->x0 = call_tmp_chpl13;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  (&ret_chpl4)->x1 = tmp_chpl;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl9 = ret_chpl4;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  tmp_x0_chpl2 = (&call_tmp_chpl9)->x0;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  tmp_x1_chpl2 = (&call_tmp_chpl9)->x1;
#line 495 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  buff_chpl = tmp_x0_chpl2;
#line 508 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl20 = size_chpl8(r2_chpl, _ln_chpl, _fn_chpl);
#line 509 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl21 = (buff_chpl + call_tmp_chpl20);
#line 509 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  *(call_tmp_chpl21) = ((uint8_t)(INT64(0)));
#line 512 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  numCodepoints_chpl = tmp_x1_chpl;
#line 513 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  if (tmp_x1_chpl == INT64(-1)) /* ZLINE: 513 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 513 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  {
#line 514 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl22 = countNumCodepoints_chpl(tmp_x0_chpl2, call_tmp_chpl20, _ln_chpl, _fn_chpl);
#line 514 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    numCodepoints_chpl = call_tmp_chpl22;
#line 514 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  }
#line 516 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  init_chpl194(&ret_chpl6, _ln_chpl, _fn_chpl);
#line 516 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  i_x_chpl2 = &ret_chpl6;
#line 516 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  (i_x_chpl2)->isOwned = UINT8(true);
#line 516 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  (i_x_chpl2)->buff = tmp_x0_chpl2;
#line 516 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  (i_x_chpl2)->buffSize = tmp_x1_chpl2;
#line 516 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  (i_x_chpl2)->buffLen = call_tmp_chpl20;
#line 516 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  (&ret_chpl6)->cachedNumCodepoints = numCodepoints_chpl;
#line 516 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  ret_chpl5 = ret_chpl6;
#line 516 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  ret_chpl = ret_chpl5;
#line 516 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _end_getSlice_chpl:;
#line 470 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  *(_retArg_chpl) = ret_chpl;
#line 470 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  return;
#line 470 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
}

#line 470 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
/* BytesStringCommon.chpl:470 */
#line 470 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
static void getSlice_chpl2(string_chpl * x_chpl,
#line 470 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                           range_int64_t_both_one_chpl r_chpl,
#line 470 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                           Error_chpl * error_out_chpl,
#line 470 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                           string_chpl * _retArg_chpl,
#line 470 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                           int64_t _ln_chpl,
#line 470 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                           int32_t _fn_chpl) {
#line 470 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl ret_chpl;
#line 474 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl ret_chpl2;
#line 478 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _tuple_2_range_int64_t_both_one_int64_t_chpl call_tmp_chpl6;
#line 478 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  Error_chpl error_chpl = NULL;
#line 478 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _tuple_2_range_int64_t_both_one_int64_t_chpl ret_tmp_chpl;
#line 478 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl tmp_x0_chpl;
#line 478 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t tmp_x1_chpl;
#line 478 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl r2_chpl;
#line 479 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t call_tmp_chpl7;
#line 480 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl ret_chpl3;
#line 484 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  c_ptr_uint8_t_chpl buff_chpl = NULL;
#line 491 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t call_tmp_chpl8;
#line 491 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t copyLen_chpl;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _tuple_2_c_ptr_uint8_t_int64_t_chpl call_tmp_chpl9;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _tuple_2_c_ptr_uint8_t_int64_t_chpl ret_chpl4;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  c_ptr_uint8_t_chpl call_tmp_chpl10 = NULL;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t call_tmp_chpl11;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t tmp_chpl;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int16_t call_tmp_chpl12;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  c_ptr_void_chpl cast_tmp_chpl = NULL;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  c_ptr_uint8_t_chpl call_tmp_chpl13 = NULL;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  c_ptr_uint8_t_chpl dst_chpl = NULL;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  c_ptr_uint8_t_chpl call_tmp_chpl14 = NULL;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  c_ptr_uint8_t_chpl call_tmp_chpl15 = NULL;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _owned_IllegalArgumentError_chpl2 call_tmp_chpl16;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _owned_IllegalArgumentError_chpl2 ret_tmp_chpl2;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref__owned_IllegalArgumentError_chpl i_arg_chpl = NULL;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  IllegalArgumentError_chpl call_tmp_chpl17 = NULL;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl call_tmp_chpl18;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  Error_chpl coerce_tmp_chpl3 = NULL;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl ret_tmp_chpl3;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref_uint8_t_chpl call_tmp_chpl19 = NULL;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  c_ptr_uint8_t_chpl tmp_x0_chpl2 = NULL;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t tmp_x1_chpl2;
#line 508 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t call_tmp_chpl20;
#line 509 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref_uint8_t_chpl call_tmp_chpl21 = NULL;
#line 512 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t numCodepoints_chpl;
#line 514 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t call_tmp_chpl22;
#line 516 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl ret_chpl5;
#line 516 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl ret_chpl6;
#line 516 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref_string_chpl i_x_chpl2 = NULL;
#line 473 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  if ((x_chpl)->buffLen == INT64(0)) /* ZLINE: 473 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 473 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  {
#line 474 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    init_chpl194(&ret_chpl2, _ln_chpl, _fn_chpl);
#line 475 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    ret_chpl = ret_chpl2;
#line 475 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    goto _end_getSlice_chpl;
#line 475 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  }
#line 478 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  error_chpl = NULL;
#line 478 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  getView_chpl6(x_chpl, r_chpl, &error_chpl, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 478 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl6 = ret_tmp_chpl;
#line 478 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  if (error_chpl != nil) /* ZLINE: 478 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 478 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  {
#line 478 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    *(error_out_chpl) = error_chpl;
#line 478 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    goto _end_getSlice_chpl;
#line 478 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  }
#line 478 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  tmp_x0_chpl = (&call_tmp_chpl6)->x0;
#line 478 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  tmp_x1_chpl = (&call_tmp_chpl6)->x1;
#line 478 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  r2_chpl = tmp_x0_chpl;
#line 479 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl7 = size_chpl8(tmp_x0_chpl, _ln_chpl, _fn_chpl);
#line 479 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  if (call_tmp_chpl7 <= INT64(0)) /* ZLINE: 479 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 479 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  {
#line 480 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    init_chpl194(&ret_chpl3, _ln_chpl, _fn_chpl);
#line 481 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    ret_chpl = ret_chpl3;
#line 481 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    goto _end_getSlice_chpl;
#line 481 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  }
#line 491 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl8 = (((int64_t)(((&r2_chpl)->_high - (&r2_chpl)->_low))) + INT64(1));
#line 491 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  copyLen_chpl = call_tmp_chpl8;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl10 = ((x_chpl)->buff + (&r2_chpl)->_low);
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl11 = chpl_here_good_alloc_size(((int64_t)((call_tmp_chpl8 + INT64(1)))), _ln_chpl, _fn_chpl);
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  if (call_tmp_chpl11 > INT64(0)) /* ZLINE: 492 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  {
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    tmp_chpl = call_tmp_chpl11;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  } else /* ZLINE: 492 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  {
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    tmp_chpl = INT64(0);
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  }
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl12 = offset_STR_COPY_DATA_chpl();
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  cast_tmp_chpl = chpl_here_alloc(tmp_chpl, call_tmp_chpl12, _ln_chpl, _fn_chpl);
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl13 = ((c_ptr_uint8_t_chpl)(cast_tmp_chpl));
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  dst_chpl = call_tmp_chpl13;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl14 = (call_tmp_chpl13 + INT64(0));
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl15 = (call_tmp_chpl10 + INT64(0));
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl_checkValue7(call_tmp_chpl8, &ret_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl16 = ret_tmp_chpl2;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  i_arg_chpl = &call_tmp_chpl16;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl17 = borrow_chpl2(i_arg_chpl);
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  if (((RootClass_chpl)(call_tmp_chpl17)) != nil) /* ZLINE: 492 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  {
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    coerce_tmp_chpl3 = ((Error_chpl)(call_tmp_chpl17));
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    chpl_check_nil(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    ((void(*)(Error_chpl,
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
       string_chpl *,
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
       int64_t,
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
       int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(coerce_tmp_chpl3))->chpl__cid) + INT64(1))])(coerce_tmp_chpl3, &ret_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl18 = ret_tmp_chpl3;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    safeCastCheckHalt_chpl(&call_tmp_chpl18, _ln_chpl, _fn_chpl);
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    i_x_chpl = &call_tmp_chpl18;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    deinit_chpl95(i_x_chpl, _ln_chpl, _fn_chpl);
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  }
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl__autoDestroy62(&call_tmp_chpl16, _ln_chpl, _fn_chpl);
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  memcpy(((c_ptr_void_chpl)(call_tmp_chpl14)), ((c_ptr_void_chpl)(call_tmp_chpl15)), ((uint64_t)(copyLen_chpl)));
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl19 = (dst_chpl + copyLen_chpl);
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  *(call_tmp_chpl19) = ((uint8_t)(INT64(0)));
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  (&ret_chpl4)->x0 = call_tmp_chpl13;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  (&ret_chpl4)->x1 = tmp_chpl;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl9 = ret_chpl4;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  tmp_x0_chpl2 = (&call_tmp_chpl9)->x0;
#line 492 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  tmp_x1_chpl2 = (&call_tmp_chpl9)->x1;
#line 495 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  buff_chpl = tmp_x0_chpl2;
#line 508 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl20 = size_chpl8(r2_chpl, _ln_chpl, _fn_chpl);
#line 509 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl21 = (buff_chpl + call_tmp_chpl20);
#line 509 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  *(call_tmp_chpl21) = ((uint8_t)(INT64(0)));
#line 512 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  numCodepoints_chpl = tmp_x1_chpl;
#line 513 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  if (tmp_x1_chpl == INT64(-1)) /* ZLINE: 513 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 513 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  {
#line 514 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl22 = countNumCodepoints_chpl(tmp_x0_chpl2, call_tmp_chpl20, _ln_chpl, _fn_chpl);
#line 514 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    numCodepoints_chpl = call_tmp_chpl22;
#line 514 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  }
#line 516 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  init_chpl194(&ret_chpl6, _ln_chpl, _fn_chpl);
#line 516 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  i_x_chpl2 = &ret_chpl6;
#line 516 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  (i_x_chpl2)->isOwned = UINT8(true);
#line 516 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  (i_x_chpl2)->buff = tmp_x0_chpl2;
#line 516 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  (i_x_chpl2)->buffSize = tmp_x1_chpl2;
#line 516 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  (i_x_chpl2)->buffLen = call_tmp_chpl20;
#line 516 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  (&ret_chpl6)->cachedNumCodepoints = numCodepoints_chpl;
#line 516 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  ret_chpl5 = ret_chpl6;
#line 516 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  ret_chpl = ret_chpl5;
#line 516 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _end_getSlice_chpl:;
#line 470 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  *(_retArg_chpl) = ret_chpl;
#line 470 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  return;
#line 470 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
}

#line 533 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
/* BytesStringCommon.chpl:533 */
#line 533 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
static void doReplace_chpl(string_chpl * x_chpl,
#line 533 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                           string_chpl * needle_chpl,
#line 533 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                           string_chpl * replacement_chpl,
#line 533 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                           int64_t count_chpl,
#line 533 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                           string_chpl * _retArg_chpl,
#line 533 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                           int64_t _ln_chpl,
#line 533 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                           int32_t _fn_chpl) {
#line 533 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl ret_chpl;
#line 538 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl result_chpl;
#line 539 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t found_chpl;
#line 540 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  byteIndex_chpl startIdx_chpl;
#line 541 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl ret_chpl2;
#line 541 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl ret_chpl3;
#line 541 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl localNeedle_chpl;
#line 542 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl ret_chpl4;
#line 542 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl ret_chpl5;
#line 542 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl localReplacement_chpl;
#line 544 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl_bool tmp_chpl;
#line 544 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl_bool tmp_chpl2;
#line 545 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_byteIndex_low_one_chpl call_tmp_chpl6;
#line 545 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_byteIndex_low_one_chpl ret_tmp_chpl;
#line 545 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref_string_chpl i_this_chpl = NULL;
#line 545 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref_string_chpl i_pattern_chpl = NULL;
#line 545 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  byteIndex_chpl ret_chpl6;
#line 545 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t ret_chpl7;
#line 545 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t ret_chpl8;
#line 545 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t call_tmp_chpl7;
#line 545 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  byteIndex_chpl ret_chpl9;
#line 545 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  byteIndex_chpl cpi_chpl;
#line 545 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t call_tmp_chpl8;
#line 545 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  byteIndex_chpl ret_chpl10;
#line 545 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  byteIndex_chpl cpi_chpl2;
#line 545 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  byteIndex_chpl idx_chpl;
#line 548 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref_int64_t_chpl i_lhs_chpl = NULL;
#line 550 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  Error_chpl error_chpl = NULL;
#line 551 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  byteIndex_chpl call_tmp_chpl9;
#line 551 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t call_tmp_chpl10;
#line 551 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  byteIndex_chpl ret_chpl11;
#line 551 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  byteIndex_chpl cpi_chpl3;
#line 551 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_byteIndex_high_one_chpl call_tmp_chpl11;
#line 551 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_byteIndex_high_one_chpl ret_tmp_chpl2;
#line 551 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl call_tmp_chpl12;
#line 551 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref_string_chpl i_this_chpl2 = NULL;
#line 551 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl ret_chpl12;
#line 551 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl call_tmp_chpl13;
#line 551 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  Error_chpl error_chpl2 = NULL;
#line 551 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl ret_tmp_chpl3;
#line 551 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl call_tmp_chpl14;
#line 551 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl ret_tmp_chpl4;
#line 552 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t ret_chpl13;
#line 552 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  byteIndex_chpl call_tmp_chpl15;
#line 552 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t call_tmp_chpl16;
#line 552 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  byteIndex_chpl ret_chpl14;
#line 552 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  byteIndex_chpl cpi_chpl4;
#line 552 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_byteIndex_low_one_chpl call_tmp_chpl17;
#line 552 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_byteIndex_low_one_chpl ret_tmp_chpl5;
#line 552 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl call_tmp_chpl18;
#line 552 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref_string_chpl i_this_chpl3 = NULL;
#line 552 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl ret_chpl15;
#line 552 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl call_tmp_chpl19;
#line 552 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  Error_chpl error_chpl3 = NULL;
#line 552 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl ret_tmp_chpl6;
#line 552 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 552 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref_string_chpl i_x_chpl2 = NULL;
#line 551 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl call_tmp_chpl20;
#line 551 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl ret_tmp_chpl7;
#line 551 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref_string_chpl i_x_chpl3 = NULL;
#line 551 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref_string_chpl i_x_chpl4 = NULL;
#line 551 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref_string_chpl i_x_chpl5 = NULL;
#line 551 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref_string_chpl i_x_chpl6 = NULL;
#line 555 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t ret_chpl16;
#line 555 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  byteIndex_chpl call_tmp_chpl21;
#line 555 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t call_tmp_chpl22;
#line 555 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  byteIndex_chpl ret_chpl17;
#line 555 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  byteIndex_chpl cpi_chpl5;
#line 544 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl_bool tmp_chpl3;
#line 557 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref_string_chpl i_x_chpl7 = NULL;
#line 557 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref_string_chpl i_x_chpl8 = NULL;
#line 538 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  init_ASSIGN__chpl25(&result_chpl, x_chpl, _ln_chpl, _fn_chpl);
#line 539 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  found_chpl = INT64(0);
#line 540 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  init_ASSIGN__chpl24(&startIdx_chpl, INT64(0));
#line 541 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  init_chpl194(&ret_chpl3, _ln_chpl, _fn_chpl);
#line 541 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  (&ret_chpl3)->cachedNumCodepoints = (needle_chpl)->cachedNumCodepoints;
#line 541 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  initWithBorrowedBuffer_chpl(&ret_chpl3, needle_chpl, _ln_chpl, _fn_chpl);
#line 541 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  ret_chpl2 = ret_chpl3;
#line 541 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  localNeedle_chpl = ret_chpl2;
#line 542 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  init_chpl194(&ret_chpl5, _ln_chpl, _fn_chpl);
#line 542 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  (&ret_chpl5)->cachedNumCodepoints = (replacement_chpl)->cachedNumCodepoints;
#line 542 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  initWithBorrowedBuffer_chpl(&ret_chpl5, replacement_chpl, _ln_chpl, _fn_chpl);
#line 542 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  ret_chpl4 = ret_chpl5;
#line 542 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  localReplacement_chpl = ret_chpl4;
#line 544 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  if (count_chpl < INT64(0)) /* ZLINE: 544 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 544 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  {
#line 544 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    tmp_chpl2 = UINT8(true);
#line 544 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  } else /* ZLINE: 544 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 544 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  {
#line 544 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    tmp_chpl2 = (INT64(0) < count_chpl);
#line 544 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  }
#line 544 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  tmp_chpl = tmp_chpl2;
#line 544 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  while (tmp_chpl) {
#line 545 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    chpl_build_low_bounded_range3(&startIdx_chpl, &ret_tmp_chpl);
#line 545 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl6 = ret_tmp_chpl;
#line 545 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    i_this_chpl = &result_chpl;
#line 545 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    i_pattern_chpl = &localNeedle_chpl;
#line 545 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    ret_chpl7 = (&result_chpl)->cachedNumCodepoints;
#line 545 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    ret_chpl8 = (&result_chpl)->buffLen;
#line 545 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    if (ret_chpl7 == ret_chpl8) /* ZLINE: 545 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 545 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    {
#line 545 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      call_tmp_chpl7 = doSearchNoEnc_chpl(i_this_chpl, i_pattern_chpl, call_tmp_chpl6, _ln_chpl, _fn_chpl);
#line 545 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      init_chpl192(&cpi_chpl);
#line 545 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      (&cpi_chpl)->_bindex = call_tmp_chpl7;
#line 545 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      ret_chpl9 = cpi_chpl;
#line 545 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      ret_chpl6 = ret_chpl9;
#line 545 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      goto _end_find_chpl;
#line 545 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    } else /* ZLINE: 545 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 545 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    {
#line 545 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      call_tmp_chpl8 = doSearchUTF8_chpl(i_this_chpl, i_pattern_chpl, call_tmp_chpl6, _ln_chpl, _fn_chpl);
#line 545 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      init_chpl192(&cpi_chpl2);
#line 545 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      (&cpi_chpl2)->_bindex = call_tmp_chpl8;
#line 545 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      ret_chpl10 = cpi_chpl2;
#line 545 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      ret_chpl6 = ret_chpl10;
#line 545 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      goto _end_find_chpl;
#line 545 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    }
#line 545 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    _end_find_chpl:;
#line 545 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    idx_chpl = ret_chpl6;
#line 546 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    if ((&idx_chpl)->_bindex == INT64(-1)) /* ZLINE: 546 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 546 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    {
#line 546 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      goto _breakLabel_chpl;
#line 546 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    }
#line 548 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    i_lhs_chpl = &found_chpl;
#line 548 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    *(i_lhs_chpl) += INT64(1);
#line 550 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    error_chpl = NULL;
#line 551 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl10 = ((&idx_chpl)->_bindex - INT64(1));
#line 551 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    init_chpl192(&cpi_chpl3);
#line 551 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    (&cpi_chpl3)->_bindex = call_tmp_chpl10;
#line 551 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    ret_chpl11 = cpi_chpl3;
#line 551 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl9 = ret_chpl11;
#line 551 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    chpl_build_high_bounded_range(&call_tmp_chpl9, &ret_tmp_chpl2);
#line 551 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl11 = ret_tmp_chpl2;
#line 551 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    i_this_chpl2 = &result_chpl;
#line 551 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    error_chpl2 = NULL;
#line 551 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    getSlice_chpl5(i_this_chpl2, call_tmp_chpl11, &error_chpl2, &ret_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 551 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl13 = ret_tmp_chpl3;
#line 551 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    if (error_chpl2 != nil) /* ZLINE: 551 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 551 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    {
#line 551 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      error_chpl = error_chpl2;
#line 551 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      goto _endthis_chpl;
#line 551 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    }
#line 551 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    ret_chpl12 = call_tmp_chpl13;
#line 551 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    _endthis_chpl:;
#line 551 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl12 = ret_chpl12;
#line 551 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    if (error_chpl != nil) /* ZLINE: 551 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 551 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    {
#line 551 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      goto handler_chpl;
#line 551 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    }
#line 551 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    chpl___PLUS_(&call_tmp_chpl12, &localReplacement_chpl, &ret_tmp_chpl4, _ln_chpl, _fn_chpl);
#line 551 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl14 = ret_tmp_chpl4;
#line 552 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    ret_chpl13 = (&localNeedle_chpl)->buffLen;
#line 552 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl16 = ((&idx_chpl)->_bindex + ret_chpl13);
#line 552 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    init_chpl192(&cpi_chpl4);
#line 552 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    (&cpi_chpl4)->_bindex = call_tmp_chpl16;
#line 552 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    ret_chpl14 = cpi_chpl4;
#line 552 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl15 = ret_chpl14;
#line 552 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    chpl_build_low_bounded_range3(&call_tmp_chpl15, &ret_tmp_chpl5);
#line 552 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl17 = ret_tmp_chpl5;
#line 552 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    i_this_chpl3 = &result_chpl;
#line 552 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    error_chpl3 = NULL;
#line 552 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    getSlice_chpl(i_this_chpl3, call_tmp_chpl17, &error_chpl3, &ret_tmp_chpl6, _ln_chpl, _fn_chpl);
#line 552 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl19 = ret_tmp_chpl6;
#line 552 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    if (error_chpl3 != nil) /* ZLINE: 552 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 552 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    {
#line 552 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      error_chpl = error_chpl3;
#line 552 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      goto _endthis_chpl2;
#line 552 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    }
#line 552 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    ret_chpl15 = call_tmp_chpl19;
#line 552 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    _endthis_chpl2:;
#line 552 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl18 = ret_chpl15;
#line 552 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    if (error_chpl != nil) /* ZLINE: 552 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 552 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    {
#line 552 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      i_x_chpl = &call_tmp_chpl14;
#line 552 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      deinit_chpl95(i_x_chpl, _ln_chpl, _fn_chpl);
#line 552 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      i_x_chpl2 = &call_tmp_chpl12;
#line 552 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      deinit_chpl95(i_x_chpl2, _ln_chpl, _fn_chpl);
#line 552 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      goto handler_chpl;
#line 552 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    }
#line 551 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    chpl___PLUS_(&call_tmp_chpl14, &call_tmp_chpl18, &ret_tmp_chpl7, _ln_chpl, _fn_chpl);
#line 551 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl20 = ret_tmp_chpl7;
#line 551 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    chpl___ASSIGN_16(&result_chpl, &call_tmp_chpl20, _ln_chpl, _fn_chpl);
#line 551 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    i_x_chpl3 = &call_tmp_chpl20;
#line 551 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    deinit_chpl95(i_x_chpl3, _ln_chpl, _fn_chpl);
#line 551 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    i_x_chpl4 = &call_tmp_chpl18;
#line 551 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    deinit_chpl95(i_x_chpl4, _ln_chpl, _fn_chpl);
#line 551 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    i_x_chpl5 = &call_tmp_chpl14;
#line 551 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    deinit_chpl95(i_x_chpl5, _ln_chpl, _fn_chpl);
#line 551 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    i_x_chpl6 = &call_tmp_chpl12;
#line 551 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    deinit_chpl95(i_x_chpl6, _ln_chpl, _fn_chpl);
#line 551 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    handler_chpl:;
#line 550 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    if (error_chpl != nil) /* ZLINE: 550 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 550 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    {
#line 550 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      chpl_uncaught_error(error_chpl, _ln_chpl, _fn_chpl);
#line 550 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    }
#line 555 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    ret_chpl16 = (&localReplacement_chpl)->buffLen;
#line 555 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl22 = ((&idx_chpl)->_bindex + ret_chpl16);
#line 555 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    init_chpl192(&cpi_chpl5);
#line 555 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    (&cpi_chpl5)->_bindex = call_tmp_chpl22;
#line 555 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    ret_chpl17 = cpi_chpl5;
#line 555 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl21 = ret_chpl17;
#line 555 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    chpl___ASSIGN_15(&startIdx_chpl, &call_tmp_chpl21);
#line 544 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    if (count_chpl < INT64(0)) /* ZLINE: 544 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 544 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    {
#line 544 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      tmp_chpl3 = UINT8(true);
#line 544 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    } else /* ZLINE: 544 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 544 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    {
#line 544 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      tmp_chpl3 = (found_chpl < count_chpl);
#line 544 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    }
#line 544 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    tmp_chpl = tmp_chpl3;
#line 544 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  }
#line 544 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _breakLabel_chpl:;
#line 557 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  ret_chpl = result_chpl;
#line 557 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  i_x_chpl7 = &localReplacement_chpl;
#line 557 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  deinit_chpl95(i_x_chpl7, _ln_chpl, _fn_chpl);
#line 557 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  i_x_chpl8 = &localNeedle_chpl;
#line 557 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  deinit_chpl95(i_x_chpl8, _ln_chpl, _fn_chpl);
#line 533 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  *(_retArg_chpl) = ret_chpl;
#line 533 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  return;
#line 533 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
}

#line 720 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
/* BytesStringCommon.chpl:720 */
#line 720 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
static int64_t doSearchNoEnc_chpl(string_chpl * x_chpl,
#line 720 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                                  string_chpl * needle_chpl,
#line 720 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                                  range_byteIndex_low_one_chpl region_chpl,
#line 720 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                                  int64_t _ln_chpl,
#line 720 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                                  int32_t _fn_chpl) {
#line 730 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t localRet_chpl;
#line 731 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t nLen_chpl;
#line 731 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t coerce_tmp_chpl3;
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _tuple_2_range_int64_t_both_one_int64_t_chpl call_tmp_chpl6;
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  Error_chpl error_chpl = NULL;
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _tuple_2_range_int64_t_both_one_int64_t_chpl ret_tmp_chpl;
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl tmp_x0_chpl;
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl view_chpl;
#line 736 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t call_tmp_chpl7;
#line 761 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl ret_chpl;
#line 761 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl ret_chpl2;
#line 761 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl localNeedle_chpl;
#line 766 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t call_tmp_chpl8;
#line 767 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl tmp_chpl;
#line 768 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_low_one_chpl call_tmp_chpl9;
#line 768 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_low_one_chpl ret_tmp_chpl2;
#line 767 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl3;
#line 770 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl _ic__F0_this_chpl;
#line 770 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t i_chpl;
#line 770 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl this_chpl31;
#line 770 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl this_chpl32;
#line 770 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t end_chpl;
#line 772 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl_bool call_tmp_chpl10;
#line 772 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl_bool call_tmp_chpl11;
#line 770 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t i_chpl2;
#line 772 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t tmp_chpl2;
#line 772 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t _ic__F1_high_chpl;
#line 772 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t i_chpl3;
#line 773 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t call_tmp_chpl12;
#line 774 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  c_ptr_uint8_t_chpl coerce_tmp_chpl4 = NULL;
#line 774 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref_uint8_t_chpl call_tmp_chpl13 = NULL;
#line 774 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  c_ptr_uint8_t_chpl coerce_tmp_chpl5 = NULL;
#line 774 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref_uint8_t_chpl call_tmp_chpl14 = NULL;
#line 780 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t call_tmp_chpl15;
#line 770 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 727 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl_buildLocaleID((x_chpl)->locale_id, c_sublocid_any, _ln_chpl, _fn_chpl);
#line 730 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  localRet_chpl = INT64(-2);
#line 731 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  coerce_tmp_chpl3 = (needle_chpl)->buffLen;
#line 731 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  nLen_chpl = coerce_tmp_chpl3;
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  error_chpl = NULL;
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  getView_chpl3(x_chpl, region_chpl, &error_chpl, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl6 = ret_tmp_chpl;
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  if (error_chpl != nil) /* ZLINE: 735 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  {
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    chpl_uncaught_error(error_chpl, _ln_chpl, _fn_chpl);
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  }
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  tmp_x0_chpl = (&call_tmp_chpl6)->x0;
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  view_chpl = tmp_x0_chpl;
#line 736 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl7 = size_chpl8(tmp_x0_chpl, _ln_chpl, _fn_chpl);
#line 744 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  if (coerce_tmp_chpl3 == INT64(0)) /* ZLINE: 744 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 744 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  {
#line 746 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    localRet_chpl = INT64(-1);
#line 746 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  }
#line 755 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  if (coerce_tmp_chpl3 > call_tmp_chpl7) /* ZLINE: 755 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 755 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  {
#line 756 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    localRet_chpl = INT64(-1);
#line 756 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  }
#line 759 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  if (localRet_chpl == INT64(-2)) /* ZLINE: 759 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 759 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  {
#line 760 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    localRet_chpl = INT64(-1);
#line 761 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    init_chpl194(&ret_chpl2, _ln_chpl, _fn_chpl);
#line 761 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    (&ret_chpl2)->cachedNumCodepoints = (needle_chpl)->cachedNumCodepoints;
#line 761 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    initWithBorrowedBuffer_chpl(&ret_chpl2, needle_chpl, _ln_chpl, _fn_chpl);
#line 761 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    ret_chpl = ret_chpl2;
#line 761 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    localNeedle_chpl = ret_chpl;
#line 766 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl8 = (((int64_t)((call_tmp_chpl7 - coerce_tmp_chpl3))) + INT64(1));
#line 768 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    chpl_build_low_bounded_range2(INT64(0), &ret_tmp_chpl2);
#line 768 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl9 = ret_tmp_chpl2;
#line 768 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    chpl___POUND_3(call_tmp_chpl9, call_tmp_chpl8, &ret_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 767 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    tmp_chpl = ret_tmp_chpl3;
#line 770 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    _ic__F0_this_chpl = tmp_chpl;
#line 770 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    checkIfIterWillOverflow_chpl(tmp_chpl, UINT8(true), _ln_chpl, _fn_chpl);
#line 770 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    i_chpl = INT64(0);
#line 770 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    this_chpl31 = _ic__F0_this_chpl;
#line 770 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    this_chpl32 = _ic__F0_this_chpl;
#line 770 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    end_chpl = (&this_chpl32)->_high;
#line 772 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl10 = (nLen_chpl < INT64(0));
#line 772 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl11 = (nLen_chpl == INT64(0));
#line 770 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    for (i_chpl = (&this_chpl31)->_low; ((i_chpl <= end_chpl)); i_chpl += INT64(1)) {
#line 770 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      i_chpl2 = i_chpl;
#line 772 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      if (call_tmp_chpl10) /* ZLINE: 772 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 772 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      {
#line 772 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        boundsCheckHalt_chpl(&_str_literal_580_chpl, _ln_chpl, _fn_chpl);
#line 772 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      }
#line 772 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      if (call_tmp_chpl11) /* ZLINE: 772 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 772 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      {
#line 772 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        tmp_chpl2 = ((int64_t)(((uint64_t)((((uint64_t)(INT64(0))) - UINT64(1))))));
#line 772 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      } else /* ZLINE: 772 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 772 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      {
#line 772 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        tmp_chpl2 = ((int64_t)((INT64(0) + ((int64_t)((nLen_chpl - INT64(1)))))));
#line 772 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      }
#line 772 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      _ic__F1_high_chpl = tmp_chpl2;
#line 772 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      i_chpl3 = INT64(0);
#line 772 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      chpl_checkIfRangeIterWillOverflow(INT64(0), tmp_chpl2, INT64(1), INT64(0), tmp_chpl2, UINT8(true), _ln_chpl, _fn_chpl);
#line 772 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      for (i_chpl3 = INT64(0); ((i_chpl3 <= _ic__F1_high_chpl)); i_chpl3 += INT64(1)) {
#line 773 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        call_tmp_chpl12 = orderToIndex_chpl2(view_chpl, ((int64_t)((i_chpl2 + i_chpl3))), _ln_chpl, _fn_chpl);
#line 774 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        coerce_tmp_chpl4 = (x_chpl)->buff;
#line 774 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        call_tmp_chpl13 = (coerce_tmp_chpl4 + call_tmp_chpl12);
#line 774 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        coerce_tmp_chpl5 = (&localNeedle_chpl)->buff;
#line 774 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        call_tmp_chpl14 = (coerce_tmp_chpl5 + i_chpl3);
#line 774 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        if (*(call_tmp_chpl13) != *(call_tmp_chpl14)) /* ZLINE: 774 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 774 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        {
#line 774 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          goto _breakLabel_chpl;
#line 774 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        }
#line 776 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        if (i_chpl3 == ((int64_t)((nLen_chpl - INT64(1))))) /* ZLINE: 776 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 776 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        {
#line 780 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          call_tmp_chpl15 = orderToIndex_chpl2(view_chpl, i_chpl2, _ln_chpl, _fn_chpl);
#line 780 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          localRet_chpl = call_tmp_chpl15;
#line 780 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        }
#line 780 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      }
#line 780 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      _breakLabel_chpl:;
#line 784 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      if (localRet_chpl != INT64(-1)) /* ZLINE: 784 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 784 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      {
#line 784 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        goto _breakLabel_chpl2;
#line 784 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      }
#line 784 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    }
#line 784 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    _breakLabel_chpl2:;
#line 770 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    i_x_chpl = &localNeedle_chpl;
#line 770 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    deinit_chpl95(i_x_chpl, _ln_chpl, _fn_chpl);
#line 770 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  }
#line 720 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  return localRet_chpl;
#line 720 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
}

#line 1029 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
/* BytesStringCommon.chpl:1029 */
#line 1029 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
static void doAppend_chpl(string_chpl * lhs_chpl,
#line 1029 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                          string_chpl * rhs_chpl,
#line 1029 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                          int64_t _ln_chpl,
#line 1029 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                          int32_t _fn_chpl) {
#line 1037 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl_bool call_tmp_chpl6;
#line 1039 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t call_tmp_chpl7;
#line 1042 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t call_tmp_chpl8;
#line 1043 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t call_tmp_chpl9;
#line 1042 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t tmp_chpl;
#line 1045 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _tuple_2_c_ptr_uint8_t_int64_t_chpl call_tmp_chpl10;
#line 1045 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _tuple_2_c_ptr_uint8_t_int64_t_chpl ret_tmp_chpl;
#line 1045 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t tmp_x1_chpl;
#line 1049 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _tuple_2_c_ptr_uint8_t_int64_t_chpl call_tmp_chpl11;
#line 1049 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _tuple_2_c_ptr_uint8_t_int64_t_chpl ret_chpl;
#line 1049 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t call_tmp_chpl12;
#line 1049 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t tmp_chpl2;
#line 1049 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int16_t call_tmp_chpl13;
#line 1049 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  c_ptr_void_chpl cast_tmp_chpl = NULL;
#line 1049 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  c_ptr_uint8_t_chpl tmp_x0_chpl = NULL;
#line 1049 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t tmp_x1_chpl2;
#line 1050 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t coerce_tmp_chpl3;
#line 1050 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  c_ptr_uint8_t_chpl call_tmp_chpl14 = NULL;
#line 1050 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  c_ptr_uint8_t_chpl call_tmp_chpl15 = NULL;
#line 1050 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _owned_IllegalArgumentError_chpl2 call_tmp_chpl16;
#line 1050 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _owned_IllegalArgumentError_chpl2 ret_tmp_chpl2;
#line 1050 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref__owned_IllegalArgumentError_chpl i_arg_chpl = NULL;
#line 1050 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  IllegalArgumentError_chpl call_tmp_chpl17 = NULL;
#line 1050 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl call_tmp_chpl18;
#line 1050 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  Error_chpl coerce_tmp_chpl4 = NULL;
#line 1050 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl ret_tmp_chpl3;
#line 1050 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 1057 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t coerce_tmp_chpl5;
#line 1057 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  c_ptr_uint8_t_chpl call_tmp_chpl19 = NULL;
#line 1057 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  c_ptr_uint8_t_chpl call_tmp_chpl20 = NULL;
#line 1057 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _owned_IllegalArgumentError_chpl2 call_tmp_chpl21;
#line 1057 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _owned_IllegalArgumentError_chpl2 ret_tmp_chpl4;
#line 1057 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref__owned_IllegalArgumentError_chpl i_arg_chpl2 = NULL;
#line 1057 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  IllegalArgumentError_chpl call_tmp_chpl22 = NULL;
#line 1057 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl call_tmp_chpl23;
#line 1057 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  Error_chpl coerce_tmp_chpl6 = NULL;
#line 1057 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl ret_tmp_chpl5;
#line 1057 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref_string_chpl i_x_chpl2 = NULL;
#line 1060 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  c_ptr_uint8_t_chpl coerce_tmp_chpl7 = NULL;
#line 1060 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref_uint8_t_chpl call_tmp_chpl24 = NULL;
#line 1061 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref_int64_t_chpl call_tmp_chpl25 = NULL;
#line 1033 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  if ((rhs_chpl)->buffLen == INT64(0)) /* ZLINE: 1033 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 1033 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  {
#line 1033 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    goto _end_doAppend_chpl;
#line 1033 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  }
#line 1036 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl_buildLocaleID((lhs_chpl)->locale_id, c_sublocid_any, _ln_chpl, _fn_chpl);
#line 1037 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl6 = safeAdd_chpl((lhs_chpl)->buffLen, (rhs_chpl)->buffLen);
#line 1037 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  if (! call_tmp_chpl6) /* ZLINE: 1037 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 1037 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  {
#line 1038 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    halt_chpl(&_str_literal_1344_chpl, _ln_chpl, _fn_chpl);
#line 1038 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  }
#line 1039 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl7 = ((lhs_chpl)->buffLen + (rhs_chpl)->buffLen);
#line 1041 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  if ((lhs_chpl)->buffSize <= call_tmp_chpl7) /* ZLINE: 1041 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 1041 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  {
#line 1042 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl8 = (call_tmp_chpl7 + INT64(1));
#line 1043 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl9 = ((int64_t)(((_real64)((((_real64)((lhs_chpl)->buffLen)) * REAL64(0x1.8p+0))))));
#line 1042 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    if (call_tmp_chpl8 > call_tmp_chpl9) /* ZLINE: 1042 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 1042 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    {
#line 1042 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      tmp_chpl = call_tmp_chpl8;
#line 1042 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    } else /* ZLINE: 1042 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 1042 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    {
#line 1042 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      tmp_chpl = call_tmp_chpl9;
#line 1042 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    }
#line 1044 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    if ((lhs_chpl)->isOwned) /* ZLINE: 1044 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 1044 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    {
#line 1045 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      bufferRealloc_chpl((lhs_chpl)->buff, tmp_chpl, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 1045 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      call_tmp_chpl10 = ret_tmp_chpl;
#line 1045 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      tmp_x1_chpl = (&call_tmp_chpl10)->x1;
#line 1046 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      (lhs_chpl)->buff = (&call_tmp_chpl10)->x0;
#line 1047 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      (lhs_chpl)->buffSize = tmp_x1_chpl;
#line 1047 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    } else /* ZLINE: 1048 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 1047 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    {
#line 1049 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      call_tmp_chpl12 = chpl_here_good_alloc_size(tmp_chpl, _ln_chpl, _fn_chpl);
#line 1049 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      if (call_tmp_chpl12 > INT64(0)) /* ZLINE: 1049 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 1049 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      {
#line 1049 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        tmp_chpl2 = call_tmp_chpl12;
#line 1049 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      } else /* ZLINE: 1049 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 1049 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      {
#line 1049 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        tmp_chpl2 = INT64(0);
#line 1049 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      }
#line 1049 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      call_tmp_chpl13 = offset_STR_COPY_DATA_chpl();
#line 1049 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      cast_tmp_chpl = chpl_here_alloc(tmp_chpl2, call_tmp_chpl13, _ln_chpl, _fn_chpl);
#line 1049 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      (&ret_chpl)->x0 = ((c_ptr_uint8_t_chpl)(cast_tmp_chpl));
#line 1049 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      (&ret_chpl)->x1 = tmp_chpl2;
#line 1049 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      call_tmp_chpl11 = ret_chpl;
#line 1049 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      tmp_x0_chpl = (&call_tmp_chpl11)->x0;
#line 1049 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      tmp_x1_chpl2 = (&call_tmp_chpl11)->x1;
#line 1050 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      coerce_tmp_chpl3 = (lhs_chpl)->buffLen;
#line 1050 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      call_tmp_chpl14 = (tmp_x0_chpl + INT64(0));
#line 1050 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      call_tmp_chpl15 = ((lhs_chpl)->buff + INT64(0));
#line 1050 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      chpl_checkValue7(coerce_tmp_chpl3, &ret_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 1050 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      call_tmp_chpl16 = ret_tmp_chpl2;
#line 1050 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      i_arg_chpl = &call_tmp_chpl16;
#line 1050 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      call_tmp_chpl17 = borrow_chpl2(i_arg_chpl);
#line 1050 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      if (((RootClass_chpl)(call_tmp_chpl17)) != nil) /* ZLINE: 1050 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 1050 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      {
#line 1050 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        coerce_tmp_chpl4 = ((Error_chpl)(call_tmp_chpl17));
#line 1050 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        chpl_check_nil(coerce_tmp_chpl4, _ln_chpl, _fn_chpl);
#line 1050 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        ((void(*)(Error_chpl,
#line 1050 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
           string_chpl *,
#line 1050 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
           int64_t,
#line 1050 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
           int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(coerce_tmp_chpl4))->chpl__cid) + INT64(1))])(coerce_tmp_chpl4, &ret_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 1050 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        call_tmp_chpl18 = ret_tmp_chpl3;
#line 1050 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        safeCastCheckHalt_chpl(&call_tmp_chpl18, _ln_chpl, _fn_chpl);
#line 1050 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        i_x_chpl = &call_tmp_chpl18;
#line 1050 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        deinit_chpl95(i_x_chpl, _ln_chpl, _fn_chpl);
#line 1050 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      }
#line 1050 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      chpl__autoDestroy62(&call_tmp_chpl16, _ln_chpl, _fn_chpl);
#line 1050 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      memcpy(((c_ptr_void_chpl)(call_tmp_chpl14)), ((c_ptr_void_chpl)(call_tmp_chpl15)), ((uint64_t)(coerce_tmp_chpl3)));
#line 1051 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      (lhs_chpl)->buff = tmp_x0_chpl;
#line 1052 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      (lhs_chpl)->buffSize = tmp_x1_chpl2;
#line 1053 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      (lhs_chpl)->isOwned = UINT8(true);
#line 1053 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    }
#line 1053 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  }
#line 1057 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  coerce_tmp_chpl5 = (rhs_chpl)->buffLen;
#line 1057 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl19 = ((lhs_chpl)->buff + (lhs_chpl)->buffLen);
#line 1057 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl20 = ((rhs_chpl)->buff + INT64(0));
#line 1057 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl_checkValue7(coerce_tmp_chpl5, &ret_tmp_chpl4, _ln_chpl, _fn_chpl);
#line 1057 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl21 = ret_tmp_chpl4;
#line 1057 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  i_arg_chpl2 = &call_tmp_chpl21;
#line 1057 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl22 = borrow_chpl2(i_arg_chpl2);
#line 1057 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  if (((RootClass_chpl)(call_tmp_chpl22)) != nil) /* ZLINE: 1057 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 1057 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  {
#line 1057 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    coerce_tmp_chpl6 = ((Error_chpl)(call_tmp_chpl22));
#line 1057 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    chpl_check_nil(coerce_tmp_chpl6, _ln_chpl, _fn_chpl);
#line 1057 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    ((void(*)(Error_chpl,
#line 1057 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
       string_chpl *,
#line 1057 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
       int64_t,
#line 1057 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
       int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(coerce_tmp_chpl6))->chpl__cid) + INT64(1))])(coerce_tmp_chpl6, &ret_tmp_chpl5, _ln_chpl, _fn_chpl);
#line 1057 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl23 = ret_tmp_chpl5;
#line 1057 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    safeCastCheckHalt_chpl(&call_tmp_chpl23, _ln_chpl, _fn_chpl);
#line 1057 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    i_x_chpl2 = &call_tmp_chpl23;
#line 1057 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    deinit_chpl95(i_x_chpl2, _ln_chpl, _fn_chpl);
#line 1057 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  }
#line 1057 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl__autoDestroy62(&call_tmp_chpl21, _ln_chpl, _fn_chpl);
#line 1057 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  memcpy(((c_ptr_void_chpl)(call_tmp_chpl19)), ((c_ptr_void_chpl)(call_tmp_chpl20)), ((uint64_t)(coerce_tmp_chpl5)));
#line 1059 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  (lhs_chpl)->buffLen = call_tmp_chpl7;
#line 1060 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  coerce_tmp_chpl7 = (lhs_chpl)->buff;
#line 1060 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl24 = (coerce_tmp_chpl7 + call_tmp_chpl7);
#line 1060 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  *(call_tmp_chpl24) = ((uint8_t)(INT64(0)));
#line 1061 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl25 = &((lhs_chpl)->cachedNumCodepoints);
#line 1061 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  *(call_tmp_chpl25) += (rhs_chpl)->cachedNumCodepoints;
#line 1061 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _end_doAppend_chpl:;
#line 1029 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  return;
#line 1029 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
}

#line 1067 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
/* BytesStringCommon.chpl:1067 */
#line 1067 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
static void resizeBuffer_chpl(string_chpl * lhs_chpl,
#line 1067 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                              int64_t buffLen_chpl,
#line 1067 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                              int64_t _ln_chpl,
#line 1067 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                              int32_t _fn_chpl) {
#line 1068 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl_bool tmp_chpl;
#line 1073 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl_bool tmp_chpl2;
#line 1076 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl_bool tmp_chpl3;
#line 1079 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _tuple_2_c_ptr_uint8_t_int64_t_chpl call_tmp_chpl6;
#line 1079 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _tuple_2_c_ptr_uint8_t_int64_t_chpl ret_chpl;
#line 1079 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t call_tmp_chpl7;
#line 1079 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t tmp_chpl4;
#line 1079 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int16_t call_tmp_chpl8;
#line 1079 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  c_ptr_void_chpl cast_tmp_chpl = NULL;
#line 1079 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t tmp_x1_chpl;
#line 1088 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl_bool tmp_chpl5;
#line 1068 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  if ((lhs_chpl)->buffLen == INT64(0)) /* ZLINE: 1068 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 1068 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  {
#line 1068 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    tmp_chpl = (buffLen_chpl == INT64(0));
#line 1068 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  } else /* ZLINE: 1068 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 1068 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  {
#line 1068 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    tmp_chpl = UINT8(false);
#line 1068 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  }
#line 1068 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  if (tmp_chpl) /* ZLINE: 1068 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 1068 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  {
#line 1068 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    goto _end_resizeBuffer_chpl;
#line 1068 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  }
#line 1072 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  if (buffLen_chpl != INT64(0)) /* ZLINE: 1072 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 1072 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  {
#line 1073 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    if (! (lhs_chpl)->isOwned) /* ZLINE: 1073 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 1073 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    {
#line 1073 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      tmp_chpl2 = UINT8(true);
#line 1073 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    } else /* ZLINE: 1073 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 1073 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    {
#line 1073 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      tmp_chpl2 = (((int64_t)((buffLen_chpl + INT64(1)))) > (lhs_chpl)->buffSize);
#line 1073 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    }
#line 1073 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    if (tmp_chpl2) /* ZLINE: 1073 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 1073 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    {
#line 1076 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      if ((lhs_chpl)->isOwned) /* ZLINE: 1076 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 1076 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      {
#line 1076 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        tmp_chpl3 = (! ((lhs_chpl)->buffLen == INT64(0)));
#line 1076 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      } else /* ZLINE: 1076 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 1076 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      {
#line 1076 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        tmp_chpl3 = UINT8(false);
#line 1076 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      }
#line 1076 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      if (tmp_chpl3) /* ZLINE: 1076 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 1076 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      {
#line 1077 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        chpl_here_free(((c_ptr_void_chpl)((lhs_chpl)->buff)), _ln_chpl, _fn_chpl);
#line 1077 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      }
#line 1079 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      call_tmp_chpl7 = chpl_here_good_alloc_size(((int64_t)((buffLen_chpl + INT64(1)))), _ln_chpl, _fn_chpl);
#line 1079 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      if (call_tmp_chpl7 > INT64(0)) /* ZLINE: 1079 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 1079 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      {
#line 1079 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        tmp_chpl4 = call_tmp_chpl7;
#line 1079 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      } else /* ZLINE: 1079 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 1079 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      {
#line 1079 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        tmp_chpl4 = INT64(0);
#line 1079 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      }
#line 1079 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      call_tmp_chpl8 = offset_STR_COPY_DATA_chpl();
#line 1079 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      cast_tmp_chpl = chpl_here_alloc(tmp_chpl4, call_tmp_chpl8, _ln_chpl, _fn_chpl);
#line 1079 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      (&ret_chpl)->x0 = ((c_ptr_uint8_t_chpl)(cast_tmp_chpl));
#line 1079 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      (&ret_chpl)->x1 = tmp_chpl4;
#line 1079 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      call_tmp_chpl6 = ret_chpl;
#line 1079 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      tmp_x1_chpl = (&call_tmp_chpl6)->x1;
#line 1080 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      (lhs_chpl)->buff = (&call_tmp_chpl6)->x0;
#line 1081 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      (lhs_chpl)->buffSize = tmp_x1_chpl;
#line 1083 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      (lhs_chpl)->isOwned = UINT8(true);
#line 1083 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    }
#line 1083 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  } else /* ZLINE: 1085 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 1083 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  {
#line 1088 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    if ((lhs_chpl)->isOwned) /* ZLINE: 1088 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 1088 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    {
#line 1088 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      tmp_chpl5 = (! ((lhs_chpl)->buffLen == INT64(0)));
#line 1088 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    } else /* ZLINE: 1088 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 1088 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    {
#line 1088 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      tmp_chpl5 = UINT8(false);
#line 1088 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    }
#line 1088 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    if (tmp_chpl5) /* ZLINE: 1088 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 1088 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    {
#line 1088 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      chpl_here_free(((c_ptr_void_chpl)((lhs_chpl)->buff)), _ln_chpl, _fn_chpl);
#line 1088 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    }
#line 1089 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    (lhs_chpl)->buffSize = INT64(0);
#line 1094 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    (lhs_chpl)->buff = ((c_ptr_uint8_t_chpl)(nil));
#line 1094 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  }
#line 1094 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _end_resizeBuffer_chpl:;
#line 1067 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  return;
#line 1067 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
}

#line 1098 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
/* BytesStringCommon.chpl:1098 */
#line 1098 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
static void reinitWithNewBuffer_chpl(string_chpl * lhs_chpl,
#line 1098 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                                     uint8_t * buff_chpl,
#line 1098 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                                     int64_t buffLen_chpl,
#line 1098 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                                     int64_t buffSize_chpl,
#line 1098 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                                     int64_t numCodepoints_chpl,
#line 1098 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                                     int64_t _ln_chpl,
#line 1098 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                                     int32_t _fn_chpl) {
#line 1100 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl_bool tmp_chpl;
#line 1104 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  c_ptr_uint8_t_chpl call_tmp_chpl6 = NULL;
#line 1104 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  c_ptr_uint8_t_chpl call_tmp_chpl7 = NULL;
#line 1104 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _owned_IllegalArgumentError_chpl2 call_tmp_chpl8;
#line 1104 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _owned_IllegalArgumentError_chpl2 ret_tmp_chpl;
#line 1104 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref__owned_IllegalArgumentError_chpl i_arg_chpl = NULL;
#line 1104 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  IllegalArgumentError_chpl call_tmp_chpl9 = NULL;
#line 1104 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl call_tmp_chpl10;
#line 1104 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  Error_chpl coerce_tmp_chpl3 = NULL;
#line 1104 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl ret_tmp_chpl2;
#line 1104 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 1105 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  c_ptr_uint8_t_chpl coerce_tmp_chpl4 = NULL;
#line 1105 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref_uint8_t_chpl call_tmp_chpl11 = NULL;
#line 1100 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  if ((lhs_chpl)->buffLen == INT64(0)) /* ZLINE: 1100 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 1100 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  {
#line 1100 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    tmp_chpl = (buff_chpl == nil);
#line 1100 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  } else /* ZLINE: 1100 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 1100 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  {
#line 1100 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    tmp_chpl = UINT8(false);
#line 1100 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  }
#line 1100 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  if (tmp_chpl) /* ZLINE: 1100 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 1100 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  {
#line 1100 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    goto _end_reinitWithNewBuffer_chpl;
#line 1100 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  }
#line 1102 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  resizeBuffer_chpl(lhs_chpl, buffLen_chpl, _ln_chpl, _fn_chpl);
#line 1103 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  if (buffLen_chpl != INT64(0)) /* ZLINE: 1103 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 1103 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  {
#line 1104 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl6 = ((lhs_chpl)->buff + INT64(0));
#line 1104 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl7 = (buff_chpl + INT64(0));
#line 1104 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    chpl_checkValue7(buffLen_chpl, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 1104 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl8 = ret_tmp_chpl;
#line 1104 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    i_arg_chpl = &call_tmp_chpl8;
#line 1104 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl9 = borrow_chpl2(i_arg_chpl);
#line 1104 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    if (((RootClass_chpl)(call_tmp_chpl9)) != nil) /* ZLINE: 1104 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 1104 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    {
#line 1104 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      coerce_tmp_chpl3 = ((Error_chpl)(call_tmp_chpl9));
#line 1104 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      chpl_check_nil(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 1104 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      ((void(*)(Error_chpl,
#line 1104 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
         string_chpl *,
#line 1104 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
         int64_t,
#line 1104 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
         int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(coerce_tmp_chpl3))->chpl__cid) + INT64(1))])(coerce_tmp_chpl3, &ret_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 1104 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      call_tmp_chpl10 = ret_tmp_chpl2;
#line 1104 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      safeCastCheckHalt_chpl(&call_tmp_chpl10, _ln_chpl, _fn_chpl);
#line 1104 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      i_x_chpl = &call_tmp_chpl10;
#line 1104 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      deinit_chpl95(i_x_chpl, _ln_chpl, _fn_chpl);
#line 1104 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    }
#line 1104 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    chpl__autoDestroy62(&call_tmp_chpl8, _ln_chpl, _fn_chpl);
#line 1104 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    memmove(((c_ptr_void_chpl)(call_tmp_chpl6)), ((c_ptr_void_chpl)(call_tmp_chpl7)), ((uint64_t)(buffLen_chpl)));
#line 1105 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    coerce_tmp_chpl4 = (lhs_chpl)->buff;
#line 1105 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl11 = (coerce_tmp_chpl4 + buffLen_chpl);
#line 1105 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    *(call_tmp_chpl11) = ((uint8_t)(INT64(0)));
#line 1105 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  }
#line 1108 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  (lhs_chpl)->buffLen = buffLen_chpl;
#line 1109 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  (lhs_chpl)->cachedNumCodepoints = numCodepoints_chpl;
#line 1109 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _end_reinitWithNewBuffer_chpl:;
#line 1098 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  return;
#line 1098 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
}

#line 1137 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
/* BytesStringCommon.chpl:1137 */
#line 1137 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
static void doAssign_chpl(string_chpl * lhs_chpl,
#line 1137 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                          string_chpl * rhs_chpl,
#line 1137 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                          int64_t _ln_chpl,
#line 1137 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                          int32_t _fn_chpl) {
#line 1165 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t ret_chpl;
#line 1165 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  ret_chpl = (rhs_chpl)->cachedNumCodepoints;
#line 1165 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  reinitWithNewBuffer_chpl(lhs_chpl, (rhs_chpl)->buff, (rhs_chpl)->buffLen, (rhs_chpl)->buffSize, ret_chpl, _ln_chpl, _fn_chpl);
#line 1137 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  return;
#line 1137 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
}

#line 1215 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
/* BytesStringCommon.chpl:1215 */
#line 1215 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
static void doConcat_chpl(string_chpl * s0_chpl,
#line 1215 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                          string_chpl * s1_chpl,
#line 1215 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                          string_chpl * _retArg_chpl,
#line 1215 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                          int64_t _ln_chpl,
#line 1215 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                          int32_t _fn_chpl) {
#line 1215 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl ret_chpl;
#line 1219 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t s0len_chpl;
#line 1219 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t coerce_tmp_chpl3;
#line 1220 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl ret_chpl2;
#line 1220 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl tmp_chpl;
#line 1221 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t s1len_chpl;
#line 1221 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t coerce_tmp_chpl4;
#line 1222 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl ret_chpl3;
#line 1222 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl tmp_chpl2;
#line 1224 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t call_tmp_chpl6;
#line 1225 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _tuple_2_c_ptr_uint8_t_int64_t_chpl call_tmp_chpl7;
#line 1225 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _tuple_2_c_ptr_uint8_t_int64_t_chpl ret_chpl4;
#line 1225 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t call_tmp_chpl8;
#line 1225 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t tmp_chpl3;
#line 1225 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int16_t call_tmp_chpl9;
#line 1225 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  c_ptr_void_chpl cast_tmp_chpl = NULL;
#line 1225 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  c_ptr_uint8_t_chpl tmp_x0_chpl = NULL;
#line 1225 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t tmp_x1_chpl;
#line 1225 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  c_ptr_uint8_t_chpl buff_chpl = NULL;
#line 1227 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  c_ptr_uint8_t_chpl call_tmp_chpl10 = NULL;
#line 1227 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  c_ptr_uint8_t_chpl call_tmp_chpl11 = NULL;
#line 1227 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _owned_IllegalArgumentError_chpl2 call_tmp_chpl12;
#line 1227 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _owned_IllegalArgumentError_chpl2 ret_tmp_chpl;
#line 1227 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref__owned_IllegalArgumentError_chpl i_arg_chpl = NULL;
#line 1227 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  IllegalArgumentError_chpl call_tmp_chpl13 = NULL;
#line 1227 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl call_tmp_chpl14;
#line 1227 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  Error_chpl coerce_tmp_chpl5 = NULL;
#line 1227 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl ret_tmp_chpl2;
#line 1227 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 1228 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  c_ptr_uint8_t_chpl call_tmp_chpl15 = NULL;
#line 1228 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  c_ptr_uint8_t_chpl call_tmp_chpl16 = NULL;
#line 1228 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _owned_IllegalArgumentError_chpl2 call_tmp_chpl17;
#line 1228 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _owned_IllegalArgumentError_chpl2 ret_tmp_chpl3;
#line 1228 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref__owned_IllegalArgumentError_chpl i_arg_chpl2 = NULL;
#line 1228 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  IllegalArgumentError_chpl call_tmp_chpl18 = NULL;
#line 1228 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl call_tmp_chpl19;
#line 1228 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  Error_chpl coerce_tmp_chpl6 = NULL;
#line 1228 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl ret_tmp_chpl4;
#line 1228 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref_string_chpl i_x_chpl2 = NULL;
#line 1230 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref_uint8_t_chpl call_tmp_chpl20 = NULL;
#line 1232 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl ret_chpl5;
#line 1236 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref_string_chpl i_x_chpl3 = NULL;
#line 1219 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  coerce_tmp_chpl3 = (s0_chpl)->buffLen;
#line 1219 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  s0len_chpl = coerce_tmp_chpl3;
#line 1220 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  if (coerce_tmp_chpl3 == INT64(0)) /* ZLINE: 1220 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 1220 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  {
#line 1220 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    init_ASSIGN__chpl25(&tmp_chpl, s1_chpl, _ln_chpl, _fn_chpl);
#line 1220 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    ret_chpl2 = tmp_chpl;
#line 1220 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    ret_chpl = ret_chpl2;
#line 1220 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    goto _end_doConcat_chpl;
#line 1220 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  }
#line 1221 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  coerce_tmp_chpl4 = (s1_chpl)->buffLen;
#line 1221 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  s1len_chpl = coerce_tmp_chpl4;
#line 1222 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  if (coerce_tmp_chpl4 == INT64(0)) /* ZLINE: 1222 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 1222 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  {
#line 1222 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    init_ASSIGN__chpl25(&tmp_chpl2, s0_chpl, _ln_chpl, _fn_chpl);
#line 1222 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    ret_chpl3 = tmp_chpl2;
#line 1222 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    ret_chpl = ret_chpl3;
#line 1222 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    goto _end_doConcat_chpl;
#line 1222 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  }
#line 1224 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl6 = (coerce_tmp_chpl3 + coerce_tmp_chpl4);
#line 1225 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl8 = chpl_here_good_alloc_size(((int64_t)((call_tmp_chpl6 + INT64(1)))), _ln_chpl, _fn_chpl);
#line 1225 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  if (call_tmp_chpl8 > INT64(0)) /* ZLINE: 1225 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 1225 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  {
#line 1225 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    tmp_chpl3 = call_tmp_chpl8;
#line 1225 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  } else /* ZLINE: 1225 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 1225 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  {
#line 1225 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    tmp_chpl3 = INT64(0);
#line 1225 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  }
#line 1225 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl9 = offset_STR_COPY_DATA_chpl();
#line 1225 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  cast_tmp_chpl = chpl_here_alloc(tmp_chpl3, call_tmp_chpl9, _ln_chpl, _fn_chpl);
#line 1225 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  (&ret_chpl4)->x0 = ((c_ptr_uint8_t_chpl)(cast_tmp_chpl));
#line 1225 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  (&ret_chpl4)->x1 = tmp_chpl3;
#line 1225 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl7 = ret_chpl4;
#line 1225 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  tmp_x0_chpl = (&call_tmp_chpl7)->x0;
#line 1225 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  tmp_x1_chpl = (&call_tmp_chpl7)->x1;
#line 1225 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  buff_chpl = tmp_x0_chpl;
#line 1227 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl10 = (tmp_x0_chpl + INT64(0));
#line 1227 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl11 = ((s0_chpl)->buff + INT64(0));
#line 1227 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl_checkValue7(coerce_tmp_chpl3, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 1227 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl12 = ret_tmp_chpl;
#line 1227 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  i_arg_chpl = &call_tmp_chpl12;
#line 1227 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl13 = borrow_chpl2(i_arg_chpl);
#line 1227 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  if (((RootClass_chpl)(call_tmp_chpl13)) != nil) /* ZLINE: 1227 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 1227 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  {
#line 1227 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    coerce_tmp_chpl5 = ((Error_chpl)(call_tmp_chpl13));
#line 1227 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    chpl_check_nil(coerce_tmp_chpl5, _ln_chpl, _fn_chpl);
#line 1227 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    ((void(*)(Error_chpl,
#line 1227 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
       string_chpl *,
#line 1227 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
       int64_t,
#line 1227 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
       int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(coerce_tmp_chpl5))->chpl__cid) + INT64(1))])(coerce_tmp_chpl5, &ret_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 1227 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl14 = ret_tmp_chpl2;
#line 1227 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    safeCastCheckHalt_chpl(&call_tmp_chpl14, _ln_chpl, _fn_chpl);
#line 1227 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    i_x_chpl = &call_tmp_chpl14;
#line 1227 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    deinit_chpl95(i_x_chpl, _ln_chpl, _fn_chpl);
#line 1227 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  }
#line 1227 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl__autoDestroy62(&call_tmp_chpl12, _ln_chpl, _fn_chpl);
#line 1227 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  memcpy(((c_ptr_void_chpl)(call_tmp_chpl10)), ((c_ptr_void_chpl)(call_tmp_chpl11)), ((uint64_t)(s0len_chpl)));
#line 1228 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl15 = (tmp_x0_chpl + s0len_chpl);
#line 1228 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl16 = ((s1_chpl)->buff + INT64(0));
#line 1228 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl_checkValue7(coerce_tmp_chpl4, &ret_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 1228 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl17 = ret_tmp_chpl3;
#line 1228 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  i_arg_chpl2 = &call_tmp_chpl17;
#line 1228 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl18 = borrow_chpl2(i_arg_chpl2);
#line 1228 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  if (((RootClass_chpl)(call_tmp_chpl18)) != nil) /* ZLINE: 1228 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 1228 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  {
#line 1228 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    coerce_tmp_chpl6 = ((Error_chpl)(call_tmp_chpl18));
#line 1228 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    chpl_check_nil(coerce_tmp_chpl6, _ln_chpl, _fn_chpl);
#line 1228 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    ((void(*)(Error_chpl,
#line 1228 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
       string_chpl *,
#line 1228 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
       int64_t,
#line 1228 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
       int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(coerce_tmp_chpl6))->chpl__cid) + INT64(1))])(coerce_tmp_chpl6, &ret_tmp_chpl4, _ln_chpl, _fn_chpl);
#line 1228 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl19 = ret_tmp_chpl4;
#line 1228 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    safeCastCheckHalt_chpl(&call_tmp_chpl19, _ln_chpl, _fn_chpl);
#line 1228 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    i_x_chpl2 = &call_tmp_chpl19;
#line 1228 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    deinit_chpl95(i_x_chpl2, _ln_chpl, _fn_chpl);
#line 1228 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  }
#line 1228 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl__autoDestroy62(&call_tmp_chpl17, _ln_chpl, _fn_chpl);
#line 1228 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  memcpy(((c_ptr_void_chpl)(call_tmp_chpl15)), ((c_ptr_void_chpl)(call_tmp_chpl16)), ((uint64_t)(s1len_chpl)));
#line 1230 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl20 = (buff_chpl + call_tmp_chpl6);
#line 1230 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  *(call_tmp_chpl20) = ((uint8_t)(INT64(0)));
#line 1232 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  init_chpl194(&ret_chpl5, _ln_chpl, _fn_chpl);
#line 1234 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  (&ret_chpl5)->cachedNumCodepoints = ((int64_t)(((s0_chpl)->cachedNumCodepoints + (s1_chpl)->cachedNumCodepoints)));
#line 1236 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  i_x_chpl3 = &ret_chpl5;
#line 1236 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  (i_x_chpl3)->isOwned = UINT8(true);
#line 1236 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  (i_x_chpl3)->buff = tmp_x0_chpl;
#line 1236 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  (i_x_chpl3)->buffSize = tmp_x1_chpl;
#line 1236 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  (i_x_chpl3)->buffLen = call_tmp_chpl6;
#line 1237 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  ret_chpl = ret_chpl5;
#line 1237 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _end_doConcat_chpl:;
#line 1215 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  *(_retArg_chpl) = ret_chpl;
#line 1215 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  return;
#line 1215 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
}

#line 1240 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
/* BytesStringCommon.chpl:1240 */
#line 1240 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
static void doStripNoEnc_chpl(string_chpl * x_chpl,
#line 1240 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                              string_chpl * chars_chpl,
#line 1240 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                              chpl_bool leading_chpl,
#line 1240 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                              chpl_bool trailing_chpl,
#line 1240 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                              string_chpl * _retArg_chpl,
#line 1240 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                              int64_t _ln_chpl,
#line 1240 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                              int32_t _fn_chpl) {
#line 1240 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl ret_chpl;
#line 1241 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl inlineImm_chpl;
#line 1241 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 1241 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl ret_chpl2;
#line 1241 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl tmp_chpl;
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl ret_chpl3;
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl tmp_chpl2;
#line 1245 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl ret_chpl4;
#line 1245 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl ret_chpl5;
#line 1245 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl localX_chpl;
#line 1246 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl ret_chpl6;
#line 1246 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl ret_chpl7;
#line 1246 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl localChars_chpl;
#line 1248 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t start_chpl2;
#line 1249 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t end_chpl;
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t ret_chpl8;
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t call_tmp_chpl6;
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl7;
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl;
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ic_these_range_int64_t_both_one_chpl _ic__chpl = NULL;
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  c_ptr_void_chpl cast_tmp_chpl = NULL;
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref_string_chpl i_this_chpl = NULL;
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ic_chpl_bytes__ref_string_chpl _ic__chpl2 = NULL;
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  c_ptr_void_chpl cast_tmp_chpl2 = NULL;
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl tmp_chpl3;
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl tmp_chpl4;
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl tmp_chpl5;
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t coerce_tmp_chpl3;
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t coerce_tmp_chpl4;
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t coerce_tmp_chpl5;
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t low_chpl2;
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t count_chpl;
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t low_chpl3;
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t tmp_chpl6;
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t low_chpl4;
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t high_chpl;
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t low_chpl5;
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref_string_chpl tmp_chpl7 = NULL;
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref_string_chpl tmp_chpl8 = NULL;
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref__ic_chpl_direct_counted_range_iter_chpl tmp_chpl9 = NULL;
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref__ic_chpl_direct_counted_range_iter_chpl tmp_chpl10 = NULL;
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref__ic_chpl_direct_counted_range_iter_helper_chpl tmp_chpl11 = NULL;
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref__ic_chpl_direct_counted_range_iter_helper_chpl tmp_chpl12 = NULL;
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref__ic_chpl_direct_counted_range_iter_helper_chpl tmp_chpl13 = NULL;
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref__ic_chpl_direct_param_stride_range_iter_chpl tmp_chpl14 = NULL;
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref__ic_chpl_direct_param_stride_range_iter_chpl tmp_chpl15 = NULL;
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref__ic_chpl_direct_param_stride_range_iter_chpl tmp_chpl16 = NULL;
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref__ic_chpl_direct_param_stride_range_iter_chpl tmp_chpl17 = NULL;
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl ret_chpl9;
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl ret_chpl10;
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ic_chpl_direct_counted_range_iter_chpl _ic__chpl3 = NULL;
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  c_ptr_void_chpl cast_tmp_chpl3 = NULL;
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ic_chpl_direct_counted_range_iter_helper_chpl _ic__chpl4 = NULL;
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  c_ptr_void_chpl cast_tmp_chpl4 = NULL;
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref__ic_chpl_direct_counted_range_iter_helper_chpl tmp_chpl18 = NULL;
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref__ic_chpl_direct_counted_range_iter_helper_chpl tmp_chpl19 = NULL;
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t call_tmp_chpl8;
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref__ic_chpl_direct_counted_range_iter_helper_chpl tmp_chpl20 = NULL;
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ic_chpl_direct_param_stride_range_iter_chpl _ic__chpl5 = NULL;
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  c_ptr_void_chpl cast_tmp_chpl5 = NULL;
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl_bool hasMore_chpl;
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t tmp_chpl21;
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  c_ptr_uint8_t_chpl coerce_tmp_chpl6 = NULL;
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t tmp_chpl22;
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref_string_chpl tmp_chpl23 = NULL;
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  uint8_t tmp_chpl24;
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref_uint8_t_chpl call_tmp_chpl9 = NULL;
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t tmp_chpl25;
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t high_chpl2;
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref__ic_chpl_direct_param_stride_range_iter_chpl tmp_chpl26 = NULL;
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  uint8_t tmp_chpl27;
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t tmp_chpl28;
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t i_chpl;
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  uint8_t xChar_chpl;
#line 1253 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref_string_chpl i_this_chpl2 = NULL;
#line 1253 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl ret_chpl11;
#line 1253 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl ret_chpl12;
#line 1253 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl localThis_chpl;
#line 1253 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t coerce_tmp_chpl7;
#line 1253 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t tmp_chpl29;
#line 1253 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t _ic__F1_high_chpl;
#line 1253 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t i_chpl2;
#line 1253 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  uint8_t yret_chpl;
#line 1253 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  c_ptr_uint8_t_chpl coerce_tmp_chpl8 = NULL;
#line 1253 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref_uint8_t_chpl call_tmp_chpl10 = NULL;
#line 1253 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref_string_chpl i_x_chpl2 = NULL;
#line 1253 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref_string_chpl i_x_chpl3 = NULL;
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t moreVal1_chpl;
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t moreValNeg1_chpl;
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t moreVal2_chpl;
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t moreValNeg2_chpl;
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t tmp_chpl30;
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t tmp_chpl31;
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref__ic_chpl_direct_param_stride_range_iter_chpl tmp_chpl32 = NULL;
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t tmp_chpl33;
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t tmp_chpl34;
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t tmp_chpl35;
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t tmp_chpl36;
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t tmp_chpl37;
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t high_chpl3;
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref__ic_chpl_direct_param_stride_range_iter_chpl tmp_chpl38 = NULL;
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref_string_chpl tmp_chpl39 = NULL;
#line 1269 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t ret_chpl13;
#line 1269 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t call_tmp_chpl11;
#line 1269 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl12;
#line 1269 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl2;
#line 1269 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl _ic__F0_this_chpl;
#line 1269 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t _ic__F1_i_chpl;
#line 1269 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t _ic__F2_end_chpl;
#line 1269 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref_string_chpl i_this_chpl3 = NULL;
#line 1269 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl _ic__F1_localThis_chpl;
#line 1269 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t _ic__F5_i_chpl;
#line 1269 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl tmp_chpl40;
#line 1269 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl tmp_chpl41;
#line 1269 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t coerce_tmp_chpl9;
#line 1269 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t coerce_tmp_chpl10;
#line 1269 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t tmp_chpl42;
#line 1269 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl ret_chpl14;
#line 1269 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl ret_chpl15;
#line 1269 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t _ic__F1_high_chpl2;
#line 1269 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl_bool hasMore_chpl2;
#line 1269 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  c_ptr_uint8_t_chpl coerce_tmp_chpl11 = NULL;
#line 1269 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  uint8_t tmp_chpl43;
#line 1269 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref_uint8_t_chpl call_tmp_chpl13 = NULL;
#line 1269 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t tmp_chpl44;
#line 1269 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t i_chpl3;
#line 1269 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  uint8_t xChar_chpl2;
#line 1270 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref_string_chpl i_this_chpl4 = NULL;
#line 1270 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl ret_chpl16;
#line 1270 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl ret_chpl17;
#line 1270 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl localThis_chpl2;
#line 1270 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t coerce_tmp_chpl12;
#line 1270 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t tmp_chpl45;
#line 1270 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t _ic__F1_high_chpl3;
#line 1270 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t i_chpl4;
#line 1270 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  uint8_t yret_chpl2;
#line 1270 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  c_ptr_uint8_t_chpl coerce_tmp_chpl13 = NULL;
#line 1270 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref_uint8_t_chpl call_tmp_chpl14 = NULL;
#line 1270 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref_string_chpl i_x_chpl4 = NULL;
#line 1270 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref_string_chpl i_x_chpl5 = NULL;
#line 1269 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t tmp_chpl46;
#line 1269 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t tmp_chpl47;
#line 1269 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t tmp_chpl48;
#line 1269 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t tmp_chpl49;
#line 1269 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref_string_chpl tmp_chpl50 = NULL;
#line 1280 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl15;
#line 1280 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl3;
#line 1280 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref_string_chpl i_this_chpl5 = NULL;
#line 1280 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl call_tmp_chpl16;
#line 1280 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  Error_chpl error_chpl = NULL;
#line 1280 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl ret_tmp_chpl4;
#line 1280 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref_string_chpl i_x_chpl6 = NULL;
#line 1280 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref_string_chpl i_x_chpl7 = NULL;
#line 1241 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  if ((x_chpl)->buffLen == INT64(0)) /* ZLINE: 1241 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 1241 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  {
#line 1241 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    inlineImm_chpl = _str_literal_163_chpl;
#line 1241 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    i_x_chpl = &inlineImm_chpl;
#line 1241 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    init_ASSIGN__chpl25(&tmp_chpl, i_x_chpl, _ln_chpl, _fn_chpl);
#line 1241 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    ret_chpl2 = tmp_chpl;
#line 1241 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    ret_chpl = ret_chpl2;
#line 1241 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    goto _end_doStripNoEnc_chpl;
#line 1241 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  }
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  if ((chars_chpl)->buffLen == INT64(0)) /* ZLINE: 1243 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  {
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    init_ASSIGN__chpl25(&tmp_chpl2, x_chpl, _ln_chpl, _fn_chpl);
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    ret_chpl3 = tmp_chpl2;
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    ret_chpl = ret_chpl3;
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    goto _end_doStripNoEnc_chpl;
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  }
#line 1245 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  init_chpl194(&ret_chpl5, _ln_chpl, _fn_chpl);
#line 1245 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  (&ret_chpl5)->cachedNumCodepoints = (x_chpl)->cachedNumCodepoints;
#line 1245 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  initWithBorrowedBuffer_chpl(&ret_chpl5, x_chpl, _ln_chpl, _fn_chpl);
#line 1245 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  ret_chpl4 = ret_chpl5;
#line 1245 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  localX_chpl = ret_chpl4;
#line 1246 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  init_chpl194(&ret_chpl7, _ln_chpl, _fn_chpl);
#line 1246 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  (&ret_chpl7)->cachedNumCodepoints = (chars_chpl)->cachedNumCodepoints;
#line 1246 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  initWithBorrowedBuffer_chpl(&ret_chpl7, chars_chpl, _ln_chpl, _fn_chpl);
#line 1246 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  ret_chpl6 = ret_chpl7;
#line 1246 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  localChars_chpl = ret_chpl6;
#line 1248 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  start_chpl2 = INT64(0);
#line 1249 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  end_chpl = ((int64_t)(((&localX_chpl)->buffLen - INT64(1))));
#line 1251 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  if (leading_chpl) /* ZLINE: 1251 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 1251 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  {
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    ret_chpl8 = (x_chpl)->cachedNumCodepoints;
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl6 = chpl__nudgeHighBound(ret_chpl8);
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    chpl_build_bounded_range6(INT64(0), call_tmp_chpl6, &ret_tmp_chpl);
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl7 = ret_tmp_chpl;
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    cast_tmp_chpl = chpl_here_alloc(sizeof(chpl__ic_these_range_int64_t_both_one_chpl_object), INT16(29), _ln_chpl, _fn_chpl);
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    _ic__chpl = ((_ic_these_range_int64_t_both_one_chpl)(cast_tmp_chpl));
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    ((RootClass_chpl)(_ic__chpl))->chpl__cid = chpl__cid__ic_these_range_int64_t_both_one_chpl;
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    chpl_check_nil(_ic__chpl, _ln_chpl, _fn_chpl);
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    (_ic__chpl)->more = INT64(1);
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    chpl_check_nil(_ic__chpl, _ln_chpl, _fn_chpl);
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    (_ic__chpl)->F0_this = call_tmp_chpl7;
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    i_this_chpl = &localX_chpl;
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    cast_tmp_chpl2 = chpl_here_alloc(sizeof(chpl__ic_chpl_bytes__ref_string_chpl_object), INT16(30), _ln_chpl, _fn_chpl);
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    _ic__chpl2 = ((_ic_chpl_bytes__ref_string_chpl)(cast_tmp_chpl2));
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    ((RootClass_chpl)(_ic__chpl2))->chpl__cid = chpl__cid__ic_chpl_bytes__ref_string_chpl;
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    chpl_check_nil(_ic__chpl2, _ln_chpl, _fn_chpl);
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    (_ic__chpl2)->more = INT64(1);
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    chpl_check_nil(_ic__chpl2, _ln_chpl, _fn_chpl);
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    (_ic__chpl2)->F0_this = i_this_chpl;
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    chpl_check_nil(_ic__chpl, _ln_chpl, _fn_chpl);
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    tmp_chpl3 = (_ic__chpl)->F0_this;
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    checkIfIterWillOverflow_chpl(tmp_chpl3, UINT8(true), _ln_chpl, _fn_chpl);
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    chpl_check_nil(_ic__chpl, _ln_chpl, _fn_chpl);
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    (_ic__chpl)->F1_i = INT64(0);
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    chpl_check_nil(_ic__chpl, _ln_chpl, _fn_chpl);
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    tmp_chpl4 = (_ic__chpl)->F0_this;
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    coerce_tmp_chpl3 = (&tmp_chpl4)->_low;
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    chpl_check_nil(_ic__chpl, _ln_chpl, _fn_chpl);
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    (_ic__chpl)->F3_call_tmp = coerce_tmp_chpl3;
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    chpl_check_nil(_ic__chpl, _ln_chpl, _fn_chpl);
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    tmp_chpl5 = (_ic__chpl)->F0_this;
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    coerce_tmp_chpl4 = (&tmp_chpl5)->_high;
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    chpl_check_nil(_ic__chpl, _ln_chpl, _fn_chpl);
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    (_ic__chpl)->F2_end = coerce_tmp_chpl4;
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    chpl_check_nil(_ic__chpl2, _ln_chpl, _fn_chpl);
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    tmp_chpl7 = (_ic__chpl2)->F0_this;
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    init_chpl194(&ret_chpl10, _ln_chpl, _fn_chpl);
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    (&ret_chpl10)->cachedNumCodepoints = (tmp_chpl7)->cachedNumCodepoints;
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    initWithBorrowedBuffer_chpl(&ret_chpl10, tmp_chpl7, _ln_chpl, _fn_chpl);
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    ret_chpl9 = ret_chpl10;
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    chpl_check_nil(_ic__chpl2, _ln_chpl, _fn_chpl);
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    (_ic__chpl2)->F1_localThis = ret_chpl9;
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    chpl_check_nil(_ic__chpl2, _ln_chpl, _fn_chpl);
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    tmp_chpl8 = &((_ic__chpl2)->F1_localThis);
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    coerce_tmp_chpl5 = (tmp_chpl8)->buffLen;
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    cast_tmp_chpl3 = chpl_here_alloc(sizeof(chpl__ic_chpl_direct_counted_range_iter_chpl_object), INT16(32), _ln_chpl, _fn_chpl);
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    _ic__chpl3 = ((_ic_chpl_direct_counted_range_iter_chpl)(cast_tmp_chpl3));
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    ((RootClass_chpl)(_ic__chpl3))->chpl__cid = chpl__cid__ic_chpl_direct_counted_range_iter_chpl;
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    chpl_check_nil(_ic__chpl3, _ln_chpl, _fn_chpl);
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    (_ic__chpl3)->more = INT64(1);
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    chpl_check_nil(_ic__chpl3, _ln_chpl, _fn_chpl);
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    (_ic__chpl3)->F0_low = INT64(0);
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    chpl_check_nil(_ic__chpl3, _ln_chpl, _fn_chpl);
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    (_ic__chpl3)->F1_count = coerce_tmp_chpl5;
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    chpl_check_nil(_ic__chpl2, _ln_chpl, _fn_chpl);
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    (_ic__chpl2)->F2__iterator = _ic__chpl3;
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    chpl_check_nil(_ic__chpl2, _ln_chpl, _fn_chpl);
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    tmp_chpl9 = &((_ic__chpl2)->F2__iterator);
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    chpl_check_nil(tmp_chpl9, _ln_chpl, _fn_chpl);
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    low_chpl2 = (*(tmp_chpl9))->F0_low;
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    chpl_check_nil(_ic__chpl2, _ln_chpl, _fn_chpl);
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    tmp_chpl10 = &((_ic__chpl2)->F2__iterator);
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    chpl_check_nil(tmp_chpl10, _ln_chpl, _fn_chpl);
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    count_chpl = (*(tmp_chpl10))->F1_count;
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    cast_tmp_chpl4 = chpl_here_alloc(sizeof(chpl__ic_chpl_direct_counted_range_iter_helper_chpl_object), INT16(31), _ln_chpl, _fn_chpl);
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    _ic__chpl4 = ((_ic_chpl_direct_counted_range_iter_helper_chpl)(cast_tmp_chpl4));
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    ((RootClass_chpl)(_ic__chpl4))->chpl__cid = chpl__cid__ic_chpl_direct_counted_range_iter_helper_chpl;
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    chpl_check_nil(_ic__chpl4, _ln_chpl, _fn_chpl);
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    (_ic__chpl4)->more = INT64(1);
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    chpl_check_nil(_ic__chpl4, _ln_chpl, _fn_chpl);
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    (_ic__chpl4)->F0_low = low_chpl2;
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    chpl_check_nil(_ic__chpl4, _ln_chpl, _fn_chpl);
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    (_ic__chpl4)->F1_count = count_chpl;
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    chpl_check_nil(_ic__chpl2, _ln_chpl, _fn_chpl);
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    (_ic__chpl2)->F3__iterator = _ic__chpl4;
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    chpl_check_nil(_ic__chpl2, _ln_chpl, _fn_chpl);
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    tmp_chpl11 = &((_ic__chpl2)->F3__iterator);
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    chpl_check_nil(tmp_chpl11, _ln_chpl, _fn_chpl);
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    if ((*(tmp_chpl11))->F1_count < INT64(0)) /* ZLINE: 1252 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    {
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      boundsCheckHalt_chpl(&_str_literal_580_chpl, _ln_chpl, _fn_chpl);
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    }
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    chpl_check_nil(_ic__chpl2, _ln_chpl, _fn_chpl);
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    tmp_chpl12 = &((_ic__chpl2)->F3__iterator);
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    chpl_check_nil(tmp_chpl12, _ln_chpl, _fn_chpl);
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    low_chpl3 = (*(tmp_chpl12))->F0_low;
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    chpl_check_nil(_ic__chpl2, _ln_chpl, _fn_chpl);
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    tmp_chpl13 = &((_ic__chpl2)->F3__iterator);
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    chpl_check_nil(tmp_chpl13, _ln_chpl, _fn_chpl);
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    if ((*(tmp_chpl13))->F1_count == INT64(0)) /* ZLINE: 1252 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    {
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      chpl_check_nil(_ic__chpl2, _ln_chpl, _fn_chpl);
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      tmp_chpl18 = &((_ic__chpl2)->F3__iterator);
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      chpl_check_nil(tmp_chpl18, _ln_chpl, _fn_chpl);
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      tmp_chpl6 = ((int64_t)(((uint64_t)((((uint64_t)((*(tmp_chpl18))->F0_low)) - UINT64(1))))));
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    } else /* ZLINE: 1252 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    {
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      chpl_check_nil(_ic__chpl2, _ln_chpl, _fn_chpl);
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      tmp_chpl19 = &((_ic__chpl2)->F3__iterator);
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      chpl_check_nil(tmp_chpl19, _ln_chpl, _fn_chpl);
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      call_tmp_chpl8 = ((*(tmp_chpl19))->F1_count - INT64(1));
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      chpl_check_nil(_ic__chpl2, _ln_chpl, _fn_chpl);
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      tmp_chpl20 = &((_ic__chpl2)->F3__iterator);
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      chpl_check_nil(tmp_chpl20, _ln_chpl, _fn_chpl);
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      tmp_chpl6 = ((int64_t)(((*(tmp_chpl20))->F0_low + call_tmp_chpl8)));
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    }
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    cast_tmp_chpl5 = chpl_here_alloc(sizeof(chpl__ic_chpl_direct_param_stride_range_iter_chpl_object), INT16(4), _ln_chpl, _fn_chpl);
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    _ic__chpl5 = ((_ic_chpl_direct_param_stride_range_iter_chpl)(cast_tmp_chpl5));
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    ((RootClass_chpl)(_ic__chpl5))->chpl__cid = chpl__cid__ic_chpl_direct_param_stride_range_iter_chpl;
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    chpl_check_nil(_ic__chpl5, _ln_chpl, _fn_chpl);
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    (_ic__chpl5)->more = INT64(1);
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    chpl_check_nil(_ic__chpl5, _ln_chpl, _fn_chpl);
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    (_ic__chpl5)->F0_low = low_chpl3;
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    chpl_check_nil(_ic__chpl5, _ln_chpl, _fn_chpl);
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    (_ic__chpl5)->F1_high = tmp_chpl6;
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    chpl_check_nil(_ic__chpl2, _ln_chpl, _fn_chpl);
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    (_ic__chpl2)->F4__iterator = _ic__chpl5;
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    chpl_check_nil(_ic__chpl2, _ln_chpl, _fn_chpl);
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    (_ic__chpl2)->F5_i = INT64(0);
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    chpl_check_nil(_ic__chpl2, _ln_chpl, _fn_chpl);
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    tmp_chpl14 = &((_ic__chpl2)->F4__iterator);
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    chpl_check_nil(tmp_chpl14, _ln_chpl, _fn_chpl);
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    low_chpl4 = (*(tmp_chpl14))->F0_low;
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    chpl_check_nil(_ic__chpl2, _ln_chpl, _fn_chpl);
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    tmp_chpl15 = &((_ic__chpl2)->F4__iterator);
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    chpl_check_nil(tmp_chpl15, _ln_chpl, _fn_chpl);
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    high_chpl = (*(tmp_chpl15))->F1_high;
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    chpl_check_nil(_ic__chpl2, _ln_chpl, _fn_chpl);
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    tmp_chpl16 = &((_ic__chpl2)->F4__iterator);
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    chpl_check_nil(tmp_chpl16, _ln_chpl, _fn_chpl);
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    low_chpl5 = (*(tmp_chpl16))->F0_low;
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    chpl_check_nil(_ic__chpl2, _ln_chpl, _fn_chpl);
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    tmp_chpl17 = &((_ic__chpl2)->F4__iterator);
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    chpl_check_nil(tmp_chpl17, _ln_chpl, _fn_chpl);
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    chpl_checkIfRangeIterWillOverflow(low_chpl5, (*(tmp_chpl17))->F1_high, INT64(1), low_chpl4, high_chpl, UINT8(true), _ln_chpl, _fn_chpl);
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    chpl_check_nil(_ic__chpl, _ln_chpl, _fn_chpl);
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    tmp_chpl21 = (_ic__chpl)->F2_end;
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    for (chpl_check_nil(_ic__chpl, _ln_chpl, _fn_chpl),tmp_chpl30 = (_ic__chpl)->F3_call_tmp,chpl_check_nil(_ic__chpl, _ln_chpl, _fn_chpl),(_ic__chpl)->F1_i = tmp_chpl30,chpl_check_nil(_ic__chpl2, _ln_chpl, _fn_chpl),tmp_chpl32 = &((_ic__chpl2)->F4__iterator),chpl_check_nil(tmp_chpl32, _ln_chpl, _fn_chpl),tmp_chpl31 = (*(tmp_chpl32))->F0_low,chpl_check_nil(_ic__chpl2, _ln_chpl, _fn_chpl),(_ic__chpl2)->F6_low = tmp_chpl31,chpl_check_nil(_ic__chpl2, _ln_chpl, _fn_chpl),tmp_chpl33 = (_ic__chpl2)->F6_low,chpl_check_nil(_ic__chpl2, _ln_chpl, _fn_chpl),(_ic__chpl2)->F5_i = tmp_chpl33; (chpl_check_nil(_ic__chpl, _ln_chpl, _fn_chpl),tmp_chpl34 = ((_ic__chpl)->F1_i <= tmp_chpl21),tmp_chpl34); chpl_check_nil(_ic__chpl, _ln_chpl, _fn_chpl),tmp_chpl35 = (_ic__chpl)->F1_i,tmp_chpl35 += INT64(1),chpl_check_nil(_ic__chpl, _ln_chpl, _fn_chpl),(_ic__chpl)->F1_i = tmp_chpl35,chpl_check_nil(_ic__chpl2, _ln_chpl, _fn_chpl),tmp_chpl36 = (_ic__chpl2)->F5_i,tmp_chpl36 += INT64(1),chpl_check_nil(_ic__chpl2, _ln_chpl, _fn_chpl),(_ic__chpl2)->F5_i = tmp_chpl36) {
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      chpl_check_nil(_ic__chpl2, _ln_chpl, _fn_chpl);
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      tmp_chpl22 = (_ic__chpl2)->F5_i;
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      chpl_check_nil(_ic__chpl2, _ln_chpl, _fn_chpl);
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      tmp_chpl23 = &((_ic__chpl2)->F1_localThis);
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      coerce_tmp_chpl6 = (tmp_chpl23)->buff;
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      call_tmp_chpl9 = (coerce_tmp_chpl6 + tmp_chpl22);
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      tmp_chpl24 = *(call_tmp_chpl9);
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      chpl_check_nil(_ic__chpl2, _ln_chpl, _fn_chpl);
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      (_ic__chpl2)->value = tmp_chpl24;
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      chpl_check_nil(_ic__chpl2, _ln_chpl, _fn_chpl);
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      (_ic__chpl2)->more = INT64(2);
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      chpl_check_nil(_ic__chpl2, _ln_chpl, _fn_chpl);
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      tmp_chpl26 = &((_ic__chpl2)->F4__iterator);
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      chpl_check_nil(tmp_chpl26, _ln_chpl, _fn_chpl);
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      high_chpl2 = (*(tmp_chpl26))->F1_high;
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      chpl_check_nil(_ic__chpl2, _ln_chpl, _fn_chpl);
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      tmp_chpl25 = ((_ic__chpl2)->F5_i <= high_chpl2);
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      hasMore_chpl = (tmp_chpl25 != INT64(0));
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      if (! hasMore_chpl) /* ZLINE: 1252 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      {
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        chpl_error("zippered iterations have non-equal lengths", _ln_chpl, _fn_chpl);
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      }
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      chpl_check_nil(_ic__chpl2, _ln_chpl, _fn_chpl);
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      tmp_chpl27 = (_ic__chpl2)->value;
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      chpl_check_nil(_ic__chpl, _ln_chpl, _fn_chpl);
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      tmp_chpl28 = (_ic__chpl)->F1_i;
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      chpl_check_nil(_ic__chpl, _ln_chpl, _fn_chpl);
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      (_ic__chpl)->value = tmp_chpl28;
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      chpl_check_nil(_ic__chpl, _ln_chpl, _fn_chpl);
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      (_ic__chpl)->more = INT64(2);
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      chpl_check_nil(_ic__chpl, _ln_chpl, _fn_chpl);
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      i_chpl = (_ic__chpl)->value;
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      xChar_chpl = tmp_chpl27;
#line 1253 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      i_this_chpl2 = &localChars_chpl;
#line 1253 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      init_chpl194(&ret_chpl12, _ln_chpl, _fn_chpl);
#line 1253 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      (&ret_chpl12)->cachedNumCodepoints = (&localChars_chpl)->cachedNumCodepoints;
#line 1253 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      initWithBorrowedBuffer_chpl(&ret_chpl12, i_this_chpl2, _ln_chpl, _fn_chpl);
#line 1253 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      ret_chpl11 = ret_chpl12;
#line 1253 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      localThis_chpl = ret_chpl11;
#line 1253 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      coerce_tmp_chpl7 = (&localThis_chpl)->buffLen;
#line 1253 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      if (coerce_tmp_chpl7 < INT64(0)) /* ZLINE: 1253 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 1253 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      {
#line 1253 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        boundsCheckHalt_chpl(&_str_literal_580_chpl, _ln_chpl, _fn_chpl);
#line 1253 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      }
#line 1253 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      if (coerce_tmp_chpl7 == INT64(0)) /* ZLINE: 1253 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 1253 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      {
#line 1253 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        tmp_chpl29 = ((int64_t)(((uint64_t)((((uint64_t)(INT64(0))) - UINT64(1))))));
#line 1253 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      } else /* ZLINE: 1253 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 1253 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      {
#line 1253 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        tmp_chpl29 = ((int64_t)((INT64(0) + ((int64_t)((coerce_tmp_chpl7 - INT64(1)))))));
#line 1253 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      }
#line 1253 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      _ic__F1_high_chpl = tmp_chpl29;
#line 1253 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      i_chpl2 = INT64(0);
#line 1253 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      chpl_checkIfRangeIterWillOverflow(INT64(0), tmp_chpl29, INT64(1), INT64(0), tmp_chpl29, UINT8(true), _ln_chpl, _fn_chpl);
#line 1253 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      for (i_chpl2 = INT64(0); ((i_chpl2 <= _ic__F1_high_chpl)); i_chpl2 += INT64(1)) {
#line 1253 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        coerce_tmp_chpl8 = (&localThis_chpl)->buff;
#line 1253 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        call_tmp_chpl10 = (coerce_tmp_chpl8 + i_chpl2);
#line 1253 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        yret_chpl = *(call_tmp_chpl10);
#line 1254 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        if (xChar_chpl == yret_chpl) /* ZLINE: 1254 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 1254 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        {
#line 1255 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          start_chpl2 = ((int64_t)((i_chpl + INT64(1))));
#line 1253 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          i_x_chpl2 = &localThis_chpl;
#line 1253 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          deinit_chpl95(i_x_chpl2, _ln_chpl, _fn_chpl);
#line 1256 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          goto _continueLabel_chpl;
#line 1256 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        }
#line 1256 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      }
#line 1253 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      i_x_chpl3 = &localThis_chpl;
#line 1253 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      deinit_chpl95(i_x_chpl3, _ln_chpl, _fn_chpl);
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      chpl_check_nil(_ic__chpl, _ln_chpl, _fn_chpl);
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      moreVal1_chpl = (_ic__chpl)->more;
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      moreValNeg1_chpl = (-moreVal1_chpl);
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      chpl_check_nil(_ic__chpl, _ln_chpl, _fn_chpl);
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      (_ic__chpl)->more = moreValNeg1_chpl;
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      chpl_check_nil(_ic__chpl, _ln_chpl, _fn_chpl);
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      advance_chpl18(_ic__chpl, _ln_chpl, _fn_chpl);
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      chpl_check_nil(_ic__chpl2, _ln_chpl, _fn_chpl);
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      moreVal2_chpl = (_ic__chpl2)->more;
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      moreValNeg2_chpl = (-moreVal2_chpl);
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      chpl_check_nil(_ic__chpl2, _ln_chpl, _fn_chpl);
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      (_ic__chpl2)->more = moreValNeg2_chpl;
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      chpl_check_nil(_ic__chpl2, _ln_chpl, _fn_chpl);
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      advance_chpl19(_ic__chpl2, _ln_chpl, _fn_chpl);
#line 1259 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      goto _breakLabel_chpl;
#line 1259 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      _continueLabel_chpl:;
#line 1259 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    }
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    chpl_check_nil(_ic__chpl2, _ln_chpl, _fn_chpl);
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    tmp_chpl38 = &((_ic__chpl2)->F4__iterator);
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    chpl_check_nil(tmp_chpl38, _ln_chpl, _fn_chpl);
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    high_chpl3 = (*(tmp_chpl38))->F1_high;
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    chpl_check_nil(_ic__chpl2, _ln_chpl, _fn_chpl);
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    tmp_chpl37 = ((_ic__chpl2)->F5_i <= high_chpl3);
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    hasMore_chpl = (tmp_chpl37 != INT64(0));
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    if (hasMore_chpl) /* ZLINE: 1252 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    {
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      chpl_error("zippered iterations have non-equal lengths", _ln_chpl, _fn_chpl);
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    }
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    chpl_check_nil(_ic__chpl2, _ln_chpl, _fn_chpl);
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    chpl_here_free(((c_ptr_void_chpl)((_ic__chpl2)->F4__iterator)), _ln_chpl, _fn_chpl);
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    chpl_check_nil(_ic__chpl2, _ln_chpl, _fn_chpl);
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    chpl_here_free(((c_ptr_void_chpl)((_ic__chpl2)->F3__iterator)), _ln_chpl, _fn_chpl);
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    chpl_check_nil(_ic__chpl2, _ln_chpl, _fn_chpl);
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    chpl_here_free(((c_ptr_void_chpl)((_ic__chpl2)->F2__iterator)), _ln_chpl, _fn_chpl);
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    chpl_check_nil(_ic__chpl2, _ln_chpl, _fn_chpl);
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    tmp_chpl39 = &((_ic__chpl2)->F1_localThis);
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    deinit_chpl95(tmp_chpl39, _ln_chpl, _fn_chpl);
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    _breakLabel_chpl:;
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    chpl_here_free(((c_ptr_void_chpl)(_ic__chpl)), _ln_chpl, _fn_chpl);
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    chpl_here_free(((c_ptr_void_chpl)(_ic__chpl2)), _ln_chpl, _fn_chpl);
#line 1252 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  }
#line 1263 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  if (trailing_chpl) /* ZLINE: 1263 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 1263 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  {
#line 1268 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    end_chpl = INT64(-1);
#line 1269 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    ret_chpl13 = (x_chpl)->cachedNumCodepoints;
#line 1269 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl11 = chpl__nudgeHighBound(ret_chpl13);
#line 1269 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    chpl_build_bounded_range6(INT64(0), call_tmp_chpl11, &ret_tmp_chpl2);
#line 1269 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl12 = ret_tmp_chpl2;
#line 1269 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    _ic__F0_this_chpl = call_tmp_chpl12;
#line 1269 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    i_this_chpl3 = &localX_chpl;
#line 1269 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    checkIfIterWillOverflow_chpl(call_tmp_chpl12, UINT8(true), _ln_chpl, _fn_chpl);
#line 1269 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    _ic__F1_i_chpl = INT64(0);
#line 1269 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    tmp_chpl40 = _ic__F0_this_chpl;
#line 1269 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    coerce_tmp_chpl9 = (&tmp_chpl40)->_low;
#line 1269 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    tmp_chpl41 = _ic__F0_this_chpl;
#line 1269 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    _ic__F2_end_chpl = (&tmp_chpl41)->_high;
#line 1269 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    init_chpl194(&ret_chpl15, _ln_chpl, _fn_chpl);
#line 1269 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    (&ret_chpl15)->cachedNumCodepoints = (&localX_chpl)->cachedNumCodepoints;
#line 1269 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    initWithBorrowedBuffer_chpl(&ret_chpl15, i_this_chpl3, _ln_chpl, _fn_chpl);
#line 1269 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    ret_chpl14 = ret_chpl15;
#line 1269 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    _ic__F1_localThis_chpl = ret_chpl14;
#line 1269 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    coerce_tmp_chpl10 = (&_ic__F1_localThis_chpl)->buffLen;
#line 1269 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    if (coerce_tmp_chpl10 < INT64(0)) /* ZLINE: 1269 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 1269 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    {
#line 1269 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      boundsCheckHalt_chpl(&_str_literal_580_chpl, _ln_chpl, _fn_chpl);
#line 1269 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    }
#line 1269 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    if (coerce_tmp_chpl10 == INT64(0)) /* ZLINE: 1269 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 1269 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    {
#line 1269 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      tmp_chpl42 = ((int64_t)(((uint64_t)((((uint64_t)(INT64(0))) - UINT64(1))))));
#line 1269 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    } else /* ZLINE: 1269 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 1269 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    {
#line 1269 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      tmp_chpl42 = ((int64_t)((INT64(0) + ((int64_t)((coerce_tmp_chpl10 - INT64(1)))))));
#line 1269 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    }
#line 1269 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    _ic__F1_high_chpl2 = tmp_chpl42;
#line 1269 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    _ic__F5_i_chpl = INT64(0);
#line 1269 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    chpl_checkIfRangeIterWillOverflow(INT64(0), tmp_chpl42, INT64(1), INT64(0), tmp_chpl42, UINT8(true), _ln_chpl, _fn_chpl);
#line 1269 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    for (_ic__F1_i_chpl = coerce_tmp_chpl9,_ic__F5_i_chpl = INT64(0); (tmp_chpl46 = (_ic__F1_i_chpl <= _ic__F2_end_chpl),tmp_chpl46); tmp_chpl47 = _ic__F1_i_chpl,tmp_chpl47 += INT64(1),_ic__F1_i_chpl = tmp_chpl47,tmp_chpl48 = _ic__F5_i_chpl,tmp_chpl48 += INT64(1),_ic__F5_i_chpl = tmp_chpl48) {
#line 1269 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      coerce_tmp_chpl11 = (&_ic__F1_localThis_chpl)->buff;
#line 1269 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      call_tmp_chpl13 = (coerce_tmp_chpl11 + _ic__F5_i_chpl);
#line 1269 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      tmp_chpl43 = *(call_tmp_chpl13);
#line 1269 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      tmp_chpl44 = (_ic__F5_i_chpl <= _ic__F1_high_chpl2);
#line 1269 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      hasMore_chpl2 = (tmp_chpl44 != INT64(0));
#line 1269 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      if (! hasMore_chpl2) /* ZLINE: 1269 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 1269 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      {
#line 1269 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        chpl_error("zippered iterations have non-equal lengths", _ln_chpl, _fn_chpl);
#line 1269 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      }
#line 1269 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      i_chpl3 = _ic__F1_i_chpl;
#line 1269 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      xChar_chpl2 = tmp_chpl43;
#line 1270 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      i_this_chpl4 = &localChars_chpl;
#line 1270 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      init_chpl194(&ret_chpl17, _ln_chpl, _fn_chpl);
#line 1270 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      (&ret_chpl17)->cachedNumCodepoints = (&localChars_chpl)->cachedNumCodepoints;
#line 1270 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      initWithBorrowedBuffer_chpl(&ret_chpl17, i_this_chpl4, _ln_chpl, _fn_chpl);
#line 1270 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      ret_chpl16 = ret_chpl17;
#line 1270 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      localThis_chpl2 = ret_chpl16;
#line 1270 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      coerce_tmp_chpl12 = (&localThis_chpl2)->buffLen;
#line 1270 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      if (coerce_tmp_chpl12 < INT64(0)) /* ZLINE: 1270 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 1270 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      {
#line 1270 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        boundsCheckHalt_chpl(&_str_literal_580_chpl, _ln_chpl, _fn_chpl);
#line 1270 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      }
#line 1270 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      if (coerce_tmp_chpl12 == INT64(0)) /* ZLINE: 1270 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 1270 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      {
#line 1270 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        tmp_chpl45 = ((int64_t)(((uint64_t)((((uint64_t)(INT64(0))) - UINT64(1))))));
#line 1270 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      } else /* ZLINE: 1270 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 1270 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      {
#line 1270 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        tmp_chpl45 = ((int64_t)((INT64(0) + ((int64_t)((coerce_tmp_chpl12 - INT64(1)))))));
#line 1270 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      }
#line 1270 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      _ic__F1_high_chpl3 = tmp_chpl45;
#line 1270 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      i_chpl4 = INT64(0);
#line 1270 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      chpl_checkIfRangeIterWillOverflow(INT64(0), tmp_chpl45, INT64(1), INT64(0), tmp_chpl45, UINT8(true), _ln_chpl, _fn_chpl);
#line 1270 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      for (i_chpl4 = INT64(0); ((i_chpl4 <= _ic__F1_high_chpl3)); i_chpl4 += INT64(1)) {
#line 1270 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        coerce_tmp_chpl13 = (&localThis_chpl2)->buff;
#line 1270 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        call_tmp_chpl14 = (coerce_tmp_chpl13 + i_chpl4);
#line 1270 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        yret_chpl2 = *(call_tmp_chpl14);
#line 1271 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        if (xChar_chpl2 == yret_chpl2) /* ZLINE: 1271 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 1271 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        {
#line 1270 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          i_x_chpl4 = &localThis_chpl2;
#line 1270 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          deinit_chpl95(i_x_chpl4, _ln_chpl, _fn_chpl);
#line 1272 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          goto _continueLabel_chpl2;
#line 1272 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        }
#line 1272 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      }
#line 1270 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      i_x_chpl5 = &localThis_chpl2;
#line 1270 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      deinit_chpl95(i_x_chpl5, _ln_chpl, _fn_chpl);
#line 1276 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      end_chpl = i_chpl3;
#line 1276 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      _continueLabel_chpl2:;
#line 1276 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    }
#line 1269 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    tmp_chpl49 = (_ic__F5_i_chpl <= _ic__F1_high_chpl2);
#line 1269 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    hasMore_chpl2 = (tmp_chpl49 != INT64(0));
#line 1269 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    if (hasMore_chpl2) /* ZLINE: 1269 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 1269 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    {
#line 1269 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      chpl_error("zippered iterations have non-equal lengths", _ln_chpl, _fn_chpl);
#line 1269 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    }
#line 1269 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    tmp_chpl50 = &_ic__F1_localThis_chpl;
#line 1269 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    deinit_chpl95(tmp_chpl50, _ln_chpl, _fn_chpl);
#line 1269 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  }
#line 1280 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl_build_bounded_range6(start_chpl2, end_chpl, &ret_tmp_chpl3);
#line 1280 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl15 = ret_tmp_chpl3;
#line 1280 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  i_this_chpl5 = &localX_chpl;
#line 1280 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  error_chpl = NULL;
#line 1280 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  getSlice_chpl2(i_this_chpl5, call_tmp_chpl15, &error_chpl, &ret_tmp_chpl4, _ln_chpl, _fn_chpl);
#line 1280 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl16 = ret_tmp_chpl4;
#line 1280 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  if (error_chpl != nil) /* ZLINE: 1280 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 1280 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  {
#line 1280 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    chpl_uncaught_error(error_chpl, _ln_chpl, _fn_chpl);
#line 1280 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  }
#line 1280 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  ret_chpl = call_tmp_chpl16;
#line 1280 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  i_x_chpl6 = &localChars_chpl;
#line 1280 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  deinit_chpl95(i_x_chpl6, _ln_chpl, _fn_chpl);
#line 1280 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  i_x_chpl7 = &localX_chpl;
#line 1280 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  deinit_chpl95(i_x_chpl7, _ln_chpl, _fn_chpl);
#line 1280 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _end_doStripNoEnc_chpl:;
#line 1240 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  *(_retArg_chpl) = ret_chpl;
#line 1240 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  return;
#line 1240 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
}

#line 1365 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
/* BytesStringCommon.chpl:1365 */
#line 1365 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
static int64_t countNumCodepoints_chpl(uint8_t * buff_chpl,
#line 1365 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                                       int64_t buffLen_chpl,
#line 1365 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                                       int64_t _ln_chpl,
#line 1365 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                                       int32_t _fn_chpl) {
#line 1365 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t ret_chpl;
#line 1366 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t n_chpl;
#line 1367 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t i_chpl;
#line 1368 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl_bool tmp_chpl;
#line 1369 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref_int64_t_chpl i_lhs_chpl = NULL;
#line 1370 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl_bool tmp_chpl2;
#line 1370 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl_bool tmp_chpl3;
#line 1370 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref_uint8_t_chpl call_tmp_chpl6 = NULL;
#line 1371 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref_int64_t_chpl i_lhs_chpl2 = NULL;
#line 1370 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl_bool tmp_chpl4;
#line 1370 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref_uint8_t_chpl call_tmp_chpl7 = NULL;
#line 1372 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref_int64_t_chpl i_lhs_chpl3 = NULL;
#line 1366 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  n_chpl = INT64(0);
#line 1367 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  i_chpl = INT64(0);
#line 1368 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  tmp_chpl = (INT64(0) < buffLen_chpl);
#line 1368 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  while (tmp_chpl) {
#line 1369 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    i_lhs_chpl = &i_chpl;
#line 1369 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    *(i_lhs_chpl) += INT64(1);
#line 1370 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    if (i_chpl < buffLen_chpl) /* ZLINE: 1370 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 1370 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    {
#line 1370 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      call_tmp_chpl6 = (buff_chpl + i_chpl);
#line 1370 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      tmp_chpl3 = (! ((*(call_tmp_chpl6) & ((uint8_t)(INT64(192)))) != ((uint8_t)(INT64(128)))));
#line 1370 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    } else /* ZLINE: 1370 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 1370 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    {
#line 1370 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      tmp_chpl3 = UINT8(false);
#line 1370 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    }
#line 1370 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    tmp_chpl2 = tmp_chpl3;
#line 1370 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    while (tmp_chpl2) {
#line 1371 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      i_lhs_chpl2 = &i_chpl;
#line 1371 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      *(i_lhs_chpl2) += INT64(1);
#line 1370 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      if (i_chpl < buffLen_chpl) /* ZLINE: 1370 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 1370 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      {
#line 1370 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        call_tmp_chpl7 = (buff_chpl + i_chpl);
#line 1370 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        tmp_chpl4 = (! ((*(call_tmp_chpl7) & ((uint8_t)(INT64(192)))) != ((uint8_t)(INT64(128)))));
#line 1370 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      } else /* ZLINE: 1370 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 1370 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      {
#line 1370 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        tmp_chpl4 = UINT8(false);
#line 1370 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      }
#line 1370 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      tmp_chpl2 = tmp_chpl4;
#line 1370 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    }
#line 1372 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    i_lhs_chpl3 = &n_chpl;
#line 1372 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    *(i_lhs_chpl3) += INT64(1);
#line 1368 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    tmp_chpl = (i_chpl < buffLen_chpl);
#line 1368 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  }
#line 1374 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  ret_chpl = n_chpl;
#line 1365 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  return ret_chpl;
#line 1365 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
}

#line 1390 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
/* BytesStringCommon.chpl:1390 */
#line 1390 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
static int64_t _findStartOfNextCodepointFromByte_chpl(string_chpl * x_chpl,
#line 1390 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                                                      byteIndex_chpl * i_chpl,
#line 1390 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                                                      int64_t _ln_chpl,
#line 1390 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                                                      int32_t _fn_chpl) {
#line 1390 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t ret_chpl;
#line 1391 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t ret_chpl2;
#line 1391 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t ret_chpl3;
#line 1393 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl_bool tmp_chpl;
#line 1393 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl_bool tmp_chpl2;
#line 1393 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  c_ptr_uint8_t_chpl coerce_tmp_chpl3 = NULL;
#line 1393 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref_uint8_t_chpl call_tmp_chpl6 = NULL;
#line 1394 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref_int64_t_chpl i_lhs_chpl = NULL;
#line 1393 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl_bool tmp_chpl3;
#line 1393 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  c_ptr_uint8_t_chpl coerce_tmp_chpl4 = NULL;
#line 1393 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref_uint8_t_chpl call_tmp_chpl7 = NULL;
#line 1391 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  ret_chpl2 = (i_chpl)->_bindex;
#line 1391 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  ret_chpl3 = ret_chpl2;
#line 1392 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  if (ret_chpl2 > INT64(0)) /* ZLINE: 1392 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 1392 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  {
#line 1393 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    if (ret_chpl2 < (x_chpl)->buffLen) /* ZLINE: 1393 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 1393 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    {
#line 1393 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      coerce_tmp_chpl3 = (x_chpl)->buff;
#line 1393 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      call_tmp_chpl6 = (coerce_tmp_chpl3 + ret_chpl2);
#line 1393 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      tmp_chpl2 = (! ((*(call_tmp_chpl6) & ((uint8_t)(INT64(192)))) != ((uint8_t)(INT64(128)))));
#line 1393 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    } else /* ZLINE: 1393 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 1393 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    {
#line 1393 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      tmp_chpl2 = UINT8(false);
#line 1393 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    }
#line 1393 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    tmp_chpl = tmp_chpl2;
#line 1393 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    while (tmp_chpl) {
#line 1394 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      i_lhs_chpl = &ret_chpl3;
#line 1394 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      *(i_lhs_chpl) += INT64(1);
#line 1393 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      if (ret_chpl3 < (x_chpl)->buffLen) /* ZLINE: 1393 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 1393 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      {
#line 1393 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        coerce_tmp_chpl4 = (x_chpl)->buff;
#line 1393 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        call_tmp_chpl7 = (coerce_tmp_chpl4 + ret_chpl3);
#line 1393 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        tmp_chpl3 = (! ((*(call_tmp_chpl7) & ((uint8_t)(INT64(192)))) != ((uint8_t)(INT64(128)))));
#line 1393 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      } else /* ZLINE: 1393 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 1393 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      {
#line 1393 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        tmp_chpl3 = UINT8(false);
#line 1393 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      }
#line 1393 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      tmp_chpl = tmp_chpl3;
#line 1393 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    }
#line 1393 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  }
#line 1397 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  ret_chpl = ret_chpl3;
#line 1390 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  return ret_chpl;
#line 1390 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
}

#line 1401 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
/* BytesStringCommon.chpl:1401 */
#line 1401 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
static void _cleanupForNumericCast_chpl(string_chpl * x_chpl,
#line 1401 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                                        int64_t _ln_chpl,
#line 1401 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                                        int32_t _fn_chpl) {
#line 1406 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl_bool hasUnderscores_chpl;
#line 1407 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t ret_chpl;
#line 1407 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t call_tmp_chpl6;
#line 1407 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t _ic__F1_high_chpl;
#line 1407 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t i_chpl;
#line 1408 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  uint8_t call_tmp_chpl7;
#line 1415 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl call_tmp_chpl8;
#line 1415 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl ret_tmp_chpl;
#line 1415 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 1417 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl_bool call_tmp_chpl9;
#line 1418 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t ret_chpl2;
#line 1422 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl_bool tmp_chpl;
#line 1422 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  uint8_t call_tmp_chpl10;
#line 1423 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl call_tmp_chpl11;
#line 1423 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  codepointIndex_chpl call_tmp_chpl12;
#line 1423 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  codepointIndex_chpl ret_chpl3;
#line 1423 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  codepointIndex_chpl cpi_chpl;
#line 1423 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl call_tmp_chpl13;
#line 1423 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl ret_tmp_chpl2;
#line 1423 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_low_one_chpl call_tmp_chpl14;
#line 1423 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  range_int64_t_low_one_chpl ret_tmp_chpl3;
#line 1423 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl call_tmp_chpl15;
#line 1423 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl call_tmp_chpl16;
#line 1423 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  Error_chpl error_chpl = NULL;
#line 1423 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl ret_tmp_chpl4;
#line 1423 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl call_tmp_chpl17;
#line 1423 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref_string_chpl i_this_chpl = NULL;
#line 1423 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl inlineImm_chpl;
#line 1423 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref_string_chpl i_pattern_chpl = NULL;
#line 1423 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl inlineImm_chpl2;
#line 1423 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref_string_chpl i_replacement_chpl = NULL;
#line 1423 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl call_tmp_chpl18;
#line 1423 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl ret_tmp_chpl5;
#line 1423 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl call_tmp_chpl19;
#line 1423 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl ret_tmp_chpl6;
#line 1423 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref_string_chpl i_x_chpl2 = NULL;
#line 1423 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref_string_chpl i_x_chpl3 = NULL;
#line 1423 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref_string_chpl i_x_chpl4 = NULL;
#line 1423 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref_string_chpl i_x_chpl5 = NULL;
#line 1425 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl call_tmp_chpl20;
#line 1425 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl inlineImm_chpl3;
#line 1425 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref_string_chpl i_pattern_chpl2 = NULL;
#line 1425 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl inlineImm_chpl4;
#line 1425 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref_string_chpl i_replacement_chpl2 = NULL;
#line 1425 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl call_tmp_chpl21;
#line 1425 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl ret_tmp_chpl7;
#line 1425 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref_string_chpl i_x_chpl6 = NULL;
#line 1432 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  c_ptr_uint8_t_chpl coerce_tmp_chpl3 = NULL;
#line 1432 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t coerce_tmp_chpl4;
#line 1432 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref_uint8_t_chpl call_tmp_chpl22 = NULL;
#line 1406 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  hasUnderscores_chpl = UINT8(false);
#line 1407 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  ret_chpl = (x_chpl)->buffLen;
#line 1407 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  call_tmp_chpl6 = chpl__nudgeHighBound(ret_chpl);
#line 1407 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ic__F1_high_chpl = call_tmp_chpl6;
#line 1407 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  i_chpl = INT64(0);
#line 1407 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl_checkIfRangeIterWillOverflow(INT64(1), call_tmp_chpl6, INT64(1), INT64(1), call_tmp_chpl6, UINT8(true), _ln_chpl, _fn_chpl);
#line 1407 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  for (i_chpl = INT64(1); ((i_chpl <= _ic__F1_high_chpl)); i_chpl += INT64(1)) {
#line 1408 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl7 = byte_chpl(x_chpl, i_chpl, _ln_chpl, _fn_chpl);
#line 1408 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    if (call_tmp_chpl7 == UINT8(95)) /* ZLINE: 1408 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 1408 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    {
#line 1409 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      hasUnderscores_chpl = UINT8(true);
#line 1410 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      goto _breakLabel_chpl;
#line 1410 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    }
#line 1410 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  }
#line 1410 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _breakLabel_chpl:;
#line 1414 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  if (hasUnderscores_chpl) /* ZLINE: 1414 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 1414 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  {
#line 1415 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    strip_chpl(x_chpl, &_str_literal_268_chpl, UINT8(true), UINT8(true), &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 1415 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl8 = ret_tmp_chpl;
#line 1415 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    chpl___ASSIGN_16(x_chpl, &call_tmp_chpl8, _ln_chpl, _fn_chpl);
#line 1415 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    i_x_chpl = &call_tmp_chpl8;
#line 1415 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    deinit_chpl95(i_x_chpl, _ln_chpl, _fn_chpl);
#line 1417 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl9 = _isSingleWord_chpl(x_chpl, _ln_chpl, _fn_chpl);
#line 1417 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    if (call_tmp_chpl9) /* ZLINE: 1417 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 1417 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    {
#line 1418 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      ret_chpl2 = (x_chpl)->cachedNumCodepoints;
#line 1419 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      if (ret_chpl2 >= INT64(2)) /* ZLINE: 1419 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 1419 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      {
#line 1422 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        if (ret_chpl2 > INT64(2)) /* ZLINE: 1422 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 1422 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        {
#line 1422 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          call_tmp_chpl10 = byte_chpl(x_chpl, INT64(0), _ln_chpl, _fn_chpl);
#line 1422 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          tmp_chpl = (call_tmp_chpl10 == UINT8(95));
#line 1422 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        } else /* ZLINE: 1422 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 1422 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        {
#line 1422 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          tmp_chpl = UINT8(false);
#line 1422 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        }
#line 1422 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        if (tmp_chpl) /* ZLINE: 1422 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 1422 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        {
#line 1423 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          init_chpl193(&cpi_chpl);
#line 1423 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          (&cpi_chpl)->_cpindex = INT64(0);
#line 1423 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          ret_chpl3 = cpi_chpl;
#line 1423 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          call_tmp_chpl12 = ret_chpl3;
#line 1423 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          this_chpl30(x_chpl, &call_tmp_chpl12, &ret_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 1423 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          call_tmp_chpl13 = ret_tmp_chpl2;
#line 1423 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          call_tmp_chpl11 = call_tmp_chpl13;
#line 1423 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          chpl_build_low_bounded_range2(INT64(1), &ret_tmp_chpl3);
#line 1423 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          call_tmp_chpl14 = ret_tmp_chpl3;
#line 1423 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          error_chpl = NULL;
#line 1423 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          getSlice_chpl4(x_chpl, call_tmp_chpl14, &error_chpl, &ret_tmp_chpl4, _ln_chpl, _fn_chpl);
#line 1423 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          call_tmp_chpl16 = ret_tmp_chpl4;
#line 1423 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          if (error_chpl != nil) /* ZLINE: 1423 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 1423 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          {
#line 1423 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
            chpl_uncaught_error(error_chpl, _ln_chpl, _fn_chpl);
#line 1423 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          }
#line 1423 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          call_tmp_chpl15 = call_tmp_chpl16;
#line 1423 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          i_this_chpl = &call_tmp_chpl15;
#line 1423 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          inlineImm_chpl = _str_literal_333_chpl;
#line 1423 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          i_pattern_chpl = &inlineImm_chpl;
#line 1423 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          inlineImm_chpl2 = _str_literal_163_chpl;
#line 1423 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          i_replacement_chpl = &inlineImm_chpl2;
#line 1423 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          doReplace_chpl(i_this_chpl, i_pattern_chpl, i_replacement_chpl, INT64(-1), &ret_tmp_chpl5, _ln_chpl, _fn_chpl);
#line 1423 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          call_tmp_chpl18 = ret_tmp_chpl5;
#line 1423 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          call_tmp_chpl17 = call_tmp_chpl18;
#line 1423 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          chpl___PLUS_(&call_tmp_chpl11, &call_tmp_chpl17, &ret_tmp_chpl6, _ln_chpl, _fn_chpl);
#line 1423 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          call_tmp_chpl19 = ret_tmp_chpl6;
#line 1423 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          chpl___ASSIGN_16(x_chpl, &call_tmp_chpl19, _ln_chpl, _fn_chpl);
#line 1423 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          i_x_chpl2 = &call_tmp_chpl19;
#line 1423 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          deinit_chpl95(i_x_chpl2, _ln_chpl, _fn_chpl);
#line 1423 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          i_x_chpl3 = &call_tmp_chpl17;
#line 1423 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          deinit_chpl95(i_x_chpl3, _ln_chpl, _fn_chpl);
#line 1423 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          i_x_chpl4 = &call_tmp_chpl15;
#line 1423 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          deinit_chpl95(i_x_chpl4, _ln_chpl, _fn_chpl);
#line 1423 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          i_x_chpl5 = &call_tmp_chpl11;
#line 1423 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          deinit_chpl95(i_x_chpl5, _ln_chpl, _fn_chpl);
#line 1423 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        } else /* ZLINE: 1424 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 1423 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        {
#line 1425 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          inlineImm_chpl3 = _str_literal_333_chpl;
#line 1425 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          i_pattern_chpl2 = &inlineImm_chpl3;
#line 1425 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          inlineImm_chpl4 = _str_literal_163_chpl;
#line 1425 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          i_replacement_chpl2 = &inlineImm_chpl4;
#line 1425 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          doReplace_chpl(x_chpl, i_pattern_chpl2, i_replacement_chpl2, INT64(-1), &ret_tmp_chpl7, _ln_chpl, _fn_chpl);
#line 1425 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          call_tmp_chpl21 = ret_tmp_chpl7;
#line 1425 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          call_tmp_chpl20 = call_tmp_chpl21;
#line 1425 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          chpl___ASSIGN_16(x_chpl, &call_tmp_chpl20, _ln_chpl, _fn_chpl);
#line 1425 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          i_x_chpl6 = &call_tmp_chpl20;
#line 1425 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
          deinit_chpl95(i_x_chpl6, _ln_chpl, _fn_chpl);
#line 1425 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
        }
#line 1425 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      }
#line 1425 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    }
#line 1425 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  }
#line 1432 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  if ((x_chpl)->buffLen > INT64(0)) /* ZLINE: 1432 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 1432 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  {
#line 1432 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    coerce_tmp_chpl3 = (x_chpl)->buff;
#line 1432 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    coerce_tmp_chpl4 = (x_chpl)->buffLen;
#line 1432 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl22 = (coerce_tmp_chpl3 + coerce_tmp_chpl4);
#line 1432 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    *(call_tmp_chpl22) = ((uint8_t)(INT64(0)));
#line 1432 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  }
#line 1401 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  return;
#line 1401 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
}

#line 1435 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
/* BytesStringCommon.chpl:1435 */
#line 1435 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
static chpl_bool _isSingleWord_chpl(string_chpl * x_chpl,
#line 1435 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                                    int64_t _ln_chpl,
#line 1435 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
                                    int32_t _fn_chpl) {
#line 1435 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl_bool ret_chpl;
#line 1439 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl ret_chpl2;
#line 1439 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl ret_chpl3;
#line 1439 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  string_chpl localThis_chpl;
#line 1439 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t coerce_tmp_chpl3;
#line 1439 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t tmp_chpl;
#line 1439 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t _ic__F1_high_chpl;
#line 1439 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int64_t i_chpl;
#line 1439 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  uint8_t yret_chpl;
#line 1439 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  c_ptr_uint8_t_chpl coerce_tmp_chpl4 = NULL;
#line 1439 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref_uint8_t_chpl call_tmp_chpl6 = NULL;
#line 1440 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  int32_t call_tmp_chpl7;
#line 1439 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 1439 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ref_string_chpl i_x_chpl2 = NULL;
#line 1439 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  init_chpl194(&ret_chpl3, _ln_chpl, _fn_chpl);
#line 1439 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  (&ret_chpl3)->cachedNumCodepoints = (x_chpl)->cachedNumCodepoints;
#line 1439 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  initWithBorrowedBuffer_chpl(&ret_chpl3, x_chpl, _ln_chpl, _fn_chpl);
#line 1439 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  ret_chpl2 = ret_chpl3;
#line 1439 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  localThis_chpl = ret_chpl2;
#line 1439 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  coerce_tmp_chpl3 = (&localThis_chpl)->buffLen;
#line 1439 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  if (coerce_tmp_chpl3 < INT64(0)) /* ZLINE: 1439 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 1439 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  {
#line 1439 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    boundsCheckHalt_chpl(&_str_literal_580_chpl, _ln_chpl, _fn_chpl);
#line 1439 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  }
#line 1439 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  if (coerce_tmp_chpl3 == INT64(0)) /* ZLINE: 1439 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 1439 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  {
#line 1439 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    tmp_chpl = ((int64_t)(((uint64_t)((((uint64_t)(INT64(0))) - UINT64(1))))));
#line 1439 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  } else /* ZLINE: 1439 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 1439 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  {
#line 1439 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    tmp_chpl = ((int64_t)((INT64(0) + ((int64_t)((coerce_tmp_chpl3 - INT64(1)))))));
#line 1439 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  }
#line 1439 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _ic__F1_high_chpl = tmp_chpl;
#line 1439 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  i_chpl = INT64(0);
#line 1439 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  chpl_checkIfRangeIterWillOverflow(INT64(0), tmp_chpl, INT64(1), INT64(0), tmp_chpl, UINT8(true), _ln_chpl, _fn_chpl);
#line 1439 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  for (i_chpl = INT64(0); ((i_chpl <= _ic__F1_high_chpl)); i_chpl += INT64(1)) {
#line 1439 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    coerce_tmp_chpl4 = (&localThis_chpl)->buff;
#line 1439 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl6 = (coerce_tmp_chpl4 + i_chpl);
#line 1439 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    yret_chpl = *(call_tmp_chpl6);
#line 1440 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    call_tmp_chpl7 = isspace(((int32_t)(yret_chpl)));
#line 1440 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    if (call_tmp_chpl7 != ((int32_t)(INT64(0)))) /* ZLINE: 1440 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl */
#line 1440 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    {
#line 1440 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      ret_chpl = UINT8(false);
#line 1439 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      i_x_chpl = &localThis_chpl;
#line 1439 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      deinit_chpl95(i_x_chpl, _ln_chpl, _fn_chpl);
#line 1440 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
      goto _end__isSingleWord_chpl;
#line 1440 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
    }
#line 1440 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  }
#line 1439 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  i_x_chpl2 = &localThis_chpl;
#line 1439 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  deinit_chpl95(i_x_chpl2, _ln_chpl, _fn_chpl);
#line 1442 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  ret_chpl = UINT8(true);
#line 1442 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  _end__isSingleWord_chpl:;
#line 1435 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
  return ret_chpl;
#line 1435 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesStringCommon.chpl"
}

