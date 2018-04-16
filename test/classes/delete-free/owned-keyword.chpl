class MyClass { var x:int; }

var a = new MyClass(1);
writeln(a);
delete a;
 // OK


var b = new _owned(new MyClass(1));
writeln(b);
 // OK

var c = new owned MyClass(1);
writeln(c);
 // OK

proc makeit(type t) {
  return new t(1);
}

var d = makeit(_owned(MyClass));
writeln(d);

var e = makeit(owned MyClass);
writeln(e);
