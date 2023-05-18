// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
proc foo() {
  // coercing from borrowed Parent? to borrowed Child?
  var alloc = new owned Parent();
  var a:borrowed Parent? = alloc;
  var a_:borrowed Child? = a;
}
proc main() {
  foo();
}
