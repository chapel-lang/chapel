class C {
  var x : integer;
}

var c = C(x = 2);

function foo(type t) {
  var x : t;
  function bar(c : C) {
    writeln(c);
  }
  writeln(x);
}

foo(integer);
foo(float);
bar(c);
