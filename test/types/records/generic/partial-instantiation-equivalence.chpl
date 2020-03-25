record R {
  param x, y;
}

type Rx = R(x=1);
type Ry = R(y=1);

writeln("Rx == Rx: ", Rx == Rx);
writeln("Rx == Ry: ", Rx == Ry);

type Rxy = Rx(y=1);
type Ryx = Ry(x=1);

writeln("Rxy, Ryx: ", Rxy:string, " ", Ryx:string);

writeln("Rxy == R(1,1): ", Rxy == R(1,1));
writeln("Ryx == R(1,1): ", Ryx == R(1,1));
writeln("Rxy == Ryx: ", Rxy == Ryx);

proc foo(r: Rxy) {
  writeln("foo: ", r.type:string);
  writeln("foo: r.type == R(1,1): ", r.type == R(1,1));
}
var r = new Ryx();
foo(r);

proc oof(rr: Ryx) {
  writeln("oof: ", rr.type:string);
  writeln("oof: rr.type == R(1,1): ", rr.type == R(1,1));
}
var rr = new Rxy();
oof(rr);
