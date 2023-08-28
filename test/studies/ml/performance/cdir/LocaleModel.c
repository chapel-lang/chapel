#line 30 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
/* LocaleModel.chpl:30 */
#line 30 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
static void chpl__init_LocaleModel(int64_t _ln_chpl,
#line 30 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
                                   int32_t _fn_chpl) {
#line 30 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  _ref_int32_t_chpl refIndentLevel_chpl = NULL;
#line 55 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl6;
#line 55 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl;
#line 55 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  _domain_DefaultRectangularDom_1_int64_t_one_chpl call_tmp_chpl7;
#line 55 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  _domain_DefaultRectangularDom_1_int64_t_one_chpl ret_tmp_chpl2;
#line 55 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  chpl___RuntimeTypeInfo4 call_tmp_chpl8;
#line 55 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  _domain_DefaultRectangularDom_1_int64_t_one_chpl init_coerce_tmp_chpl;
#line 55 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  _domain_DefaultRectangularDom_1_int64_t_one_chpl ret_tmp_chpl3;
#line 57 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  chpl___RuntimeTypeInfo19 call_tmp_chpl9;
#line 57 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  _domain_DefaultRectangularDom_1_int64_t_one_chpl _runtime_type_tmp__chpl;
#line 57 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  _array_DefaultRectangularArr_1_int64_t_one_locale_int64_t_chpl ret_tmp_chpl4;
#line 30 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  if (chpl__init_LocaleModel_p) /* ZLINE: 30 /Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl */
#line 30 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  {
#line 30 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
    goto _exit_chpl__init_LocaleModel_chpl;
#line 30 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  }
#line 30 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  printModuleInit_chpl("%*s\n", "LocaleModel", INT64(11), _ln_chpl, _fn_chpl);
#line 30 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  refIndentLevel_chpl = &moduleInitLevel_chpl;
#line 30 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  *(refIndentLevel_chpl) += INT64(1);
#line 30 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  chpl__init_LocaleModel_p = UINT8(true);
#line 30 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  {
#line 30 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
    chpl__init_LocaleModelHelpFlat(_ln_chpl, _fn_chpl);
#line 30 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
    chpl__init_LocaleModelHelpMem(_ln_chpl, _fn_chpl);
#line 30 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
    chpl__init_IO(_ln_chpl, _fn_chpl);
#line 30 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  }
#line 30 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  chpl_addModule("LocaleModel", ((c_fn_ptr)(deinit_chpl65)), _ln_chpl, _fn_chpl);
#line 55 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  chpl_build_bounded_range5(&ret_tmp_chpl);
#line 55 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  call_tmp_chpl6 = ret_tmp_chpl;
#line 55 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  chpl__buildDomainExpr3(call_tmp_chpl6, UINT8(true), &ret_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 55 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  call_tmp_chpl7 = ret_tmp_chpl2;
#line 55 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  call_tmp_chpl8 = chpl__buildDomainRuntimeType2(&defaultDist_chpl);
#line 55 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  chpl__coerceMove4(&call_tmp_chpl8, &call_tmp_chpl7, UINT8(true), &ret_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 55 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  init_coerce_tmp_chpl = ret_tmp_chpl3;
#line 55 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  chpl_emptyLocaleSpace = init_coerce_tmp_chpl;
#line 57 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  call_tmp_chpl = chpl__ensureDomainExpr2(&chpl_emptyLocaleSpace);
#line 57 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  call_tmp_chpl9 = chpl__buildArrayRuntimeType6(call_tmp_chpl);
#line 57 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  _runtime_type_tmp__chpl = (&call_tmp_chpl9)->dom;
#line 57 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  chpl__convertRuntimeTypeToValue7(&_runtime_type_tmp__chpl, UINT8(true), &ret_tmp_chpl4, _ln_chpl, _fn_chpl);
#line 57 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  chpl_emptyLocales = ret_tmp_chpl4;
#line 30 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  *(refIndentLevel_chpl) -= INT64(1);
#line 30 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  _exit_chpl__init_LocaleModel_chpl:;
#line 30 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  return;
#line 30 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
}

#line 43 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
/* LocaleModel.chpl:43 */
#line 43 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
int32_t chpl_localeModel_sublocToExecutionSubloc(int32_t full_subloc) {
#line 43 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  return full_subloc;
#line 43 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
}

#line 49 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
/* LocaleModel.chpl:49 */
#line 49 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
int32_t chpl_localeModel_sublocMerge(int32_t full_subloc,
#line 49 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
                                     int32_t execution_subloc) {
#line 49 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  return execution_subloc;
#line 49 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
}

