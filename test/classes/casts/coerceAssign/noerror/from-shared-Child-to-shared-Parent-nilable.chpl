// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
proc foo() {
  // coercing from shared Child to shared Parent?
  var allocFrom = new shared Child();
  var allocTo = new shared Parent();
  var a:shared Child = allocFrom;
  var a_:shared Parent? = allocTo;
  a_ = a;
}
proc main() {
  foo();
}
