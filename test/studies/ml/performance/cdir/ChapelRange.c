#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
/* ChapelRange.chpl:23 */
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
static void chpl__init_ChapelRange(int64_t _ln_chpl,
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                                   int32_t _fn_chpl) {
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  _ref_int32_t_chpl refIndentLevel_chpl = NULL;
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  if (chpl__init_ChapelRange_p) /* ZLINE: 23 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  {
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    goto _exit_chpl__init_ChapelRange_chpl;
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  }
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  printModuleInit_chpl("%*s\n", "ChapelRange", INT64(11), _ln_chpl, _fn_chpl);
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  refIndentLevel_chpl = &moduleInitLevel_chpl;
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  *(refIndentLevel_chpl) += INT64(1);
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  chpl__init_ChapelRange_p = UINT8(true);
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  {
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    chpl__init_ChapelBase(_ln_chpl, _fn_chpl);
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    chpl__init_HaltWrappers(_ln_chpl, _fn_chpl);
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    chpl__init_AutoMath(_ln_chpl, _fn_chpl);
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    chpl__init_DSIUtil(_ln_chpl, _fn_chpl);
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    chpl__init_Reflection(_ln_chpl, _fn_chpl);
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    chpl__init_ChapelDebugPrint(_ln_chpl, _fn_chpl);
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  }
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  *(refIndentLevel_chpl) -= INT64(1);
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  _exit_chpl__init_ChapelRange_chpl:;
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  return;
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
}

#line 216 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
/* ChapelRange.chpl:216 */
#line 216 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
static void init_chpl107(range_int64_t_both_one_chpl * this_chpl31,
#line 216 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                         int64_t low_chpl2,
#line 216 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                         int64_t high_chpl) {
#line 219 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  (this_chpl31)->_low = low_chpl2;
#line 220 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  (this_chpl31)->_high = high_chpl;
#line 216 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  return;
#line 216 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
}

#line 216 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
/* ChapelRange.chpl:216 */
#line 216 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
static void init_chpl106(range_byteIndex_both_one_chpl * this_chpl31,
#line 216 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                         byteIndex_chpl * low_chpl2,
#line 216 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                         byteIndex_chpl * high_chpl) {
#line 219 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  (this_chpl31)->_low = (low_chpl2)->_bindex;
#line 220 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  (this_chpl31)->_high = (high_chpl)->_bindex;
#line 216 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  return;
#line 216 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
}

#line 226 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
/* ChapelRange.chpl:226 */
#line 226 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
static void init_chpl109(range_int64_t_low_one_chpl * this_chpl31,
#line 226 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                         int64_t low_chpl2) {
#line 229 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  (this_chpl31)->_low = low_chpl2;
#line 230 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  (this_chpl31)->_high = INT64(0);
#line 226 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  return;
#line 226 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
}

#line 226 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
/* ChapelRange.chpl:226 */
#line 226 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
static void init_chpl110(range_byteIndex_low_one_chpl * this_chpl31,
#line 226 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                         byteIndex_chpl * low_chpl2) {
#line 229 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  (this_chpl31)->_low = (low_chpl2)->_bindex;
#line 230 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  (this_chpl31)->_high = INT64(0);
#line 226 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  return;
#line 226 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
}

#line 226 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
/* ChapelRange.chpl:226 */
#line 226 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
static void init_chpl108(range_uint64_t_low_one_chpl * this_chpl31,
#line 226 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                         uint64_t low_chpl2) {
#line 229 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  (this_chpl31)->_low = low_chpl2;
#line 230 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  (this_chpl31)->_high = UINT64(0);
#line 226 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  return;
#line 226 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
}

#line 239 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
/* ChapelRange.chpl:239 */
#line 239 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
static void init_chpl111(range_byteIndex_high_one_chpl * this_chpl31,
#line 239 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                         byteIndex_chpl * high_chpl) {
#line 242 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  (this_chpl31)->_low = INT64(0);
#line 242 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  (this_chpl31)->_high = (high_chpl)->_bindex;
#line 239 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  return;
#line 239 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
}

#line 252 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
/* ChapelRange.chpl:252 */
#line 252 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
static void init_chpl112(range_int64_t_neither_one_chpl * this_chpl31) {
#line 255 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  (this_chpl31)->_low = INT64(0);
#line 255 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  (this_chpl31)->_high = INT64(0);
#line 252 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  return;
#line 252 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
}

#line 265 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
/* ChapelRange.chpl:265 */
#line 265 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
static void init_chpl113(range_int64_t_both_one_chpl * this_chpl31) {
#line 268 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  init_chpl116(this_chpl31, INT64(1), INT64(0), INT64(1), INT64(0));
#line 265 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  return;
#line 265 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
}

#line 279 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
/* ChapelRange.chpl:279 */
#line 279 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
static void init_chpl114(range_uint64_t_low_positive_chpl * this_chpl31,
#line 279 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                         uint64_t _low_chpl,
#line 279 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                         uint64_t _high_chpl,
#line 279 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                         int64_t _stride_chpl,
#line 279 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                         uint64_t _alignment_chpl,
#line 279 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                         chpl_bool _aligned_chpl,
#line 279 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                         int64_t _ln_chpl,
#line 279 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                         int32_t _fn_chpl) {
#line 287 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  int64_t tmp_chpl;
#line 288 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  uint64_t call_tmp_chpl6;
#line 287 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  if (! _aligned_chpl) /* ZLINE: 287 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 287 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  {
#line 287 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    tmp_chpl = INT64(-1);
#line 287 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  } else /* ZLINE: 287 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 287 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  {
#line 288 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    call_tmp_chpl6 = chpl__mod(_alignment_chpl, _stride_chpl, _ln_chpl, _fn_chpl);
#line 287 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    tmp_chpl = ((int64_t)(call_tmp_chpl6));
#line 287 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  }
#line 291 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  init_chpl121(this_chpl31, _low_chpl, _high_chpl, _stride_chpl, tmp_chpl, _ln_chpl, _fn_chpl);
#line 279 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  return;
#line 279 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
}

#line 299 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
/* ChapelRange.chpl:299 */
#line 299 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
static void init_chpl115(range_int64_t_both_one_chpl * this_chpl31,
#line 299 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                         int64_t _low_chpl,
#line 299 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                         int64_t _high_chpl) {
#line 306 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  (this_chpl31)->_low = _low_chpl;
#line 307 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  (this_chpl31)->_high = _high_chpl;
#line 299 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  return;
#line 299 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
}

#line 299 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
/* ChapelRange.chpl:299 */
#line 299 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
static void init_chpl116(range_int64_t_both_one_chpl * this_chpl31,
#line 299 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                         int64_t _low_chpl,
#line 299 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                         int64_t _high_chpl,
#line 299 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                         int64_t _stride_chpl,
#line 299 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                         int64_t alignmentValue_chpl) {
#line 306 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  (this_chpl31)->_low = _low_chpl;
#line 307 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  (this_chpl31)->_high = _high_chpl;
#line 299 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  return;
#line 299 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
}

#line 299 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
/* ChapelRange.chpl:299 */
#line 299 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
static void init_chpl120(range_int64_t_low_one_chpl * this_chpl31,
#line 299 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                         int64_t _low_chpl,
#line 299 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                         int64_t _high_chpl) {
#line 306 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  (this_chpl31)->_low = _low_chpl;
#line 307 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  (this_chpl31)->_high = _high_chpl;
#line 299 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  return;
#line 299 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
}

#line 299 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
/* ChapelRange.chpl:299 */
#line 299 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
static void init_chpl119(range_int64_t_both_one_chpl * this_chpl31,
#line 299 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                         int64_t _low_chpl,
#line 299 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                         int64_t _high_chpl,
#line 299 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                         int64_t _stride_chpl) {
#line 306 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  (this_chpl31)->_low = _low_chpl;
#line 307 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  (this_chpl31)->_high = _high_chpl;
#line 299 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  return;
#line 299 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
}

#line 299 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
/* ChapelRange.chpl:299 */
#line 299 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
static void init_chpl117(range_int64_t_high_one_chpl * this_chpl31,
#line 299 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                         int64_t _low_chpl,
#line 299 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                         int64_t _high_chpl) {
#line 306 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  (this_chpl31)->_low = _low_chpl;
#line 307 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  (this_chpl31)->_high = _high_chpl;
#line 299 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  return;
#line 299 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
}

#line 299 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
/* ChapelRange.chpl:299 */
#line 299 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
static void init_chpl118(range_int64_t_both_negOne_chpl * this_chpl31,
#line 299 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                         int64_t _low_chpl,
#line 299 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                         int64_t _high_chpl) {
#line 306 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  (this_chpl31)->_low = _low_chpl;
#line 307 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  (this_chpl31)->_high = _high_chpl;
#line 299 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  return;
#line 299 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
}

#line 299 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
/* ChapelRange.chpl:299 */
#line 299 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
static void init_chpl121(range_uint64_t_low_positive_chpl * this_chpl31,
#line 299 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                         uint64_t _low_chpl,
#line 299 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                         uint64_t _high_chpl,
#line 299 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                         int64_t _stride_chpl,
#line 299 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                         int64_t alignmentValue_chpl,
#line 299 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                         int64_t _ln_chpl,
#line 299 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                         int32_t _fn_chpl) {
#line 306 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  (this_chpl31)->_low = _low_chpl;
#line 307 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  (this_chpl31)->_high = _high_chpl;
#line 310 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  (this_chpl31)->_stride = _stride_chpl;
#line 311 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  (this_chpl31)->_alignment = alignmentValue_chpl;
#line 313 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  verifyAppropriateStide_chpl(_stride_chpl, _ln_chpl, _fn_chpl);
#line 299 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  return;
#line 299 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
}

#line 400 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
/* ChapelRange.chpl:400 */
#line 400 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
static void chpl_build_bounded_range4(range_int64_t_both_one_chpl * _retArg_chpl) {
#line 402 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl6;
#line 402 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  range_int64_t_both_one_chpl initTemp_chpl;
#line 402 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  init_chpl107(&initTemp_chpl, INT64(0), INT64(2));
#line 402 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  call_tmp_chpl6 = initTemp_chpl;
#line 400 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  *(_retArg_chpl) = call_tmp_chpl6;
#line 400 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  return;
#line 400 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
}

#line 400 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
/* ChapelRange.chpl:400 */
#line 400 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
static void chpl_build_bounded_range2(range_int64_t_both_one_chpl * _retArg_chpl) {
#line 402 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl6;
#line 402 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  range_int64_t_both_one_chpl initTemp_chpl;
#line 402 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  init_chpl107(&initTemp_chpl, INT64(0), INT64(0));
#line 402 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  call_tmp_chpl6 = initTemp_chpl;
#line 400 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  *(_retArg_chpl) = call_tmp_chpl6;
#line 400 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  return;
#line 400 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
}

#line 400 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
/* ChapelRange.chpl:400 */
#line 400 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
static void chpl_build_bounded_range3(range_int64_t_both_one_chpl * _retArg_chpl) {
#line 402 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl6;
#line 402 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  range_int64_t_both_one_chpl initTemp_chpl;
#line 402 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  init_chpl107(&initTemp_chpl, INT64(0), INT64(1));
#line 402 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  call_tmp_chpl6 = initTemp_chpl;
#line 400 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  *(_retArg_chpl) = call_tmp_chpl6;
#line 400 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  return;
#line 400 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
}

#line 400 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
/* ChapelRange.chpl:400 */
#line 400 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
static void chpl_build_bounded_range(range_int64_t_both_one_chpl * _retArg_chpl) {
#line 402 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl6;
#line 402 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  range_int64_t_both_one_chpl initTemp_chpl;
#line 402 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  init_chpl107(&initTemp_chpl, INT64(0), INT64(3));
#line 402 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  call_tmp_chpl6 = initTemp_chpl;
#line 400 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  *(_retArg_chpl) = call_tmp_chpl6;
#line 400 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  return;
#line 400 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
}

#line 400 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
/* ChapelRange.chpl:400 */
#line 400 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
static void chpl_build_bounded_range5(range_int64_t_both_one_chpl * _retArg_chpl) {
#line 402 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl6;
#line 402 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  range_int64_t_both_one_chpl initTemp_chpl;
#line 402 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  init_chpl107(&initTemp_chpl, INT64(1), INT64(0));
#line 402 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  call_tmp_chpl6 = initTemp_chpl;
#line 400 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  *(_retArg_chpl) = call_tmp_chpl6;
#line 400 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  return;
#line 400 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
}

#line 411 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
/* ChapelRange.chpl:411 */
#line 411 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
static void chpl_build_bounded_range6(int64_t low_chpl2,
#line 411 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                                      int64_t high_chpl,
#line 411 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                                      range_int64_t_both_one_chpl * _retArg_chpl) {
#line 412 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl6;
#line 412 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  range_int64_t_both_one_chpl initTemp_chpl;
#line 412 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  init_chpl107(&initTemp_chpl, low_chpl2, high_chpl);
#line 412 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  call_tmp_chpl6 = initTemp_chpl;
#line 411 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  *(_retArg_chpl) = call_tmp_chpl6;
#line 411 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  return;
#line 411 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
}

#line 445 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
/* ChapelRange.chpl:445 */
#line 445 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
static int64_t chpl__nudgeHighBound(int64_t high_chpl) {
#line 446 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  int64_t call_tmp_chpl6;
#line 446 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  call_tmp_chpl6 = (high_chpl - INT64(1));
#line 445 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  return call_tmp_chpl6;
#line 445 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
}

#line 445 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
/* ChapelRange.chpl:445 */
#line 445 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
static int32_t chpl__nudgeHighBound2(int32_t high_chpl,
#line 445 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                                     int64_t _ln_chpl,
#line 445 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                                     int32_t _fn_chpl) {
#line 446 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  int32_t call_tmp_chpl6;
#line 446 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  call_tmp_chpl6 = (high_chpl - ((int32_t)(INT64(1))));
#line 445 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  return call_tmp_chpl6;
#line 445 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
}

#line 453 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
/* ChapelRange.chpl:453 */
#line 453 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
static void chpl_build_low_bounded_range2(int64_t low_chpl2,
#line 453 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                                          range_int64_t_low_one_chpl * _retArg_chpl) {
#line 454 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  range_int64_t_low_one_chpl call_tmp_chpl6;
#line 454 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  range_int64_t_low_one_chpl initTemp_chpl;
#line 454 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  init_chpl109(&initTemp_chpl, low_chpl2);
#line 454 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  call_tmp_chpl6 = initTemp_chpl;
#line 453 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  *(_retArg_chpl) = call_tmp_chpl6;
#line 453 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  return;
#line 453 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
}

#line 453 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
/* ChapelRange.chpl:453 */
#line 453 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
static void chpl_build_low_bounded_range(uint64_t low_chpl2,
#line 453 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                                         range_uint64_t_low_one_chpl * _retArg_chpl) {
#line 454 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  range_uint64_t_low_one_chpl call_tmp_chpl6;
#line 454 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  range_uint64_t_low_one_chpl initTemp_chpl;
#line 454 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  init_chpl108(&initTemp_chpl, low_chpl2);
#line 454 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  call_tmp_chpl6 = initTemp_chpl;
#line 453 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  *(_retArg_chpl) = call_tmp_chpl6;
#line 453 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  return;
#line 453 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
}

#line 477 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
/* ChapelRange.chpl:477 */
#line 477 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
static void chpl_build_unbounded_range(range_int64_t_neither_one_chpl * _retArg_chpl) {
#line 478 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  range_int64_t_neither_one_chpl call_tmp_chpl6;
#line 478 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  range_int64_t_neither_one_chpl initTemp_chpl;
#line 478 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  init_chpl112(&initTemp_chpl);
#line 478 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  call_tmp_chpl6 = initTemp_chpl;
#line 477 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  *(_retArg_chpl) = call_tmp_chpl6;
#line 477 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  return;
#line 477 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
}

#line 481 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
/* ChapelRange.chpl:481 */
#line 481 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
static void createWithSingleElement_chpl(int64_t elm_chpl,
#line 481 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                                         range_int64_t_both_one_chpl * _retArg_chpl) {
#line 482 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl6;
#line 482 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl;
#line 482 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  chpl_build_bounded_range6(elm_chpl, elm_chpl, &ret_tmp_chpl);
#line 482 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  call_tmp_chpl6 = ret_tmp_chpl;
#line 481 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  *(_retArg_chpl) = call_tmp_chpl6;
#line 481 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  return;
#line 481 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
}

#line 1008 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
/* ChapelRange.chpl:1008 */
#line 1008 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
static void verifyAppropriateStide_chpl(int64_t stride_chpl,
#line 1008 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                                        int64_t _ln_chpl,
#line 1008 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                                        int32_t _fn_chpl) {
#line 1009 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  assert_chpl((stride_chpl > INT64(0)), _ln_chpl, _fn_chpl);
#line 1008 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  return;
#line 1008 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
}

#line 1008 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
/* ChapelRange.chpl:1008 */
#line 1008 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
static void verifyAppropriateStide_chpl2(int64_t stride_chpl,
#line 1008 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                                         int64_t _ln_chpl,
#line 1008 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                                         int32_t _fn_chpl) {
#line 1009 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  assert_chpl((stride_chpl > INT64(0)), _ln_chpl, _fn_chpl);
#line 1008 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  return;
#line 1008 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
}

#line 1270 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
/* ChapelRange.chpl:1270 */
#line 1270 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
static int64_t size_chpl8(range_int64_t_both_one_chpl this_chpl31,
#line 1270 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                          int64_t _ln_chpl,
#line 1270 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                          int32_t _fn_chpl) {
#line 1271 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  int64_t call_tmp_chpl6;
#line 1271 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  call_tmp_chpl6 = sizeAs_chpl37(this_chpl31, _ln_chpl, _fn_chpl);
#line 1270 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  return call_tmp_chpl6;
#line 1270 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
}

#line 1284 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
/* ChapelRange.chpl:1284 */
#line 1284 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
static uint64_t sizeAs_chpl36(range_int64_t_both_one_chpl this_chpl31,
#line 1284 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                              int64_t _ln_chpl,
#line 1284 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                              int32_t _fn_chpl) {
#line 1292 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  uint64_t call_tmp_chpl6;
#line 1292 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  call_tmp_chpl6 = sizeAsHelp_chpl2(this_chpl31, (&this_chpl31)->_low, (&this_chpl31)->_high, _ln_chpl, _fn_chpl);
#line 1284 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  return call_tmp_chpl6;
#line 1284 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
}

#line 1284 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
/* ChapelRange.chpl:1284 */
#line 1284 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
static int64_t sizeAs_chpl37(range_int64_t_both_one_chpl this_chpl31,
#line 1284 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                             int64_t _ln_chpl,
#line 1284 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                             int32_t _fn_chpl) {
#line 1292 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  int64_t call_tmp_chpl6;
#line 1292 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  call_tmp_chpl6 = sizeAsHelp_chpl(this_chpl31, (&this_chpl31)->_low, (&this_chpl31)->_high, _ln_chpl, _fn_chpl);
#line 1284 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  return call_tmp_chpl6;
#line 1284 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
}

#line 1296 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
/* ChapelRange.chpl:1296 */
#line 1296 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
static uint64_t sizeAsHelp_chpl2(range_int64_t_both_one_chpl this_chpl31,
#line 1296 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                                 int64_t al_chpl,
#line 1296 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                                 int64_t ah_chpl,
#line 1296 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                                 int64_t _ln_chpl,
#line 1296 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                                 int32_t _fn_chpl) {
#line 1296 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  uint64_t ret_chpl;
#line 1300 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  int64_t ret_chpl2;
#line 1301 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  int64_t ret_chpl3;
#line 1308 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  uint64_t call_tmp_chpl6;
#line 1308 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  chpl_bool tmp_chpl;
#line 1308 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  uint64_t call_tmp_chpl7;
#line 1308 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  uint64_t call_tmp_chpl8;
#line 1309 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  chpl_bool tmp_chpl2;
#line 1309 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  chpl_bool tmp_chpl3;
#line 1310 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  string_chpl call_tmp_chpl9;
#line 1310 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  string_chpl ret_tmp_chpl;
#line 1310 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  string_chpl call_tmp_chpl10;
#line 1310 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  string_chpl ret_tmp_chpl2;
#line 1310 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  string_chpl call_tmp_chpl11;
#line 1310 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  string_chpl ret_tmp_chpl3;
#line 1310 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 1310 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  _ref_string_chpl i_x_chpl2 = NULL;
#line 1310 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  _ref_string_chpl i_x_chpl3 = NULL;
#line 1300 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  ret_chpl2 = (&this_chpl31)->_high;
#line 1301 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  ret_chpl3 = (&this_chpl31)->_low;
#line 1302 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  if (ret_chpl3 > ret_chpl2) /* ZLINE: 1302 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 1302 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  {
#line 1302 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    ret_chpl = UINT64(0);
#line 1302 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    goto _end_sizeAsHelp_chpl;
#line 1302 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  }
#line 1308 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  call_tmp_chpl6 = (((uint64_t)(ret_chpl2)) - ((uint64_t)(ret_chpl3)));
#line 1308 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  if (! (INT64(0) < INT64(0))) /* ZLINE: 1308 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 1308 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  {
#line 1308 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    tmp_chpl = (UINT64(1) == ((uint64_t)(INT64(0))));
#line 1308 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  } else /* ZLINE: 1308 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 1308 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  {
#line 1308 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    tmp_chpl = UINT8(false);
#line 1308 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  }
#line 1308 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  if (tmp_chpl) /* ZLINE: 1308 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 1308 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  {
#line 1308 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    halt_chpl(&_str_literal_30_chpl, _ln_chpl, _fn_chpl);
#line 1308 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  }
#line 1308 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  call_tmp_chpl7 = (call_tmp_chpl6 / UINT64(1));
#line 1308 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  call_tmp_chpl8 = (call_tmp_chpl7 + UINT64(1));
#line 1309 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  if (! (INT64(0) < INT64(0))) /* ZLINE: 1309 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 1309 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  {
#line 1309 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    tmp_chpl2 = (call_tmp_chpl8 == ((uint64_t)(INT64(0))));
#line 1309 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  } else /* ZLINE: 1309 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 1309 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  {
#line 1309 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    tmp_chpl2 = UINT8(false);
#line 1309 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  }
#line 1309 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  if (tmp_chpl2) /* ZLINE: 1309 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 1309 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  {
#line 1309 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    tmp_chpl3 = UINT8(true);
#line 1309 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  } else /* ZLINE: 1309 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 1309 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  {
#line 1309 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    tmp_chpl3 = (call_tmp_chpl8 > UINT64(18446744073709551615));
#line 1309 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  }
#line 1309 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  if (tmp_chpl3) /* ZLINE: 1309 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 1309 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  {
#line 1310 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    chpl___COLON_16(this_chpl31, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 1310 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    call_tmp_chpl9 = ret_tmp_chpl;
#line 1310 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    chpl___PLUS_(&_str_literal_4035_chpl, &call_tmp_chpl9, &ret_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 1310 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    call_tmp_chpl10 = ret_tmp_chpl2;
#line 1310 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    chpl___PLUS_(&call_tmp_chpl10, &_str_literal_183_chpl, &ret_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 1310 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    call_tmp_chpl11 = ret_tmp_chpl3;
#line 1310 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    boundsCheckHalt_chpl(&call_tmp_chpl11, _ln_chpl, _fn_chpl);
#line 1310 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    i_x_chpl = &call_tmp_chpl11;
#line 1310 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    deinit_chpl95(i_x_chpl, _ln_chpl, _fn_chpl);
#line 1310 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    i_x_chpl2 = &call_tmp_chpl10;
#line 1310 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    deinit_chpl95(i_x_chpl2, _ln_chpl, _fn_chpl);
#line 1310 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    i_x_chpl3 = &call_tmp_chpl9;
#line 1310 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    deinit_chpl95(i_x_chpl3, _ln_chpl, _fn_chpl);
#line 1310 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  }
#line 1312 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  ret_chpl = call_tmp_chpl8;
#line 1312 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  _end_sizeAsHelp_chpl:;
#line 1296 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  return ret_chpl;
#line 1296 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
}

#line 1296 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
/* ChapelRange.chpl:1296 */
#line 1296 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
static int64_t sizeAsHelp_chpl(range_int64_t_both_one_chpl this_chpl31,
#line 1296 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                               int64_t al_chpl,
#line 1296 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                               int64_t ah_chpl,
#line 1296 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                               int64_t _ln_chpl,
#line 1296 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                               int32_t _fn_chpl) {
#line 1296 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  int64_t ret_chpl;
#line 1300 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  int64_t ret_chpl2;
#line 1301 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  int64_t ret_chpl3;
#line 1308 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  uint64_t call_tmp_chpl6;
#line 1308 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  chpl_bool tmp_chpl;
#line 1308 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  uint64_t call_tmp_chpl7;
#line 1308 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  uint64_t call_tmp_chpl8;
#line 1309 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  chpl_bool tmp_chpl2;
#line 1309 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  chpl_bool tmp_chpl3;
#line 1309 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  chpl_bool tmp_chpl4;
#line 1310 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  string_chpl call_tmp_chpl9;
#line 1310 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  string_chpl ret_tmp_chpl;
#line 1310 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  string_chpl call_tmp_chpl10;
#line 1310 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  string_chpl ret_tmp_chpl2;
#line 1310 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  string_chpl call_tmp_chpl11;
#line 1310 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  string_chpl ret_tmp_chpl3;
#line 1310 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 1310 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  _ref_string_chpl i_x_chpl2 = NULL;
#line 1310 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  _ref_string_chpl i_x_chpl3 = NULL;
#line 1300 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  ret_chpl2 = (&this_chpl31)->_high;
#line 1301 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  ret_chpl3 = (&this_chpl31)->_low;
#line 1302 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  if (ret_chpl3 > ret_chpl2) /* ZLINE: 1302 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 1302 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  {
#line 1302 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    ret_chpl = INT64(0);
#line 1302 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    goto _end_sizeAsHelp_chpl;
#line 1302 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  }
#line 1308 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  call_tmp_chpl6 = (((uint64_t)(ret_chpl2)) - ((uint64_t)(ret_chpl3)));
#line 1308 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  if (! (INT64(0) < INT64(0))) /* ZLINE: 1308 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 1308 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  {
#line 1308 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    tmp_chpl = (UINT64(1) == ((uint64_t)(INT64(0))));
#line 1308 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  } else /* ZLINE: 1308 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 1308 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  {
#line 1308 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    tmp_chpl = UINT8(false);
#line 1308 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  }
#line 1308 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  if (tmp_chpl) /* ZLINE: 1308 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 1308 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  {
#line 1308 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    halt_chpl(&_str_literal_30_chpl, _ln_chpl, _fn_chpl);
#line 1308 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  }
#line 1308 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  call_tmp_chpl7 = (call_tmp_chpl6 / UINT64(1));
#line 1308 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  call_tmp_chpl8 = (call_tmp_chpl7 + UINT64(1));
#line 1309 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  if (! (INT64(0) < INT64(0))) /* ZLINE: 1309 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 1309 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  {
#line 1309 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    tmp_chpl2 = (call_tmp_chpl8 == ((uint64_t)(INT64(0))));
#line 1309 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  } else /* ZLINE: 1309 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 1309 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  {
#line 1309 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    tmp_chpl2 = UINT8(false);
#line 1309 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  }
#line 1309 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  if (tmp_chpl2) /* ZLINE: 1309 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 1309 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  {
#line 1309 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    tmp_chpl3 = UINT8(true);
#line 1309 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  } else /* ZLINE: 1309 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 1309 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  {
#line 1309 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    if (INT64(9223372036854775807) < INT64(0)) /* ZLINE: 1309 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 1309 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    {
#line 1309 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
      tmp_chpl4 = UINT8(true);
#line 1309 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    } else /* ZLINE: 1309 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 1309 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    {
#line 1309 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
      tmp_chpl4 = (call_tmp_chpl8 > ((uint64_t)(INT64(9223372036854775807))));
#line 1309 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    }
#line 1309 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    tmp_chpl3 = tmp_chpl4;
#line 1309 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  }
#line 1309 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  if (tmp_chpl3) /* ZLINE: 1309 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 1309 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  {
#line 1310 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    chpl___COLON_16(this_chpl31, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 1310 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    call_tmp_chpl9 = ret_tmp_chpl;
#line 1310 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    chpl___PLUS_(&_str_literal_4038_chpl, &call_tmp_chpl9, &ret_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 1310 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    call_tmp_chpl10 = ret_tmp_chpl2;
#line 1310 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    chpl___PLUS_(&call_tmp_chpl10, &_str_literal_183_chpl, &ret_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 1310 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    call_tmp_chpl11 = ret_tmp_chpl3;
#line 1310 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    boundsCheckHalt_chpl(&call_tmp_chpl11, _ln_chpl, _fn_chpl);
#line 1310 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    i_x_chpl = &call_tmp_chpl11;
#line 1310 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    deinit_chpl95(i_x_chpl, _ln_chpl, _fn_chpl);
#line 1310 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    i_x_chpl2 = &call_tmp_chpl10;
#line 1310 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    deinit_chpl95(i_x_chpl2, _ln_chpl, _fn_chpl);
#line 1310 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    i_x_chpl3 = &call_tmp_chpl9;
#line 1310 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    deinit_chpl95(i_x_chpl3, _ln_chpl, _fn_chpl);
#line 1310 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  }
#line 1312 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  ret_chpl = ((int64_t)(call_tmp_chpl8));
#line 1312 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  _end_sizeAsHelp_chpl:;
#line 1296 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  return ret_chpl;
#line 1296 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
}

#line 1491 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
/* ChapelRange.chpl:1491 */
#line 1491 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
static void adjustedSlice_chpl(range_int64_t_both_one_chpl * slice_chpl,
#line 1491 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                               range_int64_t_both_one_chpl * _retArg_chpl) {
#line 1491 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  range_int64_t_both_one_chpl ret_chpl;
#line 1491 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  ret_chpl = *(slice_chpl);
#line 1491 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  *(_retArg_chpl) = ret_chpl;
#line 1491 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  return;
#line 1491 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
}

#line 1522 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
/* ChapelRange.chpl:1522 */
#line 1522 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
static chpl_bool chpl___EQUALS_2(range_int64_t_both_one_chpl r1_chpl,
#line 1522 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                                 range_int64_t_both_one_chpl r2_chpl,
#line 1522 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                                 int64_t _ln_chpl,
#line 1522 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                                 int32_t _fn_chpl) {
#line 1522 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  chpl_bool ret_chpl;
#line 1536 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  uint64_t call_tmp_chpl6;
#line 1536 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  uint64_t call_tmp_chpl7;
#line 1538 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  chpl_bool tmp_chpl;
#line 1540 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  chpl_bool tmp_chpl2;
#line 1536 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  call_tmp_chpl6 = sizeAs_chpl36(r1_chpl, _ln_chpl, _fn_chpl);
#line 1536 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  call_tmp_chpl7 = sizeAs_chpl36(r2_chpl, _ln_chpl, _fn_chpl);
#line 1537 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  if (call_tmp_chpl6 != call_tmp_chpl7) /* ZLINE: 1537 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 1537 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  {
#line 1537 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    ret_chpl = UINT8(false);
#line 1537 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    goto _end__EQUALS__chpl;
#line 1537 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  }
#line 1538 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  if (! (INT64(0) < INT64(0))) /* ZLINE: 1538 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 1538 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  {
#line 1538 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    tmp_chpl = (call_tmp_chpl6 == ((uint64_t)(INT64(0))));
#line 1538 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  } else /* ZLINE: 1538 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 1538 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  {
#line 1538 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    tmp_chpl = UINT8(false);
#line 1538 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  }
#line 1538 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  if (tmp_chpl) /* ZLINE: 1538 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 1538 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  {
#line 1538 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    ret_chpl = UINT8(true);
#line 1538 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    goto _end__EQUALS__chpl;
#line 1538 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  }
#line 1539 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  if ((&r1_chpl)->_low != (&r2_chpl)->_low) /* ZLINE: 1539 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 1539 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  {
#line 1539 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    ret_chpl = UINT8(false);
#line 1539 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    goto _end__EQUALS__chpl;
#line 1539 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  }
#line 1540 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  if (! (INT64(1) < INT64(0))) /* ZLINE: 1540 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 1540 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  {
#line 1540 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    tmp_chpl2 = (call_tmp_chpl6 == ((uint64_t)(INT64(1))));
#line 1540 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  } else /* ZLINE: 1540 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 1540 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  {
#line 1540 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    tmp_chpl2 = UINT8(false);
#line 1540 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  }
#line 1540 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  if (tmp_chpl2) /* ZLINE: 1540 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 1540 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  {
#line 1540 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    ret_chpl = UINT8(true);
#line 1540 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    goto _end__EQUALS__chpl;
#line 1540 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  }
#line 1542 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  ret_chpl = UINT8(true);
#line 1542 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  _end__EQUALS__chpl:;
#line 1522 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  return ret_chpl;
#line 1522 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
}

#line 1560 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
/* ChapelRange.chpl:1560 */
#line 1560 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
static chpl_bool chpl___EXCLAMATION__ASSIGN_2(range_int64_t_both_one_chpl r1_chpl,
#line 1560 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                                              range_int64_t_both_one_chpl r2_chpl,
#line 1560 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                                              int64_t _ln_chpl,
#line 1560 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                                              int32_t _fn_chpl) {
#line 1560 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  chpl_bool call_tmp_chpl6;
#line 1560 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  chpl_bool call_tmp_chpl7;
#line 1560 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  call_tmp_chpl6 = chpl___EQUALS_2(r1_chpl, r2_chpl, _ln_chpl, _fn_chpl);
#line 1560 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  call_tmp_chpl7 = (! call_tmp_chpl6);
#line 1560 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  return call_tmp_chpl7;
#line 1560 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
}

#line 1685 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
/* ChapelRange.chpl:1685 */
#line 1685 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
static void chpl___COLON_11(range_byteIndex_both_one_chpl r_chpl,
#line 1685 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                            range_int64_t_both_one_chpl * _retArg_chpl) {
#line 1688 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  range_int64_t_both_one_chpl tmp_chpl;
#line 1689 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  range_int64_t_both_one_chpl initTemp_chpl;
#line 1689 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  init_chpl115(&initTemp_chpl, (&r_chpl)->_low, (&r_chpl)->_high);
#line 1688 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  tmp_chpl = initTemp_chpl;
#line 1685 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  *(_retArg_chpl) = tmp_chpl;
#line 1685 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  return;
#line 1685 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
}

#line 1685 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
/* ChapelRange.chpl:1685 */
#line 1685 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
static void chpl___COLON_9(range_byteIndex_low_one_chpl r_chpl,
#line 1685 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                           range_int64_t_low_one_chpl * _retArg_chpl) {
#line 1688 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  range_int64_t_low_one_chpl tmp_chpl;
#line 1689 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  range_int64_t_low_one_chpl initTemp_chpl;
#line 1689 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  init_chpl120(&initTemp_chpl, (&r_chpl)->_low, (&r_chpl)->_high);
#line 1688 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  tmp_chpl = initTemp_chpl;
#line 1685 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  *(_retArg_chpl) = tmp_chpl;
#line 1685 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  return;
#line 1685 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
}

#line 1685 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
/* ChapelRange.chpl:1685 */
#line 1685 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
static void chpl___COLON_10(range_byteIndex_high_one_chpl r_chpl,
#line 1685 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                            range_int64_t_high_one_chpl * _retArg_chpl) {
#line 1688 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  range_int64_t_high_one_chpl tmp_chpl;
#line 1689 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  range_int64_t_high_one_chpl initTemp_chpl;
#line 1689 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  init_chpl117(&initTemp_chpl, (&r_chpl)->_low, (&r_chpl)->_high);
#line 1688 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  tmp_chpl = initTemp_chpl;
#line 1685 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  *(_retArg_chpl) = tmp_chpl;
#line 1685 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  return;
#line 1685 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
}

#line 1936 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
/* ChapelRange.chpl:1936 */
#line 1936 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
static int64_t orderToIndex_chpl2(range_int64_t_both_one_chpl this_chpl31,
#line 1936 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                                  int64_t ord_chpl,
#line 1936 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                                  int64_t _ln_chpl,
#line 1936 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                                  int32_t _fn_chpl) {
#line 1939 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  chpl_bool tmp_chpl;
#line 1946 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  string_chpl call_tmp_chpl6;
#line 1946 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  string_chpl ret_tmp_chpl;
#line 1946 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  string_chpl call_tmp_chpl7;
#line 1946 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  string_chpl ret_tmp_chpl2;
#line 1946 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 1946 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  _ref_string_chpl i_x_chpl2 = NULL;
#line 1948 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  uint64_t call_tmp_chpl8;
#line 1948 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  chpl_bool tmp_chpl2;
#line 1950 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  string_chpl call_tmp_chpl9;
#line 1950 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  string_chpl ret_tmp_chpl3;
#line 1949 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  string_chpl call_tmp_chpl10;
#line 1949 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  string_chpl ret_tmp_chpl4;
#line 1949 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  string_chpl call_tmp_chpl11;
#line 1949 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  string_chpl ret_tmp_chpl5;
#line 1950 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  uint64_t call_tmp_chpl12;
#line 1950 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  string_chpl call_tmp_chpl13;
#line 1950 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  string_chpl ret_tmp_chpl6;
#line 1949 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  string_chpl call_tmp_chpl14;
#line 1949 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  string_chpl ret_tmp_chpl7;
#line 1949 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  _ref_string_chpl i_x_chpl3 = NULL;
#line 1949 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  _ref_string_chpl i_x_chpl4 = NULL;
#line 1949 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  _ref_string_chpl i_x_chpl5 = NULL;
#line 1949 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  _ref_string_chpl i_x_chpl6 = NULL;
#line 1949 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  _ref_string_chpl i_x_chpl7 = NULL;
#line 1953 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  int64_t call_tmp_chpl15;
#line 1939 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  if ((&this_chpl31)->_low > (&this_chpl31)->_high) /* ZLINE: 1939 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 1939 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  {
#line 1939 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    tmp_chpl = UINT8(false);
#line 1939 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  } else /* ZLINE: 1939 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 1939 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  {
#line 1939 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    tmp_chpl = UINT8(true);
#line 1939 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  }
#line 1939 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  if (! tmp_chpl) /* ZLINE: 1939 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 1939 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  {
#line 1940 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    boundsCheckHalt_chpl(&_str_literal_536_chpl, _ln_chpl, _fn_chpl);
#line 1940 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  }
#line 1945 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  if (ord_chpl < INT64(0)) /* ZLINE: 1945 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 1945 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  {
#line 1946 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    chpl___COLON_25(ord_chpl, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 1946 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    call_tmp_chpl6 = ret_tmp_chpl;
#line 1946 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    chpl___PLUS_(&_str_literal_538_chpl, &call_tmp_chpl6, &ret_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 1946 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    call_tmp_chpl7 = ret_tmp_chpl2;
#line 1946 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    boundsCheckHalt_chpl(&call_tmp_chpl7, _ln_chpl, _fn_chpl);
#line 1946 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    i_x_chpl = &call_tmp_chpl7;
#line 1946 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    deinit_chpl95(i_x_chpl, _ln_chpl, _fn_chpl);
#line 1946 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    i_x_chpl2 = &call_tmp_chpl6;
#line 1946 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    deinit_chpl95(i_x_chpl2, _ln_chpl, _fn_chpl);
#line 1946 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  }
#line 1948 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  call_tmp_chpl8 = sizeAs_chpl36(this_chpl31, _ln_chpl, _fn_chpl);
#line 1948 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  if (! (ord_chpl < INT64(0))) /* ZLINE: 1948 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 1948 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  {
#line 1948 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    tmp_chpl2 = (((uint64_t)(ord_chpl)) >= call_tmp_chpl8);
#line 1948 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  } else /* ZLINE: 1948 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 1948 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  {
#line 1948 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    tmp_chpl2 = UINT8(false);
#line 1948 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  }
#line 1948 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  if (tmp_chpl2) /* ZLINE: 1948 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 1948 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  {
#line 1950 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    chpl___COLON_25(ord_chpl, &ret_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 1950 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    call_tmp_chpl9 = ret_tmp_chpl3;
#line 1949 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    chpl___PLUS_(&_str_literal_539_chpl, &call_tmp_chpl9, &ret_tmp_chpl4, _ln_chpl, _fn_chpl);
#line 1949 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    call_tmp_chpl10 = ret_tmp_chpl4;
#line 1949 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    chpl___PLUS_(&call_tmp_chpl10, &_str_literal_540_chpl, &ret_tmp_chpl5, _ln_chpl, _fn_chpl);
#line 1949 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    call_tmp_chpl11 = ret_tmp_chpl5;
#line 1950 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    call_tmp_chpl12 = sizeAs_chpl36(this_chpl31, _ln_chpl, _fn_chpl);
#line 1950 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    chpl___COLON_24(call_tmp_chpl12, &ret_tmp_chpl6, _ln_chpl, _fn_chpl);
#line 1950 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    call_tmp_chpl13 = ret_tmp_chpl6;
#line 1949 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    chpl___PLUS_(&call_tmp_chpl11, &call_tmp_chpl13, &ret_tmp_chpl7, _ln_chpl, _fn_chpl);
#line 1949 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    call_tmp_chpl14 = ret_tmp_chpl7;
#line 1949 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    boundsCheckHalt_chpl(&call_tmp_chpl14, _ln_chpl, _fn_chpl);
#line 1949 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    i_x_chpl3 = &call_tmp_chpl14;
#line 1949 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    deinit_chpl95(i_x_chpl3, _ln_chpl, _fn_chpl);
#line 1949 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    i_x_chpl4 = &call_tmp_chpl13;
#line 1949 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    deinit_chpl95(i_x_chpl4, _ln_chpl, _fn_chpl);
#line 1949 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    i_x_chpl5 = &call_tmp_chpl11;
#line 1949 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    deinit_chpl95(i_x_chpl5, _ln_chpl, _fn_chpl);
#line 1949 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    i_x_chpl6 = &call_tmp_chpl10;
#line 1949 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    deinit_chpl95(i_x_chpl6, _ln_chpl, _fn_chpl);
#line 1949 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    i_x_chpl7 = &call_tmp_chpl9;
#line 1949 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    deinit_chpl95(i_x_chpl7, _ln_chpl, _fn_chpl);
#line 1949 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  }
#line 1953 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  call_tmp_chpl15 = chpl__addRangeStrides((&this_chpl31)->_low, INT64(1), ord_chpl);
#line 1936 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  return call_tmp_chpl15;
#line 1936 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
}

#line 1936 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
/* ChapelRange.chpl:1936 */
#line 1936 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
static int64_t orderToIndex_chpl(range_int64_t_low_one_chpl this_chpl31,
#line 1936 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                                 int64_t ord_chpl,
#line 1936 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                                 int64_t _ln_chpl,
#line 1936 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                                 int32_t _fn_chpl) {
#line 1946 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  string_chpl call_tmp_chpl6;
#line 1946 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  string_chpl ret_tmp_chpl;
#line 1946 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  string_chpl call_tmp_chpl7;
#line 1946 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  string_chpl ret_tmp_chpl2;
#line 1946 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 1946 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  _ref_string_chpl i_x_chpl2 = NULL;
#line 1953 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  int64_t call_tmp_chpl8;
#line 1945 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  if (ord_chpl < INT64(0)) /* ZLINE: 1945 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 1945 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  {
#line 1946 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    chpl___COLON_25(ord_chpl, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 1946 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    call_tmp_chpl6 = ret_tmp_chpl;
#line 1946 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    chpl___PLUS_(&_str_literal_538_chpl, &call_tmp_chpl6, &ret_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 1946 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    call_tmp_chpl7 = ret_tmp_chpl2;
#line 1946 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    boundsCheckHalt_chpl(&call_tmp_chpl7, _ln_chpl, _fn_chpl);
#line 1946 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    i_x_chpl = &call_tmp_chpl7;
#line 1946 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    deinit_chpl95(i_x_chpl, _ln_chpl, _fn_chpl);
#line 1946 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    i_x_chpl2 = &call_tmp_chpl6;
#line 1946 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    deinit_chpl95(i_x_chpl2, _ln_chpl, _fn_chpl);
#line 1946 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  }
#line 1953 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  call_tmp_chpl8 = chpl__addRangeStrides((&this_chpl31)->_low, INT64(1), ord_chpl);
#line 1936 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  return call_tmp_chpl8;
#line 1936 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
}

#line 2292 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
/* ChapelRange.chpl:2292 */
#line 2292 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
static void chpl_by_help(range_uint64_t_low_one_chpl r_chpl,
#line 2292 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                         uint64_t step_chpl,
#line 2292 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                         range_uint64_t_low_positive_chpl * _retArg_chpl,
#line 2292 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                         int64_t _ln_chpl,
#line 2292 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                         int32_t _fn_chpl) {
#line 2293 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  uint64_t coerce_tmp_chpl3;
#line 2294 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  uint64_t coerce_tmp_chpl4;
#line 2295 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  int64_t call_tmp_chpl6;
#line 2304 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  range_uint64_t_low_positive_chpl tmp_chpl;
#line 2304 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  range_uint64_t_low_positive_chpl ret_tmp_chpl;
#line 2293 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  coerce_tmp_chpl3 = (&r_chpl)->_low;
#line 2294 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  coerce_tmp_chpl4 = (&r_chpl)->_high;
#line 2295 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  call_tmp_chpl6 = (INT64(1) * ((int64_t)(step_chpl)));
#line 2297 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  compilerAssert_chpl();
#line 2305 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  newAlignedRange_chpl((&r_chpl)->_low, coerce_tmp_chpl3, coerce_tmp_chpl4, call_tmp_chpl6, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 2304 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  tmp_chpl = ret_tmp_chpl;
#line 2292 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  *(_retArg_chpl) = tmp_chpl;
#line 2292 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  return;
#line 2292 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
}

#line 2313 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
/* ChapelRange.chpl:2313 */
#line 2313 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
static void newAlignedRange_chpl(uint64_t alignment_chpl,
#line 2313 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                                 uint64_t lw_chpl,
#line 2313 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                                 uint64_t hh_chpl,
#line 2313 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                                 int64_t st_chpl,
#line 2313 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                                 range_uint64_t_low_positive_chpl * _retArg_chpl,
#line 2313 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                                 int64_t _ln_chpl,
#line 2313 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                                 int32_t _fn_chpl) {
#line 2314 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  range_uint64_t_low_positive_chpl call_tmp_chpl6;
#line 2314 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  range_uint64_t_low_positive_chpl initTemp_chpl;
#line 2314 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  init_chpl114(&initTemp_chpl, lw_chpl, hh_chpl, st_chpl, alignment_chpl, UINT8(true), _ln_chpl, _fn_chpl);
#line 2314 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  call_tmp_chpl6 = initTemp_chpl;
#line 2313 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  *(_retArg_chpl) = call_tmp_chpl6;
#line 2313 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  return;
#line 2313 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
}

#line 2413 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
/* ChapelRange.chpl:2413 */
#line 2413 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
static void chpl_slice3(range_int64_t_both_one_chpl this_chpl31,
#line 2413 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                        range_int64_t_both_one_chpl other_chpl,
#line 2413 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                        range_int64_t_both_one_chpl * _retArg_chpl) {
#line 2439 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  int64_t coerce_tmp_chpl3;
#line 2443 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  int64_t coerce_tmp_chpl4;
#line 2530 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  int64_t call_tmp_chpl6;
#line 2530 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  int64_t newlo_chpl;
#line 2531 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  int64_t call_tmp_chpl7;
#line 2531 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  int64_t newhi_chpl;
#line 2607 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl8;
#line 2607 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  range_int64_t_both_one_chpl initTemp_chpl;
#line 2439 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  coerce_tmp_chpl3 = (&this_chpl31)->_high;
#line 2443 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  coerce_tmp_chpl4 = (&other_chpl)->_high;
#line 2530 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  call_tmp_chpl6 = myMax_chpl5((&this_chpl31)->_low, (&other_chpl)->_low);
#line 2530 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  newlo_chpl = call_tmp_chpl6;
#line 2531 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  call_tmp_chpl7 = myMin_chpl3(coerce_tmp_chpl3, coerce_tmp_chpl4);
#line 2531 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  newhi_chpl = call_tmp_chpl7;
#line 2590 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  compilerAssert_chpl();
#line 2607 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  init_chpl116(&initTemp_chpl, newlo_chpl, newhi_chpl, INT64(1), INT64(0));
#line 2607 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  call_tmp_chpl8 = initTemp_chpl;
#line 2413 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  *(_retArg_chpl) = call_tmp_chpl8;
#line 2413 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  return;
#line 2413 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
}

#line 2413 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
/* ChapelRange.chpl:2413 */
#line 2413 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
static void chpl_slice(range_int64_t_low_one_chpl this_chpl31,
#line 2413 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                       range_int64_t_both_one_chpl other_chpl,
#line 2413 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                       range_int64_t_both_one_chpl * _retArg_chpl) {
#line 2439 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  int64_t coerce_tmp_chpl3;
#line 2443 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  int64_t coerce_tmp_chpl4;
#line 2530 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  int64_t call_tmp_chpl6;
#line 2530 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  int64_t newlo_chpl;
#line 2531 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  int64_t call_tmp_chpl7;
#line 2531 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  int64_t newhi_chpl;
#line 2607 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl8;
#line 2607 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  range_int64_t_both_one_chpl initTemp_chpl;
#line 2439 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  coerce_tmp_chpl3 = (&other_chpl)->_high;
#line 2443 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  coerce_tmp_chpl4 = (&other_chpl)->_high;
#line 2530 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  call_tmp_chpl6 = myMax_chpl2((&this_chpl31)->_low, (&other_chpl)->_low);
#line 2530 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  newlo_chpl = call_tmp_chpl6;
#line 2531 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  call_tmp_chpl7 = myMin_chpl(coerce_tmp_chpl3, coerce_tmp_chpl4);
#line 2531 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  newhi_chpl = call_tmp_chpl7;
#line 2590 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  compilerAssert_chpl();
#line 2607 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  init_chpl116(&initTemp_chpl, newlo_chpl, newhi_chpl, INT64(1), INT64(0));
#line 2607 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  call_tmp_chpl8 = initTemp_chpl;
#line 2413 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  *(_retArg_chpl) = call_tmp_chpl8;
#line 2413 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  return;
#line 2413 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
}

#line 2413 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
/* ChapelRange.chpl:2413 */
#line 2413 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
static void chpl_slice4(range_int64_t_high_one_chpl this_chpl31,
#line 2413 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                        range_int64_t_both_one_chpl other_chpl,
#line 2413 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                        range_int64_t_both_one_chpl * _retArg_chpl) {
#line 2439 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  int64_t coerce_tmp_chpl3;
#line 2443 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  int64_t coerce_tmp_chpl4;
#line 2530 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  int64_t call_tmp_chpl6;
#line 2530 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  int64_t newlo_chpl;
#line 2531 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  int64_t call_tmp_chpl7;
#line 2531 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  int64_t newhi_chpl;
#line 2607 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl8;
#line 2607 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  range_int64_t_both_one_chpl initTemp_chpl;
#line 2439 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  coerce_tmp_chpl3 = (&this_chpl31)->_high;
#line 2443 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  coerce_tmp_chpl4 = (&other_chpl)->_high;
#line 2530 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  call_tmp_chpl6 = myMax_chpl3((&other_chpl)->_low, (&other_chpl)->_low);
#line 2530 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  newlo_chpl = call_tmp_chpl6;
#line 2531 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  call_tmp_chpl7 = myMin_chpl4(coerce_tmp_chpl3, coerce_tmp_chpl4);
#line 2531 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  newhi_chpl = call_tmp_chpl7;
#line 2590 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  compilerAssert_chpl();
#line 2607 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  init_chpl116(&initTemp_chpl, newlo_chpl, newhi_chpl, INT64(1), INT64(0));
#line 2607 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  call_tmp_chpl8 = initTemp_chpl;
#line 2413 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  *(_retArg_chpl) = call_tmp_chpl8;
#line 2413 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  return;
#line 2413 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
}

#line 2413 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
/* ChapelRange.chpl:2413 */
#line 2413 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
static void chpl_slice2(range_int64_t_both_one_chpl this_chpl31,
#line 2413 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                        range_int64_t_neither_one_chpl other_chpl,
#line 2413 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                        range_int64_t_both_one_chpl * _retArg_chpl) {
#line 2439 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  int64_t coerce_tmp_chpl3;
#line 2443 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  int64_t coerce_tmp_chpl4;
#line 2530 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  int64_t call_tmp_chpl6;
#line 2530 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  int64_t newlo_chpl;
#line 2531 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  int64_t call_tmp_chpl7;
#line 2531 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  int64_t newhi_chpl;
#line 2607 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl8;
#line 2607 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  range_int64_t_both_one_chpl initTemp_chpl;
#line 2439 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  coerce_tmp_chpl3 = (&this_chpl31)->_high;
#line 2443 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  coerce_tmp_chpl4 = (&this_chpl31)->_high;
#line 2530 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  call_tmp_chpl6 = myMax_chpl4((&this_chpl31)->_low, (&this_chpl31)->_low);
#line 2530 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  newlo_chpl = call_tmp_chpl6;
#line 2531 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  call_tmp_chpl7 = myMin_chpl2(coerce_tmp_chpl3, coerce_tmp_chpl4);
#line 2531 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  newhi_chpl = call_tmp_chpl7;
#line 2590 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  compilerAssert_chpl();
#line 2607 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  init_chpl116(&initTemp_chpl, newlo_chpl, newhi_chpl, INT64(1), INT64(0));
#line 2607 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  call_tmp_chpl8 = initTemp_chpl;
#line 2413 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  *(_retArg_chpl) = call_tmp_chpl8;
#line 2413 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  return;
#line 2413 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
}

#line 2413 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
/* ChapelRange.chpl:2413 */
#line 2413 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
static void chpl_slice5(range_int64_t_both_one_chpl this_chpl31,
#line 2413 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                        range_int64_t_both_one_chpl other_chpl,
#line 2413 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                        range_int64_t_both_one_chpl * _retArg_chpl) {
#line 2439 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  int64_t coerce_tmp_chpl3;
#line 2443 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  int64_t coerce_tmp_chpl4;
#line 2530 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  int64_t call_tmp_chpl6;
#line 2530 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  int64_t newlo_chpl;
#line 2531 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  int64_t call_tmp_chpl7;
#line 2531 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  int64_t newhi_chpl;
#line 2607 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl8;
#line 2607 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  range_int64_t_both_one_chpl initTemp_chpl;
#line 2439 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  coerce_tmp_chpl3 = (&this_chpl31)->_high;
#line 2443 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  coerce_tmp_chpl4 = (&other_chpl)->_high;
#line 2530 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  call_tmp_chpl6 = myMax_chpl((&this_chpl31)->_low, (&other_chpl)->_low);
#line 2530 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  newlo_chpl = call_tmp_chpl6;
#line 2531 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  call_tmp_chpl7 = myMin_chpl5(coerce_tmp_chpl3, coerce_tmp_chpl4);
#line 2531 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  newhi_chpl = call_tmp_chpl7;
#line 2590 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  compilerAssert_chpl();
#line 2607 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  init_chpl116(&initTemp_chpl, newlo_chpl, newhi_chpl, INT64(1), INT64(0));
#line 2607 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  call_tmp_chpl8 = initTemp_chpl;
#line 2413 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  *(_retArg_chpl) = call_tmp_chpl8;
#line 2413 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  return;
#line 2413 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
}

#line 2493 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
/* ChapelRange.chpl:2493 */
#line 2493 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
static int64_t myMin_chpl2(int64_t x_chpl,
#line 2493 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                           int64_t y_chpl) {
#line 2494 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  int64_t tmp_chpl;
#line 2494 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  if (x_chpl < y_chpl) /* ZLINE: 2494 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 2494 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  {
#line 2494 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    tmp_chpl = x_chpl;
#line 2494 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  } else /* ZLINE: 2494 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 2494 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  {
#line 2494 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    tmp_chpl = y_chpl;
#line 2494 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  }
#line 2493 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  return tmp_chpl;
#line 2493 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
}

#line 2493 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
/* ChapelRange.chpl:2493 */
#line 2493 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
static int64_t myMin_chpl4(int64_t x_chpl,
#line 2493 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                           int64_t y_chpl) {
#line 2494 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  int64_t tmp_chpl;
#line 2494 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  if (x_chpl < y_chpl) /* ZLINE: 2494 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 2494 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  {
#line 2494 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    tmp_chpl = x_chpl;
#line 2494 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  } else /* ZLINE: 2494 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 2494 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  {
#line 2494 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    tmp_chpl = y_chpl;
#line 2494 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  }
#line 2493 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  return tmp_chpl;
#line 2493 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
}

#line 2493 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
/* ChapelRange.chpl:2493 */
#line 2493 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
static int64_t myMin_chpl(int64_t x_chpl,
#line 2493 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                          int64_t y_chpl) {
#line 2494 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  int64_t tmp_chpl;
#line 2494 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  if (x_chpl < y_chpl) /* ZLINE: 2494 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 2494 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  {
#line 2494 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    tmp_chpl = x_chpl;
#line 2494 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  } else /* ZLINE: 2494 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 2494 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  {
#line 2494 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    tmp_chpl = y_chpl;
#line 2494 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  }
#line 2493 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  return tmp_chpl;
#line 2493 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
}

#line 2493 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
/* ChapelRange.chpl:2493 */
#line 2493 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
static int64_t myMin_chpl3(int64_t x_chpl,
#line 2493 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                           int64_t y_chpl) {
#line 2494 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  int64_t tmp_chpl;
#line 2494 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  if (x_chpl < y_chpl) /* ZLINE: 2494 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 2494 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  {
#line 2494 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    tmp_chpl = x_chpl;
#line 2494 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  } else /* ZLINE: 2494 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 2494 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  {
#line 2494 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    tmp_chpl = y_chpl;
#line 2494 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  }
#line 2493 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  return tmp_chpl;
#line 2493 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
}

#line 2493 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
/* ChapelRange.chpl:2493 */
#line 2493 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
static int64_t myMin_chpl5(int64_t x_chpl,
#line 2493 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                           int64_t y_chpl) {
#line 2494 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  int64_t tmp_chpl;
#line 2494 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  if (x_chpl < y_chpl) /* ZLINE: 2494 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 2494 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  {
#line 2494 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    tmp_chpl = x_chpl;
#line 2494 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  } else /* ZLINE: 2494 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 2494 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  {
#line 2494 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    tmp_chpl = y_chpl;
#line 2494 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  }
#line 2493 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  return tmp_chpl;
#line 2493 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
}

#line 2521 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
/* ChapelRange.chpl:2521 */
#line 2521 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
static int64_t myMax_chpl4(int64_t x_chpl,
#line 2521 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                           int64_t y_chpl) {
#line 2522 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  int64_t tmp_chpl;
#line 2522 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  if (x_chpl > y_chpl) /* ZLINE: 2522 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 2522 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  {
#line 2522 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    tmp_chpl = x_chpl;
#line 2522 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  } else /* ZLINE: 2522 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 2522 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  {
#line 2522 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    tmp_chpl = y_chpl;
#line 2522 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  }
#line 2521 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  return tmp_chpl;
#line 2521 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
}

#line 2521 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
/* ChapelRange.chpl:2521 */
#line 2521 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
static int64_t myMax_chpl(int64_t x_chpl,
#line 2521 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                          int64_t y_chpl) {
#line 2522 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  int64_t tmp_chpl;
#line 2522 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  if (x_chpl > y_chpl) /* ZLINE: 2522 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 2522 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  {
#line 2522 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    tmp_chpl = x_chpl;
#line 2522 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  } else /* ZLINE: 2522 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 2522 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  {
#line 2522 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    tmp_chpl = y_chpl;
#line 2522 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  }
#line 2521 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  return tmp_chpl;
#line 2521 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
}

#line 2521 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
/* ChapelRange.chpl:2521 */
#line 2521 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
static int64_t myMax_chpl5(int64_t x_chpl,
#line 2521 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                           int64_t y_chpl) {
#line 2522 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  int64_t tmp_chpl;
#line 2522 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  if (x_chpl > y_chpl) /* ZLINE: 2522 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 2522 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  {
#line 2522 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    tmp_chpl = x_chpl;
#line 2522 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  } else /* ZLINE: 2522 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 2522 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  {
#line 2522 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    tmp_chpl = y_chpl;
#line 2522 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  }
#line 2521 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  return tmp_chpl;
#line 2521 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
}

#line 2521 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
/* ChapelRange.chpl:2521 */
#line 2521 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
static int64_t myMax_chpl2(int64_t x_chpl,
#line 2521 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                           int64_t y_chpl) {
#line 2522 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  int64_t tmp_chpl;
#line 2522 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  if (x_chpl > y_chpl) /* ZLINE: 2522 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 2522 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  {
#line 2522 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    tmp_chpl = x_chpl;
#line 2522 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  } else /* ZLINE: 2522 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 2522 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  {
#line 2522 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    tmp_chpl = y_chpl;
#line 2522 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  }
#line 2521 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  return tmp_chpl;
#line 2521 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
}

#line 2521 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
/* ChapelRange.chpl:2521 */
#line 2521 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
static int64_t myMax_chpl3(int64_t x_chpl,
#line 2521 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                           int64_t y_chpl) {
#line 2522 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  int64_t tmp_chpl;
#line 2522 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  if (x_chpl > y_chpl) /* ZLINE: 2522 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 2522 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  {
#line 2522 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    tmp_chpl = x_chpl;
#line 2522 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  } else /* ZLINE: 2522 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 2522 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  {
#line 2522 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    tmp_chpl = y_chpl;
#line 2522 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  }
#line 2521 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  return tmp_chpl;
#line 2521 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
}

#line 2706 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
/* ChapelRange.chpl:2706 */
#line 2706 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
static void chpl_count_help3(range_int64_t_low_one_chpl r_chpl,
#line 2706 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                             int32_t count_chpl,
#line 2706 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                             range_int64_t_both_one_chpl * _retArg_chpl,
#line 2706 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                             int64_t _ln_chpl,
#line 2706 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                             int32_t _fn_chpl) {
#line 2706 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  range_int64_t_both_one_chpl ret_chpl;
#line 2727 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  int64_t call_tmp_chpl6;
#line 2723 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl7;
#line 2723 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  range_int64_t_both_one_chpl initTemp_chpl;
#line 2752 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  chpl_bool tmp_chpl;
#line 2754 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  chpl_bool tmp_chpl2;
#line 2798 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  int64_t call_tmp_chpl8;
#line 2801 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  int64_t tmp_chpl3;
#line 2801 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  int64_t tmp_chpl4;
#line 2804 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  int64_t tmp_chpl5;
#line 2804 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  int64_t tmp_chpl6;
#line 2812 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl9;
#line 2812 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  range_int64_t_both_one_chpl initTemp_chpl2;
#line 2721 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  if (count_chpl == ((int32_t)(INT64(0)))) /* ZLINE: 2721 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 2721 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  {
#line 2727 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    call_tmp_chpl6 = absSameType_chpl();
#line 2723 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    init_chpl119(&initTemp_chpl, (&r_chpl)->_low, ((int64_t)(((&r_chpl)->_low - call_tmp_chpl6))), INT64(1));
#line 2723 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    call_tmp_chpl7 = initTemp_chpl;
#line 2723 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    ret_chpl = call_tmp_chpl7;
#line 2723 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    goto _end_chpl_count_help_chpl;
#line 2723 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  }
#line 2752 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  if (count_chpl > ((int32_t)(INT64(0)))) /* ZLINE: 2752 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 2752 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  {
#line 2752 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    tmp_chpl = UINT8(false);
#line 2752 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  } else /* ZLINE: 2752 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 2752 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  {
#line 2752 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    tmp_chpl = UINT8(false);
#line 2752 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  }
#line 2752 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  if (tmp_chpl) /* ZLINE: 2752 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 2752 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  {
#line 2753 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    boundsCheckHalt_chpl(&_str_literal_579_chpl, _ln_chpl, _fn_chpl);
#line 2753 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  }
#line 2754 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  if (count_chpl < INT32(0)) /* ZLINE: 2754 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 2754 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  {
#line 2754 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    tmp_chpl2 = UINT8(true);
#line 2754 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  } else /* ZLINE: 2754 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 2754 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  {
#line 2754 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    tmp_chpl2 = UINT8(false);
#line 2754 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  }
#line 2754 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  if (tmp_chpl2) /* ZLINE: 2754 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 2754 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  {
#line 2755 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    boundsCheckHalt_chpl(&_str_literal_580_chpl, _ln_chpl, _fn_chpl);
#line 2755 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  }
#line 2798 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  call_tmp_chpl8 = (((int64_t)(count_chpl)) * INT64(1));
#line 2801 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  if (call_tmp_chpl8 > INT64(0)) /* ZLINE: 2801 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 2801 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  {
#line 2801 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    tmp_chpl3 = (&r_chpl)->_low;
#line 2801 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  } else /* ZLINE: 2801 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 2801 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  {
#line 2801 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    tmp_chpl4 = chpl__add((&r_chpl)->_high, ((int64_t)((call_tmp_chpl8 + INT64(1)))));
#line 2801 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    tmp_chpl3 = tmp_chpl4;
#line 2801 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  }
#line 2804 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  if (call_tmp_chpl8 < INT64(0)) /* ZLINE: 2804 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 2804 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  {
#line 2804 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    tmp_chpl5 = (&r_chpl)->_high;
#line 2804 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  } else /* ZLINE: 2804 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 2804 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  {
#line 2804 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    tmp_chpl6 = chpl__add((&r_chpl)->_low, ((int64_t)((call_tmp_chpl8 - INT64(1)))));
#line 2804 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    tmp_chpl5 = tmp_chpl6;
#line 2804 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  }
#line 2812 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  init_chpl115(&initTemp_chpl2, tmp_chpl3, tmp_chpl5);
#line 2812 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  call_tmp_chpl9 = initTemp_chpl2;
#line 2812 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  ret_chpl = call_tmp_chpl9;
#line 2812 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  _end_chpl_count_help_chpl:;
#line 2706 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  *(_retArg_chpl) = ret_chpl;
#line 2706 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  return;
#line 2706 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
}

#line 2706 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
/* ChapelRange.chpl:2706 */
#line 2706 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
static void chpl_count_help2(range_int64_t_low_one_chpl r_chpl,
#line 2706 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                             uint32_t count_chpl,
#line 2706 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                             range_int64_t_both_one_chpl * _retArg_chpl,
#line 2706 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                             int64_t _ln_chpl,
#line 2706 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                             int32_t _fn_chpl) {
#line 2706 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  range_int64_t_both_one_chpl ret_chpl;
#line 2721 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  int32_t coerce_tmp_chpl3;
#line 2721 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  chpl_bool tmp_chpl;
#line 2727 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  int64_t call_tmp_chpl6;
#line 2723 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl7;
#line 2723 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  range_int64_t_both_one_chpl initTemp_chpl;
#line 2752 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  int32_t coerce_tmp_chpl4;
#line 2752 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  chpl_bool tmp_chpl2;
#line 2752 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  chpl_bool tmp_chpl3;
#line 2798 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  int64_t call_tmp_chpl8;
#line 2801 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  int64_t tmp_chpl4;
#line 2801 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  int64_t tmp_chpl5;
#line 2804 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  int64_t tmp_chpl6;
#line 2804 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  int64_t tmp_chpl7;
#line 2812 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl9;
#line 2812 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  range_int64_t_both_one_chpl initTemp_chpl2;
#line 2721 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  coerce_tmp_chpl3 = ((int32_t)(INT64(0)));
#line 2721 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  if (! (coerce_tmp_chpl3 < INT32(0))) /* ZLINE: 2721 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 2721 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  {
#line 2721 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    tmp_chpl = (count_chpl == ((uint32_t)(coerce_tmp_chpl3)));
#line 2721 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  } else /* ZLINE: 2721 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 2721 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  {
#line 2721 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    tmp_chpl = UINT8(false);
#line 2721 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  }
#line 2721 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  if (tmp_chpl) /* ZLINE: 2721 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 2721 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  {
#line 2727 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    call_tmp_chpl6 = absSameType_chpl2();
#line 2723 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    init_chpl119(&initTemp_chpl, (&r_chpl)->_low, ((int64_t)(((&r_chpl)->_low - call_tmp_chpl6))), INT64(1));
#line 2723 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    call_tmp_chpl7 = initTemp_chpl;
#line 2723 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    ret_chpl = call_tmp_chpl7;
#line 2723 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    goto _end_chpl_count_help_chpl;
#line 2723 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  }
#line 2752 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  coerce_tmp_chpl4 = ((int32_t)(INT64(0)));
#line 2752 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  if (coerce_tmp_chpl4 < INT32(0)) /* ZLINE: 2752 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 2752 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  {
#line 2752 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    tmp_chpl2 = UINT8(true);
#line 2752 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  } else /* ZLINE: 2752 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 2752 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  {
#line 2752 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    tmp_chpl2 = (count_chpl > ((uint32_t)(coerce_tmp_chpl4)));
#line 2752 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  }
#line 2752 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  if (tmp_chpl2) /* ZLINE: 2752 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 2752 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  {
#line 2752 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    tmp_chpl3 = UINT8(false);
#line 2752 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  } else /* ZLINE: 2752 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 2752 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  {
#line 2752 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    tmp_chpl3 = UINT8(false);
#line 2752 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  }
#line 2752 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  if (tmp_chpl3) /* ZLINE: 2752 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 2752 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  {
#line 2753 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    boundsCheckHalt_chpl(&_str_literal_579_chpl, _ln_chpl, _fn_chpl);
#line 2753 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  }
#line 2754 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  if (UINT8(false)) /* ZLINE: 2754 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 2754 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  {
#line 2755 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    boundsCheckHalt_chpl(&_str_literal_580_chpl, _ln_chpl, _fn_chpl);
#line 2755 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  }
#line 2798 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  call_tmp_chpl8 = (((int64_t)(count_chpl)) * INT64(1));
#line 2801 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  if (call_tmp_chpl8 > INT64(0)) /* ZLINE: 2801 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 2801 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  {
#line 2801 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    tmp_chpl4 = (&r_chpl)->_low;
#line 2801 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  } else /* ZLINE: 2801 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 2801 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  {
#line 2801 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    tmp_chpl5 = chpl__add((&r_chpl)->_high, ((int64_t)((call_tmp_chpl8 + INT64(1)))));
#line 2801 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    tmp_chpl4 = tmp_chpl5;
#line 2801 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  }
#line 2804 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  if (call_tmp_chpl8 < INT64(0)) /* ZLINE: 2804 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 2804 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  {
#line 2804 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    tmp_chpl6 = (&r_chpl)->_high;
#line 2804 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  } else /* ZLINE: 2804 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 2804 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  {
#line 2804 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    tmp_chpl7 = chpl__add((&r_chpl)->_low, ((int64_t)((call_tmp_chpl8 - INT64(1)))));
#line 2804 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    tmp_chpl6 = tmp_chpl7;
#line 2804 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  }
#line 2812 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  init_chpl115(&initTemp_chpl2, tmp_chpl4, tmp_chpl6);
#line 2812 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  call_tmp_chpl9 = initTemp_chpl2;
#line 2812 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  ret_chpl = call_tmp_chpl9;
#line 2812 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  _end_chpl_count_help_chpl:;
#line 2706 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  *(_retArg_chpl) = ret_chpl;
#line 2706 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  return;
#line 2706 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
}

#line 2706 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
/* ChapelRange.chpl:2706 */
#line 2706 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
static void chpl_count_help(range_int64_t_low_one_chpl r_chpl,
#line 2706 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                            int64_t count_chpl,
#line 2706 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                            range_int64_t_both_one_chpl * _retArg_chpl,
#line 2706 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                            int64_t _ln_chpl,
#line 2706 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                            int32_t _fn_chpl) {
#line 2706 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  range_int64_t_both_one_chpl ret_chpl;
#line 2727 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  int64_t call_tmp_chpl6;
#line 2723 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl7;
#line 2723 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  range_int64_t_both_one_chpl initTemp_chpl;
#line 2752 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  chpl_bool tmp_chpl;
#line 2754 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  chpl_bool tmp_chpl2;
#line 2798 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  int64_t call_tmp_chpl8;
#line 2801 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  int64_t tmp_chpl3;
#line 2801 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  int64_t tmp_chpl4;
#line 2804 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  int64_t tmp_chpl5;
#line 2804 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  int64_t tmp_chpl6;
#line 2812 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl9;
#line 2812 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  range_int64_t_both_one_chpl initTemp_chpl2;
#line 2721 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  if (count_chpl == INT64(0)) /* ZLINE: 2721 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 2721 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  {
#line 2727 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    call_tmp_chpl6 = absSameType_chpl3();
#line 2723 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    init_chpl119(&initTemp_chpl, (&r_chpl)->_low, ((int64_t)(((&r_chpl)->_low - call_tmp_chpl6))), INT64(1));
#line 2723 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    call_tmp_chpl7 = initTemp_chpl;
#line 2723 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    ret_chpl = call_tmp_chpl7;
#line 2723 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    goto _end_chpl_count_help_chpl;
#line 2723 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  }
#line 2752 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  if (count_chpl > INT64(0)) /* ZLINE: 2752 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 2752 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  {
#line 2752 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    tmp_chpl = UINT8(false);
#line 2752 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  } else /* ZLINE: 2752 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 2752 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  {
#line 2752 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    tmp_chpl = UINT8(false);
#line 2752 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  }
#line 2752 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  if (tmp_chpl) /* ZLINE: 2752 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 2752 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  {
#line 2753 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    boundsCheckHalt_chpl(&_str_literal_579_chpl, _ln_chpl, _fn_chpl);
#line 2753 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  }
#line 2754 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  if (count_chpl < INT64(0)) /* ZLINE: 2754 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 2754 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  {
#line 2754 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    tmp_chpl2 = UINT8(true);
#line 2754 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  } else /* ZLINE: 2754 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 2754 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  {
#line 2754 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    tmp_chpl2 = UINT8(false);
#line 2754 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  }
#line 2754 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  if (tmp_chpl2) /* ZLINE: 2754 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 2754 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  {
#line 2755 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    boundsCheckHalt_chpl(&_str_literal_580_chpl, _ln_chpl, _fn_chpl);
#line 2755 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  }
#line 2798 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  call_tmp_chpl8 = (count_chpl * INT64(1));
#line 2801 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  if (call_tmp_chpl8 > INT64(0)) /* ZLINE: 2801 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 2801 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  {
#line 2801 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    tmp_chpl3 = (&r_chpl)->_low;
#line 2801 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  } else /* ZLINE: 2801 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 2801 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  {
#line 2801 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    tmp_chpl4 = chpl__add((&r_chpl)->_high, ((int64_t)((call_tmp_chpl8 + INT64(1)))));
#line 2801 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    tmp_chpl3 = tmp_chpl4;
#line 2801 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  }
#line 2804 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  if (call_tmp_chpl8 < INT64(0)) /* ZLINE: 2804 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 2804 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  {
#line 2804 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    tmp_chpl5 = (&r_chpl)->_high;
#line 2804 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  } else /* ZLINE: 2804 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 2804 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  {
#line 2804 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    tmp_chpl6 = chpl__add((&r_chpl)->_low, ((int64_t)((call_tmp_chpl8 - INT64(1)))));
#line 2804 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    tmp_chpl5 = tmp_chpl6;
#line 2804 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  }
#line 2812 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  init_chpl115(&initTemp_chpl2, tmp_chpl3, tmp_chpl5);
#line 2812 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  call_tmp_chpl9 = initTemp_chpl2;
#line 2812 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  ret_chpl = call_tmp_chpl9;
#line 2812 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  _end_chpl_count_help_chpl:;
#line 2706 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  *(_retArg_chpl) = ret_chpl;
#line 2706 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  return;
#line 2706 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
}

#line 2713 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
/* ChapelRange.chpl:2713 */
#line 2713 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
static int64_t absSameType_chpl3(void) {
#line 2713 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  return INT64(1);
#line 2713 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
}

#line 2713 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
/* ChapelRange.chpl:2713 */
#line 2713 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
static int64_t absSameType_chpl2(void) {
#line 2713 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  return INT64(1);
#line 2713 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
}

#line 2713 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
/* ChapelRange.chpl:2713 */
#line 2713 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
static int64_t absSameType_chpl(void) {
#line 2713 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  return INT64(1);
#line 2713 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
}

#line 2832 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
/* ChapelRange.chpl:2832 */
#line 2832 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
static void chpl___POUND_(range_int64_t_low_one_chpl r_chpl,
#line 2832 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                          int32_t count_chpl,
#line 2832 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                          range_int64_t_both_one_chpl * _retArg_chpl,
#line 2832 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                          int64_t _ln_chpl,
#line 2832 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                          int32_t _fn_chpl) {
#line 2833 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl6;
#line 2833 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl;
#line 2833 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  chpl_count_help3(r_chpl, count_chpl, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 2833 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  call_tmp_chpl6 = ret_tmp_chpl;
#line 2832 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  *(_retArg_chpl) = call_tmp_chpl6;
#line 2832 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  return;
#line 2832 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
}

#line 2832 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
/* ChapelRange.chpl:2832 */
#line 2832 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
static void chpl___POUND_2(range_int64_t_low_one_chpl r_chpl,
#line 2832 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                           uint32_t count_chpl,
#line 2832 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                           range_int64_t_both_one_chpl * _retArg_chpl,
#line 2832 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                           int64_t _ln_chpl,
#line 2832 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                           int32_t _fn_chpl) {
#line 2833 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl6;
#line 2833 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl;
#line 2833 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  chpl_count_help2(r_chpl, count_chpl, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 2833 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  call_tmp_chpl6 = ret_tmp_chpl;
#line 2832 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  *(_retArg_chpl) = call_tmp_chpl6;
#line 2832 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  return;
#line 2832 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
}

#line 2832 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
/* ChapelRange.chpl:2832 */
#line 2832 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
static void chpl___POUND_3(range_int64_t_low_one_chpl r_chpl,
#line 2832 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                           int64_t count_chpl,
#line 2832 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                           range_int64_t_both_one_chpl * _retArg_chpl,
#line 2832 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                           int64_t _ln_chpl,
#line 2832 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                           int32_t _fn_chpl) {
#line 2833 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl6;
#line 2833 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl;
#line 2833 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  chpl_count_help(r_chpl, count_chpl, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 2833 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  call_tmp_chpl6 = ret_tmp_chpl;
#line 2832 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  *(_retArg_chpl) = call_tmp_chpl6;
#line 2832 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  return;
#line 2832 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
}

#line 2854 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
/* ChapelRange.chpl:2854 */
#line 2854 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
static chpl_bool chpl_checkIfRangeIterWillOverflow2(int32_t low_chpl2,
#line 2854 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                                                    int32_t high_chpl,
#line 2854 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                                                    int32_t stride_chpl,
#line 2854 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                                                    int32_t first_chpl2,
#line 2854 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                                                    int32_t last_chpl,
#line 2854 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                                                    chpl_bool shouldHalt_chpl,
#line 2854 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                                                    int64_t _ln_chpl,
#line 2854 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                                                    int32_t _fn_chpl) {
#line 2854 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  chpl_bool ret_chpl;
#line 2861 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  chpl_bool willOverFlow_chpl;
#line 2863 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  chpl_bool tmp_chpl;
#line 2867 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  chpl_bool tmp_chpl2;
#line 2889 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  chpl_bool tmp_chpl3;
#line 2857 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  if (low_chpl2 > high_chpl) /* ZLINE: 2857 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 2857 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  {
#line 2858 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    ret_chpl = UINT8(false);
#line 2858 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    goto _end_chpl_checkIfRangeIterWillOverflow_chpl;
#line 2858 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  }
#line 2861 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  willOverFlow_chpl = UINT8(false);
#line 2863 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  if (last_chpl > ((int32_t)(INT64(0)))) /* ZLINE: 2863 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 2863 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  {
#line 2863 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    tmp_chpl = (stride_chpl > ((int32_t)(INT64(0))));
#line 2863 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  } else /* ZLINE: 2863 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 2863 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  {
#line 2863 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    tmp_chpl = UINT8(false);
#line 2863 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  }
#line 2863 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  if (tmp_chpl) /* ZLINE: 2863 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 2863 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  {
#line 2864 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    if (stride_chpl > ((int32_t)((INT64(2147483647) - last_chpl)))) /* ZLINE: 2864 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 2864 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    {
#line 2865 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
      willOverFlow_chpl = UINT8(true);
#line 2865 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    }
#line 2865 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  } else /* ZLINE: 2863 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 2865 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  {
#line 2867 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    if (last_chpl < INT32(0)) /* ZLINE: 2867 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 2867 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    {
#line 2867 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
      tmp_chpl2 = (stride_chpl < INT32(0));
#line 2867 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    } else /* ZLINE: 2867 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 2867 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    {
#line 2867 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
      tmp_chpl2 = UINT8(false);
#line 2867 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    }
#line 2867 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    if (tmp_chpl2) /* ZLINE: 2867 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 2867 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    {
#line 2868 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
      if (stride_chpl < ((int32_t)((INT64(-2147483648) - last_chpl)))) /* ZLINE: 2868 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 2868 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
      {
#line 2869 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
        willOverFlow_chpl = UINT8(true);
#line 2869 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
      }
#line 2869 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    }
#line 2869 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  }
#line 2889 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  if (willOverFlow_chpl) /* ZLINE: 2889 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 2889 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  {
#line 2889 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    tmp_chpl3 = shouldHalt_chpl;
#line 2889 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  } else /* ZLINE: 2889 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 2889 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  {
#line 2889 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    tmp_chpl3 = UINT8(false);
#line 2889 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  }
#line 2889 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  if (tmp_chpl3) /* ZLINE: 2889 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 2889 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  {
#line 2890 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    boundsCheckHalt_chpl(&_str_literal_588_chpl, _ln_chpl, _fn_chpl);
#line 2890 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  }
#line 2892 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  ret_chpl = willOverFlow_chpl;
#line 2892 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  _end_chpl_checkIfRangeIterWillOverflow_chpl:;
#line 2854 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  return ret_chpl;
#line 2854 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
}

#line 2854 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
/* ChapelRange.chpl:2854 */
#line 2854 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
static chpl_bool chpl_checkIfRangeIterWillOverflow(int64_t low_chpl2,
#line 2854 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                                                   int64_t high_chpl,
#line 2854 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                                                   int64_t stride_chpl,
#line 2854 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                                                   int64_t first_chpl2,
#line 2854 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                                                   int64_t last_chpl,
#line 2854 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                                                   chpl_bool shouldHalt_chpl,
#line 2854 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                                                   int64_t _ln_chpl,
#line 2854 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                                                   int32_t _fn_chpl) {
#line 2854 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  chpl_bool ret_chpl;
#line 2861 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  chpl_bool willOverFlow_chpl;
#line 2863 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  chpl_bool tmp_chpl;
#line 2867 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  chpl_bool tmp_chpl2;
#line 2889 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  chpl_bool tmp_chpl3;
#line 2857 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  if (low_chpl2 > high_chpl) /* ZLINE: 2857 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 2857 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  {
#line 2858 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    ret_chpl = UINT8(false);
#line 2858 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    goto _end_chpl_checkIfRangeIterWillOverflow_chpl;
#line 2858 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  }
#line 2861 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  willOverFlow_chpl = UINT8(false);
#line 2863 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  if (last_chpl > INT64(0)) /* ZLINE: 2863 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 2863 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  {
#line 2863 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    tmp_chpl = (stride_chpl > INT64(0));
#line 2863 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  } else /* ZLINE: 2863 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 2863 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  {
#line 2863 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    tmp_chpl = UINT8(false);
#line 2863 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  }
#line 2863 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  if (tmp_chpl) /* ZLINE: 2863 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 2863 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  {
#line 2864 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    if (stride_chpl > ((int64_t)((INT64(9223372036854775807) - last_chpl)))) /* ZLINE: 2864 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 2864 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    {
#line 2865 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
      willOverFlow_chpl = UINT8(true);
#line 2865 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    }
#line 2865 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  } else /* ZLINE: 2863 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 2865 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  {
#line 2867 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    if (last_chpl < INT64(0)) /* ZLINE: 2867 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 2867 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    {
#line 2867 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
      tmp_chpl2 = (stride_chpl < INT64(0));
#line 2867 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    } else /* ZLINE: 2867 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 2867 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    {
#line 2867 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
      tmp_chpl2 = UINT8(false);
#line 2867 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    }
#line 2867 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    if (tmp_chpl2) /* ZLINE: 2867 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 2867 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    {
#line 2868 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
      if (stride_chpl < ((int64_t)((-INT64(9223372036854775807) - INT64(1) - last_chpl)))) /* ZLINE: 2868 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 2868 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
      {
#line 2869 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
        willOverFlow_chpl = UINT8(true);
#line 2869 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
      }
#line 2869 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    }
#line 2869 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  }
#line 2889 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  if (willOverFlow_chpl) /* ZLINE: 2889 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 2889 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  {
#line 2889 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    tmp_chpl3 = shouldHalt_chpl;
#line 2889 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  } else /* ZLINE: 2889 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 2889 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  {
#line 2889 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    tmp_chpl3 = UINT8(false);
#line 2889 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  }
#line 2889 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  if (tmp_chpl3) /* ZLINE: 2889 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 2889 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  {
#line 2890 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    boundsCheckHalt_chpl(&_str_literal_588_chpl, _ln_chpl, _fn_chpl);
#line 2890 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  }
#line 2892 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  ret_chpl = willOverFlow_chpl;
#line 2892 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  _end_chpl_checkIfRangeIterWillOverflow_chpl:;
#line 2854 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  return ret_chpl;
#line 2854 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
}

#line 2896 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
/* ChapelRange.chpl:2896 */
#line 2896 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
static chpl_bool checkIfIterWillOverflow_chpl(range_int64_t_both_one_chpl this_chpl31,
#line 2896 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                                              chpl_bool shouldHalt_chpl,
#line 2896 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                                              int64_t _ln_chpl,
#line 2896 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                                              int32_t _fn_chpl) {
#line 2899 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  chpl_bool call_tmp_chpl6;
#line 2899 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  call_tmp_chpl6 = chpl_checkIfRangeIterWillOverflow((&this_chpl31)->_low, (&this_chpl31)->_high, INT64(1), (&this_chpl31)->_low, (&this_chpl31)->_high, shouldHalt_chpl, _ln_chpl, _fn_chpl);
#line 2896 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  return call_tmp_chpl6;
#line 2896 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
}

#line 2896 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
/* ChapelRange.chpl:2896 */
#line 2896 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
static chpl_bool checkIfIterWillOverflow_chpl2(range_int64_t_both_negOne_chpl this_chpl31,
#line 2896 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                                               chpl_bool shouldHalt_chpl,
#line 2896 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                                               int64_t _ln_chpl,
#line 2896 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                                               int32_t _fn_chpl) {
#line 2899 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  chpl_bool call_tmp_chpl6;
#line 2899 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  call_tmp_chpl6 = chpl_checkIfRangeIterWillOverflow((&this_chpl31)->_low, (&this_chpl31)->_high, INT64(-1), (&this_chpl31)->_high, (&this_chpl31)->_low, shouldHalt_chpl, _ln_chpl, _fn_chpl);
#line 2896 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  return call_tmp_chpl6;
#line 2896 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
}

#line 3423 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
/* ChapelRange.chpl:3423 */
#line 3423 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
static void advance_chpl18(_ic_these_range_int64_t_both_one_chpl this_chpl31,
#line 3423 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                           int64_t _ln_chpl,
#line 3423 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                           int32_t _fn_chpl) {
#line 3423 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  int64_t more_chpl;
#line 3423 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  range_int64_t_both_one_chpl tmp_chpl;
#line 3423 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  range_int64_t_both_one_chpl tmp_chpl2;
#line 3435 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  int64_t coerce_tmp_chpl3;
#line 3423 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  range_int64_t_both_one_chpl tmp_chpl3;
#line 3436 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  int64_t coerce_tmp_chpl4;
#line 3423 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  int64_t tmp_chpl4;
#line 3423 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  int64_t tmp_chpl5;
#line 3423 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  int64_t tmp_chpl6;
#line 3423 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  int64_t tmp_chpl7;
#line 3423 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 3423 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  more_chpl = (this_chpl31)->more;
#line 3423 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  if (more_chpl == INT64(2)) /* ZLINE: 3423 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 3423 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  {
#line 3423 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    goto _jump_2_chpl;
#line 3423 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  }
#line 3423 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  if (more_chpl == INT64(-2)) /* ZLINE: 3423 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 3423 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  {
#line 3423 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    goto _jump_break_2_chpl;
#line 3423 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  }
#line 3423 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 3423 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  tmp_chpl = (this_chpl31)->F0_this;
#line 3429 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  checkIfIterWillOverflow_chpl(tmp_chpl, UINT8(true), _ln_chpl, _fn_chpl);
#line 3423 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 3423 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  (this_chpl31)->F1_i = INT64(0);
#line 3423 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 3423 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  tmp_chpl2 = (this_chpl31)->F0_this;
#line 3435 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  coerce_tmp_chpl3 = (&tmp_chpl2)->_low;
#line 3423 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 3423 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  (this_chpl31)->F3_call_tmp = coerce_tmp_chpl3;
#line 3423 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 3423 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  tmp_chpl3 = (this_chpl31)->F0_this;
#line 3436 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  coerce_tmp_chpl4 = (&tmp_chpl3)->_high;
#line 3423 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 3423 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  (this_chpl31)->F2_end = coerce_tmp_chpl4;
#line 3439 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  for (chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl),tmp_chpl5 = (this_chpl31)->F3_call_tmp,chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl),(this_chpl31)->F1_i = tmp_chpl5; (chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl),tmp_chpl6 = (this_chpl31)->F1_i,chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl),(tmp_chpl6 <= (this_chpl31)->F2_end)); chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl),tmp_chpl7 = (this_chpl31)->F1_i,tmp_chpl7 += INT64(1),chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl),(this_chpl31)->F1_i = tmp_chpl7) {
#line 3423 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 3423 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    tmp_chpl4 = (this_chpl31)->F1_i;
#line 3423 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 3423 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    (this_chpl31)->value = tmp_chpl4;
#line 3423 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 3423 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    (this_chpl31)->more = INT64(2);
#line 3423 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    goto _end_chpl;
#line 3423 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    _jump_break_2_chpl:;
#line 3443 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    goto _end_these_chpl;
#line 3443 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    _jump_2_chpl:;
#line 3443 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  }
#line 3443 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  _end_these_chpl:;
#line 3423 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 3423 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  (this_chpl31)->more = INT64(0);
#line 3423 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  _end_chpl:;
#line 3423 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  return;
#line 3423 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
}

#line 3745 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
/* ChapelRange.chpl:3745 */
#line 3745 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
static void chpl___COLON_16(range_int64_t_both_one_chpl x_chpl,
#line 3745 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                            string_chpl * _retArg_chpl,
#line 3745 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                            int64_t _ln_chpl,
#line 3745 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                            int32_t _fn_chpl) {
#line 3745 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  string_chpl ret_chpl;
#line 3746 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  string_chpl ret_chpl2;
#line 3749 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  string_chpl call_tmp_chpl6;
#line 3749 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  string_chpl ret_tmp_chpl;
#line 3749 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 3758 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  string_chpl call_tmp_chpl7;
#line 3758 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  string_chpl ret_tmp_chpl2;
#line 3758 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  _ref_string_chpl i_x_chpl2 = NULL;
#line 3746 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  init_chpl194(&ret_chpl2, _ln_chpl, _fn_chpl);
#line 3749 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  chpl___COLON_25((&x_chpl)->_low, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 3749 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  call_tmp_chpl6 = ret_tmp_chpl;
#line 3749 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  chpl___PLUS__ASSIGN_(&ret_chpl2, &call_tmp_chpl6, _ln_chpl, _fn_chpl);
#line 3749 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  i_x_chpl = &call_tmp_chpl6;
#line 3749 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  deinit_chpl95(i_x_chpl, _ln_chpl, _fn_chpl);
#line 3751 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  chpl___PLUS__ASSIGN_(&ret_chpl2, &_str_literal_469_chpl, _ln_chpl, _fn_chpl);
#line 3758 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  chpl___COLON_25((&x_chpl)->_high, &ret_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 3758 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  call_tmp_chpl7 = ret_tmp_chpl2;
#line 3758 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  chpl___PLUS__ASSIGN_(&ret_chpl2, &call_tmp_chpl7, _ln_chpl, _fn_chpl);
#line 3758 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  i_x_chpl2 = &call_tmp_chpl7;
#line 3758 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  deinit_chpl95(i_x_chpl2, _ln_chpl, _fn_chpl);
#line 3770 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  ret_chpl = ret_chpl2;
#line 3745 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  *(_retArg_chpl) = ret_chpl;
#line 3745 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  return;
#line 3745 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
}

#line 3745 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
/* ChapelRange.chpl:3745 */
#line 3745 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
static void chpl___COLON_13(range_byteIndex_both_one_chpl x_chpl,
#line 3745 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                            string_chpl * _retArg_chpl,
#line 3745 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                            int64_t _ln_chpl,
#line 3745 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                            int32_t _fn_chpl) {
#line 3745 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  string_chpl ret_chpl;
#line 3746 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  string_chpl ret_chpl2;
#line 3749 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  byteIndex_chpl call_tmp_chpl6;
#line 3749 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  int64_t coerce_tmp_chpl3;
#line 3749 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  byteIndex_chpl ret_chpl3;
#line 3749 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  byteIndex_chpl cpi_chpl;
#line 3749 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  string_chpl call_tmp_chpl7;
#line 3749 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  string_chpl ret_tmp_chpl;
#line 3749 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 3758 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  byteIndex_chpl call_tmp_chpl8;
#line 3758 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  int64_t coerce_tmp_chpl4;
#line 3758 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  byteIndex_chpl ret_chpl4;
#line 3758 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  byteIndex_chpl cpi_chpl2;
#line 3758 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  string_chpl call_tmp_chpl9;
#line 3758 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  string_chpl ret_tmp_chpl2;
#line 3758 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  _ref_string_chpl i_x_chpl2 = NULL;
#line 3746 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  init_chpl194(&ret_chpl2, _ln_chpl, _fn_chpl);
#line 3749 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  coerce_tmp_chpl3 = (&x_chpl)->_low;
#line 3749 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  init_chpl192(&cpi_chpl);
#line 3749 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  (&cpi_chpl)->_bindex = coerce_tmp_chpl3;
#line 3749 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  ret_chpl3 = cpi_chpl;
#line 3749 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  call_tmp_chpl6 = ret_chpl3;
#line 3749 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  chpl___COLON_18(&call_tmp_chpl6, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 3749 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  call_tmp_chpl7 = ret_tmp_chpl;
#line 3749 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  chpl___PLUS__ASSIGN_(&ret_chpl2, &call_tmp_chpl7, _ln_chpl, _fn_chpl);
#line 3749 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  i_x_chpl = &call_tmp_chpl7;
#line 3749 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  deinit_chpl95(i_x_chpl, _ln_chpl, _fn_chpl);
#line 3751 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  chpl___PLUS__ASSIGN_(&ret_chpl2, &_str_literal_469_chpl, _ln_chpl, _fn_chpl);
#line 3758 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  coerce_tmp_chpl4 = (&x_chpl)->_high;
#line 3758 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  init_chpl192(&cpi_chpl2);
#line 3758 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  (&cpi_chpl2)->_bindex = coerce_tmp_chpl4;
#line 3758 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  ret_chpl4 = cpi_chpl2;
#line 3758 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  call_tmp_chpl8 = ret_chpl4;
#line 3758 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  chpl___COLON_18(&call_tmp_chpl8, &ret_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 3758 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  call_tmp_chpl9 = ret_tmp_chpl2;
#line 3758 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  chpl___PLUS__ASSIGN_(&ret_chpl2, &call_tmp_chpl9, _ln_chpl, _fn_chpl);
#line 3758 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  i_x_chpl2 = &call_tmp_chpl9;
#line 3758 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  deinit_chpl95(i_x_chpl2, _ln_chpl, _fn_chpl);
#line 3770 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  ret_chpl = ret_chpl2;
#line 3745 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  *(_retArg_chpl) = ret_chpl;
#line 3745 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  return;
#line 3745 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
}

#line 3745 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
/* ChapelRange.chpl:3745 */
#line 3745 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
static void chpl___COLON_17(range_int64_t_low_one_chpl x_chpl,
#line 3745 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                            string_chpl * _retArg_chpl,
#line 3745 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                            int64_t _ln_chpl,
#line 3745 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                            int32_t _fn_chpl) {
#line 3745 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  string_chpl ret_chpl;
#line 3746 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  string_chpl ret_chpl2;
#line 3749 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  string_chpl call_tmp_chpl6;
#line 3749 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  string_chpl ret_tmp_chpl;
#line 3749 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 3746 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  init_chpl194(&ret_chpl2, _ln_chpl, _fn_chpl);
#line 3749 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  chpl___COLON_25((&x_chpl)->_low, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 3749 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  call_tmp_chpl6 = ret_tmp_chpl;
#line 3749 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  chpl___PLUS__ASSIGN_(&ret_chpl2, &call_tmp_chpl6, _ln_chpl, _fn_chpl);
#line 3749 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  i_x_chpl = &call_tmp_chpl6;
#line 3749 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  deinit_chpl95(i_x_chpl, _ln_chpl, _fn_chpl);
#line 3751 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  chpl___PLUS__ASSIGN_(&ret_chpl2, &_str_literal_469_chpl, _ln_chpl, _fn_chpl);
#line 3770 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  ret_chpl = ret_chpl2;
#line 3745 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  *(_retArg_chpl) = ret_chpl;
#line 3745 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  return;
#line 3745 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
}

#line 3745 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
/* ChapelRange.chpl:3745 */
#line 3745 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
static void chpl___COLON_12(range_byteIndex_low_one_chpl x_chpl,
#line 3745 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                            string_chpl * _retArg_chpl,
#line 3745 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                            int64_t _ln_chpl,
#line 3745 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                            int32_t _fn_chpl) {
#line 3745 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  string_chpl ret_chpl;
#line 3746 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  string_chpl ret_chpl2;
#line 3749 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  byteIndex_chpl call_tmp_chpl6;
#line 3749 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  int64_t coerce_tmp_chpl3;
#line 3749 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  byteIndex_chpl ret_chpl3;
#line 3749 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  byteIndex_chpl cpi_chpl;
#line 3749 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  string_chpl call_tmp_chpl7;
#line 3749 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  string_chpl ret_tmp_chpl;
#line 3749 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 3746 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  init_chpl194(&ret_chpl2, _ln_chpl, _fn_chpl);
#line 3749 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  coerce_tmp_chpl3 = (&x_chpl)->_low;
#line 3749 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  init_chpl192(&cpi_chpl);
#line 3749 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  (&cpi_chpl)->_bindex = coerce_tmp_chpl3;
#line 3749 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  ret_chpl3 = cpi_chpl;
#line 3749 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  call_tmp_chpl6 = ret_chpl3;
#line 3749 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  chpl___COLON_18(&call_tmp_chpl6, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 3749 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  call_tmp_chpl7 = ret_tmp_chpl;
#line 3749 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  chpl___PLUS__ASSIGN_(&ret_chpl2, &call_tmp_chpl7, _ln_chpl, _fn_chpl);
#line 3749 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  i_x_chpl = &call_tmp_chpl7;
#line 3749 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  deinit_chpl95(i_x_chpl, _ln_chpl, _fn_chpl);
#line 3751 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  chpl___PLUS__ASSIGN_(&ret_chpl2, &_str_literal_469_chpl, _ln_chpl, _fn_chpl);
#line 3770 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  ret_chpl = ret_chpl2;
#line 3745 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  *(_retArg_chpl) = ret_chpl;
#line 3745 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  return;
#line 3745 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
}

#line 3745 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
/* ChapelRange.chpl:3745 */
#line 3745 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
static void chpl___COLON_14(range_byteIndex_high_one_chpl x_chpl,
#line 3745 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                            string_chpl * _retArg_chpl,
#line 3745 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                            int64_t _ln_chpl,
#line 3745 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                            int32_t _fn_chpl) {
#line 3745 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  string_chpl ret_chpl;
#line 3746 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  string_chpl ret_chpl2;
#line 3758 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  byteIndex_chpl call_tmp_chpl6;
#line 3758 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  int64_t coerce_tmp_chpl3;
#line 3758 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  byteIndex_chpl ret_chpl3;
#line 3758 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  byteIndex_chpl cpi_chpl;
#line 3758 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  string_chpl call_tmp_chpl7;
#line 3758 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  string_chpl ret_tmp_chpl;
#line 3758 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 3746 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  init_chpl194(&ret_chpl2, _ln_chpl, _fn_chpl);
#line 3751 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  chpl___PLUS__ASSIGN_(&ret_chpl2, &_str_literal_469_chpl, _ln_chpl, _fn_chpl);
#line 3758 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  coerce_tmp_chpl3 = (&x_chpl)->_high;
#line 3758 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  init_chpl192(&cpi_chpl);
#line 3758 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  (&cpi_chpl)->_bindex = coerce_tmp_chpl3;
#line 3758 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  ret_chpl3 = cpi_chpl;
#line 3758 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  call_tmp_chpl6 = ret_chpl3;
#line 3758 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  chpl___COLON_18(&call_tmp_chpl6, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 3758 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  call_tmp_chpl7 = ret_tmp_chpl;
#line 3758 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  chpl___PLUS__ASSIGN_(&ret_chpl2, &call_tmp_chpl7, _ln_chpl, _fn_chpl);
#line 3758 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  i_x_chpl = &call_tmp_chpl7;
#line 3758 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  deinit_chpl95(i_x_chpl, _ln_chpl, _fn_chpl);
#line 3770 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  ret_chpl = ret_chpl2;
#line 3745 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  *(_retArg_chpl) = ret_chpl;
#line 3745 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  return;
#line 3745 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
}

#line 3745 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
/* ChapelRange.chpl:3745 */
#line 3745 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
static void chpl___COLON_15(range_int64_t_neither_one_chpl x_chpl,
#line 3745 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                            string_chpl * _retArg_chpl,
#line 3745 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                            int64_t _ln_chpl,
#line 3745 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                            int32_t _fn_chpl) {
#line 3745 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  string_chpl ret_chpl;
#line 3746 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  string_chpl ret_chpl2;
#line 3746 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  init_chpl194(&ret_chpl2, _ln_chpl, _fn_chpl);
#line 3751 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  chpl___PLUS__ASSIGN_(&ret_chpl2, &_str_literal_469_chpl, _ln_chpl, _fn_chpl);
#line 3770 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  ret_chpl = ret_chpl2;
#line 3745 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  *(_retArg_chpl) = ret_chpl;
#line 3745 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  return;
#line 3745 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
}

#line 3788 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
/* ChapelRange.chpl:3788 */
#line 3788 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
static uint64_t chpl__mod(uint64_t dividend_chpl,
#line 3788 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                          int64_t modulus_chpl,
#line 3788 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                          int64_t _ln_chpl,
#line 3788 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                          int32_t _fn_chpl) {
#line 3792 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  int64_t m_chpl;
#line 3796 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  chpl_bool tmp_chpl;
#line 3798 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  uint64_t call_tmp_chpl6;
#line 3798 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  chpl_bool tmp_chpl2;
#line 3798 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  uint64_t call_tmp_chpl7;
#line 3792 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  m_chpl = modulus_chpl;
#line 3796 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  if (modulus_chpl < INT64(0)) /* ZLINE: 3796 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 3796 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  {
#line 3796 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    tmp_chpl = (modulus_chpl != -INT64(9223372036854775807) - INT64(1));
#line 3796 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  } else /* ZLINE: 3796 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 3796 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  {
#line 3796 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    tmp_chpl = UINT8(false);
#line 3796 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  }
#line 3796 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  if (tmp_chpl) /* ZLINE: 3796 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 3796 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  {
#line 3796 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    m_chpl = (-modulus_chpl);
#line 3796 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  }
#line 3798 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  call_tmp_chpl6 = ((uint64_t)(m_chpl));
#line 3798 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  if (! (INT64(0) < INT64(0))) /* ZLINE: 3798 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 3798 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  {
#line 3798 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    tmp_chpl2 = (call_tmp_chpl6 == ((uint64_t)(INT64(0))));
#line 3798 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  } else /* ZLINE: 3798 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 3798 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  {
#line 3798 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    tmp_chpl2 = UINT8(false);
#line 3798 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  }
#line 3798 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  if (tmp_chpl2) /* ZLINE: 3798 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 3798 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  {
#line 3798 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    halt_chpl(&_str_literal_31_chpl, _ln_chpl, _fn_chpl);
#line 3798 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  }
#line 3798 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  call_tmp_chpl7 = (dividend_chpl % call_tmp_chpl6);
#line 3788 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  return call_tmp_chpl7;
#line 3788 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
}

#line 3834 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
/* ChapelRange.chpl:3834 */
#line 3834 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
static uint64_t chpl__diffMod(uint64_t minuend_chpl,
#line 3834 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                              uint64_t subtrahend_chpl,
#line 3834 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                              int64_t modulus_chpl,
#line 3834 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                              int64_t _ln_chpl,
#line 3834 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                              int32_t _fn_chpl) {
#line 3839 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  int64_t tmp_chpl;
#line 3841 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  uint64_t call_tmp_chpl6;
#line 3842 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  uint64_t call_tmp_chpl7;
#line 3844 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  uint64_t tmp_chpl2;
#line 3839 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  if (modulus_chpl < INT64(0)) /* ZLINE: 3839 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 3839 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  {
#line 3839 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    tmp_chpl = (-modulus_chpl);
#line 3839 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  } else /* ZLINE: 3839 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 3839 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  {
#line 3839 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    tmp_chpl = modulus_chpl;
#line 3839 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  }
#line 3841 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  call_tmp_chpl6 = chpl__mod(minuend_chpl, tmp_chpl, _ln_chpl, _fn_chpl);
#line 3842 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  call_tmp_chpl7 = chpl__mod(subtrahend_chpl, tmp_chpl, _ln_chpl, _fn_chpl);
#line 3844 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  if (call_tmp_chpl6 < call_tmp_chpl7) /* ZLINE: 3844 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 3844 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  {
#line 3844 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    tmp_chpl2 = ((uint64_t)((((uint64_t)(tmp_chpl)) - ((uint64_t)((call_tmp_chpl7 - call_tmp_chpl6))))));
#line 3844 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  } else /* ZLINE: 3844 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 3844 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  {
#line 3844 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    tmp_chpl2 = ((uint64_t)((call_tmp_chpl6 - call_tmp_chpl7)));
#line 3844 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  }
#line 3834 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  return tmp_chpl2;
#line 3834 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
}

#line 3859 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
/* ChapelRange.chpl:3859 */
#line 3859 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
static int64_t chpl__add(int64_t a_chpl,
#line 3859 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                         int64_t b_chpl) {
#line 3859 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  int64_t ret_chpl;
#line 3864 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  chpl_bool tmp_chpl;
#line 3864 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  chpl_bool tmp_chpl2;
#line 3865 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  chpl_bool tmp_chpl3;
#line 3865 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  chpl_bool tmp_chpl4;
#line 3864 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  if (a_chpl > INT64(0)) /* ZLINE: 3864 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 3864 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  {
#line 3864 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    tmp_chpl = (b_chpl > INT64(0));
#line 3864 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  } else /* ZLINE: 3864 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 3864 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  {
#line 3864 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    tmp_chpl = UINT8(false);
#line 3864 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  }
#line 3864 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  if (tmp_chpl) /* ZLINE: 3864 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 3864 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  {
#line 3864 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    tmp_chpl2 = (b_chpl > ((int64_t)((INT64(9223372036854775807) - a_chpl))));
#line 3864 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  } else /* ZLINE: 3864 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 3864 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  {
#line 3864 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    tmp_chpl2 = UINT8(false);
#line 3864 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  }
#line 3864 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  if (tmp_chpl2) /* ZLINE: 3864 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 3864 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  {
#line 3864 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    ret_chpl = INT64(9223372036854775807);
#line 3864 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    goto _end_chpl__add_chpl;
#line 3864 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  }
#line 3865 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  if (a_chpl < INT64(0)) /* ZLINE: 3865 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 3865 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  {
#line 3865 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    tmp_chpl3 = (b_chpl < INT64(0));
#line 3865 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  } else /* ZLINE: 3865 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 3865 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  {
#line 3865 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    tmp_chpl3 = UINT8(false);
#line 3865 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  }
#line 3865 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  if (tmp_chpl3) /* ZLINE: 3865 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 3865 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  {
#line 3865 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    tmp_chpl4 = (b_chpl < ((int64_t)((-INT64(9223372036854775807) - INT64(1) - a_chpl))));
#line 3865 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  } else /* ZLINE: 3865 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 3865 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  {
#line 3865 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    tmp_chpl4 = UINT8(false);
#line 3865 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  }
#line 3865 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  if (tmp_chpl4) /* ZLINE: 3865 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl */
#line 3865 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  {
#line 3865 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    ret_chpl = -INT64(9223372036854775807) - INT64(1);
#line 3865 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
    goto _end_chpl__add_chpl;
#line 3865 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  }
#line 3875 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  ret_chpl = ((int64_t)((a_chpl + b_chpl)));
#line 3875 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  _end_chpl__add_chpl:;
#line 3859 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  return ret_chpl;
#line 3859 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
}

#line 3881 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
/* ChapelRange.chpl:3881 */
#line 3881 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
static int64_t chpl__addRangeStrides(int64_t start_chpl2,
#line 3881 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                                     int64_t stride_chpl,
#line 3881 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                                     int64_t count_chpl) {
#line 3889 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  int64_t call_tmp_chpl6;
#line 3889 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  int64_t call_tmp_chpl7;
#line 3889 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  call_tmp_chpl6 = mul_chpl(stride_chpl, count_chpl);
#line 3889 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  call_tmp_chpl7 = add_chpl(start_chpl2, call_tmp_chpl6);
#line 3881 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  return call_tmp_chpl7;
#line 3881 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
}

#line 3886 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
/* ChapelRange.chpl:3886 */
#line 3886 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
static int64_t mul_chpl(int64_t a_chpl,
#line 3886 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                        int64_t b_chpl) {
#line 3886 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  int64_t call_tmp_chpl6;
#line 3886 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  call_tmp_chpl6 = (a_chpl * b_chpl);
#line 3886 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  return call_tmp_chpl6;
#line 3886 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
}

#line 3887 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
/* ChapelRange.chpl:3887 */
#line 3887 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
static int64_t add_chpl(int64_t a_chpl,
#line 3887 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
                        int64_t b_chpl) {
#line 3887 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  int64_t call_tmp_chpl6;
#line 3887 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  call_tmp_chpl6 = (a_chpl + b_chpl);
#line 3887 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
  return call_tmp_chpl6;
#line 3887 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelRange.chpl"
}