#line 62 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
/* LocaleModel.chpl:62 */
#line 62 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
static void chpl__auto_destroy_LocaleModel(LocaleModel_chpl this_chpl31,
#line 62 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
                                           int64_t _ln_chpl,
#line 62 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
                                           int32_t _fn_chpl) {
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  _ref_string_chpl _field_destructor_tmp__chpl = NULL;
#line 62 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  AbstractLocaleModel_chpl _parent_destructor_tmp__chpl = NULL;
#line 62 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  _array_DefaultRectangularArr_1_int64_t_one_locale_int64_t_chpl _field_destructor_tmp__chpl2;
#line 62 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  BaseLocale_chpl _parent_destructor_tmp__chpl2 = NULL;
#line 62 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  _ref_AtomicT_int64_t_chpl _field_destructor_tmp__chpl3 = NULL;
#line 62 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  _ref_locale_chpl _field_destructor_tmp__chpl4 = NULL;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  _field_destructor_tmp__chpl = &((this_chpl31)->local_name);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  deinit_chpl95(_field_destructor_tmp__chpl, _ln_chpl, _fn_chpl);
#line 62 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  _parent_destructor_tmp__chpl = ((AbstractLocaleModel_chpl)(this_chpl31));
#line 62 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  chpl_check_nil(_parent_destructor_tmp__chpl, _ln_chpl, _fn_chpl);
#line 62 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  _field_destructor_tmp__chpl2 = (_parent_destructor_tmp__chpl)->chpl_singletonThisLocaleArray;
#line 62 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  chpl__autoDestroy30(&_field_destructor_tmp__chpl2, _ln_chpl, _fn_chpl);
#line 62 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  _parent_destructor_tmp__chpl2 = ((BaseLocale_chpl)(_parent_destructor_tmp__chpl));
#line 62 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  chpl_check_nil(_parent_destructor_tmp__chpl2, _ln_chpl, _fn_chpl);
#line 62 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  _field_destructor_tmp__chpl3 = &((_parent_destructor_tmp__chpl2)->runningTaskCounter);
#line 62 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  deinit_chpl2(_field_destructor_tmp__chpl3);
#line 62 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  chpl_check_nil(_parent_destructor_tmp__chpl2, _ln_chpl, _fn_chpl);
#line 62 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  _field_destructor_tmp__chpl4 = &((_parent_destructor_tmp__chpl2)->parent);
#line 62 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  deinit_chpl62(_field_destructor_tmp__chpl4);
#line 62 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  return;
#line 62 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
}

#line 81 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
/* LocaleModel.chpl:81 */
#line 81 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
static LocaleModel_chpl _new_chpl54(locale_chpl * parent_loc_chpl,
#line 81 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
                                    int64_t _ln_chpl,
#line 81 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
                                    int32_t _fn_chpl) {
#line 81 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  LocaleModel_chpl initTemp_chpl = NULL;
#line 81 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  c_ptr_void_chpl cast_tmp_chpl = NULL;
#line 81 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  AbstractLocaleModel_chpl super_tmp_chpl = NULL;
#line 81 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  BaseLocale_chpl super_tmp_chpl2 = NULL;
#line 81 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  locale_chpl parent_chpl;
#line 81 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  AtomicT_int64_t_chpl runningTaskCounter_chpl;
#line 81 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  _array_DefaultRectangularArr_1_int64_t_one_locale_int64_t_chpl chpl_singletonThisLocaleArray;
#line 81 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl6;
#line 81 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl;
#line 81 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  _domain_DefaultRectangularDom_1_int64_t_one_chpl call_tmp_chpl7;
#line 81 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  _domain_DefaultRectangularDom_1_int64_t_one_chpl ret_tmp_chpl2;
#line 81 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  chpl___RuntimeTypeInfo19 call_tmp_chpl8;
#line 81 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  _domain_DefaultRectangularDom_1_int64_t_one_chpl _runtime_type_tmp__chpl;
#line 81 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  _array_DefaultRectangularArr_1_int64_t_one_locale_int64_t_chpl ret_tmp_chpl3;
#line 81 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  int32_t call_tmp_chpl9;
#line 81 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  int64_t call_tmp_chpl10;
#line 81 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  string_chpl local_name_chpl;
#line 81 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  LocaleModel_chpl tmp_chpl = NULL;
#line 81 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  cast_tmp_chpl = chpl_here_alloc(sizeof(chpl_LocaleModel_chpl_object), INT16(54), _ln_chpl, _fn_chpl);
#line 81 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  initTemp_chpl = ((LocaleModel_chpl)(cast_tmp_chpl));
#line 81 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  ((RootClass_chpl)(initTemp_chpl))->chpl__cid = chpl__cid_LocaleModel_chpl;
#line 81 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  if (rootLocaleInitialized_chpl) /* ZLINE: 81 /Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl */
#line 81 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  {
#line 81 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
    halt_chpl(&_str_literal_683_chpl, _ln_chpl, _fn_chpl);
#line 81 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  }
#line 81 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  chpl_check_nil(initTemp_chpl, _ln_chpl, _fn_chpl);
#line 81 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  super_tmp_chpl = &((initTemp_chpl)->super);
#line 81 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 81 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  super_tmp_chpl2 = &((super_tmp_chpl)->super);
#line 81 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  chpl_check_nil(super_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 81 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  ((RootClass_chpl)(&((super_tmp_chpl2)->super)))->chpl__cid = chpl__cid_RootClass_chpl;
#line 81 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  ((RootClass_chpl)(((RootClass_chpl)(super_tmp_chpl2))))->chpl__cid = chpl__cid_RootClass_chpl;
#line 81 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  init_ASSIGN__chpl(&parent_chpl, parent_loc_chpl);
#line 81 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  chpl_check_nil(super_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 81 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  (super_tmp_chpl2)->parent = parent_chpl;
#line 81 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  chpl_check_nil(super_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 81 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  (super_tmp_chpl2)->nPUsLogAcc = INT64(0);
#line 81 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  chpl_check_nil(super_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 81 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  (super_tmp_chpl2)->nPUsLogAll = INT64(0);
#line 81 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  chpl_check_nil(super_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 81 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  (super_tmp_chpl2)->nPUsPhysAcc = INT64(0);
#line 81 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  chpl_check_nil(super_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 81 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  (super_tmp_chpl2)->nPUsPhysAll = INT64(0);
#line 81 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  chpl_check_nil(super_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 81 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  (super_tmp_chpl2)->maxTaskPar = INT64(0);
#line 81 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  init_chpl18(&runningTaskCounter_chpl);
#line 81 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  chpl_check_nil(super_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 81 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  (super_tmp_chpl2)->runningTaskCounter = runningTaskCounter_chpl;
#line 81 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  ((RootClass_chpl)(super_tmp_chpl2))->chpl__cid = chpl__cid_BaseLocale_chpl;
#line 81 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  ((RootClass_chpl)(((BaseLocale_chpl)(super_tmp_chpl))))->chpl__cid = chpl__cid_BaseLocale_chpl;
#line 81 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  chpl_build_bounded_range2(&ret_tmp_chpl);
#line 81 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  call_tmp_chpl6 = ret_tmp_chpl;
#line 81 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  chpl__ensureDomainExpr5(call_tmp_chpl6, &ret_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 81 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  call_tmp_chpl7 = ret_tmp_chpl2;
#line 81 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  call_tmp_chpl8 = chpl__buildArrayRuntimeType6(&call_tmp_chpl7);
#line 81 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  _runtime_type_tmp__chpl = (&call_tmp_chpl8)->dom;
#line 81 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  chpl__convertRuntimeTypeToValue7(&_runtime_type_tmp__chpl, UINT8(false), &ret_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 81 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  chpl_singletonThisLocaleArray = ret_tmp_chpl3;
#line 81 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 81 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  (super_tmp_chpl)->chpl_singletonThisLocaleArray = chpl_singletonThisLocaleArray;
#line 81 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  ((RootClass_chpl)(super_tmp_chpl))->chpl__cid = chpl__cid_AbstractLocaleModel_chpl;
#line 81 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  chpl__autoDestroy11(&call_tmp_chpl7, _ln_chpl, _fn_chpl);
#line 81 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  ((RootClass_chpl)(((AbstractLocaleModel_chpl)(initTemp_chpl))))->chpl__cid = chpl__cid_AbstractLocaleModel_chpl;
#line 81 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  call_tmp_chpl9 = get_chpl_nodeID();
#line 81 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  call_tmp_chpl10 = ((int64_t)(call_tmp_chpl9));
#line 81 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  chpl_check_nil(initTemp_chpl, _ln_chpl, _fn_chpl);
#line 81 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  (initTemp_chpl)->_node_id = call_tmp_chpl10;
#line 81 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  init_chpl194(&local_name_chpl, _ln_chpl, _fn_chpl);
#line 81 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  chpl_check_nil(initTemp_chpl, _ln_chpl, _fn_chpl);
#line 81 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  (initTemp_chpl)->local_name = local_name_chpl;
#line 81 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  ((RootClass_chpl)(initTemp_chpl))->chpl__cid = chpl__cid_LocaleModel_chpl;
#line 81 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  chpl_check_nil(initTemp_chpl, _ln_chpl, _fn_chpl);
#line 81 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  setup_chpl(initTemp_chpl, _ln_chpl, _fn_chpl);
#line 81 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  tmp_chpl = initTemp_chpl;
#line 81 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  return tmp_chpl;
#line 81 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
}

