// the internal chpl_compare function may have external users, so it gets a deprecation warning

use Sort;

record R1 {}
proc R1.key(x) do return x;
writeln(chpl_compare(1, 2, comparator=new R1()));

record R2 {}
proc R2.compare(x, y) do return if x < y then -1 else 1;
writeln(chpl_compare(3, 4, comparator=new R2()));

record R3 {}
proc R3.compare(x, y) do return if x < y then -1 else 1;
proc R3.keyPart(elt, i) do return (keyPartStatus.returned, elt);
writeln(chpl_compare(5, 6, comparator=new R3()));

record R4 {}
proc R4.keyPart(elt, i) do return (keyPartStatus.returned, elt);
writeln(chpl_compare(7, 8, comparator=new R4()));
