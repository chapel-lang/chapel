use Itertools;

var A: [1..10] int;
forall (id, el) in zip(1..10, cycle((1, 2, 3, 4))) do A[id] = el;
writeln(A);
forall (id, el) in zip(1..10, cycle('ABC')) do A[id] = el.toByte();
writeln(A);

var B: [2..11] int;
forall (id, el) in zip(2..11, cycle([1, 2, 3])) do B[id] = el;
writeln(B);
