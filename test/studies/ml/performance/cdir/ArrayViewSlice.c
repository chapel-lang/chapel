#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
/* ArrayViewSlice.chpl:26 */
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
static void chpl__init_ArrayViewSlice(int64_t _ln_chpl,
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
                                      int32_t _fn_chpl) {
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  _ref_int32_t_chpl refIndentLevel_chpl = NULL;
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  if (chpl__init_ArrayViewSlice_p) /* ZLINE: 26 /Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl */
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  {
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
    goto _exit_chpl__init_ArrayViewSlice_chpl;
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  }
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  printModuleInit_chpl("%*s\n", "ArrayViewSlice", INT64(14), _ln_chpl, _fn_chpl);
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  refIndentLevel_chpl = &moduleInitLevel_chpl;
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  *(refIndentLevel_chpl) += INT64(1);
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  chpl__init_ArrayViewSlice_p = UINT8(true);
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  {
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
    chpl__init_Reflection(_ln_chpl, _fn_chpl);
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
    chpl__init_CTypes(_ln_chpl, _fn_chpl);
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  }
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  *(refIndentLevel_chpl) -= INT64(1);
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  _exit_chpl__init_ArrayViewSlice_chpl:;
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  return;
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
}

#line 32 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
/* ArrayViewSlice.chpl:32 */
#line 32 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
static chpl_bool buildIndexCacheHelper_chpl6(DefaultRectangularArr_1_int64_t_one__tuple_2_Tensor_3__real64_int64_t_int64_t_chpl arr_chpl,
#line 32 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
                                             DefaultRectangularDom_1_int64_t_one_chpl dom_chpl) {
#line 32 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  return UINT8(false);
#line 32 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
}

#line 32 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
/* ArrayViewSlice.chpl:32 */
#line 32 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
static chpl_bool buildIndexCacheHelper_chpl5(DefaultRectangularArr_3_int64_t_one__real64_int64_t_chpl arr_chpl,
#line 32 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
                                             DefaultRectangularDom_3_int64_t_one_chpl dom_chpl) {
#line 32 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  return UINT8(false);
#line 32 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
}

#line 32 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
/* ArrayViewSlice.chpl:32 */
#line 32 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
static chpl_bool buildIndexCacheHelper_chpl7(DefaultRectangularArr_2_int64_t_one__real64_int64_t_chpl arr_chpl,
#line 32 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
                                             DefaultRectangularDom_2_int64_t_one_chpl dom_chpl) {
#line 32 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  return UINT8(false);
#line 32 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
}

#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
/* ArrayViewSlice.chpl:51 */
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
static void chpl__auto_destroy_ArrayViewSliceArr(ArrayViewSliceArr__tuple_2_Tensor_3__real64_int64_t_int64_t_DefaultRectangularDom_1_int64_t_one_int64_t_DefaultRectangularArr_1_int64_t_one__tuple_2_Tensor_3__real64_int64_t_int64_t_chpl_bool_chpl this_chpl31,
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
                                                 int64_t _ln_chpl,
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
                                                 int32_t _fn_chpl) {
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  deinit_chpl36(((BaseArr_chpl)(((AbsBaseArr__tuple_2_Tensor_3__real64_int64_t_chpl)(this_chpl31)))), _ln_chpl, _fn_chpl);
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  return;
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
}

#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
/* ArrayViewSlice.chpl:51 */
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
static void chpl__auto_destroy_ArrayViewSliceArr3(ArrayViewSliceArr__real64_int64_t_DefaultRectangularDom_3_int64_t_one_int64_t_DefaultRectangularArr_3_int64_t_one__real64_int64_t_chpl_bool_chpl this_chpl31,
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
                                                  int64_t _ln_chpl,
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
                                                  int32_t _fn_chpl) {
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  deinit_chpl36(((BaseArr_chpl)(((AbsBaseArr__real64_chpl)(this_chpl31)))), _ln_chpl, _fn_chpl);
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  return;
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
}

