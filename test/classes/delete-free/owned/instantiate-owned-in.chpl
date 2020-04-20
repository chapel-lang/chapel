class C {}
var x = new owned C?();
foo(x);

proc foo(in y) {
  writeln(y.type:string);
}
