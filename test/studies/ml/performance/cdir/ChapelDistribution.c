#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
/* ChapelDistribution.chpl:21 */
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
static void chpl__init_ChapelDistribution(int64_t _ln_chpl,
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                                          int32_t _fn_chpl) {
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  _ref_int32_t_chpl refIndentLevel_chpl = NULL;
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  if (chpl__init_ChapelDistribution_p) /* ZLINE: 21 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl */
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  {
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    goto _exit_chpl__init_ChapelDistribution_chpl;
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  }
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  printModuleInit_chpl("%*s\n", "ChapelDistribution", INT64(18), _ln_chpl, _fn_chpl);
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  refIndentLevel_chpl = &moduleInitLevel_chpl;
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  *(refIndentLevel_chpl) += INT64(1);
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl__init_ChapelDistribution_p = UINT8(true);
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  {
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    chpl__init_ChapelArray(_ln_chpl, _fn_chpl);
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    chpl__init_ChapelRange(_ln_chpl, _fn_chpl);
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    chpl__init_ChapelLocks(_ln_chpl, _fn_chpl);
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    chpl__init_ChapelHashtable(_ln_chpl, _fn_chpl);
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    chpl__init_Sort(_ln_chpl, _fn_chpl);
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    chpl__init_CTypes(_ln_chpl, _fn_chpl);
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    chpl__init_AutoMath(_ln_chpl, _fn_chpl);
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  }
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  *(refIndentLevel_chpl) -= INT64(1);
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  _exit_chpl__init_ChapelDistribution_chpl:;
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  return;
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
}

#line 37 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
/* ChapelDistribution.chpl:37 */
#line 37 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
static void deinit_chpl30(BaseDist_chpl this_chpl31,
#line 37 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                          int64_t _ln_chpl,
#line 37 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                          int32_t _fn_chpl) {
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  _ref_chpl_LocalSpinlock_chpl _field_destructor_tmp__chpl = NULL;
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  _ref_AtomicBool_chpl _field_destructor_tmp__chpl2 = NULL;
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  _field_destructor_tmp__chpl = &((this_chpl31)->_domsLock);
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  _field_destructor_tmp__chpl2 = &((_field_destructor_tmp__chpl)->l);
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  deinit_chpl(_field_destructor_tmp__chpl2);
#line 37 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  return;
#line 37 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
}

#line 42 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
/* ChapelDistribution.chpl:42 */
#line 42 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
static BaseDist_chpl remove_chpl(BaseDist_chpl this_chpl31,
#line 42 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                                 int64_t _ln_chpl,
#line 42 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                                 int32_t _fn_chpl) {
#line 42 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseDist_chpl ret_chpl = NULL;
#line 43 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_bool free_dist_chpl;
#line 44 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_bool call_tmp_chpl6;
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  _ref_chpl_LocalSpinlock_chpl call_tmp_chpl7 = NULL;
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_bool tmp_chpl;
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  _ref_AtomicBool_chpl call_tmp_chpl8 = NULL;
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  _ref_atomic_bool_chpl call_tmp_chpl9 = NULL;
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  memory_order ret_chpl2;
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_bool call_tmp_chpl10;
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_bool tmp_chpl2;
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  _ref_AtomicBool_chpl call_tmp_chpl11 = NULL;
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  _ref_atomic_bool_chpl call_tmp_chpl12 = NULL;
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  memory_order ret_chpl3;
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_bool call_tmp_chpl13;
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  _ref_AtomicBool_chpl call_tmp_chpl14 = NULL;
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  _ref_atomic_bool_chpl call_tmp_chpl15 = NULL;
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  memory_order ret_chpl4;
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_bool call_tmp_chpl16;
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_bool tmp_chpl3;
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  _ref_AtomicBool_chpl call_tmp_chpl17 = NULL;
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  _ref_atomic_bool_chpl call_tmp_chpl18 = NULL;
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  memory_order ret_chpl5;
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_bool call_tmp_chpl19;
#line 52 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  int64_t coerce_tmp_chpl3;
#line 53 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  _ref_chpl_LocalSpinlock_chpl call_tmp_chpl20 = NULL;
#line 53 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  _ref_AtomicBool_chpl call_tmp_chpl21 = NULL;
#line 53 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  _ref_atomic_bool_chpl call_tmp_chpl22 = NULL;
#line 53 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  memory_order ret_chpl6;
#line 43 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  free_dist_chpl = UINT8(false);
#line 44 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 44 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  call_tmp_chpl6 = ((chpl_bool(*)(BaseDist_chpl))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(this_chpl31))->chpl__cid) + INT64(2))])(this_chpl31);
#line 44 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  if (call_tmp_chpl6) /* ZLINE: 44 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl */
#line 44 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  {
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    call_tmp_chpl7 = &((this_chpl31)->_domsLock);
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    call_tmp_chpl8 = &((call_tmp_chpl7)->l);
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    chpl_rmem_consist_maybe_release2(_ln_chpl, _fn_chpl);
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    call_tmp_chpl9 = &((call_tmp_chpl8)->_v);
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    ret_chpl2 = memory_order_seq_cst;
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    call_tmp_chpl10 = atomic_load_explicit_bool(call_tmp_chpl9, ret_chpl2);
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    if (call_tmp_chpl10) /* ZLINE: 48 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl */
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    {
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      tmp_chpl2 = UINT8(true);
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    } else /* ZLINE: 48 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl */
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    {
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      call_tmp_chpl11 = &((call_tmp_chpl7)->l);
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      chpl_rmem_consist_maybe_release5(_ln_chpl, _fn_chpl);
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      chpl_rmem_consist_maybe_release5(_ln_chpl, _fn_chpl);
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      call_tmp_chpl12 = &((call_tmp_chpl11)->_v);
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      ret_chpl3 = memory_order_acquire;
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      call_tmp_chpl13 = atomic_exchange_explicit_bool(call_tmp_chpl12, UINT8(true), ret_chpl3);
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      chpl_rmem_consist_maybe_acquire5(_ln_chpl, _fn_chpl);
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      tmp_chpl2 = call_tmp_chpl13;
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    }
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    tmp_chpl = tmp_chpl2;
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    while (tmp_chpl) {
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      chpl_task_yield();
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      call_tmp_chpl14 = &((call_tmp_chpl7)->l);
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      chpl_rmem_consist_maybe_release2(_ln_chpl, _fn_chpl);
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      call_tmp_chpl15 = &((call_tmp_chpl14)->_v);
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      ret_chpl4 = memory_order_seq_cst;
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      call_tmp_chpl16 = atomic_load_explicit_bool(call_tmp_chpl15, ret_chpl4);
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      if (call_tmp_chpl16) /* ZLINE: 48 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl */
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      {
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
        tmp_chpl3 = UINT8(true);
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      } else /* ZLINE: 48 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl */
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      {
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
        call_tmp_chpl17 = &((call_tmp_chpl7)->l);
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
        chpl_rmem_consist_maybe_release5(_ln_chpl, _fn_chpl);
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
        chpl_rmem_consist_maybe_release5(_ln_chpl, _fn_chpl);
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
        call_tmp_chpl18 = &((call_tmp_chpl17)->_v);
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
        ret_chpl5 = memory_order_acquire;
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
        call_tmp_chpl19 = atomic_exchange_explicit_bool(call_tmp_chpl18, UINT8(true), ret_chpl5);
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
        chpl_rmem_consist_maybe_acquire5(_ln_chpl, _fn_chpl);
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
        tmp_chpl3 = call_tmp_chpl19;
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      }
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      tmp_chpl = tmp_chpl3;
#line 48 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    }
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    (this_chpl31)->_free_when_no_doms = UINT8(true);
#line 52 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 52 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    coerce_tmp_chpl3 = (this_chpl31)->_doms_containing_dist;
#line 53 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 53 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    call_tmp_chpl20 = &((this_chpl31)->_domsLock);
#line 53 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    call_tmp_chpl21 = &((call_tmp_chpl20)->l);
#line 53 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    chpl_rmem_consist_maybe_release4(_ln_chpl, _fn_chpl);
#line 53 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    chpl_rmem_consist_maybe_release4(_ln_chpl, _fn_chpl);
#line 53 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    call_tmp_chpl22 = &((call_tmp_chpl21)->_v);
#line 53 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    ret_chpl6 = memory_order_release;
#line 53 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    atomic_store_explicit_bool(call_tmp_chpl22, UINT8(false), ret_chpl6);
#line 53 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    chpl_rmem_consist_maybe_acquire4(_ln_chpl, _fn_chpl);
#line 53 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    chpl_rmem_consist_maybe_acquire4(_ln_chpl, _fn_chpl);
#line 55 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    if (coerce_tmp_chpl3 == INT64(0)) /* ZLINE: 55 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl */
#line 55 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    {
#line 56 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      free_dist_chpl = UINT8(true);
#line 56 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    }
#line 56 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  } else /* ZLINE: 58 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl */
#line 56 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  {
#line 61 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    free_dist_chpl = UINT8(true);
#line 61 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  }
#line 63 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  if (free_dist_chpl) /* ZLINE: 63 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl */
#line 63 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  {
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    ret_chpl = this_chpl31;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    goto _end_remove_chpl;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  } else /* ZLINE: 63 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl */
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  {
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    ret_chpl = ((BaseDist_chpl)(nil));
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    goto _end_remove_chpl;
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  }
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  _end_remove_chpl:;
#line 42 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  return ret_chpl;
#line 42 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
}

#line 149 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
/* ChapelDistribution.chpl:149 */
#line 149 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
static void dsiDestroyDist_chpl4(BaseDist_chpl this_chpl31) {
#line 149 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  return;
#line 149 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
}

#line 156 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
/* ChapelDistribution.chpl:156 */
#line 156 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
static chpl_bool dsiTrackDomains_chpl(BaseDist_chpl this_chpl31) {
#line 156 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  return UINT8(true);
#line 156 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
}

#line 189 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
/* ChapelDistribution.chpl:189 */
#line 189 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
static void deinit_chpl31(BaseDom_chpl this_chpl31,
#line 189 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                          int64_t _ln_chpl,
#line 189 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                          int32_t _fn_chpl) {
#line 180 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  _ref_chpl_LocalSpinlock_chpl _field_destructor_tmp__chpl = NULL;
#line 180 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  _ref_AtomicBool_chpl _field_destructor_tmp__chpl2 = NULL;
#line 180 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 180 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  _field_destructor_tmp__chpl = &((this_chpl31)->_arrsLock);
#line 180 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  _field_destructor_tmp__chpl2 = &((_field_destructor_tmp__chpl)->l);
#line 180 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  deinit_chpl(_field_destructor_tmp__chpl2);
#line 189 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  return;
#line 189 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
}

#line 200 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
/* ChapelDistribution.chpl:200 */
#line 200 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
static BaseDist_chpl dsiMyDist_chpl5(BaseDom_chpl this_chpl31,
#line 200 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                                     int64_t _ln_chpl,
#line 200 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                                     int32_t _fn_chpl) {
#line 202 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseDist_chpl ret_chpl = NULL;
#line 201 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  halt_chpl(&_str_literal_1116_chpl, _ln_chpl, _fn_chpl);
#line 202 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  ret_chpl = ((BaseDist_chpl)(nil));
#line 200 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  return ret_chpl;
#line 200 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
}

#line 252 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
/* ChapelDistribution.chpl:252 */
#line 252 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
static void remove_chpl2(BaseDom_chpl this_chpl31,
#line 252 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                         _tuple_2_BaseDom_BaseDist_chpl * _retArg_chpl,
#line 252 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                         int64_t _ln_chpl,
#line 252 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                         int32_t _fn_chpl) {
#line 256 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseDist_chpl call_tmp_chpl6 = NULL;
#line 256 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_bool call_tmp_chpl7;
#line 256 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_bool call_tmp_chpl8;
#line 259 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseDom_chpl ret_dom_chpl = NULL;
#line 259 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseDom_chpl init_coerce_tmp_chpl = NULL;
#line 260 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseDist_chpl ret_dist_chpl = NULL;
#line 260 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseDist_chpl init_coerce_tmp_chpl2 = NULL;
#line 261 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseDist_chpl call_tmp_chpl9 = NULL;
#line 261 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseDist_chpl dist_chpl = NULL;
#line 262 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_bool free_dom_chpl;
#line 263 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_bool remove_dist_chpl;
#line 269 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  _ref_chpl_LocalSpinlock_chpl call_tmp_chpl10 = NULL;
#line 269 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_bool tmp_chpl;
#line 269 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  _ref_AtomicBool_chpl call_tmp_chpl11 = NULL;
#line 269 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  _ref_atomic_bool_chpl call_tmp_chpl12 = NULL;
#line 269 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  memory_order ret_chpl;
#line 269 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_bool call_tmp_chpl13;
#line 269 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_bool tmp_chpl2;
#line 269 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  _ref_AtomicBool_chpl call_tmp_chpl14 = NULL;
#line 269 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  _ref_atomic_bool_chpl call_tmp_chpl15 = NULL;
#line 269 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  memory_order ret_chpl2;
#line 269 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_bool call_tmp_chpl16;
#line 269 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  _ref_AtomicBool_chpl call_tmp_chpl17 = NULL;
#line 269 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  _ref_atomic_bool_chpl call_tmp_chpl18 = NULL;
#line 269 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  memory_order ret_chpl3;
#line 269 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_bool call_tmp_chpl19;
#line 269 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_bool tmp_chpl3;
#line 269 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  _ref_AtomicBool_chpl call_tmp_chpl20 = NULL;
#line 269 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  _ref_atomic_bool_chpl call_tmp_chpl21 = NULL;
#line 269 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  memory_order ret_chpl4;
#line 269 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_bool call_tmp_chpl22;
#line 270 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  int64_t coerce_tmp_chpl3;
#line 272 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  _ref_chpl_LocalSpinlock_chpl call_tmp_chpl23 = NULL;
#line 272 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  _ref_AtomicBool_chpl call_tmp_chpl24 = NULL;
#line 272 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  _ref_atomic_bool_chpl call_tmp_chpl25 = NULL;
#line 272 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  memory_order ret_chpl5;
#line 278 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_bool call_tmp_chpl26;
#line 281 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseDist_chpl coerce_tmp_chpl4 = NULL;
#line 281 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  int64_t count_chpl;
#line 281 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  int64_t cnt_chpl;
#line 281 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  _ref_chpl_LocalSpinlock_chpl call_tmp_chpl27 = NULL;
#line 281 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_bool tmp_chpl4;
#line 281 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  _ref_AtomicBool_chpl call_tmp_chpl28 = NULL;
#line 281 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  _ref_atomic_bool_chpl call_tmp_chpl29 = NULL;
#line 281 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  memory_order ret_chpl6;
#line 281 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_bool call_tmp_chpl30;
#line 281 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_bool tmp_chpl5;
#line 281 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  _ref_AtomicBool_chpl call_tmp_chpl31 = NULL;
#line 281 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  _ref_atomic_bool_chpl call_tmp_chpl32 = NULL;
#line 281 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  memory_order ret_chpl7;
#line 281 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_bool call_tmp_chpl33;
#line 281 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  _ref_AtomicBool_chpl call_tmp_chpl34 = NULL;
#line 281 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  _ref_atomic_bool_chpl call_tmp_chpl35 = NULL;
#line 281 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  memory_order ret_chpl8;
#line 281 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_bool call_tmp_chpl36;
#line 281 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_bool tmp_chpl6;
#line 281 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  _ref_AtomicBool_chpl call_tmp_chpl37 = NULL;
#line 281 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  _ref_atomic_bool_chpl call_tmp_chpl38 = NULL;
#line 281 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  memory_order ret_chpl9;
#line 281 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_bool call_tmp_chpl39;
#line 281 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  _ref_int64_t_chpl call_tmp_chpl40 = NULL;
#line 281 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  _ref_int64_t_chpl i_lhs_chpl = NULL;
#line 281 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  _ref_chpl_LocalSpinlock_chpl call_tmp_chpl41 = NULL;
#line 281 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  _ref_AtomicBool_chpl call_tmp_chpl42 = NULL;
#line 281 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  _ref_atomic_bool_chpl call_tmp_chpl43 = NULL;
#line 281 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  memory_order ret_chpl10;
#line 289 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  _tuple_2_BaseDom_BaseDist_chpl ret_tmp_chpl;
#line 256 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 256 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  call_tmp_chpl6 = ((BaseDist_chpl(*)(BaseDom_chpl,
#line 256 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                      int64_t,
#line 256 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                      int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(this_chpl31))->chpl__cid) + INT64(2))])(this_chpl31, _ln_chpl, _fn_chpl);
#line 256 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(call_tmp_chpl6, _ln_chpl, _fn_chpl);
#line 256 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  call_tmp_chpl7 = ((chpl_bool(*)(BaseDist_chpl))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(call_tmp_chpl6))->chpl__cid) + INT64(2))])(call_tmp_chpl6);
#line 256 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 256 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  call_tmp_chpl8 = ((chpl_bool(*)(BaseDom_chpl))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(this_chpl31))->chpl__cid) + INT64(1))])(this_chpl31);
#line 256 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  assert_chpl((call_tmp_chpl7 == call_tmp_chpl8), _ln_chpl, _fn_chpl);
#line 259 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  init_coerce_tmp_chpl = ((BaseDom_chpl)(nil));
#line 259 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  ret_dom_chpl = init_coerce_tmp_chpl;
#line 260 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  init_coerce_tmp_chpl2 = ((BaseDist_chpl)(nil));
#line 260 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  ret_dist_chpl = init_coerce_tmp_chpl2;
#line 261 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 261 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  call_tmp_chpl9 = ((BaseDist_chpl(*)(BaseDom_chpl,
#line 261 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                      int64_t,
#line 261 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                      int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(this_chpl31))->chpl__cid) + INT64(2))])(this_chpl31, _ln_chpl, _fn_chpl);
#line 261 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  dist_chpl = call_tmp_chpl9;
#line 262 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  free_dom_chpl = UINT8(false);
#line 263 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  remove_dist_chpl = UINT8(false);
#line 269 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 269 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  call_tmp_chpl10 = &((this_chpl31)->_arrsLock);
#line 269 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  call_tmp_chpl11 = &((call_tmp_chpl10)->l);
#line 269 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_rmem_consist_maybe_release2(_ln_chpl, _fn_chpl);
#line 269 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  call_tmp_chpl12 = &((call_tmp_chpl11)->_v);
#line 269 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  ret_chpl = memory_order_seq_cst;
#line 269 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  call_tmp_chpl13 = atomic_load_explicit_bool(call_tmp_chpl12, ret_chpl);
#line 269 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  if (call_tmp_chpl13) /* ZLINE: 269 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl */
#line 269 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  {
#line 269 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    tmp_chpl2 = UINT8(true);
#line 269 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  } else /* ZLINE: 269 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl */
#line 269 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  {
#line 269 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    call_tmp_chpl14 = &((call_tmp_chpl10)->l);
#line 269 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    chpl_rmem_consist_maybe_release5(_ln_chpl, _fn_chpl);
#line 269 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    chpl_rmem_consist_maybe_release5(_ln_chpl, _fn_chpl);
#line 269 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    call_tmp_chpl15 = &((call_tmp_chpl14)->_v);
#line 269 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    ret_chpl2 = memory_order_acquire;
#line 269 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    call_tmp_chpl16 = atomic_exchange_explicit_bool(call_tmp_chpl15, UINT8(true), ret_chpl2);
#line 269 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    chpl_rmem_consist_maybe_acquire5(_ln_chpl, _fn_chpl);
#line 269 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    tmp_chpl2 = call_tmp_chpl16;
#line 269 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  }
#line 269 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  tmp_chpl = tmp_chpl2;
#line 269 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  while (tmp_chpl) {
#line 269 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    chpl_task_yield();
#line 269 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    call_tmp_chpl17 = &((call_tmp_chpl10)->l);
#line 269 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    chpl_rmem_consist_maybe_release2(_ln_chpl, _fn_chpl);
#line 269 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    call_tmp_chpl18 = &((call_tmp_chpl17)->_v);
#line 269 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    ret_chpl3 = memory_order_seq_cst;
#line 269 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    call_tmp_chpl19 = atomic_load_explicit_bool(call_tmp_chpl18, ret_chpl3);
#line 269 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    if (call_tmp_chpl19) /* ZLINE: 269 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl */
#line 269 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    {
#line 269 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      tmp_chpl3 = UINT8(true);
#line 269 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    } else /* ZLINE: 269 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl */
#line 269 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    {
#line 269 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      call_tmp_chpl20 = &((call_tmp_chpl10)->l);
#line 269 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      chpl_rmem_consist_maybe_release5(_ln_chpl, _fn_chpl);
#line 269 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      chpl_rmem_consist_maybe_release5(_ln_chpl, _fn_chpl);
#line 269 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      call_tmp_chpl21 = &((call_tmp_chpl20)->_v);
#line 269 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      ret_chpl4 = memory_order_acquire;
#line 269 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      call_tmp_chpl22 = atomic_exchange_explicit_bool(call_tmp_chpl21, UINT8(true), ret_chpl4);
#line 269 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      chpl_rmem_consist_maybe_acquire5(_ln_chpl, _fn_chpl);
#line 269 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      tmp_chpl3 = call_tmp_chpl22;
#line 269 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    }
#line 269 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    tmp_chpl = tmp_chpl3;
#line 269 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  }
#line 270 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 270 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  coerce_tmp_chpl3 = (this_chpl31)->_arrs_containing_dom;
#line 271 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 271 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  (this_chpl31)->_free_when_no_arrs = UINT8(true);
#line 272 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 272 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  call_tmp_chpl23 = &((this_chpl31)->_arrsLock);
#line 272 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  call_tmp_chpl24 = &((call_tmp_chpl23)->l);
#line 272 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_rmem_consist_maybe_release4(_ln_chpl, _fn_chpl);
#line 272 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_rmem_consist_maybe_release4(_ln_chpl, _fn_chpl);
#line 272 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  call_tmp_chpl25 = &((call_tmp_chpl24)->_v);
#line 272 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  ret_chpl5 = memory_order_release;
#line 272 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  atomic_store_explicit_bool(call_tmp_chpl25, UINT8(false), ret_chpl5);
#line 272 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_rmem_consist_maybe_acquire4(_ln_chpl, _fn_chpl);
#line 272 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_rmem_consist_maybe_acquire4(_ln_chpl, _fn_chpl);
#line 275 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  if (coerce_tmp_chpl3 == INT64(0)) /* ZLINE: 275 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl */
#line 275 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  {
#line 276 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    free_dom_chpl = UINT8(true);
#line 278 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 278 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    call_tmp_chpl26 = ((chpl_bool(*)(BaseDom_chpl))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(this_chpl31))->chpl__cid) + INT64(1))])(this_chpl31);
#line 278 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    if (call_tmp_chpl26) /* ZLINE: 278 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl */
#line 278 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    {
#line 281 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      coerce_tmp_chpl4 = dist_chpl;
#line 281 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      count_chpl = INT64(-1);
#line 281 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      cnt_chpl = INT64(-1);
#line 281 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      chpl_check_nil(coerce_tmp_chpl4, _ln_chpl, _fn_chpl);
#line 281 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      call_tmp_chpl27 = &((coerce_tmp_chpl4)->_domsLock);
#line 281 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      call_tmp_chpl28 = &((call_tmp_chpl27)->l);
#line 281 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      chpl_rmem_consist_maybe_release2(_ln_chpl, _fn_chpl);
#line 281 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      call_tmp_chpl29 = &((call_tmp_chpl28)->_v);
#line 281 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      ret_chpl6 = memory_order_seq_cst;
#line 281 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      call_tmp_chpl30 = atomic_load_explicit_bool(call_tmp_chpl29, ret_chpl6);
#line 281 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      if (call_tmp_chpl30) /* ZLINE: 281 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl */
#line 281 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      {
#line 281 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
        tmp_chpl5 = UINT8(true);
#line 281 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      } else /* ZLINE: 281 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl */
#line 281 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      {
#line 281 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
        call_tmp_chpl31 = &((call_tmp_chpl27)->l);
#line 281 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
        chpl_rmem_consist_maybe_release5(_ln_chpl, _fn_chpl);
#line 281 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
        chpl_rmem_consist_maybe_release5(_ln_chpl, _fn_chpl);
#line 281 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
        call_tmp_chpl32 = &((call_tmp_chpl31)->_v);
#line 281 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
        ret_chpl7 = memory_order_acquire;
#line 281 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
        call_tmp_chpl33 = atomic_exchange_explicit_bool(call_tmp_chpl32, UINT8(true), ret_chpl7);
#line 281 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
        chpl_rmem_consist_maybe_acquire5(_ln_chpl, _fn_chpl);
#line 281 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
        tmp_chpl5 = call_tmp_chpl33;
#line 281 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      }
#line 281 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      tmp_chpl4 = tmp_chpl5;
#line 281 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      while (tmp_chpl4) {
#line 281 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
        chpl_task_yield();
#line 281 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
        call_tmp_chpl34 = &((call_tmp_chpl27)->l);
#line 281 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
        chpl_rmem_consist_maybe_release2(_ln_chpl, _fn_chpl);
#line 281 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
        call_tmp_chpl35 = &((call_tmp_chpl34)->_v);
#line 281 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
        ret_chpl8 = memory_order_seq_cst;
#line 281 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
        call_tmp_chpl36 = atomic_load_explicit_bool(call_tmp_chpl35, ret_chpl8);
#line 281 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
        if (call_tmp_chpl36) /* ZLINE: 281 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl */
#line 281 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
        {
#line 281 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
          tmp_chpl6 = UINT8(true);
#line 281 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
        } else /* ZLINE: 281 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl */
#line 281 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
        {
#line 281 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
          call_tmp_chpl37 = &((call_tmp_chpl27)->l);
#line 281 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
          chpl_rmem_consist_maybe_release5(_ln_chpl, _fn_chpl);
#line 281 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
          chpl_rmem_consist_maybe_release5(_ln_chpl, _fn_chpl);
#line 281 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
          call_tmp_chpl38 = &((call_tmp_chpl37)->_v);
#line 281 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
          ret_chpl9 = memory_order_acquire;
#line 281 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
          call_tmp_chpl39 = atomic_exchange_explicit_bool(call_tmp_chpl38, UINT8(true), ret_chpl9);
#line 281 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
          chpl_rmem_consist_maybe_acquire5(_ln_chpl, _fn_chpl);
#line 281 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
          tmp_chpl6 = call_tmp_chpl39;
#line 281 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
        }
#line 281 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
        tmp_chpl4 = tmp_chpl6;
#line 281 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      }
#line 281 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      chpl_check_nil(coerce_tmp_chpl4, _ln_chpl, _fn_chpl);
#line 281 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      call_tmp_chpl40 = &((coerce_tmp_chpl4)->_doms_containing_dist);
#line 281 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      *(call_tmp_chpl40) -= INT64(1);
#line 281 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      chpl_check_nil(coerce_tmp_chpl4, _ln_chpl, _fn_chpl);
#line 281 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      cnt_chpl = (coerce_tmp_chpl4)->_doms_containing_dist;
#line 281 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      chpl_check_nil(coerce_tmp_chpl4, _ln_chpl, _fn_chpl);
#line 281 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      if (! (coerce_tmp_chpl4)->_free_when_no_doms) /* ZLINE: 281 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl */
#line 281 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      {
#line 281 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
        i_lhs_chpl = &cnt_chpl;
#line 281 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
        *(i_lhs_chpl) += INT64(1);
#line 281 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      }
#line 281 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      chpl_check_nil(coerce_tmp_chpl4, _ln_chpl, _fn_chpl);
#line 281 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      call_tmp_chpl41 = &((coerce_tmp_chpl4)->_domsLock);
#line 281 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      call_tmp_chpl42 = &((call_tmp_chpl41)->l);
#line 281 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      chpl_rmem_consist_maybe_release4(_ln_chpl, _fn_chpl);
#line 281 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      chpl_rmem_consist_maybe_release4(_ln_chpl, _fn_chpl);
#line 281 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      call_tmp_chpl43 = &((call_tmp_chpl42)->_v);
#line 281 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      ret_chpl10 = memory_order_release;
#line 281 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      atomic_store_explicit_bool(call_tmp_chpl43, UINT8(false), ret_chpl10);
#line 281 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      chpl_rmem_consist_maybe_acquire4(_ln_chpl, _fn_chpl);
#line 281 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      chpl_rmem_consist_maybe_acquire4(_ln_chpl, _fn_chpl);
#line 281 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      count_chpl = cnt_chpl;
#line 281 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      remove_dist_chpl = (count_chpl == INT64(0));
#line 281 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    }
#line 281 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  }
#line 285 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  if (free_dom_chpl) /* ZLINE: 285 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl */
#line 285 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  {
#line 286 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    ret_dom_chpl = this_chpl31;
#line 286 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  }
#line 287 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  if (remove_dist_chpl) /* ZLINE: 287 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl */
#line 287 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  {
#line 288 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    ret_dist_chpl = dist_chpl;
#line 288 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  }
#line 289 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  (&ret_tmp_chpl)->x0 = ret_dom_chpl;
#line 289 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  (&ret_tmp_chpl)->x1 = ret_dist_chpl;
#line 252 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  *(_retArg_chpl) = ret_tmp_chpl;
#line 252 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  return;
#line 252 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
}

