#line 31 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
/* LocaleModelHelpSetup.chpl:31 */
#line 31 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
static void chpl__init_LocaleModelHelpSetup(int64_t _ln_chpl,
#line 31 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
                                            int32_t _fn_chpl) {
#line 31 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  _ref_int32_t_chpl refIndentLevel_chpl = NULL;
#line 31 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  if (chpl__init_LocaleModelHelpSetup_p) /* ZLINE: 31 /Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl */
#line 31 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  {
#line 31 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
    goto _exit_chpl__init_LocaleModelHelpSetup_chpl;
#line 31 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  }
#line 31 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  printModuleInit_chpl("%*s\n", "LocaleModelHelpSetup", INT64(20), _ln_chpl, _fn_chpl);
#line 31 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  refIndentLevel_chpl = &moduleInitLevel_chpl;
#line 31 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  *(refIndentLevel_chpl) += INT64(1);
#line 31 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  chpl__init_LocaleModelHelpSetup_p = UINT8(true);
#line 31 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  {
#line 31 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
    chpl__init_ChapelLocale(_ln_chpl, _fn_chpl);
#line 31 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
    chpl__init_DefaultRectangular(_ln_chpl, _fn_chpl);
#line 31 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
    chpl__init_ChapelNumLocales(_ln_chpl, _fn_chpl);
#line 31 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
    chpl__init_POSIX(_ln_chpl, _fn_chpl);
#line 31 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
    chpl__init_CTypes(_ln_chpl, _fn_chpl);
#line 31 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
    chpl__init_String(_ln_chpl, _fn_chpl);
#line 31 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  }
#line 31 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  *(refIndentLevel_chpl) -= INT64(1);
#line 31 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  _exit_chpl__init_LocaleModelHelpSetup_chpl:;
#line 31 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  return;
#line 31 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
}

#line 55 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
/* LocaleModelHelpSetup.chpl:55 */
#line 55 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
static void init_chpl161(chpl_root_locale_accum * this_chpl31) {
#line 55 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  AtomicT_int64_t_chpl nPUsPhysAcc_chpl;
#line 55 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  AtomicT_int64_t_chpl nPUsPhysAll_chpl;
#line 55 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  AtomicT_int64_t_chpl nPUsLogAcc_chpl;
#line 55 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  AtomicT_int64_t_chpl nPUsLogAll_chpl;
#line 55 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  AtomicT_int64_t_chpl maxTaskPar_chpl;
#line 55 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  init_chpl18(&nPUsPhysAcc_chpl);
#line 55 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  (this_chpl31)->nPUsPhysAcc = nPUsPhysAcc_chpl;
#line 55 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  init_chpl18(&nPUsPhysAll_chpl);
#line 55 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  (this_chpl31)->nPUsPhysAll = nPUsPhysAll_chpl;
#line 55 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  init_chpl18(&nPUsLogAcc_chpl);
#line 55 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  (this_chpl31)->nPUsLogAcc = nPUsLogAcc_chpl;
#line 55 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  init_chpl18(&nPUsLogAll_chpl);
#line 55 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  (this_chpl31)->nPUsLogAll = nPUsLogAll_chpl;
#line 55 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  init_chpl18(&maxTaskPar_chpl);
#line 55 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  (this_chpl31)->maxTaskPar = maxTaskPar_chpl;
#line 55 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  return;
#line 55 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
}

