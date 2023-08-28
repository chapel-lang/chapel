#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
/* OwnedObject.chpl:24 */
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
static void chpl__init_OwnedObject(int64_t _ln_chpl,
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                                   int32_t _fn_chpl) {
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  _ref_int32_t_chpl refIndentLevel_chpl = NULL;
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  if (chpl__init_OwnedObject_p) /* ZLINE: 24 /Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl */
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  {
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
    goto _exit_chpl__init_OwnedObject_chpl;
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  }
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  printModuleInit_chpl("%*s\n", "OwnedObject", INT64(11), _ln_chpl, _fn_chpl);
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  refIndentLevel_chpl = &moduleInitLevel_chpl;
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  *(refIndentLevel_chpl) += INT64(1);
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  chpl__init_OwnedObject_p = UINT8(true);
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  {
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
    chpl__init_HaltWrappers(_ln_chpl, _fn_chpl);
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  }
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  *(refIndentLevel_chpl) -= INT64(1);
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  _exit_chpl__init_OwnedObject_chpl:;
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  return;
#line 24 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
}

#line 50 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
/* OwnedObject.chpl:50 */
#line 50 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
static void init_chpl162(_owned_IllegalArgumentError_chpl2 * this_chpl31,
#line 50 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                         int64_t _ln_chpl,
#line 50 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                         int32_t _fn_chpl) {
#line 55 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  IllegalArgumentError_chpl init_coerce_tmp_chpl = NULL;
#line 55 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  init_coerce_tmp_chpl = ((IllegalArgumentError_chpl)(nil));
#line 55 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  (this_chpl31)->chpl_p = init_coerce_tmp_chpl;
#line 50 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  return;
#line 50 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
}

#line 50 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
/* OwnedObject.chpl:50 */
#line 50 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
static void init_chpl163(_owned_Error_chpl2 * this_chpl31,
#line 50 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                         int64_t _ln_chpl,
#line 50 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                         int32_t _fn_chpl) {
#line 55 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  Error_chpl init_coerce_tmp_chpl = NULL;
#line 55 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  init_coerce_tmp_chpl = ((Error_chpl)(nil));
#line 55 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  (this_chpl31)->chpl_p = init_coerce_tmp_chpl;
#line 50 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  return;
#line 50 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
}

#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
/* OwnedObject.chpl:65 */
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
static void init_chpl179(_owned_IllegalArgumentError_chpl * this_chpl31,
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                         IllegalArgumentError_chpl p_chpl) {
#line 67 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  (this_chpl31)->chpl_p = p_chpl;
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  return;
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
}

#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
/* OwnedObject.chpl:65 */
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
static void init_chpl188(_owned_Error_chpl * this_chpl31,
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                         Error_chpl p_chpl) {
#line 67 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  (this_chpl31)->chpl_p = p_chpl;
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  return;
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
}

#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
/* OwnedObject.chpl:65 */
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
static void init_chpl183(_owned_DecodeError_chpl * this_chpl31,
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                         DecodeError_chpl p_chpl) {
#line 67 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  (this_chpl31)->chpl_p = p_chpl;
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  return;
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
}

#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
/* OwnedObject.chpl:65 */
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
static void init_chpl174(_owned_EofError_chpl * this_chpl31,
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                         EofError_chpl p_chpl) {
#line 67 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  (this_chpl31)->chpl_p = p_chpl;
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  return;
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
}

#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
/* OwnedObject.chpl:65 */
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
static void init_chpl173(_owned_UnexpectedEofError_chpl * this_chpl31,
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                         UnexpectedEofError_chpl p_chpl) {
#line 67 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  (this_chpl31)->chpl_p = p_chpl;
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  return;
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
}

#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
/* OwnedObject.chpl:65 */
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
static void init_chpl178(_owned_BadFormatError_chpl * this_chpl31,
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                         BadFormatError_chpl p_chpl) {
#line 67 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  (this_chpl31)->chpl_p = p_chpl;
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  return;
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
}

#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
/* OwnedObject.chpl:65 */
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
static void init_chpl180(_owned_BlockingIoError_chpl * this_chpl31,
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                         BlockingIoError_chpl p_chpl) {
#line 67 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  (this_chpl31)->chpl_p = p_chpl;
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  return;
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
}

#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
/* OwnedObject.chpl:65 */
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
static void init_chpl184(_owned_ChildProcessError_chpl * this_chpl31,
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                         ChildProcessError_chpl p_chpl) {
#line 67 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  (this_chpl31)->chpl_p = p_chpl;
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  return;
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
}

#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
/* OwnedObject.chpl:65 */
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
static void init_chpl185(_owned_BrokenPipeError_chpl * this_chpl31,
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                         BrokenPipeError_chpl p_chpl) {
#line 67 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  (this_chpl31)->chpl_p = p_chpl;
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  return;
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
}

#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
/* OwnedObject.chpl:65 */
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
static void init_chpl165(_owned_ConnectionAbortedError_chpl * this_chpl31,
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                         ConnectionAbortedError_chpl p_chpl) {
#line 67 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  (this_chpl31)->chpl_p = p_chpl;
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  return;
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
}

#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
/* OwnedObject.chpl:65 */
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
static void init_chpl175(_owned_ConnectionRefusedError_chpl * this_chpl31,
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                         ConnectionRefusedError_chpl p_chpl) {
#line 67 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  (this_chpl31)->chpl_p = p_chpl;
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  return;
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
}

#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
/* OwnedObject.chpl:65 */
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
static void init_chpl172(_owned_ConnectionResetError_chpl * this_chpl31,
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                         ConnectionResetError_chpl p_chpl) {
#line 67 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  (this_chpl31)->chpl_p = p_chpl;
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  return;
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
}

#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
/* OwnedObject.chpl:65 */
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
static void init_chpl170(_owned_FileExistsError_chpl * this_chpl31,
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                         FileExistsError_chpl p_chpl) {
#line 67 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  (this_chpl31)->chpl_p = p_chpl;
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  return;
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
}

#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
/* OwnedObject.chpl:65 */
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
static void init_chpl169(_owned_FileNotFoundError_chpl * this_chpl31,
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                         FileNotFoundError_chpl p_chpl) {
#line 67 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  (this_chpl31)->chpl_p = p_chpl;
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  return;
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
}

#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
/* OwnedObject.chpl:65 */
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
static void init_chpl168(_owned_InterruptedError_chpl * this_chpl31,
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                         InterruptedError_chpl p_chpl) {
#line 67 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  (this_chpl31)->chpl_p = p_chpl;
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  return;
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
}

#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
/* OwnedObject.chpl:65 */
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
static void init_chpl167(_owned_IsADirectoryError_chpl * this_chpl31,
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                         IsADirectoryError_chpl p_chpl) {
#line 67 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  (this_chpl31)->chpl_p = p_chpl;
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  return;
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
}

#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
/* OwnedObject.chpl:65 */
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
static void init_chpl164(_owned_NotADirectoryError_chpl * this_chpl31,
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                         NotADirectoryError_chpl p_chpl) {
#line 67 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  (this_chpl31)->chpl_p = p_chpl;
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  return;
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
}

#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
/* OwnedObject.chpl:65 */
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
static void init_chpl189(_owned_PermissionError_chpl * this_chpl31,
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                         PermissionError_chpl p_chpl) {
#line 67 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  (this_chpl31)->chpl_p = p_chpl;
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  return;
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
}

#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
/* OwnedObject.chpl:65 */
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
static void init_chpl187(_owned_ProcessLookupError_chpl * this_chpl31,
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                         ProcessLookupError_chpl p_chpl) {
#line 67 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  (this_chpl31)->chpl_p = p_chpl;
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  return;
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
}

#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
/* OwnedObject.chpl:65 */
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
static void init_chpl181(_owned_TimeoutError_chpl * this_chpl31,
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                         TimeoutError_chpl p_chpl) {
#line 67 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  (this_chpl31)->chpl_p = p_chpl;
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  return;
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
}

#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
/* OwnedObject.chpl:65 */
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
static void init_chpl176(_owned_IoError_chpl * this_chpl31,
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                         IoError_chpl p_chpl) {
#line 67 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  (this_chpl31)->chpl_p = p_chpl;
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  return;
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
}

#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
/* OwnedObject.chpl:65 */
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
static void init_chpl186(_owned_SystemError_chpl * this_chpl31,
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                         SystemError_chpl p_chpl) {
#line 67 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  (this_chpl31)->chpl_p = p_chpl;
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  return;
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
}

#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
/* OwnedObject.chpl:65 */
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
static void init_chpl171(_owned_CodepointSplitError_chpl * this_chpl31,
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                         CodepointSplitError_chpl p_chpl) {
#line 67 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  (this_chpl31)->chpl_p = p_chpl;
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  return;
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
}

