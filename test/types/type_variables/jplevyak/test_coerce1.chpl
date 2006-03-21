function foo(x1 : int, x2 : int) {
  writeln("foo w/ int 1 = ", x1, " and int 2 = ", x2);
}

function foo(x1 : float, x2 : float) {
  writeln("foo w/ float 1 = ", x1, " and float 2 = ", x2);
}

function foo(x1 : int, x2 : float) {
  writeln("foo w/ int 1 = ", x1, " and float 2 = ", x2);
}

var i : int;
i = 2;
var f : float;
f = 3;

foo(i, i);
foo(i, f);
foo(f, f);
