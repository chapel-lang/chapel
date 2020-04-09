class Foo {
  var x: int;

  proc init(val) {
    this.complete();
    badCall(this, val=val);
  }
}

proc badCall(arg: borrowed Foo, val) {
  arg.x = val;
}

var foo = new unmanaged Foo(3);
writeln(foo);
delete foo;
