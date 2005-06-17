type mytuple : integer = 12;

function foo(t : mytuple, parameter i : integer) where i == 1 {
  return 1;
}

function foo(t : mytuple, parameter i : integer) where i == 2 {
  return 2;
}

var t : mytuple;

writeln(t);

t = 4;

writeln(t);

writeln(foo(t, 1));
writeln(foo(t, 2));
