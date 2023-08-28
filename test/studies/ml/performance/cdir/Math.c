#line 147 "/Users/iainmoncrief/Documents/chapel/modules/standard/Math.chpl"
/* Math.chpl:147 */
#line 147 "/Users/iainmoncrief/Documents/chapel/modules/standard/Math.chpl"
static void chpl__init_Math(int64_t _ln_chpl,
#line 147 "/Users/iainmoncrief/Documents/chapel/modules/standard/Math.chpl"
                            int32_t _fn_chpl) {
#line 147 "/Users/iainmoncrief/Documents/chapel/modules/standard/Math.chpl"
  c_string_rehook modFormatStr_chpl;
#line 147 "/Users/iainmoncrief/Documents/chapel/modules/standard/Math.chpl"
  c_string_rehook modStr_chpl;
#line 147 "/Users/iainmoncrief/Documents/chapel/modules/standard/Math.chpl"
  _ref_int32_t_chpl refIndentLevel_chpl = NULL;
#line 147 "/Users/iainmoncrief/Documents/chapel/modules/standard/Math.chpl"
  if (chpl__init_Math_p) /* ZLINE: 147 /Users/iainmoncrief/Documents/chapel/modules/standard/Math.chpl */
#line 147 "/Users/iainmoncrief/Documents/chapel/modules/standard/Math.chpl"
  {
#line 147 "/Users/iainmoncrief/Documents/chapel/modules/standard/Math.chpl"
    goto _exit_chpl__init_Math_chpl;
#line 147 "/Users/iainmoncrief/Documents/chapel/modules/standard/Math.chpl"
  }
#line 147 "/Users/iainmoncrief/Documents/chapel/modules/standard/Math.chpl"
  modFormatStr_chpl = "%*s\n";
#line 147 "/Users/iainmoncrief/Documents/chapel/modules/standard/Math.chpl"
  modStr_chpl = "Math";
#line 147 "/Users/iainmoncrief/Documents/chapel/modules/standard/Math.chpl"
  printModuleInit_chpl(modFormatStr_chpl, modStr_chpl, INT64(4), _ln_chpl, _fn_chpl);
#line 147 "/Users/iainmoncrief/Documents/chapel/modules/standard/Math.chpl"
  refIndentLevel_chpl = &moduleInitLevel_chpl;
#line 147 "/Users/iainmoncrief/Documents/chapel/modules/standard/Math.chpl"
  *(refIndentLevel_chpl) += INT64(1);
#line 147 "/Users/iainmoncrief/Documents/chapel/modules/standard/Math.chpl"
  chpl__init_Math_p = UINT8(true);
#line 147 "/Users/iainmoncrief/Documents/chapel/modules/standard/Math.chpl"
  {
#line 147 "/Users/iainmoncrief/Documents/chapel/modules/standard/Math.chpl"
    chpl__init_CTypes(_ln_chpl, _fn_chpl);
#line 147 "/Users/iainmoncrief/Documents/chapel/modules/standard/Math.chpl"
    chpl__init_AutoMath(_ln_chpl, _fn_chpl);
#line 147 "/Users/iainmoncrief/Documents/chapel/modules/standard/Math.chpl"
  }
#line 147 "/Users/iainmoncrief/Documents/chapel/modules/standard/Math.chpl"
  *(refIndentLevel_chpl) -= INT64(1);
#line 147 "/Users/iainmoncrief/Documents/chapel/modules/standard/Math.chpl"
  _endchpl__init_Math_chpl:;
#line 147 "/Users/iainmoncrief/Documents/chapel/modules/standard/Math.chpl"
  _exit_chpl__init_Math_chpl:;
#line 147 "/Users/iainmoncrief/Documents/chapel/modules/standard/Math.chpl"
  return;
#line 147 "/Users/iainmoncrief/Documents/chapel/modules/standard/Math.chpl"
}

#line 383 "/Users/iainmoncrief/Documents/chapel/modules/standard/Math.chpl"
/* Math.chpl:383 */
#line 383 "/Users/iainmoncrief/Documents/chapel/modules/standard/Math.chpl"
static _real64 cos_chpl(_real64 x_chpl) {
#line 383 "/Users/iainmoncrief/Documents/chapel/modules/standard/Math.chpl"
  _real64 ret_chpl;
#line 384 "/Users/iainmoncrief/Documents/chapel/modules/standard/Math.chpl"
  _real64 call_tmp_chpl6;
#line 383 "/Users/iainmoncrief/Documents/chapel/modules/standard/Math.chpl"
  _real64 call_tmp_chpl7;
#line 384 "/Users/iainmoncrief/Documents/chapel/modules/standard/Math.chpl"
  call_tmp_chpl6 = chpl_cos(x_chpl);
#line 384 "/Users/iainmoncrief/Documents/chapel/modules/standard/Math.chpl"
  ret_chpl = call_tmp_chpl6;
#line 383 "/Users/iainmoncrief/Documents/chapel/modules/standard/Math.chpl"
  return ret_chpl;
#line 383 "/Users/iainmoncrief/Documents/chapel/modules/standard/Math.chpl"
}

