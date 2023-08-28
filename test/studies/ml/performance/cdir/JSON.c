#line 25 "/Users/iainmoncrief/Documents/chapel/modules/packages/JSON.chpl"
/* JSON.chpl:25 */
#line 25 "/Users/iainmoncrief/Documents/chapel/modules/packages/JSON.chpl"
static void chpl__init_JSON(int64_t _ln_chpl,
#line 25 "/Users/iainmoncrief/Documents/chapel/modules/packages/JSON.chpl"
                            int32_t _fn_chpl) {
#line 25 "/Users/iainmoncrief/Documents/chapel/modules/packages/JSON.chpl"
  c_string_rehook modFormatStr_chpl;
#line 25 "/Users/iainmoncrief/Documents/chapel/modules/packages/JSON.chpl"
  c_string_rehook modStr_chpl;
#line 25 "/Users/iainmoncrief/Documents/chapel/modules/packages/JSON.chpl"
  _ref_int32_t_chpl refIndentLevel_chpl = NULL;
#line 25 "/Users/iainmoncrief/Documents/chapel/modules/packages/JSON.chpl"
  if (chpl__init_JSON_p) /* ZLINE: 25 /Users/iainmoncrief/Documents/chapel/modules/packages/JSON.chpl */
#line 25 "/Users/iainmoncrief/Documents/chapel/modules/packages/JSON.chpl"
  {
#line 25 "/Users/iainmoncrief/Documents/chapel/modules/packages/JSON.chpl"
    goto _exit_chpl__init_JSON_chpl;
#line 25 "/Users/iainmoncrief/Documents/chapel/modules/packages/JSON.chpl"
  }
#line 25 "/Users/iainmoncrief/Documents/chapel/modules/packages/JSON.chpl"
  modFormatStr_chpl = "%*s\n";
#line 25 "/Users/iainmoncrief/Documents/chapel/modules/packages/JSON.chpl"
  modStr_chpl = "JSON";
#line 25 "/Users/iainmoncrief/Documents/chapel/modules/packages/JSON.chpl"
  printModuleInit_chpl(modFormatStr_chpl, modStr_chpl, INT64(4), _ln_chpl, _fn_chpl);
#line 25 "/Users/iainmoncrief/Documents/chapel/modules/packages/JSON.chpl"
  refIndentLevel_chpl = &moduleInitLevel_chpl;
#line 25 "/Users/iainmoncrief/Documents/chapel/modules/packages/JSON.chpl"
  *(refIndentLevel_chpl) += INT64(1);
#line 25 "/Users/iainmoncrief/Documents/chapel/modules/packages/JSON.chpl"
  chpl__init_JSON_p = UINT8(true);
#line 25 "/Users/iainmoncrief/Documents/chapel/modules/packages/JSON.chpl"
  {
#line 25 "/Users/iainmoncrief/Documents/chapel/modules/packages/JSON.chpl"
    chpl__init_IO(_ln_chpl, _fn_chpl);
#line 25 "/Users/iainmoncrief/Documents/chapel/modules/packages/JSON.chpl"
    chpl__init_CTypes(_ln_chpl, _fn_chpl);
#line 25 "/Users/iainmoncrief/Documents/chapel/modules/packages/JSON.chpl"
    chpl__init_Map(_ln_chpl, _fn_chpl);
#line 25 "/Users/iainmoncrief/Documents/chapel/modules/packages/JSON.chpl"
    chpl__init_List(_ln_chpl, _fn_chpl);
#line 25 "/Users/iainmoncrief/Documents/chapel/modules/packages/JSON.chpl"
  }
#line 25 "/Users/iainmoncrief/Documents/chapel/modules/packages/JSON.chpl"
  *(refIndentLevel_chpl) -= INT64(1);
#line 25 "/Users/iainmoncrief/Documents/chapel/modules/packages/JSON.chpl"
  _endchpl__init_JSON_chpl:;
#line 25 "/Users/iainmoncrief/Documents/chapel/modules/packages/JSON.chpl"
  _exit_chpl__init_JSON_chpl:;
#line 25 "/Users/iainmoncrief/Documents/chapel/modules/packages/JSON.chpl"
  return;
#line 25 "/Users/iainmoncrief/Documents/chapel/modules/packages/JSON.chpl"
}