#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
/* LocaleModelHelpSetup.chpl:66 */
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
static void accum_chpl(chpl_root_locale_accum * this_chpl31,
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
                       locale_chpl * loc_chpl,
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
                       int64_t _ln_chpl,
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
                       int32_t _fn_chpl) {
#line 67 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  _ref_AtomicT_int64_t_chpl call_tmp_chpl6 = NULL;
#line 92 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  BaseLocale_chpl coerce_tmp_chpl3 = NULL;
#line 67 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  BaseLocale_chpl coerce_tmp_chpl4 = NULL;
#line 67 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  int64_t coerce_tmp_chpl5;
#line 67 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  _ref_atomic_int_least64_t_chpl call_tmp_chpl7 = NULL;
#line 67 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  memory_order ret_chpl;
#line 68 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  _ref_AtomicT_int64_t_chpl call_tmp_chpl8 = NULL;
#line 92 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  BaseLocale_chpl coerce_tmp_chpl6 = NULL;
#line 68 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  BaseLocale_chpl coerce_tmp_chpl7 = NULL;
#line 68 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  int64_t coerce_tmp_chpl8;
#line 68 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  _ref_atomic_int_least64_t_chpl call_tmp_chpl9 = NULL;
#line 68 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  memory_order ret_chpl2;
#line 69 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  _ref_AtomicT_int64_t_chpl call_tmp_chpl10 = NULL;
#line 92 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  BaseLocale_chpl coerce_tmp_chpl9 = NULL;
#line 69 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  BaseLocale_chpl coerce_tmp_chpl10 = NULL;
#line 69 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  int64_t coerce_tmp_chpl11;
#line 69 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  _ref_atomic_int_least64_t_chpl call_tmp_chpl11 = NULL;
#line 69 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  memory_order ret_chpl3;
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  _ref_AtomicT_int64_t_chpl call_tmp_chpl12 = NULL;
#line 92 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  BaseLocale_chpl coerce_tmp_chpl12 = NULL;
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  BaseLocale_chpl coerce_tmp_chpl13 = NULL;
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  int64_t coerce_tmp_chpl14;
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  _ref_atomic_int_least64_t_chpl call_tmp_chpl13 = NULL;
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  memory_order ret_chpl4;
#line 71 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  _ref_AtomicT_int64_t_chpl call_tmp_chpl14 = NULL;
#line 71 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  int64_t ret_chpl5;
#line 71 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  BaseLocale_chpl coerce_tmp_chpl15 = NULL;
#line 71 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  BaseLocale_chpl coerce_tmp_chpl16 = NULL;
#line 71 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  _ref_atomic_int_least64_t_chpl call_tmp_chpl15 = NULL;
#line 71 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  memory_order ret_chpl6;
#line 67 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  call_tmp_chpl6 = &((this_chpl31)->nPUsPhysAcc);
#line 92 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  coerce_tmp_chpl3 = (loc_chpl)->_instance;
#line 92 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  if (((RootClass_chpl)(coerce_tmp_chpl3)) == nil) /* ZLINE: 92 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl */
#line 92 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  {
#line 92 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    nilCheckHalt_chpl(&_str_literal_52_chpl, _ln_chpl, _fn_chpl);
#line 92 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  }
#line 67 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  coerce_tmp_chpl4 = coerce_tmp_chpl3;
#line 67 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  chpl_check_nil(coerce_tmp_chpl4, _ln_chpl, _fn_chpl);
#line 67 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  coerce_tmp_chpl5 = (coerce_tmp_chpl4)->nPUsPhysAcc;
#line 67 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  chpl_rmem_consist_maybe_release2(_ln_chpl, _fn_chpl);
#line 67 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  call_tmp_chpl7 = &((call_tmp_chpl6)->_v);
#line 67 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  ret_chpl = memory_order_seq_cst;
#line 67 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  atomic_fetch_add_explicit_int_least64_t(call_tmp_chpl7, coerce_tmp_chpl5, ret_chpl);
#line 67 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  chpl_rmem_consist_maybe_acquire2(_ln_chpl, _fn_chpl);
#line 68 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  call_tmp_chpl8 = &((this_chpl31)->nPUsPhysAll);
#line 92 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  coerce_tmp_chpl6 = (loc_chpl)->_instance;
#line 92 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  if (((RootClass_chpl)(coerce_tmp_chpl6)) == nil) /* ZLINE: 92 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl */
#line 92 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  {
#line 92 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    nilCheckHalt_chpl(&_str_literal_52_chpl, _ln_chpl, _fn_chpl);
#line 92 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  }
#line 68 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  coerce_tmp_chpl7 = coerce_tmp_chpl6;
#line 68 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  chpl_check_nil(coerce_tmp_chpl7, _ln_chpl, _fn_chpl);
#line 68 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  coerce_tmp_chpl8 = (coerce_tmp_chpl7)->nPUsPhysAll;
#line 68 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  chpl_rmem_consist_maybe_release2(_ln_chpl, _fn_chpl);
#line 68 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  call_tmp_chpl9 = &((call_tmp_chpl8)->_v);
#line 68 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  ret_chpl2 = memory_order_seq_cst;
#line 68 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  atomic_fetch_add_explicit_int_least64_t(call_tmp_chpl9, coerce_tmp_chpl8, ret_chpl2);
#line 68 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  chpl_rmem_consist_maybe_acquire2(_ln_chpl, _fn_chpl);
#line 69 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  call_tmp_chpl10 = &((this_chpl31)->nPUsLogAcc);
#line 92 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  coerce_tmp_chpl9 = (loc_chpl)->_instance;
#line 92 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  if (((RootClass_chpl)(coerce_tmp_chpl9)) == nil) /* ZLINE: 92 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl */
#line 92 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  {
#line 92 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    nilCheckHalt_chpl(&_str_literal_52_chpl, _ln_chpl, _fn_chpl);
#line 92 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  }
#line 69 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  coerce_tmp_chpl10 = coerce_tmp_chpl9;
#line 69 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  chpl_check_nil(coerce_tmp_chpl10, _ln_chpl, _fn_chpl);
#line 69 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  coerce_tmp_chpl11 = (coerce_tmp_chpl10)->nPUsLogAcc;
#line 69 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  chpl_rmem_consist_maybe_release2(_ln_chpl, _fn_chpl);
#line 69 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  call_tmp_chpl11 = &((call_tmp_chpl10)->_v);
#line 69 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  ret_chpl3 = memory_order_seq_cst;
#line 69 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  atomic_fetch_add_explicit_int_least64_t(call_tmp_chpl11, coerce_tmp_chpl11, ret_chpl3);
#line 69 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  chpl_rmem_consist_maybe_acquire2(_ln_chpl, _fn_chpl);
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  call_tmp_chpl12 = &((this_chpl31)->nPUsLogAll);
#line 92 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  coerce_tmp_chpl12 = (loc_chpl)->_instance;
#line 92 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  if (((RootClass_chpl)(coerce_tmp_chpl12)) == nil) /* ZLINE: 92 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl */
#line 92 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  {
#line 92 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
    nilCheckHalt_chpl(&_str_literal_52_chpl, _ln_chpl, _fn_chpl);
#line 92 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocale.chpl"
  }
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  coerce_tmp_chpl13 = coerce_tmp_chpl12;
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  chpl_check_nil(coerce_tmp_chpl13, _ln_chpl, _fn_chpl);
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  coerce_tmp_chpl14 = (coerce_tmp_chpl13)->nPUsLogAll;
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  chpl_rmem_consist_maybe_release2(_ln_chpl, _fn_chpl);
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  call_tmp_chpl13 = &((call_tmp_chpl12)->_v);
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  ret_chpl4 = memory_order_seq_cst;
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  atomic_fetch_add_explicit_int_least64_t(call_tmp_chpl13, coerce_tmp_chpl14, ret_chpl4);
#line 70 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  chpl_rmem_consist_maybe_acquire2(_ln_chpl, _fn_chpl);
#line 71 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  call_tmp_chpl14 = &((this_chpl31)->maxTaskPar);
#line 71 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  coerce_tmp_chpl15 = (loc_chpl)->_instance;
#line 71 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  if (((RootClass_chpl)(coerce_tmp_chpl15)) == nil) /* ZLINE: 71 /Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl */
#line 71 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  {
#line 71 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
    nilCheckHalt_chpl(&_str_literal_52_chpl, _ln_chpl, _fn_chpl);
