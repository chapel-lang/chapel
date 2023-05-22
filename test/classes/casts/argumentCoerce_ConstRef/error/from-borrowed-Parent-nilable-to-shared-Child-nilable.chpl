// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
// coercing from borrowed Parent? to shared Child?
proc bar(const ref x: shared Child?) {}
proc foo() {
  var alloc = new owned Parent?();
  var a:borrowed Parent? = alloc;
  bar(a);
}
proc main() {
  foo();
}
