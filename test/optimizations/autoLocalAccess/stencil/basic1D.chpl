use StencilDist;

const n = 10;

const Dom = stencilDist.createDomain({0..<n}, fluff=(1,));
const DomInner = Dom.expand(-2);

var InArr: [Dom] int = 1;
var OutArr: [Dom] int;

InArr.updateFluff();

writeln("Starting loop 1, expecting all local accesses (total 36)");
forall i in DomInner {
  OutArr[i] =
    InArr[i-1] +
    InArr[i-0] +
    InArr[i  ] +
    InArr[i+0] +
    InArr[i+1];
}
writeln(OutArr);
writeln("End first loop");



writeln("Starting loop 2, expecting all default accesses (total 48)");
// This loop locks in the behavior where failure in one of the dynamic checks
// causes all accesses to fall back to default offset. This is clearly not
// ideal, however, per-access dynamic check would be huge load on the compiler
// as we'd have to clone too many loops.
forall i in DomInner {
  OutArr[i] =
    InArr[i-2] +
    InArr[i-1] +
    InArr[i-0] +
    InArr[i  ] +
    InArr[i+0] +
    InArr[i+1] +
    InArr[i+2];
}
writeln(OutArr);
writeln("End second loop");

var OutArrInner: [DomInner] int;

writeln("Starting loop 3, expecting local access on the output (total 6) ",
        "expecting default access on the input (total 42)");
// In this variation, the output array's locality can be statically proven.
// Therefore, we don't need any dynamic checks on that. That must lead to
// accesses to `OutArrInner` to be optimized, while others still using default
// access due to failing dynamic checks as noted above.
forall i in DomInner {
  OutArrInner[i] =
    InArr[i-2] +
    InArr[i-1] +
    InArr[i-0] +
    InArr[i  ] +
    InArr[i+0] +
    InArr[i+1] +
    InArr[i+2];
}
writeln(OutArrInner);
writeln("End second loop");

// Everything is the same as above, but this time, we're doing '1+i's

writeln("Starting loop 4, expecting all local accesses (total 36)");
forall i in DomInner {
  OutArr[i] =
    InArr[-1+i] +
    InArr[-0+i] +
    InArr[i  ] +
    InArr[+0+i] +
    InArr[1+i];
}
writeln(OutArr);
writeln("End first loop");

