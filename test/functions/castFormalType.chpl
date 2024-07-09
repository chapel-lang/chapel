class MyClass {
  var val: int;
}
type myOwnedType = owned MyClass;
type myBorrowedType = myOwnedType:borrowed;
proc toBorrow(type x) type do return x:borrowed;

// all of these should result in a borrowed type
proc foo(a: toBorrow(myOwnedType),
         b: myBorrowedType,
         c: myOwnedType:borrowed,
         d: myOwnedType:borrowed MyClass): myOwnedType {
  writeln((a, b, c, d));
  writeln((a.type:string, b.type:string, c.type:string, d.type:string));
  return new myOwnedType(a.val+b.val+c.val+d.val);
}

var a = new myOwnedType(1);
var b = new myOwnedType(2);
var c = new myOwnedType(3);
var d = new myOwnedType(4);
var res = foo(a, b, c, d);
writeln(res);
