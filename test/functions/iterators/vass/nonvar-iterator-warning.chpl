var example1: int, example2: [1..5] int;

iter myiter() {
  yield example1;
  for e2 in example2[1..5] do yield e2;
}

for lval in myiter() do lval = 1;
