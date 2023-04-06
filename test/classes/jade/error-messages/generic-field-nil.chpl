config param explicitNilable = false;
if explicitNilable {
  class A {
    var x: A? = nil;
  }
  var a: owned A = new A();
}
else {
  class A {
    var x: A = nil;
  }
  var a: owned A = new A();
}
