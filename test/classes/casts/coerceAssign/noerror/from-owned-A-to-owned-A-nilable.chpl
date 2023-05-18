// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
proc foo() {
  // coercing from owned A to owned A?
  var allocFrom = new owned A();
  var allocTo = new owned A();
  var a:owned A = allocFrom;
  var a_:owned A? = allocTo;
  a_ = a;
}
proc main() {
  foo();
}
