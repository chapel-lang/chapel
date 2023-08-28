#line 38 "/Users/iainmoncrief/Documents/chapel/modules/packages/RangeChunk.chpl"
/* RangeChunk.chpl:38 */
#line 38 "/Users/iainmoncrief/Documents/chapel/modules/packages/RangeChunk.chpl"
static void chpl__init_RangeChunk(int64_t _ln_chpl,
#line 38 "/Users/iainmoncrief/Documents/chapel/modules/packages/RangeChunk.chpl"
                                  int32_t _fn_chpl) {
#line 38 "/Users/iainmoncrief/Documents/chapel/modules/packages/RangeChunk.chpl"
  _ref_int32_t_chpl refIndentLevel_chpl = NULL;
#line 38 "/Users/iainmoncrief/Documents/chapel/modules/packages/RangeChunk.chpl"
  if (chpl__init_RangeChunk_p) /* ZLINE: 38 /Users/iainmoncrief/Documents/chapel/modules/packages/RangeChunk.chpl */
#line 38 "/Users/iainmoncrief/Documents/chapel/modules/packages/RangeChunk.chpl"
  {
#line 38 "/Users/iainmoncrief/Documents/chapel/modules/packages/RangeChunk.chpl"
    goto _exit_chpl__init_RangeChunk_chpl;
#line 38 "/Users/iainmoncrief/Documents/chapel/modules/packages/RangeChunk.chpl"
  }
#line 38 "/Users/iainmoncrief/Documents/chapel/modules/packages/RangeChunk.chpl"
  printModuleInit_chpl("%*s\n", "RangeChunk", INT64(10), _ln_chpl, _fn_chpl);
#line 38 "/Users/iainmoncrief/Documents/chapel/modules/packages/RangeChunk.chpl"
  refIndentLevel_chpl = &moduleInitLevel_chpl;
#line 38 "/Users/iainmoncrief/Documents/chapel/modules/packages/RangeChunk.chpl"
  *(refIndentLevel_chpl) += INT64(1);
#line 38 "/Users/iainmoncrief/Documents/chapel/modules/packages/RangeChunk.chpl"
  chpl__init_RangeChunk_p = UINT8(true);
#line 38 "/Users/iainmoncrief/Documents/chapel/modules/packages/RangeChunk.chpl"
  {
#line 38 "/Users/iainmoncrief/Documents/chapel/modules/packages/RangeChunk.chpl"
    chpl__init_CTypes(_ln_chpl, _fn_chpl);
#line 38 "/Users/iainmoncrief/Documents/chapel/modules/packages/RangeChunk.chpl"
    chpl__init_AutoMath(_ln_chpl, _fn_chpl);
#line 38 "/Users/iainmoncrief/Documents/chapel/modules/packages/RangeChunk.chpl"
  }
#line 38 "/Users/iainmoncrief/Documents/chapel/modules/packages/RangeChunk.chpl"
  *(refIndentLevel_chpl) -= INT64(1);
#line 38 "/Users/iainmoncrief/Documents/chapel/modules/packages/RangeChunk.chpl"
  _exit_chpl__init_RangeChunk_chpl:;
#line 38 "/Users/iainmoncrief/Documents/chapel/modules/packages/RangeChunk.chpl"
  return;
#line 38 "/Users/iainmoncrief/Documents/chapel/modules/packages/RangeChunk.chpl"
}

