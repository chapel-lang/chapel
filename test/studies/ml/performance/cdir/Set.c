#line 47 "/Users/iainmoncrief/Documents/chapel/modules/standard/Set.chpl"
/* Set.chpl:47 */
#line 47 "/Users/iainmoncrief/Documents/chapel/modules/standard/Set.chpl"
static void chpl__init_Set(int64_t _ln_chpl,
#line 47 "/Users/iainmoncrief/Documents/chapel/modules/standard/Set.chpl"
                           int32_t _fn_chpl) {
#line 47 "/Users/iainmoncrief/Documents/chapel/modules/standard/Set.chpl"
  c_string_rehook modFormatStr_chpl;
#line 47 "/Users/iainmoncrief/Documents/chapel/modules/standard/Set.chpl"
  c_string_rehook modStr_chpl;
#line 47 "/Users/iainmoncrief/Documents/chapel/modules/standard/Set.chpl"
  _ref_int32_t_chpl refIndentLevel_chpl = NULL;
#line 47 "/Users/iainmoncrief/Documents/chapel/modules/standard/Set.chpl"
  if (chpl__init_Set_p) /* ZLINE: 47 /Users/iainmoncrief/Documents/chapel/modules/standard/Set.chpl */
#line 47 "/Users/iainmoncrief/Documents/chapel/modules/standard/Set.chpl"
  {
#line 47 "/Users/iainmoncrief/Documents/chapel/modules/standard/Set.chpl"
    goto _exit_chpl__init_Set_chpl;
#line 47 "/Users/iainmoncrief/Documents/chapel/modules/standard/Set.chpl"
  }
#line 47 "/Users/iainmoncrief/Documents/chapel/modules/standard/Set.chpl"
  modFormatStr_chpl = "%*s\n";
#line 47 "/Users/iainmoncrief/Documents/chapel/modules/standard/Set.chpl"
  modStr_chpl = "Set";
#line 47 "/Users/iainmoncrief/Documents/chapel/modules/standard/Set.chpl"
  printModuleInit_chpl(modFormatStr_chpl, modStr_chpl, INT64(3), _ln_chpl, _fn_chpl);
#line 47 "/Users/iainmoncrief/Documents/chapel/modules/standard/Set.chpl"
  refIndentLevel_chpl = &moduleInitLevel_chpl;
#line 47 "/Users/iainmoncrief/Documents/chapel/modules/standard/Set.chpl"
  *(refIndentLevel_chpl) += INT64(1);
#line 47 "/Users/iainmoncrief/Documents/chapel/modules/standard/Set.chpl"
  chpl__init_Set_p = UINT8(true);
#line 47 "/Users/iainmoncrief/Documents/chapel/modules/standard/Set.chpl"
  {
#line 47 "/Users/iainmoncrief/Documents/chapel/modules/standard/Set.chpl"
    chpl__init_ChapelLocks(_ln_chpl, _fn_chpl);
#line 47 "/Users/iainmoncrief/Documents/chapel/modules/standard/Set.chpl"
    chpl__init_IO(_ln_chpl, _fn_chpl);
#line 47 "/Users/iainmoncrief/Documents/chapel/modules/standard/Set.chpl"
    chpl__init_Reflection(_ln_chpl, _fn_chpl);
#line 47 "/Users/iainmoncrief/Documents/chapel/modules/standard/Set.chpl"
    chpl__init_ChapelHashtable(_ln_chpl, _fn_chpl);
#line 47 "/Users/iainmoncrief/Documents/chapel/modules/standard/Set.chpl"
    chpl__init_HaltWrappers(_ln_chpl, _fn_chpl);
#line 47 "/Users/iainmoncrief/Documents/chapel/modules/standard/Set.chpl"
    chpl__init_MemMove(_ln_chpl, _fn_chpl);
#line 47 "/Users/iainmoncrief/Documents/chapel/modules/standard/Set.chpl"
  }
#line 47 "/Users/iainmoncrief/Documents/chapel/modules/standard/Set.chpl"
  *(refIndentLevel_chpl) -= INT64(1);
#line 47 "/Users/iainmoncrief/Documents/chapel/modules/standard/Set.chpl"
  _endchpl__init_Set_chpl:;
#line 47 "/Users/iainmoncrief/Documents/chapel/modules/standard/Set.chpl"
  _exit_chpl__init_Set_chpl:;
#line 47 "/Users/iainmoncrief/Documents/chapel/modules/standard/Set.chpl"
  return;
#line 47 "/Users/iainmoncrief/Documents/chapel/modules/standard/Set.chpl"
}