#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
/* OwnedObject.chpl:65 */
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
static void init_chpl182(_owned_Error_chpl2 * this_chpl31,
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                         Error_chpl p_chpl) {
#line 67 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  (this_chpl31)->chpl_p = p_chpl;
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  return;
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
}

#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
/* OwnedObject.chpl:65 */
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
static void init_chpl190(_owned_TaskErrors_chpl * this_chpl31,
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                         TaskErrors_chpl p_chpl) {
#line 67 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  (this_chpl31)->chpl_p = p_chpl;
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  return;
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
}

#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
/* OwnedObject.chpl:65 */
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
static void init_chpl166(_owned_NPBRandomStream__real64_T_chpl * this_chpl31,
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                         NPBRandomStream__real64_T_chpl p_chpl) {
#line 67 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  (this_chpl31)->chpl_p = p_chpl;
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  return;
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
}

#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
/* OwnedObject.chpl:65 */
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
static void init_chpl177(_owned_PCGRandomStream_int64_t_F_chpl * this_chpl31,
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                         PCGRandomStream_int64_t_F_chpl p_chpl) {
#line 67 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  (this_chpl31)->chpl_p = p_chpl;
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  return;
#line 65 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
}

#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
/* OwnedObject.chpl:84 */
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
static void init_ASSIGN__chpl17(_owned_IllegalArgumentError_chpl2 * this_chpl31,
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                                _owned_IllegalArgumentError_chpl * src_chpl,
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                                int64_t _ln_chpl,
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                                int32_t _fn_chpl) {
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  IllegalArgumentError_chpl coerce_tmp_chpl3 = NULL;
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  coerce_tmp_chpl3 = (src_chpl)->chpl_p;
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  (src_chpl)->chpl_p = ((IllegalArgumentError_chpl)(nil));
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  if (((RootClass_chpl)(coerce_tmp_chpl3)) == nil) /* ZLINE: 93 /Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl */
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  {
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
    nilCheckHalt_chpl(&_str_literal_52_chpl, _ln_chpl, _fn_chpl);
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  }
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  (this_chpl31)->chpl_p = coerce_tmp_chpl3;
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  return;
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
}

#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
/* OwnedObject.chpl:84 */
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
static void init_ASSIGN__chpl10(_owned_SystemError_chpl * this_chpl31,
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                                _owned_BlockingIoError_chpl * src_chpl,
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                                int64_t _ln_chpl,
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                                int32_t _fn_chpl) {
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  BlockingIoError_chpl coerce_tmp_chpl3 = NULL;
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  SystemError_chpl init_coerce_tmp_chpl = NULL;
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  coerce_tmp_chpl3 = (src_chpl)->chpl_p;
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  (src_chpl)->chpl_p = ((BlockingIoError_chpl)(nil));
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  if (((RootClass_chpl)(coerce_tmp_chpl3)) == nil) /* ZLINE: 93 /Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl */
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  {
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
    nilCheckHalt_chpl(&_str_literal_52_chpl, _ln_chpl, _fn_chpl);
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  }
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  init_coerce_tmp_chpl = ((SystemError_chpl)(coerce_tmp_chpl3));
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  (this_chpl31)->chpl_p = init_coerce_tmp_chpl;
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  return;
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
}

#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
/* OwnedObject.chpl:84 */
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
static void init_ASSIGN__chpl9(_owned_SystemError_chpl * this_chpl31,
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                               _owned_ChildProcessError_chpl * src_chpl,
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                               int64_t _ln_chpl,
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                               int32_t _fn_chpl) {
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  ChildProcessError_chpl coerce_tmp_chpl3 = NULL;
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  SystemError_chpl init_coerce_tmp_chpl = NULL;
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  coerce_tmp_chpl3 = (src_chpl)->chpl_p;
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  (src_chpl)->chpl_p = ((ChildProcessError_chpl)(nil));
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  if (((RootClass_chpl)(coerce_tmp_chpl3)) == nil) /* ZLINE: 93 /Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl */
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  {
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
    nilCheckHalt_chpl(&_str_literal_52_chpl, _ln_chpl, _fn_chpl);
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  }
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  init_coerce_tmp_chpl = ((SystemError_chpl)(coerce_tmp_chpl3));
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  (this_chpl31)->chpl_p = init_coerce_tmp_chpl;
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  return;
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
}

#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
/* OwnedObject.chpl:84 */
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
static void init_ASSIGN__chpl6(_owned_SystemError_chpl * this_chpl31,
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                               _owned_BrokenPipeError_chpl * src_chpl,
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                               int64_t _ln_chpl,
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                               int32_t _fn_chpl) {
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  BrokenPipeError_chpl coerce_tmp_chpl3 = NULL;
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  SystemError_chpl init_coerce_tmp_chpl = NULL;
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  coerce_tmp_chpl3 = (src_chpl)->chpl_p;
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  (src_chpl)->chpl_p = ((BrokenPipeError_chpl)(nil));
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  if (((RootClass_chpl)(coerce_tmp_chpl3)) == nil) /* ZLINE: 93 /Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl */
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  {
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
    nilCheckHalt_chpl(&_str_literal_52_chpl, _ln_chpl, _fn_chpl);
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  }
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  init_coerce_tmp_chpl = ((SystemError_chpl)(coerce_tmp_chpl3));
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  (this_chpl31)->chpl_p = init_coerce_tmp_chpl;
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  return;
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
}

#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
/* OwnedObject.chpl:84 */
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
static void init_ASSIGN__chpl4(_owned_SystemError_chpl * this_chpl31,
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                               _owned_ConnectionAbortedError_chpl * src_chpl,
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                               int64_t _ln_chpl,
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                               int32_t _fn_chpl) {
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  ConnectionAbortedError_chpl coerce_tmp_chpl3 = NULL;
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  SystemError_chpl init_coerce_tmp_chpl = NULL;
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  coerce_tmp_chpl3 = (src_chpl)->chpl_p;
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  (src_chpl)->chpl_p = ((ConnectionAbortedError_chpl)(nil));
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  if (((RootClass_chpl)(coerce_tmp_chpl3)) == nil) /* ZLINE: 93 /Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl */
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  {
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
    nilCheckHalt_chpl(&_str_literal_52_chpl, _ln_chpl, _fn_chpl);
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  }
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  init_coerce_tmp_chpl = ((SystemError_chpl)(coerce_tmp_chpl3));
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  (this_chpl31)->chpl_p = init_coerce_tmp_chpl;
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  return;
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
}

#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
/* OwnedObject.chpl:84 */
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
static void init_ASSIGN__chpl3(_owned_SystemError_chpl * this_chpl31,
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                               _owned_ConnectionRefusedError_chpl * src_chpl,
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                               int64_t _ln_chpl,
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                               int32_t _fn_chpl) {
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  ConnectionRefusedError_chpl coerce_tmp_chpl3 = NULL;
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  SystemError_chpl init_coerce_tmp_chpl = NULL;
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  coerce_tmp_chpl3 = (src_chpl)->chpl_p;
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  (src_chpl)->chpl_p = ((ConnectionRefusedError_chpl)(nil));
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  if (((RootClass_chpl)(coerce_tmp_chpl3)) == nil) /* ZLINE: 93 /Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl */
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  {
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
    nilCheckHalt_chpl(&_str_literal_52_chpl, _ln_chpl, _fn_chpl);
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  }
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  init_coerce_tmp_chpl = ((SystemError_chpl)(coerce_tmp_chpl3));
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  (this_chpl31)->chpl_p = init_coerce_tmp_chpl;
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  return;
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
}

#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
/* OwnedObject.chpl:84 */
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
static void init_ASSIGN__chpl2(_owned_SystemError_chpl * this_chpl31,
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                               _owned_ConnectionResetError_chpl * src_chpl,
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                               int64_t _ln_chpl,
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                               int32_t _fn_chpl) {
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  ConnectionResetError_chpl coerce_tmp_chpl3 = NULL;
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  SystemError_chpl init_coerce_tmp_chpl = NULL;
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  coerce_tmp_chpl3 = (src_chpl)->chpl_p;
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  (src_chpl)->chpl_p = ((ConnectionResetError_chpl)(nil));
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  if (((RootClass_chpl)(coerce_tmp_chpl3)) == nil) /* ZLINE: 93 /Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl */
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  {
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
    nilCheckHalt_chpl(&_str_literal_52_chpl, _ln_chpl, _fn_chpl);
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  }
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  init_coerce_tmp_chpl = ((SystemError_chpl)(coerce_tmp_chpl3));
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  (this_chpl31)->chpl_p = init_coerce_tmp_chpl;
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  return;
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
}

