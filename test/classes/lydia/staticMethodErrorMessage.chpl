// Tests error output when failing to call a static method
class Foo {
  proc type someMethod(x: int) {
    writeln("in someMethod(int)");
  }
}
proc type Foo.someMethod(x: string) {
  writeln("in someMethod(string)");
}

Foo.someMethod();
