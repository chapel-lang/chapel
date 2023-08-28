#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
/* SharedObject.chpl:23 */
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
static void chpl__init_SharedObject(int64_t _ln_chpl,
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
                                    int32_t _fn_chpl) {
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
  _ref_int32_t_chpl refIndentLevel_chpl = NULL;
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
  if (chpl__init_SharedObject_p) /* ZLINE: 23 /Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl */
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
  {
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
    goto _exit_chpl__init_SharedObject_chpl;
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
  }
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
  printModuleInit_chpl("%*s\n", "SharedObject", INT64(12), _ln_chpl, _fn_chpl);
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
  refIndentLevel_chpl = &moduleInitLevel_chpl;
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
  *(refIndentLevel_chpl) += INT64(1);
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
  chpl__init_SharedObject_p = UINT8(true);
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
  {
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
    chpl__init_Errors(_ln_chpl, _fn_chpl);
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
    chpl__init_Atomics(_ln_chpl, _fn_chpl);
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
    chpl__init_ChapelBase(_ln_chpl, _fn_chpl);
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
    chpl__init_OwnedObject(_ln_chpl, _fn_chpl);
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
    chpl__init_HaltWrappers(_ln_chpl, _fn_chpl);
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
  }
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
  *(refIndentLevel_chpl) -= INT64(1);
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
  _exit_chpl__init_SharedObject_chpl:;
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
  return;
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
}

#line 34 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
/* SharedObject.chpl:34 */
#line 34 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
static void chpl__auto_destroy_ReferenceCount(ReferenceCount_chpl this_chpl31,
#line 34 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
                                              int64_t _ln_chpl,
#line 34 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
                                              int32_t _fn_chpl) {
#line 38 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
  _ref_AtomicT_int64_t_chpl _field_destructor_tmp__chpl = NULL;
#line 36 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
  _ref_AtomicT_int64_t_chpl _field_destructor_tmp__chpl2 = NULL;
#line 38 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 38 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
  _field_destructor_tmp__chpl = &((this_chpl31)->totalCount);
#line 38 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
  deinit_chpl2(_field_destructor_tmp__chpl);
#line 36 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 36 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
  _field_destructor_tmp__chpl2 = &((this_chpl31)->strongCount);
#line 36 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
  deinit_chpl2(_field_destructor_tmp__chpl2);
#line 34 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
  return;
#line 34 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
}

#line 133 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
/* SharedObject.chpl:133 */
#line 133 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
static void init_chpl191(_shared_Timezone_chpl * this_chpl31,
#line 133 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
                         int64_t _ln_chpl,
#line 133 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
                         int32_t _fn_chpl) {
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
  Timezone_chpl init_coerce_tmp_chpl = NULL;
#line 139 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
  ReferenceCount_chpl init_coerce_tmp_chpl2 = NULL;
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
  init_coerce_tmp_chpl = ((Timezone_chpl)(nil));
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
  (this_chpl31)->chpl_p = init_coerce_tmp_chpl;
#line 139 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
  init_coerce_tmp_chpl2 = ((ReferenceCount_chpl)(nil));
#line 139 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
  (this_chpl31)->chpl_pn = init_coerce_tmp_chpl2;
#line 133 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
  return;
#line 133 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
}

#line 317 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
/* SharedObject.chpl:317 */
#line 317 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
static void init_ASSIGN__chpl23(_shared_Timezone_chpl * this_chpl31,
#line 317 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
                                _nilType src_chpl,
#line 317 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
                                int64_t _ln_chpl,
#line 317 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
                                int32_t _fn_chpl) {
#line 321 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
  init_chpl191(this_chpl31, _ln_chpl, _fn_chpl);
#line 317 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
  return;
#line 317 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
}

