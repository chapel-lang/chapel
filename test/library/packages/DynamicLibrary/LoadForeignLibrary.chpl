use DynamicLoading;
use Reflection;
use ChplConfig;

proc test1() {
  writeln(Reflection.getRoutineName());

  const bin = try! binary.load("./TestCLibraryToLoad.so");

  type P1 = proc(_: int, _: int): int;

  const addTwoReturn = try! bin.retrieve("addTwoReturn", P1);

  writeln(addTwoReturn.type:string);
  writeln();

  for loc in Locales do on loc {
    const n = (here.id : int);
    const x = addTwoReturn(n, n);
    writeln(here, ' got: ', x);
    writeln('---');
  }
}

proc test2() {
  writeln(Reflection.getRoutineName());

  const suffix = binary.libSuffix;

  if CHPL_TARGET_PLATFORM == 'darwin' {
    assert(suffix == 'dylib');
  } else {
    assert(suffix == 'so');
  }
}

proc test3() {
  writeln(Reflection.getRoutineName());

  const suffix = binary.libSuffix;
  const libName1 = binary.libName('foo');
  assert(libName1 == 'libfoo.' + suffix);

  const libName2 = binary.libName('baz', 'foo/bar');
  assert(libName2 == 'foo/bar/libbaz.' + suffix);
}

proc test4() {
  writeln(Reflection.getRoutineName());

  const bin = try! binary.loadLib('TestCLibrary');
}

proc test5() {
  writeln(Reflection.getRoutineName());

  const bin1 = try! binary.loadLib('TestCLibrary', 'lib');
  const bin2 = try! binary.loadLib('TestCLibrary', 'lib/');
  const bin3 = try! binary.loadLib('TestCLibrary', 'lib//');
}

proc main() {
  const tests = [ test1, test2, test3, test4, test5 ];
  for test in tests {
    test();
    if test != tests.last then writeln();
  }
}
