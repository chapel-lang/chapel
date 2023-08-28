#line 186 "/Users/iainmoncrief/Documents/chapel/modules/standard/CommDiagnostics.chpl"
/* CommDiagnostics.chpl:186 */
#line 186 "/Users/iainmoncrief/Documents/chapel/modules/standard/CommDiagnostics.chpl"
static void chpl__init_CommDiagnostics(int64_t _ln_chpl,
#line 186 "/Users/iainmoncrief/Documents/chapel/modules/standard/CommDiagnostics.chpl"
                                       int32_t _fn_chpl) {
#line 186 "/Users/iainmoncrief/Documents/chapel/modules/standard/CommDiagnostics.chpl"
  c_string_rehook modFormatStr_chpl;
#line 186 "/Users/iainmoncrief/Documents/chapel/modules/standard/CommDiagnostics.chpl"
  c_string_rehook modStr_chpl;
#line 186 "/Users/iainmoncrief/Documents/chapel/modules/standard/CommDiagnostics.chpl"
  _ref_int32_t_chpl refIndentLevel_chpl = NULL;
#line 186 "/Users/iainmoncrief/Documents/chapel/modules/standard/CommDiagnostics.chpl"
  if (chpl__init_CommDiagnostics_p) /* ZLINE: 186 /Users/iainmoncrief/Documents/chapel/modules/standard/CommDiagnostics.chpl */
#line 186 "/Users/iainmoncrief/Documents/chapel/modules/standard/CommDiagnostics.chpl"
  {
#line 186 "/Users/iainmoncrief/Documents/chapel/modules/standard/CommDiagnostics.chpl"
    goto _exit_chpl__init_CommDiagnostics_chpl;
#line 186 "/Users/iainmoncrief/Documents/chapel/modules/standard/CommDiagnostics.chpl"
  }
#line 186 "/Users/iainmoncrief/Documents/chapel/modules/standard/CommDiagnostics.chpl"
  modFormatStr_chpl = "%*s\n";
#line 186 "/Users/iainmoncrief/Documents/chapel/modules/standard/CommDiagnostics.chpl"
  modStr_chpl = "CommDiagnostics";
#line 186 "/Users/iainmoncrief/Documents/chapel/modules/standard/CommDiagnostics.chpl"
  printModuleInit_chpl(modFormatStr_chpl, modStr_chpl, INT64(15), _ln_chpl, _fn_chpl);
#line 186 "/Users/iainmoncrief/Documents/chapel/modules/standard/CommDiagnostics.chpl"
  refIndentLevel_chpl = &moduleInitLevel_chpl;
#line 186 "/Users/iainmoncrief/Documents/chapel/modules/standard/CommDiagnostics.chpl"
  *(refIndentLevel_chpl) += INT64(1);
#line 186 "/Users/iainmoncrief/Documents/chapel/modules/standard/CommDiagnostics.chpl"
  chpl__init_CommDiagnostics_p = UINT8(true);
#line 186 "/Users/iainmoncrief/Documents/chapel/modules/standard/CommDiagnostics.chpl"
  {
#line 186 "/Users/iainmoncrief/Documents/chapel/modules/standard/CommDiagnostics.chpl"
    chpl__init_Reflection(_ln_chpl, _fn_chpl);
#line 186 "/Users/iainmoncrief/Documents/chapel/modules/standard/CommDiagnostics.chpl"
    chpl__init_Math(_ln_chpl, _fn_chpl);
#line 186 "/Users/iainmoncrief/Documents/chapel/modules/standard/CommDiagnostics.chpl"
  }
#line 186 "/Users/iainmoncrief/Documents/chapel/modules/standard/CommDiagnostics.chpl"
  *(refIndentLevel_chpl) -= INT64(1);
#line 186 "/Users/iainmoncrief/Documents/chapel/modules/standard/CommDiagnostics.chpl"
  _endchpl__init_CommDiagnostics_chpl:;
#line 186 "/Users/iainmoncrief/Documents/chapel/modules/standard/CommDiagnostics.chpl"
  _exit_chpl__init_CommDiagnostics_chpl:;
#line 186 "/Users/iainmoncrief/Documents/chapel/modules/standard/CommDiagnostics.chpl"
  return;
#line 186 "/Users/iainmoncrief/Documents/chapel/modules/standard/CommDiagnostics.chpl"
}

