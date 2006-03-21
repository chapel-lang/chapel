class bar {
  type t;
  var y : t;
}

class foo {
  type t;
  var x : bar(t) = bar(t);
  fun print() {
    writeln(x.y);
  }
}

var f : foo(t=int) = foo(t = int);

f.x.y = 12;

f.print();

var f2 : foo(t=string) = foo(t = string);

f2.x.y = "hello world";

f2.print();
