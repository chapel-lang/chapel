use StencilDist;

config const useLocalAccess = false;

const targetLocales = [loc in 0..<numLocales*2] Locales[loc%numLocales];

config const n = 10;
const space = {1..n};
const Dom = space dmapped new stencilDist(space, targetLocales=targetLocales);
var A: [Dom] int;

A = 0;
A[1] = 1;
A[n] = n;
writeln(A);

A = 0;
A.localAccess[1] = 1;
A.localAccess[7] = 7;
writeln(A);

A = space;
const B = + scan A;
writeln(B);

A.localAccess[n] = -n;
writeln(A);
