#line 33 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
/* Time.chpl:33 */
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
static void chpl__init_Time(int64_t _ln_chpl,
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
                            int32_t _fn_chpl) {
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  _ref_int32_t_chpl refIndentLevel_chpl = NULL;
#line 117 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  _tuple_13_star_int64_t_chpl this_chpl31;
#line 118 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  _tuple_13_star_int64_t_chpl call_tmp_chpl6;
#line 118 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  _tuple_13_star_int64_t_chpl ret_tmp_chpl;
#line 121 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  dateTime_chpl call_tmp_chpl7;
#line 121 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  dateTime_chpl initTemp_chpl;
#line 123 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  int64_t call_tmp_chpl8;
#line 124 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  int64_t call_tmp_chpl9;
#line 125 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  int64_t call_tmp_chpl10;
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  if (chpl__init_Time_p) /* ZLINE: 33 /Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl */
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  {
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
    goto _exit_chpl__init_Time_chpl;
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  }
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  printModuleInit_chpl("%*s\n", "Time", INT64(4), _ln_chpl, _fn_chpl);
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  refIndentLevel_chpl = &moduleInitLevel_chpl;
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  *(refIndentLevel_chpl) += INT64(1);
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  chpl__init_Time_p = UINT8(true);
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  {
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
    chpl__init_HaltWrappers(_ln_chpl, _fn_chpl);
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
    chpl__init_CTypes(_ln_chpl, _fn_chpl);
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
    chpl__init_IO(_ln_chpl, _fn_chpl);
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
    chpl__init_String(_ln_chpl, _fn_chpl);
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
    chpl__init_List(_ln_chpl, _fn_chpl);
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
    chpl__init_ChapelLocks(_ln_chpl, _fn_chpl);
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
    chpl__init_ChapelHashtable(_ln_chpl, _fn_chpl);
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
    chpl__init_FormattedIO(_ln_chpl, _fn_chpl);
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
    chpl__init_Reflection(_ln_chpl, _fn_chpl);
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  }
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  chpl_addModule("Time", ((c_fn_ptr)(chpl__deinit_Time)), _ln_chpl, _fn_chpl);
#line 117 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  *(this_chpl31 + INT64(0)) = INT64(-1);
#line 117 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  *(this_chpl31 + INT64(1)) = INT64(31);
#line 117 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  *(this_chpl31 + INT64(2)) = INT64(28);
#line 117 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  *(this_chpl31 + INT64(3)) = INT64(31);
#line 117 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  *(this_chpl31 + INT64(4)) = INT64(30);
#line 117 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  *(this_chpl31 + INT64(5)) = INT64(31);
#line 117 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  *(this_chpl31 + INT64(6)) = INT64(30);
#line 117 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  *(this_chpl31 + INT64(7)) = INT64(31);
#line 117 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  *(this_chpl31 + INT64(8)) = INT64(31);
#line 117 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  *(this_chpl31 + INT64(9)) = INT64(30);
#line 117 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  *(this_chpl31 + INT64(10)) = INT64(31);
#line 117 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  *(this_chpl31 + INT64(11)) = INT64(30);
#line 117 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  *(this_chpl31 + INT64(12)) = INT64(31);
#line 117 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  memcpy(&DAYS_IN_MONTH_chpl, &this_chpl31, sizeof(_tuple_13_star_int64_t_chpl));
#line 118 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  init_days_before_month_chpl(&ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 118 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  memcpy(&call_tmp_chpl6, &ret_tmp_chpl, sizeof(_tuple_13_star_int64_t_chpl));
#line 118 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  memcpy(&DAYS_BEFORE_MONTH_chpl, &call_tmp_chpl6, sizeof(_tuple_13_star_int64_t_chpl));
#line 121 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  init_chpl227(&initTemp_chpl, INT64(1970), INT64(1), INT64(1), INT64(0), INT64(0), INT64(0), INT64(0), _ln_chpl, _fn_chpl);
#line 121 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  call_tmp_chpl7 = initTemp_chpl;
#line 121 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  unixEpoch_chpl = call_tmp_chpl7;
#line 123 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  call_tmp_chpl8 = daysBeforeYear_chpl(INT64(401));
#line 123 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  DI400Y_chpl = call_tmp_chpl8;
#line 124 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  call_tmp_chpl9 = daysBeforeYear_chpl(INT64(101));
#line 124 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  DI100Y_chpl = call_tmp_chpl9;
#line 125 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  call_tmp_chpl10 = daysBeforeYear_chpl(INT64(5));
#line 125 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  DI4Y_chpl = call_tmp_chpl10;
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  *(refIndentLevel_chpl) -= INT64(1);
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  _exit_chpl__init_Time_chpl:;
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  return;
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
}

