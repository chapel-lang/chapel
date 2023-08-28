#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/startInitCommDiags.chpl"
/* startInitCommDiags.chpl:26 */
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/startInitCommDiags.chpl"
static void chpl__init_startInitCommDiags(int64_t _ln_chpl,
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/startInitCommDiags.chpl"
                                          int32_t _fn_chpl) {
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/startInitCommDiags.chpl"
  c_string_rehook modFormatStr_chpl;
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/startInitCommDiags.chpl"
  c_string_rehook modStr_chpl;
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/startInitCommDiags.chpl"
  _ref_int32_t_chpl refIndentLevel_chpl = NULL;
#line 30 "/Users/iainmoncrief/Documents/chapel/modules/internal/startInitCommDiags.chpl"
  chpl_bool call_tmp_chpl6;
#line 34 "/Users/iainmoncrief/Documents/chapel/modules/internal/startInitCommDiags.chpl"
  chpl_bool call_tmp_chpl7;
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/startInitCommDiags.chpl"
  if (chpl__init_startInitCommDiags_p) /* ZLINE: 26 /Users/iainmoncrief/Documents/chapel/modules/internal/startInitCommDiags.chpl */
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/startInitCommDiags.chpl"
  {
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/startInitCommDiags.chpl"
    goto _exit_chpl__init_startInitCommDiags_chpl;
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/startInitCommDiags.chpl"
  }
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/startInitCommDiags.chpl"
  modFormatStr_chpl = "%*s\n";
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/startInitCommDiags.chpl"
  modStr_chpl = "startInitCommDiags";
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/startInitCommDiags.chpl"
  printModuleInit_chpl(modFormatStr_chpl, modStr_chpl, INT64(18), _ln_chpl, _fn_chpl);
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/startInitCommDiags.chpl"
  refIndentLevel_chpl = &moduleInitLevel_chpl;
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/startInitCommDiags.chpl"
  *(refIndentLevel_chpl) += INT64(1);
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/startInitCommDiags.chpl"
  chpl__init_startInitCommDiags_p = UINT8(true);
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/startInitCommDiags.chpl"
  {
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/startInitCommDiags.chpl"
    chpl__init_ChapelBase(_ln_chpl, _fn_chpl);
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/startInitCommDiags.chpl"
    chpl__init_CommDiagnostics(_ln_chpl, _fn_chpl);
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/startInitCommDiags.chpl"
  }
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/startInitCommDiags.chpl"
  *(refIndentLevel_chpl) -= INT64(1);
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/startInitCommDiags.chpl"
  _endchpl__init_startInitCommDiags_chpl:;
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/startInitCommDiags.chpl"
  _exit_chpl__init_startInitCommDiags_chpl:;
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/startInitCommDiags.chpl"
  return;
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/startInitCommDiags.chpl"
}

