class Foo {
  var x: int;

  proc init(val) {
    badCall(arg=this, val);
    super.init();
  }
}

proc badCall(arg: borrowed Foo, val) {
  arg.x = val;
}

var foo = new unmanaged Foo(3);
writeln(foo);
delete foo;
