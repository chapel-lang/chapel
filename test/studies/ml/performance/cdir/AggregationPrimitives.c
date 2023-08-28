#line 357 "/Users/iainmoncrief/Documents/chapel/modules/packages/CopyAggregation.chpl"
/* CopyAggregation.chpl:357 */
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/packages/CopyAggregation.chpl"
static void chpl__init_AggregationPrimitives(int64_t _ln_chpl,
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/packages/CopyAggregation.chpl"
                                             int32_t _fn_chpl) {
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/packages/CopyAggregation.chpl"
  _ref_int32_t_chpl refIndentLevel_chpl = NULL;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/packages/CopyAggregation.chpl"
  if (chpl__init_AggregationPrimitives_p) /* ZLINE: 357 /Users/iainmoncrief/Documents/chapel/modules/packages/CopyAggregation.chpl */
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/packages/CopyAggregation.chpl"
  {
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/packages/CopyAggregation.chpl"
    goto _exit_chpl__init_AggregationPrimitives_chpl;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/packages/CopyAggregation.chpl"
  }
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/packages/CopyAggregation.chpl"
  printModuleInit_chpl("%*s\n", "AggregationPrimitives", INT64(21), _ln_chpl, _fn_chpl);
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/packages/CopyAggregation.chpl"
  refIndentLevel_chpl = &moduleInitLevel_chpl;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/packages/CopyAggregation.chpl"
  *(refIndentLevel_chpl) += INT64(1);
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/packages/CopyAggregation.chpl"
  chpl__init_AggregationPrimitives_p = UINT8(true);
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/packages/CopyAggregation.chpl"
  {
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/packages/CopyAggregation.chpl"
    chpl__init_CTypes(_ln_chpl, _fn_chpl);
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/packages/CopyAggregation.chpl"
    chpl__init_Communication(_ln_chpl, _fn_chpl);
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/packages/CopyAggregation.chpl"
  }
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/packages/CopyAggregation.chpl"
  *(refIndentLevel_chpl) -= INT64(1);
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/packages/CopyAggregation.chpl"
  _exit_chpl__init_AggregationPrimitives_chpl:;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/packages/CopyAggregation.chpl"
  return;
#line 357 "/Users/iainmoncrief/Documents/chapel/modules/packages/CopyAggregation.chpl"
}

