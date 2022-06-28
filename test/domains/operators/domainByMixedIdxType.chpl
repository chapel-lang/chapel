var i8 = 100: int(8);

const step = 10;

var r1 = 1..i8;
var r2 = r1 by step;
print(r1);
print(r2);

const D1 = {r1};
const D2 = {r1, r1};

print(D1 by step);
print(D2 by step);
print(D2 by (step, step));

proc print(x) {
  writeln(x, ": ", x.type:string);
}
