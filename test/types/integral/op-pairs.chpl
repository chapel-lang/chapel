proc testit(type t1, type t2) {
  var x: t1 = 2;
  var y: t2 = 1;
  param a: t1 = 2;
  param b: t2 = 1;

  if (t1 == uint && isIntType(t2) ||
      t2 == uint && isIntType(t1)) {
    writeln("Can't mix ", t1:string, " and ", t2:string);
    writeln();
    return;
  } else {
  
  writeln(t1:string, " op ", t2:string);
  writeln("---------------------------------");
  writeln(" + = ", x + y, ": ", (x + y).type:string);
  if (a + b) != (x + y) || (a + b).type != (x + y).type then
    writeln("+ = ", a + b, ": ", (a + b).type:string, " (param)");
  
  writeln(" - = ", x - y, ": ", (x - y).type:string);
  if (a - b) != (x - y) || (a - b).type != (x - y).type then
    writeln("- = ", a - b, ": ", (a - b).type:string, " (param)");

  writeln(" * = ", x * y, ": ", (x * y).type:string);
  if (a * b) != (x * y) || (a * b).type != (x * y).type then
    writeln("* = ", a * b, ": ", (a * b).type:string, " (param)");

  writeln(" / = ", x / y, ": ", (x / y).type:string);
  if (a / b) != (x / y) || (a / b).type != (x / y).type then
    writeln("/ = ", a / b, ": ", (a / b).type:string, " (param)");

  writeln(" % = ", x % y, ": ", (x % y).type:string);
  if (a % b) != (x % y) || (a % b).type != (x % y).type then
    writeln("% = ", a % b, ": ", (a % b).type:string, " (param)");

  writeln("** = ", x ** y, ": ", (x ** y).type:string);
  if (a ** b) != (x ** y) || (a ** b).type != (x ** y).type then
    writeln("** = ", a ** b, ": ", (a ** b).type:string, " (param)");
  }

  writeln();
}

type t = (int(8), int(16), int(32), int(64), uint(8), uint(16), uint(32), uint(64));

var ts: t;

for t1 in ts do
  for t2 in ts do
    testit(t1.type, t2.type);
