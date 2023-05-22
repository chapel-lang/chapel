// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
// coercing from borrowed Child to borrowed Parent?
proc bar(const ref x: borrowed Parent?) {}
proc foo() {
  var alloc = new owned Child();
  var a:borrowed Child = alloc;
  bar(a);
}
proc main() {
  foo();
}
