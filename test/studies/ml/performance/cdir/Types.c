#line 25 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
/* Types.chpl:25 */
#line 25 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
static void chpl__init_Types(int64_t _ln_chpl,
#line 25 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
                             int32_t _fn_chpl) {
#line 25 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
  _ref_int32_t_chpl refIndentLevel_chpl = NULL;
#line 25 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
  if (chpl__init_Types_p) /* ZLINE: 25 /Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl */
#line 25 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
  {
#line 25 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
    goto _exit_chpl__init_Types_chpl;
#line 25 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
  }
#line 25 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
  printModuleInit_chpl("%*s\n", "Types", INT64(5), _ln_chpl, _fn_chpl);
#line 25 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
  refIndentLevel_chpl = &moduleInitLevel_chpl;
#line 25 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
  *(refIndentLevel_chpl) += INT64(1);
#line 25 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
  chpl__init_Types_p = UINT8(true);
#line 25 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
  {
#line 25 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
    chpl__init_HaltWrappers(_ln_chpl, _fn_chpl);
#line 25 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
    chpl__init_CTypes(_ln_chpl, _fn_chpl);
#line 25 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
  }
#line 25 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
  *(refIndentLevel_chpl) -= INT64(1);
#line 25 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
  _exit_chpl__init_Types_chpl:;
#line 25 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
  return;
#line 25 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
}

#line 788 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
/* Types.chpl:788 */
#line 788 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
static _real64 min_chpl(void) {
#line 788 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
  _real64 _formal_type_tmp__chpl;
#line 789 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
  _real64 call_tmp_chpl6;
#line 789 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
  call_tmp_chpl6 = (-MAX_FLOAT64);
#line 788 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
  return call_tmp_chpl6;
#line 788 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
}

#line 894 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
/* Types.chpl:894 */
#line 894 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
static void chpl_checkValue7(int64_t this_chpl31,
#line 894 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
                             _owned_IllegalArgumentError_chpl2 * _retArg_chpl,
#line 894 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
                             int64_t _ln_chpl,
#line 894 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
                             int32_t _fn_chpl) {
#line 894 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
  _owned_IllegalArgumentError_chpl2 ret_chpl;
#line 899 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
  _owned_IllegalArgumentError_chpl call_tmp_chpl6;
#line 899 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
  IllegalArgumentError_chpl new_temp_chpl = NULL;
#line 899 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
  _owned_IllegalArgumentError_chpl initTemp_chpl;
#line 898 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
  if (this_chpl31 < INT64(0)) /* ZLINE: 898 /Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl */
#line 898 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
  {
#line 899 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
    new_temp_chpl = _new_chpl59(&_str_literal_3966_chpl, _ln_chpl, _fn_chpl);
#line 899 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
    init_chpl179(&initTemp_chpl, new_temp_chpl);
#line 899 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
    call_tmp_chpl6 = initTemp_chpl;
#line 899 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
    init_ASSIGN__chpl17(&ret_chpl, &call_tmp_chpl6, _ln_chpl, _fn_chpl);
#line 899 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
    chpl__autoDestroy59(&call_tmp_chpl6, _ln_chpl, _fn_chpl);
#line 899 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
    goto _end_chpl_checkValue_chpl;
#line 899 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
  }
#line 937 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
  init_ASSIGN__chpl22(&ret_chpl, nil, _ln_chpl, _fn_chpl);
#line 937 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
  _end_chpl_checkValue_chpl:;
#line 894 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
  *(_retArg_chpl) = ret_chpl;
#line 894 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
  return;
#line 894 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
}

#line 894 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
/* Types.chpl:894 */
#line 894 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
static void chpl_checkValue3(uint64_t this_chpl31,
#line 894 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
                             _owned_IllegalArgumentError_chpl2 * _retArg_chpl,
#line 894 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
                             int64_t _ln_chpl,
#line 894 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
                             int32_t _fn_chpl) {
#line 894 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
  _owned_IllegalArgumentError_chpl2 ret_chpl;
#line 917 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
  _owned_IllegalArgumentError_chpl call_tmp_chpl6;
#line 917 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
  IllegalArgumentError_chpl new_temp_chpl = NULL;
#line 917 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
  _owned_IllegalArgumentError_chpl initTemp_chpl;
#line 916 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
  if (this_chpl31 > UINT64(9223372036854775807)) /* ZLINE: 916 /Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl */
#line 916 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
  {
#line 917 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
    new_temp_chpl = _new_chpl59(&_str_literal_3974_chpl, _ln_chpl, _fn_chpl);
#line 917 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
    init_chpl179(&initTemp_chpl, new_temp_chpl);
#line 917 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
    call_tmp_chpl6 = initTemp_chpl;
#line 917 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
    init_ASSIGN__chpl17(&ret_chpl, &call_tmp_chpl6, _ln_chpl, _fn_chpl);
#line 917 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
    chpl__autoDestroy59(&call_tmp_chpl6, _ln_chpl, _fn_chpl);
#line 917 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
    goto _end_chpl_checkValue_chpl;
#line 917 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
  }
#line 937 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
  init_ASSIGN__chpl22(&ret_chpl, nil, _ln_chpl, _fn_chpl);
#line 937 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
  _end_chpl_checkValue_chpl:;
#line 894 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
  *(_retArg_chpl) = ret_chpl;
#line 894 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
  return;
#line 894 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
}