#line 375 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
/* ChapelDistribution.chpl:375 */
#line 375 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
static chpl_bool dsiLinksDistribution_chpl(BaseDom_chpl this_chpl31) {
#line 375 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  return UINT8(true);
#line 375 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
}

#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
/* ChapelDistribution.chpl:378 */
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
static void dsiDestroyDom_chpl5(BaseDom_chpl this_chpl31,
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                                int64_t _ln_chpl,
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                                int32_t _fn_chpl) {
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  return;
#line 378 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
}

#line 427 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
/* ChapelDistribution.chpl:427 */
#line 427 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
static void init_chpl49(BaseRectangularDom_1_int64_t_one_chpl this_chpl31,
#line 427 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        int64_t _ln_chpl,
#line 427 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        int32_t _fn_chpl) {
#line 427 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseDom_chpl super_tmp_chpl = NULL;
#line 427 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseArr_chpl _arrs_head_chpl = NULL;
#line 427 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_LocalSpinlock _arrsLock_chpl;
#line 427 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 427 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  super_tmp_chpl = &((this_chpl31)->super);
#line 427 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 427 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  ((RootClass_chpl)(&((super_tmp_chpl)->super)))->chpl__cid = chpl__cid_RootClass_chpl;
#line 427 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  ((RootClass_chpl)(((RootClass_chpl)(super_tmp_chpl))))->chpl__cid = chpl__cid_RootClass_chpl;
#line 427 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  _arrs_head_chpl = ((BaseArr_chpl)(nil));
#line 427 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 427 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  (super_tmp_chpl)->_arrs_head = _arrs_head_chpl;
#line 427 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 427 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  (super_tmp_chpl)->_arrs_containing_dom = INT64(0);
#line 427 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  init_chpl105(&_arrsLock_chpl);
#line 427 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 427 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  (super_tmp_chpl)->_arrsLock = _arrsLock_chpl;
#line 427 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 427 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  (super_tmp_chpl)->_free_when_no_arrs = UINT8(false);
#line 427 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 427 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  (super_tmp_chpl)->pid = INT64(-1);
#line 427 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 427 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  (super_tmp_chpl)->definedConst = UINT8(false);
#line 427 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  ((RootClass_chpl)(super_tmp_chpl))->chpl__cid = chpl__cid_BaseDom_chpl;
#line 427 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  ((RootClass_chpl)(((BaseDom_chpl)(this_chpl31))))->chpl__cid = chpl__cid_BaseDom_chpl;
#line 427 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  ((RootClass_chpl)(this_chpl31))->chpl__cid = chpl__cid_BaseRectangularDom_1_int64_t_one_chpl;
#line 427 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  return;
#line 427 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
}

#line 427 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
/* ChapelDistribution.chpl:427 */
#line 427 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
static void init_chpl50(BaseRectangularDom_4_int64_t_one_chpl this_chpl31,
#line 427 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        int64_t _ln_chpl,
#line 427 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        int32_t _fn_chpl) {
#line 427 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseDom_chpl super_tmp_chpl = NULL;
#line 427 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseArr_chpl _arrs_head_chpl = NULL;
#line 427 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_LocalSpinlock _arrsLock_chpl;
#line 427 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 427 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  super_tmp_chpl = &((this_chpl31)->super);
#line 427 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 427 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  ((RootClass_chpl)(&((super_tmp_chpl)->super)))->chpl__cid = chpl__cid_RootClass_chpl;
#line 427 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  ((RootClass_chpl)(((RootClass_chpl)(super_tmp_chpl))))->chpl__cid = chpl__cid_RootClass_chpl;
#line 427 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  _arrs_head_chpl = ((BaseArr_chpl)(nil));
#line 427 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 427 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  (super_tmp_chpl)->_arrs_head = _arrs_head_chpl;
#line 427 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 427 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  (super_tmp_chpl)->_arrs_containing_dom = INT64(0);
#line 427 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  init_chpl105(&_arrsLock_chpl);
#line 427 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 427 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  (super_tmp_chpl)->_arrsLock = _arrsLock_chpl;
#line 427 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 427 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  (super_tmp_chpl)->_free_when_no_arrs = UINT8(false);
#line 427 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 427 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  (super_tmp_chpl)->pid = INT64(-1);
#line 427 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 427 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  (super_tmp_chpl)->definedConst = UINT8(false);
#line 427 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  ((RootClass_chpl)(super_tmp_chpl))->chpl__cid = chpl__cid_BaseDom_chpl;
#line 427 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  ((RootClass_chpl)(((BaseDom_chpl)(this_chpl31))))->chpl__cid = chpl__cid_BaseDom_chpl;
#line 427 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  ((RootClass_chpl)(this_chpl31))->chpl__cid = chpl__cid_BaseRectangularDom_4_int64_t_one_chpl;
#line 427 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  return;
#line 427 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
}

#line 427 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
/* ChapelDistribution.chpl:427 */
#line 427 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
static void init_chpl52(BaseRectangularDom_2_int64_t_one_chpl this_chpl31,
#line 427 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        int64_t _ln_chpl,
#line 427 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        int32_t _fn_chpl) {
#line 427 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseDom_chpl super_tmp_chpl = NULL;
#line 427 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseArr_chpl _arrs_head_chpl = NULL;
#line 427 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_LocalSpinlock _arrsLock_chpl;
#line 427 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 427 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  super_tmp_chpl = &((this_chpl31)->super);
#line 427 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 427 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  ((RootClass_chpl)(&((super_tmp_chpl)->super)))->chpl__cid = chpl__cid_RootClass_chpl;
#line 427 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  ((RootClass_chpl)(((RootClass_chpl)(super_tmp_chpl))))->chpl__cid = chpl__cid_RootClass_chpl;
#line 427 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  _arrs_head_chpl = ((BaseArr_chpl)(nil));
#line 427 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 427 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  (super_tmp_chpl)->_arrs_head = _arrs_head_chpl;
#line 427 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 427 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  (super_tmp_chpl)->_arrs_containing_dom = INT64(0);
#line 427 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  init_chpl105(&_arrsLock_chpl);
#line 427 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 427 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  (super_tmp_chpl)->_arrsLock = _arrsLock_chpl;
#line 427 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 427 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  (super_tmp_chpl)->_free_when_no_arrs = UINT8(false);
#line 427 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 427 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  (super_tmp_chpl)->pid = INT64(-1);
#line 427 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 427 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  (super_tmp_chpl)->definedConst = UINT8(false);
#line 427 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  ((RootClass_chpl)(super_tmp_chpl))->chpl__cid = chpl__cid_BaseDom_chpl;
#line 427 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  ((RootClass_chpl)(((BaseDom_chpl)(this_chpl31))))->chpl__cid = chpl__cid_BaseDom_chpl;
#line 427 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  ((RootClass_chpl)(this_chpl31))->chpl__cid = chpl__cid_BaseRectangularDom_2_int64_t_one_chpl;
#line 427 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  return;
#line 427 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
}

#line 427 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
/* ChapelDistribution.chpl:427 */
#line 427 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
static void init_chpl51(BaseRectangularDom_3_int64_t_one_chpl this_chpl31,
#line 427 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        int64_t _ln_chpl,
#line 427 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        int32_t _fn_chpl) {
#line 427 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseDom_chpl super_tmp_chpl = NULL;
#line 427 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseArr_chpl _arrs_head_chpl = NULL;
#line 427 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_LocalSpinlock _arrsLock_chpl;
#line 427 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 427 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  super_tmp_chpl = &((this_chpl31)->super);
#line 427 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 427 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  ((RootClass_chpl)(&((super_tmp_chpl)->super)))->chpl__cid = chpl__cid_RootClass_chpl;
#line 427 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  ((RootClass_chpl)(((RootClass_chpl)(super_tmp_chpl))))->chpl__cid = chpl__cid_RootClass_chpl;
#line 427 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  _arrs_head_chpl = ((BaseArr_chpl)(nil));
#line 427 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 427 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  (super_tmp_chpl)->_arrs_head = _arrs_head_chpl;
#line 427 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 427 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  (super_tmp_chpl)->_arrs_containing_dom = INT64(0);
#line 427 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  init_chpl105(&_arrsLock_chpl);
#line 427 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 427 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  (super_tmp_chpl)->_arrsLock = _arrsLock_chpl;
#line 427 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 427 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  (super_tmp_chpl)->_free_when_no_arrs = UINT8(false);
#line 427 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 427 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  (super_tmp_chpl)->pid = INT64(-1);
#line 427 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 427 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  (super_tmp_chpl)->definedConst = UINT8(false);
#line 427 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  ((RootClass_chpl)(super_tmp_chpl))->chpl__cid = chpl__cid_BaseDom_chpl;
#line 427 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  ((RootClass_chpl)(((BaseDom_chpl)(this_chpl31))))->chpl__cid = chpl__cid_BaseDom_chpl;
#line 427 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  ((RootClass_chpl)(this_chpl31))->chpl__cid = chpl__cid_BaseRectangularDom_3_int64_t_one_chpl;
#line 427 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  return;
#line 427 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
}

#line 448 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
/* ChapelDistribution.chpl:448 */
#line 448 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
static void deinit_chpl33(BaseRectangularDom_1_int64_t_one_chpl this_chpl31,
#line 448 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                          int64_t _ln_chpl,
#line 448 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                          int32_t _fn_chpl) {
#line 448 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  deinit_chpl31(((BaseDom_chpl)(this_chpl31)), _ln_chpl, _fn_chpl);
#line 448 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  return;
#line 448 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
}

#line 448 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
/* ChapelDistribution.chpl:448 */
#line 448 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
static void deinit_chpl32(BaseRectangularDom_4_int64_t_one_chpl this_chpl31,
#line 448 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                          int64_t _ln_chpl,
#line 448 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                          int32_t _fn_chpl) {
#line 448 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  deinit_chpl31(((BaseDom_chpl)(this_chpl31)), _ln_chpl, _fn_chpl);
#line 448 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  return;
#line 448 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
}

#line 448 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
/* ChapelDistribution.chpl:448 */
#line 448 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
static void deinit_chpl34(BaseRectangularDom_2_int64_t_one_chpl this_chpl31,
#line 448 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                          int64_t _ln_chpl,
#line 448 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                          int32_t _fn_chpl) {
#line 448 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  deinit_chpl31(((BaseDom_chpl)(this_chpl31)), _ln_chpl, _fn_chpl);
#line 448 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  return;
#line 448 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
}

#line 448 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
/* ChapelDistribution.chpl:448 */
#line 448 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
static void deinit_chpl35(BaseRectangularDom_3_int64_t_one_chpl this_chpl31,
#line 448 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                          int64_t _ln_chpl,
#line 448 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                          int32_t _fn_chpl) {
#line 448 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  deinit_chpl31(((BaseDom_chpl)(this_chpl31)), _ln_chpl, _fn_chpl);
#line 448 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  return;
#line 448 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
}

#line 801 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
/* ChapelDistribution.chpl:801 */
#line 801 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
static void deinit_chpl36(BaseArr_chpl this_chpl31,
#line 801 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                          int64_t _ln_chpl,
#line 801 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                          int32_t _fn_chpl) {
#line 801 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  return;
#line 801 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
}

#line 806 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
/* ChapelDistribution.chpl:806 */
#line 806 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
static BaseDom_chpl dsiGetBaseDom_chpl12(BaseArr_chpl this_chpl31,
#line 806 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                                         int64_t _ln_chpl,
#line 806 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                                         int32_t _fn_chpl) {
#line 808 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseDom_chpl ret_chpl = NULL;
#line 807 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  halt_chpl(&_str_literal_1149_chpl, _ln_chpl, _fn_chpl);
#line 808 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  ret_chpl = ((BaseDom_chpl)(nil));
#line 806 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  return ret_chpl;
#line 806 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
}