#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
/* OwnedObject.chpl:84 */
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
static void init_ASSIGN__chpl5(_owned_SystemError_chpl * this_chpl31,
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                               _owned_FileExistsError_chpl * src_chpl,
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                               int64_t _ln_chpl,
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                               int32_t _fn_chpl) {
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  FileExistsError_chpl coerce_tmp_chpl3 = NULL;
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  SystemError_chpl init_coerce_tmp_chpl = NULL;
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  coerce_tmp_chpl3 = (src_chpl)->chpl_p;
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  (src_chpl)->chpl_p = ((FileExistsError_chpl)(nil));
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  if (((RootClass_chpl)(coerce_tmp_chpl3)) == nil) /* ZLINE: 93 /Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl */
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  {
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
    nilCheckHalt_chpl(&_str_literal_52_chpl, _ln_chpl, _fn_chpl);
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  }
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  init_coerce_tmp_chpl = ((SystemError_chpl)(coerce_tmp_chpl3));
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  (this_chpl31)->chpl_p = init_coerce_tmp_chpl;
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  return;
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
}

#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
/* OwnedObject.chpl:84 */
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
static void init_ASSIGN__chpl7(_owned_SystemError_chpl * this_chpl31,
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                               _owned_FileNotFoundError_chpl * src_chpl,
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                               int64_t _ln_chpl,
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                               int32_t _fn_chpl) {
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  FileNotFoundError_chpl coerce_tmp_chpl3 = NULL;
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  SystemError_chpl init_coerce_tmp_chpl = NULL;
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  coerce_tmp_chpl3 = (src_chpl)->chpl_p;
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  (src_chpl)->chpl_p = ((FileNotFoundError_chpl)(nil));
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  if (((RootClass_chpl)(coerce_tmp_chpl3)) == nil) /* ZLINE: 93 /Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl */
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  {
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
    nilCheckHalt_chpl(&_str_literal_52_chpl, _ln_chpl, _fn_chpl);
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  }
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  init_coerce_tmp_chpl = ((SystemError_chpl)(coerce_tmp_chpl3));
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  (this_chpl31)->chpl_p = init_coerce_tmp_chpl;
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  return;
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
}

#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
/* OwnedObject.chpl:84 */
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
static void init_ASSIGN__chpl8(_owned_SystemError_chpl * this_chpl31,
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                               _owned_InterruptedError_chpl * src_chpl,
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                               int64_t _ln_chpl,
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                               int32_t _fn_chpl) {
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  InterruptedError_chpl coerce_tmp_chpl3 = NULL;
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  SystemError_chpl init_coerce_tmp_chpl = NULL;
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  coerce_tmp_chpl3 = (src_chpl)->chpl_p;
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  (src_chpl)->chpl_p = ((InterruptedError_chpl)(nil));
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  if (((RootClass_chpl)(coerce_tmp_chpl3)) == nil) /* ZLINE: 93 /Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl */
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  {
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
    nilCheckHalt_chpl(&_str_literal_52_chpl, _ln_chpl, _fn_chpl);
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  }
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  init_coerce_tmp_chpl = ((SystemError_chpl)(coerce_tmp_chpl3));
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  (this_chpl31)->chpl_p = init_coerce_tmp_chpl;
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  return;
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
}

#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
/* OwnedObject.chpl:84 */
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
static void init_ASSIGN__chpl11(_owned_SystemError_chpl * this_chpl31,
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                                _owned_IsADirectoryError_chpl * src_chpl,
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                                int64_t _ln_chpl,
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                                int32_t _fn_chpl) {
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  IsADirectoryError_chpl coerce_tmp_chpl3 = NULL;
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  SystemError_chpl init_coerce_tmp_chpl = NULL;
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  coerce_tmp_chpl3 = (src_chpl)->chpl_p;
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  (src_chpl)->chpl_p = ((IsADirectoryError_chpl)(nil));
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  if (((RootClass_chpl)(coerce_tmp_chpl3)) == nil) /* ZLINE: 93 /Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl */
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  {
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
    nilCheckHalt_chpl(&_str_literal_52_chpl, _ln_chpl, _fn_chpl);
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  }
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  init_coerce_tmp_chpl = ((SystemError_chpl)(coerce_tmp_chpl3));
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  (this_chpl31)->chpl_p = init_coerce_tmp_chpl;
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  return;
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
}

#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
/* OwnedObject.chpl:84 */
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
static void init_ASSIGN__chpl12(_owned_SystemError_chpl * this_chpl31,
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                                _owned_NotADirectoryError_chpl * src_chpl,
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                                int64_t _ln_chpl,
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                                int32_t _fn_chpl) {
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  NotADirectoryError_chpl coerce_tmp_chpl3 = NULL;
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  SystemError_chpl init_coerce_tmp_chpl = NULL;
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  coerce_tmp_chpl3 = (src_chpl)->chpl_p;
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  (src_chpl)->chpl_p = ((NotADirectoryError_chpl)(nil));
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  if (((RootClass_chpl)(coerce_tmp_chpl3)) == nil) /* ZLINE: 93 /Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl */
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  {
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
    nilCheckHalt_chpl(&_str_literal_52_chpl, _ln_chpl, _fn_chpl);
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  }
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  init_coerce_tmp_chpl = ((SystemError_chpl)(coerce_tmp_chpl3));
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  (this_chpl31)->chpl_p = init_coerce_tmp_chpl;
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  return;
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
}

#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
/* OwnedObject.chpl:84 */
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
static void init_ASSIGN__chpl13(_owned_SystemError_chpl * this_chpl31,
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                                _owned_PermissionError_chpl * src_chpl,
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                                int64_t _ln_chpl,
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                                int32_t _fn_chpl) {
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  PermissionError_chpl coerce_tmp_chpl3 = NULL;
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  SystemError_chpl init_coerce_tmp_chpl = NULL;
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  coerce_tmp_chpl3 = (src_chpl)->chpl_p;
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  (src_chpl)->chpl_p = ((PermissionError_chpl)(nil));
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  if (((RootClass_chpl)(coerce_tmp_chpl3)) == nil) /* ZLINE: 93 /Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl */
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  {
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
    nilCheckHalt_chpl(&_str_literal_52_chpl, _ln_chpl, _fn_chpl);
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  }
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  init_coerce_tmp_chpl = ((SystemError_chpl)(coerce_tmp_chpl3));
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  (this_chpl31)->chpl_p = init_coerce_tmp_chpl;
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  return;
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
}

#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
/* OwnedObject.chpl:84 */
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
static void init_ASSIGN__chpl14(_owned_SystemError_chpl * this_chpl31,
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                                _owned_ProcessLookupError_chpl * src_chpl,
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                                int64_t _ln_chpl,
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                                int32_t _fn_chpl) {
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  ProcessLookupError_chpl coerce_tmp_chpl3 = NULL;
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  SystemError_chpl init_coerce_tmp_chpl = NULL;
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  coerce_tmp_chpl3 = (src_chpl)->chpl_p;
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  (src_chpl)->chpl_p = ((ProcessLookupError_chpl)(nil));
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  if (((RootClass_chpl)(coerce_tmp_chpl3)) == nil) /* ZLINE: 93 /Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl */
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  {
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
    nilCheckHalt_chpl(&_str_literal_52_chpl, _ln_chpl, _fn_chpl);
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  }
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  init_coerce_tmp_chpl = ((SystemError_chpl)(coerce_tmp_chpl3));
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  (this_chpl31)->chpl_p = init_coerce_tmp_chpl;
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  return;
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
}

#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
/* OwnedObject.chpl:84 */
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
static void init_ASSIGN__chpl15(_owned_SystemError_chpl * this_chpl31,
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                                _owned_TimeoutError_chpl * src_chpl,
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                                int64_t _ln_chpl,
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                                int32_t _fn_chpl) {
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  TimeoutError_chpl coerce_tmp_chpl3 = NULL;
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  SystemError_chpl init_coerce_tmp_chpl = NULL;
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  coerce_tmp_chpl3 = (src_chpl)->chpl_p;
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  (src_chpl)->chpl_p = ((TimeoutError_chpl)(nil));
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  if (((RootClass_chpl)(coerce_tmp_chpl3)) == nil) /* ZLINE: 93 /Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl */
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  {
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
    nilCheckHalt_chpl(&_str_literal_52_chpl, _ln_chpl, _fn_chpl);
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  }
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  init_coerce_tmp_chpl = ((SystemError_chpl)(coerce_tmp_chpl3));
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  (this_chpl31)->chpl_p = init_coerce_tmp_chpl;
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  return;
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
}

#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
/* OwnedObject.chpl:84 */
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
static void init_ASSIGN__chpl16(_owned_SystemError_chpl * this_chpl31,
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                                _owned_IoError_chpl * src_chpl,
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                                int64_t _ln_chpl,
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                                int32_t _fn_chpl) {
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  IoError_chpl coerce_tmp_chpl3 = NULL;
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  SystemError_chpl init_coerce_tmp_chpl = NULL;
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  coerce_tmp_chpl3 = (src_chpl)->chpl_p;
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  (src_chpl)->chpl_p = ((IoError_chpl)(nil));
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  if (((RootClass_chpl)(coerce_tmp_chpl3)) == nil) /* ZLINE: 93 /Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl */
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  {
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
    nilCheckHalt_chpl(&_str_literal_52_chpl, _ln_chpl, _fn_chpl);
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  }
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  init_coerce_tmp_chpl = ((SystemError_chpl)(coerce_tmp_chpl3));
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  (this_chpl31)->chpl_p = init_coerce_tmp_chpl;
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  return;
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
}

