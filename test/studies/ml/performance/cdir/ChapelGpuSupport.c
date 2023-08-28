#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
/* ChapelGpuSupport.chpl:21 */
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
static void chpl__init_ChapelGpuSupport(int64_t _ln_chpl,
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
                                        int32_t _fn_chpl) {
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
  _ref_int32_t_chpl refIndentLevel_chpl = NULL;
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
  chpl_bool tmp_chpl;
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
  chpl_bool call_tmp_chpl6;
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
  c_ptrConst_int8_t_chpl call_tmp_chpl7 = NULL;
#line 29 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
  chpl_bool tmp_chpl2;
#line 29 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
  chpl_bool call_tmp_chpl8;
#line 29 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
  chpl_bool call_tmp_chpl9;
#line 29 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
  c_ptrConst_int8_t_chpl call_tmp_chpl10 = NULL;
#line 34 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
  chpl_bool tmp_chpl3;
#line 34 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
  chpl_bool call_tmp_chpl11;
#line 34 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
  c_ptrConst_int8_t_chpl call_tmp_chpl12 = NULL;
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
  if (chpl__init_ChapelGpuSupport_p) /* ZLINE: 21 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl */
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
  {
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
    goto _exit_chpl__init_ChapelGpuSupport_chpl;
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
  }
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
  printModuleInit_chpl("%*s\n", "ChapelGpuSupport", INT64(16), _ln_chpl, _fn_chpl);
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
  refIndentLevel_chpl = &moduleInitLevel_chpl;
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
  *(refIndentLevel_chpl) += INT64(1);
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
  chpl__init_ChapelGpuSupport_p = UINT8(true);
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
  {
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
    chpl__init_ChapelBase(_ln_chpl, _fn_chpl);
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
    chpl__init_CTypes(_ln_chpl, _fn_chpl);
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
    chpl__init_String(_ln_chpl, _fn_chpl);
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
  }
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
  call_tmp_chpl6 = chpl_config_has_value(((c_ptrConst_int8_t_chpl)("debugGpu")), ((c_ptrConst_int8_t_chpl)("Built-in")));
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
  if (! call_tmp_chpl6) /* ZLINE: 26 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl */
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
  {
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
    tmp_chpl = UINT8(false);
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
  } else /* ZLINE: 26 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl */
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
  {
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
    call_tmp_chpl7 = chpl_config_get_value(((c_ptrConst_int8_t_chpl)("debugGpu")), ((c_ptrConst_int8_t_chpl)("Built-in")));
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
    tmp_chpl = _command_line_cast_chpl(call_tmp_chpl7, _ln_chpl, _fn_chpl);
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
  }
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
  debugGpu_chpl = tmp_chpl;
#line 29 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
  call_tmp_chpl8 = chpl_config_has_value(((c_ptrConst_int8_t_chpl)("gpuNoCpuModeWarning")), ((c_ptrConst_int8_t_chpl)("Built-in")));
#line 29 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
  if (! call_tmp_chpl8) /* ZLINE: 29 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl */
#line 29 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
  {
#line 29 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
    call_tmp_chpl9 = isEnvSet_chpl(&_str_literal_1300_chpl, _ln_chpl, _fn_chpl);
#line 29 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
    tmp_chpl2 = call_tmp_chpl9;
#line 29 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
  } else /* ZLINE: 29 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl */
#line 29 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
  {
#line 29 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
    call_tmp_chpl10 = chpl_config_get_value(((c_ptrConst_int8_t_chpl)("gpuNoCpuModeWarning")), ((c_ptrConst_int8_t_chpl)("Built-in")));
#line 29 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
    tmp_chpl2 = _command_line_cast_chpl8(call_tmp_chpl10, _ln_chpl, _fn_chpl);
#line 29 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
  }
#line 29 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
  gpuNoCpuModeWarning_chpl = tmp_chpl2;
#line 34 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
  call_tmp_chpl11 = chpl_config_has_value(((c_ptrConst_int8_t_chpl)("enableGpuP2P")), ((c_ptrConst_int8_t_chpl)("Built-in")));
#line 34 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
  if (! call_tmp_chpl11) /* ZLINE: 34 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl */
#line 34 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
  {
#line 34 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
    tmp_chpl3 = UINT8(false);
#line 34 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
  } else /* ZLINE: 34 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl */
#line 34 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
  {
#line 34 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
    call_tmp_chpl12 = chpl_config_get_value(((c_ptrConst_int8_t_chpl)("enableGpuP2P")), ((c_ptrConst_int8_t_chpl)("Built-in")));
#line 34 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
    tmp_chpl3 = _command_line_cast_chpl4(call_tmp_chpl12, _ln_chpl, _fn_chpl);
#line 34 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
  }
#line 34 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
  enableGpuP2P_chpl = tmp_chpl3;
#line 39 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
  chpl_gpu_debug = debugGpu_chpl;
#line 40 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
  chpl_gpu_no_cpu_mode_warning = gpuNoCpuModeWarning_chpl;
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
  *(refIndentLevel_chpl) -= INT64(1);
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
  _exit_chpl__init_ChapelGpuSupport_chpl:;
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
  return;
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
}