#line 820 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
/* ChapelDistribution.chpl:820 */
#line 820 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
static void remove_chpl4(BaseArr_chpl this_chpl31,
#line 820 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                         _tuple_2_BaseArr_BaseDom_chpl * _retArg_chpl,
#line 820 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                         int64_t _ln_chpl,
#line 820 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                         int32_t _fn_chpl) {
#line 821 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseArr_chpl ret_arr_chpl = NULL;
#line 822 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseDom_chpl ret_dom_chpl = NULL;
#line 822 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseDom_chpl init_coerce_tmp_chpl = NULL;
#line 825 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseDom_chpl call_tmp_chpl6 = NULL;
#line 825 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseDom_chpl dom_chpl = NULL;
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseArr_chpl call_tmp_chpl7 = NULL;
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseDom_chpl coerce_tmp_chpl3 = NULL;
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  int64_t count_chpl;
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  int64_t cnt_chpl;
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  _ref_chpl_LocalSpinlock_chpl call_tmp_chpl8 = NULL;
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_bool tmp_chpl;
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  _ref_AtomicBool_chpl call_tmp_chpl9 = NULL;
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  _ref_atomic_bool_chpl call_tmp_chpl10 = NULL;
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  memory_order ret_chpl;
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_bool call_tmp_chpl11;
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_bool tmp_chpl2;
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  _ref_AtomicBool_chpl call_tmp_chpl12 = NULL;
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  _ref_atomic_bool_chpl call_tmp_chpl13 = NULL;
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  memory_order ret_chpl2;
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_bool call_tmp_chpl14;
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  _ref_AtomicBool_chpl call_tmp_chpl15 = NULL;
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  _ref_atomic_bool_chpl call_tmp_chpl16 = NULL;
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  memory_order ret_chpl3;
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_bool call_tmp_chpl17;
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_bool tmp_chpl3;
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  _ref_AtomicBool_chpl call_tmp_chpl18 = NULL;
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  _ref_atomic_bool_chpl call_tmp_chpl19 = NULL;
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  memory_order ret_chpl4;
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_bool call_tmp_chpl20;
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  _ref_int64_t_chpl call_tmp_chpl21 = NULL;
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseArr_chpl coerce_tmp_chpl4 = NULL;
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseArr_chpl coerce_tmp_chpl5 = NULL;
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseArr_chpl coerce_tmp_chpl6 = NULL;
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseArr_chpl coerce_tmp_chpl7 = NULL;
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseArr_chpl coerce_tmp_chpl8 = NULL;
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseArr_chpl coerce_tmp_chpl9 = NULL;
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseArr_chpl coerce_tmp_chpl10 = NULL;
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseArr_chpl coerce_tmp_chpl11 = NULL;
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseArr_chpl coerce_tmp_chpl12 = NULL;
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseArr_chpl coerce_tmp_chpl13 = NULL;
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseArr_chpl coerce_tmp_chpl14 = NULL;
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseArr_chpl coerce_tmp_chpl15 = NULL;
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  _ref_int64_t_chpl i_lhs_chpl = NULL;
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  _ref_chpl_LocalSpinlock_chpl call_tmp_chpl22 = NULL;
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  _ref_AtomicBool_chpl call_tmp_chpl23 = NULL;
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  _ref_atomic_bool_chpl call_tmp_chpl24 = NULL;
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  memory_order ret_chpl5;
#line 833 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  _tuple_2_BaseArr_BaseDom_chpl ret_tmp_chpl;
#line 821 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  ret_arr_chpl = this_chpl31;
#line 822 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  init_coerce_tmp_chpl = ((BaseDom_chpl)(nil));
#line 822 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  ret_dom_chpl = init_coerce_tmp_chpl;
#line 825 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 825 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  call_tmp_chpl6 = ((BaseDom_chpl(*)(BaseArr_chpl,
#line 825 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                      int64_t,
#line 825 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                      int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(this_chpl31))->chpl__cid) + INT64(1))])(this_chpl31, _ln_chpl, _fn_chpl);
#line 825 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  dom_chpl = call_tmp_chpl6;
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  call_tmp_chpl7 = this_chpl31;
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  coerce_tmp_chpl3 = call_tmp_chpl6;
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  count_chpl = INT64(-1);
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  cnt_chpl = INT64(-1);
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  call_tmp_chpl8 = &((coerce_tmp_chpl3)->_arrsLock);
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  call_tmp_chpl9 = &((call_tmp_chpl8)->l);
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_rmem_consist_maybe_release2(_ln_chpl, _fn_chpl);
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  call_tmp_chpl10 = &((call_tmp_chpl9)->_v);
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  ret_chpl = memory_order_seq_cst;
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  call_tmp_chpl11 = atomic_load_explicit_bool(call_tmp_chpl10, ret_chpl);
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  if (call_tmp_chpl11) /* ZLINE: 828 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl */
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  {
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    tmp_chpl2 = UINT8(true);
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  } else /* ZLINE: 828 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl */
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  {
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    call_tmp_chpl12 = &((call_tmp_chpl8)->l);
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    chpl_rmem_consist_maybe_release5(_ln_chpl, _fn_chpl);
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    chpl_rmem_consist_maybe_release5(_ln_chpl, _fn_chpl);
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    call_tmp_chpl13 = &((call_tmp_chpl12)->_v);
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    ret_chpl2 = memory_order_acquire;
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    call_tmp_chpl14 = atomic_exchange_explicit_bool(call_tmp_chpl13, UINT8(true), ret_chpl2);
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    chpl_rmem_consist_maybe_acquire5(_ln_chpl, _fn_chpl);
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    tmp_chpl2 = call_tmp_chpl14;
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  }
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  tmp_chpl = tmp_chpl2;
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  while (tmp_chpl) {
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    chpl_task_yield();
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    call_tmp_chpl15 = &((call_tmp_chpl8)->l);
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    chpl_rmem_consist_maybe_release2(_ln_chpl, _fn_chpl);
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    call_tmp_chpl16 = &((call_tmp_chpl15)->_v);
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    ret_chpl3 = memory_order_seq_cst;
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    call_tmp_chpl17 = atomic_load_explicit_bool(call_tmp_chpl16, ret_chpl3);
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    if (call_tmp_chpl17) /* ZLINE: 828 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl */
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    {
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      tmp_chpl3 = UINT8(true);
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    } else /* ZLINE: 828 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl */
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    {
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      call_tmp_chpl18 = &((call_tmp_chpl8)->l);
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      chpl_rmem_consist_maybe_release5(_ln_chpl, _fn_chpl);
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      chpl_rmem_consist_maybe_release5(_ln_chpl, _fn_chpl);
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      call_tmp_chpl19 = &((call_tmp_chpl18)->_v);
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      ret_chpl4 = memory_order_acquire;
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      call_tmp_chpl20 = atomic_exchange_explicit_bool(call_tmp_chpl19, UINT8(true), ret_chpl4);
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      chpl_rmem_consist_maybe_acquire5(_ln_chpl, _fn_chpl);
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      tmp_chpl3 = call_tmp_chpl20;
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    }
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    tmp_chpl = tmp_chpl3;
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  }
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  call_tmp_chpl21 = &((coerce_tmp_chpl3)->_arrs_containing_dom);
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  *(call_tmp_chpl21) -= INT64(1);
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  if (! (coerce_tmp_chpl3)->definedConst) /* ZLINE: 828 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl */
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  {
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    chpl_check_nil(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    if (((RootClass_chpl)((coerce_tmp_chpl3)->_arrs_head)) == ((RootClass_chpl)(call_tmp_chpl7))) /* ZLINE: 828 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl */
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    {
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      coerce_tmp_chpl4 = call_tmp_chpl7;
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      chpl_check_nil(coerce_tmp_chpl4, _ln_chpl, _fn_chpl);
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      coerce_tmp_chpl5 = (coerce_tmp_chpl4)->next;
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      chpl_check_nil(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      (coerce_tmp_chpl3)->_arrs_head = coerce_tmp_chpl5;
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    }
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    coerce_tmp_chpl6 = call_tmp_chpl7;
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    chpl_check_nil(coerce_tmp_chpl6, _ln_chpl, _fn_chpl);
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    coerce_tmp_chpl7 = (coerce_tmp_chpl6)->next;
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    if (((RootClass_chpl)(coerce_tmp_chpl7)) != nil) /* ZLINE: 828 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl */
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    {
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      coerce_tmp_chpl8 = coerce_tmp_chpl7;
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      coerce_tmp_chpl9 = call_tmp_chpl7;
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      chpl_check_nil(coerce_tmp_chpl9, _ln_chpl, _fn_chpl);
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      coerce_tmp_chpl10 = (coerce_tmp_chpl9)->prev;
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      chpl_check_nil(coerce_tmp_chpl8, _ln_chpl, _fn_chpl);
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      (coerce_tmp_chpl8)->prev = coerce_tmp_chpl10;
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    }
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    coerce_tmp_chpl11 = call_tmp_chpl7;
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    chpl_check_nil(coerce_tmp_chpl11, _ln_chpl, _fn_chpl);
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    coerce_tmp_chpl12 = (coerce_tmp_chpl11)->prev;
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    if (((RootClass_chpl)(coerce_tmp_chpl12)) != nil) /* ZLINE: 828 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl */
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    {
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      coerce_tmp_chpl13 = coerce_tmp_chpl12;
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      coerce_tmp_chpl14 = call_tmp_chpl7;
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      chpl_check_nil(coerce_tmp_chpl14, _ln_chpl, _fn_chpl);
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      coerce_tmp_chpl15 = (coerce_tmp_chpl14)->next;
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      chpl_check_nil(coerce_tmp_chpl13, _ln_chpl, _fn_chpl);
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      (coerce_tmp_chpl13)->next = coerce_tmp_chpl15;
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    }
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  }
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  cnt_chpl = (coerce_tmp_chpl3)->_arrs_containing_dom;
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  if (! (coerce_tmp_chpl3)->_free_when_no_arrs) /* ZLINE: 828 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl */
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  {
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    i_lhs_chpl = &cnt_chpl;
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    *(i_lhs_chpl) += INT64(1);
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  }
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  call_tmp_chpl22 = &((coerce_tmp_chpl3)->_arrsLock);
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  call_tmp_chpl23 = &((call_tmp_chpl22)->l);
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_rmem_consist_maybe_release4(_ln_chpl, _fn_chpl);
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_rmem_consist_maybe_release4(_ln_chpl, _fn_chpl);
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  call_tmp_chpl24 = &((call_tmp_chpl23)->_v);
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  ret_chpl5 = memory_order_release;
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  atomic_store_explicit_bool(call_tmp_chpl24, UINT8(false), ret_chpl5);
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_rmem_consist_maybe_acquire4(_ln_chpl, _fn_chpl);
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_rmem_consist_maybe_acquire4(_ln_chpl, _fn_chpl);
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  count_chpl = cnt_chpl;
#line 830 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  if (count_chpl == INT64(0)) /* ZLINE: 830 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl */
#line 830 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  {
#line 831 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    ret_dom_chpl = dom_chpl;
#line 831 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  }
#line 833 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  (&ret_tmp_chpl)->x0 = ret_arr_chpl;
#line 833 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  (&ret_tmp_chpl)->x1 = ret_dom_chpl;
#line 820 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  *(_retArg_chpl) = ret_tmp_chpl;
#line 820 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  return;
#line 820 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
}

#line 820 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
/* ChapelDistribution.chpl:820 */
#line 820 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
static void remove_chpl3(BaseArr_chpl this_chpl31,
#line 820 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                         _tuple_2_BaseArr_BaseDom_chpl * _retArg_chpl,
#line 820 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                         int64_t _ln_chpl,
#line 820 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                         int32_t _fn_chpl) {
#line 821 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseArr_chpl ret_arr_chpl = NULL;
#line 822 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseDom_chpl ret_dom_chpl = NULL;
#line 822 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseDom_chpl init_coerce_tmp_chpl = NULL;
#line 825 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseDom_chpl call_tmp_chpl6 = NULL;
#line 825 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseDom_chpl dom_chpl = NULL;
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseDom_chpl coerce_tmp_chpl3 = NULL;
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  int64_t count_chpl;
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  int64_t cnt_chpl;
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  _ref_chpl_LocalSpinlock_chpl call_tmp_chpl7 = NULL;
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_bool tmp_chpl;
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  _ref_AtomicBool_chpl call_tmp_chpl8 = NULL;
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  _ref_atomic_bool_chpl call_tmp_chpl9 = NULL;
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  memory_order ret_chpl;
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_bool call_tmp_chpl10;
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_bool tmp_chpl2;
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  _ref_AtomicBool_chpl call_tmp_chpl11 = NULL;
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  _ref_atomic_bool_chpl call_tmp_chpl12 = NULL;
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  memory_order ret_chpl2;
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_bool call_tmp_chpl13;
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  _ref_AtomicBool_chpl call_tmp_chpl14 = NULL;
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  _ref_atomic_bool_chpl call_tmp_chpl15 = NULL;
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  memory_order ret_chpl3;
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_bool call_tmp_chpl16;
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_bool tmp_chpl3;
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  _ref_AtomicBool_chpl call_tmp_chpl17 = NULL;
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  _ref_atomic_bool_chpl call_tmp_chpl18 = NULL;
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  memory_order ret_chpl4;
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_bool call_tmp_chpl19;
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  _ref_int64_t_chpl call_tmp_chpl20 = NULL;
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  _ref_int64_t_chpl i_lhs_chpl = NULL;
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  _ref_chpl_LocalSpinlock_chpl call_tmp_chpl21 = NULL;
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  _ref_AtomicBool_chpl call_tmp_chpl22 = NULL;
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  _ref_atomic_bool_chpl call_tmp_chpl23 = NULL;
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  memory_order ret_chpl5;
#line 833 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  _tuple_2_BaseArr_BaseDom_chpl ret_tmp_chpl;
#line 821 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  ret_arr_chpl = this_chpl31;
#line 822 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  init_coerce_tmp_chpl = ((BaseDom_chpl)(nil));
#line 822 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  ret_dom_chpl = init_coerce_tmp_chpl;
#line 825 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 825 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  call_tmp_chpl6 = ((BaseDom_chpl(*)(BaseArr_chpl,
#line 825 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                      int64_t,
#line 825 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                      int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(this_chpl31))->chpl__cid) + INT64(1))])(this_chpl31, _ln_chpl, _fn_chpl);
#line 825 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  dom_chpl = call_tmp_chpl6;
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  coerce_tmp_chpl3 = call_tmp_chpl6;
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  count_chpl = INT64(-1);
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  cnt_chpl = INT64(-1);
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  call_tmp_chpl7 = &((coerce_tmp_chpl3)->_arrsLock);
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  call_tmp_chpl8 = &((call_tmp_chpl7)->l);
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_rmem_consist_maybe_release2(_ln_chpl, _fn_chpl);
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  call_tmp_chpl9 = &((call_tmp_chpl8)->_v);
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  ret_chpl = memory_order_seq_cst;
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  call_tmp_chpl10 = atomic_load_explicit_bool(call_tmp_chpl9, ret_chpl);
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  if (call_tmp_chpl10) /* ZLINE: 828 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl */
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  {
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    tmp_chpl2 = UINT8(true);
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  } else /* ZLINE: 828 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl */
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  {
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    call_tmp_chpl11 = &((call_tmp_chpl7)->l);
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    chpl_rmem_consist_maybe_release5(_ln_chpl, _fn_chpl);
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    chpl_rmem_consist_maybe_release5(_ln_chpl, _fn_chpl);
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    call_tmp_chpl12 = &((call_tmp_chpl11)->_v);
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    ret_chpl2 = memory_order_acquire;
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    call_tmp_chpl13 = atomic_exchange_explicit_bool(call_tmp_chpl12, UINT8(true), ret_chpl2);
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    chpl_rmem_consist_maybe_acquire5(_ln_chpl, _fn_chpl);
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    tmp_chpl2 = call_tmp_chpl13;
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  }
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  tmp_chpl = tmp_chpl2;
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  while (tmp_chpl) {
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    chpl_task_yield();
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    call_tmp_chpl14 = &((call_tmp_chpl7)->l);
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    chpl_rmem_consist_maybe_release2(_ln_chpl, _fn_chpl);
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    call_tmp_chpl15 = &((call_tmp_chpl14)->_v);
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    ret_chpl3 = memory_order_seq_cst;
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    call_tmp_chpl16 = atomic_load_explicit_bool(call_tmp_chpl15, ret_chpl3);
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    if (call_tmp_chpl16) /* ZLINE: 828 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl */
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    {
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      tmp_chpl3 = UINT8(true);
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    } else /* ZLINE: 828 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl */
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    {
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      call_tmp_chpl17 = &((call_tmp_chpl7)->l);
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      chpl_rmem_consist_maybe_release5(_ln_chpl, _fn_chpl);
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      chpl_rmem_consist_maybe_release5(_ln_chpl, _fn_chpl);
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      call_tmp_chpl18 = &((call_tmp_chpl17)->_v);
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      ret_chpl4 = memory_order_acquire;
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      call_tmp_chpl19 = atomic_exchange_explicit_bool(call_tmp_chpl18, UINT8(true), ret_chpl4);
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      chpl_rmem_consist_maybe_acquire5(_ln_chpl, _fn_chpl);
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      tmp_chpl3 = call_tmp_chpl19;
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    }
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    tmp_chpl = tmp_chpl3;
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  }
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  call_tmp_chpl20 = &((coerce_tmp_chpl3)->_arrs_containing_dom);
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  *(call_tmp_chpl20) -= INT64(1);
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  cnt_chpl = (coerce_tmp_chpl3)->_arrs_containing_dom;
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  if (! (coerce_tmp_chpl3)->_free_when_no_arrs) /* ZLINE: 828 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl */
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  {
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    i_lhs_chpl = &cnt_chpl;
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    *(i_lhs_chpl) += INT64(1);
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  }
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  call_tmp_chpl21 = &((coerce_tmp_chpl3)->_arrsLock);
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  call_tmp_chpl22 = &((call_tmp_chpl21)->l);
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_rmem_consist_maybe_release4(_ln_chpl, _fn_chpl);
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_rmem_consist_maybe_release4(_ln_chpl, _fn_chpl);
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  call_tmp_chpl23 = &((call_tmp_chpl22)->_v);
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  ret_chpl5 = memory_order_release;
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  atomic_store_explicit_bool(call_tmp_chpl23, UINT8(false), ret_chpl5);
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_rmem_consist_maybe_acquire4(_ln_chpl, _fn_chpl);
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_rmem_consist_maybe_acquire4(_ln_chpl, _fn_chpl);
#line 828 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  count_chpl = cnt_chpl;
#line 830 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  if (count_chpl == INT64(0)) /* ZLINE: 830 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl */
#line 830 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  {
#line 831 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    ret_dom_chpl = dom_chpl;
#line 831 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  }
#line 833 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  (&ret_tmp_chpl)->x0 = ret_arr_chpl;
#line 833 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  (&ret_tmp_chpl)->x1 = ret_dom_chpl;
#line 820 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  *(_retArg_chpl) = ret_tmp_chpl;
#line 820 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  return;
#line 820 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
}

#line 867 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
/* ChapelDistribution.chpl:867 */
#line 867 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
static void dsiDestroyArr_chpl12(BaseArr_chpl this_chpl31,
#line 867 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                                 chpl_bool deinitElts_chpl,
#line 867 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                                 int64_t _ln_chpl,
#line 867 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                                 int32_t _fn_chpl) {
#line 868 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  halt_chpl(&_str_literal_1157_chpl, _ln_chpl, _fn_chpl);
#line 867 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  return;
#line 867 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
}

#line 947 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
/* ChapelDistribution.chpl:947 */
#line 947 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
static void decEltCountsIfNeeded_chpl(BaseArr_chpl this_chpl31,
#line 947 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                                      int64_t _ln_chpl,
#line 947 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                                      int32_t _fn_chpl) {
#line 947 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  return;
#line 947 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
}

#line 963 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
/* ChapelDistribution.chpl:963 */
#line 963 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
static void chpl__auto_destroy_AbsBaseArr2(AbsBaseArr__real64_chpl this_chpl31,
#line 963 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                                           int64_t _ln_chpl,
#line 963 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                                           int32_t _fn_chpl) {
#line 963 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  deinit_chpl36(((BaseArr_chpl)(this_chpl31)), _ln_chpl, _fn_chpl);
#line 963 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  return;
#line 963 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
}

#line 963 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
/* ChapelDistribution.chpl:963 */
#line 963 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
static void init_chpl54(AbsBaseArr__tuple_2_Tensor_3__real64_int64_t_chpl this_chpl31,
#line 963 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        BaseArr_chpl prev_chpl,
#line 963 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        BaseArr_chpl next_chpl,
#line 963 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        int64_t pid_chpl,
#line 963 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        chpl_bool _decEltRefCounts_chpl,
#line 963 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        chpl_ddataResizePolicy _resizePolicy_chpl,
#line 963 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        int64_t _ln_chpl,
#line 963 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        int32_t _fn_chpl) {
#line 963 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseArr_chpl super_tmp_chpl = NULL;
#line 963 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 963 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  super_tmp_chpl = &((this_chpl31)->super);
#line 963 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 963 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  ((RootClass_chpl)(&((super_tmp_chpl)->super)))->chpl__cid = chpl__cid_RootClass_chpl;
#line 963 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  ((RootClass_chpl)(((RootClass_chpl)(super_tmp_chpl))))->chpl__cid = chpl__cid_RootClass_chpl;
#line 963 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 963 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  (super_tmp_chpl)->prev = prev_chpl;
#line 963 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 963 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  (super_tmp_chpl)->next = next_chpl;
#line 963 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 963 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  (super_tmp_chpl)->pid = pid_chpl;
#line 963 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 963 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  (super_tmp_chpl)->_decEltRefCounts = _decEltRefCounts_chpl;
#line 963 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 963 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  (super_tmp_chpl)->_resizePolicy = _resizePolicy_chpl;
#line 963 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  ((RootClass_chpl)(super_tmp_chpl))->chpl__cid = chpl__cid_BaseArr_chpl;
#line 963 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  ((RootClass_chpl)(((BaseArr_chpl)(this_chpl31))))->chpl__cid = chpl__cid_BaseArr_chpl;
#line 963 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  ((RootClass_chpl)(this_chpl31))->chpl__cid = chpl__cid_AbsBaseArr__tuple_2_Tensor_3__real64_int64_t_chpl;
#line 963 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  return;
#line 963 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
}

#line 963 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
/* ChapelDistribution.chpl:963 */
#line 963 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
static void init_chpl53(AbsBaseArr__real64_chpl this_chpl31,
#line 963 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        BaseArr_chpl prev_chpl,
#line 963 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        BaseArr_chpl next_chpl,
#line 963 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        int64_t pid_chpl,
#line 963 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        chpl_bool _decEltRefCounts_chpl,
#line 963 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        chpl_ddataResizePolicy _resizePolicy_chpl,
#line 963 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        int64_t _ln_chpl,
#line 963 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        int32_t _fn_chpl) {
#line 963 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseArr_chpl super_tmp_chpl = NULL;
#line 963 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 963 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  super_tmp_chpl = &((this_chpl31)->super);
#line 963 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 963 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  ((RootClass_chpl)(&((super_tmp_chpl)->super)))->chpl__cid = chpl__cid_RootClass_chpl;
#line 963 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  ((RootClass_chpl)(((RootClass_chpl)(super_tmp_chpl))))->chpl__cid = chpl__cid_RootClass_chpl;
#line 963 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 963 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  (super_tmp_chpl)->prev = prev_chpl;
#line 963 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 963 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  (super_tmp_chpl)->next = next_chpl;
#line 963 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 963 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  (super_tmp_chpl)->pid = pid_chpl;
#line 963 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 963 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  (super_tmp_chpl)->_decEltRefCounts = _decEltRefCounts_chpl;
#line 963 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 963 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  (super_tmp_chpl)->_resizePolicy = _resizePolicy_chpl;
#line 963 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  ((RootClass_chpl)(super_tmp_chpl))->chpl__cid = chpl__cid_BaseArr_chpl;
#line 963 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  ((RootClass_chpl)(((BaseArr_chpl)(this_chpl31))))->chpl__cid = chpl__cid_BaseArr_chpl;
#line 963 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  ((RootClass_chpl)(this_chpl31))->chpl__cid = chpl__cid_AbsBaseArr__real64_chpl;
#line 963 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  return;
#line 963 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
}

#line 963 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
/* ChapelDistribution.chpl:963 */
#line 963 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
static void chpl__auto_destroy_AbsBaseArr(AbsBaseArr__tuple_2_Tensor_3__real64_int64_t_chpl this_chpl31,
#line 963 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                                          int64_t _ln_chpl,
#line 963 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                                          int32_t _fn_chpl) {
#line 963 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  deinit_chpl36(((BaseArr_chpl)(this_chpl31)), _ln_chpl, _fn_chpl);
#line 963 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  return;
#line 963 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
}

#line 966 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
/* ChapelDistribution.chpl:966 */
#line 966 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
static void decEltCountsIfNeeded_chpl3(AbsBaseArr__real64_chpl this_chpl31,
#line 966 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                                       int64_t _ln_chpl,
#line 966 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                                       int32_t _fn_chpl) {
#line 967 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseArr_chpl coerce_tmp_chpl3 = NULL;
#line 967 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  coerce_tmp_chpl3 = ((BaseArr_chpl)(this_chpl31));
#line 967 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 967 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  if ((coerce_tmp_chpl3)->_decEltRefCounts) /* ZLINE: 967 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl */
#line 967 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  {
#line 969 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    chpl_decRefCountsForDomainsInArrayEltTypes2(((BaseArr_chpl)(this_chpl31)));
#line 969 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  }
#line 966 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  return;
#line 966 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
}

#line 966 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
/* ChapelDistribution.chpl:966 */
#line 966 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
static void decEltCountsIfNeeded_chpl2(AbsBaseArr__tuple_2_Tensor_3__real64_int64_t_chpl this_chpl31,
#line 966 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                                       int64_t _ln_chpl,
#line 966 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                                       int32_t _fn_chpl) {
#line 967 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseArr_chpl coerce_tmp_chpl3 = NULL;
#line 967 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  coerce_tmp_chpl3 = ((BaseArr_chpl)(this_chpl31));
#line 967 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 967 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  if ((coerce_tmp_chpl3)->_decEltRefCounts) /* ZLINE: 967 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl */
#line 967 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  {
#line 969 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    chpl_decRefCountsForDomainsInArrayEltTypes7(((BaseArr_chpl)(this_chpl31)));
#line 969 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  }
#line 966 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  return;
#line 966 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
}

#line 984 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
/* ChapelDistribution.chpl:984 */
#line 984 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
static void init_chpl56(BaseArrOverRectangularDom_1_int64_t_one_chpl this_chpl31,
#line 984 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        BaseArr_chpl prev_chpl,
#line 984 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        BaseArr_chpl next_chpl,
#line 984 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        int64_t pid_chpl,
#line 984 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        chpl_bool _decEltRefCounts_chpl,
#line 984 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        chpl_ddataResizePolicy _resizePolicy_chpl,
#line 984 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        int64_t _ln_chpl,
#line 984 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        int32_t _fn_chpl) {
#line 984 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseArr_chpl super_tmp_chpl = NULL;
#line 984 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 984 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  super_tmp_chpl = &((this_chpl31)->super);
#line 984 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 984 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  ((RootClass_chpl)(&((super_tmp_chpl)->super)))->chpl__cid = chpl__cid_RootClass_chpl;
#line 984 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  ((RootClass_chpl)(((RootClass_chpl)(super_tmp_chpl))))->chpl__cid = chpl__cid_RootClass_chpl;
#line 984 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 984 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  (super_tmp_chpl)->prev = prev_chpl;
#line 984 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 984 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  (super_tmp_chpl)->next = next_chpl;
#line 984 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 984 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  (super_tmp_chpl)->pid = pid_chpl;
#line 984 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 984 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  (super_tmp_chpl)->_decEltRefCounts = _decEltRefCounts_chpl;
#line 984 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 984 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  (super_tmp_chpl)->_resizePolicy = _resizePolicy_chpl;
#line 984 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  ((RootClass_chpl)(super_tmp_chpl))->chpl__cid = chpl__cid_BaseArr_chpl;
#line 984 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  ((RootClass_chpl)(((BaseArr_chpl)(this_chpl31))))->chpl__cid = chpl__cid_BaseArr_chpl;
#line 984 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  ((RootClass_chpl)(this_chpl31))->chpl__cid = chpl__cid_BaseArrOverRectangularDom_1_int64_t_one_chpl;
#line 984 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  return;
#line 984 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
}

#line 984 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
/* ChapelDistribution.chpl:984 */
#line 984 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
static void init_chpl58(BaseArrOverRectangularDom_4_int64_t_one_chpl this_chpl31,
#line 984 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        BaseArr_chpl prev_chpl,
#line 984 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        BaseArr_chpl next_chpl,
#line 984 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        int64_t pid_chpl,
#line 984 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        chpl_bool _decEltRefCounts_chpl,
#line 984 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        chpl_ddataResizePolicy _resizePolicy_chpl,
#line 984 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        int64_t _ln_chpl,
#line 984 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        int32_t _fn_chpl) {
#line 984 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseArr_chpl super_tmp_chpl = NULL;
#line 984 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 984 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  super_tmp_chpl = &((this_chpl31)->super);
#line 984 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 984 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  ((RootClass_chpl)(&((super_tmp_chpl)->super)))->chpl__cid = chpl__cid_RootClass_chpl;
#line 984 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  ((RootClass_chpl)(((RootClass_chpl)(super_tmp_chpl))))->chpl__cid = chpl__cid_RootClass_chpl;
#line 984 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 984 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  (super_tmp_chpl)->prev = prev_chpl;
#line 984 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 984 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  (super_tmp_chpl)->next = next_chpl;
#line 984 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 984 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  (super_tmp_chpl)->pid = pid_chpl;
#line 984 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 984 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  (super_tmp_chpl)->_decEltRefCounts = _decEltRefCounts_chpl;
#line 984 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 984 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  (super_tmp_chpl)->_resizePolicy = _resizePolicy_chpl;
#line 984 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  ((RootClass_chpl)(super_tmp_chpl))->chpl__cid = chpl__cid_BaseArr_chpl;
#line 984 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  ((RootClass_chpl)(((BaseArr_chpl)(this_chpl31))))->chpl__cid = chpl__cid_BaseArr_chpl;
#line 984 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  ((RootClass_chpl)(this_chpl31))->chpl__cid = chpl__cid_BaseArrOverRectangularDom_4_int64_t_one_chpl;
#line 984 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  return;
#line 984 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
}

#line 984 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
/* ChapelDistribution.chpl:984 */
#line 984 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
static void init_chpl55(BaseArrOverRectangularDom_2_int64_t_one_chpl this_chpl31,
#line 984 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        BaseArr_chpl prev_chpl,
#line 984 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        BaseArr_chpl next_chpl,
#line 984 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        int64_t pid_chpl,
#line 984 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        chpl_bool _decEltRefCounts_chpl,
#line 984 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        chpl_ddataResizePolicy _resizePolicy_chpl,
#line 984 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        int64_t _ln_chpl,
#line 984 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        int32_t _fn_chpl) {
#line 984 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseArr_chpl super_tmp_chpl = NULL;
#line 984 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 984 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  super_tmp_chpl = &((this_chpl31)->super);
#line 984 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 984 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  ((RootClass_chpl)(&((super_tmp_chpl)->super)))->chpl__cid = chpl__cid_RootClass_chpl;
#line 984 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  ((RootClass_chpl)(((RootClass_chpl)(super_tmp_chpl))))->chpl__cid = chpl__cid_RootClass_chpl;
#line 984 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 984 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  (super_tmp_chpl)->prev = prev_chpl;
#line 984 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 984 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  (super_tmp_chpl)->next = next_chpl;
#line 984 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 984 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  (super_tmp_chpl)->pid = pid_chpl;
#line 984 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 984 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  (super_tmp_chpl)->_decEltRefCounts = _decEltRefCounts_chpl;
#line 984 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 984 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  (super_tmp_chpl)->_resizePolicy = _resizePolicy_chpl;
#line 984 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  ((RootClass_chpl)(super_tmp_chpl))->chpl__cid = chpl__cid_BaseArr_chpl;
#line 984 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  ((RootClass_chpl)(((BaseArr_chpl)(this_chpl31))))->chpl__cid = chpl__cid_BaseArr_chpl;
#line 984 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  ((RootClass_chpl)(this_chpl31))->chpl__cid = chpl__cid_BaseArrOverRectangularDom_2_int64_t_one_chpl;
#line 984 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  return;
#line 984 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
}

#line 984 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
/* ChapelDistribution.chpl:984 */
#line 984 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
static void init_chpl57(BaseArrOverRectangularDom_3_int64_t_one_chpl this_chpl31,
#line 984 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        BaseArr_chpl prev_chpl,
#line 984 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        BaseArr_chpl next_chpl,
#line 984 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        int64_t pid_chpl,
#line 984 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        chpl_bool _decEltRefCounts_chpl,
#line 984 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        chpl_ddataResizePolicy _resizePolicy_chpl,
#line 984 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        int64_t _ln_chpl,
#line 984 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        int32_t _fn_chpl) {
#line 984 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseArr_chpl super_tmp_chpl = NULL;
#line 984 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 984 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  super_tmp_chpl = &((this_chpl31)->super);
#line 984 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 984 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  ((RootClass_chpl)(&((super_tmp_chpl)->super)))->chpl__cid = chpl__cid_RootClass_chpl;
#line 984 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  ((RootClass_chpl)(((RootClass_chpl)(super_tmp_chpl))))->chpl__cid = chpl__cid_RootClass_chpl;
#line 984 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 984 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  (super_tmp_chpl)->prev = prev_chpl;
#line 984 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 984 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  (super_tmp_chpl)->next = next_chpl;
#line 984 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 984 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  (super_tmp_chpl)->pid = pid_chpl;
#line 984 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 984 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  (super_tmp_chpl)->_decEltRefCounts = _decEltRefCounts_chpl;
#line 984 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 984 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  (super_tmp_chpl)->_resizePolicy = _resizePolicy_chpl;
#line 984 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  ((RootClass_chpl)(super_tmp_chpl))->chpl__cid = chpl__cid_BaseArr_chpl;
#line 984 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  ((RootClass_chpl)(((BaseArr_chpl)(this_chpl31))))->chpl__cid = chpl__cid_BaseArr_chpl;
#line 984 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  ((RootClass_chpl)(this_chpl31))->chpl__cid = chpl__cid_BaseArrOverRectangularDom_3_int64_t_one_chpl;
#line 984 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  return;
#line 984 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
}

#line 1002 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
/* ChapelDistribution.chpl:1002 */
#line 1002 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
static void dsiReallocate_chpl3(BaseArrOverRectangularDom_4_int64_t_one_chpl this_chpl31,
#line 1002 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                                _tuple_4_star_range_int64_t_both_one_chpl * bounds_chpl,
#line 1002 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                                int64_t _ln_chpl,
#line 1002 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                                int32_t _fn_chpl) {
#line 1003 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  halt_chpl(&_str_literal_1158_chpl, _ln_chpl, _fn_chpl);
#line 1002 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  return;
#line 1002 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
}

#line 1002 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
/* ChapelDistribution.chpl:1002 */
#line 1002 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
static void dsiReallocate_chpl4(BaseArrOverRectangularDom_2_int64_t_one_chpl this_chpl31,
#line 1002 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                                _tuple_2_star_range_int64_t_both_one_chpl * bounds_chpl,
#line 1002 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                                int64_t _ln_chpl,
#line 1002 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                                int32_t _fn_chpl) {
#line 1003 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  halt_chpl(&_str_literal_1158_chpl, _ln_chpl, _fn_chpl);
#line 1002 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  return;
#line 1002 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
}

#line 1002 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
/* ChapelDistribution.chpl:1002 */
#line 1002 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
static void dsiReallocate_chpl(BaseArrOverRectangularDom_3_int64_t_one_chpl this_chpl31,
#line 1002 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                               _tuple_3_star_range_int64_t_both_one_chpl * bounds_chpl,
#line 1002 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                               int64_t _ln_chpl,
#line 1002 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                               int32_t _fn_chpl) {
#line 1003 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  halt_chpl(&_str_literal_1158_chpl, _ln_chpl, _fn_chpl);
#line 1002 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  return;
#line 1002 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
}

#line 1002 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
/* ChapelDistribution.chpl:1002 */
#line 1002 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
static void dsiReallocate_chpl2(BaseArrOverRectangularDom_1_int64_t_one_chpl this_chpl31,
#line 1002 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                                _tuple_1_star_range_int64_t_both_one_chpl * bounds_chpl,
#line 1002 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                                int64_t _ln_chpl,
#line 1002 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                                int32_t _fn_chpl) {
#line 1003 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  halt_chpl(&_str_literal_1158_chpl, _ln_chpl, _fn_chpl);
#line 1002 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  return;
#line 1002 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
}

#line 1006 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
/* ChapelDistribution.chpl:1006 */
#line 1006 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
static void dsiPostReallocate_chpl3(BaseArrOverRectangularDom_1_int64_t_one_chpl this_chpl31) {
#line 1006 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  return;
#line 1006 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
}

#line 1006 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
/* ChapelDistribution.chpl:1006 */
#line 1006 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
static void dsiPostReallocate_chpl2(BaseArrOverRectangularDom_4_int64_t_one_chpl this_chpl31) {
#line 1006 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  return;
#line 1006 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
}

#line 1006 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
/* ChapelDistribution.chpl:1006 */
#line 1006 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
static void dsiPostReallocate_chpl4(BaseArrOverRectangularDom_2_int64_t_one_chpl this_chpl31) {
#line 1006 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  return;
#line 1006 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
}

#line 1006 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
/* ChapelDistribution.chpl:1006 */
#line 1006 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
static void dsiPostReallocate_chpl(BaseArrOverRectangularDom_3_int64_t_one_chpl this_chpl31) {
#line 1006 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  return;
#line 1006 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
}

#line 1009 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
/* ChapelDistribution.chpl:1009 */
#line 1009 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
static void deinit_chpl40(BaseArrOverRectangularDom_1_int64_t_one_chpl this_chpl31,
#line 1009 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                          int64_t _ln_chpl,
#line 1009 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                          int32_t _fn_chpl) {
#line 1009 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  deinit_chpl36(((BaseArr_chpl)(this_chpl31)), _ln_chpl, _fn_chpl);
#line 1009 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  return;
#line 1009 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
}

#line 1009 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
/* ChapelDistribution.chpl:1009 */
#line 1009 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
static void deinit_chpl39(BaseArrOverRectangularDom_4_int64_t_one_chpl this_chpl31,
#line 1009 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                          int64_t _ln_chpl,
#line 1009 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                          int32_t _fn_chpl) {
#line 1009 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  deinit_chpl36(((BaseArr_chpl)(this_chpl31)), _ln_chpl, _fn_chpl);
#line 1009 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  return;
#line 1009 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
}

#line 1009 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
/* ChapelDistribution.chpl:1009 */
#line 1009 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
static void deinit_chpl38(BaseArrOverRectangularDom_2_int64_t_one_chpl this_chpl31,
#line 1009 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                          int64_t _ln_chpl,
#line 1009 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                          int32_t _fn_chpl) {
#line 1009 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  deinit_chpl36(((BaseArr_chpl)(this_chpl31)), _ln_chpl, _fn_chpl);
#line 1009 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  return;
#line 1009 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
}

#line 1009 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
/* ChapelDistribution.chpl:1009 */
#line 1009 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
static void deinit_chpl37(BaseArrOverRectangularDom_3_int64_t_one_chpl this_chpl31,
#line 1009 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                          int64_t _ln_chpl,
#line 1009 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                          int32_t _fn_chpl) {
#line 1009 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  deinit_chpl36(((BaseArr_chpl)(this_chpl31)), _ln_chpl, _fn_chpl);
#line 1009 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  return;
#line 1009 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
}

#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
/* ChapelDistribution.chpl:1015 */
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
static void init_chpl66(BaseRectangularArr_1_int64_t_one_Tensor_1__real64_chpl this_chpl31,
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        BaseArr_chpl prev_chpl,
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        BaseArr_chpl next_chpl,
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        int64_t pid_chpl,
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        chpl_bool _decEltRefCounts_chpl,
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        chpl_ddataResizePolicy _resizePolicy_chpl,
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        int64_t _ln_chpl,
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        int32_t _fn_chpl) {
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseArrOverRectangularDom_1_int64_t_one_chpl super_tmp_chpl = NULL;
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  super_tmp_chpl = &((this_chpl31)->super);
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  init_chpl56(super_tmp_chpl, prev_chpl, next_chpl, pid_chpl, _decEltRefCounts_chpl, _resizePolicy_chpl, _ln_chpl, _fn_chpl);
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  ((RootClass_chpl)(((BaseArrOverRectangularDom_1_int64_t_one_chpl)(this_chpl31))))->chpl__cid = chpl__cid_BaseArrOverRectangularDom_1_int64_t_one_chpl;
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  ((RootClass_chpl)(this_chpl31))->chpl__cid = chpl__cid_BaseRectangularArr_1_int64_t_one_Tensor_1__real64_chpl;
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  return;
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
}

#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
/* ChapelDistribution.chpl:1015 */
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
static void init_chpl63(BaseRectangularArr_1_int64_t_one__tuple_2_Tensor_3__real64_int64_t_chpl this_chpl31,
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        BaseArr_chpl prev_chpl,
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        BaseArr_chpl next_chpl,
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        int64_t pid_chpl,
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        chpl_bool _decEltRefCounts_chpl,
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        chpl_ddataResizePolicy _resizePolicy_chpl,
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        int64_t _ln_chpl,
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        int32_t _fn_chpl) {
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseArrOverRectangularDom_1_int64_t_one_chpl super_tmp_chpl = NULL;
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  super_tmp_chpl = &((this_chpl31)->super);
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  init_chpl56(super_tmp_chpl, prev_chpl, next_chpl, pid_chpl, _decEltRefCounts_chpl, _resizePolicy_chpl, _ln_chpl, _fn_chpl);
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  ((RootClass_chpl)(((BaseArrOverRectangularDom_1_int64_t_one_chpl)(this_chpl31))))->chpl__cid = chpl__cid_BaseArrOverRectangularDom_1_int64_t_one_chpl;
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  ((RootClass_chpl)(this_chpl31))->chpl__cid = chpl__cid_BaseRectangularArr_1_int64_t_one__tuple_2_Tensor_3__real64_int64_t_chpl;
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  return;
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
}

#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
/* ChapelDistribution.chpl:1015 */
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
static void init_chpl69(BaseRectangularArr_1_int64_t_one_Tensor_3__real64_chpl this_chpl31,
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        BaseArr_chpl prev_chpl,
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        BaseArr_chpl next_chpl,
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        int64_t pid_chpl,
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        chpl_bool _decEltRefCounts_chpl,
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        chpl_ddataResizePolicy _resizePolicy_chpl,
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        int64_t _ln_chpl,
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        int32_t _fn_chpl) {
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseArrOverRectangularDom_1_int64_t_one_chpl super_tmp_chpl = NULL;
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  super_tmp_chpl = &((this_chpl31)->super);
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  init_chpl56(super_tmp_chpl, prev_chpl, next_chpl, pid_chpl, _decEltRefCounts_chpl, _resizePolicy_chpl, _ln_chpl, _fn_chpl);
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  ((RootClass_chpl)(((BaseArrOverRectangularDom_1_int64_t_one_chpl)(this_chpl31))))->chpl__cid = chpl__cid_BaseArrOverRectangularDom_1_int64_t_one_chpl;
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  ((RootClass_chpl)(this_chpl31))->chpl__cid = chpl__cid_BaseRectangularArr_1_int64_t_one_Tensor_3__real64_chpl;
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  return;
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
}

#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
/* ChapelDistribution.chpl:1015 */
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
static void init_chpl59(BaseRectangularArr_3_int64_t_one__real64_chpl this_chpl31,
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        BaseArr_chpl prev_chpl,
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        BaseArr_chpl next_chpl,
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        int64_t pid_chpl,
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        chpl_bool _decEltRefCounts_chpl,
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        chpl_ddataResizePolicy _resizePolicy_chpl,
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        int64_t _ln_chpl,
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        int32_t _fn_chpl) {
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseArrOverRectangularDom_3_int64_t_one_chpl super_tmp_chpl = NULL;
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  super_tmp_chpl = &((this_chpl31)->super);
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  init_chpl57(super_tmp_chpl, prev_chpl, next_chpl, pid_chpl, _decEltRefCounts_chpl, _resizePolicy_chpl, _ln_chpl, _fn_chpl);
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  ((RootClass_chpl)(((BaseArrOverRectangularDom_3_int64_t_one_chpl)(this_chpl31))))->chpl__cid = chpl__cid_BaseArrOverRectangularDom_3_int64_t_one_chpl;
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  ((RootClass_chpl)(this_chpl31))->chpl__cid = chpl__cid_BaseRectangularArr_3_int64_t_one__real64_chpl;
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  return;
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
}

#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
/* ChapelDistribution.chpl:1015 */
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
static void init_chpl61(BaseRectangularArr_1_int64_t_one__array_DefaultRectangularArr_1_int64_t_one__real64_int64_t_chpl this_chpl31,
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        BaseArr_chpl prev_chpl,
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        BaseArr_chpl next_chpl,
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        int64_t pid_chpl,
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        chpl_bool _decEltRefCounts_chpl,
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        chpl_ddataResizePolicy _resizePolicy_chpl,
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        chpl___RuntimeTypeInfo13 * eltType_chpl2,
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        int64_t _ln_chpl,
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        int32_t _fn_chpl) {
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseArrOverRectangularDom_1_int64_t_one_chpl super_tmp_chpl = NULL;
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  super_tmp_chpl = &((this_chpl31)->super);
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  init_chpl56(super_tmp_chpl, prev_chpl, next_chpl, pid_chpl, _decEltRefCounts_chpl, _resizePolicy_chpl, _ln_chpl, _fn_chpl);
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  ((RootClass_chpl)(((BaseArrOverRectangularDom_1_int64_t_one_chpl)(this_chpl31))))->chpl__cid = chpl__cid_BaseArrOverRectangularDom_1_int64_t_one_chpl;
#line 1017 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 1017 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  (this_chpl31)->eltType = *(eltType_chpl2);
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  ((RootClass_chpl)(this_chpl31))->chpl__cid = chpl__cid_BaseRectangularArr_1_int64_t_one__array_DefaultRectangularArr_1_int64_t_one__real64_int64_t_chpl;
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  return;
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
}

#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
/* ChapelDistribution.chpl:1015 */
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
static void init_chpl64(BaseRectangularArr_2_int64_t_one_uint8_t_chpl this_chpl31,
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        BaseArr_chpl prev_chpl,
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        BaseArr_chpl next_chpl,
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        int64_t pid_chpl,
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        chpl_bool _decEltRefCounts_chpl,
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        chpl_ddataResizePolicy _resizePolicy_chpl,
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        int64_t _ln_chpl,
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        int32_t _fn_chpl) {
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseArrOverRectangularDom_2_int64_t_one_chpl super_tmp_chpl = NULL;
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  super_tmp_chpl = &((this_chpl31)->super);
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  init_chpl55(super_tmp_chpl, prev_chpl, next_chpl, pid_chpl, _decEltRefCounts_chpl, _resizePolicy_chpl, _ln_chpl, _fn_chpl);
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  ((RootClass_chpl)(((BaseArrOverRectangularDom_2_int64_t_one_chpl)(this_chpl31))))->chpl__cid = chpl__cid_BaseArrOverRectangularDom_2_int64_t_one_chpl;
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  ((RootClass_chpl)(this_chpl31))->chpl__cid = chpl__cid_BaseRectangularArr_2_int64_t_one_uint8_t_chpl;
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  return;
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
}

#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
/* ChapelDistribution.chpl:1015 */
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
static void init_chpl65(BaseRectangularArr_1_int64_t_one__array_DefaultRectangularArr_2_int64_t_one__real64_int64_t_chpl this_chpl31,
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        BaseArr_chpl prev_chpl,
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        BaseArr_chpl next_chpl,
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        int64_t pid_chpl,
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        chpl_bool _decEltRefCounts_chpl,
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        chpl_ddataResizePolicy _resizePolicy_chpl,
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        chpl___RuntimeTypeInfo14 * eltType_chpl2,
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        int64_t _ln_chpl,
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        int32_t _fn_chpl) {
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseArrOverRectangularDom_1_int64_t_one_chpl super_tmp_chpl = NULL;
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  super_tmp_chpl = &((this_chpl31)->super);
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  init_chpl56(super_tmp_chpl, prev_chpl, next_chpl, pid_chpl, _decEltRefCounts_chpl, _resizePolicy_chpl, _ln_chpl, _fn_chpl);
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  ((RootClass_chpl)(((BaseArrOverRectangularDom_1_int64_t_one_chpl)(this_chpl31))))->chpl__cid = chpl__cid_BaseArrOverRectangularDom_1_int64_t_one_chpl;
#line 1017 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 1017 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  (this_chpl31)->eltType = *(eltType_chpl2);
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  ((RootClass_chpl)(this_chpl31))->chpl__cid = chpl__cid_BaseRectangularArr_1_int64_t_one__array_DefaultRectangularArr_2_int64_t_one__real64_int64_t_chpl;
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  return;
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
}

#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
/* ChapelDistribution.chpl:1015 */
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
static void init_chpl70(BaseRectangularArr_1_int64_t_one__real64_chpl this_chpl31,
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        BaseArr_chpl prev_chpl,
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        BaseArr_chpl next_chpl,
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        int64_t pid_chpl,
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        chpl_bool _decEltRefCounts_chpl,
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        chpl_ddataResizePolicy _resizePolicy_chpl,
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        int64_t _ln_chpl,
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        int32_t _fn_chpl) {
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseArrOverRectangularDom_1_int64_t_one_chpl super_tmp_chpl = NULL;
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  super_tmp_chpl = &((this_chpl31)->super);
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  init_chpl56(super_tmp_chpl, prev_chpl, next_chpl, pid_chpl, _decEltRefCounts_chpl, _resizePolicy_chpl, _ln_chpl, _fn_chpl);
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  ((RootClass_chpl)(((BaseArrOverRectangularDom_1_int64_t_one_chpl)(this_chpl31))))->chpl__cid = chpl__cid_BaseArrOverRectangularDom_1_int64_t_one_chpl;
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  ((RootClass_chpl)(this_chpl31))->chpl__cid = chpl__cid_BaseRectangularArr_1_int64_t_one__real64_chpl;
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  return;
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
}

#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
/* ChapelDistribution.chpl:1015 */
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
static void init_chpl67(BaseRectangularArr_2_int64_t_one__real64_chpl this_chpl31,
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        BaseArr_chpl prev_chpl,
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        BaseArr_chpl next_chpl,
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        int64_t pid_chpl,
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        chpl_bool _decEltRefCounts_chpl,
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        chpl_ddataResizePolicy _resizePolicy_chpl,
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        int64_t _ln_chpl,
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        int32_t _fn_chpl) {
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseArrOverRectangularDom_2_int64_t_one_chpl super_tmp_chpl = NULL;
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  super_tmp_chpl = &((this_chpl31)->super);
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  init_chpl55(super_tmp_chpl, prev_chpl, next_chpl, pid_chpl, _decEltRefCounts_chpl, _resizePolicy_chpl, _ln_chpl, _fn_chpl);
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  ((RootClass_chpl)(((BaseArrOverRectangularDom_2_int64_t_one_chpl)(this_chpl31))))->chpl__cid = chpl__cid_BaseArrOverRectangularDom_2_int64_t_one_chpl;
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  ((RootClass_chpl)(this_chpl31))->chpl__cid = chpl__cid_BaseRectangularArr_2_int64_t_one__real64_chpl;
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  return;
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
}

#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
/* ChapelDistribution.chpl:1015 */
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
static void init_chpl62(BaseRectangularArr_1_int64_t_one_uint64_t_chpl this_chpl31,
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        BaseArr_chpl prev_chpl,
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        BaseArr_chpl next_chpl,
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        int64_t pid_chpl,
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        chpl_bool _decEltRefCounts_chpl,
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        chpl_ddataResizePolicy _resizePolicy_chpl,
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        int64_t _ln_chpl,
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        int32_t _fn_chpl) {
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseArrOverRectangularDom_1_int64_t_one_chpl super_tmp_chpl = NULL;
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  super_tmp_chpl = &((this_chpl31)->super);
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  init_chpl56(super_tmp_chpl, prev_chpl, next_chpl, pid_chpl, _decEltRefCounts_chpl, _resizePolicy_chpl, _ln_chpl, _fn_chpl);
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  ((RootClass_chpl)(((BaseArrOverRectangularDom_1_int64_t_one_chpl)(this_chpl31))))->chpl__cid = chpl__cid_BaseArrOverRectangularDom_1_int64_t_one_chpl;
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  ((RootClass_chpl)(this_chpl31))->chpl__cid = chpl__cid_BaseRectangularArr_1_int64_t_one_uint64_t_chpl;
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  return;
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
}

#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
/* ChapelDistribution.chpl:1015 */
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
static void init_chpl68(BaseRectangularArr_1_int64_t_one_int64_t_chpl this_chpl31,
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        BaseArr_chpl prev_chpl,
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        BaseArr_chpl next_chpl,
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        int64_t pid_chpl,
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        chpl_bool _decEltRefCounts_chpl,
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        chpl_ddataResizePolicy _resizePolicy_chpl,
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        int64_t _ln_chpl,
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        int32_t _fn_chpl) {
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseArrOverRectangularDom_1_int64_t_one_chpl super_tmp_chpl = NULL;
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  super_tmp_chpl = &((this_chpl31)->super);
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  init_chpl56(super_tmp_chpl, prev_chpl, next_chpl, pid_chpl, _decEltRefCounts_chpl, _resizePolicy_chpl, _ln_chpl, _fn_chpl);
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  ((RootClass_chpl)(((BaseArrOverRectangularDom_1_int64_t_one_chpl)(this_chpl31))))->chpl__cid = chpl__cid_BaseArrOverRectangularDom_1_int64_t_one_chpl;
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  ((RootClass_chpl)(this_chpl31))->chpl__cid = chpl__cid_BaseRectangularArr_1_int64_t_one_int64_t_chpl;
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  return;
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
}

#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
/* ChapelDistribution.chpl:1015 */
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
static void init_chpl60(BaseRectangularArr_4_int64_t_one__real64_chpl this_chpl31,
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        BaseArr_chpl prev_chpl,
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        BaseArr_chpl next_chpl,
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        int64_t pid_chpl,
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        chpl_bool _decEltRefCounts_chpl,
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        chpl_ddataResizePolicy _resizePolicy_chpl,
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        int64_t _ln_chpl,
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        int32_t _fn_chpl) {
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseArrOverRectangularDom_4_int64_t_one_chpl super_tmp_chpl = NULL;
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  super_tmp_chpl = &((this_chpl31)->super);
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  init_chpl58(super_tmp_chpl, prev_chpl, next_chpl, pid_chpl, _decEltRefCounts_chpl, _resizePolicy_chpl, _ln_chpl, _fn_chpl);
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  ((RootClass_chpl)(((BaseArrOverRectangularDom_4_int64_t_one_chpl)(this_chpl31))))->chpl__cid = chpl__cid_BaseArrOverRectangularDom_4_int64_t_one_chpl;
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  ((RootClass_chpl)(this_chpl31))->chpl__cid = chpl__cid_BaseRectangularArr_4_int64_t_one__real64_chpl;
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  return;
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
}

#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
/* ChapelDistribution.chpl:1015 */
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
static void init_chpl71(BaseRectangularArr_1_int64_t_one_locale_chpl this_chpl31,
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        BaseArr_chpl prev_chpl,
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        BaseArr_chpl next_chpl,
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        int64_t pid_chpl,
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        chpl_bool _decEltRefCounts_chpl,
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        chpl_ddataResizePolicy _resizePolicy_chpl,
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        int64_t _ln_chpl,
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                        int32_t _fn_chpl) {
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseArrOverRectangularDom_1_int64_t_one_chpl super_tmp_chpl = NULL;
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  super_tmp_chpl = &((this_chpl31)->super);
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  init_chpl56(super_tmp_chpl, prev_chpl, next_chpl, pid_chpl, _decEltRefCounts_chpl, _resizePolicy_chpl, _ln_chpl, _fn_chpl);
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  ((RootClass_chpl)(((BaseArrOverRectangularDom_1_int64_t_one_chpl)(this_chpl31))))->chpl__cid = chpl__cid_BaseArrOverRectangularDom_1_int64_t_one_chpl;
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  ((RootClass_chpl)(this_chpl31))->chpl__cid = chpl__cid_BaseRectangularArr_1_int64_t_one_locale_chpl;
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  return;
#line 1015 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
}

#line 1019 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
/* ChapelDistribution.chpl:1019 */
#line 1019 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
static void deinit_chpl48(BaseRectangularArr_1_int64_t_one_Tensor_3__real64_chpl this_chpl31,
#line 1019 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                          int64_t _ln_chpl,
#line 1019 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                          int32_t _fn_chpl) {
#line 1019 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  deinit_chpl40(((BaseArrOverRectangularDom_1_int64_t_one_chpl)(this_chpl31)), _ln_chpl, _fn_chpl);
#line 1019 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  return;
#line 1019 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
}

#line 1019 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
/* ChapelDistribution.chpl:1019 */
#line 1019 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
static void deinit_chpl44(BaseRectangularArr_1_int64_t_one_uint64_t_chpl this_chpl31,
#line 1019 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                          int64_t _ln_chpl,
#line 1019 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                          int32_t _fn_chpl) {
#line 1019 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  deinit_chpl40(((BaseArrOverRectangularDom_1_int64_t_one_chpl)(this_chpl31)), _ln_chpl, _fn_chpl);
#line 1019 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  return;
#line 1019 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
}

#line 1019 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
/* ChapelDistribution.chpl:1019 */
#line 1019 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
static void deinit_chpl42(BaseRectangularArr_1_int64_t_one_locale_chpl this_chpl31,
#line 1019 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                          int64_t _ln_chpl,
#line 1019 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                          int32_t _fn_chpl) {
#line 1019 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  deinit_chpl40(((BaseArrOverRectangularDom_1_int64_t_one_chpl)(this_chpl31)), _ln_chpl, _fn_chpl);
#line 1019 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  return;
#line 1019 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
}

#line 1019 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
/* ChapelDistribution.chpl:1019 */
#line 1019 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
static void deinit_chpl43(BaseRectangularArr_1_int64_t_one_int64_t_chpl this_chpl31,
#line 1019 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                          int64_t _ln_chpl,
#line 1019 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                          int32_t _fn_chpl) {
#line 1019 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  deinit_chpl40(((BaseArrOverRectangularDom_1_int64_t_one_chpl)(this_chpl31)), _ln_chpl, _fn_chpl);
#line 1019 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  return;
#line 1019 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
}

#line 1019 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
/* ChapelDistribution.chpl:1019 */
#line 1019 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
static void deinit_chpl45(BaseRectangularArr_1_int64_t_one__real64_chpl this_chpl31,
#line 1019 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                          int64_t _ln_chpl,
#line 1019 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                          int32_t _fn_chpl) {
#line 1019 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  deinit_chpl40(((BaseArrOverRectangularDom_1_int64_t_one_chpl)(this_chpl31)), _ln_chpl, _fn_chpl);
#line 1019 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  return;
#line 1019 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
}

#line 1019 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
/* ChapelDistribution.chpl:1019 */
#line 1019 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
static void deinit_chpl46(BaseRectangularArr_1_int64_t_one__array_DefaultRectangularArr_2_int64_t_one__real64_int64_t_chpl this_chpl31,
#line 1019 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                          int64_t _ln_chpl,
#line 1019 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                          int32_t _fn_chpl) {
#line 1019 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  deinit_chpl40(((BaseArrOverRectangularDom_1_int64_t_one_chpl)(this_chpl31)), _ln_chpl, _fn_chpl);
#line 1019 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  return;
#line 1019 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
}

#line 1019 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
/* ChapelDistribution.chpl:1019 */
#line 1019 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
static void deinit_chpl47(BaseRectangularArr_1_int64_t_one__array_DefaultRectangularArr_1_int64_t_one__real64_int64_t_chpl this_chpl31,
#line 1019 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                          int64_t _ln_chpl,
#line 1019 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                          int32_t _fn_chpl) {
#line 1019 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  deinit_chpl40(((BaseArrOverRectangularDom_1_int64_t_one_chpl)(this_chpl31)), _ln_chpl, _fn_chpl);
#line 1019 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  return;
#line 1019 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
}

#line 1019 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
/* ChapelDistribution.chpl:1019 */
#line 1019 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
static void deinit_chpl49(BaseRectangularArr_1_int64_t_one__tuple_2_Tensor_3__real64_int64_t_chpl this_chpl31,
#line 1019 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                          int64_t _ln_chpl,
#line 1019 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                          int32_t _fn_chpl) {
#line 1019 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  deinit_chpl40(((BaseArrOverRectangularDom_1_int64_t_one_chpl)(this_chpl31)), _ln_chpl, _fn_chpl);
#line 1019 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  return;
#line 1019 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
}

#line 1019 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
/* ChapelDistribution.chpl:1019 */
#line 1019 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
static void deinit_chpl50(BaseRectangularArr_1_int64_t_one_Tensor_1__real64_chpl this_chpl31,
#line 1019 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                          int64_t _ln_chpl,
#line 1019 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                          int32_t _fn_chpl) {
#line 1019 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  deinit_chpl40(((BaseArrOverRectangularDom_1_int64_t_one_chpl)(this_chpl31)), _ln_chpl, _fn_chpl);
#line 1019 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  return;
#line 1019 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
}

#line 1019 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
/* ChapelDistribution.chpl:1019 */
#line 1019 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
static void deinit_chpl51(BaseRectangularArr_4_int64_t_one__real64_chpl this_chpl31,
#line 1019 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                          int64_t _ln_chpl,
#line 1019 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                          int32_t _fn_chpl) {
#line 1019 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  deinit_chpl39(((BaseArrOverRectangularDom_4_int64_t_one_chpl)(this_chpl31)), _ln_chpl, _fn_chpl);
#line 1019 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  return;
#line 1019 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
}

#line 1019 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
/* ChapelDistribution.chpl:1019 */
#line 1019 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
static void deinit_chpl41(BaseRectangularArr_2_int64_t_one__real64_chpl this_chpl31,
#line 1019 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                          int64_t _ln_chpl,
#line 1019 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                          int32_t _fn_chpl) {
#line 1019 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  deinit_chpl38(((BaseArrOverRectangularDom_2_int64_t_one_chpl)(this_chpl31)), _ln_chpl, _fn_chpl);
#line 1019 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  return;
#line 1019 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
}

#line 1019 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
/* ChapelDistribution.chpl:1019 */
#line 1019 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
static void deinit_chpl52(BaseRectangularArr_2_int64_t_one_uint8_t_chpl this_chpl31,
#line 1019 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                          int64_t _ln_chpl,
#line 1019 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                          int32_t _fn_chpl) {
#line 1019 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  deinit_chpl38(((BaseArrOverRectangularDom_2_int64_t_one_chpl)(this_chpl31)), _ln_chpl, _fn_chpl);
#line 1019 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  return;
#line 1019 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
}

#line 1019 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
/* ChapelDistribution.chpl:1019 */
#line 1019 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
static void deinit_chpl53(BaseRectangularArr_3_int64_t_one__real64_chpl this_chpl31,
#line 1019 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                          int64_t _ln_chpl,
#line 1019 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                          int32_t _fn_chpl) {
#line 1019 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  deinit_chpl37(((BaseArrOverRectangularDom_3_int64_t_one_chpl)(this_chpl31)), _ln_chpl, _fn_chpl);
#line 1019 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  return;
#line 1019 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
}

#line 1039 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
/* ChapelDistribution.chpl:1039 */
#line 1039 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
static void decEltCountsIfNeeded_chpl6(BaseRectangularArr_3_int64_t_one__real64_chpl this_chpl31,
#line 1039 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                                       int64_t _ln_chpl,
#line 1039 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                                       int32_t _fn_chpl) {
#line 1040 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseArr_chpl coerce_tmp_chpl3 = NULL;
#line 1040 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  coerce_tmp_chpl3 = ((BaseArr_chpl)(this_chpl31));
#line 1040 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 1040 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  if ((coerce_tmp_chpl3)->_decEltRefCounts) /* ZLINE: 1040 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl */
#line 1040 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  {
#line 1042 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    chpl_decRefCountsForDomainsInArrayEltTypes2(((BaseArr_chpl)(this_chpl31)));
#line 1042 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  }
#line 1039 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  return;
#line 1039 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
}

#line 1039 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
/* ChapelDistribution.chpl:1039 */
#line 1039 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
static void decEltCountsIfNeeded_chpl12(BaseRectangularArr_2_int64_t_one_uint8_t_chpl this_chpl31,
#line 1039 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                                        int64_t _ln_chpl,
#line 1039 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                                        int32_t _fn_chpl) {
#line 1040 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseArr_chpl coerce_tmp_chpl3 = NULL;
#line 1040 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  coerce_tmp_chpl3 = ((BaseArr_chpl)(this_chpl31));
#line 1040 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 1040 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  if ((coerce_tmp_chpl3)->_decEltRefCounts) /* ZLINE: 1040 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl */
#line 1040 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  {
#line 1042 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    chpl_decRefCountsForDomainsInArrayEltTypes5(((BaseArr_chpl)(this_chpl31)));
#line 1042 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  }
#line 1039 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  return;
#line 1039 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
}

#line 1039 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
/* ChapelDistribution.chpl:1039 */
#line 1039 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
static void decEltCountsIfNeeded_chpl8(BaseRectangularArr_2_int64_t_one__real64_chpl this_chpl31,
#line 1039 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                                       int64_t _ln_chpl,
#line 1039 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                                       int32_t _fn_chpl) {
#line 1040 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseArr_chpl coerce_tmp_chpl3 = NULL;
#line 1040 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  coerce_tmp_chpl3 = ((BaseArr_chpl)(this_chpl31));
#line 1040 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 1040 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  if ((coerce_tmp_chpl3)->_decEltRefCounts) /* ZLINE: 1040 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl */
#line 1040 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  {
#line 1042 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    chpl_decRefCountsForDomainsInArrayEltTypes2(((BaseArr_chpl)(this_chpl31)));
#line 1042 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  }
#line 1039 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  return;
#line 1039 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
}

#line 1039 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
/* ChapelDistribution.chpl:1039 */
#line 1039 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
static void decEltCountsIfNeeded_chpl7(BaseRectangularArr_4_int64_t_one__real64_chpl this_chpl31,
#line 1039 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                                       int64_t _ln_chpl,
#line 1039 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                                       int32_t _fn_chpl) {
#line 1040 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseArr_chpl coerce_tmp_chpl3 = NULL;
#line 1040 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  coerce_tmp_chpl3 = ((BaseArr_chpl)(this_chpl31));
#line 1040 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 1040 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  if ((coerce_tmp_chpl3)->_decEltRefCounts) /* ZLINE: 1040 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl */
#line 1040 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  {
#line 1042 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    chpl_decRefCountsForDomainsInArrayEltTypes2(((BaseArr_chpl)(this_chpl31)));
#line 1042 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  }
#line 1039 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  return;
#line 1039 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
}

#line 1039 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
/* ChapelDistribution.chpl:1039 */
#line 1039 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
static void decEltCountsIfNeeded_chpl10(BaseRectangularArr_1_int64_t_one_Tensor_1__real64_chpl this_chpl31,
#line 1039 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                                        int64_t _ln_chpl,
#line 1039 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                                        int32_t _fn_chpl) {
#line 1040 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseArr_chpl coerce_tmp_chpl3 = NULL;
#line 1040 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  coerce_tmp_chpl3 = ((BaseArr_chpl)(this_chpl31));
#line 1040 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 1040 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  if ((coerce_tmp_chpl3)->_decEltRefCounts) /* ZLINE: 1040 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl */
#line 1040 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  {
#line 1042 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    chpl_decRefCountsForDomainsInArrayEltTypes10(((BaseArr_chpl)(this_chpl31)));
#line 1042 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  }
#line 1039 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  return;
#line 1039 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
}

#line 1039 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
/* ChapelDistribution.chpl:1039 */
#line 1039 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
static void decEltCountsIfNeeded_chpl5(BaseRectangularArr_1_int64_t_one__tuple_2_Tensor_3__real64_int64_t_chpl this_chpl31,
#line 1039 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                                       int64_t _ln_chpl,
#line 1039 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                                       int32_t _fn_chpl) {
#line 1040 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseArr_chpl coerce_tmp_chpl3 = NULL;
#line 1040 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  coerce_tmp_chpl3 = ((BaseArr_chpl)(this_chpl31));
#line 1040 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 1040 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  if ((coerce_tmp_chpl3)->_decEltRefCounts) /* ZLINE: 1040 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl */
#line 1040 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  {
#line 1042 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    chpl_decRefCountsForDomainsInArrayEltTypes7(((BaseArr_chpl)(this_chpl31)));
#line 1042 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  }
#line 1039 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  return;
#line 1039 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
}

#line 1039 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
/* ChapelDistribution.chpl:1039 */
#line 1039 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
static void decEltCountsIfNeeded_chpl16(BaseRectangularArr_1_int64_t_one_locale_chpl this_chpl31,
#line 1039 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                                        int64_t _ln_chpl,
#line 1039 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                                        int32_t _fn_chpl) {
#line 1040 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseArr_chpl coerce_tmp_chpl3 = NULL;
#line 1040 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  coerce_tmp_chpl3 = ((BaseArr_chpl)(this_chpl31));
#line 1040 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 1040 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  if ((coerce_tmp_chpl3)->_decEltRefCounts) /* ZLINE: 1040 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl */
#line 1040 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  {
#line 1042 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    chpl_decRefCountsForDomainsInArrayEltTypes8(((BaseArr_chpl)(this_chpl31)));
#line 1042 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  }
#line 1039 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  return;
#line 1039 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
}

#line 1039 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
/* ChapelDistribution.chpl:1039 */
#line 1039 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
static void decEltCountsIfNeeded_chpl11(BaseRectangularArr_1_int64_t_one_Tensor_3__real64_chpl this_chpl31,
#line 1039 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                                        int64_t _ln_chpl,
#line 1039 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                                        int32_t _fn_chpl) {
#line 1040 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseArr_chpl coerce_tmp_chpl3 = NULL;
#line 1040 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  coerce_tmp_chpl3 = ((BaseArr_chpl)(this_chpl31));
#line 1040 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 1040 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  if ((coerce_tmp_chpl3)->_decEltRefCounts) /* ZLINE: 1040 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl */
#line 1040 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  {
#line 1042 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    chpl_decRefCountsForDomainsInArrayEltTypes6(((BaseArr_chpl)(this_chpl31)));
#line 1042 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  }
#line 1039 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  return;
#line 1039 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
}

#line 1039 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
/* ChapelDistribution.chpl:1039 */
#line 1039 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
static void decEltCountsIfNeeded_chpl13(BaseRectangularArr_1_int64_t_one__array_DefaultRectangularArr_1_int64_t_one__real64_int64_t_chpl this_chpl31,
#line 1039 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                                        int64_t _ln_chpl,
#line 1039 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                                        int32_t _fn_chpl) {
#line 1040 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseArr_chpl coerce_tmp_chpl3 = NULL;
#line 1042 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl___RuntimeTypeInfo13 call_tmp_chpl6;
#line 1042 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseRectangularArr_1_int64_t_one__array_DefaultRectangularArr_1_int64_t_one__real64_int64_t_chpl borrowOfThis_chpl = NULL;
#line 1042 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl___RuntimeTypeInfo13 call_tmp_chpl7;
#line 1040 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  coerce_tmp_chpl3 = ((BaseArr_chpl)(this_chpl31));
#line 1040 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 1040 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  if ((coerce_tmp_chpl3)->_decEltRefCounts) /* ZLINE: 1040 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl */
#line 1040 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  {
#line 1042 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    borrowOfThis_chpl = this_chpl31;
#line 1042 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    chpl_check_nil(borrowOfThis_chpl, _ln_chpl, _fn_chpl);
#line 1042 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    call_tmp_chpl7 = (borrowOfThis_chpl)->eltType;
#line 1042 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    call_tmp_chpl6 = call_tmp_chpl7;
#line 1042 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    chpl_decRefCountsForDomainsInArrayEltTypes3(((BaseArr_chpl)(this_chpl31)), &call_tmp_chpl6, _ln_chpl, _fn_chpl);
#line 1042 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  }
#line 1039 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  return;
#line 1039 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
}

#line 1039 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
/* ChapelDistribution.chpl:1039 */
#line 1039 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
static void decEltCountsIfNeeded_chpl14(BaseRectangularArr_1_int64_t_one__array_DefaultRectangularArr_2_int64_t_one__real64_int64_t_chpl this_chpl31,
#line 1039 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                                        int64_t _ln_chpl,
#line 1039 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                                        int32_t _fn_chpl) {
#line 1040 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseArr_chpl coerce_tmp_chpl3 = NULL;
#line 1042 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl___RuntimeTypeInfo14 call_tmp_chpl6;
#line 1042 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseRectangularArr_1_int64_t_one__array_DefaultRectangularArr_2_int64_t_one__real64_int64_t_chpl borrowOfThis_chpl = NULL;
#line 1042 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl___RuntimeTypeInfo14 call_tmp_chpl7;
#line 1040 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  coerce_tmp_chpl3 = ((BaseArr_chpl)(this_chpl31));
#line 1040 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 1040 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  if ((coerce_tmp_chpl3)->_decEltRefCounts) /* ZLINE: 1040 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl */
#line 1040 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  {
#line 1042 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    borrowOfThis_chpl = this_chpl31;
#line 1042 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    chpl_check_nil(borrowOfThis_chpl, _ln_chpl, _fn_chpl);
#line 1042 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    call_tmp_chpl7 = (borrowOfThis_chpl)->eltType;
#line 1042 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    call_tmp_chpl6 = call_tmp_chpl7;
#line 1042 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    chpl_decRefCountsForDomainsInArrayEltTypes(((BaseArr_chpl)(this_chpl31)), &call_tmp_chpl6, _ln_chpl, _fn_chpl);
#line 1042 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  }
#line 1039 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  return;
#line 1039 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
}

#line 1039 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
/* ChapelDistribution.chpl:1039 */
#line 1039 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
static void decEltCountsIfNeeded_chpl4(BaseRectangularArr_1_int64_t_one_int64_t_chpl this_chpl31,
#line 1039 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                                       int64_t _ln_chpl,
#line 1039 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                                       int32_t _fn_chpl) {
#line 1040 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseArr_chpl coerce_tmp_chpl3 = NULL;
#line 1040 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  coerce_tmp_chpl3 = ((BaseArr_chpl)(this_chpl31));
#line 1040 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 1040 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  if ((coerce_tmp_chpl3)->_decEltRefCounts) /* ZLINE: 1040 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl */
#line 1040 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  {
#line 1042 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    chpl_decRefCountsForDomainsInArrayEltTypes9(((BaseArr_chpl)(this_chpl31)));
#line 1042 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  }
#line 1039 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  return;
#line 1039 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
}

#line 1039 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
/* ChapelDistribution.chpl:1039 */
#line 1039 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
static void decEltCountsIfNeeded_chpl15(BaseRectangularArr_1_int64_t_one__real64_chpl this_chpl31,
#line 1039 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                                        int64_t _ln_chpl,
#line 1039 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                                        int32_t _fn_chpl) {
#line 1040 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseArr_chpl coerce_tmp_chpl3 = NULL;
#line 1040 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  coerce_tmp_chpl3 = ((BaseArr_chpl)(this_chpl31));
#line 1040 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 1040 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  if ((coerce_tmp_chpl3)->_decEltRefCounts) /* ZLINE: 1040 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl */
#line 1040 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  {
#line 1042 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    chpl_decRefCountsForDomainsInArrayEltTypes2(((BaseArr_chpl)(this_chpl31)));
#line 1042 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  }
#line 1039 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  return;
#line 1039 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
}

#line 1039 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
/* ChapelDistribution.chpl:1039 */
#line 1039 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
static void decEltCountsIfNeeded_chpl9(BaseRectangularArr_1_int64_t_one_uint64_t_chpl this_chpl31,
#line 1039 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                                       int64_t _ln_chpl,
#line 1039 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                                       int32_t _fn_chpl) {
#line 1040 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseArr_chpl coerce_tmp_chpl3 = NULL;
#line 1040 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  coerce_tmp_chpl3 = ((BaseArr_chpl)(this_chpl31));
#line 1040 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 1040 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  if ((coerce_tmp_chpl3)->_decEltRefCounts) /* ZLINE: 1040 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl */
#line 1040 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  {
#line 1042 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    chpl_decRefCountsForDomainsInArrayEltTypes4(((BaseArr_chpl)(this_chpl31)));
#line 1042 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  }
#line 1039 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  return;
#line 1039 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
}

#line 1160 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
/* ChapelDistribution.chpl:1160 */
#line 1160 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
static void _delete_dist_chpl(BaseDist_chpl dist_chpl,
#line 1160 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                              chpl_bool privatized_chpl,
#line 1160 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                              int64_t _ln_chpl,
#line 1160 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                              int32_t _fn_chpl) {
#line 1161 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(dist_chpl, _ln_chpl, _fn_chpl);
#line 1161 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  ((void(*)(BaseDist_chpl))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(dist_chpl))->chpl__cid) + INT64(1))])(dist_chpl);
#line 1167 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl__delete38(dist_chpl, _ln_chpl, _fn_chpl);
#line 1160 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  return;
#line 1160 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
}

#line 1170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
/* ChapelDistribution.chpl:1170 */
#line 1170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
static void _delete_dom_chpl7(BaseDom_chpl dom_chpl,
#line 1170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                              chpl_bool privatized_chpl,
#line 1170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                              int64_t _ln_chpl,
#line 1170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                              int32_t _fn_chpl) {
#line 1172 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(dom_chpl, _ln_chpl, _fn_chpl);
#line 1172 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  ((void(*)(BaseDom_chpl,
#line 1172 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
     int64_t,
#line 1172 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
     int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(dom_chpl))->chpl__cid) + INT64(3))])(dom_chpl, _ln_chpl, _fn_chpl);
