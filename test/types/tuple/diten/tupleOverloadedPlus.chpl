var A: [0..5, 0..5] real;
var C: [1..4, 1..4] arr33;  // Removing this line makes this test pass.

record arr33 {
  var data: [-1..1, -1..1] real;

  proc this(x,y) ref {
    return data(x,y);
  }

  proc this(xy:2*int) ref {
    return data(xy(1), xy(2));
  }
}

writeln(A((1,1)+(0, 0)));

proc +(a: 2*int, b: 2*int) {
  writeln("Choose the correct +");
  return (a(1)+b(1), a(2)+b(2));
}
