#line 1 "../lib/MNISTTester.chpl"
/* MNISTTester.chpl:1 */
#line 1 "../lib/MNISTTester.chpl"
static void chpl__init_MNISTTester(int64_t _ln_chpl,
#line 1 "../lib/MNISTTester.chpl"
                                   int32_t _fn_chpl) {
#line 1 "../lib/MNISTTester.chpl"
  _ref_int32_t_chpl refIndentLevel_chpl = NULL;
#line 13 "../lib/MNISTTester.chpl"
  string_chpl tmp_chpl;
#line 13 "../lib/MNISTTester.chpl"
  chpl_bool call_tmp_chpl6;
#line 13 "../lib/MNISTTester.chpl"
  c_ptrConst_int8_t_chpl call_tmp_chpl7 = NULL;
#line 13 "../lib/MNISTTester.chpl"
  string_chpl ret_tmp_chpl;
#line 1 "../lib/MNISTTester.chpl"
  if (chpl__init_MNISTTester_p) /* ZLINE: 1 ../lib/MNISTTester.chpl */
#line 1 "../lib/MNISTTester.chpl"
  {
#line 1 "../lib/MNISTTester.chpl"
    goto _exit_chpl__init_MNISTTester_chpl;
#line 1 "../lib/MNISTTester.chpl"
  }
#line 1 "../lib/MNISTTester.chpl"
  printModuleInit_chpl("%*s\n", "MNISTTester", INT64(11), INT64(1), INT32(66));
#line 1 "../lib/MNISTTester.chpl"
  refIndentLevel_chpl = &moduleInitLevel_chpl;
#line 1 "../lib/MNISTTester.chpl"
  *(refIndentLevel_chpl) += INT64(1);
#line 1 "../lib/MNISTTester.chpl"
  chpl__init_MNISTTester_p = UINT8(true);
#line 1 "../lib/MNISTTester.chpl"
  {
#line 1 "../lib/MNISTTester.chpl"
    chpl__init_Tensor(INT64(1), INT32(66));
#line 1 "../lib/MNISTTester.chpl"
    chpl__init_MNIST(INT64(1), INT32(66));
#line 1 "../lib/MNISTTester.chpl"
    chpl__init_Time(INT64(1), INT32(66));
#line 1 "../lib/MNISTTester.chpl"
    chpl__init_IO(INT64(1), INT32(66));
#line 1 "../lib/MNISTTester.chpl"
    chpl__init_Chai(INT64(1), INT32(66));
#line 1 "../lib/MNISTTester.chpl"
    chpl__init_CTypes(INT64(1), INT32(66));
#line 1 "../lib/MNISTTester.chpl"
    chpl__init_AutoMath(INT64(1), INT32(66));
#line 1 "../lib/MNISTTester.chpl"
  }
#line 1 "../lib/MNISTTester.chpl"
  chpl_addModule("MNISTTester", ((c_fn_ptr)(chpl__deinit_MNISTTester)), INT64(1), INT32(66));
#line 11 "../lib/MNISTTester.chpl"
  seedRandom_chpl(INT64(0));
#line 13 "../lib/MNISTTester.chpl"
  call_tmp_chpl6 = chpl_config_has_value(((c_ptrConst_int8_t_chpl)("dataPath")), ((c_ptrConst_int8_t_chpl)("MNISTTester")));
#line 13 "../lib/MNISTTester.chpl"
  if (! call_tmp_chpl6) /* ZLINE: 13 ../lib/MNISTTester.chpl */
#line 13 "../lib/MNISTTester.chpl"
  {
#line 13 "../lib/MNISTTester.chpl"
    tmp_chpl = _str_literal_1740_chpl;
#line 13 "../lib/MNISTTester.chpl"
  } else /* ZLINE: 13 ../lib/MNISTTester.chpl */
#line 13 "../lib/MNISTTester.chpl"
  {
#line 13 "../lib/MNISTTester.chpl"
    call_tmp_chpl7 = chpl_config_get_value(((c_ptrConst_int8_t_chpl)("dataPath")), ((c_ptrConst_int8_t_chpl)("MNISTTester")));
#line 13 "../lib/MNISTTester.chpl"
    _command_line_cast_chpl14(call_tmp_chpl7, &ret_tmp_chpl, INT64(0), INT32(104));
#line 13 "../lib/MNISTTester.chpl"
    tmp_chpl = ret_tmp_chpl;
#line 13 "../lib/MNISTTester.chpl"
  }
#line 13 "../lib/MNISTTester.chpl"
  dataPath_chpl = tmp_chpl;
#line 1 "../lib/MNISTTester.chpl"
  *(refIndentLevel_chpl) -= INT64(1);
#line 1 "../lib/MNISTTester.chpl"
  _exit_chpl__init_MNISTTester_chpl:;
#line 1 "../lib/MNISTTester.chpl"
  return;
#line 1 "../lib/MNISTTester.chpl"
}

#line 1 "../lib/MNISTTester.chpl"
/* MNISTTester.chpl:1 */
#line 1 "../lib/MNISTTester.chpl"
static void chpl__deinit_MNISTTester(void) {
#line 13 "../lib/MNISTTester.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 13 "../lib/MNISTTester.chpl"
  i_x_chpl = &dataPath_chpl;
#line 13 "../lib/MNISTTester.chpl"
  deinit_chpl95(i_x_chpl, INT64(13), INT32(66));
#line 1 "../lib/MNISTTester.chpl"
  return;
#line 1 "../lib/MNISTTester.chpl"
}

#line 15 "../lib/MNISTTester.chpl"
/* MNISTTester.chpl:15 */
#line 15 "../lib/MNISTTester.chpl"
static void forward_chpl(Network__tuple_4_Conv_Conv_MaxPool_SoftMax_chpl * net_chpl2,
#line 15 "../lib/MNISTTester.chpl"
                         Tensor_3__real64_chpl * x_chpl,
#line 15 "../lib/MNISTTester.chpl"
                         int64_t lb_chpl,
#line 15 "../lib/MNISTTester.chpl"
                         _tuple_3_Tensor_1__real64__real64_chpl_bool_chpl * _retArg_chpl) {
#line 16 "../lib/MNISTTester.chpl"
  Tensor_1__real64_chpl call_tmp_chpl6;
#line 16 "../lib/MNISTTester.chpl"
  Tensor_1__real64_chpl ret_tmp_chpl;
#line 16 "../lib/MNISTTester.chpl"
  Tensor_1__real64_chpl output_chpl;
#line 17 "../lib/MNISTTester.chpl"
  _ref__array_DefaultRectangularArr_1_int64_t_one__real64_int64_t_chpl call_tmp_chpl7 = NULL;
#line 17 "../lib/MNISTTester.chpl"
  _tuple_1_star_int64_t_chpl coerce_tmp_chpl3;
#line 17 "../lib/MNISTTester.chpl"
  _ref__tuple_1_star_int64_t_chpl i_i_chpl = NULL;
#line 17 "../lib/MNISTTester.chpl"
  DefaultRectangularArr_1_int64_t_one__real64_int64_t_chpl ret_chpl = NULL;
#line 17 "../lib/MNISTTester.chpl"
  DefaultRectangularArr_1_int64_t_one__real64_int64_t_chpl coerce_tmp_chpl4 = NULL;
#line 17 "../lib/MNISTTester.chpl"
  _tuple_1_star_int64_t_chpl _formal_tmp_in_ind_chpl;
#line 17 "../lib/MNISTTester.chpl"
  int64_t _formal_tmp_ind_x0_chpl;
#line 17 "../lib/MNISTTester.chpl"
  _ddata__real64_chpl coerce_tmp_chpl5 = NULL;
#line 17 "../lib/MNISTTester.chpl"
  _ref__real64_chpl call_tmp_chpl8 = NULL;
#line 17 "../lib/MNISTTester.chpl"
  _real64 call_tmp_chpl9;
#line 17 "../lib/MNISTTester.chpl"
  _real64 call_tmp_chpl10;
#line 18 "../lib/MNISTTester.chpl"
  _ref__array_DefaultRectangularArr_1_int64_t_one__real64_int64_t_chpl call_tmp_chpl11 = NULL;
#line 18 "../lib/MNISTTester.chpl"
  int64_t call_tmp_chpl12;
#line 18 "../lib/MNISTTester.chpl"
  chpl_bool call_tmp_chpl13;
#line 19 "../lib/MNISTTester.chpl"
  _tuple_3__ref_Tensor_1__real64__real64_chpl_bool_chpl call_tmp_chpl14;
#line 19 "../lib/MNISTTester.chpl"
  _tuple_3__ref_Tensor_1__real64__real64_chpl_bool_chpl ret_tmp_chpl2;
#line 19 "../lib/MNISTTester.chpl"
  _ref_Tensor_1__real64_chpl i__e0_x_chpl = NULL;
#line 19 "../lib/MNISTTester.chpl"
  _ref_Tensor_1__real64_chpl copy_ret_tmp_x0_chpl = NULL;
#line 19 "../lib/MNISTTester.chpl"
  _real64 copy_ret_tmp_x1_chpl;
#line 19 "../lib/MNISTTester.chpl"
  chpl_bool copy_ret_tmp_x2_chpl;
#line 19 "../lib/MNISTTester.chpl"
  _tuple_3_Tensor_1__real64__real64_chpl_bool_chpl ret_chpl2;
#line 19 "../lib/MNISTTester.chpl"
  Tensor_1__real64_chpl ret_chpl3;
#line 19 "../lib/MNISTTester.chpl"
  Tensor_1__real64_chpl tmp_chpl;
#line 19 "../lib/MNISTTester.chpl"
  _array_DefaultRectangularArr_1_int64_t_one__real64_int64_t_chpl _field_destructor_tmp__chpl;
#line 19 "../lib/MNISTTester.chpl"
  _domain_DefaultRectangularDom_1_int64_t_one_chpl _field_destructor_tmp__chpl2;
#line 16 "../lib/MNISTTester.chpl"
  forwardProp_chpl4(net_chpl2, x_chpl, &ret_tmp_chpl);
#line 16 "../lib/MNISTTester.chpl"
  call_tmp_chpl6 = ret_tmp_chpl;
#line 16 "../lib/MNISTTester.chpl"
  output_chpl = call_tmp_chpl6;
#line 17 "../lib/MNISTTester.chpl"
  call_tmp_chpl7 = &((&output_chpl)->data);
#line 17 "../lib/MNISTTester.chpl"
  *(coerce_tmp_chpl3 + INT64(0)) = lb_chpl;
#line 17 "../lib/MNISTTester.chpl"
  i_i_chpl = &coerce_tmp_chpl3;
#line 17 "../lib/MNISTTester.chpl"
  ret_chpl = (call_tmp_chpl7)->_instance;
#line 17 "../lib/MNISTTester.chpl"
  checkAccess_chpl11(call_tmp_chpl7, i_i_chpl, ret_chpl, INT64(17), INT32(66));
#line 17 "../lib/MNISTTester.chpl"
  coerce_tmp_chpl4 = ret_chpl;
#line 17 "../lib/MNISTTester.chpl"
  *(_formal_tmp_in_ind_chpl + INT64(0)) = *(coerce_tmp_chpl3 + INT64(0));
#line 17 "../lib/MNISTTester.chpl"
  _formal_tmp_ind_x0_chpl = *(_formal_tmp_in_ind_chpl + INT64(0));
#line 17 "../lib/MNISTTester.chpl"
  chpl_check_nil(coerce_tmp_chpl4, INT64(17), INT32(66));
#line 17 "../lib/MNISTTester.chpl"
  coerce_tmp_chpl5 = (coerce_tmp_chpl4)->shiftedData;
#line 17 "../lib/MNISTTester.chpl"
  call_tmp_chpl8 = (coerce_tmp_chpl5 + _formal_tmp_ind_x0_chpl);
#line 17 "../lib/MNISTTester.chpl"
  call_tmp_chpl9 = log(*(call_tmp_chpl8));
#line 17 "../lib/MNISTTester.chpl"
  call_tmp_chpl10 = (-call_tmp_chpl9);
#line 18 "../lib/MNISTTester.chpl"
  call_tmp_chpl11 = &((&output_chpl)->data);
#line 18 "../lib/MNISTTester.chpl"
  call_tmp_chpl12 = argmax_chpl2(call_tmp_chpl11);
#line 18 "../lib/MNISTTester.chpl"
  call_tmp_chpl13 = (call_tmp_chpl12 == lb_chpl);
#line 19 "../lib/MNISTTester.chpl"
  i__e0_x_chpl = &output_chpl;
#line 19 "../lib/MNISTTester.chpl"
  (&ret_tmp_chpl2)->x0 = i__e0_x_chpl;
#line 19 "../lib/MNISTTester.chpl"
  (&ret_tmp_chpl2)->x1 = call_tmp_chpl10;
#line 19 "../lib/MNISTTester.chpl"
  (&ret_tmp_chpl2)->x2 = call_tmp_chpl13;
#line 19 "../lib/MNISTTester.chpl"
  call_tmp_chpl14 = ret_tmp_chpl2;
#line 19 "../lib/MNISTTester.chpl"
  copy_ret_tmp_x0_chpl = (&call_tmp_chpl14)->x0;
#line 19 "../lib/MNISTTester.chpl"
  copy_ret_tmp_x1_chpl = (&call_tmp_chpl14)->x1;
#line 19 "../lib/MNISTTester.chpl"
  copy_ret_tmp_x2_chpl = (&call_tmp_chpl14)->x2;
#line 19 "../lib/MNISTTester.chpl"
  init_ASSIGN__chpl32(&tmp_chpl, copy_ret_tmp_x0_chpl);
#line 19 "../lib/MNISTTester.chpl"
  ret_chpl3 = tmp_chpl;
#line 19 "../lib/MNISTTester.chpl"
  (&ret_chpl2)->x0 = ret_chpl3;
#line 19 "../lib/MNISTTester.chpl"
  (&ret_chpl2)->x1 = copy_ret_tmp_x1_chpl;
#line 19 "../lib/MNISTTester.chpl"
  (&ret_chpl2)->x2 = copy_ret_tmp_x2_chpl;
#line 19 "../lib/MNISTTester.chpl"
  _field_destructor_tmp__chpl = (&output_chpl)->data;
#line 19 "../lib/MNISTTester.chpl"
  chpl__autoDestroy18(&_field_destructor_tmp__chpl, INT64(19), INT32(66));
#line 19 "../lib/MNISTTester.chpl"
  _field_destructor_tmp__chpl2 = (&output_chpl)->_domain;
#line 19 "../lib/MNISTTester.chpl"
  chpl__autoDestroy11(&_field_destructor_tmp__chpl2, INT64(19), INT32(66));
#line 15 "../lib/MNISTTester.chpl"
  *(_retArg_chpl) = ret_chpl2;
#line 15 "../lib/MNISTTester.chpl"
  return;
#line 15 "../lib/MNISTTester.chpl"
}

#line 22 "../lib/MNISTTester.chpl"
/* MNISTTester.chpl:22 */
#line 22 "../lib/MNISTTester.chpl"
static void train_chpl(Network__tuple_4_Conv_Conv_MaxPool_SoftMax_chpl * net_chpl2,
#line 22 "../lib/MNISTTester.chpl"
                       _array_DefaultRectangularArr_1_int64_t_one__tuple_2_Tensor_3__real64_int64_t_int64_t_chpl * data_chpl,
#line 22 "../lib/MNISTTester.chpl"
                       _real64 lr_chpl,
#line 22 "../lib/MNISTTester.chpl"
                       _tuple_2__real64_int64_t_chpl * _retArg_chpl) {
#line 32 "../lib/MNISTTester.chpl"
  int64_t local_dataParMinGranularity_chpl;
#line 32 "../lib/MNISTTester.chpl"
  chpl_bool local_dataParIgnoreRunningTasks_chpl;
#line 32 "../lib/MNISTTester.chpl"
  int64_t local_dataParTasksPerLocale_chpl;
#line 24 "../lib/MNISTTester.chpl"
  _domain_DefaultRectangularDom_1_int64_t_one_chpl call_tmp_chpl6;
#line 24 "../lib/MNISTTester.chpl"
  _domain_DefaultRectangularDom_1_int64_t_one_chpl ret_tmp_chpl;
#line 24 "../lib/MNISTTester.chpl"
  int64_t call_tmp_chpl7;
#line 24 "../lib/MNISTTester.chpl"
  int64_t size_chpl9;
#line 30 "../lib/MNISTTester.chpl"
  _array_DefaultRectangularArr_1_int64_t_one_Tensor_1__real64_int64_t_chpl gradients_chpl;
#line 30 "../lib/MNISTTester.chpl"
  range_int64_t_low_one_chpl call_tmp_chpl8;
#line 30 "../lib/MNISTTester.chpl"
  range_int64_t_low_one_chpl ret_tmp_chpl2;
#line 30 "../lib/MNISTTester.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl9;
#line 30 "../lib/MNISTTester.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl3;
#line 30 "../lib/MNISTTester.chpl"
  _domain_DefaultRectangularDom_1_int64_t_one_chpl call_tmp_chpl10;
#line 30 "../lib/MNISTTester.chpl"
  _domain_DefaultRectangularDom_1_int64_t_one_chpl ret_tmp_chpl4;
#line 30 "../lib/MNISTTester.chpl"
  chpl___RuntimeTypeInfo12 call_tmp_chpl11;
#line 30 "../lib/MNISTTester.chpl"
  _domain_DefaultRectangularDom_1_int64_t_one_chpl _runtime_type_tmp__chpl;
#line 30 "../lib/MNISTTester.chpl"
  _array_DefaultRectangularArr_1_int64_t_one_Tensor_1__real64_int64_t_chpl ret_tmp_chpl5;
#line 32 "../lib/MNISTTester.chpl"
  range_int64_t_low_one_chpl call_tmp_chpl12;
#line 32 "../lib/MNISTTester.chpl"
  range_int64_t_low_one_chpl ret_tmp_chpl6;
#line 32 "../lib/MNISTTester.chpl"
  SumReduceScanOp__real64_chpl globalRP_loss_chpl = NULL;
#line 32 "../lib/MNISTTester.chpl"
  SumReduceScanOp__real64_chpl new_temp_chpl = NULL;
#line 32 "../lib/MNISTTester.chpl"
  chpl_LocalSpinlock default_arg_l_chpl;
#line 32 "../lib/MNISTTester.chpl"
  chpl_LocalSpinlock ret_chpl;
#line 32 "../lib/MNISTTester.chpl"
  chpl_LocalSpinlock l_chpl;
#line 32 "../lib/MNISTTester.chpl"
  SumReduceScanOp__real64_chpl coerce_tmp_chpl3 = NULL;
#line 32 "../lib/MNISTTester.chpl"
  _ref__real64_chpl call_tmp_chpl13 = NULL;
#line 32 "../lib/MNISTTester.chpl"
  _real64 globalAS_loss_chpl;
#line 32 "../lib/MNISTTester.chpl"
  SumReduceScanOp_int64_t_chpl globalRP_acc_chpl = NULL;
#line 32 "../lib/MNISTTester.chpl"
  SumReduceScanOp_int64_t_chpl new_temp_chpl2 = NULL;
#line 32 "../lib/MNISTTester.chpl"
  chpl_LocalSpinlock default_arg_l_chpl2;
#line 32 "../lib/MNISTTester.chpl"
  chpl_LocalSpinlock ret_chpl2;
#line 32 "../lib/MNISTTester.chpl"
  chpl_LocalSpinlock l_chpl2;
#line 32 "../lib/MNISTTester.chpl"
  SumReduceScanOp_int64_t_chpl coerce_tmp_chpl4 = NULL;
#line 32 "../lib/MNISTTester.chpl"
  _ref_int64_t_chpl call_tmp_chpl14 = NULL;
#line 32 "../lib/MNISTTester.chpl"
  int64_t globalAS_acc_chpl;
#line 32 "../lib/MNISTTester.chpl"
  DefaultRectangularArr_1_int64_t_one__tuple_2_Tensor_3__real64_int64_t_int64_t_chpl coerce_tmp_chpl5 = NULL;
#line 32 "../lib/MNISTTester.chpl"
  int64_t tasksPerLocale_chpl;
#line 32 "../lib/MNISTTester.chpl"
  chpl_bool ignoreRunning_chpl;
#line 32 "../lib/MNISTTester.chpl"
  int64_t minIndicesPerTask_chpl;
#line 32 "../lib/MNISTTester.chpl"
  Error_chpl error_chpl = NULL;
#line 32 "../lib/MNISTTester.chpl"
  DefaultRectangularDom_1_int64_t_one_chpl coerce_tmp_chpl6 = NULL;
#line 32 "../lib/MNISTTester.chpl"
  int64_t offset_x0_chpl;
#line 32 "../lib/MNISTTester.chpl"
  _tuple_1_star_int64_t_chpl ret_tmp_chpl7;
#line 32 "../lib/MNISTTester.chpl"
  Error_chpl error_chpl2 = NULL;
#line 32 "../lib/MNISTTester.chpl"
  locale_chpl call_tmp_chpl15;
#line 32 "../lib/MNISTTester.chpl"
  chpl_localeID_t call_tmp_chpl16;
#line 32 "../lib/MNISTTester.chpl"
  int32_t call_tmp_chpl17;
#line 32 "../lib/MNISTTester.chpl"
  chpl_localeID_t call_tmp_chpl18;
#line 32 "../lib/MNISTTester.chpl"
  locale_chpl call_tmp_chpl19;
#line 32 "../lib/MNISTTester.chpl"
  locale_chpl ret_tmp_chpl8;
#line 32 "../lib/MNISTTester.chpl"
  BaseLocale_chpl coerce_tmp_chpl7 = NULL;
#line 32 "../lib/MNISTTester.chpl"
  _ref_locale_chpl i_x_chpl = NULL;
#line 32 "../lib/MNISTTester.chpl"
  int64_t tmp_chpl;
#line 32 "../lib/MNISTTester.chpl"
  locale_chpl call_tmp_chpl20;
#line 32 "../lib/MNISTTester.chpl"
  chpl_localeID_t call_tmp_chpl21;
#line 32 "../lib/MNISTTester.chpl"
  int32_t call_tmp_chpl22;
#line 32 "../lib/MNISTTester.chpl"
  chpl_localeID_t call_tmp_chpl23;
#line 32 "../lib/MNISTTester.chpl"
  locale_chpl call_tmp_chpl24;
#line 32 "../lib/MNISTTester.chpl"
  locale_chpl ret_tmp_chpl9;
#line 32 "../lib/MNISTTester.chpl"
  int64_t ret_chpl3;
#line 32 "../lib/MNISTTester.chpl"
  BaseLocale_chpl coerce_tmp_chpl8 = NULL;
#line 32 "../lib/MNISTTester.chpl"
  BaseLocale_chpl coerce_tmp_chpl9 = NULL;
#line 32 "../lib/MNISTTester.chpl"
  _ref_locale_chpl i_x_chpl2 = NULL;
#line 32 "../lib/MNISTTester.chpl"
  int64_t tmp_x0_chpl;
#line 32 "../lib/MNISTTester.chpl"
  int64_t tmp_x1_chpl;
#line 32 "../lib/MNISTTester.chpl"
  _ref__tuple_1_star_range_int64_t_both_one_chpl call_tmp_chpl25 = NULL;
#line 32 "../lib/MNISTTester.chpl"
  _tuple_2_star_int64_t_chpl ret_tmp_chpl10;
#line 32 "../lib/MNISTTester.chpl"
  int64_t numChunks_chpl;
#line 32 "../lib/MNISTTester.chpl"
  int64_t parDim_chpl;
#line 32 "../lib/MNISTTester.chpl"
  range_int64_t_both_one_chpl locBlock_x0_chpl;
#line 32 "../lib/MNISTTester.chpl"
  range_int64_t_both_one_chpl tup_x0_chpl;
#line 32 "../lib/MNISTTester.chpl"
  range_int64_t_both_one_chpl elt_x0_chpl;
#line 32 "../lib/MNISTTester.chpl"
  range_int64_t_low_one_chpl call_tmp_chpl26;
#line 32 "../lib/MNISTTester.chpl"
  range_int64_t_low_one_chpl ret_tmp_chpl11;
#line 32 "../lib/MNISTTester.chpl"
  _ref__tuple_1_star_range_int64_t_both_one_chpl call_tmp_chpl27 = NULL;
#line 32 "../lib/MNISTTester.chpl"
  int64_t call_tmp_chpl28;
#line 32 "../lib/MNISTTester.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl10;
#line 32 "../lib/MNISTTester.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl29;
#line 32 "../lib/MNISTTester.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl12;
#line 32 "../lib/MNISTTester.chpl"
  range_int64_t_both_one_chpl tmpIter_chpl;
#line 32 "../lib/MNISTTester.chpl"
  range_int64_t_low_one_chpl call_tmp_chpl30;
#line 32 "../lib/MNISTTester.chpl"
  range_int64_t_low_one_chpl ret_tmp_chpl13;
#line 32 "../lib/MNISTTester.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl14;
#line 32 "../lib/MNISTTester.chpl"
  chpl___EndCount_AtomicT_int64_t_int64_t _coforallCount_chpl = NULL;
#line 32 "../lib/MNISTTester.chpl"
  chpl___EndCount_AtomicT_int64_t_int64_t new_temp_chpl3 = NULL;
#line 32 "../lib/MNISTTester.chpl"
  int64_t numTasks_chpl;
#line 32 "../lib/MNISTTester.chpl"
  range_int64_t_both_one_chpl _ic__F0_this_chpl;
#line 32 "../lib/MNISTTester.chpl"
  int64_t i_chpl;
#line 32 "../lib/MNISTTester.chpl"
  range_int64_t_both_one_chpl this_chpl31;
#line 32 "../lib/MNISTTester.chpl"
  range_int64_t_both_one_chpl this_chpl32;
#line 32 "../lib/MNISTTester.chpl"
  int64_t end_chpl;
#line 32 "../lib/MNISTTester.chpl"
  _tuple_1_star_range_int64_t_both_one_chpl ret_tmp_chpl15;
#line 32 "../lib/MNISTTester.chpl"
  _class_localscoforall_fn_chpl114 _args_forcoforall_fn_chpl = NULL;
#line 32 "../lib/MNISTTester.chpl"
  _ref_Network__tuple_4_Conv_Conv_MaxPool_SoftMax_chpl tmp_chpl2 = NULL;
#line 32 "../lib/MNISTTester.chpl"
  _ref_range_int64_t_low_one_chpl tmp_chpl3 = NULL;
#line 32 "../lib/MNISTTester.chpl"
  raw_c_void_ptr _args_vforcoforall_fn_chpl;
#line 32 "../lib/MNISTTester.chpl"
  Error_chpl error_chpl3 = NULL;
#line 32 "../lib/MNISTTester.chpl"
  SumReduceScanOp_int64_t_chpl coerce_tmp_chpl11 = NULL;
#line 32 "../lib/MNISTTester.chpl"
  _ref_int64_t_chpl call_tmp_chpl31 = NULL;
#line 32 "../lib/MNISTTester.chpl"
  SumReduceScanOp_int64_t_chpl coerce_tmp_chpl12 = NULL;
#line 32 "../lib/MNISTTester.chpl"
  int64_t ret_chpl4;
#line 32 "../lib/MNISTTester.chpl"
  SumReduceScanOp__real64_chpl coerce_tmp_chpl13 = NULL;
#line 32 "../lib/MNISTTester.chpl"
  _ref__real64_chpl call_tmp_chpl32 = NULL;
#line 32 "../lib/MNISTTester.chpl"
  SumReduceScanOp__real64_chpl coerce_tmp_chpl14 = NULL;
#line 32 "../lib/MNISTTester.chpl"
  _real64 ret_chpl5;
#line 43 "../lib/MNISTTester.chpl"
  _array_DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl inputs_chpl;
#line 43 "../lib/MNISTTester.chpl"
  _ir_chpl__loopexpr_iter12_chpl call_tmp_chpl33;
#line 43 "../lib/MNISTTester.chpl"
  _ir_chpl__loopexpr_iter12_chpl ret_tmp_chpl16;
#line 43 "../lib/MNISTTester.chpl"
  _domain_DefaultRectangularDom_1_int64_t_one_chpl call_tmp_chpl34;
#line 43 "../lib/MNISTTester.chpl"
  _domain_DefaultRectangularDom_1_int64_t_one_chpl ret_tmp_chpl17;
#line 43 "../lib/MNISTTester.chpl"
  DefaultRectangularDom_1_int64_t_one_chpl ret_chpl6 = NULL;
#line 44 "../lib/MNISTTester.chpl"
  _array_DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl _return_tmp__chpl;
#line 44 "../lib/MNISTTester.chpl"
  _array_DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl ret_tmp_chpl18;
#line 46 "../lib/MNISTTester.chpl"
  _real64 call_tmp_chpl35;
#line 48 "../lib/MNISTTester.chpl"
  _tuple_2__real64_int64_t_chpl ret_tmp_chpl19;
#line 48 "../lib/MNISTTester.chpl"
  chpl__class_localscoforall_fn_chpl114_object chpl_macro_tmp_123;
#line 32 "../lib/MNISTTester.chpl"
  local_dataParMinGranularity_chpl = dataParMinGranularity_chpl;
#line 32 "../lib/MNISTTester.chpl"
  local_dataParIgnoreRunningTasks_chpl = dataParIgnoreRunningTasks_chpl;
#line 32 "../lib/MNISTTester.chpl"
  local_dataParTasksPerLocale_chpl = dataParTasksPerLocale_chpl;
#line 24 "../lib/MNISTTester.chpl"
  _dom_chpl2(data_chpl, &ret_tmp_chpl, INT64(24), INT32(66));
#line 24 "../lib/MNISTTester.chpl"
  call_tmp_chpl6 = ret_tmp_chpl;
#line 24 "../lib/MNISTTester.chpl"
  call_tmp_chpl7 = size_chpl4(&call_tmp_chpl6, INT64(24), INT32(66));
#line 24 "../lib/MNISTTester.chpl"
  size_chpl9 = call_tmp_chpl7;
#line 24 "../lib/MNISTTester.chpl"
  chpl__autoDestroy11(&call_tmp_chpl6, INT64(24), INT32(66));
#line 29 "../lib/MNISTTester.chpl"
  resetGradients_chpl4(net_chpl2);
#line 30 "../lib/MNISTTester.chpl"
  chpl_build_low_bounded_range2(INT64(0), &ret_tmp_chpl2);
#line 30 "../lib/MNISTTester.chpl"
  call_tmp_chpl8 = ret_tmp_chpl2;
#line 30 "../lib/MNISTTester.chpl"
  chpl___POUND_3(call_tmp_chpl8, call_tmp_chpl7, &ret_tmp_chpl3, INT64(30), INT32(66));
#line 30 "../lib/MNISTTester.chpl"
  call_tmp_chpl9 = ret_tmp_chpl3;
#line 30 "../lib/MNISTTester.chpl"
  chpl__ensureDomainExpr5(call_tmp_chpl9, &ret_tmp_chpl4, INT64(30), INT32(66));
#line 30 "../lib/MNISTTester.chpl"
  call_tmp_chpl10 = ret_tmp_chpl4;
#line 30 "../lib/MNISTTester.chpl"
  call_tmp_chpl11 = chpl__buildArrayRuntimeType11(&call_tmp_chpl10);
#line 30 "../lib/MNISTTester.chpl"
  _runtime_type_tmp__chpl = (&call_tmp_chpl11)->dom;
#line 30 "../lib/MNISTTester.chpl"
  chpl__convertRuntimeTypeToValue(&_runtime_type_tmp__chpl, UINT8(false), &ret_tmp_chpl5, INT64(30), INT32(66));
#line 30 "../lib/MNISTTester.chpl"
  gradients_chpl = ret_tmp_chpl5;
#line 32 "../lib/MNISTTester.chpl"
  chpl_build_low_bounded_range2(INT64(0), &ret_tmp_chpl6);
#line 32 "../lib/MNISTTester.chpl"
  call_tmp_chpl12 = ret_tmp_chpl6;
#line 32 "../lib/MNISTTester.chpl"
  init_chpl105(&l_chpl);
#line 32 "../lib/MNISTTester.chpl"
  ret_chpl = l_chpl;
#line 32 "../lib/MNISTTester.chpl"
  default_arg_l_chpl = ret_chpl;
#line 32 "../lib/MNISTTester.chpl"
  new_temp_chpl = _new_chpl23(&default_arg_l_chpl, REAL64(0x0p+0), INT64(32), INT32(66));
#line 32 "../lib/MNISTTester.chpl"
  globalRP_loss_chpl = new_temp_chpl;
#line 32 "../lib/MNISTTester.chpl"
  coerce_tmp_chpl3 = globalRP_loss_chpl;
#line 32 "../lib/MNISTTester.chpl"
  chpl_check_nil(coerce_tmp_chpl3, INT64(32), INT32(66));
#line 32 "../lib/MNISTTester.chpl"
  call_tmp_chpl13 = &((coerce_tmp_chpl3)->value);
#line 32 "../lib/MNISTTester.chpl"
  *(call_tmp_chpl13) += REAL64(0x0p+0);
#line 32 "../lib/MNISTTester.chpl"
  globalAS_loss_chpl = REAL64(0x0p+0);
#line 32 "../lib/MNISTTester.chpl"
  init_chpl105(&l_chpl2);
#line 32 "../lib/MNISTTester.chpl"
  ret_chpl2 = l_chpl2;
#line 32 "../lib/MNISTTester.chpl"
  default_arg_l_chpl2 = ret_chpl2;
#line 32 "../lib/MNISTTester.chpl"
  new_temp_chpl2 = _new_chpl21(&default_arg_l_chpl2, INT64(0), INT64(32), INT32(66));
#line 32 "../lib/MNISTTester.chpl"
  globalRP_acc_chpl = new_temp_chpl2;
#line 32 "../lib/MNISTTester.chpl"
  coerce_tmp_chpl4 = globalRP_acc_chpl;
#line 32 "../lib/MNISTTester.chpl"
  chpl_check_nil(coerce_tmp_chpl4, INT64(32), INT32(66));
#line 32 "../lib/MNISTTester.chpl"
  call_tmp_chpl14 = &((coerce_tmp_chpl4)->value);
#line 32 "../lib/MNISTTester.chpl"
  *(call_tmp_chpl14) += INT64(0);
#line 32 "../lib/MNISTTester.chpl"
  globalAS_acc_chpl = INT64(0);
#line 32 "../lib/MNISTTester.chpl"
  coerce_tmp_chpl5 = (data_chpl)->_instance;
#line 32 "../lib/MNISTTester.chpl"
  tasksPerLocale_chpl = local_dataParTasksPerLocale_chpl;
#line 32 "../lib/MNISTTester.chpl"
  ignoreRunning_chpl = local_dataParIgnoreRunningTasks_chpl;
#line 32 "../lib/MNISTTester.chpl"
  minIndicesPerTask_chpl = local_dataParMinGranularity_chpl;
#line 32 "../lib/MNISTTester.chpl"
  error_chpl = NULL;
#line 32 "../lib/MNISTTester.chpl"
  chpl_check_nil(coerce_tmp_chpl5, INT64(32), INT32(66));
#line 32 "../lib/MNISTTester.chpl"
  coerce_tmp_chpl6 = (coerce_tmp_chpl5)->dom;
#line 32 "../lib/MNISTTester.chpl"
  createTuple_chpl(INT64(0), &ret_tmp_chpl7);
#line 32 "../lib/MNISTTester.chpl"
  offset_x0_chpl = *(ret_tmp_chpl7 + INT64(0));
#line 32 "../lib/MNISTTester.chpl"
  error_chpl2 = NULL;
#line 32 "../lib/MNISTTester.chpl"
  call_tmp_chpl17 = get_chpl_nodeID();
#line 32 "../lib/MNISTTester.chpl"
  call_tmp_chpl18 = chpl_rt_buildLocaleID(call_tmp_chpl17, c_sublocid_any);
#line 32 "../lib/MNISTTester.chpl"
  call_tmp_chpl16 = call_tmp_chpl18;
#line 32 "../lib/MNISTTester.chpl"
  chpl_localeID_to_locale(&call_tmp_chpl16, &ret_tmp_chpl8, INT64(32), INT32(66));
#line 32 "../lib/MNISTTester.chpl"
  call_tmp_chpl19 = ret_tmp_chpl8;
#line 32 "../lib/MNISTTester.chpl"
  call_tmp_chpl15 = call_tmp_chpl19;
#line 32 "../lib/MNISTTester.chpl"
  coerce_tmp_chpl7 = (&call_tmp_chpl15)->_instance;
#line 32 "../lib/MNISTTester.chpl"
  if (((RootClass_chpl)(coerce_tmp_chpl7)) == nil) /* ZLINE: 32 ../lib/MNISTTester.chpl */
#line 32 "../lib/MNISTTester.chpl"
  {
#line 32 "../lib/MNISTTester.chpl"
    nilCheckHalt_chpl(&_str_literal_52_chpl, INT64(32), INT32(66));
#line 32 "../lib/MNISTTester.chpl"
  }
#line 32 "../lib/MNISTTester.chpl"
  chpl_check_nil(coerce_tmp_chpl7, INT64(32), INT32(66));
#line 32 "../lib/MNISTTester.chpl"
  ((int64_t(*)(BaseLocale_chpl,
#line 32 "../lib/MNISTTester.chpl"
     int64_t,
#line 32 "../lib/MNISTTester.chpl"
     int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(coerce_tmp_chpl7))->chpl__cid) + INT64(2))])(coerce_tmp_chpl7, INT64(32), INT32(66));
#line 32 "../lib/MNISTTester.chpl"
  i_x_chpl = &call_tmp_chpl15;
#line 32 "../lib/MNISTTester.chpl"
  deinit_chpl62(i_x_chpl);
#line 32 "../lib/MNISTTester.chpl"
  if (tasksPerLocale_chpl == INT64(0)) /* ZLINE: 32 ../lib/MNISTTester.chpl */