#line 1178 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl__delete39(dom_chpl, _ln_chpl, _fn_chpl);
#line 1170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  return;
#line 1170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
}

#line 1170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
/* ChapelDistribution.chpl:1170 */
#line 1170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
static void _delete_dom_chpl(DefaultRectangularDom_3_int64_t_one_chpl dom_chpl,
#line 1170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                             chpl_bool privatized_chpl,
#line 1170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                             int64_t _ln_chpl,
#line 1170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                             int32_t _fn_chpl) {
#line 1172 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseDom_chpl coerce_tmp_chpl3 = NULL;
#line 1172 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  coerce_tmp_chpl3 = ((BaseDom_chpl)(dom_chpl));
#line 1172 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 1172 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  ((void(*)(BaseDom_chpl,
#line 1172 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
     int64_t,
#line 1172 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
     int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(coerce_tmp_chpl3))->chpl__cid) + INT64(3))])(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 1178 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl__delete59(dom_chpl, _ln_chpl, _fn_chpl);
#line 1170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  return;
#line 1170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
}

#line 1170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
/* ChapelDistribution.chpl:1170 */
#line 1170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
static void _delete_dom_chpl6(DefaultRectangularDom_4_int64_t_one_chpl dom_chpl,
#line 1170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                              chpl_bool privatized_chpl,
#line 1170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                              int64_t _ln_chpl,
#line 1170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                              int32_t _fn_chpl) {
#line 1172 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseDom_chpl coerce_tmp_chpl3 = NULL;
#line 1172 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  coerce_tmp_chpl3 = ((BaseDom_chpl)(dom_chpl));
#line 1172 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 1172 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  ((void(*)(BaseDom_chpl,
#line 1172 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
     int64_t,
#line 1172 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
     int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(coerce_tmp_chpl3))->chpl__cid) + INT64(3))])(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 1178 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl__delete56(dom_chpl, _ln_chpl, _fn_chpl);
#line 1170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  return;
#line 1170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
}

