#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
/* ChapelTaskData.chpl:23 */
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
static void chpl__init_ChapelTaskData(int64_t _ln_chpl,
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
                                      int32_t _fn_chpl) {
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  _ref_int32_t_chpl refIndentLevel_chpl = NULL;
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  uint64_t call_tmp_chpl6;
#line 177 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  uint64_t call_tmp_chpl7;
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  if (chpl__init_ChapelTaskData_p) /* ZLINE: 23 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl */
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  {
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
    goto _exit_chpl__init_ChapelTaskData_chpl;
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  }
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  printModuleInit_chpl("%*s\n", "ChapelTaskData", INT64(14), _ln_chpl, _fn_chpl);
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  refIndentLevel_chpl = &moduleInitLevel_chpl;
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  *(refIndentLevel_chpl) += INT64(1);
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  chpl__init_ChapelTaskData_p = UINT8(true);
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  {
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
    chpl__init_CTypes(_ln_chpl, _fn_chpl);
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
    chpl__init_POSIX(_ln_chpl, _fn_chpl);
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  }
#line 32 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  chpl_offset_endCount = UINT64(0);
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  call_tmp_chpl6 = sizeof_endcount_ptr_chpl();
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  chpl_offset_serial = call_tmp_chpl6;
#line 34 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  chpl_offset_nextCoStmtSerial = ((uint64_t)((chpl_offset_serial + UINT64(1))));
#line 35 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  chpl_offset_commDiagsTemporarilyDisabled = ((uint64_t)((chpl_offset_nextCoStmtSerial + UINT64(1))));
#line 36 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  chpl_offset_end = ((uint64_t)((chpl_offset_commDiagsTemporarilyDisabled + UINT64(1))));
#line 177 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  call_tmp_chpl7 = sizeof(chpl_task_infoChapel_t);
#line 177 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  assert_chpl((chpl_offset_end <= call_tmp_chpl7), _ln_chpl, _fn_chpl);
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  *(refIndentLevel_chpl) -= INT64(1);
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  _exit_chpl__init_ChapelTaskData_chpl:;
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  return;
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
}

#line 40 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
/* ChapelTaskData.chpl:40 */
#line 40 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
static uint64_t sizeof_endcount_ptr_chpl(void) {
#line 41 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  uint64_t call_tmp_chpl6;
#line 41 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  uint64_t call_tmp_chpl7;
#line 41 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  uint64_t call_tmp_chpl8;
#line 41 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  call_tmp_chpl6 = sizeof(chpl_localeID_t);
#line 41 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  call_tmp_chpl7 = sizeof(c_ptr_void_chpl);
#line 41 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  call_tmp_chpl8 = (call_tmp_chpl6 + call_tmp_chpl7);
#line 40 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  return call_tmp_chpl8;
#line 40 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
}