#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
/* OwnedObject.chpl:84 */
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
static void init_ASSIGN__chpl18(_owned_Error_chpl * this_chpl31,
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                                _owned_EofError_chpl * src_chpl,
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                                int64_t _ln_chpl,
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                                int32_t _fn_chpl) {
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  EofError_chpl coerce_tmp_chpl3 = NULL;
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  Error_chpl init_coerce_tmp_chpl = NULL;
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  coerce_tmp_chpl3 = (src_chpl)->chpl_p;
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  (src_chpl)->chpl_p = ((EofError_chpl)(nil));
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  if (((RootClass_chpl)(coerce_tmp_chpl3)) == nil) /* ZLINE: 93 /Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl */
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  {
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
    nilCheckHalt_chpl(&_str_literal_52_chpl, _ln_chpl, _fn_chpl);
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  }
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  init_coerce_tmp_chpl = ((Error_chpl)(coerce_tmp_chpl3));
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  (this_chpl31)->chpl_p = init_coerce_tmp_chpl;
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  return;
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
}

#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
/* OwnedObject.chpl:84 */
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
static void init_ASSIGN__chpl19(_owned_Error_chpl * this_chpl31,
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                                _owned_UnexpectedEofError_chpl * src_chpl,
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                                int64_t _ln_chpl,
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                                int32_t _fn_chpl) {
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  UnexpectedEofError_chpl coerce_tmp_chpl3 = NULL;
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  Error_chpl init_coerce_tmp_chpl = NULL;
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  coerce_tmp_chpl3 = (src_chpl)->chpl_p;
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  (src_chpl)->chpl_p = ((UnexpectedEofError_chpl)(nil));
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  if (((RootClass_chpl)(coerce_tmp_chpl3)) == nil) /* ZLINE: 93 /Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl */
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  {
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
    nilCheckHalt_chpl(&_str_literal_52_chpl, _ln_chpl, _fn_chpl);
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  }
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  init_coerce_tmp_chpl = ((Error_chpl)(coerce_tmp_chpl3));
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  (this_chpl31)->chpl_p = init_coerce_tmp_chpl;
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  return;
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
}

#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
/* OwnedObject.chpl:84 */
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
static void init_ASSIGN__chpl20(_owned_Error_chpl * this_chpl31,
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                                _owned_BadFormatError_chpl * src_chpl,
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                                int64_t _ln_chpl,
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                                int32_t _fn_chpl) {
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  BadFormatError_chpl coerce_tmp_chpl3 = NULL;
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  Error_chpl init_coerce_tmp_chpl = NULL;
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  coerce_tmp_chpl3 = (src_chpl)->chpl_p;
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  (src_chpl)->chpl_p = ((BadFormatError_chpl)(nil));
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  if (((RootClass_chpl)(coerce_tmp_chpl3)) == nil) /* ZLINE: 93 /Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl */
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  {
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
    nilCheckHalt_chpl(&_str_literal_52_chpl, _ln_chpl, _fn_chpl);
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  }
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  init_coerce_tmp_chpl = ((Error_chpl)(coerce_tmp_chpl3));
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  (this_chpl31)->chpl_p = init_coerce_tmp_chpl;
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  return;
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
}

#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
/* OwnedObject.chpl:84 */
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
static void init_ASSIGN__chpl21(_owned_Error_chpl * this_chpl31,
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                                _owned_SystemError_chpl * src_chpl,
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                                int64_t _ln_chpl,
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                                int32_t _fn_chpl) {
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  SystemError_chpl coerce_tmp_chpl3 = NULL;
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  Error_chpl init_coerce_tmp_chpl = NULL;
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  coerce_tmp_chpl3 = (src_chpl)->chpl_p;
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  (src_chpl)->chpl_p = ((SystemError_chpl)(nil));
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  if (((RootClass_chpl)(coerce_tmp_chpl3)) == nil) /* ZLINE: 93 /Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl */
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  {
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
    nilCheckHalt_chpl(&_str_literal_52_chpl, _ln_chpl, _fn_chpl);
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  }
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  init_coerce_tmp_chpl = ((Error_chpl)(coerce_tmp_chpl3));
#line 93 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  (this_chpl31)->chpl_p = init_coerce_tmp_chpl;
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  return;
#line 84 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
}

#line 126 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
/* OwnedObject.chpl:126 */
#line 126 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
static void init_ASSIGN__chpl22(_owned_IllegalArgumentError_chpl2 * this_chpl31,
#line 126 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                                _nilType src_chpl,
#line 126 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                                int64_t _ln_chpl,
#line 126 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                                int32_t _fn_chpl) {
#line 130 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  init_chpl162(this_chpl31, _ln_chpl, _fn_chpl);
#line 126 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  return;
#line 126 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
}

#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
/* OwnedObject.chpl:221 */
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
static void deinit_chpl76(_owned_IllegalArgumentError_chpl2 * this_chpl31,
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                          int64_t _ln_chpl,
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                          int32_t _fn_chpl) {
#line 223 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  if (((RootClass_chpl)((this_chpl31)->chpl_p)) != nil) /* ZLINE: 223 /Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl */
#line 223 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  {
#line 224 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
    chpl__delete55((this_chpl31)->chpl_p, _ln_chpl, _fn_chpl);
#line 224 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  }
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  return;
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
}

#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
/* OwnedObject.chpl:221 */
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
static void deinit_chpl77(_owned_IllegalArgumentError_chpl * this_chpl31,
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                          int64_t _ln_chpl,
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                          int32_t _fn_chpl) {
#line 223 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  if (((RootClass_chpl)((this_chpl31)->chpl_p)) != nil) /* ZLINE: 223 /Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl */
#line 223 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  {
#line 224 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
    chpl__delete55((this_chpl31)->chpl_p, _ln_chpl, _fn_chpl);
#line 224 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  }
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  return;
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
}

#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
/* OwnedObject.chpl:221 */
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
static void deinit_chpl78(_owned_Error_chpl2 * this_chpl31,
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                          int64_t _ln_chpl,
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                          int32_t _fn_chpl) {
#line 223 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  if (((RootClass_chpl)((this_chpl31)->chpl_p)) != nil) /* ZLINE: 223 /Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl */
#line 223 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  {
#line 224 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
    chpl__delete74((this_chpl31)->chpl_p, _ln_chpl, _fn_chpl);
#line 224 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  }
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  return;
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
}

#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
/* OwnedObject.chpl:221 */
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
static void deinit_chpl80(_owned_Error_chpl * this_chpl31,
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                          int64_t _ln_chpl,
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                          int32_t _fn_chpl) {
#line 223 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  if (((RootClass_chpl)((this_chpl31)->chpl_p)) != nil) /* ZLINE: 223 /Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl */
#line 223 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  {
#line 224 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
    chpl__delete74((this_chpl31)->chpl_p, _ln_chpl, _fn_chpl);
#line 224 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  }
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  return;
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
}

#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
/* OwnedObject.chpl:221 */
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
static void deinit_chpl81(_owned_DecodeError_chpl * this_chpl31,
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                          int64_t _ln_chpl,
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                          int32_t _fn_chpl) {
#line 223 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  if (((RootClass_chpl)((this_chpl31)->chpl_p)) != nil) /* ZLINE: 223 /Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl */
#line 223 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  {
#line 224 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
    chpl__delete54((this_chpl31)->chpl_p, _ln_chpl, _fn_chpl);
#line 224 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  }
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  return;
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
}

#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
/* OwnedObject.chpl:221 */
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
static void deinit_chpl82(_owned_EofError_chpl * this_chpl31,
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                          int64_t _ln_chpl,
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                          int32_t _fn_chpl) {
#line 223 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  if (((RootClass_chpl)((this_chpl31)->chpl_p)) != nil) /* ZLINE: 223 /Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl */
#line 223 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  {
#line 224 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
    chpl__delete50((this_chpl31)->chpl_p, _ln_chpl, _fn_chpl);
#line 224 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  }
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  return;
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
}

#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
/* OwnedObject.chpl:221 */
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
static void deinit_chpl83(_owned_UnexpectedEofError_chpl * this_chpl31,
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                          int64_t _ln_chpl,
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                          int32_t _fn_chpl) {
#line 223 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  if (((RootClass_chpl)((this_chpl31)->chpl_p)) != nil) /* ZLINE: 223 /Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl */
#line 223 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  {
#line 224 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
    chpl__delete48((this_chpl31)->chpl_p, _ln_chpl, _fn_chpl);
#line 224 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  }
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  return;
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
}

