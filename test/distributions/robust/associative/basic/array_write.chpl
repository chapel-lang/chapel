use driver, driver_domains, driver_arrays, driver_misc;
use Sort, IO;

config const offset = 7;

for i in D {
  var myIdx = 2*i+offset;
  DomIntType += myIdx;
  DomUintType += myIdx:uintType;
  DomRealType += myIdx:realType;
  DomStringType += string.ioWrite(myIdx);
}

forall ai in DomIntType do
  AInt(ai) = ai;
writeln(sorted(AInt));

forall au in DomUintType do
  AUint(au) = au;
writeln(sorted(AUint));

forall ar in DomRealType do
  AReal(ar) = ar;
writeln(sorted(AReal));

forall aStr in DomStringType do
  AString(aStr) = aStr;
writeln(sorted(AString));
