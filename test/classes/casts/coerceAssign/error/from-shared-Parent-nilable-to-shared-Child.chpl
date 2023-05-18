// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
proc foo() {
  // coercing from shared Parent? to shared Child
  var allocFrom = new shared Parent();
  var allocTo = new shared Child();
  var a:shared Parent? = allocFrom;
  var a_:shared Child = allocTo;
  a_ = a;
}
proc main() {
  foo();
}