#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
/* ArrayViewSlice.chpl:51 */
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
static void chpl__auto_destroy_ArrayViewSliceArr2(ArrayViewSliceArr__real64_int64_t_DefaultRectangularDom_2_int64_t_one_int64_t_DefaultRectangularArr_2_int64_t_one__real64_int64_t_chpl_bool_chpl this_chpl31,
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
                                                  int64_t _ln_chpl,
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
                                                  int32_t _fn_chpl) {
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  deinit_chpl36(((BaseArr_chpl)(((AbsBaseArr__real64_chpl)(this_chpl31)))), _ln_chpl, _fn_chpl);
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  return;
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
}

#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
/* ArrayViewSlice.chpl:70 */
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
static void init_chpl15(ArrayViewSliceArr__tuple_2_Tensor_3__real64_int64_t_int64_t_DefaultRectangularDom_1_int64_t_one_int64_t_DefaultRectangularArr_1_int64_t_one__tuple_2_Tensor_3__real64_int64_t_int64_t_chpl_bool_chpl this_chpl31,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
                        int64_t chpl___DomPid_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
                        DefaultRectangularDom_1_int64_t_one_chpl dom_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
                        int64_t chpl___ArrPid_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
                        DefaultRectangularArr_1_int64_t_one__tuple_2_Tensor_3__real64_int64_t_int64_t_chpl chpl___ArrInstance_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
                        int64_t _ln_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
                        int32_t _fn_chpl) {
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  AbsBaseArr__tuple_2_Tensor_3__real64_int64_t_chpl super_tmp_chpl = NULL;
#line 71 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  BaseArr_chpl prev_chpl = NULL;
#line 71 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  BaseArr_chpl next_chpl = NULL;
#line 71 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  chpl_ddataResizePolicy _resizePolicy_chpl;
#line 77 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  chpl_bool call_tmp_chpl6;
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  super_tmp_chpl = &((this_chpl31)->super);
#line 71 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  prev_chpl = ((BaseArr_chpl)(nil));
#line 71 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  next_chpl = ((BaseArr_chpl)(nil));
#line 71 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  _resizePolicy_chpl = chpl_ddataResizePolicy_normalInit_chpl;
#line 71 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 71 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  init_chpl54(super_tmp_chpl, prev_chpl, next_chpl, INT64(-1), UINT8(false), _resizePolicy_chpl, _ln_chpl, _fn_chpl);
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  ((RootClass_chpl)(((AbsBaseArr__tuple_2_Tensor_3__real64_int64_t_chpl)(this_chpl31))))->chpl__cid = chpl__cid_AbsBaseArr__tuple_2_Tensor_3__real64_int64_t_chpl;
#line 72 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 72 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  (this_chpl31)->chpl___DomPid = chpl___DomPid_chpl;
#line 73 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 73 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  (this_chpl31)->dom = dom_chpl;
#line 74 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 74 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  (this_chpl31)->chpl___ArrPid = chpl___ArrPid_chpl;
#line 75 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 75 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  (this_chpl31)->chpl___ArrInstance = chpl___ArrInstance_chpl;
#line 77 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  call_tmp_chpl6 = buildIndexCacheHelper_chpl6(chpl___ArrInstance_chpl, dom_chpl);
#line 77 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 77 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  (this_chpl31)->indexCache = call_tmp_chpl6;
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  ((RootClass_chpl)(this_chpl31))->chpl__cid = chpl__cid_ArrayViewSliceArr__tuple_2_Tensor_3__real64_int64_t_int64_t_DefaultRectangularDom_1_int64_t_one_int64_t_DefaultRectangularArr_1_int64_t_one__tuple_2_Tensor_3__real64_int64_t_int64_t_chpl_bool_chpl;
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  return;
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
}