#line 33 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
/* Time.chpl:33 */
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
static void chpl__deinit_Time(int64_t _ln_chpl,
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
                              int32_t _fn_chpl) {
#line 121 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  dateTime_chpl local_unixEpoch_chpl;
#line 121 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  _ref_time_chpl _field_destructor_tmp__chpl = NULL;
#line 121 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  local_unixEpoch_chpl = unixEpoch_chpl;
#line 121 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  _field_destructor_tmp__chpl = &((&local_unixEpoch_chpl)->chpl_time);
#line 121 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  deinit_chpl104(_field_destructor_tmp__chpl, _ln_chpl, _fn_chpl);
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  return;
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
}

#line 187 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
/* Time.chpl:187 */
#line 187 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
static void init_days_before_month_chpl(_tuple_13_star_int64_t_chpl * _retArg_chpl,
#line 187 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
                                        int64_t _ln_chpl,
#line 187 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
                                        int32_t _fn_chpl) {
#line 190 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  _tuple_13_star_int64_t_chpl local_DAYS_IN_MONTH_chpl;
#line 187 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  _tuple_13_star_int64_t_chpl ret_chpl;
#line 188 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  _tuple_13_star_int64_t_chpl DBM_chpl;
#line 188 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  _tuple_13_star_int64_t_chpl tup_chpl;
#line 189 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  int64_t _ic__F1_high_chpl;
#line 189 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  int64_t i_chpl;
#line 190 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  _ref_int64_t_chpl call_tmp_chpl6 = NULL;
#line 190 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  _ref_int64_t_chpl call_tmp_chpl7 = NULL;
#line 190 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  _ref_int64_t_chpl call_tmp_chpl8 = NULL;
#line 190 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  memcpy(&local_DAYS_IN_MONTH_chpl, &DAYS_IN_MONTH_chpl, sizeof(_tuple_13_star_int64_t_chpl));
#line 188 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  *(tup_chpl + INT64(0)) = INT64(0);
#line 188 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  *(tup_chpl + INT64(1)) = INT64(0);
#line 188 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  *(tup_chpl + INT64(2)) = INT64(0);
#line 188 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  *(tup_chpl + INT64(3)) = INT64(0);
#line 188 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  *(tup_chpl + INT64(4)) = INT64(0);
#line 188 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  *(tup_chpl + INT64(5)) = INT64(0);
#line 188 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  *(tup_chpl + INT64(6)) = INT64(0);
#line 188 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  *(tup_chpl + INT64(7)) = INT64(0);
#line 188 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  *(tup_chpl + INT64(8)) = INT64(0);
#line 188 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  *(tup_chpl + INT64(9)) = INT64(0);
#line 188 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  *(tup_chpl + INT64(10)) = INT64(0);
#line 188 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  *(tup_chpl + INT64(11)) = INT64(0);
#line 188 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  *(tup_chpl + INT64(12)) = INT64(0);
#line 188 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  memcpy(&DBM_chpl, &tup_chpl, sizeof(_tuple_13_star_int64_t_chpl));
#line 189 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  _ic__F1_high_chpl = INT64(12);
#line 189 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  i_chpl = INT64(0);
#line 189 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  chpl_checkIfRangeIterWillOverflow(INT64(2), INT64(12), INT64(1), INT64(2), INT64(12), UINT8(true), _ln_chpl, _fn_chpl);
#line 189 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  for (i_chpl = INT64(2); ((i_chpl <= _ic__F1_high_chpl)); i_chpl += INT64(1)) {
#line 190 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
    call_tmp_chpl6 = this_chpl19(&DBM_chpl, i_chpl, _ln_chpl, _fn_chpl);
#line 190 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
    call_tmp_chpl7 = this_chpl19(&DBM_chpl, ((int64_t)((i_chpl - INT64(1)))), _ln_chpl, _fn_chpl);
#line 190 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
    call_tmp_chpl8 = this_chpl19(&local_DAYS_IN_MONTH_chpl, ((int64_t)((i_chpl - INT64(1)))), _ln_chpl, _fn_chpl);
#line 190 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
    *(call_tmp_chpl6) = ((int64_t)((*(call_tmp_chpl7) + *(call_tmp_chpl8))));
#line 190 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  }
#line 192 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  memcpy(&ret_chpl, &DBM_chpl, sizeof(_tuple_13_star_int64_t_chpl));
#line 187 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  memcpy(_retArg_chpl, &ret_chpl, sizeof(_tuple_13_star_int64_t_chpl));
#line 187 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  return;
#line 187 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
}

