
function foo(x: integer) {
  writeln("Got an integer");
}

function foo(x: float) {
  writeln("Got a float");
}

function foo(x: string) {
  writeln("Got a string");
}

function unrollme(x ...?numargs) {
  for param i in 1..numargs {
    foo(x(i));
  }
  writeln();
}

unrollme(1,2,3,4);

unrollme(1, 2.3, "4.5.6", 78);