#line 71 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  }
#line 71 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  coerce_tmp_chpl16 = coerce_tmp_chpl15;
#line 71 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  chpl_check_nil(coerce_tmp_chpl16, _ln_chpl, _fn_chpl);
#line 71 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  ret_chpl5 = (coerce_tmp_chpl16)->maxTaskPar;
#line 71 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  chpl_rmem_consist_maybe_release2(_ln_chpl, _fn_chpl);
#line 71 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  call_tmp_chpl15 = &((call_tmp_chpl14)->_v);
#line 71 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  ret_chpl6 = memory_order_seq_cst;
#line 71 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  atomic_fetch_add_explicit_int_least64_t(call_tmp_chpl15, ret_chpl5, ret_chpl6);
#line 71 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  chpl_rmem_consist_maybe_acquire2(_ln_chpl, _fn_chpl);
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  return;
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
}

#line 73 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
/* LocaleModelHelpSetup.chpl:73 */
#line 73 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
static void setRootLocaleValues_chpl(chpl_root_locale_accum * this_chpl31,
#line 73 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
                                     RootLocale_chpl dst_chpl,
#line 73 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
                                     int64_t _ln_chpl,
#line 73 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
                                     int32_t _fn_chpl) {
#line 74 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  BaseLocale_chpl coerce_tmp_chpl3 = NULL;
#line 74 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  _ref_AtomicT_int64_t_chpl call_tmp_chpl6 = NULL;
#line 74 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  _ref_atomic_int_least64_t_chpl call_tmp_chpl7 = NULL;
#line 74 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  memory_order ret_chpl;
#line 74 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  int64_t call_tmp_chpl8;
#line 75 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  BaseLocale_chpl coerce_tmp_chpl4 = NULL;
#line 75 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  _ref_AtomicT_int64_t_chpl call_tmp_chpl9 = NULL;
#line 75 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  _ref_atomic_int_least64_t_chpl call_tmp_chpl10 = NULL;
#line 75 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  memory_order ret_chpl2;
#line 75 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  int64_t call_tmp_chpl11;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  BaseLocale_chpl coerce_tmp_chpl5 = NULL;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  _ref_AtomicT_int64_t_chpl call_tmp_chpl12 = NULL;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  _ref_atomic_int_least64_t_chpl call_tmp_chpl13 = NULL;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  memory_order ret_chpl3;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  int64_t call_tmp_chpl14;
#line 77 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  BaseLocale_chpl coerce_tmp_chpl6 = NULL;
#line 77 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  _ref_AtomicT_int64_t_chpl call_tmp_chpl15 = NULL;
#line 77 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  _ref_atomic_int_least64_t_chpl call_tmp_chpl16 = NULL;
#line 77 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  memory_order ret_chpl4;
#line 77 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  int64_t call_tmp_chpl17;
#line 78 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  BaseLocale_chpl coerce_tmp_chpl7 = NULL;
#line 78 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  _ref_AtomicT_int64_t_chpl call_tmp_chpl18 = NULL;
#line 78 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  _ref_atomic_int_least64_t_chpl call_tmp_chpl19 = NULL;
#line 78 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  memory_order ret_chpl5;
#line 78 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  int64_t call_tmp_chpl20;
#line 74 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  coerce_tmp_chpl3 = ((BaseLocale_chpl)(dst_chpl));
#line 74 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  call_tmp_chpl6 = &((this_chpl31)->nPUsPhysAcc);
#line 74 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  chpl_rmem_consist_maybe_release2(_ln_chpl, _fn_chpl);
#line 74 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  call_tmp_chpl7 = &((call_tmp_chpl6)->_v);
#line 74 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  ret_chpl = memory_order_seq_cst;
#line 74 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  call_tmp_chpl8 = atomic_load_explicit_int_least64_t(call_tmp_chpl7, ret_chpl);
#line 74 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  chpl_check_nil(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 74 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  (coerce_tmp_chpl3)->nPUsPhysAcc = call_tmp_chpl8;
#line 75 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  coerce_tmp_chpl4 = ((BaseLocale_chpl)(dst_chpl));
#line 75 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  call_tmp_chpl9 = &((this_chpl31)->nPUsPhysAll);
#line 75 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  chpl_rmem_consist_maybe_release2(_ln_chpl, _fn_chpl);
#line 75 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  call_tmp_chpl10 = &((call_tmp_chpl9)->_v);
#line 75 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  ret_chpl2 = memory_order_seq_cst;
#line 75 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  call_tmp_chpl11 = atomic_load_explicit_int_least64_t(call_tmp_chpl10, ret_chpl2);
#line 75 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  chpl_check_nil(coerce_tmp_chpl4, _ln_chpl, _fn_chpl);
#line 75 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  (coerce_tmp_chpl4)->nPUsPhysAll = call_tmp_chpl11;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  coerce_tmp_chpl5 = ((BaseLocale_chpl)(dst_chpl));
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  call_tmp_chpl12 = &((this_chpl31)->nPUsLogAcc);
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  chpl_rmem_consist_maybe_release2(_ln_chpl, _fn_chpl);
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  call_tmp_chpl13 = &((call_tmp_chpl12)->_v);
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  ret_chpl3 = memory_order_seq_cst;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  call_tmp_chpl14 = atomic_load_explicit_int_least64_t(call_tmp_chpl13, ret_chpl3);
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  chpl_check_nil(coerce_tmp_chpl5, _ln_chpl, _fn_chpl);
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  (coerce_tmp_chpl5)->nPUsLogAcc = call_tmp_chpl14;
#line 77 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  coerce_tmp_chpl6 = ((BaseLocale_chpl)(dst_chpl));
#line 77 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  call_tmp_chpl15 = &((this_chpl31)->nPUsLogAll);
#line 77 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  chpl_rmem_consist_maybe_release2(_ln_chpl, _fn_chpl);
#line 77 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  call_tmp_chpl16 = &((call_tmp_chpl15)->_v);
#line 77 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  ret_chpl4 = memory_order_seq_cst;
#line 77 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  call_tmp_chpl17 = atomic_load_explicit_int_least64_t(call_tmp_chpl16, ret_chpl4);
#line 77 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  chpl_check_nil(coerce_tmp_chpl6, _ln_chpl, _fn_chpl);
#line 77 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  (coerce_tmp_chpl6)->nPUsLogAll = call_tmp_chpl17;
#line 78 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  coerce_tmp_chpl7 = ((BaseLocale_chpl)(dst_chpl));
#line 78 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  call_tmp_chpl18 = &((this_chpl31)->maxTaskPar);
#line 78 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  chpl_rmem_consist_maybe_release2(_ln_chpl, _fn_chpl);
#line 78 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  call_tmp_chpl19 = &((call_tmp_chpl18)->_v);
#line 78 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  ret_chpl5 = memory_order_seq_cst;
#line 78 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  call_tmp_chpl20 = atomic_load_explicit_int_least64_t(call_tmp_chpl19, ret_chpl5);
#line 78 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  chpl_check_nil(coerce_tmp_chpl7, _ln_chpl, _fn_chpl);
#line 78 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  (coerce_tmp_chpl7)->maxTaskPar = call_tmp_chpl20;
#line 73 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  return;
#line 73 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
}

