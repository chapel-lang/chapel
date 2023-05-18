// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
proc foo() {
  // coercing from shared Parent to unmanaged Child
  var allocFrom = new shared Parent();
  var allocTo = new unmanaged Child();
  var a:shared Parent = allocFrom;
  var a_:unmanaged Child = allocTo;
  a_ = a;
}
proc main() {
  foo();
}
