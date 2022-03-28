class MyClass { var x: int; }
var x: single unmanaged MyClass;
x.writeEF(new unmanaged MyClass(1));
var val = x.readFF();
delete val;