#line 32 "../lib/MNISTTester.chpl"
  {
#line 32 "../lib/MNISTTester.chpl"
    call_tmp_chpl22 = get_chpl_nodeID();
#line 32 "../lib/MNISTTester.chpl"
    call_tmp_chpl23 = chpl_rt_buildLocaleID(call_tmp_chpl22, c_sublocid_any);
#line 32 "../lib/MNISTTester.chpl"
    call_tmp_chpl21 = call_tmp_chpl23;
#line 32 "../lib/MNISTTester.chpl"
    chpl_localeID_to_locale(&call_tmp_chpl21, &ret_tmp_chpl9, INT64(32), INT32(66));
#line 32 "../lib/MNISTTester.chpl"
    call_tmp_chpl24 = ret_tmp_chpl9;
#line 32 "../lib/MNISTTester.chpl"
    call_tmp_chpl20 = call_tmp_chpl24;
#line 32 "../lib/MNISTTester.chpl"
    coerce_tmp_chpl8 = (&call_tmp_chpl20)->_instance;
#line 32 "../lib/MNISTTester.chpl"
    if (((RootClass_chpl)(coerce_tmp_chpl8)) == nil) /* ZLINE: 32 ../lib/MNISTTester.chpl */
#line 32 "../lib/MNISTTester.chpl"
    {
#line 32 "../lib/MNISTTester.chpl"
      nilCheckHalt_chpl(&_str_literal_52_chpl, INT64(32), INT32(66));
#line 32 "../lib/MNISTTester.chpl"
    }
#line 32 "../lib/MNISTTester.chpl"
    coerce_tmp_chpl9 = coerce_tmp_chpl8;
#line 32 "../lib/MNISTTester.chpl"
    chpl_check_nil(coerce_tmp_chpl9, INT64(32), INT32(66));
#line 32 "../lib/MNISTTester.chpl"
    ret_chpl3 = (coerce_tmp_chpl9)->maxTaskPar;
#line 32 "../lib/MNISTTester.chpl"
    tmp_chpl = ret_chpl3;
#line 32 "../lib/MNISTTester.chpl"
    i_x_chpl2 = &call_tmp_chpl20;
#line 32 "../lib/MNISTTester.chpl"
    deinit_chpl62(i_x_chpl2);
#line 32 "../lib/MNISTTester.chpl"
  } else /* ZLINE: 32 ../lib/MNISTTester.chpl */
#line 32 "../lib/MNISTTester.chpl"
  {
#line 32 "../lib/MNISTTester.chpl"
    tmp_chpl = tasksPerLocale_chpl;
#line 32 "../lib/MNISTTester.chpl"
  }
#line 32 "../lib/MNISTTester.chpl"
  if (chpl_task_getSerial()) /* ZLINE: 32 ../lib/MNISTTester.chpl */
#line 32 "../lib/MNISTTester.chpl"
  {
#line 32 "../lib/MNISTTester.chpl"
    tmp_x0_chpl = INT64(1);
#line 32 "../lib/MNISTTester.chpl"
    tmp_x1_chpl = INT64(0);
#line 32 "../lib/MNISTTester.chpl"
  } else /* ZLINE: 32 ../lib/MNISTTester.chpl */
#line 32 "../lib/MNISTTester.chpl"
  {
#line 32 "../lib/MNISTTester.chpl"
    chpl_check_nil(coerce_tmp_chpl6, INT64(32), INT32(66));
#line 32 "../lib/MNISTTester.chpl"
    call_tmp_chpl25 = &((coerce_tmp_chpl6)->ranges);
#line 32 "../lib/MNISTTester.chpl"
    _computeChunkStuff_chpl(tmp_chpl, ignoreRunning_chpl, minIndicesPerTask_chpl, call_tmp_chpl25, &ret_tmp_chpl10, INT64(32), INT32(66));
#line 32 "../lib/MNISTTester.chpl"
    tmp_x0_chpl = *(ret_tmp_chpl10 + INT64(0));
#line 32 "../lib/MNISTTester.chpl"
    tmp_x1_chpl = *(ret_tmp_chpl10 + INT64(1));
#line 32 "../lib/MNISTTester.chpl"
  }
#line 32 "../lib/MNISTTester.chpl"
  numChunks_chpl = tmp_x0_chpl;
#line 32 "../lib/MNISTTester.chpl"
  parDim_chpl = tmp_x1_chpl;
#line 32 "../lib/MNISTTester.chpl"
  init_chpl113(&elt_x0_chpl);
#line 32 "../lib/MNISTTester.chpl"
  tup_x0_chpl = elt_x0_chpl;
#line 32 "../lib/MNISTTester.chpl"
  locBlock_x0_chpl = tup_x0_chpl;
#line 32 "../lib/MNISTTester.chpl"
  chpl_build_low_bounded_range2(offset_x0_chpl, &ret_tmp_chpl11);
#line 32 "../lib/MNISTTester.chpl"
  call_tmp_chpl26 = ret_tmp_chpl11;
#line 32 "../lib/MNISTTester.chpl"
  chpl_check_nil(coerce_tmp_chpl6, INT64(32), INT32(66));
#line 32 "../lib/MNISTTester.chpl"
  call_tmp_chpl27 = &((coerce_tmp_chpl6)->ranges);
#line 32 "../lib/MNISTTester.chpl"
  coerce_tmp_chpl10 = *(*(call_tmp_chpl27) + INT64(0));
#line 32 "../lib/MNISTTester.chpl"
  call_tmp_chpl28 = sizeAs_chpl37(coerce_tmp_chpl10, INT64(32), INT32(66));
#line 32 "../lib/MNISTTester.chpl"
  chpl___POUND_3(call_tmp_chpl26, call_tmp_chpl28, &ret_tmp_chpl12, INT64(32), INT32(66));
#line 32 "../lib/MNISTTester.chpl"
  call_tmp_chpl29 = ret_tmp_chpl12;
#line 32 "../lib/MNISTTester.chpl"
  locBlock_x0_chpl = call_tmp_chpl29;
#line 32 "../lib/MNISTTester.chpl"
  chpl_build_low_bounded_range2(INT64(0), &ret_tmp_chpl13);
#line 32 "../lib/MNISTTester.chpl"
  call_tmp_chpl30 = ret_tmp_chpl13;
#line 32 "../lib/MNISTTester.chpl"
  chpl___POUND_3(call_tmp_chpl30, tmp_x0_chpl, &ret_tmp_chpl14, INT64(32), INT32(66));
#line 32 "../lib/MNISTTester.chpl"
  tmpIter_chpl = ret_tmp_chpl14;
#line 32 "../lib/MNISTTester.chpl"
  new_temp_chpl3 = _new_chpl17(INT64(32), INT32(66));
#line 32 "../lib/MNISTTester.chpl"
  _coforallCount_chpl = new_temp_chpl3;
#line 32 "../lib/MNISTTester.chpl"
  numTasks_chpl = chpl_boundedCoforallSize(tmpIter_chpl, INT64(32), INT32(66));
#line 32 "../lib/MNISTTester.chpl"
  _upEndCount_chpl(_coforallCount_chpl, numTasks_chpl, INT64(32), INT32(66));
#line 32 "../lib/MNISTTester.chpl"
  chpl_resetTaskSpawn(numTasks_chpl, INT64(32), INT32(66));
#line 32 "../lib/MNISTTester.chpl"
  _ic__F0_this_chpl = tmpIter_chpl;
#line 32 "../lib/MNISTTester.chpl"
  checkIfIterWillOverflow_chpl(tmpIter_chpl, UINT8(true), INT64(32), INT32(66));
#line 32 "../lib/MNISTTester.chpl"
  i_chpl = INT64(0);
#line 32 "../lib/MNISTTester.chpl"
  this_chpl31 = _ic__F0_this_chpl;
#line 32 "../lib/MNISTTester.chpl"
  this_chpl32 = _ic__F0_this_chpl;
#line 32 "../lib/MNISTTester.chpl"
  end_chpl = (&this_chpl32)->_high;
#line 32 "../lib/MNISTTester.chpl"
  for (i_chpl = (&this_chpl31)->_low; ((i_chpl <= end_chpl)); i_chpl += INT64(1)) {
#line 32 "../lib/MNISTTester.chpl"
    *(ret_tmp_chpl15 + INT64(0)) = locBlock_x0_chpl;
#line 32 "../lib/MNISTTester.chpl"
    _args_forcoforall_fn_chpl = ((_class_localscoforall_fn_chpl114)(&chpl_macro_tmp_123));
#line 32 "../lib/MNISTTester.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl, INT64(32), INT32(66));
#line 32 "../lib/MNISTTester.chpl"
    (_args_forcoforall_fn_chpl)->_0_numChunks = numChunks_chpl;
#line 32 "../lib/MNISTTester.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl, INT64(32), INT32(66));
#line 32 "../lib/MNISTTester.chpl"
    (_args_forcoforall_fn_chpl)->_1_parDim = parDim_chpl;
#line 32 "../lib/MNISTTester.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl, INT64(32), INT32(66));
#line 32 "../lib/MNISTTester.chpl"
    *((_args_forcoforall_fn_chpl)->_2_coerce_tmp + INT64(0)) = *(ret_tmp_chpl15 + INT64(0));
#line 32 "../lib/MNISTTester.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl, INT64(32), INT32(66));
#line 32 "../lib/MNISTTester.chpl"
    (_args_forcoforall_fn_chpl)->_3__coforallCount = _coforallCount_chpl;
#line 32 "../lib/MNISTTester.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl, INT64(32), INT32(66));
#line 32 "../lib/MNISTTester.chpl"
    (_args_forcoforall_fn_chpl)->_4_chunk = i_chpl;
#line 32 "../lib/MNISTTester.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl, INT64(32), INT32(66));
#line 32 "../lib/MNISTTester.chpl"
    (_args_forcoforall_fn_chpl)->_6_globalRP_loss = globalRP_loss_chpl;
#line 32 "../lib/MNISTTester.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl, INT64(32), INT32(66));
#line 32 "../lib/MNISTTester.chpl"
    (_args_forcoforall_fn_chpl)->_7_globalRP_acc = globalRP_acc_chpl;
#line 32 "../lib/MNISTTester.chpl"
    tmp_chpl2 = net_chpl2;
#line 32 "../lib/MNISTTester.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl, INT64(32), INT32(66));
#line 32 "../lib/MNISTTester.chpl"
    (_args_forcoforall_fn_chpl)->_8_net = tmp_chpl2;
#line 32 "../lib/MNISTTester.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl, INT64(32), INT32(66));
#line 32 "../lib/MNISTTester.chpl"
    (_args_forcoforall_fn_chpl)->_9_data = *(data_chpl);
#line 32 "../lib/MNISTTester.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl, INT64(32), INT32(66));
#line 32 "../lib/MNISTTester.chpl"
    (_args_forcoforall_fn_chpl)->_10_gradients = gradients_chpl;
#line 32 "../lib/MNISTTester.chpl"
    tmp_chpl3 = &call_tmp_chpl12;
#line 32 "../lib/MNISTTester.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl, INT64(32), INT32(66));
#line 32 "../lib/MNISTTester.chpl"
    (_args_forcoforall_fn_chpl)->_11_call_tmp = tmp_chpl3;
#line 32 "../lib/MNISTTester.chpl"
    _args_vforcoforall_fn_chpl = ((void*)(_args_forcoforall_fn_chpl));
#line 32 "../lib/MNISTTester.chpl"
    /*** wrapcoforall_fn_chpl114 ***/ chpl_taskAddCoStmt(chpl_task_getRequestedSubloc(), INT64(113), ((chpl_task_bundle_p)(_args_vforcoforall_fn_chpl)), sizeof(chpl__class_localscoforall_fn_chpl114_object), INT64(32), INT32(66));
#line 32 "../lib/MNISTTester.chpl"
  }
#line 32 "../lib/MNISTTester.chpl"
  error_chpl3 = NULL;
#line 32 "../lib/MNISTTester.chpl"
  _waitEndCount_chpl2(_coforallCount_chpl, numTasks_chpl, &error_chpl3, INT64(32), INT32(66));
#line 32 "../lib/MNISTTester.chpl"
  if (error_chpl3 != nil) /* ZLINE: 32 ../lib/MNISTTester.chpl */
#line 32 "../lib/MNISTTester.chpl"
  {
#line 32 "../lib/MNISTTester.chpl"
    chpl__delete72(_coforallCount_chpl, INT64(32), INT32(66));
#line 32 "../lib/MNISTTester.chpl"
    error_chpl2 = error_chpl3;
#line 32 "../lib/MNISTTester.chpl"
    goto forall_IBB_break_label_chpl;
#line 32 "../lib/MNISTTester.chpl"
  }
#line 32 "../lib/MNISTTester.chpl"
  chpl__delete72(_coforallCount_chpl, INT64(32), INT32(66));
#line 32 "../lib/MNISTTester.chpl"
  chpl_after_forall_fence();
#line 32 "../lib/MNISTTester.chpl"
  forall_IBB_break_label_chpl:;
#line 32 "../lib/MNISTTester.chpl"
  if (error_chpl2 != nil) /* ZLINE: 32 ../lib/MNISTTester.chpl */
#line 32 "../lib/MNISTTester.chpl"
  {
#line 32 "../lib/MNISTTester.chpl"
    error_chpl = error_chpl2;
#line 32 "../lib/MNISTTester.chpl"
    goto forall_IBB_break_label_chpl2;
#line 32 "../lib/MNISTTester.chpl"
  }
#line 32 "../lib/MNISTTester.chpl"
  chpl_after_forall_fence();
#line 32 "../lib/MNISTTester.chpl"
  forall_IBB_break_label_chpl2:;
#line 32 "../lib/MNISTTester.chpl"
  if (error_chpl != nil) /* ZLINE: 32 ../lib/MNISTTester.chpl */
#line 32 "../lib/MNISTTester.chpl"
  {
#line 32 "../lib/MNISTTester.chpl"
    chpl_propagate_error(error_chpl, INT64(32), INT32(66));
#line 32 "../lib/MNISTTester.chpl"
  }
#line 32 "../lib/MNISTTester.chpl"
  chpl_after_forall_fence();
#line 32 "../lib/MNISTTester.chpl"
  coerce_tmp_chpl11 = globalRP_acc_chpl;
#line 32 "../lib/MNISTTester.chpl"
  chpl_check_nil(coerce_tmp_chpl11, INT64(32), INT32(66));
#line 32 "../lib/MNISTTester.chpl"
  call_tmp_chpl31 = &((coerce_tmp_chpl11)->value);
#line 32 "../lib/MNISTTester.chpl"
  *(call_tmp_chpl31) += globalAS_acc_chpl;
#line 32 "../lib/MNISTTester.chpl"
  coerce_tmp_chpl12 = globalRP_acc_chpl;
#line 32 "../lib/MNISTTester.chpl"
  chpl_check_nil(coerce_tmp_chpl12, INT64(32), INT32(66));
#line 32 "../lib/MNISTTester.chpl"
  ret_chpl4 = (coerce_tmp_chpl12)->value;
#line 32 "../lib/MNISTTester.chpl"
  chpl__delete16(globalRP_acc_chpl, INT64(32), INT32(66));
#line 32 "../lib/MNISTTester.chpl"
  coerce_tmp_chpl13 = globalRP_loss_chpl;
#line 32 "../lib/MNISTTester.chpl"
  chpl_check_nil(coerce_tmp_chpl13, INT64(32), INT32(66));
#line 32 "../lib/MNISTTester.chpl"
  call_tmp_chpl32 = &((coerce_tmp_chpl13)->value);
#line 32 "../lib/MNISTTester.chpl"
  *(call_tmp_chpl32) += globalAS_loss_chpl;
#line 32 "../lib/MNISTTester.chpl"
  coerce_tmp_chpl14 = globalRP_loss_chpl;
#line 32 "../lib/MNISTTester.chpl"
  chpl_check_nil(coerce_tmp_chpl14, INT64(32), INT32(66));
#line 32 "../lib/MNISTTester.chpl"
  ret_chpl5 = (coerce_tmp_chpl14)->value;
#line 32 "../lib/MNISTTester.chpl"
  chpl__delete33(globalRP_loss_chpl, INT64(32), INT32(66));
#line 43 "../lib/MNISTTester.chpl"
  chpl__forallexpr12(data_chpl, &ret_tmp_chpl16);
#line 43 "../lib/MNISTTester.chpl"
  call_tmp_chpl33 = ret_tmp_chpl16;
#line 43 "../lib/MNISTTester.chpl"
  _dom_chpl2(data_chpl, &ret_tmp_chpl17, INT64(43), INT32(66));
#line 43 "../lib/MNISTTester.chpl"
  call_tmp_chpl34 = ret_tmp_chpl17;
#line 43 "../lib/MNISTTester.chpl"
  ret_chpl6 = (&call_tmp_chpl34)->_instance;
#line 43 "../lib/MNISTTester.chpl"
  chpl__autoDestroy11(&call_tmp_chpl34, INT64(43), INT32(66));
#line 43 "../lib/MNISTTester.chpl"
  (&call_tmp_chpl33)->_shape_ = ret_chpl6;
#line 43 "../lib/MNISTTester.chpl"
  inputs_chpl = chpl__initCopy18(&call_tmp_chpl33, UINT8(true), INT64(43), INT32(66));
#line 44 "../lib/MNISTTester.chpl"
  backwardPropBatch_chpl(net_chpl2, &inputs_chpl, &gradients_chpl, &ret_tmp_chpl18);
#line 44 "../lib/MNISTTester.chpl"
  _return_tmp__chpl = ret_tmp_chpl18;
#line 44 "../lib/MNISTTester.chpl"
  chpl__autoDestroy13(&_return_tmp__chpl, INT64(44), INT32(66));
#line 46 "../lib/MNISTTester.chpl"
  call_tmp_chpl35 = (lr_chpl / ((_real64)(size_chpl9)));
#line 46 "../lib/MNISTTester.chpl"
  optimize_chpl4(net_chpl2, call_tmp_chpl35);
#line 48 "../lib/MNISTTester.chpl"
  (&ret_tmp_chpl19)->x0 = ret_chpl5;
#line 48 "../lib/MNISTTester.chpl"
  (&ret_tmp_chpl19)->x1 = ret_chpl4;
#line 48 "../lib/MNISTTester.chpl"
  chpl__autoDestroy13(&inputs_chpl, INT64(48), INT32(66));
#line 48 "../lib/MNISTTester.chpl"
  chpl__autoDestroy20(&gradients_chpl, INT64(48), INT32(66));
#line 48 "../lib/MNISTTester.chpl"
  chpl__autoDestroy11(&call_tmp_chpl10, INT64(48), INT32(66));
#line 22 "../lib/MNISTTester.chpl"
  *(_retArg_chpl) = ret_tmp_chpl19;
#line 22 "../lib/MNISTTester.chpl"
  return;
#line 22 "../lib/MNISTTester.chpl"
}

