//
// This future illustrates a problem that occurs with procedure pointers and
// insert line numbers. Make sure you are running without CHPL_DEVELOPER set.
//

// Should hold.
compilerAssert(useProcedurePointers);

var once = false;

proc constructWideProcPointerNeedingLineNos() {
  use ChapelDynamicLoading;

  if !once {
    chpl_procToTestInsertLineNumbers();
    once = true;
  }

  const ret = chpl_procToTestInsertLineNumbers;
  return ret;
}

proc test0() {
  const p = constructWideProcPointerNeedingLineNos();
  // Indirect call with the procedure pointer.
  p();
}

proc main() {
  test0();
}
