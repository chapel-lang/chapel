record R {
  param i: int;
}

proc g(
  const ref arr: [] R(?i)
)
{

}

proc main() {
  var array: [1..8] R(10);
  g(array);
}
