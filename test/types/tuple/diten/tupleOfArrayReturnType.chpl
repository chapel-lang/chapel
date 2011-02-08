type t = [1..3] int;

proc foo(): (t, t) {
  var a:[1..3] int = [i in 1..3] i;
  var b:[1..3] int = [i in 4..6] i;
  return (a,b);
}

writeln(foo());
