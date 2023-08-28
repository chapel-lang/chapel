#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelSerializedBroadcast.chpl"
/* ChapelSerializedBroadcast.chpl:24 */
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelSerializedBroadcast.chpl"
static void chpl__init_ChapelSerializedBroadcast(int64_t _ln_chpl,
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelSerializedBroadcast.chpl"
                                                 int32_t _fn_chpl) {
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelSerializedBroadcast.chpl"
  c_string_rehook modFormatStr_chpl;
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelSerializedBroadcast.chpl"
  c_string_rehook modStr_chpl;
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelSerializedBroadcast.chpl"
  _ref_int32_t_chpl refIndentLevel_chpl = NULL;
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelSerializedBroadcast.chpl"
  if (chpl__init_ChapelSerializedBroadcast_p) /* ZLINE: 24 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelSerializedBroadcast.chpl */
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelSerializedBroadcast.chpl"
  {
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelSerializedBroadcast.chpl"
    goto _exit_chpl__init_ChapelSerializedBroadcast_chpl;
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelSerializedBroadcast.chpl"
  }
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelSerializedBroadcast.chpl"
  modFormatStr_chpl = "%*s\n";
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelSerializedBroadcast.chpl"
  modStr_chpl = "ChapelSerializedBroadcast";
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelSerializedBroadcast.chpl"
  printModuleInit_chpl(modFormatStr_chpl, modStr_chpl, INT64(25), _ln_chpl, _fn_chpl);
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelSerializedBroadcast.chpl"
  refIndentLevel_chpl = &moduleInitLevel_chpl;
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelSerializedBroadcast.chpl"
  *(refIndentLevel_chpl) += INT64(1);
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelSerializedBroadcast.chpl"
  chpl__init_ChapelSerializedBroadcast_p = UINT8(true);
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelSerializedBroadcast.chpl"
  {
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelSerializedBroadcast.chpl"
    chpl__init_ChapelLocale(_ln_chpl, _fn_chpl);
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelSerializedBroadcast.chpl"
    chpl__init_CTypes(_ln_chpl, _fn_chpl);
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelSerializedBroadcast.chpl"
  }
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelSerializedBroadcast.chpl"
  *(refIndentLevel_chpl) -= INT64(1);
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelSerializedBroadcast.chpl"
  _endchpl__init_ChapelSerializedBroadcast_chpl:;
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelSerializedBroadcast.chpl"
  _exit_chpl__init_ChapelSerializedBroadcast_chpl:;
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelSerializedBroadcast.chpl"
  return;
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelSerializedBroadcast.chpl"
}

