#line 1 "<internal>"
/* <internal>:0 */
static chpl_bool chpl___EQUALS_(locale_chpl * _arg1_chpl,
                                locale_chpl * _arg2_chpl,
                                int64_t _ln_chpl,
                                int32_t _fn_chpl) {
#line 1 "<internal>"
  chpl_bool ret_chpl;
  if (((RootClass_chpl)((_arg1_chpl)->_instance)) != ((RootClass_chpl)((_arg2_chpl)->_instance))) {
    ret_chpl = UINT8(false);
    goto _end__EQUALS__chpl;
  }
  ret_chpl = UINT8(true);
  _end__EQUALS__chpl:;
  return ret_chpl;
}

#line 1 "<internal>"
/* <internal>:0 */
static chpl_bool chpl___EXCLAMATION__ASSIGN_(locale_chpl * _arg1_chpl,
                                             locale_chpl * _arg2_chpl,
                                             int64_t _ln_chpl,
                                             int32_t _fn_chpl) {
#line 1 "<internal>"
  chpl_bool ret_chpl;
  if (((RootClass_chpl)((_arg1_chpl)->_instance)) != ((RootClass_chpl)((_arg2_chpl)->_instance))) {
    ret_chpl = UINT8(true);
    goto _end__EXCLAMATION__ASSIGN__chpl;
  }
  ret_chpl = UINT8(false);
  _end__EXCLAMATION__ASSIGN__chpl:;
  return ret_chpl;
}

#line 27 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
/* ChapelLocale.chpl:27 */
#line 27 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
static void chpl__init_ChapelLocale(int64_t _ln_chpl,
#line 27 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
                                    int32_t _fn_chpl) {
#line 27 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _ref_int32_t_chpl refIndentLevel_chpl = NULL;
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  locale_chpl call_tmp_chpl6;
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  locale_chpl initTemp_chpl;
#line 73 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  locale_chpl call_tmp_chpl7;
#line 73 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  locale_chpl initTemp_chpl2;
#line 79 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  locale_chpl call_tmp_chpl8;
#line 79 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  locale_chpl initTemp_chpl3;
#line 27 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  if (chpl__init_ChapelLocale_p) /* ZLINE: 27 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl */
#line 27 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  {
#line 27 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    goto _exit_chpl__init_ChapelLocale_chpl;
#line 27 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  }
#line 27 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  printModuleInit_chpl("%*s\n", "ChapelLocale", INT64(12), _ln_chpl, _fn_chpl);
#line 27 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  refIndentLevel_chpl = &moduleInitLevel_chpl;
#line 27 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  *(refIndentLevel_chpl) += INT64(1);
#line 27 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  chpl__init_ChapelLocale_p = UINT8(true);
#line 27 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  {
#line 27 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    chpl__init_LocaleModel(_ln_chpl, _fn_chpl);
#line 27 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    chpl__init_HaltWrappers(_ln_chpl, _fn_chpl);
#line 27 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    chpl__init_CTypes(_ln_chpl, _fn_chpl);
#line 27 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  }
#line 27 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  chpl_addModule("ChapelLocale", ((c_fn_ptr)(deinit_chpl63)), _ln_chpl, _fn_chpl);
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  compilerAssert_chpl2();
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  init_chpl103(&initTemp_chpl, _ln_chpl, _fn_chpl);
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  call_tmp_chpl6 = initTemp_chpl;
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  nilLocale_chpl = call_tmp_chpl6;
#line 73 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  init_chpl102(&initTemp_chpl2, _ln_chpl, _fn_chpl);
#line 73 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  call_tmp_chpl7 = initTemp_chpl2;
#line 73 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  defaultLocale_chpl = call_tmp_chpl7;
#line 79 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  init_chpl104(&initTemp_chpl3, _ln_chpl, _fn_chpl);
#line 79 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  call_tmp_chpl8 = initTemp_chpl3;
#line 79 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  dummyLocale_chpl = call_tmp_chpl8;
#line 536 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  init_ASSIGN__chpl(&rootLocale_chpl, &nilLocale_chpl);
#line 551 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  init_ASSIGN__chpl(&origRootLocale_chpl, &nilLocale_chpl);
#line 27 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  *(refIndentLevel_chpl) -= INT64(1);
#line 27 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _exit_chpl__init_ChapelLocale_chpl:;
#line 27 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  return;
#line 27 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
}

#line 95 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
/* ChapelLocale.chpl:95 */
#line 95 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
static void init_chpl99(locale_chpl * this_chpl31,
#line 95 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
                        int64_t _ln_chpl,
#line 95 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
                        int32_t _fn_chpl) {
#line 101 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  BaseLocale_chpl init_coerce_tmp_chpl = NULL;
#line 96 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  if (rootLocaleInitialized_chpl) /* ZLINE: 96 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl */
#line 96 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  {
#line 97 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    (this_chpl31)->_instance = (&defaultLocale_chpl)->_instance;
#line 97 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  } else /* ZLINE: 99 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl */
#line 97 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  {
#line 101 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    init_coerce_tmp_chpl = ((BaseLocale_chpl)(nil));
#line 101 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    (this_chpl31)->_instance = init_coerce_tmp_chpl;
#line 101 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  }
#line 95 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  return;
#line 95 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
}

#line 106 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
/* ChapelLocale.chpl:106 */
#line 106 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
static void init_chpl100(locale_chpl * this_chpl31,
#line 106 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
                         BaseLocale_chpl _instance_chpl) {
#line 107 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  (this_chpl31)->_instance = _instance_chpl;
#line 106 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  return;
#line 106 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
}

#line 106 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
/* ChapelLocale.chpl:106 */
#line 106 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
static void init_chpl101(locale_chpl * this_chpl31,
#line 106 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
                         BaseLocale_chpl _instance_chpl) {
#line 107 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  (this_chpl31)->_instance = _instance_chpl;
#line 106 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  return;
#line 106 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
}

#line 110 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
/* ChapelLocale.chpl:110 */
#line 110 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
static void init_chpl103(locale_chpl * this_chpl31,
#line 110 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
                         int64_t _ln_chpl,
#line 110 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
                         int32_t _fn_chpl) {
#line 110 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  (this_chpl31)->_instance = ((BaseLocale_chpl)(nil));
#line 110 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  return;
#line 110 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
}

#line 110 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
/* ChapelLocale.chpl:110 */
#line 110 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
static void init_chpl102(locale_chpl * this_chpl31,
#line 110 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
                         int64_t _ln_chpl,
#line 110 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
                         int32_t _fn_chpl) {
#line 117 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  BaseLocale_chpl init_coerce_tmp_chpl = NULL;
#line 117 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  init_coerce_tmp_chpl = ((BaseLocale_chpl)(nil));
#line 117 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  (this_chpl31)->_instance = init_coerce_tmp_chpl;
#line 110 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  return;
#line 110 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
}

#line 110 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
/* ChapelLocale.chpl:110 */
#line 110 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
static void init_chpl104(locale_chpl * this_chpl31,
#line 110 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
                         int64_t _ln_chpl,
#line 110 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
                         int32_t _fn_chpl) {
#line 115 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  DummyLocale_chpl new_temp_chpl = NULL;
#line 115 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  BaseLocale_chpl init_coerce_tmp_chpl = NULL;
#line 115 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  new_temp_chpl = _new_chpl19(_ln_chpl, _fn_chpl);
#line 115 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  init_coerce_tmp_chpl = ((BaseLocale_chpl)(new_temp_chpl));
#line 115 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  (this_chpl31)->_instance = init_coerce_tmp_chpl;
#line 110 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  return;
#line 110 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
}

#line 120 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
/* ChapelLocale.chpl:120 */
#line 120 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
static void init_ASSIGN__chpl(locale_chpl * this_chpl31,
#line 120 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
                              locale_chpl * other_chpl) {
#line 121 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  (this_chpl31)->_instance = (other_chpl)->_instance;
#line 120 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  return;
#line 120 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
}

#line 124 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
/* ChapelLocale.chpl:124 */
#line 124 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
static void deinit_chpl62(locale_chpl * this_chpl31) {
#line 124 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  return;
#line 124 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
}

#line 277 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
/* ChapelLocale.chpl:277 */
#line 277 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
static int64_t runningTasks_chpl(locale_chpl * this_chpl31,
#line 277 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
                                 int64_t _ln_chpl,
#line 277 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
                                 int32_t _fn_chpl) {
#line 92 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  BaseLocale_chpl coerce_tmp_chpl3 = NULL;
#line 278 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  BaseLocale_chpl coerce_tmp_chpl4 = NULL;
#line 278 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _ref_AtomicT_int64_t_chpl call_tmp_chpl6 = NULL;
#line 278 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _ref_atomic_int_least64_t_chpl call_tmp_chpl7 = NULL;
#line 278 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  memory_order ret_chpl;
#line 278 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  int64_t call_tmp_chpl8;
#line 278 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  int64_t tmp_chpl;
#line 92 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  coerce_tmp_chpl3 = (this_chpl31)->_instance;
#line 92 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  if (((RootClass_chpl)(coerce_tmp_chpl3)) == nil) /* ZLINE: 92 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl */
#line 92 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  {
#line 92 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    nilCheckHalt_chpl(&_str_literal_52_chpl, _ln_chpl, _fn_chpl);
#line 92 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  }
#line 278 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  coerce_tmp_chpl4 = coerce_tmp_chpl3;
#line 278 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  chpl_check_nil(coerce_tmp_chpl4, _ln_chpl, _fn_chpl);
#line 278 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  call_tmp_chpl6 = &((coerce_tmp_chpl4)->runningTaskCounter);
#line 278 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  chpl_rmem_consist_maybe_release3(_ln_chpl, _fn_chpl);
#line 278 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  call_tmp_chpl7 = &((call_tmp_chpl6)->_v);
#line 278 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  ret_chpl = memory_order_relaxed;
#line 278 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  call_tmp_chpl8 = atomic_load_explicit_int_least64_t(call_tmp_chpl7, ret_chpl);
#line 278 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  if (call_tmp_chpl8 <= INT64(0)) /* ZLINE: 278 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl */
#line 278 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  {
#line 278 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    tmp_chpl = INT64(1);
#line 278 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  } else /* ZLINE: 278 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl */
#line 278 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  {
#line 278 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    tmp_chpl = call_tmp_chpl8;
#line 278 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  }
#line 277 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  return tmp_chpl;
#line 277 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
}

#line 282 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
/* ChapelLocale.chpl:282 */
#line 282 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
static void chpl___ASSIGN_9(locale_chpl * l1_chpl,
#line 282 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
                            locale_chpl * l2_chpl) {
#line 283 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  (l1_chpl)->_instance = (l2_chpl)->_instance;
#line 282 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  return;
#line 282 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
}

#line 293 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
/* ChapelLocale.chpl:293 */
#line 293 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
static void chpl__auto_destroy_BaseLocale(BaseLocale_chpl this_chpl31,
#line 293 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
                                          int64_t _ln_chpl,
#line 293 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
                                          int32_t _fn_chpl) {
#line 369 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _ref_AtomicT_int64_t_chpl _field_destructor_tmp__chpl = NULL;
#line 310 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _ref_locale_chpl _field_destructor_tmp__chpl2 = NULL;
#line 369 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 369 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _field_destructor_tmp__chpl = &((this_chpl31)->runningTaskCounter);
#line 369 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  deinit_chpl2(_field_destructor_tmp__chpl);
#line 310 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 310 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _field_destructor_tmp__chpl2 = &((this_chpl31)->parent);
#line 310 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  deinit_chpl62(_field_destructor_tmp__chpl2);
#line 293 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  return;
#line 293 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
}

#line 325 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
/* ChapelLocale.chpl:325 */
#line 325 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
static int64_t id_chpl(BaseLocale_chpl this_chpl31,
#line 325 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
                       int64_t _ln_chpl,
#line 325 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
                       int32_t _fn_chpl) {
#line 325 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  int64_t ret_chpl;
#line 325 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  chpl_localeID_t call_tmp_chpl6;
#line 325 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  int32_t call_tmp_chpl7;
#line 325 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  call_tmp_chpl6 = chpl_gen_getLocaleID();
#line 325 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  call_tmp_chpl7 = chpl_nodeFromLocaleID(call_tmp_chpl6, _ln_chpl, _fn_chpl);
#line 325 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  ret_chpl = ((int64_t)(call_tmp_chpl7));
#line 325 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  return ret_chpl;
#line 325 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
}

#line 415 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
/* ChapelLocale.chpl:415 */
#line 415 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
static int64_t _getChildCount_chpl(BaseLocale_chpl this_chpl31,
#line 415 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
                                   int64_t _ln_chpl,
#line 415 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
                                   int32_t _fn_chpl) {
#line 416 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  pureVirtualMethodHalt_chpl(_ln_chpl, _fn_chpl);
#line 415 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  return INT64(0);
#line 415 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
}

#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
/* ChapelLocale.chpl:434 */
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
static void _getChild_chpl(BaseLocale_chpl this_chpl31,
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
                           int64_t idx_chpl,
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
                           locale_chpl * _retArg_chpl,
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
                           int64_t _ln_chpl,
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
                           int32_t _fn_chpl) {
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  locale_chpl ret_chpl;
#line 435 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  pureVirtualMethodHalt_chpl(_ln_chpl, _fn_chpl);
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  *(_retArg_chpl) = ret_chpl;
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  return;
#line 434 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
}

#line 468 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
/* ChapelLocale.chpl:468 */
#line 468 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
static void chpl__auto_destroy_DummyLocale(DummyLocale_chpl this_chpl31,
#line 468 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
                                           int64_t _ln_chpl,
#line 468 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
                                           int32_t _fn_chpl) {
#line 468 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  BaseLocale_chpl _parent_destructor_tmp__chpl = NULL;
#line 468 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _ref_AtomicT_int64_t_chpl _field_destructor_tmp__chpl = NULL;
#line 468 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _ref_locale_chpl _field_destructor_tmp__chpl2 = NULL;
#line 468 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _parent_destructor_tmp__chpl = ((BaseLocale_chpl)(this_chpl31));
#line 468 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  chpl_check_nil(_parent_destructor_tmp__chpl, _ln_chpl, _fn_chpl);
#line 468 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _field_destructor_tmp__chpl = &((_parent_destructor_tmp__chpl)->runningTaskCounter);
#line 468 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  deinit_chpl2(_field_destructor_tmp__chpl);
#line 468 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  chpl_check_nil(_parent_destructor_tmp__chpl, _ln_chpl, _fn_chpl);
#line 468 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _field_destructor_tmp__chpl2 = &((_parent_destructor_tmp__chpl)->parent);
#line 468 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  deinit_chpl62(_field_destructor_tmp__chpl2);
#line 468 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  return;
#line 468 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
}

