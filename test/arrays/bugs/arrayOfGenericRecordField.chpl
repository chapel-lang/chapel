record R {
  param i: int;
}

proc f(
  const ref arr: [] R
)
{
  writeln(arr.eltType.i);
}

proc main() {
  var array: [1..8] R(10);
  f(array);
}
