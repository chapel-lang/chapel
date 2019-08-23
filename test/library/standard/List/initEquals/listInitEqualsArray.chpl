private use Lists;

//
// Initialize a list from an array.
//
{
  var arr = [1, 2, 3, 4, 5, 6, 7, 8];
  var lst: list(int) = arr;

  assert(arr.size == lst.size);

  for (x, y) in zip(arr, lst) do
    assert(x == y);

  lst.clear();

  assert(lst.isEmpty() && !arr.isEmpty());
}
