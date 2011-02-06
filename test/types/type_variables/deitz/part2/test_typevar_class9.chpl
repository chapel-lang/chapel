class bar {
  type t;
  var y : t;
}

class foo {
  type t;
  var x : bar(t) = new bar(t);
  proc print() {
    writeln(x.y);
  }
}

var f : foo(t=int) = new foo(t = int);

f.x.y = 12;

f.print();

var f2 : foo(t=string) = new foo(t = string);

f2.x.y = "hello world";

f2.print();
