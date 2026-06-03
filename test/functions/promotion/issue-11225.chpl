// Test for issue #11225
// Promotion with named argument and default value

proc foo(a:int=1, b:int=2) {
  return a + b;
}

var A = [1,2,3,4,5];

// Works: explicit first argument, promoted second argument
var X = foo(1, b=A);
writeln(X);

// Previously failed: default first argument, promoted second argument
var Y = foo(b=A);
writeln(Y);

// Works: both arguments promoted
var B = [10,20,30,40,50];
var Z = foo(a=B, b=A);
writeln(Z);

// Works: promoted first argument with default second
var W = foo(a=A);
writeln(W);
