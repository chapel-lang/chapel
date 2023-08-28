#line 46 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
/* OS.chpl:46 */
#line 46 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
static void chpl__init_POSIX(int64_t _ln_chpl,
#line 46 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
                             int32_t _fn_chpl) {
#line 46 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _ref_int32_t_chpl refIndentLevel_chpl = NULL;
#line 46 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  if (chpl__init_POSIX_p) /* ZLINE: 46 /Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl */
#line 46 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  {
#line 46 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    goto _exit_chpl__init_POSIX_chpl;
#line 46 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  }
#line 46 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  printModuleInit_chpl("%*s\n", "POSIX", INT64(5), _ln_chpl, _fn_chpl);
#line 46 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  refIndentLevel_chpl = &moduleInitLevel_chpl;
#line 46 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  *(refIndentLevel_chpl) += INT64(1);
#line 46 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  chpl__init_POSIX_p = UINT8(true);
#line 46 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  {
#line 46 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    chpl__init_OS(_ln_chpl, _fn_chpl);
#line 46 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
    chpl__init_CTypes(_ln_chpl, _fn_chpl);
#line 46 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  }
#line 46 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  *(refIndentLevel_chpl) -= INT64(1);
#line 46 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  _exit_chpl__init_POSIX_chpl:;
#line 46 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
  return;
#line 46 "/Users/iainmoncrief/Documents/chapel/modules/standard/OS.chpl"
}