#line 469 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
/* ChapelLocale.chpl:469 */
#line 469 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
static DummyLocale_chpl _new_chpl19(int64_t _ln_chpl,
#line 469 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
                                    int32_t _fn_chpl) {
#line 469 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  DummyLocale_chpl initTemp_chpl = NULL;
#line 469 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  c_ptr_void_chpl cast_tmp_chpl = NULL;
#line 469 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  BaseLocale_chpl super_tmp_chpl = NULL;
#line 469 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _ref_locale_chpl i_parent_chpl = NULL;
#line 469 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  locale_chpl parent_chpl;
#line 469 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  AtomicT_int64_t_chpl runningTaskCounter_chpl;
#line 469 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  cast_tmp_chpl = chpl_here_alloc(sizeof(chpl_DummyLocale_chpl_object), INT16(7), _ln_chpl, _fn_chpl);
#line 469 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  initTemp_chpl = ((DummyLocale_chpl)(cast_tmp_chpl));
#line 469 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  ((RootClass_chpl)(initTemp_chpl))->chpl__cid = chpl__cid_DummyLocale_chpl;
#line 469 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  chpl_check_nil(initTemp_chpl, _ln_chpl, _fn_chpl);
#line 469 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  super_tmp_chpl = &((initTemp_chpl)->super);
#line 469 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  i_parent_chpl = &nilLocale_chpl;
#line 469 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 469 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  ((RootClass_chpl)(&((super_tmp_chpl)->super)))->chpl__cid = chpl__cid_RootClass_chpl;
#line 469 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  ((RootClass_chpl)(((RootClass_chpl)(super_tmp_chpl))))->chpl__cid = chpl__cid_RootClass_chpl;
#line 469 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  init_ASSIGN__chpl(&parent_chpl, i_parent_chpl);
#line 469 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 469 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  (super_tmp_chpl)->parent = parent_chpl;
#line 469 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 469 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  (super_tmp_chpl)->nPUsLogAcc = INT64(0);
#line 469 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 469 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  (super_tmp_chpl)->nPUsLogAll = INT64(0);
#line 469 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 469 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  (super_tmp_chpl)->nPUsPhysAcc = INT64(0);
#line 469 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 469 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  (super_tmp_chpl)->nPUsPhysAll = INT64(0);
#line 469 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 469 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  (super_tmp_chpl)->maxTaskPar = INT64(0);
#line 469 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  init_chpl18(&runningTaskCounter_chpl);
#line 469 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 469 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  (super_tmp_chpl)->runningTaskCounter = runningTaskCounter_chpl;
#line 469 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  ((RootClass_chpl)(super_tmp_chpl))->chpl__cid = chpl__cid_BaseLocale_chpl;
#line 469 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  ((RootClass_chpl)(((BaseLocale_chpl)(initTemp_chpl))))->chpl__cid = chpl__cid_BaseLocale_chpl;
#line 469 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  ((RootClass_chpl)(initTemp_chpl))->chpl__cid = chpl__cid_DummyLocale_chpl;
#line 469 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  return initTemp_chpl;
#line 469 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
}

#line 482 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
/* ChapelLocale.chpl:482 */
#line 482 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
static int64_t _getChildCount_chpl2(DummyLocale_chpl this_chpl31,
#line 482 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
                                    int64_t _ln_chpl,
#line 482 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
                                    int32_t _fn_chpl) {
#line 482 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  return INT64(0);
#line 482 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
}

#line 485 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
/* ChapelLocale.chpl:485 */
#line 485 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
static void _getChild_chpl2(DummyLocale_chpl this_chpl31,
#line 485 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
                            int64_t idx_chpl,
#line 485 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
                            locale_chpl * _retArg_chpl,
#line 485 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
                            int64_t _ln_chpl,
#line 485 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
                            int32_t _fn_chpl) {
#line 486 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  locale_chpl call_tmp_chpl6;
#line 486 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  locale_chpl initTemp_chpl;
#line 486 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  init_chpl100(&initTemp_chpl, ((BaseLocale_chpl)(this_chpl31)));
#line 486 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  call_tmp_chpl6 = initTemp_chpl;
#line 485 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  *(_retArg_chpl) = call_tmp_chpl6;
#line 485 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  return;
#line 485 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
}

#line 513 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
/* ChapelLocale.chpl:513 */
#line 513 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
static void chpl__auto_destroy_AbstractLocaleModel(AbstractLocaleModel_chpl this_chpl31,
#line 513 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
                                                   int64_t _ln_chpl,
#line 513 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
                                                   int32_t _fn_chpl) {
#line 515 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _array_DefaultRectangularArr_1_int64_t_one_locale_int64_t_chpl _field_destructor_tmp__chpl;
#line 513 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  BaseLocale_chpl _parent_destructor_tmp__chpl = NULL;
#line 513 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _ref_AtomicT_int64_t_chpl _field_destructor_tmp__chpl2 = NULL;
#line 513 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _ref_locale_chpl _field_destructor_tmp__chpl3 = NULL;
#line 515 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 515 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _field_destructor_tmp__chpl = (this_chpl31)->chpl_singletonThisLocaleArray;
#line 515 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  chpl__autoDestroy30(&_field_destructor_tmp__chpl, _ln_chpl, _fn_chpl);
#line 513 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _parent_destructor_tmp__chpl = ((BaseLocale_chpl)(this_chpl31));
#line 513 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  chpl_check_nil(_parent_destructor_tmp__chpl, _ln_chpl, _fn_chpl);
#line 513 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _field_destructor_tmp__chpl2 = &((_parent_destructor_tmp__chpl)->runningTaskCounter);
#line 513 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  deinit_chpl2(_field_destructor_tmp__chpl2);
#line 513 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  chpl_check_nil(_parent_destructor_tmp__chpl, _ln_chpl, _fn_chpl);
#line 513 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _field_destructor_tmp__chpl3 = &((_parent_destructor_tmp__chpl)->parent);
#line 513 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  deinit_chpl62(_field_destructor_tmp__chpl3);
#line 513 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  return;
#line 513 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
}

#line 554 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
/* ChapelLocale.chpl:554 */
#line 554 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
static void chpl__auto_destroy_AbstractRootLocale(AbstractRootLocale_chpl this_chpl31,
#line 554 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
                                                  int64_t _ln_chpl,
#line 554 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
                                                  int32_t _fn_chpl) {
#line 554 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  BaseLocale_chpl _parent_destructor_tmp__chpl = NULL;
#line 554 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _ref_AtomicT_int64_t_chpl _field_destructor_tmp__chpl = NULL;
#line 554 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _ref_locale_chpl _field_destructor_tmp__chpl2 = NULL;
#line 554 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _parent_destructor_tmp__chpl = ((BaseLocale_chpl)(this_chpl31));
#line 554 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  chpl_check_nil(_parent_destructor_tmp__chpl, _ln_chpl, _fn_chpl);
#line 554 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _field_destructor_tmp__chpl = &((_parent_destructor_tmp__chpl)->runningTaskCounter);
#line 554 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  deinit_chpl2(_field_destructor_tmp__chpl);
#line 554 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  chpl_check_nil(_parent_destructor_tmp__chpl, _ln_chpl, _fn_chpl);
#line 554 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _field_destructor_tmp__chpl2 = &((_parent_destructor_tmp__chpl)->parent);
#line 554 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  deinit_chpl62(_field_destructor_tmp__chpl2);
#line 554 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  return;
#line 554 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
}

#line 575 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
/* ChapelLocale.chpl:575 */
#line 575 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
static void localeIDtoLocale_chpl(AbstractRootLocale_chpl this_chpl31,
#line 575 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
                                  chpl_localeID_t * id_chpl2,
#line 575 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
                                  locale_chpl * _retArg_chpl,
#line 575 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
                                  int64_t _ln_chpl,
#line 575 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
                                  int32_t _fn_chpl) {
#line 575 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  locale_chpl ret_chpl;
#line 576 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  pureVirtualMethodHalt_chpl(_ln_chpl, _fn_chpl);
#line 575 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  *(_retArg_chpl) = ret_chpl;
#line 575 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  return;
#line 575 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
}

#line 627 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
/* ChapelLocale.chpl:627 */
#line 627 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
static void warmupRuntime_chpl(int64_t _ln_chpl,
#line 627 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
                               int32_t _fn_chpl) {
#line 629 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  range_int64_t_both_one_chpl tmpIter_chpl;
#line 629 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  range_int64_t_low_one_chpl call_tmp_chpl6;
#line 629 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  range_int64_t_low_one_chpl ret_tmp_chpl;
#line 629 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  uint32_t call_tmp_chpl7;
#line 629 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl2;
#line 629 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  chpl___EndCount_AtomicT_int64_t_int64_t _coforallCount_chpl = NULL;
#line 629 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  chpl___EndCount_AtomicT_int64_t_int64_t new_temp_chpl = NULL;
#line 629 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  int64_t numTasks_chpl;
#line 629 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  range_int64_t_both_one_chpl _ic__F0_this_chpl;
#line 629 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  int64_t i_chpl;
#line 629 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  range_int64_t_both_one_chpl this_chpl31;
#line 629 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  range_int64_t_both_one_chpl this_chpl32;
#line 629 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  int64_t end_chpl;
#line 629 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _class_localscoforall_fn_chpl63 _args_forcoforall_fn_chpl = NULL;
#line 629 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  raw_c_void_ptr _args_vforcoforall_fn_chpl;
#line 629 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  int64_t _args_sizecoforall_fn_chpl;
#line 629 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  Error_chpl error_chpl = NULL;
#line 629 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  chpl__class_localscoforall_fn_chpl63_object chpl_macro_tmp_14;
#line 629 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  chpl_build_low_bounded_range2(INT64(0), &ret_tmp_chpl);
#line 629 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  call_tmp_chpl6 = ret_tmp_chpl;
#line 629 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  call_tmp_chpl7 = chpl_task_getFixedNumThreads();
#line 629 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  chpl___POUND_2(call_tmp_chpl6, call_tmp_chpl7, &ret_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 629 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  tmpIter_chpl = ret_tmp_chpl2;
#line 629 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  new_temp_chpl = _new_chpl17(_ln_chpl, _fn_chpl);
#line 629 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _coforallCount_chpl = new_temp_chpl;
#line 629 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  numTasks_chpl = chpl_boundedCoforallSize(tmpIter_chpl, _ln_chpl, _fn_chpl);
#line 629 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _upEndCount_chpl(_coforallCount_chpl, numTasks_chpl, _ln_chpl, _fn_chpl);
#line 629 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  chpl_resetTaskSpawn(numTasks_chpl, _ln_chpl, _fn_chpl);
#line 629 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _ic__F0_this_chpl = tmpIter_chpl;
#line 629 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  checkIfIterWillOverflow_chpl(tmpIter_chpl, UINT8(true), _ln_chpl, _fn_chpl);
#line 629 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  i_chpl = INT64(0);
#line 629 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  this_chpl31 = _ic__F0_this_chpl;
#line 629 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  this_chpl32 = _ic__F0_this_chpl;
#line 629 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  end_chpl = (&this_chpl32)->_high;
#line 629 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  for (i_chpl = (&this_chpl31)->_low; ((i_chpl <= end_chpl)); i_chpl += INT64(1)) {
#line 629 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    _args_forcoforall_fn_chpl = ((_class_localscoforall_fn_chpl63)(&chpl_macro_tmp_14));
#line 629 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl, _ln_chpl, _fn_chpl);
#line 629 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    (_args_forcoforall_fn_chpl)->_0_i = i_chpl;
#line 629 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl, _ln_chpl, _fn_chpl);
#line 629 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    (_args_forcoforall_fn_chpl)->_1__coforallCount = _coforallCount_chpl;
#line 629 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    _args_vforcoforall_fn_chpl = ((void*)(_args_forcoforall_fn_chpl));
#line 629 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    _args_sizecoforall_fn_chpl = sizeof(chpl__class_localscoforall_fn_chpl63_object);
#line 629 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    (_args_forcoforall_fn_chpl)->_ln = _ln_chpl;
#line 629 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    (_args_forcoforall_fn_chpl)->_fn = _fn_chpl;
#line 629 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    /*** wrapcoforall_fn_chpl63 ***/ chpl_taskAddCoStmt(chpl_task_getRequestedSubloc(), INT64(62), ((chpl_task_bundle_p)(_args_vforcoforall_fn_chpl)), _args_sizecoforall_fn_chpl, INT64(629), INT32(26));
#line 629 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  }
#line 629 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  error_chpl = NULL;
#line 629 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _waitEndCount_chpl2(_coforallCount_chpl, numTasks_chpl, &error_chpl, _ln_chpl, _fn_chpl);
#line 629 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  if (error_chpl != nil) /* ZLINE: 629 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl */
#line 629 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  {
#line 629 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    chpl_uncaught_error(error_chpl, _ln_chpl, _fn_chpl);
#line 629 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  }
#line 629 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  chpl__delete72(_coforallCount_chpl, _ln_chpl, _fn_chpl);
#line 627 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  return;
#line 627 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
}

