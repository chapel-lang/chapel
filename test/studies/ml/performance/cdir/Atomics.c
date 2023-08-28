#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/Atomics.chpl"
/* Atomics.chpl:141 */
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/Atomics.chpl"
static void chpl__init_Atomics(int64_t _ln_chpl,
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/Atomics.chpl"
                               int32_t _fn_chpl) {
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/Atomics.chpl"
  _ref_int32_t_chpl refIndentLevel_chpl = NULL;
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/Atomics.chpl"
  if (chpl__init_Atomics_p) /* ZLINE: 141 /Users/iainmoncrief/Documents/chapel/modules/internal/Atomics.chpl */
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/Atomics.chpl"
  {
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/Atomics.chpl"
    goto _exit_chpl__init_Atomics_chpl;
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/Atomics.chpl"
  }
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/Atomics.chpl"
  printModuleInit_chpl("%*s\n", "Atomics", INT64(7), _ln_chpl, _fn_chpl);
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/Atomics.chpl"
  refIndentLevel_chpl = &moduleInitLevel_chpl;
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/Atomics.chpl"
  *(refIndentLevel_chpl) += INT64(1);
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/Atomics.chpl"
  chpl__init_Atomics_p = UINT8(true);
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/Atomics.chpl"
  {
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/Atomics.chpl"
    chpl__init_ChapelBase(_ln_chpl, _fn_chpl);
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/Atomics.chpl"
    chpl__init_MemConsistency(_ln_chpl, _fn_chpl);
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/Atomics.chpl"
    chpl__init_currentTask(_ln_chpl, _fn_chpl);
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/Atomics.chpl"
    chpl__init_String(_ln_chpl, _fn_chpl);
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/Atomics.chpl"
  }
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/Atomics.chpl"
  *(refIndentLevel_chpl) -= INT64(1);
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/Atomics.chpl"
  _exit_chpl__init_Atomics_chpl:;
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/Atomics.chpl"
  return;
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/Atomics.chpl"
}

#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/Atomics.chpl"
/* Atomics.chpl:151 */
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/Atomics.chpl"
static void atomic_fence_chpl(memory_order order_chpl,
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/Atomics.chpl"
                              int64_t _ln_chpl,
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/Atomics.chpl"
                              int32_t _fn_chpl) {
#line 152 "/Users/iainmoncrief/Documents/chapel/modules/internal/Atomics.chpl"
  chpl_atomic_thread_fence(order_chpl);
#line 154 "/Users/iainmoncrief/Documents/chapel/modules/internal/Atomics.chpl"
  chpl_rmem_consist_fence(order_chpl, _ln_chpl, _fn_chpl);
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/Atomics.chpl"
  return;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/Atomics.chpl"
}

#line 245 "/Users/iainmoncrief/Documents/chapel/modules/internal/Atomics.chpl"
/* Atomics.chpl:245 */
#line 245 "/Users/iainmoncrief/Documents/chapel/modules/internal/Atomics.chpl"
static void init_helper_chpl(AtomicBool_chpl * this_chpl31,
#line 245 "/Users/iainmoncrief/Documents/chapel/modules/internal/Atomics.chpl"
                             chpl_bool val_chpl) {
#line 252 "/Users/iainmoncrief/Documents/chapel/modules/internal/Atomics.chpl"
  _ref_atomic_bool_chpl call_tmp_chpl6 = NULL;
#line 252 "/Users/iainmoncrief/Documents/chapel/modules/internal/Atomics.chpl"
  call_tmp_chpl6 = &((this_chpl31)->_v);
#line 252 "/Users/iainmoncrief/Documents/chapel/modules/internal/Atomics.chpl"
  atomic_init_bool(call_tmp_chpl6, val_chpl);
#line 245 "/Users/iainmoncrief/Documents/chapel/modules/internal/Atomics.chpl"
  return;
#line 245 "/Users/iainmoncrief/Documents/chapel/modules/internal/Atomics.chpl"
}

#line 256 "/Users/iainmoncrief/Documents/chapel/modules/internal/Atomics.chpl"
/* Atomics.chpl:256 */
#line 256 "/Users/iainmoncrief/Documents/chapel/modules/internal/Atomics.chpl"
static void init_chpl17(AtomicBool_chpl * this_chpl31) {
#line 257 "/Users/iainmoncrief/Documents/chapel/modules/internal/Atomics.chpl"
  atomic_bool _v_chpl;
#line 257 "/Users/iainmoncrief/Documents/chapel/modules/internal/Atomics.chpl"
  memset(&_v_chpl, INT32(0), sizeof(atomic_bool));
#line 257 "/Users/iainmoncrief/Documents/chapel/modules/internal/Atomics.chpl"
  (this_chpl31)->_v = _v_chpl;
#line 259 "/Users/iainmoncrief/Documents/chapel/modules/internal/Atomics.chpl"
  init_helper_chpl(this_chpl31, UINT8(false));
#line 256 "/Users/iainmoncrief/Documents/chapel/modules/internal/Atomics.chpl"
  return;
#line 256 "/Users/iainmoncrief/Documents/chapel/modules/internal/Atomics.chpl"
}

