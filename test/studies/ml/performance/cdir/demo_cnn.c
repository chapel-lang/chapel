#line 1 "demo_cnn.chpl"
/* demo_cnn.chpl:1 */
#line 1 "demo_cnn.chpl"
static void chpl__init_demo_cnn(int64_t _ln_chpl,
#line 1 "demo_cnn.chpl"
                                int32_t _fn_chpl) {
#line 1 "demo_cnn.chpl"
  _ref_int32_t_chpl refIndentLevel_chpl = NULL;
#line 6 "demo_cnn.chpl"
  Conv_chpl call_tmp_chpl6;
#line 6 "demo_cnn.chpl"
  Conv_chpl initTemp_chpl;
#line 7 "demo_cnn.chpl"
  Conv_chpl call_tmp_chpl7;
#line 7 "demo_cnn.chpl"
  Conv_chpl initTemp_chpl2;
#line 8 "demo_cnn.chpl"
  MaxPool_chpl call_tmp_chpl8;
#line 8 "demo_cnn.chpl"
  MaxPool_chpl initTemp_chpl3;
#line 9 "demo_cnn.chpl"
  SoftMax_chpl call_tmp_chpl9;
#line 9 "demo_cnn.chpl"
  SoftMax_chpl initTemp_chpl4;
#line 5 "demo_cnn.chpl"
  _tuple_4__ref_Conv__ref_Conv__ref_MaxPool__ref_SoftMax_chpl call_tmp_chpl10;
#line 5 "demo_cnn.chpl"
  _tuple_4__ref_Conv__ref_Conv__ref_MaxPool__ref_SoftMax_chpl ret_tmp_chpl;
#line 5 "demo_cnn.chpl"
  _ref_Conv_chpl i__e0_x_chpl = NULL;
#line 5 "demo_cnn.chpl"
  _ref_Conv_chpl i__e1_x_chpl = NULL;
#line 5 "demo_cnn.chpl"
  _ref_MaxPool_chpl i__e2_x_chpl = NULL;
#line 5 "demo_cnn.chpl"
  _ref_SoftMax_chpl i__e3_x_chpl = NULL;
#line 4 "demo_cnn.chpl"
  Network__tuple_4_Conv_Conv_MaxPool_SoftMax_chpl call_tmp_chpl11;
#line 4 "demo_cnn.chpl"
  Network__tuple_4_Conv_Conv_MaxPool_SoftMax_chpl initTemp_chpl5;
#line 4 "demo_cnn.chpl"
  _ref_Tensor_1__real64_chpl _field_destructor_tmp__chpl = NULL;
#line 4 "demo_cnn.chpl"
  _array_DefaultRectangularArr_1_int64_t_one__real64_int64_t_chpl _field_destructor_tmp__chpl2;
#line 4 "demo_cnn.chpl"
  _domain_DefaultRectangularDom_1_int64_t_one_chpl _field_destructor_tmp__chpl3;
#line 4 "demo_cnn.chpl"
  _ref_Tensor_2__real64_chpl _field_destructor_tmp__chpl4 = NULL;
#line 4 "demo_cnn.chpl"
  _array_DefaultRectangularArr_2_int64_t_one__real64_int64_t_chpl _field_destructor_tmp__chpl5;
#line 4 "demo_cnn.chpl"
  _domain_DefaultRectangularDom_2_int64_t_one_chpl _field_destructor_tmp__chpl6;
#line 4 "demo_cnn.chpl"
  _ref_Tensor_1__real64_chpl _field_destructor_tmp__chpl7 = NULL;
#line 4 "demo_cnn.chpl"
  _array_DefaultRectangularArr_1_int64_t_one__real64_int64_t_chpl _field_destructor_tmp__chpl8;
#line 4 "demo_cnn.chpl"
  _domain_DefaultRectangularDom_1_int64_t_one_chpl _field_destructor_tmp__chpl9;
#line 4 "demo_cnn.chpl"
  _ref_Tensor_2__real64_chpl _field_destructor_tmp__chpl10 = NULL;
#line 4 "demo_cnn.chpl"
  _array_DefaultRectangularArr_2_int64_t_one__real64_int64_t_chpl _field_destructor_tmp__chpl11;
#line 4 "demo_cnn.chpl"
  _domain_DefaultRectangularDom_2_int64_t_one_chpl _field_destructor_tmp__chpl12;
#line 4 "demo_cnn.chpl"
  _ref_Tensor_4__real64_chpl _field_destructor_tmp__chpl13 = NULL;
#line 4 "demo_cnn.chpl"
  _array_DefaultRectangularArr_4_int64_t_one__real64_int64_t_chpl _field_destructor_tmp__chpl14;
#line 4 "demo_cnn.chpl"
  _domain_DefaultRectangularDom_4_int64_t_one_chpl _field_destructor_tmp__chpl15;
#line 4 "demo_cnn.chpl"
  _ref_Tensor_4__real64_chpl _field_destructor_tmp__chpl16 = NULL;
#line 4 "demo_cnn.chpl"
  _array_DefaultRectangularArr_4_int64_t_one__real64_int64_t_chpl _field_destructor_tmp__chpl17;
#line 4 "demo_cnn.chpl"
  _domain_DefaultRectangularDom_4_int64_t_one_chpl _field_destructor_tmp__chpl18;
#line 4 "demo_cnn.chpl"
  _ref_Tensor_4__real64_chpl _field_destructor_tmp__chpl19 = NULL;
#line 4 "demo_cnn.chpl"
  _array_DefaultRectangularArr_4_int64_t_one__real64_int64_t_chpl _field_destructor_tmp__chpl20;
#line 4 "demo_cnn.chpl"
  _domain_DefaultRectangularDom_4_int64_t_one_chpl _field_destructor_tmp__chpl21;
#line 4 "demo_cnn.chpl"
  _ref_Tensor_4__real64_chpl _field_destructor_tmp__chpl22 = NULL;
#line 4 "demo_cnn.chpl"
  _array_DefaultRectangularArr_4_int64_t_one__real64_int64_t_chpl _field_destructor_tmp__chpl23;
#line 4 "demo_cnn.chpl"
  _domain_DefaultRectangularDom_4_int64_t_one_chpl _field_destructor_tmp__chpl24;
#line 13 "demo_cnn.chpl"
  int64_t tmp_chpl;
#line 13 "demo_cnn.chpl"
  chpl_bool call_tmp_chpl12;
#line 13 "demo_cnn.chpl"
  c_ptrConst_int8_t_chpl call_tmp_chpl13 = NULL;
#line 14 "demo_cnn.chpl"
  int64_t tmp_chpl2;
#line 14 "demo_cnn.chpl"
  chpl_bool call_tmp_chpl14;
#line 14 "demo_cnn.chpl"
  c_ptrConst_int8_t_chpl call_tmp_chpl15 = NULL;
#line 16 "demo_cnn.chpl"
  _real64 tmp_chpl3;
#line 16 "demo_cnn.chpl"
  chpl_bool call_tmp_chpl16;
#line 16 "demo_cnn.chpl"
  c_ptrConst_int8_t_chpl call_tmp_chpl17 = NULL;
#line 17 "demo_cnn.chpl"
  int64_t tmp_chpl4;
#line 17 "demo_cnn.chpl"
  chpl_bool call_tmp_chpl18;
#line 17 "demo_cnn.chpl"
  c_ptrConst_int8_t_chpl call_tmp_chpl19 = NULL;
#line 18 "demo_cnn.chpl"
  int64_t tmp_chpl5;
#line 18 "demo_cnn.chpl"
  chpl_bool call_tmp_chpl20;
#line 18 "demo_cnn.chpl"
  c_ptrConst_int8_t_chpl call_tmp_chpl21 = NULL;
#line 28 "demo_cnn.chpl"
  string_chpl call_tmp_chpl22;
#line 28 "demo_cnn.chpl"
  string_chpl ret_tmp_chpl2;
#line 28 "demo_cnn.chpl"
  string_chpl call_tmp_chpl23;
#line 28 "demo_cnn.chpl"
  string_chpl ret_tmp_chpl3;
#line 28 "demo_cnn.chpl"
  string_chpl call_tmp_chpl24;
#line 28 "demo_cnn.chpl"
  string_chpl ret_tmp_chpl4;
#line 21 "demo_cnn.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 21 "demo_cnn.chpl"
  _ref_string_chpl i_x_chpl2 = NULL;
#line 21 "demo_cnn.chpl"
  _ref_string_chpl i_x_chpl3 = NULL;
#line 1 "demo_cnn.chpl"
  if (chpl__init_demo_cnn_p) /* ZLINE: 1 demo_cnn.chpl */
#line 1 "demo_cnn.chpl"
  {
#line 1 "demo_cnn.chpl"
    goto _exit_chpl__init_demo_cnn_chpl;
#line 1 "demo_cnn.chpl"
  }
#line 1 "demo_cnn.chpl"
  printModuleInit_chpl("%*s\n", "demo_cnn", INT64(8), INT64(1), INT32(64));
#line 1 "demo_cnn.chpl"
  refIndentLevel_chpl = &moduleInitLevel_chpl;
#line 1 "demo_cnn.chpl"
  *(refIndentLevel_chpl) += INT64(1);
#line 1 "demo_cnn.chpl"
  chpl__init_demo_cnn_p = UINT8(true);
#line 1 "demo_cnn.chpl"
  {
#line 1 "demo_cnn.chpl"
    chpl__init_Chai(INT64(1), INT32(64));
#line 1 "demo_cnn.chpl"
    chpl__init_MNISTTester(INT64(1), INT32(64));
#line 1 "demo_cnn.chpl"
  }
#line 1 "demo_cnn.chpl"
  chpl_addModule("demo_cnn", ((c_fn_ptr)(chpl__deinit_demo_cnn)), INT64(1), INT32(64));
#line 6 "demo_cnn.chpl"
  init_chpl229(&initTemp_chpl, INT64(1), INT64(8), INT64(7), INT64(1), INT64(0));
#line 6 "demo_cnn.chpl"
  call_tmp_chpl6 = initTemp_chpl;
#line 7 "demo_cnn.chpl"
  init_chpl229(&initTemp_chpl2, INT64(8), INT64(12), INT64(5), INT64(1), INT64(0));
#line 7 "demo_cnn.chpl"
  call_tmp_chpl7 = initTemp_chpl2;
#line 8 "demo_cnn.chpl"
  init_chpl230(&initTemp_chpl3);
#line 8 "demo_cnn.chpl"
  call_tmp_chpl8 = initTemp_chpl3;
#line 9 "demo_cnn.chpl"
  init_chpl231(&initTemp_chpl4, INT64(10));
#line 9 "demo_cnn.chpl"
  call_tmp_chpl9 = initTemp_chpl4;
#line 5 "demo_cnn.chpl"
  i__e0_x_chpl = &call_tmp_chpl6;
#line 5 "demo_cnn.chpl"
  i__e1_x_chpl = &call_tmp_chpl7;
#line 5 "demo_cnn.chpl"
  i__e2_x_chpl = &call_tmp_chpl8;
#line 5 "demo_cnn.chpl"
  i__e3_x_chpl = &call_tmp_chpl9;
#line 5 "demo_cnn.chpl"
  (&ret_tmp_chpl)->x0 = i__e0_x_chpl;
#line 5 "demo_cnn.chpl"
  (&ret_tmp_chpl)->x1 = i__e1_x_chpl;
#line 5 "demo_cnn.chpl"
  (&ret_tmp_chpl)->x2 = i__e2_x_chpl;
#line 5 "demo_cnn.chpl"
  (&ret_tmp_chpl)->x3 = i__e3_x_chpl;
#line 5 "demo_cnn.chpl"
  call_tmp_chpl10 = ret_tmp_chpl;
#line 4 "demo_cnn.chpl"
  init_chpl232(&initTemp_chpl5, &call_tmp_chpl10);
#line 4 "demo_cnn.chpl"
  call_tmp_chpl11 = initTemp_chpl5;
#line 4 "demo_cnn.chpl"
  net_chpl = call_tmp_chpl11;
#line 4 "demo_cnn.chpl"
  _field_destructor_tmp__chpl = &((&call_tmp_chpl9)->biasesGrad);
#line 4 "demo_cnn.chpl"
  _field_destructor_tmp__chpl2 = (_field_destructor_tmp__chpl)->data;
#line 4 "demo_cnn.chpl"
  chpl__autoDestroy18(&_field_destructor_tmp__chpl2, INT64(4), INT32(64));
#line 4 "demo_cnn.chpl"
  _field_destructor_tmp__chpl3 = (_field_destructor_tmp__chpl)->_domain;
#line 4 "demo_cnn.chpl"
  chpl__autoDestroy11(&_field_destructor_tmp__chpl3, INT64(4), INT32(64));
#line 4 "demo_cnn.chpl"
  _field_destructor_tmp__chpl4 = &((&call_tmp_chpl9)->weightsGrad);
#line 4 "demo_cnn.chpl"
  _field_destructor_tmp__chpl5 = (_field_destructor_tmp__chpl4)->data;
#line 4 "demo_cnn.chpl"
  chpl__autoDestroy19(&_field_destructor_tmp__chpl5, INT64(4), INT32(64));
#line 4 "demo_cnn.chpl"
  _field_destructor_tmp__chpl6 = (_field_destructor_tmp__chpl4)->_domain;
#line 4 "demo_cnn.chpl"
  chpl__autoDestroy5(&_field_destructor_tmp__chpl6, INT64(4), INT32(64));
#line 4 "demo_cnn.chpl"
  _field_destructor_tmp__chpl7 = &((&call_tmp_chpl9)->biases);
#line 4 "demo_cnn.chpl"
  _field_destructor_tmp__chpl8 = (_field_destructor_tmp__chpl7)->data;
#line 4 "demo_cnn.chpl"
  chpl__autoDestroy18(&_field_destructor_tmp__chpl8, INT64(4), INT32(64));
#line 4 "demo_cnn.chpl"
  _field_destructor_tmp__chpl9 = (_field_destructor_tmp__chpl7)->_domain;
#line 4 "demo_cnn.chpl"
  chpl__autoDestroy11(&_field_destructor_tmp__chpl9, INT64(4), INT32(64));
#line 4 "demo_cnn.chpl"
  _field_destructor_tmp__chpl10 = &((&call_tmp_chpl9)->weights);
#line 4 "demo_cnn.chpl"
  _field_destructor_tmp__chpl11 = (_field_destructor_tmp__chpl10)->data;
#line 4 "demo_cnn.chpl"
  chpl__autoDestroy19(&_field_destructor_tmp__chpl11, INT64(4), INT32(64));
#line 4 "demo_cnn.chpl"
  _field_destructor_tmp__chpl12 = (_field_destructor_tmp__chpl10)->_domain;
#line 4 "demo_cnn.chpl"
  chpl__autoDestroy5(&_field_destructor_tmp__chpl12, INT64(4), INT32(64));
#line 4 "demo_cnn.chpl"
  _field_destructor_tmp__chpl13 = &((&call_tmp_chpl7)->filtersGrad);
#line 4 "demo_cnn.chpl"
  _field_destructor_tmp__chpl14 = (_field_destructor_tmp__chpl13)->data;
#line 4 "demo_cnn.chpl"
  chpl__autoDestroy25(&_field_destructor_tmp__chpl14, INT64(4), INT32(64));
#line 4 "demo_cnn.chpl"
  _field_destructor_tmp__chpl15 = (_field_destructor_tmp__chpl13)->_domain;
#line 4 "demo_cnn.chpl"
  chpl__autoDestroy9(&_field_destructor_tmp__chpl15, INT64(4), INT32(64));
#line 4 "demo_cnn.chpl"
  _field_destructor_tmp__chpl16 = &((&call_tmp_chpl7)->filters);
#line 4 "demo_cnn.chpl"
  _field_destructor_tmp__chpl17 = (_field_destructor_tmp__chpl16)->data;
#line 4 "demo_cnn.chpl"
  chpl__autoDestroy25(&_field_destructor_tmp__chpl17, INT64(4), INT32(64));
#line 4 "demo_cnn.chpl"
  _field_destructor_tmp__chpl18 = (_field_destructor_tmp__chpl16)->_domain;
#line 4 "demo_cnn.chpl"
  chpl__autoDestroy9(&_field_destructor_tmp__chpl18, INT64(4), INT32(64));
#line 4 "demo_cnn.chpl"
  _field_destructor_tmp__chpl19 = &((&call_tmp_chpl6)->filtersGrad);
#line 4 "demo_cnn.chpl"
  _field_destructor_tmp__chpl20 = (_field_destructor_tmp__chpl19)->data;
#line 4 "demo_cnn.chpl"
  chpl__autoDestroy25(&_field_destructor_tmp__chpl20, INT64(4), INT32(64));
#line 4 "demo_cnn.chpl"
  _field_destructor_tmp__chpl21 = (_field_destructor_tmp__chpl19)->_domain;
#line 4 "demo_cnn.chpl"
  chpl__autoDestroy9(&_field_destructor_tmp__chpl21, INT64(4), INT32(64));
#line 4 "demo_cnn.chpl"
  _field_destructor_tmp__chpl22 = &((&call_tmp_chpl6)->filters);
#line 4 "demo_cnn.chpl"
  _field_destructor_tmp__chpl23 = (_field_destructor_tmp__chpl22)->data;
#line 4 "demo_cnn.chpl"
  chpl__autoDestroy25(&_field_destructor_tmp__chpl23, INT64(4), INT32(64));
#line 4 "demo_cnn.chpl"
  _field_destructor_tmp__chpl24 = (_field_destructor_tmp__chpl22)->_domain;
#line 4 "demo_cnn.chpl"
  chpl__autoDestroy9(&_field_destructor_tmp__chpl24, INT64(4), INT32(64));
#line 13 "demo_cnn.chpl"
  call_tmp_chpl12 = chpl_config_has_value(((c_ptrConst_int8_t_chpl)("numTrainImages")), ((c_ptrConst_int8_t_chpl)("demo_cnn")));
#line 13 "demo_cnn.chpl"
  if (! call_tmp_chpl12) /* ZLINE: 13 demo_cnn.chpl */
#line 13 "demo_cnn.chpl"
  {
#line 13 "demo_cnn.chpl"
    tmp_chpl = INT64(1000);
#line 13 "demo_cnn.chpl"
  } else /* ZLINE: 13 demo_cnn.chpl */
#line 13 "demo_cnn.chpl"
  {
#line 13 "demo_cnn.chpl"
    call_tmp_chpl13 = chpl_config_get_value(((c_ptrConst_int8_t_chpl)("numTrainImages")), ((c_ptrConst_int8_t_chpl)("demo_cnn")));
#line 13 "demo_cnn.chpl"
    tmp_chpl = _command_line_cast_chpl13(call_tmp_chpl13, INT64(0), INT32(103));
#line 13 "demo_cnn.chpl"
  }
#line 13 "demo_cnn.chpl"
  numTrainImages_chpl = tmp_chpl;
#line 14 "demo_cnn.chpl"
  call_tmp_chpl14 = chpl_config_has_value(((c_ptrConst_int8_t_chpl)("numTestImages")), ((c_ptrConst_int8_t_chpl)("demo_cnn")));
#line 14 "demo_cnn.chpl"
  if (! call_tmp_chpl14) /* ZLINE: 14 demo_cnn.chpl */
#line 14 "demo_cnn.chpl"
  {
#line 14 "demo_cnn.chpl"
    tmp_chpl2 = INT64(100);
#line 14 "demo_cnn.chpl"
  } else /* ZLINE: 14 demo_cnn.chpl */
#line 14 "demo_cnn.chpl"
  {
#line 14 "demo_cnn.chpl"
    call_tmp_chpl15 = chpl_config_get_value(((c_ptrConst_int8_t_chpl)("numTestImages")), ((c_ptrConst_int8_t_chpl)("demo_cnn")));
#line 14 "demo_cnn.chpl"
    tmp_chpl2 = _command_line_cast_chpl7(call_tmp_chpl15, INT64(0), INT32(102));
#line 14 "demo_cnn.chpl"
  }
#line 14 "demo_cnn.chpl"
  numTestImages_chpl = tmp_chpl2;
#line 16 "demo_cnn.chpl"
  call_tmp_chpl16 = chpl_config_has_value(((c_ptrConst_int8_t_chpl)("learnRate")), ((c_ptrConst_int8_t_chpl)("demo_cnn")));
#line 16 "demo_cnn.chpl"
  if (! call_tmp_chpl16) /* ZLINE: 16 demo_cnn.chpl */
#line 16 "demo_cnn.chpl"
  {
#line 16 "demo_cnn.chpl"
    tmp_chpl3 = REAL64(0x1.999999999999ap-5);
#line 16 "demo_cnn.chpl"
  } else /* ZLINE: 16 demo_cnn.chpl */
#line 16 "demo_cnn.chpl"
  {
#line 16 "demo_cnn.chpl"
    call_tmp_chpl17 = chpl_config_get_value(((c_ptrConst_int8_t_chpl)("learnRate")), ((c_ptrConst_int8_t_chpl)("demo_cnn")));
#line 16 "demo_cnn.chpl"
    tmp_chpl3 = _command_line_cast_chpl5(call_tmp_chpl17, INT64(0), INT32(101));
#line 16 "demo_cnn.chpl"
  }
#line 16 "demo_cnn.chpl"
  learnRate_chpl = tmp_chpl3;
#line 17 "demo_cnn.chpl"
  call_tmp_chpl18 = chpl_config_has_value(((c_ptrConst_int8_t_chpl)("batchSize")), ((c_ptrConst_int8_t_chpl)("demo_cnn")));
#line 17 "demo_cnn.chpl"
  if (! call_tmp_chpl18) /* ZLINE: 17 demo_cnn.chpl */
#line 17 "demo_cnn.chpl"
  {
#line 17 "demo_cnn.chpl"
    tmp_chpl4 = INT64(100);
#line 17 "demo_cnn.chpl"
  } else /* ZLINE: 17 demo_cnn.chpl */
#line 17 "demo_cnn.chpl"
  {
#line 17 "demo_cnn.chpl"
    call_tmp_chpl19 = chpl_config_get_value(((c_ptrConst_int8_t_chpl)("batchSize")), ((c_ptrConst_int8_t_chpl)("demo_cnn")));
#line 17 "demo_cnn.chpl"
    tmp_chpl4 = _command_line_cast_chpl3(call_tmp_chpl19, INT64(0), INT32(100));
#line 17 "demo_cnn.chpl"
  }
#line 17 "demo_cnn.chpl"
  batchSize_chpl = tmp_chpl4;
#line 18 "demo_cnn.chpl"
  call_tmp_chpl20 = chpl_config_has_value(((c_ptrConst_int8_t_chpl)("numEpochs")), ((c_ptrConst_int8_t_chpl)("demo_cnn")));
#line 18 "demo_cnn.chpl"
  if (! call_tmp_chpl20) /* ZLINE: 18 demo_cnn.chpl */
#line 18 "demo_cnn.chpl"
  {
#line 18 "demo_cnn.chpl"
    tmp_chpl5 = INT64(10);
#line 18 "demo_cnn.chpl"
  } else /* ZLINE: 18 demo_cnn.chpl */
#line 18 "demo_cnn.chpl"
  {
#line 18 "demo_cnn.chpl"
    call_tmp_chpl21 = chpl_config_get_value(((c_ptrConst_int8_t_chpl)("numEpochs")), ((c_ptrConst_int8_t_chpl)("demo_cnn")));
#line 18 "demo_cnn.chpl"
    tmp_chpl5 = _command_line_cast_chpl2(call_tmp_chpl21, INT64(0), INT32(99));
#line 18 "demo_cnn.chpl"
  }
#line 18 "demo_cnn.chpl"
  numEpochs_chpl = tmp_chpl5;
#line 28 "demo_cnn.chpl"
  signature_chpl4(&net_chpl, &ret_tmp_chpl2);
#line 28 "demo_cnn.chpl"
  call_tmp_chpl22 = ret_tmp_chpl2;
#line 28 "demo_cnn.chpl"
  chpl___PLUS_(&_str_literal_1677_chpl, &call_tmp_chpl22, &ret_tmp_chpl3, INT64(28), INT32(64));
#line 28 "demo_cnn.chpl"
  call_tmp_chpl23 = ret_tmp_chpl3;
#line 28 "demo_cnn.chpl"
  chpl___PLUS_(&call_tmp_chpl23, &_str_literal_1679_chpl, &ret_tmp_chpl4, INT64(28), INT32(64));
#line 28 "demo_cnn.chpl"
  call_tmp_chpl24 = ret_tmp_chpl4;
#line 21 "demo_cnn.chpl"
  train_chpl2(&net_chpl, numTrainImages_chpl, numTestImages_chpl, learnRate_chpl, batchSize_chpl, numEpochs_chpl, &call_tmp_chpl24);
#line 21 "demo_cnn.chpl"
  i_x_chpl = &call_tmp_chpl24;
#line 21 "demo_cnn.chpl"
  deinit_chpl95(i_x_chpl, INT64(21), INT32(64));
#line 21 "demo_cnn.chpl"
  i_x_chpl2 = &call_tmp_chpl23;
#line 21 "demo_cnn.chpl"
  deinit_chpl95(i_x_chpl2, INT64(21), INT32(64));
#line 21 "demo_cnn.chpl"
  i_x_chpl3 = &call_tmp_chpl22;
#line 21 "demo_cnn.chpl"
  deinit_chpl95(i_x_chpl3, INT64(21), INT32(64));
#line 1 "demo_cnn.chpl"
  *(refIndentLevel_chpl) -= INT64(1);
#line 1 "demo_cnn.chpl"
  _exit_chpl__init_demo_cnn_chpl:;
#line 1 "demo_cnn.chpl"
  return;
#line 1 "demo_cnn.chpl"
}

