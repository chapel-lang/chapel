use driver, driver_domains, driver_arrays, driver_misc;
use Sort, IO;

config const offset = 7;

for i in D {
  var myIdx = 2*i+offset;
  DomIntType += myIdx;
  DomUintType += myIdx:uintType;
  DomRealType += myIdx:realType;
  DomStringType += myIdx:string;
}

forall ai in DomIntType with (ref AInt) do
  AInt(ai) = ai;
writeln(sorted(AInt));

forall au in DomUintType with (ref AUint) do
  AUint(au) = au;
writeln(sorted(AUint));

forall ar in DomRealType with (ref AReal) do
  AReal(ar) = ar;
writeln(sorted(AReal));

forall aStr in DomStringType with (ref AString) do
  AString(aStr) = aStr;
writeln(sorted(AString));
