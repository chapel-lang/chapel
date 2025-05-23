//
// While this exercises the internal dynamic loading API, the intent of this
// test is to exercise the parts of the library that pertain to procedure
// types/values - specifically, that the pointers returned via loading are
// callable on each locale, and broadly behave as expected.
//

use ChapelDynamicLoading;
use CTypes;
use Reflection;

require "TestTypeOnlyHeader.h";

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

  proc ref load(sym: string, type T) throws {
    if _err != nil then throw _err;
    var err;
    var ret = _bin!.loadSymbol(sym, T, err);
    if err != nil then throw err;
    return ret;
  }
}

proc test1() {
  writeln(Reflection.getRoutineName());
  writeln();

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

proc test2() {
  writeln(Reflection.getRoutineName());
  writeln();

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

proc main() {
  test1();
  test2();
}