#line 1170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
/* ChapelDistribution.chpl:1170 */
#line 1170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
static void _delete_dom_chpl5(DefaultRectangularDom_2_int64_t_one_chpl dom_chpl,
#line 1170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                              chpl_bool privatized_chpl,
#line 1170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                              int64_t _ln_chpl,
#line 1170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                              int32_t _fn_chpl) {
#line 1172 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseDom_chpl coerce_tmp_chpl3 = NULL;
#line 1172 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  coerce_tmp_chpl3 = ((BaseDom_chpl)(dom_chpl));
#line 1172 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 1172 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  ((void(*)(BaseDom_chpl,
#line 1172 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
     int64_t,
#line 1172 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
     int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(coerce_tmp_chpl3))->chpl__cid) + INT64(3))])(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 1178 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl__delete3(dom_chpl, _ln_chpl, _fn_chpl);
#line 1170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  return;
#line 1170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
}

#line 1170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
/* ChapelDistribution.chpl:1170 */
#line 1170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
static void _delete_dom_chpl3(DefaultRectangularDom_1_int64_t_one_chpl dom_chpl,
#line 1170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                              chpl_bool privatized_chpl,
#line 1170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                              int64_t _ln_chpl,
#line 1170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                              int32_t _fn_chpl) {
#line 1172 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseDom_chpl coerce_tmp_chpl3 = NULL;
#line 1172 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  coerce_tmp_chpl3 = ((BaseDom_chpl)(dom_chpl));
#line 1172 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 1172 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  ((void(*)(BaseDom_chpl,
#line 1172 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
     int64_t,
#line 1172 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
     int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(coerce_tmp_chpl3))->chpl__cid) + INT64(3))])(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 1178 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl__delete60(dom_chpl, _ln_chpl, _fn_chpl);
#line 1170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  return;
#line 1170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
}

