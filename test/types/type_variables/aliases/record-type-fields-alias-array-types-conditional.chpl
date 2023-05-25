record R1 {
  type t1 = [1..2] int;
  type t2 = [1..2] real;

  var field: if true then t1 else t2;
}

var x : R1;
writeln(x);
