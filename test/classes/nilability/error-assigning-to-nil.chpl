class MyClass { var x: int; }

proc main() {
  var a: unmanaged MyClass = new unmanaged MyClass(1);
  delete a;
  a = nil;
}
