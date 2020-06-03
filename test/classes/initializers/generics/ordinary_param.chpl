class Foo {
  param a: bool;

  proc init(param a) {
    this.a = a;
  }
}

var f1 = new owned Foo(true);
var f2 = new owned Foo(false);
var f3: borrowed Foo(true)?;
var f4: borrowed Foo(false)?;
writeln(f1.type:string);
writeln(f2.type:string);
writeln(f3.type:string);
writeln(f4.type:string);
