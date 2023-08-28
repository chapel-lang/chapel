#line 25 "/Users/iainmoncrief/Documents/chapel/modules/packages/BinaryIO.chpl"
/* BinaryIO.chpl:25 */
#line 25 "/Users/iainmoncrief/Documents/chapel/modules/packages/BinaryIO.chpl"
static void chpl__init_BinaryIO(int64_t _ln_chpl,
#line 25 "/Users/iainmoncrief/Documents/chapel/modules/packages/BinaryIO.chpl"
                                int32_t _fn_chpl) {
#line 25 "/Users/iainmoncrief/Documents/chapel/modules/packages/BinaryIO.chpl"
  c_string_rehook modFormatStr_chpl;
#line 25 "/Users/iainmoncrief/Documents/chapel/modules/packages/BinaryIO.chpl"
  c_string_rehook modStr_chpl;
#line 25 "/Users/iainmoncrief/Documents/chapel/modules/packages/BinaryIO.chpl"
  _ref_int32_t_chpl refIndentLevel_chpl = NULL;
#line 25 "/Users/iainmoncrief/Documents/chapel/modules/packages/BinaryIO.chpl"
  if (chpl__init_BinaryIO_p) /* ZLINE: 25 /Users/iainmoncrief/Documents/chapel/modules/packages/BinaryIO.chpl */
#line 25 "/Users/iainmoncrief/Documents/chapel/modules/packages/BinaryIO.chpl"
  {
#line 25 "/Users/iainmoncrief/Documents/chapel/modules/packages/BinaryIO.chpl"
    goto _exit_chpl__init_BinaryIO_chpl;
#line 25 "/Users/iainmoncrief/Documents/chapel/modules/packages/BinaryIO.chpl"
  }
#line 25 "/Users/iainmoncrief/Documents/chapel/modules/packages/BinaryIO.chpl"
  modFormatStr_chpl = "%*s\n";
#line 25 "/Users/iainmoncrief/Documents/chapel/modules/packages/BinaryIO.chpl"
  modStr_chpl = "BinaryIO";
#line 25 "/Users/iainmoncrief/Documents/chapel/modules/packages/BinaryIO.chpl"
  printModuleInit_chpl(modFormatStr_chpl, modStr_chpl, INT64(8), _ln_chpl, _fn_chpl);
#line 25 "/Users/iainmoncrief/Documents/chapel/modules/packages/BinaryIO.chpl"
  refIndentLevel_chpl = &moduleInitLevel_chpl;
#line 25 "/Users/iainmoncrief/Documents/chapel/modules/packages/BinaryIO.chpl"
  *(refIndentLevel_chpl) += INT64(1);
#line 25 "/Users/iainmoncrief/Documents/chapel/modules/packages/BinaryIO.chpl"
  chpl__init_BinaryIO_p = UINT8(true);
#line 25 "/Users/iainmoncrief/Documents/chapel/modules/packages/BinaryIO.chpl"
  {
#line 25 "/Users/iainmoncrief/Documents/chapel/modules/packages/BinaryIO.chpl"
    chpl__init_IO(_ln_chpl, _fn_chpl);
#line 25 "/Users/iainmoncrief/Documents/chapel/modules/packages/BinaryIO.chpl"
  }
#line 25 "/Users/iainmoncrief/Documents/chapel/modules/packages/BinaryIO.chpl"
  *(refIndentLevel_chpl) -= INT64(1);
#line 25 "/Users/iainmoncrief/Documents/chapel/modules/packages/BinaryIO.chpl"
  _endchpl__init_BinaryIO_chpl:;
#line 25 "/Users/iainmoncrief/Documents/chapel/modules/packages/BinaryIO.chpl"
  _exit_chpl__init_BinaryIO_chpl:;
#line 25 "/Users/iainmoncrief/Documents/chapel/modules/packages/BinaryIO.chpl"
  return;
#line 25 "/Users/iainmoncrief/Documents/chapel/modules/packages/BinaryIO.chpl"
}

