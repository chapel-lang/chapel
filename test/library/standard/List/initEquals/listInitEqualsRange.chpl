private use Lists;

//
// Initialize a list containing elements that are copy initialized from the
// elements yielded by a range.
//
{
  const r = 1..1023;
  var lst: list(int) = r;
  for (x, y) in zip(r, lst) do
    assert(x == y);
}
