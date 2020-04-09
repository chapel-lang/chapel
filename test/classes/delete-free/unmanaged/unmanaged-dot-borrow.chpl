class MyClass {
  var x:int;
}

var ptr = new unmanaged MyClass();
var b = ptr.borrow();
var bb = b.borrow();
writeln(ptr.type:string, " ", ptr);
writeln(b.type:string, " ", b);
writeln(bb.type:string, " ", bb);

delete ptr;
