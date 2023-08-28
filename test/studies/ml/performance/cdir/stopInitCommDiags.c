#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/stopInitCommDiags.chpl"
/* stopInitCommDiags.chpl:26 */
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/stopInitCommDiags.chpl"
static void chpl__init_stopInitCommDiags(int64_t _ln_chpl,
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/stopInitCommDiags.chpl"
                                         int32_t _fn_chpl) {
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/stopInitCommDiags.chpl"
  c_string_rehook modFormatStr_chpl;
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/stopInitCommDiags.chpl"
  c_string_rehook modStr_chpl;
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/stopInitCommDiags.chpl"
  _ref_int32_t_chpl refIndentLevel_chpl = NULL;
#line 30 "/Users/iainmoncrief/Documents/chapel/modules/internal/stopInitCommDiags.chpl"
  chpl_bool call_tmp_chpl6;
#line 34 "/Users/iainmoncrief/Documents/chapel/modules/internal/stopInitCommDiags.chpl"
  chpl_bool call_tmp_chpl7;
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/stopInitCommDiags.chpl"
  if (chpl__init_stopInitCommDiags_p) /* ZLINE: 26 /Users/iainmoncrief/Documents/chapel/modules/internal/stopInitCommDiags.chpl */
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/stopInitCommDiags.chpl"
  {
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/stopInitCommDiags.chpl"
    goto _exit_chpl__init_stopInitCommDiags_chpl;
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/stopInitCommDiags.chpl"
  }
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/stopInitCommDiags.chpl"
  modFormatStr_chpl = "%*s\n";
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/stopInitCommDiags.chpl"
  modStr_chpl = "stopInitCommDiags";
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/stopInitCommDiags.chpl"
  printModuleInit_chpl(modFormatStr_chpl, modStr_chpl, INT64(17), _ln_chpl, _fn_chpl);
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/stopInitCommDiags.chpl"
  refIndentLevel_chpl = &moduleInitLevel_chpl;
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/stopInitCommDiags.chpl"
  *(refIndentLevel_chpl) += INT64(1);
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/stopInitCommDiags.chpl"
  chpl__init_stopInitCommDiags_p = UINT8(true);
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/stopInitCommDiags.chpl"
  {
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/stopInitCommDiags.chpl"
    chpl__init_ChapelBase(_ln_chpl, _fn_chpl);
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/stopInitCommDiags.chpl"
    chpl__init_CommDiagnostics(_ln_chpl, _fn_chpl);
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/stopInitCommDiags.chpl"
  }
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/stopInitCommDiags.chpl"
  *(refIndentLevel_chpl) -= INT64(1);
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/stopInitCommDiags.chpl"
  _endchpl__init_stopInitCommDiags_chpl:;
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/stopInitCommDiags.chpl"
  _exit_chpl__init_stopInitCommDiags_chpl:;
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/stopInitCommDiags.chpl"
  return;
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/stopInitCommDiags.chpl"
}

