// Should fail compile time nil check
config const neverHappen = true;
class MyClass {
  var x : int;
  proc method() {
    writeln("in method");
  }
}
if !neverHappen {
  writeln("For our test runs, this code should not be reached during runtime");
  var x:borrowed MyClass?;
  x!.method();
}