#line 1 "demo_cnn.chpl"
/* demo_cnn.chpl:1 */
#line 1 "demo_cnn.chpl"
static void chpl_user_main(void) {
#line 1 "demo_cnn.chpl"
  return;
#line 1 "demo_cnn.chpl"
}

#line 1 "demo_cnn.chpl"
/* demo_cnn.chpl:1 */
#line 1 "demo_cnn.chpl"
int64_t chpl_gen_main(chpl_main_argument * _arg) {
#line 1 "demo_cnn.chpl"
  chpl___EndCount_AtomicT_int64_t_int64_t new_temp_chpl = NULL;
#line 1 "demo_cnn.chpl"
  Error_chpl error_chpl = NULL;
#line 1 "demo_cnn.chpl"
  new_temp_chpl = _new_chpl17(INT64(1), INT32(64));
#line 1 "demo_cnn.chpl"
  chpl_task_setDynamicEndCount(new_temp_chpl, INT64(1), INT32(64));
#line 1 "demo_cnn.chpl"
  chpl_rt_preUserCodeHook();
#line 1 "demo_cnn.chpl"
  chpl__init_demo_cnn(INT64(1), INT32(64));
#line 1 "demo_cnn.chpl"
  chpl_user_main();
#line 1 "demo_cnn.chpl"
  chpl_rt_postUserCodeHook();
#line 1 "demo_cnn.chpl"
  error_chpl = NULL;
#line 1 "demo_cnn.chpl"
  _waitEndCount_chpl(new_temp_chpl, &error_chpl, INT64(1), INT32(64));
#line 1 "demo_cnn.chpl"
  if (error_chpl != nil) /* ZLINE: 1 demo_cnn.chpl */
#line 1 "demo_cnn.chpl"
  {
#line 1 "demo_cnn.chpl"
    chpl_uncaught_error(error_chpl, INT64(1), INT32(64));
#line 1 "demo_cnn.chpl"
  }
#line 1 "demo_cnn.chpl"
  chpl_deinitModules();
#line 1 "demo_cnn.chpl"
  return INT64(0);
#line 1 "demo_cnn.chpl"
}

