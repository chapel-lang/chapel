#line 25 "/Users/iainmoncrief/Documents/chapel/modules/standard/HaltWrappers.chpl"
/* HaltWrappers.chpl:25 */
#line 25 "/Users/iainmoncrief/Documents/chapel/modules/standard/HaltWrappers.chpl"
static void chpl__init_HaltWrappers(int64_t _ln_chpl,
#line 25 "/Users/iainmoncrief/Documents/chapel/modules/standard/HaltWrappers.chpl"
                                    int32_t _fn_chpl) {
#line 25 "/Users/iainmoncrief/Documents/chapel/modules/standard/HaltWrappers.chpl"
  _ref_int32_t_chpl refIndentLevel_chpl = NULL;
#line 25 "/Users/iainmoncrief/Documents/chapel/modules/standard/HaltWrappers.chpl"
  if (chpl__init_HaltWrappers_p) /* ZLINE: 25 /Users/iainmoncrief/Documents/chapel/modules/standard/HaltWrappers.chpl */
#line 25 "/Users/iainmoncrief/Documents/chapel/modules/standard/HaltWrappers.chpl"
  {
#line 25 "/Users/iainmoncrief/Documents/chapel/modules/standard/HaltWrappers.chpl"
    goto _exit_chpl__init_HaltWrappers_chpl;
#line 25 "/Users/iainmoncrief/Documents/chapel/modules/standard/HaltWrappers.chpl"
  }
#line 25 "/Users/iainmoncrief/Documents/chapel/modules/standard/HaltWrappers.chpl"
  printModuleInit_chpl("%*s\n", "HaltWrappers", INT64(12), _ln_chpl, _fn_chpl);
#line 25 "/Users/iainmoncrief/Documents/chapel/modules/standard/HaltWrappers.chpl"
  refIndentLevel_chpl = &moduleInitLevel_chpl;
#line 25 "/Users/iainmoncrief/Documents/chapel/modules/standard/HaltWrappers.chpl"
  *(refIndentLevel_chpl) += INT64(1);
#line 25 "/Users/iainmoncrief/Documents/chapel/modules/standard/HaltWrappers.chpl"
  chpl__init_HaltWrappers_p = UINT8(true);
#line 25 "/Users/iainmoncrief/Documents/chapel/modules/standard/HaltWrappers.chpl"
  *(refIndentLevel_chpl) -= INT64(1);
#line 25 "/Users/iainmoncrief/Documents/chapel/modules/standard/HaltWrappers.chpl"
  _exit_chpl__init_HaltWrappers_chpl:;
#line 25 "/Users/iainmoncrief/Documents/chapel/modules/standard/HaltWrappers.chpl"
  return;
#line 25 "/Users/iainmoncrief/Documents/chapel/modules/standard/HaltWrappers.chpl"
}

#line 39 "/Users/iainmoncrief/Documents/chapel/modules/standard/HaltWrappers.chpl"
/* HaltWrappers.chpl:39 */
#line 39 "/Users/iainmoncrief/Documents/chapel/modules/standard/HaltWrappers.chpl"
static void initHalt_chpl(string_chpl * s_chpl,
#line 39 "/Users/iainmoncrief/Documents/chapel/modules/standard/HaltWrappers.chpl"
                          int64_t _ln_chpl,
#line 39 "/Users/iainmoncrief/Documents/chapel/modules/standard/HaltWrappers.chpl"
                          int32_t _fn_chpl) {
#line 40 "/Users/iainmoncrief/Documents/chapel/modules/standard/HaltWrappers.chpl"
  halt_chpl(s_chpl, _ln_chpl, _fn_chpl);
#line 39 "/Users/iainmoncrief/Documents/chapel/modules/standard/HaltWrappers.chpl"
  return;
#line 39 "/Users/iainmoncrief/Documents/chapel/modules/standard/HaltWrappers.chpl"
}

