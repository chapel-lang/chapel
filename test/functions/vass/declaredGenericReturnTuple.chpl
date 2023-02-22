// Test of generic return/yield types w/ and w/o tuples.
// Issues #10172, #11461.

class VectorListElement {  var x;  }

const VLE = (new owned VectorListElement(7)).borrow();

proc p1(): VectorListElement { return VLE; }
writeln(p1());

proc p2(): (VectorListElement,) { return (VLE,); }
writeln(p2());

iter i1(): VectorListElement { yield VLE; }
for j1 in i1() do writeln(j1);

iter i2(): (VectorListElement,) { yield (VLE,); }
for j2 in i2() do writeln(j2);

// Ensure that these tuples are generic.
type t1 = (VectorListElement,);
compilerWarning(t1:string, "  isGeneric: ", isGeneric(t1):string);

record myRecord { type t; }
type t2 = (myRecord,);
compilerWarning(t2:string, "  isGeneric: ", isGeneric(t2):string);

type t3 = (myRecord, VectorListElement);
compilerWarning(t3:string, "  isGeneric: ", isGeneric(t3):string);

type t4 = (int, owned VectorListElement);
compilerWarning(t4:string, "  isGeneric: ", isGeneric(t4):string);
