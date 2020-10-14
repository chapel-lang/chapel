private use Vector;

//
// Initialize a vector containing elements that are copy initialized from the
// elements yielded by a range.
//
{
  const r = 1..1023;
  var lst1: vector(int) = r;

  // Both the long and short forms of `init=` should have the same type.
  var lst2 = lst1;
  assert(lst1.type == lst2.type);

  for (x, y) in zip(r, lst1) do
    assert(x == y);
}