#line 82 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
/* LocaleModelHelpSetup.chpl:82 */
#line 82 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
static void helpSetupRootLocaleFlat_chpl(RootLocale_chpl dst_chpl,
#line 82 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
                                         int64_t _ln_chpl,
#line 82 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
                                         int32_t _fn_chpl) {
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  int64_t local_numLocales_chpl;
#line 83 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  chpl_root_locale_accum root_accum_chpl;
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  RootLocale_chpl dst_chpl2 = NULL;
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  range_int64_t_both_one_chpl tmpIter_chpl;
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  range_int64_t_low_one_chpl call_tmp_chpl6;
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  range_int64_t_low_one_chpl ret_tmp_chpl;
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl2;
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  chpl___EndCount_AtomicT_int64_t_int64_t _coforallCount_chpl = NULL;
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  chpl___EndCount_AtomicT_int64_t_int64_t new_temp_chpl = NULL;
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  int64_t numTasks_chpl;
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  range_int64_t_both_one_chpl _ic__F0_this_chpl;
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  int64_t i_chpl;
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  range_int64_t_both_one_chpl this_chpl31;
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  range_int64_t_both_one_chpl this_chpl32;
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  int64_t end_chpl;
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  _class_localscoforall_fn_chpl98 _args_forcoforall_fn_chpl = NULL;
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  _ref_chpl_root_locale_accum_chpl tmp_chpl = NULL;
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  raw_c_void_ptr _args_vforcoforall_fn_chpl;
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  int64_t _args_sizecoforall_fn_chpl;
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  Error_chpl error_chpl = NULL;
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  range_int64_t_both_one_chpl tmpIter_chpl2;
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  range_int64_t_low_one_chpl call_tmp_chpl7;
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  range_int64_t_low_one_chpl ret_tmp_chpl3;
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl4;
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  chpl___EndCount_AtomicT_int64_t_int64_t _coforallCount_chpl2 = NULL;
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  chpl___EndCount_AtomicT_int64_t_int64_t new_temp_chpl2 = NULL;
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  int64_t numTasks_chpl2;
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  range_int64_t_both_one_chpl _ic__F0_this_chpl2;
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  int64_t i_chpl2;
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  range_int64_t_both_one_chpl this_chpl33;
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  range_int64_t_both_one_chpl this_chpl34;
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  int64_t end_chpl2;
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  _class_localscoforall_fn_chpl97 _args_forcoforall_fn_chpl2 = NULL;
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  raw_c_void_ptr _args_vforcoforall_fn_chpl2;
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  int64_t _args_sizecoforall_fn_chpl2;
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  Error_chpl error_chpl2 = NULL;
#line 82 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  _ref_AtomicT_int64_t_chpl _field_destructor_tmp__chpl = NULL;
#line 82 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  _ref_AtomicT_int64_t_chpl _field_destructor_tmp__chpl2 = NULL;
#line 82 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  _ref_AtomicT_int64_t_chpl _field_destructor_tmp__chpl3 = NULL;
#line 82 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  _ref_AtomicT_int64_t_chpl _field_destructor_tmp__chpl4 = NULL;
#line 82 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  _ref_AtomicT_int64_t_chpl _field_destructor_tmp__chpl5 = NULL;
#line 82 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  chpl__class_localscoforall_fn_chpl98_object chpl_macro_tmp_99;
#line 82 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  chpl__class_localscoforall_fn_chpl97_object chpl_macro_tmp_100;
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  local_numLocales_chpl = numLocales_chpl;
#line 83 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  init_chpl161(&root_accum_chpl);
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  dst_chpl2 = dst_chpl;
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  chpl_build_low_bounded_range2(INT64(0), &ret_tmp_chpl);
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  call_tmp_chpl6 = ret_tmp_chpl;
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  chpl___POUND_3(call_tmp_chpl6, local_numLocales_chpl, &ret_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  tmpIter_chpl = ret_tmp_chpl2;
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  new_temp_chpl = _new_chpl17(_ln_chpl, _fn_chpl);
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  _coforallCount_chpl = new_temp_chpl;
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  numTasks_chpl = chpl_boundedCoforallSize(tmpIter_chpl, _ln_chpl, _fn_chpl);
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  _upEndCount_chpl(_coforallCount_chpl, numTasks_chpl, _ln_chpl, _fn_chpl);
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  chpl_resetTaskSpawn(numTasks_chpl, _ln_chpl, _fn_chpl);
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  _ic__F0_this_chpl = tmpIter_chpl;
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  checkIfIterWillOverflow_chpl(tmpIter_chpl, UINT8(true), _ln_chpl, _fn_chpl);
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  i_chpl = INT64(0);
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  this_chpl31 = _ic__F0_this_chpl;
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  this_chpl32 = _ic__F0_this_chpl;
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  end_chpl = (&this_chpl32)->_high;
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  for (i_chpl = (&this_chpl31)->_low; ((i_chpl <= end_chpl)); i_chpl += INT64(1)) {
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
    _args_forcoforall_fn_chpl = ((_class_localscoforall_fn_chpl98)(&chpl_macro_tmp_99));
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl, _ln_chpl, _fn_chpl);
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
    (_args_forcoforall_fn_chpl)->_0__coforallCount = _coforallCount_chpl;
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl, _ln_chpl, _fn_chpl);
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
    (_args_forcoforall_fn_chpl)->_1_locIdx = i_chpl;
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl, _ln_chpl, _fn_chpl);
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
    (_args_forcoforall_fn_chpl)->_2_dst = dst_chpl2;
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
    tmp_chpl = &root_accum_chpl;
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl, _ln_chpl, _fn_chpl);
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
    (_args_forcoforall_fn_chpl)->_3_root_accum = tmp_chpl;
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
    _args_vforcoforall_fn_chpl = ((void*)(_args_forcoforall_fn_chpl));
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
    _args_sizecoforall_fn_chpl = sizeof(chpl__class_localscoforall_fn_chpl98_object);
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
    (_args_forcoforall_fn_chpl)->_ln = _ln_chpl;
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
    (_args_forcoforall_fn_chpl)->_fn = _fn_chpl;
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
    /*** wrapcoforall_fn_chpl98 ***/ chpl_taskAddCoStmt(chpl_task_getRequestedSubloc(), INT64(97), ((chpl_task_bundle_p)(_args_vforcoforall_fn_chpl)), _args_sizecoforall_fn_chpl, INT64(85), INT32(61));
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  }
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  error_chpl = NULL;
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  _waitEndCount_chpl2(_coforallCount_chpl, numTasks_chpl, &error_chpl, _ln_chpl, _fn_chpl);
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  if (error_chpl != nil) /* ZLINE: 85 /Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl */
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  {
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
    chpl_propagate_error(error_chpl, _ln_chpl, _fn_chpl);
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  }
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  chpl__delete72(_coforallCount_chpl, _ln_chpl, _fn_chpl);
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  chpl_build_low_bounded_range2(INT64(0), &ret_tmp_chpl3);
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  call_tmp_chpl7 = ret_tmp_chpl3;
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  chpl___POUND_3(call_tmp_chpl7, local_numLocales_chpl, &ret_tmp_chpl4, _ln_chpl, _fn_chpl);
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  tmpIter_chpl2 = ret_tmp_chpl4;
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  new_temp_chpl2 = _new_chpl17(_ln_chpl, _fn_chpl);
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  _coforallCount_chpl2 = new_temp_chpl2;
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  numTasks_chpl2 = chpl_boundedCoforallSize(tmpIter_chpl2, _ln_chpl, _fn_chpl);
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  _upEndCount_chpl(_coforallCount_chpl2, numTasks_chpl2, _ln_chpl, _fn_chpl);
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  chpl_resetTaskSpawn(numTasks_chpl2, _ln_chpl, _fn_chpl);
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  _ic__F0_this_chpl2 = tmpIter_chpl2;
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  checkIfIterWillOverflow_chpl(tmpIter_chpl2, UINT8(true), _ln_chpl, _fn_chpl);
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  i_chpl2 = INT64(0);
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  this_chpl33 = _ic__F0_this_chpl2;
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  this_chpl34 = _ic__F0_this_chpl2;
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  end_chpl2 = (&this_chpl34)->_high;
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  for (i_chpl2 = (&this_chpl33)->_low; ((i_chpl2 <= end_chpl2)); i_chpl2 += INT64(1)) {
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
    _args_forcoforall_fn_chpl2 = ((_class_localscoforall_fn_chpl97)(&chpl_macro_tmp_100));
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl2, _ln_chpl, _fn_chpl);
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
    (_args_forcoforall_fn_chpl2)->_0__coforallCount = _coforallCount_chpl2;
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl2, _ln_chpl, _fn_chpl);
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
    (_args_forcoforall_fn_chpl2)->_1_locIdx = i_chpl2;
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
    chpl_check_nil(_args_forcoforall_fn_chpl2, _ln_chpl, _fn_chpl);
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
    (_args_forcoforall_fn_chpl2)->_2_dst = dst_chpl2;
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
    _args_vforcoforall_fn_chpl2 = ((void*)(_args_forcoforall_fn_chpl2));
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
    _args_sizecoforall_fn_chpl2 = sizeof(chpl__class_localscoforall_fn_chpl97_object);
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
    (_args_forcoforall_fn_chpl2)->_ln = _ln_chpl;
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
    (_args_forcoforall_fn_chpl2)->_fn = _fn_chpl;
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
    /*** wrapcoforall_fn_chpl97 ***/ chpl_taskAddCoStmt(chpl_task_getRequestedSubloc(), INT64(96), ((chpl_task_bundle_p)(_args_vforcoforall_fn_chpl2)), _args_sizecoforall_fn_chpl2, INT64(85), INT32(61));
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  }
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  error_chpl2 = NULL;
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  _waitEndCount_chpl2(_coforallCount_chpl2, numTasks_chpl2, &error_chpl2, _ln_chpl, _fn_chpl);
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  if (error_chpl2 != nil) /* ZLINE: 85 /Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl */
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  {
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
    chpl_propagate_error(error_chpl2, _ln_chpl, _fn_chpl);
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  }
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  chpl__delete72(_coforallCount_chpl2, _ln_chpl, _fn_chpl);
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  chpl_after_forall_fence();
#line 91 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  setRootLocaleValues_chpl(&root_accum_chpl, dst_chpl, _ln_chpl, _fn_chpl);
#line 82 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  _field_destructor_tmp__chpl = &((&root_accum_chpl)->maxTaskPar);
#line 82 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  deinit_chpl2(_field_destructor_tmp__chpl);
#line 82 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  _field_destructor_tmp__chpl2 = &((&root_accum_chpl)->nPUsLogAll);
#line 82 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  deinit_chpl2(_field_destructor_tmp__chpl2);
#line 82 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  _field_destructor_tmp__chpl3 = &((&root_accum_chpl)->nPUsLogAcc);
#line 82 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  deinit_chpl2(_field_destructor_tmp__chpl3);
#line 82 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  _field_destructor_tmp__chpl4 = &((&root_accum_chpl)->nPUsPhysAll);
#line 82 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  deinit_chpl2(_field_destructor_tmp__chpl4);
#line 82 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  _field_destructor_tmp__chpl5 = &((&root_accum_chpl)->nPUsPhysAcc);
#line 82 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  deinit_chpl2(_field_destructor_tmp__chpl5);
#line 82 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  return;
#line 82 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
}