#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
/* OwnedObject.chpl:221 */
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
static void deinit_chpl67(_owned_BadFormatError_chpl * this_chpl31,
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                          int64_t _ln_chpl,
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                          int32_t _fn_chpl) {
#line 223 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  if (((RootClass_chpl)((this_chpl31)->chpl_p)) != nil) /* ZLINE: 223 /Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl */
#line 223 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  {
#line 224 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
    chpl__delete46((this_chpl31)->chpl_p, _ln_chpl, _fn_chpl);
#line 224 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  }
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  return;
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
}

#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
/* OwnedObject.chpl:221 */
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
static void deinit_chpl85(_owned_BlockingIoError_chpl * this_chpl31,
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                          int64_t _ln_chpl,
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                          int32_t _fn_chpl) {
#line 223 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  if (((RootClass_chpl)((this_chpl31)->chpl_p)) != nil) /* ZLINE: 223 /Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl */
#line 223 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  {
#line 224 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
    chpl__delete43((this_chpl31)->chpl_p, _ln_chpl, _fn_chpl);
#line 224 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  }
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  return;
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
}

#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
/* OwnedObject.chpl:221 */
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
static void deinit_chpl86(_owned_ChildProcessError_chpl * this_chpl31,
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                          int64_t _ln_chpl,
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                          int32_t _fn_chpl) {
#line 223 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  if (((RootClass_chpl)((this_chpl31)->chpl_p)) != nil) /* ZLINE: 223 /Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl */
#line 223 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  {
#line 224 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
    chpl__delete42((this_chpl31)->chpl_p, _ln_chpl, _fn_chpl);
#line 224 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  }
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  return;
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
}

#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
/* OwnedObject.chpl:221 */
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
static void deinit_chpl87(_owned_BrokenPipeError_chpl * this_chpl31,
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                          int64_t _ln_chpl,
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                          int32_t _fn_chpl) {
#line 223 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  if (((RootClass_chpl)((this_chpl31)->chpl_p)) != nil) /* ZLINE: 223 /Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl */
#line 223 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  {
#line 224 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
    chpl__delete41((this_chpl31)->chpl_p, _ln_chpl, _fn_chpl);
#line 224 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  }
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  return;
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
}

#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
/* OwnedObject.chpl:221 */
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
static void deinit_chpl89(_owned_ConnectionAbortedError_chpl * this_chpl31,
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                          int64_t _ln_chpl,
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                          int32_t _fn_chpl) {
#line 223 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  if (((RootClass_chpl)((this_chpl31)->chpl_p)) != nil) /* ZLINE: 223 /Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl */
#line 223 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  {
#line 224 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
    chpl__delete36((this_chpl31)->chpl_p, _ln_chpl, _fn_chpl);
#line 224 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  }
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  return;
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
}

#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
/* OwnedObject.chpl:221 */
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
static void deinit_chpl90(_owned_ConnectionRefusedError_chpl * this_chpl31,
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                          int64_t _ln_chpl,
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                          int32_t _fn_chpl) {
#line 223 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  if (((RootClass_chpl)((this_chpl31)->chpl_p)) != nil) /* ZLINE: 223 /Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl */
#line 223 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  {
#line 224 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
    chpl__delete30((this_chpl31)->chpl_p, _ln_chpl, _fn_chpl);
#line 224 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  }
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  return;
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
}

#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
/* OwnedObject.chpl:221 */
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
static void deinit_chpl91(_owned_ConnectionResetError_chpl * this_chpl31,
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                          int64_t _ln_chpl,
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                          int32_t _fn_chpl) {
#line 223 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  if (((RootClass_chpl)((this_chpl31)->chpl_p)) != nil) /* ZLINE: 223 /Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl */
#line 223 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  {
#line 224 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
    chpl__delete17((this_chpl31)->chpl_p, _ln_chpl, _fn_chpl);
#line 224 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  }
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  return;
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
}

#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
/* OwnedObject.chpl:221 */
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
static void deinit_chpl92(_owned_FileExistsError_chpl * this_chpl31,
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                          int64_t _ln_chpl,
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                          int32_t _fn_chpl) {
#line 223 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  if (((RootClass_chpl)((this_chpl31)->chpl_p)) != nil) /* ZLINE: 223 /Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl */
#line 223 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  {
#line 224 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
    chpl__delete15((this_chpl31)->chpl_p, _ln_chpl, _fn_chpl);
#line 224 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  }
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  return;
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
}

#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
/* OwnedObject.chpl:221 */
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
static void deinit_chpl88(_owned_FileNotFoundError_chpl * this_chpl31,
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                          int64_t _ln_chpl,
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                          int32_t _fn_chpl) {
#line 223 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  if (((RootClass_chpl)((this_chpl31)->chpl_p)) != nil) /* ZLINE: 223 /Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl */
#line 223 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  {
#line 224 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
    chpl__delete19((this_chpl31)->chpl_p, _ln_chpl, _fn_chpl);
#line 224 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  }
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  return;
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
}

#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
/* OwnedObject.chpl:221 */
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
static void deinit_chpl84(_owned_InterruptedError_chpl * this_chpl31,
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                          int64_t _ln_chpl,
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                          int32_t _fn_chpl) {
#line 223 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  if (((RootClass_chpl)((this_chpl31)->chpl_p)) != nil) /* ZLINE: 223 /Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl */
#line 223 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  {
#line 224 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
    chpl__delete66((this_chpl31)->chpl_p, _ln_chpl, _fn_chpl);
#line 224 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  }
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  return;
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
}

#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
/* OwnedObject.chpl:221 */
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
static void deinit_chpl73(_owned_IsADirectoryError_chpl * this_chpl31,
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                          int64_t _ln_chpl,
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                          int32_t _fn_chpl) {
#line 223 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  if (((RootClass_chpl)((this_chpl31)->chpl_p)) != nil) /* ZLINE: 223 /Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl */
#line 223 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  {
#line 224 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
    chpl__delete45((this_chpl31)->chpl_p, _ln_chpl, _fn_chpl);
#line 224 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  }
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  return;
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
}

#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
/* OwnedObject.chpl:221 */
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
static void deinit_chpl72(_owned_NotADirectoryError_chpl * this_chpl31,
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                          int64_t _ln_chpl,
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                          int32_t _fn_chpl) {
#line 223 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  if (((RootClass_chpl)((this_chpl31)->chpl_p)) != nil) /* ZLINE: 223 /Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl */
#line 223 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  {
#line 224 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
    chpl__delete2((this_chpl31)->chpl_p, _ln_chpl, _fn_chpl);
#line 224 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  }
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  return;
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
}

#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
/* OwnedObject.chpl:221 */
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
static void deinit_chpl70(_owned_PermissionError_chpl * this_chpl31,
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                          int64_t _ln_chpl,
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                          int32_t _fn_chpl) {
#line 223 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  if (((RootClass_chpl)((this_chpl31)->chpl_p)) != nil) /* ZLINE: 223 /Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl */
#line 223 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  {
#line 224 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
    chpl__delete4((this_chpl31)->chpl_p, _ln_chpl, _fn_chpl);
#line 224 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  }
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  return;
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
}

#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
/* OwnedObject.chpl:221 */
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
static void deinit_chpl69(_owned_ProcessLookupError_chpl * this_chpl31,
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                          int64_t _ln_chpl,
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                          int32_t _fn_chpl) {
#line 223 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  if (((RootClass_chpl)((this_chpl31)->chpl_p)) != nil) /* ZLINE: 223 /Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl */
#line 223 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  {
#line 224 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
    chpl__delete5((this_chpl31)->chpl_p, _ln_chpl, _fn_chpl);
#line 224 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  }
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  return;
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
}

#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
/* OwnedObject.chpl:221 */
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
static void deinit_chpl68(_owned_TimeoutError_chpl * this_chpl31,
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                          int64_t _ln_chpl,
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                          int32_t _fn_chpl) {
#line 223 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  if (((RootClass_chpl)((this_chpl31)->chpl_p)) != nil) /* ZLINE: 223 /Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl */
#line 223 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  {
#line 224 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
    chpl__delete7((this_chpl31)->chpl_p, _ln_chpl, _fn_chpl);
#line 224 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  }
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  return;
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
}

#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
/* OwnedObject.chpl:221 */
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
static void deinit_chpl75(_owned_IoError_chpl * this_chpl31,
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                          int64_t _ln_chpl,
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                          int32_t _fn_chpl) {
#line 223 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  if (((RootClass_chpl)((this_chpl31)->chpl_p)) != nil) /* ZLINE: 223 /Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl */
#line 223 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  {
#line 224 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
    chpl__delete8((this_chpl31)->chpl_p, _ln_chpl, _fn_chpl);
#line 224 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  }
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  return;
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
}

