#line 30 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpRuntime.chpl"
/* LocaleModelHelpRuntime.chpl:30 */
#line 30 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpRuntime.chpl"
static void chpl__init_LocaleModelHelpRuntime(int64_t _ln_chpl,
#line 30 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpRuntime.chpl"
                                              int32_t _fn_chpl) {
#line 30 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpRuntime.chpl"
  _ref_int32_t_chpl refIndentLevel_chpl = NULL;
#line 30 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpRuntime.chpl"
  if (chpl__init_LocaleModelHelpRuntime_p) /* ZLINE: 30 /Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpRuntime.chpl */
#line 30 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpRuntime.chpl"
  {
#line 30 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpRuntime.chpl"
    goto _exit_chpl__init_LocaleModelHelpRuntime_chpl;
#line 30 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpRuntime.chpl"
  }
#line 30 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpRuntime.chpl"
  printModuleInit_chpl("%*s\n", "LocaleModelHelpRuntime", INT64(22), _ln_chpl, _fn_chpl);
#line 30 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpRuntime.chpl"
  refIndentLevel_chpl = &moduleInitLevel_chpl;
#line 30 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpRuntime.chpl"
  *(refIndentLevel_chpl) += INT64(1);
#line 30 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpRuntime.chpl"
  chpl__init_LocaleModelHelpRuntime_p = UINT8(true);
#line 30 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpRuntime.chpl"
  {
#line 30 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpRuntime.chpl"
    chpl__init_CTypes(_ln_chpl, _fn_chpl);
#line 30 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpRuntime.chpl"
  }
#line 30 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpRuntime.chpl"
  *(refIndentLevel_chpl) -= INT64(1);
#line 30 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpRuntime.chpl"
  _exit_chpl__init_LocaleModelHelpRuntime_chpl:;
#line 30 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpRuntime.chpl"
  return;
#line 30 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpRuntime.chpl"
}

#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpRuntime.chpl"
/* LocaleModelHelpRuntime.chpl:76 */
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpRuntime.chpl"
static chpl_localeID_t chpl_buildLocaleID(int32_t node_chpl,
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpRuntime.chpl"
                                          int32_t subloc_chpl,
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpRuntime.chpl"
                                          int64_t _ln_chpl,
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpRuntime.chpl"
                                          int32_t _fn_chpl) {
#line 77 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpRuntime.chpl"
  chpl_localeID_t call_tmp_chpl6;
#line 77 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpRuntime.chpl"
  call_tmp_chpl6 = chpl_rt_buildLocaleID(node_chpl, subloc_chpl);
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpRuntime.chpl"
  return call_tmp_chpl6;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpRuntime.chpl"
}

#line 82 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpRuntime.chpl"
/* LocaleModelHelpRuntime.chpl:82 */
#line 82 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpRuntime.chpl"
static int32_t chpl_nodeFromLocaleID(chpl_localeID_t loc_chpl,
#line 82 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpRuntime.chpl"
                                     int64_t _ln_chpl,
#line 82 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpRuntime.chpl"
                                     int32_t _fn_chpl) {
#line 83 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpRuntime.chpl"
  int32_t call_tmp_chpl6;
#line 83 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpRuntime.chpl"
  call_tmp_chpl6 = chpl_rt_nodeFromLocaleID(loc_chpl);
#line 82 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpRuntime.chpl"
  return call_tmp_chpl6;
#line 82 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpRuntime.chpl"
}

#line 88 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpRuntime.chpl"
/* LocaleModelHelpRuntime.chpl:88 */
#line 88 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpRuntime.chpl"
static int32_t chpl_sublocFromLocaleID(chpl_localeID_t loc_chpl,
#line 88 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpRuntime.chpl"
                                       int64_t _ln_chpl,
#line 88 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpRuntime.chpl"
                                       int32_t _fn_chpl) {
#line 89 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpRuntime.chpl"
  int32_t call_tmp_chpl6;
#line 89 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpRuntime.chpl"
  call_tmp_chpl6 = chpl_rt_sublocFromLocaleID(loc_chpl);
#line 88 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpRuntime.chpl"
  return call_tmp_chpl6;
#line 88 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpRuntime.chpl"
}

