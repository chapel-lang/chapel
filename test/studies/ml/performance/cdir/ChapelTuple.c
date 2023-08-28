#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
/* ChapelTuple.chpl:23 */
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
static void chpl__init_ChapelTuple(int64_t _ln_chpl,
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
                                   int32_t _fn_chpl) {
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  _ref_int32_t_chpl refIndentLevel_chpl = NULL;
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  if (chpl__init_ChapelTuple_p) /* ZLINE: 23 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl */
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  {
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
    goto _exit_chpl__init_ChapelTuple_chpl;
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  }
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  printModuleInit_chpl("%*s\n", "ChapelTuple", INT64(11), _ln_chpl, _fn_chpl);
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  refIndentLevel_chpl = &moduleInitLevel_chpl;
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  *(refIndentLevel_chpl) += INT64(1);
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  chpl__init_ChapelTuple_p = UINT8(true);
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  {
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
    chpl__init_DSIUtil(_ln_chpl, _fn_chpl);
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  }
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  *(refIndentLevel_chpl) -= INT64(1);
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  _exit_chpl__init_ChapelTuple_chpl:;
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  return;
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
}

#line 166 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
/* ChapelTuple.chpl:166 */
#line 166 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
static int64_t * this_chpl19(_tuple_13_star_int64_t_chpl * this_chpl31,
#line 166 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
                             int64_t i_chpl,
#line 166 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
                             int64_t _ln_chpl,
#line 166 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
                             int32_t _fn_chpl) {
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  chpl_bool tmp_chpl;
#line 172 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  _ref_int64_t_chpl call_tmp_chpl6 = NULL;
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  if (i_chpl < INT64(0)) /* ZLINE: 170 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl */
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  {
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
    tmp_chpl = UINT8(true);
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  } else /* ZLINE: 170 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl */
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  {
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
    tmp_chpl = (i_chpl > INT64(12));
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  }
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  if (tmp_chpl) /* ZLINE: 170 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl */
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  {
#line 171 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
    halt_chpl5(&_str_literal_418_chpl, i_chpl, _ln_chpl, _fn_chpl);
#line 171 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  }
#line 172 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  call_tmp_chpl6 = (*(this_chpl31) + i_chpl);
#line 166 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  return call_tmp_chpl6;
#line 166 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
}

#line 166 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
/* ChapelTuple.chpl:166 */
#line 166 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
static range_int64_t_both_one_chpl * this_chpl22(_tuple_4_star_range_int64_t_both_one_chpl * this_chpl31,
#line 166 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
                                                 int64_t i_chpl,
#line 166 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
                                                 int64_t _ln_chpl,
#line 166 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
                                                 int32_t _fn_chpl) {
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  chpl_bool tmp_chpl;
#line 172 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl6 = NULL;
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  if (i_chpl < INT64(0)) /* ZLINE: 170 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl */
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  {
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
    tmp_chpl = UINT8(true);
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  } else /* ZLINE: 170 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl */
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  {
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
    tmp_chpl = (i_chpl > INT64(3));
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  }
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  if (tmp_chpl) /* ZLINE: 170 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl */
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  {
#line 171 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
    halt_chpl5(&_str_literal_418_chpl, i_chpl, _ln_chpl, _fn_chpl);
#line 171 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  }
#line 172 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  call_tmp_chpl6 = (*(this_chpl31) + i_chpl);
#line 166 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  return call_tmp_chpl6;
#line 166 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
}

#line 166 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
/* ChapelTuple.chpl:166 */
#line 166 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
static int64_t * this_chpl28(_tuple_4_star_int64_t_chpl * this_chpl31,
#line 166 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
                             int64_t i_chpl,
#line 166 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
                             int64_t _ln_chpl,
#line 166 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
                             int32_t _fn_chpl) {
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  chpl_bool tmp_chpl;
#line 172 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  _ref_int64_t_chpl call_tmp_chpl6 = NULL;
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  if (i_chpl < INT64(0)) /* ZLINE: 170 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl */
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  {
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
    tmp_chpl = UINT8(true);
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  } else /* ZLINE: 170 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl */
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  {
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
    tmp_chpl = (i_chpl > INT64(3));
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  }
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  if (tmp_chpl) /* ZLINE: 170 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl */
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  {
#line 171 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
    halt_chpl5(&_str_literal_418_chpl, i_chpl, _ln_chpl, _fn_chpl);
#line 171 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  }
#line 172 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  call_tmp_chpl6 = (*(this_chpl31) + i_chpl);
#line 166 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  return call_tmp_chpl6;
#line 166 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
}

#line 166 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
/* ChapelTuple.chpl:166 */
#line 166 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
static range_int64_t_both_one_chpl * this_chpl26(_tuple_2_star_range_int64_t_both_one_chpl * this_chpl31,
#line 166 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
                                                 int64_t i_chpl,
#line 166 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
                                                 int64_t _ln_chpl,
#line 166 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
                                                 int32_t _fn_chpl) {
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  chpl_bool tmp_chpl;
#line 172 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl6 = NULL;
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  if (i_chpl < INT64(0)) /* ZLINE: 170 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl */
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  {
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
    tmp_chpl = UINT8(true);
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  } else /* ZLINE: 170 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl */
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  {
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
    tmp_chpl = (i_chpl > INT64(1));
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  }
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  if (tmp_chpl) /* ZLINE: 170 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl */
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  {
#line 171 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
    halt_chpl5(&_str_literal_418_chpl, i_chpl, _ln_chpl, _fn_chpl);
#line 171 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  }
#line 172 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  call_tmp_chpl6 = (*(this_chpl31) + i_chpl);
#line 166 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  return call_tmp_chpl6;
#line 166 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
}

#line 166 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
/* ChapelTuple.chpl:166 */
#line 166 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
static range_int64_t_both_one_chpl * this_chpl20(_tuple_1_star_range_int64_t_both_one_chpl * this_chpl31,
#line 166 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
                                                 int64_t i_chpl,
#line 166 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
                                                 int64_t _ln_chpl,
#line 166 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
                                                 int32_t _fn_chpl) {
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  chpl_bool tmp_chpl;
#line 172 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl6 = NULL;
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  if (i_chpl < INT64(0)) /* ZLINE: 170 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl */
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  {
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
    tmp_chpl = UINT8(true);
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  } else /* ZLINE: 170 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl */
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  {
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
    tmp_chpl = (i_chpl > INT64(0));
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  }
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  if (tmp_chpl) /* ZLINE: 170 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl */
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  {
#line 171 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
    halt_chpl5(&_str_literal_418_chpl, i_chpl, _ln_chpl, _fn_chpl);
#line 171 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  }
#line 172 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  call_tmp_chpl6 = (*(this_chpl31) + i_chpl);
#line 166 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  return call_tmp_chpl6;
#line 166 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
}

#line 166 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
/* ChapelTuple.chpl:166 */
#line 166 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
static int64_t * this_chpl21(_tuple_2_star_int64_t_chpl * this_chpl31,
#line 166 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
                             int64_t i_chpl,
#line 166 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
                             int64_t _ln_chpl,
#line 166 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
                             int32_t _fn_chpl) {
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  chpl_bool tmp_chpl;
#line 172 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  _ref_int64_t_chpl call_tmp_chpl6 = NULL;
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  if (i_chpl < INT64(0)) /* ZLINE: 170 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl */
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  {
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
    tmp_chpl = UINT8(true);
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  } else /* ZLINE: 170 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl */
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  {
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
    tmp_chpl = (i_chpl > INT64(1));
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  }
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  if (tmp_chpl) /* ZLINE: 170 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl */
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  {
#line 171 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
    halt_chpl5(&_str_literal_418_chpl, i_chpl, _ln_chpl, _fn_chpl);
#line 171 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  }
#line 172 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  call_tmp_chpl6 = (*(this_chpl31) + i_chpl);
#line 166 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  return call_tmp_chpl6;
#line 166 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
}

#line 166 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
/* ChapelTuple.chpl:166 */
#line 166 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
static int64_t * this_chpl25(_tuple_1_star_int64_t_chpl * this_chpl31,
#line 166 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
                             int64_t i_chpl,
#line 166 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
                             int64_t _ln_chpl,
#line 166 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
                             int32_t _fn_chpl) {
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  chpl_bool tmp_chpl;
#line 172 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  _ref_int64_t_chpl call_tmp_chpl6 = NULL;
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  if (i_chpl < INT64(0)) /* ZLINE: 170 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl */
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  {
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
    tmp_chpl = UINT8(true);
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  } else /* ZLINE: 170 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl */
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  {
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
    tmp_chpl = (i_chpl > INT64(0));
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  }
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  if (tmp_chpl) /* ZLINE: 170 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl */
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  {
#line 171 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
    halt_chpl5(&_str_literal_418_chpl, i_chpl, _ln_chpl, _fn_chpl);
#line 171 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  }
#line 172 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  call_tmp_chpl6 = (*(this_chpl31) + i_chpl);
#line 166 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  return call_tmp_chpl6;
#line 166 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
}

#line 166 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
/* ChapelTuple.chpl:166 */
#line 166 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
static int64_t * this_chpl23(_tuple_3_star_int64_t_chpl * this_chpl31,
#line 166 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
                             int64_t i_chpl,
#line 166 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
                             int64_t _ln_chpl,
#line 166 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
                             int32_t _fn_chpl) {
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  chpl_bool tmp_chpl;
#line 172 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  _ref_int64_t_chpl call_tmp_chpl6 = NULL;
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  if (i_chpl < INT64(0)) /* ZLINE: 170 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl */
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  {
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
    tmp_chpl = UINT8(true);
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  } else /* ZLINE: 170 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl */
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  {
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
    tmp_chpl = (i_chpl > INT64(2));
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  }
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  if (tmp_chpl) /* ZLINE: 170 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl */
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  {
#line 171 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
    halt_chpl5(&_str_literal_418_chpl, i_chpl, _ln_chpl, _fn_chpl);
#line 171 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  }
#line 172 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  call_tmp_chpl6 = (*(this_chpl31) + i_chpl);
#line 166 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  return call_tmp_chpl6;
#line 166 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
}

#line 166 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
/* ChapelTuple.chpl:166 */
#line 166 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
static range_int64_t_both_one_chpl * this_chpl24(_tuple_3_star_range_int64_t_both_one_chpl * this_chpl31,
#line 166 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
                                                 int64_t i_chpl,
#line 166 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
                                                 int64_t _ln_chpl,
#line 166 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
                                                 int32_t _fn_chpl) {
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  chpl_bool tmp_chpl;
#line 172 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl6 = NULL;
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  if (i_chpl < INT64(0)) /* ZLINE: 170 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl */
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  {
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
    tmp_chpl = UINT8(true);
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  } else /* ZLINE: 170 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl */
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  {
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
    tmp_chpl = (i_chpl > INT64(2));
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  }
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  if (tmp_chpl) /* ZLINE: 170 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl */
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  {
#line 171 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
    halt_chpl5(&_str_literal_418_chpl, i_chpl, _ln_chpl, _fn_chpl);
#line 171 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  }
#line 172 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  call_tmp_chpl6 = (*(this_chpl31) + i_chpl);
#line 166 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  return call_tmp_chpl6;
#line 166 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
}

#line 166 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
/* ChapelTuple.chpl:166 */
#line 166 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
static pcg_setseq_64_xsh_rr_32_rng_chpl * this_chpl29(_tuple_2_star_pcg_setseq_64_xsh_rr_32_rng_chpl * this_chpl31,
#line 166 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
                                                      int64_t i_chpl,
#line 166 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
                                                      int64_t _ln_chpl,
#line 166 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
                                                      int32_t _fn_chpl) {
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  chpl_bool tmp_chpl;
#line 172 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  _ref_pcg_setseq_64_xsh_rr_32_rng_chpl call_tmp_chpl6 = NULL;
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  if (i_chpl < INT64(0)) /* ZLINE: 170 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl */
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  {
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
    tmp_chpl = UINT8(true);
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  } else /* ZLINE: 170 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl */
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  {
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
    tmp_chpl = (i_chpl > INT64(1));
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  }
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  if (tmp_chpl) /* ZLINE: 170 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl */
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  {
#line 171 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
    halt_chpl5(&_str_literal_418_chpl, i_chpl, _ln_chpl, _fn_chpl);
#line 171 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  }
#line 172 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  call_tmp_chpl6 = (*(this_chpl31) + i_chpl);
#line 166 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  return call_tmp_chpl6;
#line 166 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
}

#line 166 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
/* ChapelTuple.chpl:166 */
#line 166 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
static Tensor_3__real64_chpl * this_chpl27(_tuple_1_star_Tensor_3__real64_chpl * this_chpl31,
#line 166 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
                                           int64_t i_chpl,
#line 166 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
                                           int64_t _ln_chpl,
#line 166 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
                                           int32_t _fn_chpl) {
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  chpl_bool tmp_chpl;
#line 172 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  _ref_Tensor_3__real64_chpl call_tmp_chpl6 = NULL;
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  if (i_chpl < INT64(0)) /* ZLINE: 170 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl */
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  {
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
    tmp_chpl = UINT8(true);
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  } else /* ZLINE: 170 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl */
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  {
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
    tmp_chpl = (i_chpl > INT64(0));
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  }
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  if (tmp_chpl) /* ZLINE: 170 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl */
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  {
#line 171 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
    halt_chpl5(&_str_literal_418_chpl, i_chpl, _ln_chpl, _fn_chpl);
#line 171 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  }
#line 172 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  call_tmp_chpl6 = (*(this_chpl31) + i_chpl);
#line 166 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  return call_tmp_chpl6;
#line 166 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
}

#line 264 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
/* ChapelTuple.chpl:264 */
#line 264 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
static void indices_chpl3(_tuple_1_star_range_int64_t_both_one_chpl * this_chpl31,
#line 264 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
                          range_int64_t_both_one_chpl * _retArg_chpl) {
#line 265 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl6;
#line 265 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl;
#line 265 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  chpl_build_bounded_range2(&ret_tmp_chpl);
#line 265 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  call_tmp_chpl6 = ret_tmp_chpl;
#line 264 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  *(_retArg_chpl) = call_tmp_chpl6;
#line 264 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  return;
#line 264 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
}

#line 264 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
/* ChapelTuple.chpl:264 */
#line 264 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
static void indices_chpl4(_tuple_4_star_range_int64_t_both_one_chpl * this_chpl31,
#line 264 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
                          range_int64_t_both_one_chpl * _retArg_chpl) {
#line 265 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl6;
#line 265 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl;
#line 265 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  chpl_build_bounded_range(&ret_tmp_chpl);
#line 265 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  call_tmp_chpl6 = ret_tmp_chpl;
#line 264 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  *(_retArg_chpl) = call_tmp_chpl6;
#line 264 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  return;
#line 264 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
}

#line 264 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
/* ChapelTuple.chpl:264 */
#line 264 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
static void indices_chpl(_tuple_2_star_range_int64_t_both_one_chpl * this_chpl31,
#line 264 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
                         range_int64_t_both_one_chpl * _retArg_chpl) {
#line 265 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl6;
#line 265 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl;
#line 265 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  chpl_build_bounded_range3(&ret_tmp_chpl);
#line 265 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  call_tmp_chpl6 = ret_tmp_chpl;
#line 264 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  *(_retArg_chpl) = call_tmp_chpl6;
#line 264 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  return;
#line 264 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
}

#line 264 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
/* ChapelTuple.chpl:264 */
#line 264 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
static void indices_chpl2(_tuple_3_star_range_int64_t_both_one_chpl * this_chpl31,
#line 264 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
                          range_int64_t_both_one_chpl * _retArg_chpl) {
#line 265 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl6;
#line 265 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl;
#line 265 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  chpl_build_bounded_range4(&ret_tmp_chpl);
#line 265 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  call_tmp_chpl6 = ret_tmp_chpl;
#line 264 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  *(_retArg_chpl) = call_tmp_chpl6;
#line 264 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
  return;
#line 264 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTuple.chpl"
}

