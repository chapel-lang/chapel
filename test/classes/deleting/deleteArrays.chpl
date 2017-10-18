class C {
  proc deinit() {
    writeln("Deleting a C");
  }
}

{
  var A: [1..3] C = [1..3] new C();

  delete A;
}
writeln("---");
{
  var A: [1..3] C;

  delete A;
}
writeln("---");
{
  var A: [1..3] [1..3] C;
  for i in 1..3 do
    for j in 1..3 do
      A[i][j] = new C();

  delete A;
}
writeln("---");
{
  var A: [1..3] C = [1..3] new C();
  var myC = new C();
  var B: [1..4] C = [1..4] new C();
  var myC2 = new C();
  var D: [1..2] [1..3] C;
  for i in 1..2 do
    for j in 1..3 do
      D[i][j] = new C();

  delete A, myC, B, myC2, D;
}
