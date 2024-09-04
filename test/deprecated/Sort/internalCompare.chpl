// the internal chpl_compare function may have external users, so it gets a deprecation warning

use Sort;

record R {}
proc R.key(x) do return x;


writeln(chpl_compare(1, 2, comparator=new R()));