#line 46 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
/* ChapelTaskData.chpl:46 */
#line 46 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
static void chpl_task_data_setDynamicEndCount(chpl_task_infoChapel_t * tls_chpl,
#line 46 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
                                              chpl___EndCount_AtomicT_int64_t_int64_t end_chpl,
#line 46 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
                                              int64_t _ln_chpl,
#line 46 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
                                              int32_t _fn_chpl) {
#line 47 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  c_ptr_uint8_t_chpl prv_chpl = NULL;
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  uint64_t i_chpl;
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  chpl_localeID_t call_tmp_chpl6;
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  chpl_localeID_t loc_chpl;
#line 52 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  raw_c_void_ptr adr_chpl;
#line 56 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  _ref_uint8_t_chpl call_tmp_chpl7 = NULL;
#line 56 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  c_ptr_uint8_t_chpl call_tmp_chpl8 = NULL;
#line 56 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  _ref_chpl_localeID_t_chpl i_x_chpl = NULL;
#line 56 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  c_ptr_chpl_localeID_t_chpl call_tmp_chpl9 = NULL;
#line 56 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  uint64_t call_tmp_chpl10;
#line 59 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  uint64_t call_tmp_chpl11;
#line 59 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  _ref_uint64_t_chpl i_lhs_chpl = NULL;
#line 60 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  _ref_uint8_t_chpl call_tmp_chpl12 = NULL;
#line 60 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  c_ptr_uint8_t_chpl call_tmp_chpl13 = NULL;
#line 60 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  _ref_raw_c_void_ptr_chpl i_x_chpl2 = NULL;
#line 60 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  c_ptr_raw_c_void_ptr_chpl call_tmp_chpl14 = NULL;
#line 60 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  uint64_t call_tmp_chpl15;
#line 47 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  prv_chpl = ((c_ptr_uint8_t_chpl)(tls_chpl));
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  call_tmp_chpl6 = chpl_gen_getLocaleID();
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  loc_chpl = call_tmp_chpl6;
#line 52 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  adr_chpl = ((raw_c_void_ptr)(end_chpl));
#line 55 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  i_chpl = chpl_offset_endCount;
#line 56 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  call_tmp_chpl7 = (prv_chpl + i_chpl);
#line 56 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  call_tmp_chpl8 = c_pointer_return(call_tmp_chpl7);
#line 56 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  i_x_chpl = &loc_chpl;
#line 56 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  call_tmp_chpl9 = c_pointer_return(i_x_chpl);
#line 56 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  call_tmp_chpl10 = sizeof(chpl_localeID_t);
#line 56 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  memcpy(((c_ptr_void_chpl)(call_tmp_chpl8)), ((c_ptr_void_chpl)(call_tmp_chpl9)), call_tmp_chpl10);
#line 59 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  call_tmp_chpl11 = sizeof(chpl_localeID_t);
#line 59 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  i_lhs_chpl = &i_chpl;
#line 59 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  *(i_lhs_chpl) += call_tmp_chpl11;
#line 60 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  call_tmp_chpl12 = (prv_chpl + i_chpl);
#line 60 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  call_tmp_chpl13 = c_pointer_return(call_tmp_chpl12);
#line 60 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  i_x_chpl2 = &adr_chpl;
#line 60 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  call_tmp_chpl14 = c_pointer_return(i_x_chpl2);
#line 60 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  call_tmp_chpl15 = sizeof(c_ptr_void_chpl);
#line 60 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  memcpy(((c_ptr_void_chpl)(call_tmp_chpl13)), ((c_ptr_void_chpl)(call_tmp_chpl14)), call_tmp_chpl15);
#line 46 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  return;
#line 46 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
}

