proc foo(const ref x: bool(?)) {
  writeln("In ", x.type:string, " version of foo()");
}

proc bar(const ref x: bool(?w)) {
  writeln("In bool(", w, ") version of bar()");
}

var b: bool;
var b8: bool(8);
var b16: bool(16);
var b32: bool(32);
var b64: bool(64);

foo(b);
foo(b8);
foo(b16);
foo(b32);
foo(b64);

//bar(true);
//bar(b);
bar(b8);
bar(b16);
bar(b32);
bar(b64);
