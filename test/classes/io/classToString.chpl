class C {
  var x: int;
}

class D : C {
  var y: real;
  proc writeThis(ch) throws {
    super.writeThis(ch);
    ch.write("y is: ", y);
  }
}

class E : D {
  var z: bool;
}

// static type C
var c1: C = new C();
var c2: C = new D();
var c3: C = new E();

// static type D
var c4: D = new D();
var c5: D = new E();

// static type E
var c6: E = new E();

// inferred type
var c7 = new C();
var c8 = new D();
var c9 = new E();

var c10: C = new shared E();
var c11: C = new unmanaged E();
var c12: C = c3.borrow();

for mgmt in false..true {
  writeln(dynamicTypeAsString(c1, mgmt));
  writeln(dynamicTypeAsString(c2, mgmt));
  writeln(dynamicTypeAsString(c3, mgmt));
  writeln(dynamicTypeAsString(c4, mgmt));
  writeln(dynamicTypeAsString(c5, mgmt));
  writeln(dynamicTypeAsString(c6, mgmt));
  writeln(dynamicTypeAsString(c7, mgmt));
  writeln(dynamicTypeAsString(c8, mgmt));
  writeln(dynamicTypeAsString(c9, mgmt));
  writeln(dynamicTypeAsString(c10, mgmt));
  writeln(dynamicTypeAsString(c11, mgmt));
  writeln(dynamicTypeAsString(c12, mgmt));
}

delete c11;

proc dynamicTypeAsString(expr, printClassMgmt = true) {
  var ret: string;
  
  if (isClass(expr)) {
    if (printClassMgmt) {
      if (isOwnedClass(expr)) {
        ret = "owned ";
      } else if (isBorrowedClass(expr)) {
        ret = "borrowed ";
      } else if (isSharedClass(expr)) {
        ret = "shared ";
      } else if (isUnmanagedClass(expr)) {
        ret = "unmanaged ";
      } else {
        halt("Unanticipated class management type in dynamicTypeAsString()");
      }
    }
    ret += __primitive("class name by id",
                       __primitive("getcid", expr.borrow())
                      ):string;
    return ret;
  } else {
    compilerError("dynamicTypeAsString() isn't currently implemented for non-classes");
  }
}