#line 63 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
/* ChapelTaskData.chpl:63 */
#line 63 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
static chpl___EndCount_AtomicT_int64_t_int64_t chpl_task_data_getDynamicEndCount(chpl_task_infoChapel_t * tls_chpl,
#line 63 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
                                                                                 int64_t _ln_chpl,
#line 63 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
                                                                                 int32_t _fn_chpl) {
#line 71 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  uint64_t local_chpl_offset_endCount_chpl;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  c_ptr_uint8_t_chpl prv_chpl = NULL;
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  uint64_t i_chpl;
#line 67 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  chpl_localeID_t loc_chpl;
#line 68 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  c_ptr_void_chpl adr_chpl = NULL;
#line 72 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  _ref_chpl_localeID_t_chpl i_x_chpl = NULL;
#line 72 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  c_ptr_chpl_localeID_t_chpl call_tmp_chpl6 = NULL;
#line 72 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  _ref_uint8_t_chpl call_tmp_chpl7 = NULL;
#line 72 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  c_ptr_uint8_t_chpl call_tmp_chpl8 = NULL;
#line 72 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  uint64_t call_tmp_chpl9;
#line 75 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  uint64_t call_tmp_chpl10;
#line 75 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  _ref_uint64_t_chpl i_lhs_chpl = NULL;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  _ref_c_ptr_void_chpl i_x_chpl2 = NULL;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  c_ptr_c_ptr_void_chpl call_tmp_chpl11 = NULL;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  _ref_uint8_t_chpl call_tmp_chpl12 = NULL;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  c_ptr_uint8_t_chpl call_tmp_chpl13 = NULL;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  uint64_t call_tmp_chpl14;
#line 79 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  chpl___EndCount_AtomicT_int64_t_int64_t call_tmp_chpl15 = NULL;
#line 71 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  local_chpl_offset_endCount_chpl = chpl_offset_endCount;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  prv_chpl = ((c_ptr_uint8_t_chpl)(tls_chpl));
#line 67 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  memset(&loc_chpl, INT32(0), sizeof(chpl_localeID_t));
#line 68 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  adr_chpl = ((c_ptr_void_chpl)(nil));
#line 71 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  i_chpl = local_chpl_offset_endCount_chpl;
#line 72 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  i_x_chpl = &loc_chpl;
#line 72 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  call_tmp_chpl6 = c_pointer_return(i_x_chpl);
#line 72 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  call_tmp_chpl7 = (prv_chpl + i_chpl);
#line 72 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  call_tmp_chpl8 = c_pointer_return(call_tmp_chpl7);
#line 72 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  call_tmp_chpl9 = sizeof(chpl_localeID_t);
#line 72 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  memcpy(((c_ptr_void_chpl)(call_tmp_chpl6)), ((c_ptr_void_chpl)(call_tmp_chpl8)), call_tmp_chpl9);
#line 75 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  call_tmp_chpl10 = sizeof(chpl_localeID_t);
#line 75 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  i_lhs_chpl = &i_chpl;
#line 75 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  *(i_lhs_chpl) += call_tmp_chpl10;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  i_x_chpl2 = &adr_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  call_tmp_chpl11 = c_pointer_return(i_x_chpl2);
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  call_tmp_chpl12 = (prv_chpl + i_chpl);
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  call_tmp_chpl13 = c_pointer_return(call_tmp_chpl12);
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  call_tmp_chpl14 = sizeof(c_ptr_void_chpl);
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  memcpy(((c_ptr_void_chpl)(call_tmp_chpl11)), ((c_ptr_void_chpl)(call_tmp_chpl13)), call_tmp_chpl14);
#line 79 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  call_tmp_chpl15 = ((chpl___EndCount_AtomicT_int64_t_int64_t)(adr_chpl));
#line 63 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  return call_tmp_chpl15;
#line 63 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
}

#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
/* ChapelTaskData.chpl:84 */
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
static void chpl_task_data_setSerial(chpl_task_infoChapel_t * tls_chpl,
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
                                     chpl_bool makeSerial_chpl,
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
                                     int64_t _ln_chpl,
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
                                     int32_t _fn_chpl) {
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  c_ptr_uint8_t_chpl prv_chpl = NULL;
#line 86 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  uint64_t i_chpl;
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  uint8_t v_chpl;
#line 91 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  _ref_uint8_t_chpl call_tmp_chpl6 = NULL;
#line 91 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  c_ptr_uint8_t_chpl call_tmp_chpl7 = NULL;
#line 91 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  _ref_uint8_t_chpl i_x_chpl = NULL;
#line 91 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  c_ptr_uint8_t_chpl call_tmp_chpl8 = NULL;
#line 91 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  uint64_t call_tmp_chpl9;
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  prv_chpl = ((c_ptr_uint8_t_chpl)(tls_chpl));
#line 86 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  i_chpl = chpl_offset_serial;
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  v_chpl = UINT8(0);
#line 88 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  if (makeSerial_chpl) /* ZLINE: 88 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl */
#line 88 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  {
#line 89 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
    v_chpl = ((uint8_t)(INT64(1)));
#line 89 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  }
#line 91 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  call_tmp_chpl6 = (prv_chpl + i_chpl);
#line 91 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  call_tmp_chpl7 = c_pointer_return(call_tmp_chpl6);
#line 91 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  i_x_chpl = &v_chpl;
#line 91 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  call_tmp_chpl8 = c_pointer_return(i_x_chpl);
#line 91 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  call_tmp_chpl9 = sizeof(uint8_t);
#line 91 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  memcpy(((c_ptr_void_chpl)(call_tmp_chpl7)), ((c_ptr_void_chpl)(call_tmp_chpl8)), call_tmp_chpl9);
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  return;
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
}

