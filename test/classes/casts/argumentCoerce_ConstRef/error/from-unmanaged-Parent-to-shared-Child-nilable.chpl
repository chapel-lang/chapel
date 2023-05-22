// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
// coercing from unmanaged Parent to shared Child?
proc bar(const ref x: shared Child?) {}
proc foo() {
  var a = new unmanaged Parent();
  bar(a);
}
proc main() {
  foo();
}