#line 63 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
/* ChapelGpuSupport.chpl:63 */
#line 63 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
static chpl_bool isEnvSet_chpl(string_chpl * name_chpl,
#line 63 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
                               int64_t _ln_chpl,
#line 63 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
                               int32_t _fn_chpl) {
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
  string_chpl call_tmp_chpl6;
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
  string_chpl ret_chpl;
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
  string_chpl ret_chpl2;
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
  c_ptrConst_int8_t_chpl call_tmp_chpl7 = NULL;
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
  int64_t call_tmp_chpl8;
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
  _owned_IllegalArgumentError_chpl2 call_tmp_chpl9;
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
  _owned_IllegalArgumentError_chpl2 ret_tmp_chpl;
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
  _ref__owned_IllegalArgumentError_chpl i_arg_chpl = NULL;
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
  IllegalArgumentError_chpl call_tmp_chpl10 = NULL;
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
  string_chpl call_tmp_chpl11;
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
  Error_chpl coerce_tmp_chpl3 = NULL;
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
  string_chpl ret_tmp_chpl2;
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
  int64_t call_tmp_chpl12;
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
  c_ptr_uint8_t_chpl call_tmp_chpl13 = NULL;
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
  int64_t call_tmp_chpl14;
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
  bytes_chpl ret_chpl3;
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
  bytes_chpl ret_chpl4;
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
  bytes_chpl env_chpl;
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
  _ref_string_chpl i_x_chpl2 = NULL;
#line 67 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
  chpl_bool call_tmp_chpl15;
#line 67 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
  _ref_bytes_chpl i_x_chpl3 = NULL;
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
  init_chpl194(&ret_chpl2, _ln_chpl, _fn_chpl);
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
  (&ret_chpl2)->cachedNumCodepoints = (name_chpl)->cachedNumCodepoints;
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
  initWithBorrowedBuffer_chpl(&ret_chpl2, name_chpl, _ln_chpl, _fn_chpl);
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
  ret_chpl = ret_chpl2;
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
  call_tmp_chpl6 = ret_chpl;
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
  call_tmp_chpl7 = getenv(((c_ptrConst_int8_t_chpl)((&call_tmp_chpl6)->buff)));
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
  call_tmp_chpl8 = string_length_bytes(call_tmp_chpl7);
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
  chpl_checkValue2(call_tmp_chpl8, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
  call_tmp_chpl9 = ret_tmp_chpl;
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
  i_arg_chpl = &call_tmp_chpl9;
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
  call_tmp_chpl10 = borrow_chpl2(i_arg_chpl);
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
  if (((RootClass_chpl)(call_tmp_chpl10)) != nil) /* ZLINE: 66 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl */
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
  {
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
    coerce_tmp_chpl3 = ((Error_chpl)(call_tmp_chpl10));
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
    chpl_check_nil(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
    ((void(*)(Error_chpl,
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
       string_chpl *,
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
       int64_t,
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
       int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(coerce_tmp_chpl3))->chpl__cid) + INT64(1))])(coerce_tmp_chpl3, &ret_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
    call_tmp_chpl11 = ret_tmp_chpl2;
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
    safeCastCheckHalt_chpl(&call_tmp_chpl11, _ln_chpl, _fn_chpl);
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
    i_x_chpl = &call_tmp_chpl11;
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
    deinit_chpl95(i_x_chpl, _ln_chpl, _fn_chpl);
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
  }
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
  chpl__autoDestroy62(&call_tmp_chpl9, _ln_chpl, _fn_chpl);
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
  call_tmp_chpl12 = call_tmp_chpl8;
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
  call_tmp_chpl13 = ((c_ptr_uint8_t_chpl)(call_tmp_chpl7));
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
  call_tmp_chpl14 = (call_tmp_chpl12 + INT64(1));
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
  init_chpl19(&ret_chpl4, _ln_chpl, _fn_chpl);
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
  initWithBorrowedBuffer_chpl3(&ret_chpl4, call_tmp_chpl13, call_tmp_chpl12, call_tmp_chpl14);
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
  ret_chpl3 = ret_chpl4;
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
  env_chpl = ret_chpl3;
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
  i_x_chpl2 = &call_tmp_chpl6;
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
  deinit_chpl95(i_x_chpl2, _ln_chpl, _fn_chpl);
#line 67 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
  call_tmp_chpl15 = (! ((&env_chpl)->buffLen == INT64(0)));
#line 67 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
  i_x_chpl3 = &env_chpl;
#line 67 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
  deinit_chpl3(i_x_chpl3, _ln_chpl, _fn_chpl);
#line 63 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
  return call_tmp_chpl15;
#line 63 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
}

#line 71 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
/* ChapelGpuSupport.chpl:71 */
#line 71 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
void chpl_gpu_comm_on_put(int32_t dst_subloc,
#line 71 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
                          void * addr,
#line 71 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
                          int32_t src_node,
#line 71 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
                          int32_t src_subloc,
#line 71 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
                          void * raddr,
#line 71 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
                          uint64_t size) {
#line 86 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
  c_ptr_uint8_t_chpl call_tmp_chpl6 = NULL;
#line 86 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
  _ref_uint8_t_chpl call_tmp_chpl7 = NULL;
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
  c_ptr_uint8_t_chpl call_tmp_chpl8 = NULL;
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
  _ref_uint8_t_chpl call_tmp_chpl9 = NULL;
#line 82 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
  chpl_buildLocaleID(src_node, src_subloc, INT64(82), INT32(43));
#line 86 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
  call_tmp_chpl6 = ((c_ptr_uint8_t_chpl)(addr));
#line 86 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
  call_tmp_chpl7 = (call_tmp_chpl6 + INT64(0));
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
  call_tmp_chpl8 = ((c_ptr_uint8_t_chpl)(raddr));
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
  call_tmp_chpl9 = (call_tmp_chpl8 + INT64(0));
#line 88 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
  chpl_gen_comm_put(((void*)(call_tmp_chpl9)), chpl_nodeID, call_tmp_chpl7, size, COMMID(0), INT64(88), INT32(43));
#line 71 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
  return;
#line 71 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
}

#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
/* ChapelGpuSupport.chpl:94 */
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
void chpl_gpu_comm_on_get(int32_t src_subloc,
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
                          void * addr,
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
                          int32_t dst_node,
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
                          int32_t dst_subloc,
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
                          void * raddr,
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
                          uint64_t size) {
#line 109 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
  c_ptr_uint8_t_chpl call_tmp_chpl6 = NULL;
#line 109 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
  _ref_uint8_t_chpl call_tmp_chpl7 = NULL;
#line 110 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
  c_ptr_uint8_t_chpl call_tmp_chpl8 = NULL;
#line 110 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
  _ref_uint8_t_chpl call_tmp_chpl9 = NULL;
#line 105 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
  chpl_buildLocaleID(dst_node, dst_subloc, INT64(105), INT32(43));
#line 109 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
  call_tmp_chpl6 = ((c_ptr_uint8_t_chpl)(addr));
#line 109 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
  call_tmp_chpl7 = (call_tmp_chpl6 + INT64(0));
#line 110 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
  call_tmp_chpl8 = ((c_ptr_uint8_t_chpl)(raddr));
#line 110 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
  call_tmp_chpl9 = (call_tmp_chpl8 + INT64(0));
#line 111 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
  chpl_gen_comm_get(((void*)(call_tmp_chpl9)), chpl_nodeID, call_tmp_chpl7, size, COMMID(1), INT64(111), INT32(43));
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
  return;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelGpuSupport.chpl"
}