#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
/* OwnedObject.chpl:221 */
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
static void deinit_chpl71(_owned_SystemError_chpl * this_chpl31,
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                          int64_t _ln_chpl,
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                          int32_t _fn_chpl) {
#line 223 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  if (((RootClass_chpl)((this_chpl31)->chpl_p)) != nil) /* ZLINE: 223 /Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl */
#line 223 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  {
#line 224 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
    chpl__delete9((this_chpl31)->chpl_p, _ln_chpl, _fn_chpl);
#line 224 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  }
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  return;
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
}

#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
/* OwnedObject.chpl:221 */
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
static void deinit_chpl66(_owned_CodepointSplitError_chpl * this_chpl31,
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                          int64_t _ln_chpl,
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                          int32_t _fn_chpl) {
#line 223 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  if (((RootClass_chpl)((this_chpl31)->chpl_p)) != nil) /* ZLINE: 223 /Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl */
#line 223 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  {
#line 224 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
    chpl__delete11((this_chpl31)->chpl_p, _ln_chpl, _fn_chpl);
#line 224 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  }
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  return;
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
}

#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
/* OwnedObject.chpl:221 */
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
static void deinit_chpl79(_owned_TaskErrors_chpl * this_chpl31,
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                          int64_t _ln_chpl,
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                          int32_t _fn_chpl) {
#line 223 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  if (((RootClass_chpl)((this_chpl31)->chpl_p)) != nil) /* ZLINE: 223 /Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl */
#line 223 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  {
#line 224 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
    chpl__delete40((this_chpl31)->chpl_p, _ln_chpl, _fn_chpl);
#line 224 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  }
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  return;
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
}

#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
/* OwnedObject.chpl:221 */
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
static void deinit_chpl93(_owned_NPBRandomStream__real64_T_chpl * this_chpl31,
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                          int64_t _ln_chpl,
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                          int32_t _fn_chpl) {
#line 223 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  if (((RootClass_chpl)((this_chpl31)->chpl_p)) != nil) /* ZLINE: 223 /Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl */
#line 223 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  {
#line 224 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
    chpl__delete62((this_chpl31)->chpl_p, _ln_chpl, _fn_chpl);
#line 224 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  }
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  return;
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
}

#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
/* OwnedObject.chpl:221 */
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
static void deinit_chpl74(_owned_PCGRandomStream_int64_t_F_chpl * this_chpl31,
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                          int64_t _ln_chpl,
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                          int32_t _fn_chpl) {
#line 223 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  if (((RootClass_chpl)((this_chpl31)->chpl_p)) != nil) /* ZLINE: 223 /Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl */
#line 223 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  {
#line 224 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
    chpl__delete20((this_chpl31)->chpl_p, _ln_chpl, _fn_chpl);
#line 224 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  }
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  return;
#line 221 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
}

#line 286 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
/* OwnedObject.chpl:286 */
#line 286 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
static IllegalArgumentError_chpl borrow_chpl2(_owned_IllegalArgumentError_chpl2 * this_chpl31) {
#line 286 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  IllegalArgumentError_chpl ret_chpl = NULL;
#line 288 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  ret_chpl = (this_chpl31)->chpl_p;
#line 286 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  return ret_chpl;
#line 286 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
}

#line 286 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
/* OwnedObject.chpl:286 */
#line 286 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
static Error_chpl borrow_chpl3(_owned_Error_chpl2 * this_chpl31) {
#line 286 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  Error_chpl ret_chpl = NULL;
#line 288 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  ret_chpl = (this_chpl31)->chpl_p;
#line 286 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  return ret_chpl;
#line 286 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
}

#line 286 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
/* OwnedObject.chpl:286 */
#line 286 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
static NPBRandomStream__real64_T_chpl borrow_chpl4(_owned_NPBRandomStream__real64_T_chpl * this_chpl31,
#line 286 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                                                   int64_t _ln_chpl,
#line 286 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                                                   int32_t _fn_chpl) {
#line 290 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  NPBRandomStream__real64_T_chpl coerce_tmp_chpl3 = NULL;
#line 290 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  coerce_tmp_chpl3 = (this_chpl31)->chpl_p;
#line 290 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  if (((RootClass_chpl)(coerce_tmp_chpl3)) == nil) /* ZLINE: 290 /Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl */
#line 290 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  {
#line 290 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
    nilCheckHalt_chpl(&_str_literal_52_chpl, _ln_chpl, _fn_chpl);
#line 290 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  }
#line 286 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  return coerce_tmp_chpl3;
#line 286 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
}

#line 286 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
/* OwnedObject.chpl:286 */
#line 286 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
static PCGRandomStream_int64_t_F_chpl borrow_chpl(_owned_PCGRandomStream_int64_t_F_chpl * this_chpl31,
#line 286 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                                                  int64_t _ln_chpl,
#line 286 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                                                  int32_t _fn_chpl) {
#line 290 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  PCGRandomStream_int64_t_F_chpl coerce_tmp_chpl3 = NULL;
#line 290 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  coerce_tmp_chpl3 = (this_chpl31)->chpl_p;
#line 290 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  if (((RootClass_chpl)(coerce_tmp_chpl3)) == nil) /* ZLINE: 290 /Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl */
#line 290 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  {
#line 290 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
    nilCheckHalt_chpl(&_str_literal_52_chpl, _ln_chpl, _fn_chpl);
#line 290 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  }
#line 286 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  return coerce_tmp_chpl3;
#line 286 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
}

#line 311 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
/* OwnedObject.chpl:311 */
#line 311 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
static void chpl___ASSIGN_11(_owned_Error_chpl2 * lhs_chpl,
#line 311 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                             _owned_Error_chpl * rhs_chpl,
#line 311 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                             int64_t _ln_chpl,
#line 311 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                             int32_t _fn_chpl) {
#line 319 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  chpl_bool tmp_chpl;
#line 337 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  Error_chpl coerce_tmp_chpl3 = NULL;
#line 338 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  Error_chpl coerce_tmp_chpl4 = NULL;
#line 319 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  if (((RootClass_chpl)((lhs_chpl)->chpl_p)) == nil) /* ZLINE: 319 /Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl */
#line 319 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  {
#line 319 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
    tmp_chpl = (((RootClass_chpl)((rhs_chpl)->chpl_p)) == nil);
#line 319 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  } else /* ZLINE: 319 /Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl */
#line 319 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  {
#line 319 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
    tmp_chpl = UINT8(false);
#line 319 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  }
#line 319 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  if (tmp_chpl) /* ZLINE: 319 /Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl */
#line 319 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  {
#line 320 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
    goto _end__ASSIGN__chpl;
#line 320 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  }
#line 337 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  coerce_tmp_chpl3 = (lhs_chpl)->chpl_p;
#line 338 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  coerce_tmp_chpl4 = (rhs_chpl)->chpl_p;
#line 338 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  (rhs_chpl)->chpl_p = ((Error_chpl)(nil));
#line 338 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  if (((RootClass_chpl)(coerce_tmp_chpl4)) == nil) /* ZLINE: 338 /Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl */
#line 338 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  {
#line 338 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
    nilCheckHalt_chpl(&_str_literal_52_chpl, _ln_chpl, _fn_chpl);
#line 338 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  }
#line 338 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  (lhs_chpl)->chpl_p = coerce_tmp_chpl4;
#line 339 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  chpl__delete74(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 339 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  _end__ASSIGN__chpl:;
#line 311 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  return;
#line 311 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
}

#line 311 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
/* OwnedObject.chpl:311 */
#line 311 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
static void chpl___ASSIGN_10(_owned_Error_chpl2 * lhs_chpl,
#line 311 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                             _owned_Error_chpl2 * rhs_chpl,
#line 311 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                             int64_t _ln_chpl,
#line 311 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                             int32_t _fn_chpl) {
#line 319 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  chpl_bool tmp_chpl;
#line 337 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  Error_chpl coerce_tmp_chpl3 = NULL;
#line 338 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  Error_chpl coerce_tmp_chpl4 = NULL;
#line 319 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  if (((RootClass_chpl)((lhs_chpl)->chpl_p)) == nil) /* ZLINE: 319 /Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl */
#line 319 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  {
#line 319 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
    tmp_chpl = (((RootClass_chpl)((rhs_chpl)->chpl_p)) == nil);
#line 319 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  } else /* ZLINE: 319 /Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl */
#line 319 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  {
#line 319 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
    tmp_chpl = UINT8(false);
#line 319 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  }
#line 319 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  if (tmp_chpl) /* ZLINE: 319 /Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl */
#line 319 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  {
#line 320 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
    goto _end__ASSIGN__chpl;
#line 320 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  }
#line 337 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  coerce_tmp_chpl3 = (lhs_chpl)->chpl_p;
#line 338 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  coerce_tmp_chpl4 = (rhs_chpl)->chpl_p;
#line 338 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  (rhs_chpl)->chpl_p = ((Error_chpl)(nil));
#line 338 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  (lhs_chpl)->chpl_p = coerce_tmp_chpl4;
#line 339 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  chpl__delete74(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 339 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  _end__ASSIGN__chpl:;
#line 311 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  return;
#line 311 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
}

