#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelStandard.chpl"
/* ChapelStandard.chpl:24 */
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelStandard.chpl"
void chpl__init_ChapelStandard(int64_t _ln,
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelStandard.chpl"
                               int32_t _fn) {
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelStandard.chpl"
  _ref_int32_t_chpl refIndentLevel_chpl = NULL;
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelStandard.chpl"
  if (chpl__init_ChapelStandard_p) /* ZLINE: 24 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelStandard.chpl */
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelStandard.chpl"
  {
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelStandard.chpl"
    goto _exit_chpl__init_ChapelStandard_chpl;
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelStandard.chpl"
  }
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelStandard.chpl"
  printModuleInit_chpl("%*s\n", "ChapelStandard", INT64(14), _ln, _fn);
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelStandard.chpl"
  refIndentLevel_chpl = &moduleInitLevel_chpl;
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelStandard.chpl"
  *(refIndentLevel_chpl) += INT64(1);
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelStandard.chpl"
  chpl__init_ChapelStandard_p = UINT8(true);
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelStandard.chpl"
  {
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelStandard.chpl"
    chpl__init_Bytes(_ln, _fn);
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelStandard.chpl"
    chpl__init_String(_ln, _fn);
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelStandard.chpl"
    chpl__init_OwnedObject(_ln, _fn);
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelStandard.chpl"
    chpl__init_SharedObject(_ln, _fn);
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelStandard.chpl"
    chpl__init_ChapelBase(_ln, _fn);
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelStandard.chpl"
    chpl__init_Atomics(_ln, _fn);
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelStandard.chpl"
    chpl__init_ChapelThreads(_ln, _fn);
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelStandard.chpl"
    chpl__init_ChapelTuple(_ln, _fn);
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelStandard.chpl"
    chpl__init_ChapelRange(_ln, _fn);
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelStandard.chpl"
    chpl__init_ChapelReduce(_ln, _fn);
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelStandard.chpl"
    chpl__init_ChapelTaskDataHelp(_ln, _fn);
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelStandard.chpl"
    chpl__init_LocaleModel(_ln, _fn);
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelStandard.chpl"
    chpl__init_ChapelLocale(_ln, _fn);
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelStandard.chpl"
    chpl__init_ChapelPrivatization(_ln, _fn);
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelStandard.chpl"
    chpl__init_DefaultRectangular(_ln, _fn);
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelStandard.chpl"
    chpl__init_LocalesArray(_ln, _fn);
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelStandard.chpl"
    chpl__init_ChapelArray(_ln, _fn);
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelStandard.chpl"
    chpl__init_ChapelDistribution(_ln, _fn);
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelStandard.chpl"
    chpl__init_ChapelIO(_ln, _fn);
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelStandard.chpl"
    chpl__init_ChapelTaskTable(_ln, _fn);
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelStandard.chpl"
    chpl__init_MemTracking(_ln, _fn);
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelStandard.chpl"
    chpl__init_ChapelUtil(_ln, _fn);
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelStandard.chpl"
    chpl__init_Errors(_ln, _fn);
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelStandard.chpl"
    chpl__init_ChapelTaskData(_ln, _fn);
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelStandard.chpl"
    chpl__init_ExportWrappers(_ln, _fn);
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelStandard.chpl"
    chpl__init_ChapelGpuSupport(_ln, _fn);
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelStandard.chpl"
    chpl__init_Types(_ln, _fn);
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelStandard.chpl"
    chpl__init_AutoMath(_ln, _fn);
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelStandard.chpl"
    chpl__init_CTypes(_ln, _fn);
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelStandard.chpl"
    chpl__init_MemConsistency(_ln, _fn);
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelStandard.chpl"
    chpl__init_currentTask(_ln, _fn);
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelStandard.chpl"
    chpl__init_Reflection(_ln, _fn);
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelStandard.chpl"
    chpl__init_AlignedTSupport(_ln, _fn);
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelStandard.chpl"
    chpl__init_POSIX(_ln, _fn);
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelStandard.chpl"
    chpl__init_DSIUtil(_ln, _fn);
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelStandard.chpl"
    chpl__init_ChapelLocks(_ln, _fn);
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelStandard.chpl"
    chpl__init_IO(_ln, _fn);
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelStandard.chpl"
    chpl__init_ChapelHashtable(_ln, _fn);
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelStandard.chpl"
    chpl__init_OS(_ln, _fn);
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelStandard.chpl"
    chpl__init_Sort(_ln, _fn);
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelStandard.chpl"
    chpl__init_RangeChunk(_ln, _fn);
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelStandard.chpl"
    chpl__init_CopyAggregation(_ln, _fn);
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelStandard.chpl"
  }
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelStandard.chpl"
  *(refIndentLevel_chpl) -= INT64(1);
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelStandard.chpl"
  _exit_chpl__init_ChapelStandard_chpl:;
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelStandard.chpl"
  return;
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelStandard.chpl"
}

