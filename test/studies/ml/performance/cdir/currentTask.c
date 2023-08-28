#line 1781 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelBase.chpl"
/* ChapelBase.chpl:1781 */
#line 1781 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelBase.chpl"
static void chpl__init_currentTask(int64_t _ln_chpl,
#line 1781 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelBase.chpl"
                                   int32_t _fn_chpl) {
#line 1781 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelBase.chpl"
  _ref_int32_t_chpl refIndentLevel_chpl = NULL;
#line 1781 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelBase.chpl"
  if (chpl__init_currentTask_p) /* ZLINE: 1781 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelBase.chpl */
#line 1781 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelBase.chpl"
  {
#line 1781 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelBase.chpl"
    goto _exit_chpl__init_currentTask_chpl;
#line 1781 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelBase.chpl"
  }
#line 1781 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelBase.chpl"
  printModuleInit_chpl("%*s\n", "currentTask", INT64(11), _ln_chpl, _fn_chpl);
#line 1781 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelBase.chpl"
  refIndentLevel_chpl = &moduleInitLevel_chpl;
#line 1781 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelBase.chpl"
  *(refIndentLevel_chpl) += INT64(1);
#line 1781 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelBase.chpl"
  chpl__init_currentTask_p = UINT8(true);
#line 1781 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelBase.chpl"
  {
#line 1781 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelBase.chpl"
    chpl__init_ChapelBase(_ln_chpl, _fn_chpl);
#line 1781 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelBase.chpl"
  }
#line 1781 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelBase.chpl"
  *(refIndentLevel_chpl) -= INT64(1);
#line 1781 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelBase.chpl"
  _exit_chpl__init_currentTask_chpl:;
#line 1781 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelBase.chpl"
  return;
#line 1781 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelBase.chpl"
}

