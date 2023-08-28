#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
/* StringCasts.chpl:21 */
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
static void chpl__init_StringCasts(int64_t _ln_chpl,
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
                                   int32_t _fn_chpl) {
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _ref_int32_t_chpl refIndentLevel_chpl = NULL;
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  if (chpl__init_StringCasts_p) /* ZLINE: 21 /Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl */
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  {
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    goto _exit_chpl__init_StringCasts_chpl;
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  }
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  printModuleInit_chpl("%*s\n", "StringCasts", INT64(11), _ln_chpl, _fn_chpl);
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  refIndentLevel_chpl = &moduleInitLevel_chpl;
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  *(refIndentLevel_chpl) += INT64(1);
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  chpl__init_StringCasts_p = UINT8(true);
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  {
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    chpl__init_BytesStringCommon(_ln_chpl, _fn_chpl);
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    chpl__init_CTypes(_ln_chpl, _fn_chpl);
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    chpl__init_NVStringFactory(_ln_chpl, _fn_chpl);
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  }
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  *(refIndentLevel_chpl) -= INT64(1);
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _exit_chpl__init_StringCasts_chpl:;
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  return;
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
}

#line 45 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
/* StringCasts.chpl:45 */
#line 45 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
static chpl_bool chpl___COLON_19(string_chpl * x_chpl,
#line 45 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
                                 Error_chpl * error_out_chpl,
#line 45 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
                                 int64_t _ln_chpl,
#line 45 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
                                 int32_t _fn_chpl) {
#line 45 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  chpl_bool ret_chpl;
#line 46 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  string_chpl call_tmp_chpl6;
#line 46 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  string_chpl ret_tmp_chpl;
#line 46 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  string_chpl str_chpl;
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _owned_IllegalArgumentError_chpl call_tmp_chpl7;
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  IllegalArgumentError_chpl new_temp_chpl = NULL;
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _owned_IllegalArgumentError_chpl initTemp_chpl;
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  Error_chpl call_tmp_chpl8 = NULL;
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _owned_Error_chpl coerce_tmp_chpl3;
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _owned_Error_chpl ret_tmp_chpl2;
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _ref__owned_IllegalArgumentError_chpl2 i_x_chpl = NULL;
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _ref__owned_Error_chpl2 i__retArg_chpl = NULL;
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _owned_IllegalArgumentError_chpl _formal_tmp_x_chpl;
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _owned_Error_chpl ret_chpl2;
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _ref__owned_IllegalArgumentError_chpl2 i_this_chpl = NULL;
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  Error_chpl call_tmp_chpl9 = NULL;
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _ref__owned_IllegalArgumentError_chpl2 i_this_chpl2 = NULL;
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _owned_Error_chpl call_tmp_chpl10;
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _owned_Error_chpl initTemp_chpl2;
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _owned_Error_chpl _formal_tmp_in_err_chpl;
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _ref_string_chpl i_x_chpl2 = NULL;
#line 49 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  chpl_bool call_tmp_chpl11;
#line 50 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _ref_string_chpl i_x_chpl3 = NULL;
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  chpl_bool call_tmp_chpl12;
#line 52 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _ref_string_chpl i_x_chpl4 = NULL;
#line 54 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  string_chpl call_tmp_chpl13;
#line 54 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  string_chpl ret_tmp_chpl3;
#line 54 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  string_chpl call_tmp_chpl14;
#line 54 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  string_chpl ret_tmp_chpl4;
#line 54 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _owned_IllegalArgumentError_chpl call_tmp_chpl15;
#line 54 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  IllegalArgumentError_chpl new_temp_chpl2 = NULL;
#line 54 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _owned_IllegalArgumentError_chpl initTemp_chpl3;
#line 54 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  Error_chpl call_tmp_chpl16 = NULL;
#line 54 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _owned_Error_chpl coerce_tmp_chpl4;
#line 54 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _owned_Error_chpl ret_tmp_chpl5;
#line 54 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _ref__owned_IllegalArgumentError_chpl2 i_x_chpl5 = NULL;
#line 54 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _ref__owned_Error_chpl2 i__retArg_chpl2 = NULL;
#line 54 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _owned_IllegalArgumentError_chpl _formal_tmp_x_chpl2;
#line 54 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _owned_Error_chpl ret_chpl3;
#line 54 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _ref__owned_IllegalArgumentError_chpl2 i_this_chpl3 = NULL;
#line 54 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  Error_chpl call_tmp_chpl17 = NULL;
#line 54 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _ref__owned_IllegalArgumentError_chpl2 i_this_chpl4 = NULL;
#line 54 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _owned_Error_chpl call_tmp_chpl18;
#line 54 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _owned_Error_chpl initTemp_chpl4;
#line 54 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _owned_Error_chpl _formal_tmp_in_err_chpl2;
#line 54 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _ref_string_chpl i_x_chpl6 = NULL;
#line 54 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _ref_string_chpl i_x_chpl7 = NULL;
#line 54 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _ref_string_chpl i_x_chpl8 = NULL;
#line 46 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  strip_chpl(x_chpl, &_str_literal_268_chpl, UINT8(true), UINT8(true), &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 46 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  call_tmp_chpl6 = ret_tmp_chpl;
#line 46 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  str_chpl = call_tmp_chpl6;
#line 47 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  if ((&str_chpl)->buffLen == INT64(0)) /* ZLINE: 47 /Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl */
#line 47 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  {
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    new_temp_chpl = _new_chpl59(&_str_literal_1378_chpl, _ln_chpl, _fn_chpl);
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    init_chpl179(&initTemp_chpl, new_temp_chpl);
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    call_tmp_chpl7 = initTemp_chpl;
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    i_x_chpl = &call_tmp_chpl7;
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    i__retArg_chpl = &ret_tmp_chpl2;
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    _formal_tmp_x_chpl = *(i_x_chpl);
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    i_this_chpl = &_formal_tmp_x_chpl;
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    call_tmp_chpl9 = ((Error_chpl)((i_this_chpl)->chpl_p));
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    i_this_chpl2 = &_formal_tmp_x_chpl;
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    (i_this_chpl2)->chpl_p = ((IllegalArgumentError_chpl)(nil));
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    if (((RootClass_chpl)(call_tmp_chpl9)) == nil) /* ZLINE: 48 /Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl */
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    {
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
      nilCheckHalt_chpl(&_str_literal_52_chpl, _ln_chpl, _fn_chpl);
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    }
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    init_chpl188(&initTemp_chpl2, call_tmp_chpl9);
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    call_tmp_chpl10 = initTemp_chpl2;
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    ret_chpl2 = call_tmp_chpl10;
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    chpl__autoDestroy59(&_formal_tmp_x_chpl, _ln_chpl, _fn_chpl);
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    *(i__retArg_chpl) = ret_chpl2;
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    coerce_tmp_chpl3 = ret_tmp_chpl2;
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    _formal_tmp_in_err_chpl = coerce_tmp_chpl3;
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    call_tmp_chpl8 = chpl_fix_thrown_error(&_formal_tmp_in_err_chpl, _ln_chpl, _fn_chpl);
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    *(error_out_chpl) = call_tmp_chpl8;
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    i_x_chpl2 = &str_chpl;
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    deinit_chpl95(i_x_chpl2, _ln_chpl, _fn_chpl);
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    goto _end__COLON__chpl;
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  } else /* ZLINE: 47 /Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl */
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  {
#line 49 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    call_tmp_chpl11 = chpl___EQUALS_3(&str_chpl, &_str_literal_422_chpl, _ln_chpl, _fn_chpl);
#line 49 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    if (call_tmp_chpl11) /* ZLINE: 49 /Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl */
#line 49 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    {
#line 50 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
      ret_chpl = UINT8(true);
#line 50 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
      i_x_chpl3 = &str_chpl;
#line 50 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
      deinit_chpl95(i_x_chpl3, _ln_chpl, _fn_chpl);
#line 50 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
      goto _end__COLON__chpl;
#line 50 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    } else /* ZLINE: 49 /Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl */
#line 50 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    {
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
      call_tmp_chpl12 = chpl___EQUALS_3(&str_chpl, &_str_literal_1377_chpl, _ln_chpl, _fn_chpl);
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
      if (call_tmp_chpl12) /* ZLINE: 51 /Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl */
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
      {
#line 52 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
        ret_chpl = UINT8(false);
#line 52 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
        i_x_chpl4 = &str_chpl;
#line 52 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
        deinit_chpl95(i_x_chpl4, _ln_chpl, _fn_chpl);
#line 52 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
        goto _end__COLON__chpl;
#line 52 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
      } else /* ZLINE: 53 /Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl */
#line 52 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
      {
#line 54 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
        chpl___PLUS_(&_str_literal_184_chpl, x_chpl, &ret_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 54 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
        call_tmp_chpl13 = ret_tmp_chpl3;
#line 54 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
        chpl___PLUS_(&call_tmp_chpl13, &_str_literal_1361_chpl, &ret_tmp_chpl4, _ln_chpl, _fn_chpl);
#line 54 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
        call_tmp_chpl14 = ret_tmp_chpl4;
#line 54 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
        new_temp_chpl2 = _new_chpl59(&call_tmp_chpl14, _ln_chpl, _fn_chpl);
#line 54 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
        init_chpl179(&initTemp_chpl3, new_temp_chpl2);
#line 54 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
        call_tmp_chpl15 = initTemp_chpl3;
#line 54 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
        i_x_chpl5 = &call_tmp_chpl15;
#line 54 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
        i__retArg_chpl2 = &ret_tmp_chpl5;
#line 54 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
        _formal_tmp_x_chpl2 = *(i_x_chpl5);
#line 54 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
        i_this_chpl3 = &_formal_tmp_x_chpl2;
#line 54 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
        call_tmp_chpl17 = ((Error_chpl)((i_this_chpl3)->chpl_p));
#line 54 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
        i_this_chpl4 = &_formal_tmp_x_chpl2;
#line 54 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
        (i_this_chpl4)->chpl_p = ((IllegalArgumentError_chpl)(nil));
#line 54 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
        if (((RootClass_chpl)(call_tmp_chpl17)) == nil) /* ZLINE: 54 /Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl */
#line 54 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
        {
#line 54 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
          nilCheckHalt_chpl(&_str_literal_52_chpl, _ln_chpl, _fn_chpl);
#line 54 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
        }
#line 54 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
        init_chpl188(&initTemp_chpl4, call_tmp_chpl17);
#line 54 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
        call_tmp_chpl18 = initTemp_chpl4;
#line 54 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
        ret_chpl3 = call_tmp_chpl18;
#line 54 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
        chpl__autoDestroy59(&_formal_tmp_x_chpl2, _ln_chpl, _fn_chpl);
#line 54 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
        *(i__retArg_chpl2) = ret_chpl3;
#line 54 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
        coerce_tmp_chpl4 = ret_tmp_chpl5;
#line 54 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
        _formal_tmp_in_err_chpl2 = coerce_tmp_chpl4;
#line 54 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
        call_tmp_chpl16 = chpl_fix_thrown_error(&_formal_tmp_in_err_chpl2, _ln_chpl, _fn_chpl);
#line 54 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
        *(error_out_chpl) = call_tmp_chpl16;
#line 54 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
        i_x_chpl6 = &call_tmp_chpl14;
#line 54 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
        deinit_chpl95(i_x_chpl6, _ln_chpl, _fn_chpl);
#line 54 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
        i_x_chpl7 = &call_tmp_chpl13;
#line 54 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
        deinit_chpl95(i_x_chpl7, _ln_chpl, _fn_chpl);
#line 54 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
        i_x_chpl8 = &str_chpl;
#line 54 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
        deinit_chpl95(i_x_chpl8, _ln_chpl, _fn_chpl);
#line 54 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
        goto _end__COLON__chpl;
#line 54 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
      }
#line 54 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    }
#line 54 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  }
#line 54 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _end__COLON__chpl:;
#line 45 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  return ret_chpl;
#line 45 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
}

#line 60 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
/* StringCasts.chpl:60 */
#line 60 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
static void chpl___COLON_20(_tuple_4_star_int64_t_chpl * x_chpl,
#line 60 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
                            string_chpl * _retArg_chpl,
#line 60 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
                            int64_t _ln_chpl,
#line 60 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
                            int32_t _fn_chpl) {
#line 60 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  string_chpl ret_chpl;
#line 61 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  string_chpl ret_chpl2;
#line 61 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  string_chpl inlineImm_chpl;
#line 61 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 61 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  string_chpl ret_chpl3;
#line 61 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  string_chpl tmp_chpl;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  string_chpl call_tmp_chpl6;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  string_chpl ret_tmp_chpl;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _ref_string_chpl i_x_chpl2 = NULL;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  string_chpl call_tmp_chpl7;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  string_chpl ret_tmp_chpl2;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _ref_string_chpl i_x_chpl3 = NULL;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  string_chpl call_tmp_chpl8;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  string_chpl ret_tmp_chpl3;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _ref_string_chpl i_x_chpl4 = NULL;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  string_chpl call_tmp_chpl9;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  string_chpl ret_tmp_chpl4;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _ref_string_chpl i_x_chpl5 = NULL;
#line 61 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  inlineImm_chpl = _str_literal_1379_chpl;
#line 61 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  i_x_chpl = &inlineImm_chpl;
#line 61 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  init_ASSIGN__chpl25(&tmp_chpl, i_x_chpl, _ln_chpl, _fn_chpl);
#line 61 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  ret_chpl3 = tmp_chpl;
#line 61 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  ret_chpl2 = ret_chpl3;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  chpl___COLON_25(*(*(x_chpl) + INT64(0)), &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  call_tmp_chpl6 = ret_tmp_chpl;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  chpl___PLUS__ASSIGN_(&ret_chpl2, &call_tmp_chpl6, _ln_chpl, _fn_chpl);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  i_x_chpl2 = &call_tmp_chpl6;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  deinit_chpl95(i_x_chpl2, _ln_chpl, _fn_chpl);
#line 63 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  chpl___PLUS__ASSIGN_(&ret_chpl2, &_str_literal_825_chpl, _ln_chpl, _fn_chpl);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  chpl___COLON_25(*(*(x_chpl) + INT64(1)), &ret_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  call_tmp_chpl7 = ret_tmp_chpl2;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  chpl___PLUS__ASSIGN_(&ret_chpl2, &call_tmp_chpl7, _ln_chpl, _fn_chpl);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  i_x_chpl3 = &call_tmp_chpl7;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  deinit_chpl95(i_x_chpl3, _ln_chpl, _fn_chpl);
#line 63 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  chpl___PLUS__ASSIGN_(&ret_chpl2, &_str_literal_825_chpl, _ln_chpl, _fn_chpl);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  chpl___COLON_25(*(*(x_chpl) + INT64(2)), &ret_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  call_tmp_chpl8 = ret_tmp_chpl3;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  chpl___PLUS__ASSIGN_(&ret_chpl2, &call_tmp_chpl8, _ln_chpl, _fn_chpl);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  i_x_chpl4 = &call_tmp_chpl8;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  deinit_chpl95(i_x_chpl4, _ln_chpl, _fn_chpl);
#line 63 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  chpl___PLUS__ASSIGN_(&ret_chpl2, &_str_literal_825_chpl, _ln_chpl, _fn_chpl);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  chpl___COLON_25(*(*(x_chpl) + INT64(3)), &ret_tmp_chpl4, _ln_chpl, _fn_chpl);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  call_tmp_chpl9 = ret_tmp_chpl4;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  chpl___PLUS__ASSIGN_(&ret_chpl2, &call_tmp_chpl9, _ln_chpl, _fn_chpl);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  i_x_chpl5 = &call_tmp_chpl9;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  deinit_chpl95(i_x_chpl5, _ln_chpl, _fn_chpl);
#line 67 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  chpl___PLUS__ASSIGN_(&ret_chpl2, &_str_literal_415_chpl, _ln_chpl, _fn_chpl);
#line 68 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  ret_chpl = ret_chpl2;
#line 60 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  *(_retArg_chpl) = ret_chpl;
#line 60 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  return;
#line 60 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
}

#line 60 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
/* StringCasts.chpl:60 */
#line 60 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
static void chpl___COLON_22(_tuple_2_star_int64_t_chpl * x_chpl,
#line 60 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
                            string_chpl * _retArg_chpl,
#line 60 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
                            int64_t _ln_chpl,
#line 60 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
                            int32_t _fn_chpl) {
#line 60 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  string_chpl ret_chpl;
#line 61 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  string_chpl ret_chpl2;
#line 61 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  string_chpl inlineImm_chpl;
#line 61 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 61 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  string_chpl ret_chpl3;
#line 61 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  string_chpl tmp_chpl;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  string_chpl call_tmp_chpl6;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  string_chpl ret_tmp_chpl;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _ref_string_chpl i_x_chpl2 = NULL;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  string_chpl call_tmp_chpl7;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  string_chpl ret_tmp_chpl2;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _ref_string_chpl i_x_chpl3 = NULL;
#line 61 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  inlineImm_chpl = _str_literal_1379_chpl;
#line 61 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  i_x_chpl = &inlineImm_chpl;
#line 61 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  init_ASSIGN__chpl25(&tmp_chpl, i_x_chpl, _ln_chpl, _fn_chpl);
#line 61 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  ret_chpl3 = tmp_chpl;
#line 61 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  ret_chpl2 = ret_chpl3;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  chpl___COLON_25(*(*(x_chpl) + INT64(0)), &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  call_tmp_chpl6 = ret_tmp_chpl;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  chpl___PLUS__ASSIGN_(&ret_chpl2, &call_tmp_chpl6, _ln_chpl, _fn_chpl);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  i_x_chpl2 = &call_tmp_chpl6;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  deinit_chpl95(i_x_chpl2, _ln_chpl, _fn_chpl);
#line 63 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  chpl___PLUS__ASSIGN_(&ret_chpl2, &_str_literal_825_chpl, _ln_chpl, _fn_chpl);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  chpl___COLON_25(*(*(x_chpl) + INT64(1)), &ret_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  call_tmp_chpl7 = ret_tmp_chpl2;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  chpl___PLUS__ASSIGN_(&ret_chpl2, &call_tmp_chpl7, _ln_chpl, _fn_chpl);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  i_x_chpl3 = &call_tmp_chpl7;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  deinit_chpl95(i_x_chpl3, _ln_chpl, _fn_chpl);
#line 67 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  chpl___PLUS__ASSIGN_(&ret_chpl2, &_str_literal_415_chpl, _ln_chpl, _fn_chpl);
#line 68 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  ret_chpl = ret_chpl2;
#line 60 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  *(_retArg_chpl) = ret_chpl;
#line 60 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  return;
#line 60 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
}

#line 60 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
/* StringCasts.chpl:60 */
#line 60 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
static void chpl___COLON_21(_tuple_1_star_int64_t_chpl * x_chpl,
#line 60 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
                            string_chpl * _retArg_chpl,
#line 60 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
                            int64_t _ln_chpl,
#line 60 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
                            int32_t _fn_chpl) {
#line 60 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  string_chpl ret_chpl;
#line 61 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  string_chpl ret_chpl2;
#line 61 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  string_chpl inlineImm_chpl;
#line 61 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 61 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  string_chpl ret_chpl3;
#line 61 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  string_chpl tmp_chpl;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  string_chpl call_tmp_chpl6;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  string_chpl ret_tmp_chpl;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _ref_string_chpl i_x_chpl2 = NULL;
#line 61 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  inlineImm_chpl = _str_literal_1379_chpl;
#line 61 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  i_x_chpl = &inlineImm_chpl;
#line 61 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  init_ASSIGN__chpl25(&tmp_chpl, i_x_chpl, _ln_chpl, _fn_chpl);
#line 61 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  ret_chpl3 = tmp_chpl;
#line 61 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  ret_chpl2 = ret_chpl3;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  chpl___COLON_25(*(*(x_chpl) + INT64(0)), &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  call_tmp_chpl6 = ret_tmp_chpl;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  chpl___PLUS__ASSIGN_(&ret_chpl2, &call_tmp_chpl6, _ln_chpl, _fn_chpl);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  i_x_chpl2 = &call_tmp_chpl6;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  deinit_chpl95(i_x_chpl2, _ln_chpl, _fn_chpl);
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  chpl___PLUS__ASSIGN_(&ret_chpl2, &_str_literal_472_chpl, _ln_chpl, _fn_chpl);
#line 67 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  chpl___PLUS__ASSIGN_(&ret_chpl2, &_str_literal_415_chpl, _ln_chpl, _fn_chpl);
#line 68 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  ret_chpl = ret_chpl2;
#line 60 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  *(_retArg_chpl) = ret_chpl;
#line 60 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  return;
#line 60 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
}

#line 74 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
/* StringCasts.chpl:74 */
#line 74 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
static void chpl___COLON_25(int64_t x_chpl,
#line 74 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
                            string_chpl * _retArg_chpl,
#line 74 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
                            int64_t _ln_chpl,
#line 74 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
                            int32_t _fn_chpl) {
#line 81 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  chpl_bool isErr_chpl;
#line 82 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  c_ptrConst_int8_t_chpl call_tmp_chpl6 = NULL;
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _owned_IllegalArgumentError_chpl call_tmp_chpl7;
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  IllegalArgumentError_chpl new_temp_chpl = NULL;
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _owned_IllegalArgumentError_chpl initTemp_chpl;
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  Error_chpl call_tmp_chpl8 = NULL;
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _owned_Error_chpl coerce_tmp_chpl3;
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _owned_Error_chpl ret_tmp_chpl;
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _ref__owned_IllegalArgumentError_chpl2 i_x_chpl = NULL;
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _ref__owned_Error_chpl2 i__retArg_chpl = NULL;
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _owned_IllegalArgumentError_chpl _formal_tmp_x_chpl;
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _owned_Error_chpl ret_chpl;
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _ref__owned_IllegalArgumentError_chpl2 i_this_chpl = NULL;
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  Error_chpl call_tmp_chpl9 = NULL;
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _ref__owned_IllegalArgumentError_chpl2 i_this_chpl2 = NULL;
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _owned_Error_chpl call_tmp_chpl10;
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _owned_Error_chpl initTemp_chpl2;
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _owned_Error_chpl _formal_tmp_in_err_chpl;
#line 91 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  uint64_t call_tmp_chpl11;
#line 91 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _owned_IllegalArgumentError_chpl2 call_tmp_chpl12;
#line 91 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _owned_IllegalArgumentError_chpl2 ret_tmp_chpl2;
#line 91 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _ref__owned_IllegalArgumentError_chpl i_arg_chpl = NULL;
#line 91 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  IllegalArgumentError_chpl call_tmp_chpl13 = NULL;
#line 91 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  string_chpl call_tmp_chpl14;
#line 91 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  Error_chpl coerce_tmp_chpl4 = NULL;
#line 91 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  string_chpl ret_tmp_chpl3;
#line 91 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _ref_string_chpl i_x_chpl2 = NULL;
#line 91 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  int64_t call_tmp_chpl15;
#line 92 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  c_ptr_uint8_t_chpl call_tmp_chpl16 = NULL;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  int64_t call_tmp_chpl17;
#line 92 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  string_chpl ret_chpl2;
#line 92 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  string_chpl ret_chpl3;
#line 92 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _ref_string_chpl i_x_chpl3 = NULL;
#line 81 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  isErr_chpl = UINT8(false);
#line 82 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  call_tmp_chpl6 = integral_to_c_string(x_chpl, ((uint32_t)(INT64(8))), UINT8(true), &isErr_chpl);
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  if (isErr_chpl) /* ZLINE: 85 /Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl */
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  {
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    new_temp_chpl = _new_chpl59(&_str_literal_1362_chpl, _ln_chpl, _fn_chpl);
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    init_chpl179(&initTemp_chpl, new_temp_chpl);
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    call_tmp_chpl7 = initTemp_chpl;
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    i_x_chpl = &call_tmp_chpl7;
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    i__retArg_chpl = &ret_tmp_chpl;
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    _formal_tmp_x_chpl = *(i_x_chpl);
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    i_this_chpl = &_formal_tmp_x_chpl;
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    call_tmp_chpl9 = ((Error_chpl)((i_this_chpl)->chpl_p));
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    i_this_chpl2 = &_formal_tmp_x_chpl;
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    (i_this_chpl2)->chpl_p = ((IllegalArgumentError_chpl)(nil));
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    if (((RootClass_chpl)(call_tmp_chpl9)) == nil) /* ZLINE: 87 /Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl */
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    {
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
      nilCheckHalt_chpl(&_str_literal_52_chpl, _ln_chpl, _fn_chpl);
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    }
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    init_chpl188(&initTemp_chpl2, call_tmp_chpl9);
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    call_tmp_chpl10 = initTemp_chpl2;
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    ret_chpl = call_tmp_chpl10;
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    chpl__autoDestroy59(&_formal_tmp_x_chpl, _ln_chpl, _fn_chpl);
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    *(i__retArg_chpl) = ret_chpl;
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    coerce_tmp_chpl3 = ret_tmp_chpl;
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    _formal_tmp_in_err_chpl = coerce_tmp_chpl3;
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    call_tmp_chpl8 = chpl_fix_thrown_error(&_formal_tmp_in_err_chpl, _ln_chpl, _fn_chpl);
#line 86 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    if (call_tmp_chpl8 != nil) /* ZLINE: 86 /Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl */
#line 86 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    {
#line 86 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
      chpl_uncaught_error(call_tmp_chpl8, _ln_chpl, _fn_chpl);
#line 86 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    }
#line 86 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  }
#line 91 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  call_tmp_chpl11 = strlen(call_tmp_chpl6);
#line 91 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  chpl_checkValue3(call_tmp_chpl11, &ret_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 91 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  call_tmp_chpl12 = ret_tmp_chpl2;
#line 91 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  i_arg_chpl = &call_tmp_chpl12;
#line 91 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  call_tmp_chpl13 = borrow_chpl2(i_arg_chpl);
#line 91 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  if (((RootClass_chpl)(call_tmp_chpl13)) != nil) /* ZLINE: 91 /Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl */
#line 91 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  {
#line 91 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    coerce_tmp_chpl4 = ((Error_chpl)(call_tmp_chpl13));
#line 91 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    chpl_check_nil(coerce_tmp_chpl4, _ln_chpl, _fn_chpl);
#line 91 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    ((void(*)(Error_chpl,
#line 91 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
       string_chpl *,
#line 91 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
       int64_t,
#line 91 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
       int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(coerce_tmp_chpl4))->chpl__cid) + INT64(1))])(coerce_tmp_chpl4, &ret_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 91 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    call_tmp_chpl14 = ret_tmp_chpl3;
#line 91 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    safeCastCheckHalt_chpl(&call_tmp_chpl14, _ln_chpl, _fn_chpl);
#line 91 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    i_x_chpl2 = &call_tmp_chpl14;
#line 91 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    deinit_chpl95(i_x_chpl2, _ln_chpl, _fn_chpl);
#line 91 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  }
#line 91 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  chpl__autoDestroy62(&call_tmp_chpl12, _ln_chpl, _fn_chpl);
#line 91 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  call_tmp_chpl15 = ((int64_t)(call_tmp_chpl11));
#line 92 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  call_tmp_chpl16 = ((c_ptr_uint8_t_chpl)(call_tmp_chpl6));
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  call_tmp_chpl17 = (call_tmp_chpl15 + INT64(1));
#line 92 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  init_chpl194(&ret_chpl3, _ln_chpl, _fn_chpl);
#line 92 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  i_x_chpl3 = &ret_chpl3;
#line 92 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  (i_x_chpl3)->isOwned = UINT8(true);
#line 92 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  (i_x_chpl3)->buff = call_tmp_chpl16;
#line 92 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  (i_x_chpl3)->buffSize = call_tmp_chpl17;
#line 92 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  (i_x_chpl3)->buffLen = call_tmp_chpl15;
#line 92 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  (&ret_chpl3)->cachedNumCodepoints = call_tmp_chpl15;
#line 92 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  ret_chpl2 = ret_chpl3;
#line 74 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  *(_retArg_chpl) = ret_chpl2;
#line 74 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  return;
#line 74 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
}

