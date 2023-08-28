#line 866 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
/* String.chpl:866 */
#line 866 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
static void chpl__init_NVStringFactory(int64_t _ln_chpl,
#line 866 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
                                       int32_t _fn_chpl) {
#line 866 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _ref_int32_t_chpl refIndentLevel_chpl = NULL;
#line 866 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  if (chpl__init_NVStringFactory_p) /* ZLINE: 866 /Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl */
#line 866 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  {
#line 866 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    goto _exit_chpl__init_NVStringFactory_chpl;
#line 866 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  }
#line 866 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  printModuleInit_chpl("%*s\n", "NVStringFactory", INT64(15), _ln_chpl, _fn_chpl);
#line 866 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  refIndentLevel_chpl = &moduleInitLevel_chpl;
#line 866 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  *(refIndentLevel_chpl) += INT64(1);
#line 866 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  chpl__init_NVStringFactory_p = UINT8(true);
#line 866 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  {
#line 866 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    chpl__init_String(_ln_chpl, _fn_chpl);
#line 866 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    chpl__init_BytesStringCommon(_ln_chpl, _fn_chpl);
#line 866 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
    chpl__init_ByteBufferHelpers(_ln_chpl, _fn_chpl);
#line 866 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  }
#line 866 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  *(refIndentLevel_chpl) -= INT64(1);
#line 866 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  _exit_chpl__init_NVStringFactory_chpl:;
#line 866 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
  return;
#line 866 "/Users/iainmoncrief/Documents/chapel/modules/internal/String.chpl"
}

