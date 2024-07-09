class MyClass {
  var val: int;
}
type myOwnedType = owned MyClass;

// will also fail with `:shared`, `:owned`, or `:MyClass` since all are generic
proc bar(x: myOwnedType:unmanaged) {
  writeln(x.type:string);
  return x.val;
}

var x = new unmanaged MyClass(17);
writeln(bar(x));
