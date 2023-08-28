#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/DefaultSparse.chpl"
/* DefaultSparse.chpl:26 */
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/DefaultSparse.chpl"
static void chpl__init_DefaultSparse(int64_t _ln_chpl,
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/DefaultSparse.chpl"
                                     int32_t _fn_chpl) {
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/DefaultSparse.chpl"
  c_string_rehook modFormatStr_chpl;
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/DefaultSparse.chpl"
  c_string_rehook modStr_chpl;
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/DefaultSparse.chpl"
  _ref_int32_t_chpl refIndentLevel_chpl = NULL;
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/DefaultSparse.chpl"
  if (chpl__init_DefaultSparse_p) /* ZLINE: 26 /Users/iainmoncrief/Documents/chapel/modules/internal/DefaultSparse.chpl */
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/DefaultSparse.chpl"
  {
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/DefaultSparse.chpl"
    goto _exit_chpl__init_DefaultSparse_chpl;
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/DefaultSparse.chpl"
  }
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/DefaultSparse.chpl"
  modFormatStr_chpl = "%*s\n";
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/DefaultSparse.chpl"
  modStr_chpl = "DefaultSparse";
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/DefaultSparse.chpl"
  printModuleInit_chpl(modFormatStr_chpl, modStr_chpl, INT64(13), _ln_chpl, _fn_chpl);
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/DefaultSparse.chpl"
  refIndentLevel_chpl = &moduleInitLevel_chpl;
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/DefaultSparse.chpl"
  *(refIndentLevel_chpl) += INT64(1);
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/DefaultSparse.chpl"
  chpl__init_DefaultSparse_p = UINT8(true);
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/DefaultSparse.chpl"
  {
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/DefaultSparse.chpl"
    chpl__init_RangeChunk(_ln_chpl, _fn_chpl);
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/DefaultSparse.chpl"
    chpl__init_DSIUtil(_ln_chpl, _fn_chpl);
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/DefaultSparse.chpl"
    chpl__init_Search(_ln_chpl, _fn_chpl);
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/DefaultSparse.chpl"
    chpl__init_Sort(_ln_chpl, _fn_chpl);
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/DefaultSparse.chpl"
  }
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/DefaultSparse.chpl"
  *(refIndentLevel_chpl) -= INT64(1);
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/DefaultSparse.chpl"
  _endchpl__init_DefaultSparse_chpl:;
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/DefaultSparse.chpl"
  _exit_chpl__init_DefaultSparse_chpl:;
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/DefaultSparse.chpl"
  return;
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/DefaultSparse.chpl"
}

