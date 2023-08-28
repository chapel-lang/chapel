#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/DefaultAssociative.chpl"
/* DefaultAssociative.chpl:24 */
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/DefaultAssociative.chpl"
static void chpl__init_DefaultAssociative(int64_t _ln_chpl,
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/DefaultAssociative.chpl"
                                          int32_t _fn_chpl) {
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/DefaultAssociative.chpl"
  c_string_rehook modFormatStr_chpl;
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/DefaultAssociative.chpl"
  c_string_rehook modStr_chpl;
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/DefaultAssociative.chpl"
  _ref_int32_t_chpl refIndentLevel_chpl = NULL;
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/DefaultAssociative.chpl"
  if (chpl__init_DefaultAssociative_p) /* ZLINE: 24 /Users/iainmoncrief/Documents/chapel/modules/internal/DefaultAssociative.chpl */
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/DefaultAssociative.chpl"
  {
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/DefaultAssociative.chpl"
    goto _exit_chpl__init_DefaultAssociative_chpl;
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/DefaultAssociative.chpl"
  }
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/DefaultAssociative.chpl"
  modFormatStr_chpl = "%*s\n";
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/DefaultAssociative.chpl"
  modStr_chpl = "DefaultAssociative";
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/DefaultAssociative.chpl"
  printModuleInit_chpl(modFormatStr_chpl, modStr_chpl, INT64(18), _ln_chpl, _fn_chpl);
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/DefaultAssociative.chpl"
  refIndentLevel_chpl = &moduleInitLevel_chpl;
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/DefaultAssociative.chpl"
  *(refIndentLevel_chpl) += INT64(1);
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/DefaultAssociative.chpl"
  chpl__init_DefaultAssociative_p = UINT8(true);
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/DefaultAssociative.chpl"
  {
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/DefaultAssociative.chpl"
    chpl__init_DSIUtil(_ln_chpl, _fn_chpl);
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/DefaultAssociative.chpl"
    chpl__init_ChapelDistribution(_ln_chpl, _fn_chpl);
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/DefaultAssociative.chpl"
    chpl__init_ChapelRange(_ln_chpl, _fn_chpl);
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/DefaultAssociative.chpl"
    chpl__init_ChapelArray(_ln_chpl, _fn_chpl);
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/DefaultAssociative.chpl"
    chpl__init_ChapelBase(_ln_chpl, _fn_chpl);
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/DefaultAssociative.chpl"
    chpl__init_ChapelLocks(_ln_chpl, _fn_chpl);
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/DefaultAssociative.chpl"
    chpl__init_IO(_ln_chpl, _fn_chpl);
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/DefaultAssociative.chpl"
    chpl__init_ChapelHashing(_ln_chpl, _fn_chpl);
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/DefaultAssociative.chpl"
    chpl__init_ChapelHashtable(_ln_chpl, _fn_chpl);
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/DefaultAssociative.chpl"
    chpl__init_OS(_ln_chpl, _fn_chpl);
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/DefaultAssociative.chpl"
    chpl__init_Sort(_ln_chpl, _fn_chpl);
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/DefaultAssociative.chpl"
  }
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/DefaultAssociative.chpl"
  *(refIndentLevel_chpl) -= INT64(1);
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/DefaultAssociative.chpl"
  _endchpl__init_DefaultAssociative_chpl:;
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/DefaultAssociative.chpl"
  _exit_chpl__init_DefaultAssociative_chpl:;
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/DefaultAssociative.chpl"
  return;
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/DefaultAssociative.chpl"
}

