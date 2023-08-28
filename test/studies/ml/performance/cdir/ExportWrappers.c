#line 22 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExportWrappers.chpl"
/* ExportWrappers.chpl:22 */
#line 22 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExportWrappers.chpl"
static void chpl__init_ExportWrappers(int64_t _ln_chpl,
#line 22 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExportWrappers.chpl"
                                      int32_t _fn_chpl) {
#line 22 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExportWrappers.chpl"
  _ref_int32_t_chpl refIndentLevel_chpl = NULL;
#line 22 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExportWrappers.chpl"
  if (chpl__init_ExportWrappers_p) /* ZLINE: 22 /Users/iainmoncrief/Documents/chapel/modules/internal/ExportWrappers.chpl */
#line 22 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExportWrappers.chpl"
  {
#line 22 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExportWrappers.chpl"
    goto _exit_chpl__init_ExportWrappers_chpl;
#line 22 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExportWrappers.chpl"
  }
#line 22 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExportWrappers.chpl"
  printModuleInit_chpl("%*s\n", "ExportWrappers", INT64(14), _ln_chpl, _fn_chpl);
#line 22 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExportWrappers.chpl"
  refIndentLevel_chpl = &moduleInitLevel_chpl;
#line 22 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExportWrappers.chpl"
  *(refIndentLevel_chpl) += INT64(1);
#line 22 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExportWrappers.chpl"
  chpl__init_ExportWrappers_p = UINT8(true);
#line 22 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExportWrappers.chpl"
  {
#line 22 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExportWrappers.chpl"
    chpl__init_CTypes(_ln_chpl, _fn_chpl);
#line 22 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExportWrappers.chpl"
  }
#line 22 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExportWrappers.chpl"
  *(refIndentLevel_chpl) -= INT64(1);
#line 22 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExportWrappers.chpl"
  _exit_chpl__init_ExportWrappers_chpl:;
#line 22 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExportWrappers.chpl"
  return;
#line 22 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExportWrappers.chpl"
}

#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExportWrappers.chpl"
/* ExportWrappers.chpl:26 */
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExportWrappers.chpl"
static void _initDynamicEndCount_chpl(int64_t _ln_chpl,
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExportWrappers.chpl"
                                      int32_t _fn_chpl) {
#line 27 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExportWrappers.chpl"
  chpl___EndCount_AtomicT_int64_t_int64_t new_temp_chpl = NULL;
#line 27 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExportWrappers.chpl"
  new_temp_chpl = _new_chpl17(_ln_chpl, _fn_chpl);
#line 28 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExportWrappers.chpl"
  chpl_task_setDynamicEndCount(new_temp_chpl, _ln_chpl, _fn_chpl);
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExportWrappers.chpl"
  return;
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExportWrappers.chpl"
}

#line 31 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExportWrappers.chpl"
/* ExportWrappers.chpl:31 */
#line 31 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExportWrappers.chpl"
static void _destroyDynamicEndCount_chpl(int64_t _ln_chpl,
#line 31 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExportWrappers.chpl"
                                         int32_t _fn_chpl) {
#line 32 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExportWrappers.chpl"
  chpl___EndCount_AtomicT_int64_t_int64_t call_tmp_chpl6 = NULL;
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExportWrappers.chpl"
  Error_chpl error_chpl = NULL;
#line 32 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExportWrappers.chpl"
  call_tmp_chpl6 = chpl_task_getDynamicEndCount(_ln_chpl, _fn_chpl);
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExportWrappers.chpl"
  error_chpl = NULL;
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExportWrappers.chpl"
  _waitEndCount_chpl(call_tmp_chpl6, &error_chpl, _ln_chpl, _fn_chpl);
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExportWrappers.chpl"
  if (error_chpl != nil) /* ZLINE: 33 /Users/iainmoncrief/Documents/chapel/modules/internal/ExportWrappers.chpl */
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExportWrappers.chpl"
  {
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExportWrappers.chpl"
    chpl_uncaught_error(error_chpl, _ln_chpl, _fn_chpl);
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExportWrappers.chpl"
  }
#line 34 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExportWrappers.chpl"
  chpl__delete72(call_tmp_chpl6, _ln_chpl, _fn_chpl);
#line 31 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExportWrappers.chpl"
  return;
#line 31 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExportWrappers.chpl"
}

#line 38 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExportWrappers.chpl"
/* ExportWrappers.chpl:38 */
#line 38 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExportWrappers.chpl"
void chpl_libraryModuleLevelSetup(void) {
#line 39 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExportWrappers.chpl"
  _initDynamicEndCount_chpl(INT64(39), INT32(41));
#line 38 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExportWrappers.chpl"
  return;
#line 38 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExportWrappers.chpl"
}

#line 43 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExportWrappers.chpl"
/* ExportWrappers.chpl:43 */
#line 43 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExportWrappers.chpl"
void chpl_libraryModuleLevelCleanup(void) {
#line 44 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExportWrappers.chpl"
  _destroyDynamicEndCount_chpl(INT64(44), INT32(41));
#line 43 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExportWrappers.chpl"
  return;
#line 43 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExportWrappers.chpl"
}

