use Itertools;

//follower
var A: [1..10] int;
forall (id, el) in zip(1..10, cycle(1..4)) do A[id] = el;
writeln(A);
forall (id, el) in zip(1..10, cycle({1..6})) do A[id] = el;
writeln(A);

var B: [2..11] int;
forall (id, el) in zip(2..11, cycle(1..6)) do B[id] = el;
writeln(B);

//leader
var C: [1..20] int;
forall (el, id) in zip(cycle(1..5, 4), 1..20) do C[id]=el;
writeln(C);
[id in 1..20] C[id]=0;
forall (el, id) in zip(cycle({1..5}, 4), 1..20) do C[id]=el;
writeln(C);

var D: [2..21] int;
forall (el, id) in zip(cycle(1..5, 4), 2..21) do D[id]=el;
writeln(D);
