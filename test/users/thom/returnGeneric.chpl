// a generic class

class C {
  var n;
}

proc foo(): unmanaged C {
  return new unmanaged C(3);
}

var fooo = foo();
writeln(fooo);
delete fooo;

// a subclass

class D: C {
  var m;
}

proc bar(): unmanaged C {
  return new unmanaged D(4,5.5);
}

var baar = bar();
writeln(baar);
delete baar;

// TODO futher variations:
// * foo() and bar() variants that return an owned/shared/borrowed
// * multiple return statements: one returning a C, another a D
// * iter+yield instead of proc+return
// TODO combine with the following test when it works:
//   ./returnGenericSubclass.chpl
// See also:
//   #10172, #11456
//   test/functions/lydia/declaredGenericReturnType.chpl
//   ./yieldGeneric.chpl
