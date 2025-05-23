use DynamicLoading;
use Reflection;

proc test1() {
  writeln(Reflection.getRoutineName());
  writeln();

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
  writeln();
}

proc main() {
  test1();
}
