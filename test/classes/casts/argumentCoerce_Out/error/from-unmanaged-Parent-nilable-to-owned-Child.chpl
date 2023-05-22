// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
// coercing from unmanaged Parent? to owned Child
proc bar(out x: owned Child) {
  x = new owned Child();
}
proc foo() {
  var a = new unmanaged Parent?();
  bar(a);
}
proc main() {
  foo();
}
