// configuration constants
config const n = 10;              // problem size
config const epsilon = 0.0001;    // threshold for iteration

// domains for the problem
const BigD: domain(2) = {0..n+1, 0..n+1};
const D: subdomain(BigD) = {1..n, 1..n};  // really want to declare this first
// could also imagine wanting to declare one of these in terms of the other

// "directions" in the form of integer tuples
const north = (-1,  0);
const south = ( 1,  0);
const east  = ( 0,  1);
const west  = ( 0, -1);

proc main() {
  var A: [BigD] real = 0.0;
  var B: [D] real;

  // initialize south boundary to 1.0
  [ij in D.exterior(south)] A(ij) = 1.0;

  // it's a shame not to be able to declare this when it's used
  var bigdiff: real;

  do {
    // assumes promotion of + over homogeneous tuples
    B = (A[D.translate(north)] + A[D.translate(south)] +
         A[D.translate(east)]  + A[D.translate(west)]  )/4;

    bigdiff = max reduce [ij in D] abs(A(ij) - B(ij));

    [ij in D] A(ij) = B(ij);
  } while (bigdiff > epsilon);

  writeln("Final A is: \n", A[D]);
}
