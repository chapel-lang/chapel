#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskID.chpl"
/* ChapelTaskID.chpl:23 */
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskID.chpl"
static void chpl__init_ChapelTaskID(int64_t _ln_chpl,
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskID.chpl"
                                    int32_t _fn_chpl) {
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskID.chpl"
  c_string_rehook modFormatStr_chpl;
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskID.chpl"
  c_string_rehook modStr_chpl;
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskID.chpl"
  _ref_int32_t_chpl refIndentLevel_chpl = NULL;
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskID.chpl"
  if (chpl__init_ChapelTaskID_p) /* ZLINE: 23 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskID.chpl */
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskID.chpl"
  {
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskID.chpl"
    goto _exit_chpl__init_ChapelTaskID_chpl;
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskID.chpl"
  }
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskID.chpl"
  modFormatStr_chpl = "%*s\n";
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskID.chpl"
  modStr_chpl = "ChapelTaskID";
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskID.chpl"
  printModuleInit_chpl(modFormatStr_chpl, modStr_chpl, INT64(12), _ln_chpl, _fn_chpl);
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskID.chpl"
  refIndentLevel_chpl = &moduleInitLevel_chpl;
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskID.chpl"
  *(refIndentLevel_chpl) += INT64(1);
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskID.chpl"
  chpl__init_ChapelTaskID_p = UINT8(true);
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskID.chpl"
  *(refIndentLevel_chpl) -= INT64(1);
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskID.chpl"
  _endchpl__init_ChapelTaskID_chpl:;
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskID.chpl"
  _exit_chpl__init_ChapelTaskID_chpl:;
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskID.chpl"
  return;
#line 23 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelTaskID.chpl"
}