#line 629 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
/* ChapelLocale.chpl:629 */
#line 629 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
static void coforall_fn_chpl63(int64_t i_chpl,
#line 629 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
                               chpl___EndCount_AtomicT_int64_t_int64_t _coforallCount_chpl,
#line 629 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
                               int64_t _ln_chpl,
#line 629 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
                               int32_t _fn_chpl) {
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  uint64_t coerce_tmp_chpl3;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  uint64_t call_tmp_chpl6;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  uint64_t call_tmp_chpl7;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  chpl_bool tmp_chpl;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  c_ptr_void_chpl ptr_chpl = NULL;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  c_ptr_void_chpl init_coerce_tmp_chpl = NULL;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  int16_t call_tmp_chpl8;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  c_ptr_void_chpl call_tmp_chpl9 = NULL;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  uint64_t call_tmp_chpl10;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  int64_t call_tmp_chpl11;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  int64_t call_tmp_chpl12;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  int64_t p_chpl;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  string_chpl call_tmp_chpl13;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  string_chpl ret_tmp_chpl;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  string_chpl call_tmp_chpl14;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  string_chpl ret_tmp_chpl2;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  string_chpl call_tmp_chpl15;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  string_chpl ret_tmp_chpl3;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  string_chpl call_tmp_chpl16;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  string_chpl ret_tmp_chpl4;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  string_chpl call_tmp_chpl17;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  string_chpl ret_tmp_chpl5;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  string_chpl call_tmp_chpl18;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  string_chpl ret_tmp_chpl6;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  string_chpl call_tmp_chpl19;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  string_chpl ret_tmp_chpl7;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  string_chpl call_tmp_chpl20;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  string_chpl ret_tmp_chpl8;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  string_chpl call_tmp_chpl21;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  string_chpl ret_tmp_chpl9;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  string_chpl msg_chpl;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _ref_string_chpl i_x_chpl2 = NULL;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _ref_string_chpl i_x_chpl3 = NULL;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _ref_string_chpl i_x_chpl4 = NULL;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _ref_string_chpl i_x_chpl5 = NULL;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _ref_string_chpl i_x_chpl6 = NULL;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _ref_string_chpl i_x_chpl7 = NULL;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _ref_string_chpl i_x_chpl8 = NULL;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _ref_string_chpl i_x_chpl9 = NULL;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  string_chpl call_tmp_chpl22;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  string_chpl ret_tmp_chpl10;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  string_chpl call_tmp_chpl23;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  string_chpl ret_tmp_chpl11;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  string_chpl call_tmp_chpl24;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  string_chpl ret_tmp_chpl12;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  string_chpl call_tmp_chpl25;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  string_chpl ret_tmp_chpl13;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  string_chpl call_tmp_chpl26;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  string_chpl ret_tmp_chpl14;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  string_chpl call_tmp_chpl27;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  string_chpl ret_tmp_chpl15;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  string_chpl call_tmp_chpl28;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  string_chpl ret_tmp_chpl16;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  string_chpl call_tmp_chpl29;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  string_chpl ret_tmp_chpl17;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  string_chpl call_tmp_chpl30;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  string_chpl ret_tmp_chpl18;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  string_chpl call_tmp_chpl31;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  string_chpl ret_tmp_chpl19;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  string_chpl msg_chpl2;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _ref_string_chpl i_x_chpl10 = NULL;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _ref_string_chpl i_x_chpl11 = NULL;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _ref_string_chpl i_x_chpl12 = NULL;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _ref_string_chpl i_x_chpl13 = NULL;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _ref_string_chpl i_x_chpl14 = NULL;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _ref_string_chpl i_x_chpl15 = NULL;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _ref_string_chpl i_x_chpl16 = NULL;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _ref_string_chpl i_x_chpl17 = NULL;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _ref_string_chpl i_x_chpl18 = NULL;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _ref_string_chpl i_x_chpl19 = NULL;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _ref_int64_t_chpl i_lhs_chpl = NULL;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  string_chpl call_tmp_chpl32;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  string_chpl ret_tmp_chpl20;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  string_chpl call_tmp_chpl33;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  string_chpl ret_tmp_chpl21;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  string_chpl call_tmp_chpl34;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  string_chpl ret_tmp_chpl22;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  string_chpl call_tmp_chpl35;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  string_chpl ret_tmp_chpl23;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  string_chpl call_tmp_chpl36;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  string_chpl ret_tmp_chpl24;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  string_chpl call_tmp_chpl37;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  string_chpl ret_tmp_chpl25;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  string_chpl call_tmp_chpl38;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  string_chpl ret_tmp_chpl26;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  string_chpl call_tmp_chpl39;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  string_chpl ret_tmp_chpl27;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  string_chpl call_tmp_chpl40;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  string_chpl ret_tmp_chpl28;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  string_chpl msg_chpl3;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _ref_string_chpl i_x_chpl20 = NULL;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _ref_string_chpl i_x_chpl21 = NULL;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _ref_string_chpl i_x_chpl22 = NULL;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _ref_string_chpl i_x_chpl23 = NULL;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _ref_string_chpl i_x_chpl24 = NULL;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _ref_string_chpl i_x_chpl25 = NULL;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _ref_string_chpl i_x_chpl26 = NULL;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _ref_string_chpl i_x_chpl27 = NULL;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _ref_string_chpl i_x_chpl28 = NULL;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  string_chpl call_tmp_chpl41;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  string_chpl ret_tmp_chpl29;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  string_chpl call_tmp_chpl42;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  string_chpl ret_tmp_chpl30;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  string_chpl call_tmp_chpl43;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  string_chpl ret_tmp_chpl31;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  string_chpl call_tmp_chpl44;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  string_chpl ret_tmp_chpl32;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  string_chpl call_tmp_chpl45;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  string_chpl ret_tmp_chpl33;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  string_chpl call_tmp_chpl46;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  string_chpl ret_tmp_chpl34;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  string_chpl call_tmp_chpl47;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  string_chpl ret_tmp_chpl35;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  string_chpl call_tmp_chpl48;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  string_chpl ret_tmp_chpl36;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  string_chpl call_tmp_chpl49;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  string_chpl ret_tmp_chpl37;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  string_chpl call_tmp_chpl50;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  string_chpl ret_tmp_chpl38;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  string_chpl msg_chpl4;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _ref_string_chpl i_x_chpl29 = NULL;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _ref_string_chpl i_x_chpl30 = NULL;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _ref_string_chpl i_x_chpl31 = NULL;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _ref_string_chpl i_x_chpl32 = NULL;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _ref_string_chpl i_x_chpl33 = NULL;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _ref_string_chpl i_x_chpl34 = NULL;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _ref_string_chpl i_x_chpl35 = NULL;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _ref_string_chpl i_x_chpl36 = NULL;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _ref_string_chpl i_x_chpl37 = NULL;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _ref_string_chpl i_x_chpl38 = NULL;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  string_chpl call_tmp_chpl51;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  string_chpl ret_tmp_chpl39;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  string_chpl call_tmp_chpl52;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  string_chpl ret_tmp_chpl40;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  string_chpl call_tmp_chpl53;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  string_chpl ret_tmp_chpl41;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  string_chpl call_tmp_chpl54;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  string_chpl ret_tmp_chpl42;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  string_chpl call_tmp_chpl55;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  string_chpl ret_tmp_chpl43;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  string_chpl call_tmp_chpl56;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  string_chpl ret_tmp_chpl44;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  string_chpl call_tmp_chpl57;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  string_chpl ret_tmp_chpl45;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  string_chpl call_tmp_chpl58;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  string_chpl ret_tmp_chpl46;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  string_chpl call_tmp_chpl59;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  string_chpl ret_tmp_chpl47;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  string_chpl msg_chpl5;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _ref_string_chpl i_x_chpl39 = NULL;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _ref_string_chpl i_x_chpl40 = NULL;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _ref_string_chpl i_x_chpl41 = NULL;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _ref_string_chpl i_x_chpl42 = NULL;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _ref_string_chpl i_x_chpl43 = NULL;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _ref_string_chpl i_x_chpl44 = NULL;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _ref_string_chpl i_x_chpl45 = NULL;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _ref_string_chpl i_x_chpl46 = NULL;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _ref_string_chpl i_x_chpl47 = NULL;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  string_chpl call_tmp_chpl60;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  string_chpl ret_tmp_chpl48;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  string_chpl call_tmp_chpl61;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  string_chpl ret_tmp_chpl49;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  string_chpl call_tmp_chpl62;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  string_chpl ret_tmp_chpl50;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  string_chpl call_tmp_chpl63;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  string_chpl ret_tmp_chpl51;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  string_chpl call_tmp_chpl64;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  string_chpl ret_tmp_chpl52;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  string_chpl call_tmp_chpl65;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  string_chpl ret_tmp_chpl53;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  string_chpl call_tmp_chpl66;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  string_chpl ret_tmp_chpl54;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  string_chpl call_tmp_chpl67;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  string_chpl ret_tmp_chpl55;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  string_chpl call_tmp_chpl68;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  string_chpl ret_tmp_chpl56;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  string_chpl call_tmp_chpl69;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  string_chpl ret_tmp_chpl57;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  string_chpl msg_chpl6;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _ref_string_chpl i_x_chpl48 = NULL;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _ref_string_chpl i_x_chpl49 = NULL;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _ref_string_chpl i_x_chpl50 = NULL;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _ref_string_chpl i_x_chpl51 = NULL;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _ref_string_chpl i_x_chpl52 = NULL;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _ref_string_chpl i_x_chpl53 = NULL;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _ref_string_chpl i_x_chpl54 = NULL;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _ref_string_chpl i_x_chpl55 = NULL;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _ref_string_chpl i_x_chpl56 = NULL;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _ref_string_chpl i_x_chpl57 = NULL;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  uint64_t call_tmp_chpl70;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  int16_t call_tmp_chpl71;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  c_ptr_void_chpl call_tmp_chpl72 = NULL;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  c_ptr_int64_t_chpl call_tmp_chpl73 = NULL;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  c_ptr_int64_t_chpl p_chpl2 = NULL;
#line 631 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  int64_t inlineImm_chpl;
#line 631 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _ref_int64_t_chpl call_tmp_chpl74 = NULL;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  coerce_tmp_chpl3 = ((uint64_t)(INT64(1)));
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  call_tmp_chpl6 = sizeof(int64_t);
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  call_tmp_chpl7 = (coerce_tmp_chpl3 * call_tmp_chpl6);
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  if (INT64(0) < INT64(0)) /* ZLINE: 630 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl */
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  {
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    tmp_chpl = UINT8(true);
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  } else /* ZLINE: 630 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl */
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  {
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    tmp_chpl = (UINT64(0) != ((uint64_t)(INT64(0))));
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  }
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  init_coerce_tmp_chpl = ((c_ptr_void_chpl)(nil));
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  ptr_chpl = init_coerce_tmp_chpl;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  if (! tmp_chpl) /* ZLINE: 630 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl */
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  {
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    {
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      call_tmp_chpl8 = offset_ARRAY_ELEMENTS_chpl();
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      call_tmp_chpl9 = chpl_here_alloc2(call_tmp_chpl7, call_tmp_chpl8, _ln_chpl, _fn_chpl);
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      ptr_chpl = call_tmp_chpl9;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    }
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  } else /* ZLINE: 630 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl */
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  {
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    call_tmp_chpl10 = chpl_bitops_clz_64(UINT64(0));
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    call_tmp_chpl11 = (INT64(63) - ((int64_t)(call_tmp_chpl10)));
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    if (INT64(1) == INT64(0)) /* ZLINE: 630 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl */
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    {
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      halt_chpl(&_str_literal_30_chpl, _ln_chpl, _fn_chpl);
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    }
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    call_tmp_chpl12 = (call_tmp_chpl11 / INT64(1));
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    p_chpl = call_tmp_chpl12;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    if (call_tmp_chpl12 < INT64(0)) /* ZLINE: 630 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl */
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    {
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      chpl___COLON_24(UINT64(1), &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      call_tmp_chpl13 = ret_tmp_chpl;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      chpl___PLUS_(&_str_literal_45_chpl, &call_tmp_chpl13, &ret_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      call_tmp_chpl14 = ret_tmp_chpl2;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      chpl___PLUS_(&call_tmp_chpl14, &_str_literal_46_chpl, &ret_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      call_tmp_chpl15 = ret_tmp_chpl3;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      chpl___COLON_25(call_tmp_chpl12, &ret_tmp_chpl4, _ln_chpl, _fn_chpl);
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      call_tmp_chpl16 = ret_tmp_chpl4;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      chpl___PLUS_(&call_tmp_chpl15, &call_tmp_chpl16, &ret_tmp_chpl5, _ln_chpl, _fn_chpl);
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      call_tmp_chpl17 = ret_tmp_chpl5;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      chpl___PLUS_(&call_tmp_chpl17, &_str_literal_47_chpl, &ret_tmp_chpl6, _ln_chpl, _fn_chpl);
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      call_tmp_chpl18 = ret_tmp_chpl6;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      chpl___COLON_25(call_tmp_chpl12, &ret_tmp_chpl7, _ln_chpl, _fn_chpl);
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      call_tmp_chpl19 = ret_tmp_chpl7;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      chpl___PLUS_(&call_tmp_chpl18, &call_tmp_chpl19, &ret_tmp_chpl8, _ln_chpl, _fn_chpl);
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      call_tmp_chpl20 = ret_tmp_chpl8;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      chpl___PLUS_(&call_tmp_chpl20, &_str_literal_48_chpl, &ret_tmp_chpl9, _ln_chpl, _fn_chpl);
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      call_tmp_chpl21 = ret_tmp_chpl9;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      msg_chpl = call_tmp_chpl21;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      i_x_chpl = &call_tmp_chpl20;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      deinit_chpl95(i_x_chpl, _ln_chpl, _fn_chpl);
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      i_x_chpl2 = &call_tmp_chpl19;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      deinit_chpl95(i_x_chpl2, _ln_chpl, _fn_chpl);
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      i_x_chpl3 = &call_tmp_chpl18;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      deinit_chpl95(i_x_chpl3, _ln_chpl, _fn_chpl);
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      i_x_chpl4 = &call_tmp_chpl17;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      deinit_chpl95(i_x_chpl4, _ln_chpl, _fn_chpl);
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      i_x_chpl5 = &call_tmp_chpl16;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      deinit_chpl95(i_x_chpl5, _ln_chpl, _fn_chpl);
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      i_x_chpl6 = &call_tmp_chpl15;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      deinit_chpl95(i_x_chpl6, _ln_chpl, _fn_chpl);
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      i_x_chpl7 = &call_tmp_chpl14;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      deinit_chpl95(i_x_chpl7, _ln_chpl, _fn_chpl);
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      i_x_chpl8 = &call_tmp_chpl13;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      deinit_chpl95(i_x_chpl8, _ln_chpl, _fn_chpl);
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      boundsCheckHalt_chpl(&msg_chpl, _ln_chpl, _fn_chpl);
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      i_x_chpl9 = &msg_chpl;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      deinit_chpl95(i_x_chpl9, _ln_chpl, _fn_chpl);
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    } else /* ZLINE: 630 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl */
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    if (call_tmp_chpl12 >= INT64(64)) /* ZLINE: 630 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl */
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    {
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      chpl___COLON_24(UINT64(1), &ret_tmp_chpl10, _ln_chpl, _fn_chpl);
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      call_tmp_chpl22 = ret_tmp_chpl10;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      chpl___PLUS_(&_str_literal_45_chpl, &call_tmp_chpl22, &ret_tmp_chpl11, _ln_chpl, _fn_chpl);
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      call_tmp_chpl23 = ret_tmp_chpl11;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      chpl___PLUS_(&call_tmp_chpl23, &_str_literal_46_chpl, &ret_tmp_chpl12, _ln_chpl, _fn_chpl);
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      call_tmp_chpl24 = ret_tmp_chpl12;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      chpl___COLON_25(call_tmp_chpl12, &ret_tmp_chpl13, _ln_chpl, _fn_chpl);
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      call_tmp_chpl25 = ret_tmp_chpl13;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      chpl___PLUS_(&call_tmp_chpl24, &call_tmp_chpl25, &ret_tmp_chpl14, _ln_chpl, _fn_chpl);
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      call_tmp_chpl26 = ret_tmp_chpl14;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      chpl___PLUS_(&call_tmp_chpl26, &_str_literal_47_chpl, &ret_tmp_chpl15, _ln_chpl, _fn_chpl);
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      call_tmp_chpl27 = ret_tmp_chpl15;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      chpl___COLON_25(call_tmp_chpl12, &ret_tmp_chpl16, _ln_chpl, _fn_chpl);
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      call_tmp_chpl28 = ret_tmp_chpl16;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      chpl___PLUS_(&call_tmp_chpl27, &call_tmp_chpl28, &ret_tmp_chpl17, _ln_chpl, _fn_chpl);
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      call_tmp_chpl29 = ret_tmp_chpl17;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      chpl___PLUS_(&call_tmp_chpl29, &_str_literal_50_chpl, &ret_tmp_chpl18, _ln_chpl, _fn_chpl);
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      call_tmp_chpl30 = ret_tmp_chpl18;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      chpl___PLUS_(&call_tmp_chpl30, &_str_literal_3965_chpl, &ret_tmp_chpl19, _ln_chpl, _fn_chpl);
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      call_tmp_chpl31 = ret_tmp_chpl19;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      msg_chpl2 = call_tmp_chpl31;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      i_x_chpl10 = &call_tmp_chpl30;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      deinit_chpl95(i_x_chpl10, _ln_chpl, _fn_chpl);
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      i_x_chpl11 = &call_tmp_chpl29;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      deinit_chpl95(i_x_chpl11, _ln_chpl, _fn_chpl);
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      i_x_chpl12 = &call_tmp_chpl28;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      deinit_chpl95(i_x_chpl12, _ln_chpl, _fn_chpl);
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      i_x_chpl13 = &call_tmp_chpl27;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      deinit_chpl95(i_x_chpl13, _ln_chpl, _fn_chpl);
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      i_x_chpl14 = &call_tmp_chpl26;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      deinit_chpl95(i_x_chpl14, _ln_chpl, _fn_chpl);
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      i_x_chpl15 = &call_tmp_chpl25;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      deinit_chpl95(i_x_chpl15, _ln_chpl, _fn_chpl);
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      i_x_chpl16 = &call_tmp_chpl24;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      deinit_chpl95(i_x_chpl16, _ln_chpl, _fn_chpl);
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      i_x_chpl17 = &call_tmp_chpl23;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      deinit_chpl95(i_x_chpl17, _ln_chpl, _fn_chpl);
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      i_x_chpl18 = &call_tmp_chpl22;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      deinit_chpl95(i_x_chpl18, _ln_chpl, _fn_chpl);
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      boundsCheckHalt_chpl(&msg_chpl2, _ln_chpl, _fn_chpl);
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      i_x_chpl19 = &msg_chpl2;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      deinit_chpl95(i_x_chpl19, _ln_chpl, _fn_chpl);
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    }
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    if (((uint64_t)((UINT64(1) << call_tmp_chpl12))) < UINT64(0)) /* ZLINE: 630 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl */
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    {
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      i_lhs_chpl = &p_chpl;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      *(i_lhs_chpl) += INT64(1);
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    }
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    if (p_chpl < INT64(0)) /* ZLINE: 630 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl */
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    {
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      chpl___COLON_24(UINT64(1), &ret_tmp_chpl20, _ln_chpl, _fn_chpl);
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      call_tmp_chpl32 = ret_tmp_chpl20;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      chpl___PLUS_(&_str_literal_45_chpl, &call_tmp_chpl32, &ret_tmp_chpl21, _ln_chpl, _fn_chpl);
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      call_tmp_chpl33 = ret_tmp_chpl21;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      chpl___PLUS_(&call_tmp_chpl33, &_str_literal_46_chpl, &ret_tmp_chpl22, _ln_chpl, _fn_chpl);
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      call_tmp_chpl34 = ret_tmp_chpl22;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      chpl___COLON_25(p_chpl, &ret_tmp_chpl23, _ln_chpl, _fn_chpl);
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      call_tmp_chpl35 = ret_tmp_chpl23;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      chpl___PLUS_(&call_tmp_chpl34, &call_tmp_chpl35, &ret_tmp_chpl24, _ln_chpl, _fn_chpl);
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      call_tmp_chpl36 = ret_tmp_chpl24;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      chpl___PLUS_(&call_tmp_chpl36, &_str_literal_47_chpl, &ret_tmp_chpl25, _ln_chpl, _fn_chpl);
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      call_tmp_chpl37 = ret_tmp_chpl25;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      chpl___COLON_25(p_chpl, &ret_tmp_chpl26, _ln_chpl, _fn_chpl);
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      call_tmp_chpl38 = ret_tmp_chpl26;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      chpl___PLUS_(&call_tmp_chpl37, &call_tmp_chpl38, &ret_tmp_chpl27, _ln_chpl, _fn_chpl);
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      call_tmp_chpl39 = ret_tmp_chpl27;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      chpl___PLUS_(&call_tmp_chpl39, &_str_literal_48_chpl, &ret_tmp_chpl28, _ln_chpl, _fn_chpl);
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      call_tmp_chpl40 = ret_tmp_chpl28;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      msg_chpl3 = call_tmp_chpl40;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      i_x_chpl20 = &call_tmp_chpl39;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      deinit_chpl95(i_x_chpl20, _ln_chpl, _fn_chpl);
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      i_x_chpl21 = &call_tmp_chpl38;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      deinit_chpl95(i_x_chpl21, _ln_chpl, _fn_chpl);
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      i_x_chpl22 = &call_tmp_chpl37;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      deinit_chpl95(i_x_chpl22, _ln_chpl, _fn_chpl);
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      i_x_chpl23 = &call_tmp_chpl36;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      deinit_chpl95(i_x_chpl23, _ln_chpl, _fn_chpl);
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      i_x_chpl24 = &call_tmp_chpl35;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      deinit_chpl95(i_x_chpl24, _ln_chpl, _fn_chpl);
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      i_x_chpl25 = &call_tmp_chpl34;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      deinit_chpl95(i_x_chpl25, _ln_chpl, _fn_chpl);
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      i_x_chpl26 = &call_tmp_chpl33;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      deinit_chpl95(i_x_chpl26, _ln_chpl, _fn_chpl);
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      i_x_chpl27 = &call_tmp_chpl32;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      deinit_chpl95(i_x_chpl27, _ln_chpl, _fn_chpl);
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      boundsCheckHalt_chpl(&msg_chpl3, _ln_chpl, _fn_chpl);
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      i_x_chpl28 = &msg_chpl3;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      deinit_chpl95(i_x_chpl28, _ln_chpl, _fn_chpl);
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    } else /* ZLINE: 630 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl */
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    if (p_chpl >= INT64(64)) /* ZLINE: 630 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl */
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    {
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      chpl___COLON_24(UINT64(1), &ret_tmp_chpl29, _ln_chpl, _fn_chpl);
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      call_tmp_chpl41 = ret_tmp_chpl29;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      chpl___PLUS_(&_str_literal_45_chpl, &call_tmp_chpl41, &ret_tmp_chpl30, _ln_chpl, _fn_chpl);
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      call_tmp_chpl42 = ret_tmp_chpl30;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      chpl___PLUS_(&call_tmp_chpl42, &_str_literal_46_chpl, &ret_tmp_chpl31, _ln_chpl, _fn_chpl);
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      call_tmp_chpl43 = ret_tmp_chpl31;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      chpl___COLON_25(p_chpl, &ret_tmp_chpl32, _ln_chpl, _fn_chpl);
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      call_tmp_chpl44 = ret_tmp_chpl32;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      chpl___PLUS_(&call_tmp_chpl43, &call_tmp_chpl44, &ret_tmp_chpl33, _ln_chpl, _fn_chpl);
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      call_tmp_chpl45 = ret_tmp_chpl33;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      chpl___PLUS_(&call_tmp_chpl45, &_str_literal_47_chpl, &ret_tmp_chpl34, _ln_chpl, _fn_chpl);
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      call_tmp_chpl46 = ret_tmp_chpl34;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      chpl___COLON_25(p_chpl, &ret_tmp_chpl35, _ln_chpl, _fn_chpl);
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      call_tmp_chpl47 = ret_tmp_chpl35;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      chpl___PLUS_(&call_tmp_chpl46, &call_tmp_chpl47, &ret_tmp_chpl36, _ln_chpl, _fn_chpl);
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      call_tmp_chpl48 = ret_tmp_chpl36;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      chpl___PLUS_(&call_tmp_chpl48, &_str_literal_50_chpl, &ret_tmp_chpl37, _ln_chpl, _fn_chpl);
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      call_tmp_chpl49 = ret_tmp_chpl37;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      chpl___PLUS_(&call_tmp_chpl49, &_str_literal_3965_chpl, &ret_tmp_chpl38, _ln_chpl, _fn_chpl);
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      call_tmp_chpl50 = ret_tmp_chpl38;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      msg_chpl4 = call_tmp_chpl50;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      i_x_chpl29 = &call_tmp_chpl49;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      deinit_chpl95(i_x_chpl29, _ln_chpl, _fn_chpl);
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      i_x_chpl30 = &call_tmp_chpl48;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      deinit_chpl95(i_x_chpl30, _ln_chpl, _fn_chpl);
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      i_x_chpl31 = &call_tmp_chpl47;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      deinit_chpl95(i_x_chpl31, _ln_chpl, _fn_chpl);
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      i_x_chpl32 = &call_tmp_chpl46;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      deinit_chpl95(i_x_chpl32, _ln_chpl, _fn_chpl);
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      i_x_chpl33 = &call_tmp_chpl45;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      deinit_chpl95(i_x_chpl33, _ln_chpl, _fn_chpl);
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      i_x_chpl34 = &call_tmp_chpl44;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      deinit_chpl95(i_x_chpl34, _ln_chpl, _fn_chpl);
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      i_x_chpl35 = &call_tmp_chpl43;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      deinit_chpl95(i_x_chpl35, _ln_chpl, _fn_chpl);
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      i_x_chpl36 = &call_tmp_chpl42;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      deinit_chpl95(i_x_chpl36, _ln_chpl, _fn_chpl);
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      i_x_chpl37 = &call_tmp_chpl41;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      deinit_chpl95(i_x_chpl37, _ln_chpl, _fn_chpl);
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      boundsCheckHalt_chpl(&msg_chpl4, _ln_chpl, _fn_chpl);
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      i_x_chpl38 = &msg_chpl4;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      deinit_chpl95(i_x_chpl38, _ln_chpl, _fn_chpl);
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    }
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    assert_chpl((UINT64(0) <= ((uint64_t)((UINT64(1) << p_chpl)))), _ln_chpl, _fn_chpl);
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    if (p_chpl < INT64(0)) /* ZLINE: 630 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl */
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    {
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      chpl___COLON_24(UINT64(1), &ret_tmp_chpl39, _ln_chpl, _fn_chpl);
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      call_tmp_chpl51 = ret_tmp_chpl39;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      chpl___PLUS_(&_str_literal_45_chpl, &call_tmp_chpl51, &ret_tmp_chpl40, _ln_chpl, _fn_chpl);
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      call_tmp_chpl52 = ret_tmp_chpl40;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      chpl___PLUS_(&call_tmp_chpl52, &_str_literal_46_chpl, &ret_tmp_chpl41, _ln_chpl, _fn_chpl);
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      call_tmp_chpl53 = ret_tmp_chpl41;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      chpl___COLON_25(p_chpl, &ret_tmp_chpl42, _ln_chpl, _fn_chpl);
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      call_tmp_chpl54 = ret_tmp_chpl42;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      chpl___PLUS_(&call_tmp_chpl53, &call_tmp_chpl54, &ret_tmp_chpl43, _ln_chpl, _fn_chpl);
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      call_tmp_chpl55 = ret_tmp_chpl43;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      chpl___PLUS_(&call_tmp_chpl55, &_str_literal_47_chpl, &ret_tmp_chpl44, _ln_chpl, _fn_chpl);
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      call_tmp_chpl56 = ret_tmp_chpl44;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      chpl___COLON_25(p_chpl, &ret_tmp_chpl45, _ln_chpl, _fn_chpl);
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      call_tmp_chpl57 = ret_tmp_chpl45;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      chpl___PLUS_(&call_tmp_chpl56, &call_tmp_chpl57, &ret_tmp_chpl46, _ln_chpl, _fn_chpl);
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      call_tmp_chpl58 = ret_tmp_chpl46;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      chpl___PLUS_(&call_tmp_chpl58, &_str_literal_48_chpl, &ret_tmp_chpl47, _ln_chpl, _fn_chpl);
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      call_tmp_chpl59 = ret_tmp_chpl47;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      msg_chpl5 = call_tmp_chpl59;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      i_x_chpl39 = &call_tmp_chpl58;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      deinit_chpl95(i_x_chpl39, _ln_chpl, _fn_chpl);
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      i_x_chpl40 = &call_tmp_chpl57;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      deinit_chpl95(i_x_chpl40, _ln_chpl, _fn_chpl);
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      i_x_chpl41 = &call_tmp_chpl56;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      deinit_chpl95(i_x_chpl41, _ln_chpl, _fn_chpl);
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      i_x_chpl42 = &call_tmp_chpl55;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      deinit_chpl95(i_x_chpl42, _ln_chpl, _fn_chpl);
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      i_x_chpl43 = &call_tmp_chpl54;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      deinit_chpl95(i_x_chpl43, _ln_chpl, _fn_chpl);
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      i_x_chpl44 = &call_tmp_chpl53;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      deinit_chpl95(i_x_chpl44, _ln_chpl, _fn_chpl);
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      i_x_chpl45 = &call_tmp_chpl52;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      deinit_chpl95(i_x_chpl45, _ln_chpl, _fn_chpl);
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      i_x_chpl46 = &call_tmp_chpl51;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      deinit_chpl95(i_x_chpl46, _ln_chpl, _fn_chpl);
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      boundsCheckHalt_chpl(&msg_chpl5, _ln_chpl, _fn_chpl);
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      i_x_chpl47 = &msg_chpl5;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      deinit_chpl95(i_x_chpl47, _ln_chpl, _fn_chpl);
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    } else /* ZLINE: 630 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl */
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    if (p_chpl >= INT64(64)) /* ZLINE: 630 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl */
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    {
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      chpl___COLON_24(UINT64(1), &ret_tmp_chpl48, _ln_chpl, _fn_chpl);
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      call_tmp_chpl60 = ret_tmp_chpl48;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      chpl___PLUS_(&_str_literal_45_chpl, &call_tmp_chpl60, &ret_tmp_chpl49, _ln_chpl, _fn_chpl);
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      call_tmp_chpl61 = ret_tmp_chpl49;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      chpl___PLUS_(&call_tmp_chpl61, &_str_literal_46_chpl, &ret_tmp_chpl50, _ln_chpl, _fn_chpl);
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      call_tmp_chpl62 = ret_tmp_chpl50;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      chpl___COLON_25(p_chpl, &ret_tmp_chpl51, _ln_chpl, _fn_chpl);
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      call_tmp_chpl63 = ret_tmp_chpl51;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      chpl___PLUS_(&call_tmp_chpl62, &call_tmp_chpl63, &ret_tmp_chpl52, _ln_chpl, _fn_chpl);
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      call_tmp_chpl64 = ret_tmp_chpl52;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      chpl___PLUS_(&call_tmp_chpl64, &_str_literal_47_chpl, &ret_tmp_chpl53, _ln_chpl, _fn_chpl);
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      call_tmp_chpl65 = ret_tmp_chpl53;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      chpl___COLON_25(p_chpl, &ret_tmp_chpl54, _ln_chpl, _fn_chpl);
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      call_tmp_chpl66 = ret_tmp_chpl54;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      chpl___PLUS_(&call_tmp_chpl65, &call_tmp_chpl66, &ret_tmp_chpl55, _ln_chpl, _fn_chpl);
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      call_tmp_chpl67 = ret_tmp_chpl55;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      chpl___PLUS_(&call_tmp_chpl67, &_str_literal_50_chpl, &ret_tmp_chpl56, _ln_chpl, _fn_chpl);
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      call_tmp_chpl68 = ret_tmp_chpl56;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      chpl___PLUS_(&call_tmp_chpl68, &_str_literal_3965_chpl, &ret_tmp_chpl57, _ln_chpl, _fn_chpl);
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      call_tmp_chpl69 = ret_tmp_chpl57;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      msg_chpl6 = call_tmp_chpl69;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      i_x_chpl48 = &call_tmp_chpl68;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      deinit_chpl95(i_x_chpl48, _ln_chpl, _fn_chpl);
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      i_x_chpl49 = &call_tmp_chpl67;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      deinit_chpl95(i_x_chpl49, _ln_chpl, _fn_chpl);
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      i_x_chpl50 = &call_tmp_chpl66;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      deinit_chpl95(i_x_chpl50, _ln_chpl, _fn_chpl);
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      i_x_chpl51 = &call_tmp_chpl65;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      deinit_chpl95(i_x_chpl51, _ln_chpl, _fn_chpl);
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      i_x_chpl52 = &call_tmp_chpl64;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      deinit_chpl95(i_x_chpl52, _ln_chpl, _fn_chpl);
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      i_x_chpl53 = &call_tmp_chpl63;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      deinit_chpl95(i_x_chpl53, _ln_chpl, _fn_chpl);
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      i_x_chpl54 = &call_tmp_chpl62;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      deinit_chpl95(i_x_chpl54, _ln_chpl, _fn_chpl);
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      i_x_chpl55 = &call_tmp_chpl61;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      deinit_chpl95(i_x_chpl55, _ln_chpl, _fn_chpl);
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      i_x_chpl56 = &call_tmp_chpl60;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      deinit_chpl95(i_x_chpl56, _ln_chpl, _fn_chpl);
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      boundsCheckHalt_chpl(&msg_chpl6, _ln_chpl, _fn_chpl);
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      i_x_chpl57 = &msg_chpl6;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      deinit_chpl95(i_x_chpl57, _ln_chpl, _fn_chpl);
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    }
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    if (UINT64(0) != ((uint64_t)((UINT64(1) << p_chpl)))) /* ZLINE: 630 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl */
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    {
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      halt_chpl15(&_str_literal_2423_chpl, UINT64(0), _ln_chpl, _fn_chpl);
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    }
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    call_tmp_chpl70 = sizeof(c_ptr_void_chpl);
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    if (UINT64(0) < call_tmp_chpl70) /* ZLINE: 630 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl */
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    {
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      halt_chpl(&_str_literal_2424_chpl, _ln_chpl, _fn_chpl);
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    }
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    call_tmp_chpl71 = offset_ARRAY_ELEMENTS_chpl();
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    call_tmp_chpl72 = chpl_here_aligned_alloc(UINT64(0), call_tmp_chpl7, call_tmp_chpl71, _ln_chpl, _fn_chpl);
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    ptr_chpl = call_tmp_chpl72;
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  }
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  call_tmp_chpl73 = ((c_ptr_int64_t_chpl)(ptr_chpl));
#line 630 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  p_chpl2 = call_tmp_chpl73;
#line 631 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  inlineImm_chpl = INT64(0);
#line 631 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  call_tmp_chpl74 = (p_chpl2 + inlineImm_chpl);
#line 631 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  *(call_tmp_chpl74) = i_chpl;
#line 632 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  chpl_here_free(((c_ptr_void_chpl)(call_tmp_chpl73)), _ln_chpl, _fn_chpl);
#line 629 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  return;
#line 629 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
}

#line 629 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
/* ChapelLocale.chpl:629 */
#line 629 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
static void wrapcoforall_fn_chpl63(_class_localscoforall_fn_chpl63 c_chpl) {
#line 629 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  int32_t _fn_chpl;
#line 629 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  int64_t _ln_chpl;
#line 629 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  int64_t _0_i_chpl;
#line 629 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _fn_chpl = (c_chpl)->_fn;
#line 629 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _ln_chpl = (c_chpl)->_ln;
#line 629 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  chpl_rmem_consist_acquire(_ln_chpl, _fn_chpl);
#line 629 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  chpl_check_nil(c_chpl, _ln_chpl, _fn_chpl);
#line 629 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _0_i_chpl = (c_chpl)->_0_i;
#line 629 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  chpl_check_nil(c_chpl, _ln_chpl, _fn_chpl);
#line 629 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  coforall_fn_chpl63(_0_i_chpl, (c_chpl)->_1__coforallCount, _ln_chpl, _fn_chpl);
#line 629 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  chpl_check_nil(c_chpl, _ln_chpl, _fn_chpl);
#line 629 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _downEndCount_chpl((c_chpl)->_1__coforallCount, nil, _ln_chpl, _fn_chpl);
#line 629 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  return;
#line 629 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
}

#line 643 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
/* ChapelLocale.chpl:643 */
#line 643 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
static void chpl_init_rootLocale(int64_t _ln_chpl,
#line 643 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
                                 int32_t _fn_chpl) {
#line 644 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  chpl_bool tmp_chpl;
#line 647 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  RootLocale_chpl new_temp_chpl = NULL;
#line 648 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  BaseLocale_chpl coerce_tmp_chpl3 = NULL;
#line 648 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  RootLocale_chpl _formal_type_tmp__chpl = NULL;
#line 648 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  RootLocale_chpl ret_chpl = NULL;
#line 648 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  int32_t chpl_macro_tmp_15;
#line 644 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  if (numLocales_chpl > INT64(1)) /* ZLINE: 644 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl */
#line 644 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  {
#line 644 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    tmp_chpl = UINT8(true);
#line 644 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  } else /* ZLINE: 644 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl */
#line 644 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  {
#line 644 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    tmp_chpl = UINT8(false);
#line 644 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  }
#line 644 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  if (tmp_chpl) /* ZLINE: 644 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl */
#line 644 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  {
#line 645 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    halt_chpl(&_str_literal_714_chpl, _ln_chpl, _fn_chpl);
#line 645 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  }
#line 647 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  new_temp_chpl = _new_chpl55(_ln_chpl, _fn_chpl);
#line 647 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  (&origRootLocale_chpl)->_instance = ((BaseLocale_chpl)(new_temp_chpl));
#line 648 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  coerce_tmp_chpl3 = (&origRootLocale_chpl)->_instance;
#line 648 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  if (((RootClass_chpl)(coerce_tmp_chpl3)) == nil) /* ZLINE: 648 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl */
#line 648 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  {
#line 648 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    ret_chpl = ((RootLocale_chpl)(nil));
#line 648 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    goto _end__COLON__chpl;
#line 648 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  }
#line 648 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  chpl_macro_tmp_15 = ((RootClass_chpl)(coerce_tmp_chpl3))->chpl__cid;
#line 648 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  ret_chpl = ( (((chpl__cid_RootLocale_chpl <= chpl_macro_tmp_15) && (chpl_macro_tmp_15 <= chpl_subclass_max_id[chpl__cid_RootLocale_chpl])))?(((RootLocale_chpl)(coerce_tmp_chpl3))):(((RootLocale_chpl)(NULL))) );
#line 648 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _end__COLON__chpl:;
#line 648 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  if (((RootClass_chpl)(ret_chpl)) == nil) /* ZLINE: 648 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl */
#line 648 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  {
#line 648 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    nilCheckHalt_chpl(&_str_literal_52_chpl, _ln_chpl, _fn_chpl);
#line 648 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  }
#line 648 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  chpl_check_nil(ret_chpl, _ln_chpl, _fn_chpl);
#line 648 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  setup_chpl2(ret_chpl, _ln_chpl, _fn_chpl);
#line 643 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  return;
#line 643 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
}

#line 655 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
/* ChapelLocale.chpl:655 */
#line 655 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
static void chpl_rootLocaleInitPrivate(int64_t locIdx_chpl,
#line 655 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
                                       int64_t _ln_chpl,
#line 655 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
                                       int32_t _fn_chpl) {
#line 677 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  int64_t local_numLocales_chpl;
#line 662 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  RootLocale_chpl new_temp_chpl = NULL;
#line 662 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  RootLocale_chpl newRootLocale_chpl = NULL;
#line 667 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  BaseLocale_chpl coerce_tmp_chpl3 = NULL;
#line 667 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  RootLocale_chpl _formal_type_tmp__chpl = NULL;
#line 667 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  RootLocale_chpl ret_chpl = NULL;
#line 667 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _ref__array_DefaultRectangularArr_1_int64_t_one_locale_int64_t_chpl call_tmp_chpl6 = NULL;
#line 668 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  DefaultRectangularArr_1_int64_t_one_locale_int64_t_chpl coerce_tmp_chpl4 = NULL;
#line 668 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _ddata_locale_chpl origRL_chpl = NULL;
#line 669 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _ref__array_DefaultRectangularArr_1_int64_t_one_locale_int64_t_chpl call_tmp_chpl7 = NULL;
#line 669 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  DefaultRectangularArr_1_int64_t_one_locale_int64_t_chpl coerce_tmp_chpl5 = NULL;
#line 669 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _ddata_locale_chpl newRL_chpl = NULL;
#line 674 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _ref_locale_chpl call_tmp_chpl8 = NULL;
#line 676 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _ref_locale_chpl call_tmp_chpl9 = NULL;
#line 685 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  BaseLocale_chpl coerce_tmp_chpl6 = NULL;
#line 685 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  RootLocale_chpl _formal_type_tmp__chpl2 = NULL;
#line 685 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  RootLocale_chpl ret_chpl2 = NULL;
#line 685 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _ref__array_DefaultRectangularArr_1_int64_t_one_locale_int64_t_chpl call_tmp_chpl10 = NULL;
#line 685 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  int32_t chpl_macro_tmp_16;
#line 685 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  int32_t chpl_macro_tmp_17;
#line 677 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  local_numLocales_chpl = numLocales_chpl;
#line 659 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  chpl___ASSIGN_9(&rootLocale_chpl, &origRootLocale_chpl);
#line 660 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  if (locIdx_chpl != INT64(0)) /* ZLINE: 660 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl */
#line 660 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  {
#line 662 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    new_temp_chpl = _new_chpl55(_ln_chpl, _fn_chpl);
#line 662 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    newRootLocale_chpl = new_temp_chpl;
#line 667 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    coerce_tmp_chpl3 = (&origRootLocale_chpl)->_instance;
#line 667 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    if (((RootClass_chpl)(coerce_tmp_chpl3)) == nil) /* ZLINE: 667 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl */
#line 667 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    {
#line 667 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      ret_chpl = ((RootLocale_chpl)(nil));
#line 667 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      goto _end__COLON__chpl;
#line 667 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    }
#line 667 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    chpl_macro_tmp_16 = ((RootClass_chpl)(coerce_tmp_chpl3))->chpl__cid;
#line 667 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    ret_chpl = ( (((chpl__cid_RootLocale_chpl <= chpl_macro_tmp_16) && (chpl_macro_tmp_16 <= chpl_subclass_max_id[chpl__cid_RootLocale_chpl])))?(((RootLocale_chpl)(coerce_tmp_chpl3))):(((RootLocale_chpl)(NULL))) );
#line 667 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    _end__COLON__chpl:;
#line 667 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    if (((RootClass_chpl)(ret_chpl)) == nil) /* ZLINE: 667 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl */
#line 667 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    {
#line 667 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      nilCheckHalt_chpl(&_str_literal_52_chpl, _ln_chpl, _fn_chpl);
#line 667 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    }
#line 667 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    chpl_check_nil(ret_chpl, _ln_chpl, _fn_chpl);
#line 667 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    call_tmp_chpl6 = getDefaultLocaleArray_chpl(ret_chpl, _ln_chpl, _fn_chpl);
#line 668 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    coerce_tmp_chpl4 = (call_tmp_chpl6)->_instance;
#line 668 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    chpl_check_nil(coerce_tmp_chpl4, _ln_chpl, _fn_chpl);
#line 668 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    origRL_chpl = (coerce_tmp_chpl4)->shiftedData;
#line 669 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    chpl_check_nil(new_temp_chpl, _ln_chpl, _fn_chpl);
#line 669 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    call_tmp_chpl7 = getDefaultLocaleArray_chpl(new_temp_chpl, _ln_chpl, _fn_chpl);
#line 669 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    coerce_tmp_chpl5 = (call_tmp_chpl7)->_instance;
#line 669 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    chpl_check_nil(coerce_tmp_chpl5, _ln_chpl, _fn_chpl);
#line 669 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    newRL_chpl = (coerce_tmp_chpl5)->shiftedData;
#line 674 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    call_tmp_chpl8 = (newRL_chpl + INT64(0));
#line 676 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    call_tmp_chpl9 = (origRL_chpl + INT64(0));
#line 673 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    chpl_gen_comm_get(((void*)(call_tmp_chpl8)), INT64(0), call_tmp_chpl9, (sizeof(locale_chpl) * ((uint64_t)(local_numLocales_chpl))), COMMID(0), _ln_chpl, _fn_chpl);
#line 679 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    (&rootLocale_chpl)->_instance = ((BaseLocale_chpl)(newRootLocale_chpl));
#line 679 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  }
#line 681 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  if (locIdx_chpl != INT64(0)) /* ZLINE: 681 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl */
#line 681 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  {
#line 685 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    coerce_tmp_chpl6 = (&rootLocale_chpl)->_instance;
#line 685 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    if (((RootClass_chpl)(coerce_tmp_chpl6)) == nil) /* ZLINE: 685 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl */
#line 685 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    {
#line 685 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      ret_chpl2 = ((RootLocale_chpl)(nil));
#line 685 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      goto _end__COLON__chpl2;
#line 685 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    }
#line 685 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    chpl_macro_tmp_17 = ((RootClass_chpl)(coerce_tmp_chpl6))->chpl__cid;
#line 685 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    ret_chpl2 = ( (((chpl__cid_RootLocale_chpl <= chpl_macro_tmp_17) && (chpl_macro_tmp_17 <= chpl_subclass_max_id[chpl__cid_RootLocale_chpl])))?(((RootLocale_chpl)(coerce_tmp_chpl6))):(((RootLocale_chpl)(NULL))) );
#line 685 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    _end__COLON__chpl2:;
#line 685 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    if (((RootClass_chpl)(ret_chpl2)) == nil) /* ZLINE: 685 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl */
#line 685 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    {
#line 685 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      nilCheckHalt_chpl(&_str_literal_52_chpl, _ln_chpl, _fn_chpl);
#line 685 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    }
#line 685 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    chpl_check_nil(ret_chpl2, _ln_chpl, _fn_chpl);
#line 685 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    call_tmp_chpl10 = getDefaultLocaleArray_chpl(ret_chpl2, _ln_chpl, _fn_chpl);
#line 686 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    Locales_chpl = call_tmp_chpl10;
#line 686 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  }
#line 688 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  rootLocaleInitialized_chpl = UINT8(true);
#line 655 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  return;
#line 655 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
}

#line 691 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
/* ChapelLocale.chpl:691 */
#line 691 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
static void chpl_defaultLocaleInitPrivate(int64_t _ln_chpl,
#line 691 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
                                          int32_t _fn_chpl) {
#line 693 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  BaseLocale_chpl coerce_tmp_chpl3 = NULL;
#line 693 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  RootLocale_chpl _formal_type_tmp__chpl = NULL;
#line 693 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  RootLocale_chpl ret_chpl = NULL;
#line 693 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _ref__array_DefaultRectangularArr_1_int64_t_one_locale_int64_t_chpl call_tmp_chpl6 = NULL;
#line 694 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _tuple_1_star_int64_t_chpl coerce_tmp_chpl4;
#line 694 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _ref__tuple_1_star_int64_t_chpl i_i_chpl = NULL;
#line 694 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  DefaultRectangularArr_1_int64_t_one_locale_int64_t_chpl ret_chpl2 = NULL;
#line 694 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  DefaultRectangularArr_1_int64_t_one_locale_int64_t_chpl coerce_tmp_chpl5 = NULL;
#line 694 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _tuple_1_star_int64_t_chpl _formal_tmp_in_ind_chpl;
#line 694 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  int64_t _formal_tmp_ind_x0_chpl;
#line 694 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _ddata_locale_chpl coerce_tmp_chpl6 = NULL;
#line 694 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _ref_locale_chpl call_tmp_chpl7 = NULL;
#line 694 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  int32_t chpl_macro_tmp_18;
#line 693 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  coerce_tmp_chpl3 = (&rootLocale_chpl)->_instance;
#line 693 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  if (((RootClass_chpl)(coerce_tmp_chpl3)) == nil) /* ZLINE: 693 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl */
#line 693 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  {
#line 693 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    ret_chpl = ((RootLocale_chpl)(nil));
#line 693 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    goto _end__COLON__chpl;
#line 693 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  }
#line 693 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  chpl_macro_tmp_18 = ((RootClass_chpl)(coerce_tmp_chpl3))->chpl__cid;
#line 693 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  ret_chpl = ( (((chpl__cid_RootLocale_chpl <= chpl_macro_tmp_18) && (chpl_macro_tmp_18 <= chpl_subclass_max_id[chpl__cid_RootLocale_chpl])))?(((RootLocale_chpl)(coerce_tmp_chpl3))):(((RootLocale_chpl)(NULL))) );
#line 693 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _end__COLON__chpl:;
#line 693 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  if (((RootClass_chpl)(ret_chpl)) == nil) /* ZLINE: 693 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl */
#line 693 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  {
#line 693 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    nilCheckHalt_chpl(&_str_literal_52_chpl, _ln_chpl, _fn_chpl);
#line 693 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  }
#line 693 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  chpl_check_nil(ret_chpl, _ln_chpl, _fn_chpl);
#line 693 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  call_tmp_chpl6 = getDefaultLocaleArray_chpl(ret_chpl, _ln_chpl, _fn_chpl);
#line 694 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  *(coerce_tmp_chpl4 + INT64(0)) = INT64(0);
#line 694 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  i_i_chpl = &coerce_tmp_chpl4;
#line 694 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  ret_chpl2 = (call_tmp_chpl6)->_instance;
#line 694 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  checkAccess_chpl7(call_tmp_chpl6, i_i_chpl, ret_chpl2, _ln_chpl, _fn_chpl);
#line 694 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  coerce_tmp_chpl5 = ret_chpl2;
#line 694 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  *(_formal_tmp_in_ind_chpl + INT64(0)) = *(coerce_tmp_chpl4 + INT64(0));
#line 694 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _formal_tmp_ind_x0_chpl = *(_formal_tmp_in_ind_chpl + INT64(0));
#line 694 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  chpl_check_nil(coerce_tmp_chpl5, _ln_chpl, _fn_chpl);
#line 694 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  coerce_tmp_chpl6 = (coerce_tmp_chpl5)->shiftedData;
#line 694 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  call_tmp_chpl7 = (coerce_tmp_chpl6 + _formal_tmp_ind_x0_chpl);
#line 694 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  (&defaultLocale_chpl)->_instance = (call_tmp_chpl7)->_instance;
#line 691 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  return;
#line 691 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
}

#line 697 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
/* ChapelLocale.chpl:697 */
#line 697 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
static void chpl_singletonCurrentLocaleInitPrivateSublocs(locale_chpl * arg_chpl,
#line 697 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
                                                          int64_t _ln_chpl,
#line 697 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
                                                          int32_t _fn_chpl) {
#line 698 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  BaseLocale_chpl coerce_tmp_chpl3 = NULL;
#line 698 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  int64_t call_tmp_chpl6;
#line 698 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  int64_t tmp_chpl;
#line 698 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  int64_t _ic__F1_high_chpl;
#line 698 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  int64_t i_chpl;
#line 699 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  locale_chpl call_tmp_chpl7;
#line 92 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  BaseLocale_chpl coerce_tmp_chpl4 = NULL;
#line 699 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  locale_chpl ret_tmp_chpl;
#line 699 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  locale_chpl subloc_chpl;
#line 701 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  BaseLocale_chpl coerce_tmp_chpl5 = NULL;
#line 701 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  AbstractLocaleModel_chpl _formal_type_tmp__chpl = NULL;
#line 701 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  AbstractLocaleModel_chpl ret_chpl = NULL;
#line 705 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  AbstractLocaleModel_chpl coerce_tmp_chpl6 = NULL;
#line 705 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _ref__array_DefaultRectangularArr_1_int64_t_one_locale_int64_t_chpl call_tmp_chpl8 = NULL;
#line 705 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _tuple_1_star_int64_t_chpl coerce_tmp_chpl7;
#line 705 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _ref__tuple_1_star_int64_t_chpl i_i_chpl = NULL;
#line 705 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  DefaultRectangularArr_1_int64_t_one_locale_int64_t_chpl ret_chpl2 = NULL;
#line 705 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  DefaultRectangularArr_1_int64_t_one_locale_int64_t_chpl coerce_tmp_chpl8 = NULL;
#line 705 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _tuple_1_star_int64_t_chpl _formal_tmp_in_ind_chpl;
#line 705 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  int64_t _formal_tmp_ind_x0_chpl;
#line 705 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _ddata_locale_chpl coerce_tmp_chpl9 = NULL;
#line 705 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _ref_locale_chpl call_tmp_chpl9 = NULL;
#line 707 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _ref_locale_chpl i_x_chpl = NULL;
#line 707 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  int32_t chpl_macro_tmp_19;
#line 698 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  coerce_tmp_chpl3 = (arg_chpl)->_instance;
#line 698 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  if (((RootClass_chpl)(coerce_tmp_chpl3)) == nil) /* ZLINE: 698 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl */
#line 698 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  {
#line 698 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    nilCheckHalt_chpl(&_str_literal_52_chpl, _ln_chpl, _fn_chpl);
#line 698 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  }
#line 698 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  chpl_check_nil(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 698 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  call_tmp_chpl6 = ((int64_t(*)(BaseLocale_chpl,
#line 698 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
                      int64_t,
#line 698 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
                      int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(coerce_tmp_chpl3))->chpl__cid) + INT64(2))])(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 698 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  if (call_tmp_chpl6 < INT64(0)) /* ZLINE: 698 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl */
#line 698 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  {
#line 698 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    boundsCheckHalt_chpl(&_str_literal_580_chpl, _ln_chpl, _fn_chpl);
#line 698 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  }
#line 698 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  if (call_tmp_chpl6 == INT64(0)) /* ZLINE: 698 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl */
#line 698 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  {
#line 698 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    tmp_chpl = ((int64_t)(((uint64_t)((((uint64_t)(INT64(0))) - UINT64(1))))));
#line 698 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  } else /* ZLINE: 698 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl */
#line 698 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  {
#line 698 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    tmp_chpl = ((int64_t)((INT64(0) + ((int64_t)((call_tmp_chpl6 - INT64(1)))))));
#line 698 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  }
#line 698 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _ic__F1_high_chpl = tmp_chpl;
#line 698 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  i_chpl = INT64(0);
#line 698 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  chpl_checkIfRangeIterWillOverflow(INT64(0), tmp_chpl, INT64(1), INT64(0), tmp_chpl, UINT8(true), _ln_chpl, _fn_chpl);
#line 698 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  for (i_chpl = INT64(0); ((i_chpl <= _ic__F1_high_chpl)); i_chpl += INT64(1)) {
#line 92 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    coerce_tmp_chpl4 = (arg_chpl)->_instance;
#line 92 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    if (((RootClass_chpl)(coerce_tmp_chpl4)) == nil) /* ZLINE: 92 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl */
#line 92 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    {
#line 92 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      nilCheckHalt_chpl(&_str_literal_52_chpl, _ln_chpl, _fn_chpl);
#line 92 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    }
#line 699 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    chpl_check_nil(coerce_tmp_chpl4, _ln_chpl, _fn_chpl);
#line 699 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    ((void(*)(BaseLocale_chpl,
#line 699 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
       int64_t,
#line 699 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
       locale_chpl *,
#line 699 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
       int64_t,
#line 699 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
       int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(coerce_tmp_chpl4))->chpl__cid) + INT64(1))])(coerce_tmp_chpl4, i_chpl, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 699 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    call_tmp_chpl7 = ret_tmp_chpl;
#line 699 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    subloc_chpl = call_tmp_chpl7;
#line 701 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    coerce_tmp_chpl5 = (&subloc_chpl)->_instance;
#line 701 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    if (((RootClass_chpl)(coerce_tmp_chpl5)) == nil) /* ZLINE: 701 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl */
#line 701 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    {
#line 701 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      ret_chpl = ((AbstractLocaleModel_chpl)(nil));
#line 701 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      goto _end__COLON__chpl;
#line 701 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    }
#line 701 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    chpl_macro_tmp_19 = ((RootClass_chpl)(coerce_tmp_chpl5))->chpl__cid;
#line 701 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    ret_chpl = ( (((chpl__cid_AbstractLocaleModel_chpl <= chpl_macro_tmp_19) && (chpl_macro_tmp_19 <= chpl_subclass_max_id[chpl__cid_AbstractLocaleModel_chpl])))?(((AbstractLocaleModel_chpl)(coerce_tmp_chpl5))):(((AbstractLocaleModel_chpl)(NULL))) );
#line 701 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    _end__COLON__chpl:;
#line 702 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    if (((RootClass_chpl)(ret_chpl)) == nil) /* ZLINE: 702 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl */
#line 702 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    {
#line 703 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      halt_chpl(&_str_literal_718_chpl, _ln_chpl, _fn_chpl);
#line 703 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    }
#line 705 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    if (((RootClass_chpl)(ret_chpl)) == nil) /* ZLINE: 705 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl */
#line 705 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    {
#line 705 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      nilCheckHalt_chpl(&_str_literal_52_chpl, _ln_chpl, _fn_chpl);
#line 705 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    }
#line 705 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    coerce_tmp_chpl6 = ret_chpl;
#line 705 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    chpl_check_nil(coerce_tmp_chpl6, _ln_chpl, _fn_chpl);
#line 705 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    call_tmp_chpl8 = &((coerce_tmp_chpl6)->chpl_singletonThisLocaleArray);
#line 705 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    *(coerce_tmp_chpl7 + INT64(0)) = INT64(0);
#line 705 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    i_i_chpl = &coerce_tmp_chpl7;
#line 705 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    ret_chpl2 = (call_tmp_chpl8)->_instance;
#line 705 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    checkAccess_chpl7(call_tmp_chpl8, i_i_chpl, ret_chpl2, _ln_chpl, _fn_chpl);
#line 705 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    coerce_tmp_chpl8 = ret_chpl2;
#line 705 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    *(_formal_tmp_in_ind_chpl + INT64(0)) = *(coerce_tmp_chpl7 + INT64(0));
#line 705 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    _formal_tmp_ind_x0_chpl = *(_formal_tmp_in_ind_chpl + INT64(0));
#line 705 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    chpl_check_nil(coerce_tmp_chpl8, _ln_chpl, _fn_chpl);
#line 705 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    coerce_tmp_chpl9 = (coerce_tmp_chpl8)->shiftedData;
#line 705 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    call_tmp_chpl9 = (coerce_tmp_chpl9 + _formal_tmp_ind_x0_chpl);
#line 705 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    (call_tmp_chpl9)->_instance = ((BaseLocale_chpl)(ret_chpl));
#line 707 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    chpl_singletonCurrentLocaleInitPrivateSublocs(&subloc_chpl, _ln_chpl, _fn_chpl);
#line 707 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    i_x_chpl = &subloc_chpl;
#line 707 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    deinit_chpl62(i_x_chpl);
#line 707 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  }
#line 697 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  return;
#line 697 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
}

