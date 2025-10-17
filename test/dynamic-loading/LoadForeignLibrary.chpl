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

use DynamicLibraryWrapper;
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

// This test demonstrates that a library will persist on the locale that
// it is originally loaded, even if other locales attempt to load it after.
proc test5() {
  writeln(getRoutineName());

  // First create the library on LOCALE-0. This is where it will live.
  var bin0 = new dynamicLibrary("./TestCLibraryToLoad.so");
  type P1 = proc(_: int, _: int): int;
  const wideProc0 = try! bin0.load("addTwoReturn", P1);
  assert(wideProc0 != nil);

  // TODO: Need a way to force evict libraries before execution ends.
  assert(bin0._bin!.locale == here);

  coforall loc in Locales do on loc {
    var binHere = new dynamicLibrary("./TestCLibraryToLoad.so");

    // The locales should match.
    assert(binHere._bin!.locale == bin0._bin!.locale);

    // And loading should work without incident.
    type P1 = proc(_: int, _: int): int;
    const wideProcHere = try! binHere.load("addTwoReturn", P1);
    assert(wideProcHere != nil);

    // And should have the same wide index.
    assert(wideProcHere == wideProc0);
  }
}

proc main() {
  test0();
  test1();
  test2();
  test3();
  test4();
  test5();
}
