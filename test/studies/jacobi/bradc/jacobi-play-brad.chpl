// configuration constants
config const n = 10;              // problem size
config const epsilon = 0.0001;    // threshold for iteration

const D: domain(2) = [1..n, 1..n];
// is there anything crazy about declaring a superdomain?
const BigD: superdomain(2) = D.resize((-1,-1), (1, 1));

param StencilD: domain(2) = [-1..1, -1..1];

param weights: [StencilD] real = ( ( 0.00, 0.25, 0.00 ),
                                   ( 0.25, 0.00, 0.25 ),
                                   ( 0.00, 0.25, 0.00 ) );

const south = (1,0);

proc main() {
  var A: [BigD] real = 0.0;
  var B: [D] real;

  // initialize south boundary to 1.0
  [ij in D.exterior(south)] A(ij) = 1.0;

  // it's a shame not to be able to declare this when it's used
  var bigdiff: real;

  do {
    // the hope is that some major constant folding/stencil
    // optimization will take place here
    [ij in D] B(ij) = sum reduce [off in StencilD] (weight(off) * A(ij+off));

    bigdiff = max reduce [ij in D] abs(A(ij) - B(ij));

    [ij in D] A(ij) = B(ij);
  } while (bigdiff > epsilon);

  writeln("Final A is: \n", A[D]);
}


// Changes to the original:
//   * declared D first and BigD as a superdomain of D...
//   * replaced four points of stencil with a stencil domain, weight
//     array, and reduction
