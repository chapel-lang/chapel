record R {
  proc init() {
    writeln("In R.init");
  }
}

record R2 {
  var x: R;
}

union U {
  var x: R;
  var y: int;
}

writeln("Creating r");
var r = new R();
writeln("---");

writeln("creating r2");
var r2: R2;
writeln("assigning r2");
r2.x = new R();
writeln("---");

writeln("creating u");
var u: U;
writeln("assigning u.x");
u.x = new R();
writeln("re-assigning u.x");
u.x = new R();
writeln("re-re-assigning u.x");
u.x = new R();
writeln("assigning u.y");
u.y = 42;
writeln("re-re-re-assigning u.x");
u.x = new R();
writeln("---");

