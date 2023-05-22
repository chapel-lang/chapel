// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
// coercing from owned Parent? to borrowed Child
proc bar(ref x: borrowed Child) {}
proc foo() {
  var a = new owned Parent?();
  bar(a);
}
proc main() {
  foo();
}
