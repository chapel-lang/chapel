// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
// coercing from borrowed Child? to owned Parent
proc bar(const x: owned Parent) {}
proc foo() {
  var alloc = new owned Child?();
  var a:borrowed Child? = alloc;
  bar(a);
}
proc main() {
  foo();
}
