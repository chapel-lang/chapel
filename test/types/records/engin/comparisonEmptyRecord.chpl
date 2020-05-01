write("Empty record... ");
{
  record R {}

  var r1 = new R();
  var r2 = new R();

  test(r1, r2);
}
writeln("success");

write("Record with only type field... ");
{
  record R { type t; }

  var r1 = new R(int);
  var r2 = new R(int);

  test(r1, r2);
}
writeln("success");

proc test(r1, r2) {
  assert((r1 == r2) == true);
  assert((r1 != r2) == false);

  assert((r1 < r2) == false);
  assert((r2 < r1) == false);
  assert((r1 <= r2) == true);
  assert((r2 <= r1) == true);

  assert((r1 > r2) == false);
  assert((r2 > r1) == false);
  assert((r1 >= r2) == true);
  assert((r2 >= r1) == true);
}
