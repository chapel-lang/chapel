// ensure that .sorted() continues to work on associative domains
use Sort;
var d: domain(int) = {4,3,5,6,2,1,1};
writeln(d.type:string);
writeln(d.sorted());
writeln(d.sorted(new reverseComparator()));
