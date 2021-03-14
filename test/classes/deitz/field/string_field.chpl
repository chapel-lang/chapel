class C {
  var s : string;
}

var c : unmanaged C = new unmanaged C("hello world");

writeln(c);

delete c;
