class MyClass { var x; }

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

var d = makeit(_owned(MyClass(int)));
writeln(d);

var e = makeit(owned MyClass(int));
writeln(e);
