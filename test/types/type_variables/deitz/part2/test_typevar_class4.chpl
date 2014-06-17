class foo {
  type t;
  var x : t;
  proc print() {
    writeln(x);
  }
}

var f : foo(t=int) = new foo(t=int);

var f2 : foo(t=string) = new foo(t=string, x = "hello world");

f.x = 2;

f.print();

f2.print();
