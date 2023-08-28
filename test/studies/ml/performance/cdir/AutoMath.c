#line 107 "/Users/iainmoncrief/Documents/chapel/modules/standard/AutoMath.chpl"
/* AutoMath.chpl:107 */
#line 107 "/Users/iainmoncrief/Documents/chapel/modules/standard/AutoMath.chpl"
static void chpl__init_AutoMath(int64_t _ln_chpl,
#line 107 "/Users/iainmoncrief/Documents/chapel/modules/standard/AutoMath.chpl"
                                int32_t _fn_chpl) {
#line 107 "/Users/iainmoncrief/Documents/chapel/modules/standard/AutoMath.chpl"
  _ref_int32_t_chpl refIndentLevel_chpl = NULL;
#line 107 "/Users/iainmoncrief/Documents/chapel/modules/standard/AutoMath.chpl"
  if (chpl__init_AutoMath_p) /* ZLINE: 107 /Users/iainmoncrief/Documents/chapel/modules/standard/AutoMath.chpl */
#line 107 "/Users/iainmoncrief/Documents/chapel/modules/standard/AutoMath.chpl"
  {
#line 107 "/Users/iainmoncrief/Documents/chapel/modules/standard/AutoMath.chpl"
    goto _exit_chpl__init_AutoMath_chpl;
#line 107 "/Users/iainmoncrief/Documents/chapel/modules/standard/AutoMath.chpl"
  }
#line 107 "/Users/iainmoncrief/Documents/chapel/modules/standard/AutoMath.chpl"
  printModuleInit_chpl("%*s\n", "AutoMath", INT64(8), _ln_chpl, _fn_chpl);
#line 107 "/Users/iainmoncrief/Documents/chapel/modules/standard/AutoMath.chpl"
  refIndentLevel_chpl = &moduleInitLevel_chpl;
#line 107 "/Users/iainmoncrief/Documents/chapel/modules/standard/AutoMath.chpl"
  *(refIndentLevel_chpl) += INT64(1);
#line 107 "/Users/iainmoncrief/Documents/chapel/modules/standard/AutoMath.chpl"
  chpl__init_AutoMath_p = UINT8(true);
#line 107 "/Users/iainmoncrief/Documents/chapel/modules/standard/AutoMath.chpl"
  {
#line 107 "/Users/iainmoncrief/Documents/chapel/modules/standard/AutoMath.chpl"
    chpl__init_HaltWrappers(_ln_chpl, _fn_chpl);
#line 107 "/Users/iainmoncrief/Documents/chapel/modules/standard/AutoMath.chpl"
    chpl__init_CTypes(_ln_chpl, _fn_chpl);
#line 107 "/Users/iainmoncrief/Documents/chapel/modules/standard/AutoMath.chpl"
  }
#line 107 "/Users/iainmoncrief/Documents/chapel/modules/standard/AutoMath.chpl"
  *(refIndentLevel_chpl) -= INT64(1);
#line 107 "/Users/iainmoncrief/Documents/chapel/modules/standard/AutoMath.chpl"
  _exit_chpl__init_AutoMath_chpl:;
#line 107 "/Users/iainmoncrief/Documents/chapel/modules/standard/AutoMath.chpl"
  return;
#line 107 "/Users/iainmoncrief/Documents/chapel/modules/standard/AutoMath.chpl"
}

