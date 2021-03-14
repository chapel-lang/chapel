record R {
  var D = {1..3};
  var A: [D] int;
}

record RR {
  var DD = {1..3};
  var AA: [1..3] R;
  proc this(i) ref {
    return AA(i).A;
  }
}

var rr: RR;

rr(2) = 2..4;
rr(2) = 2..4;
rr(2) = 2..4;
writeln(rr);
