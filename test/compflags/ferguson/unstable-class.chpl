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
  var x: MyClass;
  var y: MyGenericClass(int);
  errorsInArgs(x, y, y);
}

proc ok() {
  var a:MyRecord;
  var b:MyGenericRecord(int);
  var c:borrowed MyClass;
  var d:unmanaged MyClass;
  var e:owned MyClass;
  var f:shared MyClass;
  var g:Owned(MyClass);
  var h:Shared(MyClass);

  var cg:borrowed MyGenericClass(int);
  var dg:unmanaged MyGenericClass(int);
  var eg:owned MyGenericClass(int);
  var fg:shared MyGenericClass(int);
  var gg:Owned(MyGenericClass(int));
  var hg:Shared(MyGenericClass(int));

  extern proc printf(fmt:c_string, arg:MyClass);
  if debug then printf("%p\n", d);
}


errors();
ok();
