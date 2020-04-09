
record R {
  type  A;
  param B;

  var foo : string;

  param C = 3;

  var x : C*int;
}

var r : R(int, false);

writeln(r);
writeln(r.type:string);
