private use UnrolledLinkedList;

//
// Initialize a list containing elements that are copy initialized from the
// elements yielded by a range.
//
{
  const r = 1..1023;
  var lst1: unrolledLinkedList(int);
  for x in r do
    lst1.append(x);

  // Both the long and short forms of `init=` should have the same type.
  var lst2 = lst1;
  assert(lst1.type == lst2.type);

  for (x, y) in zip(r, lst1) do
    assert(x == y);
}