#line 504 "/Users/iainmoncrief/Documents/chapel/modules/standard/Math.chpl"
/* Math.chpl:504 */
#line 504 "/Users/iainmoncrief/Documents/chapel/modules/standard/Math.chpl"
static _real64 exp_chpl(_real64 x_chpl) {
#line 504 "/Users/iainmoncrief/Documents/chapel/modules/standard/Math.chpl"
  _real64 ret_chpl;
#line 505 "/Users/iainmoncrief/Documents/chapel/modules/standard/Math.chpl"
  _real64 call_tmp_chpl6;
#line 504 "/Users/iainmoncrief/Documents/chapel/modules/standard/Math.chpl"
  _real64 call_tmp_chpl7;
#line 505 "/Users/iainmoncrief/Documents/chapel/modules/standard/Math.chpl"
  call_tmp_chpl6 = chpl_exp(x_chpl);
#line 505 "/Users/iainmoncrief/Documents/chapel/modules/standard/Math.chpl"
  ret_chpl = call_tmp_chpl6;
#line 504 "/Users/iainmoncrief/Documents/chapel/modules/standard/Math.chpl"
  return ret_chpl;
#line 504 "/Users/iainmoncrief/Documents/chapel/modules/standard/Math.chpl"
}

#line 636 "/Users/iainmoncrief/Documents/chapel/modules/standard/Math.chpl"
/* Math.chpl:636 */
#line 636 "/Users/iainmoncrief/Documents/chapel/modules/standard/Math.chpl"
static _real64 log_chpl(_real64 x_chpl) {
#line 636 "/Users/iainmoncrief/Documents/chapel/modules/standard/Math.chpl"
  _real64 ret_chpl;
#line 637 "/Users/iainmoncrief/Documents/chapel/modules/standard/Math.chpl"
  _real64 call_tmp_chpl6;
#line 636 "/Users/iainmoncrief/Documents/chapel/modules/standard/Math.chpl"
  _real64 call_tmp_chpl7;
#line 637 "/Users/iainmoncrief/Documents/chapel/modules/standard/Math.chpl"
  call_tmp_chpl6 = chpl_log(x_chpl);
#line 637 "/Users/iainmoncrief/Documents/chapel/modules/standard/Math.chpl"
  ret_chpl = call_tmp_chpl6;
#line 636 "/Users/iainmoncrief/Documents/chapel/modules/standard/Math.chpl"
  return ret_chpl;
#line 636 "/Users/iainmoncrief/Documents/chapel/modules/standard/Math.chpl"
}

#line 870 "/Users/iainmoncrief/Documents/chapel/modules/standard/Math.chpl"
/* Math.chpl:870 */
#line 870 "/Users/iainmoncrief/Documents/chapel/modules/standard/Math.chpl"
static int64_t log2_chpl(uint64_t x_chpl,
#line 870 "/Users/iainmoncrief/Documents/chapel/modules/standard/Math.chpl"
                         int64_t _ln_chpl,
#line 870 "/Users/iainmoncrief/Documents/chapel/modules/standard/Math.chpl"
                         int32_t _fn_chpl) {
#line 870 "/Users/iainmoncrief/Documents/chapel/modules/standard/Math.chpl"
  int64_t ret_chpl;
#line 871 "/Users/iainmoncrief/Documents/chapel/modules/standard/Math.chpl"
  int64_t call_tmp_chpl6;
#line 871 "/Users/iainmoncrief/Documents/chapel/modules/standard/Math.chpl"
  call_tmp_chpl6 = chpl_log2(x_chpl, _ln_chpl, _fn_chpl);
#line 871 "/Users/iainmoncrief/Documents/chapel/modules/standard/Math.chpl"
  ret_chpl = call_tmp_chpl6;
#line 870 "/Users/iainmoncrief/Documents/chapel/modules/standard/Math.chpl"
  return ret_chpl;
#line 870 "/Users/iainmoncrief/Documents/chapel/modules/standard/Math.chpl"
}

