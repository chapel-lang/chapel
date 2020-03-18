record R {
  var x = 42;
  proc deinit() {
    writeln("In deinit");
  }
}
var D = {1..10};
var A: [D] R;

// test default initialization
writeln(A);

// init manually
for i in D do
  A[i].x = i;
writeln(A);

// this should realloc in place
D = {1..20};
writeln(A);

// this should not since the low bound is changing and A[i] must be preserved
D = {0..21};
writeln(A);

// this should realloc in place and call deinit for 11 elements
D = {0..10};
writeln(A);