#line 32 "../lib/MNISTTester.chpl"
/* MNISTTester.chpl:32 */
#line 32 "../lib/MNISTTester.chpl"
static void coforall_fn_chpl114(int64_t numChunks_chpl,
#line 32 "../lib/MNISTTester.chpl"
                                int64_t parDim_chpl,
#line 32 "../lib/MNISTTester.chpl"
                                _tuple_1_star_range_int64_t_both_one_chpl * locBlock_chpl,
#line 32 "../lib/MNISTTester.chpl"
                                chpl___EndCount_AtomicT_int64_t_int64_t _coforallCount_chpl,
#line 32 "../lib/MNISTTester.chpl"
                                int64_t chunk_chpl,
#line 32 "../lib/MNISTTester.chpl"
                                Error_chpl * error_out_chpl,
#line 32 "../lib/MNISTTester.chpl"
                                SumReduceScanOp__real64_chpl RP_loss_chpl,
#line 32 "../lib/MNISTTester.chpl"
                                SumReduceScanOp_int64_t_chpl RP_acc_chpl,
#line 32 "../lib/MNISTTester.chpl"
                                Network__tuple_4_Conv_Conv_MaxPool_SoftMax_chpl * net_chpl2,
#line 32 "../lib/MNISTTester.chpl"
                                _array_DefaultRectangularArr_1_int64_t_one__tuple_2_Tensor_3__real64_int64_t_int64_t_chpl * data_chpl,
#line 32 "../lib/MNISTTester.chpl"
                                _array_DefaultRectangularArr_1_int64_t_one_Tensor_1__real64_int64_t_chpl * gradients_chpl,
#line 32 "../lib/MNISTTester.chpl"
                                range_int64_t_low_one_chpl * call_tmp_chpl6) {
#line 32 "../lib/MNISTTester.chpl"
  SumReduceScanOp__real64_chpl RP_loss_chpl2 = NULL;
#line 32 "../lib/MNISTTester.chpl"
  _real64 AS_loss_chpl;
#line 32 "../lib/MNISTTester.chpl"
  SumReduceScanOp__real64_chpl new_temp_chpl = NULL;
#line 32 "../lib/MNISTTester.chpl"
  chpl_LocalSpinlock default_arg_l_chpl;
#line 32 "../lib/MNISTTester.chpl"
  chpl_LocalSpinlock ret_chpl;
#line 32 "../lib/MNISTTester.chpl"
  chpl_LocalSpinlock l_chpl;
#line 32 "../lib/MNISTTester.chpl"
  SumReduceScanOp_int64_t_chpl RP_acc_chpl2 = NULL;
#line 32 "../lib/MNISTTester.chpl"
  int64_t AS_acc_chpl;
#line 32 "../lib/MNISTTester.chpl"
  SumReduceScanOp_int64_t_chpl new_temp_chpl2 = NULL;
#line 32 "../lib/MNISTTester.chpl"
  chpl_LocalSpinlock default_arg_l_chpl2;
#line 32 "../lib/MNISTTester.chpl"
  chpl_LocalSpinlock ret_chpl2;
#line 32 "../lib/MNISTTester.chpl"
  chpl_LocalSpinlock l_chpl2;
#line 32 "../lib/MNISTTester.chpl"
  _tuple_1_star_range_int64_t_both_one_chpl followMe_chpl;
#line 32 "../lib/MNISTTester.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl7 = NULL;
#line 32 "../lib/MNISTTester.chpl"
  int64_t call_tmp_chpl8;
#line 32 "../lib/MNISTTester.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl3;
#line 32 "../lib/MNISTTester.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl9 = NULL;
#line 32 "../lib/MNISTTester.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl10 = NULL;
#line 32 "../lib/MNISTTester.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl11 = NULL;
#line 32 "../lib/MNISTTester.chpl"
  _tuple_2_star_int64_t_chpl call_tmp_chpl12;
#line 32 "../lib/MNISTTester.chpl"
  _tuple_2_star_int64_t_chpl ret_tmp_chpl;
#line 32 "../lib/MNISTTester.chpl"
  int64_t tmp_x0_chpl;
#line 32 "../lib/MNISTTester.chpl"
  int64_t tmp_x1_chpl;
#line 32 "../lib/MNISTTester.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl13 = NULL;
#line 32 "../lib/MNISTTester.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl14;
#line 32 "../lib/MNISTTester.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl2;
#line 32 "../lib/MNISTTester.chpl"
  _tuple_1_star_range_int64_t_both_one_chpl _yield_expr_tmp__chpl;
#line 32 "../lib/MNISTTester.chpl"
  _tuple_1_star_range_int64_t_both_one_chpl chpl_followThis;
#line 32 "../lib/MNISTTester.chpl"
  _array_DefaultRectangularArr_1_int64_t_one__tuple_2_Tensor_3__real64_int64_t_int64_t_chpl _ir_F0_this_chpl;
#line 32 "../lib/MNISTTester.chpl"
  range_int64_t_both_one_chpl _ic__F5_block_x0_chpl;
#line 32 "../lib/MNISTTester.chpl"
  range_int64_t_both_one_chpl _ic__F7_coerce_tmp_x0_chpl;
#line 32 "../lib/MNISTTester.chpl"
  int64_t _ic__F8_i_chpl;
#line 32 "../lib/MNISTTester.chpl"
  int64_t _ic__F9_end_chpl;
#line 32 "../lib/MNISTTester.chpl"
  range_int64_t_low_one_chpl coerce_tmp_chpl4;
#line 32 "../lib/MNISTTester.chpl"
  range_int64_t_low_one_chpl _ic__F0_this_chpl;
#line 32 "../lib/MNISTTester.chpl"
  range_int64_t_both_one_chpl _ic__F2_myFollowThis_chpl;
#line 32 "../lib/MNISTTester.chpl"
  range_int64_t_both_one_chpl _ic__F3_r_chpl;
#line 32 "../lib/MNISTTester.chpl"
  int64_t _ic__F5_i_chpl;
#line 32 "../lib/MNISTTester.chpl"
  DefaultRectangularArr_1_int64_t_one__tuple_2_Tensor_3__real64_int64_t_int64_t_chpl this_chpl31 = NULL;
#line 32 "../lib/MNISTTester.chpl"
  DefaultRectangularDom_1_int64_t_one_chpl coerce_tmp_chpl5 = NULL;
#line 32 "../lib/MNISTTester.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl6;
#line 32 "../lib/MNISTTester.chpl"
  DefaultRectangularDom_1_int64_t_one_chpl this_chpl32 = NULL;
#line 32 "../lib/MNISTTester.chpl"
  uint64_t call_tmp_chpl15;
#line 32 "../lib/MNISTTester.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl7;
#line 32 "../lib/MNISTTester.chpl"
  DefaultRectangularDom_1_int64_t_one_chpl this_chpl33 = NULL;
#line 32 "../lib/MNISTTester.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl8;
#line 32 "../lib/MNISTTester.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl9;
#line 32 "../lib/MNISTTester.chpl"
  DefaultRectangularDom_1_int64_t_one_chpl this_chpl34 = NULL;
#line 32 "../lib/MNISTTester.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl10;
#line 32 "../lib/MNISTTester.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl11;
#line 32 "../lib/MNISTTester.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl16 = NULL;
#line 32 "../lib/MNISTTester.chpl"
  range_int64_t_both_one_chpl this_chpl35;
#line 32 "../lib/MNISTTester.chpl"
  range_int64_t_both_one_chpl this_chpl36;
#line 32 "../lib/MNISTTester.chpl"
  _array_DefaultRectangularArr_1_int64_t_one__tuple_2_Tensor_3__real64_int64_t_int64_t_chpl tmp_chpl;
#line 32 "../lib/MNISTTester.chpl"
  DefaultRectangularArr_1_int64_t_one__tuple_2_Tensor_3__real64_int64_t_int64_t_chpl ret_chpl3 = NULL;
#line 32 "../lib/MNISTTester.chpl"
  DefaultRectangularArr_1_int64_t_one__tuple_2_Tensor_3__real64_int64_t_int64_t_chpl _ic__F0_this_chpl2 = NULL;
#line 32 "../lib/MNISTTester.chpl"
  _tuple_1_star_int64_t_chpl ret_tmp_chpl3;
#line 32 "../lib/MNISTTester.chpl"
  range_int64_t_both_one_chpl tup_x0_chpl;
#line 32 "../lib/MNISTTester.chpl"
  range_int64_t_both_one_chpl elt_x0_chpl;
#line 32 "../lib/MNISTTester.chpl"
  int64_t coerce_tmp_chpl12;
#line 32 "../lib/MNISTTester.chpl"
  _ref__tuple_1_star_range_int64_t_both_one_chpl call_tmp_chpl17 = NULL;
#line 32 "../lib/MNISTTester.chpl"
  chpl_bool tmp_chpl2;
#line 32 "../lib/MNISTTester.chpl"
  _ref__tuple_1_star_range_int64_t_both_one_chpl call_tmp_chpl18 = NULL;
#line 32 "../lib/MNISTTester.chpl"
  int64_t call_tmp_chpl19;
#line 32 "../lib/MNISTTester.chpl"
  _ref__tuple_1_star_range_int64_t_both_one_chpl call_tmp_chpl20 = NULL;
#line 32 "../lib/MNISTTester.chpl"
  int64_t call_tmp_chpl21;
#line 32 "../lib/MNISTTester.chpl"
  _tuple_1_star_range_int64_t_both_one_chpl retcopy_chpl;
#line 32 "../lib/MNISTTester.chpl"
  range_int64_t_both_one_chpl _ic__F0_this_chpl3;
#line 32 "../lib/MNISTTester.chpl"
  int64_t coerce_tmp_chpl13;
#line 32 "../lib/MNISTTester.chpl"
  int64_t call_tmp_chpl22;
#line 32 "../lib/MNISTTester.chpl"
  range_int64_t_both_one_chpl this_chpl37;
#line 32 "../lib/MNISTTester.chpl"
  range_int64_t_both_one_chpl tmp_chpl3;
#line 32 "../lib/MNISTTester.chpl"
  range_int64_t_both_one_chpl tmp_chpl4;
#line 32 "../lib/MNISTTester.chpl"
  _ref_range_int64_t_both_one_chpl tmp_chpl5 = NULL;
#line 32 "../lib/MNISTTester.chpl"
  chpl_bool tmp_chpl6;
#line 32 "../lib/MNISTTester.chpl"
  range_int64_t_both_one_chpl tmp_chpl7;
#line 32 "../lib/MNISTTester.chpl"
  range_int64_t_both_one_chpl tmp_chpl8;
#line 32 "../lib/MNISTTester.chpl"
  int64_t call_tmp_chpl23;
#line 32 "../lib/MNISTTester.chpl"
  range_int64_t_both_one_chpl tmp_chpl9;
#line 32 "../lib/MNISTTester.chpl"
  int64_t call_tmp_chpl24;
#line 32 "../lib/MNISTTester.chpl"
  _ref_range_int64_t_both_one_chpl tmp_chpl10 = NULL;
#line 32 "../lib/MNISTTester.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl14;
#line 32 "../lib/MNISTTester.chpl"
  range_int64_t_both_one_chpl _ic__F0_this_chpl4;
#line 32 "../lib/MNISTTester.chpl"
  DefaultRectangularArr_1_int64_t_one__tuple_2_Tensor_3__real64_int64_t_int64_t_chpl this_chpl38 = NULL;
#line 32 "../lib/MNISTTester.chpl"
  _ddata__tuple_2_Tensor_3__real64_int64_t_chpl coerce_tmp_chpl15 = NULL;
#line 32 "../lib/MNISTTester.chpl"
  _ref__tuple_2_Tensor_3__real64_int64_t_chpl call_tmp_chpl25 = NULL;
#line 32 "../lib/MNISTTester.chpl"
  _ref_Tensor_3__real64_chpl im_chpl = NULL;
#line 33 "../lib/MNISTTester.chpl"
  _tuple_3_Tensor_1__real64__real64_chpl_bool_chpl call_tmp_chpl26;
#line 33 "../lib/MNISTTester.chpl"
  _tuple_3_Tensor_1__real64__real64_chpl_bool_chpl ret_tmp_chpl4;
#line 33 "../lib/MNISTTester.chpl"
  Tensor_1__real64_chpl tmp_x0_chpl2;
#line 33 "../lib/MNISTTester.chpl"
  _real64 tmp_x1_chpl2;
#line 33 "../lib/MNISTTester.chpl"
  chpl_bool tmp_x2_chpl;
#line 33 "../lib/MNISTTester.chpl"
  _ref_Tensor_1__real64_chpl call_tmp_chpl27 = NULL;
#line 33 "../lib/MNISTTester.chpl"
  Tensor_1__real64_chpl output_chpl;
#line 34 "../lib/MNISTTester.chpl"
  Tensor_1__real64_chpl call_tmp_chpl28;
#line 34 "../lib/MNISTTester.chpl"
  Tensor_1__real64_chpl ret_tmp_chpl5;
#line 34 "../lib/MNISTTester.chpl"
  Tensor_1__real64_chpl gradient_chpl;
#line 35 "../lib/MNISTTester.chpl"
  _ref__array_DefaultRectangularArr_1_int64_t_one__real64_int64_t_chpl call_tmp_chpl29 = NULL;
#line 35 "../lib/MNISTTester.chpl"
  _tuple_1_star_int64_t_chpl coerce_tmp_chpl16;
#line 35 "../lib/MNISTTester.chpl"
  _ref__tuple_1_star_int64_t_chpl i_i_chpl = NULL;
#line 35 "../lib/MNISTTester.chpl"
  DefaultRectangularArr_1_int64_t_one__real64_int64_t_chpl ret_chpl4 = NULL;
#line 35 "../lib/MNISTTester.chpl"
  DefaultRectangularArr_1_int64_t_one__real64_int64_t_chpl coerce_tmp_chpl17 = NULL;
#line 35 "../lib/MNISTTester.chpl"
  _tuple_1_star_int64_t_chpl _formal_tmp_in_ind_chpl;
#line 35 "../lib/MNISTTester.chpl"
  int64_t _formal_tmp_ind_x0_chpl;
#line 35 "../lib/MNISTTester.chpl"
  _ddata__real64_chpl coerce_tmp_chpl18 = NULL;
#line 35 "../lib/MNISTTester.chpl"
  _ref__real64_chpl call_tmp_chpl30 = NULL;
#line 35 "../lib/MNISTTester.chpl"
  _ref__array_DefaultRectangularArr_1_int64_t_one__real64_int64_t_chpl call_tmp_chpl31 = NULL;
#line 35 "../lib/MNISTTester.chpl"
  _tuple_1_star_int64_t_chpl coerce_tmp_chpl19;
#line 35 "../lib/MNISTTester.chpl"
  _ref__tuple_1_star_int64_t_chpl i_i_chpl2 = NULL;
#line 35 "../lib/MNISTTester.chpl"
  DefaultRectangularArr_1_int64_t_one__real64_int64_t_chpl ret_chpl5 = NULL;
#line 35 "../lib/MNISTTester.chpl"
  DefaultRectangularArr_1_int64_t_one__real64_int64_t_chpl coerce_tmp_chpl20 = NULL;
#line 35 "../lib/MNISTTester.chpl"
  _tuple_1_star_int64_t_chpl _formal_tmp_in_ind_chpl2;
#line 35 "../lib/MNISTTester.chpl"
  int64_t _formal_tmp_ind_x0_chpl2;
#line 35 "../lib/MNISTTester.chpl"
  _ddata__real64_chpl coerce_tmp_chpl21 = NULL;
#line 35 "../lib/MNISTTester.chpl"
  _ref__real64_chpl call_tmp_chpl32 = NULL;
#line 35 "../lib/MNISTTester.chpl"
  _real64 call_tmp_chpl33;
#line 37 "../lib/MNISTTester.chpl"
  _tuple_1_star_int64_t_chpl coerce_tmp_chpl22;
#line 37 "../lib/MNISTTester.chpl"
  _ref__tuple_1_star_int64_t_chpl i_i_chpl3 = NULL;
#line 37 "../lib/MNISTTester.chpl"
  DefaultRectangularArr_1_int64_t_one_Tensor_1__real64_int64_t_chpl ret_chpl6 = NULL;
#line 37 "../lib/MNISTTester.chpl"
  DefaultRectangularArr_1_int64_t_one_Tensor_1__real64_int64_t_chpl coerce_tmp_chpl23 = NULL;
#line 37 "../lib/MNISTTester.chpl"
  _tuple_1_star_int64_t_chpl _formal_tmp_in_ind_chpl3;
#line 37 "../lib/MNISTTester.chpl"
  int64_t _formal_tmp_ind_x0_chpl3;
#line 37 "../lib/MNISTTester.chpl"
  _ddata_Tensor_1__real64_chpl coerce_tmp_chpl24 = NULL;
#line 37 "../lib/MNISTTester.chpl"
  _ref_Tensor_1__real64_chpl call_tmp_chpl34 = NULL;
#line 40 "../lib/MNISTTester.chpl"
  _ref__real64_chpl i_lhs_chpl = NULL;
#line 41 "../lib/MNISTTester.chpl"
  int64_t tmp_chpl11;
#line 41 "../lib/MNISTTester.chpl"
  _ref_int64_t_chpl i_lhs_chpl2 = NULL;
#line 41 "../lib/MNISTTester.chpl"
  _array_DefaultRectangularArr_1_int64_t_one__real64_int64_t_chpl _field_destructor_tmp__chpl;
#line 41 "../lib/MNISTTester.chpl"
  _domain_DefaultRectangularDom_1_int64_t_one_chpl _field_destructor_tmp__chpl2;
#line 41 "../lib/MNISTTester.chpl"
  _array_DefaultRectangularArr_1_int64_t_one__real64_int64_t_chpl _field_destructor_tmp__chpl3;
#line 41 "../lib/MNISTTester.chpl"
  _domain_DefaultRectangularDom_1_int64_t_one_chpl _field_destructor_tmp__chpl4;
#line 41 "../lib/MNISTTester.chpl"
  _array_DefaultRectangularArr_1_int64_t_one__real64_int64_t_chpl _field_destructor_tmp__chpl5;
#line 41 "../lib/MNISTTester.chpl"
  _domain_DefaultRectangularDom_1_int64_t_one_chpl _field_destructor_tmp__chpl6;
#line 32 "../lib/MNISTTester.chpl"
  int64_t tmp_chpl12;
#line 32 "../lib/MNISTTester.chpl"
  int64_t tmp_chpl13;
#line 32 "../lib/MNISTTester.chpl"
  int64_t tmp_chpl14;
#line 32 "../lib/MNISTTester.chpl"
  SumReduceScanOp_int64_t_chpl coerce_tmp_chpl25 = NULL;
#line 32 "../lib/MNISTTester.chpl"
  _ref_int64_t_chpl call_tmp_chpl35 = NULL;
#line 32 "../lib/MNISTTester.chpl"
  SumReduceScanOp__real64_chpl coerce_tmp_chpl26 = NULL;
#line 32 "../lib/MNISTTester.chpl"
  _ref__real64_chpl call_tmp_chpl36 = NULL;
#line 32 "../lib/MNISTTester.chpl"
  init_chpl105(&l_chpl);
#line 32 "../lib/MNISTTester.chpl"
  ret_chpl = l_chpl;
#line 32 "../lib/MNISTTester.chpl"
  default_arg_l_chpl = ret_chpl;
#line 32 "../lib/MNISTTester.chpl"
  new_temp_chpl = _new_chpl23(&default_arg_l_chpl, REAL64(0x0p+0), INT64(32), INT32(66));
#line 32 "../lib/MNISTTester.chpl"
  RP_loss_chpl2 = new_temp_chpl;
#line 32 "../lib/MNISTTester.chpl"
  AS_loss_chpl = REAL64(0x0p+0);
#line 32 "../lib/MNISTTester.chpl"
  init_chpl105(&l_chpl2);
#line 32 "../lib/MNISTTester.chpl"
  ret_chpl2 = l_chpl2;
#line 32 "../lib/MNISTTester.chpl"
  default_arg_l_chpl2 = ret_chpl2;
#line 32 "../lib/MNISTTester.chpl"
  new_temp_chpl2 = _new_chpl21(&default_arg_l_chpl2, INT64(0), INT64(32), INT32(66));
#line 32 "../lib/MNISTTester.chpl"
  RP_acc_chpl2 = new_temp_chpl2;
#line 32 "../lib/MNISTTester.chpl"
  AS_acc_chpl = INT64(0);
#line 32 "../lib/MNISTTester.chpl"
  *(followMe_chpl + INT64(0)) = *(*(locBlock_chpl) + INT64(0));
#line 32 "../lib/MNISTTester.chpl"
  call_tmp_chpl7 = this_chpl20(locBlock_chpl, parDim_chpl, INT64(32), INT32(66));
#line 32 "../lib/MNISTTester.chpl"
  coerce_tmp_chpl3 = *(call_tmp_chpl7);
#line 32 "../lib/MNISTTester.chpl"
  call_tmp_chpl8 = sizeAs_chpl37(coerce_tmp_chpl3, INT64(32), INT32(66));
#line 32 "../lib/MNISTTester.chpl"
  call_tmp_chpl9 = this_chpl20(locBlock_chpl, parDim_chpl, INT64(32), INT32(66));
#line 32 "../lib/MNISTTester.chpl"
  call_tmp_chpl10 = this_chpl20(locBlock_chpl, parDim_chpl, INT64(32), INT32(66));
#line 32 "../lib/MNISTTester.chpl"
  call_tmp_chpl11 = this_chpl20(locBlock_chpl, parDim_chpl, INT64(32), INT32(66));
#line 32 "../lib/MNISTTester.chpl"
  _computeBlock_chpl(call_tmp_chpl8, numChunks_chpl, chunk_chpl, (call_tmp_chpl9)->_high, (call_tmp_chpl10)->_low, (call_tmp_chpl11)->_low, &ret_tmp_chpl, INT64(32), INT32(66));
#line 32 "../lib/MNISTTester.chpl"
  *(call_tmp_chpl12 + INT64(0)) = *(ret_tmp_chpl + INT64(0));
#line 32 "../lib/MNISTTester.chpl"
  *(call_tmp_chpl12 + INT64(1)) = *(ret_tmp_chpl + INT64(1));
#line 32 "../lib/MNISTTester.chpl"
  tmp_x0_chpl = *(call_tmp_chpl12 + INT64(0));
#line 32 "../lib/MNISTTester.chpl"
  tmp_x1_chpl = *(call_tmp_chpl12 + INT64(1));
#line 32 "../lib/MNISTTester.chpl"
  call_tmp_chpl13 = this_chpl20(&followMe_chpl, parDim_chpl, INT64(32), INT32(66));
#line 32 "../lib/MNISTTester.chpl"
  chpl_build_bounded_range6(tmp_x0_chpl, tmp_x1_chpl, &ret_tmp_chpl2);
#line 32 "../lib/MNISTTester.chpl"
  call_tmp_chpl14 = ret_tmp_chpl2;
#line 32 "../lib/MNISTTester.chpl"
  *(call_tmp_chpl13) = call_tmp_chpl14;
#line 32 "../lib/MNISTTester.chpl"
  *(_yield_expr_tmp__chpl + INT64(0)) = *(followMe_chpl + INT64(0));
#line 32 "../lib/MNISTTester.chpl"
  *(chpl_followThis + INT64(0)) = *(_yield_expr_tmp__chpl + INT64(0));
#line 32 "../lib/MNISTTester.chpl"
  _ir_F0_this_chpl = *(data_chpl);
#line 32 "../lib/MNISTTester.chpl"
  coerce_tmp_chpl4 = *(call_tmp_chpl6);
#line 32 "../lib/MNISTTester.chpl"
  _ic__F0_this_chpl = coerce_tmp_chpl4;
#line 32 "../lib/MNISTTester.chpl"
  tmp_chpl = _ir_F0_this_chpl;
#line 32 "../lib/MNISTTester.chpl"
  ret_chpl3 = (&tmp_chpl)->_instance;
#line 32 "../lib/MNISTTester.chpl"
  _ic__F0_this_chpl2 = ret_chpl3;
#line 32 "../lib/MNISTTester.chpl"
  this_chpl31 = ret_chpl3;
#line 32 "../lib/MNISTTester.chpl"
  chpl_check_nil(this_chpl31, INT64(32), INT32(66));
#line 32 "../lib/MNISTTester.chpl"
  coerce_tmp_chpl5 = (this_chpl31)->dom;
#line 32 "../lib/MNISTTester.chpl"
  createTuple_chpl(INT64(0), &ret_tmp_chpl3);
#line 32 "../lib/MNISTTester.chpl"
  init_chpl113(&elt_x0_chpl);
#line 32 "../lib/MNISTTester.chpl"
  tup_x0_chpl = elt_x0_chpl;
#line 32 "../lib/MNISTTester.chpl"
  _ic__F5_block_x0_chpl = tup_x0_chpl;
#line 32 "../lib/MNISTTester.chpl"
  coerce_tmp_chpl6 = *(chpl_followThis + INT64(0));
#line 32 "../lib/MNISTTester.chpl"
  coerce_tmp_chpl12 = (&coerce_tmp_chpl6)->_high;
#line 32 "../lib/MNISTTester.chpl"
  this_chpl32 = coerce_tmp_chpl5;
#line 32 "../lib/MNISTTester.chpl"
  chpl_check_nil(this_chpl32, INT64(32), INT32(66));
#line 32 "../lib/MNISTTester.chpl"
  call_tmp_chpl17 = &((this_chpl32)->ranges);
#line 32 "../lib/MNISTTester.chpl"
  coerce_tmp_chpl7 = *(*(call_tmp_chpl17) + INT64(0));
#line 32 "../lib/MNISTTester.chpl"
  call_tmp_chpl15 = sizeAs_chpl36(coerce_tmp_chpl7, INT64(32), INT32(66));
#line 32 "../lib/MNISTTester.chpl"
  if (! (coerce_tmp_chpl12 < INT64(0))) /* ZLINE: 32 ../lib/MNISTTester.chpl */
#line 32 "../lib/MNISTTester.chpl"
  {
#line 32 "../lib/MNISTTester.chpl"
    tmp_chpl2 = (((uint64_t)(coerce_tmp_chpl12)) >= call_tmp_chpl15);
#line 32 "../lib/MNISTTester.chpl"
  } else /* ZLINE: 32 ../lib/MNISTTester.chpl */
#line 32 "../lib/MNISTTester.chpl"
  {
#line 32 "../lib/MNISTTester.chpl"
    tmp_chpl2 = UINT8(false);
#line 32 "../lib/MNISTTester.chpl"
  }
#line 32 "../lib/MNISTTester.chpl"
  if (tmp_chpl2) /* ZLINE: 32 ../lib/MNISTTester.chpl */
#line 32 "../lib/MNISTTester.chpl"
  {
#line 32 "../lib/MNISTTester.chpl"
    boundsCheckHalt_chpl(&_str_literal_4152_chpl, INT64(32), INT32(66));
#line 32 "../lib/MNISTTester.chpl"
  }
#line 32 "../lib/MNISTTester.chpl"
  this_chpl33 = coerce_tmp_chpl5;
#line 32 "../lib/MNISTTester.chpl"
  chpl_check_nil(this_chpl33, INT64(32), INT32(66));
#line 32 "../lib/MNISTTester.chpl"
  call_tmp_chpl18 = &((this_chpl33)->ranges);
#line 32 "../lib/MNISTTester.chpl"
  coerce_tmp_chpl8 = *(*(call_tmp_chpl18) + INT64(0));
#line 32 "../lib/MNISTTester.chpl"
  coerce_tmp_chpl9 = *(chpl_followThis + INT64(0));
#line 32 "../lib/MNISTTester.chpl"
  call_tmp_chpl19 = ((&coerce_tmp_chpl8)->_low + ((int64_t)(((&coerce_tmp_chpl9)->_low * INT64(1)))));
#line 32 "../lib/MNISTTester.chpl"
  this_chpl34 = coerce_tmp_chpl5;
#line 32 "../lib/MNISTTester.chpl"
  chpl_check_nil(this_chpl34, INT64(32), INT32(66));
#line 32 "../lib/MNISTTester.chpl"
  call_tmp_chpl20 = &((this_chpl34)->ranges);
#line 32 "../lib/MNISTTester.chpl"
  coerce_tmp_chpl10 = *(*(call_tmp_chpl20) + INT64(0));
#line 32 "../lib/MNISTTester.chpl"
  coerce_tmp_chpl11 = *(chpl_followThis + INT64(0));
#line 32 "../lib/MNISTTester.chpl"
  call_tmp_chpl21 = ((&coerce_tmp_chpl10)->_low + ((int64_t)(((&coerce_tmp_chpl11)->_high * INT64(1)))));
#line 32 "../lib/MNISTTester.chpl"
  call_tmp_chpl16 = &_ic__F5_block_x0_chpl;
#line 32 "../lib/MNISTTester.chpl"
  compilerAssert_chpl();
#line 32 "../lib/MNISTTester.chpl"
  (call_tmp_chpl16)->_low = call_tmp_chpl19;
#line 32 "../lib/MNISTTester.chpl"
  (call_tmp_chpl16)->_high = call_tmp_chpl21;
#line 32 "../lib/MNISTTester.chpl"
  *(retcopy_chpl + INT64(0)) = _ic__F5_block_x0_chpl;
#line 32 "../lib/MNISTTester.chpl"
  _ic__F7_coerce_tmp_x0_chpl = *(retcopy_chpl + INT64(0));
#line 32 "../lib/MNISTTester.chpl"
  _ic__F0_this_chpl3 = _ic__F7_coerce_tmp_x0_chpl;
#line 32 "../lib/MNISTTester.chpl"
  checkIfIterWillOverflow_chpl(_ic__F7_coerce_tmp_x0_chpl, UINT8(true), INT64(32), INT32(66));
#line 32 "../lib/MNISTTester.chpl"
  _ic__F8_i_chpl = INT64(0);
#line 32 "../lib/MNISTTester.chpl"
  this_chpl35 = _ic__F0_this_chpl3;
#line 32 "../lib/MNISTTester.chpl"
  coerce_tmp_chpl13 = (&this_chpl35)->_low;
#line 32 "../lib/MNISTTester.chpl"
  this_chpl36 = _ic__F0_this_chpl3;
#line 32 "../lib/MNISTTester.chpl"
  _ic__F9_end_chpl = (&this_chpl36)->_high;
#line 32 "../lib/MNISTTester.chpl"
  tmp_chpl3 = *(chpl_followThis + INT64(0));
#line 32 "../lib/MNISTTester.chpl"
  _ic__F2_myFollowThis_chpl = tmp_chpl3;
#line 32 "../lib/MNISTTester.chpl"
  tmp_chpl4 = tmp_chpl3;
#line 32 "../lib/MNISTTester.chpl"
  if ((&tmp_chpl4)->_low > (&tmp_chpl4)->_high) /* ZLINE: 32 ../lib/MNISTTester.chpl */
#line 32 "../lib/MNISTTester.chpl"
  {
#line 32 "../lib/MNISTTester.chpl"
    tmp_chpl6 = UINT8(false);
#line 32 "../lib/MNISTTester.chpl"
  } else /* ZLINE: 32 ../lib/MNISTTester.chpl */
#line 32 "../lib/MNISTTester.chpl"
  {
#line 32 "../lib/MNISTTester.chpl"
    tmp_chpl6 = UINT8(true);
#line 32 "../lib/MNISTTester.chpl"
  }
#line 32 "../lib/MNISTTester.chpl"
  if (! tmp_chpl6) /* ZLINE: 32 ../lib/MNISTTester.chpl */
#line 32 "../lib/MNISTTester.chpl"
  {
#line 32 "../lib/MNISTTester.chpl"
    tmp_chpl7 = tmp_chpl3;
#line 32 "../lib/MNISTTester.chpl"
    if (! ((&tmp_chpl7)->_low > (&tmp_chpl7)->_high)) /* ZLINE: 32 ../lib/MNISTTester.chpl */
#line 32 "../lib/MNISTTester.chpl"
    {
#line 32 "../lib/MNISTTester.chpl"
      boundsCheckHalt_chpl(&_str_literal_633_chpl, INT64(32), INT32(66));
#line 32 "../lib/MNISTTester.chpl"
    }
#line 32 "../lib/MNISTTester.chpl"
  }
#line 32 "../lib/MNISTTester.chpl"
  call_tmp_chpl22 = sizeAs_chpl37(tmp_chpl3, INT64(32), INT32(66));
#line 32 "../lib/MNISTTester.chpl"
  if (call_tmp_chpl22 != INT64(0)) /* ZLINE: 32 ../lib/MNISTTester.chpl */
#line 32 "../lib/MNISTTester.chpl"
  {
#line 32 "../lib/MNISTTester.chpl"
  }
#line 32 "../lib/MNISTTester.chpl"
  tmp_chpl5 = &_ic__F3_r_chpl;
#line 32 "../lib/MNISTTester.chpl"
  init_chpl113(tmp_chpl5);
#line 32 "../lib/MNISTTester.chpl"
  if (call_tmp_chpl22 != INT64(0)) /* ZLINE: 32 ../lib/MNISTTester.chpl */
#line 32 "../lib/MNISTTester.chpl"
  {
#line 32 "../lib/MNISTTester.chpl"
    tmp_chpl8 = _ic__F2_myFollowThis_chpl;
#line 32 "../lib/MNISTTester.chpl"
    call_tmp_chpl23 = orderToIndex_chpl(coerce_tmp_chpl4, (&tmp_chpl8)->_low, INT64(32), INT32(66));
#line 32 "../lib/MNISTTester.chpl"
    tmp_chpl9 = _ic__F2_myFollowThis_chpl;
#line 32 "../lib/MNISTTester.chpl"
    call_tmp_chpl24 = orderToIndex_chpl(_ic__F0_this_chpl, (&tmp_chpl9)->_high, INT64(32), INT32(66));
#line 32 "../lib/MNISTTester.chpl"
    tmp_chpl10 = &_ic__F3_r_chpl;
#line 32 "../lib/MNISTTester.chpl"
    (tmp_chpl10)->_low = call_tmp_chpl23;
#line 32 "../lib/MNISTTester.chpl"
    (tmp_chpl10)->_high = call_tmp_chpl24;
#line 32 "../lib/MNISTTester.chpl"
    verifyAppropriateStide_chpl2(INT64(1), INT64(32), INT32(66));
#line 32 "../lib/MNISTTester.chpl"
  }
#line 32 "../lib/MNISTTester.chpl"
  coerce_tmp_chpl14 = _ic__F3_r_chpl;
#line 32 "../lib/MNISTTester.chpl"
  _ic__F0_this_chpl4 = coerce_tmp_chpl14;
#line 32 "../lib/MNISTTester.chpl"
  checkIfIterWillOverflow_chpl(coerce_tmp_chpl14, UINT8(true), INT64(32), INT32(66));
#line 32 "../lib/MNISTTester.chpl"
  _ic__F5_i_chpl = INT64(0);
#line 32 "../lib/MNISTTester.chpl"
  this_chpl37 = _ic__F0_this_chpl4;
#line 32 "../lib/MNISTTester.chpl"
  for (_ic__F8_i_chpl = coerce_tmp_chpl13,_ic__F5_i_chpl = (&this_chpl37)->_low; (tmp_chpl12 = (_ic__F8_i_chpl <= _ic__F9_end_chpl),tmp_chpl12); tmp_chpl13 = _ic__F8_i_chpl,tmp_chpl13 += INT64(1),_ic__F8_i_chpl = tmp_chpl13,tmp_chpl14 = _ic__F5_i_chpl,tmp_chpl14 += INT64(1),_ic__F5_i_chpl = tmp_chpl14) {
#line 32 "../lib/MNISTTester.chpl"
    this_chpl38 = _ic__F0_this_chpl2;
#line 32 "../lib/MNISTTester.chpl"
    chpl_check_nil(this_chpl38, INT64(32), INT32(66));
#line 32 "../lib/MNISTTester.chpl"
    coerce_tmp_chpl15 = (this_chpl38)->shiftedData;
#line 32 "../lib/MNISTTester.chpl"
    call_tmp_chpl25 = (coerce_tmp_chpl15 + _ic__F8_i_chpl);
#line 32 "../lib/MNISTTester.chpl"
    im_chpl = &((call_tmp_chpl25)->x0);
#line 33 "../lib/MNISTTester.chpl"
    forward_chpl(net_chpl2, im_chpl, (call_tmp_chpl25)->x1, &ret_tmp_chpl4);
#line 33 "../lib/MNISTTester.chpl"
    call_tmp_chpl26 = ret_tmp_chpl4;
#line 33 "../lib/MNISTTester.chpl"
    tmp_x0_chpl2 = (&call_tmp_chpl26)->x0;
#line 33 "../lib/MNISTTester.chpl"
    tmp_x1_chpl2 = (&call_tmp_chpl26)->x1;
#line 33 "../lib/MNISTTester.chpl"
    tmp_x2_chpl = (&call_tmp_chpl26)->x2;
#line 33 "../lib/MNISTTester.chpl"
    call_tmp_chpl27 = &tmp_x0_chpl2;
#line 33 "../lib/MNISTTester.chpl"
    init_ASSIGN__chpl32(&output_chpl, call_tmp_chpl27);
#line 34 "../lib/MNISTTester.chpl"
    zeros_chpl4(INT64(10), &ret_tmp_chpl5);
#line 34 "../lib/MNISTTester.chpl"
    call_tmp_chpl28 = ret_tmp_chpl5;
#line 34 "../lib/MNISTTester.chpl"
    gradient_chpl = call_tmp_chpl28;
#line 35 "../lib/MNISTTester.chpl"
    call_tmp_chpl29 = &((&gradient_chpl)->data);
#line 35 "../lib/MNISTTester.chpl"
    *(coerce_tmp_chpl16 + INT64(0)) = (call_tmp_chpl25)->x1;
#line 35 "../lib/MNISTTester.chpl"
    i_i_chpl = &coerce_tmp_chpl16;
#line 35 "../lib/MNISTTester.chpl"
    ret_chpl4 = (call_tmp_chpl29)->_instance;
#line 35 "../lib/MNISTTester.chpl"
    checkAccess_chpl11(call_tmp_chpl29, i_i_chpl, ret_chpl4, INT64(35), INT32(66));
#line 35 "../lib/MNISTTester.chpl"
    coerce_tmp_chpl17 = ret_chpl4;
#line 35 "../lib/MNISTTester.chpl"
    *(_formal_tmp_in_ind_chpl + INT64(0)) = *(coerce_tmp_chpl16 + INT64(0));
#line 35 "../lib/MNISTTester.chpl"
    _formal_tmp_ind_x0_chpl = *(_formal_tmp_in_ind_chpl + INT64(0));
#line 35 "../lib/MNISTTester.chpl"
    chpl_check_nil(coerce_tmp_chpl17, INT64(35), INT32(66));
#line 35 "../lib/MNISTTester.chpl"
    coerce_tmp_chpl18 = (coerce_tmp_chpl17)->shiftedData;
#line 35 "../lib/MNISTTester.chpl"
    call_tmp_chpl30 = (coerce_tmp_chpl18 + _formal_tmp_ind_x0_chpl);
#line 35 "../lib/MNISTTester.chpl"
    call_tmp_chpl31 = &((&output_chpl)->data);
#line 35 "../lib/MNISTTester.chpl"
    *(coerce_tmp_chpl19 + INT64(0)) = (call_tmp_chpl25)->x1;
#line 35 "../lib/MNISTTester.chpl"
    i_i_chpl2 = &coerce_tmp_chpl19;
#line 35 "../lib/MNISTTester.chpl"
    ret_chpl5 = (call_tmp_chpl31)->_instance;
#line 35 "../lib/MNISTTester.chpl"
    checkAccess_chpl11(call_tmp_chpl31, i_i_chpl2, ret_chpl5, INT64(35), INT32(66));
#line 35 "../lib/MNISTTester.chpl"
    coerce_tmp_chpl20 = ret_chpl5;
#line 35 "../lib/MNISTTester.chpl"
    *(_formal_tmp_in_ind_chpl2 + INT64(0)) = *(coerce_tmp_chpl19 + INT64(0));
#line 35 "../lib/MNISTTester.chpl"
    _formal_tmp_ind_x0_chpl2 = *(_formal_tmp_in_ind_chpl2 + INT64(0));
#line 35 "../lib/MNISTTester.chpl"
    chpl_check_nil(coerce_tmp_chpl20, INT64(35), INT32(66));
#line 35 "../lib/MNISTTester.chpl"
    coerce_tmp_chpl21 = (coerce_tmp_chpl20)->shiftedData;
#line 35 "../lib/MNISTTester.chpl"
    call_tmp_chpl32 = (coerce_tmp_chpl21 + _formal_tmp_ind_x0_chpl2);
#line 35 "../lib/MNISTTester.chpl"
    call_tmp_chpl33 = (REAL64(0x1p+0) / *(call_tmp_chpl32));
#line 35 "../lib/MNISTTester.chpl"
    *(call_tmp_chpl30) = (-call_tmp_chpl33);
#line 37 "../lib/MNISTTester.chpl"
    *(coerce_tmp_chpl22 + INT64(0)) = _ic__F5_i_chpl;
#line 37 "../lib/MNISTTester.chpl"
    i_i_chpl3 = &coerce_tmp_chpl22;
#line 37 "../lib/MNISTTester.chpl"
    ret_chpl6 = (gradients_chpl)->_instance;
#line 37 "../lib/MNISTTester.chpl"
    checkAccess_chpl2(gradients_chpl, i_i_chpl3, ret_chpl6, INT64(37), INT32(66));
#line 37 "../lib/MNISTTester.chpl"
    coerce_tmp_chpl23 = ret_chpl6;
#line 37 "../lib/MNISTTester.chpl"
    *(_formal_tmp_in_ind_chpl3 + INT64(0)) = *(coerce_tmp_chpl22 + INT64(0));
#line 37 "../lib/MNISTTester.chpl"
    _formal_tmp_ind_x0_chpl3 = *(_formal_tmp_in_ind_chpl3 + INT64(0));
#line 37 "../lib/MNISTTester.chpl"
    chpl_check_nil(coerce_tmp_chpl23, INT64(37), INT32(66));
#line 37 "../lib/MNISTTester.chpl"
    coerce_tmp_chpl24 = (coerce_tmp_chpl23)->shiftedData;
#line 37 "../lib/MNISTTester.chpl"
    call_tmp_chpl34 = (coerce_tmp_chpl24 + _formal_tmp_ind_x0_chpl3);
#line 37 "../lib/MNISTTester.chpl"
    chpl___ASSIGN_25(call_tmp_chpl34, &gradient_chpl);
#line 40 "../lib/MNISTTester.chpl"
    i_lhs_chpl = &AS_loss_chpl;
#line 40 "../lib/MNISTTester.chpl"
    *(i_lhs_chpl) += tmp_x1_chpl2;
#line 41 "../lib/MNISTTester.chpl"
    if (tmp_x2_chpl) /* ZLINE: 41 ../lib/MNISTTester.chpl */
#line 41 "../lib/MNISTTester.chpl"
    {
#line 41 "../lib/MNISTTester.chpl"
      tmp_chpl11 = INT64(1);
#line 41 "../lib/MNISTTester.chpl"
    } else /* ZLINE: 41 ../lib/MNISTTester.chpl */
#line 41 "../lib/MNISTTester.chpl"
    {
#line 41 "../lib/MNISTTester.chpl"
      tmp_chpl11 = INT64(0);
#line 41 "../lib/MNISTTester.chpl"
    }
#line 41 "../lib/MNISTTester.chpl"
    i_lhs_chpl2 = &AS_acc_chpl;
#line 41 "../lib/MNISTTester.chpl"
    *(i_lhs_chpl2) += tmp_chpl11;
#line 41 "../lib/MNISTTester.chpl"
    _field_destructor_tmp__chpl = (&gradient_chpl)->data;
#line 41 "../lib/MNISTTester.chpl"
    chpl__autoDestroy18(&_field_destructor_tmp__chpl, INT64(41), INT32(66));
#line 41 "../lib/MNISTTester.chpl"
    _field_destructor_tmp__chpl2 = (&gradient_chpl)->_domain;
#line 41 "../lib/MNISTTester.chpl"
    chpl__autoDestroy11(&_field_destructor_tmp__chpl2, INT64(41), INT32(66));
#line 41 "../lib/MNISTTester.chpl"
    _field_destructor_tmp__chpl3 = (&output_chpl)->data;
#line 41 "../lib/MNISTTester.chpl"
    chpl__autoDestroy18(&_field_destructor_tmp__chpl3, INT64(41), INT32(66));
#line 41 "../lib/MNISTTester.chpl"
    _field_destructor_tmp__chpl4 = (&output_chpl)->_domain;
#line 41 "../lib/MNISTTester.chpl"
    chpl__autoDestroy11(&_field_destructor_tmp__chpl4, INT64(41), INT32(66));
#line 41 "../lib/MNISTTester.chpl"
    _field_destructor_tmp__chpl5 = (&tmp_x0_chpl2)->data;
#line 41 "../lib/MNISTTester.chpl"
    chpl__autoDestroy18(&_field_destructor_tmp__chpl5, INT64(41), INT32(66));
#line 41 "../lib/MNISTTester.chpl"
    _field_destructor_tmp__chpl6 = (&tmp_x0_chpl2)->_domain;
#line 41 "../lib/MNISTTester.chpl"
    chpl__autoDestroy11(&_field_destructor_tmp__chpl6, INT64(41), INT32(66));
#line 41 "../lib/MNISTTester.chpl"
  }
#line 32 "../lib/MNISTTester.chpl"
  coerce_tmp_chpl25 = RP_acc_chpl2;
#line 32 "../lib/MNISTTester.chpl"
  chpl_check_nil(coerce_tmp_chpl25, INT64(32), INT32(66));
#line 32 "../lib/MNISTTester.chpl"
  call_tmp_chpl35 = &((coerce_tmp_chpl25)->value);
#line 32 "../lib/MNISTTester.chpl"
  *(call_tmp_chpl35) += AS_acc_chpl;
#line 32 "../lib/MNISTTester.chpl"
  chpl__reduceCombine5(RP_acc_chpl, RP_acc_chpl2, INT64(32), INT32(66));
#line 32 "../lib/MNISTTester.chpl"
  chpl__delete16(RP_acc_chpl2, INT64(32), INT32(66));
#line 32 "../lib/MNISTTester.chpl"
  coerce_tmp_chpl26 = RP_loss_chpl2;
#line 32 "../lib/MNISTTester.chpl"
  chpl_check_nil(coerce_tmp_chpl26, INT64(32), INT32(66));
#line 32 "../lib/MNISTTester.chpl"
  call_tmp_chpl36 = &((coerce_tmp_chpl26)->value);
#line 32 "../lib/MNISTTester.chpl"
  *(call_tmp_chpl36) += AS_loss_chpl;
#line 32 "../lib/MNISTTester.chpl"
  chpl__reduceCombine6(RP_loss_chpl, RP_loss_chpl2, INT64(32), INT32(66));
#line 32 "../lib/MNISTTester.chpl"
  chpl__delete33(RP_loss_chpl2, INT64(32), INT32(66));
#line 32 "../lib/MNISTTester.chpl"
  return;
#line 32 "../lib/MNISTTester.chpl"
}

#line 32 "../lib/MNISTTester.chpl"
/* MNISTTester.chpl:32 */
#line 32 "../lib/MNISTTester.chpl"
static void wrapcoforall_fn_chpl114(_class_localscoforall_fn_chpl114 c_chpl) {
#line 32 "../lib/MNISTTester.chpl"
  Error_chpl error_chpl = NULL;
#line 32 "../lib/MNISTTester.chpl"
  int64_t _0_numChunks_chpl;
#line 32 "../lib/MNISTTester.chpl"
  int64_t _1_parDim_chpl;
#line 32 "../lib/MNISTTester.chpl"
  _tuple_1_star_range_int64_t_both_one_chpl _2_coerce_tmp_chpl;
#line 32 "../lib/MNISTTester.chpl"
  chpl___EndCount_AtomicT_int64_t_int64_t _3__coforallCount_chpl = NULL;
#line 32 "../lib/MNISTTester.chpl"
  int64_t _4_chunk_chpl;
#line 32 "../lib/MNISTTester.chpl"
  SumReduceScanOp__real64_chpl _6_globalRP_loss_chpl = NULL;
#line 32 "../lib/MNISTTester.chpl"
  SumReduceScanOp_int64_t_chpl _7_globalRP_acc_chpl = NULL;
#line 32 "../lib/MNISTTester.chpl"
  _ref_Network__tuple_4_Conv_Conv_MaxPool_SoftMax_chpl _8_net_chpl = NULL;
#line 32 "../lib/MNISTTester.chpl"
  _array_DefaultRectangularArr_1_int64_t_one__tuple_2_Tensor_3__real64_int64_t_int64_t_chpl _9_data_chpl;
#line 32 "../lib/MNISTTester.chpl"
  _array_DefaultRectangularArr_1_int64_t_one_Tensor_1__real64_int64_t_chpl _10_gradients_chpl;
#line 32 "../lib/MNISTTester.chpl"
  _ref_range_int64_t_low_one_chpl _11_call_tmp_chpl = NULL;
#line 32 "../lib/MNISTTester.chpl"
  chpl_rmem_consist_acquire(INT64(32), INT32(66));
#line 32 "../lib/MNISTTester.chpl"
  chpl_check_nil(c_chpl, INT64(32), INT32(66));
#line 32 "../lib/MNISTTester.chpl"
  _0_numChunks_chpl = (c_chpl)->_0_numChunks;
#line 32 "../lib/MNISTTester.chpl"
  chpl_check_nil(c_chpl, INT64(32), INT32(66));
#line 32 "../lib/MNISTTester.chpl"
  _1_parDim_chpl = (c_chpl)->_1_parDim;
#line 32 "../lib/MNISTTester.chpl"
  chpl_check_nil(c_chpl, INT64(32), INT32(66));
#line 32 "../lib/MNISTTester.chpl"
  *(_2_coerce_tmp_chpl + INT64(0)) = *((c_chpl)->_2_coerce_tmp + INT64(0));
#line 32 "../lib/MNISTTester.chpl"
  chpl_check_nil(c_chpl, INT64(32), INT32(66));
#line 32 "../lib/MNISTTester.chpl"
  _3__coforallCount_chpl = (c_chpl)->_3__coforallCount;
#line 32 "../lib/MNISTTester.chpl"
  chpl_check_nil(c_chpl, INT64(32), INT32(66));
#line 32 "../lib/MNISTTester.chpl"
  _4_chunk_chpl = (c_chpl)->_4_chunk;
#line 32 "../lib/MNISTTester.chpl"
  chpl_check_nil(c_chpl, INT64(32), INT32(66));
#line 32 "../lib/MNISTTester.chpl"
  _6_globalRP_loss_chpl = (c_chpl)->_6_globalRP_loss;
#line 32 "../lib/MNISTTester.chpl"
  chpl_check_nil(c_chpl, INT64(32), INT32(66));
#line 32 "../lib/MNISTTester.chpl"
  _7_globalRP_acc_chpl = (c_chpl)->_7_globalRP_acc;
#line 32 "../lib/MNISTTester.chpl"
  chpl_check_nil(c_chpl, INT64(32), INT32(66));
#line 32 "../lib/MNISTTester.chpl"
  _8_net_chpl = (c_chpl)->_8_net;
#line 32 "../lib/MNISTTester.chpl"
  chpl_check_nil(c_chpl, INT64(32), INT32(66));
#line 32 "../lib/MNISTTester.chpl"
  _9_data_chpl = (c_chpl)->_9_data;
#line 32 "../lib/MNISTTester.chpl"
  chpl_check_nil(c_chpl, INT64(32), INT32(66));
#line 32 "../lib/MNISTTester.chpl"
  _10_gradients_chpl = (c_chpl)->_10_gradients;
#line 32 "../lib/MNISTTester.chpl"
  chpl_check_nil(c_chpl, INT64(32), INT32(66));
#line 32 "../lib/MNISTTester.chpl"
  _11_call_tmp_chpl = (c_chpl)->_11_call_tmp;
#line 32 "../lib/MNISTTester.chpl"
  coforall_fn_chpl114(_0_numChunks_chpl, _1_parDim_chpl, &_2_coerce_tmp_chpl, _3__coforallCount_chpl, _4_chunk_chpl, &error_chpl, _6_globalRP_loss_chpl, _7_globalRP_acc_chpl, _8_net_chpl, &_9_data_chpl, &_10_gradients_chpl, _11_call_tmp_chpl);
#line 32 "../lib/MNISTTester.chpl"
  chpl_check_nil(c_chpl, INT64(32), INT32(66));
#line 32 "../lib/MNISTTester.chpl"
  _downEndCount_chpl((c_chpl)->_3__coforallCount, error_chpl, INT64(32), INT32(66));
#line 32 "../lib/MNISTTester.chpl"
  return;
#line 32 "../lib/MNISTTester.chpl"
}

#line 43 "../lib/MNISTTester.chpl"
/* MNISTTester.chpl:43 */
#line 43 "../lib/MNISTTester.chpl"
static void chpl__forallexpr12(_array_DefaultRectangularArr_1_int64_t_one__tuple_2_Tensor_3__real64_int64_t_int64_t_chpl * iterExpr_chpl,
#line 43 "../lib/MNISTTester.chpl"
                               _ir_chpl__loopexpr_iter12_chpl * _retArg_chpl) {
#line 43 "../lib/MNISTTester.chpl"
  _ir_chpl__loopexpr_iter12_chpl ret_tmp_chpl;
#line 43 "../lib/MNISTTester.chpl"
  DefaultRectangularDom_1_int64_t_one_chpl _ir__shape__chpl = NULL;
#line 43 "../lib/MNISTTester.chpl"
  _array_DefaultRectangularArr_1_int64_t_one__tuple_2_Tensor_3__real64_int64_t_int64_t_chpl _ir_F0_iterator_chpl;
#line 43 "../lib/MNISTTester.chpl"
  _ir_F0_iterator_chpl = *(iterExpr_chpl);
#line 43 "../lib/MNISTTester.chpl"
  (&ret_tmp_chpl)->_shape_ = _ir__shape__chpl;
#line 43 "../lib/MNISTTester.chpl"
  (&ret_tmp_chpl)->F0_iterator = _ir_F0_iterator_chpl;
#line 43 "../lib/MNISTTester.chpl"
  *(_retArg_chpl) = ret_tmp_chpl;
#line 43 "../lib/MNISTTester.chpl"
  return;
#line 43 "../lib/MNISTTester.chpl"
}

