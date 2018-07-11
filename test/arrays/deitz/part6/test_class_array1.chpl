class C {
  var x : [1..3] int;
}

var c = new unmanaged C();
writeln(c);

delete c;
