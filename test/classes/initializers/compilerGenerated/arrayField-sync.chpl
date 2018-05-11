
class C {
  var A : [1..10] sync int;

  proc writeThis(f) {
    f.write(A.readXX());
  }
}

var c = new C();
writeln(c);
delete c;
