#line 43 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
/* ArrayViewRankChange.chpl:43 */
#line 43 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
static void chpl__init_ArrayViewRankChange(int64_t _ln_chpl,
#line 43 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                           int32_t _fn_chpl) {
#line 43 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _ref_int32_t_chpl refIndentLevel_chpl = NULL;
#line 43 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  if (chpl__init_ArrayViewRankChange_p) /* ZLINE: 43 /Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl */
#line 43 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  {
#line 43 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    goto _exit_chpl__init_ArrayViewRankChange_chpl;
#line 43 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  }
#line 43 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  printModuleInit_chpl("%*s\n", "ArrayViewRankChange", INT64(19), _ln_chpl, _fn_chpl);
#line 43 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  refIndentLevel_chpl = &moduleInitLevel_chpl;
#line 43 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(refIndentLevel_chpl) += INT64(1);
#line 43 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl__init_ArrayViewRankChange_p = UINT8(true);
#line 43 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(refIndentLevel_chpl) -= INT64(1);
#line 43 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _exit_chpl__init_ArrayViewRankChange_chpl:;
#line 43 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  return;
#line 43 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
}

#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
/* ArrayViewRankChange.chpl:51 */
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
static void init_chpl3(ArrayViewRankChangeDist_DefaultDist__tuple_4_star_chpl_bool__tuple_4_star_int64_t_chpl this_chpl31,
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                       int64_t _doms_containing_dist_chpl,
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                       chpl_LocalSpinlock * _domsLock_chpl,
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                       chpl_bool _free_when_no_doms_chpl,
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                       int64_t pid_chpl,
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                       int64_t downDistPid_chpl,
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                       DefaultDist_chpl downDistInst_chpl,
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                       _tuple_4_star_chpl_bool_chpl * collapsedDim_chpl,
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                       _tuple_4_star_int64_t_chpl * idx_chpl,
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                       int64_t _ln_chpl,
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                       int32_t _fn_chpl) {
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_LocalSpinlock _formal_tmp__domsLock_chpl;
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _tuple_4_star_chpl_bool_chpl _formal_tmp_collapsedDim_chpl;
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _tuple_4_star_int64_t_chpl _formal_tmp_idx_chpl;
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  BaseDist_chpl super_tmp_chpl = NULL;
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  _formal_tmp__domsLock_chpl = *(_domsLock_chpl);
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(_formal_tmp_collapsedDim_chpl + INT64(0)) = *(*(collapsedDim_chpl) + INT64(0));
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(_formal_tmp_collapsedDim_chpl + INT64(1)) = *(*(collapsedDim_chpl) + INT64(1));
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(_formal_tmp_collapsedDim_chpl + INT64(2)) = *(*(collapsedDim_chpl) + INT64(2));
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(_formal_tmp_collapsedDim_chpl + INT64(3)) = *(*(collapsedDim_chpl) + INT64(3));
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(_formal_tmp_idx_chpl + INT64(0)) = *(*(idx_chpl) + INT64(0));
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(_formal_tmp_idx_chpl + INT64(1)) = *(*(idx_chpl) + INT64(1));
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(_formal_tmp_idx_chpl + INT64(2)) = *(*(idx_chpl) + INT64(2));
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(_formal_tmp_idx_chpl + INT64(3)) = *(*(idx_chpl) + INT64(3));
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  super_tmp_chpl = &((this_chpl31)->super);
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ((RootClass_chpl)(&((super_tmp_chpl)->super)))->chpl__cid = chpl__cid_RootClass_chpl;
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ((RootClass_chpl)(((RootClass_chpl)(super_tmp_chpl))))->chpl__cid = chpl__cid_RootClass_chpl;
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  (super_tmp_chpl)->_doms_containing_dist = _doms_containing_dist_chpl;
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  (super_tmp_chpl)->_domsLock = _formal_tmp__domsLock_chpl;
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  (super_tmp_chpl)->_free_when_no_doms = _free_when_no_doms_chpl;
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  (super_tmp_chpl)->pid = pid_chpl;
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ((RootClass_chpl)(super_tmp_chpl))->chpl__cid = chpl__cid_BaseDist_chpl;
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ((RootClass_chpl)(((BaseDist_chpl)(this_chpl31))))->chpl__cid = chpl__cid_BaseDist_chpl;
#line 54 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 54 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  (this_chpl31)->downDistPid = downDistPid_chpl;
#line 55 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 55 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  (this_chpl31)->downDistInst = downDistInst_chpl;
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *((this_chpl31)->collapsedDim + INT64(0)) = *(_formal_tmp_collapsedDim_chpl + INT64(0));
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *((this_chpl31)->collapsedDim + INT64(1)) = *(_formal_tmp_collapsedDim_chpl + INT64(1));
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *((this_chpl31)->collapsedDim + INT64(2)) = *(_formal_tmp_collapsedDim_chpl + INT64(2));
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *((this_chpl31)->collapsedDim + INT64(3)) = *(_formal_tmp_collapsedDim_chpl + INT64(3));
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *((this_chpl31)->idx + INT64(0)) = *(_formal_tmp_idx_chpl + INT64(0));
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *((this_chpl31)->idx + INT64(1)) = *(_formal_tmp_idx_chpl + INT64(1));
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *((this_chpl31)->idx + INT64(2)) = *(_formal_tmp_idx_chpl + INT64(2));
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *((this_chpl31)->idx + INT64(3)) = *(_formal_tmp_idx_chpl + INT64(3));
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ((RootClass_chpl)(this_chpl31))->chpl__cid = chpl__cid_ArrayViewRankChangeDist_DefaultDist__tuple_4_star_chpl_bool__tuple_4_star_int64_t_chpl;
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  return;
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
}

#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
/* ArrayViewRankChange.chpl:51 */
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
static void init_chpl(ArrayViewRankChangeDist_DefaultDist__tuple_3_star_chpl_bool__tuple_3_star_int64_t_chpl this_chpl31,
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                      int64_t _doms_containing_dist_chpl,
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                      chpl_LocalSpinlock * _domsLock_chpl,
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                      chpl_bool _free_when_no_doms_chpl,
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                      int64_t pid_chpl,
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                      int64_t downDistPid_chpl,
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                      DefaultDist_chpl downDistInst_chpl,
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                      _tuple_3_star_chpl_bool_chpl * collapsedDim_chpl,
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                      _tuple_3_star_int64_t_chpl * idx_chpl,
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                      int64_t _ln_chpl,
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                      int32_t _fn_chpl) {
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_LocalSpinlock _formal_tmp__domsLock_chpl;
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _tuple_3_star_chpl_bool_chpl _formal_tmp_collapsedDim_chpl;
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _tuple_3_star_int64_t_chpl _formal_tmp_idx_chpl;
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  BaseDist_chpl super_tmp_chpl = NULL;
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  _formal_tmp__domsLock_chpl = *(_domsLock_chpl);
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(_formal_tmp_collapsedDim_chpl + INT64(0)) = *(*(collapsedDim_chpl) + INT64(0));
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(_formal_tmp_collapsedDim_chpl + INT64(1)) = *(*(collapsedDim_chpl) + INT64(1));
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(_formal_tmp_collapsedDim_chpl + INT64(2)) = *(*(collapsedDim_chpl) + INT64(2));
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(_formal_tmp_idx_chpl + INT64(0)) = *(*(idx_chpl) + INT64(0));
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(_formal_tmp_idx_chpl + INT64(1)) = *(*(idx_chpl) + INT64(1));
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(_formal_tmp_idx_chpl + INT64(2)) = *(*(idx_chpl) + INT64(2));
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  super_tmp_chpl = &((this_chpl31)->super);
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ((RootClass_chpl)(&((super_tmp_chpl)->super)))->chpl__cid = chpl__cid_RootClass_chpl;
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ((RootClass_chpl)(((RootClass_chpl)(super_tmp_chpl))))->chpl__cid = chpl__cid_RootClass_chpl;
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  (super_tmp_chpl)->_doms_containing_dist = _doms_containing_dist_chpl;
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  (super_tmp_chpl)->_domsLock = _formal_tmp__domsLock_chpl;
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  (super_tmp_chpl)->_free_when_no_doms = _free_when_no_doms_chpl;
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  (super_tmp_chpl)->pid = pid_chpl;
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ((RootClass_chpl)(super_tmp_chpl))->chpl__cid = chpl__cid_BaseDist_chpl;
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ((RootClass_chpl)(((BaseDist_chpl)(this_chpl31))))->chpl__cid = chpl__cid_BaseDist_chpl;
#line 54 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 54 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  (this_chpl31)->downDistPid = downDistPid_chpl;
#line 55 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 55 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  (this_chpl31)->downDistInst = downDistInst_chpl;
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *((this_chpl31)->collapsedDim + INT64(0)) = *(_formal_tmp_collapsedDim_chpl + INT64(0));
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *((this_chpl31)->collapsedDim + INT64(1)) = *(_formal_tmp_collapsedDim_chpl + INT64(1));
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *((this_chpl31)->collapsedDim + INT64(2)) = *(_formal_tmp_collapsedDim_chpl + INT64(2));
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *((this_chpl31)->idx + INT64(0)) = *(_formal_tmp_idx_chpl + INT64(0));
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *((this_chpl31)->idx + INT64(1)) = *(_formal_tmp_idx_chpl + INT64(1));
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *((this_chpl31)->idx + INT64(2)) = *(_formal_tmp_idx_chpl + INT64(2));
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ((RootClass_chpl)(this_chpl31))->chpl__cid = chpl__cid_ArrayViewRankChangeDist_DefaultDist__tuple_3_star_chpl_bool__tuple_3_star_int64_t_chpl;
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  return;
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
}

#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
/* ArrayViewRankChange.chpl:51 */
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
static void init_chpl2(ArrayViewRankChangeDist_DefaultDist__tuple_2_star_chpl_bool__tuple_2_star_int64_t_chpl this_chpl31,
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                       int64_t _doms_containing_dist_chpl,
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                       chpl_LocalSpinlock * _domsLock_chpl,
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                       chpl_bool _free_when_no_doms_chpl,
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                       int64_t pid_chpl,
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                       int64_t downDistPid_chpl,
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                       DefaultDist_chpl downDistInst_chpl,
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                       _tuple_2_star_chpl_bool_chpl * collapsedDim_chpl,
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                       _tuple_2_star_int64_t_chpl * idx_chpl,
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                       int64_t _ln_chpl,
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                       int32_t _fn_chpl) {
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_LocalSpinlock _formal_tmp__domsLock_chpl;
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _tuple_2_star_chpl_bool_chpl _formal_tmp_collapsedDim_chpl;
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _tuple_2_star_int64_t_chpl _formal_tmp_idx_chpl;
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  BaseDist_chpl super_tmp_chpl = NULL;
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  _formal_tmp__domsLock_chpl = *(_domsLock_chpl);
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(_formal_tmp_collapsedDim_chpl + INT64(0)) = *(*(collapsedDim_chpl) + INT64(0));
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(_formal_tmp_collapsedDim_chpl + INT64(1)) = *(*(collapsedDim_chpl) + INT64(1));
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(_formal_tmp_idx_chpl + INT64(0)) = *(*(idx_chpl) + INT64(0));
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(_formal_tmp_idx_chpl + INT64(1)) = *(*(idx_chpl) + INT64(1));
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  super_tmp_chpl = &((this_chpl31)->super);
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ((RootClass_chpl)(&((super_tmp_chpl)->super)))->chpl__cid = chpl__cid_RootClass_chpl;
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ((RootClass_chpl)(((RootClass_chpl)(super_tmp_chpl))))->chpl__cid = chpl__cid_RootClass_chpl;
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  (super_tmp_chpl)->_doms_containing_dist = _doms_containing_dist_chpl;
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  (super_tmp_chpl)->_domsLock = _formal_tmp__domsLock_chpl;
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  (super_tmp_chpl)->_free_when_no_doms = _free_when_no_doms_chpl;
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  (super_tmp_chpl)->pid = pid_chpl;
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ((RootClass_chpl)(super_tmp_chpl))->chpl__cid = chpl__cid_BaseDist_chpl;
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ((RootClass_chpl)(((BaseDist_chpl)(this_chpl31))))->chpl__cid = chpl__cid_BaseDist_chpl;
#line 54 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 54 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  (this_chpl31)->downDistPid = downDistPid_chpl;
#line 55 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 55 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  (this_chpl31)->downDistInst = downDistInst_chpl;
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *((this_chpl31)->collapsedDim + INT64(0)) = *(_formal_tmp_collapsedDim_chpl + INT64(0));
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *((this_chpl31)->collapsedDim + INT64(1)) = *(_formal_tmp_collapsedDim_chpl + INT64(1));
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *((this_chpl31)->idx + INT64(0)) = *(_formal_tmp_idx_chpl + INT64(0));
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *((this_chpl31)->idx + INT64(1)) = *(_formal_tmp_idx_chpl + INT64(1));
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ((RootClass_chpl)(this_chpl31))->chpl__cid = chpl__cid_ArrayViewRankChangeDist_DefaultDist__tuple_2_star_chpl_bool__tuple_2_star_int64_t_chpl;
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  return;
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
}

#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
/* ArrayViewRankChange.chpl:51 */
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
static void chpl__auto_destroy_ArrayViewRankChangeDist(ArrayViewRankChangeDist_DefaultDist__tuple_4_star_chpl_bool__tuple_4_star_int64_t_chpl this_chpl31,
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                       int64_t _ln_chpl,
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                       int32_t _fn_chpl) {
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  deinit_chpl30(((BaseDist_chpl)(this_chpl31)), _ln_chpl, _fn_chpl);
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  return;
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
}

#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
/* ArrayViewRankChange.chpl:51 */
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
static void chpl__auto_destroy_ArrayViewRankChangeDist2(ArrayViewRankChangeDist_DefaultDist__tuple_3_star_chpl_bool__tuple_3_star_int64_t_chpl this_chpl31,
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                        int64_t _ln_chpl,
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                        int32_t _fn_chpl) {
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  deinit_chpl30(((BaseDist_chpl)(this_chpl31)), _ln_chpl, _fn_chpl);
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  return;
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
}

#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
/* ArrayViewRankChange.chpl:51 */
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
static void chpl__auto_destroy_ArrayViewRankChangeDist3(ArrayViewRankChangeDist_DefaultDist__tuple_2_star_chpl_bool__tuple_2_star_int64_t_chpl this_chpl31,
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                        int64_t _ln_chpl,
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                        int32_t _fn_chpl) {
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  deinit_chpl30(((BaseDist_chpl)(this_chpl31)), _ln_chpl, _fn_chpl);
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  return;
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
}

#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
/* ArrayViewRankChange.chpl:51 */
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
static ArrayViewRankChangeDist_DefaultDist__tuple_4_star_chpl_bool__tuple_4_star_int64_t_chpl _new_chpl3(int64_t _doms_containing_dist_chpl,
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                                                                         chpl_LocalSpinlock * _domsLock_chpl,
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                                                                         chpl_bool _free_when_no_doms_chpl,
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                                                                         int64_t pid_chpl,
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                                                                         int64_t downDistPid_chpl,
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                                                                         DefaultDist_chpl downDistInst_chpl,
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                                                                         _tuple_4_star_chpl_bool_chpl * collapsedDim_chpl,
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                                                                         _tuple_4_star_int64_t_chpl * idx_chpl,
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                                                                         int64_t _ln_chpl,
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                                                                         int32_t _fn_chpl) {
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_LocalSpinlock _formal_tmp__domsLock_chpl;
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _tuple_4_star_chpl_bool_chpl _formal_tmp_collapsedDim_chpl;
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _tuple_4_star_int64_t_chpl _formal_tmp_idx_chpl;
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ArrayViewRankChangeDist_DefaultDist__tuple_4_star_chpl_bool__tuple_4_star_int64_t_chpl initTemp_chpl = NULL;
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  c_ptr_void_chpl cast_tmp_chpl = NULL;
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ArrayViewRankChangeDist_DefaultDist__tuple_4_star_chpl_bool__tuple_4_star_int64_t_chpl tmp_chpl = NULL;
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  _formal_tmp__domsLock_chpl = *(_domsLock_chpl);
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(_formal_tmp_collapsedDim_chpl + INT64(0)) = *(*(collapsedDim_chpl) + INT64(0));
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(_formal_tmp_collapsedDim_chpl + INT64(1)) = *(*(collapsedDim_chpl) + INT64(1));
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(_formal_tmp_collapsedDim_chpl + INT64(2)) = *(*(collapsedDim_chpl) + INT64(2));
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(_formal_tmp_collapsedDim_chpl + INT64(3)) = *(*(collapsedDim_chpl) + INT64(3));
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(_formal_tmp_idx_chpl + INT64(0)) = *(*(idx_chpl) + INT64(0));
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(_formal_tmp_idx_chpl + INT64(1)) = *(*(idx_chpl) + INT64(1));
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(_formal_tmp_idx_chpl + INT64(2)) = *(*(idx_chpl) + INT64(2));
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(_formal_tmp_idx_chpl + INT64(3)) = *(*(idx_chpl) + INT64(3));
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  cast_tmp_chpl = chpl_here_alloc(sizeof(chpl_ArrayViewRankChangeDist_DefaultDist__tuple_4_star_chpl_bool__tuple_4_star_int64_t_chpl_object), INT16(235), _ln_chpl, _fn_chpl);
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  initTemp_chpl = ((ArrayViewRankChangeDist_DefaultDist__tuple_4_star_chpl_bool__tuple_4_star_int64_t_chpl)(cast_tmp_chpl));
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ((RootClass_chpl)(initTemp_chpl))->chpl__cid = chpl__cid_ArrayViewRankChangeDist_DefaultDist__tuple_4_star_chpl_bool__tuple_4_star_int64_t_chpl;
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(initTemp_chpl, _ln_chpl, _fn_chpl);
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  init_chpl3(initTemp_chpl, _doms_containing_dist_chpl, &_formal_tmp__domsLock_chpl, _free_when_no_doms_chpl, pid_chpl, downDistPid_chpl, downDistInst_chpl, &_formal_tmp_collapsedDim_chpl, &_formal_tmp_idx_chpl, _ln_chpl, _fn_chpl);
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  tmp_chpl = initTemp_chpl;
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  return tmp_chpl;
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
}

#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
/* ArrayViewRankChange.chpl:51 */
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
static ArrayViewRankChangeDist_DefaultDist__tuple_3_star_chpl_bool__tuple_3_star_int64_t_chpl _new_chpl(int64_t _doms_containing_dist_chpl,
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                                                                        chpl_LocalSpinlock * _domsLock_chpl,
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                                                                        chpl_bool _free_when_no_doms_chpl,
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                                                                        int64_t pid_chpl,
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                                                                        int64_t downDistPid_chpl,
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                                                                        DefaultDist_chpl downDistInst_chpl,
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                                                                        _tuple_3_star_chpl_bool_chpl * collapsedDim_chpl,
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                                                                        _tuple_3_star_int64_t_chpl * idx_chpl,
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                                                                        int64_t _ln_chpl,
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                                                                        int32_t _fn_chpl) {
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_LocalSpinlock _formal_tmp__domsLock_chpl;
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _tuple_3_star_chpl_bool_chpl _formal_tmp_collapsedDim_chpl;
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _tuple_3_star_int64_t_chpl _formal_tmp_idx_chpl;
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ArrayViewRankChangeDist_DefaultDist__tuple_3_star_chpl_bool__tuple_3_star_int64_t_chpl initTemp_chpl = NULL;
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  c_ptr_void_chpl cast_tmp_chpl = NULL;
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ArrayViewRankChangeDist_DefaultDist__tuple_3_star_chpl_bool__tuple_3_star_int64_t_chpl tmp_chpl = NULL;
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  _formal_tmp__domsLock_chpl = *(_domsLock_chpl);
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(_formal_tmp_collapsedDim_chpl + INT64(0)) = *(*(collapsedDim_chpl) + INT64(0));
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(_formal_tmp_collapsedDim_chpl + INT64(1)) = *(*(collapsedDim_chpl) + INT64(1));
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(_formal_tmp_collapsedDim_chpl + INT64(2)) = *(*(collapsedDim_chpl) + INT64(2));
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(_formal_tmp_idx_chpl + INT64(0)) = *(*(idx_chpl) + INT64(0));
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(_formal_tmp_idx_chpl + INT64(1)) = *(*(idx_chpl) + INT64(1));
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(_formal_tmp_idx_chpl + INT64(2)) = *(*(idx_chpl) + INT64(2));
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  cast_tmp_chpl = chpl_here_alloc(sizeof(chpl_ArrayViewRankChangeDist_DefaultDist__tuple_3_star_chpl_bool__tuple_3_star_int64_t_chpl_object), INT16(255), _ln_chpl, _fn_chpl);
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  initTemp_chpl = ((ArrayViewRankChangeDist_DefaultDist__tuple_3_star_chpl_bool__tuple_3_star_int64_t_chpl)(cast_tmp_chpl));
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ((RootClass_chpl)(initTemp_chpl))->chpl__cid = chpl__cid_ArrayViewRankChangeDist_DefaultDist__tuple_3_star_chpl_bool__tuple_3_star_int64_t_chpl;
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(initTemp_chpl, _ln_chpl, _fn_chpl);
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  init_chpl(initTemp_chpl, _doms_containing_dist_chpl, &_formal_tmp__domsLock_chpl, _free_when_no_doms_chpl, pid_chpl, downDistPid_chpl, downDistInst_chpl, &_formal_tmp_collapsedDim_chpl, &_formal_tmp_idx_chpl, _ln_chpl, _fn_chpl);
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  tmp_chpl = initTemp_chpl;
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  return tmp_chpl;
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
}

#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
/* ArrayViewRankChange.chpl:51 */
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
static ArrayViewRankChangeDist_DefaultDist__tuple_2_star_chpl_bool__tuple_2_star_int64_t_chpl _new_chpl2(int64_t _doms_containing_dist_chpl,
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                                                                         chpl_LocalSpinlock * _domsLock_chpl,
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                                                                         chpl_bool _free_when_no_doms_chpl,
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                                                                         int64_t pid_chpl,
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                                                                         int64_t downDistPid_chpl,
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                                                                         DefaultDist_chpl downDistInst_chpl,
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                                                                         _tuple_2_star_chpl_bool_chpl * collapsedDim_chpl,
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                                                                         _tuple_2_star_int64_t_chpl * idx_chpl,
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                                                                         int64_t _ln_chpl,
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                                                                         int32_t _fn_chpl) {
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  chpl_LocalSpinlock _formal_tmp__domsLock_chpl;
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _tuple_2_star_chpl_bool_chpl _formal_tmp_collapsedDim_chpl;
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _tuple_2_star_int64_t_chpl _formal_tmp_idx_chpl;
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ArrayViewRankChangeDist_DefaultDist__tuple_2_star_chpl_bool__tuple_2_star_int64_t_chpl initTemp_chpl = NULL;
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  c_ptr_void_chpl cast_tmp_chpl = NULL;
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ArrayViewRankChangeDist_DefaultDist__tuple_2_star_chpl_bool__tuple_2_star_int64_t_chpl tmp_chpl = NULL;
#line 33 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelDistribution.chpl"
  _formal_tmp__domsLock_chpl = *(_domsLock_chpl);
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(_formal_tmp_collapsedDim_chpl + INT64(0)) = *(*(collapsedDim_chpl) + INT64(0));
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(_formal_tmp_collapsedDim_chpl + INT64(1)) = *(*(collapsedDim_chpl) + INT64(1));
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(_formal_tmp_idx_chpl + INT64(0)) = *(*(idx_chpl) + INT64(0));
#line 66 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(_formal_tmp_idx_chpl + INT64(1)) = *(*(idx_chpl) + INT64(1));
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  cast_tmp_chpl = chpl_here_alloc(sizeof(chpl_ArrayViewRankChangeDist_DefaultDist__tuple_2_star_chpl_bool__tuple_2_star_int64_t_chpl_object), INT16(296), _ln_chpl, _fn_chpl);
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  initTemp_chpl = ((ArrayViewRankChangeDist_DefaultDist__tuple_2_star_chpl_bool__tuple_2_star_int64_t_chpl)(cast_tmp_chpl));
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ((RootClass_chpl)(initTemp_chpl))->chpl__cid = chpl__cid_ArrayViewRankChangeDist_DefaultDist__tuple_2_star_chpl_bool__tuple_2_star_int64_t_chpl;
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(initTemp_chpl, _ln_chpl, _fn_chpl);
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  init_chpl2(initTemp_chpl, _doms_containing_dist_chpl, &_formal_tmp__domsLock_chpl, _free_when_no_doms_chpl, pid_chpl, downDistPid_chpl, downDistInst_chpl, &_formal_tmp_collapsedDim_chpl, &_formal_tmp_idx_chpl, _ln_chpl, _fn_chpl);
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  tmp_chpl = initTemp_chpl;
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  return tmp_chpl;
#line 51 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
}

#line 75 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
/* ArrayViewRankChange.chpl:75 */
#line 75 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
static ArrayViewRankChangeDom_3_int64_t_one__tuple_4_star_chpl_bool__tuple_4_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_4_star_chpl_bool__tuple_4_star_int64_t_chpl dsiNewRectangularDom_chpl3(ArrayViewRankChangeDist_DefaultDist__tuple_4_star_chpl_bool__tuple_4_star_int64_t_chpl this_chpl31,
#line 75 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                                                                                                                                                                                    _tuple_3_star_range_int64_t_both_one_chpl * inds_chpl,
#line 75 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                                                                                                                                                                                    int64_t _ln_chpl,
#line 75 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                                                                                                                                                                                    int32_t _fn_chpl) {
#line 81 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  BaseDist_chpl coerce_tmp_chpl3 = NULL;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ArrayViewRankChangeDom_3_int64_t_one__tuple_4_star_chpl_bool__tuple_4_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_4_star_chpl_bool__tuple_4_star_int64_t_chpl new_temp_chpl = NULL;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  DefaultRectangularDom_3_int64_t_one_chpl upDomInst_chpl = NULL;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _tuple_4_star_chpl_bool_chpl _formal_tmp_in_collapsedDim_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _tuple_4_star_int64_t_chpl _formal_tmp_in_idx_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ArrayViewRankChangeDom_3_int64_t_one__tuple_4_star_chpl_bool__tuple_4_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_4_star_chpl_bool__tuple_4_star_int64_t_chpl newdom_chpl = NULL;
#line 81 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  coerce_tmp_chpl3 = ((BaseDist_chpl)(this_chpl31));
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  upDomInst_chpl = ((DefaultRectangularDom_3_int64_t_one_chpl)(nil));
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(_formal_tmp_in_collapsedDim_chpl + INT64(0)) = *((this_chpl31)->collapsedDim + INT64(0));
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(_formal_tmp_in_collapsedDim_chpl + INT64(1)) = *((this_chpl31)->collapsedDim + INT64(1));
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(_formal_tmp_in_collapsedDim_chpl + INT64(2)) = *((this_chpl31)->collapsedDim + INT64(2));
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(_formal_tmp_in_collapsedDim_chpl + INT64(3)) = *((this_chpl31)->collapsedDim + INT64(3));
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(_formal_tmp_in_idx_chpl + INT64(0)) = *((this_chpl31)->idx + INT64(0));
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(_formal_tmp_in_idx_chpl + INT64(1)) = *((this_chpl31)->idx + INT64(1));
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(_formal_tmp_in_idx_chpl + INT64(2)) = *((this_chpl31)->idx + INT64(2));
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(_formal_tmp_in_idx_chpl + INT64(3)) = *((this_chpl31)->idx + INT64(3));
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  new_temp_chpl = _new_chpl5(upDomInst_chpl, &_formal_tmp_in_collapsedDim_chpl, &_formal_tmp_in_idx_chpl, (coerce_tmp_chpl3)->pid, this_chpl31, INT64(0), ((DefaultRectangularDom_4_int64_t_one_chpl)(nil)), _ln_chpl, _fn_chpl);
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  newdom_chpl = new_temp_chpl;
#line 83 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(new_temp_chpl, _ln_chpl, _fn_chpl);
#line 83 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  dsiSetIndices_chpl4(new_temp_chpl, inds_chpl, _ln_chpl, _fn_chpl);
#line 75 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  return newdom_chpl;
#line 75 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
}

#line 75 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
/* ArrayViewRankChange.chpl:75 */
#line 75 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
static ArrayViewRankChangeDom_2_int64_t_one__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_3_star_chpl_bool__tuple_3_star_int64_t_chpl dsiNewRectangularDom_chpl(ArrayViewRankChangeDist_DefaultDist__tuple_3_star_chpl_bool__tuple_3_star_int64_t_chpl this_chpl31,
#line 75 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                                                                                                                                                                                   _tuple_2_star_range_int64_t_both_one_chpl * inds_chpl,
#line 75 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                                                                                                                                                                                   int64_t _ln_chpl,
#line 75 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                                                                                                                                                                                   int32_t _fn_chpl) {
#line 81 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  BaseDist_chpl coerce_tmp_chpl3 = NULL;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ArrayViewRankChangeDom_2_int64_t_one__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_3_star_chpl_bool__tuple_3_star_int64_t_chpl new_temp_chpl = NULL;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  DefaultRectangularDom_2_int64_t_one_chpl upDomInst_chpl = NULL;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _tuple_3_star_chpl_bool_chpl _formal_tmp_in_collapsedDim_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _tuple_3_star_int64_t_chpl _formal_tmp_in_idx_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ArrayViewRankChangeDom_2_int64_t_one__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_3_star_chpl_bool__tuple_3_star_int64_t_chpl newdom_chpl = NULL;
#line 81 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  coerce_tmp_chpl3 = ((BaseDist_chpl)(this_chpl31));
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  upDomInst_chpl = ((DefaultRectangularDom_2_int64_t_one_chpl)(nil));
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(_formal_tmp_in_collapsedDim_chpl + INT64(0)) = *((this_chpl31)->collapsedDim + INT64(0));
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(_formal_tmp_in_collapsedDim_chpl + INT64(1)) = *((this_chpl31)->collapsedDim + INT64(1));
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(_formal_tmp_in_collapsedDim_chpl + INT64(2)) = *((this_chpl31)->collapsedDim + INT64(2));
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(_formal_tmp_in_idx_chpl + INT64(0)) = *((this_chpl31)->idx + INT64(0));
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(_formal_tmp_in_idx_chpl + INT64(1)) = *((this_chpl31)->idx + INT64(1));
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(_formal_tmp_in_idx_chpl + INT64(2)) = *((this_chpl31)->idx + INT64(2));
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  new_temp_chpl = _new_chpl4(upDomInst_chpl, &_formal_tmp_in_collapsedDim_chpl, &_formal_tmp_in_idx_chpl, (coerce_tmp_chpl3)->pid, this_chpl31, INT64(0), ((DefaultRectangularDom_3_int64_t_one_chpl)(nil)), _ln_chpl, _fn_chpl);
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  newdom_chpl = new_temp_chpl;
#line 83 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(new_temp_chpl, _ln_chpl, _fn_chpl);
#line 83 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  dsiSetIndices_chpl(new_temp_chpl, inds_chpl, _ln_chpl, _fn_chpl);
#line 75 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  return newdom_chpl;
#line 75 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
}

#line 75 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
/* ArrayViewRankChange.chpl:75 */
#line 75 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
static ArrayViewRankChangeDom_1_int64_t_one__tuple_2_star_chpl_bool__tuple_2_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_2_star_chpl_bool__tuple_2_star_int64_t_chpl dsiNewRectangularDom_chpl2(ArrayViewRankChangeDist_DefaultDist__tuple_2_star_chpl_bool__tuple_2_star_int64_t_chpl this_chpl31,
#line 75 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                                                                                                                                                                                    _tuple_1_star_range_int64_t_both_one_chpl * inds_chpl,
#line 75 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                                                                                                                                                                                    int64_t _ln_chpl,
#line 75 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                                                                                                                                                                                    int32_t _fn_chpl) {
#line 81 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  BaseDist_chpl coerce_tmp_chpl3 = NULL;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ArrayViewRankChangeDom_1_int64_t_one__tuple_2_star_chpl_bool__tuple_2_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_2_star_chpl_bool__tuple_2_star_int64_t_chpl new_temp_chpl = NULL;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  DefaultRectangularDom_1_int64_t_one_chpl upDomInst_chpl = NULL;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _tuple_2_star_chpl_bool_chpl _formal_tmp_in_collapsedDim_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _tuple_2_star_int64_t_chpl _formal_tmp_in_idx_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ArrayViewRankChangeDom_1_int64_t_one__tuple_2_star_chpl_bool__tuple_2_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_2_star_chpl_bool__tuple_2_star_int64_t_chpl newdom_chpl = NULL;
#line 81 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  coerce_tmp_chpl3 = ((BaseDist_chpl)(this_chpl31));
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  upDomInst_chpl = ((DefaultRectangularDom_1_int64_t_one_chpl)(nil));
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(_formal_tmp_in_collapsedDim_chpl + INT64(0)) = *((this_chpl31)->collapsedDim + INT64(0));
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(_formal_tmp_in_collapsedDim_chpl + INT64(1)) = *((this_chpl31)->collapsedDim + INT64(1));
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(_formal_tmp_in_idx_chpl + INT64(0)) = *((this_chpl31)->idx + INT64(0));
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(_formal_tmp_in_idx_chpl + INT64(1)) = *((this_chpl31)->idx + INT64(1));
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  new_temp_chpl = _new_chpl6(upDomInst_chpl, &_formal_tmp_in_collapsedDim_chpl, &_formal_tmp_in_idx_chpl, (coerce_tmp_chpl3)->pid, this_chpl31, INT64(0), ((DefaultRectangularDom_2_int64_t_one_chpl)(nil)), _ln_chpl, _fn_chpl);
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  newdom_chpl = new_temp_chpl;
#line 83 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(new_temp_chpl, _ln_chpl, _fn_chpl);
#line 83 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  dsiSetIndices_chpl2(new_temp_chpl, inds_chpl, _ln_chpl, _fn_chpl);
#line 75 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  return newdom_chpl;
#line 75 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
}

