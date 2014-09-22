record myarr {
  var x: real;

  proc this(i:int, j: int) ref {
    if (i==0 && j == 0) {
      return 0.0;
    } else {
      return x;
    }
  }
}

var a: myarr;

a(1,1) = 2.0;
writeln(a(1,1));

writeln("Shouldn't compile, I think.");
