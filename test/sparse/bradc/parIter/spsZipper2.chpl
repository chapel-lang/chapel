config const n = 9;

const D = {1..n, 1..n};

var S: sparse subdomain(D),
    S2: sparse subdomain(D),
    STot: sparse subdomain(D);

for i in 1..n do
  S += (i,i);

forall (ij, ij2) in zip(S, S2) with (ref STot) {
  STot += ij;
  STot += ij2;
}

writeln("STot = ", STot);
