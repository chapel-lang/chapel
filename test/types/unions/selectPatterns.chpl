union U {
  var x: int;
  var y: real;
  var z: string;
  var w: int;
}
var u: U;

proc doVisit() do
  u.visit(
    proc(x: int) { writeln("x: ", x); },
    proc(y: real) { writeln("y: ", y); },
    proc(z: string) { writeln("z: ", z); },
    proc(w: int) { writeln("w: ", w); }
  );
proc doVisitOne() {
  record F {}
  proc F.this(x) do writeln("visited: ", x);
  u.visitOne(new F());
}

proc doBasicSelect() throws do
  select u.getActiveIndex() {
    when 0 do writeln("x: ", u.x);
    when 1 do writeln("y: ", u.y);
    when 2 do writeln("z: ", u.z);
    when 3 do writeln("w: ", u.w);
    otherwise
      throw new Error("union is not currently active");
  }
proc doFancySelect() throws do
  select u.getActiveIndex() {
    when U.getFieldIndex("x") do writeln("x: ", u.x);
    when U.getFieldIndex("y") do writeln("y: ", u.y);
    when U.getFieldIndex("z") do writeln("z: ", u.z);
    when U.getFieldIndex("w") do writeln("w: ", u.w);
    otherwise
      throw new Error("union is not currently active");
  }
proc doSuperFancySelect() throws do
  select u {
    when U.x do writeln("x: ", u.x);
    when U.y do writeln("y: ", u.y);
    when U.z do writeln("z: ", u.z);
    when U.w do writeln("w: ", u.w);
    otherwise
      throw new Error("union is not currently active");
  }

writeln(u.getActiveIndex());
// doVisit(); // will halt
try {
  doBasicSelect();
} catch e {
  writeln("caught error: ", e.message());
}
try {
  doFancySelect();
} catch e {
  writeln("caught error: ", e.message());
}
try {
  doSuperFancySelect();
} catch e {
  writeln("caught error: ", e.message());
}

writeln("test getActiveIndex");
u.x = 10;
writeln(u.getActiveIndex());
u.y = 3.14;
writeln(u.getActiveIndex());
u.z = "hello";
writeln(u.getActiveIndex());
u.w = 20;
writeln(u.getActiveIndex());

writeln("test visit");
u.x = 10;
doVisit();
u.y = 3.14;
doVisit();
u.z = "hello";
doVisit();
u.w = 20;
doVisit();

writeln("test visitOne");
u.x = 10;
doVisitOne();
u.y = 3.14;
doVisitOne();
u.z = "hello";
doVisitOne();
u.w = 20;
doVisitOne();

writeln("test select");
u.x = 10;
doBasicSelect();
u.y = 3.14;
doBasicSelect();
u.z = "hello";
doBasicSelect();
u.w = 20;
doBasicSelect();

writeln("test fancy select");
u.x = 10;
doFancySelect();
u.y = 3.14;
doFancySelect();
u.z = "hello";
doFancySelect();
u.w = 20;
doFancySelect();

writeln("test super fancy select");
u.x = 10;
doSuperFancySelect();
u.y = 3.14;
doSuperFancySelect();
u.z = "hello";
doSuperFancySelect();
u.w = 20;
doSuperFancySelect();
