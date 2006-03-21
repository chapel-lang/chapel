fun foo1() {
  var x : int = 12;
  writeln(x);
}

fun foo2(i : int) {
  var x : int = 13;
  writeln(x, i);
}

fun foo3(i : int) {
  writeln(i);
}

fun foo4(i : int) {
  var j : int = 12;
  {
    var k : int = 13;
    writeln(i, j, k);
  }
}

foo1();
foo2(11);
foo3(11);
foo4(11);