#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
/* ArrayViewSlice.chpl:70 */
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
static void init_chpl14(ArrayViewSliceArr__real64_int64_t_DefaultRectangularDom_3_int64_t_one_int64_t_DefaultRectangularArr_3_int64_t_one__real64_int64_t_chpl_bool_chpl this_chpl31,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
                        int64_t chpl___DomPid_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
                        DefaultRectangularDom_3_int64_t_one_chpl dom_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
                        int64_t chpl___ArrPid_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
                        DefaultRectangularArr_3_int64_t_one__real64_int64_t_chpl chpl___ArrInstance_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
                        int64_t _ln_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
                        int32_t _fn_chpl) {
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  AbsBaseArr__real64_chpl super_tmp_chpl = NULL;
#line 71 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  BaseArr_chpl prev_chpl = NULL;
#line 71 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  BaseArr_chpl next_chpl = NULL;
#line 71 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  chpl_ddataResizePolicy _resizePolicy_chpl;
#line 77 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  chpl_bool call_tmp_chpl6;
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  super_tmp_chpl = &((this_chpl31)->super);
#line 71 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  prev_chpl = ((BaseArr_chpl)(nil));
#line 71 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  next_chpl = ((BaseArr_chpl)(nil));
#line 71 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  _resizePolicy_chpl = chpl_ddataResizePolicy_normalInit_chpl;
#line 71 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 71 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  init_chpl53(super_tmp_chpl, prev_chpl, next_chpl, INT64(-1), UINT8(false), _resizePolicy_chpl, _ln_chpl, _fn_chpl);
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  ((RootClass_chpl)(((AbsBaseArr__real64_chpl)(this_chpl31))))->chpl__cid = chpl__cid_AbsBaseArr__real64_chpl;
#line 72 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 72 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  (this_chpl31)->chpl___DomPid = chpl___DomPid_chpl;
#line 73 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 73 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  (this_chpl31)->dom = dom_chpl;
#line 74 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 74 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  (this_chpl31)->chpl___ArrPid = chpl___ArrPid_chpl;
#line 75 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 75 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  (this_chpl31)->chpl___ArrInstance = chpl___ArrInstance_chpl;
#line 77 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  call_tmp_chpl6 = buildIndexCacheHelper_chpl5(chpl___ArrInstance_chpl, dom_chpl);
#line 77 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 77 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  (this_chpl31)->indexCache = call_tmp_chpl6;
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  ((RootClass_chpl)(this_chpl31))->chpl__cid = chpl__cid_ArrayViewSliceArr__real64_int64_t_DefaultRectangularDom_3_int64_t_one_int64_t_DefaultRectangularArr_3_int64_t_one__real64_int64_t_chpl_bool_chpl;
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  return;
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
}

#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
/* ArrayViewSlice.chpl:70 */
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
static void init_chpl16(ArrayViewSliceArr__real64_int64_t_DefaultRectangularDom_2_int64_t_one_int64_t_DefaultRectangularArr_2_int64_t_one__real64_int64_t_chpl_bool_chpl this_chpl31,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
                        int64_t chpl___DomPid_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
                        DefaultRectangularDom_2_int64_t_one_chpl dom_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
                        int64_t chpl___ArrPid_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
                        DefaultRectangularArr_2_int64_t_one__real64_int64_t_chpl chpl___ArrInstance_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
                        int64_t _ln_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
                        int32_t _fn_chpl) {
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  AbsBaseArr__real64_chpl super_tmp_chpl = NULL;
#line 71 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  BaseArr_chpl prev_chpl = NULL;
#line 71 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  BaseArr_chpl next_chpl = NULL;
#line 71 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  chpl_ddataResizePolicy _resizePolicy_chpl;
#line 77 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  chpl_bool call_tmp_chpl6;
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  super_tmp_chpl = &((this_chpl31)->super);
#line 71 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  prev_chpl = ((BaseArr_chpl)(nil));
#line 71 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  next_chpl = ((BaseArr_chpl)(nil));
#line 71 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  _resizePolicy_chpl = chpl_ddataResizePolicy_normalInit_chpl;
#line 71 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 71 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  init_chpl53(super_tmp_chpl, prev_chpl, next_chpl, INT64(-1), UINT8(false), _resizePolicy_chpl, _ln_chpl, _fn_chpl);
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  ((RootClass_chpl)(((AbsBaseArr__real64_chpl)(this_chpl31))))->chpl__cid = chpl__cid_AbsBaseArr__real64_chpl;
#line 72 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 72 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  (this_chpl31)->chpl___DomPid = chpl___DomPid_chpl;
#line 73 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 73 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  (this_chpl31)->dom = dom_chpl;
#line 74 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 74 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  (this_chpl31)->chpl___ArrPid = chpl___ArrPid_chpl;
#line 75 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 75 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  (this_chpl31)->chpl___ArrInstance = chpl___ArrInstance_chpl;
#line 77 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  call_tmp_chpl6 = buildIndexCacheHelper_chpl7(chpl___ArrInstance_chpl, dom_chpl);
#line 77 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 77 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  (this_chpl31)->indexCache = call_tmp_chpl6;
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  ((RootClass_chpl)(this_chpl31))->chpl__cid = chpl__cid_ArrayViewSliceArr__real64_int64_t_DefaultRectangularDom_2_int64_t_one_int64_t_DefaultRectangularArr_2_int64_t_one__real64_int64_t_chpl_bool_chpl;
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  return;
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
}

