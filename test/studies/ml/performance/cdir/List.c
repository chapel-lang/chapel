#line 57 "/Users/iainmoncrief/Documents/chapel/modules/standard/List.chpl"
/* List.chpl:57 */
#line 57 "/Users/iainmoncrief/Documents/chapel/modules/standard/List.chpl"
static void chpl__init_List(int64_t _ln_chpl,
#line 57 "/Users/iainmoncrief/Documents/chapel/modules/standard/List.chpl"
                            int32_t _fn_chpl) {
#line 57 "/Users/iainmoncrief/Documents/chapel/modules/standard/List.chpl"
  _ref_int32_t_chpl refIndentLevel_chpl = NULL;
#line 57 "/Users/iainmoncrief/Documents/chapel/modules/standard/List.chpl"
  if (chpl__init_List_p) /* ZLINE: 57 /Users/iainmoncrief/Documents/chapel/modules/standard/List.chpl */
#line 57 "/Users/iainmoncrief/Documents/chapel/modules/standard/List.chpl"
  {
#line 57 "/Users/iainmoncrief/Documents/chapel/modules/standard/List.chpl"
    goto _exit_chpl__init_List_chpl;
#line 57 "/Users/iainmoncrief/Documents/chapel/modules/standard/List.chpl"
  }
#line 57 "/Users/iainmoncrief/Documents/chapel/modules/standard/List.chpl"
  printModuleInit_chpl("%*s\n", "List", INT64(4), _ln_chpl, _fn_chpl);
#line 57 "/Users/iainmoncrief/Documents/chapel/modules/standard/List.chpl"
  refIndentLevel_chpl = &moduleInitLevel_chpl;
#line 57 "/Users/iainmoncrief/Documents/chapel/modules/standard/List.chpl"
  *(refIndentLevel_chpl) += INT64(1);
#line 57 "/Users/iainmoncrief/Documents/chapel/modules/standard/List.chpl"
  chpl__init_List_p = UINT8(true);
#line 57 "/Users/iainmoncrief/Documents/chapel/modules/standard/List.chpl"
  {
#line 57 "/Users/iainmoncrief/Documents/chapel/modules/standard/List.chpl"
    chpl__init_ChapelLocks(_ln_chpl, _fn_chpl);
#line 57 "/Users/iainmoncrief/Documents/chapel/modules/standard/List.chpl"
    chpl__init_HaltWrappers(_ln_chpl, _fn_chpl);
#line 57 "/Users/iainmoncrief/Documents/chapel/modules/standard/List.chpl"
    chpl__init_Sort(_ln_chpl, _fn_chpl);
#line 57 "/Users/iainmoncrief/Documents/chapel/modules/standard/List.chpl"
    chpl__init_IO(_ln_chpl, _fn_chpl);
#line 57 "/Users/iainmoncrief/Documents/chapel/modules/standard/List.chpl"
    chpl__init_Reflection(_ln_chpl, _fn_chpl);
#line 57 "/Users/iainmoncrief/Documents/chapel/modules/standard/List.chpl"
    chpl__init_CTypes(_ln_chpl, _fn_chpl);
#line 57 "/Users/iainmoncrief/Documents/chapel/modules/standard/List.chpl"
    chpl__init_AutoMath(_ln_chpl, _fn_chpl);
#line 57 "/Users/iainmoncrief/Documents/chapel/modules/standard/List.chpl"
  }
#line 64 "/Users/iainmoncrief/Documents/chapel/modules/standard/List.chpl"
  _initialCapacity_chpl = INT64(8);
#line 67 "/Users/iainmoncrief/Documents/chapel/modules/standard/List.chpl"
  _initialArrayCapacity_chpl = INT64(16);
#line 57 "/Users/iainmoncrief/Documents/chapel/modules/standard/List.chpl"
  *(refIndentLevel_chpl) -= INT64(1);
#line 57 "/Users/iainmoncrief/Documents/chapel/modules/standard/List.chpl"
  _exit_chpl__init_List_chpl:;
#line 57 "/Users/iainmoncrief/Documents/chapel/modules/standard/List.chpl"
  return;
#line 57 "/Users/iainmoncrief/Documents/chapel/modules/standard/List.chpl"
}

