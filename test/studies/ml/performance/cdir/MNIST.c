#line 1 "../lib/MNIST.chpl"
/* MNIST.chpl:1 */
#line 1 "../lib/MNIST.chpl"
static void chpl__init_MNIST(int64_t _ln_chpl,
#line 1 "../lib/MNIST.chpl"
                             int32_t _fn_chpl) {
#line 1 "../lib/MNIST.chpl"
  _ref_int32_t_chpl refIndentLevel_chpl = NULL;
#line 1 "../lib/MNIST.chpl"
  if (chpl__init_MNIST_p) /* ZLINE: 1 ../lib/MNIST.chpl */
#line 1 "../lib/MNIST.chpl"
  {
#line 1 "../lib/MNIST.chpl"
    goto _exit_chpl__init_MNIST_chpl;
#line 1 "../lib/MNIST.chpl"
  }
#line 1 "../lib/MNIST.chpl"
  printModuleInit_chpl("%*s\n", "MNIST", INT64(5), INT64(1), INT32(71));
#line 1 "../lib/MNIST.chpl"
  refIndentLevel_chpl = &moduleInitLevel_chpl;
#line 1 "../lib/MNIST.chpl"
  *(refIndentLevel_chpl) += INT64(1);
#line 1 "../lib/MNIST.chpl"
  chpl__init_MNIST_p = UINT8(true);
#line 1 "../lib/MNIST.chpl"
  {
#line 1 "../lib/MNIST.chpl"
    chpl__init_IO(INT64(1), INT32(71));
#line 1 "../lib/MNIST.chpl"
  }
#line 1 "../lib/MNIST.chpl"
  *(refIndentLevel_chpl) -= INT64(1);
#line 1 "../lib/MNIST.chpl"
  _exit_chpl__init_MNIST_chpl:;
#line 1 "../lib/MNIST.chpl"
  return;
#line 1 "../lib/MNIST.chpl"
}

