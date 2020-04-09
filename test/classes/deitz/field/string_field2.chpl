class C {
  var i : int;
  var s : string;
}

var c : unmanaged C = new unmanaged C(2, "hello world");

writeln(c);

delete c;
