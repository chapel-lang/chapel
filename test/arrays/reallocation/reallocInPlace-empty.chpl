record R {
  var x = 42;
}
var D = {1..0};
var A: [D] R;

// test default initialization
writeln(A);

// init manually
for i in D do
  A[i].x = i;
writeln(A);

// this could maybe realloc in place, but doesn't currently, due to empty start
D = {1..10};
writeln(A);

// this can, though
D = {1..20};
writeln(A);

// and this too
D = {1..10};
writeln(A);

// this also could (?), but doesn't currently, due to empty result
D = {1..0};
writeln(A);
