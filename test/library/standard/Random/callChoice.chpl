// use Random; works fine
use Random;

var arr: [1..3] int = 1..3;
var rs = new randomStream(int, false);

var result = rs.choice(arr, size=3, replace=false); // this calls Random._choice()
{
  use Sort;
  sort(result);
  writeln(result);
}
