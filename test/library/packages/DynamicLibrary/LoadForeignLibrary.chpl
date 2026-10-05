use DynamicLibrary;
use Reflection;
use ChplConfig;

proc test1() {
  writeln(Reflection.getRoutineName());

  const lib = try! loadFromPath("./TestCLibraryToLoad.so");

  type P1 = proc(_: int, _: int): int;

  const addTwoReturn = try! lib.retrieve("addTwoReturn", P1);

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

  const ext = dynamicLibrary.fileExt;

  if CHPL_TARGET_PLATFORM == 'darwin' {
    assert(ext == 'dylib');
  } else {
    assert(ext == 'so');
  }
}

proc test3() {
  writeln(Reflection.getRoutineName());

  const lib = try! load('TestCLibrary');
}

proc test4() {
  writeln(Reflection.getRoutineName());

  const lib1 = try! load('TestCLibrary', 'lib');
  const lib2 = try! load('TestCLibrary', 'lib/');
  const lib3 = try! load('TestCLibrary', 'lib//');
}

proc main() {
  const tests = [ test1, test2, test3, test4 ];
  for test in tests {
    test();
    if test != tests.last then writeln();
  }
}