#line 710 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
/* ChapelLocale.chpl:710 */
#line 710 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
static void chpl_singletonCurrentLocaleInitPrivate(int64_t locIdx_chpl,
#line 710 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
                                                   int64_t _ln_chpl,
#line 710 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
                                                   int32_t _fn_chpl) {
#line 712 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  BaseLocale_chpl coerce_tmp_chpl3 = NULL;
#line 712 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  RootLocale_chpl _formal_type_tmp__chpl = NULL;
#line 712 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  RootLocale_chpl ret_chpl = NULL;
#line 712 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _ref__array_DefaultRectangularArr_1_int64_t_one_locale_int64_t_chpl call_tmp_chpl6 = NULL;
#line 713 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _tuple_1_star_int64_t_chpl coerce_tmp_chpl4;
#line 713 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _ref__tuple_1_star_int64_t_chpl i_i_chpl = NULL;
#line 713 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  DefaultRectangularArr_1_int64_t_one_locale_int64_t_chpl ret_chpl2 = NULL;
#line 713 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  DefaultRectangularArr_1_int64_t_one_locale_int64_t_chpl coerce_tmp_chpl5 = NULL;
#line 713 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _tuple_1_star_int64_t_chpl _formal_tmp_in_ind_chpl;
#line 713 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  int64_t _formal_tmp_ind_x0_chpl;
#line 713 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _ddata_locale_chpl coerce_tmp_chpl6 = NULL;
#line 713 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _ref_locale_chpl call_tmp_chpl7 = NULL;
#line 713 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  locale_chpl loc_chpl;
#line 714 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  BaseLocale_chpl coerce_tmp_chpl7 = NULL;
#line 714 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  AbstractLocaleModel_chpl _formal_type_tmp__chpl2 = NULL;
#line 714 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  AbstractLocaleModel_chpl ret_chpl3 = NULL;
#line 718 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  AbstractLocaleModel_chpl coerce_tmp_chpl8 = NULL;
#line 718 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _ref__array_DefaultRectangularArr_1_int64_t_one_locale_int64_t_chpl call_tmp_chpl8 = NULL;
#line 718 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _tuple_1_star_int64_t_chpl coerce_tmp_chpl9;
#line 718 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _ref__tuple_1_star_int64_t_chpl i_i_chpl2 = NULL;
#line 718 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  DefaultRectangularArr_1_int64_t_one_locale_int64_t_chpl ret_chpl4 = NULL;
#line 718 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  DefaultRectangularArr_1_int64_t_one_locale_int64_t_chpl coerce_tmp_chpl10 = NULL;
#line 718 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _tuple_1_star_int64_t_chpl _formal_tmp_in_ind_chpl2;
#line 718 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  int64_t _formal_tmp_ind_x0_chpl2;
#line 718 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _ddata_locale_chpl coerce_tmp_chpl11 = NULL;
#line 718 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _ref_locale_chpl call_tmp_chpl9 = NULL;
#line 710 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _ref_locale_chpl i_x_chpl = NULL;
#line 710 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  int32_t chpl_macro_tmp_20;
#line 710 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  int32_t chpl_macro_tmp_21;
#line 712 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  coerce_tmp_chpl3 = (&rootLocale_chpl)->_instance;
#line 712 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  if (((RootClass_chpl)(coerce_tmp_chpl3)) == nil) /* ZLINE: 712 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl */
#line 712 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  {
#line 712 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    ret_chpl = ((RootLocale_chpl)(nil));
#line 712 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    goto _end__COLON__chpl;
#line 712 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  }
#line 712 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  chpl_macro_tmp_20 = ((RootClass_chpl)(coerce_tmp_chpl3))->chpl__cid;
#line 712 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  ret_chpl = ( (((chpl__cid_RootLocale_chpl <= chpl_macro_tmp_20) && (chpl_macro_tmp_20 <= chpl_subclass_max_id[chpl__cid_RootLocale_chpl])))?(((RootLocale_chpl)(coerce_tmp_chpl3))):(((RootLocale_chpl)(NULL))) );
#line 712 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _end__COLON__chpl:;
#line 712 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  if (((RootClass_chpl)(ret_chpl)) == nil) /* ZLINE: 712 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl */
#line 712 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  {
#line 712 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    nilCheckHalt_chpl(&_str_literal_52_chpl, _ln_chpl, _fn_chpl);
#line 712 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  }
#line 712 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  chpl_check_nil(ret_chpl, _ln_chpl, _fn_chpl);
#line 712 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  call_tmp_chpl6 = getDefaultLocaleArray_chpl(ret_chpl, _ln_chpl, _fn_chpl);
#line 713 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  *(coerce_tmp_chpl4 + INT64(0)) = locIdx_chpl;
#line 713 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  i_i_chpl = &coerce_tmp_chpl4;
#line 713 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  ret_chpl2 = (call_tmp_chpl6)->_instance;
#line 713 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  checkAccess_chpl7(call_tmp_chpl6, i_i_chpl, ret_chpl2, _ln_chpl, _fn_chpl);
#line 713 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  coerce_tmp_chpl5 = ret_chpl2;
#line 713 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  *(_formal_tmp_in_ind_chpl + INT64(0)) = *(coerce_tmp_chpl4 + INT64(0));
#line 713 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _formal_tmp_ind_x0_chpl = *(_formal_tmp_in_ind_chpl + INT64(0));
#line 713 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  chpl_check_nil(coerce_tmp_chpl5, _ln_chpl, _fn_chpl);
#line 713 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  coerce_tmp_chpl6 = (coerce_tmp_chpl5)->shiftedData;
#line 713 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  call_tmp_chpl7 = (coerce_tmp_chpl6 + _formal_tmp_ind_x0_chpl);
#line 713 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  init_ASSIGN__chpl(&loc_chpl, call_tmp_chpl7);
#line 714 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  coerce_tmp_chpl7 = (&loc_chpl)->_instance;
#line 714 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  if (((RootClass_chpl)(coerce_tmp_chpl7)) == nil) /* ZLINE: 714 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl */
#line 714 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  {
#line 714 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    ret_chpl3 = ((AbstractLocaleModel_chpl)(nil));
#line 714 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    goto _end__COLON__chpl2;
#line 714 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  }
#line 714 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  chpl_macro_tmp_21 = ((RootClass_chpl)(coerce_tmp_chpl7))->chpl__cid;
#line 714 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  ret_chpl3 = ( (((chpl__cid_AbstractLocaleModel_chpl <= chpl_macro_tmp_21) && (chpl_macro_tmp_21 <= chpl_subclass_max_id[chpl__cid_AbstractLocaleModel_chpl])))?(((AbstractLocaleModel_chpl)(coerce_tmp_chpl7))):(((AbstractLocaleModel_chpl)(NULL))) );
#line 714 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _end__COLON__chpl2:;
#line 715 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  if (((RootClass_chpl)(ret_chpl3)) == nil) /* ZLINE: 715 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl */
#line 715 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  {
#line 716 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    halt_chpl(&_str_literal_718_chpl, _ln_chpl, _fn_chpl);
#line 716 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  }
#line 718 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  if (((RootClass_chpl)(ret_chpl3)) == nil) /* ZLINE: 718 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl */
#line 718 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  {
#line 718 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    nilCheckHalt_chpl(&_str_literal_52_chpl, _ln_chpl, _fn_chpl);
#line 718 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  }
#line 718 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  coerce_tmp_chpl8 = ret_chpl3;
#line 718 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  chpl_check_nil(coerce_tmp_chpl8, _ln_chpl, _fn_chpl);
#line 718 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  call_tmp_chpl8 = &((coerce_tmp_chpl8)->chpl_singletonThisLocaleArray);
#line 718 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  *(coerce_tmp_chpl9 + INT64(0)) = INT64(0);
#line 718 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  i_i_chpl2 = &coerce_tmp_chpl9;
#line 718 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  ret_chpl4 = (call_tmp_chpl8)->_instance;
#line 718 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  checkAccess_chpl7(call_tmp_chpl8, i_i_chpl2, ret_chpl4, _ln_chpl, _fn_chpl);
#line 718 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  coerce_tmp_chpl10 = ret_chpl4;
#line 718 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  *(_formal_tmp_in_ind_chpl2 + INT64(0)) = *(coerce_tmp_chpl9 + INT64(0));
#line 718 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _formal_tmp_ind_x0_chpl2 = *(_formal_tmp_in_ind_chpl2 + INT64(0));
#line 718 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  chpl_check_nil(coerce_tmp_chpl10, _ln_chpl, _fn_chpl);
#line 718 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  coerce_tmp_chpl11 = (coerce_tmp_chpl10)->shiftedData;
#line 718 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  call_tmp_chpl9 = (coerce_tmp_chpl11 + _formal_tmp_ind_x0_chpl2);
#line 718 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  (call_tmp_chpl9)->_instance = ((BaseLocale_chpl)(ret_chpl3));
#line 719 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  chpl_singletonCurrentLocaleInitPrivateSublocs(&loc_chpl, _ln_chpl, _fn_chpl);
#line 710 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  i_x_chpl = &loc_chpl;
#line 710 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  deinit_chpl62(i_x_chpl);
#line 710 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  return;
#line 710 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
}