#line 74 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
/* StringCasts.chpl:74 */
#line 74 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
static void chpl___COLON_24(uint64_t x_chpl,
#line 74 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
                            string_chpl * _retArg_chpl,
#line 74 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
                            int64_t _ln_chpl,
#line 74 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
                            int32_t _fn_chpl) {
#line 81 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  chpl_bool isErr_chpl;
#line 82 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  c_ptrConst_int8_t_chpl call_tmp_chpl6 = NULL;
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _owned_IllegalArgumentError_chpl call_tmp_chpl7;
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  IllegalArgumentError_chpl new_temp_chpl = NULL;
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _owned_IllegalArgumentError_chpl initTemp_chpl;
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  Error_chpl call_tmp_chpl8 = NULL;
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _owned_Error_chpl coerce_tmp_chpl3;
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _owned_Error_chpl ret_tmp_chpl;
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _ref__owned_IllegalArgumentError_chpl2 i_x_chpl = NULL;
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _ref__owned_Error_chpl2 i__retArg_chpl = NULL;
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _owned_IllegalArgumentError_chpl _formal_tmp_x_chpl;
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _owned_Error_chpl ret_chpl;
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _ref__owned_IllegalArgumentError_chpl2 i_this_chpl = NULL;
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  Error_chpl call_tmp_chpl9 = NULL;
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _ref__owned_IllegalArgumentError_chpl2 i_this_chpl2 = NULL;
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _owned_Error_chpl call_tmp_chpl10;
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _owned_Error_chpl initTemp_chpl2;
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _owned_Error_chpl _formal_tmp_in_err_chpl;
#line 91 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  uint64_t call_tmp_chpl11;
#line 91 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _owned_IllegalArgumentError_chpl2 call_tmp_chpl12;
#line 91 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _owned_IllegalArgumentError_chpl2 ret_tmp_chpl2;
#line 91 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _ref__owned_IllegalArgumentError_chpl i_arg_chpl = NULL;
#line 91 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  IllegalArgumentError_chpl call_tmp_chpl13 = NULL;
#line 91 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  string_chpl call_tmp_chpl14;
#line 91 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  Error_chpl coerce_tmp_chpl4 = NULL;
#line 91 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  string_chpl ret_tmp_chpl3;
#line 91 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _ref_string_chpl i_x_chpl2 = NULL;
#line 91 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  int64_t call_tmp_chpl15;
#line 92 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  c_ptr_uint8_t_chpl call_tmp_chpl16 = NULL;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  int64_t call_tmp_chpl17;
#line 92 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  string_chpl ret_chpl2;
#line 92 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  string_chpl ret_chpl3;
#line 92 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _ref_string_chpl i_x_chpl3 = NULL;
#line 81 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  isErr_chpl = UINT8(false);
#line 82 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  call_tmp_chpl6 = integral_to_c_string(((int64_t)(x_chpl)), ((uint32_t)(INT64(8))), UINT8(false), &isErr_chpl);
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  if (isErr_chpl) /* ZLINE: 85 /Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl */
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  {
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    new_temp_chpl = _new_chpl59(&_str_literal_1362_chpl, _ln_chpl, _fn_chpl);
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    init_chpl179(&initTemp_chpl, new_temp_chpl);
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    call_tmp_chpl7 = initTemp_chpl;
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    i_x_chpl = &call_tmp_chpl7;
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    i__retArg_chpl = &ret_tmp_chpl;
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    _formal_tmp_x_chpl = *(i_x_chpl);
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    i_this_chpl = &_formal_tmp_x_chpl;
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    call_tmp_chpl9 = ((Error_chpl)((i_this_chpl)->chpl_p));
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    i_this_chpl2 = &_formal_tmp_x_chpl;
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    (i_this_chpl2)->chpl_p = ((IllegalArgumentError_chpl)(nil));
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    if (((RootClass_chpl)(call_tmp_chpl9)) == nil) /* ZLINE: 87 /Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl */
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    {
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
      nilCheckHalt_chpl(&_str_literal_52_chpl, _ln_chpl, _fn_chpl);
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    }
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    init_chpl188(&initTemp_chpl2, call_tmp_chpl9);
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    call_tmp_chpl10 = initTemp_chpl2;
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    ret_chpl = call_tmp_chpl10;
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    chpl__autoDestroy59(&_formal_tmp_x_chpl, _ln_chpl, _fn_chpl);
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    *(i__retArg_chpl) = ret_chpl;
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    coerce_tmp_chpl3 = ret_tmp_chpl;
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    _formal_tmp_in_err_chpl = coerce_tmp_chpl3;
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    call_tmp_chpl8 = chpl_fix_thrown_error(&_formal_tmp_in_err_chpl, _ln_chpl, _fn_chpl);
#line 86 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    if (call_tmp_chpl8 != nil) /* ZLINE: 86 /Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl */
#line 86 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    {
#line 86 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
      chpl_uncaught_error(call_tmp_chpl8, _ln_chpl, _fn_chpl);
#line 86 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    }
#line 86 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  }
#line 91 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  call_tmp_chpl11 = strlen(call_tmp_chpl6);
#line 91 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  chpl_checkValue3(call_tmp_chpl11, &ret_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 91 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  call_tmp_chpl12 = ret_tmp_chpl2;
#line 91 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  i_arg_chpl = &call_tmp_chpl12;
#line 91 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  call_tmp_chpl13 = borrow_chpl2(i_arg_chpl);
#line 91 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  if (((RootClass_chpl)(call_tmp_chpl13)) != nil) /* ZLINE: 91 /Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl */
#line 91 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  {
#line 91 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    coerce_tmp_chpl4 = ((Error_chpl)(call_tmp_chpl13));
#line 91 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    chpl_check_nil(coerce_tmp_chpl4, _ln_chpl, _fn_chpl);
#line 91 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    ((void(*)(Error_chpl,
#line 91 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
       string_chpl *,
#line 91 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
       int64_t,
#line 91 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
       int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(coerce_tmp_chpl4))->chpl__cid) + INT64(1))])(coerce_tmp_chpl4, &ret_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 91 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    call_tmp_chpl14 = ret_tmp_chpl3;
#line 91 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    safeCastCheckHalt_chpl(&call_tmp_chpl14, _ln_chpl, _fn_chpl);
#line 91 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    i_x_chpl2 = &call_tmp_chpl14;
#line 91 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    deinit_chpl95(i_x_chpl2, _ln_chpl, _fn_chpl);
#line 91 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  }
#line 91 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  chpl__autoDestroy62(&call_tmp_chpl12, _ln_chpl, _fn_chpl);
#line 91 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  call_tmp_chpl15 = ((int64_t)(call_tmp_chpl11));
#line 92 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  call_tmp_chpl16 = ((c_ptr_uint8_t_chpl)(call_tmp_chpl6));
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  call_tmp_chpl17 = (call_tmp_chpl15 + INT64(1));
#line 92 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  init_chpl194(&ret_chpl3, _ln_chpl, _fn_chpl);
#line 92 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  i_x_chpl3 = &ret_chpl3;
#line 92 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  (i_x_chpl3)->isOwned = UINT8(true);
#line 92 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  (i_x_chpl3)->buff = call_tmp_chpl16;
#line 92 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  (i_x_chpl3)->buffSize = call_tmp_chpl17;
#line 92 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  (i_x_chpl3)->buffLen = call_tmp_chpl15;
#line 92 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  (&ret_chpl3)->cachedNumCodepoints = call_tmp_chpl15;
#line 92 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  ret_chpl2 = ret_chpl3;
#line 74 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  *(_retArg_chpl) = ret_chpl2;
#line 74 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  return;
#line 74 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
}