#line 105 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
/* LocaleModel.chpl:105 */
#line 105 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
static int64_t _getChildCount_chpl3(LocaleModel_chpl this_chpl31,
#line 105 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
                                    int64_t _ln_chpl,
#line 105 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
                                    int32_t _fn_chpl) {
#line 105 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  return INT64(0);
#line 105 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
}

#line 113 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
/* LocaleModel.chpl:113 */
#line 113 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
static void _getChild_chpl3(LocaleModel_chpl this_chpl31,
#line 113 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
                            int64_t idx_chpl,
#line 113 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
                            locale_chpl * _retArg_chpl,
#line 113 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
                            int64_t _ln_chpl,
#line 113 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
                            int32_t _fn_chpl) {
#line 113 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  locale_chpl ret_chpl;
#line 115 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  locale_chpl tmp_chpl;
#line 114 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  halt_chpl(&_str_literal_684_chpl, _ln_chpl, _fn_chpl);
#line 115 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  init_chpl99(&tmp_chpl, _ln_chpl, _fn_chpl);
#line 116 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  ret_chpl = tmp_chpl;
#line 113 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  *(_retArg_chpl) = ret_chpl;
#line 113 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  return;
#line 113 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
}

#line 131 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
/* LocaleModel.chpl:131 */
#line 131 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
static void setup_chpl(LocaleModel_chpl this_chpl31,
#line 131 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
                       int64_t _ln_chpl,
#line 131 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
                       int32_t _fn_chpl) {
#line 132 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  _ref_string_chpl call_tmp_chpl6 = NULL;
#line 132 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  string_chpl _formal_tmp_out_local_name_chpl;
#line 131 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 132 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 132 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  call_tmp_chpl6 = &((this_chpl31)->local_name);
#line 132 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  helpSetupLocaleFlat_chpl(this_chpl31, &_formal_tmp_out_local_name_chpl, _ln_chpl, _fn_chpl);
#line 132 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  chpl___ASSIGN_16(call_tmp_chpl6, &_formal_tmp_out_local_name_chpl, _ln_chpl, _fn_chpl);
#line 131 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  i_x_chpl = &_formal_tmp_out_local_name_chpl;
#line 131 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  deinit_chpl95(i_x_chpl, _ln_chpl, _fn_chpl);
#line 131 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  return;
#line 131 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
}

