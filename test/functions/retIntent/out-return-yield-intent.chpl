
var x = 0;
proc update() { x += 1; writeln("updated x to ", x); }

proc retOut() out do return x;
iter yieldOut() out do yield x;

const ref y = retOut();
update();
writeln("y = ", y);

for z in yieldOut() {
  update();
  writeln("z = ", z);
}
