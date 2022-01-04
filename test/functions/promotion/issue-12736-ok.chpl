// Working examples from #12736

var DS: domain(string);
writeln(DS.size);           // nb no promotion here
writeln(DS.numCodepoints);  // writes an empty line
DS.add("hi");
DS.add("world");
writeln(DS.size);
writeln(DS.numCodepoints);

// Other testing

record ThisInt {
  const ti: int;
  proc this()    return ti + 1;
  proc this(arg) return ti + arg*10 + 2;
}

record ThisWrite {
  const tw: int;
  proc this()    { writeln(tw + 3); }
  proc this(arg) { writeln(tw + arg*10 + 4); }
}

record Start {
  const sta: int;
  proc getTI return new ThisInt(sta);
  proc getTW return new ThisWrite(sta);
}

const DOM = {6..8};
var ARR = [i in DOM] new Start(i*100);
compilerWarning(ARR.type:string);

writeln(ARR.getTI);
writeln(ARR.getTI());
writeln(ARR.getTI(5));

var ARRTI = ARR.getTI();
writeln(ARRTI.domain); // must be 'DOM'

writeln(ARR.getTW);
ARR.getTW();
ARR.getTW(9);

writeln("triply nested");
record R0 { proc this() return 5; }
record R1 { proc this() return new R0(); }
record R2 { proc foo return new R1(); }
var Array1: [3..5] R2;
var Array2 = Array1.foo()(); // (R2.foo).this().this()
writeln(Array2.domain);
writeln(Array2);
