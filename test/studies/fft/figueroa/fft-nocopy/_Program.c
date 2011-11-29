/* <internal>:-1 */
object _construct_object(object meme, int32_t _ln, _string _fn) {
  object T2 = NULL;
  chpl_bool T1;
  T1 = (nil==meme);
  if (T1) {
    T2 = (object)chpl_alloc(sizeof(_object), "instance of class object", _ln, _fn);
    ((object)T2)->_cid = _e_object;
  } else {
    T2 = meme;
  }
  return T2;
}

void _initModuleGuards(void) {
  __run_ChapelBase_firsttime0 = true;
  __run_ChapelStandard_firsttime1 = true;
  __run_ChapelLocale_firsttime2 = true;
  __run_ChapelIO_firsttime3 = true;
  __run_ChapelTuple_firsttime4 = true;
  __run_ChapelReduce_firsttime5 = true;
  __run_ChapelRange_firsttime6 = true;
  __run_ChapelArray_firsttime7 = true;
  __run_ChapelUtil_firsttime8 = true;
  __run_SingleLocaleArithmetic_firsttime9 = true;
  __run_SingleLocaleAssociative_firsttime10 = true;
  __run_SingleLocaleSparse_firsttime11 = true;
  __run_SingleLocaleEnum_firsttime12 = true;
  __run_SingleLocaleOpaque_firsttime13 = true;
  __run_Types_firsttime14 = true;
  __run_Math_firsttime15 = true;
  __run_List_firsttime16 = true;
  __run_Time_firsttime17 = true;
  __run_Random_firsttime18 = true;
  __run_BitOps_firsttime19 = true;
  __run_Memory_firsttime20 = true;
  __run_Schedules_firsttime21 = true;
  __run_Sort_firsttime22 = true;
  __run_Search_firsttime23 = true;
  __run_DistCSR_firsttime24 = true;
  __run_HPCCProblemSize_firsttime25 = true;
  __run_fft_HYPHEN_nocopy_firsttime26 = true;
  return;
}

void __init__Program(int32_t _ln, _string _fn) {
  __run_fft_HYPHEN_nocopy_firsttime26 = true;
  __run_HPCCProblemSize_firsttime25 = true;
  __run_DistCSR_firsttime24 = true;
  __run_Search_firsttime23 = true;
  __run_Sort_firsttime22 = true;
  __run_Schedules_firsttime21 = true;
  __run_Memory_firsttime20 = true;
  __run_BitOps_firsttime19 = true;
  __run_Random_firsttime18 = true;
  __run_Time_firsttime17 = true;
  __run_List_firsttime16 = true;
  __run_Math_firsttime15 = true;
  __run_Types_firsttime14 = true;
  __run_SingleLocaleOpaque_firsttime13 = true;
  __run_SingleLocaleEnum_firsttime12 = true;
  __run_SingleLocaleSparse_firsttime11 = true;
  __run_SingleLocaleAssociative_firsttime10 = true;
  __run_SingleLocaleArithmetic_firsttime9 = true;
  __run_ChapelUtil_firsttime8 = true;
  __run_ChapelArray_firsttime7 = true;
  __run_ChapelRange_firsttime6 = true;
  __run_ChapelReduce_firsttime5 = true;
  __run_ChapelTuple_firsttime4 = true;
  __run_ChapelIO_firsttime3 = true;
  __run_ChapelLocale_firsttime2 = true;
  __run_ChapelStandard_firsttime1 = true;
  __run_ChapelBase_firsttime0 = true;
  if (__run_ChapelStandard_firsttime1) {
    __init_ChapelStandard(_ln, _fn);
  }
  __run_ChapelStandard_firsttime1 = false;
  _initModuleGuards();
  return;
}

/* fft-nocopy.chpl:102 */
void _heapAllocateGlobals(void) {
  broadcastingGlobalsStarted = 1;
  _chpl_comm_alloc_registry(0);
  return;
}

