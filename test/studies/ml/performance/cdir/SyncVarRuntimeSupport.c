#line 1135 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelSyncvar.chpl"
/* ChapelSyncvar.chpl:1135 */
#line 1135 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelSyncvar.chpl"
static void chpl__init_SyncVarRuntimeSupport(int64_t _ln_chpl,
#line 1135 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelSyncvar.chpl"
                                             int32_t _fn_chpl) {
#line 1135 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelSyncvar.chpl"
  c_string_rehook modFormatStr_chpl;
#line 1135 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelSyncvar.chpl"
  c_string_rehook modStr_chpl;
#line 1135 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelSyncvar.chpl"
  _ref_int32_t_chpl refIndentLevel_chpl = NULL;
#line 1135 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelSyncvar.chpl"
  if (chpl__init_SyncVarRuntimeSupport_p) /* ZLINE: 1135 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelSyncvar.chpl */
#line 1135 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelSyncvar.chpl"
  {
#line 1135 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelSyncvar.chpl"
    goto _exit_chpl__init_SyncVarRuntimeSupport_chpl;
#line 1135 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelSyncvar.chpl"
  }
#line 1135 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelSyncvar.chpl"
  modFormatStr_chpl = "%*s\n";
#line 1135 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelSyncvar.chpl"
  modStr_chpl = "SyncVarRuntimeSupport";
#line 1135 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelSyncvar.chpl"
  printModuleInit_chpl(modFormatStr_chpl, modStr_chpl, INT64(21), _ln_chpl, _fn_chpl);
#line 1135 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelSyncvar.chpl"
  refIndentLevel_chpl = &moduleInitLevel_chpl;
#line 1135 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelSyncvar.chpl"
  *(refIndentLevel_chpl) += INT64(1);
#line 1135 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelSyncvar.chpl"
  chpl__init_SyncVarRuntimeSupport_p = UINT8(true);
#line 1135 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelSyncvar.chpl"
  {
#line 1135 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelSyncvar.chpl"
    chpl__init_CTypes(_ln_chpl, _fn_chpl);
#line 1135 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelSyncvar.chpl"
    chpl__init_AlignedTSupport(_ln_chpl, _fn_chpl);
#line 1135 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelSyncvar.chpl"
    chpl__init_ChplConfig(_ln_chpl, _fn_chpl);
#line 1135 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelSyncvar.chpl"
  }
#line 1135 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelSyncvar.chpl"
  *(refIndentLevel_chpl) -= INT64(1);
#line 1135 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelSyncvar.chpl"
  _endchpl__init_SyncVarRuntimeSupport_chpl:;
#line 1135 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelSyncvar.chpl"
  _exit_chpl__init_SyncVarRuntimeSupport_chpl:;
#line 1135 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelSyncvar.chpl"
  return;
#line 1135 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelSyncvar.chpl"
}

