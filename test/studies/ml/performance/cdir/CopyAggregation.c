#line 87 "/Users/iainmoncrief/Documents/chapel/modules/packages/CopyAggregation.chpl"
/* CopyAggregation.chpl:87 */
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/packages/CopyAggregation.chpl"
static void chpl__init_CopyAggregation(int64_t _ln_chpl,
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/packages/CopyAggregation.chpl"
                                       int32_t _fn_chpl) {
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/packages/CopyAggregation.chpl"
  _ref_int32_t_chpl refIndentLevel_chpl = NULL;
#line 99 "/Users/iainmoncrief/Documents/chapel/modules/packages/CopyAggregation.chpl"
  int64_t call_tmp_chpl6;
#line 100 "/Users/iainmoncrief/Documents/chapel/modules/packages/CopyAggregation.chpl"
  int64_t call_tmp_chpl7;
#line 101 "/Users/iainmoncrief/Documents/chapel/modules/packages/CopyAggregation.chpl"
  int64_t call_tmp_chpl8;
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/packages/CopyAggregation.chpl"
  if (chpl__init_CopyAggregation_p) /* ZLINE: 87 /Users/iainmoncrief/Documents/chapel/modules/packages/CopyAggregation.chpl */
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/packages/CopyAggregation.chpl"
  {
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/packages/CopyAggregation.chpl"
    goto _exit_chpl__init_CopyAggregation_chpl;
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/packages/CopyAggregation.chpl"
  }
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/packages/CopyAggregation.chpl"
  printModuleInit_chpl("%*s\n", "CopyAggregation", INT64(15), _ln_chpl, _fn_chpl);
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/packages/CopyAggregation.chpl"
  refIndentLevel_chpl = &moduleInitLevel_chpl;
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/packages/CopyAggregation.chpl"
  *(refIndentLevel_chpl) += INT64(1);
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/packages/CopyAggregation.chpl"
  chpl__init_CopyAggregation_p = UINT8(true);
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/packages/CopyAggregation.chpl"
  {
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/packages/CopyAggregation.chpl"
    chpl__init_CTypes(_ln_chpl, _fn_chpl);
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/packages/CopyAggregation.chpl"
    chpl__init_AggregationPrimitives(_ln_chpl, _fn_chpl);
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/packages/CopyAggregation.chpl"
    chpl__init_currentTask(_ln_chpl, _fn_chpl);
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/packages/CopyAggregation.chpl"
    chpl__init_String(_ln_chpl, _fn_chpl);
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/packages/CopyAggregation.chpl"
  }
#line 99 "/Users/iainmoncrief/Documents/chapel/modules/packages/CopyAggregation.chpl"
  call_tmp_chpl6 = getEnvInt_chpl(&_str_literal_3057_chpl, INT64(1024), _ln_chpl, _fn_chpl);
#line 99 "/Users/iainmoncrief/Documents/chapel/modules/packages/CopyAggregation.chpl"
  yieldFrequency_chpl = call_tmp_chpl6;
#line 100 "/Users/iainmoncrief/Documents/chapel/modules/packages/CopyAggregation.chpl"
  call_tmp_chpl7 = getEnvInt_chpl(&_str_literal_3058_chpl, INT64(8192), _ln_chpl, _fn_chpl);
#line 100 "/Users/iainmoncrief/Documents/chapel/modules/packages/CopyAggregation.chpl"
  dstBuffSize_chpl = call_tmp_chpl7;
#line 101 "/Users/iainmoncrief/Documents/chapel/modules/packages/CopyAggregation.chpl"
  call_tmp_chpl8 = getEnvInt_chpl(&_str_literal_3059_chpl, INT64(8192), _ln_chpl, _fn_chpl);
#line 101 "/Users/iainmoncrief/Documents/chapel/modules/packages/CopyAggregation.chpl"
  srcBuffSize_chpl = call_tmp_chpl8;
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/packages/CopyAggregation.chpl"
  *(refIndentLevel_chpl) -= INT64(1);
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/packages/CopyAggregation.chpl"
  _exit_chpl__init_CopyAggregation_chpl:;
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/packages/CopyAggregation.chpl"
  return;
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/packages/CopyAggregation.chpl"
}