#line 195 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
/* Time.chpl:195 */
#line 195 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
static int64_t daysBeforeYear_chpl(int64_t year_chpl) {
#line 196 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  int64_t call_tmp_chpl6;
#line 197 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  int64_t call_tmp_chpl7;
#line 197 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  int64_t call_tmp_chpl8;
#line 197 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  int64_t call_tmp_chpl9;
#line 197 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  int64_t call_tmp_chpl10;
#line 197 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  int64_t call_tmp_chpl11;
#line 197 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  int64_t call_tmp_chpl12;
#line 197 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  int64_t call_tmp_chpl13;
#line 196 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  call_tmp_chpl6 = (year_chpl - INT64(1));
#line 197 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  call_tmp_chpl7 = (call_tmp_chpl6 * INT64(365));
#line 197 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  call_tmp_chpl8 = (call_tmp_chpl6 / INT64(4));
#line 197 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  call_tmp_chpl9 = (call_tmp_chpl7 + call_tmp_chpl8);
#line 197 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  call_tmp_chpl10 = (call_tmp_chpl6 / INT64(100));
#line 197 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  call_tmp_chpl11 = (call_tmp_chpl9 - call_tmp_chpl10);
#line 197 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  call_tmp_chpl12 = (call_tmp_chpl6 / INT64(400));
#line 197 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  call_tmp_chpl13 = (call_tmp_chpl11 + call_tmp_chpl12);
#line 195 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  return call_tmp_chpl13;
#line 195 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
}

#line 257 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
/* Time.chpl:257 */
#line 257 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
static chpl_bool isLeapYear_chpl(int64_t year_chpl) {
#line 258 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  chpl_bool tmp_chpl;
#line 258 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  chpl_bool tmp_chpl2;
#line 258 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  if (((int64_t)((year_chpl % INT64(4)))) == INT64(0)) /* ZLINE: 258 /Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl */
#line 258 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  {
#line 258 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
    if (((int64_t)((year_chpl % INT64(100)))) != INT64(0)) /* ZLINE: 258 /Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl */
#line 258 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
    {
#line 258 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
      tmp_chpl2 = UINT8(true);
#line 258 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
    } else /* ZLINE: 258 /Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl */
#line 258 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
    {
#line 258 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
      tmp_chpl2 = (((int64_t)((year_chpl % INT64(400)))) == INT64(0));
#line 258 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
    }
#line 258 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
    tmp_chpl = tmp_chpl2;
#line 258 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  } else /* ZLINE: 258 /Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl */
#line 258 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  {
#line 258 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
    tmp_chpl = UINT8(false);
#line 258 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  }
#line 257 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  return tmp_chpl;
#line 257 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
}

