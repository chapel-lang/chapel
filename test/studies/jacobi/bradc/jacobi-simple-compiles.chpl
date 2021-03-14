// configuration constants
config const n = 10;
config const epsilon = 0.0001;


const BigD: domain(2) = {0..n+1, 0..n+1};
const D: domain(2) = {1..n, 1..n};
//const D: subdomain(BigD) = {1..n, 1..n};


const north = (-1,  0);
const south = ( 1,  0);
const east  = ( 0,  1);
const west  = ( 0, -1);

proc main() {
  var A: [BigD] real = 0.0, 
      B: [D] real;

  // initialize south boundary to 1.0
  [ij in D.exterior(south)] A(ij) = 1.0;


  var bigdiff: real;

  do {

    [ij in D] B(ij) = (A(ij+north) + A(ij+south) + 
                       A(ij+east)  + A(ij+west) )/4;

    bigdiff = max reduce [ij in D] abs(A(ij) - B(ij));

    [ij in D] A(ij) = B(ij);
  } while (bigdiff > epsilon);

  writeln("Final A is: \n", A[D]);
}
