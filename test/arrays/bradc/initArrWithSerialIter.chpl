var A: [1..10] real = initArr();

iter initArr() {
  for i in 1..10 do
    yield i + i/10.0;
}

writeln(A);
