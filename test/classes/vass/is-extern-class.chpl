
class CC { var x: int; }
extern class EE { var y: real; }
record RR { var z: string; }

var e1,	e2: EE;


// We are checking that the following code resolves.

e2 = new EE();

e1 = e2;
e1 = nil;

if e1 == e2 || e1 == nil || nil == e2
|| e1 != e2 || e1 != nil || nil != e2
then
  writeln("yay");

delete e2;

testme();

// Putting these in a function to avoid the compiler reporting
// "This source location is a guess".
proc testme() {

  compilerWarning("should be false:");
  compilerWarning(isExternClassType(CC):string);
  compilerWarning(isExternClassType(RR):string);
  compilerWarning(isExternClassType(string):string);
  compilerWarning(isExternClassType(int):string);

  compilerWarning("testing EE:");
  compilerWarning(isExternClassType(EE):string);
  compilerWarning(isClassType(EE):string);
  compilerWarning(isRecord(EE):string);

  // Good enough for now.
  compilerError("done.");
}
