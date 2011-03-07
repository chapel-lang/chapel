class foo {
  type t1;
  type t2;
  var x : t1;
  var y : t2;
  proc print() {
    writeln(x, " and ", y);
  }
}

var f : foo(t1=int,t2=string) = new foo(t1 = int, t2 = string);

var f2 : foo(t1=string,t2=int) = new foo(t1 = string, t2 = int, x = "hello", y = 12);

f.x = 14;
f.y = "world";

f.print();
f2.print();
