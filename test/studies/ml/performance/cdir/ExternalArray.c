#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExternalArray.chpl"
/* ExternalArray.chpl:26 */
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExternalArray.chpl"
static void chpl__init_ExternalArray(int64_t _ln_chpl,
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExternalArray.chpl"
                                     int32_t _fn_chpl) {
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExternalArray.chpl"
  _ref_int32_t_chpl refIndentLevel_chpl = NULL;
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExternalArray.chpl"
  if (chpl__init_ExternalArray_p) /* ZLINE: 26 /Users/iainmoncrief/Documents/chapel/modules/internal/ExternalArray.chpl */
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExternalArray.chpl"
  {
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExternalArray.chpl"
    goto _exit_chpl__init_ExternalArray_chpl;
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExternalArray.chpl"
  }
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExternalArray.chpl"
  printModuleInit_chpl("%*s\n", "ExternalArray", INT64(13), _ln_chpl, _fn_chpl);
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExternalArray.chpl"
  refIndentLevel_chpl = &moduleInitLevel_chpl;
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExternalArray.chpl"
  *(refIndentLevel_chpl) += INT64(1);
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExternalArray.chpl"
  chpl__init_ExternalArray_p = UINT8(true);
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExternalArray.chpl"
  {
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExternalArray.chpl"
    chpl__init_CTypes(_ln_chpl, _fn_chpl);
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExternalArray.chpl"
    chpl__init_HaltWrappers(_ln_chpl, _fn_chpl);
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExternalArray.chpl"
  }
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExternalArray.chpl"
  *(refIndentLevel_chpl) -= INT64(1);
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExternalArray.chpl"
  _exit_chpl__init_ExternalArray_chpl:;
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExternalArray.chpl"
  return;
#line 26 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExternalArray.chpl"
}

