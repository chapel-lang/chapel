class T {
  var value = 0;
}

var D: domain(int);

D += 1;

var A: [D] borrowed T?;
var a = new T();
var b = a.borrow();

A[1] = b;

assert(A.size == 1);