#line 74 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
/* StringCasts.chpl:74 */
#line 74 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
static void chpl___COLON_23(uint8_t x_chpl,
#line 74 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
                            string_chpl * _retArg_chpl,
#line 74 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
                            int64_t _ln_chpl,
#line 74 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
                            int32_t _fn_chpl) {
#line 81 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  chpl_bool isErr_chpl;
#line 82 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  c_ptrConst_int8_t_chpl call_tmp_chpl6 = NULL;
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _owned_IllegalArgumentError_chpl call_tmp_chpl7;
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  IllegalArgumentError_chpl new_temp_chpl = NULL;
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _owned_IllegalArgumentError_chpl initTemp_chpl;
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  Error_chpl call_tmp_chpl8 = NULL;
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _owned_Error_chpl coerce_tmp_chpl3;
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _owned_Error_chpl ret_tmp_chpl;
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _ref__owned_IllegalArgumentError_chpl2 i_x_chpl = NULL;
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _ref__owned_Error_chpl2 i__retArg_chpl = NULL;
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _owned_IllegalArgumentError_chpl _formal_tmp_x_chpl;
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _owned_Error_chpl ret_chpl;
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _ref__owned_IllegalArgumentError_chpl2 i_this_chpl = NULL;
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  Error_chpl call_tmp_chpl9 = NULL;
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _ref__owned_IllegalArgumentError_chpl2 i_this_chpl2 = NULL;
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _owned_Error_chpl call_tmp_chpl10;
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _owned_Error_chpl initTemp_chpl2;
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _owned_Error_chpl _formal_tmp_in_err_chpl;
#line 91 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  uint64_t call_tmp_chpl11;
#line 91 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _owned_IllegalArgumentError_chpl2 call_tmp_chpl12;
#line 91 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _owned_IllegalArgumentError_chpl2 ret_tmp_chpl2;
#line 91 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _ref__owned_IllegalArgumentError_chpl i_arg_chpl = NULL;
#line 91 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  IllegalArgumentError_chpl call_tmp_chpl13 = NULL;
#line 91 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  string_chpl call_tmp_chpl14;
#line 91 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  Error_chpl coerce_tmp_chpl4 = NULL;
#line 91 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  string_chpl ret_tmp_chpl3;
#line 91 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _ref_string_chpl i_x_chpl2 = NULL;
#line 91 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  int64_t call_tmp_chpl15;
#line 92 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  c_ptr_uint8_t_chpl call_tmp_chpl16 = NULL;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  int64_t call_tmp_chpl17;
#line 92 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  string_chpl ret_chpl2;
#line 92 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  string_chpl ret_chpl3;
#line 92 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _ref_string_chpl i_x_chpl3 = NULL;
#line 81 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  isErr_chpl = UINT8(false);
#line 82 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  call_tmp_chpl6 = integral_to_c_string(((int64_t)(x_chpl)), ((uint32_t)(INT64(1))), UINT8(false), &isErr_chpl);
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  if (isErr_chpl) /* ZLINE: 85 /Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl */
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  {
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    new_temp_chpl = _new_chpl59(&_str_literal_1362_chpl, _ln_chpl, _fn_chpl);
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    init_chpl179(&initTemp_chpl, new_temp_chpl);
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    call_tmp_chpl7 = initTemp_chpl;
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    i_x_chpl = &call_tmp_chpl7;
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    i__retArg_chpl = &ret_tmp_chpl;
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    _formal_tmp_x_chpl = *(i_x_chpl);
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    i_this_chpl = &_formal_tmp_x_chpl;
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    call_tmp_chpl9 = ((Error_chpl)((i_this_chpl)->chpl_p));
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    i_this_chpl2 = &_formal_tmp_x_chpl;
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    (i_this_chpl2)->chpl_p = ((IllegalArgumentError_chpl)(nil));
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    if (((RootClass_chpl)(call_tmp_chpl9)) == nil) /* ZLINE: 87 /Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl */
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    {
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
      nilCheckHalt_chpl(&_str_literal_52_chpl, _ln_chpl, _fn_chpl);
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    }
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    init_chpl188(&initTemp_chpl2, call_tmp_chpl9);
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    call_tmp_chpl10 = initTemp_chpl2;
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    ret_chpl = call_tmp_chpl10;
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    chpl__autoDestroy59(&_formal_tmp_x_chpl, _ln_chpl, _fn_chpl);
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    *(i__retArg_chpl) = ret_chpl;
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    coerce_tmp_chpl3 = ret_tmp_chpl;
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    _formal_tmp_in_err_chpl = coerce_tmp_chpl3;
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    call_tmp_chpl8 = chpl_fix_thrown_error(&_formal_tmp_in_err_chpl, _ln_chpl, _fn_chpl);
#line 86 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    if (call_tmp_chpl8 != nil) /* ZLINE: 86 /Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl */
#line 86 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    {
#line 86 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
      chpl_uncaught_error(call_tmp_chpl8, _ln_chpl, _fn_chpl);
#line 86 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    }
#line 86 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  }
#line 91 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  call_tmp_chpl11 = strlen(call_tmp_chpl6);
#line 91 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  chpl_checkValue3(call_tmp_chpl11, &ret_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 91 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  call_tmp_chpl12 = ret_tmp_chpl2;
#line 91 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  i_arg_chpl = &call_tmp_chpl12;
#line 91 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  call_tmp_chpl13 = borrow_chpl2(i_arg_chpl);
#line 91 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  if (((RootClass_chpl)(call_tmp_chpl13)) != nil) /* ZLINE: 91 /Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl */
#line 91 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  {
#line 91 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    coerce_tmp_chpl4 = ((Error_chpl)(call_tmp_chpl13));
#line 91 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    chpl_check_nil(coerce_tmp_chpl4, _ln_chpl, _fn_chpl);
#line 91 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    ((void(*)(Error_chpl,
#line 91 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
       string_chpl *,
#line 91 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
       int64_t,
#line 91 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
       int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(coerce_tmp_chpl4))->chpl__cid) + INT64(1))])(coerce_tmp_chpl4, &ret_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 91 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    call_tmp_chpl14 = ret_tmp_chpl3;
#line 91 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    safeCastCheckHalt_chpl(&call_tmp_chpl14, _ln_chpl, _fn_chpl);
#line 91 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    i_x_chpl2 = &call_tmp_chpl14;
#line 91 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    deinit_chpl95(i_x_chpl2, _ln_chpl, _fn_chpl);
#line 91 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  }
#line 91 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  chpl__autoDestroy62(&call_tmp_chpl12, _ln_chpl, _fn_chpl);
#line 91 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  call_tmp_chpl15 = ((int64_t)(call_tmp_chpl11));
#line 92 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  call_tmp_chpl16 = ((c_ptr_uint8_t_chpl)(call_tmp_chpl6));
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  call_tmp_chpl17 = (call_tmp_chpl15 + INT64(1));
#line 92 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  init_chpl194(&ret_chpl3, _ln_chpl, _fn_chpl);
#line 92 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  i_x_chpl3 = &ret_chpl3;
#line 92 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  (i_x_chpl3)->isOwned = UINT8(true);
#line 92 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  (i_x_chpl3)->buff = call_tmp_chpl16;
#line 92 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  (i_x_chpl3)->buffSize = call_tmp_chpl17;
#line 92 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  (i_x_chpl3)->buffLen = call_tmp_chpl15;
#line 92 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  (&ret_chpl3)->cachedNumCodepoints = call_tmp_chpl15;
#line 92 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  ret_chpl2 = ret_chpl3;
#line 74 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  *(_retArg_chpl) = ret_chpl2;
#line 74 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  return;
#line 74 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
}

