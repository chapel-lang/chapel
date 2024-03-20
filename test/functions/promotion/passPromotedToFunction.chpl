record R { type xType; var x: xType; }
proc testit(ref r: R(?)) { r.x += 1; }

var A: [1..1] R(int);
var B: [1..1] atomic int;
var I: [1..1] int = [1, ];

// A[I].x += 1;
forall i in I with (ref A) do A[i].x += 1;
// testit(A[I]);
forall i in I with (ref A) do testit(A[i]);

writeln(A);

// B[I].add(1);
forall i in I with (ref B) do B[i].add(1);
writeln(B);
