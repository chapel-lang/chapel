#line 1803 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
/* Random.chpl:1803 */
#line 1803 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
static void chpl__init_PCGRandomLib(int64_t _ln_chpl,
#line 1803 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
                                    int32_t _fn_chpl) {
#line 1803 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  _ref_int32_t_chpl refIndentLevel_chpl = NULL;
#line 1803 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  if (chpl__init_PCGRandomLib_p) /* ZLINE: 1803 /Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl */
#line 1803 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  {
#line 1803 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    goto _exit_chpl__init_PCGRandomLib_chpl;
#line 1803 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  }
#line 1803 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  printModuleInit_chpl("%*s\n", "PCGRandomLib", INT64(12), _ln_chpl, _fn_chpl);
#line 1803 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  refIndentLevel_chpl = &moduleInitLevel_chpl;
#line 1803 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  *(refIndentLevel_chpl) += INT64(1);
#line 1803 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  chpl__init_PCGRandomLib_p = UINT8(true);
#line 1803 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  {
#line 1803 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    chpl__init_Random(_ln_chpl, _fn_chpl);
#line 1803 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  }
#line 1803 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  *(refIndentLevel_chpl) -= INT64(1);
#line 1803 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  _exit_chpl__init_PCGRandomLib_chpl:;
#line 1803 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  return;
#line 1803 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
}

#line 1920 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
/* Random.chpl:1920 */
#line 1920 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
static void init_chpl222(pcg_setseq_64_xsh_rr_32_rng_chpl * this_chpl31,
#line 1920 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
                         uint64_t state_chpl) {
#line 1922 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  (this_chpl31)->state = state_chpl;
#line 1920 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  return;
#line 1920 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
}

#line 2544 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
/* Random.chpl:2544 */
#line 2544 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
static uint64_t pcg_advance_lcg_chpl(uint64_t state_chpl,
#line 2544 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
                                     uint64_t delta_chpl,
#line 2544 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
                                     uint64_t cur_mult_chpl,
#line 2544 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
                                     uint64_t cur_plus_chpl,
#line 2544 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
                                     int64_t _ln_chpl,
#line 2544 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
                                     int32_t _fn_chpl) {
#line 2550 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  uint64_t acc_mult_chpl;
#line 2551 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  uint64_t acc_plus_chpl;
#line 2552 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  chpl_bool tmp_chpl;
#line 2552 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  chpl_bool tmp_chpl2;
#line 2552 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  chpl_bool call_tmp_chpl6;
#line 2559 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  chpl_bool call_tmp_chpl7;
#line 2553 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  chpl_bool tmp_chpl3;
#line 2554 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  _ref_uint64_t_chpl i_lhs_chpl = NULL;
#line 2558 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  _ref_uint64_t_chpl i_lhs_chpl2 = NULL;
#line 2559 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  uint64_t coerce_tmp_chpl3;
#line 2559 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  _ref_uint64_t_chpl i_lhs_chpl3 = NULL;
#line 2559 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  chpl_bool tmp_chpl4;
#line 2552 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  chpl_bool tmp_chpl5;
#line 2561 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  uint64_t call_tmp_chpl8;
#line 2550 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  acc_mult_chpl = UINT64(1);
#line 2551 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  acc_plus_chpl = UINT64(0);
#line 2552 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  if (INT64(0) < INT64(0)) /* ZLINE: 2552 /Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl */
#line 2552 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  {
#line 2552 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    tmp_chpl2 = UINT8(true);
#line 2552 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  } else /* ZLINE: 2552 /Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl */
#line 2552 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  {
#line 2552 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    tmp_chpl2 = (delta_chpl > ((uint64_t)(INT64(0))));
#line 2552 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  }
#line 2552 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  tmp_chpl = tmp_chpl2;
#line 2552 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  call_tmp_chpl6 = (INT64(0) < INT64(0));
#line 2559 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  call_tmp_chpl7 = (! (INT64(0) < INT64(0)));
#line 2552 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  while (tmp_chpl) {
#line 2553 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    if (INT64(0) < INT64(0)) /* ZLINE: 2553 /Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl */
#line 2553 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    {
#line 2553 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
      tmp_chpl3 = UINT8(true);
#line 2553 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    } else /* ZLINE: 2553 /Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl */
#line 2553 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    {
#line 2553 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
      tmp_chpl3 = ((delta_chpl & ((uint64_t)(INT64(1)))) != ((uint64_t)(INT64(0))));
#line 2553 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    }
#line 2553 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    if (tmp_chpl3) /* ZLINE: 2553 /Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl */
#line 2553 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    {
#line 2554 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
      i_lhs_chpl = &acc_mult_chpl;
#line 2554 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
      *(i_lhs_chpl) *= cur_mult_chpl;
#line 2555 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
      acc_plus_chpl = ((uint64_t)((((uint64_t)((acc_plus_chpl * cur_mult_chpl))) + cur_plus_chpl)));
#line 2555 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    }
#line 2557 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    cur_plus_chpl = ((uint64_t)((((uint64_t)((cur_mult_chpl + UINT64(1)))) * cur_plus_chpl)));
#line 2558 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    i_lhs_chpl2 = &cur_mult_chpl;
#line 2558 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    *(i_lhs_chpl2) *= cur_mult_chpl;
#line 2559 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    coerce_tmp_chpl3 = ((uint64_t)(INT64(2)));
#line 2559 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    i_lhs_chpl3 = &delta_chpl;
#line 2559 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    if (call_tmp_chpl7) /* ZLINE: 2559 /Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl */
#line 2559 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    {
#line 2559 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
      tmp_chpl4 = (coerce_tmp_chpl3 == ((uint64_t)(INT64(0))));
#line 2559 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    } else /* ZLINE: 2559 /Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl */
#line 2559 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    {
#line 2559 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
      tmp_chpl4 = UINT8(false);
#line 2559 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    }
#line 2559 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    if (tmp_chpl4) /* ZLINE: 2559 /Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl */
#line 2559 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    {
#line 2559 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
      halt_chpl(&_str_literal_30_chpl, _ln_chpl, _fn_chpl);
#line 2559 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    }
#line 2559 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    *(i_lhs_chpl3) /= coerce_tmp_chpl3;
#line 2552 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    if (call_tmp_chpl6) /* ZLINE: 2552 /Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl */
#line 2552 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    {
#line 2552 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
      tmp_chpl5 = UINT8(true);
#line 2552 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    } else /* ZLINE: 2552 /Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl */
#line 2552 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    {
#line 2552 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
      tmp_chpl5 = (delta_chpl > ((uint64_t)(INT64(0))));
#line 2552 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    }
#line 2552 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    tmp_chpl = tmp_chpl5;
#line 2552 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  }
#line 2561 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  call_tmp_chpl8 = (((uint64_t)((acc_mult_chpl * state_chpl))) + acc_plus_chpl);
#line 2544 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  return call_tmp_chpl8;
#line 2544 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
}

