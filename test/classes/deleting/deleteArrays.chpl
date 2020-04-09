class C {
  proc deinit() {
    writeln("Deleting a C");
  }
}

{
  var A: [1..3] unmanaged C = [1..3] new unmanaged C();

  delete A;
}
writeln("---");
{
  var A: [1..3] unmanaged C?;

  delete A;
}
writeln("---");
{
  var A: [1..3] [1..3] unmanaged C?;
  for i in 1..3 do
    for j in 1..3 do
      A[i][j] = new unmanaged C();

  delete A;
}
writeln("---");
{
  var A: [1..3] unmanaged C = [1..3] new unmanaged C();
  var myC = new unmanaged C();
  var B: [1..4] unmanaged C = [1..4] new unmanaged C();
  var myC2 = new unmanaged C();
  var D: [1..2] [1..3] unmanaged C?;
  for i in 1..2 do
    for j in 1..3 do
      D[i][j] = new unmanaged C();

  delete A, myC, B, myC2, D;
}