#line 1 "demo_cnn.chpl"
/* demo_cnn.chpl:1 */
#line 1 "demo_cnn.chpl"
static void chpl__deinit_demo_cnn(void) {
#line 4 "demo_cnn.chpl"
  _ref__tuple_4_Conv_Conv_MaxPool_SoftMax_chpl _field_destructor_tmp__chpl = NULL;
#line 4 "demo_cnn.chpl"
  _ref_SoftMax_chpl _field_destructor_tmp__chpl2 = NULL;
#line 4 "demo_cnn.chpl"
  _ref_Tensor_1__real64_chpl _field_destructor_tmp__chpl3 = NULL;
#line 4 "demo_cnn.chpl"
  _array_DefaultRectangularArr_1_int64_t_one__real64_int64_t_chpl _field_destructor_tmp__chpl4;
#line 4 "demo_cnn.chpl"
  _domain_DefaultRectangularDom_1_int64_t_one_chpl _field_destructor_tmp__chpl5;
#line 4 "demo_cnn.chpl"
  _ref_Tensor_2__real64_chpl _field_destructor_tmp__chpl6 = NULL;
#line 4 "demo_cnn.chpl"
  _array_DefaultRectangularArr_2_int64_t_one__real64_int64_t_chpl _field_destructor_tmp__chpl7;
#line 4 "demo_cnn.chpl"
  _domain_DefaultRectangularDom_2_int64_t_one_chpl _field_destructor_tmp__chpl8;
#line 4 "demo_cnn.chpl"
  _ref_Tensor_1__real64_chpl _field_destructor_tmp__chpl9 = NULL;
#line 4 "demo_cnn.chpl"
  _array_DefaultRectangularArr_1_int64_t_one__real64_int64_t_chpl _field_destructor_tmp__chpl10;
#line 4 "demo_cnn.chpl"
  _domain_DefaultRectangularDom_1_int64_t_one_chpl _field_destructor_tmp__chpl11;
#line 4 "demo_cnn.chpl"
  _ref_Tensor_2__real64_chpl _field_destructor_tmp__chpl12 = NULL;
#line 4 "demo_cnn.chpl"
  _array_DefaultRectangularArr_2_int64_t_one__real64_int64_t_chpl _field_destructor_tmp__chpl13;
#line 4 "demo_cnn.chpl"
  _domain_DefaultRectangularDom_2_int64_t_one_chpl _field_destructor_tmp__chpl14;
#line 4 "demo_cnn.chpl"
  _ref_Conv_chpl _field_destructor_tmp__chpl15 = NULL;
#line 4 "demo_cnn.chpl"
  _ref_Tensor_4__real64_chpl _field_destructor_tmp__chpl16 = NULL;
#line 4 "demo_cnn.chpl"
  _array_DefaultRectangularArr_4_int64_t_one__real64_int64_t_chpl _field_destructor_tmp__chpl17;
#line 4 "demo_cnn.chpl"
  _domain_DefaultRectangularDom_4_int64_t_one_chpl _field_destructor_tmp__chpl18;
#line 4 "demo_cnn.chpl"
  _ref_Tensor_4__real64_chpl _field_destructor_tmp__chpl19 = NULL;
#line 4 "demo_cnn.chpl"
  _array_DefaultRectangularArr_4_int64_t_one__real64_int64_t_chpl _field_destructor_tmp__chpl20;
#line 4 "demo_cnn.chpl"
  _domain_DefaultRectangularDom_4_int64_t_one_chpl _field_destructor_tmp__chpl21;
#line 4 "demo_cnn.chpl"
  _ref_Conv_chpl _field_destructor_tmp__chpl22 = NULL;
#line 4 "demo_cnn.chpl"
  _ref_Tensor_4__real64_chpl _field_destructor_tmp__chpl23 = NULL;
#line 4 "demo_cnn.chpl"
  _array_DefaultRectangularArr_4_int64_t_one__real64_int64_t_chpl _field_destructor_tmp__chpl24;
#line 4 "demo_cnn.chpl"
  _domain_DefaultRectangularDom_4_int64_t_one_chpl _field_destructor_tmp__chpl25;
#line 4 "demo_cnn.chpl"
  _ref_Tensor_4__real64_chpl _field_destructor_tmp__chpl26 = NULL;
#line 4 "demo_cnn.chpl"
  _array_DefaultRectangularArr_4_int64_t_one__real64_int64_t_chpl _field_destructor_tmp__chpl27;
#line 4 "demo_cnn.chpl"
  _domain_DefaultRectangularDom_4_int64_t_one_chpl _field_destructor_tmp__chpl28;
#line 4 "demo_cnn.chpl"
  _field_destructor_tmp__chpl = &((&net_chpl)->_layers);
#line 4 "demo_cnn.chpl"
  _field_destructor_tmp__chpl2 = &((_field_destructor_tmp__chpl)->x3);
#line 4 "demo_cnn.chpl"
  _field_destructor_tmp__chpl3 = &((_field_destructor_tmp__chpl2)->biasesGrad);
#line 4 "demo_cnn.chpl"
  _field_destructor_tmp__chpl4 = (_field_destructor_tmp__chpl3)->data;
#line 4 "demo_cnn.chpl"
  chpl__autoDestroy18(&_field_destructor_tmp__chpl4, INT64(4), INT32(64));
#line 4 "demo_cnn.chpl"
  _field_destructor_tmp__chpl5 = (_field_destructor_tmp__chpl3)->_domain;
#line 4 "demo_cnn.chpl"
  chpl__autoDestroy11(&_field_destructor_tmp__chpl5, INT64(4), INT32(64));
#line 4 "demo_cnn.chpl"
  _field_destructor_tmp__chpl6 = &((_field_destructor_tmp__chpl2)->weightsGrad);
#line 4 "demo_cnn.chpl"
  _field_destructor_tmp__chpl7 = (_field_destructor_tmp__chpl6)->data;
#line 4 "demo_cnn.chpl"
  chpl__autoDestroy19(&_field_destructor_tmp__chpl7, INT64(4), INT32(64));
#line 4 "demo_cnn.chpl"
  _field_destructor_tmp__chpl8 = (_field_destructor_tmp__chpl6)->_domain;
#line 4 "demo_cnn.chpl"
  chpl__autoDestroy5(&_field_destructor_tmp__chpl8, INT64(4), INT32(64));
#line 4 "demo_cnn.chpl"
  _field_destructor_tmp__chpl9 = &((_field_destructor_tmp__chpl2)->biases);
#line 4 "demo_cnn.chpl"
  _field_destructor_tmp__chpl10 = (_field_destructor_tmp__chpl9)->data;
#line 4 "demo_cnn.chpl"
  chpl__autoDestroy18(&_field_destructor_tmp__chpl10, INT64(4), INT32(64));
#line 4 "demo_cnn.chpl"
  _field_destructor_tmp__chpl11 = (_field_destructor_tmp__chpl9)->_domain;
#line 4 "demo_cnn.chpl"
  chpl__autoDestroy11(&_field_destructor_tmp__chpl11, INT64(4), INT32(64));
#line 4 "demo_cnn.chpl"
  _field_destructor_tmp__chpl12 = &((_field_destructor_tmp__chpl2)->weights);
#line 4 "demo_cnn.chpl"
  _field_destructor_tmp__chpl13 = (_field_destructor_tmp__chpl12)->data;
#line 4 "demo_cnn.chpl"
  chpl__autoDestroy19(&_field_destructor_tmp__chpl13, INT64(4), INT32(64));
#line 4 "demo_cnn.chpl"
  _field_destructor_tmp__chpl14 = (_field_destructor_tmp__chpl12)->_domain;
#line 4 "demo_cnn.chpl"
  chpl__autoDestroy5(&_field_destructor_tmp__chpl14, INT64(4), INT32(64));
#line 4 "demo_cnn.chpl"
  _field_destructor_tmp__chpl15 = &((_field_destructor_tmp__chpl)->x1);
#line 4 "demo_cnn.chpl"
  _field_destructor_tmp__chpl16 = &((_field_destructor_tmp__chpl15)->filtersGrad);
#line 4 "demo_cnn.chpl"
  _field_destructor_tmp__chpl17 = (_field_destructor_tmp__chpl16)->data;
#line 4 "demo_cnn.chpl"
  chpl__autoDestroy25(&_field_destructor_tmp__chpl17, INT64(4), INT32(64));
#line 4 "demo_cnn.chpl"
  _field_destructor_tmp__chpl18 = (_field_destructor_tmp__chpl16)->_domain;
#line 4 "demo_cnn.chpl"
  chpl__autoDestroy9(&_field_destructor_tmp__chpl18, INT64(4), INT32(64));
#line 4 "demo_cnn.chpl"
  _field_destructor_tmp__chpl19 = &((_field_destructor_tmp__chpl15)->filters);
#line 4 "demo_cnn.chpl"
  _field_destructor_tmp__chpl20 = (_field_destructor_tmp__chpl19)->data;
#line 4 "demo_cnn.chpl"
  chpl__autoDestroy25(&_field_destructor_tmp__chpl20, INT64(4), INT32(64));
#line 4 "demo_cnn.chpl"
  _field_destructor_tmp__chpl21 = (_field_destructor_tmp__chpl19)->_domain;
#line 4 "demo_cnn.chpl"
  chpl__autoDestroy9(&_field_destructor_tmp__chpl21, INT64(4), INT32(64));
#line 4 "demo_cnn.chpl"
  _field_destructor_tmp__chpl22 = &((_field_destructor_tmp__chpl)->x0);
#line 4 "demo_cnn.chpl"
  _field_destructor_tmp__chpl23 = &((_field_destructor_tmp__chpl22)->filtersGrad);
#line 4 "demo_cnn.chpl"
  _field_destructor_tmp__chpl24 = (_field_destructor_tmp__chpl23)->data;
#line 4 "demo_cnn.chpl"
  chpl__autoDestroy25(&_field_destructor_tmp__chpl24, INT64(4), INT32(64));
#line 4 "demo_cnn.chpl"
  _field_destructor_tmp__chpl25 = (_field_destructor_tmp__chpl23)->_domain;
#line 4 "demo_cnn.chpl"
  chpl__autoDestroy9(&_field_destructor_tmp__chpl25, INT64(4), INT32(64));
#line 4 "demo_cnn.chpl"
  _field_destructor_tmp__chpl26 = &((_field_destructor_tmp__chpl22)->filters);
#line 4 "demo_cnn.chpl"
  _field_destructor_tmp__chpl27 = (_field_destructor_tmp__chpl26)->data;
#line 4 "demo_cnn.chpl"
  chpl__autoDestroy25(&_field_destructor_tmp__chpl27, INT64(4), INT32(64));
#line 4 "demo_cnn.chpl"
  _field_destructor_tmp__chpl28 = (_field_destructor_tmp__chpl26)->_domain;
#line 4 "demo_cnn.chpl"
  chpl__autoDestroy9(&_field_destructor_tmp__chpl28, INT64(4), INT32(64));
#line 1 "demo_cnn.chpl"
  return;
#line 1 "demo_cnn.chpl"
}

