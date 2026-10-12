const nC = 5;
var xbar,ybar: [1..nC] real;
for C in 1..nC {
   // -----------------------------------------------------------------
   // The eigenvalues.
   // -----------------------------------------------------------------
   var lambda1 = 10.0;
   // -----------------------------------------------------------------
   // Find the ellipse for each cluster.
   // -----------------------------------------------------------------
   errorTest(lambda1,xbar,ybar);
}
proc errorTest(
   const in lambda1: real,
   out xbar: real,
   out ybar: real) {
   xbar = lambda1;
   ybar = lambda1 + 1.0;
}
