config param compOption = true;
config const option = true;

proc returnsArrayType(arg) type {
  return arg.type;
}

proc makeArray() {
  writeln("in makeArray()");
  var A:[1..10] int;
  return A;
}

proc test1() {
  writeln("test1");
  type t = returnsArrayType(makeArray());
}
test1();

proc test2() {
  writeln("test2");
  type t;
  t = returnsArrayType(makeArray());
}
test2();

proc test3() {
  writeln("test3");
  type t;
  {
    t = returnsArrayType(makeArray());
  }
}
test3();

proc test4() {
  writeln("test4");
  type t;
  if compOption {
    t = returnsArrayType(makeArray());
  } else {
    t = returnsArrayType(makeArray());
  }
}
test4();

proc test5() {
  writeln("test5");
  type t;
  if option {
    t = returnsArrayType(makeArray());
  } else {
    t = returnsArrayType(makeArray());
  }
}
test5();
