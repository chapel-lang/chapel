#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpFlat.chpl"
/* LocaleModelHelpFlat.chpl:21 */
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpFlat.chpl"
static void chpl__init_LocaleModelHelpFlat(int64_t _ln_chpl,
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpFlat.chpl"
                                           int32_t _fn_chpl) {
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpFlat.chpl"
  _ref_int32_t_chpl refIndentLevel_chpl = NULL;
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpFlat.chpl"
  if (chpl__init_LocaleModelHelpFlat_p) /* ZLINE: 21 /Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpFlat.chpl */
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpFlat.chpl"
  {
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpFlat.chpl"
    goto _exit_chpl__init_LocaleModelHelpFlat_chpl;
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpFlat.chpl"
  }
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpFlat.chpl"
  printModuleInit_chpl("%*s\n", "LocaleModelHelpFlat", INT64(19), _ln_chpl, _fn_chpl);
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpFlat.chpl"
  refIndentLevel_chpl = &moduleInitLevel_chpl;
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpFlat.chpl"
  *(refIndentLevel_chpl) += INT64(1);
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpFlat.chpl"
  chpl__init_LocaleModelHelpFlat_p = UINT8(true);
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpFlat.chpl"
  {
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpFlat.chpl"
    chpl__init_LocaleModelHelpSetup(_ln_chpl, _fn_chpl);
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpFlat.chpl"
    chpl__init_LocaleModelHelpRuntime(_ln_chpl, _fn_chpl);
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpFlat.chpl"
    chpl__init_CTypes(_ln_chpl, _fn_chpl);
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpFlat.chpl"
  }
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpFlat.chpl"
  *(refIndentLevel_chpl) -= INT64(1);
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpFlat.chpl"
  _exit_chpl__init_LocaleModelHelpFlat_chpl:;
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpFlat.chpl"
  return;
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpFlat.chpl"
}

#line 47 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpFlat.chpl"
/* LocaleModelHelpFlat.chpl:47 */
#line 47 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpFlat.chpl"
chpl_bool chpl_doDirectExecuteOn(chpl_localeID_t loc) {
#line 49 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpFlat.chpl"
  int32_t call_tmp_chpl6;
#line 50 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpFlat.chpl"
  int32_t call_tmp_chpl7;
#line 50 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpFlat.chpl"
  chpl_bool call_tmp_chpl8;
#line 49 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpFlat.chpl"
  call_tmp_chpl6 = chpl_nodeFromLocaleID(loc, INT64(49), INT32(52));
#line 50 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpFlat.chpl"
  call_tmp_chpl7 = get_chpl_nodeID();
#line 50 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpFlat.chpl"
  call_tmp_chpl8 = (call_tmp_chpl6 == call_tmp_chpl7);
#line 47 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpFlat.chpl"
  return call_tmp_chpl8;
#line 47 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpFlat.chpl"
}