#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
/* LocaleModel.chpl:150 */
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
static RootLocale_chpl _new_chpl55(int64_t _ln_chpl,
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
                                   int32_t _fn_chpl) {
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  int64_t local_numLocales_chpl;
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  RootLocale_chpl initTemp_chpl = NULL;
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  c_ptr_void_chpl cast_tmp_chpl = NULL;
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  AbstractRootLocale_chpl super_tmp_chpl = NULL;
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  _ref_locale_chpl i_parent_loc_chpl = NULL;
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  BaseLocale_chpl super_tmp_chpl2 = NULL;
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  locale_chpl parent_chpl;
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  AtomicT_int64_t_chpl runningTaskCounter_chpl;
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl6;
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl;
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  _domain_DefaultRectangularDom_1_int64_t_one_chpl call_tmp_chpl7;
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  _domain_DefaultRectangularDom_1_int64_t_one_chpl ret_tmp_chpl2;
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  chpl___RuntimeTypeInfo4 call_tmp_chpl8;
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  _domain_DefaultRectangularDom_1_int64_t_one_chpl init_coerce_tmp_chpl;
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  _domain_DefaultRectangularDom_1_int64_t_one_chpl ret_tmp_chpl3;
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  _domain_DefaultRectangularDom_1_int64_t_one_chpl myLocaleSpace_chpl;
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  _ref_DefaultRectangularDom_1_int64_t_one_chpl _instance_chpl = NULL;
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  _array_DefaultRectangularArr_1_int64_t_one_locale_int64_t_chpl myLocales_chpl;
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  _ref__domain_DefaultRectangularDom_1_int64_t_one_chpl call_tmp_chpl9 = NULL;
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  _ref__domain_DefaultRectangularDom_1_int64_t_one_chpl call_tmp_chpl10 = NULL;
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  chpl___RuntimeTypeInfo19 call_tmp_chpl11;
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  _domain_DefaultRectangularDom_1_int64_t_one_chpl _runtime_type_tmp__chpl;
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  _array_DefaultRectangularArr_1_int64_t_one_locale_int64_t_chpl ret_tmp_chpl4;
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  local_numLocales_chpl = numLocales_chpl;
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  cast_tmp_chpl = chpl_here_alloc(sizeof(chpl_RootLocale_chpl_object), INT16(49), _ln_chpl, _fn_chpl);
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  initTemp_chpl = ((RootLocale_chpl)(cast_tmp_chpl));
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  ((RootClass_chpl)(initTemp_chpl))->chpl__cid = chpl__cid_RootLocale_chpl;
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  chpl_check_nil(initTemp_chpl, _ln_chpl, _fn_chpl);
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  super_tmp_chpl = &((initTemp_chpl)->super);
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  i_parent_loc_chpl = &nilLocale_chpl;
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  super_tmp_chpl2 = &((super_tmp_chpl)->super);
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  chpl_check_nil(super_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  ((RootClass_chpl)(&((super_tmp_chpl2)->super)))->chpl__cid = chpl__cid_RootClass_chpl;
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  ((RootClass_chpl)(((RootClass_chpl)(super_tmp_chpl2))))->chpl__cid = chpl__cid_RootClass_chpl;
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  init_ASSIGN__chpl(&parent_chpl, i_parent_loc_chpl);
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  chpl_check_nil(super_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  (super_tmp_chpl2)->parent = parent_chpl;
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  chpl_check_nil(super_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  (super_tmp_chpl2)->nPUsLogAcc = INT64(0);
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  chpl_check_nil(super_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  (super_tmp_chpl2)->nPUsLogAll = INT64(0);
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  chpl_check_nil(super_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  (super_tmp_chpl2)->nPUsPhysAcc = INT64(0);
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  chpl_check_nil(super_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  (super_tmp_chpl2)->nPUsPhysAll = INT64(0);
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  chpl_check_nil(super_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  (super_tmp_chpl2)->maxTaskPar = INT64(0);
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  init_chpl18(&runningTaskCounter_chpl);
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  chpl_check_nil(super_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  (super_tmp_chpl2)->runningTaskCounter = runningTaskCounter_chpl;
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  ((RootClass_chpl)(super_tmp_chpl2))->chpl__cid = chpl__cid_BaseLocale_chpl;
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  ((RootClass_chpl)(((BaseLocale_chpl)(super_tmp_chpl))))->chpl__cid = chpl__cid_BaseLocale_chpl;
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  ((RootClass_chpl)(super_tmp_chpl))->chpl__cid = chpl__cid_AbstractRootLocale_chpl;
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  ((RootClass_chpl)(((AbstractRootLocale_chpl)(initTemp_chpl))))->chpl__cid = chpl__cid_AbstractRootLocale_chpl;
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  chpl_check_nil(initTemp_chpl, _ln_chpl, _fn_chpl);
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  ((BaseLocale_chpl)(initTemp_chpl))->nPUsPhysAcc = INT64(0);
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  chpl_check_nil(initTemp_chpl, _ln_chpl, _fn_chpl);
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  ((BaseLocale_chpl)(initTemp_chpl))->nPUsPhysAll = INT64(0);
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  chpl_check_nil(initTemp_chpl, _ln_chpl, _fn_chpl);
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  ((BaseLocale_chpl)(initTemp_chpl))->nPUsLogAcc = INT64(0);
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  chpl_check_nil(initTemp_chpl, _ln_chpl, _fn_chpl);
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  ((BaseLocale_chpl)(initTemp_chpl))->nPUsLogAll = INT64(0);
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  chpl_check_nil(initTemp_chpl, _ln_chpl, _fn_chpl);
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  ((BaseLocale_chpl)(initTemp_chpl))->maxTaskPar = INT64(0);
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  chpl_build_bounded_range6(INT64(0), ((int64_t)((local_numLocales_chpl - INT64(1)))), &ret_tmp_chpl);
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  call_tmp_chpl6 = ret_tmp_chpl;
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  chpl__buildDomainExpr3(call_tmp_chpl6, UINT8(true), &ret_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  call_tmp_chpl7 = ret_tmp_chpl2;
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  call_tmp_chpl8 = chpl__buildDomainRuntimeType2(&defaultDist_chpl);
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  chpl__coerceMove4(&call_tmp_chpl8, &call_tmp_chpl7, UINT8(false), &ret_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  init_coerce_tmp_chpl = ret_tmp_chpl3;
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  chpl_check_nil(initTemp_chpl, _ln_chpl, _fn_chpl);
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  (initTemp_chpl)->myLocaleSpace = init_coerce_tmp_chpl;
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  chpl_check_nil(initTemp_chpl, _ln_chpl, _fn_chpl);
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  myLocaleSpace_chpl = (initTemp_chpl)->myLocaleSpace;
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  _instance_chpl = &((&myLocaleSpace_chpl)->_instance);
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  chpl_check_nil(_instance_chpl, _ln_chpl, _fn_chpl);
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  ((BaseDom_chpl)(*(_instance_chpl)))->definedConst = UINT8(true);
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  chpl_check_nil(initTemp_chpl, _ln_chpl, _fn_chpl);
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  call_tmp_chpl9 = &((initTemp_chpl)->myLocaleSpace);
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  call_tmp_chpl10 = chpl__ensureDomainExpr2(call_tmp_chpl9);
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  call_tmp_chpl11 = chpl__buildArrayRuntimeType6(call_tmp_chpl10);
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  _runtime_type_tmp__chpl = (&call_tmp_chpl11)->dom;
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  chpl__convertRuntimeTypeToValue7(&_runtime_type_tmp__chpl, UINT8(false), &ret_tmp_chpl4, _ln_chpl, _fn_chpl);
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  myLocales_chpl = ret_tmp_chpl4;
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  chpl_check_nil(initTemp_chpl, _ln_chpl, _fn_chpl);
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  (initTemp_chpl)->myLocales = myLocales_chpl;
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  ((RootClass_chpl)(initTemp_chpl))->chpl__cid = chpl__cid_RootLocale_chpl;
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  return initTemp_chpl;
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
}

#line 162 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
/* LocaleModel.chpl:162 */
#line 162 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
static void setup_chpl2(RootLocale_chpl this_chpl31,
#line 162 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
                        int64_t _ln_chpl,
#line 162 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
                        int32_t _fn_chpl) {
#line 163 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  helpSetupRootLocaleFlat_chpl(this_chpl31, _ln_chpl, _fn_chpl);
#line 162 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  return;
#line 162 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
}

#line 181 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
/* LocaleModel.chpl:181 */
#line 181 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
static int64_t _getChildCount_chpl4(RootLocale_chpl this_chpl31,
#line 181 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
                                    int64_t _ln_chpl,
#line 181 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
                                    int32_t _fn_chpl) {
#line 181 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  _ref__domain_DefaultRectangularDom_1_int64_t_one_chpl call_tmp_chpl6 = NULL;
#line 181 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  int64_t call_tmp_chpl7;
#line 181 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 181 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  call_tmp_chpl6 = &((this_chpl31)->myLocaleSpace);
#line 181 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  call_tmp_chpl7 = size_chpl4(call_tmp_chpl6, _ln_chpl, _fn_chpl);
#line 181 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  return call_tmp_chpl7;
#line 181 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
}

#line 190 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
/* LocaleModel.chpl:190 */
#line 190 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
static void _getChild_chpl4(RootLocale_chpl this_chpl31,
#line 190 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
                            int64_t idx_chpl,
#line 190 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
                            locale_chpl * _retArg_chpl,
#line 190 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
                            int64_t _ln_chpl,
#line 190 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
                            int32_t _fn_chpl) {
#line 190 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  _ref__array_DefaultRectangularArr_1_int64_t_one_locale_int64_t_chpl call_tmp_chpl6 = NULL;
#line 190 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  _tuple_1_star_int64_t_chpl coerce_tmp_chpl3;
#line 190 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  _ref__tuple_1_star_int64_t_chpl i_i_chpl = NULL;
#line 190 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  DefaultRectangularArr_1_int64_t_one_locale_int64_t_chpl ret_chpl = NULL;
#line 190 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  DefaultRectangularArr_1_int64_t_one_locale_int64_t_chpl coerce_tmp_chpl4 = NULL;
#line 190 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  _tuple_1_star_int64_t_chpl _formal_tmp_in_ind_chpl;
#line 190 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  int64_t _formal_tmp_ind_x0_chpl;
#line 190 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  _ddata_locale_chpl coerce_tmp_chpl5 = NULL;
#line 190 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  _ref_locale_chpl call_tmp_chpl7 = NULL;
#line 190 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  locale_chpl ret_chpl2;
#line 190 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  locale_chpl tmp_chpl;
#line 190 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 190 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  call_tmp_chpl6 = &((this_chpl31)->myLocales);
#line 190 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  *(coerce_tmp_chpl3 + INT64(0)) = idx_chpl;
#line 190 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  i_i_chpl = &coerce_tmp_chpl3;
#line 190 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  ret_chpl = (call_tmp_chpl6)->_instance;
#line 190 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  checkAccess_chpl7(call_tmp_chpl6, i_i_chpl, ret_chpl, _ln_chpl, _fn_chpl);
#line 190 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  coerce_tmp_chpl4 = ret_chpl;
#line 190 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  *(_formal_tmp_in_ind_chpl + INT64(0)) = *(coerce_tmp_chpl3 + INT64(0));
#line 190 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  _formal_tmp_ind_x0_chpl = *(_formal_tmp_in_ind_chpl + INT64(0));
#line 190 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  chpl_check_nil(coerce_tmp_chpl4, _ln_chpl, _fn_chpl);
#line 190 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  coerce_tmp_chpl5 = (coerce_tmp_chpl4)->shiftedData;
#line 190 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  call_tmp_chpl7 = (coerce_tmp_chpl5 + _formal_tmp_ind_x0_chpl);
#line 190 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  init_ASSIGN__chpl(&tmp_chpl, call_tmp_chpl7);
#line 190 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  ret_chpl2 = tmp_chpl;
#line 190 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  *(_retArg_chpl) = ret_chpl2;
#line 190 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  return;
#line 190 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
}

#line 198 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
/* LocaleModel.chpl:198 */
#line 198 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
static _array_DefaultRectangularArr_1_int64_t_one_locale_int64_t_chpl * getDefaultLocaleArray_chpl(RootLocale_chpl this_chpl31,
#line 198 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
                                                                                                   int64_t _ln_chpl,
#line 198 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
                                                                                                   int32_t _fn_chpl) {
#line 198 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  _ref__array_DefaultRectangularArr_1_int64_t_one_locale_int64_t_chpl call_tmp_chpl6 = NULL;
#line 198 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 198 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  call_tmp_chpl6 = &((this_chpl31)->myLocales);
#line 198 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  return call_tmp_chpl6;
#line 198 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
}

#line 200 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
/* LocaleModel.chpl:200 */
#line 200 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
static void localeIDtoLocale_chpl2(RootLocale_chpl this_chpl31,
#line 200 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
                                   chpl_localeID_t * id_chpl2,
#line 200 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
                                   locale_chpl * _retArg_chpl,
#line 200 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
                                   int64_t _ln_chpl,
#line 200 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
                                   int32_t _fn_chpl) {
#line 205 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  int32_t call_tmp_chpl6;
#line 205 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  _ref__array_DefaultRectangularArr_1_int64_t_one_locale_int64_t_chpl call_tmp_chpl7 = NULL;
#line 205 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  _tuple_1_star_int64_t_chpl coerce_tmp_chpl3;
#line 205 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  _ref__tuple_1_star_int64_t_chpl i_i_chpl = NULL;
#line 205 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  DefaultRectangularArr_1_int64_t_one_locale_int64_t_chpl ret_chpl = NULL;
#line 205 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  DefaultRectangularArr_1_int64_t_one_locale_int64_t_chpl coerce_tmp_chpl4 = NULL;
#line 205 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  _tuple_1_star_int64_t_chpl _formal_tmp_in_ind_chpl;
#line 205 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  int64_t _formal_tmp_ind_x0_chpl;
#line 205 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  _ddata_locale_chpl coerce_tmp_chpl5 = NULL;
#line 205 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  _ref_locale_chpl call_tmp_chpl8 = NULL;
#line 205 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  locale_chpl ret_chpl2;
#line 205 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  locale_chpl tmp_chpl;
#line 205 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  call_tmp_chpl6 = chpl_rt_nodeFromLocaleID(*(id_chpl2));
#line 205 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 205 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  call_tmp_chpl7 = &((this_chpl31)->myLocales);
#line 205 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  *(coerce_tmp_chpl3 + INT64(0)) = ((int64_t)(call_tmp_chpl6));
#line 205 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  i_i_chpl = &coerce_tmp_chpl3;
#line 205 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  ret_chpl = (call_tmp_chpl7)->_instance;
#line 205 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  checkAccess_chpl7(call_tmp_chpl7, i_i_chpl, ret_chpl, _ln_chpl, _fn_chpl);
#line 205 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  coerce_tmp_chpl4 = ret_chpl;
#line 205 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  *(_formal_tmp_in_ind_chpl + INT64(0)) = *(coerce_tmp_chpl3 + INT64(0));
#line 205 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  _formal_tmp_ind_x0_chpl = *(_formal_tmp_in_ind_chpl + INT64(0));
#line 205 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  chpl_check_nil(coerce_tmp_chpl4, _ln_chpl, _fn_chpl);
#line 205 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  coerce_tmp_chpl5 = (coerce_tmp_chpl4)->shiftedData;
#line 205 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  call_tmp_chpl8 = (coerce_tmp_chpl5 + _formal_tmp_ind_x0_chpl);
#line 205 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  init_ASSIGN__chpl(&tmp_chpl, call_tmp_chpl8);
#line 205 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  ret_chpl2 = tmp_chpl;
#line 200 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  *(_retArg_chpl) = ret_chpl2;
#line 200 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  return;
#line 200 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
}

#line 208 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
/* LocaleModel.chpl:208 */
#line 208 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
static void deinit_chpl64(RootLocale_chpl this_chpl31,
#line 208 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
                          int64_t _ln_chpl,
#line 208 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
                          int32_t _fn_chpl) {
#line 209 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  _array_DefaultRectangularArr_1_int64_t_one_locale_int64_t_chpl _ir_F0_this_chpl;
#line 209 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  _array_DefaultRectangularArr_1_int64_t_one_locale_int64_t_chpl this_chpl32;
#line 209 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  DefaultRectangularArr_1_int64_t_one_locale_int64_t_chpl ret_chpl = NULL;
#line 209 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  DefaultRectangularArr_1_int64_t_one_locale_int64_t_chpl this_chpl33 = NULL;
#line 209 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  DefaultRectangularDom_1_int64_t_one_chpl coerce_tmp_chpl3 = NULL;
#line 209 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  DefaultRectangularDom_1_int64_t_one_chpl _ic__F1_viewDom_chpl = NULL;
#line 209 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  DefaultRectangularArr_1_int64_t_one_locale_int64_t_chpl info_chpl = NULL;
#line 209 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  int64_t call_tmp_chpl6;
#line 209 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  int64_t call_tmp_chpl7;
#line 209 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  int64_t call_tmp_chpl8;
#line 209 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  int64_t call_tmp_chpl9;
#line 209 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  DefaultRectangularDom_1_int64_t_one_chpl viewDom_chpl = NULL;
#line 209 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  int64_t call_tmp_chpl10;
#line 209 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  int64_t _ic__F1_high_chpl;
#line 209 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  int64_t _ic__F2_stride_chpl;
#line 209 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  int64_t i_chpl;
#line 209 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  _ddata_locale_chpl coerce_tmp_chpl4 = NULL;
#line 209 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  _ref_locale_chpl call_tmp_chpl11 = NULL;
#line 148 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  _array_DefaultRectangularArr_1_int64_t_one_locale_int64_t_chpl _field_destructor_tmp__chpl;
#line 146 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  _domain_DefaultRectangularDom_1_int64_t_one_chpl _field_destructor_tmp__chpl2;
#line 208 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  BaseLocale_chpl _parent_destructor_tmp__chpl = NULL;
#line 208 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  _ref_AtomicT_int64_t_chpl _field_destructor_tmp__chpl3 = NULL;
#line 208 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  _ref_locale_chpl _field_destructor_tmp__chpl4 = NULL;
#line 209 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 209 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  _ir_F0_this_chpl = (this_chpl31)->myLocales;
#line 209 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  this_chpl32 = _ir_F0_this_chpl;
#line 209 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  ret_chpl = (&this_chpl32)->_instance;
#line 209 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  this_chpl33 = ret_chpl;
#line 209 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  chpl_check_nil(this_chpl33, _ln_chpl, _fn_chpl);
#line 209 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  coerce_tmp_chpl3 = (this_chpl33)->dom;
#line 209 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  _ic__F1_viewDom_chpl = coerce_tmp_chpl3;
#line 209 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  info_chpl = ret_chpl;
#line 209 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  chpl_check_nil(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 209 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  call_tmp_chpl6 = dsiLow_chpl(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 209 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  chpl_check_nil(_ic__F1_viewDom_chpl, _ln_chpl, _fn_chpl);
#line 209 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  call_tmp_chpl7 = dsiLow_chpl(_ic__F1_viewDom_chpl, _ln_chpl, _fn_chpl);
#line 209 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  call_tmp_chpl8 = (((int64_t)((call_tmp_chpl7 + INT64(1)))) - call_tmp_chpl6);
#line 209 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  viewDom_chpl = _ic__F1_viewDom_chpl;
#line 209 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  chpl_check_nil(viewDom_chpl, _ln_chpl, _fn_chpl);
#line 209 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  call_tmp_chpl9 = dsiNumIndices_chpl2(viewDom_chpl, _ln_chpl, _fn_chpl);
#line 209 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  call_tmp_chpl10 = (call_tmp_chpl6 + ((int64_t)((((int64_t)((call_tmp_chpl9 - INT64(1)))) * call_tmp_chpl8))));
#line 209 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  _ic__F1_high_chpl = call_tmp_chpl10;
#line 209 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  _ic__F2_stride_chpl = call_tmp_chpl8;
#line 209 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  if (call_tmp_chpl8 == INT64(0)) /* ZLINE: 209 /Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl */
#line 209 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  {
#line 209 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
    boundsCheckHalt_chpl(&_str_literal_557_chpl, _ln_chpl, _fn_chpl);
#line 209 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  }
#line 209 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  chpl_checkIfRangeIterWillOverflow(call_tmp_chpl6, call_tmp_chpl10, call_tmp_chpl8, call_tmp_chpl6, call_tmp_chpl10, UINT8(true), _ln_chpl, _fn_chpl);
#line 209 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  i_chpl = INT64(0);
#line 209 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  chpl_check_nil(info_chpl, _ln_chpl, _fn_chpl);
#line 209 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  coerce_tmp_chpl4 = (info_chpl)->shiftedData;
#line 209 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  for (i_chpl = call_tmp_chpl6; ((i_chpl <= _ic__F1_high_chpl)); i_chpl += _ic__F2_stride_chpl) {
#line 209 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
    call_tmp_chpl11 = (coerce_tmp_chpl4 + i_chpl);
#line 210 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
    if (((RootClass_chpl)((call_tmp_chpl11)->_instance)) == nil) /* ZLINE: 210 /Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl */
#line 210 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
    {
#line 210 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
      nilCheckHalt_chpl(&_str_literal_52_chpl, _ln_chpl, _fn_chpl);
#line 210 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
    }
#line 211 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
    rootLocaleInitialized_chpl = UINT8(false);
#line 212 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
    chpl__delete28((call_tmp_chpl11)->_instance, _ln_chpl, _fn_chpl);
#line 212 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  }
#line 148 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 148 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  _field_destructor_tmp__chpl = (this_chpl31)->myLocales;
#line 148 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  chpl__autoDestroy30(&_field_destructor_tmp__chpl, _ln_chpl, _fn_chpl);
#line 146 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 146 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  _field_destructor_tmp__chpl2 = (this_chpl31)->myLocaleSpace;
#line 146 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  chpl__autoDestroy11(&_field_destructor_tmp__chpl2, _ln_chpl, _fn_chpl);
#line 208 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  _parent_destructor_tmp__chpl = ((BaseLocale_chpl)(((AbstractRootLocale_chpl)(this_chpl31))));
#line 208 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  chpl_check_nil(_parent_destructor_tmp__chpl, _ln_chpl, _fn_chpl);
#line 208 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  _field_destructor_tmp__chpl3 = &((_parent_destructor_tmp__chpl)->runningTaskCounter);
#line 208 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  deinit_chpl2(_field_destructor_tmp__chpl3);
#line 208 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  chpl_check_nil(_parent_destructor_tmp__chpl, _ln_chpl, _fn_chpl);
#line 208 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  _field_destructor_tmp__chpl4 = &((_parent_destructor_tmp__chpl)->parent);
#line 208 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  deinit_chpl62(_field_destructor_tmp__chpl4);
#line 208 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  return;
#line 208 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
}

#line 228 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
/* LocaleModel.chpl:228 */
#line 228 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
static void deinit_chpl65(int64_t _ln_chpl,
#line 228 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
                          int32_t _fn_chpl) {
#line 55 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  _domain_DefaultRectangularDom_1_int64_t_one_chpl local_chpl_emptyLocaleSpace_chpl;
#line 229 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  _array_DefaultRectangularArr_1_int64_t_one_locale_int64_t_chpl local_chpl_emptyLocales_chpl;
#line 229 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  _array_DefaultRectangularArr_1_int64_t_one_locale_int64_t_chpl _ir_F0_this_chpl;
#line 229 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  _array_DefaultRectangularArr_1_int64_t_one_locale_int64_t_chpl this_chpl31;
#line 229 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  DefaultRectangularArr_1_int64_t_one_locale_int64_t_chpl ret_chpl = NULL;
#line 229 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  DefaultRectangularArr_1_int64_t_one_locale_int64_t_chpl this_chpl32 = NULL;
#line 229 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  DefaultRectangularDom_1_int64_t_one_chpl coerce_tmp_chpl3 = NULL;
#line 229 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  DefaultRectangularDom_1_int64_t_one_chpl _ic__F1_viewDom_chpl = NULL;
#line 229 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  DefaultRectangularArr_1_int64_t_one_locale_int64_t_chpl info_chpl = NULL;
#line 229 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  int64_t call_tmp_chpl6;
#line 229 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  int64_t call_tmp_chpl7;
#line 229 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  int64_t call_tmp_chpl8;
#line 229 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  int64_t call_tmp_chpl9;
#line 229 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  DefaultRectangularDom_1_int64_t_one_chpl viewDom_chpl = NULL;
#line 229 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  int64_t call_tmp_chpl10;
#line 229 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  int64_t _ic__F1_high_chpl;
#line 229 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  int64_t _ic__F2_stride_chpl;
#line 229 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  int64_t i_chpl;
#line 229 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  _ddata_locale_chpl coerce_tmp_chpl4 = NULL;
#line 229 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  _ref_locale_chpl call_tmp_chpl11 = NULL;
#line 55 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  local_chpl_emptyLocaleSpace_chpl = chpl_emptyLocaleSpace;
#line 229 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  local_chpl_emptyLocales_chpl = chpl_emptyLocales;
#line 229 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  _ir_F0_this_chpl = local_chpl_emptyLocales_chpl;
#line 229 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  this_chpl31 = _ir_F0_this_chpl;
#line 229 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  ret_chpl = (&this_chpl31)->_instance;
#line 229 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  this_chpl32 = ret_chpl;
#line 229 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  chpl_check_nil(this_chpl32, _ln_chpl, _fn_chpl);
#line 229 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  coerce_tmp_chpl3 = (this_chpl32)->dom;
#line 229 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  _ic__F1_viewDom_chpl = coerce_tmp_chpl3;
#line 229 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  info_chpl = ret_chpl;
#line 229 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  chpl_check_nil(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 229 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  call_tmp_chpl6 = dsiLow_chpl(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 229 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  chpl_check_nil(_ic__F1_viewDom_chpl, _ln_chpl, _fn_chpl);
#line 229 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  call_tmp_chpl7 = dsiLow_chpl(_ic__F1_viewDom_chpl, _ln_chpl, _fn_chpl);
#line 229 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  call_tmp_chpl8 = (((int64_t)((call_tmp_chpl7 + INT64(1)))) - call_tmp_chpl6);
#line 229 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  viewDom_chpl = _ic__F1_viewDom_chpl;
#line 229 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  chpl_check_nil(viewDom_chpl, _ln_chpl, _fn_chpl);
#line 229 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  call_tmp_chpl9 = dsiNumIndices_chpl2(viewDom_chpl, _ln_chpl, _fn_chpl);
#line 229 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  call_tmp_chpl10 = (call_tmp_chpl6 + ((int64_t)((((int64_t)((call_tmp_chpl9 - INT64(1)))) * call_tmp_chpl8))));
#line 229 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  _ic__F1_high_chpl = call_tmp_chpl10;
#line 229 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  _ic__F2_stride_chpl = call_tmp_chpl8;
#line 229 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  if (call_tmp_chpl8 == INT64(0)) /* ZLINE: 229 /Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl */
#line 229 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  {
#line 229 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
    boundsCheckHalt_chpl(&_str_literal_557_chpl, _ln_chpl, _fn_chpl);
#line 229 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  }
#line 229 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  chpl_checkIfRangeIterWillOverflow(call_tmp_chpl6, call_tmp_chpl10, call_tmp_chpl8, call_tmp_chpl6, call_tmp_chpl10, UINT8(true), _ln_chpl, _fn_chpl);
#line 229 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  i_chpl = INT64(0);
#line 229 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  chpl_check_nil(info_chpl, _ln_chpl, _fn_chpl);
#line 229 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  coerce_tmp_chpl4 = (info_chpl)->shiftedData;
#line 229 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  for (i_chpl = call_tmp_chpl6; ((i_chpl <= _ic__F1_high_chpl)); i_chpl += _ic__F2_stride_chpl) {
#line 229 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
    call_tmp_chpl11 = (coerce_tmp_chpl4 + i_chpl);
#line 230 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
    chpl__delete28((call_tmp_chpl11)->_instance, _ln_chpl, _fn_chpl);
#line 230 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  }
#line 57 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  chpl__autoDestroy30(&local_chpl_emptyLocales_chpl, _ln_chpl, _fn_chpl);
#line 55 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  chpl__autoDestroy11(&local_chpl_emptyLocaleSpace_chpl, _ln_chpl, _fn_chpl);
#line 228 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
  return;
#line 228 "/Users/iainmoncrief/Documents/chapel/modules/internal/localeModels/flat/LocaleModel.chpl"
}

