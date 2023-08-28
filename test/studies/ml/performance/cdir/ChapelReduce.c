#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
/* ChapelReduce.chpl:23 */
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
static void chpl__init_ChapelReduce(int64_t _ln_chpl,
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
                                    int32_t _fn_chpl) {
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _ref_int32_t_chpl refIndentLevel_chpl = NULL;
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  if (chpl__init_ChapelReduce_p) /* ZLINE: 23 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl */
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  {
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    goto _exit_chpl__init_ChapelReduce_chpl;
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  }
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  printModuleInit_chpl("%*s\n", "ChapelReduce", INT64(12), _ln_chpl, _fn_chpl);
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  refIndentLevel_chpl = &moduleInitLevel_chpl;
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  *(refIndentLevel_chpl) += INT64(1);
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl__init_ChapelReduce_p = UINT8(true);
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  {
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    chpl__init_ChapelLocks(_ln_chpl, _fn_chpl);
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    chpl__init_Reflection(_ln_chpl, _fn_chpl);
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  }
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  *(refIndentLevel_chpl) -= INT64(1);
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _exit_chpl__init_ChapelReduce_chpl:;
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  return;
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
}

#line 62 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
/* ChapelReduce.chpl:62 */
#line 62 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
static void chpl__reduceCombine6(SumReduceScanOp__real64_chpl globalOp_chpl,
#line 62 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
                                 SumReduceScanOp__real64_chpl localOp_chpl,
#line 62 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
                                 int64_t _ln_chpl,
#line 62 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
                                 int32_t _fn_chpl) {
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  ReduceScanOp_chpl coerce_tmp_chpl3 = NULL;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _ref_chpl_LocalSpinlock_chpl call_tmp_chpl6 = NULL;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_bool tmp_chpl;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _ref_AtomicBool_chpl call_tmp_chpl7 = NULL;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _ref_atomic_bool_chpl call_tmp_chpl8 = NULL;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  memory_order ret_chpl;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_bool call_tmp_chpl9;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_bool tmp_chpl2;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _ref_AtomicBool_chpl call_tmp_chpl10 = NULL;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _ref_atomic_bool_chpl call_tmp_chpl11 = NULL;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  memory_order ret_chpl2;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_bool call_tmp_chpl12;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _ref_AtomicBool_chpl call_tmp_chpl13 = NULL;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _ref_atomic_bool_chpl call_tmp_chpl14 = NULL;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  memory_order ret_chpl3;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_bool call_tmp_chpl15;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_bool tmp_chpl3;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _ref_AtomicBool_chpl call_tmp_chpl16 = NULL;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _ref_atomic_bool_chpl call_tmp_chpl17 = NULL;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  memory_order ret_chpl4;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_bool call_tmp_chpl18;
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  SumReduceScanOp__real64_chpl coerce_tmp_chpl4 = NULL;
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _ref__real64_chpl call_tmp_chpl19 = NULL;
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  SumReduceScanOp__real64_chpl coerce_tmp_chpl5 = NULL;
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  ReduceScanOp_chpl coerce_tmp_chpl6 = NULL;
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _ref_chpl_LocalSpinlock_chpl call_tmp_chpl20 = NULL;
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _ref_AtomicBool_chpl call_tmp_chpl21 = NULL;
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _ref_atomic_bool_chpl call_tmp_chpl22 = NULL;
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  memory_order ret_chpl5;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  coerce_tmp_chpl3 = ((ReduceScanOp_chpl)(globalOp_chpl));
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_check_nil(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  call_tmp_chpl6 = &((coerce_tmp_chpl3)->l);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  call_tmp_chpl7 = &((call_tmp_chpl6)->l);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_rmem_consist_maybe_release2(_ln_chpl, _fn_chpl);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  call_tmp_chpl8 = &((call_tmp_chpl7)->_v);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  ret_chpl = memory_order_seq_cst;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  call_tmp_chpl9 = atomic_load_explicit_bool(call_tmp_chpl8, ret_chpl);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  if (call_tmp_chpl9) /* ZLINE: 64 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl */
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  {
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    tmp_chpl2 = UINT8(true);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  } else /* ZLINE: 64 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl */
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  {
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    call_tmp_chpl10 = &((call_tmp_chpl6)->l);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    chpl_rmem_consist_maybe_release5(_ln_chpl, _fn_chpl);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    chpl_rmem_consist_maybe_release5(_ln_chpl, _fn_chpl);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    call_tmp_chpl11 = &((call_tmp_chpl10)->_v);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    ret_chpl2 = memory_order_acquire;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    call_tmp_chpl12 = atomic_exchange_explicit_bool(call_tmp_chpl11, UINT8(true), ret_chpl2);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    chpl_rmem_consist_maybe_acquire5(_ln_chpl, _fn_chpl);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    tmp_chpl2 = call_tmp_chpl12;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  }
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  tmp_chpl = tmp_chpl2;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  while (tmp_chpl) {
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    chpl_task_yield();
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    call_tmp_chpl13 = &((call_tmp_chpl6)->l);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    chpl_rmem_consist_maybe_release2(_ln_chpl, _fn_chpl);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    call_tmp_chpl14 = &((call_tmp_chpl13)->_v);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    ret_chpl3 = memory_order_seq_cst;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    call_tmp_chpl15 = atomic_load_explicit_bool(call_tmp_chpl14, ret_chpl3);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    if (call_tmp_chpl15) /* ZLINE: 64 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl */
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    {
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
      tmp_chpl3 = UINT8(true);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    } else /* ZLINE: 64 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl */
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    {
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
      call_tmp_chpl16 = &((call_tmp_chpl6)->l);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
      chpl_rmem_consist_maybe_release5(_ln_chpl, _fn_chpl);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
      chpl_rmem_consist_maybe_release5(_ln_chpl, _fn_chpl);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
      call_tmp_chpl17 = &((call_tmp_chpl16)->_v);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
      ret_chpl4 = memory_order_acquire;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
      call_tmp_chpl18 = atomic_exchange_explicit_bool(call_tmp_chpl17, UINT8(true), ret_chpl4);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
      chpl_rmem_consist_maybe_acquire5(_ln_chpl, _fn_chpl);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
      tmp_chpl3 = call_tmp_chpl18;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    }
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    tmp_chpl = tmp_chpl3;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  }
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  coerce_tmp_chpl4 = globalOp_chpl;
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_check_nil(coerce_tmp_chpl4, _ln_chpl, _fn_chpl);
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  call_tmp_chpl19 = &((coerce_tmp_chpl4)->value);
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  coerce_tmp_chpl5 = localOp_chpl;
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_check_nil(coerce_tmp_chpl5, _ln_chpl, _fn_chpl);
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  *(call_tmp_chpl19) += (coerce_tmp_chpl5)->value;
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  coerce_tmp_chpl6 = ((ReduceScanOp_chpl)(globalOp_chpl));
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_check_nil(coerce_tmp_chpl6, _ln_chpl, _fn_chpl);
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  call_tmp_chpl20 = &((coerce_tmp_chpl6)->l);
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  call_tmp_chpl21 = &((call_tmp_chpl20)->l);
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_rmem_consist_maybe_release4(_ln_chpl, _fn_chpl);
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_rmem_consist_maybe_release4(_ln_chpl, _fn_chpl);
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  call_tmp_chpl22 = &((call_tmp_chpl21)->_v);
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  ret_chpl5 = memory_order_release;
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  atomic_store_explicit_bool(call_tmp_chpl22, UINT8(false), ret_chpl5);
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_rmem_consist_maybe_acquire4(_ln_chpl, _fn_chpl);
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_rmem_consist_maybe_acquire4(_ln_chpl, _fn_chpl);
#line 62 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  return;
#line 62 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
}

#line 62 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
/* ChapelReduce.chpl:62 */
#line 62 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
static void chpl__reduceCombine7(MaxReduceScanOp__real64_chpl globalOp_chpl,
#line 62 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
                                 MaxReduceScanOp__real64_chpl localOp_chpl,
#line 62 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
                                 int64_t _ln_chpl,
#line 62 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
                                 int32_t _fn_chpl) {
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  ReduceScanOp_chpl coerce_tmp_chpl3 = NULL;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _ref_chpl_LocalSpinlock_chpl call_tmp_chpl6 = NULL;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_bool tmp_chpl;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _ref_AtomicBool_chpl call_tmp_chpl7 = NULL;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _ref_atomic_bool_chpl call_tmp_chpl8 = NULL;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  memory_order ret_chpl;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_bool call_tmp_chpl9;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_bool tmp_chpl2;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _ref_AtomicBool_chpl call_tmp_chpl10 = NULL;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _ref_atomic_bool_chpl call_tmp_chpl11 = NULL;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  memory_order ret_chpl2;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_bool call_tmp_chpl12;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _ref_AtomicBool_chpl call_tmp_chpl13 = NULL;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _ref_atomic_bool_chpl call_tmp_chpl14 = NULL;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  memory_order ret_chpl3;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_bool call_tmp_chpl15;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_bool tmp_chpl3;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _ref_AtomicBool_chpl call_tmp_chpl16 = NULL;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _ref_atomic_bool_chpl call_tmp_chpl17 = NULL;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  memory_order ret_chpl4;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_bool call_tmp_chpl18;
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  ReduceScanOp_chpl coerce_tmp_chpl4 = NULL;
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _ref_chpl_LocalSpinlock_chpl call_tmp_chpl19 = NULL;
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _ref_AtomicBool_chpl call_tmp_chpl20 = NULL;
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _ref_atomic_bool_chpl call_tmp_chpl21 = NULL;
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  memory_order ret_chpl5;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  coerce_tmp_chpl3 = ((ReduceScanOp_chpl)(globalOp_chpl));
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_check_nil(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  call_tmp_chpl6 = &((coerce_tmp_chpl3)->l);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  call_tmp_chpl7 = &((call_tmp_chpl6)->l);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_rmem_consist_maybe_release2(_ln_chpl, _fn_chpl);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  call_tmp_chpl8 = &((call_tmp_chpl7)->_v);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  ret_chpl = memory_order_seq_cst;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  call_tmp_chpl9 = atomic_load_explicit_bool(call_tmp_chpl8, ret_chpl);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  if (call_tmp_chpl9) /* ZLINE: 64 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl */
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  {
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    tmp_chpl2 = UINT8(true);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  } else /* ZLINE: 64 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl */
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  {
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    call_tmp_chpl10 = &((call_tmp_chpl6)->l);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    chpl_rmem_consist_maybe_release5(_ln_chpl, _fn_chpl);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    chpl_rmem_consist_maybe_release5(_ln_chpl, _fn_chpl);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    call_tmp_chpl11 = &((call_tmp_chpl10)->_v);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    ret_chpl2 = memory_order_acquire;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    call_tmp_chpl12 = atomic_exchange_explicit_bool(call_tmp_chpl11, UINT8(true), ret_chpl2);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    chpl_rmem_consist_maybe_acquire5(_ln_chpl, _fn_chpl);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    tmp_chpl2 = call_tmp_chpl12;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  }
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  tmp_chpl = tmp_chpl2;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  while (tmp_chpl) {
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    chpl_task_yield();
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    call_tmp_chpl13 = &((call_tmp_chpl6)->l);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    chpl_rmem_consist_maybe_release2(_ln_chpl, _fn_chpl);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    call_tmp_chpl14 = &((call_tmp_chpl13)->_v);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    ret_chpl3 = memory_order_seq_cst;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    call_tmp_chpl15 = atomic_load_explicit_bool(call_tmp_chpl14, ret_chpl3);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    if (call_tmp_chpl15) /* ZLINE: 64 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl */
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    {
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
      tmp_chpl3 = UINT8(true);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    } else /* ZLINE: 64 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl */
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    {
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
      call_tmp_chpl16 = &((call_tmp_chpl6)->l);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
      chpl_rmem_consist_maybe_release5(_ln_chpl, _fn_chpl);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
      chpl_rmem_consist_maybe_release5(_ln_chpl, _fn_chpl);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
      call_tmp_chpl17 = &((call_tmp_chpl16)->_v);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
      ret_chpl4 = memory_order_acquire;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
      call_tmp_chpl18 = atomic_exchange_explicit_bool(call_tmp_chpl17, UINT8(true), ret_chpl4);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
      chpl_rmem_consist_maybe_acquire5(_ln_chpl, _fn_chpl);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
      tmp_chpl3 = call_tmp_chpl18;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    }
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    tmp_chpl = tmp_chpl3;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  }
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_check_nil(globalOp_chpl, _ln_chpl, _fn_chpl);
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  combine_chpl2(globalOp_chpl, localOp_chpl, _ln_chpl, _fn_chpl);
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  coerce_tmp_chpl4 = ((ReduceScanOp_chpl)(globalOp_chpl));
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_check_nil(coerce_tmp_chpl4, _ln_chpl, _fn_chpl);
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  call_tmp_chpl19 = &((coerce_tmp_chpl4)->l);
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  call_tmp_chpl20 = &((call_tmp_chpl19)->l);
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_rmem_consist_maybe_release4(_ln_chpl, _fn_chpl);
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_rmem_consist_maybe_release4(_ln_chpl, _fn_chpl);
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  call_tmp_chpl21 = &((call_tmp_chpl20)->_v);
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  ret_chpl5 = memory_order_release;
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  atomic_store_explicit_bool(call_tmp_chpl21, UINT8(false), ret_chpl5);
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_rmem_consist_maybe_acquire4(_ln_chpl, _fn_chpl);
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_rmem_consist_maybe_acquire4(_ln_chpl, _fn_chpl);
#line 62 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  return;
#line 62 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
}

#line 62 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
/* ChapelReduce.chpl:62 */
#line 62 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
static void chpl__reduceCombine4(ProductReduceScanOp_int64_t_chpl globalOp_chpl,
#line 62 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
                                 ProductReduceScanOp_int64_t_chpl localOp_chpl,
#line 62 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
                                 int64_t _ln_chpl,
#line 62 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
                                 int32_t _fn_chpl) {
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  ReduceScanOp_chpl coerce_tmp_chpl3 = NULL;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _ref_chpl_LocalSpinlock_chpl call_tmp_chpl6 = NULL;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_bool tmp_chpl;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _ref_AtomicBool_chpl call_tmp_chpl7 = NULL;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _ref_atomic_bool_chpl call_tmp_chpl8 = NULL;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  memory_order ret_chpl;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_bool call_tmp_chpl9;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_bool tmp_chpl2;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _ref_AtomicBool_chpl call_tmp_chpl10 = NULL;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _ref_atomic_bool_chpl call_tmp_chpl11 = NULL;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  memory_order ret_chpl2;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_bool call_tmp_chpl12;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _ref_AtomicBool_chpl call_tmp_chpl13 = NULL;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _ref_atomic_bool_chpl call_tmp_chpl14 = NULL;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  memory_order ret_chpl3;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_bool call_tmp_chpl15;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_bool tmp_chpl3;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _ref_AtomicBool_chpl call_tmp_chpl16 = NULL;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _ref_atomic_bool_chpl call_tmp_chpl17 = NULL;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  memory_order ret_chpl4;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_bool call_tmp_chpl18;
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  ReduceScanOp_chpl coerce_tmp_chpl4 = NULL;
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _ref_chpl_LocalSpinlock_chpl call_tmp_chpl19 = NULL;
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _ref_AtomicBool_chpl call_tmp_chpl20 = NULL;
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _ref_atomic_bool_chpl call_tmp_chpl21 = NULL;
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  memory_order ret_chpl5;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  coerce_tmp_chpl3 = ((ReduceScanOp_chpl)(globalOp_chpl));
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_check_nil(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  call_tmp_chpl6 = &((coerce_tmp_chpl3)->l);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  call_tmp_chpl7 = &((call_tmp_chpl6)->l);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_rmem_consist_maybe_release2(_ln_chpl, _fn_chpl);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  call_tmp_chpl8 = &((call_tmp_chpl7)->_v);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  ret_chpl = memory_order_seq_cst;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  call_tmp_chpl9 = atomic_load_explicit_bool(call_tmp_chpl8, ret_chpl);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  if (call_tmp_chpl9) /* ZLINE: 64 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl */
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  {
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    tmp_chpl2 = UINT8(true);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  } else /* ZLINE: 64 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl */
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  {
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    call_tmp_chpl10 = &((call_tmp_chpl6)->l);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    chpl_rmem_consist_maybe_release5(_ln_chpl, _fn_chpl);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    chpl_rmem_consist_maybe_release5(_ln_chpl, _fn_chpl);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    call_tmp_chpl11 = &((call_tmp_chpl10)->_v);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    ret_chpl2 = memory_order_acquire;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    call_tmp_chpl12 = atomic_exchange_explicit_bool(call_tmp_chpl11, UINT8(true), ret_chpl2);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    chpl_rmem_consist_maybe_acquire5(_ln_chpl, _fn_chpl);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    tmp_chpl2 = call_tmp_chpl12;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  }
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  tmp_chpl = tmp_chpl2;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  while (tmp_chpl) {
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    chpl_task_yield();
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    call_tmp_chpl13 = &((call_tmp_chpl6)->l);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    chpl_rmem_consist_maybe_release2(_ln_chpl, _fn_chpl);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    call_tmp_chpl14 = &((call_tmp_chpl13)->_v);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    ret_chpl3 = memory_order_seq_cst;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    call_tmp_chpl15 = atomic_load_explicit_bool(call_tmp_chpl14, ret_chpl3);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    if (call_tmp_chpl15) /* ZLINE: 64 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl */
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    {
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
      tmp_chpl3 = UINT8(true);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    } else /* ZLINE: 64 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl */
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    {
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
      call_tmp_chpl16 = &((call_tmp_chpl6)->l);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
      chpl_rmem_consist_maybe_release5(_ln_chpl, _fn_chpl);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
      chpl_rmem_consist_maybe_release5(_ln_chpl, _fn_chpl);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
      call_tmp_chpl17 = &((call_tmp_chpl16)->_v);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
      ret_chpl4 = memory_order_acquire;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
      call_tmp_chpl18 = atomic_exchange_explicit_bool(call_tmp_chpl17, UINT8(true), ret_chpl4);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
      chpl_rmem_consist_maybe_acquire5(_ln_chpl, _fn_chpl);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
      tmp_chpl3 = call_tmp_chpl18;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    }
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    tmp_chpl = tmp_chpl3;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  }
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_check_nil(globalOp_chpl, _ln_chpl, _fn_chpl);
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  combine_chpl(globalOp_chpl, localOp_chpl, _ln_chpl, _fn_chpl);
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  coerce_tmp_chpl4 = ((ReduceScanOp_chpl)(globalOp_chpl));
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_check_nil(coerce_tmp_chpl4, _ln_chpl, _fn_chpl);
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  call_tmp_chpl19 = &((coerce_tmp_chpl4)->l);
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  call_tmp_chpl20 = &((call_tmp_chpl19)->l);
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_rmem_consist_maybe_release4(_ln_chpl, _fn_chpl);
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_rmem_consist_maybe_release4(_ln_chpl, _fn_chpl);
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  call_tmp_chpl21 = &((call_tmp_chpl20)->_v);
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  ret_chpl5 = memory_order_release;
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  atomic_store_explicit_bool(call_tmp_chpl21, UINT8(false), ret_chpl5);
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_rmem_consist_maybe_acquire4(_ln_chpl, _fn_chpl);
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_rmem_consist_maybe_acquire4(_ln_chpl, _fn_chpl);
#line 62 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  return;
#line 62 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
}

#line 62 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
/* ChapelReduce.chpl:62 */
#line 62 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
static void chpl__reduceCombine5(SumReduceScanOp_int64_t_chpl globalOp_chpl,
#line 62 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
                                 SumReduceScanOp_int64_t_chpl localOp_chpl,
#line 62 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
                                 int64_t _ln_chpl,
#line 62 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
                                 int32_t _fn_chpl) {
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  ReduceScanOp_chpl coerce_tmp_chpl3 = NULL;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _ref_chpl_LocalSpinlock_chpl call_tmp_chpl6 = NULL;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_bool tmp_chpl;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _ref_AtomicBool_chpl call_tmp_chpl7 = NULL;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _ref_atomic_bool_chpl call_tmp_chpl8 = NULL;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  memory_order ret_chpl;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_bool call_tmp_chpl9;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_bool tmp_chpl2;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _ref_AtomicBool_chpl call_tmp_chpl10 = NULL;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _ref_atomic_bool_chpl call_tmp_chpl11 = NULL;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  memory_order ret_chpl2;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_bool call_tmp_chpl12;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _ref_AtomicBool_chpl call_tmp_chpl13 = NULL;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _ref_atomic_bool_chpl call_tmp_chpl14 = NULL;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  memory_order ret_chpl3;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_bool call_tmp_chpl15;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_bool tmp_chpl3;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _ref_AtomicBool_chpl call_tmp_chpl16 = NULL;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _ref_atomic_bool_chpl call_tmp_chpl17 = NULL;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  memory_order ret_chpl4;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_bool call_tmp_chpl18;
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  SumReduceScanOp_int64_t_chpl coerce_tmp_chpl4 = NULL;
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _ref_int64_t_chpl call_tmp_chpl19 = NULL;
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  SumReduceScanOp_int64_t_chpl coerce_tmp_chpl5 = NULL;
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  ReduceScanOp_chpl coerce_tmp_chpl6 = NULL;
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _ref_chpl_LocalSpinlock_chpl call_tmp_chpl20 = NULL;
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _ref_AtomicBool_chpl call_tmp_chpl21 = NULL;
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _ref_atomic_bool_chpl call_tmp_chpl22 = NULL;
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  memory_order ret_chpl5;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  coerce_tmp_chpl3 = ((ReduceScanOp_chpl)(globalOp_chpl));
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_check_nil(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  call_tmp_chpl6 = &((coerce_tmp_chpl3)->l);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  call_tmp_chpl7 = &((call_tmp_chpl6)->l);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_rmem_consist_maybe_release2(_ln_chpl, _fn_chpl);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  call_tmp_chpl8 = &((call_tmp_chpl7)->_v);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  ret_chpl = memory_order_seq_cst;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  call_tmp_chpl9 = atomic_load_explicit_bool(call_tmp_chpl8, ret_chpl);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  if (call_tmp_chpl9) /* ZLINE: 64 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl */
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  {
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    tmp_chpl2 = UINT8(true);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  } else /* ZLINE: 64 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl */
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  {
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    call_tmp_chpl10 = &((call_tmp_chpl6)->l);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    chpl_rmem_consist_maybe_release5(_ln_chpl, _fn_chpl);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    chpl_rmem_consist_maybe_release5(_ln_chpl, _fn_chpl);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    call_tmp_chpl11 = &((call_tmp_chpl10)->_v);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    ret_chpl2 = memory_order_acquire;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    call_tmp_chpl12 = atomic_exchange_explicit_bool(call_tmp_chpl11, UINT8(true), ret_chpl2);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    chpl_rmem_consist_maybe_acquire5(_ln_chpl, _fn_chpl);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    tmp_chpl2 = call_tmp_chpl12;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  }
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  tmp_chpl = tmp_chpl2;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  while (tmp_chpl) {
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    chpl_task_yield();
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    call_tmp_chpl13 = &((call_tmp_chpl6)->l);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    chpl_rmem_consist_maybe_release2(_ln_chpl, _fn_chpl);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    call_tmp_chpl14 = &((call_tmp_chpl13)->_v);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    ret_chpl3 = memory_order_seq_cst;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    call_tmp_chpl15 = atomic_load_explicit_bool(call_tmp_chpl14, ret_chpl3);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    if (call_tmp_chpl15) /* ZLINE: 64 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl */
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    {
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
      tmp_chpl3 = UINT8(true);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    } else /* ZLINE: 64 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl */
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    {
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
      call_tmp_chpl16 = &((call_tmp_chpl6)->l);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
      chpl_rmem_consist_maybe_release5(_ln_chpl, _fn_chpl);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
      chpl_rmem_consist_maybe_release5(_ln_chpl, _fn_chpl);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
      call_tmp_chpl17 = &((call_tmp_chpl16)->_v);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
      ret_chpl4 = memory_order_acquire;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
      call_tmp_chpl18 = atomic_exchange_explicit_bool(call_tmp_chpl17, UINT8(true), ret_chpl4);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
      chpl_rmem_consist_maybe_acquire5(_ln_chpl, _fn_chpl);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
      tmp_chpl3 = call_tmp_chpl18;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    }
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    tmp_chpl = tmp_chpl3;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  }
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  coerce_tmp_chpl4 = globalOp_chpl;
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_check_nil(coerce_tmp_chpl4, _ln_chpl, _fn_chpl);
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  call_tmp_chpl19 = &((coerce_tmp_chpl4)->value);
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  coerce_tmp_chpl5 = localOp_chpl;
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_check_nil(coerce_tmp_chpl5, _ln_chpl, _fn_chpl);
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  *(call_tmp_chpl19) += (coerce_tmp_chpl5)->value;
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  coerce_tmp_chpl6 = ((ReduceScanOp_chpl)(globalOp_chpl));
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_check_nil(coerce_tmp_chpl6, _ln_chpl, _fn_chpl);
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  call_tmp_chpl20 = &((coerce_tmp_chpl6)->l);
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  call_tmp_chpl21 = &((call_tmp_chpl20)->l);
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_rmem_consist_maybe_release4(_ln_chpl, _fn_chpl);
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_rmem_consist_maybe_release4(_ln_chpl, _fn_chpl);
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  call_tmp_chpl22 = &((call_tmp_chpl21)->_v);
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  ret_chpl5 = memory_order_release;
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  atomic_store_explicit_bool(call_tmp_chpl22, UINT8(false), ret_chpl5);
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_rmem_consist_maybe_acquire4(_ln_chpl, _fn_chpl);
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_rmem_consist_maybe_acquire4(_ln_chpl, _fn_chpl);
#line 62 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  return;
#line 62 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
}

#line 62 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
/* ChapelReduce.chpl:62 */
#line 62 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
static void chpl__reduceCombine2(SumReduceScanOp_Tensor_2__real64_chpl globalOp_chpl,
#line 62 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
                                 SumReduceScanOp_Tensor_2__real64_chpl localOp_chpl,
#line 62 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
                                 int64_t _ln_chpl,
#line 62 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
                                 int32_t _fn_chpl) {
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  ReduceScanOp_chpl coerce_tmp_chpl3 = NULL;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _ref_chpl_LocalSpinlock_chpl call_tmp_chpl6 = NULL;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_bool tmp_chpl;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _ref_AtomicBool_chpl call_tmp_chpl7 = NULL;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _ref_atomic_bool_chpl call_tmp_chpl8 = NULL;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  memory_order ret_chpl;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_bool call_tmp_chpl9;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_bool tmp_chpl2;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _ref_AtomicBool_chpl call_tmp_chpl10 = NULL;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _ref_atomic_bool_chpl call_tmp_chpl11 = NULL;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  memory_order ret_chpl2;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_bool call_tmp_chpl12;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _ref_AtomicBool_chpl call_tmp_chpl13 = NULL;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _ref_atomic_bool_chpl call_tmp_chpl14 = NULL;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  memory_order ret_chpl3;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_bool call_tmp_chpl15;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_bool tmp_chpl3;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _ref_AtomicBool_chpl call_tmp_chpl16 = NULL;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _ref_atomic_bool_chpl call_tmp_chpl17 = NULL;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  memory_order ret_chpl4;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_bool call_tmp_chpl18;
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  SumReduceScanOp_Tensor_2__real64_chpl coerce_tmp_chpl4 = NULL;
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _ref_Tensor_2__real64_chpl call_tmp_chpl19 = NULL;
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  SumReduceScanOp_Tensor_2__real64_chpl coerce_tmp_chpl5 = NULL;
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _ref_Tensor_2__real64_chpl call_tmp_chpl20 = NULL;
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  ReduceScanOp_chpl coerce_tmp_chpl6 = NULL;
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _ref_chpl_LocalSpinlock_chpl call_tmp_chpl21 = NULL;
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _ref_AtomicBool_chpl call_tmp_chpl22 = NULL;
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _ref_atomic_bool_chpl call_tmp_chpl23 = NULL;
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  memory_order ret_chpl5;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  coerce_tmp_chpl3 = ((ReduceScanOp_chpl)(globalOp_chpl));
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_check_nil(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  call_tmp_chpl6 = &((coerce_tmp_chpl3)->l);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  call_tmp_chpl7 = &((call_tmp_chpl6)->l);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_rmem_consist_maybe_release2(_ln_chpl, _fn_chpl);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  call_tmp_chpl8 = &((call_tmp_chpl7)->_v);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  ret_chpl = memory_order_seq_cst;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  call_tmp_chpl9 = atomic_load_explicit_bool(call_tmp_chpl8, ret_chpl);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  if (call_tmp_chpl9) /* ZLINE: 64 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl */
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  {
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    tmp_chpl2 = UINT8(true);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  } else /* ZLINE: 64 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl */
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  {
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    call_tmp_chpl10 = &((call_tmp_chpl6)->l);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    chpl_rmem_consist_maybe_release5(_ln_chpl, _fn_chpl);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    chpl_rmem_consist_maybe_release5(_ln_chpl, _fn_chpl);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    call_tmp_chpl11 = &((call_tmp_chpl10)->_v);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    ret_chpl2 = memory_order_acquire;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    call_tmp_chpl12 = atomic_exchange_explicit_bool(call_tmp_chpl11, UINT8(true), ret_chpl2);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    chpl_rmem_consist_maybe_acquire5(_ln_chpl, _fn_chpl);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    tmp_chpl2 = call_tmp_chpl12;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  }
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  tmp_chpl = tmp_chpl2;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  while (tmp_chpl) {
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    chpl_task_yield();
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    call_tmp_chpl13 = &((call_tmp_chpl6)->l);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    chpl_rmem_consist_maybe_release2(_ln_chpl, _fn_chpl);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    call_tmp_chpl14 = &((call_tmp_chpl13)->_v);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    ret_chpl3 = memory_order_seq_cst;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    call_tmp_chpl15 = atomic_load_explicit_bool(call_tmp_chpl14, ret_chpl3);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    if (call_tmp_chpl15) /* ZLINE: 64 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl */
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    {
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
      tmp_chpl3 = UINT8(true);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    } else /* ZLINE: 64 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl */
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    {
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
      call_tmp_chpl16 = &((call_tmp_chpl6)->l);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
      chpl_rmem_consist_maybe_release5(_ln_chpl, _fn_chpl);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
      chpl_rmem_consist_maybe_release5(_ln_chpl, _fn_chpl);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
      call_tmp_chpl17 = &((call_tmp_chpl16)->_v);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
      ret_chpl4 = memory_order_acquire;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
      call_tmp_chpl18 = atomic_exchange_explicit_bool(call_tmp_chpl17, UINT8(true), ret_chpl4);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
      chpl_rmem_consist_maybe_acquire5(_ln_chpl, _fn_chpl);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
      tmp_chpl3 = call_tmp_chpl18;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    }
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    tmp_chpl = tmp_chpl3;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  }
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  coerce_tmp_chpl4 = globalOp_chpl;
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_check_nil(coerce_tmp_chpl4, _ln_chpl, _fn_chpl);
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  call_tmp_chpl19 = &((coerce_tmp_chpl4)->value);
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  coerce_tmp_chpl5 = localOp_chpl;
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_check_nil(coerce_tmp_chpl5, _ln_chpl, _fn_chpl);
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  call_tmp_chpl20 = &((coerce_tmp_chpl5)->value);
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl___PLUS__ASSIGN_2(call_tmp_chpl19, call_tmp_chpl20);
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  coerce_tmp_chpl6 = ((ReduceScanOp_chpl)(globalOp_chpl));
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_check_nil(coerce_tmp_chpl6, _ln_chpl, _fn_chpl);
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  call_tmp_chpl21 = &((coerce_tmp_chpl6)->l);
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  call_tmp_chpl22 = &((call_tmp_chpl21)->l);
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_rmem_consist_maybe_release4(_ln_chpl, _fn_chpl);
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_rmem_consist_maybe_release4(_ln_chpl, _fn_chpl);
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  call_tmp_chpl23 = &((call_tmp_chpl22)->_v);
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  ret_chpl5 = memory_order_release;
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  atomic_store_explicit_bool(call_tmp_chpl23, UINT8(false), ret_chpl5);
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_rmem_consist_maybe_acquire4(_ln_chpl, _fn_chpl);
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_rmem_consist_maybe_acquire4(_ln_chpl, _fn_chpl);
#line 62 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  return;
#line 62 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
}

#line 62 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
/* ChapelReduce.chpl:62 */
#line 62 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
static void chpl__reduceCombine(SumReduceScanOp_Tensor_1__real64_chpl globalOp_chpl,
#line 62 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
                                SumReduceScanOp_Tensor_1__real64_chpl localOp_chpl,
#line 62 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
                                int64_t _ln_chpl,
#line 62 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
                                int32_t _fn_chpl) {
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  ReduceScanOp_chpl coerce_tmp_chpl3 = NULL;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _ref_chpl_LocalSpinlock_chpl call_tmp_chpl6 = NULL;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_bool tmp_chpl;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _ref_AtomicBool_chpl call_tmp_chpl7 = NULL;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _ref_atomic_bool_chpl call_tmp_chpl8 = NULL;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  memory_order ret_chpl;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_bool call_tmp_chpl9;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_bool tmp_chpl2;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _ref_AtomicBool_chpl call_tmp_chpl10 = NULL;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _ref_atomic_bool_chpl call_tmp_chpl11 = NULL;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  memory_order ret_chpl2;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_bool call_tmp_chpl12;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _ref_AtomicBool_chpl call_tmp_chpl13 = NULL;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _ref_atomic_bool_chpl call_tmp_chpl14 = NULL;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  memory_order ret_chpl3;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_bool call_tmp_chpl15;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_bool tmp_chpl3;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _ref_AtomicBool_chpl call_tmp_chpl16 = NULL;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _ref_atomic_bool_chpl call_tmp_chpl17 = NULL;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  memory_order ret_chpl4;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_bool call_tmp_chpl18;
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  SumReduceScanOp_Tensor_1__real64_chpl coerce_tmp_chpl4 = NULL;
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _ref_Tensor_1__real64_chpl call_tmp_chpl19 = NULL;
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  SumReduceScanOp_Tensor_1__real64_chpl coerce_tmp_chpl5 = NULL;
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _ref_Tensor_1__real64_chpl call_tmp_chpl20 = NULL;
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  ReduceScanOp_chpl coerce_tmp_chpl6 = NULL;
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _ref_chpl_LocalSpinlock_chpl call_tmp_chpl21 = NULL;
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _ref_AtomicBool_chpl call_tmp_chpl22 = NULL;
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _ref_atomic_bool_chpl call_tmp_chpl23 = NULL;
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  memory_order ret_chpl5;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  coerce_tmp_chpl3 = ((ReduceScanOp_chpl)(globalOp_chpl));
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_check_nil(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  call_tmp_chpl6 = &((coerce_tmp_chpl3)->l);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  call_tmp_chpl7 = &((call_tmp_chpl6)->l);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_rmem_consist_maybe_release2(_ln_chpl, _fn_chpl);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  call_tmp_chpl8 = &((call_tmp_chpl7)->_v);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  ret_chpl = memory_order_seq_cst;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  call_tmp_chpl9 = atomic_load_explicit_bool(call_tmp_chpl8, ret_chpl);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  if (call_tmp_chpl9) /* ZLINE: 64 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl */
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  {
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    tmp_chpl2 = UINT8(true);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  } else /* ZLINE: 64 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl */
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  {
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    call_tmp_chpl10 = &((call_tmp_chpl6)->l);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    chpl_rmem_consist_maybe_release5(_ln_chpl, _fn_chpl);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    chpl_rmem_consist_maybe_release5(_ln_chpl, _fn_chpl);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    call_tmp_chpl11 = &((call_tmp_chpl10)->_v);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    ret_chpl2 = memory_order_acquire;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    call_tmp_chpl12 = atomic_exchange_explicit_bool(call_tmp_chpl11, UINT8(true), ret_chpl2);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    chpl_rmem_consist_maybe_acquire5(_ln_chpl, _fn_chpl);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    tmp_chpl2 = call_tmp_chpl12;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  }
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  tmp_chpl = tmp_chpl2;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  while (tmp_chpl) {
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    chpl_task_yield();
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    call_tmp_chpl13 = &((call_tmp_chpl6)->l);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    chpl_rmem_consist_maybe_release2(_ln_chpl, _fn_chpl);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    call_tmp_chpl14 = &((call_tmp_chpl13)->_v);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    ret_chpl3 = memory_order_seq_cst;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    call_tmp_chpl15 = atomic_load_explicit_bool(call_tmp_chpl14, ret_chpl3);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    if (call_tmp_chpl15) /* ZLINE: 64 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl */
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    {
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
      tmp_chpl3 = UINT8(true);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    } else /* ZLINE: 64 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl */
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    {
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
      call_tmp_chpl16 = &((call_tmp_chpl6)->l);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
      chpl_rmem_consist_maybe_release5(_ln_chpl, _fn_chpl);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
      chpl_rmem_consist_maybe_release5(_ln_chpl, _fn_chpl);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
      call_tmp_chpl17 = &((call_tmp_chpl16)->_v);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
      ret_chpl4 = memory_order_acquire;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
      call_tmp_chpl18 = atomic_exchange_explicit_bool(call_tmp_chpl17, UINT8(true), ret_chpl4);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
      chpl_rmem_consist_maybe_acquire5(_ln_chpl, _fn_chpl);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
      tmp_chpl3 = call_tmp_chpl18;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    }
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    tmp_chpl = tmp_chpl3;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  }
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  coerce_tmp_chpl4 = globalOp_chpl;
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_check_nil(coerce_tmp_chpl4, _ln_chpl, _fn_chpl);
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  call_tmp_chpl19 = &((coerce_tmp_chpl4)->value);
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  coerce_tmp_chpl5 = localOp_chpl;
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_check_nil(coerce_tmp_chpl5, _ln_chpl, _fn_chpl);
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  call_tmp_chpl20 = &((coerce_tmp_chpl5)->value);
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl___PLUS__ASSIGN_4(call_tmp_chpl19, call_tmp_chpl20);
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  coerce_tmp_chpl6 = ((ReduceScanOp_chpl)(globalOp_chpl));
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_check_nil(coerce_tmp_chpl6, _ln_chpl, _fn_chpl);
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  call_tmp_chpl21 = &((coerce_tmp_chpl6)->l);
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  call_tmp_chpl22 = &((call_tmp_chpl21)->l);
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_rmem_consist_maybe_release4(_ln_chpl, _fn_chpl);
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_rmem_consist_maybe_release4(_ln_chpl, _fn_chpl);
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  call_tmp_chpl23 = &((call_tmp_chpl22)->_v);
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  ret_chpl5 = memory_order_release;
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  atomic_store_explicit_bool(call_tmp_chpl23, UINT8(false), ret_chpl5);
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_rmem_consist_maybe_acquire4(_ln_chpl, _fn_chpl);
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_rmem_consist_maybe_acquire4(_ln_chpl, _fn_chpl);
#line 62 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  return;
#line 62 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
}

#line 62 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
/* ChapelReduce.chpl:62 */
#line 62 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
static void chpl__reduceCombine3(SumReduceScanOp_Tensor_4__real64_chpl globalOp_chpl,
#line 62 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
                                 SumReduceScanOp_Tensor_4__real64_chpl localOp_chpl,
#line 62 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
                                 int64_t _ln_chpl,
#line 62 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
                                 int32_t _fn_chpl) {
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  ReduceScanOp_chpl coerce_tmp_chpl3 = NULL;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _ref_chpl_LocalSpinlock_chpl call_tmp_chpl6 = NULL;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_bool tmp_chpl;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _ref_AtomicBool_chpl call_tmp_chpl7 = NULL;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _ref_atomic_bool_chpl call_tmp_chpl8 = NULL;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  memory_order ret_chpl;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_bool call_tmp_chpl9;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_bool tmp_chpl2;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _ref_AtomicBool_chpl call_tmp_chpl10 = NULL;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _ref_atomic_bool_chpl call_tmp_chpl11 = NULL;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  memory_order ret_chpl2;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_bool call_tmp_chpl12;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _ref_AtomicBool_chpl call_tmp_chpl13 = NULL;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _ref_atomic_bool_chpl call_tmp_chpl14 = NULL;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  memory_order ret_chpl3;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_bool call_tmp_chpl15;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_bool tmp_chpl3;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _ref_AtomicBool_chpl call_tmp_chpl16 = NULL;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _ref_atomic_bool_chpl call_tmp_chpl17 = NULL;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  memory_order ret_chpl4;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_bool call_tmp_chpl18;
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  SumReduceScanOp_Tensor_4__real64_chpl coerce_tmp_chpl4 = NULL;
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _ref_Tensor_4__real64_chpl call_tmp_chpl19 = NULL;
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  SumReduceScanOp_Tensor_4__real64_chpl coerce_tmp_chpl5 = NULL;
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _ref_Tensor_4__real64_chpl call_tmp_chpl20 = NULL;
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  ReduceScanOp_chpl coerce_tmp_chpl6 = NULL;
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _ref_chpl_LocalSpinlock_chpl call_tmp_chpl21 = NULL;
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _ref_AtomicBool_chpl call_tmp_chpl22 = NULL;
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _ref_atomic_bool_chpl call_tmp_chpl23 = NULL;
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  memory_order ret_chpl5;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  coerce_tmp_chpl3 = ((ReduceScanOp_chpl)(globalOp_chpl));
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_check_nil(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  call_tmp_chpl6 = &((coerce_tmp_chpl3)->l);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  call_tmp_chpl7 = &((call_tmp_chpl6)->l);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_rmem_consist_maybe_release2(_ln_chpl, _fn_chpl);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  call_tmp_chpl8 = &((call_tmp_chpl7)->_v);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  ret_chpl = memory_order_seq_cst;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  call_tmp_chpl9 = atomic_load_explicit_bool(call_tmp_chpl8, ret_chpl);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  if (call_tmp_chpl9) /* ZLINE: 64 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl */
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  {
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    tmp_chpl2 = UINT8(true);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  } else /* ZLINE: 64 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl */
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  {
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    call_tmp_chpl10 = &((call_tmp_chpl6)->l);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    chpl_rmem_consist_maybe_release5(_ln_chpl, _fn_chpl);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    chpl_rmem_consist_maybe_release5(_ln_chpl, _fn_chpl);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    call_tmp_chpl11 = &((call_tmp_chpl10)->_v);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    ret_chpl2 = memory_order_acquire;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    call_tmp_chpl12 = atomic_exchange_explicit_bool(call_tmp_chpl11, UINT8(true), ret_chpl2);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    chpl_rmem_consist_maybe_acquire5(_ln_chpl, _fn_chpl);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    tmp_chpl2 = call_tmp_chpl12;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  }
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  tmp_chpl = tmp_chpl2;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  while (tmp_chpl) {
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    chpl_task_yield();
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    call_tmp_chpl13 = &((call_tmp_chpl6)->l);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    chpl_rmem_consist_maybe_release2(_ln_chpl, _fn_chpl);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    call_tmp_chpl14 = &((call_tmp_chpl13)->_v);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    ret_chpl3 = memory_order_seq_cst;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    call_tmp_chpl15 = atomic_load_explicit_bool(call_tmp_chpl14, ret_chpl3);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    if (call_tmp_chpl15) /* ZLINE: 64 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl */
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    {
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
      tmp_chpl3 = UINT8(true);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    } else /* ZLINE: 64 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl */
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    {
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
      call_tmp_chpl16 = &((call_tmp_chpl6)->l);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
      chpl_rmem_consist_maybe_release5(_ln_chpl, _fn_chpl);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
      chpl_rmem_consist_maybe_release5(_ln_chpl, _fn_chpl);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
      call_tmp_chpl17 = &((call_tmp_chpl16)->_v);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
      ret_chpl4 = memory_order_acquire;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
      call_tmp_chpl18 = atomic_exchange_explicit_bool(call_tmp_chpl17, UINT8(true), ret_chpl4);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
      chpl_rmem_consist_maybe_acquire5(_ln_chpl, _fn_chpl);
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
      tmp_chpl3 = call_tmp_chpl18;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    }
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    tmp_chpl = tmp_chpl3;
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  }
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  coerce_tmp_chpl4 = globalOp_chpl;
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_check_nil(coerce_tmp_chpl4, _ln_chpl, _fn_chpl);
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  call_tmp_chpl19 = &((coerce_tmp_chpl4)->value);
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  coerce_tmp_chpl5 = localOp_chpl;
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_check_nil(coerce_tmp_chpl5, _ln_chpl, _fn_chpl);
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  call_tmp_chpl20 = &((coerce_tmp_chpl5)->value);
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl___PLUS__ASSIGN_3(call_tmp_chpl19, call_tmp_chpl20);
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  coerce_tmp_chpl6 = ((ReduceScanOp_chpl)(globalOp_chpl));
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_check_nil(coerce_tmp_chpl6, _ln_chpl, _fn_chpl);
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  call_tmp_chpl21 = &((coerce_tmp_chpl6)->l);
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  call_tmp_chpl22 = &((call_tmp_chpl21)->l);
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_rmem_consist_maybe_release4(_ln_chpl, _fn_chpl);
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_rmem_consist_maybe_release4(_ln_chpl, _fn_chpl);
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  call_tmp_chpl23 = &((call_tmp_chpl22)->_v);
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  ret_chpl5 = memory_order_release;
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  atomic_store_explicit_bool(call_tmp_chpl23, UINT8(false), ret_chpl5);
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_rmem_consist_maybe_acquire4(_ln_chpl, _fn_chpl);
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_rmem_consist_maybe_acquire4(_ln_chpl, _fn_chpl);
#line 62 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  return;
#line 62 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
}

#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
/* ChapelReduce.chpl:134 */
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
static void chpl__auto_destroy_ReduceScanOp(ReduceScanOp_chpl this_chpl31,
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
                                            int64_t _ln_chpl,
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
                                            int32_t _fn_chpl) {
#line 135 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _ref_chpl_LocalSpinlock_chpl _field_destructor_tmp__chpl = NULL;
#line 135 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _ref_AtomicBool_chpl _field_destructor_tmp__chpl2 = NULL;
#line 135 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 135 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _field_destructor_tmp__chpl = &((this_chpl31)->l);
#line 135 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _field_destructor_tmp__chpl2 = &((_field_destructor_tmp__chpl)->l);
#line 135 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  deinit_chpl(_field_destructor_tmp__chpl2);
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  return;
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
}

#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
/* ChapelReduce.chpl:138 */
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
static void init_chpl123(SumReduceScanOp__real64_chpl this_chpl31,
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
                         chpl_LocalSpinlock * l_chpl,
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
                         _real64 value_chpl,
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
                         int64_t _ln_chpl,
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
                         int32_t _fn_chpl) {
#line 135 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_LocalSpinlock _formal_tmp_l_chpl;
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  ReduceScanOp_chpl super_tmp_chpl = NULL;
#line 135 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _formal_tmp_l_chpl = *(l_chpl);
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  super_tmp_chpl = &((this_chpl31)->super);
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  ((RootClass_chpl)(&((super_tmp_chpl)->super)))->chpl__cid = chpl__cid_RootClass_chpl;
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  ((RootClass_chpl)(((RootClass_chpl)(super_tmp_chpl))))->chpl__cid = chpl__cid_RootClass_chpl;
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  (super_tmp_chpl)->l = _formal_tmp_l_chpl;
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  ((RootClass_chpl)(super_tmp_chpl))->chpl__cid = chpl__cid_ReduceScanOp_chpl;
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  ((RootClass_chpl)(((ReduceScanOp_chpl)(this_chpl31))))->chpl__cid = chpl__cid_ReduceScanOp_chpl;
#line 140 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 140 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  (this_chpl31)->value = value_chpl;
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  ((RootClass_chpl)(this_chpl31))->chpl__cid = chpl__cid_SumReduceScanOp__real64_chpl;
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  return;
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
}

#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
/* ChapelReduce.chpl:138 */
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
static void init_chpl126(SumReduceScanOp_int64_t_chpl this_chpl31,
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
                         chpl_LocalSpinlock * l_chpl,
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
                         int64_t value_chpl,
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
                         int64_t _ln_chpl,
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
                         int32_t _fn_chpl) {
#line 135 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_LocalSpinlock _formal_tmp_l_chpl;
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  ReduceScanOp_chpl super_tmp_chpl = NULL;
#line 135 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _formal_tmp_l_chpl = *(l_chpl);
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  super_tmp_chpl = &((this_chpl31)->super);
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  ((RootClass_chpl)(&((super_tmp_chpl)->super)))->chpl__cid = chpl__cid_RootClass_chpl;
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  ((RootClass_chpl)(((RootClass_chpl)(super_tmp_chpl))))->chpl__cid = chpl__cid_RootClass_chpl;
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  (super_tmp_chpl)->l = _formal_tmp_l_chpl;
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  ((RootClass_chpl)(super_tmp_chpl))->chpl__cid = chpl__cid_ReduceScanOp_chpl;
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  ((RootClass_chpl)(((ReduceScanOp_chpl)(this_chpl31))))->chpl__cid = chpl__cid_ReduceScanOp_chpl;
#line 140 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 140 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  (this_chpl31)->value = value_chpl;
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  ((RootClass_chpl)(this_chpl31))->chpl__cid = chpl__cid_SumReduceScanOp_int64_t_chpl;
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  return;
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
}

#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
/* ChapelReduce.chpl:138 */
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
static void init_chpl122(SumReduceScanOp_Tensor_2__real64_chpl this_chpl31,
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
                         chpl_LocalSpinlock * l_chpl,
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
                         Tensor_2__real64_chpl * value_chpl,
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
                         int64_t _ln_chpl,
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
                         int32_t _fn_chpl) {
#line 135 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_LocalSpinlock _formal_tmp_l_chpl;
#line 140 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  Tensor_2__real64_chpl _formal_tmp_value_chpl;
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  ReduceScanOp_chpl super_tmp_chpl = NULL;
#line 135 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _formal_tmp_l_chpl = *(l_chpl);
#line 140 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _formal_tmp_value_chpl = *(value_chpl);
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  super_tmp_chpl = &((this_chpl31)->super);
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  ((RootClass_chpl)(&((super_tmp_chpl)->super)))->chpl__cid = chpl__cid_RootClass_chpl;
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  ((RootClass_chpl)(((RootClass_chpl)(super_tmp_chpl))))->chpl__cid = chpl__cid_RootClass_chpl;
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  (super_tmp_chpl)->l = _formal_tmp_l_chpl;
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  ((RootClass_chpl)(super_tmp_chpl))->chpl__cid = chpl__cid_ReduceScanOp_chpl;
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  ((RootClass_chpl)(((ReduceScanOp_chpl)(this_chpl31))))->chpl__cid = chpl__cid_ReduceScanOp_chpl;
#line 140 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 140 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  (this_chpl31)->value = _formal_tmp_value_chpl;
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  ((RootClass_chpl)(this_chpl31))->chpl__cid = chpl__cid_SumReduceScanOp_Tensor_2__real64_chpl;
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  return;
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
}

#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
/* ChapelReduce.chpl:138 */
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
static void init_chpl125(SumReduceScanOp_Tensor_1__real64_chpl this_chpl31,
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
                         chpl_LocalSpinlock * l_chpl,
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
                         Tensor_1__real64_chpl * value_chpl,
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
                         int64_t _ln_chpl,
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
                         int32_t _fn_chpl) {
#line 135 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_LocalSpinlock _formal_tmp_l_chpl;
#line 140 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  Tensor_1__real64_chpl _formal_tmp_value_chpl;
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  ReduceScanOp_chpl super_tmp_chpl = NULL;
#line 135 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _formal_tmp_l_chpl = *(l_chpl);
#line 140 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _formal_tmp_value_chpl = *(value_chpl);
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  super_tmp_chpl = &((this_chpl31)->super);
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  ((RootClass_chpl)(&((super_tmp_chpl)->super)))->chpl__cid = chpl__cid_RootClass_chpl;
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  ((RootClass_chpl)(((RootClass_chpl)(super_tmp_chpl))))->chpl__cid = chpl__cid_RootClass_chpl;
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  (super_tmp_chpl)->l = _formal_tmp_l_chpl;
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  ((RootClass_chpl)(super_tmp_chpl))->chpl__cid = chpl__cid_ReduceScanOp_chpl;
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  ((RootClass_chpl)(((ReduceScanOp_chpl)(this_chpl31))))->chpl__cid = chpl__cid_ReduceScanOp_chpl;
#line 140 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 140 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  (this_chpl31)->value = _formal_tmp_value_chpl;
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  ((RootClass_chpl)(this_chpl31))->chpl__cid = chpl__cid_SumReduceScanOp_Tensor_1__real64_chpl;
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  return;
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
}

#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
/* ChapelReduce.chpl:138 */
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
static void init_chpl124(SumReduceScanOp_Tensor_4__real64_chpl this_chpl31,
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
                         chpl_LocalSpinlock * l_chpl,
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
                         Tensor_4__real64_chpl * value_chpl,
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
                         int64_t _ln_chpl,
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
                         int32_t _fn_chpl) {
#line 135 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_LocalSpinlock _formal_tmp_l_chpl;
#line 140 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  Tensor_4__real64_chpl _formal_tmp_value_chpl;
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  ReduceScanOp_chpl super_tmp_chpl = NULL;
#line 135 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _formal_tmp_l_chpl = *(l_chpl);
#line 140 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _formal_tmp_value_chpl = *(value_chpl);
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  super_tmp_chpl = &((this_chpl31)->super);
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  ((RootClass_chpl)(&((super_tmp_chpl)->super)))->chpl__cid = chpl__cid_RootClass_chpl;
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  ((RootClass_chpl)(((RootClass_chpl)(super_tmp_chpl))))->chpl__cid = chpl__cid_RootClass_chpl;
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  (super_tmp_chpl)->l = _formal_tmp_l_chpl;
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  ((RootClass_chpl)(super_tmp_chpl))->chpl__cid = chpl__cid_ReduceScanOp_chpl;
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  ((RootClass_chpl)(((ReduceScanOp_chpl)(this_chpl31))))->chpl__cid = chpl__cid_ReduceScanOp_chpl;
#line 140 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 140 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  (this_chpl31)->value = _formal_tmp_value_chpl;
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  ((RootClass_chpl)(this_chpl31))->chpl__cid = chpl__cid_SumReduceScanOp_Tensor_4__real64_chpl;
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  return;
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
}

#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
/* ChapelReduce.chpl:138 */
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
static void chpl__auto_destroy_SumReduceScanOp(SumReduceScanOp__real64_chpl this_chpl31,
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
                                               int64_t _ln_chpl,
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
                                               int32_t _fn_chpl) {
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  ReduceScanOp_chpl _parent_destructor_tmp__chpl = NULL;
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _ref_chpl_LocalSpinlock_chpl _field_destructor_tmp__chpl = NULL;
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _ref_AtomicBool_chpl _field_destructor_tmp__chpl2 = NULL;
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _parent_destructor_tmp__chpl = ((ReduceScanOp_chpl)(this_chpl31));
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_check_nil(_parent_destructor_tmp__chpl, _ln_chpl, _fn_chpl);
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _field_destructor_tmp__chpl = &((_parent_destructor_tmp__chpl)->l);
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _field_destructor_tmp__chpl2 = &((_field_destructor_tmp__chpl)->l);
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  deinit_chpl(_field_destructor_tmp__chpl2);
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  return;
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
}

#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
/* ChapelReduce.chpl:138 */
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
static void chpl__auto_destroy_SumReduceScanOp2(SumReduceScanOp_int64_t_chpl this_chpl31,
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
                                                int64_t _ln_chpl,
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
                                                int32_t _fn_chpl) {
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  ReduceScanOp_chpl _parent_destructor_tmp__chpl = NULL;
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _ref_chpl_LocalSpinlock_chpl _field_destructor_tmp__chpl = NULL;
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _ref_AtomicBool_chpl _field_destructor_tmp__chpl2 = NULL;
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _parent_destructor_tmp__chpl = ((ReduceScanOp_chpl)(this_chpl31));
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_check_nil(_parent_destructor_tmp__chpl, _ln_chpl, _fn_chpl);
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _field_destructor_tmp__chpl = &((_parent_destructor_tmp__chpl)->l);
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _field_destructor_tmp__chpl2 = &((_field_destructor_tmp__chpl)->l);
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  deinit_chpl(_field_destructor_tmp__chpl2);
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  return;
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
}

#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
/* ChapelReduce.chpl:138 */
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
static void chpl__auto_destroy_SumReduceScanOp5(SumReduceScanOp_Tensor_2__real64_chpl this_chpl31,
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
                                                int64_t _ln_chpl,
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
                                                int32_t _fn_chpl) {
#line 140 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _ref_Tensor_2__real64_chpl _field_destructor_tmp__chpl = NULL;
#line 140 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _array_DefaultRectangularArr_2_int64_t_one__real64_int64_t_chpl _field_destructor_tmp__chpl2;
#line 140 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _domain_DefaultRectangularDom_2_int64_t_one_chpl _field_destructor_tmp__chpl3;
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  ReduceScanOp_chpl _parent_destructor_tmp__chpl = NULL;
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _ref_chpl_LocalSpinlock_chpl _field_destructor_tmp__chpl4 = NULL;
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _ref_AtomicBool_chpl _field_destructor_tmp__chpl5 = NULL;
#line 140 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 140 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _field_destructor_tmp__chpl = &((this_chpl31)->value);
#line 140 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _field_destructor_tmp__chpl2 = (_field_destructor_tmp__chpl)->data;
#line 140 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl__autoDestroy19(&_field_destructor_tmp__chpl2, _ln_chpl, _fn_chpl);
#line 140 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _field_destructor_tmp__chpl3 = (_field_destructor_tmp__chpl)->_domain;
#line 140 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl__autoDestroy5(&_field_destructor_tmp__chpl3, _ln_chpl, _fn_chpl);
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _parent_destructor_tmp__chpl = ((ReduceScanOp_chpl)(this_chpl31));
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_check_nil(_parent_destructor_tmp__chpl, _ln_chpl, _fn_chpl);
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _field_destructor_tmp__chpl4 = &((_parent_destructor_tmp__chpl)->l);
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _field_destructor_tmp__chpl5 = &((_field_destructor_tmp__chpl4)->l);
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  deinit_chpl(_field_destructor_tmp__chpl5);
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  return;
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
}

#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
/* ChapelReduce.chpl:138 */
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
static void chpl__auto_destroy_SumReduceScanOp4(SumReduceScanOp_Tensor_1__real64_chpl this_chpl31,
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
                                                int64_t _ln_chpl,
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
                                                int32_t _fn_chpl) {
#line 140 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _ref_Tensor_1__real64_chpl _field_destructor_tmp__chpl = NULL;
#line 140 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _array_DefaultRectangularArr_1_int64_t_one__real64_int64_t_chpl _field_destructor_tmp__chpl2;
#line 140 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _domain_DefaultRectangularDom_1_int64_t_one_chpl _field_destructor_tmp__chpl3;
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  ReduceScanOp_chpl _parent_destructor_tmp__chpl = NULL;
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _ref_chpl_LocalSpinlock_chpl _field_destructor_tmp__chpl4 = NULL;
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _ref_AtomicBool_chpl _field_destructor_tmp__chpl5 = NULL;
#line 140 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 140 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _field_destructor_tmp__chpl = &((this_chpl31)->value);
#line 140 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _field_destructor_tmp__chpl2 = (_field_destructor_tmp__chpl)->data;
#line 140 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl__autoDestroy18(&_field_destructor_tmp__chpl2, _ln_chpl, _fn_chpl);
#line 140 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _field_destructor_tmp__chpl3 = (_field_destructor_tmp__chpl)->_domain;
#line 140 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl__autoDestroy11(&_field_destructor_tmp__chpl3, _ln_chpl, _fn_chpl);
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _parent_destructor_tmp__chpl = ((ReduceScanOp_chpl)(this_chpl31));
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_check_nil(_parent_destructor_tmp__chpl, _ln_chpl, _fn_chpl);
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _field_destructor_tmp__chpl4 = &((_parent_destructor_tmp__chpl)->l);
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _field_destructor_tmp__chpl5 = &((_field_destructor_tmp__chpl4)->l);
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  deinit_chpl(_field_destructor_tmp__chpl5);
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  return;
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
}

#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
/* ChapelReduce.chpl:138 */
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
static void chpl__auto_destroy_SumReduceScanOp3(SumReduceScanOp_Tensor_4__real64_chpl this_chpl31,
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
                                                int64_t _ln_chpl,
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
                                                int32_t _fn_chpl) {
#line 140 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _ref_Tensor_4__real64_chpl _field_destructor_tmp__chpl = NULL;
#line 140 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _array_DefaultRectangularArr_4_int64_t_one__real64_int64_t_chpl _field_destructor_tmp__chpl2;
#line 140 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _domain_DefaultRectangularDom_4_int64_t_one_chpl _field_destructor_tmp__chpl3;
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  ReduceScanOp_chpl _parent_destructor_tmp__chpl = NULL;
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _ref_chpl_LocalSpinlock_chpl _field_destructor_tmp__chpl4 = NULL;
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _ref_AtomicBool_chpl _field_destructor_tmp__chpl5 = NULL;
#line 140 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 140 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _field_destructor_tmp__chpl = &((this_chpl31)->value);
#line 140 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _field_destructor_tmp__chpl2 = (_field_destructor_tmp__chpl)->data;
#line 140 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl__autoDestroy25(&_field_destructor_tmp__chpl2, _ln_chpl, _fn_chpl);
#line 140 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _field_destructor_tmp__chpl3 = (_field_destructor_tmp__chpl)->_domain;
#line 140 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl__autoDestroy9(&_field_destructor_tmp__chpl3, _ln_chpl, _fn_chpl);
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _parent_destructor_tmp__chpl = ((ReduceScanOp_chpl)(this_chpl31));
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_check_nil(_parent_destructor_tmp__chpl, _ln_chpl, _fn_chpl);
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _field_destructor_tmp__chpl4 = &((_parent_destructor_tmp__chpl)->l);
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _field_destructor_tmp__chpl5 = &((_field_destructor_tmp__chpl4)->l);
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  deinit_chpl(_field_destructor_tmp__chpl5);
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  return;
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
}

#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
/* ChapelReduce.chpl:138 */
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
static SumReduceScanOp__real64_chpl _new_chpl23(chpl_LocalSpinlock * l_chpl,
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
                                                _real64 value_chpl,
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
                                                int64_t _ln_chpl,
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
                                                int32_t _fn_chpl) {
#line 135 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_LocalSpinlock _formal_tmp_l_chpl;
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  SumReduceScanOp__real64_chpl initTemp_chpl = NULL;
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  c_ptr_void_chpl cast_tmp_chpl = NULL;
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  SumReduceScanOp__real64_chpl tmp_chpl = NULL;
#line 135 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _formal_tmp_l_chpl = *(l_chpl);
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  cast_tmp_chpl = chpl_here_alloc(sizeof(chpl_SumReduceScanOp__real64_chpl_object), INT16(262), _ln_chpl, _fn_chpl);
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  initTemp_chpl = ((SumReduceScanOp__real64_chpl)(cast_tmp_chpl));
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  ((RootClass_chpl)(initTemp_chpl))->chpl__cid = chpl__cid_SumReduceScanOp__real64_chpl;
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_check_nil(initTemp_chpl, _ln_chpl, _fn_chpl);
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  init_chpl123(initTemp_chpl, &_formal_tmp_l_chpl, value_chpl, _ln_chpl, _fn_chpl);
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  tmp_chpl = initTemp_chpl;
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  return tmp_chpl;
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
}

#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
/* ChapelReduce.chpl:138 */
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
static SumReduceScanOp_int64_t_chpl _new_chpl21(chpl_LocalSpinlock * l_chpl,
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
                                                int64_t value_chpl,
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
                                                int64_t _ln_chpl,
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
                                                int32_t _fn_chpl) {
#line 135 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_LocalSpinlock _formal_tmp_l_chpl;
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  SumReduceScanOp_int64_t_chpl initTemp_chpl = NULL;
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  c_ptr_void_chpl cast_tmp_chpl = NULL;
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  SumReduceScanOp_int64_t_chpl tmp_chpl = NULL;
#line 135 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _formal_tmp_l_chpl = *(l_chpl);
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  cast_tmp_chpl = chpl_here_alloc(sizeof(chpl_SumReduceScanOp_int64_t_chpl_object), INT16(337), _ln_chpl, _fn_chpl);
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  initTemp_chpl = ((SumReduceScanOp_int64_t_chpl)(cast_tmp_chpl));
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  ((RootClass_chpl)(initTemp_chpl))->chpl__cid = chpl__cid_SumReduceScanOp_int64_t_chpl;
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_check_nil(initTemp_chpl, _ln_chpl, _fn_chpl);
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  init_chpl126(initTemp_chpl, &_formal_tmp_l_chpl, value_chpl, _ln_chpl, _fn_chpl);
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  tmp_chpl = initTemp_chpl;
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  return tmp_chpl;
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
}

#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
/* ChapelReduce.chpl:138 */
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
static SumReduceScanOp_Tensor_2__real64_chpl _new_chpl22(chpl_LocalSpinlock * l_chpl,
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
                                                         Tensor_2__real64_chpl * value_chpl,
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
                                                         int64_t _ln_chpl,
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
                                                         int32_t _fn_chpl) {
#line 135 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_LocalSpinlock _formal_tmp_l_chpl;
#line 140 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  Tensor_2__real64_chpl _formal_tmp_value_chpl;
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  SumReduceScanOp_Tensor_2__real64_chpl initTemp_chpl = NULL;
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  c_ptr_void_chpl cast_tmp_chpl = NULL;
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  SumReduceScanOp_Tensor_2__real64_chpl tmp_chpl = NULL;
#line 135 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _formal_tmp_l_chpl = *(l_chpl);
#line 140 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _formal_tmp_value_chpl = *(value_chpl);
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  cast_tmp_chpl = chpl_here_alloc(sizeof(chpl_SumReduceScanOp_Tensor_2__real64_chpl_object), INT16(343), _ln_chpl, _fn_chpl);
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  initTemp_chpl = ((SumReduceScanOp_Tensor_2__real64_chpl)(cast_tmp_chpl));
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  ((RootClass_chpl)(initTemp_chpl))->chpl__cid = chpl__cid_SumReduceScanOp_Tensor_2__real64_chpl;
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_check_nil(initTemp_chpl, _ln_chpl, _fn_chpl);
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  init_chpl122(initTemp_chpl, &_formal_tmp_l_chpl, &_formal_tmp_value_chpl, _ln_chpl, _fn_chpl);
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  tmp_chpl = initTemp_chpl;
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  return tmp_chpl;
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
}

#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
/* ChapelReduce.chpl:138 */
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
static SumReduceScanOp_Tensor_1__real64_chpl _new_chpl24(chpl_LocalSpinlock * l_chpl,
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
                                                         Tensor_1__real64_chpl * value_chpl,
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
                                                         int64_t _ln_chpl,
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
                                                         int32_t _fn_chpl) {
#line 135 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_LocalSpinlock _formal_tmp_l_chpl;
#line 140 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  Tensor_1__real64_chpl _formal_tmp_value_chpl;
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  SumReduceScanOp_Tensor_1__real64_chpl initTemp_chpl = NULL;
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  c_ptr_void_chpl cast_tmp_chpl = NULL;
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  SumReduceScanOp_Tensor_1__real64_chpl tmp_chpl = NULL;
#line 135 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _formal_tmp_l_chpl = *(l_chpl);
#line 140 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _formal_tmp_value_chpl = *(value_chpl);
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  cast_tmp_chpl = chpl_here_alloc(sizeof(chpl_SumReduceScanOp_Tensor_1__real64_chpl_object), INT16(344), _ln_chpl, _fn_chpl);
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  initTemp_chpl = ((SumReduceScanOp_Tensor_1__real64_chpl)(cast_tmp_chpl));
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  ((RootClass_chpl)(initTemp_chpl))->chpl__cid = chpl__cid_SumReduceScanOp_Tensor_1__real64_chpl;
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_check_nil(initTemp_chpl, _ln_chpl, _fn_chpl);
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  init_chpl125(initTemp_chpl, &_formal_tmp_l_chpl, &_formal_tmp_value_chpl, _ln_chpl, _fn_chpl);
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  tmp_chpl = initTemp_chpl;
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  return tmp_chpl;
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
}

#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
/* ChapelReduce.chpl:138 */
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
static SumReduceScanOp_Tensor_4__real64_chpl _new_chpl20(chpl_LocalSpinlock * l_chpl,
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
                                                         Tensor_4__real64_chpl * value_chpl,
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
                                                         int64_t _ln_chpl,
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
                                                         int32_t _fn_chpl) {
#line 135 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_LocalSpinlock _formal_tmp_l_chpl;
#line 140 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  Tensor_4__real64_chpl _formal_tmp_value_chpl;
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  SumReduceScanOp_Tensor_4__real64_chpl initTemp_chpl = NULL;
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  c_ptr_void_chpl cast_tmp_chpl = NULL;
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  SumReduceScanOp_Tensor_4__real64_chpl tmp_chpl = NULL;
#line 135 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _formal_tmp_l_chpl = *(l_chpl);
#line 140 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _formal_tmp_value_chpl = *(value_chpl);
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  cast_tmp_chpl = chpl_here_alloc(sizeof(chpl_SumReduceScanOp_Tensor_4__real64_chpl_object), INT16(365), _ln_chpl, _fn_chpl);
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  initTemp_chpl = ((SumReduceScanOp_Tensor_4__real64_chpl)(cast_tmp_chpl));
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  ((RootClass_chpl)(initTemp_chpl))->chpl__cid = chpl__cid_SumReduceScanOp_Tensor_4__real64_chpl;
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_check_nil(initTemp_chpl, _ln_chpl, _fn_chpl);
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  init_chpl124(initTemp_chpl, &_formal_tmp_l_chpl, &_formal_tmp_value_chpl, _ln_chpl, _fn_chpl);
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  tmp_chpl = initTemp_chpl;
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  return tmp_chpl;
#line 138 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
}

#line 160 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
/* ChapelReduce.chpl:160 */
#line 160 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
static void init_chpl127(ProductReduceScanOp_int64_t_chpl this_chpl31,
#line 160 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
                         chpl_LocalSpinlock * l_chpl,
#line 160 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
                         int64_t value_chpl,
#line 160 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
                         int64_t _ln_chpl,
#line 160 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
                         int32_t _fn_chpl) {
#line 135 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_LocalSpinlock _formal_tmp_l_chpl;
#line 160 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  ReduceScanOp_chpl super_tmp_chpl = NULL;
#line 135 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _formal_tmp_l_chpl = *(l_chpl);
#line 160 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 160 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  super_tmp_chpl = &((this_chpl31)->super);
#line 160 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 160 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  ((RootClass_chpl)(&((super_tmp_chpl)->super)))->chpl__cid = chpl__cid_RootClass_chpl;
#line 160 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  ((RootClass_chpl)(((RootClass_chpl)(super_tmp_chpl))))->chpl__cid = chpl__cid_RootClass_chpl;
#line 160 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 160 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  (super_tmp_chpl)->l = _formal_tmp_l_chpl;
#line 160 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  ((RootClass_chpl)(super_tmp_chpl))->chpl__cid = chpl__cid_ReduceScanOp_chpl;
#line 160 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  ((RootClass_chpl)(((ReduceScanOp_chpl)(this_chpl31))))->chpl__cid = chpl__cid_ReduceScanOp_chpl;
#line 162 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 162 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  (this_chpl31)->value = value_chpl;
#line 160 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  ((RootClass_chpl)(this_chpl31))->chpl__cid = chpl__cid_ProductReduceScanOp_int64_t_chpl;
#line 160 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  return;
#line 160 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
}

#line 160 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
/* ChapelReduce.chpl:160 */
#line 160 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
static void chpl__auto_destroy_ProductReduceScanOp(ProductReduceScanOp_int64_t_chpl this_chpl31,
#line 160 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
                                                   int64_t _ln_chpl,
#line 160 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
                                                   int32_t _fn_chpl) {
#line 160 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  ReduceScanOp_chpl _parent_destructor_tmp__chpl = NULL;
#line 160 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _ref_chpl_LocalSpinlock_chpl _field_destructor_tmp__chpl = NULL;
#line 160 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _ref_AtomicBool_chpl _field_destructor_tmp__chpl2 = NULL;
#line 160 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _parent_destructor_tmp__chpl = ((ReduceScanOp_chpl)(this_chpl31));
#line 160 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_check_nil(_parent_destructor_tmp__chpl, _ln_chpl, _fn_chpl);
#line 160 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _field_destructor_tmp__chpl = &((_parent_destructor_tmp__chpl)->l);
#line 160 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _field_destructor_tmp__chpl2 = &((_field_destructor_tmp__chpl)->l);
#line 160 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  deinit_chpl(_field_destructor_tmp__chpl2);
#line 160 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  return;
#line 160 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
}

#line 160 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
/* ChapelReduce.chpl:160 */
#line 160 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
static ProductReduceScanOp_int64_t_chpl _new_chpl25(chpl_LocalSpinlock * l_chpl,
#line 160 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
                                                    int64_t value_chpl,
#line 160 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
                                                    int64_t _ln_chpl,
#line 160 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
                                                    int32_t _fn_chpl) {
#line 135 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_LocalSpinlock _formal_tmp_l_chpl;
#line 160 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  ProductReduceScanOp_int64_t_chpl initTemp_chpl = NULL;
#line 160 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  c_ptr_void_chpl cast_tmp_chpl = NULL;
#line 160 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  ProductReduceScanOp_int64_t_chpl tmp_chpl = NULL;
#line 135 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _formal_tmp_l_chpl = *(l_chpl);
#line 160 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  cast_tmp_chpl = chpl_here_alloc(sizeof(chpl_ProductReduceScanOp_int64_t_chpl_object), INT16(288), _ln_chpl, _fn_chpl);
#line 160 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  initTemp_chpl = ((ProductReduceScanOp_int64_t_chpl)(cast_tmp_chpl));
#line 160 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  ((RootClass_chpl)(initTemp_chpl))->chpl__cid = chpl__cid_ProductReduceScanOp_int64_t_chpl;
#line 160 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_check_nil(initTemp_chpl, _ln_chpl, _fn_chpl);
#line 160 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  init_chpl127(initTemp_chpl, &_formal_tmp_l_chpl, value_chpl, _ln_chpl, _fn_chpl);
#line 160 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  tmp_chpl = initTemp_chpl;
#line 160 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  return tmp_chpl;
#line 160 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
}

#line 164 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
/* ChapelReduce.chpl:164 */
#line 164 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
static int64_t identity_chpl(ProductReduceScanOp_int64_t_chpl this_chpl31) {
#line 164 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  return INT64(1);
#line 164 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
}

#line 165 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
/* ChapelReduce.chpl:165 */
#line 165 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
static void accumulate_chpl(ProductReduceScanOp_int64_t_chpl this_chpl31,
#line 165 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
                            int64_t x_chpl,
#line 165 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
                            int64_t _ln_chpl,
#line 165 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
                            int32_t _fn_chpl) {
#line 166 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _ref_int64_t_chpl call_tmp_chpl6 = NULL;
#line 166 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 166 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  call_tmp_chpl6 = &((this_chpl31)->value);
#line 166 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  *(call_tmp_chpl6) *= x_chpl;
#line 165 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  return;
#line 165 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
}

#line 168 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
/* ChapelReduce.chpl:168 */
#line 168 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
static void accumulateOntoState_chpl(ProductReduceScanOp_int64_t_chpl this_chpl31,
#line 168 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
                                     int64_t * state_chpl,
#line 168 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
                                     int64_t x_chpl) {
#line 169 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  *(state_chpl) *= x_chpl;
#line 168 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  return;
#line 168 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
}

#line 171 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
/* ChapelReduce.chpl:171 */
#line 171 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
static void combine_chpl(ProductReduceScanOp_int64_t_chpl this_chpl31,
#line 171 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
                         ProductReduceScanOp_int64_t_chpl x_chpl,
#line 171 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
                         int64_t _ln_chpl,
#line 171 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
                         int32_t _fn_chpl) {
#line 172 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _ref_int64_t_chpl call_tmp_chpl6 = NULL;
#line 172 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  ProductReduceScanOp_int64_t_chpl coerce_tmp_chpl3 = NULL;
#line 172 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 172 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  call_tmp_chpl6 = &((this_chpl31)->value);
#line 172 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  coerce_tmp_chpl3 = x_chpl;
#line 172 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_check_nil(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 172 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  *(call_tmp_chpl6) *= (coerce_tmp_chpl3)->value;
#line 171 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  return;
#line 171 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
}

#line 174 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
/* ChapelReduce.chpl:174 */
#line 174 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
static int64_t generate_chpl(ProductReduceScanOp_int64_t_chpl this_chpl31,
#line 174 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
                             int64_t _ln_chpl,
#line 174 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
                             int32_t _fn_chpl) {
#line 174 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  int64_t ret_chpl;
#line 174 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 174 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  ret_chpl = (this_chpl31)->value;
#line 174 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  return ret_chpl;
#line 174 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
}

#line 175 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
/* ChapelReduce.chpl:175 */
#line 175 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
static ProductReduceScanOp_int64_t_chpl clone_chpl2(ProductReduceScanOp_int64_t_chpl this_chpl31,
#line 175 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
                                                    int64_t _ln_chpl,
#line 175 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
                                                    int32_t _fn_chpl) {
#line 175 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  ProductReduceScanOp_int64_t_chpl new_temp_chpl = NULL;
#line 175 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_LocalSpinlock default_arg_l_chpl;
#line 175 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_LocalSpinlock ret_chpl;
#line 175 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_LocalSpinlock l_chpl;
#line 175 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  init_chpl105(&l_chpl);
#line 175 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  ret_chpl = l_chpl;
#line 175 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  default_arg_l_chpl = ret_chpl;
#line 175 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  new_temp_chpl = _new_chpl25(&default_arg_l_chpl, INT64(1), _ln_chpl, _fn_chpl);
#line 175 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  return new_temp_chpl;
#line 175 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
}

#line 178 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
/* ChapelReduce.chpl:178 */
#line 178 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
static void init_chpl128(MaxReduceScanOp__real64_chpl this_chpl31,
#line 178 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
                         chpl_LocalSpinlock * l_chpl,
#line 178 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
                         _real64 value_chpl,
#line 178 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
                         int64_t _ln_chpl,
#line 178 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
                         int32_t _fn_chpl) {
#line 135 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_LocalSpinlock _formal_tmp_l_chpl;
#line 178 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  ReduceScanOp_chpl super_tmp_chpl = NULL;
#line 135 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _formal_tmp_l_chpl = *(l_chpl);
#line 178 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 178 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  super_tmp_chpl = &((this_chpl31)->super);
#line 178 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 178 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  ((RootClass_chpl)(&((super_tmp_chpl)->super)))->chpl__cid = chpl__cid_RootClass_chpl;
#line 178 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  ((RootClass_chpl)(((RootClass_chpl)(super_tmp_chpl))))->chpl__cid = chpl__cid_RootClass_chpl;
#line 178 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 178 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  (super_tmp_chpl)->l = _formal_tmp_l_chpl;
#line 178 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  ((RootClass_chpl)(super_tmp_chpl))->chpl__cid = chpl__cid_ReduceScanOp_chpl;
#line 178 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  ((RootClass_chpl)(((ReduceScanOp_chpl)(this_chpl31))))->chpl__cid = chpl__cid_ReduceScanOp_chpl;
#line 180 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 180 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  (this_chpl31)->value = value_chpl;
#line 178 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  ((RootClass_chpl)(this_chpl31))->chpl__cid = chpl__cid_MaxReduceScanOp__real64_chpl;
#line 178 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  return;
#line 178 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
}

#line 178 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
/* ChapelReduce.chpl:178 */
#line 178 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
static void chpl__auto_destroy_MaxReduceScanOp(MaxReduceScanOp__real64_chpl this_chpl31,
#line 178 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
                                               int64_t _ln_chpl,
#line 178 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
                                               int32_t _fn_chpl) {
#line 178 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  ReduceScanOp_chpl _parent_destructor_tmp__chpl = NULL;
#line 178 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _ref_chpl_LocalSpinlock_chpl _field_destructor_tmp__chpl = NULL;
#line 178 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _ref_AtomicBool_chpl _field_destructor_tmp__chpl2 = NULL;
#line 178 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _parent_destructor_tmp__chpl = ((ReduceScanOp_chpl)(this_chpl31));
#line 178 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_check_nil(_parent_destructor_tmp__chpl, _ln_chpl, _fn_chpl);
#line 178 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _field_destructor_tmp__chpl = &((_parent_destructor_tmp__chpl)->l);
#line 178 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _field_destructor_tmp__chpl2 = &((_field_destructor_tmp__chpl)->l);
#line 178 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  deinit_chpl(_field_destructor_tmp__chpl2);
#line 178 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  return;
#line 178 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
}

#line 178 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
/* ChapelReduce.chpl:178 */
#line 178 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
static MaxReduceScanOp__real64_chpl _new_chpl26(chpl_LocalSpinlock * l_chpl,
#line 178 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
                                                _real64 value_chpl,
#line 178 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
                                                int64_t _ln_chpl,
#line 178 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
                                                int32_t _fn_chpl) {
#line 135 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_LocalSpinlock _formal_tmp_l_chpl;
#line 178 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  MaxReduceScanOp__real64_chpl initTemp_chpl = NULL;
#line 178 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  c_ptr_void_chpl cast_tmp_chpl = NULL;
#line 178 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  MaxReduceScanOp__real64_chpl tmp_chpl = NULL;
#line 135 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _formal_tmp_l_chpl = *(l_chpl);
#line 178 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  cast_tmp_chpl = chpl_here_alloc(sizeof(chpl_MaxReduceScanOp__real64_chpl_object), INT16(286), _ln_chpl, _fn_chpl);
#line 178 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  initTemp_chpl = ((MaxReduceScanOp__real64_chpl)(cast_tmp_chpl));
#line 178 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  ((RootClass_chpl)(initTemp_chpl))->chpl__cid = chpl__cid_MaxReduceScanOp__real64_chpl;
#line 178 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_check_nil(initTemp_chpl, _ln_chpl, _fn_chpl);
#line 178 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  init_chpl128(initTemp_chpl, &_formal_tmp_l_chpl, value_chpl, _ln_chpl, _fn_chpl);
#line 178 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  tmp_chpl = initTemp_chpl;
#line 178 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  return tmp_chpl;
#line 178 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
}

#line 182 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
/* ChapelReduce.chpl:182 */
#line 182 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
static _real64 identity_chpl2(MaxReduceScanOp__real64_chpl this_chpl31) {
#line 182 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _real64 call_tmp_chpl6;
#line 182 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  call_tmp_chpl6 = min_chpl();
#line 182 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  return call_tmp_chpl6;
#line 182 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
}

#line 183 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
/* ChapelReduce.chpl:183 */
#line 183 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
static void accumulate_chpl2(MaxReduceScanOp__real64_chpl this_chpl31,
#line 183 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
                             _real64 x_chpl,
#line 183 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
                             int64_t _ln_chpl,
#line 183 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
                             int32_t _fn_chpl) {
#line 184 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _real64 coerce_tmp_chpl3;
#line 184 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _real64 tmp_chpl;
#line 184 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_bool call_tmp_chpl6;
#line 184 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  int32_t call_tmp_chpl7;
#line 184 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 184 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  coerce_tmp_chpl3 = (this_chpl31)->value;
#line 184 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  call_tmp_chpl6 = (x_chpl > coerce_tmp_chpl3);
#line 184 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  call_tmp_chpl7 = chpl_macro_double_isnan(x_chpl);
#line 184 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  if (call_tmp_chpl6 | (call_tmp_chpl7 != INT64(0))) /* ZLINE: 184 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl */
#line 184 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  {
#line 184 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    tmp_chpl = x_chpl;
#line 184 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  } else /* ZLINE: 184 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl */
#line 184 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  {
#line 184 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    tmp_chpl = coerce_tmp_chpl3;
#line 184 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  }
#line 184 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 184 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  (this_chpl31)->value = tmp_chpl;
#line 183 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  return;
#line 183 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
}

#line 186 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
/* ChapelReduce.chpl:186 */
#line 186 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
static void accumulateOntoState_chpl2(MaxReduceScanOp__real64_chpl this_chpl31,
#line 186 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
                                      _real64 * state_chpl,
#line 186 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
                                      _real64 x_chpl,
#line 186 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
                                      int64_t _ln_chpl,
#line 186 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
                                      int32_t _fn_chpl) {
#line 187 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _real64 coerce_tmp_chpl3;
#line 187 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _real64 tmp_chpl;
#line 187 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_bool call_tmp_chpl6;
#line 187 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  int32_t call_tmp_chpl7;
#line 187 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  coerce_tmp_chpl3 = *(state_chpl);
#line 187 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  call_tmp_chpl6 = (coerce_tmp_chpl3 > x_chpl);
#line 187 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  call_tmp_chpl7 = chpl_macro_double_isnan(coerce_tmp_chpl3);
#line 187 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  if (call_tmp_chpl6 | (call_tmp_chpl7 != INT64(0))) /* ZLINE: 187 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl */
#line 187 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  {
#line 187 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    tmp_chpl = coerce_tmp_chpl3;
#line 187 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  } else /* ZLINE: 187 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl */
#line 187 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  {
#line 187 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    tmp_chpl = x_chpl;
#line 187 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  }
#line 187 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  *(state_chpl) = tmp_chpl;
#line 186 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  return;
#line 186 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
}

#line 189 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
/* ChapelReduce.chpl:189 */
#line 189 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
static void combine_chpl2(MaxReduceScanOp__real64_chpl this_chpl31,
#line 189 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
                          MaxReduceScanOp__real64_chpl x_chpl,
#line 189 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
                          int64_t _ln_chpl,
#line 189 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
                          int32_t _fn_chpl) {
#line 190 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  MaxReduceScanOp__real64_chpl coerce_tmp_chpl3 = NULL;
#line 190 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _real64 coerce_tmp_chpl4;
#line 190 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _real64 coerce_tmp_chpl5;
#line 190 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _real64 tmp_chpl;
#line 190 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_bool call_tmp_chpl6;
#line 190 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  int32_t call_tmp_chpl7;
#line 190 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  coerce_tmp_chpl3 = x_chpl;
#line 190 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 190 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  coerce_tmp_chpl4 = (this_chpl31)->value;
#line 190 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_check_nil(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 190 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  coerce_tmp_chpl5 = (coerce_tmp_chpl3)->value;
#line 190 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  call_tmp_chpl6 = (coerce_tmp_chpl4 > coerce_tmp_chpl5);
#line 190 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  call_tmp_chpl7 = chpl_macro_double_isnan(coerce_tmp_chpl4);
#line 190 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  if (call_tmp_chpl6 | (call_tmp_chpl7 != INT64(0))) /* ZLINE: 190 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl */
#line 190 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  {
#line 190 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    tmp_chpl = coerce_tmp_chpl4;
#line 190 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  } else /* ZLINE: 190 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl */
#line 190 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  {
#line 190 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
    tmp_chpl = coerce_tmp_chpl5;
#line 190 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  }
#line 190 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 190 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  (this_chpl31)->value = tmp_chpl;
#line 189 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  return;
#line 189 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
}

#line 192 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
/* ChapelReduce.chpl:192 */
#line 192 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
static _real64 generate_chpl2(MaxReduceScanOp__real64_chpl this_chpl31,
#line 192 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
                              int64_t _ln_chpl,
#line 192 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
                              int32_t _fn_chpl) {
#line 192 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _real64 ret_chpl;
#line 192 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 192 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  ret_chpl = (this_chpl31)->value;
#line 192 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  return ret_chpl;
#line 192 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
}

#line 193 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
/* ChapelReduce.chpl:193 */
#line 193 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
static MaxReduceScanOp__real64_chpl clone_chpl3(MaxReduceScanOp__real64_chpl this_chpl31,
#line 193 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
                                                int64_t _ln_chpl,
#line 193 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
                                                int32_t _fn_chpl) {
#line 193 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  MaxReduceScanOp__real64_chpl new_temp_chpl = NULL;
#line 193 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_LocalSpinlock default_arg_l_chpl;
#line 193 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_LocalSpinlock ret_chpl;
#line 193 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  chpl_LocalSpinlock l_chpl;
#line 193 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  _real64 value_chpl;
#line 193 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  init_chpl105(&l_chpl);
#line 193 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  ret_chpl = l_chpl;
#line 193 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  default_arg_l_chpl = ret_chpl;
#line 193 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  value_chpl = min_chpl();
#line 193 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  new_temp_chpl = _new_chpl26(&default_arg_l_chpl, value_chpl, _ln_chpl, _fn_chpl);
#line 193 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
  return new_temp_chpl;
#line 193 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelReduce.chpl"
}

