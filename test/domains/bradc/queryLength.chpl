config var n = 10;

var D = [1..n];

var A: [D] complex;

foo(A);

def foo(X: [?D] complex) {
  var len = D(1).length;
  writeln("length of X is: ", len);
}
