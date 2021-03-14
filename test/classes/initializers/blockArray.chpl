use BlockDist;

proc main() {
  var f = new unmanaged Foo(3, 4);

  writeln(f);

  delete f;
}

class Foo {
  var dom   : domain(2);
  var space = dom dmapped Block(dom);
  var A     : [space] int;

  proc init(n1 : int, n2 : int) {
    dom = { 1 .. n1, 1 .. n2 };

  }
}
