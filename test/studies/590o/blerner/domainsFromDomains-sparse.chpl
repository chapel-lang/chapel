config const n = 9;

const Ind = {1..n};
const D1: sparse subdomain(Ind) = [i in Ind] if (i%2) then i;
const D2: sparse subdomain(Ind) = [i in Ind] if ((i%2) == 0) then i;

writeln("D1 is: ", D1);
writeln("D2 is: ", D2);

const D = {D1, D2};

var A: [D] real = [(i,j) in D] i + j/10.0;
var B: [D1, D2] real = 0.0;

forall (i,j) in D {
  B(i,j) = i + j/10.0;
}

writeln("A is:\n", A, "\n");
writeln("B is:\n", B, "\n");
