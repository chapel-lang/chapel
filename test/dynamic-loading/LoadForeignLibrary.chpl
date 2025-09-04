//
// This test makes sure that loading and retrieving symbols from a foreign
// shared library works as expected. A retrieved symbol should be callable
// on any locale, and it should use the correct calling convention (the C
// or system calling convention).
//
// Additionally, this will test that the internal loading API behaves as
// expected - that it throws errors in the correct places and does not
// halt unexpectedly.
//

use ChapelDynamicLoading;
use CTypes;
use Reflection;

require "TestTypeOnlyHeader.h";

config const hi = 2**16;

// These types are declared in the header above, but we have to re-declare
// them here because a require doesn't actually bring them into view as an
// extern block would.
//
// TODO: Can we automagically convert the above header in a precomp?
extern record baz {
  var a: c_int;
  var b: c_int;
  var c: c_int;
  var d: c_int;
}

param FOOBAR_PADDING: int = 32;
extern record foobar {
  var a: c_double;
  var b: c_double;
  var c: c_double;
  var d: c_double;
  var e: c_double;
  var f: c_double;
  var g: c_double;
  var h: c_double;
  var x: baz;
  var padding: c_array(c_int, FOOBAR_PADDING);
}

/** Define a "user facing" wrapper around the internal dynamic library. */
record dynamicLibrary {
  var _bin: unmanaged chpl_BinaryInfo?;
  var _err: owned DynLoadError?;

  proc init(path: string) {
    init this;
    this._bin = chpl_BinaryInfo.create(path, this._err);
    if _bin then _bin!.bumpRefCount();
  }

  proc deinit() {
    if _bin then _bin!.dropRefCount();
  }

  proc err() do return _err.borrow();

  inline proc _assertRetrievedProcedureProperties(p1: ?t1) {
    use Types;

    compilerAssert(isProcedureValue(p1));
    compilerAssert(isProcedureType(t1));
    compilerAssert(isProcedure(p1));
    compilerAssert(isProcedure(t1));

    compilerAssert(!chpl_isLocalProc(p1));
    compilerAssert(!chpl_isLocalProc(t1));
    compilerAssert(!chpl_isLocalProcType(t1));

    compilerAssert(chpl_isWideProc(p1));
    compilerAssert(chpl_isWideProc(t1));
    compilerAssert(chpl_isWideProcType(t1));

    compilerAssert(chpl_isExternProc(p1));
    compilerAssert(chpl_isExternProc(t1));
    compilerAssert(chpl_isExternProcType(t1));

    const p2 = chpl_toLocalProc(p1);
    type t2 = p2.type;

    compilerAssert(isProcedureValue(p2));
    compilerAssert(isProcedureType(t2));
    compilerAssert(isProcedure(p2));
    compilerAssert(isProcedure(t2));

    compilerAssert(chpl_isLocalProc(p2));
    compilerAssert(chpl_isLocalProc(t2));
    compilerAssert(chpl_isLocalProcType(t2));

    compilerAssert(!chpl_isWideProc(p2));
    compilerAssert(!chpl_isWideProc(t2));
    compilerAssert(!chpl_isWideProcType(t2));

    compilerAssert(chpl_isExternProc(p2));
    compilerAssert(chpl_isExternProc(t2));
    compilerAssert(chpl_isExternProcType(t2));
  }

  proc ref load(sym: string, type T) throws {
    if _err != nil then throw _err;
    var err;
    var ret = _bin!.loadSymbol(sym, T, err);
    if err != nil then throw err;
    _assertRetrievedProcedureProperties(ret);
    return ret;
  }
}

proc test0() {
  writeln(getRoutineName());

  var bin = new dynamicLibrary("./TestCLibraryToLoad.so");

  type P1 = proc(_: int, _: int): int;

  const addTwoReturn = try! bin.load("addTwoReturn", P1);

  writeln(addTwoReturn.type:string);
  writeln();

  for loc in Locales do on loc {
    const n = (here.id : int);
    const x = addTwoReturn(n, n);
    writeln(here, ' got: ', x);
    writeln('---');
  }
  writeln();
}

proc test1() {
  writeln(getRoutineName());

  var bin = new dynamicLibrary("./TestCLibraryToLoad.so");

  type P1 = proc(a: real, b: real, c: real, d: real, e: real,
                 f: real,
                 g: real,
                 h: real,
                 x: baz): foobar;
  type P2 = proc(x: foobar): void;

  const createFoobar = bin.load("createFoobar", P1);
  const printFoobar = bin.load("printFoobar", P2);

  writeln(createFoobar.type:string);
  writeln(printFoobar.type:string);
  writeln();

  for loc in Locales do on loc {
    writeln(here);

    // Create the arguments.
    const n = (here.id : int);
    var b = new baz(1*n:int(32), 2*n:int(32), 3*n:int(32), 4*n:int(32));

    // Perform the extern call.
    const x = createFoobar(n, n, n, n, n, n, n, n, b);

    // Perform the extern call.
    printFoobar(x);

    // Confirm things look the same on Chapel's end.
    writeln('writeln: ', x);

    writeln('---');
  }
}

proc test2() {
  writeln(getRoutineName());

  // Here neither the library or procedure exist.
  var bin = new dynamicLibrary("SOME_FILE_THAT_DOES_NOT_EXIST");

  try {
    type P = proc(): void;
    const p = bin.load("SOME_SYMBOL_THAT_DOES_NOT_EXIST", P);
    assert(p == nil);
  } catch e {
    writeln('Caught error!');
    // TODO: Sanitize the dynamic linker output to be platform-independent.
    // writeln(e.message());
  }
}

proc test3() {
  writeln(getRoutineName());

  // Here the library exists but the procedure does not.
  var bin = new dynamicLibrary("./TestCLibraryToLoad.so");
  try {
    type P = proc(): void;
    const p = bin.load("SOME_SYMBOL_THAT_DOES_NOT_EXIST", P);
    assert(p == nil);
  } catch e {
    writeln('Caught error!');
    // TODO: Sanitize the dynamic linker output to be platform-independent.
    // writeln(e.message());
  }
}

// The goal of this test is to create heavy cross-locale contention.
// As well, there should be no problems retrieving the same symbol
// multiple times regardless of locale.
proc test4() {
  writeln(getRoutineName());

  var bin = new dynamicLibrary("./TestCLibraryToLoad.so");

  type P1 = proc(_: int, _: int): int;

  const wideProc0 = try! bin.load("addTwoReturn", P1);
  assert(wideProc0 != nil);

  // OK to take 'bin' here by 'ref' intent, it should be parallel-safe.
  const maxIters = hi;
  forall i in 1..<maxIters with (ref bin) {
    const loc = Locales[i % numLocales];
    on loc {
      const wideProc = try! bin.load("addTwoReturn", P1);
      assert(wideProc == wideProc0);
      const x = wideProc(here.id, here.id);
      assert(x == (here.id * 2));
    }
  }
}

proc main() {
  test0();
  test1();
  test2();
  test3();
  test4();
}
