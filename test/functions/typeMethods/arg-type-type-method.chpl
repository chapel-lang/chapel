
record RR { }
proc type RR.typemeth type return string;

proc test(a: RR.typemeth) {
  compilerWarning("test RR.typemeth");
}

class CC { }
proc type CC.typemeth type return real;

proc test(a: CC.typemeth) {
  compilerWarning("test CC.typemeth");
}

test("hi");
test(5.6);
compilerError("success");
