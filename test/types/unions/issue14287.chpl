record R {
  var x: int;
  proc init(xx: int = 0) { writeln("R(",xx,").init"); x = xx;}
  proc deinit() { writeln("R(",x,").deinit"); }
}

union U {
  var r1: R;
  var r2: R;
}

var u, v: U;
writeln("declared u and v");
writeln((u,v));
writeln();

u.r1 = new R(1);
writeln("set u.r1");
writeln((u,v));
writeln();

v.r1 = u.r1;
writeln("set v.r1 from u.r1");
writeln((u,v));
writeln();
  
u.r2 = v.r1;
writeln("set u.r2 from v.r1");
writeln((u,v));
writeln();

v.r2 = u.r2;
writeln("set v.r2 from u.r2");
writeln((u,v));
writeln();

u.r2 = u.r2;
writeln("set u.r2 from u.r2");
writeln((u,v));
writeln();

u.r1 = u.r2;
writeln("set u.r1 from u.r2");
writeln((u,v));
writeln();