#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
/* ChapelTaskData.chpl:93 */
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
static chpl_bool chpl_task_data_getSerial(chpl_task_infoChapel_t * tls_chpl,
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
                                          int64_t _ln_chpl,
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
                                          int32_t _fn_chpl) {
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  c_ptr_uint8_t_chpl prv_chpl = NULL;
#line 95 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  uint64_t i_chpl;
#line 96 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  uint8_t v_chpl;
#line 98 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  _ref_uint8_t_chpl i_x_chpl = NULL;
#line 98 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  c_ptr_uint8_t_chpl call_tmp_chpl6 = NULL;
#line 98 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  _ref_uint8_t_chpl call_tmp_chpl7 = NULL;
#line 98 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  c_ptr_uint8_t_chpl call_tmp_chpl8 = NULL;
#line 98 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  uint64_t call_tmp_chpl9;
#line 103 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  int8_t coerce_tmp_chpl3;
#line 103 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  chpl_bool tmp_chpl;
#line 103 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  chpl_bool tmp_chpl2;
#line 103 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  int8_t coerce_tmp_chpl4;
#line 103 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  chpl_bool tmp_chpl3;
#line 104 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  int8_t coerce_tmp_chpl5;
#line 104 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  chpl_bool tmp_chpl4;
#line 94 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  prv_chpl = ((c_ptr_uint8_t_chpl)(tls_chpl));
#line 95 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  i_chpl = chpl_offset_serial;
#line 96 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  v_chpl = UINT8(0);
#line 98 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  i_x_chpl = &v_chpl;
#line 98 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  call_tmp_chpl6 = c_pointer_return(i_x_chpl);
#line 98 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  call_tmp_chpl7 = (prv_chpl + i_chpl);
#line 98 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  call_tmp_chpl8 = c_pointer_return(call_tmp_chpl7);
#line 98 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  call_tmp_chpl9 = sizeof(uint8_t);
#line 98 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  memcpy(((c_ptr_void_chpl)(call_tmp_chpl6)), ((c_ptr_void_chpl)(call_tmp_chpl8)), call_tmp_chpl9);
#line 103 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  coerce_tmp_chpl3 = ((int8_t)(INT64(0)));
#line 103 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  if (! (coerce_tmp_chpl3 < INT8(0))) /* ZLINE: 103 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl */
#line 103 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  {
#line 103 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
    tmp_chpl = (v_chpl == ((uint8_t)(coerce_tmp_chpl3)));
#line 103 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  } else /* ZLINE: 103 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl */
#line 103 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  {
#line 103 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
    tmp_chpl = UINT8(false);
#line 103 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  }
#line 103 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  if (tmp_chpl) /* ZLINE: 103 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl */
#line 103 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  {
#line 103 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
    tmp_chpl2 = UINT8(true);
#line 103 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  } else /* ZLINE: 103 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl */
#line 103 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  {
#line 103 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
    coerce_tmp_chpl4 = ((int8_t)(INT64(1)));
#line 103 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
    if (! (coerce_tmp_chpl4 < INT8(0))) /* ZLINE: 103 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl */
#line 103 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
    {
#line 103 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
      tmp_chpl3 = (v_chpl == ((uint8_t)(coerce_tmp_chpl4)));
#line 103 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
    } else /* ZLINE: 103 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl */
#line 103 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
    {
#line 103 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
      tmp_chpl3 = UINT8(false);
#line 103 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
    }
#line 103 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
    tmp_chpl2 = tmp_chpl3;
#line 103 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  }
#line 103 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  assert_chpl(tmp_chpl2, _ln_chpl, _fn_chpl);
#line 104 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  coerce_tmp_chpl5 = ((int8_t)(INT64(1)));
#line 104 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  if (! (coerce_tmp_chpl5 < INT8(0))) /* ZLINE: 104 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl */
#line 104 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  {
#line 104 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
    tmp_chpl4 = (v_chpl == ((uint8_t)(coerce_tmp_chpl5)));
#line 104 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  } else /* ZLINE: 104 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl */
#line 104 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  {
#line 104 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
    tmp_chpl4 = UINT8(false);
#line 104 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  }
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  return tmp_chpl4;
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
}

