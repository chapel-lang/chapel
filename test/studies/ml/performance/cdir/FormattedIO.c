#line 10359 "/Users/iainmoncrief/Documents/chapel/modules/standard/IO.chpl"
/* IO.chpl:10359 */
#line 10359 "/Users/iainmoncrief/Documents/chapel/modules/standard/IO.chpl"
static void chpl__init_FormattedIO(int64_t _ln_chpl,
#line 10359 "/Users/iainmoncrief/Documents/chapel/modules/standard/IO.chpl"
                                   int32_t _fn_chpl) {
#line 10359 "/Users/iainmoncrief/Documents/chapel/modules/standard/IO.chpl"
  _ref_int32_t_chpl refIndentLevel_chpl = NULL;
#line 10359 "/Users/iainmoncrief/Documents/chapel/modules/standard/IO.chpl"
  if (chpl__init_FormattedIO_p) /* ZLINE: 10359 /Users/iainmoncrief/Documents/chapel/modules/standard/IO.chpl */
#line 10359 "/Users/iainmoncrief/Documents/chapel/modules/standard/IO.chpl"
  {
#line 10359 "/Users/iainmoncrief/Documents/chapel/modules/standard/IO.chpl"
    goto _exit_chpl__init_FormattedIO_chpl;
#line 10359 "/Users/iainmoncrief/Documents/chapel/modules/standard/IO.chpl"
  }
#line 10359 "/Users/iainmoncrief/Documents/chapel/modules/standard/IO.chpl"
  printModuleInit_chpl("%*s\n", "FormattedIO", INT64(11), _ln_chpl, _fn_chpl);
#line 10359 "/Users/iainmoncrief/Documents/chapel/modules/standard/IO.chpl"
  refIndentLevel_chpl = &moduleInitLevel_chpl;
#line 10359 "/Users/iainmoncrief/Documents/chapel/modules/standard/IO.chpl"
  *(refIndentLevel_chpl) += INT64(1);
#line 10359 "/Users/iainmoncrief/Documents/chapel/modules/standard/IO.chpl"
  chpl__init_FormattedIO_p = UINT8(true);
#line 10359 "/Users/iainmoncrief/Documents/chapel/modules/standard/IO.chpl"
  {
#line 10359 "/Users/iainmoncrief/Documents/chapel/modules/standard/IO.chpl"
    chpl__init_IO(_ln_chpl, _fn_chpl);
#line 10359 "/Users/iainmoncrief/Documents/chapel/modules/standard/IO.chpl"
    chpl__init_IO(_ln_chpl, _fn_chpl);
#line 10359 "/Users/iainmoncrief/Documents/chapel/modules/standard/IO.chpl"
    chpl__init_CTypes(_ln_chpl, _fn_chpl);
#line 10359 "/Users/iainmoncrief/Documents/chapel/modules/standard/IO.chpl"
    chpl__init_POSIX(_ln_chpl, _fn_chpl);
#line 10359 "/Users/iainmoncrief/Documents/chapel/modules/standard/IO.chpl"
    chpl__init_OS(_ln_chpl, _fn_chpl);
#line 10359 "/Users/iainmoncrief/Documents/chapel/modules/standard/IO.chpl"
    chpl__init_Regex(_ln_chpl, _fn_chpl);
#line 10359 "/Users/iainmoncrief/Documents/chapel/modules/standard/IO.chpl"
  }
#line 10359 "/Users/iainmoncrief/Documents/chapel/modules/standard/IO.chpl"
  *(refIndentLevel_chpl) -= INT64(1);
#line 10359 "/Users/iainmoncrief/Documents/chapel/modules/standard/IO.chpl"
  _exit_chpl__init_FormattedIO_chpl:;
#line 10359 "/Users/iainmoncrief/Documents/chapel/modules/standard/IO.chpl"
  return;
#line 10359 "/Users/iainmoncrief/Documents/chapel/modules/standard/IO.chpl"
}