#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
/* LocaleModelHelpSetup.chpl:85 */
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
static void coforall_fn_chpl97(chpl___EndCount_AtomicT_int64_t_int64_t _coforallCount_chpl,
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
                               int64_t locIdx_chpl,
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
                               RootLocale_chpl dst_chpl,
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
                               chpl_root_locale_accum * root_accum_chpl,
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
                               int64_t _ln_chpl,
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
                               int32_t _fn_chpl) {
#line 86 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  locale_chpl call_tmp_chpl6;
#line 86 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  locale_chpl initTemp_chpl;
#line 86 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  LocaleModel_chpl new_temp_chpl = NULL;
#line 86 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  locale_chpl call_tmp_chpl7;
#line 86 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  locale_chpl initTemp_chpl2;
#line 86 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  locale_chpl node_chpl;
#line 86 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  _ref_locale_chpl i_x_chpl = NULL;
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  _ref__array_DefaultRectangularArr_1_int64_t_one_locale_int64_t_chpl call_tmp_chpl8 = NULL;
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  _tuple_1_star_int64_t_chpl coerce_tmp_chpl3;
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  _ref__tuple_1_star_int64_t_chpl i_i_chpl = NULL;
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  DefaultRectangularArr_1_int64_t_one_locale_int64_t_chpl ret_chpl = NULL;
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  DefaultRectangularArr_1_int64_t_one_locale_int64_t_chpl coerce_tmp_chpl4 = NULL;
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  _tuple_1_star_int64_t_chpl _formal_tmp_in_ind_chpl;
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  int64_t _formal_tmp_ind_x0_chpl;
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  _ddata_locale_chpl coerce_tmp_chpl5 = NULL;
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  _ref_locale_chpl call_tmp_chpl9 = NULL;
#line 88 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  _ref_locale_chpl i_x_chpl2 = NULL;
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  chpl_buildLocaleID(((int32_t)(locIdx_chpl)), c_sublocid_any, _ln_chpl, _fn_chpl);
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  chpl_defaultDistInitPrivate(_ln_chpl, _fn_chpl);
#line 86 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  init_chpl100(&initTemp_chpl, ((BaseLocale_chpl)(dst_chpl)));
#line 86 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  call_tmp_chpl6 = initTemp_chpl;
#line 86 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  new_temp_chpl = _new_chpl54(&call_tmp_chpl6, _ln_chpl, _fn_chpl);
#line 86 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  init_chpl101(&initTemp_chpl2, ((BaseLocale_chpl)(new_temp_chpl)));
#line 86 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  call_tmp_chpl7 = initTemp_chpl2;
#line 86 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  node_chpl = call_tmp_chpl7;
#line 86 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  i_x_chpl = &call_tmp_chpl6;
#line 86 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  deinit_chpl62(i_x_chpl);
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  chpl_check_nil(dst_chpl, _ln_chpl, _fn_chpl);
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  call_tmp_chpl8 = &((dst_chpl)->myLocales);
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  *(coerce_tmp_chpl3 + INT64(0)) = locIdx_chpl;
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  i_i_chpl = &coerce_tmp_chpl3;
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  ret_chpl = (call_tmp_chpl8)->_instance;
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  checkAccess_chpl7(call_tmp_chpl8, i_i_chpl, ret_chpl, _ln_chpl, _fn_chpl);
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  coerce_tmp_chpl4 = ret_chpl;
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  *(_formal_tmp_in_ind_chpl + INT64(0)) = *(coerce_tmp_chpl3 + INT64(0));
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  _formal_tmp_ind_x0_chpl = *(_formal_tmp_in_ind_chpl + INT64(0));
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  chpl_check_nil(coerce_tmp_chpl4, _ln_chpl, _fn_chpl);
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  coerce_tmp_chpl5 = (coerce_tmp_chpl4)->shiftedData;
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  call_tmp_chpl9 = (coerce_tmp_chpl5 + _formal_tmp_ind_x0_chpl);
#line 87 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  chpl___ASSIGN_9(call_tmp_chpl9, &node_chpl);
#line 88 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  accum_chpl(root_accum_chpl, &node_chpl, _ln_chpl, _fn_chpl);
#line 88 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  i_x_chpl2 = &node_chpl;
#line 88 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  deinit_chpl62(i_x_chpl2);
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  return;
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
}

