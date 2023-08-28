#line 33 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChplConfig.chpl"
/* ChplConfig.chpl:33 */
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChplConfig.chpl"
static void chpl__init_ChplConfig(int64_t _ln_chpl,
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChplConfig.chpl"
                                  int32_t _fn_chpl) {
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChplConfig.chpl"
  c_string_rehook modFormatStr_chpl;
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChplConfig.chpl"
  c_string_rehook modStr_chpl;
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChplConfig.chpl"
  _ref_int32_t_chpl refIndentLevel_chpl = NULL;
#line 39 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChplConfig.chpl"
  string_chpl type_tmp_chpl;
#line 40 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChplConfig.chpl"
  string_chpl call_tmp_chpl6;
#line 52 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChplConfig.chpl"
  string_chpl type_tmp_chpl2;
#line 53 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChplConfig.chpl"
  string_chpl call_tmp_chpl7;
#line 57 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChplConfig.chpl"
  string_chpl type_tmp_chpl3;
#line 58 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChplConfig.chpl"
  string_chpl call_tmp_chpl8;
#line 62 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChplConfig.chpl"
  string_chpl type_tmp_chpl4;
#line 63 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChplConfig.chpl"
  string_chpl call_tmp_chpl9;
#line 67 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChplConfig.chpl"
  string_chpl type_tmp_chpl5;
#line 68 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChplConfig.chpl"
  string_chpl call_tmp_chpl10;
#line 72 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChplConfig.chpl"
  string_chpl type_tmp_chpl6;
#line 73 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChplConfig.chpl"
  string_chpl call_tmp_chpl11;
#line 77 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChplConfig.chpl"
  string_chpl type_tmp_chpl7;
#line 78 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChplConfig.chpl"
  string_chpl call_tmp_chpl12;
#line 82 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChplConfig.chpl"
  string_chpl type_tmp_chpl8;
#line 83 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChplConfig.chpl"
  string_chpl call_tmp_chpl13;
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChplConfig.chpl"
  string_chpl type_tmp_chpl9;
#line 88 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChplConfig.chpl"
  string_chpl call_tmp_chpl14;
#line 95 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChplConfig.chpl"
  string_chpl type_tmp_chpl10;
#line 96 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChplConfig.chpl"
  string_chpl call_tmp_chpl15;
#line 100 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChplConfig.chpl"
  string_chpl type_tmp_chpl11;
#line 101 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChplConfig.chpl"
  string_chpl call_tmp_chpl16;
#line 105 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChplConfig.chpl"
  string_chpl type_tmp_chpl12;
#line 106 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChplConfig.chpl"
  string_chpl call_tmp_chpl17;
#line 111 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChplConfig.chpl"
  string_chpl type_tmp_chpl13;
#line 112 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChplConfig.chpl"
  string_chpl call_tmp_chpl18;
#line 116 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChplConfig.chpl"
  string_chpl type_tmp_chpl14;
#line 117 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChplConfig.chpl"
  string_chpl call_tmp_chpl19;
#line 121 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChplConfig.chpl"
  string_chpl type_tmp_chpl15;
#line 122 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChplConfig.chpl"
  string_chpl call_tmp_chpl20;
#line 126 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChplConfig.chpl"
  string_chpl type_tmp_chpl16;
#line 127 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChplConfig.chpl"
  string_chpl call_tmp_chpl21;
#line 131 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChplConfig.chpl"
  string_chpl type_tmp_chpl17;
#line 132 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChplConfig.chpl"
  string_chpl call_tmp_chpl22;
#line 136 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChplConfig.chpl"
  string_chpl type_tmp_chpl18;
#line 137 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChplConfig.chpl"
  string_chpl call_tmp_chpl23;
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChplConfig.chpl"
  string_chpl type_tmp_chpl19;
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChplConfig.chpl"
  string_chpl call_tmp_chpl24;
#line 146 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChplConfig.chpl"
  string_chpl type_tmp_chpl20;
#line 147 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChplConfig.chpl"
  string_chpl call_tmp_chpl25;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChplConfig.chpl"
  string_chpl type_tmp_chpl21;
#line 152 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChplConfig.chpl"
  string_chpl call_tmp_chpl26;
#line 156 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChplConfig.chpl"
  string_chpl type_tmp_chpl22;
#line 157 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChplConfig.chpl"
  string_chpl call_tmp_chpl27;
#line 161 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChplConfig.chpl"
  string_chpl type_tmp_chpl23;
#line 162 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChplConfig.chpl"
  string_chpl call_tmp_chpl28;
#line 167 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChplConfig.chpl"
  string_chpl type_tmp_chpl24;
#line 168 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChplConfig.chpl"
  string_chpl call_tmp_chpl29;
#line 172 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChplConfig.chpl"
  string_chpl type_tmp_chpl25;
#line 173 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChplConfig.chpl"
  string_chpl call_tmp_chpl30;
#line 177 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChplConfig.chpl"
  string_chpl type_tmp_chpl26;
#line 178 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChplConfig.chpl"
  string_chpl call_tmp_chpl31;
#line 182 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChplConfig.chpl"
  string_chpl type_tmp_chpl27;
#line 183 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChplConfig.chpl"
  string_chpl call_tmp_chpl32;
#line 187 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChplConfig.chpl"
  string_chpl type_tmp_chpl28;
#line 188 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChplConfig.chpl"
  string_chpl call_tmp_chpl33;
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChplConfig.chpl"
  if (chpl__init_ChplConfig_p) /* ZLINE: 33 /Users/iainmoncrief/Documents/chapel/modules/standard/ChplConfig.chpl */
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChplConfig.chpl"
  {
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChplConfig.chpl"
    goto _exit_chpl__init_ChplConfig_chpl;
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChplConfig.chpl"
  }
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChplConfig.chpl"
  modFormatStr_chpl = "%*s\n";
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChplConfig.chpl"
  modStr_chpl = "ChplConfig";
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChplConfig.chpl"
  printModuleInit_chpl(modFormatStr_chpl, modStr_chpl, INT64(10), _ln_chpl, _fn_chpl);
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChplConfig.chpl"
  refIndentLevel_chpl = &moduleInitLevel_chpl;
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChplConfig.chpl"
  *(refIndentLevel_chpl) += INT64(1);
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChplConfig.chpl"
  chpl__init_ChplConfig_p = UINT8(true);
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChplConfig.chpl"
  {
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChplConfig.chpl"
    chpl__init_String(_ln_chpl, _fn_chpl);
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChplConfig.chpl"
  }
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChplConfig.chpl"
  *(refIndentLevel_chpl) -= INT64(1);
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChplConfig.chpl"
  _endchpl__init_ChplConfig_chpl:;
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChplConfig.chpl"
  _exit_chpl__init_ChplConfig_chpl:;
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChplConfig.chpl"
  return;
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChplConfig.chpl"
}