#line 311 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
/* OwnedObject.chpl:311 */
#line 311 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
static void chpl___ASSIGN_12(_owned_NPBRandomStream__real64_T_chpl * lhs_chpl,
#line 311 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                             _owned_NPBRandomStream__real64_T_chpl * rhs_chpl,
#line 311 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                             int64_t _ln_chpl,
#line 311 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                             int32_t _fn_chpl) {
#line 319 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  chpl_bool tmp_chpl;
#line 337 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  NPBRandomStream__real64_T_chpl coerce_tmp_chpl3 = NULL;
#line 338 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  NPBRandomStream__real64_T_chpl coerce_tmp_chpl4 = NULL;
#line 319 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  if (((RootClass_chpl)((lhs_chpl)->chpl_p)) == nil) /* ZLINE: 319 /Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl */
#line 319 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  {
#line 319 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
    tmp_chpl = (((RootClass_chpl)((rhs_chpl)->chpl_p)) == nil);
#line 319 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  } else /* ZLINE: 319 /Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl */
#line 319 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  {
#line 319 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
    tmp_chpl = UINT8(false);
#line 319 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  }
#line 319 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  if (tmp_chpl) /* ZLINE: 319 /Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl */
#line 319 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  {
#line 320 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
    goto _end__ASSIGN__chpl;
#line 320 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  }
#line 327 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  if (((RootClass_chpl)((rhs_chpl)->chpl_p)) == nil) /* ZLINE: 327 /Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl */
#line 327 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  {
#line 328 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
    nilCheckHalt_chpl(&_str_literal_289_chpl, _ln_chpl, _fn_chpl);
#line 328 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  }
#line 337 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  coerce_tmp_chpl3 = (lhs_chpl)->chpl_p;
#line 338 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  coerce_tmp_chpl4 = (rhs_chpl)->chpl_p;
#line 338 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  (rhs_chpl)->chpl_p = ((NPBRandomStream__real64_T_chpl)(nil));
#line 338 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  if (((RootClass_chpl)(coerce_tmp_chpl4)) == nil) /* ZLINE: 338 /Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl */
#line 338 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  {
#line 338 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
    nilCheckHalt_chpl(&_str_literal_52_chpl, _ln_chpl, _fn_chpl);
#line 338 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  }
#line 338 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  (lhs_chpl)->chpl_p = coerce_tmp_chpl4;
#line 339 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  chpl__delete62(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 339 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  _end__ASSIGN__chpl:;
#line 311 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  return;
#line 311 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
}

#line 343 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
/* OwnedObject.chpl:343 */
#line 343 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
static void chpl___ASSIGN_13(_owned_Error_chpl2 * lhs_chpl,
#line 343 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                             _nilType rhs_chpl,
#line 343 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                             int64_t _ln_chpl,
#line 343 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                             int32_t _fn_chpl) {
#line 344 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  Error_chpl coerce_tmp_chpl3 = NULL;
#line 344 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  coerce_tmp_chpl3 = (lhs_chpl)->chpl_p;
#line 344 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  (lhs_chpl)->chpl_p = ((Error_chpl)(nil));
#line 344 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  chpl__delete74(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 343 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  return;
#line 343 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
}

#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
/* OwnedObject.chpl:376 */
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
static void chpl__autoDestroy62(_owned_IllegalArgumentError_chpl2 * x_chpl,
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                                int64_t _ln_chpl,
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                                int32_t _fn_chpl) {
#line 377 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  _owned_IllegalArgumentError_chpl2 call_tmp_chpl6;
#line 377 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  call_tmp_chpl6 = *(x_chpl);
#line 377 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  deinit_chpl76(&call_tmp_chpl6, _ln_chpl, _fn_chpl);
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  return;
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
}

#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
/* OwnedObject.chpl:376 */
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
static void chpl__autoDestroy59(_owned_IllegalArgumentError_chpl * x_chpl,
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                                int64_t _ln_chpl,
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                                int32_t _fn_chpl) {
#line 377 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  _owned_IllegalArgumentError_chpl call_tmp_chpl6;
#line 377 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  call_tmp_chpl6 = *(x_chpl);
#line 377 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  deinit_chpl77(&call_tmp_chpl6, _ln_chpl, _fn_chpl);
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  return;
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
}

#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
/* OwnedObject.chpl:376 */
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
static void chpl__autoDestroy58(_owned_Error_chpl2 * x_chpl,
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                                int64_t _ln_chpl,
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                                int32_t _fn_chpl) {
#line 377 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  _owned_Error_chpl2 call_tmp_chpl6;
#line 377 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  call_tmp_chpl6 = *(x_chpl);
#line 377 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  deinit_chpl78(&call_tmp_chpl6, _ln_chpl, _fn_chpl);
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  return;
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
}

#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
/* OwnedObject.chpl:376 */
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
static void chpl__autoDestroy57(_owned_Error_chpl * x_chpl,
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                                int64_t _ln_chpl,
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                                int32_t _fn_chpl) {
#line 377 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  _owned_Error_chpl call_tmp_chpl6;
#line 377 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  call_tmp_chpl6 = *(x_chpl);
#line 377 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  deinit_chpl80(&call_tmp_chpl6, _ln_chpl, _fn_chpl);
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  return;
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
}

#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
/* OwnedObject.chpl:376 */
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
static void chpl__autoDestroy55(_owned_DecodeError_chpl * x_chpl,
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                                int64_t _ln_chpl,
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                                int32_t _fn_chpl) {
#line 377 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  _owned_DecodeError_chpl call_tmp_chpl6;
#line 377 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  call_tmp_chpl6 = *(x_chpl);
#line 377 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  deinit_chpl81(&call_tmp_chpl6, _ln_chpl, _fn_chpl);
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  return;
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
}

#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
/* OwnedObject.chpl:376 */
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
static void chpl__autoDestroy53(_owned_EofError_chpl * x_chpl,
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                                int64_t _ln_chpl,
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                                int32_t _fn_chpl) {
#line 377 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  _owned_EofError_chpl call_tmp_chpl6;
#line 377 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  call_tmp_chpl6 = *(x_chpl);
#line 377 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  deinit_chpl82(&call_tmp_chpl6, _ln_chpl, _fn_chpl);
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  return;
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
}

#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
/* OwnedObject.chpl:376 */
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
static void chpl__autoDestroy52(_owned_UnexpectedEofError_chpl * x_chpl,
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                                int64_t _ln_chpl,
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                                int32_t _fn_chpl) {
#line 377 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  _owned_UnexpectedEofError_chpl call_tmp_chpl6;
#line 377 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  call_tmp_chpl6 = *(x_chpl);
#line 377 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  deinit_chpl83(&call_tmp_chpl6, _ln_chpl, _fn_chpl);
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  return;
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
}

#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
/* OwnedObject.chpl:376 */
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
static void chpl__autoDestroy51(_owned_BadFormatError_chpl * x_chpl,
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                                int64_t _ln_chpl,
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                                int32_t _fn_chpl) {
#line 377 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  _owned_BadFormatError_chpl call_tmp_chpl6;
#line 377 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  call_tmp_chpl6 = *(x_chpl);
#line 377 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  deinit_chpl67(&call_tmp_chpl6, _ln_chpl, _fn_chpl);
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  return;
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
}

#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
/* OwnedObject.chpl:376 */
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
static void chpl__autoDestroy50(_owned_BlockingIoError_chpl * x_chpl,
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                                int64_t _ln_chpl,
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                                int32_t _fn_chpl) {
#line 377 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  _owned_BlockingIoError_chpl call_tmp_chpl6;
#line 377 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  call_tmp_chpl6 = *(x_chpl);
#line 377 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  deinit_chpl85(&call_tmp_chpl6, _ln_chpl, _fn_chpl);
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  return;
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
}

#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
/* OwnedObject.chpl:376 */
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
static void chpl__autoDestroy49(_owned_ChildProcessError_chpl * x_chpl,
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                                int64_t _ln_chpl,
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                                int32_t _fn_chpl) {
#line 377 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  _owned_ChildProcessError_chpl call_tmp_chpl6;
#line 377 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  call_tmp_chpl6 = *(x_chpl);
#line 377 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  deinit_chpl86(&call_tmp_chpl6, _ln_chpl, _fn_chpl);
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  return;
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
}

#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
/* OwnedObject.chpl:376 */
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
static void chpl__autoDestroy48(_owned_BrokenPipeError_chpl * x_chpl,
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                                int64_t _ln_chpl,
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                                int32_t _fn_chpl) {
#line 377 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  _owned_BrokenPipeError_chpl call_tmp_chpl6;
#line 377 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  call_tmp_chpl6 = *(x_chpl);
#line 377 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  deinit_chpl87(&call_tmp_chpl6, _ln_chpl, _fn_chpl);
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  return;
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
}

