#line 30 "/Users/iainmoncrief/Documents/chapel/modules/internal/PrintModuleInitOrder.chpl"
/* PrintModuleInitOrder.chpl:30 */
#line 30 "/Users/iainmoncrief/Documents/chapel/modules/internal/PrintModuleInitOrder.chpl"
void chpl__init_PrintModuleInitOrder(int64_t _ln,
#line 30 "/Users/iainmoncrief/Documents/chapel/modules/internal/PrintModuleInitOrder.chpl"
                                     int32_t _fn) {
#line 30 "/Users/iainmoncrief/Documents/chapel/modules/internal/PrintModuleInitOrder.chpl"
  _ref_int32_t_chpl refIndentLevel_chpl = NULL;
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/internal/PrintModuleInitOrder.chpl"
  chpl_bool tmp_chpl;
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/internal/PrintModuleInitOrder.chpl"
  chpl_bool call_tmp_chpl6;
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/internal/PrintModuleInitOrder.chpl"
  c_ptrConst_int8_t_chpl call_tmp_chpl7 = NULL;
#line 30 "/Users/iainmoncrief/Documents/chapel/modules/internal/PrintModuleInitOrder.chpl"
  if (chpl__init_PrintModuleInitOrder_p) /* ZLINE: 30 /Users/iainmoncrief/Documents/chapel/modules/internal/PrintModuleInitOrder.chpl */
#line 30 "/Users/iainmoncrief/Documents/chapel/modules/internal/PrintModuleInitOrder.chpl"
  {
#line 30 "/Users/iainmoncrief/Documents/chapel/modules/internal/PrintModuleInitOrder.chpl"
    goto _exit_chpl__init_PrintModuleInitOrder_chpl;
#line 30 "/Users/iainmoncrief/Documents/chapel/modules/internal/PrintModuleInitOrder.chpl"
  }
#line 30 "/Users/iainmoncrief/Documents/chapel/modules/internal/PrintModuleInitOrder.chpl"
  printModuleInit_chpl("%*s\n", "PrintModuleInitOrder", INT64(20), _ln, _fn);
#line 30 "/Users/iainmoncrief/Documents/chapel/modules/internal/PrintModuleInitOrder.chpl"
  refIndentLevel_chpl = &moduleInitLevel_chpl;
#line 30 "/Users/iainmoncrief/Documents/chapel/modules/internal/PrintModuleInitOrder.chpl"
  *(refIndentLevel_chpl) += INT64(1);
#line 30 "/Users/iainmoncrief/Documents/chapel/modules/internal/PrintModuleInitOrder.chpl"
  chpl__init_PrintModuleInitOrder_p = UINT8(true);
#line 30 "/Users/iainmoncrief/Documents/chapel/modules/internal/PrintModuleInitOrder.chpl"
  {
#line 30 "/Users/iainmoncrief/Documents/chapel/modules/internal/PrintModuleInitOrder.chpl"
    chpl__init_ChapelBase(_ln, _fn);
#line 30 "/Users/iainmoncrief/Documents/chapel/modules/internal/PrintModuleInitOrder.chpl"
    chpl__init_CTypes(_ln, _fn);
#line 30 "/Users/iainmoncrief/Documents/chapel/modules/internal/PrintModuleInitOrder.chpl"
  }
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/internal/PrintModuleInitOrder.chpl"
  call_tmp_chpl6 = chpl_config_has_value(((c_ptrConst_int8_t_chpl)("printModuleInitOrder")), ((c_ptrConst_int8_t_chpl)("Built-in")));
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/internal/PrintModuleInitOrder.chpl"
  if (! call_tmp_chpl6) /* ZLINE: 33 /Users/iainmoncrief/Documents/chapel/modules/internal/PrintModuleInitOrder.chpl */
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/internal/PrintModuleInitOrder.chpl"
  {
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/internal/PrintModuleInitOrder.chpl"
    tmp_chpl = UINT8(false);
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/internal/PrintModuleInitOrder.chpl"
  } else /* ZLINE: 33 /Users/iainmoncrief/Documents/chapel/modules/internal/PrintModuleInitOrder.chpl */
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/internal/PrintModuleInitOrder.chpl"
  {
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/internal/PrintModuleInitOrder.chpl"
    call_tmp_chpl7 = chpl_config_get_value(((c_ptrConst_int8_t_chpl)("printModuleInitOrder")), ((c_ptrConst_int8_t_chpl)("Built-in")));
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/internal/PrintModuleInitOrder.chpl"
    tmp_chpl = _command_line_cast_chpl15(call_tmp_chpl7, _ln, _fn);
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/internal/PrintModuleInitOrder.chpl"
  }
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/internal/PrintModuleInitOrder.chpl"
  printModuleInitOrder_chpl = tmp_chpl;
#line 34 "/Users/iainmoncrief/Documents/chapel/modules/internal/PrintModuleInitOrder.chpl"
  moduleInitLevel_chpl = INT32(2);
#line 53 "/Users/iainmoncrief/Documents/chapel/modules/internal/PrintModuleInitOrder.chpl"
  if (printModuleInitOrder_chpl) /* ZLINE: 53 /Users/iainmoncrief/Documents/chapel/modules/internal/PrintModuleInitOrder.chpl */
#line 53 "/Users/iainmoncrief/Documents/chapel/modules/internal/PrintModuleInitOrder.chpl"
  {
#line 53 "/Users/iainmoncrief/Documents/chapel/modules/internal/PrintModuleInitOrder.chpl"
    initPrint_chpl();
#line 53 "/Users/iainmoncrief/Documents/chapel/modules/internal/PrintModuleInitOrder.chpl"
  }
#line 30 "/Users/iainmoncrief/Documents/chapel/modules/internal/PrintModuleInitOrder.chpl"
  *(refIndentLevel_chpl) -= INT64(1);
#line 30 "/Users/iainmoncrief/Documents/chapel/modules/internal/PrintModuleInitOrder.chpl"
  _exit_chpl__init_PrintModuleInitOrder_chpl:;
#line 30 "/Users/iainmoncrief/Documents/chapel/modules/internal/PrintModuleInitOrder.chpl"
  return;
#line 30 "/Users/iainmoncrief/Documents/chapel/modules/internal/PrintModuleInitOrder.chpl"
}

