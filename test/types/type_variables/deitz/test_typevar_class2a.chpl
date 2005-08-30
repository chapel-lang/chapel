class foo {
  type t;
  var x : t;
  function print() {
    writeln(x);
  }
}

var f = foo(integer);

var f2 = foo(string);

f.x = 2;

f2.x = "hello world";

f.print();

f2.print();
