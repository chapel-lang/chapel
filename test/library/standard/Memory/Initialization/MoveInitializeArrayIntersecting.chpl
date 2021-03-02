use Memory.Initialization;

config param isSingleArray = false;

proc test1() {
  var a: [0..5] int = [0, 0, 1, 2, 3, 4];
  moveInitializeArrayElements(a, 0, 2, 4);
  writeln(a);
}

proc test2() {
  var a1: [0..5] int = [0, 0, 1, 2, 3, 4];
  moveInitializeArrayElements(a1, 0, a1, 2, 4);
  writeln(a1);
}

if isSingleArray {
  test1();
} else {
  test2();
}

