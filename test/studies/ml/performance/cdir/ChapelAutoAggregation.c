#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelAutoAggregation.chpl"
/* ChapelAutoAggregation.chpl:21 */
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelAutoAggregation.chpl"
static void chpl__init_ChapelAutoAggregation(int64_t _ln_chpl,
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelAutoAggregation.chpl"
                                             int32_t _fn_chpl) {
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelAutoAggregation.chpl"
  c_string_rehook modFormatStr_chpl;
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelAutoAggregation.chpl"
  c_string_rehook modStr_chpl;
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelAutoAggregation.chpl"
  _ref_int32_t_chpl refIndentLevel_chpl = NULL;
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelAutoAggregation.chpl"
  if (chpl__init_ChapelAutoAggregation_p) /* ZLINE: 21 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelAutoAggregation.chpl */
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelAutoAggregation.chpl"
  {
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelAutoAggregation.chpl"
    goto _exit_chpl__init_ChapelAutoAggregation_chpl;
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelAutoAggregation.chpl"
  }
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelAutoAggregation.chpl"
  modFormatStr_chpl = "%*s\n";
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelAutoAggregation.chpl"
  modStr_chpl = "ChapelAutoAggregation";
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelAutoAggregation.chpl"
  printModuleInit_chpl(modFormatStr_chpl, modStr_chpl, INT64(21), _ln_chpl, _fn_chpl);
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelAutoAggregation.chpl"
  refIndentLevel_chpl = &moduleInitLevel_chpl;
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelAutoAggregation.chpl"
  *(refIndentLevel_chpl) += INT64(1);
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelAutoAggregation.chpl"
  chpl__init_ChapelAutoAggregation_p = UINT8(true);
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelAutoAggregation.chpl"
  {
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelAutoAggregation.chpl"
    chpl__init_CopyAggregation(_ln_chpl, _fn_chpl);
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelAutoAggregation.chpl"
  }
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelAutoAggregation.chpl"
  *(refIndentLevel_chpl) -= INT64(1);
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelAutoAggregation.chpl"
  _endchpl__init_ChapelAutoAggregation_chpl:;
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelAutoAggregation.chpl"
  _exit_chpl__init_ChapelAutoAggregation_chpl:;
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelAutoAggregation.chpl"
  return;
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelAutoAggregation.chpl"
}

