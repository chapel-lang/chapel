config param tupSize = 5;
config const n = 10;

on here.getChild(1) {
  var A: [1..n] int;

  foreach a in A {
    var x: tupSize*int;
    for param i in 0..<x.size {
      x[i] = i;
    }

    var sum = 0;
    for i in 0..<x.size {
      sum += x[i];
    }

    a = sum;

  }

  writeln(A);
}

on here.getChild(1) {
  var A: [1..n] int;

  foreach a in A {
    var (x,y) = (1,2);
    var t = (3,4,5);

    a = x+y+t[0]+t[1]+t[2];
  }

  writeln(A);
}