#line 107 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
/* ChapelTaskData.chpl:107 */
#line 107 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
static void chpl_task_data_setNextCoStmtSerial(chpl_task_infoChapel_t * tls_chpl,
#line 107 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
                                               chpl_bool makeSerial_chpl,
#line 107 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
                                               int64_t _ln_chpl,
#line 107 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
                                               int32_t _fn_chpl) {
#line 108 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  c_ptr_uint8_t_chpl prv_chpl = NULL;
#line 109 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  uint64_t i_chpl;
#line 110 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  uint8_t v_chpl;
#line 113 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  _ref_uint8_t_chpl call_tmp_chpl6 = NULL;
#line 113 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  c_ptr_uint8_t_chpl call_tmp_chpl7 = NULL;
#line 113 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  _ref_uint8_t_chpl i_x_chpl = NULL;
#line 113 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  c_ptr_uint8_t_chpl call_tmp_chpl8 = NULL;
#line 113 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  uint64_t call_tmp_chpl9;
#line 108 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  prv_chpl = ((c_ptr_uint8_t_chpl)(tls_chpl));
#line 109 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  i_chpl = chpl_offset_nextCoStmtSerial;
#line 110 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  v_chpl = UINT8(0);
#line 111 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  if (makeSerial_chpl) /* ZLINE: 111 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl */
#line 111 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  {
#line 112 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
    v_chpl = ((uint8_t)(INT64(1)));
#line 112 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  }
#line 113 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  call_tmp_chpl6 = (prv_chpl + i_chpl);
#line 113 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  call_tmp_chpl7 = c_pointer_return(call_tmp_chpl6);
#line 113 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  i_x_chpl = &v_chpl;
#line 113 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  call_tmp_chpl8 = c_pointer_return(i_x_chpl);
#line 113 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  call_tmp_chpl9 = sizeof(uint8_t);
#line 113 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  memcpy(((c_ptr_void_chpl)(call_tmp_chpl7)), ((c_ptr_void_chpl)(call_tmp_chpl8)), call_tmp_chpl9);
#line 107 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  return;
#line 107 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
}

#line 116 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
/* ChapelTaskData.chpl:116 */
#line 116 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
static chpl_bool chpl_task_data_getNextCoStmtSerial(chpl_task_infoChapel_t * tls_chpl,
#line 116 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
                                                    int64_t _ln_chpl,
#line 116 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
                                                    int32_t _fn_chpl) {
#line 117 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  c_ptr_uint8_t_chpl prv_chpl = NULL;
#line 118 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  uint64_t i_chpl;
#line 119 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  uint8_t v_chpl;
#line 120 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  _ref_uint8_t_chpl i_x_chpl = NULL;
#line 120 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  c_ptr_uint8_t_chpl call_tmp_chpl6 = NULL;
#line 120 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  _ref_uint8_t_chpl call_tmp_chpl7 = NULL;
#line 120 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  c_ptr_uint8_t_chpl call_tmp_chpl8 = NULL;
#line 120 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  uint64_t call_tmp_chpl9;
#line 122 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  int8_t coerce_tmp_chpl3;
#line 122 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  chpl_bool tmp_chpl;
#line 122 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  chpl_bool tmp_chpl2;
#line 122 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  int8_t coerce_tmp_chpl4;
#line 122 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  chpl_bool tmp_chpl3;
#line 123 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  int8_t coerce_tmp_chpl5;
#line 123 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  chpl_bool tmp_chpl4;
#line 117 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  prv_chpl = ((c_ptr_uint8_t_chpl)(tls_chpl));
#line 118 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  i_chpl = chpl_offset_nextCoStmtSerial;
#line 119 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  v_chpl = UINT8(0);
#line 120 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  i_x_chpl = &v_chpl;
#line 120 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  call_tmp_chpl6 = c_pointer_return(i_x_chpl);
#line 120 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  call_tmp_chpl7 = (prv_chpl + i_chpl);
#line 120 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  call_tmp_chpl8 = c_pointer_return(call_tmp_chpl7);
#line 120 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  call_tmp_chpl9 = sizeof(uint8_t);
#line 120 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  memcpy(((c_ptr_void_chpl)(call_tmp_chpl6)), ((c_ptr_void_chpl)(call_tmp_chpl8)), call_tmp_chpl9);
#line 122 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  coerce_tmp_chpl3 = ((int8_t)(INT64(0)));
#line 122 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  if (! (coerce_tmp_chpl3 < INT8(0))) /* ZLINE: 122 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl */
#line 122 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  {
#line 122 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
    tmp_chpl = (v_chpl == ((uint8_t)(coerce_tmp_chpl3)));
#line 122 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  } else /* ZLINE: 122 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl */
#line 122 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  {
#line 122 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
    tmp_chpl = UINT8(false);
#line 122 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  }
#line 122 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  if (tmp_chpl) /* ZLINE: 122 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl */
#line 122 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  {
#line 122 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
    tmp_chpl2 = UINT8(true);
#line 122 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  } else /* ZLINE: 122 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl */
#line 122 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  {
#line 122 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
    coerce_tmp_chpl4 = ((int8_t)(INT64(1)));
#line 122 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
    if (! (coerce_tmp_chpl4 < INT8(0))) /* ZLINE: 122 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl */
#line 122 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
    {
#line 122 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
      tmp_chpl3 = (v_chpl == ((uint8_t)(coerce_tmp_chpl4)));
#line 122 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
    } else /* ZLINE: 122 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl */
#line 122 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
    {
#line 122 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
      tmp_chpl3 = UINT8(false);
#line 122 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
    }
#line 122 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
    tmp_chpl2 = tmp_chpl3;
#line 122 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  }
#line 122 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  assert_chpl(tmp_chpl2, _ln_chpl, _fn_chpl);
#line 123 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  coerce_tmp_chpl5 = ((int8_t)(INT64(1)));
#line 123 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  if (! (coerce_tmp_chpl5 < INT8(0))) /* ZLINE: 123 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl */
#line 123 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  {
#line 123 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
    tmp_chpl4 = (v_chpl == ((uint8_t)(coerce_tmp_chpl5)));
#line 123 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  } else /* ZLINE: 123 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl */
#line 123 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  {
#line 123 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
    tmp_chpl4 = UINT8(false);
#line 123 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  }
#line 116 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  return tmp_chpl4;
#line 116 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
}

