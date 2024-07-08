type indexType = int(64);

var D32: domain(1, int(32)) = {1..10:int(32)};

class D {
  var x: [D32] real;

  proc this(i: indexType) ref {
    return x(i);
  }
}

var myD = new D();

writeln("myD = ", myD);

myD(1) += 1.1;

writeln("myD = ", myD);
