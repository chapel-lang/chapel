use driver, driver_domains, driver_arrays, driver_misc;
use Sort, IO;

config const offset = 7;

for i in D {
  var myIdx = 2*i+offset;
  DomIntType += myIdx;
  DomUintType += myIdx:uintType;
  DomRealType += myIdx:realType;
  DomStringType += stringify(myIdx);
}

forall ai in DomIntType do
  AInt(ai) = ai;
writeln(AInt.sorted());

forall au in DomUintType do
  AUint(au) = au;
writeln(AUint.sorted());

forall ar in DomRealType do
  AReal(ar) = ar;
writeln(AReal.sorted());

forall aStr in DomStringType do
  AString(aStr) = aStr;
writeln(AString.sorted());