#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
/* ArrayViewSlice.chpl:70 */
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
static ArrayViewSliceArr__tuple_2_Tensor_3__real64_int64_t_int64_t_DefaultRectangularDom_1_int64_t_one_int64_t_DefaultRectangularArr_1_int64_t_one__tuple_2_Tensor_3__real64_int64_t_int64_t_chpl_bool_chpl _new_chpl14(int64_t chpl___DomPid_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
                                                                                                                                                                                                                        DefaultRectangularDom_1_int64_t_one_chpl dom_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
                                                                                                                                                                                                                        int64_t chpl___ArrPid_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
                                                                                                                                                                                                                        DefaultRectangularArr_1_int64_t_one__tuple_2_Tensor_3__real64_int64_t_int64_t_chpl chpl___ArrInstance_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
                                                                                                                                                                                                                        int64_t _ln_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
                                                                                                                                                                                                                        int32_t _fn_chpl) {
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  ArrayViewSliceArr__tuple_2_Tensor_3__real64_int64_t_int64_t_DefaultRectangularDom_1_int64_t_one_int64_t_DefaultRectangularArr_1_int64_t_one__tuple_2_Tensor_3__real64_int64_t_int64_t_chpl_bool_chpl initTemp_chpl = NULL;
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  c_ptr_void_chpl cast_tmp_chpl = NULL;
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  ArrayViewSliceArr__tuple_2_Tensor_3__real64_int64_t_int64_t_DefaultRectangularDom_1_int64_t_one_int64_t_DefaultRectangularArr_1_int64_t_one__tuple_2_Tensor_3__real64_int64_t_int64_t_chpl_bool_chpl tmp_chpl = NULL;
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  cast_tmp_chpl = chpl_here_alloc(sizeof(chpl_ArrayViewSliceArr__tuple_2_Tensor_3__real64_int64_t_int64_t_DefaultRectangularDom_1_int64_t_one_int64_t_DefaultRectangularArr_1_int64_t_one__tuple_2_Tensor_3__real64_int64_t_int64_t_chpl_bool_chpl_object), INT16(217), _ln_chpl, _fn_chpl);
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  initTemp_chpl = ((ArrayViewSliceArr__tuple_2_Tensor_3__real64_int64_t_int64_t_DefaultRectangularDom_1_int64_t_one_int64_t_DefaultRectangularArr_1_int64_t_one__tuple_2_Tensor_3__real64_int64_t_int64_t_chpl_bool_chpl)(cast_tmp_chpl));
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  ((RootClass_chpl)(initTemp_chpl))->chpl__cid = chpl__cid_ArrayViewSliceArr__tuple_2_Tensor_3__real64_int64_t_int64_t_DefaultRectangularDom_1_int64_t_one_int64_t_DefaultRectangularArr_1_int64_t_one__tuple_2_Tensor_3__real64_int64_t_int64_t_chpl_bool_chpl;
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  chpl_check_nil(initTemp_chpl, _ln_chpl, _fn_chpl);
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  init_chpl15(initTemp_chpl, chpl___DomPid_chpl, dom_chpl, chpl___ArrPid_chpl, chpl___ArrInstance_chpl, _ln_chpl, _fn_chpl);
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  tmp_chpl = initTemp_chpl;
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  return tmp_chpl;
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
}

