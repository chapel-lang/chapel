// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
proc foo() {
  // coercing from borrowed Child to unmanaged Parent
  var allocFrom = new owned Child();
  var allocTo = new unmanaged Parent();
  var a:borrowed Child = allocFrom;
  var a_:unmanaged Parent = allocTo;
  a_ = a;
}
proc main() {
  foo();
}
