class C {
  var x: int;
}

class D : C, writeSerializable {
  var y: real;

  override proc serialize(writer, ref serializer) throws {
    var ser = serializer.startClass(writer, "D", 1);
    super.serialize(writer, ser);
    ser.writeField("y", y);
    ser.endClass();
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
use CTypes;
writeln("c1 (static type: ", c1.type:string, ", dynamic type: ", string.createCopyingBuffer(__primitive("class name by id", __primitive("getcid", c1.borrow())):c_ptrConst(c_char)), ") is: ", c1);
writeln("c2 (static type: ", c2.type:string, ", dynamic type: ", string.createCopyingBuffer(__primitive("class name by id", __primitive("getcid", c2.borrow())):c_ptrConst(c_char)), ") is: ", c2);
writeln("c3 (static type: ", c3.type:string, ", dynamic type: ", string.createCopyingBuffer(__primitive("class name by id", __primitive("getcid", c3.borrow())):c_ptrConst(c_char)), ") is: ", c3);
writeln("c4 (static type: ", c4.type:string, ", dynamic type: ", string.createCopyingBuffer(__primitive("class name by id", __primitive("getcid", c4.borrow())):c_ptrConst(c_char)), ") is: ", c4);
writeln("c5 (static type: ", c5.type:string, ", dynamic type: ", string.createCopyingBuffer(__primitive("class name by id", __primitive("getcid", c5.borrow())):c_ptrConst(c_char)), ") is: ", c5);
writeln("c6 (static type: ", c6.type:string, ", dynamic type: ", string.createCopyingBuffer(__primitive("class name by id", __primitive("getcid", c6.borrow())):c_ptrConst(c_char)), ") is: ", c6);
writeln("c7 (static type: ", c7.type:string, ", dynamic type: ", string.createCopyingBuffer(__primitive("class name by id", __primitive("getcid", c7.borrow())):c_ptrConst(c_char)), ") is: ", c7);
writeln("c8 (static type: ", c8.type:string, ", dynamic type: ", string.createCopyingBuffer(__primitive("class name by id", __primitive("getcid", c8.borrow())):c_ptrConst(c_char)), ") is: ", c8);
writeln("c9 (static type: ", c9.type:string, ", dynamic type: ", string.createCopyingBuffer(__primitive("class name by id", __primitive("getcid", c9.borrow())):c_ptrConst(c_char)), ") is: ", c9);
