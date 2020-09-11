proc foo(x: int, param y: int) {
  return x * y;
}

for param j in 1..3 {
  const maxval = [i in 1..10] foo(i,j);
  writeln(maxval);
}

proc main() {
  for param j in 1..3 {
    const maxval = for i in 1..10 do foo(i,j);
    writeln(maxval);
  }
  for param j in 1..3 {
    const maxval = [i in 1..10] foo(i,j);
    writeln(maxval);
  }
}