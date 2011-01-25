proc foo() {
  for i in 1..3 {
    var A: [1..3] int;
    if i == 2 then
      continue;
    writeln(A);
  }
}

foo();