#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
/* ArrayViewSlice.chpl:70 */
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
static ArrayViewSliceArr__real64_int64_t_DefaultRectangularDom_3_int64_t_one_int64_t_DefaultRectangularArr_3_int64_t_one__real64_int64_t_chpl_bool_chpl _new_chpl15(int64_t chpl___DomPid_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
                                                                                                                                                                    DefaultRectangularDom_3_int64_t_one_chpl dom_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
                                                                                                                                                                    int64_t chpl___ArrPid_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
                                                                                                                                                                    DefaultRectangularArr_3_int64_t_one__real64_int64_t_chpl chpl___ArrInstance_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
                                                                                                                                                                    int64_t _ln_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
                                                                                                                                                                    int32_t _fn_chpl) {
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  ArrayViewSliceArr__real64_int64_t_DefaultRectangularDom_3_int64_t_one_int64_t_DefaultRectangularArr_3_int64_t_one__real64_int64_t_chpl_bool_chpl initTemp_chpl = NULL;
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  c_ptr_void_chpl cast_tmp_chpl = NULL;
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  ArrayViewSliceArr__real64_int64_t_DefaultRectangularDom_3_int64_t_one_int64_t_DefaultRectangularArr_3_int64_t_one__real64_int64_t_chpl_bool_chpl tmp_chpl = NULL;
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  cast_tmp_chpl = chpl_here_alloc(sizeof(chpl_ArrayViewSliceArr__real64_int64_t_DefaultRectangularDom_3_int64_t_one_int64_t_DefaultRectangularArr_3_int64_t_one__real64_int64_t_chpl_bool_chpl_object), INT16(276), _ln_chpl, _fn_chpl);
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  initTemp_chpl = ((ArrayViewSliceArr__real64_int64_t_DefaultRectangularDom_3_int64_t_one_int64_t_DefaultRectangularArr_3_int64_t_one__real64_int64_t_chpl_bool_chpl)(cast_tmp_chpl));
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  ((RootClass_chpl)(initTemp_chpl))->chpl__cid = chpl__cid_ArrayViewSliceArr__real64_int64_t_DefaultRectangularDom_3_int64_t_one_int64_t_DefaultRectangularArr_3_int64_t_one__real64_int64_t_chpl_bool_chpl;
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  chpl_check_nil(initTemp_chpl, _ln_chpl, _fn_chpl);
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  init_chpl14(initTemp_chpl, chpl___DomPid_chpl, dom_chpl, chpl___ArrPid_chpl, chpl___ArrInstance_chpl, _ln_chpl, _fn_chpl);
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  tmp_chpl = initTemp_chpl;
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  return tmp_chpl;
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
}

