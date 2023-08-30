// https://github.com/chapel-lang/chapel/issues/11930#issuecomment-1694507166

var D = {1..3};
var A: [D] int;

writeln(D.hasSingleLocalSubdomain());
for d in D.localSubdomains() do writeln(d);

writeln(A.hasSingleLocalSubdomain());
for d in A.localSubdomains() do writeln(d);