#line 126 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
/* ChapelTaskData.chpl:126 */
#line 126 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
static chpl_bool chpl_task_data_setCommDiagsTemporarilyDisabled(chpl_task_infoChapel_t * tls_chpl,
#line 126 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
                                                                chpl_bool disabled_chpl,
#line 126 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
                                                                int64_t _ln_chpl,
#line 126 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
                                                                int32_t _fn_chpl) {
#line 129 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  uint64_t local_chpl_offset_commDiagsTemporarilyDisabled_chpl;
#line 127 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  chpl_bool call_tmp_chpl6;
#line 128 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  c_ptr_uint8_t_chpl prv_chpl = NULL;
#line 129 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  uint64_t i_chpl;
#line 130 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  uint8_t v_chpl;
#line 133 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  _ref_uint8_t_chpl call_tmp_chpl7 = NULL;
#line 133 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  c_ptr_uint8_t_chpl call_tmp_chpl8 = NULL;
#line 133 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  _ref_uint8_t_chpl i_x_chpl = NULL;
#line 133 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  c_ptr_uint8_t_chpl call_tmp_chpl9 = NULL;
#line 133 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  uint64_t call_tmp_chpl10;
#line 129 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  local_chpl_offset_commDiagsTemporarilyDisabled_chpl = chpl_offset_commDiagsTemporarilyDisabled;
#line 127 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  call_tmp_chpl6 = chpl_task_data_getCommDiagsTemporarilyDisabled(tls_chpl, _ln_chpl, _fn_chpl);
#line 128 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  prv_chpl = ((c_ptr_uint8_t_chpl)(tls_chpl));
#line 129 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  i_chpl = local_chpl_offset_commDiagsTemporarilyDisabled_chpl;
#line 130 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  v_chpl = UINT8(0);
#line 131 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  if (disabled_chpl) /* ZLINE: 131 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl */
#line 131 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  {
#line 132 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
    v_chpl = ((uint8_t)(INT64(1)));
#line 132 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  }
#line 133 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  call_tmp_chpl7 = (prv_chpl + i_chpl);
#line 133 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  call_tmp_chpl8 = c_pointer_return(call_tmp_chpl7);
#line 133 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  i_x_chpl = &v_chpl;
#line 133 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  call_tmp_chpl9 = c_pointer_return(i_x_chpl);
#line 133 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  call_tmp_chpl10 = sizeof(uint8_t);
#line 133 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  memcpy(((c_ptr_void_chpl)(call_tmp_chpl8)), ((c_ptr_void_chpl)(call_tmp_chpl9)), call_tmp_chpl10);
#line 126 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  return call_tmp_chpl6;
#line 126 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
}

