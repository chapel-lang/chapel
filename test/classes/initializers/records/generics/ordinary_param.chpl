record Foo {
  param a: bool;

  proc init(param a) {
    this.a = a;
  }
}

var f1 = new Foo(true);
var f2 = new Foo(false);
var f3: Foo(true);
var f4: Foo(false);
writeln(f1.type:string);
writeln(f2.type:string);
writeln(f3.type:string);
writeln(f4.type:string);
