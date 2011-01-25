proc foo(x) {
  var i : int = 2;
  i = i * 6;
  writeln(i);
  writeln(x);
}

var i : int = 2;

foo(i);

var f : real = 2.3;

foo(f);

var s : string = "sugar";

foo(s);
