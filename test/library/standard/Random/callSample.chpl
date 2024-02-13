// use Random; works fine
use Random;

var arr: [1..3] int = 1..3;
var rs = new randomStream(int, false);

var result = rs.sample(arr, 3, withReplacement=false);
{
  use Sort;
  sort(result);
  writeln(result);
}
