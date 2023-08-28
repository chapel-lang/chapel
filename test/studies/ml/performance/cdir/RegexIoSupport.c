#line 1669 "/Users/iainmoncrief/Documents/chapel/modules/standard/Regex.chpl"
/* Regex.chpl:1669 */
#line 1669 "/Users/iainmoncrief/Documents/chapel/modules/standard/Regex.chpl"
static void chpl__init_RegexIoSupport(int64_t _ln_chpl,
#line 1669 "/Users/iainmoncrief/Documents/chapel/modules/standard/Regex.chpl"
                                      int32_t _fn_chpl) {
#line 1669 "/Users/iainmoncrief/Documents/chapel/modules/standard/Regex.chpl"
  _ref_int32_t_chpl refIndentLevel_chpl = NULL;
#line 1669 "/Users/iainmoncrief/Documents/chapel/modules/standard/Regex.chpl"
  if (chpl__init_RegexIoSupport_p) /* ZLINE: 1669 /Users/iainmoncrief/Documents/chapel/modules/standard/Regex.chpl */
#line 1669 "/Users/iainmoncrief/Documents/chapel/modules/standard/Regex.chpl"
  {
#line 1669 "/Users/iainmoncrief/Documents/chapel/modules/standard/Regex.chpl"
    goto _exit_chpl__init_RegexIoSupport_chpl;
#line 1669 "/Users/iainmoncrief/Documents/chapel/modules/standard/Regex.chpl"
  }
#line 1669 "/Users/iainmoncrief/Documents/chapel/modules/standard/Regex.chpl"
  printModuleInit_chpl("%*s\n", "RegexIoSupport", INT64(14), _ln_chpl, _fn_chpl);
#line 1669 "/Users/iainmoncrief/Documents/chapel/modules/standard/Regex.chpl"
  refIndentLevel_chpl = &moduleInitLevel_chpl;
#line 1669 "/Users/iainmoncrief/Documents/chapel/modules/standard/Regex.chpl"
  *(refIndentLevel_chpl) += INT64(1);
#line 1669 "/Users/iainmoncrief/Documents/chapel/modules/standard/Regex.chpl"
  chpl__init_RegexIoSupport_p = UINT8(true);
#line 1669 "/Users/iainmoncrief/Documents/chapel/modules/standard/Regex.chpl"
  {
#line 1669 "/Users/iainmoncrief/Documents/chapel/modules/standard/Regex.chpl"
    chpl__init_Regex(_ln_chpl, _fn_chpl);
#line 1669 "/Users/iainmoncrief/Documents/chapel/modules/standard/Regex.chpl"
    chpl__init_CTypes(_ln_chpl, _fn_chpl);
#line 1669 "/Users/iainmoncrief/Documents/chapel/modules/standard/Regex.chpl"
    chpl__init_IO(_ln_chpl, _fn_chpl);
#line 1669 "/Users/iainmoncrief/Documents/chapel/modules/standard/Regex.chpl"
    chpl__init_Regex(_ln_chpl, _fn_chpl);
#line 1669 "/Users/iainmoncrief/Documents/chapel/modules/standard/Regex.chpl"
  }
#line 1669 "/Users/iainmoncrief/Documents/chapel/modules/standard/Regex.chpl"
  *(refIndentLevel_chpl) -= INT64(1);
#line 1669 "/Users/iainmoncrief/Documents/chapel/modules/standard/Regex.chpl"
  _exit_chpl__init_RegexIoSupport_chpl:;
#line 1669 "/Users/iainmoncrief/Documents/chapel/modules/standard/Regex.chpl"
  return;
#line 1669 "/Users/iainmoncrief/Documents/chapel/modules/standard/Regex.chpl"
}

