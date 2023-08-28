#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocks.chpl"
/* ChapelLocks.chpl:24 */
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocks.chpl"
static void chpl__init_ChapelLocks(int64_t _ln_chpl,
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocks.chpl"
                                   int32_t _fn_chpl) {
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocks.chpl"
  _ref_int32_t_chpl refIndentLevel_chpl = NULL;
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocks.chpl"
  if (chpl__init_ChapelLocks_p) /* ZLINE: 24 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocks.chpl */
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocks.chpl"
  {
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocks.chpl"
    goto _exit_chpl__init_ChapelLocks_chpl;
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocks.chpl"
  }
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocks.chpl"
  printModuleInit_chpl("%*s\n", "ChapelLocks", INT64(11), _ln_chpl, _fn_chpl);
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocks.chpl"
  refIndentLevel_chpl = &moduleInitLevel_chpl;
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocks.chpl"
  *(refIndentLevel_chpl) += INT64(1);
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocks.chpl"
  chpl__init_ChapelLocks_p = UINT8(true);
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocks.chpl"
  {
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocks.chpl"
    chpl__init_Atomics(_ln_chpl, _fn_chpl);
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocks.chpl"
    chpl__init_ChapelBase(_ln_chpl, _fn_chpl);
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocks.chpl"
    chpl__init_MemConsistency(_ln_chpl, _fn_chpl);
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocks.chpl"
    chpl__init_currentTask(_ln_chpl, _fn_chpl);
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocks.chpl"
  }
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocks.chpl"
  *(refIndentLevel_chpl) -= INT64(1);
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocks.chpl"
  _exit_chpl__init_ChapelLocks_chpl:;
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocks.chpl"
  return;
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocks.chpl"
}

#line 39 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocks.chpl"
/* ChapelLocks.chpl:39 */
#line 39 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocks.chpl"
static void init_chpl105(chpl_LocalSpinlock * this_chpl31) {
#line 39 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocks.chpl"
  AtomicBool_chpl l_chpl;
#line 39 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocks.chpl"
  init_chpl17(&l_chpl);
#line 39 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocks.chpl"
  (this_chpl31)->l = l_chpl;
#line 39 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocks.chpl"
  return;
#line 39 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelLocks.chpl"
}

