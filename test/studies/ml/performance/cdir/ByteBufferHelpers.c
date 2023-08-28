#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
/* ByteBufferHelpers.chpl:21 */
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
static void chpl__init_ByteBufferHelpers(int64_t _ln_chpl,
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
                                         int32_t _fn_chpl) {
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
  _ref_int32_t_chpl refIndentLevel_chpl = NULL;
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
  if (chpl__init_ByteBufferHelpers_p) /* ZLINE: 21 /Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl */
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
  {
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
    goto _exit_chpl__init_ByteBufferHelpers_chpl;
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
  }
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
  printModuleInit_chpl("%*s\n", "ByteBufferHelpers", INT64(17), _ln_chpl, _fn_chpl);
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
  refIndentLevel_chpl = &moduleInitLevel_chpl;
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
  *(refIndentLevel_chpl) += INT64(1);
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
  chpl__init_ByteBufferHelpers_p = UINT8(true);
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
  {
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
    chpl__init_CTypes(_ln_chpl, _fn_chpl);
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
    chpl__init_POSIX(_ln_chpl, _fn_chpl);
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
    chpl__init_Communication(_ln_chpl, _fn_chpl);
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
    chpl__init_String(_ln_chpl, _fn_chpl);
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
  }
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
  *(refIndentLevel_chpl) -= INT64(1);
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
  _exit_chpl__init_ByteBufferHelpers_chpl:;
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
  return;
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
}

#line 49 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
/* ByteBufferHelpers.chpl:49 */
#line 49 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
static int16_t offset_STR_COPY_DATA_chpl(void) {
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
  int16_t call_tmp_chpl6;
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
  int16_t call_tmp_chpl7;
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
  call_tmp_chpl6 = chpl_memhook_md_num();
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
  call_tmp_chpl7 = (CHPL_RT_MD_STR_COPY_DATA - call_tmp_chpl6);
#line 49 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
  return call_tmp_chpl7;
#line 49 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
}

#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
/* ByteBufferHelpers.chpl:76 */
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
static void bufferRealloc_chpl(uint8_t * buf_chpl,
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
                               int64_t requestedSize_chpl,
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
                               _tuple_2_c_ptr_uint8_t_int64_t_chpl * _retArg_chpl,
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
                               int64_t _ln_chpl,
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
                               int32_t _fn_chpl) {
#line 77 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
  int64_t call_tmp_chpl6;
#line 77 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
  int64_t tmp_chpl;
#line 79 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
  int16_t call_tmp_chpl7;
#line 78 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
  c_ptr_void_chpl call_tmp_chpl8 = NULL;
#line 80 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
  _tuple_2_c_ptr_uint8_t_int64_t_chpl ret_tmp_chpl;
#line 77 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
  call_tmp_chpl6 = chpl_here_good_alloc_size(((int64_t)((requestedSize_chpl + INT64(1)))), _ln_chpl, _fn_chpl);
#line 77 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
  if (call_tmp_chpl6 > INT64(0)) /* ZLINE: 77 /Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl */
#line 77 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
  {
#line 77 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
    tmp_chpl = call_tmp_chpl6;
#line 77 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
  } else /* ZLINE: 77 /Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl */
#line 77 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
  {
#line 77 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
    tmp_chpl = INT64(0);
#line 77 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
  }
#line 79 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
  call_tmp_chpl7 = offset_STR_COPY_DATA_chpl();
#line 78 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
  call_tmp_chpl8 = chpl_here_realloc(((c_ptr_void_chpl)(buf_chpl)), tmp_chpl, call_tmp_chpl7, _ln_chpl, _fn_chpl);
#line 80 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
  (&ret_tmp_chpl)->x0 = ((c_ptr_uint8_t_chpl)(call_tmp_chpl8));
#line 80 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
  (&ret_tmp_chpl)->x1 = tmp_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
  *(_retArg_chpl) = ret_tmp_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
  return;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
}

