// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
proc foo() {
  // coercing from unmanaged Child to shared Parent
  var a = new unmanaged Child();
  var a_:shared Parent = a;
}
proc main() {
  foo();
}
