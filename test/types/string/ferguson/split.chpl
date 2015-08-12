var A = "this is a test".split();

assert(A[1] == "this");
assert(A[2] == "is");
assert(A[3] == "a");
assert(A[4] == "test");

var B = "this is a test".split(maxsplit=1);

assert(B[1] == "this");
assert(B[2] == "is a test");

var C = "this is a test".split(" a ");

assert(C[1] == "this is");
assert(C[2] == "test");