#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
/* ArrayViewSlice.chpl:70 */
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
static ArrayViewSliceArr__real64_int64_t_DefaultRectangularDom_2_int64_t_one_int64_t_DefaultRectangularArr_2_int64_t_one__real64_int64_t_chpl_bool_chpl _new_chpl16(int64_t chpl___DomPid_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
                                                                                                                                                                    DefaultRectangularDom_2_int64_t_one_chpl dom_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
                                                                                                                                                                    int64_t chpl___ArrPid_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
                                                                                                                                                                    DefaultRectangularArr_2_int64_t_one__real64_int64_t_chpl chpl___ArrInstance_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
                                                                                                                                                                    int64_t _ln_chpl,
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
                                                                                                                                                                    int32_t _fn_chpl) {
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  ArrayViewSliceArr__real64_int64_t_DefaultRectangularDom_2_int64_t_one_int64_t_DefaultRectangularArr_2_int64_t_one__real64_int64_t_chpl_bool_chpl initTemp_chpl = NULL;
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  c_ptr_void_chpl cast_tmp_chpl = NULL;
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  ArrayViewSliceArr__real64_int64_t_DefaultRectangularDom_2_int64_t_one_int64_t_DefaultRectangularArr_2_int64_t_one__real64_int64_t_chpl_bool_chpl tmp_chpl = NULL;
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  cast_tmp_chpl = chpl_here_alloc(sizeof(chpl_ArrayViewSliceArr__real64_int64_t_DefaultRectangularDom_2_int64_t_one_int64_t_DefaultRectangularArr_2_int64_t_one__real64_int64_t_chpl_bool_chpl_object), INT16(368), _ln_chpl, _fn_chpl);
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  initTemp_chpl = ((ArrayViewSliceArr__real64_int64_t_DefaultRectangularDom_2_int64_t_one_int64_t_DefaultRectangularArr_2_int64_t_one__real64_int64_t_chpl_bool_chpl)(cast_tmp_chpl));
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  ((RootClass_chpl)(initTemp_chpl))->chpl__cid = chpl__cid_ArrayViewSliceArr__real64_int64_t_DefaultRectangularDom_2_int64_t_one_int64_t_DefaultRectangularArr_2_int64_t_one__real64_int64_t_chpl_bool_chpl;
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  chpl_check_nil(initTemp_chpl, _ln_chpl, _fn_chpl);
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  init_chpl16(initTemp_chpl, chpl___DomPid_chpl, dom_chpl, chpl___ArrPid_chpl, chpl___ArrInstance_chpl, _ln_chpl, _fn_chpl);
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  tmp_chpl = initTemp_chpl;
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  return tmp_chpl;
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
}

#line 366 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
/* ArrayViewSlice.chpl:366 */
#line 366 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
static DefaultRectangularDom_1_int64_t_one_chpl dsiGetBaseDom_chpl9(ArrayViewSliceArr__tuple_2_Tensor_3__real64_int64_t_int64_t_DefaultRectangularDom_1_int64_t_one_int64_t_DefaultRectangularArr_1_int64_t_one__tuple_2_Tensor_3__real64_int64_t_int64_t_chpl_bool_chpl this_chpl31,
#line 366 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
                                                                    int64_t _ln_chpl,
#line 366 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
                                                                    int32_t _fn_chpl) {
#line 366 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  DefaultRectangularDom_1_int64_t_one_chpl ret_chpl = NULL;
#line 367 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 367 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  ret_chpl = (this_chpl31)->dom;
#line 366 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  return ret_chpl;
#line 366 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
}

#line 366 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
/* ArrayViewSlice.chpl:366 */
#line 366 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
static DefaultRectangularDom_3_int64_t_one_chpl dsiGetBaseDom_chpl11(ArrayViewSliceArr__real64_int64_t_DefaultRectangularDom_3_int64_t_one_int64_t_DefaultRectangularArr_3_int64_t_one__real64_int64_t_chpl_bool_chpl this_chpl31,
#line 366 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
                                                                     int64_t _ln_chpl,
#line 366 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
                                                                     int32_t _fn_chpl) {
#line 366 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  DefaultRectangularDom_3_int64_t_one_chpl ret_chpl = NULL;
#line 367 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 367 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  ret_chpl = (this_chpl31)->dom;
#line 366 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  return ret_chpl;
#line 366 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
}

