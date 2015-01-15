use driver_domains;

proc main() {
  const d1d => Dom1D;
  const d2d => Dom2D;
  const d3d => Dom3D;
  const d4d => Dom4D;
  const dd32 => Dom2D32;

  var v1d = d1d;
  var v2d = d2d;
  var v3d = d3d;
  var v4d = d4d;
  var vv32 = dd32;

  var r1d => v1d;
  var r2d => v2d;
  var r3d => v3d;
  var r4d => v4d;
  var rr32 => vv32;

  writeln();

  writeln(Dom1D);
  writeln(d1d);
  writeln(v1d);
  writeln(r1d);
  writeln();

  writeln(Dom2D);
  writeln(d2d);
  writeln(v2d);
  writeln(r2d);
  writeln();

  writeln(Dom3D);
  writeln(d3d);
  writeln(v3d);
  writeln(r3d);
  writeln();

  writeln(Dom4D);
  writeln(d4d);
  writeln(v4d);
  writeln(r4d);
  writeln();

  writeln(Dom2D32);
  writeln(dd32);
  writeln(vv32);
  writeln(rr32);
  writeln();

  r1d = 2..50;
  r2d = (11..22, 33..44);
  r3d = (11..11, 22..22, 33..33);
  r4d = (1..2, 3..4, 5..6, 7..8);
  rr32 = (0..0:int(32), min(int(32))..max(int(32)));

  writeln(v1d);
  writeln(v2d);
  writeln(v3d);
  writeln(v4d);
  writeln(vv32);
  writeln();
}
