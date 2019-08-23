private use Lists;

//
// Initialize a list from another list.
//
{
  var lst1: list(int);
  for x in 1..8 do
    lst1.append(x);

  var lst2: list(int) = lst1;

  for (x, y) in zip(lst1, lst2) do
    assert(x == y);

  lst1.clear();

  assert(lst2.size > lst1.size);
  assert(lst1.size == 0);
}
