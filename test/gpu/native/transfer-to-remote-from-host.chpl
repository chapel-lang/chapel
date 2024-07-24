const D = {1..10};
const D2 = D.expand(1);

on here.gpus[0] var A: [D2] int;
var B: [D] int;
A[D] = B; // this segfaults
writeln(A);
