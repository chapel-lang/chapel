public use driver_domains;

var AInt: [DomIntType] intType;
var AUint: [DomUintType] uintType;
var AReal: [DomRealType] realType;
var AString: [DomStringType] stringType;

proc outputArrays() {
  write("AInt  :"); for i in DomIntType do write(" ", AInt[i]); writeln();
  write("AUint  :"); for i in DomUintType do write(" ", AUint[i]); writeln();
  write("AReal  :"); for i in DomRealType do write(" ", AReal[i]); writeln();
  write("AString  :"); for i in DomStringType do write(" ", AString[i]); writeln();
}

