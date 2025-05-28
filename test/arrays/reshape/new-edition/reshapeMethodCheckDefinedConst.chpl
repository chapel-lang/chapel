var x = [1, 2, 3, 4];

ref y = reshape(x, {1..2, 1..2});

writeln(y);
y[1, 1] = 5;
writeln(y.domain.definedConst);

writeln(x);

{
  var D = {1..2, 1..2};

  ref z = reshape(x, D);
  writeln(z.domain.definedConst);

  writeln(z);
  z[1, 2] = 6;

  writeln(x);
}

{
  const D = {1..2, 1..2};

  ref z = reshape(x, D);
  writeln(z.domain.definedConst);

  writeln(z);
  z[2, 1] = 7;

  writeln(x);
}