#line 366 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
/* ArrayViewSlice.chpl:366 */
#line 366 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
static DefaultRectangularDom_2_int64_t_one_chpl dsiGetBaseDom_chpl10(ArrayViewSliceArr__real64_int64_t_DefaultRectangularDom_2_int64_t_one_int64_t_DefaultRectangularArr_2_int64_t_one__real64_int64_t_chpl_bool_chpl this_chpl31,
#line 366 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
                                                                     int64_t _ln_chpl,
#line 366 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
                                                                     int32_t _fn_chpl) {
#line 366 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  DefaultRectangularDom_2_int64_t_one_chpl ret_chpl = NULL;
#line 367 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 367 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  ret_chpl = (this_chpl31)->dom;
#line 366 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  return ret_chpl;
#line 366 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
}

#line 399 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
/* ArrayViewSlice.chpl:399 */
#line 399 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
static void dsiDestroyArr_chpl9(ArrayViewSliceArr__tuple_2_Tensor_3__real64_int64_t_int64_t_DefaultRectangularDom_1_int64_t_one_int64_t_DefaultRectangularArr_1_int64_t_one__tuple_2_Tensor_3__real64_int64_t_int64_t_chpl_bool_chpl this_chpl31,
#line 399 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
                                chpl_bool deinitElts_chpl,
#line 399 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
                                int64_t _ln_chpl,
#line 399 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
                                int32_t _fn_chpl) {
#line 399 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  return;
#line 399 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
}

#line 399 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
/* ArrayViewSlice.chpl:399 */
#line 399 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
static void dsiDestroyArr_chpl11(ArrayViewSliceArr__real64_int64_t_DefaultRectangularDom_3_int64_t_one_int64_t_DefaultRectangularArr_3_int64_t_one__real64_int64_t_chpl_bool_chpl this_chpl31,
#line 399 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
                                 chpl_bool deinitElts_chpl,
#line 399 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
                                 int64_t _ln_chpl,
#line 399 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
                                 int32_t _fn_chpl) {
#line 399 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  return;
#line 399 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
}

#line 399 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
/* ArrayViewSlice.chpl:399 */
#line 399 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
static void dsiDestroyArr_chpl10(ArrayViewSliceArr__real64_int64_t_DefaultRectangularDom_2_int64_t_one_int64_t_DefaultRectangularArr_2_int64_t_one__real64_int64_t_chpl_bool_chpl this_chpl31,
#line 399 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
                                 chpl_bool deinitElts_chpl,
#line 399 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
                                 int64_t _ln_chpl,
#line 399 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
                                 int32_t _fn_chpl) {
#line 399 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  return;
#line 399 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
}

#line 407 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
/* ArrayViewSlice.chpl:407 */
#line 407 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
static chpl_bool doiBulkTransferFromKnown_chpl4(ArrayViewSliceArr__real64_int64_t_DefaultRectangularDom_2_int64_t_one_int64_t_DefaultRectangularArr_2_int64_t_one__real64_int64_t_chpl_bool_chpl this_chpl31,
#line 407 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
                                                _domain_DefaultRectangularDom_2_int64_t_one_chpl * destDom_chpl,
#line 407 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
                                                DefaultRectangularArr_2_int64_t_one__real64_int64_t_chpl srcClass_chpl,
#line 407 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
                                                _domain_DefaultRectangularDom_2_int64_t_one_chpl * srcDom_chpl,
#line 407 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
                                                int64_t _ln_chpl,
#line 407 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
                                                int32_t _fn_chpl) {
#line 408 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  DefaultRectangularArr_2_int64_t_one__real64_int64_t_chpl ret_chpl = NULL;
#line 408 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  chpl_bool call_tmp_chpl6;
#line 408 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 408 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  ret_chpl = (this_chpl31)->chpl___ArrInstance;
#line 408 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  chpl_check_nil(ret_chpl, _ln_chpl, _fn_chpl);
#line 408 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  call_tmp_chpl6 = doiBulkTransferFromKnown_chpl13(ret_chpl, destDom_chpl, srcClass_chpl, srcDom_chpl, _ln_chpl, _fn_chpl);
#line 408 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  if (call_tmp_chpl6) /* ZLINE: 408 /Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl */
#line 408 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  {
#line 408 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  } else /* ZLINE: 408 /Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl */
#line 408 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  {
#line 408 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  }
#line 407 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  return call_tmp_chpl6;
#line 407 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
}