#line 98 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
/* StringCasts.chpl:98 */
#line 98 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
static int64_t chpl___COLON_27(string_chpl * x_chpl,
#line 98 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
                               Error_chpl * error_out_chpl,
#line 98 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
                               int64_t _ln_chpl,
#line 98 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
                               int32_t _fn_chpl) {
#line 98 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  int64_t ret_chpl;
#line 125 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  int64_t retVal_chpl;
#line 126 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  chpl_bool isErr_chpl;
#line 128 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  string_chpl ret_chpl2;
#line 128 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  string_chpl ret_chpl3;
#line 128 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  string_chpl localX_chpl;
#line 131 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _owned_IllegalArgumentError_chpl call_tmp_chpl6;
#line 131 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  IllegalArgumentError_chpl new_temp_chpl = NULL;
#line 131 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _owned_IllegalArgumentError_chpl initTemp_chpl;
#line 131 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  Error_chpl call_tmp_chpl7 = NULL;
#line 131 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _owned_Error_chpl coerce_tmp_chpl3;
#line 131 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _owned_Error_chpl ret_tmp_chpl;
#line 131 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _ref__owned_IllegalArgumentError_chpl2 i_x_chpl = NULL;
#line 131 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _ref__owned_Error_chpl2 i__retArg_chpl = NULL;
#line 131 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _owned_IllegalArgumentError_chpl _formal_tmp_x_chpl;
#line 131 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _owned_Error_chpl ret_chpl4;
#line 131 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _ref__owned_IllegalArgumentError_chpl2 i_this_chpl = NULL;
#line 131 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  Error_chpl call_tmp_chpl8 = NULL;
#line 131 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _ref__owned_IllegalArgumentError_chpl2 i_this_chpl2 = NULL;
#line 131 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _owned_Error_chpl call_tmp_chpl9;
#line 131 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _owned_Error_chpl initTemp_chpl2;
#line 131 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _owned_Error_chpl _formal_tmp_in_err_chpl;
#line 131 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _ref_string_chpl i_x_chpl2 = NULL;
#line 137 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  string_chpl call_tmp_chpl10;
#line 137 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  string_chpl ret_tmp_chpl2;
#line 137 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  string_chpl call_tmp_chpl11;
#line 137 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  string_chpl ret_tmp_chpl3;
#line 137 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  string_chpl call_tmp_chpl12;
#line 137 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  string_chpl ret_tmp_chpl4;
#line 137 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _owned_IllegalArgumentError_chpl call_tmp_chpl13;
#line 137 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  IllegalArgumentError_chpl new_temp_chpl2 = NULL;
#line 137 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _owned_IllegalArgumentError_chpl initTemp_chpl3;
#line 137 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  Error_chpl call_tmp_chpl14 = NULL;
#line 137 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _owned_Error_chpl coerce_tmp_chpl4;
#line 137 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _owned_Error_chpl ret_tmp_chpl5;
#line 137 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _ref__owned_IllegalArgumentError_chpl2 i_x_chpl3 = NULL;
#line 137 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _ref__owned_Error_chpl2 i__retArg_chpl2 = NULL;
#line 137 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _owned_IllegalArgumentError_chpl _formal_tmp_x_chpl2;
#line 137 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _owned_Error_chpl ret_chpl5;
#line 137 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _ref__owned_IllegalArgumentError_chpl2 i_this_chpl3 = NULL;
#line 137 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  Error_chpl call_tmp_chpl15 = NULL;
#line 137 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _ref__owned_IllegalArgumentError_chpl2 i_this_chpl4 = NULL;
#line 137 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _owned_Error_chpl call_tmp_chpl16;
#line 137 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _owned_Error_chpl initTemp_chpl4;
#line 137 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _owned_Error_chpl _formal_tmp_in_err_chpl2;
#line 137 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _ref_string_chpl i_x_chpl4 = NULL;
#line 137 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _ref_string_chpl i_x_chpl5 = NULL;
#line 137 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _ref_string_chpl i_x_chpl6 = NULL;
#line 137 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _ref_string_chpl i_x_chpl7 = NULL;
#line 144 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  int64_t call_tmp_chpl17;
#line 158 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  string_chpl call_tmp_chpl18;
#line 158 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  string_chpl ret_tmp_chpl6;
#line 158 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  string_chpl call_tmp_chpl19;
#line 158 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  string_chpl ret_tmp_chpl7;
#line 158 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  string_chpl call_tmp_chpl20;
#line 158 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  string_chpl ret_tmp_chpl8;
#line 158 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _owned_IllegalArgumentError_chpl call_tmp_chpl21;
#line 158 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  IllegalArgumentError_chpl new_temp_chpl3 = NULL;
#line 158 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _owned_IllegalArgumentError_chpl initTemp_chpl5;
#line 158 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  Error_chpl call_tmp_chpl22 = NULL;
#line 158 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _owned_Error_chpl coerce_tmp_chpl5;
#line 158 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _owned_Error_chpl ret_tmp_chpl9;
#line 158 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _ref__owned_IllegalArgumentError_chpl2 i_x_chpl8 = NULL;
#line 158 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _ref__owned_Error_chpl2 i__retArg_chpl3 = NULL;
#line 158 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _owned_IllegalArgumentError_chpl _formal_tmp_x_chpl3;
#line 158 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _owned_Error_chpl ret_chpl6;
#line 158 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _ref__owned_IllegalArgumentError_chpl2 i_this_chpl5 = NULL;
#line 158 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  Error_chpl call_tmp_chpl23 = NULL;
#line 158 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _ref__owned_IllegalArgumentError_chpl2 i_this_chpl6 = NULL;
#line 158 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _owned_Error_chpl call_tmp_chpl24;
#line 158 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _owned_Error_chpl initTemp_chpl6;
#line 158 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _owned_Error_chpl _formal_tmp_in_err_chpl3;
#line 158 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _ref_string_chpl i_x_chpl9 = NULL;
#line 158 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _ref_string_chpl i_x_chpl10 = NULL;
#line 158 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _ref_string_chpl i_x_chpl11 = NULL;
#line 158 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _ref_string_chpl i_x_chpl12 = NULL;
#line 160 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _ref_string_chpl i_x_chpl13 = NULL;
#line 126 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  isErr_chpl = UINT8(false);
#line 128 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  init_chpl194(&ret_chpl3, _ln_chpl, _fn_chpl);
#line 128 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  (&ret_chpl3)->cachedNumCodepoints = (x_chpl)->cachedNumCodepoints;
#line 128 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  initWithBorrowedBuffer_chpl(&ret_chpl3, x_chpl, _ln_chpl, _fn_chpl);
#line 128 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  ret_chpl2 = ret_chpl3;
#line 128 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  localX_chpl = ret_chpl2;
#line 130 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  if ((&localX_chpl)->buffLen == INT64(0)) /* ZLINE: 130 /Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl */
#line 130 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  {
#line 131 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    new_temp_chpl = _new_chpl59(&_str_literal_4046_chpl, _ln_chpl, _fn_chpl);
#line 131 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    init_chpl179(&initTemp_chpl, new_temp_chpl);
#line 131 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    call_tmp_chpl6 = initTemp_chpl;
#line 131 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    i_x_chpl = &call_tmp_chpl6;
#line 131 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    i__retArg_chpl = &ret_tmp_chpl;
#line 131 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    _formal_tmp_x_chpl = *(i_x_chpl);
#line 131 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    i_this_chpl = &_formal_tmp_x_chpl;
#line 131 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    call_tmp_chpl8 = ((Error_chpl)((i_this_chpl)->chpl_p));
#line 131 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    i_this_chpl2 = &_formal_tmp_x_chpl;
#line 131 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    (i_this_chpl2)->chpl_p = ((IllegalArgumentError_chpl)(nil));
#line 131 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    if (((RootClass_chpl)(call_tmp_chpl8)) == nil) /* ZLINE: 131 /Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl */
#line 131 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    {
#line 131 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
      nilCheckHalt_chpl(&_str_literal_52_chpl, _ln_chpl, _fn_chpl);
#line 131 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    }
#line 131 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    init_chpl188(&initTemp_chpl2, call_tmp_chpl8);
#line 131 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    call_tmp_chpl9 = initTemp_chpl2;
#line 131 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    ret_chpl4 = call_tmp_chpl9;
#line 131 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    chpl__autoDestroy59(&_formal_tmp_x_chpl, _ln_chpl, _fn_chpl);
#line 131 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    *(i__retArg_chpl) = ret_chpl4;
#line 131 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    coerce_tmp_chpl3 = ret_tmp_chpl;
#line 131 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    _formal_tmp_in_err_chpl = coerce_tmp_chpl3;
#line 131 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    call_tmp_chpl7 = chpl_fix_thrown_error(&_formal_tmp_in_err_chpl, _ln_chpl, _fn_chpl);
#line 131 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    *(error_out_chpl) = call_tmp_chpl7;
#line 131 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    i_x_chpl2 = &localX_chpl;
#line 131 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    deinit_chpl95(i_x_chpl2, _ln_chpl, _fn_chpl);
#line 131 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    goto _end__COLON__chpl;
#line 131 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  }
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _cleanupForNumericCast_chpl(&localX_chpl, _ln_chpl, _fn_chpl);
#line 136 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  if ((&localX_chpl)->buffLen == INT64(0)) /* ZLINE: 136 /Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl */
#line 136 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  {
#line 137 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    chpl___PLUS_(&_str_literal_184_chpl, x_chpl, &ret_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 137 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    call_tmp_chpl10 = ret_tmp_chpl2;
#line 137 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    chpl___PLUS_(&call_tmp_chpl10, &_str_literal_1364_chpl, &ret_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 137 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    call_tmp_chpl11 = ret_tmp_chpl3;
#line 137 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    chpl___PLUS_(&call_tmp_chpl11, &_str_literal_3962_chpl, &ret_tmp_chpl4, _ln_chpl, _fn_chpl);
#line 137 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    call_tmp_chpl12 = ret_tmp_chpl4;
#line 137 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    new_temp_chpl2 = _new_chpl59(&call_tmp_chpl12, _ln_chpl, _fn_chpl);
#line 137 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    init_chpl179(&initTemp_chpl3, new_temp_chpl2);
#line 137 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    call_tmp_chpl13 = initTemp_chpl3;
#line 137 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    i_x_chpl3 = &call_tmp_chpl13;
#line 137 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    i__retArg_chpl2 = &ret_tmp_chpl5;
#line 137 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    _formal_tmp_x_chpl2 = *(i_x_chpl3);
#line 137 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    i_this_chpl3 = &_formal_tmp_x_chpl2;
#line 137 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    call_tmp_chpl15 = ((Error_chpl)((i_this_chpl3)->chpl_p));
#line 137 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    i_this_chpl4 = &_formal_tmp_x_chpl2;
#line 137 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    (i_this_chpl4)->chpl_p = ((IllegalArgumentError_chpl)(nil));
#line 137 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    if (((RootClass_chpl)(call_tmp_chpl15)) == nil) /* ZLINE: 137 /Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl */
#line 137 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    {
#line 137 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
      nilCheckHalt_chpl(&_str_literal_52_chpl, _ln_chpl, _fn_chpl);
#line 137 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    }
#line 137 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    init_chpl188(&initTemp_chpl4, call_tmp_chpl15);
#line 137 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    call_tmp_chpl16 = initTemp_chpl4;
#line 137 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    ret_chpl5 = call_tmp_chpl16;
#line 137 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    chpl__autoDestroy59(&_formal_tmp_x_chpl2, _ln_chpl, _fn_chpl);
#line 137 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    *(i__retArg_chpl2) = ret_chpl5;
#line 137 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    coerce_tmp_chpl4 = ret_tmp_chpl5;
#line 137 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    _formal_tmp_in_err_chpl2 = coerce_tmp_chpl4;
#line 137 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    call_tmp_chpl14 = chpl_fix_thrown_error(&_formal_tmp_in_err_chpl2, _ln_chpl, _fn_chpl);
#line 137 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    *(error_out_chpl) = call_tmp_chpl14;
#line 137 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    i_x_chpl4 = &call_tmp_chpl12;
#line 137 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    deinit_chpl95(i_x_chpl4, _ln_chpl, _fn_chpl);
#line 137 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    i_x_chpl5 = &call_tmp_chpl11;
#line 137 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    deinit_chpl95(i_x_chpl5, _ln_chpl, _fn_chpl);
#line 137 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    i_x_chpl6 = &call_tmp_chpl10;
#line 137 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    deinit_chpl95(i_x_chpl6, _ln_chpl, _fn_chpl);
#line 137 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    i_x_chpl7 = &localX_chpl;
#line 137 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    deinit_chpl95(i_x_chpl7, _ln_chpl, _fn_chpl);
#line 137 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    goto _end__COLON__chpl;
#line 137 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  }
#line 144 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  call_tmp_chpl17 = c_string_to_int64_t(((c_ptrConst_int8_t_chpl)((&localX_chpl)->buff)), &isErr_chpl, _ln_chpl, _fn_chpl);
#line 144 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  retVal_chpl = call_tmp_chpl17;
#line 157 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  if (isErr_chpl) /* ZLINE: 157 /Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl */
#line 157 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  {
#line 158 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    chpl___PLUS_(&_str_literal_184_chpl, x_chpl, &ret_tmp_chpl6, _ln_chpl, _fn_chpl);
#line 158 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    call_tmp_chpl18 = ret_tmp_chpl6;
#line 158 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    chpl___PLUS_(&call_tmp_chpl18, &_str_literal_1364_chpl, &ret_tmp_chpl7, _ln_chpl, _fn_chpl);
#line 158 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    call_tmp_chpl19 = ret_tmp_chpl7;
#line 158 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    chpl___PLUS_(&call_tmp_chpl19, &_str_literal_3962_chpl, &ret_tmp_chpl8, _ln_chpl, _fn_chpl);
#line 158 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    call_tmp_chpl20 = ret_tmp_chpl8;
#line 158 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    new_temp_chpl3 = _new_chpl59(&call_tmp_chpl20, _ln_chpl, _fn_chpl);
#line 158 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    init_chpl179(&initTemp_chpl5, new_temp_chpl3);
#line 158 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    call_tmp_chpl21 = initTemp_chpl5;
#line 158 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    i_x_chpl8 = &call_tmp_chpl21;
#line 158 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    i__retArg_chpl3 = &ret_tmp_chpl9;
#line 158 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    _formal_tmp_x_chpl3 = *(i_x_chpl8);
#line 158 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    i_this_chpl5 = &_formal_tmp_x_chpl3;
#line 158 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    call_tmp_chpl23 = ((Error_chpl)((i_this_chpl5)->chpl_p));
#line 158 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    i_this_chpl6 = &_formal_tmp_x_chpl3;
#line 158 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    (i_this_chpl6)->chpl_p = ((IllegalArgumentError_chpl)(nil));
#line 158 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    if (((RootClass_chpl)(call_tmp_chpl23)) == nil) /* ZLINE: 158 /Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl */
#line 158 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    {
#line 158 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
      nilCheckHalt_chpl(&_str_literal_52_chpl, _ln_chpl, _fn_chpl);
#line 158 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    }
#line 158 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    init_chpl188(&initTemp_chpl6, call_tmp_chpl23);
#line 158 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    call_tmp_chpl24 = initTemp_chpl6;
#line 158 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    ret_chpl6 = call_tmp_chpl24;
#line 158 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    chpl__autoDestroy59(&_formal_tmp_x_chpl3, _ln_chpl, _fn_chpl);
#line 158 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    *(i__retArg_chpl3) = ret_chpl6;
#line 158 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    coerce_tmp_chpl5 = ret_tmp_chpl9;
#line 158 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    _formal_tmp_in_err_chpl3 = coerce_tmp_chpl5;
#line 158 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    call_tmp_chpl22 = chpl_fix_thrown_error(&_formal_tmp_in_err_chpl3, _ln_chpl, _fn_chpl);
#line 158 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    *(error_out_chpl) = call_tmp_chpl22;
#line 158 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    i_x_chpl9 = &call_tmp_chpl20;
#line 158 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    deinit_chpl95(i_x_chpl9, _ln_chpl, _fn_chpl);
#line 158 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    i_x_chpl10 = &call_tmp_chpl19;
#line 158 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    deinit_chpl95(i_x_chpl10, _ln_chpl, _fn_chpl);
#line 158 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    i_x_chpl11 = &call_tmp_chpl18;
#line 158 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    deinit_chpl95(i_x_chpl11, _ln_chpl, _fn_chpl);
#line 158 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    i_x_chpl12 = &localX_chpl;
#line 158 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    deinit_chpl95(i_x_chpl12, _ln_chpl, _fn_chpl);
#line 158 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    goto _end__COLON__chpl;
#line 158 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  }
#line 160 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  ret_chpl = retVal_chpl;
#line 160 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  i_x_chpl13 = &localX_chpl;
#line 160 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  deinit_chpl95(i_x_chpl13, _ln_chpl, _fn_chpl);
#line 160 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _end__COLON__chpl:;
#line 98 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  return ret_chpl;
#line 98 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
}