#line 137 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
/* ChapelTaskData.chpl:137 */
#line 137 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
static chpl_bool chpl_task_data_getCommDiagsTemporarilyDisabled(chpl_task_infoChapel_t * tls_chpl,
#line 137 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
                                                                int64_t _ln_chpl,
#line 137 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
                                                                int32_t _fn_chpl) {
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  c_ptr_uint8_t_chpl prv_chpl = NULL;
#line 139 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  uint64_t i_chpl;
#line 140 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  uint8_t v_chpl;
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  _ref_uint8_t_chpl i_x_chpl = NULL;
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  c_ptr_uint8_t_chpl call_tmp_chpl6 = NULL;
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  _ref_uint8_t_chpl call_tmp_chpl7 = NULL;
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  c_ptr_uint8_t_chpl call_tmp_chpl8 = NULL;
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  uint64_t call_tmp_chpl9;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  int8_t coerce_tmp_chpl3;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  chpl_bool tmp_chpl;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  chpl_bool tmp_chpl2;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  int8_t coerce_tmp_chpl4;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  chpl_bool tmp_chpl3;
#line 144 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  int8_t coerce_tmp_chpl5;
#line 144 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  chpl_bool tmp_chpl4;
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  prv_chpl = ((c_ptr_uint8_t_chpl)(tls_chpl));
#line 139 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  i_chpl = chpl_offset_commDiagsTemporarilyDisabled;
#line 140 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  v_chpl = UINT8(0);
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  i_x_chpl = &v_chpl;
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  call_tmp_chpl6 = c_pointer_return(i_x_chpl);
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  call_tmp_chpl7 = (prv_chpl + i_chpl);
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  call_tmp_chpl8 = c_pointer_return(call_tmp_chpl7);
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  call_tmp_chpl9 = sizeof(uint8_t);
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  memcpy(((c_ptr_void_chpl)(call_tmp_chpl6)), ((c_ptr_void_chpl)(call_tmp_chpl8)), call_tmp_chpl9);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  coerce_tmp_chpl3 = ((int8_t)(INT64(0)));
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  if (! (coerce_tmp_chpl3 < INT8(0))) /* ZLINE: 143 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl */
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  {
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
    tmp_chpl = (v_chpl == ((uint8_t)(coerce_tmp_chpl3)));
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  } else /* ZLINE: 143 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl */
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  {
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
    tmp_chpl = UINT8(false);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  }
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  if (tmp_chpl) /* ZLINE: 143 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl */
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  {
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
    tmp_chpl2 = UINT8(true);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  } else /* ZLINE: 143 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl */
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  {
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
    coerce_tmp_chpl4 = ((int8_t)(INT64(1)));
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
    if (! (coerce_tmp_chpl4 < INT8(0))) /* ZLINE: 143 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl */
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
    {
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
      tmp_chpl3 = (v_chpl == ((uint8_t)(coerce_tmp_chpl4)));
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
    } else /* ZLINE: 143 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl */
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
    {
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
      tmp_chpl3 = UINT8(false);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
    }
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
    tmp_chpl2 = tmp_chpl3;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  }
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  assert_chpl(tmp_chpl2, _ln_chpl, _fn_chpl);
#line 144 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  coerce_tmp_chpl5 = ((int8_t)(INT64(1)));
#line 144 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  if (! (coerce_tmp_chpl5 < INT8(0))) /* ZLINE: 144 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl */
#line 144 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  {
#line 144 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
    tmp_chpl4 = (v_chpl == ((uint8_t)(coerce_tmp_chpl5)));
#line 144 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  } else /* ZLINE: 144 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl */
#line 144 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  {
#line 144 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
    tmp_chpl4 = UINT8(false);
#line 144 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  }
#line 137 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  return tmp_chpl4;
#line 137 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
}