#line 894 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
/* Types.chpl:894 */
#line 894 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
static void chpl_checkValue2(int64_t this_chpl31,
#line 894 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
                             _owned_IllegalArgumentError_chpl2 * _retArg_chpl,
#line 894 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
                             int64_t _ln_chpl,
#line 894 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
                             int32_t _fn_chpl) {
#line 894 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
  _owned_IllegalArgumentError_chpl2 ret_chpl;
#line 937 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
  init_ASSIGN__chpl22(&ret_chpl, nil, _ln_chpl, _fn_chpl);
#line 894 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
  *(_retArg_chpl) = ret_chpl;
#line 894 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
  return;
#line 894 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
}

#line 894 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
/* Types.chpl:894 */
#line 894 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
static void chpl_checkValue6(int64_t this_chpl31,
#line 894 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
                             _owned_IllegalArgumentError_chpl2 * _retArg_chpl,
#line 894 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
                             int64_t _ln_chpl,
#line 894 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
                             int32_t _fn_chpl) {
#line 894 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
  _owned_IllegalArgumentError_chpl2 ret_chpl;
#line 923 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
  _owned_IllegalArgumentError_chpl call_tmp_chpl6;
#line 923 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
  IllegalArgumentError_chpl new_temp_chpl = NULL;
#line 923 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
  _owned_IllegalArgumentError_chpl initTemp_chpl;
#line 931 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
  _owned_IllegalArgumentError_chpl call_tmp_chpl7;
#line 931 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
  IllegalArgumentError_chpl new_temp_chpl2 = NULL;
#line 931 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
  _owned_IllegalArgumentError_chpl initTemp_chpl2;
#line 922 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
  if (this_chpl31 > INT64(2147483647)) /* ZLINE: 922 /Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl */
#line 922 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
  {
#line 923 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
    new_temp_chpl = _new_chpl59(&_str_literal_3996_chpl, _ln_chpl, _fn_chpl);
#line 923 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
    init_chpl179(&initTemp_chpl, new_temp_chpl);
#line 923 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
    call_tmp_chpl6 = initTemp_chpl;
#line 923 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
    init_ASSIGN__chpl17(&ret_chpl, &call_tmp_chpl6, _ln_chpl, _fn_chpl);
#line 923 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
    chpl__autoDestroy59(&call_tmp_chpl6, _ln_chpl, _fn_chpl);
#line 923 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
    goto _end_chpl_checkValue_chpl;
#line 923 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
  }
#line 930 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
  if (this_chpl31 < INT64(-2147483648)) /* ZLINE: 930 /Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl */
#line 930 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
  {
#line 931 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
    new_temp_chpl2 = _new_chpl59(&_str_literal_4002_chpl, _ln_chpl, _fn_chpl);
#line 931 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
    init_chpl179(&initTemp_chpl2, new_temp_chpl2);
#line 931 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
    call_tmp_chpl7 = initTemp_chpl2;
#line 931 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
    init_ASSIGN__chpl17(&ret_chpl, &call_tmp_chpl7, _ln_chpl, _fn_chpl);
#line 931 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
    chpl__autoDestroy59(&call_tmp_chpl7, _ln_chpl, _fn_chpl);
#line 931 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
    goto _end_chpl_checkValue_chpl;
#line 931 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
  }
#line 937 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
  init_ASSIGN__chpl22(&ret_chpl, nil, _ln_chpl, _fn_chpl);
#line 937 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
  _end_chpl_checkValue_chpl:;
#line 894 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
  *(_retArg_chpl) = ret_chpl;
#line 894 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
  return;
#line 894 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
}

#line 894 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
/* Types.chpl:894 */
#line 894 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
static void chpl_checkValue4(int32_t this_chpl31,
#line 894 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
                             _owned_IllegalArgumentError_chpl2 * _retArg_chpl,
#line 894 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
                             int64_t _ln_chpl,
#line 894 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
                             int32_t _fn_chpl) {
#line 894 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
  _owned_IllegalArgumentError_chpl2 ret_chpl;
#line 937 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
  init_ASSIGN__chpl22(&ret_chpl, nil, _ln_chpl, _fn_chpl);
#line 894 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
  *(_retArg_chpl) = ret_chpl;
#line 894 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
  return;
#line 894 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
}

#line 894 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
/* Types.chpl:894 */
#line 894 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
static void chpl_checkValue5(uint64_t this_chpl31,
#line 894 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
                             _owned_IllegalArgumentError_chpl2 * _retArg_chpl,
#line 894 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
                             int64_t _ln_chpl,
#line 894 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
                             int32_t _fn_chpl) {
#line 894 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
  _owned_IllegalArgumentError_chpl2 ret_chpl;
#line 937 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
  init_ASSIGN__chpl22(&ret_chpl, nil, _ln_chpl, _fn_chpl);
#line 894 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
  *(_retArg_chpl) = ret_chpl;
#line 894 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
  return;
#line 894 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
}

#line 894 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
/* Types.chpl:894 */
#line 894 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
static void chpl_checkValue(int8_t this_chpl31,
#line 894 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
                            _owned_IllegalArgumentError_chpl2 * _retArg_chpl,
#line 894 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
                            int64_t _ln_chpl,
#line 894 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
                            int32_t _fn_chpl) {
#line 894 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
  _owned_IllegalArgumentError_chpl2 ret_chpl;
#line 937 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
  init_ASSIGN__chpl22(&ret_chpl, nil, _ln_chpl, _fn_chpl);
#line 894 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
  *(_retArg_chpl) = ret_chpl;
#line 894 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
  return;
#line 894 "/Users/iainmoncrief/Documents/chapel/modules/standard/Types.chpl"
}

