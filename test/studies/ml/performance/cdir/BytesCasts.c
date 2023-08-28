#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesCasts.chpl"
/* BytesCasts.chpl:21 */
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesCasts.chpl"
static void chpl__init_BytesCasts(int64_t _ln_chpl,
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesCasts.chpl"
                                  int32_t _fn_chpl) {
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesCasts.chpl"
  c_string_rehook modFormatStr_chpl;
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesCasts.chpl"
  c_string_rehook modStr_chpl;
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesCasts.chpl"
  _ref_int32_t_chpl refIndentLevel_chpl = NULL;
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesCasts.chpl"
  if (chpl__init_BytesCasts_p) /* ZLINE: 21 /Users/iainmoncrief/Documents/chapel/modules/internal/BytesCasts.chpl */
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesCasts.chpl"
  {
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesCasts.chpl"
    goto _exit_chpl__init_BytesCasts_chpl;
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesCasts.chpl"
  }
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesCasts.chpl"
  modFormatStr_chpl = "%*s\n";
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesCasts.chpl"
  modStr_chpl = "BytesCasts";
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesCasts.chpl"
  printModuleInit_chpl(modFormatStr_chpl, modStr_chpl, INT64(10), _ln_chpl, _fn_chpl);
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesCasts.chpl"
  refIndentLevel_chpl = &moduleInitLevel_chpl;
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesCasts.chpl"
  *(refIndentLevel_chpl) += INT64(1);
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesCasts.chpl"
  chpl__init_BytesCasts_p = UINT8(true);
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesCasts.chpl"
  {
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesCasts.chpl"
    chpl__init_BytesStringCommon(_ln_chpl, _fn_chpl);
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesCasts.chpl"
    chpl__init_CTypes(_ln_chpl, _fn_chpl);
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesCasts.chpl"
  }
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesCasts.chpl"
  *(refIndentLevel_chpl) -= INT64(1);
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesCasts.chpl"
  _endchpl__init_BytesCasts_chpl:;
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesCasts.chpl"
  _exit_chpl__init_BytesCasts_chpl:;
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesCasts.chpl"
  return;
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/BytesCasts.chpl"
}

