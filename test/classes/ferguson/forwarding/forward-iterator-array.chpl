
record R {
  var A:[1..4] int;
  forwarding A only these;
}

proc test() {
  var r = new R();
  forall i in r {
    writeln(i);
  }
}

test();
