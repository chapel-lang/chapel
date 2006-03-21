class C {
  var x : int;
}

var c = C(x = 2);

fun foo(type t) {
  var x : t;
  fun bar(c : C) {
    writeln(c);
  }
  writeln(x);
}

foo(int);
foo(float);
bar(c);
