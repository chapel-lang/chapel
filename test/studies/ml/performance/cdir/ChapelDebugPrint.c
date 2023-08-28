#line 29 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDebugPrint.chpl"
/* ChapelDebugPrint.chpl:29 */
#line 29 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDebugPrint.chpl"
static void chpl__init_ChapelDebugPrint(int64_t _ln_chpl,
#line 29 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDebugPrint.chpl"
                                        int32_t _fn_chpl) {
#line 29 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDebugPrint.chpl"
  _ref_int32_t_chpl refIndentLevel_chpl = NULL;
#line 29 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDebugPrint.chpl"
  if (chpl__init_ChapelDebugPrint_p) /* ZLINE: 29 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDebugPrint.chpl */
#line 29 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDebugPrint.chpl"
  {
#line 29 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDebugPrint.chpl"
    goto _exit_chpl__init_ChapelDebugPrint_chpl;
#line 29 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDebugPrint.chpl"
  }
#line 29 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDebugPrint.chpl"
  printModuleInit_chpl("%*s\n", "ChapelDebugPrint", INT64(16), _ln_chpl, _fn_chpl);
#line 29 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDebugPrint.chpl"
  refIndentLevel_chpl = &moduleInitLevel_chpl;
#line 29 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDebugPrint.chpl"
  *(refIndentLevel_chpl) += INT64(1);
#line 29 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDebugPrint.chpl"
  chpl__init_ChapelDebugPrint_p = UINT8(true);
#line 29 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDebugPrint.chpl"
  {
#line 29 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDebugPrint.chpl"
    chpl__init_CTypes(_ln_chpl, _fn_chpl);
#line 29 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDebugPrint.chpl"
    chpl__init_ChapelIOStringifyHelper(_ln_chpl, _fn_chpl);
#line 29 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDebugPrint.chpl"
  }
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDebugPrint.chpl"
  chpl__testParOn = UINT8(false);
#line 29 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDebugPrint.chpl"
  *(refIndentLevel_chpl) -= INT64(1);
#line 29 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDebugPrint.chpl"
  _exit_chpl__init_ChapelDebugPrint_chpl:;
#line 29 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDebugPrint.chpl"
  return;
#line 29 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDebugPrint.chpl"
}

