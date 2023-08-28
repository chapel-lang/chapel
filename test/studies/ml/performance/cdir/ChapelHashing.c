#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelHashing.chpl"
/* ChapelHashing.chpl:24 */
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelHashing.chpl"
static void chpl__init_ChapelHashing(int64_t _ln_chpl,
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelHashing.chpl"
                                     int32_t _fn_chpl) {
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelHashing.chpl"
  c_string_rehook modFormatStr_chpl;
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelHashing.chpl"
  c_string_rehook modStr_chpl;
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelHashing.chpl"
  _ref_int32_t_chpl refIndentLevel_chpl = NULL;
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelHashing.chpl"
  if (chpl__init_ChapelHashing_p) /* ZLINE: 24 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelHashing.chpl */
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelHashing.chpl"
  {
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelHashing.chpl"
    goto _exit_chpl__init_ChapelHashing_chpl;
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelHashing.chpl"
  }
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelHashing.chpl"
  modFormatStr_chpl = "%*s\n";
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelHashing.chpl"
  modStr_chpl = "ChapelHashing";
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelHashing.chpl"
  printModuleInit_chpl(modFormatStr_chpl, modStr_chpl, INT64(13), _ln_chpl, _fn_chpl);
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelHashing.chpl"
  refIndentLevel_chpl = &moduleInitLevel_chpl;
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelHashing.chpl"
  *(refIndentLevel_chpl) += INT64(1);
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelHashing.chpl"
  chpl__init_ChapelHashing_p = UINT8(true);
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelHashing.chpl"
  {
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelHashing.chpl"
    chpl__init_ChapelBase(_ln_chpl, _fn_chpl);
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelHashing.chpl"
    chpl__init_Reflection(_ln_chpl, _fn_chpl);
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelHashing.chpl"
  }
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelHashing.chpl"
  *(refIndentLevel_chpl) -= INT64(1);
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelHashing.chpl"
  _endchpl__init_ChapelHashing_chpl:;
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelHashing.chpl"
  _exit_chpl__init_ChapelHashing_chpl:;
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelHashing.chpl"
  return;
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelHashing.chpl"
}

