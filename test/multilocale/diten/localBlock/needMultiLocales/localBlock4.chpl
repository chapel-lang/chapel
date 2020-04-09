proc main {
  var A: [1..5] int;
  on Locales(1) {
    for i in A.domain do
      A(i) = i;
  }

  local {
    A(3) = A(4);
  }

  writeln(A);
}
