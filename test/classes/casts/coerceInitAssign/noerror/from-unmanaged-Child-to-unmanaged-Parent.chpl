// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
proc foo() {
  // coercing from unmanaged Child to unmanaged Parent
  var a = new unmanaged Child();
  var a_:unmanaged Parent = a;
}
proc main() {
  foo();
}