#line 60 "../lib/MNISTTester.chpl"
/* MNISTTester.chpl:60 */
#line 60 "../lib/MNISTTester.chpl"
static void train_chpl2(Network__tuple_4_Conv_Conv_MaxPool_SoftMax_chpl * network_chpl,
#line 60 "../lib/MNISTTester.chpl"
                        int64_t numTrainImages_chpl2,
#line 60 "../lib/MNISTTester.chpl"
                        int64_t numTestImages_chpl2,
#line 60 "../lib/MNISTTester.chpl"
                        _real64 learnRate_chpl2,
#line 60 "../lib/MNISTTester.chpl"
                        int64_t batchSize_chpl2,
#line 60 "../lib/MNISTTester.chpl"
                        int64_t numEpochs_chpl2,
#line 60 "../lib/MNISTTester.chpl"
                        string_chpl * savePath_chpl) {
#line 101 "../lib/MNISTTester.chpl"
  locale_chpl local_nilLocale_chpl;
#line 101 "../lib/MNISTTester.chpl"
  fileWriter_T_nothing_chpl local_stdout_chpl;
#line 71 "../lib/MNISTTester.chpl"
  int64_t local_dataParMinGranularity_chpl;
#line 71 "../lib/MNISTTester.chpl"
  chpl_bool local_dataParIgnoreRunningTasks_chpl;
#line 71 "../lib/MNISTTester.chpl"
  int64_t local_dataParTasksPerLocale_chpl;
#line 68 "../lib/MNISTTester.chpl"
  int64_t call_tmp_chpl6;
#line 68 "../lib/MNISTTester.chpl"
  int64_t numImages_chpl;
#line 70 "../lib/MNISTTester.chpl"
  _array_DefaultRectangularArr_1_int64_t_one__array_DefaultRectangularArr_2_int64_t_one__real64_int64_t_int64_t_chpl call_tmp_chpl7;
#line 70 "../lib/MNISTTester.chpl"
  _array_DefaultRectangularArr_1_int64_t_one__array_DefaultRectangularArr_2_int64_t_one__real64_int64_t_int64_t_chpl ret_tmp_chpl;
#line 70 "../lib/MNISTTester.chpl"
  _array_DefaultRectangularArr_1_int64_t_one__array_DefaultRectangularArr_2_int64_t_one__real64_int64_t_int64_t_chpl imageRawData_chpl;
#line 71 "../lib/MNISTTester.chpl"
  _ref__array_DefaultRectangularArr_1_int64_t_one__array_DefaultRectangularArr_2_int64_t_one__real64_int64_t_int64_t_chpl i_lhs_chpl = NULL;
#line 71 "../lib/MNISTTester.chpl"
  _ir_chpl_promo2__HYPHEN__chpl call_tmp_chpl8;
#line 71 "../lib/MNISTTester.chpl"
  DefaultRectangularDom_1_int64_t_one_chpl _ir__shape__chpl = NULL;
#line 71 "../lib/MNISTTester.chpl"
  _array_DefaultRectangularArr_1_int64_t_one__array_DefaultRectangularArr_2_int64_t_one__real64_int64_t_int64_t_chpl _ir_F0_a_chpl;
#line 71 "../lib/MNISTTester.chpl"
  _domain_DefaultRectangularDom_1_int64_t_one_chpl call_tmp_chpl9;
#line 71 "../lib/MNISTTester.chpl"
  _domain_DefaultRectangularDom_1_int64_t_one_chpl ret_tmp_chpl2;
#line 71 "../lib/MNISTTester.chpl"
  DefaultRectangularDom_1_int64_t_one_chpl ret_chpl = NULL;
#line 71 "../lib/MNISTTester.chpl"
  DefaultRectangularArr_1_int64_t_one__array_DefaultRectangularArr_2_int64_t_one__real64_int64_t_int64_t_chpl coerce_tmp_chpl3 = NULL;
#line 71 "../lib/MNISTTester.chpl"
  int64_t tasksPerLocale_chpl;
#line 71 "../lib/MNISTTester.chpl"
  chpl_bool ignoreRunning_chpl;
#line 71 "../lib/MNISTTester.chpl"
  int64_t minIndicesPerTask_chpl;
#line 71 "../lib/MNISTTester.chpl"
  Error_chpl error_chpl = NULL;
#line 71 "../lib/MNISTTester.chpl"
  DefaultRectangularDom_1_int64_t_one_chpl coerce_tmp_chpl4 = NULL;
#line 71 "../lib/MNISTTester.chpl"
  int64_t offset_x0_chpl;
#line 71 "../lib/MNISTTester.chpl"
  _tuple_1_star_int64_t_chpl ret_tmp_chpl3;
#line 71 "../lib/MNISTTester.chpl"
  Error_chpl error_chpl2 = NULL;
#line 71 "../lib/MNISTTester.chpl"
  locale_chpl call_tmp_chpl10;
#line 71 "../lib/MNISTTester.chpl"
  chpl_localeID_t call_tmp_chpl11;
#line 71 "../lib/MNISTTester.chpl"
  int32_t call_tmp_chpl12;
#line 71 "../lib/MNISTTester.chpl"
  chpl_localeID_t call_tmp_chpl13;
#line 71 "../lib/MNISTTester.chpl"
  locale_chpl call_tmp_chpl14;
#line 71 "../lib/MNISTTester.chpl"
  locale_chpl ret_tmp_chpl4;
#line 71 "../lib/MNISTTester.chpl"
  BaseLocale_chpl coerce_tmp_chpl5 = NULL;
#line 71 "../lib/MNISTTester.chpl"
  _ref_locale_chpl i_x_chpl = NULL;
#line 71 "../lib/MNISTTester.chpl"
  int64_t tmp_chpl;
#line 71 "../lib/MNISTTester.chpl"
  locale_chpl call_tmp_chpl15;
#line 71 "../lib/MNISTTester.chpl"
  chpl_localeID_t call_tmp_chpl16;
#line 71 "../lib/MNISTTester.chpl"
  int32_t call_tmp_chpl17;
#line 71 "../lib/MNISTTester.chpl"
  chpl_localeID_t call_tmp_chpl18;
#line 71 "../lib/MNISTTester.chpl"
  locale_chpl call_tmp_chpl19;
#line 71 "../lib/MNISTTester.chpl"
  locale_chpl ret_tmp_chpl5;
#line 71 "../lib/MNISTTester.chpl"
  int64_t ret_chpl2;
#line 71 "../lib/MNISTTester.chpl"
  BaseLocale_chpl coerce_tmp_chpl6 = NULL;
#line 71 "../lib/MNISTTester.chpl"
  BaseLocale_chpl coerce_tmp_chpl7 = NULL;
#line 71 "../lib/MNISTTester.chpl"
  _ref_locale_chpl i_x_chpl2 = NULL;
#line 71 "../lib/MNISTTester.chpl"
  int64_t tmp_x0_chpl;
#line 71 "../lib/MNISTTester.chpl"
  int64_t tmp_x1_chpl;
#line 71 "../lib/MNISTTester.chpl"
  _ref__tuple_1_star_range_int64_t_both_one_chpl call_tmp_chpl20 = NULL;
#line 71 "../lib/MNISTTester.chpl"
  _tuple_2_star_int64_t_chpl ret_tmp_chpl6;
#line 71 "../lib/MNISTTester.chpl"
  int64_t numChunks_chpl;
#line 71 "../lib/MNISTTester.chpl"
  int64_t parDim_chpl;
#line 71 "../lib/MNISTTester.chpl"
  _tuple_1_star_range_int64_t_both_one_chpl locBlock_chpl;
#line 71 "../lib/MNISTTester.chpl"
  range_int64_t_both_one_chpl tup_x0_chpl;
#line 71 "../lib/MNISTTester.chpl"
  range_int64_t_both_one_chpl elt_x0_chpl;
#line 71 "../lib/MNISTTester.chpl"
  range_int64_t_low_one_chpl call_tmp_chpl21;
#line 71 "../lib/MNISTTester.chpl"
  range_int64_t_low_one_chpl ret_tmp_chpl7;
#line 71 "../lib/MNISTTester.chpl"
  _ref__tuple_1_star_range_int64_t_both_one_chpl call_tmp_chpl22 = NULL;
#line 71 "../lib/MNISTTester.chpl"
  int64_t call_tmp_chpl23;
#line 71 "../lib/MNISTTester.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl8;
#line 71 "../lib/MNISTTester.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl24;
#line 71 "../lib/MNISTTester.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl8;
#line 71 "../lib/MNISTTester.chpl"
  range_int64_t_both_one_chpl tmpIter_chpl;
#line 71 "../lib/MNISTTester.chpl"
  range_int64_t_low_one_chpl call_tmp_chpl25;
#line 71 "../lib/MNISTTester.chpl"
  range_int64_t_low_one_chpl ret_tmp_chpl9;
#line 71 "../lib/MNISTTester.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl10;
#line 71 "../lib/MNISTTester.chpl"
  chpl___EndCount_AtomicT_int64_t_int64_t _coforallCount_chpl = NULL;
#line 71 "../lib/MNISTTester.chpl"
  chpl___EndCount_AtomicT_int64_t_int64_t new_temp_chpl = NULL;
#line 71 "../lib/MNISTTester.chpl"
  int64_t numTasks_chpl;
#line 71 "../lib/MNISTTester.chpl"
  range_int64_t_both_one_chpl _ic__F0_this_chpl;
#line 71 "../lib/MNISTTester.chpl"
  int64_t i_chpl;
#line 71 "../lib/MNISTTester.chpl"
  range_int64_t_both_one_chpl this_chpl31;
#line 71 "../lib/MNISTTester.chpl"
  range_int64_t_both_one_chpl this_chpl32;
#line 71 "../lib/MNISTTester.chpl"
  int64_t end_chpl;
#line 71 "../lib/MNISTTester.chpl"
  _class_localscoforall_fn_chpl19 _args_forcoforall_fn_chpl = NULL;
#line 71 "../lib/MNISTTester.chpl"
  _ref__ir_chpl_promo2__HYPHEN__chpl tmp_chpl2 = NULL;
#line 71 "../lib/MNISTTester.chpl"
  raw_c_void_ptr _args_vforcoforall_fn_chpl;
#line 71 "../lib/MNISTTester.chpl"
  int64_t _args_sizecoforall_fn_chpl;
#line 71 "../lib/MNISTTester.chpl"
  Error_chpl error_chpl3 = NULL;
#line 72 "../lib/MNISTTester.chpl"
  _tuple_2__array_DefaultRectangularArr_1_int64_t_one_int64_t_int64_t__array_DefaultRectangularArr_1_int64_t_one__array_DefaultRectangularArr_1_int64_t_one__real64_int64_t_int64_t_chpl call_tmp_chpl26;
#line 72 "../lib/MNISTTester.chpl"
  _tuple_2__array_DefaultRectangularArr_1_int64_t_one_int64_t_int64_t__array_DefaultRectangularArr_1_int64_t_one__array_DefaultRectangularArr_1_int64_t_one__real64_int64_t_int64_t_chpl ret_tmp_chpl11;
#line 72 "../lib/MNISTTester.chpl"
  _array_DefaultRectangularArr_1_int64_t_one_int64_t_int64_t_chpl tmp_x0_chpl2;
#line 72 "../lib/MNISTTester.chpl"
  _array_DefaultRectangularArr_1_int64_t_one__array_DefaultRectangularArr_1_int64_t_one__real64_int64_t_int64_t_chpl tmp_x1_chpl2;
#line 72 "../lib/MNISTTester.chpl"
  _ref__array_DefaultRectangularArr_1_int64_t_one_int64_t_int64_t_chpl call_tmp_chpl27 = NULL;
#line 72 "../lib/MNISTTester.chpl"
  _array_DefaultRectangularArr_1_int64_t_one_int64_t_int64_t_chpl labels_chpl;
#line 72 "../lib/MNISTTester.chpl"
  _ref__array_DefaultRectangularArr_1_int64_t_one__array_DefaultRectangularArr_1_int64_t_one__real64_int64_t_int64_t_chpl call_tmp_chpl28 = NULL;
#line 72 "../lib/MNISTTester.chpl"
  _array_DefaultRectangularArr_1_int64_t_one__array_DefaultRectangularArr_1_int64_t_one__real64_int64_t_int64_t_chpl labelVectors_chpl;
#line 75 "../lib/MNISTTester.chpl"
  _array_DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl images_chpl;
#line 75 "../lib/MNISTTester.chpl"
  _ir_chpl__loopexpr_iter13_chpl call_tmp_chpl29;
#line 75 "../lib/MNISTTester.chpl"
  _ir_chpl__loopexpr_iter13_chpl ret_tmp_chpl12;
#line 75 "../lib/MNISTTester.chpl"
  _ref__array_DefaultRectangularArr_1_int64_t_one__array_DefaultRectangularArr_2_int64_t_one__real64_int64_t_int64_t_chpl i_arg_chpl = NULL;
#line 75 "../lib/MNISTTester.chpl"
  _domain_DefaultRectangularDom_1_int64_t_one_chpl call_tmp_chpl30;
#line 75 "../lib/MNISTTester.chpl"
  _domain_DefaultRectangularDom_1_int64_t_one_chpl ret_tmp_chpl13;
#line 75 "../lib/MNISTTester.chpl"
  DefaultRectangularDom_1_int64_t_one_chpl ret_chpl3 = NULL;
#line 76 "../lib/MNISTTester.chpl"
  _array_DefaultRectangularArr_1_int64_t_one__tuple_2_Tensor_3__real64_int64_t_int64_t_chpl labeledImages_chpl;
#line 76 "../lib/MNISTTester.chpl"
  _tuple_2__ref__array_DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t__ref__array_DefaultRectangularArr_1_int64_t_one_int64_t_int64_t_chpl call_tmp_chpl31;
#line 76 "../lib/MNISTTester.chpl"
  _tuple_2__ref__array_DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t__ref__array_DefaultRectangularArr_1_int64_t_one_int64_t_int64_t_chpl ret_tmp_chpl14;
#line 76 "../lib/MNISTTester.chpl"
  _ir_chpl__loopexpr_iter14_chpl call_tmp_chpl32;
#line 76 "../lib/MNISTTester.chpl"
  _ir_chpl__loopexpr_iter14_chpl ret_tmp_chpl15;
#line 76 "../lib/MNISTTester.chpl"
  _ref__array_DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl i_arg_chpl2 = NULL;
#line 76 "../lib/MNISTTester.chpl"
  _domain_DefaultRectangularDom_1_int64_t_one_chpl call_tmp_chpl33;
#line 76 "../lib/MNISTTester.chpl"
  _domain_DefaultRectangularDom_1_int64_t_one_chpl ret_tmp_chpl16;
#line 76 "../lib/MNISTTester.chpl"
  DefaultRectangularDom_1_int64_t_one_chpl ret_chpl4 = NULL;
#line 80 "../lib/MNISTTester.chpl"
  range_int64_t_low_one_chpl call_tmp_chpl34;
#line 80 "../lib/MNISTTester.chpl"
  range_int64_t_low_one_chpl ret_tmp_chpl17;
#line 80 "../lib/MNISTTester.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl35;
#line 80 "../lib/MNISTTester.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl18;
#line 80 "../lib/MNISTTester.chpl"
  _array_ArrayViewSliceArr__tuple_2_Tensor_3__real64_int64_t_int64_t_DefaultRectangularDom_1_int64_t_one_int64_t_DefaultRectangularArr_1_int64_t_one__tuple_2_Tensor_3__real64_int64_t_int64_t_chpl_bool_chpl call_tmp_chpl36;
#line 80 "../lib/MNISTTester.chpl"
  _array_ArrayViewSliceArr__tuple_2_Tensor_3__real64_int64_t_int64_t_DefaultRectangularDom_1_int64_t_one_int64_t_DefaultRectangularArr_1_int64_t_one__tuple_2_Tensor_3__real64_int64_t_int64_t_chpl_bool_chpl ret_tmp_chpl19;
#line 80 "../lib/MNISTTester.chpl"
  _array_DefaultRectangularArr_1_int64_t_one__tuple_2_Tensor_3__real64_int64_t_int64_t_chpl trainingData_chpl;
#line 81 "../lib/MNISTTester.chpl"
  range_int64_t_low_one_chpl call_tmp_chpl37;
#line 81 "../lib/MNISTTester.chpl"
  range_int64_t_low_one_chpl ret_tmp_chpl20;
#line 81 "../lib/MNISTTester.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl38;
#line 81 "../lib/MNISTTester.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl21;
#line 81 "../lib/MNISTTester.chpl"
  _array_ArrayViewSliceArr__tuple_2_Tensor_3__real64_int64_t_int64_t_DefaultRectangularDom_1_int64_t_one_int64_t_DefaultRectangularArr_1_int64_t_one__tuple_2_Tensor_3__real64_int64_t_int64_t_chpl_bool_chpl call_tmp_chpl39;
#line 81 "../lib/MNISTTester.chpl"
  _array_ArrayViewSliceArr__tuple_2_Tensor_3__real64_int64_t_int64_t_DefaultRectangularDom_1_int64_t_one_int64_t_DefaultRectangularArr_1_int64_t_one__tuple_2_Tensor_3__real64_int64_t_int64_t_chpl_bool_chpl ret_tmp_chpl22;
#line 81 "../lib/MNISTTester.chpl"
  _array_DefaultRectangularArr_1_int64_t_one__tuple_2_Tensor_3__real64_int64_t_int64_t_chpl testingData_chpl;
#line 83 "../lib/MNISTTester.chpl"
  _ref__array_DefaultRectangularArr_1_int64_t_one__tuple_2_Tensor_3__real64_int64_t_int64_t_chpl i_this_chpl = NULL;
#line 83 "../lib/MNISTTester.chpl"
  _tuple_1_star_int64_t_chpl coerce_tmp_chpl9;
#line 83 "../lib/MNISTTester.chpl"
  _ref__tuple_1_star_int64_t_chpl i_i_chpl = NULL;
#line 83 "../lib/MNISTTester.chpl"
  DefaultRectangularArr_1_int64_t_one__tuple_2_Tensor_3__real64_int64_t_int64_t_chpl ret_chpl5 = NULL;
#line 83 "../lib/MNISTTester.chpl"
  DefaultRectangularArr_1_int64_t_one__tuple_2_Tensor_3__real64_int64_t_int64_t_chpl coerce_tmp_chpl10 = NULL;
#line 83 "../lib/MNISTTester.chpl"
  _tuple_1_star_int64_t_chpl _formal_tmp_in_ind_chpl;
#line 83 "../lib/MNISTTester.chpl"
  int64_t _formal_tmp_ind_x0_chpl;
#line 83 "../lib/MNISTTester.chpl"
  _ddata__tuple_2_Tensor_3__real64_int64_t_chpl coerce_tmp_chpl11 = NULL;
#line 83 "../lib/MNISTTester.chpl"
  _ref__tuple_2_Tensor_3__real64_int64_t_chpl call_tmp_chpl40 = NULL;
#line 83 "../lib/MNISTTester.chpl"
  _ref_Tensor_3__real64_chpl call_tmp_chpl41 = NULL;
#line 83 "../lib/MNISTTester.chpl"
  Tensor_1__real64_chpl _return_tmp__chpl;
#line 83 "../lib/MNISTTester.chpl"
  Tensor_1__real64_chpl ret_tmp_chpl23;
#line 83 "../lib/MNISTTester.chpl"
  _array_DefaultRectangularArr_1_int64_t_one__real64_int64_t_chpl _field_destructor_tmp__chpl;
#line 83 "../lib/MNISTTester.chpl"
  _domain_DefaultRectangularDom_1_int64_t_one_chpl _field_destructor_tmp__chpl2;
#line 86 "../lib/MNISTTester.chpl"
  stopwatch_chpl call_tmp_chpl42;
#line 86 "../lib/MNISTTester.chpl"
  stopwatch_chpl initTemp_chpl;
#line 86 "../lib/MNISTTester.chpl"
  _timevalue call_tmp_chpl43;
#line 86 "../lib/MNISTTester.chpl"
  stopwatch_chpl t_chpl;
#line 89 "../lib/MNISTTester.chpl"
  int64_t tmp_chpl3;
#line 89 "../lib/MNISTTester.chpl"
  int64_t _ic__F1_high_chpl;
#line 89 "../lib/MNISTTester.chpl"
  int64_t i_chpl2;
#line 89 "../lib/MNISTTester.chpl"
  int64_t epoch_chpl;
#line 92 "../lib/MNISTTester.chpl"
  _ref__array_DefaultRectangularArr_1_int64_t_one__tuple_2_Tensor_3__real64_int64_t_int64_t_chpl i_this_chpl2 = NULL;
#line 92 "../lib/MNISTTester.chpl"
  _tuple_1_star_int64_t_chpl coerce_tmp_chpl12;
#line 92 "../lib/MNISTTester.chpl"
  _ref__tuple_1_star_int64_t_chpl i_i_chpl2 = NULL;
#line 92 "../lib/MNISTTester.chpl"
  DefaultRectangularArr_1_int64_t_one__tuple_2_Tensor_3__real64_int64_t_int64_t_chpl ret_chpl6 = NULL;
#line 92 "../lib/MNISTTester.chpl"
  DefaultRectangularArr_1_int64_t_one__tuple_2_Tensor_3__real64_int64_t_int64_t_chpl coerce_tmp_chpl13 = NULL;
#line 92 "../lib/MNISTTester.chpl"
  _tuple_1_star_int64_t_chpl _formal_tmp_in_ind_chpl2;
#line 92 "../lib/MNISTTester.chpl"
  int64_t _formal_tmp_ind_x0_chpl2;
#line 92 "../lib/MNISTTester.chpl"
  _ddata__tuple_2_Tensor_3__real64_int64_t_chpl coerce_tmp_chpl14 = NULL;
#line 92 "../lib/MNISTTester.chpl"
  _ref__tuple_2_Tensor_3__real64_int64_t_chpl call_tmp_chpl44 = NULL;
#line 92 "../lib/MNISTTester.chpl"
  _ref_Tensor_3__real64_chpl call_tmp_chpl45 = NULL;
#line 92 "../lib/MNISTTester.chpl"
  Tensor_1__real64_chpl _return_tmp__chpl2;
#line 92 "../lib/MNISTTester.chpl"
  Tensor_1__real64_chpl ret_tmp_chpl24;
#line 92 "../lib/MNISTTester.chpl"
  _array_DefaultRectangularArr_1_int64_t_one__real64_int64_t_chpl _field_destructor_tmp__chpl3;
#line 92 "../lib/MNISTTester.chpl"
  _domain_DefaultRectangularDom_1_int64_t_one_chpl _field_destructor_tmp__chpl4;
#line 96 "../lib/MNISTTester.chpl"
  int64_t call_tmp_chpl46;
#line 96 "../lib/MNISTTester.chpl"
  int64_t call_tmp_chpl47;
#line 96 "../lib/MNISTTester.chpl"
  int64_t tmp_chpl4;
#line 96 "../lib/MNISTTester.chpl"
  int64_t _ic__F1_high_chpl2;
#line 96 "../lib/MNISTTester.chpl"
  int64_t i_chpl3;
#line 97 "../lib/MNISTTester.chpl"
  range_int64_t_low_one_chpl call_tmp_chpl48;
#line 97 "../lib/MNISTTester.chpl"
  range_int64_t_low_one_chpl ret_tmp_chpl25;
#line 97 "../lib/MNISTTester.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl49;
#line 97 "../lib/MNISTTester.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl26;
#line 98 "../lib/MNISTTester.chpl"
  _array_ArrayViewSliceArr__tuple_2_Tensor_3__real64_int64_t_int64_t_DefaultRectangularDom_1_int64_t_one_int64_t_DefaultRectangularArr_1_int64_t_one__tuple_2_Tensor_3__real64_int64_t_int64_t_chpl_bool_chpl call_tmp_chpl50;
#line 98 "../lib/MNISTTester.chpl"
  _array_ArrayViewSliceArr__tuple_2_Tensor_3__real64_int64_t_int64_t_DefaultRectangularDom_1_int64_t_one_int64_t_DefaultRectangularArr_1_int64_t_one__tuple_2_Tensor_3__real64_int64_t_int64_t_chpl_bool_chpl ret_tmp_chpl27;
#line 98 "../lib/MNISTTester.chpl"
  _array_DefaultRectangularArr_1_int64_t_one__tuple_2_Tensor_3__real64_int64_t_int64_t_chpl batch_chpl;
#line 99 "../lib/MNISTTester.chpl"
  _tuple_2__real64_int64_t_chpl call_tmp_chpl51;
#line 99 "../lib/MNISTTester.chpl"
  _tuple_2__real64_int64_t_chpl ret_tmp_chpl28;
#line 99 "../lib/MNISTTester.chpl"
  _real64 tmp_x0_chpl3;
#line 99 "../lib/MNISTTester.chpl"
  int64_t tmp_x1_chpl3;
#line 101 "../lib/MNISTTester.chpl"
  int64_t call_tmp_chpl52;
#line 101 "../lib/MNISTTester.chpl"
  int64_t call_tmp_chpl53;
#line 101 "../lib/MNISTTester.chpl"
  int64_t call_tmp_chpl54;
#line 101 "../lib/MNISTTester.chpl"
  _real64 call_tmp_chpl55;
#line 101 "../lib/MNISTTester.chpl"
  Error_chpl error_chpl4 = NULL;
#line 101 "../lib/MNISTTester.chpl"
  _ref_fileWriter_T_nothing_chpl i_this_chpl3 = NULL;
#line 101 "../lib/MNISTTester.chpl"
  string_chpl inlineImm_chpl;
#line 101 "../lib/MNISTTester.chpl"
  _ref_string_chpl i__e0_args_chpl = NULL;
#line 101 "../lib/MNISTTester.chpl"
  string_chpl inlineImm_chpl2;
#line 101 "../lib/MNISTTester.chpl"
  _ref_string_chpl i__e1_args_chpl = NULL;
#line 101 "../lib/MNISTTester.chpl"
  string_chpl inlineImm_chpl3;
#line 101 "../lib/MNISTTester.chpl"
  _ref_string_chpl i__e3_args_chpl = NULL;
#line 101 "../lib/MNISTTester.chpl"
  string_chpl inlineImm_chpl4;
#line 101 "../lib/MNISTTester.chpl"
  _ref_string_chpl i__e5_args_chpl = NULL;
#line 101 "../lib/MNISTTester.chpl"
  string_chpl inlineImm_chpl5;
#line 101 "../lib/MNISTTester.chpl"
  _ref_string_chpl i__e7_args_chpl = NULL;
#line 101 "../lib/MNISTTester.chpl"
  string_chpl inlineImm_chpl6;
#line 101 "../lib/MNISTTester.chpl"
  _ref_string_chpl i__e9_args_chpl = NULL;
#line 101 "../lib/MNISTTester.chpl"
  string_chpl inlineImm_chpl7;
#line 101 "../lib/MNISTTester.chpl"
  _ref_string_chpl i__e11_args_chpl = NULL;
#line 101 "../lib/MNISTTester.chpl"
  _tuple_12__ref_string__ref_string_int64_t__ref_string_int64_t__ref_string__real64__ref_string_int64_t__ref_string_int64_t__ref_string_chpl args_chpl;
#line 101 "../lib/MNISTTester.chpl"
  _tuple_12__ref_string__ref_string_int64_t__ref_string_int64_t__ref_string__real64__ref_string_int64_t__ref_string_int64_t__ref_string_chpl this_chpl33;
#line 101 "../lib/MNISTTester.chpl"
  locale_chpl ret_chpl7;
#line 101 "../lib/MNISTTester.chpl"
  _ref_locale_chpl call_tmp_chpl56 = NULL;
#line 101 "../lib/MNISTTester.chpl"
  locale_chpl ret_chpl8;
#line 101 "../lib/MNISTTester.chpl"
  chpl_bool call_tmp_chpl57;
#line 101 "../lib/MNISTTester.chpl"
  locale_chpl call_tmp_chpl58;
#line 101 "../lib/MNISTTester.chpl"
  chpl_localeID_t call_tmp_chpl59;
#line 101 "../lib/MNISTTester.chpl"
  int32_t call_tmp_chpl60;
#line 101 "../lib/MNISTTester.chpl"
  chpl_localeID_t call_tmp_chpl61;
#line 101 "../lib/MNISTTester.chpl"
  locale_chpl call_tmp_chpl62;
#line 101 "../lib/MNISTTester.chpl"
  locale_chpl ret_tmp_chpl29;
#line 101 "../lib/MNISTTester.chpl"
  _ref_locale_chpl i_x_chpl3 = NULL;
#line 101 "../lib/MNISTTester.chpl"
  locale_chpl origLocale_chpl;
#line 101 "../lib/MNISTTester.chpl"
  _ref_locale_chpl call_tmp_chpl63 = NULL;
#line 101 "../lib/MNISTTester.chpl"
  Error_chpl error_chpl5 = NULL;
#line 101 "../lib/MNISTTester.chpl"
  _owned_SystemError_chpl call_tmp_chpl64;
#line 101 "../lib/MNISTTester.chpl"
  _owned_SystemError_chpl ret_tmp_chpl30;
#line 101 "../lib/MNISTTester.chpl"
  Error_chpl call_tmp_chpl65 = NULL;
#line 101 "../lib/MNISTTester.chpl"
  _owned_Error_chpl coerce_tmp_chpl15;
#line 101 "../lib/MNISTTester.chpl"
  _owned_Error_chpl ret_tmp_chpl31;
#line 101 "../lib/MNISTTester.chpl"
  _ref__owned_SystemError_chpl i_x_chpl4 = NULL;
#line 101 "../lib/MNISTTester.chpl"
  _ref__owned_Error_chpl2 i__retArg_chpl = NULL;
#line 101 "../lib/MNISTTester.chpl"
  _owned_SystemError_chpl _formal_tmp_x_chpl;
#line 101 "../lib/MNISTTester.chpl"
  _owned_Error_chpl ret_chpl9;
#line 101 "../lib/MNISTTester.chpl"
  _ref__owned_SystemError_chpl i_this_chpl4 = NULL;
#line 101 "../lib/MNISTTester.chpl"
  Error_chpl call_tmp_chpl66 = NULL;
#line 101 "../lib/MNISTTester.chpl"
  _ref__owned_SystemError_chpl i_this_chpl5 = NULL;
#line 101 "../lib/MNISTTester.chpl"
  _owned_Error_chpl call_tmp_chpl67;
#line 101 "../lib/MNISTTester.chpl"
  _owned_Error_chpl initTemp_chpl2;
#line 101 "../lib/MNISTTester.chpl"
  _owned_Error_chpl _formal_tmp_in_err_chpl;
#line 101 "../lib/MNISTTester.chpl"
  _ref_locale_chpl call_tmp_chpl68 = NULL;
#line 101 "../lib/MNISTTester.chpl"
  syserr call_tmp_chpl69;
#line 101 "../lib/MNISTTester.chpl"
  int32_t call_tmp_chpl70;
#line 101 "../lib/MNISTTester.chpl"
  Error_chpl error_chpl6 = NULL;
#line 101 "../lib/MNISTTester.chpl"
  _ref_locale_chpl i_x_chpl5 = NULL;
#line 101 "../lib/MNISTTester.chpl"
  Error_chpl error_chpl7 = NULL;
#line 101 "../lib/MNISTTester.chpl"
  _ref_string_chpl call_tmp_chpl71 = NULL;
#line 101 "../lib/MNISTTester.chpl"
  _ref_locale_chpl call_tmp_chpl72 = NULL;
#line 101 "../lib/MNISTTester.chpl"
  _ref_locale_chpl i_x_chpl6 = NULL;
#line 101 "../lib/MNISTTester.chpl"
  Error_chpl error_chpl8 = NULL;
#line 101 "../lib/MNISTTester.chpl"
  _ref_string_chpl call_tmp_chpl73 = NULL;
#line 101 "../lib/MNISTTester.chpl"
  _ref_locale_chpl call_tmp_chpl74 = NULL;
#line 101 "../lib/MNISTTester.chpl"
  _ref_locale_chpl i_x_chpl7 = NULL;
#line 101 "../lib/MNISTTester.chpl"
  Error_chpl error_chpl9 = NULL;
#line 101 "../lib/MNISTTester.chpl"
  _ref_locale_chpl call_tmp_chpl75 = NULL;
#line 101 "../lib/MNISTTester.chpl"
  _ref_locale_chpl i_x_chpl8 = NULL;
#line 101 "../lib/MNISTTester.chpl"
  Error_chpl error_chpl10 = NULL;
#line 101 "../lib/MNISTTester.chpl"
  _ref_string_chpl call_tmp_chpl76 = NULL;
#line 101 "../lib/MNISTTester.chpl"
  _ref_locale_chpl call_tmp_chpl77 = NULL;
#line 101 "../lib/MNISTTester.chpl"
  _ref_locale_chpl i_x_chpl9 = NULL;
#line 101 "../lib/MNISTTester.chpl"
  Error_chpl error_chpl11 = NULL;
#line 101 "../lib/MNISTTester.chpl"
  _ref_locale_chpl call_tmp_chpl78 = NULL;
#line 101 "../lib/MNISTTester.chpl"
  _ref_locale_chpl i_x_chpl10 = NULL;
#line 101 "../lib/MNISTTester.chpl"
  Error_chpl error_chpl12 = NULL;
#line 101 "../lib/MNISTTester.chpl"
  _ref_string_chpl call_tmp_chpl79 = NULL;
#line 101 "../lib/MNISTTester.chpl"
  _ref_locale_chpl call_tmp_chpl80 = NULL;
#line 101 "../lib/MNISTTester.chpl"
  _ref_locale_chpl i_x_chpl11 = NULL;
#line 101 "../lib/MNISTTester.chpl"
  Error_chpl error_chpl13 = NULL;
#line 101 "../lib/MNISTTester.chpl"
  _ref_locale_chpl call_tmp_chpl81 = NULL;
#line 101 "../lib/MNISTTester.chpl"
  _ref_locale_chpl i_x_chpl12 = NULL;
#line 101 "../lib/MNISTTester.chpl"
  Error_chpl error_chpl14 = NULL;
#line 101 "../lib/MNISTTester.chpl"
  _ref_string_chpl call_tmp_chpl82 = NULL;
#line 101 "../lib/MNISTTester.chpl"
  _ref_locale_chpl call_tmp_chpl83 = NULL;
#line 101 "../lib/MNISTTester.chpl"
  _ref_locale_chpl i_x_chpl13 = NULL;
#line 101 "../lib/MNISTTester.chpl"
  Error_chpl error_chpl15 = NULL;
#line 101 "../lib/MNISTTester.chpl"
  _ref_locale_chpl call_tmp_chpl84 = NULL;
#line 101 "../lib/MNISTTester.chpl"
  _ref_locale_chpl i_x_chpl14 = NULL;
#line 101 "../lib/MNISTTester.chpl"
  Error_chpl error_chpl16 = NULL;
#line 101 "../lib/MNISTTester.chpl"
  _ref_string_chpl call_tmp_chpl85 = NULL;
#line 101 "../lib/MNISTTester.chpl"
  _ref_locale_chpl call_tmp_chpl86 = NULL;
#line 101 "../lib/MNISTTester.chpl"
  _ref_locale_chpl i_x_chpl15 = NULL;
#line 101 "../lib/MNISTTester.chpl"
  Error_chpl error_chpl17 = NULL;
#line 101 "../lib/MNISTTester.chpl"
  _ref_locale_chpl call_tmp_chpl87 = NULL;
#line 101 "../lib/MNISTTester.chpl"
  _ref_locale_chpl i_x_chpl16 = NULL;
#line 101 "../lib/MNISTTester.chpl"
  Error_chpl error_chpl18 = NULL;
#line 101 "../lib/MNISTTester.chpl"
  _ref_string_chpl call_tmp_chpl88 = NULL;
#line 101 "../lib/MNISTTester.chpl"
  _ref_locale_chpl call_tmp_chpl89 = NULL;
#line 101 "../lib/MNISTTester.chpl"
  _ref_locale_chpl i_x_chpl17 = NULL;
#line 101 "../lib/MNISTTester.chpl"
  _ref_locale_chpl call_tmp_chpl90 = NULL;
#line 101 "../lib/MNISTTester.chpl"
  _ref_locale_chpl i_x_chpl18 = NULL;
#line 102 "../lib/MNISTTester.chpl"
  Error_chpl error_chpl19 = NULL;
#line 104 "../lib/MNISTTester.chpl"
  Error_chpl error_chpl20 = NULL;
#line 104 "../lib/MNISTTester.chpl"
  _ref_fileWriter_T_nothing_chpl i_this_chpl6 = NULL;
#line 104 "../lib/MNISTTester.chpl"
  string_chpl inlineImm_chpl8;
#line 104 "../lib/MNISTTester.chpl"
  _ref_string_chpl i__e0_args_chpl2 = NULL;
#line 104 "../lib/MNISTTester.chpl"
  locale_chpl ret_chpl10;
#line 104 "../lib/MNISTTester.chpl"
  _ref_locale_chpl call_tmp_chpl91 = NULL;
#line 104 "../lib/MNISTTester.chpl"
  locale_chpl ret_chpl11;
#line 104 "../lib/MNISTTester.chpl"
  chpl_bool call_tmp_chpl92;
#line 104 "../lib/MNISTTester.chpl"
  locale_chpl call_tmp_chpl93;
#line 104 "../lib/MNISTTester.chpl"
  chpl_localeID_t call_tmp_chpl94;
#line 104 "../lib/MNISTTester.chpl"
  int32_t call_tmp_chpl95;
#line 104 "../lib/MNISTTester.chpl"
  chpl_localeID_t call_tmp_chpl96;
#line 104 "../lib/MNISTTester.chpl"
  locale_chpl call_tmp_chpl97;
#line 104 "../lib/MNISTTester.chpl"
  locale_chpl ret_tmp_chpl32;
#line 104 "../lib/MNISTTester.chpl"
  _ref_locale_chpl i_x_chpl19 = NULL;
#line 104 "../lib/MNISTTester.chpl"
  locale_chpl origLocale_chpl2;
#line 104 "../lib/MNISTTester.chpl"
  _ref_locale_chpl call_tmp_chpl98 = NULL;
#line 104 "../lib/MNISTTester.chpl"
  Error_chpl error_chpl21 = NULL;
#line 104 "../lib/MNISTTester.chpl"
  _owned_SystemError_chpl call_tmp_chpl99;
#line 104 "../lib/MNISTTester.chpl"
  _owned_SystemError_chpl ret_tmp_chpl33;
#line 104 "../lib/MNISTTester.chpl"
  Error_chpl call_tmp_chpl100 = NULL;
#line 104 "../lib/MNISTTester.chpl"
  _owned_Error_chpl coerce_tmp_chpl16;
#line 104 "../lib/MNISTTester.chpl"
  _owned_Error_chpl ret_tmp_chpl34;
#line 104 "../lib/MNISTTester.chpl"
  _ref__owned_SystemError_chpl i_x_chpl20 = NULL;
#line 104 "../lib/MNISTTester.chpl"
  _ref__owned_Error_chpl2 i__retArg_chpl2 = NULL;
#line 104 "../lib/MNISTTester.chpl"
  _owned_SystemError_chpl _formal_tmp_x_chpl2;
#line 104 "../lib/MNISTTester.chpl"
  _owned_Error_chpl ret_chpl12;
#line 104 "../lib/MNISTTester.chpl"
  _ref__owned_SystemError_chpl i_this_chpl7 = NULL;
#line 104 "../lib/MNISTTester.chpl"
  Error_chpl call_tmp_chpl101 = NULL;
#line 104 "../lib/MNISTTester.chpl"
  _ref__owned_SystemError_chpl i_this_chpl8 = NULL;
#line 104 "../lib/MNISTTester.chpl"
  _owned_Error_chpl call_tmp_chpl102;
#line 104 "../lib/MNISTTester.chpl"
  _owned_Error_chpl initTemp_chpl3;
#line 104 "../lib/MNISTTester.chpl"
  _owned_Error_chpl _formal_tmp_in_err_chpl2;
#line 104 "../lib/MNISTTester.chpl"
  _ref_locale_chpl call_tmp_chpl103 = NULL;
#line 104 "../lib/MNISTTester.chpl"
  syserr call_tmp_chpl104;
#line 104 "../lib/MNISTTester.chpl"
  int32_t call_tmp_chpl105;
#line 104 "../lib/MNISTTester.chpl"
  Error_chpl error_chpl22 = NULL;
#line 104 "../lib/MNISTTester.chpl"
  _ref_locale_chpl i_x_chpl21 = NULL;
#line 104 "../lib/MNISTTester.chpl"
  Error_chpl error_chpl23 = NULL;
#line 104 "../lib/MNISTTester.chpl"
  _ref_locale_chpl call_tmp_chpl106 = NULL;
#line 104 "../lib/MNISTTester.chpl"
  _ref_locale_chpl i_x_chpl22 = NULL;
#line 104 "../lib/MNISTTester.chpl"
  _ref_locale_chpl call_tmp_chpl107 = NULL;
#line 104 "../lib/MNISTTester.chpl"
  _ref_locale_chpl i_x_chpl23 = NULL;
#line 105 "../lib/MNISTTester.chpl"
  Error_chpl error_chpl24 = NULL;
#line 112 "../lib/MNISTTester.chpl"
  SumReduceScanOp__real64_chpl globalRP_loss_chpl = NULL;
#line 112 "../lib/MNISTTester.chpl"
  SumReduceScanOp__real64_chpl new_temp_chpl2 = NULL;
#line 112 "../lib/MNISTTester.chpl"
  chpl_LocalSpinlock default_arg_l_chpl;
#line 112 "../lib/MNISTTester.chpl"
  chpl_LocalSpinlock ret_chpl13;
#line 112 "../lib/MNISTTester.chpl"
  chpl_LocalSpinlock l_chpl;
#line 112 "../lib/MNISTTester.chpl"
  SumReduceScanOp__real64_chpl coerce_tmp_chpl17 = NULL;
#line 112 "../lib/MNISTTester.chpl"
  _ref__real64_chpl call_tmp_chpl108 = NULL;
#line 112 "../lib/MNISTTester.chpl"
  _real64 globalAS_loss_chpl;
#line 112 "../lib/MNISTTester.chpl"
  SumReduceScanOp_int64_t_chpl globalRP_numCorrect_chpl = NULL;
#line 112 "../lib/MNISTTester.chpl"
  SumReduceScanOp_int64_t_chpl new_temp_chpl3 = NULL;
#line 112 "../lib/MNISTTester.chpl"
  chpl_LocalSpinlock default_arg_l_chpl2;
#line 112 "../lib/MNISTTester.chpl"
  chpl_LocalSpinlock ret_chpl14;
#line 112 "../lib/MNISTTester.chpl"
  chpl_LocalSpinlock l_chpl2;
#line 112 "../lib/MNISTTester.chpl"
  SumReduceScanOp_int64_t_chpl coerce_tmp_chpl18 = NULL;
#line 112 "../lib/MNISTTester.chpl"
  _ref_int64_t_chpl call_tmp_chpl109 = NULL;
#line 112 "../lib/MNISTTester.chpl"
  int64_t globalAS_numCorrect_chpl;
#line 112 "../lib/MNISTTester.chpl"
  DefaultRectangularArr_1_int64_t_one__tuple_2_Tensor_3__real64_int64_t_int64_t_chpl coerce_tmp_chpl19 = NULL;
#line 112 "../lib/MNISTTester.chpl"
  int64_t tasksPerLocale_chpl2;
#line 112 "../lib/MNISTTester.chpl"
  chpl_bool ignoreRunning_chpl2;
#line 112 "../lib/MNISTTester.chpl"
  int64_t minIndicesPerTask_chpl2;
#line 112 "../lib/MNISTTester.chpl"
  Error_chpl error_chpl25 = NULL;
#line 112 "../lib/MNISTTester.chpl"
  DefaultRectangularDom_1_int64_t_one_chpl coerce_tmp_chpl20 = NULL;
#line 112 "../lib/MNISTTester.chpl"
  _tuple_1_star_int64_t_chpl ret_tmp_chpl35;
#line 112 "../lib/MNISTTester.chpl"
  Error_chpl error_chpl26 = NULL;
#line 112 "../lib/MNISTTester.chpl"
  int64_t tmp_chpl5;
#line 112 "../lib/MNISTTester.chpl"
  locale_chpl call_tmp_chpl110;
#line 112 "../lib/MNISTTester.chpl"
  chpl_localeID_t call_tmp_chpl111;
#line 112 "../lib/MNISTTester.chpl"
  int32_t call_tmp_chpl112;
#line 112 "../lib/MNISTTester.chpl"
  chpl_localeID_t call_tmp_chpl113;
#line 112 "../lib/MNISTTester.chpl"
  locale_chpl call_tmp_chpl114;
#line 112 "../lib/MNISTTester.chpl"
  locale_chpl ret_tmp_chpl36;
#line 112 "../lib/MNISTTester.chpl"
  int64_t ret_chpl15;
#line 112 "../lib/MNISTTester.chpl"
  BaseLocale_chpl coerce_tmp_chpl21 = NULL;
#line 112 "../lib/MNISTTester.chpl"
  BaseLocale_chpl coerce_tmp_chpl22 = NULL;
#line 112 "../lib/MNISTTester.chpl"
  _ref_locale_chpl i_x_chpl24 = NULL;
#line 112 "../lib/MNISTTester.chpl"
  int64_t tmp_x0_chpl4;
#line 112 "../lib/MNISTTester.chpl"
  int64_t tmp_x1_chpl4;
#line 112 "../lib/MNISTTester.chpl"
  _ref__tuple_1_star_range_int64_t_both_one_chpl call_tmp_chpl115 = NULL;
#line 112 "../lib/MNISTTester.chpl"
  _tuple_2_star_int64_t_chpl ret_tmp_chpl37;
#line 112 "../lib/MNISTTester.chpl"
  int64_t numChunks_chpl2;
#line 112 "../lib/MNISTTester.chpl"
  int64_t parDim_chpl2;
#line 112 "../lib/MNISTTester.chpl"
  range_int64_t_both_one_chpl tmpIter_chpl2;
#line 112 "../lib/MNISTTester.chpl"
  range_int64_t_low_one_chpl call_tmp_chpl116;
#line 112 "../lib/MNISTTester.chpl"
  range_int64_t_low_one_chpl ret_tmp_chpl38;
#line 112 "../lib/MNISTTester.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl39;
#line 112 "../lib/MNISTTester.chpl"
  chpl___EndCount_AtomicT_int64_t_int64_t _coforallCount_chpl2 = NULL;
#line 112 "../lib/MNISTTester.chpl"
  chpl___EndCount_AtomicT_int64_t_int64_t new_temp_chpl4 = NULL;
#line 112 "../lib/MNISTTester.chpl"
  int64_t numTasks_chpl2;
#line 112 "../lib/MNISTTester.chpl"
  range_int64_t_both_one_chpl _ic__F0_this_chpl2;
#line 112 "../lib/MNISTTester.chpl"
  int64_t i_chpl4;
#line 112 "../lib/MNISTTester.chpl"
  range_int64_t_both_one_chpl this_chpl34;
#line 112 "../lib/MNISTTester.chpl"
  range_int64_t_both_one_chpl this_chpl35;
#line 112 "../lib/MNISTTester.chpl"
  int64_t end_chpl2;
#line 112 "../lib/MNISTTester.chpl"
  _class_localscoforall_fn_chpl115 _args_forcoforall_fn_chpl2 = NULL;
#line 112 "../lib/MNISTTester.chpl"
  _ref_Network__tuple_4_Conv_Conv_MaxPool_SoftMax_chpl tmp_chpl6 = NULL;
#line 112 "../lib/MNISTTester.chpl"
  raw_c_void_ptr _args_vforcoforall_fn_chpl2;
#line 112 "../lib/MNISTTester.chpl"
  Error_chpl error_chpl27 = NULL;
#line 112 "../lib/MNISTTester.chpl"
  SumReduceScanOp_int64_t_chpl coerce_tmp_chpl23 = NULL;
#line 112 "../lib/MNISTTester.chpl"
  _ref_int64_t_chpl call_tmp_chpl117 = NULL;
#line 112 "../lib/MNISTTester.chpl"
  SumReduceScanOp_int64_t_chpl coerce_tmp_chpl24 = NULL;
#line 112 "../lib/MNISTTester.chpl"
  int64_t ret_chpl16;
#line 112 "../lib/MNISTTester.chpl"
  SumReduceScanOp__real64_chpl coerce_tmp_chpl25 = NULL;
#line 112 "../lib/MNISTTester.chpl"
  _ref__real64_chpl call_tmp_chpl118 = NULL;
#line 112 "../lib/MNISTTester.chpl"
  SumReduceScanOp__real64_chpl coerce_tmp_chpl26 = NULL;
#line 112 "../lib/MNISTTester.chpl"
  _real64 ret_chpl17;
#line 118 "../lib/MNISTTester.chpl"
  int64_t call_tmp_chpl119;
#line 118 "../lib/MNISTTester.chpl"
  int64_t call_tmp_chpl120;
#line 118 "../lib/MNISTTester.chpl"
  _real64 call_tmp_chpl121;
#line 118 "../lib/MNISTTester.chpl"
  int64_t call_tmp_chpl122;
#line 120 "../lib/MNISTTester.chpl"
  Error_chpl error_chpl28 = NULL;
#line 60 "../lib/MNISTTester.chpl"
  _array_DefaultRectangularArr_1_int64_t_one__array_DefaultRectangularArr_1_int64_t_one__real64_int64_t_int64_t_chpl _field_destructor_tmp__chpl5;
#line 60 "../lib/MNISTTester.chpl"
  _array_DefaultRectangularArr_1_int64_t_one_int64_t_int64_t_chpl _field_destructor_tmp__chpl6;
#line 60 "../lib/MNISTTester.chpl"
  chpl__class_localscoforall_fn_chpl19_object chpl_macro_tmp_124;
#line 60 "../lib/MNISTTester.chpl"
  chpl__class_localscoforall_fn_chpl115_object chpl_macro_tmp_125;
#line 101 "../lib/MNISTTester.chpl"
  local_nilLocale_chpl = nilLocale_chpl;
#line 101 "../lib/MNISTTester.chpl"
  local_stdout_chpl = stdout_chpl;
#line 71 "../lib/MNISTTester.chpl"
  local_dataParMinGranularity_chpl = dataParMinGranularity_chpl;
#line 71 "../lib/MNISTTester.chpl"
  local_dataParIgnoreRunningTasks_chpl = dataParIgnoreRunningTasks_chpl;
#line 71 "../lib/MNISTTester.chpl"
  local_dataParTasksPerLocale_chpl = dataParTasksPerLocale_chpl;
#line 68 "../lib/MNISTTester.chpl"
  call_tmp_chpl6 = (numTrainImages_chpl2 + numTestImages_chpl2);
#line 68 "../lib/MNISTTester.chpl"
  numImages_chpl = call_tmp_chpl6;
#line 70 "../lib/MNISTTester.chpl"
  loadImages_chpl(call_tmp_chpl6, &_str_literal_1742_chpl, &ret_tmp_chpl);
#line 70 "../lib/MNISTTester.chpl"
  call_tmp_chpl7 = ret_tmp_chpl;
#line 70 "../lib/MNISTTester.chpl"
  imageRawData_chpl = call_tmp_chpl7;
#line 71 "../lib/MNISTTester.chpl"
  i_lhs_chpl = &imageRawData_chpl;
#line 71 "../lib/MNISTTester.chpl"
  _ir_F0_a_chpl = imageRawData_chpl;
#line 71 "../lib/MNISTTester.chpl"
  (&call_tmp_chpl8)->_shape_ = _ir__shape__chpl;
#line 71 "../lib/MNISTTester.chpl"
  (&call_tmp_chpl8)->F0_a = _ir_F0_a_chpl;
#line 71 "../lib/MNISTTester.chpl"
  (&call_tmp_chpl8)->F1_b = REAL64(0x1p-1);
#line 71 "../lib/MNISTTester.chpl"
  _dom_chpl17(i_lhs_chpl, &ret_tmp_chpl2, INT64(71), INT32(66));
#line 71 "../lib/MNISTTester.chpl"
  call_tmp_chpl9 = ret_tmp_chpl2;
#line 71 "../lib/MNISTTester.chpl"
  ret_chpl = (&call_tmp_chpl9)->_instance;
#line 71 "../lib/MNISTTester.chpl"
  chpl__autoDestroy11(&call_tmp_chpl9, INT64(71), INT32(66));
#line 71 "../lib/MNISTTester.chpl"
  (&call_tmp_chpl8)->_shape_ = ret_chpl;
#line 71 "../lib/MNISTTester.chpl"
  coerce_tmp_chpl3 = (&imageRawData_chpl)->_instance;
#line 71 "../lib/MNISTTester.chpl"
  tasksPerLocale_chpl = local_dataParTasksPerLocale_chpl;
#line 71 "../lib/MNISTTester.chpl"
  ignoreRunning_chpl = local_dataParIgnoreRunningTasks_chpl;
#line 71 "../lib/MNISTTester.chpl"
  minIndicesPerTask_chpl = local_dataParMinGranularity_chpl;
#line 71 "../lib/MNISTTester.chpl"
  error_chpl = NULL;
#line 71 "../lib/MNISTTester.chpl"
  chpl_check_nil(coerce_tmp_chpl3, INT64(71), INT32(66));
#line 71 "../lib/MNISTTester.chpl"
  coerce_tmp_chpl4 = (coerce_tmp_chpl3)->dom;
#line 71 "../lib/MNISTTester.chpl"
  createTuple_chpl(INT64(0), &ret_tmp_chpl3);
#line 71 "../lib/MNISTTester.chpl"
  offset_x0_chpl = *(ret_tmp_chpl3 + INT64(0));
#line 71 "../lib/MNISTTester.chpl"
  error_chpl2 = NULL;
#line 71 "../lib/MNISTTester.chpl"
  call_tmp_chpl12 = get_chpl_nodeID();
#line 71 "../lib/MNISTTester.chpl"
  call_tmp_chpl13 = chpl_rt_buildLocaleID(call_tmp_chpl12, c_sublocid_any);
#line 71 "../lib/MNISTTester.chpl"
  call_tmp_chpl11 = call_tmp_chpl13;
#line 71 "../lib/MNISTTester.chpl"
  chpl_localeID_to_locale(&call_tmp_chpl11, &ret_tmp_chpl4, INT64(71), INT32(66));
#line 71 "../lib/MNISTTester.chpl"
  call_tmp_chpl14 = ret_tmp_chpl4;
#line 71 "../lib/MNISTTester.chpl"
  call_tmp_chpl10 = call_tmp_chpl14;
#line 71 "../lib/MNISTTester.chpl"
  coerce_tmp_chpl5 = (&call_tmp_chpl10)->_instance;
#line 71 "../lib/MNISTTester.chpl"
  if (((RootClass_chpl)(coerce_tmp_chpl5)) == nil) /* ZLINE: 71 ../lib/MNISTTester.chpl */
#line 71 "../lib/MNISTTester.chpl"
  {
#line 71 "../lib/MNISTTester.chpl"
    nilCheckHalt_chpl(&_str_literal_52_chpl, INT64(71), INT32(66));
#line 71 "../lib/MNISTTester.chpl"
  }
#line 71 "../lib/MNISTTester.chpl"
  chpl_check_nil(coerce_tmp_chpl5, INT64(71), INT32(66));
#line 71 "../lib/MNISTTester.chpl"
  ((int64_t(*)(BaseLocale_chpl,
#line 71 "../lib/MNISTTester.chpl"
     int64_t,
#line 71 "../lib/MNISTTester.chpl"
     int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(coerce_tmp_chpl5))->chpl__cid) + INT64(2))])(coerce_tmp_chpl5, INT64(71), INT32(66));