#line 728 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
/* ChapelLocale.chpl:728 */
#line 728 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
void chpl_getLocaleID(chpl_localeID_t * localeID,
#line 728 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
                      int64_t _ln,
#line 728 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
                      int32_t _fn) {
#line 729 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  chpl_localeID_t call_tmp_chpl6;
#line 729 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  int32_t call_tmp_chpl7;
#line 729 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  chpl_localeID_t call_tmp_chpl8;
#line 729 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _ref_chpl_localeID_t_chpl i__arg2_chpl = NULL;
#line 729 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  call_tmp_chpl7 = get_chpl_nodeID();
#line 729 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  call_tmp_chpl8 = chpl_rt_buildLocaleID(call_tmp_chpl7, c_sublocid_any);
#line 729 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  call_tmp_chpl6 = call_tmp_chpl8;
#line 729 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  i__arg2_chpl = &call_tmp_chpl6;
#line 729 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  *(localeID) = *(i__arg2_chpl);
#line 728 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  return;
#line 728 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
}

#line 743 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
/* ChapelLocale.chpl:743 */
#line 743 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
static void chpl_localeID_to_locale(chpl_localeID_t * id_chpl2,
#line 743 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
                                    locale_chpl * _retArg_chpl,
#line 743 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
                                    int64_t _ln_chpl,
#line 743 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
                                    int32_t _fn_chpl) {
#line 743 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  locale_chpl ret_chpl;
#line 745 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  BaseLocale_chpl coerce_tmp_chpl3 = NULL;
#line 745 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  AbstractRootLocale_chpl _formal_type_tmp__chpl = NULL;
#line 745 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  AbstractRootLocale_chpl ret_chpl2 = NULL;
#line 745 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  locale_chpl call_tmp_chpl6;
#line 745 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  locale_chpl ret_tmp_chpl;
#line 753 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  DummyLocale_chpl new_temp_chpl = NULL;
#line 755 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _ref_locale_chpl i_x_chpl = NULL;
#line 755 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  locale_chpl ret_chpl3;
#line 755 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  locale_chpl tmp_chpl;
#line 755 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  int32_t chpl_macro_tmp_22;
#line 744 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  if (((RootClass_chpl)((&rootLocale_chpl)->_instance)) != nil) /* ZLINE: 744 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl */
#line 744 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  {
#line 745 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    coerce_tmp_chpl3 = (&rootLocale_chpl)->_instance;
#line 745 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    if (((RootClass_chpl)(coerce_tmp_chpl3)) == nil) /* ZLINE: 745 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl */
#line 745 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    {
#line 745 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      ret_chpl2 = ((AbstractRootLocale_chpl)(nil));
#line 745 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      goto _end__COLON__chpl;
#line 745 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    }
#line 745 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    chpl_macro_tmp_22 = ((RootClass_chpl)(coerce_tmp_chpl3))->chpl__cid;
#line 745 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    ret_chpl2 = ( (((chpl__cid_AbstractRootLocale_chpl <= chpl_macro_tmp_22) && (chpl_macro_tmp_22 <= chpl_subclass_max_id[chpl__cid_AbstractRootLocale_chpl])))?(((AbstractRootLocale_chpl)(coerce_tmp_chpl3))):(((AbstractRootLocale_chpl)(NULL))) );
#line 745 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    _end__COLON__chpl:;
#line 745 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    if (((RootClass_chpl)(ret_chpl2)) == nil) /* ZLINE: 745 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl */
#line 745 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    {
#line 745 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      nilCheckHalt_chpl(&_str_literal_52_chpl, _ln_chpl, _fn_chpl);
#line 745 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    }
#line 745 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    chpl_check_nil(ret_chpl2, _ln_chpl, _fn_chpl);
#line 745 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    ((void(*)(AbstractRootLocale_chpl,
#line 745 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
       chpl_localeID_t *,
#line 745 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
       locale_chpl *,
#line 745 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
       int64_t,
#line 745 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
       int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(ret_chpl2))->chpl__cid) + INT64(3))])(ret_chpl2, id_chpl2, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 745 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    call_tmp_chpl6 = ret_tmp_chpl;
#line 745 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    ret_chpl = call_tmp_chpl6;
#line 745 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    goto _end_chpl_localeID_to_locale_chpl;
#line 745 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  } else /* ZLINE: 746 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl */
#line 745 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  {
#line 752 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    if (((RootClass_chpl)((&dummyLocale_chpl)->_instance)) == nil) /* ZLINE: 752 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl */
#line 752 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    {
#line 753 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      new_temp_chpl = _new_chpl19(_ln_chpl, _fn_chpl);
#line 753 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      (&dummyLocale_chpl)->_instance = ((BaseLocale_chpl)(new_temp_chpl));
#line 753 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    }
#line 755 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    i_x_chpl = &dummyLocale_chpl;
#line 755 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    init_ASSIGN__chpl(&tmp_chpl, i_x_chpl);
#line 755 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    ret_chpl3 = tmp_chpl;
#line 755 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    ret_chpl = ret_chpl3;
#line 755 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    goto _end_chpl_localeID_to_locale_chpl;
#line 755 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  }
#line 755 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _end_chpl_localeID_to_locale_chpl:;
#line 743 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  *(_retArg_chpl) = ret_chpl;
#line 743 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  return;
#line 743 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
}

