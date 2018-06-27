var (a,b,(c,d)) = (1,2,(3,4));

writeln((a, b, c, d));

record R {
  var x: (int, int, (int, int)) = (1, 2, (3, 4));
}

record R2 {
  var (a,b,(c,d)) = (1,2,(3,4));
}

var myR: R;
writeln(myR);

var myR2: R2;
writeln(myR2);