#line 1170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
/* ChapelDistribution.chpl:1170 */
#line 1170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
static void _delete_dom_chpl9(ArrayViewRankChangeDom_3_int64_t_one__tuple_4_star_chpl_bool__tuple_4_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_4_star_chpl_bool__tuple_4_star_int64_t_chpl dom_chpl,
#line 1170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                              chpl_bool privatized_chpl,
#line 1170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                              int64_t _ln_chpl,
#line 1170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                              int32_t _fn_chpl) {
#line 1172 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(dom_chpl, _ln_chpl, _fn_chpl);
#line 1172 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  dsiDestroyDom_chpl4(dom_chpl, _ln_chpl, _fn_chpl);
#line 1178 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl__delete21(dom_chpl, _ln_chpl, _fn_chpl);
#line 1170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  return;
#line 1170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
}

#line 1170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
/* ChapelDistribution.chpl:1170 */
#line 1170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
static void _delete_dom_chpl4(ArrayViewRankChangeDom_2_int64_t_one__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_3_star_chpl_bool__tuple_3_star_int64_t_chpl dom_chpl,
#line 1170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                              chpl_bool privatized_chpl,
#line 1170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                              int64_t _ln_chpl,
#line 1170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                              int32_t _fn_chpl) {
#line 1172 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(dom_chpl, _ln_chpl, _fn_chpl);
#line 1172 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  dsiDestroyDom_chpl(dom_chpl, _ln_chpl, _fn_chpl);
#line 1178 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl__delete70(dom_chpl, _ln_chpl, _fn_chpl);
#line 1170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  return;
#line 1170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
}

#line 1170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
/* ChapelDistribution.chpl:1170 */
#line 1170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
static void _delete_dom_chpl2(ArrayViewRankChangeDom_1_int64_t_one__tuple_2_star_chpl_bool__tuple_2_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_2_star_chpl_bool__tuple_2_star_int64_t_chpl dom_chpl,
#line 1170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                              chpl_bool privatized_chpl,
#line 1170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                              int64_t _ln_chpl,
#line 1170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                              int32_t _fn_chpl) {
#line 1172 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(dom_chpl, _ln_chpl, _fn_chpl);
#line 1172 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  dsiDestroyDom_chpl2(dom_chpl, _ln_chpl, _fn_chpl);
#line 1178 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl__delete32(dom_chpl, _ln_chpl, _fn_chpl);
#line 1170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  return;
#line 1170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
}

#line 1170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
/* ChapelDistribution.chpl:1170 */
#line 1170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
static void _delete_dom_chpl8(ArrayViewRankChangeDom_2_int64_t_one__tuple_4_star_chpl_bool__tuple_4_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_4_star_chpl_bool__tuple_4_star_int64_t_chpl dom_chpl,
#line 1170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                              chpl_bool privatized_chpl,
#line 1170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                              int64_t _ln_chpl,
#line 1170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                              int32_t _fn_chpl) {
#line 1172 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(dom_chpl, _ln_chpl, _fn_chpl);
#line 1172 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  dsiDestroyDom_chpl3(dom_chpl, _ln_chpl, _fn_chpl);
#line 1178 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl__delete13(dom_chpl, _ln_chpl, _fn_chpl);
#line 1170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  return;
#line 1170 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
}

#line 1181 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
/* ChapelDistribution.chpl:1181 */
#line 1181 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
static void _delete_arr_chpl(BaseArr_chpl arr_chpl,
#line 1181 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                             chpl_bool deinitElts_chpl,
#line 1181 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                             int64_t _ln_chpl,
#line 1181 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                             int32_t _fn_chpl) {
#line 1191 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseArr_chpl coerce_tmp_chpl3 = NULL;
#line 1184 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(arr_chpl, _ln_chpl, _fn_chpl);
#line 1184 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  ((void(*)(BaseArr_chpl,
#line 1184 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
     chpl_bool,
#line 1184 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
     int64_t,
#line 1184 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
     int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(arr_chpl))->chpl__cid) + INT64(3))])(arr_chpl, deinitElts_chpl, _ln_chpl, _fn_chpl);
#line 1191 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  coerce_tmp_chpl3 = arr_chpl;
#line 1191 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 1191 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  ((void(*)(BaseArr_chpl,
#line 1191 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
     int64_t,
#line 1191 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
     int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(coerce_tmp_chpl3))->chpl__cid) + INT64(2))])(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 1198 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl__delete44(arr_chpl, _ln_chpl, _fn_chpl);
#line 1181 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  return;
#line 1181 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
}

#line 1203 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
/* ChapelDistribution.chpl:1203 */
#line 1203 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
static void castIndices_chpl3(_tuple_1_star_range_int64_t_both_one_chpl * from_chpl,
#line 1203 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                              DefaultRectangularDom_1_int64_t_one_chpl lhs_chpl,
#line 1203 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                              _tuple_1_star_range_int64_t_both_one_chpl * _retArg_chpl) {
#line 1203 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  _tuple_1_star_range_int64_t_both_one_chpl ret_chpl;
#line 1205 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  compilerAssert_chpl();
#line 1209 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  *(ret_chpl + INT64(0)) = *(*(from_chpl) + INT64(0));
#line 1203 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  *(*(_retArg_chpl) + INT64(0)) = *(ret_chpl + INT64(0));
#line 1203 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  return;
#line 1203 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
}

#line 1203 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
/* ChapelDistribution.chpl:1203 */
#line 1203 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
static void castIndices_chpl4(_tuple_4_star_range_int64_t_both_one_chpl * from_chpl,
#line 1203 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                              DefaultRectangularDom_4_int64_t_one_chpl lhs_chpl,
#line 1203 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                              _tuple_4_star_range_int64_t_both_one_chpl * _retArg_chpl) {
#line 1203 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  _tuple_4_star_range_int64_t_both_one_chpl ret_chpl;
#line 1205 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  compilerAssert_chpl();
#line 1209 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  *(ret_chpl + INT64(0)) = *(*(from_chpl) + INT64(0));
#line 1209 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  *(ret_chpl + INT64(1)) = *(*(from_chpl) + INT64(1));
#line 1209 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  *(ret_chpl + INT64(2)) = *(*(from_chpl) + INT64(2));
#line 1209 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  *(ret_chpl + INT64(3)) = *(*(from_chpl) + INT64(3));
#line 1203 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  *(*(_retArg_chpl) + INT64(0)) = *(ret_chpl + INT64(0));
#line 1203 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  *(*(_retArg_chpl) + INT64(1)) = *(ret_chpl + INT64(1));
#line 1203 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  *(*(_retArg_chpl) + INT64(2)) = *(ret_chpl + INT64(2));
#line 1203 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  *(*(_retArg_chpl) + INT64(3)) = *(ret_chpl + INT64(3));
#line 1203 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  return;
#line 1203 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
}

#line 1203 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
/* ChapelDistribution.chpl:1203 */
#line 1203 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
static void castIndices_chpl2(_tuple_2_star_range_int64_t_both_one_chpl * from_chpl,
#line 1203 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                              DefaultRectangularDom_2_int64_t_one_chpl lhs_chpl,
#line 1203 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                              _tuple_2_star_range_int64_t_both_one_chpl * _retArg_chpl) {
#line 1203 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  _tuple_2_star_range_int64_t_both_one_chpl ret_chpl;
#line 1205 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  compilerAssert_chpl();
#line 1209 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  *(ret_chpl + INT64(0)) = *(*(from_chpl) + INT64(0));
#line 1209 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  *(ret_chpl + INT64(1)) = *(*(from_chpl) + INT64(1));
#line 1203 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  *(*(_retArg_chpl) + INT64(0)) = *(ret_chpl + INT64(0));
#line 1203 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  *(*(_retArg_chpl) + INT64(1)) = *(ret_chpl + INT64(1));
#line 1203 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  return;
#line 1203 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
}

#line 1203 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
/* ChapelDistribution.chpl:1203 */
#line 1203 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
static void castIndices_chpl(_tuple_3_star_range_int64_t_both_one_chpl * from_chpl,
#line 1203 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                             DefaultRectangularDom_3_int64_t_one_chpl lhs_chpl,
#line 1203 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                             _tuple_3_star_range_int64_t_both_one_chpl * _retArg_chpl) {
#line 1203 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  _tuple_3_star_range_int64_t_both_one_chpl ret_chpl;
#line 1205 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  compilerAssert_chpl();
#line 1209 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  *(ret_chpl + INT64(0)) = *(*(from_chpl) + INT64(0));
#line 1209 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  *(ret_chpl + INT64(1)) = *(*(from_chpl) + INT64(1));
#line 1209 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  *(ret_chpl + INT64(2)) = *(*(from_chpl) + INT64(2));
#line 1203 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  *(*(_retArg_chpl) + INT64(0)) = *(ret_chpl + INT64(0));
#line 1203 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  *(*(_retArg_chpl) + INT64(1)) = *(ret_chpl + INT64(1));
#line 1203 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  *(*(_retArg_chpl) + INT64(2)) = *(ret_chpl + INT64(2));
#line 1203 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  return;
#line 1203 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
}

#line 1223 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
/* ChapelDistribution.chpl:1223 */
#line 1223 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
static void chpl_assignDomainWithGetSetIndices3(DefaultRectangularDom_1_int64_t_one_chpl lhs_chpl,
#line 1223 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                                                _domain_DefaultRectangularDom_1_int64_t_one_chpl * rhs_chpl,
#line 1223 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                                                int64_t _ln_chpl,
#line 1223 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                                                int32_t _fn_chpl) {
#line 1226 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  _tuple_1_star_range_int64_t_both_one_chpl call_tmp_chpl6;
#line 1226 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  _tuple_1_star_range_int64_t_both_one_chpl ret_tmp_chpl;
#line 1226 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  _tuple_1_star_range_int64_t_both_one_chpl rhsInds_chpl;
#line 1229 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseDom_chpl this_chpl31 = NULL;
#line 1229 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseArr_chpl tmp_chpl = NULL;
#line 1229 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseArr_chpl coerce_tmp_chpl3 = NULL;
#line 1229 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_bool tmp_chpl2;
#line 1231 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseArrOverRectangularDom_1_int64_t_one_chpl _formal_type_tmp__chpl = NULL;
#line 1231 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseArrOverRectangularDom_1_int64_t_one_chpl ret_chpl = NULL;
#line 1232 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  _tuple_1_star_range_int64_t_both_one_chpl call_tmp_chpl7;
#line 1232 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  _tuple_1_star_range_int64_t_both_one_chpl coerce_tmp_chpl4;
#line 1232 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  _tuple_1_star_range_int64_t_both_one_chpl ret_tmp_chpl2;
#line 1229 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseArr_chpl call_tmp_chpl8 = NULL;
#line 1229 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseArr_chpl coerce_tmp_chpl5 = NULL;
#line 1241 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  _tuple_1_star_range_int64_t_both_one_chpl coerce_tmp_chpl6;
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseDom_chpl this_chpl32 = NULL;
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseArr_chpl tmp_chpl3 = NULL;
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseArr_chpl coerce_tmp_chpl7 = NULL;
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_bool tmp_chpl4;
#line 1244 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseArrOverRectangularDom_1_int64_t_one_chpl _formal_type_tmp__chpl2 = NULL;
#line 1244 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseArrOverRectangularDom_1_int64_t_one_chpl ret_chpl2 = NULL;
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseArr_chpl call_tmp_chpl9 = NULL;
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseArr_chpl coerce_tmp_chpl8 = NULL;
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  int32_t chpl_macro_tmp_89;
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  int32_t chpl_macro_tmp_90;
#line 1226 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  getIndices_chpl5(rhs_chpl, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 1226 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  *(call_tmp_chpl6 + INT64(0)) = *(ret_tmp_chpl + INT64(0));
#line 1226 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  *(rhsInds_chpl + INT64(0)) = *(call_tmp_chpl6 + INT64(0));
#line 1229 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  this_chpl31 = ((BaseDom_chpl)(lhs_chpl));
#line 1229 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 1229 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  coerce_tmp_chpl3 = (this_chpl31)->_arrs_head;
#line 1229 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  tmp_chpl = coerce_tmp_chpl3;
#line 1229 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  tmp_chpl2 = (((RootClass_chpl)(coerce_tmp_chpl3)) != nil);
#line 1229 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  while (tmp_chpl2) {
#line 1229 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    if (((RootClass_chpl)(tmp_chpl)) == nil) /* ZLINE: 1229 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl */
#line 1229 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    {
#line 1229 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      nilCheckHalt_chpl(&_str_literal_52_chpl, _ln_chpl, _fn_chpl);
#line 1229 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    }
#line 1231 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    if (((RootClass_chpl)(tmp_chpl)) == nil) /* ZLINE: 1231 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl */
#line 1231 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    {
#line 1231 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      ret_chpl = ((BaseArrOverRectangularDom_1_int64_t_one_chpl)(nil));
#line 1231 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      goto _end__COLON__chpl;
#line 1231 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    }
#line 1231 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    chpl_macro_tmp_89 = ((RootClass_chpl)(tmp_chpl))->chpl__cid;
#line 1231 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    ret_chpl = ( (((chpl__cid_BaseArrOverRectangularDom_1_int64_t_one_chpl <= chpl_macro_tmp_89) && (chpl_macro_tmp_89 <= chpl_subclass_max_id[chpl__cid_BaseArrOverRectangularDom_1_int64_t_one_chpl])))?(((BaseArrOverRectangularDom_1_int64_t_one_chpl)(tmp_chpl))):(((BaseArrOverRectangularDom_1_int64_t_one_chpl)(NULL))) );
#line 1231 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    _end__COLON__chpl:;
#line 1231 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    if (((RootClass_chpl)(ret_chpl)) != nil) /* ZLINE: 1231 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl */
#line 1231 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    {
#line 1232 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      *(coerce_tmp_chpl4 + INT64(0)) = *(rhsInds_chpl + INT64(0));
#line 1232 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      castIndices_chpl3(&coerce_tmp_chpl4, lhs_chpl, &ret_tmp_chpl2);
#line 1232 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      *(call_tmp_chpl7 + INT64(0)) = *(ret_tmp_chpl2 + INT64(0));
#line 1232 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      chpl_check_nil(ret_chpl, _ln_chpl, _fn_chpl);
#line 1232 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      ((void(*)(BaseArrOverRectangularDom_1_int64_t_one_chpl,
#line 1232 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
         _tuple_1_star_range_int64_t_both_one_chpl *,
#line 1232 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
         int64_t,
#line 1232 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
         int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(ret_chpl))->chpl__cid) + INT64(5))])(ret_chpl, &call_tmp_chpl7, _ln_chpl, _fn_chpl);
#line 1232 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    } else /* ZLINE: 1231 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl */
#line 1232 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    {
#line 1235 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      halt_chpl8(&_str_literal_1180_chpl, &_str_literal_4063_chpl, &_str_literal_1181_chpl, &_str_literal_4064_chpl, _ln_chpl, _fn_chpl);
#line 1235 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    }
#line 1229 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    if (((RootClass_chpl)(tmp_chpl)) == nil) /* ZLINE: 1229 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl */
#line 1229 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    {
#line 1229 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      nilCheckHalt_chpl(&_str_literal_52_chpl, _ln_chpl, _fn_chpl);
#line 1229 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    }
#line 1229 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    call_tmp_chpl8 = tmp_chpl;
#line 1229 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    chpl_check_nil(call_tmp_chpl8, _ln_chpl, _fn_chpl);
#line 1229 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    coerce_tmp_chpl5 = (call_tmp_chpl8)->next;
#line 1229 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    tmp_chpl = coerce_tmp_chpl5;
#line 1229 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    tmp_chpl2 = (((RootClass_chpl)(coerce_tmp_chpl5)) != nil);
#line 1229 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  }
#line 1241 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  *(coerce_tmp_chpl6 + INT64(0)) = *(rhsInds_chpl + INT64(0));
#line 1241 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(lhs_chpl, _ln_chpl, _fn_chpl);
#line 1241 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  dsiSetIndices_chpl6(lhs_chpl, &coerce_tmp_chpl6, _ln_chpl, _fn_chpl);
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  this_chpl32 = ((BaseDom_chpl)(lhs_chpl));
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(this_chpl32, _ln_chpl, _fn_chpl);
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  coerce_tmp_chpl7 = (this_chpl32)->_arrs_head;
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  tmp_chpl3 = coerce_tmp_chpl7;
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  tmp_chpl4 = (((RootClass_chpl)(coerce_tmp_chpl7)) != nil);
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  while (tmp_chpl4) {
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    if (((RootClass_chpl)(tmp_chpl3)) == nil) /* ZLINE: 1243 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl */
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    {
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      nilCheckHalt_chpl(&_str_literal_52_chpl, _ln_chpl, _fn_chpl);
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    }
#line 1244 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    if (((RootClass_chpl)(tmp_chpl3)) == nil) /* ZLINE: 1244 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl */
#line 1244 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    {
#line 1244 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      ret_chpl2 = ((BaseArrOverRectangularDom_1_int64_t_one_chpl)(nil));
#line 1244 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      goto _end__COLON__chpl2;
#line 1244 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    }
#line 1244 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    chpl_macro_tmp_90 = ((RootClass_chpl)(tmp_chpl3))->chpl__cid;
#line 1244 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    ret_chpl2 = ( (((chpl__cid_BaseArrOverRectangularDom_1_int64_t_one_chpl <= chpl_macro_tmp_90) && (chpl_macro_tmp_90 <= chpl_subclass_max_id[chpl__cid_BaseArrOverRectangularDom_1_int64_t_one_chpl])))?(((BaseArrOverRectangularDom_1_int64_t_one_chpl)(tmp_chpl3))):(((BaseArrOverRectangularDom_1_int64_t_one_chpl)(NULL))) );
#line 1244 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    _end__COLON__chpl2:;
#line 1245 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    if (((RootClass_chpl)(ret_chpl2)) == nil) /* ZLINE: 1245 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl */
#line 1245 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    {
#line 1245 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      nilCheckHalt_chpl(&_str_literal_52_chpl, _ln_chpl, _fn_chpl);
#line 1245 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    }
#line 1246 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    chpl_check_nil(ret_chpl2, _ln_chpl, _fn_chpl);
#line 1246 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    ((void(*)(BaseArrOverRectangularDom_1_int64_t_one_chpl))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(ret_chpl2))->chpl__cid) + INT64(4))])(ret_chpl2);
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    if (((RootClass_chpl)(tmp_chpl3)) == nil) /* ZLINE: 1243 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl */
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    {
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      nilCheckHalt_chpl(&_str_literal_52_chpl, _ln_chpl, _fn_chpl);
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    }
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    call_tmp_chpl9 = tmp_chpl3;
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    chpl_check_nil(call_tmp_chpl9, _ln_chpl, _fn_chpl);
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    coerce_tmp_chpl8 = (call_tmp_chpl9)->next;
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    tmp_chpl3 = coerce_tmp_chpl8;
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    tmp_chpl4 = (((RootClass_chpl)(coerce_tmp_chpl8)) != nil);
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  }
#line 1223 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  return;
#line 1223 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
}

