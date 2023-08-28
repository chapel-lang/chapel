#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemConsistency.chpl"
/* MemConsistency.chpl:21 */
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemConsistency.chpl"
static void chpl__init_MemConsistency(int64_t _ln_chpl,
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemConsistency.chpl"
                                      int32_t _fn_chpl) {
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemConsistency.chpl"
  _ref_int32_t_chpl refIndentLevel_chpl = NULL;
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemConsistency.chpl"
  if (chpl__init_MemConsistency_p) /* ZLINE: 21 /Users/iainmoncrief/Documents/chapel/modules/internal/MemConsistency.chpl */
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemConsistency.chpl"
  {
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemConsistency.chpl"
    goto _exit_chpl__init_MemConsistency_chpl;
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemConsistency.chpl"
  }
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemConsistency.chpl"
  printModuleInit_chpl("%*s\n", "MemConsistency", INT64(14), _ln_chpl, _fn_chpl);
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemConsistency.chpl"
  refIndentLevel_chpl = &moduleInitLevel_chpl;
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemConsistency.chpl"
  *(refIndentLevel_chpl) += INT64(1);
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemConsistency.chpl"
  chpl__init_MemConsistency_p = UINT8(true);
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemConsistency.chpl"
  {
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemConsistency.chpl"
    chpl__init_HaltWrappers(_ln_chpl, _fn_chpl);
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemConsistency.chpl"
  }
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemConsistency.chpl"
  *(refIndentLevel_chpl) -= INT64(1);
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemConsistency.chpl"
  _exit_chpl__init_MemConsistency_chpl:;
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemConsistency.chpl"
  return;
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemConsistency.chpl"
}

#line 119 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemConsistency.chpl"
/* MemConsistency.chpl:119 */
#line 119 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemConsistency.chpl"
static void chpl_rmem_consist_maybe_release2(int64_t _ln_chpl,
#line 119 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemConsistency.chpl"
                                             int32_t _fn_chpl) {
#line 120 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemConsistency.chpl"
  memory_order ret_chpl;
#line 120 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemConsistency.chpl"
  ret_chpl = memory_order_seq_cst;
#line 120 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemConsistency.chpl"
  chpl_rmem_consist_maybe_release(ret_chpl, _ln_chpl, _fn_chpl);
#line 119 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemConsistency.chpl"
  return;
#line 119 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemConsistency.chpl"
}

#line 119 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemConsistency.chpl"
/* MemConsistency.chpl:119 */
#line 119 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemConsistency.chpl"
static void chpl_rmem_consist_maybe_release5(int64_t _ln_chpl,
#line 119 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemConsistency.chpl"
                                             int32_t _fn_chpl) {
#line 120 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemConsistency.chpl"
  memory_order ret_chpl;
#line 120 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemConsistency.chpl"
  ret_chpl = memory_order_acquire;
#line 120 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemConsistency.chpl"
  chpl_rmem_consist_maybe_release(ret_chpl, _ln_chpl, _fn_chpl);
#line 119 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemConsistency.chpl"
  return;
#line 119 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemConsistency.chpl"
}

#line 119 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemConsistency.chpl"
/* MemConsistency.chpl:119 */
#line 119 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemConsistency.chpl"
static void chpl_rmem_consist_maybe_release4(int64_t _ln_chpl,
#line 119 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemConsistency.chpl"
                                             int32_t _fn_chpl) {
#line 120 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemConsistency.chpl"
  memory_order ret_chpl;
#line 120 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemConsistency.chpl"
  ret_chpl = memory_order_release;
#line 120 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemConsistency.chpl"
  chpl_rmem_consist_maybe_release(ret_chpl, _ln_chpl, _fn_chpl);
#line 119 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemConsistency.chpl"
  return;
#line 119 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemConsistency.chpl"
}

#line 119 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemConsistency.chpl"
/* MemConsistency.chpl:119 */
#line 119 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemConsistency.chpl"
static void chpl_rmem_consist_maybe_release3(int64_t _ln_chpl,
#line 119 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemConsistency.chpl"
                                             int32_t _fn_chpl) {
#line 120 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemConsistency.chpl"
  memory_order ret_chpl;
#line 120 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemConsistency.chpl"
  ret_chpl = memory_order_relaxed;
#line 120 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemConsistency.chpl"
  chpl_rmem_consist_maybe_release(ret_chpl, _ln_chpl, _fn_chpl);
#line 119 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemConsistency.chpl"
  return;
#line 119 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemConsistency.chpl"
}

#line 126 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemConsistency.chpl"
/* MemConsistency.chpl:126 */
#line 126 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemConsistency.chpl"
static void chpl_rmem_consist_maybe_acquire5(int64_t _ln_chpl,
#line 126 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemConsistency.chpl"
                                             int32_t _fn_chpl) {
#line 127 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemConsistency.chpl"
  memory_order ret_chpl;
#line 127 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemConsistency.chpl"
  ret_chpl = memory_order_acquire;
#line 127 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemConsistency.chpl"
  chpl_rmem_consist_maybe_acquire(ret_chpl, _ln_chpl, _fn_chpl);
#line 126 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemConsistency.chpl"
  return;
#line 126 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemConsistency.chpl"
}

#line 126 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemConsistency.chpl"
/* MemConsistency.chpl:126 */
#line 126 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemConsistency.chpl"
static void chpl_rmem_consist_maybe_acquire4(int64_t _ln_chpl,
#line 126 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemConsistency.chpl"
                                             int32_t _fn_chpl) {
#line 127 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemConsistency.chpl"
  memory_order ret_chpl;
#line 127 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemConsistency.chpl"
  ret_chpl = memory_order_release;
#line 127 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemConsistency.chpl"
  chpl_rmem_consist_maybe_acquire(ret_chpl, _ln_chpl, _fn_chpl);
#line 126 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemConsistency.chpl"
  return;
#line 126 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemConsistency.chpl"
}

#line 126 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemConsistency.chpl"
/* MemConsistency.chpl:126 */
#line 126 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemConsistency.chpl"
static void chpl_rmem_consist_maybe_acquire3(int64_t _ln_chpl,
#line 126 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemConsistency.chpl"
                                             int32_t _fn_chpl) {
#line 127 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemConsistency.chpl"
  memory_order ret_chpl;
#line 127 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemConsistency.chpl"
  ret_chpl = memory_order_relaxed;
#line 127 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemConsistency.chpl"
  chpl_rmem_consist_maybe_acquire(ret_chpl, _ln_chpl, _fn_chpl);
#line 126 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemConsistency.chpl"
  return;
#line 126 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemConsistency.chpl"
}

#line 126 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemConsistency.chpl"
/* MemConsistency.chpl:126 */
#line 126 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemConsistency.chpl"
static void chpl_rmem_consist_maybe_acquire2(int64_t _ln_chpl,
#line 126 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemConsistency.chpl"
                                             int32_t _fn_chpl) {
#line 127 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemConsistency.chpl"
  memory_order ret_chpl;
#line 127 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemConsistency.chpl"
  ret_chpl = memory_order_seq_cst;
#line 127 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemConsistency.chpl"
  chpl_rmem_consist_maybe_acquire(ret_chpl, _ln_chpl, _fn_chpl);
#line 126 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemConsistency.chpl"
  return;
#line 126 "/Users/iainmoncrief/Documents/chapel/modules/internal/MemConsistency.chpl"
}

