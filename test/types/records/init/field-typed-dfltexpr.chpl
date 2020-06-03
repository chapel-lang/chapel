record R1 {
  var x1: int;
  proc init(arg:int) {
    x1 = arg;
  }
}

var glob1 = new R1(7);
writeln(glob1);

record R2 {
  var x2: R1 = glob1;
}

var glob2: R2;
writeln(glob2);
