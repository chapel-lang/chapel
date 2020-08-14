use UnrolledLinkedList;

var lst1: unrolledLinkedList(int, false);
var lst2: unrolledLinkedList(int, true);

if lst1.parSafe == lst2.parSafe then
  compilerError("Lists have matching parSafe values");
