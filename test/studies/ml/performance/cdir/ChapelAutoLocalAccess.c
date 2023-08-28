#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelAutoLocalAccess.chpl"
/* ChapelAutoLocalAccess.chpl:21 */
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelAutoLocalAccess.chpl"
static void chpl__init_ChapelAutoLocalAccess(int64_t _ln_chpl,
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelAutoLocalAccess.chpl"
                                             int32_t _fn_chpl) {
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelAutoLocalAccess.chpl"
  c_string_rehook modFormatStr_chpl;
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelAutoLocalAccess.chpl"
  c_string_rehook modStr_chpl;
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelAutoLocalAccess.chpl"
  _ref_int32_t_chpl refIndentLevel_chpl = NULL;
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelAutoLocalAccess.chpl"
  if (chpl__init_ChapelAutoLocalAccess_p) /* ZLINE: 21 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelAutoLocalAccess.chpl */
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelAutoLocalAccess.chpl"
  {
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelAutoLocalAccess.chpl"
    goto _exit_chpl__init_ChapelAutoLocalAccess_chpl;
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelAutoLocalAccess.chpl"
  }
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelAutoLocalAccess.chpl"
  modFormatStr_chpl = "%*s\n";
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelAutoLocalAccess.chpl"
  modStr_chpl = "ChapelAutoLocalAccess";
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelAutoLocalAccess.chpl"
  printModuleInit_chpl(modFormatStr_chpl, modStr_chpl, INT64(21), _ln_chpl, _fn_chpl);
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelAutoLocalAccess.chpl"
  refIndentLevel_chpl = &moduleInitLevel_chpl;
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelAutoLocalAccess.chpl"
  *(refIndentLevel_chpl) += INT64(1);
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelAutoLocalAccess.chpl"
  chpl__init_ChapelAutoLocalAccess_p = UINT8(true);
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelAutoLocalAccess.chpl"
  {
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelAutoLocalAccess.chpl"
    chpl__init_ChapelLocale(_ln_chpl, _fn_chpl);
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelAutoLocalAccess.chpl"
  }
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelAutoLocalAccess.chpl"
  *(refIndentLevel_chpl) -= INT64(1);
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelAutoLocalAccess.chpl"
  _endchpl__init_ChapelAutoLocalAccess_chpl:;
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelAutoLocalAccess.chpl"
  _exit_chpl__init_ChapelAutoLocalAccess_chpl:;
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelAutoLocalAccess.chpl"
  return;
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelAutoLocalAccess.chpl"
}

