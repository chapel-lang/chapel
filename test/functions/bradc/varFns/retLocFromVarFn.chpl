record myarr {
  var x: real;

  proc this(i:int, j: int) ref {
    var irv = 0.0;

    if (i==0 && j == 0) {
      return irv;
    } else {
      return x;
    }
  }
}


var a: myarr;

a(1,1) = 2.0;
a(0,0) = 3.0;
writeln(a(1,1));
writeln(a(0,0));


writeln("Shouldn't compile, I think.");
