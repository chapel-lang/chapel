// Test of generic return/yield types w/ and w/o tuples.
// Issues #10172, #11461.

class VectorListElement {  var x;  }

const VLE = new borrowed VectorListElement(7);

proc p1(): VectorListElement { return VLE; }
writeln(p1());

proc p2(): (VectorListElement,) { return (VLE,); }
writeln(p2());

iter i1(): VectorListElement { yield VLE; }
for j1 in i1() do writeln(j1);

iter i2(): (VectorListElement,) { yield (VLE,); }
for j2 in i2() do writeln(j2);
