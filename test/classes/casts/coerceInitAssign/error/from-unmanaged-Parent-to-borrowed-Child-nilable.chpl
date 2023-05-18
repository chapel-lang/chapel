// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
proc foo() {
  // coercing from unmanaged Parent to borrowed Child?
  var alloc = new unmanaged Parent();
  var a:unmanaged Parent = alloc;
  var a_:borrowed Child? = a;
}
proc main() {
  foo();
}
