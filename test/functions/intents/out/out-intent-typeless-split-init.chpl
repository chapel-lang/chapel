
proc foo(out a) {
  a = 1; // causes a to have inferred type int
  writeln("foo(out a) with a.type ", a.type:string);
}

proc main() {
  var x;
  foo(x); // split-init setting x's type to int
  writeln("after call, x is ", x, " : ", x.type:string);
}
