#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
/* MemTracking.chpl:23 */
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
static void chpl__init_MemTracking(int64_t _ln_chpl,
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
                                   int32_t _fn_chpl) {
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  _ref_int32_t_chpl refIndentLevel_chpl = NULL;
#line 28 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  chpl_bool tmp_chpl;
#line 28 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  chpl_bool call_tmp_chpl6;
#line 28 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  c_ptrConst_int8_t_chpl call_tmp_chpl7 = NULL;
#line 29 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  chpl_bool tmp_chpl2;
#line 29 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  chpl_bool call_tmp_chpl8;
#line 29 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  c_ptrConst_int8_t_chpl call_tmp_chpl9 = NULL;
#line 30 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  chpl_bool tmp_chpl3;
#line 30 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  chpl_bool call_tmp_chpl10;
#line 30 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  c_ptrConst_int8_t_chpl call_tmp_chpl11 = NULL;
#line 31 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  chpl_bool tmp_chpl4;
#line 31 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  chpl_bool call_tmp_chpl12;
#line 31 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  c_ptrConst_int8_t_chpl call_tmp_chpl13 = NULL;
#line 32 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  uint64_t tmp_chpl5;
#line 32 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  chpl_bool call_tmp_chpl14;
#line 32 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  c_ptrConst_int8_t_chpl call_tmp_chpl15 = NULL;
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  uint64_t tmp_chpl6;
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  chpl_bool call_tmp_chpl16;
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  c_ptrConst_int8_t_chpl call_tmp_chpl17 = NULL;
#line 34 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  string_chpl tmp_chpl7;
#line 34 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  chpl_bool call_tmp_chpl18;
#line 34 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  c_ptrConst_int8_t_chpl call_tmp_chpl19 = NULL;
#line 34 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  string_chpl ret_tmp_chpl;
#line 38 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  string_chpl tmp_chpl8;
#line 38 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  chpl_bool call_tmp_chpl20;
#line 38 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  c_ptrConst_int8_t_chpl call_tmp_chpl21 = NULL;
#line 38 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  string_chpl ret_tmp_chpl2;
#line 55 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  string_chpl tmp_chpl9;
#line 55 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  chpl_bool call_tmp_chpl22;
#line 55 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  c_ptrConst_int8_t_chpl call_tmp_chpl23 = NULL;
#line 55 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  string_chpl ret_tmp_chpl3;
#line 58 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  _owned_IllegalArgumentError_chpl2 call_tmp_chpl24;
#line 58 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  _owned_IllegalArgumentError_chpl2 ret_tmp_chpl4;
#line 58 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  _ref__owned_IllegalArgumentError_chpl i_arg_chpl = NULL;
#line 58 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  IllegalArgumentError_chpl call_tmp_chpl25 = NULL;
#line 58 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  string_chpl call_tmp_chpl26;
#line 58 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  Error_chpl coerce_tmp_chpl3 = NULL;
#line 58 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  string_chpl ret_tmp_chpl5;
#line 58 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 59 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  _owned_IllegalArgumentError_chpl2 call_tmp_chpl27;
#line 59 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  _owned_IllegalArgumentError_chpl2 ret_tmp_chpl6;
#line 59 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  _ref__owned_IllegalArgumentError_chpl i_arg_chpl2 = NULL;
#line 59 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  IllegalArgumentError_chpl call_tmp_chpl28 = NULL;
#line 59 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  string_chpl call_tmp_chpl29;
#line 59 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  Error_chpl coerce_tmp_chpl4 = NULL;
#line 59 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  string_chpl ret_tmp_chpl7;
#line 59 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  _ref_string_chpl i_x_chpl2 = NULL;
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  if (chpl__init_MemTracking_p) /* ZLINE: 23 /Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl */
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  {
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
    goto _exit_chpl__init_MemTracking_chpl;
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  }
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  printModuleInit_chpl("%*s\n", "MemTracking", INT64(11), _ln_chpl, _fn_chpl);
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  refIndentLevel_chpl = &moduleInitLevel_chpl;
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  *(refIndentLevel_chpl) += INT64(1);
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  chpl__init_MemTracking_p = UINT8(true);
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  {
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
    chpl__init_CTypes(_ln_chpl, _fn_chpl);
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  }
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  chpl_addModule("MemTracking", ((c_fn_ptr)(chpl__deinit_MemTracking)), _ln_chpl, _fn_chpl);
#line 28 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  call_tmp_chpl6 = chpl_config_has_value(((c_ptrConst_int8_t_chpl)("memTrack")), ((c_ptrConst_int8_t_chpl)("Built-in")));
#line 28 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  if (! call_tmp_chpl6) /* ZLINE: 28 /Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl */
#line 28 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  {
#line 28 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
    tmp_chpl = UINT8(false);
#line 28 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  } else /* ZLINE: 28 /Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl */
#line 28 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  {
#line 28 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
    call_tmp_chpl7 = chpl_config_get_value(((c_ptrConst_int8_t_chpl)("memTrack")), ((c_ptrConst_int8_t_chpl)("Built-in")));
#line 28 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
    tmp_chpl = _command_line_cast_chpl21(call_tmp_chpl7, _ln_chpl, _fn_chpl);
#line 28 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  }
#line 28 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  memTrack_chpl = tmp_chpl;
#line 29 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  call_tmp_chpl8 = chpl_config_has_value(((c_ptrConst_int8_t_chpl)("memStats")), ((c_ptrConst_int8_t_chpl)("Built-in")));
#line 29 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  if (! call_tmp_chpl8) /* ZLINE: 29 /Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl */
#line 29 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  {
#line 29 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
    tmp_chpl2 = UINT8(false);
#line 29 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  } else /* ZLINE: 29 /Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl */
#line 29 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  {
#line 29 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
    call_tmp_chpl9 = chpl_config_get_value(((c_ptrConst_int8_t_chpl)("memStats")), ((c_ptrConst_int8_t_chpl)("Built-in")));
#line 29 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
    tmp_chpl2 = _command_line_cast_chpl23(call_tmp_chpl9, _ln_chpl, _fn_chpl);
#line 29 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  }
#line 29 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  memStats_chpl = tmp_chpl2;
#line 30 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  call_tmp_chpl10 = chpl_config_has_value(((c_ptrConst_int8_t_chpl)("memLeaksByType")), ((c_ptrConst_int8_t_chpl)("Built-in")));
#line 30 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  if (! call_tmp_chpl10) /* ZLINE: 30 /Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl */
#line 30 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  {
#line 30 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
    tmp_chpl3 = UINT8(false);
#line 30 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  } else /* ZLINE: 30 /Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl */
#line 30 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  {
#line 30 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
    call_tmp_chpl11 = chpl_config_get_value(((c_ptrConst_int8_t_chpl)("memLeaksByType")), ((c_ptrConst_int8_t_chpl)("Built-in")));
#line 30 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
    tmp_chpl3 = _command_line_cast_chpl22(call_tmp_chpl11, _ln_chpl, _fn_chpl);
#line 30 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  }
#line 30 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  memLeaksByType_chpl = tmp_chpl3;
#line 31 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  call_tmp_chpl12 = chpl_config_has_value(((c_ptrConst_int8_t_chpl)("memLeaks")), ((c_ptrConst_int8_t_chpl)("Built-in")));
#line 31 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  if (! call_tmp_chpl12) /* ZLINE: 31 /Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl */
#line 31 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  {
#line 31 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
    tmp_chpl4 = UINT8(false);
#line 31 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  } else /* ZLINE: 31 /Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl */
#line 31 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  {
#line 31 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
    call_tmp_chpl13 = chpl_config_get_value(((c_ptrConst_int8_t_chpl)("memLeaks")), ((c_ptrConst_int8_t_chpl)("Built-in")));
#line 31 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
    tmp_chpl4 = _command_line_cast_chpl20(call_tmp_chpl13, _ln_chpl, _fn_chpl);
#line 31 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  }
#line 31 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  memLeaks_chpl = tmp_chpl4;
#line 32 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  call_tmp_chpl14 = chpl_config_has_value(((c_ptrConst_int8_t_chpl)("memMax")), ((c_ptrConst_int8_t_chpl)("Built-in")));
#line 32 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  if (! call_tmp_chpl14) /* ZLINE: 32 /Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl */
#line 32 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  {
#line 32 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
    tmp_chpl5 = UINT64(0);
#line 32 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  } else /* ZLINE: 32 /Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl */
#line 32 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  {
#line 32 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
    call_tmp_chpl15 = chpl_config_get_value(((c_ptrConst_int8_t_chpl)("memMax")), ((c_ptrConst_int8_t_chpl)("Built-in")));
#line 32 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
    tmp_chpl5 = _command_line_cast_chpl18(call_tmp_chpl15, _ln_chpl, _fn_chpl);
#line 32 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  }
#line 32 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  memMax_chpl = tmp_chpl5;
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  call_tmp_chpl16 = chpl_config_has_value(((c_ptrConst_int8_t_chpl)("memThreshold")), ((c_ptrConst_int8_t_chpl)("Built-in")));
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  if (! call_tmp_chpl16) /* ZLINE: 33 /Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl */
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  {
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
    tmp_chpl6 = UINT64(0);
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  } else /* ZLINE: 33 /Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl */
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  {
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
    call_tmp_chpl17 = chpl_config_get_value(((c_ptrConst_int8_t_chpl)("memThreshold")), ((c_ptrConst_int8_t_chpl)("Built-in")));
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
    tmp_chpl6 = _command_line_cast_chpl16(call_tmp_chpl17, _ln_chpl, _fn_chpl);
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  }
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  memThreshold_chpl = tmp_chpl6;
#line 34 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  call_tmp_chpl18 = chpl_config_has_value(((c_ptrConst_int8_t_chpl)("memLog")), ((c_ptrConst_int8_t_chpl)("Built-in")));
#line 34 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  if (! call_tmp_chpl18) /* ZLINE: 34 /Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl */
#line 34 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  {
#line 34 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
    init_chpl194(&tmp_chpl7, _ln_chpl, _fn_chpl);
#line 34 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  } else /* ZLINE: 34 /Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl */
#line 34 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  {
#line 34 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
    call_tmp_chpl19 = chpl_config_get_value(((c_ptrConst_int8_t_chpl)("memLog")), ((c_ptrConst_int8_t_chpl)("Built-in")));
#line 34 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
    _command_line_cast_chpl12(call_tmp_chpl19, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 34 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
    tmp_chpl7 = ret_tmp_chpl;
#line 34 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  }
#line 34 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  memLog_chpl = tmp_chpl7;
#line 38 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  call_tmp_chpl20 = chpl_config_has_value(((c_ptrConst_int8_t_chpl)("memLeaksLog")), ((c_ptrConst_int8_t_chpl)("Built-in")));
#line 38 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  if (! call_tmp_chpl20) /* ZLINE: 38 /Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl */
#line 38 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  {
#line 38 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
    init_chpl194(&tmp_chpl8, _ln_chpl, _fn_chpl);
#line 38 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  } else /* ZLINE: 38 /Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl */
#line 38 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  {
#line 38 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
    call_tmp_chpl21 = chpl_config_get_value(((c_ptrConst_int8_t_chpl)("memLeaksLog")), ((c_ptrConst_int8_t_chpl)("Built-in")));
#line 38 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
    _command_line_cast_chpl11(call_tmp_chpl21, &ret_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 38 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
    tmp_chpl8 = ret_tmp_chpl2;
#line 38 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  }
#line 38 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  memLeaksLog_chpl = tmp_chpl8;
#line 55 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  call_tmp_chpl22 = chpl_config_has_value(((c_ptrConst_int8_t_chpl)("memLeaksByDesc")), ((c_ptrConst_int8_t_chpl)("Built-in")));
#line 55 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  if (! call_tmp_chpl22) /* ZLINE: 55 /Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl */
#line 55 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  {
#line 55 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
    init_chpl194(&tmp_chpl9, _ln_chpl, _fn_chpl);
#line 55 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  } else /* ZLINE: 55 /Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl */
#line 55 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  {
#line 55 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
    call_tmp_chpl23 = chpl_config_get_value(((c_ptrConst_int8_t_chpl)("memLeaksByDesc")), ((c_ptrConst_int8_t_chpl)("Built-in")));
#line 55 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
    _command_line_cast_chpl10(call_tmp_chpl23, &ret_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 55 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
    tmp_chpl9 = ret_tmp_chpl3;
#line 55 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  }
#line 55 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  memLeaksByDesc_chpl = tmp_chpl9;
#line 58 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  chpl_checkValue5(memMax_chpl, &ret_tmp_chpl4, _ln_chpl, _fn_chpl);
#line 58 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  call_tmp_chpl24 = ret_tmp_chpl4;
#line 58 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  i_arg_chpl = &call_tmp_chpl24;
#line 58 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  call_tmp_chpl25 = borrow_chpl2(i_arg_chpl);
#line 58 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  if (((RootClass_chpl)(call_tmp_chpl25)) != nil) /* ZLINE: 58 /Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl */
#line 58 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  {
#line 58 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
    coerce_tmp_chpl3 = ((Error_chpl)(call_tmp_chpl25));
#line 58 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
    chpl_check_nil(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 58 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
    ((void(*)(Error_chpl,
#line 58 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
       string_chpl *,
#line 58 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
       int64_t,
#line 58 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
       int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(coerce_tmp_chpl3))->chpl__cid) + INT64(1))])(coerce_tmp_chpl3, &ret_tmp_chpl5, _ln_chpl, _fn_chpl);
#line 58 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
    call_tmp_chpl26 = ret_tmp_chpl5;
#line 58 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
    safeCastCheckHalt_chpl(&call_tmp_chpl26, _ln_chpl, _fn_chpl);
#line 58 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
    i_x_chpl = &call_tmp_chpl26;
#line 58 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
    deinit_chpl95(i_x_chpl, _ln_chpl, _fn_chpl);
#line 58 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  }
#line 58 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  chpl__autoDestroy62(&call_tmp_chpl24, _ln_chpl, _fn_chpl);
#line 58 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  cMemMax_chpl = memMax_chpl;
#line 59 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  chpl_checkValue5(memThreshold_chpl, &ret_tmp_chpl6, _ln_chpl, _fn_chpl);
#line 59 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  call_tmp_chpl27 = ret_tmp_chpl6;
#line 59 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  i_arg_chpl2 = &call_tmp_chpl27;
#line 59 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  call_tmp_chpl28 = borrow_chpl2(i_arg_chpl2);
#line 59 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  if (((RootClass_chpl)(call_tmp_chpl28)) != nil) /* ZLINE: 59 /Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl */
#line 59 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  {
#line 59 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
    coerce_tmp_chpl4 = ((Error_chpl)(call_tmp_chpl28));
#line 59 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
    chpl_check_nil(coerce_tmp_chpl4, _ln_chpl, _fn_chpl);
#line 59 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
    ((void(*)(Error_chpl,
#line 59 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
       string_chpl *,
#line 59 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
       int64_t,
#line 59 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
       int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(coerce_tmp_chpl4))->chpl__cid) + INT64(1))])(coerce_tmp_chpl4, &ret_tmp_chpl7, _ln_chpl, _fn_chpl);
#line 59 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
    call_tmp_chpl29 = ret_tmp_chpl7;
#line 59 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
    safeCastCheckHalt_chpl(&call_tmp_chpl29, _ln_chpl, _fn_chpl);
#line 59 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
    i_x_chpl2 = &call_tmp_chpl29;
#line 59 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
    deinit_chpl95(i_x_chpl2, _ln_chpl, _fn_chpl);
#line 59 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  }
#line 59 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  chpl__autoDestroy62(&call_tmp_chpl27, _ln_chpl, _fn_chpl);
#line 59 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  cMemThreshold_chpl = memThreshold_chpl;
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  *(refIndentLevel_chpl) -= INT64(1);
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  _exit_chpl__init_MemTracking_chpl:;
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  return;
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
}