#line 771 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
/* ChapelLocale.chpl:771 */
#line 771 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
void chpl_taskRunningCntInc(int64_t _ln,
#line 771 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
                            int32_t _fn) {
#line 773 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  locale_chpl call_tmp_chpl6;
#line 773 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  chpl_localeID_t call_tmp_chpl7;
#line 773 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  int32_t call_tmp_chpl8;
#line 773 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  chpl_localeID_t call_tmp_chpl9;
#line 773 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  locale_chpl call_tmp_chpl10;
#line 773 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  locale_chpl ret_tmp_chpl;
#line 92 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  BaseLocale_chpl coerce_tmp_chpl3 = NULL;
#line 773 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  BaseLocale_chpl coerce_tmp_chpl4 = NULL;
#line 773 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _ref_AtomicT_int64_t_chpl call_tmp_chpl11 = NULL;
#line 773 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _ref_atomic_int_least64_t_chpl call_tmp_chpl12 = NULL;
#line 773 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  memory_order ret_chpl;
#line 773 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _ref_locale_chpl i_x_chpl = NULL;
#line 772 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  if (rootLocaleInitialized_chpl) /* ZLINE: 772 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl */
#line 772 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  {
#line 773 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    call_tmp_chpl8 = get_chpl_nodeID();
#line 773 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    call_tmp_chpl9 = chpl_rt_buildLocaleID(call_tmp_chpl8, c_sublocid_any);
#line 773 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    call_tmp_chpl7 = call_tmp_chpl9;
#line 773 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    chpl_localeID_to_locale(&call_tmp_chpl7, &ret_tmp_chpl, _ln, _fn);
#line 773 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    call_tmp_chpl10 = ret_tmp_chpl;
#line 773 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    call_tmp_chpl6 = call_tmp_chpl10;
#line 92 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    coerce_tmp_chpl3 = (&call_tmp_chpl6)->_instance;
#line 92 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    if (((RootClass_chpl)(coerce_tmp_chpl3)) == nil) /* ZLINE: 92 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl */
#line 92 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    {
#line 92 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      nilCheckHalt_chpl(&_str_literal_52_chpl, _ln, _fn);
#line 92 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    }
#line 773 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    coerce_tmp_chpl4 = coerce_tmp_chpl3;
#line 773 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    chpl_check_nil(coerce_tmp_chpl4, _ln, _fn);
#line 773 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    call_tmp_chpl11 = &((coerce_tmp_chpl4)->runningTaskCounter);
#line 773 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    chpl_rmem_consist_maybe_release3(_ln, _fn);
#line 773 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    call_tmp_chpl12 = &((call_tmp_chpl11)->_v);
#line 773 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    ret_chpl = memory_order_relaxed;
#line 773 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    atomic_fetch_add_explicit_int_least64_t(call_tmp_chpl12, INT64(1), ret_chpl);
#line 773 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    chpl_rmem_consist_maybe_acquire3(_ln, _fn);
#line 773 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    i_x_chpl = &call_tmp_chpl6;
#line 773 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    deinit_chpl62(i_x_chpl);
#line 773 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  }
#line 771 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  return;
#line 771 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
}

