class C {
  var x : int;
}
var cObj = new C(x = 2);
var c = cObj.borrow();

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
