use OwnedObject;

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

proc errors() {
  var x = new MyClass(1);
  var y = new MyGenericClass(1);

  writeln("in errors");
  writeln(x);
  writeln(y);

  delete x;
  delete y;
}

proc ok() {
  var a = new unmanaged MyClass(1);
  var b = new unmanaged MyGenericClass(1);
  var c = new owned MyClass(1);
  var d = new owned MyGenericClass(1);
  var e = new Owned(new MyClass(1));
  var f = new Owned(new MyGenericClass(1));
  var g = new MyRecord(1);
  var h = new MyGenericRecord(1);
 
  writeln("in ok");
  writeln(a);
  writeln(b);
  writeln(c);
  writeln(d);
  writeln(e);
  writeln(f);
  writeln(g);
  writeln(h);

  delete a;
  delete b;
}


errors();
ok();
