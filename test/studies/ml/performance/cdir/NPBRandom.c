#line 2607 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
/* Random.chpl:2607 */
#line 2607 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
static void chpl__init_NPBRandom(int64_t _ln_chpl,
#line 2607 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
                                 int32_t _fn_chpl) {
#line 2607 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  _ref_int32_t_chpl refIndentLevel_chpl = NULL;
#line 2607 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  if (chpl__init_NPBRandom_p) /* ZLINE: 2607 /Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl */
#line 2607 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  {
#line 2607 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    goto _exit_chpl__init_NPBRandom_chpl;
#line 2607 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  }
#line 2607 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  printModuleInit_chpl("%*s\n", "NPBRandom", INT64(9), _ln_chpl, _fn_chpl);
#line 2607 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  refIndentLevel_chpl = &moduleInitLevel_chpl;
#line 2607 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  *(refIndentLevel_chpl) += INT64(1);
#line 2607 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  chpl__init_NPBRandom_p = UINT8(true);
#line 2607 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  {
#line 2607 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    chpl__init_Random(_ln_chpl, _fn_chpl);
#line 2607 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    chpl__init_RandomSupport(_ln_chpl, _fn_chpl);
#line 2607 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    chpl__init_ChapelLocks(_ln_chpl, _fn_chpl);
#line 2607 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    chpl__init_IO(_ln_chpl, _fn_chpl);
#line 2607 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    chpl__init_HaltWrappers(_ln_chpl, _fn_chpl);
#line 2607 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    chpl__init_DSIUtil(_ln_chpl, _fn_chpl);
#line 2607 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  }
#line 2896 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  r23_chpl = pow(REAL64(0x1p-1), ((_real64)(INT64(23))));
#line 2897 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  t23_chpl = pow(REAL64(0x1p+1), ((_real64)(INT64(23))));
#line 2898 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  r46_chpl = pow(REAL64(0x1p-1), ((_real64)(INT64(46))));
#line 2899 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  t46_chpl = pow(REAL64(0x1p+1), ((_real64)(INT64(46))));
#line 2900 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  arand_chpl = REAL64(0x1.2309ce54p+30);
#line 2607 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  *(refIndentLevel_chpl) -= INT64(1);
#line 2607 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  _exit_chpl__init_NPBRandom_chpl:;
#line 2607 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  return;
#line 2607 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
}

#line 2617 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
/* Random.chpl:2617 */
#line 2617 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
static void chpl__auto_destroy_NPBRandomStream(NPBRandomStream__real64_T_chpl this_chpl31,
#line 2617 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
                                               int64_t _ln_chpl,
#line 2617 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
                                               int32_t _fn_chpl) {
#line 2869 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  _ref_chpl_LocalSpinlock_chpl _field_destructor_tmp__chpl = NULL;
#line 2869 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  _ref_AtomicBool_chpl _field_destructor_tmp__chpl2 = NULL;
#line 2869 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 2869 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  _field_destructor_tmp__chpl = &((this_chpl31)->_l);
#line 2869 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  _field_destructor_tmp__chpl2 = &((_field_destructor_tmp__chpl)->l);
#line 2869 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  deinit_chpl(_field_destructor_tmp__chpl2);
#line 2617 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  return;
#line 2617 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
}

