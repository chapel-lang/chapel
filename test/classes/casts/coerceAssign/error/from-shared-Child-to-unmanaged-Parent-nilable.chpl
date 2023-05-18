// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
proc foo() {
  // coercing from shared Child to unmanaged Parent?
  var allocFrom = new shared Child();
  var allocTo = new unmanaged Parent();
  var a:shared Child = allocFrom;
  var a_:unmanaged Parent? = allocTo;
  a_ = a;
}
proc main() {
  foo();
}