#line 75 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
/* ArrayViewRankChange.chpl:75 */
#line 75 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
static ArrayViewRankChangeDom_2_int64_t_one__tuple_4_star_chpl_bool__tuple_4_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_4_star_chpl_bool__tuple_4_star_int64_t_chpl dsiNewRectangularDom_chpl4(ArrayViewRankChangeDist_DefaultDist__tuple_4_star_chpl_bool__tuple_4_star_int64_t_chpl this_chpl31,
#line 75 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                                                                                                                                                                                    _tuple_2_star_range_int64_t_both_one_chpl * inds_chpl,
#line 75 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                                                                                                                                                                                    int64_t _ln_chpl,
#line 75 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                                                                                                                                                                                    int32_t _fn_chpl) {
#line 81 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  BaseDist_chpl coerce_tmp_chpl3 = NULL;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ArrayViewRankChangeDom_2_int64_t_one__tuple_4_star_chpl_bool__tuple_4_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_4_star_chpl_bool__tuple_4_star_int64_t_chpl new_temp_chpl = NULL;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  DefaultRectangularDom_2_int64_t_one_chpl upDomInst_chpl = NULL;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _tuple_4_star_chpl_bool_chpl _formal_tmp_in_collapsedDim_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _tuple_4_star_int64_t_chpl _formal_tmp_in_idx_chpl;
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ArrayViewRankChangeDom_2_int64_t_one__tuple_4_star_chpl_bool__tuple_4_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_4_star_chpl_bool__tuple_4_star_int64_t_chpl newdom_chpl = NULL;
#line 81 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  coerce_tmp_chpl3 = ((BaseDist_chpl)(this_chpl31));
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  upDomInst_chpl = ((DefaultRectangularDom_2_int64_t_one_chpl)(nil));
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(_formal_tmp_in_collapsedDim_chpl + INT64(0)) = *((this_chpl31)->collapsedDim + INT64(0));
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(_formal_tmp_in_collapsedDim_chpl + INT64(1)) = *((this_chpl31)->collapsedDim + INT64(1));
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(_formal_tmp_in_collapsedDim_chpl + INT64(2)) = *((this_chpl31)->collapsedDim + INT64(2));
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(_formal_tmp_in_collapsedDim_chpl + INT64(3)) = *((this_chpl31)->collapsedDim + INT64(3));
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(_formal_tmp_in_idx_chpl + INT64(0)) = *((this_chpl31)->idx + INT64(0));
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(_formal_tmp_in_idx_chpl + INT64(1)) = *((this_chpl31)->idx + INT64(1));
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(_formal_tmp_in_idx_chpl + INT64(2)) = *((this_chpl31)->idx + INT64(2));
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(_formal_tmp_in_idx_chpl + INT64(3)) = *((this_chpl31)->idx + INT64(3));
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  new_temp_chpl = _new_chpl7(upDomInst_chpl, &_formal_tmp_in_collapsedDim_chpl, &_formal_tmp_in_idx_chpl, (coerce_tmp_chpl3)->pid, this_chpl31, INT64(0), ((DefaultRectangularDom_4_int64_t_one_chpl)(nil)), _ln_chpl, _fn_chpl);
#line 76 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  newdom_chpl = new_temp_chpl;
#line 83 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(new_temp_chpl, _ln_chpl, _fn_chpl);
#line 83 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  dsiSetIndices_chpl3(new_temp_chpl, inds_chpl, _ln_chpl, _fn_chpl);
#line 75 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  return newdom_chpl;
#line 75 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
}

#line 110 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
/* ArrayViewRankChange.chpl:110 */
#line 110 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
static void dsiDestroyDist_chpl(ArrayViewRankChangeDist_DefaultDist__tuple_4_star_chpl_bool__tuple_4_star_int64_t_chpl this_chpl31) {
#line 110 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  return;
#line 110 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
}

#line 110 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
/* ArrayViewRankChange.chpl:110 */
#line 110 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
static void dsiDestroyDist_chpl2(ArrayViewRankChangeDist_DefaultDist__tuple_3_star_chpl_bool__tuple_3_star_int64_t_chpl this_chpl31) {
#line 110 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  return;
#line 110 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
}

#line 110 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
/* ArrayViewRankChange.chpl:110 */
#line 110 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
static void dsiDestroyDist_chpl3(ArrayViewRankChangeDist_DefaultDist__tuple_2_star_chpl_bool__tuple_2_star_int64_t_chpl this_chpl31) {
#line 110 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  return;
#line 110 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
}

#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
/* ArrayViewRankChange.chpl:141 */
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
static ArrayViewRankChangeDom_1_int64_t_one__tuple_2_star_chpl_bool__tuple_2_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_2_star_chpl_bool__tuple_2_star_int64_t_chpl _new_chpl6(DefaultRectangularDom_1_int64_t_one_chpl upDomInst_chpl,
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                                                                                                                                                                    _tuple_2_star_chpl_bool_chpl * collapsedDim_chpl,
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                                                                                                                                                                    _tuple_2_star_int64_t_chpl * idx_chpl,
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                                                                                                                                                                    int64_t distPid_chpl,
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                                                                                                                                                                    ArrayViewRankChangeDist_DefaultDist__tuple_2_star_chpl_bool__tuple_2_star_int64_t_chpl distInst_chpl,
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                                                                                                                                                                    int64_t downDomPid_chpl,
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                                                                                                                                                                    DefaultRectangularDom_2_int64_t_one_chpl downDomInst_chpl,
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                                                                                                                                                                    int64_t _ln_chpl,
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                                                                                                                                                                    int32_t _fn_chpl) {
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _tuple_2_star_chpl_bool_chpl _formal_tmp_collapsedDim_chpl;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _tuple_2_star_int64_t_chpl _formal_tmp_idx_chpl;
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ArrayViewRankChangeDom_1_int64_t_one__tuple_2_star_chpl_bool__tuple_2_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_2_star_chpl_bool__tuple_2_star_int64_t_chpl initTemp_chpl = NULL;
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  c_ptr_void_chpl cast_tmp_chpl = NULL;
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ArrayViewRankChangeDom_1_int64_t_one__tuple_2_star_chpl_bool__tuple_2_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_2_star_chpl_bool__tuple_2_star_int64_t_chpl tmp_chpl = NULL;
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(_formal_tmp_collapsedDim_chpl + INT64(0)) = *(*(collapsedDim_chpl) + INT64(0));
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(_formal_tmp_collapsedDim_chpl + INT64(1)) = *(*(collapsedDim_chpl) + INT64(1));
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(_formal_tmp_idx_chpl + INT64(0)) = *(*(idx_chpl) + INT64(0));
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(_formal_tmp_idx_chpl + INT64(1)) = *(*(idx_chpl) + INT64(1));
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  cast_tmp_chpl = chpl_here_alloc(sizeof(chpl_ArrayViewRankChangeDom_1_int64_t_one__tuple_2_star_chpl_bool__tuple_2_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_2_star_chpl_bool__tuple_2_star_int64_t_chpl_object), INT16(297), _ln_chpl, _fn_chpl);
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  initTemp_chpl = ((ArrayViewRankChangeDom_1_int64_t_one__tuple_2_star_chpl_bool__tuple_2_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_2_star_chpl_bool__tuple_2_star_int64_t_chpl)(cast_tmp_chpl));
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ((RootClass_chpl)(initTemp_chpl))->chpl__cid = chpl__cid_ArrayViewRankChangeDom_1_int64_t_one__tuple_2_star_chpl_bool__tuple_2_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_2_star_chpl_bool__tuple_2_star_int64_t_chpl;
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(initTemp_chpl, _ln_chpl, _fn_chpl);
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  init_chpl4(initTemp_chpl, upDomInst_chpl, &_formal_tmp_collapsedDim_chpl, &_formal_tmp_idx_chpl, distPid_chpl, distInst_chpl, downDomPid_chpl, downDomInst_chpl, _ln_chpl, _fn_chpl);
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  tmp_chpl = initTemp_chpl;
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  return tmp_chpl;
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
}