#line 98 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
/* StringCasts.chpl:98 */
#line 98 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
static uint64_t chpl___COLON_26(string_chpl * x_chpl,
#line 98 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
                                Error_chpl * error_out_chpl,
#line 98 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
                                int64_t _ln_chpl,
#line 98 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
                                int32_t _fn_chpl) {
#line 98 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  uint64_t ret_chpl;
#line 125 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  uint64_t retVal_chpl;
#line 126 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  chpl_bool isErr_chpl;
#line 128 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  string_chpl ret_chpl2;
#line 128 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  string_chpl ret_chpl3;
#line 128 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  string_chpl localX_chpl;
#line 131 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _owned_IllegalArgumentError_chpl call_tmp_chpl6;
#line 131 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  IllegalArgumentError_chpl new_temp_chpl = NULL;
#line 131 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _owned_IllegalArgumentError_chpl initTemp_chpl;
#line 131 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  Error_chpl call_tmp_chpl7 = NULL;
#line 131 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _owned_Error_chpl coerce_tmp_chpl3;
#line 131 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _owned_Error_chpl ret_tmp_chpl;
#line 131 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _ref__owned_IllegalArgumentError_chpl2 i_x_chpl = NULL;
#line 131 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _ref__owned_Error_chpl2 i__retArg_chpl = NULL;
#line 131 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _owned_IllegalArgumentError_chpl _formal_tmp_x_chpl;
#line 131 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _owned_Error_chpl ret_chpl4;
#line 131 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _ref__owned_IllegalArgumentError_chpl2 i_this_chpl = NULL;
#line 131 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  Error_chpl call_tmp_chpl8 = NULL;
#line 131 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _ref__owned_IllegalArgumentError_chpl2 i_this_chpl2 = NULL;
#line 131 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _owned_Error_chpl call_tmp_chpl9;
#line 131 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _owned_Error_chpl initTemp_chpl2;
#line 131 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _owned_Error_chpl _formal_tmp_in_err_chpl;
#line 131 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _ref_string_chpl i_x_chpl2 = NULL;
#line 137 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  string_chpl call_tmp_chpl10;
#line 137 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  string_chpl ret_tmp_chpl2;
#line 137 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  string_chpl call_tmp_chpl11;
#line 137 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  string_chpl ret_tmp_chpl3;
#line 137 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  string_chpl call_tmp_chpl12;
#line 137 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  string_chpl ret_tmp_chpl4;
#line 137 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _owned_IllegalArgumentError_chpl call_tmp_chpl13;
#line 137 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  IllegalArgumentError_chpl new_temp_chpl2 = NULL;
#line 137 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _owned_IllegalArgumentError_chpl initTemp_chpl3;
#line 137 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  Error_chpl call_tmp_chpl14 = NULL;
#line 137 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _owned_Error_chpl coerce_tmp_chpl4;
#line 137 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _owned_Error_chpl ret_tmp_chpl5;
#line 137 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _ref__owned_IllegalArgumentError_chpl2 i_x_chpl3 = NULL;
#line 137 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _ref__owned_Error_chpl2 i__retArg_chpl2 = NULL;
#line 137 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _owned_IllegalArgumentError_chpl _formal_tmp_x_chpl2;
#line 137 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _owned_Error_chpl ret_chpl5;
#line 137 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _ref__owned_IllegalArgumentError_chpl2 i_this_chpl3 = NULL;
#line 137 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  Error_chpl call_tmp_chpl15 = NULL;
#line 137 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _ref__owned_IllegalArgumentError_chpl2 i_this_chpl4 = NULL;
#line 137 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _owned_Error_chpl call_tmp_chpl16;
#line 137 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _owned_Error_chpl initTemp_chpl4;
#line 137 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _owned_Error_chpl _formal_tmp_in_err_chpl2;
#line 137 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _ref_string_chpl i_x_chpl4 = NULL;
#line 137 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _ref_string_chpl i_x_chpl5 = NULL;
#line 137 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _ref_string_chpl i_x_chpl6 = NULL;
#line 137 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _ref_string_chpl i_x_chpl7 = NULL;
#line 152 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  uint64_t call_tmp_chpl17;
#line 158 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  string_chpl call_tmp_chpl18;
#line 158 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  string_chpl ret_tmp_chpl6;
#line 158 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  string_chpl call_tmp_chpl19;
#line 158 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  string_chpl ret_tmp_chpl7;
#line 158 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  string_chpl call_tmp_chpl20;
#line 158 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  string_chpl ret_tmp_chpl8;
#line 158 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _owned_IllegalArgumentError_chpl call_tmp_chpl21;
#line 158 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  IllegalArgumentError_chpl new_temp_chpl3 = NULL;
#line 158 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _owned_IllegalArgumentError_chpl initTemp_chpl5;
#line 158 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  Error_chpl call_tmp_chpl22 = NULL;
#line 158 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _owned_Error_chpl coerce_tmp_chpl5;
#line 158 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _owned_Error_chpl ret_tmp_chpl9;
#line 158 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _ref__owned_IllegalArgumentError_chpl2 i_x_chpl8 = NULL;
#line 158 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _ref__owned_Error_chpl2 i__retArg_chpl3 = NULL;
#line 158 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _owned_IllegalArgumentError_chpl _formal_tmp_x_chpl3;
#line 158 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _owned_Error_chpl ret_chpl6;
#line 158 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _ref__owned_IllegalArgumentError_chpl2 i_this_chpl5 = NULL;
#line 158 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  Error_chpl call_tmp_chpl23 = NULL;
#line 158 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _ref__owned_IllegalArgumentError_chpl2 i_this_chpl6 = NULL;
#line 158 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _owned_Error_chpl call_tmp_chpl24;
#line 158 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _owned_Error_chpl initTemp_chpl6;
#line 158 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _owned_Error_chpl _formal_tmp_in_err_chpl3;
#line 158 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _ref_string_chpl i_x_chpl9 = NULL;
#line 158 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _ref_string_chpl i_x_chpl10 = NULL;
#line 158 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _ref_string_chpl i_x_chpl11 = NULL;
#line 158 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _ref_string_chpl i_x_chpl12 = NULL;
#line 160 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _ref_string_chpl i_x_chpl13 = NULL;
#line 126 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  isErr_chpl = UINT8(false);
#line 128 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  init_chpl194(&ret_chpl3, _ln_chpl, _fn_chpl);
#line 128 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  (&ret_chpl3)->cachedNumCodepoints = (x_chpl)->cachedNumCodepoints;
#line 128 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  initWithBorrowedBuffer_chpl(&ret_chpl3, x_chpl, _ln_chpl, _fn_chpl);
#line 128 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  ret_chpl2 = ret_chpl3;
#line 128 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  localX_chpl = ret_chpl2;
#line 130 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  if ((&localX_chpl)->buffLen == INT64(0)) /* ZLINE: 130 /Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl */
#line 130 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  {
#line 131 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    new_temp_chpl = _new_chpl59(&_str_literal_4091_chpl, _ln_chpl, _fn_chpl);
#line 131 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    init_chpl179(&initTemp_chpl, new_temp_chpl);
#line 131 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    call_tmp_chpl6 = initTemp_chpl;
#line 131 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    i_x_chpl = &call_tmp_chpl6;
#line 131 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    i__retArg_chpl = &ret_tmp_chpl;
#line 131 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    _formal_tmp_x_chpl = *(i_x_chpl);
#line 131 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    i_this_chpl = &_formal_tmp_x_chpl;
#line 131 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    call_tmp_chpl8 = ((Error_chpl)((i_this_chpl)->chpl_p));
#line 131 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    i_this_chpl2 = &_formal_tmp_x_chpl;
#line 131 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    (i_this_chpl2)->chpl_p = ((IllegalArgumentError_chpl)(nil));
#line 131 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    if (((RootClass_chpl)(call_tmp_chpl8)) == nil) /* ZLINE: 131 /Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl */
#line 131 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    {
#line 131 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
      nilCheckHalt_chpl(&_str_literal_52_chpl, _ln_chpl, _fn_chpl);
#line 131 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    }
#line 131 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    init_chpl188(&initTemp_chpl2, call_tmp_chpl8);
#line 131 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    call_tmp_chpl9 = initTemp_chpl2;
#line 131 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    ret_chpl4 = call_tmp_chpl9;
#line 131 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    chpl__autoDestroy59(&_formal_tmp_x_chpl, _ln_chpl, _fn_chpl);
#line 131 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    *(i__retArg_chpl) = ret_chpl4;
#line 131 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    coerce_tmp_chpl3 = ret_tmp_chpl;
#line 131 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    _formal_tmp_in_err_chpl = coerce_tmp_chpl3;
#line 131 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    call_tmp_chpl7 = chpl_fix_thrown_error(&_formal_tmp_in_err_chpl, _ln_chpl, _fn_chpl);
#line 131 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    *(error_out_chpl) = call_tmp_chpl7;
#line 131 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    i_x_chpl2 = &localX_chpl;
#line 131 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    deinit_chpl95(i_x_chpl2, _ln_chpl, _fn_chpl);
#line 131 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    goto _end__COLON__chpl;
#line 131 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  }
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _cleanupForNumericCast_chpl(&localX_chpl, _ln_chpl, _fn_chpl);
#line 136 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  if ((&localX_chpl)->buffLen == INT64(0)) /* ZLINE: 136 /Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl */
#line 136 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  {
#line 137 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    chpl___PLUS_(&_str_literal_184_chpl, x_chpl, &ret_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 137 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    call_tmp_chpl10 = ret_tmp_chpl2;
#line 137 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    chpl___PLUS_(&call_tmp_chpl10, &_str_literal_1364_chpl, &ret_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 137 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    call_tmp_chpl11 = ret_tmp_chpl3;
#line 137 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    chpl___PLUS_(&call_tmp_chpl11, &_str_literal_3965_chpl, &ret_tmp_chpl4, _ln_chpl, _fn_chpl);
#line 137 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    call_tmp_chpl12 = ret_tmp_chpl4;
#line 137 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    new_temp_chpl2 = _new_chpl59(&call_tmp_chpl12, _ln_chpl, _fn_chpl);
#line 137 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    init_chpl179(&initTemp_chpl3, new_temp_chpl2);
#line 137 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    call_tmp_chpl13 = initTemp_chpl3;
#line 137 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    i_x_chpl3 = &call_tmp_chpl13;
#line 137 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    i__retArg_chpl2 = &ret_tmp_chpl5;
#line 137 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    _formal_tmp_x_chpl2 = *(i_x_chpl3);
#line 137 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    i_this_chpl3 = &_formal_tmp_x_chpl2;
#line 137 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    call_tmp_chpl15 = ((Error_chpl)((i_this_chpl3)->chpl_p));
#line 137 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    i_this_chpl4 = &_formal_tmp_x_chpl2;
#line 137 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    (i_this_chpl4)->chpl_p = ((IllegalArgumentError_chpl)(nil));
#line 137 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    if (((RootClass_chpl)(call_tmp_chpl15)) == nil) /* ZLINE: 137 /Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl */
#line 137 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    {
#line 137 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
      nilCheckHalt_chpl(&_str_literal_52_chpl, _ln_chpl, _fn_chpl);
#line 137 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    }
#line 137 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    init_chpl188(&initTemp_chpl4, call_tmp_chpl15);
#line 137 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    call_tmp_chpl16 = initTemp_chpl4;
#line 137 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    ret_chpl5 = call_tmp_chpl16;
#line 137 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    chpl__autoDestroy59(&_formal_tmp_x_chpl2, _ln_chpl, _fn_chpl);
#line 137 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    *(i__retArg_chpl2) = ret_chpl5;
#line 137 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    coerce_tmp_chpl4 = ret_tmp_chpl5;
#line 137 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    _formal_tmp_in_err_chpl2 = coerce_tmp_chpl4;
#line 137 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    call_tmp_chpl14 = chpl_fix_thrown_error(&_formal_tmp_in_err_chpl2, _ln_chpl, _fn_chpl);
#line 137 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    *(error_out_chpl) = call_tmp_chpl14;
#line 137 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    i_x_chpl4 = &call_tmp_chpl12;
#line 137 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    deinit_chpl95(i_x_chpl4, _ln_chpl, _fn_chpl);
#line 137 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    i_x_chpl5 = &call_tmp_chpl11;
#line 137 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    deinit_chpl95(i_x_chpl5, _ln_chpl, _fn_chpl);
#line 137 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    i_x_chpl6 = &call_tmp_chpl10;
#line 137 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    deinit_chpl95(i_x_chpl6, _ln_chpl, _fn_chpl);
#line 137 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    i_x_chpl7 = &localX_chpl;
#line 137 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    deinit_chpl95(i_x_chpl7, _ln_chpl, _fn_chpl);
#line 137 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    goto _end__COLON__chpl;
#line 137 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  }
#line 152 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  call_tmp_chpl17 = c_string_to_uint64_t(((c_ptrConst_int8_t_chpl)((&localX_chpl)->buff)), &isErr_chpl, _ln_chpl, _fn_chpl);
#line 152 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  retVal_chpl = call_tmp_chpl17;
#line 157 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  if (isErr_chpl) /* ZLINE: 157 /Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl */
#line 157 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  {
#line 158 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    chpl___PLUS_(&_str_literal_184_chpl, x_chpl, &ret_tmp_chpl6, _ln_chpl, _fn_chpl);
#line 158 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    call_tmp_chpl18 = ret_tmp_chpl6;
#line 158 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    chpl___PLUS_(&call_tmp_chpl18, &_str_literal_1364_chpl, &ret_tmp_chpl7, _ln_chpl, _fn_chpl);
#line 158 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    call_tmp_chpl19 = ret_tmp_chpl7;
#line 158 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    chpl___PLUS_(&call_tmp_chpl19, &_str_literal_3965_chpl, &ret_tmp_chpl8, _ln_chpl, _fn_chpl);
#line 158 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    call_tmp_chpl20 = ret_tmp_chpl8;
#line 158 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    new_temp_chpl3 = _new_chpl59(&call_tmp_chpl20, _ln_chpl, _fn_chpl);
#line 158 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    init_chpl179(&initTemp_chpl5, new_temp_chpl3);
#line 158 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    call_tmp_chpl21 = initTemp_chpl5;
#line 158 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    i_x_chpl8 = &call_tmp_chpl21;
#line 158 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    i__retArg_chpl3 = &ret_tmp_chpl9;
#line 158 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    _formal_tmp_x_chpl3 = *(i_x_chpl8);
#line 158 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    i_this_chpl5 = &_formal_tmp_x_chpl3;
#line 158 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    call_tmp_chpl23 = ((Error_chpl)((i_this_chpl5)->chpl_p));
#line 158 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    i_this_chpl6 = &_formal_tmp_x_chpl3;
#line 158 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    (i_this_chpl6)->chpl_p = ((IllegalArgumentError_chpl)(nil));
#line 158 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    if (((RootClass_chpl)(call_tmp_chpl23)) == nil) /* ZLINE: 158 /Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl */
#line 158 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    {
#line 158 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
      nilCheckHalt_chpl(&_str_literal_52_chpl, _ln_chpl, _fn_chpl);
#line 158 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    }
#line 158 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    init_chpl188(&initTemp_chpl6, call_tmp_chpl23);
#line 158 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    call_tmp_chpl24 = initTemp_chpl6;
#line 158 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    ret_chpl6 = call_tmp_chpl24;
#line 158 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    chpl__autoDestroy59(&_formal_tmp_x_chpl3, _ln_chpl, _fn_chpl);
#line 158 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    *(i__retArg_chpl3) = ret_chpl6;
#line 158 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    coerce_tmp_chpl5 = ret_tmp_chpl9;
#line 158 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    _formal_tmp_in_err_chpl3 = coerce_tmp_chpl5;
#line 158 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    call_tmp_chpl22 = chpl_fix_thrown_error(&_formal_tmp_in_err_chpl3, _ln_chpl, _fn_chpl);
#line 158 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    *(error_out_chpl) = call_tmp_chpl22;
#line 158 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    i_x_chpl9 = &call_tmp_chpl20;
#line 158 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    deinit_chpl95(i_x_chpl9, _ln_chpl, _fn_chpl);
#line 158 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    i_x_chpl10 = &call_tmp_chpl19;
#line 158 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    deinit_chpl95(i_x_chpl10, _ln_chpl, _fn_chpl);
#line 158 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    i_x_chpl11 = &call_tmp_chpl18;
#line 158 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    deinit_chpl95(i_x_chpl11, _ln_chpl, _fn_chpl);
#line 158 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    i_x_chpl12 = &localX_chpl;
#line 158 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    deinit_chpl95(i_x_chpl12, _ln_chpl, _fn_chpl);
#line 158 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    goto _end__COLON__chpl;
#line 158 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  }
#line 160 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  ret_chpl = retVal_chpl;
#line 160 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  i_x_chpl13 = &localX_chpl;
#line 160 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  deinit_chpl95(i_x_chpl13, _ln_chpl, _fn_chpl);
#line 160 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _end__COLON__chpl:;
#line 98 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  return ret_chpl;
#line 98 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
}

