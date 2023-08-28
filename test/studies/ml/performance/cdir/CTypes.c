#line 41 "/Users/iainmoncrief/Documents/chapel/modules/standard/CTypes.chpl"
/* CTypes.chpl:41 */
#line 41 "/Users/iainmoncrief/Documents/chapel/modules/standard/CTypes.chpl"
static void chpl__init_CTypes(int64_t _ln_chpl,
#line 41 "/Users/iainmoncrief/Documents/chapel/modules/standard/CTypes.chpl"
                              int32_t _fn_chpl) {
#line 41 "/Users/iainmoncrief/Documents/chapel/modules/standard/CTypes.chpl"
  _ref_int32_t_chpl refIndentLevel_chpl = NULL;
#line 41 "/Users/iainmoncrief/Documents/chapel/modules/standard/CTypes.chpl"
  if (chpl__init_CTypes_p) /* ZLINE: 41 /Users/iainmoncrief/Documents/chapel/modules/standard/CTypes.chpl */
#line 41 "/Users/iainmoncrief/Documents/chapel/modules/standard/CTypes.chpl"
  {
#line 41 "/Users/iainmoncrief/Documents/chapel/modules/standard/CTypes.chpl"
    goto _exit_chpl__init_CTypes_chpl;
#line 41 "/Users/iainmoncrief/Documents/chapel/modules/standard/CTypes.chpl"
  }
#line 41 "/Users/iainmoncrief/Documents/chapel/modules/standard/CTypes.chpl"
  printModuleInit_chpl("%*s\n", "CTypes", INT64(6), _ln_chpl, _fn_chpl);
#line 41 "/Users/iainmoncrief/Documents/chapel/modules/standard/CTypes.chpl"
  refIndentLevel_chpl = &moduleInitLevel_chpl;
#line 41 "/Users/iainmoncrief/Documents/chapel/modules/standard/CTypes.chpl"
  *(refIndentLevel_chpl) += INT64(1);
#line 41 "/Users/iainmoncrief/Documents/chapel/modules/standard/CTypes.chpl"
  chpl__init_CTypes_p = UINT8(true);
#line 41 "/Users/iainmoncrief/Documents/chapel/modules/standard/CTypes.chpl"
  {
#line 41 "/Users/iainmoncrief/Documents/chapel/modules/standard/CTypes.chpl"
    chpl__init_HaltWrappers(_ln_chpl, _fn_chpl);
#line 41 "/Users/iainmoncrief/Documents/chapel/modules/standard/CTypes.chpl"
    chpl__init_ChapelSysCTypes(_ln_chpl, _fn_chpl);
#line 41 "/Users/iainmoncrief/Documents/chapel/modules/standard/CTypes.chpl"
    chpl__init_Reflection(_ln_chpl, _fn_chpl);
#line 41 "/Users/iainmoncrief/Documents/chapel/modules/standard/CTypes.chpl"
    chpl__init_POSIX(_ln_chpl, _fn_chpl);
#line 41 "/Users/iainmoncrief/Documents/chapel/modules/standard/CTypes.chpl"
    chpl__init_AutoMath(_ln_chpl, _fn_chpl);
#line 41 "/Users/iainmoncrief/Documents/chapel/modules/standard/CTypes.chpl"
  }
#line 41 "/Users/iainmoncrief/Documents/chapel/modules/standard/CTypes.chpl"
  *(refIndentLevel_chpl) -= INT64(1);
#line 41 "/Users/iainmoncrief/Documents/chapel/modules/standard/CTypes.chpl"
  _exit_chpl__init_CTypes_chpl:;
#line 41 "/Users/iainmoncrief/Documents/chapel/modules/standard/CTypes.chpl"
  return;
#line 41 "/Users/iainmoncrief/Documents/chapel/modules/standard/CTypes.chpl"
}

#line 1189 "/Users/iainmoncrief/Documents/chapel/modules/standard/CTypes.chpl"
/* CTypes.chpl:1189 */
#line 1189 "/Users/iainmoncrief/Documents/chapel/modules/standard/CTypes.chpl"
static int16_t offset_ARRAY_ELEMENTS_chpl(void) {
#line 1193 "/Users/iainmoncrief/Documents/chapel/modules/standard/CTypes.chpl"
  int16_t call_tmp_chpl6;
#line 1193 "/Users/iainmoncrief/Documents/chapel/modules/standard/CTypes.chpl"
  int16_t call_tmp_chpl7;
#line 1193 "/Users/iainmoncrief/Documents/chapel/modules/standard/CTypes.chpl"
  call_tmp_chpl6 = chpl_memhook_md_num();
#line 1193 "/Users/iainmoncrief/Documents/chapel/modules/standard/CTypes.chpl"
  call_tmp_chpl7 = (CHPL_RT_MD_ARRAY_ELEMENTS - call_tmp_chpl6);
#line 1189 "/Users/iainmoncrief/Documents/chapel/modules/standard/CTypes.chpl"
  return call_tmp_chpl7;
#line 1189 "/Users/iainmoncrief/Documents/chapel/modules/standard/CTypes.chpl"
}

