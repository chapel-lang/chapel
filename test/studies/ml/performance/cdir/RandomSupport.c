#line 747 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
/* Random.chpl:747 */
#line 747 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
static void chpl__init_RandomSupport(int64_t _ln_chpl,
#line 747 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
                                     int32_t _fn_chpl) {
#line 747 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  _ref_int32_t_chpl refIndentLevel_chpl = NULL;
#line 747 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  if (chpl__init_RandomSupport_p) /* ZLINE: 747 /Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl */
#line 747 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  {
#line 747 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    goto _exit_chpl__init_RandomSupport_chpl;
#line 747 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  }
#line 747 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  printModuleInit_chpl("%*s\n", "RandomSupport", INT64(13), _ln_chpl, _fn_chpl);
#line 747 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  refIndentLevel_chpl = &moduleInitLevel_chpl;
#line 747 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  *(refIndentLevel_chpl) += INT64(1);
#line 747 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  chpl__init_RandomSupport_p = UINT8(true);
#line 747 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  {
#line 747 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    chpl__init_Random(_ln_chpl, _fn_chpl);
#line 747 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
    chpl__init_Time(_ln_chpl, _fn_chpl);
#line 747 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  }
#line 747 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  *(refIndentLevel_chpl) -= INT64(1);
#line 747 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  _exit_chpl__init_RandomSupport_chpl:;
#line 747 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
  return;
#line 747 "/Users/iainmoncrief/Documents/chapel/modules/standard/Random.chpl"
}

