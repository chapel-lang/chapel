#line 30 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelHashtable.chpl"
/* ChapelHashtable.chpl:30 */
#line 30 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelHashtable.chpl"
static void chpl__init_ChapelHashtable(int64_t _ln_chpl,
#line 30 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelHashtable.chpl"
                                       int32_t _fn_chpl) {
#line 30 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelHashtable.chpl"
  _ref_int32_t_chpl refIndentLevel_chpl = NULL;
#line 30 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelHashtable.chpl"
  if (chpl__init_ChapelHashtable_p) /* ZLINE: 30 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelHashtable.chpl */
#line 30 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelHashtable.chpl"
  {
#line 30 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelHashtable.chpl"
    goto _exit_chpl__init_ChapelHashtable_chpl;
#line 30 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelHashtable.chpl"
  }
#line 30 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelHashtable.chpl"
  printModuleInit_chpl("%*s\n", "ChapelHashtable", INT64(15), _ln_chpl, _fn_chpl);
#line 30 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelHashtable.chpl"
  refIndentLevel_chpl = &moduleInitLevel_chpl;
#line 30 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelHashtable.chpl"
  *(refIndentLevel_chpl) += INT64(1);
#line 30 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelHashtable.chpl"
  chpl__init_ChapelHashtable_p = UINT8(true);
#line 30 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelHashtable.chpl"
  {
#line 30 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelHashtable.chpl"
    chpl__init_ChapelBase(_ln_chpl, _fn_chpl);
#line 30 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelHashtable.chpl"
    chpl__init_DSIUtil(_ln_chpl, _fn_chpl);
#line 30 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelHashtable.chpl"
    chpl__init_CTypes(_ln_chpl, _fn_chpl);
#line 30 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelHashtable.chpl"
    chpl__init_POSIX(_ln_chpl, _fn_chpl);
#line 30 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelHashtable.chpl"
    chpl__init_AutoMath(_ln_chpl, _fn_chpl);
#line 30 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelHashtable.chpl"
    chpl__init_String(_ln_chpl, _fn_chpl);
#line 30 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelHashtable.chpl"
    chpl__init_HaltWrappers(_ln_chpl, _fn_chpl);
#line 30 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelHashtable.chpl"
    chpl__init_IO(_ln_chpl, _fn_chpl);
#line 30 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelHashtable.chpl"
  }
#line 30 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelHashtable.chpl"
  *(refIndentLevel_chpl) -= INT64(1);
#line 30 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelHashtable.chpl"
  _exit_chpl__init_ChapelHashtable_chpl:;
#line 30 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelHashtable.chpl"
  return;
#line 30 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelHashtable.chpl"
}

#line 226 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelHashtable.chpl"
/* ChapelHashtable.chpl:226 */
#line 226 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelHashtable.chpl"
static void chpl__auto_destroy_chpl__rehashHelpers(chpl__rehashHelpers this_chpl31,
#line 226 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelHashtable.chpl"
                                                   int64_t _ln_chpl,
#line 226 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelHashtable.chpl"
                                                   int32_t _fn_chpl) {
#line 226 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelHashtable.chpl"
  return;
#line 226 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelHashtable.chpl"
}

