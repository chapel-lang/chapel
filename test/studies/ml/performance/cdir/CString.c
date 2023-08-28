#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/CString.chpl"
/* CString.chpl:26 */
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/CString.chpl"
static void chpl__init_CString(int64_t _ln_chpl,
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/CString.chpl"
                               int32_t _fn_chpl) {
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/CString.chpl"
  c_string_rehook modFormatStr_chpl;
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/CString.chpl"
  c_string_rehook modStr_chpl;
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/CString.chpl"
  _ref_int32_t_chpl refIndentLevel_chpl = NULL;
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/CString.chpl"
  if (chpl__init_CString_p) /* ZLINE: 26 /Users/iainmoncrief/Documents/chapel/modules/internal/CString.chpl */
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/CString.chpl"
  {
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/CString.chpl"
    goto _exit_chpl__init_CString_chpl;
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/CString.chpl"
  }
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/CString.chpl"
  modFormatStr_chpl = "%*s\n";
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/CString.chpl"
  modStr_chpl = "CString";
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/CString.chpl"
  printModuleInit_chpl(modFormatStr_chpl, modStr_chpl, INT64(7), _ln_chpl, _fn_chpl);
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/CString.chpl"
  refIndentLevel_chpl = &moduleInitLevel_chpl;
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/CString.chpl"
  *(refIndentLevel_chpl) += INT64(1);
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/CString.chpl"
  chpl__init_CString_p = UINT8(true);
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/CString.chpl"
  {
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/CString.chpl"
    chpl__init_CTypes(_ln_chpl, _fn_chpl);
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/CString.chpl"
  }
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/CString.chpl"
  *(refIndentLevel_chpl) -= INT64(1);
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/CString.chpl"
  _endchpl__init_CString_chpl:;
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/CString.chpl"
  _exit_chpl__init_CString_chpl:;
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/CString.chpl"
  return;
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/CString.chpl"
}

#line 33 "/Users/iainmoncrief/Documents/chapel/modules/internal/CString.chpl"
/* CString.chpl:33 */
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/internal/CString.chpl"
static c_string_rehook chpl__initCopy(c_string_rehook x_chpl,
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/internal/CString.chpl"
                                      chpl_bool definedConst_chpl) {
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/internal/CString.chpl"
  c_string_rehook ret_chpl;
#line 34 "/Users/iainmoncrief/Documents/chapel/modules/internal/CString.chpl"
  ret_chpl = x_chpl;
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/internal/CString.chpl"
  return ret_chpl;
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/internal/CString.chpl"
}

#line 105 "/Users/iainmoncrief/Documents/chapel/modules/internal/CString.chpl"
/* CString.chpl:105 */
#line 105 "/Users/iainmoncrief/Documents/chapel/modules/internal/CString.chpl"
static void * chpl___COLON_(c_string_rehook x_chpl,
#line 105 "/Users/iainmoncrief/Documents/chapel/modules/internal/CString.chpl"
                            int64_t _ln_chpl,
#line 105 "/Users/iainmoncrief/Documents/chapel/modules/internal/CString.chpl"
                            int32_t _fn_chpl) {
#line 105 "/Users/iainmoncrief/Documents/chapel/modules/internal/CString.chpl"
  c_ptr_void_chpl _formal_type_tmp__chpl = NULL;
#line 105 "/Users/iainmoncrief/Documents/chapel/modules/internal/CString.chpl"
  c_ptr_void_chpl ret_chpl = NULL;
#line 106 "/Users/iainmoncrief/Documents/chapel/modules/internal/CString.chpl"
  c_ptr_void_chpl call_tmp_chpl6 = NULL;
#line 106 "/Users/iainmoncrief/Documents/chapel/modules/internal/CString.chpl"
  call_tmp_chpl6 = ((c_ptr_void_chpl)(x_chpl));
#line 106 "/Users/iainmoncrief/Documents/chapel/modules/internal/CString.chpl"
  ret_chpl = call_tmp_chpl6;
#line 105 "/Users/iainmoncrief/Documents/chapel/modules/internal/CString.chpl"
  return ret_chpl;
#line 105 "/Users/iainmoncrief/Documents/chapel/modules/internal/CString.chpl"
}