#line 40 "/Users/iainmoncrief/Documents/chapel/modules/internal/PrintModuleInitOrder.chpl"
/* PrintModuleInitOrder.chpl:40 */
#line 40 "/Users/iainmoncrief/Documents/chapel/modules/internal/PrintModuleInitOrder.chpl"
static void printModuleInit_chpl(c_string_rehook s1_chpl,
#line 40 "/Users/iainmoncrief/Documents/chapel/modules/internal/PrintModuleInitOrder.chpl"
                                 c_string_rehook s2_chpl,
#line 40 "/Users/iainmoncrief/Documents/chapel/modules/internal/PrintModuleInitOrder.chpl"
                                 int64_t len_chpl,
#line 40 "/Users/iainmoncrief/Documents/chapel/modules/internal/PrintModuleInitOrder.chpl"
                                 int64_t _ln_chpl,
#line 40 "/Users/iainmoncrief/Documents/chapel/modules/internal/PrintModuleInitOrder.chpl"
                                 int32_t _fn_chpl) {
#line 42 "/Users/iainmoncrief/Documents/chapel/modules/internal/PrintModuleInitOrder.chpl"
  if (printModuleInitOrder_chpl) /* ZLINE: 42 /Users/iainmoncrief/Documents/chapel/modules/internal/PrintModuleInitOrder.chpl */
#line 42 "/Users/iainmoncrief/Documents/chapel/modules/internal/PrintModuleInitOrder.chpl"
  {
#line 43 "/Users/iainmoncrief/Documents/chapel/modules/internal/PrintModuleInitOrder.chpl"
    printf(((c_ptrConst_int8_t_chpl)(s1_chpl)), ((int32_t)((((int32_t)((moduleInitLevel_chpl + ((int32_t)(len_chpl))))) + INT32(2)))), ((c_ptrConst_int8_t_chpl)(s2_chpl)));
#line 43 "/Users/iainmoncrief/Documents/chapel/modules/internal/PrintModuleInitOrder.chpl"
  }
#line 40 "/Users/iainmoncrief/Documents/chapel/modules/internal/PrintModuleInitOrder.chpl"
  return;
#line 40 "/Users/iainmoncrief/Documents/chapel/modules/internal/PrintModuleInitOrder.chpl"
}

#line 47 "/Users/iainmoncrief/Documents/chapel/modules/internal/PrintModuleInitOrder.chpl"
/* PrintModuleInitOrder.chpl:47 */
#line 47 "/Users/iainmoncrief/Documents/chapel/modules/internal/PrintModuleInitOrder.chpl"
static void initPrint_chpl(void) {
#line 50 "/Users/iainmoncrief/Documents/chapel/modules/internal/PrintModuleInitOrder.chpl"
  printf("%s\n", "Initializing Modules:");
#line 47 "/Users/iainmoncrief/Documents/chapel/modules/internal/PrintModuleInitOrder.chpl"
  return;
#line 47 "/Users/iainmoncrief/Documents/chapel/modules/internal/PrintModuleInitOrder.chpl"
}

