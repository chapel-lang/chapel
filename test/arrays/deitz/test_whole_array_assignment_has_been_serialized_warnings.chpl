var A: [1..5] int;

A = for i in 1..5 do i;
A = for j in 2..6 do j;

iter foo() {
  for i in 1..5 do
    yield i;
}

A = foo();
A = foo();