#line 266 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
/* Time.chpl:266 */
#line 266 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
static int64_t daysInMonth_chpl(int64_t year_chpl,
#line 266 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
                                int64_t month_chpl,
#line 266 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
                                Error_chpl * error_out_chpl,
#line 266 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
                                int64_t _ln_chpl,
#line 266 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
                                int32_t _fn_chpl) {
#line 272 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  _tuple_13_star_int64_t_chpl local_DAYS_IN_MONTH_chpl;
#line 266 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  int64_t ret_chpl;
#line 267 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  chpl_bool tmp_chpl;
#line 268 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  _owned_IllegalArgumentError_chpl call_tmp_chpl6;
#line 268 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  IllegalArgumentError_chpl new_temp_chpl = NULL;
#line 268 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  _owned_IllegalArgumentError_chpl initTemp_chpl;
#line 268 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  Error_chpl call_tmp_chpl7 = NULL;
#line 268 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  _owned_Error_chpl coerce_tmp_chpl3;
#line 268 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  _owned_Error_chpl ret_tmp_chpl;
#line 268 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  _ref__owned_IllegalArgumentError_chpl2 i_x_chpl = NULL;
#line 268 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  _ref__owned_Error_chpl2 i__retArg_chpl = NULL;
#line 268 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  _owned_IllegalArgumentError_chpl _formal_tmp_x_chpl;
#line 268 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  _owned_Error_chpl ret_chpl2;
#line 268 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  _ref__owned_IllegalArgumentError_chpl2 i_this_chpl = NULL;
#line 268 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  Error_chpl call_tmp_chpl8 = NULL;
#line 268 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  _ref__owned_IllegalArgumentError_chpl2 i_this_chpl2 = NULL;
#line 268 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  _owned_Error_chpl call_tmp_chpl9;
#line 268 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  _owned_Error_chpl initTemp_chpl2;
#line 268 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  _owned_Error_chpl _formal_tmp_in_err_chpl;
#line 269 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  chpl_bool tmp_chpl2;
#line 269 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  chpl_bool call_tmp_chpl10;
#line 272 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  _ref_int64_t_chpl call_tmp_chpl11 = NULL;
#line 272 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  memcpy(&local_DAYS_IN_MONTH_chpl, &DAYS_IN_MONTH_chpl, sizeof(_tuple_13_star_int64_t_chpl));
#line 267 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  if (month_chpl < INT64(1)) /* ZLINE: 267 /Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl */
#line 267 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  {
#line 267 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
    tmp_chpl = UINT8(true);
#line 267 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  } else /* ZLINE: 267 /Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl */
#line 267 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  {
#line 267 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
    tmp_chpl = (month_chpl > INT64(12));
#line 267 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  }
#line 267 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  if (tmp_chpl) /* ZLINE: 267 /Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl */
#line 267 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  {
#line 268 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
    new_temp_chpl = _new_chpl59(&_str_literal_2160_chpl, _ln_chpl, _fn_chpl);
#line 268 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
    init_chpl179(&initTemp_chpl, new_temp_chpl);
#line 268 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
    call_tmp_chpl6 = initTemp_chpl;
#line 268 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
    i_x_chpl = &call_tmp_chpl6;
#line 268 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
    i__retArg_chpl = &ret_tmp_chpl;
#line 268 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
    _formal_tmp_x_chpl = *(i_x_chpl);
#line 268 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
    i_this_chpl = &_formal_tmp_x_chpl;
#line 268 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
    call_tmp_chpl8 = ((Error_chpl)((i_this_chpl)->chpl_p));
#line 268 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
    i_this_chpl2 = &_formal_tmp_x_chpl;
#line 268 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
    (i_this_chpl2)->chpl_p = ((IllegalArgumentError_chpl)(nil));
#line 268 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
    if (((RootClass_chpl)(call_tmp_chpl8)) == nil) /* ZLINE: 268 /Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl */
#line 268 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
    {
#line 268 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
      nilCheckHalt_chpl(&_str_literal_52_chpl, _ln_chpl, _fn_chpl);
#line 268 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
    }
#line 268 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
    init_chpl188(&initTemp_chpl2, call_tmp_chpl8);
#line 268 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
    call_tmp_chpl9 = initTemp_chpl2;
#line 268 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
    ret_chpl2 = call_tmp_chpl9;
#line 268 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
    chpl__autoDestroy59(&_formal_tmp_x_chpl, _ln_chpl, _fn_chpl);
#line 268 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
    *(i__retArg_chpl) = ret_chpl2;
#line 268 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
    coerce_tmp_chpl3 = ret_tmp_chpl;
#line 268 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
    _formal_tmp_in_err_chpl = coerce_tmp_chpl3;
#line 268 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
    call_tmp_chpl7 = chpl_fix_thrown_error(&_formal_tmp_in_err_chpl, _ln_chpl, _fn_chpl);
#line 268 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
    *(error_out_chpl) = call_tmp_chpl7;
#line 268 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
    goto _end_daysInMonth_chpl;
#line 268 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  }
#line 269 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  if (month_chpl == INT64(2)) /* ZLINE: 269 /Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl */
#line 269 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  {
#line 269 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
    call_tmp_chpl10 = isLeapYear_chpl(year_chpl);
#line 269 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
    tmp_chpl2 = call_tmp_chpl10;
#line 269 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  } else /* ZLINE: 269 /Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl */
#line 269 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  {
#line 269 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
    tmp_chpl2 = UINT8(false);
#line 269 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  }
#line 269 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  if (tmp_chpl2) /* ZLINE: 269 /Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl */
#line 269 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  {
#line 270 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
    ret_chpl = INT64(29);
#line 270 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
    goto _end_daysInMonth_chpl;
#line 270 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  } else /* ZLINE: 269 /Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl */
#line 270 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  {
#line 272 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
    call_tmp_chpl11 = this_chpl19(&local_DAYS_IN_MONTH_chpl, month_chpl, _ln_chpl, _fn_chpl);
#line 272 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
    ret_chpl = *(call_tmp_chpl11);
#line 272 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
    goto _end_daysInMonth_chpl;
#line 272 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  }
#line 272 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  _end_daysInMonth_chpl:;
#line 266 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  return ret_chpl;
#line 266 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
}

