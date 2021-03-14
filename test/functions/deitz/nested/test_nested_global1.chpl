class C {
  var x : int;
}

var c = new borrowed C(x = 2);

proc foo(type t) {
  var x : t;
  proc bar(c : borrowed C) {
    writeln(c);
  }
  writeln(x);
}

foo(int);
foo(real);
bar(c);
