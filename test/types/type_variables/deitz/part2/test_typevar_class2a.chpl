class foo {
  type t;
  var x : t;
  proc print() {
    writeln(x);
  }
}

var f = new foo(int);

var f2 = new foo(string);

f.x = 2;

f2.x = "hello world";

f.print();

f2.print();