#line 327 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
/* Time.chpl:327 */
#line 327 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
static void init_chpl225(date_chpl * this_chpl31,
#line 327 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
                         int64_t year_chpl,
#line 327 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
                         int64_t month_chpl,
#line 327 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
                         int64_t day_chpl2,
#line 327 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
                         int64_t _ln_chpl,
#line 327 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
                         int32_t _fn_chpl) {
#line 328 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  chpl_bool tmp_chpl;
#line 330 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  chpl_bool tmp_chpl2;
#line 332 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  int64_t call_tmp_chpl6;
#line 332 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  Error_chpl error_chpl = NULL;
#line 333 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  chpl_bool tmp_chpl3;
#line 328 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  if (year_chpl < INT64(0)) /* ZLINE: 328 /Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl */
#line 328 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  {
#line 328 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
    tmp_chpl = UINT8(true);
#line 328 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  } else /* ZLINE: 328 /Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl */
#line 328 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  {
#line 328 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
    tmp_chpl = (year_chpl > INT64(10000));
#line 328 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  }
#line 328 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  if (tmp_chpl) /* ZLINE: 328 /Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl */
#line 328 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  {
#line 329 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
    initHalt_chpl(&_str_literal_2163_chpl, _ln_chpl, _fn_chpl);
#line 329 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  }
#line 330 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  if (month_chpl < INT64(1)) /* ZLINE: 330 /Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl */
#line 330 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  {
#line 330 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
    tmp_chpl2 = UINT8(true);
#line 330 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  } else /* ZLINE: 330 /Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl */
#line 330 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  {
#line 330 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
    tmp_chpl2 = (month_chpl > INT64(12));
#line 330 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  }
#line 330 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  if (tmp_chpl2) /* ZLINE: 330 /Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl */
#line 330 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  {
#line 331 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
    initHalt_chpl(&_str_literal_2164_chpl, _ln_chpl, _fn_chpl);
#line 331 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  }
#line 332 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  error_chpl = NULL;
#line 332 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  call_tmp_chpl6 = daysInMonth_chpl(year_chpl, month_chpl, &error_chpl, _ln_chpl, _fn_chpl);
#line 332 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  if (error_chpl != nil) /* ZLINE: 332 /Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl */
#line 332 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  {
#line 332 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
    chpl_uncaught_error(error_chpl, _ln_chpl, _fn_chpl);
#line 332 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  }
#line 333 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  if (day_chpl2 < INT64(1)) /* ZLINE: 333 /Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl */
#line 333 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  {
#line 333 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
    tmp_chpl3 = UINT8(true);
#line 333 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  } else /* ZLINE: 333 /Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl */
#line 333 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  {
#line 333 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
    tmp_chpl3 = (day_chpl2 > call_tmp_chpl6);
#line 333 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  }
#line 333 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  if (tmp_chpl3) /* ZLINE: 333 /Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl */
#line 333 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  {
#line 334 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
    initHalt_chpl(&_str_literal_2165_chpl, _ln_chpl, _fn_chpl);
#line 334 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  }
#line 336 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  (this_chpl31)->chpl_year = year_chpl;
#line 337 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  (this_chpl31)->chpl_month = month_chpl;
#line 338 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  (this_chpl31)->chpl_day = day_chpl2;
#line 327 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  return;
#line 327 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
}

