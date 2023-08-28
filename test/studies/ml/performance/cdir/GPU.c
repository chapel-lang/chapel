#line 35 "/Users/iainmoncrief/Documents/chapel/modules/standard/GPU.chpl"
/* GPU.chpl:35 */
#line 35 "/Users/iainmoncrief/Documents/chapel/modules/standard/GPU.chpl"
static void chpl__init_GPU(int64_t _ln_chpl,
#line 35 "/Users/iainmoncrief/Documents/chapel/modules/standard/GPU.chpl"
                           int32_t _fn_chpl) {
#line 35 "/Users/iainmoncrief/Documents/chapel/modules/standard/GPU.chpl"
  c_string_rehook modFormatStr_chpl;
#line 35 "/Users/iainmoncrief/Documents/chapel/modules/standard/GPU.chpl"
  c_string_rehook modStr_chpl;
#line 35 "/Users/iainmoncrief/Documents/chapel/modules/standard/GPU.chpl"
  _ref_int32_t_chpl refIndentLevel_chpl = NULL;
#line 35 "/Users/iainmoncrief/Documents/chapel/modules/standard/GPU.chpl"
  if (chpl__init_GPU_p) /* ZLINE: 35 /Users/iainmoncrief/Documents/chapel/modules/standard/GPU.chpl */
#line 35 "/Users/iainmoncrief/Documents/chapel/modules/standard/GPU.chpl"
  {
#line 35 "/Users/iainmoncrief/Documents/chapel/modules/standard/GPU.chpl"
    goto _exit_chpl__init_GPU_chpl;
#line 35 "/Users/iainmoncrief/Documents/chapel/modules/standard/GPU.chpl"
  }
#line 35 "/Users/iainmoncrief/Documents/chapel/modules/standard/GPU.chpl"
  modFormatStr_chpl = "%*s\n";
#line 35 "/Users/iainmoncrief/Documents/chapel/modules/standard/GPU.chpl"
  modStr_chpl = "GPU";
#line 35 "/Users/iainmoncrief/Documents/chapel/modules/standard/GPU.chpl"
  printModuleInit_chpl(modFormatStr_chpl, modStr_chpl, INT64(3), _ln_chpl, _fn_chpl);
#line 35 "/Users/iainmoncrief/Documents/chapel/modules/standard/GPU.chpl"
  refIndentLevel_chpl = &moduleInitLevel_chpl;
#line 35 "/Users/iainmoncrief/Documents/chapel/modules/standard/GPU.chpl"
  *(refIndentLevel_chpl) += INT64(1);
#line 35 "/Users/iainmoncrief/Documents/chapel/modules/standard/GPU.chpl"
  chpl__init_GPU_p = UINT8(true);
#line 35 "/Users/iainmoncrief/Documents/chapel/modules/standard/GPU.chpl"
  {
#line 35 "/Users/iainmoncrief/Documents/chapel/modules/standard/GPU.chpl"
    chpl__init_CTypes(_ln_chpl, _fn_chpl);
#line 35 "/Users/iainmoncrief/Documents/chapel/modules/standard/GPU.chpl"
    chpl__init_ChplConfig(_ln_chpl, _fn_chpl);
#line 35 "/Users/iainmoncrief/Documents/chapel/modules/standard/GPU.chpl"
  }
#line 35 "/Users/iainmoncrief/Documents/chapel/modules/standard/GPU.chpl"
  *(refIndentLevel_chpl) -= INT64(1);
#line 35 "/Users/iainmoncrief/Documents/chapel/modules/standard/GPU.chpl"
  _endchpl__init_GPU_chpl:;
#line 35 "/Users/iainmoncrief/Documents/chapel/modules/standard/GPU.chpl"
  _exit_chpl__init_GPU_chpl:;
#line 35 "/Users/iainmoncrief/Documents/chapel/modules/standard/GPU.chpl"
  return;
#line 35 "/Users/iainmoncrief/Documents/chapel/modules/standard/GPU.chpl"
}

