// Some testing that return-via-throw destroys records appropriately
config const test = 1;
use ExampleErrors;

run();

proc run() {
  if test == 1 then
    try! simpleThrow();
  if test == 2 then
    try! conditionalThrow(true);
  if test == 3 then
    try! conditionalThrow(false);
  if test == 4 then
    try! loopThrow(1);
  if test == 5 then
    try! loopThrow(2);
  if test == 6 then
    try! inThrow(new R(4));
  if test == 7 then
    try! loopThrow2(1);
  if test == 8 then
    try! loopThrow2(2);
}

record R {
  var x:int;
  proc deinit() {
    writeln("destroying R(x=", x, ")");
  }
}

proc simpleThrow() throws {
  var r = new R(1);
  writeln("in simpleThrow, r=", r);
  throw new owned Error();
}
proc conditionalThrow(arg:bool) throws {
  var r = new R(2);
  writeln("in conditionalThrow, r=", r);
  if arg then
    throw new owned Error();
}
proc loopThrow(arg:int) throws {
  var r = new R(3);
  writeln("in loopThrow, r=", r);
  for i in 1..10 {
    if i == arg then
      throw new owned Error();
  }
}

proc inThrow(in r:R) throws {
  writeln("in inThrow, r=", r);
  throw new owned Error();
}

proc loopThrow2(arg:int) throws {
  for i in 1..10 {
    var r = new R(100+i);
    writeln("in loopThrow, r=", r);
    if i == arg then
      throw new owned Error();
  }
}


