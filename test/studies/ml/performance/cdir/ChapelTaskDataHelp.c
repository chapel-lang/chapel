#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskDataHelp.chpl"
/* ChapelTaskDataHelp.chpl:23 */
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskDataHelp.chpl"
static void chpl__init_ChapelTaskDataHelp(int64_t _ln_chpl,
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskDataHelp.chpl"
                                          int32_t _fn_chpl) {
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskDataHelp.chpl"
  _ref_int32_t_chpl refIndentLevel_chpl = NULL;
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskDataHelp.chpl"
  if (chpl__init_ChapelTaskDataHelp_p) /* ZLINE: 23 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskDataHelp.chpl */
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskDataHelp.chpl"
  {
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskDataHelp.chpl"
    goto _exit_chpl__init_ChapelTaskDataHelp_chpl;
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskDataHelp.chpl"
  }
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskDataHelp.chpl"
  printModuleInit_chpl("%*s\n", "ChapelTaskDataHelp", INT64(18), _ln_chpl, _fn_chpl);
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskDataHelp.chpl"
  refIndentLevel_chpl = &moduleInitLevel_chpl;
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskDataHelp.chpl"
  *(refIndentLevel_chpl) += INT64(1);
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskDataHelp.chpl"
  chpl__init_ChapelTaskDataHelp_p = UINT8(true);
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskDataHelp.chpl"
  {
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskDataHelp.chpl"
    chpl__init_CTypes(_ln_chpl, _fn_chpl);
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskDataHelp.chpl"
    chpl__init_POSIX(_ln_chpl, _fn_chpl);
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskDataHelp.chpl"
  }
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskDataHelp.chpl"
  *(refIndentLevel_chpl) -= INT64(1);
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskDataHelp.chpl"
  _exit_chpl__init_ChapelTaskDataHelp_chpl:;
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskDataHelp.chpl"
  return;
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskDataHelp.chpl"
}

#line 38 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskDataHelp.chpl"
/* ChapelTaskDataHelp.chpl:38 */
#line 38 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskDataHelp.chpl"
static void chpl_task_data_setup(chpl_task_bundle_p args_chpl,
#line 38 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskDataHelp.chpl"
                                 chpl_task_infoChapel_t * infoChapel_chpl,
#line 38 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskDataHelp.chpl"
                                 int64_t _ln_chpl,
#line 38 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskDataHelp.chpl"
                                 int32_t _fn_chpl) {
#line 40 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskDataHelp.chpl"
  c_ptr_chpl_task_infoChapel_t_chpl call_tmp_chpl6 = NULL;
#line 41 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskDataHelp.chpl"
  uint64_t call_tmp_chpl7;
#line 40 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskDataHelp.chpl"
  call_tmp_chpl6 = chpl_task_getInfoChapelInBundle(args_chpl);
#line 41 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskDataHelp.chpl"
  call_tmp_chpl7 = sizeof(chpl_task_infoChapel_t);
#line 40 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskDataHelp.chpl"
  memcpy(((c_ptr_void_chpl)(call_tmp_chpl6)), ((c_ptr_void_chpl)(infoChapel_chpl)), call_tmp_chpl7);
#line 38 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskDataHelp.chpl"
  return;
#line 38 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskDataHelp.chpl"
}

#line 46 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskDataHelp.chpl"
/* ChapelTaskDataHelp.chpl:46 */
#line 46 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskDataHelp.chpl"
static void chpl_save_task_error(chpl___EndCountBase e_chpl,
#line 46 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskDataHelp.chpl"
                                 Error_chpl err_chpl8,
#line 46 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskDataHelp.chpl"
                                 int64_t _ln_chpl,
#line 46 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskDataHelp.chpl"
                                 int32_t _fn_chpl) {
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskDataHelp.chpl"
  chpl___EndCountBase coerce_tmp_chpl3 = NULL;
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskDataHelp.chpl"
  _ref_chpl_TaskErrors_chpl call_tmp_chpl6 = NULL;
#line 47 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskDataHelp.chpl"
  if (((RootClass_chpl)(err_chpl8)) != nil) /* ZLINE: 47 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskDataHelp.chpl */
#line 47 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskDataHelp.chpl"
  {
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskDataHelp.chpl"
    coerce_tmp_chpl3 = e_chpl;
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskDataHelp.chpl"
    chpl_check_nil(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskDataHelp.chpl"
    call_tmp_chpl6 = &((coerce_tmp_chpl3)->errors);
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskDataHelp.chpl"
    append_chpl(call_tmp_chpl6, err_chpl8, _ln_chpl, _fn_chpl);
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskDataHelp.chpl"
  }
#line 46 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskDataHelp.chpl"
  return;
#line 46 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskDataHelp.chpl"
}

