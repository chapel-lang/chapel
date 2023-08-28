#line 2 "../lib/Chai.chpl"
/* Chai.chpl:2 */
#line 2 "../lib/Chai.chpl"
static void chpl__init_Chai(int64_t _ln_chpl,
#line 2 "../lib/Chai.chpl"
                            int32_t _fn_chpl) {
#line 2 "../lib/Chai.chpl"
  _ref_int32_t_chpl refIndentLevel_chpl = NULL;
#line 2 "../lib/Chai.chpl"
  if (chpl__init_Chai_p) /* ZLINE: 2 ../lib/Chai.chpl */
#line 2 "../lib/Chai.chpl"
  {
#line 2 "../lib/Chai.chpl"
    goto _exit_chpl__init_Chai_chpl;
#line 2 "../lib/Chai.chpl"
  }
#line 2 "../lib/Chai.chpl"
  printModuleInit_chpl("%*s\n", "Chai", INT64(4), INT64(2), INT32(65));
#line 2 "../lib/Chai.chpl"
  refIndentLevel_chpl = &moduleInitLevel_chpl;
#line 2 "../lib/Chai.chpl"
  *(refIndentLevel_chpl) += INT64(1);
#line 2 "../lib/Chai.chpl"
  chpl__init_Chai_p = UINT8(true);
#line 2 "../lib/Chai.chpl"
  {
#line 2 "../lib/Chai.chpl"
    chpl__init_IO2(INT64(2), INT32(65));
#line 2 "../lib/Chai.chpl"
    chpl__init_Tensor(INT64(2), INT32(65));
#line 2 "../lib/Chai.chpl"
  }
#line 2 "../lib/Chai.chpl"
  *(refIndentLevel_chpl) -= INT64(1);
#line 2 "../lib/Chai.chpl"
  _exit_chpl__init_Chai_chpl:;
#line 2 "../lib/Chai.chpl"
  return;
#line 2 "../lib/Chai.chpl"
}

#line 190 "../lib/Chai.chpl"
/* Chai.chpl:190 */
#line 190 "../lib/Chai.chpl"
static void init_ASSIGN__chpl27(Conv_chpl * this_chpl31,
#line 190 "../lib/Chai.chpl"
                                Conv_chpl * other_chpl) {
#line 190 "../lib/Chai.chpl"
  _ref_Tensor_4__real64_chpl call_tmp_chpl6 = NULL;
#line 190 "../lib/Chai.chpl"
  Tensor_4__real64_chpl filters_chpl;
#line 190 "../lib/Chai.chpl"
  _ref_Tensor_4__real64_chpl call_tmp_chpl7 = NULL;
#line 190 "../lib/Chai.chpl"
  Tensor_4__real64_chpl filtersGrad_chpl;
#line 190 "../lib/Chai.chpl"
  (this_chpl31)->numFilters = (other_chpl)->numFilters;
#line 190 "../lib/Chai.chpl"
  call_tmp_chpl6 = &((other_chpl)->filters);
#line 190 "../lib/Chai.chpl"
  init_ASSIGN__chpl29(&filters_chpl, call_tmp_chpl6);
#line 190 "../lib/Chai.chpl"
  (this_chpl31)->filters = filters_chpl;
#line 190 "../lib/Chai.chpl"
  call_tmp_chpl7 = &((other_chpl)->filtersGrad);
#line 190 "../lib/Chai.chpl"
  init_ASSIGN__chpl29(&filtersGrad_chpl, call_tmp_chpl7);
#line 190 "../lib/Chai.chpl"
  (this_chpl31)->filtersGrad = filtersGrad_chpl;
#line 190 "../lib/Chai.chpl"
  (this_chpl31)->isFirstLayer = (other_chpl)->isFirstLayer;
#line 190 "../lib/Chai.chpl"
  (this_chpl31)->stride = (other_chpl)->stride;
#line 190 "../lib/Chai.chpl"
  (this_chpl31)->padding = (other_chpl)->padding;
#line 190 "../lib/Chai.chpl"
  return;
#line 190 "../lib/Chai.chpl"
}

#line 199 "../lib/Chai.chpl"
/* Chai.chpl:199 */
#line 199 "../lib/Chai.chpl"
static void init_chpl229(Conv_chpl * this_chpl31,
#line 199 "../lib/Chai.chpl"
                         int64_t inChannels_chpl,
#line 199 "../lib/Chai.chpl"
                         int64_t outChannels_chpl,
#line 199 "../lib/Chai.chpl"
                         int64_t kernelSize_chpl,
#line 199 "../lib/Chai.chpl"
                         int64_t stride_chpl,
#line 199 "../lib/Chai.chpl"
                         int64_t padding_chpl) {
#line 205 "../lib/Chai.chpl"
  _real64 call_tmp_chpl6;
#line 205 "../lib/Chai.chpl"
  _real64 call_tmp_chpl7;
#line 206 "../lib/Chai.chpl"
  Tensor_4__real64_chpl call_tmp_chpl8;
#line 206 "../lib/Chai.chpl"
  Tensor_4__real64_chpl ret_tmp_chpl;
#line 209 "../lib/Chai.chpl"
  Tensor_4__real64_chpl call_tmp_chpl9;
#line 209 "../lib/Chai.chpl"
  Tensor_4__real64_chpl ret_tmp_chpl2;
#line 201 "../lib/Chai.chpl"
  (this_chpl31)->numFilters = outChannels_chpl;
#line 205 "../lib/Chai.chpl"
  call_tmp_chpl6 = (REAL64(0x1p+1) / ((_real64)(((int64_t)((outChannels_chpl + ((int64_t)((((int64_t)((kernelSize_chpl * kernelSize_chpl))) * inChannels_chpl)))))))));
#line 205 "../lib/Chai.chpl"
  call_tmp_chpl7 = sqrt(call_tmp_chpl6);
#line 206 "../lib/Chai.chpl"
  randn_chpl2(outChannels_chpl, kernelSize_chpl, kernelSize_chpl, inChannels_chpl, REAL64(0x0p+0), call_tmp_chpl7, &ret_tmp_chpl);
#line 206 "../lib/Chai.chpl"
  call_tmp_chpl8 = ret_tmp_chpl;
#line 206 "../lib/Chai.chpl"
  (this_chpl31)->filters = call_tmp_chpl8;
#line 209 "../lib/Chai.chpl"
  zeros_chpl2(outChannels_chpl, kernelSize_chpl, kernelSize_chpl, inChannels_chpl, &ret_tmp_chpl2);
#line 209 "../lib/Chai.chpl"
  call_tmp_chpl9 = ret_tmp_chpl2;
#line 209 "../lib/Chai.chpl"
  (this_chpl31)->filtersGrad = call_tmp_chpl9;
#line 210 "../lib/Chai.chpl"
  (this_chpl31)->isFirstLayer = UINT8(false);
#line 210 "../lib/Chai.chpl"
  (this_chpl31)->stride = stride_chpl;
#line 211 "../lib/Chai.chpl"
  (this_chpl31)->padding = padding_chpl;
#line 199 "../lib/Chai.chpl"
  return;
#line 199 "../lib/Chai.chpl"
}

#line 226 "../lib/Chai.chpl"
/* Chai.chpl:226 */
#line 226 "../lib/Chai.chpl"
static void forwardPropBatch_chpl(Conv_chpl * this_chpl31,
#line 226 "../lib/Chai.chpl"
                                  _array_DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl * batch_chpl,
#line 226 "../lib/Chai.chpl"
                                  _array_DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl * _retArg_chpl) {
#line 229 "../lib/Chai.chpl"
  int64_t local_dataParMinGranularity_chpl;
#line 229 "../lib/Chai.chpl"
  chpl_bool local_dataParIgnoreRunningTasks_chpl;
#line 229 "../lib/Chai.chpl"
  int64_t local_dataParTasksPerLocale_chpl;
#line 227 "../lib/Chai.chpl"
  int64_t call_tmp_chpl6;
#line 228 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl convs_chpl;
#line 228 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl call_tmp_chpl7;
#line 228 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl ret_tmp_chpl;
#line 228 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl8;
#line 228 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl2;
#line 228 "../lib/Chai.chpl"
  _domain_DefaultRectangularDom_1_int64_t_one_chpl call_tmp_chpl9;
#line 228 "../lib/Chai.chpl"
  _domain_DefaultRectangularDom_1_int64_t_one_chpl ret_tmp_chpl3;
#line 228 "../lib/Chai.chpl"
  chpl___RuntimeTypeInfo6 call_tmp_chpl10;
#line 228 "../lib/Chai.chpl"
  _domain_DefaultRectangularDom_1_int64_t_one_chpl _runtime_type_tmp__chpl;
#line 228 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl ret_tmp_chpl4;
#line 229 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl call_tmp_chpl11;
#line 229 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl ret_tmp_chpl5;
#line 229 "../lib/Chai.chpl"
  DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl coerce_tmp_chpl3 = NULL;
#line 229 "../lib/Chai.chpl"
  int64_t tasksPerLocale_chpl;
#line 229 "../lib/Chai.chpl"
  chpl_bool ignoreRunning_chpl;
#line 229 "../lib/Chai.chpl"
  int64_t minIndicesPerTask_chpl;
#line 229 "../lib/Chai.chpl"
  Error_chpl error_chpl = NULL;
#line 229 "../lib/Chai.chpl"
  DefaultRectangularDom_1_int64_t_one_chpl coerce_tmp_chpl4 = NULL;
#line 229 "../lib/Chai.chpl"
  int64_t offset_x0_chpl;
#line 229 "../lib/Chai.chpl"
  _tuple_1_star_int64_t_chpl ret_tmp_chpl6;
#line 229 "../lib/Chai.chpl"
  Error_chpl error_chpl2 = NULL;
#line 229 "../lib/Chai.chpl"
  locale_chpl call_tmp_chpl12;
#line 229 "../lib/Chai.chpl"
  chpl_localeID_t call_tmp_chpl13;
#line 229 "../lib/Chai.chpl"
  int32_t call_tmp_chpl14;
#line 229 "../lib/Chai.chpl"
  chpl_localeID_t call_tmp_chpl15;
#line 229 "../lib/Chai.chpl"
  locale_chpl call_tmp_chpl16;
#line 229 "../lib/Chai.chpl"
  locale_chpl ret_tmp_chpl7;
#line 229 "../lib/Chai.chpl"
  BaseLocale_chpl coerce_tmp_chpl5 = NULL;
#line 229 "../lib/Chai.chpl"
  _ref_locale_chpl i_x_chpl = NULL;
#line 229 "../lib/Chai.chpl"
  int64_t tmp_chpl;
#line 229 "../lib/Chai.chpl"
  locale_chpl call_tmp_chpl17;
#line 229 "../lib/Chai.chpl"
  chpl_localeID_t call_tmp_chpl18;
#line 229 "../lib/Chai.chpl"
  int32_t call_tmp_chpl19;
#line 229 "../lib/Chai.chpl"
  chpl_localeID_t call_tmp_chpl20;
#line 229 "../lib/Chai.chpl"
  locale_chpl call_tmp_chpl21;
#line 229 "../lib/Chai.chpl"
  locale_chpl ret_tmp_chpl8;
#line 229 "../lib/Chai.chpl"
  int64_t ret_chpl;
#line 229 "../lib/Chai.chpl"
  BaseLocale_chpl coerce_tmp_chpl6 = NULL;
#line 229 "../lib/Chai.chpl"
  BaseLocale_chpl coerce_tmp_chpl7 = NULL;
#line 229 "../lib/Chai.chpl"
  _ref_locale_chpl i_x_chpl2 = NULL;
#line 229 "../lib/Chai.chpl"
  int64_t tmp_x0_chpl;
#line 229 "../lib/Chai.chpl"
  int64_t tmp_x1_chpl;
#line 229 "../lib/Chai.chpl"
  _ref__tuple_1_star_range_int64_t_both_one_chpl call_tmp_chpl22 = NULL;
#line 229 "../lib/Chai.chpl"
  _tuple_2_star_int64_t_chpl ret_tmp_chpl9;
#line 229 "../lib/Chai.chpl"
  int64_t numChunks_chpl;
#line 229 "../lib/Chai.chpl"
  int64_t parDim_chpl;
#line 229 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl locBlock_x0_chpl;
#line 229 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl tup_x0_chpl;
#line 229 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl elt_x0_chpl;
#line 229 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl call_tmp_chpl23;
#line 229 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl ret_tmp_chpl10;
#line 229 "../lib/Chai.chpl"
  _ref__tuple_1_star_range_int64_t_both_one_chpl call_tmp_chpl24 = NULL;
#line 229 "../lib/Chai.chpl"
  int64_t call_tmp_chpl25;
#line 229 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl8;
#line 229 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl26;
#line 229 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl11;
#line 229 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl tmpIter_chpl;
#line 229 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl call_tmp_chpl27;
#line 229 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl ret_tmp_chpl12;
#line 229 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl13;
#line 229 "../lib/Chai.chpl"
  chpl___EndCount_AtomicT_int64_t_int64_t _coforallCount_chpl = NULL;
#line 229 "../lib/Chai.chpl"
  chpl___EndCount_AtomicT_int64_t_int64_t new_temp_chpl = NULL;
#line 229 "../lib/Chai.chpl"
  int64_t numTasks_chpl;
#line 229 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl _ic__F0_this_chpl;
#line 229 "../lib/Chai.chpl"
  int64_t i_chpl;
#line 229 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl this_chpl32;
#line 229 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl this_chpl33;
#line 229 "../lib/Chai.chpl"
  int64_t end_chpl;
#line 229 "../lib/Chai.chpl"
  _tuple_1_star_range_int64_t_both_one_chpl ret_tmp_chpl14;
#line 229 "../lib/Chai.chpl"
  _class_localscoforall_fn_chpl99 _args_forcoforall_fn_chpl = NULL;
#line 229 "../lib/Chai.chpl"
  _ref_Conv_chpl tmp_chpl2 = NULL;
#line 229 "../lib/Chai.chpl"
  _ref_range_int64_t_low_one_chpl tmp_chpl3 = NULL;
#line 229 "../lib/Chai.chpl"
  raw_c_void_ptr _args_vforcoforall_fn_chpl;
#line 229 "../lib/Chai.chpl"
  Error_chpl error_chpl3 = NULL;
#line 229 "../lib/Chai.chpl"
  chpl__class_localscoforall_fn_chpl99_object chpl_macro_tmp_101;
#line 229 "../lib/Chai.chpl"
  local_dataParMinGranularity_chpl = dataParMinGranularity_chpl;
#line 229 "../lib/Chai.chpl"
  local_dataParIgnoreRunningTasks_chpl = dataParIgnoreRunningTasks_chpl;
#line 229 "../lib/Chai.chpl"
  local_dataParTasksPerLocale_chpl = dataParTasksPerLocale_chpl;
#line 227 "../lib/Chai.chpl"
  call_tmp_chpl6 = size_chpl(batch_chpl, INT64(227), INT32(65));
#line 228 "../lib/Chai.chpl"
  chpl_build_low_bounded_range2(INT64(0), &ret_tmp_chpl);
#line 228 "../lib/Chai.chpl"
  call_tmp_chpl7 = ret_tmp_chpl;
#line 228 "../lib/Chai.chpl"
  chpl___POUND_3(call_tmp_chpl7, call_tmp_chpl6, &ret_tmp_chpl2, INT64(228), INT32(65));
#line 228 "../lib/Chai.chpl"
  call_tmp_chpl8 = ret_tmp_chpl2;
#line 228 "../lib/Chai.chpl"
  chpl__ensureDomainExpr5(call_tmp_chpl8, &ret_tmp_chpl3, INT64(228), INT32(65));
#line 228 "../lib/Chai.chpl"
  call_tmp_chpl9 = ret_tmp_chpl3;
#line 228 "../lib/Chai.chpl"
  call_tmp_chpl10 = chpl__buildArrayRuntimeType2(&call_tmp_chpl9);
#line 228 "../lib/Chai.chpl"
  _runtime_type_tmp__chpl = (&call_tmp_chpl10)->dom;
#line 228 "../lib/Chai.chpl"
  chpl__convertRuntimeTypeToValue3(&_runtime_type_tmp__chpl, UINT8(false), &ret_tmp_chpl4, INT64(228), INT32(65));
#line 228 "../lib/Chai.chpl"
  convs_chpl = ret_tmp_chpl4;
#line 229 "../lib/Chai.chpl"
  chpl_build_low_bounded_range2(INT64(0), &ret_tmp_chpl5);
#line 229 "../lib/Chai.chpl"
  call_tmp_chpl11 = ret_tmp_chpl5;
#line 229 "../lib/Chai.chpl"
  coerce_tmp_chpl3 = (batch_chpl)->_instance;
#line 229 "../lib/Chai.chpl"
  tasksPerLocale_chpl = local_dataParTasksPerLocale_chpl;
#line 229 "../lib/Chai.chpl"
  ignoreRunning_chpl = local_dataParIgnoreRunningTasks_chpl;
#line 229 "../lib/Chai.chpl"
  minIndicesPerTask_chpl = local_dataParMinGranularity_chpl;
#line 229 "../lib/Chai.chpl"
  error_chpl = NULL;
#line 229 "../lib/Chai.chpl"
  chpl_check_nil(coerce_tmp_chpl3, INT64(229), INT32(65));
#line 229 "../lib/Chai.chpl"
  coerce_tmp_chpl4 = (coerce_tmp_chpl3)->dom;
#line 229 "../lib/Chai.chpl"
  createTuple_chpl(INT64(0), &ret_tmp_chpl6);
#line 229 "../lib/Chai.chpl"
  offset_x0_chpl = *(ret_tmp_chpl6 + INT64(0));
#line 229 "../lib/Chai.chpl"
  error_chpl2 = NULL;
#line 229 "../lib/Chai.chpl"
  call_tmp_chpl14 = get_chpl_nodeID();
#line 229 "../lib/Chai.chpl"
  call_tmp_chpl15 = chpl_rt_buildLocaleID(call_tmp_chpl14, c_sublocid_any);
#line 229 "../lib/Chai.chpl"
  call_tmp_chpl13 = call_tmp_chpl15;
#line 229 "../lib/Chai.chpl"
  chpl_localeID_to_locale(&call_tmp_chpl13, &ret_tmp_chpl7, INT64(229), INT32(65));
#line 229 "../lib/Chai.chpl"
  call_tmp_chpl16 = ret_tmp_chpl7;
#line 229 "../lib/Chai.chpl"
  call_tmp_chpl12 = call_tmp_chpl16;
#line 229 "../lib/Chai.chpl"
  coerce_tmp_chpl5 = (&call_tmp_chpl12)->_instance;
#line 229 "../lib/Chai.chpl"
  if (((RootClass_chpl)(coerce_tmp_chpl5)) == nil) /* ZLINE: 229 ../lib/Chai.chpl */
#line 229 "../lib/Chai.chpl"
  {
#line 229 "../lib/Chai.chpl"
    nilCheckHalt_chpl(&_str_literal_52_chpl, INT64(229), INT32(65));
#line 229 "../lib/Chai.chpl"
  }
#line 229 "../lib/Chai.chpl"
  chpl_check_nil(coerce_tmp_chpl5, INT64(229), INT32(65));
#line 229 "../lib/Chai.chpl"
  ((int64_t(*)(BaseLocale_chpl,
#line 229 "../lib/Chai.chpl"
     int64_t,
#line 229 "../lib/Chai.chpl"
     int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(coerce_tmp_chpl5))->chpl__cid) + INT64(2))])(coerce_tmp_chpl5, INT64(229), INT32(65));
#line 229 "../lib/Chai.chpl"
  i_x_chpl = &call_tmp_chpl12;
#line 229 "../lib/Chai.chpl"
  deinit_chpl62(i_x_chpl);
#line 229 "../lib/Chai.chpl"
  if (tasksPerLocale_chpl == INT64(0)) /* ZLINE: 229 ../lib/Chai.chpl */
#line 229 "../lib/Chai.chpl"
  {
#line 229 "../lib/Chai.chpl"
    call_tmp_chpl19 = get_chpl_nodeID();
#line 229 "../lib/Chai.chpl"
    call_tmp_chpl20 = chpl_rt_buildLocaleID(call_tmp_chpl19, c_sublocid_any);
#line 229 "../lib/Chai.chpl"
    call_tmp_chpl18 = call_tmp_chpl20;
#line 229 "../lib/Chai.chpl"
    chpl_localeID_to_locale(&call_tmp_chpl18, &ret_tmp_chpl8, INT64(229), INT32(65));
#line 229 "../lib/Chai.chpl"
    call_tmp_chpl21 = ret_tmp_chpl8;
#line 229 "../lib/Chai.chpl"
    call_tmp_chpl17 = call_tmp_chpl21;
#line 229 "../lib/Chai.chpl"
    coerce_tmp_chpl6 = (&call_tmp_chpl17)->_instance;
#line 229 "../lib/Chai.chpl"
    if (((RootClass_chpl)(coerce_tmp_chpl6)) == nil) /* ZLINE: 229 ../lib/Chai.chpl */
#line 229 "../lib/Chai.chpl"
    {
#line 229 "../lib/Chai.chpl"
      nilCheckHalt_chpl(&_str_literal_52_chpl, INT64(229), INT32(65));
#line 229 "../lib/Chai.chpl"
    }
#line 229 "../lib/Chai.chpl"
    coerce_tmp_chpl7 = coerce_tmp_chpl6;
#line 229 "../lib/Chai.chpl"
    chpl_check_nil(coerce_tmp_chpl7, INT64(229), INT32(65));
#line 229 "../lib/Chai.chpl"
    ret_chpl = (coerce_tmp_chpl7)->maxTaskPar;
#line 229 "../lib/Chai.chpl"
    tmp_chpl = ret_chpl;
#line 229 "../lib/Chai.chpl"
    i_x_chpl2 = &call_tmp_chpl17;
#line 229 "../lib/Chai.chpl"
    deinit_chpl62(i_x_chpl2);
#line 229 "../lib/Chai.chpl"
  } else /* ZLINE: 229 ../lib/Chai.chpl */
#line 229 "../lib/Chai.chpl"
  {
#line 229 "../lib/Chai.chpl"
    tmp_chpl = tasksPerLocale_chpl;
#line 229 "../lib/Chai.chpl"
  }
#line 229 "../lib/Chai.chpl"
  if (chpl_task_getSerial()) /* ZLINE: 229 ../lib/Chai.chpl */
#line 229 "../lib/Chai.chpl"
  {
#line 229 "../lib/Chai.chpl"
    tmp_x0_chpl = INT64(1);
#line 229 "../lib/Chai.chpl"
    tmp_x1_chpl = INT64(0);
#line 229 "../lib/Chai.chpl"
  } else /* ZLINE: 229 ../lib/Chai.chpl */
#line 229 "../lib/Chai.chpl"
  {
#line 229 "../lib/Chai.chpl"
    chpl_check_nil(coerce_tmp_chpl4, INT64(229), INT32(65));
#line 229 "../lib/Chai.chpl"
    call_tmp_chpl22 = &((coerce_tmp_chpl4)->ranges);
#line 229 "../lib/Chai.chpl"
    _computeChunkStuff_chpl(tmp_chpl, ignoreRunning_chpl, minIndicesPerTask_chpl, call_tmp_chpl22, &ret_tmp_chpl9, INT64(229), INT32(65));
#line 229 "../lib/Chai.chpl"
    tmp_x0_chpl = *(ret_tmp_chpl9 + INT64(0));
#line 229 "../lib/Chai.chpl"
    tmp_x1_chpl = *(ret_tmp_chpl9 + INT64(1));
#line 229 "../lib/Chai.chpl"
  }
#line 229 "../lib/Chai.chpl"
  numChunks_chpl = tmp_x0_chpl;
#line 229 "../lib/Chai.chpl"
  parDim_chpl = tmp_x1_chpl;
#line 229 "../lib/Chai.chpl"
  init_chpl113(&elt_x0_chpl);
#line 229 "../lib/Chai.chpl"
  tup_x0_chpl = elt_x0_chpl;
#line 229 "../lib/Chai.chpl"
  locBlock_x0_chpl = tup_x0_chpl;
#line 229 "../lib/Chai.chpl"
  chpl_build_low_bounded_range2(offset_x0_chpl, &ret_tmp_chpl10);
#line 229 "../lib/Chai.chpl"
  call_tmp_chpl23 = ret_tmp_chpl10;
#line 229 "../lib/Chai.chpl"
  chpl_check_nil(coerce_tmp_chpl4, INT64(229), INT32(65));
#line 229 "../lib/Chai.chpl"
  call_tmp_chpl24 = &((coerce_tmp_chpl4)->ranges);
#line 229 "../lib/Chai.chpl"
  coerce_tmp_chpl8 = *(*(call_tmp_chpl24) + INT64(0));
#line 229 "../lib/Chai.chpl"
  call_tmp_chpl25 = sizeAs_chpl37(coerce_tmp_chpl8, INT64(229), INT32(65));
#line 229 "../lib/Chai.chpl"
  chpl___POUND_3(call_tmp_chpl23, call_tmp_chpl25, &ret_tmp_chpl11, INT64(229), INT32(65));
#line 229 "../lib/Chai.chpl"
  call_tmp_chpl26 = ret_tmp_chpl11;
#line 229 "../lib/Chai.chpl"
  locBlock_x0_chpl = call_tmp_chpl26;
#line 229 "../lib/Chai.chpl"
  chpl_build_low_bounded_range2(INT64(0), &ret_tmp_chpl12);
#line 229 "../lib/Chai.chpl"
  call_tmp_chpl27 = ret_tmp_chpl12;
#line 229 "../lib/Chai.chpl"
  chpl___POUND_3(call_tmp_chpl27, tmp_x0_chpl, &ret_tmp_chpl13, INT64(229), INT32(65));
#line 229 "../lib/Chai.chpl"
  tmpIter_chpl = ret_tmp_chpl13;
#line 229 "../lib/Chai.chpl"
  new_temp_chpl = _new_chpl17(INT64(229), INT32(65));
#line 229 "../lib/Chai.chpl"
  _coforallCount_chpl = new_temp_chpl;
#line 229 "../lib/Chai.chpl"
  numTasks_chpl = chpl_boundedCoforallSize(tmpIter_chpl, INT64(229), INT32(65));
#line 229 "../lib/Chai.chpl"
  _upEndCount_chpl(_coforallCount_chpl, numTasks_chpl, INT64(229), INT32(65));
#line 229 "../lib/Chai.chpl"
  chpl_resetTaskSpawn(numTasks_chpl, INT64(229), INT32(65));
#line 229 "../lib/Chai.chpl"
  _ic__F0_this_chpl = tmpIter_chpl;
#line 229 "../lib/Chai.chpl"
  checkIfIterWillOverflow_chpl(tmpIter_chpl, UINT8(true), INT64(229), INT32(65));
#line 229 "../lib/Chai.chpl"
  i_chpl = INT64(0);
#line 229 "../lib/Chai.chpl"
  this_chpl32 = _ic__F0_this_chpl;
#line 229 "../lib/Chai.chpl"
  this_chpl33 = _ic__F0_this_chpl;
#line 229 "../lib/Chai.chpl"
  end_chpl = (&this_chpl33)->_high;
#line 229 "../lib/Chai.chpl"
  for (i_chpl = (&this_chpl32)->_low; ((i_chpl <= end_chpl)); i_chpl += INT64(1)) {
#line 229 "../lib/Chai.chpl"
    *(ret_tmp_chpl14 + INT64(0)) = locBlock_x0_chpl;
#line 229 "../lib/Chai.chpl"
    _args_forcoforall_fn_chpl = ((_class_localscoforall_fn_chpl99)(&chpl_macro_tmp_101));
#line 229 "../lib/Chai.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl, INT64(229), INT32(65));
#line 229 "../lib/Chai.chpl"
    (_args_forcoforall_fn_chpl)->_0_numChunks = numChunks_chpl;
#line 229 "../lib/Chai.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl, INT64(229), INT32(65));
#line 229 "../lib/Chai.chpl"
    (_args_forcoforall_fn_chpl)->_1_parDim = parDim_chpl;
#line 229 "../lib/Chai.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl, INT64(229), INT32(65));
#line 229 "../lib/Chai.chpl"
    *((_args_forcoforall_fn_chpl)->_2_coerce_tmp + INT64(0)) = *(ret_tmp_chpl14 + INT64(0));
#line 229 "../lib/Chai.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl, INT64(229), INT32(65));
#line 229 "../lib/Chai.chpl"
    (_args_forcoforall_fn_chpl)->_3__coforallCount = _coforallCount_chpl;
#line 229 "../lib/Chai.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl, INT64(229), INT32(65));
#line 229 "../lib/Chai.chpl"
    (_args_forcoforall_fn_chpl)->_4_chunk = i_chpl;
#line 229 "../lib/Chai.chpl"
    tmp_chpl2 = this_chpl31;
#line 229 "../lib/Chai.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl, INT64(229), INT32(65));
#line 229 "../lib/Chai.chpl"
    (_args_forcoforall_fn_chpl)->_6_this = tmp_chpl2;
#line 229 "../lib/Chai.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl, INT64(229), INT32(65));
#line 229 "../lib/Chai.chpl"
    (_args_forcoforall_fn_chpl)->_7_batch = *(batch_chpl);
#line 229 "../lib/Chai.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl, INT64(229), INT32(65));
#line 229 "../lib/Chai.chpl"
    (_args_forcoforall_fn_chpl)->_8_convs = convs_chpl;
#line 229 "../lib/Chai.chpl"
    tmp_chpl3 = &call_tmp_chpl11;
#line 229 "../lib/Chai.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl, INT64(229), INT32(65));
#line 229 "../lib/Chai.chpl"
    (_args_forcoforall_fn_chpl)->_9_call_tmp = tmp_chpl3;
#line 229 "../lib/Chai.chpl"
    _args_vforcoforall_fn_chpl = ((void*)(_args_forcoforall_fn_chpl));
#line 229 "../lib/Chai.chpl"
    /*** wrapcoforall_fn_chpl99 ***/ chpl_taskAddCoStmt(chpl_task_getRequestedSubloc(), INT64(98), ((chpl_task_bundle_p)(_args_vforcoforall_fn_chpl)), sizeof(chpl__class_localscoforall_fn_chpl99_object), INT64(229), INT32(65));
#line 229 "../lib/Chai.chpl"
  }
#line 229 "../lib/Chai.chpl"
  error_chpl3 = NULL;
#line 229 "../lib/Chai.chpl"
  _waitEndCount_chpl2(_coforallCount_chpl, numTasks_chpl, &error_chpl3, INT64(229), INT32(65));
#line 229 "../lib/Chai.chpl"
  if (error_chpl3 != nil) /* ZLINE: 229 ../lib/Chai.chpl */
#line 229 "../lib/Chai.chpl"
  {
#line 229 "../lib/Chai.chpl"
    chpl__delete72(_coforallCount_chpl, INT64(229), INT32(65));
#line 229 "../lib/Chai.chpl"
    error_chpl2 = error_chpl3;
#line 229 "../lib/Chai.chpl"
    goto forall_IBB_break_label_chpl;
#line 229 "../lib/Chai.chpl"
  }
#line 229 "../lib/Chai.chpl"
  chpl__delete72(_coforallCount_chpl, INT64(229), INT32(65));
#line 229 "../lib/Chai.chpl"
  chpl_after_forall_fence();
#line 229 "../lib/Chai.chpl"
  forall_IBB_break_label_chpl:;
#line 229 "../lib/Chai.chpl"
  if (error_chpl2 != nil) /* ZLINE: 229 ../lib/Chai.chpl */
#line 229 "../lib/Chai.chpl"
  {
#line 229 "../lib/Chai.chpl"
    error_chpl = error_chpl2;
#line 229 "../lib/Chai.chpl"
    goto forall_IBB_break_label_chpl2;
#line 229 "../lib/Chai.chpl"
  }
#line 229 "../lib/Chai.chpl"
  chpl_after_forall_fence();
#line 229 "../lib/Chai.chpl"
  forall_IBB_break_label_chpl2:;
#line 229 "../lib/Chai.chpl"
  if (error_chpl != nil) /* ZLINE: 229 ../lib/Chai.chpl */
#line 229 "../lib/Chai.chpl"
  {
#line 229 "../lib/Chai.chpl"
    chpl_propagate_error(error_chpl, INT64(229), INT32(65));
#line 229 "../lib/Chai.chpl"
  }
#line 229 "../lib/Chai.chpl"
  chpl_after_forall_fence();
#line 232 "../lib/Chai.chpl"
  chpl__checkRetEltTypeMatch2(&convs_chpl);
#line 232 "../lib/Chai.chpl"
  chpl__autoDestroy11(&call_tmp_chpl9, INT64(232), INT32(65));
#line 226 "../lib/Chai.chpl"
  *(_retArg_chpl) = convs_chpl;
#line 226 "../lib/Chai.chpl"
  return;
#line 226 "../lib/Chai.chpl"
}

#line 229 "../lib/Chai.chpl"
/* Chai.chpl:229 */
#line 229 "../lib/Chai.chpl"
static void wrapcoforall_fn_chpl99(_class_localscoforall_fn_chpl99 c_chpl) {
#line 229 "../lib/Chai.chpl"
  Error_chpl error_chpl = NULL;
#line 229 "../lib/Chai.chpl"
  int64_t _0_numChunks_chpl;
#line 229 "../lib/Chai.chpl"
  int64_t _1_parDim_chpl;
#line 229 "../lib/Chai.chpl"
  _tuple_1_star_range_int64_t_both_one_chpl _2_coerce_tmp_chpl;
#line 229 "../lib/Chai.chpl"
  chpl___EndCount_AtomicT_int64_t_int64_t _3__coforallCount_chpl = NULL;
#line 229 "../lib/Chai.chpl"
  int64_t _4_chunk_chpl;
#line 229 "../lib/Chai.chpl"
  _ref_Conv_chpl _6_this_chpl = NULL;
#line 229 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl _7_batch_chpl;
#line 229 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl _8_convs_chpl;
#line 229 "../lib/Chai.chpl"
  _ref_range_int64_t_low_one_chpl _9_call_tmp_chpl = NULL;
#line 229 "../lib/Chai.chpl"
  chpl_rmem_consist_acquire(INT64(229), INT32(65));
#line 229 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(229), INT32(65));
#line 229 "../lib/Chai.chpl"
  _0_numChunks_chpl = (c_chpl)->_0_numChunks;
#line 229 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(229), INT32(65));
#line 229 "../lib/Chai.chpl"
  _1_parDim_chpl = (c_chpl)->_1_parDim;
#line 229 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(229), INT32(65));
#line 229 "../lib/Chai.chpl"
  *(_2_coerce_tmp_chpl + INT64(0)) = *((c_chpl)->_2_coerce_tmp + INT64(0));
#line 229 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(229), INT32(65));
#line 229 "../lib/Chai.chpl"
  _3__coforallCount_chpl = (c_chpl)->_3__coforallCount;
#line 229 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(229), INT32(65));
#line 229 "../lib/Chai.chpl"
  _4_chunk_chpl = (c_chpl)->_4_chunk;
#line 229 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(229), INT32(65));
#line 229 "../lib/Chai.chpl"
  _6_this_chpl = (c_chpl)->_6_this;
#line 229 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(229), INT32(65));
#line 229 "../lib/Chai.chpl"
  _7_batch_chpl = (c_chpl)->_7_batch;
#line 229 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(229), INT32(65));
#line 229 "../lib/Chai.chpl"
  _8_convs_chpl = (c_chpl)->_8_convs;
#line 229 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(229), INT32(65));
#line 229 "../lib/Chai.chpl"
  _9_call_tmp_chpl = (c_chpl)->_9_call_tmp;
#line 229 "../lib/Chai.chpl"
  coforall_fn_chpl99(_0_numChunks_chpl, _1_parDim_chpl, &_2_coerce_tmp_chpl, _3__coforallCount_chpl, _4_chunk_chpl, &error_chpl, _6_this_chpl, &_7_batch_chpl, &_8_convs_chpl, _9_call_tmp_chpl);
#line 229 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(229), INT32(65));
#line 229 "../lib/Chai.chpl"
  _downEndCount_chpl((c_chpl)->_3__coforallCount, error_chpl, INT64(229), INT32(65));
#line 229 "../lib/Chai.chpl"
  return;
#line 229 "../lib/Chai.chpl"
}

#line 229 "../lib/Chai.chpl"
/* Chai.chpl:229 */
#line 229 "../lib/Chai.chpl"
static void coforall_fn_chpl99(int64_t numChunks_chpl,
#line 229 "../lib/Chai.chpl"
                               int64_t parDim_chpl,
#line 229 "../lib/Chai.chpl"
                               _tuple_1_star_range_int64_t_both_one_chpl * locBlock_chpl,
#line 229 "../lib/Chai.chpl"
                               chpl___EndCount_AtomicT_int64_t_int64_t _coforallCount_chpl,
#line 229 "../lib/Chai.chpl"
                               int64_t chunk_chpl,
#line 229 "../lib/Chai.chpl"
                               Error_chpl * error_out_chpl,
#line 229 "../lib/Chai.chpl"
                               Conv_chpl * this_chpl31,
#line 229 "../lib/Chai.chpl"
                               _array_DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl * batch_chpl,
#line 229 "../lib/Chai.chpl"
                               _array_DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl * convs_chpl,
#line 229 "../lib/Chai.chpl"
                               range_int64_t_low_one_chpl * call_tmp_chpl6) {
#line 229 "../lib/Chai.chpl"
  _tuple_1_star_range_int64_t_both_one_chpl followMe_chpl;
#line 229 "../lib/Chai.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl7 = NULL;
#line 229 "../lib/Chai.chpl"
  int64_t call_tmp_chpl8;
#line 229 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl3;
#line 229 "../lib/Chai.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl9 = NULL;
#line 229 "../lib/Chai.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl10 = NULL;
#line 229 "../lib/Chai.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl11 = NULL;
#line 229 "../lib/Chai.chpl"
  _tuple_2_star_int64_t_chpl call_tmp_chpl12;
#line 229 "../lib/Chai.chpl"
  _tuple_2_star_int64_t_chpl ret_tmp_chpl;
#line 229 "../lib/Chai.chpl"
  int64_t tmp_x0_chpl;
#line 229 "../lib/Chai.chpl"
  int64_t tmp_x1_chpl;
#line 229 "../lib/Chai.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl13 = NULL;
#line 229 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl14;
#line 229 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl2;
#line 229 "../lib/Chai.chpl"
  _tuple_1_star_range_int64_t_both_one_chpl _yield_expr_tmp__chpl;
#line 229 "../lib/Chai.chpl"
  _tuple_1_star_range_int64_t_both_one_chpl chpl_followThis;
#line 229 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl _ir_F0_this_chpl;
#line 229 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl _ic__F5_block_x0_chpl;
#line 229 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl _ic__F7_coerce_tmp_x0_chpl;
#line 229 "../lib/Chai.chpl"
  int64_t _ic__F8_i_chpl;
#line 229 "../lib/Chai.chpl"
  int64_t _ic__F9_end_chpl;
#line 229 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl coerce_tmp_chpl4;
#line 229 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl _ic__F0_this_chpl;
#line 229 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl _ic__F2_myFollowThis_chpl;
#line 229 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl _ic__F3_r_chpl;
#line 229 "../lib/Chai.chpl"
  int64_t _ic__F5_i_chpl;
#line 229 "../lib/Chai.chpl"
  DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl this_chpl32 = NULL;
#line 229 "../lib/Chai.chpl"
  DefaultRectangularDom_1_int64_t_one_chpl coerce_tmp_chpl5 = NULL;
#line 229 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl6;
#line 229 "../lib/Chai.chpl"
  DefaultRectangularDom_1_int64_t_one_chpl this_chpl33 = NULL;
#line 229 "../lib/Chai.chpl"
  uint64_t call_tmp_chpl15;
#line 229 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl7;
#line 229 "../lib/Chai.chpl"
  DefaultRectangularDom_1_int64_t_one_chpl this_chpl34 = NULL;
#line 229 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl8;
#line 229 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl9;
#line 229 "../lib/Chai.chpl"
  DefaultRectangularDom_1_int64_t_one_chpl this_chpl35 = NULL;
#line 229 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl10;
#line 229 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl11;
#line 229 "../lib/Chai.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl16 = NULL;
#line 229 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl this_chpl36;
#line 229 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl this_chpl37;
#line 229 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl tmp_chpl;
#line 229 "../lib/Chai.chpl"
  DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl ret_chpl = NULL;
#line 229 "../lib/Chai.chpl"
  DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl _ic__F0_this_chpl2 = NULL;
#line 229 "../lib/Chai.chpl"
  _tuple_1_star_int64_t_chpl ret_tmp_chpl3;
#line 229 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl tup_x0_chpl;
#line 229 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl elt_x0_chpl;
#line 229 "../lib/Chai.chpl"
  int64_t coerce_tmp_chpl12;
#line 229 "../lib/Chai.chpl"
  _ref__tuple_1_star_range_int64_t_both_one_chpl call_tmp_chpl17 = NULL;
#line 229 "../lib/Chai.chpl"
  chpl_bool tmp_chpl2;
#line 229 "../lib/Chai.chpl"
  _ref__tuple_1_star_range_int64_t_both_one_chpl call_tmp_chpl18 = NULL;
#line 229 "../lib/Chai.chpl"
  int64_t call_tmp_chpl19;
#line 229 "../lib/Chai.chpl"
  _ref__tuple_1_star_range_int64_t_both_one_chpl call_tmp_chpl20 = NULL;
#line 229 "../lib/Chai.chpl"
  int64_t call_tmp_chpl21;
#line 229 "../lib/Chai.chpl"
  _tuple_1_star_range_int64_t_both_one_chpl retcopy_chpl;
#line 229 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl _ic__F0_this_chpl3;
#line 229 "../lib/Chai.chpl"
  int64_t coerce_tmp_chpl13;
#line 229 "../lib/Chai.chpl"
  int64_t call_tmp_chpl22;
#line 229 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl this_chpl38;
#line 229 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl tmp_chpl3;
#line 229 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl tmp_chpl4;
#line 229 "../lib/Chai.chpl"
  _ref_range_int64_t_both_one_chpl tmp_chpl5 = NULL;
#line 229 "../lib/Chai.chpl"
  chpl_bool tmp_chpl6;
#line 229 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl tmp_chpl7;
#line 229 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl tmp_chpl8;
#line 229 "../lib/Chai.chpl"
  int64_t call_tmp_chpl23;
#line 229 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl tmp_chpl9;
#line 229 "../lib/Chai.chpl"
  int64_t call_tmp_chpl24;
#line 229 "../lib/Chai.chpl"
  _ref_range_int64_t_both_one_chpl tmp_chpl10 = NULL;
#line 229 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl14;
#line 229 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl _ic__F0_this_chpl4;
#line 229 "../lib/Chai.chpl"
  DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl this_chpl39 = NULL;
#line 229 "../lib/Chai.chpl"
  _ddata_Tensor_3__real64_chpl coerce_tmp_chpl15 = NULL;
#line 229 "../lib/Chai.chpl"
  _ref_Tensor_3__real64_chpl call_tmp_chpl25 = NULL;
#line 230 "../lib/Chai.chpl"
  _tuple_1_star_int64_t_chpl coerce_tmp_chpl16;
#line 230 "../lib/Chai.chpl"
  _ref__tuple_1_star_int64_t_chpl i_i_chpl = NULL;
#line 230 "../lib/Chai.chpl"
  DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl ret_chpl2 = NULL;
#line 230 "../lib/Chai.chpl"
  DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl coerce_tmp_chpl17 = NULL;
#line 230 "../lib/Chai.chpl"
  _tuple_1_star_int64_t_chpl _formal_tmp_in_ind_chpl;
#line 230 "../lib/Chai.chpl"
  int64_t _formal_tmp_ind_x0_chpl;
#line 230 "../lib/Chai.chpl"
  _ddata_Tensor_3__real64_chpl coerce_tmp_chpl18 = NULL;
#line 230 "../lib/Chai.chpl"
  _ref_Tensor_3__real64_chpl call_tmp_chpl26 = NULL;
#line 230 "../lib/Chai.chpl"
  Tensor_3__real64_chpl call_tmp_chpl27;
#line 230 "../lib/Chai.chpl"
  Tensor_3__real64_chpl ret_tmp_chpl4;
#line 230 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_3_int64_t_one__real64_int64_t_chpl _field_destructor_tmp__chpl;
#line 230 "../lib/Chai.chpl"
  _domain_DefaultRectangularDom_3_int64_t_one_chpl _field_destructor_tmp__chpl2;
#line 229 "../lib/Chai.chpl"
  int64_t tmp_chpl11;
#line 229 "../lib/Chai.chpl"
  int64_t tmp_chpl12;
#line 229 "../lib/Chai.chpl"
  int64_t tmp_chpl13;
#line 229 "../lib/Chai.chpl"
  *(followMe_chpl + INT64(0)) = *(*(locBlock_chpl) + INT64(0));
#line 229 "../lib/Chai.chpl"
  call_tmp_chpl7 = this_chpl20(locBlock_chpl, parDim_chpl, INT64(229), INT32(65));
#line 229 "../lib/Chai.chpl"
  coerce_tmp_chpl3 = *(call_tmp_chpl7);
#line 229 "../lib/Chai.chpl"
  call_tmp_chpl8 = sizeAs_chpl37(coerce_tmp_chpl3, INT64(229), INT32(65));
#line 229 "../lib/Chai.chpl"
  call_tmp_chpl9 = this_chpl20(locBlock_chpl, parDim_chpl, INT64(229), INT32(65));
#line 229 "../lib/Chai.chpl"
  call_tmp_chpl10 = this_chpl20(locBlock_chpl, parDim_chpl, INT64(229), INT32(65));
#line 229 "../lib/Chai.chpl"
  call_tmp_chpl11 = this_chpl20(locBlock_chpl, parDim_chpl, INT64(229), INT32(65));
#line 229 "../lib/Chai.chpl"
  _computeBlock_chpl(call_tmp_chpl8, numChunks_chpl, chunk_chpl, (call_tmp_chpl9)->_high, (call_tmp_chpl10)->_low, (call_tmp_chpl11)->_low, &ret_tmp_chpl, INT64(229), INT32(65));
#line 229 "../lib/Chai.chpl"
  *(call_tmp_chpl12 + INT64(0)) = *(ret_tmp_chpl + INT64(0));
#line 229 "../lib/Chai.chpl"
  *(call_tmp_chpl12 + INT64(1)) = *(ret_tmp_chpl + INT64(1));
#line 229 "../lib/Chai.chpl"
  tmp_x0_chpl = *(call_tmp_chpl12 + INT64(0));
#line 229 "../lib/Chai.chpl"
  tmp_x1_chpl = *(call_tmp_chpl12 + INT64(1));
#line 229 "../lib/Chai.chpl"
  call_tmp_chpl13 = this_chpl20(&followMe_chpl, parDim_chpl, INT64(229), INT32(65));
#line 229 "../lib/Chai.chpl"
  chpl_build_bounded_range6(tmp_x0_chpl, tmp_x1_chpl, &ret_tmp_chpl2);
#line 229 "../lib/Chai.chpl"
  call_tmp_chpl14 = ret_tmp_chpl2;
#line 229 "../lib/Chai.chpl"
  *(call_tmp_chpl13) = call_tmp_chpl14;
#line 229 "../lib/Chai.chpl"
  *(_yield_expr_tmp__chpl + INT64(0)) = *(followMe_chpl + INT64(0));
#line 229 "../lib/Chai.chpl"
  *(chpl_followThis + INT64(0)) = *(_yield_expr_tmp__chpl + INT64(0));
#line 229 "../lib/Chai.chpl"
  _ir_F0_this_chpl = *(batch_chpl);
#line 229 "../lib/Chai.chpl"
  coerce_tmp_chpl4 = *(call_tmp_chpl6);
#line 229 "../lib/Chai.chpl"
  _ic__F0_this_chpl = coerce_tmp_chpl4;
#line 229 "../lib/Chai.chpl"
  tmp_chpl = _ir_F0_this_chpl;
#line 229 "../lib/Chai.chpl"
  ret_chpl = (&tmp_chpl)->_instance;
#line 229 "../lib/Chai.chpl"
  _ic__F0_this_chpl2 = ret_chpl;
#line 229 "../lib/Chai.chpl"
  this_chpl32 = ret_chpl;
#line 229 "../lib/Chai.chpl"
  chpl_check_nil(this_chpl32, INT64(229), INT32(65));
#line 229 "../lib/Chai.chpl"
  coerce_tmp_chpl5 = (this_chpl32)->dom;
#line 229 "../lib/Chai.chpl"
  createTuple_chpl(INT64(0), &ret_tmp_chpl3);
#line 229 "../lib/Chai.chpl"
  init_chpl113(&elt_x0_chpl);
#line 229 "../lib/Chai.chpl"
  tup_x0_chpl = elt_x0_chpl;
#line 229 "../lib/Chai.chpl"
  _ic__F5_block_x0_chpl = tup_x0_chpl;
#line 229 "../lib/Chai.chpl"
  coerce_tmp_chpl6 = *(chpl_followThis + INT64(0));
#line 229 "../lib/Chai.chpl"
  coerce_tmp_chpl12 = (&coerce_tmp_chpl6)->_high;
#line 229 "../lib/Chai.chpl"
  this_chpl33 = coerce_tmp_chpl5;
#line 229 "../lib/Chai.chpl"
  chpl_check_nil(this_chpl33, INT64(229), INT32(65));
#line 229 "../lib/Chai.chpl"
  call_tmp_chpl17 = &((this_chpl33)->ranges);
#line 229 "../lib/Chai.chpl"
  coerce_tmp_chpl7 = *(*(call_tmp_chpl17) + INT64(0));
#line 229 "../lib/Chai.chpl"
  call_tmp_chpl15 = sizeAs_chpl36(coerce_tmp_chpl7, INT64(229), INT32(65));
#line 229 "../lib/Chai.chpl"
  if (! (coerce_tmp_chpl12 < INT64(0))) /* ZLINE: 229 ../lib/Chai.chpl */
#line 229 "../lib/Chai.chpl"
  {
#line 229 "../lib/Chai.chpl"
    tmp_chpl2 = (((uint64_t)(coerce_tmp_chpl12)) >= call_tmp_chpl15);
#line 229 "../lib/Chai.chpl"
  } else /* ZLINE: 229 ../lib/Chai.chpl */
#line 229 "../lib/Chai.chpl"
  {
#line 229 "../lib/Chai.chpl"
    tmp_chpl2 = UINT8(false);
#line 229 "../lib/Chai.chpl"
  }
#line 229 "../lib/Chai.chpl"
  if (tmp_chpl2) /* ZLINE: 229 ../lib/Chai.chpl */
#line 229 "../lib/Chai.chpl"
  {
#line 229 "../lib/Chai.chpl"
    boundsCheckHalt_chpl(&_str_literal_4152_chpl, INT64(229), INT32(65));
#line 229 "../lib/Chai.chpl"
  }
#line 229 "../lib/Chai.chpl"
  this_chpl34 = coerce_tmp_chpl5;
#line 229 "../lib/Chai.chpl"
  chpl_check_nil(this_chpl34, INT64(229), INT32(65));
#line 229 "../lib/Chai.chpl"
  call_tmp_chpl18 = &((this_chpl34)->ranges);
#line 229 "../lib/Chai.chpl"
  coerce_tmp_chpl8 = *(*(call_tmp_chpl18) + INT64(0));
#line 229 "../lib/Chai.chpl"
  coerce_tmp_chpl9 = *(chpl_followThis + INT64(0));
#line 229 "../lib/Chai.chpl"
  call_tmp_chpl19 = ((&coerce_tmp_chpl8)->_low + ((int64_t)(((&coerce_tmp_chpl9)->_low * INT64(1)))));
#line 229 "../lib/Chai.chpl"
  this_chpl35 = coerce_tmp_chpl5;
#line 229 "../lib/Chai.chpl"
  chpl_check_nil(this_chpl35, INT64(229), INT32(65));
#line 229 "../lib/Chai.chpl"
  call_tmp_chpl20 = &((this_chpl35)->ranges);
#line 229 "../lib/Chai.chpl"
  coerce_tmp_chpl10 = *(*(call_tmp_chpl20) + INT64(0));
#line 229 "../lib/Chai.chpl"
  coerce_tmp_chpl11 = *(chpl_followThis + INT64(0));
#line 229 "../lib/Chai.chpl"
  call_tmp_chpl21 = ((&coerce_tmp_chpl10)->_low + ((int64_t)(((&coerce_tmp_chpl11)->_high * INT64(1)))));
#line 229 "../lib/Chai.chpl"
  call_tmp_chpl16 = &_ic__F5_block_x0_chpl;
#line 229 "../lib/Chai.chpl"
  compilerAssert_chpl();
#line 229 "../lib/Chai.chpl"
  (call_tmp_chpl16)->_low = call_tmp_chpl19;
#line 229 "../lib/Chai.chpl"
  (call_tmp_chpl16)->_high = call_tmp_chpl21;
#line 229 "../lib/Chai.chpl"
  *(retcopy_chpl + INT64(0)) = _ic__F5_block_x0_chpl;
#line 229 "../lib/Chai.chpl"
  _ic__F7_coerce_tmp_x0_chpl = *(retcopy_chpl + INT64(0));
#line 229 "../lib/Chai.chpl"
  _ic__F0_this_chpl3 = _ic__F7_coerce_tmp_x0_chpl;
#line 229 "../lib/Chai.chpl"
  checkIfIterWillOverflow_chpl(_ic__F7_coerce_tmp_x0_chpl, UINT8(true), INT64(229), INT32(65));
#line 229 "../lib/Chai.chpl"
  _ic__F8_i_chpl = INT64(0);
#line 229 "../lib/Chai.chpl"
  this_chpl36 = _ic__F0_this_chpl3;
#line 229 "../lib/Chai.chpl"
  coerce_tmp_chpl13 = (&this_chpl36)->_low;
#line 229 "../lib/Chai.chpl"
  this_chpl37 = _ic__F0_this_chpl3;
#line 229 "../lib/Chai.chpl"
  _ic__F9_end_chpl = (&this_chpl37)->_high;
#line 229 "../lib/Chai.chpl"
  tmp_chpl3 = *(chpl_followThis + INT64(0));
#line 229 "../lib/Chai.chpl"
  _ic__F2_myFollowThis_chpl = tmp_chpl3;
#line 229 "../lib/Chai.chpl"
  tmp_chpl4 = tmp_chpl3;
#line 229 "../lib/Chai.chpl"
  if ((&tmp_chpl4)->_low > (&tmp_chpl4)->_high) /* ZLINE: 229 ../lib/Chai.chpl */
#line 229 "../lib/Chai.chpl"
  {
#line 229 "../lib/Chai.chpl"
    tmp_chpl6 = UINT8(false);
#line 229 "../lib/Chai.chpl"
  } else /* ZLINE: 229 ../lib/Chai.chpl */
#line 229 "../lib/Chai.chpl"
  {
#line 229 "../lib/Chai.chpl"
    tmp_chpl6 = UINT8(true);
#line 229 "../lib/Chai.chpl"
  }
#line 229 "../lib/Chai.chpl"
  if (! tmp_chpl6) /* ZLINE: 229 ../lib/Chai.chpl */
#line 229 "../lib/Chai.chpl"
  {
#line 229 "../lib/Chai.chpl"
    tmp_chpl7 = tmp_chpl3;
#line 229 "../lib/Chai.chpl"
    if (! ((&tmp_chpl7)->_low > (&tmp_chpl7)->_high)) /* ZLINE: 229 ../lib/Chai.chpl */
#line 229 "../lib/Chai.chpl"
    {
#line 229 "../lib/Chai.chpl"
      boundsCheckHalt_chpl(&_str_literal_633_chpl, INT64(229), INT32(65));
#line 229 "../lib/Chai.chpl"
    }
#line 229 "../lib/Chai.chpl"
  }
#line 229 "../lib/Chai.chpl"
  call_tmp_chpl22 = sizeAs_chpl37(tmp_chpl3, INT64(229), INT32(65));
#line 229 "../lib/Chai.chpl"
  if (call_tmp_chpl22 != INT64(0)) /* ZLINE: 229 ../lib/Chai.chpl */
#line 229 "../lib/Chai.chpl"
  {
#line 229 "../lib/Chai.chpl"
  }
#line 229 "../lib/Chai.chpl"
  tmp_chpl5 = &_ic__F3_r_chpl;
#line 229 "../lib/Chai.chpl"
  init_chpl113(tmp_chpl5);
#line 229 "../lib/Chai.chpl"
  if (call_tmp_chpl22 != INT64(0)) /* ZLINE: 229 ../lib/Chai.chpl */
#line 229 "../lib/Chai.chpl"
  {
#line 229 "../lib/Chai.chpl"
    tmp_chpl8 = _ic__F2_myFollowThis_chpl;
#line 229 "../lib/Chai.chpl"
    call_tmp_chpl23 = orderToIndex_chpl(coerce_tmp_chpl4, (&tmp_chpl8)->_low, INT64(229), INT32(65));
#line 229 "../lib/Chai.chpl"
    tmp_chpl9 = _ic__F2_myFollowThis_chpl;
#line 229 "../lib/Chai.chpl"
    call_tmp_chpl24 = orderToIndex_chpl(_ic__F0_this_chpl, (&tmp_chpl9)->_high, INT64(229), INT32(65));
#line 229 "../lib/Chai.chpl"
    tmp_chpl10 = &_ic__F3_r_chpl;
#line 229 "../lib/Chai.chpl"
    (tmp_chpl10)->_low = call_tmp_chpl23;
#line 229 "../lib/Chai.chpl"
    (tmp_chpl10)->_high = call_tmp_chpl24;
#line 229 "../lib/Chai.chpl"
    verifyAppropriateStide_chpl2(INT64(1), INT64(229), INT32(65));
#line 229 "../lib/Chai.chpl"
  }
#line 229 "../lib/Chai.chpl"
  coerce_tmp_chpl14 = _ic__F3_r_chpl;
#line 229 "../lib/Chai.chpl"
  _ic__F0_this_chpl4 = coerce_tmp_chpl14;
#line 229 "../lib/Chai.chpl"
  checkIfIterWillOverflow_chpl(coerce_tmp_chpl14, UINT8(true), INT64(229), INT32(65));
#line 229 "../lib/Chai.chpl"
  _ic__F5_i_chpl = INT64(0);
#line 229 "../lib/Chai.chpl"
  this_chpl38 = _ic__F0_this_chpl4;
#line 229 "../lib/Chai.chpl"
  for (_ic__F8_i_chpl = coerce_tmp_chpl13,_ic__F5_i_chpl = (&this_chpl38)->_low; (tmp_chpl11 = (_ic__F8_i_chpl <= _ic__F9_end_chpl),tmp_chpl11); tmp_chpl12 = _ic__F8_i_chpl,tmp_chpl12 += INT64(1),_ic__F8_i_chpl = tmp_chpl12,tmp_chpl13 = _ic__F5_i_chpl,tmp_chpl13 += INT64(1),_ic__F5_i_chpl = tmp_chpl13) {
#line 229 "../lib/Chai.chpl"
    this_chpl39 = _ic__F0_this_chpl2;
#line 229 "../lib/Chai.chpl"
    chpl_check_nil(this_chpl39, INT64(229), INT32(65));
#line 229 "../lib/Chai.chpl"
    coerce_tmp_chpl15 = (this_chpl39)->shiftedData;
#line 229 "../lib/Chai.chpl"
    call_tmp_chpl25 = (coerce_tmp_chpl15 + _ic__F8_i_chpl);
#line 230 "../lib/Chai.chpl"
    *(coerce_tmp_chpl16 + INT64(0)) = _ic__F5_i_chpl;
#line 230 "../lib/Chai.chpl"
    i_i_chpl = &coerce_tmp_chpl16;
#line 230 "../lib/Chai.chpl"
    ret_chpl2 = (convs_chpl)->_instance;
#line 230 "../lib/Chai.chpl"
    checkAccess_chpl10(convs_chpl, i_i_chpl, ret_chpl2, INT64(230), INT32(65));
#line 230 "../lib/Chai.chpl"
    coerce_tmp_chpl17 = ret_chpl2;
#line 230 "../lib/Chai.chpl"
    *(_formal_tmp_in_ind_chpl + INT64(0)) = *(coerce_tmp_chpl16 + INT64(0));
#line 230 "../lib/Chai.chpl"
    _formal_tmp_ind_x0_chpl = *(_formal_tmp_in_ind_chpl + INT64(0));
#line 230 "../lib/Chai.chpl"
    chpl_check_nil(coerce_tmp_chpl17, INT64(230), INT32(65));
#line 230 "../lib/Chai.chpl"
    coerce_tmp_chpl18 = (coerce_tmp_chpl17)->shiftedData;
#line 230 "../lib/Chai.chpl"
    call_tmp_chpl26 = (coerce_tmp_chpl18 + _formal_tmp_ind_x0_chpl);
#line 230 "../lib/Chai.chpl"
    forwardProp_chpl(this_chpl31, call_tmp_chpl25, &ret_tmp_chpl4);
#line 230 "../lib/Chai.chpl"
    call_tmp_chpl27 = ret_tmp_chpl4;
#line 230 "../lib/Chai.chpl"
    chpl___ASSIGN_24(call_tmp_chpl26, &call_tmp_chpl27);
#line 230 "../lib/Chai.chpl"
    _field_destructor_tmp__chpl = (&call_tmp_chpl27)->data;
#line 230 "../lib/Chai.chpl"
    chpl__autoDestroy28(&_field_destructor_tmp__chpl, INT64(230), INT32(65));
#line 230 "../lib/Chai.chpl"
    _field_destructor_tmp__chpl2 = (&call_tmp_chpl27)->_domain;
#line 230 "../lib/Chai.chpl"
    chpl__autoDestroy8(&_field_destructor_tmp__chpl2, INT64(230), INT32(65));
#line 230 "../lib/Chai.chpl"
  }
#line 229 "../lib/Chai.chpl"
  return;
#line 229 "../lib/Chai.chpl"
}

#line 235 "../lib/Chai.chpl"
/* Chai.chpl:235 */
#line 235 "../lib/Chai.chpl"
static void forwardProp_chpl(Conv_chpl * this_chpl31,
#line 235 "../lib/Chai.chpl"
                             Tensor_3__real64_chpl * images_chpl,
#line 235 "../lib/Chai.chpl"
                             Tensor_3__real64_chpl * _retArg_chpl) {
#line 235 "../lib/Chai.chpl"
  Tensor_3__real64_chpl ret_chpl;
#line 236 "../lib/Chai.chpl"
  _tuple_3_star_int64_t_chpl call_tmp_chpl6;
#line 236 "../lib/Chai.chpl"
  _tuple_3_star_int64_t_chpl ret_tmp_chpl;
#line 236 "../lib/Chai.chpl"
  int64_t tmp_x0_chpl;
#line 236 "../lib/Chai.chpl"
  int64_t tmp_x1_chpl;
#line 236 "../lib/Chai.chpl"
  int64_t tmp_x2_chpl;
#line 237 "../lib/Chai.chpl"
  _ref_Tensor_4__real64_chpl call_tmp_chpl7 = NULL;
#line 237 "../lib/Chai.chpl"
  _tuple_4_star_int64_t_chpl call_tmp_chpl8;
#line 237 "../lib/Chai.chpl"
  _tuple_4_star_int64_t_chpl ret_tmp_chpl2;
#line 237 "../lib/Chai.chpl"
  int64_t tmp_x0_chpl2;
#line 237 "../lib/Chai.chpl"
  int64_t tmp_x1_chpl2;
#line 237 "../lib/Chai.chpl"
  int64_t tmp_x2_chpl2;
#line 237 "../lib/Chai.chpl"
  int64_t tmp_x3_chpl;
#line 237 "../lib/Chai.chpl"
  int64_t inChannels_chpl;
#line 240 "../lib/Chai.chpl"
  _tuple_3_star_int64_t_chpl call_tmp_chpl9;
#line 240 "../lib/Chai.chpl"
  _tuple_3_star_int64_t_chpl ret_tmp_chpl3;
#line 241 "../lib/Chai.chpl"
  _ref_Tensor_4__real64_chpl call_tmp_chpl10 = NULL;
#line 241 "../lib/Chai.chpl"
  _tuple_4_star_int64_t_chpl call_tmp_chpl11;
#line 241 "../lib/Chai.chpl"
  _tuple_4_star_int64_t_chpl ret_tmp_chpl4;
#line 245 "../lib/Chai.chpl"
  Tensor_3__real64_chpl call_tmp_chpl12;
#line 245 "../lib/Chai.chpl"
  Tensor_3__real64_chpl initTemp_chpl;
#line 245 "../lib/Chai.chpl"
  Tensor_3__real64_chpl convs_chpl;
#line 246 "../lib/Chai.chpl"
  _tuple_2_star_int64_t_chpl call_tmp_chpl13;
#line 246 "../lib/Chai.chpl"
  _tuple_2_star_int64_t_chpl ret_tmp_chpl5;
#line 246 "../lib/Chai.chpl"
  _tuple_2_star_int64_t_chpl call_tmp_chpl14;
#line 246 "../lib/Chai.chpl"
  _tuple_2_star_int64_t_chpl ret_tmp_chpl6;
#line 246 "../lib/Chai.chpl"
  _tuple_2_star_int64_t_chpl call_tmp_chpl15;
#line 246 "../lib/Chai.chpl"
  _tuple_2_star_int64_t_chpl ret_tmp_chpl7;
#line 246 "../lib/Chai.chpl"
  int64_t tmp_x0_chpl3;
#line 246 "../lib/Chai.chpl"
  int64_t tmp_x1_chpl3;
#line 247 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl call_tmp_chpl16;
#line 247 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl ret_tmp_chpl8;
#line 247 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl17;
#line 247 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl9;
#line 247 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl call_tmp_chpl18;
#line 247 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl ret_tmp_chpl10;
#line 247 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl19;
#line 247 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl11;
#line 247 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl call_tmp_chpl20;
#line 247 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl ret_tmp_chpl12;
#line 247 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl21;
#line 247 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl13;
#line 247 "../lib/Chai.chpl"
  _domain_DefaultRectangularDom_3_int64_t_one_chpl call_tmp_chpl22;
#line 247 "../lib/Chai.chpl"
  _domain_DefaultRectangularDom_3_int64_t_one_chpl ret_tmp_chpl14;
#line 248 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl23;
#line 248 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl call_tmp_chpl24;
#line 248 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl ret_tmp_chpl15;
#line 248 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl16;
#line 248 "../lib/Chai.chpl"
  _ref_Tensor_4__real64_chpl call_tmp_chpl25 = NULL;
#line 248 "../lib/Chai.chpl"
  int64_t kh_chpl;
#line 248 "../lib/Chai.chpl"
  int64_t kw_chpl;
#line 248 "../lib/Chai.chpl"
  int64_t channels_chpl;
#line 248 "../lib/Chai.chpl"
  int64_t stride_svar_chpl;
#line 248 "../lib/Chai.chpl"
  int64_t padding_svar_chpl;
#line 248 "../lib/Chai.chpl"
  int64_t call_tmp_chpl26;
#line 248 "../lib/Chai.chpl"
  int64_t len_chpl;
#line 248 "../lib/Chai.chpl"
  int64_t numChunks_chpl;
#line 248 "../lib/Chai.chpl"
  int64_t tmp_chpl;
#line 248 "../lib/Chai.chpl"
  int64_t tmp_chpl2;
#line 248 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl tmpIter_chpl;
#line 248 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl call_tmp_chpl27;
#line 248 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl ret_tmp_chpl17;
#line 248 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl18;
#line 248 "../lib/Chai.chpl"
  chpl___EndCount_AtomicT_int64_t_int64_t _coforallCount_chpl = NULL;
#line 248 "../lib/Chai.chpl"
  chpl___EndCount_AtomicT_int64_t_int64_t new_temp_chpl = NULL;
#line 248 "../lib/Chai.chpl"
  int64_t numTasks_chpl;
#line 248 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl _ic__F0_this_chpl;
#line 248 "../lib/Chai.chpl"
  int64_t i_chpl;
#line 248 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl this_chpl32;
#line 248 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl this_chpl33;
#line 248 "../lib/Chai.chpl"
  int64_t end_chpl;
#line 248 "../lib/Chai.chpl"
  _class_localscoforall_fn_chpl100 _args_forcoforall_fn_chpl = NULL;
#line 248 "../lib/Chai.chpl"
  _ref_Tensor_3__real64_chpl tmp_chpl3 = NULL;
#line 248 "../lib/Chai.chpl"
  _ref_Tensor_4__real64_chpl tmp_chpl4 = NULL;
#line 248 "../lib/Chai.chpl"
  _ref_Tensor_3__real64_chpl tmp_chpl5 = NULL;
#line 248 "../lib/Chai.chpl"
  raw_c_void_ptr _args_vforcoforall_fn_chpl;
#line 248 "../lib/Chai.chpl"
  Error_chpl error_chpl = NULL;
#line 252 "../lib/Chai.chpl"
  _ref__array_DefaultRectangularArr_3_int64_t_one__real64_int64_t_chpl call_tmp_chpl28 = NULL;
#line 252 "../lib/Chai.chpl"
  chpl__class_localscoforall_fn_chpl100_object chpl_macro_tmp_102;
#line 236 "../lib/Chai.chpl"
  shape_chpl8(images_chpl, &ret_tmp_chpl);
#line 236 "../lib/Chai.chpl"
  *(call_tmp_chpl6 + INT64(0)) = *(ret_tmp_chpl + INT64(0));
#line 236 "../lib/Chai.chpl"
  *(call_tmp_chpl6 + INT64(1)) = *(ret_tmp_chpl + INT64(1));
#line 236 "../lib/Chai.chpl"
  *(call_tmp_chpl6 + INT64(2)) = *(ret_tmp_chpl + INT64(2));
#line 236 "../lib/Chai.chpl"
  tmp_x0_chpl = *(call_tmp_chpl6 + INT64(0));
#line 236 "../lib/Chai.chpl"
  tmp_x1_chpl = *(call_tmp_chpl6 + INT64(1));
#line 236 "../lib/Chai.chpl"
  tmp_x2_chpl = *(call_tmp_chpl6 + INT64(2));
#line 237 "../lib/Chai.chpl"
  call_tmp_chpl7 = &((this_chpl31)->filters);
#line 237 "../lib/Chai.chpl"
  shape_chpl5(call_tmp_chpl7, &ret_tmp_chpl2);
#line 237 "../lib/Chai.chpl"
  *(call_tmp_chpl8 + INT64(0)) = *(ret_tmp_chpl2 + INT64(0));
#line 237 "../lib/Chai.chpl"
  *(call_tmp_chpl8 + INT64(1)) = *(ret_tmp_chpl2 + INT64(1));
#line 237 "../lib/Chai.chpl"
  *(call_tmp_chpl8 + INT64(2)) = *(ret_tmp_chpl2 + INT64(2));
#line 237 "../lib/Chai.chpl"
  *(call_tmp_chpl8 + INT64(3)) = *(ret_tmp_chpl2 + INT64(3));
#line 237 "../lib/Chai.chpl"
  tmp_x0_chpl2 = *(call_tmp_chpl8 + INT64(0));
#line 237 "../lib/Chai.chpl"
  tmp_x1_chpl2 = *(call_tmp_chpl8 + INT64(1));
#line 237 "../lib/Chai.chpl"
  tmp_x2_chpl2 = *(call_tmp_chpl8 + INT64(2));
#line 237 "../lib/Chai.chpl"
  tmp_x3_chpl = *(call_tmp_chpl8 + INT64(3));
#line 237 "../lib/Chai.chpl"
  inChannels_chpl = tmp_x3_chpl;
#line 239 "../lib/Chai.chpl"
  if (tmp_x2_chpl != tmp_x3_chpl) /* ZLINE: 239 ../lib/Chai.chpl */
#line 239 "../lib/Chai.chpl"
  {
#line 240 "../lib/Chai.chpl"
    shape_chpl8(images_chpl, &ret_tmp_chpl3);
#line 240 "../lib/Chai.chpl"
    *(call_tmp_chpl9 + INT64(0)) = *(ret_tmp_chpl3 + INT64(0));
#line 240 "../lib/Chai.chpl"
    *(call_tmp_chpl9 + INT64(1)) = *(ret_tmp_chpl3 + INT64(1));
#line 240 "../lib/Chai.chpl"
    *(call_tmp_chpl9 + INT64(2)) = *(ret_tmp_chpl3 + INT64(2));
#line 240 "../lib/Chai.chpl"
    writeln_chpl2(&_str_literal_1699_chpl, &call_tmp_chpl9, INT64(240), INT32(65));
#line 241 "../lib/Chai.chpl"
    call_tmp_chpl10 = &((this_chpl31)->filters);
#line 241 "../lib/Chai.chpl"
    shape_chpl5(call_tmp_chpl10, &ret_tmp_chpl4);
#line 241 "../lib/Chai.chpl"
    *(call_tmp_chpl11 + INT64(0)) = *(ret_tmp_chpl4 + INT64(0));
#line 241 "../lib/Chai.chpl"
    *(call_tmp_chpl11 + INT64(1)) = *(ret_tmp_chpl4 + INT64(1));
#line 241 "../lib/Chai.chpl"
    *(call_tmp_chpl11 + INT64(2)) = *(ret_tmp_chpl4 + INT64(2));
#line 241 "../lib/Chai.chpl"
    *(call_tmp_chpl11 + INT64(3)) = *(ret_tmp_chpl4 + INT64(3));
#line 241 "../lib/Chai.chpl"
    writeln_chpl5(&_str_literal_1700_chpl, &call_tmp_chpl11, INT64(241), INT32(65));
#line 242 "../lib/Chai.chpl"
    err_chpl4(&_str_literal_1701_chpl);
#line 242 "../lib/Chai.chpl"
  }
#line 245 "../lib/Chai.chpl"
  init_chpl233(&initTemp_chpl);
#line 245 "../lib/Chai.chpl"
  call_tmp_chpl12 = initTemp_chpl;
#line 245 "../lib/Chai.chpl"
  convs_chpl = call_tmp_chpl12;
#line 246 "../lib/Chai.chpl"
  *(ret_tmp_chpl5 + INT64(0)) = tmp_x1_chpl2;
#line 246 "../lib/Chai.chpl"
  *(ret_tmp_chpl5 + INT64(1)) = tmp_x2_chpl2;
#line 246 "../lib/Chai.chpl"
  *(call_tmp_chpl13 + INT64(0)) = *(ret_tmp_chpl5 + INT64(0));
#line 246 "../lib/Chai.chpl"
  *(call_tmp_chpl13 + INT64(1)) = *(ret_tmp_chpl5 + INT64(1));
#line 246 "../lib/Chai.chpl"
  *(ret_tmp_chpl6 + INT64(0)) = tmp_x0_chpl;
#line 246 "../lib/Chai.chpl"
  *(ret_tmp_chpl6 + INT64(1)) = tmp_x1_chpl;
#line 246 "../lib/Chai.chpl"
  *(call_tmp_chpl14 + INT64(0)) = *(ret_tmp_chpl6 + INT64(0));
#line 246 "../lib/Chai.chpl"
  *(call_tmp_chpl14 + INT64(1)) = *(ret_tmp_chpl6 + INT64(1));
#line 246 "../lib/Chai.chpl"
  correlateShape_chpl(&call_tmp_chpl13, &call_tmp_chpl14, (this_chpl31)->stride, (this_chpl31)->padding, &ret_tmp_chpl7);
#line 246 "../lib/Chai.chpl"
  *(call_tmp_chpl15 + INT64(0)) = *(ret_tmp_chpl7 + INT64(0));
#line 246 "../lib/Chai.chpl"
  *(call_tmp_chpl15 + INT64(1)) = *(ret_tmp_chpl7 + INT64(1));
#line 246 "../lib/Chai.chpl"
  tmp_x0_chpl3 = *(call_tmp_chpl15 + INT64(0));
#line 246 "../lib/Chai.chpl"
  tmp_x1_chpl3 = *(call_tmp_chpl15 + INT64(1));
#line 247 "../lib/Chai.chpl"
  chpl_build_low_bounded_range2(INT64(0), &ret_tmp_chpl8);
#line 247 "../lib/Chai.chpl"
  call_tmp_chpl16 = ret_tmp_chpl8;
#line 247 "../lib/Chai.chpl"
  chpl___POUND_3(call_tmp_chpl16, tmp_x0_chpl3, &ret_tmp_chpl9, INT64(247), INT32(65));
#line 247 "../lib/Chai.chpl"
  call_tmp_chpl17 = ret_tmp_chpl9;
#line 247 "../lib/Chai.chpl"
  chpl_build_low_bounded_range2(INT64(0), &ret_tmp_chpl10);
#line 247 "../lib/Chai.chpl"
  call_tmp_chpl18 = ret_tmp_chpl10;
#line 247 "../lib/Chai.chpl"
  chpl___POUND_3(call_tmp_chpl18, tmp_x1_chpl3, &ret_tmp_chpl11, INT64(247), INT32(65));
#line 247 "../lib/Chai.chpl"
  call_tmp_chpl19 = ret_tmp_chpl11;
#line 247 "../lib/Chai.chpl"
  chpl_build_low_bounded_range2(INT64(0), &ret_tmp_chpl12);
#line 247 "../lib/Chai.chpl"
  call_tmp_chpl20 = ret_tmp_chpl12;
#line 247 "../lib/Chai.chpl"
  chpl___POUND_3(call_tmp_chpl20, tmp_x0_chpl2, &ret_tmp_chpl13, INT64(247), INT32(65));
#line 247 "../lib/Chai.chpl"
  call_tmp_chpl21 = ret_tmp_chpl13;
#line 247 "../lib/Chai.chpl"
  chpl__buildDomainExpr(call_tmp_chpl17, call_tmp_chpl19, call_tmp_chpl21, UINT8(true), &ret_tmp_chpl14, INT64(247), INT32(65));
#line 247 "../lib/Chai.chpl"
  call_tmp_chpl22 = ret_tmp_chpl14;
#line 247 "../lib/Chai.chpl"
  reshapeDomain_chpl3(&convs_chpl, &call_tmp_chpl22);
#line 247 "../lib/Chai.chpl"
  chpl__autoDestroy8(&call_tmp_chpl22, INT64(247), INT32(65));
#line 248 "../lib/Chai.chpl"
  chpl_build_low_bounded_range2(INT64(0), &ret_tmp_chpl15);
#line 248 "../lib/Chai.chpl"
  call_tmp_chpl24 = ret_tmp_chpl15;
#line 248 "../lib/Chai.chpl"
  chpl___POUND_3(call_tmp_chpl24, tmp_x0_chpl2, &ret_tmp_chpl16, INT64(248), INT32(65));
#line 248 "../lib/Chai.chpl"
  call_tmp_chpl23 = ret_tmp_chpl16;
#line 248 "../lib/Chai.chpl"
  call_tmp_chpl25 = &((this_chpl31)->filters);
#line 248 "../lib/Chai.chpl"
  kh_chpl = tmp_x1_chpl2;
#line 248 "../lib/Chai.chpl"
  kw_chpl = tmp_x2_chpl2;
#line 248 "../lib/Chai.chpl"
  channels_chpl = tmp_x2_chpl;
#line 248 "../lib/Chai.chpl"
  stride_svar_chpl = (this_chpl31)->stride;
#line 248 "../lib/Chai.chpl"
  padding_svar_chpl = (this_chpl31)->padding;
#line 248 "../lib/Chai.chpl"
  call_tmp_chpl26 = sizeAs_chpl37(call_tmp_chpl23, INT64(248), INT32(65));
#line 248 "../lib/Chai.chpl"
  len_chpl = call_tmp_chpl26;
#line 248 "../lib/Chai.chpl"
  if (chpl_task_getSerial()) /* ZLINE: 248 ../lib/Chai.chpl */
#line 248 "../lib/Chai.chpl"
  {
#line 248 "../lib/Chai.chpl"
    tmp_chpl = INT64(1);
#line 248 "../lib/Chai.chpl"
  } else /* ZLINE: 248 ../lib/Chai.chpl */
#line 248 "../lib/Chai.chpl"
  {
#line 248 "../lib/Chai.chpl"
    tmp_chpl2 = _computeNumChunks_chpl3(call_tmp_chpl26, INT64(248), INT32(65));
#line 248 "../lib/Chai.chpl"
    tmp_chpl = tmp_chpl2;
#line 248 "../lib/Chai.chpl"
  }
#line 248 "../lib/Chai.chpl"
  numChunks_chpl = tmp_chpl;
#line 248 "../lib/Chai.chpl"
  chpl_build_low_bounded_range2(INT64(0), &ret_tmp_chpl17);
#line 248 "../lib/Chai.chpl"
  call_tmp_chpl27 = ret_tmp_chpl17;
#line 248 "../lib/Chai.chpl"
  chpl___POUND_3(call_tmp_chpl27, tmp_chpl, &ret_tmp_chpl18, INT64(248), INT32(65));
#line 248 "../lib/Chai.chpl"
  tmpIter_chpl = ret_tmp_chpl18;
#line 248 "../lib/Chai.chpl"
  new_temp_chpl = _new_chpl17(INT64(248), INT32(65));
#line 248 "../lib/Chai.chpl"
  _coforallCount_chpl = new_temp_chpl;
#line 248 "../lib/Chai.chpl"
  numTasks_chpl = chpl_boundedCoforallSize(tmpIter_chpl, INT64(248), INT32(65));
#line 248 "../lib/Chai.chpl"
  _upEndCount_chpl(_coforallCount_chpl, numTasks_chpl, INT64(248), INT32(65));
#line 248 "../lib/Chai.chpl"
  chpl_resetTaskSpawn(numTasks_chpl, INT64(248), INT32(65));
#line 248 "../lib/Chai.chpl"
  _ic__F0_this_chpl = tmpIter_chpl;
#line 248 "../lib/Chai.chpl"
  checkIfIterWillOverflow_chpl(tmpIter_chpl, UINT8(true), INT64(248), INT32(65));
#line 248 "../lib/Chai.chpl"
  i_chpl = INT64(0);
#line 248 "../lib/Chai.chpl"
  this_chpl32 = _ic__F0_this_chpl;
#line 248 "../lib/Chai.chpl"
  this_chpl33 = _ic__F0_this_chpl;
#line 248 "../lib/Chai.chpl"
  end_chpl = (&this_chpl33)->_high;
#line 248 "../lib/Chai.chpl"
  for (i_chpl = (&this_chpl32)->_low; ((i_chpl <= end_chpl)); i_chpl += INT64(1)) {
#line 248 "../lib/Chai.chpl"
    _args_forcoforall_fn_chpl = ((_class_localscoforall_fn_chpl100)(&chpl_macro_tmp_102));
#line 248 "../lib/Chai.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl, INT64(248), INT32(65));
#line 248 "../lib/Chai.chpl"
    (_args_forcoforall_fn_chpl)->_0_len = len_chpl;
#line 248 "../lib/Chai.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl, INT64(248), INT32(65));
#line 248 "../lib/Chai.chpl"
    (_args_forcoforall_fn_chpl)->_1_numChunks = numChunks_chpl;
#line 248 "../lib/Chai.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl, INT64(248), INT32(65));
#line 248 "../lib/Chai.chpl"
    (_args_forcoforall_fn_chpl)->_2_call_tmp = call_tmp_chpl23;
#line 248 "../lib/Chai.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl, INT64(248), INT32(65));
#line 248 "../lib/Chai.chpl"
    (_args_forcoforall_fn_chpl)->_3__coforallCount = _coforallCount_chpl;
#line 248 "../lib/Chai.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl, INT64(248), INT32(65));
#line 248 "../lib/Chai.chpl"
    (_args_forcoforall_fn_chpl)->_4_chunk = i_chpl;
#line 248 "../lib/Chai.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl, INT64(248), INT32(65));
#line 248 "../lib/Chai.chpl"
    (_args_forcoforall_fn_chpl)->_5_kh = kh_chpl;
#line 248 "../lib/Chai.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl, INT64(248), INT32(65));
#line 248 "../lib/Chai.chpl"
    (_args_forcoforall_fn_chpl)->_6_kw = kw_chpl;
#line 248 "../lib/Chai.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl, INT64(248), INT32(65));
#line 248 "../lib/Chai.chpl"
    (_args_forcoforall_fn_chpl)->_7_channels = channels_chpl;
#line 248 "../lib/Chai.chpl"
    tmp_chpl3 = images_chpl;
#line 248 "../lib/Chai.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl, INT64(248), INT32(65));
#line 248 "../lib/Chai.chpl"
    (_args_forcoforall_fn_chpl)->_8_images = tmp_chpl3;
#line 248 "../lib/Chai.chpl"
    tmp_chpl4 = call_tmp_chpl25;
#line 248 "../lib/Chai.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl, INT64(248), INT32(65));
#line 248 "../lib/Chai.chpl"
    (_args_forcoforall_fn_chpl)->_9_filters_ref = tmp_chpl4;
#line 248 "../lib/Chai.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl, INT64(248), INT32(65));
#line 248 "../lib/Chai.chpl"
    (_args_forcoforall_fn_chpl)->_10_stride_svar = stride_svar_chpl;
#line 248 "../lib/Chai.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl, INT64(248), INT32(65));
#line 248 "../lib/Chai.chpl"
    (_args_forcoforall_fn_chpl)->_11_padding_svar = padding_svar_chpl;
#line 248 "../lib/Chai.chpl"
    tmp_chpl5 = &convs_chpl;
#line 248 "../lib/Chai.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl, INT64(248), INT32(65));
#line 248 "../lib/Chai.chpl"
    (_args_forcoforall_fn_chpl)->_12_convs = tmp_chpl5;
#line 248 "../lib/Chai.chpl"
    _args_vforcoforall_fn_chpl = ((void*)(_args_forcoforall_fn_chpl));
#line 248 "../lib/Chai.chpl"
    /*** wrapcoforall_fn_chpl100 ***/ chpl_taskAddCoStmt(chpl_task_getRequestedSubloc(), INT64(99), ((chpl_task_bundle_p)(_args_vforcoforall_fn_chpl)), sizeof(chpl__class_localscoforall_fn_chpl100_object), INT64(248), INT32(65));
#line 248 "../lib/Chai.chpl"
  }
#line 248 "../lib/Chai.chpl"
  error_chpl = NULL;
#line 248 "../lib/Chai.chpl"
  _waitEndCount_chpl2(_coforallCount_chpl, numTasks_chpl, &error_chpl, INT64(248), INT32(65));
#line 248 "../lib/Chai.chpl"
  if (error_chpl != nil) /* ZLINE: 248 ../lib/Chai.chpl */
#line 248 "../lib/Chai.chpl"
  {
#line 248 "../lib/Chai.chpl"
    chpl_propagate_error(error_chpl, INT64(248), INT32(65));
#line 248 "../lib/Chai.chpl"
  }
#line 248 "../lib/Chai.chpl"
  chpl__delete72(_coforallCount_chpl, INT64(248), INT32(65));
#line 248 "../lib/Chai.chpl"
  chpl_after_forall_fence();
#line 252 "../lib/Chai.chpl"
  call_tmp_chpl28 = &((&convs_chpl)->data);
#line 252 "../lib/Chai.chpl"
  chpl___SLASH__ASSIGN_2(call_tmp_chpl28, ((_real64)(inChannels_chpl)), INT64(252), INT32(65));
#line 253 "../lib/Chai.chpl"
  ret_chpl = convs_chpl;
#line 235 "../lib/Chai.chpl"
  *(_retArg_chpl) = ret_chpl;
#line 235 "../lib/Chai.chpl"
  return;
#line 235 "../lib/Chai.chpl"
}

#line 248 "../lib/Chai.chpl"
/* Chai.chpl:248 */
#line 248 "../lib/Chai.chpl"
static void wrapcoforall_fn_chpl100(_class_localscoforall_fn_chpl100 c_chpl) {
#line 248 "../lib/Chai.chpl"
  int64_t _0_len_chpl;
#line 248 "../lib/Chai.chpl"
  int64_t _1_numChunks_chpl;
#line 248 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl _2_call_tmp_chpl;
#line 248 "../lib/Chai.chpl"
  chpl___EndCount_AtomicT_int64_t_int64_t _3__coforallCount_chpl = NULL;
#line 248 "../lib/Chai.chpl"
  int64_t _4_chunk_chpl;
#line 248 "../lib/Chai.chpl"
  int64_t _5_kh_chpl;
#line 248 "../lib/Chai.chpl"
  int64_t _6_kw_chpl;
#line 248 "../lib/Chai.chpl"
  int64_t _7_channels_chpl;
#line 248 "../lib/Chai.chpl"
  _ref_Tensor_3__real64_chpl _8_images_chpl = NULL;
#line 248 "../lib/Chai.chpl"
  _ref_Tensor_4__real64_chpl _9_filters_ref_chpl = NULL;
#line 248 "../lib/Chai.chpl"
  int64_t _10_stride_svar_chpl;
#line 248 "../lib/Chai.chpl"
  int64_t _11_padding_svar_chpl;
#line 248 "../lib/Chai.chpl"
  _ref_Tensor_3__real64_chpl _12_convs_chpl = NULL;
#line 248 "../lib/Chai.chpl"
  chpl_rmem_consist_acquire(INT64(248), INT32(65));
#line 248 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(248), INT32(65));
#line 248 "../lib/Chai.chpl"
  _0_len_chpl = (c_chpl)->_0_len;
#line 248 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(248), INT32(65));
#line 248 "../lib/Chai.chpl"
  _1_numChunks_chpl = (c_chpl)->_1_numChunks;
#line 248 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(248), INT32(65));
#line 248 "../lib/Chai.chpl"
  _2_call_tmp_chpl = (c_chpl)->_2_call_tmp;
#line 248 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(248), INT32(65));
#line 248 "../lib/Chai.chpl"
  _3__coforallCount_chpl = (c_chpl)->_3__coforallCount;
#line 248 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(248), INT32(65));
#line 248 "../lib/Chai.chpl"
  _4_chunk_chpl = (c_chpl)->_4_chunk;
#line 248 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(248), INT32(65));
#line 248 "../lib/Chai.chpl"
  _5_kh_chpl = (c_chpl)->_5_kh;
#line 248 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(248), INT32(65));
#line 248 "../lib/Chai.chpl"
  _6_kw_chpl = (c_chpl)->_6_kw;
#line 248 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(248), INT32(65));
#line 248 "../lib/Chai.chpl"
  _7_channels_chpl = (c_chpl)->_7_channels;
#line 248 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(248), INT32(65));
#line 248 "../lib/Chai.chpl"
  _8_images_chpl = (c_chpl)->_8_images;
#line 248 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(248), INT32(65));
#line 248 "../lib/Chai.chpl"
  _9_filters_ref_chpl = (c_chpl)->_9_filters_ref;
#line 248 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(248), INT32(65));
#line 248 "../lib/Chai.chpl"
  _10_stride_svar_chpl = (c_chpl)->_10_stride_svar;
#line 248 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(248), INT32(65));
#line 248 "../lib/Chai.chpl"
  _11_padding_svar_chpl = (c_chpl)->_11_padding_svar;
#line 248 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(248), INT32(65));
#line 248 "../lib/Chai.chpl"
  _12_convs_chpl = (c_chpl)->_12_convs;
#line 248 "../lib/Chai.chpl"
  coforall_fn_chpl100(_0_len_chpl, _1_numChunks_chpl, _2_call_tmp_chpl, _3__coforallCount_chpl, _4_chunk_chpl, _5_kh_chpl, _6_kw_chpl, _7_channels_chpl, _8_images_chpl, _9_filters_ref_chpl, _10_stride_svar_chpl, _11_padding_svar_chpl, _12_convs_chpl);
#line 248 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(248), INT32(65));
#line 248 "../lib/Chai.chpl"
  _downEndCount_chpl((c_chpl)->_3__coforallCount, nil, INT64(248), INT32(65));
#line 248 "../lib/Chai.chpl"
  return;
#line 248 "../lib/Chai.chpl"
}

#line 248 "../lib/Chai.chpl"
/* Chai.chpl:248 */
#line 248 "../lib/Chai.chpl"
static void coforall_fn_chpl100(int64_t len_chpl,
#line 248 "../lib/Chai.chpl"
                                int64_t numChunks_chpl,
#line 248 "../lib/Chai.chpl"
                                range_int64_t_both_one_chpl this_chpl31,
#line 248 "../lib/Chai.chpl"
                                chpl___EndCount_AtomicT_int64_t_int64_t _coforallCount_chpl,
#line 248 "../lib/Chai.chpl"
                                int64_t chunk_chpl,
#line 248 "../lib/Chai.chpl"
                                int64_t kh_chpl,
#line 248 "../lib/Chai.chpl"
                                int64_t kw_chpl,
#line 248 "../lib/Chai.chpl"
                                int64_t channels_chpl,
#line 248 "../lib/Chai.chpl"
                                Tensor_3__real64_chpl * images_chpl,
#line 248 "../lib/Chai.chpl"
                                Tensor_4__real64_chpl * filters_svar_chpl,
#line 248 "../lib/Chai.chpl"
                                int64_t stride_svar_chpl,
#line 248 "../lib/Chai.chpl"
                                int64_t padding_svar_chpl,
#line 248 "../lib/Chai.chpl"
                                Tensor_3__real64_chpl * convs_chpl) {
#line 249 "../lib/Chai.chpl"
  int64_t local_dataParMinGranularity_chpl;
#line 249 "../lib/Chai.chpl"
  chpl_bool local_dataParIgnoreRunningTasks_chpl;
#line 249 "../lib/Chai.chpl"
  int64_t local_dataParTasksPerLocale_chpl;
#line 248 "../lib/Chai.chpl"
  Tensor_3__real64_chpl filter_chpl;
#line 248 "../lib/Chai.chpl"
  Tensor_3__real64_chpl call_tmp_chpl6;
#line 248 "../lib/Chai.chpl"
  Tensor_3__real64_chpl ret_tmp_chpl;
#line 248 "../lib/Chai.chpl"
  _tuple_2_star_int64_t_chpl call_tmp_chpl7;
#line 248 "../lib/Chai.chpl"
  _tuple_2_star_int64_t_chpl ret_tmp_chpl2;
#line 248 "../lib/Chai.chpl"
  int64_t tmp_x0_chpl;
#line 248 "../lib/Chai.chpl"
  int64_t tmp_x1_chpl;
#line 248 "../lib/Chai.chpl"
  int64_t _ic__F1_high_chpl;
#line 248 "../lib/Chai.chpl"
  int64_t i_chpl;
#line 248 "../lib/Chai.chpl"
  int64_t f_chpl;
#line 249 "../lib/Chai.chpl"
  _ref__array_DefaultRectangularArr_3_int64_t_one__real64_int64_t_chpl call_tmp_chpl8 = NULL;
#line 249 "../lib/Chai.chpl"
  range_int64_t_neither_one_chpl call_tmp_chpl9;
#line 249 "../lib/Chai.chpl"
  range_int64_t_neither_one_chpl ret_tmp_chpl3;
#line 249 "../lib/Chai.chpl"
  range_int64_t_neither_one_chpl call_tmp_chpl10;
#line 249 "../lib/Chai.chpl"
  range_int64_t_neither_one_chpl ret_tmp_chpl4;
#line 249 "../lib/Chai.chpl"
  range_int64_t_neither_one_chpl call_tmp_chpl11;
#line 249 "../lib/Chai.chpl"
  range_int64_t_neither_one_chpl ret_tmp_chpl5;
#line 249 "../lib/Chai.chpl"
  _array_ArrayViewRankChangeArr__real64_int64_t_ArrayViewRankChangeDom_3_int64_t_one__tuple_4_star_chpl_bool__tuple_4_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_4_star_chpl_bool__tuple_4_star_int64_t_int64_t_DefaultRectangularArr_4_int64_t_one__real64_int64_t__tuple_4_star_chpl_bool__tuple_4_star_int64_t__remoteAccessData__real64_3_int64_t_one_T_F_chpl call_tmp_chpl12;
#line 98 "../lib/Tensor.chpl"
  _ref__array_DefaultRectangularArr_4_int64_t_one__real64_int64_t_chpl call_tmp_chpl13 = NULL;
#line 249 "../lib/Chai.chpl"
  _array_ArrayViewRankChangeArr__real64_int64_t_ArrayViewRankChangeDom_3_int64_t_one__tuple_4_star_chpl_bool__tuple_4_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_4_star_chpl_bool__tuple_4_star_int64_t_int64_t_DefaultRectangularArr_4_int64_t_one__real64_int64_t__tuple_4_star_chpl_bool__tuple_4_star_int64_t__remoteAccessData__real64_3_int64_t_one_T_F_chpl ret_tmp_chpl6;
#line 249 "../lib/Chai.chpl"
  _ref__array_ArrayViewRankChangeArr__real64_int64_t_ArrayViewRankChangeDom_3_int64_t_one__tuple_4_star_chpl_bool__tuple_4_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_4_star_chpl_bool__tuple_4_star_int64_t_int64_t_DefaultRectangularArr_4_int64_t_one__real64_int64_t__tuple_4_star_chpl_bool__tuple_4_star_int64_t__remoteAccessData__real64_3_int64_t_one_T_F_chpl i_b_chpl = NULL;
#line 249 "../lib/Chai.chpl"
  uint64_t call_tmp_chpl14;
#line 249 "../lib/Chai.chpl"
  chpl_bool tmp_chpl;
#line 249 "../lib/Chai.chpl"
  chpl_bool tmp_chpl2;
#line 249 "../lib/Chai.chpl"
  uint64_t call_tmp_chpl15;
#line 249 "../lib/Chai.chpl"
  chpl_bool tmp_chpl3;
#line 249 "../lib/Chai.chpl"
  _domain_DefaultRectangularDom_3_int64_t_one_chpl call_tmp_chpl16;
#line 249 "../lib/Chai.chpl"
  _domain_DefaultRectangularDom_3_int64_t_one_chpl ret_tmp_chpl7;
#line 249 "../lib/Chai.chpl"
  _domain_ArrayViewRankChangeDom_3_int64_t_one__tuple_4_star_chpl_bool__tuple_4_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_4_star_chpl_bool__tuple_4_star_int64_t_chpl call_tmp_chpl17;
#line 249 "../lib/Chai.chpl"
  _domain_ArrayViewRankChangeDom_3_int64_t_one__tuple_4_star_chpl_bool__tuple_4_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_4_star_chpl_bool__tuple_4_star_int64_t_chpl ret_tmp_chpl8;
#line 249 "../lib/Chai.chpl"
  _ref__domain_DefaultRectangularDom_3_int64_t_one_chpl i_AD_chpl = NULL;
#line 249 "../lib/Chai.chpl"
  _ref__domain_ArrayViewRankChangeDom_3_int64_t_one__tuple_4_star_chpl_bool__tuple_4_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_4_star_chpl_bool__tuple_4_star_int64_t_chpl i_BD_chpl = NULL;
#line 249 "../lib/Chai.chpl"
  DefaultRectangularArr_3_int64_t_one__real64_int64_t_chpl ret_chpl = NULL;
#line 249 "../lib/Chai.chpl"
  ArrayViewRankChangeArr__real64_int64_t_ArrayViewRankChangeDom_3_int64_t_one__tuple_4_star_chpl_bool__tuple_4_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_4_star_chpl_bool__tuple_4_star_int64_t_int64_t_DefaultRectangularArr_4_int64_t_one__real64_int64_t__tuple_4_star_chpl_bool__tuple_4_star_int64_t__remoteAccessData__real64_3_int64_t_one_T_F_chpl ret_chpl2 = NULL;
#line 249 "../lib/Chai.chpl"
  chpl_bool call_tmp_chpl18;
#line 249 "../lib/Chai.chpl"
  DefaultRectangularArr_3_int64_t_one__real64_int64_t_chpl coerce_tmp_chpl3 = NULL;
#line 249 "../lib/Chai.chpl"
  int64_t tasksPerLocale_chpl;
#line 249 "../lib/Chai.chpl"
  chpl_bool ignoreRunning_chpl;
#line 249 "../lib/Chai.chpl"
  int64_t minIndicesPerTask_chpl;
#line 249 "../lib/Chai.chpl"
  Error_chpl error_chpl = NULL;
#line 249 "../lib/Chai.chpl"
  DefaultRectangularDom_3_int64_t_one_chpl coerce_tmp_chpl4 = NULL;
#line 249 "../lib/Chai.chpl"
  int64_t offset_x0_chpl;
#line 249 "../lib/Chai.chpl"
  int64_t offset_x1_chpl;
#line 249 "../lib/Chai.chpl"
  int64_t offset_x2_chpl;
#line 249 "../lib/Chai.chpl"
  _tuple_3_star_int64_t_chpl ret_tmp_chpl9;
#line 249 "../lib/Chai.chpl"
  Error_chpl error_chpl2 = NULL;
#line 249 "../lib/Chai.chpl"
  locale_chpl call_tmp_chpl19;
#line 249 "../lib/Chai.chpl"
  chpl_localeID_t call_tmp_chpl20;
#line 249 "../lib/Chai.chpl"
  int32_t call_tmp_chpl21;
#line 249 "../lib/Chai.chpl"
  chpl_localeID_t call_tmp_chpl22;
#line 249 "../lib/Chai.chpl"
  locale_chpl call_tmp_chpl23;
#line 249 "../lib/Chai.chpl"
  locale_chpl ret_tmp_chpl10;
#line 249 "../lib/Chai.chpl"
  BaseLocale_chpl coerce_tmp_chpl5 = NULL;
#line 249 "../lib/Chai.chpl"
  _ref_locale_chpl i_x_chpl = NULL;
#line 249 "../lib/Chai.chpl"
  int64_t tmp_chpl4;
#line 249 "../lib/Chai.chpl"
  locale_chpl call_tmp_chpl24;
#line 249 "../lib/Chai.chpl"
  chpl_localeID_t call_tmp_chpl25;
#line 249 "../lib/Chai.chpl"
  int32_t call_tmp_chpl26;
#line 249 "../lib/Chai.chpl"
  chpl_localeID_t call_tmp_chpl27;
#line 249 "../lib/Chai.chpl"
  locale_chpl call_tmp_chpl28;
#line 249 "../lib/Chai.chpl"
  locale_chpl ret_tmp_chpl11;
#line 249 "../lib/Chai.chpl"
  int64_t ret_chpl3;
#line 249 "../lib/Chai.chpl"
  BaseLocale_chpl coerce_tmp_chpl6 = NULL;
#line 249 "../lib/Chai.chpl"
  BaseLocale_chpl coerce_tmp_chpl7 = NULL;
#line 249 "../lib/Chai.chpl"
  _ref_locale_chpl i_x_chpl2 = NULL;
#line 249 "../lib/Chai.chpl"
  int64_t tmp_x0_chpl2;
#line 249 "../lib/Chai.chpl"
  int64_t tmp_x1_chpl2;
#line 249 "../lib/Chai.chpl"
  _ref__tuple_3_star_range_int64_t_both_one_chpl call_tmp_chpl29 = NULL;
#line 249 "../lib/Chai.chpl"
  _tuple_2_star_int64_t_chpl ret_tmp_chpl12;
#line 249 "../lib/Chai.chpl"
  int64_t numChunks_chpl2;
#line 249 "../lib/Chai.chpl"
  int64_t parDim_chpl;
#line 249 "../lib/Chai.chpl"
  _tuple_3_star_range_int64_t_both_one_chpl locBlock_chpl;
#line 249 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl tup_x0_chpl;
#line 249 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl tup_x1_chpl;
#line 249 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl tup_x2_chpl;
#line 249 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl elt_x0_chpl;
#line 249 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl elt_x1_chpl;
#line 249 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl elt_x2_chpl;
#line 249 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl call_tmp_chpl30;
#line 249 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl ret_tmp_chpl13;
#line 249 "../lib/Chai.chpl"
  _ref__tuple_3_star_range_int64_t_both_one_chpl call_tmp_chpl31 = NULL;
#line 249 "../lib/Chai.chpl"
  int64_t call_tmp_chpl32;
#line 249 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl8;
#line 249 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl33;
#line 249 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl14;
#line 249 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl call_tmp_chpl34;
#line 249 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl ret_tmp_chpl15;
#line 249 "../lib/Chai.chpl"
  _ref__tuple_3_star_range_int64_t_both_one_chpl call_tmp_chpl35 = NULL;
#line 249 "../lib/Chai.chpl"
  int64_t call_tmp_chpl36;
#line 249 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl9;
#line 249 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl37;
#line 249 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl16;
#line 249 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl call_tmp_chpl38;
#line 249 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl ret_tmp_chpl17;
#line 249 "../lib/Chai.chpl"
  _ref__tuple_3_star_range_int64_t_both_one_chpl call_tmp_chpl39 = NULL;
#line 249 "../lib/Chai.chpl"
  int64_t call_tmp_chpl40;
#line 249 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl10;
#line 249 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl41;
#line 249 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl18;
#line 249 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl tmpIter_chpl;
#line 249 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl call_tmp_chpl42;
#line 249 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl ret_tmp_chpl19;
#line 249 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl20;
#line 249 "../lib/Chai.chpl"
  chpl___EndCount_AtomicT_int64_t_int64_t _coforallCount_chpl2 = NULL;
#line 249 "../lib/Chai.chpl"
  chpl___EndCount_AtomicT_int64_t_int64_t new_temp_chpl = NULL;
#line 249 "../lib/Chai.chpl"
  int64_t numTasks_chpl;
#line 249 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl _ic__F0_this_chpl;
#line 249 "../lib/Chai.chpl"
  int64_t i_chpl2;
#line 249 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl this_chpl32;
#line 249 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl this_chpl33;
#line 249 "../lib/Chai.chpl"
  int64_t end_chpl;
#line 249 "../lib/Chai.chpl"
  _class_localscoforall_fn_chpl36 _args_forcoforall_fn_chpl = NULL;
#line 249 "../lib/Chai.chpl"
  raw_c_void_ptr _args_vforcoforall_fn_chpl;
#line 249 "../lib/Chai.chpl"
  int64_t _args_sizecoforall_fn_chpl;
#line 249 "../lib/Chai.chpl"
  Error_chpl error_chpl3 = NULL;
#line 250 "../lib/Chai.chpl"
  range_int64_t_neither_one_chpl call_tmp_chpl43;
#line 250 "../lib/Chai.chpl"
  range_int64_t_neither_one_chpl ret_tmp_chpl21;
#line 250 "../lib/Chai.chpl"
  range_int64_t_neither_one_chpl call_tmp_chpl44;
#line 250 "../lib/Chai.chpl"
  range_int64_t_neither_one_chpl ret_tmp_chpl22;
#line 250 "../lib/Chai.chpl"
  _array_ArrayViewRankChangeArr__real64_int64_t_ArrayViewRankChangeDom_2_int64_t_one__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_DefaultRectangularArr_3_int64_t_one__real64_int64_t__tuple_3_star_chpl_bool__tuple_3_star_int64_t__remoteAccessData__real64_2_int64_t_one_T_F_chpl call_tmp_chpl45;
#line 98 "../lib/Tensor.chpl"
  _ref__array_DefaultRectangularArr_3_int64_t_one__real64_int64_t_chpl call_tmp_chpl46 = NULL;
#line 250 "../lib/Chai.chpl"
  _array_ArrayViewRankChangeArr__real64_int64_t_ArrayViewRankChangeDom_2_int64_t_one__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_DefaultRectangularArr_3_int64_t_one__real64_int64_t__tuple_3_star_chpl_bool__tuple_3_star_int64_t__remoteAccessData__real64_2_int64_t_one_T_F_chpl ret_tmp_chpl23;
#line 250 "../lib/Chai.chpl"
  Tensor_2__real64_chpl call_tmp_chpl47;
#line 250 "../lib/Chai.chpl"
  Tensor_2__real64_chpl ret_tmp_chpl24;
#line 250 "../lib/Chai.chpl"
  _ref__array_ArrayViewRankChangeArr__real64_int64_t_ArrayViewRankChangeDom_2_int64_t_one__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_DefaultRectangularArr_3_int64_t_one__real64_int64_t__tuple_3_star_chpl_bool__tuple_3_star_int64_t__remoteAccessData__real64_2_int64_t_one_T_F_chpl i_a_chpl = NULL;
#line 250 "../lib/Chai.chpl"
  ArrayViewRankChangeArr__real64_int64_t_ArrayViewRankChangeDom_2_int64_t_one__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_DefaultRectangularArr_3_int64_t_one__real64_int64_t__tuple_3_star_chpl_bool__tuple_3_star_int64_t__remoteAccessData__real64_2_int64_t_one_T_F_chpl coerce_tmp_chpl11 = NULL;
#line 250 "../lib/Chai.chpl"
  Error_chpl error_chpl4 = NULL;
#line 250 "../lib/Chai.chpl"
  ArrayViewRankChangeDom_2_int64_t_one__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_3_star_chpl_bool__tuple_3_star_int64_t_chpl ret_chpl4 = NULL;
#line 250 "../lib/Chai.chpl"
  ArrayViewRankChangeDom_2_int64_t_one__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_3_star_chpl_bool__tuple_3_star_int64_t_chpl coerce_tmp_chpl12 = NULL;
#line 250 "../lib/Chai.chpl"
  Error_chpl error_chpl5 = NULL;
#line 250 "../lib/Chai.chpl"
  DefaultRectangularDom_2_int64_t_one_chpl coerce_tmp_chpl13 = NULL;
#line 250 "../lib/Chai.chpl"
  DefaultRectangularDom_2_int64_t_one_chpl coerce_tmp_chpl14 = NULL;
#line 250 "../lib/Chai.chpl"
  int64_t tasksPerLocale_chpl2;
#line 250 "../lib/Chai.chpl"
  chpl_bool ignoreRunning_chpl2;
#line 250 "../lib/Chai.chpl"
  int64_t minIndicesPerTask_chpl2;
#line 250 "../lib/Chai.chpl"
  int64_t offset_x0_chpl2;
#line 250 "../lib/Chai.chpl"
  int64_t offset_x1_chpl2;
#line 250 "../lib/Chai.chpl"
  _tuple_2_star_int64_t_chpl ret_tmp_chpl25;
#line 250 "../lib/Chai.chpl"
  Error_chpl error_chpl6 = NULL;
#line 250 "../lib/Chai.chpl"
  locale_chpl call_tmp_chpl48;
#line 250 "../lib/Chai.chpl"
  chpl_localeID_t call_tmp_chpl49;
#line 250 "../lib/Chai.chpl"
  int32_t call_tmp_chpl50;
#line 250 "../lib/Chai.chpl"
  chpl_localeID_t call_tmp_chpl51;
#line 250 "../lib/Chai.chpl"
  locale_chpl call_tmp_chpl52;
#line 250 "../lib/Chai.chpl"
  locale_chpl ret_tmp_chpl26;
#line 250 "../lib/Chai.chpl"
  BaseLocale_chpl coerce_tmp_chpl15 = NULL;
#line 250 "../lib/Chai.chpl"
  _ref_locale_chpl i_x_chpl3 = NULL;
#line 250 "../lib/Chai.chpl"
  int64_t tmp_chpl5;
#line 250 "../lib/Chai.chpl"
  locale_chpl call_tmp_chpl53;
#line 250 "../lib/Chai.chpl"
  chpl_localeID_t call_tmp_chpl54;
#line 250 "../lib/Chai.chpl"
  int32_t call_tmp_chpl55;
#line 250 "../lib/Chai.chpl"
  chpl_localeID_t call_tmp_chpl56;
#line 250 "../lib/Chai.chpl"
  locale_chpl call_tmp_chpl57;
#line 250 "../lib/Chai.chpl"
  locale_chpl ret_tmp_chpl27;
#line 250 "../lib/Chai.chpl"
  int64_t ret_chpl5;
#line 250 "../lib/Chai.chpl"
  BaseLocale_chpl coerce_tmp_chpl16 = NULL;
#line 250 "../lib/Chai.chpl"
  BaseLocale_chpl coerce_tmp_chpl17 = NULL;
#line 250 "../lib/Chai.chpl"
  _ref_locale_chpl i_x_chpl4 = NULL;
#line 250 "../lib/Chai.chpl"
  int64_t tmp_x0_chpl3;
#line 250 "../lib/Chai.chpl"
  int64_t tmp_x1_chpl3;
#line 250 "../lib/Chai.chpl"
  _ref__tuple_2_star_range_int64_t_both_one_chpl call_tmp_chpl58 = NULL;
#line 250 "../lib/Chai.chpl"
  _tuple_2_star_int64_t_chpl ret_tmp_chpl28;
#line 250 "../lib/Chai.chpl"
  int64_t numChunks_chpl3;
#line 250 "../lib/Chai.chpl"
  int64_t parDim_chpl2;
#line 250 "../lib/Chai.chpl"
  _tuple_2_star_range_int64_t_both_one_chpl locBlock_chpl2;
#line 250 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl tup_x0_chpl2;
#line 250 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl tup_x1_chpl2;
#line 250 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl elt_x0_chpl2;
#line 250 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl elt_x1_chpl2;
#line 250 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl call_tmp_chpl59;
#line 250 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl ret_tmp_chpl29;
#line 250 "../lib/Chai.chpl"
  _ref__tuple_2_star_range_int64_t_both_one_chpl call_tmp_chpl60 = NULL;
#line 250 "../lib/Chai.chpl"
  int64_t call_tmp_chpl61;
#line 250 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl18;
#line 250 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl62;
#line 250 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl30;
#line 250 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl call_tmp_chpl63;
#line 250 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl ret_tmp_chpl31;
#line 250 "../lib/Chai.chpl"
  _ref__tuple_2_star_range_int64_t_both_one_chpl call_tmp_chpl64 = NULL;
#line 250 "../lib/Chai.chpl"
  int64_t call_tmp_chpl65;
#line 250 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl19;
#line 250 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl66;
#line 250 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl32;
#line 250 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl tmpIter_chpl2;
#line 250 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl call_tmp_chpl67;
#line 250 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl ret_tmp_chpl33;
#line 250 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl34;
#line 250 "../lib/Chai.chpl"
  chpl___EndCount_AtomicT_int64_t_int64_t _coforallCount_chpl3 = NULL;
#line 250 "../lib/Chai.chpl"
  chpl___EndCount_AtomicT_int64_t_int64_t new_temp_chpl2 = NULL;
#line 250 "../lib/Chai.chpl"
  int64_t numTasks_chpl2;
#line 250 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl _ic__F0_this_chpl2;
#line 250 "../lib/Chai.chpl"
  int64_t i_chpl3;
#line 250 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl this_chpl34;
#line 250 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl this_chpl35;
#line 250 "../lib/Chai.chpl"
  int64_t end_chpl2;
#line 250 "../lib/Chai.chpl"
  _class_localscoforall_fn_chpl27 _args_forcoforall_fn_chpl2 = NULL;
#line 250 "../lib/Chai.chpl"
  _ref_Tensor_2__real64_chpl tmp_chpl6 = NULL;
#line 250 "../lib/Chai.chpl"
  raw_c_void_ptr _args_vforcoforall_fn_chpl2;
#line 250 "../lib/Chai.chpl"
  int64_t _args_sizecoforall_fn_chpl2;
#line 250 "../lib/Chai.chpl"
  Error_chpl error_chpl7 = NULL;
#line 250 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_2_int64_t_one__real64_int64_t_chpl _field_destructor_tmp__chpl;
#line 250 "../lib/Chai.chpl"
  _domain_DefaultRectangularDom_2_int64_t_one_chpl _field_destructor_tmp__chpl2;
#line 248 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_3_int64_t_one__real64_int64_t_chpl _field_destructor_tmp__chpl3;
#line 248 "../lib/Chai.chpl"
  _domain_DefaultRectangularDom_3_int64_t_one_chpl _field_destructor_tmp__chpl4;
#line 248 "../lib/Chai.chpl"
  chpl__class_localscoforall_fn_chpl36_object chpl_macro_tmp_103;
#line 248 "../lib/Chai.chpl"
  chpl__class_localscoforall_fn_chpl27_object chpl_macro_tmp_104;
#line 249 "../lib/Chai.chpl"
  local_dataParMinGranularity_chpl = dataParMinGranularity_chpl;
#line 249 "../lib/Chai.chpl"
  local_dataParIgnoreRunningTasks_chpl = dataParIgnoreRunningTasks_chpl;
#line 249 "../lib/Chai.chpl"
  local_dataParTasksPerLocale_chpl = dataParTasksPerLocale_chpl;
#line 248 "../lib/Chai.chpl"
  zeros_chpl(kh_chpl, kw_chpl, channels_chpl, &ret_tmp_chpl);
#line 248 "../lib/Chai.chpl"
  call_tmp_chpl6 = ret_tmp_chpl;
#line 248 "../lib/Chai.chpl"
  filter_chpl = call_tmp_chpl6;
#line 248 "../lib/Chai.chpl"
  _computeBlock_chpl(len_chpl, numChunks_chpl, chunk_chpl, (&this_chpl31)->_high, (&this_chpl31)->_low, (&this_chpl31)->_low, &ret_tmp_chpl2, INT64(248), INT32(65));
#line 248 "../lib/Chai.chpl"
  *(call_tmp_chpl7 + INT64(0)) = *(ret_tmp_chpl2 + INT64(0));
#line 248 "../lib/Chai.chpl"
  *(call_tmp_chpl7 + INT64(1)) = *(ret_tmp_chpl2 + INT64(1));
#line 248 "../lib/Chai.chpl"
  tmp_x0_chpl = *(call_tmp_chpl7 + INT64(0));
#line 248 "../lib/Chai.chpl"
  tmp_x1_chpl = *(call_tmp_chpl7 + INT64(1));
#line 248 "../lib/Chai.chpl"
  _ic__F1_high_chpl = tmp_x1_chpl;
#line 248 "../lib/Chai.chpl"
  i_chpl = INT64(0);
#line 248 "../lib/Chai.chpl"
  chpl_checkIfRangeIterWillOverflow(tmp_x0_chpl, tmp_x1_chpl, INT64(1), tmp_x0_chpl, tmp_x1_chpl, UINT8(true), INT64(248), INT32(65));
#line 248 "../lib/Chai.chpl"
  for (i_chpl = tmp_x0_chpl; ((i_chpl <= _ic__F1_high_chpl)); i_chpl += INT64(1)) {
#line 248 "../lib/Chai.chpl"
    f_chpl = i_chpl;
#line 249 "../lib/Chai.chpl"
    call_tmp_chpl8 = &((&filter_chpl)->data);
#line 249 "../lib/Chai.chpl"
    chpl_build_unbounded_range(&ret_tmp_chpl3);
#line 249 "../lib/Chai.chpl"
    call_tmp_chpl9 = ret_tmp_chpl3;
#line 249 "../lib/Chai.chpl"
    chpl_build_unbounded_range(&ret_tmp_chpl4);
#line 249 "../lib/Chai.chpl"
    call_tmp_chpl10 = ret_tmp_chpl4;
#line 249 "../lib/Chai.chpl"
    chpl_build_unbounded_range(&ret_tmp_chpl5);
#line 249 "../lib/Chai.chpl"
    call_tmp_chpl11 = ret_tmp_chpl5;
#line 98 "../lib/Tensor.chpl"
    call_tmp_chpl13 = &((filters_svar_chpl)->data);
#line 249 "../lib/Chai.chpl"
    this_chpl8(call_tmp_chpl13, i_chpl, call_tmp_chpl9, call_tmp_chpl10, call_tmp_chpl11, &ret_tmp_chpl6, INT64(249), INT32(65));
#line 249 "../lib/Chai.chpl"
    call_tmp_chpl12 = ret_tmp_chpl6;
#line 249 "../lib/Chai.chpl"
    i_b_chpl = &call_tmp_chpl12;
#line 249 "../lib/Chai.chpl"
    if (((RootClass_chpl)((&call_tmp_chpl12)->_instance)) == nil) /* ZLINE: 249 ../lib/Chai.chpl */
#line 249 "../lib/Chai.chpl"
    {
#line 249 "../lib/Chai.chpl"
      goto _end__ASSIGN__chpl;
#line 249 "../lib/Chai.chpl"
    }
#line 249 "../lib/Chai.chpl"
    if (((RootClass_chpl)((call_tmp_chpl8)->_instance)) == ((RootClass_chpl)((&call_tmp_chpl12)->_instance))) /* ZLINE: 249 ../lib/Chai.chpl */
#line 249 "../lib/Chai.chpl"
    {
#line 249 "../lib/Chai.chpl"
      goto _end__ASSIGN__chpl;
#line 249 "../lib/Chai.chpl"
    }
#line 249 "../lib/Chai.chpl"
    call_tmp_chpl14 = sizeAs_chpl6(call_tmp_chpl8, INT64(249), INT32(65));
#line 249 "../lib/Chai.chpl"
    if (! (INT64(0) < INT64(0))) /* ZLINE: 249 ../lib/Chai.chpl */
#line 249 "../lib/Chai.chpl"
    {
#line 249 "../lib/Chai.chpl"
      tmp_chpl = (call_tmp_chpl14 == ((uint64_t)(INT64(0))));
#line 249 "../lib/Chai.chpl"
    } else /* ZLINE: 249 ../lib/Chai.chpl */
#line 249 "../lib/Chai.chpl"
    {
#line 249 "../lib/Chai.chpl"
      tmp_chpl = UINT8(false);
#line 249 "../lib/Chai.chpl"
    }
#line 249 "../lib/Chai.chpl"
    if (tmp_chpl) /* ZLINE: 249 ../lib/Chai.chpl */
#line 249 "../lib/Chai.chpl"
    {
#line 249 "../lib/Chai.chpl"
      call_tmp_chpl15 = sizeAs_chpl(i_b_chpl, INT64(249), INT32(65));
#line 249 "../lib/Chai.chpl"
      if (! (INT64(0) < INT64(0))) /* ZLINE: 249 ../lib/Chai.chpl */
#line 249 "../lib/Chai.chpl"
      {
#line 249 "../lib/Chai.chpl"
        tmp_chpl3 = (call_tmp_chpl15 == ((uint64_t)(INT64(0))));
#line 249 "../lib/Chai.chpl"
      } else /* ZLINE: 249 ../lib/Chai.chpl */
#line 249 "../lib/Chai.chpl"
      {
#line 249 "../lib/Chai.chpl"
        tmp_chpl3 = UINT8(false);
#line 249 "../lib/Chai.chpl"
      }
#line 249 "../lib/Chai.chpl"
      tmp_chpl2 = tmp_chpl3;
#line 249 "../lib/Chai.chpl"
    } else /* ZLINE: 249 ../lib/Chai.chpl */
#line 249 "../lib/Chai.chpl"
    {
#line 249 "../lib/Chai.chpl"
      tmp_chpl2 = UINT8(false);
#line 249 "../lib/Chai.chpl"
    }
#line 249 "../lib/Chai.chpl"
    if (tmp_chpl2) /* ZLINE: 249 ../lib/Chai.chpl */
#line 249 "../lib/Chai.chpl"
    {
#line 249 "../lib/Chai.chpl"
      goto _end__ASSIGN__chpl;
#line 249 "../lib/Chai.chpl"
    }
#line 249 "../lib/Chai.chpl"
    checkArrayShapesUponAssignment_chpl10(call_tmp_chpl8, i_b_chpl, UINT8(false), INT64(249), INT32(65));
#line 249 "../lib/Chai.chpl"
    _dom_chpl8(call_tmp_chpl8, &ret_tmp_chpl7, INT64(249), INT32(65));
#line 249 "../lib/Chai.chpl"
    call_tmp_chpl16 = ret_tmp_chpl7;
#line 249 "../lib/Chai.chpl"
    _dom_chpl20(i_b_chpl, &ret_tmp_chpl8, INT64(249), INT32(65));
#line 249 "../lib/Chai.chpl"
    call_tmp_chpl17 = ret_tmp_chpl8;
#line 249 "../lib/Chai.chpl"
    i_AD_chpl = &call_tmp_chpl16;
#line 249 "../lib/Chai.chpl"
    i_BD_chpl = &call_tmp_chpl17;
#line 249 "../lib/Chai.chpl"
    ret_chpl = (call_tmp_chpl8)->_instance;
#line 249 "../lib/Chai.chpl"
    ret_chpl2 = (&call_tmp_chpl12)->_instance;
#line 249 "../lib/Chai.chpl"
    chpl_check_nil(ret_chpl2, INT64(249), INT32(65));
#line 249 "../lib/Chai.chpl"
    call_tmp_chpl18 = doiBulkTransferToKnown_chpl(ret_chpl2, i_BD_chpl, ret_chpl, i_AD_chpl, INT64(249), INT32(65));
#line 249 "../lib/Chai.chpl"
    if (call_tmp_chpl18) /* ZLINE: 249 ../lib/Chai.chpl */
#line 249 "../lib/Chai.chpl"
    {
#line 249 "../lib/Chai.chpl"
    } else /* ZLINE: 249 ../lib/Chai.chpl */
#line 249 "../lib/Chai.chpl"
    {
#line 249 "../lib/Chai.chpl"
    }
#line 249 "../lib/Chai.chpl"
    chpl__autoDestroy6(&call_tmp_chpl17, INT64(249), INT32(65));
#line 249 "../lib/Chai.chpl"
    chpl__autoDestroy8(&call_tmp_chpl16, INT64(249), INT32(65));
#line 249 "../lib/Chai.chpl"
    if (call_tmp_chpl18) /* ZLINE: 249 ../lib/Chai.chpl */
#line 249 "../lib/Chai.chpl"
    {
#line 249 "../lib/Chai.chpl"
    }
#line 249 "../lib/Chai.chpl"
    if (! call_tmp_chpl18) /* ZLINE: 249 ../lib/Chai.chpl */
#line 249 "../lib/Chai.chpl"
    {
#line 249 "../lib/Chai.chpl"
      coerce_tmp_chpl3 = (call_tmp_chpl8)->_instance;
#line 249 "../lib/Chai.chpl"
      tasksPerLocale_chpl = local_dataParTasksPerLocale_chpl;
#line 249 "../lib/Chai.chpl"
      ignoreRunning_chpl = local_dataParIgnoreRunningTasks_chpl;
#line 249 "../lib/Chai.chpl"
      minIndicesPerTask_chpl = local_dataParMinGranularity_chpl;
#line 249 "../lib/Chai.chpl"
      error_chpl = NULL;
#line 249 "../lib/Chai.chpl"
      chpl_check_nil(coerce_tmp_chpl3, INT64(249), INT32(65));
#line 249 "../lib/Chai.chpl"
      coerce_tmp_chpl4 = (coerce_tmp_chpl3)->dom;
#line 249 "../lib/Chai.chpl"
      createTuple_chpl2(INT64(0), &ret_tmp_chpl9);
#line 249 "../lib/Chai.chpl"
      offset_x0_chpl = *(ret_tmp_chpl9 + INT64(0));
#line 249 "../lib/Chai.chpl"
      offset_x1_chpl = *(ret_tmp_chpl9 + INT64(1));
#line 249 "../lib/Chai.chpl"
      offset_x2_chpl = *(ret_tmp_chpl9 + INT64(2));
#line 249 "../lib/Chai.chpl"
      error_chpl2 = NULL;
#line 249 "../lib/Chai.chpl"
      call_tmp_chpl21 = get_chpl_nodeID();
#line 249 "../lib/Chai.chpl"
      call_tmp_chpl22 = chpl_rt_buildLocaleID(call_tmp_chpl21, c_sublocid_any);
#line 249 "../lib/Chai.chpl"
      call_tmp_chpl20 = call_tmp_chpl22;
#line 249 "../lib/Chai.chpl"
      chpl_localeID_to_locale(&call_tmp_chpl20, &ret_tmp_chpl10, INT64(249), INT32(65));
#line 249 "../lib/Chai.chpl"
      call_tmp_chpl23 = ret_tmp_chpl10;
#line 249 "../lib/Chai.chpl"
      call_tmp_chpl19 = call_tmp_chpl23;
#line 249 "../lib/Chai.chpl"
      coerce_tmp_chpl5 = (&call_tmp_chpl19)->_instance;
#line 249 "../lib/Chai.chpl"
      if (((RootClass_chpl)(coerce_tmp_chpl5)) == nil) /* ZLINE: 249 ../lib/Chai.chpl */
#line 249 "../lib/Chai.chpl"
      {
#line 249 "../lib/Chai.chpl"
        nilCheckHalt_chpl(&_str_literal_52_chpl, INT64(249), INT32(65));
#line 249 "../lib/Chai.chpl"
      }
#line 249 "../lib/Chai.chpl"
      chpl_check_nil(coerce_tmp_chpl5, INT64(249), INT32(65));
#line 249 "../lib/Chai.chpl"
      ((int64_t(*)(BaseLocale_chpl,
#line 249 "../lib/Chai.chpl"
         int64_t,
#line 249 "../lib/Chai.chpl"
         int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(coerce_tmp_chpl5))->chpl__cid) + INT64(2))])(coerce_tmp_chpl5, INT64(249), INT32(65));
#line 249 "../lib/Chai.chpl"
      i_x_chpl = &call_tmp_chpl19;
#line 249 "../lib/Chai.chpl"
      deinit_chpl62(i_x_chpl);
#line 249 "../lib/Chai.chpl"
      if (tasksPerLocale_chpl == INT64(0)) /* ZLINE: 249 ../lib/Chai.chpl */
#line 249 "../lib/Chai.chpl"
      {
#line 249 "../lib/Chai.chpl"
        call_tmp_chpl26 = get_chpl_nodeID();
#line 249 "../lib/Chai.chpl"
        call_tmp_chpl27 = chpl_rt_buildLocaleID(call_tmp_chpl26, c_sublocid_any);
#line 249 "../lib/Chai.chpl"
        call_tmp_chpl25 = call_tmp_chpl27;
#line 249 "../lib/Chai.chpl"
        chpl_localeID_to_locale(&call_tmp_chpl25, &ret_tmp_chpl11, INT64(249), INT32(65));
#line 249 "../lib/Chai.chpl"
        call_tmp_chpl28 = ret_tmp_chpl11;
#line 249 "../lib/Chai.chpl"
        call_tmp_chpl24 = call_tmp_chpl28;
#line 249 "../lib/Chai.chpl"
        coerce_tmp_chpl6 = (&call_tmp_chpl24)->_instance;
#line 249 "../lib/Chai.chpl"
        if (((RootClass_chpl)(coerce_tmp_chpl6)) == nil) /* ZLINE: 249 ../lib/Chai.chpl */
#line 249 "../lib/Chai.chpl"
        {
#line 249 "../lib/Chai.chpl"
          nilCheckHalt_chpl(&_str_literal_52_chpl, INT64(249), INT32(65));
#line 249 "../lib/Chai.chpl"
        }
#line 249 "../lib/Chai.chpl"
        coerce_tmp_chpl7 = coerce_tmp_chpl6;
#line 249 "../lib/Chai.chpl"
        chpl_check_nil(coerce_tmp_chpl7, INT64(249), INT32(65));
#line 249 "../lib/Chai.chpl"
        ret_chpl3 = (coerce_tmp_chpl7)->maxTaskPar;
#line 249 "../lib/Chai.chpl"
        tmp_chpl4 = ret_chpl3;
#line 249 "../lib/Chai.chpl"
        i_x_chpl2 = &call_tmp_chpl24;
#line 249 "../lib/Chai.chpl"
        deinit_chpl62(i_x_chpl2);
#line 249 "../lib/Chai.chpl"
      } else /* ZLINE: 249 ../lib/Chai.chpl */
#line 249 "../lib/Chai.chpl"
      {
#line 249 "../lib/Chai.chpl"
        tmp_chpl4 = tasksPerLocale_chpl;
#line 249 "../lib/Chai.chpl"
      }
#line 249 "../lib/Chai.chpl"
      if (chpl_task_getSerial()) /* ZLINE: 249 ../lib/Chai.chpl */
#line 249 "../lib/Chai.chpl"
      {
#line 249 "../lib/Chai.chpl"
        tmp_x0_chpl2 = INT64(1);
#line 249 "../lib/Chai.chpl"
        tmp_x1_chpl2 = INT64(0);
#line 249 "../lib/Chai.chpl"
      } else /* ZLINE: 249 ../lib/Chai.chpl */
#line 249 "../lib/Chai.chpl"
      {
#line 249 "../lib/Chai.chpl"
        chpl_check_nil(coerce_tmp_chpl4, INT64(249), INT32(65));
#line 249 "../lib/Chai.chpl"
        call_tmp_chpl29 = &((coerce_tmp_chpl4)->ranges);
#line 249 "../lib/Chai.chpl"
        _computeChunkStuff_chpl3(tmp_chpl4, ignoreRunning_chpl, minIndicesPerTask_chpl, call_tmp_chpl29, &ret_tmp_chpl12, INT64(249), INT32(65));
#line 249 "../lib/Chai.chpl"
        tmp_x0_chpl2 = *(ret_tmp_chpl12 + INT64(0));
#line 249 "../lib/Chai.chpl"
        tmp_x1_chpl2 = *(ret_tmp_chpl12 + INT64(1));
#line 249 "../lib/Chai.chpl"
      }
#line 249 "../lib/Chai.chpl"
      numChunks_chpl2 = tmp_x0_chpl2;
#line 249 "../lib/Chai.chpl"
      parDim_chpl = tmp_x1_chpl2;
#line 249 "../lib/Chai.chpl"
      init_chpl113(&elt_x0_chpl);
#line 249 "../lib/Chai.chpl"
      tup_x0_chpl = elt_x0_chpl;
#line 249 "../lib/Chai.chpl"
      init_chpl113(&elt_x1_chpl);
#line 249 "../lib/Chai.chpl"
      tup_x1_chpl = elt_x1_chpl;
#line 249 "../lib/Chai.chpl"
      init_chpl113(&elt_x2_chpl);
#line 249 "../lib/Chai.chpl"
      tup_x2_chpl = elt_x2_chpl;
#line 249 "../lib/Chai.chpl"
      *(locBlock_chpl + INT64(0)) = tup_x0_chpl;
#line 249 "../lib/Chai.chpl"
      *(locBlock_chpl + INT64(1)) = tup_x1_chpl;
#line 249 "../lib/Chai.chpl"
      *(locBlock_chpl + INT64(2)) = tup_x2_chpl;
#line 249 "../lib/Chai.chpl"
      chpl_build_low_bounded_range2(offset_x0_chpl, &ret_tmp_chpl13);
#line 249 "../lib/Chai.chpl"
      call_tmp_chpl30 = ret_tmp_chpl13;
#line 249 "../lib/Chai.chpl"
      chpl_check_nil(coerce_tmp_chpl4, INT64(249), INT32(65));
#line 249 "../lib/Chai.chpl"
      call_tmp_chpl31 = &((coerce_tmp_chpl4)->ranges);
#line 249 "../lib/Chai.chpl"
      coerce_tmp_chpl8 = *(*(call_tmp_chpl31) + INT64(0));
#line 249 "../lib/Chai.chpl"
      call_tmp_chpl32 = sizeAs_chpl37(coerce_tmp_chpl8, INT64(249), INT32(65));
#line 249 "../lib/Chai.chpl"
      chpl___POUND_3(call_tmp_chpl30, call_tmp_chpl32, &ret_tmp_chpl14, INT64(249), INT32(65));
#line 249 "../lib/Chai.chpl"
      call_tmp_chpl33 = ret_tmp_chpl14;
#line 249 "../lib/Chai.chpl"
      *(locBlock_chpl + INT64(0)) = call_tmp_chpl33;
#line 249 "../lib/Chai.chpl"
      chpl_build_low_bounded_range2(offset_x1_chpl, &ret_tmp_chpl15);
#line 249 "../lib/Chai.chpl"
      call_tmp_chpl34 = ret_tmp_chpl15;
#line 249 "../lib/Chai.chpl"
      chpl_check_nil(coerce_tmp_chpl4, INT64(249), INT32(65));
#line 249 "../lib/Chai.chpl"
      call_tmp_chpl35 = &((coerce_tmp_chpl4)->ranges);
#line 249 "../lib/Chai.chpl"
      coerce_tmp_chpl9 = *(*(call_tmp_chpl35) + INT64(1));
#line 249 "../lib/Chai.chpl"
      call_tmp_chpl36 = sizeAs_chpl37(coerce_tmp_chpl9, INT64(249), INT32(65));
#line 249 "../lib/Chai.chpl"
      chpl___POUND_3(call_tmp_chpl34, call_tmp_chpl36, &ret_tmp_chpl16, INT64(249), INT32(65));
#line 249 "../lib/Chai.chpl"
      call_tmp_chpl37 = ret_tmp_chpl16;
#line 249 "../lib/Chai.chpl"
      *(locBlock_chpl + INT64(1)) = call_tmp_chpl37;
#line 249 "../lib/Chai.chpl"
      chpl_build_low_bounded_range2(offset_x2_chpl, &ret_tmp_chpl17);
#line 249 "../lib/Chai.chpl"
      call_tmp_chpl38 = ret_tmp_chpl17;
#line 249 "../lib/Chai.chpl"
      chpl_check_nil(coerce_tmp_chpl4, INT64(249), INT32(65));
#line 249 "../lib/Chai.chpl"
      call_tmp_chpl39 = &((coerce_tmp_chpl4)->ranges);
#line 249 "../lib/Chai.chpl"
      coerce_tmp_chpl10 = *(*(call_tmp_chpl39) + INT64(2));
#line 249 "../lib/Chai.chpl"
      call_tmp_chpl40 = sizeAs_chpl37(coerce_tmp_chpl10, INT64(249), INT32(65));
#line 249 "../lib/Chai.chpl"
      chpl___POUND_3(call_tmp_chpl38, call_tmp_chpl40, &ret_tmp_chpl18, INT64(249), INT32(65));
#line 249 "../lib/Chai.chpl"
      call_tmp_chpl41 = ret_tmp_chpl18;
#line 249 "../lib/Chai.chpl"
      *(locBlock_chpl + INT64(2)) = call_tmp_chpl41;
#line 249 "../lib/Chai.chpl"
      chpl_build_low_bounded_range2(INT64(0), &ret_tmp_chpl19);
#line 249 "../lib/Chai.chpl"
      call_tmp_chpl42 = ret_tmp_chpl19;
#line 249 "../lib/Chai.chpl"
      chpl___POUND_3(call_tmp_chpl42, tmp_x0_chpl2, &ret_tmp_chpl20, INT64(249), INT32(65));
#line 249 "../lib/Chai.chpl"
      tmpIter_chpl = ret_tmp_chpl20;
#line 249 "../lib/Chai.chpl"
      new_temp_chpl = _new_chpl17(INT64(249), INT32(65));
#line 249 "../lib/Chai.chpl"
      _coforallCount_chpl2 = new_temp_chpl;
#line 249 "../lib/Chai.chpl"
      numTasks_chpl = chpl_boundedCoforallSize(tmpIter_chpl, INT64(249), INT32(65));
#line 249 "../lib/Chai.chpl"
      _upEndCount_chpl(_coforallCount_chpl2, numTasks_chpl, INT64(249), INT32(65));
#line 249 "../lib/Chai.chpl"
      chpl_resetTaskSpawn(numTasks_chpl, INT64(249), INT32(65));
#line 249 "../lib/Chai.chpl"
      _ic__F0_this_chpl = tmpIter_chpl;
#line 249 "../lib/Chai.chpl"
      checkIfIterWillOverflow_chpl(tmpIter_chpl, UINT8(true), INT64(249), INT32(65));
#line 249 "../lib/Chai.chpl"
      i_chpl2 = INT64(0);
#line 249 "../lib/Chai.chpl"
      this_chpl32 = _ic__F0_this_chpl;
#line 249 "../lib/Chai.chpl"
      this_chpl33 = _ic__F0_this_chpl;
#line 249 "../lib/Chai.chpl"
      end_chpl = (&this_chpl33)->_high;
#line 249 "../lib/Chai.chpl"
      for (i_chpl2 = (&this_chpl32)->_low; ((i_chpl2 <= end_chpl)); i_chpl2 += INT64(1)) {
#line 249 "../lib/Chai.chpl"
        _args_forcoforall_fn_chpl = ((_class_localscoforall_fn_chpl36)(&chpl_macro_tmp_103));
#line 249 "../lib/Chai.chpl"
        chpl_check_nil(_args_forcoforall_fn_chpl, INT64(249), INT32(65));
#line 249 "../lib/Chai.chpl"
        (_args_forcoforall_fn_chpl)->_0_numChunks = numChunks_chpl2;
#line 249 "../lib/Chai.chpl"
        chpl_check_nil(_args_forcoforall_fn_chpl, INT64(249), INT32(65));
#line 249 "../lib/Chai.chpl"
        (_args_forcoforall_fn_chpl)->_1_parDim = parDim_chpl;
#line 249 "../lib/Chai.chpl"
        chpl_check_nil(_args_forcoforall_fn_chpl, INT64(249), INT32(65));
#line 249 "../lib/Chai.chpl"
        *((_args_forcoforall_fn_chpl)->_2_coerce_tmp + INT64(0)) = *(locBlock_chpl + INT64(0));
#line 249 "../lib/Chai.chpl"
        *((_args_forcoforall_fn_chpl)->_2_coerce_tmp + INT64(1)) = *(locBlock_chpl + INT64(1));
#line 249 "../lib/Chai.chpl"
        *((_args_forcoforall_fn_chpl)->_2_coerce_tmp + INT64(2)) = *(locBlock_chpl + INT64(2));
#line 249 "../lib/Chai.chpl"
        chpl_check_nil(_args_forcoforall_fn_chpl, INT64(249), INT32(65));
#line 249 "../lib/Chai.chpl"
        (_args_forcoforall_fn_chpl)->_3__coforallCount = _coforallCount_chpl2;
#line 249 "../lib/Chai.chpl"
        chpl_check_nil(_args_forcoforall_fn_chpl, INT64(249), INT32(65));
#line 249 "../lib/Chai.chpl"
        (_args_forcoforall_fn_chpl)->_4_chunk = i_chpl2;
#line 249 "../lib/Chai.chpl"
        chpl_check_nil(_args_forcoforall_fn_chpl, INT64(249), INT32(65));
#line 249 "../lib/Chai.chpl"
        (_args_forcoforall_fn_chpl)->_6_a = *(call_tmp_chpl8);
#line 249 "../lib/Chai.chpl"
        chpl_check_nil(_args_forcoforall_fn_chpl, INT64(249), INT32(65));
#line 249 "../lib/Chai.chpl"
        (_args_forcoforall_fn_chpl)->_7_b = *(i_b_chpl);
#line 249 "../lib/Chai.chpl"
        _args_vforcoforall_fn_chpl = ((void*)(_args_forcoforall_fn_chpl));
#line 249 "../lib/Chai.chpl"
        _args_sizecoforall_fn_chpl = sizeof(chpl__class_localscoforall_fn_chpl36_object);
#line 249 "../lib/Chai.chpl"
        (_args_forcoforall_fn_chpl)->_ln = INT64(249);
#line 249 "../lib/Chai.chpl"
        (_args_forcoforall_fn_chpl)->_fn = INT32(65);
#line 249 "../lib/Chai.chpl"
        /*** wrapcoforall_fn_chpl17 ***/ chpl_taskAddCoStmt(chpl_task_getRequestedSubloc(), INT64(16), ((chpl_task_bundle_p)(_args_vforcoforall_fn_chpl)), _args_sizecoforall_fn_chpl, INT64(2763), INT32(30));
#line 249 "../lib/Chai.chpl"
      }
#line 249 "../lib/Chai.chpl"
      error_chpl3 = NULL;
#line 249 "../lib/Chai.chpl"
      _waitEndCount_chpl2(_coforallCount_chpl2, numTasks_chpl, &error_chpl3, INT64(249), INT32(65));
#line 249 "../lib/Chai.chpl"
      if (error_chpl3 != nil) /* ZLINE: 249 ../lib/Chai.chpl */
#line 249 "../lib/Chai.chpl"
      {
#line 249 "../lib/Chai.chpl"
        chpl__delete72(_coforallCount_chpl2, INT64(249), INT32(65));
#line 249 "../lib/Chai.chpl"
        error_chpl2 = error_chpl3;
#line 249 "../lib/Chai.chpl"
        goto forall_IBB_break_label_chpl;
#line 249 "../lib/Chai.chpl"
      }
#line 249 "../lib/Chai.chpl"
      chpl__delete72(_coforallCount_chpl2, INT64(249), INT32(65));
#line 249 "../lib/Chai.chpl"
      chpl_after_forall_fence();
#line 249 "../lib/Chai.chpl"
      forall_IBB_break_label_chpl:;
#line 249 "../lib/Chai.chpl"
      if (error_chpl2 != nil) /* ZLINE: 249 ../lib/Chai.chpl */
#line 249 "../lib/Chai.chpl"
      {
#line 249 "../lib/Chai.chpl"
        error_chpl = error_chpl2;
#line 249 "../lib/Chai.chpl"
        goto forall_IBB_break_label_chpl2;
#line 249 "../lib/Chai.chpl"
      }
#line 249 "../lib/Chai.chpl"
      chpl_after_forall_fence();
#line 249 "../lib/Chai.chpl"
      forall_IBB_break_label_chpl2:;
#line 249 "../lib/Chai.chpl"
      if (error_chpl != nil) /* ZLINE: 249 ../lib/Chai.chpl */
#line 249 "../lib/Chai.chpl"
      {
#line 249 "../lib/Chai.chpl"
        chpl_propagate_error(error_chpl, INT64(249), INT32(65));
#line 249 "../lib/Chai.chpl"
      }
#line 249 "../lib/Chai.chpl"
      chpl_after_forall_fence();
#line 249 "../lib/Chai.chpl"
    }
#line 249 "../lib/Chai.chpl"
    _end__ASSIGN__chpl:;
#line 249 "../lib/Chai.chpl"
    chpl__autoDestroy29(&call_tmp_chpl12, INT64(249), INT32(65));
#line 250 "../lib/Chai.chpl"
    chpl_build_unbounded_range(&ret_tmp_chpl21);
#line 250 "../lib/Chai.chpl"
    call_tmp_chpl43 = ret_tmp_chpl21;
#line 250 "../lib/Chai.chpl"
    chpl_build_unbounded_range(&ret_tmp_chpl22);
#line 250 "../lib/Chai.chpl"
    call_tmp_chpl44 = ret_tmp_chpl22;
#line 98 "../lib/Tensor.chpl"
    call_tmp_chpl46 = &((convs_chpl)->data);
#line 250 "../lib/Chai.chpl"
    this_chpl4(call_tmp_chpl46, call_tmp_chpl43, call_tmp_chpl44, f_chpl, &ret_tmp_chpl23, INT64(250), INT32(65));
#line 250 "../lib/Chai.chpl"
    call_tmp_chpl45 = ret_tmp_chpl23;
#line 250 "../lib/Chai.chpl"
    correlate_chpl2(&filter_chpl, images_chpl, stride_svar_chpl, padding_svar_chpl, &ret_tmp_chpl24);
#line 250 "../lib/Chai.chpl"
    call_tmp_chpl47 = ret_tmp_chpl24;
#line 250 "../lib/Chai.chpl"
    i_a_chpl = &call_tmp_chpl45;
#line 250 "../lib/Chai.chpl"
    coerce_tmp_chpl11 = (&call_tmp_chpl45)->_instance;
#line 250 "../lib/Chai.chpl"
    error_chpl4 = NULL;
#line 250 "../lib/Chai.chpl"
    chpl_check_nil(coerce_tmp_chpl11, INT64(250), INT32(65));
#line 250 "../lib/Chai.chpl"
    ret_chpl4 = (coerce_tmp_chpl11)->dom;
#line 250 "../lib/Chai.chpl"
    coerce_tmp_chpl12 = ret_chpl4;
#line 250 "../lib/Chai.chpl"
    error_chpl5 = NULL;
#line 250 "../lib/Chai.chpl"
    chpl_check_nil(coerce_tmp_chpl12, INT64(250), INT32(65));
#line 250 "../lib/Chai.chpl"
    if (((RootClass_chpl)((coerce_tmp_chpl12)->downDomInst)) == nil) /* ZLINE: 250 ../lib/Chai.chpl */
#line 250 "../lib/Chai.chpl"
    {
#line 250 "../lib/Chai.chpl"
      nilCheckHalt_chpl(&_str_literal_52_chpl, INT64(250), INT32(65));
#line 250 "../lib/Chai.chpl"
    }
#line 250 "../lib/Chai.chpl"
    chpl_check_nil(coerce_tmp_chpl12, INT64(250), INT32(65));
#line 250 "../lib/Chai.chpl"
    if (((RootClass_chpl)((coerce_tmp_chpl12)->downDomInst)) == nil) /* ZLINE: 250 ../lib/Chai.chpl */
#line 250 "../lib/Chai.chpl"
    {
#line 250 "../lib/Chai.chpl"
      nilCheckHalt_chpl(&_str_literal_52_chpl, INT64(250), INT32(65));
#line 250 "../lib/Chai.chpl"
    }
#line 250 "../lib/Chai.chpl"
    chpl_check_nil(coerce_tmp_chpl12, INT64(250), INT32(65));
#line 250 "../lib/Chai.chpl"
    if (((RootClass_chpl)((coerce_tmp_chpl12)->downDomInst)) == nil) /* ZLINE: 250 ../lib/Chai.chpl */
#line 250 "../lib/Chai.chpl"
    {
#line 250 "../lib/Chai.chpl"
      nilCheckHalt_chpl(&_str_literal_52_chpl, INT64(250), INT32(65));
#line 250 "../lib/Chai.chpl"
    }
#line 250 "../lib/Chai.chpl"
    chpl_check_nil(coerce_tmp_chpl12, INT64(250), INT32(65));
#line 250 "../lib/Chai.chpl"
    coerce_tmp_chpl13 = (coerce_tmp_chpl12)->upDomInst;
#line 250 "../lib/Chai.chpl"
    if (((RootClass_chpl)(coerce_tmp_chpl13)) == nil) /* ZLINE: 250 ../lib/Chai.chpl */
#line 250 "../lib/Chai.chpl"
    {
#line 250 "../lib/Chai.chpl"
      nilCheckHalt_chpl(&_str_literal_52_chpl, INT64(250), INT32(65));
#line 250 "../lib/Chai.chpl"
    }
#line 250 "../lib/Chai.chpl"
    chpl_check_nil(coerce_tmp_chpl12, INT64(250), INT32(65));
#line 250 "../lib/Chai.chpl"
    if (((RootClass_chpl)((coerce_tmp_chpl12)->upDomInst)) == nil) /* ZLINE: 250 ../lib/Chai.chpl */
#line 250 "../lib/Chai.chpl"
    {
#line 250 "../lib/Chai.chpl"
      nilCheckHalt_chpl(&_str_literal_52_chpl, INT64(250), INT32(65));
#line 250 "../lib/Chai.chpl"
    }
#line 250 "../lib/Chai.chpl"
    coerce_tmp_chpl14 = coerce_tmp_chpl13;
#line 250 "../lib/Chai.chpl"
    tasksPerLocale_chpl2 = local_dataParTasksPerLocale_chpl;
#line 250 "../lib/Chai.chpl"
    ignoreRunning_chpl2 = local_dataParIgnoreRunningTasks_chpl;
#line 250 "../lib/Chai.chpl"
    minIndicesPerTask_chpl2 = local_dataParMinGranularity_chpl;
#line 250 "../lib/Chai.chpl"
    createTuple_chpl4(INT64(0), &ret_tmp_chpl25);
#line 250 "../lib/Chai.chpl"
    offset_x0_chpl2 = *(ret_tmp_chpl25 + INT64(0));
#line 250 "../lib/Chai.chpl"
    offset_x1_chpl2 = *(ret_tmp_chpl25 + INT64(1));
#line 250 "../lib/Chai.chpl"
    error_chpl6 = NULL;
#line 250 "../lib/Chai.chpl"
    call_tmp_chpl50 = get_chpl_nodeID();
#line 250 "../lib/Chai.chpl"
    call_tmp_chpl51 = chpl_rt_buildLocaleID(call_tmp_chpl50, c_sublocid_any);
#line 250 "../lib/Chai.chpl"
    call_tmp_chpl49 = call_tmp_chpl51;
#line 250 "../lib/Chai.chpl"
    chpl_localeID_to_locale(&call_tmp_chpl49, &ret_tmp_chpl26, INT64(250), INT32(65));
#line 250 "../lib/Chai.chpl"
    call_tmp_chpl52 = ret_tmp_chpl26;
#line 250 "../lib/Chai.chpl"
    call_tmp_chpl48 = call_tmp_chpl52;
#line 250 "../lib/Chai.chpl"
    coerce_tmp_chpl15 = (&call_tmp_chpl48)->_instance;
#line 250 "../lib/Chai.chpl"
    if (((RootClass_chpl)(coerce_tmp_chpl15)) == nil) /* ZLINE: 250 ../lib/Chai.chpl */
#line 250 "../lib/Chai.chpl"
    {
#line 250 "../lib/Chai.chpl"
      nilCheckHalt_chpl(&_str_literal_52_chpl, INT64(250), INT32(65));
#line 250 "../lib/Chai.chpl"
    }
#line 250 "../lib/Chai.chpl"
    chpl_check_nil(coerce_tmp_chpl15, INT64(250), INT32(65));
#line 250 "../lib/Chai.chpl"
    ((int64_t(*)(BaseLocale_chpl,
#line 250 "../lib/Chai.chpl"
       int64_t,
#line 250 "../lib/Chai.chpl"
       int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(coerce_tmp_chpl15))->chpl__cid) + INT64(2))])(coerce_tmp_chpl15, INT64(250), INT32(65));
#line 250 "../lib/Chai.chpl"
    i_x_chpl3 = &call_tmp_chpl48;
#line 250 "../lib/Chai.chpl"
    deinit_chpl62(i_x_chpl3);
#line 250 "../lib/Chai.chpl"
    if (tasksPerLocale_chpl2 == INT64(0)) /* ZLINE: 250 ../lib/Chai.chpl */
#line 250 "../lib/Chai.chpl"
    {
#line 250 "../lib/Chai.chpl"
      call_tmp_chpl55 = get_chpl_nodeID();
#line 250 "../lib/Chai.chpl"
      call_tmp_chpl56 = chpl_rt_buildLocaleID(call_tmp_chpl55, c_sublocid_any);
#line 250 "../lib/Chai.chpl"
      call_tmp_chpl54 = call_tmp_chpl56;
#line 250 "../lib/Chai.chpl"
      chpl_localeID_to_locale(&call_tmp_chpl54, &ret_tmp_chpl27, INT64(250), INT32(65));
#line 250 "../lib/Chai.chpl"
      call_tmp_chpl57 = ret_tmp_chpl27;
#line 250 "../lib/Chai.chpl"
      call_tmp_chpl53 = call_tmp_chpl57;
#line 250 "../lib/Chai.chpl"
      coerce_tmp_chpl16 = (&call_tmp_chpl53)->_instance;
#line 250 "../lib/Chai.chpl"
      if (((RootClass_chpl)(coerce_tmp_chpl16)) == nil) /* ZLINE: 250 ../lib/Chai.chpl */
#line 250 "../lib/Chai.chpl"
      {
#line 250 "../lib/Chai.chpl"
        nilCheckHalt_chpl(&_str_literal_52_chpl, INT64(250), INT32(65));
#line 250 "../lib/Chai.chpl"
      }
#line 250 "../lib/Chai.chpl"
      coerce_tmp_chpl17 = coerce_tmp_chpl16;
#line 250 "../lib/Chai.chpl"
      chpl_check_nil(coerce_tmp_chpl17, INT64(250), INT32(65));
#line 250 "../lib/Chai.chpl"
      ret_chpl5 = (coerce_tmp_chpl17)->maxTaskPar;
#line 250 "../lib/Chai.chpl"
      tmp_chpl5 = ret_chpl5;
#line 250 "../lib/Chai.chpl"
      i_x_chpl4 = &call_tmp_chpl53;
#line 250 "../lib/Chai.chpl"
      deinit_chpl62(i_x_chpl4);
#line 250 "../lib/Chai.chpl"
    } else /* ZLINE: 250 ../lib/Chai.chpl */
#line 250 "../lib/Chai.chpl"
    {
#line 250 "../lib/Chai.chpl"
      tmp_chpl5 = tasksPerLocale_chpl2;
#line 250 "../lib/Chai.chpl"
    }
#line 250 "../lib/Chai.chpl"
    if (chpl_task_getSerial()) /* ZLINE: 250 ../lib/Chai.chpl */
#line 250 "../lib/Chai.chpl"
    {
#line 250 "../lib/Chai.chpl"
      tmp_x0_chpl3 = INT64(1);
#line 250 "../lib/Chai.chpl"
      tmp_x1_chpl3 = INT64(0);
#line 250 "../lib/Chai.chpl"
    } else /* ZLINE: 250 ../lib/Chai.chpl */
#line 250 "../lib/Chai.chpl"
    {
#line 250 "../lib/Chai.chpl"
      chpl_check_nil(coerce_tmp_chpl14, INT64(250), INT32(65));
#line 250 "../lib/Chai.chpl"
      call_tmp_chpl58 = &((coerce_tmp_chpl14)->ranges);
#line 250 "../lib/Chai.chpl"
      _computeChunkStuff_chpl4(tmp_chpl5, ignoreRunning_chpl2, minIndicesPerTask_chpl2, call_tmp_chpl58, &ret_tmp_chpl28, INT64(250), INT32(65));
#line 250 "../lib/Chai.chpl"
      tmp_x0_chpl3 = *(ret_tmp_chpl28 + INT64(0));
#line 250 "../lib/Chai.chpl"
      tmp_x1_chpl3 = *(ret_tmp_chpl28 + INT64(1));
#line 250 "../lib/Chai.chpl"
    }
#line 250 "../lib/Chai.chpl"
    numChunks_chpl3 = tmp_x0_chpl3;
#line 250 "../lib/Chai.chpl"
    parDim_chpl2 = tmp_x1_chpl3;
#line 250 "../lib/Chai.chpl"
    init_chpl113(&elt_x0_chpl2);
#line 250 "../lib/Chai.chpl"
    tup_x0_chpl2 = elt_x0_chpl2;
#line 250 "../lib/Chai.chpl"
    init_chpl113(&elt_x1_chpl2);
#line 250 "../lib/Chai.chpl"
    tup_x1_chpl2 = elt_x1_chpl2;
#line 250 "../lib/Chai.chpl"
    *(locBlock_chpl2 + INT64(0)) = tup_x0_chpl2;
#line 250 "../lib/Chai.chpl"
    *(locBlock_chpl2 + INT64(1)) = tup_x1_chpl2;
#line 250 "../lib/Chai.chpl"
    chpl_build_low_bounded_range2(offset_x0_chpl2, &ret_tmp_chpl29);
#line 250 "../lib/Chai.chpl"
    call_tmp_chpl59 = ret_tmp_chpl29;
#line 250 "../lib/Chai.chpl"
    chpl_check_nil(coerce_tmp_chpl14, INT64(250), INT32(65));
#line 250 "../lib/Chai.chpl"
    call_tmp_chpl60 = &((coerce_tmp_chpl14)->ranges);
#line 250 "../lib/Chai.chpl"
    coerce_tmp_chpl18 = *(*(call_tmp_chpl60) + INT64(0));
#line 250 "../lib/Chai.chpl"
    call_tmp_chpl61 = sizeAs_chpl37(coerce_tmp_chpl18, INT64(250), INT32(65));
#line 250 "../lib/Chai.chpl"
    chpl___POUND_3(call_tmp_chpl59, call_tmp_chpl61, &ret_tmp_chpl30, INT64(250), INT32(65));
#line 250 "../lib/Chai.chpl"
    call_tmp_chpl62 = ret_tmp_chpl30;
#line 250 "../lib/Chai.chpl"
    *(locBlock_chpl2 + INT64(0)) = call_tmp_chpl62;
#line 250 "../lib/Chai.chpl"
    chpl_build_low_bounded_range2(offset_x1_chpl2, &ret_tmp_chpl31);
#line 250 "../lib/Chai.chpl"
    call_tmp_chpl63 = ret_tmp_chpl31;
#line 250 "../lib/Chai.chpl"
    chpl_check_nil(coerce_tmp_chpl14, INT64(250), INT32(65));
#line 250 "../lib/Chai.chpl"
    call_tmp_chpl64 = &((coerce_tmp_chpl14)->ranges);
#line 250 "../lib/Chai.chpl"
    coerce_tmp_chpl19 = *(*(call_tmp_chpl64) + INT64(1));
#line 250 "../lib/Chai.chpl"
    call_tmp_chpl65 = sizeAs_chpl37(coerce_tmp_chpl19, INT64(250), INT32(65));
#line 250 "../lib/Chai.chpl"
    chpl___POUND_3(call_tmp_chpl63, call_tmp_chpl65, &ret_tmp_chpl32, INT64(250), INT32(65));
#line 250 "../lib/Chai.chpl"
    call_tmp_chpl66 = ret_tmp_chpl32;
#line 250 "../lib/Chai.chpl"
    *(locBlock_chpl2 + INT64(1)) = call_tmp_chpl66;
#line 250 "../lib/Chai.chpl"
    chpl_build_low_bounded_range2(INT64(0), &ret_tmp_chpl33);
#line 250 "../lib/Chai.chpl"
    call_tmp_chpl67 = ret_tmp_chpl33;
#line 250 "../lib/Chai.chpl"
    chpl___POUND_3(call_tmp_chpl67, tmp_x0_chpl3, &ret_tmp_chpl34, INT64(250), INT32(65));
#line 250 "../lib/Chai.chpl"
    tmpIter_chpl2 = ret_tmp_chpl34;
#line 250 "../lib/Chai.chpl"
    new_temp_chpl2 = _new_chpl17(INT64(250), INT32(65));
#line 250 "../lib/Chai.chpl"
    _coforallCount_chpl3 = new_temp_chpl2;
#line 250 "../lib/Chai.chpl"
    numTasks_chpl2 = chpl_boundedCoforallSize(tmpIter_chpl2, INT64(250), INT32(65));
#line 250 "../lib/Chai.chpl"
    _upEndCount_chpl(_coforallCount_chpl3, numTasks_chpl2, INT64(250), INT32(65));
#line 250 "../lib/Chai.chpl"
    chpl_resetTaskSpawn(numTasks_chpl2, INT64(250), INT32(65));
#line 250 "../lib/Chai.chpl"
    _ic__F0_this_chpl2 = tmpIter_chpl2;
#line 250 "../lib/Chai.chpl"
    checkIfIterWillOverflow_chpl(tmpIter_chpl2, UINT8(true), INT64(250), INT32(65));
#line 250 "../lib/Chai.chpl"
    i_chpl3 = INT64(0);
#line 250 "../lib/Chai.chpl"
    this_chpl34 = _ic__F0_this_chpl2;
#line 250 "../lib/Chai.chpl"
    this_chpl35 = _ic__F0_this_chpl2;
#line 250 "../lib/Chai.chpl"
    end_chpl2 = (&this_chpl35)->_high;
#line 250 "../lib/Chai.chpl"
    for (i_chpl3 = (&this_chpl34)->_low; ((i_chpl3 <= end_chpl2)); i_chpl3 += INT64(1)) {
#line 250 "../lib/Chai.chpl"
      _args_forcoforall_fn_chpl2 = ((_class_localscoforall_fn_chpl27)(&chpl_macro_tmp_104));
#line 250 "../lib/Chai.chpl"
      chpl_check_nil(_args_forcoforall_fn_chpl2, INT64(250), INT32(65));
#line 250 "../lib/Chai.chpl"
      (_args_forcoforall_fn_chpl2)->_0_numChunks = numChunks_chpl3;
#line 250 "../lib/Chai.chpl"
      chpl_check_nil(_args_forcoforall_fn_chpl2, INT64(250), INT32(65));
#line 250 "../lib/Chai.chpl"
      (_args_forcoforall_fn_chpl2)->_1_parDim = parDim_chpl2;
#line 250 "../lib/Chai.chpl"
      chpl_check_nil(_args_forcoforall_fn_chpl2, INT64(250), INT32(65));
#line 250 "../lib/Chai.chpl"
      *((_args_forcoforall_fn_chpl2)->_2_coerce_tmp + INT64(0)) = *(locBlock_chpl2 + INT64(0));
#line 250 "../lib/Chai.chpl"
      *((_args_forcoforall_fn_chpl2)->_2_coerce_tmp + INT64(1)) = *(locBlock_chpl2 + INT64(1));
#line 250 "../lib/Chai.chpl"
      chpl_check_nil(_args_forcoforall_fn_chpl2, INT64(250), INT32(65));
#line 250 "../lib/Chai.chpl"
      (_args_forcoforall_fn_chpl2)->_3__coforallCount = _coforallCount_chpl3;
#line 250 "../lib/Chai.chpl"
      chpl_check_nil(_args_forcoforall_fn_chpl2, INT64(250), INT32(65));
#line 250 "../lib/Chai.chpl"
      (_args_forcoforall_fn_chpl2)->_4_chunk = i_chpl3;
#line 250 "../lib/Chai.chpl"
      tmp_chpl6 = &call_tmp_chpl47;
#line 250 "../lib/Chai.chpl"
      chpl_check_nil(_args_forcoforall_fn_chpl2, INT64(250), INT32(65));
#line 250 "../lib/Chai.chpl"
      (_args_forcoforall_fn_chpl2)->_6_b = tmp_chpl6;
#line 250 "../lib/Chai.chpl"
      chpl_check_nil(_args_forcoforall_fn_chpl2, INT64(250), INT32(65));
#line 250 "../lib/Chai.chpl"
      (_args_forcoforall_fn_chpl2)->_7_a = *(i_a_chpl);
#line 250 "../lib/Chai.chpl"
      _args_vforcoforall_fn_chpl2 = ((void*)(_args_forcoforall_fn_chpl2));
#line 250 "../lib/Chai.chpl"
      _args_sizecoforall_fn_chpl2 = sizeof(chpl__class_localscoforall_fn_chpl27_object);
#line 250 "../lib/Chai.chpl"
      (_args_forcoforall_fn_chpl2)->_ln = INT64(250);
#line 250 "../lib/Chai.chpl"
      (_args_forcoforall_fn_chpl2)->_fn = INT32(65);
#line 250 "../lib/Chai.chpl"
      /*** wrapcoforall_fn_chpl16 ***/ chpl_taskAddCoStmt(chpl_task_getRequestedSubloc(), INT64(15), ((chpl_task_bundle_p)(_args_vforcoforall_fn_chpl2)), _args_sizecoforall_fn_chpl2, INT64(2763), INT32(30));
#line 250 "../lib/Chai.chpl"
    }
#line 250 "../lib/Chai.chpl"
    error_chpl7 = NULL;
#line 250 "../lib/Chai.chpl"
    _waitEndCount_chpl2(_coforallCount_chpl3, numTasks_chpl2, &error_chpl7, INT64(250), INT32(65));
#line 250 "../lib/Chai.chpl"
    if (error_chpl7 != nil) /* ZLINE: 250 ../lib/Chai.chpl */
#line 250 "../lib/Chai.chpl"
    {
#line 250 "../lib/Chai.chpl"
      chpl__delete72(_coforallCount_chpl3, INT64(250), INT32(65));
#line 250 "../lib/Chai.chpl"
      error_chpl6 = error_chpl7;
#line 250 "../lib/Chai.chpl"
      goto forall_IBB_break_label_chpl3;
#line 250 "../lib/Chai.chpl"
    }
#line 250 "../lib/Chai.chpl"
    chpl__delete72(_coforallCount_chpl3, INT64(250), INT32(65));
#line 250 "../lib/Chai.chpl"
    chpl_after_forall_fence();
#line 250 "../lib/Chai.chpl"
    forall_IBB_break_label_chpl3:;
#line 250 "../lib/Chai.chpl"
    if (error_chpl6 != nil) /* ZLINE: 250 ../lib/Chai.chpl */
#line 250 "../lib/Chai.chpl"
    {
#line 250 "../lib/Chai.chpl"
      error_chpl5 = error_chpl6;
#line 250 "../lib/Chai.chpl"
      goto forall_IBB_break_label_chpl4;
#line 250 "../lib/Chai.chpl"
    }
#line 250 "../lib/Chai.chpl"
    chpl_after_forall_fence();
#line 250 "../lib/Chai.chpl"
    forall_IBB_break_label_chpl4:;
#line 250 "../lib/Chai.chpl"
    if (error_chpl5 != nil) /* ZLINE: 250 ../lib/Chai.chpl */
#line 250 "../lib/Chai.chpl"
    {
#line 250 "../lib/Chai.chpl"
      error_chpl4 = error_chpl5;
#line 250 "../lib/Chai.chpl"
      goto forall_IBB_break_label_chpl5;
#line 250 "../lib/Chai.chpl"
    }
#line 250 "../lib/Chai.chpl"
    chpl_after_forall_fence();
#line 250 "../lib/Chai.chpl"
    forall_IBB_break_label_chpl5:;
#line 250 "../lib/Chai.chpl"
    if (error_chpl4 != nil) /* ZLINE: 250 ../lib/Chai.chpl */
#line 250 "../lib/Chai.chpl"
    {
#line 250 "../lib/Chai.chpl"
      chpl_propagate_error(error_chpl4, INT64(250), INT32(65));
#line 250 "../lib/Chai.chpl"
    }
#line 250 "../lib/Chai.chpl"
    chpl_after_forall_fence();
#line 250 "../lib/Chai.chpl"
    _field_destructor_tmp__chpl = (&call_tmp_chpl47)->data;
#line 250 "../lib/Chai.chpl"
    chpl__autoDestroy19(&_field_destructor_tmp__chpl, INT64(250), INT32(65));
#line 250 "../lib/Chai.chpl"
    _field_destructor_tmp__chpl2 = (&call_tmp_chpl47)->_domain;
#line 250 "../lib/Chai.chpl"
    chpl__autoDestroy5(&_field_destructor_tmp__chpl2, INT64(250), INT32(65));
#line 250 "../lib/Chai.chpl"
    chpl__autoDestroy15(&call_tmp_chpl45, INT64(250), INT32(65));
#line 250 "../lib/Chai.chpl"
  }
#line 248 "../lib/Chai.chpl"
  _field_destructor_tmp__chpl3 = (&filter_chpl)->data;
#line 248 "../lib/Chai.chpl"
  chpl__autoDestroy28(&_field_destructor_tmp__chpl3, INT64(248), INT32(65));
#line 248 "../lib/Chai.chpl"
  _field_destructor_tmp__chpl4 = (&filter_chpl)->_domain;
#line 248 "../lib/Chai.chpl"
  chpl__autoDestroy8(&_field_destructor_tmp__chpl4, INT64(248), INT32(65));
#line 248 "../lib/Chai.chpl"
  return;
#line 248 "../lib/Chai.chpl"
}

#line 285 "../lib/Chai.chpl"
/* Chai.chpl:285 */
#line 285 "../lib/Chai.chpl"
static void backwardBatch_chpl(Conv_chpl * this_chpl31,
#line 285 "../lib/Chai.chpl"
                               _array_DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl * deltas_chpl,
#line 285 "../lib/Chai.chpl"
                               _array_DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl * inputs_chpl,
#line 285 "../lib/Chai.chpl"
                               _array_DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl * _retArg_chpl) {
#line 289 "../lib/Chai.chpl"
  int64_t local_dataParMinGranularity_chpl;
#line 289 "../lib/Chai.chpl"
  chpl_bool local_dataParIgnoreRunningTasks_chpl;
#line 289 "../lib/Chai.chpl"
  int64_t local_dataParTasksPerLocale_chpl;
#line 286 "../lib/Chai.chpl"
  int64_t call_tmp_chpl6;
#line 287 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl newDeltas_chpl;
#line 287 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl call_tmp_chpl7;
#line 287 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl ret_tmp_chpl;
#line 287 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl8;
#line 287 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl2;
#line 287 "../lib/Chai.chpl"
  _domain_DefaultRectangularDom_1_int64_t_one_chpl call_tmp_chpl9;
#line 287 "../lib/Chai.chpl"
  _domain_DefaultRectangularDom_1_int64_t_one_chpl ret_tmp_chpl3;
#line 287 "../lib/Chai.chpl"
  chpl___RuntimeTypeInfo6 call_tmp_chpl10;
#line 287 "../lib/Chai.chpl"
  _domain_DefaultRectangularDom_1_int64_t_one_chpl _runtime_type_tmp__chpl;
#line 287 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl ret_tmp_chpl4;
#line 288 "../lib/Chai.chpl"
  _ref_Tensor_4__real64_chpl call_tmp_chpl11 = NULL;
#line 288 "../lib/Chai.chpl"
  Tensor_4__real64_chpl filtersGrad_chpl;
#line 289 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl call_tmp_chpl12;
#line 289 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl ret_tmp_chpl5;
#line 289 "../lib/Chai.chpl"
  SumReduceScanOp_Tensor_4__real64_chpl globalRP_filtersGrad_chpl = NULL;
#line 289 "../lib/Chai.chpl"
  SumReduceScanOp_Tensor_4__real64_chpl new_temp_chpl = NULL;
#line 289 "../lib/Chai.chpl"
  chpl_LocalSpinlock default_arg_l_chpl;
#line 289 "../lib/Chai.chpl"
  chpl_LocalSpinlock ret_chpl;
#line 289 "../lib/Chai.chpl"
  chpl_LocalSpinlock l_chpl;
#line 289 "../lib/Chai.chpl"
  Tensor_4__real64_chpl default_arg_value_chpl;
#line 289 "../lib/Chai.chpl"
  Tensor_4__real64_chpl ret_chpl2;
#line 289 "../lib/Chai.chpl"
  Tensor_4__real64_chpl value_chpl;
#line 289 "../lib/Chai.chpl"
  SumReduceScanOp_Tensor_4__real64_chpl coerce_tmp_chpl3 = NULL;
#line 289 "../lib/Chai.chpl"
  _ref_Tensor_4__real64_chpl i_x_chpl = NULL;
#line 289 "../lib/Chai.chpl"
  _ref_Tensor_4__real64_chpl call_tmp_chpl13 = NULL;
#line 289 "../lib/Chai.chpl"
  Tensor_4__real64_chpl ret_chpl3;
#line 289 "../lib/Chai.chpl"
  Tensor_4__real64_chpl x_chpl;
#line 289 "../lib/Chai.chpl"
  Tensor_4__real64_chpl globalAS_filtersGrad_chpl;
#line 289 "../lib/Chai.chpl"
  _ref_Tensor_4__real64_chpl call_tmp_chpl14 = NULL;
#line 289 "../lib/Chai.chpl"
  int64_t stride_svar_chpl;
#line 289 "../lib/Chai.chpl"
  int64_t padding_svar_chpl;
#line 289 "../lib/Chai.chpl"
  DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl coerce_tmp_chpl4 = NULL;
#line 289 "../lib/Chai.chpl"
  int64_t tasksPerLocale_chpl;
#line 289 "../lib/Chai.chpl"
  chpl_bool ignoreRunning_chpl;
#line 289 "../lib/Chai.chpl"
  int64_t minIndicesPerTask_chpl;
#line 289 "../lib/Chai.chpl"
  Error_chpl error_chpl = NULL;
#line 289 "../lib/Chai.chpl"
  DefaultRectangularDom_1_int64_t_one_chpl coerce_tmp_chpl5 = NULL;
#line 289 "../lib/Chai.chpl"
  int64_t offset_x0_chpl;
#line 289 "../lib/Chai.chpl"
  _tuple_1_star_int64_t_chpl ret_tmp_chpl6;
#line 289 "../lib/Chai.chpl"
  Error_chpl error_chpl2 = NULL;
#line 289 "../lib/Chai.chpl"
  locale_chpl call_tmp_chpl15;
#line 289 "../lib/Chai.chpl"
  chpl_localeID_t call_tmp_chpl16;
#line 289 "../lib/Chai.chpl"
  int32_t call_tmp_chpl17;
#line 289 "../lib/Chai.chpl"
  chpl_localeID_t call_tmp_chpl18;
#line 289 "../lib/Chai.chpl"
  locale_chpl call_tmp_chpl19;
#line 289 "../lib/Chai.chpl"
  locale_chpl ret_tmp_chpl7;
#line 289 "../lib/Chai.chpl"
  BaseLocale_chpl coerce_tmp_chpl6 = NULL;
#line 289 "../lib/Chai.chpl"
  _ref_locale_chpl i_x_chpl2 = NULL;
#line 289 "../lib/Chai.chpl"
  int64_t tmp_chpl;
#line 289 "../lib/Chai.chpl"
  locale_chpl call_tmp_chpl20;
#line 289 "../lib/Chai.chpl"
  chpl_localeID_t call_tmp_chpl21;
#line 289 "../lib/Chai.chpl"
  int32_t call_tmp_chpl22;
#line 289 "../lib/Chai.chpl"
  chpl_localeID_t call_tmp_chpl23;
#line 289 "../lib/Chai.chpl"
  locale_chpl call_tmp_chpl24;
#line 289 "../lib/Chai.chpl"
  locale_chpl ret_tmp_chpl8;
#line 289 "../lib/Chai.chpl"
  int64_t ret_chpl4;
#line 289 "../lib/Chai.chpl"
  BaseLocale_chpl coerce_tmp_chpl7 = NULL;
#line 289 "../lib/Chai.chpl"
  BaseLocale_chpl coerce_tmp_chpl8 = NULL;
#line 289 "../lib/Chai.chpl"
  _ref_locale_chpl i_x_chpl3 = NULL;
#line 289 "../lib/Chai.chpl"
  int64_t tmp_x0_chpl;
#line 289 "../lib/Chai.chpl"
  int64_t tmp_x1_chpl;
#line 289 "../lib/Chai.chpl"
  _ref__tuple_1_star_range_int64_t_both_one_chpl call_tmp_chpl25 = NULL;
#line 289 "../lib/Chai.chpl"
  _tuple_2_star_int64_t_chpl ret_tmp_chpl9;
#line 289 "../lib/Chai.chpl"
  int64_t numChunks_chpl;
#line 289 "../lib/Chai.chpl"
  int64_t parDim_chpl;
#line 289 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl locBlock_x0_chpl;
#line 289 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl tup_x0_chpl;
#line 289 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl elt_x0_chpl;
#line 289 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl call_tmp_chpl26;
#line 289 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl ret_tmp_chpl10;
#line 289 "../lib/Chai.chpl"
  _ref__tuple_1_star_range_int64_t_both_one_chpl call_tmp_chpl27 = NULL;
#line 289 "../lib/Chai.chpl"
  int64_t call_tmp_chpl28;
#line 289 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl9;
#line 289 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl29;
#line 289 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl11;
#line 289 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl tmpIter_chpl;
#line 289 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl call_tmp_chpl30;
#line 289 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl ret_tmp_chpl12;
#line 289 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl13;
#line 289 "../lib/Chai.chpl"
  chpl___EndCount_AtomicT_int64_t_int64_t _coforallCount_chpl = NULL;
#line 289 "../lib/Chai.chpl"
  chpl___EndCount_AtomicT_int64_t_int64_t new_temp_chpl2 = NULL;
#line 289 "../lib/Chai.chpl"
  int64_t numTasks_chpl;
#line 289 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl _ic__F0_this_chpl;
#line 289 "../lib/Chai.chpl"
  int64_t i_chpl;
#line 289 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl this_chpl32;
#line 289 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl this_chpl33;
#line 289 "../lib/Chai.chpl"
  int64_t end_chpl;
#line 289 "../lib/Chai.chpl"
  _tuple_1_star_range_int64_t_both_one_chpl ret_tmp_chpl14;
#line 289 "../lib/Chai.chpl"
  _class_localscoforall_fn_chpl101 _args_forcoforall_fn_chpl = NULL;
#line 289 "../lib/Chai.chpl"
  _ref_Tensor_4__real64_chpl tmp_chpl2 = NULL;
#line 289 "../lib/Chai.chpl"
  _ref_range_int64_t_low_one_chpl tmp_chpl3 = NULL;
#line 289 "../lib/Chai.chpl"
  raw_c_void_ptr _args_vforcoforall_fn_chpl;
#line 289 "../lib/Chai.chpl"
  Error_chpl error_chpl3 = NULL;
#line 289 "../lib/Chai.chpl"
  SumReduceScanOp_Tensor_4__real64_chpl coerce_tmp_chpl10 = NULL;
#line 289 "../lib/Chai.chpl"
  _ref_Tensor_4__real64_chpl i_x_chpl4 = NULL;
#line 289 "../lib/Chai.chpl"
  _ref_Tensor_4__real64_chpl call_tmp_chpl31 = NULL;
#line 289 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_4_int64_t_one__real64_int64_t_chpl _field_destructor_tmp__chpl;
#line 289 "../lib/Chai.chpl"
  _domain_DefaultRectangularDom_4_int64_t_one_chpl _field_destructor_tmp__chpl2;
#line 289 "../lib/Chai.chpl"
  Tensor_4__real64_chpl chpl_gentemp;
#line 289 "../lib/Chai.chpl"
  SumReduceScanOp_Tensor_4__real64_chpl coerce_tmp_chpl11 = NULL;
#line 289 "../lib/Chai.chpl"
  _ref_Tensor_4__real64_chpl call_tmp_chpl32 = NULL;
#line 289 "../lib/Chai.chpl"
  Tensor_4__real64_chpl ret_chpl5;
#line 289 "../lib/Chai.chpl"
  Tensor_4__real64_chpl tmp_chpl4;
#line 289 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_4_int64_t_one__real64_int64_t_chpl _field_destructor_tmp__chpl3;
#line 289 "../lib/Chai.chpl"
  _domain_DefaultRectangularDom_4_int64_t_one_chpl _field_destructor_tmp__chpl4;
#line 313 "../lib/Chai.chpl"
  _ref_Tensor_4__real64_chpl call_tmp_chpl33 = NULL;
#line 313 "../lib/Chai.chpl"
  _ref__array_DefaultRectangularArr_4_int64_t_one__real64_int64_t_chpl call_tmp_chpl34 = NULL;
#line 313 "../lib/Chai.chpl"
  _ref__array_DefaultRectangularArr_4_int64_t_one__real64_int64_t_chpl call_tmp_chpl35 = NULL;
#line 313 "../lib/Chai.chpl"
  uint64_t call_tmp_chpl36;
#line 313 "../lib/Chai.chpl"
  chpl_bool tmp_chpl5;
#line 313 "../lib/Chai.chpl"
  chpl_bool tmp_chpl6;
#line 313 "../lib/Chai.chpl"
  uint64_t call_tmp_chpl37;
#line 313 "../lib/Chai.chpl"
  chpl_bool tmp_chpl7;
#line 313 "../lib/Chai.chpl"
  _domain_DefaultRectangularDom_4_int64_t_one_chpl call_tmp_chpl38;
#line 313 "../lib/Chai.chpl"
  _domain_DefaultRectangularDom_4_int64_t_one_chpl ret_tmp_chpl15;
#line 313 "../lib/Chai.chpl"
  _domain_DefaultRectangularDom_4_int64_t_one_chpl call_tmp_chpl39;
#line 313 "../lib/Chai.chpl"
  _domain_DefaultRectangularDom_4_int64_t_one_chpl ret_tmp_chpl16;
#line 313 "../lib/Chai.chpl"
  _ref__domain_DefaultRectangularDom_4_int64_t_one_chpl i_AD_chpl = NULL;
#line 313 "../lib/Chai.chpl"
  _ref__domain_DefaultRectangularDom_4_int64_t_one_chpl i_BD_chpl = NULL;
#line 313 "../lib/Chai.chpl"
  DefaultRectangularArr_4_int64_t_one__real64_int64_t_chpl ret_chpl6 = NULL;
#line 313 "../lib/Chai.chpl"
  DefaultRectangularArr_4_int64_t_one__real64_int64_t_chpl ret_chpl7 = NULL;
#line 313 "../lib/Chai.chpl"
  chpl_bool call_tmp_chpl40;
#line 313 "../lib/Chai.chpl"
  DefaultRectangularArr_4_int64_t_one__real64_int64_t_chpl coerce_tmp_chpl12 = NULL;
#line 313 "../lib/Chai.chpl"
  int64_t tasksPerLocale_chpl2;
#line 313 "../lib/Chai.chpl"
  chpl_bool ignoreRunning_chpl2;
#line 313 "../lib/Chai.chpl"
  int64_t minIndicesPerTask_chpl2;
#line 313 "../lib/Chai.chpl"
  Error_chpl error_chpl4 = NULL;
#line 313 "../lib/Chai.chpl"
  DefaultRectangularDom_4_int64_t_one_chpl coerce_tmp_chpl13 = NULL;
#line 313 "../lib/Chai.chpl"
  int64_t offset_x0_chpl2;
#line 313 "../lib/Chai.chpl"
  int64_t offset_x1_chpl;
#line 313 "../lib/Chai.chpl"
  int64_t offset_x2_chpl;
#line 313 "../lib/Chai.chpl"
  int64_t offset_x3_chpl;
#line 313 "../lib/Chai.chpl"
  _tuple_4_star_int64_t_chpl ret_tmp_chpl17;
#line 313 "../lib/Chai.chpl"
  Error_chpl error_chpl5 = NULL;
#line 313 "../lib/Chai.chpl"
  locale_chpl call_tmp_chpl41;
#line 313 "../lib/Chai.chpl"
  chpl_localeID_t call_tmp_chpl42;
#line 313 "../lib/Chai.chpl"
  int32_t call_tmp_chpl43;
#line 313 "../lib/Chai.chpl"
  chpl_localeID_t call_tmp_chpl44;
#line 313 "../lib/Chai.chpl"
  locale_chpl call_tmp_chpl45;
#line 313 "../lib/Chai.chpl"
  locale_chpl ret_tmp_chpl18;
#line 313 "../lib/Chai.chpl"
  BaseLocale_chpl coerce_tmp_chpl14 = NULL;
#line 313 "../lib/Chai.chpl"
  _ref_locale_chpl i_x_chpl5 = NULL;
#line 313 "../lib/Chai.chpl"
  int64_t tmp_chpl8;
#line 313 "../lib/Chai.chpl"
  locale_chpl call_tmp_chpl46;
#line 313 "../lib/Chai.chpl"
  chpl_localeID_t call_tmp_chpl47;
#line 313 "../lib/Chai.chpl"
  int32_t call_tmp_chpl48;
#line 313 "../lib/Chai.chpl"
  chpl_localeID_t call_tmp_chpl49;
#line 313 "../lib/Chai.chpl"
  locale_chpl call_tmp_chpl50;
#line 313 "../lib/Chai.chpl"
  locale_chpl ret_tmp_chpl19;
#line 313 "../lib/Chai.chpl"
  int64_t ret_chpl8;
#line 313 "../lib/Chai.chpl"
  BaseLocale_chpl coerce_tmp_chpl15 = NULL;
#line 313 "../lib/Chai.chpl"
  BaseLocale_chpl coerce_tmp_chpl16 = NULL;
#line 313 "../lib/Chai.chpl"
  _ref_locale_chpl i_x_chpl6 = NULL;
#line 313 "../lib/Chai.chpl"
  int64_t tmp_x0_chpl2;
#line 313 "../lib/Chai.chpl"
  int64_t tmp_x1_chpl2;
#line 313 "../lib/Chai.chpl"
  _ref__tuple_4_star_range_int64_t_both_one_chpl call_tmp_chpl51 = NULL;
#line 313 "../lib/Chai.chpl"
  _tuple_2_star_int64_t_chpl ret_tmp_chpl20;
#line 313 "../lib/Chai.chpl"
  int64_t numChunks_chpl2;
#line 313 "../lib/Chai.chpl"
  int64_t parDim_chpl2;
#line 313 "../lib/Chai.chpl"
  _tuple_4_star_range_int64_t_both_one_chpl locBlock_chpl;
#line 313 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl tup_x0_chpl2;
#line 313 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl tup_x1_chpl;
#line 313 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl tup_x2_chpl;
#line 313 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl tup_x3_chpl;
#line 313 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl elt_x0_chpl2;
#line 313 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl elt_x1_chpl;
#line 313 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl elt_x2_chpl;
#line 313 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl elt_x3_chpl;
#line 313 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl call_tmp_chpl52;
#line 313 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl ret_tmp_chpl21;
#line 313 "../lib/Chai.chpl"
  _ref__tuple_4_star_range_int64_t_both_one_chpl call_tmp_chpl53 = NULL;
#line 313 "../lib/Chai.chpl"
  int64_t call_tmp_chpl54;
#line 313 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl17;
#line 313 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl55;
#line 313 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl22;
#line 313 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl call_tmp_chpl56;
#line 313 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl ret_tmp_chpl23;
#line 313 "../lib/Chai.chpl"
  _ref__tuple_4_star_range_int64_t_both_one_chpl call_tmp_chpl57 = NULL;
#line 313 "../lib/Chai.chpl"
  int64_t call_tmp_chpl58;
#line 313 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl18;
#line 313 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl59;
#line 313 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl24;
#line 313 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl call_tmp_chpl60;
#line 313 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl ret_tmp_chpl25;
#line 313 "../lib/Chai.chpl"
  _ref__tuple_4_star_range_int64_t_both_one_chpl call_tmp_chpl61 = NULL;
#line 313 "../lib/Chai.chpl"
  int64_t call_tmp_chpl62;
#line 313 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl19;
#line 313 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl63;
#line 313 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl26;
#line 313 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl call_tmp_chpl64;
#line 313 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl ret_tmp_chpl27;
#line 313 "../lib/Chai.chpl"
  _ref__tuple_4_star_range_int64_t_both_one_chpl call_tmp_chpl65 = NULL;
#line 313 "../lib/Chai.chpl"
  int64_t call_tmp_chpl66;
#line 313 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl20;
#line 313 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl67;
#line 313 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl28;
#line 313 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl tmpIter_chpl2;
#line 313 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl call_tmp_chpl68;
#line 313 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl ret_tmp_chpl29;
#line 313 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl30;
#line 313 "../lib/Chai.chpl"
  chpl___EndCount_AtomicT_int64_t_int64_t _coforallCount_chpl2 = NULL;
#line 313 "../lib/Chai.chpl"
  chpl___EndCount_AtomicT_int64_t_int64_t new_temp_chpl3 = NULL;
#line 313 "../lib/Chai.chpl"
  int64_t numTasks_chpl2;
#line 313 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl _ic__F0_this_chpl2;
#line 313 "../lib/Chai.chpl"
  int64_t i_chpl2;
#line 313 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl this_chpl34;
#line 313 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl this_chpl35;
#line 313 "../lib/Chai.chpl"
  int64_t end_chpl2;
#line 313 "../lib/Chai.chpl"
  _class_localscoforall_fn_chpl33 _args_forcoforall_fn_chpl2 = NULL;
#line 313 "../lib/Chai.chpl"
  raw_c_void_ptr _args_vforcoforall_fn_chpl2;
#line 313 "../lib/Chai.chpl"
  int64_t _args_sizecoforall_fn_chpl;
#line 313 "../lib/Chai.chpl"
  Error_chpl error_chpl6 = NULL;
#line 314 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_4_int64_t_one__real64_int64_t_chpl _field_destructor_tmp__chpl5;
#line 314 "../lib/Chai.chpl"
  _domain_DefaultRectangularDom_4_int64_t_one_chpl _field_destructor_tmp__chpl6;
#line 314 "../lib/Chai.chpl"
  chpl__class_localscoforall_fn_chpl101_object chpl_macro_tmp_105;
#line 314 "../lib/Chai.chpl"
  chpl__class_localscoforall_fn_chpl33_object chpl_macro_tmp_106;
#line 289 "../lib/Chai.chpl"
  local_dataParMinGranularity_chpl = dataParMinGranularity_chpl;
#line 289 "../lib/Chai.chpl"
  local_dataParIgnoreRunningTasks_chpl = dataParIgnoreRunningTasks_chpl;
#line 289 "../lib/Chai.chpl"
  local_dataParTasksPerLocale_chpl = dataParTasksPerLocale_chpl;
#line 286 "../lib/Chai.chpl"
  call_tmp_chpl6 = size_chpl(deltas_chpl, INT64(286), INT32(65));
#line 287 "../lib/Chai.chpl"
  chpl_build_low_bounded_range2(INT64(0), &ret_tmp_chpl);
#line 287 "../lib/Chai.chpl"
  call_tmp_chpl7 = ret_tmp_chpl;
#line 287 "../lib/Chai.chpl"
  chpl___POUND_3(call_tmp_chpl7, call_tmp_chpl6, &ret_tmp_chpl2, INT64(287), INT32(65));
#line 287 "../lib/Chai.chpl"
  call_tmp_chpl8 = ret_tmp_chpl2;
#line 287 "../lib/Chai.chpl"
  chpl__ensureDomainExpr5(call_tmp_chpl8, &ret_tmp_chpl3, INT64(287), INT32(65));
#line 287 "../lib/Chai.chpl"
  call_tmp_chpl9 = ret_tmp_chpl3;
#line 287 "../lib/Chai.chpl"
  call_tmp_chpl10 = chpl__buildArrayRuntimeType2(&call_tmp_chpl9);
#line 287 "../lib/Chai.chpl"
  _runtime_type_tmp__chpl = (&call_tmp_chpl10)->dom;
#line 287 "../lib/Chai.chpl"
  chpl__convertRuntimeTypeToValue3(&_runtime_type_tmp__chpl, UINT8(false), &ret_tmp_chpl4, INT64(287), INT32(65));
#line 287 "../lib/Chai.chpl"
  newDeltas_chpl = ret_tmp_chpl4;
#line 288 "../lib/Chai.chpl"
  call_tmp_chpl11 = &((this_chpl31)->filtersGrad);
#line 288 "../lib/Chai.chpl"
  init_ASSIGN__chpl29(&filtersGrad_chpl, call_tmp_chpl11);
#line 289 "../lib/Chai.chpl"
  chpl_build_low_bounded_range2(INT64(0), &ret_tmp_chpl5);
#line 289 "../lib/Chai.chpl"
  call_tmp_chpl12 = ret_tmp_chpl5;
#line 289 "../lib/Chai.chpl"
  init_chpl105(&l_chpl);
#line 289 "../lib/Chai.chpl"
  ret_chpl = l_chpl;
#line 289 "../lib/Chai.chpl"
  default_arg_l_chpl = ret_chpl;
#line 289 "../lib/Chai.chpl"
  init_chpl234(&value_chpl);
#line 289 "../lib/Chai.chpl"
  ret_chpl2 = value_chpl;
#line 289 "../lib/Chai.chpl"
  default_arg_value_chpl = ret_chpl2;
#line 289 "../lib/Chai.chpl"
  new_temp_chpl = _new_chpl20(&default_arg_l_chpl, &default_arg_value_chpl, INT64(289), INT32(65));
#line 289 "../lib/Chai.chpl"
  globalRP_filtersGrad_chpl = new_temp_chpl;
#line 289 "../lib/Chai.chpl"
  coerce_tmp_chpl3 = globalRP_filtersGrad_chpl;
#line 289 "../lib/Chai.chpl"
  i_x_chpl = &filtersGrad_chpl;
#line 289 "../lib/Chai.chpl"
  chpl_check_nil(coerce_tmp_chpl3, INT64(289), INT32(65));
#line 289 "../lib/Chai.chpl"
  call_tmp_chpl13 = &((coerce_tmp_chpl3)->value);
#line 289 "../lib/Chai.chpl"
  chpl___PLUS__ASSIGN_3(call_tmp_chpl13, i_x_chpl);
#line 289 "../lib/Chai.chpl"
  init_chpl234(&x_chpl);
#line 289 "../lib/Chai.chpl"
  ret_chpl3 = x_chpl;
#line 289 "../lib/Chai.chpl"
  globalAS_filtersGrad_chpl = ret_chpl3;
#line 289 "../lib/Chai.chpl"
  call_tmp_chpl14 = &((this_chpl31)->filters);
#line 289 "../lib/Chai.chpl"
  stride_svar_chpl = (this_chpl31)->stride;
#line 289 "../lib/Chai.chpl"
  padding_svar_chpl = (this_chpl31)->padding;
#line 289 "../lib/Chai.chpl"
  coerce_tmp_chpl4 = (deltas_chpl)->_instance;
#line 289 "../lib/Chai.chpl"
  tasksPerLocale_chpl = local_dataParTasksPerLocale_chpl;
#line 289 "../lib/Chai.chpl"
  ignoreRunning_chpl = local_dataParIgnoreRunningTasks_chpl;
#line 289 "../lib/Chai.chpl"
  minIndicesPerTask_chpl = local_dataParMinGranularity_chpl;
#line 289 "../lib/Chai.chpl"
  error_chpl = NULL;
#line 289 "../lib/Chai.chpl"
  chpl_check_nil(coerce_tmp_chpl4, INT64(289), INT32(65));
#line 289 "../lib/Chai.chpl"
  coerce_tmp_chpl5 = (coerce_tmp_chpl4)->dom;
#line 289 "../lib/Chai.chpl"
  createTuple_chpl(INT64(0), &ret_tmp_chpl6);
#line 289 "../lib/Chai.chpl"
  offset_x0_chpl = *(ret_tmp_chpl6 + INT64(0));
#line 289 "../lib/Chai.chpl"
  error_chpl2 = NULL;
#line 289 "../lib/Chai.chpl"
  call_tmp_chpl17 = get_chpl_nodeID();
#line 289 "../lib/Chai.chpl"
  call_tmp_chpl18 = chpl_rt_buildLocaleID(call_tmp_chpl17, c_sublocid_any);
#line 289 "../lib/Chai.chpl"
  call_tmp_chpl16 = call_tmp_chpl18;
#line 289 "../lib/Chai.chpl"
  chpl_localeID_to_locale(&call_tmp_chpl16, &ret_tmp_chpl7, INT64(289), INT32(65));
#line 289 "../lib/Chai.chpl"
  call_tmp_chpl19 = ret_tmp_chpl7;
#line 289 "../lib/Chai.chpl"
  call_tmp_chpl15 = call_tmp_chpl19;
#line 289 "../lib/Chai.chpl"
  coerce_tmp_chpl6 = (&call_tmp_chpl15)->_instance;
#line 289 "../lib/Chai.chpl"
  if (((RootClass_chpl)(coerce_tmp_chpl6)) == nil) /* ZLINE: 289 ../lib/Chai.chpl */
#line 289 "../lib/Chai.chpl"
  {
#line 289 "../lib/Chai.chpl"
    nilCheckHalt_chpl(&_str_literal_52_chpl, INT64(289), INT32(65));
#line 289 "../lib/Chai.chpl"
  }
#line 289 "../lib/Chai.chpl"
  chpl_check_nil(coerce_tmp_chpl6, INT64(289), INT32(65));
#line 289 "../lib/Chai.chpl"
  ((int64_t(*)(BaseLocale_chpl,
#line 289 "../lib/Chai.chpl"
     int64_t,
#line 289 "../lib/Chai.chpl"
     int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(coerce_tmp_chpl6))->chpl__cid) + INT64(2))])(coerce_tmp_chpl6, INT64(289), INT32(65));
#line 289 "../lib/Chai.chpl"
  i_x_chpl2 = &call_tmp_chpl15;
#line 289 "../lib/Chai.chpl"
  deinit_chpl62(i_x_chpl2);
#line 289 "../lib/Chai.chpl"
  if (tasksPerLocale_chpl == INT64(0)) /* ZLINE: 289 ../lib/Chai.chpl */
#line 289 "../lib/Chai.chpl"
  {
#line 289 "../lib/Chai.chpl"
    call_tmp_chpl22 = get_chpl_nodeID();
#line 289 "../lib/Chai.chpl"
    call_tmp_chpl23 = chpl_rt_buildLocaleID(call_tmp_chpl22, c_sublocid_any);
#line 289 "../lib/Chai.chpl"
    call_tmp_chpl21 = call_tmp_chpl23;
#line 289 "../lib/Chai.chpl"
    chpl_localeID_to_locale(&call_tmp_chpl21, &ret_tmp_chpl8, INT64(289), INT32(65));
#line 289 "../lib/Chai.chpl"
    call_tmp_chpl24 = ret_tmp_chpl8;
#line 289 "../lib/Chai.chpl"
    call_tmp_chpl20 = call_tmp_chpl24;
#line 289 "../lib/Chai.chpl"
    coerce_tmp_chpl7 = (&call_tmp_chpl20)->_instance;
#line 289 "../lib/Chai.chpl"
    if (((RootClass_chpl)(coerce_tmp_chpl7)) == nil) /* ZLINE: 289 ../lib/Chai.chpl */
#line 289 "../lib/Chai.chpl"
    {
#line 289 "../lib/Chai.chpl"
      nilCheckHalt_chpl(&_str_literal_52_chpl, INT64(289), INT32(65));
#line 289 "../lib/Chai.chpl"
    }
#line 289 "../lib/Chai.chpl"
    coerce_tmp_chpl8 = coerce_tmp_chpl7;
#line 289 "../lib/Chai.chpl"
    chpl_check_nil(coerce_tmp_chpl8, INT64(289), INT32(65));
#line 289 "../lib/Chai.chpl"
    ret_chpl4 = (coerce_tmp_chpl8)->maxTaskPar;
#line 289 "../lib/Chai.chpl"
    tmp_chpl = ret_chpl4;
#line 289 "../lib/Chai.chpl"
    i_x_chpl3 = &call_tmp_chpl20;
#line 289 "../lib/Chai.chpl"
    deinit_chpl62(i_x_chpl3);
#line 289 "../lib/Chai.chpl"
  } else /* ZLINE: 289 ../lib/Chai.chpl */
#line 289 "../lib/Chai.chpl"
  {
#line 289 "../lib/Chai.chpl"
    tmp_chpl = tasksPerLocale_chpl;
#line 289 "../lib/Chai.chpl"
  }
#line 289 "../lib/Chai.chpl"
  if (chpl_task_getSerial()) /* ZLINE: 289 ../lib/Chai.chpl */
#line 289 "../lib/Chai.chpl"
  {
#line 289 "../lib/Chai.chpl"
    tmp_x0_chpl = INT64(1);
#line 289 "../lib/Chai.chpl"
    tmp_x1_chpl = INT64(0);
#line 289 "../lib/Chai.chpl"
  } else /* ZLINE: 289 ../lib/Chai.chpl */
#line 289 "../lib/Chai.chpl"
  {
#line 289 "../lib/Chai.chpl"
    chpl_check_nil(coerce_tmp_chpl5, INT64(289), INT32(65));
#line 289 "../lib/Chai.chpl"
    call_tmp_chpl25 = &((coerce_tmp_chpl5)->ranges);
#line 289 "../lib/Chai.chpl"
    _computeChunkStuff_chpl(tmp_chpl, ignoreRunning_chpl, minIndicesPerTask_chpl, call_tmp_chpl25, &ret_tmp_chpl9, INT64(289), INT32(65));
#line 289 "../lib/Chai.chpl"
    tmp_x0_chpl = *(ret_tmp_chpl9 + INT64(0));
#line 289 "../lib/Chai.chpl"
    tmp_x1_chpl = *(ret_tmp_chpl9 + INT64(1));
#line 289 "../lib/Chai.chpl"
  }
#line 289 "../lib/Chai.chpl"
  numChunks_chpl = tmp_x0_chpl;
#line 289 "../lib/Chai.chpl"
  parDim_chpl = tmp_x1_chpl;
#line 289 "../lib/Chai.chpl"
  init_chpl113(&elt_x0_chpl);
#line 289 "../lib/Chai.chpl"
  tup_x0_chpl = elt_x0_chpl;
#line 289 "../lib/Chai.chpl"
  locBlock_x0_chpl = tup_x0_chpl;
#line 289 "../lib/Chai.chpl"
  chpl_build_low_bounded_range2(offset_x0_chpl, &ret_tmp_chpl10);
#line 289 "../lib/Chai.chpl"
  call_tmp_chpl26 = ret_tmp_chpl10;
#line 289 "../lib/Chai.chpl"
  chpl_check_nil(coerce_tmp_chpl5, INT64(289), INT32(65));
#line 289 "../lib/Chai.chpl"
  call_tmp_chpl27 = &((coerce_tmp_chpl5)->ranges);
#line 289 "../lib/Chai.chpl"
  coerce_tmp_chpl9 = *(*(call_tmp_chpl27) + INT64(0));
#line 289 "../lib/Chai.chpl"
  call_tmp_chpl28 = sizeAs_chpl37(coerce_tmp_chpl9, INT64(289), INT32(65));
#line 289 "../lib/Chai.chpl"
  chpl___POUND_3(call_tmp_chpl26, call_tmp_chpl28, &ret_tmp_chpl11, INT64(289), INT32(65));
#line 289 "../lib/Chai.chpl"
  call_tmp_chpl29 = ret_tmp_chpl11;
#line 289 "../lib/Chai.chpl"
  locBlock_x0_chpl = call_tmp_chpl29;
#line 289 "../lib/Chai.chpl"
  chpl_build_low_bounded_range2(INT64(0), &ret_tmp_chpl12);
#line 289 "../lib/Chai.chpl"
  call_tmp_chpl30 = ret_tmp_chpl12;
#line 289 "../lib/Chai.chpl"
  chpl___POUND_3(call_tmp_chpl30, tmp_x0_chpl, &ret_tmp_chpl13, INT64(289), INT32(65));
#line 289 "../lib/Chai.chpl"
  tmpIter_chpl = ret_tmp_chpl13;
#line 289 "../lib/Chai.chpl"
  new_temp_chpl2 = _new_chpl17(INT64(289), INT32(65));
#line 289 "../lib/Chai.chpl"
  _coforallCount_chpl = new_temp_chpl2;
#line 289 "../lib/Chai.chpl"
  numTasks_chpl = chpl_boundedCoforallSize(tmpIter_chpl, INT64(289), INT32(65));
#line 289 "../lib/Chai.chpl"
  _upEndCount_chpl(_coforallCount_chpl, numTasks_chpl, INT64(289), INT32(65));
#line 289 "../lib/Chai.chpl"
  chpl_resetTaskSpawn(numTasks_chpl, INT64(289), INT32(65));
#line 289 "../lib/Chai.chpl"
  _ic__F0_this_chpl = tmpIter_chpl;
#line 289 "../lib/Chai.chpl"
  checkIfIterWillOverflow_chpl(tmpIter_chpl, UINT8(true), INT64(289), INT32(65));
#line 289 "../lib/Chai.chpl"
  i_chpl = INT64(0);
#line 289 "../lib/Chai.chpl"
  this_chpl32 = _ic__F0_this_chpl;
#line 289 "../lib/Chai.chpl"
  this_chpl33 = _ic__F0_this_chpl;
#line 289 "../lib/Chai.chpl"
  end_chpl = (&this_chpl33)->_high;
#line 289 "../lib/Chai.chpl"
  for (i_chpl = (&this_chpl32)->_low; ((i_chpl <= end_chpl)); i_chpl += INT64(1)) {
#line 289 "../lib/Chai.chpl"
    *(ret_tmp_chpl14 + INT64(0)) = locBlock_x0_chpl;
#line 289 "../lib/Chai.chpl"
    _args_forcoforall_fn_chpl = ((_class_localscoforall_fn_chpl101)(&chpl_macro_tmp_105));
#line 289 "../lib/Chai.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl, INT64(289), INT32(65));
#line 289 "../lib/Chai.chpl"
    (_args_forcoforall_fn_chpl)->_0_numChunks = numChunks_chpl;
#line 289 "../lib/Chai.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl, INT64(289), INT32(65));
#line 289 "../lib/Chai.chpl"
    (_args_forcoforall_fn_chpl)->_1_parDim = parDim_chpl;
#line 289 "../lib/Chai.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl, INT64(289), INT32(65));
#line 289 "../lib/Chai.chpl"
    *((_args_forcoforall_fn_chpl)->_2_coerce_tmp + INT64(0)) = *(ret_tmp_chpl14 + INT64(0));
#line 289 "../lib/Chai.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl, INT64(289), INT32(65));
#line 289 "../lib/Chai.chpl"
    (_args_forcoforall_fn_chpl)->_3__coforallCount = _coforallCount_chpl;
#line 289 "../lib/Chai.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl, INT64(289), INT32(65));
#line 289 "../lib/Chai.chpl"
    (_args_forcoforall_fn_chpl)->_4_chunk = i_chpl;
#line 289 "../lib/Chai.chpl"
    tmp_chpl2 = call_tmp_chpl14;
#line 289 "../lib/Chai.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl, INT64(289), INT32(65));
#line 289 "../lib/Chai.chpl"
    (_args_forcoforall_fn_chpl)->_6_filters_ref = tmp_chpl2;
#line 289 "../lib/Chai.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl, INT64(289), INT32(65));
#line 289 "../lib/Chai.chpl"
    (_args_forcoforall_fn_chpl)->_7_stride_svar = stride_svar_chpl;
#line 289 "../lib/Chai.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl, INT64(289), INT32(65));
#line 289 "../lib/Chai.chpl"
    (_args_forcoforall_fn_chpl)->_8_padding_svar = padding_svar_chpl;
#line 289 "../lib/Chai.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl, INT64(289), INT32(65));
#line 289 "../lib/Chai.chpl"
    (_args_forcoforall_fn_chpl)->_9_globalRP_filtersGrad = globalRP_filtersGrad_chpl;
#line 289 "../lib/Chai.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl, INT64(289), INT32(65));
#line 289 "../lib/Chai.chpl"
    (_args_forcoforall_fn_chpl)->_10_deltas = *(deltas_chpl);
#line 289 "../lib/Chai.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl, INT64(289), INT32(65));
#line 289 "../lib/Chai.chpl"
    (_args_forcoforall_fn_chpl)->_11_inputs = *(inputs_chpl);
#line 289 "../lib/Chai.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl, INT64(289), INT32(65));
#line 289 "../lib/Chai.chpl"
    (_args_forcoforall_fn_chpl)->_12_newDeltas = newDeltas_chpl;
#line 289 "../lib/Chai.chpl"
    tmp_chpl3 = &call_tmp_chpl12;
#line 289 "../lib/Chai.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl, INT64(289), INT32(65));
#line 289 "../lib/Chai.chpl"
    (_args_forcoforall_fn_chpl)->_13_call_tmp = tmp_chpl3;
#line 289 "../lib/Chai.chpl"
    _args_vforcoforall_fn_chpl = ((void*)(_args_forcoforall_fn_chpl));
#line 289 "../lib/Chai.chpl"
    /*** wrapcoforall_fn_chpl101 ***/ chpl_taskAddCoStmt(chpl_task_getRequestedSubloc(), INT64(100), ((chpl_task_bundle_p)(_args_vforcoforall_fn_chpl)), sizeof(chpl__class_localscoforall_fn_chpl101_object), INT64(289), INT32(65));
#line 289 "../lib/Chai.chpl"
  }
#line 289 "../lib/Chai.chpl"
  error_chpl3 = NULL;
#line 289 "../lib/Chai.chpl"
  _waitEndCount_chpl2(_coforallCount_chpl, numTasks_chpl, &error_chpl3, INT64(289), INT32(65));
#line 289 "../lib/Chai.chpl"
  if (error_chpl3 != nil) /* ZLINE: 289 ../lib/Chai.chpl */
#line 289 "../lib/Chai.chpl"
  {
#line 289 "../lib/Chai.chpl"
    chpl__delete72(_coforallCount_chpl, INT64(289), INT32(65));
#line 289 "../lib/Chai.chpl"
    error_chpl2 = error_chpl3;
#line 289 "../lib/Chai.chpl"
    goto forall_IBB_break_label_chpl;
#line 289 "../lib/Chai.chpl"
  }
#line 289 "../lib/Chai.chpl"
  chpl__delete72(_coforallCount_chpl, INT64(289), INT32(65));
#line 289 "../lib/Chai.chpl"
  chpl_after_forall_fence();
#line 289 "../lib/Chai.chpl"
  forall_IBB_break_label_chpl:;
#line 289 "../lib/Chai.chpl"
  if (error_chpl2 != nil) /* ZLINE: 289 ../lib/Chai.chpl */
#line 289 "../lib/Chai.chpl"
  {
#line 289 "../lib/Chai.chpl"
    error_chpl = error_chpl2;
#line 289 "../lib/Chai.chpl"
    goto forall_IBB_break_label_chpl2;
#line 289 "../lib/Chai.chpl"
  }
#line 289 "../lib/Chai.chpl"
  chpl_after_forall_fence();
#line 289 "../lib/Chai.chpl"
  forall_IBB_break_label_chpl2:;
#line 289 "../lib/Chai.chpl"
  if (error_chpl != nil) /* ZLINE: 289 ../lib/Chai.chpl */
#line 289 "../lib/Chai.chpl"
  {
#line 289 "../lib/Chai.chpl"
    chpl_propagate_error(error_chpl, INT64(289), INT32(65));
#line 289 "../lib/Chai.chpl"
  }
#line 289 "../lib/Chai.chpl"
  chpl_after_forall_fence();
#line 289 "../lib/Chai.chpl"
  coerce_tmp_chpl10 = globalRP_filtersGrad_chpl;
#line 289 "../lib/Chai.chpl"
  i_x_chpl4 = &globalAS_filtersGrad_chpl;
#line 289 "../lib/Chai.chpl"
  chpl_check_nil(coerce_tmp_chpl10, INT64(289), INT32(65));
#line 289 "../lib/Chai.chpl"
  call_tmp_chpl31 = &((coerce_tmp_chpl10)->value);
#line 289 "../lib/Chai.chpl"
  chpl___PLUS__ASSIGN_3(call_tmp_chpl31, i_x_chpl4);
#line 289 "../lib/Chai.chpl"
  _field_destructor_tmp__chpl = (&globalAS_filtersGrad_chpl)->data;
#line 289 "../lib/Chai.chpl"
  chpl__autoDestroy25(&_field_destructor_tmp__chpl, INT64(289), INT32(65));
#line 289 "../lib/Chai.chpl"
  _field_destructor_tmp__chpl2 = (&globalAS_filtersGrad_chpl)->_domain;
#line 289 "../lib/Chai.chpl"
  chpl__autoDestroy9(&_field_destructor_tmp__chpl2, INT64(289), INT32(65));
#line 289 "../lib/Chai.chpl"
  coerce_tmp_chpl11 = globalRP_filtersGrad_chpl;
#line 289 "../lib/Chai.chpl"
  chpl_check_nil(coerce_tmp_chpl11, INT64(289), INT32(65));
#line 289 "../lib/Chai.chpl"
  call_tmp_chpl32 = &((coerce_tmp_chpl11)->value);
#line 289 "../lib/Chai.chpl"
  init_ASSIGN__chpl29(&tmp_chpl4, call_tmp_chpl32);
#line 289 "../lib/Chai.chpl"
  ret_chpl5 = tmp_chpl4;
#line 289 "../lib/Chai.chpl"
  chpl_gentemp = ret_chpl5;
#line 289 "../lib/Chai.chpl"
  chpl___ASSIGN_26(&filtersGrad_chpl, &chpl_gentemp);
#line 289 "../lib/Chai.chpl"
  chpl__delete14(globalRP_filtersGrad_chpl, INT64(289), INT32(65));
#line 289 "../lib/Chai.chpl"
  _field_destructor_tmp__chpl3 = (&chpl_gentemp)->data;
#line 289 "../lib/Chai.chpl"
  chpl__autoDestroy25(&_field_destructor_tmp__chpl3, INT64(289), INT32(65));
#line 289 "../lib/Chai.chpl"
  _field_destructor_tmp__chpl4 = (&chpl_gentemp)->_domain;
#line 289 "../lib/Chai.chpl"
  chpl__autoDestroy9(&_field_destructor_tmp__chpl4, INT64(289), INT32(65));
#line 313 "../lib/Chai.chpl"
  call_tmp_chpl33 = &((this_chpl31)->filtersGrad);
#line 313 "../lib/Chai.chpl"
  call_tmp_chpl34 = &((call_tmp_chpl33)->data);
#line 313 "../lib/Chai.chpl"
  call_tmp_chpl35 = &((&filtersGrad_chpl)->data);
#line 313 "../lib/Chai.chpl"
  if (((RootClass_chpl)((call_tmp_chpl35)->_instance)) == nil) /* ZLINE: 313 ../lib/Chai.chpl */
#line 313 "../lib/Chai.chpl"
  {
#line 313 "../lib/Chai.chpl"
    goto _end__ASSIGN__chpl;
#line 313 "../lib/Chai.chpl"
  }
#line 313 "../lib/Chai.chpl"
  if (((RootClass_chpl)((call_tmp_chpl34)->_instance)) == ((RootClass_chpl)((call_tmp_chpl35)->_instance))) /* ZLINE: 313 ../lib/Chai.chpl */
#line 313 "../lib/Chai.chpl"
  {
#line 313 "../lib/Chai.chpl"
    goto _end__ASSIGN__chpl;
#line 313 "../lib/Chai.chpl"
  }
#line 313 "../lib/Chai.chpl"
  call_tmp_chpl36 = sizeAs_chpl19(call_tmp_chpl34, INT64(313), INT32(65));
#line 313 "../lib/Chai.chpl"
  if (! (INT64(0) < INT64(0))) /* ZLINE: 313 ../lib/Chai.chpl */
#line 313 "../lib/Chai.chpl"
  {
#line 313 "../lib/Chai.chpl"
    tmp_chpl5 = (call_tmp_chpl36 == ((uint64_t)(INT64(0))));
#line 313 "../lib/Chai.chpl"
  } else /* ZLINE: 313 ../lib/Chai.chpl */
#line 313 "../lib/Chai.chpl"
  {
#line 313 "../lib/Chai.chpl"
    tmp_chpl5 = UINT8(false);
#line 313 "../lib/Chai.chpl"
  }
#line 313 "../lib/Chai.chpl"
  if (tmp_chpl5) /* ZLINE: 313 ../lib/Chai.chpl */
#line 313 "../lib/Chai.chpl"
  {
#line 313 "../lib/Chai.chpl"
    call_tmp_chpl37 = sizeAs_chpl19(call_tmp_chpl35, INT64(313), INT32(65));
#line 313 "../lib/Chai.chpl"
    if (! (INT64(0) < INT64(0))) /* ZLINE: 313 ../lib/Chai.chpl */
#line 313 "../lib/Chai.chpl"
    {
#line 313 "../lib/Chai.chpl"
      tmp_chpl7 = (call_tmp_chpl37 == ((uint64_t)(INT64(0))));
#line 313 "../lib/Chai.chpl"
    } else /* ZLINE: 313 ../lib/Chai.chpl */
#line 313 "../lib/Chai.chpl"
    {
#line 313 "../lib/Chai.chpl"
      tmp_chpl7 = UINT8(false);
#line 313 "../lib/Chai.chpl"
    }
#line 313 "../lib/Chai.chpl"
    tmp_chpl6 = tmp_chpl7;
#line 313 "../lib/Chai.chpl"
  } else /* ZLINE: 313 ../lib/Chai.chpl */
#line 313 "../lib/Chai.chpl"
  {
#line 313 "../lib/Chai.chpl"
    tmp_chpl6 = UINT8(false);
#line 313 "../lib/Chai.chpl"
  }
#line 313 "../lib/Chai.chpl"
  if (tmp_chpl6) /* ZLINE: 313 ../lib/Chai.chpl */
#line 313 "../lib/Chai.chpl"
  {
#line 313 "../lib/Chai.chpl"
    goto _end__ASSIGN__chpl;
#line 313 "../lib/Chai.chpl"
  }
#line 313 "../lib/Chai.chpl"
  checkArrayShapesUponAssignment_chpl3(call_tmp_chpl34, call_tmp_chpl35, UINT8(false), INT64(313), INT32(65));
#line 313 "../lib/Chai.chpl"
  _dom_chpl3(call_tmp_chpl34, &ret_tmp_chpl15, INT64(313), INT32(65));
#line 313 "../lib/Chai.chpl"
  call_tmp_chpl38 = ret_tmp_chpl15;
#line 313 "../lib/Chai.chpl"
  _dom_chpl3(call_tmp_chpl35, &ret_tmp_chpl16, INT64(313), INT32(65));
#line 313 "../lib/Chai.chpl"
  call_tmp_chpl39 = ret_tmp_chpl16;
#line 313 "../lib/Chai.chpl"
  i_AD_chpl = &call_tmp_chpl38;
#line 313 "../lib/Chai.chpl"
  i_BD_chpl = &call_tmp_chpl39;
#line 313 "../lib/Chai.chpl"
  ret_chpl6 = (call_tmp_chpl34)->_instance;
#line 313 "../lib/Chai.chpl"
  ret_chpl7 = (call_tmp_chpl35)->_instance;
#line 313 "../lib/Chai.chpl"
  chpl_check_nil(ret_chpl6, INT64(313), INT32(65));
#line 313 "../lib/Chai.chpl"
  call_tmp_chpl40 = doiBulkTransferFromKnown_chpl6(ret_chpl6, i_AD_chpl, ret_chpl7, i_BD_chpl, INT64(313), INT32(65));
#line 313 "../lib/Chai.chpl"
  if (call_tmp_chpl40) /* ZLINE: 313 ../lib/Chai.chpl */
#line 313 "../lib/Chai.chpl"
  {
#line 313 "../lib/Chai.chpl"
  } else /* ZLINE: 313 ../lib/Chai.chpl */
#line 313 "../lib/Chai.chpl"
  {
#line 313 "../lib/Chai.chpl"
  }
#line 313 "../lib/Chai.chpl"
  chpl__autoDestroy9(&call_tmp_chpl39, INT64(313), INT32(65));
#line 313 "../lib/Chai.chpl"
  chpl__autoDestroy9(&call_tmp_chpl38, INT64(313), INT32(65));
#line 313 "../lib/Chai.chpl"
  if (call_tmp_chpl40) /* ZLINE: 313 ../lib/Chai.chpl */
#line 313 "../lib/Chai.chpl"
  {
#line 313 "../lib/Chai.chpl"
  }
#line 313 "../lib/Chai.chpl"
  if (! call_tmp_chpl40) /* ZLINE: 313 ../lib/Chai.chpl */
#line 313 "../lib/Chai.chpl"
  {
#line 313 "../lib/Chai.chpl"
    coerce_tmp_chpl12 = (call_tmp_chpl34)->_instance;
#line 313 "../lib/Chai.chpl"
    tasksPerLocale_chpl2 = local_dataParTasksPerLocale_chpl;
#line 313 "../lib/Chai.chpl"
    ignoreRunning_chpl2 = local_dataParIgnoreRunningTasks_chpl;
#line 313 "../lib/Chai.chpl"
    minIndicesPerTask_chpl2 = local_dataParMinGranularity_chpl;
#line 313 "../lib/Chai.chpl"
    error_chpl4 = NULL;
#line 313 "../lib/Chai.chpl"
    chpl_check_nil(coerce_tmp_chpl12, INT64(313), INT32(65));
#line 313 "../lib/Chai.chpl"
    coerce_tmp_chpl13 = (coerce_tmp_chpl12)->dom;
#line 313 "../lib/Chai.chpl"
    createTuple_chpl3(INT64(0), &ret_tmp_chpl17);
#line 313 "../lib/Chai.chpl"
    offset_x0_chpl2 = *(ret_tmp_chpl17 + INT64(0));
#line 313 "../lib/Chai.chpl"
    offset_x1_chpl = *(ret_tmp_chpl17 + INT64(1));
#line 313 "../lib/Chai.chpl"
    offset_x2_chpl = *(ret_tmp_chpl17 + INT64(2));
#line 313 "../lib/Chai.chpl"
    offset_x3_chpl = *(ret_tmp_chpl17 + INT64(3));
#line 313 "../lib/Chai.chpl"
    error_chpl5 = NULL;
#line 313 "../lib/Chai.chpl"
    call_tmp_chpl43 = get_chpl_nodeID();
#line 313 "../lib/Chai.chpl"
    call_tmp_chpl44 = chpl_rt_buildLocaleID(call_tmp_chpl43, c_sublocid_any);
#line 313 "../lib/Chai.chpl"
    call_tmp_chpl42 = call_tmp_chpl44;
#line 313 "../lib/Chai.chpl"
    chpl_localeID_to_locale(&call_tmp_chpl42, &ret_tmp_chpl18, INT64(313), INT32(65));
#line 313 "../lib/Chai.chpl"
    call_tmp_chpl45 = ret_tmp_chpl18;
#line 313 "../lib/Chai.chpl"
    call_tmp_chpl41 = call_tmp_chpl45;
#line 313 "../lib/Chai.chpl"
    coerce_tmp_chpl14 = (&call_tmp_chpl41)->_instance;
#line 313 "../lib/Chai.chpl"
    if (((RootClass_chpl)(coerce_tmp_chpl14)) == nil) /* ZLINE: 313 ../lib/Chai.chpl */
#line 313 "../lib/Chai.chpl"
    {
#line 313 "../lib/Chai.chpl"
      nilCheckHalt_chpl(&_str_literal_52_chpl, INT64(313), INT32(65));
#line 313 "../lib/Chai.chpl"
    }
#line 313 "../lib/Chai.chpl"
    chpl_check_nil(coerce_tmp_chpl14, INT64(313), INT32(65));
#line 313 "../lib/Chai.chpl"
    ((int64_t(*)(BaseLocale_chpl,
#line 313 "../lib/Chai.chpl"
       int64_t,
#line 313 "../lib/Chai.chpl"
       int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(coerce_tmp_chpl14))->chpl__cid) + INT64(2))])(coerce_tmp_chpl14, INT64(313), INT32(65));
#line 313 "../lib/Chai.chpl"
    i_x_chpl5 = &call_tmp_chpl41;
#line 313 "../lib/Chai.chpl"
    deinit_chpl62(i_x_chpl5);
#line 313 "../lib/Chai.chpl"
    if (tasksPerLocale_chpl2 == INT64(0)) /* ZLINE: 313 ../lib/Chai.chpl */
#line 313 "../lib/Chai.chpl"
    {
#line 313 "../lib/Chai.chpl"
      call_tmp_chpl48 = get_chpl_nodeID();
#line 313 "../lib/Chai.chpl"
      call_tmp_chpl49 = chpl_rt_buildLocaleID(call_tmp_chpl48, c_sublocid_any);
#line 313 "../lib/Chai.chpl"
      call_tmp_chpl47 = call_tmp_chpl49;
#line 313 "../lib/Chai.chpl"
      chpl_localeID_to_locale(&call_tmp_chpl47, &ret_tmp_chpl19, INT64(313), INT32(65));
#line 313 "../lib/Chai.chpl"
      call_tmp_chpl50 = ret_tmp_chpl19;
#line 313 "../lib/Chai.chpl"
      call_tmp_chpl46 = call_tmp_chpl50;
#line 313 "../lib/Chai.chpl"
      coerce_tmp_chpl15 = (&call_tmp_chpl46)->_instance;
#line 313 "../lib/Chai.chpl"
      if (((RootClass_chpl)(coerce_tmp_chpl15)) == nil) /* ZLINE: 313 ../lib/Chai.chpl */
#line 313 "../lib/Chai.chpl"
      {
#line 313 "../lib/Chai.chpl"
        nilCheckHalt_chpl(&_str_literal_52_chpl, INT64(313), INT32(65));
#line 313 "../lib/Chai.chpl"
      }
#line 313 "../lib/Chai.chpl"
      coerce_tmp_chpl16 = coerce_tmp_chpl15;
#line 313 "../lib/Chai.chpl"
      chpl_check_nil(coerce_tmp_chpl16, INT64(313), INT32(65));
#line 313 "../lib/Chai.chpl"
      ret_chpl8 = (coerce_tmp_chpl16)->maxTaskPar;
#line 313 "../lib/Chai.chpl"
      tmp_chpl8 = ret_chpl8;
#line 313 "../lib/Chai.chpl"
      i_x_chpl6 = &call_tmp_chpl46;
#line 313 "../lib/Chai.chpl"
      deinit_chpl62(i_x_chpl6);
#line 313 "../lib/Chai.chpl"
    } else /* ZLINE: 313 ../lib/Chai.chpl */
#line 313 "../lib/Chai.chpl"
    {
#line 313 "../lib/Chai.chpl"
      tmp_chpl8 = tasksPerLocale_chpl2;
#line 313 "../lib/Chai.chpl"
    }
#line 313 "../lib/Chai.chpl"
    if (chpl_task_getSerial()) /* ZLINE: 313 ../lib/Chai.chpl */
#line 313 "../lib/Chai.chpl"
    {
#line 313 "../lib/Chai.chpl"
      tmp_x0_chpl2 = INT64(1);
#line 313 "../lib/Chai.chpl"
      tmp_x1_chpl2 = INT64(0);
#line 313 "../lib/Chai.chpl"
    } else /* ZLINE: 313 ../lib/Chai.chpl */
#line 313 "../lib/Chai.chpl"
    {
#line 313 "../lib/Chai.chpl"
      chpl_check_nil(coerce_tmp_chpl13, INT64(313), INT32(65));
#line 313 "../lib/Chai.chpl"
      call_tmp_chpl51 = &((coerce_tmp_chpl13)->ranges);
#line 313 "../lib/Chai.chpl"
      _computeChunkStuff_chpl2(tmp_chpl8, ignoreRunning_chpl2, minIndicesPerTask_chpl2, call_tmp_chpl51, &ret_tmp_chpl20, INT64(313), INT32(65));
#line 313 "../lib/Chai.chpl"
      tmp_x0_chpl2 = *(ret_tmp_chpl20 + INT64(0));
#line 313 "../lib/Chai.chpl"
      tmp_x1_chpl2 = *(ret_tmp_chpl20 + INT64(1));
#line 313 "../lib/Chai.chpl"
    }
#line 313 "../lib/Chai.chpl"
    numChunks_chpl2 = tmp_x0_chpl2;
#line 313 "../lib/Chai.chpl"
    parDim_chpl2 = tmp_x1_chpl2;
#line 313 "../lib/Chai.chpl"
    init_chpl113(&elt_x0_chpl2);
#line 313 "../lib/Chai.chpl"
    tup_x0_chpl2 = elt_x0_chpl2;
#line 313 "../lib/Chai.chpl"
    init_chpl113(&elt_x1_chpl);
#line 313 "../lib/Chai.chpl"
    tup_x1_chpl = elt_x1_chpl;
#line 313 "../lib/Chai.chpl"
    init_chpl113(&elt_x2_chpl);
#line 313 "../lib/Chai.chpl"
    tup_x2_chpl = elt_x2_chpl;
#line 313 "../lib/Chai.chpl"
    init_chpl113(&elt_x3_chpl);
#line 313 "../lib/Chai.chpl"
    tup_x3_chpl = elt_x3_chpl;
#line 313 "../lib/Chai.chpl"
    *(locBlock_chpl + INT64(0)) = tup_x0_chpl2;
#line 313 "../lib/Chai.chpl"
    *(locBlock_chpl + INT64(1)) = tup_x1_chpl;
#line 313 "../lib/Chai.chpl"
    *(locBlock_chpl + INT64(2)) = tup_x2_chpl;
#line 313 "../lib/Chai.chpl"
    *(locBlock_chpl + INT64(3)) = tup_x3_chpl;
#line 313 "../lib/Chai.chpl"
    chpl_build_low_bounded_range2(offset_x0_chpl2, &ret_tmp_chpl21);
#line 313 "../lib/Chai.chpl"
    call_tmp_chpl52 = ret_tmp_chpl21;
#line 313 "../lib/Chai.chpl"
    chpl_check_nil(coerce_tmp_chpl13, INT64(313), INT32(65));
#line 313 "../lib/Chai.chpl"
    call_tmp_chpl53 = &((coerce_tmp_chpl13)->ranges);
#line 313 "../lib/Chai.chpl"
    coerce_tmp_chpl17 = *(*(call_tmp_chpl53) + INT64(0));
#line 313 "../lib/Chai.chpl"
    call_tmp_chpl54 = sizeAs_chpl37(coerce_tmp_chpl17, INT64(313), INT32(65));
#line 313 "../lib/Chai.chpl"
    chpl___POUND_3(call_tmp_chpl52, call_tmp_chpl54, &ret_tmp_chpl22, INT64(313), INT32(65));
#line 313 "../lib/Chai.chpl"
    call_tmp_chpl55 = ret_tmp_chpl22;
#line 313 "../lib/Chai.chpl"
    *(locBlock_chpl + INT64(0)) = call_tmp_chpl55;
#line 313 "../lib/Chai.chpl"
    chpl_build_low_bounded_range2(offset_x1_chpl, &ret_tmp_chpl23);
#line 313 "../lib/Chai.chpl"
    call_tmp_chpl56 = ret_tmp_chpl23;
#line 313 "../lib/Chai.chpl"
    chpl_check_nil(coerce_tmp_chpl13, INT64(313), INT32(65));
#line 313 "../lib/Chai.chpl"
    call_tmp_chpl57 = &((coerce_tmp_chpl13)->ranges);
#line 313 "../lib/Chai.chpl"
    coerce_tmp_chpl18 = *(*(call_tmp_chpl57) + INT64(1));
#line 313 "../lib/Chai.chpl"
    call_tmp_chpl58 = sizeAs_chpl37(coerce_tmp_chpl18, INT64(313), INT32(65));
#line 313 "../lib/Chai.chpl"
    chpl___POUND_3(call_tmp_chpl56, call_tmp_chpl58, &ret_tmp_chpl24, INT64(313), INT32(65));
#line 313 "../lib/Chai.chpl"
    call_tmp_chpl59 = ret_tmp_chpl24;
#line 313 "../lib/Chai.chpl"
    *(locBlock_chpl + INT64(1)) = call_tmp_chpl59;
#line 313 "../lib/Chai.chpl"
    chpl_build_low_bounded_range2(offset_x2_chpl, &ret_tmp_chpl25);
#line 313 "../lib/Chai.chpl"
    call_tmp_chpl60 = ret_tmp_chpl25;
#line 313 "../lib/Chai.chpl"
    chpl_check_nil(coerce_tmp_chpl13, INT64(313), INT32(65));
#line 313 "../lib/Chai.chpl"
    call_tmp_chpl61 = &((coerce_tmp_chpl13)->ranges);
#line 313 "../lib/Chai.chpl"
    coerce_tmp_chpl19 = *(*(call_tmp_chpl61) + INT64(2));
#line 313 "../lib/Chai.chpl"
    call_tmp_chpl62 = sizeAs_chpl37(coerce_tmp_chpl19, INT64(313), INT32(65));
#line 313 "../lib/Chai.chpl"
    chpl___POUND_3(call_tmp_chpl60, call_tmp_chpl62, &ret_tmp_chpl26, INT64(313), INT32(65));
#line 313 "../lib/Chai.chpl"
    call_tmp_chpl63 = ret_tmp_chpl26;
#line 313 "../lib/Chai.chpl"
    *(locBlock_chpl + INT64(2)) = call_tmp_chpl63;
#line 313 "../lib/Chai.chpl"
    chpl_build_low_bounded_range2(offset_x3_chpl, &ret_tmp_chpl27);
#line 313 "../lib/Chai.chpl"
    call_tmp_chpl64 = ret_tmp_chpl27;
#line 313 "../lib/Chai.chpl"
    chpl_check_nil(coerce_tmp_chpl13, INT64(313), INT32(65));
#line 313 "../lib/Chai.chpl"
    call_tmp_chpl65 = &((coerce_tmp_chpl13)->ranges);
#line 313 "../lib/Chai.chpl"
    coerce_tmp_chpl20 = *(*(call_tmp_chpl65) + INT64(3));
#line 313 "../lib/Chai.chpl"
    call_tmp_chpl66 = sizeAs_chpl37(coerce_tmp_chpl20, INT64(313), INT32(65));
#line 313 "../lib/Chai.chpl"
    chpl___POUND_3(call_tmp_chpl64, call_tmp_chpl66, &ret_tmp_chpl28, INT64(313), INT32(65));
#line 313 "../lib/Chai.chpl"
    call_tmp_chpl67 = ret_tmp_chpl28;
#line 313 "../lib/Chai.chpl"
    *(locBlock_chpl + INT64(3)) = call_tmp_chpl67;
#line 313 "../lib/Chai.chpl"
    chpl_build_low_bounded_range2(INT64(0), &ret_tmp_chpl29);
#line 313 "../lib/Chai.chpl"
    call_tmp_chpl68 = ret_tmp_chpl29;
#line 313 "../lib/Chai.chpl"
    chpl___POUND_3(call_tmp_chpl68, tmp_x0_chpl2, &ret_tmp_chpl30, INT64(313), INT32(65));
#line 313 "../lib/Chai.chpl"
    tmpIter_chpl2 = ret_tmp_chpl30;
#line 313 "../lib/Chai.chpl"
    new_temp_chpl3 = _new_chpl17(INT64(313), INT32(65));
#line 313 "../lib/Chai.chpl"
    _coforallCount_chpl2 = new_temp_chpl3;
#line 313 "../lib/Chai.chpl"
    numTasks_chpl2 = chpl_boundedCoforallSize(tmpIter_chpl2, INT64(313), INT32(65));
#line 313 "../lib/Chai.chpl"
    _upEndCount_chpl(_coforallCount_chpl2, numTasks_chpl2, INT64(313), INT32(65));
#line 313 "../lib/Chai.chpl"
    chpl_resetTaskSpawn(numTasks_chpl2, INT64(313), INT32(65));
#line 313 "../lib/Chai.chpl"
    _ic__F0_this_chpl2 = tmpIter_chpl2;
#line 313 "../lib/Chai.chpl"
    checkIfIterWillOverflow_chpl(tmpIter_chpl2, UINT8(true), INT64(313), INT32(65));
#line 313 "../lib/Chai.chpl"
    i_chpl2 = INT64(0);
#line 313 "../lib/Chai.chpl"
    this_chpl34 = _ic__F0_this_chpl2;
#line 313 "../lib/Chai.chpl"
    this_chpl35 = _ic__F0_this_chpl2;
#line 313 "../lib/Chai.chpl"
    end_chpl2 = (&this_chpl35)->_high;
#line 313 "../lib/Chai.chpl"
    for (i_chpl2 = (&this_chpl34)->_low; ((i_chpl2 <= end_chpl2)); i_chpl2 += INT64(1)) {
#line 313 "../lib/Chai.chpl"
      _args_forcoforall_fn_chpl2 = ((_class_localscoforall_fn_chpl33)(&chpl_macro_tmp_106));
#line 313 "../lib/Chai.chpl"
      chpl_check_nil(_args_forcoforall_fn_chpl2, INT64(313), INT32(65));
#line 313 "../lib/Chai.chpl"
      (_args_forcoforall_fn_chpl2)->_0_numChunks = numChunks_chpl2;
#line 313 "../lib/Chai.chpl"
      chpl_check_nil(_args_forcoforall_fn_chpl2, INT64(313), INT32(65));
#line 313 "../lib/Chai.chpl"
      (_args_forcoforall_fn_chpl2)->_1_parDim = parDim_chpl2;
#line 313 "../lib/Chai.chpl"
      chpl_check_nil(_args_forcoforall_fn_chpl2, INT64(313), INT32(65));
#line 313 "../lib/Chai.chpl"
      *((_args_forcoforall_fn_chpl2)->_2_coerce_tmp + INT64(0)) = *(locBlock_chpl + INT64(0));
#line 313 "../lib/Chai.chpl"
      *((_args_forcoforall_fn_chpl2)->_2_coerce_tmp + INT64(1)) = *(locBlock_chpl + INT64(1));
#line 313 "../lib/Chai.chpl"
      *((_args_forcoforall_fn_chpl2)->_2_coerce_tmp + INT64(2)) = *(locBlock_chpl + INT64(2));
#line 313 "../lib/Chai.chpl"
      *((_args_forcoforall_fn_chpl2)->_2_coerce_tmp + INT64(3)) = *(locBlock_chpl + INT64(3));
#line 313 "../lib/Chai.chpl"
      chpl_check_nil(_args_forcoforall_fn_chpl2, INT64(313), INT32(65));
#line 313 "../lib/Chai.chpl"
      (_args_forcoforall_fn_chpl2)->_3__coforallCount = _coforallCount_chpl2;
#line 313 "../lib/Chai.chpl"
      chpl_check_nil(_args_forcoforall_fn_chpl2, INT64(313), INT32(65));
#line 313 "../lib/Chai.chpl"
      (_args_forcoforall_fn_chpl2)->_4_chunk = i_chpl2;
#line 313 "../lib/Chai.chpl"
      chpl_check_nil(_args_forcoforall_fn_chpl2, INT64(313), INT32(65));
#line 313 "../lib/Chai.chpl"
      (_args_forcoforall_fn_chpl2)->_6_a = *(call_tmp_chpl34);
#line 313 "../lib/Chai.chpl"
      chpl_check_nil(_args_forcoforall_fn_chpl2, INT64(313), INT32(65));
#line 313 "../lib/Chai.chpl"
      (_args_forcoforall_fn_chpl2)->_7_b = *(call_tmp_chpl35);
#line 313 "../lib/Chai.chpl"
      _args_vforcoforall_fn_chpl2 = ((void*)(_args_forcoforall_fn_chpl2));
#line 313 "../lib/Chai.chpl"
      _args_sizecoforall_fn_chpl = sizeof(chpl__class_localscoforall_fn_chpl33_object);
#line 313 "../lib/Chai.chpl"
      (_args_forcoforall_fn_chpl2)->_ln = INT64(313);
#line 313 "../lib/Chai.chpl"
      (_args_forcoforall_fn_chpl2)->_fn = INT32(65);
#line 313 "../lib/Chai.chpl"
      /*** wrapcoforall_fn_chpl31 ***/ chpl_taskAddCoStmt(chpl_task_getRequestedSubloc(), INT64(30), ((chpl_task_bundle_p)(_args_vforcoforall_fn_chpl2)), _args_sizecoforall_fn_chpl, INT64(2763), INT32(30));
#line 313 "../lib/Chai.chpl"
    }
#line 313 "../lib/Chai.chpl"
    error_chpl6 = NULL;
#line 313 "../lib/Chai.chpl"
    _waitEndCount_chpl2(_coforallCount_chpl2, numTasks_chpl2, &error_chpl6, INT64(313), INT32(65));
#line 313 "../lib/Chai.chpl"
    if (error_chpl6 != nil) /* ZLINE: 313 ../lib/Chai.chpl */
#line 313 "../lib/Chai.chpl"
    {
#line 313 "../lib/Chai.chpl"
      chpl__delete72(_coforallCount_chpl2, INT64(313), INT32(65));
#line 313 "../lib/Chai.chpl"
      error_chpl5 = error_chpl6;
#line 313 "../lib/Chai.chpl"
      goto forall_IBB_break_label_chpl3;
#line 313 "../lib/Chai.chpl"
    }
#line 313 "../lib/Chai.chpl"
    chpl__delete72(_coforallCount_chpl2, INT64(313), INT32(65));
#line 313 "../lib/Chai.chpl"
    chpl_after_forall_fence();
#line 313 "../lib/Chai.chpl"
    forall_IBB_break_label_chpl3:;
#line 313 "../lib/Chai.chpl"
    if (error_chpl5 != nil) /* ZLINE: 313 ../lib/Chai.chpl */
#line 313 "../lib/Chai.chpl"
    {
#line 313 "../lib/Chai.chpl"
      error_chpl4 = error_chpl5;
#line 313 "../lib/Chai.chpl"
      goto forall_IBB_break_label_chpl4;
#line 313 "../lib/Chai.chpl"
    }
#line 313 "../lib/Chai.chpl"
    chpl_after_forall_fence();
#line 313 "../lib/Chai.chpl"
    forall_IBB_break_label_chpl4:;
#line 313 "../lib/Chai.chpl"
    if (error_chpl4 != nil) /* ZLINE: 313 ../lib/Chai.chpl */
#line 313 "../lib/Chai.chpl"
    {
#line 313 "../lib/Chai.chpl"
      chpl_propagate_error(error_chpl4, INT64(313), INT32(65));
#line 313 "../lib/Chai.chpl"
    }
#line 313 "../lib/Chai.chpl"
    chpl_after_forall_fence();
#line 313 "../lib/Chai.chpl"
  }
#line 313 "../lib/Chai.chpl"
  _end__ASSIGN__chpl:;
#line 314 "../lib/Chai.chpl"
  chpl__checkRetEltTypeMatch2(&newDeltas_chpl);
#line 314 "../lib/Chai.chpl"
  _field_destructor_tmp__chpl5 = (&filtersGrad_chpl)->data;
#line 314 "../lib/Chai.chpl"
  chpl__autoDestroy25(&_field_destructor_tmp__chpl5, INT64(314), INT32(65));
#line 314 "../lib/Chai.chpl"
  _field_destructor_tmp__chpl6 = (&filtersGrad_chpl)->_domain;
#line 314 "../lib/Chai.chpl"
  chpl__autoDestroy9(&_field_destructor_tmp__chpl6, INT64(314), INT32(65));
#line 314 "../lib/Chai.chpl"
  chpl__autoDestroy11(&call_tmp_chpl9, INT64(314), INT32(65));
#line 285 "../lib/Chai.chpl"
  *(_retArg_chpl) = newDeltas_chpl;
#line 285 "../lib/Chai.chpl"
  return;
#line 285 "../lib/Chai.chpl"
}

#line 289 "../lib/Chai.chpl"
/* Chai.chpl:289 */
#line 289 "../lib/Chai.chpl"
static void wrapcoforall_fn_chpl101(_class_localscoforall_fn_chpl101 c_chpl) {
#line 289 "../lib/Chai.chpl"
  Error_chpl error_chpl = NULL;
#line 289 "../lib/Chai.chpl"
  int64_t _0_numChunks_chpl;
#line 289 "../lib/Chai.chpl"
  int64_t _1_parDim_chpl;
#line 289 "../lib/Chai.chpl"
  _tuple_1_star_range_int64_t_both_one_chpl _2_coerce_tmp_chpl;
#line 289 "../lib/Chai.chpl"
  chpl___EndCount_AtomicT_int64_t_int64_t _3__coforallCount_chpl = NULL;
#line 289 "../lib/Chai.chpl"
  int64_t _4_chunk_chpl;
#line 289 "../lib/Chai.chpl"
  _ref_Tensor_4__real64_chpl _6_filters_ref_chpl = NULL;
#line 289 "../lib/Chai.chpl"
  int64_t _7_stride_svar_chpl;
#line 289 "../lib/Chai.chpl"
  int64_t _8_padding_svar_chpl;
#line 289 "../lib/Chai.chpl"
  SumReduceScanOp_Tensor_4__real64_chpl _9_globalRP_filtersGrad_chpl = NULL;
#line 289 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl _10_deltas_chpl;
#line 289 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl _11_inputs_chpl;
#line 289 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl _12_newDeltas_chpl;
#line 289 "../lib/Chai.chpl"
  _ref_range_int64_t_low_one_chpl _13_call_tmp_chpl = NULL;
#line 289 "../lib/Chai.chpl"
  chpl_rmem_consist_acquire(INT64(289), INT32(65));
#line 289 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(289), INT32(65));
#line 289 "../lib/Chai.chpl"
  _0_numChunks_chpl = (c_chpl)->_0_numChunks;
#line 289 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(289), INT32(65));
#line 289 "../lib/Chai.chpl"
  _1_parDim_chpl = (c_chpl)->_1_parDim;
#line 289 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(289), INT32(65));
#line 289 "../lib/Chai.chpl"
  *(_2_coerce_tmp_chpl + INT64(0)) = *((c_chpl)->_2_coerce_tmp + INT64(0));
#line 289 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(289), INT32(65));
#line 289 "../lib/Chai.chpl"
  _3__coforallCount_chpl = (c_chpl)->_3__coforallCount;
#line 289 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(289), INT32(65));
#line 289 "../lib/Chai.chpl"
  _4_chunk_chpl = (c_chpl)->_4_chunk;
#line 289 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(289), INT32(65));
#line 289 "../lib/Chai.chpl"
  _6_filters_ref_chpl = (c_chpl)->_6_filters_ref;
#line 289 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(289), INT32(65));
#line 289 "../lib/Chai.chpl"
  _7_stride_svar_chpl = (c_chpl)->_7_stride_svar;
#line 289 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(289), INT32(65));
#line 289 "../lib/Chai.chpl"
  _8_padding_svar_chpl = (c_chpl)->_8_padding_svar;
#line 289 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(289), INT32(65));
#line 289 "../lib/Chai.chpl"
  _9_globalRP_filtersGrad_chpl = (c_chpl)->_9_globalRP_filtersGrad;
#line 289 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(289), INT32(65));
#line 289 "../lib/Chai.chpl"
  _10_deltas_chpl = (c_chpl)->_10_deltas;
#line 289 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(289), INT32(65));
#line 289 "../lib/Chai.chpl"
  _11_inputs_chpl = (c_chpl)->_11_inputs;
#line 289 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(289), INT32(65));
#line 289 "../lib/Chai.chpl"
  _12_newDeltas_chpl = (c_chpl)->_12_newDeltas;
#line 289 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(289), INT32(65));
#line 289 "../lib/Chai.chpl"
  _13_call_tmp_chpl = (c_chpl)->_13_call_tmp;
#line 289 "../lib/Chai.chpl"
  coforall_fn_chpl101(_0_numChunks_chpl, _1_parDim_chpl, &_2_coerce_tmp_chpl, _3__coforallCount_chpl, _4_chunk_chpl, &error_chpl, _6_filters_ref_chpl, _7_stride_svar_chpl, _8_padding_svar_chpl, _9_globalRP_filtersGrad_chpl, &_10_deltas_chpl, &_11_inputs_chpl, &_12_newDeltas_chpl, _13_call_tmp_chpl);
#line 289 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(289), INT32(65));
#line 289 "../lib/Chai.chpl"
  _downEndCount_chpl((c_chpl)->_3__coforallCount, error_chpl, INT64(289), INT32(65));
#line 289 "../lib/Chai.chpl"
  return;
#line 289 "../lib/Chai.chpl"
}

#line 289 "../lib/Chai.chpl"
/* Chai.chpl:289 */
#line 289 "../lib/Chai.chpl"
static void coforall_fn_chpl101(int64_t numChunks_chpl,
#line 289 "../lib/Chai.chpl"
                                int64_t parDim_chpl,
#line 289 "../lib/Chai.chpl"
                                _tuple_1_star_range_int64_t_both_one_chpl * locBlock_chpl,
#line 289 "../lib/Chai.chpl"
                                chpl___EndCount_AtomicT_int64_t_int64_t _coforallCount_chpl,
#line 289 "../lib/Chai.chpl"
                                int64_t chunk_chpl,
#line 289 "../lib/Chai.chpl"
                                Error_chpl * error_out_chpl,
#line 289 "../lib/Chai.chpl"
                                Tensor_4__real64_chpl * filters_svar_chpl,
#line 289 "../lib/Chai.chpl"
                                int64_t stride_svar_chpl,
#line 289 "../lib/Chai.chpl"
                                int64_t padding_svar_chpl,
#line 289 "../lib/Chai.chpl"
                                SumReduceScanOp_Tensor_4__real64_chpl RP_filtersGrad_chpl,
#line 289 "../lib/Chai.chpl"
                                _array_DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl * deltas_chpl,
#line 289 "../lib/Chai.chpl"
                                _array_DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl * inputs_chpl,
#line 289 "../lib/Chai.chpl"
                                _array_DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl * newDeltas_chpl,
#line 289 "../lib/Chai.chpl"
                                range_int64_t_low_one_chpl * call_tmp_chpl6) {
#line 300 "../lib/Chai.chpl"
  int64_t local_dataParMinGranularity_chpl;
#line 300 "../lib/Chai.chpl"
  chpl_bool local_dataParIgnoreRunningTasks_chpl;
#line 300 "../lib/Chai.chpl"
  int64_t local_dataParTasksPerLocale_chpl;
#line 289 "../lib/Chai.chpl"
  SumReduceScanOp_Tensor_4__real64_chpl RP_filtersGrad_chpl2 = NULL;
#line 289 "../lib/Chai.chpl"
  Tensor_4__real64_chpl AS_filtersGrad_chpl;
#line 289 "../lib/Chai.chpl"
  SumReduceScanOp_Tensor_4__real64_chpl new_temp_chpl = NULL;
#line 289 "../lib/Chai.chpl"
  chpl_LocalSpinlock default_arg_l_chpl;
#line 289 "../lib/Chai.chpl"
  chpl_LocalSpinlock ret_chpl;
#line 289 "../lib/Chai.chpl"
  chpl_LocalSpinlock l_chpl;
#line 289 "../lib/Chai.chpl"
  Tensor_4__real64_chpl default_arg_value_chpl;
#line 289 "../lib/Chai.chpl"
  Tensor_4__real64_chpl ret_chpl2;
#line 289 "../lib/Chai.chpl"
  Tensor_4__real64_chpl value_chpl;
#line 289 "../lib/Chai.chpl"
  Tensor_4__real64_chpl ret_chpl3;
#line 289 "../lib/Chai.chpl"
  Tensor_4__real64_chpl x_chpl;
#line 289 "../lib/Chai.chpl"
  _tuple_1_star_range_int64_t_both_one_chpl followMe_chpl;
#line 289 "../lib/Chai.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl7 = NULL;
#line 289 "../lib/Chai.chpl"
  int64_t call_tmp_chpl8;
#line 289 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl3;
#line 289 "../lib/Chai.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl9 = NULL;
#line 289 "../lib/Chai.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl10 = NULL;
#line 289 "../lib/Chai.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl11 = NULL;
#line 289 "../lib/Chai.chpl"
  _tuple_2_star_int64_t_chpl call_tmp_chpl12;
#line 289 "../lib/Chai.chpl"
  _tuple_2_star_int64_t_chpl ret_tmp_chpl;
#line 289 "../lib/Chai.chpl"
  int64_t tmp_x0_chpl;
#line 289 "../lib/Chai.chpl"
  int64_t tmp_x1_chpl;
#line 289 "../lib/Chai.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl13 = NULL;
#line 289 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl14;
#line 289 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl2;
#line 289 "../lib/Chai.chpl"
  _tuple_1_star_range_int64_t_both_one_chpl _yield_expr_tmp__chpl;
#line 289 "../lib/Chai.chpl"
  _tuple_1_star_range_int64_t_both_one_chpl chpl_followThis;
#line 289 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl _ir_F0_this_chpl;
#line 289 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl _ic__F5_block_x0_chpl;
#line 289 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl _ic__F7_coerce_tmp_x0_chpl;
#line 289 "../lib/Chai.chpl"
  int64_t _ic__F8_i_chpl;
#line 289 "../lib/Chai.chpl"
  int64_t _ic__F9_end_chpl;
#line 289 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl _ir_F0_this_chpl2;
#line 289 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl _ic__F5_block_x0_chpl2;
#line 289 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl _ic__F7_coerce_tmp_x0_chpl2;
#line 289 "../lib/Chai.chpl"
  int64_t _ic__F8_i_chpl2;
#line 289 "../lib/Chai.chpl"
  int64_t _ic__F9_end_chpl2;
#line 289 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl coerce_tmp_chpl4;
#line 289 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl _ic__F0_this_chpl;
#line 289 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl _ic__F2_myFollowThis_chpl;
#line 289 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl _ic__F3_r_chpl;
#line 289 "../lib/Chai.chpl"
  int64_t _ic__F5_i_chpl;
#line 289 "../lib/Chai.chpl"
  DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl this_chpl31 = NULL;
#line 289 "../lib/Chai.chpl"
  DefaultRectangularDom_1_int64_t_one_chpl coerce_tmp_chpl5 = NULL;
#line 289 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl6;
#line 289 "../lib/Chai.chpl"
  DefaultRectangularDom_1_int64_t_one_chpl this_chpl32 = NULL;
#line 289 "../lib/Chai.chpl"
  uint64_t call_tmp_chpl15;
#line 289 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl7;
#line 289 "../lib/Chai.chpl"
  DefaultRectangularDom_1_int64_t_one_chpl this_chpl33 = NULL;
#line 289 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl8;
#line 289 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl9;
#line 289 "../lib/Chai.chpl"
  DefaultRectangularDom_1_int64_t_one_chpl this_chpl34 = NULL;
#line 289 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl10;
#line 289 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl11;
#line 289 "../lib/Chai.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl16 = NULL;
#line 289 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl this_chpl35;
#line 289 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl this_chpl36;
#line 289 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl tmp_chpl;
#line 289 "../lib/Chai.chpl"
  DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl ret_chpl4 = NULL;
#line 289 "../lib/Chai.chpl"
  DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl _ic__F0_this_chpl2 = NULL;
#line 289 "../lib/Chai.chpl"
  _tuple_1_star_int64_t_chpl ret_tmp_chpl3;
#line 289 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl tup_x0_chpl;
#line 289 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl elt_x0_chpl;
#line 289 "../lib/Chai.chpl"
  int64_t coerce_tmp_chpl12;
#line 289 "../lib/Chai.chpl"
  _ref__tuple_1_star_range_int64_t_both_one_chpl call_tmp_chpl17 = NULL;
#line 289 "../lib/Chai.chpl"
  chpl_bool tmp_chpl2;
#line 289 "../lib/Chai.chpl"
  _ref__tuple_1_star_range_int64_t_both_one_chpl call_tmp_chpl18 = NULL;
#line 289 "../lib/Chai.chpl"
  int64_t call_tmp_chpl19;
#line 289 "../lib/Chai.chpl"
  _ref__tuple_1_star_range_int64_t_both_one_chpl call_tmp_chpl20 = NULL;
#line 289 "../lib/Chai.chpl"
  int64_t call_tmp_chpl21;
#line 289 "../lib/Chai.chpl"
  _tuple_1_star_range_int64_t_both_one_chpl retcopy_chpl;
#line 289 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl _ic__F0_this_chpl3;
#line 289 "../lib/Chai.chpl"
  int64_t coerce_tmp_chpl13;
#line 289 "../lib/Chai.chpl"
  DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl this_chpl37 = NULL;
#line 289 "../lib/Chai.chpl"
  DefaultRectangularDom_1_int64_t_one_chpl coerce_tmp_chpl14 = NULL;
#line 289 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl15;
#line 289 "../lib/Chai.chpl"
  DefaultRectangularDom_1_int64_t_one_chpl this_chpl38 = NULL;
#line 289 "../lib/Chai.chpl"
  uint64_t call_tmp_chpl22;
#line 289 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl16;
#line 289 "../lib/Chai.chpl"
  DefaultRectangularDom_1_int64_t_one_chpl this_chpl39 = NULL;
#line 289 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl17;
#line 289 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl18;
#line 289 "../lib/Chai.chpl"
  DefaultRectangularDom_1_int64_t_one_chpl this_chpl40 = NULL;
#line 289 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl19;
#line 289 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl20;
#line 289 "../lib/Chai.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl23 = NULL;
#line 289 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl this_chpl41;
#line 289 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl this_chpl42;
#line 289 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl tmp_chpl3;
#line 289 "../lib/Chai.chpl"
  DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl ret_chpl5 = NULL;
#line 289 "../lib/Chai.chpl"
  DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl _ic__F0_this_chpl4 = NULL;
#line 289 "../lib/Chai.chpl"
  _tuple_1_star_int64_t_chpl ret_tmp_chpl4;
#line 289 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl tup_x0_chpl2;
#line 289 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl elt_x0_chpl2;
#line 289 "../lib/Chai.chpl"
  int64_t coerce_tmp_chpl21;
#line 289 "../lib/Chai.chpl"
  _ref__tuple_1_star_range_int64_t_both_one_chpl call_tmp_chpl24 = NULL;
#line 289 "../lib/Chai.chpl"
  chpl_bool tmp_chpl4;
#line 289 "../lib/Chai.chpl"
  _ref__tuple_1_star_range_int64_t_both_one_chpl call_tmp_chpl25 = NULL;
#line 289 "../lib/Chai.chpl"
  int64_t call_tmp_chpl26;
#line 289 "../lib/Chai.chpl"
  _ref__tuple_1_star_range_int64_t_both_one_chpl call_tmp_chpl27 = NULL;
#line 289 "../lib/Chai.chpl"
  int64_t call_tmp_chpl28;
#line 289 "../lib/Chai.chpl"
  _tuple_1_star_range_int64_t_both_one_chpl retcopy_chpl2;
#line 289 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl _ic__F0_this_chpl5;
#line 289 "../lib/Chai.chpl"
  int64_t coerce_tmp_chpl22;
#line 289 "../lib/Chai.chpl"
  chpl_bool hasMore_chpl;
#line 289 "../lib/Chai.chpl"
  int64_t call_tmp_chpl29;
#line 289 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl this_chpl43;
#line 289 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl tmp_chpl5;
#line 289 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl tmp_chpl6;
#line 289 "../lib/Chai.chpl"
  _ref_range_int64_t_both_one_chpl tmp_chpl7 = NULL;
#line 289 "../lib/Chai.chpl"
  chpl_bool tmp_chpl8;
#line 289 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl tmp_chpl9;
#line 289 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl tmp_chpl10;
#line 289 "../lib/Chai.chpl"
  int64_t call_tmp_chpl30;
#line 289 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl tmp_chpl11;
#line 289 "../lib/Chai.chpl"
  int64_t call_tmp_chpl31;
#line 289 "../lib/Chai.chpl"
  _ref_range_int64_t_both_one_chpl tmp_chpl12 = NULL;
#line 289 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl23;
#line 289 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl _ic__F0_this_chpl6;
#line 289 "../lib/Chai.chpl"
  DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl this_chpl44 = NULL;
#line 289 "../lib/Chai.chpl"
  _ddata_Tensor_3__real64_chpl coerce_tmp_chpl24 = NULL;
#line 289 "../lib/Chai.chpl"
  _ref_Tensor_3__real64_chpl call_tmp_chpl32 = NULL;
#line 289 "../lib/Chai.chpl"
  int64_t tmp_chpl13;
#line 289 "../lib/Chai.chpl"
  DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl this_chpl45 = NULL;
#line 289 "../lib/Chai.chpl"
  _ddata_Tensor_3__real64_chpl coerce_tmp_chpl25 = NULL;
#line 289 "../lib/Chai.chpl"
  _ref_Tensor_3__real64_chpl call_tmp_chpl33 = NULL;
#line 289 "../lib/Chai.chpl"
  int64_t i_chpl;
#line 291 "../lib/Chai.chpl"
  _tuple_3_star_int64_t_chpl call_tmp_chpl34;
#line 291 "../lib/Chai.chpl"
  _tuple_3_star_int64_t_chpl ret_tmp_chpl5;
#line 291 "../lib/Chai.chpl"
  int64_t tmp_x0_chpl2;
#line 291 "../lib/Chai.chpl"
  int64_t tmp_x1_chpl2;
#line 291 "../lib/Chai.chpl"
  int64_t tmp_x2_chpl;
#line 292 "../lib/Chai.chpl"
  Tensor_4__real64_chpl call_tmp_chpl35;
#line 292 "../lib/Chai.chpl"
  _tuple_4_star_int64_t_chpl call_tmp_chpl36;
#line 292 "../lib/Chai.chpl"
  _tuple_4_star_int64_t_chpl ret_tmp_chpl6;
#line 292 "../lib/Chai.chpl"
  int64_t tmp_x0_chpl3;
#line 292 "../lib/Chai.chpl"
  int64_t tmp_x1_chpl3;
#line 292 "../lib/Chai.chpl"
  int64_t tmp_x2_chpl2;
#line 292 "../lib/Chai.chpl"
  int64_t tmp_x3_chpl;
#line 293 "../lib/Chai.chpl"
  _tuple_3_star_int64_t_chpl call_tmp_chpl37;
#line 293 "../lib/Chai.chpl"
  _tuple_3_star_int64_t_chpl ret_tmp_chpl7;
#line 293 "../lib/Chai.chpl"
  int64_t tmp_x0_chpl4;
#line 293 "../lib/Chai.chpl"
  int64_t tmp_x1_chpl4;
#line 296 "../lib/Chai.chpl"
  Tensor_4__real64_chpl call_tmp_chpl38;
#line 296 "../lib/Chai.chpl"
  Tensor_4__real64_chpl ret_tmp_chpl8;
#line 296 "../lib/Chai.chpl"
  Tensor_4__real64_chpl dL_dF_chpl;
#line 298 "../lib/Chai.chpl"
  Tensor_3__real64_chpl call_tmp_chpl39;
#line 298 "../lib/Chai.chpl"
  Tensor_3__real64_chpl initTemp_chpl;
#line 298 "../lib/Chai.chpl"
  Tensor_3__real64_chpl dL_dX_chpl;
#line 299 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl call_tmp_chpl40;
#line 299 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl ret_tmp_chpl9;
#line 299 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl41;
#line 299 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl10;
#line 299 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl call_tmp_chpl42;
#line 299 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl ret_tmp_chpl11;
#line 299 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl43;
#line 299 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl12;
#line 299 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl call_tmp_chpl44;
#line 299 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl ret_tmp_chpl13;
#line 299 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl45;
#line 299 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl14;
#line 299 "../lib/Chai.chpl"
  _domain_DefaultRectangularDom_3_int64_t_one_chpl call_tmp_chpl46;
#line 299 "../lib/Chai.chpl"
  _domain_DefaultRectangularDom_3_int64_t_one_chpl ret_tmp_chpl15;
#line 300 "../lib/Chai.chpl"
  _domain_DefaultRectangularDom_3_int64_t_one_chpl call_tmp_chpl47;
#line 300 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl call_tmp_chpl48;
#line 300 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl ret_tmp_chpl16;
#line 300 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl49;
#line 300 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl17;
#line 300 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl call_tmp_chpl50;
#line 300 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl ret_tmp_chpl18;
#line 300 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl51;
#line 300 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl19;
#line 300 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl call_tmp_chpl52;
#line 300 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl ret_tmp_chpl20;
#line 300 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl53;
#line 300 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl21;
#line 300 "../lib/Chai.chpl"
  _domain_DefaultRectangularDom_3_int64_t_one_chpl ret_tmp_chpl22;
#line 300 "../lib/Chai.chpl"
  int64_t outChannels_chpl;
#line 300 "../lib/Chai.chpl"
  int64_t dh_chpl;
#line 300 "../lib/Chai.chpl"
  int64_t dw_chpl;
#line 300 "../lib/Chai.chpl"
  int64_t kh_chpl;
#line 300 "../lib/Chai.chpl"
  int64_t kw_chpl;
#line 300 "../lib/Chai.chpl"
  int64_t stride_svar_chpl2;
#line 300 "../lib/Chai.chpl"
  int64_t padding_svar_chpl2;
#line 300 "../lib/Chai.chpl"
  DefaultRectangularDom_3_int64_t_one_chpl coerce_tmp_chpl26 = NULL;
#line 300 "../lib/Chai.chpl"
  int64_t tasksPerLocale_chpl;
#line 300 "../lib/Chai.chpl"
  chpl_bool ignoreRunning_chpl;
#line 300 "../lib/Chai.chpl"
  int64_t minIndicesPerTask_chpl;
#line 300 "../lib/Chai.chpl"
  _tuple_3_star_int64_t_chpl ret_tmp_chpl23;
#line 300 "../lib/Chai.chpl"
  Error_chpl error_chpl = NULL;
#line 300 "../lib/Chai.chpl"
  int64_t tmp_chpl14;
#line 300 "../lib/Chai.chpl"
  locale_chpl call_tmp_chpl54;
#line 300 "../lib/Chai.chpl"
  chpl_localeID_t call_tmp_chpl55;
#line 300 "../lib/Chai.chpl"
  int32_t call_tmp_chpl56;
#line 300 "../lib/Chai.chpl"
  chpl_localeID_t call_tmp_chpl57;
#line 300 "../lib/Chai.chpl"
  locale_chpl call_tmp_chpl58;
#line 300 "../lib/Chai.chpl"
  locale_chpl ret_tmp_chpl24;
#line 300 "../lib/Chai.chpl"
  int64_t ret_chpl6;
#line 300 "../lib/Chai.chpl"
  BaseLocale_chpl coerce_tmp_chpl27 = NULL;
#line 300 "../lib/Chai.chpl"
  BaseLocale_chpl coerce_tmp_chpl28 = NULL;
#line 300 "../lib/Chai.chpl"
  _ref_locale_chpl i_x_chpl = NULL;
#line 300 "../lib/Chai.chpl"
  int64_t tmp_x0_chpl5;
#line 300 "../lib/Chai.chpl"
  int64_t tmp_x1_chpl5;
#line 300 "../lib/Chai.chpl"
  _ref__tuple_3_star_range_int64_t_both_one_chpl call_tmp_chpl59 = NULL;
#line 300 "../lib/Chai.chpl"
  _tuple_2_star_int64_t_chpl ret_tmp_chpl25;
#line 300 "../lib/Chai.chpl"
  int64_t numChunks_chpl2;
#line 300 "../lib/Chai.chpl"
  int64_t parDim_chpl2;
#line 300 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl tmpIter_chpl;
#line 300 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl call_tmp_chpl60;
#line 300 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl ret_tmp_chpl26;
#line 300 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl27;
#line 300 "../lib/Chai.chpl"
  chpl___EndCount_AtomicT_int64_t_int64_t _coforallCount_chpl2 = NULL;
#line 300 "../lib/Chai.chpl"
  chpl___EndCount_AtomicT_int64_t_int64_t new_temp_chpl2 = NULL;
#line 300 "../lib/Chai.chpl"
  int64_t numTasks_chpl;
#line 300 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl _ic__F0_this_chpl7;
#line 300 "../lib/Chai.chpl"
  int64_t i_chpl2;
#line 300 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl this_chpl46;
#line 300 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl this_chpl47;
#line 300 "../lib/Chai.chpl"
  int64_t end_chpl;
#line 300 "../lib/Chai.chpl"
  _class_localscoforall_fn_chpl102 _args_forcoforall_fn_chpl = NULL;
#line 300 "../lib/Chai.chpl"
  _ref_Tensor_3__real64_chpl tmp_chpl15 = NULL;
#line 300 "../lib/Chai.chpl"
  _ref_Tensor_4__real64_chpl tmp_chpl16 = NULL;
#line 300 "../lib/Chai.chpl"
  _ref_Tensor_3__real64_chpl tmp_chpl17 = NULL;
#line 300 "../lib/Chai.chpl"
  raw_c_void_ptr _args_vforcoforall_fn_chpl;
#line 300 "../lib/Chai.chpl"
  Error_chpl error_chpl2 = NULL;
#line 311 "../lib/Chai.chpl"
  _tuple_1_star_int64_t_chpl coerce_tmp_chpl29;
#line 311 "../lib/Chai.chpl"
  _ref__tuple_1_star_int64_t_chpl i_i_chpl = NULL;
#line 311 "../lib/Chai.chpl"
  DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl ret_chpl7 = NULL;
#line 311 "../lib/Chai.chpl"
  DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl coerce_tmp_chpl30 = NULL;
#line 311 "../lib/Chai.chpl"
  _tuple_1_star_int64_t_chpl _formal_tmp_in_ind_chpl;
#line 311 "../lib/Chai.chpl"
  int64_t _formal_tmp_ind_x0_chpl;
#line 311 "../lib/Chai.chpl"
  _ddata_Tensor_3__real64_chpl coerce_tmp_chpl31 = NULL;
#line 311 "../lib/Chai.chpl"
  _ref_Tensor_3__real64_chpl call_tmp_chpl61 = NULL;
#line 311 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_3_int64_t_one__real64_int64_t_chpl _field_destructor_tmp__chpl;
#line 311 "../lib/Chai.chpl"
  _domain_DefaultRectangularDom_3_int64_t_one_chpl _field_destructor_tmp__chpl2;
#line 311 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_4_int64_t_one__real64_int64_t_chpl _field_destructor_tmp__chpl3;
#line 311 "../lib/Chai.chpl"
  _domain_DefaultRectangularDom_4_int64_t_one_chpl _field_destructor_tmp__chpl4;
#line 289 "../lib/Chai.chpl"
  int64_t tmp_chpl18;
#line 289 "../lib/Chai.chpl"
  int64_t tmp_chpl19;
#line 289 "../lib/Chai.chpl"
  int64_t tmp_chpl20;
#line 289 "../lib/Chai.chpl"
  int64_t tmp_chpl21;
#line 289 "../lib/Chai.chpl"
  int64_t tmp_chpl22;
#line 289 "../lib/Chai.chpl"
  SumReduceScanOp_Tensor_4__real64_chpl coerce_tmp_chpl32 = NULL;
#line 289 "../lib/Chai.chpl"
  _ref_Tensor_4__real64_chpl i_x_chpl2 = NULL;
#line 289 "../lib/Chai.chpl"
  _ref_Tensor_4__real64_chpl call_tmp_chpl62 = NULL;
#line 289 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_4_int64_t_one__real64_int64_t_chpl _field_destructor_tmp__chpl5;
#line 289 "../lib/Chai.chpl"
  _domain_DefaultRectangularDom_4_int64_t_one_chpl _field_destructor_tmp__chpl6;
#line 289 "../lib/Chai.chpl"
  chpl__class_localscoforall_fn_chpl102_object chpl_macro_tmp_107;
#line 300 "../lib/Chai.chpl"
  local_dataParMinGranularity_chpl = dataParMinGranularity_chpl;
#line 300 "../lib/Chai.chpl"
  local_dataParIgnoreRunningTasks_chpl = dataParIgnoreRunningTasks_chpl;
#line 300 "../lib/Chai.chpl"
  local_dataParTasksPerLocale_chpl = dataParTasksPerLocale_chpl;
#line 289 "../lib/Chai.chpl"
  init_chpl105(&l_chpl);
#line 289 "../lib/Chai.chpl"
  ret_chpl = l_chpl;
#line 289 "../lib/Chai.chpl"
  default_arg_l_chpl = ret_chpl;
#line 289 "../lib/Chai.chpl"
  init_chpl234(&value_chpl);
#line 289 "../lib/Chai.chpl"
  ret_chpl2 = value_chpl;
#line 289 "../lib/Chai.chpl"
  default_arg_value_chpl = ret_chpl2;
#line 289 "../lib/Chai.chpl"
  new_temp_chpl = _new_chpl20(&default_arg_l_chpl, &default_arg_value_chpl, INT64(289), INT32(65));
#line 289 "../lib/Chai.chpl"
  RP_filtersGrad_chpl2 = new_temp_chpl;
#line 289 "../lib/Chai.chpl"
  init_chpl234(&x_chpl);
#line 289 "../lib/Chai.chpl"
  ret_chpl3 = x_chpl;
#line 289 "../lib/Chai.chpl"
  AS_filtersGrad_chpl = ret_chpl3;
#line 289 "../lib/Chai.chpl"
  *(followMe_chpl + INT64(0)) = *(*(locBlock_chpl) + INT64(0));
#line 289 "../lib/Chai.chpl"
  call_tmp_chpl7 = this_chpl20(locBlock_chpl, parDim_chpl, INT64(289), INT32(65));
#line 289 "../lib/Chai.chpl"
  coerce_tmp_chpl3 = *(call_tmp_chpl7);
#line 289 "../lib/Chai.chpl"
  call_tmp_chpl8 = sizeAs_chpl37(coerce_tmp_chpl3, INT64(289), INT32(65));
#line 289 "../lib/Chai.chpl"
  call_tmp_chpl9 = this_chpl20(locBlock_chpl, parDim_chpl, INT64(289), INT32(65));
#line 289 "../lib/Chai.chpl"
  call_tmp_chpl10 = this_chpl20(locBlock_chpl, parDim_chpl, INT64(289), INT32(65));
#line 289 "../lib/Chai.chpl"
  call_tmp_chpl11 = this_chpl20(locBlock_chpl, parDim_chpl, INT64(289), INT32(65));
#line 289 "../lib/Chai.chpl"
  _computeBlock_chpl(call_tmp_chpl8, numChunks_chpl, chunk_chpl, (call_tmp_chpl9)->_high, (call_tmp_chpl10)->_low, (call_tmp_chpl11)->_low, &ret_tmp_chpl, INT64(289), INT32(65));
#line 289 "../lib/Chai.chpl"
  *(call_tmp_chpl12 + INT64(0)) = *(ret_tmp_chpl + INT64(0));
#line 289 "../lib/Chai.chpl"
  *(call_tmp_chpl12 + INT64(1)) = *(ret_tmp_chpl + INT64(1));
#line 289 "../lib/Chai.chpl"
  tmp_x0_chpl = *(call_tmp_chpl12 + INT64(0));
#line 289 "../lib/Chai.chpl"
  tmp_x1_chpl = *(call_tmp_chpl12 + INT64(1));
#line 289 "../lib/Chai.chpl"
  call_tmp_chpl13 = this_chpl20(&followMe_chpl, parDim_chpl, INT64(289), INT32(65));
#line 289 "../lib/Chai.chpl"
  chpl_build_bounded_range6(tmp_x0_chpl, tmp_x1_chpl, &ret_tmp_chpl2);
#line 289 "../lib/Chai.chpl"
  call_tmp_chpl14 = ret_tmp_chpl2;
#line 289 "../lib/Chai.chpl"
  *(call_tmp_chpl13) = call_tmp_chpl14;
#line 289 "../lib/Chai.chpl"
  *(_yield_expr_tmp__chpl + INT64(0)) = *(followMe_chpl + INT64(0));
#line 289 "../lib/Chai.chpl"
  *(chpl_followThis + INT64(0)) = *(_yield_expr_tmp__chpl + INT64(0));
#line 289 "../lib/Chai.chpl"
  _ir_F0_this_chpl = *(deltas_chpl);
#line 289 "../lib/Chai.chpl"
  _ir_F0_this_chpl2 = *(inputs_chpl);
#line 289 "../lib/Chai.chpl"
  coerce_tmp_chpl4 = *(call_tmp_chpl6);
#line 289 "../lib/Chai.chpl"
  _ic__F0_this_chpl = coerce_tmp_chpl4;
#line 289 "../lib/Chai.chpl"
  tmp_chpl = _ir_F0_this_chpl;
#line 289 "../lib/Chai.chpl"
  ret_chpl4 = (&tmp_chpl)->_instance;
#line 289 "../lib/Chai.chpl"
  _ic__F0_this_chpl2 = ret_chpl4;
#line 289 "../lib/Chai.chpl"
  this_chpl31 = ret_chpl4;
#line 289 "../lib/Chai.chpl"
  chpl_check_nil(this_chpl31, INT64(289), INT32(65));
#line 289 "../lib/Chai.chpl"
  coerce_tmp_chpl5 = (this_chpl31)->dom;
#line 289 "../lib/Chai.chpl"
  createTuple_chpl(INT64(0), &ret_tmp_chpl3);
#line 289 "../lib/Chai.chpl"
  init_chpl113(&elt_x0_chpl);
#line 289 "../lib/Chai.chpl"
  tup_x0_chpl = elt_x0_chpl;
#line 289 "../lib/Chai.chpl"
  _ic__F5_block_x0_chpl = tup_x0_chpl;
#line 289 "../lib/Chai.chpl"
  coerce_tmp_chpl6 = *(chpl_followThis + INT64(0));
#line 289 "../lib/Chai.chpl"
  coerce_tmp_chpl12 = (&coerce_tmp_chpl6)->_high;
#line 289 "../lib/Chai.chpl"
  this_chpl32 = coerce_tmp_chpl5;
#line 289 "../lib/Chai.chpl"
  chpl_check_nil(this_chpl32, INT64(289), INT32(65));
#line 289 "../lib/Chai.chpl"
  call_tmp_chpl17 = &((this_chpl32)->ranges);
#line 289 "../lib/Chai.chpl"
  coerce_tmp_chpl7 = *(*(call_tmp_chpl17) + INT64(0));
#line 289 "../lib/Chai.chpl"
  call_tmp_chpl15 = sizeAs_chpl36(coerce_tmp_chpl7, INT64(289), INT32(65));
#line 289 "../lib/Chai.chpl"
  if (! (coerce_tmp_chpl12 < INT64(0))) /* ZLINE: 289 ../lib/Chai.chpl */
#line 289 "../lib/Chai.chpl"
  {
#line 289 "../lib/Chai.chpl"
    tmp_chpl2 = (((uint64_t)(coerce_tmp_chpl12)) >= call_tmp_chpl15);
#line 289 "../lib/Chai.chpl"
  } else /* ZLINE: 289 ../lib/Chai.chpl */
#line 289 "../lib/Chai.chpl"
  {
#line 289 "../lib/Chai.chpl"
    tmp_chpl2 = UINT8(false);
#line 289 "../lib/Chai.chpl"
  }
#line 289 "../lib/Chai.chpl"
  if (tmp_chpl2) /* ZLINE: 289 ../lib/Chai.chpl */
#line 289 "../lib/Chai.chpl"
  {
#line 289 "../lib/Chai.chpl"
    boundsCheckHalt_chpl(&_str_literal_4152_chpl, INT64(289), INT32(65));
#line 289 "../lib/Chai.chpl"
  }
#line 289 "../lib/Chai.chpl"
  this_chpl33 = coerce_tmp_chpl5;
#line 289 "../lib/Chai.chpl"
  chpl_check_nil(this_chpl33, INT64(289), INT32(65));
#line 289 "../lib/Chai.chpl"
  call_tmp_chpl18 = &((this_chpl33)->ranges);
#line 289 "../lib/Chai.chpl"
  coerce_tmp_chpl8 = *(*(call_tmp_chpl18) + INT64(0));
#line 289 "../lib/Chai.chpl"
  coerce_tmp_chpl9 = *(chpl_followThis + INT64(0));
#line 289 "../lib/Chai.chpl"
  call_tmp_chpl19 = ((&coerce_tmp_chpl8)->_low + ((int64_t)(((&coerce_tmp_chpl9)->_low * INT64(1)))));
#line 289 "../lib/Chai.chpl"
  this_chpl34 = coerce_tmp_chpl5;
#line 289 "../lib/Chai.chpl"
  chpl_check_nil(this_chpl34, INT64(289), INT32(65));
#line 289 "../lib/Chai.chpl"
  call_tmp_chpl20 = &((this_chpl34)->ranges);
#line 289 "../lib/Chai.chpl"
  coerce_tmp_chpl10 = *(*(call_tmp_chpl20) + INT64(0));
#line 289 "../lib/Chai.chpl"
  coerce_tmp_chpl11 = *(chpl_followThis + INT64(0));
#line 289 "../lib/Chai.chpl"
  call_tmp_chpl21 = ((&coerce_tmp_chpl10)->_low + ((int64_t)(((&coerce_tmp_chpl11)->_high * INT64(1)))));
#line 289 "../lib/Chai.chpl"
  call_tmp_chpl16 = &_ic__F5_block_x0_chpl;
#line 289 "../lib/Chai.chpl"
  compilerAssert_chpl();
#line 289 "../lib/Chai.chpl"
  (call_tmp_chpl16)->_low = call_tmp_chpl19;
#line 289 "../lib/Chai.chpl"
  (call_tmp_chpl16)->_high = call_tmp_chpl21;
#line 289 "../lib/Chai.chpl"
  *(retcopy_chpl + INT64(0)) = _ic__F5_block_x0_chpl;
#line 289 "../lib/Chai.chpl"
  _ic__F7_coerce_tmp_x0_chpl = *(retcopy_chpl + INT64(0));
#line 289 "../lib/Chai.chpl"
  _ic__F0_this_chpl3 = _ic__F7_coerce_tmp_x0_chpl;
#line 289 "../lib/Chai.chpl"
  checkIfIterWillOverflow_chpl(_ic__F7_coerce_tmp_x0_chpl, UINT8(true), INT64(289), INT32(65));
#line 289 "../lib/Chai.chpl"
  _ic__F8_i_chpl = INT64(0);
#line 289 "../lib/Chai.chpl"
  this_chpl35 = _ic__F0_this_chpl3;
#line 289 "../lib/Chai.chpl"
  coerce_tmp_chpl13 = (&this_chpl35)->_low;
#line 289 "../lib/Chai.chpl"
  this_chpl36 = _ic__F0_this_chpl3;
#line 289 "../lib/Chai.chpl"
  _ic__F9_end_chpl = (&this_chpl36)->_high;
#line 289 "../lib/Chai.chpl"
  tmp_chpl3 = _ir_F0_this_chpl2;
#line 289 "../lib/Chai.chpl"
  ret_chpl5 = (&tmp_chpl3)->_instance;
#line 289 "../lib/Chai.chpl"
  _ic__F0_this_chpl4 = ret_chpl5;
#line 289 "../lib/Chai.chpl"
  this_chpl37 = ret_chpl5;
#line 289 "../lib/Chai.chpl"
  chpl_check_nil(this_chpl37, INT64(289), INT32(65));
#line 289 "../lib/Chai.chpl"
  coerce_tmp_chpl14 = (this_chpl37)->dom;
#line 289 "../lib/Chai.chpl"
  createTuple_chpl(INT64(0), &ret_tmp_chpl4);
#line 289 "../lib/Chai.chpl"
  init_chpl113(&elt_x0_chpl2);
#line 289 "../lib/Chai.chpl"
  tup_x0_chpl2 = elt_x0_chpl2;
#line 289 "../lib/Chai.chpl"
  _ic__F5_block_x0_chpl2 = tup_x0_chpl2;
#line 289 "../lib/Chai.chpl"
  coerce_tmp_chpl15 = *(chpl_followThis + INT64(0));
#line 289 "../lib/Chai.chpl"
  coerce_tmp_chpl21 = (&coerce_tmp_chpl15)->_high;
#line 289 "../lib/Chai.chpl"
  this_chpl38 = coerce_tmp_chpl14;
#line 289 "../lib/Chai.chpl"
  chpl_check_nil(this_chpl38, INT64(289), INT32(65));
#line 289 "../lib/Chai.chpl"
  call_tmp_chpl24 = &((this_chpl38)->ranges);
#line 289 "../lib/Chai.chpl"
  coerce_tmp_chpl16 = *(*(call_tmp_chpl24) + INT64(0));
#line 289 "../lib/Chai.chpl"
  call_tmp_chpl22 = sizeAs_chpl36(coerce_tmp_chpl16, INT64(289), INT32(65));
#line 289 "../lib/Chai.chpl"
  if (! (coerce_tmp_chpl21 < INT64(0))) /* ZLINE: 289 ../lib/Chai.chpl */
#line 289 "../lib/Chai.chpl"
  {
#line 289 "../lib/Chai.chpl"
    tmp_chpl4 = (((uint64_t)(coerce_tmp_chpl21)) >= call_tmp_chpl22);
#line 289 "../lib/Chai.chpl"
  } else /* ZLINE: 289 ../lib/Chai.chpl */
#line 289 "../lib/Chai.chpl"
  {
#line 289 "../lib/Chai.chpl"
    tmp_chpl4 = UINT8(false);
#line 289 "../lib/Chai.chpl"
  }
#line 289 "../lib/Chai.chpl"
  if (tmp_chpl4) /* ZLINE: 289 ../lib/Chai.chpl */
#line 289 "../lib/Chai.chpl"
  {
#line 289 "../lib/Chai.chpl"
    boundsCheckHalt_chpl(&_str_literal_4152_chpl, INT64(289), INT32(65));
#line 289 "../lib/Chai.chpl"
  }
#line 289 "../lib/Chai.chpl"
  this_chpl39 = coerce_tmp_chpl14;
#line 289 "../lib/Chai.chpl"
  chpl_check_nil(this_chpl39, INT64(289), INT32(65));
#line 289 "../lib/Chai.chpl"
  call_tmp_chpl25 = &((this_chpl39)->ranges);
#line 289 "../lib/Chai.chpl"
  coerce_tmp_chpl17 = *(*(call_tmp_chpl25) + INT64(0));
#line 289 "../lib/Chai.chpl"
  coerce_tmp_chpl18 = *(chpl_followThis + INT64(0));
#line 289 "../lib/Chai.chpl"
  call_tmp_chpl26 = ((&coerce_tmp_chpl17)->_low + ((int64_t)(((&coerce_tmp_chpl18)->_low * INT64(1)))));
#line 289 "../lib/Chai.chpl"
  this_chpl40 = coerce_tmp_chpl14;
#line 289 "../lib/Chai.chpl"
  chpl_check_nil(this_chpl40, INT64(289), INT32(65));
#line 289 "../lib/Chai.chpl"
  call_tmp_chpl27 = &((this_chpl40)->ranges);
#line 289 "../lib/Chai.chpl"
  coerce_tmp_chpl19 = *(*(call_tmp_chpl27) + INT64(0));
#line 289 "../lib/Chai.chpl"
  coerce_tmp_chpl20 = *(chpl_followThis + INT64(0));
#line 289 "../lib/Chai.chpl"
  call_tmp_chpl28 = ((&coerce_tmp_chpl19)->_low + ((int64_t)(((&coerce_tmp_chpl20)->_high * INT64(1)))));
#line 289 "../lib/Chai.chpl"
  call_tmp_chpl23 = &_ic__F5_block_x0_chpl2;
#line 289 "../lib/Chai.chpl"
  compilerAssert_chpl();
#line 289 "../lib/Chai.chpl"
  (call_tmp_chpl23)->_low = call_tmp_chpl26;
#line 289 "../lib/Chai.chpl"
  (call_tmp_chpl23)->_high = call_tmp_chpl28;
#line 289 "../lib/Chai.chpl"
  *(retcopy_chpl2 + INT64(0)) = _ic__F5_block_x0_chpl2;
#line 289 "../lib/Chai.chpl"
  _ic__F7_coerce_tmp_x0_chpl2 = *(retcopy_chpl2 + INT64(0));
#line 289 "../lib/Chai.chpl"
  _ic__F0_this_chpl5 = _ic__F7_coerce_tmp_x0_chpl2;
#line 289 "../lib/Chai.chpl"
  checkIfIterWillOverflow_chpl(_ic__F7_coerce_tmp_x0_chpl2, UINT8(true), INT64(289), INT32(65));
#line 289 "../lib/Chai.chpl"
  _ic__F8_i_chpl2 = INT64(0);
#line 289 "../lib/Chai.chpl"
  this_chpl41 = _ic__F0_this_chpl5;
#line 289 "../lib/Chai.chpl"
  coerce_tmp_chpl22 = (&this_chpl41)->_low;
#line 289 "../lib/Chai.chpl"
  this_chpl42 = _ic__F0_this_chpl5;
#line 289 "../lib/Chai.chpl"
  _ic__F9_end_chpl2 = (&this_chpl42)->_high;
#line 289 "../lib/Chai.chpl"
  tmp_chpl5 = *(chpl_followThis + INT64(0));
#line 289 "../lib/Chai.chpl"
  _ic__F2_myFollowThis_chpl = tmp_chpl5;
#line 289 "../lib/Chai.chpl"
  tmp_chpl6 = tmp_chpl5;
#line 289 "../lib/Chai.chpl"
  if ((&tmp_chpl6)->_low > (&tmp_chpl6)->_high) /* ZLINE: 289 ../lib/Chai.chpl */
#line 289 "../lib/Chai.chpl"
  {
#line 289 "../lib/Chai.chpl"
    tmp_chpl8 = UINT8(false);
#line 289 "../lib/Chai.chpl"
  } else /* ZLINE: 289 ../lib/Chai.chpl */
#line 289 "../lib/Chai.chpl"
  {
#line 289 "../lib/Chai.chpl"
    tmp_chpl8 = UINT8(true);
#line 289 "../lib/Chai.chpl"
  }
#line 289 "../lib/Chai.chpl"
  if (! tmp_chpl8) /* ZLINE: 289 ../lib/Chai.chpl */
#line 289 "../lib/Chai.chpl"
  {
#line 289 "../lib/Chai.chpl"
    tmp_chpl9 = tmp_chpl5;
#line 289 "../lib/Chai.chpl"
    if (! ((&tmp_chpl9)->_low > (&tmp_chpl9)->_high)) /* ZLINE: 289 ../lib/Chai.chpl */
#line 289 "../lib/Chai.chpl"
    {
#line 289 "../lib/Chai.chpl"
      boundsCheckHalt_chpl(&_str_literal_633_chpl, INT64(289), INT32(65));
#line 289 "../lib/Chai.chpl"
    }
#line 289 "../lib/Chai.chpl"
  }
#line 289 "../lib/Chai.chpl"
  call_tmp_chpl29 = sizeAs_chpl37(tmp_chpl5, INT64(289), INT32(65));
#line 289 "../lib/Chai.chpl"
  if (call_tmp_chpl29 != INT64(0)) /* ZLINE: 289 ../lib/Chai.chpl */
#line 289 "../lib/Chai.chpl"
  {
#line 289 "../lib/Chai.chpl"
  }
#line 289 "../lib/Chai.chpl"
  tmp_chpl7 = &_ic__F3_r_chpl;
#line 289 "../lib/Chai.chpl"
  init_chpl113(tmp_chpl7);
#line 289 "../lib/Chai.chpl"
  if (call_tmp_chpl29 != INT64(0)) /* ZLINE: 289 ../lib/Chai.chpl */
#line 289 "../lib/Chai.chpl"
  {
#line 289 "../lib/Chai.chpl"
    tmp_chpl10 = _ic__F2_myFollowThis_chpl;
#line 289 "../lib/Chai.chpl"
    call_tmp_chpl30 = orderToIndex_chpl(coerce_tmp_chpl4, (&tmp_chpl10)->_low, INT64(289), INT32(65));
#line 289 "../lib/Chai.chpl"
    tmp_chpl11 = _ic__F2_myFollowThis_chpl;
#line 289 "../lib/Chai.chpl"
    call_tmp_chpl31 = orderToIndex_chpl(_ic__F0_this_chpl, (&tmp_chpl11)->_high, INT64(289), INT32(65));
#line 289 "../lib/Chai.chpl"
    tmp_chpl12 = &_ic__F3_r_chpl;
#line 289 "../lib/Chai.chpl"
    (tmp_chpl12)->_low = call_tmp_chpl30;
#line 289 "../lib/Chai.chpl"
    (tmp_chpl12)->_high = call_tmp_chpl31;
#line 289 "../lib/Chai.chpl"
    verifyAppropriateStide_chpl2(INT64(1), INT64(289), INT32(65));
#line 289 "../lib/Chai.chpl"
  }
#line 289 "../lib/Chai.chpl"
  coerce_tmp_chpl23 = _ic__F3_r_chpl;
#line 289 "../lib/Chai.chpl"
  _ic__F0_this_chpl6 = coerce_tmp_chpl23;
#line 289 "../lib/Chai.chpl"
  checkIfIterWillOverflow_chpl(coerce_tmp_chpl23, UINT8(true), INT64(289), INT32(65));
#line 289 "../lib/Chai.chpl"
  _ic__F5_i_chpl = INT64(0);
#line 289 "../lib/Chai.chpl"
  this_chpl43 = _ic__F0_this_chpl6;
#line 289 "../lib/Chai.chpl"
  for (_ic__F8_i_chpl = coerce_tmp_chpl13,_ic__F8_i_chpl2 = coerce_tmp_chpl22,_ic__F5_i_chpl = (&this_chpl43)->_low; (tmp_chpl18 = (_ic__F8_i_chpl <= _ic__F9_end_chpl),tmp_chpl18); tmp_chpl19 = _ic__F8_i_chpl,tmp_chpl19 += INT64(1),_ic__F8_i_chpl = tmp_chpl19,tmp_chpl20 = _ic__F8_i_chpl2,tmp_chpl20 += INT64(1),_ic__F8_i_chpl2 = tmp_chpl20,tmp_chpl21 = _ic__F5_i_chpl,tmp_chpl21 += INT64(1),_ic__F5_i_chpl = tmp_chpl21) {
#line 289 "../lib/Chai.chpl"
    this_chpl44 = _ic__F0_this_chpl4;
#line 289 "../lib/Chai.chpl"
    chpl_check_nil(this_chpl44, INT64(289), INT32(65));
#line 289 "../lib/Chai.chpl"
    coerce_tmp_chpl24 = (this_chpl44)->shiftedData;
#line 289 "../lib/Chai.chpl"
    call_tmp_chpl32 = (coerce_tmp_chpl24 + _ic__F8_i_chpl2);
#line 289 "../lib/Chai.chpl"
    tmp_chpl13 = (_ic__F8_i_chpl2 <= _ic__F9_end_chpl2);
#line 289 "../lib/Chai.chpl"
    hasMore_chpl = (tmp_chpl13 != INT64(0));
#line 289 "../lib/Chai.chpl"
    if (! hasMore_chpl) /* ZLINE: 289 ../lib/Chai.chpl */
#line 289 "../lib/Chai.chpl"
    {
#line 289 "../lib/Chai.chpl"
      chpl_error("zippered iterations have non-equal lengths", INT64(289), INT32(65));
#line 289 "../lib/Chai.chpl"
    }
#line 289 "../lib/Chai.chpl"
    this_chpl45 = _ic__F0_this_chpl2;
#line 289 "../lib/Chai.chpl"
    chpl_check_nil(this_chpl45, INT64(289), INT32(65));
#line 289 "../lib/Chai.chpl"
    coerce_tmp_chpl25 = (this_chpl45)->shiftedData;
#line 289 "../lib/Chai.chpl"
    call_tmp_chpl33 = (coerce_tmp_chpl25 + _ic__F8_i_chpl);
#line 289 "../lib/Chai.chpl"
    i_chpl = _ic__F5_i_chpl;
#line 291 "../lib/Chai.chpl"
    shape_chpl8(call_tmp_chpl32, &ret_tmp_chpl5);
#line 291 "../lib/Chai.chpl"
    *(call_tmp_chpl34 + INT64(0)) = *(ret_tmp_chpl5 + INT64(0));
#line 291 "../lib/Chai.chpl"
    *(call_tmp_chpl34 + INT64(1)) = *(ret_tmp_chpl5 + INT64(1));
#line 291 "../lib/Chai.chpl"
    *(call_tmp_chpl34 + INT64(2)) = *(ret_tmp_chpl5 + INT64(2));
#line 291 "../lib/Chai.chpl"
    tmp_x0_chpl2 = *(call_tmp_chpl34 + INT64(0));
#line 291 "../lib/Chai.chpl"
    tmp_x1_chpl2 = *(call_tmp_chpl34 + INT64(1));
#line 291 "../lib/Chai.chpl"
    tmp_x2_chpl = *(call_tmp_chpl34 + INT64(2));
#line 292 "../lib/Chai.chpl"
    call_tmp_chpl35 = *(filters_svar_chpl);
#line 292 "../lib/Chai.chpl"
    shape_chpl5(&call_tmp_chpl35, &ret_tmp_chpl6);
#line 292 "../lib/Chai.chpl"
    *(call_tmp_chpl36 + INT64(0)) = *(ret_tmp_chpl6 + INT64(0));
#line 292 "../lib/Chai.chpl"
    *(call_tmp_chpl36 + INT64(1)) = *(ret_tmp_chpl6 + INT64(1));
#line 292 "../lib/Chai.chpl"
    *(call_tmp_chpl36 + INT64(2)) = *(ret_tmp_chpl6 + INT64(2));
#line 292 "../lib/Chai.chpl"
    *(call_tmp_chpl36 + INT64(3)) = *(ret_tmp_chpl6 + INT64(3));
#line 292 "../lib/Chai.chpl"
    tmp_x0_chpl3 = *(call_tmp_chpl36 + INT64(0));
#line 292 "../lib/Chai.chpl"
    tmp_x1_chpl3 = *(call_tmp_chpl36 + INT64(1));
#line 292 "../lib/Chai.chpl"
    tmp_x2_chpl2 = *(call_tmp_chpl36 + INT64(2));
#line 292 "../lib/Chai.chpl"
    tmp_x3_chpl = *(call_tmp_chpl36 + INT64(3));
#line 293 "../lib/Chai.chpl"
    shape_chpl8(call_tmp_chpl33, &ret_tmp_chpl7);
#line 293 "../lib/Chai.chpl"
    *(call_tmp_chpl37 + INT64(0)) = *(ret_tmp_chpl7 + INT64(0));
#line 293 "../lib/Chai.chpl"
    *(call_tmp_chpl37 + INT64(1)) = *(ret_tmp_chpl7 + INT64(1));
#line 293 "../lib/Chai.chpl"
    *(call_tmp_chpl37 + INT64(2)) = *(ret_tmp_chpl7 + INT64(2));
#line 293 "../lib/Chai.chpl"
    tmp_x0_chpl4 = *(call_tmp_chpl37 + INT64(0));
#line 293 "../lib/Chai.chpl"
    tmp_x1_chpl4 = *(call_tmp_chpl37 + INT64(1));
#line 294 "../lib/Chai.chpl"
    if (*(call_tmp_chpl37 + INT64(2)) != tmp_x0_chpl3) /* ZLINE: 294 ../lib/Chai.chpl */
#line 294 "../lib/Chai.chpl"
    {
#line 294 "../lib/Chai.chpl"
      err_chpl4(&_str_literal_1702_chpl);
#line 294 "../lib/Chai.chpl"
    }
#line 295 "../lib/Chai.chpl"
    if (tmp_x2_chpl != tmp_x3_chpl) /* ZLINE: 295 ../lib/Chai.chpl */
#line 295 "../lib/Chai.chpl"
    {
#line 295 "../lib/Chai.chpl"
      err_chpl4(&_str_literal_1703_chpl);
#line 295 "../lib/Chai.chpl"
    }
#line 296 "../lib/Chai.chpl"
    filterGradient_chpl(call_tmp_chpl32, call_tmp_chpl33, stride_svar_chpl, padding_svar_chpl, tmp_x1_chpl3, &ret_tmp_chpl8);
#line 296 "../lib/Chai.chpl"
    call_tmp_chpl38 = ret_tmp_chpl8;
#line 296 "../lib/Chai.chpl"
    dL_dF_chpl = call_tmp_chpl38;
#line 297 "../lib/Chai.chpl"
    chpl___PLUS__ASSIGN_3(&AS_filtersGrad_chpl, &dL_dF_chpl);
#line 298 "../lib/Chai.chpl"
    init_chpl233(&initTemp_chpl);
#line 298 "../lib/Chai.chpl"
    call_tmp_chpl39 = initTemp_chpl;
#line 298 "../lib/Chai.chpl"
    dL_dX_chpl = call_tmp_chpl39;
#line 299 "../lib/Chai.chpl"
    chpl_build_low_bounded_range2(INT64(0), &ret_tmp_chpl9);
#line 299 "../lib/Chai.chpl"
    call_tmp_chpl40 = ret_tmp_chpl9;
#line 299 "../lib/Chai.chpl"
    chpl___POUND_3(call_tmp_chpl40, tmp_x0_chpl2, &ret_tmp_chpl10, INT64(299), INT32(65));
#line 299 "../lib/Chai.chpl"
    call_tmp_chpl41 = ret_tmp_chpl10;
#line 299 "../lib/Chai.chpl"
    chpl_build_low_bounded_range2(INT64(0), &ret_tmp_chpl11);
#line 299 "../lib/Chai.chpl"
    call_tmp_chpl42 = ret_tmp_chpl11;
#line 299 "../lib/Chai.chpl"
    chpl___POUND_3(call_tmp_chpl42, tmp_x1_chpl2, &ret_tmp_chpl12, INT64(299), INT32(65));
#line 299 "../lib/Chai.chpl"
    call_tmp_chpl43 = ret_tmp_chpl12;
#line 299 "../lib/Chai.chpl"
    chpl_build_low_bounded_range2(INT64(0), &ret_tmp_chpl13);
#line 299 "../lib/Chai.chpl"
    call_tmp_chpl44 = ret_tmp_chpl13;
#line 299 "../lib/Chai.chpl"
    chpl___POUND_3(call_tmp_chpl44, tmp_x3_chpl, &ret_tmp_chpl14, INT64(299), INT32(65));
#line 299 "../lib/Chai.chpl"
    call_tmp_chpl45 = ret_tmp_chpl14;
#line 299 "../lib/Chai.chpl"
    chpl__buildDomainExpr(call_tmp_chpl41, call_tmp_chpl43, call_tmp_chpl45, UINT8(true), &ret_tmp_chpl15, INT64(299), INT32(65));
#line 299 "../lib/Chai.chpl"
    call_tmp_chpl46 = ret_tmp_chpl15;
#line 299 "../lib/Chai.chpl"
    reshapeDomain_chpl3(&dL_dX_chpl, &call_tmp_chpl46);
#line 299 "../lib/Chai.chpl"
    chpl__autoDestroy8(&call_tmp_chpl46, INT64(299), INT32(65));
#line 300 "../lib/Chai.chpl"
    chpl_build_low_bounded_range2(INT64(0), &ret_tmp_chpl16);
#line 300 "../lib/Chai.chpl"
    call_tmp_chpl48 = ret_tmp_chpl16;
#line 300 "../lib/Chai.chpl"
    chpl___POUND_3(call_tmp_chpl48, tmp_x0_chpl2, &ret_tmp_chpl17, INT64(300), INT32(65));
#line 300 "../lib/Chai.chpl"
    call_tmp_chpl49 = ret_tmp_chpl17;
#line 300 "../lib/Chai.chpl"
    chpl_build_low_bounded_range2(INT64(0), &ret_tmp_chpl18);
#line 300 "../lib/Chai.chpl"
    call_tmp_chpl50 = ret_tmp_chpl18;
#line 300 "../lib/Chai.chpl"
    chpl___POUND_3(call_tmp_chpl50, tmp_x1_chpl2, &ret_tmp_chpl19, INT64(300), INT32(65));
#line 300 "../lib/Chai.chpl"
    call_tmp_chpl51 = ret_tmp_chpl19;
#line 300 "../lib/Chai.chpl"
    chpl_build_low_bounded_range2(INT64(0), &ret_tmp_chpl20);
#line 300 "../lib/Chai.chpl"
    call_tmp_chpl52 = ret_tmp_chpl20;
#line 300 "../lib/Chai.chpl"
    chpl___POUND_3(call_tmp_chpl52, tmp_x3_chpl, &ret_tmp_chpl21, INT64(300), INT32(65));
#line 300 "../lib/Chai.chpl"
    call_tmp_chpl53 = ret_tmp_chpl21;
#line 300 "../lib/Chai.chpl"
    chpl__buildDomainExpr(call_tmp_chpl49, call_tmp_chpl51, call_tmp_chpl53, UINT8(true), &ret_tmp_chpl22, INT64(300), INT32(65));
#line 300 "../lib/Chai.chpl"
    call_tmp_chpl47 = ret_tmp_chpl22;
#line 300 "../lib/Chai.chpl"
    outChannels_chpl = tmp_x0_chpl3;
#line 300 "../lib/Chai.chpl"
    dh_chpl = tmp_x0_chpl4;
#line 300 "../lib/Chai.chpl"
    dw_chpl = tmp_x1_chpl4;
#line 300 "../lib/Chai.chpl"
    kh_chpl = tmp_x1_chpl3;
#line 300 "../lib/Chai.chpl"
    kw_chpl = tmp_x2_chpl2;
#line 300 "../lib/Chai.chpl"
    stride_svar_chpl2 = stride_svar_chpl;
#line 300 "../lib/Chai.chpl"
    padding_svar_chpl2 = padding_svar_chpl;
#line 300 "../lib/Chai.chpl"
    coerce_tmp_chpl26 = (&call_tmp_chpl47)->_instance;
#line 300 "../lib/Chai.chpl"
    tasksPerLocale_chpl = local_dataParTasksPerLocale_chpl;
#line 300 "../lib/Chai.chpl"
    ignoreRunning_chpl = local_dataParIgnoreRunningTasks_chpl;
#line 300 "../lib/Chai.chpl"
    minIndicesPerTask_chpl = local_dataParMinGranularity_chpl;
#line 300 "../lib/Chai.chpl"
    createTuple_chpl2(INT64(0), &ret_tmp_chpl23);
#line 300 "../lib/Chai.chpl"
    error_chpl = NULL;
#line 300 "../lib/Chai.chpl"
    if (tasksPerLocale_chpl == INT64(0)) /* ZLINE: 300 ../lib/Chai.chpl */
#line 300 "../lib/Chai.chpl"
    {
#line 300 "../lib/Chai.chpl"
      call_tmp_chpl56 = get_chpl_nodeID();
#line 300 "../lib/Chai.chpl"
      call_tmp_chpl57 = chpl_rt_buildLocaleID(call_tmp_chpl56, c_sublocid_any);
#line 300 "../lib/Chai.chpl"
      call_tmp_chpl55 = call_tmp_chpl57;
#line 300 "../lib/Chai.chpl"
      chpl_localeID_to_locale(&call_tmp_chpl55, &ret_tmp_chpl24, INT64(300), INT32(65));
#line 300 "../lib/Chai.chpl"
      call_tmp_chpl58 = ret_tmp_chpl24;
#line 300 "../lib/Chai.chpl"
      call_tmp_chpl54 = call_tmp_chpl58;
#line 300 "../lib/Chai.chpl"
      coerce_tmp_chpl27 = (&call_tmp_chpl54)->_instance;
#line 300 "../lib/Chai.chpl"
      if (((RootClass_chpl)(coerce_tmp_chpl27)) == nil) /* ZLINE: 300 ../lib/Chai.chpl */
#line 300 "../lib/Chai.chpl"
      {
#line 300 "../lib/Chai.chpl"
        nilCheckHalt_chpl(&_str_literal_52_chpl, INT64(300), INT32(65));
#line 300 "../lib/Chai.chpl"
      }
#line 300 "../lib/Chai.chpl"
      coerce_tmp_chpl28 = coerce_tmp_chpl27;
#line 300 "../lib/Chai.chpl"
      chpl_check_nil(coerce_tmp_chpl28, INT64(300), INT32(65));
#line 300 "../lib/Chai.chpl"
      ret_chpl6 = (coerce_tmp_chpl28)->maxTaskPar;
#line 300 "../lib/Chai.chpl"
      tmp_chpl14 = ret_chpl6;
#line 300 "../lib/Chai.chpl"
      i_x_chpl = &call_tmp_chpl54;
#line 300 "../lib/Chai.chpl"
      deinit_chpl62(i_x_chpl);
#line 300 "../lib/Chai.chpl"
    } else /* ZLINE: 300 ../lib/Chai.chpl */
#line 300 "../lib/Chai.chpl"
    {
#line 300 "../lib/Chai.chpl"
      tmp_chpl14 = tasksPerLocale_chpl;
#line 300 "../lib/Chai.chpl"
    }
#line 300 "../lib/Chai.chpl"
    if (chpl_task_getSerial()) /* ZLINE: 300 ../lib/Chai.chpl */
#line 300 "../lib/Chai.chpl"
    {
#line 300 "../lib/Chai.chpl"
      tmp_x0_chpl5 = INT64(1);
#line 300 "../lib/Chai.chpl"
      tmp_x1_chpl5 = INT64(0);
#line 300 "../lib/Chai.chpl"
    } else /* ZLINE: 300 ../lib/Chai.chpl */
#line 300 "../lib/Chai.chpl"
    {
#line 300 "../lib/Chai.chpl"
      chpl_check_nil(coerce_tmp_chpl26, INT64(300), INT32(65));
#line 300 "../lib/Chai.chpl"
      call_tmp_chpl59 = &((coerce_tmp_chpl26)->ranges);
#line 300 "../lib/Chai.chpl"
      _computeChunkStuff_chpl3(tmp_chpl14, ignoreRunning_chpl, minIndicesPerTask_chpl, call_tmp_chpl59, &ret_tmp_chpl25, INT64(300), INT32(65));
#line 300 "../lib/Chai.chpl"
      tmp_x0_chpl5 = *(ret_tmp_chpl25 + INT64(0));
#line 300 "../lib/Chai.chpl"
      tmp_x1_chpl5 = *(ret_tmp_chpl25 + INT64(1));
#line 300 "../lib/Chai.chpl"
    }
#line 300 "../lib/Chai.chpl"
    numChunks_chpl2 = tmp_x0_chpl5;
#line 300 "../lib/Chai.chpl"
    parDim_chpl2 = tmp_x1_chpl5;
#line 300 "../lib/Chai.chpl"
    chpl_build_low_bounded_range2(INT64(0), &ret_tmp_chpl26);
#line 300 "../lib/Chai.chpl"
    call_tmp_chpl60 = ret_tmp_chpl26;
#line 300 "../lib/Chai.chpl"
    chpl___POUND_3(call_tmp_chpl60, tmp_x0_chpl5, &ret_tmp_chpl27, INT64(300), INT32(65));
#line 300 "../lib/Chai.chpl"
    tmpIter_chpl = ret_tmp_chpl27;
#line 300 "../lib/Chai.chpl"
    new_temp_chpl2 = _new_chpl17(INT64(300), INT32(65));
#line 300 "../lib/Chai.chpl"
    _coforallCount_chpl2 = new_temp_chpl2;
#line 300 "../lib/Chai.chpl"
    numTasks_chpl = chpl_boundedCoforallSize(tmpIter_chpl, INT64(300), INT32(65));
#line 300 "../lib/Chai.chpl"
    _upEndCount_chpl(_coforallCount_chpl2, numTasks_chpl, INT64(300), INT32(65));
#line 300 "../lib/Chai.chpl"
    chpl_resetTaskSpawn(numTasks_chpl, INT64(300), INT32(65));
#line 300 "../lib/Chai.chpl"
    _ic__F0_this_chpl7 = tmpIter_chpl;
#line 300 "../lib/Chai.chpl"
    checkIfIterWillOverflow_chpl(tmpIter_chpl, UINT8(true), INT64(300), INT32(65));
#line 300 "../lib/Chai.chpl"
    i_chpl2 = INT64(0);
#line 300 "../lib/Chai.chpl"
    this_chpl46 = _ic__F0_this_chpl7;
#line 300 "../lib/Chai.chpl"
    this_chpl47 = _ic__F0_this_chpl7;
#line 300 "../lib/Chai.chpl"
    end_chpl = (&this_chpl47)->_high;
#line 300 "../lib/Chai.chpl"
    for (i_chpl2 = (&this_chpl46)->_low; ((i_chpl2 <= end_chpl)); i_chpl2 += INT64(1)) {
#line 300 "../lib/Chai.chpl"
      _args_forcoforall_fn_chpl = ((_class_localscoforall_fn_chpl102)(&chpl_macro_tmp_107));
#line 300 "../lib/Chai.chpl"
      chpl_check_nil(_args_forcoforall_fn_chpl, INT64(300), INT32(65));
#line 300 "../lib/Chai.chpl"
      (_args_forcoforall_fn_chpl)->_0_numChunks = numChunks_chpl2;
#line 300 "../lib/Chai.chpl"
      chpl_check_nil(_args_forcoforall_fn_chpl, INT64(300), INT32(65));
#line 300 "../lib/Chai.chpl"
      (_args_forcoforall_fn_chpl)->_1_parDim = parDim_chpl2;
#line 300 "../lib/Chai.chpl"
      chpl_check_nil(_args_forcoforall_fn_chpl, INT64(300), INT32(65));
#line 300 "../lib/Chai.chpl"
      (_args_forcoforall_fn_chpl)->_2_coerce_tmp = coerce_tmp_chpl26;
#line 300 "../lib/Chai.chpl"
      chpl_check_nil(_args_forcoforall_fn_chpl, INT64(300), INT32(65));
#line 300 "../lib/Chai.chpl"
      (_args_forcoforall_fn_chpl)->_3__coforallCount = _coforallCount_chpl2;
#line 300 "../lib/Chai.chpl"
      chpl_check_nil(_args_forcoforall_fn_chpl, INT64(300), INT32(65));
#line 300 "../lib/Chai.chpl"
      (_args_forcoforall_fn_chpl)->_4_chunk = i_chpl2;
#line 300 "../lib/Chai.chpl"
      chpl_check_nil(_args_forcoforall_fn_chpl, INT64(300), INT32(65));
#line 300 "../lib/Chai.chpl"
      (_args_forcoforall_fn_chpl)->_6_outChannels = outChannels_chpl;
#line 300 "../lib/Chai.chpl"
      chpl_check_nil(_args_forcoforall_fn_chpl, INT64(300), INT32(65));
#line 300 "../lib/Chai.chpl"
      (_args_forcoforall_fn_chpl)->_7_dh = dh_chpl;
#line 300 "../lib/Chai.chpl"
      chpl_check_nil(_args_forcoforall_fn_chpl, INT64(300), INT32(65));
#line 300 "../lib/Chai.chpl"
      (_args_forcoforall_fn_chpl)->_8_dw = dw_chpl;
#line 300 "../lib/Chai.chpl"
      chpl_check_nil(_args_forcoforall_fn_chpl, INT64(300), INT32(65));
#line 300 "../lib/Chai.chpl"
      (_args_forcoforall_fn_chpl)->_9_kh = kh_chpl;
#line 300 "../lib/Chai.chpl"
      chpl_check_nil(_args_forcoforall_fn_chpl, INT64(300), INT32(65));
#line 300 "../lib/Chai.chpl"
      (_args_forcoforall_fn_chpl)->_10_kw = kw_chpl;
#line 300 "../lib/Chai.chpl"
      chpl_check_nil(_args_forcoforall_fn_chpl, INT64(300), INT32(65));
#line 300 "../lib/Chai.chpl"
      (_args_forcoforall_fn_chpl)->_11_stride_svar = stride_svar_chpl2;
#line 300 "../lib/Chai.chpl"
      chpl_check_nil(_args_forcoforall_fn_chpl, INT64(300), INT32(65));
#line 300 "../lib/Chai.chpl"
      (_args_forcoforall_fn_chpl)->_12_padding_svar = padding_svar_chpl2;
#line 300 "../lib/Chai.chpl"
      tmp_chpl15 = call_tmp_chpl33;
#line 300 "../lib/Chai.chpl"
      chpl_check_nil(_args_forcoforall_fn_chpl, INT64(300), INT32(65));
#line 300 "../lib/Chai.chpl"
      (_args_forcoforall_fn_chpl)->_13_delta = tmp_chpl15;
#line 300 "../lib/Chai.chpl"
      tmp_chpl16 = filters_svar_chpl;
#line 300 "../lib/Chai.chpl"
      chpl_check_nil(_args_forcoforall_fn_chpl, INT64(300), INT32(65));
#line 300 "../lib/Chai.chpl"
      (_args_forcoforall_fn_chpl)->_14_filters_svar = tmp_chpl16;
#line 300 "../lib/Chai.chpl"
      tmp_chpl17 = &dL_dX_chpl;
#line 300 "../lib/Chai.chpl"
      chpl_check_nil(_args_forcoforall_fn_chpl, INT64(300), INT32(65));
#line 300 "../lib/Chai.chpl"
      (_args_forcoforall_fn_chpl)->_15_dL_dX = tmp_chpl17;
#line 300 "../lib/Chai.chpl"
      _args_vforcoforall_fn_chpl = ((void*)(_args_forcoforall_fn_chpl));
#line 300 "../lib/Chai.chpl"
      /*** wrapcoforall_fn_chpl102 ***/ chpl_taskAddCoStmt(chpl_task_getRequestedSubloc(), INT64(101), ((chpl_task_bundle_p)(_args_vforcoforall_fn_chpl)), sizeof(chpl__class_localscoforall_fn_chpl102_object), INT64(300), INT32(65));
#line 300 "../lib/Chai.chpl"
    }
#line 300 "../lib/Chai.chpl"
    error_chpl2 = NULL;
#line 300 "../lib/Chai.chpl"
    _waitEndCount_chpl2(_coforallCount_chpl2, numTasks_chpl, &error_chpl2, INT64(300), INT32(65));
#line 300 "../lib/Chai.chpl"
    if (error_chpl2 != nil) /* ZLINE: 300 ../lib/Chai.chpl */
#line 300 "../lib/Chai.chpl"
    {
#line 300 "../lib/Chai.chpl"
      chpl__delete72(_coforallCount_chpl2, INT64(300), INT32(65));
#line 300 "../lib/Chai.chpl"
      error_chpl = error_chpl2;
#line 300 "../lib/Chai.chpl"
      goto forall_IBB_break_label_chpl;
#line 300 "../lib/Chai.chpl"
    }
#line 300 "../lib/Chai.chpl"
    chpl__delete72(_coforallCount_chpl2, INT64(300), INT32(65));
#line 300 "../lib/Chai.chpl"
    chpl_after_forall_fence();
#line 300 "../lib/Chai.chpl"
    forall_IBB_break_label_chpl:;
#line 300 "../lib/Chai.chpl"
    if (error_chpl != nil) /* ZLINE: 300 ../lib/Chai.chpl */
#line 300 "../lib/Chai.chpl"
    {
#line 300 "../lib/Chai.chpl"
      chpl_propagate_error(error_chpl, INT64(300), INT32(65));
#line 300 "../lib/Chai.chpl"
    }
#line 300 "../lib/Chai.chpl"
    chpl_after_forall_fence();
#line 300 "../lib/Chai.chpl"
    chpl__autoDestroy8(&call_tmp_chpl47, INT64(300), INT32(65));
#line 311 "../lib/Chai.chpl"
    *(coerce_tmp_chpl29 + INT64(0)) = i_chpl;
#line 311 "../lib/Chai.chpl"
    i_i_chpl = &coerce_tmp_chpl29;
#line 311 "../lib/Chai.chpl"
    ret_chpl7 = (newDeltas_chpl)->_instance;
#line 311 "../lib/Chai.chpl"
    checkAccess_chpl10(newDeltas_chpl, i_i_chpl, ret_chpl7, INT64(311), INT32(65));
#line 311 "../lib/Chai.chpl"
    coerce_tmp_chpl30 = ret_chpl7;
#line 311 "../lib/Chai.chpl"
    *(_formal_tmp_in_ind_chpl + INT64(0)) = *(coerce_tmp_chpl29 + INT64(0));
#line 311 "../lib/Chai.chpl"
    _formal_tmp_ind_x0_chpl = *(_formal_tmp_in_ind_chpl + INT64(0));
#line 311 "../lib/Chai.chpl"
    chpl_check_nil(coerce_tmp_chpl30, INT64(311), INT32(65));
#line 311 "../lib/Chai.chpl"
    coerce_tmp_chpl31 = (coerce_tmp_chpl30)->shiftedData;
#line 311 "../lib/Chai.chpl"
    call_tmp_chpl61 = (coerce_tmp_chpl31 + _formal_tmp_ind_x0_chpl);
#line 311 "../lib/Chai.chpl"
    chpl___ASSIGN_24(call_tmp_chpl61, &dL_dX_chpl);
#line 311 "../lib/Chai.chpl"
    _field_destructor_tmp__chpl = (&dL_dX_chpl)->data;
#line 311 "../lib/Chai.chpl"
    chpl__autoDestroy28(&_field_destructor_tmp__chpl, INT64(311), INT32(65));
#line 311 "../lib/Chai.chpl"
    _field_destructor_tmp__chpl2 = (&dL_dX_chpl)->_domain;
#line 311 "../lib/Chai.chpl"
    chpl__autoDestroy8(&_field_destructor_tmp__chpl2, INT64(311), INT32(65));
#line 311 "../lib/Chai.chpl"
    _field_destructor_tmp__chpl3 = (&dL_dF_chpl)->data;
#line 311 "../lib/Chai.chpl"
    chpl__autoDestroy25(&_field_destructor_tmp__chpl3, INT64(311), INT32(65));
#line 311 "../lib/Chai.chpl"
    _field_destructor_tmp__chpl4 = (&dL_dF_chpl)->_domain;
#line 311 "../lib/Chai.chpl"
    chpl__autoDestroy9(&_field_destructor_tmp__chpl4, INT64(311), INT32(65));
#line 311 "../lib/Chai.chpl"
  }
#line 289 "../lib/Chai.chpl"
  tmp_chpl22 = (_ic__F8_i_chpl2 <= _ic__F9_end_chpl2);
#line 289 "../lib/Chai.chpl"
  hasMore_chpl = (tmp_chpl22 != INT64(0));
#line 289 "../lib/Chai.chpl"
  if (hasMore_chpl) /* ZLINE: 289 ../lib/Chai.chpl */
#line 289 "../lib/Chai.chpl"
  {
#line 289 "../lib/Chai.chpl"
    chpl_error("zippered iterations have non-equal lengths", INT64(289), INT32(65));
#line 289 "../lib/Chai.chpl"
  }
#line 289 "../lib/Chai.chpl"
  coerce_tmp_chpl32 = RP_filtersGrad_chpl2;
#line 289 "../lib/Chai.chpl"
  i_x_chpl2 = &AS_filtersGrad_chpl;
#line 289 "../lib/Chai.chpl"
  chpl_check_nil(coerce_tmp_chpl32, INT64(289), INT32(65));
#line 289 "../lib/Chai.chpl"
  call_tmp_chpl62 = &((coerce_tmp_chpl32)->value);
#line 289 "../lib/Chai.chpl"
  chpl___PLUS__ASSIGN_3(call_tmp_chpl62, i_x_chpl2);
#line 289 "../lib/Chai.chpl"
  _field_destructor_tmp__chpl5 = (&AS_filtersGrad_chpl)->data;
#line 289 "../lib/Chai.chpl"
  chpl__autoDestroy25(&_field_destructor_tmp__chpl5, INT64(289), INT32(65));
#line 289 "../lib/Chai.chpl"
  _field_destructor_tmp__chpl6 = (&AS_filtersGrad_chpl)->_domain;
#line 289 "../lib/Chai.chpl"
  chpl__autoDestroy9(&_field_destructor_tmp__chpl6, INT64(289), INT32(65));
#line 289 "../lib/Chai.chpl"
  chpl__reduceCombine3(RP_filtersGrad_chpl, RP_filtersGrad_chpl2, INT64(289), INT32(65));
#line 289 "../lib/Chai.chpl"
  chpl__delete14(RP_filtersGrad_chpl2, INT64(289), INT32(65));
#line 289 "../lib/Chai.chpl"
  return;
#line 289 "../lib/Chai.chpl"
}

#line 300 "../lib/Chai.chpl"
/* Chai.chpl:300 */
#line 300 "../lib/Chai.chpl"
static void wrapcoforall_fn_chpl102(_class_localscoforall_fn_chpl102 c_chpl) {
#line 300 "../lib/Chai.chpl"
  Error_chpl error_chpl = NULL;
#line 300 "../lib/Chai.chpl"
  int64_t _0_numChunks_chpl;
#line 300 "../lib/Chai.chpl"
  int64_t _1_parDim_chpl;
#line 300 "../lib/Chai.chpl"
  DefaultRectangularDom_3_int64_t_one_chpl _2_coerce_tmp_chpl = NULL;
#line 300 "../lib/Chai.chpl"
  chpl___EndCount_AtomicT_int64_t_int64_t _3__coforallCount_chpl = NULL;
#line 300 "../lib/Chai.chpl"
  int64_t _4_chunk_chpl;
#line 300 "../lib/Chai.chpl"
  int64_t _6_outChannels_chpl;
#line 300 "../lib/Chai.chpl"
  int64_t _7_dh_chpl;
#line 300 "../lib/Chai.chpl"
  int64_t _8_dw_chpl;
#line 300 "../lib/Chai.chpl"
  int64_t _9_kh_chpl;
#line 300 "../lib/Chai.chpl"
  int64_t _10_kw_chpl;
#line 300 "../lib/Chai.chpl"
  int64_t _11_stride_svar_chpl;
#line 300 "../lib/Chai.chpl"
  int64_t _12_padding_svar_chpl;
#line 300 "../lib/Chai.chpl"
  _ref_Tensor_3__real64_chpl _13_delta_chpl = NULL;
#line 300 "../lib/Chai.chpl"
  _ref_Tensor_4__real64_chpl _14_filters_svar_chpl = NULL;
#line 300 "../lib/Chai.chpl"
  _ref_Tensor_3__real64_chpl _15_dL_dX_chpl = NULL;
#line 300 "../lib/Chai.chpl"
  chpl_rmem_consist_acquire(INT64(300), INT32(65));
#line 300 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(300), INT32(65));
#line 300 "../lib/Chai.chpl"
  _0_numChunks_chpl = (c_chpl)->_0_numChunks;
#line 300 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(300), INT32(65));
#line 300 "../lib/Chai.chpl"
  _1_parDim_chpl = (c_chpl)->_1_parDim;
#line 300 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(300), INT32(65));
#line 300 "../lib/Chai.chpl"
  _2_coerce_tmp_chpl = (c_chpl)->_2_coerce_tmp;
#line 300 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(300), INT32(65));
#line 300 "../lib/Chai.chpl"
  _3__coforallCount_chpl = (c_chpl)->_3__coforallCount;
#line 300 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(300), INT32(65));
#line 300 "../lib/Chai.chpl"
  _4_chunk_chpl = (c_chpl)->_4_chunk;
#line 300 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(300), INT32(65));
#line 300 "../lib/Chai.chpl"
  _6_outChannels_chpl = (c_chpl)->_6_outChannels;
#line 300 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(300), INT32(65));
#line 300 "../lib/Chai.chpl"
  _7_dh_chpl = (c_chpl)->_7_dh;
#line 300 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(300), INT32(65));
#line 300 "../lib/Chai.chpl"
  _8_dw_chpl = (c_chpl)->_8_dw;
#line 300 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(300), INT32(65));
#line 300 "../lib/Chai.chpl"
  _9_kh_chpl = (c_chpl)->_9_kh;
#line 300 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(300), INT32(65));
#line 300 "../lib/Chai.chpl"
  _10_kw_chpl = (c_chpl)->_10_kw;
#line 300 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(300), INT32(65));
#line 300 "../lib/Chai.chpl"
  _11_stride_svar_chpl = (c_chpl)->_11_stride_svar;
#line 300 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(300), INT32(65));
#line 300 "../lib/Chai.chpl"
  _12_padding_svar_chpl = (c_chpl)->_12_padding_svar;
#line 300 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(300), INT32(65));
#line 300 "../lib/Chai.chpl"
  _13_delta_chpl = (c_chpl)->_13_delta;
#line 300 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(300), INT32(65));
#line 300 "../lib/Chai.chpl"
  _14_filters_svar_chpl = (c_chpl)->_14_filters_svar;
#line 300 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(300), INT32(65));
#line 300 "../lib/Chai.chpl"
  _15_dL_dX_chpl = (c_chpl)->_15_dL_dX;
#line 300 "../lib/Chai.chpl"
  coforall_fn_chpl102(_0_numChunks_chpl, _1_parDim_chpl, _2_coerce_tmp_chpl, _3__coforallCount_chpl, _4_chunk_chpl, &error_chpl, _6_outChannels_chpl, _7_dh_chpl, _8_dw_chpl, _9_kh_chpl, _10_kw_chpl, _11_stride_svar_chpl, _12_padding_svar_chpl, _13_delta_chpl, _14_filters_svar_chpl, _15_dL_dX_chpl);
#line 300 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(300), INT32(65));
#line 300 "../lib/Chai.chpl"
  _downEndCount_chpl((c_chpl)->_3__coforallCount, error_chpl, INT64(300), INT32(65));
#line 300 "../lib/Chai.chpl"
  return;
#line 300 "../lib/Chai.chpl"
}

#line 300 "../lib/Chai.chpl"
/* Chai.chpl:300 */
#line 300 "../lib/Chai.chpl"
static void coforall_fn_chpl102(int64_t numChunks_chpl,
#line 300 "../lib/Chai.chpl"
                                int64_t parDim_chpl,
#line 300 "../lib/Chai.chpl"
                                DefaultRectangularDom_3_int64_t_one_chpl this_chpl31,
#line 300 "../lib/Chai.chpl"
                                chpl___EndCount_AtomicT_int64_t_int64_t _coforallCount_chpl,
#line 300 "../lib/Chai.chpl"
                                int64_t chunk_chpl,
#line 300 "../lib/Chai.chpl"
                                Error_chpl * error_out_chpl,
#line 300 "../lib/Chai.chpl"
                                int64_t outChannels_chpl,
#line 300 "../lib/Chai.chpl"
                                int64_t dh_chpl,
#line 300 "../lib/Chai.chpl"
                                int64_t dw_chpl,
#line 300 "../lib/Chai.chpl"
                                int64_t kh_chpl,
#line 300 "../lib/Chai.chpl"
                                int64_t kw_chpl,
#line 300 "../lib/Chai.chpl"
                                int64_t stride_svar_chpl,
#line 300 "../lib/Chai.chpl"
                                int64_t padding_svar_chpl,
#line 300 "../lib/Chai.chpl"
                                Tensor_3__real64_chpl * delta_chpl,
#line 300 "../lib/Chai.chpl"
                                Tensor_4__real64_chpl * filters_svar_chpl,
#line 300 "../lib/Chai.chpl"
                                Tensor_3__real64_chpl * dL_dX_chpl) {
#line 300 "../lib/Chai.chpl"
  _tuple_3_star_range_int64_t_both_one_chpl ranges_chpl;
#line 300 "../lib/Chai.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl6 = NULL;
#line 300 "../lib/Chai.chpl"
  uint64_t call_tmp_chpl7;
#line 300 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl3;
#line 300 "../lib/Chai.chpl"
  uint64_t call_tmp_chpl8;
#line 300 "../lib/Chai.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl9 = NULL;
#line 300 "../lib/Chai.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl10 = NULL;
#line 300 "../lib/Chai.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl11 = NULL;
#line 300 "../lib/Chai.chpl"
  _tuple_2_star_int64_t_chpl call_tmp_chpl12;
#line 300 "../lib/Chai.chpl"
  _tuple_2_star_int64_t_chpl ret_tmp_chpl;
#line 300 "../lib/Chai.chpl"
  int64_t tmp_x0_chpl;
#line 300 "../lib/Chai.chpl"
  int64_t tmp_x1_chpl;
#line 300 "../lib/Chai.chpl"
  _tuple_3_star_range_int64_t_both_one_chpl block_chpl;
#line 300 "../lib/Chai.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl13 = NULL;
#line 300 "../lib/Chai.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl14 = NULL;
#line 300 "../lib/Chai.chpl"
  _tuple_3_star_range_int64_t_both_one_chpl coerce_tmp_chpl4;
#line 300 "../lib/Chai.chpl"
  _tuple_3_star_range_int64_t_both_one_chpl retcopy_chpl;
#line 300 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl5;
#line 300 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl _ic__F0_this_chpl;
#line 300 "../lib/Chai.chpl"
  int64_t i_chpl;
#line 300 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl this_chpl32;
#line 300 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl this_chpl33;
#line 300 "../lib/Chai.chpl"
  int64_t end_chpl;
#line 300 "../lib/Chai.chpl"
  int64_t i_chpl2;
#line 300 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl6;
#line 300 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl _ic__F0_this_chpl2;
#line 300 "../lib/Chai.chpl"
  int64_t i_chpl3;
#line 300 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl this_chpl34;
#line 300 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl this_chpl35;
#line 300 "../lib/Chai.chpl"
  int64_t end_chpl2;
#line 300 "../lib/Chai.chpl"
  int64_t i_chpl4;
#line 300 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl7;
#line 300 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl _ic__F0_this_chpl3;
#line 300 "../lib/Chai.chpl"
  int64_t i_chpl5;
#line 300 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl this_chpl36;
#line 300 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl this_chpl37;
#line 300 "../lib/Chai.chpl"
  int64_t end_chpl3;
#line 300 "../lib/Chai.chpl"
  _tuple_2_star_int64_t_chpl ret_tmp_chpl2;
#line 300 "../lib/Chai.chpl"
  _tuple_2_star_int64_t_chpl _yieldedIndex_chpl;
#line 300 "../lib/Chai.chpl"
  int64_t j_x0_chpl;
#line 300 "../lib/Chai.chpl"
  int64_t j_x1_chpl;
#line 300 "../lib/Chai.chpl"
  _tuple_3_star_int64_t_chpl ret_tmp_chpl3;
#line 300 "../lib/Chai.chpl"
  _tuple_3_star_int64_t_chpl yret_chpl;
#line 300 "../lib/Chai.chpl"
  int64_t chpl_idx_1_x0;
#line 300 "../lib/Chai.chpl"
  int64_t chpl_idx_1_x1;
#line 300 "../lib/Chai.chpl"
  int64_t chpl_idx_1_x2;
#line 300 "../lib/Chai.chpl"
  int64_t m_chpl;
#line 300 "../lib/Chai.chpl"
  int64_t n_chpl;
#line 300 "../lib/Chai.chpl"
  int64_t ci_chpl;
#line 302 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl15;
#line 302 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl call_tmp_chpl16;
#line 302 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl ret_tmp_chpl4;
#line 302 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl5;
#line 302 "../lib/Chai.chpl"
  SumReduceScanOp__real64_chpl globalRP_sum_chpl = NULL;
#line 302 "../lib/Chai.chpl"
  SumReduceScanOp__real64_chpl new_temp_chpl = NULL;
#line 302 "../lib/Chai.chpl"
  chpl_LocalSpinlock default_arg_l_chpl;
#line 302 "../lib/Chai.chpl"
  chpl_LocalSpinlock ret_chpl;
#line 302 "../lib/Chai.chpl"
  chpl_LocalSpinlock l_chpl;
#line 302 "../lib/Chai.chpl"
  SumReduceScanOp__real64_chpl coerce_tmp_chpl8 = NULL;
#line 302 "../lib/Chai.chpl"
  _ref__real64_chpl call_tmp_chpl17 = NULL;
#line 302 "../lib/Chai.chpl"
  _real64 globalAS_sum_chpl;
#line 302 "../lib/Chai.chpl"
  int64_t dh_chpl2;
#line 302 "../lib/Chai.chpl"
  int64_t dw_chpl2;
#line 302 "../lib/Chai.chpl"
  int64_t kh_chpl2;
#line 302 "../lib/Chai.chpl"
  int64_t kw_chpl2;
#line 302 "../lib/Chai.chpl"
  int64_t m_chpl2;
#line 302 "../lib/Chai.chpl"
  int64_t n_chpl2;
#line 302 "../lib/Chai.chpl"
  int64_t stride_svar_chpl2;
#line 302 "../lib/Chai.chpl"
  int64_t padding_svar_chpl2;
#line 302 "../lib/Chai.chpl"
  int64_t ci_chpl2;
#line 302 "../lib/Chai.chpl"
  int64_t call_tmp_chpl18;
#line 302 "../lib/Chai.chpl"
  int64_t len_chpl;
#line 302 "../lib/Chai.chpl"
  int64_t numChunks_chpl2;
#line 302 "../lib/Chai.chpl"
  int64_t tmp_chpl;
#line 302 "../lib/Chai.chpl"
  int64_t tmp_chpl2;
#line 302 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl tmpIter_chpl;
#line 302 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl call_tmp_chpl19;
#line 302 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl ret_tmp_chpl6;
#line 302 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl7;
#line 302 "../lib/Chai.chpl"
  chpl___EndCount_AtomicT_int64_t_int64_t _coforallCount_chpl2 = NULL;
#line 302 "../lib/Chai.chpl"
  chpl___EndCount_AtomicT_int64_t_int64_t new_temp_chpl2 = NULL;
#line 302 "../lib/Chai.chpl"
  int64_t numTasks_chpl;
#line 302 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl _ic__F0_this_chpl4;
#line 302 "../lib/Chai.chpl"
  int64_t i_chpl6;
#line 302 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl this_chpl38;
#line 302 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl this_chpl39;
#line 302 "../lib/Chai.chpl"
  int64_t end_chpl4;
#line 302 "../lib/Chai.chpl"
  _class_localscoforall_fn_chpl103 _args_forcoforall_fn_chpl = NULL;
#line 302 "../lib/Chai.chpl"
  _ref_Tensor_3__real64_chpl tmp_chpl3 = NULL;
#line 302 "../lib/Chai.chpl"
  _ref_Tensor_4__real64_chpl tmp_chpl4 = NULL;
#line 302 "../lib/Chai.chpl"
  raw_c_void_ptr _args_vforcoforall_fn_chpl;
#line 302 "../lib/Chai.chpl"
  Error_chpl error_chpl = NULL;
#line 302 "../lib/Chai.chpl"
  SumReduceScanOp__real64_chpl coerce_tmp_chpl9 = NULL;
#line 302 "../lib/Chai.chpl"
  _ref__real64_chpl call_tmp_chpl20 = NULL;
#line 302 "../lib/Chai.chpl"
  SumReduceScanOp__real64_chpl coerce_tmp_chpl10 = NULL;
#line 302 "../lib/Chai.chpl"
  _real64 ret_chpl2;
#line 309 "../lib/Chai.chpl"
  _ref__array_DefaultRectangularArr_3_int64_t_one__real64_int64_t_chpl call_tmp_chpl21 = NULL;
#line 309 "../lib/Chai.chpl"
  _tuple_3_star_int64_t_chpl coerce_tmp_chpl11;
#line 309 "../lib/Chai.chpl"
  _ref__tuple_3_star_int64_t_chpl i_i_chpl = NULL;
#line 309 "../lib/Chai.chpl"
  DefaultRectangularArr_3_int64_t_one__real64_int64_t_chpl ret_chpl3 = NULL;
#line 309 "../lib/Chai.chpl"
  DefaultRectangularArr_3_int64_t_one__real64_int64_t_chpl coerce_tmp_chpl12 = NULL;
#line 309 "../lib/Chai.chpl"
  _tuple_3_star_int64_t_chpl _formal_tmp_in_ind_chpl;
#line 309 "../lib/Chai.chpl"
  int64_t _formal_tmp_ind_x0_chpl;
#line 309 "../lib/Chai.chpl"
  int64_t _formal_tmp_ind_x1_chpl;
#line 309 "../lib/Chai.chpl"
  int64_t _formal_tmp_ind_x2_chpl;
#line 309 "../lib/Chai.chpl"
  int64_t ret_chpl4;
#line 309 "../lib/Chai.chpl"
  int64_t sum_chpl;
#line 309 "../lib/Chai.chpl"
  _ref__tuple_3_star_int64_t_chpl call_tmp_chpl22 = NULL;
#line 309 "../lib/Chai.chpl"
  _ref_int64_t_chpl i_lhs_chpl = NULL;
#line 309 "../lib/Chai.chpl"
  _ref__tuple_3_star_int64_t_chpl call_tmp_chpl23 = NULL;
#line 309 "../lib/Chai.chpl"
  _ref_int64_t_chpl i_lhs_chpl2 = NULL;
#line 309 "../lib/Chai.chpl"
  _ref_int64_t_chpl i_lhs_chpl3 = NULL;
#line 309 "../lib/Chai.chpl"
  _ddata__real64_chpl coerce_tmp_chpl13 = NULL;
#line 309 "../lib/Chai.chpl"
  _ref__real64_chpl call_tmp_chpl24 = NULL;
#line 309 "../lib/Chai.chpl"
  int64_t optinfo_chpl;
#line 309 "../lib/Chai.chpl"
  chpl__class_localscoforall_fn_chpl103_object chpl_macro_tmp_108;
#line 300 "../lib/Chai.chpl"
  chpl_check_nil(this_chpl31, INT64(300), INT32(65));
#line 300 "../lib/Chai.chpl"
  *(ranges_chpl + INT64(0)) = *((this_chpl31)->ranges + INT64(0));
#line 300 "../lib/Chai.chpl"
  *(ranges_chpl + INT64(1)) = *((this_chpl31)->ranges + INT64(1));
#line 300 "../lib/Chai.chpl"
  *(ranges_chpl + INT64(2)) = *((this_chpl31)->ranges + INT64(2));
#line 300 "../lib/Chai.chpl"
  call_tmp_chpl6 = this_chpl24(&ranges_chpl, parDim_chpl, INT64(300), INT32(65));
#line 300 "../lib/Chai.chpl"
  coerce_tmp_chpl3 = *(call_tmp_chpl6);
#line 300 "../lib/Chai.chpl"
  call_tmp_chpl7 = sizeAs_chpl36(coerce_tmp_chpl3, INT64(300), INT32(65));
#line 300 "../lib/Chai.chpl"
  this_chpl24(&ranges_chpl, parDim_chpl, INT64(300), INT32(65));
#line 300 "../lib/Chai.chpl"
  call_tmp_chpl8 = (call_tmp_chpl7 * UINT64(1));
#line 300 "../lib/Chai.chpl"
  call_tmp_chpl9 = this_chpl24(&ranges_chpl, parDim_chpl, INT64(300), INT32(65));
#line 300 "../lib/Chai.chpl"
  call_tmp_chpl10 = this_chpl24(&ranges_chpl, parDim_chpl, INT64(300), INT32(65));
#line 300 "../lib/Chai.chpl"
  call_tmp_chpl11 = this_chpl24(&ranges_chpl, parDim_chpl, INT64(300), INT32(65));
#line 300 "../lib/Chai.chpl"
  _computeBlock_chpl2(call_tmp_chpl8, numChunks_chpl, chunk_chpl, (call_tmp_chpl9)->_high, (call_tmp_chpl10)->_low, (call_tmp_chpl11)->_low, &ret_tmp_chpl, INT64(300), INT32(65));
#line 300 "../lib/Chai.chpl"
  *(call_tmp_chpl12 + INT64(0)) = *(ret_tmp_chpl + INT64(0));
#line 300 "../lib/Chai.chpl"
  *(call_tmp_chpl12 + INT64(1)) = *(ret_tmp_chpl + INT64(1));
#line 300 "../lib/Chai.chpl"
  tmp_x0_chpl = *(call_tmp_chpl12 + INT64(0));
#line 300 "../lib/Chai.chpl"
  tmp_x1_chpl = *(call_tmp_chpl12 + INT64(1));
#line 300 "../lib/Chai.chpl"
  *(block_chpl + INT64(0)) = *(ranges_chpl + INT64(0));
#line 300 "../lib/Chai.chpl"
  *(block_chpl + INT64(1)) = *(ranges_chpl + INT64(1));
#line 300 "../lib/Chai.chpl"
  *(block_chpl + INT64(2)) = *(ranges_chpl + INT64(2));
#line 300 "../lib/Chai.chpl"
  call_tmp_chpl13 = this_chpl24(&block_chpl, parDim_chpl, INT64(300), INT32(65));
#line 300 "../lib/Chai.chpl"
  (call_tmp_chpl13)->_low = tmp_x0_chpl;
#line 300 "../lib/Chai.chpl"
  call_tmp_chpl14 = this_chpl24(&block_chpl, parDim_chpl, INT64(300), INT32(65));
#line 300 "../lib/Chai.chpl"
  (call_tmp_chpl14)->_high = tmp_x1_chpl;
#line 300 "../lib/Chai.chpl"
  *(retcopy_chpl + INT64(0)) = *(block_chpl + INT64(0));
#line 300 "../lib/Chai.chpl"
  *(retcopy_chpl + INT64(1)) = *(block_chpl + INT64(1));
#line 300 "../lib/Chai.chpl"
  *(retcopy_chpl + INT64(2)) = *(block_chpl + INT64(2));
#line 300 "../lib/Chai.chpl"
  *(coerce_tmp_chpl4 + INT64(0)) = *(retcopy_chpl + INT64(0));
#line 300 "../lib/Chai.chpl"
  *(coerce_tmp_chpl4 + INT64(1)) = *(retcopy_chpl + INT64(1));
#line 300 "../lib/Chai.chpl"
  *(coerce_tmp_chpl4 + INT64(2)) = *(retcopy_chpl + INT64(2));
#line 300 "../lib/Chai.chpl"
  coerce_tmp_chpl5 = *(coerce_tmp_chpl4 + INT64(0));
#line 300 "../lib/Chai.chpl"
  _ic__F0_this_chpl = coerce_tmp_chpl5;
#line 300 "../lib/Chai.chpl"
  checkIfIterWillOverflow_chpl(coerce_tmp_chpl5, UINT8(true), INT64(300), INT32(65));
#line 300 "../lib/Chai.chpl"
  i_chpl = INT64(0);
#line 300 "../lib/Chai.chpl"
  this_chpl32 = _ic__F0_this_chpl;
#line 300 "../lib/Chai.chpl"
  this_chpl33 = _ic__F0_this_chpl;
#line 300 "../lib/Chai.chpl"
  end_chpl = (&this_chpl33)->_high;
#line 300 "../lib/Chai.chpl"
  for (i_chpl = (&this_chpl32)->_low; ((i_chpl <= end_chpl)); i_chpl += INT64(1)) {
#line 300 "../lib/Chai.chpl"
    i_chpl2 = i_chpl;
#line 300 "../lib/Chai.chpl"
    coerce_tmp_chpl6 = *(coerce_tmp_chpl4 + INT64(1));
#line 300 "../lib/Chai.chpl"
    _ic__F0_this_chpl2 = coerce_tmp_chpl6;
#line 300 "../lib/Chai.chpl"
    checkIfIterWillOverflow_chpl(coerce_tmp_chpl6, UINT8(true), INT64(300), INT32(65));
#line 300 "../lib/Chai.chpl"
    i_chpl3 = INT64(0);
#line 300 "../lib/Chai.chpl"
    this_chpl34 = _ic__F0_this_chpl2;
#line 300 "../lib/Chai.chpl"
    this_chpl35 = _ic__F0_this_chpl2;
#line 300 "../lib/Chai.chpl"
    end_chpl2 = (&this_chpl35)->_high;
#line 300 "../lib/Chai.chpl"
    for (i_chpl3 = (&this_chpl34)->_low; ((i_chpl3 <= end_chpl2)); i_chpl3 += INT64(1)) {
#line 300 "../lib/Chai.chpl"
      i_chpl4 = i_chpl3;
#line 300 "../lib/Chai.chpl"
      coerce_tmp_chpl7 = *(coerce_tmp_chpl4 + INT64(2));
#line 300 "../lib/Chai.chpl"
      _ic__F0_this_chpl3 = coerce_tmp_chpl7;
#line 300 "../lib/Chai.chpl"
      checkIfIterWillOverflow_chpl(coerce_tmp_chpl7, UINT8(true), INT64(300), INT32(65));
#line 300 "../lib/Chai.chpl"
      i_chpl5 = INT64(0);
#line 300 "../lib/Chai.chpl"
      this_chpl36 = _ic__F0_this_chpl3;
#line 300 "../lib/Chai.chpl"
      this_chpl37 = _ic__F0_this_chpl3;
#line 300 "../lib/Chai.chpl"
      end_chpl3 = (&this_chpl37)->_high;
#line 300 "../lib/Chai.chpl"
      for (i_chpl5 = (&this_chpl36)->_low; ((i_chpl5 <= end_chpl3)); i_chpl5 += INT64(1)) {
#line 300 "../lib/Chai.chpl"
        *(ret_tmp_chpl2 + INT64(0)) = i_chpl4;
#line 300 "../lib/Chai.chpl"
        *(ret_tmp_chpl2 + INT64(1)) = i_chpl5;
#line 300 "../lib/Chai.chpl"
        *(_yieldedIndex_chpl + INT64(0)) = *(ret_tmp_chpl2 + INT64(0));
#line 300 "../lib/Chai.chpl"
        *(_yieldedIndex_chpl + INT64(1)) = *(ret_tmp_chpl2 + INT64(1));
#line 300 "../lib/Chai.chpl"
        j_x0_chpl = *(_yieldedIndex_chpl + INT64(0));
#line 300 "../lib/Chai.chpl"
        j_x1_chpl = *(_yieldedIndex_chpl + INT64(1));
#line 300 "../lib/Chai.chpl"
        *(ret_tmp_chpl3 + INT64(0)) = i_chpl2;
#line 300 "../lib/Chai.chpl"
        *(ret_tmp_chpl3 + INT64(1)) = j_x0_chpl;
#line 300 "../lib/Chai.chpl"
        *(ret_tmp_chpl3 + INT64(2)) = j_x1_chpl;
#line 300 "../lib/Chai.chpl"
        *(yret_chpl + INT64(0)) = *(ret_tmp_chpl3 + INT64(0));
#line 300 "../lib/Chai.chpl"
        *(yret_chpl + INT64(1)) = *(ret_tmp_chpl3 + INT64(1));
#line 300 "../lib/Chai.chpl"
        *(yret_chpl + INT64(2)) = *(ret_tmp_chpl3 + INT64(2));
#line 300 "../lib/Chai.chpl"
        chpl_idx_1_x0 = *(yret_chpl + INT64(0));
#line 300 "../lib/Chai.chpl"
        chpl_idx_1_x1 = *(yret_chpl + INT64(1));
#line 300 "../lib/Chai.chpl"
        chpl_idx_1_x2 = *(yret_chpl + INT64(2));
#line 300 "../lib/Chai.chpl"
        m_chpl = chpl_idx_1_x0;
#line 300 "../lib/Chai.chpl"
        n_chpl = chpl_idx_1_x1;
#line 300 "../lib/Chai.chpl"
        ci_chpl = chpl_idx_1_x2;
#line 302 "../lib/Chai.chpl"
        chpl_build_low_bounded_range2(INT64(0), &ret_tmp_chpl4);
#line 302 "../lib/Chai.chpl"
        call_tmp_chpl16 = ret_tmp_chpl4;
#line 302 "../lib/Chai.chpl"
        chpl___POUND_3(call_tmp_chpl16, outChannels_chpl, &ret_tmp_chpl5, INT64(302), INT32(65));
#line 302 "../lib/Chai.chpl"
        call_tmp_chpl15 = ret_tmp_chpl5;
#line 302 "../lib/Chai.chpl"
        init_chpl105(&l_chpl);
#line 302 "../lib/Chai.chpl"
        ret_chpl = l_chpl;
#line 302 "../lib/Chai.chpl"
        default_arg_l_chpl = ret_chpl;
#line 302 "../lib/Chai.chpl"
        new_temp_chpl = _new_chpl23(&default_arg_l_chpl, REAL64(0x0p+0), INT64(302), INT32(65));
#line 302 "../lib/Chai.chpl"
        globalRP_sum_chpl = new_temp_chpl;
#line 302 "../lib/Chai.chpl"
        coerce_tmp_chpl8 = globalRP_sum_chpl;
#line 302 "../lib/Chai.chpl"
        chpl_check_nil(coerce_tmp_chpl8, INT64(302), INT32(65));
#line 302 "../lib/Chai.chpl"
        call_tmp_chpl17 = &((coerce_tmp_chpl8)->value);
#line 302 "../lib/Chai.chpl"
        *(call_tmp_chpl17) += REAL64(0x0p+0);
#line 302 "../lib/Chai.chpl"
        globalAS_sum_chpl = REAL64(0x0p+0);
#line 302 "../lib/Chai.chpl"
        dh_chpl2 = dh_chpl;
#line 302 "../lib/Chai.chpl"
        dw_chpl2 = dw_chpl;
#line 302 "../lib/Chai.chpl"
        kh_chpl2 = kh_chpl;
#line 302 "../lib/Chai.chpl"
        kw_chpl2 = kw_chpl;
#line 302 "../lib/Chai.chpl"
        m_chpl2 = chpl_idx_1_x0;
#line 302 "../lib/Chai.chpl"
        n_chpl2 = chpl_idx_1_x1;
#line 302 "../lib/Chai.chpl"
        stride_svar_chpl2 = stride_svar_chpl;
#line 302 "../lib/Chai.chpl"
        padding_svar_chpl2 = padding_svar_chpl;
#line 302 "../lib/Chai.chpl"
        ci_chpl2 = chpl_idx_1_x2;
#line 302 "../lib/Chai.chpl"
        call_tmp_chpl18 = sizeAs_chpl37(call_tmp_chpl15, INT64(302), INT32(65));
#line 302 "../lib/Chai.chpl"
        len_chpl = call_tmp_chpl18;
#line 302 "../lib/Chai.chpl"
        if (chpl_task_getSerial()) /* ZLINE: 302 ../lib/Chai.chpl */
#line 302 "../lib/Chai.chpl"
        {
#line 302 "../lib/Chai.chpl"
          tmp_chpl = INT64(1);
#line 302 "../lib/Chai.chpl"
        } else /* ZLINE: 302 ../lib/Chai.chpl */
#line 302 "../lib/Chai.chpl"
        {
#line 302 "../lib/Chai.chpl"
          tmp_chpl2 = _computeNumChunks_chpl3(call_tmp_chpl18, INT64(302), INT32(65));
#line 302 "../lib/Chai.chpl"
          tmp_chpl = tmp_chpl2;
#line 302 "../lib/Chai.chpl"
        }
#line 302 "../lib/Chai.chpl"
        numChunks_chpl2 = tmp_chpl;
#line 302 "../lib/Chai.chpl"
        chpl_build_low_bounded_range2(INT64(0), &ret_tmp_chpl6);
#line 302 "../lib/Chai.chpl"
        call_tmp_chpl19 = ret_tmp_chpl6;
#line 302 "../lib/Chai.chpl"
        chpl___POUND_3(call_tmp_chpl19, tmp_chpl, &ret_tmp_chpl7, INT64(302), INT32(65));
#line 302 "../lib/Chai.chpl"
        tmpIter_chpl = ret_tmp_chpl7;
#line 302 "../lib/Chai.chpl"
        new_temp_chpl2 = _new_chpl17(INT64(302), INT32(65));
#line 302 "../lib/Chai.chpl"
        _coforallCount_chpl2 = new_temp_chpl2;
#line 302 "../lib/Chai.chpl"
        numTasks_chpl = chpl_boundedCoforallSize(tmpIter_chpl, INT64(302), INT32(65));
#line 302 "../lib/Chai.chpl"
        _upEndCount_chpl(_coforallCount_chpl2, numTasks_chpl, INT64(302), INT32(65));
#line 302 "../lib/Chai.chpl"
        chpl_resetTaskSpawn(numTasks_chpl, INT64(302), INT32(65));
#line 302 "../lib/Chai.chpl"
        _ic__F0_this_chpl4 = tmpIter_chpl;
#line 302 "../lib/Chai.chpl"
        checkIfIterWillOverflow_chpl(tmpIter_chpl, UINT8(true), INT64(302), INT32(65));
#line 302 "../lib/Chai.chpl"
        i_chpl6 = INT64(0);
#line 302 "../lib/Chai.chpl"
        this_chpl38 = _ic__F0_this_chpl4;
#line 302 "../lib/Chai.chpl"
        this_chpl39 = _ic__F0_this_chpl4;
#line 302 "../lib/Chai.chpl"
        end_chpl4 = (&this_chpl39)->_high;
#line 302 "../lib/Chai.chpl"
        for (i_chpl6 = (&this_chpl38)->_low; ((i_chpl6 <= end_chpl4)); i_chpl6 += INT64(1)) {
#line 302 "../lib/Chai.chpl"
          _args_forcoforall_fn_chpl = ((_class_localscoforall_fn_chpl103)(&chpl_macro_tmp_108));
#line 302 "../lib/Chai.chpl"
          chpl_check_nil(_args_forcoforall_fn_chpl, INT64(302), INT32(65));
#line 302 "../lib/Chai.chpl"
          (_args_forcoforall_fn_chpl)->_0_len = len_chpl;
#line 302 "../lib/Chai.chpl"
          chpl_check_nil(_args_forcoforall_fn_chpl, INT64(302), INT32(65));
#line 302 "../lib/Chai.chpl"
          (_args_forcoforall_fn_chpl)->_1_numChunks = numChunks_chpl2;
#line 302 "../lib/Chai.chpl"
          chpl_check_nil(_args_forcoforall_fn_chpl, INT64(302), INT32(65));
#line 302 "../lib/Chai.chpl"
          (_args_forcoforall_fn_chpl)->_2_call_tmp = call_tmp_chpl15;
#line 302 "../lib/Chai.chpl"
          chpl_check_nil(_args_forcoforall_fn_chpl, INT64(302), INT32(65));
#line 302 "../lib/Chai.chpl"
          (_args_forcoforall_fn_chpl)->_3__coforallCount = _coforallCount_chpl2;
#line 302 "../lib/Chai.chpl"
          chpl_check_nil(_args_forcoforall_fn_chpl, INT64(302), INT32(65));
#line 302 "../lib/Chai.chpl"
          (_args_forcoforall_fn_chpl)->_4_chunk = i_chpl6;
#line 302 "../lib/Chai.chpl"
          chpl_check_nil(_args_forcoforall_fn_chpl, INT64(302), INT32(65));
#line 302 "../lib/Chai.chpl"
          (_args_forcoforall_fn_chpl)->_5_dh = dh_chpl2;
#line 302 "../lib/Chai.chpl"
          chpl_check_nil(_args_forcoforall_fn_chpl, INT64(302), INT32(65));
#line 302 "../lib/Chai.chpl"
          (_args_forcoforall_fn_chpl)->_6_dw = dw_chpl2;
#line 302 "../lib/Chai.chpl"
          chpl_check_nil(_args_forcoforall_fn_chpl, INT64(302), INT32(65));
#line 302 "../lib/Chai.chpl"
          (_args_forcoforall_fn_chpl)->_7_kh = kh_chpl2;
#line 302 "../lib/Chai.chpl"
          chpl_check_nil(_args_forcoforall_fn_chpl, INT64(302), INT32(65));
#line 302 "../lib/Chai.chpl"
          (_args_forcoforall_fn_chpl)->_8_kw = kw_chpl2;
#line 302 "../lib/Chai.chpl"
          chpl_check_nil(_args_forcoforall_fn_chpl, INT64(302), INT32(65));
#line 302 "../lib/Chai.chpl"
          (_args_forcoforall_fn_chpl)->_9_m = m_chpl2;
#line 302 "../lib/Chai.chpl"
          chpl_check_nil(_args_forcoforall_fn_chpl, INT64(302), INT32(65));
#line 302 "../lib/Chai.chpl"
          (_args_forcoforall_fn_chpl)->_10_n = n_chpl2;
#line 302 "../lib/Chai.chpl"
          chpl_check_nil(_args_forcoforall_fn_chpl, INT64(302), INT32(65));
#line 302 "../lib/Chai.chpl"
          (_args_forcoforall_fn_chpl)->_11_stride_svar = stride_svar_chpl2;
#line 302 "../lib/Chai.chpl"
          chpl_check_nil(_args_forcoforall_fn_chpl, INT64(302), INT32(65));
#line 302 "../lib/Chai.chpl"
          (_args_forcoforall_fn_chpl)->_12_padding_svar = padding_svar_chpl2;
#line 302 "../lib/Chai.chpl"
          tmp_chpl3 = delta_chpl;
#line 302 "../lib/Chai.chpl"
          chpl_check_nil(_args_forcoforall_fn_chpl, INT64(302), INT32(65));
#line 302 "../lib/Chai.chpl"
          (_args_forcoforall_fn_chpl)->_13_delta = tmp_chpl3;
#line 302 "../lib/Chai.chpl"
          tmp_chpl4 = filters_svar_chpl;
#line 302 "../lib/Chai.chpl"
          chpl_check_nil(_args_forcoforall_fn_chpl, INT64(302), INT32(65));
#line 302 "../lib/Chai.chpl"
          (_args_forcoforall_fn_chpl)->_14_filters_svar = tmp_chpl4;
#line 302 "../lib/Chai.chpl"
          chpl_check_nil(_args_forcoforall_fn_chpl, INT64(302), INT32(65));
#line 302 "../lib/Chai.chpl"
          (_args_forcoforall_fn_chpl)->_15_ci = ci_chpl2;
#line 302 "../lib/Chai.chpl"
          chpl_check_nil(_args_forcoforall_fn_chpl, INT64(302), INT32(65));
#line 302 "../lib/Chai.chpl"
          (_args_forcoforall_fn_chpl)->_16_globalRP_sum = globalRP_sum_chpl;
#line 302 "../lib/Chai.chpl"
          _args_vforcoforall_fn_chpl = ((void*)(_args_forcoforall_fn_chpl));
#line 302 "../lib/Chai.chpl"
          /*** wrapcoforall_fn_chpl103 ***/ chpl_taskAddCoStmt(chpl_task_getRequestedSubloc(), INT64(102), ((chpl_task_bundle_p)(_args_vforcoforall_fn_chpl)), sizeof(chpl__class_localscoforall_fn_chpl103_object), INT64(302), INT32(65));
#line 302 "../lib/Chai.chpl"
        }
#line 302 "../lib/Chai.chpl"
        error_chpl = NULL;
#line 302 "../lib/Chai.chpl"
        _waitEndCount_chpl2(_coforallCount_chpl2, numTasks_chpl, &error_chpl, INT64(302), INT32(65));
#line 302 "../lib/Chai.chpl"
        if (error_chpl != nil) /* ZLINE: 302 ../lib/Chai.chpl */
#line 302 "../lib/Chai.chpl"
        {
#line 302 "../lib/Chai.chpl"
          chpl_propagate_error(error_chpl, INT64(302), INT32(65));
#line 302 "../lib/Chai.chpl"
        }
#line 302 "../lib/Chai.chpl"
        chpl__delete72(_coforallCount_chpl2, INT64(302), INT32(65));
#line 302 "../lib/Chai.chpl"
        chpl_after_forall_fence();
#line 302 "../lib/Chai.chpl"
        coerce_tmp_chpl9 = globalRP_sum_chpl;
#line 302 "../lib/Chai.chpl"
        chpl_check_nil(coerce_tmp_chpl9, INT64(302), INT32(65));
#line 302 "../lib/Chai.chpl"
        call_tmp_chpl20 = &((coerce_tmp_chpl9)->value);
#line 302 "../lib/Chai.chpl"
        *(call_tmp_chpl20) += globalAS_sum_chpl;
#line 302 "../lib/Chai.chpl"
        coerce_tmp_chpl10 = globalRP_sum_chpl;
#line 302 "../lib/Chai.chpl"
        chpl_check_nil(coerce_tmp_chpl10, INT64(302), INT32(65));
#line 302 "../lib/Chai.chpl"
        ret_chpl2 = (coerce_tmp_chpl10)->value;
#line 302 "../lib/Chai.chpl"
        chpl__delete33(globalRP_sum_chpl, INT64(302), INT32(65));
#line 309 "../lib/Chai.chpl"
        call_tmp_chpl21 = &((dL_dX_chpl)->data);
#line 309 "../lib/Chai.chpl"
        *(coerce_tmp_chpl11 + INT64(0)) = m_chpl;
#line 309 "../lib/Chai.chpl"
        *(coerce_tmp_chpl11 + INT64(1)) = n_chpl;
#line 309 "../lib/Chai.chpl"
        *(coerce_tmp_chpl11 + INT64(2)) = ci_chpl;
#line 309 "../lib/Chai.chpl"
        i_i_chpl = &coerce_tmp_chpl11;
#line 309 "../lib/Chai.chpl"
        ret_chpl3 = (call_tmp_chpl21)->_instance;
#line 309 "../lib/Chai.chpl"
        checkAccess_chpl9(call_tmp_chpl21, i_i_chpl, ret_chpl3, INT64(309), INT32(65));
#line 309 "../lib/Chai.chpl"
        coerce_tmp_chpl12 = ret_chpl3;
#line 309 "../lib/Chai.chpl"
        *(_formal_tmp_in_ind_chpl + INT64(0)) = *(coerce_tmp_chpl11 + INT64(0));
#line 309 "../lib/Chai.chpl"
        *(_formal_tmp_in_ind_chpl + INT64(1)) = *(coerce_tmp_chpl11 + INT64(1));
#line 309 "../lib/Chai.chpl"
        *(_formal_tmp_in_ind_chpl + INT64(2)) = *(coerce_tmp_chpl11 + INT64(2));
#line 309 "../lib/Chai.chpl"
        _formal_tmp_ind_x0_chpl = *(_formal_tmp_in_ind_chpl + INT64(0));
#line 309 "../lib/Chai.chpl"
        _formal_tmp_ind_x1_chpl = *(_formal_tmp_in_ind_chpl + INT64(1));
#line 309 "../lib/Chai.chpl"
        _formal_tmp_ind_x2_chpl = *(_formal_tmp_in_ind_chpl + INT64(2));
#line 309 "../lib/Chai.chpl"
        sum_chpl = INT64(0);
#line 309 "../lib/Chai.chpl"
        chpl_check_nil(coerce_tmp_chpl12, INT64(309), INT32(65));
#line 309 "../lib/Chai.chpl"
        call_tmp_chpl22 = &((coerce_tmp_chpl12)->blk);
#line 309 "../lib/Chai.chpl"
        i_lhs_chpl = &sum_chpl;
#line 309 "../lib/Chai.chpl"
        *(i_lhs_chpl) += ((int64_t)((_formal_tmp_ind_x0_chpl * *(*(call_tmp_chpl22) + INT64(0)))));
#line 309 "../lib/Chai.chpl"
        chpl_check_nil(coerce_tmp_chpl12, INT64(309), INT32(65));
#line 309 "../lib/Chai.chpl"
        call_tmp_chpl23 = &((coerce_tmp_chpl12)->blk);
#line 309 "../lib/Chai.chpl"
        i_lhs_chpl2 = &sum_chpl;
#line 309 "../lib/Chai.chpl"
        *(i_lhs_chpl2) += ((int64_t)((_formal_tmp_ind_x1_chpl * *(*(call_tmp_chpl23) + INT64(1)))));
#line 309 "../lib/Chai.chpl"
        i_lhs_chpl3 = &sum_chpl;
#line 309 "../lib/Chai.chpl"
        *(i_lhs_chpl3) += _formal_tmp_ind_x2_chpl;
#line 309 "../lib/Chai.chpl"
        ret_chpl4 = sum_chpl;
#line 309 "../lib/Chai.chpl"
        chpl_check_nil(coerce_tmp_chpl12, INT64(309), INT32(65));
#line 309 "../lib/Chai.chpl"
        coerce_tmp_chpl13 = (coerce_tmp_chpl12)->shiftedData;
#line 309 "../lib/Chai.chpl"
        call_tmp_chpl24 = (coerce_tmp_chpl13 + ret_chpl4);
#line 309 "../lib/Chai.chpl"
        *(call_tmp_chpl24) = ret_chpl2;
#line 309 "../lib/Chai.chpl"
      }
#line 309 "../lib/Chai.chpl"
    }
#line 309 "../lib/Chai.chpl"
  }
#line 300 "../lib/Chai.chpl"
  return;
#line 300 "../lib/Chai.chpl"
}

#line 302 "../lib/Chai.chpl"
/* Chai.chpl:302 */
#line 302 "../lib/Chai.chpl"
static void wrapcoforall_fn_chpl103(_class_localscoforall_fn_chpl103 c_chpl) {
#line 302 "../lib/Chai.chpl"
  int64_t _0_len_chpl;
#line 302 "../lib/Chai.chpl"
  int64_t _1_numChunks_chpl;
#line 302 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl _2_call_tmp_chpl;
#line 302 "../lib/Chai.chpl"
  chpl___EndCount_AtomicT_int64_t_int64_t _3__coforallCount_chpl = NULL;
#line 302 "../lib/Chai.chpl"
  int64_t _4_chunk_chpl;
#line 302 "../lib/Chai.chpl"
  int64_t _5_dh_chpl;
#line 302 "../lib/Chai.chpl"
  int64_t _6_dw_chpl;
#line 302 "../lib/Chai.chpl"
  int64_t _7_kh_chpl;
#line 302 "../lib/Chai.chpl"
  int64_t _8_kw_chpl;
#line 302 "../lib/Chai.chpl"
  int64_t _9_m_chpl;
#line 302 "../lib/Chai.chpl"
  int64_t _10_n_chpl;
#line 302 "../lib/Chai.chpl"
  int64_t _11_stride_svar_chpl;
#line 302 "../lib/Chai.chpl"
  int64_t _12_padding_svar_chpl;
#line 302 "../lib/Chai.chpl"
  _ref_Tensor_3__real64_chpl _13_delta_chpl = NULL;
#line 302 "../lib/Chai.chpl"
  _ref_Tensor_4__real64_chpl _14_filters_svar_chpl = NULL;
#line 302 "../lib/Chai.chpl"
  int64_t _15_ci_chpl;
#line 302 "../lib/Chai.chpl"
  chpl_rmem_consist_acquire(INT64(302), INT32(65));
#line 302 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(302), INT32(65));
#line 302 "../lib/Chai.chpl"
  _0_len_chpl = (c_chpl)->_0_len;
#line 302 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(302), INT32(65));
#line 302 "../lib/Chai.chpl"
  _1_numChunks_chpl = (c_chpl)->_1_numChunks;
#line 302 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(302), INT32(65));
#line 302 "../lib/Chai.chpl"
  _2_call_tmp_chpl = (c_chpl)->_2_call_tmp;
#line 302 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(302), INT32(65));
#line 302 "../lib/Chai.chpl"
  _3__coforallCount_chpl = (c_chpl)->_3__coforallCount;
#line 302 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(302), INT32(65));
#line 302 "../lib/Chai.chpl"
  _4_chunk_chpl = (c_chpl)->_4_chunk;
#line 302 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(302), INT32(65));
#line 302 "../lib/Chai.chpl"
  _5_dh_chpl = (c_chpl)->_5_dh;
#line 302 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(302), INT32(65));
#line 302 "../lib/Chai.chpl"
  _6_dw_chpl = (c_chpl)->_6_dw;
#line 302 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(302), INT32(65));
#line 302 "../lib/Chai.chpl"
  _7_kh_chpl = (c_chpl)->_7_kh;
#line 302 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(302), INT32(65));
#line 302 "../lib/Chai.chpl"
  _8_kw_chpl = (c_chpl)->_8_kw;
#line 302 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(302), INT32(65));
#line 302 "../lib/Chai.chpl"
  _9_m_chpl = (c_chpl)->_9_m;
#line 302 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(302), INT32(65));
#line 302 "../lib/Chai.chpl"
  _10_n_chpl = (c_chpl)->_10_n;
#line 302 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(302), INT32(65));
#line 302 "../lib/Chai.chpl"
  _11_stride_svar_chpl = (c_chpl)->_11_stride_svar;
#line 302 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(302), INT32(65));
#line 302 "../lib/Chai.chpl"
  _12_padding_svar_chpl = (c_chpl)->_12_padding_svar;
#line 302 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(302), INT32(65));
#line 302 "../lib/Chai.chpl"
  _13_delta_chpl = (c_chpl)->_13_delta;
#line 302 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(302), INT32(65));
#line 302 "../lib/Chai.chpl"
  _14_filters_svar_chpl = (c_chpl)->_14_filters_svar;
#line 302 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(302), INT32(65));
#line 302 "../lib/Chai.chpl"
  _15_ci_chpl = (c_chpl)->_15_ci;
#line 302 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(302), INT32(65));
#line 302 "../lib/Chai.chpl"
  coforall_fn_chpl103(_0_len_chpl, _1_numChunks_chpl, _2_call_tmp_chpl, _3__coforallCount_chpl, _4_chunk_chpl, _5_dh_chpl, _6_dw_chpl, _7_kh_chpl, _8_kw_chpl, _9_m_chpl, _10_n_chpl, _11_stride_svar_chpl, _12_padding_svar_chpl, _13_delta_chpl, _14_filters_svar_chpl, _15_ci_chpl, (c_chpl)->_16_globalRP_sum);
#line 302 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(302), INT32(65));
#line 302 "../lib/Chai.chpl"
  _downEndCount_chpl((c_chpl)->_3__coforallCount, nil, INT64(302), INT32(65));
#line 302 "../lib/Chai.chpl"
  return;
#line 302 "../lib/Chai.chpl"
}

#line 302 "../lib/Chai.chpl"
/* Chai.chpl:302 */
#line 302 "../lib/Chai.chpl"
static void coforall_fn_chpl103(int64_t len_chpl,
#line 302 "../lib/Chai.chpl"
                                int64_t numChunks_chpl,
#line 302 "../lib/Chai.chpl"
                                range_int64_t_both_one_chpl this_chpl31,
#line 302 "../lib/Chai.chpl"
                                chpl___EndCount_AtomicT_int64_t_int64_t _coforallCount_chpl,
#line 302 "../lib/Chai.chpl"
                                int64_t chunk_chpl,
#line 302 "../lib/Chai.chpl"
                                int64_t dh_chpl,
#line 302 "../lib/Chai.chpl"
                                int64_t dw_chpl,
#line 302 "../lib/Chai.chpl"
                                int64_t kh_chpl,
#line 302 "../lib/Chai.chpl"
                                int64_t kw_chpl,
#line 302 "../lib/Chai.chpl"
                                int64_t m_chpl,
#line 302 "../lib/Chai.chpl"
                                int64_t n_chpl,
#line 302 "../lib/Chai.chpl"
                                int64_t stride_svar_chpl,
#line 302 "../lib/Chai.chpl"
                                int64_t padding_svar_chpl,
#line 302 "../lib/Chai.chpl"
                                Tensor_3__real64_chpl * delta_chpl,
#line 302 "../lib/Chai.chpl"
                                Tensor_4__real64_chpl * filters_svar_chpl,
#line 302 "../lib/Chai.chpl"
                                int64_t ci_chpl,
#line 302 "../lib/Chai.chpl"
                                SumReduceScanOp__real64_chpl RP_sum_chpl) {
#line 303 "../lib/Chai.chpl"
  int64_t local_dataParMinGranularity_chpl;
#line 303 "../lib/Chai.chpl"
  chpl_bool local_dataParIgnoreRunningTasks_chpl;
#line 303 "../lib/Chai.chpl"
  int64_t local_dataParTasksPerLocale_chpl;
#line 302 "../lib/Chai.chpl"
  SumReduceScanOp__real64_chpl RP_sum_chpl2 = NULL;
#line 302 "../lib/Chai.chpl"
  _real64 AS_sum_chpl;
#line 302 "../lib/Chai.chpl"
  SumReduceScanOp__real64_chpl new_temp_chpl = NULL;
#line 302 "../lib/Chai.chpl"
  chpl_LocalSpinlock default_arg_l_chpl;
#line 302 "../lib/Chai.chpl"
  chpl_LocalSpinlock ret_chpl;
#line 302 "../lib/Chai.chpl"
  chpl_LocalSpinlock l_chpl;
#line 302 "../lib/Chai.chpl"
  _tuple_2_star_int64_t_chpl call_tmp_chpl6;
#line 302 "../lib/Chai.chpl"
  _tuple_2_star_int64_t_chpl ret_tmp_chpl;
#line 302 "../lib/Chai.chpl"
  int64_t tmp_x0_chpl;
#line 302 "../lib/Chai.chpl"
  int64_t tmp_x1_chpl;
#line 302 "../lib/Chai.chpl"
  int64_t _ic__F1_high_chpl;
#line 302 "../lib/Chai.chpl"
  int64_t i_chpl;
#line 303 "../lib/Chai.chpl"
  _domain_DefaultRectangularDom_2_int64_t_one_chpl call_tmp_chpl7;
#line 303 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl call_tmp_chpl8;
#line 303 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl ret_tmp_chpl2;
#line 303 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl9;
#line 303 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl3;
#line 303 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl call_tmp_chpl10;
#line 303 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl ret_tmp_chpl4;
#line 303 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl11;
#line 303 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl5;
#line 303 "../lib/Chai.chpl"
  _domain_DefaultRectangularDom_2_int64_t_one_chpl ret_tmp_chpl6;
#line 303 "../lib/Chai.chpl"
  SumReduceScanOp__real64_chpl globalRP_sum_chpl = NULL;
#line 303 "../lib/Chai.chpl"
  SumReduceScanOp__real64_chpl new_temp_chpl2 = NULL;
#line 303 "../lib/Chai.chpl"
  chpl_LocalSpinlock default_arg_l_chpl2;
#line 303 "../lib/Chai.chpl"
  chpl_LocalSpinlock ret_chpl2;
#line 303 "../lib/Chai.chpl"
  chpl_LocalSpinlock l_chpl2;
#line 303 "../lib/Chai.chpl"
  SumReduceScanOp__real64_chpl coerce_tmp_chpl3 = NULL;
#line 303 "../lib/Chai.chpl"
  _ref__real64_chpl call_tmp_chpl12 = NULL;
#line 303 "../lib/Chai.chpl"
  _real64 globalAS_sum_chpl;
#line 303 "../lib/Chai.chpl"
  int64_t kh_chpl2;
#line 303 "../lib/Chai.chpl"
  int64_t kw_chpl2;
#line 303 "../lib/Chai.chpl"
  int64_t m_chpl2;
#line 303 "../lib/Chai.chpl"
  int64_t n_chpl2;
#line 303 "../lib/Chai.chpl"
  int64_t stride_svar_chpl2;
#line 303 "../lib/Chai.chpl"
  int64_t padding_svar_chpl2;
#line 303 "../lib/Chai.chpl"
  int64_t co_chpl;
#line 303 "../lib/Chai.chpl"
  int64_t ci_chpl2;
#line 303 "../lib/Chai.chpl"
  DefaultRectangularDom_2_int64_t_one_chpl coerce_tmp_chpl4 = NULL;
#line 303 "../lib/Chai.chpl"
  int64_t tasksPerLocale_chpl;
#line 303 "../lib/Chai.chpl"
  chpl_bool ignoreRunning_chpl;
#line 303 "../lib/Chai.chpl"
  int64_t minIndicesPerTask_chpl;
#line 303 "../lib/Chai.chpl"
  _tuple_2_star_int64_t_chpl ret_tmp_chpl7;
#line 303 "../lib/Chai.chpl"
  Error_chpl error_chpl = NULL;
#line 303 "../lib/Chai.chpl"
  int64_t tmp_chpl;
#line 303 "../lib/Chai.chpl"
  locale_chpl call_tmp_chpl13;
#line 303 "../lib/Chai.chpl"
  chpl_localeID_t call_tmp_chpl14;
#line 303 "../lib/Chai.chpl"
  int32_t call_tmp_chpl15;
#line 303 "../lib/Chai.chpl"
  chpl_localeID_t call_tmp_chpl16;
#line 303 "../lib/Chai.chpl"
  locale_chpl call_tmp_chpl17;
#line 303 "../lib/Chai.chpl"
  locale_chpl ret_tmp_chpl8;
#line 303 "../lib/Chai.chpl"
  int64_t ret_chpl3;
#line 303 "../lib/Chai.chpl"
  BaseLocale_chpl coerce_tmp_chpl5 = NULL;
#line 303 "../lib/Chai.chpl"
  BaseLocale_chpl coerce_tmp_chpl6 = NULL;
#line 303 "../lib/Chai.chpl"
  _ref_locale_chpl i_x_chpl = NULL;
#line 303 "../lib/Chai.chpl"
  int64_t tmp_x0_chpl2;
#line 303 "../lib/Chai.chpl"
  int64_t tmp_x1_chpl2;
#line 303 "../lib/Chai.chpl"
  _ref__tuple_2_star_range_int64_t_both_one_chpl call_tmp_chpl18 = NULL;
#line 303 "../lib/Chai.chpl"
  _tuple_2_star_int64_t_chpl ret_tmp_chpl9;
#line 303 "../lib/Chai.chpl"
  int64_t numChunks_chpl2;
#line 303 "../lib/Chai.chpl"
  int64_t parDim_chpl;
#line 303 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl tmpIter_chpl;
#line 303 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl call_tmp_chpl19;
#line 303 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl ret_tmp_chpl10;
#line 303 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl11;
#line 303 "../lib/Chai.chpl"
  chpl___EndCount_AtomicT_int64_t_int64_t _coforallCount_chpl2 = NULL;
#line 303 "../lib/Chai.chpl"
  chpl___EndCount_AtomicT_int64_t_int64_t new_temp_chpl3 = NULL;
#line 303 "../lib/Chai.chpl"
  int64_t numTasks_chpl;
#line 303 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl _ic__F0_this_chpl;
#line 303 "../lib/Chai.chpl"
  int64_t i_chpl2;
#line 303 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl this_chpl32;
#line 303 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl this_chpl33;
#line 303 "../lib/Chai.chpl"
  int64_t end_chpl;
#line 303 "../lib/Chai.chpl"
  _class_localscoforall_fn_chpl104 _args_forcoforall_fn_chpl = NULL;
#line 303 "../lib/Chai.chpl"
  _ref_Tensor_3__real64_chpl tmp_chpl2 = NULL;
#line 303 "../lib/Chai.chpl"
  _ref_Tensor_4__real64_chpl tmp_chpl3 = NULL;
#line 303 "../lib/Chai.chpl"
  raw_c_void_ptr _args_vforcoforall_fn_chpl;
#line 303 "../lib/Chai.chpl"
  Error_chpl error_chpl2 = NULL;
#line 303 "../lib/Chai.chpl"
  SumReduceScanOp__real64_chpl coerce_tmp_chpl7 = NULL;
#line 303 "../lib/Chai.chpl"
  _ref__real64_chpl call_tmp_chpl20 = NULL;
#line 303 "../lib/Chai.chpl"
  SumReduceScanOp__real64_chpl coerce_tmp_chpl8 = NULL;
#line 302 "../lib/Chai.chpl"
  SumReduceScanOp__real64_chpl coerce_tmp_chpl9 = NULL;
#line 302 "../lib/Chai.chpl"
  _ref__real64_chpl call_tmp_chpl21 = NULL;
#line 302 "../lib/Chai.chpl"
  chpl__class_localscoforall_fn_chpl104_object chpl_macro_tmp_109;
#line 303 "../lib/Chai.chpl"
  local_dataParMinGranularity_chpl = dataParMinGranularity_chpl;
#line 303 "../lib/Chai.chpl"
  local_dataParIgnoreRunningTasks_chpl = dataParIgnoreRunningTasks_chpl;
#line 303 "../lib/Chai.chpl"
  local_dataParTasksPerLocale_chpl = dataParTasksPerLocale_chpl;
#line 302 "../lib/Chai.chpl"
  init_chpl105(&l_chpl);
#line 302 "../lib/Chai.chpl"
  ret_chpl = l_chpl;
#line 302 "../lib/Chai.chpl"
  default_arg_l_chpl = ret_chpl;
#line 302 "../lib/Chai.chpl"
  new_temp_chpl = _new_chpl23(&default_arg_l_chpl, REAL64(0x0p+0), INT64(302), INT32(65));
#line 302 "../lib/Chai.chpl"
  RP_sum_chpl2 = new_temp_chpl;
#line 302 "../lib/Chai.chpl"
  AS_sum_chpl = REAL64(0x0p+0);
#line 302 "../lib/Chai.chpl"
  _computeBlock_chpl(len_chpl, numChunks_chpl, chunk_chpl, (&this_chpl31)->_high, (&this_chpl31)->_low, (&this_chpl31)->_low, &ret_tmp_chpl, INT64(302), INT32(65));
#line 302 "../lib/Chai.chpl"
  *(call_tmp_chpl6 + INT64(0)) = *(ret_tmp_chpl + INT64(0));
#line 302 "../lib/Chai.chpl"
  *(call_tmp_chpl6 + INT64(1)) = *(ret_tmp_chpl + INT64(1));
#line 302 "../lib/Chai.chpl"
  tmp_x0_chpl = *(call_tmp_chpl6 + INT64(0));
#line 302 "../lib/Chai.chpl"
  tmp_x1_chpl = *(call_tmp_chpl6 + INT64(1));
#line 302 "../lib/Chai.chpl"
  _ic__F1_high_chpl = tmp_x1_chpl;
#line 302 "../lib/Chai.chpl"
  i_chpl = INT64(0);
#line 302 "../lib/Chai.chpl"
  chpl_checkIfRangeIterWillOverflow(tmp_x0_chpl, tmp_x1_chpl, INT64(1), tmp_x0_chpl, tmp_x1_chpl, UINT8(true), INT64(302), INT32(65));
#line 302 "../lib/Chai.chpl"
  for (i_chpl = tmp_x0_chpl; ((i_chpl <= _ic__F1_high_chpl)); i_chpl += INT64(1)) {
#line 303 "../lib/Chai.chpl"
    chpl_build_low_bounded_range2(INT64(0), &ret_tmp_chpl2);
#line 303 "../lib/Chai.chpl"
    call_tmp_chpl8 = ret_tmp_chpl2;
#line 303 "../lib/Chai.chpl"
    chpl___POUND_3(call_tmp_chpl8, dh_chpl, &ret_tmp_chpl3, INT64(303), INT32(65));
#line 303 "../lib/Chai.chpl"
    call_tmp_chpl9 = ret_tmp_chpl3;
#line 303 "../lib/Chai.chpl"
    chpl_build_low_bounded_range2(INT64(0), &ret_tmp_chpl4);
#line 303 "../lib/Chai.chpl"
    call_tmp_chpl10 = ret_tmp_chpl4;
#line 303 "../lib/Chai.chpl"
    chpl___POUND_3(call_tmp_chpl10, dw_chpl, &ret_tmp_chpl5, INT64(303), INT32(65));
#line 303 "../lib/Chai.chpl"
    call_tmp_chpl11 = ret_tmp_chpl5;
#line 303 "../lib/Chai.chpl"
    chpl__buildDomainExpr2(call_tmp_chpl9, call_tmp_chpl11, UINT8(true), &ret_tmp_chpl6, INT64(303), INT32(65));
#line 303 "../lib/Chai.chpl"
    call_tmp_chpl7 = ret_tmp_chpl6;
#line 303 "../lib/Chai.chpl"
    init_chpl105(&l_chpl2);
#line 303 "../lib/Chai.chpl"
    ret_chpl2 = l_chpl2;
#line 303 "../lib/Chai.chpl"
    default_arg_l_chpl2 = ret_chpl2;
#line 303 "../lib/Chai.chpl"
    new_temp_chpl2 = _new_chpl23(&default_arg_l_chpl2, REAL64(0x0p+0), INT64(303), INT32(65));
#line 303 "../lib/Chai.chpl"
    globalRP_sum_chpl = new_temp_chpl2;
#line 303 "../lib/Chai.chpl"
    coerce_tmp_chpl3 = globalRP_sum_chpl;
#line 303 "../lib/Chai.chpl"
    chpl_check_nil(coerce_tmp_chpl3, INT64(303), INT32(65));
#line 303 "../lib/Chai.chpl"
    call_tmp_chpl12 = &((coerce_tmp_chpl3)->value);
#line 303 "../lib/Chai.chpl"
    *(call_tmp_chpl12) += AS_sum_chpl;
#line 303 "../lib/Chai.chpl"
    globalAS_sum_chpl = REAL64(0x0p+0);
#line 303 "../lib/Chai.chpl"
    kh_chpl2 = kh_chpl;
#line 303 "../lib/Chai.chpl"
    kw_chpl2 = kw_chpl;
#line 303 "../lib/Chai.chpl"
    m_chpl2 = m_chpl;
#line 303 "../lib/Chai.chpl"
    n_chpl2 = n_chpl;
#line 303 "../lib/Chai.chpl"
    stride_svar_chpl2 = stride_svar_chpl;
#line 303 "../lib/Chai.chpl"
    padding_svar_chpl2 = padding_svar_chpl;
#line 303 "../lib/Chai.chpl"
    co_chpl = i_chpl;
#line 303 "../lib/Chai.chpl"
    ci_chpl2 = ci_chpl;
#line 303 "../lib/Chai.chpl"
    coerce_tmp_chpl4 = (&call_tmp_chpl7)->_instance;
#line 303 "../lib/Chai.chpl"
    tasksPerLocale_chpl = local_dataParTasksPerLocale_chpl;
#line 303 "../lib/Chai.chpl"
    ignoreRunning_chpl = local_dataParIgnoreRunningTasks_chpl;
#line 303 "../lib/Chai.chpl"
    minIndicesPerTask_chpl = local_dataParMinGranularity_chpl;
#line 303 "../lib/Chai.chpl"
    createTuple_chpl4(INT64(0), &ret_tmp_chpl7);
#line 303 "../lib/Chai.chpl"
    error_chpl = NULL;
#line 303 "../lib/Chai.chpl"
    if (tasksPerLocale_chpl == INT64(0)) /* ZLINE: 303 ../lib/Chai.chpl */
#line 303 "../lib/Chai.chpl"
    {
#line 303 "../lib/Chai.chpl"
      call_tmp_chpl15 = get_chpl_nodeID();
#line 303 "../lib/Chai.chpl"
      call_tmp_chpl16 = chpl_rt_buildLocaleID(call_tmp_chpl15, c_sublocid_any);
#line 303 "../lib/Chai.chpl"
      call_tmp_chpl14 = call_tmp_chpl16;
#line 303 "../lib/Chai.chpl"
      chpl_localeID_to_locale(&call_tmp_chpl14, &ret_tmp_chpl8, INT64(303), INT32(65));
#line 303 "../lib/Chai.chpl"
      call_tmp_chpl17 = ret_tmp_chpl8;
#line 303 "../lib/Chai.chpl"
      call_tmp_chpl13 = call_tmp_chpl17;
#line 303 "../lib/Chai.chpl"
      coerce_tmp_chpl5 = (&call_tmp_chpl13)->_instance;
#line 303 "../lib/Chai.chpl"
      if (((RootClass_chpl)(coerce_tmp_chpl5)) == nil) /* ZLINE: 303 ../lib/Chai.chpl */
#line 303 "../lib/Chai.chpl"
      {
#line 303 "../lib/Chai.chpl"
        nilCheckHalt_chpl(&_str_literal_52_chpl, INT64(303), INT32(65));
#line 303 "../lib/Chai.chpl"
      }
#line 303 "../lib/Chai.chpl"
      coerce_tmp_chpl6 = coerce_tmp_chpl5;
#line 303 "../lib/Chai.chpl"
      chpl_check_nil(coerce_tmp_chpl6, INT64(303), INT32(65));
#line 303 "../lib/Chai.chpl"
      ret_chpl3 = (coerce_tmp_chpl6)->maxTaskPar;
#line 303 "../lib/Chai.chpl"
      tmp_chpl = ret_chpl3;
#line 303 "../lib/Chai.chpl"
      i_x_chpl = &call_tmp_chpl13;
#line 303 "../lib/Chai.chpl"
      deinit_chpl62(i_x_chpl);
#line 303 "../lib/Chai.chpl"
    } else /* ZLINE: 303 ../lib/Chai.chpl */
#line 303 "../lib/Chai.chpl"
    {
#line 303 "../lib/Chai.chpl"
      tmp_chpl = tasksPerLocale_chpl;
#line 303 "../lib/Chai.chpl"
    }
#line 303 "../lib/Chai.chpl"
    if (chpl_task_getSerial()) /* ZLINE: 303 ../lib/Chai.chpl */
#line 303 "../lib/Chai.chpl"
    {
#line 303 "../lib/Chai.chpl"
      tmp_x0_chpl2 = INT64(1);
#line 303 "../lib/Chai.chpl"
      tmp_x1_chpl2 = INT64(0);
#line 303 "../lib/Chai.chpl"
    } else /* ZLINE: 303 ../lib/Chai.chpl */
#line 303 "../lib/Chai.chpl"
    {
#line 303 "../lib/Chai.chpl"
      chpl_check_nil(coerce_tmp_chpl4, INT64(303), INT32(65));
#line 303 "../lib/Chai.chpl"
      call_tmp_chpl18 = &((coerce_tmp_chpl4)->ranges);
#line 303 "../lib/Chai.chpl"
      _computeChunkStuff_chpl4(tmp_chpl, ignoreRunning_chpl, minIndicesPerTask_chpl, call_tmp_chpl18, &ret_tmp_chpl9, INT64(303), INT32(65));
#line 303 "../lib/Chai.chpl"
      tmp_x0_chpl2 = *(ret_tmp_chpl9 + INT64(0));
#line 303 "../lib/Chai.chpl"
      tmp_x1_chpl2 = *(ret_tmp_chpl9 + INT64(1));
#line 303 "../lib/Chai.chpl"
    }
#line 303 "../lib/Chai.chpl"
    numChunks_chpl2 = tmp_x0_chpl2;
#line 303 "../lib/Chai.chpl"
    parDim_chpl = tmp_x1_chpl2;
#line 303 "../lib/Chai.chpl"
    chpl_build_low_bounded_range2(INT64(0), &ret_tmp_chpl10);
#line 303 "../lib/Chai.chpl"
    call_tmp_chpl19 = ret_tmp_chpl10;
#line 303 "../lib/Chai.chpl"
    chpl___POUND_3(call_tmp_chpl19, tmp_x0_chpl2, &ret_tmp_chpl11, INT64(303), INT32(65));
#line 303 "../lib/Chai.chpl"
    tmpIter_chpl = ret_tmp_chpl11;
#line 303 "../lib/Chai.chpl"
    new_temp_chpl3 = _new_chpl17(INT64(303), INT32(65));
#line 303 "../lib/Chai.chpl"
    _coforallCount_chpl2 = new_temp_chpl3;
#line 303 "../lib/Chai.chpl"
    numTasks_chpl = chpl_boundedCoforallSize(tmpIter_chpl, INT64(303), INT32(65));
#line 303 "../lib/Chai.chpl"
    _upEndCount_chpl(_coforallCount_chpl2, numTasks_chpl, INT64(303), INT32(65));
#line 303 "../lib/Chai.chpl"
    chpl_resetTaskSpawn(numTasks_chpl, INT64(303), INT32(65));
#line 303 "../lib/Chai.chpl"
    _ic__F0_this_chpl = tmpIter_chpl;
#line 303 "../lib/Chai.chpl"
    checkIfIterWillOverflow_chpl(tmpIter_chpl, UINT8(true), INT64(303), INT32(65));
#line 303 "../lib/Chai.chpl"
    i_chpl2 = INT64(0);
#line 303 "../lib/Chai.chpl"
    this_chpl32 = _ic__F0_this_chpl;
#line 303 "../lib/Chai.chpl"
    this_chpl33 = _ic__F0_this_chpl;
#line 303 "../lib/Chai.chpl"
    end_chpl = (&this_chpl33)->_high;
#line 303 "../lib/Chai.chpl"
    for (i_chpl2 = (&this_chpl32)->_low; ((i_chpl2 <= end_chpl)); i_chpl2 += INT64(1)) {
#line 303 "../lib/Chai.chpl"
      _args_forcoforall_fn_chpl = ((_class_localscoforall_fn_chpl104)(&chpl_macro_tmp_109));
#line 303 "../lib/Chai.chpl"
      chpl_check_nil(_args_forcoforall_fn_chpl, INT64(303), INT32(65));
#line 303 "../lib/Chai.chpl"
      (_args_forcoforall_fn_chpl)->_0_numChunks = numChunks_chpl2;
#line 303 "../lib/Chai.chpl"
      chpl_check_nil(_args_forcoforall_fn_chpl, INT64(303), INT32(65));
#line 303 "../lib/Chai.chpl"
      (_args_forcoforall_fn_chpl)->_1_parDim = parDim_chpl;
#line 303 "../lib/Chai.chpl"
      chpl_check_nil(_args_forcoforall_fn_chpl, INT64(303), INT32(65));
#line 303 "../lib/Chai.chpl"
      (_args_forcoforall_fn_chpl)->_2_coerce_tmp = coerce_tmp_chpl4;
#line 303 "../lib/Chai.chpl"
      chpl_check_nil(_args_forcoforall_fn_chpl, INT64(303), INT32(65));
#line 303 "../lib/Chai.chpl"
      (_args_forcoforall_fn_chpl)->_3__coforallCount = _coforallCount_chpl2;
#line 303 "../lib/Chai.chpl"
      chpl_check_nil(_args_forcoforall_fn_chpl, INT64(303), INT32(65));
#line 303 "../lib/Chai.chpl"
      (_args_forcoforall_fn_chpl)->_4_chunk = i_chpl2;
#line 303 "../lib/Chai.chpl"
      chpl_check_nil(_args_forcoforall_fn_chpl, INT64(303), INT32(65));
#line 303 "../lib/Chai.chpl"
      (_args_forcoforall_fn_chpl)->_6_kh = kh_chpl2;
#line 303 "../lib/Chai.chpl"
      chpl_check_nil(_args_forcoforall_fn_chpl, INT64(303), INT32(65));
#line 303 "../lib/Chai.chpl"
      (_args_forcoforall_fn_chpl)->_7_kw = kw_chpl2;
#line 303 "../lib/Chai.chpl"
      chpl_check_nil(_args_forcoforall_fn_chpl, INT64(303), INT32(65));
#line 303 "../lib/Chai.chpl"
      (_args_forcoforall_fn_chpl)->_8_m = m_chpl2;
#line 303 "../lib/Chai.chpl"
      chpl_check_nil(_args_forcoforall_fn_chpl, INT64(303), INT32(65));
#line 303 "../lib/Chai.chpl"
      (_args_forcoforall_fn_chpl)->_9_n = n_chpl2;
#line 303 "../lib/Chai.chpl"
      chpl_check_nil(_args_forcoforall_fn_chpl, INT64(303), INT32(65));
#line 303 "../lib/Chai.chpl"
      (_args_forcoforall_fn_chpl)->_10_stride_svar = stride_svar_chpl2;
#line 303 "../lib/Chai.chpl"
      chpl_check_nil(_args_forcoforall_fn_chpl, INT64(303), INT32(65));
#line 303 "../lib/Chai.chpl"
      (_args_forcoforall_fn_chpl)->_11_padding_svar = padding_svar_chpl2;
#line 303 "../lib/Chai.chpl"
      tmp_chpl2 = delta_chpl;
#line 303 "../lib/Chai.chpl"
      chpl_check_nil(_args_forcoforall_fn_chpl, INT64(303), INT32(65));
#line 303 "../lib/Chai.chpl"
      (_args_forcoforall_fn_chpl)->_12_delta = tmp_chpl2;
#line 303 "../lib/Chai.chpl"
      chpl_check_nil(_args_forcoforall_fn_chpl, INT64(303), INT32(65));
#line 303 "../lib/Chai.chpl"
      (_args_forcoforall_fn_chpl)->_13_co = co_chpl;
#line 303 "../lib/Chai.chpl"
      tmp_chpl3 = filters_svar_chpl;
#line 303 "../lib/Chai.chpl"
      chpl_check_nil(_args_forcoforall_fn_chpl, INT64(303), INT32(65));
#line 303 "../lib/Chai.chpl"
      (_args_forcoforall_fn_chpl)->_14_filters_svar = tmp_chpl3;
#line 303 "../lib/Chai.chpl"
      chpl_check_nil(_args_forcoforall_fn_chpl, INT64(303), INT32(65));
#line 303 "../lib/Chai.chpl"
      (_args_forcoforall_fn_chpl)->_15_ci = ci_chpl2;
#line 303 "../lib/Chai.chpl"
      chpl_check_nil(_args_forcoforall_fn_chpl, INT64(303), INT32(65));
#line 303 "../lib/Chai.chpl"
      (_args_forcoforall_fn_chpl)->_16_globalRP_sum = globalRP_sum_chpl;
#line 303 "../lib/Chai.chpl"
      _args_vforcoforall_fn_chpl = ((void*)(_args_forcoforall_fn_chpl));
#line 303 "../lib/Chai.chpl"
      /*** wrapcoforall_fn_chpl104 ***/ chpl_taskAddCoStmt(chpl_task_getRequestedSubloc(), INT64(103), ((chpl_task_bundle_p)(_args_vforcoforall_fn_chpl)), sizeof(chpl__class_localscoforall_fn_chpl104_object), INT64(303), INT32(65));
#line 303 "../lib/Chai.chpl"
    }
#line 303 "../lib/Chai.chpl"
    error_chpl2 = NULL;
#line 303 "../lib/Chai.chpl"
    _waitEndCount_chpl2(_coforallCount_chpl2, numTasks_chpl, &error_chpl2, INT64(303), INT32(65));
#line 303 "../lib/Chai.chpl"
    if (error_chpl2 != nil) /* ZLINE: 303 ../lib/Chai.chpl */
#line 303 "../lib/Chai.chpl"
    {
#line 303 "../lib/Chai.chpl"
      chpl__delete72(_coforallCount_chpl2, INT64(303), INT32(65));
#line 303 "../lib/Chai.chpl"
      error_chpl = error_chpl2;
#line 303 "../lib/Chai.chpl"
      goto forall_IBB_break_label_chpl;
#line 303 "../lib/Chai.chpl"
    }
#line 303 "../lib/Chai.chpl"
    chpl__delete72(_coforallCount_chpl2, INT64(303), INT32(65));
#line 303 "../lib/Chai.chpl"
    chpl_after_forall_fence();
#line 303 "../lib/Chai.chpl"
    forall_IBB_break_label_chpl:;
#line 303 "../lib/Chai.chpl"
    if (error_chpl != nil) /* ZLINE: 303 ../lib/Chai.chpl */
#line 303 "../lib/Chai.chpl"
    {
#line 303 "../lib/Chai.chpl"
      chpl_propagate_error(error_chpl, INT64(303), INT32(65));
#line 303 "../lib/Chai.chpl"
    }
#line 303 "../lib/Chai.chpl"
    chpl_after_forall_fence();
#line 303 "../lib/Chai.chpl"
    chpl__autoDestroy5(&call_tmp_chpl7, INT64(303), INT32(65));
#line 303 "../lib/Chai.chpl"
    coerce_tmp_chpl7 = globalRP_sum_chpl;
#line 303 "../lib/Chai.chpl"
    chpl_check_nil(coerce_tmp_chpl7, INT64(303), INT32(65));
#line 303 "../lib/Chai.chpl"
    call_tmp_chpl20 = &((coerce_tmp_chpl7)->value);
#line 303 "../lib/Chai.chpl"
    *(call_tmp_chpl20) += globalAS_sum_chpl;
#line 303 "../lib/Chai.chpl"
    coerce_tmp_chpl8 = globalRP_sum_chpl;
#line 303 "../lib/Chai.chpl"
    chpl_check_nil(coerce_tmp_chpl8, INT64(303), INT32(65));
#line 303 "../lib/Chai.chpl"
    AS_sum_chpl = (coerce_tmp_chpl8)->value;
#line 303 "../lib/Chai.chpl"
    chpl__delete33(globalRP_sum_chpl, INT64(303), INT32(65));
#line 303 "../lib/Chai.chpl"
  }
#line 302 "../lib/Chai.chpl"
  coerce_tmp_chpl9 = RP_sum_chpl2;
#line 302 "../lib/Chai.chpl"
  chpl_check_nil(coerce_tmp_chpl9, INT64(302), INT32(65));
#line 302 "../lib/Chai.chpl"
  call_tmp_chpl21 = &((coerce_tmp_chpl9)->value);
#line 302 "../lib/Chai.chpl"
  *(call_tmp_chpl21) += AS_sum_chpl;
#line 302 "../lib/Chai.chpl"
  chpl__reduceCombine6(RP_sum_chpl, RP_sum_chpl2, INT64(302), INT32(65));
#line 302 "../lib/Chai.chpl"
  chpl__delete33(RP_sum_chpl2, INT64(302), INT32(65));
#line 302 "../lib/Chai.chpl"
  return;
#line 302 "../lib/Chai.chpl"
}

#line 303 "../lib/Chai.chpl"
/* Chai.chpl:303 */
#line 303 "../lib/Chai.chpl"
static void wrapcoforall_fn_chpl104(_class_localscoforall_fn_chpl104 c_chpl) {
#line 303 "../lib/Chai.chpl"
  Error_chpl error_chpl = NULL;
#line 303 "../lib/Chai.chpl"
  int64_t _0_numChunks_chpl;
#line 303 "../lib/Chai.chpl"
  int64_t _1_parDim_chpl;
#line 303 "../lib/Chai.chpl"
  DefaultRectangularDom_2_int64_t_one_chpl _2_coerce_tmp_chpl = NULL;
#line 303 "../lib/Chai.chpl"
  chpl___EndCount_AtomicT_int64_t_int64_t _3__coforallCount_chpl = NULL;
#line 303 "../lib/Chai.chpl"
  int64_t _4_chunk_chpl;
#line 303 "../lib/Chai.chpl"
  int64_t _6_kh_chpl;
#line 303 "../lib/Chai.chpl"
  int64_t _7_kw_chpl;
#line 303 "../lib/Chai.chpl"
  int64_t _8_m_chpl;
#line 303 "../lib/Chai.chpl"
  int64_t _9_n_chpl;
#line 303 "../lib/Chai.chpl"
  int64_t _10_stride_svar_chpl;
#line 303 "../lib/Chai.chpl"
  int64_t _11_padding_svar_chpl;
#line 303 "../lib/Chai.chpl"
  _ref_Tensor_3__real64_chpl _12_delta_chpl = NULL;
#line 303 "../lib/Chai.chpl"
  int64_t _13_co_chpl;
#line 303 "../lib/Chai.chpl"
  _ref_Tensor_4__real64_chpl _14_filters_svar_chpl = NULL;
#line 303 "../lib/Chai.chpl"
  int64_t _15_ci_chpl;
#line 303 "../lib/Chai.chpl"
  chpl_rmem_consist_acquire(INT64(303), INT32(65));
#line 303 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(303), INT32(65));
#line 303 "../lib/Chai.chpl"
  _0_numChunks_chpl = (c_chpl)->_0_numChunks;
#line 303 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(303), INT32(65));
#line 303 "../lib/Chai.chpl"
  _1_parDim_chpl = (c_chpl)->_1_parDim;
#line 303 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(303), INT32(65));
#line 303 "../lib/Chai.chpl"
  _2_coerce_tmp_chpl = (c_chpl)->_2_coerce_tmp;
#line 303 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(303), INT32(65));
#line 303 "../lib/Chai.chpl"
  _3__coforallCount_chpl = (c_chpl)->_3__coforallCount;
#line 303 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(303), INT32(65));
#line 303 "../lib/Chai.chpl"
  _4_chunk_chpl = (c_chpl)->_4_chunk;
#line 303 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(303), INT32(65));
#line 303 "../lib/Chai.chpl"
  _6_kh_chpl = (c_chpl)->_6_kh;
#line 303 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(303), INT32(65));
#line 303 "../lib/Chai.chpl"
  _7_kw_chpl = (c_chpl)->_7_kw;
#line 303 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(303), INT32(65));
#line 303 "../lib/Chai.chpl"
  _8_m_chpl = (c_chpl)->_8_m;
#line 303 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(303), INT32(65));
#line 303 "../lib/Chai.chpl"
  _9_n_chpl = (c_chpl)->_9_n;
#line 303 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(303), INT32(65));
#line 303 "../lib/Chai.chpl"
  _10_stride_svar_chpl = (c_chpl)->_10_stride_svar;
#line 303 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(303), INT32(65));
#line 303 "../lib/Chai.chpl"
  _11_padding_svar_chpl = (c_chpl)->_11_padding_svar;
#line 303 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(303), INT32(65));
#line 303 "../lib/Chai.chpl"
  _12_delta_chpl = (c_chpl)->_12_delta;
#line 303 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(303), INT32(65));
#line 303 "../lib/Chai.chpl"
  _13_co_chpl = (c_chpl)->_13_co;
#line 303 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(303), INT32(65));
#line 303 "../lib/Chai.chpl"
  _14_filters_svar_chpl = (c_chpl)->_14_filters_svar;
#line 303 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(303), INT32(65));
#line 303 "../lib/Chai.chpl"
  _15_ci_chpl = (c_chpl)->_15_ci;
#line 303 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(303), INT32(65));
#line 303 "../lib/Chai.chpl"
  coforall_fn_chpl104(_0_numChunks_chpl, _1_parDim_chpl, _2_coerce_tmp_chpl, _3__coforallCount_chpl, _4_chunk_chpl, &error_chpl, _6_kh_chpl, _7_kw_chpl, _8_m_chpl, _9_n_chpl, _10_stride_svar_chpl, _11_padding_svar_chpl, _12_delta_chpl, _13_co_chpl, _14_filters_svar_chpl, _15_ci_chpl, (c_chpl)->_16_globalRP_sum);
#line 303 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(303), INT32(65));
#line 303 "../lib/Chai.chpl"
  _downEndCount_chpl((c_chpl)->_3__coforallCount, error_chpl, INT64(303), INT32(65));
#line 303 "../lib/Chai.chpl"
  return;
#line 303 "../lib/Chai.chpl"
}

#line 303 "../lib/Chai.chpl"
/* Chai.chpl:303 */
#line 303 "../lib/Chai.chpl"
static void coforall_fn_chpl104(int64_t numChunks_chpl,
#line 303 "../lib/Chai.chpl"
                                int64_t parDim_chpl,
#line 303 "../lib/Chai.chpl"
                                DefaultRectangularDom_2_int64_t_one_chpl this_chpl31,
#line 303 "../lib/Chai.chpl"
                                chpl___EndCount_AtomicT_int64_t_int64_t _coforallCount_chpl,
#line 303 "../lib/Chai.chpl"
                                int64_t chunk_chpl,
#line 303 "../lib/Chai.chpl"
                                Error_chpl * error_out_chpl,
#line 303 "../lib/Chai.chpl"
                                int64_t kh_chpl,
#line 303 "../lib/Chai.chpl"
                                int64_t kw_chpl,
#line 303 "../lib/Chai.chpl"
                                int64_t m_chpl,
#line 303 "../lib/Chai.chpl"
                                int64_t n_chpl,
#line 303 "../lib/Chai.chpl"
                                int64_t stride_svar_chpl,
#line 303 "../lib/Chai.chpl"
                                int64_t padding_svar_chpl,
#line 303 "../lib/Chai.chpl"
                                Tensor_3__real64_chpl * delta_chpl,
#line 303 "../lib/Chai.chpl"
                                int64_t co_chpl,
#line 303 "../lib/Chai.chpl"
                                Tensor_4__real64_chpl * filters_svar_chpl,
#line 303 "../lib/Chai.chpl"
                                int64_t ci_chpl,
#line 303 "../lib/Chai.chpl"
                                SumReduceScanOp__real64_chpl RP_sum_chpl) {
#line 303 "../lib/Chai.chpl"
  SumReduceScanOp__real64_chpl RP_sum_chpl2 = NULL;
#line 303 "../lib/Chai.chpl"
  _real64 AS_sum_chpl;
#line 303 "../lib/Chai.chpl"
  SumReduceScanOp__real64_chpl new_temp_chpl = NULL;
#line 303 "../lib/Chai.chpl"
  chpl_LocalSpinlock default_arg_l_chpl;
#line 303 "../lib/Chai.chpl"
  chpl_LocalSpinlock ret_chpl;
#line 303 "../lib/Chai.chpl"
  chpl_LocalSpinlock l_chpl;
#line 303 "../lib/Chai.chpl"
  _tuple_2_star_range_int64_t_both_one_chpl ranges_chpl;
#line 303 "../lib/Chai.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl6 = NULL;
#line 303 "../lib/Chai.chpl"
  uint64_t call_tmp_chpl7;
#line 303 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl3;
#line 303 "../lib/Chai.chpl"
  uint64_t call_tmp_chpl8;
#line 303 "../lib/Chai.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl9 = NULL;
#line 303 "../lib/Chai.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl10 = NULL;
#line 303 "../lib/Chai.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl11 = NULL;
#line 303 "../lib/Chai.chpl"
  _tuple_2_star_int64_t_chpl call_tmp_chpl12;
#line 303 "../lib/Chai.chpl"
  _tuple_2_star_int64_t_chpl ret_tmp_chpl;
#line 303 "../lib/Chai.chpl"
  int64_t tmp_x0_chpl;
#line 303 "../lib/Chai.chpl"
  int64_t tmp_x1_chpl;
#line 303 "../lib/Chai.chpl"
  _tuple_2_star_range_int64_t_both_one_chpl block_chpl;
#line 303 "../lib/Chai.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl13 = NULL;
#line 303 "../lib/Chai.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl14 = NULL;
#line 303 "../lib/Chai.chpl"
  _tuple_2_star_range_int64_t_both_one_chpl coerce_tmp_chpl4;
#line 303 "../lib/Chai.chpl"
  _tuple_2_star_range_int64_t_both_one_chpl retcopy_chpl;
#line 303 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl5;
#line 303 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl _ic__F0_this_chpl;
#line 303 "../lib/Chai.chpl"
  int64_t i_chpl;
#line 303 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl this_chpl32;
#line 303 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl this_chpl33;
#line 303 "../lib/Chai.chpl"
  int64_t end_chpl;
#line 303 "../lib/Chai.chpl"
  int64_t i_chpl2;
#line 303 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl6;
#line 303 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl _ic__F0_this_chpl2;
#line 303 "../lib/Chai.chpl"
  int64_t i_chpl3;
#line 303 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl this_chpl34;
#line 303 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl this_chpl35;
#line 303 "../lib/Chai.chpl"
  int64_t end_chpl2;
#line 303 "../lib/Chai.chpl"
  _tuple_2_star_int64_t_chpl ret_tmp_chpl2;
#line 303 "../lib/Chai.chpl"
  _tuple_2_star_int64_t_chpl yret_chpl;
#line 303 "../lib/Chai.chpl"
  int64_t chpl_idx_1_x0;
#line 303 "../lib/Chai.chpl"
  int64_t chpl_idx_1_x1;
#line 304 "../lib/Chai.chpl"
  _tuple_2_star_int64_t_chpl call_tmp_chpl15;
#line 304 "../lib/Chai.chpl"
  _tuple_2_star_int64_t_chpl ret_tmp_chpl3;
#line 304 "../lib/Chai.chpl"
  _tuple_2_star_int64_t_chpl call_tmp_chpl16;
#line 304 "../lib/Chai.chpl"
  _tuple_2_star_int64_t_chpl ret_tmp_chpl4;
#line 304 "../lib/Chai.chpl"
  _tuple_2_star_int64_t_chpl call_tmp_chpl17;
#line 304 "../lib/Chai.chpl"
  _tuple_2_star_int64_t_chpl ret_tmp_chpl5;
#line 304 "../lib/Chai.chpl"
  _tuple_2_star_int64_t_chpl call_tmp_chpl18;
#line 304 "../lib/Chai.chpl"
  _tuple_2_star_int64_t_chpl ret_tmp_chpl6;
#line 304 "../lib/Chai.chpl"
  int64_t tmp_x0_chpl2;
#line 304 "../lib/Chai.chpl"
  int64_t tmp_x1_chpl2;
#line 306 "../lib/Chai.chpl"
  _ref__array_DefaultRectangularArr_3_int64_t_one__real64_int64_t_chpl call_tmp_chpl19 = NULL;
#line 306 "../lib/Chai.chpl"
  _tuple_3_star_int64_t_chpl coerce_tmp_chpl7;
#line 306 "../lib/Chai.chpl"
  _ref__tuple_3_star_int64_t_chpl i_i_chpl = NULL;
#line 306 "../lib/Chai.chpl"
  DefaultRectangularArr_3_int64_t_one__real64_int64_t_chpl ret_chpl2 = NULL;
#line 306 "../lib/Chai.chpl"
  DefaultRectangularArr_3_int64_t_one__real64_int64_t_chpl coerce_tmp_chpl8 = NULL;
#line 306 "../lib/Chai.chpl"
  _tuple_3_star_int64_t_chpl _formal_tmp_in_ind_chpl;
#line 306 "../lib/Chai.chpl"
  int64_t _formal_tmp_ind_x0_chpl;
#line 306 "../lib/Chai.chpl"
  int64_t _formal_tmp_ind_x1_chpl;
#line 306 "../lib/Chai.chpl"
  int64_t _formal_tmp_ind_x2_chpl;
#line 306 "../lib/Chai.chpl"
  _real64 ret_chpl3;
#line 306 "../lib/Chai.chpl"
  int64_t ret_chpl4;
#line 306 "../lib/Chai.chpl"
  int64_t sum_chpl;
#line 306 "../lib/Chai.chpl"
  _ref__tuple_3_star_int64_t_chpl call_tmp_chpl20 = NULL;
#line 306 "../lib/Chai.chpl"
  _ref_int64_t_chpl i_lhs_chpl = NULL;
#line 306 "../lib/Chai.chpl"
  _ref__tuple_3_star_int64_t_chpl call_tmp_chpl21 = NULL;
#line 306 "../lib/Chai.chpl"
  _ref_int64_t_chpl i_lhs_chpl2 = NULL;
#line 306 "../lib/Chai.chpl"
  _ref_int64_t_chpl i_lhs_chpl3 = NULL;
#line 306 "../lib/Chai.chpl"
  _ddata__real64_chpl coerce_tmp_chpl9 = NULL;
#line 306 "../lib/Chai.chpl"
  _ref__real64_chpl call_tmp_chpl22 = NULL;
#line 306 "../lib/Chai.chpl"
  _ref__array_DefaultRectangularArr_4_int64_t_one__real64_int64_t_chpl call_tmp_chpl23 = NULL;
#line 306 "../lib/Chai.chpl"
  _tuple_4_star_int64_t_chpl coerce_tmp_chpl10;
#line 306 "../lib/Chai.chpl"
  _ref__tuple_4_star_int64_t_chpl i_i_chpl2 = NULL;
#line 306 "../lib/Chai.chpl"
  DefaultRectangularArr_4_int64_t_one__real64_int64_t_chpl ret_chpl5 = NULL;
#line 306 "../lib/Chai.chpl"
  DefaultRectangularArr_4_int64_t_one__real64_int64_t_chpl coerce_tmp_chpl11 = NULL;
#line 306 "../lib/Chai.chpl"
  _tuple_4_star_int64_t_chpl _formal_tmp_in_ind_chpl2;
#line 306 "../lib/Chai.chpl"
  int64_t _formal_tmp_ind_x0_chpl2;
#line 306 "../lib/Chai.chpl"
  int64_t _formal_tmp_ind_x1_chpl2;
#line 306 "../lib/Chai.chpl"
  int64_t _formal_tmp_ind_x2_chpl2;
#line 306 "../lib/Chai.chpl"
  int64_t _formal_tmp_ind_x3_chpl;
#line 306 "../lib/Chai.chpl"
  int64_t ret_chpl6;
#line 306 "../lib/Chai.chpl"
  int64_t sum_chpl2;
#line 306 "../lib/Chai.chpl"
  _ref__tuple_4_star_int64_t_chpl call_tmp_chpl24 = NULL;
#line 306 "../lib/Chai.chpl"
  _ref_int64_t_chpl i_lhs_chpl4 = NULL;
#line 306 "../lib/Chai.chpl"
  _ref__tuple_4_star_int64_t_chpl call_tmp_chpl25 = NULL;
#line 306 "../lib/Chai.chpl"
  _ref_int64_t_chpl i_lhs_chpl5 = NULL;
#line 306 "../lib/Chai.chpl"
  _ref__tuple_4_star_int64_t_chpl call_tmp_chpl26 = NULL;
#line 306 "../lib/Chai.chpl"
  _ref_int64_t_chpl i_lhs_chpl6 = NULL;
#line 306 "../lib/Chai.chpl"
  _ref_int64_t_chpl i_lhs_chpl7 = NULL;
#line 306 "../lib/Chai.chpl"
  _ddata__real64_chpl coerce_tmp_chpl12 = NULL;
#line 306 "../lib/Chai.chpl"
  _ref__real64_chpl call_tmp_chpl27 = NULL;
#line 306 "../lib/Chai.chpl"
  _ref__real64_chpl i_lhs_chpl8 = NULL;
#line 303 "../lib/Chai.chpl"
  SumReduceScanOp__real64_chpl coerce_tmp_chpl13 = NULL;
#line 303 "../lib/Chai.chpl"
  _ref__real64_chpl call_tmp_chpl28 = NULL;
#line 303 "../lib/Chai.chpl"
  init_chpl105(&l_chpl);
#line 303 "../lib/Chai.chpl"
  ret_chpl = l_chpl;
#line 303 "../lib/Chai.chpl"
  default_arg_l_chpl = ret_chpl;
#line 303 "../lib/Chai.chpl"
  new_temp_chpl = _new_chpl23(&default_arg_l_chpl, REAL64(0x0p+0), INT64(303), INT32(65));
#line 303 "../lib/Chai.chpl"
  RP_sum_chpl2 = new_temp_chpl;
#line 303 "../lib/Chai.chpl"
  AS_sum_chpl = REAL64(0x0p+0);
#line 303 "../lib/Chai.chpl"
  chpl_check_nil(this_chpl31, INT64(303), INT32(65));
#line 303 "../lib/Chai.chpl"
  *(ranges_chpl + INT64(0)) = *((this_chpl31)->ranges + INT64(0));
#line 303 "../lib/Chai.chpl"
  *(ranges_chpl + INT64(1)) = *((this_chpl31)->ranges + INT64(1));
#line 303 "../lib/Chai.chpl"
  call_tmp_chpl6 = this_chpl26(&ranges_chpl, parDim_chpl, INT64(303), INT32(65));
#line 303 "../lib/Chai.chpl"
  coerce_tmp_chpl3 = *(call_tmp_chpl6);
#line 303 "../lib/Chai.chpl"
  call_tmp_chpl7 = sizeAs_chpl36(coerce_tmp_chpl3, INT64(303), INT32(65));
#line 303 "../lib/Chai.chpl"
  this_chpl26(&ranges_chpl, parDim_chpl, INT64(303), INT32(65));
#line 303 "../lib/Chai.chpl"
  call_tmp_chpl8 = (call_tmp_chpl7 * UINT64(1));
#line 303 "../lib/Chai.chpl"
  call_tmp_chpl9 = this_chpl26(&ranges_chpl, parDim_chpl, INT64(303), INT32(65));
#line 303 "../lib/Chai.chpl"
  call_tmp_chpl10 = this_chpl26(&ranges_chpl, parDim_chpl, INT64(303), INT32(65));
#line 303 "../lib/Chai.chpl"
  call_tmp_chpl11 = this_chpl26(&ranges_chpl, parDim_chpl, INT64(303), INT32(65));
#line 303 "../lib/Chai.chpl"
  _computeBlock_chpl2(call_tmp_chpl8, numChunks_chpl, chunk_chpl, (call_tmp_chpl9)->_high, (call_tmp_chpl10)->_low, (call_tmp_chpl11)->_low, &ret_tmp_chpl, INT64(303), INT32(65));
#line 303 "../lib/Chai.chpl"
  *(call_tmp_chpl12 + INT64(0)) = *(ret_tmp_chpl + INT64(0));
#line 303 "../lib/Chai.chpl"
  *(call_tmp_chpl12 + INT64(1)) = *(ret_tmp_chpl + INT64(1));
#line 303 "../lib/Chai.chpl"
  tmp_x0_chpl = *(call_tmp_chpl12 + INT64(0));
#line 303 "../lib/Chai.chpl"
  tmp_x1_chpl = *(call_tmp_chpl12 + INT64(1));
#line 303 "../lib/Chai.chpl"
  *(block_chpl + INT64(0)) = *(ranges_chpl + INT64(0));
#line 303 "../lib/Chai.chpl"
  *(block_chpl + INT64(1)) = *(ranges_chpl + INT64(1));
#line 303 "../lib/Chai.chpl"
  call_tmp_chpl13 = this_chpl26(&block_chpl, parDim_chpl, INT64(303), INT32(65));
#line 303 "../lib/Chai.chpl"
  (call_tmp_chpl13)->_low = tmp_x0_chpl;
#line 303 "../lib/Chai.chpl"
  call_tmp_chpl14 = this_chpl26(&block_chpl, parDim_chpl, INT64(303), INT32(65));
#line 303 "../lib/Chai.chpl"
  (call_tmp_chpl14)->_high = tmp_x1_chpl;
#line 303 "../lib/Chai.chpl"
  *(retcopy_chpl + INT64(0)) = *(block_chpl + INT64(0));
#line 303 "../lib/Chai.chpl"
  *(retcopy_chpl + INT64(1)) = *(block_chpl + INT64(1));
#line 303 "../lib/Chai.chpl"
  *(coerce_tmp_chpl4 + INT64(0)) = *(retcopy_chpl + INT64(0));
#line 303 "../lib/Chai.chpl"
  *(coerce_tmp_chpl4 + INT64(1)) = *(retcopy_chpl + INT64(1));
#line 303 "../lib/Chai.chpl"
  coerce_tmp_chpl5 = *(coerce_tmp_chpl4 + INT64(0));
#line 303 "../lib/Chai.chpl"
  _ic__F0_this_chpl = coerce_tmp_chpl5;
#line 303 "../lib/Chai.chpl"
  checkIfIterWillOverflow_chpl(coerce_tmp_chpl5, UINT8(true), INT64(303), INT32(65));
#line 303 "../lib/Chai.chpl"
  i_chpl = INT64(0);
#line 303 "../lib/Chai.chpl"
  this_chpl32 = _ic__F0_this_chpl;
#line 303 "../lib/Chai.chpl"
  this_chpl33 = _ic__F0_this_chpl;
#line 303 "../lib/Chai.chpl"
  end_chpl = (&this_chpl33)->_high;
#line 303 "../lib/Chai.chpl"
  for (i_chpl = (&this_chpl32)->_low; ((i_chpl <= end_chpl)); i_chpl += INT64(1)) {
#line 303 "../lib/Chai.chpl"
    i_chpl2 = i_chpl;
#line 303 "../lib/Chai.chpl"
    coerce_tmp_chpl6 = *(coerce_tmp_chpl4 + INT64(1));
#line 303 "../lib/Chai.chpl"
    _ic__F0_this_chpl2 = coerce_tmp_chpl6;
#line 303 "../lib/Chai.chpl"
    checkIfIterWillOverflow_chpl(coerce_tmp_chpl6, UINT8(true), INT64(303), INT32(65));
#line 303 "../lib/Chai.chpl"
    i_chpl3 = INT64(0);
#line 303 "../lib/Chai.chpl"
    this_chpl34 = _ic__F0_this_chpl2;
#line 303 "../lib/Chai.chpl"
    this_chpl35 = _ic__F0_this_chpl2;
#line 303 "../lib/Chai.chpl"
    end_chpl2 = (&this_chpl35)->_high;
#line 303 "../lib/Chai.chpl"
    for (i_chpl3 = (&this_chpl34)->_low; ((i_chpl3 <= end_chpl2)); i_chpl3 += INT64(1)) {
#line 303 "../lib/Chai.chpl"
      *(ret_tmp_chpl2 + INT64(0)) = i_chpl2;
#line 303 "../lib/Chai.chpl"
      *(ret_tmp_chpl2 + INT64(1)) = i_chpl3;
#line 303 "../lib/Chai.chpl"
      *(yret_chpl + INT64(0)) = *(ret_tmp_chpl2 + INT64(0));
#line 303 "../lib/Chai.chpl"
      *(yret_chpl + INT64(1)) = *(ret_tmp_chpl2 + INT64(1));
#line 303 "../lib/Chai.chpl"
      chpl_idx_1_x0 = *(yret_chpl + INT64(0));
#line 303 "../lib/Chai.chpl"
      chpl_idx_1_x1 = *(yret_chpl + INT64(1));
#line 304 "../lib/Chai.chpl"
      *(ret_tmp_chpl3 + INT64(0)) = kh_chpl;
#line 304 "../lib/Chai.chpl"
      *(ret_tmp_chpl3 + INT64(1)) = kw_chpl;
#line 304 "../lib/Chai.chpl"
      *(call_tmp_chpl15 + INT64(0)) = *(ret_tmp_chpl3 + INT64(0));
#line 304 "../lib/Chai.chpl"
      *(call_tmp_chpl15 + INT64(1)) = *(ret_tmp_chpl3 + INT64(1));
#line 304 "../lib/Chai.chpl"
      *(ret_tmp_chpl4 + INT64(0)) = m_chpl;
#line 304 "../lib/Chai.chpl"
      *(ret_tmp_chpl4 + INT64(1)) = n_chpl;
#line 304 "../lib/Chai.chpl"
      *(call_tmp_chpl16 + INT64(0)) = *(ret_tmp_chpl4 + INT64(0));
#line 304 "../lib/Chai.chpl"
      *(call_tmp_chpl16 + INT64(1)) = *(ret_tmp_chpl4 + INT64(1));
#line 304 "../lib/Chai.chpl"
      *(ret_tmp_chpl5 + INT64(0)) = chpl_idx_1_x0;
#line 304 "../lib/Chai.chpl"
      *(ret_tmp_chpl5 + INT64(1)) = chpl_idx_1_x1;
#line 304 "../lib/Chai.chpl"
      *(call_tmp_chpl17 + INT64(0)) = *(ret_tmp_chpl5 + INT64(0));
#line 304 "../lib/Chai.chpl"
      *(call_tmp_chpl17 + INT64(1)) = *(ret_tmp_chpl5 + INT64(1));
#line 304 "../lib/Chai.chpl"
      correlateWeightIdx_chpl(&call_tmp_chpl15, &call_tmp_chpl16, &call_tmp_chpl17, stride_svar_chpl, padding_svar_chpl, &ret_tmp_chpl6);
#line 304 "../lib/Chai.chpl"
      *(call_tmp_chpl18 + INT64(0)) = *(ret_tmp_chpl6 + INT64(0));
#line 304 "../lib/Chai.chpl"
      *(call_tmp_chpl18 + INT64(1)) = *(ret_tmp_chpl6 + INT64(1));
#line 304 "../lib/Chai.chpl"
      tmp_x0_chpl2 = *(call_tmp_chpl18 + INT64(0));
#line 304 "../lib/Chai.chpl"
      tmp_x1_chpl2 = *(call_tmp_chpl18 + INT64(1));
#line 305 "../lib/Chai.chpl"
      if (tmp_x0_chpl2 != INT64(-1)) /* ZLINE: 305 ../lib/Chai.chpl */
#line 305 "../lib/Chai.chpl"
      {
#line 306 "../lib/Chai.chpl"
        call_tmp_chpl19 = &((delta_chpl)->data);
#line 306 "../lib/Chai.chpl"
        *(coerce_tmp_chpl7 + INT64(0)) = chpl_idx_1_x0;
#line 306 "../lib/Chai.chpl"
        *(coerce_tmp_chpl7 + INT64(1)) = chpl_idx_1_x1;
#line 306 "../lib/Chai.chpl"
        *(coerce_tmp_chpl7 + INT64(2)) = co_chpl;
#line 306 "../lib/Chai.chpl"
        i_i_chpl = &coerce_tmp_chpl7;
#line 306 "../lib/Chai.chpl"
        ret_chpl2 = (call_tmp_chpl19)->_instance;
#line 306 "../lib/Chai.chpl"
        checkAccess_chpl9(call_tmp_chpl19, i_i_chpl, ret_chpl2, INT64(306), INT32(65));
#line 306 "../lib/Chai.chpl"
        coerce_tmp_chpl8 = ret_chpl2;
#line 306 "../lib/Chai.chpl"
        *(_formal_tmp_in_ind_chpl + INT64(0)) = *(coerce_tmp_chpl7 + INT64(0));
#line 306 "../lib/Chai.chpl"
        *(_formal_tmp_in_ind_chpl + INT64(1)) = *(coerce_tmp_chpl7 + INT64(1));
#line 306 "../lib/Chai.chpl"
        *(_formal_tmp_in_ind_chpl + INT64(2)) = *(coerce_tmp_chpl7 + INT64(2));
#line 306 "../lib/Chai.chpl"
        _formal_tmp_ind_x0_chpl = *(_formal_tmp_in_ind_chpl + INT64(0));
#line 306 "../lib/Chai.chpl"
        _formal_tmp_ind_x1_chpl = *(_formal_tmp_in_ind_chpl + INT64(1));
#line 306 "../lib/Chai.chpl"
        _formal_tmp_ind_x2_chpl = *(_formal_tmp_in_ind_chpl + INT64(2));
#line 306 "../lib/Chai.chpl"
        sum_chpl = INT64(0);
#line 306 "../lib/Chai.chpl"
        chpl_check_nil(coerce_tmp_chpl8, INT64(306), INT32(65));
#line 306 "../lib/Chai.chpl"
        call_tmp_chpl20 = &((coerce_tmp_chpl8)->blk);
#line 306 "../lib/Chai.chpl"
        i_lhs_chpl = &sum_chpl;
#line 306 "../lib/Chai.chpl"
        *(i_lhs_chpl) += ((int64_t)((_formal_tmp_ind_x0_chpl * *(*(call_tmp_chpl20) + INT64(0)))));
#line 306 "../lib/Chai.chpl"
        chpl_check_nil(coerce_tmp_chpl8, INT64(306), INT32(65));
#line 306 "../lib/Chai.chpl"
        call_tmp_chpl21 = &((coerce_tmp_chpl8)->blk);
#line 306 "../lib/Chai.chpl"
        i_lhs_chpl2 = &sum_chpl;
#line 306 "../lib/Chai.chpl"
        *(i_lhs_chpl2) += ((int64_t)((_formal_tmp_ind_x1_chpl * *(*(call_tmp_chpl21) + INT64(1)))));
#line 306 "../lib/Chai.chpl"
        i_lhs_chpl3 = &sum_chpl;
#line 306 "../lib/Chai.chpl"
        *(i_lhs_chpl3) += _formal_tmp_ind_x2_chpl;
#line 306 "../lib/Chai.chpl"
        ret_chpl4 = sum_chpl;
#line 306 "../lib/Chai.chpl"
        chpl_check_nil(coerce_tmp_chpl8, INT64(306), INT32(65));
#line 306 "../lib/Chai.chpl"
        coerce_tmp_chpl9 = (coerce_tmp_chpl8)->shiftedData;
#line 306 "../lib/Chai.chpl"
        call_tmp_chpl22 = (coerce_tmp_chpl9 + ret_chpl4);
#line 306 "../lib/Chai.chpl"
        ret_chpl3 = *(call_tmp_chpl22);
#line 306 "../lib/Chai.chpl"
        call_tmp_chpl23 = &((filters_svar_chpl)->data);
#line 306 "../lib/Chai.chpl"
        *(coerce_tmp_chpl10 + INT64(0)) = co_chpl;
#line 306 "../lib/Chai.chpl"
        *(coerce_tmp_chpl10 + INT64(1)) = tmp_x0_chpl2;
#line 306 "../lib/Chai.chpl"
        *(coerce_tmp_chpl10 + INT64(2)) = tmp_x1_chpl2;
#line 306 "../lib/Chai.chpl"
        *(coerce_tmp_chpl10 + INT64(3)) = ci_chpl;
#line 306 "../lib/Chai.chpl"
        i_i_chpl2 = &coerce_tmp_chpl10;
#line 306 "../lib/Chai.chpl"
        ret_chpl5 = (call_tmp_chpl23)->_instance;
#line 306 "../lib/Chai.chpl"
        checkAccess_chpl3(call_tmp_chpl23, i_i_chpl2, ret_chpl5, INT64(306), INT32(65));
#line 306 "../lib/Chai.chpl"
        coerce_tmp_chpl11 = ret_chpl5;
#line 306 "../lib/Chai.chpl"
        *(_formal_tmp_in_ind_chpl2 + INT64(0)) = *(coerce_tmp_chpl10 + INT64(0));
#line 306 "../lib/Chai.chpl"
        *(_formal_tmp_in_ind_chpl2 + INT64(1)) = *(coerce_tmp_chpl10 + INT64(1));
#line 306 "../lib/Chai.chpl"
        *(_formal_tmp_in_ind_chpl2 + INT64(2)) = *(coerce_tmp_chpl10 + INT64(2));
#line 306 "../lib/Chai.chpl"
        *(_formal_tmp_in_ind_chpl2 + INT64(3)) = *(coerce_tmp_chpl10 + INT64(3));
#line 306 "../lib/Chai.chpl"
        _formal_tmp_ind_x0_chpl2 = *(_formal_tmp_in_ind_chpl2 + INT64(0));
#line 306 "../lib/Chai.chpl"
        _formal_tmp_ind_x1_chpl2 = *(_formal_tmp_in_ind_chpl2 + INT64(1));
#line 306 "../lib/Chai.chpl"
        _formal_tmp_ind_x2_chpl2 = *(_formal_tmp_in_ind_chpl2 + INT64(2));
#line 306 "../lib/Chai.chpl"
        _formal_tmp_ind_x3_chpl = *(_formal_tmp_in_ind_chpl2 + INT64(3));
#line 306 "../lib/Chai.chpl"
        sum_chpl2 = INT64(0);
#line 306 "../lib/Chai.chpl"
        chpl_check_nil(coerce_tmp_chpl11, INT64(306), INT32(65));
#line 306 "../lib/Chai.chpl"
        call_tmp_chpl24 = &((coerce_tmp_chpl11)->blk);
#line 306 "../lib/Chai.chpl"
        i_lhs_chpl4 = &sum_chpl2;
#line 306 "../lib/Chai.chpl"
        *(i_lhs_chpl4) += ((int64_t)((_formal_tmp_ind_x0_chpl2 * *(*(call_tmp_chpl24) + INT64(0)))));
#line 306 "../lib/Chai.chpl"
        chpl_check_nil(coerce_tmp_chpl11, INT64(306), INT32(65));
#line 306 "../lib/Chai.chpl"
        call_tmp_chpl25 = &((coerce_tmp_chpl11)->blk);
#line 306 "../lib/Chai.chpl"
        i_lhs_chpl5 = &sum_chpl2;
#line 306 "../lib/Chai.chpl"
        *(i_lhs_chpl5) += ((int64_t)((_formal_tmp_ind_x1_chpl2 * *(*(call_tmp_chpl25) + INT64(1)))));
#line 306 "../lib/Chai.chpl"
        chpl_check_nil(coerce_tmp_chpl11, INT64(306), INT32(65));
#line 306 "../lib/Chai.chpl"
        call_tmp_chpl26 = &((coerce_tmp_chpl11)->blk);
#line 306 "../lib/Chai.chpl"
        i_lhs_chpl6 = &sum_chpl2;
#line 306 "../lib/Chai.chpl"
        *(i_lhs_chpl6) += ((int64_t)((_formal_tmp_ind_x2_chpl2 * *(*(call_tmp_chpl26) + INT64(2)))));
#line 306 "../lib/Chai.chpl"
        i_lhs_chpl7 = &sum_chpl2;
#line 306 "../lib/Chai.chpl"
        *(i_lhs_chpl7) += _formal_tmp_ind_x3_chpl;
#line 306 "../lib/Chai.chpl"
        ret_chpl6 = sum_chpl2;
#line 306 "../lib/Chai.chpl"
        chpl_check_nil(coerce_tmp_chpl11, INT64(306), INT32(65));
#line 306 "../lib/Chai.chpl"
        coerce_tmp_chpl12 = (coerce_tmp_chpl11)->shiftedData;
#line 306 "../lib/Chai.chpl"
        call_tmp_chpl27 = (coerce_tmp_chpl12 + ret_chpl6);
#line 306 "../lib/Chai.chpl"
        i_lhs_chpl8 = &AS_sum_chpl;
#line 306 "../lib/Chai.chpl"
        *(i_lhs_chpl8) += ((_real64)((ret_chpl3 * *(call_tmp_chpl27))));
#line 306 "../lib/Chai.chpl"
      }
#line 306 "../lib/Chai.chpl"
    }
#line 306 "../lib/Chai.chpl"
  }
#line 303 "../lib/Chai.chpl"
  coerce_tmp_chpl13 = RP_sum_chpl2;
#line 303 "../lib/Chai.chpl"
  chpl_check_nil(coerce_tmp_chpl13, INT64(303), INT32(65));
#line 303 "../lib/Chai.chpl"
  call_tmp_chpl28 = &((coerce_tmp_chpl13)->value);
#line 303 "../lib/Chai.chpl"
  *(call_tmp_chpl28) += AS_sum_chpl;
#line 303 "../lib/Chai.chpl"
  chpl__reduceCombine6(RP_sum_chpl, RP_sum_chpl2, INT64(303), INT32(65));
#line 303 "../lib/Chai.chpl"
  chpl__delete33(RP_sum_chpl2, INT64(303), INT32(65));
#line 303 "../lib/Chai.chpl"
  return;
#line 303 "../lib/Chai.chpl"
}

#line 317 "../lib/Chai.chpl"
/* Chai.chpl:317 */
#line 317 "../lib/Chai.chpl"
static void optimize_chpl(Conv_chpl * this_chpl31,
#line 317 "../lib/Chai.chpl"
                          _real64 mag_chpl) {
#line 318 "../lib/Chai.chpl"
  _ref_Tensor_4__real64_chpl call_tmp_chpl6 = NULL;
#line 318 "../lib/Chai.chpl"
  _ref_Tensor_4__real64_chpl call_tmp_chpl7 = NULL;
#line 318 "../lib/Chai.chpl"
  Tensor_4__real64_chpl call_tmp_chpl8;
#line 318 "../lib/Chai.chpl"
  Tensor_4__real64_chpl ret_tmp_chpl;
#line 317 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_4_int64_t_one__real64_int64_t_chpl _field_destructor_tmp__chpl;
#line 317 "../lib/Chai.chpl"
  _domain_DefaultRectangularDom_4_int64_t_one_chpl _field_destructor_tmp__chpl2;
#line 318 "../lib/Chai.chpl"
  call_tmp_chpl6 = &((this_chpl31)->filters);
#line 318 "../lib/Chai.chpl"
  call_tmp_chpl7 = &((this_chpl31)->filtersGrad);
#line 318 "../lib/Chai.chpl"
  chpl___ASTERISK_(mag_chpl, call_tmp_chpl7, &ret_tmp_chpl);
#line 318 "../lib/Chai.chpl"
  call_tmp_chpl8 = ret_tmp_chpl;
#line 318 "../lib/Chai.chpl"
  chpl___HYPHEN__ASSIGN_2(call_tmp_chpl6, &call_tmp_chpl8);
#line 317 "../lib/Chai.chpl"
  _field_destructor_tmp__chpl = (&call_tmp_chpl8)->data;
#line 317 "../lib/Chai.chpl"
  chpl__autoDestroy25(&_field_destructor_tmp__chpl, INT64(317), INT32(65));
#line 317 "../lib/Chai.chpl"
  _field_destructor_tmp__chpl2 = (&call_tmp_chpl8)->_domain;
#line 317 "../lib/Chai.chpl"
  chpl__autoDestroy9(&_field_destructor_tmp__chpl2, INT64(317), INT32(65));
#line 317 "../lib/Chai.chpl"
  return;
#line 317 "../lib/Chai.chpl"
}

#line 321 "../lib/Chai.chpl"
/* Chai.chpl:321 */
#line 321 "../lib/Chai.chpl"
static void resetGradients_chpl(Conv_chpl * this_chpl31) {
#line 322 "../lib/Chai.chpl"
  _ref_Tensor_4__real64_chpl call_tmp_chpl6 = NULL;
#line 322 "../lib/Chai.chpl"
  _ref__array_DefaultRectangularArr_4_int64_t_one__real64_int64_t_chpl call_tmp_chpl7 = NULL;
#line 322 "../lib/Chai.chpl"
  call_tmp_chpl6 = &((this_chpl31)->filtersGrad);
#line 322 "../lib/Chai.chpl"
  call_tmp_chpl7 = &((call_tmp_chpl6)->data);
#line 322 "../lib/Chai.chpl"
  chpl___ASSIGN_2(call_tmp_chpl7, REAL64(0x0p+0), INT64(322), INT32(65));
#line 321 "../lib/Chai.chpl"
  return;
#line 321 "../lib/Chai.chpl"
}

#line 325 "../lib/Chai.chpl"
/* Chai.chpl:325 */
#line 325 "../lib/Chai.chpl"
static void write_chpl2(Conv_chpl * this_chpl31,
#line 325 "../lib/Chai.chpl"
                        fileWriter_T_BinarySerializer_chpl * fw_chpl,
#line 325 "../lib/Chai.chpl"
                        Error_chpl * error_out_chpl) {
#line 326 "../lib/Chai.chpl"
  locale_chpl local_nilLocale_chpl;
#line 326 "../lib/Chai.chpl"
  int64_t coerce_tmp_chpl3;
#line 326 "../lib/Chai.chpl"
  Error_chpl error_chpl = NULL;
#line 326 "../lib/Chai.chpl"
  locale_chpl ret_chpl;
#line 326 "../lib/Chai.chpl"
  _ref_locale_chpl call_tmp_chpl6 = NULL;
#line 326 "../lib/Chai.chpl"
  locale_chpl ret_chpl2;
#line 326 "../lib/Chai.chpl"
  chpl_bool call_tmp_chpl7;
#line 326 "../lib/Chai.chpl"
  locale_chpl call_tmp_chpl8;
#line 326 "../lib/Chai.chpl"
  chpl_localeID_t call_tmp_chpl9;
#line 326 "../lib/Chai.chpl"
  int32_t call_tmp_chpl10;
#line 326 "../lib/Chai.chpl"
  chpl_localeID_t call_tmp_chpl11;
#line 326 "../lib/Chai.chpl"
  locale_chpl call_tmp_chpl12;
#line 326 "../lib/Chai.chpl"
  locale_chpl ret_tmp_chpl;
#line 326 "../lib/Chai.chpl"
  _ref_locale_chpl i_x_chpl = NULL;
#line 326 "../lib/Chai.chpl"
  locale_chpl origLocale_chpl;
#line 326 "../lib/Chai.chpl"
  _ref_locale_chpl call_tmp_chpl13 = NULL;
#line 326 "../lib/Chai.chpl"
  Error_chpl error_chpl2 = NULL;
#line 326 "../lib/Chai.chpl"
  _owned_SystemError_chpl call_tmp_chpl14;
#line 326 "../lib/Chai.chpl"
  _owned_SystemError_chpl ret_tmp_chpl2;
#line 326 "../lib/Chai.chpl"
  Error_chpl call_tmp_chpl15 = NULL;
#line 326 "../lib/Chai.chpl"
  _owned_Error_chpl coerce_tmp_chpl4;
#line 326 "../lib/Chai.chpl"
  _owned_Error_chpl ret_tmp_chpl3;
#line 326 "../lib/Chai.chpl"
  _ref__owned_SystemError_chpl i_x_chpl2 = NULL;
#line 326 "../lib/Chai.chpl"
  _ref__owned_Error_chpl2 i__retArg_chpl = NULL;
#line 326 "../lib/Chai.chpl"
  _owned_SystemError_chpl _formal_tmp_x_chpl;
#line 326 "../lib/Chai.chpl"
  _owned_Error_chpl ret_chpl3;
#line 326 "../lib/Chai.chpl"
  _ref__owned_SystemError_chpl i_this_chpl = NULL;
#line 326 "../lib/Chai.chpl"
  Error_chpl call_tmp_chpl16 = NULL;
#line 326 "../lib/Chai.chpl"
  _ref__owned_SystemError_chpl i_this_chpl2 = NULL;
#line 326 "../lib/Chai.chpl"
  _owned_Error_chpl call_tmp_chpl17;
#line 326 "../lib/Chai.chpl"
  _owned_Error_chpl initTemp_chpl;
#line 326 "../lib/Chai.chpl"
  _owned_Error_chpl _formal_tmp_in_err_chpl;
#line 326 "../lib/Chai.chpl"
  _ref_locale_chpl call_tmp_chpl18 = NULL;
#line 326 "../lib/Chai.chpl"
  syserr call_tmp_chpl19;
#line 326 "../lib/Chai.chpl"
  int32_t call_tmp_chpl20;
#line 326 "../lib/Chai.chpl"
  Error_chpl error_chpl3 = NULL;
#line 326 "../lib/Chai.chpl"
  _ref_locale_chpl i_x_chpl3 = NULL;
#line 326 "../lib/Chai.chpl"
  Error_chpl error_chpl4 = NULL;
#line 326 "../lib/Chai.chpl"
  _ref_locale_chpl call_tmp_chpl21 = NULL;
#line 326 "../lib/Chai.chpl"
  _ref_locale_chpl i_x_chpl4 = NULL;
#line 326 "../lib/Chai.chpl"
  _ref_locale_chpl call_tmp_chpl22 = NULL;
#line 326 "../lib/Chai.chpl"
  _ref_locale_chpl i_x_chpl5 = NULL;
#line 327 "../lib/Chai.chpl"
  _ref_Tensor_4__real64_chpl call_tmp_chpl23 = NULL;
#line 327 "../lib/Chai.chpl"
  Error_chpl error_chpl5 = NULL;
#line 326 "../lib/Chai.chpl"
  local_nilLocale_chpl = nilLocale_chpl;
#line 326 "../lib/Chai.chpl"
  coerce_tmp_chpl3 = (this_chpl31)->numFilters;
#line 326 "../lib/Chai.chpl"
  error_chpl = NULL;
#line 326 "../lib/Chai.chpl"
  call_tmp_chpl6 = &((fw_chpl)->_readWriteThisFromLocale);
#line 326 "../lib/Chai.chpl"
  init_ASSIGN__chpl(&ret_chpl2, call_tmp_chpl6);
#line 326 "../lib/Chai.chpl"
  call_tmp_chpl7 = chpl___EQUALS_(&ret_chpl2, &local_nilLocale_chpl, INT64(326), INT32(65));
#line 326 "../lib/Chai.chpl"
  if (call_tmp_chpl7) /* ZLINE: 326 ../lib/Chai.chpl */
#line 326 "../lib/Chai.chpl"
  {
#line 326 "../lib/Chai.chpl"
    call_tmp_chpl10 = get_chpl_nodeID();
#line 326 "../lib/Chai.chpl"
    call_tmp_chpl11 = chpl_rt_buildLocaleID(call_tmp_chpl10, c_sublocid_any);
#line 326 "../lib/Chai.chpl"
    call_tmp_chpl9 = call_tmp_chpl11;
#line 326 "../lib/Chai.chpl"
    chpl_localeID_to_locale(&call_tmp_chpl9, &ret_tmp_chpl, INT64(326), INT32(65));
#line 326 "../lib/Chai.chpl"
    call_tmp_chpl12 = ret_tmp_chpl;
#line 326 "../lib/Chai.chpl"
    call_tmp_chpl8 = call_tmp_chpl12;
#line 326 "../lib/Chai.chpl"
    chpl___ASSIGN_9(&ret_chpl2, &call_tmp_chpl8);
#line 326 "../lib/Chai.chpl"
    i_x_chpl = &call_tmp_chpl8;
#line 326 "../lib/Chai.chpl"
    deinit_chpl62(i_x_chpl);
#line 326 "../lib/Chai.chpl"
  }
#line 326 "../lib/Chai.chpl"
  ret_chpl = ret_chpl2;
#line 326 "../lib/Chai.chpl"
  origLocale_chpl = ret_chpl;
#line 326 "../lib/Chai.chpl"
  call_tmp_chpl13 = &((fw_chpl)->_home);
#line 326 "../lib/Chai.chpl"
  if (((RootClass_chpl)((call_tmp_chpl13)->_instance)) == nil) /* ZLINE: 326 ../lib/Chai.chpl */
#line 326 "../lib/Chai.chpl"
  {
#line 326 "../lib/Chai.chpl"
    nilCheckHalt_chpl(&_str_literal_52_chpl, INT64(326), INT32(65));
#line 326 "../lib/Chai.chpl"
  }
#line 326 "../lib/Chai.chpl"
  error_chpl2 = NULL;
#line 326 "../lib/Chai.chpl"
  qio_int_to_err(((int32_t)(INT64(0))));
#line 326 "../lib/Chai.chpl"
  if ((fw_chpl)->_channel_internal == nil) /* ZLINE: 326 ../lib/Chai.chpl */
#line 326 "../lib/Chai.chpl"
  {
#line 326 "../lib/Chai.chpl"
    createSystemError_chpl2(((int64_t)(EINVAL)), &_str_literal_1920_chpl, &ret_tmp_chpl2, INT64(326), INT32(65));
#line 326 "../lib/Chai.chpl"
    call_tmp_chpl14 = ret_tmp_chpl2;
#line 326 "../lib/Chai.chpl"
    i_x_chpl2 = &call_tmp_chpl14;
#line 326 "../lib/Chai.chpl"
    i__retArg_chpl = &ret_tmp_chpl3;
#line 326 "../lib/Chai.chpl"
    _formal_tmp_x_chpl = *(i_x_chpl2);
#line 326 "../lib/Chai.chpl"
    i_this_chpl = &_formal_tmp_x_chpl;
#line 326 "../lib/Chai.chpl"
    call_tmp_chpl16 = ((Error_chpl)((i_this_chpl)->chpl_p));
#line 326 "../lib/Chai.chpl"
    i_this_chpl2 = &_formal_tmp_x_chpl;
#line 326 "../lib/Chai.chpl"
    (i_this_chpl2)->chpl_p = ((SystemError_chpl)(nil));
#line 326 "../lib/Chai.chpl"
    if (((RootClass_chpl)(call_tmp_chpl16)) == nil) /* ZLINE: 326 ../lib/Chai.chpl */
#line 326 "../lib/Chai.chpl"
    {
#line 326 "../lib/Chai.chpl"
      nilCheckHalt_chpl(&_str_literal_52_chpl, INT64(326), INT32(65));
#line 326 "../lib/Chai.chpl"
    }
#line 326 "../lib/Chai.chpl"
    init_chpl188(&initTemp_chpl, call_tmp_chpl16);
#line 326 "../lib/Chai.chpl"
    call_tmp_chpl17 = initTemp_chpl;
#line 326 "../lib/Chai.chpl"
    ret_chpl3 = call_tmp_chpl17;
#line 326 "../lib/Chai.chpl"
    chpl__autoDestroy54(&_formal_tmp_x_chpl, INT64(326), INT32(65));
#line 326 "../lib/Chai.chpl"
    *(i__retArg_chpl) = ret_chpl3;
#line 326 "../lib/Chai.chpl"
    coerce_tmp_chpl4 = ret_tmp_chpl3;
#line 326 "../lib/Chai.chpl"
    _formal_tmp_in_err_chpl = coerce_tmp_chpl4;
#line 326 "../lib/Chai.chpl"
    call_tmp_chpl15 = chpl_fix_thrown_error(&_formal_tmp_in_err_chpl, INT64(326), INT32(65));
#line 326 "../lib/Chai.chpl"
    error_chpl2 = call_tmp_chpl15;
#line 326 "../lib/Chai.chpl"
    goto _endlock_chpl;
#line 326 "../lib/Chai.chpl"
  }
#line 326 "../lib/Chai.chpl"
  call_tmp_chpl18 = &((fw_chpl)->_home);
#line 326 "../lib/Chai.chpl"
  if (((RootClass_chpl)((call_tmp_chpl18)->_instance)) == nil) /* ZLINE: 326 ../lib/Chai.chpl */
#line 326 "../lib/Chai.chpl"
  {
#line 326 "../lib/Chai.chpl"
    nilCheckHalt_chpl(&_str_literal_52_chpl, INT64(326), INT32(65));
#line 326 "../lib/Chai.chpl"
  }
#line 326 "../lib/Chai.chpl"
  call_tmp_chpl19 = qio_channel_lock((fw_chpl)->_channel_internal);
#line 326 "../lib/Chai.chpl"
  call_tmp_chpl20 = qio_err_iserr(call_tmp_chpl19);
#line 326 "../lib/Chai.chpl"
  if (call_tmp_chpl20 != INT32(0)) /* ZLINE: 326 ../lib/Chai.chpl */
#line 326 "../lib/Chai.chpl"
  {
#line 326 "../lib/Chai.chpl"
    error_chpl3 = NULL;
#line 326 "../lib/Chai.chpl"
    _ch_ioerror_chpl3(fw_chpl, call_tmp_chpl19, &_str_literal_1919_chpl, &error_chpl3, INT64(326), INT32(65));
#line 326 "../lib/Chai.chpl"
    if (error_chpl3 != nil) /* ZLINE: 326 ../lib/Chai.chpl */
#line 326 "../lib/Chai.chpl"
    {
#line 326 "../lib/Chai.chpl"
      goto handler_chpl;
#line 326 "../lib/Chai.chpl"
    }
#line 326 "../lib/Chai.chpl"
    handler_chpl:;
#line 326 "../lib/Chai.chpl"
    if (error_chpl3 != nil) /* ZLINE: 326 ../lib/Chai.chpl */
#line 326 "../lib/Chai.chpl"
    {
#line 326 "../lib/Chai.chpl"
      error_chpl2 = error_chpl3;
#line 326 "../lib/Chai.chpl"
      goto _endlock_chpl;
#line 326 "../lib/Chai.chpl"
    }
#line 326 "../lib/Chai.chpl"
  }
#line 326 "../lib/Chai.chpl"
  _endlock_chpl:;
#line 326 "../lib/Chai.chpl"
  if (error_chpl2 != nil) /* ZLINE: 326 ../lib/Chai.chpl */
#line 326 "../lib/Chai.chpl"
  {
#line 326 "../lib/Chai.chpl"
    goto handler_chpl2;
#line 326 "../lib/Chai.chpl"
  }
#line 326 "../lib/Chai.chpl"
  handler_chpl2:;
#line 326 "../lib/Chai.chpl"
  if (error_chpl2 != nil) /* ZLINE: 326 ../lib/Chai.chpl */
#line 326 "../lib/Chai.chpl"
  {
#line 326 "../lib/Chai.chpl"
    error_chpl = error_chpl2;
#line 326 "../lib/Chai.chpl"
    i_x_chpl3 = &origLocale_chpl;
#line 326 "../lib/Chai.chpl"
    deinit_chpl62(i_x_chpl3);
#line 326 "../lib/Chai.chpl"
    goto _endwrite_chpl;
#line 326 "../lib/Chai.chpl"
  }
#line 326 "../lib/Chai.chpl"
  error_chpl4 = NULL;
#line 326 "../lib/Chai.chpl"
  _serializeOne_chpl2(fw_chpl, coerce_tmp_chpl3, &origLocale_chpl, &error_chpl4, INT64(326), INT32(65));
#line 326 "../lib/Chai.chpl"
  if (error_chpl4 != nil) /* ZLINE: 326 ../lib/Chai.chpl */
#line 326 "../lib/Chai.chpl"
  {
#line 326 "../lib/Chai.chpl"
    error_chpl = error_chpl4;
#line 326 "../lib/Chai.chpl"
    call_tmp_chpl21 = &((fw_chpl)->_home);
#line 326 "../lib/Chai.chpl"
    if (((RootClass_chpl)((call_tmp_chpl21)->_instance)) == nil) /* ZLINE: 326 ../lib/Chai.chpl */
#line 326 "../lib/Chai.chpl"
    {
#line 326 "../lib/Chai.chpl"
      nilCheckHalt_chpl(&_str_literal_52_chpl, INT64(326), INT32(65));
#line 326 "../lib/Chai.chpl"
    }
#line 326 "../lib/Chai.chpl"
    qio_channel_unlock((fw_chpl)->_channel_internal);
#line 326 "../lib/Chai.chpl"
    i_x_chpl4 = &origLocale_chpl;
#line 326 "../lib/Chai.chpl"
    deinit_chpl62(i_x_chpl4);
#line 326 "../lib/Chai.chpl"
    goto _endwrite_chpl;
#line 326 "../lib/Chai.chpl"
  }
#line 326 "../lib/Chai.chpl"
  call_tmp_chpl22 = &((fw_chpl)->_home);
#line 326 "../lib/Chai.chpl"
  if (((RootClass_chpl)((call_tmp_chpl22)->_instance)) == nil) /* ZLINE: 326 ../lib/Chai.chpl */
#line 326 "../lib/Chai.chpl"
  {
#line 326 "../lib/Chai.chpl"
    nilCheckHalt_chpl(&_str_literal_52_chpl, INT64(326), INT32(65));
#line 326 "../lib/Chai.chpl"
  }
#line 326 "../lib/Chai.chpl"
  qio_channel_unlock((fw_chpl)->_channel_internal);
#line 326 "../lib/Chai.chpl"
  i_x_chpl5 = &origLocale_chpl;
#line 326 "../lib/Chai.chpl"
  deinit_chpl62(i_x_chpl5);
#line 326 "../lib/Chai.chpl"
  _endwrite_chpl:;
#line 326 "../lib/Chai.chpl"
  if (error_chpl != nil) /* ZLINE: 326 ../lib/Chai.chpl */
#line 326 "../lib/Chai.chpl"
  {
#line 326 "../lib/Chai.chpl"
    *(error_out_chpl) = error_chpl;
#line 326 "../lib/Chai.chpl"
    goto _endwrite_chpl2;
#line 326 "../lib/Chai.chpl"
  }
#line 327 "../lib/Chai.chpl"
  call_tmp_chpl23 = &((this_chpl31)->filters);
#line 327 "../lib/Chai.chpl"
  error_chpl5 = NULL;
#line 327 "../lib/Chai.chpl"
  write_chpl7(call_tmp_chpl23, fw_chpl, &error_chpl5);
#line 327 "../lib/Chai.chpl"
  if (error_chpl5 != nil) /* ZLINE: 327 ../lib/Chai.chpl */
#line 327 "../lib/Chai.chpl"
  {
#line 327 "../lib/Chai.chpl"
    *(error_out_chpl) = error_chpl5;
#line 327 "../lib/Chai.chpl"
    goto _endwrite_chpl2;
#line 327 "../lib/Chai.chpl"
  }
#line 327 "../lib/Chai.chpl"
  _endwrite_chpl2:;
#line 325 "../lib/Chai.chpl"
  return;
#line 325 "../lib/Chai.chpl"
}

#line 336 "../lib/Chai.chpl"
/* Chai.chpl:336 */
#line 336 "../lib/Chai.chpl"
static void signature_chpl(Conv_chpl * this_chpl31,
#line 336 "../lib/Chai.chpl"
                           string_chpl * _retArg_chpl) {
#line 337 "../lib/Chai.chpl"
  _ref_Tensor_4__real64_chpl call_tmp_chpl6 = NULL;
#line 337 "../lib/Chai.chpl"
  int64_t call_tmp_x0_chpl;
#line 337 "../lib/Chai.chpl"
  int64_t call_tmp_x2_chpl;
#line 337 "../lib/Chai.chpl"
  _tuple_4_star_int64_t_chpl ret_tmp_chpl;
#line 338 "../lib/Chai.chpl"
  string_chpl call_tmp_chpl7;
#line 338 "../lib/Chai.chpl"
  string_chpl ret_tmp_chpl2;
#line 338 "../lib/Chai.chpl"
  string_chpl call_tmp_chpl8;
#line 338 "../lib/Chai.chpl"
  string_chpl ret_tmp_chpl3;
#line 338 "../lib/Chai.chpl"
  string_chpl call_tmp_chpl9;
#line 338 "../lib/Chai.chpl"
  string_chpl ret_tmp_chpl4;
#line 338 "../lib/Chai.chpl"
  string_chpl call_tmp_chpl10;
#line 338 "../lib/Chai.chpl"
  string_chpl ret_tmp_chpl5;
#line 338 "../lib/Chai.chpl"
  string_chpl call_tmp_chpl11;
#line 338 "../lib/Chai.chpl"
  string_chpl ret_tmp_chpl6;
#line 338 "../lib/Chai.chpl"
  string_chpl call_tmp_chpl12;
#line 338 "../lib/Chai.chpl"
  string_chpl ret_tmp_chpl7;
#line 338 "../lib/Chai.chpl"
  string_chpl call_tmp_chpl13;
#line 338 "../lib/Chai.chpl"
  string_chpl ret_tmp_chpl8;
#line 338 "../lib/Chai.chpl"
  string_chpl call_tmp_chpl14;
#line 338 "../lib/Chai.chpl"
  string_chpl ret_tmp_chpl9;
#line 338 "../lib/Chai.chpl"
  string_chpl call_tmp_chpl15;
#line 338 "../lib/Chai.chpl"
  string_chpl ret_tmp_chpl10;
#line 338 "../lib/Chai.chpl"
  string_chpl call_tmp_chpl16;
#line 338 "../lib/Chai.chpl"
  string_chpl ret_tmp_chpl11;
#line 338 "../lib/Chai.chpl"
  string_chpl call_tmp_chpl17;
#line 338 "../lib/Chai.chpl"
  string_chpl ret_tmp_chpl12;
#line 338 "../lib/Chai.chpl"
  string_chpl call_tmp_chpl18;
#line 338 "../lib/Chai.chpl"
  string_chpl ret_tmp_chpl13;
#line 338 "../lib/Chai.chpl"
  string_chpl call_tmp_chpl19;
#line 338 "../lib/Chai.chpl"
  string_chpl ret_tmp_chpl14;
#line 338 "../lib/Chai.chpl"
  string_chpl call_tmp_chpl20;
#line 338 "../lib/Chai.chpl"
  string_chpl ret_tmp_chpl15;
#line 338 "../lib/Chai.chpl"
  string_chpl call_tmp_chpl21;
#line 338 "../lib/Chai.chpl"
  string_chpl ret_tmp_chpl16;
#line 338 "../lib/Chai.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 338 "../lib/Chai.chpl"
  _ref_string_chpl i_x_chpl2 = NULL;
#line 338 "../lib/Chai.chpl"
  _ref_string_chpl i_x_chpl3 = NULL;
#line 338 "../lib/Chai.chpl"
  _ref_string_chpl i_x_chpl4 = NULL;
#line 338 "../lib/Chai.chpl"
  _ref_string_chpl i_x_chpl5 = NULL;
#line 338 "../lib/Chai.chpl"
  _ref_string_chpl i_x_chpl6 = NULL;
#line 338 "../lib/Chai.chpl"
  _ref_string_chpl i_x_chpl7 = NULL;
#line 338 "../lib/Chai.chpl"
  _ref_string_chpl i_x_chpl8 = NULL;
#line 338 "../lib/Chai.chpl"
  _ref_string_chpl i_x_chpl9 = NULL;
#line 338 "../lib/Chai.chpl"
  _ref_string_chpl i_x_chpl10 = NULL;
#line 338 "../lib/Chai.chpl"
  _ref_string_chpl i_x_chpl11 = NULL;
#line 338 "../lib/Chai.chpl"
  _ref_string_chpl i_x_chpl12 = NULL;
#line 338 "../lib/Chai.chpl"
  _ref_string_chpl i_x_chpl13 = NULL;
#line 338 "../lib/Chai.chpl"
  _ref_string_chpl i_x_chpl14 = NULL;
#line 337 "../lib/Chai.chpl"
  call_tmp_chpl6 = &((this_chpl31)->filters);
#line 337 "../lib/Chai.chpl"
  shape_chpl5(call_tmp_chpl6, &ret_tmp_chpl);
#line 337 "../lib/Chai.chpl"
  call_tmp_x0_chpl = *(ret_tmp_chpl + INT64(0));
#line 337 "../lib/Chai.chpl"
  call_tmp_x2_chpl = *(ret_tmp_chpl + INT64(2));
#line 338 "../lib/Chai.chpl"
  chpl___COLON_25(*(ret_tmp_chpl + INT64(3)), &ret_tmp_chpl2, INT64(338), INT32(65));
#line 338 "../lib/Chai.chpl"
  call_tmp_chpl7 = ret_tmp_chpl2;
#line 338 "../lib/Chai.chpl"
  chpl___PLUS_(&_str_literal_1705_chpl, &call_tmp_chpl7, &ret_tmp_chpl3, INT64(338), INT32(65));
#line 338 "../lib/Chai.chpl"
  call_tmp_chpl8 = ret_tmp_chpl3;
#line 338 "../lib/Chai.chpl"
  chpl___PLUS_(&call_tmp_chpl8, &_str_literal_472_chpl, &ret_tmp_chpl4, INT64(338), INT32(65));
#line 338 "../lib/Chai.chpl"
  call_tmp_chpl9 = ret_tmp_chpl4;
#line 338 "../lib/Chai.chpl"
  chpl___COLON_25(call_tmp_x0_chpl, &ret_tmp_chpl5, INT64(338), INT32(65));
#line 338 "../lib/Chai.chpl"
  call_tmp_chpl10 = ret_tmp_chpl5;
#line 338 "../lib/Chai.chpl"
  chpl___PLUS_(&call_tmp_chpl9, &call_tmp_chpl10, &ret_tmp_chpl6, INT64(338), INT32(65));
#line 338 "../lib/Chai.chpl"
  call_tmp_chpl11 = ret_tmp_chpl6;
#line 338 "../lib/Chai.chpl"
  chpl___PLUS_(&call_tmp_chpl11, &_str_literal_1706_chpl, &ret_tmp_chpl7, INT64(338), INT32(65));
#line 338 "../lib/Chai.chpl"
  call_tmp_chpl12 = ret_tmp_chpl7;
#line 338 "../lib/Chai.chpl"
  chpl___COLON_25(call_tmp_x2_chpl, &ret_tmp_chpl8, INT64(338), INT32(65));
#line 338 "../lib/Chai.chpl"
  call_tmp_chpl13 = ret_tmp_chpl8;
#line 338 "../lib/Chai.chpl"
  chpl___PLUS_(&call_tmp_chpl12, &call_tmp_chpl13, &ret_tmp_chpl9, INT64(338), INT32(65));
#line 338 "../lib/Chai.chpl"
  call_tmp_chpl14 = ret_tmp_chpl9;
#line 338 "../lib/Chai.chpl"
  chpl___PLUS_(&call_tmp_chpl14, &_str_literal_1707_chpl, &ret_tmp_chpl10, INT64(338), INT32(65));
#line 338 "../lib/Chai.chpl"
  call_tmp_chpl15 = ret_tmp_chpl10;
#line 338 "../lib/Chai.chpl"
  chpl___COLON_25((this_chpl31)->stride, &ret_tmp_chpl11, INT64(338), INT32(65));
#line 338 "../lib/Chai.chpl"
  call_tmp_chpl16 = ret_tmp_chpl11;
#line 338 "../lib/Chai.chpl"
  chpl___PLUS_(&call_tmp_chpl15, &call_tmp_chpl16, &ret_tmp_chpl12, INT64(338), INT32(65));
#line 338 "../lib/Chai.chpl"
  call_tmp_chpl17 = ret_tmp_chpl12;
#line 338 "../lib/Chai.chpl"
  chpl___PLUS_(&call_tmp_chpl17, &_str_literal_1708_chpl, &ret_tmp_chpl13, INT64(338), INT32(65));
#line 338 "../lib/Chai.chpl"
  call_tmp_chpl18 = ret_tmp_chpl13;
#line 338 "../lib/Chai.chpl"
  chpl___COLON_25((this_chpl31)->padding, &ret_tmp_chpl14, INT64(338), INT32(65));
#line 338 "../lib/Chai.chpl"
  call_tmp_chpl19 = ret_tmp_chpl14;
#line 338 "../lib/Chai.chpl"
  chpl___PLUS_(&call_tmp_chpl18, &call_tmp_chpl19, &ret_tmp_chpl15, INT64(338), INT32(65));
#line 338 "../lib/Chai.chpl"
  call_tmp_chpl20 = ret_tmp_chpl15;
#line 338 "../lib/Chai.chpl"
  chpl___PLUS_(&call_tmp_chpl20, &_str_literal_415_chpl, &ret_tmp_chpl16, INT64(338), INT32(65));
#line 338 "../lib/Chai.chpl"
  call_tmp_chpl21 = ret_tmp_chpl16;
#line 338 "../lib/Chai.chpl"
  i_x_chpl = &call_tmp_chpl20;
#line 338 "../lib/Chai.chpl"
  deinit_chpl95(i_x_chpl, INT64(338), INT32(65));
#line 338 "../lib/Chai.chpl"
  i_x_chpl2 = &call_tmp_chpl19;
#line 338 "../lib/Chai.chpl"
  deinit_chpl95(i_x_chpl2, INT64(338), INT32(65));
#line 338 "../lib/Chai.chpl"
  i_x_chpl3 = &call_tmp_chpl18;
#line 338 "../lib/Chai.chpl"
  deinit_chpl95(i_x_chpl3, INT64(338), INT32(65));
#line 338 "../lib/Chai.chpl"
  i_x_chpl4 = &call_tmp_chpl17;
#line 338 "../lib/Chai.chpl"
  deinit_chpl95(i_x_chpl4, INT64(338), INT32(65));
#line 338 "../lib/Chai.chpl"
  i_x_chpl5 = &call_tmp_chpl16;
#line 338 "../lib/Chai.chpl"
  deinit_chpl95(i_x_chpl5, INT64(338), INT32(65));
#line 338 "../lib/Chai.chpl"
  i_x_chpl6 = &call_tmp_chpl15;
#line 338 "../lib/Chai.chpl"
  deinit_chpl95(i_x_chpl6, INT64(338), INT32(65));
#line 338 "../lib/Chai.chpl"
  i_x_chpl7 = &call_tmp_chpl14;
#line 338 "../lib/Chai.chpl"
  deinit_chpl95(i_x_chpl7, INT64(338), INT32(65));
#line 338 "../lib/Chai.chpl"
  i_x_chpl8 = &call_tmp_chpl13;
#line 338 "../lib/Chai.chpl"
  deinit_chpl95(i_x_chpl8, INT64(338), INT32(65));
#line 338 "../lib/Chai.chpl"
  i_x_chpl9 = &call_tmp_chpl12;
#line 338 "../lib/Chai.chpl"
  deinit_chpl95(i_x_chpl9, INT64(338), INT32(65));
#line 338 "../lib/Chai.chpl"
  i_x_chpl10 = &call_tmp_chpl11;
#line 338 "../lib/Chai.chpl"
  deinit_chpl95(i_x_chpl10, INT64(338), INT32(65));
#line 338 "../lib/Chai.chpl"
  i_x_chpl11 = &call_tmp_chpl10;
#line 338 "../lib/Chai.chpl"
  deinit_chpl95(i_x_chpl11, INT64(338), INT32(65));
#line 338 "../lib/Chai.chpl"
  i_x_chpl12 = &call_tmp_chpl9;
#line 338 "../lib/Chai.chpl"
  deinit_chpl95(i_x_chpl12, INT64(338), INT32(65));
#line 338 "../lib/Chai.chpl"
  i_x_chpl13 = &call_tmp_chpl8;
#line 338 "../lib/Chai.chpl"
  deinit_chpl95(i_x_chpl13, INT64(338), INT32(65));
#line 338 "../lib/Chai.chpl"
  i_x_chpl14 = &call_tmp_chpl7;
#line 338 "../lib/Chai.chpl"
  deinit_chpl95(i_x_chpl14, INT64(338), INT32(65));
#line 336 "../lib/Chai.chpl"
  *(_retArg_chpl) = call_tmp_chpl21;
#line 336 "../lib/Chai.chpl"
  return;
#line 336 "../lib/Chai.chpl"
}

#line 342 "../lib/Chai.chpl"
/* Chai.chpl:342 */
#line 342 "../lib/Chai.chpl"
static void init_chpl230(MaxPool_chpl * this_chpl31) {
#line 342 "../lib/Chai.chpl"
  return;
#line 342 "../lib/Chai.chpl"
}

#line 344 "../lib/Chai.chpl"
/* Chai.chpl:344 */
#line 344 "../lib/Chai.chpl"
static void forwardPropBatch_chpl2(MaxPool_chpl * this_chpl31,
#line 344 "../lib/Chai.chpl"
                                   _array_DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl * batch_chpl,
#line 344 "../lib/Chai.chpl"
                                   _array_DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl * _retArg_chpl) {
#line 347 "../lib/Chai.chpl"
  int64_t local_dataParMinGranularity_chpl;
#line 347 "../lib/Chai.chpl"
  chpl_bool local_dataParIgnoreRunningTasks_chpl;
#line 347 "../lib/Chai.chpl"
  int64_t local_dataParTasksPerLocale_chpl;
#line 345 "../lib/Chai.chpl"
  int64_t call_tmp_chpl6;
#line 346 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl pools_chpl;
#line 346 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl call_tmp_chpl7;
#line 346 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl ret_tmp_chpl;
#line 346 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl8;
#line 346 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl2;
#line 346 "../lib/Chai.chpl"
  _domain_DefaultRectangularDom_1_int64_t_one_chpl call_tmp_chpl9;
#line 346 "../lib/Chai.chpl"
  _domain_DefaultRectangularDom_1_int64_t_one_chpl ret_tmp_chpl3;
#line 346 "../lib/Chai.chpl"
  chpl___RuntimeTypeInfo6 call_tmp_chpl10;
#line 346 "../lib/Chai.chpl"
  _domain_DefaultRectangularDom_1_int64_t_one_chpl _runtime_type_tmp__chpl;
#line 346 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl ret_tmp_chpl4;
#line 347 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl call_tmp_chpl11;
#line 347 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl ret_tmp_chpl5;
#line 347 "../lib/Chai.chpl"
  DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl coerce_tmp_chpl3 = NULL;
#line 347 "../lib/Chai.chpl"
  int64_t tasksPerLocale_chpl;
#line 347 "../lib/Chai.chpl"
  chpl_bool ignoreRunning_chpl;
#line 347 "../lib/Chai.chpl"
  int64_t minIndicesPerTask_chpl;
#line 347 "../lib/Chai.chpl"
  Error_chpl error_chpl = NULL;
#line 347 "../lib/Chai.chpl"
  DefaultRectangularDom_1_int64_t_one_chpl coerce_tmp_chpl4 = NULL;
#line 347 "../lib/Chai.chpl"
  int64_t offset_x0_chpl;
#line 347 "../lib/Chai.chpl"
  _tuple_1_star_int64_t_chpl ret_tmp_chpl6;
#line 347 "../lib/Chai.chpl"
  Error_chpl error_chpl2 = NULL;
#line 347 "../lib/Chai.chpl"
  locale_chpl call_tmp_chpl12;
#line 347 "../lib/Chai.chpl"
  chpl_localeID_t call_tmp_chpl13;
#line 347 "../lib/Chai.chpl"
  int32_t call_tmp_chpl14;
#line 347 "../lib/Chai.chpl"
  chpl_localeID_t call_tmp_chpl15;
#line 347 "../lib/Chai.chpl"
  locale_chpl call_tmp_chpl16;
#line 347 "../lib/Chai.chpl"
  locale_chpl ret_tmp_chpl7;
#line 347 "../lib/Chai.chpl"
  BaseLocale_chpl coerce_tmp_chpl5 = NULL;
#line 347 "../lib/Chai.chpl"
  _ref_locale_chpl i_x_chpl = NULL;
#line 347 "../lib/Chai.chpl"
  int64_t tmp_chpl;
#line 347 "../lib/Chai.chpl"
  locale_chpl call_tmp_chpl17;
#line 347 "../lib/Chai.chpl"
  chpl_localeID_t call_tmp_chpl18;
#line 347 "../lib/Chai.chpl"
  int32_t call_tmp_chpl19;
#line 347 "../lib/Chai.chpl"
  chpl_localeID_t call_tmp_chpl20;
#line 347 "../lib/Chai.chpl"
  locale_chpl call_tmp_chpl21;
#line 347 "../lib/Chai.chpl"
  locale_chpl ret_tmp_chpl8;
#line 347 "../lib/Chai.chpl"
  int64_t ret_chpl;
#line 347 "../lib/Chai.chpl"
  BaseLocale_chpl coerce_tmp_chpl6 = NULL;
#line 347 "../lib/Chai.chpl"
  BaseLocale_chpl coerce_tmp_chpl7 = NULL;
#line 347 "../lib/Chai.chpl"
  _ref_locale_chpl i_x_chpl2 = NULL;
#line 347 "../lib/Chai.chpl"
  int64_t tmp_x0_chpl;
#line 347 "../lib/Chai.chpl"
  int64_t tmp_x1_chpl;
#line 347 "../lib/Chai.chpl"
  _ref__tuple_1_star_range_int64_t_both_one_chpl call_tmp_chpl22 = NULL;
#line 347 "../lib/Chai.chpl"
  _tuple_2_star_int64_t_chpl ret_tmp_chpl9;
#line 347 "../lib/Chai.chpl"
  int64_t numChunks_chpl;
#line 347 "../lib/Chai.chpl"
  int64_t parDim_chpl;
#line 347 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl locBlock_x0_chpl;
#line 347 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl tup_x0_chpl;
#line 347 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl elt_x0_chpl;
#line 347 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl call_tmp_chpl23;
#line 347 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl ret_tmp_chpl10;
#line 347 "../lib/Chai.chpl"
  _ref__tuple_1_star_range_int64_t_both_one_chpl call_tmp_chpl24 = NULL;
#line 347 "../lib/Chai.chpl"
  int64_t call_tmp_chpl25;
#line 347 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl8;
#line 347 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl26;
#line 347 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl11;
#line 347 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl tmpIter_chpl;
#line 347 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl call_tmp_chpl27;
#line 347 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl ret_tmp_chpl12;
#line 347 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl13;
#line 347 "../lib/Chai.chpl"
  chpl___EndCount_AtomicT_int64_t_int64_t _coforallCount_chpl = NULL;
#line 347 "../lib/Chai.chpl"
  chpl___EndCount_AtomicT_int64_t_int64_t new_temp_chpl = NULL;
#line 347 "../lib/Chai.chpl"
  int64_t numTasks_chpl;
#line 347 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl _ic__F0_this_chpl;
#line 347 "../lib/Chai.chpl"
  int64_t i_chpl;
#line 347 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl this_chpl32;
#line 347 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl this_chpl33;
#line 347 "../lib/Chai.chpl"
  int64_t end_chpl;
#line 347 "../lib/Chai.chpl"
  _tuple_1_star_range_int64_t_both_one_chpl ret_tmp_chpl14;
#line 347 "../lib/Chai.chpl"
  _class_localscoforall_fn_chpl105 _args_forcoforall_fn_chpl = NULL;
#line 347 "../lib/Chai.chpl"
  _ref_MaxPool_chpl tmp_chpl2 = NULL;
#line 347 "../lib/Chai.chpl"
  _ref_range_int64_t_low_one_chpl tmp_chpl3 = NULL;
#line 347 "../lib/Chai.chpl"
  raw_c_void_ptr _args_vforcoforall_fn_chpl;
#line 347 "../lib/Chai.chpl"
  Error_chpl error_chpl3 = NULL;
#line 347 "../lib/Chai.chpl"
  chpl__class_localscoforall_fn_chpl105_object chpl_macro_tmp_110;
#line 347 "../lib/Chai.chpl"
  local_dataParMinGranularity_chpl = dataParMinGranularity_chpl;
#line 347 "../lib/Chai.chpl"
  local_dataParIgnoreRunningTasks_chpl = dataParIgnoreRunningTasks_chpl;
#line 347 "../lib/Chai.chpl"
  local_dataParTasksPerLocale_chpl = dataParTasksPerLocale_chpl;
#line 345 "../lib/Chai.chpl"
  call_tmp_chpl6 = size_chpl(batch_chpl, INT64(345), INT32(65));
#line 346 "../lib/Chai.chpl"
  chpl_build_low_bounded_range2(INT64(0), &ret_tmp_chpl);
#line 346 "../lib/Chai.chpl"
  call_tmp_chpl7 = ret_tmp_chpl;
#line 346 "../lib/Chai.chpl"
  chpl___POUND_3(call_tmp_chpl7, call_tmp_chpl6, &ret_tmp_chpl2, INT64(346), INT32(65));
#line 346 "../lib/Chai.chpl"
  call_tmp_chpl8 = ret_tmp_chpl2;
#line 346 "../lib/Chai.chpl"
  chpl__ensureDomainExpr5(call_tmp_chpl8, &ret_tmp_chpl3, INT64(346), INT32(65));
#line 346 "../lib/Chai.chpl"
  call_tmp_chpl9 = ret_tmp_chpl3;
#line 346 "../lib/Chai.chpl"
  call_tmp_chpl10 = chpl__buildArrayRuntimeType2(&call_tmp_chpl9);
#line 346 "../lib/Chai.chpl"
  _runtime_type_tmp__chpl = (&call_tmp_chpl10)->dom;
#line 346 "../lib/Chai.chpl"
  chpl__convertRuntimeTypeToValue3(&_runtime_type_tmp__chpl, UINT8(false), &ret_tmp_chpl4, INT64(346), INT32(65));
#line 346 "../lib/Chai.chpl"
  pools_chpl = ret_tmp_chpl4;
#line 347 "../lib/Chai.chpl"
  chpl_build_low_bounded_range2(INT64(0), &ret_tmp_chpl5);
#line 347 "../lib/Chai.chpl"
  call_tmp_chpl11 = ret_tmp_chpl5;
#line 347 "../lib/Chai.chpl"
  coerce_tmp_chpl3 = (batch_chpl)->_instance;
#line 347 "../lib/Chai.chpl"
  tasksPerLocale_chpl = local_dataParTasksPerLocale_chpl;
#line 347 "../lib/Chai.chpl"
  ignoreRunning_chpl = local_dataParIgnoreRunningTasks_chpl;
#line 347 "../lib/Chai.chpl"
  minIndicesPerTask_chpl = local_dataParMinGranularity_chpl;
#line 347 "../lib/Chai.chpl"
  error_chpl = NULL;
#line 347 "../lib/Chai.chpl"
  chpl_check_nil(coerce_tmp_chpl3, INT64(347), INT32(65));
#line 347 "../lib/Chai.chpl"
  coerce_tmp_chpl4 = (coerce_tmp_chpl3)->dom;
#line 347 "../lib/Chai.chpl"
  createTuple_chpl(INT64(0), &ret_tmp_chpl6);
#line 347 "../lib/Chai.chpl"
  offset_x0_chpl = *(ret_tmp_chpl6 + INT64(0));
#line 347 "../lib/Chai.chpl"
  error_chpl2 = NULL;
#line 347 "../lib/Chai.chpl"
  call_tmp_chpl14 = get_chpl_nodeID();
#line 347 "../lib/Chai.chpl"
  call_tmp_chpl15 = chpl_rt_buildLocaleID(call_tmp_chpl14, c_sublocid_any);
#line 347 "../lib/Chai.chpl"
  call_tmp_chpl13 = call_tmp_chpl15;
#line 347 "../lib/Chai.chpl"
  chpl_localeID_to_locale(&call_tmp_chpl13, &ret_tmp_chpl7, INT64(347), INT32(65));
#line 347 "../lib/Chai.chpl"
  call_tmp_chpl16 = ret_tmp_chpl7;
#line 347 "../lib/Chai.chpl"
  call_tmp_chpl12 = call_tmp_chpl16;
#line 347 "../lib/Chai.chpl"
  coerce_tmp_chpl5 = (&call_tmp_chpl12)->_instance;
#line 347 "../lib/Chai.chpl"
  if (((RootClass_chpl)(coerce_tmp_chpl5)) == nil) /* ZLINE: 347 ../lib/Chai.chpl */
#line 347 "../lib/Chai.chpl"
  {
#line 347 "../lib/Chai.chpl"
    nilCheckHalt_chpl(&_str_literal_52_chpl, INT64(347), INT32(65));
#line 347 "../lib/Chai.chpl"
  }
#line 347 "../lib/Chai.chpl"
  chpl_check_nil(coerce_tmp_chpl5, INT64(347), INT32(65));
#line 347 "../lib/Chai.chpl"
  ((int64_t(*)(BaseLocale_chpl,
#line 347 "../lib/Chai.chpl"
     int64_t,
#line 347 "../lib/Chai.chpl"
     int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(coerce_tmp_chpl5))->chpl__cid) + INT64(2))])(coerce_tmp_chpl5, INT64(347), INT32(65));
#line 347 "../lib/Chai.chpl"
  i_x_chpl = &call_tmp_chpl12;
#line 347 "../lib/Chai.chpl"
  deinit_chpl62(i_x_chpl);
#line 347 "../lib/Chai.chpl"
  if (tasksPerLocale_chpl == INT64(0)) /* ZLINE: 347 ../lib/Chai.chpl */
#line 347 "../lib/Chai.chpl"
  {
#line 347 "../lib/Chai.chpl"
    call_tmp_chpl19 = get_chpl_nodeID();
#line 347 "../lib/Chai.chpl"
    call_tmp_chpl20 = chpl_rt_buildLocaleID(call_tmp_chpl19, c_sublocid_any);
#line 347 "../lib/Chai.chpl"
    call_tmp_chpl18 = call_tmp_chpl20;
#line 347 "../lib/Chai.chpl"
    chpl_localeID_to_locale(&call_tmp_chpl18, &ret_tmp_chpl8, INT64(347), INT32(65));
#line 347 "../lib/Chai.chpl"
    call_tmp_chpl21 = ret_tmp_chpl8;
#line 347 "../lib/Chai.chpl"
    call_tmp_chpl17 = call_tmp_chpl21;
#line 347 "../lib/Chai.chpl"
    coerce_tmp_chpl6 = (&call_tmp_chpl17)->_instance;
#line 347 "../lib/Chai.chpl"
    if (((RootClass_chpl)(coerce_tmp_chpl6)) == nil) /* ZLINE: 347 ../lib/Chai.chpl */
#line 347 "../lib/Chai.chpl"
    {
#line 347 "../lib/Chai.chpl"
      nilCheckHalt_chpl(&_str_literal_52_chpl, INT64(347), INT32(65));
#line 347 "../lib/Chai.chpl"
    }
#line 347 "../lib/Chai.chpl"
    coerce_tmp_chpl7 = coerce_tmp_chpl6;
#line 347 "../lib/Chai.chpl"
    chpl_check_nil(coerce_tmp_chpl7, INT64(347), INT32(65));
#line 347 "../lib/Chai.chpl"
    ret_chpl = (coerce_tmp_chpl7)->maxTaskPar;
#line 347 "../lib/Chai.chpl"
    tmp_chpl = ret_chpl;
#line 347 "../lib/Chai.chpl"
    i_x_chpl2 = &call_tmp_chpl17;
#line 347 "../lib/Chai.chpl"
    deinit_chpl62(i_x_chpl2);
#line 347 "../lib/Chai.chpl"
  } else /* ZLINE: 347 ../lib/Chai.chpl */
#line 347 "../lib/Chai.chpl"
  {
#line 347 "../lib/Chai.chpl"
    tmp_chpl = tasksPerLocale_chpl;
#line 347 "../lib/Chai.chpl"
  }
#line 347 "../lib/Chai.chpl"
  if (chpl_task_getSerial()) /* ZLINE: 347 ../lib/Chai.chpl */
#line 347 "../lib/Chai.chpl"
  {
#line 347 "../lib/Chai.chpl"
    tmp_x0_chpl = INT64(1);
#line 347 "../lib/Chai.chpl"
    tmp_x1_chpl = INT64(0);
#line 347 "../lib/Chai.chpl"
  } else /* ZLINE: 347 ../lib/Chai.chpl */
#line 347 "../lib/Chai.chpl"
  {
#line 347 "../lib/Chai.chpl"
    chpl_check_nil(coerce_tmp_chpl4, INT64(347), INT32(65));
#line 347 "../lib/Chai.chpl"
    call_tmp_chpl22 = &((coerce_tmp_chpl4)->ranges);
#line 347 "../lib/Chai.chpl"
    _computeChunkStuff_chpl(tmp_chpl, ignoreRunning_chpl, minIndicesPerTask_chpl, call_tmp_chpl22, &ret_tmp_chpl9, INT64(347), INT32(65));
#line 347 "../lib/Chai.chpl"
    tmp_x0_chpl = *(ret_tmp_chpl9 + INT64(0));
#line 347 "../lib/Chai.chpl"
    tmp_x1_chpl = *(ret_tmp_chpl9 + INT64(1));
#line 347 "../lib/Chai.chpl"
  }
#line 347 "../lib/Chai.chpl"
  numChunks_chpl = tmp_x0_chpl;
#line 347 "../lib/Chai.chpl"
  parDim_chpl = tmp_x1_chpl;
#line 347 "../lib/Chai.chpl"
  init_chpl113(&elt_x0_chpl);
#line 347 "../lib/Chai.chpl"
  tup_x0_chpl = elt_x0_chpl;
#line 347 "../lib/Chai.chpl"
  locBlock_x0_chpl = tup_x0_chpl;
#line 347 "../lib/Chai.chpl"
  chpl_build_low_bounded_range2(offset_x0_chpl, &ret_tmp_chpl10);
#line 347 "../lib/Chai.chpl"
  call_tmp_chpl23 = ret_tmp_chpl10;
#line 347 "../lib/Chai.chpl"
  chpl_check_nil(coerce_tmp_chpl4, INT64(347), INT32(65));
#line 347 "../lib/Chai.chpl"
  call_tmp_chpl24 = &((coerce_tmp_chpl4)->ranges);
#line 347 "../lib/Chai.chpl"
  coerce_tmp_chpl8 = *(*(call_tmp_chpl24) + INT64(0));
#line 347 "../lib/Chai.chpl"
  call_tmp_chpl25 = sizeAs_chpl37(coerce_tmp_chpl8, INT64(347), INT32(65));
#line 347 "../lib/Chai.chpl"
  chpl___POUND_3(call_tmp_chpl23, call_tmp_chpl25, &ret_tmp_chpl11, INT64(347), INT32(65));
#line 347 "../lib/Chai.chpl"
  call_tmp_chpl26 = ret_tmp_chpl11;
#line 347 "../lib/Chai.chpl"
  locBlock_x0_chpl = call_tmp_chpl26;
#line 347 "../lib/Chai.chpl"
  chpl_build_low_bounded_range2(INT64(0), &ret_tmp_chpl12);
#line 347 "../lib/Chai.chpl"
  call_tmp_chpl27 = ret_tmp_chpl12;
#line 347 "../lib/Chai.chpl"
  chpl___POUND_3(call_tmp_chpl27, tmp_x0_chpl, &ret_tmp_chpl13, INT64(347), INT32(65));
#line 347 "../lib/Chai.chpl"
  tmpIter_chpl = ret_tmp_chpl13;
#line 347 "../lib/Chai.chpl"
  new_temp_chpl = _new_chpl17(INT64(347), INT32(65));
#line 347 "../lib/Chai.chpl"
  _coforallCount_chpl = new_temp_chpl;
#line 347 "../lib/Chai.chpl"
  numTasks_chpl = chpl_boundedCoforallSize(tmpIter_chpl, INT64(347), INT32(65));
#line 347 "../lib/Chai.chpl"
  _upEndCount_chpl(_coforallCount_chpl, numTasks_chpl, INT64(347), INT32(65));
#line 347 "../lib/Chai.chpl"
  chpl_resetTaskSpawn(numTasks_chpl, INT64(347), INT32(65));
#line 347 "../lib/Chai.chpl"
  _ic__F0_this_chpl = tmpIter_chpl;
#line 347 "../lib/Chai.chpl"
  checkIfIterWillOverflow_chpl(tmpIter_chpl, UINT8(true), INT64(347), INT32(65));
#line 347 "../lib/Chai.chpl"
  i_chpl = INT64(0);
#line 347 "../lib/Chai.chpl"
  this_chpl32 = _ic__F0_this_chpl;
#line 347 "../lib/Chai.chpl"
  this_chpl33 = _ic__F0_this_chpl;
#line 347 "../lib/Chai.chpl"
  end_chpl = (&this_chpl33)->_high;
#line 347 "../lib/Chai.chpl"
  for (i_chpl = (&this_chpl32)->_low; ((i_chpl <= end_chpl)); i_chpl += INT64(1)) {
#line 347 "../lib/Chai.chpl"
    *(ret_tmp_chpl14 + INT64(0)) = locBlock_x0_chpl;
#line 347 "../lib/Chai.chpl"
    _args_forcoforall_fn_chpl = ((_class_localscoforall_fn_chpl105)(&chpl_macro_tmp_110));
#line 347 "../lib/Chai.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl, INT64(347), INT32(65));
#line 347 "../lib/Chai.chpl"
    (_args_forcoforall_fn_chpl)->_0_numChunks = numChunks_chpl;
#line 347 "../lib/Chai.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl, INT64(347), INT32(65));
#line 347 "../lib/Chai.chpl"
    (_args_forcoforall_fn_chpl)->_1_parDim = parDim_chpl;
#line 347 "../lib/Chai.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl, INT64(347), INT32(65));
#line 347 "../lib/Chai.chpl"
    *((_args_forcoforall_fn_chpl)->_2_coerce_tmp + INT64(0)) = *(ret_tmp_chpl14 + INT64(0));
#line 347 "../lib/Chai.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl, INT64(347), INT32(65));
#line 347 "../lib/Chai.chpl"
    (_args_forcoforall_fn_chpl)->_3__coforallCount = _coforallCount_chpl;
#line 347 "../lib/Chai.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl, INT64(347), INT32(65));
#line 347 "../lib/Chai.chpl"
    (_args_forcoforall_fn_chpl)->_4_chunk = i_chpl;
#line 347 "../lib/Chai.chpl"
    tmp_chpl2 = this_chpl31;
#line 347 "../lib/Chai.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl, INT64(347), INT32(65));
#line 347 "../lib/Chai.chpl"
    (_args_forcoforall_fn_chpl)->_6_this = tmp_chpl2;
#line 347 "../lib/Chai.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl, INT64(347), INT32(65));
#line 347 "../lib/Chai.chpl"
    (_args_forcoforall_fn_chpl)->_7_batch = *(batch_chpl);
#line 347 "../lib/Chai.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl, INT64(347), INT32(65));
#line 347 "../lib/Chai.chpl"
    (_args_forcoforall_fn_chpl)->_8_pools = pools_chpl;
#line 347 "../lib/Chai.chpl"
    tmp_chpl3 = &call_tmp_chpl11;
#line 347 "../lib/Chai.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl, INT64(347), INT32(65));
#line 347 "../lib/Chai.chpl"
    (_args_forcoforall_fn_chpl)->_9_call_tmp = tmp_chpl3;
#line 347 "../lib/Chai.chpl"
    _args_vforcoforall_fn_chpl = ((void*)(_args_forcoforall_fn_chpl));
#line 347 "../lib/Chai.chpl"
    /*** wrapcoforall_fn_chpl105 ***/ chpl_taskAddCoStmt(chpl_task_getRequestedSubloc(), INT64(104), ((chpl_task_bundle_p)(_args_vforcoforall_fn_chpl)), sizeof(chpl__class_localscoforall_fn_chpl105_object), INT64(347), INT32(65));
#line 347 "../lib/Chai.chpl"
  }
#line 347 "../lib/Chai.chpl"
  error_chpl3 = NULL;
#line 347 "../lib/Chai.chpl"
  _waitEndCount_chpl2(_coforallCount_chpl, numTasks_chpl, &error_chpl3, INT64(347), INT32(65));
#line 347 "../lib/Chai.chpl"
  if (error_chpl3 != nil) /* ZLINE: 347 ../lib/Chai.chpl */
#line 347 "../lib/Chai.chpl"
  {
#line 347 "../lib/Chai.chpl"
    chpl__delete72(_coforallCount_chpl, INT64(347), INT32(65));
#line 347 "../lib/Chai.chpl"
    error_chpl2 = error_chpl3;
#line 347 "../lib/Chai.chpl"
    goto forall_IBB_break_label_chpl;
#line 347 "../lib/Chai.chpl"
  }
#line 347 "../lib/Chai.chpl"
  chpl__delete72(_coforallCount_chpl, INT64(347), INT32(65));
#line 347 "../lib/Chai.chpl"
  chpl_after_forall_fence();
#line 347 "../lib/Chai.chpl"
  forall_IBB_break_label_chpl:;
#line 347 "../lib/Chai.chpl"
  if (error_chpl2 != nil) /* ZLINE: 347 ../lib/Chai.chpl */
#line 347 "../lib/Chai.chpl"
  {
#line 347 "../lib/Chai.chpl"
    error_chpl = error_chpl2;
#line 347 "../lib/Chai.chpl"
    goto forall_IBB_break_label_chpl2;
#line 347 "../lib/Chai.chpl"
  }
#line 347 "../lib/Chai.chpl"
  chpl_after_forall_fence();
#line 347 "../lib/Chai.chpl"
  forall_IBB_break_label_chpl2:;
#line 347 "../lib/Chai.chpl"
  if (error_chpl != nil) /* ZLINE: 347 ../lib/Chai.chpl */
#line 347 "../lib/Chai.chpl"
  {
#line 347 "../lib/Chai.chpl"
    chpl_propagate_error(error_chpl, INT64(347), INT32(65));
#line 347 "../lib/Chai.chpl"
  }
#line 347 "../lib/Chai.chpl"
  chpl_after_forall_fence();
#line 350 "../lib/Chai.chpl"
  chpl__checkRetEltTypeMatch2(&pools_chpl);
#line 350 "../lib/Chai.chpl"
  chpl__autoDestroy11(&call_tmp_chpl9, INT64(350), INT32(65));
#line 344 "../lib/Chai.chpl"
  *(_retArg_chpl) = pools_chpl;
#line 344 "../lib/Chai.chpl"
  return;
#line 344 "../lib/Chai.chpl"
}

#line 347 "../lib/Chai.chpl"
/* Chai.chpl:347 */
#line 347 "../lib/Chai.chpl"
static void wrapcoforall_fn_chpl105(_class_localscoforall_fn_chpl105 c_chpl) {
#line 347 "../lib/Chai.chpl"
  Error_chpl error_chpl = NULL;
#line 347 "../lib/Chai.chpl"
  int64_t _0_numChunks_chpl;
#line 347 "../lib/Chai.chpl"
  int64_t _1_parDim_chpl;
#line 347 "../lib/Chai.chpl"
  _tuple_1_star_range_int64_t_both_one_chpl _2_coerce_tmp_chpl;
#line 347 "../lib/Chai.chpl"
  chpl___EndCount_AtomicT_int64_t_int64_t _3__coforallCount_chpl = NULL;
#line 347 "../lib/Chai.chpl"
  int64_t _4_chunk_chpl;
#line 347 "../lib/Chai.chpl"
  _ref_MaxPool_chpl _6_this_chpl = NULL;
#line 347 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl _7_batch_chpl;
#line 347 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl _8_pools_chpl;
#line 347 "../lib/Chai.chpl"
  _ref_range_int64_t_low_one_chpl _9_call_tmp_chpl = NULL;
#line 347 "../lib/Chai.chpl"
  chpl_rmem_consist_acquire(INT64(347), INT32(65));
#line 347 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(347), INT32(65));
#line 347 "../lib/Chai.chpl"
  _0_numChunks_chpl = (c_chpl)->_0_numChunks;
#line 347 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(347), INT32(65));
#line 347 "../lib/Chai.chpl"
  _1_parDim_chpl = (c_chpl)->_1_parDim;
#line 347 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(347), INT32(65));
#line 347 "../lib/Chai.chpl"
  *(_2_coerce_tmp_chpl + INT64(0)) = *((c_chpl)->_2_coerce_tmp + INT64(0));
#line 347 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(347), INT32(65));
#line 347 "../lib/Chai.chpl"
  _3__coforallCount_chpl = (c_chpl)->_3__coforallCount;
#line 347 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(347), INT32(65));
#line 347 "../lib/Chai.chpl"
  _4_chunk_chpl = (c_chpl)->_4_chunk;
#line 347 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(347), INT32(65));
#line 347 "../lib/Chai.chpl"
  _6_this_chpl = (c_chpl)->_6_this;
#line 347 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(347), INT32(65));
#line 347 "../lib/Chai.chpl"
  _7_batch_chpl = (c_chpl)->_7_batch;
#line 347 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(347), INT32(65));
#line 347 "../lib/Chai.chpl"
  _8_pools_chpl = (c_chpl)->_8_pools;
#line 347 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(347), INT32(65));
#line 347 "../lib/Chai.chpl"
  _9_call_tmp_chpl = (c_chpl)->_9_call_tmp;
#line 347 "../lib/Chai.chpl"
  coforall_fn_chpl105(_0_numChunks_chpl, _1_parDim_chpl, &_2_coerce_tmp_chpl, _3__coforallCount_chpl, _4_chunk_chpl, &error_chpl, _6_this_chpl, &_7_batch_chpl, &_8_pools_chpl, _9_call_tmp_chpl);
#line 347 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(347), INT32(65));
#line 347 "../lib/Chai.chpl"
  _downEndCount_chpl((c_chpl)->_3__coforallCount, error_chpl, INT64(347), INT32(65));
#line 347 "../lib/Chai.chpl"
  return;
#line 347 "../lib/Chai.chpl"
}

#line 347 "../lib/Chai.chpl"
/* Chai.chpl:347 */
#line 347 "../lib/Chai.chpl"
static void coforall_fn_chpl105(int64_t numChunks_chpl,
#line 347 "../lib/Chai.chpl"
                                int64_t parDim_chpl,
#line 347 "../lib/Chai.chpl"
                                _tuple_1_star_range_int64_t_both_one_chpl * locBlock_chpl,
#line 347 "../lib/Chai.chpl"
                                chpl___EndCount_AtomicT_int64_t_int64_t _coforallCount_chpl,
#line 347 "../lib/Chai.chpl"
                                int64_t chunk_chpl,
#line 347 "../lib/Chai.chpl"
                                Error_chpl * error_out_chpl,
#line 347 "../lib/Chai.chpl"
                                MaxPool_chpl * this_chpl31,
#line 347 "../lib/Chai.chpl"
                                _array_DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl * batch_chpl,
#line 347 "../lib/Chai.chpl"
                                _array_DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl * pools_chpl,
#line 347 "../lib/Chai.chpl"
                                range_int64_t_low_one_chpl * call_tmp_chpl6) {
#line 347 "../lib/Chai.chpl"
  _tuple_1_star_range_int64_t_both_one_chpl followMe_chpl;
#line 347 "../lib/Chai.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl7 = NULL;
#line 347 "../lib/Chai.chpl"
  int64_t call_tmp_chpl8;
#line 347 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl3;
#line 347 "../lib/Chai.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl9 = NULL;
#line 347 "../lib/Chai.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl10 = NULL;
#line 347 "../lib/Chai.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl11 = NULL;
#line 347 "../lib/Chai.chpl"
  _tuple_2_star_int64_t_chpl call_tmp_chpl12;
#line 347 "../lib/Chai.chpl"
  _tuple_2_star_int64_t_chpl ret_tmp_chpl;
#line 347 "../lib/Chai.chpl"
  int64_t tmp_x0_chpl;
#line 347 "../lib/Chai.chpl"
  int64_t tmp_x1_chpl;
#line 347 "../lib/Chai.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl13 = NULL;
#line 347 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl14;
#line 347 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl2;
#line 347 "../lib/Chai.chpl"
  _tuple_1_star_range_int64_t_both_one_chpl _yield_expr_tmp__chpl;
#line 347 "../lib/Chai.chpl"
  _tuple_1_star_range_int64_t_both_one_chpl chpl_followThis;
#line 347 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl _ir_F0_this_chpl;
#line 347 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl _ic__F5_block_x0_chpl;
#line 347 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl _ic__F7_coerce_tmp_x0_chpl;
#line 347 "../lib/Chai.chpl"
  int64_t _ic__F8_i_chpl;
#line 347 "../lib/Chai.chpl"
  int64_t _ic__F9_end_chpl;
#line 347 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl coerce_tmp_chpl4;
#line 347 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl _ic__F0_this_chpl;
#line 347 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl _ic__F2_myFollowThis_chpl;
#line 347 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl _ic__F3_r_chpl;
#line 347 "../lib/Chai.chpl"
  int64_t _ic__F5_i_chpl;
#line 347 "../lib/Chai.chpl"
  DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl this_chpl32 = NULL;
#line 347 "../lib/Chai.chpl"
  DefaultRectangularDom_1_int64_t_one_chpl coerce_tmp_chpl5 = NULL;
#line 347 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl6;
#line 347 "../lib/Chai.chpl"
  DefaultRectangularDom_1_int64_t_one_chpl this_chpl33 = NULL;
#line 347 "../lib/Chai.chpl"
  uint64_t call_tmp_chpl15;
#line 347 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl7;
#line 347 "../lib/Chai.chpl"
  DefaultRectangularDom_1_int64_t_one_chpl this_chpl34 = NULL;
#line 347 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl8;
#line 347 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl9;
#line 347 "../lib/Chai.chpl"
  DefaultRectangularDom_1_int64_t_one_chpl this_chpl35 = NULL;
#line 347 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl10;
#line 347 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl11;
#line 347 "../lib/Chai.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl16 = NULL;
#line 347 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl this_chpl36;
#line 347 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl this_chpl37;
#line 347 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl tmp_chpl;
#line 347 "../lib/Chai.chpl"
  DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl ret_chpl = NULL;
#line 347 "../lib/Chai.chpl"
  DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl _ic__F0_this_chpl2 = NULL;
#line 347 "../lib/Chai.chpl"
  _tuple_1_star_int64_t_chpl ret_tmp_chpl3;
#line 347 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl tup_x0_chpl;
#line 347 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl elt_x0_chpl;
#line 347 "../lib/Chai.chpl"
  int64_t coerce_tmp_chpl12;
#line 347 "../lib/Chai.chpl"
  _ref__tuple_1_star_range_int64_t_both_one_chpl call_tmp_chpl17 = NULL;
#line 347 "../lib/Chai.chpl"
  chpl_bool tmp_chpl2;
#line 347 "../lib/Chai.chpl"
  _ref__tuple_1_star_range_int64_t_both_one_chpl call_tmp_chpl18 = NULL;
#line 347 "../lib/Chai.chpl"
  int64_t call_tmp_chpl19;
#line 347 "../lib/Chai.chpl"
  _ref__tuple_1_star_range_int64_t_both_one_chpl call_tmp_chpl20 = NULL;
#line 347 "../lib/Chai.chpl"
  int64_t call_tmp_chpl21;
#line 347 "../lib/Chai.chpl"
  _tuple_1_star_range_int64_t_both_one_chpl retcopy_chpl;
#line 347 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl _ic__F0_this_chpl3;
#line 347 "../lib/Chai.chpl"
  int64_t coerce_tmp_chpl13;
#line 347 "../lib/Chai.chpl"
  int64_t call_tmp_chpl22;
#line 347 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl this_chpl38;
#line 347 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl tmp_chpl3;
#line 347 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl tmp_chpl4;
#line 347 "../lib/Chai.chpl"
  _ref_range_int64_t_both_one_chpl tmp_chpl5 = NULL;
#line 347 "../lib/Chai.chpl"
  chpl_bool tmp_chpl6;
#line 347 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl tmp_chpl7;
#line 347 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl tmp_chpl8;
#line 347 "../lib/Chai.chpl"
  int64_t call_tmp_chpl23;
#line 347 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl tmp_chpl9;
#line 347 "../lib/Chai.chpl"
  int64_t call_tmp_chpl24;
#line 347 "../lib/Chai.chpl"
  _ref_range_int64_t_both_one_chpl tmp_chpl10 = NULL;
#line 347 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl14;
#line 347 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl _ic__F0_this_chpl4;
#line 347 "../lib/Chai.chpl"
  DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl this_chpl39 = NULL;
#line 347 "../lib/Chai.chpl"
  _ddata_Tensor_3__real64_chpl coerce_tmp_chpl15 = NULL;
#line 347 "../lib/Chai.chpl"
  _ref_Tensor_3__real64_chpl call_tmp_chpl25 = NULL;
#line 348 "../lib/Chai.chpl"
  _tuple_1_star_int64_t_chpl coerce_tmp_chpl16;
#line 348 "../lib/Chai.chpl"
  _ref__tuple_1_star_int64_t_chpl i_i_chpl = NULL;
#line 348 "../lib/Chai.chpl"
  DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl ret_chpl2 = NULL;
#line 348 "../lib/Chai.chpl"
  DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl coerce_tmp_chpl17 = NULL;
#line 348 "../lib/Chai.chpl"
  _tuple_1_star_int64_t_chpl _formal_tmp_in_ind_chpl;
#line 348 "../lib/Chai.chpl"
  int64_t _formal_tmp_ind_x0_chpl;
#line 348 "../lib/Chai.chpl"
  _ddata_Tensor_3__real64_chpl coerce_tmp_chpl18 = NULL;
#line 348 "../lib/Chai.chpl"
  _ref_Tensor_3__real64_chpl call_tmp_chpl26 = NULL;
#line 348 "../lib/Chai.chpl"
  Tensor_3__real64_chpl call_tmp_chpl27;
#line 348 "../lib/Chai.chpl"
  Tensor_3__real64_chpl ret_tmp_chpl4;
#line 348 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_3_int64_t_one__real64_int64_t_chpl _field_destructor_tmp__chpl;
#line 348 "../lib/Chai.chpl"
  _domain_DefaultRectangularDom_3_int64_t_one_chpl _field_destructor_tmp__chpl2;
#line 347 "../lib/Chai.chpl"
  int64_t tmp_chpl11;
#line 347 "../lib/Chai.chpl"
  int64_t tmp_chpl12;
#line 347 "../lib/Chai.chpl"
  int64_t tmp_chpl13;
#line 347 "../lib/Chai.chpl"
  *(followMe_chpl + INT64(0)) = *(*(locBlock_chpl) + INT64(0));
#line 347 "../lib/Chai.chpl"
  call_tmp_chpl7 = this_chpl20(locBlock_chpl, parDim_chpl, INT64(347), INT32(65));
#line 347 "../lib/Chai.chpl"
  coerce_tmp_chpl3 = *(call_tmp_chpl7);
#line 347 "../lib/Chai.chpl"
  call_tmp_chpl8 = sizeAs_chpl37(coerce_tmp_chpl3, INT64(347), INT32(65));
#line 347 "../lib/Chai.chpl"
  call_tmp_chpl9 = this_chpl20(locBlock_chpl, parDim_chpl, INT64(347), INT32(65));
#line 347 "../lib/Chai.chpl"
  call_tmp_chpl10 = this_chpl20(locBlock_chpl, parDim_chpl, INT64(347), INT32(65));
#line 347 "../lib/Chai.chpl"
  call_tmp_chpl11 = this_chpl20(locBlock_chpl, parDim_chpl, INT64(347), INT32(65));
#line 347 "../lib/Chai.chpl"
  _computeBlock_chpl(call_tmp_chpl8, numChunks_chpl, chunk_chpl, (call_tmp_chpl9)->_high, (call_tmp_chpl10)->_low, (call_tmp_chpl11)->_low, &ret_tmp_chpl, INT64(347), INT32(65));
#line 347 "../lib/Chai.chpl"
  *(call_tmp_chpl12 + INT64(0)) = *(ret_tmp_chpl + INT64(0));
#line 347 "../lib/Chai.chpl"
  *(call_tmp_chpl12 + INT64(1)) = *(ret_tmp_chpl + INT64(1));
#line 347 "../lib/Chai.chpl"
  tmp_x0_chpl = *(call_tmp_chpl12 + INT64(0));
#line 347 "../lib/Chai.chpl"
  tmp_x1_chpl = *(call_tmp_chpl12 + INT64(1));
#line 347 "../lib/Chai.chpl"
  call_tmp_chpl13 = this_chpl20(&followMe_chpl, parDim_chpl, INT64(347), INT32(65));
#line 347 "../lib/Chai.chpl"
  chpl_build_bounded_range6(tmp_x0_chpl, tmp_x1_chpl, &ret_tmp_chpl2);
#line 347 "../lib/Chai.chpl"
  call_tmp_chpl14 = ret_tmp_chpl2;
#line 347 "../lib/Chai.chpl"
  *(call_tmp_chpl13) = call_tmp_chpl14;
#line 347 "../lib/Chai.chpl"
  *(_yield_expr_tmp__chpl + INT64(0)) = *(followMe_chpl + INT64(0));
#line 347 "../lib/Chai.chpl"
  *(chpl_followThis + INT64(0)) = *(_yield_expr_tmp__chpl + INT64(0));
#line 347 "../lib/Chai.chpl"
  _ir_F0_this_chpl = *(batch_chpl);
#line 347 "../lib/Chai.chpl"
  coerce_tmp_chpl4 = *(call_tmp_chpl6);
#line 347 "../lib/Chai.chpl"
  _ic__F0_this_chpl = coerce_tmp_chpl4;
#line 347 "../lib/Chai.chpl"
  tmp_chpl = _ir_F0_this_chpl;
#line 347 "../lib/Chai.chpl"
  ret_chpl = (&tmp_chpl)->_instance;
#line 347 "../lib/Chai.chpl"
  _ic__F0_this_chpl2 = ret_chpl;
#line 347 "../lib/Chai.chpl"
  this_chpl32 = ret_chpl;
#line 347 "../lib/Chai.chpl"
  chpl_check_nil(this_chpl32, INT64(347), INT32(65));
#line 347 "../lib/Chai.chpl"
  coerce_tmp_chpl5 = (this_chpl32)->dom;
#line 347 "../lib/Chai.chpl"
  createTuple_chpl(INT64(0), &ret_tmp_chpl3);
#line 347 "../lib/Chai.chpl"
  init_chpl113(&elt_x0_chpl);
#line 347 "../lib/Chai.chpl"
  tup_x0_chpl = elt_x0_chpl;
#line 347 "../lib/Chai.chpl"
  _ic__F5_block_x0_chpl = tup_x0_chpl;
#line 347 "../lib/Chai.chpl"
  coerce_tmp_chpl6 = *(chpl_followThis + INT64(0));
#line 347 "../lib/Chai.chpl"
  coerce_tmp_chpl12 = (&coerce_tmp_chpl6)->_high;
#line 347 "../lib/Chai.chpl"
  this_chpl33 = coerce_tmp_chpl5;
#line 347 "../lib/Chai.chpl"
  chpl_check_nil(this_chpl33, INT64(347), INT32(65));
#line 347 "../lib/Chai.chpl"
  call_tmp_chpl17 = &((this_chpl33)->ranges);
#line 347 "../lib/Chai.chpl"
  coerce_tmp_chpl7 = *(*(call_tmp_chpl17) + INT64(0));
#line 347 "../lib/Chai.chpl"
  call_tmp_chpl15 = sizeAs_chpl36(coerce_tmp_chpl7, INT64(347), INT32(65));
#line 347 "../lib/Chai.chpl"
  if (! (coerce_tmp_chpl12 < INT64(0))) /* ZLINE: 347 ../lib/Chai.chpl */
#line 347 "../lib/Chai.chpl"
  {
#line 347 "../lib/Chai.chpl"
    tmp_chpl2 = (((uint64_t)(coerce_tmp_chpl12)) >= call_tmp_chpl15);
#line 347 "../lib/Chai.chpl"
  } else /* ZLINE: 347 ../lib/Chai.chpl */
#line 347 "../lib/Chai.chpl"
  {
#line 347 "../lib/Chai.chpl"
    tmp_chpl2 = UINT8(false);
#line 347 "../lib/Chai.chpl"
  }
#line 347 "../lib/Chai.chpl"
  if (tmp_chpl2) /* ZLINE: 347 ../lib/Chai.chpl */
#line 347 "../lib/Chai.chpl"
  {
#line 347 "../lib/Chai.chpl"
    boundsCheckHalt_chpl(&_str_literal_4152_chpl, INT64(347), INT32(65));
#line 347 "../lib/Chai.chpl"
  }
#line 347 "../lib/Chai.chpl"
  this_chpl34 = coerce_tmp_chpl5;
#line 347 "../lib/Chai.chpl"
  chpl_check_nil(this_chpl34, INT64(347), INT32(65));
#line 347 "../lib/Chai.chpl"
  call_tmp_chpl18 = &((this_chpl34)->ranges);
#line 347 "../lib/Chai.chpl"
  coerce_tmp_chpl8 = *(*(call_tmp_chpl18) + INT64(0));
#line 347 "../lib/Chai.chpl"
  coerce_tmp_chpl9 = *(chpl_followThis + INT64(0));
#line 347 "../lib/Chai.chpl"
  call_tmp_chpl19 = ((&coerce_tmp_chpl8)->_low + ((int64_t)(((&coerce_tmp_chpl9)->_low * INT64(1)))));
#line 347 "../lib/Chai.chpl"
  this_chpl35 = coerce_tmp_chpl5;
#line 347 "../lib/Chai.chpl"
  chpl_check_nil(this_chpl35, INT64(347), INT32(65));
#line 347 "../lib/Chai.chpl"
  call_tmp_chpl20 = &((this_chpl35)->ranges);
#line 347 "../lib/Chai.chpl"
  coerce_tmp_chpl10 = *(*(call_tmp_chpl20) + INT64(0));
#line 347 "../lib/Chai.chpl"
  coerce_tmp_chpl11 = *(chpl_followThis + INT64(0));
#line 347 "../lib/Chai.chpl"
  call_tmp_chpl21 = ((&coerce_tmp_chpl10)->_low + ((int64_t)(((&coerce_tmp_chpl11)->_high * INT64(1)))));
#line 347 "../lib/Chai.chpl"
  call_tmp_chpl16 = &_ic__F5_block_x0_chpl;
#line 347 "../lib/Chai.chpl"
  compilerAssert_chpl();
#line 347 "../lib/Chai.chpl"
  (call_tmp_chpl16)->_low = call_tmp_chpl19;
#line 347 "../lib/Chai.chpl"
  (call_tmp_chpl16)->_high = call_tmp_chpl21;
#line 347 "../lib/Chai.chpl"
  *(retcopy_chpl + INT64(0)) = _ic__F5_block_x0_chpl;
#line 347 "../lib/Chai.chpl"
  _ic__F7_coerce_tmp_x0_chpl = *(retcopy_chpl + INT64(0));
#line 347 "../lib/Chai.chpl"
  _ic__F0_this_chpl3 = _ic__F7_coerce_tmp_x0_chpl;
#line 347 "../lib/Chai.chpl"
  checkIfIterWillOverflow_chpl(_ic__F7_coerce_tmp_x0_chpl, UINT8(true), INT64(347), INT32(65));
#line 347 "../lib/Chai.chpl"
  _ic__F8_i_chpl = INT64(0);
#line 347 "../lib/Chai.chpl"
  this_chpl36 = _ic__F0_this_chpl3;
#line 347 "../lib/Chai.chpl"
  coerce_tmp_chpl13 = (&this_chpl36)->_low;
#line 347 "../lib/Chai.chpl"
  this_chpl37 = _ic__F0_this_chpl3;
#line 347 "../lib/Chai.chpl"
  _ic__F9_end_chpl = (&this_chpl37)->_high;
#line 347 "../lib/Chai.chpl"
  tmp_chpl3 = *(chpl_followThis + INT64(0));
#line 347 "../lib/Chai.chpl"
  _ic__F2_myFollowThis_chpl = tmp_chpl3;
#line 347 "../lib/Chai.chpl"
  tmp_chpl4 = tmp_chpl3;
#line 347 "../lib/Chai.chpl"
  if ((&tmp_chpl4)->_low > (&tmp_chpl4)->_high) /* ZLINE: 347 ../lib/Chai.chpl */
#line 347 "../lib/Chai.chpl"
  {
#line 347 "../lib/Chai.chpl"
    tmp_chpl6 = UINT8(false);
#line 347 "../lib/Chai.chpl"
  } else /* ZLINE: 347 ../lib/Chai.chpl */
#line 347 "../lib/Chai.chpl"
  {
#line 347 "../lib/Chai.chpl"
    tmp_chpl6 = UINT8(true);
#line 347 "../lib/Chai.chpl"
  }
#line 347 "../lib/Chai.chpl"
  if (! tmp_chpl6) /* ZLINE: 347 ../lib/Chai.chpl */
#line 347 "../lib/Chai.chpl"
  {
#line 347 "../lib/Chai.chpl"
    tmp_chpl7 = tmp_chpl3;
#line 347 "../lib/Chai.chpl"
    if (! ((&tmp_chpl7)->_low > (&tmp_chpl7)->_high)) /* ZLINE: 347 ../lib/Chai.chpl */
#line 347 "../lib/Chai.chpl"
    {
#line 347 "../lib/Chai.chpl"
      boundsCheckHalt_chpl(&_str_literal_633_chpl, INT64(347), INT32(65));
#line 347 "../lib/Chai.chpl"
    }
#line 347 "../lib/Chai.chpl"
  }
#line 347 "../lib/Chai.chpl"
  call_tmp_chpl22 = sizeAs_chpl37(tmp_chpl3, INT64(347), INT32(65));
#line 347 "../lib/Chai.chpl"
  if (call_tmp_chpl22 != INT64(0)) /* ZLINE: 347 ../lib/Chai.chpl */
#line 347 "../lib/Chai.chpl"
  {
#line 347 "../lib/Chai.chpl"
  }
#line 347 "../lib/Chai.chpl"
  tmp_chpl5 = &_ic__F3_r_chpl;
#line 347 "../lib/Chai.chpl"
  init_chpl113(tmp_chpl5);
#line 347 "../lib/Chai.chpl"
  if (call_tmp_chpl22 != INT64(0)) /* ZLINE: 347 ../lib/Chai.chpl */
#line 347 "../lib/Chai.chpl"
  {
#line 347 "../lib/Chai.chpl"
    tmp_chpl8 = _ic__F2_myFollowThis_chpl;
#line 347 "../lib/Chai.chpl"
    call_tmp_chpl23 = orderToIndex_chpl(coerce_tmp_chpl4, (&tmp_chpl8)->_low, INT64(347), INT32(65));
#line 347 "../lib/Chai.chpl"
    tmp_chpl9 = _ic__F2_myFollowThis_chpl;
#line 347 "../lib/Chai.chpl"
    call_tmp_chpl24 = orderToIndex_chpl(_ic__F0_this_chpl, (&tmp_chpl9)->_high, INT64(347), INT32(65));
#line 347 "../lib/Chai.chpl"
    tmp_chpl10 = &_ic__F3_r_chpl;
#line 347 "../lib/Chai.chpl"
    (tmp_chpl10)->_low = call_tmp_chpl23;
#line 347 "../lib/Chai.chpl"
    (tmp_chpl10)->_high = call_tmp_chpl24;
#line 347 "../lib/Chai.chpl"
    verifyAppropriateStide_chpl2(INT64(1), INT64(347), INT32(65));
#line 347 "../lib/Chai.chpl"
  }
#line 347 "../lib/Chai.chpl"
  coerce_tmp_chpl14 = _ic__F3_r_chpl;
#line 347 "../lib/Chai.chpl"
  _ic__F0_this_chpl4 = coerce_tmp_chpl14;
#line 347 "../lib/Chai.chpl"
  checkIfIterWillOverflow_chpl(coerce_tmp_chpl14, UINT8(true), INT64(347), INT32(65));
#line 347 "../lib/Chai.chpl"
  _ic__F5_i_chpl = INT64(0);
#line 347 "../lib/Chai.chpl"
  this_chpl38 = _ic__F0_this_chpl4;
#line 347 "../lib/Chai.chpl"
  for (_ic__F8_i_chpl = coerce_tmp_chpl13,_ic__F5_i_chpl = (&this_chpl38)->_low; (tmp_chpl11 = (_ic__F8_i_chpl <= _ic__F9_end_chpl),tmp_chpl11); tmp_chpl12 = _ic__F8_i_chpl,tmp_chpl12 += INT64(1),_ic__F8_i_chpl = tmp_chpl12,tmp_chpl13 = _ic__F5_i_chpl,tmp_chpl13 += INT64(1),_ic__F5_i_chpl = tmp_chpl13) {
#line 347 "../lib/Chai.chpl"
    this_chpl39 = _ic__F0_this_chpl2;
#line 347 "../lib/Chai.chpl"
    chpl_check_nil(this_chpl39, INT64(347), INT32(65));
#line 347 "../lib/Chai.chpl"
    coerce_tmp_chpl15 = (this_chpl39)->shiftedData;
#line 347 "../lib/Chai.chpl"
    call_tmp_chpl25 = (coerce_tmp_chpl15 + _ic__F8_i_chpl);
#line 348 "../lib/Chai.chpl"
    *(coerce_tmp_chpl16 + INT64(0)) = _ic__F5_i_chpl;
#line 348 "../lib/Chai.chpl"
    i_i_chpl = &coerce_tmp_chpl16;
#line 348 "../lib/Chai.chpl"
    ret_chpl2 = (pools_chpl)->_instance;
#line 348 "../lib/Chai.chpl"
    checkAccess_chpl10(pools_chpl, i_i_chpl, ret_chpl2, INT64(348), INT32(65));
#line 348 "../lib/Chai.chpl"
    coerce_tmp_chpl17 = ret_chpl2;
#line 348 "../lib/Chai.chpl"
    *(_formal_tmp_in_ind_chpl + INT64(0)) = *(coerce_tmp_chpl16 + INT64(0));
#line 348 "../lib/Chai.chpl"
    _formal_tmp_ind_x0_chpl = *(_formal_tmp_in_ind_chpl + INT64(0));
#line 348 "../lib/Chai.chpl"
    chpl_check_nil(coerce_tmp_chpl17, INT64(348), INT32(65));
#line 348 "../lib/Chai.chpl"
    coerce_tmp_chpl18 = (coerce_tmp_chpl17)->shiftedData;
#line 348 "../lib/Chai.chpl"
    call_tmp_chpl26 = (coerce_tmp_chpl18 + _formal_tmp_ind_x0_chpl);
#line 348 "../lib/Chai.chpl"
    forwardProp_chpl2(this_chpl31, call_tmp_chpl25, &ret_tmp_chpl4);
#line 348 "../lib/Chai.chpl"
    call_tmp_chpl27 = ret_tmp_chpl4;
#line 348 "../lib/Chai.chpl"
    chpl___ASSIGN_24(call_tmp_chpl26, &call_tmp_chpl27);
#line 348 "../lib/Chai.chpl"
    _field_destructor_tmp__chpl = (&call_tmp_chpl27)->data;
#line 348 "../lib/Chai.chpl"
    chpl__autoDestroy28(&_field_destructor_tmp__chpl, INT64(348), INT32(65));
#line 348 "../lib/Chai.chpl"
    _field_destructor_tmp__chpl2 = (&call_tmp_chpl27)->_domain;
#line 348 "../lib/Chai.chpl"
    chpl__autoDestroy8(&_field_destructor_tmp__chpl2, INT64(348), INT32(65));
#line 348 "../lib/Chai.chpl"
  }
#line 347 "../lib/Chai.chpl"
  return;
#line 347 "../lib/Chai.chpl"
}

#line 353 "../lib/Chai.chpl"
/* Chai.chpl:353 */
#line 353 "../lib/Chai.chpl"
static void forwardProp_chpl2(MaxPool_chpl * this_chpl31,
#line 353 "../lib/Chai.chpl"
                              Tensor_3__real64_chpl * convs_chpl,
#line 353 "../lib/Chai.chpl"
                              Tensor_3__real64_chpl * _retArg_chpl) {
#line 360 "../lib/Chai.chpl"
  int64_t local_dataParMinGranularity_chpl;
#line 360 "../lib/Chai.chpl"
  chpl_bool local_dataParIgnoreRunningTasks_chpl;
#line 360 "../lib/Chai.chpl"
  int64_t local_dataParTasksPerLocale_chpl;
#line 354 "../lib/Chai.chpl"
  _tuple_3_star_int64_t_chpl call_tmp_chpl6;
#line 354 "../lib/Chai.chpl"
  _tuple_3_star_int64_t_chpl ret_tmp_chpl;
#line 354 "../lib/Chai.chpl"
  int64_t tmp_x1_chpl;
#line 354 "../lib/Chai.chpl"
  int64_t tmp_x2_chpl;
#line 355 "../lib/Chai.chpl"
  int64_t call_tmp_chpl7;
#line 356 "../lib/Chai.chpl"
  int64_t call_tmp_chpl8;
#line 359 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_3_int64_t_one__real64_int64_t_chpl pools_chpl;
#line 359 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl call_tmp_chpl9;
#line 359 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl ret_tmp_chpl2;
#line 359 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl10;
#line 359 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl3;
#line 359 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl call_tmp_chpl11;
#line 359 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl ret_tmp_chpl4;
#line 359 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl12;
#line 359 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl5;
#line 359 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl call_tmp_chpl13;
#line 359 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl ret_tmp_chpl6;
#line 359 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl14;
#line 359 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl7;
#line 359 "../lib/Chai.chpl"
  _domain_DefaultRectangularDom_3_int64_t_one_chpl call_tmp_chpl15;
#line 359 "../lib/Chai.chpl"
  _domain_DefaultRectangularDom_3_int64_t_one_chpl ret_tmp_chpl8;
#line 359 "../lib/Chai.chpl"
  chpl___RuntimeTypeInfo7 call_tmp_chpl16;
#line 359 "../lib/Chai.chpl"
  _domain_DefaultRectangularDom_3_int64_t_one_chpl _runtime_type_tmp__chpl;
#line 359 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_3_int64_t_one__real64_int64_t_chpl ret_tmp_chpl9;
#line 360 "../lib/Chai.chpl"
  _domain_DefaultRectangularDom_3_int64_t_one_chpl call_tmp_chpl17;
#line 360 "../lib/Chai.chpl"
  _domain_DefaultRectangularDom_3_int64_t_one_chpl ret_tmp_chpl10;
#line 360 "../lib/Chai.chpl"
  _domain_DefaultRectangularDom_3_int64_t_one_chpl call_tmp_chpl18;
#line 360 "../lib/Chai.chpl"
  _domain_DefaultRectangularDom_3_int64_t_one_chpl ret_tmp_chpl11;
#line 360 "../lib/Chai.chpl"
  DefaultRectangularDom_3_int64_t_one_chpl coerce_tmp_chpl3 = NULL;
#line 360 "../lib/Chai.chpl"
  int64_t tasksPerLocale_chpl;
#line 360 "../lib/Chai.chpl"
  chpl_bool ignoreRunning_chpl;
#line 360 "../lib/Chai.chpl"
  int64_t minIndicesPerTask_chpl;
#line 360 "../lib/Chai.chpl"
  _tuple_3_star_int64_t_chpl ret_tmp_chpl12;
#line 360 "../lib/Chai.chpl"
  Error_chpl error_chpl = NULL;
#line 360 "../lib/Chai.chpl"
  int64_t tmp_chpl;
#line 360 "../lib/Chai.chpl"
  locale_chpl call_tmp_chpl19;
#line 360 "../lib/Chai.chpl"
  chpl_localeID_t call_tmp_chpl20;
#line 360 "../lib/Chai.chpl"
  int32_t call_tmp_chpl21;
#line 360 "../lib/Chai.chpl"
  chpl_localeID_t call_tmp_chpl22;
#line 360 "../lib/Chai.chpl"
  locale_chpl call_tmp_chpl23;
#line 360 "../lib/Chai.chpl"
  locale_chpl ret_tmp_chpl13;
#line 360 "../lib/Chai.chpl"
  int64_t ret_chpl;
#line 360 "../lib/Chai.chpl"
  BaseLocale_chpl coerce_tmp_chpl4 = NULL;
#line 360 "../lib/Chai.chpl"
  BaseLocale_chpl coerce_tmp_chpl5 = NULL;
#line 360 "../lib/Chai.chpl"
  _ref_locale_chpl i_x_chpl = NULL;
#line 360 "../lib/Chai.chpl"
  int64_t tmp_x0_chpl;
#line 360 "../lib/Chai.chpl"
  int64_t tmp_x1_chpl2;
#line 360 "../lib/Chai.chpl"
  _ref__tuple_3_star_range_int64_t_both_one_chpl call_tmp_chpl24 = NULL;
#line 360 "../lib/Chai.chpl"
  _tuple_2_star_int64_t_chpl ret_tmp_chpl14;
#line 360 "../lib/Chai.chpl"
  int64_t numChunks_chpl;
#line 360 "../lib/Chai.chpl"
  int64_t parDim_chpl;
#line 360 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl tmpIter_chpl;
#line 360 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl call_tmp_chpl25;
#line 360 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl ret_tmp_chpl15;
#line 360 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl16;
#line 360 "../lib/Chai.chpl"
  chpl___EndCount_AtomicT_int64_t_int64_t _coforallCount_chpl = NULL;
#line 360 "../lib/Chai.chpl"
  chpl___EndCount_AtomicT_int64_t_int64_t new_temp_chpl = NULL;
#line 360 "../lib/Chai.chpl"
  int64_t numTasks_chpl;
#line 360 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl _ic__F0_this_chpl;
#line 360 "../lib/Chai.chpl"
  int64_t i_chpl;
#line 360 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl this_chpl32;
#line 360 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl this_chpl33;
#line 360 "../lib/Chai.chpl"
  int64_t end_chpl;
#line 360 "../lib/Chai.chpl"
  _class_localscoforall_fn_chpl106 _args_forcoforall_fn_chpl = NULL;
#line 360 "../lib/Chai.chpl"
  _ref_Tensor_3__real64_chpl tmp_chpl2 = NULL;
#line 360 "../lib/Chai.chpl"
  raw_c_void_ptr _args_vforcoforall_fn_chpl;
#line 360 "../lib/Chai.chpl"
  Error_chpl error_chpl2 = NULL;
#line 364 "../lib/Chai.chpl"
  Tensor_3__real64_chpl call_tmp_chpl26;
#line 364 "../lib/Chai.chpl"
  Tensor_3__real64_chpl initTemp_chpl;
#line 364 "../lib/Chai.chpl"
  chpl__class_localscoforall_fn_chpl106_object chpl_macro_tmp_111;
#line 360 "../lib/Chai.chpl"
  local_dataParMinGranularity_chpl = dataParMinGranularity_chpl;
#line 360 "../lib/Chai.chpl"
  local_dataParIgnoreRunningTasks_chpl = dataParIgnoreRunningTasks_chpl;
#line 360 "../lib/Chai.chpl"
  local_dataParTasksPerLocale_chpl = dataParTasksPerLocale_chpl;
#line 354 "../lib/Chai.chpl"
  shape_chpl8(convs_chpl, &ret_tmp_chpl);
#line 354 "../lib/Chai.chpl"
  *(call_tmp_chpl6 + INT64(0)) = *(ret_tmp_chpl + INT64(0));
#line 354 "../lib/Chai.chpl"
  *(call_tmp_chpl6 + INT64(1)) = *(ret_tmp_chpl + INT64(1));
#line 354 "../lib/Chai.chpl"
  *(call_tmp_chpl6 + INT64(2)) = *(ret_tmp_chpl + INT64(2));
#line 354 "../lib/Chai.chpl"
  tmp_x1_chpl = *(call_tmp_chpl6 + INT64(1));
#line 354 "../lib/Chai.chpl"
  tmp_x2_chpl = *(call_tmp_chpl6 + INT64(2));
#line 355 "../lib/Chai.chpl"
  call_tmp_chpl7 = (*(call_tmp_chpl6 + INT64(0)) / INT64(2));
#line 356 "../lib/Chai.chpl"
  call_tmp_chpl8 = (tmp_x1_chpl / INT64(2));
#line 359 "../lib/Chai.chpl"
  chpl_build_low_bounded_range2(INT64(0), &ret_tmp_chpl2);
#line 359 "../lib/Chai.chpl"
  call_tmp_chpl9 = ret_tmp_chpl2;
#line 359 "../lib/Chai.chpl"
  chpl___POUND_3(call_tmp_chpl9, call_tmp_chpl7, &ret_tmp_chpl3, INT64(359), INT32(65));
#line 359 "../lib/Chai.chpl"
  call_tmp_chpl10 = ret_tmp_chpl3;
#line 359 "../lib/Chai.chpl"
  chpl_build_low_bounded_range2(INT64(0), &ret_tmp_chpl4);
#line 359 "../lib/Chai.chpl"
  call_tmp_chpl11 = ret_tmp_chpl4;
#line 359 "../lib/Chai.chpl"
  chpl___POUND_3(call_tmp_chpl11, call_tmp_chpl8, &ret_tmp_chpl5, INT64(359), INT32(65));
#line 359 "../lib/Chai.chpl"
  call_tmp_chpl12 = ret_tmp_chpl5;
#line 359 "../lib/Chai.chpl"
  chpl_build_low_bounded_range2(INT64(0), &ret_tmp_chpl6);
#line 359 "../lib/Chai.chpl"
  call_tmp_chpl13 = ret_tmp_chpl6;
#line 359 "../lib/Chai.chpl"
  chpl___POUND_3(call_tmp_chpl13, tmp_x2_chpl, &ret_tmp_chpl7, INT64(359), INT32(65));
#line 359 "../lib/Chai.chpl"
  call_tmp_chpl14 = ret_tmp_chpl7;
#line 359 "../lib/Chai.chpl"
  chpl__ensureDomainExpr6(call_tmp_chpl10, call_tmp_chpl12, call_tmp_chpl14, &ret_tmp_chpl8, INT64(359), INT32(65));
#line 359 "../lib/Chai.chpl"
  call_tmp_chpl15 = ret_tmp_chpl8;
#line 359 "../lib/Chai.chpl"
  call_tmp_chpl16 = chpl__buildArrayRuntimeType3(&call_tmp_chpl15);
#line 359 "../lib/Chai.chpl"
  _runtime_type_tmp__chpl = (&call_tmp_chpl16)->dom;
#line 359 "../lib/Chai.chpl"
  chpl__convertRuntimeTypeToValue6(&_runtime_type_tmp__chpl, UINT8(false), &ret_tmp_chpl9, INT64(359), INT32(65));
#line 359 "../lib/Chai.chpl"
  pools_chpl = ret_tmp_chpl9;
#line 360 "../lib/Chai.chpl"
  _dom_chpl8(&pools_chpl, &ret_tmp_chpl10, INT64(360), INT32(65));
#line 360 "../lib/Chai.chpl"
  call_tmp_chpl17 = ret_tmp_chpl10;
#line 360 "../lib/Chai.chpl"
  _dom_chpl8(&pools_chpl, &ret_tmp_chpl11, INT64(360), INT32(65));
#line 360 "../lib/Chai.chpl"
  call_tmp_chpl18 = ret_tmp_chpl11;
#line 360 "../lib/Chai.chpl"
  coerce_tmp_chpl3 = (&call_tmp_chpl18)->_instance;
#line 360 "../lib/Chai.chpl"
  tasksPerLocale_chpl = local_dataParTasksPerLocale_chpl;
#line 360 "../lib/Chai.chpl"
  ignoreRunning_chpl = local_dataParIgnoreRunningTasks_chpl;
#line 360 "../lib/Chai.chpl"
  minIndicesPerTask_chpl = local_dataParMinGranularity_chpl;
#line 360 "../lib/Chai.chpl"
  createTuple_chpl2(INT64(0), &ret_tmp_chpl12);
#line 360 "../lib/Chai.chpl"
  error_chpl = NULL;
#line 360 "../lib/Chai.chpl"
  if (tasksPerLocale_chpl == INT64(0)) /* ZLINE: 360 ../lib/Chai.chpl */
#line 360 "../lib/Chai.chpl"
  {
#line 360 "../lib/Chai.chpl"
    call_tmp_chpl21 = get_chpl_nodeID();
#line 360 "../lib/Chai.chpl"
    call_tmp_chpl22 = chpl_rt_buildLocaleID(call_tmp_chpl21, c_sublocid_any);
#line 360 "../lib/Chai.chpl"
    call_tmp_chpl20 = call_tmp_chpl22;
#line 360 "../lib/Chai.chpl"
    chpl_localeID_to_locale(&call_tmp_chpl20, &ret_tmp_chpl13, INT64(360), INT32(65));
#line 360 "../lib/Chai.chpl"
    call_tmp_chpl23 = ret_tmp_chpl13;
#line 360 "../lib/Chai.chpl"
    call_tmp_chpl19 = call_tmp_chpl23;
#line 360 "../lib/Chai.chpl"
    coerce_tmp_chpl4 = (&call_tmp_chpl19)->_instance;
#line 360 "../lib/Chai.chpl"
    if (((RootClass_chpl)(coerce_tmp_chpl4)) == nil) /* ZLINE: 360 ../lib/Chai.chpl */
#line 360 "../lib/Chai.chpl"
    {
#line 360 "../lib/Chai.chpl"
      nilCheckHalt_chpl(&_str_literal_52_chpl, INT64(360), INT32(65));
#line 360 "../lib/Chai.chpl"
    }
#line 360 "../lib/Chai.chpl"
    coerce_tmp_chpl5 = coerce_tmp_chpl4;
#line 360 "../lib/Chai.chpl"
    chpl_check_nil(coerce_tmp_chpl5, INT64(360), INT32(65));
#line 360 "../lib/Chai.chpl"
    ret_chpl = (coerce_tmp_chpl5)->maxTaskPar;
#line 360 "../lib/Chai.chpl"
    tmp_chpl = ret_chpl;
#line 360 "../lib/Chai.chpl"
    i_x_chpl = &call_tmp_chpl19;
#line 360 "../lib/Chai.chpl"
    deinit_chpl62(i_x_chpl);
#line 360 "../lib/Chai.chpl"
  } else /* ZLINE: 360 ../lib/Chai.chpl */
#line 360 "../lib/Chai.chpl"
  {
#line 360 "../lib/Chai.chpl"
    tmp_chpl = tasksPerLocale_chpl;
#line 360 "../lib/Chai.chpl"
  }
#line 360 "../lib/Chai.chpl"
  if (chpl_task_getSerial()) /* ZLINE: 360 ../lib/Chai.chpl */
#line 360 "../lib/Chai.chpl"
  {
#line 360 "../lib/Chai.chpl"
    tmp_x0_chpl = INT64(1);
#line 360 "../lib/Chai.chpl"
    tmp_x1_chpl2 = INT64(0);
#line 360 "../lib/Chai.chpl"
  } else /* ZLINE: 360 ../lib/Chai.chpl */
#line 360 "../lib/Chai.chpl"
  {
#line 360 "../lib/Chai.chpl"
    chpl_check_nil(coerce_tmp_chpl3, INT64(360), INT32(65));
#line 360 "../lib/Chai.chpl"
    call_tmp_chpl24 = &((coerce_tmp_chpl3)->ranges);
#line 360 "../lib/Chai.chpl"
    _computeChunkStuff_chpl3(tmp_chpl, ignoreRunning_chpl, minIndicesPerTask_chpl, call_tmp_chpl24, &ret_tmp_chpl14, INT64(360), INT32(65));
#line 360 "../lib/Chai.chpl"
    tmp_x0_chpl = *(ret_tmp_chpl14 + INT64(0));
#line 360 "../lib/Chai.chpl"
    tmp_x1_chpl2 = *(ret_tmp_chpl14 + INT64(1));
#line 360 "../lib/Chai.chpl"
  }
#line 360 "../lib/Chai.chpl"
  numChunks_chpl = tmp_x0_chpl;
#line 360 "../lib/Chai.chpl"
  parDim_chpl = tmp_x1_chpl2;
#line 360 "../lib/Chai.chpl"
  chpl_build_low_bounded_range2(INT64(0), &ret_tmp_chpl15);
#line 360 "../lib/Chai.chpl"
  call_tmp_chpl25 = ret_tmp_chpl15;
#line 360 "../lib/Chai.chpl"
  chpl___POUND_3(call_tmp_chpl25, tmp_x0_chpl, &ret_tmp_chpl16, INT64(360), INT32(65));
#line 360 "../lib/Chai.chpl"
  tmpIter_chpl = ret_tmp_chpl16;
#line 360 "../lib/Chai.chpl"
  new_temp_chpl = _new_chpl17(INT64(360), INT32(65));
#line 360 "../lib/Chai.chpl"
  _coforallCount_chpl = new_temp_chpl;
#line 360 "../lib/Chai.chpl"
  numTasks_chpl = chpl_boundedCoforallSize(tmpIter_chpl, INT64(360), INT32(65));
#line 360 "../lib/Chai.chpl"
  _upEndCount_chpl(_coforallCount_chpl, numTasks_chpl, INT64(360), INT32(65));
#line 360 "../lib/Chai.chpl"
  chpl_resetTaskSpawn(numTasks_chpl, INT64(360), INT32(65));
#line 360 "../lib/Chai.chpl"
  _ic__F0_this_chpl = tmpIter_chpl;
#line 360 "../lib/Chai.chpl"
  checkIfIterWillOverflow_chpl(tmpIter_chpl, UINT8(true), INT64(360), INT32(65));
#line 360 "../lib/Chai.chpl"
  i_chpl = INT64(0);
#line 360 "../lib/Chai.chpl"
  this_chpl32 = _ic__F0_this_chpl;
#line 360 "../lib/Chai.chpl"
  this_chpl33 = _ic__F0_this_chpl;
#line 360 "../lib/Chai.chpl"
  end_chpl = (&this_chpl33)->_high;
#line 360 "../lib/Chai.chpl"
  for (i_chpl = (&this_chpl32)->_low; ((i_chpl <= end_chpl)); i_chpl += INT64(1)) {
#line 360 "../lib/Chai.chpl"
    _args_forcoforall_fn_chpl = ((_class_localscoforall_fn_chpl106)(&chpl_macro_tmp_111));
#line 360 "../lib/Chai.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl, INT64(360), INT32(65));
#line 360 "../lib/Chai.chpl"
    (_args_forcoforall_fn_chpl)->_0_numChunks = numChunks_chpl;
#line 360 "../lib/Chai.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl, INT64(360), INT32(65));
#line 360 "../lib/Chai.chpl"
    (_args_forcoforall_fn_chpl)->_1_parDim = parDim_chpl;
#line 360 "../lib/Chai.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl, INT64(360), INT32(65));
#line 360 "../lib/Chai.chpl"
    (_args_forcoforall_fn_chpl)->_2_coerce_tmp = coerce_tmp_chpl3;
#line 360 "../lib/Chai.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl, INT64(360), INT32(65));
#line 360 "../lib/Chai.chpl"
    (_args_forcoforall_fn_chpl)->_3__coforallCount = _coforallCount_chpl;
#line 360 "../lib/Chai.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl, INT64(360), INT32(65));
#line 360 "../lib/Chai.chpl"
    (_args_forcoforall_fn_chpl)->_4_chunk = i_chpl;
#line 360 "../lib/Chai.chpl"
    tmp_chpl2 = convs_chpl;
#line 360 "../lib/Chai.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl, INT64(360), INT32(65));
#line 360 "../lib/Chai.chpl"
    (_args_forcoforall_fn_chpl)->_6_convs = tmp_chpl2;
#line 360 "../lib/Chai.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl, INT64(360), INT32(65));
#line 360 "../lib/Chai.chpl"
    (_args_forcoforall_fn_chpl)->_7_pools = pools_chpl;
#line 360 "../lib/Chai.chpl"
    _args_vforcoforall_fn_chpl = ((void*)(_args_forcoforall_fn_chpl));
#line 360 "../lib/Chai.chpl"
    /*** wrapcoforall_fn_chpl106 ***/ chpl_taskAddCoStmt(chpl_task_getRequestedSubloc(), INT64(105), ((chpl_task_bundle_p)(_args_vforcoforall_fn_chpl)), sizeof(chpl__class_localscoforall_fn_chpl106_object), INT64(360), INT32(65));
#line 360 "../lib/Chai.chpl"
  }
#line 360 "../lib/Chai.chpl"
  error_chpl2 = NULL;
#line 360 "../lib/Chai.chpl"
  _waitEndCount_chpl2(_coforallCount_chpl, numTasks_chpl, &error_chpl2, INT64(360), INT32(65));
#line 360 "../lib/Chai.chpl"
  if (error_chpl2 != nil) /* ZLINE: 360 ../lib/Chai.chpl */
#line 360 "../lib/Chai.chpl"
  {
#line 360 "../lib/Chai.chpl"
    chpl__delete72(_coforallCount_chpl, INT64(360), INT32(65));
#line 360 "../lib/Chai.chpl"
    error_chpl = error_chpl2;
#line 360 "../lib/Chai.chpl"
    goto forall_IBB_break_label_chpl;
#line 360 "../lib/Chai.chpl"
  }
#line 360 "../lib/Chai.chpl"
  chpl__delete72(_coforallCount_chpl, INT64(360), INT32(65));
#line 360 "../lib/Chai.chpl"
  chpl_after_forall_fence();
#line 360 "../lib/Chai.chpl"
  forall_IBB_break_label_chpl:;
#line 360 "../lib/Chai.chpl"
  if (error_chpl != nil) /* ZLINE: 360 ../lib/Chai.chpl */
#line 360 "../lib/Chai.chpl"
  {
#line 360 "../lib/Chai.chpl"
    chpl_propagate_error(error_chpl, INT64(360), INT32(65));
#line 360 "../lib/Chai.chpl"
  }
#line 360 "../lib/Chai.chpl"
  chpl_after_forall_fence();
#line 360 "../lib/Chai.chpl"
  chpl__autoDestroy8(&call_tmp_chpl18, INT64(360), INT32(65));
#line 360 "../lib/Chai.chpl"
  chpl__autoDestroy8(&call_tmp_chpl17, INT64(360), INT32(65));
#line 364 "../lib/Chai.chpl"
  init_chpl245(&initTemp_chpl, &pools_chpl);
#line 364 "../lib/Chai.chpl"
  call_tmp_chpl26 = initTemp_chpl;
#line 366 "../lib/Chai.chpl"
  chpl__autoDestroy28(&pools_chpl, INT64(366), INT32(65));
#line 366 "../lib/Chai.chpl"
  chpl__autoDestroy8(&call_tmp_chpl15, INT64(366), INT32(65));
#line 353 "../lib/Chai.chpl"
  *(_retArg_chpl) = call_tmp_chpl26;
#line 353 "../lib/Chai.chpl"
  return;
#line 353 "../lib/Chai.chpl"
}

#line 360 "../lib/Chai.chpl"
/* Chai.chpl:360 */
#line 360 "../lib/Chai.chpl"
static void wrapcoforall_fn_chpl106(_class_localscoforall_fn_chpl106 c_chpl) {
#line 360 "../lib/Chai.chpl"
  Error_chpl error_chpl = NULL;
#line 360 "../lib/Chai.chpl"
  int64_t _0_numChunks_chpl;
#line 360 "../lib/Chai.chpl"
  int64_t _1_parDim_chpl;
#line 360 "../lib/Chai.chpl"
  DefaultRectangularDom_3_int64_t_one_chpl _2_coerce_tmp_chpl = NULL;
#line 360 "../lib/Chai.chpl"
  chpl___EndCount_AtomicT_int64_t_int64_t _3__coforallCount_chpl = NULL;
#line 360 "../lib/Chai.chpl"
  int64_t _4_chunk_chpl;
#line 360 "../lib/Chai.chpl"
  _ref_Tensor_3__real64_chpl _6_convs_chpl = NULL;
#line 360 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_3_int64_t_one__real64_int64_t_chpl _7_pools_chpl;
#line 360 "../lib/Chai.chpl"
  chpl_rmem_consist_acquire(INT64(360), INT32(65));
#line 360 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(360), INT32(65));
#line 360 "../lib/Chai.chpl"
  _0_numChunks_chpl = (c_chpl)->_0_numChunks;
#line 360 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(360), INT32(65));
#line 360 "../lib/Chai.chpl"
  _1_parDim_chpl = (c_chpl)->_1_parDim;
#line 360 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(360), INT32(65));
#line 360 "../lib/Chai.chpl"
  _2_coerce_tmp_chpl = (c_chpl)->_2_coerce_tmp;
#line 360 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(360), INT32(65));
#line 360 "../lib/Chai.chpl"
  _3__coforallCount_chpl = (c_chpl)->_3__coforallCount;
#line 360 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(360), INT32(65));
#line 360 "../lib/Chai.chpl"
  _4_chunk_chpl = (c_chpl)->_4_chunk;
#line 360 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(360), INT32(65));
#line 360 "../lib/Chai.chpl"
  _6_convs_chpl = (c_chpl)->_6_convs;
#line 360 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(360), INT32(65));
#line 360 "../lib/Chai.chpl"
  _7_pools_chpl = (c_chpl)->_7_pools;
#line 360 "../lib/Chai.chpl"
  coforall_fn_chpl106(_0_numChunks_chpl, _1_parDim_chpl, _2_coerce_tmp_chpl, _3__coforallCount_chpl, _4_chunk_chpl, &error_chpl, _6_convs_chpl, &_7_pools_chpl);
#line 360 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(360), INT32(65));
#line 360 "../lib/Chai.chpl"
  _downEndCount_chpl((c_chpl)->_3__coforallCount, error_chpl, INT64(360), INT32(65));
#line 360 "../lib/Chai.chpl"
  return;
#line 360 "../lib/Chai.chpl"
}

#line 360 "../lib/Chai.chpl"
/* Chai.chpl:360 */
#line 360 "../lib/Chai.chpl"
static void coforall_fn_chpl106(int64_t numChunks_chpl,
#line 360 "../lib/Chai.chpl"
                                int64_t parDim_chpl,
#line 360 "../lib/Chai.chpl"
                                DefaultRectangularDom_3_int64_t_one_chpl this_chpl31,
#line 360 "../lib/Chai.chpl"
                                chpl___EndCount_AtomicT_int64_t_int64_t _coforallCount_chpl,
#line 360 "../lib/Chai.chpl"
                                int64_t chunk_chpl,
#line 360 "../lib/Chai.chpl"
                                Error_chpl * error_out_chpl,
#line 360 "../lib/Chai.chpl"
                                Tensor_3__real64_chpl * convs_chpl,
#line 360 "../lib/Chai.chpl"
                                _array_DefaultRectangularArr_3_int64_t_one__real64_int64_t_chpl * pools_chpl) {
#line 362 "../lib/Chai.chpl"
  int64_t local_dataParMinGranularity_chpl;
#line 362 "../lib/Chai.chpl"
  chpl_bool local_dataParIgnoreRunningTasks_chpl;
#line 362 "../lib/Chai.chpl"
  int64_t local_dataParTasksPerLocale_chpl;
#line 360 "../lib/Chai.chpl"
  _tuple_3_star_range_int64_t_both_one_chpl ranges_chpl;
#line 360 "../lib/Chai.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl6 = NULL;
#line 360 "../lib/Chai.chpl"
  uint64_t call_tmp_chpl7;
#line 360 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl3;
#line 360 "../lib/Chai.chpl"
  uint64_t call_tmp_chpl8;
#line 360 "../lib/Chai.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl9 = NULL;
#line 360 "../lib/Chai.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl10 = NULL;
#line 360 "../lib/Chai.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl11 = NULL;
#line 360 "../lib/Chai.chpl"
  _tuple_2_star_int64_t_chpl call_tmp_chpl12;
#line 360 "../lib/Chai.chpl"
  _tuple_2_star_int64_t_chpl ret_tmp_chpl;
#line 360 "../lib/Chai.chpl"
  int64_t tmp_x0_chpl;
#line 360 "../lib/Chai.chpl"
  int64_t tmp_x1_chpl;
#line 360 "../lib/Chai.chpl"
  _tuple_3_star_range_int64_t_both_one_chpl block_chpl;
#line 360 "../lib/Chai.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl13 = NULL;
#line 360 "../lib/Chai.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl14 = NULL;
#line 360 "../lib/Chai.chpl"
  _tuple_3_star_range_int64_t_both_one_chpl coerce_tmp_chpl4;
#line 360 "../lib/Chai.chpl"
  _tuple_3_star_range_int64_t_both_one_chpl retcopy_chpl;
#line 360 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl5;
#line 360 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl _ic__F0_this_chpl;
#line 360 "../lib/Chai.chpl"
  int64_t i_chpl;
#line 360 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl this_chpl32;
#line 360 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl this_chpl33;
#line 360 "../lib/Chai.chpl"
  int64_t end_chpl;
#line 360 "../lib/Chai.chpl"
  int64_t i_chpl2;
#line 360 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl6;
#line 360 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl _ic__F0_this_chpl2;
#line 360 "../lib/Chai.chpl"
  int64_t i_chpl3;
#line 360 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl this_chpl34;
#line 360 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl this_chpl35;
#line 360 "../lib/Chai.chpl"
  int64_t end_chpl2;
#line 360 "../lib/Chai.chpl"
  int64_t i_chpl4;
#line 360 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl7;
#line 360 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl _ic__F0_this_chpl3;
#line 360 "../lib/Chai.chpl"
  int64_t i_chpl5;
#line 360 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl this_chpl36;
#line 360 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl this_chpl37;
#line 360 "../lib/Chai.chpl"
  int64_t end_chpl3;
#line 360 "../lib/Chai.chpl"
  _tuple_2_star_int64_t_chpl ret_tmp_chpl2;
#line 360 "../lib/Chai.chpl"
  _tuple_2_star_int64_t_chpl _yieldedIndex_chpl;
#line 360 "../lib/Chai.chpl"
  int64_t j_x0_chpl;
#line 360 "../lib/Chai.chpl"
  int64_t j_x1_chpl;
#line 360 "../lib/Chai.chpl"
  _tuple_3_star_int64_t_chpl ret_tmp_chpl3;
#line 360 "../lib/Chai.chpl"
  _tuple_3_star_int64_t_chpl yret_chpl;
#line 360 "../lib/Chai.chpl"
  int64_t chpl_idx_1_x0;
#line 360 "../lib/Chai.chpl"
  int64_t chpl_idx_1_x1;
#line 360 "../lib/Chai.chpl"
  int64_t chpl_idx_1_x2;
#line 361 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl call_tmp_chpl15;
#line 361 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl ret_tmp_chpl4;
#line 361 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl16;
#line 361 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl5;
#line 361 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl call_tmp_chpl17;
#line 361 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl ret_tmp_chpl6;
#line 361 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl18;
#line 361 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl7;
#line 361 "../lib/Chai.chpl"
  range_int64_t_neither_one_chpl call_tmp_chpl19;
#line 361 "../lib/Chai.chpl"
  range_int64_t_neither_one_chpl ret_tmp_chpl8;
#line 361 "../lib/Chai.chpl"
  _array_ArrayViewSliceArr__real64_int64_t_DefaultRectangularDom_3_int64_t_one_int64_t_DefaultRectangularArr_3_int64_t_one__real64_int64_t_chpl_bool_chpl call_tmp_chpl20;
#line 98 "../lib/Tensor.chpl"
  _ref__array_DefaultRectangularArr_3_int64_t_one__real64_int64_t_chpl call_tmp_chpl21 = NULL;
#line 361 "../lib/Chai.chpl"
  _array_ArrayViewSliceArr__real64_int64_t_DefaultRectangularDom_3_int64_t_one_int64_t_DefaultRectangularArr_3_int64_t_one__real64_int64_t_chpl_bool_chpl ret_tmp_chpl9;
#line 361 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_3_int64_t_one__real64_int64_t_chpl region_chpl;
#line 362 "../lib/Chai.chpl"
  _tuple_3_star_int64_t_chpl coerce_tmp_chpl8;
#line 362 "../lib/Chai.chpl"
  _ref__tuple_3_star_int64_t_chpl i_i_chpl = NULL;
#line 362 "../lib/Chai.chpl"
  DefaultRectangularArr_3_int64_t_one__real64_int64_t_chpl ret_chpl = NULL;
#line 362 "../lib/Chai.chpl"
  DefaultRectangularArr_3_int64_t_one__real64_int64_t_chpl coerce_tmp_chpl9 = NULL;
#line 362 "../lib/Chai.chpl"
  _tuple_3_star_int64_t_chpl _formal_tmp_in_ind_chpl;
#line 362 "../lib/Chai.chpl"
  int64_t _formal_tmp_ind_x0_chpl;
#line 362 "../lib/Chai.chpl"
  int64_t _formal_tmp_ind_x1_chpl;
#line 362 "../lib/Chai.chpl"
  int64_t _formal_tmp_ind_x2_chpl;
#line 362 "../lib/Chai.chpl"
  int64_t ret_chpl2;
#line 362 "../lib/Chai.chpl"
  int64_t sum_chpl;
#line 362 "../lib/Chai.chpl"
  _ref__tuple_3_star_int64_t_chpl call_tmp_chpl22 = NULL;
#line 362 "../lib/Chai.chpl"
  _ref_int64_t_chpl i_lhs_chpl = NULL;
#line 362 "../lib/Chai.chpl"
  _ref__tuple_3_star_int64_t_chpl call_tmp_chpl23 = NULL;
#line 362 "../lib/Chai.chpl"
  _ref_int64_t_chpl i_lhs_chpl2 = NULL;
#line 362 "../lib/Chai.chpl"
  _ref_int64_t_chpl i_lhs_chpl3 = NULL;
#line 362 "../lib/Chai.chpl"
  _ddata__real64_chpl coerce_tmp_chpl10 = NULL;
#line 362 "../lib/Chai.chpl"
  _ref__real64_chpl call_tmp_chpl24 = NULL;
#line 362 "../lib/Chai.chpl"
  range_int64_t_neither_one_chpl call_tmp_chpl25;
#line 362 "../lib/Chai.chpl"
  range_int64_t_neither_one_chpl ret_tmp_chpl10;
#line 362 "../lib/Chai.chpl"
  range_int64_t_neither_one_chpl call_tmp_chpl26;
#line 362 "../lib/Chai.chpl"
  range_int64_t_neither_one_chpl ret_tmp_chpl11;
#line 362 "../lib/Chai.chpl"
  _array_ArrayViewRankChangeArr__real64_int64_t_ArrayViewRankChangeDom_2_int64_t_one__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_DefaultRectangularArr_3_int64_t_one__real64_int64_t__tuple_3_star_chpl_bool__tuple_3_star_int64_t__remoteAccessData__real64_2_int64_t_one_T_F_chpl call_tmp_chpl27;
#line 362 "../lib/Chai.chpl"
  _array_ArrayViewRankChangeArr__real64_int64_t_ArrayViewRankChangeDom_2_int64_t_one__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_DefaultRectangularArr_3_int64_t_one__real64_int64_t__tuple_3_star_chpl_bool__tuple_3_star_int64_t__remoteAccessData__real64_2_int64_t_one_T_F_chpl ret_tmp_chpl12;
#line 362 "../lib/Chai.chpl"
  MaxReduceScanOp__real64_chpl globalRP_call_tmp_chpl = NULL;
#line 362 "../lib/Chai.chpl"
  MaxReduceScanOp__real64_chpl new_temp_chpl = NULL;
#line 362 "../lib/Chai.chpl"
  chpl_LocalSpinlock default_arg_l_chpl;
#line 362 "../lib/Chai.chpl"
  chpl_LocalSpinlock ret_chpl3;
#line 362 "../lib/Chai.chpl"
  chpl_LocalSpinlock l_chpl;
#line 362 "../lib/Chai.chpl"
  _real64 value_chpl;
#line 362 "../lib/Chai.chpl"
  _real64 call_tmp_chpl28;
#line 362 "../lib/Chai.chpl"
  MaxReduceScanOp__real64_chpl coerce_tmp_chpl11 = NULL;
#line 362 "../lib/Chai.chpl"
  _real64 globalAS_chpl_redSVar_chpl;
#line 362 "../lib/Chai.chpl"
  ArrayViewRankChangeArr__real64_int64_t_ArrayViewRankChangeDom_2_int64_t_one__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_DefaultRectangularArr_3_int64_t_one__real64_int64_t__tuple_3_star_chpl_bool__tuple_3_star_int64_t__remoteAccessData__real64_2_int64_t_one_T_F_chpl ret_chpl4 = NULL;
#line 362 "../lib/Chai.chpl"
  ArrayViewRankChangeArr__real64_int64_t_ArrayViewRankChangeDom_2_int64_t_one__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_DefaultRectangularArr_3_int64_t_one__real64_int64_t__tuple_3_star_chpl_bool__tuple_3_star_int64_t__remoteAccessData__real64_2_int64_t_one_T_F_chpl coerce_tmp_chpl12 = NULL;
#line 362 "../lib/Chai.chpl"
  Error_chpl error_chpl = NULL;
#line 362 "../lib/Chai.chpl"
  ArrayViewRankChangeDom_2_int64_t_one__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_3_star_chpl_bool__tuple_3_star_int64_t_chpl ret_chpl5 = NULL;
#line 362 "../lib/Chai.chpl"
  ArrayViewRankChangeDom_2_int64_t_one__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_3_star_chpl_bool__tuple_3_star_int64_t_chpl coerce_tmp_chpl13 = NULL;
#line 362 "../lib/Chai.chpl"
  Error_chpl error_chpl2 = NULL;
#line 362 "../lib/Chai.chpl"
  ArrayViewRankChangeArr__real64_int64_t_ArrayViewRankChangeDom_2_int64_t_one__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_DefaultRectangularArr_3_int64_t_one__real64_int64_t__tuple_3_star_chpl_bool__tuple_3_star_int64_t__remoteAccessData__real64_2_int64_t_one_T_F_chpl this_chpl38 = NULL;
#line 362 "../lib/Chai.chpl"
  DefaultRectangularDom_2_int64_t_one_chpl coerce_tmp_chpl14 = NULL;
#line 362 "../lib/Chai.chpl"
  DefaultRectangularDom_2_int64_t_one_chpl coerce_tmp_chpl15 = NULL;
#line 362 "../lib/Chai.chpl"
  int64_t tasksPerLocale_chpl;
#line 362 "../lib/Chai.chpl"
  chpl_bool ignoreRunning_chpl;
#line 362 "../lib/Chai.chpl"
  int64_t minIndicesPerTask_chpl;
#line 362 "../lib/Chai.chpl"
  _tuple_2_star_int64_t_chpl ret_tmp_chpl13;
#line 362 "../lib/Chai.chpl"
  Error_chpl error_chpl3 = NULL;
#line 362 "../lib/Chai.chpl"
  int64_t tmp_chpl;
#line 362 "../lib/Chai.chpl"
  locale_chpl call_tmp_chpl29;
#line 362 "../lib/Chai.chpl"
  chpl_localeID_t call_tmp_chpl30;
#line 362 "../lib/Chai.chpl"
  int32_t call_tmp_chpl31;
#line 362 "../lib/Chai.chpl"
  chpl_localeID_t call_tmp_chpl32;
#line 362 "../lib/Chai.chpl"
  locale_chpl call_tmp_chpl33;
#line 362 "../lib/Chai.chpl"
  locale_chpl ret_tmp_chpl14;
#line 362 "../lib/Chai.chpl"
  int64_t ret_chpl6;
#line 362 "../lib/Chai.chpl"
  BaseLocale_chpl coerce_tmp_chpl16 = NULL;
#line 362 "../lib/Chai.chpl"
  BaseLocale_chpl coerce_tmp_chpl17 = NULL;
#line 362 "../lib/Chai.chpl"
  _ref_locale_chpl i_x_chpl = NULL;
#line 362 "../lib/Chai.chpl"
  int64_t tmp_x0_chpl2;
#line 362 "../lib/Chai.chpl"
  int64_t tmp_x1_chpl2;
#line 362 "../lib/Chai.chpl"
  _ref__tuple_2_star_range_int64_t_both_one_chpl call_tmp_chpl34 = NULL;
#line 362 "../lib/Chai.chpl"
  _tuple_2_star_int64_t_chpl ret_tmp_chpl15;
#line 362 "../lib/Chai.chpl"
  int64_t numChunks_chpl2;
#line 362 "../lib/Chai.chpl"
  int64_t parDim_chpl2;
#line 362 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl tmpIter_chpl;
#line 362 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl call_tmp_chpl35;
#line 362 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl ret_tmp_chpl16;
#line 362 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl17;
#line 362 "../lib/Chai.chpl"
  chpl___EndCount_AtomicT_int64_t_int64_t _coforallCount_chpl2 = NULL;
#line 362 "../lib/Chai.chpl"
  chpl___EndCount_AtomicT_int64_t_int64_t new_temp_chpl2 = NULL;
#line 362 "../lib/Chai.chpl"
  int64_t numTasks_chpl;
#line 362 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl _ic__F0_this_chpl4;
#line 362 "../lib/Chai.chpl"
  int64_t i_chpl6;
#line 362 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl this_chpl39;
#line 362 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl this_chpl40;
#line 362 "../lib/Chai.chpl"
  int64_t end_chpl4;
#line 362 "../lib/Chai.chpl"
  _class_localscoforall_fn_chpl107 _args_forcoforall_fn_chpl = NULL;
#line 362 "../lib/Chai.chpl"
  raw_c_void_ptr _args_vforcoforall_fn_chpl;
#line 362 "../lib/Chai.chpl"
  Error_chpl error_chpl4 = NULL;
#line 362 "../lib/Chai.chpl"
  MaxReduceScanOp__real64_chpl coerce_tmp_chpl18 = NULL;
#line 362 "../lib/Chai.chpl"
  _real64 call_tmp_chpl36;
#line 362 "../lib/Chai.chpl"
  MaxReduceScanOp__real64_chpl coerce_tmp_chpl19 = NULL;
#line 362 "../lib/Chai.chpl"
  chpl__class_localscoforall_fn_chpl107_object chpl_macro_tmp_112;
#line 362 "../lib/Chai.chpl"
  local_dataParMinGranularity_chpl = dataParMinGranularity_chpl;
#line 362 "../lib/Chai.chpl"
  local_dataParIgnoreRunningTasks_chpl = dataParIgnoreRunningTasks_chpl;
#line 362 "../lib/Chai.chpl"
  local_dataParTasksPerLocale_chpl = dataParTasksPerLocale_chpl;
#line 360 "../lib/Chai.chpl"
  chpl_check_nil(this_chpl31, INT64(360), INT32(65));
#line 360 "../lib/Chai.chpl"
  *(ranges_chpl + INT64(0)) = *((this_chpl31)->ranges + INT64(0));
#line 360 "../lib/Chai.chpl"
  *(ranges_chpl + INT64(1)) = *((this_chpl31)->ranges + INT64(1));
#line 360 "../lib/Chai.chpl"
  *(ranges_chpl + INT64(2)) = *((this_chpl31)->ranges + INT64(2));
#line 360 "../lib/Chai.chpl"
  call_tmp_chpl6 = this_chpl24(&ranges_chpl, parDim_chpl, INT64(360), INT32(65));
#line 360 "../lib/Chai.chpl"
  coerce_tmp_chpl3 = *(call_tmp_chpl6);
#line 360 "../lib/Chai.chpl"
  call_tmp_chpl7 = sizeAs_chpl36(coerce_tmp_chpl3, INT64(360), INT32(65));
#line 360 "../lib/Chai.chpl"
  this_chpl24(&ranges_chpl, parDim_chpl, INT64(360), INT32(65));
#line 360 "../lib/Chai.chpl"
  call_tmp_chpl8 = (call_tmp_chpl7 * UINT64(1));
#line 360 "../lib/Chai.chpl"
  call_tmp_chpl9 = this_chpl24(&ranges_chpl, parDim_chpl, INT64(360), INT32(65));
#line 360 "../lib/Chai.chpl"
  call_tmp_chpl10 = this_chpl24(&ranges_chpl, parDim_chpl, INT64(360), INT32(65));
#line 360 "../lib/Chai.chpl"
  call_tmp_chpl11 = this_chpl24(&ranges_chpl, parDim_chpl, INT64(360), INT32(65));
#line 360 "../lib/Chai.chpl"
  _computeBlock_chpl2(call_tmp_chpl8, numChunks_chpl, chunk_chpl, (call_tmp_chpl9)->_high, (call_tmp_chpl10)->_low, (call_tmp_chpl11)->_low, &ret_tmp_chpl, INT64(360), INT32(65));
#line 360 "../lib/Chai.chpl"
  *(call_tmp_chpl12 + INT64(0)) = *(ret_tmp_chpl + INT64(0));
#line 360 "../lib/Chai.chpl"
  *(call_tmp_chpl12 + INT64(1)) = *(ret_tmp_chpl + INT64(1));
#line 360 "../lib/Chai.chpl"
  tmp_x0_chpl = *(call_tmp_chpl12 + INT64(0));
#line 360 "../lib/Chai.chpl"
  tmp_x1_chpl = *(call_tmp_chpl12 + INT64(1));
#line 360 "../lib/Chai.chpl"
  *(block_chpl + INT64(0)) = *(ranges_chpl + INT64(0));
#line 360 "../lib/Chai.chpl"
  *(block_chpl + INT64(1)) = *(ranges_chpl + INT64(1));
#line 360 "../lib/Chai.chpl"
  *(block_chpl + INT64(2)) = *(ranges_chpl + INT64(2));
#line 360 "../lib/Chai.chpl"
  call_tmp_chpl13 = this_chpl24(&block_chpl, parDim_chpl, INT64(360), INT32(65));
#line 360 "../lib/Chai.chpl"
  (call_tmp_chpl13)->_low = tmp_x0_chpl;
#line 360 "../lib/Chai.chpl"
  call_tmp_chpl14 = this_chpl24(&block_chpl, parDim_chpl, INT64(360), INT32(65));
#line 360 "../lib/Chai.chpl"
  (call_tmp_chpl14)->_high = tmp_x1_chpl;
#line 360 "../lib/Chai.chpl"
  *(retcopy_chpl + INT64(0)) = *(block_chpl + INT64(0));
#line 360 "../lib/Chai.chpl"
  *(retcopy_chpl + INT64(1)) = *(block_chpl + INT64(1));
#line 360 "../lib/Chai.chpl"
  *(retcopy_chpl + INT64(2)) = *(block_chpl + INT64(2));
#line 360 "../lib/Chai.chpl"
  *(coerce_tmp_chpl4 + INT64(0)) = *(retcopy_chpl + INT64(0));
#line 360 "../lib/Chai.chpl"
  *(coerce_tmp_chpl4 + INT64(1)) = *(retcopy_chpl + INT64(1));
#line 360 "../lib/Chai.chpl"
  *(coerce_tmp_chpl4 + INT64(2)) = *(retcopy_chpl + INT64(2));
#line 360 "../lib/Chai.chpl"
  coerce_tmp_chpl5 = *(coerce_tmp_chpl4 + INT64(0));
#line 360 "../lib/Chai.chpl"
  _ic__F0_this_chpl = coerce_tmp_chpl5;
#line 360 "../lib/Chai.chpl"
  checkIfIterWillOverflow_chpl(coerce_tmp_chpl5, UINT8(true), INT64(360), INT32(65));
#line 360 "../lib/Chai.chpl"
  i_chpl = INT64(0);
#line 360 "../lib/Chai.chpl"
  this_chpl32 = _ic__F0_this_chpl;
#line 360 "../lib/Chai.chpl"
  this_chpl33 = _ic__F0_this_chpl;
#line 360 "../lib/Chai.chpl"
  end_chpl = (&this_chpl33)->_high;
#line 360 "../lib/Chai.chpl"
  for (i_chpl = (&this_chpl32)->_low; ((i_chpl <= end_chpl)); i_chpl += INT64(1)) {
#line 360 "../lib/Chai.chpl"
    i_chpl2 = i_chpl;
#line 360 "../lib/Chai.chpl"
    coerce_tmp_chpl6 = *(coerce_tmp_chpl4 + INT64(1));
#line 360 "../lib/Chai.chpl"
    _ic__F0_this_chpl2 = coerce_tmp_chpl6;
#line 360 "../lib/Chai.chpl"
    checkIfIterWillOverflow_chpl(coerce_tmp_chpl6, UINT8(true), INT64(360), INT32(65));
#line 360 "../lib/Chai.chpl"
    i_chpl3 = INT64(0);
#line 360 "../lib/Chai.chpl"
    this_chpl34 = _ic__F0_this_chpl2;
#line 360 "../lib/Chai.chpl"
    this_chpl35 = _ic__F0_this_chpl2;
#line 360 "../lib/Chai.chpl"
    end_chpl2 = (&this_chpl35)->_high;
#line 360 "../lib/Chai.chpl"
    for (i_chpl3 = (&this_chpl34)->_low; ((i_chpl3 <= end_chpl2)); i_chpl3 += INT64(1)) {
#line 360 "../lib/Chai.chpl"
      i_chpl4 = i_chpl3;
#line 360 "../lib/Chai.chpl"
      coerce_tmp_chpl7 = *(coerce_tmp_chpl4 + INT64(2));
#line 360 "../lib/Chai.chpl"
      _ic__F0_this_chpl3 = coerce_tmp_chpl7;
#line 360 "../lib/Chai.chpl"
      checkIfIterWillOverflow_chpl(coerce_tmp_chpl7, UINT8(true), INT64(360), INT32(65));
#line 360 "../lib/Chai.chpl"
      i_chpl5 = INT64(0);
#line 360 "../lib/Chai.chpl"
      this_chpl36 = _ic__F0_this_chpl3;
#line 360 "../lib/Chai.chpl"
      this_chpl37 = _ic__F0_this_chpl3;
#line 360 "../lib/Chai.chpl"
      end_chpl3 = (&this_chpl37)->_high;
#line 360 "../lib/Chai.chpl"
      for (i_chpl5 = (&this_chpl36)->_low; ((i_chpl5 <= end_chpl3)); i_chpl5 += INT64(1)) {
#line 360 "../lib/Chai.chpl"
        *(ret_tmp_chpl2 + INT64(0)) = i_chpl4;
#line 360 "../lib/Chai.chpl"
        *(ret_tmp_chpl2 + INT64(1)) = i_chpl5;
#line 360 "../lib/Chai.chpl"
        *(_yieldedIndex_chpl + INT64(0)) = *(ret_tmp_chpl2 + INT64(0));
#line 360 "../lib/Chai.chpl"
        *(_yieldedIndex_chpl + INT64(1)) = *(ret_tmp_chpl2 + INT64(1));
#line 360 "../lib/Chai.chpl"
        j_x0_chpl = *(_yieldedIndex_chpl + INT64(0));
#line 360 "../lib/Chai.chpl"
        j_x1_chpl = *(_yieldedIndex_chpl + INT64(1));
#line 360 "../lib/Chai.chpl"
        *(ret_tmp_chpl3 + INT64(0)) = i_chpl2;
#line 360 "../lib/Chai.chpl"
        *(ret_tmp_chpl3 + INT64(1)) = j_x0_chpl;
#line 360 "../lib/Chai.chpl"
        *(ret_tmp_chpl3 + INT64(2)) = j_x1_chpl;
#line 360 "../lib/Chai.chpl"
        *(yret_chpl + INT64(0)) = *(ret_tmp_chpl3 + INT64(0));
#line 360 "../lib/Chai.chpl"
        *(yret_chpl + INT64(1)) = *(ret_tmp_chpl3 + INT64(1));
#line 360 "../lib/Chai.chpl"
        *(yret_chpl + INT64(2)) = *(ret_tmp_chpl3 + INT64(2));
#line 360 "../lib/Chai.chpl"
        chpl_idx_1_x0 = *(yret_chpl + INT64(0));
#line 360 "../lib/Chai.chpl"
        chpl_idx_1_x1 = *(yret_chpl + INT64(1));
#line 360 "../lib/Chai.chpl"
        chpl_idx_1_x2 = *(yret_chpl + INT64(2));
#line 361 "../lib/Chai.chpl"
        chpl_build_low_bounded_range2(((int64_t)((chpl_idx_1_x0 * INT64(2)))), &ret_tmp_chpl4);
#line 361 "../lib/Chai.chpl"
        call_tmp_chpl15 = ret_tmp_chpl4;
#line 361 "../lib/Chai.chpl"
        chpl___POUND_3(call_tmp_chpl15, INT64(2), &ret_tmp_chpl5, INT64(361), INT32(65));
#line 361 "../lib/Chai.chpl"
        call_tmp_chpl16 = ret_tmp_chpl5;
#line 361 "../lib/Chai.chpl"
        chpl_build_low_bounded_range2(((int64_t)((chpl_idx_1_x1 * INT64(2)))), &ret_tmp_chpl6);
#line 361 "../lib/Chai.chpl"
        call_tmp_chpl17 = ret_tmp_chpl6;
#line 361 "../lib/Chai.chpl"
        chpl___POUND_3(call_tmp_chpl17, INT64(2), &ret_tmp_chpl7, INT64(361), INT32(65));
#line 361 "../lib/Chai.chpl"
        call_tmp_chpl18 = ret_tmp_chpl7;
#line 361 "../lib/Chai.chpl"
        chpl_build_unbounded_range(&ret_tmp_chpl8);
#line 361 "../lib/Chai.chpl"
        call_tmp_chpl19 = ret_tmp_chpl8;
#line 98 "../lib/Tensor.chpl"
        call_tmp_chpl21 = &((convs_chpl)->data);
#line 361 "../lib/Chai.chpl"
        this_chpl3(call_tmp_chpl21, call_tmp_chpl16, call_tmp_chpl18, call_tmp_chpl19, &ret_tmp_chpl9, INT64(361), INT32(65));
#line 361 "../lib/Chai.chpl"
        call_tmp_chpl20 = ret_tmp_chpl9;
#line 361 "../lib/Chai.chpl"
        region_chpl = chpl__initCopy6(&call_tmp_chpl20, UINT8(true), INT64(361), INT32(65));
#line 361 "../lib/Chai.chpl"
        chpl__autoDestroy17(&call_tmp_chpl20, INT64(361), INT32(65));
#line 362 "../lib/Chai.chpl"
        *(coerce_tmp_chpl8 + INT64(0)) = chpl_idx_1_x0;
#line 362 "../lib/Chai.chpl"
        *(coerce_tmp_chpl8 + INT64(1)) = chpl_idx_1_x1;
#line 362 "../lib/Chai.chpl"
        *(coerce_tmp_chpl8 + INT64(2)) = chpl_idx_1_x2;
#line 362 "../lib/Chai.chpl"
        i_i_chpl = &coerce_tmp_chpl8;
#line 362 "../lib/Chai.chpl"
        checkAccess_chpl9(pools_chpl, i_i_chpl, (pools_chpl)->_instance, INT64(362), INT32(65));
#line 362 "../lib/Chai.chpl"
        ret_chpl = (pools_chpl)->_instance;
#line 362 "../lib/Chai.chpl"
        checkAccess_chpl9(pools_chpl, i_i_chpl, ret_chpl, INT64(362), INT32(65));
#line 362 "../lib/Chai.chpl"
        coerce_tmp_chpl9 = ret_chpl;
#line 362 "../lib/Chai.chpl"
        *(_formal_tmp_in_ind_chpl + INT64(0)) = *(coerce_tmp_chpl8 + INT64(0));
#line 362 "../lib/Chai.chpl"
        *(_formal_tmp_in_ind_chpl + INT64(1)) = *(coerce_tmp_chpl8 + INT64(1));
#line 362 "../lib/Chai.chpl"
        *(_formal_tmp_in_ind_chpl + INT64(2)) = *(coerce_tmp_chpl8 + INT64(2));
#line 362 "../lib/Chai.chpl"
        _formal_tmp_ind_x0_chpl = *(_formal_tmp_in_ind_chpl + INT64(0));
#line 362 "../lib/Chai.chpl"
        _formal_tmp_ind_x1_chpl = *(_formal_tmp_in_ind_chpl + INT64(1));
#line 362 "../lib/Chai.chpl"
        _formal_tmp_ind_x2_chpl = *(_formal_tmp_in_ind_chpl + INT64(2));
#line 362 "../lib/Chai.chpl"
        sum_chpl = INT64(0);
#line 362 "../lib/Chai.chpl"
        chpl_check_nil(coerce_tmp_chpl9, INT64(362), INT32(65));
#line 362 "../lib/Chai.chpl"
        call_tmp_chpl22 = &((coerce_tmp_chpl9)->blk);
#line 362 "../lib/Chai.chpl"
        i_lhs_chpl = &sum_chpl;
#line 362 "../lib/Chai.chpl"
        *(i_lhs_chpl) += ((int64_t)((_formal_tmp_ind_x0_chpl * *(*(call_tmp_chpl22) + INT64(0)))));
#line 362 "../lib/Chai.chpl"
        chpl_check_nil(coerce_tmp_chpl9, INT64(362), INT32(65));
#line 362 "../lib/Chai.chpl"
        call_tmp_chpl23 = &((coerce_tmp_chpl9)->blk);
#line 362 "../lib/Chai.chpl"
        i_lhs_chpl2 = &sum_chpl;
#line 362 "../lib/Chai.chpl"
        *(i_lhs_chpl2) += ((int64_t)((_formal_tmp_ind_x1_chpl * *(*(call_tmp_chpl23) + INT64(1)))));
#line 362 "../lib/Chai.chpl"
        i_lhs_chpl3 = &sum_chpl;
#line 362 "../lib/Chai.chpl"
        *(i_lhs_chpl3) += _formal_tmp_ind_x2_chpl;
#line 362 "../lib/Chai.chpl"
        ret_chpl2 = sum_chpl;
#line 362 "../lib/Chai.chpl"
        chpl_check_nil(coerce_tmp_chpl9, INT64(362), INT32(65));
#line 362 "../lib/Chai.chpl"
        coerce_tmp_chpl10 = (coerce_tmp_chpl9)->shiftedData;
#line 362 "../lib/Chai.chpl"
        call_tmp_chpl24 = (coerce_tmp_chpl10 + ret_chpl2);
#line 362 "../lib/Chai.chpl"
        chpl_build_unbounded_range(&ret_tmp_chpl10);
#line 362 "../lib/Chai.chpl"
        call_tmp_chpl25 = ret_tmp_chpl10;
#line 362 "../lib/Chai.chpl"
        chpl_build_unbounded_range(&ret_tmp_chpl11);
#line 362 "../lib/Chai.chpl"
        call_tmp_chpl26 = ret_tmp_chpl11;
#line 362 "../lib/Chai.chpl"
        this_chpl4(&region_chpl, call_tmp_chpl25, call_tmp_chpl26, chpl_idx_1_x2, &ret_tmp_chpl12, INT64(362), INT32(65));
#line 362 "../lib/Chai.chpl"
        call_tmp_chpl27 = ret_tmp_chpl12;
#line 362 "../lib/Chai.chpl"
        init_chpl105(&l_chpl);
#line 362 "../lib/Chai.chpl"
        ret_chpl3 = l_chpl;
#line 362 "../lib/Chai.chpl"
        default_arg_l_chpl = ret_chpl3;
#line 362 "../lib/Chai.chpl"
        value_chpl = min_chpl();
#line 362 "../lib/Chai.chpl"
        new_temp_chpl = _new_chpl26(&default_arg_l_chpl, value_chpl, INT64(362), INT32(65));
#line 362 "../lib/Chai.chpl"
        globalRP_call_tmp_chpl = new_temp_chpl;
#line 362 "../lib/Chai.chpl"
        coerce_tmp_chpl11 = globalRP_call_tmp_chpl;
#line 362 "../lib/Chai.chpl"
        chpl_check_nil(coerce_tmp_chpl11, INT64(362), INT32(65));
#line 362 "../lib/Chai.chpl"
        call_tmp_chpl28 = identity_chpl2(coerce_tmp_chpl11);
#line 362 "../lib/Chai.chpl"
        globalAS_chpl_redSVar_chpl = call_tmp_chpl28;
#line 362 "../lib/Chai.chpl"
        ret_chpl4 = (&call_tmp_chpl27)->_instance;
#line 362 "../lib/Chai.chpl"
        coerce_tmp_chpl12 = ret_chpl4;
#line 362 "../lib/Chai.chpl"
        error_chpl = NULL;
#line 362 "../lib/Chai.chpl"
        chpl_check_nil(coerce_tmp_chpl12, INT64(362), INT32(65));
#line 362 "../lib/Chai.chpl"
        ret_chpl5 = (coerce_tmp_chpl12)->dom;
#line 362 "../lib/Chai.chpl"
        coerce_tmp_chpl13 = ret_chpl5;
#line 362 "../lib/Chai.chpl"
        error_chpl2 = NULL;
#line 362 "../lib/Chai.chpl"
        this_chpl38 = ret_chpl4;
#line 362 "../lib/Chai.chpl"
        chpl_check_nil(coerce_tmp_chpl13, INT64(362), INT32(65));
#line 362 "../lib/Chai.chpl"
        coerce_tmp_chpl14 = (coerce_tmp_chpl13)->upDomInst;
#line 362 "../lib/Chai.chpl"
        if (((RootClass_chpl)(coerce_tmp_chpl14)) == nil) /* ZLINE: 362 ../lib/Chai.chpl */
#line 362 "../lib/Chai.chpl"
        {
#line 362 "../lib/Chai.chpl"
          nilCheckHalt_chpl(&_str_literal_52_chpl, INT64(362), INT32(65));
#line 362 "../lib/Chai.chpl"
        }
#line 362 "../lib/Chai.chpl"
        chpl_check_nil(coerce_tmp_chpl13, INT64(362), INT32(65));
#line 362 "../lib/Chai.chpl"
        if (((RootClass_chpl)((coerce_tmp_chpl13)->upDomInst)) == nil) /* ZLINE: 362 ../lib/Chai.chpl */
#line 362 "../lib/Chai.chpl"
        {
#line 362 "../lib/Chai.chpl"
          nilCheckHalt_chpl(&_str_literal_52_chpl, INT64(362), INT32(65));
#line 362 "../lib/Chai.chpl"
        }
#line 362 "../lib/Chai.chpl"
        coerce_tmp_chpl15 = coerce_tmp_chpl14;
#line 362 "../lib/Chai.chpl"
        tasksPerLocale_chpl = local_dataParTasksPerLocale_chpl;
#line 362 "../lib/Chai.chpl"
        ignoreRunning_chpl = local_dataParIgnoreRunningTasks_chpl;
#line 362 "../lib/Chai.chpl"
        minIndicesPerTask_chpl = local_dataParMinGranularity_chpl;
#line 362 "../lib/Chai.chpl"
        createTuple_chpl4(INT64(0), &ret_tmp_chpl13);
#line 362 "../lib/Chai.chpl"
        error_chpl3 = NULL;
#line 362 "../lib/Chai.chpl"
        if (tasksPerLocale_chpl == INT64(0)) /* ZLINE: 362 ../lib/Chai.chpl */
#line 362 "../lib/Chai.chpl"
        {
#line 362 "../lib/Chai.chpl"
          call_tmp_chpl31 = get_chpl_nodeID();
#line 362 "../lib/Chai.chpl"
          call_tmp_chpl32 = chpl_rt_buildLocaleID(call_tmp_chpl31, c_sublocid_any);
#line 362 "../lib/Chai.chpl"
          call_tmp_chpl30 = call_tmp_chpl32;
#line 362 "../lib/Chai.chpl"
          chpl_localeID_to_locale(&call_tmp_chpl30, &ret_tmp_chpl14, INT64(362), INT32(65));
#line 362 "../lib/Chai.chpl"
          call_tmp_chpl33 = ret_tmp_chpl14;
#line 362 "../lib/Chai.chpl"
          call_tmp_chpl29 = call_tmp_chpl33;
#line 362 "../lib/Chai.chpl"
          coerce_tmp_chpl16 = (&call_tmp_chpl29)->_instance;
#line 362 "../lib/Chai.chpl"
          if (((RootClass_chpl)(coerce_tmp_chpl16)) == nil) /* ZLINE: 362 ../lib/Chai.chpl */
#line 362 "../lib/Chai.chpl"
          {
#line 362 "../lib/Chai.chpl"
            nilCheckHalt_chpl(&_str_literal_52_chpl, INT64(362), INT32(65));
#line 362 "../lib/Chai.chpl"
          }
#line 362 "../lib/Chai.chpl"
          coerce_tmp_chpl17 = coerce_tmp_chpl16;
#line 362 "../lib/Chai.chpl"
          chpl_check_nil(coerce_tmp_chpl17, INT64(362), INT32(65));
#line 362 "../lib/Chai.chpl"
          ret_chpl6 = (coerce_tmp_chpl17)->maxTaskPar;
#line 362 "../lib/Chai.chpl"
          tmp_chpl = ret_chpl6;
#line 362 "../lib/Chai.chpl"
          i_x_chpl = &call_tmp_chpl29;
#line 362 "../lib/Chai.chpl"
          deinit_chpl62(i_x_chpl);
#line 362 "../lib/Chai.chpl"
        } else /* ZLINE: 362 ../lib/Chai.chpl */
#line 362 "../lib/Chai.chpl"
        {
#line 362 "../lib/Chai.chpl"
          tmp_chpl = tasksPerLocale_chpl;
#line 362 "../lib/Chai.chpl"
        }
#line 362 "../lib/Chai.chpl"
        if (chpl_task_getSerial()) /* ZLINE: 362 ../lib/Chai.chpl */
#line 362 "../lib/Chai.chpl"
        {
#line 362 "../lib/Chai.chpl"
          tmp_x0_chpl2 = INT64(1);
#line 362 "../lib/Chai.chpl"
          tmp_x1_chpl2 = INT64(0);
#line 362 "../lib/Chai.chpl"
        } else /* ZLINE: 362 ../lib/Chai.chpl */
#line 362 "../lib/Chai.chpl"
        {
#line 362 "../lib/Chai.chpl"
          chpl_check_nil(coerce_tmp_chpl15, INT64(362), INT32(65));
#line 362 "../lib/Chai.chpl"
          call_tmp_chpl34 = &((coerce_tmp_chpl15)->ranges);
#line 362 "../lib/Chai.chpl"
          _computeChunkStuff_chpl4(tmp_chpl, ignoreRunning_chpl, minIndicesPerTask_chpl, call_tmp_chpl34, &ret_tmp_chpl15, INT64(362), INT32(65));
#line 362 "../lib/Chai.chpl"
          tmp_x0_chpl2 = *(ret_tmp_chpl15 + INT64(0));
#line 362 "../lib/Chai.chpl"
          tmp_x1_chpl2 = *(ret_tmp_chpl15 + INT64(1));
#line 362 "../lib/Chai.chpl"
        }
#line 362 "../lib/Chai.chpl"
        numChunks_chpl2 = tmp_x0_chpl2;
#line 362 "../lib/Chai.chpl"
        parDim_chpl2 = tmp_x1_chpl2;
#line 362 "../lib/Chai.chpl"
        chpl_build_low_bounded_range2(INT64(0), &ret_tmp_chpl16);
#line 362 "../lib/Chai.chpl"
        call_tmp_chpl35 = ret_tmp_chpl16;
#line 362 "../lib/Chai.chpl"
        chpl___POUND_3(call_tmp_chpl35, tmp_x0_chpl2, &ret_tmp_chpl17, INT64(362), INT32(65));
#line 362 "../lib/Chai.chpl"
        tmpIter_chpl = ret_tmp_chpl17;
#line 362 "../lib/Chai.chpl"
        new_temp_chpl2 = _new_chpl17(INT64(362), INT32(65));
#line 362 "../lib/Chai.chpl"
        _coforallCount_chpl2 = new_temp_chpl2;
#line 362 "../lib/Chai.chpl"
        numTasks_chpl = chpl_boundedCoforallSize(tmpIter_chpl, INT64(362), INT32(65));
#line 362 "../lib/Chai.chpl"
        _upEndCount_chpl(_coforallCount_chpl2, numTasks_chpl, INT64(362), INT32(65));
#line 362 "../lib/Chai.chpl"
        chpl_resetTaskSpawn(numTasks_chpl, INT64(362), INT32(65));
#line 362 "../lib/Chai.chpl"
        _ic__F0_this_chpl4 = tmpIter_chpl;
#line 362 "../lib/Chai.chpl"
        checkIfIterWillOverflow_chpl(tmpIter_chpl, UINT8(true), INT64(362), INT32(65));
#line 362 "../lib/Chai.chpl"
        i_chpl6 = INT64(0);
#line 362 "../lib/Chai.chpl"
        this_chpl39 = _ic__F0_this_chpl4;
#line 362 "../lib/Chai.chpl"
        this_chpl40 = _ic__F0_this_chpl4;
#line 362 "../lib/Chai.chpl"
        end_chpl4 = (&this_chpl40)->_high;
#line 362 "../lib/Chai.chpl"
        for (i_chpl6 = (&this_chpl39)->_low; ((i_chpl6 <= end_chpl4)); i_chpl6 += INT64(1)) {
#line 362 "../lib/Chai.chpl"
          _args_forcoforall_fn_chpl = ((_class_localscoforall_fn_chpl107)(&chpl_macro_tmp_112));
#line 362 "../lib/Chai.chpl"
          chpl_check_nil(_args_forcoforall_fn_chpl, INT64(362), INT32(65));
#line 362 "../lib/Chai.chpl"
          (_args_forcoforall_fn_chpl)->_0_numChunks = numChunks_chpl2;
#line 362 "../lib/Chai.chpl"
          chpl_check_nil(_args_forcoforall_fn_chpl, INT64(362), INT32(65));
#line 362 "../lib/Chai.chpl"
          (_args_forcoforall_fn_chpl)->_1_parDim = parDim_chpl2;
#line 362 "../lib/Chai.chpl"
          chpl_check_nil(_args_forcoforall_fn_chpl, INT64(362), INT32(65));
#line 362 "../lib/Chai.chpl"
          (_args_forcoforall_fn_chpl)->_2_coerce_tmp = coerce_tmp_chpl15;
#line 362 "../lib/Chai.chpl"
          chpl_check_nil(_args_forcoforall_fn_chpl, INT64(362), INT32(65));
#line 362 "../lib/Chai.chpl"
          (_args_forcoforall_fn_chpl)->_3__coforallCount = _coforallCount_chpl2;
#line 362 "../lib/Chai.chpl"
          chpl_check_nil(_args_forcoforall_fn_chpl, INT64(362), INT32(65));
#line 362 "../lib/Chai.chpl"
          (_args_forcoforall_fn_chpl)->_4_chunk = i_chpl6;
#line 362 "../lib/Chai.chpl"
          chpl_check_nil(_args_forcoforall_fn_chpl, INT64(362), INT32(65));
#line 362 "../lib/Chai.chpl"
          (_args_forcoforall_fn_chpl)->_6_this = this_chpl38;
#line 362 "../lib/Chai.chpl"
          chpl_check_nil(_args_forcoforall_fn_chpl, INT64(362), INT32(65));
#line 362 "../lib/Chai.chpl"
          (_args_forcoforall_fn_chpl)->_7_globalRP_call_tmp = globalRP_call_tmp_chpl;
#line 362 "../lib/Chai.chpl"
          _args_vforcoforall_fn_chpl = ((void*)(_args_forcoforall_fn_chpl));
#line 362 "../lib/Chai.chpl"
          /*** wrapcoforall_fn_chpl107 ***/ chpl_taskAddCoStmt(chpl_task_getRequestedSubloc(), INT64(106), ((chpl_task_bundle_p)(_args_vforcoforall_fn_chpl)), sizeof(chpl__class_localscoforall_fn_chpl107_object), INT64(362), INT32(65));
#line 362 "../lib/Chai.chpl"
        }
#line 362 "../lib/Chai.chpl"
        error_chpl4 = NULL;
#line 362 "../lib/Chai.chpl"
        _waitEndCount_chpl2(_coforallCount_chpl2, numTasks_chpl, &error_chpl4, INT64(362), INT32(65));
#line 362 "../lib/Chai.chpl"
        if (error_chpl4 != nil) /* ZLINE: 362 ../lib/Chai.chpl */
#line 362 "../lib/Chai.chpl"
        {
#line 362 "../lib/Chai.chpl"
          chpl__delete72(_coforallCount_chpl2, INT64(362), INT32(65));
#line 362 "../lib/Chai.chpl"
          error_chpl3 = error_chpl4;
#line 362 "../lib/Chai.chpl"
          goto forall_IBB_break_label_chpl;
#line 362 "../lib/Chai.chpl"
        }
#line 362 "../lib/Chai.chpl"
        chpl__delete72(_coforallCount_chpl2, INT64(362), INT32(65));
#line 362 "../lib/Chai.chpl"
        chpl_after_forall_fence();
#line 362 "../lib/Chai.chpl"
        forall_IBB_break_label_chpl:;
#line 362 "../lib/Chai.chpl"
        if (error_chpl3 != nil) /* ZLINE: 362 ../lib/Chai.chpl */
#line 362 "../lib/Chai.chpl"
        {
#line 362 "../lib/Chai.chpl"
          error_chpl2 = error_chpl3;
#line 362 "../lib/Chai.chpl"
          goto forall_IBB_break_label_chpl2;
#line 362 "../lib/Chai.chpl"
        }
#line 362 "../lib/Chai.chpl"
        chpl_after_forall_fence();
#line 362 "../lib/Chai.chpl"
        forall_IBB_break_label_chpl2:;
#line 362 "../lib/Chai.chpl"
        if (error_chpl2 != nil) /* ZLINE: 362 ../lib/Chai.chpl */
#line 362 "../lib/Chai.chpl"
        {
#line 362 "../lib/Chai.chpl"
          error_chpl = error_chpl2;
#line 362 "../lib/Chai.chpl"
          goto forall_IBB_break_label_chpl3;
#line 362 "../lib/Chai.chpl"
        }
#line 362 "../lib/Chai.chpl"
        chpl_after_forall_fence();
#line 362 "../lib/Chai.chpl"
        forall_IBB_break_label_chpl3:;
#line 362 "../lib/Chai.chpl"
        if (error_chpl != nil) /* ZLINE: 362 ../lib/Chai.chpl */
#line 362 "../lib/Chai.chpl"
        {
#line 362 "../lib/Chai.chpl"
          chpl_propagate_error(error_chpl, INT64(362), INT32(65));
#line 362 "../lib/Chai.chpl"
        }
#line 362 "../lib/Chai.chpl"
        chpl_after_forall_fence();
#line 362 "../lib/Chai.chpl"
        coerce_tmp_chpl18 = globalRP_call_tmp_chpl;
#line 362 "../lib/Chai.chpl"
        chpl_check_nil(coerce_tmp_chpl18, INT64(362), INT32(65));
#line 362 "../lib/Chai.chpl"
        accumulate_chpl2(coerce_tmp_chpl18, globalAS_chpl_redSVar_chpl, INT64(362), INT32(65));
#line 362 "../lib/Chai.chpl"
        coerce_tmp_chpl19 = globalRP_call_tmp_chpl;
#line 362 "../lib/Chai.chpl"
        chpl_check_nil(coerce_tmp_chpl19, INT64(362), INT32(65));
#line 362 "../lib/Chai.chpl"
        call_tmp_chpl36 = generate_chpl2(coerce_tmp_chpl19, INT64(362), INT32(65));
#line 362 "../lib/Chai.chpl"
        chpl__delete(globalRP_call_tmp_chpl, INT64(362), INT32(65));
#line 362 "../lib/Chai.chpl"
        *(call_tmp_chpl24) = call_tmp_chpl36;
#line 362 "../lib/Chai.chpl"
        chpl__autoDestroy15(&call_tmp_chpl27, INT64(362), INT32(65));
#line 362 "../lib/Chai.chpl"
        chpl__autoDestroy28(&region_chpl, INT64(362), INT32(65));
#line 362 "../lib/Chai.chpl"
      }
#line 362 "../lib/Chai.chpl"
    }
#line 362 "../lib/Chai.chpl"
  }
#line 360 "../lib/Chai.chpl"
  return;
#line 360 "../lib/Chai.chpl"
}

#line 362 "../lib/Chai.chpl"
/* Chai.chpl:362 */
#line 362 "../lib/Chai.chpl"
static void wrapcoforall_fn_chpl107(_class_localscoforall_fn_chpl107 c_chpl) {
#line 362 "../lib/Chai.chpl"
  Error_chpl error_chpl = NULL;
#line 362 "../lib/Chai.chpl"
  int64_t _0_numChunks_chpl;
#line 362 "../lib/Chai.chpl"
  int64_t _1_parDim_chpl;
#line 362 "../lib/Chai.chpl"
  DefaultRectangularDom_2_int64_t_one_chpl _2_coerce_tmp_chpl = NULL;
#line 362 "../lib/Chai.chpl"
  chpl___EndCount_AtomicT_int64_t_int64_t _3__coforallCount_chpl = NULL;
#line 362 "../lib/Chai.chpl"
  int64_t _4_chunk_chpl;
#line 362 "../lib/Chai.chpl"
  ArrayViewRankChangeArr__real64_int64_t_ArrayViewRankChangeDom_2_int64_t_one__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_DefaultRectangularArr_3_int64_t_one__real64_int64_t__tuple_3_star_chpl_bool__tuple_3_star_int64_t__remoteAccessData__real64_2_int64_t_one_T_F_chpl _6_this_chpl = NULL;
#line 362 "../lib/Chai.chpl"
  chpl_rmem_consist_acquire(INT64(362), INT32(65));
#line 362 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(362), INT32(65));
#line 362 "../lib/Chai.chpl"
  _0_numChunks_chpl = (c_chpl)->_0_numChunks;
#line 362 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(362), INT32(65));
#line 362 "../lib/Chai.chpl"
  _1_parDim_chpl = (c_chpl)->_1_parDim;
#line 362 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(362), INT32(65));
#line 362 "../lib/Chai.chpl"
  _2_coerce_tmp_chpl = (c_chpl)->_2_coerce_tmp;
#line 362 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(362), INT32(65));
#line 362 "../lib/Chai.chpl"
  _3__coforallCount_chpl = (c_chpl)->_3__coforallCount;
#line 362 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(362), INT32(65));
#line 362 "../lib/Chai.chpl"
  _4_chunk_chpl = (c_chpl)->_4_chunk;
#line 362 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(362), INT32(65));
#line 362 "../lib/Chai.chpl"
  _6_this_chpl = (c_chpl)->_6_this;
#line 362 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(362), INT32(65));
#line 362 "../lib/Chai.chpl"
  coforall_fn_chpl107(_0_numChunks_chpl, _1_parDim_chpl, _2_coerce_tmp_chpl, _3__coforallCount_chpl, _4_chunk_chpl, &error_chpl, _6_this_chpl, (c_chpl)->_7_globalRP_call_tmp);
#line 362 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(362), INT32(65));
#line 362 "../lib/Chai.chpl"
  _downEndCount_chpl((c_chpl)->_3__coforallCount, error_chpl, INT64(362), INT32(65));
#line 362 "../lib/Chai.chpl"
  return;
#line 362 "../lib/Chai.chpl"
}

#line 362 "../lib/Chai.chpl"
/* Chai.chpl:362 */
#line 362 "../lib/Chai.chpl"
static void coforall_fn_chpl107(int64_t numChunks_chpl,
#line 362 "../lib/Chai.chpl"
                                int64_t parDim_chpl,
#line 362 "../lib/Chai.chpl"
                                DefaultRectangularDom_2_int64_t_one_chpl this_chpl31,
#line 362 "../lib/Chai.chpl"
                                chpl___EndCount_AtomicT_int64_t_int64_t _coforallCount_chpl,
#line 362 "../lib/Chai.chpl"
                                int64_t chunk_chpl,
#line 362 "../lib/Chai.chpl"
                                Error_chpl * error_out_chpl,
#line 362 "../lib/Chai.chpl"
                                ArrayViewRankChangeArr__real64_int64_t_ArrayViewRankChangeDom_2_int64_t_one__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_DefaultRectangularArr_3_int64_t_one__real64_int64_t__tuple_3_star_chpl_bool__tuple_3_star_int64_t__remoteAccessData__real64_2_int64_t_one_T_F_chpl this_chpl32,
#line 362 "../lib/Chai.chpl"
                                MaxReduceScanOp__real64_chpl RP_chpl_redSVar_chpl) {
#line 362 "../lib/Chai.chpl"
  MaxReduceScanOp__real64_chpl RP_chpl_redSVar_chpl2 = NULL;
#line 362 "../lib/Chai.chpl"
  _real64 AS_chpl_redSVar_chpl;
#line 362 "../lib/Chai.chpl"
  MaxReduceScanOp__real64_chpl call_tmp_chpl6 = NULL;
#line 362 "../lib/Chai.chpl"
  _real64 call_tmp_chpl7;
#line 362 "../lib/Chai.chpl"
  _tuple_2_star_range_int64_t_both_one_chpl ranges_chpl;
#line 362 "../lib/Chai.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl8 = NULL;
#line 362 "../lib/Chai.chpl"
  uint64_t call_tmp_chpl9;
#line 362 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl3;
#line 362 "../lib/Chai.chpl"
  uint64_t call_tmp_chpl10;
#line 362 "../lib/Chai.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl11 = NULL;
#line 362 "../lib/Chai.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl12 = NULL;
#line 362 "../lib/Chai.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl13 = NULL;
#line 362 "../lib/Chai.chpl"
  _tuple_2_star_int64_t_chpl call_tmp_chpl14;
#line 362 "../lib/Chai.chpl"
  _tuple_2_star_int64_t_chpl ret_tmp_chpl;
#line 362 "../lib/Chai.chpl"
  int64_t tmp_x0_chpl;
#line 362 "../lib/Chai.chpl"
  int64_t tmp_x1_chpl;
#line 362 "../lib/Chai.chpl"
  _tuple_2_star_range_int64_t_both_one_chpl block_chpl;
#line 362 "../lib/Chai.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl15 = NULL;
#line 362 "../lib/Chai.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl16 = NULL;
#line 362 "../lib/Chai.chpl"
  _tuple_2_star_range_int64_t_both_one_chpl coerce_tmp_chpl4;
#line 362 "../lib/Chai.chpl"
  _tuple_2_star_range_int64_t_both_one_chpl retcopy_chpl;
#line 362 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl5;
#line 362 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl _ic__F0_this_chpl;
#line 362 "../lib/Chai.chpl"
  int64_t i_chpl;
#line 362 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl this_chpl33;
#line 362 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl this_chpl34;
#line 362 "../lib/Chai.chpl"
  int64_t end_chpl;
#line 362 "../lib/Chai.chpl"
  int64_t i_chpl2;
#line 362 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl6;
#line 362 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl _ic__F0_this_chpl2;
#line 362 "../lib/Chai.chpl"
  int64_t i_chpl3;
#line 362 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl this_chpl35;
#line 362 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl this_chpl36;
#line 362 "../lib/Chai.chpl"
  int64_t end_chpl2;
#line 362 "../lib/Chai.chpl"
  _tuple_2_star_int64_t_chpl ret_tmp_chpl2;
#line 362 "../lib/Chai.chpl"
  _tuple_2_star_int64_t_chpl i_chpl4;
#line 362 "../lib/Chai.chpl"
  _ref__remoteAccessData__real64_2_int64_t_one_T_chpl call_tmp_chpl17 = NULL;
#line 362 "../lib/Chai.chpl"
  _ref__remoteAccessData__real64_2_int64_t_one_T_chpl call_tmp_chpl18 = NULL;
#line 362 "../lib/Chai.chpl"
  int64_t retcopy_x1_chpl;
#line 362 "../lib/Chai.chpl"
  int64_t ret_chpl;
#line 362 "../lib/Chai.chpl"
  int64_t sum_chpl;
#line 362 "../lib/Chai.chpl"
  _ref__tuple_2_star_int64_t_chpl call_tmp_chpl19 = NULL;
#line 362 "../lib/Chai.chpl"
  _ref_int64_t_chpl i_lhs_chpl = NULL;
#line 362 "../lib/Chai.chpl"
  _ref__tuple_2_star_int64_t_chpl call_tmp_chpl20 = NULL;
#line 362 "../lib/Chai.chpl"
  _ref_int64_t_chpl i_lhs_chpl2 = NULL;
#line 362 "../lib/Chai.chpl"
  _ddata__real64_chpl coerce_tmp_chpl7 = NULL;
#line 362 "../lib/Chai.chpl"
  _ref__real64_chpl call_tmp_chpl21 = NULL;
#line 362 "../lib/Chai.chpl"
  _real64 coerce_tmp_chpl8;
#line 362 "../lib/Chai.chpl"
  MaxReduceScanOp__real64_chpl coerce_tmp_chpl9 = NULL;
#line 362 "../lib/Chai.chpl"
  chpl_check_nil(RP_chpl_redSVar_chpl, INT64(362), INT32(65));
#line 362 "../lib/Chai.chpl"
  call_tmp_chpl6 = clone_chpl3(RP_chpl_redSVar_chpl, INT64(362), INT32(65));
#line 362 "../lib/Chai.chpl"
  RP_chpl_redSVar_chpl2 = call_tmp_chpl6;
#line 362 "../lib/Chai.chpl"
  chpl_check_nil(call_tmp_chpl6, INT64(362), INT32(65));
#line 362 "../lib/Chai.chpl"
  call_tmp_chpl7 = identity_chpl2(call_tmp_chpl6);
#line 362 "../lib/Chai.chpl"
  AS_chpl_redSVar_chpl = call_tmp_chpl7;
#line 362 "../lib/Chai.chpl"
  chpl_check_nil(this_chpl31, INT64(362), INT32(65));
#line 362 "../lib/Chai.chpl"
  *(ranges_chpl + INT64(0)) = *((this_chpl31)->ranges + INT64(0));
#line 362 "../lib/Chai.chpl"
  *(ranges_chpl + INT64(1)) = *((this_chpl31)->ranges + INT64(1));
#line 362 "../lib/Chai.chpl"
  call_tmp_chpl8 = this_chpl26(&ranges_chpl, parDim_chpl, INT64(362), INT32(65));
#line 362 "../lib/Chai.chpl"
  coerce_tmp_chpl3 = *(call_tmp_chpl8);
#line 362 "../lib/Chai.chpl"
  call_tmp_chpl9 = sizeAs_chpl36(coerce_tmp_chpl3, INT64(362), INT32(65));
#line 362 "../lib/Chai.chpl"
  this_chpl26(&ranges_chpl, parDim_chpl, INT64(362), INT32(65));
#line 362 "../lib/Chai.chpl"
  call_tmp_chpl10 = (call_tmp_chpl9 * UINT64(1));
#line 362 "../lib/Chai.chpl"
  call_tmp_chpl11 = this_chpl26(&ranges_chpl, parDim_chpl, INT64(362), INT32(65));
#line 362 "../lib/Chai.chpl"
  call_tmp_chpl12 = this_chpl26(&ranges_chpl, parDim_chpl, INT64(362), INT32(65));
#line 362 "../lib/Chai.chpl"
  call_tmp_chpl13 = this_chpl26(&ranges_chpl, parDim_chpl, INT64(362), INT32(65));
#line 362 "../lib/Chai.chpl"
  _computeBlock_chpl2(call_tmp_chpl10, numChunks_chpl, chunk_chpl, (call_tmp_chpl11)->_high, (call_tmp_chpl12)->_low, (call_tmp_chpl13)->_low, &ret_tmp_chpl, INT64(362), INT32(65));
#line 362 "../lib/Chai.chpl"
  *(call_tmp_chpl14 + INT64(0)) = *(ret_tmp_chpl + INT64(0));
#line 362 "../lib/Chai.chpl"
  *(call_tmp_chpl14 + INT64(1)) = *(ret_tmp_chpl + INT64(1));
#line 362 "../lib/Chai.chpl"
  tmp_x0_chpl = *(call_tmp_chpl14 + INT64(0));
#line 362 "../lib/Chai.chpl"
  tmp_x1_chpl = *(call_tmp_chpl14 + INT64(1));
#line 362 "../lib/Chai.chpl"
  *(block_chpl + INT64(0)) = *(ranges_chpl + INT64(0));
#line 362 "../lib/Chai.chpl"
  *(block_chpl + INT64(1)) = *(ranges_chpl + INT64(1));
#line 362 "../lib/Chai.chpl"
  call_tmp_chpl15 = this_chpl26(&block_chpl, parDim_chpl, INT64(362), INT32(65));
#line 362 "../lib/Chai.chpl"
  (call_tmp_chpl15)->_low = tmp_x0_chpl;
#line 362 "../lib/Chai.chpl"
  call_tmp_chpl16 = this_chpl26(&block_chpl, parDim_chpl, INT64(362), INT32(65));
#line 362 "../lib/Chai.chpl"
  (call_tmp_chpl16)->_high = tmp_x1_chpl;
#line 362 "../lib/Chai.chpl"
  *(retcopy_chpl + INT64(0)) = *(block_chpl + INT64(0));
#line 362 "../lib/Chai.chpl"
  *(retcopy_chpl + INT64(1)) = *(block_chpl + INT64(1));
#line 362 "../lib/Chai.chpl"
  *(coerce_tmp_chpl4 + INT64(0)) = *(retcopy_chpl + INT64(0));
#line 362 "../lib/Chai.chpl"
  *(coerce_tmp_chpl4 + INT64(1)) = *(retcopy_chpl + INT64(1));
#line 362 "../lib/Chai.chpl"
  coerce_tmp_chpl5 = *(coerce_tmp_chpl4 + INT64(0));
#line 362 "../lib/Chai.chpl"
  _ic__F0_this_chpl = coerce_tmp_chpl5;
#line 362 "../lib/Chai.chpl"
  checkIfIterWillOverflow_chpl(coerce_tmp_chpl5, UINT8(true), INT64(362), INT32(65));
#line 362 "../lib/Chai.chpl"
  i_chpl = INT64(0);
#line 362 "../lib/Chai.chpl"
  this_chpl33 = _ic__F0_this_chpl;
#line 362 "../lib/Chai.chpl"
  this_chpl34 = _ic__F0_this_chpl;
#line 362 "../lib/Chai.chpl"
  end_chpl = (&this_chpl34)->_high;
#line 362 "../lib/Chai.chpl"
  for (i_chpl = (&this_chpl33)->_low; ((i_chpl <= end_chpl)); i_chpl += INT64(1)) {
#line 362 "../lib/Chai.chpl"
    i_chpl2 = i_chpl;
#line 362 "../lib/Chai.chpl"
    coerce_tmp_chpl6 = *(coerce_tmp_chpl4 + INT64(1));
#line 362 "../lib/Chai.chpl"
    _ic__F0_this_chpl2 = coerce_tmp_chpl6;
#line 362 "../lib/Chai.chpl"
    checkIfIterWillOverflow_chpl(coerce_tmp_chpl6, UINT8(true), INT64(362), INT32(65));
#line 362 "../lib/Chai.chpl"
    i_chpl3 = INT64(0);
#line 362 "../lib/Chai.chpl"
    this_chpl35 = _ic__F0_this_chpl2;
#line 362 "../lib/Chai.chpl"
    this_chpl36 = _ic__F0_this_chpl2;
#line 362 "../lib/Chai.chpl"
    end_chpl2 = (&this_chpl36)->_high;
#line 362 "../lib/Chai.chpl"
    for (i_chpl3 = (&this_chpl35)->_low; ((i_chpl3 <= end_chpl2)); i_chpl3 += INT64(1)) {
#line 362 "../lib/Chai.chpl"
      *(ret_tmp_chpl2 + INT64(0)) = i_chpl2;
#line 362 "../lib/Chai.chpl"
      *(ret_tmp_chpl2 + INT64(1)) = i_chpl3;
#line 362 "../lib/Chai.chpl"
      *(i_chpl4 + INT64(0)) = *(ret_tmp_chpl2 + INT64(0));
#line 362 "../lib/Chai.chpl"
      *(i_chpl4 + INT64(1)) = *(ret_tmp_chpl2 + INT64(1));
#line 362 "../lib/Chai.chpl"
      chpl_check_nil(this_chpl32, INT64(362), INT32(65));
#line 362 "../lib/Chai.chpl"
      call_tmp_chpl17 = &((this_chpl32)->indexCache);
#line 362 "../lib/Chai.chpl"
      chpl_check_nil(this_chpl32, INT64(362), INT32(65));
#line 362 "../lib/Chai.chpl"
      call_tmp_chpl18 = &((this_chpl32)->indexCache);
#line 362 "../lib/Chai.chpl"
      retcopy_x1_chpl = *(i_chpl4 + INT64(1));
#line 362 "../lib/Chai.chpl"
      sum_chpl = INT64(0);
#line 362 "../lib/Chai.chpl"
      call_tmp_chpl19 = &((call_tmp_chpl18)->blk);
#line 362 "../lib/Chai.chpl"
      i_lhs_chpl = &sum_chpl;
#line 362 "../lib/Chai.chpl"
      *(i_lhs_chpl) += ((int64_t)((*(i_chpl4 + INT64(0)) * *(*(call_tmp_chpl19) + INT64(0)))));
#line 362 "../lib/Chai.chpl"
      call_tmp_chpl20 = &((call_tmp_chpl18)->blk);
#line 362 "../lib/Chai.chpl"
      i_lhs_chpl2 = &sum_chpl;
#line 362 "../lib/Chai.chpl"
      *(i_lhs_chpl2) += ((int64_t)((retcopy_x1_chpl * *(*(call_tmp_chpl20) + INT64(1)))));
#line 362 "../lib/Chai.chpl"
      ret_chpl = sum_chpl;
#line 362 "../lib/Chai.chpl"
      coerce_tmp_chpl7 = (call_tmp_chpl17)->shiftedData;
#line 362 "../lib/Chai.chpl"
      call_tmp_chpl21 = (coerce_tmp_chpl7 + ret_chpl);
#line 362 "../lib/Chai.chpl"
      coerce_tmp_chpl8 = *(call_tmp_chpl21);
#line 362 "../lib/Chai.chpl"
      chpl_check_nil(RP_chpl_redSVar_chpl2, INT64(362), INT32(65));
#line 362 "../lib/Chai.chpl"
      accumulateOntoState_chpl2(RP_chpl_redSVar_chpl2, &AS_chpl_redSVar_chpl, coerce_tmp_chpl8, INT64(362), INT32(65));
#line 362 "../lib/Chai.chpl"
    }
#line 362 "../lib/Chai.chpl"
  }
#line 362 "../lib/Chai.chpl"
  coerce_tmp_chpl9 = RP_chpl_redSVar_chpl2;
#line 362 "../lib/Chai.chpl"
  chpl_check_nil(coerce_tmp_chpl9, INT64(362), INT32(65));
#line 362 "../lib/Chai.chpl"
  accumulate_chpl2(coerce_tmp_chpl9, AS_chpl_redSVar_chpl, INT64(362), INT32(65));
#line 362 "../lib/Chai.chpl"
  chpl__reduceCombine7(RP_chpl_redSVar_chpl, RP_chpl_redSVar_chpl2, INT64(362), INT32(65));
#line 362 "../lib/Chai.chpl"
  chpl__delete(RP_chpl_redSVar_chpl2, INT64(362), INT32(65));
#line 362 "../lib/Chai.chpl"
  return;
#line 362 "../lib/Chai.chpl"
}

#line 369 "../lib/Chai.chpl"
/* Chai.chpl:369 */
#line 369 "../lib/Chai.chpl"
static void argmax_chpl(MaxPool_chpl * this_chpl31,
#line 369 "../lib/Chai.chpl"
                        _array_ArrayViewRankChangeArr__real64_int64_t_ArrayViewRankChangeDom_2_int64_t_one__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_DefaultRectangularArr_3_int64_t_one__real64_int64_t__tuple_3_star_chpl_bool__tuple_3_star_int64_t__remoteAccessData__real64_2_int64_t_one_T_T_chpl * m_chpl,
#line 369 "../lib/Chai.chpl"
                        _tuple_2_star_int64_t_chpl * _retArg_chpl) {
#line 370 "../lib/Chai.chpl"
  _domain_ArrayViewRankChangeDom_2_int64_t_one__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_3_star_chpl_bool__tuple_3_star_int64_t_chpl call_tmp_chpl6;
#line 370 "../lib/Chai.chpl"
  _domain_ArrayViewRankChangeDom_2_int64_t_one__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_3_star_chpl_bool__tuple_3_star_int64_t_chpl ret_tmp_chpl;
#line 370 "../lib/Chai.chpl"
  _tuple_2_star_int64_t_chpl call_tmp_chpl7;
#line 370 "../lib/Chai.chpl"
  _tuple_2_star_int64_t_chpl ret_tmp_chpl2;
#line 370 "../lib/Chai.chpl"
  _ref__tuple_2_star_int64_t_chpl i_i_chpl = NULL;
#line 370 "../lib/Chai.chpl"
  ArrayViewRankChangeArr__real64_int64_t_ArrayViewRankChangeDom_2_int64_t_one__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_DefaultRectangularArr_3_int64_t_one__real64_int64_t__tuple_3_star_chpl_bool__tuple_3_star_int64_t__remoteAccessData__real64_2_int64_t_one_T_T_chpl ret_chpl = NULL;
#line 370 "../lib/Chai.chpl"
  ArrayViewRankChangeArr__real64_int64_t_ArrayViewRankChangeDom_2_int64_t_one__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_DefaultRectangularArr_3_int64_t_one__real64_int64_t__tuple_3_star_chpl_bool__tuple_3_star_int64_t__remoteAccessData__real64_2_int64_t_one_T_T_chpl coerce_tmp_chpl3 = NULL;
#line 370 "../lib/Chai.chpl"
  _ref__remoteAccessData__real64_2_int64_t_one_T_chpl call_tmp_chpl8 = NULL;
#line 370 "../lib/Chai.chpl"
  int64_t ret_chpl2;
#line 370 "../lib/Chai.chpl"
  int64_t sum_chpl;
#line 370 "../lib/Chai.chpl"
  _ref__tuple_2_star_int64_t_chpl call_tmp_chpl9 = NULL;
#line 370 "../lib/Chai.chpl"
  _ref_int64_t_chpl i_lhs_chpl = NULL;
#line 370 "../lib/Chai.chpl"
  _ref__tuple_2_star_int64_t_chpl call_tmp_chpl10 = NULL;
#line 370 "../lib/Chai.chpl"
  _ref_int64_t_chpl i_lhs_chpl2 = NULL;
#line 370 "../lib/Chai.chpl"
  _ref__remoteAccessData__real64_2_int64_t_one_T_chpl call_tmp_chpl11 = NULL;
#line 370 "../lib/Chai.chpl"
  _ddata__real64_chpl coerce_tmp_chpl4 = NULL;
#line 370 "../lib/Chai.chpl"
  _ref__real64_chpl call_tmp_chpl12 = NULL;
#line 370 "../lib/Chai.chpl"
  _real64 max_chpl;
#line 371 "../lib/Chai.chpl"
  _domain_ArrayViewRankChangeDom_2_int64_t_one__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_3_star_chpl_bool__tuple_3_star_int64_t_chpl call_tmp_chpl13;
#line 371 "../lib/Chai.chpl"
  _domain_ArrayViewRankChangeDom_2_int64_t_one__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_3_star_chpl_bool__tuple_3_star_int64_t_chpl ret_tmp_chpl3;
#line 371 "../lib/Chai.chpl"
  _tuple_2_star_int64_t_chpl call_tmp_chpl14;
#line 371 "../lib/Chai.chpl"
  _tuple_2_star_int64_t_chpl ret_tmp_chpl4;
#line 371 "../lib/Chai.chpl"
  _tuple_2_star_int64_t_chpl maxIndex_chpl;
#line 372 "../lib/Chai.chpl"
  _domain_ArrayViewRankChangeDom_2_int64_t_one__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_3_star_chpl_bool__tuple_3_star_int64_t_chpl call_tmp_chpl15;
#line 372 "../lib/Chai.chpl"
  _domain_ArrayViewRankChangeDom_2_int64_t_one__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_3_star_chpl_bool__tuple_3_star_int64_t_chpl ret_tmp_chpl5;
#line 372 "../lib/Chai.chpl"
  _domain_ArrayViewRankChangeDom_2_int64_t_one__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_3_star_chpl_bool__tuple_3_star_int64_t_chpl this_chpl32;
#line 372 "../lib/Chai.chpl"
  ArrayViewRankChangeDom_2_int64_t_one__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_3_star_chpl_bool__tuple_3_star_int64_t_chpl ret_chpl3 = NULL;
#line 372 "../lib/Chai.chpl"
  ArrayViewRankChangeDom_2_int64_t_one__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_3_star_chpl_bool__tuple_3_star_int64_t_chpl this_chpl33 = NULL;
#line 372 "../lib/Chai.chpl"
  ArrayViewRankChangeDom_2_int64_t_one__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_3_star_chpl_bool__tuple_3_star_int64_t_chpl this_chpl34 = NULL;
#line 372 "../lib/Chai.chpl"
  DefaultRectangularDom_2_int64_t_one_chpl coerce_tmp_chpl5 = NULL;
#line 372 "../lib/Chai.chpl"
  _tuple_2_star_int64_t_chpl ret_tmp_chpl6;
#line 372 "../lib/Chai.chpl"
  DefaultRectangularDom_2_int64_t_one_chpl _ic__F0_this_chpl = NULL;
#line 372 "../lib/Chai.chpl"
  DefaultRectangularDom_2_int64_t_one_chpl this_chpl35 = NULL;
#line 372 "../lib/Chai.chpl"
  _ref__tuple_2_star_range_int64_t_both_one_chpl call_tmp_chpl16 = NULL;
#line 372 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl6;
#line 372 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl _ic__F0_this_chpl2;
#line 372 "../lib/Chai.chpl"
  int64_t i_chpl;
#line 372 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl this_chpl36;
#line 372 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl this_chpl37;
#line 372 "../lib/Chai.chpl"
  int64_t end_chpl;
#line 372 "../lib/Chai.chpl"
  DefaultRectangularDom_2_int64_t_one_chpl this_chpl38 = NULL;
#line 372 "../lib/Chai.chpl"
  int64_t i_chpl2;
#line 372 "../lib/Chai.chpl"
  _ref__tuple_2_star_range_int64_t_both_one_chpl call_tmp_chpl17 = NULL;
#line 372 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl7;
#line 372 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl _ic__F0_this_chpl3;
#line 372 "../lib/Chai.chpl"
  int64_t i_chpl3;
#line 372 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl this_chpl39;
#line 372 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl this_chpl40;
#line 372 "../lib/Chai.chpl"
  int64_t end_chpl2;
#line 372 "../lib/Chai.chpl"
  _tuple_2_star_int64_t_chpl ret_tmp_chpl7;
#line 372 "../lib/Chai.chpl"
  _tuple_2_star_int64_t_chpl _yieldedIndex_chpl;
#line 372 "../lib/Chai.chpl"
  int64_t _yieldedIndex_x0_chpl;
#line 372 "../lib/Chai.chpl"
  int64_t _yieldedIndex_x1_chpl;
#line 373 "../lib/Chai.chpl"
  _tuple_2_star_int64_t_chpl coerce_tmp_chpl8;
#line 373 "../lib/Chai.chpl"
  _ref__tuple_2_star_int64_t_chpl i_i_chpl2 = NULL;
#line 373 "../lib/Chai.chpl"
  ArrayViewRankChangeArr__real64_int64_t_ArrayViewRankChangeDom_2_int64_t_one__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_DefaultRectangularArr_3_int64_t_one__real64_int64_t__tuple_3_star_chpl_bool__tuple_3_star_int64_t__remoteAccessData__real64_2_int64_t_one_T_T_chpl ret_chpl4 = NULL;
#line 373 "../lib/Chai.chpl"
  ArrayViewRankChangeArr__real64_int64_t_ArrayViewRankChangeDom_2_int64_t_one__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_DefaultRectangularArr_3_int64_t_one__real64_int64_t__tuple_3_star_chpl_bool__tuple_3_star_int64_t__remoteAccessData__real64_2_int64_t_one_T_T_chpl coerce_tmp_chpl9 = NULL;
#line 373 "../lib/Chai.chpl"
  _real64 ret_chpl5;
#line 373 "../lib/Chai.chpl"
  _ref__remoteAccessData__real64_2_int64_t_one_T_chpl call_tmp_chpl18 = NULL;
#line 373 "../lib/Chai.chpl"
  int64_t ret_chpl6;
#line 373 "../lib/Chai.chpl"
  int64_t sum_chpl2;
#line 373 "../lib/Chai.chpl"
  _ref__tuple_2_star_int64_t_chpl call_tmp_chpl19 = NULL;
#line 373 "../lib/Chai.chpl"
  _ref_int64_t_chpl i_lhs_chpl3 = NULL;
#line 373 "../lib/Chai.chpl"
  _ref__tuple_2_star_int64_t_chpl call_tmp_chpl20 = NULL;
#line 373 "../lib/Chai.chpl"
  _ref_int64_t_chpl i_lhs_chpl4 = NULL;
#line 373 "../lib/Chai.chpl"
  _ref__remoteAccessData__real64_2_int64_t_one_T_chpl call_tmp_chpl21 = NULL;
#line 373 "../lib/Chai.chpl"
  _ddata__real64_chpl coerce_tmp_chpl10 = NULL;
#line 373 "../lib/Chai.chpl"
  _ref__real64_chpl call_tmp_chpl22 = NULL;
#line 374 "../lib/Chai.chpl"
  _tuple_2_star_int64_t_chpl coerce_tmp_chpl11;
#line 374 "../lib/Chai.chpl"
  _ref__tuple_2_star_int64_t_chpl i_i_chpl3 = NULL;
#line 374 "../lib/Chai.chpl"
  ArrayViewRankChangeArr__real64_int64_t_ArrayViewRankChangeDom_2_int64_t_one__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_DefaultRectangularArr_3_int64_t_one__real64_int64_t__tuple_3_star_chpl_bool__tuple_3_star_int64_t__remoteAccessData__real64_2_int64_t_one_T_T_chpl ret_chpl7 = NULL;
#line 374 "../lib/Chai.chpl"
  ArrayViewRankChangeArr__real64_int64_t_ArrayViewRankChangeDom_2_int64_t_one__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_DefaultRectangularArr_3_int64_t_one__real64_int64_t__tuple_3_star_chpl_bool__tuple_3_star_int64_t__remoteAccessData__real64_2_int64_t_one_T_T_chpl coerce_tmp_chpl12 = NULL;
#line 374 "../lib/Chai.chpl"
  _ref__remoteAccessData__real64_2_int64_t_one_T_chpl call_tmp_chpl23 = NULL;
#line 374 "../lib/Chai.chpl"
  int64_t ret_chpl8;
#line 374 "../lib/Chai.chpl"
  int64_t sum_chpl3;
#line 374 "../lib/Chai.chpl"
  _ref__tuple_2_star_int64_t_chpl call_tmp_chpl24 = NULL;
#line 374 "../lib/Chai.chpl"
  _ref_int64_t_chpl i_lhs_chpl5 = NULL;
#line 374 "../lib/Chai.chpl"
  _ref__tuple_2_star_int64_t_chpl call_tmp_chpl25 = NULL;
#line 374 "../lib/Chai.chpl"
  _ref_int64_t_chpl i_lhs_chpl6 = NULL;
#line 374 "../lib/Chai.chpl"
  _ref__remoteAccessData__real64_2_int64_t_one_T_chpl call_tmp_chpl26 = NULL;
#line 374 "../lib/Chai.chpl"
  _ddata__real64_chpl coerce_tmp_chpl13 = NULL;
#line 374 "../lib/Chai.chpl"
  _ref__real64_chpl call_tmp_chpl27 = NULL;
#line 375 "../lib/Chai.chpl"
  _tuple_2_star_int64_t_chpl call_tmp_chpl28;
#line 375 "../lib/Chai.chpl"
  _tuple_2_star_int64_t_chpl ret_tmp_chpl8;
#line 375 "../lib/Chai.chpl"
  _ref__tuple_2_star_int64_t_chpl i_x_chpl = NULL;
#line 375 "../lib/Chai.chpl"
  _ref__tuple_2_star_int64_t_chpl i_y_chpl = NULL;
#line 378 "../lib/Chai.chpl"
  _domain_ArrayViewRankChangeDom_2_int64_t_one__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_3_star_chpl_bool__tuple_3_star_int64_t_chpl call_tmp_chpl29;
#line 378 "../lib/Chai.chpl"
  _domain_ArrayViewRankChangeDom_2_int64_t_one__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_3_star_chpl_bool__tuple_3_star_int64_t_chpl ret_tmp_chpl9;
#line 378 "../lib/Chai.chpl"
  _tuple_2_star_int64_t_chpl ret_tmp_chpl10;
#line 378 "../lib/Chai.chpl"
  _tuple_2_star_int64_t_chpl ret_chpl9;
#line 378 "../lib/Chai.chpl"
  int64_t call_tmp_chpl30;
#line 370 "../lib/Chai.chpl"
  _dom_chpl12(m_chpl, &ret_tmp_chpl, INT64(370), INT32(65));
#line 370 "../lib/Chai.chpl"
  call_tmp_chpl6 = ret_tmp_chpl;
#line 370 "../lib/Chai.chpl"
  first_chpl(&call_tmp_chpl6, &ret_tmp_chpl2, INT64(370), INT32(65));
#line 370 "../lib/Chai.chpl"
  *(call_tmp_chpl7 + INT64(0)) = *(ret_tmp_chpl2 + INT64(0));
#line 370 "../lib/Chai.chpl"
  *(call_tmp_chpl7 + INT64(1)) = *(ret_tmp_chpl2 + INT64(1));
#line 370 "../lib/Chai.chpl"
  i_i_chpl = &call_tmp_chpl7;
#line 370 "../lib/Chai.chpl"
  ret_chpl = (m_chpl)->_instance;
#line 370 "../lib/Chai.chpl"
  checkAccess_chpl14(m_chpl, i_i_chpl, ret_chpl, INT64(370), INT32(65));
#line 370 "../lib/Chai.chpl"
  coerce_tmp_chpl3 = ret_chpl;
#line 370 "../lib/Chai.chpl"
  chpl_check_nil(coerce_tmp_chpl3, INT64(370), INT32(65));
#line 370 "../lib/Chai.chpl"
  call_tmp_chpl8 = &((coerce_tmp_chpl3)->indexCache);
#line 370 "../lib/Chai.chpl"
  sum_chpl = INT64(0);
#line 370 "../lib/Chai.chpl"
  call_tmp_chpl9 = &((call_tmp_chpl8)->blk);
#line 370 "../lib/Chai.chpl"
  i_lhs_chpl = &sum_chpl;
#line 370 "../lib/Chai.chpl"
  *(i_lhs_chpl) += ((int64_t)((*(call_tmp_chpl7 + INT64(0)) * *(*(call_tmp_chpl9) + INT64(0)))));
#line 370 "../lib/Chai.chpl"
  call_tmp_chpl10 = &((call_tmp_chpl8)->blk);
#line 370 "../lib/Chai.chpl"
  i_lhs_chpl2 = &sum_chpl;
#line 370 "../lib/Chai.chpl"
  *(i_lhs_chpl2) += ((int64_t)((*(call_tmp_chpl7 + INT64(1)) * *(*(call_tmp_chpl10) + INT64(1)))));
#line 370 "../lib/Chai.chpl"
  ret_chpl2 = sum_chpl;
#line 370 "../lib/Chai.chpl"
  chpl_check_nil(coerce_tmp_chpl3, INT64(370), INT32(65));
#line 370 "../lib/Chai.chpl"
  call_tmp_chpl11 = &((coerce_tmp_chpl3)->indexCache);
#line 370 "../lib/Chai.chpl"
  coerce_tmp_chpl4 = (call_tmp_chpl11)->shiftedData;
#line 370 "../lib/Chai.chpl"
  call_tmp_chpl12 = (coerce_tmp_chpl4 + ret_chpl2);
#line 370 "../lib/Chai.chpl"
  max_chpl = *(call_tmp_chpl12);
#line 370 "../lib/Chai.chpl"
  chpl__autoDestroy12(&call_tmp_chpl6, INT64(370), INT32(65));
#line 371 "../lib/Chai.chpl"
  _dom_chpl12(m_chpl, &ret_tmp_chpl3, INT64(371), INT32(65));
#line 371 "../lib/Chai.chpl"
  call_tmp_chpl13 = ret_tmp_chpl3;
#line 371 "../lib/Chai.chpl"
  first_chpl(&call_tmp_chpl13, &ret_tmp_chpl4, INT64(371), INT32(65));
#line 371 "../lib/Chai.chpl"
  *(call_tmp_chpl14 + INT64(0)) = *(ret_tmp_chpl4 + INT64(0));
#line 371 "../lib/Chai.chpl"
  *(call_tmp_chpl14 + INT64(1)) = *(ret_tmp_chpl4 + INT64(1));
#line 371 "../lib/Chai.chpl"
  *(maxIndex_chpl + INT64(0)) = *(call_tmp_chpl14 + INT64(0));
#line 371 "../lib/Chai.chpl"
  *(maxIndex_chpl + INT64(1)) = *(call_tmp_chpl14 + INT64(1));
#line 371 "../lib/Chai.chpl"
  chpl__autoDestroy12(&call_tmp_chpl13, INT64(371), INT32(65));
#line 372 "../lib/Chai.chpl"
  _dom_chpl12(m_chpl, &ret_tmp_chpl5, INT64(372), INT32(65));
#line 372 "../lib/Chai.chpl"
  call_tmp_chpl15 = ret_tmp_chpl5;
#line 372 "../lib/Chai.chpl"
  this_chpl32 = call_tmp_chpl15;
#line 372 "../lib/Chai.chpl"
  ret_chpl3 = (&this_chpl32)->_instance;
#line 372 "../lib/Chai.chpl"
  this_chpl33 = ret_chpl3;
#line 372 "../lib/Chai.chpl"
  chpl_check_nil(this_chpl33, INT64(372), INT32(65));
#line 372 "../lib/Chai.chpl"
  if (((RootClass_chpl)((this_chpl33)->downDomInst)) == nil) /* ZLINE: 372 ../lib/Chai.chpl */
#line 372 "../lib/Chai.chpl"
  {
#line 372 "../lib/Chai.chpl"
    nilCheckHalt_chpl(&_str_literal_52_chpl, INT64(372), INT32(65));
#line 372 "../lib/Chai.chpl"
  }
#line 372 "../lib/Chai.chpl"
  chpl_check_nil(this_chpl33, INT64(372), INT32(65));
#line 372 "../lib/Chai.chpl"
  if (((RootClass_chpl)((this_chpl33)->downDomInst)) == nil) /* ZLINE: 372 ../lib/Chai.chpl */
#line 372 "../lib/Chai.chpl"
  {
#line 372 "../lib/Chai.chpl"
    nilCheckHalt_chpl(&_str_literal_52_chpl, INT64(372), INT32(65));
#line 372 "../lib/Chai.chpl"
  }
#line 372 "../lib/Chai.chpl"
  chpl_check_nil(this_chpl33, INT64(372), INT32(65));
#line 372 "../lib/Chai.chpl"
  if (((RootClass_chpl)((this_chpl33)->downDomInst)) == nil) /* ZLINE: 372 ../lib/Chai.chpl */
#line 372 "../lib/Chai.chpl"
  {
#line 372 "../lib/Chai.chpl"
    nilCheckHalt_chpl(&_str_literal_52_chpl, INT64(372), INT32(65));
#line 372 "../lib/Chai.chpl"
  }
#line 372 "../lib/Chai.chpl"
  this_chpl34 = ret_chpl3;
#line 372 "../lib/Chai.chpl"
  chpl_check_nil(this_chpl34, INT64(372), INT32(65));
#line 372 "../lib/Chai.chpl"
  coerce_tmp_chpl5 = (this_chpl34)->upDomInst;
#line 372 "../lib/Chai.chpl"
  if (((RootClass_chpl)(coerce_tmp_chpl5)) == nil) /* ZLINE: 372 ../lib/Chai.chpl */
#line 372 "../lib/Chai.chpl"
  {
#line 372 "../lib/Chai.chpl"
    nilCheckHalt_chpl(&_str_literal_52_chpl, INT64(372), INT32(65));
#line 372 "../lib/Chai.chpl"
  }
#line 372 "../lib/Chai.chpl"
  chpl_check_nil(this_chpl34, INT64(372), INT32(65));
#line 372 "../lib/Chai.chpl"
  if (((RootClass_chpl)((this_chpl34)->upDomInst)) == nil) /* ZLINE: 372 ../lib/Chai.chpl */
#line 372 "../lib/Chai.chpl"
  {
#line 372 "../lib/Chai.chpl"
    nilCheckHalt_chpl(&_str_literal_52_chpl, INT64(372), INT32(65));
#line 372 "../lib/Chai.chpl"
  }
#line 372 "../lib/Chai.chpl"
  createTuple_chpl4(INT64(0), &ret_tmp_chpl6);
#line 372 "../lib/Chai.chpl"
  _ic__F0_this_chpl = coerce_tmp_chpl5;
#line 372 "../lib/Chai.chpl"
  this_chpl35 = coerce_tmp_chpl5;
#line 372 "../lib/Chai.chpl"
  chpl_check_nil(this_chpl35, INT64(372), INT32(65));
#line 372 "../lib/Chai.chpl"
  call_tmp_chpl16 = &((this_chpl35)->ranges);
#line 372 "../lib/Chai.chpl"
  coerce_tmp_chpl6 = *(*(call_tmp_chpl16) + INT64(0));
#line 372 "../lib/Chai.chpl"
  _ic__F0_this_chpl2 = coerce_tmp_chpl6;
#line 372 "../lib/Chai.chpl"
  checkIfIterWillOverflow_chpl(coerce_tmp_chpl6, UINT8(true), INT64(372), INT32(65));
#line 372 "../lib/Chai.chpl"
  i_chpl = INT64(0);
#line 372 "../lib/Chai.chpl"
  this_chpl36 = _ic__F0_this_chpl2;
#line 372 "../lib/Chai.chpl"
  this_chpl37 = _ic__F0_this_chpl2;
#line 372 "../lib/Chai.chpl"
  end_chpl = (&this_chpl37)->_high;
#line 372 "../lib/Chai.chpl"
  this_chpl38 = _ic__F0_this_chpl;
#line 372 "../lib/Chai.chpl"
  for (i_chpl = (&this_chpl36)->_low; ((i_chpl <= end_chpl)); i_chpl += INT64(1)) {
#line 372 "../lib/Chai.chpl"
    i_chpl2 = i_chpl;
#line 372 "../lib/Chai.chpl"
    chpl_check_nil(this_chpl38, INT64(372), INT32(65));
#line 372 "../lib/Chai.chpl"
    call_tmp_chpl17 = &((this_chpl38)->ranges);
#line 372 "../lib/Chai.chpl"
    coerce_tmp_chpl7 = *(*(call_tmp_chpl17) + INT64(1));
#line 372 "../lib/Chai.chpl"
    _ic__F0_this_chpl3 = coerce_tmp_chpl7;
#line 372 "../lib/Chai.chpl"
    checkIfIterWillOverflow_chpl(coerce_tmp_chpl7, UINT8(true), INT64(372), INT32(65));
#line 372 "../lib/Chai.chpl"
    i_chpl3 = INT64(0);
#line 372 "../lib/Chai.chpl"
    this_chpl39 = _ic__F0_this_chpl3;
#line 372 "../lib/Chai.chpl"
    this_chpl40 = _ic__F0_this_chpl3;
#line 372 "../lib/Chai.chpl"
    end_chpl2 = (&this_chpl40)->_high;
#line 372 "../lib/Chai.chpl"
    for (i_chpl3 = (&this_chpl39)->_low; ((i_chpl3 <= end_chpl2)); i_chpl3 += INT64(1)) {
#line 372 "../lib/Chai.chpl"
      *(ret_tmp_chpl7 + INT64(0)) = i_chpl2;
#line 372 "../lib/Chai.chpl"
      *(ret_tmp_chpl7 + INT64(1)) = i_chpl3;
#line 372 "../lib/Chai.chpl"
      *(_yieldedIndex_chpl + INT64(0)) = *(ret_tmp_chpl7 + INT64(0));
#line 372 "../lib/Chai.chpl"
      *(_yieldedIndex_chpl + INT64(1)) = *(ret_tmp_chpl7 + INT64(1));
#line 372 "../lib/Chai.chpl"
      _yieldedIndex_x0_chpl = *(_yieldedIndex_chpl + INT64(0));
#line 372 "../lib/Chai.chpl"
      _yieldedIndex_x1_chpl = *(_yieldedIndex_chpl + INT64(1));
#line 373 "../lib/Chai.chpl"
      *(coerce_tmp_chpl8 + INT64(0)) = _yieldedIndex_x0_chpl;
#line 373 "../lib/Chai.chpl"
      *(coerce_tmp_chpl8 + INT64(1)) = _yieldedIndex_x1_chpl;
#line 373 "../lib/Chai.chpl"
      i_i_chpl2 = &coerce_tmp_chpl8;
#line 373 "../lib/Chai.chpl"
      ret_chpl4 = (m_chpl)->_instance;
#line 373 "../lib/Chai.chpl"
      checkAccess_chpl14(m_chpl, i_i_chpl2, ret_chpl4, INT64(373), INT32(65));
#line 373 "../lib/Chai.chpl"
      coerce_tmp_chpl9 = ret_chpl4;
#line 373 "../lib/Chai.chpl"
      chpl_check_nil(coerce_tmp_chpl9, INT64(373), INT32(65));
#line 373 "../lib/Chai.chpl"
      call_tmp_chpl18 = &((coerce_tmp_chpl9)->indexCache);
#line 373 "../lib/Chai.chpl"
      sum_chpl2 = INT64(0);
#line 373 "../lib/Chai.chpl"
      call_tmp_chpl19 = &((call_tmp_chpl18)->blk);
#line 373 "../lib/Chai.chpl"
      i_lhs_chpl3 = &sum_chpl2;
#line 373 "../lib/Chai.chpl"
      *(i_lhs_chpl3) += ((int64_t)((*(coerce_tmp_chpl8 + INT64(0)) * *(*(call_tmp_chpl19) + INT64(0)))));
#line 373 "../lib/Chai.chpl"
      call_tmp_chpl20 = &((call_tmp_chpl18)->blk);
#line 373 "../lib/Chai.chpl"
      i_lhs_chpl4 = &sum_chpl2;
#line 373 "../lib/Chai.chpl"
      *(i_lhs_chpl4) += ((int64_t)((*(coerce_tmp_chpl8 + INT64(1)) * *(*(call_tmp_chpl20) + INT64(1)))));
#line 373 "../lib/Chai.chpl"
      ret_chpl6 = sum_chpl2;
#line 373 "../lib/Chai.chpl"
      chpl_check_nil(coerce_tmp_chpl9, INT64(373), INT32(65));
#line 373 "../lib/Chai.chpl"
      call_tmp_chpl21 = &((coerce_tmp_chpl9)->indexCache);
#line 373 "../lib/Chai.chpl"
      coerce_tmp_chpl10 = (call_tmp_chpl21)->shiftedData;
#line 373 "../lib/Chai.chpl"
      call_tmp_chpl22 = (coerce_tmp_chpl10 + ret_chpl6);
#line 373 "../lib/Chai.chpl"
      ret_chpl5 = *(call_tmp_chpl22);
#line 373 "../lib/Chai.chpl"
      if (ret_chpl5 > max_chpl) /* ZLINE: 373 ../lib/Chai.chpl */
#line 373 "../lib/Chai.chpl"
      {
#line 374 "../lib/Chai.chpl"
        *(coerce_tmp_chpl11 + INT64(0)) = _yieldedIndex_x0_chpl;
#line 374 "../lib/Chai.chpl"
        *(coerce_tmp_chpl11 + INT64(1)) = _yieldedIndex_x1_chpl;
#line 374 "../lib/Chai.chpl"
        i_i_chpl3 = &coerce_tmp_chpl11;
#line 374 "../lib/Chai.chpl"
        ret_chpl7 = (m_chpl)->_instance;
#line 374 "../lib/Chai.chpl"
        checkAccess_chpl14(m_chpl, i_i_chpl3, ret_chpl7, INT64(374), INT32(65));
#line 374 "../lib/Chai.chpl"
        coerce_tmp_chpl12 = ret_chpl7;
#line 374 "../lib/Chai.chpl"
        chpl_check_nil(coerce_tmp_chpl12, INT64(374), INT32(65));
#line 374 "../lib/Chai.chpl"
        call_tmp_chpl23 = &((coerce_tmp_chpl12)->indexCache);
#line 374 "../lib/Chai.chpl"
        sum_chpl3 = INT64(0);
#line 374 "../lib/Chai.chpl"
        call_tmp_chpl24 = &((call_tmp_chpl23)->blk);
#line 374 "../lib/Chai.chpl"
        i_lhs_chpl5 = &sum_chpl3;
#line 374 "../lib/Chai.chpl"
        *(i_lhs_chpl5) += ((int64_t)((*(coerce_tmp_chpl11 + INT64(0)) * *(*(call_tmp_chpl24) + INT64(0)))));
#line 374 "../lib/Chai.chpl"
        call_tmp_chpl25 = &((call_tmp_chpl23)->blk);
#line 374 "../lib/Chai.chpl"
        i_lhs_chpl6 = &sum_chpl3;
#line 374 "../lib/Chai.chpl"
        *(i_lhs_chpl6) += ((int64_t)((*(coerce_tmp_chpl11 + INT64(1)) * *(*(call_tmp_chpl25) + INT64(1)))));
#line 374 "../lib/Chai.chpl"
        ret_chpl8 = sum_chpl3;
#line 374 "../lib/Chai.chpl"
        chpl_check_nil(coerce_tmp_chpl12, INT64(374), INT32(65));
#line 374 "../lib/Chai.chpl"
        call_tmp_chpl26 = &((coerce_tmp_chpl12)->indexCache);
#line 374 "../lib/Chai.chpl"
        coerce_tmp_chpl13 = (call_tmp_chpl26)->shiftedData;
#line 374 "../lib/Chai.chpl"
        call_tmp_chpl27 = (coerce_tmp_chpl13 + ret_chpl8);
#line 374 "../lib/Chai.chpl"
        max_chpl = *(call_tmp_chpl27);
#line 375 "../lib/Chai.chpl"
        *(ret_tmp_chpl8 + INT64(0)) = _yieldedIndex_x0_chpl;
#line 375 "../lib/Chai.chpl"
        *(ret_tmp_chpl8 + INT64(1)) = _yieldedIndex_x1_chpl;
#line 375 "../lib/Chai.chpl"
        *(call_tmp_chpl28 + INT64(0)) = *(ret_tmp_chpl8 + INT64(0));
#line 375 "../lib/Chai.chpl"
        *(call_tmp_chpl28 + INT64(1)) = *(ret_tmp_chpl8 + INT64(1));
#line 375 "../lib/Chai.chpl"
        i_x_chpl = &maxIndex_chpl;
#line 375 "../lib/Chai.chpl"
        i_y_chpl = &call_tmp_chpl28;
#line 375 "../lib/Chai.chpl"
        *(*(i_x_chpl) + INT64(0)) = *(*(i_y_chpl) + INT64(0));
#line 375 "../lib/Chai.chpl"
        *(*(i_x_chpl) + INT64(1)) = *(*(i_y_chpl) + INT64(1));
#line 375 "../lib/Chai.chpl"
      }
#line 375 "../lib/Chai.chpl"
    }
#line 375 "../lib/Chai.chpl"
  }
#line 372 "../lib/Chai.chpl"
  chpl__autoDestroy12(&call_tmp_chpl15, INT64(372), INT32(65));
#line 378 "../lib/Chai.chpl"
  _dom_chpl12(m_chpl, &ret_tmp_chpl9, INT64(378), INT32(65));
#line 378 "../lib/Chai.chpl"
  call_tmp_chpl29 = ret_tmp_chpl9;
#line 378 "../lib/Chai.chpl"
  first_chpl(&call_tmp_chpl29, &ret_tmp_chpl10, INT64(378), INT32(65));
#line 378 "../lib/Chai.chpl"
  call_tmp_chpl30 = (*(maxIndex_chpl + INT64(1)) - *(ret_tmp_chpl10 + INT64(1)));
#line 378 "../lib/Chai.chpl"
  *(ret_chpl9 + INT64(0)) = ((int64_t)((*(maxIndex_chpl + INT64(0)) - *(ret_tmp_chpl10 + INT64(0)))));
#line 378 "../lib/Chai.chpl"
  *(ret_chpl9 + INT64(1)) = call_tmp_chpl30;
#line 378 "../lib/Chai.chpl"
  chpl__autoDestroy12(&call_tmp_chpl29, INT64(378), INT32(65));
#line 369 "../lib/Chai.chpl"
  *(*(_retArg_chpl) + INT64(0)) = *(ret_chpl9 + INT64(0));
#line 369 "../lib/Chai.chpl"
  *(*(_retArg_chpl) + INT64(1)) = *(ret_chpl9 + INT64(1));
#line 369 "../lib/Chai.chpl"
  return;
#line 369 "../lib/Chai.chpl"
}

#line 381 "../lib/Chai.chpl"
/* Chai.chpl:381 */
#line 381 "../lib/Chai.chpl"
static void backward_chpl(MaxPool_chpl * this_chpl31,
#line 381 "../lib/Chai.chpl"
                          Tensor_3__real64_chpl * delta_chpl,
#line 381 "../lib/Chai.chpl"
                          Tensor_3__real64_chpl * convs_chpl,
#line 381 "../lib/Chai.chpl"
                          Tensor_3__real64_chpl * _retArg_chpl) {
#line 388 "../lib/Chai.chpl"
  int64_t local_dataParMinGranularity_chpl;
#line 388 "../lib/Chai.chpl"
  chpl_bool local_dataParIgnoreRunningTasks_chpl;
#line 388 "../lib/Chai.chpl"
  int64_t local_dataParTasksPerLocale_chpl;
#line 382 "../lib/Chai.chpl"
  _tuple_3_star_int64_t_chpl call_tmp_chpl6;
#line 382 "../lib/Chai.chpl"
  _tuple_3_star_int64_t_chpl ret_tmp_chpl;
#line 382 "../lib/Chai.chpl"
  int64_t tmp_x0_chpl;
#line 382 "../lib/Chai.chpl"
  int64_t tmp_x1_chpl;
#line 382 "../lib/Chai.chpl"
  int64_t tmp_x2_chpl;
#line 387 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_3_int64_t_one__real64_int64_t_chpl grad_chpl;
#line 387 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl call_tmp_chpl7;
#line 387 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl ret_tmp_chpl2;
#line 387 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl8;
#line 387 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl3;
#line 387 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl call_tmp_chpl9;
#line 387 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl ret_tmp_chpl4;
#line 387 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl10;
#line 387 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl5;
#line 387 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl call_tmp_chpl11;
#line 387 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl ret_tmp_chpl6;
#line 387 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl12;
#line 387 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl7;
#line 387 "../lib/Chai.chpl"
  _domain_DefaultRectangularDom_3_int64_t_one_chpl call_tmp_chpl13;
#line 387 "../lib/Chai.chpl"
  _domain_DefaultRectangularDom_3_int64_t_one_chpl ret_tmp_chpl8;
#line 387 "../lib/Chai.chpl"
  chpl___RuntimeTypeInfo7 call_tmp_chpl14;
#line 387 "../lib/Chai.chpl"
  _domain_DefaultRectangularDom_3_int64_t_one_chpl _runtime_type_tmp__chpl;
#line 387 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_3_int64_t_one__real64_int64_t_chpl ret_tmp_chpl9;
#line 388 "../lib/Chai.chpl"
  _domain_DefaultRectangularDom_3_int64_t_one_chpl call_tmp_chpl15;
#line 388 "../lib/Chai.chpl"
  _ref__array_DefaultRectangularArr_3_int64_t_one__real64_int64_t_chpl call_tmp_chpl16 = NULL;
#line 388 "../lib/Chai.chpl"
  _domain_DefaultRectangularDom_3_int64_t_one_chpl ret_tmp_chpl10;
#line 388 "../lib/Chai.chpl"
  DefaultRectangularDom_3_int64_t_one_chpl coerce_tmp_chpl3 = NULL;
#line 388 "../lib/Chai.chpl"
  int64_t tasksPerLocale_chpl;
#line 388 "../lib/Chai.chpl"
  chpl_bool ignoreRunning_chpl;
#line 388 "../lib/Chai.chpl"
  int64_t minIndicesPerTask_chpl;
#line 388 "../lib/Chai.chpl"
  _tuple_3_star_int64_t_chpl ret_tmp_chpl11;
#line 388 "../lib/Chai.chpl"
  Error_chpl error_chpl = NULL;
#line 388 "../lib/Chai.chpl"
  int64_t tmp_chpl;
#line 388 "../lib/Chai.chpl"
  locale_chpl call_tmp_chpl17;
#line 388 "../lib/Chai.chpl"
  chpl_localeID_t call_tmp_chpl18;
#line 388 "../lib/Chai.chpl"
  int32_t call_tmp_chpl19;
#line 388 "../lib/Chai.chpl"
  chpl_localeID_t call_tmp_chpl20;
#line 388 "../lib/Chai.chpl"
  locale_chpl call_tmp_chpl21;
#line 388 "../lib/Chai.chpl"
  locale_chpl ret_tmp_chpl12;
#line 388 "../lib/Chai.chpl"
  int64_t ret_chpl;
#line 388 "../lib/Chai.chpl"
  BaseLocale_chpl coerce_tmp_chpl4 = NULL;
#line 388 "../lib/Chai.chpl"
  BaseLocale_chpl coerce_tmp_chpl5 = NULL;
#line 388 "../lib/Chai.chpl"
  _ref_locale_chpl i_x_chpl = NULL;
#line 388 "../lib/Chai.chpl"
  int64_t tmp_x0_chpl2;
#line 388 "../lib/Chai.chpl"
  int64_t tmp_x1_chpl2;
#line 388 "../lib/Chai.chpl"
  _ref__tuple_3_star_range_int64_t_both_one_chpl call_tmp_chpl22 = NULL;
#line 388 "../lib/Chai.chpl"
  _tuple_2_star_int64_t_chpl ret_tmp_chpl13;
#line 388 "../lib/Chai.chpl"
  int64_t numChunks_chpl;
#line 388 "../lib/Chai.chpl"
  int64_t parDim_chpl;
#line 388 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl tmpIter_chpl;
#line 388 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl call_tmp_chpl23;
#line 388 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl ret_tmp_chpl14;
#line 388 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl15;
#line 388 "../lib/Chai.chpl"
  chpl___EndCount_AtomicT_int64_t_int64_t _coforallCount_chpl = NULL;
#line 388 "../lib/Chai.chpl"
  chpl___EndCount_AtomicT_int64_t_int64_t new_temp_chpl = NULL;
#line 388 "../lib/Chai.chpl"
  int64_t numTasks_chpl;
#line 388 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl _ic__F0_this_chpl;
#line 388 "../lib/Chai.chpl"
  int64_t i_chpl;
#line 388 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl this_chpl32;
#line 388 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl this_chpl33;
#line 388 "../lib/Chai.chpl"
  int64_t end_chpl;
#line 388 "../lib/Chai.chpl"
  _class_localscoforall_fn_chpl108 _args_forcoforall_fn_chpl = NULL;
#line 388 "../lib/Chai.chpl"
  _ref_Tensor_3__real64_chpl tmp_chpl2 = NULL;
#line 388 "../lib/Chai.chpl"
  _ref_MaxPool_chpl tmp_chpl3 = NULL;
#line 388 "../lib/Chai.chpl"
  _ref_Tensor_3__real64_chpl tmp_chpl4 = NULL;
#line 388 "../lib/Chai.chpl"
  raw_c_void_ptr _args_vforcoforall_fn_chpl;
#line 388 "../lib/Chai.chpl"
  Error_chpl error_chpl2 = NULL;
#line 394 "../lib/Chai.chpl"
  Tensor_3__real64_chpl call_tmp_chpl24;
#line 394 "../lib/Chai.chpl"
  Tensor_3__real64_chpl initTemp_chpl;
#line 394 "../lib/Chai.chpl"
  chpl__class_localscoforall_fn_chpl108_object chpl_macro_tmp_113;
#line 388 "../lib/Chai.chpl"
  local_dataParMinGranularity_chpl = dataParMinGranularity_chpl;
#line 388 "../lib/Chai.chpl"
  local_dataParIgnoreRunningTasks_chpl = dataParIgnoreRunningTasks_chpl;
#line 388 "../lib/Chai.chpl"
  local_dataParTasksPerLocale_chpl = dataParTasksPerLocale_chpl;
#line 382 "../lib/Chai.chpl"
  shape_chpl8(convs_chpl, &ret_tmp_chpl);
#line 382 "../lib/Chai.chpl"
  *(call_tmp_chpl6 + INT64(0)) = *(ret_tmp_chpl + INT64(0));
#line 382 "../lib/Chai.chpl"
  *(call_tmp_chpl6 + INT64(1)) = *(ret_tmp_chpl + INT64(1));
#line 382 "../lib/Chai.chpl"
  *(call_tmp_chpl6 + INT64(2)) = *(ret_tmp_chpl + INT64(2));
#line 382 "../lib/Chai.chpl"
  tmp_x0_chpl = *(call_tmp_chpl6 + INT64(0));
#line 382 "../lib/Chai.chpl"
  tmp_x1_chpl = *(call_tmp_chpl6 + INT64(1));
#line 382 "../lib/Chai.chpl"
  tmp_x2_chpl = *(call_tmp_chpl6 + INT64(2));
#line 387 "../lib/Chai.chpl"
  chpl_build_low_bounded_range2(INT64(0), &ret_tmp_chpl2);
#line 387 "../lib/Chai.chpl"
  call_tmp_chpl7 = ret_tmp_chpl2;
#line 387 "../lib/Chai.chpl"
  chpl___POUND_3(call_tmp_chpl7, tmp_x0_chpl, &ret_tmp_chpl3, INT64(387), INT32(65));
#line 387 "../lib/Chai.chpl"
  call_tmp_chpl8 = ret_tmp_chpl3;
#line 387 "../lib/Chai.chpl"
  chpl_build_low_bounded_range2(INT64(0), &ret_tmp_chpl4);
#line 387 "../lib/Chai.chpl"
  call_tmp_chpl9 = ret_tmp_chpl4;
#line 387 "../lib/Chai.chpl"
  chpl___POUND_3(call_tmp_chpl9, tmp_x1_chpl, &ret_tmp_chpl5, INT64(387), INT32(65));
#line 387 "../lib/Chai.chpl"
  call_tmp_chpl10 = ret_tmp_chpl5;
#line 387 "../lib/Chai.chpl"
  chpl_build_low_bounded_range2(INT64(0), &ret_tmp_chpl6);
#line 387 "../lib/Chai.chpl"
  call_tmp_chpl11 = ret_tmp_chpl6;
#line 387 "../lib/Chai.chpl"
  chpl___POUND_3(call_tmp_chpl11, tmp_x2_chpl, &ret_tmp_chpl7, INT64(387), INT32(65));
#line 387 "../lib/Chai.chpl"
  call_tmp_chpl12 = ret_tmp_chpl7;
#line 387 "../lib/Chai.chpl"
  chpl__ensureDomainExpr6(call_tmp_chpl8, call_tmp_chpl10, call_tmp_chpl12, &ret_tmp_chpl8, INT64(387), INT32(65));
#line 387 "../lib/Chai.chpl"
  call_tmp_chpl13 = ret_tmp_chpl8;
#line 387 "../lib/Chai.chpl"
  call_tmp_chpl14 = chpl__buildArrayRuntimeType3(&call_tmp_chpl13);
#line 387 "../lib/Chai.chpl"
  _runtime_type_tmp__chpl = (&call_tmp_chpl14)->dom;
#line 387 "../lib/Chai.chpl"
  chpl__convertRuntimeTypeToValue6(&_runtime_type_tmp__chpl, UINT8(false), &ret_tmp_chpl9, INT64(387), INT32(65));
#line 387 "../lib/Chai.chpl"
  grad_chpl = ret_tmp_chpl9;
#line 388 "../lib/Chai.chpl"
  call_tmp_chpl16 = &((delta_chpl)->data);
#line 388 "../lib/Chai.chpl"
  _dom_chpl8(call_tmp_chpl16, &ret_tmp_chpl10, INT64(388), INT32(65));
#line 388 "../lib/Chai.chpl"
  call_tmp_chpl15 = ret_tmp_chpl10;
#line 388 "../lib/Chai.chpl"
  coerce_tmp_chpl3 = (&call_tmp_chpl15)->_instance;
#line 388 "../lib/Chai.chpl"
  tasksPerLocale_chpl = local_dataParTasksPerLocale_chpl;
#line 388 "../lib/Chai.chpl"
  ignoreRunning_chpl = local_dataParIgnoreRunningTasks_chpl;
#line 388 "../lib/Chai.chpl"
  minIndicesPerTask_chpl = local_dataParMinGranularity_chpl;
#line 388 "../lib/Chai.chpl"
  createTuple_chpl2(INT64(0), &ret_tmp_chpl11);
#line 388 "../lib/Chai.chpl"
  error_chpl = NULL;
#line 388 "../lib/Chai.chpl"
  if (tasksPerLocale_chpl == INT64(0)) /* ZLINE: 388 ../lib/Chai.chpl */
#line 388 "../lib/Chai.chpl"
  {
#line 388 "../lib/Chai.chpl"
    call_tmp_chpl19 = get_chpl_nodeID();
#line 388 "../lib/Chai.chpl"
    call_tmp_chpl20 = chpl_rt_buildLocaleID(call_tmp_chpl19, c_sublocid_any);
#line 388 "../lib/Chai.chpl"
    call_tmp_chpl18 = call_tmp_chpl20;
#line 388 "../lib/Chai.chpl"
    chpl_localeID_to_locale(&call_tmp_chpl18, &ret_tmp_chpl12, INT64(388), INT32(65));
#line 388 "../lib/Chai.chpl"
    call_tmp_chpl21 = ret_tmp_chpl12;
#line 388 "../lib/Chai.chpl"
    call_tmp_chpl17 = call_tmp_chpl21;
#line 388 "../lib/Chai.chpl"
    coerce_tmp_chpl4 = (&call_tmp_chpl17)->_instance;
#line 388 "../lib/Chai.chpl"
    if (((RootClass_chpl)(coerce_tmp_chpl4)) == nil) /* ZLINE: 388 ../lib/Chai.chpl */
#line 388 "../lib/Chai.chpl"
    {
#line 388 "../lib/Chai.chpl"
      nilCheckHalt_chpl(&_str_literal_52_chpl, INT64(388), INT32(65));
#line 388 "../lib/Chai.chpl"
    }
#line 388 "../lib/Chai.chpl"
    coerce_tmp_chpl5 = coerce_tmp_chpl4;
#line 388 "../lib/Chai.chpl"
    chpl_check_nil(coerce_tmp_chpl5, INT64(388), INT32(65));
#line 388 "../lib/Chai.chpl"
    ret_chpl = (coerce_tmp_chpl5)->maxTaskPar;
#line 388 "../lib/Chai.chpl"
    tmp_chpl = ret_chpl;
#line 388 "../lib/Chai.chpl"
    i_x_chpl = &call_tmp_chpl17;
#line 388 "../lib/Chai.chpl"
    deinit_chpl62(i_x_chpl);
#line 388 "../lib/Chai.chpl"
  } else /* ZLINE: 388 ../lib/Chai.chpl */
#line 388 "../lib/Chai.chpl"
  {
#line 388 "../lib/Chai.chpl"
    tmp_chpl = tasksPerLocale_chpl;
#line 388 "../lib/Chai.chpl"
  }
#line 388 "../lib/Chai.chpl"
  if (chpl_task_getSerial()) /* ZLINE: 388 ../lib/Chai.chpl */
#line 388 "../lib/Chai.chpl"
  {
#line 388 "../lib/Chai.chpl"
    tmp_x0_chpl2 = INT64(1);
#line 388 "../lib/Chai.chpl"
    tmp_x1_chpl2 = INT64(0);
#line 388 "../lib/Chai.chpl"
  } else /* ZLINE: 388 ../lib/Chai.chpl */
#line 388 "../lib/Chai.chpl"
  {
#line 388 "../lib/Chai.chpl"
    chpl_check_nil(coerce_tmp_chpl3, INT64(388), INT32(65));
#line 388 "../lib/Chai.chpl"
    call_tmp_chpl22 = &((coerce_tmp_chpl3)->ranges);
#line 388 "../lib/Chai.chpl"
    _computeChunkStuff_chpl3(tmp_chpl, ignoreRunning_chpl, minIndicesPerTask_chpl, call_tmp_chpl22, &ret_tmp_chpl13, INT64(388), INT32(65));
#line 388 "../lib/Chai.chpl"
    tmp_x0_chpl2 = *(ret_tmp_chpl13 + INT64(0));
#line 388 "../lib/Chai.chpl"
    tmp_x1_chpl2 = *(ret_tmp_chpl13 + INT64(1));
#line 388 "../lib/Chai.chpl"
  }
#line 388 "../lib/Chai.chpl"
  numChunks_chpl = tmp_x0_chpl2;
#line 388 "../lib/Chai.chpl"
  parDim_chpl = tmp_x1_chpl2;
#line 388 "../lib/Chai.chpl"
  chpl_build_low_bounded_range2(INT64(0), &ret_tmp_chpl14);
#line 388 "../lib/Chai.chpl"
  call_tmp_chpl23 = ret_tmp_chpl14;
#line 388 "../lib/Chai.chpl"
  chpl___POUND_3(call_tmp_chpl23, tmp_x0_chpl2, &ret_tmp_chpl15, INT64(388), INT32(65));
#line 388 "../lib/Chai.chpl"
  tmpIter_chpl = ret_tmp_chpl15;
#line 388 "../lib/Chai.chpl"
  new_temp_chpl = _new_chpl17(INT64(388), INT32(65));
#line 388 "../lib/Chai.chpl"
  _coforallCount_chpl = new_temp_chpl;
#line 388 "../lib/Chai.chpl"
  numTasks_chpl = chpl_boundedCoforallSize(tmpIter_chpl, INT64(388), INT32(65));
#line 388 "../lib/Chai.chpl"
  _upEndCount_chpl(_coforallCount_chpl, numTasks_chpl, INT64(388), INT32(65));
#line 388 "../lib/Chai.chpl"
  chpl_resetTaskSpawn(numTasks_chpl, INT64(388), INT32(65));
#line 388 "../lib/Chai.chpl"
  _ic__F0_this_chpl = tmpIter_chpl;
#line 388 "../lib/Chai.chpl"
  checkIfIterWillOverflow_chpl(tmpIter_chpl, UINT8(true), INT64(388), INT32(65));
#line 388 "../lib/Chai.chpl"
  i_chpl = INT64(0);
#line 388 "../lib/Chai.chpl"
  this_chpl32 = _ic__F0_this_chpl;
#line 388 "../lib/Chai.chpl"
  this_chpl33 = _ic__F0_this_chpl;
#line 388 "../lib/Chai.chpl"
  end_chpl = (&this_chpl33)->_high;
#line 388 "../lib/Chai.chpl"
  for (i_chpl = (&this_chpl32)->_low; ((i_chpl <= end_chpl)); i_chpl += INT64(1)) {
#line 388 "../lib/Chai.chpl"
    _args_forcoforall_fn_chpl = ((_class_localscoforall_fn_chpl108)(&chpl_macro_tmp_113));
#line 388 "../lib/Chai.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl, INT64(388), INT32(65));
#line 388 "../lib/Chai.chpl"
    (_args_forcoforall_fn_chpl)->_0_numChunks = numChunks_chpl;
#line 388 "../lib/Chai.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl, INT64(388), INT32(65));
#line 388 "../lib/Chai.chpl"
    (_args_forcoforall_fn_chpl)->_1_parDim = parDim_chpl;
#line 388 "../lib/Chai.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl, INT64(388), INT32(65));
#line 388 "../lib/Chai.chpl"
    (_args_forcoforall_fn_chpl)->_2_coerce_tmp = coerce_tmp_chpl3;
#line 388 "../lib/Chai.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl, INT64(388), INT32(65));
#line 388 "../lib/Chai.chpl"
    (_args_forcoforall_fn_chpl)->_3__coforallCount = _coforallCount_chpl;
#line 388 "../lib/Chai.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl, INT64(388), INT32(65));
#line 388 "../lib/Chai.chpl"
    (_args_forcoforall_fn_chpl)->_4_chunk = i_chpl;
#line 388 "../lib/Chai.chpl"
    tmp_chpl2 = convs_chpl;
#line 388 "../lib/Chai.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl, INT64(388), INT32(65));
#line 388 "../lib/Chai.chpl"
    (_args_forcoforall_fn_chpl)->_6_convs = tmp_chpl2;
#line 388 "../lib/Chai.chpl"
    tmp_chpl3 = this_chpl31;
#line 388 "../lib/Chai.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl, INT64(388), INT32(65));
#line 388 "../lib/Chai.chpl"
    (_args_forcoforall_fn_chpl)->_7_this = tmp_chpl3;
#line 388 "../lib/Chai.chpl"
    tmp_chpl4 = delta_chpl;
#line 388 "../lib/Chai.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl, INT64(388), INT32(65));
#line 388 "../lib/Chai.chpl"
    (_args_forcoforall_fn_chpl)->_8_delta = tmp_chpl4;
#line 388 "../lib/Chai.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl, INT64(388), INT32(65));
#line 388 "../lib/Chai.chpl"
    (_args_forcoforall_fn_chpl)->_9_grad = grad_chpl;
#line 388 "../lib/Chai.chpl"
    _args_vforcoforall_fn_chpl = ((void*)(_args_forcoforall_fn_chpl));
#line 388 "../lib/Chai.chpl"
    /*** wrapcoforall_fn_chpl108 ***/ chpl_taskAddCoStmt(chpl_task_getRequestedSubloc(), INT64(107), ((chpl_task_bundle_p)(_args_vforcoforall_fn_chpl)), sizeof(chpl__class_localscoforall_fn_chpl108_object), INT64(388), INT32(65));
#line 388 "../lib/Chai.chpl"
  }
#line 388 "../lib/Chai.chpl"
  error_chpl2 = NULL;
#line 388 "../lib/Chai.chpl"
  _waitEndCount_chpl2(_coforallCount_chpl, numTasks_chpl, &error_chpl2, INT64(388), INT32(65));
#line 388 "../lib/Chai.chpl"
  if (error_chpl2 != nil) /* ZLINE: 388 ../lib/Chai.chpl */
#line 388 "../lib/Chai.chpl"
  {
#line 388 "../lib/Chai.chpl"
    chpl__delete72(_coforallCount_chpl, INT64(388), INT32(65));
#line 388 "../lib/Chai.chpl"
    error_chpl = error_chpl2;
#line 388 "../lib/Chai.chpl"
    goto forall_IBB_break_label_chpl;
#line 388 "../lib/Chai.chpl"
  }
#line 388 "../lib/Chai.chpl"
  chpl__delete72(_coforallCount_chpl, INT64(388), INT32(65));
#line 388 "../lib/Chai.chpl"
  chpl_after_forall_fence();
#line 388 "../lib/Chai.chpl"
  forall_IBB_break_label_chpl:;
#line 388 "../lib/Chai.chpl"
  if (error_chpl != nil) /* ZLINE: 388 ../lib/Chai.chpl */
#line 388 "../lib/Chai.chpl"
  {
#line 388 "../lib/Chai.chpl"
    chpl_propagate_error(error_chpl, INT64(388), INT32(65));
#line 388 "../lib/Chai.chpl"
  }
#line 388 "../lib/Chai.chpl"
  chpl_after_forall_fence();
#line 388 "../lib/Chai.chpl"
  chpl__autoDestroy8(&call_tmp_chpl15, INT64(388), INT32(65));
#line 394 "../lib/Chai.chpl"
  init_chpl245(&initTemp_chpl, &grad_chpl);
#line 394 "../lib/Chai.chpl"
  call_tmp_chpl24 = initTemp_chpl;
#line 396 "../lib/Chai.chpl"
  chpl__autoDestroy28(&grad_chpl, INT64(396), INT32(65));
#line 396 "../lib/Chai.chpl"
  chpl__autoDestroy8(&call_tmp_chpl13, INT64(396), INT32(65));
#line 381 "../lib/Chai.chpl"
  *(_retArg_chpl) = call_tmp_chpl24;
#line 381 "../lib/Chai.chpl"
  return;
#line 381 "../lib/Chai.chpl"
}

#line 388 "../lib/Chai.chpl"
/* Chai.chpl:388 */
#line 388 "../lib/Chai.chpl"
static void wrapcoforall_fn_chpl108(_class_localscoforall_fn_chpl108 c_chpl) {
#line 388 "../lib/Chai.chpl"
  Error_chpl error_chpl = NULL;
#line 388 "../lib/Chai.chpl"
  int64_t _0_numChunks_chpl;
#line 388 "../lib/Chai.chpl"
  int64_t _1_parDim_chpl;
#line 388 "../lib/Chai.chpl"
  DefaultRectangularDom_3_int64_t_one_chpl _2_coerce_tmp_chpl = NULL;
#line 388 "../lib/Chai.chpl"
  chpl___EndCount_AtomicT_int64_t_int64_t _3__coforallCount_chpl = NULL;
#line 388 "../lib/Chai.chpl"
  int64_t _4_chunk_chpl;
#line 388 "../lib/Chai.chpl"
  _ref_Tensor_3__real64_chpl _6_convs_chpl = NULL;
#line 388 "../lib/Chai.chpl"
  _ref_MaxPool_chpl _7_this_chpl = NULL;
#line 388 "../lib/Chai.chpl"
  _ref_Tensor_3__real64_chpl _8_delta_chpl = NULL;
#line 388 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_3_int64_t_one__real64_int64_t_chpl _9_grad_chpl;
#line 388 "../lib/Chai.chpl"
  chpl_rmem_consist_acquire(INT64(388), INT32(65));
#line 388 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(388), INT32(65));
#line 388 "../lib/Chai.chpl"
  _0_numChunks_chpl = (c_chpl)->_0_numChunks;
#line 388 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(388), INT32(65));
#line 388 "../lib/Chai.chpl"
  _1_parDim_chpl = (c_chpl)->_1_parDim;
#line 388 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(388), INT32(65));
#line 388 "../lib/Chai.chpl"
  _2_coerce_tmp_chpl = (c_chpl)->_2_coerce_tmp;
#line 388 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(388), INT32(65));
#line 388 "../lib/Chai.chpl"
  _3__coforallCount_chpl = (c_chpl)->_3__coforallCount;
#line 388 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(388), INT32(65));
#line 388 "../lib/Chai.chpl"
  _4_chunk_chpl = (c_chpl)->_4_chunk;
#line 388 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(388), INT32(65));
#line 388 "../lib/Chai.chpl"
  _6_convs_chpl = (c_chpl)->_6_convs;
#line 388 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(388), INT32(65));
#line 388 "../lib/Chai.chpl"
  _7_this_chpl = (c_chpl)->_7_this;
#line 388 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(388), INT32(65));
#line 388 "../lib/Chai.chpl"
  _8_delta_chpl = (c_chpl)->_8_delta;
#line 388 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(388), INT32(65));
#line 388 "../lib/Chai.chpl"
  _9_grad_chpl = (c_chpl)->_9_grad;
#line 388 "../lib/Chai.chpl"
  coforall_fn_chpl108(_0_numChunks_chpl, _1_parDim_chpl, _2_coerce_tmp_chpl, _3__coforallCount_chpl, _4_chunk_chpl, &error_chpl, _6_convs_chpl, _7_this_chpl, _8_delta_chpl, &_9_grad_chpl);
#line 388 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(388), INT32(65));
#line 388 "../lib/Chai.chpl"
  _downEndCount_chpl((c_chpl)->_3__coforallCount, error_chpl, INT64(388), INT32(65));
#line 388 "../lib/Chai.chpl"
  return;
#line 388 "../lib/Chai.chpl"
}

#line 388 "../lib/Chai.chpl"
/* Chai.chpl:388 */
#line 388 "../lib/Chai.chpl"
static void coforall_fn_chpl108(int64_t numChunks_chpl,
#line 388 "../lib/Chai.chpl"
                                int64_t parDim_chpl,
#line 388 "../lib/Chai.chpl"
                                DefaultRectangularDom_3_int64_t_one_chpl this_chpl31,
#line 388 "../lib/Chai.chpl"
                                chpl___EndCount_AtomicT_int64_t_int64_t _coforallCount_chpl,
#line 388 "../lib/Chai.chpl"
                                int64_t chunk_chpl,
#line 388 "../lib/Chai.chpl"
                                Error_chpl * error_out_chpl,
#line 388 "../lib/Chai.chpl"
                                Tensor_3__real64_chpl * convs_chpl,
#line 388 "../lib/Chai.chpl"
                                MaxPool_chpl * this_chpl32,
#line 388 "../lib/Chai.chpl"
                                Tensor_3__real64_chpl * delta_chpl,
#line 388 "../lib/Chai.chpl"
                                _array_DefaultRectangularArr_3_int64_t_one__real64_int64_t_chpl * grad_chpl) {
#line 388 "../lib/Chai.chpl"
  _tuple_3_star_range_int64_t_both_one_chpl ranges_chpl;
#line 388 "../lib/Chai.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl6 = NULL;
#line 388 "../lib/Chai.chpl"
  uint64_t call_tmp_chpl7;
#line 388 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl3;
#line 388 "../lib/Chai.chpl"
  uint64_t call_tmp_chpl8;
#line 388 "../lib/Chai.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl9 = NULL;
#line 388 "../lib/Chai.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl10 = NULL;
#line 388 "../lib/Chai.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl11 = NULL;
#line 388 "../lib/Chai.chpl"
  _tuple_2_star_int64_t_chpl call_tmp_chpl12;
#line 388 "../lib/Chai.chpl"
  _tuple_2_star_int64_t_chpl ret_tmp_chpl;
#line 388 "../lib/Chai.chpl"
  int64_t tmp_x0_chpl;
#line 388 "../lib/Chai.chpl"
  int64_t tmp_x1_chpl;
#line 388 "../lib/Chai.chpl"
  _tuple_3_star_range_int64_t_both_one_chpl block_chpl;
#line 388 "../lib/Chai.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl13 = NULL;
#line 388 "../lib/Chai.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl14 = NULL;
#line 388 "../lib/Chai.chpl"
  _tuple_3_star_range_int64_t_both_one_chpl coerce_tmp_chpl4;
#line 388 "../lib/Chai.chpl"
  _tuple_3_star_range_int64_t_both_one_chpl retcopy_chpl;
#line 388 "../lib/Chai.chpl"
  DefaultRectangularDom_3_int64_t_one_chpl _ic__F0_this_chpl = NULL;
#line 388 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl5;
#line 388 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl _ic__F0_this_chpl2;
#line 388 "../lib/Chai.chpl"
  int64_t i_chpl;
#line 388 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl this_chpl33;
#line 388 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl this_chpl34;
#line 388 "../lib/Chai.chpl"
  int64_t end_chpl;
#line 388 "../lib/Chai.chpl"
  int64_t i_chpl2;
#line 388 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl6;
#line 388 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl _ic__F0_this_chpl3;
#line 388 "../lib/Chai.chpl"
  int64_t i_chpl3;
#line 388 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl this_chpl35;
#line 388 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl this_chpl36;
#line 388 "../lib/Chai.chpl"
  int64_t end_chpl2;
#line 388 "../lib/Chai.chpl"
  int64_t i_chpl4;
#line 388 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl7;
#line 388 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl _ic__F0_this_chpl4;
#line 388 "../lib/Chai.chpl"
  int64_t i_chpl5;
#line 388 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl this_chpl37;
#line 388 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl this_chpl38;
#line 388 "../lib/Chai.chpl"
  int64_t end_chpl3;
#line 388 "../lib/Chai.chpl"
  _tuple_2_star_int64_t_chpl ret_tmp_chpl2;
#line 388 "../lib/Chai.chpl"
  _tuple_2_star_int64_t_chpl _yieldedIndex_chpl;
#line 388 "../lib/Chai.chpl"
  int64_t j_x0_chpl;
#line 388 "../lib/Chai.chpl"
  int64_t j_x1_chpl;
#line 388 "../lib/Chai.chpl"
  _tuple_3_star_int64_t_chpl ret_tmp_chpl3;
#line 388 "../lib/Chai.chpl"
  _tuple_3_star_int64_t_chpl yret_chpl;
#line 388 "../lib/Chai.chpl"
  int64_t chpl_idx_1_x0;
#line 388 "../lib/Chai.chpl"
  int64_t chpl_idx_1_x1;
#line 388 "../lib/Chai.chpl"
  int64_t chpl_idx_1_x2;
#line 389 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl call_tmp_chpl15;
#line 389 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl ret_tmp_chpl4;
#line 389 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl16;
#line 389 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl5;
#line 389 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl call_tmp_chpl17;
#line 389 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl ret_tmp_chpl6;
#line 389 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl18;
#line 389 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl7;
#line 389 "../lib/Chai.chpl"
  _array_ArrayViewRankChangeArr__real64_int64_t_ArrayViewRankChangeDom_2_int64_t_one__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_DefaultRectangularArr_3_int64_t_one__real64_int64_t__tuple_3_star_chpl_bool__tuple_3_star_int64_t__remoteAccessData__real64_2_int64_t_one_T_F_chpl call_tmp_chpl19;
#line 98 "../lib/Tensor.chpl"
  _ref__array_DefaultRectangularArr_3_int64_t_one__real64_int64_t_chpl call_tmp_chpl20 = NULL;
#line 389 "../lib/Chai.chpl"
  _array_ArrayViewRankChangeArr__real64_int64_t_ArrayViewRankChangeDom_2_int64_t_one__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_DefaultRectangularArr_3_int64_t_one__real64_int64_t__tuple_3_star_chpl_bool__tuple_3_star_int64_t__remoteAccessData__real64_2_int64_t_one_T_F_chpl ret_tmp_chpl8;
#line 389 "../lib/Chai.chpl"
  _array_ArrayViewRankChangeArr__real64_int64_t_ArrayViewRankChangeDom_2_int64_t_one__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_DefaultRectangularArr_3_int64_t_one__real64_int64_t__tuple_3_star_chpl_bool__tuple_3_star_int64_t__remoteAccessData__real64_2_int64_t_one_T_T_chpl region_chpl;
#line 390 "../lib/Chai.chpl"
  _tuple_2_star_int64_t_chpl call_tmp_chpl21;
#line 390 "../lib/Chai.chpl"
  _tuple_2_star_int64_t_chpl ret_tmp_chpl9;
#line 391 "../lib/Chai.chpl"
  int64_t call_tmp_chpl22;
#line 391 "../lib/Chai.chpl"
  _tuple_3_star_int64_t_chpl coerce_tmp_chpl8;
#line 391 "../lib/Chai.chpl"
  _ref__tuple_3_star_int64_t_chpl i_i_chpl = NULL;
#line 391 "../lib/Chai.chpl"
  DefaultRectangularArr_3_int64_t_one__real64_int64_t_chpl ret_chpl = NULL;
#line 391 "../lib/Chai.chpl"
  DefaultRectangularArr_3_int64_t_one__real64_int64_t_chpl coerce_tmp_chpl9 = NULL;
#line 391 "../lib/Chai.chpl"
  _tuple_3_star_int64_t_chpl _formal_tmp_in_ind_chpl;
#line 391 "../lib/Chai.chpl"
  int64_t _formal_tmp_ind_x0_chpl;
#line 391 "../lib/Chai.chpl"
  int64_t _formal_tmp_ind_x1_chpl;
#line 391 "../lib/Chai.chpl"
  int64_t _formal_tmp_ind_x2_chpl;
#line 391 "../lib/Chai.chpl"
  int64_t ret_chpl2;
#line 391 "../lib/Chai.chpl"
  int64_t sum_chpl;
#line 391 "../lib/Chai.chpl"
  _ref__tuple_3_star_int64_t_chpl call_tmp_chpl23 = NULL;
#line 391 "../lib/Chai.chpl"
  _ref_int64_t_chpl i_lhs_chpl = NULL;
#line 391 "../lib/Chai.chpl"
  _ref__tuple_3_star_int64_t_chpl call_tmp_chpl24 = NULL;
#line 391 "../lib/Chai.chpl"
  _ref_int64_t_chpl i_lhs_chpl2 = NULL;
#line 391 "../lib/Chai.chpl"
  _ref_int64_t_chpl i_lhs_chpl3 = NULL;
#line 391 "../lib/Chai.chpl"
  _ddata__real64_chpl coerce_tmp_chpl10 = NULL;
#line 391 "../lib/Chai.chpl"
  _ref__real64_chpl call_tmp_chpl25 = NULL;
#line 391 "../lib/Chai.chpl"
  _ref__array_DefaultRectangularArr_3_int64_t_one__real64_int64_t_chpl call_tmp_chpl26 = NULL;
#line 391 "../lib/Chai.chpl"
  _tuple_3_star_int64_t_chpl coerce_tmp_chpl11;
#line 391 "../lib/Chai.chpl"
  _ref__tuple_3_star_int64_t_chpl i_i_chpl2 = NULL;
#line 391 "../lib/Chai.chpl"
  DefaultRectangularArr_3_int64_t_one__real64_int64_t_chpl ret_chpl3 = NULL;
#line 391 "../lib/Chai.chpl"
  DefaultRectangularArr_3_int64_t_one__real64_int64_t_chpl coerce_tmp_chpl12 = NULL;
#line 391 "../lib/Chai.chpl"
  _tuple_3_star_int64_t_chpl _formal_tmp_in_ind_chpl2;
#line 391 "../lib/Chai.chpl"
  int64_t _formal_tmp_ind_x0_chpl2;
#line 391 "../lib/Chai.chpl"
  int64_t _formal_tmp_ind_x1_chpl2;
#line 391 "../lib/Chai.chpl"
  int64_t _formal_tmp_ind_x2_chpl2;
#line 391 "../lib/Chai.chpl"
  int64_t ret_chpl4;
#line 391 "../lib/Chai.chpl"
  int64_t sum_chpl2;
#line 391 "../lib/Chai.chpl"
  _ref__tuple_3_star_int64_t_chpl call_tmp_chpl27 = NULL;
#line 391 "../lib/Chai.chpl"
  _ref_int64_t_chpl i_lhs_chpl4 = NULL;
#line 391 "../lib/Chai.chpl"
  _ref__tuple_3_star_int64_t_chpl call_tmp_chpl28 = NULL;
#line 391 "../lib/Chai.chpl"
  _ref_int64_t_chpl i_lhs_chpl5 = NULL;
#line 391 "../lib/Chai.chpl"
  _ref_int64_t_chpl i_lhs_chpl6 = NULL;
#line 391 "../lib/Chai.chpl"
  _ddata__real64_chpl coerce_tmp_chpl13 = NULL;
#line 391 "../lib/Chai.chpl"
  _ref__real64_chpl call_tmp_chpl29 = NULL;
#line 388 "../lib/Chai.chpl"
  chpl_check_nil(this_chpl31, INT64(388), INT32(65));
#line 388 "../lib/Chai.chpl"
  *(ranges_chpl + INT64(0)) = *((this_chpl31)->ranges + INT64(0));
#line 388 "../lib/Chai.chpl"
  *(ranges_chpl + INT64(1)) = *((this_chpl31)->ranges + INT64(1));
#line 388 "../lib/Chai.chpl"
  *(ranges_chpl + INT64(2)) = *((this_chpl31)->ranges + INT64(2));
#line 388 "../lib/Chai.chpl"
  call_tmp_chpl6 = this_chpl24(&ranges_chpl, parDim_chpl, INT64(388), INT32(65));
#line 388 "../lib/Chai.chpl"
  coerce_tmp_chpl3 = *(call_tmp_chpl6);
#line 388 "../lib/Chai.chpl"
  call_tmp_chpl7 = sizeAs_chpl36(coerce_tmp_chpl3, INT64(388), INT32(65));
#line 388 "../lib/Chai.chpl"
  this_chpl24(&ranges_chpl, parDim_chpl, INT64(388), INT32(65));
#line 388 "../lib/Chai.chpl"
  call_tmp_chpl8 = (call_tmp_chpl7 * UINT64(1));
#line 388 "../lib/Chai.chpl"
  call_tmp_chpl9 = this_chpl24(&ranges_chpl, parDim_chpl, INT64(388), INT32(65));
#line 388 "../lib/Chai.chpl"
  call_tmp_chpl10 = this_chpl24(&ranges_chpl, parDim_chpl, INT64(388), INT32(65));
#line 388 "../lib/Chai.chpl"
  call_tmp_chpl11 = this_chpl24(&ranges_chpl, parDim_chpl, INT64(388), INT32(65));
#line 388 "../lib/Chai.chpl"
  _computeBlock_chpl2(call_tmp_chpl8, numChunks_chpl, chunk_chpl, (call_tmp_chpl9)->_high, (call_tmp_chpl10)->_low, (call_tmp_chpl11)->_low, &ret_tmp_chpl, INT64(388), INT32(65));
#line 388 "../lib/Chai.chpl"
  *(call_tmp_chpl12 + INT64(0)) = *(ret_tmp_chpl + INT64(0));
#line 388 "../lib/Chai.chpl"
  *(call_tmp_chpl12 + INT64(1)) = *(ret_tmp_chpl + INT64(1));
#line 388 "../lib/Chai.chpl"
  tmp_x0_chpl = *(call_tmp_chpl12 + INT64(0));
#line 388 "../lib/Chai.chpl"
  tmp_x1_chpl = *(call_tmp_chpl12 + INT64(1));
#line 388 "../lib/Chai.chpl"
  *(block_chpl + INT64(0)) = *(ranges_chpl + INT64(0));
#line 388 "../lib/Chai.chpl"
  *(block_chpl + INT64(1)) = *(ranges_chpl + INT64(1));
#line 388 "../lib/Chai.chpl"
  *(block_chpl + INT64(2)) = *(ranges_chpl + INT64(2));
#line 388 "../lib/Chai.chpl"
  call_tmp_chpl13 = this_chpl24(&block_chpl, parDim_chpl, INT64(388), INT32(65));
#line 388 "../lib/Chai.chpl"
  (call_tmp_chpl13)->_low = tmp_x0_chpl;
#line 388 "../lib/Chai.chpl"
  call_tmp_chpl14 = this_chpl24(&block_chpl, parDim_chpl, INT64(388), INT32(65));
#line 388 "../lib/Chai.chpl"
  (call_tmp_chpl14)->_high = tmp_x1_chpl;
#line 388 "../lib/Chai.chpl"
  *(retcopy_chpl + INT64(0)) = *(block_chpl + INT64(0));
#line 388 "../lib/Chai.chpl"
  *(retcopy_chpl + INT64(1)) = *(block_chpl + INT64(1));
#line 388 "../lib/Chai.chpl"
  *(retcopy_chpl + INT64(2)) = *(block_chpl + INT64(2));
#line 388 "../lib/Chai.chpl"
  *(coerce_tmp_chpl4 + INT64(0)) = *(retcopy_chpl + INT64(0));
#line 388 "../lib/Chai.chpl"
  *(coerce_tmp_chpl4 + INT64(1)) = *(retcopy_chpl + INT64(1));
#line 388 "../lib/Chai.chpl"
  *(coerce_tmp_chpl4 + INT64(2)) = *(retcopy_chpl + INT64(2));
#line 388 "../lib/Chai.chpl"
  _ic__F0_this_chpl = this_chpl31;
#line 388 "../lib/Chai.chpl"
  coerce_tmp_chpl5 = *(coerce_tmp_chpl4 + INT64(0));
#line 388 "../lib/Chai.chpl"
  _ic__F0_this_chpl2 = coerce_tmp_chpl5;
#line 388 "../lib/Chai.chpl"
  checkIfIterWillOverflow_chpl(coerce_tmp_chpl5, UINT8(true), INT64(388), INT32(65));
#line 388 "../lib/Chai.chpl"
  i_chpl = INT64(0);
#line 388 "../lib/Chai.chpl"
  this_chpl33 = _ic__F0_this_chpl2;
#line 388 "../lib/Chai.chpl"
  this_chpl34 = _ic__F0_this_chpl2;
#line 388 "../lib/Chai.chpl"
  end_chpl = (&this_chpl34)->_high;
#line 388 "../lib/Chai.chpl"
  for (i_chpl = (&this_chpl33)->_low; ((i_chpl <= end_chpl)); i_chpl += INT64(1)) {
#line 388 "../lib/Chai.chpl"
    i_chpl2 = i_chpl;
#line 388 "../lib/Chai.chpl"
    coerce_tmp_chpl6 = *(coerce_tmp_chpl4 + INT64(1));
#line 388 "../lib/Chai.chpl"
    _ic__F0_this_chpl3 = coerce_tmp_chpl6;
#line 388 "../lib/Chai.chpl"
    checkIfIterWillOverflow_chpl(coerce_tmp_chpl6, UINT8(true), INT64(388), INT32(65));
#line 388 "../lib/Chai.chpl"
    i_chpl3 = INT64(0);
#line 388 "../lib/Chai.chpl"
    this_chpl35 = _ic__F0_this_chpl3;
#line 388 "../lib/Chai.chpl"
    this_chpl36 = _ic__F0_this_chpl3;
#line 388 "../lib/Chai.chpl"
    end_chpl2 = (&this_chpl36)->_high;
#line 388 "../lib/Chai.chpl"
    for (i_chpl3 = (&this_chpl35)->_low; ((i_chpl3 <= end_chpl2)); i_chpl3 += INT64(1)) {
#line 388 "../lib/Chai.chpl"
      i_chpl4 = i_chpl3;
#line 388 "../lib/Chai.chpl"
      coerce_tmp_chpl7 = *(coerce_tmp_chpl4 + INT64(2));
#line 388 "../lib/Chai.chpl"
      _ic__F0_this_chpl4 = coerce_tmp_chpl7;
#line 388 "../lib/Chai.chpl"
      checkIfIterWillOverflow_chpl(coerce_tmp_chpl7, UINT8(true), INT64(388), INT32(65));
#line 388 "../lib/Chai.chpl"
      i_chpl5 = INT64(0);
#line 388 "../lib/Chai.chpl"
      this_chpl37 = _ic__F0_this_chpl4;
#line 388 "../lib/Chai.chpl"
      this_chpl38 = _ic__F0_this_chpl4;
#line 388 "../lib/Chai.chpl"
      end_chpl3 = (&this_chpl38)->_high;
#line 388 "../lib/Chai.chpl"
      for (i_chpl5 = (&this_chpl37)->_low; ((i_chpl5 <= end_chpl3)); i_chpl5 += INT64(1)) {
#line 388 "../lib/Chai.chpl"
        *(ret_tmp_chpl2 + INT64(0)) = i_chpl4;
#line 388 "../lib/Chai.chpl"
        *(ret_tmp_chpl2 + INT64(1)) = i_chpl5;
#line 388 "../lib/Chai.chpl"
        *(_yieldedIndex_chpl + INT64(0)) = *(ret_tmp_chpl2 + INT64(0));
#line 388 "../lib/Chai.chpl"
        *(_yieldedIndex_chpl + INT64(1)) = *(ret_tmp_chpl2 + INT64(1));
#line 388 "../lib/Chai.chpl"
        j_x0_chpl = *(_yieldedIndex_chpl + INT64(0));
#line 388 "../lib/Chai.chpl"
        j_x1_chpl = *(_yieldedIndex_chpl + INT64(1));
#line 388 "../lib/Chai.chpl"
        *(ret_tmp_chpl3 + INT64(0)) = i_chpl2;
#line 388 "../lib/Chai.chpl"
        *(ret_tmp_chpl3 + INT64(1)) = j_x0_chpl;
#line 388 "../lib/Chai.chpl"
        *(ret_tmp_chpl3 + INT64(2)) = j_x1_chpl;
#line 388 "../lib/Chai.chpl"
        *(yret_chpl + INT64(0)) = *(ret_tmp_chpl3 + INT64(0));
#line 388 "../lib/Chai.chpl"
        *(yret_chpl + INT64(1)) = *(ret_tmp_chpl3 + INT64(1));
#line 388 "../lib/Chai.chpl"
        *(yret_chpl + INT64(2)) = *(ret_tmp_chpl3 + INT64(2));
#line 388 "../lib/Chai.chpl"
        chpl_idx_1_x0 = *(yret_chpl + INT64(0));
#line 388 "../lib/Chai.chpl"
        chpl_idx_1_x1 = *(yret_chpl + INT64(1));
#line 388 "../lib/Chai.chpl"
        chpl_idx_1_x2 = *(yret_chpl + INT64(2));
#line 389 "../lib/Chai.chpl"
        chpl_build_low_bounded_range2(((int64_t)((chpl_idx_1_x0 * INT64(2)))), &ret_tmp_chpl4);
#line 389 "../lib/Chai.chpl"
        call_tmp_chpl15 = ret_tmp_chpl4;
#line 389 "../lib/Chai.chpl"
        chpl___POUND_3(call_tmp_chpl15, INT64(2), &ret_tmp_chpl5, INT64(389), INT32(65));
#line 389 "../lib/Chai.chpl"
        call_tmp_chpl16 = ret_tmp_chpl5;
#line 389 "../lib/Chai.chpl"
        chpl_build_low_bounded_range2(((int64_t)((chpl_idx_1_x1 * INT64(2)))), &ret_tmp_chpl6);
#line 389 "../lib/Chai.chpl"
        call_tmp_chpl17 = ret_tmp_chpl6;
#line 389 "../lib/Chai.chpl"
        chpl___POUND_3(call_tmp_chpl17, INT64(2), &ret_tmp_chpl7, INT64(389), INT32(65));
#line 389 "../lib/Chai.chpl"
        call_tmp_chpl18 = ret_tmp_chpl7;
#line 98 "../lib/Tensor.chpl"
        call_tmp_chpl20 = &((convs_chpl)->data);
#line 389 "../lib/Chai.chpl"
        this_chpl5(call_tmp_chpl20, call_tmp_chpl16, call_tmp_chpl18, chpl_idx_1_x2, &ret_tmp_chpl8, INT64(389), INT32(65));
#line 389 "../lib/Chai.chpl"
        call_tmp_chpl19 = ret_tmp_chpl8;
#line 389 "../lib/Chai.chpl"
        region_chpl = chpl__initCopy8(&call_tmp_chpl19, UINT8(true), INT64(389), INT32(65));
#line 389 "../lib/Chai.chpl"
        chpl__autoDestroy15(&call_tmp_chpl19, INT64(389), INT32(65));
#line 390 "../lib/Chai.chpl"
        argmax_chpl(this_chpl32, &region_chpl, &ret_tmp_chpl9);
#line 390 "../lib/Chai.chpl"
        *(call_tmp_chpl21 + INT64(0)) = *(ret_tmp_chpl9 + INT64(0));
#line 390 "../lib/Chai.chpl"
        *(call_tmp_chpl21 + INT64(1)) = *(ret_tmp_chpl9 + INT64(1));
#line 391 "../lib/Chai.chpl"
        call_tmp_chpl22 = (((int64_t)((chpl_idx_1_x1 * INT64(2)))) + *(call_tmp_chpl21 + INT64(1)));
#line 391 "../lib/Chai.chpl"
        *(coerce_tmp_chpl8 + INT64(0)) = ((int64_t)((((int64_t)((chpl_idx_1_x0 * INT64(2)))) + *(call_tmp_chpl21 + INT64(0)))));
#line 391 "../lib/Chai.chpl"
        *(coerce_tmp_chpl8 + INT64(1)) = call_tmp_chpl22;
#line 391 "../lib/Chai.chpl"
        *(coerce_tmp_chpl8 + INT64(2)) = chpl_idx_1_x2;
#line 391 "../lib/Chai.chpl"
        i_i_chpl = &coerce_tmp_chpl8;
#line 391 "../lib/Chai.chpl"
        ret_chpl = (grad_chpl)->_instance;
#line 391 "../lib/Chai.chpl"
        checkAccess_chpl9(grad_chpl, i_i_chpl, ret_chpl, INT64(391), INT32(65));
#line 391 "../lib/Chai.chpl"
        coerce_tmp_chpl9 = ret_chpl;
#line 391 "../lib/Chai.chpl"
        *(_formal_tmp_in_ind_chpl + INT64(0)) = *(coerce_tmp_chpl8 + INT64(0));
#line 391 "../lib/Chai.chpl"
        *(_formal_tmp_in_ind_chpl + INT64(1)) = *(coerce_tmp_chpl8 + INT64(1));
#line 391 "../lib/Chai.chpl"
        *(_formal_tmp_in_ind_chpl + INT64(2)) = *(coerce_tmp_chpl8 + INT64(2));
#line 391 "../lib/Chai.chpl"
        _formal_tmp_ind_x0_chpl = *(_formal_tmp_in_ind_chpl + INT64(0));
#line 391 "../lib/Chai.chpl"
        _formal_tmp_ind_x1_chpl = *(_formal_tmp_in_ind_chpl + INT64(1));
#line 391 "../lib/Chai.chpl"
        _formal_tmp_ind_x2_chpl = *(_formal_tmp_in_ind_chpl + INT64(2));
#line 391 "../lib/Chai.chpl"
        sum_chpl = INT64(0);
#line 391 "../lib/Chai.chpl"
        chpl_check_nil(coerce_tmp_chpl9, INT64(391), INT32(65));
#line 391 "../lib/Chai.chpl"
        call_tmp_chpl23 = &((coerce_tmp_chpl9)->blk);
#line 391 "../lib/Chai.chpl"
        i_lhs_chpl = &sum_chpl;
#line 391 "../lib/Chai.chpl"
        *(i_lhs_chpl) += ((int64_t)((_formal_tmp_ind_x0_chpl * *(*(call_tmp_chpl23) + INT64(0)))));
#line 391 "../lib/Chai.chpl"
        chpl_check_nil(coerce_tmp_chpl9, INT64(391), INT32(65));
#line 391 "../lib/Chai.chpl"
        call_tmp_chpl24 = &((coerce_tmp_chpl9)->blk);
#line 391 "../lib/Chai.chpl"
        i_lhs_chpl2 = &sum_chpl;
#line 391 "../lib/Chai.chpl"
        *(i_lhs_chpl2) += ((int64_t)((_formal_tmp_ind_x1_chpl * *(*(call_tmp_chpl24) + INT64(1)))));
#line 391 "../lib/Chai.chpl"
        i_lhs_chpl3 = &sum_chpl;
#line 391 "../lib/Chai.chpl"
        *(i_lhs_chpl3) += _formal_tmp_ind_x2_chpl;
#line 391 "../lib/Chai.chpl"
        ret_chpl2 = sum_chpl;
#line 391 "../lib/Chai.chpl"
        chpl_check_nil(coerce_tmp_chpl9, INT64(391), INT32(65));
#line 391 "../lib/Chai.chpl"
        coerce_tmp_chpl10 = (coerce_tmp_chpl9)->shiftedData;
#line 391 "../lib/Chai.chpl"
        call_tmp_chpl25 = (coerce_tmp_chpl10 + ret_chpl2);
#line 391 "../lib/Chai.chpl"
        call_tmp_chpl26 = &((delta_chpl)->data);
#line 391 "../lib/Chai.chpl"
        *(coerce_tmp_chpl11 + INT64(0)) = chpl_idx_1_x0;
#line 391 "../lib/Chai.chpl"
        *(coerce_tmp_chpl11 + INT64(1)) = chpl_idx_1_x1;
#line 391 "../lib/Chai.chpl"
        *(coerce_tmp_chpl11 + INT64(2)) = chpl_idx_1_x2;
#line 391 "../lib/Chai.chpl"
        i_i_chpl2 = &coerce_tmp_chpl11;
#line 391 "../lib/Chai.chpl"
        ret_chpl3 = (call_tmp_chpl26)->_instance;
#line 391 "../lib/Chai.chpl"
        checkAccess_chpl9(call_tmp_chpl26, i_i_chpl2, ret_chpl3, INT64(391), INT32(65));
#line 391 "../lib/Chai.chpl"
        coerce_tmp_chpl12 = ret_chpl3;
#line 391 "../lib/Chai.chpl"
        *(_formal_tmp_in_ind_chpl2 + INT64(0)) = *(coerce_tmp_chpl11 + INT64(0));
#line 391 "../lib/Chai.chpl"
        *(_formal_tmp_in_ind_chpl2 + INT64(1)) = *(coerce_tmp_chpl11 + INT64(1));
#line 391 "../lib/Chai.chpl"
        *(_formal_tmp_in_ind_chpl2 + INT64(2)) = *(coerce_tmp_chpl11 + INT64(2));
#line 391 "../lib/Chai.chpl"
        _formal_tmp_ind_x0_chpl2 = *(_formal_tmp_in_ind_chpl2 + INT64(0));
#line 391 "../lib/Chai.chpl"
        _formal_tmp_ind_x1_chpl2 = *(_formal_tmp_in_ind_chpl2 + INT64(1));
#line 391 "../lib/Chai.chpl"
        _formal_tmp_ind_x2_chpl2 = *(_formal_tmp_in_ind_chpl2 + INT64(2));
#line 391 "../lib/Chai.chpl"
        sum_chpl2 = INT64(0);
#line 391 "../lib/Chai.chpl"
        chpl_check_nil(coerce_tmp_chpl12, INT64(391), INT32(65));
#line 391 "../lib/Chai.chpl"
        call_tmp_chpl27 = &((coerce_tmp_chpl12)->blk);
#line 391 "../lib/Chai.chpl"
        i_lhs_chpl4 = &sum_chpl2;
#line 391 "../lib/Chai.chpl"
        *(i_lhs_chpl4) += ((int64_t)((_formal_tmp_ind_x0_chpl2 * *(*(call_tmp_chpl27) + INT64(0)))));
#line 391 "../lib/Chai.chpl"
        chpl_check_nil(coerce_tmp_chpl12, INT64(391), INT32(65));
#line 391 "../lib/Chai.chpl"
        call_tmp_chpl28 = &((coerce_tmp_chpl12)->blk);
#line 391 "../lib/Chai.chpl"
        i_lhs_chpl5 = &sum_chpl2;
#line 391 "../lib/Chai.chpl"
        *(i_lhs_chpl5) += ((int64_t)((_formal_tmp_ind_x1_chpl2 * *(*(call_tmp_chpl28) + INT64(1)))));
#line 391 "../lib/Chai.chpl"
        i_lhs_chpl6 = &sum_chpl2;
#line 391 "../lib/Chai.chpl"
        *(i_lhs_chpl6) += _formal_tmp_ind_x2_chpl2;
#line 391 "../lib/Chai.chpl"
        ret_chpl4 = sum_chpl2;
#line 391 "../lib/Chai.chpl"
        chpl_check_nil(coerce_tmp_chpl12, INT64(391), INT32(65));
#line 391 "../lib/Chai.chpl"
        coerce_tmp_chpl13 = (coerce_tmp_chpl12)->shiftedData;
#line 391 "../lib/Chai.chpl"
        call_tmp_chpl29 = (coerce_tmp_chpl13 + ret_chpl4);
#line 391 "../lib/Chai.chpl"
        *(call_tmp_chpl25) = *(call_tmp_chpl29);
#line 391 "../lib/Chai.chpl"
        chpl__autoDestroy16(&region_chpl, INT64(391), INT32(65));
#line 391 "../lib/Chai.chpl"
      }
#line 391 "../lib/Chai.chpl"
    }
#line 391 "../lib/Chai.chpl"
  }
#line 388 "../lib/Chai.chpl"
  return;
#line 388 "../lib/Chai.chpl"
}

#line 399 "../lib/Chai.chpl"
/* Chai.chpl:399 */
#line 399 "../lib/Chai.chpl"
static void backwardBatch_chpl2(MaxPool_chpl * this_chpl31,
#line 399 "../lib/Chai.chpl"
                                _array_DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl * deltas_chpl,
#line 399 "../lib/Chai.chpl"
                                _array_DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl * inputs_chpl,
#line 399 "../lib/Chai.chpl"
                                _array_DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl * _retArg_chpl) {
#line 402 "../lib/Chai.chpl"
  int64_t local_dataParMinGranularity_chpl;
#line 402 "../lib/Chai.chpl"
  chpl_bool local_dataParIgnoreRunningTasks_chpl;
#line 402 "../lib/Chai.chpl"
  int64_t local_dataParTasksPerLocale_chpl;
#line 400 "../lib/Chai.chpl"
  int64_t call_tmp_chpl6;
#line 401 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl newDeltas_chpl;
#line 401 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl call_tmp_chpl7;
#line 401 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl ret_tmp_chpl;
#line 401 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl8;
#line 401 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl2;
#line 401 "../lib/Chai.chpl"
  _domain_DefaultRectangularDom_1_int64_t_one_chpl call_tmp_chpl9;
#line 401 "../lib/Chai.chpl"
  _domain_DefaultRectangularDom_1_int64_t_one_chpl ret_tmp_chpl3;
#line 401 "../lib/Chai.chpl"
  chpl___RuntimeTypeInfo6 call_tmp_chpl10;
#line 401 "../lib/Chai.chpl"
  _domain_DefaultRectangularDom_1_int64_t_one_chpl _runtime_type_tmp__chpl;
#line 401 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl ret_tmp_chpl4;
#line 402 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl call_tmp_chpl11;
#line 402 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl ret_tmp_chpl5;
#line 402 "../lib/Chai.chpl"
  DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl coerce_tmp_chpl3 = NULL;
#line 402 "../lib/Chai.chpl"
  int64_t tasksPerLocale_chpl;
#line 402 "../lib/Chai.chpl"
  chpl_bool ignoreRunning_chpl;
#line 402 "../lib/Chai.chpl"
  int64_t minIndicesPerTask_chpl;
#line 402 "../lib/Chai.chpl"
  Error_chpl error_chpl = NULL;
#line 402 "../lib/Chai.chpl"
  DefaultRectangularDom_1_int64_t_one_chpl coerce_tmp_chpl4 = NULL;
#line 402 "../lib/Chai.chpl"
  int64_t offset_x0_chpl;
#line 402 "../lib/Chai.chpl"
  _tuple_1_star_int64_t_chpl ret_tmp_chpl6;
#line 402 "../lib/Chai.chpl"
  Error_chpl error_chpl2 = NULL;
#line 402 "../lib/Chai.chpl"
  locale_chpl call_tmp_chpl12;
#line 402 "../lib/Chai.chpl"
  chpl_localeID_t call_tmp_chpl13;
#line 402 "../lib/Chai.chpl"
  int32_t call_tmp_chpl14;
#line 402 "../lib/Chai.chpl"
  chpl_localeID_t call_tmp_chpl15;
#line 402 "../lib/Chai.chpl"
  locale_chpl call_tmp_chpl16;
#line 402 "../lib/Chai.chpl"
  locale_chpl ret_tmp_chpl7;
#line 402 "../lib/Chai.chpl"
  BaseLocale_chpl coerce_tmp_chpl5 = NULL;
#line 402 "../lib/Chai.chpl"
  _ref_locale_chpl i_x_chpl = NULL;
#line 402 "../lib/Chai.chpl"
  int64_t tmp_chpl;
#line 402 "../lib/Chai.chpl"
  locale_chpl call_tmp_chpl17;
#line 402 "../lib/Chai.chpl"
  chpl_localeID_t call_tmp_chpl18;
#line 402 "../lib/Chai.chpl"
  int32_t call_tmp_chpl19;
#line 402 "../lib/Chai.chpl"
  chpl_localeID_t call_tmp_chpl20;
#line 402 "../lib/Chai.chpl"
  locale_chpl call_tmp_chpl21;
#line 402 "../lib/Chai.chpl"
  locale_chpl ret_tmp_chpl8;
#line 402 "../lib/Chai.chpl"
  int64_t ret_chpl;
#line 402 "../lib/Chai.chpl"
  BaseLocale_chpl coerce_tmp_chpl6 = NULL;
#line 402 "../lib/Chai.chpl"
  BaseLocale_chpl coerce_tmp_chpl7 = NULL;
#line 402 "../lib/Chai.chpl"
  _ref_locale_chpl i_x_chpl2 = NULL;
#line 402 "../lib/Chai.chpl"
  int64_t tmp_x0_chpl;
#line 402 "../lib/Chai.chpl"
  int64_t tmp_x1_chpl;
#line 402 "../lib/Chai.chpl"
  _ref__tuple_1_star_range_int64_t_both_one_chpl call_tmp_chpl22 = NULL;
#line 402 "../lib/Chai.chpl"
  _tuple_2_star_int64_t_chpl ret_tmp_chpl9;
#line 402 "../lib/Chai.chpl"
  int64_t numChunks_chpl;
#line 402 "../lib/Chai.chpl"
  int64_t parDim_chpl;
#line 402 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl locBlock_x0_chpl;
#line 402 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl tup_x0_chpl;
#line 402 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl elt_x0_chpl;
#line 402 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl call_tmp_chpl23;
#line 402 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl ret_tmp_chpl10;
#line 402 "../lib/Chai.chpl"
  _ref__tuple_1_star_range_int64_t_both_one_chpl call_tmp_chpl24 = NULL;
#line 402 "../lib/Chai.chpl"
  int64_t call_tmp_chpl25;
#line 402 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl8;
#line 402 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl26;
#line 402 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl11;
#line 402 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl tmpIter_chpl;
#line 402 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl call_tmp_chpl27;
#line 402 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl ret_tmp_chpl12;
#line 402 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl13;
#line 402 "../lib/Chai.chpl"
  chpl___EndCount_AtomicT_int64_t_int64_t _coforallCount_chpl = NULL;
#line 402 "../lib/Chai.chpl"
  chpl___EndCount_AtomicT_int64_t_int64_t new_temp_chpl = NULL;
#line 402 "../lib/Chai.chpl"
  int64_t numTasks_chpl;
#line 402 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl _ic__F0_this_chpl;
#line 402 "../lib/Chai.chpl"
  int64_t i_chpl;
#line 402 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl this_chpl32;
#line 402 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl this_chpl33;
#line 402 "../lib/Chai.chpl"
  int64_t end_chpl;
#line 402 "../lib/Chai.chpl"
  _tuple_1_star_range_int64_t_both_one_chpl ret_tmp_chpl14;
#line 402 "../lib/Chai.chpl"
  _class_localscoforall_fn_chpl109 _args_forcoforall_fn_chpl = NULL;
#line 402 "../lib/Chai.chpl"
  _ref_MaxPool_chpl tmp_chpl2 = NULL;
#line 402 "../lib/Chai.chpl"
  _ref_range_int64_t_low_one_chpl tmp_chpl3 = NULL;
#line 402 "../lib/Chai.chpl"
  raw_c_void_ptr _args_vforcoforall_fn_chpl;
#line 402 "../lib/Chai.chpl"
  Error_chpl error_chpl3 = NULL;
#line 402 "../lib/Chai.chpl"
  chpl__class_localscoforall_fn_chpl109_object chpl_macro_tmp_114;
#line 402 "../lib/Chai.chpl"
  local_dataParMinGranularity_chpl = dataParMinGranularity_chpl;
#line 402 "../lib/Chai.chpl"
  local_dataParIgnoreRunningTasks_chpl = dataParIgnoreRunningTasks_chpl;
#line 402 "../lib/Chai.chpl"
  local_dataParTasksPerLocale_chpl = dataParTasksPerLocale_chpl;
#line 400 "../lib/Chai.chpl"
  call_tmp_chpl6 = size_chpl(deltas_chpl, INT64(400), INT32(65));
#line 401 "../lib/Chai.chpl"
  chpl_build_low_bounded_range2(INT64(0), &ret_tmp_chpl);
#line 401 "../lib/Chai.chpl"
  call_tmp_chpl7 = ret_tmp_chpl;
#line 401 "../lib/Chai.chpl"
  chpl___POUND_3(call_tmp_chpl7, call_tmp_chpl6, &ret_tmp_chpl2, INT64(401), INT32(65));
#line 401 "../lib/Chai.chpl"
  call_tmp_chpl8 = ret_tmp_chpl2;
#line 401 "../lib/Chai.chpl"
  chpl__ensureDomainExpr5(call_tmp_chpl8, &ret_tmp_chpl3, INT64(401), INT32(65));
#line 401 "../lib/Chai.chpl"
  call_tmp_chpl9 = ret_tmp_chpl3;
#line 401 "../lib/Chai.chpl"
  call_tmp_chpl10 = chpl__buildArrayRuntimeType2(&call_tmp_chpl9);
#line 401 "../lib/Chai.chpl"
  _runtime_type_tmp__chpl = (&call_tmp_chpl10)->dom;
#line 401 "../lib/Chai.chpl"
  chpl__convertRuntimeTypeToValue3(&_runtime_type_tmp__chpl, UINT8(false), &ret_tmp_chpl4, INT64(401), INT32(65));
#line 401 "../lib/Chai.chpl"
  newDeltas_chpl = ret_tmp_chpl4;
#line 402 "../lib/Chai.chpl"
  chpl_build_low_bounded_range2(INT64(0), &ret_tmp_chpl5);
#line 402 "../lib/Chai.chpl"
  call_tmp_chpl11 = ret_tmp_chpl5;
#line 402 "../lib/Chai.chpl"
  coerce_tmp_chpl3 = (deltas_chpl)->_instance;
#line 402 "../lib/Chai.chpl"
  tasksPerLocale_chpl = local_dataParTasksPerLocale_chpl;
#line 402 "../lib/Chai.chpl"
  ignoreRunning_chpl = local_dataParIgnoreRunningTasks_chpl;
#line 402 "../lib/Chai.chpl"
  minIndicesPerTask_chpl = local_dataParMinGranularity_chpl;
#line 402 "../lib/Chai.chpl"
  error_chpl = NULL;
#line 402 "../lib/Chai.chpl"
  chpl_check_nil(coerce_tmp_chpl3, INT64(402), INT32(65));
#line 402 "../lib/Chai.chpl"
  coerce_tmp_chpl4 = (coerce_tmp_chpl3)->dom;
#line 402 "../lib/Chai.chpl"
  createTuple_chpl(INT64(0), &ret_tmp_chpl6);
#line 402 "../lib/Chai.chpl"
  offset_x0_chpl = *(ret_tmp_chpl6 + INT64(0));
#line 402 "../lib/Chai.chpl"
  error_chpl2 = NULL;
#line 402 "../lib/Chai.chpl"
  call_tmp_chpl14 = get_chpl_nodeID();
#line 402 "../lib/Chai.chpl"
  call_tmp_chpl15 = chpl_rt_buildLocaleID(call_tmp_chpl14, c_sublocid_any);
#line 402 "../lib/Chai.chpl"
  call_tmp_chpl13 = call_tmp_chpl15;
#line 402 "../lib/Chai.chpl"
  chpl_localeID_to_locale(&call_tmp_chpl13, &ret_tmp_chpl7, INT64(402), INT32(65));
#line 402 "../lib/Chai.chpl"
  call_tmp_chpl16 = ret_tmp_chpl7;
#line 402 "../lib/Chai.chpl"
  call_tmp_chpl12 = call_tmp_chpl16;
#line 402 "../lib/Chai.chpl"
  coerce_tmp_chpl5 = (&call_tmp_chpl12)->_instance;
#line 402 "../lib/Chai.chpl"
  if (((RootClass_chpl)(coerce_tmp_chpl5)) == nil) /* ZLINE: 402 ../lib/Chai.chpl */
#line 402 "../lib/Chai.chpl"
  {
#line 402 "../lib/Chai.chpl"
    nilCheckHalt_chpl(&_str_literal_52_chpl, INT64(402), INT32(65));
#line 402 "../lib/Chai.chpl"
  }
#line 402 "../lib/Chai.chpl"
  chpl_check_nil(coerce_tmp_chpl5, INT64(402), INT32(65));
#line 402 "../lib/Chai.chpl"
  ((int64_t(*)(BaseLocale_chpl,
#line 402 "../lib/Chai.chpl"
     int64_t,
#line 402 "../lib/Chai.chpl"
     int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(coerce_tmp_chpl5))->chpl__cid) + INT64(2))])(coerce_tmp_chpl5, INT64(402), INT32(65));
#line 402 "../lib/Chai.chpl"
  i_x_chpl = &call_tmp_chpl12;
#line 402 "../lib/Chai.chpl"
  deinit_chpl62(i_x_chpl);
#line 402 "../lib/Chai.chpl"
  if (tasksPerLocale_chpl == INT64(0)) /* ZLINE: 402 ../lib/Chai.chpl */
#line 402 "../lib/Chai.chpl"
  {
#line 402 "../lib/Chai.chpl"
    call_tmp_chpl19 = get_chpl_nodeID();
#line 402 "../lib/Chai.chpl"
    call_tmp_chpl20 = chpl_rt_buildLocaleID(call_tmp_chpl19, c_sublocid_any);
#line 402 "../lib/Chai.chpl"
    call_tmp_chpl18 = call_tmp_chpl20;
#line 402 "../lib/Chai.chpl"
    chpl_localeID_to_locale(&call_tmp_chpl18, &ret_tmp_chpl8, INT64(402), INT32(65));
#line 402 "../lib/Chai.chpl"
    call_tmp_chpl21 = ret_tmp_chpl8;
#line 402 "../lib/Chai.chpl"
    call_tmp_chpl17 = call_tmp_chpl21;
#line 402 "../lib/Chai.chpl"
    coerce_tmp_chpl6 = (&call_tmp_chpl17)->_instance;
#line 402 "../lib/Chai.chpl"
    if (((RootClass_chpl)(coerce_tmp_chpl6)) == nil) /* ZLINE: 402 ../lib/Chai.chpl */
#line 402 "../lib/Chai.chpl"
    {
#line 402 "../lib/Chai.chpl"
      nilCheckHalt_chpl(&_str_literal_52_chpl, INT64(402), INT32(65));
#line 402 "../lib/Chai.chpl"
    }
#line 402 "../lib/Chai.chpl"
    coerce_tmp_chpl7 = coerce_tmp_chpl6;
#line 402 "../lib/Chai.chpl"
    chpl_check_nil(coerce_tmp_chpl7, INT64(402), INT32(65));
#line 402 "../lib/Chai.chpl"
    ret_chpl = (coerce_tmp_chpl7)->maxTaskPar;
#line 402 "../lib/Chai.chpl"
    tmp_chpl = ret_chpl;
#line 402 "../lib/Chai.chpl"
    i_x_chpl2 = &call_tmp_chpl17;
#line 402 "../lib/Chai.chpl"
    deinit_chpl62(i_x_chpl2);
#line 402 "../lib/Chai.chpl"
  } else /* ZLINE: 402 ../lib/Chai.chpl */
#line 402 "../lib/Chai.chpl"
  {
#line 402 "../lib/Chai.chpl"
    tmp_chpl = tasksPerLocale_chpl;
#line 402 "../lib/Chai.chpl"
  }
#line 402 "../lib/Chai.chpl"
  if (chpl_task_getSerial()) /* ZLINE: 402 ../lib/Chai.chpl */
#line 402 "../lib/Chai.chpl"
  {
#line 402 "../lib/Chai.chpl"
    tmp_x0_chpl = INT64(1);
#line 402 "../lib/Chai.chpl"
    tmp_x1_chpl = INT64(0);
#line 402 "../lib/Chai.chpl"
  } else /* ZLINE: 402 ../lib/Chai.chpl */
#line 402 "../lib/Chai.chpl"
  {
#line 402 "../lib/Chai.chpl"
    chpl_check_nil(coerce_tmp_chpl4, INT64(402), INT32(65));
#line 402 "../lib/Chai.chpl"
    call_tmp_chpl22 = &((coerce_tmp_chpl4)->ranges);
#line 402 "../lib/Chai.chpl"
    _computeChunkStuff_chpl(tmp_chpl, ignoreRunning_chpl, minIndicesPerTask_chpl, call_tmp_chpl22, &ret_tmp_chpl9, INT64(402), INT32(65));
#line 402 "../lib/Chai.chpl"
    tmp_x0_chpl = *(ret_tmp_chpl9 + INT64(0));
#line 402 "../lib/Chai.chpl"
    tmp_x1_chpl = *(ret_tmp_chpl9 + INT64(1));
#line 402 "../lib/Chai.chpl"
  }
#line 402 "../lib/Chai.chpl"
  numChunks_chpl = tmp_x0_chpl;
#line 402 "../lib/Chai.chpl"
  parDim_chpl = tmp_x1_chpl;
#line 402 "../lib/Chai.chpl"
  init_chpl113(&elt_x0_chpl);
#line 402 "../lib/Chai.chpl"
  tup_x0_chpl = elt_x0_chpl;
#line 402 "../lib/Chai.chpl"
  locBlock_x0_chpl = tup_x0_chpl;
#line 402 "../lib/Chai.chpl"
  chpl_build_low_bounded_range2(offset_x0_chpl, &ret_tmp_chpl10);
#line 402 "../lib/Chai.chpl"
  call_tmp_chpl23 = ret_tmp_chpl10;
#line 402 "../lib/Chai.chpl"
  chpl_check_nil(coerce_tmp_chpl4, INT64(402), INT32(65));
#line 402 "../lib/Chai.chpl"
  call_tmp_chpl24 = &((coerce_tmp_chpl4)->ranges);
#line 402 "../lib/Chai.chpl"
  coerce_tmp_chpl8 = *(*(call_tmp_chpl24) + INT64(0));
#line 402 "../lib/Chai.chpl"
  call_tmp_chpl25 = sizeAs_chpl37(coerce_tmp_chpl8, INT64(402), INT32(65));
#line 402 "../lib/Chai.chpl"
  chpl___POUND_3(call_tmp_chpl23, call_tmp_chpl25, &ret_tmp_chpl11, INT64(402), INT32(65));
#line 402 "../lib/Chai.chpl"
  call_tmp_chpl26 = ret_tmp_chpl11;
#line 402 "../lib/Chai.chpl"
  locBlock_x0_chpl = call_tmp_chpl26;
#line 402 "../lib/Chai.chpl"
  chpl_build_low_bounded_range2(INT64(0), &ret_tmp_chpl12);
#line 402 "../lib/Chai.chpl"
  call_tmp_chpl27 = ret_tmp_chpl12;
#line 402 "../lib/Chai.chpl"
  chpl___POUND_3(call_tmp_chpl27, tmp_x0_chpl, &ret_tmp_chpl13, INT64(402), INT32(65));
#line 402 "../lib/Chai.chpl"
  tmpIter_chpl = ret_tmp_chpl13;
#line 402 "../lib/Chai.chpl"
  new_temp_chpl = _new_chpl17(INT64(402), INT32(65));
#line 402 "../lib/Chai.chpl"
  _coforallCount_chpl = new_temp_chpl;
#line 402 "../lib/Chai.chpl"
  numTasks_chpl = chpl_boundedCoforallSize(tmpIter_chpl, INT64(402), INT32(65));
#line 402 "../lib/Chai.chpl"
  _upEndCount_chpl(_coforallCount_chpl, numTasks_chpl, INT64(402), INT32(65));
#line 402 "../lib/Chai.chpl"
  chpl_resetTaskSpawn(numTasks_chpl, INT64(402), INT32(65));
#line 402 "../lib/Chai.chpl"
  _ic__F0_this_chpl = tmpIter_chpl;
#line 402 "../lib/Chai.chpl"
  checkIfIterWillOverflow_chpl(tmpIter_chpl, UINT8(true), INT64(402), INT32(65));
#line 402 "../lib/Chai.chpl"
  i_chpl = INT64(0);
#line 402 "../lib/Chai.chpl"
  this_chpl32 = _ic__F0_this_chpl;
#line 402 "../lib/Chai.chpl"
  this_chpl33 = _ic__F0_this_chpl;
#line 402 "../lib/Chai.chpl"
  end_chpl = (&this_chpl33)->_high;
#line 402 "../lib/Chai.chpl"
  for (i_chpl = (&this_chpl32)->_low; ((i_chpl <= end_chpl)); i_chpl += INT64(1)) {
#line 402 "../lib/Chai.chpl"
    *(ret_tmp_chpl14 + INT64(0)) = locBlock_x0_chpl;
#line 402 "../lib/Chai.chpl"
    _args_forcoforall_fn_chpl = ((_class_localscoforall_fn_chpl109)(&chpl_macro_tmp_114));
#line 402 "../lib/Chai.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl, INT64(402), INT32(65));
#line 402 "../lib/Chai.chpl"
    (_args_forcoforall_fn_chpl)->_0_numChunks = numChunks_chpl;
#line 402 "../lib/Chai.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl, INT64(402), INT32(65));
#line 402 "../lib/Chai.chpl"
    (_args_forcoforall_fn_chpl)->_1_parDim = parDim_chpl;
#line 402 "../lib/Chai.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl, INT64(402), INT32(65));
#line 402 "../lib/Chai.chpl"
    *((_args_forcoforall_fn_chpl)->_2_coerce_tmp + INT64(0)) = *(ret_tmp_chpl14 + INT64(0));
#line 402 "../lib/Chai.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl, INT64(402), INT32(65));
#line 402 "../lib/Chai.chpl"
    (_args_forcoforall_fn_chpl)->_3__coforallCount = _coforallCount_chpl;
#line 402 "../lib/Chai.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl, INT64(402), INT32(65));
#line 402 "../lib/Chai.chpl"
    (_args_forcoforall_fn_chpl)->_4_chunk = i_chpl;
#line 402 "../lib/Chai.chpl"
    tmp_chpl2 = this_chpl31;
#line 402 "../lib/Chai.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl, INT64(402), INT32(65));
#line 402 "../lib/Chai.chpl"
    (_args_forcoforall_fn_chpl)->_6_this = tmp_chpl2;
#line 402 "../lib/Chai.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl, INT64(402), INT32(65));
#line 402 "../lib/Chai.chpl"
    (_args_forcoforall_fn_chpl)->_7_deltas = *(deltas_chpl);
#line 402 "../lib/Chai.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl, INT64(402), INT32(65));
#line 402 "../lib/Chai.chpl"
    (_args_forcoforall_fn_chpl)->_8_inputs = *(inputs_chpl);
#line 402 "../lib/Chai.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl, INT64(402), INT32(65));
#line 402 "../lib/Chai.chpl"
    (_args_forcoforall_fn_chpl)->_9_newDeltas = newDeltas_chpl;
#line 402 "../lib/Chai.chpl"
    tmp_chpl3 = &call_tmp_chpl11;
#line 402 "../lib/Chai.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl, INT64(402), INT32(65));
#line 402 "../lib/Chai.chpl"
    (_args_forcoforall_fn_chpl)->_10_call_tmp = tmp_chpl3;
#line 402 "../lib/Chai.chpl"
    _args_vforcoforall_fn_chpl = ((void*)(_args_forcoforall_fn_chpl));
#line 402 "../lib/Chai.chpl"
    /*** wrapcoforall_fn_chpl109 ***/ chpl_taskAddCoStmt(chpl_task_getRequestedSubloc(), INT64(108), ((chpl_task_bundle_p)(_args_vforcoforall_fn_chpl)), sizeof(chpl__class_localscoforall_fn_chpl109_object), INT64(402), INT32(65));
#line 402 "../lib/Chai.chpl"
  }
#line 402 "../lib/Chai.chpl"
  error_chpl3 = NULL;
#line 402 "../lib/Chai.chpl"
  _waitEndCount_chpl2(_coforallCount_chpl, numTasks_chpl, &error_chpl3, INT64(402), INT32(65));
#line 402 "../lib/Chai.chpl"
  if (error_chpl3 != nil) /* ZLINE: 402 ../lib/Chai.chpl */
#line 402 "../lib/Chai.chpl"
  {
#line 402 "../lib/Chai.chpl"
    chpl__delete72(_coforallCount_chpl, INT64(402), INT32(65));
#line 402 "../lib/Chai.chpl"
    error_chpl2 = error_chpl3;
#line 402 "../lib/Chai.chpl"
    goto forall_IBB_break_label_chpl;
#line 402 "../lib/Chai.chpl"
  }
#line 402 "../lib/Chai.chpl"
  chpl__delete72(_coforallCount_chpl, INT64(402), INT32(65));
#line 402 "../lib/Chai.chpl"
  chpl_after_forall_fence();
#line 402 "../lib/Chai.chpl"
  forall_IBB_break_label_chpl:;
#line 402 "../lib/Chai.chpl"
  if (error_chpl2 != nil) /* ZLINE: 402 ../lib/Chai.chpl */
#line 402 "../lib/Chai.chpl"
  {
#line 402 "../lib/Chai.chpl"
    error_chpl = error_chpl2;
#line 402 "../lib/Chai.chpl"
    goto forall_IBB_break_label_chpl2;
#line 402 "../lib/Chai.chpl"
  }
#line 402 "../lib/Chai.chpl"
  chpl_after_forall_fence();
#line 402 "../lib/Chai.chpl"
  forall_IBB_break_label_chpl2:;
#line 402 "../lib/Chai.chpl"
  if (error_chpl != nil) /* ZLINE: 402 ../lib/Chai.chpl */
#line 402 "../lib/Chai.chpl"
  {
#line 402 "../lib/Chai.chpl"
    chpl_propagate_error(error_chpl, INT64(402), INT32(65));
#line 402 "../lib/Chai.chpl"
  }
#line 402 "../lib/Chai.chpl"
  chpl_after_forall_fence();
#line 405 "../lib/Chai.chpl"
  chpl__checkRetEltTypeMatch2(&newDeltas_chpl);
#line 405 "../lib/Chai.chpl"
  chpl__autoDestroy11(&call_tmp_chpl9, INT64(405), INT32(65));
#line 399 "../lib/Chai.chpl"
  *(_retArg_chpl) = newDeltas_chpl;
#line 399 "../lib/Chai.chpl"
  return;
#line 399 "../lib/Chai.chpl"
}

#line 402 "../lib/Chai.chpl"
/* Chai.chpl:402 */
#line 402 "../lib/Chai.chpl"
static void wrapcoforall_fn_chpl109(_class_localscoforall_fn_chpl109 c_chpl) {
#line 402 "../lib/Chai.chpl"
  Error_chpl error_chpl = NULL;
#line 402 "../lib/Chai.chpl"
  int64_t _0_numChunks_chpl;
#line 402 "../lib/Chai.chpl"
  int64_t _1_parDim_chpl;
#line 402 "../lib/Chai.chpl"
  _tuple_1_star_range_int64_t_both_one_chpl _2_coerce_tmp_chpl;
#line 402 "../lib/Chai.chpl"
  chpl___EndCount_AtomicT_int64_t_int64_t _3__coforallCount_chpl = NULL;
#line 402 "../lib/Chai.chpl"
  int64_t _4_chunk_chpl;
#line 402 "../lib/Chai.chpl"
  _ref_MaxPool_chpl _6_this_chpl = NULL;
#line 402 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl _7_deltas_chpl;
#line 402 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl _8_inputs_chpl;
#line 402 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl _9_newDeltas_chpl;
#line 402 "../lib/Chai.chpl"
  _ref_range_int64_t_low_one_chpl _10_call_tmp_chpl = NULL;
#line 402 "../lib/Chai.chpl"
  chpl_rmem_consist_acquire(INT64(402), INT32(65));
#line 402 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(402), INT32(65));
#line 402 "../lib/Chai.chpl"
  _0_numChunks_chpl = (c_chpl)->_0_numChunks;
#line 402 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(402), INT32(65));
#line 402 "../lib/Chai.chpl"
  _1_parDim_chpl = (c_chpl)->_1_parDim;
#line 402 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(402), INT32(65));
#line 402 "../lib/Chai.chpl"
  *(_2_coerce_tmp_chpl + INT64(0)) = *((c_chpl)->_2_coerce_tmp + INT64(0));
#line 402 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(402), INT32(65));
#line 402 "../lib/Chai.chpl"
  _3__coforallCount_chpl = (c_chpl)->_3__coforallCount;
#line 402 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(402), INT32(65));
#line 402 "../lib/Chai.chpl"
  _4_chunk_chpl = (c_chpl)->_4_chunk;
#line 402 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(402), INT32(65));
#line 402 "../lib/Chai.chpl"
  _6_this_chpl = (c_chpl)->_6_this;
#line 402 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(402), INT32(65));
#line 402 "../lib/Chai.chpl"
  _7_deltas_chpl = (c_chpl)->_7_deltas;
#line 402 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(402), INT32(65));
#line 402 "../lib/Chai.chpl"
  _8_inputs_chpl = (c_chpl)->_8_inputs;
#line 402 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(402), INT32(65));
#line 402 "../lib/Chai.chpl"
  _9_newDeltas_chpl = (c_chpl)->_9_newDeltas;
#line 402 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(402), INT32(65));
#line 402 "../lib/Chai.chpl"
  _10_call_tmp_chpl = (c_chpl)->_10_call_tmp;
#line 402 "../lib/Chai.chpl"
  coforall_fn_chpl109(_0_numChunks_chpl, _1_parDim_chpl, &_2_coerce_tmp_chpl, _3__coforallCount_chpl, _4_chunk_chpl, &error_chpl, _6_this_chpl, &_7_deltas_chpl, &_8_inputs_chpl, &_9_newDeltas_chpl, _10_call_tmp_chpl);
#line 402 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(402), INT32(65));
#line 402 "../lib/Chai.chpl"
  _downEndCount_chpl((c_chpl)->_3__coforallCount, error_chpl, INT64(402), INT32(65));
#line 402 "../lib/Chai.chpl"
  return;
#line 402 "../lib/Chai.chpl"
}

#line 402 "../lib/Chai.chpl"
/* Chai.chpl:402 */
#line 402 "../lib/Chai.chpl"
static void coforall_fn_chpl109(int64_t numChunks_chpl,
#line 402 "../lib/Chai.chpl"
                                int64_t parDim_chpl,
#line 402 "../lib/Chai.chpl"
                                _tuple_1_star_range_int64_t_both_one_chpl * locBlock_chpl,
#line 402 "../lib/Chai.chpl"
                                chpl___EndCount_AtomicT_int64_t_int64_t _coforallCount_chpl,
#line 402 "../lib/Chai.chpl"
                                int64_t chunk_chpl,
#line 402 "../lib/Chai.chpl"
                                Error_chpl * error_out_chpl,
#line 402 "../lib/Chai.chpl"
                                MaxPool_chpl * this_chpl31,
#line 402 "../lib/Chai.chpl"
                                _array_DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl * deltas_chpl,
#line 402 "../lib/Chai.chpl"
                                _array_DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl * inputs_chpl,
#line 402 "../lib/Chai.chpl"
                                _array_DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl * newDeltas_chpl,
#line 402 "../lib/Chai.chpl"
                                range_int64_t_low_one_chpl * call_tmp_chpl6) {
#line 402 "../lib/Chai.chpl"
  _tuple_1_star_range_int64_t_both_one_chpl followMe_chpl;
#line 402 "../lib/Chai.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl7 = NULL;
#line 402 "../lib/Chai.chpl"
  int64_t call_tmp_chpl8;
#line 402 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl3;
#line 402 "../lib/Chai.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl9 = NULL;
#line 402 "../lib/Chai.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl10 = NULL;
#line 402 "../lib/Chai.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl11 = NULL;
#line 402 "../lib/Chai.chpl"
  _tuple_2_star_int64_t_chpl call_tmp_chpl12;
#line 402 "../lib/Chai.chpl"
  _tuple_2_star_int64_t_chpl ret_tmp_chpl;
#line 402 "../lib/Chai.chpl"
  int64_t tmp_x0_chpl;
#line 402 "../lib/Chai.chpl"
  int64_t tmp_x1_chpl;
#line 402 "../lib/Chai.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl13 = NULL;
#line 402 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl14;
#line 402 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl2;
#line 402 "../lib/Chai.chpl"
  _tuple_1_star_range_int64_t_both_one_chpl _yield_expr_tmp__chpl;
#line 402 "../lib/Chai.chpl"
  _tuple_1_star_range_int64_t_both_one_chpl chpl_followThis;
#line 402 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl _ir_F0_this_chpl;
#line 402 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl _ic__F5_block_x0_chpl;
#line 402 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl _ic__F7_coerce_tmp_x0_chpl;
#line 402 "../lib/Chai.chpl"
  int64_t _ic__F8_i_chpl;
#line 402 "../lib/Chai.chpl"
  int64_t _ic__F9_end_chpl;
#line 402 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl _ir_F0_this_chpl2;
#line 402 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl _ic__F5_block_x0_chpl2;
#line 402 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl _ic__F7_coerce_tmp_x0_chpl2;
#line 402 "../lib/Chai.chpl"
  int64_t _ic__F8_i_chpl2;
#line 402 "../lib/Chai.chpl"
  int64_t _ic__F9_end_chpl2;
#line 402 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl coerce_tmp_chpl4;
#line 402 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl _ic__F0_this_chpl;
#line 402 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl _ic__F2_myFollowThis_chpl;
#line 402 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl _ic__F3_r_chpl;
#line 402 "../lib/Chai.chpl"
  int64_t _ic__F5_i_chpl;
#line 402 "../lib/Chai.chpl"
  DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl this_chpl32 = NULL;
#line 402 "../lib/Chai.chpl"
  DefaultRectangularDom_1_int64_t_one_chpl coerce_tmp_chpl5 = NULL;
#line 402 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl6;
#line 402 "../lib/Chai.chpl"
  DefaultRectangularDom_1_int64_t_one_chpl this_chpl33 = NULL;
#line 402 "../lib/Chai.chpl"
  uint64_t call_tmp_chpl15;
#line 402 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl7;
#line 402 "../lib/Chai.chpl"
  DefaultRectangularDom_1_int64_t_one_chpl this_chpl34 = NULL;
#line 402 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl8;
#line 402 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl9;
#line 402 "../lib/Chai.chpl"
  DefaultRectangularDom_1_int64_t_one_chpl this_chpl35 = NULL;
#line 402 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl10;
#line 402 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl11;
#line 402 "../lib/Chai.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl16 = NULL;
#line 402 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl this_chpl36;
#line 402 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl this_chpl37;
#line 402 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl tmp_chpl;
#line 402 "../lib/Chai.chpl"
  DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl ret_chpl = NULL;
#line 402 "../lib/Chai.chpl"
  DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl _ic__F0_this_chpl2 = NULL;
#line 402 "../lib/Chai.chpl"
  _tuple_1_star_int64_t_chpl ret_tmp_chpl3;
#line 402 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl tup_x0_chpl;
#line 402 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl elt_x0_chpl;
#line 402 "../lib/Chai.chpl"
  int64_t coerce_tmp_chpl12;
#line 402 "../lib/Chai.chpl"
  _ref__tuple_1_star_range_int64_t_both_one_chpl call_tmp_chpl17 = NULL;
#line 402 "../lib/Chai.chpl"
  chpl_bool tmp_chpl2;
#line 402 "../lib/Chai.chpl"
  _ref__tuple_1_star_range_int64_t_both_one_chpl call_tmp_chpl18 = NULL;
#line 402 "../lib/Chai.chpl"
  int64_t call_tmp_chpl19;
#line 402 "../lib/Chai.chpl"
  _ref__tuple_1_star_range_int64_t_both_one_chpl call_tmp_chpl20 = NULL;
#line 402 "../lib/Chai.chpl"
  int64_t call_tmp_chpl21;
#line 402 "../lib/Chai.chpl"
  _tuple_1_star_range_int64_t_both_one_chpl retcopy_chpl;
#line 402 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl _ic__F0_this_chpl3;
#line 402 "../lib/Chai.chpl"
  int64_t coerce_tmp_chpl13;
#line 402 "../lib/Chai.chpl"
  DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl this_chpl38 = NULL;
#line 402 "../lib/Chai.chpl"
  DefaultRectangularDom_1_int64_t_one_chpl coerce_tmp_chpl14 = NULL;
#line 402 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl15;
#line 402 "../lib/Chai.chpl"
  DefaultRectangularDom_1_int64_t_one_chpl this_chpl39 = NULL;
#line 402 "../lib/Chai.chpl"
  uint64_t call_tmp_chpl22;
#line 402 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl16;
#line 402 "../lib/Chai.chpl"
  DefaultRectangularDom_1_int64_t_one_chpl this_chpl40 = NULL;
#line 402 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl17;
#line 402 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl18;
#line 402 "../lib/Chai.chpl"
  DefaultRectangularDom_1_int64_t_one_chpl this_chpl41 = NULL;
#line 402 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl19;
#line 402 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl20;
#line 402 "../lib/Chai.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl23 = NULL;
#line 402 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl this_chpl42;
#line 402 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl this_chpl43;
#line 402 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl tmp_chpl3;
#line 402 "../lib/Chai.chpl"
  DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl ret_chpl2 = NULL;
#line 402 "../lib/Chai.chpl"
  DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl _ic__F0_this_chpl4 = NULL;
#line 402 "../lib/Chai.chpl"
  _tuple_1_star_int64_t_chpl ret_tmp_chpl4;
#line 402 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl tup_x0_chpl2;
#line 402 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl elt_x0_chpl2;
#line 402 "../lib/Chai.chpl"
  int64_t coerce_tmp_chpl21;
#line 402 "../lib/Chai.chpl"
  _ref__tuple_1_star_range_int64_t_both_one_chpl call_tmp_chpl24 = NULL;
#line 402 "../lib/Chai.chpl"
  chpl_bool tmp_chpl4;
#line 402 "../lib/Chai.chpl"
  _ref__tuple_1_star_range_int64_t_both_one_chpl call_tmp_chpl25 = NULL;
#line 402 "../lib/Chai.chpl"
  int64_t call_tmp_chpl26;
#line 402 "../lib/Chai.chpl"
  _ref__tuple_1_star_range_int64_t_both_one_chpl call_tmp_chpl27 = NULL;
#line 402 "../lib/Chai.chpl"
  int64_t call_tmp_chpl28;
#line 402 "../lib/Chai.chpl"
  _tuple_1_star_range_int64_t_both_one_chpl retcopy_chpl2;
#line 402 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl _ic__F0_this_chpl5;
#line 402 "../lib/Chai.chpl"
  int64_t coerce_tmp_chpl22;
#line 402 "../lib/Chai.chpl"
  chpl_bool hasMore_chpl;
#line 402 "../lib/Chai.chpl"
  int64_t call_tmp_chpl29;
#line 402 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl this_chpl44;
#line 402 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl tmp_chpl5;
#line 402 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl tmp_chpl6;
#line 402 "../lib/Chai.chpl"
  _ref_range_int64_t_both_one_chpl tmp_chpl7 = NULL;
#line 402 "../lib/Chai.chpl"
  chpl_bool tmp_chpl8;
#line 402 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl tmp_chpl9;
#line 402 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl tmp_chpl10;
#line 402 "../lib/Chai.chpl"
  int64_t call_tmp_chpl30;
#line 402 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl tmp_chpl11;
#line 402 "../lib/Chai.chpl"
  int64_t call_tmp_chpl31;
#line 402 "../lib/Chai.chpl"
  _ref_range_int64_t_both_one_chpl tmp_chpl12 = NULL;
#line 402 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl23;
#line 402 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl _ic__F0_this_chpl6;
#line 402 "../lib/Chai.chpl"
  DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl this_chpl45 = NULL;
#line 402 "../lib/Chai.chpl"
  _ddata_Tensor_3__real64_chpl coerce_tmp_chpl24 = NULL;
#line 402 "../lib/Chai.chpl"
  _ref_Tensor_3__real64_chpl call_tmp_chpl32 = NULL;
#line 402 "../lib/Chai.chpl"
  int64_t tmp_chpl13;
#line 402 "../lib/Chai.chpl"
  DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl this_chpl46 = NULL;
#line 402 "../lib/Chai.chpl"
  _ddata_Tensor_3__real64_chpl coerce_tmp_chpl25 = NULL;
#line 402 "../lib/Chai.chpl"
  _ref_Tensor_3__real64_chpl call_tmp_chpl33 = NULL;
#line 403 "../lib/Chai.chpl"
  _tuple_1_star_int64_t_chpl coerce_tmp_chpl26;
#line 403 "../lib/Chai.chpl"
  _ref__tuple_1_star_int64_t_chpl i_i_chpl = NULL;
#line 403 "../lib/Chai.chpl"
  DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl ret_chpl3 = NULL;
#line 403 "../lib/Chai.chpl"
  DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl coerce_tmp_chpl27 = NULL;
#line 403 "../lib/Chai.chpl"
  _tuple_1_star_int64_t_chpl _formal_tmp_in_ind_chpl;
#line 403 "../lib/Chai.chpl"
  int64_t _formal_tmp_ind_x0_chpl;
#line 403 "../lib/Chai.chpl"
  _ddata_Tensor_3__real64_chpl coerce_tmp_chpl28 = NULL;
#line 403 "../lib/Chai.chpl"
  _ref_Tensor_3__real64_chpl call_tmp_chpl34 = NULL;
#line 403 "../lib/Chai.chpl"
  Tensor_3__real64_chpl call_tmp_chpl35;
#line 403 "../lib/Chai.chpl"
  Tensor_3__real64_chpl ret_tmp_chpl5;
#line 403 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_3_int64_t_one__real64_int64_t_chpl _field_destructor_tmp__chpl;
#line 403 "../lib/Chai.chpl"
  _domain_DefaultRectangularDom_3_int64_t_one_chpl _field_destructor_tmp__chpl2;
#line 402 "../lib/Chai.chpl"
  int64_t tmp_chpl14;
#line 402 "../lib/Chai.chpl"
  int64_t tmp_chpl15;
#line 402 "../lib/Chai.chpl"
  int64_t tmp_chpl16;
#line 402 "../lib/Chai.chpl"
  int64_t tmp_chpl17;
#line 402 "../lib/Chai.chpl"
  int64_t tmp_chpl18;
#line 402 "../lib/Chai.chpl"
  *(followMe_chpl + INT64(0)) = *(*(locBlock_chpl) + INT64(0));
#line 402 "../lib/Chai.chpl"
  call_tmp_chpl7 = this_chpl20(locBlock_chpl, parDim_chpl, INT64(402), INT32(65));
#line 402 "../lib/Chai.chpl"
  coerce_tmp_chpl3 = *(call_tmp_chpl7);
#line 402 "../lib/Chai.chpl"
  call_tmp_chpl8 = sizeAs_chpl37(coerce_tmp_chpl3, INT64(402), INT32(65));
#line 402 "../lib/Chai.chpl"
  call_tmp_chpl9 = this_chpl20(locBlock_chpl, parDim_chpl, INT64(402), INT32(65));
#line 402 "../lib/Chai.chpl"
  call_tmp_chpl10 = this_chpl20(locBlock_chpl, parDim_chpl, INT64(402), INT32(65));
#line 402 "../lib/Chai.chpl"
  call_tmp_chpl11 = this_chpl20(locBlock_chpl, parDim_chpl, INT64(402), INT32(65));
#line 402 "../lib/Chai.chpl"
  _computeBlock_chpl(call_tmp_chpl8, numChunks_chpl, chunk_chpl, (call_tmp_chpl9)->_high, (call_tmp_chpl10)->_low, (call_tmp_chpl11)->_low, &ret_tmp_chpl, INT64(402), INT32(65));
#line 402 "../lib/Chai.chpl"
  *(call_tmp_chpl12 + INT64(0)) = *(ret_tmp_chpl + INT64(0));
#line 402 "../lib/Chai.chpl"
  *(call_tmp_chpl12 + INT64(1)) = *(ret_tmp_chpl + INT64(1));
#line 402 "../lib/Chai.chpl"
  tmp_x0_chpl = *(call_tmp_chpl12 + INT64(0));
#line 402 "../lib/Chai.chpl"
  tmp_x1_chpl = *(call_tmp_chpl12 + INT64(1));
#line 402 "../lib/Chai.chpl"
  call_tmp_chpl13 = this_chpl20(&followMe_chpl, parDim_chpl, INT64(402), INT32(65));
#line 402 "../lib/Chai.chpl"
  chpl_build_bounded_range6(tmp_x0_chpl, tmp_x1_chpl, &ret_tmp_chpl2);
#line 402 "../lib/Chai.chpl"
  call_tmp_chpl14 = ret_tmp_chpl2;
#line 402 "../lib/Chai.chpl"
  *(call_tmp_chpl13) = call_tmp_chpl14;
#line 402 "../lib/Chai.chpl"
  *(_yield_expr_tmp__chpl + INT64(0)) = *(followMe_chpl + INT64(0));
#line 402 "../lib/Chai.chpl"
  *(chpl_followThis + INT64(0)) = *(_yield_expr_tmp__chpl + INT64(0));
#line 402 "../lib/Chai.chpl"
  _ir_F0_this_chpl = *(deltas_chpl);
#line 402 "../lib/Chai.chpl"
  _ir_F0_this_chpl2 = *(inputs_chpl);
#line 402 "../lib/Chai.chpl"
  coerce_tmp_chpl4 = *(call_tmp_chpl6);
#line 402 "../lib/Chai.chpl"
  _ic__F0_this_chpl = coerce_tmp_chpl4;
#line 402 "../lib/Chai.chpl"
  tmp_chpl = _ir_F0_this_chpl;
#line 402 "../lib/Chai.chpl"
  ret_chpl = (&tmp_chpl)->_instance;
#line 402 "../lib/Chai.chpl"
  _ic__F0_this_chpl2 = ret_chpl;
#line 402 "../lib/Chai.chpl"
  this_chpl32 = ret_chpl;
#line 402 "../lib/Chai.chpl"
  chpl_check_nil(this_chpl32, INT64(402), INT32(65));
#line 402 "../lib/Chai.chpl"
  coerce_tmp_chpl5 = (this_chpl32)->dom;
#line 402 "../lib/Chai.chpl"
  createTuple_chpl(INT64(0), &ret_tmp_chpl3);
#line 402 "../lib/Chai.chpl"
  init_chpl113(&elt_x0_chpl);
#line 402 "../lib/Chai.chpl"
  tup_x0_chpl = elt_x0_chpl;
#line 402 "../lib/Chai.chpl"
  _ic__F5_block_x0_chpl = tup_x0_chpl;
#line 402 "../lib/Chai.chpl"
  coerce_tmp_chpl6 = *(chpl_followThis + INT64(0));
#line 402 "../lib/Chai.chpl"
  coerce_tmp_chpl12 = (&coerce_tmp_chpl6)->_high;
#line 402 "../lib/Chai.chpl"
  this_chpl33 = coerce_tmp_chpl5;
#line 402 "../lib/Chai.chpl"
  chpl_check_nil(this_chpl33, INT64(402), INT32(65));
#line 402 "../lib/Chai.chpl"
  call_tmp_chpl17 = &((this_chpl33)->ranges);
#line 402 "../lib/Chai.chpl"
  coerce_tmp_chpl7 = *(*(call_tmp_chpl17) + INT64(0));
#line 402 "../lib/Chai.chpl"
  call_tmp_chpl15 = sizeAs_chpl36(coerce_tmp_chpl7, INT64(402), INT32(65));
#line 402 "../lib/Chai.chpl"
  if (! (coerce_tmp_chpl12 < INT64(0))) /* ZLINE: 402 ../lib/Chai.chpl */
#line 402 "../lib/Chai.chpl"
  {
#line 402 "../lib/Chai.chpl"
    tmp_chpl2 = (((uint64_t)(coerce_tmp_chpl12)) >= call_tmp_chpl15);
#line 402 "../lib/Chai.chpl"
  } else /* ZLINE: 402 ../lib/Chai.chpl */
#line 402 "../lib/Chai.chpl"
  {
#line 402 "../lib/Chai.chpl"
    tmp_chpl2 = UINT8(false);
#line 402 "../lib/Chai.chpl"
  }
#line 402 "../lib/Chai.chpl"
  if (tmp_chpl2) /* ZLINE: 402 ../lib/Chai.chpl */
#line 402 "../lib/Chai.chpl"
  {
#line 402 "../lib/Chai.chpl"
    boundsCheckHalt_chpl(&_str_literal_4152_chpl, INT64(402), INT32(65));
#line 402 "../lib/Chai.chpl"
  }
#line 402 "../lib/Chai.chpl"
  this_chpl34 = coerce_tmp_chpl5;
#line 402 "../lib/Chai.chpl"
  chpl_check_nil(this_chpl34, INT64(402), INT32(65));
#line 402 "../lib/Chai.chpl"
  call_tmp_chpl18 = &((this_chpl34)->ranges);
#line 402 "../lib/Chai.chpl"
  coerce_tmp_chpl8 = *(*(call_tmp_chpl18) + INT64(0));
#line 402 "../lib/Chai.chpl"
  coerce_tmp_chpl9 = *(chpl_followThis + INT64(0));
#line 402 "../lib/Chai.chpl"
  call_tmp_chpl19 = ((&coerce_tmp_chpl8)->_low + ((int64_t)(((&coerce_tmp_chpl9)->_low * INT64(1)))));
#line 402 "../lib/Chai.chpl"
  this_chpl35 = coerce_tmp_chpl5;
#line 402 "../lib/Chai.chpl"
  chpl_check_nil(this_chpl35, INT64(402), INT32(65));
#line 402 "../lib/Chai.chpl"
  call_tmp_chpl20 = &((this_chpl35)->ranges);
#line 402 "../lib/Chai.chpl"
  coerce_tmp_chpl10 = *(*(call_tmp_chpl20) + INT64(0));
#line 402 "../lib/Chai.chpl"
  coerce_tmp_chpl11 = *(chpl_followThis + INT64(0));
#line 402 "../lib/Chai.chpl"
  call_tmp_chpl21 = ((&coerce_tmp_chpl10)->_low + ((int64_t)(((&coerce_tmp_chpl11)->_high * INT64(1)))));
#line 402 "../lib/Chai.chpl"
  call_tmp_chpl16 = &_ic__F5_block_x0_chpl;
#line 402 "../lib/Chai.chpl"
  compilerAssert_chpl();
#line 402 "../lib/Chai.chpl"
  (call_tmp_chpl16)->_low = call_tmp_chpl19;
#line 402 "../lib/Chai.chpl"
  (call_tmp_chpl16)->_high = call_tmp_chpl21;
#line 402 "../lib/Chai.chpl"
  *(retcopy_chpl + INT64(0)) = _ic__F5_block_x0_chpl;
#line 402 "../lib/Chai.chpl"
  _ic__F7_coerce_tmp_x0_chpl = *(retcopy_chpl + INT64(0));
#line 402 "../lib/Chai.chpl"
  _ic__F0_this_chpl3 = _ic__F7_coerce_tmp_x0_chpl;
#line 402 "../lib/Chai.chpl"
  checkIfIterWillOverflow_chpl(_ic__F7_coerce_tmp_x0_chpl, UINT8(true), INT64(402), INT32(65));
#line 402 "../lib/Chai.chpl"
  _ic__F8_i_chpl = INT64(0);
#line 402 "../lib/Chai.chpl"
  this_chpl36 = _ic__F0_this_chpl3;
#line 402 "../lib/Chai.chpl"
  coerce_tmp_chpl13 = (&this_chpl36)->_low;
#line 402 "../lib/Chai.chpl"
  this_chpl37 = _ic__F0_this_chpl3;
#line 402 "../lib/Chai.chpl"
  _ic__F9_end_chpl = (&this_chpl37)->_high;
#line 402 "../lib/Chai.chpl"
  tmp_chpl3 = _ir_F0_this_chpl2;
#line 402 "../lib/Chai.chpl"
  ret_chpl2 = (&tmp_chpl3)->_instance;
#line 402 "../lib/Chai.chpl"
  _ic__F0_this_chpl4 = ret_chpl2;
#line 402 "../lib/Chai.chpl"
  this_chpl38 = ret_chpl2;
#line 402 "../lib/Chai.chpl"
  chpl_check_nil(this_chpl38, INT64(402), INT32(65));
#line 402 "../lib/Chai.chpl"
  coerce_tmp_chpl14 = (this_chpl38)->dom;
#line 402 "../lib/Chai.chpl"
  createTuple_chpl(INT64(0), &ret_tmp_chpl4);
#line 402 "../lib/Chai.chpl"
  init_chpl113(&elt_x0_chpl2);
#line 402 "../lib/Chai.chpl"
  tup_x0_chpl2 = elt_x0_chpl2;
#line 402 "../lib/Chai.chpl"
  _ic__F5_block_x0_chpl2 = tup_x0_chpl2;
#line 402 "../lib/Chai.chpl"
  coerce_tmp_chpl15 = *(chpl_followThis + INT64(0));
#line 402 "../lib/Chai.chpl"
  coerce_tmp_chpl21 = (&coerce_tmp_chpl15)->_high;
#line 402 "../lib/Chai.chpl"
  this_chpl39 = coerce_tmp_chpl14;
#line 402 "../lib/Chai.chpl"
  chpl_check_nil(this_chpl39, INT64(402), INT32(65));
#line 402 "../lib/Chai.chpl"
  call_tmp_chpl24 = &((this_chpl39)->ranges);
#line 402 "../lib/Chai.chpl"
  coerce_tmp_chpl16 = *(*(call_tmp_chpl24) + INT64(0));
#line 402 "../lib/Chai.chpl"
  call_tmp_chpl22 = sizeAs_chpl36(coerce_tmp_chpl16, INT64(402), INT32(65));
#line 402 "../lib/Chai.chpl"
  if (! (coerce_tmp_chpl21 < INT64(0))) /* ZLINE: 402 ../lib/Chai.chpl */
#line 402 "../lib/Chai.chpl"
  {
#line 402 "../lib/Chai.chpl"
    tmp_chpl4 = (((uint64_t)(coerce_tmp_chpl21)) >= call_tmp_chpl22);
#line 402 "../lib/Chai.chpl"
  } else /* ZLINE: 402 ../lib/Chai.chpl */
#line 402 "../lib/Chai.chpl"
  {
#line 402 "../lib/Chai.chpl"
    tmp_chpl4 = UINT8(false);
#line 402 "../lib/Chai.chpl"
  }
#line 402 "../lib/Chai.chpl"
  if (tmp_chpl4) /* ZLINE: 402 ../lib/Chai.chpl */
#line 402 "../lib/Chai.chpl"
  {
#line 402 "../lib/Chai.chpl"
    boundsCheckHalt_chpl(&_str_literal_4152_chpl, INT64(402), INT32(65));
#line 402 "../lib/Chai.chpl"
  }
#line 402 "../lib/Chai.chpl"
  this_chpl40 = coerce_tmp_chpl14;
#line 402 "../lib/Chai.chpl"
  chpl_check_nil(this_chpl40, INT64(402), INT32(65));
#line 402 "../lib/Chai.chpl"
  call_tmp_chpl25 = &((this_chpl40)->ranges);
#line 402 "../lib/Chai.chpl"
  coerce_tmp_chpl17 = *(*(call_tmp_chpl25) + INT64(0));
#line 402 "../lib/Chai.chpl"
  coerce_tmp_chpl18 = *(chpl_followThis + INT64(0));
#line 402 "../lib/Chai.chpl"
  call_tmp_chpl26 = ((&coerce_tmp_chpl17)->_low + ((int64_t)(((&coerce_tmp_chpl18)->_low * INT64(1)))));
#line 402 "../lib/Chai.chpl"
  this_chpl41 = coerce_tmp_chpl14;
#line 402 "../lib/Chai.chpl"
  chpl_check_nil(this_chpl41, INT64(402), INT32(65));
#line 402 "../lib/Chai.chpl"
  call_tmp_chpl27 = &((this_chpl41)->ranges);
#line 402 "../lib/Chai.chpl"
  coerce_tmp_chpl19 = *(*(call_tmp_chpl27) + INT64(0));
#line 402 "../lib/Chai.chpl"
  coerce_tmp_chpl20 = *(chpl_followThis + INT64(0));
#line 402 "../lib/Chai.chpl"
  call_tmp_chpl28 = ((&coerce_tmp_chpl19)->_low + ((int64_t)(((&coerce_tmp_chpl20)->_high * INT64(1)))));
#line 402 "../lib/Chai.chpl"
  call_tmp_chpl23 = &_ic__F5_block_x0_chpl2;
#line 402 "../lib/Chai.chpl"
  compilerAssert_chpl();
#line 402 "../lib/Chai.chpl"
  (call_tmp_chpl23)->_low = call_tmp_chpl26;
#line 402 "../lib/Chai.chpl"
  (call_tmp_chpl23)->_high = call_tmp_chpl28;
#line 402 "../lib/Chai.chpl"
  *(retcopy_chpl2 + INT64(0)) = _ic__F5_block_x0_chpl2;
#line 402 "../lib/Chai.chpl"
  _ic__F7_coerce_tmp_x0_chpl2 = *(retcopy_chpl2 + INT64(0));
#line 402 "../lib/Chai.chpl"
  _ic__F0_this_chpl5 = _ic__F7_coerce_tmp_x0_chpl2;
#line 402 "../lib/Chai.chpl"
  checkIfIterWillOverflow_chpl(_ic__F7_coerce_tmp_x0_chpl2, UINT8(true), INT64(402), INT32(65));
#line 402 "../lib/Chai.chpl"
  _ic__F8_i_chpl2 = INT64(0);
#line 402 "../lib/Chai.chpl"
  this_chpl42 = _ic__F0_this_chpl5;
#line 402 "../lib/Chai.chpl"
  coerce_tmp_chpl22 = (&this_chpl42)->_low;
#line 402 "../lib/Chai.chpl"
  this_chpl43 = _ic__F0_this_chpl5;
#line 402 "../lib/Chai.chpl"
  _ic__F9_end_chpl2 = (&this_chpl43)->_high;
#line 402 "../lib/Chai.chpl"
  tmp_chpl5 = *(chpl_followThis + INT64(0));
#line 402 "../lib/Chai.chpl"
  _ic__F2_myFollowThis_chpl = tmp_chpl5;
#line 402 "../lib/Chai.chpl"
  tmp_chpl6 = tmp_chpl5;
#line 402 "../lib/Chai.chpl"
  if ((&tmp_chpl6)->_low > (&tmp_chpl6)->_high) /* ZLINE: 402 ../lib/Chai.chpl */
#line 402 "../lib/Chai.chpl"
  {
#line 402 "../lib/Chai.chpl"
    tmp_chpl8 = UINT8(false);
#line 402 "../lib/Chai.chpl"
  } else /* ZLINE: 402 ../lib/Chai.chpl */
#line 402 "../lib/Chai.chpl"
  {
#line 402 "../lib/Chai.chpl"
    tmp_chpl8 = UINT8(true);
#line 402 "../lib/Chai.chpl"
  }
#line 402 "../lib/Chai.chpl"
  if (! tmp_chpl8) /* ZLINE: 402 ../lib/Chai.chpl */
#line 402 "../lib/Chai.chpl"
  {
#line 402 "../lib/Chai.chpl"
    tmp_chpl9 = tmp_chpl5;
#line 402 "../lib/Chai.chpl"
    if (! ((&tmp_chpl9)->_low > (&tmp_chpl9)->_high)) /* ZLINE: 402 ../lib/Chai.chpl */
#line 402 "../lib/Chai.chpl"
    {
#line 402 "../lib/Chai.chpl"
      boundsCheckHalt_chpl(&_str_literal_633_chpl, INT64(402), INT32(65));
#line 402 "../lib/Chai.chpl"
    }
#line 402 "../lib/Chai.chpl"
  }
#line 402 "../lib/Chai.chpl"
  call_tmp_chpl29 = sizeAs_chpl37(tmp_chpl5, INT64(402), INT32(65));
#line 402 "../lib/Chai.chpl"
  if (call_tmp_chpl29 != INT64(0)) /* ZLINE: 402 ../lib/Chai.chpl */
#line 402 "../lib/Chai.chpl"
  {
#line 402 "../lib/Chai.chpl"
  }
#line 402 "../lib/Chai.chpl"
  tmp_chpl7 = &_ic__F3_r_chpl;
#line 402 "../lib/Chai.chpl"
  init_chpl113(tmp_chpl7);
#line 402 "../lib/Chai.chpl"
  if (call_tmp_chpl29 != INT64(0)) /* ZLINE: 402 ../lib/Chai.chpl */
#line 402 "../lib/Chai.chpl"
  {
#line 402 "../lib/Chai.chpl"
    tmp_chpl10 = _ic__F2_myFollowThis_chpl;
#line 402 "../lib/Chai.chpl"
    call_tmp_chpl30 = orderToIndex_chpl(coerce_tmp_chpl4, (&tmp_chpl10)->_low, INT64(402), INT32(65));
#line 402 "../lib/Chai.chpl"
    tmp_chpl11 = _ic__F2_myFollowThis_chpl;
#line 402 "../lib/Chai.chpl"
    call_tmp_chpl31 = orderToIndex_chpl(_ic__F0_this_chpl, (&tmp_chpl11)->_high, INT64(402), INT32(65));
#line 402 "../lib/Chai.chpl"
    tmp_chpl12 = &_ic__F3_r_chpl;
#line 402 "../lib/Chai.chpl"
    (tmp_chpl12)->_low = call_tmp_chpl30;
#line 402 "../lib/Chai.chpl"
    (tmp_chpl12)->_high = call_tmp_chpl31;
#line 402 "../lib/Chai.chpl"
    verifyAppropriateStide_chpl2(INT64(1), INT64(402), INT32(65));
#line 402 "../lib/Chai.chpl"
  }
#line 402 "../lib/Chai.chpl"
  coerce_tmp_chpl23 = _ic__F3_r_chpl;
#line 402 "../lib/Chai.chpl"
  _ic__F0_this_chpl6 = coerce_tmp_chpl23;
#line 402 "../lib/Chai.chpl"
  checkIfIterWillOverflow_chpl(coerce_tmp_chpl23, UINT8(true), INT64(402), INT32(65));
#line 402 "../lib/Chai.chpl"
  _ic__F5_i_chpl = INT64(0);
#line 402 "../lib/Chai.chpl"
  this_chpl44 = _ic__F0_this_chpl6;
#line 402 "../lib/Chai.chpl"
  for (_ic__F8_i_chpl = coerce_tmp_chpl13,_ic__F8_i_chpl2 = coerce_tmp_chpl22,_ic__F5_i_chpl = (&this_chpl44)->_low; (tmp_chpl14 = (_ic__F8_i_chpl <= _ic__F9_end_chpl),tmp_chpl14); tmp_chpl15 = _ic__F8_i_chpl,tmp_chpl15 += INT64(1),_ic__F8_i_chpl = tmp_chpl15,tmp_chpl16 = _ic__F8_i_chpl2,tmp_chpl16 += INT64(1),_ic__F8_i_chpl2 = tmp_chpl16,tmp_chpl17 = _ic__F5_i_chpl,tmp_chpl17 += INT64(1),_ic__F5_i_chpl = tmp_chpl17) {
#line 402 "../lib/Chai.chpl"
    this_chpl45 = _ic__F0_this_chpl4;
#line 402 "../lib/Chai.chpl"
    chpl_check_nil(this_chpl45, INT64(402), INT32(65));
#line 402 "../lib/Chai.chpl"
    coerce_tmp_chpl24 = (this_chpl45)->shiftedData;
#line 402 "../lib/Chai.chpl"
    call_tmp_chpl32 = (coerce_tmp_chpl24 + _ic__F8_i_chpl2);
#line 402 "../lib/Chai.chpl"
    tmp_chpl13 = (_ic__F8_i_chpl2 <= _ic__F9_end_chpl2);
#line 402 "../lib/Chai.chpl"
    hasMore_chpl = (tmp_chpl13 != INT64(0));
#line 402 "../lib/Chai.chpl"
    if (! hasMore_chpl) /* ZLINE: 402 ../lib/Chai.chpl */
#line 402 "../lib/Chai.chpl"
    {
#line 402 "../lib/Chai.chpl"
      chpl_error("zippered iterations have non-equal lengths", INT64(402), INT32(65));
#line 402 "../lib/Chai.chpl"
    }
#line 402 "../lib/Chai.chpl"
    this_chpl46 = _ic__F0_this_chpl2;
#line 402 "../lib/Chai.chpl"
    chpl_check_nil(this_chpl46, INT64(402), INT32(65));
#line 402 "../lib/Chai.chpl"
    coerce_tmp_chpl25 = (this_chpl46)->shiftedData;
#line 402 "../lib/Chai.chpl"
    call_tmp_chpl33 = (coerce_tmp_chpl25 + _ic__F8_i_chpl);
#line 403 "../lib/Chai.chpl"
    *(coerce_tmp_chpl26 + INT64(0)) = _ic__F5_i_chpl;
#line 403 "../lib/Chai.chpl"
    i_i_chpl = &coerce_tmp_chpl26;
#line 403 "../lib/Chai.chpl"
    ret_chpl3 = (newDeltas_chpl)->_instance;
#line 403 "../lib/Chai.chpl"
    checkAccess_chpl10(newDeltas_chpl, i_i_chpl, ret_chpl3, INT64(403), INT32(65));
#line 403 "../lib/Chai.chpl"
    coerce_tmp_chpl27 = ret_chpl3;
#line 403 "../lib/Chai.chpl"
    *(_formal_tmp_in_ind_chpl + INT64(0)) = *(coerce_tmp_chpl26 + INT64(0));
#line 403 "../lib/Chai.chpl"
    _formal_tmp_ind_x0_chpl = *(_formal_tmp_in_ind_chpl + INT64(0));
#line 403 "../lib/Chai.chpl"
    chpl_check_nil(coerce_tmp_chpl27, INT64(403), INT32(65));
#line 403 "../lib/Chai.chpl"
    coerce_tmp_chpl28 = (coerce_tmp_chpl27)->shiftedData;
#line 403 "../lib/Chai.chpl"
    call_tmp_chpl34 = (coerce_tmp_chpl28 + _formal_tmp_ind_x0_chpl);
#line 403 "../lib/Chai.chpl"
    backward_chpl(this_chpl31, call_tmp_chpl33, call_tmp_chpl32, &ret_tmp_chpl5);
#line 403 "../lib/Chai.chpl"
    call_tmp_chpl35 = ret_tmp_chpl5;
#line 403 "../lib/Chai.chpl"
    chpl___ASSIGN_24(call_tmp_chpl34, &call_tmp_chpl35);
#line 403 "../lib/Chai.chpl"
    _field_destructor_tmp__chpl = (&call_tmp_chpl35)->data;
#line 403 "../lib/Chai.chpl"
    chpl__autoDestroy28(&_field_destructor_tmp__chpl, INT64(403), INT32(65));
#line 403 "../lib/Chai.chpl"
    _field_destructor_tmp__chpl2 = (&call_tmp_chpl35)->_domain;
#line 403 "../lib/Chai.chpl"
    chpl__autoDestroy8(&_field_destructor_tmp__chpl2, INT64(403), INT32(65));
#line 403 "../lib/Chai.chpl"
  }
#line 402 "../lib/Chai.chpl"
  tmp_chpl18 = (_ic__F8_i_chpl2 <= _ic__F9_end_chpl2);
#line 402 "../lib/Chai.chpl"
  hasMore_chpl = (tmp_chpl18 != INT64(0));
#line 402 "../lib/Chai.chpl"
  if (hasMore_chpl) /* ZLINE: 402 ../lib/Chai.chpl */
#line 402 "../lib/Chai.chpl"
  {
#line 402 "../lib/Chai.chpl"
    chpl_error("zippered iterations have non-equal lengths", INT64(402), INT32(65));
#line 402 "../lib/Chai.chpl"
  }
#line 402 "../lib/Chai.chpl"
  return;
#line 402 "../lib/Chai.chpl"
}

#line 408 "../lib/Chai.chpl"
/* Chai.chpl:408 */
#line 408 "../lib/Chai.chpl"
static void optimize_chpl2(MaxPool_chpl * this_chpl31,
#line 408 "../lib/Chai.chpl"
                           _real64 mag_chpl) {
#line 408 "../lib/Chai.chpl"
  return;
#line 408 "../lib/Chai.chpl"
}

#line 409 "../lib/Chai.chpl"
/* Chai.chpl:409 */
#line 409 "../lib/Chai.chpl"
static void resetGradients_chpl2(MaxPool_chpl * this_chpl31) {
#line 409 "../lib/Chai.chpl"
  return;
#line 409 "../lib/Chai.chpl"
}

#line 411 "../lib/Chai.chpl"
/* Chai.chpl:411 */
#line 411 "../lib/Chai.chpl"
static void write_chpl3(MaxPool_chpl * this_chpl31,
#line 411 "../lib/Chai.chpl"
                        fileWriter_T_BinarySerializer_chpl * fw_chpl,
#line 411 "../lib/Chai.chpl"
                        Error_chpl * error_out_chpl) {
#line 411 "../lib/Chai.chpl"
  return;
#line 411 "../lib/Chai.chpl"
}

#line 416 "../lib/Chai.chpl"
/* Chai.chpl:416 */
#line 416 "../lib/Chai.chpl"
static void signature_chpl2(MaxPool_chpl * this_chpl31,
#line 416 "../lib/Chai.chpl"
                            string_chpl * _retArg_chpl) {
#line 417 "../lib/Chai.chpl"
  string_chpl inlineImm_chpl;
#line 417 "../lib/Chai.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 417 "../lib/Chai.chpl"
  string_chpl ret_chpl;
#line 417 "../lib/Chai.chpl"
  string_chpl tmp_chpl;
#line 417 "../lib/Chai.chpl"
  inlineImm_chpl = _str_literal_1710_chpl;
#line 417 "../lib/Chai.chpl"
  i_x_chpl = &inlineImm_chpl;
#line 417 "../lib/Chai.chpl"
  init_ASSIGN__chpl25(&tmp_chpl, i_x_chpl, INT64(417), INT32(65));
#line 417 "../lib/Chai.chpl"
  ret_chpl = tmp_chpl;
#line 416 "../lib/Chai.chpl"
  *(_retArg_chpl) = ret_chpl;
#line 416 "../lib/Chai.chpl"
  return;
#line 416 "../lib/Chai.chpl"
}

#line 486 "../lib/Chai.chpl"
/* Chai.chpl:486 */
#line 486 "../lib/Chai.chpl"
static void init_ASSIGN__chpl28(SoftMax_chpl * this_chpl31,
#line 486 "../lib/Chai.chpl"
                                SoftMax_chpl * other_chpl) {
#line 486 "../lib/Chai.chpl"
  _ref_Tensor_2__real64_chpl call_tmp_chpl6 = NULL;
#line 486 "../lib/Chai.chpl"
  Tensor_2__real64_chpl weights_chpl;
#line 486 "../lib/Chai.chpl"
  _ref_Tensor_1__real64_chpl call_tmp_chpl7 = NULL;
#line 486 "../lib/Chai.chpl"
  Tensor_1__real64_chpl biases_chpl;
#line 486 "../lib/Chai.chpl"
  _ref_Tensor_2__real64_chpl call_tmp_chpl8 = NULL;
#line 486 "../lib/Chai.chpl"
  Tensor_2__real64_chpl weightsGrad_chpl;
#line 486 "../lib/Chai.chpl"
  _ref_Tensor_1__real64_chpl call_tmp_chpl9 = NULL;
#line 486 "../lib/Chai.chpl"
  Tensor_1__real64_chpl biasesGrad_chpl;
#line 486 "../lib/Chai.chpl"
  call_tmp_chpl6 = &((other_chpl)->weights);
#line 486 "../lib/Chai.chpl"
  init_ASSIGN__chpl31(&weights_chpl, call_tmp_chpl6);
#line 486 "../lib/Chai.chpl"
  (this_chpl31)->weights = weights_chpl;
#line 486 "../lib/Chai.chpl"
  call_tmp_chpl7 = &((other_chpl)->biases);
#line 486 "../lib/Chai.chpl"
  init_ASSIGN__chpl32(&biases_chpl, call_tmp_chpl7);
#line 486 "../lib/Chai.chpl"
  (this_chpl31)->biases = biases_chpl;
#line 486 "../lib/Chai.chpl"
  call_tmp_chpl8 = &((other_chpl)->weightsGrad);
#line 486 "../lib/Chai.chpl"
  init_ASSIGN__chpl31(&weightsGrad_chpl, call_tmp_chpl8);
#line 486 "../lib/Chai.chpl"
  (this_chpl31)->weightsGrad = weightsGrad_chpl;
#line 486 "../lib/Chai.chpl"
  call_tmp_chpl9 = &((other_chpl)->biasesGrad);
#line 486 "../lib/Chai.chpl"
  init_ASSIGN__chpl32(&biasesGrad_chpl, call_tmp_chpl9);
#line 486 "../lib/Chai.chpl"
  (this_chpl31)->biasesGrad = biasesGrad_chpl;
#line 486 "../lib/Chai.chpl"
  (this_chpl31)->uninitialized = (other_chpl)->uninitialized;
#line 486 "../lib/Chai.chpl"
  (this_chpl31)->outputSize = (other_chpl)->outputSize;
#line 486 "../lib/Chai.chpl"
  return;
#line 486 "../lib/Chai.chpl"
}

#line 506 "../lib/Chai.chpl"
/* Chai.chpl:506 */
#line 506 "../lib/Chai.chpl"
static void init_chpl231(SoftMax_chpl * this_chpl31,
#line 506 "../lib/Chai.chpl"
                         int64_t outputSize_chpl) {
#line 507 "../lib/Chai.chpl"
  Tensor_2__real64_chpl weights_chpl;
#line 507 "../lib/Chai.chpl"
  Tensor_1__real64_chpl biases_chpl;
#line 507 "../lib/Chai.chpl"
  Tensor_2__real64_chpl weightsGrad_chpl;
#line 507 "../lib/Chai.chpl"
  Tensor_1__real64_chpl biasesGrad_chpl;
#line 507 "../lib/Chai.chpl"
  init_chpl236(&weights_chpl);
#line 507 "../lib/Chai.chpl"
  (this_chpl31)->weights = weights_chpl;
#line 507 "../lib/Chai.chpl"
  init_chpl235(&biases_chpl);
#line 507 "../lib/Chai.chpl"
  (this_chpl31)->biases = biases_chpl;
#line 507 "../lib/Chai.chpl"
  init_chpl236(&weightsGrad_chpl);
#line 507 "../lib/Chai.chpl"
  (this_chpl31)->weightsGrad = weightsGrad_chpl;
#line 507 "../lib/Chai.chpl"
  init_chpl235(&biasesGrad_chpl);
#line 507 "../lib/Chai.chpl"
  (this_chpl31)->biasesGrad = biasesGrad_chpl;
#line 507 "../lib/Chai.chpl"
  (this_chpl31)->uninitialized = UINT8(true);
#line 507 "../lib/Chai.chpl"
  (this_chpl31)->outputSize = outputSize_chpl;
#line 506 "../lib/Chai.chpl"
  return;
#line 506 "../lib/Chai.chpl"
}

#line 510 "../lib/Chai.chpl"
/* Chai.chpl:510 */
#line 510 "../lib/Chai.chpl"
static void initialize_chpl(SoftMax_chpl * this_chpl31,
#line 510 "../lib/Chai.chpl"
                            Tensor_3__real64_chpl * input_chpl) {
#line 511 "../lib/Chai.chpl"
  int64_t local_dataParMinGranularity_chpl;
#line 511 "../lib/Chai.chpl"
  chpl_bool local_dataParIgnoreRunningTasks_chpl;
#line 511 "../lib/Chai.chpl"
  int64_t local_dataParTasksPerLocale_chpl;
#line 511 "../lib/Chai.chpl"
  _tuple_3_star_int64_t_chpl call_tmp_chpl6;
#line 511 "../lib/Chai.chpl"
  _tuple_3_star_int64_t_chpl ret_tmp_chpl;
#line 511 "../lib/Chai.chpl"
  _tuple_3_star_int64_t_chpl chpl__iterLF;
#line 511 "../lib/Chai.chpl"
  ProductReduceScanOp_int64_t_chpl globalRP_call_tmp_chpl = NULL;
#line 511 "../lib/Chai.chpl"
  ProductReduceScanOp_int64_t_chpl new_temp_chpl = NULL;
#line 511 "../lib/Chai.chpl"
  chpl_LocalSpinlock default_arg_l_chpl;
#line 511 "../lib/Chai.chpl"
  chpl_LocalSpinlock ret_chpl;
#line 511 "../lib/Chai.chpl"
  chpl_LocalSpinlock l_chpl;
#line 511 "../lib/Chai.chpl"
  int64_t call_tmp_chpl7;
#line 511 "../lib/Chai.chpl"
  ProductReduceScanOp_int64_t_chpl coerce_tmp_chpl3 = NULL;
#line 511 "../lib/Chai.chpl"
  int64_t globalAS_chpl_redSVar_chpl;
#line 511 "../lib/Chai.chpl"
  int64_t tmp_chpl;
#line 511 "../lib/Chai.chpl"
  locale_chpl call_tmp_chpl8;
#line 511 "../lib/Chai.chpl"
  chpl_localeID_t call_tmp_chpl9;
#line 511 "../lib/Chai.chpl"
  int32_t call_tmp_chpl10;
#line 511 "../lib/Chai.chpl"
  chpl_localeID_t call_tmp_chpl11;
#line 511 "../lib/Chai.chpl"
  locale_chpl call_tmp_chpl12;
#line 511 "../lib/Chai.chpl"
  locale_chpl ret_tmp_chpl2;
#line 511 "../lib/Chai.chpl"
  int64_t ret_chpl2;
#line 511 "../lib/Chai.chpl"
  BaseLocale_chpl coerce_tmp_chpl4 = NULL;
#line 511 "../lib/Chai.chpl"
  BaseLocale_chpl coerce_tmp_chpl5 = NULL;
#line 511 "../lib/Chai.chpl"
  _ref_locale_chpl i_x_chpl = NULL;
#line 511 "../lib/Chai.chpl"
  chpl_bool ignoreRunning_chpl;
#line 511 "../lib/Chai.chpl"
  int64_t minIndicesPerTask_chpl;
#line 511 "../lib/Chai.chpl"
  int64_t length_chpl;
#line 511 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl call_tmp_chpl13;
#line 511 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl ret_tmp_chpl3;
#line 511 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl14;
#line 511 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl4;
#line 511 "../lib/Chai.chpl"
  _tuple_1_star_range_int64_t_both_one_chpl ret_tmp_chpl5;
#line 511 "../lib/Chai.chpl"
  _tuple_2_star_int64_t_chpl call_tmp_chpl15;
#line 511 "../lib/Chai.chpl"
  _tuple_1_star_range_int64_t_both_one_chpl coerce_tmp_chpl6;
#line 511 "../lib/Chai.chpl"
  _tuple_2_star_int64_t_chpl ret_tmp_chpl6;
#line 511 "../lib/Chai.chpl"
  int64_t tmp_x0_chpl;
#line 511 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl tmpIter_chpl;
#line 511 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl call_tmp_chpl16;
#line 511 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl ret_tmp_chpl7;
#line 511 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl8;
#line 511 "../lib/Chai.chpl"
  chpl___EndCount_AtomicT_int64_t_int64_t _coforallCount_chpl = NULL;
#line 511 "../lib/Chai.chpl"
  chpl___EndCount_AtomicT_int64_t_int64_t new_temp_chpl2 = NULL;
#line 511 "../lib/Chai.chpl"
  int64_t numTasks_chpl;
#line 511 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl _ic__F0_this_chpl;
#line 511 "../lib/Chai.chpl"
  int64_t numChunks_captemp_chpl;
#line 511 "../lib/Chai.chpl"
  int64_t i_chpl;
#line 511 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl this_chpl32;
#line 511 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl this_chpl33;
#line 511 "../lib/Chai.chpl"
  int64_t end_chpl;
#line 511 "../lib/Chai.chpl"
  _class_localscoforall_fn_chpl110 _args_forcoforall_fn_chpl = NULL;
#line 511 "../lib/Chai.chpl"
  _ref__tuple_3_star_int64_t_chpl tmp_chpl2 = NULL;
#line 511 "../lib/Chai.chpl"
  raw_c_void_ptr _args_vforcoforall_fn_chpl;
#line 511 "../lib/Chai.chpl"
  Error_chpl error_chpl = NULL;
#line 511 "../lib/Chai.chpl"
  ProductReduceScanOp_int64_t_chpl coerce_tmp_chpl7 = NULL;
#line 511 "../lib/Chai.chpl"
  int64_t call_tmp_chpl17;
#line 511 "../lib/Chai.chpl"
  ProductReduceScanOp_int64_t_chpl coerce_tmp_chpl8 = NULL;
#line 514 "../lib/Chai.chpl"
  _real64 call_tmp_chpl18;
#line 516 "../lib/Chai.chpl"
  _ref_Tensor_2__real64_chpl call_tmp_chpl19 = NULL;
#line 516 "../lib/Chai.chpl"
  Tensor_2__real64_chpl call_tmp_chpl20;
#line 516 "../lib/Chai.chpl"
  Tensor_2__real64_chpl ret_tmp_chpl9;
#line 516 "../lib/Chai.chpl"
  Tensor_2__real64_chpl call_tmp_chpl21;
#line 516 "../lib/Chai.chpl"
  Tensor_2__real64_chpl ret_tmp_chpl10;
#line 516 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_2_int64_t_one__real64_int64_t_chpl _field_destructor_tmp__chpl;
#line 516 "../lib/Chai.chpl"
  _domain_DefaultRectangularDom_2_int64_t_one_chpl _field_destructor_tmp__chpl2;
#line 516 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_2_int64_t_one__real64_int64_t_chpl _field_destructor_tmp__chpl3;
#line 516 "../lib/Chai.chpl"
  _domain_DefaultRectangularDom_2_int64_t_one_chpl _field_destructor_tmp__chpl4;
#line 517 "../lib/Chai.chpl"
  _ref_Tensor_1__real64_chpl call_tmp_chpl22 = NULL;
#line 517 "../lib/Chai.chpl"
  Tensor_1__real64_chpl call_tmp_chpl23;
#line 517 "../lib/Chai.chpl"
  Tensor_1__real64_chpl ret_tmp_chpl11;
#line 517 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_1_int64_t_one__real64_int64_t_chpl _field_destructor_tmp__chpl5;
#line 517 "../lib/Chai.chpl"
  _domain_DefaultRectangularDom_1_int64_t_one_chpl _field_destructor_tmp__chpl6;
#line 519 "../lib/Chai.chpl"
  _ref_Tensor_2__real64_chpl call_tmp_chpl24 = NULL;
#line 519 "../lib/Chai.chpl"
  Tensor_2__real64_chpl call_tmp_chpl25;
#line 519 "../lib/Chai.chpl"
  Tensor_2__real64_chpl ret_tmp_chpl12;
#line 519 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_2_int64_t_one__real64_int64_t_chpl _field_destructor_tmp__chpl7;
#line 519 "../lib/Chai.chpl"
  _domain_DefaultRectangularDom_2_int64_t_one_chpl _field_destructor_tmp__chpl8;
#line 520 "../lib/Chai.chpl"
  _ref_Tensor_1__real64_chpl call_tmp_chpl26 = NULL;
#line 520 "../lib/Chai.chpl"
  Tensor_1__real64_chpl call_tmp_chpl27;
#line 520 "../lib/Chai.chpl"
  Tensor_1__real64_chpl ret_tmp_chpl13;
#line 520 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_1_int64_t_one__real64_int64_t_chpl _field_destructor_tmp__chpl9;
#line 520 "../lib/Chai.chpl"
  _domain_DefaultRectangularDom_1_int64_t_one_chpl _field_destructor_tmp__chpl10;
#line 520 "../lib/Chai.chpl"
  chpl__class_localscoforall_fn_chpl110_object chpl_macro_tmp_115;
#line 511 "../lib/Chai.chpl"
  local_dataParMinGranularity_chpl = dataParMinGranularity_chpl;
#line 511 "../lib/Chai.chpl"
  local_dataParIgnoreRunningTasks_chpl = dataParIgnoreRunningTasks_chpl;
#line 511 "../lib/Chai.chpl"
  local_dataParTasksPerLocale_chpl = dataParTasksPerLocale_chpl;
#line 511 "../lib/Chai.chpl"
  shape_chpl8(input_chpl, &ret_tmp_chpl);
#line 511 "../lib/Chai.chpl"
  *(call_tmp_chpl6 + INT64(0)) = *(ret_tmp_chpl + INT64(0));
#line 511 "../lib/Chai.chpl"
  *(call_tmp_chpl6 + INT64(1)) = *(ret_tmp_chpl + INT64(1));
#line 511 "../lib/Chai.chpl"
  *(call_tmp_chpl6 + INT64(2)) = *(ret_tmp_chpl + INT64(2));
#line 511 "../lib/Chai.chpl"
  *(chpl__iterLF + INT64(0)) = *(call_tmp_chpl6 + INT64(0));
#line 511 "../lib/Chai.chpl"
  *(chpl__iterLF + INT64(1)) = *(call_tmp_chpl6 + INT64(1));
#line 511 "../lib/Chai.chpl"
  *(chpl__iterLF + INT64(2)) = *(call_tmp_chpl6 + INT64(2));
#line 511 "../lib/Chai.chpl"
  init_chpl105(&l_chpl);
#line 511 "../lib/Chai.chpl"
  ret_chpl = l_chpl;
#line 511 "../lib/Chai.chpl"
  default_arg_l_chpl = ret_chpl;
#line 511 "../lib/Chai.chpl"
  new_temp_chpl = _new_chpl25(&default_arg_l_chpl, INT64(1), INT64(511), INT32(65));
#line 511 "../lib/Chai.chpl"
  globalRP_call_tmp_chpl = new_temp_chpl;
#line 511 "../lib/Chai.chpl"
  coerce_tmp_chpl3 = globalRP_call_tmp_chpl;
#line 511 "../lib/Chai.chpl"
  chpl_check_nil(coerce_tmp_chpl3, INT64(511), INT32(65));
#line 511 "../lib/Chai.chpl"
  call_tmp_chpl7 = identity_chpl(coerce_tmp_chpl3);
#line 511 "../lib/Chai.chpl"
  globalAS_chpl_redSVar_chpl = call_tmp_chpl7;
#line 511 "../lib/Chai.chpl"
  if (local_dataParTasksPerLocale_chpl == INT64(0)) /* ZLINE: 511 ../lib/Chai.chpl */
#line 511 "../lib/Chai.chpl"
  {
#line 511 "../lib/Chai.chpl"
    call_tmp_chpl10 = get_chpl_nodeID();
#line 511 "../lib/Chai.chpl"
    call_tmp_chpl11 = chpl_rt_buildLocaleID(call_tmp_chpl10, c_sublocid_any);
#line 511 "../lib/Chai.chpl"
    call_tmp_chpl9 = call_tmp_chpl11;
#line 511 "../lib/Chai.chpl"
    chpl_localeID_to_locale(&call_tmp_chpl9, &ret_tmp_chpl2, INT64(511), INT32(65));
#line 511 "../lib/Chai.chpl"
    call_tmp_chpl12 = ret_tmp_chpl2;
#line 511 "../lib/Chai.chpl"
    call_tmp_chpl8 = call_tmp_chpl12;
#line 511 "../lib/Chai.chpl"
    coerce_tmp_chpl4 = (&call_tmp_chpl8)->_instance;
#line 511 "../lib/Chai.chpl"
    if (((RootClass_chpl)(coerce_tmp_chpl4)) == nil) /* ZLINE: 511 ../lib/Chai.chpl */
#line 511 "../lib/Chai.chpl"
    {
#line 511 "../lib/Chai.chpl"
      nilCheckHalt_chpl(&_str_literal_52_chpl, INT64(511), INT32(65));
#line 511 "../lib/Chai.chpl"
    }
#line 511 "../lib/Chai.chpl"
    coerce_tmp_chpl5 = coerce_tmp_chpl4;
#line 511 "../lib/Chai.chpl"
    chpl_check_nil(coerce_tmp_chpl5, INT64(511), INT32(65));
#line 511 "../lib/Chai.chpl"
    ret_chpl2 = (coerce_tmp_chpl5)->maxTaskPar;
#line 511 "../lib/Chai.chpl"
    tmp_chpl = ret_chpl2;
#line 511 "../lib/Chai.chpl"
    i_x_chpl = &call_tmp_chpl8;
#line 511 "../lib/Chai.chpl"
    deinit_chpl62(i_x_chpl);
#line 511 "../lib/Chai.chpl"
  } else /* ZLINE: 511 ../lib/Chai.chpl */
#line 511 "../lib/Chai.chpl"
  {
#line 511 "../lib/Chai.chpl"
    tmp_chpl = local_dataParTasksPerLocale_chpl;
#line 511 "../lib/Chai.chpl"
  }
#line 511 "../lib/Chai.chpl"
  ignoreRunning_chpl = local_dataParIgnoreRunningTasks_chpl;
#line 511 "../lib/Chai.chpl"
  minIndicesPerTask_chpl = local_dataParMinGranularity_chpl;
#line 511 "../lib/Chai.chpl"
  length_chpl = INT64(3);
#line 511 "../lib/Chai.chpl"
  chpl_build_low_bounded_range2(INT64(0), &ret_tmp_chpl3);
#line 511 "../lib/Chai.chpl"
  call_tmp_chpl13 = ret_tmp_chpl3;
#line 511 "../lib/Chai.chpl"
  chpl___POUND_3(call_tmp_chpl13, INT64(3), &ret_tmp_chpl4, INT64(511), INT32(65));
#line 511 "../lib/Chai.chpl"
  call_tmp_chpl14 = ret_tmp_chpl4;
#line 511 "../lib/Chai.chpl"
  *(ret_tmp_chpl5 + INT64(0)) = call_tmp_chpl14;
#line 511 "../lib/Chai.chpl"
  *(coerce_tmp_chpl6 + INT64(0)) = *(ret_tmp_chpl5 + INT64(0));
#line 511 "../lib/Chai.chpl"
  _computeChunkStuff_chpl(tmp_chpl, ignoreRunning_chpl, minIndicesPerTask_chpl, &coerce_tmp_chpl6, &ret_tmp_chpl6, INT64(511), INT32(65));
#line 511 "../lib/Chai.chpl"
  *(call_tmp_chpl15 + INT64(0)) = *(ret_tmp_chpl6 + INT64(0));
#line 511 "../lib/Chai.chpl"
  *(call_tmp_chpl15 + INT64(1)) = *(ret_tmp_chpl6 + INT64(1));
#line 511 "../lib/Chai.chpl"
  tmp_x0_chpl = *(call_tmp_chpl15 + INT64(0));
#line 511 "../lib/Chai.chpl"
  chpl_build_low_bounded_range2(INT64(0), &ret_tmp_chpl7);
#line 511 "../lib/Chai.chpl"
  call_tmp_chpl16 = ret_tmp_chpl7;
#line 511 "../lib/Chai.chpl"
  chpl___POUND_3(call_tmp_chpl16, tmp_x0_chpl, &ret_tmp_chpl8, INT64(511), INT32(65));
#line 511 "../lib/Chai.chpl"
  tmpIter_chpl = ret_tmp_chpl8;
#line 511 "../lib/Chai.chpl"
  new_temp_chpl2 = _new_chpl17(INT64(511), INT32(65));
#line 511 "../lib/Chai.chpl"
  _coforallCount_chpl = new_temp_chpl2;
#line 511 "../lib/Chai.chpl"
  numTasks_chpl = chpl_boundedCoforallSize(tmpIter_chpl, INT64(511), INT32(65));
#line 511 "../lib/Chai.chpl"
  _upEndCount_chpl(_coforallCount_chpl, numTasks_chpl, INT64(511), INT32(65));
#line 511 "../lib/Chai.chpl"
  chpl_resetTaskSpawn(numTasks_chpl, INT64(511), INT32(65));
#line 511 "../lib/Chai.chpl"
  _ic__F0_this_chpl = tmpIter_chpl;
#line 511 "../lib/Chai.chpl"
  numChunks_captemp_chpl = tmp_x0_chpl;
#line 511 "../lib/Chai.chpl"
  checkIfIterWillOverflow_chpl(tmpIter_chpl, UINT8(true), INT64(511), INT32(65));
#line 511 "../lib/Chai.chpl"
  i_chpl = INT64(0);
#line 511 "../lib/Chai.chpl"
  this_chpl32 = _ic__F0_this_chpl;
#line 511 "../lib/Chai.chpl"
  this_chpl33 = _ic__F0_this_chpl;
#line 511 "../lib/Chai.chpl"
  end_chpl = (&this_chpl33)->_high;
#line 511 "../lib/Chai.chpl"
  for (i_chpl = (&this_chpl32)->_low; ((i_chpl <= end_chpl)); i_chpl += INT64(1)) {
#line 511 "../lib/Chai.chpl"
    _args_forcoforall_fn_chpl = ((_class_localscoforall_fn_chpl110)(&chpl_macro_tmp_115));
#line 511 "../lib/Chai.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl, INT64(511), INT32(65));
#line 511 "../lib/Chai.chpl"
    (_args_forcoforall_fn_chpl)->_0_length = length_chpl;
#line 511 "../lib/Chai.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl, INT64(511), INT32(65));
#line 511 "../lib/Chai.chpl"
    (_args_forcoforall_fn_chpl)->_1_numChunks_captemp = numChunks_captemp_chpl;
#line 511 "../lib/Chai.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl, INT64(511), INT32(65));
#line 511 "../lib/Chai.chpl"
    (_args_forcoforall_fn_chpl)->_2__coforallCount = _coforallCount_chpl;
#line 511 "../lib/Chai.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl, INT64(511), INT32(65));
#line 511 "../lib/Chai.chpl"
    (_args_forcoforall_fn_chpl)->_3_chunk = i_chpl;
#line 511 "../lib/Chai.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl, INT64(511), INT32(65));
#line 511 "../lib/Chai.chpl"
    (_args_forcoforall_fn_chpl)->_4_globalRP_call_tmp = globalRP_call_tmp_chpl;
#line 511 "../lib/Chai.chpl"
    tmp_chpl2 = &chpl__iterLF;
#line 511 "../lib/Chai.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl, INT64(511), INT32(65));
#line 511 "../lib/Chai.chpl"
    (_args_forcoforall_fn_chpl)->_5_chpl__iterLF = tmp_chpl2;
#line 511 "../lib/Chai.chpl"
    _args_vforcoforall_fn_chpl = ((void*)(_args_forcoforall_fn_chpl));
#line 511 "../lib/Chai.chpl"
    /*** wrapcoforall_fn_chpl110 ***/ chpl_taskAddCoStmt(chpl_task_getRequestedSubloc(), INT64(109), ((chpl_task_bundle_p)(_args_vforcoforall_fn_chpl)), sizeof(chpl__class_localscoforall_fn_chpl110_object), INT64(511), INT32(65));
#line 511 "../lib/Chai.chpl"
  }
#line 511 "../lib/Chai.chpl"
  error_chpl = NULL;
#line 511 "../lib/Chai.chpl"
  _waitEndCount_chpl2(_coforallCount_chpl, numTasks_chpl, &error_chpl, INT64(511), INT32(65));
#line 511 "../lib/Chai.chpl"
  if (error_chpl != nil) /* ZLINE: 511 ../lib/Chai.chpl */
#line 511 "../lib/Chai.chpl"
  {
#line 511 "../lib/Chai.chpl"
    chpl_propagate_error(error_chpl, INT64(511), INT32(65));
#line 511 "../lib/Chai.chpl"
  }
#line 511 "../lib/Chai.chpl"
  chpl__delete72(_coforallCount_chpl, INT64(511), INT32(65));
#line 511 "../lib/Chai.chpl"
  chpl_after_forall_fence();
#line 511 "../lib/Chai.chpl"
  coerce_tmp_chpl7 = globalRP_call_tmp_chpl;
#line 511 "../lib/Chai.chpl"
  chpl_check_nil(coerce_tmp_chpl7, INT64(511), INT32(65));
#line 511 "../lib/Chai.chpl"
  accumulate_chpl(coerce_tmp_chpl7, globalAS_chpl_redSVar_chpl, INT64(511), INT32(65));
#line 511 "../lib/Chai.chpl"
  coerce_tmp_chpl8 = globalRP_call_tmp_chpl;
#line 511 "../lib/Chai.chpl"
  chpl_check_nil(coerce_tmp_chpl8, INT64(511), INT32(65));
#line 511 "../lib/Chai.chpl"
  call_tmp_chpl17 = generate_chpl(coerce_tmp_chpl8, INT64(511), INT32(65));
#line 511 "../lib/Chai.chpl"
  chpl__delete23(globalRP_call_tmp_chpl, INT64(511), INT32(65));
#line 512 "../lib/Chai.chpl"
  if (call_tmp_chpl17 < INT64(1)) /* ZLINE: 512 ../lib/Chai.chpl */
#line 512 "../lib/Chai.chpl"
  {
#line 512 "../lib/Chai.chpl"
    err_chpl4(&_str_literal_1718_chpl);
#line 512 "../lib/Chai.chpl"
  }
#line 514 "../lib/Chai.chpl"
  call_tmp_chpl18 = (REAL64(0x1p+1) / ((_real64)(((int64_t)((call_tmp_chpl17 + (this_chpl31)->outputSize))))));
#line 514 "../lib/Chai.chpl"
  sqrt(call_tmp_chpl18);
#line 516 "../lib/Chai.chpl"
  call_tmp_chpl19 = &((this_chpl31)->weights);
#line 516 "../lib/Chai.chpl"
  randn_chpl((this_chpl31)->outputSize, call_tmp_chpl17, &ret_tmp_chpl9);
#line 516 "../lib/Chai.chpl"
  call_tmp_chpl20 = ret_tmp_chpl9;
#line 516 "../lib/Chai.chpl"
  chpl___SLASH_(&call_tmp_chpl20, ((_real64)(call_tmp_chpl17)), &ret_tmp_chpl10);
#line 516 "../lib/Chai.chpl"
  call_tmp_chpl21 = ret_tmp_chpl10;
#line 516 "../lib/Chai.chpl"
  chpl___ASSIGN_23(call_tmp_chpl19, &call_tmp_chpl21);
#line 516 "../lib/Chai.chpl"
  _field_destructor_tmp__chpl = (&call_tmp_chpl21)->data;
#line 516 "../lib/Chai.chpl"
  chpl__autoDestroy19(&_field_destructor_tmp__chpl, INT64(516), INT32(65));
#line 516 "../lib/Chai.chpl"
  _field_destructor_tmp__chpl2 = (&call_tmp_chpl21)->_domain;
#line 516 "../lib/Chai.chpl"
  chpl__autoDestroy5(&_field_destructor_tmp__chpl2, INT64(516), INT32(65));
#line 516 "../lib/Chai.chpl"
  _field_destructor_tmp__chpl3 = (&call_tmp_chpl20)->data;
#line 516 "../lib/Chai.chpl"
  chpl__autoDestroy19(&_field_destructor_tmp__chpl3, INT64(516), INT32(65));
#line 516 "../lib/Chai.chpl"
  _field_destructor_tmp__chpl4 = (&call_tmp_chpl20)->_domain;
#line 516 "../lib/Chai.chpl"
  chpl__autoDestroy5(&_field_destructor_tmp__chpl4, INT64(516), INT32(65));
#line 517 "../lib/Chai.chpl"
  call_tmp_chpl22 = &((this_chpl31)->biases);
#line 517 "../lib/Chai.chpl"
  zeros_chpl4((this_chpl31)->outputSize, &ret_tmp_chpl11);
#line 517 "../lib/Chai.chpl"
  call_tmp_chpl23 = ret_tmp_chpl11;
#line 517 "../lib/Chai.chpl"
  chpl___ASSIGN_25(call_tmp_chpl22, &call_tmp_chpl23);
#line 517 "../lib/Chai.chpl"
  _field_destructor_tmp__chpl5 = (&call_tmp_chpl23)->data;
#line 517 "../lib/Chai.chpl"
  chpl__autoDestroy18(&_field_destructor_tmp__chpl5, INT64(517), INT32(65));
#line 517 "../lib/Chai.chpl"
  _field_destructor_tmp__chpl6 = (&call_tmp_chpl23)->_domain;
#line 517 "../lib/Chai.chpl"
  chpl__autoDestroy11(&_field_destructor_tmp__chpl6, INT64(517), INT32(65));
#line 519 "../lib/Chai.chpl"
  call_tmp_chpl24 = &((this_chpl31)->weightsGrad);
#line 519 "../lib/Chai.chpl"
  zeros_chpl3((this_chpl31)->outputSize, call_tmp_chpl17, &ret_tmp_chpl12);
#line 519 "../lib/Chai.chpl"
  call_tmp_chpl25 = ret_tmp_chpl12;
#line 519 "../lib/Chai.chpl"
  chpl___ASSIGN_23(call_tmp_chpl24, &call_tmp_chpl25);
#line 519 "../lib/Chai.chpl"
  _field_destructor_tmp__chpl7 = (&call_tmp_chpl25)->data;
#line 519 "../lib/Chai.chpl"
  chpl__autoDestroy19(&_field_destructor_tmp__chpl7, INT64(519), INT32(65));
#line 519 "../lib/Chai.chpl"
  _field_destructor_tmp__chpl8 = (&call_tmp_chpl25)->_domain;
#line 519 "../lib/Chai.chpl"
  chpl__autoDestroy5(&_field_destructor_tmp__chpl8, INT64(519), INT32(65));
#line 520 "../lib/Chai.chpl"
  call_tmp_chpl26 = &((this_chpl31)->biasesGrad);
#line 520 "../lib/Chai.chpl"
  zeros_chpl4((this_chpl31)->outputSize, &ret_tmp_chpl13);
#line 520 "../lib/Chai.chpl"
  call_tmp_chpl27 = ret_tmp_chpl13;
#line 520 "../lib/Chai.chpl"
  chpl___ASSIGN_25(call_tmp_chpl26, &call_tmp_chpl27);
#line 520 "../lib/Chai.chpl"
  _field_destructor_tmp__chpl9 = (&call_tmp_chpl27)->data;
#line 520 "../lib/Chai.chpl"
  chpl__autoDestroy18(&_field_destructor_tmp__chpl9, INT64(520), INT32(65));
#line 520 "../lib/Chai.chpl"
  _field_destructor_tmp__chpl10 = (&call_tmp_chpl27)->_domain;
#line 520 "../lib/Chai.chpl"
  chpl__autoDestroy11(&_field_destructor_tmp__chpl10, INT64(520), INT32(65));
#line 522 "../lib/Chai.chpl"
  (this_chpl31)->uninitialized = UINT8(false);
#line 510 "../lib/Chai.chpl"
  return;
#line 510 "../lib/Chai.chpl"
}

#line 511 "../lib/Chai.chpl"
/* Chai.chpl:511 */
#line 511 "../lib/Chai.chpl"
static void coforall_fn_chpl110(int64_t length_chpl,
#line 511 "../lib/Chai.chpl"
                                int64_t numChunks_chpl,
#line 511 "../lib/Chai.chpl"
                                chpl___EndCount_AtomicT_int64_t_int64_t _coforallCount_chpl,
#line 511 "../lib/Chai.chpl"
                                int64_t chunk_chpl,
#line 511 "../lib/Chai.chpl"
                                ProductReduceScanOp_int64_t_chpl RP_chpl_redSVar_chpl,
#line 511 "../lib/Chai.chpl"
                                _tuple_3_star_int64_t_chpl * chpl__iterLF) {
#line 511 "../lib/Chai.chpl"
  ProductReduceScanOp_int64_t_chpl RP_chpl_redSVar_chpl2 = NULL;
#line 511 "../lib/Chai.chpl"
  int64_t AS_chpl_redSVar_chpl;
#line 511 "../lib/Chai.chpl"
  ProductReduceScanOp_int64_t_chpl call_tmp_chpl6 = NULL;
#line 511 "../lib/Chai.chpl"
  int64_t call_tmp_chpl7;
#line 511 "../lib/Chai.chpl"
  _tuple_2_star_int64_t_chpl call_tmp_chpl8;
#line 511 "../lib/Chai.chpl"
  _tuple_2_star_int64_t_chpl ret_tmp_chpl;
#line 511 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl9;
#line 511 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl2;
#line 511 "../lib/Chai.chpl"
  _tuple_1_star_range_int64_t_both_one_chpl ret_tmp_chpl3;
#line 511 "../lib/Chai.chpl"
  _tuple_1_star_range_int64_t_both_one_chpl chpl_followThis;
#line 511 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl fThis_chpl;
#line 511 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl _ic__F0_this_chpl;
#line 511 "../lib/Chai.chpl"
  int64_t i_chpl;
#line 511 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl this_chpl31;
#line 511 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl this_chpl32;
#line 511 "../lib/Chai.chpl"
  int64_t end_chpl;
#line 511 "../lib/Chai.chpl"
  _ref_int64_t_chpl call_tmp_chpl10 = NULL;
#line 511 "../lib/Chai.chpl"
  int64_t coerce_tmp_chpl3;
#line 511 "../lib/Chai.chpl"
  ProductReduceScanOp_int64_t_chpl coerce_tmp_chpl4 = NULL;
#line 511 "../lib/Chai.chpl"
  chpl_check_nil(RP_chpl_redSVar_chpl, INT64(511), INT32(65));
#line 511 "../lib/Chai.chpl"
  call_tmp_chpl6 = clone_chpl2(RP_chpl_redSVar_chpl, INT64(511), INT32(65));
#line 511 "../lib/Chai.chpl"
  RP_chpl_redSVar_chpl2 = call_tmp_chpl6;
#line 511 "../lib/Chai.chpl"
  chpl_check_nil(call_tmp_chpl6, INT64(511), INT32(65));
#line 511 "../lib/Chai.chpl"
  call_tmp_chpl7 = identity_chpl(call_tmp_chpl6);
#line 511 "../lib/Chai.chpl"
  AS_chpl_redSVar_chpl = call_tmp_chpl7;
#line 511 "../lib/Chai.chpl"
  _computeBlock_chpl(length_chpl, numChunks_chpl, chunk_chpl, ((int64_t)((length_chpl - INT64(1)))), INT64(0), INT64(0), &ret_tmp_chpl, INT64(511), INT32(65));
#line 511 "../lib/Chai.chpl"
  *(call_tmp_chpl8 + INT64(0)) = *(ret_tmp_chpl + INT64(0));
#line 511 "../lib/Chai.chpl"
  *(call_tmp_chpl8 + INT64(1)) = *(ret_tmp_chpl + INT64(1));
#line 511 "../lib/Chai.chpl"
  chpl_build_bounded_range6(*(call_tmp_chpl8 + INT64(0)), *(call_tmp_chpl8 + INT64(1)), &ret_tmp_chpl2);
#line 511 "../lib/Chai.chpl"
  call_tmp_chpl9 = ret_tmp_chpl2;
#line 511 "../lib/Chai.chpl"
  *(ret_tmp_chpl3 + INT64(0)) = call_tmp_chpl9;
#line 511 "../lib/Chai.chpl"
  *(chpl_followThis + INT64(0)) = *(ret_tmp_chpl3 + INT64(0));
#line 511 "../lib/Chai.chpl"
  fThis_chpl = *(chpl_followThis + INT64(0));
#line 511 "../lib/Chai.chpl"
  _ic__F0_this_chpl = fThis_chpl;
#line 511 "../lib/Chai.chpl"
  checkIfIterWillOverflow_chpl(fThis_chpl, UINT8(true), INT64(511), INT32(65));
#line 511 "../lib/Chai.chpl"
  i_chpl = INT64(0);
#line 511 "../lib/Chai.chpl"
  this_chpl31 = _ic__F0_this_chpl;
#line 511 "../lib/Chai.chpl"
  this_chpl32 = _ic__F0_this_chpl;
#line 511 "../lib/Chai.chpl"
  end_chpl = (&this_chpl32)->_high;
#line 511 "../lib/Chai.chpl"
  for (i_chpl = (&this_chpl31)->_low; ((i_chpl <= end_chpl)); i_chpl += INT64(1)) {
#line 511 "../lib/Chai.chpl"
    call_tmp_chpl10 = this_chpl23(chpl__iterLF, i_chpl, INT64(511), INT32(65));
#line 511 "../lib/Chai.chpl"
    coerce_tmp_chpl3 = *(call_tmp_chpl10);
#line 511 "../lib/Chai.chpl"
    chpl_check_nil(RP_chpl_redSVar_chpl2, INT64(511), INT32(65));
#line 511 "../lib/Chai.chpl"
    accumulateOntoState_chpl(RP_chpl_redSVar_chpl2, &AS_chpl_redSVar_chpl, coerce_tmp_chpl3);
#line 511 "../lib/Chai.chpl"
  }
#line 511 "../lib/Chai.chpl"
  coerce_tmp_chpl4 = RP_chpl_redSVar_chpl2;
#line 511 "../lib/Chai.chpl"
  chpl_check_nil(coerce_tmp_chpl4, INT64(511), INT32(65));
#line 511 "../lib/Chai.chpl"
  accumulate_chpl(coerce_tmp_chpl4, AS_chpl_redSVar_chpl, INT64(511), INT32(65));
#line 511 "../lib/Chai.chpl"
  chpl__reduceCombine4(RP_chpl_redSVar_chpl, RP_chpl_redSVar_chpl2, INT64(511), INT32(65));
#line 511 "../lib/Chai.chpl"
  chpl__delete23(RP_chpl_redSVar_chpl2, INT64(511), INT32(65));
#line 511 "../lib/Chai.chpl"
  return;
#line 511 "../lib/Chai.chpl"
}

#line 511 "../lib/Chai.chpl"
/* Chai.chpl:511 */
#line 511 "../lib/Chai.chpl"
static void wrapcoforall_fn_chpl110(_class_localscoforall_fn_chpl110 c_chpl) {
#line 511 "../lib/Chai.chpl"
  int64_t _0_length_chpl;
#line 511 "../lib/Chai.chpl"
  int64_t _1_numChunks_captemp_chpl;
#line 511 "../lib/Chai.chpl"
  chpl___EndCount_AtomicT_int64_t_int64_t _2__coforallCount_chpl = NULL;
#line 511 "../lib/Chai.chpl"
  int64_t _3_chunk_chpl;
#line 511 "../lib/Chai.chpl"
  ProductReduceScanOp_int64_t_chpl _4_globalRP_call_tmp_chpl = NULL;
#line 511 "../lib/Chai.chpl"
  _ref__tuple_3_star_int64_t_chpl _5_chpl__iterLF_chpl = NULL;
#line 511 "../lib/Chai.chpl"
  chpl_rmem_consist_acquire(INT64(511), INT32(65));
#line 511 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(511), INT32(65));
#line 511 "../lib/Chai.chpl"
  _0_length_chpl = (c_chpl)->_0_length;
#line 511 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(511), INT32(65));
#line 511 "../lib/Chai.chpl"
  _1_numChunks_captemp_chpl = (c_chpl)->_1_numChunks_captemp;
#line 511 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(511), INT32(65));
#line 511 "../lib/Chai.chpl"
  _2__coforallCount_chpl = (c_chpl)->_2__coforallCount;
#line 511 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(511), INT32(65));
#line 511 "../lib/Chai.chpl"
  _3_chunk_chpl = (c_chpl)->_3_chunk;
#line 511 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(511), INT32(65));
#line 511 "../lib/Chai.chpl"
  _4_globalRP_call_tmp_chpl = (c_chpl)->_4_globalRP_call_tmp;
#line 511 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(511), INT32(65));
#line 511 "../lib/Chai.chpl"
  _5_chpl__iterLF_chpl = (c_chpl)->_5_chpl__iterLF;
#line 511 "../lib/Chai.chpl"
  coforall_fn_chpl110(_0_length_chpl, _1_numChunks_captemp_chpl, _2__coforallCount_chpl, _3_chunk_chpl, _4_globalRP_call_tmp_chpl, _5_chpl__iterLF_chpl);
#line 511 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(511), INT32(65));
#line 511 "../lib/Chai.chpl"
  _downEndCount_chpl((c_chpl)->_2__coforallCount, nil, INT64(511), INT32(65));
#line 511 "../lib/Chai.chpl"
  return;
#line 511 "../lib/Chai.chpl"
}

#line 525 "../lib/Chai.chpl"
/* Chai.chpl:525 */
#line 525 "../lib/Chai.chpl"
static void forwardPropBatch_chpl3(SoftMax_chpl * this_chpl31,
#line 525 "../lib/Chai.chpl"
                                   _array_DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl * batch_chpl,
#line 525 "../lib/Chai.chpl"
                                   _array_DefaultRectangularArr_1_int64_t_one_Tensor_1__real64_int64_t_chpl * _retArg_chpl) {
#line 530 "../lib/Chai.chpl"
  int64_t local_dataParMinGranularity_chpl;
#line 530 "../lib/Chai.chpl"
  chpl_bool local_dataParIgnoreRunningTasks_chpl;
#line 530 "../lib/Chai.chpl"
  int64_t local_dataParTasksPerLocale_chpl;
#line 526 "../lib/Chai.chpl"
  _tuple_1_star_int64_t_chpl coerce_tmp_chpl3;
#line 526 "../lib/Chai.chpl"
  _ref__tuple_1_star_int64_t_chpl i_i_chpl = NULL;
#line 526 "../lib/Chai.chpl"
  DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl ret_chpl = NULL;
#line 526 "../lib/Chai.chpl"
  DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl coerce_tmp_chpl4 = NULL;
#line 526 "../lib/Chai.chpl"
  _tuple_1_star_int64_t_chpl _formal_tmp_in_ind_chpl;
#line 526 "../lib/Chai.chpl"
  int64_t _formal_tmp_ind_x0_chpl;
#line 526 "../lib/Chai.chpl"
  _ddata_Tensor_3__real64_chpl coerce_tmp_chpl5 = NULL;
#line 526 "../lib/Chai.chpl"
  _ref_Tensor_3__real64_chpl call_tmp_chpl6 = NULL;
#line 528 "../lib/Chai.chpl"
  int64_t call_tmp_chpl7;
#line 529 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_1_int64_t_one_Tensor_1__real64_int64_t_chpl outputs_chpl;
#line 529 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl call_tmp_chpl8;
#line 529 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl ret_tmp_chpl;
#line 529 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl9;
#line 529 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl2;
#line 529 "../lib/Chai.chpl"
  _domain_DefaultRectangularDom_1_int64_t_one_chpl call_tmp_chpl10;
#line 529 "../lib/Chai.chpl"
  _domain_DefaultRectangularDom_1_int64_t_one_chpl ret_tmp_chpl3;
#line 529 "../lib/Chai.chpl"
  chpl___RuntimeTypeInfo12 call_tmp_chpl11;
#line 529 "../lib/Chai.chpl"
  _domain_DefaultRectangularDom_1_int64_t_one_chpl _runtime_type_tmp__chpl;
#line 529 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_1_int64_t_one_Tensor_1__real64_int64_t_chpl ret_tmp_chpl4;
#line 530 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl call_tmp_chpl12;
#line 530 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl ret_tmp_chpl5;
#line 530 "../lib/Chai.chpl"
  DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl coerce_tmp_chpl6 = NULL;
#line 530 "../lib/Chai.chpl"
  int64_t tasksPerLocale_chpl;
#line 530 "../lib/Chai.chpl"
  chpl_bool ignoreRunning_chpl;
#line 530 "../lib/Chai.chpl"
  int64_t minIndicesPerTask_chpl;
#line 530 "../lib/Chai.chpl"
  Error_chpl error_chpl = NULL;
#line 530 "../lib/Chai.chpl"
  DefaultRectangularDom_1_int64_t_one_chpl coerce_tmp_chpl7 = NULL;
#line 530 "../lib/Chai.chpl"
  int64_t offset_x0_chpl;
#line 530 "../lib/Chai.chpl"
  _tuple_1_star_int64_t_chpl ret_tmp_chpl6;
#line 530 "../lib/Chai.chpl"
  Error_chpl error_chpl2 = NULL;
#line 530 "../lib/Chai.chpl"
  locale_chpl call_tmp_chpl13;
#line 530 "../lib/Chai.chpl"
  chpl_localeID_t call_tmp_chpl14;
#line 530 "../lib/Chai.chpl"
  int32_t call_tmp_chpl15;
#line 530 "../lib/Chai.chpl"
  chpl_localeID_t call_tmp_chpl16;
#line 530 "../lib/Chai.chpl"
  locale_chpl call_tmp_chpl17;
#line 530 "../lib/Chai.chpl"
  locale_chpl ret_tmp_chpl7;
#line 530 "../lib/Chai.chpl"
  BaseLocale_chpl coerce_tmp_chpl8 = NULL;
#line 530 "../lib/Chai.chpl"
  _ref_locale_chpl i_x_chpl = NULL;
#line 530 "../lib/Chai.chpl"
  int64_t tmp_chpl;
#line 530 "../lib/Chai.chpl"
  locale_chpl call_tmp_chpl18;
#line 530 "../lib/Chai.chpl"
  chpl_localeID_t call_tmp_chpl19;
#line 530 "../lib/Chai.chpl"
  int32_t call_tmp_chpl20;
#line 530 "../lib/Chai.chpl"
  chpl_localeID_t call_tmp_chpl21;
#line 530 "../lib/Chai.chpl"
  locale_chpl call_tmp_chpl22;
#line 530 "../lib/Chai.chpl"
  locale_chpl ret_tmp_chpl8;
#line 530 "../lib/Chai.chpl"
  int64_t ret_chpl2;
#line 530 "../lib/Chai.chpl"
  BaseLocale_chpl coerce_tmp_chpl9 = NULL;
#line 530 "../lib/Chai.chpl"
  BaseLocale_chpl coerce_tmp_chpl10 = NULL;
#line 530 "../lib/Chai.chpl"
  _ref_locale_chpl i_x_chpl2 = NULL;
#line 530 "../lib/Chai.chpl"
  int64_t tmp_x0_chpl;
#line 530 "../lib/Chai.chpl"
  int64_t tmp_x1_chpl;
#line 530 "../lib/Chai.chpl"
  _ref__tuple_1_star_range_int64_t_both_one_chpl call_tmp_chpl23 = NULL;
#line 530 "../lib/Chai.chpl"
  _tuple_2_star_int64_t_chpl ret_tmp_chpl9;
#line 530 "../lib/Chai.chpl"
  int64_t numChunks_chpl;
#line 530 "../lib/Chai.chpl"
  int64_t parDim_chpl;
#line 530 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl locBlock_x0_chpl;
#line 530 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl tup_x0_chpl;
#line 530 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl elt_x0_chpl;
#line 530 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl call_tmp_chpl24;
#line 530 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl ret_tmp_chpl10;
#line 530 "../lib/Chai.chpl"
  _ref__tuple_1_star_range_int64_t_both_one_chpl call_tmp_chpl25 = NULL;
#line 530 "../lib/Chai.chpl"
  int64_t call_tmp_chpl26;
#line 530 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl11;
#line 530 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl27;
#line 530 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl11;
#line 530 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl tmpIter_chpl;
#line 530 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl call_tmp_chpl28;
#line 530 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl ret_tmp_chpl12;
#line 530 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl13;
#line 530 "../lib/Chai.chpl"
  chpl___EndCount_AtomicT_int64_t_int64_t _coforallCount_chpl = NULL;
#line 530 "../lib/Chai.chpl"
  chpl___EndCount_AtomicT_int64_t_int64_t new_temp_chpl = NULL;
#line 530 "../lib/Chai.chpl"
  int64_t numTasks_chpl;
#line 530 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl _ic__F0_this_chpl;
#line 530 "../lib/Chai.chpl"
  int64_t i_chpl;
#line 530 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl this_chpl32;
#line 530 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl this_chpl33;
#line 530 "../lib/Chai.chpl"
  int64_t end_chpl;
#line 530 "../lib/Chai.chpl"
  _tuple_1_star_range_int64_t_both_one_chpl ret_tmp_chpl14;
#line 530 "../lib/Chai.chpl"
  _class_localscoforall_fn_chpl111 _args_forcoforall_fn_chpl = NULL;
#line 530 "../lib/Chai.chpl"
  _ref_SoftMax_chpl tmp_chpl2 = NULL;
#line 530 "../lib/Chai.chpl"
  _ref_range_int64_t_low_one_chpl tmp_chpl3 = NULL;
#line 530 "../lib/Chai.chpl"
  raw_c_void_ptr _args_vforcoforall_fn_chpl;
#line 530 "../lib/Chai.chpl"
  Error_chpl error_chpl3 = NULL;
#line 530 "../lib/Chai.chpl"
  chpl__class_localscoforall_fn_chpl111_object chpl_macro_tmp_116;
#line 530 "../lib/Chai.chpl"
  local_dataParMinGranularity_chpl = dataParMinGranularity_chpl;
#line 530 "../lib/Chai.chpl"
  local_dataParIgnoreRunningTasks_chpl = dataParIgnoreRunningTasks_chpl;
#line 530 "../lib/Chai.chpl"
  local_dataParTasksPerLocale_chpl = dataParTasksPerLocale_chpl;
#line 526 "../lib/Chai.chpl"
  if ((this_chpl31)->uninitialized) /* ZLINE: 526 ../lib/Chai.chpl */
#line 526 "../lib/Chai.chpl"
  {
#line 526 "../lib/Chai.chpl"
    *(coerce_tmp_chpl3 + INT64(0)) = INT64(0);
#line 526 "../lib/Chai.chpl"
    i_i_chpl = &coerce_tmp_chpl3;
#line 526 "../lib/Chai.chpl"
    ret_chpl = (batch_chpl)->_instance;
#line 526 "../lib/Chai.chpl"
    checkAccess_chpl10(batch_chpl, i_i_chpl, ret_chpl, INT64(526), INT32(65));
#line 526 "../lib/Chai.chpl"
    coerce_tmp_chpl4 = ret_chpl;
#line 526 "../lib/Chai.chpl"
    *(_formal_tmp_in_ind_chpl + INT64(0)) = *(coerce_tmp_chpl3 + INT64(0));
#line 526 "../lib/Chai.chpl"
    _formal_tmp_ind_x0_chpl = *(_formal_tmp_in_ind_chpl + INT64(0));
#line 526 "../lib/Chai.chpl"
    chpl_check_nil(coerce_tmp_chpl4, INT64(526), INT32(65));
#line 526 "../lib/Chai.chpl"
    coerce_tmp_chpl5 = (coerce_tmp_chpl4)->shiftedData;
#line 526 "../lib/Chai.chpl"
    call_tmp_chpl6 = (coerce_tmp_chpl5 + _formal_tmp_ind_x0_chpl);
#line 526 "../lib/Chai.chpl"
    initialize_chpl(this_chpl31, call_tmp_chpl6);
#line 526 "../lib/Chai.chpl"
  }
#line 528 "../lib/Chai.chpl"
  call_tmp_chpl7 = size_chpl(batch_chpl, INT64(528), INT32(65));
#line 529 "../lib/Chai.chpl"
  chpl_build_low_bounded_range2(INT64(0), &ret_tmp_chpl);
#line 529 "../lib/Chai.chpl"
  call_tmp_chpl8 = ret_tmp_chpl;
#line 529 "../lib/Chai.chpl"
  chpl___POUND_3(call_tmp_chpl8, call_tmp_chpl7, &ret_tmp_chpl2, INT64(529), INT32(65));
#line 529 "../lib/Chai.chpl"
  call_tmp_chpl9 = ret_tmp_chpl2;
#line 529 "../lib/Chai.chpl"
  chpl__ensureDomainExpr5(call_tmp_chpl9, &ret_tmp_chpl3, INT64(529), INT32(65));
#line 529 "../lib/Chai.chpl"
  call_tmp_chpl10 = ret_tmp_chpl3;
#line 529 "../lib/Chai.chpl"
  call_tmp_chpl11 = chpl__buildArrayRuntimeType11(&call_tmp_chpl10);
#line 529 "../lib/Chai.chpl"
  _runtime_type_tmp__chpl = (&call_tmp_chpl11)->dom;
#line 529 "../lib/Chai.chpl"
  chpl__convertRuntimeTypeToValue(&_runtime_type_tmp__chpl, UINT8(false), &ret_tmp_chpl4, INT64(529), INT32(65));
#line 529 "../lib/Chai.chpl"
  outputs_chpl = ret_tmp_chpl4;
#line 530 "../lib/Chai.chpl"
  chpl_build_low_bounded_range2(INT64(0), &ret_tmp_chpl5);
#line 530 "../lib/Chai.chpl"
  call_tmp_chpl12 = ret_tmp_chpl5;
#line 530 "../lib/Chai.chpl"
  coerce_tmp_chpl6 = (batch_chpl)->_instance;
#line 530 "../lib/Chai.chpl"
  tasksPerLocale_chpl = local_dataParTasksPerLocale_chpl;
#line 530 "../lib/Chai.chpl"
  ignoreRunning_chpl = local_dataParIgnoreRunningTasks_chpl;
#line 530 "../lib/Chai.chpl"
  minIndicesPerTask_chpl = local_dataParMinGranularity_chpl;
#line 530 "../lib/Chai.chpl"
  error_chpl = NULL;
#line 530 "../lib/Chai.chpl"
  chpl_check_nil(coerce_tmp_chpl6, INT64(530), INT32(65));
#line 530 "../lib/Chai.chpl"
  coerce_tmp_chpl7 = (coerce_tmp_chpl6)->dom;
#line 530 "../lib/Chai.chpl"
  createTuple_chpl(INT64(0), &ret_tmp_chpl6);
#line 530 "../lib/Chai.chpl"
  offset_x0_chpl = *(ret_tmp_chpl6 + INT64(0));
#line 530 "../lib/Chai.chpl"
  error_chpl2 = NULL;
#line 530 "../lib/Chai.chpl"
  call_tmp_chpl15 = get_chpl_nodeID();
#line 530 "../lib/Chai.chpl"
  call_tmp_chpl16 = chpl_rt_buildLocaleID(call_tmp_chpl15, c_sublocid_any);
#line 530 "../lib/Chai.chpl"
  call_tmp_chpl14 = call_tmp_chpl16;
#line 530 "../lib/Chai.chpl"
  chpl_localeID_to_locale(&call_tmp_chpl14, &ret_tmp_chpl7, INT64(530), INT32(65));
#line 530 "../lib/Chai.chpl"
  call_tmp_chpl17 = ret_tmp_chpl7;
#line 530 "../lib/Chai.chpl"
  call_tmp_chpl13 = call_tmp_chpl17;
#line 530 "../lib/Chai.chpl"
  coerce_tmp_chpl8 = (&call_tmp_chpl13)->_instance;
#line 530 "../lib/Chai.chpl"
  if (((RootClass_chpl)(coerce_tmp_chpl8)) == nil) /* ZLINE: 530 ../lib/Chai.chpl */
#line 530 "../lib/Chai.chpl"
  {
#line 530 "../lib/Chai.chpl"
    nilCheckHalt_chpl(&_str_literal_52_chpl, INT64(530), INT32(65));
#line 530 "../lib/Chai.chpl"
  }
#line 530 "../lib/Chai.chpl"
  chpl_check_nil(coerce_tmp_chpl8, INT64(530), INT32(65));
#line 530 "../lib/Chai.chpl"
  ((int64_t(*)(BaseLocale_chpl,
#line 530 "../lib/Chai.chpl"
     int64_t,
#line 530 "../lib/Chai.chpl"
     int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(coerce_tmp_chpl8))->chpl__cid) + INT64(2))])(coerce_tmp_chpl8, INT64(530), INT32(65));
#line 530 "../lib/Chai.chpl"
  i_x_chpl = &call_tmp_chpl13;
#line 530 "../lib/Chai.chpl"
  deinit_chpl62(i_x_chpl);
#line 530 "../lib/Chai.chpl"
  if (tasksPerLocale_chpl == INT64(0)) /* ZLINE: 530 ../lib/Chai.chpl */
#line 530 "../lib/Chai.chpl"
  {
#line 530 "../lib/Chai.chpl"
    call_tmp_chpl20 = get_chpl_nodeID();
#line 530 "../lib/Chai.chpl"
    call_tmp_chpl21 = chpl_rt_buildLocaleID(call_tmp_chpl20, c_sublocid_any);
#line 530 "../lib/Chai.chpl"
    call_tmp_chpl19 = call_tmp_chpl21;
#line 530 "../lib/Chai.chpl"
    chpl_localeID_to_locale(&call_tmp_chpl19, &ret_tmp_chpl8, INT64(530), INT32(65));
#line 530 "../lib/Chai.chpl"
    call_tmp_chpl22 = ret_tmp_chpl8;
#line 530 "../lib/Chai.chpl"
    call_tmp_chpl18 = call_tmp_chpl22;
#line 530 "../lib/Chai.chpl"
    coerce_tmp_chpl9 = (&call_tmp_chpl18)->_instance;
#line 530 "../lib/Chai.chpl"
    if (((RootClass_chpl)(coerce_tmp_chpl9)) == nil) /* ZLINE: 530 ../lib/Chai.chpl */
#line 530 "../lib/Chai.chpl"
    {
#line 530 "../lib/Chai.chpl"
      nilCheckHalt_chpl(&_str_literal_52_chpl, INT64(530), INT32(65));
#line 530 "../lib/Chai.chpl"
    }
#line 530 "../lib/Chai.chpl"
    coerce_tmp_chpl10 = coerce_tmp_chpl9;
#line 530 "../lib/Chai.chpl"
    chpl_check_nil(coerce_tmp_chpl10, INT64(530), INT32(65));
#line 530 "../lib/Chai.chpl"
    ret_chpl2 = (coerce_tmp_chpl10)->maxTaskPar;
#line 530 "../lib/Chai.chpl"
    tmp_chpl = ret_chpl2;
#line 530 "../lib/Chai.chpl"
    i_x_chpl2 = &call_tmp_chpl18;
#line 530 "../lib/Chai.chpl"
    deinit_chpl62(i_x_chpl2);
#line 530 "../lib/Chai.chpl"
  } else /* ZLINE: 530 ../lib/Chai.chpl */
#line 530 "../lib/Chai.chpl"
  {
#line 530 "../lib/Chai.chpl"
    tmp_chpl = tasksPerLocale_chpl;
#line 530 "../lib/Chai.chpl"
  }
#line 530 "../lib/Chai.chpl"
  if (chpl_task_getSerial()) /* ZLINE: 530 ../lib/Chai.chpl */
#line 530 "../lib/Chai.chpl"
  {
#line 530 "../lib/Chai.chpl"
    tmp_x0_chpl = INT64(1);
#line 530 "../lib/Chai.chpl"
    tmp_x1_chpl = INT64(0);
#line 530 "../lib/Chai.chpl"
  } else /* ZLINE: 530 ../lib/Chai.chpl */
#line 530 "../lib/Chai.chpl"
  {
#line 530 "../lib/Chai.chpl"
    chpl_check_nil(coerce_tmp_chpl7, INT64(530), INT32(65));
#line 530 "../lib/Chai.chpl"
    call_tmp_chpl23 = &((coerce_tmp_chpl7)->ranges);
#line 530 "../lib/Chai.chpl"
    _computeChunkStuff_chpl(tmp_chpl, ignoreRunning_chpl, minIndicesPerTask_chpl, call_tmp_chpl23, &ret_tmp_chpl9, INT64(530), INT32(65));
#line 530 "../lib/Chai.chpl"
    tmp_x0_chpl = *(ret_tmp_chpl9 + INT64(0));
#line 530 "../lib/Chai.chpl"
    tmp_x1_chpl = *(ret_tmp_chpl9 + INT64(1));
#line 530 "../lib/Chai.chpl"
  }
#line 530 "../lib/Chai.chpl"
  numChunks_chpl = tmp_x0_chpl;
#line 530 "../lib/Chai.chpl"
  parDim_chpl = tmp_x1_chpl;
#line 530 "../lib/Chai.chpl"
  init_chpl113(&elt_x0_chpl);
#line 530 "../lib/Chai.chpl"
  tup_x0_chpl = elt_x0_chpl;
#line 530 "../lib/Chai.chpl"
  locBlock_x0_chpl = tup_x0_chpl;
#line 530 "../lib/Chai.chpl"
  chpl_build_low_bounded_range2(offset_x0_chpl, &ret_tmp_chpl10);
#line 530 "../lib/Chai.chpl"
  call_tmp_chpl24 = ret_tmp_chpl10;
#line 530 "../lib/Chai.chpl"
  chpl_check_nil(coerce_tmp_chpl7, INT64(530), INT32(65));
#line 530 "../lib/Chai.chpl"
  call_tmp_chpl25 = &((coerce_tmp_chpl7)->ranges);
#line 530 "../lib/Chai.chpl"
  coerce_tmp_chpl11 = *(*(call_tmp_chpl25) + INT64(0));
#line 530 "../lib/Chai.chpl"
  call_tmp_chpl26 = sizeAs_chpl37(coerce_tmp_chpl11, INT64(530), INT32(65));
#line 530 "../lib/Chai.chpl"
  chpl___POUND_3(call_tmp_chpl24, call_tmp_chpl26, &ret_tmp_chpl11, INT64(530), INT32(65));
#line 530 "../lib/Chai.chpl"
  call_tmp_chpl27 = ret_tmp_chpl11;
#line 530 "../lib/Chai.chpl"
  locBlock_x0_chpl = call_tmp_chpl27;
#line 530 "../lib/Chai.chpl"
  chpl_build_low_bounded_range2(INT64(0), &ret_tmp_chpl12);
#line 530 "../lib/Chai.chpl"
  call_tmp_chpl28 = ret_tmp_chpl12;
#line 530 "../lib/Chai.chpl"
  chpl___POUND_3(call_tmp_chpl28, tmp_x0_chpl, &ret_tmp_chpl13, INT64(530), INT32(65));
#line 530 "../lib/Chai.chpl"
  tmpIter_chpl = ret_tmp_chpl13;
#line 530 "../lib/Chai.chpl"
  new_temp_chpl = _new_chpl17(INT64(530), INT32(65));
#line 530 "../lib/Chai.chpl"
  _coforallCount_chpl = new_temp_chpl;
#line 530 "../lib/Chai.chpl"
  numTasks_chpl = chpl_boundedCoforallSize(tmpIter_chpl, INT64(530), INT32(65));
#line 530 "../lib/Chai.chpl"
  _upEndCount_chpl(_coforallCount_chpl, numTasks_chpl, INT64(530), INT32(65));
#line 530 "../lib/Chai.chpl"
  chpl_resetTaskSpawn(numTasks_chpl, INT64(530), INT32(65));
#line 530 "../lib/Chai.chpl"
  _ic__F0_this_chpl = tmpIter_chpl;
#line 530 "../lib/Chai.chpl"
  checkIfIterWillOverflow_chpl(tmpIter_chpl, UINT8(true), INT64(530), INT32(65));
#line 530 "../lib/Chai.chpl"
  i_chpl = INT64(0);
#line 530 "../lib/Chai.chpl"
  this_chpl32 = _ic__F0_this_chpl;
#line 530 "../lib/Chai.chpl"
  this_chpl33 = _ic__F0_this_chpl;
#line 530 "../lib/Chai.chpl"
  end_chpl = (&this_chpl33)->_high;
#line 530 "../lib/Chai.chpl"
  for (i_chpl = (&this_chpl32)->_low; ((i_chpl <= end_chpl)); i_chpl += INT64(1)) {
#line 530 "../lib/Chai.chpl"
    *(ret_tmp_chpl14 + INT64(0)) = locBlock_x0_chpl;
#line 530 "../lib/Chai.chpl"
    _args_forcoforall_fn_chpl = ((_class_localscoforall_fn_chpl111)(&chpl_macro_tmp_116));
#line 530 "../lib/Chai.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl, INT64(530), INT32(65));
#line 530 "../lib/Chai.chpl"
    (_args_forcoforall_fn_chpl)->_0_numChunks = numChunks_chpl;
#line 530 "../lib/Chai.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl, INT64(530), INT32(65));
#line 530 "../lib/Chai.chpl"
    (_args_forcoforall_fn_chpl)->_1_parDim = parDim_chpl;
#line 530 "../lib/Chai.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl, INT64(530), INT32(65));
#line 530 "../lib/Chai.chpl"
    *((_args_forcoforall_fn_chpl)->_2_coerce_tmp + INT64(0)) = *(ret_tmp_chpl14 + INT64(0));
#line 530 "../lib/Chai.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl, INT64(530), INT32(65));
#line 530 "../lib/Chai.chpl"
    (_args_forcoforall_fn_chpl)->_3__coforallCount = _coforallCount_chpl;
#line 530 "../lib/Chai.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl, INT64(530), INT32(65));
#line 530 "../lib/Chai.chpl"
    (_args_forcoforall_fn_chpl)->_4_chunk = i_chpl;
#line 530 "../lib/Chai.chpl"
    tmp_chpl2 = this_chpl31;
#line 530 "../lib/Chai.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl, INT64(530), INT32(65));
#line 530 "../lib/Chai.chpl"
    (_args_forcoforall_fn_chpl)->_6_this = tmp_chpl2;
#line 530 "../lib/Chai.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl, INT64(530), INT32(65));
#line 530 "../lib/Chai.chpl"
    (_args_forcoforall_fn_chpl)->_7_batch = *(batch_chpl);
#line 530 "../lib/Chai.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl, INT64(530), INT32(65));
#line 530 "../lib/Chai.chpl"
    (_args_forcoforall_fn_chpl)->_8_outputs = outputs_chpl;
#line 530 "../lib/Chai.chpl"
    tmp_chpl3 = &call_tmp_chpl12;
#line 530 "../lib/Chai.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl, INT64(530), INT32(65));
#line 530 "../lib/Chai.chpl"
    (_args_forcoforall_fn_chpl)->_9_call_tmp = tmp_chpl3;
#line 530 "../lib/Chai.chpl"
    _args_vforcoforall_fn_chpl = ((void*)(_args_forcoforall_fn_chpl));
#line 530 "../lib/Chai.chpl"
    /*** wrapcoforall_fn_chpl111 ***/ chpl_taskAddCoStmt(chpl_task_getRequestedSubloc(), INT64(110), ((chpl_task_bundle_p)(_args_vforcoforall_fn_chpl)), sizeof(chpl__class_localscoforall_fn_chpl111_object), INT64(530), INT32(65));
#line 530 "../lib/Chai.chpl"
  }
#line 530 "../lib/Chai.chpl"
  error_chpl3 = NULL;
#line 530 "../lib/Chai.chpl"
  _waitEndCount_chpl2(_coforallCount_chpl, numTasks_chpl, &error_chpl3, INT64(530), INT32(65));
#line 530 "../lib/Chai.chpl"
  if (error_chpl3 != nil) /* ZLINE: 530 ../lib/Chai.chpl */
#line 530 "../lib/Chai.chpl"
  {
#line 530 "../lib/Chai.chpl"
    chpl__delete72(_coforallCount_chpl, INT64(530), INT32(65));
#line 530 "../lib/Chai.chpl"
    error_chpl2 = error_chpl3;
#line 530 "../lib/Chai.chpl"
    goto forall_IBB_break_label_chpl;
#line 530 "../lib/Chai.chpl"
  }
#line 530 "../lib/Chai.chpl"
  chpl__delete72(_coforallCount_chpl, INT64(530), INT32(65));
#line 530 "../lib/Chai.chpl"
  chpl_after_forall_fence();
#line 530 "../lib/Chai.chpl"
  forall_IBB_break_label_chpl:;
#line 530 "../lib/Chai.chpl"
  if (error_chpl2 != nil) /* ZLINE: 530 ../lib/Chai.chpl */
#line 530 "../lib/Chai.chpl"
  {
#line 530 "../lib/Chai.chpl"
    error_chpl = error_chpl2;
#line 530 "../lib/Chai.chpl"
    goto forall_IBB_break_label_chpl2;
#line 530 "../lib/Chai.chpl"
  }
#line 530 "../lib/Chai.chpl"
  chpl_after_forall_fence();
#line 530 "../lib/Chai.chpl"
  forall_IBB_break_label_chpl2:;
#line 530 "../lib/Chai.chpl"
  if (error_chpl != nil) /* ZLINE: 530 ../lib/Chai.chpl */
#line 530 "../lib/Chai.chpl"
  {
#line 530 "../lib/Chai.chpl"
    chpl_propagate_error(error_chpl, INT64(530), INT32(65));
#line 530 "../lib/Chai.chpl"
  }
#line 530 "../lib/Chai.chpl"
  chpl_after_forall_fence();
#line 533 "../lib/Chai.chpl"
  chpl__checkRetEltTypeMatch(&outputs_chpl);
#line 533 "../lib/Chai.chpl"
  chpl__autoDestroy11(&call_tmp_chpl10, INT64(533), INT32(65));
#line 525 "../lib/Chai.chpl"
  *(_retArg_chpl) = outputs_chpl;
#line 525 "../lib/Chai.chpl"
  return;
#line 525 "../lib/Chai.chpl"
}

#line 530 "../lib/Chai.chpl"
/* Chai.chpl:530 */
#line 530 "../lib/Chai.chpl"
static void wrapcoforall_fn_chpl111(_class_localscoforall_fn_chpl111 c_chpl) {
#line 530 "../lib/Chai.chpl"
  Error_chpl error_chpl = NULL;
#line 530 "../lib/Chai.chpl"
  int64_t _0_numChunks_chpl;
#line 530 "../lib/Chai.chpl"
  int64_t _1_parDim_chpl;
#line 530 "../lib/Chai.chpl"
  _tuple_1_star_range_int64_t_both_one_chpl _2_coerce_tmp_chpl;
#line 530 "../lib/Chai.chpl"
  chpl___EndCount_AtomicT_int64_t_int64_t _3__coforallCount_chpl = NULL;
#line 530 "../lib/Chai.chpl"
  int64_t _4_chunk_chpl;
#line 530 "../lib/Chai.chpl"
  _ref_SoftMax_chpl _6_this_chpl = NULL;
#line 530 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl _7_batch_chpl;
#line 530 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_1_int64_t_one_Tensor_1__real64_int64_t_chpl _8_outputs_chpl;
#line 530 "../lib/Chai.chpl"
  _ref_range_int64_t_low_one_chpl _9_call_tmp_chpl = NULL;
#line 530 "../lib/Chai.chpl"
  chpl_rmem_consist_acquire(INT64(530), INT32(65));
#line 530 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(530), INT32(65));
#line 530 "../lib/Chai.chpl"
  _0_numChunks_chpl = (c_chpl)->_0_numChunks;
#line 530 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(530), INT32(65));
#line 530 "../lib/Chai.chpl"
  _1_parDim_chpl = (c_chpl)->_1_parDim;
#line 530 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(530), INT32(65));
#line 530 "../lib/Chai.chpl"
  *(_2_coerce_tmp_chpl + INT64(0)) = *((c_chpl)->_2_coerce_tmp + INT64(0));
#line 530 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(530), INT32(65));
#line 530 "../lib/Chai.chpl"
  _3__coforallCount_chpl = (c_chpl)->_3__coforallCount;
#line 530 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(530), INT32(65));
#line 530 "../lib/Chai.chpl"
  _4_chunk_chpl = (c_chpl)->_4_chunk;
#line 530 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(530), INT32(65));
#line 530 "../lib/Chai.chpl"
  _6_this_chpl = (c_chpl)->_6_this;
#line 530 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(530), INT32(65));
#line 530 "../lib/Chai.chpl"
  _7_batch_chpl = (c_chpl)->_7_batch;
#line 530 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(530), INT32(65));
#line 530 "../lib/Chai.chpl"
  _8_outputs_chpl = (c_chpl)->_8_outputs;
#line 530 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(530), INT32(65));
#line 530 "../lib/Chai.chpl"
  _9_call_tmp_chpl = (c_chpl)->_9_call_tmp;
#line 530 "../lib/Chai.chpl"
  coforall_fn_chpl111(_0_numChunks_chpl, _1_parDim_chpl, &_2_coerce_tmp_chpl, _3__coforallCount_chpl, _4_chunk_chpl, &error_chpl, _6_this_chpl, &_7_batch_chpl, &_8_outputs_chpl, _9_call_tmp_chpl);
#line 530 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(530), INT32(65));
#line 530 "../lib/Chai.chpl"
  _downEndCount_chpl((c_chpl)->_3__coforallCount, error_chpl, INT64(530), INT32(65));
#line 530 "../lib/Chai.chpl"
  return;
#line 530 "../lib/Chai.chpl"
}

#line 530 "../lib/Chai.chpl"
/* Chai.chpl:530 */
#line 530 "../lib/Chai.chpl"
static void coforall_fn_chpl111(int64_t numChunks_chpl,
#line 530 "../lib/Chai.chpl"
                                int64_t parDim_chpl,
#line 530 "../lib/Chai.chpl"
                                _tuple_1_star_range_int64_t_both_one_chpl * locBlock_chpl,
#line 530 "../lib/Chai.chpl"
                                chpl___EndCount_AtomicT_int64_t_int64_t _coforallCount_chpl,
#line 530 "../lib/Chai.chpl"
                                int64_t chunk_chpl,
#line 530 "../lib/Chai.chpl"
                                Error_chpl * error_out_chpl,
#line 530 "../lib/Chai.chpl"
                                SoftMax_chpl * this_chpl31,
#line 530 "../lib/Chai.chpl"
                                _array_DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl * batch_chpl,
#line 530 "../lib/Chai.chpl"
                                _array_DefaultRectangularArr_1_int64_t_one_Tensor_1__real64_int64_t_chpl * outputs_chpl,
#line 530 "../lib/Chai.chpl"
                                range_int64_t_low_one_chpl * call_tmp_chpl6) {
#line 530 "../lib/Chai.chpl"
  _tuple_1_star_range_int64_t_both_one_chpl followMe_chpl;
#line 530 "../lib/Chai.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl7 = NULL;
#line 530 "../lib/Chai.chpl"
  int64_t call_tmp_chpl8;
#line 530 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl3;
#line 530 "../lib/Chai.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl9 = NULL;
#line 530 "../lib/Chai.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl10 = NULL;
#line 530 "../lib/Chai.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl11 = NULL;
#line 530 "../lib/Chai.chpl"
  _tuple_2_star_int64_t_chpl call_tmp_chpl12;
#line 530 "../lib/Chai.chpl"
  _tuple_2_star_int64_t_chpl ret_tmp_chpl;
#line 530 "../lib/Chai.chpl"
  int64_t tmp_x0_chpl;
#line 530 "../lib/Chai.chpl"
  int64_t tmp_x1_chpl;
#line 530 "../lib/Chai.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl13 = NULL;
#line 530 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl14;
#line 530 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl2;
#line 530 "../lib/Chai.chpl"
  _tuple_1_star_range_int64_t_both_one_chpl _yield_expr_tmp__chpl;
#line 530 "../lib/Chai.chpl"
  _tuple_1_star_range_int64_t_both_one_chpl chpl_followThis;
#line 530 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl _ir_F0_this_chpl;
#line 530 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl _ic__F5_block_x0_chpl;
#line 530 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl _ic__F7_coerce_tmp_x0_chpl;
#line 530 "../lib/Chai.chpl"
  int64_t _ic__F8_i_chpl;
#line 530 "../lib/Chai.chpl"
  int64_t _ic__F9_end_chpl;
#line 530 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl coerce_tmp_chpl4;
#line 530 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl _ic__F0_this_chpl;
#line 530 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl _ic__F2_myFollowThis_chpl;
#line 530 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl _ic__F3_r_chpl;
#line 530 "../lib/Chai.chpl"
  int64_t _ic__F5_i_chpl;
#line 530 "../lib/Chai.chpl"
  DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl this_chpl32 = NULL;
#line 530 "../lib/Chai.chpl"
  DefaultRectangularDom_1_int64_t_one_chpl coerce_tmp_chpl5 = NULL;
#line 530 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl6;
#line 530 "../lib/Chai.chpl"
  DefaultRectangularDom_1_int64_t_one_chpl this_chpl33 = NULL;
#line 530 "../lib/Chai.chpl"
  uint64_t call_tmp_chpl15;
#line 530 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl7;
#line 530 "../lib/Chai.chpl"
  DefaultRectangularDom_1_int64_t_one_chpl this_chpl34 = NULL;
#line 530 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl8;
#line 530 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl9;
#line 530 "../lib/Chai.chpl"
  DefaultRectangularDom_1_int64_t_one_chpl this_chpl35 = NULL;
#line 530 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl10;
#line 530 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl11;
#line 530 "../lib/Chai.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl16 = NULL;
#line 530 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl this_chpl36;
#line 530 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl this_chpl37;
#line 530 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl tmp_chpl;
#line 530 "../lib/Chai.chpl"
  DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl ret_chpl = NULL;
#line 530 "../lib/Chai.chpl"
  DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl _ic__F0_this_chpl2 = NULL;
#line 530 "../lib/Chai.chpl"
  _tuple_1_star_int64_t_chpl ret_tmp_chpl3;
#line 530 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl tup_x0_chpl;
#line 530 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl elt_x0_chpl;
#line 530 "../lib/Chai.chpl"
  int64_t coerce_tmp_chpl12;
#line 530 "../lib/Chai.chpl"
  _ref__tuple_1_star_range_int64_t_both_one_chpl call_tmp_chpl17 = NULL;
#line 530 "../lib/Chai.chpl"
  chpl_bool tmp_chpl2;
#line 530 "../lib/Chai.chpl"
  _ref__tuple_1_star_range_int64_t_both_one_chpl call_tmp_chpl18 = NULL;
#line 530 "../lib/Chai.chpl"
  int64_t call_tmp_chpl19;
#line 530 "../lib/Chai.chpl"
  _ref__tuple_1_star_range_int64_t_both_one_chpl call_tmp_chpl20 = NULL;
#line 530 "../lib/Chai.chpl"
  int64_t call_tmp_chpl21;
#line 530 "../lib/Chai.chpl"
  _tuple_1_star_range_int64_t_both_one_chpl retcopy_chpl;
#line 530 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl _ic__F0_this_chpl3;
#line 530 "../lib/Chai.chpl"
  int64_t coerce_tmp_chpl13;
#line 530 "../lib/Chai.chpl"
  int64_t call_tmp_chpl22;
#line 530 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl this_chpl38;
#line 530 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl tmp_chpl3;
#line 530 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl tmp_chpl4;
#line 530 "../lib/Chai.chpl"
  _ref_range_int64_t_both_one_chpl tmp_chpl5 = NULL;
#line 530 "../lib/Chai.chpl"
  chpl_bool tmp_chpl6;
#line 530 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl tmp_chpl7;
#line 530 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl tmp_chpl8;
#line 530 "../lib/Chai.chpl"
  int64_t call_tmp_chpl23;
#line 530 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl tmp_chpl9;
#line 530 "../lib/Chai.chpl"
  int64_t call_tmp_chpl24;
#line 530 "../lib/Chai.chpl"
  _ref_range_int64_t_both_one_chpl tmp_chpl10 = NULL;
#line 530 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl14;
#line 530 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl _ic__F0_this_chpl4;
#line 530 "../lib/Chai.chpl"
  DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl this_chpl39 = NULL;
#line 530 "../lib/Chai.chpl"
  _ddata_Tensor_3__real64_chpl coerce_tmp_chpl15 = NULL;
#line 530 "../lib/Chai.chpl"
  _ref_Tensor_3__real64_chpl call_tmp_chpl25 = NULL;
#line 531 "../lib/Chai.chpl"
  _tuple_1_star_int64_t_chpl coerce_tmp_chpl16;
#line 531 "../lib/Chai.chpl"
  _ref__tuple_1_star_int64_t_chpl i_i_chpl = NULL;
#line 531 "../lib/Chai.chpl"
  DefaultRectangularArr_1_int64_t_one_Tensor_1__real64_int64_t_chpl ret_chpl2 = NULL;
#line 531 "../lib/Chai.chpl"
  DefaultRectangularArr_1_int64_t_one_Tensor_1__real64_int64_t_chpl coerce_tmp_chpl17 = NULL;
#line 531 "../lib/Chai.chpl"
  _tuple_1_star_int64_t_chpl _formal_tmp_in_ind_chpl;
#line 531 "../lib/Chai.chpl"
  int64_t _formal_tmp_ind_x0_chpl;
#line 531 "../lib/Chai.chpl"
  _ddata_Tensor_1__real64_chpl coerce_tmp_chpl18 = NULL;
#line 531 "../lib/Chai.chpl"
  _ref_Tensor_1__real64_chpl call_tmp_chpl26 = NULL;
#line 531 "../lib/Chai.chpl"
  Tensor_1__real64_chpl call_tmp_chpl27;
#line 531 "../lib/Chai.chpl"
  Tensor_1__real64_chpl ret_tmp_chpl4;
#line 531 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_1_int64_t_one__real64_int64_t_chpl _field_destructor_tmp__chpl;
#line 531 "../lib/Chai.chpl"
  _domain_DefaultRectangularDom_1_int64_t_one_chpl _field_destructor_tmp__chpl2;
#line 530 "../lib/Chai.chpl"
  int64_t tmp_chpl11;
#line 530 "../lib/Chai.chpl"
  int64_t tmp_chpl12;
#line 530 "../lib/Chai.chpl"
  int64_t tmp_chpl13;
#line 530 "../lib/Chai.chpl"
  *(followMe_chpl + INT64(0)) = *(*(locBlock_chpl) + INT64(0));
#line 530 "../lib/Chai.chpl"
  call_tmp_chpl7 = this_chpl20(locBlock_chpl, parDim_chpl, INT64(530), INT32(65));
#line 530 "../lib/Chai.chpl"
  coerce_tmp_chpl3 = *(call_tmp_chpl7);
#line 530 "../lib/Chai.chpl"
  call_tmp_chpl8 = sizeAs_chpl37(coerce_tmp_chpl3, INT64(530), INT32(65));
#line 530 "../lib/Chai.chpl"
  call_tmp_chpl9 = this_chpl20(locBlock_chpl, parDim_chpl, INT64(530), INT32(65));
#line 530 "../lib/Chai.chpl"
  call_tmp_chpl10 = this_chpl20(locBlock_chpl, parDim_chpl, INT64(530), INT32(65));
#line 530 "../lib/Chai.chpl"
  call_tmp_chpl11 = this_chpl20(locBlock_chpl, parDim_chpl, INT64(530), INT32(65));
#line 530 "../lib/Chai.chpl"
  _computeBlock_chpl(call_tmp_chpl8, numChunks_chpl, chunk_chpl, (call_tmp_chpl9)->_high, (call_tmp_chpl10)->_low, (call_tmp_chpl11)->_low, &ret_tmp_chpl, INT64(530), INT32(65));
#line 530 "../lib/Chai.chpl"
  *(call_tmp_chpl12 + INT64(0)) = *(ret_tmp_chpl + INT64(0));
#line 530 "../lib/Chai.chpl"
  *(call_tmp_chpl12 + INT64(1)) = *(ret_tmp_chpl + INT64(1));
#line 530 "../lib/Chai.chpl"
  tmp_x0_chpl = *(call_tmp_chpl12 + INT64(0));
#line 530 "../lib/Chai.chpl"
  tmp_x1_chpl = *(call_tmp_chpl12 + INT64(1));
#line 530 "../lib/Chai.chpl"
  call_tmp_chpl13 = this_chpl20(&followMe_chpl, parDim_chpl, INT64(530), INT32(65));
#line 530 "../lib/Chai.chpl"
  chpl_build_bounded_range6(tmp_x0_chpl, tmp_x1_chpl, &ret_tmp_chpl2);
#line 530 "../lib/Chai.chpl"
  call_tmp_chpl14 = ret_tmp_chpl2;
#line 530 "../lib/Chai.chpl"
  *(call_tmp_chpl13) = call_tmp_chpl14;
#line 530 "../lib/Chai.chpl"
  *(_yield_expr_tmp__chpl + INT64(0)) = *(followMe_chpl + INT64(0));
#line 530 "../lib/Chai.chpl"
  *(chpl_followThis + INT64(0)) = *(_yield_expr_tmp__chpl + INT64(0));
#line 530 "../lib/Chai.chpl"
  _ir_F0_this_chpl = *(batch_chpl);
#line 530 "../lib/Chai.chpl"
  coerce_tmp_chpl4 = *(call_tmp_chpl6);
#line 530 "../lib/Chai.chpl"
  _ic__F0_this_chpl = coerce_tmp_chpl4;
#line 530 "../lib/Chai.chpl"
  tmp_chpl = _ir_F0_this_chpl;
#line 530 "../lib/Chai.chpl"
  ret_chpl = (&tmp_chpl)->_instance;
#line 530 "../lib/Chai.chpl"
  _ic__F0_this_chpl2 = ret_chpl;
#line 530 "../lib/Chai.chpl"
  this_chpl32 = ret_chpl;
#line 530 "../lib/Chai.chpl"
  chpl_check_nil(this_chpl32, INT64(530), INT32(65));
#line 530 "../lib/Chai.chpl"
  coerce_tmp_chpl5 = (this_chpl32)->dom;
#line 530 "../lib/Chai.chpl"
  createTuple_chpl(INT64(0), &ret_tmp_chpl3);
#line 530 "../lib/Chai.chpl"
  init_chpl113(&elt_x0_chpl);
#line 530 "../lib/Chai.chpl"
  tup_x0_chpl = elt_x0_chpl;
#line 530 "../lib/Chai.chpl"
  _ic__F5_block_x0_chpl = tup_x0_chpl;
#line 530 "../lib/Chai.chpl"
  coerce_tmp_chpl6 = *(chpl_followThis + INT64(0));
#line 530 "../lib/Chai.chpl"
  coerce_tmp_chpl12 = (&coerce_tmp_chpl6)->_high;
#line 530 "../lib/Chai.chpl"
  this_chpl33 = coerce_tmp_chpl5;
#line 530 "../lib/Chai.chpl"
  chpl_check_nil(this_chpl33, INT64(530), INT32(65));
#line 530 "../lib/Chai.chpl"
  call_tmp_chpl17 = &((this_chpl33)->ranges);
#line 530 "../lib/Chai.chpl"
  coerce_tmp_chpl7 = *(*(call_tmp_chpl17) + INT64(0));
#line 530 "../lib/Chai.chpl"
  call_tmp_chpl15 = sizeAs_chpl36(coerce_tmp_chpl7, INT64(530), INT32(65));
#line 530 "../lib/Chai.chpl"
  if (! (coerce_tmp_chpl12 < INT64(0))) /* ZLINE: 530 ../lib/Chai.chpl */
#line 530 "../lib/Chai.chpl"
  {
#line 530 "../lib/Chai.chpl"
    tmp_chpl2 = (((uint64_t)(coerce_tmp_chpl12)) >= call_tmp_chpl15);
#line 530 "../lib/Chai.chpl"
  } else /* ZLINE: 530 ../lib/Chai.chpl */
#line 530 "../lib/Chai.chpl"
  {
#line 530 "../lib/Chai.chpl"
    tmp_chpl2 = UINT8(false);
#line 530 "../lib/Chai.chpl"
  }
#line 530 "../lib/Chai.chpl"
  if (tmp_chpl2) /* ZLINE: 530 ../lib/Chai.chpl */
#line 530 "../lib/Chai.chpl"
  {
#line 530 "../lib/Chai.chpl"
    boundsCheckHalt_chpl(&_str_literal_4152_chpl, INT64(530), INT32(65));
#line 530 "../lib/Chai.chpl"
  }
#line 530 "../lib/Chai.chpl"
  this_chpl34 = coerce_tmp_chpl5;
#line 530 "../lib/Chai.chpl"
  chpl_check_nil(this_chpl34, INT64(530), INT32(65));
#line 530 "../lib/Chai.chpl"
  call_tmp_chpl18 = &((this_chpl34)->ranges);
#line 530 "../lib/Chai.chpl"
  coerce_tmp_chpl8 = *(*(call_tmp_chpl18) + INT64(0));
#line 530 "../lib/Chai.chpl"
  coerce_tmp_chpl9 = *(chpl_followThis + INT64(0));
#line 530 "../lib/Chai.chpl"
  call_tmp_chpl19 = ((&coerce_tmp_chpl8)->_low + ((int64_t)(((&coerce_tmp_chpl9)->_low * INT64(1)))));
#line 530 "../lib/Chai.chpl"
  this_chpl35 = coerce_tmp_chpl5;
#line 530 "../lib/Chai.chpl"
  chpl_check_nil(this_chpl35, INT64(530), INT32(65));
#line 530 "../lib/Chai.chpl"
  call_tmp_chpl20 = &((this_chpl35)->ranges);
#line 530 "../lib/Chai.chpl"
  coerce_tmp_chpl10 = *(*(call_tmp_chpl20) + INT64(0));
#line 530 "../lib/Chai.chpl"
  coerce_tmp_chpl11 = *(chpl_followThis + INT64(0));
#line 530 "../lib/Chai.chpl"
  call_tmp_chpl21 = ((&coerce_tmp_chpl10)->_low + ((int64_t)(((&coerce_tmp_chpl11)->_high * INT64(1)))));
#line 530 "../lib/Chai.chpl"
  call_tmp_chpl16 = &_ic__F5_block_x0_chpl;
#line 530 "../lib/Chai.chpl"
  compilerAssert_chpl();
#line 530 "../lib/Chai.chpl"
  (call_tmp_chpl16)->_low = call_tmp_chpl19;
#line 530 "../lib/Chai.chpl"
  (call_tmp_chpl16)->_high = call_tmp_chpl21;
#line 530 "../lib/Chai.chpl"
  *(retcopy_chpl + INT64(0)) = _ic__F5_block_x0_chpl;
#line 530 "../lib/Chai.chpl"
  _ic__F7_coerce_tmp_x0_chpl = *(retcopy_chpl + INT64(0));
#line 530 "../lib/Chai.chpl"
  _ic__F0_this_chpl3 = _ic__F7_coerce_tmp_x0_chpl;
#line 530 "../lib/Chai.chpl"
  checkIfIterWillOverflow_chpl(_ic__F7_coerce_tmp_x0_chpl, UINT8(true), INT64(530), INT32(65));
#line 530 "../lib/Chai.chpl"
  _ic__F8_i_chpl = INT64(0);
#line 530 "../lib/Chai.chpl"
  this_chpl36 = _ic__F0_this_chpl3;
#line 530 "../lib/Chai.chpl"
  coerce_tmp_chpl13 = (&this_chpl36)->_low;
#line 530 "../lib/Chai.chpl"
  this_chpl37 = _ic__F0_this_chpl3;
#line 530 "../lib/Chai.chpl"
  _ic__F9_end_chpl = (&this_chpl37)->_high;
#line 530 "../lib/Chai.chpl"
  tmp_chpl3 = *(chpl_followThis + INT64(0));
#line 530 "../lib/Chai.chpl"
  _ic__F2_myFollowThis_chpl = tmp_chpl3;
#line 530 "../lib/Chai.chpl"
  tmp_chpl4 = tmp_chpl3;
#line 530 "../lib/Chai.chpl"
  if ((&tmp_chpl4)->_low > (&tmp_chpl4)->_high) /* ZLINE: 530 ../lib/Chai.chpl */
#line 530 "../lib/Chai.chpl"
  {
#line 530 "../lib/Chai.chpl"
    tmp_chpl6 = UINT8(false);
#line 530 "../lib/Chai.chpl"
  } else /* ZLINE: 530 ../lib/Chai.chpl */
#line 530 "../lib/Chai.chpl"
  {
#line 530 "../lib/Chai.chpl"
    tmp_chpl6 = UINT8(true);
#line 530 "../lib/Chai.chpl"
  }
#line 530 "../lib/Chai.chpl"
  if (! tmp_chpl6) /* ZLINE: 530 ../lib/Chai.chpl */
#line 530 "../lib/Chai.chpl"
  {
#line 530 "../lib/Chai.chpl"
    tmp_chpl7 = tmp_chpl3;
#line 530 "../lib/Chai.chpl"
    if (! ((&tmp_chpl7)->_low > (&tmp_chpl7)->_high)) /* ZLINE: 530 ../lib/Chai.chpl */
#line 530 "../lib/Chai.chpl"
    {
#line 530 "../lib/Chai.chpl"
      boundsCheckHalt_chpl(&_str_literal_633_chpl, INT64(530), INT32(65));
#line 530 "../lib/Chai.chpl"
    }
#line 530 "../lib/Chai.chpl"
  }
#line 530 "../lib/Chai.chpl"
  call_tmp_chpl22 = sizeAs_chpl37(tmp_chpl3, INT64(530), INT32(65));
#line 530 "../lib/Chai.chpl"
  if (call_tmp_chpl22 != INT64(0)) /* ZLINE: 530 ../lib/Chai.chpl */
#line 530 "../lib/Chai.chpl"
  {
#line 530 "../lib/Chai.chpl"
  }
#line 530 "../lib/Chai.chpl"
  tmp_chpl5 = &_ic__F3_r_chpl;
#line 530 "../lib/Chai.chpl"
  init_chpl113(tmp_chpl5);
#line 530 "../lib/Chai.chpl"
  if (call_tmp_chpl22 != INT64(0)) /* ZLINE: 530 ../lib/Chai.chpl */
#line 530 "../lib/Chai.chpl"
  {
#line 530 "../lib/Chai.chpl"
    tmp_chpl8 = _ic__F2_myFollowThis_chpl;
#line 530 "../lib/Chai.chpl"
    call_tmp_chpl23 = orderToIndex_chpl(coerce_tmp_chpl4, (&tmp_chpl8)->_low, INT64(530), INT32(65));
#line 530 "../lib/Chai.chpl"
    tmp_chpl9 = _ic__F2_myFollowThis_chpl;
#line 530 "../lib/Chai.chpl"
    call_tmp_chpl24 = orderToIndex_chpl(_ic__F0_this_chpl, (&tmp_chpl9)->_high, INT64(530), INT32(65));
#line 530 "../lib/Chai.chpl"
    tmp_chpl10 = &_ic__F3_r_chpl;
#line 530 "../lib/Chai.chpl"
    (tmp_chpl10)->_low = call_tmp_chpl23;
#line 530 "../lib/Chai.chpl"
    (tmp_chpl10)->_high = call_tmp_chpl24;
#line 530 "../lib/Chai.chpl"
    verifyAppropriateStide_chpl2(INT64(1), INT64(530), INT32(65));
#line 530 "../lib/Chai.chpl"
  }
#line 530 "../lib/Chai.chpl"
  coerce_tmp_chpl14 = _ic__F3_r_chpl;
#line 530 "../lib/Chai.chpl"
  _ic__F0_this_chpl4 = coerce_tmp_chpl14;
#line 530 "../lib/Chai.chpl"
  checkIfIterWillOverflow_chpl(coerce_tmp_chpl14, UINT8(true), INT64(530), INT32(65));
#line 530 "../lib/Chai.chpl"
  _ic__F5_i_chpl = INT64(0);
#line 530 "../lib/Chai.chpl"
  this_chpl38 = _ic__F0_this_chpl4;
#line 530 "../lib/Chai.chpl"
  for (_ic__F8_i_chpl = coerce_tmp_chpl13,_ic__F5_i_chpl = (&this_chpl38)->_low; (tmp_chpl11 = (_ic__F8_i_chpl <= _ic__F9_end_chpl),tmp_chpl11); tmp_chpl12 = _ic__F8_i_chpl,tmp_chpl12 += INT64(1),_ic__F8_i_chpl = tmp_chpl12,tmp_chpl13 = _ic__F5_i_chpl,tmp_chpl13 += INT64(1),_ic__F5_i_chpl = tmp_chpl13) {
#line 530 "../lib/Chai.chpl"
    this_chpl39 = _ic__F0_this_chpl2;
#line 530 "../lib/Chai.chpl"
    chpl_check_nil(this_chpl39, INT64(530), INT32(65));
#line 530 "../lib/Chai.chpl"
    coerce_tmp_chpl15 = (this_chpl39)->shiftedData;
#line 530 "../lib/Chai.chpl"
    call_tmp_chpl25 = (coerce_tmp_chpl15 + _ic__F8_i_chpl);
#line 531 "../lib/Chai.chpl"
    *(coerce_tmp_chpl16 + INT64(0)) = _ic__F5_i_chpl;
#line 531 "../lib/Chai.chpl"
    i_i_chpl = &coerce_tmp_chpl16;
#line 531 "../lib/Chai.chpl"
    ret_chpl2 = (outputs_chpl)->_instance;
#line 531 "../lib/Chai.chpl"
    checkAccess_chpl2(outputs_chpl, i_i_chpl, ret_chpl2, INT64(531), INT32(65));
#line 531 "../lib/Chai.chpl"
    coerce_tmp_chpl17 = ret_chpl2;
#line 531 "../lib/Chai.chpl"
    *(_formal_tmp_in_ind_chpl + INT64(0)) = *(coerce_tmp_chpl16 + INT64(0));
#line 531 "../lib/Chai.chpl"
    _formal_tmp_ind_x0_chpl = *(_formal_tmp_in_ind_chpl + INT64(0));
#line 531 "../lib/Chai.chpl"
    chpl_check_nil(coerce_tmp_chpl17, INT64(531), INT32(65));
#line 531 "../lib/Chai.chpl"
    coerce_tmp_chpl18 = (coerce_tmp_chpl17)->shiftedData;
#line 531 "../lib/Chai.chpl"
    call_tmp_chpl26 = (coerce_tmp_chpl18 + _formal_tmp_ind_x0_chpl);
#line 531 "../lib/Chai.chpl"
    forwardProp_chpl3(this_chpl31, call_tmp_chpl25, &ret_tmp_chpl4);
#line 531 "../lib/Chai.chpl"
    call_tmp_chpl27 = ret_tmp_chpl4;
#line 531 "../lib/Chai.chpl"
    chpl___ASSIGN_25(call_tmp_chpl26, &call_tmp_chpl27);
#line 531 "../lib/Chai.chpl"
    _field_destructor_tmp__chpl = (&call_tmp_chpl27)->data;
#line 531 "../lib/Chai.chpl"
    chpl__autoDestroy18(&_field_destructor_tmp__chpl, INT64(531), INT32(65));
#line 531 "../lib/Chai.chpl"
    _field_destructor_tmp__chpl2 = (&call_tmp_chpl27)->_domain;
#line 531 "../lib/Chai.chpl"
    chpl__autoDestroy11(&_field_destructor_tmp__chpl2, INT64(531), INT32(65));
#line 531 "../lib/Chai.chpl"
  }
#line 530 "../lib/Chai.chpl"
  return;
#line 530 "../lib/Chai.chpl"
}

#line 537 "../lib/Chai.chpl"
/* Chai.chpl:537 */
#line 537 "../lib/Chai.chpl"
static void forwardProp_chpl3(SoftMax_chpl * this_chpl31,
#line 537 "../lib/Chai.chpl"
                              Tensor_3__real64_chpl * input_chpl,
#line 537 "../lib/Chai.chpl"
                              Tensor_1__real64_chpl * _retArg_chpl) {
#line 542 "../lib/Chai.chpl"
  Tensor_1__real64_chpl call_tmp_chpl6;
#line 542 "../lib/Chai.chpl"
  Tensor_1__real64_chpl ret_tmp_chpl;
#line 542 "../lib/Chai.chpl"
  Tensor_1__real64_chpl flattened_chpl;
#line 543 "../lib/Chai.chpl"
  _ref_Tensor_2__real64_chpl call_tmp_chpl7 = NULL;
#line 543 "../lib/Chai.chpl"
  Tensor_1__real64_chpl call_tmp_chpl8;
#line 543 "../lib/Chai.chpl"
  Tensor_1__real64_chpl ret_tmp_chpl2;
#line 543 "../lib/Chai.chpl"
  _ref_Tensor_1__real64_chpl call_tmp_chpl9 = NULL;
#line 543 "../lib/Chai.chpl"
  Tensor_1__real64_chpl call_tmp_chpl10;
#line 543 "../lib/Chai.chpl"
  Tensor_1__real64_chpl ret_tmp_chpl3;
#line 543 "../lib/Chai.chpl"
  Tensor_1__real64_chpl z_chpl;
#line 543 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_1_int64_t_one__real64_int64_t_chpl _field_destructor_tmp__chpl;
#line 543 "../lib/Chai.chpl"
  _domain_DefaultRectangularDom_1_int64_t_one_chpl _field_destructor_tmp__chpl2;
#line 544 "../lib/Chai.chpl"
  Tensor_1__real64_chpl call_tmp_chpl11;
#line 544 "../lib/Chai.chpl"
  Tensor_1__real64_chpl ret_tmp_chpl4;
#line 544 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_1_int64_t_one__real64_int64_t_chpl _field_destructor_tmp__chpl3;
#line 544 "../lib/Chai.chpl"
  _domain_DefaultRectangularDom_1_int64_t_one_chpl _field_destructor_tmp__chpl4;
#line 544 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_1_int64_t_one__real64_int64_t_chpl _field_destructor_tmp__chpl5;
#line 544 "../lib/Chai.chpl"
  _domain_DefaultRectangularDom_1_int64_t_one_chpl _field_destructor_tmp__chpl6;
#line 538 "../lib/Chai.chpl"
  debugWrite_chpl(&_str_literal_1719_chpl);
#line 540 "../lib/Chai.chpl"
  if ((this_chpl31)->uninitialized) /* ZLINE: 540 ../lib/Chai.chpl */
#line 540 "../lib/Chai.chpl"
  {
#line 540 "../lib/Chai.chpl"
    initialize_chpl(this_chpl31, input_chpl);
#line 540 "../lib/Chai.chpl"
  }
#line 542 "../lib/Chai.chpl"
  flatten_chpl(input_chpl, &ret_tmp_chpl);
#line 542 "../lib/Chai.chpl"
  call_tmp_chpl6 = ret_tmp_chpl;
#line 542 "../lib/Chai.chpl"
  flattened_chpl = call_tmp_chpl6;
#line 543 "../lib/Chai.chpl"
  call_tmp_chpl7 = &((this_chpl31)->weights);
#line 543 "../lib/Chai.chpl"
  chpl___ASTERISK_4(call_tmp_chpl7, &flattened_chpl, &ret_tmp_chpl2);
#line 543 "../lib/Chai.chpl"
  call_tmp_chpl8 = ret_tmp_chpl2;
#line 543 "../lib/Chai.chpl"
  call_tmp_chpl9 = &((this_chpl31)->biases);
#line 543 "../lib/Chai.chpl"
  chpl___PLUS_2(&call_tmp_chpl8, call_tmp_chpl9, &ret_tmp_chpl3);
#line 543 "../lib/Chai.chpl"
  call_tmp_chpl10 = ret_tmp_chpl3;
#line 543 "../lib/Chai.chpl"
  z_chpl = call_tmp_chpl10;
#line 543 "../lib/Chai.chpl"
  _field_destructor_tmp__chpl = (&call_tmp_chpl8)->data;
#line 543 "../lib/Chai.chpl"
  chpl__autoDestroy18(&_field_destructor_tmp__chpl, INT64(543), INT32(65));
#line 543 "../lib/Chai.chpl"
  _field_destructor_tmp__chpl2 = (&call_tmp_chpl8)->_domain;
#line 543 "../lib/Chai.chpl"
  chpl__autoDestroy11(&_field_destructor_tmp__chpl2, INT64(543), INT32(65));
#line 544 "../lib/Chai.chpl"
  softmax_chpl(&z_chpl, &ret_tmp_chpl4);
#line 544 "../lib/Chai.chpl"
  call_tmp_chpl11 = ret_tmp_chpl4;
#line 544 "../lib/Chai.chpl"
  _field_destructor_tmp__chpl3 = (&z_chpl)->data;
#line 544 "../lib/Chai.chpl"
  chpl__autoDestroy18(&_field_destructor_tmp__chpl3, INT64(544), INT32(65));
#line 544 "../lib/Chai.chpl"
  _field_destructor_tmp__chpl4 = (&z_chpl)->_domain;
#line 544 "../lib/Chai.chpl"
  chpl__autoDestroy11(&_field_destructor_tmp__chpl4, INT64(544), INT32(65));
#line 544 "../lib/Chai.chpl"
  _field_destructor_tmp__chpl5 = (&flattened_chpl)->data;
#line 544 "../lib/Chai.chpl"
  chpl__autoDestroy18(&_field_destructor_tmp__chpl5, INT64(544), INT32(65));
#line 544 "../lib/Chai.chpl"
  _field_destructor_tmp__chpl6 = (&flattened_chpl)->_domain;
#line 544 "../lib/Chai.chpl"
  chpl__autoDestroy11(&_field_destructor_tmp__chpl6, INT64(544), INT32(65));
#line 537 "../lib/Chai.chpl"
  *(_retArg_chpl) = call_tmp_chpl11;
#line 537 "../lib/Chai.chpl"
  return;
#line 537 "../lib/Chai.chpl"
}

#line 581 "../lib/Chai.chpl"
/* Chai.chpl:581 */
#line 581 "../lib/Chai.chpl"
static void backwardBatch_chpl3(SoftMax_chpl * this_chpl31,
#line 581 "../lib/Chai.chpl"
                                _array_DefaultRectangularArr_1_int64_t_one_Tensor_1__real64_int64_t_chpl * deltas_chpl,
#line 581 "../lib/Chai.chpl"
                                _array_DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl * inputs_chpl,
#line 581 "../lib/Chai.chpl"
                                _array_DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl * _retArg_chpl) {
#line 584 "../lib/Chai.chpl"
  int64_t local_dataParMinGranularity_chpl;
#line 584 "../lib/Chai.chpl"
  chpl_bool local_dataParIgnoreRunningTasks_chpl;
#line 584 "../lib/Chai.chpl"
  int64_t local_dataParTasksPerLocale_chpl;
#line 581 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl ret_chpl;
#line 582 "../lib/Chai.chpl"
  int64_t call_tmp_chpl6;
#line 583 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl newDeltas_chpl;
#line 583 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl call_tmp_chpl7;
#line 583 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl ret_tmp_chpl;
#line 583 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl8;
#line 583 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl2;
#line 583 "../lib/Chai.chpl"
  _domain_DefaultRectangularDom_1_int64_t_one_chpl call_tmp_chpl9;
#line 583 "../lib/Chai.chpl"
  _domain_DefaultRectangularDom_1_int64_t_one_chpl ret_tmp_chpl3;
#line 583 "../lib/Chai.chpl"
  chpl___RuntimeTypeInfo6 call_tmp_chpl10;
#line 583 "../lib/Chai.chpl"
  _domain_DefaultRectangularDom_1_int64_t_one_chpl _runtime_type_tmp__chpl;
#line 583 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl ret_tmp_chpl4;
#line 584 "../lib/Chai.chpl"
  _tuple_1_star_int64_t_chpl coerce_tmp_chpl3;
#line 584 "../lib/Chai.chpl"
  _ref__tuple_1_star_int64_t_chpl i_i_chpl = NULL;
#line 584 "../lib/Chai.chpl"
  DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl ret_chpl2 = NULL;
#line 584 "../lib/Chai.chpl"
  DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl coerce_tmp_chpl4 = NULL;
#line 584 "../lib/Chai.chpl"
  _tuple_1_star_int64_t_chpl _formal_tmp_in_ind_chpl;
#line 584 "../lib/Chai.chpl"
  int64_t _formal_tmp_ind_x0_chpl;
#line 584 "../lib/Chai.chpl"
  _ddata_Tensor_3__real64_chpl coerce_tmp_chpl5 = NULL;
#line 584 "../lib/Chai.chpl"
  _ref_Tensor_3__real64_chpl call_tmp_chpl11 = NULL;
#line 584 "../lib/Chai.chpl"
  _domain_DefaultRectangularDom_3_int64_t_one_chpl call_tmp_chpl12;
#line 584 "../lib/Chai.chpl"
  _domain_DefaultRectangularDom_3_int64_t_one_chpl ret_tmp_chpl5;
#line 584 "../lib/Chai.chpl"
  _ir_chpl_promo7_reshapeDomain__ref__array_DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl ret_tmp_chpl6;
#line 584 "../lib/Chai.chpl"
  _ir_chpl_promo7_reshapeDomain__ref__array_DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl chpl__iterLF;
#line 584 "../lib/Chai.chpl"
  Error_chpl error_chpl = NULL;
#line 584 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl ftemp_chpl;
#line 584 "../lib/Chai.chpl"
  DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl coerce_tmp_chpl6 = NULL;
#line 584 "../lib/Chai.chpl"
  int64_t tasksPerLocale_chpl;
#line 584 "../lib/Chai.chpl"
  chpl_bool ignoreRunning_chpl;
#line 584 "../lib/Chai.chpl"
  int64_t minIndicesPerTask_chpl;
#line 584 "../lib/Chai.chpl"
  Error_chpl error_chpl2 = NULL;
#line 584 "../lib/Chai.chpl"
  DefaultRectangularDom_1_int64_t_one_chpl coerce_tmp_chpl7 = NULL;
#line 584 "../lib/Chai.chpl"
  int64_t offset_x0_chpl;
#line 584 "../lib/Chai.chpl"
  _tuple_1_star_int64_t_chpl ret_tmp_chpl7;
#line 584 "../lib/Chai.chpl"
  Error_chpl error_chpl3 = NULL;
#line 584 "../lib/Chai.chpl"
  locale_chpl call_tmp_chpl13;
#line 584 "../lib/Chai.chpl"
  chpl_localeID_t call_tmp_chpl14;
#line 584 "../lib/Chai.chpl"
  int32_t call_tmp_chpl15;
#line 584 "../lib/Chai.chpl"
  chpl_localeID_t call_tmp_chpl16;
#line 584 "../lib/Chai.chpl"
  locale_chpl call_tmp_chpl17;
#line 584 "../lib/Chai.chpl"
  locale_chpl ret_tmp_chpl8;
#line 584 "../lib/Chai.chpl"
  BaseLocale_chpl coerce_tmp_chpl8 = NULL;
#line 584 "../lib/Chai.chpl"
  _ref_locale_chpl i_x_chpl = NULL;
#line 584 "../lib/Chai.chpl"
  int64_t tmp_chpl;
#line 584 "../lib/Chai.chpl"
  locale_chpl call_tmp_chpl18;
#line 584 "../lib/Chai.chpl"
  chpl_localeID_t call_tmp_chpl19;
#line 584 "../lib/Chai.chpl"
  int32_t call_tmp_chpl20;
#line 584 "../lib/Chai.chpl"
  chpl_localeID_t call_tmp_chpl21;
#line 584 "../lib/Chai.chpl"
  locale_chpl call_tmp_chpl22;
#line 584 "../lib/Chai.chpl"
  locale_chpl ret_tmp_chpl9;
#line 584 "../lib/Chai.chpl"
  int64_t ret_chpl3;
#line 584 "../lib/Chai.chpl"
  BaseLocale_chpl coerce_tmp_chpl9 = NULL;
#line 584 "../lib/Chai.chpl"
  BaseLocale_chpl coerce_tmp_chpl10 = NULL;
#line 584 "../lib/Chai.chpl"
  _ref_locale_chpl i_x_chpl2 = NULL;
#line 584 "../lib/Chai.chpl"
  int64_t tmp_x0_chpl;
#line 584 "../lib/Chai.chpl"
  int64_t tmp_x1_chpl;
#line 584 "../lib/Chai.chpl"
  _ref__tuple_1_star_range_int64_t_both_one_chpl call_tmp_chpl23 = NULL;
#line 584 "../lib/Chai.chpl"
  _tuple_2_star_int64_t_chpl ret_tmp_chpl10;
#line 584 "../lib/Chai.chpl"
  int64_t numChunks_chpl;
#line 584 "../lib/Chai.chpl"
  int64_t parDim_chpl;
#line 584 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl locBlock_x0_chpl;
#line 584 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl tup_x0_chpl;
#line 584 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl elt_x0_chpl;
#line 584 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl call_tmp_chpl24;
#line 584 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl ret_tmp_chpl11;
#line 584 "../lib/Chai.chpl"
  _ref__tuple_1_star_range_int64_t_both_one_chpl call_tmp_chpl25 = NULL;
#line 584 "../lib/Chai.chpl"
  int64_t call_tmp_chpl26;
#line 584 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl11;
#line 584 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl27;
#line 584 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl12;
#line 584 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl tmpIter_chpl;
#line 584 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl call_tmp_chpl28;
#line 584 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl ret_tmp_chpl13;
#line 584 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl14;
#line 584 "../lib/Chai.chpl"
  chpl___EndCount_AtomicT_int64_t_int64_t _coforallCount_chpl = NULL;
#line 584 "../lib/Chai.chpl"
  chpl___EndCount_AtomicT_int64_t_int64_t new_temp_chpl = NULL;
#line 584 "../lib/Chai.chpl"
  int64_t numTasks_chpl;
#line 584 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl _ic__F0_this_chpl;
#line 584 "../lib/Chai.chpl"
  int64_t i_chpl;
#line 584 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl this_chpl32;
#line 584 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl this_chpl33;
#line 584 "../lib/Chai.chpl"
  int64_t end_chpl;
#line 584 "../lib/Chai.chpl"
  _tuple_1_star_range_int64_t_both_one_chpl ret_tmp_chpl15;
#line 584 "../lib/Chai.chpl"
  _class_localscoforall_fn_chpl112 _args_forcoforall_fn_chpl = NULL;
#line 584 "../lib/Chai.chpl"
  _ref__ir_chpl_promo7_reshapeDomain__ref__array_DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl tmp_chpl2 = NULL;
#line 584 "../lib/Chai.chpl"
  raw_c_void_ptr _args_vforcoforall_fn_chpl;
#line 584 "../lib/Chai.chpl"
  Error_chpl error_chpl4 = NULL;
#line 586 "../lib/Chai.chpl"
  _ref_Tensor_2__real64_chpl call_tmp_chpl29 = NULL;
#line 586 "../lib/Chai.chpl"
  Tensor_2__real64_chpl weightsGrad_chpl;
#line 587 "../lib/Chai.chpl"
  _ref_Tensor_1__real64_chpl call_tmp_chpl30 = NULL;
#line 587 "../lib/Chai.chpl"
  Tensor_1__real64_chpl biasesGrad_chpl;
#line 588 "../lib/Chai.chpl"
  _domain_DefaultRectangularDom_1_int64_t_one_chpl call_tmp_chpl31;
#line 588 "../lib/Chai.chpl"
  _domain_DefaultRectangularDom_1_int64_t_one_chpl ret_tmp_chpl16;
#line 588 "../lib/Chai.chpl"
  SumReduceScanOp_Tensor_2__real64_chpl globalRP_weightsGrad_chpl = NULL;
#line 588 "../lib/Chai.chpl"
  SumReduceScanOp_Tensor_2__real64_chpl new_temp_chpl2 = NULL;
#line 588 "../lib/Chai.chpl"
  chpl_LocalSpinlock default_arg_l_chpl;
#line 588 "../lib/Chai.chpl"
  chpl_LocalSpinlock ret_chpl4;
#line 588 "../lib/Chai.chpl"
  chpl_LocalSpinlock l_chpl;
#line 588 "../lib/Chai.chpl"
  Tensor_2__real64_chpl default_arg_value_chpl;
#line 588 "../lib/Chai.chpl"
  Tensor_2__real64_chpl ret_chpl5;
#line 588 "../lib/Chai.chpl"
  Tensor_2__real64_chpl value_chpl;
#line 588 "../lib/Chai.chpl"
  SumReduceScanOp_Tensor_2__real64_chpl coerce_tmp_chpl12 = NULL;
#line 588 "../lib/Chai.chpl"
  _ref_Tensor_2__real64_chpl i_x_chpl3 = NULL;
#line 588 "../lib/Chai.chpl"
  _ref_Tensor_2__real64_chpl call_tmp_chpl32 = NULL;
#line 588 "../lib/Chai.chpl"
  Tensor_2__real64_chpl ret_chpl6;
#line 588 "../lib/Chai.chpl"
  Tensor_2__real64_chpl x_chpl;
#line 588 "../lib/Chai.chpl"
  Tensor_2__real64_chpl globalAS_weightsGrad_chpl;
#line 588 "../lib/Chai.chpl"
  SumReduceScanOp_Tensor_1__real64_chpl globalRP_biasesGrad_chpl = NULL;
#line 588 "../lib/Chai.chpl"
  SumReduceScanOp_Tensor_1__real64_chpl new_temp_chpl3 = NULL;
#line 588 "../lib/Chai.chpl"
  chpl_LocalSpinlock default_arg_l_chpl2;
#line 588 "../lib/Chai.chpl"
  chpl_LocalSpinlock ret_chpl7;
#line 588 "../lib/Chai.chpl"
  chpl_LocalSpinlock l_chpl2;
#line 588 "../lib/Chai.chpl"
  Tensor_1__real64_chpl default_arg_value_chpl2;
#line 588 "../lib/Chai.chpl"
  Tensor_1__real64_chpl ret_chpl8;
#line 588 "../lib/Chai.chpl"
  Tensor_1__real64_chpl value_chpl2;
#line 588 "../lib/Chai.chpl"
  SumReduceScanOp_Tensor_1__real64_chpl coerce_tmp_chpl13 = NULL;
#line 588 "../lib/Chai.chpl"
  _ref_Tensor_1__real64_chpl i_x_chpl4 = NULL;
#line 588 "../lib/Chai.chpl"
  _ref_Tensor_1__real64_chpl call_tmp_chpl33 = NULL;
#line 588 "../lib/Chai.chpl"
  Tensor_1__real64_chpl ret_chpl9;
#line 588 "../lib/Chai.chpl"
  Tensor_1__real64_chpl x_chpl2;
#line 588 "../lib/Chai.chpl"
  Tensor_1__real64_chpl globalAS_biasesGrad_chpl;
#line 588 "../lib/Chai.chpl"
  DefaultRectangularArr_1_int64_t_one_Tensor_1__real64_int64_t_chpl coerce_tmp_chpl14 = NULL;
#line 588 "../lib/Chai.chpl"
  int64_t tasksPerLocale_chpl2;
#line 588 "../lib/Chai.chpl"
  chpl_bool ignoreRunning_chpl2;
#line 588 "../lib/Chai.chpl"
  int64_t minIndicesPerTask_chpl2;
#line 588 "../lib/Chai.chpl"
  Error_chpl error_chpl5 = NULL;
#line 588 "../lib/Chai.chpl"
  DefaultRectangularDom_1_int64_t_one_chpl coerce_tmp_chpl15 = NULL;
#line 588 "../lib/Chai.chpl"
  int64_t offset_x0_chpl2;
#line 588 "../lib/Chai.chpl"
  _tuple_1_star_int64_t_chpl ret_tmp_chpl17;
#line 588 "../lib/Chai.chpl"
  Error_chpl error_chpl6 = NULL;
#line 588 "../lib/Chai.chpl"
  locale_chpl call_tmp_chpl34;
#line 588 "../lib/Chai.chpl"
  chpl_localeID_t call_tmp_chpl35;
#line 588 "../lib/Chai.chpl"
  int32_t call_tmp_chpl36;
#line 588 "../lib/Chai.chpl"
  chpl_localeID_t call_tmp_chpl37;
#line 588 "../lib/Chai.chpl"
  locale_chpl call_tmp_chpl38;
#line 588 "../lib/Chai.chpl"
  locale_chpl ret_tmp_chpl18;
#line 588 "../lib/Chai.chpl"
  BaseLocale_chpl coerce_tmp_chpl16 = NULL;
#line 588 "../lib/Chai.chpl"
  _ref_locale_chpl i_x_chpl5 = NULL;
#line 588 "../lib/Chai.chpl"
  int64_t tmp_chpl3;
#line 588 "../lib/Chai.chpl"
  locale_chpl call_tmp_chpl39;
#line 588 "../lib/Chai.chpl"
  chpl_localeID_t call_tmp_chpl40;
#line 588 "../lib/Chai.chpl"
  int32_t call_tmp_chpl41;
#line 588 "../lib/Chai.chpl"
  chpl_localeID_t call_tmp_chpl42;
#line 588 "../lib/Chai.chpl"
  locale_chpl call_tmp_chpl43;
#line 588 "../lib/Chai.chpl"
  locale_chpl ret_tmp_chpl19;
#line 588 "../lib/Chai.chpl"
  int64_t ret_chpl10;
#line 588 "../lib/Chai.chpl"
  BaseLocale_chpl coerce_tmp_chpl17 = NULL;
#line 588 "../lib/Chai.chpl"
  BaseLocale_chpl coerce_tmp_chpl18 = NULL;
#line 588 "../lib/Chai.chpl"
  _ref_locale_chpl i_x_chpl6 = NULL;
#line 588 "../lib/Chai.chpl"
  int64_t tmp_x0_chpl2;
#line 588 "../lib/Chai.chpl"
  int64_t tmp_x1_chpl2;
#line 588 "../lib/Chai.chpl"
  _ref__tuple_1_star_range_int64_t_both_one_chpl call_tmp_chpl44 = NULL;
#line 588 "../lib/Chai.chpl"
  _tuple_2_star_int64_t_chpl ret_tmp_chpl20;
#line 588 "../lib/Chai.chpl"
  int64_t numChunks_chpl2;
#line 588 "../lib/Chai.chpl"
  int64_t parDim_chpl2;
#line 588 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl locBlock_x0_chpl2;
#line 588 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl tup_x0_chpl2;
#line 588 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl elt_x0_chpl2;
#line 588 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl call_tmp_chpl45;
#line 588 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl ret_tmp_chpl21;
#line 588 "../lib/Chai.chpl"
  _ref__tuple_1_star_range_int64_t_both_one_chpl call_tmp_chpl46 = NULL;
#line 588 "../lib/Chai.chpl"
  int64_t call_tmp_chpl47;
#line 588 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl19;
#line 588 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl48;
#line 588 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl22;
#line 588 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl tmpIter_chpl2;
#line 588 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl call_tmp_chpl49;
#line 588 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl ret_tmp_chpl23;
#line 588 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl24;
#line 588 "../lib/Chai.chpl"
  chpl___EndCount_AtomicT_int64_t_int64_t _coforallCount_chpl2 = NULL;
#line 588 "../lib/Chai.chpl"
  chpl___EndCount_AtomicT_int64_t_int64_t new_temp_chpl4 = NULL;
#line 588 "../lib/Chai.chpl"
  int64_t numTasks_chpl2;
#line 588 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl _ic__F0_this_chpl2;
#line 588 "../lib/Chai.chpl"
  int64_t i_chpl2;
#line 588 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl this_chpl34;
#line 588 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl this_chpl35;
#line 588 "../lib/Chai.chpl"
  int64_t end_chpl2;
#line 588 "../lib/Chai.chpl"
  _tuple_1_star_range_int64_t_both_one_chpl ret_tmp_chpl25;
#line 588 "../lib/Chai.chpl"
  _class_localscoforall_fn_chpl113 _args_forcoforall_fn_chpl2 = NULL;
#line 588 "../lib/Chai.chpl"
  _ref_SoftMax_chpl tmp_chpl4 = NULL;
#line 588 "../lib/Chai.chpl"
  raw_c_void_ptr _args_vforcoforall_fn_chpl2;
#line 588 "../lib/Chai.chpl"
  Error_chpl error_chpl7 = NULL;
#line 588 "../lib/Chai.chpl"
  SumReduceScanOp_Tensor_1__real64_chpl coerce_tmp_chpl20 = NULL;
#line 588 "../lib/Chai.chpl"
  _ref_Tensor_1__real64_chpl i_x_chpl7 = NULL;
#line 588 "../lib/Chai.chpl"
  _ref_Tensor_1__real64_chpl call_tmp_chpl50 = NULL;
#line 588 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_1_int64_t_one__real64_int64_t_chpl _field_destructor_tmp__chpl;
#line 588 "../lib/Chai.chpl"
  _domain_DefaultRectangularDom_1_int64_t_one_chpl _field_destructor_tmp__chpl2;
#line 588 "../lib/Chai.chpl"
  Tensor_1__real64_chpl chpl_gentemp;
#line 588 "../lib/Chai.chpl"
  SumReduceScanOp_Tensor_1__real64_chpl coerce_tmp_chpl21 = NULL;
#line 588 "../lib/Chai.chpl"
  _ref_Tensor_1__real64_chpl call_tmp_chpl51 = NULL;
#line 588 "../lib/Chai.chpl"
  Tensor_1__real64_chpl ret_chpl11;
#line 588 "../lib/Chai.chpl"
  Tensor_1__real64_chpl tmp_chpl5;
#line 588 "../lib/Chai.chpl"
  SumReduceScanOp_Tensor_2__real64_chpl coerce_tmp_chpl22 = NULL;
#line 588 "../lib/Chai.chpl"
  _ref_Tensor_2__real64_chpl i_x_chpl8 = NULL;
#line 588 "../lib/Chai.chpl"
  _ref_Tensor_2__real64_chpl call_tmp_chpl52 = NULL;
#line 588 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_2_int64_t_one__real64_int64_t_chpl _field_destructor_tmp__chpl3;
#line 588 "../lib/Chai.chpl"
  _domain_DefaultRectangularDom_2_int64_t_one_chpl _field_destructor_tmp__chpl4;
#line 588 "../lib/Chai.chpl"
  Tensor_2__real64_chpl chpl_gentemp2;
#line 588 "../lib/Chai.chpl"
  SumReduceScanOp_Tensor_2__real64_chpl coerce_tmp_chpl23 = NULL;
#line 588 "../lib/Chai.chpl"
  _ref_Tensor_2__real64_chpl call_tmp_chpl53 = NULL;
#line 588 "../lib/Chai.chpl"
  Tensor_2__real64_chpl ret_chpl12;
#line 588 "../lib/Chai.chpl"
  Tensor_2__real64_chpl tmp_chpl6;
#line 612 "../lib/Chai.chpl"
  _ref_Tensor_2__real64_chpl call_tmp_chpl54 = NULL;
#line 612 "../lib/Chai.chpl"
  _ref__array_DefaultRectangularArr_2_int64_t_one__real64_int64_t_chpl call_tmp_chpl55 = NULL;
#line 612 "../lib/Chai.chpl"
  _ref__array_DefaultRectangularArr_2_int64_t_one__real64_int64_t_chpl call_tmp_chpl56 = NULL;
#line 612 "../lib/Chai.chpl"
  uint64_t call_tmp_chpl57;
#line 612 "../lib/Chai.chpl"
  chpl_bool tmp_chpl7;
#line 612 "../lib/Chai.chpl"
  chpl_bool tmp_chpl8;
#line 612 "../lib/Chai.chpl"
  uint64_t call_tmp_chpl58;
#line 612 "../lib/Chai.chpl"
  chpl_bool tmp_chpl9;
#line 612 "../lib/Chai.chpl"
  _domain_DefaultRectangularDom_2_int64_t_one_chpl call_tmp_chpl59;
#line 612 "../lib/Chai.chpl"
  _domain_DefaultRectangularDom_2_int64_t_one_chpl ret_tmp_chpl26;
#line 612 "../lib/Chai.chpl"
  _domain_DefaultRectangularDom_2_int64_t_one_chpl call_tmp_chpl60;
#line 612 "../lib/Chai.chpl"
  _domain_DefaultRectangularDom_2_int64_t_one_chpl ret_tmp_chpl27;
#line 612 "../lib/Chai.chpl"
  _ref__domain_DefaultRectangularDom_2_int64_t_one_chpl i_AD_chpl = NULL;
#line 612 "../lib/Chai.chpl"
  _ref__domain_DefaultRectangularDom_2_int64_t_one_chpl i_BD_chpl = NULL;
#line 612 "../lib/Chai.chpl"
  DefaultRectangularArr_2_int64_t_one__real64_int64_t_chpl ret_chpl13 = NULL;
#line 612 "../lib/Chai.chpl"
  DefaultRectangularArr_2_int64_t_one__real64_int64_t_chpl ret_chpl14 = NULL;
#line 612 "../lib/Chai.chpl"
  chpl_bool call_tmp_chpl61;
#line 612 "../lib/Chai.chpl"
  DefaultRectangularArr_2_int64_t_one__real64_int64_t_chpl coerce_tmp_chpl24 = NULL;
#line 612 "../lib/Chai.chpl"
  int64_t tasksPerLocale_chpl3;
#line 612 "../lib/Chai.chpl"
  chpl_bool ignoreRunning_chpl3;
#line 612 "../lib/Chai.chpl"
  int64_t minIndicesPerTask_chpl3;
#line 612 "../lib/Chai.chpl"
  Error_chpl error_chpl8 = NULL;
#line 612 "../lib/Chai.chpl"
  DefaultRectangularDom_2_int64_t_one_chpl coerce_tmp_chpl25 = NULL;
#line 612 "../lib/Chai.chpl"
  int64_t offset_x0_chpl3;
#line 612 "../lib/Chai.chpl"
  int64_t offset_x1_chpl;
#line 612 "../lib/Chai.chpl"
  _tuple_2_star_int64_t_chpl ret_tmp_chpl28;
#line 612 "../lib/Chai.chpl"
  Error_chpl error_chpl9 = NULL;
#line 612 "../lib/Chai.chpl"
  locale_chpl call_tmp_chpl62;
#line 612 "../lib/Chai.chpl"
  chpl_localeID_t call_tmp_chpl63;
#line 612 "../lib/Chai.chpl"
  int32_t call_tmp_chpl64;
#line 612 "../lib/Chai.chpl"
  chpl_localeID_t call_tmp_chpl65;
#line 612 "../lib/Chai.chpl"
  locale_chpl call_tmp_chpl66;
#line 612 "../lib/Chai.chpl"
  locale_chpl ret_tmp_chpl29;
#line 612 "../lib/Chai.chpl"
  BaseLocale_chpl coerce_tmp_chpl26 = NULL;
#line 612 "../lib/Chai.chpl"
  _ref_locale_chpl i_x_chpl9 = NULL;
#line 612 "../lib/Chai.chpl"
  int64_t tmp_chpl10;
#line 612 "../lib/Chai.chpl"
  locale_chpl call_tmp_chpl67;
#line 612 "../lib/Chai.chpl"
  chpl_localeID_t call_tmp_chpl68;
#line 612 "../lib/Chai.chpl"
  int32_t call_tmp_chpl69;
#line 612 "../lib/Chai.chpl"
  chpl_localeID_t call_tmp_chpl70;
#line 612 "../lib/Chai.chpl"
  locale_chpl call_tmp_chpl71;
#line 612 "../lib/Chai.chpl"
  locale_chpl ret_tmp_chpl30;
#line 612 "../lib/Chai.chpl"
  int64_t ret_chpl15;
#line 612 "../lib/Chai.chpl"
  BaseLocale_chpl coerce_tmp_chpl27 = NULL;
#line 612 "../lib/Chai.chpl"
  BaseLocale_chpl coerce_tmp_chpl28 = NULL;
#line 612 "../lib/Chai.chpl"
  _ref_locale_chpl i_x_chpl10 = NULL;
#line 612 "../lib/Chai.chpl"
  int64_t tmp_x0_chpl3;
#line 612 "../lib/Chai.chpl"
  int64_t tmp_x1_chpl3;
#line 612 "../lib/Chai.chpl"
  _ref__tuple_2_star_range_int64_t_both_one_chpl call_tmp_chpl72 = NULL;
#line 612 "../lib/Chai.chpl"
  _tuple_2_star_int64_t_chpl ret_tmp_chpl31;
#line 612 "../lib/Chai.chpl"
  int64_t numChunks_chpl3;
#line 612 "../lib/Chai.chpl"
  int64_t parDim_chpl3;
#line 612 "../lib/Chai.chpl"
  _tuple_2_star_range_int64_t_both_one_chpl locBlock_chpl;
#line 612 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl tup_x0_chpl3;
#line 612 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl tup_x1_chpl;
#line 612 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl elt_x0_chpl3;
#line 612 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl elt_x1_chpl;
#line 612 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl call_tmp_chpl73;
#line 612 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl ret_tmp_chpl32;
#line 612 "../lib/Chai.chpl"
  _ref__tuple_2_star_range_int64_t_both_one_chpl call_tmp_chpl74 = NULL;
#line 612 "../lib/Chai.chpl"
  int64_t call_tmp_chpl75;
#line 612 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl29;
#line 612 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl76;
#line 612 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl33;
#line 612 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl call_tmp_chpl77;
#line 612 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl ret_tmp_chpl34;
#line 612 "../lib/Chai.chpl"
  _ref__tuple_2_star_range_int64_t_both_one_chpl call_tmp_chpl78 = NULL;
#line 612 "../lib/Chai.chpl"
  int64_t call_tmp_chpl79;
#line 612 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl30;
#line 612 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl80;
#line 612 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl35;
#line 612 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl tmpIter_chpl3;
#line 612 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl call_tmp_chpl81;
#line 612 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl ret_tmp_chpl36;
#line 612 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl37;
#line 612 "../lib/Chai.chpl"
  chpl___EndCount_AtomicT_int64_t_int64_t _coforallCount_chpl3 = NULL;
#line 612 "../lib/Chai.chpl"
  chpl___EndCount_AtomicT_int64_t_int64_t new_temp_chpl5 = NULL;
#line 612 "../lib/Chai.chpl"
  int64_t numTasks_chpl3;
#line 612 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl _ic__F0_this_chpl3;
#line 612 "../lib/Chai.chpl"
  int64_t i_chpl3;
#line 612 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl this_chpl36;
#line 612 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl this_chpl37;
#line 612 "../lib/Chai.chpl"
  int64_t end_chpl3;
#line 612 "../lib/Chai.chpl"
  _class_localscoforall_fn_chpl18 _args_forcoforall_fn_chpl3 = NULL;
#line 612 "../lib/Chai.chpl"
  raw_c_void_ptr _args_vforcoforall_fn_chpl3;
#line 612 "../lib/Chai.chpl"
  int64_t _args_sizecoforall_fn_chpl;
#line 612 "../lib/Chai.chpl"
  Error_chpl error_chpl10 = NULL;
#line 612 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_2_int64_t_one__real64_int64_t_chpl _field_destructor_tmp__chpl5;
#line 612 "../lib/Chai.chpl"
  _domain_DefaultRectangularDom_2_int64_t_one_chpl _field_destructor_tmp__chpl6;
#line 612 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_1_int64_t_one__real64_int64_t_chpl _field_destructor_tmp__chpl7;
#line 612 "../lib/Chai.chpl"
  _domain_DefaultRectangularDom_1_int64_t_one_chpl _field_destructor_tmp__chpl8;
#line 613 "../lib/Chai.chpl"
  _ref_Tensor_1__real64_chpl call_tmp_chpl82 = NULL;
#line 613 "../lib/Chai.chpl"
  _ref__array_DefaultRectangularArr_1_int64_t_one__real64_int64_t_chpl call_tmp_chpl83 = NULL;
#line 613 "../lib/Chai.chpl"
  _ref__array_DefaultRectangularArr_1_int64_t_one__real64_int64_t_chpl call_tmp_chpl84 = NULL;
#line 613 "../lib/Chai.chpl"
  uint64_t call_tmp_chpl85;
#line 613 "../lib/Chai.chpl"
  chpl_bool tmp_chpl11;
#line 613 "../lib/Chai.chpl"
  chpl_bool tmp_chpl12;
#line 613 "../lib/Chai.chpl"
  uint64_t call_tmp_chpl86;
#line 613 "../lib/Chai.chpl"
  chpl_bool tmp_chpl13;
#line 613 "../lib/Chai.chpl"
  _domain_DefaultRectangularDom_1_int64_t_one_chpl call_tmp_chpl87;
#line 613 "../lib/Chai.chpl"
  _domain_DefaultRectangularDom_1_int64_t_one_chpl ret_tmp_chpl38;
#line 613 "../lib/Chai.chpl"
  _domain_DefaultRectangularDom_1_int64_t_one_chpl call_tmp_chpl88;
#line 613 "../lib/Chai.chpl"
  _domain_DefaultRectangularDom_1_int64_t_one_chpl ret_tmp_chpl39;
#line 613 "../lib/Chai.chpl"
  _ref__domain_DefaultRectangularDom_1_int64_t_one_chpl i_AD_chpl2 = NULL;
#line 613 "../lib/Chai.chpl"
  _ref__domain_DefaultRectangularDom_1_int64_t_one_chpl i_BD_chpl2 = NULL;
#line 613 "../lib/Chai.chpl"
  DefaultRectangularArr_1_int64_t_one__real64_int64_t_chpl ret_chpl16 = NULL;
#line 613 "../lib/Chai.chpl"
  DefaultRectangularArr_1_int64_t_one__real64_int64_t_chpl ret_chpl17 = NULL;
#line 613 "../lib/Chai.chpl"
  chpl_bool call_tmp_chpl89;
#line 613 "../lib/Chai.chpl"
  DefaultRectangularArr_1_int64_t_one__real64_int64_t_chpl coerce_tmp_chpl31 = NULL;
#line 613 "../lib/Chai.chpl"
  int64_t tasksPerLocale_chpl4;
#line 613 "../lib/Chai.chpl"
  chpl_bool ignoreRunning_chpl4;
#line 613 "../lib/Chai.chpl"
  int64_t minIndicesPerTask_chpl4;
#line 613 "../lib/Chai.chpl"
  Error_chpl error_chpl11 = NULL;
#line 613 "../lib/Chai.chpl"
  DefaultRectangularDom_1_int64_t_one_chpl coerce_tmp_chpl32 = NULL;
#line 613 "../lib/Chai.chpl"
  int64_t offset_x0_chpl4;
#line 613 "../lib/Chai.chpl"
  _tuple_1_star_int64_t_chpl ret_tmp_chpl40;
#line 613 "../lib/Chai.chpl"
  Error_chpl error_chpl12 = NULL;
#line 613 "../lib/Chai.chpl"
  locale_chpl call_tmp_chpl90;
#line 613 "../lib/Chai.chpl"
  chpl_localeID_t call_tmp_chpl91;
#line 613 "../lib/Chai.chpl"
  int32_t call_tmp_chpl92;
#line 613 "../lib/Chai.chpl"
  chpl_localeID_t call_tmp_chpl93;
#line 613 "../lib/Chai.chpl"
  locale_chpl call_tmp_chpl94;
#line 613 "../lib/Chai.chpl"
  locale_chpl ret_tmp_chpl41;
#line 613 "../lib/Chai.chpl"
  BaseLocale_chpl coerce_tmp_chpl33 = NULL;
#line 613 "../lib/Chai.chpl"
  _ref_locale_chpl i_x_chpl11 = NULL;
#line 613 "../lib/Chai.chpl"
  int64_t tmp_chpl14;
#line 613 "../lib/Chai.chpl"
  locale_chpl call_tmp_chpl95;
#line 613 "../lib/Chai.chpl"
  chpl_localeID_t call_tmp_chpl96;
#line 613 "../lib/Chai.chpl"
  int32_t call_tmp_chpl97;
#line 613 "../lib/Chai.chpl"
  chpl_localeID_t call_tmp_chpl98;
#line 613 "../lib/Chai.chpl"
  locale_chpl call_tmp_chpl99;
#line 613 "../lib/Chai.chpl"
  locale_chpl ret_tmp_chpl42;
#line 613 "../lib/Chai.chpl"
  int64_t ret_chpl18;
#line 613 "../lib/Chai.chpl"
  BaseLocale_chpl coerce_tmp_chpl34 = NULL;
#line 613 "../lib/Chai.chpl"
  BaseLocale_chpl coerce_tmp_chpl35 = NULL;
#line 613 "../lib/Chai.chpl"
  _ref_locale_chpl i_x_chpl12 = NULL;
#line 613 "../lib/Chai.chpl"
  int64_t tmp_x0_chpl4;
#line 613 "../lib/Chai.chpl"
  int64_t tmp_x1_chpl4;
#line 613 "../lib/Chai.chpl"
  _ref__tuple_1_star_range_int64_t_both_one_chpl call_tmp_chpl100 = NULL;
#line 613 "../lib/Chai.chpl"
  _tuple_2_star_int64_t_chpl ret_tmp_chpl43;
#line 613 "../lib/Chai.chpl"
  int64_t numChunks_chpl4;
#line 613 "../lib/Chai.chpl"
  int64_t parDim_chpl4;
#line 613 "../lib/Chai.chpl"
  _tuple_1_star_range_int64_t_both_one_chpl locBlock_chpl2;
#line 613 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl tup_x0_chpl4;
#line 613 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl elt_x0_chpl4;
#line 613 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl call_tmp_chpl101;
#line 613 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl ret_tmp_chpl44;
#line 613 "../lib/Chai.chpl"
  _ref__tuple_1_star_range_int64_t_both_one_chpl call_tmp_chpl102 = NULL;
#line 613 "../lib/Chai.chpl"
  int64_t call_tmp_chpl103;
#line 613 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl36;
#line 613 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl104;
#line 613 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl45;
#line 613 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl tmpIter_chpl4;
#line 613 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl call_tmp_chpl105;
#line 613 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl ret_tmp_chpl46;
#line 613 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl47;
#line 613 "../lib/Chai.chpl"
  chpl___EndCount_AtomicT_int64_t_int64_t _coforallCount_chpl4 = NULL;
#line 613 "../lib/Chai.chpl"
  chpl___EndCount_AtomicT_int64_t_int64_t new_temp_chpl6 = NULL;
#line 613 "../lib/Chai.chpl"
  int64_t numTasks_chpl4;
#line 613 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl _ic__F0_this_chpl4;
#line 613 "../lib/Chai.chpl"
  int64_t i_chpl4;
#line 613 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl this_chpl38;
#line 613 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl this_chpl39;
#line 613 "../lib/Chai.chpl"
  int64_t end_chpl4;
#line 613 "../lib/Chai.chpl"
  _class_localscoforall_fn_chpl24 _args_forcoforall_fn_chpl4 = NULL;
#line 613 "../lib/Chai.chpl"
  raw_c_void_ptr _args_vforcoforall_fn_chpl4;
#line 613 "../lib/Chai.chpl"
  int64_t _args_sizecoforall_fn_chpl2;
#line 613 "../lib/Chai.chpl"
  Error_chpl error_chpl13 = NULL;
#line 614 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_1_int64_t_one__real64_int64_t_chpl _field_destructor_tmp__chpl9;
#line 614 "../lib/Chai.chpl"
  _domain_DefaultRectangularDom_1_int64_t_one_chpl _field_destructor_tmp__chpl10;
#line 614 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_2_int64_t_one__real64_int64_t_chpl _field_destructor_tmp__chpl11;
#line 614 "../lib/Chai.chpl"
  _domain_DefaultRectangularDom_2_int64_t_one_chpl _field_destructor_tmp__chpl12;
#line 614 "../lib/Chai.chpl"
  chpl__class_localscoforall_fn_chpl112_object chpl_macro_tmp_117;
#line 614 "../lib/Chai.chpl"
  chpl__class_localscoforall_fn_chpl113_object chpl_macro_tmp_118;
#line 614 "../lib/Chai.chpl"
  chpl__class_localscoforall_fn_chpl18_object chpl_macro_tmp_119;
#line 614 "../lib/Chai.chpl"
  chpl__class_localscoforall_fn_chpl24_object chpl_macro_tmp_120;
#line 584 "../lib/Chai.chpl"
  local_dataParMinGranularity_chpl = dataParMinGranularity_chpl;
#line 584 "../lib/Chai.chpl"
  local_dataParIgnoreRunningTasks_chpl = dataParIgnoreRunningTasks_chpl;
#line 584 "../lib/Chai.chpl"
  local_dataParTasksPerLocale_chpl = dataParTasksPerLocale_chpl;
#line 582 "../lib/Chai.chpl"
  call_tmp_chpl6 = size_chpl3(deltas_chpl, INT64(582), INT32(65));
#line 583 "../lib/Chai.chpl"
  chpl_build_low_bounded_range2(INT64(0), &ret_tmp_chpl);
#line 583 "../lib/Chai.chpl"
  call_tmp_chpl7 = ret_tmp_chpl;
#line 583 "../lib/Chai.chpl"
  chpl___POUND_3(call_tmp_chpl7, call_tmp_chpl6, &ret_tmp_chpl2, INT64(583), INT32(65));
#line 583 "../lib/Chai.chpl"
  call_tmp_chpl8 = ret_tmp_chpl2;
#line 583 "../lib/Chai.chpl"
  chpl__ensureDomainExpr5(call_tmp_chpl8, &ret_tmp_chpl3, INT64(583), INT32(65));
#line 583 "../lib/Chai.chpl"
  call_tmp_chpl9 = ret_tmp_chpl3;
#line 583 "../lib/Chai.chpl"
  call_tmp_chpl10 = chpl__buildArrayRuntimeType2(&call_tmp_chpl9);
#line 583 "../lib/Chai.chpl"
  _runtime_type_tmp__chpl = (&call_tmp_chpl10)->dom;
#line 583 "../lib/Chai.chpl"
  chpl__convertRuntimeTypeToValue3(&_runtime_type_tmp__chpl, UINT8(false), &ret_tmp_chpl4, INT64(583), INT32(65));
#line 583 "../lib/Chai.chpl"
  newDeltas_chpl = ret_tmp_chpl4;
#line 584 "../lib/Chai.chpl"
  *(coerce_tmp_chpl3 + INT64(0)) = INT64(0);
#line 584 "../lib/Chai.chpl"
  i_i_chpl = &coerce_tmp_chpl3;
#line 584 "../lib/Chai.chpl"
  ret_chpl2 = (inputs_chpl)->_instance;
#line 584 "../lib/Chai.chpl"
  checkAccess_chpl10(inputs_chpl, i_i_chpl, ret_chpl2, INT64(584), INT32(65));
#line 584 "../lib/Chai.chpl"
  coerce_tmp_chpl4 = ret_chpl2;
#line 584 "../lib/Chai.chpl"
  *(_formal_tmp_in_ind_chpl + INT64(0)) = *(coerce_tmp_chpl3 + INT64(0));
#line 584 "../lib/Chai.chpl"
  _formal_tmp_ind_x0_chpl = *(_formal_tmp_in_ind_chpl + INT64(0));
#line 584 "../lib/Chai.chpl"
  chpl_check_nil(coerce_tmp_chpl4, INT64(584), INT32(65));
#line 584 "../lib/Chai.chpl"
  coerce_tmp_chpl5 = (coerce_tmp_chpl4)->shiftedData;
#line 584 "../lib/Chai.chpl"
  call_tmp_chpl11 = (coerce_tmp_chpl5 + _formal_tmp_ind_x0_chpl);
#line 584 "../lib/Chai.chpl"
  _dom_chpl22(call_tmp_chpl11, &ret_tmp_chpl5);
#line 584 "../lib/Chai.chpl"
  call_tmp_chpl12 = ret_tmp_chpl5;
#line 584 "../lib/Chai.chpl"
  (&ret_tmp_chpl6)->F0_this = newDeltas_chpl;
#line 584 "../lib/Chai.chpl"
  (&ret_tmp_chpl6)->F1_d = call_tmp_chpl12;
#line 584 "../lib/Chai.chpl"
  chpl__iterLF = ret_tmp_chpl6;
#line 584 "../lib/Chai.chpl"
  error_chpl = NULL;
#line 584 "../lib/Chai.chpl"
  ftemp_chpl = newDeltas_chpl;
#line 584 "../lib/Chai.chpl"
  coerce_tmp_chpl6 = (&ftemp_chpl)->_instance;
#line 584 "../lib/Chai.chpl"
  tasksPerLocale_chpl = local_dataParTasksPerLocale_chpl;
#line 584 "../lib/Chai.chpl"
  ignoreRunning_chpl = local_dataParIgnoreRunningTasks_chpl;
#line 584 "../lib/Chai.chpl"
  minIndicesPerTask_chpl = local_dataParMinGranularity_chpl;
#line 584 "../lib/Chai.chpl"
  error_chpl2 = NULL;
#line 584 "../lib/Chai.chpl"
  chpl_check_nil(coerce_tmp_chpl6, INT64(584), INT32(65));
#line 584 "../lib/Chai.chpl"
  coerce_tmp_chpl7 = (coerce_tmp_chpl6)->dom;
#line 584 "../lib/Chai.chpl"
  createTuple_chpl(INT64(0), &ret_tmp_chpl7);
#line 584 "../lib/Chai.chpl"
  offset_x0_chpl = *(ret_tmp_chpl7 + INT64(0));
#line 584 "../lib/Chai.chpl"
  error_chpl3 = NULL;
#line 584 "../lib/Chai.chpl"
  call_tmp_chpl15 = get_chpl_nodeID();
#line 584 "../lib/Chai.chpl"
  call_tmp_chpl16 = chpl_rt_buildLocaleID(call_tmp_chpl15, c_sublocid_any);
#line 584 "../lib/Chai.chpl"
  call_tmp_chpl14 = call_tmp_chpl16;
#line 584 "../lib/Chai.chpl"
  chpl_localeID_to_locale(&call_tmp_chpl14, &ret_tmp_chpl8, INT64(584), INT32(65));
#line 584 "../lib/Chai.chpl"
  call_tmp_chpl17 = ret_tmp_chpl8;
#line 584 "../lib/Chai.chpl"
  call_tmp_chpl13 = call_tmp_chpl17;
#line 584 "../lib/Chai.chpl"
  coerce_tmp_chpl8 = (&call_tmp_chpl13)->_instance;
#line 584 "../lib/Chai.chpl"
  if (((RootClass_chpl)(coerce_tmp_chpl8)) == nil) /* ZLINE: 584 ../lib/Chai.chpl */
#line 584 "../lib/Chai.chpl"
  {
#line 584 "../lib/Chai.chpl"
    nilCheckHalt_chpl(&_str_literal_52_chpl, INT64(584), INT32(65));
#line 584 "../lib/Chai.chpl"
  }
#line 584 "../lib/Chai.chpl"
  chpl_check_nil(coerce_tmp_chpl8, INT64(584), INT32(65));
#line 584 "../lib/Chai.chpl"
  ((int64_t(*)(BaseLocale_chpl,
#line 584 "../lib/Chai.chpl"
     int64_t,
#line 584 "../lib/Chai.chpl"
     int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(coerce_tmp_chpl8))->chpl__cid) + INT64(2))])(coerce_tmp_chpl8, INT64(584), INT32(65));
#line 584 "../lib/Chai.chpl"
  i_x_chpl = &call_tmp_chpl13;
#line 584 "../lib/Chai.chpl"
  deinit_chpl62(i_x_chpl);
#line 584 "../lib/Chai.chpl"
  if (tasksPerLocale_chpl == INT64(0)) /* ZLINE: 584 ../lib/Chai.chpl */
#line 584 "../lib/Chai.chpl"
  {
#line 584 "../lib/Chai.chpl"
    call_tmp_chpl20 = get_chpl_nodeID();
#line 584 "../lib/Chai.chpl"
    call_tmp_chpl21 = chpl_rt_buildLocaleID(call_tmp_chpl20, c_sublocid_any);
#line 584 "../lib/Chai.chpl"
    call_tmp_chpl19 = call_tmp_chpl21;
#line 584 "../lib/Chai.chpl"
    chpl_localeID_to_locale(&call_tmp_chpl19, &ret_tmp_chpl9, INT64(584), INT32(65));
#line 584 "../lib/Chai.chpl"
    call_tmp_chpl22 = ret_tmp_chpl9;
#line 584 "../lib/Chai.chpl"
    call_tmp_chpl18 = call_tmp_chpl22;
#line 584 "../lib/Chai.chpl"
    coerce_tmp_chpl9 = (&call_tmp_chpl18)->_instance;
#line 584 "../lib/Chai.chpl"
    if (((RootClass_chpl)(coerce_tmp_chpl9)) == nil) /* ZLINE: 584 ../lib/Chai.chpl */
#line 584 "../lib/Chai.chpl"
    {
#line 584 "../lib/Chai.chpl"
      nilCheckHalt_chpl(&_str_literal_52_chpl, INT64(584), INT32(65));
#line 584 "../lib/Chai.chpl"
    }
#line 584 "../lib/Chai.chpl"
    coerce_tmp_chpl10 = coerce_tmp_chpl9;
#line 584 "../lib/Chai.chpl"
    chpl_check_nil(coerce_tmp_chpl10, INT64(584), INT32(65));
#line 584 "../lib/Chai.chpl"
    ret_chpl3 = (coerce_tmp_chpl10)->maxTaskPar;
#line 584 "../lib/Chai.chpl"
    tmp_chpl = ret_chpl3;
#line 584 "../lib/Chai.chpl"
    i_x_chpl2 = &call_tmp_chpl18;
#line 584 "../lib/Chai.chpl"
    deinit_chpl62(i_x_chpl2);
#line 584 "../lib/Chai.chpl"
  } else /* ZLINE: 584 ../lib/Chai.chpl */
#line 584 "../lib/Chai.chpl"
  {
#line 584 "../lib/Chai.chpl"
    tmp_chpl = tasksPerLocale_chpl;
#line 584 "../lib/Chai.chpl"
  }
#line 584 "../lib/Chai.chpl"
  if (chpl_task_getSerial()) /* ZLINE: 584 ../lib/Chai.chpl */
#line 584 "../lib/Chai.chpl"
  {
#line 584 "../lib/Chai.chpl"
    tmp_x0_chpl = INT64(1);
#line 584 "../lib/Chai.chpl"
    tmp_x1_chpl = INT64(0);
#line 584 "../lib/Chai.chpl"
  } else /* ZLINE: 584 ../lib/Chai.chpl */
#line 584 "../lib/Chai.chpl"
  {
#line 584 "../lib/Chai.chpl"
    chpl_check_nil(coerce_tmp_chpl7, INT64(584), INT32(65));
#line 584 "../lib/Chai.chpl"
    call_tmp_chpl23 = &((coerce_tmp_chpl7)->ranges);
#line 584 "../lib/Chai.chpl"
    _computeChunkStuff_chpl(tmp_chpl, ignoreRunning_chpl, minIndicesPerTask_chpl, call_tmp_chpl23, &ret_tmp_chpl10, INT64(584), INT32(65));
#line 584 "../lib/Chai.chpl"
    tmp_x0_chpl = *(ret_tmp_chpl10 + INT64(0));
#line 584 "../lib/Chai.chpl"
    tmp_x1_chpl = *(ret_tmp_chpl10 + INT64(1));
#line 584 "../lib/Chai.chpl"
  }
#line 584 "../lib/Chai.chpl"
  numChunks_chpl = tmp_x0_chpl;
#line 584 "../lib/Chai.chpl"
  parDim_chpl = tmp_x1_chpl;
#line 584 "../lib/Chai.chpl"
  init_chpl113(&elt_x0_chpl);
#line 584 "../lib/Chai.chpl"
  tup_x0_chpl = elt_x0_chpl;
#line 584 "../lib/Chai.chpl"
  locBlock_x0_chpl = tup_x0_chpl;
#line 584 "../lib/Chai.chpl"
  chpl_build_low_bounded_range2(offset_x0_chpl, &ret_tmp_chpl11);
#line 584 "../lib/Chai.chpl"
  call_tmp_chpl24 = ret_tmp_chpl11;
#line 584 "../lib/Chai.chpl"
  chpl_check_nil(coerce_tmp_chpl7, INT64(584), INT32(65));
#line 584 "../lib/Chai.chpl"
  call_tmp_chpl25 = &((coerce_tmp_chpl7)->ranges);
#line 584 "../lib/Chai.chpl"
  coerce_tmp_chpl11 = *(*(call_tmp_chpl25) + INT64(0));
#line 584 "../lib/Chai.chpl"
  call_tmp_chpl26 = sizeAs_chpl37(coerce_tmp_chpl11, INT64(584), INT32(65));
#line 584 "../lib/Chai.chpl"
  chpl___POUND_3(call_tmp_chpl24, call_tmp_chpl26, &ret_tmp_chpl12, INT64(584), INT32(65));
#line 584 "../lib/Chai.chpl"
  call_tmp_chpl27 = ret_tmp_chpl12;
#line 584 "../lib/Chai.chpl"
  locBlock_x0_chpl = call_tmp_chpl27;
#line 584 "../lib/Chai.chpl"
  chpl_build_low_bounded_range2(INT64(0), &ret_tmp_chpl13);
#line 584 "../lib/Chai.chpl"
  call_tmp_chpl28 = ret_tmp_chpl13;
#line 584 "../lib/Chai.chpl"
  chpl___POUND_3(call_tmp_chpl28, tmp_x0_chpl, &ret_tmp_chpl14, INT64(584), INT32(65));
#line 584 "../lib/Chai.chpl"
  tmpIter_chpl = ret_tmp_chpl14;
#line 584 "../lib/Chai.chpl"
  new_temp_chpl = _new_chpl17(INT64(584), INT32(65));
#line 584 "../lib/Chai.chpl"
  _coforallCount_chpl = new_temp_chpl;
#line 584 "../lib/Chai.chpl"
  numTasks_chpl = chpl_boundedCoforallSize(tmpIter_chpl, INT64(584), INT32(65));
#line 584 "../lib/Chai.chpl"
  _upEndCount_chpl(_coforallCount_chpl, numTasks_chpl, INT64(584), INT32(65));
#line 584 "../lib/Chai.chpl"
  chpl_resetTaskSpawn(numTasks_chpl, INT64(584), INT32(65));
#line 584 "../lib/Chai.chpl"
  _ic__F0_this_chpl = tmpIter_chpl;
#line 584 "../lib/Chai.chpl"
  checkIfIterWillOverflow_chpl(tmpIter_chpl, UINT8(true), INT64(584), INT32(65));
#line 584 "../lib/Chai.chpl"
  i_chpl = INT64(0);
#line 584 "../lib/Chai.chpl"
  this_chpl32 = _ic__F0_this_chpl;
#line 584 "../lib/Chai.chpl"
  this_chpl33 = _ic__F0_this_chpl;
#line 584 "../lib/Chai.chpl"
  end_chpl = (&this_chpl33)->_high;
#line 584 "../lib/Chai.chpl"
  for (i_chpl = (&this_chpl32)->_low; ((i_chpl <= end_chpl)); i_chpl += INT64(1)) {
#line 584 "../lib/Chai.chpl"
    *(ret_tmp_chpl15 + INT64(0)) = locBlock_x0_chpl;
#line 584 "../lib/Chai.chpl"
    _args_forcoforall_fn_chpl = ((_class_localscoforall_fn_chpl112)(&chpl_macro_tmp_117));
#line 584 "../lib/Chai.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl, INT64(584), INT32(65));
#line 584 "../lib/Chai.chpl"
    (_args_forcoforall_fn_chpl)->_0_numChunks = numChunks_chpl;
#line 584 "../lib/Chai.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl, INT64(584), INT32(65));
#line 584 "../lib/Chai.chpl"
    (_args_forcoforall_fn_chpl)->_1_parDim = parDim_chpl;
#line 584 "../lib/Chai.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl, INT64(584), INT32(65));
#line 584 "../lib/Chai.chpl"
    *((_args_forcoforall_fn_chpl)->_2_coerce_tmp + INT64(0)) = *(ret_tmp_chpl15 + INT64(0));
#line 584 "../lib/Chai.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl, INT64(584), INT32(65));
#line 584 "../lib/Chai.chpl"
    (_args_forcoforall_fn_chpl)->_3__coforallCount = _coforallCount_chpl;
#line 584 "../lib/Chai.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl, INT64(584), INT32(65));
#line 584 "../lib/Chai.chpl"
    (_args_forcoforall_fn_chpl)->_4_chunk = i_chpl;
#line 584 "../lib/Chai.chpl"
    tmp_chpl2 = &chpl__iterLF;
#line 584 "../lib/Chai.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl, INT64(584), INT32(65));
#line 584 "../lib/Chai.chpl"
    (_args_forcoforall_fn_chpl)->_6_chpl__iterLF = tmp_chpl2;
#line 584 "../lib/Chai.chpl"
    _args_vforcoforall_fn_chpl = ((void*)(_args_forcoforall_fn_chpl));
#line 584 "../lib/Chai.chpl"
    /*** wrapcoforall_fn_chpl112 ***/ chpl_taskAddCoStmt(chpl_task_getRequestedSubloc(), INT64(111), ((chpl_task_bundle_p)(_args_vforcoforall_fn_chpl)), sizeof(chpl__class_localscoforall_fn_chpl112_object), INT64(584), INT32(65));
#line 584 "../lib/Chai.chpl"
  }
#line 584 "../lib/Chai.chpl"
  error_chpl4 = NULL;
#line 584 "../lib/Chai.chpl"
  _waitEndCount_chpl2(_coforallCount_chpl, numTasks_chpl, &error_chpl4, INT64(584), INT32(65));
#line 584 "../lib/Chai.chpl"
  if (error_chpl4 != nil) /* ZLINE: 584 ../lib/Chai.chpl */
#line 584 "../lib/Chai.chpl"
  {
#line 584 "../lib/Chai.chpl"
    chpl__delete72(_coforallCount_chpl, INT64(584), INT32(65));
#line 584 "../lib/Chai.chpl"
    error_chpl3 = error_chpl4;
#line 584 "../lib/Chai.chpl"
    goto forall_IBB_break_label_chpl;
#line 584 "../lib/Chai.chpl"
  }
#line 584 "../lib/Chai.chpl"
  chpl__delete72(_coforallCount_chpl, INT64(584), INT32(65));
#line 584 "../lib/Chai.chpl"
  chpl_after_forall_fence();
#line 584 "../lib/Chai.chpl"
  forall_IBB_break_label_chpl:;
#line 584 "../lib/Chai.chpl"
  if (error_chpl3 != nil) /* ZLINE: 584 ../lib/Chai.chpl */
#line 584 "../lib/Chai.chpl"
  {
#line 584 "../lib/Chai.chpl"
    error_chpl2 = error_chpl3;
#line 584 "../lib/Chai.chpl"
    goto forall_IBB_break_label_chpl2;
#line 584 "../lib/Chai.chpl"
  }
#line 584 "../lib/Chai.chpl"
  chpl_after_forall_fence();
#line 584 "../lib/Chai.chpl"
  forall_IBB_break_label_chpl2:;
#line 584 "../lib/Chai.chpl"
  if (error_chpl2 != nil) /* ZLINE: 584 ../lib/Chai.chpl */
#line 584 "../lib/Chai.chpl"
  {
#line 584 "../lib/Chai.chpl"
    error_chpl = error_chpl2;
#line 584 "../lib/Chai.chpl"
    goto forall_IBB_break_label_chpl3;
#line 584 "../lib/Chai.chpl"
  }
#line 584 "../lib/Chai.chpl"
  chpl_after_forall_fence();
#line 584 "../lib/Chai.chpl"
  forall_IBB_break_label_chpl3:;
#line 584 "../lib/Chai.chpl"
  if (error_chpl != nil) /* ZLINE: 584 ../lib/Chai.chpl */
#line 584 "../lib/Chai.chpl"
  {
#line 584 "../lib/Chai.chpl"
    chpl_propagate_error(error_chpl, INT64(584), INT32(65));
#line 584 "../lib/Chai.chpl"
  }
#line 584 "../lib/Chai.chpl"
  chpl_after_forall_fence();
#line 584 "../lib/Chai.chpl"
  chpl__autoDestroy8(&call_tmp_chpl12, INT64(584), INT32(65));
#line 586 "../lib/Chai.chpl"
  call_tmp_chpl29 = &((this_chpl31)->weightsGrad);
#line 586 "../lib/Chai.chpl"
  init_ASSIGN__chpl31(&weightsGrad_chpl, call_tmp_chpl29);
#line 587 "../lib/Chai.chpl"
  call_tmp_chpl30 = &((this_chpl31)->biasesGrad);
#line 587 "../lib/Chai.chpl"
  init_ASSIGN__chpl32(&biasesGrad_chpl, call_tmp_chpl30);
#line 588 "../lib/Chai.chpl"
  _dom_chpl6(&newDeltas_chpl, &ret_tmp_chpl16, INT64(588), INT32(65));
#line 588 "../lib/Chai.chpl"
  call_tmp_chpl31 = ret_tmp_chpl16;
#line 588 "../lib/Chai.chpl"
  init_chpl105(&l_chpl);
#line 588 "../lib/Chai.chpl"
  ret_chpl4 = l_chpl;
#line 588 "../lib/Chai.chpl"
  default_arg_l_chpl = ret_chpl4;
#line 588 "../lib/Chai.chpl"
  init_chpl236(&value_chpl);
#line 588 "../lib/Chai.chpl"
  ret_chpl5 = value_chpl;
#line 588 "../lib/Chai.chpl"
  default_arg_value_chpl = ret_chpl5;
#line 588 "../lib/Chai.chpl"
  new_temp_chpl2 = _new_chpl22(&default_arg_l_chpl, &default_arg_value_chpl, INT64(588), INT32(65));
#line 588 "../lib/Chai.chpl"
  globalRP_weightsGrad_chpl = new_temp_chpl2;
#line 588 "../lib/Chai.chpl"
  coerce_tmp_chpl12 = globalRP_weightsGrad_chpl;
#line 588 "../lib/Chai.chpl"
  i_x_chpl3 = &weightsGrad_chpl;
#line 588 "../lib/Chai.chpl"
  chpl_check_nil(coerce_tmp_chpl12, INT64(588), INT32(65));
#line 588 "../lib/Chai.chpl"
  call_tmp_chpl32 = &((coerce_tmp_chpl12)->value);
#line 588 "../lib/Chai.chpl"
  chpl___PLUS__ASSIGN_2(call_tmp_chpl32, i_x_chpl3);
#line 588 "../lib/Chai.chpl"
  init_chpl236(&x_chpl);
#line 588 "../lib/Chai.chpl"
  ret_chpl6 = x_chpl;
#line 588 "../lib/Chai.chpl"
  globalAS_weightsGrad_chpl = ret_chpl6;
#line 588 "../lib/Chai.chpl"
  init_chpl105(&l_chpl2);
#line 588 "../lib/Chai.chpl"
  ret_chpl7 = l_chpl2;
#line 588 "../lib/Chai.chpl"
  default_arg_l_chpl2 = ret_chpl7;
#line 588 "../lib/Chai.chpl"
  init_chpl235(&value_chpl2);
#line 588 "../lib/Chai.chpl"
  ret_chpl8 = value_chpl2;
#line 588 "../lib/Chai.chpl"
  default_arg_value_chpl2 = ret_chpl8;
#line 588 "../lib/Chai.chpl"
  new_temp_chpl3 = _new_chpl24(&default_arg_l_chpl2, &default_arg_value_chpl2, INT64(588), INT32(65));
#line 588 "../lib/Chai.chpl"
  globalRP_biasesGrad_chpl = new_temp_chpl3;
#line 588 "../lib/Chai.chpl"
  coerce_tmp_chpl13 = globalRP_biasesGrad_chpl;
#line 588 "../lib/Chai.chpl"
  i_x_chpl4 = &biasesGrad_chpl;
#line 588 "../lib/Chai.chpl"
  chpl_check_nil(coerce_tmp_chpl13, INT64(588), INT32(65));
#line 588 "../lib/Chai.chpl"
  call_tmp_chpl33 = &((coerce_tmp_chpl13)->value);
#line 588 "../lib/Chai.chpl"
  chpl___PLUS__ASSIGN_4(call_tmp_chpl33, i_x_chpl4);
#line 588 "../lib/Chai.chpl"
  init_chpl235(&x_chpl2);
#line 588 "../lib/Chai.chpl"
  ret_chpl9 = x_chpl2;
#line 588 "../lib/Chai.chpl"
  globalAS_biasesGrad_chpl = ret_chpl9;
#line 588 "../lib/Chai.chpl"
  coerce_tmp_chpl14 = (deltas_chpl)->_instance;
#line 588 "../lib/Chai.chpl"
  tasksPerLocale_chpl2 = local_dataParTasksPerLocale_chpl;
#line 588 "../lib/Chai.chpl"
  ignoreRunning_chpl2 = local_dataParIgnoreRunningTasks_chpl;
#line 588 "../lib/Chai.chpl"
  minIndicesPerTask_chpl2 = local_dataParMinGranularity_chpl;
#line 588 "../lib/Chai.chpl"
  error_chpl5 = NULL;
#line 588 "../lib/Chai.chpl"
  chpl_check_nil(coerce_tmp_chpl14, INT64(588), INT32(65));
#line 588 "../lib/Chai.chpl"
  coerce_tmp_chpl15 = (coerce_tmp_chpl14)->dom;
#line 588 "../lib/Chai.chpl"
  createTuple_chpl(INT64(0), &ret_tmp_chpl17);
#line 588 "../lib/Chai.chpl"
  offset_x0_chpl2 = *(ret_tmp_chpl17 + INT64(0));
#line 588 "../lib/Chai.chpl"
  error_chpl6 = NULL;
#line 588 "../lib/Chai.chpl"
  call_tmp_chpl36 = get_chpl_nodeID();
#line 588 "../lib/Chai.chpl"
  call_tmp_chpl37 = chpl_rt_buildLocaleID(call_tmp_chpl36, c_sublocid_any);
#line 588 "../lib/Chai.chpl"
  call_tmp_chpl35 = call_tmp_chpl37;
#line 588 "../lib/Chai.chpl"
  chpl_localeID_to_locale(&call_tmp_chpl35, &ret_tmp_chpl18, INT64(588), INT32(65));
#line 588 "../lib/Chai.chpl"
  call_tmp_chpl38 = ret_tmp_chpl18;
#line 588 "../lib/Chai.chpl"
  call_tmp_chpl34 = call_tmp_chpl38;
#line 588 "../lib/Chai.chpl"
  coerce_tmp_chpl16 = (&call_tmp_chpl34)->_instance;
#line 588 "../lib/Chai.chpl"
  if (((RootClass_chpl)(coerce_tmp_chpl16)) == nil) /* ZLINE: 588 ../lib/Chai.chpl */
#line 588 "../lib/Chai.chpl"
  {
#line 588 "../lib/Chai.chpl"
    nilCheckHalt_chpl(&_str_literal_52_chpl, INT64(588), INT32(65));
#line 588 "../lib/Chai.chpl"
  }
#line 588 "../lib/Chai.chpl"
  chpl_check_nil(coerce_tmp_chpl16, INT64(588), INT32(65));
#line 588 "../lib/Chai.chpl"
  ((int64_t(*)(BaseLocale_chpl,
#line 588 "../lib/Chai.chpl"
     int64_t,
#line 588 "../lib/Chai.chpl"
     int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(coerce_tmp_chpl16))->chpl__cid) + INT64(2))])(coerce_tmp_chpl16, INT64(588), INT32(65));
#line 588 "../lib/Chai.chpl"
  i_x_chpl5 = &call_tmp_chpl34;
#line 588 "../lib/Chai.chpl"
  deinit_chpl62(i_x_chpl5);
#line 588 "../lib/Chai.chpl"
  if (tasksPerLocale_chpl2 == INT64(0)) /* ZLINE: 588 ../lib/Chai.chpl */
#line 588 "../lib/Chai.chpl"
  {
#line 588 "../lib/Chai.chpl"
    call_tmp_chpl41 = get_chpl_nodeID();
#line 588 "../lib/Chai.chpl"
    call_tmp_chpl42 = chpl_rt_buildLocaleID(call_tmp_chpl41, c_sublocid_any);
#line 588 "../lib/Chai.chpl"
    call_tmp_chpl40 = call_tmp_chpl42;
#line 588 "../lib/Chai.chpl"
    chpl_localeID_to_locale(&call_tmp_chpl40, &ret_tmp_chpl19, INT64(588), INT32(65));
#line 588 "../lib/Chai.chpl"
    call_tmp_chpl43 = ret_tmp_chpl19;
#line 588 "../lib/Chai.chpl"
    call_tmp_chpl39 = call_tmp_chpl43;
#line 588 "../lib/Chai.chpl"
    coerce_tmp_chpl17 = (&call_tmp_chpl39)->_instance;
#line 588 "../lib/Chai.chpl"
    if (((RootClass_chpl)(coerce_tmp_chpl17)) == nil) /* ZLINE: 588 ../lib/Chai.chpl */
#line 588 "../lib/Chai.chpl"
    {
#line 588 "../lib/Chai.chpl"
      nilCheckHalt_chpl(&_str_literal_52_chpl, INT64(588), INT32(65));
#line 588 "../lib/Chai.chpl"
    }
#line 588 "../lib/Chai.chpl"
    coerce_tmp_chpl18 = coerce_tmp_chpl17;
#line 588 "../lib/Chai.chpl"
    chpl_check_nil(coerce_tmp_chpl18, INT64(588), INT32(65));
#line 588 "../lib/Chai.chpl"
    ret_chpl10 = (coerce_tmp_chpl18)->maxTaskPar;
#line 588 "../lib/Chai.chpl"
    tmp_chpl3 = ret_chpl10;
#line 588 "../lib/Chai.chpl"
    i_x_chpl6 = &call_tmp_chpl39;
#line 588 "../lib/Chai.chpl"
    deinit_chpl62(i_x_chpl6);
#line 588 "../lib/Chai.chpl"
  } else /* ZLINE: 588 ../lib/Chai.chpl */
#line 588 "../lib/Chai.chpl"
  {
#line 588 "../lib/Chai.chpl"
    tmp_chpl3 = tasksPerLocale_chpl2;
#line 588 "../lib/Chai.chpl"
  }
#line 588 "../lib/Chai.chpl"
  if (chpl_task_getSerial()) /* ZLINE: 588 ../lib/Chai.chpl */
#line 588 "../lib/Chai.chpl"
  {
#line 588 "../lib/Chai.chpl"
    tmp_x0_chpl2 = INT64(1);
#line 588 "../lib/Chai.chpl"
    tmp_x1_chpl2 = INT64(0);
#line 588 "../lib/Chai.chpl"
  } else /* ZLINE: 588 ../lib/Chai.chpl */
#line 588 "../lib/Chai.chpl"
  {
#line 588 "../lib/Chai.chpl"
    chpl_check_nil(coerce_tmp_chpl15, INT64(588), INT32(65));
#line 588 "../lib/Chai.chpl"
    call_tmp_chpl44 = &((coerce_tmp_chpl15)->ranges);
#line 588 "../lib/Chai.chpl"
    _computeChunkStuff_chpl(tmp_chpl3, ignoreRunning_chpl2, minIndicesPerTask_chpl2, call_tmp_chpl44, &ret_tmp_chpl20, INT64(588), INT32(65));
#line 588 "../lib/Chai.chpl"
    tmp_x0_chpl2 = *(ret_tmp_chpl20 + INT64(0));
#line 588 "../lib/Chai.chpl"
    tmp_x1_chpl2 = *(ret_tmp_chpl20 + INT64(1));
#line 588 "../lib/Chai.chpl"
  }
#line 588 "../lib/Chai.chpl"
  numChunks_chpl2 = tmp_x0_chpl2;
#line 588 "../lib/Chai.chpl"
  parDim_chpl2 = tmp_x1_chpl2;
#line 588 "../lib/Chai.chpl"
  init_chpl113(&elt_x0_chpl2);
#line 588 "../lib/Chai.chpl"
  tup_x0_chpl2 = elt_x0_chpl2;
#line 588 "../lib/Chai.chpl"
  locBlock_x0_chpl2 = tup_x0_chpl2;
#line 588 "../lib/Chai.chpl"
  chpl_build_low_bounded_range2(offset_x0_chpl2, &ret_tmp_chpl21);
#line 588 "../lib/Chai.chpl"
  call_tmp_chpl45 = ret_tmp_chpl21;
#line 588 "../lib/Chai.chpl"
  chpl_check_nil(coerce_tmp_chpl15, INT64(588), INT32(65));
#line 588 "../lib/Chai.chpl"
  call_tmp_chpl46 = &((coerce_tmp_chpl15)->ranges);
#line 588 "../lib/Chai.chpl"
  coerce_tmp_chpl19 = *(*(call_tmp_chpl46) + INT64(0));
#line 588 "../lib/Chai.chpl"
  call_tmp_chpl47 = sizeAs_chpl37(coerce_tmp_chpl19, INT64(588), INT32(65));
#line 588 "../lib/Chai.chpl"
  chpl___POUND_3(call_tmp_chpl45, call_tmp_chpl47, &ret_tmp_chpl22, INT64(588), INT32(65));
#line 588 "../lib/Chai.chpl"
  call_tmp_chpl48 = ret_tmp_chpl22;
#line 588 "../lib/Chai.chpl"
  locBlock_x0_chpl2 = call_tmp_chpl48;
#line 588 "../lib/Chai.chpl"
  chpl_build_low_bounded_range2(INT64(0), &ret_tmp_chpl23);
#line 588 "../lib/Chai.chpl"
  call_tmp_chpl49 = ret_tmp_chpl23;
#line 588 "../lib/Chai.chpl"
  chpl___POUND_3(call_tmp_chpl49, tmp_x0_chpl2, &ret_tmp_chpl24, INT64(588), INT32(65));
#line 588 "../lib/Chai.chpl"
  tmpIter_chpl2 = ret_tmp_chpl24;
#line 588 "../lib/Chai.chpl"
  new_temp_chpl4 = _new_chpl17(INT64(588), INT32(65));
#line 588 "../lib/Chai.chpl"
  _coforallCount_chpl2 = new_temp_chpl4;
#line 588 "../lib/Chai.chpl"
  numTasks_chpl2 = chpl_boundedCoforallSize(tmpIter_chpl2, INT64(588), INT32(65));
#line 588 "../lib/Chai.chpl"
  _upEndCount_chpl(_coforallCount_chpl2, numTasks_chpl2, INT64(588), INT32(65));
#line 588 "../lib/Chai.chpl"
  chpl_resetTaskSpawn(numTasks_chpl2, INT64(588), INT32(65));
#line 588 "../lib/Chai.chpl"
  _ic__F0_this_chpl2 = tmpIter_chpl2;
#line 588 "../lib/Chai.chpl"
  checkIfIterWillOverflow_chpl(tmpIter_chpl2, UINT8(true), INT64(588), INT32(65));
#line 588 "../lib/Chai.chpl"
  i_chpl2 = INT64(0);
#line 588 "../lib/Chai.chpl"
  this_chpl34 = _ic__F0_this_chpl2;
#line 588 "../lib/Chai.chpl"
  this_chpl35 = _ic__F0_this_chpl2;
#line 588 "../lib/Chai.chpl"
  end_chpl2 = (&this_chpl35)->_high;
#line 588 "../lib/Chai.chpl"
  for (i_chpl2 = (&this_chpl34)->_low; ((i_chpl2 <= end_chpl2)); i_chpl2 += INT64(1)) {
#line 588 "../lib/Chai.chpl"
    *(ret_tmp_chpl25 + INT64(0)) = locBlock_x0_chpl2;
#line 588 "../lib/Chai.chpl"
    _args_forcoforall_fn_chpl2 = ((_class_localscoforall_fn_chpl113)(&chpl_macro_tmp_118));
#line 588 "../lib/Chai.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl2, INT64(588), INT32(65));
#line 588 "../lib/Chai.chpl"
    (_args_forcoforall_fn_chpl2)->_0_numChunks = numChunks_chpl2;
#line 588 "../lib/Chai.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl2, INT64(588), INT32(65));
#line 588 "../lib/Chai.chpl"
    (_args_forcoforall_fn_chpl2)->_1_parDim = parDim_chpl2;
#line 588 "../lib/Chai.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl2, INT64(588), INT32(65));
#line 588 "../lib/Chai.chpl"
    *((_args_forcoforall_fn_chpl2)->_2_coerce_tmp + INT64(0)) = *(ret_tmp_chpl25 + INT64(0));
#line 588 "../lib/Chai.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl2, INT64(588), INT32(65));
#line 588 "../lib/Chai.chpl"
    (_args_forcoforall_fn_chpl2)->_3__coforallCount = _coforallCount_chpl2;
#line 588 "../lib/Chai.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl2, INT64(588), INT32(65));
#line 588 "../lib/Chai.chpl"
    (_args_forcoforall_fn_chpl2)->_4_chunk = i_chpl2;
#line 588 "../lib/Chai.chpl"
    tmp_chpl4 = this_chpl31;
#line 588 "../lib/Chai.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl2, INT64(588), INT32(65));
#line 588 "../lib/Chai.chpl"
    (_args_forcoforall_fn_chpl2)->_6_this = tmp_chpl4;
#line 588 "../lib/Chai.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl2, INT64(588), INT32(65));
#line 588 "../lib/Chai.chpl"
    (_args_forcoforall_fn_chpl2)->_7_globalRP_weightsGrad = globalRP_weightsGrad_chpl;
#line 588 "../lib/Chai.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl2, INT64(588), INT32(65));
#line 588 "../lib/Chai.chpl"
    (_args_forcoforall_fn_chpl2)->_8_globalRP_biasesGrad = globalRP_biasesGrad_chpl;
#line 588 "../lib/Chai.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl2, INT64(588), INT32(65));
#line 588 "../lib/Chai.chpl"
    (_args_forcoforall_fn_chpl2)->_9_deltas = *(deltas_chpl);
#line 588 "../lib/Chai.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl2, INT64(588), INT32(65));
#line 588 "../lib/Chai.chpl"
    (_args_forcoforall_fn_chpl2)->_10_inputs = *(inputs_chpl);
#line 588 "../lib/Chai.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl2, INT64(588), INT32(65));
#line 588 "../lib/Chai.chpl"
    (_args_forcoforall_fn_chpl2)->_11_newDeltas = newDeltas_chpl;
#line 588 "../lib/Chai.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl2, INT64(588), INT32(65));
#line 588 "../lib/Chai.chpl"
    (_args_forcoforall_fn_chpl2)->_12_call_tmp = call_tmp_chpl31;
#line 588 "../lib/Chai.chpl"
    _args_vforcoforall_fn_chpl2 = ((void*)(_args_forcoforall_fn_chpl2));
#line 588 "../lib/Chai.chpl"
    /*** wrapcoforall_fn_chpl113 ***/ chpl_taskAddCoStmt(chpl_task_getRequestedSubloc(), INT64(112), ((chpl_task_bundle_p)(_args_vforcoforall_fn_chpl2)), sizeof(chpl__class_localscoforall_fn_chpl113_object), INT64(588), INT32(65));
#line 588 "../lib/Chai.chpl"
  }
#line 588 "../lib/Chai.chpl"
  error_chpl7 = NULL;
#line 588 "../lib/Chai.chpl"
  _waitEndCount_chpl2(_coforallCount_chpl2, numTasks_chpl2, &error_chpl7, INT64(588), INT32(65));
#line 588 "../lib/Chai.chpl"
  if (error_chpl7 != nil) /* ZLINE: 588 ../lib/Chai.chpl */
#line 588 "../lib/Chai.chpl"
  {
#line 588 "../lib/Chai.chpl"
    chpl__delete72(_coforallCount_chpl2, INT64(588), INT32(65));
#line 588 "../lib/Chai.chpl"
    error_chpl6 = error_chpl7;
#line 588 "../lib/Chai.chpl"
    goto forall_IBB_break_label_chpl4;
#line 588 "../lib/Chai.chpl"
  }
#line 588 "../lib/Chai.chpl"
  chpl__delete72(_coforallCount_chpl2, INT64(588), INT32(65));
#line 588 "../lib/Chai.chpl"
  chpl_after_forall_fence();
#line 588 "../lib/Chai.chpl"
  forall_IBB_break_label_chpl4:;
#line 588 "../lib/Chai.chpl"
  if (error_chpl6 != nil) /* ZLINE: 588 ../lib/Chai.chpl */
#line 588 "../lib/Chai.chpl"
  {
#line 588 "../lib/Chai.chpl"
    error_chpl5 = error_chpl6;
#line 588 "../lib/Chai.chpl"
    goto forall_IBB_break_label_chpl5;
#line 588 "../lib/Chai.chpl"
  }
#line 588 "../lib/Chai.chpl"
  chpl_after_forall_fence();
#line 588 "../lib/Chai.chpl"
  forall_IBB_break_label_chpl5:;
#line 588 "../lib/Chai.chpl"
  if (error_chpl5 != nil) /* ZLINE: 588 ../lib/Chai.chpl */
#line 588 "../lib/Chai.chpl"
  {
#line 588 "../lib/Chai.chpl"
    chpl_propagate_error(error_chpl5, INT64(588), INT32(65));
#line 588 "../lib/Chai.chpl"
  }
#line 588 "../lib/Chai.chpl"
  chpl_after_forall_fence();
#line 588 "../lib/Chai.chpl"
  chpl__autoDestroy11(&call_tmp_chpl31, INT64(588), INT32(65));
#line 588 "../lib/Chai.chpl"
  coerce_tmp_chpl20 = globalRP_biasesGrad_chpl;
#line 588 "../lib/Chai.chpl"
  i_x_chpl7 = &globalAS_biasesGrad_chpl;
#line 588 "../lib/Chai.chpl"
  chpl_check_nil(coerce_tmp_chpl20, INT64(588), INT32(65));
#line 588 "../lib/Chai.chpl"
  call_tmp_chpl50 = &((coerce_tmp_chpl20)->value);
#line 588 "../lib/Chai.chpl"
  chpl___PLUS__ASSIGN_4(call_tmp_chpl50, i_x_chpl7);
#line 588 "../lib/Chai.chpl"
  _field_destructor_tmp__chpl = (&globalAS_biasesGrad_chpl)->data;
#line 588 "../lib/Chai.chpl"
  chpl__autoDestroy18(&_field_destructor_tmp__chpl, INT64(588), INT32(65));
#line 588 "../lib/Chai.chpl"
  _field_destructor_tmp__chpl2 = (&globalAS_biasesGrad_chpl)->_domain;
#line 588 "../lib/Chai.chpl"
  chpl__autoDestroy11(&_field_destructor_tmp__chpl2, INT64(588), INT32(65));
#line 588 "../lib/Chai.chpl"
  coerce_tmp_chpl21 = globalRP_biasesGrad_chpl;
#line 588 "../lib/Chai.chpl"
  chpl_check_nil(coerce_tmp_chpl21, INT64(588), INT32(65));
#line 588 "../lib/Chai.chpl"
  call_tmp_chpl51 = &((coerce_tmp_chpl21)->value);
#line 588 "../lib/Chai.chpl"
  init_ASSIGN__chpl32(&tmp_chpl5, call_tmp_chpl51);
#line 588 "../lib/Chai.chpl"
  ret_chpl11 = tmp_chpl5;
#line 588 "../lib/Chai.chpl"
  chpl_gentemp = ret_chpl11;
#line 588 "../lib/Chai.chpl"
  chpl___ASSIGN_25(&biasesGrad_chpl, &chpl_gentemp);
#line 588 "../lib/Chai.chpl"
  chpl__delete25(globalRP_biasesGrad_chpl, INT64(588), INT32(65));
#line 588 "../lib/Chai.chpl"
  coerce_tmp_chpl22 = globalRP_weightsGrad_chpl;
#line 588 "../lib/Chai.chpl"
  i_x_chpl8 = &globalAS_weightsGrad_chpl;
#line 588 "../lib/Chai.chpl"
  chpl_check_nil(coerce_tmp_chpl22, INT64(588), INT32(65));
#line 588 "../lib/Chai.chpl"
  call_tmp_chpl52 = &((coerce_tmp_chpl22)->value);
#line 588 "../lib/Chai.chpl"
  chpl___PLUS__ASSIGN_2(call_tmp_chpl52, i_x_chpl8);
#line 588 "../lib/Chai.chpl"
  _field_destructor_tmp__chpl3 = (&globalAS_weightsGrad_chpl)->data;
#line 588 "../lib/Chai.chpl"
  chpl__autoDestroy19(&_field_destructor_tmp__chpl3, INT64(588), INT32(65));
#line 588 "../lib/Chai.chpl"
  _field_destructor_tmp__chpl4 = (&globalAS_weightsGrad_chpl)->_domain;
#line 588 "../lib/Chai.chpl"
  chpl__autoDestroy5(&_field_destructor_tmp__chpl4, INT64(588), INT32(65));
#line 588 "../lib/Chai.chpl"
  coerce_tmp_chpl23 = globalRP_weightsGrad_chpl;
#line 588 "../lib/Chai.chpl"
  chpl_check_nil(coerce_tmp_chpl23, INT64(588), INT32(65));
#line 588 "../lib/Chai.chpl"
  call_tmp_chpl53 = &((coerce_tmp_chpl23)->value);
#line 588 "../lib/Chai.chpl"
  init_ASSIGN__chpl31(&tmp_chpl6, call_tmp_chpl53);
#line 588 "../lib/Chai.chpl"
  ret_chpl12 = tmp_chpl6;
#line 588 "../lib/Chai.chpl"
  chpl_gentemp2 = ret_chpl12;
#line 588 "../lib/Chai.chpl"
  chpl___ASSIGN_23(&weightsGrad_chpl, &chpl_gentemp2);
#line 588 "../lib/Chai.chpl"
  chpl__delete64(globalRP_weightsGrad_chpl, INT64(588), INT32(65));
#line 612 "../lib/Chai.chpl"
  call_tmp_chpl54 = &((this_chpl31)->weightsGrad);
#line 612 "../lib/Chai.chpl"
  call_tmp_chpl55 = &((call_tmp_chpl54)->data);
#line 612 "../lib/Chai.chpl"
  call_tmp_chpl56 = &((&weightsGrad_chpl)->data);
#line 612 "../lib/Chai.chpl"
  if (((RootClass_chpl)((call_tmp_chpl56)->_instance)) == nil) /* ZLINE: 612 ../lib/Chai.chpl */
#line 612 "../lib/Chai.chpl"
  {
#line 612 "../lib/Chai.chpl"
    goto _end__ASSIGN__chpl;
#line 612 "../lib/Chai.chpl"
  }
#line 612 "../lib/Chai.chpl"
  if (((RootClass_chpl)((call_tmp_chpl55)->_instance)) == ((RootClass_chpl)((call_tmp_chpl56)->_instance))) /* ZLINE: 612 ../lib/Chai.chpl */
#line 612 "../lib/Chai.chpl"
  {
#line 612 "../lib/Chai.chpl"
    goto _end__ASSIGN__chpl;
#line 612 "../lib/Chai.chpl"
  }
#line 612 "../lib/Chai.chpl"
  call_tmp_chpl57 = sizeAs_chpl14(call_tmp_chpl55, INT64(612), INT32(65));
#line 612 "../lib/Chai.chpl"
  if (! (INT64(0) < INT64(0))) /* ZLINE: 612 ../lib/Chai.chpl */
#line 612 "../lib/Chai.chpl"
  {
#line 612 "../lib/Chai.chpl"
    tmp_chpl7 = (call_tmp_chpl57 == ((uint64_t)(INT64(0))));
#line 612 "../lib/Chai.chpl"
  } else /* ZLINE: 612 ../lib/Chai.chpl */
#line 612 "../lib/Chai.chpl"
  {
#line 612 "../lib/Chai.chpl"
    tmp_chpl7 = UINT8(false);
#line 612 "../lib/Chai.chpl"
  }
#line 612 "../lib/Chai.chpl"
  if (tmp_chpl7) /* ZLINE: 612 ../lib/Chai.chpl */
#line 612 "../lib/Chai.chpl"
  {
#line 612 "../lib/Chai.chpl"
    call_tmp_chpl58 = sizeAs_chpl14(call_tmp_chpl56, INT64(612), INT32(65));
#line 612 "../lib/Chai.chpl"
    if (! (INT64(0) < INT64(0))) /* ZLINE: 612 ../lib/Chai.chpl */
#line 612 "../lib/Chai.chpl"
    {
#line 612 "../lib/Chai.chpl"
      tmp_chpl9 = (call_tmp_chpl58 == ((uint64_t)(INT64(0))));
#line 612 "../lib/Chai.chpl"
    } else /* ZLINE: 612 ../lib/Chai.chpl */
#line 612 "../lib/Chai.chpl"
    {
#line 612 "../lib/Chai.chpl"
      tmp_chpl9 = UINT8(false);
#line 612 "../lib/Chai.chpl"
    }
#line 612 "../lib/Chai.chpl"
    tmp_chpl8 = tmp_chpl9;
#line 612 "../lib/Chai.chpl"
  } else /* ZLINE: 612 ../lib/Chai.chpl */
#line 612 "../lib/Chai.chpl"
  {
#line 612 "../lib/Chai.chpl"
    tmp_chpl8 = UINT8(false);
#line 612 "../lib/Chai.chpl"
  }
#line 612 "../lib/Chai.chpl"
  if (tmp_chpl8) /* ZLINE: 612 ../lib/Chai.chpl */
#line 612 "../lib/Chai.chpl"
  {
#line 612 "../lib/Chai.chpl"
    goto _end__ASSIGN__chpl;
#line 612 "../lib/Chai.chpl"
  }
#line 612 "../lib/Chai.chpl"
  checkArrayShapesUponAssignment_chpl9(call_tmp_chpl55, call_tmp_chpl56, UINT8(false), INT64(612), INT32(65));
#line 612 "../lib/Chai.chpl"
  _dom_chpl14(call_tmp_chpl55, &ret_tmp_chpl26, INT64(612), INT32(65));
#line 612 "../lib/Chai.chpl"
  call_tmp_chpl59 = ret_tmp_chpl26;
#line 612 "../lib/Chai.chpl"
  _dom_chpl14(call_tmp_chpl56, &ret_tmp_chpl27, INT64(612), INT32(65));
#line 612 "../lib/Chai.chpl"
  call_tmp_chpl60 = ret_tmp_chpl27;
#line 612 "../lib/Chai.chpl"
  i_AD_chpl = &call_tmp_chpl59;
#line 612 "../lib/Chai.chpl"
  i_BD_chpl = &call_tmp_chpl60;
#line 612 "../lib/Chai.chpl"
  ret_chpl13 = (call_tmp_chpl55)->_instance;
#line 612 "../lib/Chai.chpl"
  ret_chpl14 = (call_tmp_chpl56)->_instance;
#line 612 "../lib/Chai.chpl"
  chpl_check_nil(ret_chpl13, INT64(612), INT32(65));
#line 612 "../lib/Chai.chpl"
  call_tmp_chpl61 = doiBulkTransferFromKnown_chpl13(ret_chpl13, i_AD_chpl, ret_chpl14, i_BD_chpl, INT64(612), INT32(65));
#line 612 "../lib/Chai.chpl"
  if (call_tmp_chpl61) /* ZLINE: 612 ../lib/Chai.chpl */
#line 612 "../lib/Chai.chpl"
  {
#line 612 "../lib/Chai.chpl"
  } else /* ZLINE: 612 ../lib/Chai.chpl */
#line 612 "../lib/Chai.chpl"
  {
#line 612 "../lib/Chai.chpl"
  }
#line 612 "../lib/Chai.chpl"
  chpl__autoDestroy5(&call_tmp_chpl60, INT64(612), INT32(65));
#line 612 "../lib/Chai.chpl"
  chpl__autoDestroy5(&call_tmp_chpl59, INT64(612), INT32(65));
#line 612 "../lib/Chai.chpl"
  if (call_tmp_chpl61) /* ZLINE: 612 ../lib/Chai.chpl */
#line 612 "../lib/Chai.chpl"
  {
#line 612 "../lib/Chai.chpl"
  }
#line 612 "../lib/Chai.chpl"
  if (! call_tmp_chpl61) /* ZLINE: 612 ../lib/Chai.chpl */
#line 612 "../lib/Chai.chpl"
  {
#line 612 "../lib/Chai.chpl"
    coerce_tmp_chpl24 = (call_tmp_chpl55)->_instance;
#line 612 "../lib/Chai.chpl"
    tasksPerLocale_chpl3 = local_dataParTasksPerLocale_chpl;
#line 612 "../lib/Chai.chpl"
    ignoreRunning_chpl3 = local_dataParIgnoreRunningTasks_chpl;
#line 612 "../lib/Chai.chpl"
    minIndicesPerTask_chpl3 = local_dataParMinGranularity_chpl;
#line 612 "../lib/Chai.chpl"
    error_chpl8 = NULL;
#line 612 "../lib/Chai.chpl"
    chpl_check_nil(coerce_tmp_chpl24, INT64(612), INT32(65));
#line 612 "../lib/Chai.chpl"
    coerce_tmp_chpl25 = (coerce_tmp_chpl24)->dom;
#line 612 "../lib/Chai.chpl"
    createTuple_chpl4(INT64(0), &ret_tmp_chpl28);
#line 612 "../lib/Chai.chpl"
    offset_x0_chpl3 = *(ret_tmp_chpl28 + INT64(0));
#line 612 "../lib/Chai.chpl"
    offset_x1_chpl = *(ret_tmp_chpl28 + INT64(1));
#line 612 "../lib/Chai.chpl"
    error_chpl9 = NULL;
#line 612 "../lib/Chai.chpl"
    call_tmp_chpl64 = get_chpl_nodeID();
#line 612 "../lib/Chai.chpl"
    call_tmp_chpl65 = chpl_rt_buildLocaleID(call_tmp_chpl64, c_sublocid_any);
#line 612 "../lib/Chai.chpl"
    call_tmp_chpl63 = call_tmp_chpl65;
#line 612 "../lib/Chai.chpl"
    chpl_localeID_to_locale(&call_tmp_chpl63, &ret_tmp_chpl29, INT64(612), INT32(65));
#line 612 "../lib/Chai.chpl"
    call_tmp_chpl66 = ret_tmp_chpl29;
#line 612 "../lib/Chai.chpl"
    call_tmp_chpl62 = call_tmp_chpl66;
#line 612 "../lib/Chai.chpl"
    coerce_tmp_chpl26 = (&call_tmp_chpl62)->_instance;
#line 612 "../lib/Chai.chpl"
    if (((RootClass_chpl)(coerce_tmp_chpl26)) == nil) /* ZLINE: 612 ../lib/Chai.chpl */
#line 612 "../lib/Chai.chpl"
    {
#line 612 "../lib/Chai.chpl"
      nilCheckHalt_chpl(&_str_literal_52_chpl, INT64(612), INT32(65));
#line 612 "../lib/Chai.chpl"
    }
#line 612 "../lib/Chai.chpl"
    chpl_check_nil(coerce_tmp_chpl26, INT64(612), INT32(65));
#line 612 "../lib/Chai.chpl"
    ((int64_t(*)(BaseLocale_chpl,
#line 612 "../lib/Chai.chpl"
       int64_t,
#line 612 "../lib/Chai.chpl"
       int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(coerce_tmp_chpl26))->chpl__cid) + INT64(2))])(coerce_tmp_chpl26, INT64(612), INT32(65));
#line 612 "../lib/Chai.chpl"
    i_x_chpl9 = &call_tmp_chpl62;
#line 612 "../lib/Chai.chpl"
    deinit_chpl62(i_x_chpl9);
#line 612 "../lib/Chai.chpl"
    if (tasksPerLocale_chpl3 == INT64(0)) /* ZLINE: 612 ../lib/Chai.chpl */
#line 612 "../lib/Chai.chpl"
    {
#line 612 "../lib/Chai.chpl"
      call_tmp_chpl69 = get_chpl_nodeID();
#line 612 "../lib/Chai.chpl"
      call_tmp_chpl70 = chpl_rt_buildLocaleID(call_tmp_chpl69, c_sublocid_any);
#line 612 "../lib/Chai.chpl"
      call_tmp_chpl68 = call_tmp_chpl70;
#line 612 "../lib/Chai.chpl"
      chpl_localeID_to_locale(&call_tmp_chpl68, &ret_tmp_chpl30, INT64(612), INT32(65));
#line 612 "../lib/Chai.chpl"
      call_tmp_chpl71 = ret_tmp_chpl30;
#line 612 "../lib/Chai.chpl"
      call_tmp_chpl67 = call_tmp_chpl71;
#line 612 "../lib/Chai.chpl"
      coerce_tmp_chpl27 = (&call_tmp_chpl67)->_instance;
#line 612 "../lib/Chai.chpl"
      if (((RootClass_chpl)(coerce_tmp_chpl27)) == nil) /* ZLINE: 612 ../lib/Chai.chpl */
#line 612 "../lib/Chai.chpl"
      {
#line 612 "../lib/Chai.chpl"
        nilCheckHalt_chpl(&_str_literal_52_chpl, INT64(612), INT32(65));
#line 612 "../lib/Chai.chpl"
      }
#line 612 "../lib/Chai.chpl"
      coerce_tmp_chpl28 = coerce_tmp_chpl27;
#line 612 "../lib/Chai.chpl"
      chpl_check_nil(coerce_tmp_chpl28, INT64(612), INT32(65));
#line 612 "../lib/Chai.chpl"
      ret_chpl15 = (coerce_tmp_chpl28)->maxTaskPar;
#line 612 "../lib/Chai.chpl"
      tmp_chpl10 = ret_chpl15;
#line 612 "../lib/Chai.chpl"
      i_x_chpl10 = &call_tmp_chpl67;
#line 612 "../lib/Chai.chpl"
      deinit_chpl62(i_x_chpl10);
#line 612 "../lib/Chai.chpl"
    } else /* ZLINE: 612 ../lib/Chai.chpl */
#line 612 "../lib/Chai.chpl"
    {
#line 612 "../lib/Chai.chpl"
      tmp_chpl10 = tasksPerLocale_chpl3;
#line 612 "../lib/Chai.chpl"
    }
#line 612 "../lib/Chai.chpl"
    if (chpl_task_getSerial()) /* ZLINE: 612 ../lib/Chai.chpl */
#line 612 "../lib/Chai.chpl"
    {
#line 612 "../lib/Chai.chpl"
      tmp_x0_chpl3 = INT64(1);
#line 612 "../lib/Chai.chpl"
      tmp_x1_chpl3 = INT64(0);
#line 612 "../lib/Chai.chpl"
    } else /* ZLINE: 612 ../lib/Chai.chpl */
#line 612 "../lib/Chai.chpl"
    {
#line 612 "../lib/Chai.chpl"
      chpl_check_nil(coerce_tmp_chpl25, INT64(612), INT32(65));
#line 612 "../lib/Chai.chpl"
      call_tmp_chpl72 = &((coerce_tmp_chpl25)->ranges);
#line 612 "../lib/Chai.chpl"
      _computeChunkStuff_chpl4(tmp_chpl10, ignoreRunning_chpl3, minIndicesPerTask_chpl3, call_tmp_chpl72, &ret_tmp_chpl31, INT64(612), INT32(65));
#line 612 "../lib/Chai.chpl"
      tmp_x0_chpl3 = *(ret_tmp_chpl31 + INT64(0));
#line 612 "../lib/Chai.chpl"
      tmp_x1_chpl3 = *(ret_tmp_chpl31 + INT64(1));
#line 612 "../lib/Chai.chpl"
    }
#line 612 "../lib/Chai.chpl"
    numChunks_chpl3 = tmp_x0_chpl3;
#line 612 "../lib/Chai.chpl"
    parDim_chpl3 = tmp_x1_chpl3;
#line 612 "../lib/Chai.chpl"
    init_chpl113(&elt_x0_chpl3);
#line 612 "../lib/Chai.chpl"
    tup_x0_chpl3 = elt_x0_chpl3;
#line 612 "../lib/Chai.chpl"
    init_chpl113(&elt_x1_chpl);
#line 612 "../lib/Chai.chpl"
    tup_x1_chpl = elt_x1_chpl;
#line 612 "../lib/Chai.chpl"
    *(locBlock_chpl + INT64(0)) = tup_x0_chpl3;
#line 612 "../lib/Chai.chpl"
    *(locBlock_chpl + INT64(1)) = tup_x1_chpl;
#line 612 "../lib/Chai.chpl"
    chpl_build_low_bounded_range2(offset_x0_chpl3, &ret_tmp_chpl32);
#line 612 "../lib/Chai.chpl"
    call_tmp_chpl73 = ret_tmp_chpl32;
#line 612 "../lib/Chai.chpl"
    chpl_check_nil(coerce_tmp_chpl25, INT64(612), INT32(65));
#line 612 "../lib/Chai.chpl"
    call_tmp_chpl74 = &((coerce_tmp_chpl25)->ranges);
#line 612 "../lib/Chai.chpl"
    coerce_tmp_chpl29 = *(*(call_tmp_chpl74) + INT64(0));
#line 612 "../lib/Chai.chpl"
    call_tmp_chpl75 = sizeAs_chpl37(coerce_tmp_chpl29, INT64(612), INT32(65));
#line 612 "../lib/Chai.chpl"
    chpl___POUND_3(call_tmp_chpl73, call_tmp_chpl75, &ret_tmp_chpl33, INT64(612), INT32(65));
#line 612 "../lib/Chai.chpl"
    call_tmp_chpl76 = ret_tmp_chpl33;
#line 612 "../lib/Chai.chpl"
    *(locBlock_chpl + INT64(0)) = call_tmp_chpl76;
#line 612 "../lib/Chai.chpl"
    chpl_build_low_bounded_range2(offset_x1_chpl, &ret_tmp_chpl34);
#line 612 "../lib/Chai.chpl"
    call_tmp_chpl77 = ret_tmp_chpl34;
#line 612 "../lib/Chai.chpl"
    chpl_check_nil(coerce_tmp_chpl25, INT64(612), INT32(65));
#line 612 "../lib/Chai.chpl"
    call_tmp_chpl78 = &((coerce_tmp_chpl25)->ranges);
#line 612 "../lib/Chai.chpl"
    coerce_tmp_chpl30 = *(*(call_tmp_chpl78) + INT64(1));
#line 612 "../lib/Chai.chpl"
    call_tmp_chpl79 = sizeAs_chpl37(coerce_tmp_chpl30, INT64(612), INT32(65));
#line 612 "../lib/Chai.chpl"
    chpl___POUND_3(call_tmp_chpl77, call_tmp_chpl79, &ret_tmp_chpl35, INT64(612), INT32(65));
#line 612 "../lib/Chai.chpl"
    call_tmp_chpl80 = ret_tmp_chpl35;
#line 612 "../lib/Chai.chpl"
    *(locBlock_chpl + INT64(1)) = call_tmp_chpl80;
#line 612 "../lib/Chai.chpl"
    chpl_build_low_bounded_range2(INT64(0), &ret_tmp_chpl36);
#line 612 "../lib/Chai.chpl"
    call_tmp_chpl81 = ret_tmp_chpl36;
#line 612 "../lib/Chai.chpl"
    chpl___POUND_3(call_tmp_chpl81, tmp_x0_chpl3, &ret_tmp_chpl37, INT64(612), INT32(65));
#line 612 "../lib/Chai.chpl"
    tmpIter_chpl3 = ret_tmp_chpl37;
#line 612 "../lib/Chai.chpl"
    new_temp_chpl5 = _new_chpl17(INT64(612), INT32(65));
#line 612 "../lib/Chai.chpl"
    _coforallCount_chpl3 = new_temp_chpl5;
#line 612 "../lib/Chai.chpl"
    numTasks_chpl3 = chpl_boundedCoforallSize(tmpIter_chpl3, INT64(612), INT32(65));
#line 612 "../lib/Chai.chpl"
    _upEndCount_chpl(_coforallCount_chpl3, numTasks_chpl3, INT64(612), INT32(65));
#line 612 "../lib/Chai.chpl"
    chpl_resetTaskSpawn(numTasks_chpl3, INT64(612), INT32(65));
#line 612 "../lib/Chai.chpl"
    _ic__F0_this_chpl3 = tmpIter_chpl3;
#line 612 "../lib/Chai.chpl"
    checkIfIterWillOverflow_chpl(tmpIter_chpl3, UINT8(true), INT64(612), INT32(65));
#line 612 "../lib/Chai.chpl"
    i_chpl3 = INT64(0);
#line 612 "../lib/Chai.chpl"
    this_chpl36 = _ic__F0_this_chpl3;
#line 612 "../lib/Chai.chpl"
    this_chpl37 = _ic__F0_this_chpl3;
#line 612 "../lib/Chai.chpl"
    end_chpl3 = (&this_chpl37)->_high;
#line 612 "../lib/Chai.chpl"
    for (i_chpl3 = (&this_chpl36)->_low; ((i_chpl3 <= end_chpl3)); i_chpl3 += INT64(1)) {
#line 612 "../lib/Chai.chpl"
      _args_forcoforall_fn_chpl3 = ((_class_localscoforall_fn_chpl18)(&chpl_macro_tmp_119));
#line 612 "../lib/Chai.chpl"
      chpl_check_nil(_args_forcoforall_fn_chpl3, INT64(612), INT32(65));
#line 612 "../lib/Chai.chpl"
      (_args_forcoforall_fn_chpl3)->_0_numChunks = numChunks_chpl3;
#line 612 "../lib/Chai.chpl"
      chpl_check_nil(_args_forcoforall_fn_chpl3, INT64(612), INT32(65));
#line 612 "../lib/Chai.chpl"
      (_args_forcoforall_fn_chpl3)->_1_parDim = parDim_chpl3;
#line 612 "../lib/Chai.chpl"
      chpl_check_nil(_args_forcoforall_fn_chpl3, INT64(612), INT32(65));
#line 612 "../lib/Chai.chpl"
      *((_args_forcoforall_fn_chpl3)->_2_coerce_tmp + INT64(0)) = *(locBlock_chpl + INT64(0));
#line 612 "../lib/Chai.chpl"
      *((_args_forcoforall_fn_chpl3)->_2_coerce_tmp + INT64(1)) = *(locBlock_chpl + INT64(1));
#line 612 "../lib/Chai.chpl"
      chpl_check_nil(_args_forcoforall_fn_chpl3, INT64(612), INT32(65));
#line 612 "../lib/Chai.chpl"
      (_args_forcoforall_fn_chpl3)->_3__coforallCount = _coforallCount_chpl3;
#line 612 "../lib/Chai.chpl"
      chpl_check_nil(_args_forcoforall_fn_chpl3, INT64(612), INT32(65));
#line 612 "../lib/Chai.chpl"
      (_args_forcoforall_fn_chpl3)->_4_chunk = i_chpl3;
#line 612 "../lib/Chai.chpl"
      chpl_check_nil(_args_forcoforall_fn_chpl3, INT64(612), INT32(65));
#line 612 "../lib/Chai.chpl"
      (_args_forcoforall_fn_chpl3)->_6_a = *(call_tmp_chpl55);
#line 612 "../lib/Chai.chpl"
      chpl_check_nil(_args_forcoforall_fn_chpl3, INT64(612), INT32(65));
#line 612 "../lib/Chai.chpl"
      (_args_forcoforall_fn_chpl3)->_7_b = *(call_tmp_chpl56);
#line 612 "../lib/Chai.chpl"
      _args_vforcoforall_fn_chpl3 = ((void*)(_args_forcoforall_fn_chpl3));
#line 612 "../lib/Chai.chpl"
      _args_sizecoforall_fn_chpl = sizeof(chpl__class_localscoforall_fn_chpl18_object);
#line 612 "../lib/Chai.chpl"
      (_args_forcoforall_fn_chpl3)->_ln = INT64(612);
#line 612 "../lib/Chai.chpl"
      (_args_forcoforall_fn_chpl3)->_fn = INT32(65);
#line 612 "../lib/Chai.chpl"
      /*** wrapcoforall_fn_chpl29 ***/ chpl_taskAddCoStmt(chpl_task_getRequestedSubloc(), INT64(28), ((chpl_task_bundle_p)(_args_vforcoforall_fn_chpl3)), _args_sizecoforall_fn_chpl, INT64(2763), INT32(30));
#line 612 "../lib/Chai.chpl"
    }
#line 612 "../lib/Chai.chpl"
    error_chpl10 = NULL;
#line 612 "../lib/Chai.chpl"
    _waitEndCount_chpl2(_coforallCount_chpl3, numTasks_chpl3, &error_chpl10, INT64(612), INT32(65));
#line 612 "../lib/Chai.chpl"
    if (error_chpl10 != nil) /* ZLINE: 612 ../lib/Chai.chpl */
#line 612 "../lib/Chai.chpl"
    {
#line 612 "../lib/Chai.chpl"
      chpl__delete72(_coforallCount_chpl3, INT64(612), INT32(65));
#line 612 "../lib/Chai.chpl"
      error_chpl9 = error_chpl10;
#line 612 "../lib/Chai.chpl"
      goto forall_IBB_break_label_chpl6;
#line 612 "../lib/Chai.chpl"
    }
#line 612 "../lib/Chai.chpl"
    chpl__delete72(_coforallCount_chpl3, INT64(612), INT32(65));
#line 612 "../lib/Chai.chpl"
    chpl_after_forall_fence();
#line 612 "../lib/Chai.chpl"
    forall_IBB_break_label_chpl6:;
#line 612 "../lib/Chai.chpl"
    if (error_chpl9 != nil) /* ZLINE: 612 ../lib/Chai.chpl */
#line 612 "../lib/Chai.chpl"
    {
#line 612 "../lib/Chai.chpl"
      error_chpl8 = error_chpl9;
#line 612 "../lib/Chai.chpl"
      goto forall_IBB_break_label_chpl7;
#line 612 "../lib/Chai.chpl"
    }
#line 612 "../lib/Chai.chpl"
    chpl_after_forall_fence();
#line 612 "../lib/Chai.chpl"
    forall_IBB_break_label_chpl7:;
#line 612 "../lib/Chai.chpl"
    if (error_chpl8 != nil) /* ZLINE: 612 ../lib/Chai.chpl */
#line 612 "../lib/Chai.chpl"
    {
#line 612 "../lib/Chai.chpl"
      chpl_propagate_error(error_chpl8, INT64(612), INT32(65));
#line 612 "../lib/Chai.chpl"
    }
#line 612 "../lib/Chai.chpl"
    chpl_after_forall_fence();
#line 612 "../lib/Chai.chpl"
  }
#line 612 "../lib/Chai.chpl"
  _end__ASSIGN__chpl:;
#line 612 "../lib/Chai.chpl"
  _field_destructor_tmp__chpl5 = (&chpl_gentemp2)->data;
#line 612 "../lib/Chai.chpl"
  chpl__autoDestroy19(&_field_destructor_tmp__chpl5, INT64(612), INT32(65));
#line 612 "../lib/Chai.chpl"
  _field_destructor_tmp__chpl6 = (&chpl_gentemp2)->_domain;
#line 612 "../lib/Chai.chpl"
  chpl__autoDestroy5(&_field_destructor_tmp__chpl6, INT64(612), INT32(65));
#line 612 "../lib/Chai.chpl"
  _field_destructor_tmp__chpl7 = (&chpl_gentemp)->data;
#line 612 "../lib/Chai.chpl"
  chpl__autoDestroy18(&_field_destructor_tmp__chpl7, INT64(612), INT32(65));
#line 612 "../lib/Chai.chpl"
  _field_destructor_tmp__chpl8 = (&chpl_gentemp)->_domain;
#line 612 "../lib/Chai.chpl"
  chpl__autoDestroy11(&_field_destructor_tmp__chpl8, INT64(612), INT32(65));
#line 613 "../lib/Chai.chpl"
  call_tmp_chpl82 = &((this_chpl31)->biasesGrad);
#line 613 "../lib/Chai.chpl"
  call_tmp_chpl83 = &((call_tmp_chpl82)->data);
#line 613 "../lib/Chai.chpl"
  call_tmp_chpl84 = &((&biasesGrad_chpl)->data);
#line 613 "../lib/Chai.chpl"
  if (((RootClass_chpl)((call_tmp_chpl84)->_instance)) == nil) /* ZLINE: 613 ../lib/Chai.chpl */
#line 613 "../lib/Chai.chpl"
  {
#line 613 "../lib/Chai.chpl"
    goto _end__ASSIGN__chpl2;
#line 613 "../lib/Chai.chpl"
  }
#line 613 "../lib/Chai.chpl"
  if (((RootClass_chpl)((call_tmp_chpl83)->_instance)) == ((RootClass_chpl)((call_tmp_chpl84)->_instance))) /* ZLINE: 613 ../lib/Chai.chpl */
#line 613 "../lib/Chai.chpl"
  {
#line 613 "../lib/Chai.chpl"
    goto _end__ASSIGN__chpl2;
#line 613 "../lib/Chai.chpl"
  }
#line 613 "../lib/Chai.chpl"
  call_tmp_chpl85 = sizeAs_chpl4(call_tmp_chpl83, INT64(613), INT32(65));
#line 613 "../lib/Chai.chpl"
  if (! (INT64(0) < INT64(0))) /* ZLINE: 613 ../lib/Chai.chpl */
#line 613 "../lib/Chai.chpl"
  {
#line 613 "../lib/Chai.chpl"
    tmp_chpl11 = (call_tmp_chpl85 == ((uint64_t)(INT64(0))));
#line 613 "../lib/Chai.chpl"
  } else /* ZLINE: 613 ../lib/Chai.chpl */
#line 613 "../lib/Chai.chpl"
  {
#line 613 "../lib/Chai.chpl"
    tmp_chpl11 = UINT8(false);
#line 613 "../lib/Chai.chpl"
  }
#line 613 "../lib/Chai.chpl"
  if (tmp_chpl11) /* ZLINE: 613 ../lib/Chai.chpl */
#line 613 "../lib/Chai.chpl"
  {
#line 613 "../lib/Chai.chpl"
    call_tmp_chpl86 = sizeAs_chpl4(call_tmp_chpl84, INT64(613), INT32(65));
#line 613 "../lib/Chai.chpl"
    if (! (INT64(0) < INT64(0))) /* ZLINE: 613 ../lib/Chai.chpl */
#line 613 "../lib/Chai.chpl"
    {
#line 613 "../lib/Chai.chpl"
      tmp_chpl13 = (call_tmp_chpl86 == ((uint64_t)(INT64(0))));
#line 613 "../lib/Chai.chpl"
    } else /* ZLINE: 613 ../lib/Chai.chpl */
#line 613 "../lib/Chai.chpl"
    {
#line 613 "../lib/Chai.chpl"
      tmp_chpl13 = UINT8(false);
#line 613 "../lib/Chai.chpl"
    }
#line 613 "../lib/Chai.chpl"
    tmp_chpl12 = tmp_chpl13;
#line 613 "../lib/Chai.chpl"
  } else /* ZLINE: 613 ../lib/Chai.chpl */
#line 613 "../lib/Chai.chpl"
  {
#line 613 "../lib/Chai.chpl"
    tmp_chpl12 = UINT8(false);
#line 613 "../lib/Chai.chpl"
  }
#line 613 "../lib/Chai.chpl"
  if (tmp_chpl12) /* ZLINE: 613 ../lib/Chai.chpl */
#line 613 "../lib/Chai.chpl"
  {
#line 613 "../lib/Chai.chpl"
    goto _end__ASSIGN__chpl2;
#line 613 "../lib/Chai.chpl"
  }
#line 613 "../lib/Chai.chpl"
  checkArrayShapesUponAssignment_chpl7(call_tmp_chpl83, call_tmp_chpl84, UINT8(false), INT64(613), INT32(65));
#line 613 "../lib/Chai.chpl"
  _dom_chpl18(call_tmp_chpl83, &ret_tmp_chpl38, INT64(613), INT32(65));
#line 613 "../lib/Chai.chpl"
  call_tmp_chpl87 = ret_tmp_chpl38;
#line 613 "../lib/Chai.chpl"
  _dom_chpl18(call_tmp_chpl84, &ret_tmp_chpl39, INT64(613), INT32(65));
#line 613 "../lib/Chai.chpl"
  call_tmp_chpl88 = ret_tmp_chpl39;
#line 613 "../lib/Chai.chpl"
  i_AD_chpl2 = &call_tmp_chpl87;
#line 613 "../lib/Chai.chpl"
  i_BD_chpl2 = &call_tmp_chpl88;
#line 613 "../lib/Chai.chpl"
  ret_chpl16 = (call_tmp_chpl83)->_instance;
#line 613 "../lib/Chai.chpl"
  ret_chpl17 = (call_tmp_chpl84)->_instance;
#line 613 "../lib/Chai.chpl"
  chpl_check_nil(ret_chpl16, INT64(613), INT32(65));
#line 613 "../lib/Chai.chpl"
  call_tmp_chpl89 = doiBulkTransferFromKnown_chpl9(ret_chpl16, i_AD_chpl2, ret_chpl17, i_BD_chpl2, INT64(613), INT32(65));
#line 613 "../lib/Chai.chpl"
  if (call_tmp_chpl89) /* ZLINE: 613 ../lib/Chai.chpl */
#line 613 "../lib/Chai.chpl"
  {
#line 613 "../lib/Chai.chpl"
  } else /* ZLINE: 613 ../lib/Chai.chpl */
#line 613 "../lib/Chai.chpl"
  {
#line 613 "../lib/Chai.chpl"
  }
#line 613 "../lib/Chai.chpl"
  chpl__autoDestroy11(&call_tmp_chpl88, INT64(613), INT32(65));
#line 613 "../lib/Chai.chpl"
  chpl__autoDestroy11(&call_tmp_chpl87, INT64(613), INT32(65));
#line 613 "../lib/Chai.chpl"
  if (call_tmp_chpl89) /* ZLINE: 613 ../lib/Chai.chpl */
#line 613 "../lib/Chai.chpl"
  {
#line 613 "../lib/Chai.chpl"
  }
#line 613 "../lib/Chai.chpl"
  if (! call_tmp_chpl89) /* ZLINE: 613 ../lib/Chai.chpl */
#line 613 "../lib/Chai.chpl"
  {
#line 613 "../lib/Chai.chpl"
    coerce_tmp_chpl31 = (call_tmp_chpl83)->_instance;
#line 613 "../lib/Chai.chpl"
    tasksPerLocale_chpl4 = local_dataParTasksPerLocale_chpl;
#line 613 "../lib/Chai.chpl"
    ignoreRunning_chpl4 = local_dataParIgnoreRunningTasks_chpl;
#line 613 "../lib/Chai.chpl"
    minIndicesPerTask_chpl4 = local_dataParMinGranularity_chpl;
#line 613 "../lib/Chai.chpl"
    error_chpl11 = NULL;
#line 613 "../lib/Chai.chpl"
    chpl_check_nil(coerce_tmp_chpl31, INT64(613), INT32(65));
#line 613 "../lib/Chai.chpl"
    coerce_tmp_chpl32 = (coerce_tmp_chpl31)->dom;
#line 613 "../lib/Chai.chpl"
    createTuple_chpl(INT64(0), &ret_tmp_chpl40);
#line 613 "../lib/Chai.chpl"
    offset_x0_chpl4 = *(ret_tmp_chpl40 + INT64(0));
#line 613 "../lib/Chai.chpl"
    error_chpl12 = NULL;
#line 613 "../lib/Chai.chpl"
    call_tmp_chpl92 = get_chpl_nodeID();
#line 613 "../lib/Chai.chpl"
    call_tmp_chpl93 = chpl_rt_buildLocaleID(call_tmp_chpl92, c_sublocid_any);
#line 613 "../lib/Chai.chpl"
    call_tmp_chpl91 = call_tmp_chpl93;
#line 613 "../lib/Chai.chpl"
    chpl_localeID_to_locale(&call_tmp_chpl91, &ret_tmp_chpl41, INT64(613), INT32(65));
#line 613 "../lib/Chai.chpl"
    call_tmp_chpl94 = ret_tmp_chpl41;
#line 613 "../lib/Chai.chpl"
    call_tmp_chpl90 = call_tmp_chpl94;
#line 613 "../lib/Chai.chpl"
    coerce_tmp_chpl33 = (&call_tmp_chpl90)->_instance;
#line 613 "../lib/Chai.chpl"
    if (((RootClass_chpl)(coerce_tmp_chpl33)) == nil) /* ZLINE: 613 ../lib/Chai.chpl */
#line 613 "../lib/Chai.chpl"
    {
#line 613 "../lib/Chai.chpl"
      nilCheckHalt_chpl(&_str_literal_52_chpl, INT64(613), INT32(65));
#line 613 "../lib/Chai.chpl"
    }
#line 613 "../lib/Chai.chpl"
    chpl_check_nil(coerce_tmp_chpl33, INT64(613), INT32(65));
#line 613 "../lib/Chai.chpl"
    ((int64_t(*)(BaseLocale_chpl,
#line 613 "../lib/Chai.chpl"
       int64_t,
#line 613 "../lib/Chai.chpl"
       int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(coerce_tmp_chpl33))->chpl__cid) + INT64(2))])(coerce_tmp_chpl33, INT64(613), INT32(65));
#line 613 "../lib/Chai.chpl"
    i_x_chpl11 = &call_tmp_chpl90;
#line 613 "../lib/Chai.chpl"
    deinit_chpl62(i_x_chpl11);
#line 613 "../lib/Chai.chpl"
    if (tasksPerLocale_chpl4 == INT64(0)) /* ZLINE: 613 ../lib/Chai.chpl */
#line 613 "../lib/Chai.chpl"
    {
#line 613 "../lib/Chai.chpl"
      call_tmp_chpl97 = get_chpl_nodeID();
#line 613 "../lib/Chai.chpl"
      call_tmp_chpl98 = chpl_rt_buildLocaleID(call_tmp_chpl97, c_sublocid_any);
#line 613 "../lib/Chai.chpl"
      call_tmp_chpl96 = call_tmp_chpl98;
#line 613 "../lib/Chai.chpl"
      chpl_localeID_to_locale(&call_tmp_chpl96, &ret_tmp_chpl42, INT64(613), INT32(65));
#line 613 "../lib/Chai.chpl"
      call_tmp_chpl99 = ret_tmp_chpl42;
#line 613 "../lib/Chai.chpl"
      call_tmp_chpl95 = call_tmp_chpl99;
#line 613 "../lib/Chai.chpl"
      coerce_tmp_chpl34 = (&call_tmp_chpl95)->_instance;
#line 613 "../lib/Chai.chpl"
      if (((RootClass_chpl)(coerce_tmp_chpl34)) == nil) /* ZLINE: 613 ../lib/Chai.chpl */
#line 613 "../lib/Chai.chpl"
      {
#line 613 "../lib/Chai.chpl"
        nilCheckHalt_chpl(&_str_literal_52_chpl, INT64(613), INT32(65));
#line 613 "../lib/Chai.chpl"
      }
#line 613 "../lib/Chai.chpl"
      coerce_tmp_chpl35 = coerce_tmp_chpl34;
#line 613 "../lib/Chai.chpl"
      chpl_check_nil(coerce_tmp_chpl35, INT64(613), INT32(65));
#line 613 "../lib/Chai.chpl"
      ret_chpl18 = (coerce_tmp_chpl35)->maxTaskPar;
#line 613 "../lib/Chai.chpl"
      tmp_chpl14 = ret_chpl18;
#line 613 "../lib/Chai.chpl"
      i_x_chpl12 = &call_tmp_chpl95;
#line 613 "../lib/Chai.chpl"
      deinit_chpl62(i_x_chpl12);
#line 613 "../lib/Chai.chpl"
    } else /* ZLINE: 613 ../lib/Chai.chpl */
#line 613 "../lib/Chai.chpl"
    {
#line 613 "../lib/Chai.chpl"
      tmp_chpl14 = tasksPerLocale_chpl4;
#line 613 "../lib/Chai.chpl"
    }
#line 613 "../lib/Chai.chpl"
    if (chpl_task_getSerial()) /* ZLINE: 613 ../lib/Chai.chpl */
#line 613 "../lib/Chai.chpl"
    {
#line 613 "../lib/Chai.chpl"
      tmp_x0_chpl4 = INT64(1);
#line 613 "../lib/Chai.chpl"
      tmp_x1_chpl4 = INT64(0);
#line 613 "../lib/Chai.chpl"
    } else /* ZLINE: 613 ../lib/Chai.chpl */
#line 613 "../lib/Chai.chpl"
    {
#line 613 "../lib/Chai.chpl"
      chpl_check_nil(coerce_tmp_chpl32, INT64(613), INT32(65));
#line 613 "../lib/Chai.chpl"
      call_tmp_chpl100 = &((coerce_tmp_chpl32)->ranges);
#line 613 "../lib/Chai.chpl"
      _computeChunkStuff_chpl(tmp_chpl14, ignoreRunning_chpl4, minIndicesPerTask_chpl4, call_tmp_chpl100, &ret_tmp_chpl43, INT64(613), INT32(65));
#line 613 "../lib/Chai.chpl"
      tmp_x0_chpl4 = *(ret_tmp_chpl43 + INT64(0));
#line 613 "../lib/Chai.chpl"
      tmp_x1_chpl4 = *(ret_tmp_chpl43 + INT64(1));
#line 613 "../lib/Chai.chpl"
    }
#line 613 "../lib/Chai.chpl"
    numChunks_chpl4 = tmp_x0_chpl4;
#line 613 "../lib/Chai.chpl"
    parDim_chpl4 = tmp_x1_chpl4;
#line 613 "../lib/Chai.chpl"
    init_chpl113(&elt_x0_chpl4);
#line 613 "../lib/Chai.chpl"
    tup_x0_chpl4 = elt_x0_chpl4;
#line 613 "../lib/Chai.chpl"
    *(locBlock_chpl2 + INT64(0)) = tup_x0_chpl4;
#line 613 "../lib/Chai.chpl"
    chpl_build_low_bounded_range2(offset_x0_chpl4, &ret_tmp_chpl44);
#line 613 "../lib/Chai.chpl"
    call_tmp_chpl101 = ret_tmp_chpl44;
#line 613 "../lib/Chai.chpl"
    chpl_check_nil(coerce_tmp_chpl32, INT64(613), INT32(65));
#line 613 "../lib/Chai.chpl"
    call_tmp_chpl102 = &((coerce_tmp_chpl32)->ranges);
#line 613 "../lib/Chai.chpl"
    coerce_tmp_chpl36 = *(*(call_tmp_chpl102) + INT64(0));
#line 613 "../lib/Chai.chpl"
    call_tmp_chpl103 = sizeAs_chpl37(coerce_tmp_chpl36, INT64(613), INT32(65));
#line 613 "../lib/Chai.chpl"
    chpl___POUND_3(call_tmp_chpl101, call_tmp_chpl103, &ret_tmp_chpl45, INT64(613), INT32(65));
#line 613 "../lib/Chai.chpl"
    call_tmp_chpl104 = ret_tmp_chpl45;
#line 613 "../lib/Chai.chpl"
    *(locBlock_chpl2 + INT64(0)) = call_tmp_chpl104;
#line 613 "../lib/Chai.chpl"
    chpl_build_low_bounded_range2(INT64(0), &ret_tmp_chpl46);
#line 613 "../lib/Chai.chpl"
    call_tmp_chpl105 = ret_tmp_chpl46;
#line 613 "../lib/Chai.chpl"
    chpl___POUND_3(call_tmp_chpl105, tmp_x0_chpl4, &ret_tmp_chpl47, INT64(613), INT32(65));
#line 613 "../lib/Chai.chpl"
    tmpIter_chpl4 = ret_tmp_chpl47;
#line 613 "../lib/Chai.chpl"
    new_temp_chpl6 = _new_chpl17(INT64(613), INT32(65));
#line 613 "../lib/Chai.chpl"
    _coforallCount_chpl4 = new_temp_chpl6;
#line 613 "../lib/Chai.chpl"
    numTasks_chpl4 = chpl_boundedCoforallSize(tmpIter_chpl4, INT64(613), INT32(65));
#line 613 "../lib/Chai.chpl"
    _upEndCount_chpl(_coforallCount_chpl4, numTasks_chpl4, INT64(613), INT32(65));
#line 613 "../lib/Chai.chpl"
    chpl_resetTaskSpawn(numTasks_chpl4, INT64(613), INT32(65));
#line 613 "../lib/Chai.chpl"
    _ic__F0_this_chpl4 = tmpIter_chpl4;
#line 613 "../lib/Chai.chpl"
    checkIfIterWillOverflow_chpl(tmpIter_chpl4, UINT8(true), INT64(613), INT32(65));
#line 613 "../lib/Chai.chpl"
    i_chpl4 = INT64(0);
#line 613 "../lib/Chai.chpl"
    this_chpl38 = _ic__F0_this_chpl4;
#line 613 "../lib/Chai.chpl"
    this_chpl39 = _ic__F0_this_chpl4;
#line 613 "../lib/Chai.chpl"
    end_chpl4 = (&this_chpl39)->_high;
#line 613 "../lib/Chai.chpl"
    for (i_chpl4 = (&this_chpl38)->_low; ((i_chpl4 <= end_chpl4)); i_chpl4 += INT64(1)) {
#line 613 "../lib/Chai.chpl"
      _args_forcoforall_fn_chpl4 = ((_class_localscoforall_fn_chpl24)(&chpl_macro_tmp_120));
#line 613 "../lib/Chai.chpl"
      chpl_check_nil(_args_forcoforall_fn_chpl4, INT64(613), INT32(65));
#line 613 "../lib/Chai.chpl"
      (_args_forcoforall_fn_chpl4)->_0_numChunks = numChunks_chpl4;
#line 613 "../lib/Chai.chpl"
      chpl_check_nil(_args_forcoforall_fn_chpl4, INT64(613), INT32(65));
#line 613 "../lib/Chai.chpl"
      (_args_forcoforall_fn_chpl4)->_1_parDim = parDim_chpl4;
#line 613 "../lib/Chai.chpl"
      chpl_check_nil(_args_forcoforall_fn_chpl4, INT64(613), INT32(65));
#line 613 "../lib/Chai.chpl"
      *((_args_forcoforall_fn_chpl4)->_2_coerce_tmp + INT64(0)) = *(locBlock_chpl2 + INT64(0));
#line 613 "../lib/Chai.chpl"
      chpl_check_nil(_args_forcoforall_fn_chpl4, INT64(613), INT32(65));
#line 613 "../lib/Chai.chpl"
      (_args_forcoforall_fn_chpl4)->_3__coforallCount = _coforallCount_chpl4;
#line 613 "../lib/Chai.chpl"
      chpl_check_nil(_args_forcoforall_fn_chpl4, INT64(613), INT32(65));
#line 613 "../lib/Chai.chpl"
      (_args_forcoforall_fn_chpl4)->_4_chunk = i_chpl4;
#line 613 "../lib/Chai.chpl"
      chpl_check_nil(_args_forcoforall_fn_chpl4, INT64(613), INT32(65));
#line 613 "../lib/Chai.chpl"
      (_args_forcoforall_fn_chpl4)->_6_a = *(call_tmp_chpl83);
#line 613 "../lib/Chai.chpl"
      chpl_check_nil(_args_forcoforall_fn_chpl4, INT64(613), INT32(65));
#line 613 "../lib/Chai.chpl"
      (_args_forcoforall_fn_chpl4)->_7_b = *(call_tmp_chpl84);
#line 613 "../lib/Chai.chpl"
      _args_vforcoforall_fn_chpl4 = ((void*)(_args_forcoforall_fn_chpl4));
#line 613 "../lib/Chai.chpl"
      _args_sizecoforall_fn_chpl2 = sizeof(chpl__class_localscoforall_fn_chpl24_object);
#line 613 "../lib/Chai.chpl"
      (_args_forcoforall_fn_chpl4)->_ln = INT64(613);
#line 613 "../lib/Chai.chpl"
      (_args_forcoforall_fn_chpl4)->_fn = INT32(65);
#line 613 "../lib/Chai.chpl"
      /*** wrapcoforall_fn_chpl33 ***/ chpl_taskAddCoStmt(chpl_task_getRequestedSubloc(), INT64(32), ((chpl_task_bundle_p)(_args_vforcoforall_fn_chpl4)), _args_sizecoforall_fn_chpl2, INT64(2763), INT32(30));
#line 613 "../lib/Chai.chpl"
    }
#line 613 "../lib/Chai.chpl"
    error_chpl13 = NULL;
#line 613 "../lib/Chai.chpl"
    _waitEndCount_chpl2(_coforallCount_chpl4, numTasks_chpl4, &error_chpl13, INT64(613), INT32(65));
#line 613 "../lib/Chai.chpl"
    if (error_chpl13 != nil) /* ZLINE: 613 ../lib/Chai.chpl */
#line 613 "../lib/Chai.chpl"
    {
#line 613 "../lib/Chai.chpl"
      chpl__delete72(_coforallCount_chpl4, INT64(613), INT32(65));
#line 613 "../lib/Chai.chpl"
      error_chpl12 = error_chpl13;
#line 613 "../lib/Chai.chpl"
      goto forall_IBB_break_label_chpl8;
#line 613 "../lib/Chai.chpl"
    }
#line 613 "../lib/Chai.chpl"
    chpl__delete72(_coforallCount_chpl4, INT64(613), INT32(65));
#line 613 "../lib/Chai.chpl"
    chpl_after_forall_fence();
#line 613 "../lib/Chai.chpl"
    forall_IBB_break_label_chpl8:;
#line 613 "../lib/Chai.chpl"
    if (error_chpl12 != nil) /* ZLINE: 613 ../lib/Chai.chpl */
#line 613 "../lib/Chai.chpl"
    {
#line 613 "../lib/Chai.chpl"
      error_chpl11 = error_chpl12;
#line 613 "../lib/Chai.chpl"
      goto forall_IBB_break_label_chpl9;
#line 613 "../lib/Chai.chpl"
    }
#line 613 "../lib/Chai.chpl"
    chpl_after_forall_fence();
#line 613 "../lib/Chai.chpl"
    forall_IBB_break_label_chpl9:;
#line 613 "../lib/Chai.chpl"
    if (error_chpl11 != nil) /* ZLINE: 613 ../lib/Chai.chpl */
#line 613 "../lib/Chai.chpl"
    {
#line 613 "../lib/Chai.chpl"
      chpl_propagate_error(error_chpl11, INT64(613), INT32(65));
#line 613 "../lib/Chai.chpl"
    }
#line 613 "../lib/Chai.chpl"
    chpl_after_forall_fence();
#line 613 "../lib/Chai.chpl"
  }
#line 613 "../lib/Chai.chpl"
  _end__ASSIGN__chpl2:;
#line 614 "../lib/Chai.chpl"
  chpl__checkRetEltTypeMatch2(&newDeltas_chpl);
#line 614 "../lib/Chai.chpl"
  ret_chpl = newDeltas_chpl;
#line 614 "../lib/Chai.chpl"
  _field_destructor_tmp__chpl9 = (&biasesGrad_chpl)->data;
#line 614 "../lib/Chai.chpl"
  chpl__autoDestroy18(&_field_destructor_tmp__chpl9, INT64(614), INT32(65));
#line 614 "../lib/Chai.chpl"
  _field_destructor_tmp__chpl10 = (&biasesGrad_chpl)->_domain;
#line 614 "../lib/Chai.chpl"
  chpl__autoDestroy11(&_field_destructor_tmp__chpl10, INT64(614), INT32(65));
#line 614 "../lib/Chai.chpl"
  _field_destructor_tmp__chpl11 = (&weightsGrad_chpl)->data;
#line 614 "../lib/Chai.chpl"
  chpl__autoDestroy19(&_field_destructor_tmp__chpl11, INT64(614), INT32(65));
#line 614 "../lib/Chai.chpl"
  _field_destructor_tmp__chpl12 = (&weightsGrad_chpl)->_domain;
#line 614 "../lib/Chai.chpl"
  chpl__autoDestroy5(&_field_destructor_tmp__chpl12, INT64(614), INT32(65));
#line 614 "../lib/Chai.chpl"
  chpl__autoDestroy11(&call_tmp_chpl9, INT64(614), INT32(65));
#line 581 "../lib/Chai.chpl"
  *(_retArg_chpl) = ret_chpl;
#line 581 "../lib/Chai.chpl"
  return;
#line 581 "../lib/Chai.chpl"
}

#line 584 "../lib/Chai.chpl"
/* Chai.chpl:584 */
#line 584 "../lib/Chai.chpl"
static void wrapcoforall_fn_chpl112(_class_localscoforall_fn_chpl112 c_chpl) {
#line 584 "../lib/Chai.chpl"
  Error_chpl error_chpl = NULL;
#line 584 "../lib/Chai.chpl"
  int64_t _0_numChunks_chpl;
#line 584 "../lib/Chai.chpl"
  int64_t _1_parDim_chpl;
#line 584 "../lib/Chai.chpl"
  _tuple_1_star_range_int64_t_both_one_chpl _2_coerce_tmp_chpl;
#line 584 "../lib/Chai.chpl"
  chpl___EndCount_AtomicT_int64_t_int64_t _3__coforallCount_chpl = NULL;
#line 584 "../lib/Chai.chpl"
  int64_t _4_chunk_chpl;
#line 584 "../lib/Chai.chpl"
  _ref__ir_chpl_promo7_reshapeDomain__ref__array_DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl _6_chpl__iterLF_chpl = NULL;
#line 584 "../lib/Chai.chpl"
  chpl_rmem_consist_acquire(INT64(584), INT32(65));
#line 584 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(584), INT32(65));
#line 584 "../lib/Chai.chpl"
  _0_numChunks_chpl = (c_chpl)->_0_numChunks;
#line 584 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(584), INT32(65));
#line 584 "../lib/Chai.chpl"
  _1_parDim_chpl = (c_chpl)->_1_parDim;
#line 584 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(584), INT32(65));
#line 584 "../lib/Chai.chpl"
  *(_2_coerce_tmp_chpl + INT64(0)) = *((c_chpl)->_2_coerce_tmp + INT64(0));
#line 584 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(584), INT32(65));
#line 584 "../lib/Chai.chpl"
  _3__coforallCount_chpl = (c_chpl)->_3__coforallCount;
#line 584 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(584), INT32(65));
#line 584 "../lib/Chai.chpl"
  _4_chunk_chpl = (c_chpl)->_4_chunk;
#line 584 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(584), INT32(65));
#line 584 "../lib/Chai.chpl"
  _6_chpl__iterLF_chpl = (c_chpl)->_6_chpl__iterLF;
#line 584 "../lib/Chai.chpl"
  coforall_fn_chpl112(_0_numChunks_chpl, _1_parDim_chpl, &_2_coerce_tmp_chpl, _3__coforallCount_chpl, _4_chunk_chpl, &error_chpl, _6_chpl__iterLF_chpl);
#line 584 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(584), INT32(65));
#line 584 "../lib/Chai.chpl"
  _downEndCount_chpl((c_chpl)->_3__coforallCount, error_chpl, INT64(584), INT32(65));
#line 584 "../lib/Chai.chpl"
  return;
#line 584 "../lib/Chai.chpl"
}

#line 584 "../lib/Chai.chpl"
/* Chai.chpl:584 */
#line 584 "../lib/Chai.chpl"
static void coforall_fn_chpl112(int64_t numChunks_chpl,
#line 584 "../lib/Chai.chpl"
                                int64_t parDim_chpl,
#line 584 "../lib/Chai.chpl"
                                _tuple_1_star_range_int64_t_both_one_chpl * locBlock_chpl,
#line 584 "../lib/Chai.chpl"
                                chpl___EndCount_AtomicT_int64_t_int64_t _coforallCount_chpl,
#line 584 "../lib/Chai.chpl"
                                int64_t chunk_chpl,
#line 584 "../lib/Chai.chpl"
                                Error_chpl * error_out_chpl,
#line 584 "../lib/Chai.chpl"
                                _ir_chpl_promo7_reshapeDomain__ref__array_DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl * chpl__iterLF) {
#line 584 "../lib/Chai.chpl"
  _tuple_1_star_range_int64_t_both_one_chpl followMe_chpl;
#line 584 "../lib/Chai.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl6 = NULL;
#line 584 "../lib/Chai.chpl"
  int64_t call_tmp_chpl7;
#line 584 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl3;
#line 584 "../lib/Chai.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl8 = NULL;
#line 584 "../lib/Chai.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl9 = NULL;
#line 584 "../lib/Chai.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl10 = NULL;
#line 584 "../lib/Chai.chpl"
  _tuple_2_star_int64_t_chpl call_tmp_chpl11;
#line 584 "../lib/Chai.chpl"
  _tuple_2_star_int64_t_chpl ret_tmp_chpl;
#line 584 "../lib/Chai.chpl"
  int64_t tmp_x0_chpl;
#line 584 "../lib/Chai.chpl"
  int64_t tmp_x1_chpl;
#line 584 "../lib/Chai.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl12 = NULL;
#line 584 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl13;
#line 584 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl2;
#line 584 "../lib/Chai.chpl"
  _tuple_1_star_range_int64_t_both_one_chpl _yield_expr_tmp__chpl;
#line 584 "../lib/Chai.chpl"
  _tuple_1_star_range_int64_t_both_one_chpl chpl_followThis;
#line 584 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl tmp_chpl;
#line 584 "../lib/Chai.chpl"
  _domain_DefaultRectangularDom_3_int64_t_one_chpl tmp_chpl2;
#line 584 "../lib/Chai.chpl"
  _domain_DefaultRectangularDom_3_int64_t_one_chpl _ic__F1_d_chpl;
#line 584 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl this_chpl31;
#line 584 "../lib/Chai.chpl"
  DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl ret_chpl = NULL;
#line 584 "../lib/Chai.chpl"
  DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl _ic__F0_this_chpl = NULL;
#line 584 "../lib/Chai.chpl"
  DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl this_chpl32 = NULL;
#line 584 "../lib/Chai.chpl"
  DefaultRectangularDom_1_int64_t_one_chpl coerce_tmp_chpl4 = NULL;
#line 584 "../lib/Chai.chpl"
  _tuple_1_star_int64_t_chpl ret_tmp_chpl3;
#line 584 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl block_x0_chpl;
#line 584 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl tup_x0_chpl;
#line 584 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl elt_x0_chpl;
#line 584 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl5;
#line 584 "../lib/Chai.chpl"
  int64_t coerce_tmp_chpl6;
#line 584 "../lib/Chai.chpl"
  DefaultRectangularDom_1_int64_t_one_chpl this_chpl33 = NULL;
#line 584 "../lib/Chai.chpl"
  _ref__tuple_1_star_range_int64_t_both_one_chpl call_tmp_chpl14 = NULL;
#line 584 "../lib/Chai.chpl"
  uint64_t call_tmp_chpl15;
#line 584 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl7;
#line 584 "../lib/Chai.chpl"
  chpl_bool tmp_chpl3;
#line 584 "../lib/Chai.chpl"
  DefaultRectangularDom_1_int64_t_one_chpl this_chpl34 = NULL;
#line 584 "../lib/Chai.chpl"
  _ref__tuple_1_star_range_int64_t_both_one_chpl call_tmp_chpl16 = NULL;
#line 584 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl8;
#line 584 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl9;
#line 584 "../lib/Chai.chpl"
  int64_t call_tmp_chpl17;
#line 584 "../lib/Chai.chpl"
  DefaultRectangularDom_1_int64_t_one_chpl this_chpl35 = NULL;
#line 584 "../lib/Chai.chpl"
  _ref__tuple_1_star_range_int64_t_both_one_chpl call_tmp_chpl18 = NULL;
#line 584 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl10;
#line 584 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl11;
#line 584 "../lib/Chai.chpl"
  int64_t call_tmp_chpl19;
#line 584 "../lib/Chai.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl20 = NULL;
#line 584 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl retcopy_x0_chpl;
#line 584 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl _ic__F0_this_chpl2;
#line 584 "../lib/Chai.chpl"
  int64_t i_chpl;
#line 584 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl this_chpl36;
#line 584 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl this_chpl37;
#line 584 "../lib/Chai.chpl"
  int64_t end_chpl;
#line 584 "../lib/Chai.chpl"
  DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl this_chpl38 = NULL;
#line 584 "../lib/Chai.chpl"
  _ddata_Tensor_3__real64_chpl coerce_tmp_chpl12 = NULL;
#line 584 "../lib/Chai.chpl"
  _ref_Tensor_3__real64_chpl call_tmp_chpl21 = NULL;
#line 584 "../lib/Chai.chpl"
  _domain_DefaultRectangularDom_3_int64_t_one_chpl d_chpl;
#line 584 "../lib/Chai.chpl"
  *(followMe_chpl + INT64(0)) = *(*(locBlock_chpl) + INT64(0));
#line 584 "../lib/Chai.chpl"
  call_tmp_chpl6 = this_chpl20(locBlock_chpl, parDim_chpl, INT64(584), INT32(65));
#line 584 "../lib/Chai.chpl"
  coerce_tmp_chpl3 = *(call_tmp_chpl6);
#line 584 "../lib/Chai.chpl"
  call_tmp_chpl7 = sizeAs_chpl37(coerce_tmp_chpl3, INT64(584), INT32(65));
#line 584 "../lib/Chai.chpl"
  call_tmp_chpl8 = this_chpl20(locBlock_chpl, parDim_chpl, INT64(584), INT32(65));
#line 584 "../lib/Chai.chpl"
  call_tmp_chpl9 = this_chpl20(locBlock_chpl, parDim_chpl, INT64(584), INT32(65));
#line 584 "../lib/Chai.chpl"
  call_tmp_chpl10 = this_chpl20(locBlock_chpl, parDim_chpl, INT64(584), INT32(65));
#line 584 "../lib/Chai.chpl"
  _computeBlock_chpl(call_tmp_chpl7, numChunks_chpl, chunk_chpl, (call_tmp_chpl8)->_high, (call_tmp_chpl9)->_low, (call_tmp_chpl10)->_low, &ret_tmp_chpl, INT64(584), INT32(65));
#line 584 "../lib/Chai.chpl"
  *(call_tmp_chpl11 + INT64(0)) = *(ret_tmp_chpl + INT64(0));
#line 584 "../lib/Chai.chpl"
  *(call_tmp_chpl11 + INT64(1)) = *(ret_tmp_chpl + INT64(1));
#line 584 "../lib/Chai.chpl"
  tmp_x0_chpl = *(call_tmp_chpl11 + INT64(0));
#line 584 "../lib/Chai.chpl"
  tmp_x1_chpl = *(call_tmp_chpl11 + INT64(1));
#line 584 "../lib/Chai.chpl"
  call_tmp_chpl12 = this_chpl20(&followMe_chpl, parDim_chpl, INT64(584), INT32(65));
#line 584 "../lib/Chai.chpl"
  chpl_build_bounded_range6(tmp_x0_chpl, tmp_x1_chpl, &ret_tmp_chpl2);
#line 584 "../lib/Chai.chpl"
  call_tmp_chpl13 = ret_tmp_chpl2;
#line 584 "../lib/Chai.chpl"
  *(call_tmp_chpl12) = call_tmp_chpl13;
#line 584 "../lib/Chai.chpl"
  *(_yield_expr_tmp__chpl + INT64(0)) = *(followMe_chpl + INT64(0));
#line 584 "../lib/Chai.chpl"
  *(chpl_followThis + INT64(0)) = *(_yield_expr_tmp__chpl + INT64(0));
#line 584 "../lib/Chai.chpl"
  tmp_chpl = (chpl__iterLF)->F0_this;
#line 584 "../lib/Chai.chpl"
  tmp_chpl2 = (chpl__iterLF)->F1_d;
#line 584 "../lib/Chai.chpl"
  _ic__F1_d_chpl = tmp_chpl2;
#line 584 "../lib/Chai.chpl"
  this_chpl31 = tmp_chpl;
#line 584 "../lib/Chai.chpl"
  ret_chpl = (&this_chpl31)->_instance;
#line 584 "../lib/Chai.chpl"
  _ic__F0_this_chpl = ret_chpl;
#line 584 "../lib/Chai.chpl"
  this_chpl32 = ret_chpl;
#line 584 "../lib/Chai.chpl"
  chpl_check_nil(this_chpl32, INT64(584), INT32(65));
#line 584 "../lib/Chai.chpl"
  coerce_tmp_chpl4 = (this_chpl32)->dom;
#line 584 "../lib/Chai.chpl"
  createTuple_chpl(INT64(0), &ret_tmp_chpl3);
#line 584 "../lib/Chai.chpl"
  init_chpl113(&elt_x0_chpl);
#line 584 "../lib/Chai.chpl"
  tup_x0_chpl = elt_x0_chpl;
#line 584 "../lib/Chai.chpl"
  block_x0_chpl = tup_x0_chpl;
#line 584 "../lib/Chai.chpl"
  coerce_tmp_chpl5 = *(chpl_followThis + INT64(0));
#line 584 "../lib/Chai.chpl"
  coerce_tmp_chpl6 = (&coerce_tmp_chpl5)->_high;
#line 584 "../lib/Chai.chpl"
  this_chpl33 = coerce_tmp_chpl4;
#line 584 "../lib/Chai.chpl"
  chpl_check_nil(this_chpl33, INT64(584), INT32(65));
#line 584 "../lib/Chai.chpl"
  call_tmp_chpl14 = &((this_chpl33)->ranges);
#line 584 "../lib/Chai.chpl"
  coerce_tmp_chpl7 = *(*(call_tmp_chpl14) + INT64(0));
#line 584 "../lib/Chai.chpl"
  call_tmp_chpl15 = sizeAs_chpl36(coerce_tmp_chpl7, INT64(584), INT32(65));
#line 584 "../lib/Chai.chpl"
  if (! (coerce_tmp_chpl6 < INT64(0))) /* ZLINE: 584 ../lib/Chai.chpl */
#line 584 "../lib/Chai.chpl"
  {
#line 584 "../lib/Chai.chpl"
    tmp_chpl3 = (((uint64_t)(coerce_tmp_chpl6)) >= call_tmp_chpl15);
#line 584 "../lib/Chai.chpl"
  } else /* ZLINE: 584 ../lib/Chai.chpl */
#line 584 "../lib/Chai.chpl"
  {
#line 584 "../lib/Chai.chpl"
    tmp_chpl3 = UINT8(false);
#line 584 "../lib/Chai.chpl"
  }
#line 584 "../lib/Chai.chpl"
  if (tmp_chpl3) /* ZLINE: 584 ../lib/Chai.chpl */
#line 584 "../lib/Chai.chpl"
  {
#line 584 "../lib/Chai.chpl"
    boundsCheckHalt_chpl(&_str_literal_4152_chpl, INT64(584), INT32(65));
#line 584 "../lib/Chai.chpl"
  }
#line 584 "../lib/Chai.chpl"
  this_chpl34 = coerce_tmp_chpl4;
#line 584 "../lib/Chai.chpl"
  chpl_check_nil(this_chpl34, INT64(584), INT32(65));
#line 584 "../lib/Chai.chpl"
  call_tmp_chpl16 = &((this_chpl34)->ranges);
#line 584 "../lib/Chai.chpl"
  coerce_tmp_chpl8 = *(*(call_tmp_chpl16) + INT64(0));
#line 584 "../lib/Chai.chpl"
  coerce_tmp_chpl9 = *(chpl_followThis + INT64(0));
#line 584 "../lib/Chai.chpl"
  call_tmp_chpl17 = ((&coerce_tmp_chpl8)->_low + ((int64_t)(((&coerce_tmp_chpl9)->_low * INT64(1)))));
#line 584 "../lib/Chai.chpl"
  this_chpl35 = coerce_tmp_chpl4;
#line 584 "../lib/Chai.chpl"
  chpl_check_nil(this_chpl35, INT64(584), INT32(65));
#line 584 "../lib/Chai.chpl"
  call_tmp_chpl18 = &((this_chpl35)->ranges);
#line 584 "../lib/Chai.chpl"
  coerce_tmp_chpl10 = *(*(call_tmp_chpl18) + INT64(0));
#line 584 "../lib/Chai.chpl"
  coerce_tmp_chpl11 = *(chpl_followThis + INT64(0));
#line 584 "../lib/Chai.chpl"
  call_tmp_chpl19 = ((&coerce_tmp_chpl10)->_low + ((int64_t)(((&coerce_tmp_chpl11)->_high * INT64(1)))));
#line 584 "../lib/Chai.chpl"
  call_tmp_chpl20 = &block_x0_chpl;
#line 584 "../lib/Chai.chpl"
  compilerAssert_chpl();
#line 584 "../lib/Chai.chpl"
  (call_tmp_chpl20)->_low = call_tmp_chpl17;
#line 584 "../lib/Chai.chpl"
  (call_tmp_chpl20)->_high = call_tmp_chpl19;
#line 584 "../lib/Chai.chpl"
  retcopy_x0_chpl = block_x0_chpl;
#line 584 "../lib/Chai.chpl"
  _ic__F0_this_chpl2 = retcopy_x0_chpl;
#line 584 "../lib/Chai.chpl"
  checkIfIterWillOverflow_chpl(retcopy_x0_chpl, UINT8(true), INT64(584), INT32(65));
#line 584 "../lib/Chai.chpl"
  i_chpl = INT64(0);
#line 584 "../lib/Chai.chpl"
  this_chpl36 = _ic__F0_this_chpl2;
#line 584 "../lib/Chai.chpl"
  this_chpl37 = _ic__F0_this_chpl2;
#line 584 "../lib/Chai.chpl"
  end_chpl = (&this_chpl37)->_high;
#line 584 "../lib/Chai.chpl"
  for (i_chpl = (&this_chpl36)->_low; ((i_chpl <= end_chpl)); i_chpl += INT64(1)) {
#line 584 "../lib/Chai.chpl"
    this_chpl38 = _ic__F0_this_chpl;
#line 584 "../lib/Chai.chpl"
    chpl_check_nil(this_chpl38, INT64(584), INT32(65));
#line 584 "../lib/Chai.chpl"
    coerce_tmp_chpl12 = (this_chpl38)->shiftedData;
#line 584 "../lib/Chai.chpl"
    call_tmp_chpl21 = (coerce_tmp_chpl12 + i_chpl);
#line 584 "../lib/Chai.chpl"
    d_chpl = _ic__F1_d_chpl;
#line 584 "../lib/Chai.chpl"
    reshapeDomain_chpl3(call_tmp_chpl21, &d_chpl);
#line 584 "../lib/Chai.chpl"
  }
#line 584 "../lib/Chai.chpl"
  return;
#line 584 "../lib/Chai.chpl"
}

#line 588 "../lib/Chai.chpl"
/* Chai.chpl:588 */
#line 588 "../lib/Chai.chpl"
static void wrapcoforall_fn_chpl113(_class_localscoforall_fn_chpl113 c_chpl) {
#line 588 "../lib/Chai.chpl"
  Error_chpl error_chpl = NULL;
#line 588 "../lib/Chai.chpl"
  int64_t _0_numChunks_chpl;
#line 588 "../lib/Chai.chpl"
  int64_t _1_parDim_chpl;
#line 588 "../lib/Chai.chpl"
  _tuple_1_star_range_int64_t_both_one_chpl _2_coerce_tmp_chpl;
#line 588 "../lib/Chai.chpl"
  chpl___EndCount_AtomicT_int64_t_int64_t _3__coforallCount_chpl = NULL;
#line 588 "../lib/Chai.chpl"
  int64_t _4_chunk_chpl;
#line 588 "../lib/Chai.chpl"
  _ref_SoftMax_chpl _6_this_chpl = NULL;
#line 588 "../lib/Chai.chpl"
  SumReduceScanOp_Tensor_2__real64_chpl _7_globalRP_weightsGrad_chpl = NULL;
#line 588 "../lib/Chai.chpl"
  SumReduceScanOp_Tensor_1__real64_chpl _8_globalRP_biasesGrad_chpl = NULL;
#line 588 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_1_int64_t_one_Tensor_1__real64_int64_t_chpl _9_deltas_chpl;
#line 588 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl _10_inputs_chpl;
#line 588 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl _11_newDeltas_chpl;
#line 588 "../lib/Chai.chpl"
  _domain_DefaultRectangularDom_1_int64_t_one_chpl _12_call_tmp_chpl;
#line 588 "../lib/Chai.chpl"
  chpl_rmem_consist_acquire(INT64(588), INT32(65));
#line 588 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(588), INT32(65));
#line 588 "../lib/Chai.chpl"
  _0_numChunks_chpl = (c_chpl)->_0_numChunks;
#line 588 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(588), INT32(65));
#line 588 "../lib/Chai.chpl"
  _1_parDim_chpl = (c_chpl)->_1_parDim;
#line 588 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(588), INT32(65));
#line 588 "../lib/Chai.chpl"
  *(_2_coerce_tmp_chpl + INT64(0)) = *((c_chpl)->_2_coerce_tmp + INT64(0));
#line 588 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(588), INT32(65));
#line 588 "../lib/Chai.chpl"
  _3__coforallCount_chpl = (c_chpl)->_3__coforallCount;
#line 588 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(588), INT32(65));
#line 588 "../lib/Chai.chpl"
  _4_chunk_chpl = (c_chpl)->_4_chunk;
#line 588 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(588), INT32(65));
#line 588 "../lib/Chai.chpl"
  _6_this_chpl = (c_chpl)->_6_this;
#line 588 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(588), INT32(65));
#line 588 "../lib/Chai.chpl"
  _7_globalRP_weightsGrad_chpl = (c_chpl)->_7_globalRP_weightsGrad;
#line 588 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(588), INT32(65));
#line 588 "../lib/Chai.chpl"
  _8_globalRP_biasesGrad_chpl = (c_chpl)->_8_globalRP_biasesGrad;
#line 588 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(588), INT32(65));
#line 588 "../lib/Chai.chpl"
  _9_deltas_chpl = (c_chpl)->_9_deltas;
#line 588 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(588), INT32(65));
#line 588 "../lib/Chai.chpl"
  _10_inputs_chpl = (c_chpl)->_10_inputs;
#line 588 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(588), INT32(65));
#line 588 "../lib/Chai.chpl"
  _11_newDeltas_chpl = (c_chpl)->_11_newDeltas;
#line 588 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(588), INT32(65));
#line 588 "../lib/Chai.chpl"
  _12_call_tmp_chpl = (c_chpl)->_12_call_tmp;
#line 588 "../lib/Chai.chpl"
  coforall_fn_chpl113(_0_numChunks_chpl, _1_parDim_chpl, &_2_coerce_tmp_chpl, _3__coforallCount_chpl, _4_chunk_chpl, &error_chpl, _6_this_chpl, _7_globalRP_weightsGrad_chpl, _8_globalRP_biasesGrad_chpl, &_9_deltas_chpl, &_10_inputs_chpl, &_11_newDeltas_chpl, &_12_call_tmp_chpl);
#line 588 "../lib/Chai.chpl"
  chpl_check_nil(c_chpl, INT64(588), INT32(65));
#line 588 "../lib/Chai.chpl"
  _downEndCount_chpl((c_chpl)->_3__coforallCount, error_chpl, INT64(588), INT32(65));
#line 588 "../lib/Chai.chpl"
  return;
#line 588 "../lib/Chai.chpl"
}

#line 588 "../lib/Chai.chpl"
/* Chai.chpl:588 */
#line 588 "../lib/Chai.chpl"
static void coforall_fn_chpl113(int64_t numChunks_chpl,
#line 588 "../lib/Chai.chpl"
                                int64_t parDim_chpl,
#line 588 "../lib/Chai.chpl"
                                _tuple_1_star_range_int64_t_both_one_chpl * locBlock_chpl,
#line 588 "../lib/Chai.chpl"
                                chpl___EndCount_AtomicT_int64_t_int64_t _coforallCount_chpl,
#line 588 "../lib/Chai.chpl"
                                int64_t chunk_chpl,
#line 588 "../lib/Chai.chpl"
                                Error_chpl * error_out_chpl,
#line 588 "../lib/Chai.chpl"
                                SoftMax_chpl * this_chpl31,
#line 588 "../lib/Chai.chpl"
                                SumReduceScanOp_Tensor_2__real64_chpl RP_weightsGrad_chpl,
#line 588 "../lib/Chai.chpl"
                                SumReduceScanOp_Tensor_1__real64_chpl RP_biasesGrad_chpl,
#line 588 "../lib/Chai.chpl"
                                _array_DefaultRectangularArr_1_int64_t_one_Tensor_1__real64_int64_t_chpl * deltas_chpl,
#line 588 "../lib/Chai.chpl"
                                _array_DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl * inputs_chpl,
#line 588 "../lib/Chai.chpl"
                                _array_DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl * newDeltas_chpl,
#line 588 "../lib/Chai.chpl"
                                _domain_DefaultRectangularDom_1_int64_t_one_chpl * call_tmp_chpl6) {
#line 588 "../lib/Chai.chpl"
  SumReduceScanOp_Tensor_2__real64_chpl RP_weightsGrad_chpl2 = NULL;
#line 588 "../lib/Chai.chpl"
  Tensor_2__real64_chpl AS_weightsGrad_chpl;
#line 588 "../lib/Chai.chpl"
  SumReduceScanOp_Tensor_2__real64_chpl new_temp_chpl = NULL;
#line 588 "../lib/Chai.chpl"
  chpl_LocalSpinlock default_arg_l_chpl;
#line 588 "../lib/Chai.chpl"
  chpl_LocalSpinlock ret_chpl;
#line 588 "../lib/Chai.chpl"
  chpl_LocalSpinlock l_chpl;
#line 588 "../lib/Chai.chpl"
  Tensor_2__real64_chpl default_arg_value_chpl;
#line 588 "../lib/Chai.chpl"
  Tensor_2__real64_chpl ret_chpl2;
#line 588 "../lib/Chai.chpl"
  Tensor_2__real64_chpl value_chpl;
#line 588 "../lib/Chai.chpl"
  Tensor_2__real64_chpl ret_chpl3;
#line 588 "../lib/Chai.chpl"
  Tensor_2__real64_chpl x_chpl;
#line 588 "../lib/Chai.chpl"
  SumReduceScanOp_Tensor_1__real64_chpl RP_biasesGrad_chpl2 = NULL;
#line 588 "../lib/Chai.chpl"
  Tensor_1__real64_chpl AS_biasesGrad_chpl;
#line 588 "../lib/Chai.chpl"
  SumReduceScanOp_Tensor_1__real64_chpl new_temp_chpl2 = NULL;
#line 588 "../lib/Chai.chpl"
  chpl_LocalSpinlock default_arg_l_chpl2;
#line 588 "../lib/Chai.chpl"
  chpl_LocalSpinlock ret_chpl4;
#line 588 "../lib/Chai.chpl"
  chpl_LocalSpinlock l_chpl2;
#line 588 "../lib/Chai.chpl"
  Tensor_1__real64_chpl default_arg_value_chpl2;
#line 588 "../lib/Chai.chpl"
  Tensor_1__real64_chpl ret_chpl5;
#line 588 "../lib/Chai.chpl"
  Tensor_1__real64_chpl value_chpl2;
#line 588 "../lib/Chai.chpl"
  Tensor_1__real64_chpl ret_chpl6;
#line 588 "../lib/Chai.chpl"
  Tensor_1__real64_chpl x_chpl2;
#line 588 "../lib/Chai.chpl"
  _tuple_1_star_range_int64_t_both_one_chpl followMe_chpl;
#line 588 "../lib/Chai.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl7 = NULL;
#line 588 "../lib/Chai.chpl"
  int64_t call_tmp_chpl8;
#line 588 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl3;
#line 588 "../lib/Chai.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl9 = NULL;
#line 588 "../lib/Chai.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl10 = NULL;
#line 588 "../lib/Chai.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl11 = NULL;
#line 588 "../lib/Chai.chpl"
  _tuple_2_star_int64_t_chpl call_tmp_chpl12;
#line 588 "../lib/Chai.chpl"
  _tuple_2_star_int64_t_chpl ret_tmp_chpl;
#line 588 "../lib/Chai.chpl"
  int64_t tmp_x0_chpl;
#line 588 "../lib/Chai.chpl"
  int64_t tmp_x1_chpl;
#line 588 "../lib/Chai.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl13 = NULL;
#line 588 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl14;
#line 588 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl2;
#line 588 "../lib/Chai.chpl"
  _tuple_1_star_range_int64_t_both_one_chpl _yield_expr_tmp__chpl;
#line 588 "../lib/Chai.chpl"
  _tuple_1_star_range_int64_t_both_one_chpl chpl_followThis;
#line 588 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_1_int64_t_one_Tensor_1__real64_int64_t_chpl _ir_F0_this_chpl;
#line 588 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl _ic__F5_block_x0_chpl;
#line 588 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl _ic__F7_coerce_tmp_x0_chpl;
#line 588 "../lib/Chai.chpl"
  int64_t _ic__F8_i_chpl;
#line 588 "../lib/Chai.chpl"
  int64_t _ic__F9_end_chpl;
#line 588 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl _ir_F0_this_chpl2;
#line 588 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl _ic__F5_block_x0_chpl2;
#line 588 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl _ic__F7_coerce_tmp_x0_chpl2;
#line 588 "../lib/Chai.chpl"
  int64_t _ic__F8_i_chpl2;
#line 588 "../lib/Chai.chpl"
  int64_t _ic__F9_end_chpl2;
#line 588 "../lib/Chai.chpl"
  _domain_DefaultRectangularDom_1_int64_t_one_chpl _ir_F0_this_chpl3;
#line 588 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl _ic__F4_block_x0_chpl;
#line 588 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl _ic__F6_coerce_tmp_x0_chpl;
#line 588 "../lib/Chai.chpl"
  int64_t _ic__F7_i_chpl;
#line 588 "../lib/Chai.chpl"
  int64_t _ic__F8_end_chpl;
#line 588 "../lib/Chai.chpl"
  DefaultRectangularArr_1_int64_t_one_Tensor_1__real64_int64_t_chpl this_chpl32 = NULL;
#line 588 "../lib/Chai.chpl"
  DefaultRectangularDom_1_int64_t_one_chpl coerce_tmp_chpl4 = NULL;
#line 588 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl5;
#line 588 "../lib/Chai.chpl"
  DefaultRectangularDom_1_int64_t_one_chpl this_chpl33 = NULL;
#line 588 "../lib/Chai.chpl"
  uint64_t call_tmp_chpl15;
#line 588 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl6;
#line 588 "../lib/Chai.chpl"
  DefaultRectangularDom_1_int64_t_one_chpl this_chpl34 = NULL;
#line 588 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl7;
#line 588 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl8;
#line 588 "../lib/Chai.chpl"
  DefaultRectangularDom_1_int64_t_one_chpl this_chpl35 = NULL;
#line 588 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl9;
#line 588 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl10;
#line 588 "../lib/Chai.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl16 = NULL;
#line 588 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl this_chpl36;
#line 588 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl this_chpl37;
#line 588 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_1_int64_t_one_Tensor_1__real64_int64_t_chpl tmp_chpl;
#line 588 "../lib/Chai.chpl"
  DefaultRectangularArr_1_int64_t_one_Tensor_1__real64_int64_t_chpl ret_chpl7 = NULL;
#line 588 "../lib/Chai.chpl"
  DefaultRectangularArr_1_int64_t_one_Tensor_1__real64_int64_t_chpl _ic__F0_this_chpl = NULL;
#line 588 "../lib/Chai.chpl"
  _tuple_1_star_int64_t_chpl ret_tmp_chpl3;
#line 588 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl tup_x0_chpl;
#line 588 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl elt_x0_chpl;
#line 588 "../lib/Chai.chpl"
  int64_t coerce_tmp_chpl11;
#line 588 "../lib/Chai.chpl"
  _ref__tuple_1_star_range_int64_t_both_one_chpl call_tmp_chpl17 = NULL;
#line 588 "../lib/Chai.chpl"
  chpl_bool tmp_chpl2;
#line 588 "../lib/Chai.chpl"
  _ref__tuple_1_star_range_int64_t_both_one_chpl call_tmp_chpl18 = NULL;
#line 588 "../lib/Chai.chpl"
  int64_t call_tmp_chpl19;
#line 588 "../lib/Chai.chpl"
  _ref__tuple_1_star_range_int64_t_both_one_chpl call_tmp_chpl20 = NULL;
#line 588 "../lib/Chai.chpl"
  int64_t call_tmp_chpl21;
#line 588 "../lib/Chai.chpl"
  _tuple_1_star_range_int64_t_both_one_chpl retcopy_chpl;
#line 588 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl _ic__F0_this_chpl2;
#line 588 "../lib/Chai.chpl"
  int64_t coerce_tmp_chpl12;
#line 588 "../lib/Chai.chpl"
  DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl this_chpl38 = NULL;
#line 588 "../lib/Chai.chpl"
  DefaultRectangularDom_1_int64_t_one_chpl coerce_tmp_chpl13 = NULL;
#line 588 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl14;
#line 588 "../lib/Chai.chpl"
  DefaultRectangularDom_1_int64_t_one_chpl this_chpl39 = NULL;
#line 588 "../lib/Chai.chpl"
  uint64_t call_tmp_chpl22;
#line 588 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl15;
#line 588 "../lib/Chai.chpl"
  DefaultRectangularDom_1_int64_t_one_chpl this_chpl40 = NULL;
#line 588 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl16;
#line 588 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl17;
#line 588 "../lib/Chai.chpl"
  DefaultRectangularDom_1_int64_t_one_chpl this_chpl41 = NULL;
#line 588 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl18;
#line 588 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl19;
#line 588 "../lib/Chai.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl23 = NULL;
#line 588 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl this_chpl42;
#line 588 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl this_chpl43;
#line 588 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl tmp_chpl3;
#line 588 "../lib/Chai.chpl"
  DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl ret_chpl8 = NULL;
#line 588 "../lib/Chai.chpl"
  DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl _ic__F0_this_chpl3 = NULL;
#line 588 "../lib/Chai.chpl"
  _tuple_1_star_int64_t_chpl ret_tmp_chpl4;
#line 588 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl tup_x0_chpl2;
#line 588 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl elt_x0_chpl2;
#line 588 "../lib/Chai.chpl"
  int64_t coerce_tmp_chpl20;
#line 588 "../lib/Chai.chpl"
  _ref__tuple_1_star_range_int64_t_both_one_chpl call_tmp_chpl24 = NULL;
#line 588 "../lib/Chai.chpl"
  chpl_bool tmp_chpl4;
#line 588 "../lib/Chai.chpl"
  _ref__tuple_1_star_range_int64_t_both_one_chpl call_tmp_chpl25 = NULL;
#line 588 "../lib/Chai.chpl"
  int64_t call_tmp_chpl26;
#line 588 "../lib/Chai.chpl"
  _ref__tuple_1_star_range_int64_t_both_one_chpl call_tmp_chpl27 = NULL;
#line 588 "../lib/Chai.chpl"
  int64_t call_tmp_chpl28;
#line 588 "../lib/Chai.chpl"
  _tuple_1_star_range_int64_t_both_one_chpl retcopy_chpl2;
#line 588 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl _ic__F0_this_chpl4;
#line 588 "../lib/Chai.chpl"
  int64_t coerce_tmp_chpl21;
#line 588 "../lib/Chai.chpl"
  chpl_bool hasMore_chpl;
#line 588 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl22;
#line 588 "../lib/Chai.chpl"
  DefaultRectangularDom_1_int64_t_one_chpl this_chpl44 = NULL;
#line 588 "../lib/Chai.chpl"
  uint64_t call_tmp_chpl29;
#line 588 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl23;
#line 588 "../lib/Chai.chpl"
  DefaultRectangularDom_1_int64_t_one_chpl this_chpl45 = NULL;
#line 588 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl24;
#line 588 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl25;
#line 588 "../lib/Chai.chpl"
  DefaultRectangularDom_1_int64_t_one_chpl this_chpl46 = NULL;
#line 588 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl26;
#line 588 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl27;
#line 588 "../lib/Chai.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl30 = NULL;
#line 588 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl this_chpl47;
#line 588 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl this_chpl48;
#line 588 "../lib/Chai.chpl"
  _domain_DefaultRectangularDom_1_int64_t_one_chpl tmp_chpl5;
#line 588 "../lib/Chai.chpl"
  DefaultRectangularDom_1_int64_t_one_chpl ret_chpl9 = NULL;
#line 588 "../lib/Chai.chpl"
  _tuple_1_star_int64_t_chpl ret_tmp_chpl5;
#line 588 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl tup_x0_chpl3;
#line 588 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl elt_x0_chpl3;
#line 588 "../lib/Chai.chpl"
  int64_t coerce_tmp_chpl28;
#line 588 "../lib/Chai.chpl"
  _ref__tuple_1_star_range_int64_t_both_one_chpl call_tmp_chpl31 = NULL;
#line 588 "../lib/Chai.chpl"
  chpl_bool tmp_chpl6;
#line 588 "../lib/Chai.chpl"
  _ref__tuple_1_star_range_int64_t_both_one_chpl call_tmp_chpl32 = NULL;
#line 588 "../lib/Chai.chpl"
  int64_t call_tmp_chpl33;
#line 588 "../lib/Chai.chpl"
  _ref__tuple_1_star_range_int64_t_both_one_chpl call_tmp_chpl34 = NULL;
#line 588 "../lib/Chai.chpl"
  int64_t call_tmp_chpl35;
#line 588 "../lib/Chai.chpl"
  _tuple_1_star_range_int64_t_both_one_chpl retcopy_chpl3;
#line 588 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl _ic__F0_this_chpl5;
#line 588 "../lib/Chai.chpl"
  chpl_bool hasMore_chpl2;
#line 588 "../lib/Chai.chpl"
  int64_t tmp_chpl7;
#line 588 "../lib/Chai.chpl"
  DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl this_chpl49 = NULL;
#line 588 "../lib/Chai.chpl"
  _ddata_Tensor_3__real64_chpl coerce_tmp_chpl29 = NULL;
#line 588 "../lib/Chai.chpl"
  _ref_Tensor_3__real64_chpl call_tmp_chpl36 = NULL;
#line 588 "../lib/Chai.chpl"
  int64_t tmp_chpl8;
#line 588 "../lib/Chai.chpl"
  DefaultRectangularArr_1_int64_t_one_Tensor_1__real64_int64_t_chpl this_chpl50 = NULL;
#line 588 "../lib/Chai.chpl"
  _ddata_Tensor_1__real64_chpl coerce_tmp_chpl30 = NULL;
#line 588 "../lib/Chai.chpl"
  _ref_Tensor_1__real64_chpl call_tmp_chpl37 = NULL;
#line 588 "../lib/Chai.chpl"
  int64_t idx_chpl;
#line 590 "../lib/Chai.chpl"
  Tensor_1__real64_chpl call_tmp_chpl38;
#line 590 "../lib/Chai.chpl"
  Tensor_1__real64_chpl ret_tmp_chpl6;
#line 590 "../lib/Chai.chpl"
  Tensor_1__real64_chpl flattened_chpl;
#line 591 "../lib/Chai.chpl"
  _ref_Tensor_2__real64_chpl call_tmp_chpl39 = NULL;
#line 591 "../lib/Chai.chpl"
  Tensor_1__real64_chpl call_tmp_chpl40;
#line 591 "../lib/Chai.chpl"
  Tensor_1__real64_chpl ret_tmp_chpl7;
#line 591 "../lib/Chai.chpl"
  _ref_Tensor_1__real64_chpl call_tmp_chpl41 = NULL;
#line 591 "../lib/Chai.chpl"
  Tensor_1__real64_chpl call_tmp_chpl42;
#line 591 "../lib/Chai.chpl"
  Tensor_1__real64_chpl ret_tmp_chpl8;
#line 591 "../lib/Chai.chpl"
  Tensor_1__real64_chpl Z_chpl;
#line 591 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_1_int64_t_one__real64_int64_t_chpl _field_destructor_tmp__chpl;
#line 591 "../lib/Chai.chpl"
  _domain_DefaultRectangularDom_1_int64_t_one_chpl _field_destructor_tmp__chpl2;
#line 592 "../lib/Chai.chpl"
  _tuple_3_Tensor_1__real64__real64_Tensor_1__real64_chpl call_tmp_chpl43;
#line 592 "../lib/Chai.chpl"
  _tuple_3_Tensor_1__real64__real64_Tensor_1__real64_chpl ret_tmp_chpl9;
#line 592 "../lib/Chai.chpl"
  Tensor_1__real64_chpl tmp_x0_chpl2;
#line 592 "../lib/Chai.chpl"
  _real64 tmp_x1_chpl2;
#line 592 "../lib/Chai.chpl"
  Tensor_1__real64_chpl tmp_x2_chpl;
#line 592 "../lib/Chai.chpl"
  _ref_Tensor_1__real64_chpl call_tmp_chpl44 = NULL;
#line 592 "../lib/Chai.chpl"
  Tensor_1__real64_chpl exp_chpl;
#line 592 "../lib/Chai.chpl"
  _real64 expSum_chpl;
#line 592 "../lib/Chai.chpl"
  _ref_Tensor_1__real64_chpl call_tmp_chpl45 = NULL;
#line 592 "../lib/Chai.chpl"
  Tensor_1__real64_chpl softmax_chpl2;
#line 593 "../lib/Chai.chpl"
  Tensor_1__real64_chpl dL_dOut_chpl;
#line 594 "../lib/Chai.chpl"
  int64_t nonZeroIdx_chpl;
#line 595 "../lib/Chai.chpl"
  _ref__array_DefaultRectangularArr_1_int64_t_one__real64_int64_t_chpl call_tmp_chpl46 = NULL;
#line 595 "../lib/Chai.chpl"
  _domain_DefaultRectangularDom_1_int64_t_one_chpl call_tmp_chpl47;
#line 595 "../lib/Chai.chpl"
  _domain_DefaultRectangularDom_1_int64_t_one_chpl ret_tmp_chpl10;
#line 595 "../lib/Chai.chpl"
  _domain_DefaultRectangularDom_1_int64_t_one_chpl this_chpl51;
#line 595 "../lib/Chai.chpl"
  DefaultRectangularDom_1_int64_t_one_chpl ret_chpl10 = NULL;
#line 595 "../lib/Chai.chpl"
  _tuple_1_star_int64_t_chpl ret_tmp_chpl11;
#line 595 "../lib/Chai.chpl"
  DefaultRectangularDom_1_int64_t_one_chpl this_chpl52 = NULL;
#line 595 "../lib/Chai.chpl"
  _ref__tuple_1_star_range_int64_t_both_one_chpl call_tmp_chpl48 = NULL;
#line 595 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl31;
#line 595 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl _ic__F0_this_chpl6;
#line 595 "../lib/Chai.chpl"
  int64_t i_chpl;
#line 595 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl this_chpl53;
#line 595 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl this_chpl54;
#line 595 "../lib/Chai.chpl"
  int64_t end_chpl;
#line 596 "../lib/Chai.chpl"
  _ref__array_DefaultRectangularArr_1_int64_t_one__real64_int64_t_chpl call_tmp_chpl49 = NULL;
#line 596 "../lib/Chai.chpl"
  _tuple_1_star_int64_t_chpl coerce_tmp_chpl32;
#line 596 "../lib/Chai.chpl"
  _ref__tuple_1_star_int64_t_chpl i_i_chpl = NULL;
#line 596 "../lib/Chai.chpl"
  DefaultRectangularArr_1_int64_t_one__real64_int64_t_chpl ret_chpl11 = NULL;
#line 596 "../lib/Chai.chpl"
  DefaultRectangularArr_1_int64_t_one__real64_int64_t_chpl coerce_tmp_chpl33 = NULL;
#line 596 "../lib/Chai.chpl"
  _tuple_1_star_int64_t_chpl _formal_tmp_in_ind_chpl;
#line 596 "../lib/Chai.chpl"
  int64_t _formal_tmp_ind_x0_chpl;
#line 596 "../lib/Chai.chpl"
  _real64 ret_chpl12;
#line 596 "../lib/Chai.chpl"
  _ddata__real64_chpl coerce_tmp_chpl34 = NULL;
#line 596 "../lib/Chai.chpl"
  _ref__real64_chpl call_tmp_chpl50 = NULL;
#line 599 "../lib/Chai.chpl"
  _ref__array_DefaultRectangularArr_1_int64_t_one__real64_int64_t_chpl call_tmp_chpl51 = NULL;
#line 599 "../lib/Chai.chpl"
  _tuple_1_star_int64_t_chpl coerce_tmp_chpl35;
#line 599 "../lib/Chai.chpl"
  _ref__tuple_1_star_int64_t_chpl i_i_chpl2 = NULL;
#line 599 "../lib/Chai.chpl"
  DefaultRectangularArr_1_int64_t_one__real64_int64_t_chpl ret_chpl13 = NULL;
#line 599 "../lib/Chai.chpl"
  DefaultRectangularArr_1_int64_t_one__real64_int64_t_chpl coerce_tmp_chpl36 = NULL;
#line 599 "../lib/Chai.chpl"
  _tuple_1_star_int64_t_chpl _formal_tmp_in_ind_chpl2;
#line 599 "../lib/Chai.chpl"
  int64_t _formal_tmp_ind_x0_chpl2;
#line 599 "../lib/Chai.chpl"
  _ddata__real64_chpl coerce_tmp_chpl37 = NULL;
#line 599 "../lib/Chai.chpl"
  _ref__real64_chpl call_tmp_chpl52 = NULL;
#line 599 "../lib/Chai.chpl"
  _real64 call_tmp_chpl53;
#line 599 "../lib/Chai.chpl"
  Tensor_1__real64_chpl call_tmp_chpl54;
#line 599 "../lib/Chai.chpl"
  Tensor_1__real64_chpl ret_tmp_chpl12;
#line 599 "../lib/Chai.chpl"
  Tensor_1__real64_chpl call_tmp_chpl55;
#line 599 "../lib/Chai.chpl"
  Tensor_1__real64_chpl ret_tmp_chpl13;
#line 599 "../lib/Chai.chpl"
  Tensor_1__real64_chpl dOut_dZ_chpl;
#line 599 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_1_int64_t_one__real64_int64_t_chpl _field_destructor_tmp__chpl3;
#line 599 "../lib/Chai.chpl"
  _domain_DefaultRectangularDom_1_int64_t_one_chpl _field_destructor_tmp__chpl4;
#line 600 "../lib/Chai.chpl"
  _ref__array_DefaultRectangularArr_1_int64_t_one__real64_int64_t_chpl call_tmp_chpl56 = NULL;
#line 600 "../lib/Chai.chpl"
  _tuple_1_star_int64_t_chpl coerce_tmp_chpl38;
#line 600 "../lib/Chai.chpl"
  _ref__tuple_1_star_int64_t_chpl i_i_chpl3 = NULL;
#line 600 "../lib/Chai.chpl"
  DefaultRectangularArr_1_int64_t_one__real64_int64_t_chpl ret_chpl14 = NULL;
#line 600 "../lib/Chai.chpl"
  DefaultRectangularArr_1_int64_t_one__real64_int64_t_chpl coerce_tmp_chpl39 = NULL;
#line 600 "../lib/Chai.chpl"
  _tuple_1_star_int64_t_chpl _formal_tmp_in_ind_chpl3;
#line 600 "../lib/Chai.chpl"
  int64_t _formal_tmp_ind_x0_chpl3;
#line 600 "../lib/Chai.chpl"
  _ddata__real64_chpl coerce_tmp_chpl40 = NULL;
#line 600 "../lib/Chai.chpl"
  _ref__real64_chpl call_tmp_chpl57 = NULL;
#line 600 "../lib/Chai.chpl"
  _ref__array_DefaultRectangularArr_1_int64_t_one__real64_int64_t_chpl call_tmp_chpl58 = NULL;
#line 600 "../lib/Chai.chpl"
  _tuple_1_star_int64_t_chpl coerce_tmp_chpl41;
#line 600 "../lib/Chai.chpl"
  _ref__tuple_1_star_int64_t_chpl i_i_chpl4 = NULL;
#line 600 "../lib/Chai.chpl"
  DefaultRectangularArr_1_int64_t_one__real64_int64_t_chpl ret_chpl15 = NULL;
#line 600 "../lib/Chai.chpl"
  DefaultRectangularArr_1_int64_t_one__real64_int64_t_chpl coerce_tmp_chpl42 = NULL;
#line 600 "../lib/Chai.chpl"
  _tuple_1_star_int64_t_chpl _formal_tmp_in_ind_chpl4;
#line 600 "../lib/Chai.chpl"
  int64_t _formal_tmp_ind_x0_chpl4;
#line 600 "../lib/Chai.chpl"
  _real64 ret_chpl16;
#line 600 "../lib/Chai.chpl"
  _ddata__real64_chpl coerce_tmp_chpl43 = NULL;
#line 600 "../lib/Chai.chpl"
  _ref__real64_chpl call_tmp_chpl59 = NULL;
#line 600 "../lib/Chai.chpl"
  _ref__array_DefaultRectangularArr_1_int64_t_one__real64_int64_t_chpl call_tmp_chpl60 = NULL;
#line 600 "../lib/Chai.chpl"
  _tuple_1_star_int64_t_chpl coerce_tmp_chpl44;
#line 600 "../lib/Chai.chpl"
  _ref__tuple_1_star_int64_t_chpl i_i_chpl5 = NULL;
#line 600 "../lib/Chai.chpl"
  DefaultRectangularArr_1_int64_t_one__real64_int64_t_chpl ret_chpl17 = NULL;
#line 600 "../lib/Chai.chpl"
  DefaultRectangularArr_1_int64_t_one__real64_int64_t_chpl coerce_tmp_chpl45 = NULL;
#line 600 "../lib/Chai.chpl"
  _tuple_1_star_int64_t_chpl _formal_tmp_in_ind_chpl5;
#line 600 "../lib/Chai.chpl"
  int64_t _formal_tmp_ind_x0_chpl5;
#line 600 "../lib/Chai.chpl"
  _ddata__real64_chpl coerce_tmp_chpl46 = NULL;
#line 600 "../lib/Chai.chpl"
  _ref__real64_chpl call_tmp_chpl61 = NULL;
#line 600 "../lib/Chai.chpl"
  _real64 call_tmp_chpl62;
#line 601 "../lib/Chai.chpl"
  Tensor_1__real64_chpl dZ_dW_chpl;
#line 603 "../lib/Chai.chpl"
  _ref_Tensor_2__real64_chpl call_tmp_chpl63 = NULL;
#line 603 "../lib/Chai.chpl"
  Tensor_2__real64_chpl dZ_dIn_chpl;
#line 604 "../lib/Chai.chpl"
  _ref__array_DefaultRectangularArr_1_int64_t_one__real64_int64_t_chpl call_tmp_chpl64 = NULL;
#line 604 "../lib/Chai.chpl"
  _tuple_1_star_int64_t_chpl coerce_tmp_chpl47;
#line 604 "../lib/Chai.chpl"
  _ref__tuple_1_star_int64_t_chpl i_i_chpl6 = NULL;
#line 604 "../lib/Chai.chpl"
  DefaultRectangularArr_1_int64_t_one__real64_int64_t_chpl ret_chpl18 = NULL;
#line 604 "../lib/Chai.chpl"
  DefaultRectangularArr_1_int64_t_one__real64_int64_t_chpl coerce_tmp_chpl48 = NULL;
#line 604 "../lib/Chai.chpl"
  _tuple_1_star_int64_t_chpl _formal_tmp_in_ind_chpl6;
#line 604 "../lib/Chai.chpl"
  int64_t _formal_tmp_ind_x0_chpl6;
#line 604 "../lib/Chai.chpl"
  _ddata__real64_chpl coerce_tmp_chpl49 = NULL;
#line 604 "../lib/Chai.chpl"
  _ref__real64_chpl call_tmp_chpl65 = NULL;
#line 604 "../lib/Chai.chpl"
  Tensor_1__real64_chpl call_tmp_chpl66;
#line 604 "../lib/Chai.chpl"
  Tensor_1__real64_chpl ret_tmp_chpl14;
#line 604 "../lib/Chai.chpl"
  Tensor_1__real64_chpl dL_dZ_chpl;
#line 605 "../lib/Chai.chpl"
  Tensor_2__real64_chpl call_tmp_chpl67;
#line 605 "../lib/Chai.chpl"
  Tensor_2__real64_chpl ret_tmp_chpl15;
#line 605 "../lib/Chai.chpl"
  Tensor_2__real64_chpl call_tmp_chpl68;
#line 605 "../lib/Chai.chpl"
  Tensor_2__real64_chpl ret_tmp_chpl16;
#line 605 "../lib/Chai.chpl"
  Tensor_2__real64_chpl dL_dW_chpl;
#line 605 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_2_int64_t_one__real64_int64_t_chpl _field_destructor_tmp__chpl5;
#line 605 "../lib/Chai.chpl"
  _domain_DefaultRectangularDom_2_int64_t_one_chpl _field_destructor_tmp__chpl6;
#line 606 "../lib/Chai.chpl"
  Tensor_1__real64_chpl call_tmp_chpl69;
#line 606 "../lib/Chai.chpl"
  Tensor_1__real64_chpl ret_tmp_chpl17;
#line 606 "../lib/Chai.chpl"
  Tensor_1__real64_chpl dL_dB_chpl;
#line 607 "../lib/Chai.chpl"
  Tensor_2__real64_chpl call_tmp_chpl70;
#line 607 "../lib/Chai.chpl"
  Tensor_2__real64_chpl ret_tmp_chpl18;
#line 607 "../lib/Chai.chpl"
  Tensor_1__real64_chpl call_tmp_chpl71;
#line 607 "../lib/Chai.chpl"
  Tensor_1__real64_chpl ret_tmp_chpl19;
#line 607 "../lib/Chai.chpl"
  Tensor_1__real64_chpl dL_dIn_chpl;
#line 607 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_2_int64_t_one__real64_int64_t_chpl _field_destructor_tmp__chpl7;
#line 607 "../lib/Chai.chpl"
  _domain_DefaultRectangularDom_2_int64_t_one_chpl _field_destructor_tmp__chpl8;
#line 610 "../lib/Chai.chpl"
  _tuple_1_star_int64_t_chpl coerce_tmp_chpl50;
#line 610 "../lib/Chai.chpl"
  _ref__tuple_1_star_int64_t_chpl i_i_chpl7 = NULL;
#line 610 "../lib/Chai.chpl"
  DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl ret_chpl19 = NULL;
#line 610 "../lib/Chai.chpl"
  DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl coerce_tmp_chpl51 = NULL;
#line 610 "../lib/Chai.chpl"
  _tuple_1_star_int64_t_chpl _formal_tmp_in_ind_chpl7;
#line 610 "../lib/Chai.chpl"
  int64_t _formal_tmp_ind_x0_chpl7;
#line 610 "../lib/Chai.chpl"
  _ddata_Tensor_3__real64_chpl coerce_tmp_chpl52 = NULL;
#line 610 "../lib/Chai.chpl"
  _ref_Tensor_3__real64_chpl call_tmp_chpl72 = NULL;
#line 610 "../lib/Chai.chpl"
  _domain_DefaultRectangularDom_3_int64_t_one_chpl call_tmp_chpl73;
#line 610 "../lib/Chai.chpl"
  _domain_DefaultRectangularDom_3_int64_t_one_chpl ret_tmp_chpl20;
#line 610 "../lib/Chai.chpl"
  Tensor_3__real64_chpl call_tmp_chpl74;
#line 610 "../lib/Chai.chpl"
  Tensor_3__real64_chpl ret_tmp_chpl21;
#line 610 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_3_int64_t_one__real64_int64_t_chpl _field_destructor_tmp__chpl9;
#line 610 "../lib/Chai.chpl"
  _domain_DefaultRectangularDom_3_int64_t_one_chpl _field_destructor_tmp__chpl10;
#line 610 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_1_int64_t_one__real64_int64_t_chpl _field_destructor_tmp__chpl11;
#line 610 "../lib/Chai.chpl"
  _domain_DefaultRectangularDom_1_int64_t_one_chpl _field_destructor_tmp__chpl12;
#line 610 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_1_int64_t_one__real64_int64_t_chpl _field_destructor_tmp__chpl13;
#line 610 "../lib/Chai.chpl"
  _domain_DefaultRectangularDom_1_int64_t_one_chpl _field_destructor_tmp__chpl14;
#line 610 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_2_int64_t_one__real64_int64_t_chpl _field_destructor_tmp__chpl15;
#line 610 "../lib/Chai.chpl"
  _domain_DefaultRectangularDom_2_int64_t_one_chpl _field_destructor_tmp__chpl16;
#line 610 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_1_int64_t_one__real64_int64_t_chpl _field_destructor_tmp__chpl17;
#line 610 "../lib/Chai.chpl"
  _domain_DefaultRectangularDom_1_int64_t_one_chpl _field_destructor_tmp__chpl18;
#line 610 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_2_int64_t_one__real64_int64_t_chpl _field_destructor_tmp__chpl19;
#line 610 "../lib/Chai.chpl"
  _domain_DefaultRectangularDom_2_int64_t_one_chpl _field_destructor_tmp__chpl20;
#line 610 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_1_int64_t_one__real64_int64_t_chpl _field_destructor_tmp__chpl21;
#line 610 "../lib/Chai.chpl"
  _domain_DefaultRectangularDom_1_int64_t_one_chpl _field_destructor_tmp__chpl22;
#line 610 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_1_int64_t_one__real64_int64_t_chpl _field_destructor_tmp__chpl23;
#line 610 "../lib/Chai.chpl"
  _domain_DefaultRectangularDom_1_int64_t_one_chpl _field_destructor_tmp__chpl24;
#line 610 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_1_int64_t_one__real64_int64_t_chpl _field_destructor_tmp__chpl25;
#line 610 "../lib/Chai.chpl"
  _domain_DefaultRectangularDom_1_int64_t_one_chpl _field_destructor_tmp__chpl26;
#line 610 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_1_int64_t_one__real64_int64_t_chpl _field_destructor_tmp__chpl27;
#line 610 "../lib/Chai.chpl"
  _domain_DefaultRectangularDom_1_int64_t_one_chpl _field_destructor_tmp__chpl28;
#line 610 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_1_int64_t_one__real64_int64_t_chpl _field_destructor_tmp__chpl29;
#line 610 "../lib/Chai.chpl"
  _domain_DefaultRectangularDom_1_int64_t_one_chpl _field_destructor_tmp__chpl30;
#line 610 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_1_int64_t_one__real64_int64_t_chpl _field_destructor_tmp__chpl31;
#line 610 "../lib/Chai.chpl"
  _domain_DefaultRectangularDom_1_int64_t_one_chpl _field_destructor_tmp__chpl32;
#line 610 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_1_int64_t_one__real64_int64_t_chpl _field_destructor_tmp__chpl33;
#line 610 "../lib/Chai.chpl"
  _domain_DefaultRectangularDom_1_int64_t_one_chpl _field_destructor_tmp__chpl34;
#line 610 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_1_int64_t_one__real64_int64_t_chpl _field_destructor_tmp__chpl35;
#line 610 "../lib/Chai.chpl"
  _domain_DefaultRectangularDom_1_int64_t_one_chpl _field_destructor_tmp__chpl36;
#line 588 "../lib/Chai.chpl"
  int64_t tmp_chpl9;
#line 588 "../lib/Chai.chpl"
  int64_t tmp_chpl10;
#line 588 "../lib/Chai.chpl"
  int64_t tmp_chpl11;
#line 588 "../lib/Chai.chpl"
  int64_t tmp_chpl12;
#line 588 "../lib/Chai.chpl"
  int64_t tmp_chpl13;
#line 588 "../lib/Chai.chpl"
  int64_t tmp_chpl14;
#line 588 "../lib/Chai.chpl"
  SumReduceScanOp_Tensor_1__real64_chpl coerce_tmp_chpl53 = NULL;
#line 588 "../lib/Chai.chpl"
  _ref_Tensor_1__real64_chpl i_x_chpl = NULL;
#line 588 "../lib/Chai.chpl"
  _ref_Tensor_1__real64_chpl call_tmp_chpl75 = NULL;
#line 588 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_1_int64_t_one__real64_int64_t_chpl _field_destructor_tmp__chpl37;
#line 588 "../lib/Chai.chpl"
  _domain_DefaultRectangularDom_1_int64_t_one_chpl _field_destructor_tmp__chpl38;
#line 588 "../lib/Chai.chpl"
  SumReduceScanOp_Tensor_2__real64_chpl coerce_tmp_chpl54 = NULL;
#line 588 "../lib/Chai.chpl"
  _ref_Tensor_2__real64_chpl i_x_chpl2 = NULL;
#line 588 "../lib/Chai.chpl"
  _ref_Tensor_2__real64_chpl call_tmp_chpl76 = NULL;
#line 588 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_2_int64_t_one__real64_int64_t_chpl _field_destructor_tmp__chpl39;
#line 588 "../lib/Chai.chpl"
  _domain_DefaultRectangularDom_2_int64_t_one_chpl _field_destructor_tmp__chpl40;
#line 588 "../lib/Chai.chpl"
  init_chpl105(&l_chpl);
#line 588 "../lib/Chai.chpl"
  ret_chpl = l_chpl;
#line 588 "../lib/Chai.chpl"
  default_arg_l_chpl = ret_chpl;
#line 588 "../lib/Chai.chpl"
  init_chpl236(&value_chpl);
#line 588 "../lib/Chai.chpl"
  ret_chpl2 = value_chpl;
#line 588 "../lib/Chai.chpl"
  default_arg_value_chpl = ret_chpl2;
#line 588 "../lib/Chai.chpl"
  new_temp_chpl = _new_chpl22(&default_arg_l_chpl, &default_arg_value_chpl, INT64(588), INT32(65));
#line 588 "../lib/Chai.chpl"
  RP_weightsGrad_chpl2 = new_temp_chpl;
#line 588 "../lib/Chai.chpl"
  init_chpl236(&x_chpl);
#line 588 "../lib/Chai.chpl"
  ret_chpl3 = x_chpl;
#line 588 "../lib/Chai.chpl"
  AS_weightsGrad_chpl = ret_chpl3;
#line 588 "../lib/Chai.chpl"
  init_chpl105(&l_chpl2);
#line 588 "../lib/Chai.chpl"
  ret_chpl4 = l_chpl2;
#line 588 "../lib/Chai.chpl"
  default_arg_l_chpl2 = ret_chpl4;
#line 588 "../lib/Chai.chpl"
  init_chpl235(&value_chpl2);
#line 588 "../lib/Chai.chpl"
  ret_chpl5 = value_chpl2;
#line 588 "../lib/Chai.chpl"
  default_arg_value_chpl2 = ret_chpl5;
#line 588 "../lib/Chai.chpl"
  new_temp_chpl2 = _new_chpl24(&default_arg_l_chpl2, &default_arg_value_chpl2, INT64(588), INT32(65));
#line 588 "../lib/Chai.chpl"
  RP_biasesGrad_chpl2 = new_temp_chpl2;
#line 588 "../lib/Chai.chpl"
  init_chpl235(&x_chpl2);
#line 588 "../lib/Chai.chpl"
  ret_chpl6 = x_chpl2;
#line 588 "../lib/Chai.chpl"
  AS_biasesGrad_chpl = ret_chpl6;
#line 588 "../lib/Chai.chpl"
  *(followMe_chpl + INT64(0)) = *(*(locBlock_chpl) + INT64(0));
#line 588 "../lib/Chai.chpl"
  call_tmp_chpl7 = this_chpl20(locBlock_chpl, parDim_chpl, INT64(588), INT32(65));
#line 588 "../lib/Chai.chpl"
  coerce_tmp_chpl3 = *(call_tmp_chpl7);
#line 588 "../lib/Chai.chpl"
  call_tmp_chpl8 = sizeAs_chpl37(coerce_tmp_chpl3, INT64(588), INT32(65));
#line 588 "../lib/Chai.chpl"
  call_tmp_chpl9 = this_chpl20(locBlock_chpl, parDim_chpl, INT64(588), INT32(65));
#line 588 "../lib/Chai.chpl"
  call_tmp_chpl10 = this_chpl20(locBlock_chpl, parDim_chpl, INT64(588), INT32(65));
#line 588 "../lib/Chai.chpl"
  call_tmp_chpl11 = this_chpl20(locBlock_chpl, parDim_chpl, INT64(588), INT32(65));
#line 588 "../lib/Chai.chpl"
  _computeBlock_chpl(call_tmp_chpl8, numChunks_chpl, chunk_chpl, (call_tmp_chpl9)->_high, (call_tmp_chpl10)->_low, (call_tmp_chpl11)->_low, &ret_tmp_chpl, INT64(588), INT32(65));
#line 588 "../lib/Chai.chpl"
  *(call_tmp_chpl12 + INT64(0)) = *(ret_tmp_chpl + INT64(0));
#line 588 "../lib/Chai.chpl"
  *(call_tmp_chpl12 + INT64(1)) = *(ret_tmp_chpl + INT64(1));
#line 588 "../lib/Chai.chpl"
  tmp_x0_chpl = *(call_tmp_chpl12 + INT64(0));
#line 588 "../lib/Chai.chpl"
  tmp_x1_chpl = *(call_tmp_chpl12 + INT64(1));
#line 588 "../lib/Chai.chpl"
  call_tmp_chpl13 = this_chpl20(&followMe_chpl, parDim_chpl, INT64(588), INT32(65));
#line 588 "../lib/Chai.chpl"
  chpl_build_bounded_range6(tmp_x0_chpl, tmp_x1_chpl, &ret_tmp_chpl2);
#line 588 "../lib/Chai.chpl"
  call_tmp_chpl14 = ret_tmp_chpl2;
#line 588 "../lib/Chai.chpl"
  *(call_tmp_chpl13) = call_tmp_chpl14;
#line 588 "../lib/Chai.chpl"
  *(_yield_expr_tmp__chpl + INT64(0)) = *(followMe_chpl + INT64(0));
#line 588 "../lib/Chai.chpl"
  *(chpl_followThis + INT64(0)) = *(_yield_expr_tmp__chpl + INT64(0));
#line 588 "../lib/Chai.chpl"
  _ir_F0_this_chpl = *(deltas_chpl);
#line 588 "../lib/Chai.chpl"
  _ir_F0_this_chpl2 = *(inputs_chpl);
#line 588 "../lib/Chai.chpl"
  _ir_F0_this_chpl3 = *(call_tmp_chpl6);
#line 588 "../lib/Chai.chpl"
  tmp_chpl = _ir_F0_this_chpl;
#line 588 "../lib/Chai.chpl"
  ret_chpl7 = (&tmp_chpl)->_instance;
#line 588 "../lib/Chai.chpl"
  _ic__F0_this_chpl = ret_chpl7;
#line 588 "../lib/Chai.chpl"
  this_chpl32 = ret_chpl7;
#line 588 "../lib/Chai.chpl"
  chpl_check_nil(this_chpl32, INT64(588), INT32(65));
#line 588 "../lib/Chai.chpl"
  coerce_tmp_chpl4 = (this_chpl32)->dom;
#line 588 "../lib/Chai.chpl"
  createTuple_chpl(INT64(0), &ret_tmp_chpl3);
#line 588 "../lib/Chai.chpl"
  init_chpl113(&elt_x0_chpl);
#line 588 "../lib/Chai.chpl"
  tup_x0_chpl = elt_x0_chpl;
#line 588 "../lib/Chai.chpl"
  _ic__F5_block_x0_chpl = tup_x0_chpl;
#line 588 "../lib/Chai.chpl"
  coerce_tmp_chpl5 = *(chpl_followThis + INT64(0));
#line 588 "../lib/Chai.chpl"
  coerce_tmp_chpl11 = (&coerce_tmp_chpl5)->_high;
#line 588 "../lib/Chai.chpl"
  this_chpl33 = coerce_tmp_chpl4;
#line 588 "../lib/Chai.chpl"
  chpl_check_nil(this_chpl33, INT64(588), INT32(65));
#line 588 "../lib/Chai.chpl"
  call_tmp_chpl17 = &((this_chpl33)->ranges);
#line 588 "../lib/Chai.chpl"
  coerce_tmp_chpl6 = *(*(call_tmp_chpl17) + INT64(0));
#line 588 "../lib/Chai.chpl"
  call_tmp_chpl15 = sizeAs_chpl36(coerce_tmp_chpl6, INT64(588), INT32(65));
#line 588 "../lib/Chai.chpl"
  if (! (coerce_tmp_chpl11 < INT64(0))) /* ZLINE: 588 ../lib/Chai.chpl */
#line 588 "../lib/Chai.chpl"
  {
#line 588 "../lib/Chai.chpl"
    tmp_chpl2 = (((uint64_t)(coerce_tmp_chpl11)) >= call_tmp_chpl15);
#line 588 "../lib/Chai.chpl"
  } else /* ZLINE: 588 ../lib/Chai.chpl */
#line 588 "../lib/Chai.chpl"
  {
#line 588 "../lib/Chai.chpl"
    tmp_chpl2 = UINT8(false);
#line 588 "../lib/Chai.chpl"
  }
#line 588 "../lib/Chai.chpl"
  if (tmp_chpl2) /* ZLINE: 588 ../lib/Chai.chpl */
#line 588 "../lib/Chai.chpl"
  {
#line 588 "../lib/Chai.chpl"
    boundsCheckHalt_chpl(&_str_literal_4152_chpl, INT64(588), INT32(65));
#line 588 "../lib/Chai.chpl"
  }
#line 588 "../lib/Chai.chpl"
  this_chpl34 = coerce_tmp_chpl4;
#line 588 "../lib/Chai.chpl"
  chpl_check_nil(this_chpl34, INT64(588), INT32(65));
#line 588 "../lib/Chai.chpl"
  call_tmp_chpl18 = &((this_chpl34)->ranges);
#line 588 "../lib/Chai.chpl"
  coerce_tmp_chpl7 = *(*(call_tmp_chpl18) + INT64(0));
#line 588 "../lib/Chai.chpl"
  coerce_tmp_chpl8 = *(chpl_followThis + INT64(0));
#line 588 "../lib/Chai.chpl"
  call_tmp_chpl19 = ((&coerce_tmp_chpl7)->_low + ((int64_t)(((&coerce_tmp_chpl8)->_low * INT64(1)))));
#line 588 "../lib/Chai.chpl"
  this_chpl35 = coerce_tmp_chpl4;
#line 588 "../lib/Chai.chpl"
  chpl_check_nil(this_chpl35, INT64(588), INT32(65));
#line 588 "../lib/Chai.chpl"
  call_tmp_chpl20 = &((this_chpl35)->ranges);
#line 588 "../lib/Chai.chpl"
  coerce_tmp_chpl9 = *(*(call_tmp_chpl20) + INT64(0));
#line 588 "../lib/Chai.chpl"
  coerce_tmp_chpl10 = *(chpl_followThis + INT64(0));
#line 588 "../lib/Chai.chpl"
  call_tmp_chpl21 = ((&coerce_tmp_chpl9)->_low + ((int64_t)(((&coerce_tmp_chpl10)->_high * INT64(1)))));
#line 588 "../lib/Chai.chpl"
  call_tmp_chpl16 = &_ic__F5_block_x0_chpl;
#line 588 "../lib/Chai.chpl"
  compilerAssert_chpl();
#line 588 "../lib/Chai.chpl"
  (call_tmp_chpl16)->_low = call_tmp_chpl19;
#line 588 "../lib/Chai.chpl"
  (call_tmp_chpl16)->_high = call_tmp_chpl21;
#line 588 "../lib/Chai.chpl"
  *(retcopy_chpl + INT64(0)) = _ic__F5_block_x0_chpl;
#line 588 "../lib/Chai.chpl"
  _ic__F7_coerce_tmp_x0_chpl = *(retcopy_chpl + INT64(0));
#line 588 "../lib/Chai.chpl"
  _ic__F0_this_chpl2 = _ic__F7_coerce_tmp_x0_chpl;
#line 588 "../lib/Chai.chpl"
  checkIfIterWillOverflow_chpl(_ic__F7_coerce_tmp_x0_chpl, UINT8(true), INT64(588), INT32(65));
#line 588 "../lib/Chai.chpl"
  _ic__F8_i_chpl = INT64(0);
#line 588 "../lib/Chai.chpl"
  this_chpl36 = _ic__F0_this_chpl2;
#line 588 "../lib/Chai.chpl"
  coerce_tmp_chpl12 = (&this_chpl36)->_low;
#line 588 "../lib/Chai.chpl"
  this_chpl37 = _ic__F0_this_chpl2;
#line 588 "../lib/Chai.chpl"
  _ic__F9_end_chpl = (&this_chpl37)->_high;
#line 588 "../lib/Chai.chpl"
  tmp_chpl3 = _ir_F0_this_chpl2;
#line 588 "../lib/Chai.chpl"
  ret_chpl8 = (&tmp_chpl3)->_instance;
#line 588 "../lib/Chai.chpl"
  _ic__F0_this_chpl3 = ret_chpl8;
#line 588 "../lib/Chai.chpl"
  this_chpl38 = ret_chpl8;
#line 588 "../lib/Chai.chpl"
  chpl_check_nil(this_chpl38, INT64(588), INT32(65));
#line 588 "../lib/Chai.chpl"
  coerce_tmp_chpl13 = (this_chpl38)->dom;
#line 588 "../lib/Chai.chpl"
  createTuple_chpl(INT64(0), &ret_tmp_chpl4);
#line 588 "../lib/Chai.chpl"
  init_chpl113(&elt_x0_chpl2);
#line 588 "../lib/Chai.chpl"
  tup_x0_chpl2 = elt_x0_chpl2;
#line 588 "../lib/Chai.chpl"
  _ic__F5_block_x0_chpl2 = tup_x0_chpl2;
#line 588 "../lib/Chai.chpl"
  coerce_tmp_chpl14 = *(chpl_followThis + INT64(0));
#line 588 "../lib/Chai.chpl"
  coerce_tmp_chpl20 = (&coerce_tmp_chpl14)->_high;
#line 588 "../lib/Chai.chpl"
  this_chpl39 = coerce_tmp_chpl13;
#line 588 "../lib/Chai.chpl"
  chpl_check_nil(this_chpl39, INT64(588), INT32(65));
#line 588 "../lib/Chai.chpl"
  call_tmp_chpl24 = &((this_chpl39)->ranges);
#line 588 "../lib/Chai.chpl"
  coerce_tmp_chpl15 = *(*(call_tmp_chpl24) + INT64(0));
#line 588 "../lib/Chai.chpl"
  call_tmp_chpl22 = sizeAs_chpl36(coerce_tmp_chpl15, INT64(588), INT32(65));
#line 588 "../lib/Chai.chpl"
  if (! (coerce_tmp_chpl20 < INT64(0))) /* ZLINE: 588 ../lib/Chai.chpl */
#line 588 "../lib/Chai.chpl"
  {
#line 588 "../lib/Chai.chpl"
    tmp_chpl4 = (((uint64_t)(coerce_tmp_chpl20)) >= call_tmp_chpl22);
#line 588 "../lib/Chai.chpl"
  } else /* ZLINE: 588 ../lib/Chai.chpl */
#line 588 "../lib/Chai.chpl"
  {
#line 588 "../lib/Chai.chpl"
    tmp_chpl4 = UINT8(false);
#line 588 "../lib/Chai.chpl"
  }
#line 588 "../lib/Chai.chpl"
  if (tmp_chpl4) /* ZLINE: 588 ../lib/Chai.chpl */
#line 588 "../lib/Chai.chpl"
  {
#line 588 "../lib/Chai.chpl"
    boundsCheckHalt_chpl(&_str_literal_4152_chpl, INT64(588), INT32(65));
#line 588 "../lib/Chai.chpl"
  }
#line 588 "../lib/Chai.chpl"
  this_chpl40 = coerce_tmp_chpl13;
#line 588 "../lib/Chai.chpl"
  chpl_check_nil(this_chpl40, INT64(588), INT32(65));
#line 588 "../lib/Chai.chpl"
  call_tmp_chpl25 = &((this_chpl40)->ranges);
#line 588 "../lib/Chai.chpl"
  coerce_tmp_chpl16 = *(*(call_tmp_chpl25) + INT64(0));
#line 588 "../lib/Chai.chpl"
  coerce_tmp_chpl17 = *(chpl_followThis + INT64(0));
#line 588 "../lib/Chai.chpl"
  call_tmp_chpl26 = ((&coerce_tmp_chpl16)->_low + ((int64_t)(((&coerce_tmp_chpl17)->_low * INT64(1)))));
#line 588 "../lib/Chai.chpl"
  this_chpl41 = coerce_tmp_chpl13;
#line 588 "../lib/Chai.chpl"
  chpl_check_nil(this_chpl41, INT64(588), INT32(65));
#line 588 "../lib/Chai.chpl"
  call_tmp_chpl27 = &((this_chpl41)->ranges);
#line 588 "../lib/Chai.chpl"
  coerce_tmp_chpl18 = *(*(call_tmp_chpl27) + INT64(0));
#line 588 "../lib/Chai.chpl"
  coerce_tmp_chpl19 = *(chpl_followThis + INT64(0));
#line 588 "../lib/Chai.chpl"
  call_tmp_chpl28 = ((&coerce_tmp_chpl18)->_low + ((int64_t)(((&coerce_tmp_chpl19)->_high * INT64(1)))));
#line 588 "../lib/Chai.chpl"
  call_tmp_chpl23 = &_ic__F5_block_x0_chpl2;
#line 588 "../lib/Chai.chpl"
  compilerAssert_chpl();
#line 588 "../lib/Chai.chpl"
  (call_tmp_chpl23)->_low = call_tmp_chpl26;
#line 588 "../lib/Chai.chpl"
  (call_tmp_chpl23)->_high = call_tmp_chpl28;
#line 588 "../lib/Chai.chpl"
  *(retcopy_chpl2 + INT64(0)) = _ic__F5_block_x0_chpl2;
#line 588 "../lib/Chai.chpl"
  _ic__F7_coerce_tmp_x0_chpl2 = *(retcopy_chpl2 + INT64(0));
#line 588 "../lib/Chai.chpl"
  _ic__F0_this_chpl4 = _ic__F7_coerce_tmp_x0_chpl2;
#line 588 "../lib/Chai.chpl"
  checkIfIterWillOverflow_chpl(_ic__F7_coerce_tmp_x0_chpl2, UINT8(true), INT64(588), INT32(65));
#line 588 "../lib/Chai.chpl"
  _ic__F8_i_chpl2 = INT64(0);
#line 588 "../lib/Chai.chpl"
  this_chpl42 = _ic__F0_this_chpl4;
#line 588 "../lib/Chai.chpl"
  coerce_tmp_chpl21 = (&this_chpl42)->_low;
#line 588 "../lib/Chai.chpl"
  this_chpl43 = _ic__F0_this_chpl4;
#line 588 "../lib/Chai.chpl"
  _ic__F9_end_chpl2 = (&this_chpl43)->_high;
#line 588 "../lib/Chai.chpl"
  tmp_chpl5 = _ir_F0_this_chpl3;
#line 588 "../lib/Chai.chpl"
  ret_chpl9 = (&tmp_chpl5)->_instance;
#line 588 "../lib/Chai.chpl"
  createTuple_chpl(INT64(0), &ret_tmp_chpl5);
#line 588 "../lib/Chai.chpl"
  init_chpl113(&elt_x0_chpl3);
#line 588 "../lib/Chai.chpl"
  tup_x0_chpl3 = elt_x0_chpl3;
#line 588 "../lib/Chai.chpl"
  _ic__F4_block_x0_chpl = tup_x0_chpl3;
#line 588 "../lib/Chai.chpl"
  coerce_tmp_chpl22 = *(chpl_followThis + INT64(0));
#line 588 "../lib/Chai.chpl"
  coerce_tmp_chpl28 = (&coerce_tmp_chpl22)->_high;
#line 588 "../lib/Chai.chpl"
  this_chpl44 = ret_chpl9;
#line 588 "../lib/Chai.chpl"
  chpl_check_nil(this_chpl44, INT64(588), INT32(65));
#line 588 "../lib/Chai.chpl"
  call_tmp_chpl31 = &((this_chpl44)->ranges);
#line 588 "../lib/Chai.chpl"
  coerce_tmp_chpl23 = *(*(call_tmp_chpl31) + INT64(0));
#line 588 "../lib/Chai.chpl"
  call_tmp_chpl29 = sizeAs_chpl36(coerce_tmp_chpl23, INT64(588), INT32(65));
#line 588 "../lib/Chai.chpl"
  if (! (coerce_tmp_chpl28 < INT64(0))) /* ZLINE: 588 ../lib/Chai.chpl */
#line 588 "../lib/Chai.chpl"
  {
#line 588 "../lib/Chai.chpl"
    tmp_chpl6 = (((uint64_t)(coerce_tmp_chpl28)) >= call_tmp_chpl29);
#line 588 "../lib/Chai.chpl"
  } else /* ZLINE: 588 ../lib/Chai.chpl */
#line 588 "../lib/Chai.chpl"
  {
#line 588 "../lib/Chai.chpl"
    tmp_chpl6 = UINT8(false);
#line 588 "../lib/Chai.chpl"
  }
#line 588 "../lib/Chai.chpl"
  if (tmp_chpl6) /* ZLINE: 588 ../lib/Chai.chpl */
#line 588 "../lib/Chai.chpl"
  {
#line 588 "../lib/Chai.chpl"
    boundsCheckHalt_chpl(&_str_literal_4152_chpl, INT64(588), INT32(65));
#line 588 "../lib/Chai.chpl"
  }
#line 588 "../lib/Chai.chpl"
  this_chpl45 = ret_chpl9;
#line 588 "../lib/Chai.chpl"
  chpl_check_nil(this_chpl45, INT64(588), INT32(65));
#line 588 "../lib/Chai.chpl"
  call_tmp_chpl32 = &((this_chpl45)->ranges);
#line 588 "../lib/Chai.chpl"
  coerce_tmp_chpl24 = *(*(call_tmp_chpl32) + INT64(0));
#line 588 "../lib/Chai.chpl"
  coerce_tmp_chpl25 = *(chpl_followThis + INT64(0));
#line 588 "../lib/Chai.chpl"
  call_tmp_chpl33 = ((&coerce_tmp_chpl24)->_low + ((int64_t)(((&coerce_tmp_chpl25)->_low * INT64(1)))));
#line 588 "../lib/Chai.chpl"
  this_chpl46 = ret_chpl9;
#line 588 "../lib/Chai.chpl"
  chpl_check_nil(this_chpl46, INT64(588), INT32(65));
#line 588 "../lib/Chai.chpl"
  call_tmp_chpl34 = &((this_chpl46)->ranges);
#line 588 "../lib/Chai.chpl"
  coerce_tmp_chpl26 = *(*(call_tmp_chpl34) + INT64(0));
#line 588 "../lib/Chai.chpl"
  coerce_tmp_chpl27 = *(chpl_followThis + INT64(0));
#line 588 "../lib/Chai.chpl"
  call_tmp_chpl35 = ((&coerce_tmp_chpl26)->_low + ((int64_t)(((&coerce_tmp_chpl27)->_high * INT64(1)))));
#line 588 "../lib/Chai.chpl"
  call_tmp_chpl30 = &_ic__F4_block_x0_chpl;
#line 588 "../lib/Chai.chpl"
  compilerAssert_chpl();
#line 588 "../lib/Chai.chpl"
  (call_tmp_chpl30)->_low = call_tmp_chpl33;
#line 588 "../lib/Chai.chpl"
  (call_tmp_chpl30)->_high = call_tmp_chpl35;
#line 588 "../lib/Chai.chpl"
  *(retcopy_chpl3 + INT64(0)) = _ic__F4_block_x0_chpl;
#line 588 "../lib/Chai.chpl"
  _ic__F6_coerce_tmp_x0_chpl = *(retcopy_chpl3 + INT64(0));
#line 588 "../lib/Chai.chpl"
  _ic__F0_this_chpl5 = _ic__F6_coerce_tmp_x0_chpl;
#line 588 "../lib/Chai.chpl"
  checkIfIterWillOverflow_chpl(_ic__F6_coerce_tmp_x0_chpl, UINT8(true), INT64(588), INT32(65));
#line 588 "../lib/Chai.chpl"
  _ic__F7_i_chpl = INT64(0);
#line 588 "../lib/Chai.chpl"
  this_chpl47 = _ic__F0_this_chpl5;
#line 588 "../lib/Chai.chpl"
  this_chpl48 = _ic__F0_this_chpl5;
#line 588 "../lib/Chai.chpl"
  _ic__F8_end_chpl = (&this_chpl48)->_high;
#line 588 "../lib/Chai.chpl"
  for (_ic__F8_i_chpl = coerce_tmp_chpl12,_ic__F8_i_chpl2 = coerce_tmp_chpl21,_ic__F7_i_chpl = (&this_chpl47)->_low; (tmp_chpl9 = (_ic__F8_i_chpl <= _ic__F9_end_chpl),tmp_chpl9); tmp_chpl10 = _ic__F8_i_chpl,tmp_chpl10 += INT64(1),_ic__F8_i_chpl = tmp_chpl10,tmp_chpl11 = _ic__F8_i_chpl2,tmp_chpl11 += INT64(1),_ic__F8_i_chpl2 = tmp_chpl11,tmp_chpl12 = _ic__F7_i_chpl,tmp_chpl12 += INT64(1),_ic__F7_i_chpl = tmp_chpl12) {
#line 588 "../lib/Chai.chpl"
    tmp_chpl7 = (_ic__F7_i_chpl <= _ic__F8_end_chpl);
#line 588 "../lib/Chai.chpl"
    hasMore_chpl2 = (tmp_chpl7 != INT64(0));
#line 588 "../lib/Chai.chpl"
    if (! hasMore_chpl2) /* ZLINE: 588 ../lib/Chai.chpl */
#line 588 "../lib/Chai.chpl"
    {
#line 588 "../lib/Chai.chpl"
      chpl_error("zippered iterations have non-equal lengths", INT64(588), INT32(65));
#line 588 "../lib/Chai.chpl"
    }
#line 588 "../lib/Chai.chpl"
    this_chpl49 = _ic__F0_this_chpl3;
#line 588 "../lib/Chai.chpl"
    chpl_check_nil(this_chpl49, INT64(588), INT32(65));
#line 588 "../lib/Chai.chpl"
    coerce_tmp_chpl29 = (this_chpl49)->shiftedData;
#line 588 "../lib/Chai.chpl"
    call_tmp_chpl36 = (coerce_tmp_chpl29 + _ic__F8_i_chpl2);
#line 588 "../lib/Chai.chpl"
    tmp_chpl8 = (_ic__F8_i_chpl2 <= _ic__F9_end_chpl2);
#line 588 "../lib/Chai.chpl"
    hasMore_chpl = (tmp_chpl8 != INT64(0));
#line 588 "../lib/Chai.chpl"
    if (! hasMore_chpl) /* ZLINE: 588 ../lib/Chai.chpl */
#line 588 "../lib/Chai.chpl"
    {
#line 588 "../lib/Chai.chpl"
      chpl_error("zippered iterations have non-equal lengths", INT64(588), INT32(65));
#line 588 "../lib/Chai.chpl"
    }
#line 588 "../lib/Chai.chpl"
    this_chpl50 = _ic__F0_this_chpl;
#line 588 "../lib/Chai.chpl"
    chpl_check_nil(this_chpl50, INT64(588), INT32(65));
#line 588 "../lib/Chai.chpl"
    coerce_tmp_chpl30 = (this_chpl50)->shiftedData;
#line 588 "../lib/Chai.chpl"
    call_tmp_chpl37 = (coerce_tmp_chpl30 + _ic__F8_i_chpl);
#line 588 "../lib/Chai.chpl"
    idx_chpl = _ic__F7_i_chpl;
#line 590 "../lib/Chai.chpl"
    flatten_chpl(call_tmp_chpl36, &ret_tmp_chpl6);
#line 590 "../lib/Chai.chpl"
    call_tmp_chpl38 = ret_tmp_chpl6;
#line 590 "../lib/Chai.chpl"
    flattened_chpl = call_tmp_chpl38;
#line 591 "../lib/Chai.chpl"
    call_tmp_chpl39 = &((this_chpl31)->weights);
#line 591 "../lib/Chai.chpl"
    chpl___ASTERISK_4(call_tmp_chpl39, &flattened_chpl, &ret_tmp_chpl7);
#line 591 "../lib/Chai.chpl"
    call_tmp_chpl40 = ret_tmp_chpl7;
#line 591 "../lib/Chai.chpl"
    call_tmp_chpl41 = &((this_chpl31)->biases);
#line 591 "../lib/Chai.chpl"
    chpl___PLUS_2(&call_tmp_chpl40, call_tmp_chpl41, &ret_tmp_chpl8);
#line 591 "../lib/Chai.chpl"
    call_tmp_chpl42 = ret_tmp_chpl8;
#line 591 "../lib/Chai.chpl"
    Z_chpl = call_tmp_chpl42;
#line 591 "../lib/Chai.chpl"
    _field_destructor_tmp__chpl = (&call_tmp_chpl40)->data;
#line 591 "../lib/Chai.chpl"
    chpl__autoDestroy18(&_field_destructor_tmp__chpl, INT64(591), INT32(65));
#line 591 "../lib/Chai.chpl"
    _field_destructor_tmp__chpl2 = (&call_tmp_chpl40)->_domain;
#line 591 "../lib/Chai.chpl"
    chpl__autoDestroy11(&_field_destructor_tmp__chpl2, INT64(591), INT32(65));
#line 592 "../lib/Chai.chpl"
    softmaxParts_chpl(&Z_chpl, &ret_tmp_chpl9);
#line 592 "../lib/Chai.chpl"
    call_tmp_chpl43 = ret_tmp_chpl9;
#line 592 "../lib/Chai.chpl"
    tmp_x0_chpl2 = (&call_tmp_chpl43)->x0;
#line 592 "../lib/Chai.chpl"
    tmp_x1_chpl2 = (&call_tmp_chpl43)->x1;
#line 592 "../lib/Chai.chpl"
    tmp_x2_chpl = (&call_tmp_chpl43)->x2;
#line 592 "../lib/Chai.chpl"
    call_tmp_chpl44 = &tmp_x0_chpl2;
#line 592 "../lib/Chai.chpl"
    init_ASSIGN__chpl32(&exp_chpl, call_tmp_chpl44);
#line 592 "../lib/Chai.chpl"
    expSum_chpl = tmp_x1_chpl2;
#line 592 "../lib/Chai.chpl"
    call_tmp_chpl45 = &tmp_x2_chpl;
#line 592 "../lib/Chai.chpl"
    init_ASSIGN__chpl32(&softmax_chpl2, call_tmp_chpl45);
#line 593 "../lib/Chai.chpl"
    init_ASSIGN__chpl32(&dL_dOut_chpl, call_tmp_chpl37);
#line 594 "../lib/Chai.chpl"
    nonZeroIdx_chpl = INT64(-1);
#line 595 "../lib/Chai.chpl"
    call_tmp_chpl46 = &((call_tmp_chpl37)->data);
#line 595 "../lib/Chai.chpl"
    _dom_chpl18(call_tmp_chpl46, &ret_tmp_chpl10, INT64(595), INT32(65));
#line 595 "../lib/Chai.chpl"
    call_tmp_chpl47 = ret_tmp_chpl10;
#line 595 "../lib/Chai.chpl"
    this_chpl51 = call_tmp_chpl47;
#line 595 "../lib/Chai.chpl"
    ret_chpl10 = (&this_chpl51)->_instance;
#line 595 "../lib/Chai.chpl"
    createTuple_chpl(INT64(0), &ret_tmp_chpl11);
#line 595 "../lib/Chai.chpl"
    this_chpl52 = ret_chpl10;
#line 595 "../lib/Chai.chpl"
    chpl_check_nil(this_chpl52, INT64(595), INT32(65));
#line 595 "../lib/Chai.chpl"
    call_tmp_chpl48 = &((this_chpl52)->ranges);
#line 595 "../lib/Chai.chpl"
    coerce_tmp_chpl31 = *(*(call_tmp_chpl48) + INT64(0));
#line 595 "../lib/Chai.chpl"
    _ic__F0_this_chpl6 = coerce_tmp_chpl31;
#line 595 "../lib/Chai.chpl"
    checkIfIterWillOverflow_chpl(coerce_tmp_chpl31, UINT8(true), INT64(595), INT32(65));
#line 595 "../lib/Chai.chpl"
    i_chpl = INT64(0);
#line 595 "../lib/Chai.chpl"
    this_chpl53 = _ic__F0_this_chpl6;
#line 595 "../lib/Chai.chpl"
    this_chpl54 = _ic__F0_this_chpl6;
#line 595 "../lib/Chai.chpl"
    end_chpl = (&this_chpl54)->_high;
#line 595 "../lib/Chai.chpl"
    for (i_chpl = (&this_chpl53)->_low; ((i_chpl <= end_chpl)); i_chpl += INT64(1)) {
#line 596 "../lib/Chai.chpl"
      call_tmp_chpl49 = &((call_tmp_chpl37)->data);
#line 596 "../lib/Chai.chpl"
      *(coerce_tmp_chpl32 + INT64(0)) = i_chpl;
#line 596 "../lib/Chai.chpl"
      i_i_chpl = &coerce_tmp_chpl32;
#line 596 "../lib/Chai.chpl"
      ret_chpl11 = (call_tmp_chpl49)->_instance;
#line 596 "../lib/Chai.chpl"
      checkAccess_chpl11(call_tmp_chpl49, i_i_chpl, ret_chpl11, INT64(596), INT32(65));
#line 596 "../lib/Chai.chpl"
      coerce_tmp_chpl33 = ret_chpl11;
#line 596 "../lib/Chai.chpl"
      *(_formal_tmp_in_ind_chpl + INT64(0)) = *(coerce_tmp_chpl32 + INT64(0));
#line 596 "../lib/Chai.chpl"
      _formal_tmp_ind_x0_chpl = *(_formal_tmp_in_ind_chpl + INT64(0));
#line 596 "../lib/Chai.chpl"
      chpl_check_nil(coerce_tmp_chpl33, INT64(596), INT32(65));
#line 596 "../lib/Chai.chpl"
      coerce_tmp_chpl34 = (coerce_tmp_chpl33)->shiftedData;
#line 596 "../lib/Chai.chpl"
      call_tmp_chpl50 = (coerce_tmp_chpl34 + _formal_tmp_ind_x0_chpl);
#line 596 "../lib/Chai.chpl"
      ret_chpl12 = *(call_tmp_chpl50);
#line 596 "../lib/Chai.chpl"
      if (ret_chpl12 != REAL64(0x0p+0)) /* ZLINE: 596 ../lib/Chai.chpl */
#line 596 "../lib/Chai.chpl"
      {
#line 596 "../lib/Chai.chpl"
        nonZeroIdx_chpl = i_chpl;
#line 596 "../lib/Chai.chpl"
        goto _breakLabel_chpl;
#line 596 "../lib/Chai.chpl"
      }
#line 596 "../lib/Chai.chpl"
    }
#line 596 "../lib/Chai.chpl"
    _breakLabel_chpl:;
#line 595 "../lib/Chai.chpl"
    chpl__autoDestroy11(&call_tmp_chpl47, INT64(595), INT32(65));
#line 597 "../lib/Chai.chpl"
    if (nonZeroIdx_chpl == INT64(-1)) /* ZLINE: 597 ../lib/Chai.chpl */
#line 597 "../lib/Chai.chpl"
    {
#line 597 "../lib/Chai.chpl"
      err_chpl4(&_str_literal_1720_chpl);
#line 597 "../lib/Chai.chpl"
    }
#line 599 "../lib/Chai.chpl"
    call_tmp_chpl51 = &((&exp_chpl)->data);
#line 599 "../lib/Chai.chpl"
    *(coerce_tmp_chpl35 + INT64(0)) = nonZeroIdx_chpl;
#line 599 "../lib/Chai.chpl"
    i_i_chpl2 = &coerce_tmp_chpl35;
#line 599 "../lib/Chai.chpl"
    ret_chpl13 = (call_tmp_chpl51)->_instance;
#line 599 "../lib/Chai.chpl"
    checkAccess_chpl11(call_tmp_chpl51, i_i_chpl2, ret_chpl13, INT64(599), INT32(65));
#line 599 "../lib/Chai.chpl"
    coerce_tmp_chpl36 = ret_chpl13;
#line 599 "../lib/Chai.chpl"
    *(_formal_tmp_in_ind_chpl2 + INT64(0)) = *(coerce_tmp_chpl35 + INT64(0));
#line 599 "../lib/Chai.chpl"
    _formal_tmp_ind_x0_chpl2 = *(_formal_tmp_in_ind_chpl2 + INT64(0));
#line 599 "../lib/Chai.chpl"
    chpl_check_nil(coerce_tmp_chpl36, INT64(599), INT32(65));
#line 599 "../lib/Chai.chpl"
    coerce_tmp_chpl37 = (coerce_tmp_chpl36)->shiftedData;
#line 599 "../lib/Chai.chpl"
    call_tmp_chpl52 = (coerce_tmp_chpl37 + _formal_tmp_ind_x0_chpl2);
#line 599 "../lib/Chai.chpl"
    call_tmp_chpl53 = (-*(call_tmp_chpl52));
#line 599 "../lib/Chai.chpl"
    chpl___SLASH_2(&exp_chpl, pow(expSum_chpl, REAL64(0x1p+1)), &ret_tmp_chpl12);
#line 599 "../lib/Chai.chpl"
    call_tmp_chpl54 = ret_tmp_chpl12;
#line 599 "../lib/Chai.chpl"
    chpl___ASTERISK_2(call_tmp_chpl53, &call_tmp_chpl54, &ret_tmp_chpl13);
#line 599 "../lib/Chai.chpl"
    call_tmp_chpl55 = ret_tmp_chpl13;
#line 599 "../lib/Chai.chpl"
    dOut_dZ_chpl = call_tmp_chpl55;
#line 599 "../lib/Chai.chpl"
    _field_destructor_tmp__chpl3 = (&call_tmp_chpl54)->data;
#line 599 "../lib/Chai.chpl"
    chpl__autoDestroy18(&_field_destructor_tmp__chpl3, INT64(599), INT32(65));
#line 599 "../lib/Chai.chpl"
    _field_destructor_tmp__chpl4 = (&call_tmp_chpl54)->_domain;
#line 599 "../lib/Chai.chpl"
    chpl__autoDestroy11(&_field_destructor_tmp__chpl4, INT64(599), INT32(65));
#line 600 "../lib/Chai.chpl"
    call_tmp_chpl56 = &((&dOut_dZ_chpl)->data);
#line 600 "../lib/Chai.chpl"
    *(coerce_tmp_chpl38 + INT64(0)) = nonZeroIdx_chpl;
#line 600 "../lib/Chai.chpl"
    i_i_chpl3 = &coerce_tmp_chpl38;
#line 600 "../lib/Chai.chpl"
    ret_chpl14 = (call_tmp_chpl56)->_instance;
#line 600 "../lib/Chai.chpl"
    checkAccess_chpl11(call_tmp_chpl56, i_i_chpl3, ret_chpl14, INT64(600), INT32(65));
#line 600 "../lib/Chai.chpl"
    coerce_tmp_chpl39 = ret_chpl14;
#line 600 "../lib/Chai.chpl"
    *(_formal_tmp_in_ind_chpl3 + INT64(0)) = *(coerce_tmp_chpl38 + INT64(0));
#line 600 "../lib/Chai.chpl"
    _formal_tmp_ind_x0_chpl3 = *(_formal_tmp_in_ind_chpl3 + INT64(0));
#line 600 "../lib/Chai.chpl"
    chpl_check_nil(coerce_tmp_chpl39, INT64(600), INT32(65));
#line 600 "../lib/Chai.chpl"
    coerce_tmp_chpl40 = (coerce_tmp_chpl39)->shiftedData;
#line 600 "../lib/Chai.chpl"
    call_tmp_chpl57 = (coerce_tmp_chpl40 + _formal_tmp_ind_x0_chpl3);
#line 600 "../lib/Chai.chpl"
    call_tmp_chpl58 = &((&exp_chpl)->data);
#line 600 "../lib/Chai.chpl"
    *(coerce_tmp_chpl41 + INT64(0)) = nonZeroIdx_chpl;
#line 600 "../lib/Chai.chpl"
    i_i_chpl4 = &coerce_tmp_chpl41;
#line 600 "../lib/Chai.chpl"
    ret_chpl15 = (call_tmp_chpl58)->_instance;
#line 600 "../lib/Chai.chpl"
    checkAccess_chpl11(call_tmp_chpl58, i_i_chpl4, ret_chpl15, INT64(600), INT32(65));
#line 600 "../lib/Chai.chpl"
    coerce_tmp_chpl42 = ret_chpl15;
#line 600 "../lib/Chai.chpl"
    *(_formal_tmp_in_ind_chpl4 + INT64(0)) = *(coerce_tmp_chpl41 + INT64(0));
#line 600 "../lib/Chai.chpl"
    _formal_tmp_ind_x0_chpl4 = *(_formal_tmp_in_ind_chpl4 + INT64(0));
#line 600 "../lib/Chai.chpl"
    chpl_check_nil(coerce_tmp_chpl42, INT64(600), INT32(65));
#line 600 "../lib/Chai.chpl"
    coerce_tmp_chpl43 = (coerce_tmp_chpl42)->shiftedData;
#line 600 "../lib/Chai.chpl"
    call_tmp_chpl59 = (coerce_tmp_chpl43 + _formal_tmp_ind_x0_chpl4);
#line 600 "../lib/Chai.chpl"
    ret_chpl16 = *(call_tmp_chpl59);
#line 600 "../lib/Chai.chpl"
    call_tmp_chpl60 = &((&exp_chpl)->data);
#line 600 "../lib/Chai.chpl"
    *(coerce_tmp_chpl44 + INT64(0)) = nonZeroIdx_chpl;
#line 600 "../lib/Chai.chpl"
    i_i_chpl5 = &coerce_tmp_chpl44;
#line 600 "../lib/Chai.chpl"
    ret_chpl17 = (call_tmp_chpl60)->_instance;
#line 600 "../lib/Chai.chpl"
    checkAccess_chpl11(call_tmp_chpl60, i_i_chpl5, ret_chpl17, INT64(600), INT32(65));
#line 600 "../lib/Chai.chpl"
    coerce_tmp_chpl45 = ret_chpl17;
#line 600 "../lib/Chai.chpl"
    *(_formal_tmp_in_ind_chpl5 + INT64(0)) = *(coerce_tmp_chpl44 + INT64(0));
#line 600 "../lib/Chai.chpl"
    _formal_tmp_ind_x0_chpl5 = *(_formal_tmp_in_ind_chpl5 + INT64(0));
#line 600 "../lib/Chai.chpl"
    chpl_check_nil(coerce_tmp_chpl45, INT64(600), INT32(65));
#line 600 "../lib/Chai.chpl"
    coerce_tmp_chpl46 = (coerce_tmp_chpl45)->shiftedData;
#line 600 "../lib/Chai.chpl"
    call_tmp_chpl61 = (coerce_tmp_chpl46 + _formal_tmp_ind_x0_chpl5);
#line 600 "../lib/Chai.chpl"
    call_tmp_chpl62 = (((_real64)((ret_chpl16 * ((_real64)((expSum_chpl - *(call_tmp_chpl61))))))) / pow(expSum_chpl, REAL64(0x1p+1)));
#line 600 "../lib/Chai.chpl"
    *(call_tmp_chpl57) = call_tmp_chpl62;
#line 601 "../lib/Chai.chpl"
    dZ_dW_chpl = flattened_chpl;
#line 603 "../lib/Chai.chpl"
    call_tmp_chpl63 = &((this_chpl31)->weights);
#line 603 "../lib/Chai.chpl"
    init_ASSIGN__chpl31(&dZ_dIn_chpl, call_tmp_chpl63);
#line 604 "../lib/Chai.chpl"
    call_tmp_chpl64 = &((&dL_dOut_chpl)->data);
#line 604 "../lib/Chai.chpl"
    *(coerce_tmp_chpl47 + INT64(0)) = nonZeroIdx_chpl;
#line 604 "../lib/Chai.chpl"
    i_i_chpl6 = &coerce_tmp_chpl47;
#line 604 "../lib/Chai.chpl"
    ret_chpl18 = (call_tmp_chpl64)->_instance;
#line 604 "../lib/Chai.chpl"
    checkAccess_chpl11(call_tmp_chpl64, i_i_chpl6, ret_chpl18, INT64(604), INT32(65));
#line 604 "../lib/Chai.chpl"
    coerce_tmp_chpl48 = ret_chpl18;
#line 604 "../lib/Chai.chpl"
    *(_formal_tmp_in_ind_chpl6 + INT64(0)) = *(coerce_tmp_chpl47 + INT64(0));
#line 604 "../lib/Chai.chpl"
    _formal_tmp_ind_x0_chpl6 = *(_formal_tmp_in_ind_chpl6 + INT64(0));
#line 604 "../lib/Chai.chpl"
    chpl_check_nil(coerce_tmp_chpl48, INT64(604), INT32(65));
#line 604 "../lib/Chai.chpl"
    coerce_tmp_chpl49 = (coerce_tmp_chpl48)->shiftedData;
#line 604 "../lib/Chai.chpl"
    call_tmp_chpl65 = (coerce_tmp_chpl49 + _formal_tmp_ind_x0_chpl6);
#line 604 "../lib/Chai.chpl"
    chpl___ASTERISK_2(*(call_tmp_chpl65), &dOut_dZ_chpl, &ret_tmp_chpl14);
#line 604 "../lib/Chai.chpl"
    call_tmp_chpl66 = ret_tmp_chpl14;
#line 604 "../lib/Chai.chpl"
    dL_dZ_chpl = call_tmp_chpl66;
#line 605 "../lib/Chai.chpl"
    transpose_chpl(&dZ_dW_chpl, &ret_tmp_chpl15);
#line 605 "../lib/Chai.chpl"
    call_tmp_chpl67 = ret_tmp_chpl15;
#line 605 "../lib/Chai.chpl"
    chpl___ASTERISK_5(&dL_dZ_chpl, &call_tmp_chpl67, &ret_tmp_chpl16);
#line 605 "../lib/Chai.chpl"
    call_tmp_chpl68 = ret_tmp_chpl16;
#line 605 "../lib/Chai.chpl"
    dL_dW_chpl = call_tmp_chpl68;
#line 605 "../lib/Chai.chpl"
    _field_destructor_tmp__chpl5 = (&call_tmp_chpl67)->data;
#line 605 "../lib/Chai.chpl"
    chpl__autoDestroy19(&_field_destructor_tmp__chpl5, INT64(605), INT32(65));
#line 605 "../lib/Chai.chpl"
    _field_destructor_tmp__chpl6 = (&call_tmp_chpl67)->_domain;
#line 605 "../lib/Chai.chpl"
    chpl__autoDestroy5(&_field_destructor_tmp__chpl6, INT64(605), INT32(65));
#line 606 "../lib/Chai.chpl"
    chpl___ASTERISK_3(&dL_dZ_chpl, REAL64(0x1p+0), &ret_tmp_chpl17);
#line 606 "../lib/Chai.chpl"
    call_tmp_chpl69 = ret_tmp_chpl17;
#line 606 "../lib/Chai.chpl"
    dL_dB_chpl = call_tmp_chpl69;
#line 607 "../lib/Chai.chpl"
    transpose_chpl2(&dZ_dIn_chpl, &ret_tmp_chpl18);
#line 607 "../lib/Chai.chpl"
    call_tmp_chpl70 = ret_tmp_chpl18;
#line 607 "../lib/Chai.chpl"
    chpl___ASTERISK_4(&call_tmp_chpl70, &dL_dZ_chpl, &ret_tmp_chpl19);
#line 607 "../lib/Chai.chpl"
    call_tmp_chpl71 = ret_tmp_chpl19;
#line 607 "../lib/Chai.chpl"
    dL_dIn_chpl = call_tmp_chpl71;
#line 607 "../lib/Chai.chpl"
    _field_destructor_tmp__chpl7 = (&call_tmp_chpl70)->data;
#line 607 "../lib/Chai.chpl"
    chpl__autoDestroy19(&_field_destructor_tmp__chpl7, INT64(607), INT32(65));
#line 607 "../lib/Chai.chpl"
    _field_destructor_tmp__chpl8 = (&call_tmp_chpl70)->_domain;
#line 607 "../lib/Chai.chpl"
    chpl__autoDestroy5(&_field_destructor_tmp__chpl8, INT64(607), INT32(65));
#line 608 "../lib/Chai.chpl"
    chpl___PLUS__ASSIGN_2(&AS_weightsGrad_chpl, &dL_dW_chpl);
#line 609 "../lib/Chai.chpl"
    chpl___PLUS__ASSIGN_4(&AS_biasesGrad_chpl, &dL_dB_chpl);
#line 610 "../lib/Chai.chpl"
    *(coerce_tmp_chpl50 + INT64(0)) = idx_chpl;
#line 610 "../lib/Chai.chpl"
    i_i_chpl7 = &coerce_tmp_chpl50;
#line 610 "../lib/Chai.chpl"
    ret_chpl19 = (newDeltas_chpl)->_instance;
#line 610 "../lib/Chai.chpl"
    checkAccess_chpl10(newDeltas_chpl, i_i_chpl7, ret_chpl19, INT64(610), INT32(65));
#line 610 "../lib/Chai.chpl"
    coerce_tmp_chpl51 = ret_chpl19;
#line 610 "../lib/Chai.chpl"
    *(_formal_tmp_in_ind_chpl7 + INT64(0)) = *(coerce_tmp_chpl50 + INT64(0));
#line 610 "../lib/Chai.chpl"
    _formal_tmp_ind_x0_chpl7 = *(_formal_tmp_in_ind_chpl7 + INT64(0));
#line 610 "../lib/Chai.chpl"
    chpl_check_nil(coerce_tmp_chpl51, INT64(610), INT32(65));
#line 610 "../lib/Chai.chpl"
    coerce_tmp_chpl52 = (coerce_tmp_chpl51)->shiftedData;
#line 610 "../lib/Chai.chpl"
    call_tmp_chpl72 = (coerce_tmp_chpl52 + _formal_tmp_ind_x0_chpl7);
#line 610 "../lib/Chai.chpl"
    _dom_chpl22(call_tmp_chpl36, &ret_tmp_chpl20);
#line 610 "../lib/Chai.chpl"
    call_tmp_chpl73 = ret_tmp_chpl20;
#line 610 "../lib/Chai.chpl"
    reshape_chpl(&dL_dIn_chpl, &call_tmp_chpl73, &ret_tmp_chpl21);
#line 610 "../lib/Chai.chpl"
    call_tmp_chpl74 = ret_tmp_chpl21;
#line 610 "../lib/Chai.chpl"
    chpl___ASSIGN_24(call_tmp_chpl72, &call_tmp_chpl74);
#line 610 "../lib/Chai.chpl"
    _field_destructor_tmp__chpl9 = (&call_tmp_chpl74)->data;
#line 610 "../lib/Chai.chpl"
    chpl__autoDestroy28(&_field_destructor_tmp__chpl9, INT64(610), INT32(65));
#line 610 "../lib/Chai.chpl"
    _field_destructor_tmp__chpl10 = (&call_tmp_chpl74)->_domain;
#line 610 "../lib/Chai.chpl"
    chpl__autoDestroy8(&_field_destructor_tmp__chpl10, INT64(610), INT32(65));
#line 610 "../lib/Chai.chpl"
    chpl__autoDestroy8(&call_tmp_chpl73, INT64(610), INT32(65));
#line 610 "../lib/Chai.chpl"
    _field_destructor_tmp__chpl11 = (&dL_dIn_chpl)->data;
#line 610 "../lib/Chai.chpl"
    chpl__autoDestroy18(&_field_destructor_tmp__chpl11, INT64(610), INT32(65));
#line 610 "../lib/Chai.chpl"
    _field_destructor_tmp__chpl12 = (&dL_dIn_chpl)->_domain;
#line 610 "../lib/Chai.chpl"
    chpl__autoDestroy11(&_field_destructor_tmp__chpl12, INT64(610), INT32(65));
#line 610 "../lib/Chai.chpl"
    _field_destructor_tmp__chpl13 = (&dL_dB_chpl)->data;
#line 610 "../lib/Chai.chpl"
    chpl__autoDestroy18(&_field_destructor_tmp__chpl13, INT64(610), INT32(65));
#line 610 "../lib/Chai.chpl"
    _field_destructor_tmp__chpl14 = (&dL_dB_chpl)->_domain;
#line 610 "../lib/Chai.chpl"
    chpl__autoDestroy11(&_field_destructor_tmp__chpl14, INT64(610), INT32(65));
#line 610 "../lib/Chai.chpl"
    _field_destructor_tmp__chpl15 = (&dL_dW_chpl)->data;
#line 610 "../lib/Chai.chpl"
    chpl__autoDestroy19(&_field_destructor_tmp__chpl15, INT64(610), INT32(65));
#line 610 "../lib/Chai.chpl"
    _field_destructor_tmp__chpl16 = (&dL_dW_chpl)->_domain;
#line 610 "../lib/Chai.chpl"
    chpl__autoDestroy5(&_field_destructor_tmp__chpl16, INT64(610), INT32(65));
#line 610 "../lib/Chai.chpl"
    _field_destructor_tmp__chpl17 = (&dL_dZ_chpl)->data;
#line 610 "../lib/Chai.chpl"
    chpl__autoDestroy18(&_field_destructor_tmp__chpl17, INT64(610), INT32(65));
#line 610 "../lib/Chai.chpl"
    _field_destructor_tmp__chpl18 = (&dL_dZ_chpl)->_domain;
#line 610 "../lib/Chai.chpl"
    chpl__autoDestroy11(&_field_destructor_tmp__chpl18, INT64(610), INT32(65));
#line 610 "../lib/Chai.chpl"
    _field_destructor_tmp__chpl19 = (&dZ_dIn_chpl)->data;
#line 610 "../lib/Chai.chpl"
    chpl__autoDestroy19(&_field_destructor_tmp__chpl19, INT64(610), INT32(65));
#line 610 "../lib/Chai.chpl"
    _field_destructor_tmp__chpl20 = (&dZ_dIn_chpl)->_domain;
#line 610 "../lib/Chai.chpl"
    chpl__autoDestroy5(&_field_destructor_tmp__chpl20, INT64(610), INT32(65));
#line 610 "../lib/Chai.chpl"
    _field_destructor_tmp__chpl21 = (&dZ_dW_chpl)->data;
#line 610 "../lib/Chai.chpl"
    chpl__autoDestroy18(&_field_destructor_tmp__chpl21, INT64(610), INT32(65));
#line 610 "../lib/Chai.chpl"
    _field_destructor_tmp__chpl22 = (&dZ_dW_chpl)->_domain;
#line 610 "../lib/Chai.chpl"
    chpl__autoDestroy11(&_field_destructor_tmp__chpl22, INT64(610), INT32(65));
#line 610 "../lib/Chai.chpl"
    _field_destructor_tmp__chpl23 = (&dOut_dZ_chpl)->data;
#line 610 "../lib/Chai.chpl"
    chpl__autoDestroy18(&_field_destructor_tmp__chpl23, INT64(610), INT32(65));
#line 610 "../lib/Chai.chpl"
    _field_destructor_tmp__chpl24 = (&dOut_dZ_chpl)->_domain;
#line 610 "../lib/Chai.chpl"
    chpl__autoDestroy11(&_field_destructor_tmp__chpl24, INT64(610), INT32(65));
#line 610 "../lib/Chai.chpl"
    _field_destructor_tmp__chpl25 = (&dL_dOut_chpl)->data;
#line 610 "../lib/Chai.chpl"
    chpl__autoDestroy18(&_field_destructor_tmp__chpl25, INT64(610), INT32(65));
#line 610 "../lib/Chai.chpl"
    _field_destructor_tmp__chpl26 = (&dL_dOut_chpl)->_domain;
#line 610 "../lib/Chai.chpl"
    chpl__autoDestroy11(&_field_destructor_tmp__chpl26, INT64(610), INT32(65));
#line 610 "../lib/Chai.chpl"
    _field_destructor_tmp__chpl27 = (&softmax_chpl2)->data;
#line 610 "../lib/Chai.chpl"
    chpl__autoDestroy18(&_field_destructor_tmp__chpl27, INT64(610), INT32(65));
#line 610 "../lib/Chai.chpl"
    _field_destructor_tmp__chpl28 = (&softmax_chpl2)->_domain;
#line 610 "../lib/Chai.chpl"
    chpl__autoDestroy11(&_field_destructor_tmp__chpl28, INT64(610), INT32(65));
#line 610 "../lib/Chai.chpl"
    _field_destructor_tmp__chpl29 = (&exp_chpl)->data;
#line 610 "../lib/Chai.chpl"
    chpl__autoDestroy18(&_field_destructor_tmp__chpl29, INT64(610), INT32(65));
#line 610 "../lib/Chai.chpl"
    _field_destructor_tmp__chpl30 = (&exp_chpl)->_domain;
#line 610 "../lib/Chai.chpl"
    chpl__autoDestroy11(&_field_destructor_tmp__chpl30, INT64(610), INT32(65));
#line 610 "../lib/Chai.chpl"
    _field_destructor_tmp__chpl31 = (&tmp_x2_chpl)->data;
#line 610 "../lib/Chai.chpl"
    chpl__autoDestroy18(&_field_destructor_tmp__chpl31, INT64(610), INT32(65));
#line 610 "../lib/Chai.chpl"
    _field_destructor_tmp__chpl32 = (&tmp_x2_chpl)->_domain;
#line 610 "../lib/Chai.chpl"
    chpl__autoDestroy11(&_field_destructor_tmp__chpl32, INT64(610), INT32(65));
#line 610 "../lib/Chai.chpl"
    _field_destructor_tmp__chpl33 = (&tmp_x0_chpl2)->data;
#line 610 "../lib/Chai.chpl"
    chpl__autoDestroy18(&_field_destructor_tmp__chpl33, INT64(610), INT32(65));
#line 610 "../lib/Chai.chpl"
    _field_destructor_tmp__chpl34 = (&tmp_x0_chpl2)->_domain;
#line 610 "../lib/Chai.chpl"
    chpl__autoDestroy11(&_field_destructor_tmp__chpl34, INT64(610), INT32(65));
#line 610 "../lib/Chai.chpl"
    _field_destructor_tmp__chpl35 = (&Z_chpl)->data;
#line 610 "../lib/Chai.chpl"
    chpl__autoDestroy18(&_field_destructor_tmp__chpl35, INT64(610), INT32(65));
#line 610 "../lib/Chai.chpl"
    _field_destructor_tmp__chpl36 = (&Z_chpl)->_domain;
#line 610 "../lib/Chai.chpl"
    chpl__autoDestroy11(&_field_destructor_tmp__chpl36, INT64(610), INT32(65));
#line 610 "../lib/Chai.chpl"
  }
#line 588 "../lib/Chai.chpl"
  tmp_chpl13 = (_ic__F7_i_chpl <= _ic__F8_end_chpl);
#line 588 "../lib/Chai.chpl"
  hasMore_chpl2 = (tmp_chpl13 != INT64(0));
#line 588 "../lib/Chai.chpl"
  if (hasMore_chpl2) /* ZLINE: 588 ../lib/Chai.chpl */
#line 588 "../lib/Chai.chpl"
  {
#line 588 "../lib/Chai.chpl"
    chpl_error("zippered iterations have non-equal lengths", INT64(588), INT32(65));
#line 588 "../lib/Chai.chpl"
  }
#line 588 "../lib/Chai.chpl"
  tmp_chpl14 = (_ic__F8_i_chpl2 <= _ic__F9_end_chpl2);
#line 588 "../lib/Chai.chpl"
  hasMore_chpl = (tmp_chpl14 != INT64(0));
#line 588 "../lib/Chai.chpl"
  if (hasMore_chpl) /* ZLINE: 588 ../lib/Chai.chpl */
#line 588 "../lib/Chai.chpl"
  {
#line 588 "../lib/Chai.chpl"
    chpl_error("zippered iterations have non-equal lengths", INT64(588), INT32(65));
#line 588 "../lib/Chai.chpl"
  }
#line 588 "../lib/Chai.chpl"
  coerce_tmp_chpl53 = RP_biasesGrad_chpl2;
#line 588 "../lib/Chai.chpl"
  i_x_chpl = &AS_biasesGrad_chpl;
#line 588 "../lib/Chai.chpl"
  chpl_check_nil(coerce_tmp_chpl53, INT64(588), INT32(65));
#line 588 "../lib/Chai.chpl"
  call_tmp_chpl75 = &((coerce_tmp_chpl53)->value);
#line 588 "../lib/Chai.chpl"
  chpl___PLUS__ASSIGN_4(call_tmp_chpl75, i_x_chpl);
#line 588 "../lib/Chai.chpl"
  _field_destructor_tmp__chpl37 = (&AS_biasesGrad_chpl)->data;
#line 588 "../lib/Chai.chpl"
  chpl__autoDestroy18(&_field_destructor_tmp__chpl37, INT64(588), INT32(65));
#line 588 "../lib/Chai.chpl"
  _field_destructor_tmp__chpl38 = (&AS_biasesGrad_chpl)->_domain;
#line 588 "../lib/Chai.chpl"
  chpl__autoDestroy11(&_field_destructor_tmp__chpl38, INT64(588), INT32(65));
#line 588 "../lib/Chai.chpl"
  chpl__reduceCombine(RP_biasesGrad_chpl, RP_biasesGrad_chpl2, INT64(588), INT32(65));
#line 588 "../lib/Chai.chpl"
  chpl__delete25(RP_biasesGrad_chpl2, INT64(588), INT32(65));
#line 588 "../lib/Chai.chpl"
  coerce_tmp_chpl54 = RP_weightsGrad_chpl2;
#line 588 "../lib/Chai.chpl"
  i_x_chpl2 = &AS_weightsGrad_chpl;
#line 588 "../lib/Chai.chpl"
  chpl_check_nil(coerce_tmp_chpl54, INT64(588), INT32(65));
#line 588 "../lib/Chai.chpl"
  call_tmp_chpl76 = &((coerce_tmp_chpl54)->value);
#line 588 "../lib/Chai.chpl"
  chpl___PLUS__ASSIGN_2(call_tmp_chpl76, i_x_chpl2);
#line 588 "../lib/Chai.chpl"
  _field_destructor_tmp__chpl39 = (&AS_weightsGrad_chpl)->data;
#line 588 "../lib/Chai.chpl"
  chpl__autoDestroy19(&_field_destructor_tmp__chpl39, INT64(588), INT32(65));
#line 588 "../lib/Chai.chpl"
  _field_destructor_tmp__chpl40 = (&AS_weightsGrad_chpl)->_domain;
#line 588 "../lib/Chai.chpl"
  chpl__autoDestroy5(&_field_destructor_tmp__chpl40, INT64(588), INT32(65));
#line 588 "../lib/Chai.chpl"
  chpl__reduceCombine2(RP_weightsGrad_chpl, RP_weightsGrad_chpl2, INT64(588), INT32(65));
#line 588 "../lib/Chai.chpl"
  chpl__delete64(RP_weightsGrad_chpl2, INT64(588), INT32(65));
#line 588 "../lib/Chai.chpl"
  return;
#line 588 "../lib/Chai.chpl"
}

#line 617 "../lib/Chai.chpl"
/* Chai.chpl:617 */
#line 617 "../lib/Chai.chpl"
static void optimize_chpl3(SoftMax_chpl * this_chpl31,
#line 617 "../lib/Chai.chpl"
                           _real64 mag_chpl) {
#line 618 "../lib/Chai.chpl"
  int64_t local_dataParMinGranularity_chpl;
#line 618 "../lib/Chai.chpl"
  chpl_bool local_dataParIgnoreRunningTasks_chpl;
#line 618 "../lib/Chai.chpl"
  int64_t local_dataParTasksPerLocale_chpl;
#line 618 "../lib/Chai.chpl"
  _ref_Tensor_2__real64_chpl call_tmp_chpl6 = NULL;
#line 618 "../lib/Chai.chpl"
  _ref__array_DefaultRectangularArr_2_int64_t_one__real64_int64_t_chpl call_tmp_chpl7 = NULL;
#line 618 "../lib/Chai.chpl"
  _ref_Tensor_2__real64_chpl call_tmp_chpl8 = NULL;
#line 618 "../lib/Chai.chpl"
  _ref__array_DefaultRectangularArr_2_int64_t_one__real64_int64_t_chpl call_tmp_chpl9 = NULL;
#line 618 "../lib/Chai.chpl"
  _ir_chpl_promo15__ASTERISK__chpl call_tmp_chpl10;
#line 618 "../lib/Chai.chpl"
  _ir_chpl_promo15__ASTERISK__chpl ret_tmp_chpl;
#line 618 "../lib/Chai.chpl"
  DefaultRectangularDom_2_int64_t_one_chpl _ir__shape__chpl = NULL;
#line 618 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_2_int64_t_one__real64_int64_t_chpl _ir_F1_b_chpl;
#line 618 "../lib/Chai.chpl"
  _domain_DefaultRectangularDom_2_int64_t_one_chpl call_tmp_chpl11;
#line 618 "../lib/Chai.chpl"
  _domain_DefaultRectangularDom_2_int64_t_one_chpl ret_tmp_chpl2;
#line 618 "../lib/Chai.chpl"
  DefaultRectangularDom_2_int64_t_one_chpl ret_chpl = NULL;
#line 618 "../lib/Chai.chpl"
  _ir_chpl_promo16__HYPHEN__chpl call_tmp_chpl12;
#line 618 "../lib/Chai.chpl"
  DefaultRectangularDom_2_int64_t_one_chpl _ir__shape__chpl2 = NULL;
#line 618 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_2_int64_t_one__real64_int64_t_chpl _ir_F0_a_chpl;
#line 618 "../lib/Chai.chpl"
  _ref__ir_chpl_promo15__ASTERISK__chpl _ir_F1_b_chpl2 = NULL;
#line 618 "../lib/Chai.chpl"
  _domain_DefaultRectangularDom_2_int64_t_one_chpl call_tmp_chpl13;
#line 618 "../lib/Chai.chpl"
  _domain_DefaultRectangularDom_2_int64_t_one_chpl ret_tmp_chpl3;
#line 618 "../lib/Chai.chpl"
  DefaultRectangularDom_2_int64_t_one_chpl ret_chpl2 = NULL;
#line 618 "../lib/Chai.chpl"
  DefaultRectangularArr_2_int64_t_one__real64_int64_t_chpl coerce_tmp_chpl3 = NULL;
#line 618 "../lib/Chai.chpl"
  int64_t tasksPerLocale_chpl;
#line 618 "../lib/Chai.chpl"
  chpl_bool ignoreRunning_chpl;
#line 618 "../lib/Chai.chpl"
  int64_t minIndicesPerTask_chpl;
#line 618 "../lib/Chai.chpl"
  Error_chpl error_chpl = NULL;
#line 618 "../lib/Chai.chpl"
  DefaultRectangularDom_2_int64_t_one_chpl coerce_tmp_chpl4 = NULL;
#line 618 "../lib/Chai.chpl"
  int64_t offset_x0_chpl;
#line 618 "../lib/Chai.chpl"
  int64_t offset_x1_chpl;
#line 618 "../lib/Chai.chpl"
  _tuple_2_star_int64_t_chpl ret_tmp_chpl4;
#line 618 "../lib/Chai.chpl"
  Error_chpl error_chpl2 = NULL;
#line 618 "../lib/Chai.chpl"
  locale_chpl call_tmp_chpl14;
#line 618 "../lib/Chai.chpl"
  chpl_localeID_t call_tmp_chpl15;
#line 618 "../lib/Chai.chpl"
  int32_t call_tmp_chpl16;
#line 618 "../lib/Chai.chpl"
  chpl_localeID_t call_tmp_chpl17;
#line 618 "../lib/Chai.chpl"
  locale_chpl call_tmp_chpl18;
#line 618 "../lib/Chai.chpl"
  locale_chpl ret_tmp_chpl5;
#line 618 "../lib/Chai.chpl"
  BaseLocale_chpl coerce_tmp_chpl5 = NULL;
#line 618 "../lib/Chai.chpl"
  _ref_locale_chpl i_x_chpl = NULL;
#line 618 "../lib/Chai.chpl"
  int64_t tmp_chpl;
#line 618 "../lib/Chai.chpl"
  locale_chpl call_tmp_chpl19;
#line 618 "../lib/Chai.chpl"
  chpl_localeID_t call_tmp_chpl20;
#line 618 "../lib/Chai.chpl"
  int32_t call_tmp_chpl21;
#line 618 "../lib/Chai.chpl"
  chpl_localeID_t call_tmp_chpl22;
#line 618 "../lib/Chai.chpl"
  locale_chpl call_tmp_chpl23;
#line 618 "../lib/Chai.chpl"
  locale_chpl ret_tmp_chpl6;
#line 618 "../lib/Chai.chpl"
  int64_t ret_chpl3;
#line 618 "../lib/Chai.chpl"
  BaseLocale_chpl coerce_tmp_chpl6 = NULL;
#line 618 "../lib/Chai.chpl"
  BaseLocale_chpl coerce_tmp_chpl7 = NULL;
#line 618 "../lib/Chai.chpl"
  _ref_locale_chpl i_x_chpl2 = NULL;
#line 618 "../lib/Chai.chpl"
  int64_t tmp_x0_chpl;
#line 618 "../lib/Chai.chpl"
  int64_t tmp_x1_chpl;
#line 618 "../lib/Chai.chpl"
  _ref__tuple_2_star_range_int64_t_both_one_chpl call_tmp_chpl24 = NULL;
#line 618 "../lib/Chai.chpl"
  _tuple_2_star_int64_t_chpl ret_tmp_chpl7;
#line 618 "../lib/Chai.chpl"
  int64_t numChunks_chpl;
#line 618 "../lib/Chai.chpl"
  int64_t parDim_chpl;
#line 618 "../lib/Chai.chpl"
  _tuple_2_star_range_int64_t_both_one_chpl locBlock_chpl;
#line 618 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl tup_x0_chpl;
#line 618 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl tup_x1_chpl;
#line 618 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl elt_x0_chpl;
#line 618 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl elt_x1_chpl;
#line 618 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl call_tmp_chpl25;
#line 618 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl ret_tmp_chpl8;
#line 618 "../lib/Chai.chpl"
  _ref__tuple_2_star_range_int64_t_both_one_chpl call_tmp_chpl26 = NULL;
#line 618 "../lib/Chai.chpl"
  int64_t call_tmp_chpl27;
#line 618 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl8;
#line 618 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl28;
#line 618 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl9;
#line 618 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl call_tmp_chpl29;
#line 618 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl ret_tmp_chpl10;
#line 618 "../lib/Chai.chpl"
  _ref__tuple_2_star_range_int64_t_both_one_chpl call_tmp_chpl30 = NULL;
#line 618 "../lib/Chai.chpl"
  int64_t call_tmp_chpl31;
#line 618 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl9;
#line 618 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl32;
#line 618 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl11;
#line 618 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl tmpIter_chpl;
#line 618 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl call_tmp_chpl33;
#line 618 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl ret_tmp_chpl12;
#line 618 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl13;
#line 618 "../lib/Chai.chpl"
  chpl___EndCount_AtomicT_int64_t_int64_t _coforallCount_chpl = NULL;
#line 618 "../lib/Chai.chpl"
  chpl___EndCount_AtomicT_int64_t_int64_t new_temp_chpl = NULL;
#line 618 "../lib/Chai.chpl"
  int64_t numTasks_chpl;
#line 618 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl _ic__F0_this_chpl;
#line 618 "../lib/Chai.chpl"
  int64_t i_chpl;
#line 618 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl this_chpl32;
#line 618 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl this_chpl33;
#line 618 "../lib/Chai.chpl"
  int64_t end_chpl;
#line 618 "../lib/Chai.chpl"
  _class_localscoforall_fn_chpl21 _args_forcoforall_fn_chpl = NULL;
#line 618 "../lib/Chai.chpl"
  _ref__ir_chpl_promo16__HYPHEN__chpl tmp_chpl2 = NULL;
#line 618 "../lib/Chai.chpl"
  raw_c_void_ptr _args_vforcoforall_fn_chpl;
#line 618 "../lib/Chai.chpl"
  int64_t _args_sizecoforall_fn_chpl;
#line 618 "../lib/Chai.chpl"
  Error_chpl error_chpl3 = NULL;
#line 619 "../lib/Chai.chpl"
  _ref_Tensor_1__real64_chpl call_tmp_chpl34 = NULL;
#line 619 "../lib/Chai.chpl"
  _ref__array_DefaultRectangularArr_1_int64_t_one__real64_int64_t_chpl call_tmp_chpl35 = NULL;
#line 619 "../lib/Chai.chpl"
  _ref_Tensor_1__real64_chpl call_tmp_chpl36 = NULL;
#line 619 "../lib/Chai.chpl"
  _ref__array_DefaultRectangularArr_1_int64_t_one__real64_int64_t_chpl call_tmp_chpl37 = NULL;
#line 619 "../lib/Chai.chpl"
  _ir_chpl_promo10__ASTERISK__chpl call_tmp_chpl38;
#line 619 "../lib/Chai.chpl"
  _ir_chpl_promo10__ASTERISK__chpl ret_tmp_chpl14;
#line 619 "../lib/Chai.chpl"
  DefaultRectangularDom_1_int64_t_one_chpl _ir__shape__chpl3 = NULL;
#line 619 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_1_int64_t_one__real64_int64_t_chpl _ir_F1_b_chpl3;
#line 619 "../lib/Chai.chpl"
  _domain_DefaultRectangularDom_1_int64_t_one_chpl call_tmp_chpl39;
#line 619 "../lib/Chai.chpl"
  _domain_DefaultRectangularDom_1_int64_t_one_chpl ret_tmp_chpl15;
#line 619 "../lib/Chai.chpl"
  DefaultRectangularDom_1_int64_t_one_chpl ret_chpl4 = NULL;
#line 619 "../lib/Chai.chpl"
  _ir_chpl_promo17__HYPHEN__chpl call_tmp_chpl40;
#line 619 "../lib/Chai.chpl"
  DefaultRectangularDom_1_int64_t_one_chpl _ir__shape__chpl4 = NULL;
#line 619 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_1_int64_t_one__real64_int64_t_chpl _ir_F0_a_chpl2;
#line 619 "../lib/Chai.chpl"
  _ref__ir_chpl_promo10__ASTERISK__chpl _ir_F1_b_chpl4 = NULL;
#line 619 "../lib/Chai.chpl"
  _domain_DefaultRectangularDom_1_int64_t_one_chpl call_tmp_chpl41;
#line 619 "../lib/Chai.chpl"
  _domain_DefaultRectangularDom_1_int64_t_one_chpl ret_tmp_chpl16;
#line 619 "../lib/Chai.chpl"
  DefaultRectangularDom_1_int64_t_one_chpl ret_chpl5 = NULL;
#line 619 "../lib/Chai.chpl"
  DefaultRectangularArr_1_int64_t_one__real64_int64_t_chpl coerce_tmp_chpl10 = NULL;
#line 619 "../lib/Chai.chpl"
  int64_t tasksPerLocale_chpl2;
#line 619 "../lib/Chai.chpl"
  chpl_bool ignoreRunning_chpl2;
#line 619 "../lib/Chai.chpl"
  int64_t minIndicesPerTask_chpl2;
#line 619 "../lib/Chai.chpl"
  Error_chpl error_chpl4 = NULL;
#line 619 "../lib/Chai.chpl"
  DefaultRectangularDom_1_int64_t_one_chpl coerce_tmp_chpl11 = NULL;
#line 619 "../lib/Chai.chpl"
  int64_t offset_x0_chpl2;
#line 619 "../lib/Chai.chpl"
  _tuple_1_star_int64_t_chpl ret_tmp_chpl17;
#line 619 "../lib/Chai.chpl"
  Error_chpl error_chpl5 = NULL;
#line 619 "../lib/Chai.chpl"
  locale_chpl call_tmp_chpl42;
#line 619 "../lib/Chai.chpl"
  chpl_localeID_t call_tmp_chpl43;
#line 619 "../lib/Chai.chpl"
  int32_t call_tmp_chpl44;
#line 619 "../lib/Chai.chpl"
  chpl_localeID_t call_tmp_chpl45;
#line 619 "../lib/Chai.chpl"
  locale_chpl call_tmp_chpl46;
#line 619 "../lib/Chai.chpl"
  locale_chpl ret_tmp_chpl18;
#line 619 "../lib/Chai.chpl"
  BaseLocale_chpl coerce_tmp_chpl12 = NULL;
#line 619 "../lib/Chai.chpl"
  _ref_locale_chpl i_x_chpl3 = NULL;
#line 619 "../lib/Chai.chpl"
  int64_t tmp_chpl3;
#line 619 "../lib/Chai.chpl"
  locale_chpl call_tmp_chpl47;
#line 619 "../lib/Chai.chpl"
  chpl_localeID_t call_tmp_chpl48;
#line 619 "../lib/Chai.chpl"
  int32_t call_tmp_chpl49;
#line 619 "../lib/Chai.chpl"
  chpl_localeID_t call_tmp_chpl50;
#line 619 "../lib/Chai.chpl"
  locale_chpl call_tmp_chpl51;
#line 619 "../lib/Chai.chpl"
  locale_chpl ret_tmp_chpl19;
#line 619 "../lib/Chai.chpl"
  int64_t ret_chpl6;
#line 619 "../lib/Chai.chpl"
  BaseLocale_chpl coerce_tmp_chpl13 = NULL;
#line 619 "../lib/Chai.chpl"
  BaseLocale_chpl coerce_tmp_chpl14 = NULL;
#line 619 "../lib/Chai.chpl"
  _ref_locale_chpl i_x_chpl4 = NULL;
#line 619 "../lib/Chai.chpl"
  int64_t tmp_x0_chpl2;
#line 619 "../lib/Chai.chpl"
  int64_t tmp_x1_chpl2;
#line 619 "../lib/Chai.chpl"
  _ref__tuple_1_star_range_int64_t_both_one_chpl call_tmp_chpl52 = NULL;
#line 619 "../lib/Chai.chpl"
  _tuple_2_star_int64_t_chpl ret_tmp_chpl20;
#line 619 "../lib/Chai.chpl"
  int64_t numChunks_chpl2;
#line 619 "../lib/Chai.chpl"
  int64_t parDim_chpl2;
#line 619 "../lib/Chai.chpl"
  _tuple_1_star_range_int64_t_both_one_chpl locBlock_chpl2;
#line 619 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl tup_x0_chpl2;
#line 619 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl elt_x0_chpl2;
#line 619 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl call_tmp_chpl53;
#line 619 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl ret_tmp_chpl21;
#line 619 "../lib/Chai.chpl"
  _ref__tuple_1_star_range_int64_t_both_one_chpl call_tmp_chpl54 = NULL;
#line 619 "../lib/Chai.chpl"
  int64_t call_tmp_chpl55;
#line 619 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl15;
#line 619 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl56;
#line 619 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl22;
#line 619 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl tmpIter_chpl2;
#line 619 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl call_tmp_chpl57;
#line 619 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl ret_tmp_chpl23;
#line 619 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl24;
#line 619 "../lib/Chai.chpl"
  chpl___EndCount_AtomicT_int64_t_int64_t _coforallCount_chpl2 = NULL;
#line 619 "../lib/Chai.chpl"
  chpl___EndCount_AtomicT_int64_t_int64_t new_temp_chpl2 = NULL;
#line 619 "../lib/Chai.chpl"
  int64_t numTasks_chpl2;
#line 619 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl _ic__F0_this_chpl2;
#line 619 "../lib/Chai.chpl"
  int64_t i_chpl2;
#line 619 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl this_chpl34;
#line 619 "../lib/Chai.chpl"
  range_int64_t_both_one_chpl this_chpl35;
#line 619 "../lib/Chai.chpl"
  int64_t end_chpl2;
#line 619 "../lib/Chai.chpl"
  _class_localscoforall_fn_chpl17 _args_forcoforall_fn_chpl2 = NULL;
#line 619 "../lib/Chai.chpl"
  _ref__ir_chpl_promo17__HYPHEN__chpl tmp_chpl4 = NULL;
#line 619 "../lib/Chai.chpl"
  raw_c_void_ptr _args_vforcoforall_fn_chpl2;
#line 619 "../lib/Chai.chpl"
  int64_t _args_sizecoforall_fn_chpl2;
#line 619 "../lib/Chai.chpl"
  Error_chpl error_chpl6 = NULL;
#line 619 "../lib/Chai.chpl"
  chpl__class_localscoforall_fn_chpl21_object chpl_macro_tmp_121;
#line 619 "../lib/Chai.chpl"
  chpl__class_localscoforall_fn_chpl17_object chpl_macro_tmp_122;
#line 618 "../lib/Chai.chpl"
  local_dataParMinGranularity_chpl = dataParMinGranularity_chpl;
#line 618 "../lib/Chai.chpl"
  local_dataParIgnoreRunningTasks_chpl = dataParIgnoreRunningTasks_chpl;
#line 618 "../lib/Chai.chpl"
  local_dataParTasksPerLocale_chpl = dataParTasksPerLocale_chpl;
#line 618 "../lib/Chai.chpl"
  call_tmp_chpl6 = &((this_chpl31)->weights);
#line 618 "../lib/Chai.chpl"
  call_tmp_chpl7 = &((call_tmp_chpl6)->data);
#line 618 "../lib/Chai.chpl"
  call_tmp_chpl8 = &((this_chpl31)->weightsGrad);
#line 618 "../lib/Chai.chpl"
  call_tmp_chpl9 = &((call_tmp_chpl8)->data);
#line 618 "../lib/Chai.chpl"
  _ir_F1_b_chpl = (call_tmp_chpl8)->data;
#line 618 "../lib/Chai.chpl"
  (&ret_tmp_chpl)->_shape_ = _ir__shape__chpl;
#line 618 "../lib/Chai.chpl"
  (&ret_tmp_chpl)->F0_a = mag_chpl;
#line 618 "../lib/Chai.chpl"
  (&ret_tmp_chpl)->F1_b = _ir_F1_b_chpl;
#line 618 "../lib/Chai.chpl"
  call_tmp_chpl10 = ret_tmp_chpl;
#line 618 "../lib/Chai.chpl"
  _dom_chpl14(call_tmp_chpl9, &ret_tmp_chpl2, INT64(618), INT32(65));
#line 618 "../lib/Chai.chpl"
  call_tmp_chpl11 = ret_tmp_chpl2;
#line 618 "../lib/Chai.chpl"
  ret_chpl = (&call_tmp_chpl11)->_instance;
#line 618 "../lib/Chai.chpl"
  chpl__autoDestroy5(&call_tmp_chpl11, INT64(618), INT32(65));
#line 618 "../lib/Chai.chpl"
  (&call_tmp_chpl10)->_shape_ = ret_chpl;
#line 618 "../lib/Chai.chpl"
  _ir_F0_a_chpl = (call_tmp_chpl6)->data;
#line 618 "../lib/Chai.chpl"
  _ir_F1_b_chpl2 = &call_tmp_chpl10;
#line 618 "../lib/Chai.chpl"
  (&call_tmp_chpl12)->_shape_ = _ir__shape__chpl2;
#line 618 "../lib/Chai.chpl"
  (&call_tmp_chpl12)->F0_a = _ir_F0_a_chpl;
#line 618 "../lib/Chai.chpl"
  (&call_tmp_chpl12)->F1_b = _ir_F1_b_chpl2;
#line 618 "../lib/Chai.chpl"
  _dom_chpl14(call_tmp_chpl7, &ret_tmp_chpl3, INT64(618), INT32(65));
#line 618 "../lib/Chai.chpl"
  call_tmp_chpl13 = ret_tmp_chpl3;
#line 618 "../lib/Chai.chpl"
  ret_chpl2 = (&call_tmp_chpl13)->_instance;
#line 618 "../lib/Chai.chpl"
  chpl__autoDestroy5(&call_tmp_chpl13, INT64(618), INT32(65));
#line 618 "../lib/Chai.chpl"
  (&call_tmp_chpl12)->_shape_ = ret_chpl2;
#line 618 "../lib/Chai.chpl"
  coerce_tmp_chpl3 = (call_tmp_chpl7)->_instance;
#line 618 "../lib/Chai.chpl"
  tasksPerLocale_chpl = local_dataParTasksPerLocale_chpl;
#line 618 "../lib/Chai.chpl"
  ignoreRunning_chpl = local_dataParIgnoreRunningTasks_chpl;
#line 618 "../lib/Chai.chpl"
  minIndicesPerTask_chpl = local_dataParMinGranularity_chpl;
#line 618 "../lib/Chai.chpl"
  error_chpl = NULL;
#line 618 "../lib/Chai.chpl"
  chpl_check_nil(coerce_tmp_chpl3, INT64(618), INT32(65));
#line 618 "../lib/Chai.chpl"
  coerce_tmp_chpl4 = (coerce_tmp_chpl3)->dom;
#line 618 "../lib/Chai.chpl"
  createTuple_chpl4(INT64(0), &ret_tmp_chpl4);
#line 618 "../lib/Chai.chpl"
  offset_x0_chpl = *(ret_tmp_chpl4 + INT64(0));
#line 618 "../lib/Chai.chpl"
  offset_x1_chpl = *(ret_tmp_chpl4 + INT64(1));
#line 618 "../lib/Chai.chpl"
  error_chpl2 = NULL;
#line 618 "../lib/Chai.chpl"
  call_tmp_chpl16 = get_chpl_nodeID();
#line 618 "../lib/Chai.chpl"
  call_tmp_chpl17 = chpl_rt_buildLocaleID(call_tmp_chpl16, c_sublocid_any);
#line 618 "../lib/Chai.chpl"
  call_tmp_chpl15 = call_tmp_chpl17;
#line 618 "../lib/Chai.chpl"
  chpl_localeID_to_locale(&call_tmp_chpl15, &ret_tmp_chpl5, INT64(618), INT32(65));
#line 618 "../lib/Chai.chpl"
  call_tmp_chpl18 = ret_tmp_chpl5;
#line 618 "../lib/Chai.chpl"
  call_tmp_chpl14 = call_tmp_chpl18;
#line 618 "../lib/Chai.chpl"
  coerce_tmp_chpl5 = (&call_tmp_chpl14)->_instance;
#line 618 "../lib/Chai.chpl"
  if (((RootClass_chpl)(coerce_tmp_chpl5)) == nil) /* ZLINE: 618 ../lib/Chai.chpl */
#line 618 "../lib/Chai.chpl"
  {
#line 618 "../lib/Chai.chpl"
    nilCheckHalt_chpl(&_str_literal_52_chpl, INT64(618), INT32(65));
#line 618 "../lib/Chai.chpl"
  }
#line 618 "../lib/Chai.chpl"
  chpl_check_nil(coerce_tmp_chpl5, INT64(618), INT32(65));
#line 618 "../lib/Chai.chpl"
  ((int64_t(*)(BaseLocale_chpl,
#line 618 "../lib/Chai.chpl"
     int64_t,
#line 618 "../lib/Chai.chpl"
     int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(coerce_tmp_chpl5))->chpl__cid) + INT64(2))])(coerce_tmp_chpl5, INT64(618), INT32(65));
#line 618 "../lib/Chai.chpl"
  i_x_chpl = &call_tmp_chpl14;
#line 618 "../lib/Chai.chpl"
  deinit_chpl62(i_x_chpl);
#line 618 "../lib/Chai.chpl"
  if (tasksPerLocale_chpl == INT64(0)) /* ZLINE: 618 ../lib/Chai.chpl */
#line 618 "../lib/Chai.chpl"
  {
#line 618 "../lib/Chai.chpl"
    call_tmp_chpl21 = get_chpl_nodeID();
#line 618 "../lib/Chai.chpl"
    call_tmp_chpl22 = chpl_rt_buildLocaleID(call_tmp_chpl21, c_sublocid_any);
#line 618 "../lib/Chai.chpl"
    call_tmp_chpl20 = call_tmp_chpl22;
#line 618 "../lib/Chai.chpl"
    chpl_localeID_to_locale(&call_tmp_chpl20, &ret_tmp_chpl6, INT64(618), INT32(65));
#line 618 "../lib/Chai.chpl"
    call_tmp_chpl23 = ret_tmp_chpl6;
#line 618 "../lib/Chai.chpl"
    call_tmp_chpl19 = call_tmp_chpl23;
#line 618 "../lib/Chai.chpl"
    coerce_tmp_chpl6 = (&call_tmp_chpl19)->_instance;
#line 618 "../lib/Chai.chpl"
    if (((RootClass_chpl)(coerce_tmp_chpl6)) == nil) /* ZLINE: 618 ../lib/Chai.chpl */
#line 618 "../lib/Chai.chpl"
    {
#line 618 "../lib/Chai.chpl"
      nilCheckHalt_chpl(&_str_literal_52_chpl, INT64(618), INT32(65));
#line 618 "../lib/Chai.chpl"
    }
#line 618 "../lib/Chai.chpl"
    coerce_tmp_chpl7 = coerce_tmp_chpl6;
#line 618 "../lib/Chai.chpl"
    chpl_check_nil(coerce_tmp_chpl7, INT64(618), INT32(65));
#line 618 "../lib/Chai.chpl"
    ret_chpl3 = (coerce_tmp_chpl7)->maxTaskPar;
#line 618 "../lib/Chai.chpl"
    tmp_chpl = ret_chpl3;
#line 618 "../lib/Chai.chpl"
    i_x_chpl2 = &call_tmp_chpl19;
#line 618 "../lib/Chai.chpl"
    deinit_chpl62(i_x_chpl2);
#line 618 "../lib/Chai.chpl"
  } else /* ZLINE: 618 ../lib/Chai.chpl */
#line 618 "../lib/Chai.chpl"
  {
#line 618 "../lib/Chai.chpl"
    tmp_chpl = tasksPerLocale_chpl;
#line 618 "../lib/Chai.chpl"
  }
#line 618 "../lib/Chai.chpl"
  if (chpl_task_getSerial()) /* ZLINE: 618 ../lib/Chai.chpl */
#line 618 "../lib/Chai.chpl"
  {
#line 618 "../lib/Chai.chpl"
    tmp_x0_chpl = INT64(1);
#line 618 "../lib/Chai.chpl"
    tmp_x1_chpl = INT64(0);
#line 618 "../lib/Chai.chpl"
  } else /* ZLINE: 618 ../lib/Chai.chpl */
#line 618 "../lib/Chai.chpl"
  {
#line 618 "../lib/Chai.chpl"
    chpl_check_nil(coerce_tmp_chpl4, INT64(618), INT32(65));
#line 618 "../lib/Chai.chpl"
    call_tmp_chpl24 = &((coerce_tmp_chpl4)->ranges);
#line 618 "../lib/Chai.chpl"
    _computeChunkStuff_chpl4(tmp_chpl, ignoreRunning_chpl, minIndicesPerTask_chpl, call_tmp_chpl24, &ret_tmp_chpl7, INT64(618), INT32(65));
#line 618 "../lib/Chai.chpl"
    tmp_x0_chpl = *(ret_tmp_chpl7 + INT64(0));
#line 618 "../lib/Chai.chpl"
    tmp_x1_chpl = *(ret_tmp_chpl7 + INT64(1));
#line 618 "../lib/Chai.chpl"
  }
#line 618 "../lib/Chai.chpl"
  numChunks_chpl = tmp_x0_chpl;
#line 618 "../lib/Chai.chpl"
  parDim_chpl = tmp_x1_chpl;
#line 618 "../lib/Chai.chpl"
  init_chpl113(&elt_x0_chpl);
#line 618 "../lib/Chai.chpl"
  tup_x0_chpl = elt_x0_chpl;
#line 618 "../lib/Chai.chpl"
  init_chpl113(&elt_x1_chpl);
#line 618 "../lib/Chai.chpl"
  tup_x1_chpl = elt_x1_chpl;
#line 618 "../lib/Chai.chpl"
  *(locBlock_chpl + INT64(0)) = tup_x0_chpl;
#line 618 "../lib/Chai.chpl"
  *(locBlock_chpl + INT64(1)) = tup_x1_chpl;
#line 618 "../lib/Chai.chpl"
  chpl_build_low_bounded_range2(offset_x0_chpl, &ret_tmp_chpl8);
#line 618 "../lib/Chai.chpl"
  call_tmp_chpl25 = ret_tmp_chpl8;
#line 618 "../lib/Chai.chpl"
  chpl_check_nil(coerce_tmp_chpl4, INT64(618), INT32(65));
#line 618 "../lib/Chai.chpl"
  call_tmp_chpl26 = &((coerce_tmp_chpl4)->ranges);
#line 618 "../lib/Chai.chpl"
  coerce_tmp_chpl8 = *(*(call_tmp_chpl26) + INT64(0));
#line 618 "../lib/Chai.chpl"
  call_tmp_chpl27 = sizeAs_chpl37(coerce_tmp_chpl8, INT64(618), INT32(65));
#line 618 "../lib/Chai.chpl"
  chpl___POUND_3(call_tmp_chpl25, call_tmp_chpl27, &ret_tmp_chpl9, INT64(618), INT32(65));
#line 618 "../lib/Chai.chpl"
  call_tmp_chpl28 = ret_tmp_chpl9;
#line 618 "../lib/Chai.chpl"
  *(locBlock_chpl + INT64(0)) = call_tmp_chpl28;
#line 618 "../lib/Chai.chpl"
  chpl_build_low_bounded_range2(offset_x1_chpl, &ret_tmp_chpl10);
#line 618 "../lib/Chai.chpl"
  call_tmp_chpl29 = ret_tmp_chpl10;
#line 618 "../lib/Chai.chpl"
  chpl_check_nil(coerce_tmp_chpl4, INT64(618), INT32(65));
#line 618 "../lib/Chai.chpl"
  call_tmp_chpl30 = &((coerce_tmp_chpl4)->ranges);
#line 618 "../lib/Chai.chpl"
  coerce_tmp_chpl9 = *(*(call_tmp_chpl30) + INT64(1));
#line 618 "../lib/Chai.chpl"
  call_tmp_chpl31 = sizeAs_chpl37(coerce_tmp_chpl9, INT64(618), INT32(65));
#line 618 "../lib/Chai.chpl"
  chpl___POUND_3(call_tmp_chpl29, call_tmp_chpl31, &ret_tmp_chpl11, INT64(618), INT32(65));
#line 618 "../lib/Chai.chpl"
  call_tmp_chpl32 = ret_tmp_chpl11;
#line 618 "../lib/Chai.chpl"
  *(locBlock_chpl + INT64(1)) = call_tmp_chpl32;
#line 618 "../lib/Chai.chpl"
  chpl_build_low_bounded_range2(INT64(0), &ret_tmp_chpl12);
#line 618 "../lib/Chai.chpl"
  call_tmp_chpl33 = ret_tmp_chpl12;
#line 618 "../lib/Chai.chpl"
  chpl___POUND_3(call_tmp_chpl33, tmp_x0_chpl, &ret_tmp_chpl13, INT64(618), INT32(65));
#line 618 "../lib/Chai.chpl"
  tmpIter_chpl = ret_tmp_chpl13;
#line 618 "../lib/Chai.chpl"
  new_temp_chpl = _new_chpl17(INT64(618), INT32(65));
#line 618 "../lib/Chai.chpl"
  _coforallCount_chpl = new_temp_chpl;
#line 618 "../lib/Chai.chpl"
  numTasks_chpl = chpl_boundedCoforallSize(tmpIter_chpl, INT64(618), INT32(65));
#line 618 "../lib/Chai.chpl"
  _upEndCount_chpl(_coforallCount_chpl, numTasks_chpl, INT64(618), INT32(65));
#line 618 "../lib/Chai.chpl"
  chpl_resetTaskSpawn(numTasks_chpl, INT64(618), INT32(65));
#line 618 "../lib/Chai.chpl"
  _ic__F0_this_chpl = tmpIter_chpl;
#line 618 "../lib/Chai.chpl"
  checkIfIterWillOverflow_chpl(tmpIter_chpl, UINT8(true), INT64(618), INT32(65));
#line 618 "../lib/Chai.chpl"
  i_chpl = INT64(0);
#line 618 "../lib/Chai.chpl"
  this_chpl32 = _ic__F0_this_chpl;
#line 618 "../lib/Chai.chpl"
  this_chpl33 = _ic__F0_this_chpl;
#line 618 "../lib/Chai.chpl"
  end_chpl = (&this_chpl33)->_high;
#line 618 "../lib/Chai.chpl"
  for (i_chpl = (&this_chpl32)->_low; ((i_chpl <= end_chpl)); i_chpl += INT64(1)) {
#line 618 "../lib/Chai.chpl"
    _args_forcoforall_fn_chpl = ((_class_localscoforall_fn_chpl21)(&chpl_macro_tmp_121));
#line 618 "../lib/Chai.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl, INT64(618), INT32(65));
#line 618 "../lib/Chai.chpl"
    (_args_forcoforall_fn_chpl)->_0_numChunks = numChunks_chpl;
#line 618 "../lib/Chai.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl, INT64(618), INT32(65));
#line 618 "../lib/Chai.chpl"
    (_args_forcoforall_fn_chpl)->_1_parDim = parDim_chpl;
#line 618 "../lib/Chai.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl, INT64(618), INT32(65));
#line 618 "../lib/Chai.chpl"
    *((_args_forcoforall_fn_chpl)->_2_coerce_tmp + INT64(0)) = *(locBlock_chpl + INT64(0));
#line 618 "../lib/Chai.chpl"
    *((_args_forcoforall_fn_chpl)->_2_coerce_tmp + INT64(1)) = *(locBlock_chpl + INT64(1));
#line 618 "../lib/Chai.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl, INT64(618), INT32(65));
#line 618 "../lib/Chai.chpl"
    (_args_forcoforall_fn_chpl)->_3__coforallCount = _coforallCount_chpl;
#line 618 "../lib/Chai.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl, INT64(618), INT32(65));
#line 618 "../lib/Chai.chpl"
    (_args_forcoforall_fn_chpl)->_4_chunk = i_chpl;
#line 618 "../lib/Chai.chpl"
    tmp_chpl2 = &call_tmp_chpl12;
#line 618 "../lib/Chai.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl, INT64(618), INT32(65));
#line 618 "../lib/Chai.chpl"
    (_args_forcoforall_fn_chpl)->_6_b = tmp_chpl2;
#line 618 "../lib/Chai.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl, INT64(618), INT32(65));
#line 618 "../lib/Chai.chpl"
    (_args_forcoforall_fn_chpl)->_7_a = *(call_tmp_chpl7);
#line 618 "../lib/Chai.chpl"
    _args_vforcoforall_fn_chpl = ((void*)(_args_forcoforall_fn_chpl));
#line 618 "../lib/Chai.chpl"
    _args_sizecoforall_fn_chpl = sizeof(chpl__class_localscoforall_fn_chpl21_object);
#line 618 "../lib/Chai.chpl"
    (_args_forcoforall_fn_chpl)->_ln = INT64(618);
#line 618 "../lib/Chai.chpl"
    (_args_forcoforall_fn_chpl)->_fn = INT32(65);
#line 618 "../lib/Chai.chpl"
    /*** wrapcoforall_fn_chpl23 ***/ chpl_taskAddCoStmt(chpl_task_getRequestedSubloc(), INT64(22), ((chpl_task_bundle_p)(_args_vforcoforall_fn_chpl)), _args_sizecoforall_fn_chpl, INT64(2763), INT32(30));
#line 618 "../lib/Chai.chpl"
  }
#line 618 "../lib/Chai.chpl"
  error_chpl3 = NULL;
#line 618 "../lib/Chai.chpl"
  _waitEndCount_chpl2(_coforallCount_chpl, numTasks_chpl, &error_chpl3, INT64(618), INT32(65));
#line 618 "../lib/Chai.chpl"
  if (error_chpl3 != nil) /* ZLINE: 618 ../lib/Chai.chpl */
#line 618 "../lib/Chai.chpl"
  {
#line 618 "../lib/Chai.chpl"
    chpl__delete72(_coforallCount_chpl, INT64(618), INT32(65));
#line 618 "../lib/Chai.chpl"
    error_chpl2 = error_chpl3;
#line 618 "../lib/Chai.chpl"
    goto forall_IBB_break_label_chpl;
#line 618 "../lib/Chai.chpl"
  }
#line 618 "../lib/Chai.chpl"
  chpl__delete72(_coforallCount_chpl, INT64(618), INT32(65));
#line 618 "../lib/Chai.chpl"
  chpl_after_forall_fence();
#line 618 "../lib/Chai.chpl"
  forall_IBB_break_label_chpl:;
#line 618 "../lib/Chai.chpl"
  if (error_chpl2 != nil) /* ZLINE: 618 ../lib/Chai.chpl */
#line 618 "../lib/Chai.chpl"
  {
#line 618 "../lib/Chai.chpl"
    error_chpl = error_chpl2;
#line 618 "../lib/Chai.chpl"
    goto forall_IBB_break_label_chpl2;
#line 618 "../lib/Chai.chpl"
  }
#line 618 "../lib/Chai.chpl"
  chpl_after_forall_fence();
#line 618 "../lib/Chai.chpl"
  forall_IBB_break_label_chpl2:;
#line 618 "../lib/Chai.chpl"
  if (error_chpl != nil) /* ZLINE: 618 ../lib/Chai.chpl */
#line 618 "../lib/Chai.chpl"
  {
#line 618 "../lib/Chai.chpl"
    chpl_propagate_error(error_chpl, INT64(618), INT32(65));
#line 618 "../lib/Chai.chpl"
  }
#line 618 "../lib/Chai.chpl"
  chpl_after_forall_fence();
#line 619 "../lib/Chai.chpl"
  call_tmp_chpl34 = &((this_chpl31)->biases);
#line 619 "../lib/Chai.chpl"
  call_tmp_chpl35 = &((call_tmp_chpl34)->data);
#line 619 "../lib/Chai.chpl"
  call_tmp_chpl36 = &((this_chpl31)->biasesGrad);
#line 619 "../lib/Chai.chpl"
  call_tmp_chpl37 = &((call_tmp_chpl36)->data);
#line 619 "../lib/Chai.chpl"
  _ir_F1_b_chpl3 = (call_tmp_chpl36)->data;
#line 619 "../lib/Chai.chpl"
  (&ret_tmp_chpl14)->_shape_ = _ir__shape__chpl3;
#line 619 "../lib/Chai.chpl"
  (&ret_tmp_chpl14)->F0_a = mag_chpl;
#line 619 "../lib/Chai.chpl"
  (&ret_tmp_chpl14)->F1_b = _ir_F1_b_chpl3;
#line 619 "../lib/Chai.chpl"
  call_tmp_chpl38 = ret_tmp_chpl14;
#line 619 "../lib/Chai.chpl"
  _dom_chpl18(call_tmp_chpl37, &ret_tmp_chpl15, INT64(619), INT32(65));
#line 619 "../lib/Chai.chpl"
  call_tmp_chpl39 = ret_tmp_chpl15;
#line 619 "../lib/Chai.chpl"
  ret_chpl4 = (&call_tmp_chpl39)->_instance;
#line 619 "../lib/Chai.chpl"
  chpl__autoDestroy11(&call_tmp_chpl39, INT64(619), INT32(65));
#line 619 "../lib/Chai.chpl"
  (&call_tmp_chpl38)->_shape_ = ret_chpl4;
#line 619 "../lib/Chai.chpl"
  _ir_F0_a_chpl2 = (call_tmp_chpl34)->data;
#line 619 "../lib/Chai.chpl"
  _ir_F1_b_chpl4 = &call_tmp_chpl38;
#line 619 "../lib/Chai.chpl"
  (&call_tmp_chpl40)->_shape_ = _ir__shape__chpl4;
#line 619 "../lib/Chai.chpl"
  (&call_tmp_chpl40)->F0_a = _ir_F0_a_chpl2;
#line 619 "../lib/Chai.chpl"
  (&call_tmp_chpl40)->F1_b = _ir_F1_b_chpl4;
#line 619 "../lib/Chai.chpl"
  _dom_chpl18(call_tmp_chpl35, &ret_tmp_chpl16, INT64(619), INT32(65));
#line 619 "../lib/Chai.chpl"
  call_tmp_chpl41 = ret_tmp_chpl16;
#line 619 "../lib/Chai.chpl"
  ret_chpl5 = (&call_tmp_chpl41)->_instance;
#line 619 "../lib/Chai.chpl"
  chpl__autoDestroy11(&call_tmp_chpl41, INT64(619), INT32(65));
#line 619 "../lib/Chai.chpl"
  (&call_tmp_chpl40)->_shape_ = ret_chpl5;
#line 619 "../lib/Chai.chpl"
  coerce_tmp_chpl10 = (call_tmp_chpl35)->_instance;
#line 619 "../lib/Chai.chpl"
  tasksPerLocale_chpl2 = local_dataParTasksPerLocale_chpl;
#line 619 "../lib/Chai.chpl"
  ignoreRunning_chpl2 = local_dataParIgnoreRunningTasks_chpl;
#line 619 "../lib/Chai.chpl"
  minIndicesPerTask_chpl2 = local_dataParMinGranularity_chpl;
#line 619 "../lib/Chai.chpl"
  error_chpl4 = NULL;
#line 619 "../lib/Chai.chpl"
  chpl_check_nil(coerce_tmp_chpl10, INT64(619), INT32(65));
#line 619 "../lib/Chai.chpl"
  coerce_tmp_chpl11 = (coerce_tmp_chpl10)->dom;
#line 619 "../lib/Chai.chpl"
  createTuple_chpl(INT64(0), &ret_tmp_chpl17);
#line 619 "../lib/Chai.chpl"
  offset_x0_chpl2 = *(ret_tmp_chpl17 + INT64(0));
#line 619 "../lib/Chai.chpl"
  error_chpl5 = NULL;
#line 619 "../lib/Chai.chpl"
  call_tmp_chpl44 = get_chpl_nodeID();
#line 619 "../lib/Chai.chpl"
  call_tmp_chpl45 = chpl_rt_buildLocaleID(call_tmp_chpl44, c_sublocid_any);
#line 619 "../lib/Chai.chpl"
  call_tmp_chpl43 = call_tmp_chpl45;
#line 619 "../lib/Chai.chpl"
  chpl_localeID_to_locale(&call_tmp_chpl43, &ret_tmp_chpl18, INT64(619), INT32(65));
#line 619 "../lib/Chai.chpl"
  call_tmp_chpl46 = ret_tmp_chpl18;
#line 619 "../lib/Chai.chpl"
  call_tmp_chpl42 = call_tmp_chpl46;
#line 619 "../lib/Chai.chpl"
  coerce_tmp_chpl12 = (&call_tmp_chpl42)->_instance;
#line 619 "../lib/Chai.chpl"
  if (((RootClass_chpl)(coerce_tmp_chpl12)) == nil) /* ZLINE: 619 ../lib/Chai.chpl */
#line 619 "../lib/Chai.chpl"
  {
#line 619 "../lib/Chai.chpl"
    nilCheckHalt_chpl(&_str_literal_52_chpl, INT64(619), INT32(65));
#line 619 "../lib/Chai.chpl"
  }
#line 619 "../lib/Chai.chpl"
  chpl_check_nil(coerce_tmp_chpl12, INT64(619), INT32(65));
#line 619 "../lib/Chai.chpl"
  ((int64_t(*)(BaseLocale_chpl,
#line 619 "../lib/Chai.chpl"
     int64_t,
#line 619 "../lib/Chai.chpl"
     int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(coerce_tmp_chpl12))->chpl__cid) + INT64(2))])(coerce_tmp_chpl12, INT64(619), INT32(65));
#line 619 "../lib/Chai.chpl"
  i_x_chpl3 = &call_tmp_chpl42;
#line 619 "../lib/Chai.chpl"
  deinit_chpl62(i_x_chpl3);
#line 619 "../lib/Chai.chpl"
  if (tasksPerLocale_chpl2 == INT64(0)) /* ZLINE: 619 ../lib/Chai.chpl */
#line 619 "../lib/Chai.chpl"
  {
#line 619 "../lib/Chai.chpl"
    call_tmp_chpl49 = get_chpl_nodeID();
#line 619 "../lib/Chai.chpl"
    call_tmp_chpl50 = chpl_rt_buildLocaleID(call_tmp_chpl49, c_sublocid_any);
#line 619 "../lib/Chai.chpl"
    call_tmp_chpl48 = call_tmp_chpl50;
#line 619 "../lib/Chai.chpl"
    chpl_localeID_to_locale(&call_tmp_chpl48, &ret_tmp_chpl19, INT64(619), INT32(65));
#line 619 "../lib/Chai.chpl"
    call_tmp_chpl51 = ret_tmp_chpl19;
#line 619 "../lib/Chai.chpl"
    call_tmp_chpl47 = call_tmp_chpl51;
#line 619 "../lib/Chai.chpl"
    coerce_tmp_chpl13 = (&call_tmp_chpl47)->_instance;
#line 619 "../lib/Chai.chpl"
    if (((RootClass_chpl)(coerce_tmp_chpl13)) == nil) /* ZLINE: 619 ../lib/Chai.chpl */
#line 619 "../lib/Chai.chpl"
    {
#line 619 "../lib/Chai.chpl"
      nilCheckHalt_chpl(&_str_literal_52_chpl, INT64(619), INT32(65));
#line 619 "../lib/Chai.chpl"
    }
#line 619 "../lib/Chai.chpl"
    coerce_tmp_chpl14 = coerce_tmp_chpl13;
#line 619 "../lib/Chai.chpl"
    chpl_check_nil(coerce_tmp_chpl14, INT64(619), INT32(65));
#line 619 "../lib/Chai.chpl"
    ret_chpl6 = (coerce_tmp_chpl14)->maxTaskPar;
#line 619 "../lib/Chai.chpl"
    tmp_chpl3 = ret_chpl6;
#line 619 "../lib/Chai.chpl"
    i_x_chpl4 = &call_tmp_chpl47;
#line 619 "../lib/Chai.chpl"
    deinit_chpl62(i_x_chpl4);
#line 619 "../lib/Chai.chpl"
  } else /* ZLINE: 619 ../lib/Chai.chpl */
#line 619 "../lib/Chai.chpl"
  {
#line 619 "../lib/Chai.chpl"
    tmp_chpl3 = tasksPerLocale_chpl2;
#line 619 "../lib/Chai.chpl"
  }
#line 619 "../lib/Chai.chpl"
  if (chpl_task_getSerial()) /* ZLINE: 619 ../lib/Chai.chpl */
#line 619 "../lib/Chai.chpl"
  {
#line 619 "../lib/Chai.chpl"
    tmp_x0_chpl2 = INT64(1);
#line 619 "../lib/Chai.chpl"
    tmp_x1_chpl2 = INT64(0);
#line 619 "../lib/Chai.chpl"
  } else /* ZLINE: 619 ../lib/Chai.chpl */
#line 619 "../lib/Chai.chpl"
  {
#line 619 "../lib/Chai.chpl"
    chpl_check_nil(coerce_tmp_chpl11, INT64(619), INT32(65));
#line 619 "../lib/Chai.chpl"
    call_tmp_chpl52 = &((coerce_tmp_chpl11)->ranges);
#line 619 "../lib/Chai.chpl"
    _computeChunkStuff_chpl(tmp_chpl3, ignoreRunning_chpl2, minIndicesPerTask_chpl2, call_tmp_chpl52, &ret_tmp_chpl20, INT64(619), INT32(65));
#line 619 "../lib/Chai.chpl"
    tmp_x0_chpl2 = *(ret_tmp_chpl20 + INT64(0));
#line 619 "../lib/Chai.chpl"
    tmp_x1_chpl2 = *(ret_tmp_chpl20 + INT64(1));
#line 619 "../lib/Chai.chpl"
  }
#line 619 "../lib/Chai.chpl"
  numChunks_chpl2 = tmp_x0_chpl2;
#line 619 "../lib/Chai.chpl"
  parDim_chpl2 = tmp_x1_chpl2;
#line 619 "../lib/Chai.chpl"
  init_chpl113(&elt_x0_chpl2);
#line 619 "../lib/Chai.chpl"
  tup_x0_chpl2 = elt_x0_chpl2;
#line 619 "../lib/Chai.chpl"
  *(locBlock_chpl2 + INT64(0)) = tup_x0_chpl2;
#line 619 "../lib/Chai.chpl"
  chpl_build_low_bounded_range2(offset_x0_chpl2, &ret_tmp_chpl21);
#line 619 "../lib/Chai.chpl"
  call_tmp_chpl53 = ret_tmp_chpl21;
#line 619 "../lib/Chai.chpl"
  chpl_check_nil(coerce_tmp_chpl11, INT64(619), INT32(65));
#line 619 "../lib/Chai.chpl"
  call_tmp_chpl54 = &((coerce_tmp_chpl11)->ranges);
#line 619 "../lib/Chai.chpl"
  coerce_tmp_chpl15 = *(*(call_tmp_chpl54) + INT64(0));
#line 619 "../lib/Chai.chpl"
  call_tmp_chpl55 = sizeAs_chpl37(coerce_tmp_chpl15, INT64(619), INT32(65));
#line 619 "../lib/Chai.chpl"
  chpl___POUND_3(call_tmp_chpl53, call_tmp_chpl55, &ret_tmp_chpl22, INT64(619), INT32(65));
#line 619 "../lib/Chai.chpl"
  call_tmp_chpl56 = ret_tmp_chpl22;
#line 619 "../lib/Chai.chpl"
  *(locBlock_chpl2 + INT64(0)) = call_tmp_chpl56;
#line 619 "../lib/Chai.chpl"
  chpl_build_low_bounded_range2(INT64(0), &ret_tmp_chpl23);
#line 619 "../lib/Chai.chpl"
  call_tmp_chpl57 = ret_tmp_chpl23;
#line 619 "../lib/Chai.chpl"
  chpl___POUND_3(call_tmp_chpl57, tmp_x0_chpl2, &ret_tmp_chpl24, INT64(619), INT32(65));
#line 619 "../lib/Chai.chpl"
  tmpIter_chpl2 = ret_tmp_chpl24;
#line 619 "../lib/Chai.chpl"
  new_temp_chpl2 = _new_chpl17(INT64(619), INT32(65));
#line 619 "../lib/Chai.chpl"
  _coforallCount_chpl2 = new_temp_chpl2;
#line 619 "../lib/Chai.chpl"
  numTasks_chpl2 = chpl_boundedCoforallSize(tmpIter_chpl2, INT64(619), INT32(65));
#line 619 "../lib/Chai.chpl"
  _upEndCount_chpl(_coforallCount_chpl2, numTasks_chpl2, INT64(619), INT32(65));
#line 619 "../lib/Chai.chpl"
  chpl_resetTaskSpawn(numTasks_chpl2, INT64(619), INT32(65));
#line 619 "../lib/Chai.chpl"
  _ic__F0_this_chpl2 = tmpIter_chpl2;
#line 619 "../lib/Chai.chpl"
  checkIfIterWillOverflow_chpl(tmpIter_chpl2, UINT8(true), INT64(619), INT32(65));
#line 619 "../lib/Chai.chpl"
  i_chpl2 = INT64(0);
#line 619 "../lib/Chai.chpl"
  this_chpl34 = _ic__F0_this_chpl2;
#line 619 "../lib/Chai.chpl"
  this_chpl35 = _ic__F0_this_chpl2;
#line 619 "../lib/Chai.chpl"
  end_chpl2 = (&this_chpl35)->_high;
#line 619 "../lib/Chai.chpl"
  for (i_chpl2 = (&this_chpl34)->_low; ((i_chpl2 <= end_chpl2)); i_chpl2 += INT64(1)) {
#line 619 "../lib/Chai.chpl"
    _args_forcoforall_fn_chpl2 = ((_class_localscoforall_fn_chpl17)(&chpl_macro_tmp_122));
#line 619 "../lib/Chai.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl2, INT64(619), INT32(65));
#line 619 "../lib/Chai.chpl"
    (_args_forcoforall_fn_chpl2)->_0_numChunks = numChunks_chpl2;
#line 619 "../lib/Chai.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl2, INT64(619), INT32(65));
#line 619 "../lib/Chai.chpl"
    (_args_forcoforall_fn_chpl2)->_1_parDim = parDim_chpl2;
#line 619 "../lib/Chai.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl2, INT64(619), INT32(65));
#line 619 "../lib/Chai.chpl"
    *((_args_forcoforall_fn_chpl2)->_2_coerce_tmp + INT64(0)) = *(locBlock_chpl2 + INT64(0));
#line 619 "../lib/Chai.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl2, INT64(619), INT32(65));
#line 619 "../lib/Chai.chpl"
    (_args_forcoforall_fn_chpl2)->_3__coforallCount = _coforallCount_chpl2;
#line 619 "../lib/Chai.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl2, INT64(619), INT32(65));
#line 619 "../lib/Chai.chpl"
    (_args_forcoforall_fn_chpl2)->_4_chunk = i_chpl2;
#line 619 "../lib/Chai.chpl"
    tmp_chpl4 = &call_tmp_chpl40;
#line 619 "../lib/Chai.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl2, INT64(619), INT32(65));
#line 619 "../lib/Chai.chpl"
    (_args_forcoforall_fn_chpl2)->_6_b = tmp_chpl4;
#line 619 "../lib/Chai.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl2, INT64(619), INT32(65));
#line 619 "../lib/Chai.chpl"
    (_args_forcoforall_fn_chpl2)->_7_a = *(call_tmp_chpl35);
#line 619 "../lib/Chai.chpl"
    _args_vforcoforall_fn_chpl2 = ((void*)(_args_forcoforall_fn_chpl2));
#line 619 "../lib/Chai.chpl"
    _args_sizecoforall_fn_chpl2 = sizeof(chpl__class_localscoforall_fn_chpl17_object);
#line 619 "../lib/Chai.chpl"
    (_args_forcoforall_fn_chpl2)->_ln = INT64(619);
#line 619 "../lib/Chai.chpl"
    (_args_forcoforall_fn_chpl2)->_fn = INT32(65);
#line 619 "../lib/Chai.chpl"
    /*** wrapcoforall_fn_chpl22 ***/ chpl_taskAddCoStmt(chpl_task_getRequestedSubloc(), INT64(21), ((chpl_task_bundle_p)(_args_vforcoforall_fn_chpl2)), _args_sizecoforall_fn_chpl2, INT64(2763), INT32(30));
#line 619 "../lib/Chai.chpl"
  }
#line 619 "../lib/Chai.chpl"
  error_chpl6 = NULL;
#line 619 "../lib/Chai.chpl"
  _waitEndCount_chpl2(_coforallCount_chpl2, numTasks_chpl2, &error_chpl6, INT64(619), INT32(65));
#line 619 "../lib/Chai.chpl"
  if (error_chpl6 != nil) /* ZLINE: 619 ../lib/Chai.chpl */
#line 619 "../lib/Chai.chpl"
  {
#line 619 "../lib/Chai.chpl"
    chpl__delete72(_coforallCount_chpl2, INT64(619), INT32(65));
#line 619 "../lib/Chai.chpl"
    error_chpl5 = error_chpl6;
#line 619 "../lib/Chai.chpl"
    goto forall_IBB_break_label_chpl3;
#line 619 "../lib/Chai.chpl"
  }
#line 619 "../lib/Chai.chpl"
  chpl__delete72(_coforallCount_chpl2, INT64(619), INT32(65));
#line 619 "../lib/Chai.chpl"
  chpl_after_forall_fence();
#line 619 "../lib/Chai.chpl"
  forall_IBB_break_label_chpl3:;
#line 619 "../lib/Chai.chpl"
  if (error_chpl5 != nil) /* ZLINE: 619 ../lib/Chai.chpl */
#line 619 "../lib/Chai.chpl"
  {
#line 619 "../lib/Chai.chpl"
    error_chpl4 = error_chpl5;
#line 619 "../lib/Chai.chpl"
    goto forall_IBB_break_label_chpl4;
#line 619 "../lib/Chai.chpl"
  }
#line 619 "../lib/Chai.chpl"
  chpl_after_forall_fence();
#line 619 "../lib/Chai.chpl"
  forall_IBB_break_label_chpl4:;
#line 619 "../lib/Chai.chpl"
  if (error_chpl4 != nil) /* ZLINE: 619 ../lib/Chai.chpl */
#line 619 "../lib/Chai.chpl"
  {
#line 619 "../lib/Chai.chpl"
    chpl_propagate_error(error_chpl4, INT64(619), INT32(65));
#line 619 "../lib/Chai.chpl"
  }
#line 619 "../lib/Chai.chpl"
  chpl_after_forall_fence();
#line 617 "../lib/Chai.chpl"
  return;
#line 617 "../lib/Chai.chpl"
}

#line 621 "../lib/Chai.chpl"
/* Chai.chpl:621 */
#line 621 "../lib/Chai.chpl"
static void resetGradients_chpl3(SoftMax_chpl * this_chpl31) {
#line 622 "../lib/Chai.chpl"
  _ref_Tensor_2__real64_chpl call_tmp_chpl6 = NULL;
#line 622 "../lib/Chai.chpl"
  _ref__array_DefaultRectangularArr_2_int64_t_one__real64_int64_t_chpl call_tmp_chpl7 = NULL;
#line 623 "../lib/Chai.chpl"
  _ref_Tensor_1__real64_chpl call_tmp_chpl8 = NULL;
#line 623 "../lib/Chai.chpl"
  _ref__array_DefaultRectangularArr_1_int64_t_one__real64_int64_t_chpl call_tmp_chpl9 = NULL;
#line 622 "../lib/Chai.chpl"
  call_tmp_chpl6 = &((this_chpl31)->weightsGrad);
#line 622 "../lib/Chai.chpl"
  call_tmp_chpl7 = &((call_tmp_chpl6)->data);
#line 622 "../lib/Chai.chpl"
  chpl___ASSIGN_3(call_tmp_chpl7, REAL64(0x0p+0), INT64(622), INT32(65));
#line 623 "../lib/Chai.chpl"
  call_tmp_chpl8 = &((this_chpl31)->biasesGrad);
#line 623 "../lib/Chai.chpl"
  call_tmp_chpl9 = &((call_tmp_chpl8)->data);
#line 623 "../lib/Chai.chpl"
  chpl___ASSIGN_(call_tmp_chpl9, REAL64(0x0p+0), INT64(623), INT32(65));
#line 621 "../lib/Chai.chpl"
  return;
#line 621 "../lib/Chai.chpl"
}

#line 626 "../lib/Chai.chpl"
/* Chai.chpl:626 */
#line 626 "../lib/Chai.chpl"
static void write_chpl4(SoftMax_chpl * this_chpl31,
#line 626 "../lib/Chai.chpl"
                        fileWriter_T_BinarySerializer_chpl * fw_chpl,
#line 626 "../lib/Chai.chpl"
                        Error_chpl * error_out_chpl) {
#line 627 "../lib/Chai.chpl"
  _ref_Tensor_2__real64_chpl call_tmp_chpl6 = NULL;
#line 627 "../lib/Chai.chpl"
  Error_chpl error_chpl = NULL;
#line 628 "../lib/Chai.chpl"
  _ref_Tensor_1__real64_chpl call_tmp_chpl7 = NULL;
#line 628 "../lib/Chai.chpl"
  Error_chpl error_chpl2 = NULL;
#line 627 "../lib/Chai.chpl"
  call_tmp_chpl6 = &((this_chpl31)->weights);
#line 627 "../lib/Chai.chpl"
  error_chpl = NULL;
#line 627 "../lib/Chai.chpl"
  write_chpl6(call_tmp_chpl6, fw_chpl, &error_chpl);
#line 627 "../lib/Chai.chpl"
  if (error_chpl != nil) /* ZLINE: 627 ../lib/Chai.chpl */
#line 627 "../lib/Chai.chpl"
  {
#line 627 "../lib/Chai.chpl"
    *(error_out_chpl) = error_chpl;
#line 627 "../lib/Chai.chpl"
    goto _endwrite_chpl;
#line 627 "../lib/Chai.chpl"
  }
#line 628 "../lib/Chai.chpl"
  call_tmp_chpl7 = &((this_chpl31)->biases);
#line 628 "../lib/Chai.chpl"
  error_chpl2 = NULL;
#line 628 "../lib/Chai.chpl"
  write_chpl5(call_tmp_chpl7, fw_chpl, &error_chpl2);
#line 628 "../lib/Chai.chpl"
  if (error_chpl2 != nil) /* ZLINE: 628 ../lib/Chai.chpl */
#line 628 "../lib/Chai.chpl"
  {
#line 628 "../lib/Chai.chpl"
    *(error_out_chpl) = error_chpl2;
#line 628 "../lib/Chai.chpl"
    goto _endwrite_chpl;
#line 628 "../lib/Chai.chpl"
  }
#line 628 "../lib/Chai.chpl"
  _endwrite_chpl:;
#line 626 "../lib/Chai.chpl"
  return;
#line 626 "../lib/Chai.chpl"
}

#line 636 "../lib/Chai.chpl"
/* Chai.chpl:636 */
#line 636 "../lib/Chai.chpl"
static void signature_chpl3(SoftMax_chpl * this_chpl31,
#line 636 "../lib/Chai.chpl"
                            string_chpl * _retArg_chpl) {
#line 637 "../lib/Chai.chpl"
  string_chpl call_tmp_chpl6;
#line 637 "../lib/Chai.chpl"
  string_chpl ret_tmp_chpl;
#line 637 "../lib/Chai.chpl"
  string_chpl call_tmp_chpl7;
#line 637 "../lib/Chai.chpl"
  string_chpl ret_tmp_chpl2;
#line 637 "../lib/Chai.chpl"
  string_chpl call_tmp_chpl8;
#line 637 "../lib/Chai.chpl"
  string_chpl ret_tmp_chpl3;
#line 636 "../lib/Chai.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 636 "../lib/Chai.chpl"
  _ref_string_chpl i_x_chpl2 = NULL;
#line 637 "../lib/Chai.chpl"
  chpl___COLON_25((this_chpl31)->outputSize, &ret_tmp_chpl, INT64(637), INT32(65));
#line 637 "../lib/Chai.chpl"
  call_tmp_chpl6 = ret_tmp_chpl;
#line 637 "../lib/Chai.chpl"
  chpl___PLUS_(&_str_literal_1721_chpl, &call_tmp_chpl6, &ret_tmp_chpl2, INT64(637), INT32(65));
#line 637 "../lib/Chai.chpl"
  call_tmp_chpl7 = ret_tmp_chpl2;
#line 637 "../lib/Chai.chpl"
  chpl___PLUS_(&call_tmp_chpl7, &_str_literal_415_chpl, &ret_tmp_chpl3, INT64(637), INT32(65));
#line 637 "../lib/Chai.chpl"
  call_tmp_chpl8 = ret_tmp_chpl3;
#line 636 "../lib/Chai.chpl"
  *(_retArg_chpl) = call_tmp_chpl8;
#line 636 "../lib/Chai.chpl"
  i_x_chpl = &call_tmp_chpl7;
#line 636 "../lib/Chai.chpl"
  deinit_chpl95(i_x_chpl, INT64(636), INT32(65));
#line 636 "../lib/Chai.chpl"
  i_x_chpl2 = &call_tmp_chpl6;
#line 636 "../lib/Chai.chpl"
  deinit_chpl95(i_x_chpl2, INT64(636), INT32(65));
#line 636 "../lib/Chai.chpl"
  return;
#line 636 "../lib/Chai.chpl"
}

#line 670 "../lib/Chai.chpl"
/* Chai.chpl:670 */
#line 670 "../lib/Chai.chpl"
static void forwardPropHelpBatch_chpl2(_tuple_4_Conv_Conv_MaxPool_SoftMax_chpl * layers_chpl2,
#line 670 "../lib/Chai.chpl"
                                       _array_DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl * inputs_chpl,
#line 670 "../lib/Chai.chpl"
                                       _array_DefaultRectangularArr_1_int64_t_one_Tensor_1__real64_int64_t_chpl * _retArg_chpl) {
#line 672 "../lib/Chai.chpl"
  _ref_Conv_chpl call_tmp_chpl6 = NULL;
#line 672 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl call_tmp_chpl7;
#line 672 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl ret_tmp_chpl;
#line 672 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl nextInputs_chpl;
#line 673 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_1_int64_t_one_Tensor_1__real64_int64_t_chpl call_tmp_chpl8;
#line 673 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_1_int64_t_one_Tensor_1__real64_int64_t_chpl ret_tmp_chpl2;
#line 672 "../lib/Chai.chpl"
  call_tmp_chpl6 = &((layers_chpl2)->x0);
#line 672 "../lib/Chai.chpl"
  forwardPropBatch_chpl(call_tmp_chpl6, inputs_chpl, &ret_tmp_chpl);
#line 672 "../lib/Chai.chpl"
  call_tmp_chpl7 = ret_tmp_chpl;
#line 672 "../lib/Chai.chpl"
  nextInputs_chpl = call_tmp_chpl7;
#line 673 "../lib/Chai.chpl"
  forwardPropHelpBatch_chpl3(layers_chpl2, &nextInputs_chpl, &ret_tmp_chpl2);
#line 673 "../lib/Chai.chpl"
  call_tmp_chpl8 = ret_tmp_chpl2;
#line 673 "../lib/Chai.chpl"
  chpl__autoDestroy13(&nextInputs_chpl, INT64(673), INT32(65));
#line 670 "../lib/Chai.chpl"
  *(_retArg_chpl) = call_tmp_chpl8;
#line 670 "../lib/Chai.chpl"
  return;
#line 670 "../lib/Chai.chpl"
}

#line 670 "../lib/Chai.chpl"
/* Chai.chpl:670 */
#line 670 "../lib/Chai.chpl"
static void forwardPropHelpBatch_chpl4(_tuple_4_Conv_Conv_MaxPool_SoftMax_chpl * layers_chpl2,
#line 670 "../lib/Chai.chpl"
                                       _array_DefaultRectangularArr_1_int64_t_one_Tensor_1__real64_int64_t_chpl * inputs_chpl,
#line 670 "../lib/Chai.chpl"
                                       _array_DefaultRectangularArr_1_int64_t_one_Tensor_1__real64_int64_t_chpl * _retArg_chpl) {
#line 670 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_1_int64_t_one_Tensor_1__real64_int64_t_chpl ret_chpl;
#line 671 "../lib/Chai.chpl"
  ret_chpl = chpl__autoCopy5(inputs_chpl, UINT8(false), INT64(671), INT32(65));
#line 670 "../lib/Chai.chpl"
  *(_retArg_chpl) = ret_chpl;
#line 670 "../lib/Chai.chpl"
  return;
#line 670 "../lib/Chai.chpl"
}

#line 670 "../lib/Chai.chpl"
/* Chai.chpl:670 */
#line 670 "../lib/Chai.chpl"
static void forwardPropHelpBatch_chpl(_tuple_4_Conv_Conv_MaxPool_SoftMax_chpl * layers_chpl2,
#line 670 "../lib/Chai.chpl"
                                      _array_DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl * inputs_chpl,
#line 670 "../lib/Chai.chpl"
                                      _array_DefaultRectangularArr_1_int64_t_one_Tensor_1__real64_int64_t_chpl * _retArg_chpl) {
#line 672 "../lib/Chai.chpl"
  _ref_SoftMax_chpl call_tmp_chpl6 = NULL;
#line 672 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_1_int64_t_one_Tensor_1__real64_int64_t_chpl call_tmp_chpl7;
#line 672 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_1_int64_t_one_Tensor_1__real64_int64_t_chpl ret_tmp_chpl;
#line 672 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_1_int64_t_one_Tensor_1__real64_int64_t_chpl nextInputs_chpl;
#line 673 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_1_int64_t_one_Tensor_1__real64_int64_t_chpl call_tmp_chpl8;
#line 673 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_1_int64_t_one_Tensor_1__real64_int64_t_chpl ret_tmp_chpl2;
#line 672 "../lib/Chai.chpl"
  call_tmp_chpl6 = &((layers_chpl2)->x3);
#line 672 "../lib/Chai.chpl"
  forwardPropBatch_chpl3(call_tmp_chpl6, inputs_chpl, &ret_tmp_chpl);
#line 672 "../lib/Chai.chpl"
  call_tmp_chpl7 = ret_tmp_chpl;
#line 672 "../lib/Chai.chpl"
  nextInputs_chpl = call_tmp_chpl7;
#line 673 "../lib/Chai.chpl"
  forwardPropHelpBatch_chpl4(layers_chpl2, &nextInputs_chpl, &ret_tmp_chpl2);
#line 673 "../lib/Chai.chpl"
  call_tmp_chpl8 = ret_tmp_chpl2;
#line 673 "../lib/Chai.chpl"
  chpl__autoDestroy20(&nextInputs_chpl, INT64(673), INT32(65));
#line 670 "../lib/Chai.chpl"
  *(_retArg_chpl) = call_tmp_chpl8;
#line 670 "../lib/Chai.chpl"
  return;
#line 670 "../lib/Chai.chpl"
}

#line 670 "../lib/Chai.chpl"
/* Chai.chpl:670 */
#line 670 "../lib/Chai.chpl"
static void forwardPropHelpBatch_chpl5(_tuple_4_Conv_Conv_MaxPool_SoftMax_chpl * layers_chpl2,
#line 670 "../lib/Chai.chpl"
                                       _array_DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl * inputs_chpl,
#line 670 "../lib/Chai.chpl"
                                       _array_DefaultRectangularArr_1_int64_t_one_Tensor_1__real64_int64_t_chpl * _retArg_chpl) {
#line 672 "../lib/Chai.chpl"
  _ref_MaxPool_chpl call_tmp_chpl6 = NULL;
#line 672 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl call_tmp_chpl7;
#line 672 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl ret_tmp_chpl;
#line 672 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl nextInputs_chpl;
#line 673 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_1_int64_t_one_Tensor_1__real64_int64_t_chpl call_tmp_chpl8;
#line 673 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_1_int64_t_one_Tensor_1__real64_int64_t_chpl ret_tmp_chpl2;
#line 672 "../lib/Chai.chpl"
  call_tmp_chpl6 = &((layers_chpl2)->x2);
#line 672 "../lib/Chai.chpl"
  forwardPropBatch_chpl2(call_tmp_chpl6, inputs_chpl, &ret_tmp_chpl);
#line 672 "../lib/Chai.chpl"
  call_tmp_chpl7 = ret_tmp_chpl;
#line 672 "../lib/Chai.chpl"
  nextInputs_chpl = call_tmp_chpl7;
#line 673 "../lib/Chai.chpl"
  forwardPropHelpBatch_chpl(layers_chpl2, &nextInputs_chpl, &ret_tmp_chpl2);
#line 673 "../lib/Chai.chpl"
  call_tmp_chpl8 = ret_tmp_chpl2;
#line 673 "../lib/Chai.chpl"
  chpl__autoDestroy13(&nextInputs_chpl, INT64(673), INT32(65));
#line 670 "../lib/Chai.chpl"
  *(_retArg_chpl) = call_tmp_chpl8;
#line 670 "../lib/Chai.chpl"
  return;
#line 670 "../lib/Chai.chpl"
}

#line 670 "../lib/Chai.chpl"
/* Chai.chpl:670 */
#line 670 "../lib/Chai.chpl"
static void forwardPropHelpBatch_chpl3(_tuple_4_Conv_Conv_MaxPool_SoftMax_chpl * layers_chpl2,
#line 670 "../lib/Chai.chpl"
                                       _array_DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl * inputs_chpl,
#line 670 "../lib/Chai.chpl"
                                       _array_DefaultRectangularArr_1_int64_t_one_Tensor_1__real64_int64_t_chpl * _retArg_chpl) {
#line 672 "../lib/Chai.chpl"
  _ref_Conv_chpl call_tmp_chpl6 = NULL;
#line 672 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl call_tmp_chpl7;
#line 672 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl ret_tmp_chpl;
#line 672 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl nextInputs_chpl;
#line 673 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_1_int64_t_one_Tensor_1__real64_int64_t_chpl call_tmp_chpl8;
#line 673 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_1_int64_t_one_Tensor_1__real64_int64_t_chpl ret_tmp_chpl2;
#line 672 "../lib/Chai.chpl"
  call_tmp_chpl6 = &((layers_chpl2)->x1);
#line 672 "../lib/Chai.chpl"
  forwardPropBatch_chpl(call_tmp_chpl6, inputs_chpl, &ret_tmp_chpl);
#line 672 "../lib/Chai.chpl"
  call_tmp_chpl7 = ret_tmp_chpl;
#line 672 "../lib/Chai.chpl"
  nextInputs_chpl = call_tmp_chpl7;
#line 673 "../lib/Chai.chpl"
  forwardPropHelpBatch_chpl5(layers_chpl2, &nextInputs_chpl, &ret_tmp_chpl2);
#line 673 "../lib/Chai.chpl"
  call_tmp_chpl8 = ret_tmp_chpl2;
#line 673 "../lib/Chai.chpl"
  chpl__autoDestroy13(&nextInputs_chpl, INT64(673), INT32(65));
#line 670 "../lib/Chai.chpl"
  *(_retArg_chpl) = call_tmp_chpl8;
#line 670 "../lib/Chai.chpl"
  return;
#line 670 "../lib/Chai.chpl"
}

#line 676 "../lib/Chai.chpl"
/* Chai.chpl:676 */
#line 676 "../lib/Chai.chpl"
static void backwardForwardPropHelpBatch_chpl2(_tuple_4_Conv_Conv_MaxPool_SoftMax_chpl * layers_chpl2,
#line 676 "../lib/Chai.chpl"
                                               _array_DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl * inputs_chpl,
#line 676 "../lib/Chai.chpl"
                                               _array_DefaultRectangularArr_1_int64_t_one_Tensor_1__real64_int64_t_chpl * lastDeltas_chpl,
#line 676 "../lib/Chai.chpl"
                                               _array_DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl * _retArg_chpl) {
#line 679 "../lib/Chai.chpl"
  _ref_SoftMax_chpl call_tmp_chpl6 = NULL;
#line 679 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_1_int64_t_one_Tensor_1__real64_int64_t_chpl call_tmp_chpl7;
#line 679 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_1_int64_t_one_Tensor_1__real64_int64_t_chpl ret_tmp_chpl;
#line 679 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_1_int64_t_one_Tensor_1__real64_int64_t_chpl lastInputs_chpl;
#line 680 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_1_int64_t_one_Tensor_1__real64_int64_t_chpl call_tmp_chpl8;
#line 680 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_1_int64_t_one_Tensor_1__real64_int64_t_chpl ret_tmp_chpl2;
#line 680 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_1_int64_t_one_Tensor_1__real64_int64_t_chpl deltas_chpl;
#line 681 "../lib/Chai.chpl"
  _ref_SoftMax_chpl call_tmp_chpl9 = NULL;
#line 681 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl call_tmp_chpl10;
#line 681 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl ret_tmp_chpl3;
#line 679 "../lib/Chai.chpl"
  call_tmp_chpl6 = &((layers_chpl2)->x3);
#line 679 "../lib/Chai.chpl"
  forwardPropBatch_chpl3(call_tmp_chpl6, inputs_chpl, &ret_tmp_chpl);
#line 679 "../lib/Chai.chpl"
  call_tmp_chpl7 = ret_tmp_chpl;
#line 679 "../lib/Chai.chpl"
  lastInputs_chpl = call_tmp_chpl7;
#line 680 "../lib/Chai.chpl"
  backwardForwardPropHelpBatch_chpl(layers_chpl2, &lastInputs_chpl, lastDeltas_chpl, &ret_tmp_chpl2);
#line 680 "../lib/Chai.chpl"
  call_tmp_chpl8 = ret_tmp_chpl2;
#line 680 "../lib/Chai.chpl"
  deltas_chpl = call_tmp_chpl8;
#line 681 "../lib/Chai.chpl"
  call_tmp_chpl9 = &((layers_chpl2)->x3);
#line 681 "../lib/Chai.chpl"
  backwardBatch_chpl3(call_tmp_chpl9, &deltas_chpl, inputs_chpl, &ret_tmp_chpl3);
#line 681 "../lib/Chai.chpl"
  call_tmp_chpl10 = ret_tmp_chpl3;
#line 681 "../lib/Chai.chpl"
  chpl__autoDestroy20(&deltas_chpl, INT64(681), INT32(65));
#line 681 "../lib/Chai.chpl"
  chpl__autoDestroy20(&lastInputs_chpl, INT64(681), INT32(65));
#line 676 "../lib/Chai.chpl"
  *(_retArg_chpl) = call_tmp_chpl10;
#line 676 "../lib/Chai.chpl"
  return;
#line 676 "../lib/Chai.chpl"
}

#line 676 "../lib/Chai.chpl"
/* Chai.chpl:676 */
#line 676 "../lib/Chai.chpl"
static void backwardForwardPropHelpBatch_chpl3(_tuple_4_Conv_Conv_MaxPool_SoftMax_chpl * layers_chpl2,
#line 676 "../lib/Chai.chpl"
                                               _array_DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl * inputs_chpl,
#line 676 "../lib/Chai.chpl"
                                               _array_DefaultRectangularArr_1_int64_t_one_Tensor_1__real64_int64_t_chpl * lastDeltas_chpl,
#line 676 "../lib/Chai.chpl"
                                               _array_DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl * _retArg_chpl) {
#line 679 "../lib/Chai.chpl"
  _ref_MaxPool_chpl call_tmp_chpl6 = NULL;
#line 679 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl call_tmp_chpl7;
#line 679 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl ret_tmp_chpl;
#line 679 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl lastInputs_chpl;
#line 680 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl call_tmp_chpl8;
#line 680 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl ret_tmp_chpl2;
#line 680 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl deltas_chpl;
#line 681 "../lib/Chai.chpl"
  _ref_MaxPool_chpl call_tmp_chpl9 = NULL;
#line 681 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl call_tmp_chpl10;
#line 681 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl ret_tmp_chpl3;
#line 679 "../lib/Chai.chpl"
  call_tmp_chpl6 = &((layers_chpl2)->x2);
#line 679 "../lib/Chai.chpl"
  forwardPropBatch_chpl2(call_tmp_chpl6, inputs_chpl, &ret_tmp_chpl);
#line 679 "../lib/Chai.chpl"
  call_tmp_chpl7 = ret_tmp_chpl;
#line 679 "../lib/Chai.chpl"
  lastInputs_chpl = call_tmp_chpl7;
#line 680 "../lib/Chai.chpl"
  backwardForwardPropHelpBatch_chpl2(layers_chpl2, &lastInputs_chpl, lastDeltas_chpl, &ret_tmp_chpl2);
#line 680 "../lib/Chai.chpl"
  call_tmp_chpl8 = ret_tmp_chpl2;
#line 680 "../lib/Chai.chpl"
  deltas_chpl = call_tmp_chpl8;
#line 681 "../lib/Chai.chpl"
  call_tmp_chpl9 = &((layers_chpl2)->x2);
#line 681 "../lib/Chai.chpl"
  backwardBatch_chpl2(call_tmp_chpl9, &deltas_chpl, inputs_chpl, &ret_tmp_chpl3);
#line 681 "../lib/Chai.chpl"
  call_tmp_chpl10 = ret_tmp_chpl3;
#line 681 "../lib/Chai.chpl"
  chpl__autoDestroy13(&deltas_chpl, INT64(681), INT32(65));
#line 681 "../lib/Chai.chpl"
  chpl__autoDestroy13(&lastInputs_chpl, INT64(681), INT32(65));
#line 676 "../lib/Chai.chpl"
  *(_retArg_chpl) = call_tmp_chpl10;
#line 676 "../lib/Chai.chpl"
  return;
#line 676 "../lib/Chai.chpl"
}

#line 676 "../lib/Chai.chpl"
/* Chai.chpl:676 */
#line 676 "../lib/Chai.chpl"
static void backwardForwardPropHelpBatch_chpl4(_tuple_4_Conv_Conv_MaxPool_SoftMax_chpl * layers_chpl2,
#line 676 "../lib/Chai.chpl"
                                               _array_DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl * inputs_chpl,
#line 676 "../lib/Chai.chpl"
                                               _array_DefaultRectangularArr_1_int64_t_one_Tensor_1__real64_int64_t_chpl * lastDeltas_chpl,
#line 676 "../lib/Chai.chpl"
                                               _array_DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl * _retArg_chpl) {
#line 679 "../lib/Chai.chpl"
  _ref_Conv_chpl call_tmp_chpl6 = NULL;
#line 679 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl call_tmp_chpl7;
#line 679 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl ret_tmp_chpl;
#line 679 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl lastInputs_chpl;
#line 680 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl call_tmp_chpl8;
#line 680 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl ret_tmp_chpl2;
#line 680 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl deltas_chpl;
#line 681 "../lib/Chai.chpl"
  _ref_Conv_chpl call_tmp_chpl9 = NULL;
#line 681 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl call_tmp_chpl10;
#line 681 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl ret_tmp_chpl3;
#line 679 "../lib/Chai.chpl"
  call_tmp_chpl6 = &((layers_chpl2)->x1);
#line 679 "../lib/Chai.chpl"
  forwardPropBatch_chpl(call_tmp_chpl6, inputs_chpl, &ret_tmp_chpl);
#line 679 "../lib/Chai.chpl"
  call_tmp_chpl7 = ret_tmp_chpl;
#line 679 "../lib/Chai.chpl"
  lastInputs_chpl = call_tmp_chpl7;
#line 680 "../lib/Chai.chpl"
  backwardForwardPropHelpBatch_chpl3(layers_chpl2, &lastInputs_chpl, lastDeltas_chpl, &ret_tmp_chpl2);
#line 680 "../lib/Chai.chpl"
  call_tmp_chpl8 = ret_tmp_chpl2;
#line 680 "../lib/Chai.chpl"
  deltas_chpl = call_tmp_chpl8;
#line 681 "../lib/Chai.chpl"
  call_tmp_chpl9 = &((layers_chpl2)->x1);
#line 681 "../lib/Chai.chpl"
  backwardBatch_chpl(call_tmp_chpl9, &deltas_chpl, inputs_chpl, &ret_tmp_chpl3);
#line 681 "../lib/Chai.chpl"
  call_tmp_chpl10 = ret_tmp_chpl3;
#line 681 "../lib/Chai.chpl"
  chpl__autoDestroy13(&deltas_chpl, INT64(681), INT32(65));
#line 681 "../lib/Chai.chpl"
  chpl__autoDestroy13(&lastInputs_chpl, INT64(681), INT32(65));
#line 676 "../lib/Chai.chpl"
  *(_retArg_chpl) = call_tmp_chpl10;
#line 676 "../lib/Chai.chpl"
  return;
#line 676 "../lib/Chai.chpl"
}

#line 676 "../lib/Chai.chpl"
/* Chai.chpl:676 */
#line 676 "../lib/Chai.chpl"
static void backwardForwardPropHelpBatch_chpl5(_tuple_4_Conv_Conv_MaxPool_SoftMax_chpl * layers_chpl2,
#line 676 "../lib/Chai.chpl"
                                               _array_DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl * inputs_chpl,
#line 676 "../lib/Chai.chpl"
                                               _array_DefaultRectangularArr_1_int64_t_one_Tensor_1__real64_int64_t_chpl * lastDeltas_chpl,
#line 676 "../lib/Chai.chpl"
                                               _array_DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl * _retArg_chpl) {
#line 679 "../lib/Chai.chpl"
  _ref_Conv_chpl call_tmp_chpl6 = NULL;
#line 679 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl call_tmp_chpl7;
#line 679 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl ret_tmp_chpl;
#line 679 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl lastInputs_chpl;
#line 680 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl call_tmp_chpl8;
#line 680 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl ret_tmp_chpl2;
#line 680 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl deltas_chpl;
#line 681 "../lib/Chai.chpl"
  _ref_Conv_chpl call_tmp_chpl9 = NULL;
#line 681 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl call_tmp_chpl10;
#line 681 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl ret_tmp_chpl3;
#line 679 "../lib/Chai.chpl"
  call_tmp_chpl6 = &((layers_chpl2)->x0);
#line 679 "../lib/Chai.chpl"
  forwardPropBatch_chpl(call_tmp_chpl6, inputs_chpl, &ret_tmp_chpl);
#line 679 "../lib/Chai.chpl"
  call_tmp_chpl7 = ret_tmp_chpl;
#line 679 "../lib/Chai.chpl"
  lastInputs_chpl = call_tmp_chpl7;
#line 680 "../lib/Chai.chpl"
  backwardForwardPropHelpBatch_chpl4(layers_chpl2, &lastInputs_chpl, lastDeltas_chpl, &ret_tmp_chpl2);
#line 680 "../lib/Chai.chpl"
  call_tmp_chpl8 = ret_tmp_chpl2;
#line 680 "../lib/Chai.chpl"
  deltas_chpl = call_tmp_chpl8;
#line 681 "../lib/Chai.chpl"
  call_tmp_chpl9 = &((layers_chpl2)->x0);
#line 681 "../lib/Chai.chpl"
  backwardBatch_chpl(call_tmp_chpl9, &deltas_chpl, inputs_chpl, &ret_tmp_chpl3);
#line 681 "../lib/Chai.chpl"
  call_tmp_chpl10 = ret_tmp_chpl3;
#line 681 "../lib/Chai.chpl"
  chpl__autoDestroy13(&deltas_chpl, INT64(681), INT32(65));
#line 681 "../lib/Chai.chpl"
  chpl__autoDestroy13(&lastInputs_chpl, INT64(681), INT32(65));
#line 676 "../lib/Chai.chpl"
  *(_retArg_chpl) = call_tmp_chpl10;
#line 676 "../lib/Chai.chpl"
  return;
#line 676 "../lib/Chai.chpl"
}

#line 676 "../lib/Chai.chpl"
/* Chai.chpl:676 */
#line 676 "../lib/Chai.chpl"
static void backwardForwardPropHelpBatch_chpl(_tuple_4_Conv_Conv_MaxPool_SoftMax_chpl * layers_chpl2,
#line 676 "../lib/Chai.chpl"
                                              _array_DefaultRectangularArr_1_int64_t_one_Tensor_1__real64_int64_t_chpl * inputs_chpl,
#line 676 "../lib/Chai.chpl"
                                              _array_DefaultRectangularArr_1_int64_t_one_Tensor_1__real64_int64_t_chpl * lastDeltas_chpl,
#line 676 "../lib/Chai.chpl"
                                              _array_DefaultRectangularArr_1_int64_t_one_Tensor_1__real64_int64_t_chpl * _retArg_chpl) {
#line 676 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_1_int64_t_one_Tensor_1__real64_int64_t_chpl ret_chpl;
#line 677 "../lib/Chai.chpl"
  ret_chpl = chpl__autoCopy5(lastDeltas_chpl, UINT8(false), INT64(677), INT32(65));
#line 676 "../lib/Chai.chpl"
  *(_retArg_chpl) = ret_chpl;
#line 676 "../lib/Chai.chpl"
  return;
#line 676 "../lib/Chai.chpl"
}

#line 686 "../lib/Chai.chpl"
/* Chai.chpl:686 */
#line 686 "../lib/Chai.chpl"
static _tuple_4_Conv_Conv_MaxPool_SoftMax_chpl * layers_chpl(Network__tuple_4_Conv_Conv_MaxPool_SoftMax_chpl * this_chpl31) {
#line 686 "../lib/Chai.chpl"
  _ref__tuple_4_Conv_Conv_MaxPool_SoftMax_chpl call_tmp_chpl6 = NULL;
#line 686 "../lib/Chai.chpl"
  call_tmp_chpl6 = &((this_chpl31)->_layers);
#line 686 "../lib/Chai.chpl"
  return call_tmp_chpl6;
#line 686 "../lib/Chai.chpl"
}

#line 688 "../lib/Chai.chpl"
/* Chai.chpl:688 */
#line 688 "../lib/Chai.chpl"
static void init_chpl232(Network__tuple_4_Conv_Conv_MaxPool_SoftMax_chpl * this_chpl31,
#line 688 "../lib/Chai.chpl"
                         _tuple_4__ref_Conv__ref_Conv__ref_MaxPool__ref_SoftMax_chpl * layers_chpl2) {
#line 689 "../lib/Chai.chpl"
  _tuple_4_Conv_Conv_MaxPool_SoftMax_chpl ret_chpl;
#line 689 "../lib/Chai.chpl"
  _ref_Conv_chpl read_x0_chpl = NULL;
#line 689 "../lib/Chai.chpl"
  Conv_chpl ret_chpl2;
#line 689 "../lib/Chai.chpl"
  Conv_chpl tmp_chpl;
#line 689 "../lib/Chai.chpl"
  _ref_Conv_chpl read_x1_chpl = NULL;
#line 689 "../lib/Chai.chpl"
  Conv_chpl ret_chpl3;
#line 689 "../lib/Chai.chpl"
  Conv_chpl tmp_chpl2;
#line 689 "../lib/Chai.chpl"
  _ref_MaxPool_chpl read_x2_chpl = NULL;
#line 689 "../lib/Chai.chpl"
  MaxPool_chpl elt_x2_chpl;
#line 689 "../lib/Chai.chpl"
  _ref_SoftMax_chpl read_x3_chpl = NULL;
#line 689 "../lib/Chai.chpl"
  SoftMax_chpl ret_chpl4;
#line 689 "../lib/Chai.chpl"
  SoftMax_chpl tmp_chpl3;
#line 690 "../lib/Chai.chpl"
  _ref__tuple_4_Conv_Conv_MaxPool_SoftMax_chpl call_tmp_chpl6 = NULL;
#line 690 "../lib/Chai.chpl"
  _ref_Conv_chpl call_tmp_chpl7 = NULL;
#line 689 "../lib/Chai.chpl"
  read_x0_chpl = (layers_chpl2)->x0;
#line 689 "../lib/Chai.chpl"
  init_ASSIGN__chpl27(&tmp_chpl, read_x0_chpl);
#line 689 "../lib/Chai.chpl"
  ret_chpl2 = tmp_chpl;
#line 689 "../lib/Chai.chpl"
  read_x1_chpl = (layers_chpl2)->x1;
#line 689 "../lib/Chai.chpl"
  init_ASSIGN__chpl27(&tmp_chpl2, read_x1_chpl);
#line 689 "../lib/Chai.chpl"
  ret_chpl3 = tmp_chpl2;
#line 689 "../lib/Chai.chpl"
  read_x2_chpl = (layers_chpl2)->x2;
#line 689 "../lib/Chai.chpl"
  elt_x2_chpl = *(read_x2_chpl);
#line 689 "../lib/Chai.chpl"
  read_x3_chpl = (layers_chpl2)->x3;
#line 689 "../lib/Chai.chpl"
  init_ASSIGN__chpl28(&tmp_chpl3, read_x3_chpl);
#line 689 "../lib/Chai.chpl"
  ret_chpl4 = tmp_chpl3;
#line 689 "../lib/Chai.chpl"
  (&ret_chpl)->x0 = ret_chpl2;
#line 689 "../lib/Chai.chpl"
  (&ret_chpl)->x1 = ret_chpl3;
#line 689 "../lib/Chai.chpl"
  (&ret_chpl)->x2 = elt_x2_chpl;
#line 689 "../lib/Chai.chpl"
  (&ret_chpl)->x3 = ret_chpl4;
#line 689 "../lib/Chai.chpl"
  (this_chpl31)->_layers = ret_chpl;
#line 690 "../lib/Chai.chpl"
  call_tmp_chpl6 = &((this_chpl31)->_layers);
#line 690 "../lib/Chai.chpl"
  call_tmp_chpl7 = &((call_tmp_chpl6)->x0);
#line 690 "../lib/Chai.chpl"
  (call_tmp_chpl7)->isFirstLayer = UINT8(true);
#line 688 "../lib/Chai.chpl"
  return;
#line 688 "../lib/Chai.chpl"
}

#line 704 "../lib/Chai.chpl"
/* Chai.chpl:704 */
#line 704 "../lib/Chai.chpl"
static void forwardProp_chpl4(Network__tuple_4_Conv_Conv_MaxPool_SoftMax_chpl * this_chpl31,
#line 704 "../lib/Chai.chpl"
                              Tensor_3__real64_chpl * input_chpl,
#line 704 "../lib/Chai.chpl"
                              Tensor_1__real64_chpl * _retArg_chpl) {
#line 705 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl call_tmp_chpl6;
#line 705 "../lib/Chai.chpl"
  Tensor_3__real64_chpl _formal_tmp_in__e0_elems_chpl;
#line 705 "../lib/Chai.chpl"
  Tensor_3__real64_chpl ret_chpl;
#line 705 "../lib/Chai.chpl"
  Tensor_3__real64_chpl tmp_chpl;
#line 705 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl ret_tmp_chpl;
#line 705 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl inputs_chpl;
#line 706 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_1_int64_t_one_Tensor_1__real64_int64_t_chpl call_tmp_chpl7;
#line 706 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_1_int64_t_one_Tensor_1__real64_int64_t_chpl ret_tmp_chpl2;
#line 706 "../lib/Chai.chpl"
  _ref__array_DefaultRectangularArr_1_int64_t_one_Tensor_1__real64_int64_t_chpl i_this_chpl = NULL;
#line 706 "../lib/Chai.chpl"
  _tuple_1_star_int64_t_chpl coerce_tmp_chpl3;
#line 706 "../lib/Chai.chpl"
  _ref__tuple_1_star_int64_t_chpl i_i_chpl = NULL;
#line 706 "../lib/Chai.chpl"
  DefaultRectangularArr_1_int64_t_one_Tensor_1__real64_int64_t_chpl ret_chpl2 = NULL;
#line 706 "../lib/Chai.chpl"
  DefaultRectangularArr_1_int64_t_one_Tensor_1__real64_int64_t_chpl coerce_tmp_chpl4 = NULL;
#line 706 "../lib/Chai.chpl"
  _tuple_1_star_int64_t_chpl _formal_tmp_in_ind_chpl;
#line 706 "../lib/Chai.chpl"
  int64_t _formal_tmp_ind_x0_chpl;
#line 706 "../lib/Chai.chpl"
  _ddata_Tensor_1__real64_chpl coerce_tmp_chpl5 = NULL;
#line 706 "../lib/Chai.chpl"
  _ref_Tensor_1__real64_chpl call_tmp_chpl8 = NULL;
#line 706 "../lib/Chai.chpl"
  Tensor_1__real64_chpl ret_chpl3;
#line 706 "../lib/Chai.chpl"
  Tensor_1__real64_chpl tmp_chpl2;
#line 705 "../lib/Chai.chpl"
  init_ASSIGN__chpl30(&tmp_chpl, input_chpl);
#line 705 "../lib/Chai.chpl"
  ret_chpl = tmp_chpl;
#line 705 "../lib/Chai.chpl"
  _formal_tmp_in__e0_elems_chpl = ret_chpl;
#line 705 "../lib/Chai.chpl"
  chpl__buildArrayExpr(&_formal_tmp_in__e0_elems_chpl, &ret_tmp_chpl, INT64(705), INT32(65));
#line 705 "../lib/Chai.chpl"
  call_tmp_chpl6 = ret_tmp_chpl;
#line 705 "../lib/Chai.chpl"
  inputs_chpl = call_tmp_chpl6;
#line 706 "../lib/Chai.chpl"
  forwardPropBatch_chpl4(this_chpl31, &inputs_chpl, &ret_tmp_chpl2);
#line 706 "../lib/Chai.chpl"
  call_tmp_chpl7 = ret_tmp_chpl2;
#line 706 "../lib/Chai.chpl"
  i_this_chpl = &call_tmp_chpl7;
#line 706 "../lib/Chai.chpl"
  *(coerce_tmp_chpl3 + INT64(0)) = INT64(0);
#line 706 "../lib/Chai.chpl"
  i_i_chpl = &coerce_tmp_chpl3;
#line 706 "../lib/Chai.chpl"
  ret_chpl2 = (&call_tmp_chpl7)->_instance;
#line 706 "../lib/Chai.chpl"
  checkAccess_chpl2(i_this_chpl, i_i_chpl, ret_chpl2, INT64(706), INT32(65));
#line 706 "../lib/Chai.chpl"
  coerce_tmp_chpl4 = ret_chpl2;
#line 706 "../lib/Chai.chpl"
  *(_formal_tmp_in_ind_chpl + INT64(0)) = *(coerce_tmp_chpl3 + INT64(0));
#line 706 "../lib/Chai.chpl"
  _formal_tmp_ind_x0_chpl = *(_formal_tmp_in_ind_chpl + INT64(0));
#line 706 "../lib/Chai.chpl"
  chpl_check_nil(coerce_tmp_chpl4, INT64(706), INT32(65));
#line 706 "../lib/Chai.chpl"
  coerce_tmp_chpl5 = (coerce_tmp_chpl4)->shiftedData;
#line 706 "../lib/Chai.chpl"
  call_tmp_chpl8 = (coerce_tmp_chpl5 + _formal_tmp_ind_x0_chpl);
#line 706 "../lib/Chai.chpl"
  init_ASSIGN__chpl32(&tmp_chpl2, call_tmp_chpl8);
#line 706 "../lib/Chai.chpl"
  ret_chpl3 = tmp_chpl2;
#line 706 "../lib/Chai.chpl"
  chpl__autoDestroy20(&call_tmp_chpl7, INT64(706), INT32(65));
#line 706 "../lib/Chai.chpl"
  chpl__autoDestroy13(&inputs_chpl, INT64(706), INT32(65));
#line 704 "../lib/Chai.chpl"
  *(_retArg_chpl) = ret_chpl3;
#line 704 "../lib/Chai.chpl"
  return;
#line 704 "../lib/Chai.chpl"
}

#line 715 "../lib/Chai.chpl"
/* Chai.chpl:715 */
#line 715 "../lib/Chai.chpl"
static void forwardPropBatch_chpl4(Network__tuple_4_Conv_Conv_MaxPool_SoftMax_chpl * this_chpl31,
#line 715 "../lib/Chai.chpl"
                                   _array_DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl * inputs_chpl,
#line 715 "../lib/Chai.chpl"
                                   _array_DefaultRectangularArr_1_int64_t_one_Tensor_1__real64_int64_t_chpl * _retArg_chpl) {
#line 716 "../lib/Chai.chpl"
  _ref__tuple_4_Conv_Conv_MaxPool_SoftMax_chpl call_tmp_chpl6 = NULL;
#line 716 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_1_int64_t_one_Tensor_1__real64_int64_t_chpl call_tmp_chpl7;
#line 716 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_1_int64_t_one_Tensor_1__real64_int64_t_chpl ret_tmp_chpl;
#line 716 "../lib/Chai.chpl"
  call_tmp_chpl6 = layers_chpl(this_chpl31);
#line 716 "../lib/Chai.chpl"
  forwardPropHelpBatch_chpl2(call_tmp_chpl6, inputs_chpl, &ret_tmp_chpl);
#line 716 "../lib/Chai.chpl"
  call_tmp_chpl7 = ret_tmp_chpl;
#line 715 "../lib/Chai.chpl"
  *(_retArg_chpl) = call_tmp_chpl7;
#line 715 "../lib/Chai.chpl"
  return;
#line 715 "../lib/Chai.chpl"
}

#line 718 "../lib/Chai.chpl"
/* Chai.chpl:718 */
#line 718 "../lib/Chai.chpl"
static void backwardPropBatch_chpl(Network__tuple_4_Conv_Conv_MaxPool_SoftMax_chpl * this_chpl31,
#line 718 "../lib/Chai.chpl"
                                   _array_DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl * inputs_chpl,
#line 718 "../lib/Chai.chpl"
                                   _array_DefaultRectangularArr_1_int64_t_one_Tensor_1__real64_int64_t_chpl * deltas_chpl,
#line 718 "../lib/Chai.chpl"
                                   _array_DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl * _retArg_chpl) {
#line 719 "../lib/Chai.chpl"
  _ref__tuple_4_Conv_Conv_MaxPool_SoftMax_chpl call_tmp_chpl6 = NULL;
#line 719 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl call_tmp_chpl7;
#line 719 "../lib/Chai.chpl"
  _array_DefaultRectangularArr_1_int64_t_one_Tensor_3__real64_int64_t_chpl ret_tmp_chpl;
#line 719 "../lib/Chai.chpl"
  call_tmp_chpl6 = layers_chpl(this_chpl31);
#line 719 "../lib/Chai.chpl"
  backwardForwardPropHelpBatch_chpl5(call_tmp_chpl6, inputs_chpl, deltas_chpl, &ret_tmp_chpl);
#line 719 "../lib/Chai.chpl"
  call_tmp_chpl7 = ret_tmp_chpl;
#line 718 "../lib/Chai.chpl"
  *(_retArg_chpl) = call_tmp_chpl7;
#line 718 "../lib/Chai.chpl"
  return;
#line 718 "../lib/Chai.chpl"
}

#line 722 "../lib/Chai.chpl"
/* Chai.chpl:722 */
#line 722 "../lib/Chai.chpl"
static void optimize_chpl4(Network__tuple_4_Conv_Conv_MaxPool_SoftMax_chpl * this_chpl31,
#line 722 "../lib/Chai.chpl"
                           _real64 mag_chpl) {
#line 724 "../lib/Chai.chpl"
  _ref_Conv_chpl call_tmp_chpl6 = NULL;
#line 724 "../lib/Chai.chpl"
  _ref__tuple_4_Conv_Conv_MaxPool_SoftMax_chpl _this_tmp__chpl = NULL;
#line 724 "../lib/Chai.chpl"
  _ref_Conv_chpl call_tmp_chpl7 = NULL;
#line 724 "../lib/Chai.chpl"
  _ref__tuple_4_Conv_Conv_MaxPool_SoftMax_chpl _this_tmp__chpl2 = NULL;
#line 724 "../lib/Chai.chpl"
  _ref_MaxPool_chpl call_tmp_chpl8 = NULL;
#line 724 "../lib/Chai.chpl"
  _ref__tuple_4_Conv_Conv_MaxPool_SoftMax_chpl _this_tmp__chpl3 = NULL;
#line 724 "../lib/Chai.chpl"
  _ref_SoftMax_chpl call_tmp_chpl9 = NULL;
#line 724 "../lib/Chai.chpl"
  _ref__tuple_4_Conv_Conv_MaxPool_SoftMax_chpl _this_tmp__chpl4 = NULL;
#line 723 "../lib/Chai.chpl"
  layers_chpl(this_chpl31);
#line 723 "../lib/Chai.chpl"
  layers_chpl(this_chpl31);
#line 724 "../lib/Chai.chpl"
  _this_tmp__chpl = layers_chpl(this_chpl31);
#line 724 "../lib/Chai.chpl"
  call_tmp_chpl6 = &((_this_tmp__chpl)->x0);
#line 724 "../lib/Chai.chpl"
  optimize_chpl(call_tmp_chpl6, mag_chpl);
#line 724 "../lib/Chai.chpl"
  _this_tmp__chpl2 = layers_chpl(this_chpl31);
#line 724 "../lib/Chai.chpl"
  call_tmp_chpl7 = &((_this_tmp__chpl2)->x1);
#line 724 "../lib/Chai.chpl"
  optimize_chpl(call_tmp_chpl7, mag_chpl);
#line 724 "../lib/Chai.chpl"
  _this_tmp__chpl3 = layers_chpl(this_chpl31);
#line 724 "../lib/Chai.chpl"
  call_tmp_chpl8 = &((_this_tmp__chpl3)->x2);
#line 724 "../lib/Chai.chpl"
  optimize_chpl2(call_tmp_chpl8, mag_chpl);
#line 724 "../lib/Chai.chpl"
  _this_tmp__chpl4 = layers_chpl(this_chpl31);
#line 724 "../lib/Chai.chpl"
  call_tmp_chpl9 = &((_this_tmp__chpl4)->x3);
#line 724 "../lib/Chai.chpl"
  optimize_chpl3(call_tmp_chpl9, mag_chpl);
#line 722 "../lib/Chai.chpl"
  return;
#line 722 "../lib/Chai.chpl"
}

#line 727 "../lib/Chai.chpl"
/* Chai.chpl:727 */
#line 727 "../lib/Chai.chpl"
static void resetGradients_chpl4(Network__tuple_4_Conv_Conv_MaxPool_SoftMax_chpl * this_chpl31) {
#line 729 "../lib/Chai.chpl"
  _ref_Conv_chpl call_tmp_chpl6 = NULL;
#line 729 "../lib/Chai.chpl"
  _ref__tuple_4_Conv_Conv_MaxPool_SoftMax_chpl _this_tmp__chpl = NULL;
#line 729 "../lib/Chai.chpl"
  _ref_Conv_chpl call_tmp_chpl7 = NULL;
#line 729 "../lib/Chai.chpl"
  _ref__tuple_4_Conv_Conv_MaxPool_SoftMax_chpl _this_tmp__chpl2 = NULL;
#line 729 "../lib/Chai.chpl"
  _ref_MaxPool_chpl call_tmp_chpl8 = NULL;
#line 729 "../lib/Chai.chpl"
  _ref__tuple_4_Conv_Conv_MaxPool_SoftMax_chpl _this_tmp__chpl3 = NULL;
#line 729 "../lib/Chai.chpl"
  _ref_SoftMax_chpl call_tmp_chpl9 = NULL;
#line 729 "../lib/Chai.chpl"
  _ref__tuple_4_Conv_Conv_MaxPool_SoftMax_chpl _this_tmp__chpl4 = NULL;
#line 728 "../lib/Chai.chpl"
  layers_chpl(this_chpl31);
#line 728 "../lib/Chai.chpl"
  layers_chpl(this_chpl31);
#line 729 "../lib/Chai.chpl"
  _this_tmp__chpl = layers_chpl(this_chpl31);
#line 729 "../lib/Chai.chpl"
  call_tmp_chpl6 = &((_this_tmp__chpl)->x0);
#line 729 "../lib/Chai.chpl"
  resetGradients_chpl(call_tmp_chpl6);
#line 729 "../lib/Chai.chpl"
  _this_tmp__chpl2 = layers_chpl(this_chpl31);
#line 729 "../lib/Chai.chpl"
  call_tmp_chpl7 = &((_this_tmp__chpl2)->x1);
#line 729 "../lib/Chai.chpl"
  resetGradients_chpl(call_tmp_chpl7);
#line 729 "../lib/Chai.chpl"
  _this_tmp__chpl3 = layers_chpl(this_chpl31);
#line 729 "../lib/Chai.chpl"
  call_tmp_chpl8 = &((_this_tmp__chpl3)->x2);
#line 729 "../lib/Chai.chpl"
  resetGradients_chpl2(call_tmp_chpl8);
#line 729 "../lib/Chai.chpl"
  _this_tmp__chpl4 = layers_chpl(this_chpl31);
#line 729 "../lib/Chai.chpl"
  call_tmp_chpl9 = &((_this_tmp__chpl4)->x3);
#line 729 "../lib/Chai.chpl"
  resetGradients_chpl3(call_tmp_chpl9);
#line 727 "../lib/Chai.chpl"
  return;
#line 727 "../lib/Chai.chpl"
}

#line 773 "../lib/Chai.chpl"
/* Chai.chpl:773 */
#line 773 "../lib/Chai.chpl"
static void save_chpl(Network__tuple_4_Conv_Conv_MaxPool_SoftMax_chpl * this_chpl31,
#line 773 "../lib/Chai.chpl"
                      string_chpl * path_chpl,
#line 773 "../lib/Chai.chpl"
                      Error_chpl * error_out_chpl) {
#line 778 "../lib/Chai.chpl"
  locale_chpl local_nilLocale_chpl;
#line 774 "../lib/Chai.chpl"
  file_chpl call_tmp_chpl6;
#line 774 "../lib/Chai.chpl"
  ioHintSet_chpl default_arg_hints_chpl;
#line 774 "../lib/Chai.chpl"
  Error_chpl error_chpl = NULL;
#line 774 "../lib/Chai.chpl"
  ioHintSet_chpl hints_chpl;
#line 774 "../lib/Chai.chpl"
  ioHintSet_chpl ret_tmp_chpl;
#line 774 "../lib/Chai.chpl"
  Error_chpl error_chpl2 = NULL;
#line 774 "../lib/Chai.chpl"
  file_chpl ret_tmp_chpl2;
#line 774 "../lib/Chai.chpl"
  file_chpl file_chpl2;
#line 775 "../lib/Chai.chpl"
  BinarySerializer_chpl call_tmp_chpl7;
#line 775 "../lib/Chai.chpl"
  BinarySerializer_chpl initTemp_chpl;
#line 776 "../lib/Chai.chpl"
  fileWriter_T_BinarySerializer_chpl call_tmp_chpl8;
#line 776 "../lib/Chai.chpl"
  Error_chpl error_chpl3 = NULL;
#line 776 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl region_chpl;
#line 776 "../lib/Chai.chpl"
  range_int64_t_low_one_chpl ret_tmp_chpl3;
#line 776 "../lib/Chai.chpl"
  _ref_file_chpl i_x_chpl = NULL;
#line 776 "../lib/Chai.chpl"
  ioHintSet_chpl default_arg_hints_chpl2;
#line 776 "../lib/Chai.chpl"
  Error_chpl error_chpl4 = NULL;
#line 776 "../lib/Chai.chpl"
  ioHintSet_chpl hints_chpl2;
#line 776 "../lib/Chai.chpl"
  ioHintSet_chpl ret_tmp_chpl4;
#line 776 "../lib/Chai.chpl"
  _ref_file_chpl i_x_chpl2 = NULL;
#line 776 "../lib/Chai.chpl"
  BinarySerializer_chpl _formal_tmp_in_serializer_chpl;
#line 776 "../lib/Chai.chpl"
  Error_chpl error_chpl5 = NULL;
#line 776 "../lib/Chai.chpl"
  fileWriter_T_BinarySerializer_chpl ret_tmp_chpl5;
#line 776 "../lib/Chai.chpl"
  _ref_file_chpl i_x_chpl3 = NULL;
#line 776 "../lib/Chai.chpl"
  fileWriter_T_BinarySerializer_chpl fw_chpl;
#line 778 "../lib/Chai.chpl"
  Error_chpl error_chpl6 = NULL;
#line 778 "../lib/Chai.chpl"
  _ref_fileWriter_T_BinarySerializer_chpl i_this_chpl = NULL;
#line 778 "../lib/Chai.chpl"
  locale_chpl ret_chpl;
#line 778 "../lib/Chai.chpl"
  _ref_locale_chpl call_tmp_chpl9 = NULL;
#line 778 "../lib/Chai.chpl"
  locale_chpl ret_chpl2;
#line 778 "../lib/Chai.chpl"
  chpl_bool call_tmp_chpl10;
#line 778 "../lib/Chai.chpl"
  locale_chpl call_tmp_chpl11;
#line 778 "../lib/Chai.chpl"
  chpl_localeID_t call_tmp_chpl12;
#line 778 "../lib/Chai.chpl"
  int32_t call_tmp_chpl13;
#line 778 "../lib/Chai.chpl"
  chpl_localeID_t call_tmp_chpl14;
#line 778 "../lib/Chai.chpl"
  locale_chpl call_tmp_chpl15;
#line 778 "../lib/Chai.chpl"
  locale_chpl ret_tmp_chpl6;
#line 778 "../lib/Chai.chpl"
  _ref_locale_chpl i_x_chpl4 = NULL;
#line 778 "../lib/Chai.chpl"
  locale_chpl origLocale_chpl;
#line 778 "../lib/Chai.chpl"
  _ref_locale_chpl call_tmp_chpl16 = NULL;
#line 778 "../lib/Chai.chpl"
  Error_chpl error_chpl7 = NULL;
#line 778 "../lib/Chai.chpl"
  _owned_SystemError_chpl call_tmp_chpl17;
#line 778 "../lib/Chai.chpl"
  _owned_SystemError_chpl ret_tmp_chpl7;
#line 778 "../lib/Chai.chpl"
  Error_chpl call_tmp_chpl18 = NULL;
#line 778 "../lib/Chai.chpl"
  _owned_Error_chpl coerce_tmp_chpl3;
#line 778 "../lib/Chai.chpl"
  _owned_Error_chpl ret_tmp_chpl8;
#line 778 "../lib/Chai.chpl"
  _ref__owned_SystemError_chpl i_x_chpl5 = NULL;
#line 778 "../lib/Chai.chpl"
  _ref__owned_Error_chpl2 i__retArg_chpl = NULL;
#line 778 "../lib/Chai.chpl"
  _owned_SystemError_chpl _formal_tmp_x_chpl;
#line 778 "../lib/Chai.chpl"
  _owned_Error_chpl ret_chpl3;
#line 778 "../lib/Chai.chpl"
  _ref__owned_SystemError_chpl i_this_chpl2 = NULL;
#line 778 "../lib/Chai.chpl"
  Error_chpl call_tmp_chpl19 = NULL;
#line 778 "../lib/Chai.chpl"
  _ref__owned_SystemError_chpl i_this_chpl3 = NULL;
#line 778 "../lib/Chai.chpl"
  _owned_Error_chpl call_tmp_chpl20;
#line 778 "../lib/Chai.chpl"
  _owned_Error_chpl initTemp_chpl2;
#line 778 "../lib/Chai.chpl"
  _owned_Error_chpl _formal_tmp_in_err_chpl;
#line 778 "../lib/Chai.chpl"
  _ref_locale_chpl call_tmp_chpl21 = NULL;
#line 778 "../lib/Chai.chpl"
  syserr call_tmp_chpl22;
#line 778 "../lib/Chai.chpl"
  int32_t call_tmp_chpl23;
#line 778 "../lib/Chai.chpl"
  Error_chpl error_chpl8 = NULL;
#line 778 "../lib/Chai.chpl"
  _ref_locale_chpl i_x_chpl6 = NULL;
#line 778 "../lib/Chai.chpl"
  Error_chpl error_chpl9 = NULL;
#line 778 "../lib/Chai.chpl"
  _ref_locale_chpl call_tmp_chpl24 = NULL;
#line 778 "../lib/Chai.chpl"
  _ref_locale_chpl i_x_chpl7 = NULL;
#line 778 "../lib/Chai.chpl"
  _ref_locale_chpl call_tmp_chpl25 = NULL;
#line 778 "../lib/Chai.chpl"
  _ref_locale_chpl i_x_chpl8 = NULL;
#line 778 "../lib/Chai.chpl"
  _ref_fileWriter_T_BinarySerializer_chpl i_x_chpl9 = NULL;
#line 778 "../lib/Chai.chpl"
  _ref_file_chpl i_x_chpl10 = NULL;
#line 780 "../lib/Chai.chpl"
  _ref_Conv_chpl call_tmp_chpl26 = NULL;
#line 780 "../lib/Chai.chpl"
  _ref__tuple_4_Conv_Conv_MaxPool_SoftMax_chpl _this_tmp__chpl = NULL;
#line 780 "../lib/Chai.chpl"
  Error_chpl error_chpl10 = NULL;
#line 780 "../lib/Chai.chpl"
  _ref_fileWriter_T_BinarySerializer_chpl i_x_chpl11 = NULL;
#line 780 "../lib/Chai.chpl"
  _ref_file_chpl i_x_chpl12 = NULL;
#line 780 "../lib/Chai.chpl"
  _ref_Conv_chpl call_tmp_chpl27 = NULL;
#line 780 "../lib/Chai.chpl"
  _ref__tuple_4_Conv_Conv_MaxPool_SoftMax_chpl _this_tmp__chpl2 = NULL;
#line 780 "../lib/Chai.chpl"
  Error_chpl error_chpl11 = NULL;
#line 780 "../lib/Chai.chpl"
  _ref_fileWriter_T_BinarySerializer_chpl i_x_chpl13 = NULL;
#line 780 "../lib/Chai.chpl"
  _ref_file_chpl i_x_chpl14 = NULL;
#line 780 "../lib/Chai.chpl"
  _ref_MaxPool_chpl call_tmp_chpl28 = NULL;
#line 780 "../lib/Chai.chpl"
  _ref__tuple_4_Conv_Conv_MaxPool_SoftMax_chpl _this_tmp__chpl3 = NULL;
#line 780 "../lib/Chai.chpl"
  Error_chpl error_chpl12 = NULL;
#line 780 "../lib/Chai.chpl"
  _ref_fileWriter_T_BinarySerializer_chpl i_x_chpl15 = NULL;
#line 780 "../lib/Chai.chpl"
  _ref_file_chpl i_x_chpl16 = NULL;
#line 780 "../lib/Chai.chpl"
  _ref_SoftMax_chpl call_tmp_chpl29 = NULL;
#line 780 "../lib/Chai.chpl"
  _ref__tuple_4_Conv_Conv_MaxPool_SoftMax_chpl _this_tmp__chpl4 = NULL;
#line 780 "../lib/Chai.chpl"
  Error_chpl error_chpl13 = NULL;
#line 780 "../lib/Chai.chpl"
  _ref_fileWriter_T_BinarySerializer_chpl i_x_chpl17 = NULL;
#line 780 "../lib/Chai.chpl"
  _ref_file_chpl i_x_chpl18 = NULL;
#line 782 "../lib/Chai.chpl"
  Error_chpl error_chpl14 = NULL;
#line 782 "../lib/Chai.chpl"
  _ref_fileWriter_T_BinarySerializer_chpl i_x_chpl19 = NULL;
#line 782 "../lib/Chai.chpl"
  _ref_file_chpl i_x_chpl20 = NULL;
#line 773 "../lib/Chai.chpl"
  _ref_fileWriter_T_BinarySerializer_chpl i_x_chpl21 = NULL;
#line 773 "../lib/Chai.chpl"
  _ref_file_chpl i_x_chpl22 = NULL;
#line 778 "../lib/Chai.chpl"
  local_nilLocale_chpl = nilLocale_chpl;
#line 774 "../lib/Chai.chpl"
  error_chpl = NULL;
#line 774 "../lib/Chai.chpl"
  empty_chpl2(&ret_tmp_chpl);
#line 774 "../lib/Chai.chpl"
  hints_chpl = ret_tmp_chpl;
#line 774 "../lib/Chai.chpl"
  default_arg_hints_chpl = hints_chpl;
#line 774 "../lib/Chai.chpl"
  if (error_chpl != nil) /* ZLINE: 774 ../lib/Chai.chpl */
#line 774 "../lib/Chai.chpl"
  {
#line 774 "../lib/Chai.chpl"
    *(error_out_chpl) = error_chpl;
#line 774 "../lib/Chai.chpl"
    goto _endsave_chpl;
#line 774 "../lib/Chai.chpl"
  }
#line 774 "../lib/Chai.chpl"
  error_chpl2 = NULL;
#line 774 "../lib/Chai.chpl"
  open_chpl(path_chpl, ioMode_chpl_cw_chpl, &default_arg_hints_chpl, &error_chpl2, &ret_tmp_chpl2, INT64(774), INT32(65));
#line 774 "../lib/Chai.chpl"
  call_tmp_chpl6 = ret_tmp_chpl2;
#line 774 "../lib/Chai.chpl"
  if (error_chpl2 != nil) /* ZLINE: 774 ../lib/Chai.chpl */
#line 774 "../lib/Chai.chpl"
  {
#line 774 "../lib/Chai.chpl"
    *(error_out_chpl) = error_chpl2;
#line 774 "../lib/Chai.chpl"
    goto _endsave_chpl;
#line 774 "../lib/Chai.chpl"
  }
#line 774 "../lib/Chai.chpl"
  file_chpl2 = call_tmp_chpl6;
#line 775 "../lib/Chai.chpl"
  init_chpl203(&initTemp_chpl, ioendian_chpl_big_chpl, UINT8(true));
#line 775 "../lib/Chai.chpl"
  call_tmp_chpl7 = initTemp_chpl;
#line 776 "../lib/Chai.chpl"
  error_chpl3 = NULL;
#line 776 "../lib/Chai.chpl"
  chpl_build_low_bounded_range2(INT64(0), &ret_tmp_chpl3);
#line 776 "../lib/Chai.chpl"
  region_chpl = ret_tmp_chpl3;
#line 776 "../lib/Chai.chpl"
  if (error_chpl3 != nil) /* ZLINE: 776 ../lib/Chai.chpl */
#line 776 "../lib/Chai.chpl"
  {
#line 776 "../lib/Chai.chpl"
    *(error_out_chpl) = error_chpl3;
#line 776 "../lib/Chai.chpl"
    i_x_chpl = &file_chpl2;
#line 776 "../lib/Chai.chpl"
    deinit_chpl97(i_x_chpl, INT64(776), INT32(65));
#line 776 "../lib/Chai.chpl"
    goto _endsave_chpl;
#line 776 "../lib/Chai.chpl"
  }
#line 776 "../lib/Chai.chpl"
  error_chpl4 = NULL;
#line 776 "../lib/Chai.chpl"
  empty_chpl2(&ret_tmp_chpl4);
#line 776 "../lib/Chai.chpl"
  hints_chpl2 = ret_tmp_chpl4;
#line 776 "../lib/Chai.chpl"
  default_arg_hints_chpl2 = hints_chpl2;
#line 776 "../lib/Chai.chpl"
  if (error_chpl4 != nil) /* ZLINE: 776 ../lib/Chai.chpl */
#line 776 "../lib/Chai.chpl"
  {
#line 776 "../lib/Chai.chpl"
    *(error_out_chpl) = error_chpl4;
#line 776 "../lib/Chai.chpl"
    i_x_chpl2 = &file_chpl2;
#line 776 "../lib/Chai.chpl"
    deinit_chpl97(i_x_chpl2, INT64(776), INT32(65));
#line 776 "../lib/Chai.chpl"
    goto _endsave_chpl;
#line 776 "../lib/Chai.chpl"
  }
#line 776 "../lib/Chai.chpl"
  _formal_tmp_in_serializer_chpl = call_tmp_chpl7;
#line 776 "../lib/Chai.chpl"
  error_chpl5 = NULL;
#line 776 "../lib/Chai.chpl"
  writer_chpl2(&file_chpl2, region_chpl, &default_arg_hints_chpl2, &_formal_tmp_in_serializer_chpl, &error_chpl5, &ret_tmp_chpl5, INT64(776), INT32(65));
#line 776 "../lib/Chai.chpl"
  call_tmp_chpl8 = ret_tmp_chpl5;
#line 776 "../lib/Chai.chpl"
  if (error_chpl5 != nil) /* ZLINE: 776 ../lib/Chai.chpl */
#line 776 "../lib/Chai.chpl"
  {
#line 776 "../lib/Chai.chpl"
    *(error_out_chpl) = error_chpl5;
#line 776 "../lib/Chai.chpl"
    i_x_chpl3 = &file_chpl2;
#line 776 "../lib/Chai.chpl"
    deinit_chpl97(i_x_chpl3, INT64(776), INT32(65));
#line 776 "../lib/Chai.chpl"
    goto _endsave_chpl;
#line 776 "../lib/Chai.chpl"
  }
#line 776 "../lib/Chai.chpl"
  fw_chpl = call_tmp_chpl8;
#line 778 "../lib/Chai.chpl"
  layers_chpl(this_chpl31);
#line 778 "../lib/Chai.chpl"
  error_chpl6 = NULL;
#line 778 "../lib/Chai.chpl"
  i_this_chpl = &fw_chpl;
#line 778 "../lib/Chai.chpl"
  call_tmp_chpl9 = &((&fw_chpl)->_readWriteThisFromLocale);
#line 778 "../lib/Chai.chpl"
  init_ASSIGN__chpl(&ret_chpl2, call_tmp_chpl9);
#line 778 "../lib/Chai.chpl"
  call_tmp_chpl10 = chpl___EQUALS_(&ret_chpl2, &local_nilLocale_chpl, INT64(778), INT32(65));
#line 778 "../lib/Chai.chpl"
  if (call_tmp_chpl10) /* ZLINE: 778 ../lib/Chai.chpl */
#line 778 "../lib/Chai.chpl"
  {
#line 778 "../lib/Chai.chpl"
    call_tmp_chpl13 = get_chpl_nodeID();
#line 778 "../lib/Chai.chpl"
    call_tmp_chpl14 = chpl_rt_buildLocaleID(call_tmp_chpl13, c_sublocid_any);
#line 778 "../lib/Chai.chpl"
    call_tmp_chpl12 = call_tmp_chpl14;
#line 778 "../lib/Chai.chpl"
    chpl_localeID_to_locale(&call_tmp_chpl12, &ret_tmp_chpl6, INT64(778), INT32(65));
#line 778 "../lib/Chai.chpl"
    call_tmp_chpl15 = ret_tmp_chpl6;
#line 778 "../lib/Chai.chpl"
    call_tmp_chpl11 = call_tmp_chpl15;
#line 778 "../lib/Chai.chpl"
    chpl___ASSIGN_9(&ret_chpl2, &call_tmp_chpl11);
#line 778 "../lib/Chai.chpl"
    i_x_chpl4 = &call_tmp_chpl11;
#line 778 "../lib/Chai.chpl"
    deinit_chpl62(i_x_chpl4);
#line 778 "../lib/Chai.chpl"
  }
#line 778 "../lib/Chai.chpl"
  ret_chpl = ret_chpl2;
#line 778 "../lib/Chai.chpl"
  origLocale_chpl = ret_chpl;
#line 778 "../lib/Chai.chpl"
  call_tmp_chpl16 = &((&fw_chpl)->_home);
#line 778 "../lib/Chai.chpl"
  if (((RootClass_chpl)((call_tmp_chpl16)->_instance)) == nil) /* ZLINE: 778 ../lib/Chai.chpl */
#line 778 "../lib/Chai.chpl"
  {
#line 778 "../lib/Chai.chpl"
    nilCheckHalt_chpl(&_str_literal_52_chpl, INT64(778), INT32(65));
#line 778 "../lib/Chai.chpl"
  }
#line 778 "../lib/Chai.chpl"
  error_chpl7 = NULL;
#line 778 "../lib/Chai.chpl"
  qio_int_to_err(((int32_t)(INT64(0))));
#line 778 "../lib/Chai.chpl"
  if ((&fw_chpl)->_channel_internal == nil) /* ZLINE: 778 ../lib/Chai.chpl */
#line 778 "../lib/Chai.chpl"
  {
#line 778 "../lib/Chai.chpl"
    createSystemError_chpl2(((int64_t)(EINVAL)), &_str_literal_1920_chpl, &ret_tmp_chpl7, INT64(778), INT32(65));
#line 778 "../lib/Chai.chpl"
    call_tmp_chpl17 = ret_tmp_chpl7;
#line 778 "../lib/Chai.chpl"
    i_x_chpl5 = &call_tmp_chpl17;
#line 778 "../lib/Chai.chpl"
    i__retArg_chpl = &ret_tmp_chpl8;
#line 778 "../lib/Chai.chpl"
    _formal_tmp_x_chpl = *(i_x_chpl5);
#line 778 "../lib/Chai.chpl"
    i_this_chpl2 = &_formal_tmp_x_chpl;
#line 778 "../lib/Chai.chpl"
    call_tmp_chpl19 = ((Error_chpl)((i_this_chpl2)->chpl_p));
#line 778 "../lib/Chai.chpl"
    i_this_chpl3 = &_formal_tmp_x_chpl;
#line 778 "../lib/Chai.chpl"
    (i_this_chpl3)->chpl_p = ((SystemError_chpl)(nil));
#line 778 "../lib/Chai.chpl"
    if (((RootClass_chpl)(call_tmp_chpl19)) == nil) /* ZLINE: 778 ../lib/Chai.chpl */
#line 778 "../lib/Chai.chpl"
    {
#line 778 "../lib/Chai.chpl"
      nilCheckHalt_chpl(&_str_literal_52_chpl, INT64(778), INT32(65));
#line 778 "../lib/Chai.chpl"
    }
#line 778 "../lib/Chai.chpl"
    init_chpl188(&initTemp_chpl2, call_tmp_chpl19);
#line 778 "../lib/Chai.chpl"
    call_tmp_chpl20 = initTemp_chpl2;
#line 778 "../lib/Chai.chpl"
    ret_chpl3 = call_tmp_chpl20;
#line 778 "../lib/Chai.chpl"
    chpl__autoDestroy54(&_formal_tmp_x_chpl, INT64(778), INT32(65));
#line 778 "../lib/Chai.chpl"
    *(i__retArg_chpl) = ret_chpl3;
#line 778 "../lib/Chai.chpl"
    coerce_tmp_chpl3 = ret_tmp_chpl8;
#line 778 "../lib/Chai.chpl"
    _formal_tmp_in_err_chpl = coerce_tmp_chpl3;
#line 778 "../lib/Chai.chpl"
    call_tmp_chpl18 = chpl_fix_thrown_error(&_formal_tmp_in_err_chpl, INT64(778), INT32(65));
#line 778 "../lib/Chai.chpl"
    error_chpl7 = call_tmp_chpl18;
#line 778 "../lib/Chai.chpl"
    goto _endlock_chpl;
#line 778 "../lib/Chai.chpl"
  }
#line 778 "../lib/Chai.chpl"
  call_tmp_chpl21 = &((&fw_chpl)->_home);
#line 778 "../lib/Chai.chpl"
  if (((RootClass_chpl)((call_tmp_chpl21)->_instance)) == nil) /* ZLINE: 778 ../lib/Chai.chpl */
#line 778 "../lib/Chai.chpl"
  {
#line 778 "../lib/Chai.chpl"
    nilCheckHalt_chpl(&_str_literal_52_chpl, INT64(778), INT32(65));
#line 778 "../lib/Chai.chpl"
  }
#line 778 "../lib/Chai.chpl"
  call_tmp_chpl22 = qio_channel_lock((&fw_chpl)->_channel_internal);
#line 778 "../lib/Chai.chpl"
  call_tmp_chpl23 = qio_err_iserr(call_tmp_chpl22);
#line 778 "../lib/Chai.chpl"
  if (call_tmp_chpl23 != INT32(0)) /* ZLINE: 778 ../lib/Chai.chpl */
#line 778 "../lib/Chai.chpl"
  {
#line 778 "../lib/Chai.chpl"
    error_chpl8 = NULL;
#line 778 "../lib/Chai.chpl"
    _ch_ioerror_chpl3(i_this_chpl, call_tmp_chpl22, &_str_literal_1919_chpl, &error_chpl8, INT64(778), INT32(65));
#line 778 "../lib/Chai.chpl"
    if (error_chpl8 != nil) /* ZLINE: 778 ../lib/Chai.chpl */
#line 778 "../lib/Chai.chpl"
    {
#line 778 "../lib/Chai.chpl"
      goto handler_chpl;
#line 778 "../lib/Chai.chpl"
    }
#line 778 "../lib/Chai.chpl"
    handler_chpl:;
#line 778 "../lib/Chai.chpl"
    if (error_chpl8 != nil) /* ZLINE: 778 ../lib/Chai.chpl */
#line 778 "../lib/Chai.chpl"
    {
#line 778 "../lib/Chai.chpl"
      error_chpl7 = error_chpl8;
#line 778 "../lib/Chai.chpl"
      goto _endlock_chpl;
#line 778 "../lib/Chai.chpl"
    }
#line 778 "../lib/Chai.chpl"
  }
#line 778 "../lib/Chai.chpl"
  _endlock_chpl:;
#line 778 "../lib/Chai.chpl"
  if (error_chpl7 != nil) /* ZLINE: 778 ../lib/Chai.chpl */
#line 778 "../lib/Chai.chpl"
  {
#line 778 "../lib/Chai.chpl"
    goto handler_chpl2;
#line 778 "../lib/Chai.chpl"
  }
#line 778 "../lib/Chai.chpl"
  handler_chpl2:;
#line 778 "../lib/Chai.chpl"
  if (error_chpl7 != nil) /* ZLINE: 778 ../lib/Chai.chpl */
#line 778 "../lib/Chai.chpl"
  {
#line 778 "../lib/Chai.chpl"
    error_chpl6 = error_chpl7;
#line 778 "../lib/Chai.chpl"
    i_x_chpl6 = &origLocale_chpl;
#line 778 "../lib/Chai.chpl"
    deinit_chpl62(i_x_chpl6);
#line 778 "../lib/Chai.chpl"
    goto _endwrite_chpl;
#line 778 "../lib/Chai.chpl"
  }
#line 778 "../lib/Chai.chpl"
  error_chpl9 = NULL;
#line 778 "../lib/Chai.chpl"
  _serializeOne_chpl2(i_this_chpl, INT64(4), &origLocale_chpl, &error_chpl9, INT64(778), INT32(65));
#line 778 "../lib/Chai.chpl"
  if (error_chpl9 != nil) /* ZLINE: 778 ../lib/Chai.chpl */
#line 778 "../lib/Chai.chpl"
  {
#line 778 "../lib/Chai.chpl"
    error_chpl6 = error_chpl9;
#line 778 "../lib/Chai.chpl"
    call_tmp_chpl24 = &((&fw_chpl)->_home);
#line 778 "../lib/Chai.chpl"
    if (((RootClass_chpl)((call_tmp_chpl24)->_instance)) == nil) /* ZLINE: 778 ../lib/Chai.chpl */
#line 778 "../lib/Chai.chpl"
    {
#line 778 "../lib/Chai.chpl"
      nilCheckHalt_chpl(&_str_literal_52_chpl, INT64(778), INT32(65));
#line 778 "../lib/Chai.chpl"
    }
#line 778 "../lib/Chai.chpl"
    qio_channel_unlock((&fw_chpl)->_channel_internal);
#line 778 "../lib/Chai.chpl"
    i_x_chpl7 = &origLocale_chpl;
#line 778 "../lib/Chai.chpl"
    deinit_chpl62(i_x_chpl7);
#line 778 "../lib/Chai.chpl"
    goto _endwrite_chpl;
#line 778 "../lib/Chai.chpl"
  }
#line 778 "../lib/Chai.chpl"
  call_tmp_chpl25 = &((&fw_chpl)->_home);
#line 778 "../lib/Chai.chpl"
  if (((RootClass_chpl)((call_tmp_chpl25)->_instance)) == nil) /* ZLINE: 778 ../lib/Chai.chpl */
#line 778 "../lib/Chai.chpl"
  {
#line 778 "../lib/Chai.chpl"
    nilCheckHalt_chpl(&_str_literal_52_chpl, INT64(778), INT32(65));
#line 778 "../lib/Chai.chpl"
  }
#line 778 "../lib/Chai.chpl"
  qio_channel_unlock((&fw_chpl)->_channel_internal);
#line 778 "../lib/Chai.chpl"
  i_x_chpl8 = &origLocale_chpl;
#line 778 "../lib/Chai.chpl"
  deinit_chpl62(i_x_chpl8);
#line 778 "../lib/Chai.chpl"
  _endwrite_chpl:;
#line 778 "../lib/Chai.chpl"
  if (error_chpl6 != nil) /* ZLINE: 778 ../lib/Chai.chpl */
#line 778 "../lib/Chai.chpl"
  {
#line 778 "../lib/Chai.chpl"
    *(error_out_chpl) = error_chpl6;
#line 778 "../lib/Chai.chpl"
    i_x_chpl9 = &fw_chpl;
#line 778 "../lib/Chai.chpl"
    deinit_chpl101(i_x_chpl9, INT64(778), INT32(65));
#line 778 "../lib/Chai.chpl"
    i_x_chpl10 = &file_chpl2;
#line 778 "../lib/Chai.chpl"
    deinit_chpl97(i_x_chpl10, INT64(778), INT32(65));
#line 778 "../lib/Chai.chpl"
    goto _endsave_chpl;
#line 778 "../lib/Chai.chpl"
  }
#line 779 "../lib/Chai.chpl"
  layers_chpl(this_chpl31);
#line 779 "../lib/Chai.chpl"
  layers_chpl(this_chpl31);
#line 780 "../lib/Chai.chpl"
  _this_tmp__chpl = layers_chpl(this_chpl31);
#line 780 "../lib/Chai.chpl"
  call_tmp_chpl26 = &((_this_tmp__chpl)->x0);
#line 780 "../lib/Chai.chpl"
  error_chpl10 = NULL;
#line 780 "../lib/Chai.chpl"
  write_chpl2(call_tmp_chpl26, &fw_chpl, &error_chpl10);
#line 780 "../lib/Chai.chpl"
  if (error_chpl10 != nil) /* ZLINE: 780 ../lib/Chai.chpl */
#line 780 "../lib/Chai.chpl"
  {
#line 780 "../lib/Chai.chpl"
    *(error_out_chpl) = error_chpl10;
#line 780 "../lib/Chai.chpl"
    i_x_chpl11 = &fw_chpl;
#line 780 "../lib/Chai.chpl"
    deinit_chpl101(i_x_chpl11, INT64(780), INT32(65));
#line 780 "../lib/Chai.chpl"
    i_x_chpl12 = &file_chpl2;
#line 780 "../lib/Chai.chpl"
    deinit_chpl97(i_x_chpl12, INT64(780), INT32(65));
#line 780 "../lib/Chai.chpl"
    goto _endsave_chpl;
#line 780 "../lib/Chai.chpl"
  }
#line 780 "../lib/Chai.chpl"
  _this_tmp__chpl2 = layers_chpl(this_chpl31);
#line 780 "../lib/Chai.chpl"
  call_tmp_chpl27 = &((_this_tmp__chpl2)->x1);
#line 780 "../lib/Chai.chpl"
  error_chpl11 = NULL;
#line 780 "../lib/Chai.chpl"
  write_chpl2(call_tmp_chpl27, &fw_chpl, &error_chpl11);
#line 780 "../lib/Chai.chpl"
  if (error_chpl11 != nil) /* ZLINE: 780 ../lib/Chai.chpl */
#line 780 "../lib/Chai.chpl"
  {
#line 780 "../lib/Chai.chpl"
    *(error_out_chpl) = error_chpl11;
#line 780 "../lib/Chai.chpl"
    i_x_chpl13 = &fw_chpl;
#line 780 "../lib/Chai.chpl"
    deinit_chpl101(i_x_chpl13, INT64(780), INT32(65));
#line 780 "../lib/Chai.chpl"
    i_x_chpl14 = &file_chpl2;
#line 780 "../lib/Chai.chpl"
    deinit_chpl97(i_x_chpl14, INT64(780), INT32(65));
#line 780 "../lib/Chai.chpl"
    goto _endsave_chpl;
#line 780 "../lib/Chai.chpl"
  }
#line 780 "../lib/Chai.chpl"
  _this_tmp__chpl3 = layers_chpl(this_chpl31);
#line 780 "../lib/Chai.chpl"
  call_tmp_chpl28 = &((_this_tmp__chpl3)->x2);
#line 780 "../lib/Chai.chpl"
  error_chpl12 = NULL;
#line 780 "../lib/Chai.chpl"
  write_chpl3(call_tmp_chpl28, &fw_chpl, &error_chpl12);
#line 780 "../lib/Chai.chpl"
  if (error_chpl12 != nil) /* ZLINE: 780 ../lib/Chai.chpl */
#line 780 "../lib/Chai.chpl"
  {
#line 780 "../lib/Chai.chpl"
    *(error_out_chpl) = error_chpl12;
#line 780 "../lib/Chai.chpl"
    i_x_chpl15 = &fw_chpl;
#line 780 "../lib/Chai.chpl"
    deinit_chpl101(i_x_chpl15, INT64(780), INT32(65));
#line 780 "../lib/Chai.chpl"
    i_x_chpl16 = &file_chpl2;
#line 780 "../lib/Chai.chpl"
    deinit_chpl97(i_x_chpl16, INT64(780), INT32(65));
#line 780 "../lib/Chai.chpl"
    goto _endsave_chpl;
#line 780 "../lib/Chai.chpl"
  }
#line 780 "../lib/Chai.chpl"
  _this_tmp__chpl4 = layers_chpl(this_chpl31);
#line 780 "../lib/Chai.chpl"
  call_tmp_chpl29 = &((_this_tmp__chpl4)->x3);
#line 780 "../lib/Chai.chpl"
  error_chpl13 = NULL;
#line 780 "../lib/Chai.chpl"
  write_chpl4(call_tmp_chpl29, &fw_chpl, &error_chpl13);
#line 780 "../lib/Chai.chpl"
  if (error_chpl13 != nil) /* ZLINE: 780 ../lib/Chai.chpl */
#line 780 "../lib/Chai.chpl"
  {
#line 780 "../lib/Chai.chpl"
    *(error_out_chpl) = error_chpl13;
#line 780 "../lib/Chai.chpl"
    i_x_chpl17 = &fw_chpl;
#line 780 "../lib/Chai.chpl"
    deinit_chpl101(i_x_chpl17, INT64(780), INT32(65));
#line 780 "../lib/Chai.chpl"
    i_x_chpl18 = &file_chpl2;
#line 780 "../lib/Chai.chpl"
    deinit_chpl97(i_x_chpl18, INT64(780), INT32(65));
#line 780 "../lib/Chai.chpl"
    goto _endsave_chpl;
#line 780 "../lib/Chai.chpl"
  }
#line 782 "../lib/Chai.chpl"
  error_chpl14 = NULL;
#line 782 "../lib/Chai.chpl"
  close_chpl6(&fw_chpl, &error_chpl14, INT64(782), INT32(65));
#line 782 "../lib/Chai.chpl"
  if (error_chpl14 != nil) /* ZLINE: 782 ../lib/Chai.chpl */
#line 782 "../lib/Chai.chpl"
  {
#line 782 "../lib/Chai.chpl"
    *(error_out_chpl) = error_chpl14;
#line 782 "../lib/Chai.chpl"
    i_x_chpl19 = &fw_chpl;
#line 782 "../lib/Chai.chpl"
    deinit_chpl101(i_x_chpl19, INT64(782), INT32(65));
#line 782 "../lib/Chai.chpl"
    i_x_chpl20 = &file_chpl2;
#line 782 "../lib/Chai.chpl"
    deinit_chpl97(i_x_chpl20, INT64(782), INT32(65));
#line 782 "../lib/Chai.chpl"
    goto _endsave_chpl;
#line 782 "../lib/Chai.chpl"
  }
#line 773 "../lib/Chai.chpl"
  i_x_chpl21 = &fw_chpl;
#line 773 "../lib/Chai.chpl"
  deinit_chpl101(i_x_chpl21, INT64(773), INT32(65));
#line 773 "../lib/Chai.chpl"
  i_x_chpl22 = &file_chpl2;
#line 773 "../lib/Chai.chpl"
  deinit_chpl97(i_x_chpl22, INT64(773), INT32(65));
#line 773 "../lib/Chai.chpl"
  _endsave_chpl:;
#line 773 "../lib/Chai.chpl"
  return;
#line 773 "../lib/Chai.chpl"
}

#line 797 "../lib/Chai.chpl"
/* Chai.chpl:797 */
#line 797 "../lib/Chai.chpl"
static void signature_chpl4(Network__tuple_4_Conv_Conv_MaxPool_SoftMax_chpl * this_chpl31,
#line 797 "../lib/Chai.chpl"
                            string_chpl * _retArg_chpl) {
#line 797 "../lib/Chai.chpl"
  string_chpl ret_chpl;
#line 798 "../lib/Chai.chpl"
  string_chpl sig_chpl;
#line 798 "../lib/Chai.chpl"
  string_chpl inlineImm_chpl;
#line 798 "../lib/Chai.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 798 "../lib/Chai.chpl"
  string_chpl ret_chpl2;
#line 798 "../lib/Chai.chpl"
  string_chpl tmp_chpl;
#line 800 "../lib/Chai.chpl"
  _ref_Conv_chpl call_tmp_chpl6 = NULL;
#line 800 "../lib/Chai.chpl"
  _ref__tuple_4_Conv_Conv_MaxPool_SoftMax_chpl _this_tmp__chpl = NULL;
#line 800 "../lib/Chai.chpl"
  string_chpl call_tmp_chpl7;
#line 800 "../lib/Chai.chpl"
  string_chpl ret_tmp_chpl;
#line 800 "../lib/Chai.chpl"
  _ref_string_chpl i_x_chpl2 = NULL;
#line 800 "../lib/Chai.chpl"
  _ref_Conv_chpl call_tmp_chpl8 = NULL;
#line 800 "../lib/Chai.chpl"
  _ref__tuple_4_Conv_Conv_MaxPool_SoftMax_chpl _this_tmp__chpl2 = NULL;
#line 800 "../lib/Chai.chpl"
  string_chpl call_tmp_chpl9;
#line 800 "../lib/Chai.chpl"
  string_chpl ret_tmp_chpl2;
#line 800 "../lib/Chai.chpl"
  _ref_string_chpl i_x_chpl3 = NULL;
#line 800 "../lib/Chai.chpl"
  _ref_MaxPool_chpl call_tmp_chpl10 = NULL;
#line 800 "../lib/Chai.chpl"
  _ref__tuple_4_Conv_Conv_MaxPool_SoftMax_chpl _this_tmp__chpl3 = NULL;
#line 800 "../lib/Chai.chpl"
  string_chpl call_tmp_chpl11;
#line 800 "../lib/Chai.chpl"
  string_chpl ret_tmp_chpl3;
#line 800 "../lib/Chai.chpl"
  _ref_string_chpl i_x_chpl4 = NULL;
#line 800 "../lib/Chai.chpl"
  _ref_SoftMax_chpl call_tmp_chpl12 = NULL;
#line 800 "../lib/Chai.chpl"
  _ref__tuple_4_Conv_Conv_MaxPool_SoftMax_chpl _this_tmp__chpl4 = NULL;
#line 800 "../lib/Chai.chpl"
  string_chpl call_tmp_chpl13;
#line 800 "../lib/Chai.chpl"
  string_chpl ret_tmp_chpl4;
#line 800 "../lib/Chai.chpl"
  _ref_string_chpl i_x_chpl5 = NULL;
#line 798 "../lib/Chai.chpl"
  inlineImm_chpl = _str_literal_848_chpl;
#line 798 "../lib/Chai.chpl"
  i_x_chpl = &inlineImm_chpl;
#line 798 "../lib/Chai.chpl"
  init_ASSIGN__chpl25(&tmp_chpl, i_x_chpl, INT64(798), INT32(65));
#line 798 "../lib/Chai.chpl"
  ret_chpl2 = tmp_chpl;
#line 798 "../lib/Chai.chpl"
  sig_chpl = ret_chpl2;
#line 799 "../lib/Chai.chpl"
  layers_chpl(this_chpl31);
#line 799 "../lib/Chai.chpl"
  layers_chpl(this_chpl31);
#line 800 "../lib/Chai.chpl"
  _this_tmp__chpl = layers_chpl(this_chpl31);
#line 800 "../lib/Chai.chpl"
  call_tmp_chpl6 = &((_this_tmp__chpl)->x0);
#line 800 "../lib/Chai.chpl"
  signature_chpl(call_tmp_chpl6, &ret_tmp_chpl);
#line 800 "../lib/Chai.chpl"
  call_tmp_chpl7 = ret_tmp_chpl;
#line 800 "../lib/Chai.chpl"
  chpl___PLUS__ASSIGN_(&sig_chpl, &call_tmp_chpl7, INT64(800), INT32(65));
#line 800 "../lib/Chai.chpl"
  i_x_chpl2 = &call_tmp_chpl7;
#line 800 "../lib/Chai.chpl"
  deinit_chpl95(i_x_chpl2, INT64(800), INT32(65));
#line 801 "../lib/Chai.chpl"
  layers_chpl(this_chpl31);
#line 801 "../lib/Chai.chpl"
  chpl___PLUS__ASSIGN_(&sig_chpl, &_str_literal_472_chpl, INT64(801), INT32(65));
#line 800 "../lib/Chai.chpl"
  _this_tmp__chpl2 = layers_chpl(this_chpl31);
#line 800 "../lib/Chai.chpl"
  call_tmp_chpl8 = &((_this_tmp__chpl2)->x1);
#line 800 "../lib/Chai.chpl"
  signature_chpl(call_tmp_chpl8, &ret_tmp_chpl2);
#line 800 "../lib/Chai.chpl"
  call_tmp_chpl9 = ret_tmp_chpl2;
#line 800 "../lib/Chai.chpl"
  chpl___PLUS__ASSIGN_(&sig_chpl, &call_tmp_chpl9, INT64(800), INT32(65));
#line 800 "../lib/Chai.chpl"
  i_x_chpl3 = &call_tmp_chpl9;
#line 800 "../lib/Chai.chpl"
  deinit_chpl95(i_x_chpl3, INT64(800), INT32(65));
#line 801 "../lib/Chai.chpl"
  layers_chpl(this_chpl31);
#line 801 "../lib/Chai.chpl"
  chpl___PLUS__ASSIGN_(&sig_chpl, &_str_literal_472_chpl, INT64(801), INT32(65));
#line 800 "../lib/Chai.chpl"
  _this_tmp__chpl3 = layers_chpl(this_chpl31);
#line 800 "../lib/Chai.chpl"
  call_tmp_chpl10 = &((_this_tmp__chpl3)->x2);
#line 800 "../lib/Chai.chpl"
  signature_chpl2(call_tmp_chpl10, &ret_tmp_chpl3);
#line 800 "../lib/Chai.chpl"
  call_tmp_chpl11 = ret_tmp_chpl3;
#line 800 "../lib/Chai.chpl"
  chpl___PLUS__ASSIGN_(&sig_chpl, &call_tmp_chpl11, INT64(800), INT32(65));
#line 800 "../lib/Chai.chpl"
  i_x_chpl4 = &call_tmp_chpl11;
#line 800 "../lib/Chai.chpl"
  deinit_chpl95(i_x_chpl4, INT64(800), INT32(65));
#line 801 "../lib/Chai.chpl"
  layers_chpl(this_chpl31);
#line 801 "../lib/Chai.chpl"
  chpl___PLUS__ASSIGN_(&sig_chpl, &_str_literal_472_chpl, INT64(801), INT32(65));
#line 800 "../lib/Chai.chpl"
  _this_tmp__chpl4 = layers_chpl(this_chpl31);
#line 800 "../lib/Chai.chpl"
  call_tmp_chpl12 = &((_this_tmp__chpl4)->x3);
#line 800 "../lib/Chai.chpl"
  signature_chpl3(call_tmp_chpl12, &ret_tmp_chpl4);
#line 800 "../lib/Chai.chpl"
  call_tmp_chpl13 = ret_tmp_chpl4;
#line 800 "../lib/Chai.chpl"
  chpl___PLUS__ASSIGN_(&sig_chpl, &call_tmp_chpl13, INT64(800), INT32(65));
#line 800 "../lib/Chai.chpl"
  i_x_chpl5 = &call_tmp_chpl13;
#line 800 "../lib/Chai.chpl"
  deinit_chpl95(i_x_chpl5, INT64(800), INT32(65));
#line 801 "../lib/Chai.chpl"
  layers_chpl(this_chpl31);
#line 803 "../lib/Chai.chpl"
  chpl___PLUS__ASSIGN_(&sig_chpl, &_str_literal_851_chpl, INT64(803), INT32(65));
#line 804 "../lib/Chai.chpl"
  ret_chpl = sig_chpl;
#line 797 "../lib/Chai.chpl"
  *(_retArg_chpl) = ret_chpl;
#line 797 "../lib/Chai.chpl"
  return;
#line 797 "../lib/Chai.chpl"
}