#line 1223 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
/* ChapelDistribution.chpl:1223 */
#line 1223 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
static void chpl_assignDomainWithGetSetIndices5(DefaultRectangularDom_4_int64_t_one_chpl lhs_chpl,
#line 1223 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                                                _domain_DefaultRectangularDom_4_int64_t_one_chpl * rhs_chpl,
#line 1223 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                                                int64_t _ln_chpl,
#line 1223 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                                                int32_t _fn_chpl) {
#line 1226 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  _tuple_4_star_range_int64_t_both_one_chpl call_tmp_chpl6;
#line 1226 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  _tuple_4_star_range_int64_t_both_one_chpl ret_tmp_chpl;
#line 1226 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  _tuple_4_star_range_int64_t_both_one_chpl rhsInds_chpl;
#line 1229 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseDom_chpl this_chpl31 = NULL;
#line 1229 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseArr_chpl tmp_chpl = NULL;
#line 1229 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseArr_chpl coerce_tmp_chpl3 = NULL;
#line 1229 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_bool tmp_chpl2;
#line 1231 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseArrOverRectangularDom_4_int64_t_one_chpl _formal_type_tmp__chpl = NULL;
#line 1231 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseArrOverRectangularDom_4_int64_t_one_chpl ret_chpl = NULL;
#line 1232 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  _tuple_4_star_range_int64_t_both_one_chpl call_tmp_chpl7;
#line 1232 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  _tuple_4_star_range_int64_t_both_one_chpl coerce_tmp_chpl4;
#line 1232 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  _tuple_4_star_range_int64_t_both_one_chpl ret_tmp_chpl2;
#line 1229 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseArr_chpl call_tmp_chpl8 = NULL;
#line 1229 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseArr_chpl coerce_tmp_chpl5 = NULL;
#line 1241 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  _tuple_4_star_range_int64_t_both_one_chpl coerce_tmp_chpl6;
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseDom_chpl this_chpl32 = NULL;
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseArr_chpl tmp_chpl3 = NULL;
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseArr_chpl coerce_tmp_chpl7 = NULL;
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_bool tmp_chpl4;
#line 1244 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseArrOverRectangularDom_4_int64_t_one_chpl _formal_type_tmp__chpl2 = NULL;
#line 1244 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseArrOverRectangularDom_4_int64_t_one_chpl ret_chpl2 = NULL;
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseArr_chpl call_tmp_chpl9 = NULL;
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseArr_chpl coerce_tmp_chpl8 = NULL;
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  int32_t chpl_macro_tmp_91;
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  int32_t chpl_macro_tmp_92;
#line 1226 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  getIndices_chpl4(rhs_chpl, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 1226 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  *(call_tmp_chpl6 + INT64(0)) = *(ret_tmp_chpl + INT64(0));
#line 1226 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  *(call_tmp_chpl6 + INT64(1)) = *(ret_tmp_chpl + INT64(1));
#line 1226 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  *(call_tmp_chpl6 + INT64(2)) = *(ret_tmp_chpl + INT64(2));
#line 1226 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  *(call_tmp_chpl6 + INT64(3)) = *(ret_tmp_chpl + INT64(3));
#line 1226 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  *(rhsInds_chpl + INT64(0)) = *(call_tmp_chpl6 + INT64(0));
#line 1226 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  *(rhsInds_chpl + INT64(1)) = *(call_tmp_chpl6 + INT64(1));
#line 1226 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  *(rhsInds_chpl + INT64(2)) = *(call_tmp_chpl6 + INT64(2));
#line 1226 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  *(rhsInds_chpl + INT64(3)) = *(call_tmp_chpl6 + INT64(3));
#line 1229 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  this_chpl31 = ((BaseDom_chpl)(lhs_chpl));
#line 1229 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 1229 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  coerce_tmp_chpl3 = (this_chpl31)->_arrs_head;
#line 1229 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  tmp_chpl = coerce_tmp_chpl3;
#line 1229 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  tmp_chpl2 = (((RootClass_chpl)(coerce_tmp_chpl3)) != nil);
#line 1229 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  while (tmp_chpl2) {
#line 1229 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    if (((RootClass_chpl)(tmp_chpl)) == nil) /* ZLINE: 1229 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl */
#line 1229 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    {
#line 1229 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      nilCheckHalt_chpl(&_str_literal_52_chpl, _ln_chpl, _fn_chpl);
#line 1229 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    }
#line 1231 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    if (((RootClass_chpl)(tmp_chpl)) == nil) /* ZLINE: 1231 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl */
#line 1231 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    {
#line 1231 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      ret_chpl = ((BaseArrOverRectangularDom_4_int64_t_one_chpl)(nil));
#line 1231 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      goto _end__COLON__chpl;
#line 1231 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    }
#line 1231 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    chpl_macro_tmp_91 = ((RootClass_chpl)(tmp_chpl))->chpl__cid;
#line 1231 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    ret_chpl = ( (((chpl__cid_BaseArrOverRectangularDom_4_int64_t_one_chpl <= chpl_macro_tmp_91) && (chpl_macro_tmp_91 <= chpl_subclass_max_id[chpl__cid_BaseArrOverRectangularDom_4_int64_t_one_chpl])))?(((BaseArrOverRectangularDom_4_int64_t_one_chpl)(tmp_chpl))):(((BaseArrOverRectangularDom_4_int64_t_one_chpl)(NULL))) );
#line 1231 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    _end__COLON__chpl:;
#line 1231 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    if (((RootClass_chpl)(ret_chpl)) != nil) /* ZLINE: 1231 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl */
#line 1231 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    {
#line 1232 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      *(coerce_tmp_chpl4 + INT64(0)) = *(rhsInds_chpl + INT64(0));
#line 1232 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      *(coerce_tmp_chpl4 + INT64(1)) = *(rhsInds_chpl + INT64(1));
#line 1232 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      *(coerce_tmp_chpl4 + INT64(2)) = *(rhsInds_chpl + INT64(2));
#line 1232 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      *(coerce_tmp_chpl4 + INT64(3)) = *(rhsInds_chpl + INT64(3));
#line 1232 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      castIndices_chpl4(&coerce_tmp_chpl4, lhs_chpl, &ret_tmp_chpl2);
#line 1232 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      *(call_tmp_chpl7 + INT64(0)) = *(ret_tmp_chpl2 + INT64(0));
#line 1232 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      *(call_tmp_chpl7 + INT64(1)) = *(ret_tmp_chpl2 + INT64(1));
#line 1232 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      *(call_tmp_chpl7 + INT64(2)) = *(ret_tmp_chpl2 + INT64(2));
#line 1232 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      *(call_tmp_chpl7 + INT64(3)) = *(ret_tmp_chpl2 + INT64(3));
#line 1232 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      chpl_check_nil(ret_chpl, _ln_chpl, _fn_chpl);
#line 1232 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      ((void(*)(BaseArrOverRectangularDom_4_int64_t_one_chpl,
#line 1232 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
         _tuple_4_star_range_int64_t_both_one_chpl *,
#line 1232 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
         int64_t,
#line 1232 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
         int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(ret_chpl))->chpl__cid) + INT64(4))])(ret_chpl, &call_tmp_chpl7, _ln_chpl, _fn_chpl);
#line 1232 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    } else /* ZLINE: 1231 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl */
#line 1232 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    {
#line 1235 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      halt_chpl8(&_str_literal_1180_chpl, &_str_literal_4109_chpl, &_str_literal_1181_chpl, &_str_literal_4110_chpl, _ln_chpl, _fn_chpl);
#line 1235 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    }
#line 1229 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    if (((RootClass_chpl)(tmp_chpl)) == nil) /* ZLINE: 1229 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl */
#line 1229 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    {
#line 1229 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      nilCheckHalt_chpl(&_str_literal_52_chpl, _ln_chpl, _fn_chpl);
#line 1229 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    }
#line 1229 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    call_tmp_chpl8 = tmp_chpl;
#line 1229 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    chpl_check_nil(call_tmp_chpl8, _ln_chpl, _fn_chpl);
#line 1229 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    coerce_tmp_chpl5 = (call_tmp_chpl8)->next;
#line 1229 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    tmp_chpl = coerce_tmp_chpl5;
#line 1229 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    tmp_chpl2 = (((RootClass_chpl)(coerce_tmp_chpl5)) != nil);
#line 1229 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  }
#line 1241 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  *(coerce_tmp_chpl6 + INT64(0)) = *(rhsInds_chpl + INT64(0));
#line 1241 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  *(coerce_tmp_chpl6 + INT64(1)) = *(rhsInds_chpl + INT64(1));
#line 1241 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  *(coerce_tmp_chpl6 + INT64(2)) = *(rhsInds_chpl + INT64(2));
#line 1241 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  *(coerce_tmp_chpl6 + INT64(3)) = *(rhsInds_chpl + INT64(3));
#line 1241 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(lhs_chpl, _ln_chpl, _fn_chpl);
#line 1241 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  dsiSetIndices_chpl8(lhs_chpl, &coerce_tmp_chpl6, _ln_chpl, _fn_chpl);
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  this_chpl32 = ((BaseDom_chpl)(lhs_chpl));
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(this_chpl32, _ln_chpl, _fn_chpl);
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  coerce_tmp_chpl7 = (this_chpl32)->_arrs_head;
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  tmp_chpl3 = coerce_tmp_chpl7;
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  tmp_chpl4 = (((RootClass_chpl)(coerce_tmp_chpl7)) != nil);
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  while (tmp_chpl4) {
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    if (((RootClass_chpl)(tmp_chpl3)) == nil) /* ZLINE: 1243 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl */
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    {
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      nilCheckHalt_chpl(&_str_literal_52_chpl, _ln_chpl, _fn_chpl);
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    }
#line 1244 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    if (((RootClass_chpl)(tmp_chpl3)) == nil) /* ZLINE: 1244 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl */
#line 1244 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    {
#line 1244 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      ret_chpl2 = ((BaseArrOverRectangularDom_4_int64_t_one_chpl)(nil));
#line 1244 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      goto _end__COLON__chpl2;
#line 1244 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    }
#line 1244 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    chpl_macro_tmp_92 = ((RootClass_chpl)(tmp_chpl3))->chpl__cid;
#line 1244 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    ret_chpl2 = ( (((chpl__cid_BaseArrOverRectangularDom_4_int64_t_one_chpl <= chpl_macro_tmp_92) && (chpl_macro_tmp_92 <= chpl_subclass_max_id[chpl__cid_BaseArrOverRectangularDom_4_int64_t_one_chpl])))?(((BaseArrOverRectangularDom_4_int64_t_one_chpl)(tmp_chpl3))):(((BaseArrOverRectangularDom_4_int64_t_one_chpl)(NULL))) );
#line 1244 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    _end__COLON__chpl2:;
#line 1245 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    if (((RootClass_chpl)(ret_chpl2)) == nil) /* ZLINE: 1245 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl */
#line 1245 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    {
#line 1245 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      nilCheckHalt_chpl(&_str_literal_52_chpl, _ln_chpl, _fn_chpl);
#line 1245 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    }
#line 1246 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    chpl_check_nil(ret_chpl2, _ln_chpl, _fn_chpl);
#line 1246 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    ((void(*)(BaseArrOverRectangularDom_4_int64_t_one_chpl))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(ret_chpl2))->chpl__cid) + INT64(5))])(ret_chpl2);
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    if (((RootClass_chpl)(tmp_chpl3)) == nil) /* ZLINE: 1243 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl */
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    {
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      nilCheckHalt_chpl(&_str_literal_52_chpl, _ln_chpl, _fn_chpl);
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    }
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    call_tmp_chpl9 = tmp_chpl3;
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    chpl_check_nil(call_tmp_chpl9, _ln_chpl, _fn_chpl);
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    coerce_tmp_chpl8 = (call_tmp_chpl9)->next;
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    tmp_chpl3 = coerce_tmp_chpl8;
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    tmp_chpl4 = (((RootClass_chpl)(coerce_tmp_chpl8)) != nil);
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  }
#line 1223 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  return;
#line 1223 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
}

#line 1223 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
/* ChapelDistribution.chpl:1223 */
#line 1223 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
static void chpl_assignDomainWithGetSetIndices4(DefaultRectangularDom_2_int64_t_one_chpl lhs_chpl,
#line 1223 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                                                _domain_DefaultRectangularDom_2_int64_t_one_chpl * rhs_chpl,
#line 1223 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                                                int64_t _ln_chpl,
#line 1223 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                                                int32_t _fn_chpl) {
#line 1226 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  _tuple_2_star_range_int64_t_both_one_chpl call_tmp_chpl6;
#line 1226 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  _tuple_2_star_range_int64_t_both_one_chpl ret_tmp_chpl;
#line 1226 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  _tuple_2_star_range_int64_t_both_one_chpl rhsInds_chpl;
#line 1229 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseDom_chpl this_chpl31 = NULL;
#line 1229 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseArr_chpl tmp_chpl = NULL;
#line 1229 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseArr_chpl coerce_tmp_chpl3 = NULL;
#line 1229 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_bool tmp_chpl2;
#line 1231 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseArrOverRectangularDom_2_int64_t_one_chpl _formal_type_tmp__chpl = NULL;
#line 1231 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseArrOverRectangularDom_2_int64_t_one_chpl ret_chpl = NULL;
#line 1232 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  _tuple_2_star_range_int64_t_both_one_chpl call_tmp_chpl7;
#line 1232 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  _tuple_2_star_range_int64_t_both_one_chpl coerce_tmp_chpl4;
#line 1232 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  _tuple_2_star_range_int64_t_both_one_chpl ret_tmp_chpl2;
#line 1229 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseArr_chpl call_tmp_chpl8 = NULL;
#line 1229 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseArr_chpl coerce_tmp_chpl5 = NULL;
#line 1241 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  _tuple_2_star_range_int64_t_both_one_chpl coerce_tmp_chpl6;
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseDom_chpl this_chpl32 = NULL;
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseArr_chpl tmp_chpl3 = NULL;
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseArr_chpl coerce_tmp_chpl7 = NULL;
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_bool tmp_chpl4;
#line 1244 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseArrOverRectangularDom_2_int64_t_one_chpl _formal_type_tmp__chpl2 = NULL;
#line 1244 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseArrOverRectangularDom_2_int64_t_one_chpl ret_chpl2 = NULL;
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseArr_chpl call_tmp_chpl9 = NULL;
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseArr_chpl coerce_tmp_chpl8 = NULL;
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  int32_t chpl_macro_tmp_93;
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  int32_t chpl_macro_tmp_94;
#line 1226 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  getIndices_chpl(rhs_chpl, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 1226 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  *(call_tmp_chpl6 + INT64(0)) = *(ret_tmp_chpl + INT64(0));
#line 1226 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  *(call_tmp_chpl6 + INT64(1)) = *(ret_tmp_chpl + INT64(1));
#line 1226 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  *(rhsInds_chpl + INT64(0)) = *(call_tmp_chpl6 + INT64(0));
#line 1226 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  *(rhsInds_chpl + INT64(1)) = *(call_tmp_chpl6 + INT64(1));
#line 1229 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  this_chpl31 = ((BaseDom_chpl)(lhs_chpl));
#line 1229 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 1229 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  coerce_tmp_chpl3 = (this_chpl31)->_arrs_head;
#line 1229 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  tmp_chpl = coerce_tmp_chpl3;
#line 1229 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  tmp_chpl2 = (((RootClass_chpl)(coerce_tmp_chpl3)) != nil);
#line 1229 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  while (tmp_chpl2) {
#line 1229 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    if (((RootClass_chpl)(tmp_chpl)) == nil) /* ZLINE: 1229 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl */
#line 1229 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    {
#line 1229 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      nilCheckHalt_chpl(&_str_literal_52_chpl, _ln_chpl, _fn_chpl);
#line 1229 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    }
#line 1231 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    if (((RootClass_chpl)(tmp_chpl)) == nil) /* ZLINE: 1231 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl */
#line 1231 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    {
#line 1231 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      ret_chpl = ((BaseArrOverRectangularDom_2_int64_t_one_chpl)(nil));
#line 1231 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      goto _end__COLON__chpl;
#line 1231 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    }
#line 1231 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    chpl_macro_tmp_93 = ((RootClass_chpl)(tmp_chpl))->chpl__cid;
#line 1231 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    ret_chpl = ( (((chpl__cid_BaseArrOverRectangularDom_2_int64_t_one_chpl <= chpl_macro_tmp_93) && (chpl_macro_tmp_93 <= chpl_subclass_max_id[chpl__cid_BaseArrOverRectangularDom_2_int64_t_one_chpl])))?(((BaseArrOverRectangularDom_2_int64_t_one_chpl)(tmp_chpl))):(((BaseArrOverRectangularDom_2_int64_t_one_chpl)(NULL))) );
#line 1231 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    _end__COLON__chpl:;
#line 1231 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    if (((RootClass_chpl)(ret_chpl)) != nil) /* ZLINE: 1231 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl */
#line 1231 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    {
#line 1232 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      *(coerce_tmp_chpl4 + INT64(0)) = *(rhsInds_chpl + INT64(0));
#line 1232 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      *(coerce_tmp_chpl4 + INT64(1)) = *(rhsInds_chpl + INT64(1));
#line 1232 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      castIndices_chpl2(&coerce_tmp_chpl4, lhs_chpl, &ret_tmp_chpl2);
#line 1232 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      *(call_tmp_chpl7 + INT64(0)) = *(ret_tmp_chpl2 + INT64(0));
#line 1232 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      *(call_tmp_chpl7 + INT64(1)) = *(ret_tmp_chpl2 + INT64(1));
#line 1232 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      chpl_check_nil(ret_chpl, _ln_chpl, _fn_chpl);
#line 1232 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      ((void(*)(BaseArrOverRectangularDom_2_int64_t_one_chpl,
#line 1232 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
         _tuple_2_star_range_int64_t_both_one_chpl *,
#line 1232 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
         int64_t,
#line 1232 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
         int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(ret_chpl))->chpl__cid) + INT64(5))])(ret_chpl, &call_tmp_chpl7, _ln_chpl, _fn_chpl);
#line 1232 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    } else /* ZLINE: 1231 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl */
#line 1232 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    {
#line 1235 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      halt_chpl8(&_str_literal_1180_chpl, &_str_literal_4171_chpl, &_str_literal_1181_chpl, &_str_literal_4172_chpl, _ln_chpl, _fn_chpl);
#line 1235 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    }
#line 1229 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    if (((RootClass_chpl)(tmp_chpl)) == nil) /* ZLINE: 1229 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl */
#line 1229 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    {
#line 1229 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      nilCheckHalt_chpl(&_str_literal_52_chpl, _ln_chpl, _fn_chpl);
#line 1229 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    }
#line 1229 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    call_tmp_chpl8 = tmp_chpl;
#line 1229 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    chpl_check_nil(call_tmp_chpl8, _ln_chpl, _fn_chpl);
#line 1229 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    coerce_tmp_chpl5 = (call_tmp_chpl8)->next;
#line 1229 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    tmp_chpl = coerce_tmp_chpl5;
#line 1229 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    tmp_chpl2 = (((RootClass_chpl)(coerce_tmp_chpl5)) != nil);
#line 1229 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  }
#line 1241 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  *(coerce_tmp_chpl6 + INT64(0)) = *(rhsInds_chpl + INT64(0));
#line 1241 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  *(coerce_tmp_chpl6 + INT64(1)) = *(rhsInds_chpl + INT64(1));
#line 1241 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(lhs_chpl, _ln_chpl, _fn_chpl);
#line 1241 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  dsiSetIndices_chpl5(lhs_chpl, &coerce_tmp_chpl6, _ln_chpl, _fn_chpl);
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  this_chpl32 = ((BaseDom_chpl)(lhs_chpl));
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(this_chpl32, _ln_chpl, _fn_chpl);
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  coerce_tmp_chpl7 = (this_chpl32)->_arrs_head;
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  tmp_chpl3 = coerce_tmp_chpl7;
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  tmp_chpl4 = (((RootClass_chpl)(coerce_tmp_chpl7)) != nil);
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  while (tmp_chpl4) {
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    if (((RootClass_chpl)(tmp_chpl3)) == nil) /* ZLINE: 1243 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl */
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    {
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      nilCheckHalt_chpl(&_str_literal_52_chpl, _ln_chpl, _fn_chpl);
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    }
#line 1244 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    if (((RootClass_chpl)(tmp_chpl3)) == nil) /* ZLINE: 1244 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl */
#line 1244 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    {
#line 1244 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      ret_chpl2 = ((BaseArrOverRectangularDom_2_int64_t_one_chpl)(nil));
#line 1244 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      goto _end__COLON__chpl2;
#line 1244 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    }
#line 1244 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    chpl_macro_tmp_94 = ((RootClass_chpl)(tmp_chpl3))->chpl__cid;
#line 1244 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    ret_chpl2 = ( (((chpl__cid_BaseArrOverRectangularDom_2_int64_t_one_chpl <= chpl_macro_tmp_94) && (chpl_macro_tmp_94 <= chpl_subclass_max_id[chpl__cid_BaseArrOverRectangularDom_2_int64_t_one_chpl])))?(((BaseArrOverRectangularDom_2_int64_t_one_chpl)(tmp_chpl3))):(((BaseArrOverRectangularDom_2_int64_t_one_chpl)(NULL))) );
#line 1244 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    _end__COLON__chpl2:;
#line 1245 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    if (((RootClass_chpl)(ret_chpl2)) == nil) /* ZLINE: 1245 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl */
#line 1245 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    {
#line 1245 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      nilCheckHalt_chpl(&_str_literal_52_chpl, _ln_chpl, _fn_chpl);
#line 1245 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    }
#line 1246 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    chpl_check_nil(ret_chpl2, _ln_chpl, _fn_chpl);
#line 1246 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    ((void(*)(BaseArrOverRectangularDom_2_int64_t_one_chpl))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(ret_chpl2))->chpl__cid) + INT64(4))])(ret_chpl2);
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    if (((RootClass_chpl)(tmp_chpl3)) == nil) /* ZLINE: 1243 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl */
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    {
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      nilCheckHalt_chpl(&_str_literal_52_chpl, _ln_chpl, _fn_chpl);
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    }
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    call_tmp_chpl9 = tmp_chpl3;
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    chpl_check_nil(call_tmp_chpl9, _ln_chpl, _fn_chpl);
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    coerce_tmp_chpl8 = (call_tmp_chpl9)->next;
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    tmp_chpl3 = coerce_tmp_chpl8;
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    tmp_chpl4 = (((RootClass_chpl)(coerce_tmp_chpl8)) != nil);
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  }
#line 1223 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  return;
#line 1223 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
}

