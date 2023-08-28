#line 6 "../lib/Chai.chpl"
/* Chai.chpl:6 */
#line 6 "../lib/Chai.chpl"
static void chpl__init_IO2(int64_t _ln_chpl,
#line 6 "../lib/Chai.chpl"
                           int32_t _fn_chpl) {
#line 6 "../lib/Chai.chpl"
  _ref_int32_t_chpl refIndentLevel_chpl = NULL;
#line 6 "../lib/Chai.chpl"
  if (chpl__init_IO_p) /* ZLINE: 6 ../lib/Chai.chpl */
#line 6 "../lib/Chai.chpl"
  {
#line 6 "../lib/Chai.chpl"
    goto _exit_chpl__init_IO_chpl;
#line 6 "../lib/Chai.chpl"
  }
#line 6 "../lib/Chai.chpl"
  printModuleInit_chpl("%*s\n", "IO", INT64(2), INT64(6), INT32(65));
#line 6 "../lib/Chai.chpl"
  refIndentLevel_chpl = &moduleInitLevel_chpl;
#line 6 "../lib/Chai.chpl"
  *(refIndentLevel_chpl) += INT64(1);
#line 6 "../lib/Chai.chpl"
  chpl__init_IO_p = UINT8(true);
#line 6 "../lib/Chai.chpl"
  {
#line 6 "../lib/Chai.chpl"
    chpl__init_Chai(INT64(6), INT32(65));
#line 6 "../lib/Chai.chpl"
    chpl__init_IO(INT64(6), INT32(65));
#line 6 "../lib/Chai.chpl"
  }
#line 6 "../lib/Chai.chpl"
  *(refIndentLevel_chpl) -= INT64(1);
#line 6 "../lib/Chai.chpl"
  _exit_chpl__init_IO_chpl:;
#line 6 "../lib/Chai.chpl"
  return;
#line 6 "../lib/Chai.chpl"
}

