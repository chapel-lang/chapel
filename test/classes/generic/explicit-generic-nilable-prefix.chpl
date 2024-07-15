class C { var x; }

proc f(ref arg: C?(?)) {
  writeln(arg.type:string);
}

var x:owned C? = new owned C(1);
var b:C? = x.borrow();
f(x);
f(b);
