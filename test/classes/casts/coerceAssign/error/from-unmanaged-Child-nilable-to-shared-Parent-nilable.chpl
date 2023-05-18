// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
proc foo() {
  // coercing from unmanaged Child? to shared Parent?
  var allocFrom = new unmanaged Child();
  var allocTo = new shared Parent();
  var a:unmanaged Child? = allocFrom;
  var a_:shared Parent? = allocTo;
  a_ = a;
}
proc main() {
  foo();
}
