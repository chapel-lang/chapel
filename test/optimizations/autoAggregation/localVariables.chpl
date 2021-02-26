use BlockDist;

var a = newBlockArr(0..10, int);

forall i in a.domain {
  var d: 11*int;
  a[i] = d[10-i];
}

writeln(a);

forall i in a.domain {
  var d: 11*int;
  d[10-i] = a[i];
}

writeln(a);

// this doesn't make much sense but should compile
// at least make sure we compile this
forall i in a.domain {
  var d: 11*int;
  var e: 11*int;
  d[10-i] = e[i];
}