#line 58 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpFlat.chpl"
/* LocaleModelHelpFlat.chpl:58 */
#line 58 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpFlat.chpl"
void chpl_executeOn(chpl_localeID_t loc,
#line 58 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpFlat.chpl"
                    int64_t fn,
#line 58 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpFlat.chpl"
                    chpl_comm_on_bundle_p args,
#line 58 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpFlat.chpl"
                    uint64_t args_size,
#line 58 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpFlat.chpl"
                    int64_t _ln,
#line 58 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpFlat.chpl"
                    int32_t _fn) {
#line 63 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpFlat.chpl"
  int32_t call_tmp_chpl6;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpFlat.chpl"
  int32_t call_tmp_chpl7;
#line 71 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpFlat.chpl"
  c_ptr_chpl_task_infoChapel_t_chpl call_tmp_chpl8 = NULL;
#line 72 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpFlat.chpl"
  chpl_task_bundle_p call_tmp_chpl9;
#line 73 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpFlat.chpl"
  int32_t call_tmp_chpl10;
#line 63 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpFlat.chpl"
  call_tmp_chpl6 = chpl_nodeFromLocaleID(loc, _ln, _fn);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpFlat.chpl"
  call_tmp_chpl7 = get_chpl_nodeID();
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpFlat.chpl"
  if (call_tmp_chpl6 == call_tmp_chpl7) /* ZLINE: 64 /Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpFlat.chpl */
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpFlat.chpl"
  {
#line 69 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpFlat.chpl"
    chpl_ftable_call(fn, args);
#line 69 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpFlat.chpl"
  } else /* ZLINE: 70 /Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpFlat.chpl */
#line 69 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpFlat.chpl"
  {
#line 71 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpFlat.chpl"
    call_tmp_chpl8 = chpl_task_getInfoChapel();
#line 72 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpFlat.chpl"
    call_tmp_chpl9 = chpl_comm_on_bundle_task_bundle(args);
#line 72 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpFlat.chpl"
    chpl_task_data_setup(call_tmp_chpl9, call_tmp_chpl8, _ln, _fn);
#line 73 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpFlat.chpl"
    call_tmp_chpl10 = chpl_sublocFromLocaleID(loc, _ln, _fn);
#line 73 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpFlat.chpl"
    chpl_comm_execute_on(((int64_t)(call_tmp_chpl6)), ((int64_t)(call_tmp_chpl10)), fn, args, args_size, _ln, _fn);
#line 73 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpFlat.chpl"
  }
#line 58 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpFlat.chpl"
  return;
#line 58 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpFlat.chpl"
}

#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpFlat.chpl"
/* LocaleModelHelpFlat.chpl:84 */
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpFlat.chpl"
void chpl_executeOnFast(chpl_localeID_t loc,
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpFlat.chpl"
                        int64_t fn,
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpFlat.chpl"
                        chpl_comm_on_bundle_p args,
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpFlat.chpl"
                        uint64_t args_size,
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpFlat.chpl"
                        int64_t _ln,
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpFlat.chpl"
                        int32_t _fn) {
#line 89 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpFlat.chpl"
  int32_t call_tmp_chpl6;
#line 90 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpFlat.chpl"
  int32_t call_tmp_chpl7;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpFlat.chpl"
  c_ptr_chpl_task_infoChapel_t_chpl call_tmp_chpl8 = NULL;
#line 95 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpFlat.chpl"
  chpl_task_bundle_p call_tmp_chpl9;
#line 96 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpFlat.chpl"
  int32_t call_tmp_chpl10;
#line 89 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpFlat.chpl"
  call_tmp_chpl6 = chpl_nodeFromLocaleID(loc, _ln, _fn);
#line 90 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpFlat.chpl"
  call_tmp_chpl7 = get_chpl_nodeID();
#line 90 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpFlat.chpl"
  if (call_tmp_chpl6 == call_tmp_chpl7) /* ZLINE: 90 /Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpFlat.chpl */
#line 90 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpFlat.chpl"
  {
#line 92 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpFlat.chpl"
    chpl_ftable_call(fn, args);
#line 92 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpFlat.chpl"
  } else /* ZLINE: 93 /Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpFlat.chpl */
#line 92 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpFlat.chpl"
  {
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpFlat.chpl"
    call_tmp_chpl8 = chpl_task_getInfoChapel();
#line 95 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpFlat.chpl"
    call_tmp_chpl9 = chpl_comm_on_bundle_task_bundle(args);
#line 95 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpFlat.chpl"
    chpl_task_data_setup(call_tmp_chpl9, call_tmp_chpl8, _ln, _fn);
#line 96 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpFlat.chpl"
    call_tmp_chpl10 = chpl_sublocFromLocaleID(loc, _ln, _fn);
#line 96 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpFlat.chpl"
    chpl_comm_execute_on_fast(((int64_t)(call_tmp_chpl6)), ((int64_t)(call_tmp_chpl10)), fn, args, args_size, _ln, _fn);
#line 96 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpFlat.chpl"
  }
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpFlat.chpl"
  return;
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpFlat.chpl"
}

