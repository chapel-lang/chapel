// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
// coercing from borrowed Parent to shared Child?
proc bar(inout x: shared Child?) {
  x = new shared Child?();
}
proc foo() {
  var alloc = new owned Parent();
  var a:borrowed Parent = alloc;
  bar(a);
}
proc main() {
  foo();
}
