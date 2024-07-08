class MyClass {
  var val: int;
}
type myOwnedType = owned MyClass;
type myUnmanagedType = unmanaged MyClass;

proc bar(x: myOwnedType:myUnmanagedType) {
  writeln(x.type:string);
  return x.val;
}

var x = new MyClass(17);
writeln(bar(x));