#line 106 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpFlat.chpl"
/* LocaleModelHelpFlat.chpl:106 */
#line 106 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpFlat.chpl"
void chpl_executeOnNB(chpl_localeID_t loc,
#line 106 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpFlat.chpl"
                      int64_t fn,
#line 106 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpFlat.chpl"
                      chpl_comm_on_bundle_p args,
#line 106 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpFlat.chpl"
                      uint64_t args_size,
#line 106 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpFlat.chpl"
                      int64_t _ln,
#line 106 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpFlat.chpl"
                      int32_t _fn) {
#line 115 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpFlat.chpl"
  int32_t call_tmp_chpl6;
#line 116 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpFlat.chpl"
  c_ptr_chpl_task_infoChapel_t_chpl call_tmp_chpl7 = NULL;
#line 117 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpFlat.chpl"
  chpl_bool call_tmp_chpl8;
#line 118 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpFlat.chpl"
  int32_t call_tmp_chpl9;
#line 123 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpFlat.chpl"
  chpl_task_bundle_p call_tmp_chpl10;
#line 127 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpFlat.chpl"
  chpl_task_bundle_p call_tmp_chpl11;
#line 115 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpFlat.chpl"
  call_tmp_chpl6 = chpl_nodeFromLocaleID(loc, _ln, _fn);
#line 116 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpFlat.chpl"
  call_tmp_chpl7 = chpl_task_getInfoChapel();
#line 117 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpFlat.chpl"
  call_tmp_chpl8 = chpl_task_data_getSerial(call_tmp_chpl7, _ln, _fn);
#line 118 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpFlat.chpl"
  call_tmp_chpl9 = get_chpl_nodeID();
#line 118 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpFlat.chpl"
  if (call_tmp_chpl6 == call_tmp_chpl9) /* ZLINE: 118 /Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpFlat.chpl */
#line 118 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpFlat.chpl"
  {
#line 120 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpFlat.chpl"
    if (call_tmp_chpl8) /* ZLINE: 120 /Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpFlat.chpl */
#line 120 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpFlat.chpl"
    {
#line 121 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpFlat.chpl"
      chpl_ftable_call(fn, args);
#line 121 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpFlat.chpl"
    } else /* ZLINE: 122 /Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpFlat.chpl */
#line 121 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpFlat.chpl"
    {
#line 123 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpFlat.chpl"
      call_tmp_chpl10 = chpl_comm_on_bundle_task_bundle(args);
#line 123 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpFlat.chpl"
      chpl_task_data_setup(call_tmp_chpl10, call_tmp_chpl7, _ln, _fn);
#line 124 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpFlat.chpl"
      chpl_comm_taskCallFTable(fn, args, args_size, ((int64_t)(c_sublocid_any)), _ln, _fn);
#line 124 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpFlat.chpl"
    }
#line 124 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpFlat.chpl"
  } else /* ZLINE: 126 /Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpFlat.chpl */
#line 124 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpFlat.chpl"
  {
#line 127 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpFlat.chpl"
    call_tmp_chpl11 = chpl_comm_on_bundle_task_bundle(args);
#line 127 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpFlat.chpl"
    chpl_task_data_setup(call_tmp_chpl11, call_tmp_chpl7, _ln, _fn);
#line 128 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpFlat.chpl"
    if (call_tmp_chpl8) /* ZLINE: 128 /Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpFlat.chpl */
#line 128 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpFlat.chpl"
    {
#line 129 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpFlat.chpl"
      chpl_comm_execute_on(((int64_t)(call_tmp_chpl6)), ((int64_t)(c_sublocid_any)), fn, args, args_size, _ln, _fn);
#line 129 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpFlat.chpl"
    } else /* ZLINE: 130 /Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpFlat.chpl */
#line 129 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpFlat.chpl"
    {
#line 131 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpFlat.chpl"
      chpl_comm_execute_on_nb(((int64_t)(call_tmp_chpl6)), ((int64_t)(c_sublocid_any)), fn, args, args_size, _ln, _fn);
#line 131 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpFlat.chpl"
    }
#line 131 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpFlat.chpl"
  }
#line 106 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpFlat.chpl"
  return;
#line 106 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpFlat.chpl"
}

