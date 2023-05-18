// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
proc foo() {
  // coercing from owned Parent to unmanaged Child
  var allocFrom = new owned Parent();
  var allocTo = new unmanaged Child();
  var a:owned Parent = allocFrom;
  var a_:unmanaged Child = allocTo;
  a_ = a;
}
proc main() {
  foo();
}
