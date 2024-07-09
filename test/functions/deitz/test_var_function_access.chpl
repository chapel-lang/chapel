class foo {
  var a : int;
}

proc bar(x : borrowed foo) ref {
  return x;
}

var fObj = new foo(a = 12);
var f : borrowed foo = fObj.borrow();

writeln(f);

var i : int;

writeln(i);

i = bar(f).a;

writeln(i);

bar(f).a = 2;

writeln(f);
