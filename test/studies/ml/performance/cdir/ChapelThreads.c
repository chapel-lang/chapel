#line 30 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelThreads.chpl"
/* ChapelThreads.chpl:30 */
#line 30 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelThreads.chpl"
static void chpl__init_ChapelThreads(int64_t _ln_chpl,
#line 30 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelThreads.chpl"
                                     int32_t _fn_chpl) {
#line 30 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelThreads.chpl"
  _ref_int32_t_chpl refIndentLevel_chpl = NULL;
#line 34 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelThreads.chpl"
  int32_t call_tmp_chpl6;
#line 34 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelThreads.chpl"
  int64_t init_coerce_tmp_chpl;
#line 30 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelThreads.chpl"
  if (chpl__init_ChapelThreads_p) /* ZLINE: 30 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelThreads.chpl */
#line 30 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelThreads.chpl"
  {
#line 30 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelThreads.chpl"
    goto _exit_chpl__init_ChapelThreads_chpl;
#line 30 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelThreads.chpl"
  }
#line 30 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelThreads.chpl"
  printModuleInit_chpl("%*s\n", "ChapelThreads", INT64(13), _ln_chpl, _fn_chpl);
#line 30 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelThreads.chpl"
  refIndentLevel_chpl = &moduleInitLevel_chpl;
#line 30 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelThreads.chpl"
  *(refIndentLevel_chpl) += INT64(1);
#line 30 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelThreads.chpl"
  chpl__init_ChapelThreads_p = UINT8(true);
#line 34 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelThreads.chpl"
  call_tmp_chpl6 = chpl_task_getenvNumThreadsPerLocale();
#line 34 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelThreads.chpl"
  init_coerce_tmp_chpl = ((int64_t)(call_tmp_chpl6));
#line 34 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelThreads.chpl"
  numThreadsPerLocale_chpl = init_coerce_tmp_chpl;
#line 30 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelThreads.chpl"
  *(refIndentLevel_chpl) -= INT64(1);
#line 30 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelThreads.chpl"
  _exit_chpl__init_ChapelThreads_chpl:;
#line 30 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelThreads.chpl"
  return;
#line 30 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelThreads.chpl"
}

