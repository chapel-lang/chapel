// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
proc foo() {
  // coercing from owned Child to shared Parent?
  var allocFrom = new owned Child();
  var allocTo = new shared Parent();
  var a:owned Child = allocFrom;
  var a_:shared Parent? = allocTo;
  a_ = a;
}
proc main() {
  foo();
}