#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
/* LocaleModelHelpSetup.chpl:85 */
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
static void coforall_fn_chpl98(chpl___EndCount_AtomicT_int64_t_int64_t _coforallCount_chpl,
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
                               int64_t locIdx_chpl,
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
                               RootLocale_chpl dst_chpl,
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
                               int64_t _ln_chpl,
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
                               int32_t _fn_chpl) {
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  chpl_buildLocaleID(((int32_t)(locIdx_chpl)), c_sublocid_any, _ln_chpl, _fn_chpl);
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  chpl_rootLocaleInitPrivate(locIdx_chpl, _ln_chpl, _fn_chpl);
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  chpl_defaultLocaleInitPrivate(_ln_chpl, _fn_chpl);
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  chpl_singletonCurrentLocaleInitPrivate(locIdx_chpl, _ln_chpl, _fn_chpl);
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  warmupRuntime_chpl(_ln_chpl, _fn_chpl);
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  return;
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
}

#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
/* LocaleModelHelpSetup.chpl:85 */
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
static void wrapcoforall_fn_chpl98(_class_localscoforall_fn_chpl98 c_chpl) {
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  int32_t _fn_chpl;
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  int64_t _ln_chpl;
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  chpl___EndCount_AtomicT_int64_t_int64_t _0__coforallCount_chpl = NULL;
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  int64_t _1_locIdx_chpl;
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  RootLocale_chpl _2_dst_chpl = NULL;
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  _ref_chpl_root_locale_accum_chpl _3_root_accum_chpl = NULL;
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  _fn_chpl = (c_chpl)->_fn;
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  _ln_chpl = (c_chpl)->_ln;
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  chpl_rmem_consist_acquire(_ln_chpl, _fn_chpl);
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  chpl_check_nil(c_chpl, _ln_chpl, _fn_chpl);
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  _0__coforallCount_chpl = (c_chpl)->_0__coforallCount;
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  chpl_check_nil(c_chpl, _ln_chpl, _fn_chpl);
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  _1_locIdx_chpl = (c_chpl)->_1_locIdx;
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  chpl_check_nil(c_chpl, _ln_chpl, _fn_chpl);
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  _2_dst_chpl = (c_chpl)->_2_dst;
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  chpl_check_nil(c_chpl, _ln_chpl, _fn_chpl);
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  _3_root_accum_chpl = (c_chpl)->_3_root_accum;
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  coforall_fn_chpl97(_0__coforallCount_chpl, _1_locIdx_chpl, _2_dst_chpl, _3_root_accum_chpl, _ln_chpl, _fn_chpl);
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  chpl_check_nil(c_chpl, _ln_chpl, _fn_chpl);
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  _downEndCount_chpl((c_chpl)->_0__coforallCount, nil, _ln_chpl, _fn_chpl);
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  return;
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
}

