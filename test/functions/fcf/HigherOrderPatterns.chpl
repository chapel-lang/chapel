use HigherOrderProcs;

proc test0() {
  writeln(array(1, 2, 3, 4));
}

proc test1() {
  var a = [1,2,3,4,5,6,7,8];
  writeln(dropWhile(a, proc(x: int) { return x <= 4; }));
}

proc test2() {
  var a = [1,2,3,4,5,6,7,8];
  writeln(filter(a, proc(x: int) { return x % 2; }));
}

proc test3() {
  var a = [1,2,3,4,5,6,7,8];
  writeln(foldLeft(a, 0, proc(x: int, y: int) { return x - y; }));
}

proc test4() {
  var a = [1,2,3,4,5,6,7,8];
  writeln(foldRight(a, 0, proc(x: int, y: int) { return x - y; }));
}

proc test5() {
  var a = [1,2,3,4,5,6,7,8];
  writeln(map(a, proc(x: int) { return x*x; }));
}

proc test6() {
  var a = [1,2,3,4,5,6,7,8];
  writeln(takeWhile(a, proc(x: int) { return x <= 4; }));
}

var tests = [
  test0, test1, test2, test3, test4, test5, test6
];

proc main() {
  for test in tests {
    // TODO: For this to be possible function types cannot print addresses.
    // writeln("--- ", test:string, " ---");
    test();
  }
}
