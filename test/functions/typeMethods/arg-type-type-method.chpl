

class CC { }
proc type CC.typemethod type return int;

proc test(arg: CC.typemethod) {
  compilerError("success");
}

test(5);
