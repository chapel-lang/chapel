// just checking that the RHS is a param since the test
// will fail if that's not the case
param p = 1 + 0i;

var A: [0..3] complex(64);
// Note: If rewritten (more correctly) as `A = (1 + 0i): t;`, error does not occur
A = 1 + 0i;

writeln('OK');
