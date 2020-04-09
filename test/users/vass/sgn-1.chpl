config param sgnParam1 = -5, sgnParam2 = 0, sgnParam3 = 7;
compilerWarning(sgn(sgnParam1):string);
compilerWarning(sgn(sgnParam2):string);
compilerWarning(sgn(sgnParam3):string);

var sgnResult3: uint(8) = sgn(sgnParam3);
writeln(sgnResult3);

proc test(type argT, type resT,
          arg1: argT, arg2: argT, arg3: argT)
{
  writeln(argT:string, " -> ", resT:string);
  var r: resT;
  r = sgn(arg1); writeln(r);
  r = sgn(arg2); writeln(r);
  r = sgn(arg3); writeln(r);
}

test(int(64), int(8), 0, 44, -23);
test(uint(64), uint(8), 0, 23, 55);
test(real, int(8), -33.44, 55.22, 0.0);
