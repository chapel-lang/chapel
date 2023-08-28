#line 735 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
/* SharedObject.chpl:735 */
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
static void chpl__init_WeakPointer(int64_t _ln_chpl,
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
                                   int32_t _fn_chpl) {
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
  c_string_rehook modFormatStr_chpl;
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
  c_string_rehook modStr_chpl;
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
  _ref_int32_t_chpl refIndentLevel_chpl = NULL;
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
  if (chpl__init_WeakPointer_p) /* ZLINE: 735 /Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl */
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
  {
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
    goto _exit_chpl__init_WeakPointer_chpl;
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
  }
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
  modFormatStr_chpl = "%*s\n";
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
  modStr_chpl = "WeakPointer";
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
  printModuleInit_chpl(modFormatStr_chpl, modStr_chpl, INT64(11), _ln_chpl, _fn_chpl);
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
  refIndentLevel_chpl = &moduleInitLevel_chpl;
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
  *(refIndentLevel_chpl) += INT64(1);
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
  chpl__init_WeakPointer_p = UINT8(true);
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
  {
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
    chpl__init_Errors(_ln_chpl, _fn_chpl);
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
    chpl__init_Atomics(_ln_chpl, _fn_chpl);
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
    chpl__init_ChapelBase(_ln_chpl, _fn_chpl);
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
  }
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
  *(refIndentLevel_chpl) -= INT64(1);
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
  _endchpl__init_WeakPointer_chpl:;
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
  _exit_chpl__init_WeakPointer_chpl:;
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
  return;
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
}

