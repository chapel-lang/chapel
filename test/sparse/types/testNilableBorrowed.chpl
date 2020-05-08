class T {
  var value = 0;
}

var D = {1..100};
var sparseD: sparse subdomain(D);

sparseD += 1;

var A: [sparseD] borrowed T?;
var a = new T();
var b: borrowed T? = a.borrow();
A[1] = b;
assert(A.size == 1);
