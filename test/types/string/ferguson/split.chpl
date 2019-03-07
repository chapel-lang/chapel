var A = "this is a test".split();
var Au = "être coincé à l'hôtel".split();

assert(A[1] == "this");
assert(A[2] == "is");
assert(A[3] == "a");
assert(A[4] == "test");

assert(Au[1] == "être");
assert(Au[2] == "coincé");
assert(Au[3] == "à");
assert(Au[4] == "l'hôtel");

var B = "this is a test".split(maxsplit=1);
var Bu = "être coincé à l'hôtel".split(maxsplit=1);

assert(B[1] == "this");
assert(B[2] == "is a test");

assert(Bu[1] == "être");
assert(Bu[2] == "coincé à l'hôtel");

var C = "this is a test".split(" a ");
var Cu = "être coincé à l'hôtel".split(" à ");

assert(C[1] == "this is");
assert(C[2] == "test");

assert(Cu[1] == "être coincé");
assert(Cu[2] == "l'hôtel");
