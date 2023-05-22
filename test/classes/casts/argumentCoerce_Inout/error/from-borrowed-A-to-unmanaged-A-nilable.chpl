// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
// coercing from borrowed A to unmanaged A?
proc bar(inout x: unmanaged A?) {
  x = new unmanaged A?();
}
proc foo() {
  var alloc = new owned A();
  var a:borrowed A = alloc;
  bar(a);
}
proc main() {
  foo();
}
