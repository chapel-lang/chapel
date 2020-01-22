var A = "this is a test".split();
var Au = "être coincé à l'hôtel".split();

assert(A[0] == "this");
assert(A[1] == "is");
assert(A[2] == "a");
assert(A[3] == "test");

assert(Au[0] == "être");
assert(Au[1] == "coincé");
assert(Au[2] == "à");
assert(Au[3] == "l'hôtel");

var B = "this is a test".split(maxsplit=1);
var Bu = "être coincé à l'hôtel".split(maxsplit=1);

assert(B[0] == "this");
assert(B[1] == "is a test");

assert(Bu[0] == "être");
assert(Bu[1] == "coincé à l'hôtel");

var C = "this is a test".split(" a ");
var Cu = "être coincé à l'hôtel".split(" à ");

assert(C[0] == "this is");
assert(C[1] == "test");

assert(Cu[0] == "être coincé");
assert(Cu[1] == "l'hôtel");
