#line 23 "/Users/iainmoncrief/Documents/chapel/modules/packages/Search.chpl"
/* Search.chpl:23 */
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/packages/Search.chpl"
static void chpl__init_Search(int64_t _ln_chpl,
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/packages/Search.chpl"
                              int32_t _fn_chpl) {
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/packages/Search.chpl"
  c_string_rehook modFormatStr_chpl;
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/packages/Search.chpl"
  c_string_rehook modStr_chpl;
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/packages/Search.chpl"
  _ref_int32_t_chpl refIndentLevel_chpl = NULL;
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/packages/Search.chpl"
  if (chpl__init_Search_p) /* ZLINE: 23 /Users/iainmoncrief/Documents/chapel/modules/packages/Search.chpl */
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/packages/Search.chpl"
  {
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/packages/Search.chpl"
    goto _exit_chpl__init_Search_chpl;
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/packages/Search.chpl"
  }
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/packages/Search.chpl"
  modFormatStr_chpl = "%*s\n";
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/packages/Search.chpl"
  modStr_chpl = "Search";
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/packages/Search.chpl"
  printModuleInit_chpl(modFormatStr_chpl, modStr_chpl, INT64(6), _ln_chpl, _fn_chpl);
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/packages/Search.chpl"
  refIndentLevel_chpl = &moduleInitLevel_chpl;
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/packages/Search.chpl"
  *(refIndentLevel_chpl) += INT64(1);
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/packages/Search.chpl"
  chpl__init_Search_p = UINT8(true);
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/packages/Search.chpl"
  {
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/packages/Search.chpl"
    chpl__init_Sort(_ln_chpl, _fn_chpl);
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/packages/Search.chpl"
  }
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/packages/Search.chpl"
  *(refIndentLevel_chpl) -= INT64(1);
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/packages/Search.chpl"
  _endchpl__init_Search_chpl:;
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/packages/Search.chpl"
  _exit_chpl__init_Search_chpl:;
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/packages/Search.chpl"
  return;
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/packages/Search.chpl"
}

