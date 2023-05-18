// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
proc foo() {
  // coercing from unmanaged Parent to shared Child?
  var allocFrom = new unmanaged Parent();
  var allocTo = new shared Child();
  var a:unmanaged Parent = allocFrom;
  var a_:shared Child? = allocTo;
  a_ = a;
}
proc main() {
  foo();
}
