#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelPrivatization.chpl"
/* ChapelPrivatization.chpl:21 */
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelPrivatization.chpl"
static void chpl__init_ChapelPrivatization(int64_t _ln_chpl,
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelPrivatization.chpl"
                                           int32_t _fn_chpl) {
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelPrivatization.chpl"
  _ref_int32_t_chpl refIndentLevel_chpl = NULL;
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelPrivatization.chpl"
  if (chpl__init_ChapelPrivatization_p) /* ZLINE: 21 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelPrivatization.chpl */
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelPrivatization.chpl"
  {
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelPrivatization.chpl"
    goto _exit_chpl__init_ChapelPrivatization_chpl;
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelPrivatization.chpl"
  }
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelPrivatization.chpl"
  printModuleInit_chpl("%*s\n", "ChapelPrivatization", INT64(19), _ln_chpl, _fn_chpl);
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelPrivatization.chpl"
  refIndentLevel_chpl = &moduleInitLevel_chpl;
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelPrivatization.chpl"
  *(refIndentLevel_chpl) += INT64(1);
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelPrivatization.chpl"
  chpl__init_ChapelPrivatization_p = UINT8(true);
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelPrivatization.chpl"
  {
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelPrivatization.chpl"
    chpl__init_CTypes(_ln_chpl, _fn_chpl);
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelPrivatization.chpl"
  }
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelPrivatization.chpl"
  *(refIndentLevel_chpl) -= INT64(1);
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelPrivatization.chpl"
  _exit_chpl__init_ChapelPrivatization_chpl:;
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelPrivatization.chpl"
  return;
#line 21 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelPrivatization.chpl"
}