#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
/* LocaleModelHelpSetup.chpl:85 */
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
static void wrapcoforall_fn_chpl97(_class_localscoforall_fn_chpl97 c_chpl) {
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  int32_t _fn_chpl;
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  int64_t _ln_chpl;
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  chpl___EndCount_AtomicT_int64_t_int64_t _0__coforallCount_chpl = NULL;
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  int64_t _1_locIdx_chpl;
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  _fn_chpl = (c_chpl)->_fn;
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  _ln_chpl = (c_chpl)->_ln;
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  chpl_rmem_consist_acquire(_ln_chpl, _fn_chpl);
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  chpl_check_nil(c_chpl, _ln_chpl, _fn_chpl);
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  _0__coforallCount_chpl = (c_chpl)->_0__coforallCount;
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  chpl_check_nil(c_chpl, _ln_chpl, _fn_chpl);
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  _1_locIdx_chpl = (c_chpl)->_1_locIdx;
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  chpl_check_nil(c_chpl, _ln_chpl, _fn_chpl);
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  coforall_fn_chpl98(_0__coforallCount_chpl, _1_locIdx_chpl, (c_chpl)->_2_dst, _ln_chpl, _fn_chpl);
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  chpl_check_nil(c_chpl, _ln_chpl, _fn_chpl);
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  _downEndCount_chpl((c_chpl)->_0__coforallCount, nil, _ln_chpl, _fn_chpl);
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  return;
#line 85 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
}