#line 780 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
/* ChapelLocale.chpl:780 */
#line 780 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
void chpl_taskRunningCntDec(int64_t _ln,
#line 780 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
                            int32_t _fn) {
#line 782 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  locale_chpl call_tmp_chpl6;
#line 782 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  chpl_localeID_t call_tmp_chpl7;
#line 782 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  int32_t call_tmp_chpl8;
#line 782 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  chpl_localeID_t call_tmp_chpl9;
#line 782 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  locale_chpl call_tmp_chpl10;
#line 782 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  locale_chpl ret_tmp_chpl;
#line 92 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  BaseLocale_chpl coerce_tmp_chpl3 = NULL;
#line 782 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  BaseLocale_chpl coerce_tmp_chpl4 = NULL;
#line 782 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _ref_AtomicT_int64_t_chpl call_tmp_chpl11 = NULL;
#line 782 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _ref_atomic_int_least64_t_chpl call_tmp_chpl12 = NULL;
#line 782 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  memory_order ret_chpl;
#line 782 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _ref_locale_chpl i_x_chpl = NULL;
#line 781 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  if (rootLocaleInitialized_chpl) /* ZLINE: 781 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl */
#line 781 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  {
#line 782 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    call_tmp_chpl8 = get_chpl_nodeID();
#line 782 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    call_tmp_chpl9 = chpl_rt_buildLocaleID(call_tmp_chpl8, c_sublocid_any);
#line 782 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    call_tmp_chpl7 = call_tmp_chpl9;
#line 782 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    chpl_localeID_to_locale(&call_tmp_chpl7, &ret_tmp_chpl, _ln, _fn);
#line 782 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    call_tmp_chpl10 = ret_tmp_chpl;
#line 782 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    call_tmp_chpl6 = call_tmp_chpl10;
#line 92 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    coerce_tmp_chpl3 = (&call_tmp_chpl6)->_instance;
#line 92 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    if (((RootClass_chpl)(coerce_tmp_chpl3)) == nil) /* ZLINE: 92 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl */
#line 92 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    {
#line 92 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
      nilCheckHalt_chpl(&_str_literal_52_chpl, _ln, _fn);
#line 92 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    }
#line 782 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    coerce_tmp_chpl4 = coerce_tmp_chpl3;
#line 782 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    chpl_check_nil(coerce_tmp_chpl4, _ln, _fn);
#line 782 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    call_tmp_chpl11 = &((coerce_tmp_chpl4)->runningTaskCounter);
#line 782 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    chpl_rmem_consist_maybe_release3(_ln, _fn);
#line 782 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    call_tmp_chpl12 = &((call_tmp_chpl11)->_v);
#line 782 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    ret_chpl = memory_order_relaxed;
#line 782 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    atomic_fetch_sub_explicit_int_least64_t(call_tmp_chpl12, INT64(1), ret_chpl);
#line 782 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    chpl_rmem_consist_maybe_acquire3(_ln, _fn);
#line 782 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    i_x_chpl = &call_tmp_chpl6;
#line 782 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    deinit_chpl62(i_x_chpl);
#line 782 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  }
#line 780 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  return;
#line 780 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
}