#line 328 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
/* SharedObject.chpl:328 */
#line 328 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
static void doClear_chpl(_shared_Timezone_chpl * this_chpl31,
#line 328 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
                         int64_t _ln_chpl,
#line 328 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
                         int32_t _fn_chpl) {
#line 329 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
  chpl_bool tmp_chpl;
#line 330 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
  ReferenceCount_chpl coerce_tmp_chpl3 = NULL;
#line 330 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
  ReferenceCount_chpl coerce_tmp_chpl4 = NULL;
#line 330 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
  _ref_AtomicT_int64_t_chpl call_tmp_chpl6 = NULL;
#line 330 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
  _ref_atomic_int_least64_t_chpl call_tmp_chpl7 = NULL;
#line 330 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
  memory_order ret_chpl;
#line 330 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
  int64_t call_tmp_chpl8;
#line 334 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
  ReferenceCount_chpl coerce_tmp_chpl5 = NULL;
#line 334 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
  ReferenceCount_chpl coerce_tmp_chpl6 = NULL;
#line 334 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
  _ref_AtomicT_int64_t_chpl call_tmp_chpl9 = NULL;
#line 334 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
  _ref_atomic_int_least64_t_chpl call_tmp_chpl10 = NULL;
#line 334 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
  memory_order ret_chpl2;
#line 334 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
  int64_t call_tmp_chpl11;
#line 329 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
  if (((RootClass_chpl)((this_chpl31)->chpl_p)) != nil) /* ZLINE: 329 /Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl */
#line 329 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
  {
#line 329 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
    tmp_chpl = (((RootClass_chpl)((this_chpl31)->chpl_pn)) != nil);
#line 329 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
  } else /* ZLINE: 329 /Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl */
#line 329 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
  {
#line 329 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
    tmp_chpl = UINT8(false);
#line 329 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
  }
#line 329 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
  if (tmp_chpl) /* ZLINE: 329 /Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl */
#line 329 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
  {
#line 330 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
    coerce_tmp_chpl3 = (this_chpl31)->chpl_pn;
#line 330 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
    if (((RootClass_chpl)(coerce_tmp_chpl3)) == nil) /* ZLINE: 330 /Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl */
#line 330 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
    {
#line 330 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
      nilCheckHalt_chpl(&_str_literal_52_chpl, _ln_chpl, _fn_chpl);
#line 330 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
    }
#line 330 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
    coerce_tmp_chpl4 = coerce_tmp_chpl3;
#line 330 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
    chpl_check_nil(coerce_tmp_chpl4, _ln_chpl, _fn_chpl);
#line 330 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
    call_tmp_chpl6 = &((coerce_tmp_chpl4)->strongCount);
#line 330 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
    chpl_rmem_consist_maybe_release2(_ln_chpl, _fn_chpl);
#line 330 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
    call_tmp_chpl7 = &((call_tmp_chpl6)->_v);
#line 330 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
    ret_chpl = memory_order_seq_cst;
#line 330 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
    call_tmp_chpl8 = atomic_fetch_sub_explicit_int_least64_t(call_tmp_chpl7, INT64(1), ret_chpl);
#line 331 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
    if (((int64_t)((call_tmp_chpl8 - INT64(1)))) == INT64(0)) /* ZLINE: 331 /Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl */
#line 331 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
    {
#line 332 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
      chpl__delete24((this_chpl31)->chpl_p, _ln_chpl, _fn_chpl);
#line 332 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
    }
#line 334 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
    coerce_tmp_chpl5 = (this_chpl31)->chpl_pn;
#line 334 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
    if (((RootClass_chpl)(coerce_tmp_chpl5)) == nil) /* ZLINE: 334 /Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl */
#line 334 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
    {
#line 334 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
      nilCheckHalt_chpl(&_str_literal_52_chpl, _ln_chpl, _fn_chpl);
#line 334 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
    }
#line 334 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
    coerce_tmp_chpl6 = coerce_tmp_chpl5;
#line 334 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
    chpl_check_nil(coerce_tmp_chpl6, _ln_chpl, _fn_chpl);
#line 334 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
    call_tmp_chpl9 = &((coerce_tmp_chpl6)->totalCount);
#line 334 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
    chpl_rmem_consist_maybe_release2(_ln_chpl, _fn_chpl);
#line 334 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
    call_tmp_chpl10 = &((call_tmp_chpl9)->_v);
#line 334 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
    ret_chpl2 = memory_order_seq_cst;
#line 334 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
    call_tmp_chpl11 = atomic_fetch_sub_explicit_int_least64_t(call_tmp_chpl10, INT64(1), ret_chpl2);
#line 335 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
    if (((int64_t)((call_tmp_chpl11 - INT64(1)))) == INT64(0)) /* ZLINE: 335 /Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl */
#line 335 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
    {
#line 336 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
      chpl__delete27((this_chpl31)->chpl_pn, _ln_chpl, _fn_chpl);
#line 336 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
    }
#line 336 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
  }
#line 338 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
  (this_chpl31)->chpl_p = ((Timezone_chpl)(nil));
#line 339 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
  (this_chpl31)->chpl_pn = ((ReferenceCount_chpl)(nil));
#line 328 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
  return;
#line 328 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
}

#line 415 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
/* SharedObject.chpl:415 */
#line 415 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
static void deinit_chpl94(_shared_Timezone_chpl * this_chpl31,
#line 415 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
                          int64_t _ln_chpl,
#line 415 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
                          int32_t _fn_chpl) {
#line 417 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
  doClear_chpl(this_chpl31, _ln_chpl, _fn_chpl);
#line 415 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
  return;
#line 415 "/Users/iainmoncrief/Documents/chapel/modules/internal/SharedObject.chpl"
}

