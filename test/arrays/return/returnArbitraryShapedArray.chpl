proc inc(X: [] real): [] real {
  if numLocales > 10 then
    return X + 1;
  else
    return X + 2;
}

var A = reshape([1.0, 2.0, 3.0, 4.0, 5.0, 6.0], {0..1,2..4});

writeln(inc(A));
writeln(A);