#line 71 "../lib/MNISTTester.chpl"
  i_x_chpl = &call_tmp_chpl10;
#line 71 "../lib/MNISTTester.chpl"
  deinit_chpl62(i_x_chpl);
#line 71 "../lib/MNISTTester.chpl"
  if (tasksPerLocale_chpl == INT64(0)) /* ZLINE: 71 ../lib/MNISTTester.chpl */
#line 71 "../lib/MNISTTester.chpl"
  {
#line 71 "../lib/MNISTTester.chpl"
    call_tmp_chpl17 = get_chpl_nodeID();
#line 71 "../lib/MNISTTester.chpl"
    call_tmp_chpl18 = chpl_rt_buildLocaleID(call_tmp_chpl17, c_sublocid_any);
#line 71 "../lib/MNISTTester.chpl"
    call_tmp_chpl16 = call_tmp_chpl18;
#line 71 "../lib/MNISTTester.chpl"
    chpl_localeID_to_locale(&call_tmp_chpl16, &ret_tmp_chpl5, INT64(71), INT32(66));
#line 71 "../lib/MNISTTester.chpl"
    call_tmp_chpl19 = ret_tmp_chpl5;
#line 71 "../lib/MNISTTester.chpl"
    call_tmp_chpl15 = call_tmp_chpl19;
#line 71 "../lib/MNISTTester.chpl"
    coerce_tmp_chpl6 = (&call_tmp_chpl15)->_instance;
#line 71 "../lib/MNISTTester.chpl"
    if (((RootClass_chpl)(coerce_tmp_chpl6)) == nil) /* ZLINE: 71 ../lib/MNISTTester.chpl */
#line 71 "../lib/MNISTTester.chpl"
    {
#line 71 "../lib/MNISTTester.chpl"
      nilCheckHalt_chpl(&_str_literal_52_chpl, INT64(71), INT32(66));
#line 71 "../lib/MNISTTester.chpl"
    }
#line 71 "../lib/MNISTTester.chpl"
    coerce_tmp_chpl7 = coerce_tmp_chpl6;
#line 71 "../lib/MNISTTester.chpl"
    chpl_check_nil(coerce_tmp_chpl7, INT64(71), INT32(66));
#line 71 "../lib/MNISTTester.chpl"
    ret_chpl2 = (coerce_tmp_chpl7)->maxTaskPar;
#line 71 "../lib/MNISTTester.chpl"
    tmp_chpl = ret_chpl2;
#line 71 "../lib/MNISTTester.chpl"
    i_x_chpl2 = &call_tmp_chpl15;
#line 71 "../lib/MNISTTester.chpl"
    deinit_chpl62(i_x_chpl2);
#line 71 "../lib/MNISTTester.chpl"
  } else /* ZLINE: 71 ../lib/MNISTTester.chpl */
#line 71 "../lib/MNISTTester.chpl"
  {
#line 71 "../lib/MNISTTester.chpl"
    tmp_chpl = tasksPerLocale_chpl;
#line 71 "../lib/MNISTTester.chpl"
  }
#line 71 "../lib/MNISTTester.chpl"
  if (chpl_task_getSerial()) /* ZLINE: 71 ../lib/MNISTTester.chpl */
#line 71 "../lib/MNISTTester.chpl"
  {
#line 71 "../lib/MNISTTester.chpl"
    tmp_x0_chpl = INT64(1);
#line 71 "../lib/MNISTTester.chpl"
    tmp_x1_chpl = INT64(0);
#line 71 "../lib/MNISTTester.chpl"
  } else /* ZLINE: 71 ../lib/MNISTTester.chpl */
#line 71 "../lib/MNISTTester.chpl"
  {
#line 71 "../lib/MNISTTester.chpl"
    chpl_check_nil(coerce_tmp_chpl4, INT64(71), INT32(66));
#line 71 "../lib/MNISTTester.chpl"
    call_tmp_chpl20 = &((coerce_tmp_chpl4)->ranges);
#line 71 "../lib/MNISTTester.chpl"
    _computeChunkStuff_chpl(tmp_chpl, ignoreRunning_chpl, minIndicesPerTask_chpl, call_tmp_chpl20, &ret_tmp_chpl6, INT64(71), INT32(66));
#line 71 "../lib/MNISTTester.chpl"
    tmp_x0_chpl = *(ret_tmp_chpl6 + INT64(0));
#line 71 "../lib/MNISTTester.chpl"
    tmp_x1_chpl = *(ret_tmp_chpl6 + INT64(1));
#line 71 "../lib/MNISTTester.chpl"
  }
#line 71 "../lib/MNISTTester.chpl"
  numChunks_chpl = tmp_x0_chpl;
#line 71 "../lib/MNISTTester.chpl"
  parDim_chpl = tmp_x1_chpl;
#line 71 "../lib/MNISTTester.chpl"
  init_chpl113(&elt_x0_chpl);
#line 71 "../lib/MNISTTester.chpl"
  tup_x0_chpl = elt_x0_chpl;
#line 71 "../lib/MNISTTester.chpl"
  *(locBlock_chpl + INT64(0)) = tup_x0_chpl;
#line 71 "../lib/MNISTTester.chpl"
  chpl_build_low_bounded_range2(offset_x0_chpl, &ret_tmp_chpl7);
#line 71 "../lib/MNISTTester.chpl"
  call_tmp_chpl21 = ret_tmp_chpl7;
#line 71 "../lib/MNISTTester.chpl"
  chpl_check_nil(coerce_tmp_chpl4, INT64(71), INT32(66));
#line 71 "../lib/MNISTTester.chpl"
  call_tmp_chpl22 = &((coerce_tmp_chpl4)->ranges);
#line 71 "../lib/MNISTTester.chpl"
  coerce_tmp_chpl8 = *(*(call_tmp_chpl22) + INT64(0));
#line 71 "../lib/MNISTTester.chpl"
  call_tmp_chpl23 = sizeAs_chpl37(coerce_tmp_chpl8, INT64(71), INT32(66));
#line 71 "../lib/MNISTTester.chpl"
  chpl___POUND_3(call_tmp_chpl21, call_tmp_chpl23, &ret_tmp_chpl8, INT64(71), INT32(66));
#line 71 "../lib/MNISTTester.chpl"
  call_tmp_chpl24 = ret_tmp_chpl8;
#line 71 "../lib/MNISTTester.chpl"
  *(locBlock_chpl + INT64(0)) = call_tmp_chpl24;
#line 71 "../lib/MNISTTester.chpl"
  chpl_build_low_bounded_range2(INT64(0), &ret_tmp_chpl9);
#line 71 "../lib/MNISTTester.chpl"
  call_tmp_chpl25 = ret_tmp_chpl9;
#line 71 "../lib/MNISTTester.chpl"
  chpl___POUND_3(call_tmp_chpl25, tmp_x0_chpl, &ret_tmp_chpl10, INT64(71), INT32(66));
#line 71 "../lib/MNISTTester.chpl"
  tmpIter_chpl = ret_tmp_chpl10;
#line 71 "../lib/MNISTTester.chpl"
  new_temp_chpl = _new_chpl17(INT64(71), INT32(66));
#line 71 "../lib/MNISTTester.chpl"
  _coforallCount_chpl = new_temp_chpl;
#line 71 "../lib/MNISTTester.chpl"
  numTasks_chpl = chpl_boundedCoforallSize(tmpIter_chpl, INT64(71), INT32(66));
#line 71 "../lib/MNISTTester.chpl"
  _upEndCount_chpl(_coforallCount_chpl, numTasks_chpl, INT64(71), INT32(66));
#line 71 "../lib/MNISTTester.chpl"
  chpl_resetTaskSpawn(numTasks_chpl, INT64(71), INT32(66));
#line 71 "../lib/MNISTTester.chpl"
  _ic__F0_this_chpl = tmpIter_chpl;
#line 71 "../lib/MNISTTester.chpl"
  checkIfIterWillOverflow_chpl(tmpIter_chpl, UINT8(true), INT64(71), INT32(66));
#line 71 "../lib/MNISTTester.chpl"
  i_chpl = INT64(0);
#line 71 "../lib/MNISTTester.chpl"
  this_chpl31 = _ic__F0_this_chpl;
#line 71 "../lib/MNISTTester.chpl"
  this_chpl32 = _ic__F0_this_chpl;
#line 71 "../lib/MNISTTester.chpl"
  end_chpl = (&this_chpl32)->_high;
#line 71 "../lib/MNISTTester.chpl"
  for (i_chpl = (&this_chpl31)->_low; ((i_chpl <= end_chpl)); i_chpl += INT64(1)) {
#line 71 "../lib/MNISTTester.chpl"
    _args_forcoforall_fn_chpl = ((_class_localscoforall_fn_chpl19)(&chpl_macro_tmp_124));
#line 71 "../lib/MNISTTester.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl, INT64(71), INT32(66));
#line 71 "../lib/MNISTTester.chpl"
    (_args_forcoforall_fn_chpl)->_0_numChunks = numChunks_chpl;
#line 71 "../lib/MNISTTester.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl, INT64(71), INT32(66));
#line 71 "../lib/MNISTTester.chpl"
    (_args_forcoforall_fn_chpl)->_1_parDim = parDim_chpl;
#line 71 "../lib/MNISTTester.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl, INT64(71), INT32(66));
#line 71 "../lib/MNISTTester.chpl"
    *((_args_forcoforall_fn_chpl)->_2_coerce_tmp + INT64(0)) = *(locBlock_chpl + INT64(0));
#line 71 "../lib/MNISTTester.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl, INT64(71), INT32(66));
#line 71 "../lib/MNISTTester.chpl"
    (_args_forcoforall_fn_chpl)->_3__coforallCount = _coforallCount_chpl;
#line 71 "../lib/MNISTTester.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl, INT64(71), INT32(66));
#line 71 "../lib/MNISTTester.chpl"
    (_args_forcoforall_fn_chpl)->_4_chunk = i_chpl;
#line 71 "../lib/MNISTTester.chpl"
    tmp_chpl2 = &call_tmp_chpl8;
#line 71 "../lib/MNISTTester.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl, INT64(71), INT32(66));
#line 71 "../lib/MNISTTester.chpl"
    (_args_forcoforall_fn_chpl)->_6_b = tmp_chpl2;
#line 71 "../lib/MNISTTester.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl, INT64(71), INT32(66));
#line 71 "../lib/MNISTTester.chpl"
    (_args_forcoforall_fn_chpl)->_7_a = *(i_lhs_chpl);
#line 71 "../lib/MNISTTester.chpl"
    _args_vforcoforall_fn_chpl = ((void*)(_args_forcoforall_fn_chpl));
#line 71 "../lib/MNISTTester.chpl"
    _args_sizecoforall_fn_chpl = sizeof(chpl__class_localscoforall_fn_chpl19_object);
#line 71 "../lib/MNISTTester.chpl"
    (_args_forcoforall_fn_chpl)->_ln = INT64(71);
#line 71 "../lib/MNISTTester.chpl"
    (_args_forcoforall_fn_chpl)->_fn = INT32(66);
#line 71 "../lib/MNISTTester.chpl"
    /*** wrapcoforall_fn_chpl30 ***/ chpl_taskAddCoStmt(chpl_task_getRequestedSubloc(), INT64(29), ((chpl_task_bundle_p)(_args_vforcoforall_fn_chpl)), _args_sizecoforall_fn_chpl, INT64(2763), INT32(30));
#line 71 "../lib/MNISTTester.chpl"
  }
#line 71 "../lib/MNISTTester.chpl"
  error_chpl3 = NULL;
#line 71 "../lib/MNISTTester.chpl"
  _waitEndCount_chpl2(_coforallCount_chpl, numTasks_chpl, &error_chpl3, INT64(71), INT32(66));
#line 71 "../lib/MNISTTester.chpl"
  if (error_chpl3 != nil) /* ZLINE: 71 ../lib/MNISTTester.chpl */
#line 71 "../lib/MNISTTester.chpl"
  {
#line 71 "../lib/MNISTTester.chpl"
    chpl__delete72(_coforallCount_chpl, INT64(71), INT32(66));
#line 71 "../lib/MNISTTester.chpl"
    error_chpl2 = error_chpl3;
#line 71 "../lib/MNISTTester.chpl"
    goto forall_IBB_break_label_chpl;
#line 71 "../lib/MNISTTester.chpl"
  }
#line 71 "../lib/MNISTTester.chpl"
  chpl__delete72(_coforallCount_chpl, INT64(71), INT32(66));
#line 71 "../lib/MNISTTester.chpl"
  chpl_after_forall_fence();
#line 71 "../lib/MNISTTester.chpl"
  forall_IBB_break_label_chpl:;
#line 71 "../lib/MNISTTester.chpl"
  if (error_chpl2 != nil) /* ZLINE: 71 ../lib/MNISTTester.chpl */
#line 71 "../lib/MNISTTester.chpl"
  {
#line 71 "../lib/MNISTTester.chpl"
    error_chpl = error_chpl2;
#line 71 "../lib/MNISTTester.chpl"
    goto forall_IBB_break_label_chpl2;
#line 71 "../lib/MNISTTester.chpl"
  }
#line 71 "../lib/MNISTTester.chpl"
  chpl_after_forall_fence();
#line 71 "../lib/MNISTTester.chpl"
  forall_IBB_break_label_chpl2:;
#line 71 "../lib/MNISTTester.chpl"
  if (error_chpl != nil) /* ZLINE: 71 ../lib/MNISTTester.chpl */
#line 71 "../lib/MNISTTester.chpl"
  {
#line 71 "../lib/MNISTTester.chpl"
    chpl_propagate_error(error_chpl, INT64(71), INT32(66));
#line 71 "../lib/MNISTTester.chpl"
  }
#line 71 "../lib/MNISTTester.chpl"
  chpl_after_forall_fence();
#line 72 "../lib/MNISTTester.chpl"
  loadLabels_chpl(numImages_chpl, &_str_literal_1743_chpl, &ret_tmp_chpl11);
#line 72 "../lib/MNISTTester.chpl"
  call_tmp_chpl26 = ret_tmp_chpl11;
#line 72 "../lib/MNISTTester.chpl"
  tmp_x0_chpl2 = (&call_tmp_chpl26)->x0;
#line 72 "../lib/MNISTTester.chpl"
  tmp_x1_chpl2 = (&call_tmp_chpl26)->x1;
#line 72 "../lib/MNISTTester.chpl"
  call_tmp_chpl27 = &tmp_x0_chpl2;
#line 72 "../lib/MNISTTester.chpl"
  labels_chpl = chpl__initCopy13(call_tmp_chpl27, UINT8(false), INT64(72), INT32(66));
#line 72 "../lib/MNISTTester.chpl"
  call_tmp_chpl28 = &tmp_x1_chpl2;
#line 72 "../lib/MNISTTester.chpl"
  labelVectors_chpl = chpl__initCopy12(call_tmp_chpl28, UINT8(false), INT64(72), INT32(66));
#line 75 "../lib/MNISTTester.chpl"
  chpl__forallexpr13(&imageRawData_chpl, &ret_tmp_chpl12);
#line 75 "../lib/MNISTTester.chpl"
  call_tmp_chpl29 = ret_tmp_chpl12;
#line 75 "../lib/MNISTTester.chpl"
  i_arg_chpl = &imageRawData_chpl;
#line 75 "../lib/MNISTTester.chpl"
  _dom_chpl17(i_arg_chpl, &ret_tmp_chpl13, INT64(75), INT32(66));
#line 75 "../lib/MNISTTester.chpl"
  call_tmp_chpl30 = ret_tmp_chpl13;
#line 75 "../lib/MNISTTester.chpl"
  ret_chpl3 = (&call_tmp_chpl30)->_instance;
#line 75 "../lib/MNISTTester.chpl"
  chpl__autoDestroy11(&call_tmp_chpl30, INT64(75), INT32(66));
#line 75 "../lib/MNISTTester.chpl"
  (&call_tmp_chpl29)->_shape_ = ret_chpl3;
#line 75 "../lib/MNISTTester.chpl"
  images_chpl = chpl__initCopy15(&call_tmp_chpl29, UINT8(false), INT64(75), INT32(66));
#line 76 "../lib/MNISTTester.chpl"
  (&ret_tmp_chpl14)->x0 = images_chpl;
#line 76 "../lib/MNISTTester.chpl"
  (&ret_tmp_chpl14)->x1 = labels_chpl;
#line 76 "../lib/MNISTTester.chpl"
  call_tmp_chpl31 = ret_tmp_chpl14;
#line 76 "../lib/MNISTTester.chpl"
  chpl__forexpr14(&call_tmp_chpl31, &ret_tmp_chpl15);
#line 76 "../lib/MNISTTester.chpl"
  call_tmp_chpl32 = ret_tmp_chpl15;
#line 76 "../lib/MNISTTester.chpl"
  i_arg_chpl2 = &images_chpl;
#line 76 "../lib/MNISTTester.chpl"
  _dom_chpl6(i_arg_chpl2, &ret_tmp_chpl16, INT64(76), INT32(66));
#line 76 "../lib/MNISTTester.chpl"
  call_tmp_chpl33 = ret_tmp_chpl16;
#line 76 "../lib/MNISTTester.chpl"
  ret_chpl4 = (&call_tmp_chpl33)->_instance;
#line 76 "../lib/MNISTTester.chpl"
  chpl__autoDestroy11(&call_tmp_chpl33, INT64(76), INT32(66));
#line 76 "../lib/MNISTTester.chpl"
  (&call_tmp_chpl32)->_shape_ = ret_chpl4;
#line 76 "../lib/MNISTTester.chpl"
  labeledImages_chpl = chpl__initCopy14(&call_tmp_chpl32, UINT8(false), INT64(76), INT32(66));
#line 78 "../lib/MNISTTester.chpl"
  shuffle_chpl3(&labeledImages_chpl);
#line 80 "../lib/MNISTTester.chpl"
  chpl_build_low_bounded_range2(INT64(0), &ret_tmp_chpl17);
#line 80 "../lib/MNISTTester.chpl"
  call_tmp_chpl34 = ret_tmp_chpl17;
#line 80 "../lib/MNISTTester.chpl"
  chpl___POUND_3(call_tmp_chpl34, numTrainImages_chpl2, &ret_tmp_chpl18, INT64(80), INT32(66));
#line 80 "../lib/MNISTTester.chpl"
  call_tmp_chpl35 = ret_tmp_chpl18;
#line 80 "../lib/MNISTTester.chpl"
  this_chpl(&labeledImages_chpl, call_tmp_chpl35, &ret_tmp_chpl19, INT64(80), INT32(66));
#line 80 "../lib/MNISTTester.chpl"
  call_tmp_chpl36 = ret_tmp_chpl19;
#line 80 "../lib/MNISTTester.chpl"
  trainingData_chpl = chpl__initCopy10(&call_tmp_chpl36, UINT8(false), INT64(80), INT32(66));
#line 80 "../lib/MNISTTester.chpl"
  chpl__autoDestroy34(&call_tmp_chpl36, INT64(80), INT32(66));
#line 81 "../lib/MNISTTester.chpl"
  chpl_build_low_bounded_range2(numTrainImages_chpl2, &ret_tmp_chpl20);
#line 81 "../lib/MNISTTester.chpl"
  call_tmp_chpl37 = ret_tmp_chpl20;
#line 81 "../lib/MNISTTester.chpl"
  chpl___POUND_3(call_tmp_chpl37, numTestImages_chpl2, &ret_tmp_chpl21, INT64(81), INT32(66));
#line 81 "../lib/MNISTTester.chpl"
  call_tmp_chpl38 = ret_tmp_chpl21;
#line 81 "../lib/MNISTTester.chpl"
  this_chpl(&labeledImages_chpl, call_tmp_chpl38, &ret_tmp_chpl22, INT64(81), INT32(66));
#line 81 "../lib/MNISTTester.chpl"
  call_tmp_chpl39 = ret_tmp_chpl22;
#line 81 "../lib/MNISTTester.chpl"
  testingData_chpl = chpl__initCopy10(&call_tmp_chpl39, UINT8(false), INT64(81), INT32(66));
#line 81 "../lib/MNISTTester.chpl"
  chpl__autoDestroy34(&call_tmp_chpl39, INT64(81), INT32(66));
#line 83 "../lib/MNISTTester.chpl"
  i_this_chpl = &trainingData_chpl;
#line 83 "../lib/MNISTTester.chpl"
  *(coerce_tmp_chpl9 + INT64(0)) = INT64(0);
#line 83 "../lib/MNISTTester.chpl"
  i_i_chpl = &coerce_tmp_chpl9;
#line 83 "../lib/MNISTTester.chpl"
  ret_chpl5 = (&trainingData_chpl)->_instance;
#line 83 "../lib/MNISTTester.chpl"
  checkAccess_chpl4(i_this_chpl, i_i_chpl, ret_chpl5, INT64(83), INT32(66));
#line 83 "../lib/MNISTTester.chpl"
  coerce_tmp_chpl10 = ret_chpl5;
#line 83 "../lib/MNISTTester.chpl"
  *(_formal_tmp_in_ind_chpl + INT64(0)) = *(coerce_tmp_chpl9 + INT64(0));
#line 83 "../lib/MNISTTester.chpl"
  _formal_tmp_ind_x0_chpl = *(_formal_tmp_in_ind_chpl + INT64(0));
#line 83 "../lib/MNISTTester.chpl"
  chpl_check_nil(coerce_tmp_chpl10, INT64(83), INT32(66));
#line 83 "../lib/MNISTTester.chpl"
  coerce_tmp_chpl11 = (coerce_tmp_chpl10)->shiftedData;
#line 83 "../lib/MNISTTester.chpl"
  call_tmp_chpl40 = (coerce_tmp_chpl11 + _formal_tmp_ind_x0_chpl);
#line 83 "../lib/MNISTTester.chpl"
  call_tmp_chpl41 = &((call_tmp_chpl40)->x0);
#line 83 "../lib/MNISTTester.chpl"
  forwardProp_chpl4(network_chpl, call_tmp_chpl41, &ret_tmp_chpl23);
#line 83 "../lib/MNISTTester.chpl"
  _return_tmp__chpl = ret_tmp_chpl23;
#line 83 "../lib/MNISTTester.chpl"
  _field_destructor_tmp__chpl = (&_return_tmp__chpl)->data;
#line 83 "../lib/MNISTTester.chpl"
  chpl__autoDestroy18(&_field_destructor_tmp__chpl, INT64(83), INT32(66));
#line 83 "../lib/MNISTTester.chpl"
  _field_destructor_tmp__chpl2 = (&_return_tmp__chpl)->_domain;
#line 83 "../lib/MNISTTester.chpl"
  chpl__autoDestroy11(&_field_destructor_tmp__chpl2, INT64(83), INT32(66));
#line 84 "../lib/MNISTTester.chpl"
  writeln_chpl4(&_str_literal_1744_chpl, INT64(84), INT32(66));
#line 86 "../lib/MNISTTester.chpl"
  call_tmp_chpl43 = chpl_null_timevalue();
#line 86 "../lib/MNISTTester.chpl"
  init_chpl228(&initTemp_chpl, call_tmp_chpl43, REAL64(0x0p+0), UINT8(false));
#line 86 "../lib/MNISTTester.chpl"
  call_tmp_chpl42 = initTemp_chpl;
#line 86 "../lib/MNISTTester.chpl"
  t_chpl = call_tmp_chpl42;
#line 87 "../lib/MNISTTester.chpl"
  start_chpl(&t_chpl, INT64(87), INT32(66));
#line 89 "../lib/MNISTTester.chpl"
  if (numEpochs_chpl2 < INT64(0)) /* ZLINE: 89 ../lib/MNISTTester.chpl */
#line 89 "../lib/MNISTTester.chpl"
  {
#line 89 "../lib/MNISTTester.chpl"
    boundsCheckHalt_chpl(&_str_literal_580_chpl, INT64(89), INT32(66));
#line 89 "../lib/MNISTTester.chpl"
  }
