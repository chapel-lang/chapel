class C {
  var A: [1..3] borrowed C?;
}

var c = new unmanaged C();

writeln(c);

delete c;
