config param testLow = false;
config param testHigh = false;
config param testUnbounded = false;
config param testBounded = false;

if !(testLow || testHigh || testUnbounded || testBounded) then
  compilerError("need to set at least one of the expected config params");

var rb = 1..10;
var rlow = 1..;
var rhigh = ..10;
var ru = ..;

if testLow then
  writeln(rb:rlow.type);

if testHigh then
  writeln(rb:rhigh.type);

if testUnbounded then
  writeln(rb:ru.type);

if testBounded then
  writeln(rlow: rb.type);