#line 374 "/Users/iainmoncrief/Documents/chapel/modules/packages/CopyAggregation.chpl"
/* CopyAggregation.chpl:374 */
#line 374 "/Users/iainmoncrief/Documents/chapel/modules/packages/CopyAggregation.chpl"
static int64_t getEnvInt_chpl(string_chpl * name_chpl,
#line 374 "/Users/iainmoncrief/Documents/chapel/modules/packages/CopyAggregation.chpl"
                              int64_t default_chpl,
#line 374 "/Users/iainmoncrief/Documents/chapel/modules/packages/CopyAggregation.chpl"
                              int64_t _ln_chpl,
#line 374 "/Users/iainmoncrief/Documents/chapel/modules/packages/CopyAggregation.chpl"
                              int32_t _fn_chpl) {
#line 374 "/Users/iainmoncrief/Documents/chapel/modules/packages/CopyAggregation.chpl"
  int64_t ret_chpl;
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/packages/CopyAggregation.chpl"
  string_chpl call_tmp_chpl6;
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/packages/CopyAggregation.chpl"
  string_chpl ret_chpl2;
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/packages/CopyAggregation.chpl"
  string_chpl ret_chpl3;
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/packages/CopyAggregation.chpl"
  c_ptrConst_int8_t_chpl call_tmp_chpl7 = NULL;
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/packages/CopyAggregation.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 377 "/Users/iainmoncrief/Documents/chapel/modules/packages/CopyAggregation.chpl"
  int64_t call_tmp_chpl8;
#line 377 "/Users/iainmoncrief/Documents/chapel/modules/packages/CopyAggregation.chpl"
  _owned_IllegalArgumentError_chpl2 call_tmp_chpl9;
#line 377 "/Users/iainmoncrief/Documents/chapel/modules/packages/CopyAggregation.chpl"
  _owned_IllegalArgumentError_chpl2 ret_tmp_chpl;
#line 377 "/Users/iainmoncrief/Documents/chapel/modules/packages/CopyAggregation.chpl"
  _ref__owned_IllegalArgumentError_chpl i_arg_chpl = NULL;
#line 377 "/Users/iainmoncrief/Documents/chapel/modules/packages/CopyAggregation.chpl"
  IllegalArgumentError_chpl call_tmp_chpl10 = NULL;
#line 377 "/Users/iainmoncrief/Documents/chapel/modules/packages/CopyAggregation.chpl"
  string_chpl call_tmp_chpl11;
#line 377 "/Users/iainmoncrief/Documents/chapel/modules/packages/CopyAggregation.chpl"
  Error_chpl coerce_tmp_chpl3 = NULL;
#line 377 "/Users/iainmoncrief/Documents/chapel/modules/packages/CopyAggregation.chpl"
  string_chpl ret_tmp_chpl2;
#line 377 "/Users/iainmoncrief/Documents/chapel/modules/packages/CopyAggregation.chpl"
  _ref_string_chpl i_x_chpl2 = NULL;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/packages/CopyAggregation.chpl"
  string_chpl call_tmp_chpl12;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/packages/CopyAggregation.chpl"
  Error_chpl error_chpl = NULL;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/packages/CopyAggregation.chpl"
  string_chpl ret_tmp_chpl3;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/packages/CopyAggregation.chpl"
  string_chpl strval_chpl;
#line 379 "/Users/iainmoncrief/Documents/chapel/modules/packages/CopyAggregation.chpl"
  _ref_string_chpl i_x_chpl3 = NULL;
#line 380 "/Users/iainmoncrief/Documents/chapel/modules/packages/CopyAggregation.chpl"
  int64_t call_tmp_chpl13;
#line 380 "/Users/iainmoncrief/Documents/chapel/modules/packages/CopyAggregation.chpl"
  Error_chpl error_chpl2 = NULL;
#line 380 "/Users/iainmoncrief/Documents/chapel/modules/packages/CopyAggregation.chpl"
  _ref_string_chpl i_x_chpl4 = NULL;
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/packages/CopyAggregation.chpl"
  init_chpl194(&ret_chpl3, _ln_chpl, _fn_chpl);
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/packages/CopyAggregation.chpl"
  (&ret_chpl3)->cachedNumCodepoints = (name_chpl)->cachedNumCodepoints;
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/packages/CopyAggregation.chpl"
  initWithBorrowedBuffer_chpl(&ret_chpl3, name_chpl, _ln_chpl, _fn_chpl);
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/packages/CopyAggregation.chpl"
  ret_chpl2 = ret_chpl3;
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/packages/CopyAggregation.chpl"
  call_tmp_chpl6 = ret_chpl2;
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/packages/CopyAggregation.chpl"
  call_tmp_chpl7 = getenv(((c_ptrConst_int8_t_chpl)((&call_tmp_chpl6)->buff)));
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/packages/CopyAggregation.chpl"
  i_x_chpl = &call_tmp_chpl6;
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/packages/CopyAggregation.chpl"
  deinit_chpl95(i_x_chpl, _ln_chpl, _fn_chpl);
#line 377 "/Users/iainmoncrief/Documents/chapel/modules/packages/CopyAggregation.chpl"
  call_tmp_chpl8 = string_length_bytes(call_tmp_chpl7);
#line 377 "/Users/iainmoncrief/Documents/chapel/modules/packages/CopyAggregation.chpl"
  chpl_checkValue2(call_tmp_chpl8, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 377 "/Users/iainmoncrief/Documents/chapel/modules/packages/CopyAggregation.chpl"
  call_tmp_chpl9 = ret_tmp_chpl;
#line 377 "/Users/iainmoncrief/Documents/chapel/modules/packages/CopyAggregation.chpl"
  i_arg_chpl = &call_tmp_chpl9;
#line 377 "/Users/iainmoncrief/Documents/chapel/modules/packages/CopyAggregation.chpl"
  call_tmp_chpl10 = borrow_chpl2(i_arg_chpl);
#line 377 "/Users/iainmoncrief/Documents/chapel/modules/packages/CopyAggregation.chpl"
  if (((RootClass_chpl)(call_tmp_chpl10)) != nil) /* ZLINE: 377 /Users/iainmoncrief/Documents/chapel/modules/packages/CopyAggregation.chpl */
#line 377 "/Users/iainmoncrief/Documents/chapel/modules/packages/CopyAggregation.chpl"
  {
#line 377 "/Users/iainmoncrief/Documents/chapel/modules/packages/CopyAggregation.chpl"
    coerce_tmp_chpl3 = ((Error_chpl)(call_tmp_chpl10));
#line 377 "/Users/iainmoncrief/Documents/chapel/modules/packages/CopyAggregation.chpl"
    chpl_check_nil(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 377 "/Users/iainmoncrief/Documents/chapel/modules/packages/CopyAggregation.chpl"
    ((void(*)(Error_chpl,
#line 377 "/Users/iainmoncrief/Documents/chapel/modules/packages/CopyAggregation.chpl"
       string_chpl *,
#line 377 "/Users/iainmoncrief/Documents/chapel/modules/packages/CopyAggregation.chpl"
       int64_t,
#line 377 "/Users/iainmoncrief/Documents/chapel/modules/packages/CopyAggregation.chpl"
       int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(coerce_tmp_chpl3))->chpl__cid) + INT64(1))])(coerce_tmp_chpl3, &ret_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 377 "/Users/iainmoncrief/Documents/chapel/modules/packages/CopyAggregation.chpl"
    call_tmp_chpl11 = ret_tmp_chpl2;
#line 377 "/Users/iainmoncrief/Documents/chapel/modules/packages/CopyAggregation.chpl"
    safeCastCheckHalt_chpl(&call_tmp_chpl11, _ln_chpl, _fn_chpl);
#line 377 "/Users/iainmoncrief/Documents/chapel/modules/packages/CopyAggregation.chpl"
    i_x_chpl2 = &call_tmp_chpl11;
#line 377 "/Users/iainmoncrief/Documents/chapel/modules/packages/CopyAggregation.chpl"
    deinit_chpl95(i_x_chpl2, _ln_chpl, _fn_chpl);
#line 377 "/Users/iainmoncrief/Documents/chapel/modules/packages/CopyAggregation.chpl"
  }
#line 377 "/Users/iainmoncrief/Documents/chapel/modules/packages/CopyAggregation.chpl"
  chpl__autoDestroy62(&call_tmp_chpl9, _ln_chpl, _fn_chpl);
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/packages/CopyAggregation.chpl"
  error_chpl = NULL;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/packages/CopyAggregation.chpl"
  createAdoptingBuffer_chpl(call_tmp_chpl7, call_tmp_chpl8, &error_chpl, &ret_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/packages/CopyAggregation.chpl"
  call_tmp_chpl12 = ret_tmp_chpl3;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/packages/CopyAggregation.chpl"
  if (error_chpl != nil) /* ZLINE: 378 /Users/iainmoncrief/Documents/chapel/modules/packages/CopyAggregation.chpl */
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/packages/CopyAggregation.chpl"
  {
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/packages/CopyAggregation.chpl"
    chpl_uncaught_error(error_chpl, _ln_chpl, _fn_chpl);
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/packages/CopyAggregation.chpl"
  }
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/packages/CopyAggregation.chpl"
  strval_chpl = call_tmp_chpl12;
#line 379 "/Users/iainmoncrief/Documents/chapel/modules/packages/CopyAggregation.chpl"
  if ((&strval_chpl)->buffLen == INT64(0)) /* ZLINE: 379 /Users/iainmoncrief/Documents/chapel/modules/packages/CopyAggregation.chpl */
#line 379 "/Users/iainmoncrief/Documents/chapel/modules/packages/CopyAggregation.chpl"
  {
#line 379 "/Users/iainmoncrief/Documents/chapel/modules/packages/CopyAggregation.chpl"
    ret_chpl = default_chpl;
#line 379 "/Users/iainmoncrief/Documents/chapel/modules/packages/CopyAggregation.chpl"
    i_x_chpl3 = &strval_chpl;
#line 379 "/Users/iainmoncrief/Documents/chapel/modules/packages/CopyAggregation.chpl"
    deinit_chpl95(i_x_chpl3, _ln_chpl, _fn_chpl);
#line 379 "/Users/iainmoncrief/Documents/chapel/modules/packages/CopyAggregation.chpl"
    goto _end_getEnvInt_chpl;
#line 379 "/Users/iainmoncrief/Documents/chapel/modules/packages/CopyAggregation.chpl"
  }
#line 380 "/Users/iainmoncrief/Documents/chapel/modules/packages/CopyAggregation.chpl"
  error_chpl2 = NULL;
#line 380 "/Users/iainmoncrief/Documents/chapel/modules/packages/CopyAggregation.chpl"
  call_tmp_chpl13 = chpl___COLON_27(&strval_chpl, &error_chpl2, _ln_chpl, _fn_chpl);
#line 380 "/Users/iainmoncrief/Documents/chapel/modules/packages/CopyAggregation.chpl"
  if (error_chpl2 != nil) /* ZLINE: 380 /Users/iainmoncrief/Documents/chapel/modules/packages/CopyAggregation.chpl */
#line 380 "/Users/iainmoncrief/Documents/chapel/modules/packages/CopyAggregation.chpl"
  {
#line 380 "/Users/iainmoncrief/Documents/chapel/modules/packages/CopyAggregation.chpl"
    chpl_uncaught_error(error_chpl2, _ln_chpl, _fn_chpl);
#line 380 "/Users/iainmoncrief/Documents/chapel/modules/packages/CopyAggregation.chpl"
  }
#line 380 "/Users/iainmoncrief/Documents/chapel/modules/packages/CopyAggregation.chpl"
  ret_chpl = call_tmp_chpl13;
#line 380 "/Users/iainmoncrief/Documents/chapel/modules/packages/CopyAggregation.chpl"
  i_x_chpl4 = &strval_chpl;
#line 380 "/Users/iainmoncrief/Documents/chapel/modules/packages/CopyAggregation.chpl"
  deinit_chpl95(i_x_chpl4, _ln_chpl, _fn_chpl);
#line 380 "/Users/iainmoncrief/Documents/chapel/modules/packages/CopyAggregation.chpl"
  _end_getEnvInt_chpl:;
#line 374 "/Users/iainmoncrief/Documents/chapel/modules/packages/CopyAggregation.chpl"
  return ret_chpl;
#line 374 "/Users/iainmoncrief/Documents/chapel/modules/packages/CopyAggregation.chpl"
}

