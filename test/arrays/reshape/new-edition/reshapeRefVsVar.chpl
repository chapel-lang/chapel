config param copy = false;

var x = [1, 2, 3, 4];

{
  var y = reshape(x, {1..2, 1..2}, copy=copy);

  writeln(y);
  y[1, 1] = 5;

  writeln(x);

  ref z = reshape(x, {1..2, 1..2}, copy=copy);

  writeln(z);
  z[1, 1] = 5;

  writeln(x);
}

x = [1, 2, 3, 4];

{
  var y = reshape(x, 1..2, 1..2, copy=copy);

  writeln(y);
  y[1, 1] = 5;

  writeln(x);

  ref z = reshape(x, 1..2, 1..2, copy=copy);

  writeln(z);
  z[1, 1] = 5;

  writeln(x);
}

