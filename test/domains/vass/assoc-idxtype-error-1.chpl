// Exercising the legal-idx-type-for-assoc-domain check.
// This tests that the error is/is not raised.
// Additions to the set of legal cases and
// improvements to the error message are welcome.

record R {}
enum E {eee}

// ensure legal cases still work
var db: domain(bool);
var di: domain(int);
var du: domain(uint);
var dr: domain(real);
var dc: domain(complex);
var dg: domain(imag);
//not interesting: var dt: domain(chpl_taskID_t);
var ds: domain(string);
var dC: domain(borrowed object);
var dE: domain(E);
var dT: domain((int,real));
var dR: domain(R);

// these should raise errors
var ed: domain(domain(1));
var ea: domain([1..3]int);