#line 1223 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
/* ChapelDistribution.chpl:1223 */
#line 1223 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
static void chpl_assignDomainWithGetSetIndices2(DefaultRectangularDom_3_int64_t_one_chpl lhs_chpl,
#line 1223 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                                                _domain_DefaultRectangularDom_3_int64_t_one_chpl * rhs_chpl,
#line 1223 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                                                int64_t _ln_chpl,
#line 1223 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                                                int32_t _fn_chpl) {
#line 1226 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  _tuple_3_star_range_int64_t_both_one_chpl call_tmp_chpl6;
#line 1226 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  _tuple_3_star_range_int64_t_both_one_chpl ret_tmp_chpl;
#line 1226 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  _tuple_3_star_range_int64_t_both_one_chpl rhsInds_chpl;
#line 1229 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseDom_chpl this_chpl31 = NULL;
#line 1229 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseArr_chpl tmp_chpl = NULL;
#line 1229 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseArr_chpl coerce_tmp_chpl3 = NULL;
#line 1229 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_bool tmp_chpl2;
#line 1231 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseArrOverRectangularDom_3_int64_t_one_chpl _formal_type_tmp__chpl = NULL;
#line 1231 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseArrOverRectangularDom_3_int64_t_one_chpl ret_chpl = NULL;
#line 1232 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  _tuple_3_star_range_int64_t_both_one_chpl call_tmp_chpl7;
#line 1232 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  _tuple_3_star_range_int64_t_both_one_chpl coerce_tmp_chpl4;
#line 1232 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  _tuple_3_star_range_int64_t_both_one_chpl ret_tmp_chpl2;
#line 1229 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseArr_chpl call_tmp_chpl8 = NULL;
#line 1229 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseArr_chpl coerce_tmp_chpl5 = NULL;
#line 1241 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  _tuple_3_star_range_int64_t_both_one_chpl coerce_tmp_chpl6;
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseDom_chpl this_chpl32 = NULL;
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseArr_chpl tmp_chpl3 = NULL;
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseArr_chpl coerce_tmp_chpl7 = NULL;
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_bool tmp_chpl4;
#line 1244 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseArrOverRectangularDom_3_int64_t_one_chpl _formal_type_tmp__chpl2 = NULL;
#line 1244 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseArrOverRectangularDom_3_int64_t_one_chpl ret_chpl2 = NULL;
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseArr_chpl call_tmp_chpl9 = NULL;
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseArr_chpl coerce_tmp_chpl8 = NULL;
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  int32_t chpl_macro_tmp_95;
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  int32_t chpl_macro_tmp_96;
#line 1226 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  getIndices_chpl3(rhs_chpl, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 1226 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  *(call_tmp_chpl6 + INT64(0)) = *(ret_tmp_chpl + INT64(0));
#line 1226 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  *(call_tmp_chpl6 + INT64(1)) = *(ret_tmp_chpl + INT64(1));
#line 1226 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  *(call_tmp_chpl6 + INT64(2)) = *(ret_tmp_chpl + INT64(2));
#line 1226 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  *(rhsInds_chpl + INT64(0)) = *(call_tmp_chpl6 + INT64(0));
#line 1226 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  *(rhsInds_chpl + INT64(1)) = *(call_tmp_chpl6 + INT64(1));
#line 1226 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  *(rhsInds_chpl + INT64(2)) = *(call_tmp_chpl6 + INT64(2));
#line 1229 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  this_chpl31 = ((BaseDom_chpl)(lhs_chpl));
#line 1229 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 1229 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  coerce_tmp_chpl3 = (this_chpl31)->_arrs_head;
#line 1229 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  tmp_chpl = coerce_tmp_chpl3;
#line 1229 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  tmp_chpl2 = (((RootClass_chpl)(coerce_tmp_chpl3)) != nil);
#line 1229 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  while (tmp_chpl2) {
#line 1229 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    if (((RootClass_chpl)(tmp_chpl)) == nil) /* ZLINE: 1229 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl */
#line 1229 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    {
#line 1229 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      nilCheckHalt_chpl(&_str_literal_52_chpl, _ln_chpl, _fn_chpl);
#line 1229 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    }
#line 1231 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    if (((RootClass_chpl)(tmp_chpl)) == nil) /* ZLINE: 1231 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl */
#line 1231 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    {
#line 1231 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      ret_chpl = ((BaseArrOverRectangularDom_3_int64_t_one_chpl)(nil));
#line 1231 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      goto _end__COLON__chpl;
#line 1231 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    }
#line 1231 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    chpl_macro_tmp_95 = ((RootClass_chpl)(tmp_chpl))->chpl__cid;
#line 1231 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    ret_chpl = ( (((chpl__cid_BaseArrOverRectangularDom_3_int64_t_one_chpl <= chpl_macro_tmp_95) && (chpl_macro_tmp_95 <= chpl_subclass_max_id[chpl__cid_BaseArrOverRectangularDom_3_int64_t_one_chpl])))?(((BaseArrOverRectangularDom_3_int64_t_one_chpl)(tmp_chpl))):(((BaseArrOverRectangularDom_3_int64_t_one_chpl)(NULL))) );
#line 1231 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    _end__COLON__chpl:;
#line 1231 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    if (((RootClass_chpl)(ret_chpl)) != nil) /* ZLINE: 1231 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl */
#line 1231 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    {
#line 1232 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      *(coerce_tmp_chpl4 + INT64(0)) = *(rhsInds_chpl + INT64(0));
#line 1232 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      *(coerce_tmp_chpl4 + INT64(1)) = *(rhsInds_chpl + INT64(1));
#line 1232 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      *(coerce_tmp_chpl4 + INT64(2)) = *(rhsInds_chpl + INT64(2));
#line 1232 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      castIndices_chpl(&coerce_tmp_chpl4, lhs_chpl, &ret_tmp_chpl2);
#line 1232 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      *(call_tmp_chpl7 + INT64(0)) = *(ret_tmp_chpl2 + INT64(0));
#line 1232 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      *(call_tmp_chpl7 + INT64(1)) = *(ret_tmp_chpl2 + INT64(1));
#line 1232 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      *(call_tmp_chpl7 + INT64(2)) = *(ret_tmp_chpl2 + INT64(2));
#line 1232 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      chpl_check_nil(ret_chpl, _ln_chpl, _fn_chpl);
#line 1232 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      ((void(*)(BaseArrOverRectangularDom_3_int64_t_one_chpl,
#line 1232 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
         _tuple_3_star_range_int64_t_both_one_chpl *,
#line 1232 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
         int64_t,
#line 1232 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
         int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(ret_chpl))->chpl__cid) + INT64(5))])(ret_chpl, &call_tmp_chpl7, _ln_chpl, _fn_chpl);
#line 1232 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    } else /* ZLINE: 1231 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl */
#line 1232 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    {
#line 1235 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      halt_chpl8(&_str_literal_1180_chpl, &_str_literal_4260_chpl, &_str_literal_1181_chpl, &_str_literal_4261_chpl, _ln_chpl, _fn_chpl);
#line 1235 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    }
#line 1229 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    if (((RootClass_chpl)(tmp_chpl)) == nil) /* ZLINE: 1229 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl */
#line 1229 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    {
#line 1229 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      nilCheckHalt_chpl(&_str_literal_52_chpl, _ln_chpl, _fn_chpl);
#line 1229 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    }
#line 1229 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    call_tmp_chpl8 = tmp_chpl;
#line 1229 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    chpl_check_nil(call_tmp_chpl8, _ln_chpl, _fn_chpl);
#line 1229 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    coerce_tmp_chpl5 = (call_tmp_chpl8)->next;
#line 1229 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    tmp_chpl = coerce_tmp_chpl5;
#line 1229 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    tmp_chpl2 = (((RootClass_chpl)(coerce_tmp_chpl5)) != nil);
#line 1229 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  }
#line 1241 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  *(coerce_tmp_chpl6 + INT64(0)) = *(rhsInds_chpl + INT64(0));
#line 1241 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  *(coerce_tmp_chpl6 + INT64(1)) = *(rhsInds_chpl + INT64(1));
#line 1241 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  *(coerce_tmp_chpl6 + INT64(2)) = *(rhsInds_chpl + INT64(2));
#line 1241 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(lhs_chpl, _ln_chpl, _fn_chpl);
#line 1241 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  dsiSetIndices_chpl7(lhs_chpl, &coerce_tmp_chpl6, _ln_chpl, _fn_chpl);
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  this_chpl32 = ((BaseDom_chpl)(lhs_chpl));
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(this_chpl32, _ln_chpl, _fn_chpl);
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  coerce_tmp_chpl7 = (this_chpl32)->_arrs_head;
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  tmp_chpl3 = coerce_tmp_chpl7;
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  tmp_chpl4 = (((RootClass_chpl)(coerce_tmp_chpl7)) != nil);
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  while (tmp_chpl4) {
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    if (((RootClass_chpl)(tmp_chpl3)) == nil) /* ZLINE: 1243 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl */
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    {
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      nilCheckHalt_chpl(&_str_literal_52_chpl, _ln_chpl, _fn_chpl);
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    }
#line 1244 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    if (((RootClass_chpl)(tmp_chpl3)) == nil) /* ZLINE: 1244 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl */
#line 1244 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    {
#line 1244 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      ret_chpl2 = ((BaseArrOverRectangularDom_3_int64_t_one_chpl)(nil));
#line 1244 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      goto _end__COLON__chpl2;
#line 1244 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    }
#line 1244 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    chpl_macro_tmp_96 = ((RootClass_chpl)(tmp_chpl3))->chpl__cid;
#line 1244 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    ret_chpl2 = ( (((chpl__cid_BaseArrOverRectangularDom_3_int64_t_one_chpl <= chpl_macro_tmp_96) && (chpl_macro_tmp_96 <= chpl_subclass_max_id[chpl__cid_BaseArrOverRectangularDom_3_int64_t_one_chpl])))?(((BaseArrOverRectangularDom_3_int64_t_one_chpl)(tmp_chpl3))):(((BaseArrOverRectangularDom_3_int64_t_one_chpl)(NULL))) );
#line 1244 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    _end__COLON__chpl2:;
#line 1245 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    if (((RootClass_chpl)(ret_chpl2)) == nil) /* ZLINE: 1245 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl */
#line 1245 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    {
#line 1245 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      nilCheckHalt_chpl(&_str_literal_52_chpl, _ln_chpl, _fn_chpl);
#line 1245 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    }
#line 1246 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    chpl_check_nil(ret_chpl2, _ln_chpl, _fn_chpl);
#line 1246 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    ((void(*)(BaseArrOverRectangularDom_3_int64_t_one_chpl))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(ret_chpl2))->chpl__cid) + INT64(4))])(ret_chpl2);
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    if (((RootClass_chpl)(tmp_chpl3)) == nil) /* ZLINE: 1243 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl */
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    {
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      nilCheckHalt_chpl(&_str_literal_52_chpl, _ln_chpl, _fn_chpl);
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    }
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    call_tmp_chpl9 = tmp_chpl3;
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    chpl_check_nil(call_tmp_chpl9, _ln_chpl, _fn_chpl);
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    coerce_tmp_chpl8 = (call_tmp_chpl9)->next;
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    tmp_chpl3 = coerce_tmp_chpl8;
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    tmp_chpl4 = (((RootClass_chpl)(coerce_tmp_chpl8)) != nil);
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  }
#line 1223 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  return;
#line 1223 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
}

#line 1223 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
/* ChapelDistribution.chpl:1223 */
#line 1223 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
static void chpl_assignDomainWithGetSetIndices(DefaultRectangularDom_2_int64_t_one_chpl lhs_chpl,
#line 1223 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                                               _domain_ArrayViewRankChangeDom_2_int64_t_one__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_3_star_chpl_bool__tuple_3_star_int64_t_chpl * rhs_chpl,
#line 1223 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                                               int64_t _ln_chpl,
#line 1223 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
                                               int32_t _fn_chpl) {
#line 1226 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  _tuple_2_star_range_int64_t_both_one_chpl call_tmp_chpl6;
#line 1226 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  _tuple_2_star_range_int64_t_both_one_chpl ret_tmp_chpl;
#line 1226 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  _tuple_2_star_range_int64_t_both_one_chpl rhsInds_chpl;
#line 1229 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseDom_chpl this_chpl31 = NULL;
#line 1229 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseArr_chpl tmp_chpl = NULL;
#line 1229 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseArr_chpl coerce_tmp_chpl3 = NULL;
#line 1229 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_bool tmp_chpl2;
#line 1231 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseArrOverRectangularDom_2_int64_t_one_chpl _formal_type_tmp__chpl = NULL;
#line 1231 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseArrOverRectangularDom_2_int64_t_one_chpl ret_chpl = NULL;
#line 1232 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  _tuple_2_star_range_int64_t_both_one_chpl call_tmp_chpl7;
#line 1232 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  _tuple_2_star_range_int64_t_both_one_chpl coerce_tmp_chpl4;
#line 1232 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  _tuple_2_star_range_int64_t_both_one_chpl ret_tmp_chpl2;
#line 1229 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseArr_chpl call_tmp_chpl8 = NULL;
#line 1229 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseArr_chpl coerce_tmp_chpl5 = NULL;
#line 1241 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  _tuple_2_star_range_int64_t_both_one_chpl coerce_tmp_chpl6;
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseDom_chpl this_chpl32 = NULL;
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseArr_chpl tmp_chpl3 = NULL;
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseArr_chpl coerce_tmp_chpl7 = NULL;
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_bool tmp_chpl4;
#line 1244 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseArrOverRectangularDom_2_int64_t_one_chpl _formal_type_tmp__chpl2 = NULL;
#line 1244 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseArrOverRectangularDom_2_int64_t_one_chpl ret_chpl2 = NULL;
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseArr_chpl call_tmp_chpl9 = NULL;
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  BaseArr_chpl coerce_tmp_chpl8 = NULL;
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  int32_t chpl_macro_tmp_97;
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  int32_t chpl_macro_tmp_98;
#line 1226 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  getIndices_chpl2(rhs_chpl, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 1226 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  *(call_tmp_chpl6 + INT64(0)) = *(ret_tmp_chpl + INT64(0));
#line 1226 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  *(call_tmp_chpl6 + INT64(1)) = *(ret_tmp_chpl + INT64(1));
#line 1226 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  *(rhsInds_chpl + INT64(0)) = *(call_tmp_chpl6 + INT64(0));
#line 1226 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  *(rhsInds_chpl + INT64(1)) = *(call_tmp_chpl6 + INT64(1));
#line 1229 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  this_chpl31 = ((BaseDom_chpl)(lhs_chpl));
#line 1229 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 1229 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  coerce_tmp_chpl3 = (this_chpl31)->_arrs_head;
#line 1229 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  tmp_chpl = coerce_tmp_chpl3;
#line 1229 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  tmp_chpl2 = (((RootClass_chpl)(coerce_tmp_chpl3)) != nil);
#line 1229 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  while (tmp_chpl2) {
#line 1229 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    if (((RootClass_chpl)(tmp_chpl)) == nil) /* ZLINE: 1229 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl */
#line 1229 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    {
#line 1229 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      nilCheckHalt_chpl(&_str_literal_52_chpl, _ln_chpl, _fn_chpl);
#line 1229 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    }
#line 1231 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    if (((RootClass_chpl)(tmp_chpl)) == nil) /* ZLINE: 1231 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl */
#line 1231 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    {
#line 1231 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      ret_chpl = ((BaseArrOverRectangularDom_2_int64_t_one_chpl)(nil));
#line 1231 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      goto _end__COLON__chpl;
#line 1231 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    }
#line 1231 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    chpl_macro_tmp_97 = ((RootClass_chpl)(tmp_chpl))->chpl__cid;
#line 1231 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    ret_chpl = ( (((chpl__cid_BaseArrOverRectangularDom_2_int64_t_one_chpl <= chpl_macro_tmp_97) && (chpl_macro_tmp_97 <= chpl_subclass_max_id[chpl__cid_BaseArrOverRectangularDom_2_int64_t_one_chpl])))?(((BaseArrOverRectangularDom_2_int64_t_one_chpl)(tmp_chpl))):(((BaseArrOverRectangularDom_2_int64_t_one_chpl)(NULL))) );
#line 1231 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    _end__COLON__chpl:;
#line 1231 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    if (((RootClass_chpl)(ret_chpl)) != nil) /* ZLINE: 1231 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl */
#line 1231 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    {
#line 1232 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      *(coerce_tmp_chpl4 + INT64(0)) = *(rhsInds_chpl + INT64(0));
#line 1232 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      *(coerce_tmp_chpl4 + INT64(1)) = *(rhsInds_chpl + INT64(1));
#line 1232 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      castIndices_chpl2(&coerce_tmp_chpl4, lhs_chpl, &ret_tmp_chpl2);
#line 1232 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      *(call_tmp_chpl7 + INT64(0)) = *(ret_tmp_chpl2 + INT64(0));
#line 1232 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      *(call_tmp_chpl7 + INT64(1)) = *(ret_tmp_chpl2 + INT64(1));
#line 1232 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      chpl_check_nil(ret_chpl, _ln_chpl, _fn_chpl);
#line 1232 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      ((void(*)(BaseArrOverRectangularDom_2_int64_t_one_chpl,
#line 1232 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
         _tuple_2_star_range_int64_t_both_one_chpl *,
#line 1232 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
         int64_t,
#line 1232 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
         int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(ret_chpl))->chpl__cid) + INT64(5))])(ret_chpl, &call_tmp_chpl7, _ln_chpl, _fn_chpl);
#line 1232 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    } else /* ZLINE: 1231 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl */
#line 1232 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    {
#line 1235 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      halt_chpl8(&_str_literal_1180_chpl, &_str_literal_4171_chpl, &_str_literal_1181_chpl, &_str_literal_4172_chpl, _ln_chpl, _fn_chpl);
#line 1235 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    }
#line 1229 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    if (((RootClass_chpl)(tmp_chpl)) == nil) /* ZLINE: 1229 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl */
#line 1229 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    {
#line 1229 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      nilCheckHalt_chpl(&_str_literal_52_chpl, _ln_chpl, _fn_chpl);
#line 1229 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    }
#line 1229 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    call_tmp_chpl8 = tmp_chpl;
#line 1229 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    chpl_check_nil(call_tmp_chpl8, _ln_chpl, _fn_chpl);
#line 1229 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    coerce_tmp_chpl5 = (call_tmp_chpl8)->next;
#line 1229 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    tmp_chpl = coerce_tmp_chpl5;
#line 1229 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    tmp_chpl2 = (((RootClass_chpl)(coerce_tmp_chpl5)) != nil);
#line 1229 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  }
#line 1241 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  *(coerce_tmp_chpl6 + INT64(0)) = *(rhsInds_chpl + INT64(0));
#line 1241 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  *(coerce_tmp_chpl6 + INT64(1)) = *(rhsInds_chpl + INT64(1));
#line 1241 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(lhs_chpl, _ln_chpl, _fn_chpl);
#line 1241 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  dsiSetIndices_chpl5(lhs_chpl, &coerce_tmp_chpl6, _ln_chpl, _fn_chpl);
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  this_chpl32 = ((BaseDom_chpl)(lhs_chpl));
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_check_nil(this_chpl32, _ln_chpl, _fn_chpl);
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  coerce_tmp_chpl7 = (this_chpl32)->_arrs_head;
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  tmp_chpl3 = coerce_tmp_chpl7;
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  tmp_chpl4 = (((RootClass_chpl)(coerce_tmp_chpl7)) != nil);
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  while (tmp_chpl4) {
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    if (((RootClass_chpl)(tmp_chpl3)) == nil) /* ZLINE: 1243 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl */
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    {
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      nilCheckHalt_chpl(&_str_literal_52_chpl, _ln_chpl, _fn_chpl);
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    }
#line 1244 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    if (((RootClass_chpl)(tmp_chpl3)) == nil) /* ZLINE: 1244 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl */
#line 1244 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    {
#line 1244 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      ret_chpl2 = ((BaseArrOverRectangularDom_2_int64_t_one_chpl)(nil));
#line 1244 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      goto _end__COLON__chpl2;
#line 1244 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    }
#line 1244 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    chpl_macro_tmp_98 = ((RootClass_chpl)(tmp_chpl3))->chpl__cid;
#line 1244 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    ret_chpl2 = ( (((chpl__cid_BaseArrOverRectangularDom_2_int64_t_one_chpl <= chpl_macro_tmp_98) && (chpl_macro_tmp_98 <= chpl_subclass_max_id[chpl__cid_BaseArrOverRectangularDom_2_int64_t_one_chpl])))?(((BaseArrOverRectangularDom_2_int64_t_one_chpl)(tmp_chpl3))):(((BaseArrOverRectangularDom_2_int64_t_one_chpl)(NULL))) );
#line 1244 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    _end__COLON__chpl2:;
#line 1245 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    if (((RootClass_chpl)(ret_chpl2)) == nil) /* ZLINE: 1245 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl */
#line 1245 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    {
#line 1245 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      nilCheckHalt_chpl(&_str_literal_52_chpl, _ln_chpl, _fn_chpl);
#line 1245 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    }
#line 1246 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    chpl_check_nil(ret_chpl2, _ln_chpl, _fn_chpl);
#line 1246 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    ((void(*)(BaseArrOverRectangularDom_2_int64_t_one_chpl))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(ret_chpl2))->chpl__cid) + INT64(4))])(ret_chpl2);
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    if (((RootClass_chpl)(tmp_chpl3)) == nil) /* ZLINE: 1243 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl */
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    {
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
      nilCheckHalt_chpl(&_str_literal_52_chpl, _ln_chpl, _fn_chpl);
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    }
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    call_tmp_chpl9 = tmp_chpl3;
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    chpl_check_nil(call_tmp_chpl9, _ln_chpl, _fn_chpl);
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    coerce_tmp_chpl8 = (call_tmp_chpl9)->next;
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    tmp_chpl3 = coerce_tmp_chpl8;
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
    tmp_chpl4 = (((RootClass_chpl)(coerce_tmp_chpl8)) != nil);
#line 1243 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  }
#line 1223 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  return;
#line 1223 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
}