#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
/* OwnedObject.chpl:376 */
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
static void chpl__autoDestroy47(_owned_ConnectionAbortedError_chpl * x_chpl,
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                                int64_t _ln_chpl,
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                                int32_t _fn_chpl) {
#line 377 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  _owned_ConnectionAbortedError_chpl call_tmp_chpl6;
#line 377 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  call_tmp_chpl6 = *(x_chpl);
#line 377 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  deinit_chpl89(&call_tmp_chpl6, _ln_chpl, _fn_chpl);
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  return;
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
}

#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
/* OwnedObject.chpl:376 */
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
static void chpl__autoDestroy46(_owned_ConnectionRefusedError_chpl * x_chpl,
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                                int64_t _ln_chpl,
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                                int32_t _fn_chpl) {
#line 377 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  _owned_ConnectionRefusedError_chpl call_tmp_chpl6;
#line 377 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  call_tmp_chpl6 = *(x_chpl);
#line 377 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  deinit_chpl90(&call_tmp_chpl6, _ln_chpl, _fn_chpl);
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  return;
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
}

#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
/* OwnedObject.chpl:376 */
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
static void chpl__autoDestroy45(_owned_ConnectionResetError_chpl * x_chpl,
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                                int64_t _ln_chpl,
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                                int32_t _fn_chpl) {
#line 377 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  _owned_ConnectionResetError_chpl call_tmp_chpl6;
#line 377 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  call_tmp_chpl6 = *(x_chpl);
#line 377 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  deinit_chpl91(&call_tmp_chpl6, _ln_chpl, _fn_chpl);
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  return;
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
}

#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
/* OwnedObject.chpl:376 */
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
static void chpl__autoDestroy44(_owned_FileExistsError_chpl * x_chpl,
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                                int64_t _ln_chpl,
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                                int32_t _fn_chpl) {
#line 377 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  _owned_FileExistsError_chpl call_tmp_chpl6;
#line 377 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  call_tmp_chpl6 = *(x_chpl);
#line 377 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  deinit_chpl92(&call_tmp_chpl6, _ln_chpl, _fn_chpl);
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  return;
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
}

#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
/* OwnedObject.chpl:376 */
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
static void chpl__autoDestroy43(_owned_FileNotFoundError_chpl * x_chpl,
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                                int64_t _ln_chpl,
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                                int32_t _fn_chpl) {
#line 377 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  _owned_FileNotFoundError_chpl call_tmp_chpl6;
#line 377 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  call_tmp_chpl6 = *(x_chpl);
#line 377 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  deinit_chpl88(&call_tmp_chpl6, _ln_chpl, _fn_chpl);
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  return;
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
}

#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
/* OwnedObject.chpl:376 */
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
static void chpl__autoDestroy42(_owned_InterruptedError_chpl * x_chpl,
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                                int64_t _ln_chpl,
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                                int32_t _fn_chpl) {
#line 377 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  _owned_InterruptedError_chpl call_tmp_chpl6;
#line 377 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  call_tmp_chpl6 = *(x_chpl);
#line 377 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  deinit_chpl84(&call_tmp_chpl6, _ln_chpl, _fn_chpl);
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  return;
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
}

#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
/* OwnedObject.chpl:376 */
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
static void chpl__autoDestroy41(_owned_IsADirectoryError_chpl * x_chpl,
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                                int64_t _ln_chpl,
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                                int32_t _fn_chpl) {
#line 377 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  _owned_IsADirectoryError_chpl call_tmp_chpl6;
#line 377 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  call_tmp_chpl6 = *(x_chpl);
#line 377 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  deinit_chpl73(&call_tmp_chpl6, _ln_chpl, _fn_chpl);
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  return;
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
}

#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
/* OwnedObject.chpl:376 */
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
static void chpl__autoDestroy40(_owned_NotADirectoryError_chpl * x_chpl,
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                                int64_t _ln_chpl,
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                                int32_t _fn_chpl) {
#line 377 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  _owned_NotADirectoryError_chpl call_tmp_chpl6;
#line 377 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  call_tmp_chpl6 = *(x_chpl);
#line 377 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  deinit_chpl72(&call_tmp_chpl6, _ln_chpl, _fn_chpl);
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  return;
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
}

#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
/* OwnedObject.chpl:376 */
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
static void chpl__autoDestroy39(_owned_PermissionError_chpl * x_chpl,
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                                int64_t _ln_chpl,
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                                int32_t _fn_chpl) {
#line 377 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  _owned_PermissionError_chpl call_tmp_chpl6;
#line 377 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  call_tmp_chpl6 = *(x_chpl);
#line 377 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  deinit_chpl70(&call_tmp_chpl6, _ln_chpl, _fn_chpl);
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  return;
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
}

#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
/* OwnedObject.chpl:376 */
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
static void chpl__autoDestroy37(_owned_ProcessLookupError_chpl * x_chpl,
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                                int64_t _ln_chpl,
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                                int32_t _fn_chpl) {
#line 377 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  _owned_ProcessLookupError_chpl call_tmp_chpl6;
#line 377 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  call_tmp_chpl6 = *(x_chpl);
#line 377 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  deinit_chpl69(&call_tmp_chpl6, _ln_chpl, _fn_chpl);
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  return;
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
}

#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
/* OwnedObject.chpl:376 */
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
static void chpl__autoDestroy35(_owned_TimeoutError_chpl * x_chpl,
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                                int64_t _ln_chpl,
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                                int32_t _fn_chpl) {
#line 377 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  _owned_TimeoutError_chpl call_tmp_chpl6;
#line 377 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  call_tmp_chpl6 = *(x_chpl);
#line 377 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  deinit_chpl68(&call_tmp_chpl6, _ln_chpl, _fn_chpl);
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  return;
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
}

#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
/* OwnedObject.chpl:376 */
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
static void chpl__autoDestroy38(_owned_IoError_chpl * x_chpl,
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                                int64_t _ln_chpl,
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                                int32_t _fn_chpl) {
#line 377 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  _owned_IoError_chpl call_tmp_chpl6;
#line 377 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  call_tmp_chpl6 = *(x_chpl);
#line 377 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  deinit_chpl75(&call_tmp_chpl6, _ln_chpl, _fn_chpl);
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  return;
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
}

#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
/* OwnedObject.chpl:376 */
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
static void chpl__autoDestroy54(_owned_SystemError_chpl * x_chpl,
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                                int64_t _ln_chpl,
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                                int32_t _fn_chpl) {
#line 377 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  _owned_SystemError_chpl call_tmp_chpl6;
#line 377 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  call_tmp_chpl6 = *(x_chpl);
#line 377 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  deinit_chpl71(&call_tmp_chpl6, _ln_chpl, _fn_chpl);
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  return;
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
}

#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
/* OwnedObject.chpl:376 */
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
static void chpl__autoDestroy36(_owned_CodepointSplitError_chpl * x_chpl,
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                                int64_t _ln_chpl,
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                                int32_t _fn_chpl) {
#line 377 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  _owned_CodepointSplitError_chpl call_tmp_chpl6;
#line 377 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  call_tmp_chpl6 = *(x_chpl);
#line 377 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  deinit_chpl66(&call_tmp_chpl6, _ln_chpl, _fn_chpl);
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  return;
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
}

#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
/* OwnedObject.chpl:376 */
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
static void chpl__autoDestroy56(_owned_TaskErrors_chpl * x_chpl,
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                                int64_t _ln_chpl,
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                                int32_t _fn_chpl) {
#line 377 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  _owned_TaskErrors_chpl call_tmp_chpl6;
#line 377 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  call_tmp_chpl6 = *(x_chpl);
#line 377 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  deinit_chpl79(&call_tmp_chpl6, _ln_chpl, _fn_chpl);
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  return;
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
}

#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
/* OwnedObject.chpl:376 */
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
static void chpl__autoDestroy60(_owned_NPBRandomStream__real64_T_chpl * x_chpl,
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                                int64_t _ln_chpl,
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                                int32_t _fn_chpl) {
#line 377 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  _owned_NPBRandomStream__real64_T_chpl call_tmp_chpl6;
#line 377 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  call_tmp_chpl6 = *(x_chpl);
#line 377 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  deinit_chpl93(&call_tmp_chpl6, _ln_chpl, _fn_chpl);
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  return;
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
}

#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
/* OwnedObject.chpl:376 */
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
static void chpl__autoDestroy61(_owned_PCGRandomStream_int64_t_F_chpl * x_chpl,
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                                int64_t _ln_chpl,
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
                                int32_t _fn_chpl) {
#line 377 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  _owned_PCGRandomStream_int64_t_F_chpl call_tmp_chpl6;
#line 377 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  call_tmp_chpl6 = *(x_chpl);
#line 377 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  deinit_chpl74(&call_tmp_chpl6, _ln_chpl, _fn_chpl);
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
  return;
#line 376 "/Users/iainmoncrief/Documents/chapel/modules/internal/OwnedObject.chpl"
}

