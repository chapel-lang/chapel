record R {
  type t;
  var x: t;
}

var D = {1..3};

writeln(D);

var rD = new R(D.type);

writeln(rD);

var rrD = new R(rD.type);

writeln(rrD);