#line 247 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExternalArray.chpl"
/* ExternalArray.chpl:247 */
#line 247 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExternalArray.chpl"
void cleanupOpaqueArray(chpl_opaque_array * arr) {
#line 248 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExternalArray.chpl"
  BaseArr_chpl call_tmp_chpl6 = NULL;
#line 250 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExternalArray.chpl"
  _tuple_2_BaseArr_BaseDom_chpl call_tmp_chpl7;
#line 250 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExternalArray.chpl"
  _tuple_2_BaseArr_BaseDom_chpl ret_tmp_chpl;
#line 250 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExternalArray.chpl"
  BaseArr_chpl tmp_x0_chpl = NULL;
#line 250 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExternalArray.chpl"
  BaseDom_chpl tmp_x1_chpl = NULL;
#line 250 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExternalArray.chpl"
  BaseDom_chpl domToFree_chpl = NULL;
#line 250 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExternalArray.chpl"
  BaseDom_chpl init_coerce_tmp_chpl = NULL;
#line 250 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExternalArray.chpl"
  BaseDist_chpl distToRemove_chpl = NULL;
#line 250 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExternalArray.chpl"
  BaseDist_chpl init_coerce_tmp_chpl2 = NULL;
#line 250 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExternalArray.chpl"
  BaseDist_chpl distToFree_chpl = NULL;
#line 250 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExternalArray.chpl"
  BaseDist_chpl init_coerce_tmp_chpl3 = NULL;
#line 250 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExternalArray.chpl"
  chpl_bool domIsPrivatized_chpl;
#line 250 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExternalArray.chpl"
  BaseDist_chpl tmp_x1_chpl2 = NULL;
#line 250 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExternalArray.chpl"
  _tuple_2_BaseDom_BaseDist_chpl ret_tmp_chpl2;
#line 250 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExternalArray.chpl"
  chpl_bool distIsPrivatized_chpl;
#line 250 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExternalArray.chpl"
  BaseDist_chpl call_tmp_chpl8 = NULL;
#line 248 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExternalArray.chpl"
  call_tmp_chpl6 = ((BaseArr_chpl)((arr)->_instance));
#line 249 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExternalArray.chpl"
  if (((RootClass_chpl)(call_tmp_chpl6)) != nil) /* ZLINE: 249 /Users/iainmoncrief/Documents/chapel/modules/internal/ExternalArray.chpl */
#line 249 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExternalArray.chpl"
  {
#line 250 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExternalArray.chpl"
    if (! (arr)->_unowned) /* ZLINE: 250 /Users/iainmoncrief/Documents/chapel/modules/internal/ExternalArray.chpl */
#line 250 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExternalArray.chpl"
    {
#line 250 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExternalArray.chpl"
      chpl_check_nil(call_tmp_chpl6, INT64(250), INT32(54));
#line 250 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExternalArray.chpl"
      remove_chpl4(call_tmp_chpl6, &ret_tmp_chpl, INT64(250), INT32(54));
#line 250 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExternalArray.chpl"
      call_tmp_chpl7 = ret_tmp_chpl;
#line 250 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExternalArray.chpl"
      tmp_x0_chpl = (&call_tmp_chpl7)->x0;
#line 250 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExternalArray.chpl"
      tmp_x1_chpl = (&call_tmp_chpl7)->x1;
#line 250 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExternalArray.chpl"
      init_coerce_tmp_chpl = ((BaseDom_chpl)(nil));
#line 250 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExternalArray.chpl"
      domToFree_chpl = init_coerce_tmp_chpl;
#line 250 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExternalArray.chpl"
      init_coerce_tmp_chpl2 = ((BaseDist_chpl)(nil));
#line 250 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExternalArray.chpl"
      distToRemove_chpl = init_coerce_tmp_chpl2;
#line 250 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExternalArray.chpl"
      init_coerce_tmp_chpl3 = ((BaseDist_chpl)(nil));
#line 250 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExternalArray.chpl"
      distToFree_chpl = init_coerce_tmp_chpl3;
#line 250 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExternalArray.chpl"
      domIsPrivatized_chpl = UINT8(false);
#line 250 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExternalArray.chpl"
      if (((RootClass_chpl)(tmp_x1_chpl)) != nil) /* ZLINE: 250 /Users/iainmoncrief/Documents/chapel/modules/internal/ExternalArray.chpl */
#line 250 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExternalArray.chpl"
      {
#line 250 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExternalArray.chpl"
        chpl_check_nil(tmp_x1_chpl, INT64(250), INT32(54));
#line 250 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExternalArray.chpl"
        remove_chpl2(tmp_x1_chpl, &ret_tmp_chpl2, INT64(250), INT32(54));
#line 250 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExternalArray.chpl"
        tmp_x1_chpl2 = (&ret_tmp_chpl2)->x1;
#line 250 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExternalArray.chpl"
        domToFree_chpl = (&ret_tmp_chpl2)->x0;
#line 250 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExternalArray.chpl"
        distToRemove_chpl = tmp_x1_chpl2;
#line 250 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExternalArray.chpl"
        domIsPrivatized_chpl = UINT8(false);
#line 250 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExternalArray.chpl"
      }
#line 250 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExternalArray.chpl"
      distIsPrivatized_chpl = UINT8(false);
#line 250 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExternalArray.chpl"
      if (((RootClass_chpl)(distToRemove_chpl)) != nil) /* ZLINE: 250 /Users/iainmoncrief/Documents/chapel/modules/internal/ExternalArray.chpl */
#line 250 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExternalArray.chpl"
      {
#line 250 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExternalArray.chpl"
        chpl_check_nil(distToRemove_chpl, INT64(250), INT32(54));
#line 250 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExternalArray.chpl"
        call_tmp_chpl8 = remove_chpl(distToRemove_chpl, INT64(250), INT32(54));
#line 250 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExternalArray.chpl"
        distToFree_chpl = call_tmp_chpl8;
#line 250 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExternalArray.chpl"
        distIsPrivatized_chpl = UINT8(false);
#line 250 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExternalArray.chpl"
      }
#line 250 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExternalArray.chpl"
      if (((RootClass_chpl)(tmp_x0_chpl)) != nil) /* ZLINE: 250 /Users/iainmoncrief/Documents/chapel/modules/internal/ExternalArray.chpl */
#line 250 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExternalArray.chpl"
      {
#line 250 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExternalArray.chpl"
        _delete_arr_chpl(call_tmp_chpl6, UINT8(true), INT64(250), INT32(54));
#line 250 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExternalArray.chpl"
      }
#line 250 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExternalArray.chpl"
      if (((RootClass_chpl)(domToFree_chpl)) != nil) /* ZLINE: 250 /Users/iainmoncrief/Documents/chapel/modules/internal/ExternalArray.chpl */
#line 250 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExternalArray.chpl"
      {
#line 250 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExternalArray.chpl"
        if (((RootClass_chpl)(tmp_x1_chpl)) == nil) /* ZLINE: 250 /Users/iainmoncrief/Documents/chapel/modules/internal/ExternalArray.chpl */
#line 250 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExternalArray.chpl"
        {
#line 250 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExternalArray.chpl"
          nilCheckHalt_chpl(&_str_literal_52_chpl, INT64(250), INT32(54));
#line 250 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExternalArray.chpl"
        }
#line 250 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExternalArray.chpl"
        _delete_dom_chpl7(tmp_x1_chpl, domIsPrivatized_chpl, INT64(250), INT32(54));
#line 250 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExternalArray.chpl"
      }
#line 250 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExternalArray.chpl"
      if (((RootClass_chpl)(distToFree_chpl)) != nil) /* ZLINE: 250 /Users/iainmoncrief/Documents/chapel/modules/internal/ExternalArray.chpl */
#line 250 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExternalArray.chpl"
      {
#line 250 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExternalArray.chpl"
        _delete_dist_chpl(distToFree_chpl, distIsPrivatized_chpl, INT64(250), INT32(54));
#line 250 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExternalArray.chpl"
      }
#line 250 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExternalArray.chpl"
    } else /* ZLINE: 250 /Users/iainmoncrief/Documents/chapel/modules/internal/ExternalArray.chpl */
#line 250 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExternalArray.chpl"
    {
#line 250 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExternalArray.chpl"
    }
#line 250 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExternalArray.chpl"
  }
#line 247 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExternalArray.chpl"
  return;
#line 247 "/Users/iainmoncrief/Documents/chapel/modules/internal/ExternalArray.chpl"
}

