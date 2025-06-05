import Sort;
use printHelp;

var d : domain(string);
var a : [d] int;

d.add("two");
d.add("zero");

a("two") = 7;
a("zero") = 4;

writelnSorted(d);
writelnSortedByDom(a);

for i in Sort.sorted(d) do
  writeln(i, " -> ", a(i));
