use List;
use Sort;

{
  var lst = new list([4, 8, 2, 1, 7, 3, 9, 6, 5]);
  for x in sorted(lst) {
    writeln(x);
  }
  var y = sorted(lst);
  writeln(y);
}
{
  var lst = new list([4, 8, 2, 1, 7, 3, 9, 6, 5], parSafe = true);
  for x in sorted(lst) {
    writeln(x);
  }
  writeln(sorted(lst));
}
