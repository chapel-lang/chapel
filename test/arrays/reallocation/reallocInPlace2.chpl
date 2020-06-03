record R {
  var x = 42;
  proc deinit() {
    writeln("In deinit");
  }
}
writeln("D = {1..10}");
var D = {1..10};
var A: [D] R;

// test default initialization
writeln(A);

// init manually
for i in D do
  A[i].x = i;
writeln(A);

// this should realloc in place
// this should not call R.deinit
writeln("D = {1..20}");
D = {1..20};
writeln(A);

// this should not since the low bound is changing and A[i] must be preserved
// this should not call R.deinit
writeln("D = {0..21}");
D = {0..21};
writeln(A);

writeln("D = {0..10}");
// this should realloc in place and call deinit for 11 elements
// namely 11..21
D = {0..10};
writeln(A);

writeln("D = {0..10}");
// this shouldn't realloc at all because the domain isn't changing
D = {0..10};
writeln(A);
