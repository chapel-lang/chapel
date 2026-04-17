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

record r {
  var p = constructWideProcPointerNeedingLineNos();
}

proc test1() {
  var r1 = new r();
  r1.p();
}

proc test2() {
  proc call(p) do p();

  const p = constructWideProcPointerNeedingLineNos();
  call(p);
}

proc test3() {
  const p = constructWideProcPointerNeedingLineNos();
  proc nested1() {
    p();
    proc nested2() {
      p();
      proc nested3() {
        p();
      }
      nested3();
    }
    nested2();
  }
  nested1();
}

proc main() {
  test0();
  test1();
  test2();
  test3();
}
