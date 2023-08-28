#line 33 "/Users/iainmoncrief/Documents/chapel/modules/standard/Map.chpl"
/* Map.chpl:33 */
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/standard/Map.chpl"
static void chpl__init_Map(int64_t _ln_chpl,
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/standard/Map.chpl"
                           int32_t _fn_chpl) {
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/standard/Map.chpl"
  c_string_rehook modFormatStr_chpl;
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/standard/Map.chpl"
  c_string_rehook modStr_chpl;
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/standard/Map.chpl"
  _ref_int32_t_chpl refIndentLevel_chpl = NULL;
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/standard/Map.chpl"
  if (chpl__init_Map_p) /* ZLINE: 33 /Users/iainmoncrief/Documents/chapel/modules/standard/Map.chpl */
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/standard/Map.chpl"
  {
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/standard/Map.chpl"
    goto _exit_chpl__init_Map_chpl;
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/standard/Map.chpl"
  }
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/standard/Map.chpl"
  modFormatStr_chpl = "%*s\n";
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/standard/Map.chpl"
  modStr_chpl = "Map";
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/standard/Map.chpl"
  printModuleInit_chpl(modFormatStr_chpl, modStr_chpl, INT64(3), _ln_chpl, _fn_chpl);
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/standard/Map.chpl"
  refIndentLevel_chpl = &moduleInitLevel_chpl;
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/standard/Map.chpl"
  *(refIndentLevel_chpl) += INT64(1);
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/standard/Map.chpl"
  chpl__init_Map_p = UINT8(true);
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/standard/Map.chpl"
  {
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/standard/Map.chpl"
    chpl__init_ChapelLocks(_ln_chpl, _fn_chpl);
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/standard/Map.chpl"
    chpl__init_ChapelHashtable(_ln_chpl, _fn_chpl);
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/standard/Map.chpl"
    chpl__init_HaltWrappers(_ln_chpl, _fn_chpl);
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/standard/Map.chpl"
    chpl__init_IO(_ln_chpl, _fn_chpl);
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/standard/Map.chpl"
    chpl__init_FormattedIO(_ln_chpl, _fn_chpl);
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/standard/Map.chpl"
    chpl__init_Reflection(_ln_chpl, _fn_chpl);
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/standard/Map.chpl"
  }
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/standard/Map.chpl"
  *(refIndentLevel_chpl) -= INT64(1);
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/standard/Map.chpl"
  _endchpl__init_Map_chpl:;
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/standard/Map.chpl"
  _exit_chpl__init_Map_chpl:;
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/standard/Map.chpl"
  return;
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/standard/Map.chpl"
}