#line 275 "/Users/iainmoncrief/Documents/chapel/modules/internal/Atomics.chpl"
/* Atomics.chpl:275 */
#line 275 "/Users/iainmoncrief/Documents/chapel/modules/internal/Atomics.chpl"
static void deinit_chpl(AtomicBool_chpl * this_chpl31) {
#line 281 "/Users/iainmoncrief/Documents/chapel/modules/internal/Atomics.chpl"
  _ref_atomic_bool_chpl call_tmp_chpl6 = NULL;
#line 281 "/Users/iainmoncrief/Documents/chapel/modules/internal/Atomics.chpl"
  call_tmp_chpl6 = &((this_chpl31)->_v);
#line 281 "/Users/iainmoncrief/Documents/chapel/modules/internal/Atomics.chpl"
  atomic_destroy_bool(call_tmp_chpl6);
#line 275 "/Users/iainmoncrief/Documents/chapel/modules/internal/Atomics.chpl"
  return;
#line 275 "/Users/iainmoncrief/Documents/chapel/modules/internal/Atomics.chpl"
}

#line 438 "/Users/iainmoncrief/Documents/chapel/modules/internal/Atomics.chpl"
/* Atomics.chpl:438 */
#line 438 "/Users/iainmoncrief/Documents/chapel/modules/internal/Atomics.chpl"
static void init_helper_chpl2(AtomicT_int64_t_chpl * this_chpl31,
#line 438 "/Users/iainmoncrief/Documents/chapel/modules/internal/Atomics.chpl"
                              int64_t val_chpl) {
#line 445 "/Users/iainmoncrief/Documents/chapel/modules/internal/Atomics.chpl"
  _ref_atomic_int_least64_t_chpl call_tmp_chpl6 = NULL;
#line 445 "/Users/iainmoncrief/Documents/chapel/modules/internal/Atomics.chpl"
  call_tmp_chpl6 = &((this_chpl31)->_v);
#line 445 "/Users/iainmoncrief/Documents/chapel/modules/internal/Atomics.chpl"
  atomic_init_int_least64_t(call_tmp_chpl6, val_chpl);
#line 438 "/Users/iainmoncrief/Documents/chapel/modules/internal/Atomics.chpl"
  return;
#line 438 "/Users/iainmoncrief/Documents/chapel/modules/internal/Atomics.chpl"
}

#line 449 "/Users/iainmoncrief/Documents/chapel/modules/internal/Atomics.chpl"
/* Atomics.chpl:449 */
#line 449 "/Users/iainmoncrief/Documents/chapel/modules/internal/Atomics.chpl"
static void init_chpl18(AtomicT_int64_t_chpl * this_chpl31) {
#line 451 "/Users/iainmoncrief/Documents/chapel/modules/internal/Atomics.chpl"
  atomic_int_least64_t _v_chpl;
#line 451 "/Users/iainmoncrief/Documents/chapel/modules/internal/Atomics.chpl"
  memset(&_v_chpl, INT32(0), sizeof(atomic_int_least64_t));
#line 451 "/Users/iainmoncrief/Documents/chapel/modules/internal/Atomics.chpl"
  (this_chpl31)->_v = _v_chpl;
#line 453 "/Users/iainmoncrief/Documents/chapel/modules/internal/Atomics.chpl"
  init_helper_chpl2(this_chpl31, INT64(0));
#line 449 "/Users/iainmoncrief/Documents/chapel/modules/internal/Atomics.chpl"
  return;
#line 449 "/Users/iainmoncrief/Documents/chapel/modules/internal/Atomics.chpl"
}

#line 471 "/Users/iainmoncrief/Documents/chapel/modules/internal/Atomics.chpl"
/* Atomics.chpl:471 */
#line 471 "/Users/iainmoncrief/Documents/chapel/modules/internal/Atomics.chpl"
static void deinit_chpl2(AtomicT_int64_t_chpl * this_chpl31) {
#line 477 "/Users/iainmoncrief/Documents/chapel/modules/internal/Atomics.chpl"
  _ref_atomic_int_least64_t_chpl call_tmp_chpl6 = NULL;
#line 477 "/Users/iainmoncrief/Documents/chapel/modules/internal/Atomics.chpl"
  call_tmp_chpl6 = &((this_chpl31)->_v);
#line 477 "/Users/iainmoncrief/Documents/chapel/modules/internal/Atomics.chpl"
  atomic_destroy_int_least64_t(call_tmp_chpl6);
#line 471 "/Users/iainmoncrief/Documents/chapel/modules/internal/Atomics.chpl"
  return;
#line 471 "/Users/iainmoncrief/Documents/chapel/modules/internal/Atomics.chpl"
}

