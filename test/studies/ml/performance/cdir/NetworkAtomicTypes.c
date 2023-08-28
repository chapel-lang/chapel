#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/NetworkAtomicTypes.chpl"
/* NetworkAtomicTypes.chpl:23 */
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/NetworkAtomicTypes.chpl"
static void chpl__init_NetworkAtomicTypes(int64_t _ln_chpl,
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/NetworkAtomicTypes.chpl"
                                          int32_t _fn_chpl) {
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/NetworkAtomicTypes.chpl"
  c_string_rehook modFormatStr_chpl;
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/NetworkAtomicTypes.chpl"
  c_string_rehook modStr_chpl;
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/NetworkAtomicTypes.chpl"
  _ref_int32_t_chpl refIndentLevel_chpl = NULL;
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/NetworkAtomicTypes.chpl"
  if (chpl__init_NetworkAtomicTypes_p) /* ZLINE: 23 /Users/iainmoncrief/Documents/chapel/modules/internal/NetworkAtomicTypes.chpl */
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/NetworkAtomicTypes.chpl"
  {
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/NetworkAtomicTypes.chpl"
    goto _exit_chpl__init_NetworkAtomicTypes_chpl;
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/NetworkAtomicTypes.chpl"
  }
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/NetworkAtomicTypes.chpl"
  modFormatStr_chpl = "%*s\n";
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/NetworkAtomicTypes.chpl"
  modStr_chpl = "NetworkAtomicTypes";
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/NetworkAtomicTypes.chpl"
  printModuleInit_chpl(modFormatStr_chpl, modStr_chpl, INT64(18), _ln_chpl, _fn_chpl);
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/NetworkAtomicTypes.chpl"
  refIndentLevel_chpl = &moduleInitLevel_chpl;
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/NetworkAtomicTypes.chpl"
  *(refIndentLevel_chpl) += INT64(1);
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/NetworkAtomicTypes.chpl"
  chpl__init_NetworkAtomicTypes_p = UINT8(true);
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/NetworkAtomicTypes.chpl"
  *(refIndentLevel_chpl) -= INT64(1);
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/NetworkAtomicTypes.chpl"
  _endchpl__init_NetworkAtomicTypes_chpl:;
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/NetworkAtomicTypes.chpl"
  _exit_chpl__init_NetworkAtomicTypes_chpl:;
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/NetworkAtomicTypes.chpl"
  return;
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/NetworkAtomicTypes.chpl"
}

