class C {
  var n : int = 4;
  var d : domain(2) = [1..n, 1..n];
  var a : [d] int;
}

var c = C(n = 8);
writeln(c);