#line 6 "../lib/MNIST.chpl"
/* MNIST.chpl:6 */
#line 6 "../lib/MNIST.chpl"
static void loadImages_chpl(int64_t num_chpl,
#line 6 "../lib/MNIST.chpl"
                            string_chpl * fileName_chpl,
#line 6 "../lib/MNIST.chpl"
                            _array_DefaultRectangularArr_1_int64_t_one__array_DefaultRectangularArr_2_int64_t_one__real64_int64_t_int64_t_chpl * _retArg_chpl) {
#line 44 "../lib/MNIST.chpl"
  int64_t local_dataParMinGranularity_chpl;
#line 44 "../lib/MNIST.chpl"
  chpl_bool local_dataParIgnoreRunningTasks_chpl;
#line 44 "../lib/MNIST.chpl"
  int64_t local_dataParTasksPerLocale_chpl;
#line 6 "../lib/MNIST.chpl"
  _array_DefaultRectangularArr_1_int64_t_one__array_DefaultRectangularArr_2_int64_t_one__real64_int64_t_int64_t_chpl ret_chpl;
#line 8 "../lib/MNIST.chpl"
  BinaryDeserializer_chpl call_tmp_chpl6;
#line 8 "../lib/MNIST.chpl"
  BinaryDeserializer_chpl initTemp_chpl;
#line 10 "../lib/MNIST.chpl"
  fileReader_T_BinaryDeserializer_chpl call_tmp_chpl7;
#line 10 "../lib/MNIST.chpl"
  Error_chpl error_chpl = NULL;
#line 10 "../lib/MNIST.chpl"
  range_int64_t_low_one_chpl region_chpl;
#line 10 "../lib/MNIST.chpl"
  range_int64_t_low_one_chpl ret_tmp_chpl;
#line 10 "../lib/MNIST.chpl"
  ioHintSet_chpl default_arg_hints_chpl;
#line 10 "../lib/MNIST.chpl"
  Error_chpl error_chpl2 = NULL;
#line 10 "../lib/MNIST.chpl"
  ioHintSet_chpl hints_chpl;
#line 10 "../lib/MNIST.chpl"
  ioHintSet_chpl ret_tmp_chpl2;
#line 10 "../lib/MNIST.chpl"
  BinaryDeserializer_chpl _formal_tmp_in_deserializer_chpl;
#line 10 "../lib/MNIST.chpl"
  Error_chpl error_chpl3 = NULL;
#line 10 "../lib/MNIST.chpl"
  fileReader_T_BinaryDeserializer_chpl ret_tmp_chpl3;
#line 10 "../lib/MNIST.chpl"
  fileReader_T_BinaryDeserializer_chpl fr_chpl;
#line 12 "../lib/MNIST.chpl"
  int32_t call_tmp_chpl8;
#line 12 "../lib/MNIST.chpl"
  Error_chpl error_chpl4 = NULL;
#line 19 "../lib/MNIST.chpl"
  Error_chpl error_chpl5 = NULL;
#line 22 "../lib/MNIST.chpl"
  int32_t call_tmp_chpl9;
#line 22 "../lib/MNIST.chpl"
  Error_chpl error_chpl6 = NULL;
#line 22 "../lib/MNIST.chpl"
  int32_t rowCount_chpl;
#line 23 "../lib/MNIST.chpl"
  int32_t call_tmp_chpl10;
#line 23 "../lib/MNIST.chpl"
  Error_chpl error_chpl7 = NULL;
#line 23 "../lib/MNIST.chpl"
  int32_t columnCount_chpl;
#line 28 "../lib/MNIST.chpl"
  range_int64_t_low_one_chpl call_tmp_chpl11;
#line 28 "../lib/MNIST.chpl"
  range_int64_t_low_one_chpl ret_tmp_chpl4;
#line 28 "../lib/MNIST.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl12;
#line 28 "../lib/MNIST.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl5;
#line 28 "../lib/MNIST.chpl"
  range_int64_t_low_one_chpl call_tmp_chpl13;
#line 28 "../lib/MNIST.chpl"
  range_int64_t_low_one_chpl ret_tmp_chpl6;
#line 28 "../lib/MNIST.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl14;
#line 28 "../lib/MNIST.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl7;
#line 28 "../lib/MNIST.chpl"
  _domain_DefaultRectangularDom_2_int64_t_one_chpl call_tmp_chpl15;
#line 28 "../lib/MNIST.chpl"
  _domain_DefaultRectangularDom_2_int64_t_one_chpl ret_tmp_chpl8;
#line 28 "../lib/MNIST.chpl"
  _domain_DefaultRectangularDom_2_int64_t_one_chpl imageDomain_chpl;
#line 28 "../lib/MNIST.chpl"
  _ref_DefaultRectangularDom_2_int64_t_one_chpl _instance_chpl = NULL;
#line 41 "../lib/MNIST.chpl"
  _array_DefaultRectangularArr_1_int64_t_one__array_DefaultRectangularArr_2_int64_t_one__real64_int64_t_int64_t_chpl images_chpl;
#line 41 "../lib/MNIST.chpl"
  range_int64_t_low_one_chpl call_tmp_chpl16;
#line 41 "../lib/MNIST.chpl"
  range_int64_t_low_one_chpl ret_tmp_chpl9;
#line 41 "../lib/MNIST.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl17;
#line 41 "../lib/MNIST.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl10;
#line 41 "../lib/MNIST.chpl"
  _domain_DefaultRectangularDom_1_int64_t_one_chpl call_tmp_chpl18;
#line 41 "../lib/MNIST.chpl"
  _domain_DefaultRectangularDom_1_int64_t_one_chpl ret_tmp_chpl11;
#line 41 "../lib/MNIST.chpl"
  _ref__domain_DefaultRectangularDom_2_int64_t_one_chpl call_tmp_chpl19 = NULL;
#line 41 "../lib/MNIST.chpl"
  chpl___RuntimeTypeInfo14 call_tmp_chpl20;
#line 41 "../lib/MNIST.chpl"
  chpl___RuntimeTypeInfo11 call_tmp_chpl21;
#line 41 "../lib/MNIST.chpl"
  _domain_DefaultRectangularDom_1_int64_t_one_chpl _runtime_type_tmp__chpl;
#line 41 "../lib/MNIST.chpl"
  chpl___RuntimeTypeInfo14 _runtime_type_tmp__chpl2;
#line 41 "../lib/MNIST.chpl"
  _array_DefaultRectangularArr_1_int64_t_one__array_DefaultRectangularArr_2_int64_t_one__real64_int64_t_int64_t_chpl ret_tmp_chpl12;
#line 43 "../lib/MNIST.chpl"
  _domain_DefaultRectangularDom_1_int64_t_one_chpl call_tmp_chpl22;
#line 43 "../lib/MNIST.chpl"
  _domain_DefaultRectangularDom_1_int64_t_one_chpl ret_tmp_chpl13;
#line 43 "../lib/MNIST.chpl"
  _domain_DefaultRectangularDom_1_int64_t_one_chpl this_chpl31;
#line 43 "../lib/MNIST.chpl"
  DefaultRectangularDom_1_int64_t_one_chpl ret_chpl2 = NULL;
#line 43 "../lib/MNIST.chpl"
  _tuple_1_star_int64_t_chpl ret_tmp_chpl14;
#line 43 "../lib/MNIST.chpl"
  DefaultRectangularDom_1_int64_t_one_chpl this_chpl32 = NULL;
#line 43 "../lib/MNIST.chpl"
  _ref__tuple_1_star_range_int64_t_both_one_chpl call_tmp_chpl23 = NULL;
#line 43 "../lib/MNIST.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl3;
#line 43 "../lib/MNIST.chpl"
  range_int64_t_both_one_chpl _ic__F0_this_chpl;
#line 43 "../lib/MNIST.chpl"
  int64_t i_chpl;
#line 43 "../lib/MNIST.chpl"
  range_int64_t_both_one_chpl this_chpl33;
#line 43 "../lib/MNIST.chpl"
  range_int64_t_both_one_chpl this_chpl34;
#line 43 "../lib/MNIST.chpl"
  int64_t end_chpl;
#line 44 "../lib/MNIST.chpl"
  _ref__array_DefaultRectangularArr_1_int64_t_one__array_DefaultRectangularArr_2_int64_t_one__real64_int64_t_int64_t_chpl i_this_chpl = NULL;
#line 44 "../lib/MNIST.chpl"
  _tuple_1_star_int64_t_chpl coerce_tmp_chpl4;
#line 44 "../lib/MNIST.chpl"
  _ref__tuple_1_star_int64_t_chpl i_i_chpl = NULL;
#line 44 "../lib/MNIST.chpl"
  DefaultRectangularArr_1_int64_t_one__array_DefaultRectangularArr_2_int64_t_one__real64_int64_t_int64_t_chpl ret_chpl3 = NULL;
#line 44 "../lib/MNIST.chpl"
  DefaultRectangularArr_1_int64_t_one__array_DefaultRectangularArr_2_int64_t_one__real64_int64_t_int64_t_chpl coerce_tmp_chpl5 = NULL;
#line 44 "../lib/MNIST.chpl"
  _tuple_1_star_int64_t_chpl _formal_tmp_in_ind_chpl;
#line 44 "../lib/MNIST.chpl"
  int64_t _formal_tmp_ind_x0_chpl;
#line 44 "../lib/MNIST.chpl"
  _ddata__array_DefaultRectangularArr_2_int64_t_one__real64_int64_t_chpl coerce_tmp_chpl6 = NULL;
#line 44 "../lib/MNIST.chpl"
  _ref__array_DefaultRectangularArr_2_int64_t_one__real64_int64_t_chpl call_tmp_chpl24 = NULL;
#line 44 "../lib/MNIST.chpl"
  _array_DefaultRectangularArr_2_int64_t_one__real64_int64_t_chpl call_tmp_chpl25;
#line 44 "../lib/MNIST.chpl"
  _array_DefaultRectangularArr_2_int64_t_one__real64_int64_t_chpl ret_tmp_chpl15;
#line 44 "../lib/MNIST.chpl"
  _ref__array_DefaultRectangularArr_2_int64_t_one__real64_int64_t_chpl i_b_chpl = NULL;
#line 44 "../lib/MNIST.chpl"
  uint64_t call_tmp_chpl26;
#line 44 "../lib/MNIST.chpl"
  chpl_bool tmp_chpl;
#line 44 "../lib/MNIST.chpl"
  chpl_bool tmp_chpl2;
#line 44 "../lib/MNIST.chpl"
  uint64_t call_tmp_chpl27;
#line 44 "../lib/MNIST.chpl"
  chpl_bool tmp_chpl3;
#line 44 "../lib/MNIST.chpl"
  _domain_DefaultRectangularDom_2_int64_t_one_chpl call_tmp_chpl28;
#line 44 "../lib/MNIST.chpl"
  _domain_DefaultRectangularDom_2_int64_t_one_chpl ret_tmp_chpl16;
#line 44 "../lib/MNIST.chpl"
  _domain_DefaultRectangularDom_2_int64_t_one_chpl call_tmp_chpl29;
#line 44 "../lib/MNIST.chpl"
  _domain_DefaultRectangularDom_2_int64_t_one_chpl ret_tmp_chpl17;
#line 44 "../lib/MNIST.chpl"
  _ref__domain_DefaultRectangularDom_2_int64_t_one_chpl i_AD_chpl = NULL;
#line 44 "../lib/MNIST.chpl"
  _ref__domain_DefaultRectangularDom_2_int64_t_one_chpl i_BD_chpl = NULL;
#line 44 "../lib/MNIST.chpl"
  DefaultRectangularArr_2_int64_t_one__real64_int64_t_chpl ret_chpl4 = NULL;
#line 44 "../lib/MNIST.chpl"
  DefaultRectangularArr_2_int64_t_one__real64_int64_t_chpl ret_chpl5 = NULL;
#line 44 "../lib/MNIST.chpl"
  chpl_bool call_tmp_chpl30;
#line 44 "../lib/MNIST.chpl"
  DefaultRectangularArr_2_int64_t_one__real64_int64_t_chpl coerce_tmp_chpl7 = NULL;
#line 44 "../lib/MNIST.chpl"
  int64_t tasksPerLocale_chpl;
#line 44 "../lib/MNIST.chpl"
  chpl_bool ignoreRunning_chpl;
#line 44 "../lib/MNIST.chpl"
  int64_t minIndicesPerTask_chpl;
#line 44 "../lib/MNIST.chpl"
  Error_chpl error_chpl8 = NULL;
#line 44 "../lib/MNIST.chpl"
  DefaultRectangularDom_2_int64_t_one_chpl coerce_tmp_chpl8 = NULL;
#line 44 "../lib/MNIST.chpl"
  int64_t offset_x0_chpl;
#line 44 "../lib/MNIST.chpl"
  int64_t offset_x1_chpl;
#line 44 "../lib/MNIST.chpl"
  _tuple_2_star_int64_t_chpl ret_tmp_chpl18;
#line 44 "../lib/MNIST.chpl"
  Error_chpl error_chpl9 = NULL;
#line 44 "../lib/MNIST.chpl"
  locale_chpl call_tmp_chpl31;
#line 44 "../lib/MNIST.chpl"
  chpl_localeID_t call_tmp_chpl32;
#line 44 "../lib/MNIST.chpl"
  int32_t call_tmp_chpl33;
#line 44 "../lib/MNIST.chpl"
  chpl_localeID_t call_tmp_chpl34;
#line 44 "../lib/MNIST.chpl"
  locale_chpl call_tmp_chpl35;
#line 44 "../lib/MNIST.chpl"
  locale_chpl ret_tmp_chpl19;
#line 44 "../lib/MNIST.chpl"
  BaseLocale_chpl coerce_tmp_chpl9 = NULL;
#line 44 "../lib/MNIST.chpl"
  _ref_locale_chpl i_x_chpl = NULL;
#line 44 "../lib/MNIST.chpl"
  int64_t tmp_chpl4;
#line 44 "../lib/MNIST.chpl"
  locale_chpl call_tmp_chpl36;
#line 44 "../lib/MNIST.chpl"
  chpl_localeID_t call_tmp_chpl37;
#line 44 "../lib/MNIST.chpl"
  int32_t call_tmp_chpl38;
#line 44 "../lib/MNIST.chpl"
  chpl_localeID_t call_tmp_chpl39;
#line 44 "../lib/MNIST.chpl"
  locale_chpl call_tmp_chpl40;
#line 44 "../lib/MNIST.chpl"
  locale_chpl ret_tmp_chpl20;
#line 44 "../lib/MNIST.chpl"
  int64_t ret_chpl6;
#line 44 "../lib/MNIST.chpl"
  BaseLocale_chpl coerce_tmp_chpl10 = NULL;
#line 44 "../lib/MNIST.chpl"
  BaseLocale_chpl coerce_tmp_chpl11 = NULL;
#line 44 "../lib/MNIST.chpl"
  _ref_locale_chpl i_x_chpl2 = NULL;
#line 44 "../lib/MNIST.chpl"
  int64_t tmp_x0_chpl;
#line 44 "../lib/MNIST.chpl"
  int64_t tmp_x1_chpl;
#line 44 "../lib/MNIST.chpl"
  _ref__tuple_2_star_range_int64_t_both_one_chpl call_tmp_chpl41 = NULL;
#line 44 "../lib/MNIST.chpl"
  _tuple_2_star_int64_t_chpl ret_tmp_chpl21;
#line 44 "../lib/MNIST.chpl"
  int64_t numChunks_chpl;
#line 44 "../lib/MNIST.chpl"
  int64_t parDim_chpl;
#line 44 "../lib/MNIST.chpl"
  _tuple_2_star_range_int64_t_both_one_chpl locBlock_chpl;
#line 44 "../lib/MNIST.chpl"
  range_int64_t_both_one_chpl tup_x0_chpl;
#line 44 "../lib/MNIST.chpl"
  range_int64_t_both_one_chpl tup_x1_chpl;
#line 44 "../lib/MNIST.chpl"
  range_int64_t_both_one_chpl elt_x0_chpl;
#line 44 "../lib/MNIST.chpl"
  range_int64_t_both_one_chpl elt_x1_chpl;
#line 44 "../lib/MNIST.chpl"
  range_int64_t_low_one_chpl call_tmp_chpl42;
#line 44 "../lib/MNIST.chpl"
  range_int64_t_low_one_chpl ret_tmp_chpl22;
#line 44 "../lib/MNIST.chpl"
  _ref__tuple_2_star_range_int64_t_both_one_chpl call_tmp_chpl43 = NULL;
#line 44 "../lib/MNIST.chpl"
  int64_t call_tmp_chpl44;
#line 44 "../lib/MNIST.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl12;
#line 44 "../lib/MNIST.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl45;
#line 44 "../lib/MNIST.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl23;
#line 44 "../lib/MNIST.chpl"
  range_int64_t_low_one_chpl call_tmp_chpl46;
#line 44 "../lib/MNIST.chpl"
  range_int64_t_low_one_chpl ret_tmp_chpl24;
#line 44 "../lib/MNIST.chpl"
  _ref__tuple_2_star_range_int64_t_both_one_chpl call_tmp_chpl47 = NULL;
#line 44 "../lib/MNIST.chpl"
  int64_t call_tmp_chpl48;
#line 44 "../lib/MNIST.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl13;
#line 44 "../lib/MNIST.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl49;
#line 44 "../lib/MNIST.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl25;
#line 44 "../lib/MNIST.chpl"
  range_int64_t_both_one_chpl tmpIter_chpl;
#line 44 "../lib/MNIST.chpl"
  range_int64_t_low_one_chpl call_tmp_chpl50;
#line 44 "../lib/MNIST.chpl"
  range_int64_t_low_one_chpl ret_tmp_chpl26;
#line 44 "../lib/MNIST.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl27;
#line 44 "../lib/MNIST.chpl"
  chpl___EndCount_AtomicT_int64_t_int64_t _coforallCount_chpl = NULL;
#line 44 "../lib/MNIST.chpl"
  chpl___EndCount_AtomicT_int64_t_int64_t new_temp_chpl = NULL;
#line 44 "../lib/MNIST.chpl"
  int64_t numTasks_chpl;
#line 44 "../lib/MNIST.chpl"
  range_int64_t_both_one_chpl _ic__F0_this_chpl2;
#line 44 "../lib/MNIST.chpl"
  int64_t i_chpl2;
#line 44 "../lib/MNIST.chpl"
  range_int64_t_both_one_chpl this_chpl35;
#line 44 "../lib/MNIST.chpl"
  range_int64_t_both_one_chpl this_chpl36;
#line 44 "../lib/MNIST.chpl"
  int64_t end_chpl2;
#line 44 "../lib/MNIST.chpl"
  _class_localscoforall_fn_chpl18 _args_forcoforall_fn_chpl = NULL;
#line 44 "../lib/MNIST.chpl"
  raw_c_void_ptr _args_vforcoforall_fn_chpl;
#line 44 "../lib/MNIST.chpl"
  int64_t _args_sizecoforall_fn_chpl;
#line 44 "../lib/MNIST.chpl"
  Error_chpl error_chpl10 = NULL;
#line 47 "../lib/MNIST.chpl"
  _ref_fileReader_T_BinaryDeserializer_chpl i_x_chpl3 = NULL;
#line 47 "../lib/MNIST.chpl"
  chpl__class_localscoforall_fn_chpl18_object chpl_macro_tmp_159;
#line 44 "../lib/MNIST.chpl"
  local_dataParMinGranularity_chpl = dataParMinGranularity_chpl;
#line 44 "../lib/MNIST.chpl"
  local_dataParIgnoreRunningTasks_chpl = dataParIgnoreRunningTasks_chpl;
#line 44 "../lib/MNIST.chpl"
  local_dataParTasksPerLocale_chpl = dataParTasksPerLocale_chpl;
#line 8 "../lib/MNIST.chpl"
  init_chpl204(&initTemp_chpl, ioendian_chpl_big_chpl, UINT8(true));
#line 8 "../lib/MNIST.chpl"
  call_tmp_chpl6 = initTemp_chpl;
#line 10 "../lib/MNIST.chpl"
  error_chpl = NULL;
#line 10 "../lib/MNIST.chpl"
  chpl_build_low_bounded_range2(INT64(0), &ret_tmp_chpl);
#line 10 "../lib/MNIST.chpl"
  region_chpl = ret_tmp_chpl;
#line 10 "../lib/MNIST.chpl"
  if (error_chpl != nil) /* ZLINE: 10 ../lib/MNIST.chpl */
#line 10 "../lib/MNIST.chpl"
  {
#line 10 "../lib/MNIST.chpl"
    chpl_uncaught_error(error_chpl, INT64(10), INT32(71));
#line 10 "../lib/MNIST.chpl"
  }
#line 10 "../lib/MNIST.chpl"
  error_chpl2 = NULL;
#line 10 "../lib/MNIST.chpl"
  empty_chpl2(&ret_tmp_chpl2);
#line 10 "../lib/MNIST.chpl"
  hints_chpl = ret_tmp_chpl2;
#line 10 "../lib/MNIST.chpl"
  default_arg_hints_chpl = hints_chpl;
#line 10 "../lib/MNIST.chpl"
  if (error_chpl2 != nil) /* ZLINE: 10 ../lib/MNIST.chpl */
#line 10 "../lib/MNIST.chpl"
  {
#line 10 "../lib/MNIST.chpl"
    chpl_uncaught_error(error_chpl2, INT64(10), INT32(71));
#line 10 "../lib/MNIST.chpl"
  }
#line 10 "../lib/MNIST.chpl"
  _formal_tmp_in_deserializer_chpl = call_tmp_chpl6;
#line 10 "../lib/MNIST.chpl"
  error_chpl3 = NULL;
#line 10 "../lib/MNIST.chpl"
  openReader_chpl(fileName_chpl, region_chpl, &default_arg_hints_chpl, &_formal_tmp_in_deserializer_chpl, &error_chpl3, &ret_tmp_chpl3, INT64(10), INT32(71));
#line 10 "../lib/MNIST.chpl"
  call_tmp_chpl7 = ret_tmp_chpl3;
#line 10 "../lib/MNIST.chpl"
  if (error_chpl3 != nil) /* ZLINE: 10 ../lib/MNIST.chpl */
#line 10 "../lib/MNIST.chpl"
  {
#line 10 "../lib/MNIST.chpl"
    chpl_uncaught_error(error_chpl3, INT64(10), INT32(71));
#line 10 "../lib/MNIST.chpl"
  }
#line 10 "../lib/MNIST.chpl"
  fr_chpl = call_tmp_chpl7;
#line 12 "../lib/MNIST.chpl"
  error_chpl4 = NULL;
#line 12 "../lib/MNIST.chpl"
  call_tmp_chpl8 = read_chpl2(&fr_chpl, &error_chpl4, INT64(12), INT32(71));
#line 12 "../lib/MNIST.chpl"
  if (error_chpl4 != nil) /* ZLINE: 12 ../lib/MNIST.chpl */
#line 12 "../lib/MNIST.chpl"
  {
#line 12 "../lib/MNIST.chpl"
    chpl_uncaught_error(error_chpl4, INT64(12), INT32(71));
#line 12 "../lib/MNIST.chpl"
  }
#line 14 "../lib/MNIST.chpl"
  if (call_tmp_chpl8 != ((int32_t)(INT64(2051)))) /* ZLINE: 14 ../lib/MNIST.chpl */
#line 14 "../lib/MNIST.chpl"
  {
#line 15 "../lib/MNIST.chpl"
    writeln_chpl4(&_str_literal_2140_chpl, INT64(15), INT32(71));
#line 16 "../lib/MNIST.chpl"
    halt_chpl14(INT64(1), INT64(16), INT32(71));
#line 16 "../lib/MNIST.chpl"
  }
#line 19 "../lib/MNIST.chpl"
  error_chpl5 = NULL;
#line 19 "../lib/MNIST.chpl"
  read_chpl2(&fr_chpl, &error_chpl5, INT64(19), INT32(71));
#line 19 "../lib/MNIST.chpl"
  if (error_chpl5 != nil) /* ZLINE: 19 ../lib/MNIST.chpl */
#line 19 "../lib/MNIST.chpl"
  {
#line 19 "../lib/MNIST.chpl"
    chpl_uncaught_error(error_chpl5, INT64(19), INT32(71));
#line 19 "../lib/MNIST.chpl"
  }
#line 22 "../lib/MNIST.chpl"
  error_chpl6 = NULL;
#line 22 "../lib/MNIST.chpl"
  call_tmp_chpl9 = read_chpl2(&fr_chpl, &error_chpl6, INT64(22), INT32(71));
#line 22 "../lib/MNIST.chpl"
  if (error_chpl6 != nil) /* ZLINE: 22 ../lib/MNIST.chpl */
#line 22 "../lib/MNIST.chpl"
  {
#line 22 "../lib/MNIST.chpl"
    chpl_uncaught_error(error_chpl6, INT64(22), INT32(71));
#line 22 "../lib/MNIST.chpl"
  }
#line 22 "../lib/MNIST.chpl"
  rowCount_chpl = call_tmp_chpl9;
#line 23 "../lib/MNIST.chpl"
  error_chpl7 = NULL;
#line 23 "../lib/MNIST.chpl"
  call_tmp_chpl10 = read_chpl2(&fr_chpl, &error_chpl7, INT64(23), INT32(71));
#line 23 "../lib/MNIST.chpl"
  if (error_chpl7 != nil) /* ZLINE: 23 ../lib/MNIST.chpl */
#line 23 "../lib/MNIST.chpl"
  {
#line 23 "../lib/MNIST.chpl"
    chpl_uncaught_error(error_chpl7, INT64(23), INT32(71));
#line 23 "../lib/MNIST.chpl"
  }
#line 23 "../lib/MNIST.chpl"
  columnCount_chpl = call_tmp_chpl10;
#line 28 "../lib/MNIST.chpl"
  chpl_build_low_bounded_range2(INT64(0), &ret_tmp_chpl4);
#line 28 "../lib/MNIST.chpl"
  call_tmp_chpl11 = ret_tmp_chpl4;
#line 28 "../lib/MNIST.chpl"
  chpl___POUND_(call_tmp_chpl11, call_tmp_chpl9, &ret_tmp_chpl5, INT64(28), INT32(71));
#line 28 "../lib/MNIST.chpl"
  call_tmp_chpl12 = ret_tmp_chpl5;
#line 28 "../lib/MNIST.chpl"
  chpl_build_low_bounded_range2(INT64(0), &ret_tmp_chpl6);
#line 28 "../lib/MNIST.chpl"
  call_tmp_chpl13 = ret_tmp_chpl6;
#line 28 "../lib/MNIST.chpl"
  chpl___POUND_(call_tmp_chpl13, call_tmp_chpl10, &ret_tmp_chpl7, INT64(28), INT32(71));
#line 28 "../lib/MNIST.chpl"
  call_tmp_chpl14 = ret_tmp_chpl7;
#line 28 "../lib/MNIST.chpl"
  chpl__buildDomainExpr2(call_tmp_chpl12, call_tmp_chpl14, UINT8(false), &ret_tmp_chpl8, INT64(28), INT32(71));
#line 28 "../lib/MNIST.chpl"
  call_tmp_chpl15 = ret_tmp_chpl8;
#line 28 "../lib/MNIST.chpl"
  imageDomain_chpl = call_tmp_chpl15;
#line 28 "../lib/MNIST.chpl"
  _instance_chpl = &((&imageDomain_chpl)->_instance);
#line 28 "../lib/MNIST.chpl"
  chpl_check_nil(_instance_chpl, INT64(28), INT32(71));
#line 28 "../lib/MNIST.chpl"
  ((BaseDom_chpl)(*(_instance_chpl)))->definedConst = UINT8(false);
#line 41 "../lib/MNIST.chpl"
  chpl_build_low_bounded_range2(INT64(0), &ret_tmp_chpl9);
#line 41 "../lib/MNIST.chpl"
  call_tmp_chpl16 = ret_tmp_chpl9;
#line 41 "../lib/MNIST.chpl"
  chpl___POUND_3(call_tmp_chpl16, num_chpl, &ret_tmp_chpl10, INT64(41), INT32(71));
#line 41 "../lib/MNIST.chpl"
  call_tmp_chpl17 = ret_tmp_chpl10;
#line 41 "../lib/MNIST.chpl"
  chpl__ensureDomainExpr5(call_tmp_chpl17, &ret_tmp_chpl11, INT64(41), INT32(71));
#line 41 "../lib/MNIST.chpl"
  call_tmp_chpl18 = ret_tmp_chpl11;
#line 41 "../lib/MNIST.chpl"
  call_tmp_chpl19 = chpl__ensureDomainExpr(&imageDomain_chpl);
#line 41 "../lib/MNIST.chpl"
  call_tmp_chpl20 = chpl__buildArrayRuntimeType14(call_tmp_chpl19);
#line 41 "../lib/MNIST.chpl"
  call_tmp_chpl21 = chpl__buildArrayRuntimeType10(&call_tmp_chpl18, &call_tmp_chpl20);
#line 41 "../lib/MNIST.chpl"
  _runtime_type_tmp__chpl = (&call_tmp_chpl21)->dom;
#line 41 "../lib/MNIST.chpl"
  _runtime_type_tmp__chpl2 = (&call_tmp_chpl21)->eltType;
#line 41 "../lib/MNIST.chpl"
  chpl__convertRuntimeTypeToValue11(&_runtime_type_tmp__chpl, &_runtime_type_tmp__chpl2, UINT8(false), &ret_tmp_chpl12, INT64(41), INT32(71));
#line 41 "../lib/MNIST.chpl"
  images_chpl = ret_tmp_chpl12;
#line 43 "../lib/MNIST.chpl"
  _dom_chpl17(&images_chpl, &ret_tmp_chpl13, INT64(43), INT32(71));
#line 43 "../lib/MNIST.chpl"
  call_tmp_chpl22 = ret_tmp_chpl13;
#line 43 "../lib/MNIST.chpl"
  this_chpl31 = call_tmp_chpl22;
#line 43 "../lib/MNIST.chpl"
  ret_chpl2 = (&this_chpl31)->_instance;
#line 43 "../lib/MNIST.chpl"
  createTuple_chpl(INT64(0), &ret_tmp_chpl14);
#line 43 "../lib/MNIST.chpl"
  this_chpl32 = ret_chpl2;
#line 43 "../lib/MNIST.chpl"
  chpl_check_nil(this_chpl32, INT64(43), INT32(71));
#line 43 "../lib/MNIST.chpl"
  call_tmp_chpl23 = &((this_chpl32)->ranges);
#line 43 "../lib/MNIST.chpl"
  coerce_tmp_chpl3 = *(*(call_tmp_chpl23) + INT64(0));
#line 43 "../lib/MNIST.chpl"
  _ic__F0_this_chpl = coerce_tmp_chpl3;
#line 43 "../lib/MNIST.chpl"
  checkIfIterWillOverflow_chpl(coerce_tmp_chpl3, UINT8(true), INT64(43), INT32(71));
#line 43 "../lib/MNIST.chpl"
  i_chpl = INT64(0);
#line 43 "../lib/MNIST.chpl"
  this_chpl33 = _ic__F0_this_chpl;
#line 43 "../lib/MNIST.chpl"
  this_chpl34 = _ic__F0_this_chpl;
#line 43 "../lib/MNIST.chpl"
  end_chpl = (&this_chpl34)->_high;
#line 43 "../lib/MNIST.chpl"
  for (i_chpl = (&this_chpl33)->_low; ((i_chpl <= end_chpl)); i_chpl += INT64(1)) {
#line 44 "../lib/MNIST.chpl"
    i_this_chpl = &images_chpl;
#line 44 "../lib/MNIST.chpl"
    *(coerce_tmp_chpl4 + INT64(0)) = i_chpl;
#line 44 "../lib/MNIST.chpl"
    i_i_chpl = &coerce_tmp_chpl4;
#line 44 "../lib/MNIST.chpl"
    ret_chpl3 = (&images_chpl)->_instance;
#line 44 "../lib/MNIST.chpl"
    checkAccess_chpl5(i_this_chpl, i_i_chpl, ret_chpl3, INT64(44), INT32(71));
#line 44 "../lib/MNIST.chpl"
    coerce_tmp_chpl5 = ret_chpl3;
#line 44 "../lib/MNIST.chpl"
    *(_formal_tmp_in_ind_chpl + INT64(0)) = *(coerce_tmp_chpl4 + INT64(0));
#line 44 "../lib/MNIST.chpl"
    _formal_tmp_ind_x0_chpl = *(_formal_tmp_in_ind_chpl + INT64(0));
#line 44 "../lib/MNIST.chpl"
    chpl_check_nil(coerce_tmp_chpl5, INT64(44), INT32(71));
#line 44 "../lib/MNIST.chpl"
    coerce_tmp_chpl6 = (coerce_tmp_chpl5)->shiftedData;
#line 44 "../lib/MNIST.chpl"
    call_tmp_chpl24 = (coerce_tmp_chpl6 + _formal_tmp_ind_x0_chpl);
#line 44 "../lib/MNIST.chpl"
    readImage_chpl(&fr_chpl, &rowCount_chpl, &columnCount_chpl, &imageDomain_chpl, &ret_tmp_chpl15);
#line 44 "../lib/MNIST.chpl"
    call_tmp_chpl25 = ret_tmp_chpl15;
#line 44 "../lib/MNIST.chpl"
    i_b_chpl = &call_tmp_chpl25;
#line 44 "../lib/MNIST.chpl"
    if (((RootClass_chpl)((&call_tmp_chpl25)->_instance)) == nil) /* ZLINE: 44 ../lib/MNIST.chpl */
#line 44 "../lib/MNIST.chpl"
    {
#line 44 "../lib/MNIST.chpl"
      goto _end__ASSIGN__chpl;
#line 44 "../lib/MNIST.chpl"
    }
#line 44 "../lib/MNIST.chpl"
    if (((RootClass_chpl)((call_tmp_chpl24)->_instance)) == ((RootClass_chpl)((&call_tmp_chpl25)->_instance))) /* ZLINE: 44 ../lib/MNIST.chpl */
#line 44 "../lib/MNIST.chpl"
    {
#line 44 "../lib/MNIST.chpl"
      goto _end__ASSIGN__chpl;
#line 44 "../lib/MNIST.chpl"
    }
#line 44 "../lib/MNIST.chpl"
    call_tmp_chpl26 = sizeAs_chpl14(call_tmp_chpl24, INT64(44), INT32(71));
#line 44 "../lib/MNIST.chpl"
    if (! (INT64(0) < INT64(0))) /* ZLINE: 44 ../lib/MNIST.chpl */
#line 44 "../lib/MNIST.chpl"
    {
#line 44 "../lib/MNIST.chpl"
      tmp_chpl = (call_tmp_chpl26 == ((uint64_t)(INT64(0))));
#line 44 "../lib/MNIST.chpl"
    } else /* ZLINE: 44 ../lib/MNIST.chpl */
#line 44 "../lib/MNIST.chpl"
    {
#line 44 "../lib/MNIST.chpl"
      tmp_chpl = UINT8(false);
#line 44 "../lib/MNIST.chpl"
    }
#line 44 "../lib/MNIST.chpl"
    if (tmp_chpl) /* ZLINE: 44 ../lib/MNIST.chpl */
#line 44 "../lib/MNIST.chpl"
    {
#line 44 "../lib/MNIST.chpl"
      call_tmp_chpl27 = sizeAs_chpl14(i_b_chpl, INT64(44), INT32(71));
#line 44 "../lib/MNIST.chpl"
      if (! (INT64(0) < INT64(0))) /* ZLINE: 44 ../lib/MNIST.chpl */
#line 44 "../lib/MNIST.chpl"
      {
#line 44 "../lib/MNIST.chpl"
        tmp_chpl3 = (call_tmp_chpl27 == ((uint64_t)(INT64(0))));
#line 44 "../lib/MNIST.chpl"
      } else /* ZLINE: 44 ../lib/MNIST.chpl */
#line 44 "../lib/MNIST.chpl"
      {
#line 44 "../lib/MNIST.chpl"
        tmp_chpl3 = UINT8(false);
#line 44 "../lib/MNIST.chpl"
      }
#line 44 "../lib/MNIST.chpl"
      tmp_chpl2 = tmp_chpl3;
#line 44 "../lib/MNIST.chpl"
    } else /* ZLINE: 44 ../lib/MNIST.chpl */
#line 44 "../lib/MNIST.chpl"
    {
#line 44 "../lib/MNIST.chpl"
      tmp_chpl2 = UINT8(false);
#line 44 "../lib/MNIST.chpl"
    }
#line 44 "../lib/MNIST.chpl"
    if (tmp_chpl2) /* ZLINE: 44 ../lib/MNIST.chpl */
#line 44 "../lib/MNIST.chpl"
    {
#line 44 "../lib/MNIST.chpl"
      goto _end__ASSIGN__chpl;
#line 44 "../lib/MNIST.chpl"
    }
#line 44 "../lib/MNIST.chpl"
    checkArrayShapesUponAssignment_chpl9(call_tmp_chpl24, i_b_chpl, UINT8(false), INT64(44), INT32(71));
#line 44 "../lib/MNIST.chpl"
    _dom_chpl14(call_tmp_chpl24, &ret_tmp_chpl16, INT64(44), INT32(71));
#line 44 "../lib/MNIST.chpl"
    call_tmp_chpl28 = ret_tmp_chpl16;
#line 44 "../lib/MNIST.chpl"
    _dom_chpl14(i_b_chpl, &ret_tmp_chpl17, INT64(44), INT32(71));
#line 44 "../lib/MNIST.chpl"
    call_tmp_chpl29 = ret_tmp_chpl17;
#line 44 "../lib/MNIST.chpl"
    i_AD_chpl = &call_tmp_chpl28;
#line 44 "../lib/MNIST.chpl"
    i_BD_chpl = &call_tmp_chpl29;
#line 44 "../lib/MNIST.chpl"
    ret_chpl4 = (call_tmp_chpl24)->_instance;
#line 44 "../lib/MNIST.chpl"
    ret_chpl5 = (&call_tmp_chpl25)->_instance;
#line 44 "../lib/MNIST.chpl"
    chpl_check_nil(ret_chpl4, INT64(44), INT32(71));
#line 44 "../lib/MNIST.chpl"
    call_tmp_chpl30 = doiBulkTransferFromKnown_chpl13(ret_chpl4, i_AD_chpl, ret_chpl5, i_BD_chpl, INT64(44), INT32(71));
#line 44 "../lib/MNIST.chpl"
    if (call_tmp_chpl30) /* ZLINE: 44 ../lib/MNIST.chpl */
#line 44 "../lib/MNIST.chpl"
    {
#line 44 "../lib/MNIST.chpl"
    } else /* ZLINE: 44 ../lib/MNIST.chpl */
#line 44 "../lib/MNIST.chpl"
    {
#line 44 "../lib/MNIST.chpl"
    }
#line 44 "../lib/MNIST.chpl"
    chpl__autoDestroy5(&call_tmp_chpl29, INT64(44), INT32(71));
#line 44 "../lib/MNIST.chpl"
    chpl__autoDestroy5(&call_tmp_chpl28, INT64(44), INT32(71));
#line 44 "../lib/MNIST.chpl"
    if (call_tmp_chpl30) /* ZLINE: 44 ../lib/MNIST.chpl */
#line 44 "../lib/MNIST.chpl"
    {
#line 44 "../lib/MNIST.chpl"
    }
#line 44 "../lib/MNIST.chpl"
    if (! call_tmp_chpl30) /* ZLINE: 44 ../lib/MNIST.chpl */
#line 44 "../lib/MNIST.chpl"
    {
#line 44 "../lib/MNIST.chpl"
      coerce_tmp_chpl7 = (call_tmp_chpl24)->_instance;
#line 44 "../lib/MNIST.chpl"
      tasksPerLocale_chpl = local_dataParTasksPerLocale_chpl;
#line 44 "../lib/MNIST.chpl"
      ignoreRunning_chpl = local_dataParIgnoreRunningTasks_chpl;
#line 44 "../lib/MNIST.chpl"
      minIndicesPerTask_chpl = local_dataParMinGranularity_chpl;
#line 44 "../lib/MNIST.chpl"
      error_chpl8 = NULL;
#line 44 "../lib/MNIST.chpl"
      chpl_check_nil(coerce_tmp_chpl7, INT64(44), INT32(71));
#line 44 "../lib/MNIST.chpl"
      coerce_tmp_chpl8 = (coerce_tmp_chpl7)->dom;
#line 44 "../lib/MNIST.chpl"
      createTuple_chpl4(INT64(0), &ret_tmp_chpl18);
#line 44 "../lib/MNIST.chpl"
      offset_x0_chpl = *(ret_tmp_chpl18 + INT64(0));
#line 44 "../lib/MNIST.chpl"
      offset_x1_chpl = *(ret_tmp_chpl18 + INT64(1));
#line 44 "../lib/MNIST.chpl"
      error_chpl9 = NULL;
#line 44 "../lib/MNIST.chpl"
      call_tmp_chpl33 = get_chpl_nodeID();
#line 44 "../lib/MNIST.chpl"
      call_tmp_chpl34 = chpl_rt_buildLocaleID(call_tmp_chpl33, c_sublocid_any);
#line 44 "../lib/MNIST.chpl"
      call_tmp_chpl32 = call_tmp_chpl34;
#line 44 "../lib/MNIST.chpl"
      chpl_localeID_to_locale(&call_tmp_chpl32, &ret_tmp_chpl19, INT64(44), INT32(71));
#line 44 "../lib/MNIST.chpl"
      call_tmp_chpl35 = ret_tmp_chpl19;
#line 44 "../lib/MNIST.chpl"
      call_tmp_chpl31 = call_tmp_chpl35;
#line 44 "../lib/MNIST.chpl"
      coerce_tmp_chpl9 = (&call_tmp_chpl31)->_instance;
#line 44 "../lib/MNIST.chpl"
      if (((RootClass_chpl)(coerce_tmp_chpl9)) == nil) /* ZLINE: 44 ../lib/MNIST.chpl */
#line 44 "../lib/MNIST.chpl"
      {
#line 44 "../lib/MNIST.chpl"
        nilCheckHalt_chpl(&_str_literal_52_chpl, INT64(44), INT32(71));
#line 44 "../lib/MNIST.chpl"
      }
#line 44 "../lib/MNIST.chpl"
      chpl_check_nil(coerce_tmp_chpl9, INT64(44), INT32(71));
#line 44 "../lib/MNIST.chpl"
      ((int64_t(*)(BaseLocale_chpl,
#line 44 "../lib/MNIST.chpl"
         int64_t,
#line 44 "../lib/MNIST.chpl"
         int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(coerce_tmp_chpl9))->chpl__cid) + INT64(2))])(coerce_tmp_chpl9, INT64(44), INT32(71));
#line 44 "../lib/MNIST.chpl"
      i_x_chpl = &call_tmp_chpl31;
#line 44 "../lib/MNIST.chpl"
      deinit_chpl62(i_x_chpl);
#line 44 "../lib/MNIST.chpl"
      if (tasksPerLocale_chpl == INT64(0)) /* ZLINE: 44 ../lib/MNIST.chpl */
#line 44 "../lib/MNIST.chpl"
      {
#line 44 "../lib/MNIST.chpl"
        call_tmp_chpl38 = get_chpl_nodeID();
#line 44 "../lib/MNIST.chpl"
        call_tmp_chpl39 = chpl_rt_buildLocaleID(call_tmp_chpl38, c_sublocid_any);
#line 44 "../lib/MNIST.chpl"
        call_tmp_chpl37 = call_tmp_chpl39;
#line 44 "../lib/MNIST.chpl"
        chpl_localeID_to_locale(&call_tmp_chpl37, &ret_tmp_chpl20, INT64(44), INT32(71));
#line 44 "../lib/MNIST.chpl"
        call_tmp_chpl40 = ret_tmp_chpl20;
#line 44 "../lib/MNIST.chpl"
        call_tmp_chpl36 = call_tmp_chpl40;
#line 44 "../lib/MNIST.chpl"
        coerce_tmp_chpl10 = (&call_tmp_chpl36)->_instance;
#line 44 "../lib/MNIST.chpl"
        if (((RootClass_chpl)(coerce_tmp_chpl10)) == nil) /* ZLINE: 44 ../lib/MNIST.chpl */
#line 44 "../lib/MNIST.chpl"
        {
#line 44 "../lib/MNIST.chpl"
          nilCheckHalt_chpl(&_str_literal_52_chpl, INT64(44), INT32(71));
#line 44 "../lib/MNIST.chpl"
        }
#line 44 "../lib/MNIST.chpl"
        coerce_tmp_chpl11 = coerce_tmp_chpl10;
#line 44 "../lib/MNIST.chpl"
        chpl_check_nil(coerce_tmp_chpl11, INT64(44), INT32(71));
#line 44 "../lib/MNIST.chpl"
        ret_chpl6 = (coerce_tmp_chpl11)->maxTaskPar;
#line 44 "../lib/MNIST.chpl"
        tmp_chpl4 = ret_chpl6;
#line 44 "../lib/MNIST.chpl"
        i_x_chpl2 = &call_tmp_chpl36;
#line 44 "../lib/MNIST.chpl"
        deinit_chpl62(i_x_chpl2);
#line 44 "../lib/MNIST.chpl"
      } else /* ZLINE: 44 ../lib/MNIST.chpl */
#line 44 "../lib/MNIST.chpl"
      {
#line 44 "../lib/MNIST.chpl"
        tmp_chpl4 = tasksPerLocale_chpl;
#line 44 "../lib/MNIST.chpl"
      }
#line 44 "../lib/MNIST.chpl"
      if (chpl_task_getSerial()) /* ZLINE: 44 ../lib/MNIST.chpl */
#line 44 "../lib/MNIST.chpl"
      {
#line 44 "../lib/MNIST.chpl"
        tmp_x0_chpl = INT64(1);
#line 44 "../lib/MNIST.chpl"
        tmp_x1_chpl = INT64(0);
#line 44 "../lib/MNIST.chpl"
      } else /* ZLINE: 44 ../lib/MNIST.chpl */
#line 44 "../lib/MNIST.chpl"
      {
#line 44 "../lib/MNIST.chpl"
        chpl_check_nil(coerce_tmp_chpl8, INT64(44), INT32(71));
#line 44 "../lib/MNIST.chpl"
        call_tmp_chpl41 = &((coerce_tmp_chpl8)->ranges);
#line 44 "../lib/MNIST.chpl"
        _computeChunkStuff_chpl4(tmp_chpl4, ignoreRunning_chpl, minIndicesPerTask_chpl, call_tmp_chpl41, &ret_tmp_chpl21, INT64(44), INT32(71));
#line 44 "../lib/MNIST.chpl"
        tmp_x0_chpl = *(ret_tmp_chpl21 + INT64(0));
#line 44 "../lib/MNIST.chpl"
        tmp_x1_chpl = *(ret_tmp_chpl21 + INT64(1));
#line 44 "../lib/MNIST.chpl"
      }
#line 44 "../lib/MNIST.chpl"
      numChunks_chpl = tmp_x0_chpl;
#line 44 "../lib/MNIST.chpl"
      parDim_chpl = tmp_x1_chpl;
#line 44 "../lib/MNIST.chpl"
      init_chpl113(&elt_x0_chpl);
#line 44 "../lib/MNIST.chpl"
      tup_x0_chpl = elt_x0_chpl;
#line 44 "../lib/MNIST.chpl"
      init_chpl113(&elt_x1_chpl);
#line 44 "../lib/MNIST.chpl"
      tup_x1_chpl = elt_x1_chpl;
#line 44 "../lib/MNIST.chpl"
      *(locBlock_chpl + INT64(0)) = tup_x0_chpl;
#line 44 "../lib/MNIST.chpl"
      *(locBlock_chpl + INT64(1)) = tup_x1_chpl;
#line 44 "../lib/MNIST.chpl"
      chpl_build_low_bounded_range2(offset_x0_chpl, &ret_tmp_chpl22);
#line 44 "../lib/MNIST.chpl"
      call_tmp_chpl42 = ret_tmp_chpl22;
#line 44 "../lib/MNIST.chpl"
      chpl_check_nil(coerce_tmp_chpl8, INT64(44), INT32(71));
#line 44 "../lib/MNIST.chpl"
      call_tmp_chpl43 = &((coerce_tmp_chpl8)->ranges);
#line 44 "../lib/MNIST.chpl"
      coerce_tmp_chpl12 = *(*(call_tmp_chpl43) + INT64(0));
#line 44 "../lib/MNIST.chpl"
      call_tmp_chpl44 = sizeAs_chpl37(coerce_tmp_chpl12, INT64(44), INT32(71));
#line 44 "../lib/MNIST.chpl"
      chpl___POUND_3(call_tmp_chpl42, call_tmp_chpl44, &ret_tmp_chpl23, INT64(44), INT32(71));
#line 44 "../lib/MNIST.chpl"
      call_tmp_chpl45 = ret_tmp_chpl23;
#line 44 "../lib/MNIST.chpl"
      *(locBlock_chpl + INT64(0)) = call_tmp_chpl45;
#line 44 "../lib/MNIST.chpl"
      chpl_build_low_bounded_range2(offset_x1_chpl, &ret_tmp_chpl24);
#line 44 "../lib/MNIST.chpl"
      call_tmp_chpl46 = ret_tmp_chpl24;
#line 44 "../lib/MNIST.chpl"
      chpl_check_nil(coerce_tmp_chpl8, INT64(44), INT32(71));
#line 44 "../lib/MNIST.chpl"
      call_tmp_chpl47 = &((coerce_tmp_chpl8)->ranges);
#line 44 "../lib/MNIST.chpl"
      coerce_tmp_chpl13 = *(*(call_tmp_chpl47) + INT64(1));
#line 44 "../lib/MNIST.chpl"
      call_tmp_chpl48 = sizeAs_chpl37(coerce_tmp_chpl13, INT64(44), INT32(71));
#line 44 "../lib/MNIST.chpl"
      chpl___POUND_3(call_tmp_chpl46, call_tmp_chpl48, &ret_tmp_chpl25, INT64(44), INT32(71));
#line 44 "../lib/MNIST.chpl"
      call_tmp_chpl49 = ret_tmp_chpl25;
#line 44 "../lib/MNIST.chpl"
      *(locBlock_chpl + INT64(1)) = call_tmp_chpl49;
#line 44 "../lib/MNIST.chpl"
      chpl_build_low_bounded_range2(INT64(0), &ret_tmp_chpl26);
#line 44 "../lib/MNIST.chpl"
      call_tmp_chpl50 = ret_tmp_chpl26;
#line 44 "../lib/MNIST.chpl"
      chpl___POUND_3(call_tmp_chpl50, tmp_x0_chpl, &ret_tmp_chpl27, INT64(44), INT32(71));
#line 44 "../lib/MNIST.chpl"
      tmpIter_chpl = ret_tmp_chpl27;
#line 44 "../lib/MNIST.chpl"
      new_temp_chpl = _new_chpl17(INT64(44), INT32(71));
#line 44 "../lib/MNIST.chpl"
      _coforallCount_chpl = new_temp_chpl;
#line 44 "../lib/MNIST.chpl"
      numTasks_chpl = chpl_boundedCoforallSize(tmpIter_chpl, INT64(44), INT32(71));
#line 44 "../lib/MNIST.chpl"
      _upEndCount_chpl(_coforallCount_chpl, numTasks_chpl, INT64(44), INT32(71));
#line 44 "../lib/MNIST.chpl"
      chpl_resetTaskSpawn(numTasks_chpl, INT64(44), INT32(71));
#line 44 "../lib/MNIST.chpl"
      _ic__F0_this_chpl2 = tmpIter_chpl;
#line 44 "../lib/MNIST.chpl"
      checkIfIterWillOverflow_chpl(tmpIter_chpl, UINT8(true), INT64(44), INT32(71));
#line 44 "../lib/MNIST.chpl"
      i_chpl2 = INT64(0);
#line 44 "../lib/MNIST.chpl"
      this_chpl35 = _ic__F0_this_chpl2;
#line 44 "../lib/MNIST.chpl"
      this_chpl36 = _ic__F0_this_chpl2;
#line 44 "../lib/MNIST.chpl"
      end_chpl2 = (&this_chpl36)->_high;
#line 44 "../lib/MNIST.chpl"
      for (i_chpl2 = (&this_chpl35)->_low; ((i_chpl2 <= end_chpl2)); i_chpl2 += INT64(1)) {
#line 44 "../lib/MNIST.chpl"
        _args_forcoforall_fn_chpl = ((_class_localscoforall_fn_chpl18)(&chpl_macro_tmp_159));
#line 44 "../lib/MNIST.chpl"
        chpl_check_nil(_args_forcoforall_fn_chpl, INT64(44), INT32(71));
#line 44 "../lib/MNIST.chpl"
        (_args_forcoforall_fn_chpl)->_0_numChunks = numChunks_chpl;
#line 44 "../lib/MNIST.chpl"
        chpl_check_nil(_args_forcoforall_fn_chpl, INT64(44), INT32(71));
#line 44 "../lib/MNIST.chpl"
        (_args_forcoforall_fn_chpl)->_1_parDim = parDim_chpl;
#line 44 "../lib/MNIST.chpl"
        chpl_check_nil(_args_forcoforall_fn_chpl, INT64(44), INT32(71));
#line 44 "../lib/MNIST.chpl"
        *((_args_forcoforall_fn_chpl)->_2_coerce_tmp + INT64(0)) = *(locBlock_chpl + INT64(0));
#line 44 "../lib/MNIST.chpl"
        *((_args_forcoforall_fn_chpl)->_2_coerce_tmp + INT64(1)) = *(locBlock_chpl + INT64(1));
#line 44 "../lib/MNIST.chpl"
        chpl_check_nil(_args_forcoforall_fn_chpl, INT64(44), INT32(71));
#line 44 "../lib/MNIST.chpl"
        (_args_forcoforall_fn_chpl)->_3__coforallCount = _coforallCount_chpl;
#line 44 "../lib/MNIST.chpl"
        chpl_check_nil(_args_forcoforall_fn_chpl, INT64(44), INT32(71));
#line 44 "../lib/MNIST.chpl"
        (_args_forcoforall_fn_chpl)->_4_chunk = i_chpl2;
#line 44 "../lib/MNIST.chpl"
        chpl_check_nil(_args_forcoforall_fn_chpl, INT64(44), INT32(71));
#line 44 "../lib/MNIST.chpl"
        (_args_forcoforall_fn_chpl)->_6_a = *(call_tmp_chpl24);
#line 44 "../lib/MNIST.chpl"
        chpl_check_nil(_args_forcoforall_fn_chpl, INT64(44), INT32(71));
#line 44 "../lib/MNIST.chpl"
        (_args_forcoforall_fn_chpl)->_7_b = *(i_b_chpl);
#line 44 "../lib/MNIST.chpl"
        _args_vforcoforall_fn_chpl = ((void*)(_args_forcoforall_fn_chpl));
#line 44 "../lib/MNIST.chpl"
        _args_sizecoforall_fn_chpl = sizeof(chpl__class_localscoforall_fn_chpl18_object);
#line 44 "../lib/MNIST.chpl"
        (_args_forcoforall_fn_chpl)->_ln = INT64(44);
#line 44 "../lib/MNIST.chpl"
        (_args_forcoforall_fn_chpl)->_fn = INT32(71);
#line 44 "../lib/MNIST.chpl"
        /*** wrapcoforall_fn_chpl29 ***/ chpl_taskAddCoStmt(chpl_task_getRequestedSubloc(), INT64(28), ((chpl_task_bundle_p)(_args_vforcoforall_fn_chpl)), _args_sizecoforall_fn_chpl, INT64(2763), INT32(30));
#line 44 "../lib/MNIST.chpl"
      }
#line 44 "../lib/MNIST.chpl"
      error_chpl10 = NULL;
#line 44 "../lib/MNIST.chpl"
      _waitEndCount_chpl2(_coforallCount_chpl, numTasks_chpl, &error_chpl10, INT64(44), INT32(71));
#line 44 "../lib/MNIST.chpl"
      if (error_chpl10 != nil) /* ZLINE: 44 ../lib/MNIST.chpl */
#line 44 "../lib/MNIST.chpl"
      {
#line 44 "../lib/MNIST.chpl"
        chpl__delete72(_coforallCount_chpl, INT64(44), INT32(71));
#line 44 "../lib/MNIST.chpl"
        error_chpl9 = error_chpl10;
#line 44 "../lib/MNIST.chpl"
        goto forall_IBB_break_label_chpl;
#line 44 "../lib/MNIST.chpl"
      }
#line 44 "../lib/MNIST.chpl"
      chpl__delete72(_coforallCount_chpl, INT64(44), INT32(71));
#line 44 "../lib/MNIST.chpl"
      chpl_after_forall_fence();
#line 44 "../lib/MNIST.chpl"
      forall_IBB_break_label_chpl:;
#line 44 "../lib/MNIST.chpl"
      if (error_chpl9 != nil) /* ZLINE: 44 ../lib/MNIST.chpl */
#line 44 "../lib/MNIST.chpl"
      {
#line 44 "../lib/MNIST.chpl"
        error_chpl8 = error_chpl9;
#line 44 "../lib/MNIST.chpl"
        goto forall_IBB_break_label_chpl2;
#line 44 "../lib/MNIST.chpl"
      }
#line 44 "../lib/MNIST.chpl"
      chpl_after_forall_fence();
#line 44 "../lib/MNIST.chpl"
      forall_IBB_break_label_chpl2:;
#line 44 "../lib/MNIST.chpl"
      if (error_chpl8 != nil) /* ZLINE: 44 ../lib/MNIST.chpl */
#line 44 "../lib/MNIST.chpl"
      {
#line 44 "../lib/MNIST.chpl"
        chpl_propagate_error(error_chpl8, INT64(44), INT32(71));
#line 44 "../lib/MNIST.chpl"
      }
#line 44 "../lib/MNIST.chpl"
      chpl_after_forall_fence();
#line 44 "../lib/MNIST.chpl"
    }