#line 411 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
/* ArrayViewSlice.chpl:411 */
#line 411 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
static chpl_bool doiBulkTransferToKnown_chpl5(ArrayViewSliceArr__tuple_2_Tensor_3__real64_int64_t_int64_t_DefaultRectangularDom_1_int64_t_one_int64_t_DefaultRectangularArr_1_int64_t_one__tuple_2_Tensor_3__real64_int64_t_int64_t_chpl_bool_chpl this_chpl31,
#line 411 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
                                              _domain_DefaultRectangularDom_1_int64_t_one_chpl * srcDom_chpl,
#line 411 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
                                              DefaultRectangularArr_1_int64_t_one__tuple_2_Tensor_3__real64_int64_t_int64_t_chpl destClass_chpl,
#line 411 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
                                              _domain_DefaultRectangularDom_1_int64_t_one_chpl * destDom_chpl,
#line 411 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
                                              int64_t _ln_chpl,
#line 411 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
                                              int32_t _fn_chpl) {
#line 412 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  DefaultRectangularArr_1_int64_t_one__tuple_2_Tensor_3__real64_int64_t_int64_t_chpl ret_chpl = NULL;
#line 412 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  chpl_bool call_tmp_chpl6;
#line 412 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 412 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  ret_chpl = (this_chpl31)->chpl___ArrInstance;
#line 412 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  chpl_check_nil(destClass_chpl, _ln_chpl, _fn_chpl);
#line 412 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  call_tmp_chpl6 = doiBulkTransferFromKnown_chpl7(destClass_chpl, destDom_chpl, ret_chpl, srcDom_chpl, _ln_chpl, _fn_chpl);
#line 412 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  if (call_tmp_chpl6) /* ZLINE: 412 /Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl */
#line 412 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  {
#line 412 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  } else /* ZLINE: 412 /Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl */
#line 412 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  {
#line 412 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  }
#line 411 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  return call_tmp_chpl6;
#line 411 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
}

#line 411 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
/* ArrayViewSlice.chpl:411 */
#line 411 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
static chpl_bool doiBulkTransferToKnown_chpl6(ArrayViewSliceArr__real64_int64_t_DefaultRectangularDom_3_int64_t_one_int64_t_DefaultRectangularArr_3_int64_t_one__real64_int64_t_chpl_bool_chpl this_chpl31,
#line 411 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
                                              _domain_DefaultRectangularDom_3_int64_t_one_chpl * srcDom_chpl,
#line 411 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
                                              DefaultRectangularArr_3_int64_t_one__real64_int64_t_chpl destClass_chpl,
#line 411 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
                                              _domain_DefaultRectangularDom_3_int64_t_one_chpl * destDom_chpl,
#line 411 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
                                              int64_t _ln_chpl,
#line 411 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
                                              int32_t _fn_chpl) {
#line 412 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  DefaultRectangularArr_3_int64_t_one__real64_int64_t_chpl ret_chpl = NULL;
#line 412 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  chpl_bool call_tmp_chpl6;
#line 412 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 412 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  ret_chpl = (this_chpl31)->chpl___ArrInstance;
#line 412 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  chpl_check_nil(destClass_chpl, _ln_chpl, _fn_chpl);
#line 412 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  call_tmp_chpl6 = doiBulkTransferFromKnown_chpl8(destClass_chpl, destDom_chpl, ret_chpl, srcDom_chpl, _ln_chpl, _fn_chpl);
#line 412 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  if (call_tmp_chpl6) /* ZLINE: 412 /Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl */
#line 412 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  {
#line 412 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  } else /* ZLINE: 412 /Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl */
#line 412 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  {
#line 412 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  }
#line 411 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
  return call_tmp_chpl6;
#line 411 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewSlice.chpl"
}

