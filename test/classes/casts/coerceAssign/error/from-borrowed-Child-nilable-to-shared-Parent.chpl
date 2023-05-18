// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
proc foo() {
  // coercing from borrowed Child? to shared Parent
  var allocFrom = new owned Child?();
  var a:borrowed Child? = allocFrom;
  var a_ = new shared Parent();
  a_ = a;
}
proc main() {
  foo();
}
