
class C {
  var A : [1..10] sync int;

  override proc writeThis(f) {
    f.write(A.readXX());
  }
}

var c = new unmanaged C();
writeln(c);
delete c;
