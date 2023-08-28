#line 62 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
/* Random.chpl:62 */
#line 62 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
static void chpl__init_Random(int64_t _ln_chpl,
#line 62 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
                              int32_t _fn_chpl) {
#line 62 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  _ref_int32_t_chpl refIndentLevel_chpl = NULL;
#line 62 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  if (chpl__init_Random_p) /* ZLINE: 62 /Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl */
#line 62 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  {
#line 62 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    goto _exit_chpl__init_Random_chpl;
#line 62 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  }
#line 62 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  printModuleInit_chpl("%*s\n", "Random", INT64(6), _ln_chpl, _fn_chpl);
#line 62 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  refIndentLevel_chpl = &moduleInitLevel_chpl;
#line 62 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  *(refIndentLevel_chpl) += INT64(1);
#line 62 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  chpl__init_Random_p = UINT8(true);
#line 62 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  {
#line 62 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    chpl__init_RandomSupport(_ln_chpl, _fn_chpl);
#line 62 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    chpl__init_NPBRandom(_ln_chpl, _fn_chpl);
#line 62 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    chpl__init_PCGRandom(_ln_chpl, _fn_chpl);
#line 62 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    chpl__init_IO(_ln_chpl, _fn_chpl);
#line 62 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    chpl__init_Sort(_ln_chpl, _fn_chpl);
#line 62 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    chpl__init_CTypes(_ln_chpl, _fn_chpl);
#line 62 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    chpl__init_AutoMath(_ln_chpl, _fn_chpl);
#line 62 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    chpl__init_ChapelLocks(_ln_chpl, _fn_chpl);
#line 62 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    chpl__init_Reflection(_ln_chpl, _fn_chpl);
#line 62 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    chpl__init_ChapelHashtable(_ln_chpl, _fn_chpl);
#line 62 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    chpl__init_HaltWrappers(_ln_chpl, _fn_chpl);
#line 62 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  }
#line 62 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  *(refIndentLevel_chpl) -= INT64(1);
#line 62 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  _exit_chpl__init_Random_chpl:;
#line 62 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  return;
#line 62 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
}

#line 190 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
/* Random.chpl:190 */
#line 190 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
static void shuffle_chpl2(_array_DefaultRectangularArr_1_int64_t_one__tuple_2_Tensor_3__real64_int64_t_int64_t_chpl * arr_chpl,
#line 190 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
                          int64_t seed_chpl,
#line 190 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
                          int64_t _ln_chpl,
#line 190 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
                          int32_t _fn_chpl) {
#line 196 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  _domain_DefaultRectangularDom_1_int64_t_one_chpl call_tmp_chpl6;
#line 196 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  _domain_DefaultRectangularDom_1_int64_t_one_chpl ret_tmp_chpl;
#line 195 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  _owned_PCGRandomStream_int64_t_F_chpl call_tmp_chpl7;
#line 195 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  _owned_PCGRandomStream_int64_t_F_chpl ret_tmp_chpl2;
#line 195 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  _owned_PCGRandomStream_int64_t_F_chpl randNums_chpl;
#line 199 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  PCGRandomStream_int64_t_F_chpl coerce_tmp_chpl3 = NULL;
#line 196 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  _dom_chpl2(arr_chpl, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 196 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  call_tmp_chpl6 = ret_tmp_chpl;
#line 195 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  createRandomStream_chpl(seed_chpl, &ret_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 195 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  call_tmp_chpl7 = ret_tmp_chpl2;
#line 195 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  randNums_chpl = call_tmp_chpl7;
#line 195 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  chpl__autoDestroy11(&call_tmp_chpl6, _ln_chpl, _fn_chpl);
#line 199 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  coerce_tmp_chpl3 = borrow_chpl(&randNums_chpl, _ln_chpl, _fn_chpl);
#line 199 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  chpl_check_nil(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 199 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  shuffle_chpl(coerce_tmp_chpl3, arr_chpl, _ln_chpl, _fn_chpl);
#line 190 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  chpl__autoDestroy61(&randNums_chpl, _ln_chpl, _fn_chpl);
#line 190 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  return;
#line 190 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
}

#line 259 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
/* Random.chpl:259 */
#line 259 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
static void createRandomStream_chpl(int64_t seed_chpl,
#line 259 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
                                    _owned_PCGRandomStream_int64_t_F_chpl * _retArg_chpl,
#line 259 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
                                    int64_t _ln_chpl,
#line 259 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
                                    int32_t _fn_chpl) {
#line 259 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  _owned_PCGRandomStream_int64_t_F_chpl ret_chpl;
#line 264 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  _owned_PCGRandomStream_int64_t_F_chpl call_tmp_chpl6;
#line 264 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  PCGRandomStream_int64_t_F_chpl new_temp_chpl = NULL;
#line 264 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  _owned_PCGRandomStream_int64_t_F_chpl initTemp_chpl;
#line 264 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  new_temp_chpl = _new_chpl86(seed_chpl, _ln_chpl, _fn_chpl);
#line 264 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  init_chpl177(&initTemp_chpl, new_temp_chpl);
#line 264 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  call_tmp_chpl6 = initTemp_chpl;
#line 264 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  ret_chpl = call_tmp_chpl6;
#line 259 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  *(_retArg_chpl) = ret_chpl;
#line 259 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  return;
#line 259 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
}