#line 89 "../lib/MNISTTester.chpl"
  if (numEpochs_chpl2 == INT64(0)) /* ZLINE: 89 ../lib/MNISTTester.chpl */
#line 89 "../lib/MNISTTester.chpl"
  {
#line 89 "../lib/MNISTTester.chpl"
    tmp_chpl3 = ((int64_t)(((uint64_t)((((uint64_t)(INT64(0))) - UINT64(1))))));
#line 89 "../lib/MNISTTester.chpl"
  } else /* ZLINE: 89 ../lib/MNISTTester.chpl */
#line 89 "../lib/MNISTTester.chpl"
  {
#line 89 "../lib/MNISTTester.chpl"
    tmp_chpl3 = ((int64_t)((INT64(0) + ((int64_t)((numEpochs_chpl2 - INT64(1)))))));
#line 89 "../lib/MNISTTester.chpl"
  }
#line 89 "../lib/MNISTTester.chpl"
  _ic__F1_high_chpl = tmp_chpl3;
#line 89 "../lib/MNISTTester.chpl"
  i_chpl2 = INT64(0);
#line 89 "../lib/MNISTTester.chpl"
  chpl_checkIfRangeIterWillOverflow(INT64(0), tmp_chpl3, INT64(1), INT64(0), tmp_chpl3, UINT8(true), INT64(89), INT32(66));