#line 44 "../lib/MNIST.chpl"
    _end__ASSIGN__chpl:;
#line 44 "../lib/MNIST.chpl"
    chpl__autoDestroy19(&call_tmp_chpl25, INT64(44), INT32(71));
#line 44 "../lib/MNIST.chpl"
  }
#line 43 "../lib/MNIST.chpl"
  chpl__autoDestroy11(&call_tmp_chpl22, INT64(43), INT32(71));
#line 47 "../lib/MNIST.chpl"
  ret_chpl = images_chpl;
#line 47 "../lib/MNIST.chpl"
  chpl__autoDestroy11(&call_tmp_chpl18, INT64(47), INT32(71));
#line 47 "../lib/MNIST.chpl"
  chpl__autoDestroy5(&imageDomain_chpl, INT64(47), INT32(71));
#line 47 "../lib/MNIST.chpl"
  i_x_chpl3 = &fr_chpl;
#line 47 "../lib/MNIST.chpl"
  deinit_chpl98(i_x_chpl3, INT64(47), INT32(71));
#line 6 "../lib/MNIST.chpl"
  *(_retArg_chpl) = ret_chpl;
#line 6 "../lib/MNIST.chpl"
  return;
#line 6 "../lib/MNIST.chpl"
}

#line 30 "../lib/MNIST.chpl"
/* MNIST.chpl:30 */
#line 30 "../lib/MNIST.chpl"
static void readImage_chpl(fileReader_T_BinaryDeserializer_chpl * fr_chpl,
#line 30 "../lib/MNIST.chpl"
                           int32_t * rowCount_chpl,
#line 30 "../lib/MNIST.chpl"
                           int32_t * columnCount_chpl,
#line 30 "../lib/MNIST.chpl"
                           _domain_DefaultRectangularDom_2_int64_t_one_chpl * imageDomain_chpl,
#line 30 "../lib/MNIST.chpl"
                           _array_DefaultRectangularArr_2_int64_t_one__real64_int64_t_chpl * _retArg_chpl) {
#line 31 "../lib/MNIST.chpl"
  _array_DefaultRectangularArr_2_int64_t_one_uint8_t_int64_t_chpl raw_chpl;
#line 31 "../lib/MNIST.chpl"
  _ref__domain_DefaultRectangularDom_2_int64_t_one_chpl call_tmp_chpl6 = NULL;
#line 31 "../lib/MNIST.chpl"
  chpl___RuntimeTypeInfo10 call_tmp_chpl7;
#line 31 "../lib/MNIST.chpl"
  _domain_DefaultRectangularDom_2_int64_t_one_chpl _runtime_type_tmp__chpl;
#line 31 "../lib/MNIST.chpl"
  _array_DefaultRectangularArr_2_int64_t_one_uint8_t_int64_t_chpl ret_tmp_chpl;
#line 32 "../lib/MNIST.chpl"
  int32_t call_tmp_chpl8;
#line 32 "../lib/MNIST.chpl"
  int32_t coerce_tmp_chpl3;
#line 32 "../lib/MNIST.chpl"
  int32_t _ic__F1_high_chpl;
#line 32 "../lib/MNIST.chpl"
  int32_t i_chpl;
#line 32 "../lib/MNIST.chpl"
  int32_t i_chpl2;
#line 33 "../lib/MNIST.chpl"
  int32_t call_tmp_chpl9;
#line 33 "../lib/MNIST.chpl"
  int32_t coerce_tmp_chpl4;
#line 33 "../lib/MNIST.chpl"
  int32_t _ic__F1_high_chpl2;
#line 33 "../lib/MNIST.chpl"
  int32_t i_chpl3;
#line 34 "../lib/MNIST.chpl"
  int64_t coerce_tmp_chpl5;
#line 34 "../lib/MNIST.chpl"
  _ref__array_DefaultRectangularArr_2_int64_t_one_uint8_t_int64_t_chpl i_this_chpl = NULL;
#line 34 "../lib/MNIST.chpl"
  _tuple_2_star_int64_t_chpl coerce_tmp_chpl6;
#line 34 "../lib/MNIST.chpl"
  _ref__tuple_2_star_int64_t_chpl i_i_chpl = NULL;
#line 34 "../lib/MNIST.chpl"
  DefaultRectangularArr_2_int64_t_one_uint8_t_int64_t_chpl ret_chpl = NULL;
#line 34 "../lib/MNIST.chpl"
  DefaultRectangularArr_2_int64_t_one_uint8_t_int64_t_chpl coerce_tmp_chpl7 = NULL;
#line 34 "../lib/MNIST.chpl"
  _tuple_2_star_int64_t_chpl _formal_tmp_in_ind_chpl;
#line 34 "../lib/MNIST.chpl"
  int64_t _formal_tmp_ind_x0_chpl;
#line 34 "../lib/MNIST.chpl"
  int64_t _formal_tmp_ind_x1_chpl;
#line 34 "../lib/MNIST.chpl"
  int64_t ret_chpl2;
#line 34 "../lib/MNIST.chpl"
  int64_t sum_chpl;
#line 34 "../lib/MNIST.chpl"
  _ref__tuple_2_star_int64_t_chpl call_tmp_chpl10 = NULL;
#line 34 "../lib/MNIST.chpl"
  _ref_int64_t_chpl i_lhs_chpl = NULL;
#line 34 "../lib/MNIST.chpl"
  _ref_int64_t_chpl i_lhs_chpl2 = NULL;
#line 34 "../lib/MNIST.chpl"
  _ddata_uint8_t_chpl coerce_tmp_chpl8 = NULL;
#line 34 "../lib/MNIST.chpl"
  _ref_uint8_t_chpl call_tmp_chpl11 = NULL;
#line 34 "../lib/MNIST.chpl"
  uint8_t call_tmp_chpl12;
#line 34 "../lib/MNIST.chpl"
  Error_chpl error_chpl = NULL;
#line 37 "../lib/MNIST.chpl"
  _ir_chpl_promo1__SLASH__chpl call_tmp_chpl13;
#line 37 "../lib/MNIST.chpl"
  _real32 coerce_tmp_chpl9;
#line 37 "../lib/MNIST.chpl"
  _ir_chpl_promo1__SLASH__chpl ret_tmp_chpl2;
#line 37 "../lib/MNIST.chpl"
  DefaultRectangularDom_2_int64_t_one_chpl _ir__shape__chpl = NULL;
#line 37 "../lib/MNIST.chpl"
  _array_DefaultRectangularArr_2_int64_t_one_uint8_t_int64_t_chpl _ir_F0_a_chpl;
#line 37 "../lib/MNIST.chpl"
  _ref__array_DefaultRectangularArr_2_int64_t_one_uint8_t_int64_t_chpl i_arg_chpl = NULL;
#line 37 "../lib/MNIST.chpl"
  _domain_DefaultRectangularDom_2_int64_t_one_chpl call_tmp_chpl14;
#line 37 "../lib/MNIST.chpl"
  _domain_DefaultRectangularDom_2_int64_t_one_chpl ret_tmp_chpl3;
#line 37 "../lib/MNIST.chpl"
  DefaultRectangularDom_2_int64_t_one_chpl ret_chpl3 = NULL;
#line 37 "../lib/MNIST.chpl"
  _ref__domain_DefaultRectangularDom_2_int64_t_one_chpl call_tmp_chpl15 = NULL;
#line 37 "../lib/MNIST.chpl"
  chpl___RuntimeTypeInfo14 call_tmp_chpl16;
#line 37 "../lib/MNIST.chpl"
  _array_DefaultRectangularArr_2_int64_t_one__real64_int64_t_chpl init_coerce_tmp_chpl;
#line 37 "../lib/MNIST.chpl"
  _array_DefaultRectangularArr_2_int64_t_one__real64_int64_t_chpl ret_tmp_chpl4;
#line 31 "../lib/MNIST.chpl"
  call_tmp_chpl6 = chpl__ensureDomainExpr(imageDomain_chpl);
#line 31 "../lib/MNIST.chpl"
  call_tmp_chpl7 = chpl__buildArrayRuntimeType8(call_tmp_chpl6);
#line 31 "../lib/MNIST.chpl"
  _runtime_type_tmp__chpl = (&call_tmp_chpl7)->dom;
#line 31 "../lib/MNIST.chpl"
  chpl__convertRuntimeTypeToValue12(&_runtime_type_tmp__chpl, UINT8(false), &ret_tmp_chpl, INT64(31), INT32(71));
#line 31 "../lib/MNIST.chpl"
  raw_chpl = ret_tmp_chpl;
#line 32 "../lib/MNIST.chpl"
  call_tmp_chpl8 = chpl__nudgeHighBound2(*(rowCount_chpl), INT64(32), INT32(71));
#line 32 "../lib/MNIST.chpl"
  coerce_tmp_chpl3 = ((int32_t)(INT64(0)));
#line 32 "../lib/MNIST.chpl"
  _ic__F1_high_chpl = call_tmp_chpl8;
#line 32 "../lib/MNIST.chpl"
  i_chpl = INT32(0);
#line 32 "../lib/MNIST.chpl"
  chpl_checkIfRangeIterWillOverflow2(coerce_tmp_chpl3, call_tmp_chpl8, INT32(1), coerce_tmp_chpl3, call_tmp_chpl8, UINT8(true), INT64(32), INT32(71));
#line 32 "../lib/MNIST.chpl"
  for (i_chpl = coerce_tmp_chpl3; ((i_chpl <= _ic__F1_high_chpl)); i_chpl += INT32(1)) {
#line 32 "../lib/MNIST.chpl"
    i_chpl2 = i_chpl;
#line 33 "../lib/MNIST.chpl"
    call_tmp_chpl9 = chpl__nudgeHighBound2(*(columnCount_chpl), INT64(33), INT32(71));
#line 33 "../lib/MNIST.chpl"
    coerce_tmp_chpl4 = ((int32_t)(INT64(0)));
#line 33 "../lib/MNIST.chpl"
    _ic__F1_high_chpl2 = call_tmp_chpl9;
#line 33 "../lib/MNIST.chpl"
    i_chpl3 = INT32(0);
#line 33 "../lib/MNIST.chpl"
    chpl_checkIfRangeIterWillOverflow2(coerce_tmp_chpl4, call_tmp_chpl9, INT32(1), coerce_tmp_chpl4, call_tmp_chpl9, UINT8(true), INT64(33), INT32(71));
#line 33 "../lib/MNIST.chpl"
    for (i_chpl3 = coerce_tmp_chpl4; ((i_chpl3 <= _ic__F1_high_chpl2)); i_chpl3 += INT32(1)) {
#line 34 "../lib/MNIST.chpl"
      coerce_tmp_chpl5 = ((int64_t)(i_chpl3));
#line 34 "../lib/MNIST.chpl"
      i_this_chpl = &raw_chpl;
#line 34 "../lib/MNIST.chpl"
      *(coerce_tmp_chpl6 + INT64(0)) = ((int64_t)(i_chpl2));
#line 34 "../lib/MNIST.chpl"
      *(coerce_tmp_chpl6 + INT64(1)) = coerce_tmp_chpl5;
#line 34 "../lib/MNIST.chpl"
      i_i_chpl = &coerce_tmp_chpl6;
#line 34 "../lib/MNIST.chpl"
      ret_chpl = (&raw_chpl)->_instance;
#line 34 "../lib/MNIST.chpl"
      checkAccess_chpl8(i_this_chpl, i_i_chpl, ret_chpl, INT64(34), INT32(71));
#line 34 "../lib/MNIST.chpl"
      coerce_tmp_chpl7 = ret_chpl;
#line 34 "../lib/MNIST.chpl"
      *(_formal_tmp_in_ind_chpl + INT64(0)) = *(coerce_tmp_chpl6 + INT64(0));
#line 34 "../lib/MNIST.chpl"
      *(_formal_tmp_in_ind_chpl + INT64(1)) = *(coerce_tmp_chpl6 + INT64(1));
#line 34 "../lib/MNIST.chpl"
      _formal_tmp_ind_x0_chpl = *(_formal_tmp_in_ind_chpl + INT64(0));
#line 34 "../lib/MNIST.chpl"
      _formal_tmp_ind_x1_chpl = *(_formal_tmp_in_ind_chpl + INT64(1));
#line 34 "../lib/MNIST.chpl"
      sum_chpl = INT64(0);
#line 34 "../lib/MNIST.chpl"
      chpl_check_nil(coerce_tmp_chpl7, INT64(34), INT32(71));
#line 34 "../lib/MNIST.chpl"
      call_tmp_chpl10 = &((coerce_tmp_chpl7)->blk);
#line 34 "../lib/MNIST.chpl"
      i_lhs_chpl = &sum_chpl;
#line 34 "../lib/MNIST.chpl"
      *(i_lhs_chpl) += ((int64_t)((_formal_tmp_ind_x0_chpl * *(*(call_tmp_chpl10) + INT64(0)))));
#line 34 "../lib/MNIST.chpl"
      i_lhs_chpl2 = &sum_chpl;
#line 34 "../lib/MNIST.chpl"
      *(i_lhs_chpl2) += _formal_tmp_ind_x1_chpl;
#line 34 "../lib/MNIST.chpl"
      ret_chpl2 = sum_chpl;
#line 34 "../lib/MNIST.chpl"
      chpl_check_nil(coerce_tmp_chpl7, INT64(34), INT32(71));
#line 34 "../lib/MNIST.chpl"
      coerce_tmp_chpl8 = (coerce_tmp_chpl7)->shiftedData;
#line 34 "../lib/MNIST.chpl"
      call_tmp_chpl11 = (coerce_tmp_chpl8 + ret_chpl2);
#line 34 "../lib/MNIST.chpl"
      error_chpl = NULL;
#line 34 "../lib/MNIST.chpl"
      call_tmp_chpl12 = read_chpl(fr_chpl, &error_chpl, INT64(34), INT32(71));
#line 34 "../lib/MNIST.chpl"
      if (error_chpl != nil) /* ZLINE: 34 ../lib/MNIST.chpl */
#line 34 "../lib/MNIST.chpl"
      {
#line 34 "../lib/MNIST.chpl"
        chpl_uncaught_error(error_chpl, INT64(34), INT32(71));
#line 34 "../lib/MNIST.chpl"
      }
#line 34 "../lib/MNIST.chpl"
      *(call_tmp_chpl11) = call_tmp_chpl12;
#line 34 "../lib/MNIST.chpl"
    }
#line 34 "../lib/MNIST.chpl"
  }
#line 37 "../lib/MNIST.chpl"
  coerce_tmp_chpl9 = ((_real32)(REAL64(0x1.fep+7)));
#line 37 "../lib/MNIST.chpl"
  _ir_F0_a_chpl = raw_chpl;
#line 37 "../lib/MNIST.chpl"
  (&ret_tmp_chpl2)->_shape_ = _ir__shape__chpl;
#line 37 "../lib/MNIST.chpl"
  (&ret_tmp_chpl2)->F0_a = _ir_F0_a_chpl;
#line 37 "../lib/MNIST.chpl"
  (&ret_tmp_chpl2)->F1_b = coerce_tmp_chpl9;
#line 37 "../lib/MNIST.chpl"
  call_tmp_chpl13 = ret_tmp_chpl2;
#line 37 "../lib/MNIST.chpl"
  i_arg_chpl = &raw_chpl;
#line 37 "../lib/MNIST.chpl"
  _dom_chpl15(i_arg_chpl, &ret_tmp_chpl3, INT64(37), INT32(71));
#line 37 "../lib/MNIST.chpl"
  call_tmp_chpl14 = ret_tmp_chpl3;
#line 37 "../lib/MNIST.chpl"
  ret_chpl3 = (&call_tmp_chpl14)->_instance;
#line 37 "../lib/MNIST.chpl"
  chpl__autoDestroy5(&call_tmp_chpl14, INT64(37), INT32(71));
#line 37 "../lib/MNIST.chpl"
  (&call_tmp_chpl13)->_shape_ = ret_chpl3;
#line 37 "../lib/MNIST.chpl"
  call_tmp_chpl15 = chpl__ensureDomainExpr(imageDomain_chpl);
#line 37 "../lib/MNIST.chpl"
  call_tmp_chpl16 = chpl__buildArrayRuntimeType14(call_tmp_chpl15);
#line 37 "../lib/MNIST.chpl"
  chpl__coerceMove(&call_tmp_chpl16, &call_tmp_chpl13, UINT8(false), &ret_tmp_chpl4, INT64(37), INT32(71));
#line 37 "../lib/MNIST.chpl"
  init_coerce_tmp_chpl = ret_tmp_chpl4;
#line 38 "../lib/MNIST.chpl"
  chpl__autoDestroy26(&raw_chpl, INT64(38), INT32(71));
#line 30 "../lib/MNIST.chpl"
  *(_retArg_chpl) = init_coerce_tmp_chpl;
#line 30 "../lib/MNIST.chpl"
  return;
#line 30 "../lib/MNIST.chpl"
}

