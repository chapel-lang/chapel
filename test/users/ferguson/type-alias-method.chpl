
// Primitive

type my_type = int;

proc my_type.doit() {
  writeln(this);
}

var str = 5;
str.doit();

// Concrete record

record R {
  var x : int;
}

type OtherR = R;

proc OtherR.doit() {
  writeln(this);
}

var r = new OtherR(5);
r.doit();

// Instantiated generic record

record G {
  type T;
  var x : T;
}

type IntG = G(int);

proc IntG.doit() {
  writeln(this);
}

var g = new IntG(5);
g.doit();
