var A: [0..5, 0..5] real;
var C: [1..4, 1..4] arr33;  // Removing this line makes this test pass.

record arr33 {
  var data: [-1..1, -1..1] real;

  proc this(x,y) ref {
    return data(x,y);
  }

  proc this(xy:2*int) ref {
    return data(xy(0), xy(1));
  }
}

writeln(A((1,1)+(0, 0)));

proc +(a: 2*int, b: 2*int) {
  writeln("Choose the correct +");
  return (a(0)+b(0), a(1)+b(1));
}
