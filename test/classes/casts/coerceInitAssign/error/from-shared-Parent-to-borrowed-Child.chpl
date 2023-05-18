// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
proc foo() {
  // coercing from shared Parent to borrowed Child
  var alloc = new shared Parent();
  var a:shared Parent = alloc;
  var a_:borrowed Child = a;
}
proc main() {
  foo();
}
