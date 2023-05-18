// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
proc foo() {
  // coercing from unmanaged Parent? to unmanaged Child
  var alloc = new unmanaged Parent();
  var a:unmanaged Parent? = alloc;
  var a_:unmanaged Child = a;
}
proc main() {
  foo();
}