#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
/* ArrayViewRankChange.chpl:141 */
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
static void init_chpl6(ArrayViewRankChangeDom_3_int64_t_one__tuple_4_star_chpl_bool__tuple_4_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_4_star_chpl_bool__tuple_4_star_int64_t_chpl this_chpl31,
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                       DefaultRectangularDom_3_int64_t_one_chpl upDomInst_chpl,
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                       _tuple_4_star_chpl_bool_chpl * collapsedDim_chpl,
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                       _tuple_4_star_int64_t_chpl * idx_chpl,
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                       int64_t distPid_chpl,
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                       ArrayViewRankChangeDist_DefaultDist__tuple_4_star_chpl_bool__tuple_4_star_int64_t_chpl distInst_chpl,
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                       int64_t downDomPid_chpl,
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                       DefaultRectangularDom_4_int64_t_one_chpl downDomInst_chpl,
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                       int64_t _ln_chpl,
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                       int32_t _fn_chpl) {
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _tuple_4_star_chpl_bool_chpl _formal_tmp_collapsedDim_chpl;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _tuple_4_star_int64_t_chpl _formal_tmp_idx_chpl;
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  BaseRectangularDom_3_int64_t_one_chpl super_tmp_chpl = NULL;
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(_formal_tmp_collapsedDim_chpl + INT64(0)) = *(*(collapsedDim_chpl) + INT64(0));
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(_formal_tmp_collapsedDim_chpl + INT64(1)) = *(*(collapsedDim_chpl) + INT64(1));
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(_formal_tmp_collapsedDim_chpl + INT64(2)) = *(*(collapsedDim_chpl) + INT64(2));
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(_formal_tmp_collapsedDim_chpl + INT64(3)) = *(*(collapsedDim_chpl) + INT64(3));
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(_formal_tmp_idx_chpl + INT64(0)) = *(*(idx_chpl) + INT64(0));
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(_formal_tmp_idx_chpl + INT64(1)) = *(*(idx_chpl) + INT64(1));
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(_formal_tmp_idx_chpl + INT64(2)) = *(*(idx_chpl) + INT64(2));
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(_formal_tmp_idx_chpl + INT64(3)) = *(*(idx_chpl) + INT64(3));
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  super_tmp_chpl = &((this_chpl31)->super);
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  init_chpl51(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ((RootClass_chpl)(((BaseRectangularDom_3_int64_t_one_chpl)(this_chpl31))))->chpl__cid = chpl__cid_BaseRectangularDom_3_int64_t_one_chpl;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  (this_chpl31)->upDomInst = upDomInst_chpl;
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *((this_chpl31)->collapsedDim + INT64(0)) = *(_formal_tmp_collapsedDim_chpl + INT64(0));
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *((this_chpl31)->collapsedDim + INT64(1)) = *(_formal_tmp_collapsedDim_chpl + INT64(1));
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *((this_chpl31)->collapsedDim + INT64(2)) = *(_formal_tmp_collapsedDim_chpl + INT64(2));
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *((this_chpl31)->collapsedDim + INT64(3)) = *(_formal_tmp_collapsedDim_chpl + INT64(3));
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *((this_chpl31)->idx + INT64(0)) = *(_formal_tmp_idx_chpl + INT64(0));
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *((this_chpl31)->idx + INT64(1)) = *(_formal_tmp_idx_chpl + INT64(1));
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *((this_chpl31)->idx + INT64(2)) = *(_formal_tmp_idx_chpl + INT64(2));
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *((this_chpl31)->idx + INT64(3)) = *(_formal_tmp_idx_chpl + INT64(3));
#line 153 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 153 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  (this_chpl31)->distPid = distPid_chpl;
#line 154 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 154 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  (this_chpl31)->distInst = distInst_chpl;
#line 167 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 167 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  (this_chpl31)->downDomPid = downDomPid_chpl;
#line 168 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 168 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  (this_chpl31)->downDomInst = downDomInst_chpl;
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ((RootClass_chpl)(this_chpl31))->chpl__cid = chpl__cid_ArrayViewRankChangeDom_3_int64_t_one__tuple_4_star_chpl_bool__tuple_4_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_4_star_chpl_bool__tuple_4_star_int64_t_chpl;
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  return;
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
}

#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
/* ArrayViewRankChange.chpl:141 */
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
static void init_chpl5(ArrayViewRankChangeDom_2_int64_t_one__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_3_star_chpl_bool__tuple_3_star_int64_t_chpl this_chpl31,
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                       DefaultRectangularDom_2_int64_t_one_chpl upDomInst_chpl,
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                       _tuple_3_star_chpl_bool_chpl * collapsedDim_chpl,
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                       _tuple_3_star_int64_t_chpl * idx_chpl,
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                       int64_t distPid_chpl,
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                       ArrayViewRankChangeDist_DefaultDist__tuple_3_star_chpl_bool__tuple_3_star_int64_t_chpl distInst_chpl,
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                       int64_t downDomPid_chpl,
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                       DefaultRectangularDom_3_int64_t_one_chpl downDomInst_chpl,
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                       int64_t _ln_chpl,
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                       int32_t _fn_chpl) {
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _tuple_3_star_chpl_bool_chpl _formal_tmp_collapsedDim_chpl;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _tuple_3_star_int64_t_chpl _formal_tmp_idx_chpl;
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  BaseRectangularDom_2_int64_t_one_chpl super_tmp_chpl = NULL;
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(_formal_tmp_collapsedDim_chpl + INT64(0)) = *(*(collapsedDim_chpl) + INT64(0));
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(_formal_tmp_collapsedDim_chpl + INT64(1)) = *(*(collapsedDim_chpl) + INT64(1));
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(_formal_tmp_collapsedDim_chpl + INT64(2)) = *(*(collapsedDim_chpl) + INT64(2));
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(_formal_tmp_idx_chpl + INT64(0)) = *(*(idx_chpl) + INT64(0));
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(_formal_tmp_idx_chpl + INT64(1)) = *(*(idx_chpl) + INT64(1));
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(_formal_tmp_idx_chpl + INT64(2)) = *(*(idx_chpl) + INT64(2));
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  super_tmp_chpl = &((this_chpl31)->super);
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  init_chpl52(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ((RootClass_chpl)(((BaseRectangularDom_2_int64_t_one_chpl)(this_chpl31))))->chpl__cid = chpl__cid_BaseRectangularDom_2_int64_t_one_chpl;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  (this_chpl31)->upDomInst = upDomInst_chpl;
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *((this_chpl31)->collapsedDim + INT64(0)) = *(_formal_tmp_collapsedDim_chpl + INT64(0));
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *((this_chpl31)->collapsedDim + INT64(1)) = *(_formal_tmp_collapsedDim_chpl + INT64(1));
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *((this_chpl31)->collapsedDim + INT64(2)) = *(_formal_tmp_collapsedDim_chpl + INT64(2));
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *((this_chpl31)->idx + INT64(0)) = *(_formal_tmp_idx_chpl + INT64(0));
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *((this_chpl31)->idx + INT64(1)) = *(_formal_tmp_idx_chpl + INT64(1));
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *((this_chpl31)->idx + INT64(2)) = *(_formal_tmp_idx_chpl + INT64(2));
#line 153 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 153 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  (this_chpl31)->distPid = distPid_chpl;
#line 154 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 154 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  (this_chpl31)->distInst = distInst_chpl;
#line 167 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 167 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  (this_chpl31)->downDomPid = downDomPid_chpl;
#line 168 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 168 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  (this_chpl31)->downDomInst = downDomInst_chpl;
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ((RootClass_chpl)(this_chpl31))->chpl__cid = chpl__cid_ArrayViewRankChangeDom_2_int64_t_one__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_3_star_chpl_bool__tuple_3_star_int64_t_chpl;
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  return;
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
}

#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
/* ArrayViewRankChange.chpl:141 */
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
static void init_chpl4(ArrayViewRankChangeDom_1_int64_t_one__tuple_2_star_chpl_bool__tuple_2_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_2_star_chpl_bool__tuple_2_star_int64_t_chpl this_chpl31,
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                       DefaultRectangularDom_1_int64_t_one_chpl upDomInst_chpl,
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                       _tuple_2_star_chpl_bool_chpl * collapsedDim_chpl,
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                       _tuple_2_star_int64_t_chpl * idx_chpl,
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                       int64_t distPid_chpl,
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                       ArrayViewRankChangeDist_DefaultDist__tuple_2_star_chpl_bool__tuple_2_star_int64_t_chpl distInst_chpl,
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                       int64_t downDomPid_chpl,
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                       DefaultRectangularDom_2_int64_t_one_chpl downDomInst_chpl,
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                       int64_t _ln_chpl,
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                       int32_t _fn_chpl) {
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _tuple_2_star_chpl_bool_chpl _formal_tmp_collapsedDim_chpl;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _tuple_2_star_int64_t_chpl _formal_tmp_idx_chpl;
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  BaseRectangularDom_1_int64_t_one_chpl super_tmp_chpl = NULL;
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(_formal_tmp_collapsedDim_chpl + INT64(0)) = *(*(collapsedDim_chpl) + INT64(0));
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(_formal_tmp_collapsedDim_chpl + INT64(1)) = *(*(collapsedDim_chpl) + INT64(1));
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(_formal_tmp_idx_chpl + INT64(0)) = *(*(idx_chpl) + INT64(0));
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(_formal_tmp_idx_chpl + INT64(1)) = *(*(idx_chpl) + INT64(1));
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  super_tmp_chpl = &((this_chpl31)->super);
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  init_chpl49(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ((RootClass_chpl)(((BaseRectangularDom_1_int64_t_one_chpl)(this_chpl31))))->chpl__cid = chpl__cid_BaseRectangularDom_1_int64_t_one_chpl;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  (this_chpl31)->upDomInst = upDomInst_chpl;
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *((this_chpl31)->collapsedDim + INT64(0)) = *(_formal_tmp_collapsedDim_chpl + INT64(0));
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *((this_chpl31)->collapsedDim + INT64(1)) = *(_formal_tmp_collapsedDim_chpl + INT64(1));
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *((this_chpl31)->idx + INT64(0)) = *(_formal_tmp_idx_chpl + INT64(0));
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *((this_chpl31)->idx + INT64(1)) = *(_formal_tmp_idx_chpl + INT64(1));
#line 153 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 153 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  (this_chpl31)->distPid = distPid_chpl;
#line 154 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 154 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  (this_chpl31)->distInst = distInst_chpl;
#line 167 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 167 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  (this_chpl31)->downDomPid = downDomPid_chpl;
#line 168 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 168 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  (this_chpl31)->downDomInst = downDomInst_chpl;
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ((RootClass_chpl)(this_chpl31))->chpl__cid = chpl__cid_ArrayViewRankChangeDom_1_int64_t_one__tuple_2_star_chpl_bool__tuple_2_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_2_star_chpl_bool__tuple_2_star_int64_t_chpl;
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  return;
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
}

#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
/* ArrayViewRankChange.chpl:141 */
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
static void init_chpl7(ArrayViewRankChangeDom_2_int64_t_one__tuple_4_star_chpl_bool__tuple_4_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_4_star_chpl_bool__tuple_4_star_int64_t_chpl this_chpl31,
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                       DefaultRectangularDom_2_int64_t_one_chpl upDomInst_chpl,
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                       _tuple_4_star_chpl_bool_chpl * collapsedDim_chpl,
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                       _tuple_4_star_int64_t_chpl * idx_chpl,
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                       int64_t distPid_chpl,
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                       ArrayViewRankChangeDist_DefaultDist__tuple_4_star_chpl_bool__tuple_4_star_int64_t_chpl distInst_chpl,
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                       int64_t downDomPid_chpl,
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                       DefaultRectangularDom_4_int64_t_one_chpl downDomInst_chpl,
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                       int64_t _ln_chpl,
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                       int32_t _fn_chpl) {
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _tuple_4_star_chpl_bool_chpl _formal_tmp_collapsedDim_chpl;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _tuple_4_star_int64_t_chpl _formal_tmp_idx_chpl;
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  BaseRectangularDom_2_int64_t_one_chpl super_tmp_chpl = NULL;
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(_formal_tmp_collapsedDim_chpl + INT64(0)) = *(*(collapsedDim_chpl) + INT64(0));
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(_formal_tmp_collapsedDim_chpl + INT64(1)) = *(*(collapsedDim_chpl) + INT64(1));
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(_formal_tmp_collapsedDim_chpl + INT64(2)) = *(*(collapsedDim_chpl) + INT64(2));
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(_formal_tmp_collapsedDim_chpl + INT64(3)) = *(*(collapsedDim_chpl) + INT64(3));
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(_formal_tmp_idx_chpl + INT64(0)) = *(*(idx_chpl) + INT64(0));
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(_formal_tmp_idx_chpl + INT64(1)) = *(*(idx_chpl) + INT64(1));
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(_formal_tmp_idx_chpl + INT64(2)) = *(*(idx_chpl) + INT64(2));
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(_formal_tmp_idx_chpl + INT64(3)) = *(*(idx_chpl) + INT64(3));
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  super_tmp_chpl = &((this_chpl31)->super);
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  init_chpl52(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ((RootClass_chpl)(((BaseRectangularDom_2_int64_t_one_chpl)(this_chpl31))))->chpl__cid = chpl__cid_BaseRectangularDom_2_int64_t_one_chpl;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  (this_chpl31)->upDomInst = upDomInst_chpl;
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *((this_chpl31)->collapsedDim + INT64(0)) = *(_formal_tmp_collapsedDim_chpl + INT64(0));
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *((this_chpl31)->collapsedDim + INT64(1)) = *(_formal_tmp_collapsedDim_chpl + INT64(1));
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *((this_chpl31)->collapsedDim + INT64(2)) = *(_formal_tmp_collapsedDim_chpl + INT64(2));
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *((this_chpl31)->collapsedDim + INT64(3)) = *(_formal_tmp_collapsedDim_chpl + INT64(3));
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *((this_chpl31)->idx + INT64(0)) = *(_formal_tmp_idx_chpl + INT64(0));
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *((this_chpl31)->idx + INT64(1)) = *(_formal_tmp_idx_chpl + INT64(1));
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *((this_chpl31)->idx + INT64(2)) = *(_formal_tmp_idx_chpl + INT64(2));
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *((this_chpl31)->idx + INT64(3)) = *(_formal_tmp_idx_chpl + INT64(3));
#line 153 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 153 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  (this_chpl31)->distPid = distPid_chpl;
#line 154 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 154 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  (this_chpl31)->distInst = distInst_chpl;
#line 167 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 167 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  (this_chpl31)->downDomPid = downDomPid_chpl;
#line 168 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 168 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  (this_chpl31)->downDomInst = downDomInst_chpl;
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ((RootClass_chpl)(this_chpl31))->chpl__cid = chpl__cid_ArrayViewRankChangeDom_2_int64_t_one__tuple_4_star_chpl_bool__tuple_4_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_4_star_chpl_bool__tuple_4_star_int64_t_chpl;
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  return;
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
}

#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
/* ArrayViewRankChange.chpl:141 */
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
static void chpl__auto_destroy_ArrayViewRankChangeDom4(ArrayViewRankChangeDom_3_int64_t_one__tuple_4_star_chpl_bool__tuple_4_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_4_star_chpl_bool__tuple_4_star_int64_t_chpl this_chpl31,
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                       int64_t _ln_chpl,
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                       int32_t _fn_chpl) {
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  deinit_chpl35(((BaseRectangularDom_3_int64_t_one_chpl)(this_chpl31)), _ln_chpl, _fn_chpl);
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  return;
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
}

#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
/* ArrayViewRankChange.chpl:141 */
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
static void chpl__auto_destroy_ArrayViewRankChangeDom3(ArrayViewRankChangeDom_2_int64_t_one__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_3_star_chpl_bool__tuple_3_star_int64_t_chpl this_chpl31,
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                       int64_t _ln_chpl,
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                       int32_t _fn_chpl) {
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  deinit_chpl34(((BaseRectangularDom_2_int64_t_one_chpl)(this_chpl31)), _ln_chpl, _fn_chpl);
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  return;
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
}

#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
/* ArrayViewRankChange.chpl:141 */
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
static void chpl__auto_destroy_ArrayViewRankChangeDom(ArrayViewRankChangeDom_1_int64_t_one__tuple_2_star_chpl_bool__tuple_2_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_2_star_chpl_bool__tuple_2_star_int64_t_chpl this_chpl31,
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                      int64_t _ln_chpl,
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                      int32_t _fn_chpl) {
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  deinit_chpl33(((BaseRectangularDom_1_int64_t_one_chpl)(this_chpl31)), _ln_chpl, _fn_chpl);
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  return;
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
}

#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
/* ArrayViewRankChange.chpl:141 */
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
static void chpl__auto_destroy_ArrayViewRankChangeDom2(ArrayViewRankChangeDom_2_int64_t_one__tuple_4_star_chpl_bool__tuple_4_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_4_star_chpl_bool__tuple_4_star_int64_t_chpl this_chpl31,
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                       int64_t _ln_chpl,
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                       int32_t _fn_chpl) {
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  deinit_chpl34(((BaseRectangularDom_2_int64_t_one_chpl)(this_chpl31)), _ln_chpl, _fn_chpl);
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  return;
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
}

#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
/* ArrayViewRankChange.chpl:141 */
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
static ArrayViewRankChangeDom_3_int64_t_one__tuple_4_star_chpl_bool__tuple_4_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_4_star_chpl_bool__tuple_4_star_int64_t_chpl _new_chpl5(DefaultRectangularDom_3_int64_t_one_chpl upDomInst_chpl,
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                                                                                                                                                                    _tuple_4_star_chpl_bool_chpl * collapsedDim_chpl,
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                                                                                                                                                                    _tuple_4_star_int64_t_chpl * idx_chpl,
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                                                                                                                                                                    int64_t distPid_chpl,
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                                                                                                                                                                    ArrayViewRankChangeDist_DefaultDist__tuple_4_star_chpl_bool__tuple_4_star_int64_t_chpl distInst_chpl,
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                                                                                                                                                                    int64_t downDomPid_chpl,
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                                                                                                                                                                    DefaultRectangularDom_4_int64_t_one_chpl downDomInst_chpl,
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                                                                                                                                                                    int64_t _ln_chpl,
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                                                                                                                                                                    int32_t _fn_chpl) {
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _tuple_4_star_chpl_bool_chpl _formal_tmp_collapsedDim_chpl;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _tuple_4_star_int64_t_chpl _formal_tmp_idx_chpl;
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ArrayViewRankChangeDom_3_int64_t_one__tuple_4_star_chpl_bool__tuple_4_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_4_star_chpl_bool__tuple_4_star_int64_t_chpl initTemp_chpl = NULL;
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  c_ptr_void_chpl cast_tmp_chpl = NULL;
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ArrayViewRankChangeDom_3_int64_t_one__tuple_4_star_chpl_bool__tuple_4_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_4_star_chpl_bool__tuple_4_star_int64_t_chpl tmp_chpl = NULL;
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(_formal_tmp_collapsedDim_chpl + INT64(0)) = *(*(collapsedDim_chpl) + INT64(0));
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(_formal_tmp_collapsedDim_chpl + INT64(1)) = *(*(collapsedDim_chpl) + INT64(1));
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(_formal_tmp_collapsedDim_chpl + INT64(2)) = *(*(collapsedDim_chpl) + INT64(2));
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(_formal_tmp_collapsedDim_chpl + INT64(3)) = *(*(collapsedDim_chpl) + INT64(3));
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(_formal_tmp_idx_chpl + INT64(0)) = *(*(idx_chpl) + INT64(0));
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(_formal_tmp_idx_chpl + INT64(1)) = *(*(idx_chpl) + INT64(1));
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(_formal_tmp_idx_chpl + INT64(2)) = *(*(idx_chpl) + INT64(2));
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(_formal_tmp_idx_chpl + INT64(3)) = *(*(idx_chpl) + INT64(3));
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  cast_tmp_chpl = chpl_here_alloc(sizeof(chpl_ArrayViewRankChangeDom_3_int64_t_one__tuple_4_star_chpl_bool__tuple_4_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_4_star_chpl_bool__tuple_4_star_int64_t_chpl_object), INT16(236), _ln_chpl, _fn_chpl);
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  initTemp_chpl = ((ArrayViewRankChangeDom_3_int64_t_one__tuple_4_star_chpl_bool__tuple_4_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_4_star_chpl_bool__tuple_4_star_int64_t_chpl)(cast_tmp_chpl));
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ((RootClass_chpl)(initTemp_chpl))->chpl__cid = chpl__cid_ArrayViewRankChangeDom_3_int64_t_one__tuple_4_star_chpl_bool__tuple_4_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_4_star_chpl_bool__tuple_4_star_int64_t_chpl;
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(initTemp_chpl, _ln_chpl, _fn_chpl);
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  init_chpl6(initTemp_chpl, upDomInst_chpl, &_formal_tmp_collapsedDim_chpl, &_formal_tmp_idx_chpl, distPid_chpl, distInst_chpl, downDomPid_chpl, downDomInst_chpl, _ln_chpl, _fn_chpl);
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  tmp_chpl = initTemp_chpl;
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  return tmp_chpl;
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
}

#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
/* ArrayViewRankChange.chpl:141 */
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
static ArrayViewRankChangeDom_2_int64_t_one__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_3_star_chpl_bool__tuple_3_star_int64_t_chpl _new_chpl4(DefaultRectangularDom_2_int64_t_one_chpl upDomInst_chpl,
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                                                                                                                                                                    _tuple_3_star_chpl_bool_chpl * collapsedDim_chpl,
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                                                                                                                                                                    _tuple_3_star_int64_t_chpl * idx_chpl,
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                                                                                                                                                                    int64_t distPid_chpl,
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                                                                                                                                                                    ArrayViewRankChangeDist_DefaultDist__tuple_3_star_chpl_bool__tuple_3_star_int64_t_chpl distInst_chpl,
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                                                                                                                                                                    int64_t downDomPid_chpl,
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                                                                                                                                                                    DefaultRectangularDom_3_int64_t_one_chpl downDomInst_chpl,
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                                                                                                                                                                    int64_t _ln_chpl,
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                                                                                                                                                                    int32_t _fn_chpl) {
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _tuple_3_star_chpl_bool_chpl _formal_tmp_collapsedDim_chpl;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _tuple_3_star_int64_t_chpl _formal_tmp_idx_chpl;
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ArrayViewRankChangeDom_2_int64_t_one__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_3_star_chpl_bool__tuple_3_star_int64_t_chpl initTemp_chpl = NULL;
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  c_ptr_void_chpl cast_tmp_chpl = NULL;
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ArrayViewRankChangeDom_2_int64_t_one__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_3_star_chpl_bool__tuple_3_star_int64_t_chpl tmp_chpl = NULL;
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(_formal_tmp_collapsedDim_chpl + INT64(0)) = *(*(collapsedDim_chpl) + INT64(0));
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(_formal_tmp_collapsedDim_chpl + INT64(1)) = *(*(collapsedDim_chpl) + INT64(1));
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(_formal_tmp_collapsedDim_chpl + INT64(2)) = *(*(collapsedDim_chpl) + INT64(2));
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(_formal_tmp_idx_chpl + INT64(0)) = *(*(idx_chpl) + INT64(0));
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(_formal_tmp_idx_chpl + INT64(1)) = *(*(idx_chpl) + INT64(1));
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(_formal_tmp_idx_chpl + INT64(2)) = *(*(idx_chpl) + INT64(2));
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  cast_tmp_chpl = chpl_here_alloc(sizeof(chpl_ArrayViewRankChangeDom_2_int64_t_one__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_3_star_chpl_bool__tuple_3_star_int64_t_chpl_object), INT16(256), _ln_chpl, _fn_chpl);
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  initTemp_chpl = ((ArrayViewRankChangeDom_2_int64_t_one__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_3_star_chpl_bool__tuple_3_star_int64_t_chpl)(cast_tmp_chpl));
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ((RootClass_chpl)(initTemp_chpl))->chpl__cid = chpl__cid_ArrayViewRankChangeDom_2_int64_t_one__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_3_star_chpl_bool__tuple_3_star_int64_t_chpl;
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(initTemp_chpl, _ln_chpl, _fn_chpl);
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  init_chpl5(initTemp_chpl, upDomInst_chpl, &_formal_tmp_collapsedDim_chpl, &_formal_tmp_idx_chpl, distPid_chpl, distInst_chpl, downDomPid_chpl, downDomInst_chpl, _ln_chpl, _fn_chpl);
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  tmp_chpl = initTemp_chpl;
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  return tmp_chpl;
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
}

#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
/* ArrayViewRankChange.chpl:141 */
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
static ArrayViewRankChangeDom_2_int64_t_one__tuple_4_star_chpl_bool__tuple_4_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_4_star_chpl_bool__tuple_4_star_int64_t_chpl _new_chpl7(DefaultRectangularDom_2_int64_t_one_chpl upDomInst_chpl,
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                                                                                                                                                                    _tuple_4_star_chpl_bool_chpl * collapsedDim_chpl,
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                                                                                                                                                                    _tuple_4_star_int64_t_chpl * idx_chpl,
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                                                                                                                                                                    int64_t distPid_chpl,
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                                                                                                                                                                    ArrayViewRankChangeDist_DefaultDist__tuple_4_star_chpl_bool__tuple_4_star_int64_t_chpl distInst_chpl,
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                                                                                                                                                                    int64_t downDomPid_chpl,
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                                                                                                                                                                    DefaultRectangularDom_4_int64_t_one_chpl downDomInst_chpl,
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                                                                                                                                                                    int64_t _ln_chpl,
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                                                                                                                                                                    int32_t _fn_chpl) {
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _tuple_4_star_chpl_bool_chpl _formal_tmp_collapsedDim_chpl;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _tuple_4_star_int64_t_chpl _formal_tmp_idx_chpl;
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ArrayViewRankChangeDom_2_int64_t_one__tuple_4_star_chpl_bool__tuple_4_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_4_star_chpl_bool__tuple_4_star_int64_t_chpl initTemp_chpl = NULL;
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  c_ptr_void_chpl cast_tmp_chpl = NULL;
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ArrayViewRankChangeDom_2_int64_t_one__tuple_4_star_chpl_bool__tuple_4_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_4_star_chpl_bool__tuple_4_star_int64_t_chpl tmp_chpl = NULL;
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(_formal_tmp_collapsedDim_chpl + INT64(0)) = *(*(collapsedDim_chpl) + INT64(0));
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(_formal_tmp_collapsedDim_chpl + INT64(1)) = *(*(collapsedDim_chpl) + INT64(1));
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(_formal_tmp_collapsedDim_chpl + INT64(2)) = *(*(collapsedDim_chpl) + INT64(2));
#line 150 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(_formal_tmp_collapsedDim_chpl + INT64(3)) = *(*(collapsedDim_chpl) + INT64(3));
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(_formal_tmp_idx_chpl + INT64(0)) = *(*(idx_chpl) + INT64(0));
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(_formal_tmp_idx_chpl + INT64(1)) = *(*(idx_chpl) + INT64(1));
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(_formal_tmp_idx_chpl + INT64(2)) = *(*(idx_chpl) + INT64(2));
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(_formal_tmp_idx_chpl + INT64(3)) = *(*(idx_chpl) + INT64(3));
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  cast_tmp_chpl = chpl_here_alloc(sizeof(chpl_ArrayViewRankChangeDom_2_int64_t_one__tuple_4_star_chpl_bool__tuple_4_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_4_star_chpl_bool__tuple_4_star_int64_t_chpl_object), INT16(366), _ln_chpl, _fn_chpl);
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  initTemp_chpl = ((ArrayViewRankChangeDom_2_int64_t_one__tuple_4_star_chpl_bool__tuple_4_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_4_star_chpl_bool__tuple_4_star_int64_t_chpl)(cast_tmp_chpl));
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ((RootClass_chpl)(initTemp_chpl))->chpl__cid = chpl__cid_ArrayViewRankChangeDom_2_int64_t_one__tuple_4_star_chpl_bool__tuple_4_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_4_star_chpl_bool__tuple_4_star_int64_t_chpl;
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(initTemp_chpl, _ln_chpl, _fn_chpl);
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  init_chpl7(initTemp_chpl, upDomInst_chpl, &_formal_tmp_collapsedDim_chpl, &_formal_tmp_idx_chpl, distPid_chpl, distInst_chpl, downDomPid_chpl, downDomInst_chpl, _ln_chpl, _fn_chpl);
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  tmp_chpl = initTemp_chpl;
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  return tmp_chpl;
#line 141 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
}

#line 185 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
/* ArrayViewRankChange.chpl:185 */
#line 185 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
static ArrayViewRankChangeArr__real64_int64_t_ArrayViewRankChangeDom_1_int64_t_one__tuple_2_star_chpl_bool__tuple_2_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_2_star_chpl_bool__tuple_2_star_int64_t_int64_t_DefaultRectangularArr_2_int64_t_one__real64_int64_t__tuple_2_star_chpl_bool__tuple_2_star_int64_t__remoteAccessData__real64_1_int64_t_one_T_T_chpl dsiBuildArray_chpl(ArrayViewRankChangeDom_1_int64_t_one__tuple_2_star_chpl_bool__tuple_2_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_2_star_chpl_bool__tuple_2_star_int64_t_chpl this_chpl31,
#line 185 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                                                                                                                                                                                                                                                                                                                                                                         int64_t _ln_chpl,
#line 185 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                                                                                                                                                                                                                                                                                                                                                                         int32_t _fn_chpl) {
#line 187 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  DefaultRectangularDom_2_int64_t_one_chpl coerce_tmp_chpl3 = NULL;
#line 187 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  DefaultRectangularArr_2_int64_t_one__real64_int64_t_chpl call_tmp_chpl6 = NULL;
#line 187 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _array_DefaultRectangularArr_2_int64_t_one__real64_int64_t_chpl call_tmp_chpl7;
#line 187 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _array_DefaultRectangularArr_2_int64_t_one__real64_int64_t_chpl ret_tmp_chpl;
#line 187 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _array_DefaultRectangularArr_2_int64_t_one__real64_int64_t_chpl downarr_chpl;
#line 189 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  BaseDom_chpl coerce_tmp_chpl4 = NULL;
#line 190 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ArrayViewRankChangeDom_1_int64_t_one__tuple_2_star_chpl_bool__tuple_2_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_2_star_chpl_bool__tuple_2_star_int64_t_chpl call_tmp_chpl8 = NULL;
#line 193 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _ref__tuple_2_star_chpl_bool_chpl call_tmp_chpl9 = NULL;
#line 194 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _ref__tuple_2_star_int64_t_chpl call_tmp_chpl10 = NULL;
#line 188 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ArrayViewRankChangeArr__real64_int64_t_ArrayViewRankChangeDom_1_int64_t_one__tuple_2_star_chpl_bool__tuple_2_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_2_star_chpl_bool__tuple_2_star_int64_t_int64_t_DefaultRectangularArr_2_int64_t_one__real64_int64_t__tuple_2_star_chpl_bool__tuple_2_star_int64_t__remoteAccessData__real64_1_int64_t_one_T_T_chpl new_temp_chpl = NULL;
#line 187 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 187 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  if (((RootClass_chpl)((this_chpl31)->downDomInst)) == nil) /* ZLINE: 187 /Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl */
#line 187 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  {
#line 187 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    nilCheckHalt_chpl(&_str_literal_52_chpl, _ln_chpl, _fn_chpl);
#line 187 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  }
#line 187 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 187 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  coerce_tmp_chpl3 = (this_chpl31)->downDomInst;
#line 187 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  if (((RootClass_chpl)(coerce_tmp_chpl3)) == nil) /* ZLINE: 187 /Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl */
#line 187 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  {
#line 187 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    nilCheckHalt_chpl(&_str_literal_52_chpl, _ln_chpl, _fn_chpl);
#line 187 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  }
#line 187 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 187 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  if (((RootClass_chpl)((this_chpl31)->downDomInst)) == nil) /* ZLINE: 187 /Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl */
#line 187 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  {
#line 187 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    nilCheckHalt_chpl(&_str_literal_52_chpl, _ln_chpl, _fn_chpl);
#line 187 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  }
#line 187 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 187 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  call_tmp_chpl6 = dsiBuildArray_chpl10(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 187 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _newArray_chpl5(call_tmp_chpl6, &ret_tmp_chpl);
#line 187 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  call_tmp_chpl7 = ret_tmp_chpl;
#line 187 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  downarr_chpl = call_tmp_chpl7;
#line 189 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  coerce_tmp_chpl4 = ((BaseDom_chpl)(this_chpl31));
#line 190 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  call_tmp_chpl8 = this_chpl31;
#line 193 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 193 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  call_tmp_chpl9 = &((this_chpl31)->collapsedDim);
#line 194 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 194 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  call_tmp_chpl10 = &((this_chpl31)->idx);
#line 189 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(coerce_tmp_chpl4, _ln_chpl, _fn_chpl);
#line 188 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  new_temp_chpl = _new_chpl10((coerce_tmp_chpl4)->pid, call_tmp_chpl8, (&downarr_chpl)->_pid, (&downarr_chpl)->_instance, call_tmp_chpl9, call_tmp_chpl10, _ln_chpl, _fn_chpl);
#line 185 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  return new_temp_chpl;
#line 185 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
}

#line 185 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
/* ArrayViewRankChange.chpl:185 */
#line 185 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
static ArrayViewRankChangeArr__real64_int64_t_ArrayViewRankChangeDom_2_int64_t_one__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_DefaultRectangularArr_3_int64_t_one__real64_int64_t__tuple_3_star_chpl_bool__tuple_3_star_int64_t__remoteAccessData__real64_2_int64_t_one_T_T_chpl dsiBuildArray_chpl2(ArrayViewRankChangeDom_2_int64_t_one__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_3_star_chpl_bool__tuple_3_star_int64_t_chpl this_chpl31,
#line 185 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                                                                                                                                                                                                                                                                                                                                                                          int64_t _ln_chpl,
#line 185 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                                                                                                                                                                                                                                                                                                                                                                          int32_t _fn_chpl) {
#line 187 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  DefaultRectangularDom_3_int64_t_one_chpl coerce_tmp_chpl3 = NULL;
#line 187 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  DefaultRectangularArr_3_int64_t_one__real64_int64_t_chpl call_tmp_chpl6 = NULL;
#line 187 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _array_DefaultRectangularArr_3_int64_t_one__real64_int64_t_chpl call_tmp_chpl7;
#line 187 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _array_DefaultRectangularArr_3_int64_t_one__real64_int64_t_chpl ret_tmp_chpl;
#line 187 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _array_DefaultRectangularArr_3_int64_t_one__real64_int64_t_chpl downarr_chpl;
#line 189 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  BaseDom_chpl coerce_tmp_chpl4 = NULL;
#line 190 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ArrayViewRankChangeDom_2_int64_t_one__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_3_star_chpl_bool__tuple_3_star_int64_t_chpl call_tmp_chpl8 = NULL;
#line 193 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _ref__tuple_3_star_chpl_bool_chpl call_tmp_chpl9 = NULL;
#line 194 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _ref__tuple_3_star_int64_t_chpl call_tmp_chpl10 = NULL;
#line 188 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ArrayViewRankChangeArr__real64_int64_t_ArrayViewRankChangeDom_2_int64_t_one__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_DefaultRectangularArr_3_int64_t_one__real64_int64_t__tuple_3_star_chpl_bool__tuple_3_star_int64_t__remoteAccessData__real64_2_int64_t_one_T_T_chpl new_temp_chpl = NULL;
#line 187 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 187 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  if (((RootClass_chpl)((this_chpl31)->downDomInst)) == nil) /* ZLINE: 187 /Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl */
#line 187 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  {
#line 187 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    nilCheckHalt_chpl(&_str_literal_52_chpl, _ln_chpl, _fn_chpl);
#line 187 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  }
#line 187 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 187 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  coerce_tmp_chpl3 = (this_chpl31)->downDomInst;
#line 187 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  if (((RootClass_chpl)(coerce_tmp_chpl3)) == nil) /* ZLINE: 187 /Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl */
#line 187 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  {
#line 187 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    nilCheckHalt_chpl(&_str_literal_52_chpl, _ln_chpl, _fn_chpl);
#line 187 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  }
#line 187 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 187 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  if (((RootClass_chpl)((this_chpl31)->downDomInst)) == nil) /* ZLINE: 187 /Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl */
#line 187 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  {
#line 187 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    nilCheckHalt_chpl(&_str_literal_52_chpl, _ln_chpl, _fn_chpl);
#line 187 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  }
#line 187 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 187 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  call_tmp_chpl6 = dsiBuildArray_chpl7(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 187 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _newArray_chpl18(call_tmp_chpl6, &ret_tmp_chpl);
#line 187 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  call_tmp_chpl7 = ret_tmp_chpl;
#line 187 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  downarr_chpl = call_tmp_chpl7;
#line 189 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  coerce_tmp_chpl4 = ((BaseDom_chpl)(this_chpl31));
#line 190 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  call_tmp_chpl8 = this_chpl31;
#line 193 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 193 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  call_tmp_chpl9 = &((this_chpl31)->collapsedDim);
#line 194 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 194 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  call_tmp_chpl10 = &((this_chpl31)->idx);
#line 189 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(coerce_tmp_chpl4, _ln_chpl, _fn_chpl);
#line 188 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  new_temp_chpl = _new_chpl9((coerce_tmp_chpl4)->pid, call_tmp_chpl8, (&downarr_chpl)->_pid, (&downarr_chpl)->_instance, call_tmp_chpl9, call_tmp_chpl10, _ln_chpl, _fn_chpl);
#line 185 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  return new_temp_chpl;
#line 185 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
}

#line 198 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
/* ArrayViewRankChange.chpl:198 */
#line 198 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
static void dsiSetIndices_chpl4(ArrayViewRankChangeDom_3_int64_t_one__tuple_4_star_chpl_bool__tuple_4_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_4_star_chpl_bool__tuple_4_star_int64_t_chpl this_chpl31,
#line 198 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                _tuple_3_star_range_int64_t_both_one_chpl * inds_chpl,
#line 198 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                int64_t _ln_chpl,
#line 198 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                int32_t _fn_chpl) {
#line 203 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _domain_DefaultRectangularDom_3_int64_t_one_chpl call_tmp_chpl6;
#line 203 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl3;
#line 203 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl4;
#line 203 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl5;
#line 203 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _domain_DefaultRectangularDom_3_int64_t_one_chpl ret_tmp_chpl;
#line 203 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _domain_DefaultRectangularDom_3_int64_t_one_chpl upDomRec_chpl;
#line 203 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _ref_DefaultRectangularDom_3_int64_t_one_chpl _instance_chpl = NULL;
#line 204 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  DefaultRectangularDom_3_int64_t_one_chpl ret_chpl = NULL;
#line 209 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _tuple_4_star_range_int64_t_both_one_chpl ret_tmp_chpl2;
#line 209 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl tup_x0_chpl;
#line 209 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl tup_x1_chpl;
#line 209 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl tup_x2_chpl;
#line 209 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl tup_x3_chpl;
#line 209 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl elt_x0_chpl;
#line 209 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl elt_x1_chpl;
#line 209 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl elt_x2_chpl;
#line 209 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl elt_x3_chpl;
#line 210 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ArrayViewRankChangeDist_DefaultDist__tuple_4_star_chpl_bool__tuple_4_star_int64_t_chpl coerce_tmp_chpl6 = NULL;
#line 210 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  DefaultRectangularDom_4_int64_t_one_chpl call_tmp_chpl7 = NULL;
#line 210 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _tuple_4_star_range_int64_t_both_one_chpl coerce_tmp_chpl7;
#line 213 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _domain_DefaultRectangularDom_4_int64_t_one_chpl call_tmp_chpl8;
#line 213 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _domain_DefaultRectangularDom_4_int64_t_one_chpl initTemp_chpl;
#line 213 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _domain_DefaultRectangularDom_4_int64_t_one_chpl downDomLoc_chpl;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _ref__tuple_4_star_chpl_bool_chpl call_tmp_chpl9 = NULL;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _ref__tuple_4_star_int64_t_chpl call_tmp_chpl10 = NULL;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _domain_DefaultRectangularDom_4_int64_t_one_chpl call_tmp_chpl11;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl ranges_x0_chpl;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl ranges_x1_chpl;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl ranges_x2_chpl;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl ranges_x3_chpl;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl tup_x0_chpl2;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl tup_x1_chpl2;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl tup_x2_chpl2;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl tup_x3_chpl2;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl elt_x0_chpl2;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl elt_x1_chpl2;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl elt_x2_chpl2;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl elt_x3_chpl2;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  int64_t j_chpl;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl12 = NULL;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl8;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _ref_int64_t_chpl i_lhs_chpl = NULL;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl13;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl3;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl14 = NULL;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl9;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _ref_int64_t_chpl i_lhs_chpl2 = NULL;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl15;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl4;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl16 = NULL;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl10;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _ref_int64_t_chpl i_lhs_chpl3 = NULL;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl17;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl5;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl18 = NULL;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl11;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _ref_int64_t_chpl i_lhs_chpl4 = NULL;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl19;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl6;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _domain_DefaultRectangularDom_4_int64_t_one_chpl call_tmp_chpl20;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _domain_DefaultRectangularDom_4_int64_t_one_chpl ret_tmp_chpl7;
#line 215 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  BaseDom_chpl coerce_tmp_chpl12 = NULL;
#line 216 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  int64_t coerce_tmp_chpl13;
#line 217 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  DefaultRectangularDom_4_int64_t_one_chpl coerce_tmp_chpl14 = NULL;
#line 200 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 200 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  dsiDestroyDom_chpl4(this_chpl31, _ln_chpl, _fn_chpl);
#line 203 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  coerce_tmp_chpl3 = *(*(inds_chpl) + INT64(0));
#line 203 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  coerce_tmp_chpl4 = *(*(inds_chpl) + INT64(1));
#line 203 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  coerce_tmp_chpl5 = *(*(inds_chpl) + INT64(2));
#line 203 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl__buildDomainExpr(coerce_tmp_chpl3, coerce_tmp_chpl4, coerce_tmp_chpl5, UINT8(false), &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 203 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  call_tmp_chpl6 = ret_tmp_chpl;
#line 203 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  upDomRec_chpl = call_tmp_chpl6;
#line 203 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _instance_chpl = &((&upDomRec_chpl)->_instance);
#line 203 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(_instance_chpl, _ln_chpl, _fn_chpl);
#line 203 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ((BaseDom_chpl)(*(_instance_chpl)))->definedConst = UINT8(false);
#line 204 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ret_chpl = (&upDomRec_chpl)->_instance;
#line 204 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 204 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  (this_chpl31)->upDomInst = ret_chpl;
#line 209 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  init_chpl113(&elt_x0_chpl);
#line 209 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  tup_x0_chpl = elt_x0_chpl;
#line 209 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  init_chpl113(&elt_x1_chpl);
#line 209 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  tup_x1_chpl = elt_x1_chpl;
#line 209 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  init_chpl113(&elt_x2_chpl);
#line 209 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  tup_x2_chpl = elt_x2_chpl;
#line 209 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  init_chpl113(&elt_x3_chpl);
#line 209 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  tup_x3_chpl = elt_x3_chpl;
#line 209 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(ret_tmp_chpl2 + INT64(0)) = tup_x0_chpl;
#line 209 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(ret_tmp_chpl2 + INT64(1)) = tup_x1_chpl;
#line 209 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(ret_tmp_chpl2 + INT64(2)) = tup_x2_chpl;
#line 209 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(ret_tmp_chpl2 + INT64(3)) = tup_x3_chpl;
#line 210 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 210 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  coerce_tmp_chpl6 = (this_chpl31)->distInst;
#line 210 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(coerce_tmp_chpl6, _ln_chpl, _fn_chpl);
#line 210 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(coerce_tmp_chpl7 + INT64(0)) = *(ret_tmp_chpl2 + INT64(0));
#line 210 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(coerce_tmp_chpl7 + INT64(1)) = *(ret_tmp_chpl2 + INT64(1));
#line 210 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(coerce_tmp_chpl7 + INT64(2)) = *(ret_tmp_chpl2 + INT64(2));
#line 210 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(coerce_tmp_chpl7 + INT64(3)) = *(ret_tmp_chpl2 + INT64(3));
#line 210 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  call_tmp_chpl7 = chpl_dsiNewRectangularDom3((coerce_tmp_chpl6)->downDistInst, &coerce_tmp_chpl7, UINT8(false), _ln_chpl, _fn_chpl);
#line 213 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  init_chpl81(&initTemp_chpl, call_tmp_chpl7);
#line 213 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  call_tmp_chpl8 = initTemp_chpl;
#line 213 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  downDomLoc_chpl = call_tmp_chpl8;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  call_tmp_chpl9 = &((this_chpl31)->collapsedDim);
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  call_tmp_chpl10 = &((this_chpl31)->idx);
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  init_chpl113(&elt_x0_chpl2);
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  tup_x0_chpl2 = elt_x0_chpl2;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  init_chpl113(&elt_x1_chpl2);
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  tup_x1_chpl2 = elt_x1_chpl2;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  init_chpl113(&elt_x2_chpl2);
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  tup_x2_chpl2 = elt_x2_chpl2;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  init_chpl113(&elt_x3_chpl2);
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  tup_x3_chpl2 = elt_x3_chpl2;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ranges_x0_chpl = tup_x0_chpl2;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ranges_x1_chpl = tup_x1_chpl2;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ranges_x2_chpl = tup_x2_chpl2;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ranges_x3_chpl = tup_x3_chpl2;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  j_chpl = INT64(0);
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  if (! *(*(call_tmp_chpl9) + INT64(0))) /* ZLINE: 214 /Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl */
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  {
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    call_tmp_chpl12 = this_chpl24(inds_chpl, INT64(0), _ln_chpl, _fn_chpl);
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    coerce_tmp_chpl8 = *(call_tmp_chpl12);
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    ranges_x0_chpl = coerce_tmp_chpl8;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    i_lhs_chpl = &j_chpl;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    *(i_lhs_chpl) += INT64(1);
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  } else /* ZLINE: 214 /Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl */
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  {
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    createWithSingleElement_chpl(*(*(call_tmp_chpl10) + INT64(0)), &ret_tmp_chpl3);
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    call_tmp_chpl13 = ret_tmp_chpl3;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    ranges_x0_chpl = call_tmp_chpl13;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  }
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  if (! *(*(call_tmp_chpl9) + INT64(1))) /* ZLINE: 214 /Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl */
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  {
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    call_tmp_chpl14 = this_chpl24(inds_chpl, j_chpl, _ln_chpl, _fn_chpl);
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    coerce_tmp_chpl9 = *(call_tmp_chpl14);
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    ranges_x1_chpl = coerce_tmp_chpl9;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    i_lhs_chpl2 = &j_chpl;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    *(i_lhs_chpl2) += INT64(1);
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  } else /* ZLINE: 214 /Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl */
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  {
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    createWithSingleElement_chpl(*(*(call_tmp_chpl10) + INT64(1)), &ret_tmp_chpl4);
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    call_tmp_chpl15 = ret_tmp_chpl4;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    ranges_x1_chpl = call_tmp_chpl15;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  }
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  if (! *(*(call_tmp_chpl9) + INT64(2))) /* ZLINE: 214 /Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl */
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  {
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    call_tmp_chpl16 = this_chpl24(inds_chpl, j_chpl, _ln_chpl, _fn_chpl);
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    coerce_tmp_chpl10 = *(call_tmp_chpl16);
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    ranges_x2_chpl = coerce_tmp_chpl10;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    i_lhs_chpl3 = &j_chpl;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    *(i_lhs_chpl3) += INT64(1);
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  } else /* ZLINE: 214 /Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl */
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  {
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    createWithSingleElement_chpl(*(*(call_tmp_chpl10) + INT64(2)), &ret_tmp_chpl5);
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    call_tmp_chpl17 = ret_tmp_chpl5;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    ranges_x2_chpl = call_tmp_chpl17;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  }
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  if (! *(*(call_tmp_chpl9) + INT64(3))) /* ZLINE: 214 /Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl */
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  {
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    call_tmp_chpl18 = this_chpl24(inds_chpl, j_chpl, _ln_chpl, _fn_chpl);
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    coerce_tmp_chpl11 = *(call_tmp_chpl18);
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    ranges_x3_chpl = coerce_tmp_chpl11;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    i_lhs_chpl4 = &j_chpl;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    *(i_lhs_chpl4) += INT64(1);
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  } else /* ZLINE: 214 /Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl */
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  {
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    createWithSingleElement_chpl(*(*(call_tmp_chpl10) + INT64(3)), &ret_tmp_chpl6);
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    call_tmp_chpl19 = ret_tmp_chpl6;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    ranges_x3_chpl = call_tmp_chpl19;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  }
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl__buildDomainExpr4(ranges_x0_chpl, ranges_x1_chpl, ranges_x2_chpl, ranges_x3_chpl, UINT8(true), &ret_tmp_chpl7, _ln_chpl, _fn_chpl);
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  call_tmp_chpl20 = ret_tmp_chpl7;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  call_tmp_chpl11 = call_tmp_chpl20;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl___ASSIGN_4(&downDomLoc_chpl, &call_tmp_chpl11, _ln_chpl, _fn_chpl);
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl__autoDestroy9(&call_tmp_chpl11, _ln_chpl, _fn_chpl);
#line 215 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  coerce_tmp_chpl12 = ((BaseDom_chpl)((&downDomLoc_chpl)->_instance));
#line 215 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(coerce_tmp_chpl12, _ln_chpl, _fn_chpl);
#line 215 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  (coerce_tmp_chpl12)->_free_when_no_arrs = UINT8(true);
#line 216 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  coerce_tmp_chpl13 = (&downDomLoc_chpl)->_pid;
#line 216 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 216 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  (this_chpl31)->downDomPid = coerce_tmp_chpl13;
#line 217 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  coerce_tmp_chpl14 = (&downDomLoc_chpl)->_instance;
#line 217 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 217 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  (this_chpl31)->downDomInst = coerce_tmp_chpl14;
#line 198 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  return;
#line 198 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
}

#line 198 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
/* ArrayViewRankChange.chpl:198 */
#line 198 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
static void dsiSetIndices_chpl(ArrayViewRankChangeDom_2_int64_t_one__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_3_star_chpl_bool__tuple_3_star_int64_t_chpl this_chpl31,
#line 198 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                               _tuple_2_star_range_int64_t_both_one_chpl * inds_chpl,
#line 198 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                               int64_t _ln_chpl,
#line 198 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                               int32_t _fn_chpl) {
#line 203 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _domain_DefaultRectangularDom_2_int64_t_one_chpl call_tmp_chpl6;
#line 203 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl3;
#line 203 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl4;
#line 203 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _domain_DefaultRectangularDom_2_int64_t_one_chpl ret_tmp_chpl;
#line 203 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _domain_DefaultRectangularDom_2_int64_t_one_chpl upDomRec_chpl;
#line 203 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _ref_DefaultRectangularDom_2_int64_t_one_chpl _instance_chpl = NULL;
#line 204 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  DefaultRectangularDom_2_int64_t_one_chpl ret_chpl = NULL;
#line 209 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _tuple_3_star_range_int64_t_both_one_chpl ret_tmp_chpl2;
#line 209 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl tup_x0_chpl;
#line 209 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl tup_x1_chpl;
#line 209 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl tup_x2_chpl;
#line 209 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl elt_x0_chpl;
#line 209 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl elt_x1_chpl;
#line 209 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl elt_x2_chpl;
#line 210 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ArrayViewRankChangeDist_DefaultDist__tuple_3_star_chpl_bool__tuple_3_star_int64_t_chpl coerce_tmp_chpl5 = NULL;
#line 210 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  DefaultRectangularDom_3_int64_t_one_chpl call_tmp_chpl7 = NULL;
#line 210 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _tuple_3_star_range_int64_t_both_one_chpl coerce_tmp_chpl6;
#line 213 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _domain_DefaultRectangularDom_3_int64_t_one_chpl call_tmp_chpl8;
#line 213 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _domain_DefaultRectangularDom_3_int64_t_one_chpl initTemp_chpl;
#line 213 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _domain_DefaultRectangularDom_3_int64_t_one_chpl downDomLoc_chpl;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _ref__tuple_3_star_chpl_bool_chpl call_tmp_chpl9 = NULL;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _ref__tuple_3_star_int64_t_chpl call_tmp_chpl10 = NULL;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _domain_DefaultRectangularDom_3_int64_t_one_chpl call_tmp_chpl11;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl ranges_x0_chpl;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl ranges_x1_chpl;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl ranges_x2_chpl;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl tup_x0_chpl2;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl tup_x1_chpl2;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl tup_x2_chpl2;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl elt_x0_chpl2;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl elt_x1_chpl2;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl elt_x2_chpl2;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  int64_t j_chpl;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl12 = NULL;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl7;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _ref_int64_t_chpl i_lhs_chpl = NULL;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl13;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl3;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl14 = NULL;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl8;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _ref_int64_t_chpl i_lhs_chpl2 = NULL;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl15;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl4;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl16 = NULL;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl9;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _ref_int64_t_chpl i_lhs_chpl3 = NULL;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl17;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl5;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _domain_DefaultRectangularDom_3_int64_t_one_chpl call_tmp_chpl18;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _domain_DefaultRectangularDom_3_int64_t_one_chpl ret_tmp_chpl6;
#line 215 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  BaseDom_chpl coerce_tmp_chpl10 = NULL;
#line 216 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  int64_t coerce_tmp_chpl11;
#line 217 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  DefaultRectangularDom_3_int64_t_one_chpl coerce_tmp_chpl12 = NULL;
#line 200 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 200 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  dsiDestroyDom_chpl(this_chpl31, _ln_chpl, _fn_chpl);
#line 203 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  coerce_tmp_chpl3 = *(*(inds_chpl) + INT64(0));
#line 203 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  coerce_tmp_chpl4 = *(*(inds_chpl) + INT64(1));
#line 203 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl__buildDomainExpr2(coerce_tmp_chpl3, coerce_tmp_chpl4, UINT8(false), &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 203 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  call_tmp_chpl6 = ret_tmp_chpl;
#line 203 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  upDomRec_chpl = call_tmp_chpl6;
#line 203 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _instance_chpl = &((&upDomRec_chpl)->_instance);
#line 203 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(_instance_chpl, _ln_chpl, _fn_chpl);
#line 203 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ((BaseDom_chpl)(*(_instance_chpl)))->definedConst = UINT8(false);
#line 204 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ret_chpl = (&upDomRec_chpl)->_instance;
#line 204 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 204 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  (this_chpl31)->upDomInst = ret_chpl;
#line 209 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  init_chpl113(&elt_x0_chpl);
#line 209 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  tup_x0_chpl = elt_x0_chpl;
#line 209 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  init_chpl113(&elt_x1_chpl);
#line 209 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  tup_x1_chpl = elt_x1_chpl;
#line 209 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  init_chpl113(&elt_x2_chpl);
#line 209 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  tup_x2_chpl = elt_x2_chpl;
#line 209 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(ret_tmp_chpl2 + INT64(0)) = tup_x0_chpl;
#line 209 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(ret_tmp_chpl2 + INT64(1)) = tup_x1_chpl;
#line 209 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(ret_tmp_chpl2 + INT64(2)) = tup_x2_chpl;
#line 210 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 210 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  coerce_tmp_chpl5 = (this_chpl31)->distInst;
#line 210 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(coerce_tmp_chpl5, _ln_chpl, _fn_chpl);
#line 210 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(coerce_tmp_chpl6 + INT64(0)) = *(ret_tmp_chpl2 + INT64(0));
#line 210 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(coerce_tmp_chpl6 + INT64(1)) = *(ret_tmp_chpl2 + INT64(1));
#line 210 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(coerce_tmp_chpl6 + INT64(2)) = *(ret_tmp_chpl2 + INT64(2));
#line 210 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  call_tmp_chpl7 = chpl_dsiNewRectangularDom((coerce_tmp_chpl5)->downDistInst, &coerce_tmp_chpl6, UINT8(false), _ln_chpl, _fn_chpl);
#line 213 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  init_chpl82(&initTemp_chpl, call_tmp_chpl7);
#line 213 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  call_tmp_chpl8 = initTemp_chpl;
#line 213 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  downDomLoc_chpl = call_tmp_chpl8;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  call_tmp_chpl9 = &((this_chpl31)->collapsedDim);
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  call_tmp_chpl10 = &((this_chpl31)->idx);
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  init_chpl113(&elt_x0_chpl2);
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  tup_x0_chpl2 = elt_x0_chpl2;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  init_chpl113(&elt_x1_chpl2);
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  tup_x1_chpl2 = elt_x1_chpl2;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  init_chpl113(&elt_x2_chpl2);
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  tup_x2_chpl2 = elt_x2_chpl2;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ranges_x0_chpl = tup_x0_chpl2;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ranges_x1_chpl = tup_x1_chpl2;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ranges_x2_chpl = tup_x2_chpl2;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  j_chpl = INT64(0);
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  if (! *(*(call_tmp_chpl9) + INT64(0))) /* ZLINE: 214 /Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl */
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  {
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    call_tmp_chpl12 = this_chpl26(inds_chpl, INT64(0), _ln_chpl, _fn_chpl);
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    coerce_tmp_chpl7 = *(call_tmp_chpl12);
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    ranges_x0_chpl = coerce_tmp_chpl7;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    i_lhs_chpl = &j_chpl;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    *(i_lhs_chpl) += INT64(1);
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  } else /* ZLINE: 214 /Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl */
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  {
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    createWithSingleElement_chpl(*(*(call_tmp_chpl10) + INT64(0)), &ret_tmp_chpl3);
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    call_tmp_chpl13 = ret_tmp_chpl3;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    ranges_x0_chpl = call_tmp_chpl13;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  }
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  if (! *(*(call_tmp_chpl9) + INT64(1))) /* ZLINE: 214 /Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl */
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  {
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    call_tmp_chpl14 = this_chpl26(inds_chpl, j_chpl, _ln_chpl, _fn_chpl);
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    coerce_tmp_chpl8 = *(call_tmp_chpl14);
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    ranges_x1_chpl = coerce_tmp_chpl8;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    i_lhs_chpl2 = &j_chpl;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    *(i_lhs_chpl2) += INT64(1);
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  } else /* ZLINE: 214 /Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl */
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  {
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    createWithSingleElement_chpl(*(*(call_tmp_chpl10) + INT64(1)), &ret_tmp_chpl4);
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    call_tmp_chpl15 = ret_tmp_chpl4;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    ranges_x1_chpl = call_tmp_chpl15;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  }
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  if (! *(*(call_tmp_chpl9) + INT64(2))) /* ZLINE: 214 /Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl */
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  {
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    call_tmp_chpl16 = this_chpl26(inds_chpl, j_chpl, _ln_chpl, _fn_chpl);
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    coerce_tmp_chpl9 = *(call_tmp_chpl16);
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    ranges_x2_chpl = coerce_tmp_chpl9;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    i_lhs_chpl3 = &j_chpl;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    *(i_lhs_chpl3) += INT64(1);
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  } else /* ZLINE: 214 /Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl */
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  {
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    createWithSingleElement_chpl(*(*(call_tmp_chpl10) + INT64(2)), &ret_tmp_chpl5);
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    call_tmp_chpl17 = ret_tmp_chpl5;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    ranges_x2_chpl = call_tmp_chpl17;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  }
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl__buildDomainExpr(ranges_x0_chpl, ranges_x1_chpl, ranges_x2_chpl, UINT8(true), &ret_tmp_chpl6, _ln_chpl, _fn_chpl);
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  call_tmp_chpl18 = ret_tmp_chpl6;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  call_tmp_chpl11 = call_tmp_chpl18;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl___ASSIGN_6(&downDomLoc_chpl, &call_tmp_chpl11, _ln_chpl, _fn_chpl);
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl__autoDestroy8(&call_tmp_chpl11, _ln_chpl, _fn_chpl);
#line 215 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  coerce_tmp_chpl10 = ((BaseDom_chpl)((&downDomLoc_chpl)->_instance));
#line 215 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(coerce_tmp_chpl10, _ln_chpl, _fn_chpl);
#line 215 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  (coerce_tmp_chpl10)->_free_when_no_arrs = UINT8(true);
#line 216 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  coerce_tmp_chpl11 = (&downDomLoc_chpl)->_pid;
#line 216 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 216 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  (this_chpl31)->downDomPid = coerce_tmp_chpl11;
#line 217 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  coerce_tmp_chpl12 = (&downDomLoc_chpl)->_instance;
#line 217 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 217 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  (this_chpl31)->downDomInst = coerce_tmp_chpl12;
#line 198 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  return;
#line 198 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
}

#line 198 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
/* ArrayViewRankChange.chpl:198 */
#line 198 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
static void dsiSetIndices_chpl2(ArrayViewRankChangeDom_1_int64_t_one__tuple_2_star_chpl_bool__tuple_2_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_2_star_chpl_bool__tuple_2_star_int64_t_chpl this_chpl31,
#line 198 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                _tuple_1_star_range_int64_t_both_one_chpl * inds_chpl,
#line 198 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                int64_t _ln_chpl,
#line 198 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                int32_t _fn_chpl) {
#line 203 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _domain_DefaultRectangularDom_1_int64_t_one_chpl call_tmp_chpl6;
#line 203 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl3;
#line 203 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _domain_DefaultRectangularDom_1_int64_t_one_chpl ret_tmp_chpl;
#line 203 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _domain_DefaultRectangularDom_1_int64_t_one_chpl upDomRec_chpl;
#line 203 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _ref_DefaultRectangularDom_1_int64_t_one_chpl _instance_chpl = NULL;
#line 204 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  DefaultRectangularDom_1_int64_t_one_chpl ret_chpl = NULL;
#line 209 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _tuple_2_star_range_int64_t_both_one_chpl ret_tmp_chpl2;
#line 209 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl tup_x0_chpl;
#line 209 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl tup_x1_chpl;
#line 209 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl elt_x0_chpl;
#line 209 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl elt_x1_chpl;
#line 210 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ArrayViewRankChangeDist_DefaultDist__tuple_2_star_chpl_bool__tuple_2_star_int64_t_chpl coerce_tmp_chpl4 = NULL;
#line 210 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  DefaultRectangularDom_2_int64_t_one_chpl call_tmp_chpl7 = NULL;
#line 210 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _tuple_2_star_range_int64_t_both_one_chpl coerce_tmp_chpl5;
#line 213 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _domain_DefaultRectangularDom_2_int64_t_one_chpl call_tmp_chpl8;
#line 213 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _domain_DefaultRectangularDom_2_int64_t_one_chpl initTemp_chpl;
#line 213 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _domain_DefaultRectangularDom_2_int64_t_one_chpl downDomLoc_chpl;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _ref__tuple_2_star_chpl_bool_chpl call_tmp_chpl9 = NULL;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _ref__tuple_2_star_int64_t_chpl call_tmp_chpl10 = NULL;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _domain_DefaultRectangularDom_2_int64_t_one_chpl call_tmp_chpl11;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl ranges_x0_chpl;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl ranges_x1_chpl;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl tup_x0_chpl2;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl tup_x1_chpl2;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl elt_x0_chpl2;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl elt_x1_chpl2;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  int64_t j_chpl;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl12 = NULL;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl6;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _ref_int64_t_chpl i_lhs_chpl = NULL;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl13;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl3;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl14 = NULL;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl7;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _ref_int64_t_chpl i_lhs_chpl2 = NULL;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl15;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl4;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _domain_DefaultRectangularDom_2_int64_t_one_chpl call_tmp_chpl16;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _domain_DefaultRectangularDom_2_int64_t_one_chpl ret_tmp_chpl5;
#line 215 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  BaseDom_chpl coerce_tmp_chpl8 = NULL;
#line 216 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  int64_t coerce_tmp_chpl9;
#line 217 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  DefaultRectangularDom_2_int64_t_one_chpl coerce_tmp_chpl10 = NULL;
#line 200 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 200 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  dsiDestroyDom_chpl2(this_chpl31, _ln_chpl, _fn_chpl);
#line 203 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  coerce_tmp_chpl3 = *(*(inds_chpl) + INT64(0));
#line 203 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl__buildDomainExpr3(coerce_tmp_chpl3, UINT8(false), &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 203 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  call_tmp_chpl6 = ret_tmp_chpl;
#line 203 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  upDomRec_chpl = call_tmp_chpl6;
#line 203 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _instance_chpl = &((&upDomRec_chpl)->_instance);
#line 203 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(_instance_chpl, _ln_chpl, _fn_chpl);
#line 203 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ((BaseDom_chpl)(*(_instance_chpl)))->definedConst = UINT8(false);
#line 204 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ret_chpl = (&upDomRec_chpl)->_instance;
#line 204 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 204 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  (this_chpl31)->upDomInst = ret_chpl;
#line 209 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  init_chpl113(&elt_x0_chpl);
#line 209 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  tup_x0_chpl = elt_x0_chpl;
#line 209 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  init_chpl113(&elt_x1_chpl);
#line 209 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  tup_x1_chpl = elt_x1_chpl;
#line 209 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(ret_tmp_chpl2 + INT64(0)) = tup_x0_chpl;
#line 209 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(ret_tmp_chpl2 + INT64(1)) = tup_x1_chpl;
#line 210 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 210 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  coerce_tmp_chpl4 = (this_chpl31)->distInst;
#line 210 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(coerce_tmp_chpl4, _ln_chpl, _fn_chpl);
#line 210 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(coerce_tmp_chpl5 + INT64(0)) = *(ret_tmp_chpl2 + INT64(0));
#line 210 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(coerce_tmp_chpl5 + INT64(1)) = *(ret_tmp_chpl2 + INT64(1));
#line 210 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  call_tmp_chpl7 = chpl_dsiNewRectangularDom2((coerce_tmp_chpl4)->downDistInst, &coerce_tmp_chpl5, UINT8(false), _ln_chpl, _fn_chpl);
#line 213 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  init_chpl84(&initTemp_chpl, call_tmp_chpl7);
#line 213 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  call_tmp_chpl8 = initTemp_chpl;
#line 213 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  downDomLoc_chpl = call_tmp_chpl8;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  call_tmp_chpl9 = &((this_chpl31)->collapsedDim);
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  call_tmp_chpl10 = &((this_chpl31)->idx);
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  init_chpl113(&elt_x0_chpl2);
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  tup_x0_chpl2 = elt_x0_chpl2;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  init_chpl113(&elt_x1_chpl2);
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  tup_x1_chpl2 = elt_x1_chpl2;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ranges_x0_chpl = tup_x0_chpl2;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ranges_x1_chpl = tup_x1_chpl2;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  j_chpl = INT64(0);
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  if (! *(*(call_tmp_chpl9) + INT64(0))) /* ZLINE: 214 /Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl */
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  {
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    call_tmp_chpl12 = this_chpl20(inds_chpl, INT64(0), _ln_chpl, _fn_chpl);
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    coerce_tmp_chpl6 = *(call_tmp_chpl12);
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    ranges_x0_chpl = coerce_tmp_chpl6;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    i_lhs_chpl = &j_chpl;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    *(i_lhs_chpl) += INT64(1);
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  } else /* ZLINE: 214 /Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl */
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  {
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    createWithSingleElement_chpl(*(*(call_tmp_chpl10) + INT64(0)), &ret_tmp_chpl3);
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    call_tmp_chpl13 = ret_tmp_chpl3;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    ranges_x0_chpl = call_tmp_chpl13;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  }
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  if (! *(*(call_tmp_chpl9) + INT64(1))) /* ZLINE: 214 /Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl */
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  {
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    call_tmp_chpl14 = this_chpl20(inds_chpl, j_chpl, _ln_chpl, _fn_chpl);
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    coerce_tmp_chpl7 = *(call_tmp_chpl14);
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    ranges_x1_chpl = coerce_tmp_chpl7;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    i_lhs_chpl2 = &j_chpl;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    *(i_lhs_chpl2) += INT64(1);
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  } else /* ZLINE: 214 /Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl */
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  {
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    createWithSingleElement_chpl(*(*(call_tmp_chpl10) + INT64(1)), &ret_tmp_chpl4);
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    call_tmp_chpl15 = ret_tmp_chpl4;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    ranges_x1_chpl = call_tmp_chpl15;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  }
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl__buildDomainExpr2(ranges_x0_chpl, ranges_x1_chpl, UINT8(true), &ret_tmp_chpl5, _ln_chpl, _fn_chpl);
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  call_tmp_chpl16 = ret_tmp_chpl5;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  call_tmp_chpl11 = call_tmp_chpl16;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl___ASSIGN_5(&downDomLoc_chpl, &call_tmp_chpl11, _ln_chpl, _fn_chpl);
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl__autoDestroy5(&call_tmp_chpl11, _ln_chpl, _fn_chpl);
#line 215 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  coerce_tmp_chpl8 = ((BaseDom_chpl)((&downDomLoc_chpl)->_instance));
#line 215 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(coerce_tmp_chpl8, _ln_chpl, _fn_chpl);
#line 215 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  (coerce_tmp_chpl8)->_free_when_no_arrs = UINT8(true);
#line 216 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  coerce_tmp_chpl9 = (&downDomLoc_chpl)->_pid;
#line 216 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 216 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  (this_chpl31)->downDomPid = coerce_tmp_chpl9;
#line 217 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  coerce_tmp_chpl10 = (&downDomLoc_chpl)->_instance;
#line 217 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 217 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  (this_chpl31)->downDomInst = coerce_tmp_chpl10;
#line 198 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  return;
#line 198 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
}

#line 198 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
/* ArrayViewRankChange.chpl:198 */
#line 198 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
static void dsiSetIndices_chpl3(ArrayViewRankChangeDom_2_int64_t_one__tuple_4_star_chpl_bool__tuple_4_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_4_star_chpl_bool__tuple_4_star_int64_t_chpl this_chpl31,
#line 198 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                _tuple_2_star_range_int64_t_both_one_chpl * inds_chpl,
#line 198 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                int64_t _ln_chpl,
#line 198 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                int32_t _fn_chpl) {
#line 203 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _domain_DefaultRectangularDom_2_int64_t_one_chpl call_tmp_chpl6;
#line 203 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl3;
#line 203 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl4;
#line 203 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _domain_DefaultRectangularDom_2_int64_t_one_chpl ret_tmp_chpl;
#line 203 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _domain_DefaultRectangularDom_2_int64_t_one_chpl upDomRec_chpl;
#line 203 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _ref_DefaultRectangularDom_2_int64_t_one_chpl _instance_chpl = NULL;
#line 204 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  DefaultRectangularDom_2_int64_t_one_chpl ret_chpl = NULL;
#line 209 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _tuple_4_star_range_int64_t_both_one_chpl ret_tmp_chpl2;
#line 209 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl tup_x0_chpl;
#line 209 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl tup_x1_chpl;
#line 209 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl tup_x2_chpl;
#line 209 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl tup_x3_chpl;
#line 209 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl elt_x0_chpl;
#line 209 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl elt_x1_chpl;
#line 209 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl elt_x2_chpl;
#line 209 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl elt_x3_chpl;
#line 210 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ArrayViewRankChangeDist_DefaultDist__tuple_4_star_chpl_bool__tuple_4_star_int64_t_chpl coerce_tmp_chpl5 = NULL;
#line 210 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  DefaultRectangularDom_4_int64_t_one_chpl call_tmp_chpl7 = NULL;
#line 210 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _tuple_4_star_range_int64_t_both_one_chpl coerce_tmp_chpl6;
#line 213 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _domain_DefaultRectangularDom_4_int64_t_one_chpl call_tmp_chpl8;
#line 213 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _domain_DefaultRectangularDom_4_int64_t_one_chpl initTemp_chpl;
#line 213 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _domain_DefaultRectangularDom_4_int64_t_one_chpl downDomLoc_chpl;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _ref__tuple_4_star_chpl_bool_chpl call_tmp_chpl9 = NULL;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _ref__tuple_4_star_int64_t_chpl call_tmp_chpl10 = NULL;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _domain_DefaultRectangularDom_4_int64_t_one_chpl call_tmp_chpl11;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl ranges_x0_chpl;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl ranges_x1_chpl;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl ranges_x2_chpl;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl ranges_x3_chpl;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl tup_x0_chpl2;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl tup_x1_chpl2;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl tup_x2_chpl2;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl tup_x3_chpl2;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl elt_x0_chpl2;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl elt_x1_chpl2;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl elt_x2_chpl2;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl elt_x3_chpl2;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  int64_t j_chpl;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl12 = NULL;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl7;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _ref_int64_t_chpl i_lhs_chpl = NULL;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl13;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl3;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl14 = NULL;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl8;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _ref_int64_t_chpl i_lhs_chpl2 = NULL;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl15;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl4;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl16 = NULL;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl9;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _ref_int64_t_chpl i_lhs_chpl3 = NULL;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl17;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl5;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl18 = NULL;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl10;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _ref_int64_t_chpl i_lhs_chpl4 = NULL;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl19;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl6;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _domain_DefaultRectangularDom_4_int64_t_one_chpl call_tmp_chpl20;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _domain_DefaultRectangularDom_4_int64_t_one_chpl ret_tmp_chpl7;
#line 215 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  BaseDom_chpl coerce_tmp_chpl11 = NULL;
#line 216 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  int64_t coerce_tmp_chpl12;
#line 217 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  DefaultRectangularDom_4_int64_t_one_chpl coerce_tmp_chpl13 = NULL;
#line 200 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 200 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  dsiDestroyDom_chpl3(this_chpl31, _ln_chpl, _fn_chpl);
#line 203 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  coerce_tmp_chpl3 = *(*(inds_chpl) + INT64(0));
#line 203 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  coerce_tmp_chpl4 = *(*(inds_chpl) + INT64(1));
#line 203 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl__buildDomainExpr2(coerce_tmp_chpl3, coerce_tmp_chpl4, UINT8(false), &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 203 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  call_tmp_chpl6 = ret_tmp_chpl;
#line 203 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  upDomRec_chpl = call_tmp_chpl6;
#line 203 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _instance_chpl = &((&upDomRec_chpl)->_instance);
#line 203 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(_instance_chpl, _ln_chpl, _fn_chpl);
#line 203 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ((BaseDom_chpl)(*(_instance_chpl)))->definedConst = UINT8(false);
#line 204 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ret_chpl = (&upDomRec_chpl)->_instance;
#line 204 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 204 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  (this_chpl31)->upDomInst = ret_chpl;
#line 209 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  init_chpl113(&elt_x0_chpl);
#line 209 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  tup_x0_chpl = elt_x0_chpl;
#line 209 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  init_chpl113(&elt_x1_chpl);
#line 209 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  tup_x1_chpl = elt_x1_chpl;
#line 209 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  init_chpl113(&elt_x2_chpl);
#line 209 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  tup_x2_chpl = elt_x2_chpl;
#line 209 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  init_chpl113(&elt_x3_chpl);
#line 209 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  tup_x3_chpl = elt_x3_chpl;
#line 209 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(ret_tmp_chpl2 + INT64(0)) = tup_x0_chpl;
#line 209 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(ret_tmp_chpl2 + INT64(1)) = tup_x1_chpl;
#line 209 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(ret_tmp_chpl2 + INT64(2)) = tup_x2_chpl;
#line 209 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(ret_tmp_chpl2 + INT64(3)) = tup_x3_chpl;
#line 210 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 210 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  coerce_tmp_chpl5 = (this_chpl31)->distInst;
#line 210 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(coerce_tmp_chpl5, _ln_chpl, _fn_chpl);
#line 210 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(coerce_tmp_chpl6 + INT64(0)) = *(ret_tmp_chpl2 + INT64(0));
#line 210 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(coerce_tmp_chpl6 + INT64(1)) = *(ret_tmp_chpl2 + INT64(1));
#line 210 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(coerce_tmp_chpl6 + INT64(2)) = *(ret_tmp_chpl2 + INT64(2));
#line 210 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(coerce_tmp_chpl6 + INT64(3)) = *(ret_tmp_chpl2 + INT64(3));
#line 210 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  call_tmp_chpl7 = chpl_dsiNewRectangularDom3((coerce_tmp_chpl5)->downDistInst, &coerce_tmp_chpl6, UINT8(false), _ln_chpl, _fn_chpl);
#line 213 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  init_chpl81(&initTemp_chpl, call_tmp_chpl7);
#line 213 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  call_tmp_chpl8 = initTemp_chpl;
#line 213 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  downDomLoc_chpl = call_tmp_chpl8;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  call_tmp_chpl9 = &((this_chpl31)->collapsedDim);
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  call_tmp_chpl10 = &((this_chpl31)->idx);
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  init_chpl113(&elt_x0_chpl2);
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  tup_x0_chpl2 = elt_x0_chpl2;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  init_chpl113(&elt_x1_chpl2);
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  tup_x1_chpl2 = elt_x1_chpl2;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  init_chpl113(&elt_x2_chpl2);
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  tup_x2_chpl2 = elt_x2_chpl2;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  init_chpl113(&elt_x3_chpl2);
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  tup_x3_chpl2 = elt_x3_chpl2;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ranges_x0_chpl = tup_x0_chpl2;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ranges_x1_chpl = tup_x1_chpl2;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ranges_x2_chpl = tup_x2_chpl2;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ranges_x3_chpl = tup_x3_chpl2;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  j_chpl = INT64(0);
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  if (! *(*(call_tmp_chpl9) + INT64(0))) /* ZLINE: 214 /Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl */
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  {
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    call_tmp_chpl12 = this_chpl26(inds_chpl, INT64(0), _ln_chpl, _fn_chpl);
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    coerce_tmp_chpl7 = *(call_tmp_chpl12);
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    ranges_x0_chpl = coerce_tmp_chpl7;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    i_lhs_chpl = &j_chpl;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    *(i_lhs_chpl) += INT64(1);
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  } else /* ZLINE: 214 /Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl */
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  {
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    createWithSingleElement_chpl(*(*(call_tmp_chpl10) + INT64(0)), &ret_tmp_chpl3);
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    call_tmp_chpl13 = ret_tmp_chpl3;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    ranges_x0_chpl = call_tmp_chpl13;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  }
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  if (! *(*(call_tmp_chpl9) + INT64(1))) /* ZLINE: 214 /Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl */
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  {
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    call_tmp_chpl14 = this_chpl26(inds_chpl, j_chpl, _ln_chpl, _fn_chpl);
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    coerce_tmp_chpl8 = *(call_tmp_chpl14);
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    ranges_x1_chpl = coerce_tmp_chpl8;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    i_lhs_chpl2 = &j_chpl;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    *(i_lhs_chpl2) += INT64(1);
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  } else /* ZLINE: 214 /Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl */
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  {
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    createWithSingleElement_chpl(*(*(call_tmp_chpl10) + INT64(1)), &ret_tmp_chpl4);
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    call_tmp_chpl15 = ret_tmp_chpl4;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    ranges_x1_chpl = call_tmp_chpl15;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  }
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  if (! *(*(call_tmp_chpl9) + INT64(2))) /* ZLINE: 214 /Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl */
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  {
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    call_tmp_chpl16 = this_chpl26(inds_chpl, j_chpl, _ln_chpl, _fn_chpl);
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    coerce_tmp_chpl9 = *(call_tmp_chpl16);
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    ranges_x2_chpl = coerce_tmp_chpl9;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    i_lhs_chpl3 = &j_chpl;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    *(i_lhs_chpl3) += INT64(1);
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  } else /* ZLINE: 214 /Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl */
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  {
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    createWithSingleElement_chpl(*(*(call_tmp_chpl10) + INT64(2)), &ret_tmp_chpl5);
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    call_tmp_chpl17 = ret_tmp_chpl5;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    ranges_x2_chpl = call_tmp_chpl17;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  }
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  if (! *(*(call_tmp_chpl9) + INT64(3))) /* ZLINE: 214 /Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl */
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  {
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    call_tmp_chpl18 = this_chpl26(inds_chpl, j_chpl, _ln_chpl, _fn_chpl);
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    coerce_tmp_chpl10 = *(call_tmp_chpl18);
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    ranges_x3_chpl = coerce_tmp_chpl10;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    i_lhs_chpl4 = &j_chpl;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    *(i_lhs_chpl4) += INT64(1);
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  } else /* ZLINE: 214 /Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl */
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  {
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    createWithSingleElement_chpl(*(*(call_tmp_chpl10) + INT64(3)), &ret_tmp_chpl6);
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    call_tmp_chpl19 = ret_tmp_chpl6;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    ranges_x3_chpl = call_tmp_chpl19;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  }
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl__buildDomainExpr4(ranges_x0_chpl, ranges_x1_chpl, ranges_x2_chpl, ranges_x3_chpl, UINT8(true), &ret_tmp_chpl7, _ln_chpl, _fn_chpl);
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  call_tmp_chpl20 = ret_tmp_chpl7;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  call_tmp_chpl11 = call_tmp_chpl20;
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl___ASSIGN_4(&downDomLoc_chpl, &call_tmp_chpl11, _ln_chpl, _fn_chpl);
#line 214 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl__autoDestroy9(&call_tmp_chpl11, _ln_chpl, _fn_chpl);
#line 215 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  coerce_tmp_chpl11 = ((BaseDom_chpl)((&downDomLoc_chpl)->_instance));
#line 215 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(coerce_tmp_chpl11, _ln_chpl, _fn_chpl);
#line 215 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  (coerce_tmp_chpl11)->_free_when_no_arrs = UINT8(true);
#line 216 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  coerce_tmp_chpl12 = (&downDomLoc_chpl)->_pid;
#line 216 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 216 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  (this_chpl31)->downDomPid = coerce_tmp_chpl12;
#line 217 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  coerce_tmp_chpl13 = (&downDomLoc_chpl)->_instance;
#line 217 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 217 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  (this_chpl31)->downDomInst = coerce_tmp_chpl13;
#line 198 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  return;
#line 198 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
}

#line 332 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
/* ArrayViewRankChange.chpl:332 */
#line 332 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
static ArrayViewRankChangeDist_DefaultDist__tuple_2_star_chpl_bool__tuple_2_star_int64_t_chpl dsiMyDist_chpl(ArrayViewRankChangeDom_1_int64_t_one__tuple_2_star_chpl_bool__tuple_2_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_2_star_chpl_bool__tuple_2_star_int64_t_chpl this_chpl31,
#line 332 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                                                                             int64_t _ln_chpl,
#line 332 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                                                                             int32_t _fn_chpl) {
#line 333 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ArrayViewRankChangeDist_DefaultDist__tuple_2_star_chpl_bool__tuple_2_star_int64_t_chpl ret_chpl = NULL;
#line 333 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 333 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ret_chpl = (this_chpl31)->distInst;
#line 332 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  return ret_chpl;
#line 332 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
}

#line 332 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
/* ArrayViewRankChange.chpl:332 */
#line 332 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
static ArrayViewRankChangeDist_DefaultDist__tuple_3_star_chpl_bool__tuple_3_star_int64_t_chpl dsiMyDist_chpl2(ArrayViewRankChangeDom_2_int64_t_one__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_3_star_chpl_bool__tuple_3_star_int64_t_chpl this_chpl31,
#line 332 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                                                                              int64_t _ln_chpl,
#line 332 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                                                                              int32_t _fn_chpl) {
#line 333 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ArrayViewRankChangeDist_DefaultDist__tuple_3_star_chpl_bool__tuple_3_star_int64_t_chpl ret_chpl = NULL;
#line 333 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 333 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ret_chpl = (this_chpl31)->distInst;
#line 332 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  return ret_chpl;
#line 332 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
}

#line 332 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
/* ArrayViewRankChange.chpl:332 */
#line 332 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
static ArrayViewRankChangeDist_DefaultDist__tuple_4_star_chpl_bool__tuple_4_star_int64_t_chpl dsiMyDist_chpl3(ArrayViewRankChangeDom_2_int64_t_one__tuple_4_star_chpl_bool__tuple_4_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_4_star_chpl_bool__tuple_4_star_int64_t_chpl this_chpl31,
#line 332 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                                                                              int64_t _ln_chpl,
#line 332 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                                                                              int32_t _fn_chpl) {
#line 333 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ArrayViewRankChangeDist_DefaultDist__tuple_4_star_chpl_bool__tuple_4_star_int64_t_chpl ret_chpl = NULL;
#line 333 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 333 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ret_chpl = (this_chpl31)->distInst;
#line 332 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  return ret_chpl;
#line 332 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
}

#line 332 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
/* ArrayViewRankChange.chpl:332 */
#line 332 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
static ArrayViewRankChangeDist_DefaultDist__tuple_4_star_chpl_bool__tuple_4_star_int64_t_chpl dsiMyDist_chpl4(ArrayViewRankChangeDom_3_int64_t_one__tuple_4_star_chpl_bool__tuple_4_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_4_star_chpl_bool__tuple_4_star_int64_t_chpl this_chpl31,
#line 332 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                                                                              int64_t _ln_chpl,
#line 332 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                                                                              int32_t _fn_chpl) {
#line 333 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ArrayViewRankChangeDist_DefaultDist__tuple_4_star_chpl_bool__tuple_4_star_int64_t_chpl ret_chpl = NULL;
#line 333 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 333 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ret_chpl = (this_chpl31)->distInst;
#line 332 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  return ret_chpl;
#line 332 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
}

#line 383 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
/* ArrayViewRankChange.chpl:383 */
#line 383 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
static void dsiDestroyDom_chpl4(ArrayViewRankChangeDom_3_int64_t_one__tuple_4_star_chpl_bool__tuple_4_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_4_star_chpl_bool__tuple_4_star_int64_t_chpl this_chpl31,
#line 383 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                int64_t _ln_chpl,
#line 383 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                int32_t _fn_chpl) {
#line 385 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  DefaultRectangularDom_3_int64_t_one_chpl coerce_tmp_chpl3 = NULL;
#line 387 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  DefaultRectangularDom_4_int64_t_one_chpl coerce_tmp_chpl4 = NULL;
#line 384 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 384 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  if (((RootClass_chpl)((this_chpl31)->upDomInst)) != nil) /* ZLINE: 384 /Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl */
#line 384 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  {
#line 385 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 385 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    coerce_tmp_chpl3 = (this_chpl31)->upDomInst;
#line 385 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    if (((RootClass_chpl)(coerce_tmp_chpl3)) == nil) /* ZLINE: 385 /Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl */
#line 385 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    {
#line 385 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
      nilCheckHalt_chpl(&_str_literal_52_chpl, _ln_chpl, _fn_chpl);
#line 385 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    }
#line 385 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    _delete_dom_chpl(coerce_tmp_chpl3, UINT8(false), _ln_chpl, _fn_chpl);
#line 385 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  }
#line 386 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 386 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  if (((RootClass_chpl)((this_chpl31)->downDomInst)) != nil) /* ZLINE: 386 /Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl */
#line 386 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  {
#line 387 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 387 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    coerce_tmp_chpl4 = (this_chpl31)->downDomInst;
#line 387 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    if (((RootClass_chpl)(coerce_tmp_chpl4)) == nil) /* ZLINE: 387 /Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl */
#line 387 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    {
#line 387 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
      nilCheckHalt_chpl(&_str_literal_52_chpl, _ln_chpl, _fn_chpl);
#line 387 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    }
#line 387 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 387 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    if (((RootClass_chpl)((this_chpl31)->downDomInst)) == nil) /* ZLINE: 387 /Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl */
#line 387 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    {
#line 387 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
      nilCheckHalt_chpl(&_str_literal_52_chpl, _ln_chpl, _fn_chpl);
#line 387 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    }
#line 387 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    _delete_dom_chpl6(coerce_tmp_chpl4, UINT8(false), _ln_chpl, _fn_chpl);
#line 387 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  }
#line 383 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  return;
#line 383 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
}

#line 383 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
/* ArrayViewRankChange.chpl:383 */
#line 383 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
static void dsiDestroyDom_chpl(ArrayViewRankChangeDom_2_int64_t_one__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_3_star_chpl_bool__tuple_3_star_int64_t_chpl this_chpl31,
#line 383 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                               int64_t _ln_chpl,
#line 383 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                               int32_t _fn_chpl) {
#line 385 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  DefaultRectangularDom_2_int64_t_one_chpl coerce_tmp_chpl3 = NULL;
#line 387 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  DefaultRectangularDom_3_int64_t_one_chpl coerce_tmp_chpl4 = NULL;
#line 384 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 384 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  if (((RootClass_chpl)((this_chpl31)->upDomInst)) != nil) /* ZLINE: 384 /Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl */
#line 384 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  {
#line 385 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 385 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    coerce_tmp_chpl3 = (this_chpl31)->upDomInst;
#line 385 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    if (((RootClass_chpl)(coerce_tmp_chpl3)) == nil) /* ZLINE: 385 /Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl */
#line 385 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    {
#line 385 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
      nilCheckHalt_chpl(&_str_literal_52_chpl, _ln_chpl, _fn_chpl);
#line 385 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    }
#line 385 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    _delete_dom_chpl5(coerce_tmp_chpl3, UINT8(false), _ln_chpl, _fn_chpl);
#line 385 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  }
#line 386 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 386 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  if (((RootClass_chpl)((this_chpl31)->downDomInst)) != nil) /* ZLINE: 386 /Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl */
#line 386 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  {
#line 387 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 387 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    coerce_tmp_chpl4 = (this_chpl31)->downDomInst;
#line 387 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    if (((RootClass_chpl)(coerce_tmp_chpl4)) == nil) /* ZLINE: 387 /Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl */
#line 387 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    {
#line 387 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
      nilCheckHalt_chpl(&_str_literal_52_chpl, _ln_chpl, _fn_chpl);
#line 387 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    }
#line 387 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 387 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    if (((RootClass_chpl)((this_chpl31)->downDomInst)) == nil) /* ZLINE: 387 /Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl */
#line 387 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    {
#line 387 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
      nilCheckHalt_chpl(&_str_literal_52_chpl, _ln_chpl, _fn_chpl);
#line 387 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    }
#line 387 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    _delete_dom_chpl(coerce_tmp_chpl4, UINT8(false), _ln_chpl, _fn_chpl);
#line 387 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  }
#line 383 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  return;
#line 383 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
}

#line 383 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
/* ArrayViewRankChange.chpl:383 */
#line 383 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
static void dsiDestroyDom_chpl2(ArrayViewRankChangeDom_1_int64_t_one__tuple_2_star_chpl_bool__tuple_2_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_2_star_chpl_bool__tuple_2_star_int64_t_chpl this_chpl31,
#line 383 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                int64_t _ln_chpl,
#line 383 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                int32_t _fn_chpl) {
#line 385 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  DefaultRectangularDom_1_int64_t_one_chpl coerce_tmp_chpl3 = NULL;
#line 387 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  DefaultRectangularDom_2_int64_t_one_chpl coerce_tmp_chpl4 = NULL;
#line 384 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 384 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  if (((RootClass_chpl)((this_chpl31)->upDomInst)) != nil) /* ZLINE: 384 /Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl */
#line 384 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  {
#line 385 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 385 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    coerce_tmp_chpl3 = (this_chpl31)->upDomInst;
#line 385 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    if (((RootClass_chpl)(coerce_tmp_chpl3)) == nil) /* ZLINE: 385 /Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl */
#line 385 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    {
#line 385 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
      nilCheckHalt_chpl(&_str_literal_52_chpl, _ln_chpl, _fn_chpl);
#line 385 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    }
#line 385 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    _delete_dom_chpl3(coerce_tmp_chpl3, UINT8(false), _ln_chpl, _fn_chpl);
#line 385 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  }
#line 386 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 386 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  if (((RootClass_chpl)((this_chpl31)->downDomInst)) != nil) /* ZLINE: 386 /Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl */
#line 386 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  {
#line 387 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 387 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    coerce_tmp_chpl4 = (this_chpl31)->downDomInst;
#line 387 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    if (((RootClass_chpl)(coerce_tmp_chpl4)) == nil) /* ZLINE: 387 /Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl */
#line 387 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    {
#line 387 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
      nilCheckHalt_chpl(&_str_literal_52_chpl, _ln_chpl, _fn_chpl);
#line 387 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    }
#line 387 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 387 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    if (((RootClass_chpl)((this_chpl31)->downDomInst)) == nil) /* ZLINE: 387 /Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl */
#line 387 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    {
#line 387 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
      nilCheckHalt_chpl(&_str_literal_52_chpl, _ln_chpl, _fn_chpl);
#line 387 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    }
#line 387 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    _delete_dom_chpl5(coerce_tmp_chpl4, UINT8(false), _ln_chpl, _fn_chpl);
#line 387 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  }
#line 383 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  return;
#line 383 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
}

#line 383 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
/* ArrayViewRankChange.chpl:383 */
#line 383 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
static void dsiDestroyDom_chpl3(ArrayViewRankChangeDom_2_int64_t_one__tuple_4_star_chpl_bool__tuple_4_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_4_star_chpl_bool__tuple_4_star_int64_t_chpl this_chpl31,
#line 383 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                int64_t _ln_chpl,
#line 383 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                int32_t _fn_chpl) {
#line 385 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  DefaultRectangularDom_2_int64_t_one_chpl coerce_tmp_chpl3 = NULL;
#line 387 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  DefaultRectangularDom_4_int64_t_one_chpl coerce_tmp_chpl4 = NULL;
#line 384 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 384 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  if (((RootClass_chpl)((this_chpl31)->upDomInst)) != nil) /* ZLINE: 384 /Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl */
#line 384 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  {
#line 385 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 385 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    coerce_tmp_chpl3 = (this_chpl31)->upDomInst;
#line 385 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    if (((RootClass_chpl)(coerce_tmp_chpl3)) == nil) /* ZLINE: 385 /Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl */
#line 385 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    {
#line 385 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
      nilCheckHalt_chpl(&_str_literal_52_chpl, _ln_chpl, _fn_chpl);
#line 385 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    }
#line 385 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    _delete_dom_chpl5(coerce_tmp_chpl3, UINT8(false), _ln_chpl, _fn_chpl);
#line 385 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  }
#line 386 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 386 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  if (((RootClass_chpl)((this_chpl31)->downDomInst)) != nil) /* ZLINE: 386 /Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl */
#line 386 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  {
#line 387 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 387 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    coerce_tmp_chpl4 = (this_chpl31)->downDomInst;
#line 387 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    if (((RootClass_chpl)(coerce_tmp_chpl4)) == nil) /* ZLINE: 387 /Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl */
#line 387 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    {
#line 387 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
      nilCheckHalt_chpl(&_str_literal_52_chpl, _ln_chpl, _fn_chpl);
#line 387 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    }
#line 387 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 387 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    if (((RootClass_chpl)((this_chpl31)->downDomInst)) == nil) /* ZLINE: 387 /Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl */
#line 387 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    {
#line 387 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
      nilCheckHalt_chpl(&_str_literal_52_chpl, _ln_chpl, _fn_chpl);
#line 387 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    }
#line 387 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    _delete_dom_chpl6(coerce_tmp_chpl4, UINT8(false), _ln_chpl, _fn_chpl);
#line 387 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  }
#line 383 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  return;
#line 383 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
}

#line 398 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
/* ArrayViewRankChange.chpl:398 */
#line 398 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
static void dsiFirst_chpl(ArrayViewRankChangeDom_2_int64_t_one__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_3_star_chpl_bool__tuple_3_star_int64_t_chpl this_chpl31,
#line 398 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                          _tuple_2_star_int64_t_chpl * _retArg_chpl,
#line 398 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                          int64_t _ln_chpl,
#line 398 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                          int32_t _fn_chpl) {
#line 398 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  DefaultRectangularDom_2_int64_t_one_chpl coerce_tmp_chpl3 = NULL;
#line 398 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _tuple_2_star_int64_t_chpl call_tmp_chpl6;
#line 398 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _tuple_2_star_int64_t_chpl ret_tmp_chpl;
#line 398 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 398 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  coerce_tmp_chpl3 = (this_chpl31)->upDomInst;
#line 398 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  if (((RootClass_chpl)(coerce_tmp_chpl3)) == nil) /* ZLINE: 398 /Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl */
#line 398 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  {
#line 398 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    nilCheckHalt_chpl(&_str_literal_52_chpl, _ln_chpl, _fn_chpl);
#line 398 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  }
#line 398 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 398 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  if (((RootClass_chpl)((this_chpl31)->upDomInst)) == nil) /* ZLINE: 398 /Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl */
#line 398 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  {
#line 398 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    nilCheckHalt_chpl(&_str_literal_52_chpl, _ln_chpl, _fn_chpl);
#line 398 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  }
#line 398 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 398 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  dsiFirst_chpl2(coerce_tmp_chpl3, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 398 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(call_tmp_chpl6 + INT64(0)) = *(ret_tmp_chpl + INT64(0));
#line 398 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(call_tmp_chpl6 + INT64(1)) = *(ret_tmp_chpl + INT64(1));
#line 398 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(*(_retArg_chpl) + INT64(0)) = *(call_tmp_chpl6 + INT64(0));
#line 398 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(*(_retArg_chpl) + INT64(1)) = *(call_tmp_chpl6 + INT64(1));
#line 398 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  return;
#line 398 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
}

#line 447 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
/* ArrayViewRankChange.chpl:447 */
#line 447 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
static void buildIndexCacheHelper_chpl3(DefaultRectangularArr_4_int64_t_one__real64_int64_t_chpl arr_chpl,
#line 447 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                        ArrayViewRankChangeDom_3_int64_t_one__tuple_4_star_chpl_bool__tuple_4_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_4_star_chpl_bool__tuple_4_star_int64_t_chpl dom_chpl,
#line 447 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                        _tuple_4_star_chpl_bool_chpl * collapsedDim_chpl,
#line 447 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                        _tuple_4_star_int64_t_chpl * idx_chpl,
#line 447 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                        _remoteAccessData__real64_3_int64_t_one_T_chpl * _retArg_chpl,
#line 447 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                        int64_t _ln_chpl,
#line 447 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                        int32_t _fn_chpl) {
#line 458 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _remoteAccessData__real64_4_int64_t_one_F_chpl call_tmp_chpl6;
#line 458 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _remoteAccessData__real64_4_int64_t_one_F_chpl ret_tmp_chpl;
#line 458 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _remoteAccessData__real64_3_int64_t_one_T_chpl call_tmp_chpl7;
#line 458 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _remoteAccessData__real64_3_int64_t_one_T_chpl ret_tmp_chpl2;
#line 458 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(arr_chpl, _ln_chpl, _fn_chpl);
#line 458 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  dsiGetRAD_chpl3(arr_chpl, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 458 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  call_tmp_chpl6 = ret_tmp_chpl;
#line 458 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  toRankChange_chpl3(&call_tmp_chpl6, dom_chpl, collapsedDim_chpl, idx_chpl, &ret_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 458 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  call_tmp_chpl7 = ret_tmp_chpl2;
#line 447 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(_retArg_chpl) = call_tmp_chpl7;
#line 447 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  return;
#line 447 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
}

#line 447 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
/* ArrayViewRankChange.chpl:447 */
#line 447 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
static void buildIndexCacheHelper_chpl4(DefaultRectangularArr_3_int64_t_one__real64_int64_t_chpl arr_chpl,
#line 447 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                        ArrayViewRankChangeDom_2_int64_t_one__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_3_star_chpl_bool__tuple_3_star_int64_t_chpl dom_chpl,
#line 447 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                        _tuple_3_star_chpl_bool_chpl * collapsedDim_chpl,
#line 447 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                        _tuple_3_star_int64_t_chpl * idx_chpl,
#line 447 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                        _remoteAccessData__real64_2_int64_t_one_T_chpl * _retArg_chpl,
#line 447 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                        int64_t _ln_chpl,
#line 447 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                        int32_t _fn_chpl) {
#line 458 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _remoteAccessData__real64_3_int64_t_one_F_chpl call_tmp_chpl6;
#line 458 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _remoteAccessData__real64_3_int64_t_one_F_chpl ret_tmp_chpl;
#line 458 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _remoteAccessData__real64_2_int64_t_one_T_chpl call_tmp_chpl7;
#line 458 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _remoteAccessData__real64_2_int64_t_one_T_chpl ret_tmp_chpl2;
#line 458 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(arr_chpl, _ln_chpl, _fn_chpl);
#line 458 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  dsiGetRAD_chpl(arr_chpl, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 458 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  call_tmp_chpl6 = ret_tmp_chpl;
#line 458 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  toRankChange_chpl(&call_tmp_chpl6, dom_chpl, collapsedDim_chpl, idx_chpl, &ret_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 458 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  call_tmp_chpl7 = ret_tmp_chpl2;
#line 447 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(_retArg_chpl) = call_tmp_chpl7;
#line 447 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  return;
#line 447 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
}

#line 447 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
/* ArrayViewRankChange.chpl:447 */
#line 447 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
static void buildIndexCacheHelper_chpl(DefaultRectangularArr_2_int64_t_one__real64_int64_t_chpl arr_chpl,
#line 447 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                       ArrayViewRankChangeDom_1_int64_t_one__tuple_2_star_chpl_bool__tuple_2_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_2_star_chpl_bool__tuple_2_star_int64_t_chpl dom_chpl,
#line 447 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                       _tuple_2_star_chpl_bool_chpl * collapsedDim_chpl,
#line 447 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                       _tuple_2_star_int64_t_chpl * idx_chpl,
#line 447 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                       _remoteAccessData__real64_1_int64_t_one_T_chpl * _retArg_chpl,
#line 447 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                       int64_t _ln_chpl,
#line 447 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                       int32_t _fn_chpl) {
#line 458 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _remoteAccessData__real64_2_int64_t_one_F_chpl call_tmp_chpl6;
#line 458 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _remoteAccessData__real64_2_int64_t_one_F_chpl ret_tmp_chpl;
#line 458 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _remoteAccessData__real64_1_int64_t_one_T_chpl call_tmp_chpl7;
#line 458 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _remoteAccessData__real64_1_int64_t_one_T_chpl ret_tmp_chpl2;
#line 458 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(arr_chpl, _ln_chpl, _fn_chpl);
#line 458 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  dsiGetRAD_chpl2(arr_chpl, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 458 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  call_tmp_chpl6 = ret_tmp_chpl;
#line 458 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  toRankChange_chpl4(&call_tmp_chpl6, dom_chpl, collapsedDim_chpl, idx_chpl, &ret_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 458 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  call_tmp_chpl7 = ret_tmp_chpl2;
#line 447 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(_retArg_chpl) = call_tmp_chpl7;
#line 447 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  return;
#line 447 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
}

#line 447 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
/* ArrayViewRankChange.chpl:447 */
#line 447 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
static void buildIndexCacheHelper_chpl2(DefaultRectangularArr_4_int64_t_one__real64_int64_t_chpl arr_chpl,
#line 447 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                        ArrayViewRankChangeDom_2_int64_t_one__tuple_4_star_chpl_bool__tuple_4_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_4_star_chpl_bool__tuple_4_star_int64_t_chpl dom_chpl,
#line 447 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                        _tuple_4_star_chpl_bool_chpl * collapsedDim_chpl,
#line 447 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                        _tuple_4_star_int64_t_chpl * idx_chpl,
#line 447 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                        _remoteAccessData__real64_2_int64_t_one_T_chpl * _retArg_chpl,
#line 447 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                        int64_t _ln_chpl,
#line 447 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                        int32_t _fn_chpl) {
#line 458 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _remoteAccessData__real64_4_int64_t_one_F_chpl call_tmp_chpl6;
#line 458 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _remoteAccessData__real64_4_int64_t_one_F_chpl ret_tmp_chpl;
#line 458 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _remoteAccessData__real64_2_int64_t_one_T_chpl call_tmp_chpl7;
#line 458 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _remoteAccessData__real64_2_int64_t_one_T_chpl ret_tmp_chpl2;
#line 458 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(arr_chpl, _ln_chpl, _fn_chpl);
#line 458 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  dsiGetRAD_chpl3(arr_chpl, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 458 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  call_tmp_chpl6 = ret_tmp_chpl;
#line 458 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  toRankChange_chpl2(&call_tmp_chpl6, dom_chpl, collapsedDim_chpl, idx_chpl, &ret_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 458 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  call_tmp_chpl7 = ret_tmp_chpl2;
#line 447 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(_retArg_chpl) = call_tmp_chpl7;
#line 447 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  return;
#line 447 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
}

#line 471 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
/* ArrayViewRankChange.chpl:471 */
#line 471 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
static void chpl__auto_destroy_ArrayViewRankChangeArr8(ArrayViewRankChangeArr__real64_int64_t_ArrayViewRankChangeDom_3_int64_t_one__tuple_4_star_chpl_bool__tuple_4_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_4_star_chpl_bool__tuple_4_star_int64_t_int64_t_DefaultRectangularArr_4_int64_t_one__real64_int64_t__tuple_4_star_chpl_bool__tuple_4_star_int64_t__remoteAccessData__real64_3_int64_t_one_T_F_chpl this_chpl31,
#line 471 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                       int64_t _ln_chpl,
#line 471 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                       int32_t _fn_chpl) {
#line 471 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  deinit_chpl36(((BaseArr_chpl)(((AbsBaseArr__real64_chpl)(this_chpl31)))), _ln_chpl, _fn_chpl);
#line 471 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  return;
#line 471 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
}

#line 471 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
/* ArrayViewRankChange.chpl:471 */
#line 471 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
static void chpl__auto_destroy_ArrayViewRankChangeArr(ArrayViewRankChangeArr__real64_int64_t_ArrayViewRankChangeDom_1_int64_t_one__tuple_2_star_chpl_bool__tuple_2_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_2_star_chpl_bool__tuple_2_star_int64_t_int64_t_DefaultRectangularArr_2_int64_t_one__real64_int64_t__tuple_2_star_chpl_bool__tuple_2_star_int64_t__remoteAccessData__real64_1_int64_t_one_T_F_chpl this_chpl31,
#line 471 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                      int64_t _ln_chpl,
#line 471 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                      int32_t _fn_chpl) {
#line 471 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  deinit_chpl36(((BaseArr_chpl)(((AbsBaseArr__real64_chpl)(this_chpl31)))), _ln_chpl, _fn_chpl);
#line 471 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  return;
#line 471 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
}

#line 471 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
/* ArrayViewRankChange.chpl:471 */
#line 471 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
static void chpl__auto_destroy_ArrayViewRankChangeArr2(ArrayViewRankChangeArr__real64_int64_t_ArrayViewRankChangeDom_2_int64_t_one__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_DefaultRectangularArr_3_int64_t_one__real64_int64_t__tuple_3_star_chpl_bool__tuple_3_star_int64_t__remoteAccessData__real64_2_int64_t_one_T_F_chpl this_chpl31,
#line 471 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                       int64_t _ln_chpl,
#line 471 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                       int32_t _fn_chpl) {
#line 471 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  deinit_chpl36(((BaseArr_chpl)(((AbsBaseArr__real64_chpl)(this_chpl31)))), _ln_chpl, _fn_chpl);
#line 471 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  return;
#line 471 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
}

#line 471 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
/* ArrayViewRankChange.chpl:471 */
#line 471 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
static void chpl__auto_destroy_ArrayViewRankChangeArr3(ArrayViewRankChangeArr__real64_int64_t_ArrayViewRankChangeDom_2_int64_t_one__tuple_4_star_chpl_bool__tuple_4_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_4_star_chpl_bool__tuple_4_star_int64_t_int64_t_DefaultRectangularArr_4_int64_t_one__real64_int64_t__tuple_4_star_chpl_bool__tuple_4_star_int64_t__remoteAccessData__real64_2_int64_t_one_T_F_chpl this_chpl31,
#line 471 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                       int64_t _ln_chpl,
#line 471 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                       int32_t _fn_chpl) {
#line 471 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  deinit_chpl36(((BaseArr_chpl)(((AbsBaseArr__real64_chpl)(this_chpl31)))), _ln_chpl, _fn_chpl);
#line 471 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  return;
#line 471 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
}

#line 471 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
/* ArrayViewRankChange.chpl:471 */
#line 471 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
static void chpl__auto_destroy_ArrayViewRankChangeArr7(ArrayViewRankChangeArr__real64_int64_t_ArrayViewRankChangeDom_3_int64_t_one__tuple_4_star_chpl_bool__tuple_4_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_4_star_chpl_bool__tuple_4_star_int64_t_int64_t_DefaultRectangularArr_4_int64_t_one__real64_int64_t__tuple_4_star_chpl_bool__tuple_4_star_int64_t__remoteAccessData__real64_3_int64_t_one_T_T_chpl this_chpl31,
#line 471 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                       int64_t _ln_chpl,
#line 471 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                       int32_t _fn_chpl) {
#line 471 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  deinit_chpl36(((BaseArr_chpl)(((AbsBaseArr__real64_chpl)(this_chpl31)))), _ln_chpl, _fn_chpl);
#line 471 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  return;
#line 471 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
}

#line 471 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
/* ArrayViewRankChange.chpl:471 */
#line 471 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
static void chpl__auto_destroy_ArrayViewRankChangeArr6(ArrayViewRankChangeArr__real64_int64_t_ArrayViewRankChangeDom_2_int64_t_one__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_DefaultRectangularArr_3_int64_t_one__real64_int64_t__tuple_3_star_chpl_bool__tuple_3_star_int64_t__remoteAccessData__real64_2_int64_t_one_T_T_chpl this_chpl31,
#line 471 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                       int64_t _ln_chpl,
#line 471 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                       int32_t _fn_chpl) {
#line 471 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  deinit_chpl36(((BaseArr_chpl)(((AbsBaseArr__real64_chpl)(this_chpl31)))), _ln_chpl, _fn_chpl);
#line 471 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  return;
#line 471 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
}

#line 471 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
/* ArrayViewRankChange.chpl:471 */
#line 471 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
static void chpl__auto_destroy_ArrayViewRankChangeArr5(ArrayViewRankChangeArr__real64_int64_t_ArrayViewRankChangeDom_1_int64_t_one__tuple_2_star_chpl_bool__tuple_2_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_2_star_chpl_bool__tuple_2_star_int64_t_int64_t_DefaultRectangularArr_2_int64_t_one__real64_int64_t__tuple_2_star_chpl_bool__tuple_2_star_int64_t__remoteAccessData__real64_1_int64_t_one_T_T_chpl this_chpl31,
#line 471 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                       int64_t _ln_chpl,
#line 471 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                       int32_t _fn_chpl) {
#line 471 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  deinit_chpl36(((BaseArr_chpl)(((AbsBaseArr__real64_chpl)(this_chpl31)))), _ln_chpl, _fn_chpl);
#line 471 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  return;
#line 471 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
}

#line 471 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
/* ArrayViewRankChange.chpl:471 */
#line 471 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
static void chpl__auto_destroy_ArrayViewRankChangeArr4(ArrayViewRankChangeArr__real64_int64_t_ArrayViewRankChangeDom_2_int64_t_one__tuple_4_star_chpl_bool__tuple_4_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_4_star_chpl_bool__tuple_4_star_int64_t_int64_t_DefaultRectangularArr_4_int64_t_one__real64_int64_t__tuple_4_star_chpl_bool__tuple_4_star_int64_t__remoteAccessData__real64_2_int64_t_one_T_T_chpl this_chpl31,
#line 471 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                       int64_t _ln_chpl,
#line 471 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                       int32_t _fn_chpl) {
#line 471 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  deinit_chpl36(((BaseArr_chpl)(((AbsBaseArr__real64_chpl)(this_chpl31)))), _ln_chpl, _fn_chpl);
#line 471 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  return;
#line 471 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
}

#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
/* ArrayViewRankChange.chpl:501 */
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
static void init_chpl13(ArrayViewRankChangeArr__real64_int64_t_ArrayViewRankChangeDom_3_int64_t_one__tuple_4_star_chpl_bool__tuple_4_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_4_star_chpl_bool__tuple_4_star_int64_t_int64_t_DefaultRectangularArr_4_int64_t_one__real64_int64_t__tuple_4_star_chpl_bool__tuple_4_star_int64_t__remoteAccessData__real64_3_int64_t_one_T_F_chpl this_chpl31,
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                        int64_t chpl___DomPid_chpl,
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                        ArrayViewRankChangeDom_3_int64_t_one__tuple_4_star_chpl_bool__tuple_4_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_4_star_chpl_bool__tuple_4_star_int64_t_chpl dom_chpl,
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                        int64_t chpl___ArrPid_chpl,
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                        DefaultRectangularArr_4_int64_t_one__real64_int64_t_chpl chpl___ArrInstance_chpl,
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                        _tuple_4_star_chpl_bool_chpl * collapsedDim_chpl,
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                        _tuple_4_star_int64_t_chpl * idx_chpl,
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                        int64_t _ln_chpl,
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                        int32_t _fn_chpl) {
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  AbsBaseArr__real64_chpl super_tmp_chpl = NULL;
#line 505 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  BaseArr_chpl prev_chpl = NULL;
#line 505 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  BaseArr_chpl next_chpl = NULL;
#line 505 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_ddataResizePolicy _resizePolicy_chpl;
#line 510 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _tuple_4_star_chpl_bool_chpl collapsedDim_chpl2;
#line 511 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _tuple_4_star_int64_t_chpl idx_chpl2;
#line 512 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _remoteAccessData__real64_3_int64_t_one_T_chpl call_tmp_chpl6;
#line 512 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _remoteAccessData__real64_3_int64_t_one_T_chpl ret_tmp_chpl;
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  super_tmp_chpl = &((this_chpl31)->super);
#line 505 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  prev_chpl = ((BaseArr_chpl)(nil));
#line 505 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  next_chpl = ((BaseArr_chpl)(nil));
#line 505 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _resizePolicy_chpl = chpl_ddataResizePolicy_normalInit_chpl;
#line 505 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 505 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  init_chpl53(super_tmp_chpl, prev_chpl, next_chpl, INT64(-1), UINT8(false), _resizePolicy_chpl, _ln_chpl, _fn_chpl);
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ((RootClass_chpl)(((AbsBaseArr__real64_chpl)(this_chpl31))))->chpl__cid = chpl__cid_AbsBaseArr__real64_chpl;
#line 506 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 506 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  (this_chpl31)->chpl___DomPid = chpl___DomPid_chpl;
#line 507 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 507 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  (this_chpl31)->dom = dom_chpl;
#line 508 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 508 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  (this_chpl31)->chpl___ArrPid = chpl___ArrPid_chpl;
#line 509 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 509 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  (this_chpl31)->chpl___ArrInstance = chpl___ArrInstance_chpl;
#line 510 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(collapsedDim_chpl2 + INT64(0)) = *(*(collapsedDim_chpl) + INT64(0));
#line 510 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(collapsedDim_chpl2 + INT64(1)) = *(*(collapsedDim_chpl) + INT64(1));
#line 510 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(collapsedDim_chpl2 + INT64(2)) = *(*(collapsedDim_chpl) + INT64(2));
#line 510 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(collapsedDim_chpl2 + INT64(3)) = *(*(collapsedDim_chpl) + INT64(3));
#line 510 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 510 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *((this_chpl31)->collapsedDim + INT64(0)) = *(collapsedDim_chpl2 + INT64(0));
#line 510 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *((this_chpl31)->collapsedDim + INT64(1)) = *(collapsedDim_chpl2 + INT64(1));
#line 510 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *((this_chpl31)->collapsedDim + INT64(2)) = *(collapsedDim_chpl2 + INT64(2));
#line 510 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *((this_chpl31)->collapsedDim + INT64(3)) = *(collapsedDim_chpl2 + INT64(3));
#line 511 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(idx_chpl2 + INT64(0)) = *(*(idx_chpl) + INT64(0));
#line 511 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(idx_chpl2 + INT64(1)) = *(*(idx_chpl) + INT64(1));
#line 511 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(idx_chpl2 + INT64(2)) = *(*(idx_chpl) + INT64(2));
#line 511 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(idx_chpl2 + INT64(3)) = *(*(idx_chpl) + INT64(3));
#line 511 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 511 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *((this_chpl31)->idx + INT64(0)) = *(idx_chpl2 + INT64(0));
#line 511 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *((this_chpl31)->idx + INT64(1)) = *(idx_chpl2 + INT64(1));
#line 511 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *((this_chpl31)->idx + INT64(2)) = *(idx_chpl2 + INT64(2));
#line 511 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *((this_chpl31)->idx + INT64(3)) = *(idx_chpl2 + INT64(3));
#line 512 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  buildIndexCacheHelper_chpl3(chpl___ArrInstance_chpl, dom_chpl, collapsedDim_chpl, idx_chpl, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 512 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  call_tmp_chpl6 = ret_tmp_chpl;
#line 512 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 512 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  (this_chpl31)->indexCache = call_tmp_chpl6;
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ((RootClass_chpl)(this_chpl31))->chpl__cid = chpl__cid_ArrayViewRankChangeArr__real64_int64_t_ArrayViewRankChangeDom_3_int64_t_one__tuple_4_star_chpl_bool__tuple_4_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_4_star_chpl_bool__tuple_4_star_int64_t_int64_t_DefaultRectangularArr_4_int64_t_one__real64_int64_t__tuple_4_star_chpl_bool__tuple_4_star_int64_t__remoteAccessData__real64_3_int64_t_one_T_F_chpl;
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  return;
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
}

#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
/* ArrayViewRankChange.chpl:501 */
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
static void init_chpl8(ArrayViewRankChangeArr__real64_int64_t_ArrayViewRankChangeDom_2_int64_t_one__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_DefaultRectangularArr_3_int64_t_one__real64_int64_t__tuple_3_star_chpl_bool__tuple_3_star_int64_t__remoteAccessData__real64_2_int64_t_one_T_F_chpl this_chpl31,
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                       int64_t chpl___DomPid_chpl,
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                       ArrayViewRankChangeDom_2_int64_t_one__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_3_star_chpl_bool__tuple_3_star_int64_t_chpl dom_chpl,
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                       int64_t chpl___ArrPid_chpl,
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                       DefaultRectangularArr_3_int64_t_one__real64_int64_t_chpl chpl___ArrInstance_chpl,
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                       _tuple_3_star_chpl_bool_chpl * collapsedDim_chpl,
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                       _tuple_3_star_int64_t_chpl * idx_chpl,
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                       int64_t _ln_chpl,
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                       int32_t _fn_chpl) {
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  AbsBaseArr__real64_chpl super_tmp_chpl = NULL;
#line 505 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  BaseArr_chpl prev_chpl = NULL;
#line 505 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  BaseArr_chpl next_chpl = NULL;
#line 505 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_ddataResizePolicy _resizePolicy_chpl;
#line 510 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _tuple_3_star_chpl_bool_chpl collapsedDim_chpl2;
#line 511 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _tuple_3_star_int64_t_chpl idx_chpl2;
#line 512 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _remoteAccessData__real64_2_int64_t_one_T_chpl call_tmp_chpl6;
#line 512 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _remoteAccessData__real64_2_int64_t_one_T_chpl ret_tmp_chpl;
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  super_tmp_chpl = &((this_chpl31)->super);
#line 505 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  prev_chpl = ((BaseArr_chpl)(nil));
#line 505 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  next_chpl = ((BaseArr_chpl)(nil));
#line 505 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _resizePolicy_chpl = chpl_ddataResizePolicy_normalInit_chpl;
#line 505 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 505 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  init_chpl53(super_tmp_chpl, prev_chpl, next_chpl, INT64(-1), UINT8(false), _resizePolicy_chpl, _ln_chpl, _fn_chpl);
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ((RootClass_chpl)(((AbsBaseArr__real64_chpl)(this_chpl31))))->chpl__cid = chpl__cid_AbsBaseArr__real64_chpl;
#line 506 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 506 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  (this_chpl31)->chpl___DomPid = chpl___DomPid_chpl;
#line 507 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 507 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  (this_chpl31)->dom = dom_chpl;
#line 508 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 508 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  (this_chpl31)->chpl___ArrPid = chpl___ArrPid_chpl;
#line 509 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 509 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  (this_chpl31)->chpl___ArrInstance = chpl___ArrInstance_chpl;
#line 510 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(collapsedDim_chpl2 + INT64(0)) = *(*(collapsedDim_chpl) + INT64(0));
#line 510 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(collapsedDim_chpl2 + INT64(1)) = *(*(collapsedDim_chpl) + INT64(1));
#line 510 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(collapsedDim_chpl2 + INT64(2)) = *(*(collapsedDim_chpl) + INT64(2));
#line 510 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 510 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *((this_chpl31)->collapsedDim + INT64(0)) = *(collapsedDim_chpl2 + INT64(0));
#line 510 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *((this_chpl31)->collapsedDim + INT64(1)) = *(collapsedDim_chpl2 + INT64(1));
#line 510 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *((this_chpl31)->collapsedDim + INT64(2)) = *(collapsedDim_chpl2 + INT64(2));
#line 511 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(idx_chpl2 + INT64(0)) = *(*(idx_chpl) + INT64(0));
#line 511 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(idx_chpl2 + INT64(1)) = *(*(idx_chpl) + INT64(1));
#line 511 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(idx_chpl2 + INT64(2)) = *(*(idx_chpl) + INT64(2));
#line 511 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 511 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *((this_chpl31)->idx + INT64(0)) = *(idx_chpl2 + INT64(0));
#line 511 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *((this_chpl31)->idx + INT64(1)) = *(idx_chpl2 + INT64(1));
#line 511 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *((this_chpl31)->idx + INT64(2)) = *(idx_chpl2 + INT64(2));
#line 512 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  buildIndexCacheHelper_chpl4(chpl___ArrInstance_chpl, dom_chpl, collapsedDim_chpl, idx_chpl, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 512 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  call_tmp_chpl6 = ret_tmp_chpl;
#line 512 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 512 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  (this_chpl31)->indexCache = call_tmp_chpl6;
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ((RootClass_chpl)(this_chpl31))->chpl__cid = chpl__cid_ArrayViewRankChangeArr__real64_int64_t_ArrayViewRankChangeDom_2_int64_t_one__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_DefaultRectangularArr_3_int64_t_one__real64_int64_t__tuple_3_star_chpl_bool__tuple_3_star_int64_t__remoteAccessData__real64_2_int64_t_one_T_F_chpl;
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  return;
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
}

#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
/* ArrayViewRankChange.chpl:501 */
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
static void init_chpl10(ArrayViewRankChangeArr__real64_int64_t_ArrayViewRankChangeDom_2_int64_t_one__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_DefaultRectangularArr_3_int64_t_one__real64_int64_t__tuple_3_star_chpl_bool__tuple_3_star_int64_t__remoteAccessData__real64_2_int64_t_one_T_T_chpl this_chpl31,
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                        int64_t chpl___DomPid_chpl,
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                        ArrayViewRankChangeDom_2_int64_t_one__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_3_star_chpl_bool__tuple_3_star_int64_t_chpl dom_chpl,
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                        int64_t chpl___ArrPid_chpl,
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                        DefaultRectangularArr_3_int64_t_one__real64_int64_t_chpl chpl___ArrInstance_chpl,
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                        _tuple_3_star_chpl_bool_chpl * collapsedDim_chpl,
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                        _tuple_3_star_int64_t_chpl * idx_chpl,
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                        int64_t _ln_chpl,
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                        int32_t _fn_chpl) {
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  AbsBaseArr__real64_chpl super_tmp_chpl = NULL;
#line 505 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  BaseArr_chpl prev_chpl = NULL;
#line 505 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  BaseArr_chpl next_chpl = NULL;
#line 505 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_ddataResizePolicy _resizePolicy_chpl;
#line 510 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _tuple_3_star_chpl_bool_chpl collapsedDim_chpl2;
#line 511 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _tuple_3_star_int64_t_chpl idx_chpl2;
#line 512 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _remoteAccessData__real64_2_int64_t_one_T_chpl call_tmp_chpl6;
#line 512 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _remoteAccessData__real64_2_int64_t_one_T_chpl ret_tmp_chpl;
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  super_tmp_chpl = &((this_chpl31)->super);
#line 505 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  prev_chpl = ((BaseArr_chpl)(nil));
#line 505 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  next_chpl = ((BaseArr_chpl)(nil));
#line 505 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _resizePolicy_chpl = chpl_ddataResizePolicy_normalInit_chpl;
#line 505 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 505 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  init_chpl53(super_tmp_chpl, prev_chpl, next_chpl, INT64(-1), UINT8(false), _resizePolicy_chpl, _ln_chpl, _fn_chpl);
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ((RootClass_chpl)(((AbsBaseArr__real64_chpl)(this_chpl31))))->chpl__cid = chpl__cid_AbsBaseArr__real64_chpl;
#line 506 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 506 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  (this_chpl31)->chpl___DomPid = chpl___DomPid_chpl;
#line 507 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 507 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  (this_chpl31)->dom = dom_chpl;
#line 508 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 508 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  (this_chpl31)->chpl___ArrPid = chpl___ArrPid_chpl;
#line 509 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 509 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  (this_chpl31)->chpl___ArrInstance = chpl___ArrInstance_chpl;
#line 510 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(collapsedDim_chpl2 + INT64(0)) = *(*(collapsedDim_chpl) + INT64(0));
#line 510 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(collapsedDim_chpl2 + INT64(1)) = *(*(collapsedDim_chpl) + INT64(1));
#line 510 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(collapsedDim_chpl2 + INT64(2)) = *(*(collapsedDim_chpl) + INT64(2));
#line 510 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 510 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *((this_chpl31)->collapsedDim + INT64(0)) = *(collapsedDim_chpl2 + INT64(0));
#line 510 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *((this_chpl31)->collapsedDim + INT64(1)) = *(collapsedDim_chpl2 + INT64(1));
#line 510 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *((this_chpl31)->collapsedDim + INT64(2)) = *(collapsedDim_chpl2 + INT64(2));
#line 511 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(idx_chpl2 + INT64(0)) = *(*(idx_chpl) + INT64(0));
#line 511 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(idx_chpl2 + INT64(1)) = *(*(idx_chpl) + INT64(1));
#line 511 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(idx_chpl2 + INT64(2)) = *(*(idx_chpl) + INT64(2));
#line 511 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 511 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *((this_chpl31)->idx + INT64(0)) = *(idx_chpl2 + INT64(0));
#line 511 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *((this_chpl31)->idx + INT64(1)) = *(idx_chpl2 + INT64(1));
#line 511 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *((this_chpl31)->idx + INT64(2)) = *(idx_chpl2 + INT64(2));
#line 512 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  buildIndexCacheHelper_chpl4(chpl___ArrInstance_chpl, dom_chpl, collapsedDim_chpl, idx_chpl, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 512 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  call_tmp_chpl6 = ret_tmp_chpl;
#line 512 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 512 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  (this_chpl31)->indexCache = call_tmp_chpl6;
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ((RootClass_chpl)(this_chpl31))->chpl__cid = chpl__cid_ArrayViewRankChangeArr__real64_int64_t_ArrayViewRankChangeDom_2_int64_t_one__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_DefaultRectangularArr_3_int64_t_one__real64_int64_t__tuple_3_star_chpl_bool__tuple_3_star_int64_t__remoteAccessData__real64_2_int64_t_one_T_T_chpl;
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  return;
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
}

#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
/* ArrayViewRankChange.chpl:501 */
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
static void init_chpl12(ArrayViewRankChangeArr__real64_int64_t_ArrayViewRankChangeDom_1_int64_t_one__tuple_2_star_chpl_bool__tuple_2_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_2_star_chpl_bool__tuple_2_star_int64_t_int64_t_DefaultRectangularArr_2_int64_t_one__real64_int64_t__tuple_2_star_chpl_bool__tuple_2_star_int64_t__remoteAccessData__real64_1_int64_t_one_T_F_chpl this_chpl31,
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                        int64_t chpl___DomPid_chpl,
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                        ArrayViewRankChangeDom_1_int64_t_one__tuple_2_star_chpl_bool__tuple_2_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_2_star_chpl_bool__tuple_2_star_int64_t_chpl dom_chpl,
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                        int64_t chpl___ArrPid_chpl,
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                        DefaultRectangularArr_2_int64_t_one__real64_int64_t_chpl chpl___ArrInstance_chpl,
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                        _tuple_2_star_chpl_bool_chpl * collapsedDim_chpl,
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                        _tuple_2_star_int64_t_chpl * idx_chpl,
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                        int64_t _ln_chpl,
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                        int32_t _fn_chpl) {
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  AbsBaseArr__real64_chpl super_tmp_chpl = NULL;
#line 505 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  BaseArr_chpl prev_chpl = NULL;
#line 505 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  BaseArr_chpl next_chpl = NULL;
#line 505 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_ddataResizePolicy _resizePolicy_chpl;
#line 510 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _tuple_2_star_chpl_bool_chpl collapsedDim_chpl2;
#line 511 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _tuple_2_star_int64_t_chpl idx_chpl2;
#line 512 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _remoteAccessData__real64_1_int64_t_one_T_chpl call_tmp_chpl6;
#line 512 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _remoteAccessData__real64_1_int64_t_one_T_chpl ret_tmp_chpl;
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  super_tmp_chpl = &((this_chpl31)->super);
#line 505 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  prev_chpl = ((BaseArr_chpl)(nil));
#line 505 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  next_chpl = ((BaseArr_chpl)(nil));
#line 505 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _resizePolicy_chpl = chpl_ddataResizePolicy_normalInit_chpl;
#line 505 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 505 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  init_chpl53(super_tmp_chpl, prev_chpl, next_chpl, INT64(-1), UINT8(false), _resizePolicy_chpl, _ln_chpl, _fn_chpl);
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ((RootClass_chpl)(((AbsBaseArr__real64_chpl)(this_chpl31))))->chpl__cid = chpl__cid_AbsBaseArr__real64_chpl;
#line 506 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 506 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  (this_chpl31)->chpl___DomPid = chpl___DomPid_chpl;
#line 507 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 507 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  (this_chpl31)->dom = dom_chpl;
#line 508 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 508 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  (this_chpl31)->chpl___ArrPid = chpl___ArrPid_chpl;
#line 509 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 509 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  (this_chpl31)->chpl___ArrInstance = chpl___ArrInstance_chpl;
#line 510 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(collapsedDim_chpl2 + INT64(0)) = *(*(collapsedDim_chpl) + INT64(0));
#line 510 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(collapsedDim_chpl2 + INT64(1)) = *(*(collapsedDim_chpl) + INT64(1));
#line 510 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 510 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *((this_chpl31)->collapsedDim + INT64(0)) = *(collapsedDim_chpl2 + INT64(0));
#line 510 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *((this_chpl31)->collapsedDim + INT64(1)) = *(collapsedDim_chpl2 + INT64(1));
#line 511 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(idx_chpl2 + INT64(0)) = *(*(idx_chpl) + INT64(0));
#line 511 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(idx_chpl2 + INT64(1)) = *(*(idx_chpl) + INT64(1));
#line 511 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 511 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *((this_chpl31)->idx + INT64(0)) = *(idx_chpl2 + INT64(0));
#line 511 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *((this_chpl31)->idx + INT64(1)) = *(idx_chpl2 + INT64(1));
#line 512 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  buildIndexCacheHelper_chpl(chpl___ArrInstance_chpl, dom_chpl, collapsedDim_chpl, idx_chpl, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 512 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  call_tmp_chpl6 = ret_tmp_chpl;
#line 512 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 512 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  (this_chpl31)->indexCache = call_tmp_chpl6;
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ((RootClass_chpl)(this_chpl31))->chpl__cid = chpl__cid_ArrayViewRankChangeArr__real64_int64_t_ArrayViewRankChangeDom_1_int64_t_one__tuple_2_star_chpl_bool__tuple_2_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_2_star_chpl_bool__tuple_2_star_int64_t_int64_t_DefaultRectangularArr_2_int64_t_one__real64_int64_t__tuple_2_star_chpl_bool__tuple_2_star_int64_t__remoteAccessData__real64_1_int64_t_one_T_F_chpl;
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  return;
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
}

#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
/* ArrayViewRankChange.chpl:501 */
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
static void init_chpl11(ArrayViewRankChangeArr__real64_int64_t_ArrayViewRankChangeDom_1_int64_t_one__tuple_2_star_chpl_bool__tuple_2_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_2_star_chpl_bool__tuple_2_star_int64_t_int64_t_DefaultRectangularArr_2_int64_t_one__real64_int64_t__tuple_2_star_chpl_bool__tuple_2_star_int64_t__remoteAccessData__real64_1_int64_t_one_T_T_chpl this_chpl31,
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                        int64_t chpl___DomPid_chpl,
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                        ArrayViewRankChangeDom_1_int64_t_one__tuple_2_star_chpl_bool__tuple_2_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_2_star_chpl_bool__tuple_2_star_int64_t_chpl dom_chpl,
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                        int64_t chpl___ArrPid_chpl,
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                        DefaultRectangularArr_2_int64_t_one__real64_int64_t_chpl chpl___ArrInstance_chpl,
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                        _tuple_2_star_chpl_bool_chpl * collapsedDim_chpl,
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                        _tuple_2_star_int64_t_chpl * idx_chpl,
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                        int64_t _ln_chpl,
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                        int32_t _fn_chpl) {
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  AbsBaseArr__real64_chpl super_tmp_chpl = NULL;
#line 505 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  BaseArr_chpl prev_chpl = NULL;
#line 505 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  BaseArr_chpl next_chpl = NULL;
#line 505 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_ddataResizePolicy _resizePolicy_chpl;
#line 510 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _tuple_2_star_chpl_bool_chpl collapsedDim_chpl2;
#line 511 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _tuple_2_star_int64_t_chpl idx_chpl2;
#line 512 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _remoteAccessData__real64_1_int64_t_one_T_chpl call_tmp_chpl6;
#line 512 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _remoteAccessData__real64_1_int64_t_one_T_chpl ret_tmp_chpl;
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  super_tmp_chpl = &((this_chpl31)->super);
#line 505 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  prev_chpl = ((BaseArr_chpl)(nil));
#line 505 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  next_chpl = ((BaseArr_chpl)(nil));
#line 505 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _resizePolicy_chpl = chpl_ddataResizePolicy_normalInit_chpl;
#line 505 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 505 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  init_chpl53(super_tmp_chpl, prev_chpl, next_chpl, INT64(-1), UINT8(false), _resizePolicy_chpl, _ln_chpl, _fn_chpl);
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ((RootClass_chpl)(((AbsBaseArr__real64_chpl)(this_chpl31))))->chpl__cid = chpl__cid_AbsBaseArr__real64_chpl;
#line 506 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 506 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  (this_chpl31)->chpl___DomPid = chpl___DomPid_chpl;
#line 507 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 507 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  (this_chpl31)->dom = dom_chpl;
#line 508 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 508 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  (this_chpl31)->chpl___ArrPid = chpl___ArrPid_chpl;
#line 509 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 509 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  (this_chpl31)->chpl___ArrInstance = chpl___ArrInstance_chpl;
#line 510 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(collapsedDim_chpl2 + INT64(0)) = *(*(collapsedDim_chpl) + INT64(0));
#line 510 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(collapsedDim_chpl2 + INT64(1)) = *(*(collapsedDim_chpl) + INT64(1));
#line 510 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 510 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *((this_chpl31)->collapsedDim + INT64(0)) = *(collapsedDim_chpl2 + INT64(0));
#line 510 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *((this_chpl31)->collapsedDim + INT64(1)) = *(collapsedDim_chpl2 + INT64(1));
#line 511 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(idx_chpl2 + INT64(0)) = *(*(idx_chpl) + INT64(0));
#line 511 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(idx_chpl2 + INT64(1)) = *(*(idx_chpl) + INT64(1));
#line 511 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 511 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *((this_chpl31)->idx + INT64(0)) = *(idx_chpl2 + INT64(0));
#line 511 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *((this_chpl31)->idx + INT64(1)) = *(idx_chpl2 + INT64(1));
#line 512 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  buildIndexCacheHelper_chpl(chpl___ArrInstance_chpl, dom_chpl, collapsedDim_chpl, idx_chpl, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 512 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  call_tmp_chpl6 = ret_tmp_chpl;
#line 512 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 512 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  (this_chpl31)->indexCache = call_tmp_chpl6;
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ((RootClass_chpl)(this_chpl31))->chpl__cid = chpl__cid_ArrayViewRankChangeArr__real64_int64_t_ArrayViewRankChangeDom_1_int64_t_one__tuple_2_star_chpl_bool__tuple_2_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_2_star_chpl_bool__tuple_2_star_int64_t_int64_t_DefaultRectangularArr_2_int64_t_one__real64_int64_t__tuple_2_star_chpl_bool__tuple_2_star_int64_t__remoteAccessData__real64_1_int64_t_one_T_T_chpl;
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  return;
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
}

#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
/* ArrayViewRankChange.chpl:501 */
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
static void init_chpl9(ArrayViewRankChangeArr__real64_int64_t_ArrayViewRankChangeDom_2_int64_t_one__tuple_4_star_chpl_bool__tuple_4_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_4_star_chpl_bool__tuple_4_star_int64_t_int64_t_DefaultRectangularArr_4_int64_t_one__real64_int64_t__tuple_4_star_chpl_bool__tuple_4_star_int64_t__remoteAccessData__real64_2_int64_t_one_T_F_chpl this_chpl31,
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                       int64_t chpl___DomPid_chpl,
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                       ArrayViewRankChangeDom_2_int64_t_one__tuple_4_star_chpl_bool__tuple_4_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_4_star_chpl_bool__tuple_4_star_int64_t_chpl dom_chpl,
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                       int64_t chpl___ArrPid_chpl,
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                       DefaultRectangularArr_4_int64_t_one__real64_int64_t_chpl chpl___ArrInstance_chpl,
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                       _tuple_4_star_chpl_bool_chpl * collapsedDim_chpl,
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                       _tuple_4_star_int64_t_chpl * idx_chpl,
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                       int64_t _ln_chpl,
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                       int32_t _fn_chpl) {
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  AbsBaseArr__real64_chpl super_tmp_chpl = NULL;
#line 505 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  BaseArr_chpl prev_chpl = NULL;
#line 505 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  BaseArr_chpl next_chpl = NULL;
#line 505 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_ddataResizePolicy _resizePolicy_chpl;
#line 510 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _tuple_4_star_chpl_bool_chpl collapsedDim_chpl2;
#line 511 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _tuple_4_star_int64_t_chpl idx_chpl2;
#line 512 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _remoteAccessData__real64_2_int64_t_one_T_chpl call_tmp_chpl6;
#line 512 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _remoteAccessData__real64_2_int64_t_one_T_chpl ret_tmp_chpl;
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  super_tmp_chpl = &((this_chpl31)->super);
#line 505 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  prev_chpl = ((BaseArr_chpl)(nil));
#line 505 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  next_chpl = ((BaseArr_chpl)(nil));
#line 505 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _resizePolicy_chpl = chpl_ddataResizePolicy_normalInit_chpl;
#line 505 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(super_tmp_chpl, _ln_chpl, _fn_chpl);
#line 505 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  init_chpl53(super_tmp_chpl, prev_chpl, next_chpl, INT64(-1), UINT8(false), _resizePolicy_chpl, _ln_chpl, _fn_chpl);
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ((RootClass_chpl)(((AbsBaseArr__real64_chpl)(this_chpl31))))->chpl__cid = chpl__cid_AbsBaseArr__real64_chpl;
#line 506 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 506 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  (this_chpl31)->chpl___DomPid = chpl___DomPid_chpl;
#line 507 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 507 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  (this_chpl31)->dom = dom_chpl;
#line 508 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 508 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  (this_chpl31)->chpl___ArrPid = chpl___ArrPid_chpl;
#line 509 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 509 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  (this_chpl31)->chpl___ArrInstance = chpl___ArrInstance_chpl;
#line 510 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(collapsedDim_chpl2 + INT64(0)) = *(*(collapsedDim_chpl) + INT64(0));
#line 510 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(collapsedDim_chpl2 + INT64(1)) = *(*(collapsedDim_chpl) + INT64(1));
#line 510 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(collapsedDim_chpl2 + INT64(2)) = *(*(collapsedDim_chpl) + INT64(2));
#line 510 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(collapsedDim_chpl2 + INT64(3)) = *(*(collapsedDim_chpl) + INT64(3));
#line 510 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 510 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *((this_chpl31)->collapsedDim + INT64(0)) = *(collapsedDim_chpl2 + INT64(0));
#line 510 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *((this_chpl31)->collapsedDim + INT64(1)) = *(collapsedDim_chpl2 + INT64(1));
#line 510 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *((this_chpl31)->collapsedDim + INT64(2)) = *(collapsedDim_chpl2 + INT64(2));
#line 510 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *((this_chpl31)->collapsedDim + INT64(3)) = *(collapsedDim_chpl2 + INT64(3));
#line 511 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(idx_chpl2 + INT64(0)) = *(*(idx_chpl) + INT64(0));
#line 511 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(idx_chpl2 + INT64(1)) = *(*(idx_chpl) + INT64(1));
#line 511 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(idx_chpl2 + INT64(2)) = *(*(idx_chpl) + INT64(2));
#line 511 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(idx_chpl2 + INT64(3)) = *(*(idx_chpl) + INT64(3));
#line 511 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 511 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *((this_chpl31)->idx + INT64(0)) = *(idx_chpl2 + INT64(0));
#line 511 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *((this_chpl31)->idx + INT64(1)) = *(idx_chpl2 + INT64(1));
#line 511 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *((this_chpl31)->idx + INT64(2)) = *(idx_chpl2 + INT64(2));
#line 511 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *((this_chpl31)->idx + INT64(3)) = *(idx_chpl2 + INT64(3));
#line 512 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  buildIndexCacheHelper_chpl2(chpl___ArrInstance_chpl, dom_chpl, collapsedDim_chpl, idx_chpl, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 512 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  call_tmp_chpl6 = ret_tmp_chpl;
#line 512 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 512 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  (this_chpl31)->indexCache = call_tmp_chpl6;
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ((RootClass_chpl)(this_chpl31))->chpl__cid = chpl__cid_ArrayViewRankChangeArr__real64_int64_t_ArrayViewRankChangeDom_2_int64_t_one__tuple_4_star_chpl_bool__tuple_4_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_4_star_chpl_bool__tuple_4_star_int64_t_int64_t_DefaultRectangularArr_4_int64_t_one__real64_int64_t__tuple_4_star_chpl_bool__tuple_4_star_int64_t__remoteAccessData__real64_2_int64_t_one_T_F_chpl;
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  return;
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
}

#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
/* ArrayViewRankChange.chpl:501 */
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
static ArrayViewRankChangeArr__real64_int64_t_ArrayViewRankChangeDom_1_int64_t_one__tuple_2_star_chpl_bool__tuple_2_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_2_star_chpl_bool__tuple_2_star_int64_t_int64_t_DefaultRectangularArr_2_int64_t_one__real64_int64_t__tuple_2_star_chpl_bool__tuple_2_star_int64_t__remoteAccessData__real64_1_int64_t_one_T_T_chpl _new_chpl10(int64_t chpl___DomPid_chpl,
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                                                                                                                                                                                                                                                                                                                                                                  ArrayViewRankChangeDom_1_int64_t_one__tuple_2_star_chpl_bool__tuple_2_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_2_star_chpl_bool__tuple_2_star_int64_t_chpl dom_chpl,
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                                                                                                                                                                                                                                                                                                                                                                  int64_t chpl___ArrPid_chpl,
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                                                                                                                                                                                                                                                                                                                                                                  DefaultRectangularArr_2_int64_t_one__real64_int64_t_chpl chpl___ArrInstance_chpl,
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                                                                                                                                                                                                                                                                                                                                                                  _tuple_2_star_chpl_bool_chpl * collapsedDim_chpl,
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                                                                                                                                                                                                                                                                                                                                                                  _tuple_2_star_int64_t_chpl * idx_chpl,
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                                                                                                                                                                                                                                                                                                                                                                  int64_t _ln_chpl,
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                                                                                                                                                                                                                                                                                                                                                                  int32_t _fn_chpl) {
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ArrayViewRankChangeArr__real64_int64_t_ArrayViewRankChangeDom_1_int64_t_one__tuple_2_star_chpl_bool__tuple_2_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_2_star_chpl_bool__tuple_2_star_int64_t_int64_t_DefaultRectangularArr_2_int64_t_one__real64_int64_t__tuple_2_star_chpl_bool__tuple_2_star_int64_t__remoteAccessData__real64_1_int64_t_one_T_T_chpl initTemp_chpl = NULL;
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  c_ptr_void_chpl cast_tmp_chpl = NULL;
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ArrayViewRankChangeArr__real64_int64_t_ArrayViewRankChangeDom_1_int64_t_one__tuple_2_star_chpl_bool__tuple_2_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_2_star_chpl_bool__tuple_2_star_int64_t_int64_t_DefaultRectangularArr_2_int64_t_one__real64_int64_t__tuple_2_star_chpl_bool__tuple_2_star_int64_t__remoteAccessData__real64_1_int64_t_one_T_T_chpl tmp_chpl = NULL;
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  cast_tmp_chpl = chpl_here_alloc(sizeof(chpl_ArrayViewRankChangeArr__real64_int64_t_ArrayViewRankChangeDom_1_int64_t_one__tuple_2_star_chpl_bool__tuple_2_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_2_star_chpl_bool__tuple_2_star_int64_t_int64_t_DefaultRectangularArr_2_int64_t_one__real64_int64_t__tuple_2_star_chpl_bool__tuple_2_star_int64_t__remoteAccessData__real64_1_int64_t_one_T_T_chpl_object), INT16(299), _ln_chpl, _fn_chpl);
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  initTemp_chpl = ((ArrayViewRankChangeArr__real64_int64_t_ArrayViewRankChangeDom_1_int64_t_one__tuple_2_star_chpl_bool__tuple_2_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_2_star_chpl_bool__tuple_2_star_int64_t_int64_t_DefaultRectangularArr_2_int64_t_one__real64_int64_t__tuple_2_star_chpl_bool__tuple_2_star_int64_t__remoteAccessData__real64_1_int64_t_one_T_T_chpl)(cast_tmp_chpl));
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ((RootClass_chpl)(initTemp_chpl))->chpl__cid = chpl__cid_ArrayViewRankChangeArr__real64_int64_t_ArrayViewRankChangeDom_1_int64_t_one__tuple_2_star_chpl_bool__tuple_2_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_2_star_chpl_bool__tuple_2_star_int64_t_int64_t_DefaultRectangularArr_2_int64_t_one__real64_int64_t__tuple_2_star_chpl_bool__tuple_2_star_int64_t__remoteAccessData__real64_1_int64_t_one_T_T_chpl;
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(initTemp_chpl, _ln_chpl, _fn_chpl);
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  init_chpl11(initTemp_chpl, chpl___DomPid_chpl, dom_chpl, chpl___ArrPid_chpl, chpl___ArrInstance_chpl, collapsedDim_chpl, idx_chpl, _ln_chpl, _fn_chpl);
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  tmp_chpl = initTemp_chpl;
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  return tmp_chpl;
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
}

#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
/* ArrayViewRankChange.chpl:501 */
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
static ArrayViewRankChangeArr__real64_int64_t_ArrayViewRankChangeDom_1_int64_t_one__tuple_2_star_chpl_bool__tuple_2_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_2_star_chpl_bool__tuple_2_star_int64_t_int64_t_DefaultRectangularArr_2_int64_t_one__real64_int64_t__tuple_2_star_chpl_bool__tuple_2_star_int64_t__remoteAccessData__real64_1_int64_t_one_T_F_chpl _new_chpl12(int64_t chpl___DomPid_chpl,
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                                                                                                                                                                                                                                                                                                                                                                  ArrayViewRankChangeDom_1_int64_t_one__tuple_2_star_chpl_bool__tuple_2_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_2_star_chpl_bool__tuple_2_star_int64_t_chpl dom_chpl,
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                                                                                                                                                                                                                                                                                                                                                                  int64_t chpl___ArrPid_chpl,
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                                                                                                                                                                                                                                                                                                                                                                  DefaultRectangularArr_2_int64_t_one__real64_int64_t_chpl chpl___ArrInstance_chpl,
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                                                                                                                                                                                                                                                                                                                                                                  _tuple_2_star_chpl_bool_chpl * collapsedDim_chpl,
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                                                                                                                                                                                                                                                                                                                                                                  _tuple_2_star_int64_t_chpl * idx_chpl,
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                                                                                                                                                                                                                                                                                                                                                                  int64_t _ln_chpl,
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                                                                                                                                                                                                                                                                                                                                                                  int32_t _fn_chpl) {
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ArrayViewRankChangeArr__real64_int64_t_ArrayViewRankChangeDom_1_int64_t_one__tuple_2_star_chpl_bool__tuple_2_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_2_star_chpl_bool__tuple_2_star_int64_t_int64_t_DefaultRectangularArr_2_int64_t_one__real64_int64_t__tuple_2_star_chpl_bool__tuple_2_star_int64_t__remoteAccessData__real64_1_int64_t_one_T_F_chpl initTemp_chpl = NULL;
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  c_ptr_void_chpl cast_tmp_chpl = NULL;
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ArrayViewRankChangeArr__real64_int64_t_ArrayViewRankChangeDom_1_int64_t_one__tuple_2_star_chpl_bool__tuple_2_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_2_star_chpl_bool__tuple_2_star_int64_t_int64_t_DefaultRectangularArr_2_int64_t_one__real64_int64_t__tuple_2_star_chpl_bool__tuple_2_star_int64_t__remoteAccessData__real64_1_int64_t_one_T_F_chpl tmp_chpl = NULL;
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  cast_tmp_chpl = chpl_here_alloc(sizeof(chpl_ArrayViewRankChangeArr__real64_int64_t_ArrayViewRankChangeDom_1_int64_t_one__tuple_2_star_chpl_bool__tuple_2_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_2_star_chpl_bool__tuple_2_star_int64_t_int64_t_DefaultRectangularArr_2_int64_t_one__real64_int64_t__tuple_2_star_chpl_bool__tuple_2_star_int64_t__remoteAccessData__real64_1_int64_t_one_T_F_chpl_object), INT16(298), _ln_chpl, _fn_chpl);
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  initTemp_chpl = ((ArrayViewRankChangeArr__real64_int64_t_ArrayViewRankChangeDom_1_int64_t_one__tuple_2_star_chpl_bool__tuple_2_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_2_star_chpl_bool__tuple_2_star_int64_t_int64_t_DefaultRectangularArr_2_int64_t_one__real64_int64_t__tuple_2_star_chpl_bool__tuple_2_star_int64_t__remoteAccessData__real64_1_int64_t_one_T_F_chpl)(cast_tmp_chpl));
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ((RootClass_chpl)(initTemp_chpl))->chpl__cid = chpl__cid_ArrayViewRankChangeArr__real64_int64_t_ArrayViewRankChangeDom_1_int64_t_one__tuple_2_star_chpl_bool__tuple_2_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_2_star_chpl_bool__tuple_2_star_int64_t_int64_t_DefaultRectangularArr_2_int64_t_one__real64_int64_t__tuple_2_star_chpl_bool__tuple_2_star_int64_t__remoteAccessData__real64_1_int64_t_one_T_F_chpl;
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(initTemp_chpl, _ln_chpl, _fn_chpl);
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  init_chpl12(initTemp_chpl, chpl___DomPid_chpl, dom_chpl, chpl___ArrPid_chpl, chpl___ArrInstance_chpl, collapsedDim_chpl, idx_chpl, _ln_chpl, _fn_chpl);
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  tmp_chpl = initTemp_chpl;
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  return tmp_chpl;
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
}

#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
/* ArrayViewRankChange.chpl:501 */
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
static ArrayViewRankChangeArr__real64_int64_t_ArrayViewRankChangeDom_2_int64_t_one__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_DefaultRectangularArr_3_int64_t_one__real64_int64_t__tuple_3_star_chpl_bool__tuple_3_star_int64_t__remoteAccessData__real64_2_int64_t_one_T_T_chpl _new_chpl9(int64_t chpl___DomPid_chpl,
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                                                                                                                                                                                                                                                                                                                                                                 ArrayViewRankChangeDom_2_int64_t_one__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_3_star_chpl_bool__tuple_3_star_int64_t_chpl dom_chpl,
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                                                                                                                                                                                                                                                                                                                                                                 int64_t chpl___ArrPid_chpl,
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                                                                                                                                                                                                                                                                                                                                                                 DefaultRectangularArr_3_int64_t_one__real64_int64_t_chpl chpl___ArrInstance_chpl,
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                                                                                                                                                                                                                                                                                                                                                                 _tuple_3_star_chpl_bool_chpl * collapsedDim_chpl,
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                                                                                                                                                                                                                                                                                                                                                                 _tuple_3_star_int64_t_chpl * idx_chpl,
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                                                                                                                                                                                                                                                                                                                                                                 int64_t _ln_chpl,
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                                                                                                                                                                                                                                                                                                                                                                 int32_t _fn_chpl) {
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ArrayViewRankChangeArr__real64_int64_t_ArrayViewRankChangeDom_2_int64_t_one__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_DefaultRectangularArr_3_int64_t_one__real64_int64_t__tuple_3_star_chpl_bool__tuple_3_star_int64_t__remoteAccessData__real64_2_int64_t_one_T_T_chpl initTemp_chpl = NULL;
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  c_ptr_void_chpl cast_tmp_chpl = NULL;
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ArrayViewRankChangeArr__real64_int64_t_ArrayViewRankChangeDom_2_int64_t_one__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_DefaultRectangularArr_3_int64_t_one__real64_int64_t__tuple_3_star_chpl_bool__tuple_3_star_int64_t__remoteAccessData__real64_2_int64_t_one_T_T_chpl tmp_chpl = NULL;
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  cast_tmp_chpl = chpl_here_alloc(sizeof(chpl_ArrayViewRankChangeArr__real64_int64_t_ArrayViewRankChangeDom_2_int64_t_one__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_DefaultRectangularArr_3_int64_t_one__real64_int64_t__tuple_3_star_chpl_bool__tuple_3_star_int64_t__remoteAccessData__real64_2_int64_t_one_T_T_chpl_object), INT16(266), _ln_chpl, _fn_chpl);
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  initTemp_chpl = ((ArrayViewRankChangeArr__real64_int64_t_ArrayViewRankChangeDom_2_int64_t_one__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_DefaultRectangularArr_3_int64_t_one__real64_int64_t__tuple_3_star_chpl_bool__tuple_3_star_int64_t__remoteAccessData__real64_2_int64_t_one_T_T_chpl)(cast_tmp_chpl));
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ((RootClass_chpl)(initTemp_chpl))->chpl__cid = chpl__cid_ArrayViewRankChangeArr__real64_int64_t_ArrayViewRankChangeDom_2_int64_t_one__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_DefaultRectangularArr_3_int64_t_one__real64_int64_t__tuple_3_star_chpl_bool__tuple_3_star_int64_t__remoteAccessData__real64_2_int64_t_one_T_T_chpl;
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(initTemp_chpl, _ln_chpl, _fn_chpl);
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  init_chpl10(initTemp_chpl, chpl___DomPid_chpl, dom_chpl, chpl___ArrPid_chpl, chpl___ArrInstance_chpl, collapsedDim_chpl, idx_chpl, _ln_chpl, _fn_chpl);
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  tmp_chpl = initTemp_chpl;
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  return tmp_chpl;
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
}

#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
/* ArrayViewRankChange.chpl:501 */
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
static ArrayViewRankChangeArr__real64_int64_t_ArrayViewRankChangeDom_2_int64_t_one__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_DefaultRectangularArr_3_int64_t_one__real64_int64_t__tuple_3_star_chpl_bool__tuple_3_star_int64_t__remoteAccessData__real64_2_int64_t_one_T_F_chpl _new_chpl8(int64_t chpl___DomPid_chpl,
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                                                                                                                                                                                                                                                                                                                                                                 ArrayViewRankChangeDom_2_int64_t_one__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_3_star_chpl_bool__tuple_3_star_int64_t_chpl dom_chpl,
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                                                                                                                                                                                                                                                                                                                                                                 int64_t chpl___ArrPid_chpl,
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                                                                                                                                                                                                                                                                                                                                                                 DefaultRectangularArr_3_int64_t_one__real64_int64_t_chpl chpl___ArrInstance_chpl,
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                                                                                                                                                                                                                                                                                                                                                                 _tuple_3_star_chpl_bool_chpl * collapsedDim_chpl,
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                                                                                                                                                                                                                                                                                                                                                                 _tuple_3_star_int64_t_chpl * idx_chpl,
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                                                                                                                                                                                                                                                                                                                                                                 int64_t _ln_chpl,
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                                                                                                                                                                                                                                                                                                                                                                 int32_t _fn_chpl) {
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ArrayViewRankChangeArr__real64_int64_t_ArrayViewRankChangeDom_2_int64_t_one__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_DefaultRectangularArr_3_int64_t_one__real64_int64_t__tuple_3_star_chpl_bool__tuple_3_star_int64_t__remoteAccessData__real64_2_int64_t_one_T_F_chpl initTemp_chpl = NULL;
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  c_ptr_void_chpl cast_tmp_chpl = NULL;
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ArrayViewRankChangeArr__real64_int64_t_ArrayViewRankChangeDom_2_int64_t_one__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_DefaultRectangularArr_3_int64_t_one__real64_int64_t__tuple_3_star_chpl_bool__tuple_3_star_int64_t__remoteAccessData__real64_2_int64_t_one_T_F_chpl tmp_chpl = NULL;
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  cast_tmp_chpl = chpl_here_alloc(sizeof(chpl_ArrayViewRankChangeArr__real64_int64_t_ArrayViewRankChangeDom_2_int64_t_one__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_DefaultRectangularArr_3_int64_t_one__real64_int64_t__tuple_3_star_chpl_bool__tuple_3_star_int64_t__remoteAccessData__real64_2_int64_t_one_T_F_chpl_object), INT16(257), _ln_chpl, _fn_chpl);
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  initTemp_chpl = ((ArrayViewRankChangeArr__real64_int64_t_ArrayViewRankChangeDom_2_int64_t_one__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_DefaultRectangularArr_3_int64_t_one__real64_int64_t__tuple_3_star_chpl_bool__tuple_3_star_int64_t__remoteAccessData__real64_2_int64_t_one_T_F_chpl)(cast_tmp_chpl));
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ((RootClass_chpl)(initTemp_chpl))->chpl__cid = chpl__cid_ArrayViewRankChangeArr__real64_int64_t_ArrayViewRankChangeDom_2_int64_t_one__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_DefaultRectangularArr_3_int64_t_one__real64_int64_t__tuple_3_star_chpl_bool__tuple_3_star_int64_t__remoteAccessData__real64_2_int64_t_one_T_F_chpl;
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(initTemp_chpl, _ln_chpl, _fn_chpl);
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  init_chpl8(initTemp_chpl, chpl___DomPid_chpl, dom_chpl, chpl___ArrPid_chpl, chpl___ArrInstance_chpl, collapsedDim_chpl, idx_chpl, _ln_chpl, _fn_chpl);
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  tmp_chpl = initTemp_chpl;
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  return tmp_chpl;
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
}

#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
/* ArrayViewRankChange.chpl:501 */
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
static ArrayViewRankChangeArr__real64_int64_t_ArrayViewRankChangeDom_3_int64_t_one__tuple_4_star_chpl_bool__tuple_4_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_4_star_chpl_bool__tuple_4_star_int64_t_int64_t_DefaultRectangularArr_4_int64_t_one__real64_int64_t__tuple_4_star_chpl_bool__tuple_4_star_int64_t__remoteAccessData__real64_3_int64_t_one_T_F_chpl _new_chpl13(int64_t chpl___DomPid_chpl,
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                                                                                                                                                                                                                                                                                                                                                                  ArrayViewRankChangeDom_3_int64_t_one__tuple_4_star_chpl_bool__tuple_4_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_4_star_chpl_bool__tuple_4_star_int64_t_chpl dom_chpl,
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                                                                                                                                                                                                                                                                                                                                                                  int64_t chpl___ArrPid_chpl,
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                                                                                                                                                                                                                                                                                                                                                                  DefaultRectangularArr_4_int64_t_one__real64_int64_t_chpl chpl___ArrInstance_chpl,
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                                                                                                                                                                                                                                                                                                                                                                  _tuple_4_star_chpl_bool_chpl * collapsedDim_chpl,
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                                                                                                                                                                                                                                                                                                                                                                  _tuple_4_star_int64_t_chpl * idx_chpl,
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                                                                                                                                                                                                                                                                                                                                                                  int64_t _ln_chpl,
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                                                                                                                                                                                                                                                                                                                                                                  int32_t _fn_chpl) {
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ArrayViewRankChangeArr__real64_int64_t_ArrayViewRankChangeDom_3_int64_t_one__tuple_4_star_chpl_bool__tuple_4_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_4_star_chpl_bool__tuple_4_star_int64_t_int64_t_DefaultRectangularArr_4_int64_t_one__real64_int64_t__tuple_4_star_chpl_bool__tuple_4_star_int64_t__remoteAccessData__real64_3_int64_t_one_T_F_chpl initTemp_chpl = NULL;
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  c_ptr_void_chpl cast_tmp_chpl = NULL;
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ArrayViewRankChangeArr__real64_int64_t_ArrayViewRankChangeDom_3_int64_t_one__tuple_4_star_chpl_bool__tuple_4_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_4_star_chpl_bool__tuple_4_star_int64_t_int64_t_DefaultRectangularArr_4_int64_t_one__real64_int64_t__tuple_4_star_chpl_bool__tuple_4_star_int64_t__remoteAccessData__real64_3_int64_t_one_T_F_chpl tmp_chpl = NULL;
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  cast_tmp_chpl = chpl_here_alloc(sizeof(chpl_ArrayViewRankChangeArr__real64_int64_t_ArrayViewRankChangeDom_3_int64_t_one__tuple_4_star_chpl_bool__tuple_4_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_4_star_chpl_bool__tuple_4_star_int64_t_int64_t_DefaultRectangularArr_4_int64_t_one__real64_int64_t__tuple_4_star_chpl_bool__tuple_4_star_int64_t__remoteAccessData__real64_3_int64_t_one_T_F_chpl_object), INT16(237), _ln_chpl, _fn_chpl);
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  initTemp_chpl = ((ArrayViewRankChangeArr__real64_int64_t_ArrayViewRankChangeDom_3_int64_t_one__tuple_4_star_chpl_bool__tuple_4_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_4_star_chpl_bool__tuple_4_star_int64_t_int64_t_DefaultRectangularArr_4_int64_t_one__real64_int64_t__tuple_4_star_chpl_bool__tuple_4_star_int64_t__remoteAccessData__real64_3_int64_t_one_T_F_chpl)(cast_tmp_chpl));
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ((RootClass_chpl)(initTemp_chpl))->chpl__cid = chpl__cid_ArrayViewRankChangeArr__real64_int64_t_ArrayViewRankChangeDom_3_int64_t_one__tuple_4_star_chpl_bool__tuple_4_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_4_star_chpl_bool__tuple_4_star_int64_t_int64_t_DefaultRectangularArr_4_int64_t_one__real64_int64_t__tuple_4_star_chpl_bool__tuple_4_star_int64_t__remoteAccessData__real64_3_int64_t_one_T_F_chpl;
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(initTemp_chpl, _ln_chpl, _fn_chpl);
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  init_chpl13(initTemp_chpl, chpl___DomPid_chpl, dom_chpl, chpl___ArrPid_chpl, chpl___ArrInstance_chpl, collapsedDim_chpl, idx_chpl, _ln_chpl, _fn_chpl);
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  tmp_chpl = initTemp_chpl;
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  return tmp_chpl;
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
}

#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
/* ArrayViewRankChange.chpl:501 */
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
static ArrayViewRankChangeArr__real64_int64_t_ArrayViewRankChangeDom_2_int64_t_one__tuple_4_star_chpl_bool__tuple_4_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_4_star_chpl_bool__tuple_4_star_int64_t_int64_t_DefaultRectangularArr_4_int64_t_one__real64_int64_t__tuple_4_star_chpl_bool__tuple_4_star_int64_t__remoteAccessData__real64_2_int64_t_one_T_F_chpl _new_chpl11(int64_t chpl___DomPid_chpl,
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                                                                                                                                                                                                                                                                                                                                                                  ArrayViewRankChangeDom_2_int64_t_one__tuple_4_star_chpl_bool__tuple_4_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_4_star_chpl_bool__tuple_4_star_int64_t_chpl dom_chpl,
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                                                                                                                                                                                                                                                                                                                                                                  int64_t chpl___ArrPid_chpl,
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                                                                                                                                                                                                                                                                                                                                                                  DefaultRectangularArr_4_int64_t_one__real64_int64_t_chpl chpl___ArrInstance_chpl,
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                                                                                                                                                                                                                                                                                                                                                                  _tuple_4_star_chpl_bool_chpl * collapsedDim_chpl,
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                                                                                                                                                                                                                                                                                                                                                                  _tuple_4_star_int64_t_chpl * idx_chpl,
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                                                                                                                                                                                                                                                                                                                                                                  int64_t _ln_chpl,
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                                                                                                                                                                                                                                                                                                                                                                  int32_t _fn_chpl) {
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ArrayViewRankChangeArr__real64_int64_t_ArrayViewRankChangeDom_2_int64_t_one__tuple_4_star_chpl_bool__tuple_4_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_4_star_chpl_bool__tuple_4_star_int64_t_int64_t_DefaultRectangularArr_4_int64_t_one__real64_int64_t__tuple_4_star_chpl_bool__tuple_4_star_int64_t__remoteAccessData__real64_2_int64_t_one_T_F_chpl initTemp_chpl = NULL;
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  c_ptr_void_chpl cast_tmp_chpl = NULL;
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ArrayViewRankChangeArr__real64_int64_t_ArrayViewRankChangeDom_2_int64_t_one__tuple_4_star_chpl_bool__tuple_4_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_4_star_chpl_bool__tuple_4_star_int64_t_int64_t_DefaultRectangularArr_4_int64_t_one__real64_int64_t__tuple_4_star_chpl_bool__tuple_4_star_int64_t__remoteAccessData__real64_2_int64_t_one_T_F_chpl tmp_chpl = NULL;
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  cast_tmp_chpl = chpl_here_alloc(sizeof(chpl_ArrayViewRankChangeArr__real64_int64_t_ArrayViewRankChangeDom_2_int64_t_one__tuple_4_star_chpl_bool__tuple_4_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_4_star_chpl_bool__tuple_4_star_int64_t_int64_t_DefaultRectangularArr_4_int64_t_one__real64_int64_t__tuple_4_star_chpl_bool__tuple_4_star_int64_t__remoteAccessData__real64_2_int64_t_one_T_F_chpl_object), INT16(367), _ln_chpl, _fn_chpl);
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  initTemp_chpl = ((ArrayViewRankChangeArr__real64_int64_t_ArrayViewRankChangeDom_2_int64_t_one__tuple_4_star_chpl_bool__tuple_4_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_4_star_chpl_bool__tuple_4_star_int64_t_int64_t_DefaultRectangularArr_4_int64_t_one__real64_int64_t__tuple_4_star_chpl_bool__tuple_4_star_int64_t__remoteAccessData__real64_2_int64_t_one_T_F_chpl)(cast_tmp_chpl));
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ((RootClass_chpl)(initTemp_chpl))->chpl__cid = chpl__cid_ArrayViewRankChangeArr__real64_int64_t_ArrayViewRankChangeDom_2_int64_t_one__tuple_4_star_chpl_bool__tuple_4_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_4_star_chpl_bool__tuple_4_star_int64_t_int64_t_DefaultRectangularArr_4_int64_t_one__real64_int64_t__tuple_4_star_chpl_bool__tuple_4_star_int64_t__remoteAccessData__real64_2_int64_t_one_T_F_chpl;
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(initTemp_chpl, _ln_chpl, _fn_chpl);
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  init_chpl9(initTemp_chpl, chpl___DomPid_chpl, dom_chpl, chpl___ArrPid_chpl, chpl___ArrInstance_chpl, collapsedDim_chpl, idx_chpl, _ln_chpl, _fn_chpl);
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  tmp_chpl = initTemp_chpl;
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  return tmp_chpl;
#line 501 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
}

#line 774 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
/* ArrayViewRankChange.chpl:774 */
#line 774 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
static ArrayViewRankChangeDom_2_int64_t_one__tuple_4_star_chpl_bool__tuple_4_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_4_star_chpl_bool__tuple_4_star_int64_t_chpl dsiGetBaseDom_chpl3(ArrayViewRankChangeArr__real64_int64_t_ArrayViewRankChangeDom_2_int64_t_one__tuple_4_star_chpl_bool__tuple_4_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_4_star_chpl_bool__tuple_4_star_int64_t_int64_t_DefaultRectangularArr_4_int64_t_one__real64_int64_t__tuple_4_star_chpl_bool__tuple_4_star_int64_t__remoteAccessData__real64_2_int64_t_one_T_F_chpl this_chpl31,
#line 774 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                                                                                                                                                                             int64_t _ln_chpl,
#line 774 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                                                                                                                                                                             int32_t _fn_chpl) {
#line 774 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ArrayViewRankChangeDom_2_int64_t_one__tuple_4_star_chpl_bool__tuple_4_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_4_star_chpl_bool__tuple_4_star_int64_t_chpl ret_chpl = NULL;
#line 775 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 775 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ret_chpl = (this_chpl31)->dom;
#line 774 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  return ret_chpl;
#line 774 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
}

#line 774 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
/* ArrayViewRankChange.chpl:774 */
#line 774 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
static ArrayViewRankChangeDom_1_int64_t_one__tuple_2_star_chpl_bool__tuple_2_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_2_star_chpl_bool__tuple_2_star_int64_t_chpl dsiGetBaseDom_chpl4(ArrayViewRankChangeArr__real64_int64_t_ArrayViewRankChangeDom_1_int64_t_one__tuple_2_star_chpl_bool__tuple_2_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_2_star_chpl_bool__tuple_2_star_int64_t_int64_t_DefaultRectangularArr_2_int64_t_one__real64_int64_t__tuple_2_star_chpl_bool__tuple_2_star_int64_t__remoteAccessData__real64_1_int64_t_one_T_T_chpl this_chpl31,
#line 774 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                                                                                                                                                                             int64_t _ln_chpl,
#line 774 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                                                                                                                                                                             int32_t _fn_chpl) {
#line 774 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ArrayViewRankChangeDom_1_int64_t_one__tuple_2_star_chpl_bool__tuple_2_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_2_star_chpl_bool__tuple_2_star_int64_t_chpl ret_chpl = NULL;
#line 775 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 775 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ret_chpl = (this_chpl31)->dom;
#line 774 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  return ret_chpl;
#line 774 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
}

#line 774 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
/* ArrayViewRankChange.chpl:774 */
#line 774 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
static ArrayViewRankChangeDom_2_int64_t_one__tuple_4_star_chpl_bool__tuple_4_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_4_star_chpl_bool__tuple_4_star_int64_t_chpl dsiGetBaseDom_chpl(ArrayViewRankChangeArr__real64_int64_t_ArrayViewRankChangeDom_2_int64_t_one__tuple_4_star_chpl_bool__tuple_4_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_4_star_chpl_bool__tuple_4_star_int64_t_int64_t_DefaultRectangularArr_4_int64_t_one__real64_int64_t__tuple_4_star_chpl_bool__tuple_4_star_int64_t__remoteAccessData__real64_2_int64_t_one_T_T_chpl this_chpl31,
#line 774 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                                                                                                                                                                            int64_t _ln_chpl,
#line 774 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                                                                                                                                                                            int32_t _fn_chpl) {
#line 774 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ArrayViewRankChangeDom_2_int64_t_one__tuple_4_star_chpl_bool__tuple_4_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_4_star_chpl_bool__tuple_4_star_int64_t_chpl ret_chpl = NULL;
#line 775 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 775 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ret_chpl = (this_chpl31)->dom;
#line 774 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  return ret_chpl;
#line 774 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
}

#line 774 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
/* ArrayViewRankChange.chpl:774 */
#line 774 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
static ArrayViewRankChangeDom_1_int64_t_one__tuple_2_star_chpl_bool__tuple_2_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_2_star_chpl_bool__tuple_2_star_int64_t_chpl dsiGetBaseDom_chpl7(ArrayViewRankChangeArr__real64_int64_t_ArrayViewRankChangeDom_1_int64_t_one__tuple_2_star_chpl_bool__tuple_2_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_2_star_chpl_bool__tuple_2_star_int64_t_int64_t_DefaultRectangularArr_2_int64_t_one__real64_int64_t__tuple_2_star_chpl_bool__tuple_2_star_int64_t__remoteAccessData__real64_1_int64_t_one_T_F_chpl this_chpl31,
#line 774 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                                                                                                                                                                             int64_t _ln_chpl,
#line 774 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                                                                                                                                                                             int32_t _fn_chpl) {
#line 774 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ArrayViewRankChangeDom_1_int64_t_one__tuple_2_star_chpl_bool__tuple_2_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_2_star_chpl_bool__tuple_2_star_int64_t_chpl ret_chpl = NULL;
#line 775 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 775 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ret_chpl = (this_chpl31)->dom;
#line 774 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  return ret_chpl;
#line 774 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
}

#line 774 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
/* ArrayViewRankChange.chpl:774 */
#line 774 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
static ArrayViewRankChangeDom_2_int64_t_one__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_3_star_chpl_bool__tuple_3_star_int64_t_chpl dsiGetBaseDom_chpl8(ArrayViewRankChangeArr__real64_int64_t_ArrayViewRankChangeDom_2_int64_t_one__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_DefaultRectangularArr_3_int64_t_one__real64_int64_t__tuple_3_star_chpl_bool__tuple_3_star_int64_t__remoteAccessData__real64_2_int64_t_one_T_T_chpl this_chpl31,
#line 774 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                                                                                                                                                                             int64_t _ln_chpl,
#line 774 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                                                                                                                                                                             int32_t _fn_chpl) {
#line 774 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ArrayViewRankChangeDom_2_int64_t_one__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_3_star_chpl_bool__tuple_3_star_int64_t_chpl ret_chpl = NULL;
#line 775 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 775 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ret_chpl = (this_chpl31)->dom;
#line 774 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  return ret_chpl;
#line 774 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
}

#line 774 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
/* ArrayViewRankChange.chpl:774 */
#line 774 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
static ArrayViewRankChangeDom_2_int64_t_one__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_3_star_chpl_bool__tuple_3_star_int64_t_chpl dsiGetBaseDom_chpl6(ArrayViewRankChangeArr__real64_int64_t_ArrayViewRankChangeDom_2_int64_t_one__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_DefaultRectangularArr_3_int64_t_one__real64_int64_t__tuple_3_star_chpl_bool__tuple_3_star_int64_t__remoteAccessData__real64_2_int64_t_one_T_F_chpl this_chpl31,
#line 774 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                                                                                                                                                                             int64_t _ln_chpl,
#line 774 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                                                                                                                                                                             int32_t _fn_chpl) {
#line 774 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ArrayViewRankChangeDom_2_int64_t_one__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_3_star_chpl_bool__tuple_3_star_int64_t_chpl ret_chpl = NULL;
#line 775 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 775 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ret_chpl = (this_chpl31)->dom;
#line 774 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  return ret_chpl;
#line 774 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
}

#line 774 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
/* ArrayViewRankChange.chpl:774 */
#line 774 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
static ArrayViewRankChangeDom_3_int64_t_one__tuple_4_star_chpl_bool__tuple_4_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_4_star_chpl_bool__tuple_4_star_int64_t_chpl dsiGetBaseDom_chpl5(ArrayViewRankChangeArr__real64_int64_t_ArrayViewRankChangeDom_3_int64_t_one__tuple_4_star_chpl_bool__tuple_4_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_4_star_chpl_bool__tuple_4_star_int64_t_int64_t_DefaultRectangularArr_4_int64_t_one__real64_int64_t__tuple_4_star_chpl_bool__tuple_4_star_int64_t__remoteAccessData__real64_3_int64_t_one_T_T_chpl this_chpl31,
#line 774 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                                                                                                                                                                             int64_t _ln_chpl,
#line 774 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                                                                                                                                                                             int32_t _fn_chpl) {
#line 774 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ArrayViewRankChangeDom_3_int64_t_one__tuple_4_star_chpl_bool__tuple_4_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_4_star_chpl_bool__tuple_4_star_int64_t_chpl ret_chpl = NULL;
#line 775 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 775 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ret_chpl = (this_chpl31)->dom;
#line 774 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  return ret_chpl;
#line 774 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
}

#line 774 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
/* ArrayViewRankChange.chpl:774 */
#line 774 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
static ArrayViewRankChangeDom_3_int64_t_one__tuple_4_star_chpl_bool__tuple_4_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_4_star_chpl_bool__tuple_4_star_int64_t_chpl dsiGetBaseDom_chpl2(ArrayViewRankChangeArr__real64_int64_t_ArrayViewRankChangeDom_3_int64_t_one__tuple_4_star_chpl_bool__tuple_4_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_4_star_chpl_bool__tuple_4_star_int64_t_int64_t_DefaultRectangularArr_4_int64_t_one__real64_int64_t__tuple_4_star_chpl_bool__tuple_4_star_int64_t__remoteAccessData__real64_3_int64_t_one_T_F_chpl this_chpl31,
#line 774 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                                                                                                                                                                             int64_t _ln_chpl,
#line 774 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                                                                                                                                                                             int32_t _fn_chpl) {
#line 774 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ArrayViewRankChangeDom_3_int64_t_one__tuple_4_star_chpl_bool__tuple_4_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_4_star_chpl_bool__tuple_4_star_int64_t_chpl ret_chpl = NULL;
#line 775 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 775 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ret_chpl = (this_chpl31)->dom;
#line 774 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  return ret_chpl;
#line 774 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
}

#line 800 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
/* ArrayViewRankChange.chpl:800 */
#line 800 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
static void dsiElementInitializationComplete_chpl2(ArrayViewRankChangeArr__real64_int64_t_ArrayViewRankChangeDom_1_int64_t_one__tuple_2_star_chpl_bool__tuple_2_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_2_star_chpl_bool__tuple_2_star_int64_t_int64_t_DefaultRectangularArr_2_int64_t_one__real64_int64_t__tuple_2_star_chpl_bool__tuple_2_star_int64_t__remoteAccessData__real64_1_int64_t_one_T_T_chpl this_chpl31) {
#line 800 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  return;
#line 800 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
}

#line 800 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
/* ArrayViewRankChange.chpl:800 */
#line 800 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
static void dsiElementInitializationComplete_chpl(ArrayViewRankChangeArr__real64_int64_t_ArrayViewRankChangeDom_2_int64_t_one__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_DefaultRectangularArr_3_int64_t_one__real64_int64_t__tuple_3_star_chpl_bool__tuple_3_star_int64_t__remoteAccessData__real64_2_int64_t_one_T_T_chpl this_chpl31) {
#line 800 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  return;
#line 800 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
}

#line 804 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
/* ArrayViewRankChange.chpl:804 */
#line 804 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
static void dsiDestroyArr_chpl8(ArrayViewRankChangeArr__real64_int64_t_ArrayViewRankChangeDom_3_int64_t_one__tuple_4_star_chpl_bool__tuple_4_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_4_star_chpl_bool__tuple_4_star_int64_t_int64_t_DefaultRectangularArr_4_int64_t_one__real64_int64_t__tuple_4_star_chpl_bool__tuple_4_star_int64_t__remoteAccessData__real64_3_int64_t_one_T_F_chpl this_chpl31,
#line 804 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                chpl_bool deinitElts_chpl,
#line 804 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                int64_t _ln_chpl,
#line 804 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                int32_t _fn_chpl) {
#line 804 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  return;
#line 804 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
}

#line 804 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
/* ArrayViewRankChange.chpl:804 */
#line 804 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
static void dsiDestroyArr_chpl6(ArrayViewRankChangeArr__real64_int64_t_ArrayViewRankChangeDom_3_int64_t_one__tuple_4_star_chpl_bool__tuple_4_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_4_star_chpl_bool__tuple_4_star_int64_t_int64_t_DefaultRectangularArr_4_int64_t_one__real64_int64_t__tuple_4_star_chpl_bool__tuple_4_star_int64_t__remoteAccessData__real64_3_int64_t_one_T_T_chpl this_chpl31,
#line 804 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                chpl_bool deinitElts_chpl,
#line 804 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                int64_t _ln_chpl,
#line 804 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                int32_t _fn_chpl) {
#line 806 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 806 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _delete_arr_chpl(((BaseArr_chpl)((this_chpl31)->chpl___ArrInstance)), UINT8(true), _ln_chpl, _fn_chpl);
#line 804 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  return;
#line 804 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
}

#line 804 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
/* ArrayViewRankChange.chpl:804 */
#line 804 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
static void dsiDestroyArr_chpl5(ArrayViewRankChangeArr__real64_int64_t_ArrayViewRankChangeDom_2_int64_t_one__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_DefaultRectangularArr_3_int64_t_one__real64_int64_t__tuple_3_star_chpl_bool__tuple_3_star_int64_t__remoteAccessData__real64_2_int64_t_one_T_F_chpl this_chpl31,
#line 804 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                chpl_bool deinitElts_chpl,
#line 804 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                int64_t _ln_chpl,
#line 804 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                int32_t _fn_chpl) {
#line 804 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  return;
#line 804 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
}

#line 804 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
/* ArrayViewRankChange.chpl:804 */
#line 804 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
static void dsiDestroyArr_chpl7(ArrayViewRankChangeArr__real64_int64_t_ArrayViewRankChangeDom_1_int64_t_one__tuple_2_star_chpl_bool__tuple_2_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_2_star_chpl_bool__tuple_2_star_int64_t_int64_t_DefaultRectangularArr_2_int64_t_one__real64_int64_t__tuple_2_star_chpl_bool__tuple_2_star_int64_t__remoteAccessData__real64_1_int64_t_one_T_F_chpl this_chpl31,
#line 804 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                chpl_bool deinitElts_chpl,
#line 804 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                int64_t _ln_chpl,
#line 804 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                int32_t _fn_chpl) {
#line 804 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  return;
#line 804 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
}

#line 804 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
/* ArrayViewRankChange.chpl:804 */
#line 804 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
static void dsiDestroyArr_chpl2(ArrayViewRankChangeArr__real64_int64_t_ArrayViewRankChangeDom_1_int64_t_one__tuple_2_star_chpl_bool__tuple_2_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_2_star_chpl_bool__tuple_2_star_int64_t_int64_t_DefaultRectangularArr_2_int64_t_one__real64_int64_t__tuple_2_star_chpl_bool__tuple_2_star_int64_t__remoteAccessData__real64_1_int64_t_one_T_T_chpl this_chpl31,
#line 804 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                chpl_bool deinitElts_chpl,
#line 804 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                int64_t _ln_chpl,
#line 804 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                int32_t _fn_chpl) {
#line 806 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 806 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _delete_arr_chpl(((BaseArr_chpl)((this_chpl31)->chpl___ArrInstance)), UINT8(true), _ln_chpl, _fn_chpl);
#line 804 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  return;
#line 804 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
}

#line 804 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
/* ArrayViewRankChange.chpl:804 */
#line 804 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
static void dsiDestroyArr_chpl3(ArrayViewRankChangeArr__real64_int64_t_ArrayViewRankChangeDom_2_int64_t_one__tuple_4_star_chpl_bool__tuple_4_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_4_star_chpl_bool__tuple_4_star_int64_t_int64_t_DefaultRectangularArr_4_int64_t_one__real64_int64_t__tuple_4_star_chpl_bool__tuple_4_star_int64_t__remoteAccessData__real64_2_int64_t_one_T_F_chpl this_chpl31,
#line 804 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                chpl_bool deinitElts_chpl,
#line 804 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                int64_t _ln_chpl,
#line 804 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                int32_t _fn_chpl) {
#line 804 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  return;
#line 804 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
}

#line 804 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
/* ArrayViewRankChange.chpl:804 */
#line 804 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
static void dsiDestroyArr_chpl4(ArrayViewRankChangeArr__real64_int64_t_ArrayViewRankChangeDom_2_int64_t_one__tuple_4_star_chpl_bool__tuple_4_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_4_star_chpl_bool__tuple_4_star_int64_t_int64_t_DefaultRectangularArr_4_int64_t_one__real64_int64_t__tuple_4_star_chpl_bool__tuple_4_star_int64_t__remoteAccessData__real64_2_int64_t_one_T_T_chpl this_chpl31,
#line 804 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                chpl_bool deinitElts_chpl,
#line 804 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                int64_t _ln_chpl,
#line 804 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                int32_t _fn_chpl) {
#line 806 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 806 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _delete_arr_chpl(((BaseArr_chpl)((this_chpl31)->chpl___ArrInstance)), UINT8(true), _ln_chpl, _fn_chpl);
#line 804 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  return;
#line 804 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
}

#line 804 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
/* ArrayViewRankChange.chpl:804 */
#line 804 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
static void dsiDestroyArr_chpl(ArrayViewRankChangeArr__real64_int64_t_ArrayViewRankChangeDom_2_int64_t_one__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_DefaultRectangularArr_3_int64_t_one__real64_int64_t__tuple_3_star_chpl_bool__tuple_3_star_int64_t__remoteAccessData__real64_2_int64_t_one_T_T_chpl this_chpl31,
#line 804 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                               chpl_bool deinitElts_chpl,
#line 804 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                               int64_t _ln_chpl,
#line 804 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                               int32_t _fn_chpl) {
#line 806 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 806 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _delete_arr_chpl(((BaseArr_chpl)((this_chpl31)->chpl___ArrInstance)), UINT8(true), _ln_chpl, _fn_chpl);
#line 804 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  return;
#line 804 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
}

#line 821 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
/* ArrayViewRankChange.chpl:821 */
#line 821 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
static chpl_bool doiBulkTransferFromKnown_chpl(ArrayViewRankChangeArr__real64_int64_t_ArrayViewRankChangeDom_1_int64_t_one__tuple_2_star_chpl_bool__tuple_2_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_2_star_chpl_bool__tuple_2_star_int64_t_int64_t_DefaultRectangularArr_2_int64_t_one__real64_int64_t__tuple_2_star_chpl_bool__tuple_2_star_int64_t__remoteAccessData__real64_1_int64_t_one_T_T_chpl this_chpl31,
#line 821 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                               _domain_ArrayViewRankChangeDom_1_int64_t_one__tuple_2_star_chpl_bool__tuple_2_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_2_star_chpl_bool__tuple_2_star_int64_t_chpl * destDom_chpl,
#line 821 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                               ArrayViewRankChangeArr__real64_int64_t_ArrayViewRankChangeDom_1_int64_t_one__tuple_2_star_chpl_bool__tuple_2_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_2_star_chpl_bool__tuple_2_star_int64_t_int64_t_DefaultRectangularArr_2_int64_t_one__real64_int64_t__tuple_2_star_chpl_bool__tuple_2_star_int64_t__remoteAccessData__real64_1_int64_t_one_T_F_chpl srcClass_chpl,
#line 821 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                               _domain_ArrayViewRankChangeDom_1_int64_t_one__tuple_2_star_chpl_bool__tuple_2_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_2_star_chpl_bool__tuple_2_star_int64_t_chpl * srcDom_chpl,
#line 821 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                               int64_t _ln_chpl,
#line 821 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                               int32_t _fn_chpl) {
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _tuple_1_star_range_int64_t_both_one_chpl call_tmp_chpl6;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _tuple_1_star_range_int64_t_both_one_chpl ret_tmp_chpl;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ArrayViewRankChangeDom_1_int64_t_one__tuple_2_star_chpl_bool__tuple_2_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_2_star_chpl_bool__tuple_2_star_int64_t_chpl coerce_tmp_chpl3 = NULL;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _ref__tuple_2_star_chpl_bool_chpl call_tmp_chpl7 = NULL;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ArrayViewRankChangeDom_1_int64_t_one__tuple_2_star_chpl_bool__tuple_2_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_2_star_chpl_bool__tuple_2_star_int64_t_chpl coerce_tmp_chpl4 = NULL;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _ref__tuple_2_star_int64_t_chpl call_tmp_chpl8 = NULL;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _ref__tuple_1_star_range_int64_t_both_one_chpl i_dims_chpl = NULL;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl ranges_x0_chpl;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl ranges_x1_chpl;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl tup_x0_chpl;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl tup_x1_chpl;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl elt_x0_chpl;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl elt_x1_chpl;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  int64_t j_chpl;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl9 = NULL;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl5;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _ref_int64_t_chpl i_lhs_chpl = NULL;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl10;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl2;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl11 = NULL;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl6;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _ref_int64_t_chpl i_lhs_chpl2 = NULL;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl12;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl3;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _domain_DefaultRectangularDom_2_int64_t_one_chpl call_tmp_chpl13;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _domain_DefaultRectangularDom_2_int64_t_one_chpl ret_tmp_chpl4;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _domain_DefaultRectangularDom_2_int64_t_one_chpl shifted_chpl;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _ref_DefaultRectangularDom_2_int64_t_one_chpl _instance_chpl = NULL;
#line 824 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  DefaultRectangularArr_2_int64_t_one__real64_int64_t_chpl ret_chpl = NULL;
#line 824 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _ref__domain_DefaultRectangularDom_2_int64_t_one_chpl i_destDom_chpl = NULL;
#line 824 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_bool call_tmp_chpl14;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  dims_chpl2(destDom_chpl, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(call_tmp_chpl6 + INT64(0)) = *(ret_tmp_chpl + INT64(0));
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  coerce_tmp_chpl3 = (this_chpl31)->dom;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  call_tmp_chpl7 = &((coerce_tmp_chpl3)->collapsedDim);
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  coerce_tmp_chpl4 = (this_chpl31)->dom;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(coerce_tmp_chpl4, _ln_chpl, _fn_chpl);
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  call_tmp_chpl8 = &((coerce_tmp_chpl4)->idx);
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  i_dims_chpl = &call_tmp_chpl6;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  init_chpl113(&elt_x0_chpl);
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  tup_x0_chpl = elt_x0_chpl;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  init_chpl113(&elt_x1_chpl);
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  tup_x1_chpl = elt_x1_chpl;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ranges_x0_chpl = tup_x0_chpl;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ranges_x1_chpl = tup_x1_chpl;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  j_chpl = INT64(0);
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  if (! *(*(call_tmp_chpl7) + INT64(0))) /* ZLINE: 823 /Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl */
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  {
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    call_tmp_chpl9 = this_chpl20(i_dims_chpl, INT64(0), _ln_chpl, _fn_chpl);
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    coerce_tmp_chpl5 = *(call_tmp_chpl9);
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    ranges_x0_chpl = coerce_tmp_chpl5;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    i_lhs_chpl = &j_chpl;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    *(i_lhs_chpl) += INT64(1);
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  } else /* ZLINE: 823 /Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl */
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  {
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    createWithSingleElement_chpl(*(*(call_tmp_chpl8) + INT64(0)), &ret_tmp_chpl2);
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    call_tmp_chpl10 = ret_tmp_chpl2;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    ranges_x0_chpl = call_tmp_chpl10;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  }
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  if (! *(*(call_tmp_chpl7) + INT64(1))) /* ZLINE: 823 /Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl */
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  {
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    call_tmp_chpl11 = this_chpl20(i_dims_chpl, j_chpl, _ln_chpl, _fn_chpl);
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    coerce_tmp_chpl6 = *(call_tmp_chpl11);
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    ranges_x1_chpl = coerce_tmp_chpl6;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    i_lhs_chpl2 = &j_chpl;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    *(i_lhs_chpl2) += INT64(1);
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  } else /* ZLINE: 823 /Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl */
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  {
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    createWithSingleElement_chpl(*(*(call_tmp_chpl8) + INT64(1)), &ret_tmp_chpl3);
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    call_tmp_chpl12 = ret_tmp_chpl3;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    ranges_x1_chpl = call_tmp_chpl12;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  }
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl__buildDomainExpr2(ranges_x0_chpl, ranges_x1_chpl, UINT8(true), &ret_tmp_chpl4, _ln_chpl, _fn_chpl);
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  call_tmp_chpl13 = ret_tmp_chpl4;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  shifted_chpl = call_tmp_chpl13;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _instance_chpl = &((&shifted_chpl)->_instance);
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(_instance_chpl, _ln_chpl, _fn_chpl);
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ((BaseDom_chpl)(*(_instance_chpl)))->definedConst = UINT8(true);
#line 824 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 824 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ret_chpl = (this_chpl31)->chpl___ArrInstance;
#line 824 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  i_destDom_chpl = &shifted_chpl;
#line 824 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(srcClass_chpl, _ln_chpl, _fn_chpl);
#line 824 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  call_tmp_chpl14 = doiBulkTransferToKnown_chpl2(srcClass_chpl, srcDom_chpl, ret_chpl, i_destDom_chpl, _ln_chpl, _fn_chpl);
#line 824 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  if (call_tmp_chpl14) /* ZLINE: 824 /Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl */
#line 824 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  {
#line 824 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  } else /* ZLINE: 824 /Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl */
#line 824 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  {
#line 824 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  }
#line 824 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl__autoDestroy5(&shifted_chpl, _ln_chpl, _fn_chpl);
#line 821 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  return call_tmp_chpl14;
#line 821 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
}

#line 821 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
/* ArrayViewRankChange.chpl:821 */
#line 821 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
static chpl_bool doiBulkTransferFromKnown_chpl3(ArrayViewRankChangeArr__real64_int64_t_ArrayViewRankChangeDom_1_int64_t_one__tuple_2_star_chpl_bool__tuple_2_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_2_star_chpl_bool__tuple_2_star_int64_t_int64_t_DefaultRectangularArr_2_int64_t_one__real64_int64_t__tuple_2_star_chpl_bool__tuple_2_star_int64_t__remoteAccessData__real64_1_int64_t_one_T_F_chpl this_chpl31,
#line 821 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                _domain_ArrayViewRankChangeDom_1_int64_t_one__tuple_2_star_chpl_bool__tuple_2_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_2_star_chpl_bool__tuple_2_star_int64_t_chpl * destDom_chpl,
#line 821 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                DefaultRectangularArr_1_int64_t_one__real64_int64_t_chpl srcClass_chpl,
#line 821 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                _domain_DefaultRectangularDom_1_int64_t_one_chpl * srcDom_chpl,
#line 821 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                int64_t _ln_chpl,
#line 821 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                int32_t _fn_chpl) {
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _tuple_1_star_range_int64_t_both_one_chpl call_tmp_chpl6;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _tuple_1_star_range_int64_t_both_one_chpl ret_tmp_chpl;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ArrayViewRankChangeDom_1_int64_t_one__tuple_2_star_chpl_bool__tuple_2_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_2_star_chpl_bool__tuple_2_star_int64_t_chpl coerce_tmp_chpl3 = NULL;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _ref__tuple_2_star_chpl_bool_chpl call_tmp_chpl7 = NULL;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ArrayViewRankChangeDom_1_int64_t_one__tuple_2_star_chpl_bool__tuple_2_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_2_star_chpl_bool__tuple_2_star_int64_t_chpl coerce_tmp_chpl4 = NULL;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _ref__tuple_2_star_int64_t_chpl call_tmp_chpl8 = NULL;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _ref__tuple_1_star_range_int64_t_both_one_chpl i_dims_chpl = NULL;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl ranges_x0_chpl;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl ranges_x1_chpl;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl tup_x0_chpl;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl tup_x1_chpl;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl elt_x0_chpl;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl elt_x1_chpl;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  int64_t j_chpl;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl9 = NULL;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl5;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _ref_int64_t_chpl i_lhs_chpl = NULL;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl10;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl2;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl11 = NULL;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl6;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _ref_int64_t_chpl i_lhs_chpl2 = NULL;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl12;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl3;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _domain_DefaultRectangularDom_2_int64_t_one_chpl call_tmp_chpl13;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _domain_DefaultRectangularDom_2_int64_t_one_chpl ret_tmp_chpl4;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _domain_DefaultRectangularDom_2_int64_t_one_chpl shifted_chpl;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _ref_DefaultRectangularDom_2_int64_t_one_chpl _instance_chpl = NULL;
#line 824 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  DefaultRectangularArr_2_int64_t_one__real64_int64_t_chpl ret_chpl = NULL;
#line 824 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _ref__domain_DefaultRectangularDom_2_int64_t_one_chpl i_destDom_chpl = NULL;
#line 824 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_bool call_tmp_chpl14;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  dims_chpl2(destDom_chpl, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(call_tmp_chpl6 + INT64(0)) = *(ret_tmp_chpl + INT64(0));
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  coerce_tmp_chpl3 = (this_chpl31)->dom;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  call_tmp_chpl7 = &((coerce_tmp_chpl3)->collapsedDim);
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  coerce_tmp_chpl4 = (this_chpl31)->dom;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(coerce_tmp_chpl4, _ln_chpl, _fn_chpl);
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  call_tmp_chpl8 = &((coerce_tmp_chpl4)->idx);
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  i_dims_chpl = &call_tmp_chpl6;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  init_chpl113(&elt_x0_chpl);
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  tup_x0_chpl = elt_x0_chpl;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  init_chpl113(&elt_x1_chpl);
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  tup_x1_chpl = elt_x1_chpl;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ranges_x0_chpl = tup_x0_chpl;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ranges_x1_chpl = tup_x1_chpl;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  j_chpl = INT64(0);
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  if (! *(*(call_tmp_chpl7) + INT64(0))) /* ZLINE: 823 /Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl */
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  {
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    call_tmp_chpl9 = this_chpl20(i_dims_chpl, INT64(0), _ln_chpl, _fn_chpl);
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    coerce_tmp_chpl5 = *(call_tmp_chpl9);
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    ranges_x0_chpl = coerce_tmp_chpl5;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    i_lhs_chpl = &j_chpl;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    *(i_lhs_chpl) += INT64(1);
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  } else /* ZLINE: 823 /Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl */
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  {
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    createWithSingleElement_chpl(*(*(call_tmp_chpl8) + INT64(0)), &ret_tmp_chpl2);
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    call_tmp_chpl10 = ret_tmp_chpl2;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    ranges_x0_chpl = call_tmp_chpl10;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  }
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  if (! *(*(call_tmp_chpl7) + INT64(1))) /* ZLINE: 823 /Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl */
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  {
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    call_tmp_chpl11 = this_chpl20(i_dims_chpl, j_chpl, _ln_chpl, _fn_chpl);
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    coerce_tmp_chpl6 = *(call_tmp_chpl11);
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    ranges_x1_chpl = coerce_tmp_chpl6;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    i_lhs_chpl2 = &j_chpl;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    *(i_lhs_chpl2) += INT64(1);
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  } else /* ZLINE: 823 /Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl */
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  {
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    createWithSingleElement_chpl(*(*(call_tmp_chpl8) + INT64(1)), &ret_tmp_chpl3);
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    call_tmp_chpl12 = ret_tmp_chpl3;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    ranges_x1_chpl = call_tmp_chpl12;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  }
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl__buildDomainExpr2(ranges_x0_chpl, ranges_x1_chpl, UINT8(true), &ret_tmp_chpl4, _ln_chpl, _fn_chpl);
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  call_tmp_chpl13 = ret_tmp_chpl4;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  shifted_chpl = call_tmp_chpl13;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _instance_chpl = &((&shifted_chpl)->_instance);
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(_instance_chpl, _ln_chpl, _fn_chpl);
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ((BaseDom_chpl)(*(_instance_chpl)))->definedConst = UINT8(true);
#line 824 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 824 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ret_chpl = (this_chpl31)->chpl___ArrInstance;
#line 824 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  i_destDom_chpl = &shifted_chpl;
#line 824 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(ret_chpl, _ln_chpl, _fn_chpl);
#line 824 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  call_tmp_chpl14 = doiBulkTransferFromKnown_chpl11(ret_chpl, i_destDom_chpl, srcClass_chpl, srcDom_chpl, _ln_chpl, _fn_chpl);
#line 824 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  if (call_tmp_chpl14) /* ZLINE: 824 /Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl */
#line 824 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  {
#line 824 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  } else /* ZLINE: 824 /Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl */
#line 824 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  {
#line 824 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  }
#line 824 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl__autoDestroy5(&shifted_chpl, _ln_chpl, _fn_chpl);
#line 821 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  return call_tmp_chpl14;
#line 821 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
}

#line 821 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
/* ArrayViewRankChange.chpl:821 */
#line 821 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
static chpl_bool doiBulkTransferFromKnown_chpl2(ArrayViewRankChangeArr__real64_int64_t_ArrayViewRankChangeDom_2_int64_t_one__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_DefaultRectangularArr_3_int64_t_one__real64_int64_t__tuple_3_star_chpl_bool__tuple_3_star_int64_t__remoteAccessData__real64_2_int64_t_one_T_T_chpl this_chpl31,
#line 821 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                _domain_ArrayViewRankChangeDom_2_int64_t_one__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_3_star_chpl_bool__tuple_3_star_int64_t_chpl * destDom_chpl,
#line 821 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                ArrayViewRankChangeArr__real64_int64_t_ArrayViewRankChangeDom_2_int64_t_one__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_DefaultRectangularArr_3_int64_t_one__real64_int64_t__tuple_3_star_chpl_bool__tuple_3_star_int64_t__remoteAccessData__real64_2_int64_t_one_T_F_chpl srcClass_chpl,
#line 821 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                _domain_ArrayViewRankChangeDom_2_int64_t_one__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_3_star_chpl_bool__tuple_3_star_int64_t_chpl * srcDom_chpl,
#line 821 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                int64_t _ln_chpl,
#line 821 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                                int32_t _fn_chpl) {
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _tuple_2_star_range_int64_t_both_one_chpl call_tmp_chpl6;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _tuple_2_star_range_int64_t_both_one_chpl ret_tmp_chpl;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ArrayViewRankChangeDom_2_int64_t_one__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_3_star_chpl_bool__tuple_3_star_int64_t_chpl coerce_tmp_chpl3 = NULL;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _ref__tuple_3_star_chpl_bool_chpl call_tmp_chpl7 = NULL;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ArrayViewRankChangeDom_2_int64_t_one__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_3_star_chpl_bool__tuple_3_star_int64_t_chpl coerce_tmp_chpl4 = NULL;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _ref__tuple_3_star_int64_t_chpl call_tmp_chpl8 = NULL;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _ref__tuple_2_star_range_int64_t_both_one_chpl i_dims_chpl = NULL;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl ranges_x0_chpl;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl ranges_x1_chpl;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl ranges_x2_chpl;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl tup_x0_chpl;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl tup_x1_chpl;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl tup_x2_chpl;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl elt_x0_chpl;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl elt_x1_chpl;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl elt_x2_chpl;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  int64_t j_chpl;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl9 = NULL;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl5;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _ref_int64_t_chpl i_lhs_chpl = NULL;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl10;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl2;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl11 = NULL;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl6;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _ref_int64_t_chpl i_lhs_chpl2 = NULL;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl12;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl3;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl13 = NULL;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl7;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _ref_int64_t_chpl i_lhs_chpl3 = NULL;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl14;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl4;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _domain_DefaultRectangularDom_3_int64_t_one_chpl call_tmp_chpl15;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _domain_DefaultRectangularDom_3_int64_t_one_chpl ret_tmp_chpl5;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _domain_DefaultRectangularDom_3_int64_t_one_chpl shifted_chpl;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _ref_DefaultRectangularDom_3_int64_t_one_chpl _instance_chpl = NULL;
#line 824 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  DefaultRectangularArr_3_int64_t_one__real64_int64_t_chpl ret_chpl = NULL;
#line 824 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _ref__domain_DefaultRectangularDom_3_int64_t_one_chpl i_destDom_chpl = NULL;
#line 824 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_bool call_tmp_chpl16;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  dims_chpl6(destDom_chpl, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(call_tmp_chpl6 + INT64(0)) = *(ret_tmp_chpl + INT64(0));
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(call_tmp_chpl6 + INT64(1)) = *(ret_tmp_chpl + INT64(1));
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  coerce_tmp_chpl3 = (this_chpl31)->dom;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  call_tmp_chpl7 = &((coerce_tmp_chpl3)->collapsedDim);
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  coerce_tmp_chpl4 = (this_chpl31)->dom;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(coerce_tmp_chpl4, _ln_chpl, _fn_chpl);
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  call_tmp_chpl8 = &((coerce_tmp_chpl4)->idx);
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  i_dims_chpl = &call_tmp_chpl6;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  init_chpl113(&elt_x0_chpl);
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  tup_x0_chpl = elt_x0_chpl;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  init_chpl113(&elt_x1_chpl);
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  tup_x1_chpl = elt_x1_chpl;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  init_chpl113(&elt_x2_chpl);
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  tup_x2_chpl = elt_x2_chpl;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ranges_x0_chpl = tup_x0_chpl;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ranges_x1_chpl = tup_x1_chpl;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ranges_x2_chpl = tup_x2_chpl;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  j_chpl = INT64(0);
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  if (! *(*(call_tmp_chpl7) + INT64(0))) /* ZLINE: 823 /Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl */
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  {
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    call_tmp_chpl9 = this_chpl26(i_dims_chpl, INT64(0), _ln_chpl, _fn_chpl);
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    coerce_tmp_chpl5 = *(call_tmp_chpl9);
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    ranges_x0_chpl = coerce_tmp_chpl5;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    i_lhs_chpl = &j_chpl;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    *(i_lhs_chpl) += INT64(1);
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  } else /* ZLINE: 823 /Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl */
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  {
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    createWithSingleElement_chpl(*(*(call_tmp_chpl8) + INT64(0)), &ret_tmp_chpl2);
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    call_tmp_chpl10 = ret_tmp_chpl2;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    ranges_x0_chpl = call_tmp_chpl10;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  }
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  if (! *(*(call_tmp_chpl7) + INT64(1))) /* ZLINE: 823 /Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl */
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  {
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    call_tmp_chpl11 = this_chpl26(i_dims_chpl, j_chpl, _ln_chpl, _fn_chpl);
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    coerce_tmp_chpl6 = *(call_tmp_chpl11);
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    ranges_x1_chpl = coerce_tmp_chpl6;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    i_lhs_chpl2 = &j_chpl;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    *(i_lhs_chpl2) += INT64(1);
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  } else /* ZLINE: 823 /Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl */
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  {
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    createWithSingleElement_chpl(*(*(call_tmp_chpl8) + INT64(1)), &ret_tmp_chpl3);
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    call_tmp_chpl12 = ret_tmp_chpl3;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    ranges_x1_chpl = call_tmp_chpl12;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  }
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  if (! *(*(call_tmp_chpl7) + INT64(2))) /* ZLINE: 823 /Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl */
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  {
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    call_tmp_chpl13 = this_chpl26(i_dims_chpl, j_chpl, _ln_chpl, _fn_chpl);
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    coerce_tmp_chpl7 = *(call_tmp_chpl13);
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    ranges_x2_chpl = coerce_tmp_chpl7;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    i_lhs_chpl3 = &j_chpl;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    *(i_lhs_chpl3) += INT64(1);
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  } else /* ZLINE: 823 /Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl */
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  {
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    createWithSingleElement_chpl(*(*(call_tmp_chpl8) + INT64(2)), &ret_tmp_chpl4);
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    call_tmp_chpl14 = ret_tmp_chpl4;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    ranges_x2_chpl = call_tmp_chpl14;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  }
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl__buildDomainExpr(ranges_x0_chpl, ranges_x1_chpl, ranges_x2_chpl, UINT8(true), &ret_tmp_chpl5, _ln_chpl, _fn_chpl);
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  call_tmp_chpl15 = ret_tmp_chpl5;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  shifted_chpl = call_tmp_chpl15;
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _instance_chpl = &((&shifted_chpl)->_instance);
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(_instance_chpl, _ln_chpl, _fn_chpl);
#line 823 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ((BaseDom_chpl)(*(_instance_chpl)))->definedConst = UINT8(true);
#line 824 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 824 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ret_chpl = (this_chpl31)->chpl___ArrInstance;
#line 824 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  i_destDom_chpl = &shifted_chpl;
#line 824 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(srcClass_chpl, _ln_chpl, _fn_chpl);
#line 824 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  call_tmp_chpl16 = doiBulkTransferToKnown_chpl4(srcClass_chpl, srcDom_chpl, ret_chpl, i_destDom_chpl, _ln_chpl, _fn_chpl);
#line 824 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  if (call_tmp_chpl16) /* ZLINE: 824 /Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl */
#line 824 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  {
#line 824 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  } else /* ZLINE: 824 /Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl */
#line 824 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  {
#line 824 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  }
#line 824 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl__autoDestroy8(&shifted_chpl, _ln_chpl, _fn_chpl);
#line 821 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  return call_tmp_chpl16;
#line 821 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
}

#line 827 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
/* ArrayViewRankChange.chpl:827 */
#line 827 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
static chpl_bool doiBulkTransferToKnown_chpl(ArrayViewRankChangeArr__real64_int64_t_ArrayViewRankChangeDom_3_int64_t_one__tuple_4_star_chpl_bool__tuple_4_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_4_star_chpl_bool__tuple_4_star_int64_t_int64_t_DefaultRectangularArr_4_int64_t_one__real64_int64_t__tuple_4_star_chpl_bool__tuple_4_star_int64_t__remoteAccessData__real64_3_int64_t_one_T_F_chpl this_chpl31,
#line 827 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                             _domain_ArrayViewRankChangeDom_3_int64_t_one__tuple_4_star_chpl_bool__tuple_4_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_4_star_chpl_bool__tuple_4_star_int64_t_chpl * srcDom_chpl,
#line 827 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                             DefaultRectangularArr_3_int64_t_one__real64_int64_t_chpl destClass_chpl,
#line 827 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                             _domain_DefaultRectangularDom_3_int64_t_one_chpl * destDom_chpl,
#line 827 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                             int64_t _ln_chpl,
#line 827 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                             int32_t _fn_chpl) {
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _tuple_3_star_range_int64_t_both_one_chpl call_tmp_chpl6;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _tuple_3_star_range_int64_t_both_one_chpl ret_tmp_chpl;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ArrayViewRankChangeDom_3_int64_t_one__tuple_4_star_chpl_bool__tuple_4_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_4_star_chpl_bool__tuple_4_star_int64_t_chpl coerce_tmp_chpl3 = NULL;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _ref__tuple_4_star_chpl_bool_chpl call_tmp_chpl7 = NULL;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ArrayViewRankChangeDom_3_int64_t_one__tuple_4_star_chpl_bool__tuple_4_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_4_star_chpl_bool__tuple_4_star_int64_t_chpl coerce_tmp_chpl4 = NULL;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _ref__tuple_4_star_int64_t_chpl call_tmp_chpl8 = NULL;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _ref__tuple_3_star_range_int64_t_both_one_chpl i_dims_chpl = NULL;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl ranges_x0_chpl;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl ranges_x1_chpl;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl ranges_x2_chpl;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl ranges_x3_chpl;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl tup_x0_chpl;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl tup_x1_chpl;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl tup_x2_chpl;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl tup_x3_chpl;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl elt_x0_chpl;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl elt_x1_chpl;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl elt_x2_chpl;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl elt_x3_chpl;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  int64_t j_chpl;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl9 = NULL;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl5;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _ref_int64_t_chpl i_lhs_chpl = NULL;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl10;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl2;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl11 = NULL;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl6;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _ref_int64_t_chpl i_lhs_chpl2 = NULL;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl12;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl3;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl13 = NULL;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl7;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _ref_int64_t_chpl i_lhs_chpl3 = NULL;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl14;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl4;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl15 = NULL;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl8;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _ref_int64_t_chpl i_lhs_chpl4 = NULL;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl16;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl5;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _domain_DefaultRectangularDom_4_int64_t_one_chpl call_tmp_chpl17;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _domain_DefaultRectangularDom_4_int64_t_one_chpl ret_tmp_chpl6;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _domain_DefaultRectangularDom_4_int64_t_one_chpl shifted_chpl;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _ref_DefaultRectangularDom_4_int64_t_one_chpl _instance_chpl = NULL;
#line 830 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  DefaultRectangularArr_4_int64_t_one__real64_int64_t_chpl ret_chpl = NULL;
#line 830 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _ref__domain_DefaultRectangularDom_4_int64_t_one_chpl i_srcDom_chpl = NULL;
#line 830 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_bool call_tmp_chpl18;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  dims_chpl(srcDom_chpl, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(call_tmp_chpl6 + INT64(0)) = *(ret_tmp_chpl + INT64(0));
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(call_tmp_chpl6 + INT64(1)) = *(ret_tmp_chpl + INT64(1));
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(call_tmp_chpl6 + INT64(2)) = *(ret_tmp_chpl + INT64(2));
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  coerce_tmp_chpl3 = (this_chpl31)->dom;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  call_tmp_chpl7 = &((coerce_tmp_chpl3)->collapsedDim);
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  coerce_tmp_chpl4 = (this_chpl31)->dom;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(coerce_tmp_chpl4, _ln_chpl, _fn_chpl);
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  call_tmp_chpl8 = &((coerce_tmp_chpl4)->idx);
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  i_dims_chpl = &call_tmp_chpl6;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  init_chpl113(&elt_x0_chpl);
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  tup_x0_chpl = elt_x0_chpl;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  init_chpl113(&elt_x1_chpl);
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  tup_x1_chpl = elt_x1_chpl;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  init_chpl113(&elt_x2_chpl);
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  tup_x2_chpl = elt_x2_chpl;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  init_chpl113(&elt_x3_chpl);
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  tup_x3_chpl = elt_x3_chpl;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ranges_x0_chpl = tup_x0_chpl;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ranges_x1_chpl = tup_x1_chpl;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ranges_x2_chpl = tup_x2_chpl;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ranges_x3_chpl = tup_x3_chpl;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  j_chpl = INT64(0);
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  if (! *(*(call_tmp_chpl7) + INT64(0))) /* ZLINE: 829 /Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl */
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  {
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    call_tmp_chpl9 = this_chpl24(i_dims_chpl, INT64(0), _ln_chpl, _fn_chpl);
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    coerce_tmp_chpl5 = *(call_tmp_chpl9);
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    ranges_x0_chpl = coerce_tmp_chpl5;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    i_lhs_chpl = &j_chpl;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    *(i_lhs_chpl) += INT64(1);
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  } else /* ZLINE: 829 /Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl */
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  {
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    createWithSingleElement_chpl(*(*(call_tmp_chpl8) + INT64(0)), &ret_tmp_chpl2);
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    call_tmp_chpl10 = ret_tmp_chpl2;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    ranges_x0_chpl = call_tmp_chpl10;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  }
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  if (! *(*(call_tmp_chpl7) + INT64(1))) /* ZLINE: 829 /Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl */
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  {
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    call_tmp_chpl11 = this_chpl24(i_dims_chpl, j_chpl, _ln_chpl, _fn_chpl);
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    coerce_tmp_chpl6 = *(call_tmp_chpl11);
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    ranges_x1_chpl = coerce_tmp_chpl6;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    i_lhs_chpl2 = &j_chpl;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    *(i_lhs_chpl2) += INT64(1);
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  } else /* ZLINE: 829 /Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl */
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  {
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    createWithSingleElement_chpl(*(*(call_tmp_chpl8) + INT64(1)), &ret_tmp_chpl3);
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    call_tmp_chpl12 = ret_tmp_chpl3;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    ranges_x1_chpl = call_tmp_chpl12;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  }
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  if (! *(*(call_tmp_chpl7) + INT64(2))) /* ZLINE: 829 /Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl */
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  {
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    call_tmp_chpl13 = this_chpl24(i_dims_chpl, j_chpl, _ln_chpl, _fn_chpl);
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    coerce_tmp_chpl7 = *(call_tmp_chpl13);
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    ranges_x2_chpl = coerce_tmp_chpl7;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    i_lhs_chpl3 = &j_chpl;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    *(i_lhs_chpl3) += INT64(1);
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  } else /* ZLINE: 829 /Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl */
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  {
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    createWithSingleElement_chpl(*(*(call_tmp_chpl8) + INT64(2)), &ret_tmp_chpl4);
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    call_tmp_chpl14 = ret_tmp_chpl4;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    ranges_x2_chpl = call_tmp_chpl14;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  }
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  if (! *(*(call_tmp_chpl7) + INT64(3))) /* ZLINE: 829 /Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl */
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  {
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    call_tmp_chpl15 = this_chpl24(i_dims_chpl, j_chpl, _ln_chpl, _fn_chpl);
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    coerce_tmp_chpl8 = *(call_tmp_chpl15);
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    ranges_x3_chpl = coerce_tmp_chpl8;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    i_lhs_chpl4 = &j_chpl;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    *(i_lhs_chpl4) += INT64(1);
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  } else /* ZLINE: 829 /Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl */
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  {
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    createWithSingleElement_chpl(*(*(call_tmp_chpl8) + INT64(3)), &ret_tmp_chpl5);
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    call_tmp_chpl16 = ret_tmp_chpl5;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    ranges_x3_chpl = call_tmp_chpl16;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  }
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl__buildDomainExpr4(ranges_x0_chpl, ranges_x1_chpl, ranges_x2_chpl, ranges_x3_chpl, UINT8(true), &ret_tmp_chpl6, _ln_chpl, _fn_chpl);
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  call_tmp_chpl17 = ret_tmp_chpl6;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  shifted_chpl = call_tmp_chpl17;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _instance_chpl = &((&shifted_chpl)->_instance);
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(_instance_chpl, _ln_chpl, _fn_chpl);
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ((BaseDom_chpl)(*(_instance_chpl)))->definedConst = UINT8(true);
#line 830 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 830 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ret_chpl = (this_chpl31)->chpl___ArrInstance;
#line 830 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  i_srcDom_chpl = &shifted_chpl;
#line 830 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(destClass_chpl, _ln_chpl, _fn_chpl);
#line 830 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  call_tmp_chpl18 = doiBulkTransferFromKnown_chpl12(destClass_chpl, destDom_chpl, ret_chpl, i_srcDom_chpl, _ln_chpl, _fn_chpl);
#line 830 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  if (call_tmp_chpl18) /* ZLINE: 830 /Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl */
#line 830 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  {
#line 830 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  } else /* ZLINE: 830 /Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl */
#line 830 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  {
#line 830 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  }
#line 830 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl__autoDestroy9(&shifted_chpl, _ln_chpl, _fn_chpl);
#line 827 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  return call_tmp_chpl18;
#line 827 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
}

#line 827 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
/* ArrayViewRankChange.chpl:827 */
#line 827 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
static chpl_bool doiBulkTransferToKnown_chpl2(ArrayViewRankChangeArr__real64_int64_t_ArrayViewRankChangeDom_1_int64_t_one__tuple_2_star_chpl_bool__tuple_2_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_2_star_chpl_bool__tuple_2_star_int64_t_int64_t_DefaultRectangularArr_2_int64_t_one__real64_int64_t__tuple_2_star_chpl_bool__tuple_2_star_int64_t__remoteAccessData__real64_1_int64_t_one_T_F_chpl this_chpl31,
#line 827 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                              _domain_ArrayViewRankChangeDom_1_int64_t_one__tuple_2_star_chpl_bool__tuple_2_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_2_star_chpl_bool__tuple_2_star_int64_t_chpl * srcDom_chpl,
#line 827 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                              DefaultRectangularArr_2_int64_t_one__real64_int64_t_chpl destClass_chpl,
#line 827 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                              _domain_DefaultRectangularDom_2_int64_t_one_chpl * destDom_chpl,
#line 827 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                              int64_t _ln_chpl,
#line 827 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                              int32_t _fn_chpl) {
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _tuple_1_star_range_int64_t_both_one_chpl call_tmp_chpl6;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _tuple_1_star_range_int64_t_both_one_chpl ret_tmp_chpl;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ArrayViewRankChangeDom_1_int64_t_one__tuple_2_star_chpl_bool__tuple_2_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_2_star_chpl_bool__tuple_2_star_int64_t_chpl coerce_tmp_chpl3 = NULL;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _ref__tuple_2_star_chpl_bool_chpl call_tmp_chpl7 = NULL;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ArrayViewRankChangeDom_1_int64_t_one__tuple_2_star_chpl_bool__tuple_2_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_2_star_chpl_bool__tuple_2_star_int64_t_chpl coerce_tmp_chpl4 = NULL;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _ref__tuple_2_star_int64_t_chpl call_tmp_chpl8 = NULL;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _ref__tuple_1_star_range_int64_t_both_one_chpl i_dims_chpl = NULL;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl ranges_x0_chpl;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl ranges_x1_chpl;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl tup_x0_chpl;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl tup_x1_chpl;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl elt_x0_chpl;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl elt_x1_chpl;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  int64_t j_chpl;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl9 = NULL;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl5;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _ref_int64_t_chpl i_lhs_chpl = NULL;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl10;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl2;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl11 = NULL;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl6;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _ref_int64_t_chpl i_lhs_chpl2 = NULL;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl12;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl3;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _domain_DefaultRectangularDom_2_int64_t_one_chpl call_tmp_chpl13;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _domain_DefaultRectangularDom_2_int64_t_one_chpl ret_tmp_chpl4;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _domain_DefaultRectangularDom_2_int64_t_one_chpl shifted_chpl;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _ref_DefaultRectangularDom_2_int64_t_one_chpl _instance_chpl = NULL;
#line 830 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  DefaultRectangularArr_2_int64_t_one__real64_int64_t_chpl ret_chpl = NULL;
#line 830 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _ref__domain_DefaultRectangularDom_2_int64_t_one_chpl i_srcDom_chpl = NULL;
#line 830 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_bool call_tmp_chpl14;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  dims_chpl2(srcDom_chpl, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(call_tmp_chpl6 + INT64(0)) = *(ret_tmp_chpl + INT64(0));
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  coerce_tmp_chpl3 = (this_chpl31)->dom;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  call_tmp_chpl7 = &((coerce_tmp_chpl3)->collapsedDim);
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  coerce_tmp_chpl4 = (this_chpl31)->dom;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(coerce_tmp_chpl4, _ln_chpl, _fn_chpl);
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  call_tmp_chpl8 = &((coerce_tmp_chpl4)->idx);
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  i_dims_chpl = &call_tmp_chpl6;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  init_chpl113(&elt_x0_chpl);
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  tup_x0_chpl = elt_x0_chpl;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  init_chpl113(&elt_x1_chpl);
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  tup_x1_chpl = elt_x1_chpl;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ranges_x0_chpl = tup_x0_chpl;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ranges_x1_chpl = tup_x1_chpl;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  j_chpl = INT64(0);
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  if (! *(*(call_tmp_chpl7) + INT64(0))) /* ZLINE: 829 /Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl */
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  {
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    call_tmp_chpl9 = this_chpl20(i_dims_chpl, INT64(0), _ln_chpl, _fn_chpl);
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    coerce_tmp_chpl5 = *(call_tmp_chpl9);
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    ranges_x0_chpl = coerce_tmp_chpl5;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    i_lhs_chpl = &j_chpl;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    *(i_lhs_chpl) += INT64(1);
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  } else /* ZLINE: 829 /Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl */
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  {
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    createWithSingleElement_chpl(*(*(call_tmp_chpl8) + INT64(0)), &ret_tmp_chpl2);
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    call_tmp_chpl10 = ret_tmp_chpl2;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    ranges_x0_chpl = call_tmp_chpl10;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  }
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  if (! *(*(call_tmp_chpl7) + INT64(1))) /* ZLINE: 829 /Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl */
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  {
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    call_tmp_chpl11 = this_chpl20(i_dims_chpl, j_chpl, _ln_chpl, _fn_chpl);
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    coerce_tmp_chpl6 = *(call_tmp_chpl11);
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    ranges_x1_chpl = coerce_tmp_chpl6;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    i_lhs_chpl2 = &j_chpl;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    *(i_lhs_chpl2) += INT64(1);
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  } else /* ZLINE: 829 /Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl */
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  {
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    createWithSingleElement_chpl(*(*(call_tmp_chpl8) + INT64(1)), &ret_tmp_chpl3);
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    call_tmp_chpl12 = ret_tmp_chpl3;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    ranges_x1_chpl = call_tmp_chpl12;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  }
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl__buildDomainExpr2(ranges_x0_chpl, ranges_x1_chpl, UINT8(true), &ret_tmp_chpl4, _ln_chpl, _fn_chpl);
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  call_tmp_chpl13 = ret_tmp_chpl4;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  shifted_chpl = call_tmp_chpl13;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _instance_chpl = &((&shifted_chpl)->_instance);
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(_instance_chpl, _ln_chpl, _fn_chpl);
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ((BaseDom_chpl)(*(_instance_chpl)))->definedConst = UINT8(true);
#line 830 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 830 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ret_chpl = (this_chpl31)->chpl___ArrInstance;
#line 830 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  i_srcDom_chpl = &shifted_chpl;
#line 830 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(destClass_chpl, _ln_chpl, _fn_chpl);
#line 830 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  call_tmp_chpl14 = doiBulkTransferFromKnown_chpl13(destClass_chpl, destDom_chpl, ret_chpl, i_srcDom_chpl, _ln_chpl, _fn_chpl);
#line 830 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  if (call_tmp_chpl14) /* ZLINE: 830 /Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl */
#line 830 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  {
#line 830 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  } else /* ZLINE: 830 /Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl */
#line 830 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  {
#line 830 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  }
#line 830 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl__autoDestroy5(&shifted_chpl, _ln_chpl, _fn_chpl);
#line 827 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  return call_tmp_chpl14;
#line 827 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
}

#line 827 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
/* ArrayViewRankChange.chpl:827 */
#line 827 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
static chpl_bool doiBulkTransferToKnown_chpl4(ArrayViewRankChangeArr__real64_int64_t_ArrayViewRankChangeDom_2_int64_t_one__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_DefaultRectangularArr_3_int64_t_one__real64_int64_t__tuple_3_star_chpl_bool__tuple_3_star_int64_t__remoteAccessData__real64_2_int64_t_one_T_F_chpl this_chpl31,
#line 827 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                              _domain_ArrayViewRankChangeDom_2_int64_t_one__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_3_star_chpl_bool__tuple_3_star_int64_t_chpl * srcDom_chpl,
#line 827 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                              DefaultRectangularArr_3_int64_t_one__real64_int64_t_chpl destClass_chpl,
#line 827 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                              _domain_DefaultRectangularDom_3_int64_t_one_chpl * destDom_chpl,
#line 827 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                              int64_t _ln_chpl,
#line 827 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                              int32_t _fn_chpl) {
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _tuple_2_star_range_int64_t_both_one_chpl call_tmp_chpl6;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _tuple_2_star_range_int64_t_both_one_chpl ret_tmp_chpl;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ArrayViewRankChangeDom_2_int64_t_one__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_3_star_chpl_bool__tuple_3_star_int64_t_chpl coerce_tmp_chpl3 = NULL;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _ref__tuple_3_star_chpl_bool_chpl call_tmp_chpl7 = NULL;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ArrayViewRankChangeDom_2_int64_t_one__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_3_star_chpl_bool__tuple_3_star_int64_t_chpl coerce_tmp_chpl4 = NULL;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _ref__tuple_3_star_int64_t_chpl call_tmp_chpl8 = NULL;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _ref__tuple_2_star_range_int64_t_both_one_chpl i_dims_chpl = NULL;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl ranges_x0_chpl;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl ranges_x1_chpl;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl ranges_x2_chpl;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl tup_x0_chpl;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl tup_x1_chpl;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl tup_x2_chpl;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl elt_x0_chpl;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl elt_x1_chpl;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl elt_x2_chpl;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  int64_t j_chpl;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl9 = NULL;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl5;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _ref_int64_t_chpl i_lhs_chpl = NULL;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl10;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl2;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl11 = NULL;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl6;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _ref_int64_t_chpl i_lhs_chpl2 = NULL;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl12;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl3;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl13 = NULL;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl7;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _ref_int64_t_chpl i_lhs_chpl3 = NULL;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl14;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl4;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _domain_DefaultRectangularDom_3_int64_t_one_chpl call_tmp_chpl15;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _domain_DefaultRectangularDom_3_int64_t_one_chpl ret_tmp_chpl5;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _domain_DefaultRectangularDom_3_int64_t_one_chpl shifted_chpl;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _ref_DefaultRectangularDom_3_int64_t_one_chpl _instance_chpl = NULL;
#line 830 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  DefaultRectangularArr_3_int64_t_one__real64_int64_t_chpl ret_chpl = NULL;
#line 830 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _ref__domain_DefaultRectangularDom_3_int64_t_one_chpl i_srcDom_chpl = NULL;
#line 830 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_bool call_tmp_chpl16;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  dims_chpl6(srcDom_chpl, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(call_tmp_chpl6 + INT64(0)) = *(ret_tmp_chpl + INT64(0));
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(call_tmp_chpl6 + INT64(1)) = *(ret_tmp_chpl + INT64(1));
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  coerce_tmp_chpl3 = (this_chpl31)->dom;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  call_tmp_chpl7 = &((coerce_tmp_chpl3)->collapsedDim);
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  coerce_tmp_chpl4 = (this_chpl31)->dom;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(coerce_tmp_chpl4, _ln_chpl, _fn_chpl);
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  call_tmp_chpl8 = &((coerce_tmp_chpl4)->idx);
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  i_dims_chpl = &call_tmp_chpl6;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  init_chpl113(&elt_x0_chpl);
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  tup_x0_chpl = elt_x0_chpl;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  init_chpl113(&elt_x1_chpl);
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  tup_x1_chpl = elt_x1_chpl;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  init_chpl113(&elt_x2_chpl);
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  tup_x2_chpl = elt_x2_chpl;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ranges_x0_chpl = tup_x0_chpl;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ranges_x1_chpl = tup_x1_chpl;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ranges_x2_chpl = tup_x2_chpl;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  j_chpl = INT64(0);
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  if (! *(*(call_tmp_chpl7) + INT64(0))) /* ZLINE: 829 /Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl */
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  {
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    call_tmp_chpl9 = this_chpl26(i_dims_chpl, INT64(0), _ln_chpl, _fn_chpl);
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    coerce_tmp_chpl5 = *(call_tmp_chpl9);
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    ranges_x0_chpl = coerce_tmp_chpl5;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    i_lhs_chpl = &j_chpl;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    *(i_lhs_chpl) += INT64(1);
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  } else /* ZLINE: 829 /Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl */
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  {
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    createWithSingleElement_chpl(*(*(call_tmp_chpl8) + INT64(0)), &ret_tmp_chpl2);
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    call_tmp_chpl10 = ret_tmp_chpl2;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    ranges_x0_chpl = call_tmp_chpl10;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  }
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  if (! *(*(call_tmp_chpl7) + INT64(1))) /* ZLINE: 829 /Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl */
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  {
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    call_tmp_chpl11 = this_chpl26(i_dims_chpl, j_chpl, _ln_chpl, _fn_chpl);
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    coerce_tmp_chpl6 = *(call_tmp_chpl11);
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    ranges_x1_chpl = coerce_tmp_chpl6;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    i_lhs_chpl2 = &j_chpl;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    *(i_lhs_chpl2) += INT64(1);
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  } else /* ZLINE: 829 /Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl */
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  {
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    createWithSingleElement_chpl(*(*(call_tmp_chpl8) + INT64(1)), &ret_tmp_chpl3);
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    call_tmp_chpl12 = ret_tmp_chpl3;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    ranges_x1_chpl = call_tmp_chpl12;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  }
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  if (! *(*(call_tmp_chpl7) + INT64(2))) /* ZLINE: 829 /Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl */
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  {
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    call_tmp_chpl13 = this_chpl26(i_dims_chpl, j_chpl, _ln_chpl, _fn_chpl);
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    coerce_tmp_chpl7 = *(call_tmp_chpl13);
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    ranges_x2_chpl = coerce_tmp_chpl7;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    i_lhs_chpl3 = &j_chpl;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    *(i_lhs_chpl3) += INT64(1);
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  } else /* ZLINE: 829 /Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl */
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  {
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    createWithSingleElement_chpl(*(*(call_tmp_chpl8) + INT64(2)), &ret_tmp_chpl4);
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    call_tmp_chpl14 = ret_tmp_chpl4;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    ranges_x2_chpl = call_tmp_chpl14;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  }
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl__buildDomainExpr(ranges_x0_chpl, ranges_x1_chpl, ranges_x2_chpl, UINT8(true), &ret_tmp_chpl5, _ln_chpl, _fn_chpl);
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  call_tmp_chpl15 = ret_tmp_chpl5;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  shifted_chpl = call_tmp_chpl15;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _instance_chpl = &((&shifted_chpl)->_instance);
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(_instance_chpl, _ln_chpl, _fn_chpl);
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ((BaseDom_chpl)(*(_instance_chpl)))->definedConst = UINT8(true);
#line 830 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 830 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ret_chpl = (this_chpl31)->chpl___ArrInstance;
#line 830 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  i_srcDom_chpl = &shifted_chpl;
#line 830 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(destClass_chpl, _ln_chpl, _fn_chpl);
#line 830 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  call_tmp_chpl16 = doiBulkTransferFromKnown_chpl8(destClass_chpl, destDom_chpl, ret_chpl, i_srcDom_chpl, _ln_chpl, _fn_chpl);
#line 830 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  if (call_tmp_chpl16) /* ZLINE: 830 /Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl */
#line 830 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  {
#line 830 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  } else /* ZLINE: 830 /Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl */
#line 830 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  {
#line 830 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  }
#line 830 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl__autoDestroy8(&shifted_chpl, _ln_chpl, _fn_chpl);
#line 827 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  return call_tmp_chpl16;
#line 827 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
}

#line 827 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
/* ArrayViewRankChange.chpl:827 */
#line 827 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
static chpl_bool doiBulkTransferToKnown_chpl3(ArrayViewRankChangeArr__real64_int64_t_ArrayViewRankChangeDom_2_int64_t_one__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_DefaultRectangularArr_3_int64_t_one__real64_int64_t__tuple_3_star_chpl_bool__tuple_3_star_int64_t__remoteAccessData__real64_2_int64_t_one_T_F_chpl this_chpl31,
#line 827 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                              _domain_ArrayViewRankChangeDom_2_int64_t_one__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_3_star_chpl_bool__tuple_3_star_int64_t_chpl * srcDom_chpl,
#line 827 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                              DefaultRectangularArr_2_int64_t_one__real64_int64_t_chpl destClass_chpl,
#line 827 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                              _domain_DefaultRectangularDom_2_int64_t_one_chpl * destDom_chpl,
#line 827 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                              int64_t _ln_chpl,
#line 827 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
                                              int32_t _fn_chpl) {
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _tuple_2_star_range_int64_t_both_one_chpl call_tmp_chpl6;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _tuple_2_star_range_int64_t_both_one_chpl ret_tmp_chpl;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ArrayViewRankChangeDom_2_int64_t_one__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_3_star_chpl_bool__tuple_3_star_int64_t_chpl coerce_tmp_chpl3 = NULL;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _ref__tuple_3_star_chpl_bool_chpl call_tmp_chpl7 = NULL;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ArrayViewRankChangeDom_2_int64_t_one__tuple_3_star_chpl_bool__tuple_3_star_int64_t_int64_t_ArrayViewRankChangeDist_DefaultDist__tuple_3_star_chpl_bool__tuple_3_star_int64_t_chpl coerce_tmp_chpl4 = NULL;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _ref__tuple_3_star_int64_t_chpl call_tmp_chpl8 = NULL;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _ref__tuple_2_star_range_int64_t_both_one_chpl i_dims_chpl = NULL;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl ranges_x0_chpl;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl ranges_x1_chpl;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl ranges_x2_chpl;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl tup_x0_chpl;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl tup_x1_chpl;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl tup_x2_chpl;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl elt_x0_chpl;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl elt_x1_chpl;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl elt_x2_chpl;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  int64_t j_chpl;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl9 = NULL;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl5;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _ref_int64_t_chpl i_lhs_chpl = NULL;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl10;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl2;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl11 = NULL;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl6;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _ref_int64_t_chpl i_lhs_chpl2 = NULL;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl12;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl3;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _ref_range_int64_t_both_one_chpl call_tmp_chpl13 = NULL;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl coerce_tmp_chpl7;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _ref_int64_t_chpl i_lhs_chpl3 = NULL;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl call_tmp_chpl14;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  range_int64_t_both_one_chpl ret_tmp_chpl4;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _domain_DefaultRectangularDom_3_int64_t_one_chpl call_tmp_chpl15;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _domain_DefaultRectangularDom_3_int64_t_one_chpl ret_tmp_chpl5;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _domain_DefaultRectangularDom_3_int64_t_one_chpl shifted_chpl;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _ref_DefaultRectangularDom_3_int64_t_one_chpl _instance_chpl = NULL;
#line 830 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  DefaultRectangularArr_3_int64_t_one__real64_int64_t_chpl ret_chpl = NULL;
#line 830 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _ref__domain_DefaultRectangularDom_3_int64_t_one_chpl i_srcDom_chpl = NULL;
#line 830 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_bool call_tmp_chpl16;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  dims_chpl6(srcDom_chpl, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(call_tmp_chpl6 + INT64(0)) = *(ret_tmp_chpl + INT64(0));
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  *(call_tmp_chpl6 + INT64(1)) = *(ret_tmp_chpl + INT64(1));
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  coerce_tmp_chpl3 = (this_chpl31)->dom;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  call_tmp_chpl7 = &((coerce_tmp_chpl3)->collapsedDim);
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  coerce_tmp_chpl4 = (this_chpl31)->dom;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(coerce_tmp_chpl4, _ln_chpl, _fn_chpl);
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  call_tmp_chpl8 = &((coerce_tmp_chpl4)->idx);
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  i_dims_chpl = &call_tmp_chpl6;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  init_chpl113(&elt_x0_chpl);
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  tup_x0_chpl = elt_x0_chpl;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  init_chpl113(&elt_x1_chpl);
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  tup_x1_chpl = elt_x1_chpl;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  init_chpl113(&elt_x2_chpl);
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  tup_x2_chpl = elt_x2_chpl;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ranges_x0_chpl = tup_x0_chpl;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ranges_x1_chpl = tup_x1_chpl;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ranges_x2_chpl = tup_x2_chpl;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  j_chpl = INT64(0);
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  if (! *(*(call_tmp_chpl7) + INT64(0))) /* ZLINE: 829 /Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl */
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  {
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    call_tmp_chpl9 = this_chpl26(i_dims_chpl, INT64(0), _ln_chpl, _fn_chpl);
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    coerce_tmp_chpl5 = *(call_tmp_chpl9);
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    ranges_x0_chpl = coerce_tmp_chpl5;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    i_lhs_chpl = &j_chpl;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    *(i_lhs_chpl) += INT64(1);
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  } else /* ZLINE: 829 /Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl */
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  {
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    createWithSingleElement_chpl(*(*(call_tmp_chpl8) + INT64(0)), &ret_tmp_chpl2);
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    call_tmp_chpl10 = ret_tmp_chpl2;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    ranges_x0_chpl = call_tmp_chpl10;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  }
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  if (! *(*(call_tmp_chpl7) + INT64(1))) /* ZLINE: 829 /Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl */
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  {
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    call_tmp_chpl11 = this_chpl26(i_dims_chpl, j_chpl, _ln_chpl, _fn_chpl);
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    coerce_tmp_chpl6 = *(call_tmp_chpl11);
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    ranges_x1_chpl = coerce_tmp_chpl6;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    i_lhs_chpl2 = &j_chpl;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    *(i_lhs_chpl2) += INT64(1);
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  } else /* ZLINE: 829 /Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl */
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  {
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    createWithSingleElement_chpl(*(*(call_tmp_chpl8) + INT64(1)), &ret_tmp_chpl3);
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    call_tmp_chpl12 = ret_tmp_chpl3;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    ranges_x1_chpl = call_tmp_chpl12;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  }
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  if (! *(*(call_tmp_chpl7) + INT64(2))) /* ZLINE: 829 /Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl */
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  {
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    call_tmp_chpl13 = this_chpl26(i_dims_chpl, j_chpl, _ln_chpl, _fn_chpl);
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    coerce_tmp_chpl7 = *(call_tmp_chpl13);
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    ranges_x2_chpl = coerce_tmp_chpl7;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    i_lhs_chpl3 = &j_chpl;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    *(i_lhs_chpl3) += INT64(1);
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  } else /* ZLINE: 829 /Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl */
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  {
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    createWithSingleElement_chpl(*(*(call_tmp_chpl8) + INT64(2)), &ret_tmp_chpl4);
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    call_tmp_chpl14 = ret_tmp_chpl4;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
    ranges_x2_chpl = call_tmp_chpl14;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  }
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl__buildDomainExpr(ranges_x0_chpl, ranges_x1_chpl, ranges_x2_chpl, UINT8(true), &ret_tmp_chpl5, _ln_chpl, _fn_chpl);
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  call_tmp_chpl15 = ret_tmp_chpl5;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  shifted_chpl = call_tmp_chpl15;
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  _instance_chpl = &((&shifted_chpl)->_instance);
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(_instance_chpl, _ln_chpl, _fn_chpl);
#line 829 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ((BaseDom_chpl)(*(_instance_chpl)))->definedConst = UINT8(true);
#line 830 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(this_chpl31, _ln_chpl, _fn_chpl);
#line 830 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  ret_chpl = (this_chpl31)->chpl___ArrInstance;
#line 830 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  i_srcDom_chpl = &shifted_chpl;
#line 830 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl_check_nil(destClass_chpl, _ln_chpl, _fn_chpl);
#line 830 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  call_tmp_chpl16 = doiBulkTransferFromKnown_chpl15(destClass_chpl, destDom_chpl, ret_chpl, i_srcDom_chpl, _ln_chpl, _fn_chpl);
#line 830 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  if (call_tmp_chpl16) /* ZLINE: 830 /Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl */
#line 830 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  {
#line 830 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  } else /* ZLINE: 830 /Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl */
#line 830 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  {
#line 830 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  }
#line 830 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  chpl__autoDestroy8(&shifted_chpl, _ln_chpl, _fn_chpl);
#line 827 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
  return call_tmp_chpl16;
#line 827 "/Users/iainmoncrief/Documents/chapel/modules/internal/ArrayViewRankChange.chpl"
}

