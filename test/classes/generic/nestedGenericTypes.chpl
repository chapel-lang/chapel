class MyGenericClass { var x; }
record Wrapper { var impl; }
var x: Wrapper(unmanaged MyGenericClass)
  = new Wrapper(new unmanaged MyGenericClass(1));
writeln(x);
delete x.impl;

record MyGenericRecord { var x; }
var y: Wrapper(MyGenericRecord)
  = new Wrapper(new MyGenericRecord(2));
writeln(y);
