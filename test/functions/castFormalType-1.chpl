class MyClass {
  var val: int;
}
type myOwnedType = owned MyClass;
proc getType() type do return myOwnedType;

proc bar(x: getType():borrowed) {
  writeln(x.type:string);
  return x.val;
}

var x = new MyClass(17);
writeln(bar(x));
