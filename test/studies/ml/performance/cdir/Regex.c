#line 338 "/Users/iainmoncrief/Documents/chapel/modules/standard/Regex.chpl"
/* Regex.chpl:338 */
#line 338 "/Users/iainmoncrief/Documents/chapel/modules/standard/Regex.chpl"
static void chpl__init_Regex(int64_t _ln_chpl,
#line 338 "/Users/iainmoncrief/Documents/chapel/modules/standard/Regex.chpl"
                             int32_t _fn_chpl) {
#line 338 "/Users/iainmoncrief/Documents/chapel/modules/standard/Regex.chpl"
  _ref_int32_t_chpl refIndentLevel_chpl = NULL;
#line 338 "/Users/iainmoncrief/Documents/chapel/modules/standard/Regex.chpl"
  if (chpl__init_Regex_p) /* ZLINE: 338 /Users/iainmoncrief/Documents/chapel/modules/standard/Regex.chpl */
#line 338 "/Users/iainmoncrief/Documents/chapel/modules/standard/Regex.chpl"
  {
#line 338 "/Users/iainmoncrief/Documents/chapel/modules/standard/Regex.chpl"
    goto _exit_chpl__init_Regex_chpl;
#line 338 "/Users/iainmoncrief/Documents/chapel/modules/standard/Regex.chpl"
  }
#line 338 "/Users/iainmoncrief/Documents/chapel/modules/standard/Regex.chpl"
  printModuleInit_chpl("%*s\n", "Regex", INT64(5), _ln_chpl, _fn_chpl);
#line 338 "/Users/iainmoncrief/Documents/chapel/modules/standard/Regex.chpl"
  refIndentLevel_chpl = &moduleInitLevel_chpl;
#line 338 "/Users/iainmoncrief/Documents/chapel/modules/standard/Regex.chpl"
  *(refIndentLevel_chpl) += INT64(1);
#line 338 "/Users/iainmoncrief/Documents/chapel/modules/standard/Regex.chpl"
  chpl__init_Regex_p = UINT8(true);
#line 338 "/Users/iainmoncrief/Documents/chapel/modules/standard/Regex.chpl"
  {
#line 338 "/Users/iainmoncrief/Documents/chapel/modules/standard/Regex.chpl"
    chpl__init_OS(_ln_chpl, _fn_chpl);
#line 338 "/Users/iainmoncrief/Documents/chapel/modules/standard/Regex.chpl"
    chpl__init_CTypes(_ln_chpl, _fn_chpl);
#line 338 "/Users/iainmoncrief/Documents/chapel/modules/standard/Regex.chpl"
    chpl__init_IO(_ln_chpl, _fn_chpl);
#line 338 "/Users/iainmoncrief/Documents/chapel/modules/standard/Regex.chpl"
    chpl__init_ByteBufferHelpers(_ln_chpl, _fn_chpl);
#line 338 "/Users/iainmoncrief/Documents/chapel/modules/standard/Regex.chpl"
    chpl__init_RegexIoSupport(_ln_chpl, _fn_chpl);
#line 338 "/Users/iainmoncrief/Documents/chapel/modules/standard/Regex.chpl"
    chpl__init_String(_ln_chpl, _fn_chpl);
#line 338 "/Users/iainmoncrief/Documents/chapel/modules/standard/Regex.chpl"
  }
#line 344 "/Users/iainmoncrief/Documents/chapel/modules/standard/Regex.chpl"
  initBufferSizeForSlowReplaceAndCount_chpl = INT64(16);
#line 338 "/Users/iainmoncrief/Documents/chapel/modules/standard/Regex.chpl"
  *(refIndentLevel_chpl) -= INT64(1);
#line 338 "/Users/iainmoncrief/Documents/chapel/modules/standard/Regex.chpl"
  _exit_chpl__init_Regex_chpl:;
#line 338 "/Users/iainmoncrief/Documents/chapel/modules/standard/Regex.chpl"
  return;
#line 338 "/Users/iainmoncrief/Documents/chapel/modules/standard/Regex.chpl"
}

