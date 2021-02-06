private use UnrolledLinkedList;

//
// Initialize a list from another list.
//
{
  var lst1: unrolledLinkedList(int);
  for x in 1..8 do
    lst1.append(x);

  var lst2: unrolledLinkedList(int) = lst1;

  // Both the long and short forms of `init=` should have the same type.
  var lst3 = lst2;
  assert(lst3.type == lst2.type);

  for (x, y) in zip(lst1, lst2) do
    assert(x == y);

  lst1.clear();

  assert(lst2.size > lst1.size);
  assert(lst1.size == 0);
}