#line 89 "../lib/MNISTTester.chpl"
  for (i_chpl2 = INT64(0); ((i_chpl2 <= _ic__F1_high_chpl)); i_chpl2 += INT64(1)) {
#line 89 "../lib/MNISTTester.chpl"
    epoch_chpl = i_chpl2;
#line 91 "../lib/MNISTTester.chpl"
    writeln_chpl3(&_str_literal_1746_chpl, ((int64_t)((i_chpl2 + INT64(1)))), INT64(91), INT32(66));
#line 92 "../lib/MNISTTester.chpl"
    i_this_chpl2 = &trainingData_chpl;
#line 92 "../lib/MNISTTester.chpl"
    *(coerce_tmp_chpl12 + INT64(0)) = INT64(0);
#line 92 "../lib/MNISTTester.chpl"
    i_i_chpl2 = &coerce_tmp_chpl12;
#line 92 "../lib/MNISTTester.chpl"
    ret_chpl6 = (&trainingData_chpl)->_instance;
#line 92 "../lib/MNISTTester.chpl"
    checkAccess_chpl4(i_this_chpl2, i_i_chpl2, ret_chpl6, INT64(92), INT32(66));
#line 92 "../lib/MNISTTester.chpl"
    coerce_tmp_chpl13 = ret_chpl6;
#line 92 "../lib/MNISTTester.chpl"
    *(_formal_tmp_in_ind_chpl2 + INT64(0)) = *(coerce_tmp_chpl12 + INT64(0));
#line 92 "../lib/MNISTTester.chpl"
    _formal_tmp_ind_x0_chpl2 = *(_formal_tmp_in_ind_chpl2 + INT64(0));
#line 92 "../lib/MNISTTester.chpl"
    chpl_check_nil(coerce_tmp_chpl13, INT64(92), INT32(66));
#line 92 "../lib/MNISTTester.chpl"
    coerce_tmp_chpl14 = (coerce_tmp_chpl13)->shiftedData;
#line 92 "../lib/MNISTTester.chpl"
    call_tmp_chpl44 = (coerce_tmp_chpl14 + _formal_tmp_ind_x0_chpl2);
#line 92 "../lib/MNISTTester.chpl"
    call_tmp_chpl45 = &((call_tmp_chpl44)->x0);
#line 92 "../lib/MNISTTester.chpl"
    forwardProp_chpl4(network_chpl, call_tmp_chpl45, &ret_tmp_chpl24);
#line 92 "../lib/MNISTTester.chpl"
    _return_tmp__chpl2 = ret_tmp_chpl24;
#line 92 "../lib/MNISTTester.chpl"
    _field_destructor_tmp__chpl3 = (&_return_tmp__chpl2)->data;
#line 92 "../lib/MNISTTester.chpl"
    chpl__autoDestroy18(&_field_destructor_tmp__chpl3, INT64(92), INT32(66));
#line 92 "../lib/MNISTTester.chpl"
    _field_destructor_tmp__chpl4 = (&_return_tmp__chpl2)->_domain;
#line 92 "../lib/MNISTTester.chpl"
    chpl__autoDestroy11(&_field_destructor_tmp__chpl4, INT64(92), INT32(66));
#line 94 "../lib/MNISTTester.chpl"
    shuffle_chpl3(&trainingData_chpl);
#line 96 "../lib/MNISTTester.chpl"
    call_tmp_chpl46 = size_chpl2(&trainingData_chpl, INT64(96), INT32(66));
#line 96 "../lib/MNISTTester.chpl"
    if (batchSize_chpl2 == INT64(0)) /* ZLINE: 96 ../lib/MNISTTester.chpl */
#line 96 "../lib/MNISTTester.chpl"
    {
#line 96 "../lib/MNISTTester.chpl"
      halt_chpl(&_str_literal_30_chpl, INT64(96), INT32(66));
#line 96 "../lib/MNISTTester.chpl"
    }
#line 96 "../lib/MNISTTester.chpl"
    call_tmp_chpl47 = (call_tmp_chpl46 / batchSize_chpl2);
#line 96 "../lib/MNISTTester.chpl"
    if (call_tmp_chpl47 < INT64(0)) /* ZLINE: 96 ../lib/MNISTTester.chpl */
#line 96 "../lib/MNISTTester.chpl"
    {
#line 96 "../lib/MNISTTester.chpl"
      boundsCheckHalt_chpl(&_str_literal_580_chpl, INT64(96), INT32(66));
#line 96 "../lib/MNISTTester.chpl"
    }
#line 96 "../lib/MNISTTester.chpl"
    if (call_tmp_chpl47 == INT64(0)) /* ZLINE: 96 ../lib/MNISTTester.chpl */
#line 96 "../lib/MNISTTester.chpl"
    {
#line 96 "../lib/MNISTTester.chpl"
      tmp_chpl4 = ((int64_t)(((uint64_t)((((uint64_t)(INT64(0))) - UINT64(1))))));
#line 96 "../lib/MNISTTester.chpl"
    } else /* ZLINE: 96 ../lib/MNISTTester.chpl */
#line 96 "../lib/MNISTTester.chpl"
    {
#line 96 "../lib/MNISTTester.chpl"
      tmp_chpl4 = ((int64_t)((INT64(0) + ((int64_t)((call_tmp_chpl47 - INT64(1)))))));
#line 96 "../lib/MNISTTester.chpl"
    }
#line 96 "../lib/MNISTTester.chpl"
    _ic__F1_high_chpl2 = tmp_chpl4;
#line 96 "../lib/MNISTTester.chpl"
    i_chpl3 = INT64(0);
#line 96 "../lib/MNISTTester.chpl"
    chpl_checkIfRangeIterWillOverflow(INT64(0), tmp_chpl4, INT64(1), INT64(0), tmp_chpl4, UINT8(true), INT64(96), INT32(66));
#line 96 "../lib/MNISTTester.chpl"
    for (i_chpl3 = INT64(0); ((i_chpl3 <= _ic__F1_high_chpl2)); i_chpl3 += INT64(1)) {
#line 97 "../lib/MNISTTester.chpl"
      chpl_build_low_bounded_range2(((int64_t)((i_chpl3 * batchSize_chpl2))), &ret_tmp_chpl25);
#line 97 "../lib/MNISTTester.chpl"
      call_tmp_chpl48 = ret_tmp_chpl25;
#line 97 "../lib/MNISTTester.chpl"
      chpl___POUND_3(call_tmp_chpl48, batchSize_chpl2, &ret_tmp_chpl26, INT64(97), INT32(66));
#line 97 "../lib/MNISTTester.chpl"
      call_tmp_chpl49 = ret_tmp_chpl26;
#line 98 "../lib/MNISTTester.chpl"
      this_chpl(&trainingData_chpl, call_tmp_chpl49, &ret_tmp_chpl27, INT64(98), INT32(66));
#line 98 "../lib/MNISTTester.chpl"
      call_tmp_chpl50 = ret_tmp_chpl27;
#line 98 "../lib/MNISTTester.chpl"
      batch_chpl = chpl__initCopy10(&call_tmp_chpl50, UINT8(true), INT64(98), INT32(66));
#line 98 "../lib/MNISTTester.chpl"
      chpl__autoDestroy34(&call_tmp_chpl50, INT64(98), INT32(66));
#line 99 "../lib/MNISTTester.chpl"
      train_chpl(network_chpl, &batch_chpl, learnRate_chpl2, &ret_tmp_chpl28);
#line 99 "../lib/MNISTTester.chpl"
      call_tmp_chpl51 = ret_tmp_chpl28;
#line 99 "../lib/MNISTTester.chpl"
      tmp_x0_chpl3 = (&call_tmp_chpl51)->x0;
#line 99 "../lib/MNISTTester.chpl"
      tmp_x1_chpl3 = (&call_tmp_chpl51)->x1;
#line 101 "../lib/MNISTTester.chpl"
      call_tmp_chpl52 = (i_chpl3 + INT64(1));
#line 101 "../lib/MNISTTester.chpl"
      call_tmp_chpl53 = size_chpl2(&trainingData_chpl, INT64(101), INT32(66));
#line 101 "../lib/MNISTTester.chpl"
      if (batchSize_chpl2 == INT64(0)) /* ZLINE: 101 ../lib/MNISTTester.chpl */
#line 101 "../lib/MNISTTester.chpl"
      {
#line 101 "../lib/MNISTTester.chpl"
        halt_chpl(&_str_literal_30_chpl, INT64(101), INT32(66));
#line 101 "../lib/MNISTTester.chpl"
      }
#line 101 "../lib/MNISTTester.chpl"
      call_tmp_chpl54 = (call_tmp_chpl53 / batchSize_chpl2);
#line 101 "../lib/MNISTTester.chpl"
      call_tmp_chpl55 = (tmp_x0_chpl3 / ((_real64)(batchSize_chpl2)));
#line 101 "../lib/MNISTTester.chpl"
      error_chpl4 = NULL;
#line 101 "../lib/MNISTTester.chpl"
      i_this_chpl3 = &stdout_chpl;
#line 101 "../lib/MNISTTester.chpl"
      inlineImm_chpl = _str_literal_1747_chpl;
#line 101 "../lib/MNISTTester.chpl"
      i__e0_args_chpl = &inlineImm_chpl;
#line 101 "../lib/MNISTTester.chpl"
      inlineImm_chpl2 = _str_literal_848_chpl;
#line 101 "../lib/MNISTTester.chpl"
      i__e1_args_chpl = &inlineImm_chpl2;
#line 101 "../lib/MNISTTester.chpl"
      inlineImm_chpl3 = _str_literal_1748_chpl;
#line 101 "../lib/MNISTTester.chpl"
      i__e3_args_chpl = &inlineImm_chpl3;
#line 101 "../lib/MNISTTester.chpl"
      inlineImm_chpl4 = _str_literal_1749_chpl;
#line 101 "../lib/MNISTTester.chpl"
      i__e5_args_chpl = &inlineImm_chpl4;
#line 101 "../lib/MNISTTester.chpl"
      inlineImm_chpl5 = _str_literal_1750_chpl;
#line 101 "../lib/MNISTTester.chpl"
      i__e7_args_chpl = &inlineImm_chpl5;
#line 101 "../lib/MNISTTester.chpl"
      inlineImm_chpl6 = _str_literal_1643_chpl;
#line 101 "../lib/MNISTTester.chpl"
      i__e9_args_chpl = &inlineImm_chpl6;
#line 101 "../lib/MNISTTester.chpl"
      inlineImm_chpl7 = _str_literal_415_chpl;
#line 101 "../lib/MNISTTester.chpl"
      i__e11_args_chpl = &inlineImm_chpl7;
#line 101 "../lib/MNISTTester.chpl"
      (&this_chpl33)->x0 = i__e0_args_chpl;
#line 101 "../lib/MNISTTester.chpl"
      (&this_chpl33)->x1 = i__e1_args_chpl;
#line 101 "../lib/MNISTTester.chpl"
      (&this_chpl33)->x2 = call_tmp_chpl52;
#line 101 "../lib/MNISTTester.chpl"
      (&this_chpl33)->x3 = i__e3_args_chpl;
#line 101 "../lib/MNISTTester.chpl"
      (&this_chpl33)->x4 = call_tmp_chpl54;
#line 101 "../lib/MNISTTester.chpl"
      (&this_chpl33)->x5 = i__e5_args_chpl;
#line 101 "../lib/MNISTTester.chpl"
      (&this_chpl33)->x6 = call_tmp_chpl55;
#line 101 "../lib/MNISTTester.chpl"
      (&this_chpl33)->x7 = i__e7_args_chpl;
#line 101 "../lib/MNISTTester.chpl"
      (&this_chpl33)->x8 = tmp_x1_chpl3;
#line 101 "../lib/MNISTTester.chpl"
      (&this_chpl33)->x9 = i__e9_args_chpl;
#line 101 "../lib/MNISTTester.chpl"
      (&this_chpl33)->x10 = batchSize_chpl2;
#line 101 "../lib/MNISTTester.chpl"
      (&this_chpl33)->x11 = i__e11_args_chpl;
#line 101 "../lib/MNISTTester.chpl"
      args_chpl = this_chpl33;
#line 101 "../lib/MNISTTester.chpl"
      call_tmp_chpl56 = &((&local_stdout_chpl)->_readWriteThisFromLocale);
#line 101 "../lib/MNISTTester.chpl"
      init_ASSIGN__chpl(&ret_chpl8, call_tmp_chpl56);
#line 101 "../lib/MNISTTester.chpl"
      call_tmp_chpl57 = chpl___EQUALS_(&ret_chpl8, &local_nilLocale_chpl, INT64(101), INT32(66));
#line 101 "../lib/MNISTTester.chpl"
      if (call_tmp_chpl57) /* ZLINE: 101 ../lib/MNISTTester.chpl */
#line 101 "../lib/MNISTTester.chpl"
      {
#line 101 "../lib/MNISTTester.chpl"
        call_tmp_chpl60 = get_chpl_nodeID();
#line 101 "../lib/MNISTTester.chpl"
        call_tmp_chpl61 = chpl_rt_buildLocaleID(call_tmp_chpl60, c_sublocid_any);
#line 101 "../lib/MNISTTester.chpl"
        call_tmp_chpl59 = call_tmp_chpl61;
#line 101 "../lib/MNISTTester.chpl"
        chpl_localeID_to_locale(&call_tmp_chpl59, &ret_tmp_chpl29, INT64(101), INT32(66));
#line 101 "../lib/MNISTTester.chpl"
        call_tmp_chpl62 = ret_tmp_chpl29;
#line 101 "../lib/MNISTTester.chpl"
        call_tmp_chpl58 = call_tmp_chpl62;
#line 101 "../lib/MNISTTester.chpl"
        chpl___ASSIGN_9(&ret_chpl8, &call_tmp_chpl58);
#line 101 "../lib/MNISTTester.chpl"
        i_x_chpl3 = &call_tmp_chpl58;
#line 101 "../lib/MNISTTester.chpl"
        deinit_chpl62(i_x_chpl3);
#line 101 "../lib/MNISTTester.chpl"
      }
#line 101 "../lib/MNISTTester.chpl"
      ret_chpl7 = ret_chpl8;
#line 101 "../lib/MNISTTester.chpl"
      origLocale_chpl = ret_chpl7;
#line 101 "../lib/MNISTTester.chpl"
      call_tmp_chpl63 = &((&local_stdout_chpl)->_home);
#line 101 "../lib/MNISTTester.chpl"
      if (((RootClass_chpl)((call_tmp_chpl63)->_instance)) == nil) /* ZLINE: 101 ../lib/MNISTTester.chpl */
#line 101 "../lib/MNISTTester.chpl"
      {
#line 101 "../lib/MNISTTester.chpl"
        nilCheckHalt_chpl(&_str_literal_52_chpl, INT64(101), INT32(66));
#line 101 "../lib/MNISTTester.chpl"
      }
#line 101 "../lib/MNISTTester.chpl"
      error_chpl5 = NULL;
#line 101 "../lib/MNISTTester.chpl"
      qio_int_to_err(((int32_t)(INT64(0))));
#line 101 "../lib/MNISTTester.chpl"
      if ((&local_stdout_chpl)->_channel_internal == nil) /* ZLINE: 101 ../lib/MNISTTester.chpl */
#line 101 "../lib/MNISTTester.chpl"
      {
#line 101 "../lib/MNISTTester.chpl"
        createSystemError_chpl2(((int64_t)(EINVAL)), &_str_literal_1920_chpl, &ret_tmp_chpl30, INT64(101), INT32(66));
#line 101 "../lib/MNISTTester.chpl"
        call_tmp_chpl64 = ret_tmp_chpl30;
#line 101 "../lib/MNISTTester.chpl"
        i_x_chpl4 = &call_tmp_chpl64;
#line 101 "../lib/MNISTTester.chpl"
        i__retArg_chpl = &ret_tmp_chpl31;
#line 101 "../lib/MNISTTester.chpl"
        _formal_tmp_x_chpl = *(i_x_chpl4);
#line 101 "../lib/MNISTTester.chpl"
        i_this_chpl4 = &_formal_tmp_x_chpl;
#line 101 "../lib/MNISTTester.chpl"
        call_tmp_chpl66 = ((Error_chpl)((i_this_chpl4)->chpl_p));
#line 101 "../lib/MNISTTester.chpl"
        i_this_chpl5 = &_formal_tmp_x_chpl;
#line 101 "../lib/MNISTTester.chpl"
        (i_this_chpl5)->chpl_p = ((SystemError_chpl)(nil));
#line 101 "../lib/MNISTTester.chpl"
        if (((RootClass_chpl)(call_tmp_chpl66)) == nil) /* ZLINE: 101 ../lib/MNISTTester.chpl */
#line 101 "../lib/MNISTTester.chpl"
        {
#line 101 "../lib/MNISTTester.chpl"
          nilCheckHalt_chpl(&_str_literal_52_chpl, INT64(101), INT32(66));
#line 101 "../lib/MNISTTester.chpl"
        }
#line 101 "../lib/MNISTTester.chpl"
        init_chpl188(&initTemp_chpl2, call_tmp_chpl66);
#line 101 "../lib/MNISTTester.chpl"
        call_tmp_chpl67 = initTemp_chpl2;
#line 101 "../lib/MNISTTester.chpl"
        ret_chpl9 = call_tmp_chpl67;
#line 101 "../lib/MNISTTester.chpl"
        chpl__autoDestroy54(&_formal_tmp_x_chpl, INT64(101), INT32(66));
#line 101 "../lib/MNISTTester.chpl"
        *(i__retArg_chpl) = ret_chpl9;
#line 101 "../lib/MNISTTester.chpl"
        coerce_tmp_chpl15 = ret_tmp_chpl31;
#line 101 "../lib/MNISTTester.chpl"
        _formal_tmp_in_err_chpl = coerce_tmp_chpl15;
#line 101 "../lib/MNISTTester.chpl"
        call_tmp_chpl65 = chpl_fix_thrown_error(&_formal_tmp_in_err_chpl, INT64(101), INT32(66));
#line 101 "../lib/MNISTTester.chpl"
        error_chpl5 = call_tmp_chpl65;
#line 101 "../lib/MNISTTester.chpl"
        goto _endlock_chpl;
#line 101 "../lib/MNISTTester.chpl"
      }
#line 101 "../lib/MNISTTester.chpl"
      call_tmp_chpl68 = &((&local_stdout_chpl)->_home);
#line 101 "../lib/MNISTTester.chpl"
      if (((RootClass_chpl)((call_tmp_chpl68)->_instance)) == nil) /* ZLINE: 101 ../lib/MNISTTester.chpl */
#line 101 "../lib/MNISTTester.chpl"
      {
#line 101 "../lib/MNISTTester.chpl"
        nilCheckHalt_chpl(&_str_literal_52_chpl, INT64(101), INT32(66));
#line 101 "../lib/MNISTTester.chpl"
      }
#line 101 "../lib/MNISTTester.chpl"
      call_tmp_chpl69 = qio_channel_lock((&local_stdout_chpl)->_channel_internal);
#line 101 "../lib/MNISTTester.chpl"
      call_tmp_chpl70 = qio_err_iserr(call_tmp_chpl69);
#line 101 "../lib/MNISTTester.chpl"
      if (call_tmp_chpl70 != INT32(0)) /* ZLINE: 101 ../lib/MNISTTester.chpl */
#line 101 "../lib/MNISTTester.chpl"
      {
#line 101 "../lib/MNISTTester.chpl"
        error_chpl6 = NULL;
#line 101 "../lib/MNISTTester.chpl"
        _ch_ioerror_chpl5(i_this_chpl3, call_tmp_chpl69, &_str_literal_1919_chpl, &error_chpl6, INT64(101), INT32(66));
#line 101 "../lib/MNISTTester.chpl"
        if (error_chpl6 != nil) /* ZLINE: 101 ../lib/MNISTTester.chpl */
#line 101 "../lib/MNISTTester.chpl"
        {
#line 101 "../lib/MNISTTester.chpl"
          goto handler_chpl;
#line 101 "../lib/MNISTTester.chpl"
        }
#line 101 "../lib/MNISTTester.chpl"
        handler_chpl:;
#line 101 "../lib/MNISTTester.chpl"
        if (error_chpl6 != nil) /* ZLINE: 101 ../lib/MNISTTester.chpl */
#line 101 "../lib/MNISTTester.chpl"
        {
#line 101 "../lib/MNISTTester.chpl"
          error_chpl5 = error_chpl6;
#line 101 "../lib/MNISTTester.chpl"
          goto _endlock_chpl;
#line 101 "../lib/MNISTTester.chpl"
        }
#line 101 "../lib/MNISTTester.chpl"
      }
#line 101 "../lib/MNISTTester.chpl"
      _endlock_chpl:;
#line 101 "../lib/MNISTTester.chpl"
      if (error_chpl5 != nil) /* ZLINE: 101 ../lib/MNISTTester.chpl */
#line 101 "../lib/MNISTTester.chpl"
      {
#line 101 "../lib/MNISTTester.chpl"
        goto handler_chpl2;
#line 101 "../lib/MNISTTester.chpl"
      }
#line 101 "../lib/MNISTTester.chpl"
      handler_chpl2:;
#line 101 "../lib/MNISTTester.chpl"
      if (error_chpl5 != nil) /* ZLINE: 101 ../lib/MNISTTester.chpl */
#line 101 "../lib/MNISTTester.chpl"
      {
#line 101 "../lib/MNISTTester.chpl"
        error_chpl4 = error_chpl5;
#line 101 "../lib/MNISTTester.chpl"
        i_x_chpl5 = &origLocale_chpl;
#line 101 "../lib/MNISTTester.chpl"
        deinit_chpl62(i_x_chpl5);
#line 101 "../lib/MNISTTester.chpl"
        goto _endwrite_chpl;
#line 101 "../lib/MNISTTester.chpl"
      }
#line 101 "../lib/MNISTTester.chpl"
      error_chpl7 = NULL;
#line 101 "../lib/MNISTTester.chpl"
      call_tmp_chpl71 = (&args_chpl)->x0;
#line 101 "../lib/MNISTTester.chpl"
      _writeOne_chpl(i_this_chpl3, call_tmp_chpl71, &origLocale_chpl, &error_chpl7, INT64(101), INT32(66));
#line 101 "../lib/MNISTTester.chpl"
      if (error_chpl7 != nil) /* ZLINE: 101 ../lib/MNISTTester.chpl */
#line 101 "../lib/MNISTTester.chpl"
      {
#line 101 "../lib/MNISTTester.chpl"
        goto handler_chpl3;
#line 101 "../lib/MNISTTester.chpl"
      }
#line 101 "../lib/MNISTTester.chpl"
      handler_chpl3:;
#line 101 "../lib/MNISTTester.chpl"
      if (error_chpl7 != nil) /* ZLINE: 101 ../lib/MNISTTester.chpl */
#line 101 "../lib/MNISTTester.chpl"
      {
#line 101 "../lib/MNISTTester.chpl"
        error_chpl4 = error_chpl7;
#line 101 "../lib/MNISTTester.chpl"
        call_tmp_chpl72 = &((&local_stdout_chpl)->_home);
#line 101 "../lib/MNISTTester.chpl"
        if (((RootClass_chpl)((call_tmp_chpl72)->_instance)) == nil) /* ZLINE: 101 ../lib/MNISTTester.chpl */
#line 101 "../lib/MNISTTester.chpl"
        {
#line 101 "../lib/MNISTTester.chpl"
          nilCheckHalt_chpl(&_str_literal_52_chpl, INT64(101), INT32(66));
#line 101 "../lib/MNISTTester.chpl"
        }
#line 101 "../lib/MNISTTester.chpl"
        qio_channel_unlock((&local_stdout_chpl)->_channel_internal);
#line 101 "../lib/MNISTTester.chpl"
        i_x_chpl6 = &origLocale_chpl;
#line 101 "../lib/MNISTTester.chpl"
        deinit_chpl62(i_x_chpl6);
#line 101 "../lib/MNISTTester.chpl"
        goto _endwrite_chpl;
#line 101 "../lib/MNISTTester.chpl"
      }
#line 101 "../lib/MNISTTester.chpl"
      error_chpl8 = NULL;
#line 101 "../lib/MNISTTester.chpl"
      call_tmp_chpl73 = (&args_chpl)->x1;
#line 101 "../lib/MNISTTester.chpl"
      _writeOne_chpl(i_this_chpl3, call_tmp_chpl73, &origLocale_chpl, &error_chpl8, INT64(101), INT32(66));
#line 101 "../lib/MNISTTester.chpl"
      if (error_chpl8 != nil) /* ZLINE: 101 ../lib/MNISTTester.chpl */
#line 101 "../lib/MNISTTester.chpl"
      {
#line 101 "../lib/MNISTTester.chpl"
        goto handler_chpl4;
#line 101 "../lib/MNISTTester.chpl"
      }
#line 101 "../lib/MNISTTester.chpl"
      handler_chpl4:;
#line 101 "../lib/MNISTTester.chpl"
      if (error_chpl8 != nil) /* ZLINE: 101 ../lib/MNISTTester.chpl */
#line 101 "../lib/MNISTTester.chpl"
      {
#line 101 "../lib/MNISTTester.chpl"
        error_chpl4 = error_chpl8;
#line 101 "../lib/MNISTTester.chpl"
        call_tmp_chpl74 = &((&local_stdout_chpl)->_home);
#line 101 "../lib/MNISTTester.chpl"
        if (((RootClass_chpl)((call_tmp_chpl74)->_instance)) == nil) /* ZLINE: 101 ../lib/MNISTTester.chpl */
#line 101 "../lib/MNISTTester.chpl"
        {
#line 101 "../lib/MNISTTester.chpl"
          nilCheckHalt_chpl(&_str_literal_52_chpl, INT64(101), INT32(66));
#line 101 "../lib/MNISTTester.chpl"
        }
#line 101 "../lib/MNISTTester.chpl"
        qio_channel_unlock((&local_stdout_chpl)->_channel_internal);
#line 101 "../lib/MNISTTester.chpl"
        i_x_chpl7 = &origLocale_chpl;
#line 101 "../lib/MNISTTester.chpl"
        deinit_chpl62(i_x_chpl7);
#line 101 "../lib/MNISTTester.chpl"
        goto _endwrite_chpl;
#line 101 "../lib/MNISTTester.chpl"
      }
#line 101 "../lib/MNISTTester.chpl"
      error_chpl9 = NULL;
#line 101 "../lib/MNISTTester.chpl"
      _writeOne_chpl3(i_this_chpl3, (&args_chpl)->x2, &origLocale_chpl, &error_chpl9, INT64(101), INT32(66));
#line 101 "../lib/MNISTTester.chpl"
      if (error_chpl9 != nil) /* ZLINE: 101 ../lib/MNISTTester.chpl */
#line 101 "../lib/MNISTTester.chpl"
      {
#line 101 "../lib/MNISTTester.chpl"
        goto handler_chpl5;
#line 101 "../lib/MNISTTester.chpl"
      }
#line 101 "../lib/MNISTTester.chpl"
      handler_chpl5:;
#line 101 "../lib/MNISTTester.chpl"
      if (error_chpl9 != nil) /* ZLINE: 101 ../lib/MNISTTester.chpl */
#line 101 "../lib/MNISTTester.chpl"
      {
#line 101 "../lib/MNISTTester.chpl"
        error_chpl4 = error_chpl9;
#line 101 "../lib/MNISTTester.chpl"
        call_tmp_chpl75 = &((&local_stdout_chpl)->_home);
#line 101 "../lib/MNISTTester.chpl"
        if (((RootClass_chpl)((call_tmp_chpl75)->_instance)) == nil) /* ZLINE: 101 ../lib/MNISTTester.chpl */
#line 101 "../lib/MNISTTester.chpl"
        {
#line 101 "../lib/MNISTTester.chpl"
          nilCheckHalt_chpl(&_str_literal_52_chpl, INT64(101), INT32(66));
#line 101 "../lib/MNISTTester.chpl"
        }
#line 101 "../lib/MNISTTester.chpl"
        qio_channel_unlock((&local_stdout_chpl)->_channel_internal);
#line 101 "../lib/MNISTTester.chpl"
        i_x_chpl8 = &origLocale_chpl;
#line 101 "../lib/MNISTTester.chpl"
        deinit_chpl62(i_x_chpl8);
#line 101 "../lib/MNISTTester.chpl"
        goto _endwrite_chpl;
#line 101 "../lib/MNISTTester.chpl"
      }
#line 101 "../lib/MNISTTester.chpl"
      error_chpl10 = NULL;
#line 101 "../lib/MNISTTester.chpl"
      call_tmp_chpl76 = (&args_chpl)->x3;
#line 101 "../lib/MNISTTester.chpl"
      _writeOne_chpl(i_this_chpl3, call_tmp_chpl76, &origLocale_chpl, &error_chpl10, INT64(101), INT32(66));
#line 101 "../lib/MNISTTester.chpl"
      if (error_chpl10 != nil) /* ZLINE: 101 ../lib/MNISTTester.chpl */
#line 101 "../lib/MNISTTester.chpl"
      {
#line 101 "../lib/MNISTTester.chpl"
        goto handler_chpl6;
#line 101 "../lib/MNISTTester.chpl"
      }
#line 101 "../lib/MNISTTester.chpl"
      handler_chpl6:;
#line 101 "../lib/MNISTTester.chpl"
      if (error_chpl10 != nil) /* ZLINE: 101 ../lib/MNISTTester.chpl */
#line 101 "../lib/MNISTTester.chpl"
      {
#line 101 "../lib/MNISTTester.chpl"
        error_chpl4 = error_chpl10;
#line 101 "../lib/MNISTTester.chpl"
        call_tmp_chpl77 = &((&local_stdout_chpl)->_home);
#line 101 "../lib/MNISTTester.chpl"
        if (((RootClass_chpl)((call_tmp_chpl77)->_instance)) == nil) /* ZLINE: 101 ../lib/MNISTTester.chpl */
#line 101 "../lib/MNISTTester.chpl"
        {
#line 101 "../lib/MNISTTester.chpl"
          nilCheckHalt_chpl(&_str_literal_52_chpl, INT64(101), INT32(66));
#line 101 "../lib/MNISTTester.chpl"
        }
#line 101 "../lib/MNISTTester.chpl"
        qio_channel_unlock((&local_stdout_chpl)->_channel_internal);
#line 101 "../lib/MNISTTester.chpl"
        i_x_chpl9 = &origLocale_chpl;
#line 101 "../lib/MNISTTester.chpl"
        deinit_chpl62(i_x_chpl9);
#line 101 "../lib/MNISTTester.chpl"
        goto _endwrite_chpl;
#line 101 "../lib/MNISTTester.chpl"
      }
#line 101 "../lib/MNISTTester.chpl"
      error_chpl11 = NULL;
#line 101 "../lib/MNISTTester.chpl"
      _writeOne_chpl3(i_this_chpl3, (&args_chpl)->x4, &origLocale_chpl, &error_chpl11, INT64(101), INT32(66));
#line 101 "../lib/MNISTTester.chpl"
      if (error_chpl11 != nil) /* ZLINE: 101 ../lib/MNISTTester.chpl */
#line 101 "../lib/MNISTTester.chpl"
      {
#line 101 "../lib/MNISTTester.chpl"
        goto handler_chpl7;
#line 101 "../lib/MNISTTester.chpl"
      }
#line 101 "../lib/MNISTTester.chpl"
      handler_chpl7:;
#line 101 "../lib/MNISTTester.chpl"
      if (error_chpl11 != nil) /* ZLINE: 101 ../lib/MNISTTester.chpl */
#line 101 "../lib/MNISTTester.chpl"
      {
#line 101 "../lib/MNISTTester.chpl"
        error_chpl4 = error_chpl11;
#line 101 "../lib/MNISTTester.chpl"
        call_tmp_chpl78 = &((&local_stdout_chpl)->_home);
#line 101 "../lib/MNISTTester.chpl"
        if (((RootClass_chpl)((call_tmp_chpl78)->_instance)) == nil) /* ZLINE: 101 ../lib/MNISTTester.chpl */
#line 101 "../lib/MNISTTester.chpl"
        {
#line 101 "../lib/MNISTTester.chpl"
          nilCheckHalt_chpl(&_str_literal_52_chpl, INT64(101), INT32(66));
#line 101 "../lib/MNISTTester.chpl"
        }
#line 101 "../lib/MNISTTester.chpl"
        qio_channel_unlock((&local_stdout_chpl)->_channel_internal);
#line 101 "../lib/MNISTTester.chpl"
        i_x_chpl10 = &origLocale_chpl;
#line 101 "../lib/MNISTTester.chpl"
        deinit_chpl62(i_x_chpl10);
#line 101 "../lib/MNISTTester.chpl"
        goto _endwrite_chpl;
#line 101 "../lib/MNISTTester.chpl"
      }
#line 101 "../lib/MNISTTester.chpl"
      error_chpl12 = NULL;
#line 101 "../lib/MNISTTester.chpl"
      call_tmp_chpl79 = (&args_chpl)->x5;
#line 101 "../lib/MNISTTester.chpl"
      _writeOne_chpl(i_this_chpl3, call_tmp_chpl79, &origLocale_chpl, &error_chpl12, INT64(101), INT32(66));
#line 101 "../lib/MNISTTester.chpl"
      if (error_chpl12 != nil) /* ZLINE: 101 ../lib/MNISTTester.chpl */
#line 101 "../lib/MNISTTester.chpl"
      {
#line 101 "../lib/MNISTTester.chpl"
        goto handler_chpl8;
#line 101 "../lib/MNISTTester.chpl"
      }
#line 101 "../lib/MNISTTester.chpl"
      handler_chpl8:;
#line 101 "../lib/MNISTTester.chpl"
      if (error_chpl12 != nil) /* ZLINE: 101 ../lib/MNISTTester.chpl */
#line 101 "../lib/MNISTTester.chpl"
      {
#line 101 "../lib/MNISTTester.chpl"
        error_chpl4 = error_chpl12;
#line 101 "../lib/MNISTTester.chpl"
        call_tmp_chpl80 = &((&local_stdout_chpl)->_home);
#line 101 "../lib/MNISTTester.chpl"
        if (((RootClass_chpl)((call_tmp_chpl80)->_instance)) == nil) /* ZLINE: 101 ../lib/MNISTTester.chpl */
#line 101 "../lib/MNISTTester.chpl"
        {
#line 101 "../lib/MNISTTester.chpl"
          nilCheckHalt_chpl(&_str_literal_52_chpl, INT64(101), INT32(66));
#line 101 "../lib/MNISTTester.chpl"
        }
#line 101 "../lib/MNISTTester.chpl"
        qio_channel_unlock((&local_stdout_chpl)->_channel_internal);
#line 101 "../lib/MNISTTester.chpl"
        i_x_chpl11 = &origLocale_chpl;
#line 101 "../lib/MNISTTester.chpl"
        deinit_chpl62(i_x_chpl11);
#line 101 "../lib/MNISTTester.chpl"
        goto _endwrite_chpl;
#line 101 "../lib/MNISTTester.chpl"
      }
#line 101 "../lib/MNISTTester.chpl"
      error_chpl13 = NULL;
#line 101 "../lib/MNISTTester.chpl"
      _writeOne_chpl8(i_this_chpl3, (&args_chpl)->x6, &origLocale_chpl, &error_chpl13, INT64(101), INT32(66));
#line 101 "../lib/MNISTTester.chpl"
      if (error_chpl13 != nil) /* ZLINE: 101 ../lib/MNISTTester.chpl */
#line 101 "../lib/MNISTTester.chpl"
      {
#line 101 "../lib/MNISTTester.chpl"
        goto handler_chpl9;
#line 101 "../lib/MNISTTester.chpl"
      }
#line 101 "../lib/MNISTTester.chpl"
      handler_chpl9:;
#line 101 "../lib/MNISTTester.chpl"
      if (error_chpl13 != nil) /* ZLINE: 101 ../lib/MNISTTester.chpl */
#line 101 "../lib/MNISTTester.chpl"
      {
#line 101 "../lib/MNISTTester.chpl"
        error_chpl4 = error_chpl13;
#line 101 "../lib/MNISTTester.chpl"
        call_tmp_chpl81 = &((&local_stdout_chpl)->_home);
#line 101 "../lib/MNISTTester.chpl"
        if (((RootClass_chpl)((call_tmp_chpl81)->_instance)) == nil) /* ZLINE: 101 ../lib/MNISTTester.chpl */
#line 101 "../lib/MNISTTester.chpl"
        {
#line 101 "../lib/MNISTTester.chpl"
          nilCheckHalt_chpl(&_str_literal_52_chpl, INT64(101), INT32(66));
#line 101 "../lib/MNISTTester.chpl"
        }
#line 101 "../lib/MNISTTester.chpl"
        qio_channel_unlock((&local_stdout_chpl)->_channel_internal);
#line 101 "../lib/MNISTTester.chpl"
        i_x_chpl12 = &origLocale_chpl;
#line 101 "../lib/MNISTTester.chpl"
        deinit_chpl62(i_x_chpl12);
#line 101 "../lib/MNISTTester.chpl"
        goto _endwrite_chpl;
#line 101 "../lib/MNISTTester.chpl"
      }
#line 101 "../lib/MNISTTester.chpl"
      error_chpl14 = NULL;
#line 101 "../lib/MNISTTester.chpl"
      call_tmp_chpl82 = (&args_chpl)->x7;
#line 101 "../lib/MNISTTester.chpl"
      _writeOne_chpl(i_this_chpl3, call_tmp_chpl82, &origLocale_chpl, &error_chpl14, INT64(101), INT32(66));
#line 101 "../lib/MNISTTester.chpl"
      if (error_chpl14 != nil) /* ZLINE: 101 ../lib/MNISTTester.chpl */
#line 101 "../lib/MNISTTester.chpl"
      {
#line 101 "../lib/MNISTTester.chpl"
        goto handler_chpl10;
#line 101 "../lib/MNISTTester.chpl"
      }
#line 101 "../lib/MNISTTester.chpl"
      handler_chpl10:;
#line 101 "../lib/MNISTTester.chpl"
      if (error_chpl14 != nil) /* ZLINE: 101 ../lib/MNISTTester.chpl */
#line 101 "../lib/MNISTTester.chpl"
      {
#line 101 "../lib/MNISTTester.chpl"
        error_chpl4 = error_chpl14;
#line 101 "../lib/MNISTTester.chpl"
        call_tmp_chpl83 = &((&local_stdout_chpl)->_home);
#line 101 "../lib/MNISTTester.chpl"
        if (((RootClass_chpl)((call_tmp_chpl83)->_instance)) == nil) /* ZLINE: 101 ../lib/MNISTTester.chpl */
#line 101 "../lib/MNISTTester.chpl"
        {
#line 101 "../lib/MNISTTester.chpl"
          nilCheckHalt_chpl(&_str_literal_52_chpl, INT64(101), INT32(66));
#line 101 "../lib/MNISTTester.chpl"
        }
#line 101 "../lib/MNISTTester.chpl"
        qio_channel_unlock((&local_stdout_chpl)->_channel_internal);
#line 101 "../lib/MNISTTester.chpl"
        i_x_chpl13 = &origLocale_chpl;
#line 101 "../lib/MNISTTester.chpl"
        deinit_chpl62(i_x_chpl13);
#line 101 "../lib/MNISTTester.chpl"
        goto _endwrite_chpl;
#line 101 "../lib/MNISTTester.chpl"
      }
#line 101 "../lib/MNISTTester.chpl"
      error_chpl15 = NULL;
#line 101 "../lib/MNISTTester.chpl"
      _writeOne_chpl3(i_this_chpl3, (&args_chpl)->x8, &origLocale_chpl, &error_chpl15, INT64(101), INT32(66));
#line 101 "../lib/MNISTTester.chpl"
      if (error_chpl15 != nil) /* ZLINE: 101 ../lib/MNISTTester.chpl */
#line 101 "../lib/MNISTTester.chpl"
      {
#line 101 "../lib/MNISTTester.chpl"
        goto handler_chpl11;
#line 101 "../lib/MNISTTester.chpl"
      }
#line 101 "../lib/MNISTTester.chpl"
      handler_chpl11:;
#line 101 "../lib/MNISTTester.chpl"
      if (error_chpl15 != nil) /* ZLINE: 101 ../lib/MNISTTester.chpl */
#line 101 "../lib/MNISTTester.chpl"
      {
#line 101 "../lib/MNISTTester.chpl"
        error_chpl4 = error_chpl15;
#line 101 "../lib/MNISTTester.chpl"
        call_tmp_chpl84 = &((&local_stdout_chpl)->_home);
#line 101 "../lib/MNISTTester.chpl"
        if (((RootClass_chpl)((call_tmp_chpl84)->_instance)) == nil) /* ZLINE: 101 ../lib/MNISTTester.chpl */
#line 101 "../lib/MNISTTester.chpl"
        {
#line 101 "../lib/MNISTTester.chpl"
          nilCheckHalt_chpl(&_str_literal_52_chpl, INT64(101), INT32(66));
#line 101 "../lib/MNISTTester.chpl"
        }
#line 101 "../lib/MNISTTester.chpl"
        qio_channel_unlock((&local_stdout_chpl)->_channel_internal);
#line 101 "../lib/MNISTTester.chpl"
        i_x_chpl14 = &origLocale_chpl;
#line 101 "../lib/MNISTTester.chpl"
        deinit_chpl62(i_x_chpl14);
#line 101 "../lib/MNISTTester.chpl"
        goto _endwrite_chpl;
#line 101 "../lib/MNISTTester.chpl"
      }
#line 101 "../lib/MNISTTester.chpl"
      error_chpl16 = NULL;
#line 101 "../lib/MNISTTester.chpl"
      call_tmp_chpl85 = (&args_chpl)->x9;
#line 101 "../lib/MNISTTester.chpl"
      _writeOne_chpl(i_this_chpl3, call_tmp_chpl85, &origLocale_chpl, &error_chpl16, INT64(101), INT32(66));
#line 101 "../lib/MNISTTester.chpl"
      if (error_chpl16 != nil) /* ZLINE: 101 ../lib/MNISTTester.chpl */
#line 101 "../lib/MNISTTester.chpl"
      {
#line 101 "../lib/MNISTTester.chpl"
        goto handler_chpl12;
#line 101 "../lib/MNISTTester.chpl"
      }
#line 101 "../lib/MNISTTester.chpl"
      handler_chpl12:;
#line 101 "../lib/MNISTTester.chpl"
      if (error_chpl16 != nil) /* ZLINE: 101 ../lib/MNISTTester.chpl */
#line 101 "../lib/MNISTTester.chpl"
      {
#line 101 "../lib/MNISTTester.chpl"
        error_chpl4 = error_chpl16;
#line 101 "../lib/MNISTTester.chpl"
        call_tmp_chpl86 = &((&local_stdout_chpl)->_home);
#line 101 "../lib/MNISTTester.chpl"
        if (((RootClass_chpl)((call_tmp_chpl86)->_instance)) == nil) /* ZLINE: 101 ../lib/MNISTTester.chpl */
#line 101 "../lib/MNISTTester.chpl"
        {
#line 101 "../lib/MNISTTester.chpl"
          nilCheckHalt_chpl(&_str_literal_52_chpl, INT64(101), INT32(66));
#line 101 "../lib/MNISTTester.chpl"
        }
#line 101 "../lib/MNISTTester.chpl"
        qio_channel_unlock((&local_stdout_chpl)->_channel_internal);
#line 101 "../lib/MNISTTester.chpl"
        i_x_chpl15 = &origLocale_chpl;
#line 101 "../lib/MNISTTester.chpl"
        deinit_chpl62(i_x_chpl15);
#line 101 "../lib/MNISTTester.chpl"
        goto _endwrite_chpl;
#line 101 "../lib/MNISTTester.chpl"
      }
#line 101 "../lib/MNISTTester.chpl"
      error_chpl17 = NULL;
#line 101 "../lib/MNISTTester.chpl"
      _writeOne_chpl3(i_this_chpl3, (&args_chpl)->x10, &origLocale_chpl, &error_chpl17, INT64(101), INT32(66));
#line 101 "../lib/MNISTTester.chpl"
      if (error_chpl17 != nil) /* ZLINE: 101 ../lib/MNISTTester.chpl */
#line 101 "../lib/MNISTTester.chpl"
      {
#line 101 "../lib/MNISTTester.chpl"
        goto handler_chpl13;
#line 101 "../lib/MNISTTester.chpl"
      }
#line 101 "../lib/MNISTTester.chpl"
      handler_chpl13:;
#line 101 "../lib/MNISTTester.chpl"
      if (error_chpl17 != nil) /* ZLINE: 101 ../lib/MNISTTester.chpl */
#line 101 "../lib/MNISTTester.chpl"
      {
#line 101 "../lib/MNISTTester.chpl"
        error_chpl4 = error_chpl17;
#line 101 "../lib/MNISTTester.chpl"
        call_tmp_chpl87 = &((&local_stdout_chpl)->_home);
#line 101 "../lib/MNISTTester.chpl"
        if (((RootClass_chpl)((call_tmp_chpl87)->_instance)) == nil) /* ZLINE: 101 ../lib/MNISTTester.chpl */
#line 101 "../lib/MNISTTester.chpl"
        {
#line 101 "../lib/MNISTTester.chpl"
          nilCheckHalt_chpl(&_str_literal_52_chpl, INT64(101), INT32(66));
#line 101 "../lib/MNISTTester.chpl"
        }
#line 101 "../lib/MNISTTester.chpl"
        qio_channel_unlock((&local_stdout_chpl)->_channel_internal);
#line 101 "../lib/MNISTTester.chpl"
        i_x_chpl16 = &origLocale_chpl;
#line 101 "../lib/MNISTTester.chpl"
        deinit_chpl62(i_x_chpl16);
#line 101 "../lib/MNISTTester.chpl"
        goto _endwrite_chpl;
#line 101 "../lib/MNISTTester.chpl"
      }
#line 101 "../lib/MNISTTester.chpl"
      error_chpl18 = NULL;
#line 101 "../lib/MNISTTester.chpl"
      call_tmp_chpl88 = (&args_chpl)->x11;
#line 101 "../lib/MNISTTester.chpl"
      _writeOne_chpl(i_this_chpl3, call_tmp_chpl88, &origLocale_chpl, &error_chpl18, INT64(101), INT32(66));
#line 101 "../lib/MNISTTester.chpl"
      if (error_chpl18 != nil) /* ZLINE: 101 ../lib/MNISTTester.chpl */
#line 101 "../lib/MNISTTester.chpl"
      {
#line 101 "../lib/MNISTTester.chpl"
        goto handler_chpl14;
#line 101 "../lib/MNISTTester.chpl"
      }
#line 101 "../lib/MNISTTester.chpl"
      handler_chpl14:;
#line 101 "../lib/MNISTTester.chpl"
      if (error_chpl18 != nil) /* ZLINE: 101 ../lib/MNISTTester.chpl */
#line 101 "../lib/MNISTTester.chpl"
      {
#line 101 "../lib/MNISTTester.chpl"
        error_chpl4 = error_chpl18;
#line 101 "../lib/MNISTTester.chpl"
        call_tmp_chpl89 = &((&local_stdout_chpl)->_home);
#line 101 "../lib/MNISTTester.chpl"
        if (((RootClass_chpl)((call_tmp_chpl89)->_instance)) == nil) /* ZLINE: 101 ../lib/MNISTTester.chpl */
#line 101 "../lib/MNISTTester.chpl"
        {
#line 101 "../lib/MNISTTester.chpl"
          nilCheckHalt_chpl(&_str_literal_52_chpl, INT64(101), INT32(66));
#line 101 "../lib/MNISTTester.chpl"
        }
#line 101 "../lib/MNISTTester.chpl"
        qio_channel_unlock((&local_stdout_chpl)->_channel_internal);
#line 101 "../lib/MNISTTester.chpl"
        i_x_chpl17 = &origLocale_chpl;
#line 101 "../lib/MNISTTester.chpl"
        deinit_chpl62(i_x_chpl17);
#line 101 "../lib/MNISTTester.chpl"
        goto _endwrite_chpl;
#line 101 "../lib/MNISTTester.chpl"
      }
#line 101 "../lib/MNISTTester.chpl"
      call_tmp_chpl90 = &((&local_stdout_chpl)->_home);
#line 101 "../lib/MNISTTester.chpl"
      if (((RootClass_chpl)((call_tmp_chpl90)->_instance)) == nil) /* ZLINE: 101 ../lib/MNISTTester.chpl */
#line 101 "../lib/MNISTTester.chpl"
      {
#line 101 "../lib/MNISTTester.chpl"
        nilCheckHalt_chpl(&_str_literal_52_chpl, INT64(101), INT32(66));
#line 101 "../lib/MNISTTester.chpl"
      }
#line 101 "../lib/MNISTTester.chpl"
      qio_channel_unlock((&local_stdout_chpl)->_channel_internal);
#line 101 "../lib/MNISTTester.chpl"
      i_x_chpl18 = &origLocale_chpl;
#line 101 "../lib/MNISTTester.chpl"
      deinit_chpl62(i_x_chpl18);
#line 101 "../lib/MNISTTester.chpl"
      _endwrite_chpl:;
#line 101 "../lib/MNISTTester.chpl"
      if (error_chpl4 != nil) /* ZLINE: 101 ../lib/MNISTTester.chpl */
#line 101 "../lib/MNISTTester.chpl"
      {
#line 101 "../lib/MNISTTester.chpl"
        chpl_uncaught_error(error_chpl4, INT64(101), INT32(66));
#line 101 "../lib/MNISTTester.chpl"
      }
#line 102 "../lib/MNISTTester.chpl"
      error_chpl19 = NULL;
#line 102 "../lib/MNISTTester.chpl"
      flush_chpl(&local_stdout_chpl, &error_chpl19, INT64(102), INT32(66));
#line 102 "../lib/MNISTTester.chpl"
      if (error_chpl19 != nil) /* ZLINE: 102 ../lib/MNISTTester.chpl */
#line 102 "../lib/MNISTTester.chpl"
      {
#line 102 "../lib/MNISTTester.chpl"
        chpl_uncaught_error(error_chpl19, INT64(102), INT32(66));
#line 102 "../lib/MNISTTester.chpl"
      }
#line 102 "../lib/MNISTTester.chpl"
      chpl__autoDestroy32(&batch_chpl, INT64(102), INT32(66));
#line 102 "../lib/MNISTTester.chpl"
    }
#line 104 "../lib/MNISTTester.chpl"
    error_chpl20 = NULL;
#line 104 "../lib/MNISTTester.chpl"
    i_this_chpl6 = &stdout_chpl;
#line 104 "../lib/MNISTTester.chpl"
    inlineImm_chpl8 = _str_literal_610_chpl;
#line 104 "../lib/MNISTTester.chpl"
    i__e0_args_chpl2 = &inlineImm_chpl8;
#line 104 "../lib/MNISTTester.chpl"
    call_tmp_chpl91 = &((&local_stdout_chpl)->_readWriteThisFromLocale);
#line 104 "../lib/MNISTTester.chpl"
    init_ASSIGN__chpl(&ret_chpl11, call_tmp_chpl91);
#line 104 "../lib/MNISTTester.chpl"
    call_tmp_chpl92 = chpl___EQUALS_(&ret_chpl11, &local_nilLocale_chpl, INT64(104), INT32(66));
#line 104 "../lib/MNISTTester.chpl"
    if (call_tmp_chpl92) /* ZLINE: 104 ../lib/MNISTTester.chpl */
#line 104 "../lib/MNISTTester.chpl"
    {
#line 104 "../lib/MNISTTester.chpl"
      call_tmp_chpl95 = get_chpl_nodeID();
#line 104 "../lib/MNISTTester.chpl"
      call_tmp_chpl96 = chpl_rt_buildLocaleID(call_tmp_chpl95, c_sublocid_any);
#line 104 "../lib/MNISTTester.chpl"
      call_tmp_chpl94 = call_tmp_chpl96;
#line 104 "../lib/MNISTTester.chpl"
      chpl_localeID_to_locale(&call_tmp_chpl94, &ret_tmp_chpl32, INT64(104), INT32(66));
#line 104 "../lib/MNISTTester.chpl"
      call_tmp_chpl97 = ret_tmp_chpl32;
#line 104 "../lib/MNISTTester.chpl"
      call_tmp_chpl93 = call_tmp_chpl97;
#line 104 "../lib/MNISTTester.chpl"
      chpl___ASSIGN_9(&ret_chpl11, &call_tmp_chpl93);
#line 104 "../lib/MNISTTester.chpl"
      i_x_chpl19 = &call_tmp_chpl93;
#line 104 "../lib/MNISTTester.chpl"
      deinit_chpl62(i_x_chpl19);
#line 104 "../lib/MNISTTester.chpl"
    }
#line 104 "../lib/MNISTTester.chpl"
    ret_chpl10 = ret_chpl11;
#line 104 "../lib/MNISTTester.chpl"
    origLocale_chpl2 = ret_chpl10;
#line 104 "../lib/MNISTTester.chpl"
    call_tmp_chpl98 = &((&local_stdout_chpl)->_home);
#line 104 "../lib/MNISTTester.chpl"
    if (((RootClass_chpl)((call_tmp_chpl98)->_instance)) == nil) /* ZLINE: 104 ../lib/MNISTTester.chpl */
#line 104 "../lib/MNISTTester.chpl"
    {
#line 104 "../lib/MNISTTester.chpl"
      nilCheckHalt_chpl(&_str_literal_52_chpl, INT64(104), INT32(66));
#line 104 "../lib/MNISTTester.chpl"
    }
#line 104 "../lib/MNISTTester.chpl"
    error_chpl21 = NULL;
#line 104 "../lib/MNISTTester.chpl"
    qio_int_to_err(((int32_t)(INT64(0))));
#line 104 "../lib/MNISTTester.chpl"
    if ((&local_stdout_chpl)->_channel_internal == nil) /* ZLINE: 104 ../lib/MNISTTester.chpl */
#line 104 "../lib/MNISTTester.chpl"
    {
#line 104 "../lib/MNISTTester.chpl"
      createSystemError_chpl2(((int64_t)(EINVAL)), &_str_literal_1920_chpl, &ret_tmp_chpl33, INT64(104), INT32(66));
#line 104 "../lib/MNISTTester.chpl"
      call_tmp_chpl99 = ret_tmp_chpl33;
#line 104 "../lib/MNISTTester.chpl"
      i_x_chpl20 = &call_tmp_chpl99;
#line 104 "../lib/MNISTTester.chpl"
      i__retArg_chpl2 = &ret_tmp_chpl34;
#line 104 "../lib/MNISTTester.chpl"
      _formal_tmp_x_chpl2 = *(i_x_chpl20);
#line 104 "../lib/MNISTTester.chpl"
      i_this_chpl7 = &_formal_tmp_x_chpl2;
#line 104 "../lib/MNISTTester.chpl"
      call_tmp_chpl101 = ((Error_chpl)((i_this_chpl7)->chpl_p));
#line 104 "../lib/MNISTTester.chpl"
      i_this_chpl8 = &_formal_tmp_x_chpl2;
#line 104 "../lib/MNISTTester.chpl"
      (i_this_chpl8)->chpl_p = ((SystemError_chpl)(nil));
#line 104 "../lib/MNISTTester.chpl"
      if (((RootClass_chpl)(call_tmp_chpl101)) == nil) /* ZLINE: 104 ../lib/MNISTTester.chpl */
#line 104 "../lib/MNISTTester.chpl"
      {
#line 104 "../lib/MNISTTester.chpl"
        nilCheckHalt_chpl(&_str_literal_52_chpl, INT64(104), INT32(66));
#line 104 "../lib/MNISTTester.chpl"
      }
#line 104 "../lib/MNISTTester.chpl"
      init_chpl188(&initTemp_chpl3, call_tmp_chpl101);
#line 104 "../lib/MNISTTester.chpl"
      call_tmp_chpl102 = initTemp_chpl3;
#line 104 "../lib/MNISTTester.chpl"
      ret_chpl12 = call_tmp_chpl102;
#line 104 "../lib/MNISTTester.chpl"
      chpl__autoDestroy54(&_formal_tmp_x_chpl2, INT64(104), INT32(66));
#line 104 "../lib/MNISTTester.chpl"
      *(i__retArg_chpl2) = ret_chpl12;
#line 104 "../lib/MNISTTester.chpl"
      coerce_tmp_chpl16 = ret_tmp_chpl34;
#line 104 "../lib/MNISTTester.chpl"
      _formal_tmp_in_err_chpl2 = coerce_tmp_chpl16;
#line 104 "../lib/MNISTTester.chpl"
      call_tmp_chpl100 = chpl_fix_thrown_error(&_formal_tmp_in_err_chpl2, INT64(104), INT32(66));
#line 104 "../lib/MNISTTester.chpl"
      error_chpl21 = call_tmp_chpl100;
#line 104 "../lib/MNISTTester.chpl"
      goto _endlock_chpl2;
#line 104 "../lib/MNISTTester.chpl"
    }
#line 104 "../lib/MNISTTester.chpl"
    call_tmp_chpl103 = &((&local_stdout_chpl)->_home);
#line 104 "../lib/MNISTTester.chpl"
    if (((RootClass_chpl)((call_tmp_chpl103)->_instance)) == nil) /* ZLINE: 104 ../lib/MNISTTester.chpl */
#line 104 "../lib/MNISTTester.chpl"
    {
#line 104 "../lib/MNISTTester.chpl"
      nilCheckHalt_chpl(&_str_literal_52_chpl, INT64(104), INT32(66));
#line 104 "../lib/MNISTTester.chpl"
    }
#line 104 "../lib/MNISTTester.chpl"
    call_tmp_chpl104 = qio_channel_lock((&local_stdout_chpl)->_channel_internal);
#line 104 "../lib/MNISTTester.chpl"
    call_tmp_chpl105 = qio_err_iserr(call_tmp_chpl104);
#line 104 "../lib/MNISTTester.chpl"
    if (call_tmp_chpl105 != INT32(0)) /* ZLINE: 104 ../lib/MNISTTester.chpl */
#line 104 "../lib/MNISTTester.chpl"
    {
#line 104 "../lib/MNISTTester.chpl"
      error_chpl22 = NULL;
#line 104 "../lib/MNISTTester.chpl"
      _ch_ioerror_chpl5(i_this_chpl6, call_tmp_chpl104, &_str_literal_1919_chpl, &error_chpl22, INT64(104), INT32(66));
#line 104 "../lib/MNISTTester.chpl"
      if (error_chpl22 != nil) /* ZLINE: 104 ../lib/MNISTTester.chpl */
#line 104 "../lib/MNISTTester.chpl"
      {
#line 104 "../lib/MNISTTester.chpl"
        goto handler_chpl15;
#line 104 "../lib/MNISTTester.chpl"
      }
#line 104 "../lib/MNISTTester.chpl"
      handler_chpl15:;
#line 104 "../lib/MNISTTester.chpl"
      if (error_chpl22 != nil) /* ZLINE: 104 ../lib/MNISTTester.chpl */
#line 104 "../lib/MNISTTester.chpl"
      {
#line 104 "../lib/MNISTTester.chpl"
        error_chpl21 = error_chpl22;
#line 104 "../lib/MNISTTester.chpl"
        goto _endlock_chpl2;
#line 104 "../lib/MNISTTester.chpl"
      }
#line 104 "../lib/MNISTTester.chpl"
    }
#line 104 "../lib/MNISTTester.chpl"
    _endlock_chpl2:;
#line 104 "../lib/MNISTTester.chpl"
    if (error_chpl21 != nil) /* ZLINE: 104 ../lib/MNISTTester.chpl */
#line 104 "../lib/MNISTTester.chpl"
    {
#line 104 "../lib/MNISTTester.chpl"
      goto handler_chpl16;
#line 104 "../lib/MNISTTester.chpl"
    }
#line 104 "../lib/MNISTTester.chpl"
    handler_chpl16:;
#line 104 "../lib/MNISTTester.chpl"
    if (error_chpl21 != nil) /* ZLINE: 104 ../lib/MNISTTester.chpl */
#line 104 "../lib/MNISTTester.chpl"
    {
#line 104 "../lib/MNISTTester.chpl"
      error_chpl20 = error_chpl21;
#line 104 "../lib/MNISTTester.chpl"
      i_x_chpl21 = &origLocale_chpl2;
#line 104 "../lib/MNISTTester.chpl"
      deinit_chpl62(i_x_chpl21);
#line 104 "../lib/MNISTTester.chpl"
      goto _endwrite_chpl2;
#line 104 "../lib/MNISTTester.chpl"
    }
#line 104 "../lib/MNISTTester.chpl"
    error_chpl23 = NULL;
#line 104 "../lib/MNISTTester.chpl"
    _writeOne_chpl(i_this_chpl6, i__e0_args_chpl2, &origLocale_chpl2, &error_chpl23, INT64(104), INT32(66));
#line 104 "../lib/MNISTTester.chpl"
    if (error_chpl23 != nil) /* ZLINE: 104 ../lib/MNISTTester.chpl */
#line 104 "../lib/MNISTTester.chpl"
    {
#line 104 "../lib/MNISTTester.chpl"
      goto handler_chpl17;
#line 104 "../lib/MNISTTester.chpl"
    }
#line 104 "../lib/MNISTTester.chpl"
    handler_chpl17:;
#line 104 "../lib/MNISTTester.chpl"
    if (error_chpl23 != nil) /* ZLINE: 104 ../lib/MNISTTester.chpl */
#line 104 "../lib/MNISTTester.chpl"
    {
#line 104 "../lib/MNISTTester.chpl"
      error_chpl20 = error_chpl23;
#line 104 "../lib/MNISTTester.chpl"
      call_tmp_chpl106 = &((&local_stdout_chpl)->_home);
#line 104 "../lib/MNISTTester.chpl"
      if (((RootClass_chpl)((call_tmp_chpl106)->_instance)) == nil) /* ZLINE: 104 ../lib/MNISTTester.chpl */
#line 104 "../lib/MNISTTester.chpl"
      {
#line 104 "../lib/MNISTTester.chpl"
        nilCheckHalt_chpl(&_str_literal_52_chpl, INT64(104), INT32(66));
#line 104 "../lib/MNISTTester.chpl"
      }
#line 104 "../lib/MNISTTester.chpl"
      qio_channel_unlock((&local_stdout_chpl)->_channel_internal);
#line 104 "../lib/MNISTTester.chpl"
      i_x_chpl22 = &origLocale_chpl2;
#line 104 "../lib/MNISTTester.chpl"
      deinit_chpl62(i_x_chpl22);
#line 104 "../lib/MNISTTester.chpl"
      goto _endwrite_chpl2;
#line 104 "../lib/MNISTTester.chpl"
    }
#line 104 "../lib/MNISTTester.chpl"
    call_tmp_chpl107 = &((&local_stdout_chpl)->_home);
#line 104 "../lib/MNISTTester.chpl"
    if (((RootClass_chpl)((call_tmp_chpl107)->_instance)) == nil) /* ZLINE: 104 ../lib/MNISTTester.chpl */
#line 104 "../lib/MNISTTester.chpl"
    {
#line 104 "../lib/MNISTTester.chpl"
      nilCheckHalt_chpl(&_str_literal_52_chpl, INT64(104), INT32(66));
#line 104 "../lib/MNISTTester.chpl"
    }
#line 104 "../lib/MNISTTester.chpl"
    qio_channel_unlock((&local_stdout_chpl)->_channel_internal);
#line 104 "../lib/MNISTTester.chpl"
    i_x_chpl23 = &origLocale_chpl2;
#line 104 "../lib/MNISTTester.chpl"
    deinit_chpl62(i_x_chpl23);
#line 104 "../lib/MNISTTester.chpl"
    _endwrite_chpl2:;
#line 104 "../lib/MNISTTester.chpl"
    if (error_chpl20 != nil) /* ZLINE: 104 ../lib/MNISTTester.chpl */
#line 104 "../lib/MNISTTester.chpl"
    {
#line 104 "../lib/MNISTTester.chpl"
      chpl_uncaught_error(error_chpl20, INT64(104), INT32(66));
#line 104 "../lib/MNISTTester.chpl"
    }
#line 105 "../lib/MNISTTester.chpl"
    error_chpl24 = NULL;
#line 105 "../lib/MNISTTester.chpl"
    flush_chpl(&local_stdout_chpl, &error_chpl24, INT64(105), INT32(66));
#line 105 "../lib/MNISTTester.chpl"
    if (error_chpl24 != nil) /* ZLINE: 105 ../lib/MNISTTester.chpl */
#line 105 "../lib/MNISTTester.chpl"
    {
#line 105 "../lib/MNISTTester.chpl"
      chpl_uncaught_error(error_chpl24, INT64(105), INT32(66));
#line 105 "../lib/MNISTTester.chpl"
    }
#line 107 "../lib/MNISTTester.chpl"
    writeln_chpl4(&_str_literal_1751_chpl, INT64(107), INT32(66));
#line 112 "../lib/MNISTTester.chpl"
    init_chpl105(&l_chpl);
#line 112 "../lib/MNISTTester.chpl"
    ret_chpl13 = l_chpl;
#line 112 "../lib/MNISTTester.chpl"
    default_arg_l_chpl = ret_chpl13;
#line 112 "../lib/MNISTTester.chpl"
    new_temp_chpl2 = _new_chpl23(&default_arg_l_chpl, REAL64(0x0p+0), INT64(112), INT32(66));
#line 112 "../lib/MNISTTester.chpl"
    globalRP_loss_chpl = new_temp_chpl2;
#line 112 "../lib/MNISTTester.chpl"
    coerce_tmp_chpl17 = globalRP_loss_chpl;
#line 112 "../lib/MNISTTester.chpl"
    chpl_check_nil(coerce_tmp_chpl17, INT64(112), INT32(66));
#line 112 "../lib/MNISTTester.chpl"
    call_tmp_chpl108 = &((coerce_tmp_chpl17)->value);
#line 112 "../lib/MNISTTester.chpl"
    *(call_tmp_chpl108) += REAL64(0x0p+0);
#line 112 "../lib/MNISTTester.chpl"
    globalAS_loss_chpl = REAL64(0x0p+0);
#line 112 "../lib/MNISTTester.chpl"
    init_chpl105(&l_chpl2);
#line 112 "../lib/MNISTTester.chpl"
    ret_chpl14 = l_chpl2;
#line 112 "../lib/MNISTTester.chpl"
    default_arg_l_chpl2 = ret_chpl14;
#line 112 "../lib/MNISTTester.chpl"
    new_temp_chpl3 = _new_chpl21(&default_arg_l_chpl2, INT64(0), INT64(112), INT32(66));
#line 112 "../lib/MNISTTester.chpl"
    globalRP_numCorrect_chpl = new_temp_chpl3;
#line 112 "../lib/MNISTTester.chpl"
    coerce_tmp_chpl18 = globalRP_numCorrect_chpl;
#line 112 "../lib/MNISTTester.chpl"
    chpl_check_nil(coerce_tmp_chpl18, INT64(112), INT32(66));
#line 112 "../lib/MNISTTester.chpl"
    call_tmp_chpl109 = &((coerce_tmp_chpl18)->value);
#line 112 "../lib/MNISTTester.chpl"
    *(call_tmp_chpl109) += INT64(0);
#line 112 "../lib/MNISTTester.chpl"
    globalAS_numCorrect_chpl = INT64(0);
#line 112 "../lib/MNISTTester.chpl"
    coerce_tmp_chpl19 = (&testingData_chpl)->_instance;
#line 112 "../lib/MNISTTester.chpl"
    tasksPerLocale_chpl2 = local_dataParTasksPerLocale_chpl;
#line 112 "../lib/MNISTTester.chpl"
    ignoreRunning_chpl2 = local_dataParIgnoreRunningTasks_chpl;
#line 112 "../lib/MNISTTester.chpl"
    minIndicesPerTask_chpl2 = local_dataParMinGranularity_chpl;
#line 112 "../lib/MNISTTester.chpl"
    error_chpl25 = NULL;
#line 112 "../lib/MNISTTester.chpl"
    chpl_check_nil(coerce_tmp_chpl19, INT64(112), INT32(66));
#line 112 "../lib/MNISTTester.chpl"
    coerce_tmp_chpl20 = (coerce_tmp_chpl19)->dom;
#line 112 "../lib/MNISTTester.chpl"
    createTuple_chpl(INT64(0), &ret_tmp_chpl35);
#line 112 "../lib/MNISTTester.chpl"
    error_chpl26 = NULL;
#line 112 "../lib/MNISTTester.chpl"
    if (tasksPerLocale_chpl2 == INT64(0)) /* ZLINE: 112 ../lib/MNISTTester.chpl */
#line 112 "../lib/MNISTTester.chpl"
    {
#line 112 "../lib/MNISTTester.chpl"
      call_tmp_chpl112 = get_chpl_nodeID();
#line 112 "../lib/MNISTTester.chpl"
      call_tmp_chpl113 = chpl_rt_buildLocaleID(call_tmp_chpl112, c_sublocid_any);
#line 112 "../lib/MNISTTester.chpl"
      call_tmp_chpl111 = call_tmp_chpl113;
#line 112 "../lib/MNISTTester.chpl"
      chpl_localeID_to_locale(&call_tmp_chpl111, &ret_tmp_chpl36, INT64(112), INT32(66));
#line 112 "../lib/MNISTTester.chpl"
      call_tmp_chpl114 = ret_tmp_chpl36;
#line 112 "../lib/MNISTTester.chpl"
      call_tmp_chpl110 = call_tmp_chpl114;
#line 112 "../lib/MNISTTester.chpl"
      coerce_tmp_chpl21 = (&call_tmp_chpl110)->_instance;
#line 112 "../lib/MNISTTester.chpl"
      if (((RootClass_chpl)(coerce_tmp_chpl21)) == nil) /* ZLINE: 112 ../lib/MNISTTester.chpl */
#line 112 "../lib/MNISTTester.chpl"
      {
#line 112 "../lib/MNISTTester.chpl"
        nilCheckHalt_chpl(&_str_literal_52_chpl, INT64(112), INT32(66));
#line 112 "../lib/MNISTTester.chpl"
      }
#line 112 "../lib/MNISTTester.chpl"
      coerce_tmp_chpl22 = coerce_tmp_chpl21;
#line 112 "../lib/MNISTTester.chpl"
      chpl_check_nil(coerce_tmp_chpl22, INT64(112), INT32(66));
#line 112 "../lib/MNISTTester.chpl"
      ret_chpl15 = (coerce_tmp_chpl22)->maxTaskPar;
#line 112 "../lib/MNISTTester.chpl"
      tmp_chpl5 = ret_chpl15;
#line 112 "../lib/MNISTTester.chpl"
      i_x_chpl24 = &call_tmp_chpl110;
#line 112 "../lib/MNISTTester.chpl"
      deinit_chpl62(i_x_chpl24);
#line 112 "../lib/MNISTTester.chpl"
    } else /* ZLINE: 112 ../lib/MNISTTester.chpl */
#line 112 "../lib/MNISTTester.chpl"
    {
#line 112 "../lib/MNISTTester.chpl"
      tmp_chpl5 = tasksPerLocale_chpl2;
#line 112 "../lib/MNISTTester.chpl"
    }
#line 112 "../lib/MNISTTester.chpl"
    if (chpl_task_getSerial()) /* ZLINE: 112 ../lib/MNISTTester.chpl */
#line 112 "../lib/MNISTTester.chpl"
    {
#line 112 "../lib/MNISTTester.chpl"
      tmp_x0_chpl4 = INT64(1);
#line 112 "../lib/MNISTTester.chpl"
      tmp_x1_chpl4 = INT64(0);
#line 112 "../lib/MNISTTester.chpl"
    } else /* ZLINE: 112 ../lib/MNISTTester.chpl */
#line 112 "../lib/MNISTTester.chpl"
    {
#line 112 "../lib/MNISTTester.chpl"
      chpl_check_nil(coerce_tmp_chpl20, INT64(112), INT32(66));
#line 112 "../lib/MNISTTester.chpl"
      call_tmp_chpl115 = &((coerce_tmp_chpl20)->ranges);
#line 112 "../lib/MNISTTester.chpl"
      _computeChunkStuff_chpl(tmp_chpl5, ignoreRunning_chpl2, minIndicesPerTask_chpl2, call_tmp_chpl115, &ret_tmp_chpl37, INT64(112), INT32(66));
#line 112 "../lib/MNISTTester.chpl"
      tmp_x0_chpl4 = *(ret_tmp_chpl37 + INT64(0));
#line 112 "../lib/MNISTTester.chpl"
      tmp_x1_chpl4 = *(ret_tmp_chpl37 + INT64(1));
#line 112 "../lib/MNISTTester.chpl"
    }
#line 112 "../lib/MNISTTester.chpl"
    numChunks_chpl2 = tmp_x0_chpl4;
#line 112 "../lib/MNISTTester.chpl"
    parDim_chpl2 = tmp_x1_chpl4;
#line 112 "../lib/MNISTTester.chpl"
    chpl_build_low_bounded_range2(INT64(0), &ret_tmp_chpl38);
#line 112 "../lib/MNISTTester.chpl"
    call_tmp_chpl116 = ret_tmp_chpl38;
#line 112 "../lib/MNISTTester.chpl"
    chpl___POUND_3(call_tmp_chpl116, tmp_x0_chpl4, &ret_tmp_chpl39, INT64(112), INT32(66));
#line 112 "../lib/MNISTTester.chpl"
    tmpIter_chpl2 = ret_tmp_chpl39;
#line 112 "../lib/MNISTTester.chpl"
    new_temp_chpl4 = _new_chpl17(INT64(112), INT32(66));
#line 112 "../lib/MNISTTester.chpl"
    _coforallCount_chpl2 = new_temp_chpl4;
#line 112 "../lib/MNISTTester.chpl"
    numTasks_chpl2 = chpl_boundedCoforallSize(tmpIter_chpl2, INT64(112), INT32(66));
#line 112 "../lib/MNISTTester.chpl"
    _upEndCount_chpl(_coforallCount_chpl2, numTasks_chpl2, INT64(112), INT32(66));
#line 112 "../lib/MNISTTester.chpl"
    chpl_resetTaskSpawn(numTasks_chpl2, INT64(112), INT32(66));
#line 112 "../lib/MNISTTester.chpl"
    _ic__F0_this_chpl2 = tmpIter_chpl2;
#line 112 "../lib/MNISTTester.chpl"
    checkIfIterWillOverflow_chpl(tmpIter_chpl2, UINT8(true), INT64(112), INT32(66));
#line 112 "../lib/MNISTTester.chpl"
    i_chpl4 = INT64(0);
#line 112 "../lib/MNISTTester.chpl"
    this_chpl34 = _ic__F0_this_chpl2;
#line 112 "../lib/MNISTTester.chpl"
    this_chpl35 = _ic__F0_this_chpl2;
#line 112 "../lib/MNISTTester.chpl"
    end_chpl2 = (&this_chpl35)->_high;
#line 112 "../lib/MNISTTester.chpl"
    for (i_chpl4 = (&this_chpl34)->_low; ((i_chpl4 <= end_chpl2)); i_chpl4 += INT64(1)) {
#line 112 "../lib/MNISTTester.chpl"
      _args_forcoforall_fn_chpl2 = ((_class_localscoforall_fn_chpl115)(&chpl_macro_tmp_125));
#line 112 "../lib/MNISTTester.chpl"
      chpl_check_nil(_args_forcoforall_fn_chpl2, INT64(112), INT32(66));
#line 112 "../lib/MNISTTester.chpl"
      (_args_forcoforall_fn_chpl2)->_0_numChunks = numChunks_chpl2;
#line 112 "../lib/MNISTTester.chpl"
      chpl_check_nil(_args_forcoforall_fn_chpl2, INT64(112), INT32(66));
#line 112 "../lib/MNISTTester.chpl"
      (_args_forcoforall_fn_chpl2)->_1_parDim = parDim_chpl2;
#line 112 "../lib/MNISTTester.chpl"
      chpl_check_nil(_args_forcoforall_fn_chpl2, INT64(112), INT32(66));
#line 112 "../lib/MNISTTester.chpl"
      (_args_forcoforall_fn_chpl2)->_2_coerce_tmp = coerce_tmp_chpl20;
#line 112 "../lib/MNISTTester.chpl"
      chpl_check_nil(_args_forcoforall_fn_chpl2, INT64(112), INT32(66));
#line 112 "../lib/MNISTTester.chpl"
      (_args_forcoforall_fn_chpl2)->_3__coforallCount = _coforallCount_chpl2;
#line 112 "../lib/MNISTTester.chpl"
      chpl_check_nil(_args_forcoforall_fn_chpl2, INT64(112), INT32(66));
#line 112 "../lib/MNISTTester.chpl"
      (_args_forcoforall_fn_chpl2)->_4_chunk = i_chpl4;
#line 112 "../lib/MNISTTester.chpl"
      chpl_check_nil(_args_forcoforall_fn_chpl2, INT64(112), INT32(66));
#line 112 "../lib/MNISTTester.chpl"
      (_args_forcoforall_fn_chpl2)->_6_coerce_tmp = coerce_tmp_chpl19;
#line 112 "../lib/MNISTTester.chpl"
      chpl_check_nil(_args_forcoforall_fn_chpl2, INT64(112), INT32(66));
#line 112 "../lib/MNISTTester.chpl"
      (_args_forcoforall_fn_chpl2)->_7_globalRP_loss = globalRP_loss_chpl;
#line 112 "../lib/MNISTTester.chpl"
      chpl_check_nil(_args_forcoforall_fn_chpl2, INT64(112), INT32(66));
#line 112 "../lib/MNISTTester.chpl"
      (_args_forcoforall_fn_chpl2)->_8_globalRP_numCorrect = globalRP_numCorrect_chpl;
#line 112 "../lib/MNISTTester.chpl"
      tmp_chpl6 = network_chpl;
#line 112 "../lib/MNISTTester.chpl"
      chpl_check_nil(_args_forcoforall_fn_chpl2, INT64(112), INT32(66));
#line 112 "../lib/MNISTTester.chpl"
      (_args_forcoforall_fn_chpl2)->_9_network = tmp_chpl6;
#line 112 "../lib/MNISTTester.chpl"
      _args_vforcoforall_fn_chpl2 = ((void*)(_args_forcoforall_fn_chpl2));
#line 112 "../lib/MNISTTester.chpl"
      /*** wrapcoforall_fn_chpl115 ***/ chpl_taskAddCoStmt(chpl_task_getRequestedSubloc(), INT64(114), ((chpl_task_bundle_p)(_args_vforcoforall_fn_chpl2)), sizeof(chpl__class_localscoforall_fn_chpl115_object), INT64(112), INT32(66));
#line 112 "../lib/MNISTTester.chpl"
    }
#line 112 "../lib/MNISTTester.chpl"
    error_chpl27 = NULL;
#line 112 "../lib/MNISTTester.chpl"
    _waitEndCount_chpl2(_coforallCount_chpl2, numTasks_chpl2, &error_chpl27, INT64(112), INT32(66));
#line 112 "../lib/MNISTTester.chpl"
    if (error_chpl27 != nil) /* ZLINE: 112 ../lib/MNISTTester.chpl */
#line 112 "../lib/MNISTTester.chpl"
    {
#line 112 "../lib/MNISTTester.chpl"
      chpl__delete72(_coforallCount_chpl2, INT64(112), INT32(66));
#line 112 "../lib/MNISTTester.chpl"
      error_chpl26 = error_chpl27;
#line 112 "../lib/MNISTTester.chpl"
      goto forall_IBB_break_label_chpl3;
#line 112 "../lib/MNISTTester.chpl"
    }
#line 112 "../lib/MNISTTester.chpl"
    chpl__delete72(_coforallCount_chpl2, INT64(112), INT32(66));
#line 112 "../lib/MNISTTester.chpl"
    chpl_after_forall_fence();
#line 112 "../lib/MNISTTester.chpl"
    forall_IBB_break_label_chpl3:;
#line 112 "../lib/MNISTTester.chpl"
    if (error_chpl26 != nil) /* ZLINE: 112 ../lib/MNISTTester.chpl */
#line 112 "../lib/MNISTTester.chpl"
    {
#line 112 "../lib/MNISTTester.chpl"
      error_chpl25 = error_chpl26;
#line 112 "../lib/MNISTTester.chpl"
      goto forall_IBB_break_label_chpl4;
#line 112 "../lib/MNISTTester.chpl"
    }
#line 112 "../lib/MNISTTester.chpl"
    chpl_after_forall_fence();
#line 112 "../lib/MNISTTester.chpl"
    forall_IBB_break_label_chpl4:;
#line 112 "../lib/MNISTTester.chpl"
    if (error_chpl25 != nil) /* ZLINE: 112 ../lib/MNISTTester.chpl */
#line 112 "../lib/MNISTTester.chpl"
    {
#line 112 "../lib/MNISTTester.chpl"
      chpl_propagate_error(error_chpl25, INT64(112), INT32(66));
#line 112 "../lib/MNISTTester.chpl"
    }
#line 112 "../lib/MNISTTester.chpl"
    chpl_after_forall_fence();
#line 112 "../lib/MNISTTester.chpl"
    coerce_tmp_chpl23 = globalRP_numCorrect_chpl;
#line 112 "../lib/MNISTTester.chpl"
    chpl_check_nil(coerce_tmp_chpl23, INT64(112), INT32(66));
#line 112 "../lib/MNISTTester.chpl"
    call_tmp_chpl117 = &((coerce_tmp_chpl23)->value);
#line 112 "../lib/MNISTTester.chpl"
    *(call_tmp_chpl117) += globalAS_numCorrect_chpl;
#line 112 "../lib/MNISTTester.chpl"
    coerce_tmp_chpl24 = globalRP_numCorrect_chpl;
#line 112 "../lib/MNISTTester.chpl"
    chpl_check_nil(coerce_tmp_chpl24, INT64(112), INT32(66));
#line 112 "../lib/MNISTTester.chpl"
    ret_chpl16 = (coerce_tmp_chpl24)->value;
#line 112 "../lib/MNISTTester.chpl"
    chpl__delete16(globalRP_numCorrect_chpl, INT64(112), INT32(66));
#line 112 "../lib/MNISTTester.chpl"
    coerce_tmp_chpl25 = globalRP_loss_chpl;
#line 112 "../lib/MNISTTester.chpl"
    chpl_check_nil(coerce_tmp_chpl25, INT64(112), INT32(66));
#line 112 "../lib/MNISTTester.chpl"
    call_tmp_chpl118 = &((coerce_tmp_chpl25)->value);
#line 112 "../lib/MNISTTester.chpl"
    *(call_tmp_chpl118) += globalAS_loss_chpl;
#line 112 "../lib/MNISTTester.chpl"
    coerce_tmp_chpl26 = globalRP_loss_chpl;
#line 112 "../lib/MNISTTester.chpl"
    chpl_check_nil(coerce_tmp_chpl26, INT64(112), INT32(66));
#line 112 "../lib/MNISTTester.chpl"
    ret_chpl17 = (coerce_tmp_chpl26)->value;
#line 112 "../lib/MNISTTester.chpl"
    chpl__delete33(globalRP_loss_chpl, INT64(112), INT32(66));
#line 118 "../lib/MNISTTester.chpl"
    call_tmp_chpl119 = (epoch_chpl + INT64(1));
#line 118 "../lib/MNISTTester.chpl"
    call_tmp_chpl120 = size_chpl2(&testingData_chpl, INT64(118), INT32(66));
#line 118 "../lib/MNISTTester.chpl"
    call_tmp_chpl121 = (ret_chpl17 / ((_real64)(call_tmp_chpl120)));
#line 118 "../lib/MNISTTester.chpl"
    call_tmp_chpl122 = size_chpl2(&testingData_chpl, INT64(118), INT32(66));
#line 118 "../lib/MNISTTester.chpl"
    writeln_chpl(&_str_literal_1752_chpl, call_tmp_chpl119, &_str_literal_1753_chpl, call_tmp_chpl121, &_str_literal_1754_chpl, ret_chpl16, &_str_literal_1643_chpl, call_tmp_chpl122, INT64(118), INT32(66));
#line 120 "../lib/MNISTTester.chpl"
    error_chpl28 = NULL;
#line 120 "../lib/MNISTTester.chpl"
    save_chpl(network_chpl, savePath_chpl, &error_chpl28);
#line 120 "../lib/MNISTTester.chpl"
    if (error_chpl28 != nil) /* ZLINE: 120 ../lib/MNISTTester.chpl */
#line 120 "../lib/MNISTTester.chpl"
    {
#line 120 "../lib/MNISTTester.chpl"
      chpl_uncaught_error(error_chpl28, INT64(120), INT32(66));
#line 120 "../lib/MNISTTester.chpl"
    }
#line 120 "../lib/MNISTTester.chpl"
  }
