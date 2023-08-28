#line 41 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocalesArray.chpl"
/* LocalesArray.chpl:41 */
#line 41 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocalesArray.chpl"
static void chpl__init_LocalesArray(int64_t _ln_chpl,
#line 41 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocalesArray.chpl"
                                    int32_t _fn_chpl) {
#line 41 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocalesArray.chpl"
  _ref_int32_t_chpl refIndentLevel_chpl = NULL;
#line 54 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocalesArray.chpl"
  RootLocale_chpl _formal_type_tmp__chpl = NULL;
#line 54 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocalesArray.chpl"
  RootLocale_chpl ret_chpl = NULL;
#line 58 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocalesArray.chpl"
  _domain_DefaultRectangularDom_1_int64_t_one_chpl call_tmp_chpl6;
#line 58 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocalesArray.chpl"
  _domain_DefaultRectangularDom_1_int64_t_one_chpl ret_tmp_chpl;
#line 58 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocalesArray.chpl"
  int32_t chpl_macro_tmp_62;
#line 41 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocalesArray.chpl"
  if (chpl__init_LocalesArray_p) /* ZLINE: 41 /Users/iainmoncrief/Documents/chapel/modules/internal/LocalesArray.chpl */
#line 41 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocalesArray.chpl"
  {
#line 41 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocalesArray.chpl"
    goto _exit_chpl__init_LocalesArray_chpl;
#line 41 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocalesArray.chpl"
  }
#line 41 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocalesArray.chpl"
  printModuleInit_chpl("%*s\n", "LocalesArray", INT64(12), _ln_chpl, _fn_chpl);
#line 41 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocalesArray.chpl"
  refIndentLevel_chpl = &moduleInitLevel_chpl;
#line 41 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocalesArray.chpl"
  *(refIndentLevel_chpl) += INT64(1);
#line 41 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocalesArray.chpl"
  chpl__init_LocalesArray_p = UINT8(true);
#line 41 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocalesArray.chpl"
  chpl_addModule("LocalesArray", ((c_fn_ptr)(chpl__deinit_LocalesArray)), _ln_chpl, _fn_chpl);
#line 45 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocalesArray.chpl"
  chpl_init_rootLocale(_ln_chpl, _fn_chpl);
#line 54 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocalesArray.chpl"
  call_tmp_chpl4 = &((&rootLocale_chpl)->_instance);
#line 54 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocalesArray.chpl"
  coerce_tmp_chpl2 = *(call_tmp_chpl4);
#line 54 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocalesArray.chpl"
  coerce_tmp_chpl = coerce_tmp_chpl2;
#line 54 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocalesArray.chpl"
  if (((RootClass_chpl)(coerce_tmp_chpl)) == nil) /* ZLINE: 54 /Users/iainmoncrief/Documents/chapel/modules/internal/LocalesArray.chpl */
#line 54 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocalesArray.chpl"
  {
#line 54 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocalesArray.chpl"
    ret_chpl = ((RootLocale_chpl)(nil));
#line 54 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocalesArray.chpl"
    goto _end__COLON__chpl;
#line 54 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocalesArray.chpl"
  }
#line 54 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocalesArray.chpl"
  chpl_macro_tmp_62 = ((RootClass_chpl)(coerce_tmp_chpl))->chpl__cid;
#line 54 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocalesArray.chpl"
  ret_chpl = ( (((chpl__cid_RootLocale_chpl <= chpl_macro_tmp_62) && (chpl_macro_tmp_62 <= chpl_subclass_max_id[chpl__cid_RootLocale_chpl])))?(((RootLocale_chpl)(coerce_tmp_chpl))):(((RootLocale_chpl)(NULL))) );
#line 54 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocalesArray.chpl"
  _end__COLON__chpl:;
#line 54 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocalesArray.chpl"
  call_tmp_chpl2 = ret_chpl;
#line 54 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocalesArray.chpl"
  if (((RootClass_chpl)(call_tmp_chpl2)) == nil) /* ZLINE: 54 /Users/iainmoncrief/Documents/chapel/modules/internal/LocalesArray.chpl */
#line 54 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocalesArray.chpl"
  {
#line 54 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocalesArray.chpl"
    nilCheckHalt_chpl(&_str_literal_52_chpl, _ln_chpl, _fn_chpl);
#line 54 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocalesArray.chpl"
  }
#line 54 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocalesArray.chpl"
  call_tmp_chpl3 = call_tmp_chpl2;
#line 54 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocalesArray.chpl"
  chpl_check_nil(call_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 54 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocalesArray.chpl"
  call_tmp_chpl5 = getDefaultLocaleArray_chpl(call_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 54 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocalesArray.chpl"
  Locales_chpl = call_tmp_chpl5;
#line 58 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocalesArray.chpl"
  _dom_chpl10(Locales_chpl, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 58 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocalesArray.chpl"
  call_tmp_chpl6 = ret_tmp_chpl;
#line 58 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocalesArray.chpl"
  LocaleSpace_chpl = chpl__initCopy3(&call_tmp_chpl6, UINT8(true), _ln_chpl, _fn_chpl);
#line 41 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocalesArray.chpl"
  *(refIndentLevel_chpl) -= INT64(1);
#line 41 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocalesArray.chpl"
  _exit_chpl__init_LocalesArray_chpl:;
#line 41 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocalesArray.chpl"
  return;
#line 41 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocalesArray.chpl"
}

#line 41 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocalesArray.chpl"
/* LocalesArray.chpl:41 */
#line 41 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocalesArray.chpl"
static void chpl__deinit_LocalesArray(int64_t _ln_chpl,
#line 41 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocalesArray.chpl"
                                      int32_t _fn_chpl) {
#line 58 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocalesArray.chpl"
  _domain_DefaultRectangularDom_1_int64_t_one_chpl local_LocaleSpace_chpl;
#line 58 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocalesArray.chpl"
  local_LocaleSpace_chpl = LocaleSpace_chpl;
#line 58 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocalesArray.chpl"
  chpl__autoDestroy11(&local_LocaleSpace_chpl, _ln_chpl, _fn_chpl);
#line 41 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocalesArray.chpl"
  return;
#line 41 "/Users/iainmoncrief/Documents/chapel/modules/internal/LocalesArray.chpl"
}

