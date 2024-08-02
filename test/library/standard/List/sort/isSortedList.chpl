use List, Sort;

config param locking = false;
{
  var lst = new list(int, locking);
  writeln("empty list: ", isSorted(lst));
}
{
  var lst = new list([1], locking);
  writeln("length 1 list: ", isSorted(lst));
}
{
  var lst = new list([1, 2], locking);
  writeln("length 2 list: ", isSorted(lst));
}
{
  var lst = new list([1, 2, 3, 4, 5], locking);
  writeln("sorted list: ", isSorted(lst));
}
{
  var lst = new list([1, 2, 3, 4, 5, 3], locking);
  writeln("unsorted list: ", isSorted(lst));
}
