// modified from https://github.com/chapel-lang/chapel/issues/11421
class C {
  var dom = {1..4};
  var arr: [dom] real = 0.0;
}

var data: [1..2] real;

operator +(c1: owned C , c2: owned C) {
  return c1;
}

var c = new owned C();
writeln(c.arr);
