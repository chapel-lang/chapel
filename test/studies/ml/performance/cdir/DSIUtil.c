#line 1 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
/* DSIUtil.chpl:1 */
#line 1 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
static void chpl__init_DSIUtil(int64_t _ln_chpl,
#line 1 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
                               int32_t _fn_chpl) {
#line 1 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_int32_t_chpl refIndentLevel_chpl = NULL;
#line 1 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  if (chpl__init_DSIUtil_p) /* ZLINE: 1 /Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl */
#line 1 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  {
#line 1 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    goto _exit_chpl__init_DSIUtil_chpl;
#line 1 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  }
#line 1 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  printModuleInit_chpl("%*s\n", "DSIUtil", INT64(7), _ln_chpl, _fn_chpl);
#line 1 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  refIndentLevel_chpl = &moduleInitLevel_chpl;
#line 1 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(refIndentLevel_chpl) += INT64(1);
#line 1 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  chpl__init_DSIUtil_p = UINT8(true);
#line 1 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(refIndentLevel_chpl) -= INT64(1);
#line 1 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _exit_chpl__init_DSIUtil_chpl:;
#line 1 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  return;
#line 1 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
}

#line 38 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
/* DSIUtil.chpl:38 */
#line 38 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
static void createTuple_chpl(int64_t val_chpl,
#line 38 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
                             _tuple_1_star_int64_t_chpl * _retArg_chpl) {
#line 38 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _tuple_1_star_int64_t_chpl ret_chpl;
#line 39 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(ret_chpl + INT64(0)) = val_chpl;
#line 38 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(*(_retArg_chpl) + INT64(0)) = *(ret_chpl + INT64(0));
#line 38 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  return;
#line 38 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
}

#line 38 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
/* DSIUtil.chpl:38 */
#line 38 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
static void createTuple_chpl3(int64_t val_chpl,
#line 38 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
                              _tuple_4_star_int64_t_chpl * _retArg_chpl) {
#line 38 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _tuple_4_star_int64_t_chpl ret_chpl;
#line 39 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(ret_chpl + INT64(0)) = val_chpl;
#line 39 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(ret_chpl + INT64(1)) = val_chpl;
#line 39 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(ret_chpl + INT64(2)) = val_chpl;
#line 39 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(ret_chpl + INT64(3)) = val_chpl;
#line 38 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(*(_retArg_chpl) + INT64(0)) = *(ret_chpl + INT64(0));
#line 38 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(*(_retArg_chpl) + INT64(1)) = *(ret_chpl + INT64(1));
#line 38 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(*(_retArg_chpl) + INT64(2)) = *(ret_chpl + INT64(2));
#line 38 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(*(_retArg_chpl) + INT64(3)) = *(ret_chpl + INT64(3));
#line 38 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  return;
#line 38 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
}

#line 38 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
/* DSIUtil.chpl:38 */
#line 38 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
static void createTuple_chpl4(int64_t val_chpl,
#line 38 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
                              _tuple_2_star_int64_t_chpl * _retArg_chpl) {
#line 38 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _tuple_2_star_int64_t_chpl ret_chpl;
#line 39 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(ret_chpl + INT64(0)) = val_chpl;
#line 39 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(ret_chpl + INT64(1)) = val_chpl;
#line 38 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(*(_retArg_chpl) + INT64(0)) = *(ret_chpl + INT64(0));
#line 38 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(*(_retArg_chpl) + INT64(1)) = *(ret_chpl + INT64(1));
#line 38 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  return;
#line 38 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
}

#line 38 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
/* DSIUtil.chpl:38 */
#line 38 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
static void createTuple_chpl2(int64_t val_chpl,
#line 38 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
                              _tuple_3_star_int64_t_chpl * _retArg_chpl) {
#line 38 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _tuple_3_star_int64_t_chpl ret_chpl;
#line 39 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(ret_chpl + INT64(0)) = val_chpl;
#line 39 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(ret_chpl + INT64(1)) = val_chpl;
#line 39 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(ret_chpl + INT64(2)) = val_chpl;
#line 38 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(*(_retArg_chpl) + INT64(0)) = *(ret_chpl + INT64(0));
#line 38 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(*(_retArg_chpl) + INT64(1)) = *(ret_chpl + INT64(1));
#line 38 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(*(_retArg_chpl) + INT64(2)) = *(ret_chpl + INT64(2));
#line 38 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  return;
#line 38 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
}

