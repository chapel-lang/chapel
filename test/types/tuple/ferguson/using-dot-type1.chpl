proc foo(a, b) {
  var tup = (a,b);
  type t = tup.type;
  var x:t;
  return x;
}

proc run() {
  var a = (1,2);
  var b = ("hi ", "there");
  var x = foo( a, b );
  writeln(x);
}

run();
