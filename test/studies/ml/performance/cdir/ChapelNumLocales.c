#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelNumLocales.chpl"
/* ChapelNumLocales.chpl:23 */
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelNumLocales.chpl"
static void chpl__init_ChapelNumLocales(int64_t _ln_chpl,
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelNumLocales.chpl"
                                        int32_t _fn_chpl) {
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelNumLocales.chpl"
  _ref_int32_t_chpl refIndentLevel_chpl = NULL;
#line 31 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelNumLocales.chpl"
  int64_t tmp_chpl;
#line 31 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelNumLocales.chpl"
  chpl_bool call_tmp_chpl6;
#line 31 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelNumLocales.chpl"
  int64_t call_tmp_chpl7;
#line 31 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelNumLocales.chpl"
  c_ptrConst_int8_t_chpl call_tmp_chpl8 = NULL;
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelNumLocales.chpl"
  if (chpl__init_ChapelNumLocales_p) /* ZLINE: 23 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelNumLocales.chpl */
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelNumLocales.chpl"
  {
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelNumLocales.chpl"
    goto _exit_chpl__init_ChapelNumLocales_chpl;
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelNumLocales.chpl"
  }
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelNumLocales.chpl"
  printModuleInit_chpl("%*s\n", "ChapelNumLocales", INT64(16), _ln_chpl, _fn_chpl);
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelNumLocales.chpl"
  refIndentLevel_chpl = &moduleInitLevel_chpl;
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelNumLocales.chpl"
  *(refIndentLevel_chpl) += INT64(1);
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelNumLocales.chpl"
  chpl__init_ChapelNumLocales_p = UINT8(true);
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelNumLocales.chpl"
  {
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelNumLocales.chpl"
    chpl__init_ChapelBase(_ln_chpl, _fn_chpl);
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelNumLocales.chpl"
  }
#line 31 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelNumLocales.chpl"
  call_tmp_chpl6 = chpl_config_has_value(((c_ptrConst_int8_t_chpl)("numLocales")), ((c_ptrConst_int8_t_chpl)("Built-in")));
#line 31 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelNumLocales.chpl"
  if (! call_tmp_chpl6) /* ZLINE: 31 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelNumLocales.chpl */
#line 31 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelNumLocales.chpl"
  {
#line 31 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelNumLocales.chpl"
    call_tmp_chpl7 = chpl_comm_default_num_locales();
#line 31 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelNumLocales.chpl"
    tmp_chpl = call_tmp_chpl7;
#line 31 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelNumLocales.chpl"
  } else /* ZLINE: 31 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelNumLocales.chpl */
#line 31 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelNumLocales.chpl"
  {
#line 31 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelNumLocales.chpl"
    call_tmp_chpl8 = chpl_config_get_value(((c_ptrConst_int8_t_chpl)("numLocales")), ((c_ptrConst_int8_t_chpl)("Built-in")));
#line 31 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelNumLocales.chpl"
    tmp_chpl = _command_line_cast_chpl9(call_tmp_chpl8, _ln_chpl, _fn_chpl);
#line 31 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelNumLocales.chpl"
  }
#line 31 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelNumLocales.chpl"
  numLocales_chpl = tmp_chpl;
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelNumLocales.chpl"
  *(refIndentLevel_chpl) -= INT64(1);
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelNumLocales.chpl"
  _exit_chpl__init_ChapelNumLocales_chpl:;
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelNumLocales.chpl"
  return;
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelNumLocales.chpl"
}

