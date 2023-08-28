#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewReindex.chpl"
/* ArrayViewReindex.chpl:26 */
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewReindex.chpl"
static void chpl__init_ArrayViewReindex(int64_t _ln_chpl,
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewReindex.chpl"
                                        int32_t _fn_chpl) {
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewReindex.chpl"
  c_string_rehook modFormatStr_chpl;
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewReindex.chpl"
  c_string_rehook modStr_chpl;
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewReindex.chpl"
  _ref_int32_t_chpl refIndentLevel_chpl = NULL;
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewReindex.chpl"
  if (chpl__init_ArrayViewReindex_p) /* ZLINE: 26 /Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewReindex.chpl */
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewReindex.chpl"
  {
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewReindex.chpl"
    goto _exit_chpl__init_ArrayViewReindex_chpl;
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewReindex.chpl"
  }
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewReindex.chpl"
  modFormatStr_chpl = "%*s\n";
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewReindex.chpl"
  modStr_chpl = "ArrayViewReindex";
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewReindex.chpl"
  printModuleInit_chpl(modFormatStr_chpl, modStr_chpl, INT64(16), _ln_chpl, _fn_chpl);
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewReindex.chpl"
  refIndentLevel_chpl = &moduleInitLevel_chpl;
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewReindex.chpl"
  *(refIndentLevel_chpl) += INT64(1);
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewReindex.chpl"
  chpl__init_ArrayViewReindex_p = UINT8(true);
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewReindex.chpl"
  *(refIndentLevel_chpl) -= INT64(1);
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewReindex.chpl"
  _endchpl__init_ArrayViewReindex_chpl:;
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewReindex.chpl"
  _exit_chpl__init_ArrayViewReindex_chpl:;
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewReindex.chpl"
  return;
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewReindex.chpl"
}

