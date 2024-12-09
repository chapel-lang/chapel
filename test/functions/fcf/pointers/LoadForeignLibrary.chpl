use ChapelDynamicLoading;

/** Define a "user facing" wrapper around the internal dynamic library. */
record dynamicLibrary {
  var _bin: unmanaged chpl_BinaryInfo?;
  var _err: owned DynLibError?;

  proc init(path: string) {
    init this;
    this._bin = chpl_BinaryInfo.create(path, this._err);
    _bin!.bumpRefCount();
  }

  proc deinit() {
    _bin!.dropRefCount();
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
  var bin = new dynamicLibrary("./TestCLibraryToLoad.so");

  type P1 = proc(_: int, _: int): int;

  const addTwoReturn = try! bin.load("addTwoReturn", P1);

  writeln(addTwoReturn.type:string);

  for loc in Locales do on loc {
    const n = (here.id : int);
    const x = addTwoReturn(n, n);
    writeln(here, ' got: ', x);
  }
}

extern {
  #include "TestTypeOnlyHeader.h"
}

proc test2() {
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
  }
}

proc main() {
  test1();
  test2();
}