#line 169 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
/* LocaleModelHelpSetup.chpl:169 */
#line 169 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
static void helpSetupLocaleFlat_chpl(LocaleModel_chpl dst_chpl,
#line 169 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
                                     string_chpl * local_name_chpl,
#line 169 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
                                     int64_t _ln_chpl,
#line 169 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
                                     int32_t _fn_chpl) {
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  string_chpl _node_name_chpl;
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  Error_chpl error_chpl = NULL;
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  c_ptrConst_int8_t_chpl call_tmp_chpl6 = NULL;
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  int64_t call_tmp_chpl7;
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  _owned_IllegalArgumentError_chpl2 call_tmp_chpl8;
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  _owned_IllegalArgumentError_chpl2 ret_tmp_chpl;
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  _ref__owned_IllegalArgumentError_chpl i_arg_chpl = NULL;
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  IllegalArgumentError_chpl call_tmp_chpl9 = NULL;
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  string_chpl call_tmp_chpl10;
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  Error_chpl coerce_tmp_chpl3 = NULL;
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  string_chpl ret_tmp_chpl2;
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  int64_t call_tmp_chpl11;
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  string_chpl ret_chpl;
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  string_chpl call_tmp_chpl12;
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  Error_chpl error_chpl2 = NULL;
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  string_chpl ret_tmp_chpl3;
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  int32_t call_tmp_chpl13;
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  string_chpl call_tmp_chpl14;
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  string_chpl ret_tmp_chpl4;
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  string_chpl _node_id_chpl;
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  _ref_string_chpl _fold_tmp_chpl = NULL;
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  string_chpl ret_chpl2;
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  string_chpl tmp_chpl;
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  _ref_string_chpl i_x_chpl2 = NULL;
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  _ref_string_chpl i_x_chpl3 = NULL;
#line 173 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  BaseLocale_chpl coerce_tmp_chpl4 = NULL;
#line 173 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  int32_t call_tmp_chpl15;
#line 173 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  int64_t coerce_tmp_chpl5;
#line 174 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  BaseLocale_chpl coerce_tmp_chpl6 = NULL;
#line 174 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  int32_t call_tmp_chpl16;
#line 174 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  int64_t coerce_tmp_chpl7;
#line 177 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  BaseLocale_chpl coerce_tmp_chpl8 = NULL;
#line 177 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  int32_t call_tmp_chpl17;
#line 177 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  int64_t coerce_tmp_chpl9;
#line 178 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  BaseLocale_chpl coerce_tmp_chpl10 = NULL;
#line 178 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  int32_t call_tmp_chpl18;
#line 178 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  int64_t coerce_tmp_chpl11;
#line 181 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  BaseLocale_chpl coerce_tmp_chpl12 = NULL;
#line 181 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  uint32_t call_tmp_chpl19;
#line 181 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  int64_t coerce_tmp_chpl13;
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  error_chpl = NULL;
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  call_tmp_chpl6 = chpl_nodeName();
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  call_tmp_chpl7 = string_length_bytes(call_tmp_chpl6);
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  chpl_checkValue2(call_tmp_chpl7, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  call_tmp_chpl8 = ret_tmp_chpl;
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  i_arg_chpl = &call_tmp_chpl8;
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  call_tmp_chpl9 = borrow_chpl2(i_arg_chpl);
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  if (((RootClass_chpl)(call_tmp_chpl9)) != nil) /* ZLINE: 170 /Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl */
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  {
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
    coerce_tmp_chpl3 = ((Error_chpl)(call_tmp_chpl9));
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
    chpl_check_nil(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
    ((void(*)(Error_chpl,
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
       string_chpl *,
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
       int64_t,
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
       int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(coerce_tmp_chpl3))->chpl__cid) + INT64(1))])(coerce_tmp_chpl3, &ret_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
    call_tmp_chpl10 = ret_tmp_chpl2;
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
    safeCastCheckHalt_chpl(&call_tmp_chpl10, _ln_chpl, _fn_chpl);
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
    i_x_chpl = &call_tmp_chpl10;
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
    deinit_chpl95(i_x_chpl, _ln_chpl, _fn_chpl);
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  }
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  chpl__autoDestroy62(&call_tmp_chpl8, _ln_chpl, _fn_chpl);
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  call_tmp_chpl11 = call_tmp_chpl7;
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  if (error_chpl != nil) /* ZLINE: 170 /Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl */
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  {
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
    goto handler_chpl;
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  }
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  error_chpl2 = NULL;
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  createCopyingBuffer_chpl(((c_ptr_uint8_t_chpl)(call_tmp_chpl6)), call_tmp_chpl11, ((int64_t)((call_tmp_chpl11 + INT64(1)))), decodePolicy_chpl_strict_chpl, &error_chpl2, &ret_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  call_tmp_chpl12 = ret_tmp_chpl3;
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  if (error_chpl2 != nil) /* ZLINE: 170 /Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl */
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  {
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
    error_chpl = error_chpl2;
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
    goto _endcreateCopyingBuffer_chpl;
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  }
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  ret_chpl = call_tmp_chpl12;
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  _endcreateCopyingBuffer_chpl:;
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  if (error_chpl != nil) /* ZLINE: 170 /Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl */
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  {
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
    goto handler_chpl;
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  }
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  _node_name_chpl = ret_chpl;
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  handler_chpl:;
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  if (error_chpl != nil) /* ZLINE: 170 /Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl */
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  {
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
    chpl_uncaught_error(error_chpl, _ln_chpl, _fn_chpl);
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  }
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  call_tmp_chpl13 = get_chpl_nodeID();
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  chpl___COLON_25(((int64_t)(call_tmp_chpl13)), &ret_tmp_chpl4, _ln_chpl, _fn_chpl);
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  call_tmp_chpl14 = ret_tmp_chpl4;
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  _node_id_chpl = call_tmp_chpl14;
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  _fold_tmp_chpl = &_node_name_chpl;
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  init_ASSIGN__chpl25(&tmp_chpl, _fold_tmp_chpl, _ln_chpl, _fn_chpl);
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  ret_chpl2 = tmp_chpl;
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  i_x_chpl2 = &_node_id_chpl;
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  deinit_chpl95(i_x_chpl2, _ln_chpl, _fn_chpl);
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  i_x_chpl3 = &_node_name_chpl;
#line 170 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  deinit_chpl95(i_x_chpl3, _ln_chpl, _fn_chpl);
#line 173 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  coerce_tmp_chpl4 = ((BaseLocale_chpl)(dst_chpl));
#line 173 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  call_tmp_chpl15 = chpl_topo_getNumCPUsPhysical(UINT8(true));
#line 173 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  coerce_tmp_chpl5 = ((int64_t)(call_tmp_chpl15));
#line 173 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  chpl_check_nil(coerce_tmp_chpl4, _ln_chpl, _fn_chpl);
#line 173 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  (coerce_tmp_chpl4)->nPUsPhysAcc = coerce_tmp_chpl5;
#line 174 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  coerce_tmp_chpl6 = ((BaseLocale_chpl)(dst_chpl));
#line 174 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  call_tmp_chpl16 = chpl_topo_getNumCPUsPhysical(UINT8(false));
#line 174 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  coerce_tmp_chpl7 = ((int64_t)(call_tmp_chpl16));
#line 174 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  chpl_check_nil(coerce_tmp_chpl6, _ln_chpl, _fn_chpl);
#line 174 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  (coerce_tmp_chpl6)->nPUsPhysAll = coerce_tmp_chpl7;
#line 177 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  coerce_tmp_chpl8 = ((BaseLocale_chpl)(dst_chpl));
#line 177 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  call_tmp_chpl17 = chpl_topo_getNumCPUsLogical(UINT8(true));
#line 177 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  coerce_tmp_chpl9 = ((int64_t)(call_tmp_chpl17));
#line 177 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  chpl_check_nil(coerce_tmp_chpl8, _ln_chpl, _fn_chpl);
#line 177 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  (coerce_tmp_chpl8)->nPUsLogAcc = coerce_tmp_chpl9;
#line 178 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  coerce_tmp_chpl10 = ((BaseLocale_chpl)(dst_chpl));
#line 178 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  call_tmp_chpl18 = chpl_topo_getNumCPUsLogical(UINT8(false));
#line 178 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  coerce_tmp_chpl11 = ((int64_t)(call_tmp_chpl18));
#line 178 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  chpl_check_nil(coerce_tmp_chpl10, _ln_chpl, _fn_chpl);
#line 178 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  (coerce_tmp_chpl10)->nPUsLogAll = coerce_tmp_chpl11;
#line 181 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  coerce_tmp_chpl12 = ((BaseLocale_chpl)(dst_chpl));
#line 181 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  call_tmp_chpl19 = chpl_task_getMaxPar();
#line 181 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  coerce_tmp_chpl13 = ((int64_t)(call_tmp_chpl19));
#line 181 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  chpl_check_nil(coerce_tmp_chpl12, _ln_chpl, _fn_chpl);
#line 181 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  (coerce_tmp_chpl12)->maxTaskPar = coerce_tmp_chpl13;
#line 169 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  *(local_name_chpl) = ret_chpl2;
#line 169 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
  return;
#line 169 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocaleModelHelpSetup.chpl"
}

