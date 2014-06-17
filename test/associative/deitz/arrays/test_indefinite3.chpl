use printHelp;

var d : domain(int);
var a : [d] int;

for i in 1..24 {
  d.add(i);
  a(i) = i;
  writelnSortedByDom(a);
}