#line 66 "/Users/iainmoncrief/Documents/chapel/modules/standard/HaltWrappers.chpl"
/* HaltWrappers.chpl:66 */
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/standard/HaltWrappers.chpl"
static void pureVirtualMethodHalt_chpl(int64_t _ln_chpl,
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/standard/HaltWrappers.chpl"
                                       int32_t _fn_chpl) {
#line 67 "/Users/iainmoncrief/Documents/chapel/modules/standard/HaltWrappers.chpl"
  halt_chpl(&_str_literal_2433_chpl, _ln_chpl, _fn_chpl);
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/standard/HaltWrappers.chpl"
  return;
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/standard/HaltWrappers.chpl"
}

#line 76 "/Users/iainmoncrief/Documents/chapel/modules/standard/HaltWrappers.chpl"
/* HaltWrappers.chpl:76 */
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/standard/HaltWrappers.chpl"
static void exhaustiveSelectHalt_chpl(string_chpl * s_chpl,
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/standard/HaltWrappers.chpl"
                                      int64_t _ln_chpl,
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/standard/HaltWrappers.chpl"
                                      int32_t _fn_chpl) {
#line 77 "/Users/iainmoncrief/Documents/chapel/modules/standard/HaltWrappers.chpl"
  halt_chpl(s_chpl, _ln_chpl, _fn_chpl);
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/standard/HaltWrappers.chpl"
  return;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/standard/HaltWrappers.chpl"
}

#line 110 "/Users/iainmoncrief/Documents/chapel/modules/standard/HaltWrappers.chpl"
/* HaltWrappers.chpl:110 */
#line 110 "/Users/iainmoncrief/Documents/chapel/modules/standard/HaltWrappers.chpl"
static void nilCheckHalt_chpl(string_chpl * s_chpl,
#line 110 "/Users/iainmoncrief/Documents/chapel/modules/standard/HaltWrappers.chpl"
                              int64_t _ln_chpl,
#line 110 "/Users/iainmoncrief/Documents/chapel/modules/standard/HaltWrappers.chpl"
                              int32_t _fn_chpl) {
#line 111 "/Users/iainmoncrief/Documents/chapel/modules/standard/HaltWrappers.chpl"
  halt_chpl(s_chpl, _ln_chpl, _fn_chpl);
#line 110 "/Users/iainmoncrief/Documents/chapel/modules/standard/HaltWrappers.chpl"
  return;
#line 110 "/Users/iainmoncrief/Documents/chapel/modules/standard/HaltWrappers.chpl"
}

#line 117 "/Users/iainmoncrief/Documents/chapel/modules/standard/HaltWrappers.chpl"
/* HaltWrappers.chpl:117 */
#line 117 "/Users/iainmoncrief/Documents/chapel/modules/standard/HaltWrappers.chpl"
static void boundsCheckHalt_chpl(string_chpl * s_chpl,
#line 117 "/Users/iainmoncrief/Documents/chapel/modules/standard/HaltWrappers.chpl"
                                 int64_t _ln_chpl,
#line 117 "/Users/iainmoncrief/Documents/chapel/modules/standard/HaltWrappers.chpl"
                                 int32_t _fn_chpl) {
#line 118 "/Users/iainmoncrief/Documents/chapel/modules/standard/HaltWrappers.chpl"
  halt_chpl(s_chpl, _ln_chpl, _fn_chpl);
#line 117 "/Users/iainmoncrief/Documents/chapel/modules/standard/HaltWrappers.chpl"
  return;
#line 117 "/Users/iainmoncrief/Documents/chapel/modules/standard/HaltWrappers.chpl"
}

#line 138 "/Users/iainmoncrief/Documents/chapel/modules/standard/HaltWrappers.chpl"
/* HaltWrappers.chpl:138 */
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/standard/HaltWrappers.chpl"
static void safeCastCheckHalt_chpl(string_chpl * s_chpl,
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/standard/HaltWrappers.chpl"
                                   int64_t _ln_chpl,
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/standard/HaltWrappers.chpl"
                                   int32_t _fn_chpl) {
#line 139 "/Users/iainmoncrief/Documents/chapel/modules/standard/HaltWrappers.chpl"
  halt_chpl(s_chpl, _ln_chpl, _fn_chpl);
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/standard/HaltWrappers.chpl"
  return;
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/standard/HaltWrappers.chpl"
}

