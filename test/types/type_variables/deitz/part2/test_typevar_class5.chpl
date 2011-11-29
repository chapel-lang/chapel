class foo {
  type t;
  var x : t;
  var y : t;
  proc print() {
    writeln(x, " and ", y);
  }
}

var f : foo(t=int) = new foo(t = int);

var f2 : foo(t=string) = new foo(t = string, x = "hello", y = "world");

f.x = 12;
f.y = 14;

f.print();
f2.print();
