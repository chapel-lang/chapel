function foo1() {
  var x : integer = 12;
  writeln(x);
}

function foo2(i : integer) {
  var x : integer = 13;
  writeln(x, i);
}

function foo3(i : integer) {
  writeln(i);
}

function foo4(i : integer) {
  var j : integer = 12;
  {
    var k : integer = 13;
    writeln(i, j, k);
  }
}

foo1();
foo2(11);
foo3(11);
foo4(11);
