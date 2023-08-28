#line 37 "/Users/iainmoncrief/Documents/chapel/modules/standard/MemMove.chpl"
/* MemMove.chpl:37 */
#line 37 "/Users/iainmoncrief/Documents/chapel/modules/standard/MemMove.chpl"
static void chpl__init_MemMove(int64_t _ln_chpl,
#line 37 "/Users/iainmoncrief/Documents/chapel/modules/standard/MemMove.chpl"
                               int32_t _fn_chpl) {
#line 37 "/Users/iainmoncrief/Documents/chapel/modules/standard/MemMove.chpl"
  c_string_rehook modFormatStr_chpl;
#line 37 "/Users/iainmoncrief/Documents/chapel/modules/standard/MemMove.chpl"
  c_string_rehook modStr_chpl;
#line 37 "/Users/iainmoncrief/Documents/chapel/modules/standard/MemMove.chpl"
  _ref_int32_t_chpl refIndentLevel_chpl = NULL;
#line 37 "/Users/iainmoncrief/Documents/chapel/modules/standard/MemMove.chpl"
  if (chpl__init_MemMove_p) /* ZLINE: 37 /Users/iainmoncrief/Documents/chapel/modules/standard/MemMove.chpl */
#line 37 "/Users/iainmoncrief/Documents/chapel/modules/standard/MemMove.chpl"
  {
#line 37 "/Users/iainmoncrief/Documents/chapel/modules/standard/MemMove.chpl"
    goto _exit_chpl__init_MemMove_chpl;
#line 37 "/Users/iainmoncrief/Documents/chapel/modules/standard/MemMove.chpl"
  }
#line 37 "/Users/iainmoncrief/Documents/chapel/modules/standard/MemMove.chpl"
  modFormatStr_chpl = "%*s\n";
#line 37 "/Users/iainmoncrief/Documents/chapel/modules/standard/MemMove.chpl"
  modStr_chpl = "MemMove";
#line 37 "/Users/iainmoncrief/Documents/chapel/modules/standard/MemMove.chpl"
  printModuleInit_chpl(modFormatStr_chpl, modStr_chpl, INT64(7), _ln_chpl, _fn_chpl);
#line 37 "/Users/iainmoncrief/Documents/chapel/modules/standard/MemMove.chpl"
  refIndentLevel_chpl = &moduleInitLevel_chpl;
#line 37 "/Users/iainmoncrief/Documents/chapel/modules/standard/MemMove.chpl"
  *(refIndentLevel_chpl) += INT64(1);
#line 37 "/Users/iainmoncrief/Documents/chapel/modules/standard/MemMove.chpl"
  chpl__init_MemMove_p = UINT8(true);
#line 37 "/Users/iainmoncrief/Documents/chapel/modules/standard/MemMove.chpl"
  {
#line 37 "/Users/iainmoncrief/Documents/chapel/modules/standard/MemMove.chpl"
    chpl__init_HaltWrappers(_ln_chpl, _fn_chpl);
#line 37 "/Users/iainmoncrief/Documents/chapel/modules/standard/MemMove.chpl"
    chpl__init_IO(_ln_chpl, _fn_chpl);
#line 37 "/Users/iainmoncrief/Documents/chapel/modules/standard/MemMove.chpl"
  }
#line 37 "/Users/iainmoncrief/Documents/chapel/modules/standard/MemMove.chpl"
  *(refIndentLevel_chpl) -= INT64(1);
#line 37 "/Users/iainmoncrief/Documents/chapel/modules/standard/MemMove.chpl"
  _endchpl__init_MemMove_chpl:;
#line 37 "/Users/iainmoncrief/Documents/chapel/modules/standard/MemMove.chpl"
  _exit_chpl__init_MemMove_chpl:;
#line 37 "/Users/iainmoncrief/Documents/chapel/modules/standard/MemMove.chpl"
  return;
#line 37 "/Users/iainmoncrief/Documents/chapel/modules/standard/MemMove.chpl"
}

