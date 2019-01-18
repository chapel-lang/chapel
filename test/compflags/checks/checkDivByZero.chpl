config param testParamCases = 0;
config var num = 10,
           unum = 10: uint,
           denom = 0,
           udenom = 0: uint;

config const testExecCases = 0;

if testParamCases == 1 then
  writeln(10/0);

if testParamCases == 2 then
  writeln(num/0);

if testParamCases == 3 then
  writeln(unum/0);

if testParamCases == 4 then
  writeln(10:uint / 0: uint);

if testExecCases == 0 then
  writeln(num/denom);

if testExecCases == 1 then
  writeln(unum/udenom);

if testExecCases == 2 then
  num /= denom;

if testExecCases == 3 then
  unum /= udenom;
