config const debug = false;

class MyClass {
  var x:int;
}

class MyGenericClass {
  var y;
}

record MyRecord {
  var x:int;
}

record MyGenericRecord {
  var y;
}

class ClassWithError {
  var next: MyClass;
}

proc errorsInArgs( x: MyClass, y: MyGenericClass, z: MyGenericClass(int) ) {
}

proc errors() {
  var x: MyClass = new MyClass(1);
  var y: MyGenericClass(int);
  var a: [1..10] MyClass;
  var b: [1..10] MyGenericClass(int);
  var c: MyGenericRecord(MyClass);
  errorsInArgs(x, y, y);
}

proc okTypeMethod(type t) {
}

proc ok() {
  var a:MyRecord;
  var b:MyGenericRecord(int);
  var c:borrowed MyClass;
  var d:unmanaged MyClass;
  var e:owned MyClass;
  var f:shared MyClass;

  var cg:borrowed MyGenericClass(int);
  var dg:unmanaged MyGenericClass(int);
  var eg:owned MyGenericClass(int);
  var fg:shared MyGenericClass(int);

  extern proc printf(fmt:c_string, arg:MyClass);
  if debug then printf("%p\n", d);

  okTypeMethod(unmanaged MyClass);
  okTypeMethod(MyClass); // intentionally allowed

  type okType = MyClass; // intentionally allowed
}


errors();
ok();
