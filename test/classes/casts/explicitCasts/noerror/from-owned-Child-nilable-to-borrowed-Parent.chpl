// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
proc foo() {
  // casting from owned Child? to borrowed Parent
  var alloc = new owned Child();
  var a:owned Child? = alloc;
  var a_ = a:borrowed Parent;
}
proc main() {
  foo();
}