#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
/* MemTracking.chpl:23 */
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
static void chpl__deinit_MemTracking(int64_t _ln_chpl,
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
                                     int32_t _fn_chpl) {
#line 34 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 34 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  i_x_chpl = &memLog_chpl;
#line 34 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  deinit_chpl95(i_x_chpl, _ln_chpl, _fn_chpl);
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  return;
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
}

#line 74 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
/* MemTracking.chpl:74 */
#line 74 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
void chpl_memTracking_returnConfigVals(chpl_bool * ret_memTrack,
#line 74 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
                                       chpl_bool * ret_memStats,
#line 74 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
                                       chpl_bool * ret_memLeaksByType,
#line 74 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
                                       c_ptrConst_int8_t_chpl * ret_memLeaksByDesc,
#line 74 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
                                       chpl_bool * ret_memLeaks,
#line 74 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
                                       uint64_t * ret_memMax,
#line 74 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
                                       uint64_t * ret_memThreshold,
#line 74 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
                                       c_ptrConst_int8_t_chpl * ret_memLog,
#line 74 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
                                       c_ptrConst_int8_t_chpl * ret_memLeaksLog) {
#line 109 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  string_chpl local_memLeaksLog_chpl;
#line 100 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  string_chpl local_memLog_chpl;
#line 91 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  string_chpl local_memLeaksByDesc_chpl;
#line 88 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  uint64_t local_cMemThreshold_chpl;
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  uint64_t local_cMemMax_chpl;
#line 86 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  chpl_bool local_memLeaks_chpl;
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  chpl_bool local_memLeaksByType_chpl;
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  chpl_bool local_memStats_chpl;
#line 90 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  locale_chpl call_tmp_chpl6;
#line 90 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  chpl_localeID_t call_tmp_chpl7;
#line 90 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  int32_t call_tmp_chpl8;
#line 90 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  chpl_localeID_t call_tmp_chpl9;
#line 90 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  locale_chpl call_tmp_chpl10;
#line 90 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  locale_chpl ret_tmp_chpl;
#line 90 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  BaseLocale_chpl coerce_tmp_chpl3 = NULL;
#line 90 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  int64_t call_tmp_chpl11;
#line 92 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  string_chpl local_memLeaksByDesc_chpl2;
#line 95 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 101 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  string_chpl local_memLog_chpl2;
#line 104 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  _ref_string_chpl i_x_chpl2 = NULL;
#line 110 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  string_chpl local_memLeaksLog_chpl2;
#line 113 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  _ref_string_chpl i_x_chpl3 = NULL;
#line 90 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  _ref_locale_chpl i_x_chpl4 = NULL;
#line 109 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  local_memLeaksLog_chpl = memLeaksLog_chpl;
#line 100 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  local_memLog_chpl = memLog_chpl;
#line 91 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  local_memLeaksByDesc_chpl = memLeaksByDesc_chpl;
#line 88 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  local_cMemThreshold_chpl = cMemThreshold_chpl;
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  local_cMemMax_chpl = cMemMax_chpl;
#line 86 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  local_memLeaks_chpl = memLeaks_chpl;
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  local_memLeaksByType_chpl = memLeaksByType_chpl;
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  local_memStats_chpl = memStats_chpl;
#line 83 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  *(ret_memTrack) = memTrack_chpl;
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  *(ret_memStats) = local_memStats_chpl;
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  *(ret_memLeaksByType) = local_memLeaksByType_chpl;
#line 86 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  *(ret_memLeaks) = local_memLeaks_chpl;
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  *(ret_memMax) = local_cMemMax_chpl;
#line 88 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  *(ret_memThreshold) = local_cMemThreshold_chpl;
#line 90 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  call_tmp_chpl8 = get_chpl_nodeID();
#line 90 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  call_tmp_chpl9 = chpl_rt_buildLocaleID(call_tmp_chpl8, c_sublocid_any);
#line 90 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  call_tmp_chpl7 = call_tmp_chpl9;
#line 90 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  chpl_localeID_to_locale(&call_tmp_chpl7, &ret_tmp_chpl, INT64(90), INT32(38));
#line 90 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  call_tmp_chpl10 = ret_tmp_chpl;
#line 90 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  call_tmp_chpl6 = call_tmp_chpl10;
#line 90 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  coerce_tmp_chpl3 = (&call_tmp_chpl6)->_instance;
#line 90 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  if (((RootClass_chpl)(coerce_tmp_chpl3)) == nil) /* ZLINE: 90 /Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl */
#line 90 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  {
#line 90 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
    nilCheckHalt_chpl(&_str_literal_52_chpl, INT64(90), INT32(38));
#line 90 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  }
#line 90 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  chpl_check_nil(coerce_tmp_chpl3, INT64(90), INT32(38));
#line 90 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  call_tmp_chpl11 = id_chpl(coerce_tmp_chpl3, INT64(90), INT32(38));
#line 90 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  if (call_tmp_chpl11 != INT64(0)) /* ZLINE: 90 /Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl */
#line 90 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  {
#line 91 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
    if (! ((&local_memLeaksByDesc_chpl)->buffLen == INT64(0))) /* ZLINE: 91 /Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl */
#line 91 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
    {
#line 92 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
      init_ASSIGN__chpl25(&local_memLeaksByDesc_chpl2, &local_memLeaksByDesc_chpl, INT64(92), INT32(38));
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
      (&local_memLeaksByDesc_chpl2)->isOwned = UINT8(false);
#line 95 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
      *(ret_memLeaksByDesc) = ((c_ptrConst_int8_t_chpl)((&local_memLeaksByDesc_chpl2)->buff));
#line 95 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
      i_x_chpl = &local_memLeaksByDesc_chpl2;
#line 95 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
      deinit_chpl95(i_x_chpl, INT64(95), INT32(38));
#line 95 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
    } else /* ZLINE: 96 /Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl */
#line 95 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
    {
#line 97 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
      *(ret_memLeaksByDesc) = ((c_ptrConst_int8_t_chpl)(nil));
#line 97 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
    }
#line 100 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
    if (! ((&local_memLog_chpl)->buffLen == INT64(0))) /* ZLINE: 100 /Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl */
#line 100 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
    {
#line 101 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
      init_ASSIGN__chpl25(&local_memLog_chpl2, &local_memLog_chpl, INT64(101), INT32(38));
#line 103 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
      (&local_memLog_chpl2)->isOwned = UINT8(false);
#line 104 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
      *(ret_memLog) = ((c_ptrConst_int8_t_chpl)((&local_memLog_chpl2)->buff));
#line 104 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
      i_x_chpl2 = &local_memLog_chpl2;
#line 104 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
      deinit_chpl95(i_x_chpl2, INT64(104), INT32(38));
#line 104 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
    } else /* ZLINE: 105 /Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl */
#line 104 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
    {
#line 106 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
      *(ret_memLog) = ((c_ptrConst_int8_t_chpl)(nil));
#line 106 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
    }
#line 109 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
    if (! ((&local_memLeaksLog_chpl)->buffLen == INT64(0))) /* ZLINE: 109 /Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl */
#line 109 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
    {
#line 110 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
      init_ASSIGN__chpl25(&local_memLeaksLog_chpl2, &local_memLeaksLog_chpl, INT64(110), INT32(38));
#line 112 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
      (&local_memLeaksLog_chpl2)->isOwned = UINT8(false);
#line 113 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
      *(ret_memLeaksLog) = ((c_ptrConst_int8_t_chpl)((&local_memLeaksLog_chpl2)->buff));
#line 113 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
      i_x_chpl3 = &local_memLeaksLog_chpl2;
#line 113 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
      deinit_chpl95(i_x_chpl3, INT64(113), INT32(38));
#line 113 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
    } else /* ZLINE: 114 /Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl */
#line 113 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
    {
#line 115 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
      *(ret_memLeaksLog) = ((c_ptrConst_int8_t_chpl)(nil));
#line 115 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
    }
#line 115 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  } else /* ZLINE: 118 /Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl */
#line 115 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  {
#line 119 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
    *(ret_memLeaksByDesc) = ((c_ptrConst_int8_t_chpl)((&local_memLeaksByDesc_chpl)->buff));
#line 120 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
    *(ret_memLog) = ((c_ptrConst_int8_t_chpl)((&local_memLog_chpl)->buff));
#line 121 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
    *(ret_memLeaksLog) = ((c_ptrConst_int8_t_chpl)((&local_memLeaksLog_chpl)->buff));
#line 121 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  }
#line 90 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  i_x_chpl4 = &call_tmp_chpl6;
#line 90 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  deinit_chpl62(i_x_chpl4);
#line 74 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
  return;
#line 74 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemTracking.chpl"
}

