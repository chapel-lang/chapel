class C {
  var n : integer = 4;
  var d : domain(2) = [1..n, 1..n];
  var a : [d] integer;
}

var c = C(n = 8);
writeln(c);
