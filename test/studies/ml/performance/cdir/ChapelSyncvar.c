#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelSyncvar.chpl"
/* ChapelSyncvar.chpl:24 */
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelSyncvar.chpl"
static void chpl__init_ChapelSyncvar(int64_t _ln_chpl,
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelSyncvar.chpl"
                                     int32_t _fn_chpl) {
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelSyncvar.chpl"
  c_string_rehook modFormatStr_chpl;
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelSyncvar.chpl"
  c_string_rehook modStr_chpl;
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelSyncvar.chpl"
  _ref_int32_t_chpl refIndentLevel_chpl = NULL;
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelSyncvar.chpl"
  if (chpl__init_ChapelSyncvar_p) /* ZLINE: 24 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelSyncvar.chpl */
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelSyncvar.chpl"
  {
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelSyncvar.chpl"
    goto _exit_chpl__init_ChapelSyncvar_chpl;
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelSyncvar.chpl"
  }
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelSyncvar.chpl"
  modFormatStr_chpl = "%*s\n";
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelSyncvar.chpl"
  modStr_chpl = "ChapelSyncvar";
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelSyncvar.chpl"
  printModuleInit_chpl(modFormatStr_chpl, modStr_chpl, INT64(13), _ln_chpl, _fn_chpl);
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelSyncvar.chpl"
  refIndentLevel_chpl = &moduleInitLevel_chpl;
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelSyncvar.chpl"
  *(refIndentLevel_chpl) += INT64(1);
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelSyncvar.chpl"
  chpl__init_ChapelSyncvar_p = UINT8(true);
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelSyncvar.chpl"
  {
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelSyncvar.chpl"
    chpl__init_AlignedTSupport(_ln_chpl, _fn_chpl);
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelSyncvar.chpl"
    chpl__init_MemConsistency(_ln_chpl, _fn_chpl);
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelSyncvar.chpl"
    chpl__init_SyncVarRuntimeSupport(_ln_chpl, _fn_chpl);
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelSyncvar.chpl"
    chpl__init_CTypes(_ln_chpl, _fn_chpl);
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelSyncvar.chpl"
    chpl__init_POSIX(_ln_chpl, _fn_chpl);
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelSyncvar.chpl"
    chpl__init_currentTask(_ln_chpl, _fn_chpl);
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelSyncvar.chpl"
  }
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelSyncvar.chpl"
  *(refIndentLevel_chpl) -= INT64(1);
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelSyncvar.chpl"
  _endchpl__init_ChapelSyncvar_chpl:;
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelSyncvar.chpl"
  _exit_chpl__init_ChapelSyncvar_chpl:;
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelSyncvar.chpl"
  return;
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelSyncvar.chpl"
}