#line 152 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
/* ChapelTaskData.chpl:152 */
#line 152 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
static void chpl_task_setDynamicEndCount(chpl___EndCount_AtomicT_int64_t_int64_t end_chpl,
#line 152 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
                                         int64_t _ln_chpl,
#line 152 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
                                         int32_t _fn_chpl) {
#line 153 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  c_ptr_chpl_task_infoChapel_t_chpl call_tmp_chpl6 = NULL;
#line 153 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  call_tmp_chpl6 = chpl_task_getInfoChapel();
#line 153 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  chpl_task_data_setDynamicEndCount(call_tmp_chpl6, end_chpl, _ln_chpl, _fn_chpl);
#line 152 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  return;
#line 152 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
}

#line 155 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
/* ChapelTaskData.chpl:155 */
#line 155 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
static chpl___EndCount_AtomicT_int64_t_int64_t chpl_task_getDynamicEndCount(int64_t _ln_chpl,
#line 155 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
                                                                            int32_t _fn_chpl) {
#line 156 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  c_ptr_chpl_task_infoChapel_t_chpl call_tmp_chpl6 = NULL;
#line 156 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  chpl___EndCount_AtomicT_int64_t_int64_t call_tmp_chpl7 = NULL;
#line 156 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  call_tmp_chpl6 = chpl_task_getInfoChapel();
#line 156 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  call_tmp_chpl7 = chpl_task_data_getDynamicEndCount(call_tmp_chpl6, _ln_chpl, _fn_chpl);
#line 155 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  return call_tmp_chpl7;
#line 155 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
}

#line 159 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
/* ChapelTaskData.chpl:159 */
#line 159 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
void chpl_task_setSerial(chpl_bool makeSerial) {
#line 160 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  c_ptr_chpl_task_infoChapel_t_chpl call_tmp_chpl6 = NULL;
#line 160 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  call_tmp_chpl6 = chpl_task_getInfoChapel();
#line 160 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  chpl_task_data_setSerial(call_tmp_chpl6, makeSerial, INT64(160), INT32(7));
#line 159 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  return;
#line 159 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
}

#line 162 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
/* ChapelTaskData.chpl:162 */
#line 162 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
chpl_bool chpl_task_getSerial(void) {
#line 163 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  c_ptr_chpl_task_infoChapel_t_chpl call_tmp_chpl6 = NULL;
#line 163 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  chpl_bool call_tmp_chpl7;
#line 163 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  call_tmp_chpl6 = chpl_task_getInfoChapel();
#line 163 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  call_tmp_chpl7 = chpl_task_data_getSerial(call_tmp_chpl6, INT64(163), INT32(7));
#line 162 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  return call_tmp_chpl7;
#line 162 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
}

#line 166 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
/* ChapelTaskData.chpl:166 */
#line 166 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
chpl_bool chpl_task_setCommDiagsTemporarilyDisabled(chpl_bool disabled) {
#line 167 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  c_ptr_chpl_task_infoChapel_t_chpl call_tmp_chpl6 = NULL;
#line 167 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  chpl_bool call_tmp_chpl7;
#line 167 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  call_tmp_chpl6 = chpl_task_getInfoChapel();
#line 167 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  call_tmp_chpl7 = chpl_task_data_setCommDiagsTemporarilyDisabled(call_tmp_chpl6, disabled, INT64(167), INT32(7));
#line 166 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  return call_tmp_chpl7;
#line 166 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
}

#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
/* ChapelTaskData.chpl:170 */
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
chpl_bool chpl_task_getCommDiagsTemporarilyDisabled(void) {
#line 171 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  c_ptr_chpl_task_infoChapel_t_chpl call_tmp_chpl6 = NULL;
#line 171 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  chpl_bool call_tmp_chpl7;
#line 171 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  call_tmp_chpl6 = chpl_task_getInfoChapel();
#line 171 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  call_tmp_chpl7 = chpl_task_data_getCommDiagsTemporarilyDisabled(call_tmp_chpl6, INT64(171), INT32(7));
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
  return call_tmp_chpl7;
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskData.chpl"
}

