//
// This future illustrates a problem that occurs with procedure pointers and
// insert line numbers. Make sure you are running without CHPL_DEVELOPER set.
//

// Should hold.
compilerAssert(useProcedurePointers);

var once = false;

// NOTE: Pragma is a lie, but we need 'compiler/passes/insertLineNumbers.cpp'
// to thread line numbers through this as though it is a procedure defined
// in a standard or internal module.
//
pragma "compiler generated"
proc procToTestInsertLineNumbers() {
  // This is a standard module call that requires line numbers.
  warning('INSERT LINE NUMBERS TEST!');
}

proc constructWideProcPointerNeedingLineNos() {
  use ChapelDynamicLoading;

  if !once {
    procToTestInsertLineNumbers();
    once = true;
  }

  const ret = procToTestInsertLineNumbers;
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
