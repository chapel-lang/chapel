function foo(x1 : integer, x2 : integer) {
  writeln("foo w/ integer 1 = ", x1, " and integer 2 = ", x2);
}

function foo(x1 : float, x2 : float) {
  writeln("foo w/ float 1 = ", x1, " and float 2 = ", x2);
}

function foo(x1 : integer, x2 : float) {
  writeln("foo w/ integer 1 = ", x1, " and float 2 = ", x2);
}

var i;
i = 2;
var f;
f = 3.0;

foo(i, i);
foo(i, f);
foo(f, f);