#line 2663 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
/* Random.chpl:2663 */
#line 2663 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
static void init_chpl220(NPBRandomStream__real64_T_chpl this_chpl31,
#line 2663 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
                         int64_t seed_chpl,
#line 2663 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
                         int64_t _ln_chpl,
#line 2663 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
                         int32_t _fn_chpl) {
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  string_chpl call_tmp_chpl6;
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  string_chpl ret_tmp_chpl;
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  string_chpl call_tmp_chpl7;
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  string_chpl ret_tmp_chpl2;
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  string_chpl call_tmp_chpl8;
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  string_chpl ret_tmp_chpl3;
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  string_chpl call_tmp_chpl9;
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  string_chpl ret_tmp_chpl4;
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  string_chpl call_tmp_chpl10;
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  string_chpl ret_tmp_chpl5;
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  string_chpl call_tmp_chpl11;
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  string_chpl ret_tmp_chpl6;
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  string_chpl call_tmp_chpl12;
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  string_chpl ret_tmp_chpl7;
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  string_chpl call_tmp_chpl13;
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  string_chpl ret_tmp_chpl8;
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  string_chpl call_tmp_chpl14;
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  string_chpl ret_tmp_chpl9;
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  string_chpl msg_chpl;
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  _ref_string_chpl i_x_chpl2 = NULL;
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  _ref_string_chpl i_x_chpl3 = NULL;
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  _ref_string_chpl i_x_chpl4 = NULL;
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  _ref_string_chpl i_x_chpl5 = NULL;
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  _ref_string_chpl i_x_chpl6 = NULL;
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  _ref_string_chpl i_x_chpl7 = NULL;
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  _ref_string_chpl i_x_chpl8 = NULL;
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  _ref_string_chpl i_x_chpl9 = NULL;
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  string_chpl call_tmp_chpl15;
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  string_chpl ret_tmp_chpl10;
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  string_chpl call_tmp_chpl16;
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  string_chpl ret_tmp_chpl11;
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  string_chpl call_tmp_chpl17;
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  string_chpl ret_tmp_chpl12;
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  string_chpl call_tmp_chpl18;
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  string_chpl ret_tmp_chpl13;
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  string_chpl call_tmp_chpl19;
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  string_chpl ret_tmp_chpl14;
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  string_chpl call_tmp_chpl20;
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  string_chpl ret_tmp_chpl15;
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  string_chpl call_tmp_chpl21;
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  string_chpl ret_tmp_chpl16;
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  string_chpl call_tmp_chpl22;
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  string_chpl ret_tmp_chpl17;
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  string_chpl call_tmp_chpl23;
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  string_chpl ret_tmp_chpl18;
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  string_chpl call_tmp_chpl24;
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  string_chpl ret_tmp_chpl19;
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  string_chpl msg_chpl2;
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  _ref_string_chpl i_x_chpl10 = NULL;
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  _ref_string_chpl i_x_chpl11 = NULL;
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  _ref_string_chpl i_x_chpl12 = NULL;
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  _ref_string_chpl i_x_chpl13 = NULL;
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  _ref_string_chpl i_x_chpl14 = NULL;
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  _ref_string_chpl i_x_chpl15 = NULL;
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  _ref_string_chpl i_x_chpl16 = NULL;
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  _ref_string_chpl i_x_chpl17 = NULL;
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  _ref_string_chpl i_x_chpl18 = NULL;
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  _ref_string_chpl i_x_chpl19 = NULL;
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  uint64_t call_tmp_chpl25;
#line 2675 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  uint64_t call_tmp_chpl26;
#line 2676 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  uint64_t call_tmp_chpl27;
#line 2678 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  chpl_bool tmp_chpl;
#line 2682 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  int64_t call_tmp_chpl28;
#line 2684 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  chpl_LocalSpinlock _l_chpl;
#line 2684 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  _real64 init_coerce_tmp_chpl;
#line 2686 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  chpl_bool tmp_chpl2;
#line 2686 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  chpl_bool tmp_chpl3;
#line 2686 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  int64_t call_tmp_chpl29;
#line 2689 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  _real64 coerce_tmp_chpl3;
#line 2663 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 2663 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  ((RootClass_chpl)(&((this_chpl31)->super)))->chpl__cid = chpl__cid_RootClass_chpl;
#line 2663 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  ((RootClass_chpl)(((RootClass_chpl)(this_chpl31))))->chpl__cid = chpl__cid_RootClass_chpl;
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  if (INT64(46) < INT64(0)) /* ZLINE: 2674 /Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl */
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  {
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    chpl___COLON_24(UINT64(1), &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    call_tmp_chpl6 = ret_tmp_chpl;
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    chpl___PLUS_(&_str_literal_45_chpl, &call_tmp_chpl6, &ret_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    call_tmp_chpl7 = ret_tmp_chpl2;
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    chpl___PLUS_(&call_tmp_chpl7, &_str_literal_46_chpl, &ret_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    call_tmp_chpl8 = ret_tmp_chpl3;
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    chpl___COLON_25(INT64(46), &ret_tmp_chpl4, _ln_chpl, _fn_chpl);
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    call_tmp_chpl9 = ret_tmp_chpl4;
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    chpl___PLUS_(&call_tmp_chpl8, &call_tmp_chpl9, &ret_tmp_chpl5, _ln_chpl, _fn_chpl);
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    call_tmp_chpl10 = ret_tmp_chpl5;
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    chpl___PLUS_(&call_tmp_chpl10, &_str_literal_47_chpl, &ret_tmp_chpl6, _ln_chpl, _fn_chpl);
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    call_tmp_chpl11 = ret_tmp_chpl6;
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    chpl___COLON_25(INT64(46), &ret_tmp_chpl7, _ln_chpl, _fn_chpl);
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    call_tmp_chpl12 = ret_tmp_chpl7;
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    chpl___PLUS_(&call_tmp_chpl11, &call_tmp_chpl12, &ret_tmp_chpl8, _ln_chpl, _fn_chpl);
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    call_tmp_chpl13 = ret_tmp_chpl8;
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    chpl___PLUS_(&call_tmp_chpl13, &_str_literal_48_chpl, &ret_tmp_chpl9, _ln_chpl, _fn_chpl);
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    call_tmp_chpl14 = ret_tmp_chpl9;
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    msg_chpl = call_tmp_chpl14;
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    i_x_chpl = &call_tmp_chpl13;
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    deinit_chpl95(i_x_chpl, _ln_chpl, _fn_chpl);
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    i_x_chpl2 = &call_tmp_chpl12;
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    deinit_chpl95(i_x_chpl2, _ln_chpl, _fn_chpl);
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    i_x_chpl3 = &call_tmp_chpl11;
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    deinit_chpl95(i_x_chpl3, _ln_chpl, _fn_chpl);
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    i_x_chpl4 = &call_tmp_chpl10;
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    deinit_chpl95(i_x_chpl4, _ln_chpl, _fn_chpl);
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    i_x_chpl5 = &call_tmp_chpl9;
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    deinit_chpl95(i_x_chpl5, _ln_chpl, _fn_chpl);
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    i_x_chpl6 = &call_tmp_chpl8;
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    deinit_chpl95(i_x_chpl6, _ln_chpl, _fn_chpl);
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    i_x_chpl7 = &call_tmp_chpl7;
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    deinit_chpl95(i_x_chpl7, _ln_chpl, _fn_chpl);
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    i_x_chpl8 = &call_tmp_chpl6;
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    deinit_chpl95(i_x_chpl8, _ln_chpl, _fn_chpl);
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    boundsCheckHalt_chpl(&msg_chpl, _ln_chpl, _fn_chpl);
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    i_x_chpl9 = &msg_chpl;
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    deinit_chpl95(i_x_chpl9, _ln_chpl, _fn_chpl);
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  } else /* ZLINE: 2674 /Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl */
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  if (INT64(46) >= INT64(64)) /* ZLINE: 2674 /Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl */
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  {
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    chpl___COLON_24(UINT64(1), &ret_tmp_chpl10, _ln_chpl, _fn_chpl);
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    call_tmp_chpl15 = ret_tmp_chpl10;
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    chpl___PLUS_(&_str_literal_45_chpl, &call_tmp_chpl15, &ret_tmp_chpl11, _ln_chpl, _fn_chpl);
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    call_tmp_chpl16 = ret_tmp_chpl11;
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    chpl___PLUS_(&call_tmp_chpl16, &_str_literal_46_chpl, &ret_tmp_chpl12, _ln_chpl, _fn_chpl);
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    call_tmp_chpl17 = ret_tmp_chpl12;
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    chpl___COLON_25(INT64(46), &ret_tmp_chpl13, _ln_chpl, _fn_chpl);
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    call_tmp_chpl18 = ret_tmp_chpl13;
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    chpl___PLUS_(&call_tmp_chpl17, &call_tmp_chpl18, &ret_tmp_chpl14, _ln_chpl, _fn_chpl);
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    call_tmp_chpl19 = ret_tmp_chpl14;
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    chpl___PLUS_(&call_tmp_chpl19, &_str_literal_47_chpl, &ret_tmp_chpl15, _ln_chpl, _fn_chpl);
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    call_tmp_chpl20 = ret_tmp_chpl15;
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    chpl___COLON_25(INT64(46), &ret_tmp_chpl16, _ln_chpl, _fn_chpl);
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    call_tmp_chpl21 = ret_tmp_chpl16;
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    chpl___PLUS_(&call_tmp_chpl20, &call_tmp_chpl21, &ret_tmp_chpl17, _ln_chpl, _fn_chpl);
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    call_tmp_chpl22 = ret_tmp_chpl17;
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    chpl___PLUS_(&call_tmp_chpl22, &_str_literal_50_chpl, &ret_tmp_chpl18, _ln_chpl, _fn_chpl);
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    call_tmp_chpl23 = ret_tmp_chpl18;
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    chpl___PLUS_(&call_tmp_chpl23, &_str_literal_3965_chpl, &ret_tmp_chpl19, _ln_chpl, _fn_chpl);
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    call_tmp_chpl24 = ret_tmp_chpl19;
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    msg_chpl2 = call_tmp_chpl24;
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    i_x_chpl10 = &call_tmp_chpl23;
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    deinit_chpl95(i_x_chpl10, _ln_chpl, _fn_chpl);
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    i_x_chpl11 = &call_tmp_chpl22;
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    deinit_chpl95(i_x_chpl11, _ln_chpl, _fn_chpl);
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    i_x_chpl12 = &call_tmp_chpl21;
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    deinit_chpl95(i_x_chpl12, _ln_chpl, _fn_chpl);
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    i_x_chpl13 = &call_tmp_chpl20;
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    deinit_chpl95(i_x_chpl13, _ln_chpl, _fn_chpl);
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    i_x_chpl14 = &call_tmp_chpl19;
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    deinit_chpl95(i_x_chpl14, _ln_chpl, _fn_chpl);
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    i_x_chpl15 = &call_tmp_chpl18;
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    deinit_chpl95(i_x_chpl15, _ln_chpl, _fn_chpl);
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    i_x_chpl16 = &call_tmp_chpl17;
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    deinit_chpl95(i_x_chpl16, _ln_chpl, _fn_chpl);
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    i_x_chpl17 = &call_tmp_chpl16;
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    deinit_chpl95(i_x_chpl17, _ln_chpl, _fn_chpl);
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    i_x_chpl18 = &call_tmp_chpl15;
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    deinit_chpl95(i_x_chpl18, _ln_chpl, _fn_chpl);
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    boundsCheckHalt_chpl(&msg_chpl2, _ln_chpl, _fn_chpl);
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    i_x_chpl19 = &msg_chpl2;
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    deinit_chpl95(i_x_chpl19, _ln_chpl, _fn_chpl);
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  }
#line 2674 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  call_tmp_chpl25 = (UINT64(1) << INT64(46));
#line 2675 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  call_tmp_chpl26 = (call_tmp_chpl25 - UINT64(1));
#line 2676 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  call_tmp_chpl27 = ((uint64_t)(seed_chpl));
#line 2678 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  if (! (INT64(0) < INT64(0))) /* ZLINE: 2678 /Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl */
#line 2678 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  {
#line 2678 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    tmp_chpl = (((uint64_t)((call_tmp_chpl27 % UINT64(2)))) == ((uint64_t)(INT64(0))));
#line 2678 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  } else /* ZLINE: 2678 /Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl */
#line 2678 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  {
#line 2678 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    tmp_chpl = UINT8(false);
#line 2678 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  }
#line 2678 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  if (tmp_chpl) /* ZLINE: 2678 /Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl */
#line 2678 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  {
#line 2679 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    initHalt_chpl(&_str_literal_2353_chpl, _ln_chpl, _fn_chpl);
#line 2679 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  }
#line 2682 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  call_tmp_chpl28 = ((int64_t)((call_tmp_chpl27 & call_tmp_chpl26)));
#line 2682 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 2682 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  (this_chpl31)->seed = call_tmp_chpl28;
#line 2684 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  init_chpl105(&_l_chpl);
#line 2684 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 2684 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  (this_chpl31)->_l = _l_chpl;
#line 2684 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 2684 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  init_coerce_tmp_chpl = ((_real64)((this_chpl31)->seed));
#line 2684 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 2684 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  (this_chpl31)->NPBRandomStreamPrivate_cursor = init_coerce_tmp_chpl;
#line 2684 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 2684 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  (this_chpl31)->NPBRandomStreamPrivate_count = INT64(1);
#line 2663 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  ((RootClass_chpl)(this_chpl31))->chpl__cid = chpl__cid_NPBRandomStream__real64_T_chpl;
#line 2686 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 2686 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  if (((int64_t)(((this_chpl31)->seed % INT64(2)))) == INT64(0)) /* ZLINE: 2686 /Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl */
#line 2686 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  {
#line 2686 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    tmp_chpl2 = UINT8(true);
#line 2686 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  } else /* ZLINE: 2686 /Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl */
#line 2686 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  {
#line 2686 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 2686 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    tmp_chpl2 = ((this_chpl31)->seed < INT64(1));
#line 2686 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  }
#line 2686 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  if (tmp_chpl2) /* ZLINE: 2686 /Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl */
#line 2686 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  {
#line 2686 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    tmp_chpl3 = UINT8(true);
#line 2686 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  } else /* ZLINE: 2686 /Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl */
#line 2686 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  {
#line 2686 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    call_tmp_chpl29 = ((int64_t)(call_tmp_chpl25));
#line 2686 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 2686 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    tmp_chpl3 = ((this_chpl31)->seed > call_tmp_chpl29);
#line 2686 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  }
#line 2686 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  if (tmp_chpl3) /* ZLINE: 2686 /Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl */
#line 2686 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  {
#line 2687 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    initHalt_chpl(&_str_literal_2354_chpl, _ln_chpl, _fn_chpl);
#line 2687 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  }
#line 2689 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  coerce_tmp_chpl3 = ((_real64)(seed_chpl));
#line 2689 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 2689 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  (this_chpl31)->NPBRandomStreamPrivate_cursor = coerce_tmp_chpl3;
#line 2690 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 2690 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  (this_chpl31)->NPBRandomStreamPrivate_count = INT64(1);
#line 2663 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  return;
#line 2663 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
}

#line 2663 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
/* Random.chpl:2663 */
#line 2663 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
static NPBRandomStream__real64_T_chpl _new_chpl66(int64_t seed_chpl,
#line 2663 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
                                                  int64_t _ln_chpl,
#line 2663 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
                                                  int32_t _fn_chpl) {
#line 2663 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  NPBRandomStream__real64_T_chpl initTemp_chpl = NULL;
#line 2663 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  c_ptr_void_chpl cast_tmp_chpl = NULL;
#line 2663 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  NPBRandomStream__real64_T_chpl tmp_chpl = NULL;
#line 2663 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  cast_tmp_chpl = chpl_here_alloc(sizeof(chpl_NPBRandomStream__real64_T_chpl_object), INT16(56), _ln_chpl, _fn_chpl);
#line 2663 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  initTemp_chpl = ((NPBRandomStream__real64_T_chpl)(cast_tmp_chpl));
#line 2663 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  ((RootClass_chpl)(initTemp_chpl))->chpl__cid = chpl__cid_NPBRandomStream__real64_T_chpl;
#line 2663 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  chpl_check_nil(initTemp_chpl, _ln_chpl, _fn_chpl);
#line 2663 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  init_chpl220(initTemp_chpl, seed_chpl, _ln_chpl, _fn_chpl);
#line 2663 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  tmp_chpl = initTemp_chpl;
#line 2663 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  return tmp_chpl;
#line 2663 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
}

#line 2699 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
/* Random.chpl:2699 */
#line 2699 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
static _real64 NPBRandomStreamPrivate_getNext_noLock_chpl(NPBRandomStream__real64_T_chpl this_chpl31,
#line 2699 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
                                                          int64_t _ln_chpl,
#line 2699 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
                                                          int32_t _fn_chpl) {
#line 2703 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  _ref_int64_t_chpl call_tmp_chpl6 = NULL;
#line 2705 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  _real64 call_tmp_chpl7;
#line 2705 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  _real64 _formal_tmp_in_x_chpl;
#line 2703 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 2703 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  call_tmp_chpl6 = &((this_chpl31)->NPBRandomStreamPrivate_count);
#line 2703 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  *(call_tmp_chpl6) += INT64(1);
#line 2705 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 2705 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  _formal_tmp_in_x_chpl = (this_chpl31)->NPBRandomStreamPrivate_cursor;
#line 2705 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  call_tmp_chpl7 = randlc_chpl2(&_formal_tmp_in_x_chpl);
#line 2705 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 2705 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  (this_chpl31)->NPBRandomStreamPrivate_cursor = _formal_tmp_in_x_chpl;
#line 2699 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  return call_tmp_chpl7;
#line 2699 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
}

#line 2724 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
/* Random.chpl:2724 */
#line 2724 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
static _real64 getNext_chpl(NPBRandomStream__real64_T_chpl this_chpl31,
#line 2724 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
                            int64_t _ln_chpl,
#line 2724 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
                            int32_t _fn_chpl) {
#line 2725 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  _ref_chpl_LocalSpinlock_chpl call_tmp_chpl6 = NULL;
#line 2725 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  chpl_bool tmp_chpl;
#line 2725 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  _ref_AtomicBool_chpl call_tmp_chpl7 = NULL;
#line 2725 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  _ref_atomic_bool_chpl call_tmp_chpl8 = NULL;
#line 2725 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  memory_order ret_chpl;
#line 2725 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  chpl_bool call_tmp_chpl9;
#line 2725 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  chpl_bool tmp_chpl2;
#line 2725 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  _ref_AtomicBool_chpl call_tmp_chpl10 = NULL;
#line 2725 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  _ref_atomic_bool_chpl call_tmp_chpl11 = NULL;
#line 2725 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  memory_order ret_chpl2;
#line 2725 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  chpl_bool call_tmp_chpl12;
#line 2725 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  _ref_AtomicBool_chpl call_tmp_chpl13 = NULL;
#line 2725 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  _ref_atomic_bool_chpl call_tmp_chpl14 = NULL;
#line 2725 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  memory_order ret_chpl3;
#line 2725 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  chpl_bool call_tmp_chpl15;
#line 2725 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  chpl_bool tmp_chpl3;
#line 2725 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  _ref_AtomicBool_chpl call_tmp_chpl16 = NULL;
#line 2725 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  _ref_atomic_bool_chpl call_tmp_chpl17 = NULL;
#line 2725 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  memory_order ret_chpl4;
#line 2725 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  chpl_bool call_tmp_chpl18;
#line 2726 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  _real64 call_tmp_chpl19;
#line 2727 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  _ref_chpl_LocalSpinlock_chpl call_tmp_chpl20 = NULL;
#line 2727 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  _ref_AtomicBool_chpl call_tmp_chpl21 = NULL;
#line 2727 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  _ref_atomic_bool_chpl call_tmp_chpl22 = NULL;
#line 2727 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  memory_order ret_chpl5;
#line 2725 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 2725 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  call_tmp_chpl6 = &((this_chpl31)->_l);
#line 2725 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  call_tmp_chpl7 = &((call_tmp_chpl6)->l);
#line 2725 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  chpl_rmem_consist_maybe_release2(_ln_chpl, _fn_chpl);
#line 2725 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  call_tmp_chpl8 = &((call_tmp_chpl7)->_v);
#line 2725 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  ret_chpl = memory_order_seq_cst;
#line 2725 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  call_tmp_chpl9 = atomic_load_explicit_bool(call_tmp_chpl8, ret_chpl);
#line 2725 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  if (call_tmp_chpl9) /* ZLINE: 2725 /Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl */
#line 2725 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  {
#line 2725 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    tmp_chpl2 = UINT8(true);
#line 2725 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  } else /* ZLINE: 2725 /Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl */
#line 2725 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  {
#line 2725 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    call_tmp_chpl10 = &((call_tmp_chpl6)->l);
#line 2725 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    chpl_rmem_consist_maybe_release5(_ln_chpl, _fn_chpl);
#line 2725 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    chpl_rmem_consist_maybe_release5(_ln_chpl, _fn_chpl);
#line 2725 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    call_tmp_chpl11 = &((call_tmp_chpl10)->_v);
#line 2725 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    ret_chpl2 = memory_order_acquire;
#line 2725 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    call_tmp_chpl12 = atomic_exchange_explicit_bool(call_tmp_chpl11, UINT8(true), ret_chpl2);
#line 2725 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    chpl_rmem_consist_maybe_acquire5(_ln_chpl, _fn_chpl);
#line 2725 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    tmp_chpl2 = call_tmp_chpl12;
#line 2725 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  }
#line 2725 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  tmp_chpl = tmp_chpl2;
#line 2725 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  while (tmp_chpl) {
#line 2725 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    chpl_task_yield();
#line 2725 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    call_tmp_chpl13 = &((call_tmp_chpl6)->l);
#line 2725 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    chpl_rmem_consist_maybe_release2(_ln_chpl, _fn_chpl);
#line 2725 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    call_tmp_chpl14 = &((call_tmp_chpl13)->_v);
#line 2725 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    ret_chpl3 = memory_order_seq_cst;
#line 2725 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    call_tmp_chpl15 = atomic_load_explicit_bool(call_tmp_chpl14, ret_chpl3);
#line 2725 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    if (call_tmp_chpl15) /* ZLINE: 2725 /Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl */
#line 2725 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    {
#line 2725 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
      tmp_chpl3 = UINT8(true);
#line 2725 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    } else /* ZLINE: 2725 /Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl */
#line 2725 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    {
#line 2725 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
      call_tmp_chpl16 = &((call_tmp_chpl6)->l);
#line 2725 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
      chpl_rmem_consist_maybe_release5(_ln_chpl, _fn_chpl);
#line 2725 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
      chpl_rmem_consist_maybe_release5(_ln_chpl, _fn_chpl);
#line 2725 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
      call_tmp_chpl17 = &((call_tmp_chpl16)->_v);
#line 2725 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
      ret_chpl4 = memory_order_acquire;
#line 2725 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
      call_tmp_chpl18 = atomic_exchange_explicit_bool(call_tmp_chpl17, UINT8(true), ret_chpl4);
#line 2725 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
      chpl_rmem_consist_maybe_acquire5(_ln_chpl, _fn_chpl);
#line 2725 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
      tmp_chpl3 = call_tmp_chpl18;
#line 2725 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    }
#line 2725 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    tmp_chpl = tmp_chpl3;
#line 2725 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  }
#line 2726 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 2726 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  call_tmp_chpl19 = NPBRandomStreamPrivate_getNext_noLock_chpl(this_chpl31, _ln_chpl, _fn_chpl);
#line 2727 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 2727 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  call_tmp_chpl20 = &((this_chpl31)->_l);
#line 2727 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  call_tmp_chpl21 = &((call_tmp_chpl20)->l);
#line 2727 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  chpl_rmem_consist_maybe_release4(_ln_chpl, _fn_chpl);
#line 2727 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  chpl_rmem_consist_maybe_release4(_ln_chpl, _fn_chpl);
#line 2727 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  call_tmp_chpl22 = &((call_tmp_chpl21)->_v);
#line 2727 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  ret_chpl5 = memory_order_release;
#line 2727 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  atomic_store_explicit_bool(call_tmp_chpl22, UINT8(false), ret_chpl5);
#line 2727 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  chpl_rmem_consist_maybe_acquire4(_ln_chpl, _fn_chpl);
#line 2727 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  chpl_rmem_consist_maybe_acquire4(_ln_chpl, _fn_chpl);
#line 2724 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  return call_tmp_chpl19;
#line 2724 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
}

#line 2907 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
/* Random.chpl:2907 */
#line 2907 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
static _real64 randlc_chpl(_real64 * x_chpl,
#line 2907 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
                           _real64 a_chpl) {
#line 2919 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  _real64 local_t46_chpl;
#line 2918 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  _real64 local_r46_chpl;
#line 2910 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  _real64 local_t23_chpl;
#line 2908 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  _real64 local_r23_chpl;
#line 2909 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  _real64 call_tmp_chpl6;
#line 2910 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  _real64 call_tmp_chpl7;
#line 2912 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  _real64 call_tmp_chpl8;
#line 2913 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  _real64 call_tmp_chpl9;
#line 2914 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  _real64 call_tmp_chpl10;
#line 2915 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  _real64 call_tmp_chpl11;
#line 2917 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  _real64 call_tmp_chpl12;
#line 2918 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  _real64 call_tmp_chpl13;
#line 2919 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  _real64 call_tmp_chpl14;
#line 2921 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  _real64 call_tmp_chpl15;
#line 2919 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  local_t46_chpl = t46_chpl;
#line 2918 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  local_r46_chpl = r46_chpl;
#line 2910 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  local_t23_chpl = t23_chpl;
#line 2908 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  local_r23_chpl = r23_chpl;
#line 2909 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  call_tmp_chpl6 = floor(((_real64)((local_r23_chpl * a_chpl))));
#line 2910 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  call_tmp_chpl7 = (a_chpl - ((_real64)((local_t23_chpl * call_tmp_chpl6))));
#line 2912 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  call_tmp_chpl8 = floor(((_real64)((local_r23_chpl * *(x_chpl)))));
#line 2913 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  call_tmp_chpl9 = (*(x_chpl) - ((_real64)((local_t23_chpl * call_tmp_chpl8))));
#line 2914 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  call_tmp_chpl10 = (((_real64)((call_tmp_chpl6 * call_tmp_chpl9))) + ((_real64)((call_tmp_chpl7 * call_tmp_chpl8))));
#line 2915 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  call_tmp_chpl11 = floor(((_real64)((local_r23_chpl * call_tmp_chpl10))));
#line 2917 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  call_tmp_chpl12 = (((_real64)((local_t23_chpl * ((_real64)((call_tmp_chpl10 - ((_real64)((local_t23_chpl * call_tmp_chpl11))))))))) + ((_real64)((call_tmp_chpl7 * call_tmp_chpl9))));
#line 2918 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  call_tmp_chpl13 = floor(((_real64)((local_r46_chpl * call_tmp_chpl12))));
#line 2919 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  call_tmp_chpl14 = (call_tmp_chpl12 - ((_real64)((local_t46_chpl * call_tmp_chpl13))));
#line 2920 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  *(x_chpl) = call_tmp_chpl14;
#line 2921 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  call_tmp_chpl15 = (local_r46_chpl * call_tmp_chpl14);
#line 2907 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  return call_tmp_chpl15;
#line 2907 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
}

#line 2925 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
/* Random.chpl:2925 */
#line 2925 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
static _real64 randlc_chpl2(_real64 * x_chpl) {
#line 2935 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  _real64 local_arand_chpl;
#line 2935 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  _real64 call_tmp_chpl6;
#line 2935 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  _real64 _formal_tmp_in_x_chpl;
#line 2935 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  local_arand_chpl = arand_chpl;
#line 2935 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  _formal_tmp_in_x_chpl = *(x_chpl);
#line 2935 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  call_tmp_chpl6 = randlc_chpl(&_formal_tmp_in_x_chpl, local_arand_chpl);
#line 2935 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  *(x_chpl) = _formal_tmp_in_x_chpl;
#line 2925 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  return call_tmp_chpl6;
#line 2925 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
}

