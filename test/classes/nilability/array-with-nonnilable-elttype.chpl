// Checking for the error cases of a default-initialized array
// with a non-nilable element type...

// ... a variable declaration
var x1: [1..3] owned object;
var x2: [1..3] shared object;
var x3: [1..3] borrowed object;
var x4: [1..3] unmanaged object;

// ... a field; compiler-generated initializer
class C1 {
  var A1: [1..3] borrowed object;
}

var c1 = new C1();

// ... a field; user-defined initializer
class C2 {
  var A2: [1..3] borrowed object;
  proc init() {
    // A2 is default-initialized here
    this.complete();
  }
}

var c2 = new C2();
