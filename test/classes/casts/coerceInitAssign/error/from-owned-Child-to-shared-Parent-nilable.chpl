// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
proc foo() {
  // coercing from owned Child to shared Parent?
  var a = new owned Child();
  var a_:shared Parent? = a;
}
proc main() {
  foo();
}
