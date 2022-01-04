record R {
  var data: [1..3] real;
}

record S {
  var rr: R;
  
  proc foo() {
    forall i in 1..3 {
      rr.data[i] = i;
    }

    writeln(rr);
  }
}

var s: S;
s.foo();

