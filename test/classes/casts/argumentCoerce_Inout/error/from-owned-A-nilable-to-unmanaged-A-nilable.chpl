// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
// coercing from owned A? to unmanaged A?
proc bar(inout x: unmanaged A?) {
  x = new unmanaged A?();
}
proc foo() {
  var a = new owned A?();
  bar(a);
}
proc main() {
  foo();
}