#line 48 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
/* DSIUtil.chpl:48 */
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
static void _computeChunkStuff_chpl2(int64_t maxTasks_chpl,
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
                                     chpl_bool ignoreRunning_chpl,
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
                                     int64_t minSize_chpl,
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
                                     _tuple_4_star_range_int64_t_both_one_chpl * ranges_chpl,
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
                                     _tuple_2_star_int64_t_chpl * _retArg_chpl,
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
                                     int64_t _ln_chpl,
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
                                     int32_t _fn_chpl) {
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _tuple_2_star_int64_t_chpl ret_chpl;
#line 53 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  uint64_t numElems_chpl;
#line 55 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  uint64_t call_tmp_chpl6;
#line 55 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl3;
#line 55 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_uint64_t_chpl i_lhs_chpl = NULL;
#line 55 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  uint64_t call_tmp_chpl7;
#line 55 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl4;
#line 55 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_uint64_t_chpl i_lhs_chpl2 = NULL;
#line 55 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  uint64_t call_tmp_chpl8;
#line 55 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl5;
#line 55 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_uint64_t_chpl i_lhs_chpl3 = NULL;
#line 55 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  uint64_t call_tmp_chpl9;
#line 55 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl6;
#line 55 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_uint64_t_chpl i_lhs_chpl4 = NULL;
#line 58 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  int64_t call_tmp_chpl10;
#line 58 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  int64_t numChunks_chpl;
#line 60 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _tuple_2_star_int64_t_chpl ret_tmp_chpl;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  int64_t parDim_chpl;
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  int64_t maxDim_chpl;
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  uint64_t maxElems_chpl;
#line 68 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  int64_t _ic__F1_high_chpl;
#line 68 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  int64_t i_chpl;
#line 69 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl11 = NULL;
#line 69 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  uint64_t call_tmp_chpl12;
#line 69 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl7;
#line 89 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _tuple_2_star_int64_t_chpl ret_tmp_chpl2;
#line 53 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  numElems_chpl = UINT64(1);
#line 55 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  coerce_tmp_chpl3 = *(*(ranges_chpl) + INT64(0));
#line 55 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  call_tmp_chpl6 = sizeAs_chpl36(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 55 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  i_lhs_chpl = &numElems_chpl;
#line 55 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(i_lhs_chpl) *= call_tmp_chpl6;
#line 55 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  coerce_tmp_chpl4 = *(*(ranges_chpl) + INT64(1));
#line 55 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  call_tmp_chpl7 = sizeAs_chpl36(coerce_tmp_chpl4, _ln_chpl, _fn_chpl);
#line 55 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  i_lhs_chpl2 = &numElems_chpl;
#line 55 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(i_lhs_chpl2) *= call_tmp_chpl7;
#line 55 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  coerce_tmp_chpl5 = *(*(ranges_chpl) + INT64(2));
#line 55 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  call_tmp_chpl8 = sizeAs_chpl36(coerce_tmp_chpl5, _ln_chpl, _fn_chpl);
#line 55 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  i_lhs_chpl3 = &numElems_chpl;
#line 55 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(i_lhs_chpl3) *= call_tmp_chpl8;
#line 55 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  coerce_tmp_chpl6 = *(*(ranges_chpl) + INT64(3));
#line 55 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  call_tmp_chpl9 = sizeAs_chpl36(coerce_tmp_chpl6, _ln_chpl, _fn_chpl);
#line 55 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  i_lhs_chpl4 = &numElems_chpl;
#line 55 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(i_lhs_chpl4) *= call_tmp_chpl9;
#line 58 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  call_tmp_chpl10 = _computeNumChunks_chpl2(maxTasks_chpl, ignoreRunning_chpl, minSize_chpl, numElems_chpl, _ln_chpl, _fn_chpl);
#line 58 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  numChunks_chpl = call_tmp_chpl10;
#line 59 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  if (call_tmp_chpl10 == INT64(0)) /* ZLINE: 59 /Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl */
#line 59 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  {
#line 60 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    *(ret_tmp_chpl + INT64(0)) = INT64(0);
#line 60 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    *(ret_tmp_chpl + INT64(1)) = INT64(-1);
#line 60 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    *(ret_chpl + INT64(0)) = *(ret_tmp_chpl + INT64(0));
#line 60 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    *(ret_chpl + INT64(1)) = *(ret_tmp_chpl + INT64(1));
#line 60 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    goto _end__computeChunkStuff_chpl;
#line 60 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  }
#line 61 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  assert_chpl((call_tmp_chpl10 > INT64(0)), _ln_chpl, _fn_chpl);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  parDim_chpl = INT64(-1);
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  maxDim_chpl = INT64(-1);
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  maxElems_chpl = UINT64(0);
#line 68 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ic__F1_high_chpl = INT64(3);
#line 68 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  i_chpl = INT64(0);
#line 68 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  chpl_checkIfRangeIterWillOverflow(INT64(0), INT64(3), INT64(1), INT64(0), INT64(3), UINT8(true), _ln_chpl, _fn_chpl);
#line 68 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  for (i_chpl = INT64(0); ((i_chpl <= _ic__F1_high_chpl)); i_chpl += INT64(1)) {
#line 69 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    call_tmp_chpl11 = this_chpl22(ranges_chpl, i_chpl, _ln_chpl, _fn_chpl);
#line 69 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    coerce_tmp_chpl7 = *(call_tmp_chpl11);
#line 69 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    call_tmp_chpl12 = sizeAs_chpl36(coerce_tmp_chpl7, _ln_chpl, _fn_chpl);
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    if (call_tmp_chpl12 >= ((uint64_t)(numChunks_chpl))) /* ZLINE: 70 /Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl */
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    {
#line 71 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      parDim_chpl = i_chpl;
#line 72 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      goto _breakLabel_chpl;
#line 72 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    }
#line 74 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    if (call_tmp_chpl12 > maxElems_chpl) /* ZLINE: 74 /Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl */
#line 74 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    {
#line 75 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      maxElems_chpl = call_tmp_chpl12;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      maxDim_chpl = i_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    }
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  }
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _breakLabel_chpl:;
#line 80 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  if (parDim_chpl == INT64(-1)) /* ZLINE: 80 /Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl */
#line 80 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  {
#line 81 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    parDim_chpl = maxDim_chpl;
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    if (maxElems_chpl < ((uint64_t)(numChunks_chpl))) /* ZLINE: 85 /Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl */
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    {
#line 86 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      numChunks_chpl = ((int64_t)(maxElems_chpl));
#line 86 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    }
#line 86 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  }
#line 89 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(ret_tmp_chpl2 + INT64(0)) = numChunks_chpl;
#line 89 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(ret_tmp_chpl2 + INT64(1)) = parDim_chpl;
#line 89 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(ret_chpl + INT64(0)) = *(ret_tmp_chpl2 + INT64(0));
#line 89 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(ret_chpl + INT64(1)) = *(ret_tmp_chpl2 + INT64(1));
#line 89 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _end__computeChunkStuff_chpl:;
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(*(_retArg_chpl) + INT64(0)) = *(ret_chpl + INT64(0));
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(*(_retArg_chpl) + INT64(1)) = *(ret_chpl + INT64(1));
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  return;
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
}

#line 48 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
/* DSIUtil.chpl:48 */
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
static void _computeChunkStuff_chpl4(int64_t maxTasks_chpl,
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
                                     chpl_bool ignoreRunning_chpl,
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
                                     int64_t minSize_chpl,
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
                                     _tuple_2_star_range_int64_t_both_one_chpl * ranges_chpl,
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
                                     _tuple_2_star_int64_t_chpl * _retArg_chpl,
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
                                     int64_t _ln_chpl,
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
                                     int32_t _fn_chpl) {
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _tuple_2_star_int64_t_chpl ret_chpl;
#line 53 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  uint64_t numElems_chpl;
#line 55 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  uint64_t call_tmp_chpl6;
#line 55 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl3;
#line 55 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_uint64_t_chpl i_lhs_chpl = NULL;
#line 55 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  uint64_t call_tmp_chpl7;
#line 55 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl4;
#line 55 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_uint64_t_chpl i_lhs_chpl2 = NULL;
#line 58 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  int64_t call_tmp_chpl8;
#line 58 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  int64_t numChunks_chpl;
#line 60 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _tuple_2_star_int64_t_chpl ret_tmp_chpl;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  int64_t parDim_chpl;
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  int64_t maxDim_chpl;
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  uint64_t maxElems_chpl;
#line 68 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  int64_t _ic__F1_high_chpl;
#line 68 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  int64_t i_chpl;
#line 69 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl9 = NULL;
#line 69 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  uint64_t call_tmp_chpl10;
#line 69 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl5;
#line 89 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _tuple_2_star_int64_t_chpl ret_tmp_chpl2;
#line 53 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  numElems_chpl = UINT64(1);
#line 55 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  coerce_tmp_chpl3 = *(*(ranges_chpl) + INT64(0));
#line 55 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  call_tmp_chpl6 = sizeAs_chpl36(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 55 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  i_lhs_chpl = &numElems_chpl;
#line 55 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(i_lhs_chpl) *= call_tmp_chpl6;
#line 55 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  coerce_tmp_chpl4 = *(*(ranges_chpl) + INT64(1));
#line 55 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  call_tmp_chpl7 = sizeAs_chpl36(coerce_tmp_chpl4, _ln_chpl, _fn_chpl);
#line 55 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  i_lhs_chpl2 = &numElems_chpl;
#line 55 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(i_lhs_chpl2) *= call_tmp_chpl7;
#line 58 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  call_tmp_chpl8 = _computeNumChunks_chpl2(maxTasks_chpl, ignoreRunning_chpl, minSize_chpl, numElems_chpl, _ln_chpl, _fn_chpl);
#line 58 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  numChunks_chpl = call_tmp_chpl8;
#line 59 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  if (call_tmp_chpl8 == INT64(0)) /* ZLINE: 59 /Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl */
#line 59 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  {
#line 60 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    *(ret_tmp_chpl + INT64(0)) = INT64(0);
#line 60 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    *(ret_tmp_chpl + INT64(1)) = INT64(-1);
#line 60 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    *(ret_chpl + INT64(0)) = *(ret_tmp_chpl + INT64(0));
#line 60 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    *(ret_chpl + INT64(1)) = *(ret_tmp_chpl + INT64(1));
#line 60 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    goto _end__computeChunkStuff_chpl;
#line 60 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  }
#line 61 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  assert_chpl((call_tmp_chpl8 > INT64(0)), _ln_chpl, _fn_chpl);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  parDim_chpl = INT64(-1);
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  maxDim_chpl = INT64(-1);
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  maxElems_chpl = UINT64(0);
#line 68 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ic__F1_high_chpl = INT64(1);
#line 68 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  i_chpl = INT64(0);
#line 68 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  chpl_checkIfRangeIterWillOverflow(INT64(0), INT64(1), INT64(1), INT64(0), INT64(1), UINT8(true), _ln_chpl, _fn_chpl);
#line 68 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  for (i_chpl = INT64(0); ((i_chpl <= _ic__F1_high_chpl)); i_chpl += INT64(1)) {
#line 69 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    call_tmp_chpl9 = this_chpl26(ranges_chpl, i_chpl, _ln_chpl, _fn_chpl);
#line 69 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    coerce_tmp_chpl5 = *(call_tmp_chpl9);
#line 69 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    call_tmp_chpl10 = sizeAs_chpl36(coerce_tmp_chpl5, _ln_chpl, _fn_chpl);
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    if (call_tmp_chpl10 >= ((uint64_t)(numChunks_chpl))) /* ZLINE: 70 /Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl */
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    {
#line 71 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      parDim_chpl = i_chpl;
#line 72 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      goto _breakLabel_chpl;
#line 72 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    }
#line 74 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    if (call_tmp_chpl10 > maxElems_chpl) /* ZLINE: 74 /Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl */
#line 74 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    {
#line 75 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      maxElems_chpl = call_tmp_chpl10;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      maxDim_chpl = i_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    }
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  }
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _breakLabel_chpl:;
#line 80 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  if (parDim_chpl == INT64(-1)) /* ZLINE: 80 /Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl */
#line 80 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  {
#line 81 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    parDim_chpl = maxDim_chpl;
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    if (maxElems_chpl < ((uint64_t)(numChunks_chpl))) /* ZLINE: 85 /Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl */
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    {
#line 86 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      numChunks_chpl = ((int64_t)(maxElems_chpl));
#line 86 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    }
#line 86 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  }
#line 89 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(ret_tmp_chpl2 + INT64(0)) = numChunks_chpl;
#line 89 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(ret_tmp_chpl2 + INT64(1)) = parDim_chpl;
#line 89 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(ret_chpl + INT64(0)) = *(ret_tmp_chpl2 + INT64(0));
#line 89 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(ret_chpl + INT64(1)) = *(ret_tmp_chpl2 + INT64(1));
#line 89 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _end__computeChunkStuff_chpl:;
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(*(_retArg_chpl) + INT64(0)) = *(ret_chpl + INT64(0));
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(*(_retArg_chpl) + INT64(1)) = *(ret_chpl + INT64(1));
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  return;
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
}

#line 48 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
/* DSIUtil.chpl:48 */
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
static void _computeChunkStuff_chpl(int64_t maxTasks_chpl,
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
                                    chpl_bool ignoreRunning_chpl,
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
                                    int64_t minSize_chpl,
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
                                    _tuple_1_star_range_int64_t_both_one_chpl * ranges_chpl,
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
                                    _tuple_2_star_int64_t_chpl * _retArg_chpl,
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
                                    int64_t _ln_chpl,
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
                                    int32_t _fn_chpl) {
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _tuple_2_star_int64_t_chpl ret_chpl;
#line 53 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  uint64_t numElems_chpl;
#line 55 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  uint64_t call_tmp_chpl6;
#line 55 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl3;
#line 55 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_uint64_t_chpl i_lhs_chpl = NULL;
#line 58 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  int64_t call_tmp_chpl7;
#line 58 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  int64_t numChunks_chpl;
#line 60 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _tuple_2_star_int64_t_chpl ret_tmp_chpl;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  int64_t parDim_chpl;
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  int64_t maxDim_chpl;
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  uint64_t maxElems_chpl;
#line 68 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  int64_t _ic__F1_high_chpl;
#line 68 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  int64_t i_chpl;
#line 69 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl8 = NULL;
#line 69 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  uint64_t call_tmp_chpl9;
#line 69 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl4;
#line 89 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _tuple_2_star_int64_t_chpl ret_tmp_chpl2;
#line 53 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  numElems_chpl = UINT64(1);
#line 55 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  coerce_tmp_chpl3 = *(*(ranges_chpl) + INT64(0));
#line 55 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  call_tmp_chpl6 = sizeAs_chpl36(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 55 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  i_lhs_chpl = &numElems_chpl;
#line 55 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(i_lhs_chpl) *= call_tmp_chpl6;
#line 58 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  call_tmp_chpl7 = _computeNumChunks_chpl2(maxTasks_chpl, ignoreRunning_chpl, minSize_chpl, numElems_chpl, _ln_chpl, _fn_chpl);
#line 58 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  numChunks_chpl = call_tmp_chpl7;
#line 59 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  if (call_tmp_chpl7 == INT64(0)) /* ZLINE: 59 /Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl */
#line 59 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  {
#line 60 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    *(ret_tmp_chpl + INT64(0)) = INT64(0);
#line 60 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    *(ret_tmp_chpl + INT64(1)) = INT64(-1);
#line 60 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    *(ret_chpl + INT64(0)) = *(ret_tmp_chpl + INT64(0));
#line 60 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    *(ret_chpl + INT64(1)) = *(ret_tmp_chpl + INT64(1));
#line 60 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    goto _end__computeChunkStuff_chpl;
#line 60 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  }
#line 61 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  assert_chpl((call_tmp_chpl7 > INT64(0)), _ln_chpl, _fn_chpl);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  parDim_chpl = INT64(-1);
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  maxDim_chpl = INT64(-1);
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  maxElems_chpl = UINT64(0);
#line 68 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ic__F1_high_chpl = INT64(0);
#line 68 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  i_chpl = INT64(0);
#line 68 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  chpl_checkIfRangeIterWillOverflow(INT64(0), INT64(0), INT64(1), INT64(0), INT64(0), UINT8(true), _ln_chpl, _fn_chpl);
#line 68 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  for (i_chpl = INT64(0); ((i_chpl <= _ic__F1_high_chpl)); i_chpl += INT64(1)) {
#line 69 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    call_tmp_chpl8 = this_chpl20(ranges_chpl, i_chpl, _ln_chpl, _fn_chpl);
#line 69 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    coerce_tmp_chpl4 = *(call_tmp_chpl8);
#line 69 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    call_tmp_chpl9 = sizeAs_chpl36(coerce_tmp_chpl4, _ln_chpl, _fn_chpl);
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    if (call_tmp_chpl9 >= ((uint64_t)(numChunks_chpl))) /* ZLINE: 70 /Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl */
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    {
#line 71 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      parDim_chpl = i_chpl;
#line 72 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      goto _breakLabel_chpl;
#line 72 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    }
#line 74 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    if (call_tmp_chpl9 > maxElems_chpl) /* ZLINE: 74 /Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl */
#line 74 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    {
#line 75 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      maxElems_chpl = call_tmp_chpl9;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      maxDim_chpl = i_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    }
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  }
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _breakLabel_chpl:;
#line 80 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  if (parDim_chpl == INT64(-1)) /* ZLINE: 80 /Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl */
#line 80 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  {
#line 81 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    parDim_chpl = maxDim_chpl;
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    if (maxElems_chpl < ((uint64_t)(numChunks_chpl))) /* ZLINE: 85 /Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl */
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    {
#line 86 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      numChunks_chpl = ((int64_t)(maxElems_chpl));
#line 86 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    }
#line 86 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  }
#line 89 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(ret_tmp_chpl2 + INT64(0)) = numChunks_chpl;
#line 89 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(ret_tmp_chpl2 + INT64(1)) = parDim_chpl;
#line 89 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(ret_chpl + INT64(0)) = *(ret_tmp_chpl2 + INT64(0));
#line 89 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(ret_chpl + INT64(1)) = *(ret_tmp_chpl2 + INT64(1));
#line 89 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _end__computeChunkStuff_chpl:;
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(*(_retArg_chpl) + INT64(0)) = *(ret_chpl + INT64(0));
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(*(_retArg_chpl) + INT64(1)) = *(ret_chpl + INT64(1));
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  return;
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
}

#line 48 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
/* DSIUtil.chpl:48 */
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
static void _computeChunkStuff_chpl3(int64_t maxTasks_chpl,
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
                                     chpl_bool ignoreRunning_chpl,
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
                                     int64_t minSize_chpl,
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
                                     _tuple_3_star_range_int64_t_both_one_chpl * ranges_chpl,
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
                                     _tuple_2_star_int64_t_chpl * _retArg_chpl,
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
                                     int64_t _ln_chpl,
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
                                     int32_t _fn_chpl) {
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _tuple_2_star_int64_t_chpl ret_chpl;
#line 53 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  uint64_t numElems_chpl;
#line 55 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  uint64_t call_tmp_chpl6;
#line 55 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl3;
#line 55 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_uint64_t_chpl i_lhs_chpl = NULL;
#line 55 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  uint64_t call_tmp_chpl7;
#line 55 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl4;
#line 55 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_uint64_t_chpl i_lhs_chpl2 = NULL;
#line 55 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  uint64_t call_tmp_chpl8;
#line 55 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl5;
#line 55 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_uint64_t_chpl i_lhs_chpl3 = NULL;
#line 58 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  int64_t call_tmp_chpl9;
#line 58 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  int64_t numChunks_chpl;
#line 60 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _tuple_2_star_int64_t_chpl ret_tmp_chpl;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  int64_t parDim_chpl;
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  int64_t maxDim_chpl;
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  uint64_t maxElems_chpl;
#line 68 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  int64_t _ic__F1_high_chpl;
#line 68 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  int64_t i_chpl;
#line 69 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl10 = NULL;
#line 69 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  uint64_t call_tmp_chpl11;
#line 69 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl6;
#line 89 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _tuple_2_star_int64_t_chpl ret_tmp_chpl2;
#line 53 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  numElems_chpl = UINT64(1);
#line 55 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  coerce_tmp_chpl3 = *(*(ranges_chpl) + INT64(0));
#line 55 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  call_tmp_chpl6 = sizeAs_chpl36(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 55 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  i_lhs_chpl = &numElems_chpl;
#line 55 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(i_lhs_chpl) *= call_tmp_chpl6;
#line 55 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  coerce_tmp_chpl4 = *(*(ranges_chpl) + INT64(1));
#line 55 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  call_tmp_chpl7 = sizeAs_chpl36(coerce_tmp_chpl4, _ln_chpl, _fn_chpl);
#line 55 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  i_lhs_chpl2 = &numElems_chpl;
#line 55 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(i_lhs_chpl2) *= call_tmp_chpl7;
#line 55 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  coerce_tmp_chpl5 = *(*(ranges_chpl) + INT64(2));
#line 55 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  call_tmp_chpl8 = sizeAs_chpl36(coerce_tmp_chpl5, _ln_chpl, _fn_chpl);
#line 55 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  i_lhs_chpl3 = &numElems_chpl;
#line 55 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(i_lhs_chpl3) *= call_tmp_chpl8;
#line 58 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  call_tmp_chpl9 = _computeNumChunks_chpl2(maxTasks_chpl, ignoreRunning_chpl, minSize_chpl, numElems_chpl, _ln_chpl, _fn_chpl);
#line 58 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  numChunks_chpl = call_tmp_chpl9;
#line 59 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  if (call_tmp_chpl9 == INT64(0)) /* ZLINE: 59 /Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl */
#line 59 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  {
#line 60 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    *(ret_tmp_chpl + INT64(0)) = INT64(0);
#line 60 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    *(ret_tmp_chpl + INT64(1)) = INT64(-1);
#line 60 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    *(ret_chpl + INT64(0)) = *(ret_tmp_chpl + INT64(0));
#line 60 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    *(ret_chpl + INT64(1)) = *(ret_tmp_chpl + INT64(1));
#line 60 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    goto _end__computeChunkStuff_chpl;
#line 60 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  }
#line 61 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  assert_chpl((call_tmp_chpl9 > INT64(0)), _ln_chpl, _fn_chpl);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  parDim_chpl = INT64(-1);
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  maxDim_chpl = INT64(-1);
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  maxElems_chpl = UINT64(0);
#line 68 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ic__F1_high_chpl = INT64(2);
#line 68 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  i_chpl = INT64(0);
#line 68 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  chpl_checkIfRangeIterWillOverflow(INT64(0), INT64(2), INT64(1), INT64(0), INT64(2), UINT8(true), _ln_chpl, _fn_chpl);
#line 68 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  for (i_chpl = INT64(0); ((i_chpl <= _ic__F1_high_chpl)); i_chpl += INT64(1)) {
#line 69 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    call_tmp_chpl10 = this_chpl24(ranges_chpl, i_chpl, _ln_chpl, _fn_chpl);
#line 69 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    coerce_tmp_chpl6 = *(call_tmp_chpl10);
#line 69 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    call_tmp_chpl11 = sizeAs_chpl36(coerce_tmp_chpl6, _ln_chpl, _fn_chpl);
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    if (call_tmp_chpl11 >= ((uint64_t)(numChunks_chpl))) /* ZLINE: 70 /Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl */
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    {
#line 71 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      parDim_chpl = i_chpl;
#line 72 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      goto _breakLabel_chpl;
#line 72 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    }
#line 74 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    if (call_tmp_chpl11 > maxElems_chpl) /* ZLINE: 74 /Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl */
#line 74 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    {
#line 75 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      maxElems_chpl = call_tmp_chpl11;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      maxDim_chpl = i_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    }
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  }
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _breakLabel_chpl:;
#line 80 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  if (parDim_chpl == INT64(-1)) /* ZLINE: 80 /Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl */
#line 80 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  {
#line 81 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    parDim_chpl = maxDim_chpl;
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    if (maxElems_chpl < ((uint64_t)(numChunks_chpl))) /* ZLINE: 85 /Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl */
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    {
#line 86 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      numChunks_chpl = ((int64_t)(maxElems_chpl));
#line 86 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    }
#line 86 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  }
#line 89 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(ret_tmp_chpl2 + INT64(0)) = numChunks_chpl;
#line 89 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(ret_tmp_chpl2 + INT64(1)) = parDim_chpl;
#line 89 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(ret_chpl + INT64(0)) = *(ret_tmp_chpl2 + INT64(0));
#line 89 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(ret_chpl + INT64(1)) = *(ret_tmp_chpl2 + INT64(1));
#line 89 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _end__computeChunkStuff_chpl:;
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(*(_retArg_chpl) + INT64(0)) = *(ret_chpl + INT64(0));
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(*(_retArg_chpl) + INT64(1)) = *(ret_chpl + INT64(1));
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  return;
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
}

#line 93 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
/* DSIUtil.chpl:93 */
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
static int64_t _computeNumChunks_chpl(int64_t maxTasks_chpl,
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
                                      chpl_bool ignoreRunning_chpl,
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
                                      int64_t minSize_chpl,
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
                                      int64_t numElems_chpl,
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
                                      int64_t _ln_chpl,
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
                                      int32_t _fn_chpl) {
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  int64_t ret_chpl;
#line 98 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  uint64_t call_tmp_chpl6;
#line 98 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  uint64_t unumElems_chpl;
#line 99 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  int64_t numChunks_chpl;
#line 101 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  locale_chpl call_tmp_chpl7;
#line 101 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  chpl_localeID_t call_tmp_chpl8;
#line 101 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  int32_t call_tmp_chpl9;
#line 101 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  chpl_localeID_t call_tmp_chpl10;
#line 101 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  locale_chpl call_tmp_chpl11;
#line 101 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  locale_chpl ret_tmp_chpl;
#line 101 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  int64_t call_tmp_chpl12;
#line 101 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  int64_t call_tmp_chpl13;
#line 101 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_locale_chpl i_x_chpl = NULL;
#line 102 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  int64_t tmp_chpl;
#line 109 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  chpl_bool tmp_chpl2;
#line 109 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  chpl_bool tmp_chpl3;
#line 110 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_int64_t_chpl i_lhs_chpl = NULL;
#line 109 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  chpl_bool tmp_chpl4;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  if (numElems_chpl <= INT64(0)) /* ZLINE: 94 /Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl */
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  {
#line 95 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    ret_chpl = INT64(0);
#line 95 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    goto _end__computeNumChunks_chpl;
#line 95 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  }
#line 98 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  call_tmp_chpl6 = ((uint64_t)(numElems_chpl));
#line 98 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  unumElems_chpl = call_tmp_chpl6;
#line 99 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  numChunks_chpl = maxTasks_chpl;
#line 100 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  if (! ignoreRunning_chpl) /* ZLINE: 100 /Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl */
#line 100 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  {
#line 101 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    call_tmp_chpl9 = get_chpl_nodeID();
#line 101 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    call_tmp_chpl10 = chpl_rt_buildLocaleID(call_tmp_chpl9, c_sublocid_any);
#line 101 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    call_tmp_chpl8 = call_tmp_chpl10;
#line 101 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    chpl_localeID_to_locale(&call_tmp_chpl8, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 101 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    call_tmp_chpl11 = ret_tmp_chpl;
#line 101 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    call_tmp_chpl7 = call_tmp_chpl11;
#line 101 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    call_tmp_chpl12 = runningTasks_chpl(&call_tmp_chpl7, _ln_chpl, _fn_chpl);
#line 101 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    call_tmp_chpl13 = (call_tmp_chpl12 - INT64(1));
#line 101 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    i_x_chpl = &call_tmp_chpl7;
#line 101 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    deinit_chpl62(i_x_chpl);
#line 102 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    if (call_tmp_chpl13 < maxTasks_chpl) /* ZLINE: 102 /Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl */
#line 102 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    {
#line 102 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      tmp_chpl = ((int64_t)((maxTasks_chpl - call_tmp_chpl13)));
#line 102 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    } else /* ZLINE: 102 /Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl */
#line 102 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    {
#line 102 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      tmp_chpl = INT64(1);
#line 102 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    }
#line 102 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    numChunks_chpl = tmp_chpl;
#line 102 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  }
#line 107 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  if (minSize_chpl > INT64(0)) /* ZLINE: 109 /Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl */
#line 107 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  {
#line 109 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    if (call_tmp_chpl6 < ((uint64_t)(((int64_t)((minSize_chpl * numChunks_chpl)))))) /* ZLINE: 109 /Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl */
#line 109 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    {
#line 109 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      tmp_chpl3 = (numChunks_chpl > INT64(1));
#line 109 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    } else /* ZLINE: 109 /Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl */
#line 109 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    {
#line 109 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      tmp_chpl3 = UINT8(false);
#line 109 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    }
#line 109 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    tmp_chpl2 = tmp_chpl3;
#line 109 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    while (tmp_chpl2) {
#line 110 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      i_lhs_chpl = &numChunks_chpl;
#line 110 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      *(i_lhs_chpl) -= INT64(1);
#line 109 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      if (unumElems_chpl < ((uint64_t)(((int64_t)((minSize_chpl * numChunks_chpl)))))) /* ZLINE: 109 /Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl */
#line 109 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      {
#line 109 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
        tmp_chpl4 = (numChunks_chpl > INT64(1));
#line 109 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      } else /* ZLINE: 109 /Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl */
#line 109 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      {
#line 109 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
        tmp_chpl4 = UINT8(false);
#line 109 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      }
#line 109 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      tmp_chpl2 = tmp_chpl4;
#line 109 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    }
#line 109 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  }
#line 113 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  if (((uint64_t)(numChunks_chpl)) > unumElems_chpl) /* ZLINE: 113 /Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl */
#line 113 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  {
#line 113 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    numChunks_chpl = ((int64_t)(unumElems_chpl));
#line 113 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  }
#line 115 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  ret_chpl = numChunks_chpl;
#line 115 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _end__computeNumChunks_chpl:;
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  return ret_chpl;
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
}

#line 93 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
/* DSIUtil.chpl:93 */
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
static int64_t _computeNumChunks_chpl2(int64_t maxTasks_chpl,
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
                                       chpl_bool ignoreRunning_chpl,
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
                                       int64_t minSize_chpl,
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
                                       uint64_t numElems_chpl,
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
                                       int64_t _ln_chpl,
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
                                       int32_t _fn_chpl) {
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  int64_t ret_chpl;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  chpl_bool tmp_chpl;
#line 98 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  uint64_t unumElems_chpl;
#line 99 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  int64_t numChunks_chpl;
#line 101 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  locale_chpl call_tmp_chpl6;
#line 101 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  chpl_localeID_t call_tmp_chpl7;
#line 101 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  int32_t call_tmp_chpl8;
#line 101 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  chpl_localeID_t call_tmp_chpl9;
#line 101 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  locale_chpl call_tmp_chpl10;
#line 101 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  locale_chpl ret_tmp_chpl;
#line 101 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  int64_t call_tmp_chpl11;
#line 101 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  int64_t call_tmp_chpl12;
#line 101 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_locale_chpl i_x_chpl = NULL;
#line 102 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  int64_t tmp_chpl2;
#line 109 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  chpl_bool tmp_chpl3;
#line 109 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  chpl_bool tmp_chpl4;
#line 110 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_int64_t_chpl i_lhs_chpl = NULL;
#line 109 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  chpl_bool tmp_chpl5;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  if (! (INT64(0) < INT64(0))) /* ZLINE: 94 /Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl */
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  {
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    tmp_chpl = (numElems_chpl <= ((uint64_t)(INT64(0))));
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  } else /* ZLINE: 94 /Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl */
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  {
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    tmp_chpl = UINT8(false);
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  }
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  if (tmp_chpl) /* ZLINE: 94 /Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl */
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  {
#line 95 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    ret_chpl = INT64(0);
#line 95 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    goto _end__computeNumChunks_chpl;
#line 95 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  }
#line 98 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  unumElems_chpl = numElems_chpl;
#line 99 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  numChunks_chpl = maxTasks_chpl;
#line 100 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  if (! ignoreRunning_chpl) /* ZLINE: 100 /Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl */
#line 100 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  {
#line 101 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    call_tmp_chpl8 = get_chpl_nodeID();
#line 101 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    call_tmp_chpl9 = chpl_rt_buildLocaleID(call_tmp_chpl8, c_sublocid_any);
#line 101 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    call_tmp_chpl7 = call_tmp_chpl9;
#line 101 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    chpl_localeID_to_locale(&call_tmp_chpl7, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 101 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    call_tmp_chpl10 = ret_tmp_chpl;
#line 101 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    call_tmp_chpl6 = call_tmp_chpl10;
#line 101 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    call_tmp_chpl11 = runningTasks_chpl(&call_tmp_chpl6, _ln_chpl, _fn_chpl);
#line 101 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    call_tmp_chpl12 = (call_tmp_chpl11 - INT64(1));
#line 101 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    i_x_chpl = &call_tmp_chpl6;
#line 101 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    deinit_chpl62(i_x_chpl);
#line 102 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    if (call_tmp_chpl12 < maxTasks_chpl) /* ZLINE: 102 /Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl */
#line 102 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    {
#line 102 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      tmp_chpl2 = ((int64_t)((maxTasks_chpl - call_tmp_chpl12)));
#line 102 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    } else /* ZLINE: 102 /Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl */
#line 102 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    {
#line 102 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      tmp_chpl2 = INT64(1);
#line 102 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    }
#line 102 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    numChunks_chpl = tmp_chpl2;
#line 102 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  }
#line 107 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  if (minSize_chpl > INT64(0)) /* ZLINE: 109 /Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl */
#line 107 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  {
#line 109 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    if (numElems_chpl < ((uint64_t)(((int64_t)((minSize_chpl * numChunks_chpl)))))) /* ZLINE: 109 /Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl */
#line 109 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    {
#line 109 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      tmp_chpl4 = (numChunks_chpl > INT64(1));
#line 109 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    } else /* ZLINE: 109 /Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl */
#line 109 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    {
#line 109 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      tmp_chpl4 = UINT8(false);
#line 109 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    }
#line 109 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    tmp_chpl3 = tmp_chpl4;
#line 109 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    while (tmp_chpl3) {
#line 110 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      i_lhs_chpl = &numChunks_chpl;
#line 110 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      *(i_lhs_chpl) -= INT64(1);
#line 109 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      if (unumElems_chpl < ((uint64_t)(((int64_t)((minSize_chpl * numChunks_chpl)))))) /* ZLINE: 109 /Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl */
#line 109 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      {
#line 109 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
        tmp_chpl5 = (numChunks_chpl > INT64(1));
#line 109 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      } else /* ZLINE: 109 /Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl */
#line 109 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      {
#line 109 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
        tmp_chpl5 = UINT8(false);
#line 109 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      }
#line 109 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      tmp_chpl3 = tmp_chpl5;
#line 109 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    }
#line 109 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  }
#line 113 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  if (((uint64_t)(numChunks_chpl)) > unumElems_chpl) /* ZLINE: 113 /Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl */
#line 113 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  {
#line 113 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    numChunks_chpl = ((int64_t)(unumElems_chpl));
#line 113 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  }
#line 115 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  ret_chpl = numChunks_chpl;
#line 115 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _end__computeNumChunks_chpl:;
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  return ret_chpl;
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
}

#line 119 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
/* DSIUtil.chpl:119 */
#line 119 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
static int64_t _computeNumChunks_chpl3(int64_t numElems_chpl,
#line 119 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
                                       int64_t _ln_chpl,
#line 119 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
                                       int32_t _fn_chpl) {
#line 125 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  int64_t local_dataParMinGranularity_chpl;
#line 124 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  chpl_bool local_dataParIgnoreRunningTasks_chpl;
#line 121 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  int64_t local_dataParTasksPerLocale_chpl;
#line 121 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  int64_t tmp_chpl;
#line 122 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  locale_chpl call_tmp_chpl6;
#line 122 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  chpl_localeID_t call_tmp_chpl7;
#line 122 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  int32_t call_tmp_chpl8;
#line 122 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  chpl_localeID_t call_tmp_chpl9;
#line 122 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  locale_chpl call_tmp_chpl10;
#line 122 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  locale_chpl ret_tmp_chpl;
#line 122 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  int64_t ret_chpl;
#line 122 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  BaseLocale_chpl coerce_tmp_chpl3 = NULL;
#line 122 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  BaseLocale_chpl coerce_tmp_chpl4 = NULL;
#line 121 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_locale_chpl i_x_chpl = NULL;
#line 126 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  int64_t call_tmp_chpl11;
#line 125 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  local_dataParMinGranularity_chpl = dataParMinGranularity_chpl;
#line 124 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  local_dataParIgnoreRunningTasks_chpl = dataParIgnoreRunningTasks_chpl;
#line 121 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  local_dataParTasksPerLocale_chpl = dataParTasksPerLocale_chpl;
#line 121 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  if (local_dataParTasksPerLocale_chpl == INT64(0)) /* ZLINE: 121 /Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl */
#line 121 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  {
#line 122 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    call_tmp_chpl8 = get_chpl_nodeID();
#line 122 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    call_tmp_chpl9 = chpl_rt_buildLocaleID(call_tmp_chpl8, c_sublocid_any);
#line 122 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    call_tmp_chpl7 = call_tmp_chpl9;
#line 122 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    chpl_localeID_to_locale(&call_tmp_chpl7, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 122 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    call_tmp_chpl10 = ret_tmp_chpl;
#line 122 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    call_tmp_chpl6 = call_tmp_chpl10;
#line 122 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    coerce_tmp_chpl3 = (&call_tmp_chpl6)->_instance;
#line 122 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    if (((RootClass_chpl)(coerce_tmp_chpl3)) == nil) /* ZLINE: 122 /Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl */
#line 122 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    {
#line 122 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      nilCheckHalt_chpl(&_str_literal_52_chpl, _ln_chpl, _fn_chpl);
#line 122 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    }
#line 122 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    coerce_tmp_chpl4 = coerce_tmp_chpl3;
#line 122 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    chpl_check_nil(coerce_tmp_chpl4, _ln_chpl, _fn_chpl);
#line 122 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    ret_chpl = (coerce_tmp_chpl4)->maxTaskPar;
#line 121 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    tmp_chpl = ret_chpl;
#line 121 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    i_x_chpl = &call_tmp_chpl6;
#line 121 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    deinit_chpl62(i_x_chpl);
#line 121 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  } else /* ZLINE: 121 /Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl */
#line 121 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  {
#line 121 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    tmp_chpl = local_dataParTasksPerLocale_chpl;
#line 121 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  }
#line 126 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  call_tmp_chpl11 = _computeNumChunks_chpl(tmp_chpl, local_dataParIgnoreRunningTasks_chpl, local_dataParMinGranularity_chpl, numElems_chpl, _ln_chpl, _fn_chpl);
#line 119 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  return call_tmp_chpl11;
#line 119 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
}

#line 119 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
/* DSIUtil.chpl:119 */
#line 119 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
static int64_t _computeNumChunks_chpl4(uint64_t numElems_chpl,
#line 119 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
                                       int64_t _ln_chpl,
#line 119 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
                                       int32_t _fn_chpl) {
#line 125 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  int64_t local_dataParMinGranularity_chpl;
#line 124 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  chpl_bool local_dataParIgnoreRunningTasks_chpl;
#line 121 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  int64_t local_dataParTasksPerLocale_chpl;
#line 121 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  int64_t tmp_chpl;
#line 122 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  locale_chpl call_tmp_chpl6;
#line 122 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  chpl_localeID_t call_tmp_chpl7;
#line 122 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  int32_t call_tmp_chpl8;
#line 122 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  chpl_localeID_t call_tmp_chpl9;
#line 122 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  locale_chpl call_tmp_chpl10;
#line 122 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  locale_chpl ret_tmp_chpl;
#line 122 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  int64_t ret_chpl;
#line 122 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  BaseLocale_chpl coerce_tmp_chpl3 = NULL;
#line 122 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  BaseLocale_chpl coerce_tmp_chpl4 = NULL;
#line 121 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_locale_chpl i_x_chpl = NULL;
#line 126 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  int64_t call_tmp_chpl11;
#line 125 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  local_dataParMinGranularity_chpl = dataParMinGranularity_chpl;
#line 124 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  local_dataParIgnoreRunningTasks_chpl = dataParIgnoreRunningTasks_chpl;
#line 121 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  local_dataParTasksPerLocale_chpl = dataParTasksPerLocale_chpl;
#line 121 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  if (local_dataParTasksPerLocale_chpl == INT64(0)) /* ZLINE: 121 /Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl */
#line 121 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  {
#line 122 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    call_tmp_chpl8 = get_chpl_nodeID();
#line 122 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    call_tmp_chpl9 = chpl_rt_buildLocaleID(call_tmp_chpl8, c_sublocid_any);
#line 122 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    call_tmp_chpl7 = call_tmp_chpl9;
#line 122 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    chpl_localeID_to_locale(&call_tmp_chpl7, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 122 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    call_tmp_chpl10 = ret_tmp_chpl;
#line 122 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    call_tmp_chpl6 = call_tmp_chpl10;
#line 122 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    coerce_tmp_chpl3 = (&call_tmp_chpl6)->_instance;
#line 122 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    if (((RootClass_chpl)(coerce_tmp_chpl3)) == nil) /* ZLINE: 122 /Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl */
#line 122 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    {
#line 122 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      nilCheckHalt_chpl(&_str_literal_52_chpl, _ln_chpl, _fn_chpl);
#line 122 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    }
#line 122 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    coerce_tmp_chpl4 = coerce_tmp_chpl3;
#line 122 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    chpl_check_nil(coerce_tmp_chpl4, _ln_chpl, _fn_chpl);
#line 122 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    ret_chpl = (coerce_tmp_chpl4)->maxTaskPar;
#line 121 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    tmp_chpl = ret_chpl;
#line 121 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    i_x_chpl = &call_tmp_chpl6;
#line 121 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    deinit_chpl62(i_x_chpl);
#line 121 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  } else /* ZLINE: 121 /Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl */
#line 121 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  {
#line 121 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    tmp_chpl = local_dataParTasksPerLocale_chpl;
#line 121 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  }
#line 126 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  call_tmp_chpl11 = _computeNumChunks_chpl2(tmp_chpl, local_dataParIgnoreRunningTasks_chpl, local_dataParMinGranularity_chpl, numElems_chpl, _ln_chpl, _fn_chpl);
#line 119 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  return call_tmp_chpl11;
#line 119 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
}

#line 164 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
/* DSIUtil.chpl:164 */
#line 164 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
static uint64_t intCeilXDivByY_chpl(uint64_t x_chpl,
#line 164 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
                                    uint64_t y_chpl,
#line 164 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
                                    int64_t _ln_chpl,
#line 164 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
                                    int32_t _fn_chpl) {
#line 164 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  uint64_t call_tmp_chpl6;
#line 164 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  chpl_bool tmp_chpl;
#line 164 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  uint64_t call_tmp_chpl7;
#line 164 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  uint64_t call_tmp_chpl8;
#line 164 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  call_tmp_chpl6 = (x_chpl - UINT64(1));
#line 164 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  if (! (INT64(0) < INT64(0))) /* ZLINE: 164 /Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl */
#line 164 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  {
#line 164 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    tmp_chpl = (y_chpl == ((uint64_t)(INT64(0))));
#line 164 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  } else /* ZLINE: 164 /Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl */
#line 164 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  {
#line 164 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    tmp_chpl = UINT8(false);
#line 164 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  }
#line 164 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  if (tmp_chpl) /* ZLINE: 164 /Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl */
#line 164 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  {
#line 164 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    halt_chpl(&_str_literal_30_chpl, _ln_chpl, _fn_chpl);
#line 164 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  }
#line 164 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  call_tmp_chpl7 = (call_tmp_chpl6 / y_chpl);
#line 164 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  call_tmp_chpl8 = (UINT64(1) + call_tmp_chpl7);
#line 164 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  return call_tmp_chpl8;
#line 164 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
}

#line 166 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
/* DSIUtil.chpl:166 */
#line 166 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
static void _computeBlock_chpl(int64_t numelems_chpl,
#line 166 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
                               int64_t numblocks_chpl,
#line 166 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
                               int64_t blocknum_chpl,
#line 166 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
                               int64_t wayhi_chpl,
#line 166 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
                               int64_t waylo_chpl,
#line 166 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
                               int64_t lo_chpl,
#line 166 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
                               _tuple_2_star_int64_t_chpl * _retArg_chpl,
#line 166 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
                               int64_t _ln_chpl,
#line 166 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
                               int32_t _fn_chpl) {
#line 166 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _tuple_2_star_int64_t_chpl ret_chpl;
#line 169 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _tuple_2_star_int64_t_chpl ret_tmp_chpl;
#line 172 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  int64_t tmp_chpl;
#line 173 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  uint64_t call_tmp_chpl6;
#line 175 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  int64_t tmp_chpl2;
#line 176 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  uint64_t call_tmp_chpl7;
#line 178 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _tuple_2_star_int64_t_chpl ret_tmp_chpl2;
#line 168 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  if (numelems_chpl == INT64(0)) /* ZLINE: 168 /Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl */
#line 168 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  {
#line 169 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    *(ret_tmp_chpl + INT64(0)) = INT64(1);
#line 169 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    *(ret_tmp_chpl + INT64(1)) = INT64(0);
#line 169 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    *(ret_chpl + INT64(0)) = *(ret_tmp_chpl + INT64(0));
#line 169 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    *(ret_chpl + INT64(1)) = *(ret_tmp_chpl + INT64(1));
#line 169 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    goto _end__computeBlock_chpl;
#line 169 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  }
#line 172 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  if (blocknum_chpl == INT64(0)) /* ZLINE: 172 /Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl */
#line 172 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  {
#line 172 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    tmp_chpl = waylo_chpl;
#line 172 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  } else /* ZLINE: 172 /Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl */
#line 172 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  {
#line 173 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    call_tmp_chpl6 = intCeilXDivByY_chpl(((uint64_t)((((uint64_t)(numelems_chpl)) * ((uint64_t)(blocknum_chpl))))), ((uint64_t)(numblocks_chpl)), _ln_chpl, _fn_chpl);
#line 172 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    tmp_chpl = ((int64_t)((lo_chpl + ((int64_t)(call_tmp_chpl6)))));
#line 172 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  }
#line 175 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  if (blocknum_chpl == ((int64_t)((numblocks_chpl - INT64(1))))) /* ZLINE: 175 /Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl */
#line 175 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  {
#line 175 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    tmp_chpl2 = wayhi_chpl;
#line 175 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  } else /* ZLINE: 175 /Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl */
#line 175 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  {
#line 176 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    call_tmp_chpl7 = intCeilXDivByY_chpl(((uint64_t)((((uint64_t)(numelems_chpl)) * ((uint64_t)(((int64_t)((blocknum_chpl + INT64(1))))))))), ((uint64_t)(numblocks_chpl)), _ln_chpl, _fn_chpl);
#line 175 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    tmp_chpl2 = ((int64_t)((((int64_t)((lo_chpl + ((int64_t)(call_tmp_chpl7))))) - INT64(1))));
#line 175 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  }
#line 178 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(ret_tmp_chpl2 + INT64(0)) = tmp_chpl;
#line 178 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(ret_tmp_chpl2 + INT64(1)) = tmp_chpl2;
#line 178 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(ret_chpl + INT64(0)) = *(ret_tmp_chpl2 + INT64(0));
#line 178 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(ret_chpl + INT64(1)) = *(ret_tmp_chpl2 + INT64(1));
#line 178 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _end__computeBlock_chpl:;
#line 166 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(*(_retArg_chpl) + INT64(0)) = *(ret_chpl + INT64(0));
#line 166 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(*(_retArg_chpl) + INT64(1)) = *(ret_chpl + INT64(1));
#line 166 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  return;
#line 166 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
}

#line 166 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
/* DSIUtil.chpl:166 */
#line 166 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
static void _computeBlock_chpl2(uint64_t numelems_chpl,
#line 166 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
                                int64_t numblocks_chpl,
#line 166 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
                                int64_t blocknum_chpl,
#line 166 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
                                int64_t wayhi_chpl,
#line 166 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
                                int64_t waylo_chpl,
#line 166 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
                                int64_t lo_chpl,
#line 166 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
                                _tuple_2_star_int64_t_chpl * _retArg_chpl,
#line 166 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
                                int64_t _ln_chpl,
#line 166 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
                                int32_t _fn_chpl) {
#line 166 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _tuple_2_star_int64_t_chpl ret_chpl;
#line 168 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  chpl_bool tmp_chpl;
#line 169 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _tuple_2_star_int64_t_chpl ret_tmp_chpl;
#line 172 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  int64_t tmp_chpl2;
#line 173 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  uint64_t call_tmp_chpl6;
#line 175 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  int64_t tmp_chpl3;
#line 176 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  uint64_t call_tmp_chpl7;
#line 178 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _tuple_2_star_int64_t_chpl ret_tmp_chpl2;
#line 168 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  if (! (INT64(0) < INT64(0))) /* ZLINE: 168 /Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl */
#line 168 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  {
#line 168 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    tmp_chpl = (numelems_chpl == ((uint64_t)(INT64(0))));
#line 168 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  } else /* ZLINE: 168 /Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl */
#line 168 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  {
#line 168 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    tmp_chpl = UINT8(false);
#line 168 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  }
#line 168 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  if (tmp_chpl) /* ZLINE: 168 /Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl */
#line 168 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  {
#line 169 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    *(ret_tmp_chpl + INT64(0)) = INT64(1);
#line 169 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    *(ret_tmp_chpl + INT64(1)) = INT64(0);
#line 169 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    *(ret_chpl + INT64(0)) = *(ret_tmp_chpl + INT64(0));
#line 169 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    *(ret_chpl + INT64(1)) = *(ret_tmp_chpl + INT64(1));
#line 169 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    goto _end__computeBlock_chpl;
#line 169 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  }
#line 172 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  if (blocknum_chpl == INT64(0)) /* ZLINE: 172 /Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl */
#line 172 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  {
#line 172 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    tmp_chpl2 = waylo_chpl;
#line 172 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  } else /* ZLINE: 172 /Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl */
#line 172 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  {
#line 173 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    call_tmp_chpl6 = intCeilXDivByY_chpl(((uint64_t)((numelems_chpl * ((uint64_t)(blocknum_chpl))))), ((uint64_t)(numblocks_chpl)), _ln_chpl, _fn_chpl);
#line 172 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    tmp_chpl2 = ((int64_t)((lo_chpl + ((int64_t)(call_tmp_chpl6)))));
#line 172 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  }
#line 175 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  if (blocknum_chpl == ((int64_t)((numblocks_chpl - INT64(1))))) /* ZLINE: 175 /Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl */
#line 175 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  {
#line 175 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    tmp_chpl3 = wayhi_chpl;
#line 175 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  } else /* ZLINE: 175 /Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl */
#line 175 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  {
#line 176 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    call_tmp_chpl7 = intCeilXDivByY_chpl(((uint64_t)((numelems_chpl * ((uint64_t)(((int64_t)((blocknum_chpl + INT64(1))))))))), ((uint64_t)(numblocks_chpl)), _ln_chpl, _fn_chpl);
#line 175 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    tmp_chpl3 = ((int64_t)((((int64_t)((lo_chpl + ((int64_t)(call_tmp_chpl7))))) - INT64(1))));
#line 175 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  }
#line 178 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(ret_tmp_chpl2 + INT64(0)) = tmp_chpl2;
#line 178 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(ret_tmp_chpl2 + INT64(1)) = tmp_chpl3;
#line 178 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(ret_chpl + INT64(0)) = *(ret_tmp_chpl2 + INT64(0));
#line 178 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(ret_chpl + INT64(1)) = *(ret_tmp_chpl2 + INT64(1));
#line 178 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _end__computeBlock_chpl:;
#line 166 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(*(_retArg_chpl) + INT64(0)) = *(ret_chpl + INT64(0));
#line 166 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(*(_retArg_chpl) + INT64(1)) = *(ret_chpl + INT64(1));
#line 166 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  return;
#line 166 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
}

#line 570 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
/* DSIUtil.chpl:570 */
#line 570 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
static void bulkCommComputeActiveDims_chpl2(_tuple_4_star_range_int64_t_both_one_chpl * LeftDims_chpl,
#line 570 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
                                            _tuple_4_star_range_int64_t_both_one_chpl * RightDims_chpl,
#line 570 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
                                            _tuple_3__tuple_4_star_int64_t__tuple_4_star_int64_t_int64_t_chpl * _retArg_chpl,
#line 570 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
                                            int64_t _ln_chpl,
#line 570 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
                                            int32_t _fn_chpl) {
#line 575 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  int64_t inferredRank_chpl;
#line 579 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _tuple_4_star_int64_t_chpl LeftActives_chpl;
#line 579 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _tuple_4_star_int64_t_chpl ret_tmp_chpl;
#line 579 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _tuple_4_star_int64_t_chpl RightActives_chpl;
#line 579 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _tuple_4_star_int64_t_chpl ret_tmp_chpl2;
#line 581 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  int64_t li_chpl;
#line 581 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  int64_t ri_chpl;
#line 593 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_int64_t_chpl call_tmp_chpl6 = NULL;
#line 594 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_int64_t_chpl call_tmp_chpl7 = NULL;
#line 596 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_int64_t_chpl i_lhs_chpl = NULL;
#line 597 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_int64_t_chpl i_lhs_chpl2 = NULL;
#line 598 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_int64_t_chpl i_lhs_chpl3 = NULL;
#line 599 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  chpl_bool tmp_chpl;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _tuple_3__ref__tuple_4_star_int64_t__ref__tuple_4_star_int64_t_int64_t_chpl call_tmp_chpl8;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _tuple_4_star_int64_t_chpl coerce_tmp_chpl3;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _tuple_4_star_int64_t_chpl retcopy_chpl;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _tuple_4_star_int64_t_chpl coerce_tmp_chpl4;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _tuple_4_star_int64_t_chpl retcopy_chpl2;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _tuple_3__ref__tuple_4_star_int64_t__ref__tuple_4_star_int64_t_int64_t_chpl ret_tmp_chpl3;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref__tuple_4_star_int64_t_chpl this_x0_chpl = NULL;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref__tuple_4_star_int64_t_chpl this_x1_chpl = NULL;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  int64_t this_x2_chpl;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref__tuple_4_star_int64_t_chpl copy_ret_tmp_x0_chpl = NULL;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref__tuple_4_star_int64_t_chpl copy_ret_tmp_x1_chpl = NULL;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  int64_t copy_ret_tmp_x2_chpl;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _tuple_3__tuple_4_star_int64_t__tuple_4_star_int64_t_int64_t_chpl ret_chpl;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _tuple_4_star_int64_t_chpl elt_x0_chpl;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _tuple_4_star_int64_t_chpl elt_x1_chpl;
#line 575 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  inferredRank_chpl = INT64(0);
#line 579 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(ret_tmp_chpl + INT64(0)) = INT64(0);
#line 579 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(ret_tmp_chpl + INT64(1)) = INT64(0);
#line 579 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(ret_tmp_chpl + INT64(2)) = INT64(0);
#line 579 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(ret_tmp_chpl + INT64(3)) = INT64(0);
#line 579 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(LeftActives_chpl + INT64(0)) = *(ret_tmp_chpl + INT64(0));
#line 579 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(LeftActives_chpl + INT64(1)) = *(ret_tmp_chpl + INT64(1));
#line 579 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(LeftActives_chpl + INT64(2)) = *(ret_tmp_chpl + INT64(2));
#line 579 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(LeftActives_chpl + INT64(3)) = *(ret_tmp_chpl + INT64(3));
#line 579 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(ret_tmp_chpl2 + INT64(0)) = INT64(0);
#line 579 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(ret_tmp_chpl2 + INT64(1)) = INT64(0);
#line 579 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(ret_tmp_chpl2 + INT64(2)) = INT64(0);
#line 579 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(ret_tmp_chpl2 + INT64(3)) = INT64(0);
#line 579 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(RightActives_chpl + INT64(0)) = *(ret_tmp_chpl2 + INT64(0));
#line 579 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(RightActives_chpl + INT64(1)) = *(ret_tmp_chpl2 + INT64(1));
#line 579 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(RightActives_chpl + INT64(2)) = *(ret_tmp_chpl2 + INT64(2));
#line 579 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(RightActives_chpl + INT64(3)) = *(ret_tmp_chpl2 + INT64(3));
#line 581 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  li_chpl = INT64(0);
#line 581 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  ri_chpl = INT64(0);
#line 590 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  do {
#line 591 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    advance_chpl23(LeftDims_chpl, RightDims_chpl, &li_chpl, &ri_chpl, _ln_chpl, _fn_chpl);
#line 593 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    call_tmp_chpl6 = this_chpl28(&LeftActives_chpl, inferredRank_chpl, _ln_chpl, _fn_chpl);
#line 593 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    *(call_tmp_chpl6) = li_chpl;
#line 594 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    call_tmp_chpl7 = this_chpl28(&RightActives_chpl, inferredRank_chpl, _ln_chpl, _fn_chpl);
#line 594 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    *(call_tmp_chpl7) = ri_chpl;
#line 596 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    i_lhs_chpl = &inferredRank_chpl;
#line 596 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    *(i_lhs_chpl) += INT64(1);
#line 597 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    i_lhs_chpl2 = &li_chpl;
#line 597 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    *(i_lhs_chpl2) += INT64(1);
#line 598 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    i_lhs_chpl3 = &ri_chpl;
#line 598 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    *(i_lhs_chpl3) += INT64(1);
#line 599 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    if (li_chpl < INT64(4)) /* ZLINE: 599 /Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl */
#line 599 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    {
#line 599 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      tmp_chpl = (ri_chpl < INT64(4));
#line 599 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    } else /* ZLINE: 599 /Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl */
#line 599 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    {
#line 599 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      tmp_chpl = UINT8(false);
#line 599 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    }
#line 599 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  } while (tmp_chpl);
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(retcopy_chpl + INT64(0)) = *(LeftActives_chpl + INT64(0));
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(retcopy_chpl + INT64(1)) = *(LeftActives_chpl + INT64(1));
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(retcopy_chpl + INT64(2)) = *(LeftActives_chpl + INT64(2));
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(retcopy_chpl + INT64(3)) = *(LeftActives_chpl + INT64(3));
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(coerce_tmp_chpl3 + INT64(0)) = *(retcopy_chpl + INT64(0));
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(coerce_tmp_chpl3 + INT64(1)) = *(retcopy_chpl + INT64(1));
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(coerce_tmp_chpl3 + INT64(2)) = *(retcopy_chpl + INT64(2));
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(coerce_tmp_chpl3 + INT64(3)) = *(retcopy_chpl + INT64(3));
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(retcopy_chpl2 + INT64(0)) = *(RightActives_chpl + INT64(0));
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(retcopy_chpl2 + INT64(1)) = *(RightActives_chpl + INT64(1));
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(retcopy_chpl2 + INT64(2)) = *(RightActives_chpl + INT64(2));
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(retcopy_chpl2 + INT64(3)) = *(RightActives_chpl + INT64(3));
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(coerce_tmp_chpl4 + INT64(0)) = *(retcopy_chpl2 + INT64(0));
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(coerce_tmp_chpl4 + INT64(1)) = *(retcopy_chpl2 + INT64(1));
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(coerce_tmp_chpl4 + INT64(2)) = *(retcopy_chpl2 + INT64(2));
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(coerce_tmp_chpl4 + INT64(3)) = *(retcopy_chpl2 + INT64(3));
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  this_x0_chpl = &coerce_tmp_chpl3;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  this_x1_chpl = &coerce_tmp_chpl4;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  this_x2_chpl = inferredRank_chpl;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  (&ret_tmp_chpl3)->x0 = this_x0_chpl;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  (&ret_tmp_chpl3)->x1 = this_x1_chpl;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  (&ret_tmp_chpl3)->x2 = this_x2_chpl;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  call_tmp_chpl8 = ret_tmp_chpl3;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  copy_ret_tmp_x0_chpl = (&call_tmp_chpl8)->x0;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  copy_ret_tmp_x1_chpl = (&call_tmp_chpl8)->x1;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  copy_ret_tmp_x2_chpl = (&call_tmp_chpl8)->x2;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(elt_x0_chpl + INT64(0)) = *(*(copy_ret_tmp_x0_chpl) + INT64(0));
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(elt_x0_chpl + INT64(1)) = *(*(copy_ret_tmp_x0_chpl) + INT64(1));
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(elt_x0_chpl + INT64(2)) = *(*(copy_ret_tmp_x0_chpl) + INT64(2));
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(elt_x0_chpl + INT64(3)) = *(*(copy_ret_tmp_x0_chpl) + INT64(3));
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(elt_x1_chpl + INT64(0)) = *(*(copy_ret_tmp_x1_chpl) + INT64(0));
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(elt_x1_chpl + INT64(1)) = *(*(copy_ret_tmp_x1_chpl) + INT64(1));
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(elt_x1_chpl + INT64(2)) = *(*(copy_ret_tmp_x1_chpl) + INT64(2));
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(elt_x1_chpl + INT64(3)) = *(*(copy_ret_tmp_x1_chpl) + INT64(3));
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *((&ret_chpl)->x0 + INT64(0)) = *(elt_x0_chpl + INT64(0));
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *((&ret_chpl)->x0 + INT64(1)) = *(elt_x0_chpl + INT64(1));
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *((&ret_chpl)->x0 + INT64(2)) = *(elt_x0_chpl + INT64(2));
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *((&ret_chpl)->x0 + INT64(3)) = *(elt_x0_chpl + INT64(3));
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *((&ret_chpl)->x1 + INT64(0)) = *(elt_x1_chpl + INT64(0));
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *((&ret_chpl)->x1 + INT64(1)) = *(elt_x1_chpl + INT64(1));
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *((&ret_chpl)->x1 + INT64(2)) = *(elt_x1_chpl + INT64(2));
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *((&ret_chpl)->x1 + INT64(3)) = *(elt_x1_chpl + INT64(3));
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  (&ret_chpl)->x2 = copy_ret_tmp_x2_chpl;
#line 570 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(_retArg_chpl) = ret_chpl;
#line 570 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  return;
#line 570 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
}

#line 570 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
/* DSIUtil.chpl:570 */
#line 570 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
static void bulkCommComputeActiveDims_chpl7(_tuple_2_star_range_int64_t_both_one_chpl * LeftDims_chpl,
#line 570 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
                                            _tuple_2_star_range_int64_t_both_one_chpl * RightDims_chpl,
#line 570 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
                                            _tuple_3__tuple_2_star_int64_t__tuple_2_star_int64_t_int64_t_chpl * _retArg_chpl,
#line 570 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
                                            int64_t _ln_chpl,
#line 570 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
                                            int32_t _fn_chpl) {
#line 575 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  int64_t inferredRank_chpl;
#line 579 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _tuple_2_star_int64_t_chpl LeftActives_chpl;
#line 579 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _tuple_2_star_int64_t_chpl ret_tmp_chpl;
#line 579 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _tuple_2_star_int64_t_chpl RightActives_chpl;
#line 579 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _tuple_2_star_int64_t_chpl ret_tmp_chpl2;
#line 581 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  int64_t li_chpl;
#line 581 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  int64_t ri_chpl;
#line 593 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_int64_t_chpl call_tmp_chpl6 = NULL;
#line 594 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_int64_t_chpl call_tmp_chpl7 = NULL;
#line 596 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_int64_t_chpl i_lhs_chpl = NULL;
#line 597 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_int64_t_chpl i_lhs_chpl2 = NULL;
#line 598 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_int64_t_chpl i_lhs_chpl3 = NULL;
#line 599 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  chpl_bool tmp_chpl;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _tuple_3__ref__tuple_2_star_int64_t__ref__tuple_2_star_int64_t_int64_t_chpl call_tmp_chpl8;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _tuple_2_star_int64_t_chpl coerce_tmp_chpl3;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _tuple_2_star_int64_t_chpl retcopy_chpl;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _tuple_2_star_int64_t_chpl coerce_tmp_chpl4;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _tuple_2_star_int64_t_chpl retcopy_chpl2;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _tuple_3__ref__tuple_2_star_int64_t__ref__tuple_2_star_int64_t_int64_t_chpl ret_tmp_chpl3;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref__tuple_2_star_int64_t_chpl this_x0_chpl = NULL;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref__tuple_2_star_int64_t_chpl this_x1_chpl = NULL;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  int64_t this_x2_chpl;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref__tuple_2_star_int64_t_chpl copy_ret_tmp_x0_chpl = NULL;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref__tuple_2_star_int64_t_chpl copy_ret_tmp_x1_chpl = NULL;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  int64_t copy_ret_tmp_x2_chpl;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _tuple_3__tuple_2_star_int64_t__tuple_2_star_int64_t_int64_t_chpl ret_chpl;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _tuple_2_star_int64_t_chpl elt_x0_chpl;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _tuple_2_star_int64_t_chpl elt_x1_chpl;
#line 575 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  inferredRank_chpl = INT64(0);
#line 579 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(ret_tmp_chpl + INT64(0)) = INT64(0);
#line 579 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(ret_tmp_chpl + INT64(1)) = INT64(0);
#line 579 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(LeftActives_chpl + INT64(0)) = *(ret_tmp_chpl + INT64(0));
#line 579 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(LeftActives_chpl + INT64(1)) = *(ret_tmp_chpl + INT64(1));
#line 579 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(ret_tmp_chpl2 + INT64(0)) = INT64(0);
#line 579 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(ret_tmp_chpl2 + INT64(1)) = INT64(0);
#line 579 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(RightActives_chpl + INT64(0)) = *(ret_tmp_chpl2 + INT64(0));
#line 579 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(RightActives_chpl + INT64(1)) = *(ret_tmp_chpl2 + INT64(1));
#line 581 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  li_chpl = INT64(0);
#line 581 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  ri_chpl = INT64(0);
#line 590 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  do {
#line 591 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    advance_chpl24(LeftDims_chpl, RightDims_chpl, &li_chpl, &ri_chpl, _ln_chpl, _fn_chpl);
#line 593 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    call_tmp_chpl6 = this_chpl21(&LeftActives_chpl, inferredRank_chpl, _ln_chpl, _fn_chpl);
#line 593 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    *(call_tmp_chpl6) = li_chpl;
#line 594 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    call_tmp_chpl7 = this_chpl21(&RightActives_chpl, inferredRank_chpl, _ln_chpl, _fn_chpl);
#line 594 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    *(call_tmp_chpl7) = ri_chpl;
#line 596 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    i_lhs_chpl = &inferredRank_chpl;
#line 596 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    *(i_lhs_chpl) += INT64(1);
#line 597 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    i_lhs_chpl2 = &li_chpl;
#line 597 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    *(i_lhs_chpl2) += INT64(1);
#line 598 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    i_lhs_chpl3 = &ri_chpl;
#line 598 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    *(i_lhs_chpl3) += INT64(1);
#line 599 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    if (li_chpl < INT64(2)) /* ZLINE: 599 /Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl */
#line 599 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    {
#line 599 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      tmp_chpl = (ri_chpl < INT64(2));
#line 599 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    } else /* ZLINE: 599 /Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl */
#line 599 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    {
#line 599 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      tmp_chpl = UINT8(false);
#line 599 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    }
#line 599 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  } while (tmp_chpl);
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(retcopy_chpl + INT64(0)) = *(LeftActives_chpl + INT64(0));
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(retcopy_chpl + INT64(1)) = *(LeftActives_chpl + INT64(1));
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(coerce_tmp_chpl3 + INT64(0)) = *(retcopy_chpl + INT64(0));
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(coerce_tmp_chpl3 + INT64(1)) = *(retcopy_chpl + INT64(1));
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(retcopy_chpl2 + INT64(0)) = *(RightActives_chpl + INT64(0));
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(retcopy_chpl2 + INT64(1)) = *(RightActives_chpl + INT64(1));
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(coerce_tmp_chpl4 + INT64(0)) = *(retcopy_chpl2 + INT64(0));
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(coerce_tmp_chpl4 + INT64(1)) = *(retcopy_chpl2 + INT64(1));
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  this_x0_chpl = &coerce_tmp_chpl3;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  this_x1_chpl = &coerce_tmp_chpl4;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  this_x2_chpl = inferredRank_chpl;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  (&ret_tmp_chpl3)->x0 = this_x0_chpl;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  (&ret_tmp_chpl3)->x1 = this_x1_chpl;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  (&ret_tmp_chpl3)->x2 = this_x2_chpl;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  call_tmp_chpl8 = ret_tmp_chpl3;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  copy_ret_tmp_x0_chpl = (&call_tmp_chpl8)->x0;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  copy_ret_tmp_x1_chpl = (&call_tmp_chpl8)->x1;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  copy_ret_tmp_x2_chpl = (&call_tmp_chpl8)->x2;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(elt_x0_chpl + INT64(0)) = *(*(copy_ret_tmp_x0_chpl) + INT64(0));
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(elt_x0_chpl + INT64(1)) = *(*(copy_ret_tmp_x0_chpl) + INT64(1));
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(elt_x1_chpl + INT64(0)) = *(*(copy_ret_tmp_x1_chpl) + INT64(0));
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(elt_x1_chpl + INT64(1)) = *(*(copy_ret_tmp_x1_chpl) + INT64(1));
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *((&ret_chpl)->x0 + INT64(0)) = *(elt_x0_chpl + INT64(0));
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *((&ret_chpl)->x0 + INT64(1)) = *(elt_x0_chpl + INT64(1));
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *((&ret_chpl)->x1 + INT64(0)) = *(elt_x1_chpl + INT64(0));
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *((&ret_chpl)->x1 + INT64(1)) = *(elt_x1_chpl + INT64(1));
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  (&ret_chpl)->x2 = copy_ret_tmp_x2_chpl;
#line 570 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(_retArg_chpl) = ret_chpl;
#line 570 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  return;
#line 570 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
}

#line 570 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
/* DSIUtil.chpl:570 */
#line 570 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
static void bulkCommComputeActiveDims_chpl4(_tuple_1_star_range_int64_t_both_one_chpl * LeftDims_chpl,
#line 570 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
                                            _tuple_1_star_range_int64_t_both_one_chpl * RightDims_chpl,
#line 570 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
                                            _tuple_3__tuple_1_star_int64_t__tuple_1_star_int64_t_int64_t_chpl * _retArg_chpl,
#line 570 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
                                            int64_t _ln_chpl,
#line 570 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
                                            int32_t _fn_chpl) {
#line 575 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  int64_t inferredRank_chpl;
#line 579 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _tuple_1_star_int64_t_chpl LeftActives_chpl;
#line 579 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _tuple_1_star_int64_t_chpl ret_tmp_chpl;
#line 579 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _tuple_1_star_int64_t_chpl RightActives_chpl;
#line 579 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _tuple_1_star_int64_t_chpl ret_tmp_chpl2;
#line 581 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  int64_t li_chpl;
#line 581 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  int64_t ri_chpl;
#line 593 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_int64_t_chpl call_tmp_chpl6 = NULL;
#line 594 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_int64_t_chpl call_tmp_chpl7 = NULL;
#line 596 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_int64_t_chpl i_lhs_chpl = NULL;
#line 597 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_int64_t_chpl i_lhs_chpl2 = NULL;
#line 598 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_int64_t_chpl i_lhs_chpl3 = NULL;
#line 599 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  chpl_bool tmp_chpl;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _tuple_3__ref__tuple_1_star_int64_t__ref__tuple_1_star_int64_t_int64_t_chpl call_tmp_chpl8;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _tuple_1_star_int64_t_chpl coerce_tmp_chpl3;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _tuple_1_star_int64_t_chpl retcopy_chpl;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _tuple_1_star_int64_t_chpl coerce_tmp_chpl4;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _tuple_1_star_int64_t_chpl retcopy_chpl2;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _tuple_3__ref__tuple_1_star_int64_t__ref__tuple_1_star_int64_t_int64_t_chpl ret_tmp_chpl3;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref__tuple_1_star_int64_t_chpl this_x0_chpl = NULL;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref__tuple_1_star_int64_t_chpl this_x1_chpl = NULL;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  int64_t this_x2_chpl;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref__tuple_1_star_int64_t_chpl copy_ret_tmp_x0_chpl = NULL;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref__tuple_1_star_int64_t_chpl copy_ret_tmp_x1_chpl = NULL;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  int64_t copy_ret_tmp_x2_chpl;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _tuple_3__tuple_1_star_int64_t__tuple_1_star_int64_t_int64_t_chpl ret_chpl;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _tuple_1_star_int64_t_chpl elt_x0_chpl;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _tuple_1_star_int64_t_chpl elt_x1_chpl;
#line 575 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  inferredRank_chpl = INT64(0);
#line 579 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(ret_tmp_chpl + INT64(0)) = INT64(0);
#line 579 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(LeftActives_chpl + INT64(0)) = *(ret_tmp_chpl + INT64(0));
#line 579 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(ret_tmp_chpl2 + INT64(0)) = INT64(0);
#line 579 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(RightActives_chpl + INT64(0)) = *(ret_tmp_chpl2 + INT64(0));
#line 581 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  li_chpl = INT64(0);
#line 581 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  ri_chpl = INT64(0);
#line 590 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  do {
#line 591 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    advance_chpl22(LeftDims_chpl, RightDims_chpl, &li_chpl, &ri_chpl, _ln_chpl, _fn_chpl);
#line 593 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    call_tmp_chpl6 = this_chpl25(&LeftActives_chpl, inferredRank_chpl, _ln_chpl, _fn_chpl);
#line 593 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    *(call_tmp_chpl6) = li_chpl;
#line 594 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    call_tmp_chpl7 = this_chpl25(&RightActives_chpl, inferredRank_chpl, _ln_chpl, _fn_chpl);
#line 594 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    *(call_tmp_chpl7) = ri_chpl;
#line 596 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    i_lhs_chpl = &inferredRank_chpl;
#line 596 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    *(i_lhs_chpl) += INT64(1);
#line 597 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    i_lhs_chpl2 = &li_chpl;
#line 597 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    *(i_lhs_chpl2) += INT64(1);
#line 598 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    i_lhs_chpl3 = &ri_chpl;
#line 598 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    *(i_lhs_chpl3) += INT64(1);
#line 599 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    if (li_chpl < INT64(1)) /* ZLINE: 599 /Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl */
#line 599 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    {
#line 599 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      tmp_chpl = (ri_chpl < INT64(1));
#line 599 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    } else /* ZLINE: 599 /Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl */
#line 599 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    {
#line 599 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      tmp_chpl = UINT8(false);
#line 599 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    }
#line 599 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  } while (tmp_chpl);
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(retcopy_chpl + INT64(0)) = *(LeftActives_chpl + INT64(0));
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(coerce_tmp_chpl3 + INT64(0)) = *(retcopy_chpl + INT64(0));
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(retcopy_chpl2 + INT64(0)) = *(RightActives_chpl + INT64(0));
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(coerce_tmp_chpl4 + INT64(0)) = *(retcopy_chpl2 + INT64(0));
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  this_x0_chpl = &coerce_tmp_chpl3;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  this_x1_chpl = &coerce_tmp_chpl4;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  this_x2_chpl = inferredRank_chpl;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  (&ret_tmp_chpl3)->x0 = this_x0_chpl;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  (&ret_tmp_chpl3)->x1 = this_x1_chpl;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  (&ret_tmp_chpl3)->x2 = this_x2_chpl;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  call_tmp_chpl8 = ret_tmp_chpl3;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  copy_ret_tmp_x0_chpl = (&call_tmp_chpl8)->x0;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  copy_ret_tmp_x1_chpl = (&call_tmp_chpl8)->x1;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  copy_ret_tmp_x2_chpl = (&call_tmp_chpl8)->x2;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(elt_x0_chpl + INT64(0)) = *(*(copy_ret_tmp_x0_chpl) + INT64(0));
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(elt_x1_chpl + INT64(0)) = *(*(copy_ret_tmp_x1_chpl) + INT64(0));
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *((&ret_chpl)->x0 + INT64(0)) = *(elt_x0_chpl + INT64(0));
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *((&ret_chpl)->x1 + INT64(0)) = *(elt_x1_chpl + INT64(0));
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  (&ret_chpl)->x2 = copy_ret_tmp_x2_chpl;
#line 570 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(_retArg_chpl) = ret_chpl;
#line 570 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  return;
#line 570 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
}

#line 570 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
/* DSIUtil.chpl:570 */
#line 570 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
static void bulkCommComputeActiveDims_chpl3(_tuple_3_star_range_int64_t_both_one_chpl * LeftDims_chpl,
#line 570 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
                                            _tuple_3_star_range_int64_t_both_one_chpl * RightDims_chpl,
#line 570 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
                                            _tuple_3__tuple_3_star_int64_t__tuple_3_star_int64_t_int64_t_chpl * _retArg_chpl,
#line 570 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
                                            int64_t _ln_chpl,
#line 570 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
                                            int32_t _fn_chpl) {
#line 575 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  int64_t inferredRank_chpl;
#line 579 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _tuple_3_star_int64_t_chpl LeftActives_chpl;
#line 579 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _tuple_3_star_int64_t_chpl ret_tmp_chpl;
#line 579 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _tuple_3_star_int64_t_chpl RightActives_chpl;
#line 579 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _tuple_3_star_int64_t_chpl ret_tmp_chpl2;
#line 581 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  int64_t li_chpl;
#line 581 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  int64_t ri_chpl;
#line 593 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_int64_t_chpl call_tmp_chpl6 = NULL;
#line 594 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_int64_t_chpl call_tmp_chpl7 = NULL;
#line 596 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_int64_t_chpl i_lhs_chpl = NULL;
#line 597 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_int64_t_chpl i_lhs_chpl2 = NULL;
#line 598 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_int64_t_chpl i_lhs_chpl3 = NULL;
#line 599 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  chpl_bool tmp_chpl;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _tuple_3__ref__tuple_3_star_int64_t__ref__tuple_3_star_int64_t_int64_t_chpl call_tmp_chpl8;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _tuple_3_star_int64_t_chpl coerce_tmp_chpl3;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _tuple_3_star_int64_t_chpl retcopy_chpl;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _tuple_3_star_int64_t_chpl coerce_tmp_chpl4;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _tuple_3_star_int64_t_chpl retcopy_chpl2;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _tuple_3__ref__tuple_3_star_int64_t__ref__tuple_3_star_int64_t_int64_t_chpl ret_tmp_chpl3;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref__tuple_3_star_int64_t_chpl this_x0_chpl = NULL;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref__tuple_3_star_int64_t_chpl this_x1_chpl = NULL;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  int64_t this_x2_chpl;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref__tuple_3_star_int64_t_chpl copy_ret_tmp_x0_chpl = NULL;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref__tuple_3_star_int64_t_chpl copy_ret_tmp_x1_chpl = NULL;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  int64_t copy_ret_tmp_x2_chpl;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _tuple_3__tuple_3_star_int64_t__tuple_3_star_int64_t_int64_t_chpl ret_chpl;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _tuple_3_star_int64_t_chpl elt_x0_chpl;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _tuple_3_star_int64_t_chpl elt_x1_chpl;
#line 575 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  inferredRank_chpl = INT64(0);
#line 579 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(ret_tmp_chpl + INT64(0)) = INT64(0);
#line 579 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(ret_tmp_chpl + INT64(1)) = INT64(0);
#line 579 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(ret_tmp_chpl + INT64(2)) = INT64(0);
#line 579 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(LeftActives_chpl + INT64(0)) = *(ret_tmp_chpl + INT64(0));
#line 579 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(LeftActives_chpl + INT64(1)) = *(ret_tmp_chpl + INT64(1));
#line 579 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(LeftActives_chpl + INT64(2)) = *(ret_tmp_chpl + INT64(2));
#line 579 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(ret_tmp_chpl2 + INT64(0)) = INT64(0);
#line 579 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(ret_tmp_chpl2 + INT64(1)) = INT64(0);
#line 579 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(ret_tmp_chpl2 + INT64(2)) = INT64(0);
#line 579 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(RightActives_chpl + INT64(0)) = *(ret_tmp_chpl2 + INT64(0));
#line 579 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(RightActives_chpl + INT64(1)) = *(ret_tmp_chpl2 + INT64(1));
#line 579 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(RightActives_chpl + INT64(2)) = *(ret_tmp_chpl2 + INT64(2));
#line 581 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  li_chpl = INT64(0);
#line 581 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  ri_chpl = INT64(0);
#line 590 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  do {
#line 591 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    advance_chpl26(LeftDims_chpl, RightDims_chpl, &li_chpl, &ri_chpl, _ln_chpl, _fn_chpl);
#line 593 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    call_tmp_chpl6 = this_chpl23(&LeftActives_chpl, inferredRank_chpl, _ln_chpl, _fn_chpl);
#line 593 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    *(call_tmp_chpl6) = li_chpl;
#line 594 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    call_tmp_chpl7 = this_chpl23(&RightActives_chpl, inferredRank_chpl, _ln_chpl, _fn_chpl);
#line 594 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    *(call_tmp_chpl7) = ri_chpl;
#line 596 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    i_lhs_chpl = &inferredRank_chpl;
#line 596 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    *(i_lhs_chpl) += INT64(1);
#line 597 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    i_lhs_chpl2 = &li_chpl;
#line 597 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    *(i_lhs_chpl2) += INT64(1);
#line 598 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    i_lhs_chpl3 = &ri_chpl;
#line 598 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    *(i_lhs_chpl3) += INT64(1);
#line 599 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    if (li_chpl < INT64(3)) /* ZLINE: 599 /Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl */
#line 599 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    {
#line 599 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      tmp_chpl = (ri_chpl < INT64(3));
#line 599 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    } else /* ZLINE: 599 /Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl */
#line 599 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    {
#line 599 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      tmp_chpl = UINT8(false);
#line 599 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    }
#line 599 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  } while (tmp_chpl);
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(retcopy_chpl + INT64(0)) = *(LeftActives_chpl + INT64(0));
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(retcopy_chpl + INT64(1)) = *(LeftActives_chpl + INT64(1));
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(retcopy_chpl + INT64(2)) = *(LeftActives_chpl + INT64(2));
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(coerce_tmp_chpl3 + INT64(0)) = *(retcopy_chpl + INT64(0));
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(coerce_tmp_chpl3 + INT64(1)) = *(retcopy_chpl + INT64(1));
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(coerce_tmp_chpl3 + INT64(2)) = *(retcopy_chpl + INT64(2));
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(retcopy_chpl2 + INT64(0)) = *(RightActives_chpl + INT64(0));
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(retcopy_chpl2 + INT64(1)) = *(RightActives_chpl + INT64(1));
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(retcopy_chpl2 + INT64(2)) = *(RightActives_chpl + INT64(2));
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(coerce_tmp_chpl4 + INT64(0)) = *(retcopy_chpl2 + INT64(0));
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(coerce_tmp_chpl4 + INT64(1)) = *(retcopy_chpl2 + INT64(1));
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(coerce_tmp_chpl4 + INT64(2)) = *(retcopy_chpl2 + INT64(2));
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  this_x0_chpl = &coerce_tmp_chpl3;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  this_x1_chpl = &coerce_tmp_chpl4;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  this_x2_chpl = inferredRank_chpl;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  (&ret_tmp_chpl3)->x0 = this_x0_chpl;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  (&ret_tmp_chpl3)->x1 = this_x1_chpl;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  (&ret_tmp_chpl3)->x2 = this_x2_chpl;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  call_tmp_chpl8 = ret_tmp_chpl3;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  copy_ret_tmp_x0_chpl = (&call_tmp_chpl8)->x0;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  copy_ret_tmp_x1_chpl = (&call_tmp_chpl8)->x1;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  copy_ret_tmp_x2_chpl = (&call_tmp_chpl8)->x2;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(elt_x0_chpl + INT64(0)) = *(*(copy_ret_tmp_x0_chpl) + INT64(0));
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(elt_x0_chpl + INT64(1)) = *(*(copy_ret_tmp_x0_chpl) + INT64(1));
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(elt_x0_chpl + INT64(2)) = *(*(copy_ret_tmp_x0_chpl) + INT64(2));
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(elt_x1_chpl + INT64(0)) = *(*(copy_ret_tmp_x1_chpl) + INT64(0));
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(elt_x1_chpl + INT64(1)) = *(*(copy_ret_tmp_x1_chpl) + INT64(1));
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(elt_x1_chpl + INT64(2)) = *(*(copy_ret_tmp_x1_chpl) + INT64(2));
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *((&ret_chpl)->x0 + INT64(0)) = *(elt_x0_chpl + INT64(0));
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *((&ret_chpl)->x0 + INT64(1)) = *(elt_x0_chpl + INT64(1));
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *((&ret_chpl)->x0 + INT64(2)) = *(elt_x0_chpl + INT64(2));
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *((&ret_chpl)->x1 + INT64(0)) = *(elt_x1_chpl + INT64(0));
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *((&ret_chpl)->x1 + INT64(1)) = *(elt_x1_chpl + INT64(1));
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *((&ret_chpl)->x1 + INT64(2)) = *(elt_x1_chpl + INT64(2));
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  (&ret_chpl)->x2 = copy_ret_tmp_x2_chpl;
#line 570 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(_retArg_chpl) = ret_chpl;
#line 570 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  return;
#line 570 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
}

#line 570 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
/* DSIUtil.chpl:570 */
#line 570 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
static void bulkCommComputeActiveDims_chpl6(_tuple_3_star_range_int64_t_both_one_chpl * LeftDims_chpl,
#line 570 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
                                            _tuple_4_star_range_int64_t_both_one_chpl * RightDims_chpl,
#line 570 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
                                            _tuple_3__tuple_3_star_int64_t__tuple_3_star_int64_t_int64_t_chpl * _retArg_chpl,
#line 570 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
                                            int64_t _ln_chpl,
#line 570 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
                                            int32_t _fn_chpl) {
#line 575 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  int64_t inferredRank_chpl;
#line 579 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _tuple_3_star_int64_t_chpl LeftActives_chpl;
#line 579 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _tuple_3_star_int64_t_chpl ret_tmp_chpl;
#line 579 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _tuple_3_star_int64_t_chpl RightActives_chpl;
#line 579 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _tuple_3_star_int64_t_chpl ret_tmp_chpl2;
#line 581 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  int64_t li_chpl;
#line 581 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  int64_t ri_chpl;
#line 593 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_int64_t_chpl call_tmp_chpl6 = NULL;
#line 594 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_int64_t_chpl call_tmp_chpl7 = NULL;
#line 596 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_int64_t_chpl i_lhs_chpl = NULL;
#line 597 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_int64_t_chpl i_lhs_chpl2 = NULL;
#line 598 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_int64_t_chpl i_lhs_chpl3 = NULL;
#line 599 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  chpl_bool tmp_chpl;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _tuple_3__ref__tuple_3_star_int64_t__ref__tuple_3_star_int64_t_int64_t_chpl call_tmp_chpl8;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _tuple_3_star_int64_t_chpl coerce_tmp_chpl3;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _tuple_3_star_int64_t_chpl retcopy_chpl;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _tuple_3_star_int64_t_chpl coerce_tmp_chpl4;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _tuple_3_star_int64_t_chpl retcopy_chpl2;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _tuple_3__ref__tuple_3_star_int64_t__ref__tuple_3_star_int64_t_int64_t_chpl ret_tmp_chpl3;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref__tuple_3_star_int64_t_chpl this_x0_chpl = NULL;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref__tuple_3_star_int64_t_chpl this_x1_chpl = NULL;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  int64_t this_x2_chpl;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref__tuple_3_star_int64_t_chpl copy_ret_tmp_x0_chpl = NULL;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref__tuple_3_star_int64_t_chpl copy_ret_tmp_x1_chpl = NULL;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  int64_t copy_ret_tmp_x2_chpl;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _tuple_3__tuple_3_star_int64_t__tuple_3_star_int64_t_int64_t_chpl ret_chpl;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _tuple_3_star_int64_t_chpl elt_x0_chpl;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _tuple_3_star_int64_t_chpl elt_x1_chpl;
#line 575 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  inferredRank_chpl = INT64(0);
#line 579 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(ret_tmp_chpl + INT64(0)) = INT64(0);
#line 579 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(ret_tmp_chpl + INT64(1)) = INT64(0);
#line 579 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(ret_tmp_chpl + INT64(2)) = INT64(0);
#line 579 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(LeftActives_chpl + INT64(0)) = *(ret_tmp_chpl + INT64(0));
#line 579 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(LeftActives_chpl + INT64(1)) = *(ret_tmp_chpl + INT64(1));
#line 579 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(LeftActives_chpl + INT64(2)) = *(ret_tmp_chpl + INT64(2));
#line 579 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(ret_tmp_chpl2 + INT64(0)) = INT64(0);
#line 579 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(ret_tmp_chpl2 + INT64(1)) = INT64(0);
#line 579 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(ret_tmp_chpl2 + INT64(2)) = INT64(0);
#line 579 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(RightActives_chpl + INT64(0)) = *(ret_tmp_chpl2 + INT64(0));
#line 579 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(RightActives_chpl + INT64(1)) = *(ret_tmp_chpl2 + INT64(1));
#line 579 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(RightActives_chpl + INT64(2)) = *(ret_tmp_chpl2 + INT64(2));
#line 581 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  li_chpl = INT64(0);
#line 581 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  ri_chpl = INT64(0);
#line 590 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  do {
#line 591 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    advance_chpl20(LeftDims_chpl, RightDims_chpl, &li_chpl, &ri_chpl, _ln_chpl, _fn_chpl);
#line 593 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    call_tmp_chpl6 = this_chpl23(&LeftActives_chpl, inferredRank_chpl, _ln_chpl, _fn_chpl);
#line 593 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    *(call_tmp_chpl6) = li_chpl;
#line 594 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    call_tmp_chpl7 = this_chpl23(&RightActives_chpl, inferredRank_chpl, _ln_chpl, _fn_chpl);
#line 594 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    *(call_tmp_chpl7) = ri_chpl;
#line 596 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    i_lhs_chpl = &inferredRank_chpl;
#line 596 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    *(i_lhs_chpl) += INT64(1);
#line 597 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    i_lhs_chpl2 = &li_chpl;
#line 597 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    *(i_lhs_chpl2) += INT64(1);
#line 598 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    i_lhs_chpl3 = &ri_chpl;
#line 598 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    *(i_lhs_chpl3) += INT64(1);
#line 599 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    if (li_chpl < INT64(3)) /* ZLINE: 599 /Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl */
#line 599 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    {
#line 599 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      tmp_chpl = (ri_chpl < INT64(4));
#line 599 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    } else /* ZLINE: 599 /Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl */
#line 599 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    {
#line 599 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      tmp_chpl = UINT8(false);
#line 599 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    }
#line 599 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  } while (tmp_chpl);
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(retcopy_chpl + INT64(0)) = *(LeftActives_chpl + INT64(0));
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(retcopy_chpl + INT64(1)) = *(LeftActives_chpl + INT64(1));
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(retcopy_chpl + INT64(2)) = *(LeftActives_chpl + INT64(2));
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(coerce_tmp_chpl3 + INT64(0)) = *(retcopy_chpl + INT64(0));
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(coerce_tmp_chpl3 + INT64(1)) = *(retcopy_chpl + INT64(1));
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(coerce_tmp_chpl3 + INT64(2)) = *(retcopy_chpl + INT64(2));
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(retcopy_chpl2 + INT64(0)) = *(RightActives_chpl + INT64(0));
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(retcopy_chpl2 + INT64(1)) = *(RightActives_chpl + INT64(1));
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(retcopy_chpl2 + INT64(2)) = *(RightActives_chpl + INT64(2));
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(coerce_tmp_chpl4 + INT64(0)) = *(retcopy_chpl2 + INT64(0));
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(coerce_tmp_chpl4 + INT64(1)) = *(retcopy_chpl2 + INT64(1));
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(coerce_tmp_chpl4 + INT64(2)) = *(retcopy_chpl2 + INT64(2));
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  this_x0_chpl = &coerce_tmp_chpl3;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  this_x1_chpl = &coerce_tmp_chpl4;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  this_x2_chpl = inferredRank_chpl;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  (&ret_tmp_chpl3)->x0 = this_x0_chpl;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  (&ret_tmp_chpl3)->x1 = this_x1_chpl;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  (&ret_tmp_chpl3)->x2 = this_x2_chpl;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  call_tmp_chpl8 = ret_tmp_chpl3;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  copy_ret_tmp_x0_chpl = (&call_tmp_chpl8)->x0;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  copy_ret_tmp_x1_chpl = (&call_tmp_chpl8)->x1;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  copy_ret_tmp_x2_chpl = (&call_tmp_chpl8)->x2;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(elt_x0_chpl + INT64(0)) = *(*(copy_ret_tmp_x0_chpl) + INT64(0));
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(elt_x0_chpl + INT64(1)) = *(*(copy_ret_tmp_x0_chpl) + INT64(1));
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(elt_x0_chpl + INT64(2)) = *(*(copy_ret_tmp_x0_chpl) + INT64(2));
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(elt_x1_chpl + INT64(0)) = *(*(copy_ret_tmp_x1_chpl) + INT64(0));
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(elt_x1_chpl + INT64(1)) = *(*(copy_ret_tmp_x1_chpl) + INT64(1));
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(elt_x1_chpl + INT64(2)) = *(*(copy_ret_tmp_x1_chpl) + INT64(2));
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *((&ret_chpl)->x0 + INT64(0)) = *(elt_x0_chpl + INT64(0));
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *((&ret_chpl)->x0 + INT64(1)) = *(elt_x0_chpl + INT64(1));
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *((&ret_chpl)->x0 + INT64(2)) = *(elt_x0_chpl + INT64(2));
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *((&ret_chpl)->x1 + INT64(0)) = *(elt_x1_chpl + INT64(0));
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *((&ret_chpl)->x1 + INT64(1)) = *(elt_x1_chpl + INT64(1));
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *((&ret_chpl)->x1 + INT64(2)) = *(elt_x1_chpl + INT64(2));
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  (&ret_chpl)->x2 = copy_ret_tmp_x2_chpl;
#line 570 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(_retArg_chpl) = ret_chpl;
#line 570 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  return;
#line 570 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
}

#line 570 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
/* DSIUtil.chpl:570 */
#line 570 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
static void bulkCommComputeActiveDims_chpl(_tuple_2_star_range_int64_t_both_one_chpl * LeftDims_chpl,
#line 570 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
                                           _tuple_1_star_range_int64_t_both_one_chpl * RightDims_chpl,
#line 570 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
                                           _tuple_3__tuple_1_star_int64_t__tuple_1_star_int64_t_int64_t_chpl * _retArg_chpl,
#line 570 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
                                           int64_t _ln_chpl,
#line 570 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
                                           int32_t _fn_chpl) {
#line 575 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  int64_t inferredRank_chpl;
#line 579 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _tuple_1_star_int64_t_chpl LeftActives_chpl;
#line 579 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _tuple_1_star_int64_t_chpl ret_tmp_chpl;
#line 579 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _tuple_1_star_int64_t_chpl RightActives_chpl;
#line 579 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _tuple_1_star_int64_t_chpl ret_tmp_chpl2;
#line 581 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  int64_t li_chpl;
#line 581 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  int64_t ri_chpl;
#line 593 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_int64_t_chpl call_tmp_chpl6 = NULL;
#line 594 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_int64_t_chpl call_tmp_chpl7 = NULL;
#line 596 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_int64_t_chpl i_lhs_chpl = NULL;
#line 597 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_int64_t_chpl i_lhs_chpl2 = NULL;
#line 598 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_int64_t_chpl i_lhs_chpl3 = NULL;
#line 599 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  chpl_bool tmp_chpl;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _tuple_3__ref__tuple_1_star_int64_t__ref__tuple_1_star_int64_t_int64_t_chpl call_tmp_chpl8;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _tuple_1_star_int64_t_chpl coerce_tmp_chpl3;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _tuple_1_star_int64_t_chpl retcopy_chpl;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _tuple_1_star_int64_t_chpl coerce_tmp_chpl4;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _tuple_1_star_int64_t_chpl retcopy_chpl2;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _tuple_3__ref__tuple_1_star_int64_t__ref__tuple_1_star_int64_t_int64_t_chpl ret_tmp_chpl3;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref__tuple_1_star_int64_t_chpl this_x0_chpl = NULL;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref__tuple_1_star_int64_t_chpl this_x1_chpl = NULL;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  int64_t this_x2_chpl;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref__tuple_1_star_int64_t_chpl copy_ret_tmp_x0_chpl = NULL;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref__tuple_1_star_int64_t_chpl copy_ret_tmp_x1_chpl = NULL;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  int64_t copy_ret_tmp_x2_chpl;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _tuple_3__tuple_1_star_int64_t__tuple_1_star_int64_t_int64_t_chpl ret_chpl;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _tuple_1_star_int64_t_chpl elt_x0_chpl;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _tuple_1_star_int64_t_chpl elt_x1_chpl;
#line 575 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  inferredRank_chpl = INT64(0);
#line 579 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(ret_tmp_chpl + INT64(0)) = INT64(0);
#line 579 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(LeftActives_chpl + INT64(0)) = *(ret_tmp_chpl + INT64(0));
#line 579 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(ret_tmp_chpl2 + INT64(0)) = INT64(0);
#line 579 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(RightActives_chpl + INT64(0)) = *(ret_tmp_chpl2 + INT64(0));
#line 581 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  li_chpl = INT64(0);
#line 581 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  ri_chpl = INT64(0);
#line 590 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  do {
#line 591 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    advance_chpl25(LeftDims_chpl, RightDims_chpl, &li_chpl, &ri_chpl, _ln_chpl, _fn_chpl);
#line 593 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    call_tmp_chpl6 = this_chpl25(&LeftActives_chpl, inferredRank_chpl, _ln_chpl, _fn_chpl);
#line 593 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    *(call_tmp_chpl6) = li_chpl;
#line 594 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    call_tmp_chpl7 = this_chpl25(&RightActives_chpl, inferredRank_chpl, _ln_chpl, _fn_chpl);
#line 594 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    *(call_tmp_chpl7) = ri_chpl;
#line 596 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    i_lhs_chpl = &inferredRank_chpl;
#line 596 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    *(i_lhs_chpl) += INT64(1);
#line 597 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    i_lhs_chpl2 = &li_chpl;
#line 597 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    *(i_lhs_chpl2) += INT64(1);
#line 598 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    i_lhs_chpl3 = &ri_chpl;
#line 598 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    *(i_lhs_chpl3) += INT64(1);
#line 599 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    if (li_chpl < INT64(2)) /* ZLINE: 599 /Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl */
#line 599 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    {
#line 599 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      tmp_chpl = (ri_chpl < INT64(1));
#line 599 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    } else /* ZLINE: 599 /Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl */
#line 599 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    {
#line 599 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      tmp_chpl = UINT8(false);
#line 599 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    }
#line 599 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  } while (tmp_chpl);
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(retcopy_chpl + INT64(0)) = *(LeftActives_chpl + INT64(0));
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(coerce_tmp_chpl3 + INT64(0)) = *(retcopy_chpl + INT64(0));
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(retcopy_chpl2 + INT64(0)) = *(RightActives_chpl + INT64(0));
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(coerce_tmp_chpl4 + INT64(0)) = *(retcopy_chpl2 + INT64(0));
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  this_x0_chpl = &coerce_tmp_chpl3;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  this_x1_chpl = &coerce_tmp_chpl4;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  this_x2_chpl = inferredRank_chpl;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  (&ret_tmp_chpl3)->x0 = this_x0_chpl;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  (&ret_tmp_chpl3)->x1 = this_x1_chpl;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  (&ret_tmp_chpl3)->x2 = this_x2_chpl;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  call_tmp_chpl8 = ret_tmp_chpl3;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  copy_ret_tmp_x0_chpl = (&call_tmp_chpl8)->x0;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  copy_ret_tmp_x1_chpl = (&call_tmp_chpl8)->x1;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  copy_ret_tmp_x2_chpl = (&call_tmp_chpl8)->x2;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(elt_x0_chpl + INT64(0)) = *(*(copy_ret_tmp_x0_chpl) + INT64(0));
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(elt_x1_chpl + INT64(0)) = *(*(copy_ret_tmp_x1_chpl) + INT64(0));
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *((&ret_chpl)->x0 + INT64(0)) = *(elt_x0_chpl + INT64(0));
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *((&ret_chpl)->x1 + INT64(0)) = *(elt_x1_chpl + INT64(0));
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  (&ret_chpl)->x2 = copy_ret_tmp_x2_chpl;
#line 570 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(_retArg_chpl) = ret_chpl;
#line 570 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  return;
#line 570 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
}

#line 570 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
/* DSIUtil.chpl:570 */
#line 570 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
static void bulkCommComputeActiveDims_chpl5(_tuple_2_star_range_int64_t_both_one_chpl * LeftDims_chpl,
#line 570 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
                                            _tuple_3_star_range_int64_t_both_one_chpl * RightDims_chpl,
#line 570 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
                                            _tuple_3__tuple_2_star_int64_t__tuple_2_star_int64_t_int64_t_chpl * _retArg_chpl,
#line 570 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
                                            int64_t _ln_chpl,
#line 570 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
                                            int32_t _fn_chpl) {
#line 575 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  int64_t inferredRank_chpl;
#line 579 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _tuple_2_star_int64_t_chpl LeftActives_chpl;
#line 579 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _tuple_2_star_int64_t_chpl ret_tmp_chpl;
#line 579 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _tuple_2_star_int64_t_chpl RightActives_chpl;
#line 579 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _tuple_2_star_int64_t_chpl ret_tmp_chpl2;
#line 581 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  int64_t li_chpl;
#line 581 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  int64_t ri_chpl;
#line 593 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_int64_t_chpl call_tmp_chpl6 = NULL;
#line 594 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_int64_t_chpl call_tmp_chpl7 = NULL;
#line 596 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_int64_t_chpl i_lhs_chpl = NULL;
#line 597 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_int64_t_chpl i_lhs_chpl2 = NULL;
#line 598 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_int64_t_chpl i_lhs_chpl3 = NULL;
#line 599 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  chpl_bool tmp_chpl;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _tuple_3__ref__tuple_2_star_int64_t__ref__tuple_2_star_int64_t_int64_t_chpl call_tmp_chpl8;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _tuple_2_star_int64_t_chpl coerce_tmp_chpl3;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _tuple_2_star_int64_t_chpl retcopy_chpl;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _tuple_2_star_int64_t_chpl coerce_tmp_chpl4;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _tuple_2_star_int64_t_chpl retcopy_chpl2;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _tuple_3__ref__tuple_2_star_int64_t__ref__tuple_2_star_int64_t_int64_t_chpl ret_tmp_chpl3;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref__tuple_2_star_int64_t_chpl this_x0_chpl = NULL;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref__tuple_2_star_int64_t_chpl this_x1_chpl = NULL;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  int64_t this_x2_chpl;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref__tuple_2_star_int64_t_chpl copy_ret_tmp_x0_chpl = NULL;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref__tuple_2_star_int64_t_chpl copy_ret_tmp_x1_chpl = NULL;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  int64_t copy_ret_tmp_x2_chpl;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _tuple_3__tuple_2_star_int64_t__tuple_2_star_int64_t_int64_t_chpl ret_chpl;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _tuple_2_star_int64_t_chpl elt_x0_chpl;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _tuple_2_star_int64_t_chpl elt_x1_chpl;
#line 575 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  inferredRank_chpl = INT64(0);
#line 579 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(ret_tmp_chpl + INT64(0)) = INT64(0);
#line 579 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(ret_tmp_chpl + INT64(1)) = INT64(0);
#line 579 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(LeftActives_chpl + INT64(0)) = *(ret_tmp_chpl + INT64(0));
#line 579 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(LeftActives_chpl + INT64(1)) = *(ret_tmp_chpl + INT64(1));
#line 579 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(ret_tmp_chpl2 + INT64(0)) = INT64(0);
#line 579 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(ret_tmp_chpl2 + INT64(1)) = INT64(0);
#line 579 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(RightActives_chpl + INT64(0)) = *(ret_tmp_chpl2 + INT64(0));
#line 579 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(RightActives_chpl + INT64(1)) = *(ret_tmp_chpl2 + INT64(1));
#line 581 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  li_chpl = INT64(0);
#line 581 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  ri_chpl = INT64(0);
#line 590 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  do {
#line 591 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    advance_chpl21(LeftDims_chpl, RightDims_chpl, &li_chpl, &ri_chpl, _ln_chpl, _fn_chpl);
#line 593 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    call_tmp_chpl6 = this_chpl21(&LeftActives_chpl, inferredRank_chpl, _ln_chpl, _fn_chpl);
#line 593 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    *(call_tmp_chpl6) = li_chpl;
#line 594 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    call_tmp_chpl7 = this_chpl21(&RightActives_chpl, inferredRank_chpl, _ln_chpl, _fn_chpl);
#line 594 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    *(call_tmp_chpl7) = ri_chpl;
#line 596 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    i_lhs_chpl = &inferredRank_chpl;
#line 596 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    *(i_lhs_chpl) += INT64(1);
#line 597 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    i_lhs_chpl2 = &li_chpl;
#line 597 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    *(i_lhs_chpl2) += INT64(1);
#line 598 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    i_lhs_chpl3 = &ri_chpl;
#line 598 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    *(i_lhs_chpl3) += INT64(1);
#line 599 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    if (li_chpl < INT64(2)) /* ZLINE: 599 /Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl */
#line 599 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    {
#line 599 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      tmp_chpl = (ri_chpl < INT64(3));
#line 599 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    } else /* ZLINE: 599 /Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl */
#line 599 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    {
#line 599 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      tmp_chpl = UINT8(false);
#line 599 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    }
#line 599 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  } while (tmp_chpl);
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(retcopy_chpl + INT64(0)) = *(LeftActives_chpl + INT64(0));
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(retcopy_chpl + INT64(1)) = *(LeftActives_chpl + INT64(1));
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(coerce_tmp_chpl3 + INT64(0)) = *(retcopy_chpl + INT64(0));
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(coerce_tmp_chpl3 + INT64(1)) = *(retcopy_chpl + INT64(1));
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(retcopy_chpl2 + INT64(0)) = *(RightActives_chpl + INT64(0));
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(retcopy_chpl2 + INT64(1)) = *(RightActives_chpl + INT64(1));
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(coerce_tmp_chpl4 + INT64(0)) = *(retcopy_chpl2 + INT64(0));
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(coerce_tmp_chpl4 + INT64(1)) = *(retcopy_chpl2 + INT64(1));
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  this_x0_chpl = &coerce_tmp_chpl3;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  this_x1_chpl = &coerce_tmp_chpl4;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  this_x2_chpl = inferredRank_chpl;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  (&ret_tmp_chpl3)->x0 = this_x0_chpl;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  (&ret_tmp_chpl3)->x1 = this_x1_chpl;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  (&ret_tmp_chpl3)->x2 = this_x2_chpl;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  call_tmp_chpl8 = ret_tmp_chpl3;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  copy_ret_tmp_x0_chpl = (&call_tmp_chpl8)->x0;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  copy_ret_tmp_x1_chpl = (&call_tmp_chpl8)->x1;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  copy_ret_tmp_x2_chpl = (&call_tmp_chpl8)->x2;
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(elt_x0_chpl + INT64(0)) = *(*(copy_ret_tmp_x0_chpl) + INT64(0));
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(elt_x0_chpl + INT64(1)) = *(*(copy_ret_tmp_x0_chpl) + INT64(1));
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(elt_x1_chpl + INT64(0)) = *(*(copy_ret_tmp_x1_chpl) + INT64(0));
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(elt_x1_chpl + INT64(1)) = *(*(copy_ret_tmp_x1_chpl) + INT64(1));
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *((&ret_chpl)->x0 + INT64(0)) = *(elt_x0_chpl + INT64(0));
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *((&ret_chpl)->x0 + INT64(1)) = *(elt_x0_chpl + INT64(1));
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *((&ret_chpl)->x1 + INT64(0)) = *(elt_x1_chpl + INT64(0));
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *((&ret_chpl)->x1 + INT64(1)) = *(elt_x1_chpl + INT64(1));
#line 601 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  (&ret_chpl)->x2 = copy_ret_tmp_x2_chpl;
#line 570 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  *(_retArg_chpl) = ret_chpl;
#line 570 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  return;
#line 570 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
}

#line 582 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
/* DSIUtil.chpl:582 */
#line 582 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
static void advance_chpl23(_tuple_4_star_range_int64_t_both_one_chpl * LeftDims_chpl,
#line 582 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
                           _tuple_4_star_range_int64_t_both_one_chpl * RightDims_chpl,
#line 582 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
                           int64_t * li_chpl,
#line 582 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
                           int64_t * ri_chpl,
#line 582 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
                           int64_t _ln_chpl,
#line 582 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
                           int32_t _fn_chpl) {
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  chpl_bool tmp_chpl;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl6 = NULL;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  int64_t call_tmp_chpl7;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl3;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  chpl_bool tmp_chpl2;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl8 = NULL;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  uint64_t call_tmp_chpl9;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl4;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl10 = NULL;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  uint64_t call_tmp_chpl11;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl5;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl12 = NULL;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  int64_t call_tmp_chpl13;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl6;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  chpl_bool tmp_chpl3;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl14 = NULL;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  uint64_t call_tmp_chpl15;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl7;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl16 = NULL;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  uint64_t call_tmp_chpl17;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl8;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  chpl_bool tmp_chpl4;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl18 = NULL;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  int64_t call_tmp_chpl19;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl9;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  chpl_bool tmp_chpl5;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl20 = NULL;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  uint64_t call_tmp_chpl21;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl10;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl22 = NULL;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  uint64_t call_tmp_chpl23;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl11;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl24 = NULL;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  int64_t call_tmp_chpl25;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl12;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  chpl_bool tmp_chpl6;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl26 = NULL;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  uint64_t call_tmp_chpl27;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl13;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl28 = NULL;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  uint64_t call_tmp_chpl29;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl14;
#line 587 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl30 = NULL;
#line 587 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  uint64_t call_tmp_chpl31;
#line 587 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl15;
#line 587 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl32 = NULL;
#line 587 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  uint64_t call_tmp_chpl33;
#line 587 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl16;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  call_tmp_chpl6 = this_chpl22(LeftDims_chpl, *(li_chpl), _ln_chpl, _fn_chpl);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  coerce_tmp_chpl3 = *(call_tmp_chpl6);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  call_tmp_chpl7 = sizeAs_chpl37(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  if (call_tmp_chpl7 == INT64(1)) /* ZLINE: 584 /Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl */
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  {
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    call_tmp_chpl8 = this_chpl22(LeftDims_chpl, *(li_chpl), _ln_chpl, _fn_chpl);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    coerce_tmp_chpl4 = *(call_tmp_chpl8);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    call_tmp_chpl9 = sizeAs_chpl36(coerce_tmp_chpl4, _ln_chpl, _fn_chpl);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    call_tmp_chpl10 = this_chpl22(RightDims_chpl, *(ri_chpl), _ln_chpl, _fn_chpl);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    coerce_tmp_chpl5 = *(call_tmp_chpl10);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    call_tmp_chpl11 = sizeAs_chpl36(coerce_tmp_chpl5, _ln_chpl, _fn_chpl);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    tmp_chpl2 = (call_tmp_chpl9 != call_tmp_chpl11);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  } else /* ZLINE: 584 /Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl */
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  {
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    tmp_chpl2 = UINT8(false);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  }
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  tmp_chpl = tmp_chpl2;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  while (tmp_chpl) {
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    *(li_chpl) += INT64(1);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    call_tmp_chpl12 = this_chpl22(LeftDims_chpl, *(li_chpl), _ln_chpl, _fn_chpl);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    coerce_tmp_chpl6 = *(call_tmp_chpl12);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    call_tmp_chpl13 = sizeAs_chpl37(coerce_tmp_chpl6, _ln_chpl, _fn_chpl);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    if (call_tmp_chpl13 == INT64(1)) /* ZLINE: 584 /Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl */
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    {
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      call_tmp_chpl14 = this_chpl22(LeftDims_chpl, *(li_chpl), _ln_chpl, _fn_chpl);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      coerce_tmp_chpl7 = *(call_tmp_chpl14);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      call_tmp_chpl15 = sizeAs_chpl36(coerce_tmp_chpl7, _ln_chpl, _fn_chpl);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      call_tmp_chpl16 = this_chpl22(RightDims_chpl, *(ri_chpl), _ln_chpl, _fn_chpl);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      coerce_tmp_chpl8 = *(call_tmp_chpl16);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      call_tmp_chpl17 = sizeAs_chpl36(coerce_tmp_chpl8, _ln_chpl, _fn_chpl);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      tmp_chpl3 = (call_tmp_chpl15 != call_tmp_chpl17);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    } else /* ZLINE: 584 /Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl */
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    {
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      tmp_chpl3 = UINT8(false);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    }
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    tmp_chpl = tmp_chpl3;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  }
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  call_tmp_chpl18 = this_chpl22(RightDims_chpl, *(ri_chpl), _ln_chpl, _fn_chpl);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  coerce_tmp_chpl9 = *(call_tmp_chpl18);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  call_tmp_chpl19 = sizeAs_chpl37(coerce_tmp_chpl9, _ln_chpl, _fn_chpl);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  if (call_tmp_chpl19 == INT64(1)) /* ZLINE: 585 /Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl */
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  {
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    call_tmp_chpl20 = this_chpl22(RightDims_chpl, *(ri_chpl), _ln_chpl, _fn_chpl);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    coerce_tmp_chpl10 = *(call_tmp_chpl20);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    call_tmp_chpl21 = sizeAs_chpl36(coerce_tmp_chpl10, _ln_chpl, _fn_chpl);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    call_tmp_chpl22 = this_chpl22(LeftDims_chpl, *(li_chpl), _ln_chpl, _fn_chpl);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    coerce_tmp_chpl11 = *(call_tmp_chpl22);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    call_tmp_chpl23 = sizeAs_chpl36(coerce_tmp_chpl11, _ln_chpl, _fn_chpl);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    tmp_chpl5 = (call_tmp_chpl21 != call_tmp_chpl23);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  } else /* ZLINE: 585 /Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl */
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  {
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    tmp_chpl5 = UINT8(false);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  }
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  tmp_chpl4 = tmp_chpl5;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  while (tmp_chpl4) {
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    *(ri_chpl) += INT64(1);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    call_tmp_chpl24 = this_chpl22(RightDims_chpl, *(ri_chpl), _ln_chpl, _fn_chpl);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    coerce_tmp_chpl12 = *(call_tmp_chpl24);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    call_tmp_chpl25 = sizeAs_chpl37(coerce_tmp_chpl12, _ln_chpl, _fn_chpl);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    if (call_tmp_chpl25 == INT64(1)) /* ZLINE: 585 /Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl */
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    {
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      call_tmp_chpl26 = this_chpl22(RightDims_chpl, *(ri_chpl), _ln_chpl, _fn_chpl);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      coerce_tmp_chpl13 = *(call_tmp_chpl26);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      call_tmp_chpl27 = sizeAs_chpl36(coerce_tmp_chpl13, _ln_chpl, _fn_chpl);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      call_tmp_chpl28 = this_chpl22(LeftDims_chpl, *(li_chpl), _ln_chpl, _fn_chpl);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      coerce_tmp_chpl14 = *(call_tmp_chpl28);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      call_tmp_chpl29 = sizeAs_chpl36(coerce_tmp_chpl14, _ln_chpl, _fn_chpl);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      tmp_chpl6 = (call_tmp_chpl27 != call_tmp_chpl29);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    } else /* ZLINE: 585 /Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl */
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    {
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      tmp_chpl6 = UINT8(false);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    }
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    tmp_chpl4 = tmp_chpl6;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  }
#line 587 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  call_tmp_chpl30 = this_chpl22(LeftDims_chpl, *(li_chpl), _ln_chpl, _fn_chpl);
#line 587 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  coerce_tmp_chpl15 = *(call_tmp_chpl30);
#line 587 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  call_tmp_chpl31 = sizeAs_chpl36(coerce_tmp_chpl15, _ln_chpl, _fn_chpl);
#line 587 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  call_tmp_chpl32 = this_chpl22(RightDims_chpl, *(ri_chpl), _ln_chpl, _fn_chpl);
#line 587 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  coerce_tmp_chpl16 = *(call_tmp_chpl32);
#line 587 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  call_tmp_chpl33 = sizeAs_chpl36(coerce_tmp_chpl16, _ln_chpl, _fn_chpl);
#line 587 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  assert_chpl((call_tmp_chpl31 == call_tmp_chpl33), _ln_chpl, _fn_chpl);
#line 582 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  return;
#line 582 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
}

#line 582 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
/* DSIUtil.chpl:582 */
#line 582 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
static void advance_chpl24(_tuple_2_star_range_int64_t_both_one_chpl * LeftDims_chpl,
#line 582 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
                           _tuple_2_star_range_int64_t_both_one_chpl * RightDims_chpl,
#line 582 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
                           int64_t * li_chpl,
#line 582 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
                           int64_t * ri_chpl,
#line 582 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
                           int64_t _ln_chpl,
#line 582 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
                           int32_t _fn_chpl) {
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  chpl_bool tmp_chpl;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl6 = NULL;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  int64_t call_tmp_chpl7;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl3;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  chpl_bool tmp_chpl2;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl8 = NULL;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  uint64_t call_tmp_chpl9;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl4;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl10 = NULL;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  uint64_t call_tmp_chpl11;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl5;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl12 = NULL;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  int64_t call_tmp_chpl13;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl6;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  chpl_bool tmp_chpl3;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl14 = NULL;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  uint64_t call_tmp_chpl15;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl7;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl16 = NULL;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  uint64_t call_tmp_chpl17;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl8;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  chpl_bool tmp_chpl4;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl18 = NULL;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  int64_t call_tmp_chpl19;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl9;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  chpl_bool tmp_chpl5;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl20 = NULL;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  uint64_t call_tmp_chpl21;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl10;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl22 = NULL;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  uint64_t call_tmp_chpl23;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl11;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl24 = NULL;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  int64_t call_tmp_chpl25;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl12;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  chpl_bool tmp_chpl6;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl26 = NULL;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  uint64_t call_tmp_chpl27;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl13;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl28 = NULL;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  uint64_t call_tmp_chpl29;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl14;
#line 587 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl30 = NULL;
#line 587 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  uint64_t call_tmp_chpl31;
#line 587 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl15;
#line 587 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl32 = NULL;
#line 587 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  uint64_t call_tmp_chpl33;
#line 587 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl16;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  call_tmp_chpl6 = this_chpl26(LeftDims_chpl, *(li_chpl), _ln_chpl, _fn_chpl);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  coerce_tmp_chpl3 = *(call_tmp_chpl6);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  call_tmp_chpl7 = sizeAs_chpl37(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  if (call_tmp_chpl7 == INT64(1)) /* ZLINE: 584 /Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl */
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  {
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    call_tmp_chpl8 = this_chpl26(LeftDims_chpl, *(li_chpl), _ln_chpl, _fn_chpl);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    coerce_tmp_chpl4 = *(call_tmp_chpl8);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    call_tmp_chpl9 = sizeAs_chpl36(coerce_tmp_chpl4, _ln_chpl, _fn_chpl);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    call_tmp_chpl10 = this_chpl26(RightDims_chpl, *(ri_chpl), _ln_chpl, _fn_chpl);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    coerce_tmp_chpl5 = *(call_tmp_chpl10);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    call_tmp_chpl11 = sizeAs_chpl36(coerce_tmp_chpl5, _ln_chpl, _fn_chpl);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    tmp_chpl2 = (call_tmp_chpl9 != call_tmp_chpl11);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  } else /* ZLINE: 584 /Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl */
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  {
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    tmp_chpl2 = UINT8(false);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  }
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  tmp_chpl = tmp_chpl2;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  while (tmp_chpl) {
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    *(li_chpl) += INT64(1);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    call_tmp_chpl12 = this_chpl26(LeftDims_chpl, *(li_chpl), _ln_chpl, _fn_chpl);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    coerce_tmp_chpl6 = *(call_tmp_chpl12);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    call_tmp_chpl13 = sizeAs_chpl37(coerce_tmp_chpl6, _ln_chpl, _fn_chpl);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    if (call_tmp_chpl13 == INT64(1)) /* ZLINE: 584 /Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl */
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    {
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      call_tmp_chpl14 = this_chpl26(LeftDims_chpl, *(li_chpl), _ln_chpl, _fn_chpl);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      coerce_tmp_chpl7 = *(call_tmp_chpl14);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      call_tmp_chpl15 = sizeAs_chpl36(coerce_tmp_chpl7, _ln_chpl, _fn_chpl);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      call_tmp_chpl16 = this_chpl26(RightDims_chpl, *(ri_chpl), _ln_chpl, _fn_chpl);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      coerce_tmp_chpl8 = *(call_tmp_chpl16);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      call_tmp_chpl17 = sizeAs_chpl36(coerce_tmp_chpl8, _ln_chpl, _fn_chpl);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      tmp_chpl3 = (call_tmp_chpl15 != call_tmp_chpl17);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    } else /* ZLINE: 584 /Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl */
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    {
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      tmp_chpl3 = UINT8(false);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    }
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    tmp_chpl = tmp_chpl3;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  }
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  call_tmp_chpl18 = this_chpl26(RightDims_chpl, *(ri_chpl), _ln_chpl, _fn_chpl);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  coerce_tmp_chpl9 = *(call_tmp_chpl18);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  call_tmp_chpl19 = sizeAs_chpl37(coerce_tmp_chpl9, _ln_chpl, _fn_chpl);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  if (call_tmp_chpl19 == INT64(1)) /* ZLINE: 585 /Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl */
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  {
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    call_tmp_chpl20 = this_chpl26(RightDims_chpl, *(ri_chpl), _ln_chpl, _fn_chpl);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    coerce_tmp_chpl10 = *(call_tmp_chpl20);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    call_tmp_chpl21 = sizeAs_chpl36(coerce_tmp_chpl10, _ln_chpl, _fn_chpl);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    call_tmp_chpl22 = this_chpl26(LeftDims_chpl, *(li_chpl), _ln_chpl, _fn_chpl);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    coerce_tmp_chpl11 = *(call_tmp_chpl22);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    call_tmp_chpl23 = sizeAs_chpl36(coerce_tmp_chpl11, _ln_chpl, _fn_chpl);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    tmp_chpl5 = (call_tmp_chpl21 != call_tmp_chpl23);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  } else /* ZLINE: 585 /Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl */
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  {
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    tmp_chpl5 = UINT8(false);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  }
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  tmp_chpl4 = tmp_chpl5;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  while (tmp_chpl4) {
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    *(ri_chpl) += INT64(1);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    call_tmp_chpl24 = this_chpl26(RightDims_chpl, *(ri_chpl), _ln_chpl, _fn_chpl);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    coerce_tmp_chpl12 = *(call_tmp_chpl24);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    call_tmp_chpl25 = sizeAs_chpl37(coerce_tmp_chpl12, _ln_chpl, _fn_chpl);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    if (call_tmp_chpl25 == INT64(1)) /* ZLINE: 585 /Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl */
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    {
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      call_tmp_chpl26 = this_chpl26(RightDims_chpl, *(ri_chpl), _ln_chpl, _fn_chpl);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      coerce_tmp_chpl13 = *(call_tmp_chpl26);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      call_tmp_chpl27 = sizeAs_chpl36(coerce_tmp_chpl13, _ln_chpl, _fn_chpl);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      call_tmp_chpl28 = this_chpl26(LeftDims_chpl, *(li_chpl), _ln_chpl, _fn_chpl);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      coerce_tmp_chpl14 = *(call_tmp_chpl28);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      call_tmp_chpl29 = sizeAs_chpl36(coerce_tmp_chpl14, _ln_chpl, _fn_chpl);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      tmp_chpl6 = (call_tmp_chpl27 != call_tmp_chpl29);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    } else /* ZLINE: 585 /Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl */
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    {
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      tmp_chpl6 = UINT8(false);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    }
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    tmp_chpl4 = tmp_chpl6;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  }
#line 587 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  call_tmp_chpl30 = this_chpl26(LeftDims_chpl, *(li_chpl), _ln_chpl, _fn_chpl);
#line 587 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  coerce_tmp_chpl15 = *(call_tmp_chpl30);
#line 587 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  call_tmp_chpl31 = sizeAs_chpl36(coerce_tmp_chpl15, _ln_chpl, _fn_chpl);
#line 587 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  call_tmp_chpl32 = this_chpl26(RightDims_chpl, *(ri_chpl), _ln_chpl, _fn_chpl);
#line 587 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  coerce_tmp_chpl16 = *(call_tmp_chpl32);
#line 587 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  call_tmp_chpl33 = sizeAs_chpl36(coerce_tmp_chpl16, _ln_chpl, _fn_chpl);
#line 587 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  assert_chpl((call_tmp_chpl31 == call_tmp_chpl33), _ln_chpl, _fn_chpl);
#line 582 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  return;
#line 582 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
}

#line 582 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
/* DSIUtil.chpl:582 */
#line 582 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
static void advance_chpl22(_tuple_1_star_range_int64_t_both_one_chpl * LeftDims_chpl,
#line 582 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
                           _tuple_1_star_range_int64_t_both_one_chpl * RightDims_chpl,
#line 582 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
                           int64_t * li_chpl,
#line 582 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
                           int64_t * ri_chpl,
#line 582 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
                           int64_t _ln_chpl,
#line 582 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
                           int32_t _fn_chpl) {
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  chpl_bool tmp_chpl;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl6 = NULL;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  int64_t call_tmp_chpl7;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl3;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  chpl_bool tmp_chpl2;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl8 = NULL;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  uint64_t call_tmp_chpl9;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl4;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl10 = NULL;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  uint64_t call_tmp_chpl11;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl5;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl12 = NULL;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  int64_t call_tmp_chpl13;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl6;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  chpl_bool tmp_chpl3;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl14 = NULL;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  uint64_t call_tmp_chpl15;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl7;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl16 = NULL;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  uint64_t call_tmp_chpl17;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl8;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  chpl_bool tmp_chpl4;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl18 = NULL;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  int64_t call_tmp_chpl19;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl9;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  chpl_bool tmp_chpl5;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl20 = NULL;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  uint64_t call_tmp_chpl21;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl10;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl22 = NULL;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  uint64_t call_tmp_chpl23;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl11;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl24 = NULL;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  int64_t call_tmp_chpl25;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl12;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  chpl_bool tmp_chpl6;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl26 = NULL;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  uint64_t call_tmp_chpl27;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl13;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl28 = NULL;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  uint64_t call_tmp_chpl29;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl14;
#line 587 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl30 = NULL;
#line 587 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  uint64_t call_tmp_chpl31;
#line 587 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl15;
#line 587 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl32 = NULL;
#line 587 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  uint64_t call_tmp_chpl33;
#line 587 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl16;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  call_tmp_chpl6 = this_chpl20(LeftDims_chpl, *(li_chpl), _ln_chpl, _fn_chpl);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  coerce_tmp_chpl3 = *(call_tmp_chpl6);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  call_tmp_chpl7 = sizeAs_chpl37(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  if (call_tmp_chpl7 == INT64(1)) /* ZLINE: 584 /Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl */
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  {
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    call_tmp_chpl8 = this_chpl20(LeftDims_chpl, *(li_chpl), _ln_chpl, _fn_chpl);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    coerce_tmp_chpl4 = *(call_tmp_chpl8);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    call_tmp_chpl9 = sizeAs_chpl36(coerce_tmp_chpl4, _ln_chpl, _fn_chpl);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    call_tmp_chpl10 = this_chpl20(RightDims_chpl, *(ri_chpl), _ln_chpl, _fn_chpl);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    coerce_tmp_chpl5 = *(call_tmp_chpl10);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    call_tmp_chpl11 = sizeAs_chpl36(coerce_tmp_chpl5, _ln_chpl, _fn_chpl);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    tmp_chpl2 = (call_tmp_chpl9 != call_tmp_chpl11);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  } else /* ZLINE: 584 /Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl */
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  {
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    tmp_chpl2 = UINT8(false);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  }
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  tmp_chpl = tmp_chpl2;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  while (tmp_chpl) {
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    *(li_chpl) += INT64(1);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    call_tmp_chpl12 = this_chpl20(LeftDims_chpl, *(li_chpl), _ln_chpl, _fn_chpl);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    coerce_tmp_chpl6 = *(call_tmp_chpl12);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    call_tmp_chpl13 = sizeAs_chpl37(coerce_tmp_chpl6, _ln_chpl, _fn_chpl);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    if (call_tmp_chpl13 == INT64(1)) /* ZLINE: 584 /Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl */
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    {
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      call_tmp_chpl14 = this_chpl20(LeftDims_chpl, *(li_chpl), _ln_chpl, _fn_chpl);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      coerce_tmp_chpl7 = *(call_tmp_chpl14);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      call_tmp_chpl15 = sizeAs_chpl36(coerce_tmp_chpl7, _ln_chpl, _fn_chpl);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      call_tmp_chpl16 = this_chpl20(RightDims_chpl, *(ri_chpl), _ln_chpl, _fn_chpl);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      coerce_tmp_chpl8 = *(call_tmp_chpl16);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      call_tmp_chpl17 = sizeAs_chpl36(coerce_tmp_chpl8, _ln_chpl, _fn_chpl);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      tmp_chpl3 = (call_tmp_chpl15 != call_tmp_chpl17);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    } else /* ZLINE: 584 /Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl */
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    {
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      tmp_chpl3 = UINT8(false);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    }
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    tmp_chpl = tmp_chpl3;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  }
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  call_tmp_chpl18 = this_chpl20(RightDims_chpl, *(ri_chpl), _ln_chpl, _fn_chpl);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  coerce_tmp_chpl9 = *(call_tmp_chpl18);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  call_tmp_chpl19 = sizeAs_chpl37(coerce_tmp_chpl9, _ln_chpl, _fn_chpl);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  if (call_tmp_chpl19 == INT64(1)) /* ZLINE: 585 /Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl */
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  {
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    call_tmp_chpl20 = this_chpl20(RightDims_chpl, *(ri_chpl), _ln_chpl, _fn_chpl);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    coerce_tmp_chpl10 = *(call_tmp_chpl20);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    call_tmp_chpl21 = sizeAs_chpl36(coerce_tmp_chpl10, _ln_chpl, _fn_chpl);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    call_tmp_chpl22 = this_chpl20(LeftDims_chpl, *(li_chpl), _ln_chpl, _fn_chpl);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    coerce_tmp_chpl11 = *(call_tmp_chpl22);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    call_tmp_chpl23 = sizeAs_chpl36(coerce_tmp_chpl11, _ln_chpl, _fn_chpl);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    tmp_chpl5 = (call_tmp_chpl21 != call_tmp_chpl23);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  } else /* ZLINE: 585 /Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl */
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  {
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    tmp_chpl5 = UINT8(false);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  }
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  tmp_chpl4 = tmp_chpl5;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  while (tmp_chpl4) {
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    *(ri_chpl) += INT64(1);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    call_tmp_chpl24 = this_chpl20(RightDims_chpl, *(ri_chpl), _ln_chpl, _fn_chpl);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    coerce_tmp_chpl12 = *(call_tmp_chpl24);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    call_tmp_chpl25 = sizeAs_chpl37(coerce_tmp_chpl12, _ln_chpl, _fn_chpl);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    if (call_tmp_chpl25 == INT64(1)) /* ZLINE: 585 /Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl */
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    {
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      call_tmp_chpl26 = this_chpl20(RightDims_chpl, *(ri_chpl), _ln_chpl, _fn_chpl);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      coerce_tmp_chpl13 = *(call_tmp_chpl26);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      call_tmp_chpl27 = sizeAs_chpl36(coerce_tmp_chpl13, _ln_chpl, _fn_chpl);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      call_tmp_chpl28 = this_chpl20(LeftDims_chpl, *(li_chpl), _ln_chpl, _fn_chpl);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      coerce_tmp_chpl14 = *(call_tmp_chpl28);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      call_tmp_chpl29 = sizeAs_chpl36(coerce_tmp_chpl14, _ln_chpl, _fn_chpl);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      tmp_chpl6 = (call_tmp_chpl27 != call_tmp_chpl29);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    } else /* ZLINE: 585 /Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl */
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    {
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      tmp_chpl6 = UINT8(false);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    }
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    tmp_chpl4 = tmp_chpl6;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  }
#line 587 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  call_tmp_chpl30 = this_chpl20(LeftDims_chpl, *(li_chpl), _ln_chpl, _fn_chpl);
#line 587 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  coerce_tmp_chpl15 = *(call_tmp_chpl30);
#line 587 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  call_tmp_chpl31 = sizeAs_chpl36(coerce_tmp_chpl15, _ln_chpl, _fn_chpl);
#line 587 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  call_tmp_chpl32 = this_chpl20(RightDims_chpl, *(ri_chpl), _ln_chpl, _fn_chpl);
#line 587 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  coerce_tmp_chpl16 = *(call_tmp_chpl32);
#line 587 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  call_tmp_chpl33 = sizeAs_chpl36(coerce_tmp_chpl16, _ln_chpl, _fn_chpl);
#line 587 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  assert_chpl((call_tmp_chpl31 == call_tmp_chpl33), _ln_chpl, _fn_chpl);
#line 582 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  return;
#line 582 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
}

#line 582 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
/* DSIUtil.chpl:582 */
#line 582 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
static void advance_chpl26(_tuple_3_star_range_int64_t_both_one_chpl * LeftDims_chpl,
#line 582 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
                           _tuple_3_star_range_int64_t_both_one_chpl * RightDims_chpl,
#line 582 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
                           int64_t * li_chpl,
#line 582 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
                           int64_t * ri_chpl,
#line 582 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
                           int64_t _ln_chpl,
#line 582 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
                           int32_t _fn_chpl) {
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  chpl_bool tmp_chpl;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl6 = NULL;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  int64_t call_tmp_chpl7;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl3;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  chpl_bool tmp_chpl2;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl8 = NULL;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  uint64_t call_tmp_chpl9;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl4;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl10 = NULL;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  uint64_t call_tmp_chpl11;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl5;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl12 = NULL;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  int64_t call_tmp_chpl13;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl6;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  chpl_bool tmp_chpl3;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl14 = NULL;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  uint64_t call_tmp_chpl15;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl7;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl16 = NULL;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  uint64_t call_tmp_chpl17;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl8;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  chpl_bool tmp_chpl4;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl18 = NULL;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  int64_t call_tmp_chpl19;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl9;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  chpl_bool tmp_chpl5;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl20 = NULL;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  uint64_t call_tmp_chpl21;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl10;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl22 = NULL;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  uint64_t call_tmp_chpl23;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl11;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl24 = NULL;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  int64_t call_tmp_chpl25;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl12;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  chpl_bool tmp_chpl6;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl26 = NULL;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  uint64_t call_tmp_chpl27;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl13;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl28 = NULL;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  uint64_t call_tmp_chpl29;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl14;
#line 587 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl30 = NULL;
#line 587 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  uint64_t call_tmp_chpl31;
#line 587 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl15;
#line 587 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl32 = NULL;
#line 587 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  uint64_t call_tmp_chpl33;
#line 587 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl16;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  call_tmp_chpl6 = this_chpl24(LeftDims_chpl, *(li_chpl), _ln_chpl, _fn_chpl);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  coerce_tmp_chpl3 = *(call_tmp_chpl6);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  call_tmp_chpl7 = sizeAs_chpl37(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  if (call_tmp_chpl7 == INT64(1)) /* ZLINE: 584 /Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl */
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  {
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    call_tmp_chpl8 = this_chpl24(LeftDims_chpl, *(li_chpl), _ln_chpl, _fn_chpl);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    coerce_tmp_chpl4 = *(call_tmp_chpl8);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    call_tmp_chpl9 = sizeAs_chpl36(coerce_tmp_chpl4, _ln_chpl, _fn_chpl);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    call_tmp_chpl10 = this_chpl24(RightDims_chpl, *(ri_chpl), _ln_chpl, _fn_chpl);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    coerce_tmp_chpl5 = *(call_tmp_chpl10);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    call_tmp_chpl11 = sizeAs_chpl36(coerce_tmp_chpl5, _ln_chpl, _fn_chpl);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    tmp_chpl2 = (call_tmp_chpl9 != call_tmp_chpl11);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  } else /* ZLINE: 584 /Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl */
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  {
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    tmp_chpl2 = UINT8(false);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  }
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  tmp_chpl = tmp_chpl2;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  while (tmp_chpl) {
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    *(li_chpl) += INT64(1);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    call_tmp_chpl12 = this_chpl24(LeftDims_chpl, *(li_chpl), _ln_chpl, _fn_chpl);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    coerce_tmp_chpl6 = *(call_tmp_chpl12);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    call_tmp_chpl13 = sizeAs_chpl37(coerce_tmp_chpl6, _ln_chpl, _fn_chpl);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    if (call_tmp_chpl13 == INT64(1)) /* ZLINE: 584 /Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl */
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    {
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      call_tmp_chpl14 = this_chpl24(LeftDims_chpl, *(li_chpl), _ln_chpl, _fn_chpl);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      coerce_tmp_chpl7 = *(call_tmp_chpl14);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      call_tmp_chpl15 = sizeAs_chpl36(coerce_tmp_chpl7, _ln_chpl, _fn_chpl);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      call_tmp_chpl16 = this_chpl24(RightDims_chpl, *(ri_chpl), _ln_chpl, _fn_chpl);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      coerce_tmp_chpl8 = *(call_tmp_chpl16);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      call_tmp_chpl17 = sizeAs_chpl36(coerce_tmp_chpl8, _ln_chpl, _fn_chpl);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      tmp_chpl3 = (call_tmp_chpl15 != call_tmp_chpl17);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    } else /* ZLINE: 584 /Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl */
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    {
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      tmp_chpl3 = UINT8(false);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    }
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    tmp_chpl = tmp_chpl3;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  }
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  call_tmp_chpl18 = this_chpl24(RightDims_chpl, *(ri_chpl), _ln_chpl, _fn_chpl);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  coerce_tmp_chpl9 = *(call_tmp_chpl18);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  call_tmp_chpl19 = sizeAs_chpl37(coerce_tmp_chpl9, _ln_chpl, _fn_chpl);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  if (call_tmp_chpl19 == INT64(1)) /* ZLINE: 585 /Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl */
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  {
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    call_tmp_chpl20 = this_chpl24(RightDims_chpl, *(ri_chpl), _ln_chpl, _fn_chpl);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    coerce_tmp_chpl10 = *(call_tmp_chpl20);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    call_tmp_chpl21 = sizeAs_chpl36(coerce_tmp_chpl10, _ln_chpl, _fn_chpl);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    call_tmp_chpl22 = this_chpl24(LeftDims_chpl, *(li_chpl), _ln_chpl, _fn_chpl);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    coerce_tmp_chpl11 = *(call_tmp_chpl22);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    call_tmp_chpl23 = sizeAs_chpl36(coerce_tmp_chpl11, _ln_chpl, _fn_chpl);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    tmp_chpl5 = (call_tmp_chpl21 != call_tmp_chpl23);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  } else /* ZLINE: 585 /Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl */
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  {
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    tmp_chpl5 = UINT8(false);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  }
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  tmp_chpl4 = tmp_chpl5;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  while (tmp_chpl4) {
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    *(ri_chpl) += INT64(1);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    call_tmp_chpl24 = this_chpl24(RightDims_chpl, *(ri_chpl), _ln_chpl, _fn_chpl);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    coerce_tmp_chpl12 = *(call_tmp_chpl24);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    call_tmp_chpl25 = sizeAs_chpl37(coerce_tmp_chpl12, _ln_chpl, _fn_chpl);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    if (call_tmp_chpl25 == INT64(1)) /* ZLINE: 585 /Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl */
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    {
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      call_tmp_chpl26 = this_chpl24(RightDims_chpl, *(ri_chpl), _ln_chpl, _fn_chpl);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      coerce_tmp_chpl13 = *(call_tmp_chpl26);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      call_tmp_chpl27 = sizeAs_chpl36(coerce_tmp_chpl13, _ln_chpl, _fn_chpl);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      call_tmp_chpl28 = this_chpl24(LeftDims_chpl, *(li_chpl), _ln_chpl, _fn_chpl);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      coerce_tmp_chpl14 = *(call_tmp_chpl28);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      call_tmp_chpl29 = sizeAs_chpl36(coerce_tmp_chpl14, _ln_chpl, _fn_chpl);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      tmp_chpl6 = (call_tmp_chpl27 != call_tmp_chpl29);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    } else /* ZLINE: 585 /Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl */
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    {
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      tmp_chpl6 = UINT8(false);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    }
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    tmp_chpl4 = tmp_chpl6;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  }
#line 587 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  call_tmp_chpl30 = this_chpl24(LeftDims_chpl, *(li_chpl), _ln_chpl, _fn_chpl);
#line 587 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  coerce_tmp_chpl15 = *(call_tmp_chpl30);
#line 587 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  call_tmp_chpl31 = sizeAs_chpl36(coerce_tmp_chpl15, _ln_chpl, _fn_chpl);
#line 587 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  call_tmp_chpl32 = this_chpl24(RightDims_chpl, *(ri_chpl), _ln_chpl, _fn_chpl);
#line 587 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  coerce_tmp_chpl16 = *(call_tmp_chpl32);
#line 587 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  call_tmp_chpl33 = sizeAs_chpl36(coerce_tmp_chpl16, _ln_chpl, _fn_chpl);
#line 587 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  assert_chpl((call_tmp_chpl31 == call_tmp_chpl33), _ln_chpl, _fn_chpl);
#line 582 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  return;
#line 582 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
}

#line 582 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
/* DSIUtil.chpl:582 */
#line 582 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
static void advance_chpl20(_tuple_3_star_range_int64_t_both_one_chpl * LeftDims_chpl,
#line 582 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
                           _tuple_4_star_range_int64_t_both_one_chpl * RightDims_chpl,
#line 582 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
                           int64_t * li_chpl,
#line 582 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
                           int64_t * ri_chpl,
#line 582 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
                           int64_t _ln_chpl,
#line 582 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
                           int32_t _fn_chpl) {
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  chpl_bool tmp_chpl;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl6 = NULL;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  int64_t call_tmp_chpl7;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl3;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  chpl_bool tmp_chpl2;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl8 = NULL;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  uint64_t call_tmp_chpl9;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl4;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl10 = NULL;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  uint64_t call_tmp_chpl11;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl5;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl12 = NULL;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  int64_t call_tmp_chpl13;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl6;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  chpl_bool tmp_chpl3;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl14 = NULL;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  uint64_t call_tmp_chpl15;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl7;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl16 = NULL;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  uint64_t call_tmp_chpl17;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl8;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  chpl_bool tmp_chpl4;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl18 = NULL;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  int64_t call_tmp_chpl19;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl9;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  chpl_bool tmp_chpl5;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl20 = NULL;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  uint64_t call_tmp_chpl21;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl10;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl22 = NULL;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  uint64_t call_tmp_chpl23;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl11;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl24 = NULL;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  int64_t call_tmp_chpl25;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl12;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  chpl_bool tmp_chpl6;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl26 = NULL;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  uint64_t call_tmp_chpl27;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl13;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl28 = NULL;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  uint64_t call_tmp_chpl29;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl14;
#line 587 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl30 = NULL;
#line 587 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  uint64_t call_tmp_chpl31;
#line 587 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl15;
#line 587 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl32 = NULL;
#line 587 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  uint64_t call_tmp_chpl33;
#line 587 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl16;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  call_tmp_chpl6 = this_chpl24(LeftDims_chpl, *(li_chpl), _ln_chpl, _fn_chpl);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  coerce_tmp_chpl3 = *(call_tmp_chpl6);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  call_tmp_chpl7 = sizeAs_chpl37(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  if (call_tmp_chpl7 == INT64(1)) /* ZLINE: 584 /Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl */
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  {
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    call_tmp_chpl8 = this_chpl24(LeftDims_chpl, *(li_chpl), _ln_chpl, _fn_chpl);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    coerce_tmp_chpl4 = *(call_tmp_chpl8);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    call_tmp_chpl9 = sizeAs_chpl36(coerce_tmp_chpl4, _ln_chpl, _fn_chpl);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    call_tmp_chpl10 = this_chpl22(RightDims_chpl, *(ri_chpl), _ln_chpl, _fn_chpl);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    coerce_tmp_chpl5 = *(call_tmp_chpl10);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    call_tmp_chpl11 = sizeAs_chpl36(coerce_tmp_chpl5, _ln_chpl, _fn_chpl);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    tmp_chpl2 = (call_tmp_chpl9 != call_tmp_chpl11);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  } else /* ZLINE: 584 /Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl */
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  {
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    tmp_chpl2 = UINT8(false);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  }
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  tmp_chpl = tmp_chpl2;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  while (tmp_chpl) {
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    *(li_chpl) += INT64(1);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    call_tmp_chpl12 = this_chpl24(LeftDims_chpl, *(li_chpl), _ln_chpl, _fn_chpl);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    coerce_tmp_chpl6 = *(call_tmp_chpl12);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    call_tmp_chpl13 = sizeAs_chpl37(coerce_tmp_chpl6, _ln_chpl, _fn_chpl);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    if (call_tmp_chpl13 == INT64(1)) /* ZLINE: 584 /Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl */
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    {
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      call_tmp_chpl14 = this_chpl24(LeftDims_chpl, *(li_chpl), _ln_chpl, _fn_chpl);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      coerce_tmp_chpl7 = *(call_tmp_chpl14);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      call_tmp_chpl15 = sizeAs_chpl36(coerce_tmp_chpl7, _ln_chpl, _fn_chpl);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      call_tmp_chpl16 = this_chpl22(RightDims_chpl, *(ri_chpl), _ln_chpl, _fn_chpl);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      coerce_tmp_chpl8 = *(call_tmp_chpl16);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      call_tmp_chpl17 = sizeAs_chpl36(coerce_tmp_chpl8, _ln_chpl, _fn_chpl);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      tmp_chpl3 = (call_tmp_chpl15 != call_tmp_chpl17);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    } else /* ZLINE: 584 /Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl */
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    {
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      tmp_chpl3 = UINT8(false);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    }
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    tmp_chpl = tmp_chpl3;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  }
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  call_tmp_chpl18 = this_chpl22(RightDims_chpl, *(ri_chpl), _ln_chpl, _fn_chpl);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  coerce_tmp_chpl9 = *(call_tmp_chpl18);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  call_tmp_chpl19 = sizeAs_chpl37(coerce_tmp_chpl9, _ln_chpl, _fn_chpl);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  if (call_tmp_chpl19 == INT64(1)) /* ZLINE: 585 /Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl */
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  {
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    call_tmp_chpl20 = this_chpl22(RightDims_chpl, *(ri_chpl), _ln_chpl, _fn_chpl);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    coerce_tmp_chpl10 = *(call_tmp_chpl20);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    call_tmp_chpl21 = sizeAs_chpl36(coerce_tmp_chpl10, _ln_chpl, _fn_chpl);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    call_tmp_chpl22 = this_chpl24(LeftDims_chpl, *(li_chpl), _ln_chpl, _fn_chpl);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    coerce_tmp_chpl11 = *(call_tmp_chpl22);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    call_tmp_chpl23 = sizeAs_chpl36(coerce_tmp_chpl11, _ln_chpl, _fn_chpl);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    tmp_chpl5 = (call_tmp_chpl21 != call_tmp_chpl23);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  } else /* ZLINE: 585 /Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl */
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  {
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    tmp_chpl5 = UINT8(false);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  }
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  tmp_chpl4 = tmp_chpl5;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  while (tmp_chpl4) {
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    *(ri_chpl) += INT64(1);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    call_tmp_chpl24 = this_chpl22(RightDims_chpl, *(ri_chpl), _ln_chpl, _fn_chpl);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    coerce_tmp_chpl12 = *(call_tmp_chpl24);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    call_tmp_chpl25 = sizeAs_chpl37(coerce_tmp_chpl12, _ln_chpl, _fn_chpl);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    if (call_tmp_chpl25 == INT64(1)) /* ZLINE: 585 /Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl */
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    {
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      call_tmp_chpl26 = this_chpl22(RightDims_chpl, *(ri_chpl), _ln_chpl, _fn_chpl);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      coerce_tmp_chpl13 = *(call_tmp_chpl26);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      call_tmp_chpl27 = sizeAs_chpl36(coerce_tmp_chpl13, _ln_chpl, _fn_chpl);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      call_tmp_chpl28 = this_chpl24(LeftDims_chpl, *(li_chpl), _ln_chpl, _fn_chpl);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      coerce_tmp_chpl14 = *(call_tmp_chpl28);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      call_tmp_chpl29 = sizeAs_chpl36(coerce_tmp_chpl14, _ln_chpl, _fn_chpl);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      tmp_chpl6 = (call_tmp_chpl27 != call_tmp_chpl29);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    } else /* ZLINE: 585 /Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl */
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    {
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      tmp_chpl6 = UINT8(false);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    }
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    tmp_chpl4 = tmp_chpl6;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  }
#line 587 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  call_tmp_chpl30 = this_chpl24(LeftDims_chpl, *(li_chpl), _ln_chpl, _fn_chpl);
#line 587 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  coerce_tmp_chpl15 = *(call_tmp_chpl30);
#line 587 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  call_tmp_chpl31 = sizeAs_chpl36(coerce_tmp_chpl15, _ln_chpl, _fn_chpl);
#line 587 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  call_tmp_chpl32 = this_chpl22(RightDims_chpl, *(ri_chpl), _ln_chpl, _fn_chpl);
#line 587 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  coerce_tmp_chpl16 = *(call_tmp_chpl32);
#line 587 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  call_tmp_chpl33 = sizeAs_chpl36(coerce_tmp_chpl16, _ln_chpl, _fn_chpl);
#line 587 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  assert_chpl((call_tmp_chpl31 == call_tmp_chpl33), _ln_chpl, _fn_chpl);
#line 582 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  return;
#line 582 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
}

#line 582 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
/* DSIUtil.chpl:582 */
#line 582 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
static void advance_chpl25(_tuple_2_star_range_int64_t_both_one_chpl * LeftDims_chpl,
#line 582 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
                           _tuple_1_star_range_int64_t_both_one_chpl * RightDims_chpl,
#line 582 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
                           int64_t * li_chpl,
#line 582 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
                           int64_t * ri_chpl,
#line 582 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
                           int64_t _ln_chpl,
#line 582 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
                           int32_t _fn_chpl) {
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  chpl_bool tmp_chpl;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl6 = NULL;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  int64_t call_tmp_chpl7;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl3;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  chpl_bool tmp_chpl2;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl8 = NULL;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  uint64_t call_tmp_chpl9;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl4;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl10 = NULL;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  uint64_t call_tmp_chpl11;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl5;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl12 = NULL;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  int64_t call_tmp_chpl13;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl6;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  chpl_bool tmp_chpl3;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl14 = NULL;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  uint64_t call_tmp_chpl15;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl7;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl16 = NULL;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  uint64_t call_tmp_chpl17;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl8;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  chpl_bool tmp_chpl4;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl18 = NULL;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  int64_t call_tmp_chpl19;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl9;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  chpl_bool tmp_chpl5;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl20 = NULL;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  uint64_t call_tmp_chpl21;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl10;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl22 = NULL;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  uint64_t call_tmp_chpl23;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl11;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl24 = NULL;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  int64_t call_tmp_chpl25;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl12;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  chpl_bool tmp_chpl6;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl26 = NULL;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  uint64_t call_tmp_chpl27;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl13;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl28 = NULL;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  uint64_t call_tmp_chpl29;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl14;
#line 587 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl30 = NULL;
#line 587 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  uint64_t call_tmp_chpl31;
#line 587 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl15;
#line 587 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl32 = NULL;
#line 587 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  uint64_t call_tmp_chpl33;
#line 587 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl16;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  call_tmp_chpl6 = this_chpl26(LeftDims_chpl, *(li_chpl), _ln_chpl, _fn_chpl);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  coerce_tmp_chpl3 = *(call_tmp_chpl6);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  call_tmp_chpl7 = sizeAs_chpl37(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  if (call_tmp_chpl7 == INT64(1)) /* ZLINE: 584 /Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl */
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  {
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    call_tmp_chpl8 = this_chpl26(LeftDims_chpl, *(li_chpl), _ln_chpl, _fn_chpl);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    coerce_tmp_chpl4 = *(call_tmp_chpl8);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    call_tmp_chpl9 = sizeAs_chpl36(coerce_tmp_chpl4, _ln_chpl, _fn_chpl);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    call_tmp_chpl10 = this_chpl20(RightDims_chpl, *(ri_chpl), _ln_chpl, _fn_chpl);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    coerce_tmp_chpl5 = *(call_tmp_chpl10);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    call_tmp_chpl11 = sizeAs_chpl36(coerce_tmp_chpl5, _ln_chpl, _fn_chpl);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    tmp_chpl2 = (call_tmp_chpl9 != call_tmp_chpl11);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  } else /* ZLINE: 584 /Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl */
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  {
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    tmp_chpl2 = UINT8(false);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  }
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  tmp_chpl = tmp_chpl2;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  while (tmp_chpl) {
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    *(li_chpl) += INT64(1);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    call_tmp_chpl12 = this_chpl26(LeftDims_chpl, *(li_chpl), _ln_chpl, _fn_chpl);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    coerce_tmp_chpl6 = *(call_tmp_chpl12);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    call_tmp_chpl13 = sizeAs_chpl37(coerce_tmp_chpl6, _ln_chpl, _fn_chpl);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    if (call_tmp_chpl13 == INT64(1)) /* ZLINE: 584 /Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl */
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    {
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      call_tmp_chpl14 = this_chpl26(LeftDims_chpl, *(li_chpl), _ln_chpl, _fn_chpl);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      coerce_tmp_chpl7 = *(call_tmp_chpl14);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      call_tmp_chpl15 = sizeAs_chpl36(coerce_tmp_chpl7, _ln_chpl, _fn_chpl);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      call_tmp_chpl16 = this_chpl20(RightDims_chpl, *(ri_chpl), _ln_chpl, _fn_chpl);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      coerce_tmp_chpl8 = *(call_tmp_chpl16);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      call_tmp_chpl17 = sizeAs_chpl36(coerce_tmp_chpl8, _ln_chpl, _fn_chpl);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      tmp_chpl3 = (call_tmp_chpl15 != call_tmp_chpl17);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    } else /* ZLINE: 584 /Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl */
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    {
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      tmp_chpl3 = UINT8(false);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    }
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    tmp_chpl = tmp_chpl3;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  }
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  call_tmp_chpl18 = this_chpl20(RightDims_chpl, *(ri_chpl), _ln_chpl, _fn_chpl);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  coerce_tmp_chpl9 = *(call_tmp_chpl18);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  call_tmp_chpl19 = sizeAs_chpl37(coerce_tmp_chpl9, _ln_chpl, _fn_chpl);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  if (call_tmp_chpl19 == INT64(1)) /* ZLINE: 585 /Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl */
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  {
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    call_tmp_chpl20 = this_chpl20(RightDims_chpl, *(ri_chpl), _ln_chpl, _fn_chpl);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    coerce_tmp_chpl10 = *(call_tmp_chpl20);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    call_tmp_chpl21 = sizeAs_chpl36(coerce_tmp_chpl10, _ln_chpl, _fn_chpl);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    call_tmp_chpl22 = this_chpl26(LeftDims_chpl, *(li_chpl), _ln_chpl, _fn_chpl);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    coerce_tmp_chpl11 = *(call_tmp_chpl22);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    call_tmp_chpl23 = sizeAs_chpl36(coerce_tmp_chpl11, _ln_chpl, _fn_chpl);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    tmp_chpl5 = (call_tmp_chpl21 != call_tmp_chpl23);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  } else /* ZLINE: 585 /Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl */
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  {
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    tmp_chpl5 = UINT8(false);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  }
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  tmp_chpl4 = tmp_chpl5;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  while (tmp_chpl4) {
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    *(ri_chpl) += INT64(1);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    call_tmp_chpl24 = this_chpl20(RightDims_chpl, *(ri_chpl), _ln_chpl, _fn_chpl);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    coerce_tmp_chpl12 = *(call_tmp_chpl24);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    call_tmp_chpl25 = sizeAs_chpl37(coerce_tmp_chpl12, _ln_chpl, _fn_chpl);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    if (call_tmp_chpl25 == INT64(1)) /* ZLINE: 585 /Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl */
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    {
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      call_tmp_chpl26 = this_chpl20(RightDims_chpl, *(ri_chpl), _ln_chpl, _fn_chpl);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      coerce_tmp_chpl13 = *(call_tmp_chpl26);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      call_tmp_chpl27 = sizeAs_chpl36(coerce_tmp_chpl13, _ln_chpl, _fn_chpl);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      call_tmp_chpl28 = this_chpl26(LeftDims_chpl, *(li_chpl), _ln_chpl, _fn_chpl);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      coerce_tmp_chpl14 = *(call_tmp_chpl28);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      call_tmp_chpl29 = sizeAs_chpl36(coerce_tmp_chpl14, _ln_chpl, _fn_chpl);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      tmp_chpl6 = (call_tmp_chpl27 != call_tmp_chpl29);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    } else /* ZLINE: 585 /Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl */
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    {
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      tmp_chpl6 = UINT8(false);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    }
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    tmp_chpl4 = tmp_chpl6;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  }
#line 587 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  call_tmp_chpl30 = this_chpl26(LeftDims_chpl, *(li_chpl), _ln_chpl, _fn_chpl);
#line 587 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  coerce_tmp_chpl15 = *(call_tmp_chpl30);
#line 587 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  call_tmp_chpl31 = sizeAs_chpl36(coerce_tmp_chpl15, _ln_chpl, _fn_chpl);
#line 587 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  call_tmp_chpl32 = this_chpl20(RightDims_chpl, *(ri_chpl), _ln_chpl, _fn_chpl);
#line 587 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  coerce_tmp_chpl16 = *(call_tmp_chpl32);
#line 587 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  call_tmp_chpl33 = sizeAs_chpl36(coerce_tmp_chpl16, _ln_chpl, _fn_chpl);
#line 587 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  assert_chpl((call_tmp_chpl31 == call_tmp_chpl33), _ln_chpl, _fn_chpl);
#line 582 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  return;
#line 582 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
}

#line 582 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
/* DSIUtil.chpl:582 */
#line 582 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
static void advance_chpl21(_tuple_2_star_range_int64_t_both_one_chpl * LeftDims_chpl,
#line 582 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
                           _tuple_3_star_range_int64_t_both_one_chpl * RightDims_chpl,
#line 582 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
                           int64_t * li_chpl,
#line 582 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
                           int64_t * ri_chpl,
#line 582 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
                           int64_t _ln_chpl,
#line 582 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
                           int32_t _fn_chpl) {
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  chpl_bool tmp_chpl;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl6 = NULL;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  int64_t call_tmp_chpl7;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl3;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  chpl_bool tmp_chpl2;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl8 = NULL;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  uint64_t call_tmp_chpl9;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl4;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl10 = NULL;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  uint64_t call_tmp_chpl11;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl5;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl12 = NULL;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  int64_t call_tmp_chpl13;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl6;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  chpl_bool tmp_chpl3;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl14 = NULL;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  uint64_t call_tmp_chpl15;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl7;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl16 = NULL;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  uint64_t call_tmp_chpl17;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl8;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  chpl_bool tmp_chpl4;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl18 = NULL;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  int64_t call_tmp_chpl19;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl9;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  chpl_bool tmp_chpl5;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl20 = NULL;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  uint64_t call_tmp_chpl21;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl10;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl22 = NULL;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  uint64_t call_tmp_chpl23;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl11;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl24 = NULL;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  int64_t call_tmp_chpl25;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl12;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  chpl_bool tmp_chpl6;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl26 = NULL;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  uint64_t call_tmp_chpl27;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl13;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl28 = NULL;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  uint64_t call_tmp_chpl29;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl14;
#line 587 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl30 = NULL;
#line 587 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  uint64_t call_tmp_chpl31;
#line 587 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl15;
#line 587 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl32 = NULL;
#line 587 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  uint64_t call_tmp_chpl33;
#line 587 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl16;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  call_tmp_chpl6 = this_chpl26(LeftDims_chpl, *(li_chpl), _ln_chpl, _fn_chpl);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  coerce_tmp_chpl3 = *(call_tmp_chpl6);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  call_tmp_chpl7 = sizeAs_chpl37(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  if (call_tmp_chpl7 == INT64(1)) /* ZLINE: 584 /Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl */
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  {
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    call_tmp_chpl8 = this_chpl26(LeftDims_chpl, *(li_chpl), _ln_chpl, _fn_chpl);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    coerce_tmp_chpl4 = *(call_tmp_chpl8);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    call_tmp_chpl9 = sizeAs_chpl36(coerce_tmp_chpl4, _ln_chpl, _fn_chpl);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    call_tmp_chpl10 = this_chpl24(RightDims_chpl, *(ri_chpl), _ln_chpl, _fn_chpl);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    coerce_tmp_chpl5 = *(call_tmp_chpl10);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    call_tmp_chpl11 = sizeAs_chpl36(coerce_tmp_chpl5, _ln_chpl, _fn_chpl);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    tmp_chpl2 = (call_tmp_chpl9 != call_tmp_chpl11);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  } else /* ZLINE: 584 /Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl */
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  {
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    tmp_chpl2 = UINT8(false);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  }
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  tmp_chpl = tmp_chpl2;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  while (tmp_chpl) {
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    *(li_chpl) += INT64(1);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    call_tmp_chpl12 = this_chpl26(LeftDims_chpl, *(li_chpl), _ln_chpl, _fn_chpl);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    coerce_tmp_chpl6 = *(call_tmp_chpl12);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    call_tmp_chpl13 = sizeAs_chpl37(coerce_tmp_chpl6, _ln_chpl, _fn_chpl);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    if (call_tmp_chpl13 == INT64(1)) /* ZLINE: 584 /Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl */
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    {
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      call_tmp_chpl14 = this_chpl26(LeftDims_chpl, *(li_chpl), _ln_chpl, _fn_chpl);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      coerce_tmp_chpl7 = *(call_tmp_chpl14);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      call_tmp_chpl15 = sizeAs_chpl36(coerce_tmp_chpl7, _ln_chpl, _fn_chpl);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      call_tmp_chpl16 = this_chpl24(RightDims_chpl, *(ri_chpl), _ln_chpl, _fn_chpl);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      coerce_tmp_chpl8 = *(call_tmp_chpl16);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      call_tmp_chpl17 = sizeAs_chpl36(coerce_tmp_chpl8, _ln_chpl, _fn_chpl);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      tmp_chpl3 = (call_tmp_chpl15 != call_tmp_chpl17);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    } else /* ZLINE: 584 /Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl */
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    {
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      tmp_chpl3 = UINT8(false);
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    }
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    tmp_chpl = tmp_chpl3;
#line 584 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  }
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  call_tmp_chpl18 = this_chpl24(RightDims_chpl, *(ri_chpl), _ln_chpl, _fn_chpl);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  coerce_tmp_chpl9 = *(call_tmp_chpl18);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  call_tmp_chpl19 = sizeAs_chpl37(coerce_tmp_chpl9, _ln_chpl, _fn_chpl);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  if (call_tmp_chpl19 == INT64(1)) /* ZLINE: 585 /Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl */
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  {
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    call_tmp_chpl20 = this_chpl24(RightDims_chpl, *(ri_chpl), _ln_chpl, _fn_chpl);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    coerce_tmp_chpl10 = *(call_tmp_chpl20);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    call_tmp_chpl21 = sizeAs_chpl36(coerce_tmp_chpl10, _ln_chpl, _fn_chpl);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    call_tmp_chpl22 = this_chpl26(LeftDims_chpl, *(li_chpl), _ln_chpl, _fn_chpl);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    coerce_tmp_chpl11 = *(call_tmp_chpl22);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    call_tmp_chpl23 = sizeAs_chpl36(coerce_tmp_chpl11, _ln_chpl, _fn_chpl);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    tmp_chpl5 = (call_tmp_chpl21 != call_tmp_chpl23);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  } else /* ZLINE: 585 /Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl */
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  {
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    tmp_chpl5 = UINT8(false);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  }
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  tmp_chpl4 = tmp_chpl5;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  while (tmp_chpl4) {
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    *(ri_chpl) += INT64(1);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    call_tmp_chpl24 = this_chpl24(RightDims_chpl, *(ri_chpl), _ln_chpl, _fn_chpl);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    coerce_tmp_chpl12 = *(call_tmp_chpl24);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    call_tmp_chpl25 = sizeAs_chpl37(coerce_tmp_chpl12, _ln_chpl, _fn_chpl);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    if (call_tmp_chpl25 == INT64(1)) /* ZLINE: 585 /Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl */
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    {
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      call_tmp_chpl26 = this_chpl24(RightDims_chpl, *(ri_chpl), _ln_chpl, _fn_chpl);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      coerce_tmp_chpl13 = *(call_tmp_chpl26);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      call_tmp_chpl27 = sizeAs_chpl36(coerce_tmp_chpl13, _ln_chpl, _fn_chpl);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      call_tmp_chpl28 = this_chpl26(LeftDims_chpl, *(li_chpl), _ln_chpl, _fn_chpl);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      coerce_tmp_chpl14 = *(call_tmp_chpl28);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      call_tmp_chpl29 = sizeAs_chpl36(coerce_tmp_chpl14, _ln_chpl, _fn_chpl);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      tmp_chpl6 = (call_tmp_chpl27 != call_tmp_chpl29);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    } else /* ZLINE: 585 /Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl */
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    {
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
      tmp_chpl6 = UINT8(false);
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    }
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
    tmp_chpl4 = tmp_chpl6;
#line 585 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  }
#line 587 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  call_tmp_chpl30 = this_chpl26(LeftDims_chpl, *(li_chpl), _ln_chpl, _fn_chpl);
#line 587 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  coerce_tmp_chpl15 = *(call_tmp_chpl30);
#line 587 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  call_tmp_chpl31 = sizeAs_chpl36(coerce_tmp_chpl15, _ln_chpl, _fn_chpl);
#line 587 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  call_tmp_chpl32 = this_chpl24(RightDims_chpl, *(ri_chpl), _ln_chpl, _fn_chpl);
#line 587 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  coerce_tmp_chpl16 = *(call_tmp_chpl32);
#line 587 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  call_tmp_chpl33 = sizeAs_chpl36(coerce_tmp_chpl16, _ln_chpl, _fn_chpl);
#line 587 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  assert_chpl((call_tmp_chpl31 == call_tmp_chpl33), _ln_chpl, _fn_chpl);
#line 582 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
  return;
#line 582 "/Users/iainmoncrief/Documents/chapel/modules/dists/DSIUtil.chpl"
}

