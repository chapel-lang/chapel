config const n = 4;

var A: [1..n] int;

A = min(int);
var (elt, loc) = maxloc reduce zip(A, A.domain);
if elt != min(int) then halt("elt != min(int)");
if loc != 1 then halt("loc != 1");

A = max(int);
(elt, loc) = minloc reduce zip(A, A.domain);
if elt != max(int) then halt("elt != max(int)");
if loc != 1 then halt("loc != 1");

writeln("SUCCESS");
