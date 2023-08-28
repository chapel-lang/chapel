#line 32 "/Users/iainmoncrief/Documents/chapel/modules/standard/Communication.chpl"
/* Communication.chpl:32 */
#line 32 "/Users/iainmoncrief/Documents/chapel/modules/standard/Communication.chpl"
static void chpl__init_Communication(int64_t _ln_chpl,
#line 32 "/Users/iainmoncrief/Documents/chapel/modules/standard/Communication.chpl"
                                     int32_t _fn_chpl) {
#line 32 "/Users/iainmoncrief/Documents/chapel/modules/standard/Communication.chpl"
  _ref_int32_t_chpl refIndentLevel_chpl = NULL;
#line 32 "/Users/iainmoncrief/Documents/chapel/modules/standard/Communication.chpl"
  if (chpl__init_Communication_p) /* ZLINE: 32 /Users/iainmoncrief/Documents/chapel/modules/standard/Communication.chpl */
#line 32 "/Users/iainmoncrief/Documents/chapel/modules/standard/Communication.chpl"
  {
#line 32 "/Users/iainmoncrief/Documents/chapel/modules/standard/Communication.chpl"
    goto _exit_chpl__init_Communication_chpl;
#line 32 "/Users/iainmoncrief/Documents/chapel/modules/standard/Communication.chpl"
  }
#line 32 "/Users/iainmoncrief/Documents/chapel/modules/standard/Communication.chpl"
  printModuleInit_chpl("%*s\n", "Communication", INT64(13), _ln_chpl, _fn_chpl);
#line 32 "/Users/iainmoncrief/Documents/chapel/modules/standard/Communication.chpl"
  refIndentLevel_chpl = &moduleInitLevel_chpl;
#line 32 "/Users/iainmoncrief/Documents/chapel/modules/standard/Communication.chpl"
  *(refIndentLevel_chpl) += INT64(1);
#line 32 "/Users/iainmoncrief/Documents/chapel/modules/standard/Communication.chpl"
  chpl__init_Communication_p = UINT8(true);
#line 32 "/Users/iainmoncrief/Documents/chapel/modules/standard/Communication.chpl"
  {
#line 32 "/Users/iainmoncrief/Documents/chapel/modules/standard/Communication.chpl"
    chpl__init_CTypes(_ln_chpl, _fn_chpl);
#line 32 "/Users/iainmoncrief/Documents/chapel/modules/standard/Communication.chpl"
  }
#line 32 "/Users/iainmoncrief/Documents/chapel/modules/standard/Communication.chpl"
  *(refIndentLevel_chpl) -= INT64(1);
#line 32 "/Users/iainmoncrief/Documents/chapel/modules/standard/Communication.chpl"
  _exit_chpl__init_Communication_chpl:;
#line 32 "/Users/iainmoncrief/Documents/chapel/modules/standard/Communication.chpl"
  return;
#line 32 "/Users/iainmoncrief/Documents/chapel/modules/standard/Communication.chpl"
}