#line 686 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
/* Time.chpl:686 */
#line 686 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
static void init_chpl226(time_chpl * this_chpl31,
#line 686 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
                         int64_t hour_chpl,
#line 686 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
                         int64_t minute_chpl,
#line 686 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
                         int64_t second_chpl,
#line 686 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
                         int64_t microsecond_chpl,
#line 686 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
                         int64_t _ln_chpl,
#line 686 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
                         int32_t _fn_chpl) {
#line 687 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  chpl_bool tmp_chpl;
#line 689 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  chpl_bool tmp_chpl2;
#line 691 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  chpl_bool tmp_chpl3;
#line 693 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  chpl_bool tmp_chpl4;
#line 699 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  _shared_Timezone_chpl chpl_tz;
#line 687 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  if (hour_chpl < INT64(0)) /* ZLINE: 687 /Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl */
#line 687 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  {
#line 687 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
    tmp_chpl = UINT8(true);
#line 687 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  } else /* ZLINE: 687 /Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl */
#line 687 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  {
#line 687 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
    tmp_chpl = (hour_chpl >= INT64(24));
#line 687 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  }
#line 687 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  if (tmp_chpl) /* ZLINE: 687 /Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl */
#line 687 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  {
#line 688 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
    initHalt_chpl(&_str_literal_2206_chpl, _ln_chpl, _fn_chpl);
#line 688 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  }
#line 689 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  if (minute_chpl < INT64(0)) /* ZLINE: 689 /Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl */
#line 689 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  {
#line 689 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
    tmp_chpl2 = UINT8(true);
#line 689 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  } else /* ZLINE: 689 /Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl */
#line 689 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  {
#line 689 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
    tmp_chpl2 = (minute_chpl >= INT64(60));
#line 689 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  }
#line 689 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  if (tmp_chpl2) /* ZLINE: 689 /Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl */
#line 689 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  {
#line 690 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
    initHalt_chpl(&_str_literal_2207_chpl, _ln_chpl, _fn_chpl);
#line 690 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  }
#line 691 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  if (second_chpl < INT64(0)) /* ZLINE: 691 /Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl */
#line 691 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  {
#line 691 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
    tmp_chpl3 = UINT8(true);
#line 691 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  } else /* ZLINE: 691 /Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl */
#line 691 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  {
#line 691 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
    tmp_chpl3 = (second_chpl >= INT64(60));
#line 691 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  }
#line 691 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  if (tmp_chpl3) /* ZLINE: 691 /Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl */
#line 691 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  {
#line 692 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
    initHalt_chpl(&_str_literal_2209_chpl, _ln_chpl, _fn_chpl);
#line 692 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  }
#line 693 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  if (microsecond_chpl < INT64(0)) /* ZLINE: 693 /Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl */
#line 693 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  {
#line 693 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
    tmp_chpl4 = UINT8(true);
#line 693 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  } else /* ZLINE: 693 /Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl */
#line 693 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  {
#line 693 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
    tmp_chpl4 = (microsecond_chpl >= INT64(1000000));
#line 693 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  }
#line 693 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  if (tmp_chpl4) /* ZLINE: 693 /Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl */
#line 693 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  {
#line 694 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
    initHalt_chpl(&_str_literal_2210_chpl, _ln_chpl, _fn_chpl);
#line 694 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  }
#line 695 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  (this_chpl31)->chpl_hour = hour_chpl;
#line 696 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  (this_chpl31)->chpl_minute = minute_chpl;
#line 697 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  (this_chpl31)->chpl_second = second_chpl;
#line 698 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  (this_chpl31)->chpl_microsecond = microsecond_chpl;
#line 699 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  init_ASSIGN__chpl23(&chpl_tz, nil, _ln_chpl, _fn_chpl);
#line 699 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  (this_chpl31)->chpl_tz = chpl_tz;
#line 686 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  return;
#line 686 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
}

