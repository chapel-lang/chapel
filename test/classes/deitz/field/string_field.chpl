class C {
  var s : string;
}

var c : C = new unmanaged C("hello world");

writeln(c);

delete c;