#line 194 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
/* StringCasts.chpl:194 */
#line 194 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
static _real64 chpl___COLON_28(string_chpl * x_chpl,
#line 194 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
                               Error_chpl * error_out_chpl,
#line 194 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
                               int64_t _ln_chpl,
#line 194 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
                               int32_t _fn_chpl) {
#line 194 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _real64 ret_chpl;
#line 202 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _real64 retVal_chpl;
#line 203 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  chpl_bool isErr_chpl;
#line 204 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  string_chpl ret_chpl2;
#line 204 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  string_chpl ret_chpl3;
#line 204 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  string_chpl localX_chpl;
#line 207 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _owned_IllegalArgumentError_chpl call_tmp_chpl6;
#line 207 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  IllegalArgumentError_chpl new_temp_chpl = NULL;
#line 207 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _owned_IllegalArgumentError_chpl initTemp_chpl;
#line 207 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  Error_chpl call_tmp_chpl7 = NULL;
#line 207 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _owned_Error_chpl coerce_tmp_chpl3;
#line 207 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _owned_Error_chpl ret_tmp_chpl;
#line 207 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _ref__owned_IllegalArgumentError_chpl2 i_x_chpl = NULL;
#line 207 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _ref__owned_Error_chpl2 i__retArg_chpl = NULL;
#line 207 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _owned_IllegalArgumentError_chpl _formal_tmp_x_chpl;
#line 207 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _owned_Error_chpl ret_chpl4;
#line 207 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _ref__owned_IllegalArgumentError_chpl2 i_this_chpl = NULL;
#line 207 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  Error_chpl call_tmp_chpl8 = NULL;
#line 207 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _ref__owned_IllegalArgumentError_chpl2 i_this_chpl2 = NULL;
#line 207 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _owned_Error_chpl call_tmp_chpl9;
#line 207 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _owned_Error_chpl initTemp_chpl2;
#line 207 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _owned_Error_chpl _formal_tmp_in_err_chpl;
#line 207 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _ref_string_chpl i_x_chpl2 = NULL;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _real64 call_tmp_chpl10;
#line 219 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  string_chpl call_tmp_chpl11;
#line 219 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  string_chpl ret_tmp_chpl2;
#line 219 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  string_chpl call_tmp_chpl12;
#line 219 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  string_chpl ret_tmp_chpl3;
#line 219 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  string_chpl call_tmp_chpl13;
#line 219 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  string_chpl ret_tmp_chpl4;
#line 219 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  string_chpl call_tmp_chpl14;
#line 219 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  string_chpl ret_tmp_chpl5;
#line 219 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _owned_IllegalArgumentError_chpl call_tmp_chpl15;
#line 219 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  IllegalArgumentError_chpl new_temp_chpl2 = NULL;
#line 219 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _owned_IllegalArgumentError_chpl initTemp_chpl3;
#line 219 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  Error_chpl call_tmp_chpl16 = NULL;
#line 219 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _owned_Error_chpl coerce_tmp_chpl4;
#line 219 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _owned_Error_chpl ret_tmp_chpl6;
#line 219 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _ref__owned_IllegalArgumentError_chpl2 i_x_chpl3 = NULL;
#line 219 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _ref__owned_Error_chpl2 i__retArg_chpl2 = NULL;
#line 219 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _owned_IllegalArgumentError_chpl _formal_tmp_x_chpl2;
#line 219 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _owned_Error_chpl ret_chpl5;
#line 219 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _ref__owned_IllegalArgumentError_chpl2 i_this_chpl3 = NULL;
#line 219 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  Error_chpl call_tmp_chpl17 = NULL;
#line 219 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _ref__owned_IllegalArgumentError_chpl2 i_this_chpl4 = NULL;
#line 219 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _owned_Error_chpl call_tmp_chpl18;
#line 219 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _owned_Error_chpl initTemp_chpl4;
#line 219 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _owned_Error_chpl _formal_tmp_in_err_chpl2;
#line 219 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _ref_string_chpl i_x_chpl4 = NULL;
#line 219 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _ref_string_chpl i_x_chpl5 = NULL;
#line 219 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _ref_string_chpl i_x_chpl6 = NULL;
#line 219 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _ref_string_chpl i_x_chpl7 = NULL;
#line 219 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _ref_string_chpl i_x_chpl8 = NULL;
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _ref_string_chpl i_x_chpl9 = NULL;
#line 203 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  isErr_chpl = UINT8(false);
#line 204 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  init_chpl194(&ret_chpl3, _ln_chpl, _fn_chpl);
#line 204 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  (&ret_chpl3)->cachedNumCodepoints = (x_chpl)->cachedNumCodepoints;
#line 204 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  initWithBorrowedBuffer_chpl(&ret_chpl3, x_chpl, _ln_chpl, _fn_chpl);
#line 204 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  ret_chpl2 = ret_chpl3;
#line 204 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  localX_chpl = ret_chpl2;
#line 206 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  if ((&localX_chpl)->buffLen == INT64(0)) /* ZLINE: 206 /Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl */
#line 206 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  {
#line 207 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    new_temp_chpl = _new_chpl59(&_str_literal_4027_chpl, _ln_chpl, _fn_chpl);
#line 207 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    init_chpl179(&initTemp_chpl, new_temp_chpl);
#line 207 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    call_tmp_chpl6 = initTemp_chpl;
#line 207 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    i_x_chpl = &call_tmp_chpl6;
#line 207 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    i__retArg_chpl = &ret_tmp_chpl;
#line 207 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    _formal_tmp_x_chpl = *(i_x_chpl);
#line 207 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    i_this_chpl = &_formal_tmp_x_chpl;
#line 207 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    call_tmp_chpl8 = ((Error_chpl)((i_this_chpl)->chpl_p));
#line 207 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    i_this_chpl2 = &_formal_tmp_x_chpl;
#line 207 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    (i_this_chpl2)->chpl_p = ((IllegalArgumentError_chpl)(nil));
#line 207 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    if (((RootClass_chpl)(call_tmp_chpl8)) == nil) /* ZLINE: 207 /Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl */
#line 207 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    {
#line 207 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
      nilCheckHalt_chpl(&_str_literal_52_chpl, _ln_chpl, _fn_chpl);
#line 207 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    }
#line 207 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    init_chpl188(&initTemp_chpl2, call_tmp_chpl8);
#line 207 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    call_tmp_chpl9 = initTemp_chpl2;
#line 207 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    ret_chpl4 = call_tmp_chpl9;
#line 207 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    chpl__autoDestroy59(&_formal_tmp_x_chpl, _ln_chpl, _fn_chpl);
#line 207 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    *(i__retArg_chpl) = ret_chpl4;
#line 207 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    coerce_tmp_chpl3 = ret_tmp_chpl;
#line 207 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    _formal_tmp_in_err_chpl = coerce_tmp_chpl3;
#line 207 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    call_tmp_chpl7 = chpl_fix_thrown_error(&_formal_tmp_in_err_chpl, _ln_chpl, _fn_chpl);
#line 207 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    *(error_out_chpl) = call_tmp_chpl7;
#line 207 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    i_x_chpl2 = &localX_chpl;
#line 207 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    deinit_chpl95(i_x_chpl2, _ln_chpl, _fn_chpl);
#line 207 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    goto _end__COLON__chpl;
#line 207 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  }
#line 210 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _cleanupForNumericCast_chpl(&localX_chpl, _ln_chpl, _fn_chpl);
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  call_tmp_chpl10 = c_string_to_real64(((c_ptrConst_int8_t_chpl)((&localX_chpl)->buff)), &isErr_chpl, _ln_chpl, _fn_chpl);
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  retVal_chpl = call_tmp_chpl10;
#line 218 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  if (isErr_chpl) /* ZLINE: 218 /Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl */
#line 218 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  {
#line 219 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    chpl___PLUS_(&_str_literal_184_chpl, x_chpl, &ret_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 219 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    call_tmp_chpl11 = ret_tmp_chpl2;
#line 219 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    chpl___PLUS_(&call_tmp_chpl11, &_str_literal_1368_chpl, &ret_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 219 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    call_tmp_chpl12 = ret_tmp_chpl3;
#line 219 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    chpl___PLUS_(&call_tmp_chpl12, &_str_literal_3958_chpl, &ret_tmp_chpl4, _ln_chpl, _fn_chpl);
#line 219 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    call_tmp_chpl13 = ret_tmp_chpl4;
#line 219 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    chpl___PLUS_(&call_tmp_chpl13, &_str_literal_415_chpl, &ret_tmp_chpl5, _ln_chpl, _fn_chpl);
#line 219 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    call_tmp_chpl14 = ret_tmp_chpl5;
#line 219 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    new_temp_chpl2 = _new_chpl59(&call_tmp_chpl14, _ln_chpl, _fn_chpl);
#line 219 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    init_chpl179(&initTemp_chpl3, new_temp_chpl2);
#line 219 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    call_tmp_chpl15 = initTemp_chpl3;
#line 219 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    i_x_chpl3 = &call_tmp_chpl15;
#line 219 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    i__retArg_chpl2 = &ret_tmp_chpl6;
#line 219 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    _formal_tmp_x_chpl2 = *(i_x_chpl3);
#line 219 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    i_this_chpl3 = &_formal_tmp_x_chpl2;
#line 219 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    call_tmp_chpl17 = ((Error_chpl)((i_this_chpl3)->chpl_p));
#line 219 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    i_this_chpl4 = &_formal_tmp_x_chpl2;
#line 219 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    (i_this_chpl4)->chpl_p = ((IllegalArgumentError_chpl)(nil));
#line 219 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    if (((RootClass_chpl)(call_tmp_chpl17)) == nil) /* ZLINE: 219 /Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl */
#line 219 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    {
#line 219 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
      nilCheckHalt_chpl(&_str_literal_52_chpl, _ln_chpl, _fn_chpl);
#line 219 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    }
#line 219 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    init_chpl188(&initTemp_chpl4, call_tmp_chpl17);
#line 219 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    call_tmp_chpl18 = initTemp_chpl4;
#line 219 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    ret_chpl5 = call_tmp_chpl18;
#line 219 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    chpl__autoDestroy59(&_formal_tmp_x_chpl2, _ln_chpl, _fn_chpl);
#line 219 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    *(i__retArg_chpl2) = ret_chpl5;
#line 219 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    coerce_tmp_chpl4 = ret_tmp_chpl6;
#line 219 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    _formal_tmp_in_err_chpl2 = coerce_tmp_chpl4;
#line 219 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    call_tmp_chpl16 = chpl_fix_thrown_error(&_formal_tmp_in_err_chpl2, _ln_chpl, _fn_chpl);
#line 219 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    *(error_out_chpl) = call_tmp_chpl16;
#line 219 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    i_x_chpl4 = &call_tmp_chpl14;
#line 219 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    deinit_chpl95(i_x_chpl4, _ln_chpl, _fn_chpl);
#line 219 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    i_x_chpl5 = &call_tmp_chpl13;
#line 219 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    deinit_chpl95(i_x_chpl5, _ln_chpl, _fn_chpl);
#line 219 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    i_x_chpl6 = &call_tmp_chpl12;
#line 219 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    deinit_chpl95(i_x_chpl6, _ln_chpl, _fn_chpl);
#line 219 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    i_x_chpl7 = &call_tmp_chpl11;
#line 219 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    deinit_chpl95(i_x_chpl7, _ln_chpl, _fn_chpl);
#line 219 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    i_x_chpl8 = &localX_chpl;
#line 219 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    deinit_chpl95(i_x_chpl8, _ln_chpl, _fn_chpl);
#line 219 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
    goto _end__COLON__chpl;
#line 219 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  }
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  ret_chpl = retVal_chpl;
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  i_x_chpl9 = &localX_chpl;
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  deinit_chpl95(i_x_chpl9, _ln_chpl, _fn_chpl);
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  _end__COLON__chpl:;
#line 194 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
  return ret_chpl;
#line 194 "/Users/iainmoncrief/Documents/chapel/modules/internal/StringCasts.chpl"
}