#line 788 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
/* ChapelLocale.chpl:788 */
#line 788 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
void chpl_taskRunningCntReset(int64_t _ln,
#line 788 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
                              int32_t _fn) {
#line 789 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  locale_chpl call_tmp_chpl6;
#line 789 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  chpl_localeID_t call_tmp_chpl7;
#line 789 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  int32_t call_tmp_chpl8;
#line 789 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  chpl_localeID_t call_tmp_chpl9;
#line 789 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  locale_chpl call_tmp_chpl10;
#line 789 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  locale_chpl ret_tmp_chpl;
#line 92 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  BaseLocale_chpl coerce_tmp_chpl3 = NULL;
#line 789 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  BaseLocale_chpl coerce_tmp_chpl4 = NULL;
#line 789 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _ref_AtomicT_int64_t_chpl call_tmp_chpl11 = NULL;
#line 789 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _ref_atomic_int_least64_t_chpl call_tmp_chpl12 = NULL;
#line 789 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  memory_order ret_chpl;
#line 788 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _ref_locale_chpl i_x_chpl = NULL;
#line 789 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  call_tmp_chpl8 = get_chpl_nodeID();
#line 789 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  call_tmp_chpl9 = chpl_rt_buildLocaleID(call_tmp_chpl8, c_sublocid_any);
#line 789 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  call_tmp_chpl7 = call_tmp_chpl9;
#line 789 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  chpl_localeID_to_locale(&call_tmp_chpl7, &ret_tmp_chpl, _ln, _fn);
#line 789 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  call_tmp_chpl10 = ret_tmp_chpl;
#line 789 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  call_tmp_chpl6 = call_tmp_chpl10;
#line 92 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  coerce_tmp_chpl3 = (&call_tmp_chpl6)->_instance;
#line 92 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  if (((RootClass_chpl)(coerce_tmp_chpl3)) == nil) /* ZLINE: 92 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl */
#line 92 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  {
#line 92 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    nilCheckHalt_chpl(&_str_literal_52_chpl, _ln, _fn);
#line 92 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  }
#line 789 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  coerce_tmp_chpl4 = coerce_tmp_chpl3;
#line 789 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  chpl_check_nil(coerce_tmp_chpl4, _ln, _fn);
#line 789 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  call_tmp_chpl11 = &((coerce_tmp_chpl4)->runningTaskCounter);
#line 789 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  chpl_rmem_consist_maybe_release3(_ln, _fn);
#line 789 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  call_tmp_chpl12 = &((call_tmp_chpl11)->_v);
#line 789 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  ret_chpl = memory_order_relaxed;
#line 789 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  atomic_store_explicit_int_least64_t(call_tmp_chpl12, INT64(0), ret_chpl);
#line 789 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  chpl_rmem_consist_maybe_acquire3(_ln, _fn);
#line 788 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  i_x_chpl = &call_tmp_chpl6;
#line 788 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  deinit_chpl62(i_x_chpl);
#line 788 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  return;
#line 788 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
}

#line 793 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
/* ChapelLocale.chpl:793 */
#line 793 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
static void deinit_chpl63(int64_t _ln_chpl,
#line 793 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
                          int32_t _fn_chpl) {
#line 551 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _ref_locale_chpl i_x_chpl = NULL;
#line 536 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _ref_locale_chpl i_x_chpl2 = NULL;
#line 79 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _ref_locale_chpl i_x_chpl3 = NULL;
#line 73 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _ref_locale_chpl i_x_chpl4 = NULL;
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  _ref_locale_chpl i_x_chpl5 = NULL;
#line 794 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  chpl__delete28((&origRootLocale_chpl)->_instance, _ln_chpl, _fn_chpl);
#line 795 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  chpl__delete28((&dummyLocale_chpl)->_instance, _ln_chpl, _fn_chpl);
#line 551 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  i_x_chpl = &origRootLocale_chpl;
#line 551 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  deinit_chpl62(i_x_chpl);
#line 536 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  i_x_chpl2 = &rootLocale_chpl;
#line 536 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  deinit_chpl62(i_x_chpl2);
#line 79 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  i_x_chpl3 = &dummyLocale_chpl;
#line 79 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  deinit_chpl62(i_x_chpl3);
#line 73 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  i_x_chpl4 = &defaultLocale_chpl;
#line 73 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  deinit_chpl62(i_x_chpl4);
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  i_x_chpl5 = &nilLocale_chpl;
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  deinit_chpl62(i_x_chpl5);
#line 793 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  return;
#line 793 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
}

