class C {
  var x : int;
}

var c = C(x = 2);

function foo(type t) {
  var x : t;
  function bar(c : C) {
    writeln(c);
  }
  writeln(x);
}

foo(int);
foo(float);
bar(c);
