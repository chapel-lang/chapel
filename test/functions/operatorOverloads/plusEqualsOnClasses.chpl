class C {
  var val: int;
}

proc +=(rhs: C, lhs: C) {
  rhs.val += lhs.val;
}

var c1 = new owned C(42),
    c2 = new owned C(23);

writeln(c1,c2);
c1 += c2;
writeln(c1,c2);

