// This test times the various loops run as 'for' vs. 'forall'.

config var n = 1200000;

use Time;
var tm: Timer;

var d1: domain(1) = {1..n};
var d2: domain(2) = {1..n,1..n};

test(1, d1);
test(2, d2);
test(1, d1);
test(2, d2);
test(1, d1);
test(2, d2);

// main test driver

proc test(param dim:int, d: domain(dim)) {
  writeln("=== n=", n, " ", dim, "-d ===");
  // This is our sparse domain that everything will spin around.
  var sd: sparse subdomain(d);
  populateDomain(dim, sd);
//  writeln("sd(", dim, ") = ", sd);

  proc st      {
    tm.clear();
    tm.start();
  }
  proc fi(msg) {
    tm.stop();
    var ms = tm.elapsed(TimeUnits.milliseconds);
    writeln(msg, " : ", ms, " ms");
  }

  var A, B, C: [sd] int;
  var alpha = 10;

  st; forall i in sd { A(i) = 10; }
  fi("ix = const | par");

  st; for    i in sd { B(i) = 20; }
  fi("ix = const | seq");

  st; forall a in A { a = 30; }
  fi("ivar = const | par");

  st; for    b in B { b = 40; }
  fi("ivar = const | seq");

  st; forall i in sd { C(i) = A(i) + alpha * B(i); }
  fi("ix = ix, ix | par");

  st; for    i in sd { A(i) = B(i) + alpha * C(i); }
  fi("ix = ix, ix | seq");

  st; forall (i,j,k) in zip(sd,sd,sd) { C(i) = A(j) + alpha * B(k); }
  fi("ix1 = ix2, ix3 | par");

  st; for    (i,j,k) in zip(sd,sd,sd) { A(i) = B(j) + alpha * C(k); }
  fi("ix1 = ix2, ix3 | seq");

  st; forall (a,b,c) in zip(A,B,C) { c = a + alpha * b; }
  fi("ivar1 = ivar2, ivar3 | par");

  st; for    (a,b,c) in zip(A,B,C) { a = b + alpha * c; }
  fi("ivar1 = ivar2, ivar3 | seq");
}

proc populateDomain(param dim, ref sd) where dim == 1 {
  // for 1-D, use half of the values; domain members are not tuples
  for i in 1..n by 2 do sd += i;
}

proc populateDomain(param dim, ref sd) where dim > 1 {
  for i in 1..n-1 {
    var member: index(sd);
    for param dm in 0..dim-1 do member(dm) =
      // feeble attempt at something more sophisticated than just a diagonal
      if dm % 2 == 1 then i else i + 1;
    sd += member;
  }
}