#line 52 "../lib/MNIST.chpl"
/* MNIST.chpl:52 */
#line 52 "../lib/MNIST.chpl"
static void loadLabels_chpl(int64_t num_chpl,
#line 52 "../lib/MNIST.chpl"
                            string_chpl * fileName_chpl,
#line 52 "../lib/MNIST.chpl"
                            _tuple_2__array_DefaultRectangularArr_1_int64_t_one_int64_t_int64_t__array_DefaultRectangularArr_1_int64_t_one__array_DefaultRectangularArr_1_int64_t_one__real64_int64_t_int64_t_chpl * _retArg_chpl) {
#line 54 "../lib/MNIST.chpl"
  BinaryDeserializer_chpl call_tmp_chpl6;
#line 54 "../lib/MNIST.chpl"
  BinaryDeserializer_chpl initTemp_chpl;
#line 56 "../lib/MNIST.chpl"
  fileReader_T_BinaryDeserializer_chpl call_tmp_chpl7;
#line 56 "../lib/MNIST.chpl"
  Error_chpl error_chpl = NULL;
#line 56 "../lib/MNIST.chpl"
  range_int64_t_low_one_chpl region_chpl;
#line 56 "../lib/MNIST.chpl"
  range_int64_t_low_one_chpl ret_tmp_chpl;
#line 56 "../lib/MNIST.chpl"
  ioHintSet_chpl default_arg_hints_chpl;
#line 56 "../lib/MNIST.chpl"
  Error_chpl error_chpl2 = NULL;
#line 56 "../lib/MNIST.chpl"
  ioHintSet_chpl hints_chpl;
#line 56 "../lib/MNIST.chpl"
  ioHintSet_chpl ret_tmp_chpl2;
#line 56 "../lib/MNIST.chpl"
  BinaryDeserializer_chpl _formal_tmp_in_deserializer_chpl;
#line 56 "../lib/MNIST.chpl"
  Error_chpl error_chpl3 = NULL;
#line 56 "../lib/MNIST.chpl"
  fileReader_T_BinaryDeserializer_chpl ret_tmp_chpl3;
#line 56 "../lib/MNIST.chpl"
  fileReader_T_BinaryDeserializer_chpl fr_chpl;
#line 58 "../lib/MNIST.chpl"
  int32_t call_tmp_chpl8;
#line 58 "../lib/MNIST.chpl"
  Error_chpl error_chpl4 = NULL;
#line 65 "../lib/MNIST.chpl"
  Error_chpl error_chpl5 = NULL;
#line 68 "../lib/MNIST.chpl"
  _array_DefaultRectangularArr_1_int64_t_one_int64_t_int64_t_chpl labels_chpl;
#line 68 "../lib/MNIST.chpl"
  range_int64_t_low_one_chpl call_tmp_chpl9;
#line 68 "../lib/MNIST.chpl"
  range_int64_t_low_one_chpl ret_tmp_chpl4;
#line 68 "../lib/MNIST.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl10;
#line 68 "../lib/MNIST.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl5;
#line 68 "../lib/MNIST.chpl"
  _domain_DefaultRectangularDom_1_int64_t_one_chpl call_tmp_chpl11;
#line 68 "../lib/MNIST.chpl"
  _domain_DefaultRectangularDom_1_int64_t_one_chpl ret_tmp_chpl6;
#line 68 "../lib/MNIST.chpl"
  chpl___RuntimeTypeInfo16 call_tmp_chpl12;
#line 68 "../lib/MNIST.chpl"
  _domain_DefaultRectangularDom_1_int64_t_one_chpl _runtime_type_tmp__chpl;
#line 68 "../lib/MNIST.chpl"
  _array_DefaultRectangularArr_1_int64_t_one_int64_t_int64_t_chpl ret_tmp_chpl7;
#line 70 "../lib/MNIST.chpl"
  _domain_DefaultRectangularDom_1_int64_t_one_chpl call_tmp_chpl13;
#line 70 "../lib/MNIST.chpl"
  _domain_DefaultRectangularDom_1_int64_t_one_chpl ret_tmp_chpl8;
#line 70 "../lib/MNIST.chpl"
  _domain_DefaultRectangularDom_1_int64_t_one_chpl this_chpl31;
#line 70 "../lib/MNIST.chpl"
  DefaultRectangularDom_1_int64_t_one_chpl ret_chpl = NULL;
#line 70 "../lib/MNIST.chpl"
  _tuple_1_star_int64_t_chpl ret_tmp_chpl9;
#line 70 "../lib/MNIST.chpl"
  DefaultRectangularDom_1_int64_t_one_chpl this_chpl32 = NULL;
#line 70 "../lib/MNIST.chpl"
  _ref__tuple_1_star_range_int64_t_both_one_chpl call_tmp_chpl14 = NULL;
#line 70 "../lib/MNIST.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl3;
#line 70 "../lib/MNIST.chpl"
  range_int64_t_both_one_chpl _ic__F0_this_chpl;
#line 70 "../lib/MNIST.chpl"
  int64_t i_chpl;
#line 70 "../lib/MNIST.chpl"
  range_int64_t_both_one_chpl this_chpl33;
#line 70 "../lib/MNIST.chpl"
  range_int64_t_both_one_chpl this_chpl34;
#line 70 "../lib/MNIST.chpl"
  int64_t end_chpl;
#line 71 "../lib/MNIST.chpl"
  _ref__array_DefaultRectangularArr_1_int64_t_one_int64_t_int64_t_chpl i_this_chpl = NULL;
#line 71 "../lib/MNIST.chpl"
  _tuple_1_star_int64_t_chpl coerce_tmp_chpl4;
#line 71 "../lib/MNIST.chpl"
  _ref__tuple_1_star_int64_t_chpl i_i_chpl = NULL;
#line 71 "../lib/MNIST.chpl"
  DefaultRectangularArr_1_int64_t_one_int64_t_int64_t_chpl ret_chpl2 = NULL;
#line 71 "../lib/MNIST.chpl"
  DefaultRectangularArr_1_int64_t_one_int64_t_int64_t_chpl coerce_tmp_chpl5 = NULL;
#line 71 "../lib/MNIST.chpl"
  _tuple_1_star_int64_t_chpl _formal_tmp_in_ind_chpl;
#line 71 "../lib/MNIST.chpl"
  int64_t _formal_tmp_ind_x0_chpl;
#line 71 "../lib/MNIST.chpl"
  _ddata_int64_t_chpl coerce_tmp_chpl6 = NULL;
#line 71 "../lib/MNIST.chpl"
  _ref_int64_t_chpl call_tmp_chpl15 = NULL;
#line 71 "../lib/MNIST.chpl"
  int8_t call_tmp_chpl16;
#line 71 "../lib/MNIST.chpl"
  Error_chpl error_chpl6 = NULL;
#line 75 "../lib/MNIST.chpl"
  _array_DefaultRectangularArr_1_int64_t_one__array_DefaultRectangularArr_1_int64_t_one__real64_int64_t_int64_t_chpl formattedLabels_chpl;
#line 75 "../lib/MNIST.chpl"
  range_int64_t_low_one_chpl call_tmp_chpl17;
#line 75 "../lib/MNIST.chpl"
  range_int64_t_low_one_chpl ret_tmp_chpl10;
#line 75 "../lib/MNIST.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl18;
#line 75 "../lib/MNIST.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl11;
#line 75 "../lib/MNIST.chpl"
  _domain_DefaultRectangularDom_1_int64_t_one_chpl call_tmp_chpl19;
#line 75 "../lib/MNIST.chpl"
  _domain_DefaultRectangularDom_1_int64_t_one_chpl ret_tmp_chpl12;
#line 75 "../lib/MNIST.chpl"
  range_int64_t_low_one_chpl call_tmp_chpl20;
#line 75 "../lib/MNIST.chpl"
  range_int64_t_low_one_chpl ret_tmp_chpl13;
#line 75 "../lib/MNIST.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl21;
#line 75 "../lib/MNIST.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl14;
#line 75 "../lib/MNIST.chpl"
  _domain_DefaultRectangularDom_1_int64_t_one_chpl call_tmp_chpl22;
#line 75 "../lib/MNIST.chpl"
  _domain_DefaultRectangularDom_1_int64_t_one_chpl ret_tmp_chpl15;
#line 75 "../lib/MNIST.chpl"
  chpl___RuntimeTypeInfo13 call_tmp_chpl23;
#line 75 "../lib/MNIST.chpl"
  chpl___RuntimeTypeInfo9 call_tmp_chpl24;
#line 75 "../lib/MNIST.chpl"
  _domain_DefaultRectangularDom_1_int64_t_one_chpl _runtime_type_tmp__chpl2;
#line 75 "../lib/MNIST.chpl"
  chpl___RuntimeTypeInfo13 _runtime_type_tmp__chpl3;
#line 75 "../lib/MNIST.chpl"
  _array_DefaultRectangularArr_1_int64_t_one__array_DefaultRectangularArr_1_int64_t_one__real64_int64_t_int64_t_chpl ret_tmp_chpl16;
#line 77 "../lib/MNIST.chpl"
  _domain_DefaultRectangularDom_1_int64_t_one_chpl call_tmp_chpl25;
#line 77 "../lib/MNIST.chpl"
  _domain_DefaultRectangularDom_1_int64_t_one_chpl ret_tmp_chpl17;
#line 77 "../lib/MNIST.chpl"
  _domain_DefaultRectangularDom_1_int64_t_one_chpl this_chpl35;
#line 77 "../lib/MNIST.chpl"
  DefaultRectangularDom_1_int64_t_one_chpl ret_chpl3 = NULL;
#line 77 "../lib/MNIST.chpl"
  _tuple_1_star_int64_t_chpl ret_tmp_chpl18;
#line 77 "../lib/MNIST.chpl"
  DefaultRectangularDom_1_int64_t_one_chpl this_chpl36 = NULL;
#line 77 "../lib/MNIST.chpl"
  _ref__tuple_1_star_range_int64_t_both_one_chpl call_tmp_chpl26 = NULL;
#line 77 "../lib/MNIST.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl7;
#line 77 "../lib/MNIST.chpl"
  range_int64_t_both_one_chpl _ic__F0_this_chpl2;
#line 77 "../lib/MNIST.chpl"
  int64_t i_chpl2;
#line 77 "../lib/MNIST.chpl"
  range_int64_t_both_one_chpl this_chpl37;
#line 77 "../lib/MNIST.chpl"
  range_int64_t_both_one_chpl this_chpl38;
#line 77 "../lib/MNIST.chpl"
  int64_t end_chpl2;
#line 78 "../lib/MNIST.chpl"
  int64_t _ic__F1_high_chpl;
#line 77 "../lib/MNIST.chpl"
  int64_t i_chpl3;
#line 78 "../lib/MNIST.chpl"
  int64_t i_chpl4;
#line 79 "../lib/MNIST.chpl"
  _real64 tmp_chpl;
#line 79 "../lib/MNIST.chpl"
  _ref__array_DefaultRectangularArr_1_int64_t_one_int64_t_int64_t_chpl i_this_chpl2 = NULL;
#line 79 "../lib/MNIST.chpl"
  _tuple_1_star_int64_t_chpl coerce_tmp_chpl8;
#line 79 "../lib/MNIST.chpl"
  _ref__tuple_1_star_int64_t_chpl i_i_chpl2 = NULL;
#line 79 "../lib/MNIST.chpl"
  DefaultRectangularArr_1_int64_t_one_int64_t_int64_t_chpl ret_chpl4 = NULL;
#line 79 "../lib/MNIST.chpl"
  DefaultRectangularArr_1_int64_t_one_int64_t_int64_t_chpl coerce_tmp_chpl9 = NULL;
#line 79 "../lib/MNIST.chpl"
  _tuple_1_star_int64_t_chpl _formal_tmp_in_ind_chpl2;
#line 79 "../lib/MNIST.chpl"
  int64_t _formal_tmp_ind_x0_chpl2;
#line 79 "../lib/MNIST.chpl"
  _ddata_int64_t_chpl coerce_tmp_chpl10 = NULL;
#line 79 "../lib/MNIST.chpl"
  _ref_int64_t_chpl call_tmp_chpl27 = NULL;
#line 79 "../lib/MNIST.chpl"
  _ref__array_DefaultRectangularArr_1_int64_t_one__array_DefaultRectangularArr_1_int64_t_one__real64_int64_t_int64_t_chpl i_this_chpl3 = NULL;
#line 79 "../lib/MNIST.chpl"
  _tuple_1_star_int64_t_chpl coerce_tmp_chpl11;
#line 79 "../lib/MNIST.chpl"
  _ref__tuple_1_star_int64_t_chpl i_i_chpl3 = NULL;
#line 79 "../lib/MNIST.chpl"
  DefaultRectangularArr_1_int64_t_one__array_DefaultRectangularArr_1_int64_t_one__real64_int64_t_int64_t_chpl ret_chpl5 = NULL;
#line 79 "../lib/MNIST.chpl"
  DefaultRectangularArr_1_int64_t_one__array_DefaultRectangularArr_1_int64_t_one__real64_int64_t_int64_t_chpl coerce_tmp_chpl12 = NULL;
#line 79 "../lib/MNIST.chpl"
  _tuple_1_star_int64_t_chpl _formal_tmp_in_ind_chpl3;
#line 79 "../lib/MNIST.chpl"
  int64_t _formal_tmp_ind_x0_chpl3;
#line 79 "../lib/MNIST.chpl"
  _ddata__array_DefaultRectangularArr_1_int64_t_one__real64_int64_t_chpl coerce_tmp_chpl13 = NULL;
#line 79 "../lib/MNIST.chpl"
  _ref__array_DefaultRectangularArr_1_int64_t_one__real64_int64_t_chpl call_tmp_chpl28 = NULL;
#line 79 "../lib/MNIST.chpl"
  _tuple_1_star_int64_t_chpl coerce_tmp_chpl14;
#line 79 "../lib/MNIST.chpl"
  _ref__tuple_1_star_int64_t_chpl i_i_chpl4 = NULL;
#line 79 "../lib/MNIST.chpl"
  DefaultRectangularArr_1_int64_t_one__real64_int64_t_chpl ret_chpl6 = NULL;
#line 79 "../lib/MNIST.chpl"
  DefaultRectangularArr_1_int64_t_one__real64_int64_t_chpl coerce_tmp_chpl15 = NULL;
#line 79 "../lib/MNIST.chpl"
  _tuple_1_star_int64_t_chpl _formal_tmp_in_ind_chpl4;
#line 79 "../lib/MNIST.chpl"
  int64_t _formal_tmp_ind_x0_chpl4;
#line 79 "../lib/MNIST.chpl"
  _ddata__real64_chpl coerce_tmp_chpl16 = NULL;
#line 79 "../lib/MNIST.chpl"
  _ref__real64_chpl call_tmp_chpl29 = NULL;
#line 83 "../lib/MNIST.chpl"
  _tuple_2__ref__array_DefaultRectangularArr_1_int64_t_one_int64_t_int64_t__ref__array_DefaultRectangularArr_1_int64_t_one__array_DefaultRectangularArr_1_int64_t_one__real64_int64_t_int64_t_chpl call_tmp_chpl30;
#line 83 "../lib/MNIST.chpl"
  _tuple_2__ref__array_DefaultRectangularArr_1_int64_t_one_int64_t_int64_t__ref__array_DefaultRectangularArr_1_int64_t_one__array_DefaultRectangularArr_1_int64_t_one__real64_int64_t_int64_t_chpl ret_tmp_chpl19;
#line 83 "../lib/MNIST.chpl"
  _array_DefaultRectangularArr_1_int64_t_one_int64_t_int64_t_chpl this_x0_chpl;
#line 83 "../lib/MNIST.chpl"
  _array_DefaultRectangularArr_1_int64_t_one__array_DefaultRectangularArr_1_int64_t_one__real64_int64_t_int64_t_chpl this_x1_chpl;
#line 83 "../lib/MNIST.chpl"
  _array_DefaultRectangularArr_1_int64_t_one_int64_t_int64_t_chpl copy_ret_tmp_x0_chpl;
#line 83 "../lib/MNIST.chpl"
  _array_DefaultRectangularArr_1_int64_t_one__array_DefaultRectangularArr_1_int64_t_one__real64_int64_t_int64_t_chpl copy_ret_tmp_x1_chpl;
#line 83 "../lib/MNIST.chpl"
  _tuple_2__array_DefaultRectangularArr_1_int64_t_one_int64_t_int64_t__array_DefaultRectangularArr_1_int64_t_one__array_DefaultRectangularArr_1_int64_t_one__real64_int64_t_int64_t_chpl ret_chpl7;
#line 83 "../lib/MNIST.chpl"
  _array_DefaultRectangularArr_1_int64_t_one_int64_t_int64_t_chpl read_x0_chpl;
#line 83 "../lib/MNIST.chpl"
  _array_DefaultRectangularArr_1_int64_t_one_int64_t_int64_t_chpl elt_x0_chpl;
#line 83 "../lib/MNIST.chpl"
  _array_DefaultRectangularArr_1_int64_t_one__array_DefaultRectangularArr_1_int64_t_one__real64_int64_t_int64_t_chpl read_x1_chpl;
#line 83 "../lib/MNIST.chpl"
  _array_DefaultRectangularArr_1_int64_t_one__array_DefaultRectangularArr_1_int64_t_one__real64_int64_t_int64_t_chpl elt_x1_chpl;
#line 83 "../lib/MNIST.chpl"
  _ref_fileReader_T_BinaryDeserializer_chpl i_x_chpl = NULL;
#line 54 "../lib/MNIST.chpl"
  init_chpl204(&initTemp_chpl, ioendian_chpl_big_chpl, UINT8(true));
#line 54 "../lib/MNIST.chpl"
  call_tmp_chpl6 = initTemp_chpl;
#line 56 "../lib/MNIST.chpl"
  error_chpl = NULL;
#line 56 "../lib/MNIST.chpl"
  chpl_build_low_bounded_range2(INT64(0), &ret_tmp_chpl);
#line 56 "../lib/MNIST.chpl"
  region_chpl = ret_tmp_chpl;
#line 56 "../lib/MNIST.chpl"
  if (error_chpl != nil) /* ZLINE: 56 ../lib/MNIST.chpl */
#line 56 "../lib/MNIST.chpl"
  {
#line 56 "../lib/MNIST.chpl"
    chpl_uncaught_error(error_chpl, INT64(56), INT32(71));
#line 56 "../lib/MNIST.chpl"
  }
#line 56 "../lib/MNIST.chpl"
  error_chpl2 = NULL;
#line 56 "../lib/MNIST.chpl"
  empty_chpl2(&ret_tmp_chpl2);
#line 56 "../lib/MNIST.chpl"
  hints_chpl = ret_tmp_chpl2;
#line 56 "../lib/MNIST.chpl"
  default_arg_hints_chpl = hints_chpl;
#line 56 "../lib/MNIST.chpl"
  if (error_chpl2 != nil) /* ZLINE: 56 ../lib/MNIST.chpl */
#line 56 "../lib/MNIST.chpl"
  {
#line 56 "../lib/MNIST.chpl"
    chpl_uncaught_error(error_chpl2, INT64(56), INT32(71));
#line 56 "../lib/MNIST.chpl"
  }
#line 56 "../lib/MNIST.chpl"
  _formal_tmp_in_deserializer_chpl = call_tmp_chpl6;
#line 56 "../lib/MNIST.chpl"
  error_chpl3 = NULL;
#line 56 "../lib/MNIST.chpl"
  openReader_chpl(fileName_chpl, region_chpl, &default_arg_hints_chpl, &_formal_tmp_in_deserializer_chpl, &error_chpl3, &ret_tmp_chpl3, INT64(56), INT32(71));
#line 56 "../lib/MNIST.chpl"
  call_tmp_chpl7 = ret_tmp_chpl3;
#line 56 "../lib/MNIST.chpl"
  if (error_chpl3 != nil) /* ZLINE: 56 ../lib/MNIST.chpl */
#line 56 "../lib/MNIST.chpl"
  {
#line 56 "../lib/MNIST.chpl"
    chpl_uncaught_error(error_chpl3, INT64(56), INT32(71));
#line 56 "../lib/MNIST.chpl"
  }
#line 56 "../lib/MNIST.chpl"
  fr_chpl = call_tmp_chpl7;
#line 58 "../lib/MNIST.chpl"
  error_chpl4 = NULL;
#line 58 "../lib/MNIST.chpl"
  call_tmp_chpl8 = read_chpl2(&fr_chpl, &error_chpl4, INT64(58), INT32(71));
#line 58 "../lib/MNIST.chpl"
  if (error_chpl4 != nil) /* ZLINE: 58 ../lib/MNIST.chpl */
#line 58 "../lib/MNIST.chpl"
  {
#line 58 "../lib/MNIST.chpl"
    chpl_uncaught_error(error_chpl4, INT64(58), INT32(71));
#line 58 "../lib/MNIST.chpl"
  }
#line 60 "../lib/MNIST.chpl"
  if (call_tmp_chpl8 != ((int32_t)(INT64(2049)))) /* ZLINE: 60 ../lib/MNIST.chpl */
#line 60 "../lib/MNIST.chpl"
  {
#line 61 "../lib/MNIST.chpl"
    writeln_chpl4(&_str_literal_2144_chpl, INT64(61), INT32(71));
#line 62 "../lib/MNIST.chpl"
    halt_chpl14(INT64(1), INT64(62), INT32(71));
#line 62 "../lib/MNIST.chpl"
  }
#line 65 "../lib/MNIST.chpl"
  error_chpl5 = NULL;
#line 65 "../lib/MNIST.chpl"
  read_chpl2(&fr_chpl, &error_chpl5, INT64(65), INT32(71));
#line 65 "../lib/MNIST.chpl"
  if (error_chpl5 != nil) /* ZLINE: 65 ../lib/MNIST.chpl */
#line 65 "../lib/MNIST.chpl"
  {
#line 65 "../lib/MNIST.chpl"
    chpl_uncaught_error(error_chpl5, INT64(65), INT32(71));
#line 65 "../lib/MNIST.chpl"
  }
#line 68 "../lib/MNIST.chpl"
  chpl_build_low_bounded_range2(INT64(0), &ret_tmp_chpl4);
#line 68 "../lib/MNIST.chpl"
  call_tmp_chpl9 = ret_tmp_chpl4;
#line 68 "../lib/MNIST.chpl"
  chpl___POUND_3(call_tmp_chpl9, num_chpl, &ret_tmp_chpl5, INT64(68), INT32(71));
#line 68 "../lib/MNIST.chpl"
  call_tmp_chpl10 = ret_tmp_chpl5;
#line 68 "../lib/MNIST.chpl"
  chpl__ensureDomainExpr5(call_tmp_chpl10, &ret_tmp_chpl6, INT64(68), INT32(71));
#line 68 "../lib/MNIST.chpl"
  call_tmp_chpl11 = ret_tmp_chpl6;
#line 68 "../lib/MNIST.chpl"
  call_tmp_chpl12 = chpl__buildArrayRuntimeType(&call_tmp_chpl11);
#line 68 "../lib/MNIST.chpl"
  _runtime_type_tmp__chpl = (&call_tmp_chpl12)->dom;
#line 68 "../lib/MNIST.chpl"
  chpl__convertRuntimeTypeToValue4(&_runtime_type_tmp__chpl, UINT8(false), &ret_tmp_chpl7, INT64(68), INT32(71));
#line 68 "../lib/MNIST.chpl"
  labels_chpl = ret_tmp_chpl7;
#line 70 "../lib/MNIST.chpl"
  _dom_chpl5(&labels_chpl, &ret_tmp_chpl8, INT64(70), INT32(71));
#line 70 "../lib/MNIST.chpl"
  call_tmp_chpl13 = ret_tmp_chpl8;
#line 70 "../lib/MNIST.chpl"
  this_chpl31 = call_tmp_chpl13;
#line 70 "../lib/MNIST.chpl"
  ret_chpl = (&this_chpl31)->_instance;
#line 70 "../lib/MNIST.chpl"
  createTuple_chpl(INT64(0), &ret_tmp_chpl9);
#line 70 "../lib/MNIST.chpl"
  this_chpl32 = ret_chpl;
#line 70 "../lib/MNIST.chpl"
  chpl_check_nil(this_chpl32, INT64(70), INT32(71));
#line 70 "../lib/MNIST.chpl"
  call_tmp_chpl14 = &((this_chpl32)->ranges);
#line 70 "../lib/MNIST.chpl"
  coerce_tmp_chpl3 = *(*(call_tmp_chpl14) + INT64(0));
#line 70 "../lib/MNIST.chpl"
  _ic__F0_this_chpl = coerce_tmp_chpl3;
#line 70 "../lib/MNIST.chpl"
  checkIfIterWillOverflow_chpl(coerce_tmp_chpl3, UINT8(true), INT64(70), INT32(71));
#line 70 "../lib/MNIST.chpl"
  i_chpl = INT64(0);
#line 70 "../lib/MNIST.chpl"
  this_chpl33 = _ic__F0_this_chpl;
#line 70 "../lib/MNIST.chpl"
  this_chpl34 = _ic__F0_this_chpl;
#line 70 "../lib/MNIST.chpl"
  end_chpl = (&this_chpl34)->_high;
#line 70 "../lib/MNIST.chpl"
  for (i_chpl = (&this_chpl33)->_low; ((i_chpl <= end_chpl)); i_chpl += INT64(1)) {
#line 71 "../lib/MNIST.chpl"
    i_this_chpl = &labels_chpl;
#line 71 "../lib/MNIST.chpl"
    *(coerce_tmp_chpl4 + INT64(0)) = i_chpl;
#line 71 "../lib/MNIST.chpl"
    i_i_chpl = &coerce_tmp_chpl4;
#line 71 "../lib/MNIST.chpl"
    ret_chpl2 = (&labels_chpl)->_instance;
#line 71 "../lib/MNIST.chpl"
    checkAccess_chpl6(i_this_chpl, i_i_chpl, ret_chpl2, INT64(71), INT32(71));
#line 71 "../lib/MNIST.chpl"
    coerce_tmp_chpl5 = ret_chpl2;
#line 71 "../lib/MNIST.chpl"
    *(_formal_tmp_in_ind_chpl + INT64(0)) = *(coerce_tmp_chpl4 + INT64(0));
#line 71 "../lib/MNIST.chpl"
    _formal_tmp_ind_x0_chpl = *(_formal_tmp_in_ind_chpl + INT64(0));
#line 71 "../lib/MNIST.chpl"
    chpl_check_nil(coerce_tmp_chpl5, INT64(71), INT32(71));
#line 71 "../lib/MNIST.chpl"
    coerce_tmp_chpl6 = (coerce_tmp_chpl5)->shiftedData;
#line 71 "../lib/MNIST.chpl"
    call_tmp_chpl15 = (coerce_tmp_chpl6 + _formal_tmp_ind_x0_chpl);
#line 71 "../lib/MNIST.chpl"
    error_chpl6 = NULL;
#line 71 "../lib/MNIST.chpl"
    call_tmp_chpl16 = read_chpl3(&fr_chpl, &error_chpl6, INT64(71), INT32(71));
#line 71 "../lib/MNIST.chpl"
    if (error_chpl6 != nil) /* ZLINE: 71 ../lib/MNIST.chpl */
#line 71 "../lib/MNIST.chpl"
    {
#line 71 "../lib/MNIST.chpl"
      chpl_uncaught_error(error_chpl6, INT64(71), INT32(71));
#line 71 "../lib/MNIST.chpl"
    }
#line 71 "../lib/MNIST.chpl"
    *(call_tmp_chpl15) = ((int64_t)(call_tmp_chpl16));
#line 71 "../lib/MNIST.chpl"
  }
#line 70 "../lib/MNIST.chpl"
  chpl__autoDestroy11(&call_tmp_chpl13, INT64(70), INT32(71));
#line 75 "../lib/MNIST.chpl"
  chpl_build_low_bounded_range2(INT64(0), &ret_tmp_chpl10);
#line 75 "../lib/MNIST.chpl"
  call_tmp_chpl17 = ret_tmp_chpl10;
#line 75 "../lib/MNIST.chpl"
  chpl___POUND_3(call_tmp_chpl17, num_chpl, &ret_tmp_chpl11, INT64(75), INT32(71));
#line 75 "../lib/MNIST.chpl"
  call_tmp_chpl18 = ret_tmp_chpl11;
#line 75 "../lib/MNIST.chpl"
  chpl__ensureDomainExpr5(call_tmp_chpl18, &ret_tmp_chpl12, INT64(75), INT32(71));
#line 75 "../lib/MNIST.chpl"
  call_tmp_chpl19 = ret_tmp_chpl12;
#line 75 "../lib/MNIST.chpl"
  chpl_build_low_bounded_range2(INT64(0), &ret_tmp_chpl13);
#line 75 "../lib/MNIST.chpl"
  call_tmp_chpl20 = ret_tmp_chpl13;
#line 75 "../lib/MNIST.chpl"
  chpl___POUND_3(call_tmp_chpl20, INT64(10), &ret_tmp_chpl14, INT64(75), INT32(71));
#line 75 "../lib/MNIST.chpl"
  call_tmp_chpl21 = ret_tmp_chpl14;
#line 75 "../lib/MNIST.chpl"
  chpl__ensureDomainExpr5(call_tmp_chpl21, &ret_tmp_chpl15, INT64(75), INT32(71));
#line 75 "../lib/MNIST.chpl"
  call_tmp_chpl22 = ret_tmp_chpl15;
#line 75 "../lib/MNIST.chpl"
  call_tmp_chpl23 = chpl__buildArrayRuntimeType12(&call_tmp_chpl22);
#line 75 "../lib/MNIST.chpl"
  call_tmp_chpl24 = chpl__buildArrayRuntimeType5(&call_tmp_chpl19, &call_tmp_chpl23);
#line 75 "../lib/MNIST.chpl"
  _runtime_type_tmp__chpl2 = (&call_tmp_chpl24)->dom;
#line 75 "../lib/MNIST.chpl"
  _runtime_type_tmp__chpl3 = (&call_tmp_chpl24)->eltType;
#line 75 "../lib/MNIST.chpl"
  chpl__convertRuntimeTypeToValue8(&_runtime_type_tmp__chpl2, &_runtime_type_tmp__chpl3, UINT8(false), &ret_tmp_chpl16, INT64(75), INT32(71));
#line 75 "../lib/MNIST.chpl"
  formattedLabels_chpl = ret_tmp_chpl16;
#line 77 "../lib/MNIST.chpl"
  _dom_chpl5(&labels_chpl, &ret_tmp_chpl17, INT64(77), INT32(71));
#line 77 "../lib/MNIST.chpl"
  call_tmp_chpl25 = ret_tmp_chpl17;
#line 77 "../lib/MNIST.chpl"
  this_chpl35 = call_tmp_chpl25;
#line 77 "../lib/MNIST.chpl"
  ret_chpl3 = (&this_chpl35)->_instance;
#line 77 "../lib/MNIST.chpl"
  createTuple_chpl(INT64(0), &ret_tmp_chpl18);
#line 77 "../lib/MNIST.chpl"
  this_chpl36 = ret_chpl3;
#line 77 "../lib/MNIST.chpl"
  chpl_check_nil(this_chpl36, INT64(77), INT32(71));
#line 77 "../lib/MNIST.chpl"
  call_tmp_chpl26 = &((this_chpl36)->ranges);
#line 77 "../lib/MNIST.chpl"
  coerce_tmp_chpl7 = *(*(call_tmp_chpl26) + INT64(0));
#line 77 "../lib/MNIST.chpl"
  _ic__F0_this_chpl2 = coerce_tmp_chpl7;
#line 77 "../lib/MNIST.chpl"
  checkIfIterWillOverflow_chpl(coerce_tmp_chpl7, UINT8(true), INT64(77), INT32(71));
#line 77 "../lib/MNIST.chpl"
  i_chpl2 = INT64(0);
#line 77 "../lib/MNIST.chpl"
  this_chpl37 = _ic__F0_this_chpl2;
#line 77 "../lib/MNIST.chpl"
  this_chpl38 = _ic__F0_this_chpl2;
#line 77 "../lib/MNIST.chpl"
  end_chpl2 = (&this_chpl38)->_high;
#line 78 "../lib/MNIST.chpl"
  _ic__F1_high_chpl = INT64(9);
#line 77 "../lib/MNIST.chpl"
  for (i_chpl2 = (&this_chpl37)->_low; ((i_chpl2 <= end_chpl2)); i_chpl2 += INT64(1)) {
#line 77 "../lib/MNIST.chpl"
    i_chpl3 = i_chpl2;
#line 78 "../lib/MNIST.chpl"
    i_chpl4 = INT64(0);
#line 78 "../lib/MNIST.chpl"
    chpl_checkIfRangeIterWillOverflow(INT64(0), INT64(9), INT64(1), INT64(0), INT64(9), UINT8(true), INT64(78), INT32(71));
#line 78 "../lib/MNIST.chpl"
    for (i_chpl4 = INT64(0); ((i_chpl4 <= _ic__F1_high_chpl)); i_chpl4 += INT64(1)) {
#line 79 "../lib/MNIST.chpl"
      i_this_chpl2 = &labels_chpl;
#line 79 "../lib/MNIST.chpl"
      *(coerce_tmp_chpl8 + INT64(0)) = i_chpl3;
#line 79 "../lib/MNIST.chpl"
      i_i_chpl2 = &coerce_tmp_chpl8;
#line 79 "../lib/MNIST.chpl"
      ret_chpl4 = (&labels_chpl)->_instance;
#line 79 "../lib/MNIST.chpl"
      checkAccess_chpl6(i_this_chpl2, i_i_chpl2, ret_chpl4, INT64(79), INT32(71));
#line 79 "../lib/MNIST.chpl"
      coerce_tmp_chpl9 = ret_chpl4;
#line 79 "../lib/MNIST.chpl"
      *(_formal_tmp_in_ind_chpl2 + INT64(0)) = *(coerce_tmp_chpl8 + INT64(0));
#line 79 "../lib/MNIST.chpl"
      _formal_tmp_ind_x0_chpl2 = *(_formal_tmp_in_ind_chpl2 + INT64(0));
#line 79 "../lib/MNIST.chpl"
      chpl_check_nil(coerce_tmp_chpl9, INT64(79), INT32(71));
#line 79 "../lib/MNIST.chpl"
      coerce_tmp_chpl10 = (coerce_tmp_chpl9)->shiftedData;
#line 79 "../lib/MNIST.chpl"
      call_tmp_chpl27 = (coerce_tmp_chpl10 + _formal_tmp_ind_x0_chpl2);
#line 79 "../lib/MNIST.chpl"
      if (*(call_tmp_chpl27) == i_chpl4) /* ZLINE: 79 ../lib/MNIST.chpl */
#line 79 "../lib/MNIST.chpl"
      {
#line 79 "../lib/MNIST.chpl"
        tmp_chpl = REAL64(0x1p+0);
#line 79 "../lib/MNIST.chpl"
      } else /* ZLINE: 79 ../lib/MNIST.chpl */
#line 79 "../lib/MNIST.chpl"
      {
#line 79 "../lib/MNIST.chpl"
        tmp_chpl = REAL64(0x0p+0);
#line 79 "../lib/MNIST.chpl"
      }
#line 79 "../lib/MNIST.chpl"
      i_this_chpl3 = &formattedLabels_chpl;
#line 79 "../lib/MNIST.chpl"
      *(coerce_tmp_chpl11 + INT64(0)) = i_chpl3;
#line 79 "../lib/MNIST.chpl"
      i_i_chpl3 = &coerce_tmp_chpl11;
#line 79 "../lib/MNIST.chpl"
      ret_chpl5 = (&formattedLabels_chpl)->_instance;
#line 79 "../lib/MNIST.chpl"
      checkAccess_chpl(i_this_chpl3, i_i_chpl3, ret_chpl5, INT64(79), INT32(71));
#line 79 "../lib/MNIST.chpl"
      coerce_tmp_chpl12 = ret_chpl5;
#line 79 "../lib/MNIST.chpl"
      *(_formal_tmp_in_ind_chpl3 + INT64(0)) = *(coerce_tmp_chpl11 + INT64(0));
#line 79 "../lib/MNIST.chpl"
      _formal_tmp_ind_x0_chpl3 = *(_formal_tmp_in_ind_chpl3 + INT64(0));
#line 79 "../lib/MNIST.chpl"
      chpl_check_nil(coerce_tmp_chpl12, INT64(79), INT32(71));
#line 79 "../lib/MNIST.chpl"
      coerce_tmp_chpl13 = (coerce_tmp_chpl12)->shiftedData;
#line 79 "../lib/MNIST.chpl"
      call_tmp_chpl28 = (coerce_tmp_chpl13 + _formal_tmp_ind_x0_chpl3);
#line 79 "../lib/MNIST.chpl"
      *(coerce_tmp_chpl14 + INT64(0)) = i_chpl4;
#line 79 "../lib/MNIST.chpl"
      i_i_chpl4 = &coerce_tmp_chpl14;
#line 79 "../lib/MNIST.chpl"
      ret_chpl6 = (call_tmp_chpl28)->_instance;
#line 79 "../lib/MNIST.chpl"
      checkAccess_chpl11(call_tmp_chpl28, i_i_chpl4, ret_chpl6, INT64(79), INT32(71));
#line 79 "../lib/MNIST.chpl"
      coerce_tmp_chpl15 = ret_chpl6;
#line 79 "../lib/MNIST.chpl"
      *(_formal_tmp_in_ind_chpl4 + INT64(0)) = *(coerce_tmp_chpl14 + INT64(0));
#line 79 "../lib/MNIST.chpl"
      _formal_tmp_ind_x0_chpl4 = *(_formal_tmp_in_ind_chpl4 + INT64(0));
#line 79 "../lib/MNIST.chpl"
      chpl_check_nil(coerce_tmp_chpl15, INT64(79), INT32(71));
#line 79 "../lib/MNIST.chpl"
      coerce_tmp_chpl16 = (coerce_tmp_chpl15)->shiftedData;
#line 79 "../lib/MNIST.chpl"
      call_tmp_chpl29 = (coerce_tmp_chpl16 + _formal_tmp_ind_x0_chpl4);
#line 79 "../lib/MNIST.chpl"
      *(call_tmp_chpl29) = tmp_chpl;
#line 79 "../lib/MNIST.chpl"
    }
#line 79 "../lib/MNIST.chpl"
  }
#line 77 "../lib/MNIST.chpl"
  chpl__autoDestroy11(&call_tmp_chpl25, INT64(77), INT32(71));
#line 83 "../lib/MNIST.chpl"
  this_x0_chpl = labels_chpl;
#line 83 "../lib/MNIST.chpl"
  this_x1_chpl = formattedLabels_chpl;
#line 83 "../lib/MNIST.chpl"
  (&ret_tmp_chpl19)->x0 = this_x0_chpl;
#line 83 "../lib/MNIST.chpl"
  (&ret_tmp_chpl19)->x1 = this_x1_chpl;
#line 83 "../lib/MNIST.chpl"
  call_tmp_chpl30 = ret_tmp_chpl19;
#line 83 "../lib/MNIST.chpl"
  copy_ret_tmp_x0_chpl = (&call_tmp_chpl30)->x0;
#line 83 "../lib/MNIST.chpl"
  copy_ret_tmp_x1_chpl = (&call_tmp_chpl30)->x1;
#line 83 "../lib/MNIST.chpl"
  read_x0_chpl = copy_ret_tmp_x0_chpl;
#line 83 "../lib/MNIST.chpl"
  elt_x0_chpl = chpl__initCopy13(&read_x0_chpl, UINT8(false), INT64(83), INT32(71));
#line 83 "../lib/MNIST.chpl"
  read_x1_chpl = copy_ret_tmp_x1_chpl;
#line 83 "../lib/MNIST.chpl"
  elt_x1_chpl = chpl__initCopy12(&read_x1_chpl, UINT8(false), INT64(83), INT32(71));
#line 83 "../lib/MNIST.chpl"
  (&ret_chpl7)->x0 = elt_x0_chpl;
#line 83 "../lib/MNIST.chpl"
  (&ret_chpl7)->x1 = elt_x1_chpl;
#line 83 "../lib/MNIST.chpl"
  chpl__autoDestroy27(&formattedLabels_chpl, INT64(83), INT32(71));
#line 83 "../lib/MNIST.chpl"
  chpl__autoDestroy11(&call_tmp_chpl22, INT64(83), INT32(71));
#line 83 "../lib/MNIST.chpl"
  chpl__autoDestroy11(&call_tmp_chpl19, INT64(83), INT32(71));
#line 83 "../lib/MNIST.chpl"
  chpl__autoDestroy24(&labels_chpl, INT64(83), INT32(71));
#line 83 "../lib/MNIST.chpl"
  chpl__autoDestroy11(&call_tmp_chpl11, INT64(83), INT32(71));
#line 83 "../lib/MNIST.chpl"
  i_x_chpl = &fr_chpl;
#line 83 "../lib/MNIST.chpl"
  deinit_chpl98(i_x_chpl, INT64(83), INT32(71));
#line 52 "../lib/MNIST.chpl"
  *(_retArg_chpl) = ret_chpl7;
#line 52 "../lib/MNIST.chpl"
  return;
#line 52 "../lib/MNIST.chpl"
}