#line 60 "../lib/MNISTTester.chpl"
  chpl__autoDestroy32(&testingData_chpl, INT64(60), INT32(66));
#line 60 "../lib/MNISTTester.chpl"
  chpl__autoDestroy32(&trainingData_chpl, INT64(60), INT32(66));
#line 60 "../lib/MNISTTester.chpl"
  chpl__autoDestroy32(&labeledImages_chpl, INT64(60), INT32(66));
#line 60 "../lib/MNISTTester.chpl"
  chpl__autoDestroy13(&images_chpl, INT64(60), INT32(66));
#line 60 "../lib/MNISTTester.chpl"
  chpl__autoDestroy27(&labelVectors_chpl, INT64(60), INT32(66));
#line 60 "../lib/MNISTTester.chpl"
  chpl__autoDestroy24(&labels_chpl, INT64(60), INT32(66));
#line 60 "../lib/MNISTTester.chpl"
  _field_destructor_tmp__chpl5 = tmp_x1_chpl2;
#line 60 "../lib/MNISTTester.chpl"
  chpl__autoDestroy27(&_field_destructor_tmp__chpl5, INT64(60), INT32(66));
#line 60 "../lib/MNISTTester.chpl"
  _field_destructor_tmp__chpl6 = tmp_x0_chpl2;
#line 60 "../lib/MNISTTester.chpl"
  chpl__autoDestroy24(&_field_destructor_tmp__chpl6, INT64(60), INT32(66));
#line 60 "../lib/MNISTTester.chpl"
  chpl__autoDestroy23(&imageRawData_chpl, INT64(60), INT32(66));
#line 60 "../lib/MNISTTester.chpl"
  return;
#line 60 "../lib/MNISTTester.chpl"
}

#line 75 "../lib/MNISTTester.chpl"
/* MNISTTester.chpl:75 */
#line 75 "../lib/MNISTTester.chpl"
static void chpl__forallexpr13(_array_DefaultRectangularArr_1_int64_t_one__array_DefaultRectangularArr_2_int64_t_one__real64_int64_t_int64_t_chpl * iterExpr_chpl,
#line 75 "../lib/MNISTTester.chpl"
                               _ir_chpl__loopexpr_iter13_chpl * _retArg_chpl) {
#line 75 "../lib/MNISTTester.chpl"
  _ir_chpl__loopexpr_iter13_chpl ret_tmp_chpl;
#line 75 "../lib/MNISTTester.chpl"
  DefaultRectangularDom_1_int64_t_one_chpl _ir__shape__chpl = NULL;
#line 75 "../lib/MNISTTester.chpl"
  _array_DefaultRectangularArr_1_int64_t_one__array_DefaultRectangularArr_2_int64_t_one__real64_int64_t_int64_t_chpl _ir_F0_iterator_chpl;
#line 75 "../lib/MNISTTester.chpl"
  _ir_F0_iterator_chpl = *(iterExpr_chpl);
#line 75 "../lib/MNISTTester.chpl"
  (&ret_tmp_chpl)->_shape_ = _ir__shape__chpl;
#line 75 "../lib/MNISTTester.chpl"
  (&ret_tmp_chpl)->F0_iterator = _ir_F0_iterator_chpl;
#line 75 "../lib/MNISTTester.chpl"
  *(_retArg_chpl) = ret_tmp_chpl;
#line 75 "../lib/MNISTTester.chpl"
  return;
#line 75 "../lib/MNISTTester.chpl"
}

#line 76 "../lib/MNISTTester.chpl"
/* MNISTTester.chpl:76 */
#line 76 "../lib/MNISTTester.chpl"
static void chpl__forexpr14(_tuple_2__ref__array_DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t__ref__array_DefaultRectangularArr_1_int64_t_one_int64_t_int64_t_chpl * iterExpr_chpl,
#line 76 "../lib/MNISTTester.chpl"
                            _ir_chpl__loopexpr_iter14_chpl * _retArg_chpl) {
#line 76 "../lib/MNISTTester.chpl"
  _ir_chpl__loopexpr_iter14_chpl ret_tmp_chpl;
#line 76 "../lib/MNISTTester.chpl"
  DefaultRectangularDom_1_int64_t_one_chpl _ir__shape__chpl = NULL;
#line 76 "../lib/MNISTTester.chpl"
  _ref__tuple_2__ref__array_DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t__ref__array_DefaultRectangularArr_1_int64_t_one_int64_t_int64_t_chpl _ir_F0_iterator_chpl = NULL;
#line 76 "../lib/MNISTTester.chpl"
  _ir_F0_iterator_chpl = iterExpr_chpl;
#line 76 "../lib/MNISTTester.chpl"
  (&ret_tmp_chpl)->_shape_ = _ir__shape__chpl;
#line 76 "../lib/MNISTTester.chpl"
  (&ret_tmp_chpl)->F0_iterator = _ir_F0_iterator_chpl;
#line 76 "../lib/MNISTTester.chpl"
  *(_retArg_chpl) = ret_tmp_chpl;
#line 76 "../lib/MNISTTester.chpl"
  return;
#line 76 "../lib/MNISTTester.chpl"
}

#line 112 "../lib/MNISTTester.chpl"
/* MNISTTester.chpl:112 */
#line 112 "../lib/MNISTTester.chpl"
static void coforall_fn_chpl115(int64_t numChunks_chpl,
#line 112 "../lib/MNISTTester.chpl"
                                int64_t parDim_chpl,
#line 112 "../lib/MNISTTester.chpl"
                                DefaultRectangularDom_1_int64_t_one_chpl this_chpl31,
#line 112 "../lib/MNISTTester.chpl"
                                chpl___EndCount_AtomicT_int64_t_int64_t _coforallCount_chpl,
#line 112 "../lib/MNISTTester.chpl"
                                int64_t chunk_chpl,
#line 112 "../lib/MNISTTester.chpl"
                                Error_chpl * error_out_chpl,
#line 112 "../lib/MNISTTester.chpl"
                                DefaultRectangularArr_1_int64_t_one__tuple_2_Tensor_3__real64_int64_t_int64_t_chpl this_chpl32,
#line 112 "../lib/MNISTTester.chpl"
                                SumReduceScanOp__real64_chpl RP_loss_chpl,
#line 112 "../lib/MNISTTester.chpl"
                                SumReduceScanOp_int64_t_chpl RP_numCorrect_chpl,
#line 112 "../lib/MNISTTester.chpl"
                                Network__tuple_4_Conv_Conv_MaxPool_SoftMax_chpl * network_chpl) {
#line 112 "../lib/MNISTTester.chpl"
  SumReduceScanOp__real64_chpl RP_loss_chpl2 = NULL;
#line 112 "../lib/MNISTTester.chpl"
  _real64 AS_loss_chpl;
#line 112 "../lib/MNISTTester.chpl"
  SumReduceScanOp__real64_chpl new_temp_chpl = NULL;
#line 112 "../lib/MNISTTester.chpl"
  chpl_LocalSpinlock default_arg_l_chpl;
#line 112 "../lib/MNISTTester.chpl"
  chpl_LocalSpinlock ret_chpl;
#line 112 "../lib/MNISTTester.chpl"
  chpl_LocalSpinlock l_chpl;
#line 112 "../lib/MNISTTester.chpl"
  SumReduceScanOp_int64_t_chpl RP_numCorrect_chpl2 = NULL;
#line 112 "../lib/MNISTTester.chpl"
  int64_t AS_numCorrect_chpl;
#line 112 "../lib/MNISTTester.chpl"
  SumReduceScanOp_int64_t_chpl new_temp_chpl2 = NULL;
#line 112 "../lib/MNISTTester.chpl"
  chpl_LocalSpinlock default_arg_l_chpl2;
#line 112 "../lib/MNISTTester.chpl"
  chpl_LocalSpinlock ret_chpl2;
#line 112 "../lib/MNISTTester.chpl"
  chpl_LocalSpinlock l_chpl2;
#line 112 "../lib/MNISTTester.chpl"
  _tuple_1_star_range_int64_t_both_one_chpl ranges_chpl;
#line 112 "../lib/MNISTTester.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl6 = NULL;
#line 112 "../lib/MNISTTester.chpl"
  uint64_t call_tmp_chpl7;
#line 112 "../lib/MNISTTester.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl3;
#line 112 "../lib/MNISTTester.chpl"
  uint64_t call_tmp_chpl8;
#line 112 "../lib/MNISTTester.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl9 = NULL;
#line 112 "../lib/MNISTTester.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl10 = NULL;
#line 112 "../lib/MNISTTester.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl11 = NULL;
#line 112 "../lib/MNISTTester.chpl"
  _tuple_2_star_int64_t_chpl call_tmp_chpl12;
#line 112 "../lib/MNISTTester.chpl"
  _tuple_2_star_int64_t_chpl ret_tmp_chpl;
#line 112 "../lib/MNISTTester.chpl"
  int64_t tmp_x0_chpl;
#line 112 "../lib/MNISTTester.chpl"
  int64_t tmp_x1_chpl;
#line 112 "../lib/MNISTTester.chpl"
  _tuple_1_star_range_int64_t_both_one_chpl block_chpl;
#line 112 "../lib/MNISTTester.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl13 = NULL;
#line 112 "../lib/MNISTTester.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl14 = NULL;
#line 112 "../lib/MNISTTester.chpl"
  _tuple_1_star_range_int64_t_both_one_chpl coerce_tmp_chpl4;
#line 112 "../lib/MNISTTester.chpl"
  _tuple_1_star_range_int64_t_both_one_chpl retcopy_chpl;
#line 112 "../lib/MNISTTester.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl5;
#line 112 "../lib/MNISTTester.chpl"
  range_int64_t_both_one_chpl _ic__F0_this_chpl;
#line 112 "../lib/MNISTTester.chpl"
  int64_t i_chpl;
#line 112 "../lib/MNISTTester.chpl"
  range_int64_t_both_one_chpl this_chpl33;
#line 112 "../lib/MNISTTester.chpl"
  range_int64_t_both_one_chpl this_chpl34;
#line 112 "../lib/MNISTTester.chpl"
  int64_t end_chpl;
#line 112 "../lib/MNISTTester.chpl"
  _ddata__tuple_2_Tensor_3__real64_int64_t_chpl coerce_tmp_chpl6 = NULL;
#line 112 "../lib/MNISTTester.chpl"
  _ref__tuple_2_Tensor_3__real64_int64_t_chpl call_tmp_chpl15 = NULL;
#line 112 "../lib/MNISTTester.chpl"
  _ref_Tensor_3__real64_chpl im_chpl = NULL;
#line 113 "../lib/MNISTTester.chpl"
  _tuple_3_Tensor_1__real64__real64_chpl_bool_chpl call_tmp_chpl16;
#line 113 "../lib/MNISTTester.chpl"
  _tuple_3_Tensor_1__real64__real64_chpl_bool_chpl ret_tmp_chpl2;
#line 113 "../lib/MNISTTester.chpl"
  Tensor_1__real64_chpl tmp_x0_chpl2;
#line 113 "../lib/MNISTTester.chpl"
  _real64 tmp_x1_chpl2;
#line 113 "../lib/MNISTTester.chpl"
  chpl_bool tmp_x2_chpl;
#line 113 "../lib/MNISTTester.chpl"
  _ref_Tensor_1__real64_chpl call_tmp_chpl17 = NULL;
#line 113 "../lib/MNISTTester.chpl"
  Tensor_1__real64_chpl o_chpl;
#line 114 "../lib/MNISTTester.chpl"
  _ref__real64_chpl i_lhs_chpl = NULL;
#line 115 "../lib/MNISTTester.chpl"
  _array_DefaultRectangularArr_1_int64_t_one__real64_int64_t_chpl _field_destructor_tmp__chpl;
#line 115 "../lib/MNISTTester.chpl"
  _domain_DefaultRectangularDom_1_int64_t_one_chpl _field_destructor_tmp__chpl2;
#line 115 "../lib/MNISTTester.chpl"
  _array_DefaultRectangularArr_1_int64_t_one__real64_int64_t_chpl _field_destructor_tmp__chpl3;
#line 115 "../lib/MNISTTester.chpl"
  _domain_DefaultRectangularDom_1_int64_t_one_chpl _field_destructor_tmp__chpl4;
#line 112 "../lib/MNISTTester.chpl"
  SumReduceScanOp_int64_t_chpl coerce_tmp_chpl7 = NULL;
#line 112 "../lib/MNISTTester.chpl"
  _ref_int64_t_chpl call_tmp_chpl18 = NULL;
#line 112 "../lib/MNISTTester.chpl"
  SumReduceScanOp__real64_chpl coerce_tmp_chpl8 = NULL;
#line 112 "../lib/MNISTTester.chpl"
  _ref__real64_chpl call_tmp_chpl19 = NULL;
#line 112 "../lib/MNISTTester.chpl"
  init_chpl105(&l_chpl);
#line 112 "../lib/MNISTTester.chpl"
  ret_chpl = l_chpl;
#line 112 "../lib/MNISTTester.chpl"
  default_arg_l_chpl = ret_chpl;
#line 112 "../lib/MNISTTester.chpl"
  new_temp_chpl = _new_chpl23(&default_arg_l_chpl, REAL64(0x0p+0), INT64(112), INT32(66));
#line 112 "../lib/MNISTTester.chpl"
  RP_loss_chpl2 = new_temp_chpl;
#line 112 "../lib/MNISTTester.chpl"
  AS_loss_chpl = REAL64(0x0p+0);
#line 112 "../lib/MNISTTester.chpl"
  init_chpl105(&l_chpl2);
#line 112 "../lib/MNISTTester.chpl"
  ret_chpl2 = l_chpl2;
#line 112 "../lib/MNISTTester.chpl"
  default_arg_l_chpl2 = ret_chpl2;
#line 112 "../lib/MNISTTester.chpl"
  new_temp_chpl2 = _new_chpl21(&default_arg_l_chpl2, INT64(0), INT64(112), INT32(66));
#line 112 "../lib/MNISTTester.chpl"
  RP_numCorrect_chpl2 = new_temp_chpl2;
#line 112 "../lib/MNISTTester.chpl"
  AS_numCorrect_chpl = INT64(0);
#line 112 "../lib/MNISTTester.chpl"
  chpl_check_nil(this_chpl31, INT64(112), INT32(66));
#line 112 "../lib/MNISTTester.chpl"
  *(ranges_chpl + INT64(0)) = *((this_chpl31)->ranges + INT64(0));
#line 112 "../lib/MNISTTester.chpl"
  call_tmp_chpl6 = this_chpl20(&ranges_chpl, parDim_chpl, INT64(112), INT32(66));
#line 112 "../lib/MNISTTester.chpl"
  coerce_tmp_chpl3 = *(call_tmp_chpl6);
#line 112 "../lib/MNISTTester.chpl"
  call_tmp_chpl7 = sizeAs_chpl36(coerce_tmp_chpl3, INT64(112), INT32(66));
#line 112 "../lib/MNISTTester.chpl"
  this_chpl20(&ranges_chpl, parDim_chpl, INT64(112), INT32(66));
#line 112 "../lib/MNISTTester.chpl"
  call_tmp_chpl8 = (call_tmp_chpl7 * UINT64(1));
#line 112 "../lib/MNISTTester.chpl"
  call_tmp_chpl9 = this_chpl20(&ranges_chpl, parDim_chpl, INT64(112), INT32(66));
#line 112 "../lib/MNISTTester.chpl"
  call_tmp_chpl10 = this_chpl20(&ranges_chpl, parDim_chpl, INT64(112), INT32(66));
#line 112 "../lib/MNISTTester.chpl"
  call_tmp_chpl11 = this_chpl20(&ranges_chpl, parDim_chpl, INT64(112), INT32(66));
#line 112 "../lib/MNISTTester.chpl"
  _computeBlock_chpl2(call_tmp_chpl8, numChunks_chpl, chunk_chpl, (call_tmp_chpl9)->_high, (call_tmp_chpl10)->_low, (call_tmp_chpl11)->_low, &ret_tmp_chpl, INT64(112), INT32(66));
#line 112 "../lib/MNISTTester.chpl"
  *(call_tmp_chpl12 + INT64(0)) = *(ret_tmp_chpl + INT64(0));
#line 112 "../lib/MNISTTester.chpl"
  *(call_tmp_chpl12 + INT64(1)) = *(ret_tmp_chpl + INT64(1));
#line 112 "../lib/MNISTTester.chpl"
  tmp_x0_chpl = *(call_tmp_chpl12 + INT64(0));
#line 112 "../lib/MNISTTester.chpl"
  tmp_x1_chpl = *(call_tmp_chpl12 + INT64(1));
#line 112 "../lib/MNISTTester.chpl"
  *(block_chpl + INT64(0)) = *(ranges_chpl + INT64(0));
#line 112 "../lib/MNISTTester.chpl"
  call_tmp_chpl13 = this_chpl20(&block_chpl, parDim_chpl, INT64(112), INT32(66));
#line 112 "../lib/MNISTTester.chpl"
  (call_tmp_chpl13)->_low = tmp_x0_chpl;
#line 112 "../lib/MNISTTester.chpl"
  call_tmp_chpl14 = this_chpl20(&block_chpl, parDim_chpl, INT64(112), INT32(66));
#line 112 "../lib/MNISTTester.chpl"
  (call_tmp_chpl14)->_high = tmp_x1_chpl;
#line 112 "../lib/MNISTTester.chpl"
  *(retcopy_chpl + INT64(0)) = *(block_chpl + INT64(0));
#line 112 "../lib/MNISTTester.chpl"
  *(coerce_tmp_chpl4 + INT64(0)) = *(retcopy_chpl + INT64(0));
#line 112 "../lib/MNISTTester.chpl"
  coerce_tmp_chpl5 = *(coerce_tmp_chpl4 + INT64(0));
#line 112 "../lib/MNISTTester.chpl"
  _ic__F0_this_chpl = coerce_tmp_chpl5;
#line 112 "../lib/MNISTTester.chpl"
  checkIfIterWillOverflow_chpl(coerce_tmp_chpl5, UINT8(true), INT64(112), INT32(66));
#line 112 "../lib/MNISTTester.chpl"
  i_chpl = INT64(0);
#line 112 "../lib/MNISTTester.chpl"
  this_chpl33 = _ic__F0_this_chpl;
#line 112 "../lib/MNISTTester.chpl"
  this_chpl34 = _ic__F0_this_chpl;
#line 112 "../lib/MNISTTester.chpl"
  end_chpl = (&this_chpl34)->_high;
#line 112 "../lib/MNISTTester.chpl"
  for (i_chpl = (&this_chpl33)->_low; ((i_chpl <= end_chpl)); i_chpl += INT64(1)) {
#line 112 "../lib/MNISTTester.chpl"
    chpl_check_nil(this_chpl32, INT64(112), INT32(66));
#line 112 "../lib/MNISTTester.chpl"
    coerce_tmp_chpl6 = (this_chpl32)->shiftedData;
#line 112 "../lib/MNISTTester.chpl"
    call_tmp_chpl15 = (coerce_tmp_chpl6 + i_chpl);
#line 112 "../lib/MNISTTester.chpl"
    im_chpl = &((call_tmp_chpl15)->x0);
#line 113 "../lib/MNISTTester.chpl"
    forward_chpl(network_chpl, im_chpl, (call_tmp_chpl15)->x1, &ret_tmp_chpl2);
#line 113 "../lib/MNISTTester.chpl"
    call_tmp_chpl16 = ret_tmp_chpl2;
#line 113 "../lib/MNISTTester.chpl"
    tmp_x0_chpl2 = (&call_tmp_chpl16)->x0;
#line 113 "../lib/MNISTTester.chpl"
    tmp_x1_chpl2 = (&call_tmp_chpl16)->x1;
#line 113 "../lib/MNISTTester.chpl"
    tmp_x2_chpl = (&call_tmp_chpl16)->x2;
#line 113 "../lib/MNISTTester.chpl"
    call_tmp_chpl17 = &tmp_x0_chpl2;
#line 113 "../lib/MNISTTester.chpl"
    init_ASSIGN__chpl32(&o_chpl, call_tmp_chpl17);
#line 114 "../lib/MNISTTester.chpl"
    i_lhs_chpl = &AS_loss_chpl;
#line 114 "../lib/MNISTTester.chpl"
    *(i_lhs_chpl) += tmp_x1_chpl2;
#line 115 "../lib/MNISTTester.chpl"
    AS_numCorrect_chpl = ((int64_t)((AS_numCorrect_chpl + ((int64_t)(tmp_x2_chpl)))));
#line 115 "../lib/MNISTTester.chpl"
    _field_destructor_tmp__chpl = (&o_chpl)->data;
#line 115 "../lib/MNISTTester.chpl"
    chpl__autoDestroy18(&_field_destructor_tmp__chpl, INT64(115), INT32(66));
#line 115 "../lib/MNISTTester.chpl"
    _field_destructor_tmp__chpl2 = (&o_chpl)->_domain;
#line 115 "../lib/MNISTTester.chpl"
    chpl__autoDestroy11(&_field_destructor_tmp__chpl2, INT64(115), INT32(66));
#line 115 "../lib/MNISTTester.chpl"
    _field_destructor_tmp__chpl3 = (&tmp_x0_chpl2)->data;
#line 115 "../lib/MNISTTester.chpl"
    chpl__autoDestroy18(&_field_destructor_tmp__chpl3, INT64(115), INT32(66));
#line 115 "../lib/MNISTTester.chpl"
    _field_destructor_tmp__chpl4 = (&tmp_x0_chpl2)->_domain;
#line 115 "../lib/MNISTTester.chpl"
    chpl__autoDestroy11(&_field_destructor_tmp__chpl4, INT64(115), INT32(66));
#line 115 "../lib/MNISTTester.chpl"
  }
#line 112 "../lib/MNISTTester.chpl"
  coerce_tmp_chpl7 = RP_numCorrect_chpl2;
#line 112 "../lib/MNISTTester.chpl"
  chpl_check_nil(coerce_tmp_chpl7, INT64(112), INT32(66));
#line 112 "../lib/MNISTTester.chpl"
  call_tmp_chpl18 = &((coerce_tmp_chpl7)->value);
#line 112 "../lib/MNISTTester.chpl"
  *(call_tmp_chpl18) += AS_numCorrect_chpl;
#line 112 "../lib/MNISTTester.chpl"
  chpl__reduceCombine5(RP_numCorrect_chpl, RP_numCorrect_chpl2, INT64(112), INT32(66));
#line 112 "../lib/MNISTTester.chpl"
  chpl__delete16(RP_numCorrect_chpl2, INT64(112), INT32(66));
#line 112 "../lib/MNISTTester.chpl"
  coerce_tmp_chpl8 = RP_loss_chpl2;
#line 112 "../lib/MNISTTester.chpl"
  chpl_check_nil(coerce_tmp_chpl8, INT64(112), INT32(66));
#line 112 "../lib/MNISTTester.chpl"
  call_tmp_chpl19 = &((coerce_tmp_chpl8)->value);
#line 112 "../lib/MNISTTester.chpl"
  *(call_tmp_chpl19) += AS_loss_chpl;
#line 112 "../lib/MNISTTester.chpl"
  chpl__reduceCombine6(RP_loss_chpl, RP_loss_chpl2, INT64(112), INT32(66));
#line 112 "../lib/MNISTTester.chpl"
  chpl__delete33(RP_loss_chpl2, INT64(112), INT32(66));
#line 112 "../lib/MNISTTester.chpl"
  return;
#line 112 "../lib/MNISTTester.chpl"
}

#line 112 "../lib/MNISTTester.chpl"
/* MNISTTester.chpl:112 */
#line 112 "../lib/MNISTTester.chpl"
static void wrapcoforall_fn_chpl115(_class_localscoforall_fn_chpl115 c_chpl) {
#line 112 "../lib/MNISTTester.chpl"
  Error_chpl error_chpl = NULL;
#line 112 "../lib/MNISTTester.chpl"
  int64_t _0_numChunks_chpl;
#line 112 "../lib/MNISTTester.chpl"
  int64_t _1_parDim_chpl;
#line 112 "../lib/MNISTTester.chpl"
  DefaultRectangularDom_1_int64_t_one_chpl _2_coerce_tmp_chpl = NULL;
#line 112 "../lib/MNISTTester.chpl"
  chpl___EndCount_AtomicT_int64_t_int64_t _3__coforallCount_chpl = NULL;
#line 112 "../lib/MNISTTester.chpl"
  int64_t _4_chunk_chpl;
#line 112 "../lib/MNISTTester.chpl"
  DefaultRectangularArr_1_int64_t_one__tuple_2_Tensor_3__real64_int64_t_int64_t_chpl _6_coerce_tmp_chpl = NULL;
#line 112 "../lib/MNISTTester.chpl"
  SumReduceScanOp__real64_chpl _7_globalRP_loss_chpl = NULL;
#line 112 "../lib/MNISTTester.chpl"
  SumReduceScanOp_int64_t_chpl _8_globalRP_numCorrect_chpl = NULL;
#line 112 "../lib/MNISTTester.chpl"
  _ref_Network__tuple_4_Conv_Conv_MaxPool_SoftMax_chpl _9_network_chpl = NULL;
#line 112 "../lib/MNISTTester.chpl"
  chpl_rmem_consist_acquire(INT64(112), INT32(66));
#line 112 "../lib/MNISTTester.chpl"
  chpl_check_nil(c_chpl, INT64(112), INT32(66));
#line 112 "../lib/MNISTTester.chpl"
  _0_numChunks_chpl = (c_chpl)->_0_numChunks;
#line 112 "../lib/MNISTTester.chpl"
  chpl_check_nil(c_chpl, INT64(112), INT32(66));
#line 112 "../lib/MNISTTester.chpl"
  _1_parDim_chpl = (c_chpl)->_1_parDim;
#line 112 "../lib/MNISTTester.chpl"
  chpl_check_nil(c_chpl, INT64(112), INT32(66));
#line 112 "../lib/MNISTTester.chpl"
  _2_coerce_tmp_chpl = (c_chpl)->_2_coerce_tmp;
#line 112 "../lib/MNISTTester.chpl"
  chpl_check_nil(c_chpl, INT64(112), INT32(66));
#line 112 "../lib/MNISTTester.chpl"
  _3__coforallCount_chpl = (c_chpl)->_3__coforallCount;
#line 112 "../lib/MNISTTester.chpl"
  chpl_check_nil(c_chpl, INT64(112), INT32(66));
#line 112 "../lib/MNISTTester.chpl"
  _4_chunk_chpl = (c_chpl)->_4_chunk;
#line 112 "../lib/MNISTTester.chpl"
  chpl_check_nil(c_chpl, INT64(112), INT32(66));
#line 112 "../lib/MNISTTester.chpl"
  _6_coerce_tmp_chpl = (c_chpl)->_6_coerce_tmp;
#line 112 "../lib/MNISTTester.chpl"
  chpl_check_nil(c_chpl, INT64(112), INT32(66));
#line 112 "../lib/MNISTTester.chpl"
  _7_globalRP_loss_chpl = (c_chpl)->_7_globalRP_loss;
#line 112 "../lib/MNISTTester.chpl"
  chpl_check_nil(c_chpl, INT64(112), INT32(66));
#line 112 "../lib/MNISTTester.chpl"
  _8_globalRP_numCorrect_chpl = (c_chpl)->_8_globalRP_numCorrect;
#line 112 "../lib/MNISTTester.chpl"
  chpl_check_nil(c_chpl, INT64(112), INT32(66));
#line 112 "../lib/MNISTTester.chpl"
  _9_network_chpl = (c_chpl)->_9_network;
#line 112 "../lib/MNISTTester.chpl"
  coforall_fn_chpl115(_0_numChunks_chpl, _1_parDim_chpl, _2_coerce_tmp_chpl, _3__coforallCount_chpl, _4_chunk_chpl, &error_chpl, _6_coerce_tmp_chpl, _7_globalRP_loss_chpl, _8_globalRP_numCorrect_chpl, _9_network_chpl);
#line 112 "../lib/MNISTTester.chpl"
  chpl_check_nil(c_chpl, INT64(112), INT32(66));
#line 112 "../lib/MNISTTester.chpl"
  _downEndCount_chpl((c_chpl)->_3__coforallCount, error_chpl, INT64(112), INT32(66));
#line 112 "../lib/MNISTTester.chpl"
  return;
#line 112 "../lib/MNISTTester.chpl"
}

