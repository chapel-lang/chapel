class MyClass {
  var x:int;
}

var a:unmanaged = new unmanaged MyClass(1);
writeln(a);
delete a;
