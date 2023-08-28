#line 22 "/Users/iainmoncrief/Documents/chapel/modules/internal/NetworkAtomics.chpl"
/* NetworkAtomics.chpl:22 */
#line 22 "/Users/iainmoncrief/Documents/chapel/modules/internal/NetworkAtomics.chpl"
static void chpl__init_NetworkAtomics(int64_t _ln_chpl,
#line 22 "/Users/iainmoncrief/Documents/chapel/modules/internal/NetworkAtomics.chpl"
                                      int32_t _fn_chpl) {
#line 22 "/Users/iainmoncrief/Documents/chapel/modules/internal/NetworkAtomics.chpl"
  c_string_rehook modFormatStr_chpl;
#line 22 "/Users/iainmoncrief/Documents/chapel/modules/internal/NetworkAtomics.chpl"
  c_string_rehook modStr_chpl;
#line 22 "/Users/iainmoncrief/Documents/chapel/modules/internal/NetworkAtomics.chpl"
  _ref_int32_t_chpl refIndentLevel_chpl = NULL;
#line 22 "/Users/iainmoncrief/Documents/chapel/modules/internal/NetworkAtomics.chpl"
  if (chpl__init_NetworkAtomics_p) /* ZLINE: 22 /Users/iainmoncrief/Documents/chapel/modules/internal/NetworkAtomics.chpl */
#line 22 "/Users/iainmoncrief/Documents/chapel/modules/internal/NetworkAtomics.chpl"
  {
#line 22 "/Users/iainmoncrief/Documents/chapel/modules/internal/NetworkAtomics.chpl"
    goto _exit_chpl__init_NetworkAtomics_chpl;
#line 22 "/Users/iainmoncrief/Documents/chapel/modules/internal/NetworkAtomics.chpl"
  }
#line 22 "/Users/iainmoncrief/Documents/chapel/modules/internal/NetworkAtomics.chpl"
  modFormatStr_chpl = "%*s\n";
#line 22 "/Users/iainmoncrief/Documents/chapel/modules/internal/NetworkAtomics.chpl"
  modStr_chpl = "NetworkAtomics";
#line 22 "/Users/iainmoncrief/Documents/chapel/modules/internal/NetworkAtomics.chpl"
  printModuleInit_chpl(modFormatStr_chpl, modStr_chpl, INT64(14), _ln_chpl, _fn_chpl);
#line 22 "/Users/iainmoncrief/Documents/chapel/modules/internal/NetworkAtomics.chpl"
  refIndentLevel_chpl = &moduleInitLevel_chpl;
#line 22 "/Users/iainmoncrief/Documents/chapel/modules/internal/NetworkAtomics.chpl"
  *(refIndentLevel_chpl) += INT64(1);
#line 22 "/Users/iainmoncrief/Documents/chapel/modules/internal/NetworkAtomics.chpl"
  chpl__init_NetworkAtomics_p = UINT8(true);
#line 22 "/Users/iainmoncrief/Documents/chapel/modules/internal/NetworkAtomics.chpl"
  {
#line 22 "/Users/iainmoncrief/Documents/chapel/modules/internal/NetworkAtomics.chpl"
    chpl__init_MemConsistency(_ln_chpl, _fn_chpl);
#line 22 "/Users/iainmoncrief/Documents/chapel/modules/internal/NetworkAtomics.chpl"
    chpl__init_CTypes(_ln_chpl, _fn_chpl);
#line 22 "/Users/iainmoncrief/Documents/chapel/modules/internal/NetworkAtomics.chpl"
    chpl__init_currentTask(_ln_chpl, _fn_chpl);
#line 22 "/Users/iainmoncrief/Documents/chapel/modules/internal/NetworkAtomics.chpl"
  }
#line 22 "/Users/iainmoncrief/Documents/chapel/modules/internal/NetworkAtomics.chpl"
  *(refIndentLevel_chpl) -= INT64(1);
#line 22 "/Users/iainmoncrief/Documents/chapel/modules/internal/NetworkAtomics.chpl"
  _endchpl__init_NetworkAtomics_chpl:;
#line 22 "/Users/iainmoncrief/Documents/chapel/modules/internal/NetworkAtomics.chpl"
  _exit_chpl__init_NetworkAtomics_chpl:;
#line 22 "/Users/iainmoncrief/Documents/chapel/modules/internal/NetworkAtomics.chpl"
  return;
#line 22 "/Users/iainmoncrief/Documents/chapel/modules/internal/NetworkAtomics.chpl"
}

