class C {
  var A: [1..3] C;
}

var c = new unmanaged C();

writeln(c);

delete c;
