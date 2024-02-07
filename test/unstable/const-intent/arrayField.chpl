record Foo {
  var x: [0..3]	int;
}

var arr	= [2, 4, 6, 2];
var globalFoo = new Foo(arr);

proc takesAFoo(const arg: Foo) {
  writeln(arg);
  // Won't trigger the warning
  globalFoo.x[2] = 7;
  writeln(arg);
}

proc main() {
  takesAFoo(globalFoo);
}