#line 707 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
/* Time.chpl:707 */
#line 707 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
static void deinit_chpl104(time_chpl * this_chpl31,
#line 707 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
                           int64_t _ln_chpl,
#line 707 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
                           int32_t _fn_chpl) {
#line 611 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  _ref__shared_Timezone_chpl _field_destructor_tmp__chpl = NULL;
#line 611 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  _field_destructor_tmp__chpl = &((this_chpl31)->chpl_tz);
#line 611 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  deinit_chpl94(_field_destructor_tmp__chpl, _ln_chpl, _fn_chpl);
#line 707 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  return;
#line 707 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
}

#line 1094 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
/* Time.chpl:1094 */
#line 1094 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
static void init_chpl227(dateTime_chpl * this_chpl31,
#line 1094 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
                         int64_t year_chpl,
#line 1094 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
                         int64_t month_chpl,
#line 1094 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
                         int64_t day_chpl2,
#line 1094 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
                         int64_t hour_chpl,
#line 1094 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
                         int64_t minute_chpl,
#line 1094 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
                         int64_t second_chpl,
#line 1094 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
                         int64_t microsecond_chpl,
#line 1094 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
                         int64_t _ln_chpl,
#line 1094 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
                         int32_t _fn_chpl) {
#line 1096 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  date_chpl call_tmp_chpl6;
#line 1096 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  date_chpl initTemp_chpl;
#line 1097 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  time_chpl call_tmp_chpl7;
#line 1097 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  time_chpl initTemp_chpl2;
#line 1096 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  init_chpl225(&initTemp_chpl, year_chpl, month_chpl, day_chpl2, _ln_chpl, _fn_chpl);
#line 1096 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  call_tmp_chpl6 = initTemp_chpl;
#line 1096 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  (this_chpl31)->chpl_date = call_tmp_chpl6;
#line 1097 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  init_chpl226(&initTemp_chpl2, hour_chpl, minute_chpl, second_chpl, microsecond_chpl, _ln_chpl, _fn_chpl);
#line 1097 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  call_tmp_chpl7 = initTemp_chpl2;
#line 1097 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  (this_chpl31)->chpl_time = call_tmp_chpl7;
#line 1094 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  return;
#line 1094 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
}

#line 1963 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
/* Time.chpl:1963 */
#line 1963 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
static void chpl__auto_destroy_Timezone(Timezone_chpl this_chpl31,
#line 1963 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
                                        int64_t _ln_chpl,
#line 1963 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
                                        int32_t _fn_chpl) {
#line 1963 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  return;
#line 1963 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
}

#line 2084 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
/* Time.chpl:2084 */
#line 2084 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
static void init_chpl228(stopwatch_chpl * this_chpl31,
#line 2084 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
                         _timevalue time_chpl2,
#line 2084 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
                         _real64 accumulated_chpl,
#line 2084 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
                         chpl_bool running_chpl) {
#line 2086 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  (this_chpl31)->time = time_chpl2;
#line 2089 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  (this_chpl31)->accumulated = accumulated_chpl;
#line 2092 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  (this_chpl31)->running = running_chpl;
#line 2084 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  return;
#line 2084 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
}

#line 2107 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
/* Time.chpl:2107 */
#line 2107 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
static void start_chpl(stopwatch_chpl * this_chpl31,
#line 2107 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
                       int64_t _ln_chpl,
#line 2107 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
                       int32_t _fn_chpl) {
#line 2110 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  _timevalue call_tmp_chpl6;
#line 2108 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  if (! (this_chpl31)->running) /* ZLINE: 2108 /Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl */
#line 2108 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  {
#line 2109 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
    (this_chpl31)->running = UINT8(true);
#line 2110 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
    call_tmp_chpl6 = chpl_now_timevalue();
#line 2110 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
    (this_chpl31)->time = call_tmp_chpl6;
#line 2110 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  } else /* ZLINE: 2111 /Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl */
#line 2110 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  {
#line 2112 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
    warning_chpl(&_str_literal_2266_chpl, _ln_chpl, _fn_chpl);
#line 2112 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  }
#line 2107 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
  return;
#line 2107 "/Users/iainmoncrief/Documents/chapel/modules/standard/Time.chpl"
}

