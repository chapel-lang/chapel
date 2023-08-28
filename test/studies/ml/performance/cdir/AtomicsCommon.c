#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/AtomicsCommon.chpl"
/* AtomicsCommon.chpl:21 */
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/AtomicsCommon.chpl"
static void chpl__init_AtomicsCommon(int64_t _ln_chpl,
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/AtomicsCommon.chpl"
                                     int32_t _fn_chpl) {
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/AtomicsCommon.chpl"
  c_string_rehook modFormatStr_chpl;
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/AtomicsCommon.chpl"
  c_string_rehook modStr_chpl;
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/AtomicsCommon.chpl"
  _ref_int32_t_chpl refIndentLevel_chpl = NULL;
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/AtomicsCommon.chpl"
  if (chpl__init_AtomicsCommon_p) /* ZLINE: 21 /Users/iainmoncrief/Documents/chapel/modules/internal/AtomicsCommon.chpl */
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/AtomicsCommon.chpl"
  {
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/AtomicsCommon.chpl"
    goto _exit_chpl__init_AtomicsCommon_chpl;
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/AtomicsCommon.chpl"
  }
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/AtomicsCommon.chpl"
  modFormatStr_chpl = "%*s\n";
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/AtomicsCommon.chpl"
  modStr_chpl = "AtomicsCommon";
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/AtomicsCommon.chpl"
  printModuleInit_chpl(modFormatStr_chpl, modStr_chpl, INT64(13), _ln_chpl, _fn_chpl);
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/AtomicsCommon.chpl"
  refIndentLevel_chpl = &moduleInitLevel_chpl;
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/AtomicsCommon.chpl"
  *(refIndentLevel_chpl) += INT64(1);
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/AtomicsCommon.chpl"
  chpl__init_AtomicsCommon_p = UINT8(true);
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/AtomicsCommon.chpl"
  *(refIndentLevel_chpl) -= INT64(1);
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/AtomicsCommon.chpl"
  _endchpl__init_AtomicsCommon_chpl:;
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/AtomicsCommon.chpl"
  _exit_chpl__init_AtomicsCommon_chpl:;
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/AtomicsCommon.chpl"
  return;
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/AtomicsCommon.chpl"
}

