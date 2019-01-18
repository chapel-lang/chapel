class Foo {
  var x: int;

  proc init(xVal) {
    x = xVal;
    this.complete();
    select xVal.type {
      when int(16) do
        writeln("It was small");
      when int(32) do
        writeln("It was smaller than the default");
      when int(8) do
        writeln("It was tiny!");
      otherwise do
        writeln("It was normal");
      }
  }
}

var foo = new owned Foo(4:int(16));
writeln(foo);
