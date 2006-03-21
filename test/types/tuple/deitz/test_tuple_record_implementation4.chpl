typedef mytuple : int = 12;

function foo(t : mytuple, param i : int) where i == 1 {
  return 1;
}

function foo(t : mytuple, param i : int) where i == 2 {
  return 2;
}

var t : mytuple;

writeln(t);

t = 4;

writeln(t);

writeln(foo(t, 1));
writeln(foo(t, 2));