#line 83 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
/* ByteBufferHelpers.chpl:83 */
#line 83 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
static void bufferEnsureSize_chpl(uint8_t * buf_chpl,
#line 83 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
                                  int64_t currentSize_chpl,
#line 83 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
                                  int64_t requestedSize_chpl,
#line 83 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
                                  _tuple_2_c_ptr_uint8_t_int64_t_chpl * _retArg_chpl,
#line 83 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
                                  int64_t _ln_chpl,
#line 83 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
                                  int32_t _fn_chpl) {
#line 83 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
  _tuple_2_c_ptr_uint8_t_int64_t_chpl ret_chpl;
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
  _tuple_2_c_ptr_uint8_t_int64_t_chpl call_tmp_chpl6;
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
  _tuple_2_c_ptr_uint8_t_int64_t_chpl ret_tmp_chpl;
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
  _tuple_2_c_ptr_uint8_t_int64_t_chpl ret_tmp_chpl2;
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
  if (currentSize_chpl < requestedSize_chpl) /* ZLINE: 84 /Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl */
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
  {
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
    bufferRealloc_chpl(buf_chpl, requestedSize_chpl, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
    call_tmp_chpl6 = ret_tmp_chpl;
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
    ret_chpl = call_tmp_chpl6;
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
    goto _end_bufferEnsureSize_chpl;
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
  } else /* ZLINE: 84 /Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl */
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
  {
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
    (&ret_tmp_chpl2)->x0 = buf_chpl;
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
    (&ret_tmp_chpl2)->x1 = currentSize_chpl;
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
    ret_chpl = ret_tmp_chpl2;
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
    goto _end_bufferEnsureSize_chpl;
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
  }
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
  _end_bufferEnsureSize_chpl:;
#line 83 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
  *(_retArg_chpl) = ret_chpl;
#line 83 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
  return;
#line 83 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
}

#line 90 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
/* ByteBufferHelpers.chpl:90 */
#line 90 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
static uint8_t * bufferCopyRemote_chpl(int64_t src_loc_id_chpl,
#line 90 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
                                       uint8_t * src_addr_chpl,
#line 90 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
                                       int64_t len_chpl,
#line 90 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
                                       int64_t _ln_chpl,
#line 90 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
                                       int32_t _fn_chpl) {
#line 92 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
  int64_t call_tmp_chpl6;
#line 92 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
  int64_t tmp_chpl;
#line 92 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
  int16_t call_tmp_chpl7;
#line 92 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
  c_ptr_void_chpl cast_tmp_chpl = NULL;
#line 92 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
  c_ptr_uint8_t_chpl call_tmp_chpl8 = NULL;
#line 92 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
  c_ptr_uint8_t_chpl dst_chpl = NULL;
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
  _owned_IllegalArgumentError_chpl2 call_tmp_chpl9;
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
  _owned_IllegalArgumentError_chpl2 ret_tmp_chpl;
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
  _ref__owned_IllegalArgumentError_chpl i_arg_chpl = NULL;
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
  IllegalArgumentError_chpl call_tmp_chpl10 = NULL;
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
  string_chpl call_tmp_chpl11;
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
  Error_chpl coerce_tmp_chpl3 = NULL;
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
  string_chpl ret_tmp_chpl2;
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
  uint64_t call_tmp_chpl12;
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
  c_ptr_void_chpl coerce_tmp_chpl4 = NULL;
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
  c_ptr_void_chpl coerce_tmp_chpl5 = NULL;
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
  chpl_bool tmp_chpl2;
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
  c_ptr_uint8_t_chpl call_tmp_chpl13 = NULL;
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
  _ref_uint8_t_chpl call_tmp_chpl14 = NULL;
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
  c_ptr_uint8_t_chpl call_tmp_chpl15 = NULL;
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
  _ref_uint8_t_chpl call_tmp_chpl16 = NULL;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
  _ref_uint8_t_chpl call_tmp_chpl17 = NULL;
#line 92 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
  call_tmp_chpl6 = chpl_here_good_alloc_size(((int64_t)((len_chpl + INT64(1)))), _ln_chpl, _fn_chpl);
#line 92 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
  if (call_tmp_chpl6 > INT64(0)) /* ZLINE: 92 /Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl */
#line 92 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
  {
#line 92 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
    tmp_chpl = call_tmp_chpl6;
#line 92 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
  } else /* ZLINE: 92 /Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl */
#line 92 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
  {
#line 92 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
    tmp_chpl = INT64(0);
#line 92 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
  }
#line 92 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
  call_tmp_chpl7 = offset_STR_COPY_DATA_chpl();
#line 92 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
  cast_tmp_chpl = chpl_here_alloc(tmp_chpl, call_tmp_chpl7, _ln_chpl, _fn_chpl);
#line 92 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
  call_tmp_chpl8 = ((c_ptr_uint8_t_chpl)(cast_tmp_chpl));
#line 92 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
  dst_chpl = call_tmp_chpl8;
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
  chpl_checkValue7(len_chpl, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
  call_tmp_chpl9 = ret_tmp_chpl;
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
  i_arg_chpl = &call_tmp_chpl9;
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
  call_tmp_chpl10 = borrow_chpl2(i_arg_chpl);
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
  if (((RootClass_chpl)(call_tmp_chpl10)) != nil) /* ZLINE: 93 /Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl */
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
  {
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
    coerce_tmp_chpl3 = ((Error_chpl)(call_tmp_chpl10));
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
    chpl_check_nil(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
    ((void(*)(Error_chpl,
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
       string_chpl *,
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
       int64_t,
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
       int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(coerce_tmp_chpl3))->chpl__cid) + INT64(1))])(coerce_tmp_chpl3, &ret_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
    call_tmp_chpl11 = ret_tmp_chpl2;
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
    safeCastCheckHalt_chpl(&call_tmp_chpl11, _ln_chpl, _fn_chpl);
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
    i_x_chpl = &call_tmp_chpl11;
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
    deinit_chpl95(i_x_chpl, _ln_chpl, _fn_chpl);
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
  }
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
  chpl__autoDestroy62(&call_tmp_chpl9, _ln_chpl, _fn_chpl);
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
  call_tmp_chpl12 = ((uint64_t)(len_chpl));
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
  coerce_tmp_chpl4 = ((c_ptr_void_chpl)(call_tmp_chpl8));
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
  coerce_tmp_chpl5 = ((c_ptr_void_chpl)(src_addr_chpl));
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
  if (src_loc_id_chpl < INT64(0)) /* ZLINE: 93 /Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl */
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
  {
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
    tmp_chpl2 = UINT8(true);
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
  } else /* ZLINE: 93 /Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl */
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
  {
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
    tmp_chpl2 = (src_loc_id_chpl >= ((int64_t)(chpl_numNodes)));
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
  }
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
  if (tmp_chpl2) /* ZLINE: 93 /Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl */
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
  {
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
    halt_chpl3(&_str_literal_3130_chpl, src_loc_id_chpl, &_str_literal_3131_chpl, _ln_chpl, _fn_chpl);
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
  }
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
  call_tmp_chpl13 = ((c_ptr_uint8_t_chpl)(coerce_tmp_chpl4));
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
  call_tmp_chpl14 = (call_tmp_chpl13 + INT64(0));
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
  call_tmp_chpl15 = ((c_ptr_uint8_t_chpl)(coerce_tmp_chpl5));
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
  call_tmp_chpl16 = (call_tmp_chpl15 + INT64(0));
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
  chpl_gen_comm_get(((void*)(call_tmp_chpl14)), src_loc_id_chpl, call_tmp_chpl16, call_tmp_chpl12, COMMID(0), _ln_chpl, _fn_chpl);
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
  call_tmp_chpl17 = (dst_chpl + len_chpl);
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
  *(call_tmp_chpl17) = ((uint8_t)(INT64(0)));
#line 90 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
  return call_tmp_chpl8;
#line 90 "/Users/iainmoncrief/Documents/chapel/modules/internal/ByteBufferHelpers.chpl"
}

