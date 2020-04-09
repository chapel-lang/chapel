// 2017/12/24
//
// Confirm that _new() is constructed for a generic class whether
//
//   1) the init/_new pair is constructed together
//      i.e. while resolving PRIM_NEW for MyClass1
//
//   2) init is generated implicitly before the appropriate PRIM_NEW
//

class MyClass0 {
  var a0;
  var b0;

  proc init(a, b) {
    a0 = a;
    b0 = b;

  }
}

class MyClass1 : MyClass0 {
  var a1;
  var b1;

  proc init(a0, b0, a, b) {
    super.init(a0, b0);

    a1 = a;
    b1 = b;
  }
}

proc main() {
  var c0 = new unmanaged MyClass1(1, 2, 3, 4);
  var c1 = new unmanaged MyClass0(1, 2);

  writeln('c0 : ', c0);
  writeln('c1 : ', c1);

  delete c1;
  delete c0;
}

