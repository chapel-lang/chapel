//
// The purpose of this test is to measure COMM counts when calling dynamic
// loading routines. This way we can know the performance of certain
// actions like e.g., loading a symbol from a binary.
//
use DynamicLibraryWrapper;
use ChapelDynamicLoading;
use CommCounter;
use CTypes;
use Reflection;

require "TestTypeOnlyHeader.h";

config const hi = 2**16;

var cc = new commCounter();

proc test0() {
  writeln(getRoutineName());

  manage cc {
    var bin = new dynamicLibrary("./TestCLibraryToLoad.so");
  }

  cc.printStats();
}

proc test1() {
  writeln(getRoutineName());

  var bin = new dynamicLibrary('./TestCLibraryToLoad.so');

  manage cc {
    type P1 = proc(_: int, _: int): int;
    const addTwoReturn = try! bin.load('addTwoReturn', P1);
    assert(addTwoReturn != nil);
  }

  cc.printStats();
}

proc test2() {
  writeln(getRoutineName());

  var bin = new dynamicLibrary('./TestCLibraryToLoad.so');
  type P1 = proc(_: int, _: int): int;
  const addTwoReturn = try! bin.load('addTwoReturn', P1);
  assert(addTwoReturn != nil);

  manage cc {
    for loc in Locales do on loc {
      const n = addTwoReturn(42, here.id : int);
      writeln(n);
    }
  }

  cc.printStats();
}

proc main() {
  test0();
  test1();
  test2();
}
