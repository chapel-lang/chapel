
record RRcon { }
proc type RRcon.typemeth type return string;

proc test1(a: RRcon.typemeth) {
  compilerWarning("test1 RRcon.typemeth");
}

record RRgen { type tt; }
proc type RRgen.typemeth type return string;

proc test2(a: RRgen.typemeth) {
  compilerWarning("test2 RRgen.typemeth");
}

class CCcon { }
proc type CCcon.typemeth type return real;

proc test1(a: CCcon.typemeth) {
  compilerWarning("test1 CCcon.typemeth");
}

class CCgen { type qq; }
proc type CCgen.typemeth type return real;

proc test2(a: CCgen.typemeth) {
  compilerWarning("test2 CCgen.typemeth");
}

test1("hi");
test2("hi");
test1(5.6);
test2(5.6);
compilerError("success");
