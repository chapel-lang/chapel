class C {
  var a: int;
  var b: int;

}

def C.initialize() {
  b = a;
}

var myC = new C(a=2);

writeln("myC is: ", myC);
