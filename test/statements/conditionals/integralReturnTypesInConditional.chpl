proc testGeneric(arg: bool, type t, type tt) {
  var i: t = 0;
  var u: tt = 0;
  if arg then return i;
  else return u;
}

proc testGenericCast(arg: bool, type t, type tt) {
  var zero: uint(8);
  if arg then return zero:t;
  else return zero:tt;
}

proc testParamGenericCast(arg: bool, type t, type tt) {
  if arg then return 0:t;
  else return 0:tt;
}

proc testExpression(arg: bool, type t, type tt) {
  var zero: uint(8);
  var x = if arg then zero:t else zero:tt;
  writeln("testExpression: when ", arg:string, " ", t:string, " ",
          tt:string, " then ", x.type:string);
}

proc testParamExpression(arg: bool, type t, type tt) {
  var x = if arg then 0:t else 0:tt;
  writeln("testParamExpression: when ", arg:string, " ", t:string, " ",
          tt:string, " then ", x.type:string);
}

const A = [true, false];

const Ti = (0:int(8), 0:int(16), 0:int(32), 0:int(64));
const Tu = (0:uint(8), 0:uint(16), 0:uint(32), 0:uint(64));

for a in A {
  for ti in Ti {
    for tti in Ti {
      var xxx = testGeneric(a, ti.type, tti.type);
      writeln("testGeneric: when ", a:string, " ", ti.type:string, " ",
              tti.type:string, " then ", xxx.type:string);
      var yyy = testGenericCast(a, ti.type, tti.type);
      writeln("testGenericCast: when ", a:string, " ", ti.type:string, " ",
              tti.type:string, " then ", yyy.type:string);
      testExpression(a, ti.type, tti.type);
      testParamExpression(a, ti.type, tti.type);
      var zzz = testParamGenericCast(a, ti.type, tti.type);
      writeln("testParamGenericCast: when ", a:string, " ", ti.type:string, " ",
              tti.type:string, " then ", zzz.type:string);
    }
    for tu in Tu {
      testExpression(a, ti.type, tu.type);
      testExpression(a, tu.type, ti.type);
      testParamExpression(a, ti.type, tu.type);
      testParamExpression(a, tu.type, ti.type);
      var x = testGeneric(a, ti.type, tu.type);
      writeln("testGeneric: when ", a:string, " ", ti.type:string, " ",
              tu.type:string, " then ", x.type:string);
      var xx = testGeneric(a, tu.type, ti.type);
      writeln("testGeneric: when ", a:string, " ", tu.type:string, " ",
              ti.type:string, " then " , xx.type:string);

      var y = testGenericCast(a, ti.type, tu.type);
      writeln("testGenericCast: when ", a:string, " ", ti.type:string, " ",
              tu.type:string, " then " , y.type:string);
      var yy = testGenericCast(a, tu.type, ti.type);
      writeln("testGenericCast: when ", a:string, " ", tu.type:string, " ",
              ti.type:string, " then " , yy.type:string);

      var z = testParamGenericCast(a, ti.type, tu.type);
      writeln("testParamGenericCast: when ", a:string, " ", ti.type:string, " ",
              tu.type:string, " then ", z.type:string);
      var zz = testParamGenericCast(a, tu.type, ti.type);
      writeln("testParamGenericCast: when ", a:string, " ", tu.type:string, " ",
              ti.type:string, " then ", zz.type:string);
    }
  }
  for tz in Tu {
    for tzz in Tu {
      var xxx = testGeneric(a, tz.type, tzz.type);
      writeln("testGeneric: when ", a:string, " ", tz.type:string, " ",
              tzz.type:string, " then " , xxx.type:string);
      var yyy = testGenericCast(a, tz.type, tzz.type);
      writeln("testGenericCast: when ", a:string, " ", tz.type:string, " ",
              tzz.type:string, " then " , yyy.type:string);
      testExpression(a, tz.type, tzz.type);
      testParamExpression(a, tz.type, tzz.type);
      var zzz = testParamGenericCast(a, tz.type, tzz.type);
      writeln("testParamGenericCast: when ", a:string, " ", tz.type:string, " ",
              tzz.type:string, " then ", zzz.type:string);
    }
  }
}
