// Checking for the error cases of a default-initialized array
// with a non-nilable element type...

// ... a variable declaration
var x1: [1..3] owned RootClass;
var x2: [1..3] shared RootClass;
var x3: [1..3] borrowed RootClass;
var x4: [1..3] unmanaged RootClass;

// ... a field; compiler-generated initializer
class C1 {
  var A1: [1..3] borrowed RootClass;
}

var c1 = new C1();

// ... a field; user-defined initializer
class C2 {
  var A2: [1..3] borrowed RootClass;
  proc init() {
    // A2 is default-initialized here
    init this;
  }
}

var c2 = new C2();

/// adding arrays of arrays

// ... a variable declaration
var y1: [1..3] [1..2] owned RootClass;
var y2: [1..3] [1..2] shared RootClass;
var y3: [1..3] [1..2] borrowed RootClass;
var y4: [1..3] [1..2] unmanaged RootClass;

// ... a field; compiler-generated initializer
class D1 {
  var B1: [1..3] [1..2] borrowed RootClass;
}

var d1 = new D1();

// ... a field; user-defined initializer
class D2 {
  var B2: [1..3] [1..2] borrowed RootClass;
  proc init() {
    // B2 is default-initialized here
    init this;
  }
}

var d2 = new D2();