#line 135 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpRuntime.chpl"
/* LocaleModelHelpRuntime.chpl:135 */
#line 135 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpRuntime.chpl"
static void chpl_taskAddBegin(int64_t subloc_id_chpl,
#line 135 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpRuntime.chpl"
                              int64_t fn_chpl,
#line 135 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpRuntime.chpl"
                              chpl_task_bundle_p args_chpl,
#line 135 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpRuntime.chpl"
                              uint64_t args_size_chpl,
#line 135 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpRuntime.chpl"
                              int64_t _ln_chpl,
#line 135 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpRuntime.chpl"
                              int32_t _fn_chpl) {
#line 140 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpRuntime.chpl"
  c_ptr_chpl_task_infoChapel_t_chpl call_tmp_chpl6 = NULL;
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpRuntime.chpl"
  chpl_bool call_tmp_chpl7;
#line 140 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpRuntime.chpl"
  call_tmp_chpl6 = chpl_task_getInfoChapel();
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpRuntime.chpl"
  call_tmp_chpl7 = chpl_task_data_getSerial(call_tmp_chpl6, _ln_chpl, _fn_chpl);
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpRuntime.chpl"
  if (call_tmp_chpl7) /* ZLINE: 142 /Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpRuntime.chpl */
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpRuntime.chpl"
  {
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpRuntime.chpl"
    chpl_ftable_call(fn_chpl, args_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpRuntime.chpl"
  } else /* ZLINE: 144 /Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpRuntime.chpl */
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpRuntime.chpl"
  {
#line 145 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpRuntime.chpl"
    chpl_task_data_setup(args_chpl, call_tmp_chpl6, _ln_chpl, _fn_chpl);
#line 146 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpRuntime.chpl"
    chpl_task_addTask(fn_chpl, args_chpl, args_size_chpl, subloc_id_chpl, _ln_chpl, _fn_chpl);
#line 146 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpRuntime.chpl"
  }
#line 135 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpRuntime.chpl"
  return;
#line 135 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpRuntime.chpl"
}

#line 155 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpRuntime.chpl"
/* LocaleModelHelpRuntime.chpl:155 */
#line 155 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpRuntime.chpl"
static void chpl_taskAddCoStmt(int64_t subloc_id_chpl,
#line 155 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpRuntime.chpl"
                               int64_t fn_chpl,
#line 155 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpRuntime.chpl"
                               chpl_task_bundle_p args_chpl,
#line 155 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpRuntime.chpl"
                               uint64_t args_size_chpl,
#line 155 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpRuntime.chpl"
                               int64_t _ln_chpl,
#line 155 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpRuntime.chpl"
                               int32_t _fn_chpl) {
#line 160 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpRuntime.chpl"
  c_ptr_chpl_task_infoChapel_t_chpl call_tmp_chpl6 = NULL;
#line 161 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpRuntime.chpl"
  chpl_bool call_tmp_chpl7;
#line 161 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpRuntime.chpl"
  chpl_bool isSerial_chpl;
#line 162 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpRuntime.chpl"
  chpl_bool call_tmp_chpl8;
#line 160 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpRuntime.chpl"
  call_tmp_chpl6 = chpl_task_getInfoChapel();
#line 161 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpRuntime.chpl"
  call_tmp_chpl7 = chpl_task_data_getSerial(call_tmp_chpl6, _ln_chpl, _fn_chpl);
#line 161 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpRuntime.chpl"
  isSerial_chpl = call_tmp_chpl7;
#line 162 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpRuntime.chpl"
  call_tmp_chpl8 = chpl_task_data_getNextCoStmtSerial(call_tmp_chpl6, _ln_chpl, _fn_chpl);
#line 162 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpRuntime.chpl"
  if (call_tmp_chpl8) /* ZLINE: 162 /Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpRuntime.chpl */
#line 162 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpRuntime.chpl"
  {
#line 163 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpRuntime.chpl"
    isSerial_chpl = UINT8(true);
#line 164 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpRuntime.chpl"
    chpl_task_data_setNextCoStmtSerial(call_tmp_chpl6, UINT8(false), _ln_chpl, _fn_chpl);
#line 164 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpRuntime.chpl"
  }
#line 166 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpRuntime.chpl"
  if (isSerial_chpl) /* ZLINE: 166 /Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpRuntime.chpl */
#line 166 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpRuntime.chpl"
  {
#line 167 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpRuntime.chpl"
    chpl_ftable_call(fn_chpl, args_chpl);
#line 167 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpRuntime.chpl"
  } else /* ZLINE: 168 /Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpRuntime.chpl */
#line 167 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpRuntime.chpl"
  {
#line 169 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpRuntime.chpl"
    chpl_task_data_setup(args_chpl, call_tmp_chpl6, _ln_chpl, _fn_chpl);
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpRuntime.chpl"
    chpl_task_addTask(fn_chpl, args_chpl, args_size_chpl, subloc_id_chpl, _ln_chpl, _fn_chpl);
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpRuntime.chpl"
  }
#line 155 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpRuntime.chpl"
  return;
#line 155 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpRuntime.chpl"
}

