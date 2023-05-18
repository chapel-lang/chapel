// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
proc foo() {
  // coercing from owned Parent? to owned Child?
  var allocFrom = new owned Parent();
  var allocTo = new owned Child();
  var a:owned Parent? = allocFrom;
  var a_:owned Child? = allocTo;
  a_ = a;
}
proc main() {
  foo();
}
