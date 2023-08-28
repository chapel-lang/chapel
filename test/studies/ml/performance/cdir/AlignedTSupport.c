#line 1219 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelSyncvar.chpl"
/* ChapelSyncvar.chpl:1219 */
#line 1219 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelSyncvar.chpl"
static void chpl__init_AlignedTSupport(int64_t _ln_chpl,
#line 1219 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelSyncvar.chpl"
                                       int32_t _fn_chpl) {
#line 1219 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelSyncvar.chpl"
  _ref_int32_t_chpl refIndentLevel_chpl = NULL;
#line 1219 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelSyncvar.chpl"
  if (chpl__init_AlignedTSupport_p) /* ZLINE: 1219 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelSyncvar.chpl */
#line 1219 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelSyncvar.chpl"
  {
#line 1219 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelSyncvar.chpl"
    goto _exit_chpl__init_AlignedTSupport_chpl;
#line 1219 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelSyncvar.chpl"
  }
#line 1219 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelSyncvar.chpl"
  printModuleInit_chpl("%*s\n", "AlignedTSupport", INT64(15), _ln_chpl, _fn_chpl);
#line 1219 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelSyncvar.chpl"
  refIndentLevel_chpl = &moduleInitLevel_chpl;
#line 1219 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelSyncvar.chpl"
  *(refIndentLevel_chpl) += INT64(1);
#line 1219 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelSyncvar.chpl"
  chpl__init_AlignedTSupport_p = UINT8(true);
#line 1219 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelSyncvar.chpl"
  *(refIndentLevel_chpl) -= INT64(1);
#line 1219 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelSyncvar.chpl"
  _exit_chpl__init_AlignedTSupport_chpl:;
#line 1219 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelSyncvar.chpl"
  return;
#line 1219 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelSyncvar.chpl"
}

