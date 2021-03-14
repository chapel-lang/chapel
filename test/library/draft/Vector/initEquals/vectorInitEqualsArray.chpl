private use Vector;

//
// Initialize a vector from an array.
//
{
  var arr = [1, 2, 3, 4, 5, 6, 7, 8];
  var lst1: vector(int) = arr;

  // Both the long and short forms of `init=` should have the same type.
  var lst2 = lst1;
  assert(lst1.type == lst2.type);

  assert(arr.size == lst1.size);

  for (x, y) in zip(arr, lst1) do
    assert(x == y);

  lst1.clear();

  assert(lst1.isEmpty() && !arr.isEmpty());
}
