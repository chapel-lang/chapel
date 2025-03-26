{
  proc testProcCastZero(arg: bool) {
    if arg then return 0:int(8); else return 0:int(8);
  }
  var x = testProcCastZero(true);
  writeln("testProcCastZero: when true int(8) int(8) then ", x.type:string);
}

{
  proc testProcZero(arg: bool) {
    var i: int(8) = 0;
    var u: int(8) = 0;
    if arg then return i; else return u;
  }
  var x = testProcZero(true);
  writeln("testProcZero: when true int(8) int(8) then ", x.type:string);
}

{
  proc testProcCastMax(arg: bool) {
    if arg then return max(int(8)):int(8); else return max(int(8)):int(8);
  }
  var x = testProcCastMax(true);
  writeln("testProcCastMax: when true int(8) int(8) then ", x.type:string);
}

{
  proc testProcMax(arg: bool) {
    var i: int(8) = max(int(8));
    var u: int(8) = max(int(8));
    if arg then return i; else return u;
  }
  var x = testProcMax(true);
  writeln("testProcMax: when true int(8) int(8) then ", x.type:string);
}

{
  proc testParamProcCastZero(param arg: bool) param {
    if arg then return 0:int(8); else return 0:int(8);
  }
  param x = testParamProcCastZero(true);
  writeln("testParamProcCastZero: when true int(8) int(8) then ", x.type:string);
}

{
  proc testParamProcZero(param arg: bool) param {
    param i: int(8) = 0;
    param u: int(8) = 0;
    if arg then return i; else return u;
  }
  param x = testParamProcZero(true);
  writeln("testParamProcZero: when true int(8) int(8) then ", x.type:string);
}

{
  proc testParamProcCastMax(param arg: bool) param {
    if arg then return max(int(8)):int(8); else return max(int(8)):int(8);
  }
  param x = testParamProcCastMax(true);
  writeln("testParamProcCastMax: when true int(8) int(8) then ", x.type:string);
}

{
  proc testParamProcMax(param arg: bool) param {
    param i: int(8) = max(int(8));
    param u: int(8) = max(int(8));
    if arg then return i; else return u;
  }
  param x = testParamProcMax(true);
  writeln("testParamProcMax: when true int(8) int(8) then ", x.type:string);
}

{
  var b: bool = true;
  var x = if b then 0:int(8) else 0:int(8);
  writeln("testExpressionZero: when true int(8) int(8) then ", x.type:string);
}

{
  var b: bool = true;
  var x = if b then max(int(8)):int(8) else max(int(8)):int(8);
  writeln("testExpressionMax: when true int(8) int(8) then ", x.type:string);
}

{
  param x = if true then 0:int(8) else 0:int(8);
  writeln("testParamExpressionZero: when true int(8) int(8) then ", x.type:string);
}

{
  param x = if true then max(int(8)):int(8) else max(int(8)):int(8);
  writeln("testParamExpressionMax: when true int(8) int(8) then ", x.type:string);
}

{
  proc testProcCastZero(arg: bool) {
    if arg then return 0:int(8); else return 0:int(16);
  }
  var x = testProcCastZero(true);
  writeln("testProcCastZero: when true int(8) int(16) then ", x.type:string);
}

{
  proc testProcZero(arg: bool) {
    var i: int(8) = 0;
    var u: int(16) = 0;
    if arg then return i; else return u;
  }
  var x = testProcZero(true);
  writeln("testProcZero: when true int(8) int(16) then ", x.type:string);
}

{
  proc testProcCastMax(arg: bool) {
    if arg then return max(int(8)):int(8); else return max(int(16)):int(16);
  }
  var x = testProcCastMax(true);
  writeln("testProcCastMax: when true int(8) int(16) then ", x.type:string);
}

{
  proc testProcMax(arg: bool) {
    var i: int(8) = max(int(8));
    var u: int(16) = max(int(16));
    if arg then return i; else return u;
  }
  var x = testProcMax(true);
  writeln("testProcMax: when true int(8) int(16) then ", x.type:string);
}

{
  proc testParamProcCastZero(param arg: bool) param {
    if arg then return 0:int(8); else return 0:int(16);
  }
  param x = testParamProcCastZero(true);
  writeln("testParamProcCastZero: when true int(8) int(16) then ", x.type:string);
}

{
  proc testParamProcZero(param arg: bool) param {
    param i: int(8) = 0;
    param u: int(16) = 0;
    if arg then return i; else return u;
  }
  param x = testParamProcZero(true);
  writeln("testParamProcZero: when true int(8) int(16) then ", x.type:string);
}

{
  proc testParamProcCastMax(param arg: bool) param {
    if arg then return max(int(8)):int(8); else return max(int(16)):int(16);
  }
  param x = testParamProcCastMax(true);
  writeln("testParamProcCastMax: when true int(8) int(16) then ", x.type:string);
}

{
  proc testParamProcMax(param arg: bool) param {
    param i: int(8) = max(int(8));
    param u: int(16) = max(int(16));
    if arg then return i; else return u;
  }
  param x = testParamProcMax(true);
  writeln("testParamProcMax: when true int(8) int(16) then ", x.type:string);
}

{
  var b: bool = true;
  var x = if b then 0:int(8) else 0:int(16);
  writeln("testExpressionZero: when true int(8) int(16) then ", x.type:string);
}

{
  var b: bool = true;
  var x = if b then max(int(8)):int(8) else max(int(16)):int(16);
  writeln("testExpressionMax: when true int(8) int(16) then ", x.type:string);
}

{
  param x = if true then 0:int(8) else 0:int(16);
  writeln("testParamExpressionZero: when true int(8) int(16) then ", x.type:string);
}

{
  param x = if true then max(int(8)):int(8) else max(int(16)):int(16);
  writeln("testParamExpressionMax: when true int(8) int(16) then ", x.type:string);
}

{
  proc testProcCastZero(arg: bool) {
    if arg then return 0:int(8); else return 0:int(32);
  }
  var x = testProcCastZero(true);
  writeln("testProcCastZero: when true int(8) int(32) then ", x.type:string);
}

{
  proc testProcZero(arg: bool) {
    var i: int(8) = 0;
    var u: int(32) = 0;
    if arg then return i; else return u;
  }
  var x = testProcZero(true);
  writeln("testProcZero: when true int(8) int(32) then ", x.type:string);
}

{
  proc testProcCastMax(arg: bool) {
    if arg then return max(int(8)):int(8); else return max(int(32)):int(32);
  }
  var x = testProcCastMax(true);
  writeln("testProcCastMax: when true int(8) int(32) then ", x.type:string);
}

{
  proc testProcMax(arg: bool) {
    var i: int(8) = max(int(8));
    var u: int(32) = max(int(32));
    if arg then return i; else return u;
  }
  var x = testProcMax(true);
  writeln("testProcMax: when true int(8) int(32) then ", x.type:string);
}

{
  proc testParamProcCastZero(param arg: bool) param {
    if arg then return 0:int(8); else return 0:int(32);
  }
  param x = testParamProcCastZero(true);
  writeln("testParamProcCastZero: when true int(8) int(32) then ", x.type:string);
}

{
  proc testParamProcZero(param arg: bool) param {
    param i: int(8) = 0;
    param u: int(32) = 0;
    if arg then return i; else return u;
  }
  param x = testParamProcZero(true);
  writeln("testParamProcZero: when true int(8) int(32) then ", x.type:string);
}

{
  proc testParamProcCastMax(param arg: bool) param {
    if arg then return max(int(8)):int(8); else return max(int(32)):int(32);
  }
  param x = testParamProcCastMax(true);
  writeln("testParamProcCastMax: when true int(8) int(32) then ", x.type:string);
}

{
  proc testParamProcMax(param arg: bool) param {
    param i: int(8) = max(int(8));
    param u: int(32) = max(int(32));
    if arg then return i; else return u;
  }
  param x = testParamProcMax(true);
  writeln("testParamProcMax: when true int(8) int(32) then ", x.type:string);
}

{
  var b: bool = true;
  var x = if b then 0:int(8) else 0:int(32);
  writeln("testExpressionZero: when true int(8) int(32) then ", x.type:string);
}

{
  var b: bool = true;
  var x = if b then max(int(8)):int(8) else max(int(32)):int(32);
  writeln("testExpressionMax: when true int(8) int(32) then ", x.type:string);
}

{
  param x = if true then 0:int(8) else 0:int(32);
  writeln("testParamExpressionZero: when true int(8) int(32) then ", x.type:string);
}

{
  param x = if true then max(int(8)):int(8) else max(int(32)):int(32);
  writeln("testParamExpressionMax: when true int(8) int(32) then ", x.type:string);
}

{
  proc testProcCastZero(arg: bool) {
    if arg then return 0:int(8); else return 0:int(64);
  }
  var x = testProcCastZero(true);
  writeln("testProcCastZero: when true int(8) int(64) then ", x.type:string);
}

{
  proc testProcZero(arg: bool) {
    var i: int(8) = 0;
    var u: int(64) = 0;
    if arg then return i; else return u;
  }
  var x = testProcZero(true);
  writeln("testProcZero: when true int(8) int(64) then ", x.type:string);
}

{
  proc testProcCastMax(arg: bool) {
    if arg then return max(int(8)):int(8); else return max(int(64)):int(64);
  }
  var x = testProcCastMax(true);
  writeln("testProcCastMax: when true int(8) int(64) then ", x.type:string);
}

{
  proc testProcMax(arg: bool) {
    var i: int(8) = max(int(8));
    var u: int(64) = max(int(64));
    if arg then return i; else return u;
  }
  var x = testProcMax(true);
  writeln("testProcMax: when true int(8) int(64) then ", x.type:string);
}

{
  proc testParamProcCastZero(param arg: bool) param {
    if arg then return 0:int(8); else return 0:int(64);
  }
  param x = testParamProcCastZero(true);
  writeln("testParamProcCastZero: when true int(8) int(64) then ", x.type:string);
}

{
  proc testParamProcZero(param arg: bool) param {
    param i: int(8) = 0;
    param u: int(64) = 0;
    if arg then return i; else return u;
  }
  param x = testParamProcZero(true);
  writeln("testParamProcZero: when true int(8) int(64) then ", x.type:string);
}

{
  proc testParamProcCastMax(param arg: bool) param {
    if arg then return max(int(8)):int(8); else return max(int(64)):int(64);
  }
  param x = testParamProcCastMax(true);
  writeln("testParamProcCastMax: when true int(8) int(64) then ", x.type:string);
}

{
  proc testParamProcMax(param arg: bool) param {
    param i: int(8) = max(int(8));
    param u: int(64) = max(int(64));
    if arg then return i; else return u;
  }
  param x = testParamProcMax(true);
  writeln("testParamProcMax: when true int(8) int(64) then ", x.type:string);
}

{
  var b: bool = true;
  var x = if b then 0:int(8) else 0:int(64);
  writeln("testExpressionZero: when true int(8) int(64) then ", x.type:string);
}

{
  var b: bool = true;
  var x = if b then max(int(8)):int(8) else max(int(64)):int(64);
  writeln("testExpressionMax: when true int(8) int(64) then ", x.type:string);
}

{
  param x = if true then 0:int(8) else 0:int(64);
  writeln("testParamExpressionZero: when true int(8) int(64) then ", x.type:string);
}

{
  param x = if true then max(int(8)):int(8) else max(int(64)):int(64);
  writeln("testParamExpressionMax: when true int(8) int(64) then ", x.type:string);
}

{
  proc testProcCastZero(arg: bool) {
    if arg then return 0:int(8); else return 0:uint(8);
  }
  var x = testProcCastZero(true);
  writeln("testProcCastZero: when true int(8) uint(8) then ", x.type:string);
}

{
  proc testProcZero(arg: bool) {
    var i: int(8) = 0;
    var u: uint(8) = 0;
    if arg then return i; else return u;
  }
  var x = testProcZero(true);
  writeln("testProcZero: when true int(8) uint(8) then ", x.type:string);
}

{
  proc testProcCastMax(arg: bool) {
    if arg then return max(int(8)):int(8); else return max(uint(8)):uint(8);
  }
  var x = testProcCastMax(true);
  writeln("testProcCastMax: when true int(8) uint(8) then ", x.type:string);
}

{
  proc testProcMax(arg: bool) {
    var i: int(8) = max(int(8));
    var u: uint(8) = max(uint(8));
    if arg then return i; else return u;
  }
  var x = testProcMax(true);
  writeln("testProcMax: when true int(8) uint(8) then ", x.type:string);
}

{
  proc testParamProcCastZero(param arg: bool) param {
    if arg then return 0:int(8); else return 0:uint(8);
  }
  param x = testParamProcCastZero(true);
  writeln("testParamProcCastZero: when true int(8) uint(8) then ", x.type:string);
}

{
  proc testParamProcZero(param arg: bool) param {
    param i: int(8) = 0;
    param u: uint(8) = 0;
    if arg then return i; else return u;
  }
  param x = testParamProcZero(true);
  writeln("testParamProcZero: when true int(8) uint(8) then ", x.type:string);
}

{
  proc testParamProcCastMax(param arg: bool) param {
    if arg then return max(int(8)):int(8); else return max(uint(8)):uint(8);
  }
  param x = testParamProcCastMax(true);
  writeln("testParamProcCastMax: when true int(8) uint(8) then ", x.type:string);
}

{
  proc testParamProcMax(param arg: bool) param {
    param i: int(8) = max(int(8));
    param u: uint(8) = max(uint(8));
    if arg then return i; else return u;
  }
  param x = testParamProcMax(true);
  writeln("testParamProcMax: when true int(8) uint(8) then ", x.type:string);
}

{
  var b: bool = true;
  var x = if b then 0:int(8) else 0:uint(8);
  writeln("testExpressionZero: when true int(8) uint(8) then ", x.type:string);
}

{
  var b: bool = true;
  var x = if b then max(int(8)):int(8) else max(uint(8)):uint(8);
  writeln("testExpressionMax: when true int(8) uint(8) then ", x.type:string);
}

{
  param x = if true then 0:int(8) else 0:uint(8);
  writeln("testParamExpressionZero: when true int(8) uint(8) then ", x.type:string);
}

{
  param x = if true then max(int(8)):int(8) else max(uint(8)):uint(8);
  writeln("testParamExpressionMax: when true int(8) uint(8) then ", x.type:string);
}

{
  proc testProcCastZero(arg: bool) {
    if arg then return 0:int(8); else return 0:uint(16);
  }
  var x = testProcCastZero(true);
  writeln("testProcCastZero: when true int(8) uint(16) then ", x.type:string);
}

{
  proc testProcZero(arg: bool) {
    var i: int(8) = 0;
    var u: uint(16) = 0;
    if arg then return i; else return u;
  }
  var x = testProcZero(true);
  writeln("testProcZero: when true int(8) uint(16) then ", x.type:string);
}

{
  proc testProcCastMax(arg: bool) {
    if arg then return max(int(8)):int(8); else return max(uint(16)):uint(16);
  }
  var x = testProcCastMax(true);
  writeln("testProcCastMax: when true int(8) uint(16) then ", x.type:string);
}

{
  proc testProcMax(arg: bool) {
    var i: int(8) = max(int(8));
    var u: uint(16) = max(uint(16));
    if arg then return i; else return u;
  }
  var x = testProcMax(true);
  writeln("testProcMax: when true int(8) uint(16) then ", x.type:string);
}

{
  proc testParamProcCastZero(param arg: bool) param {
    if arg then return 0:int(8); else return 0:uint(16);
  }
  param x = testParamProcCastZero(true);
  writeln("testParamProcCastZero: when true int(8) uint(16) then ", x.type:string);
}

{
  proc testParamProcZero(param arg: bool) param {
    param i: int(8) = 0;
    param u: uint(16) = 0;
    if arg then return i; else return u;
  }
  param x = testParamProcZero(true);
  writeln("testParamProcZero: when true int(8) uint(16) then ", x.type:string);
}

{
  proc testParamProcCastMax(param arg: bool) param {
    if arg then return max(int(8)):int(8); else return max(uint(16)):uint(16);
  }
  param x = testParamProcCastMax(true);
  writeln("testParamProcCastMax: when true int(8) uint(16) then ", x.type:string);
}

{
  proc testParamProcMax(param arg: bool) param {
    param i: int(8) = max(int(8));
    param u: uint(16) = max(uint(16));
    if arg then return i; else return u;
  }
  param x = testParamProcMax(true);
  writeln("testParamProcMax: when true int(8) uint(16) then ", x.type:string);
}

{
  var b: bool = true;
  var x = if b then 0:int(8) else 0:uint(16);
  writeln("testExpressionZero: when true int(8) uint(16) then ", x.type:string);
}

{
  var b: bool = true;
  var x = if b then max(int(8)):int(8) else max(uint(16)):uint(16);
  writeln("testExpressionMax: when true int(8) uint(16) then ", x.type:string);
}

{
  param x = if true then 0:int(8) else 0:uint(16);
  writeln("testParamExpressionZero: when true int(8) uint(16) then ", x.type:string);
}

{
  param x = if true then max(int(8)):int(8) else max(uint(16)):uint(16);
  writeln("testParamExpressionMax: when true int(8) uint(16) then ", x.type:string);
}

{
  proc testProcCastZero(arg: bool) {
    if arg then return 0:int(8); else return 0:uint(32);
  }
  var x = testProcCastZero(true);
  writeln("testProcCastZero: when true int(8) uint(32) then ", x.type:string);
}

{
  proc testProcZero(arg: bool) {
    var i: int(8) = 0;
    var u: uint(32) = 0;
    if arg then return i; else return u;
  }
  var x = testProcZero(true);
  writeln("testProcZero: when true int(8) uint(32) then ", x.type:string);
}

{
  proc testProcCastMax(arg: bool) {
    if arg then return max(int(8)):int(8); else return max(uint(32)):uint(32);
  }
  var x = testProcCastMax(true);
  writeln("testProcCastMax: when true int(8) uint(32) then ", x.type:string);
}

{
  proc testProcMax(arg: bool) {
    var i: int(8) = max(int(8));
    var u: uint(32) = max(uint(32));
    if arg then return i; else return u;
  }
  var x = testProcMax(true);
  writeln("testProcMax: when true int(8) uint(32) then ", x.type:string);
}

{
  proc testParamProcCastZero(param arg: bool) param {
    if arg then return 0:int(8); else return 0:uint(32);
  }
  param x = testParamProcCastZero(true);
  writeln("testParamProcCastZero: when true int(8) uint(32) then ", x.type:string);
}

{
  proc testParamProcZero(param arg: bool) param {
    param i: int(8) = 0;
    param u: uint(32) = 0;
    if arg then return i; else return u;
  }
  param x = testParamProcZero(true);
  writeln("testParamProcZero: when true int(8) uint(32) then ", x.type:string);
}

{
  proc testParamProcCastMax(param arg: bool) param {
    if arg then return max(int(8)):int(8); else return max(uint(32)):uint(32);
  }
  param x = testParamProcCastMax(true);
  writeln("testParamProcCastMax: when true int(8) uint(32) then ", x.type:string);
}

{
  proc testParamProcMax(param arg: bool) param {
    param i: int(8) = max(int(8));
    param u: uint(32) = max(uint(32));
    if arg then return i; else return u;
  }
  param x = testParamProcMax(true);
  writeln("testParamProcMax: when true int(8) uint(32) then ", x.type:string);
}

{
  var b: bool = true;
  var x = if b then 0:int(8) else 0:uint(32);
  writeln("testExpressionZero: when true int(8) uint(32) then ", x.type:string);
}

{
  var b: bool = true;
  var x = if b then max(int(8)):int(8) else max(uint(32)):uint(32);
  writeln("testExpressionMax: when true int(8) uint(32) then ", x.type:string);
}

{
  param x = if true then 0:int(8) else 0:uint(32);
  writeln("testParamExpressionZero: when true int(8) uint(32) then ", x.type:string);
}

{
  param x = if true then max(int(8)):int(8) else max(uint(32)):uint(32);
  writeln("testParamExpressionMax: when true int(8) uint(32) then ", x.type:string);
}

{
  proc testProcCastZero(arg: bool) {
    if arg then return 0:int(8); else return 0:uint(64);
  }
  var x = testProcCastZero(true);
  writeln("testProcCastZero: when true int(8) uint(64) then ", x.type:string);
}

{
  proc testProcZero(arg: bool) {
    var i: int(8) = 0;
    var u: uint(64) = 0;
    if arg then return i; else return u;
  }
  var x = testProcZero(true);
  writeln("testProcZero: when true int(8) uint(64) then ", x.type:string);
}

{
  proc testProcCastMax(arg: bool) {
    if arg then return max(int(8)):int(8); else return max(uint(64)):uint(64);
  }
  var x = testProcCastMax(true);
  writeln("testProcCastMax: when true int(8) uint(64) then ", x.type:string);
}

{
  proc testProcMax(arg: bool) {
    var i: int(8) = max(int(8));
    var u: uint(64) = max(uint(64));
    if arg then return i; else return u;
  }
  var x = testProcMax(true);
  writeln("testProcMax: when true int(8) uint(64) then ", x.type:string);
}

{
  proc testParamProcCastZero(param arg: bool) param {
    if arg then return 0:int(8); else return 0:uint(64);
  }
  param x = testParamProcCastZero(true);
  writeln("testParamProcCastZero: when true int(8) uint(64) then ", x.type:string);
}

{
  proc testParamProcZero(param arg: bool) param {
    param i: int(8) = 0;
    param u: uint(64) = 0;
    if arg then return i; else return u;
  }
  param x = testParamProcZero(true);
  writeln("testParamProcZero: when true int(8) uint(64) then ", x.type:string);
}

{
  proc testParamProcCastMax(param arg: bool) param {
    if arg then return max(int(8)):int(8); else return max(uint(64)):uint(64);
  }
  param x = testParamProcCastMax(true);
  writeln("testParamProcCastMax: when true int(8) uint(64) then ", x.type:string);
}

{
  proc testParamProcMax(param arg: bool) param {
    param i: int(8) = max(int(8));
    param u: uint(64) = max(uint(64));
    if arg then return i; else return u;
  }
  param x = testParamProcMax(true);
  writeln("testParamProcMax: when true int(8) uint(64) then ", x.type:string);
}

{
  var b: bool = true;
  var x = if b then 0:int(8) else 0:uint(64);
  writeln("testExpressionZero: when true int(8) uint(64) then ", x.type:string);
}

{
  var b: bool = true;
  var x = if b then max(int(8)):int(8) else max(uint(64)):uint(64);
  writeln("testExpressionMax: when true int(8) uint(64) then ", x.type:string);
}

{
  param x = if true then 0:int(8) else 0:uint(64);
  writeln("testParamExpressionZero: when true int(8) uint(64) then ", x.type:string);
}

{
  param x = if true then max(int(8)):int(8) else max(uint(64)):uint(64);
  writeln("testParamExpressionMax: when true int(8) uint(64) then ", x.type:string);
}

{
  proc testProcCastZero(arg: bool) {
    if arg then return 0:int(16); else return 0:int(8);
  }
  var x = testProcCastZero(true);
  writeln("testProcCastZero: when true int(16) int(8) then ", x.type:string);
}

{
  proc testProcZero(arg: bool) {
    var i: int(16) = 0;
    var u: int(8) = 0;
    if arg then return i; else return u;
  }
  var x = testProcZero(true);
  writeln("testProcZero: when true int(16) int(8) then ", x.type:string);
}

{
  proc testProcCastMax(arg: bool) {
    if arg then return max(int(16)):int(16); else return max(int(8)):int(8);
  }
  var x = testProcCastMax(true);
  writeln("testProcCastMax: when true int(16) int(8) then ", x.type:string);
}

{
  proc testProcMax(arg: bool) {
    var i: int(16) = max(int(16));
    var u: int(8) = max(int(8));
    if arg then return i; else return u;
  }
  var x = testProcMax(true);
  writeln("testProcMax: when true int(16) int(8) then ", x.type:string);
}

{
  proc testParamProcCastZero(param arg: bool) param {
    if arg then return 0:int(16); else return 0:int(8);
  }
  param x = testParamProcCastZero(true);
  writeln("testParamProcCastZero: when true int(16) int(8) then ", x.type:string);
}

{
  proc testParamProcZero(param arg: bool) param {
    param i: int(16) = 0;
    param u: int(8) = 0;
    if arg then return i; else return u;
  }
  param x = testParamProcZero(true);
  writeln("testParamProcZero: when true int(16) int(8) then ", x.type:string);
}

{
  proc testParamProcCastMax(param arg: bool) param {
    if arg then return max(int(16)):int(16); else return max(int(8)):int(8);
  }
  param x = testParamProcCastMax(true);
  writeln("testParamProcCastMax: when true int(16) int(8) then ", x.type:string);
}

{
  proc testParamProcMax(param arg: bool) param {
    param i: int(16) = max(int(16));
    param u: int(8) = max(int(8));
    if arg then return i; else return u;
  }
  param x = testParamProcMax(true);
  writeln("testParamProcMax: when true int(16) int(8) then ", x.type:string);
}

{
  var b: bool = true;
  var x = if b then 0:int(16) else 0:int(8);
  writeln("testExpressionZero: when true int(16) int(8) then ", x.type:string);
}

{
  var b: bool = true;
  var x = if b then max(int(16)):int(16) else max(int(8)):int(8);
  writeln("testExpressionMax: when true int(16) int(8) then ", x.type:string);
}

{
  param x = if true then 0:int(16) else 0:int(8);
  writeln("testParamExpressionZero: when true int(16) int(8) then ", x.type:string);
}

{
  param x = if true then max(int(16)):int(16) else max(int(8)):int(8);
  writeln("testParamExpressionMax: when true int(16) int(8) then ", x.type:string);
}

{
  proc testProcCastZero(arg: bool) {
    if arg then return 0:int(16); else return 0:int(16);
  }
  var x = testProcCastZero(true);
  writeln("testProcCastZero: when true int(16) int(16) then ", x.type:string);
}

{
  proc testProcZero(arg: bool) {
    var i: int(16) = 0;
    var u: int(16) = 0;
    if arg then return i; else return u;
  }
  var x = testProcZero(true);
  writeln("testProcZero: when true int(16) int(16) then ", x.type:string);
}

{
  proc testProcCastMax(arg: bool) {
    if arg then return max(int(16)):int(16); else return max(int(16)):int(16);
  }
  var x = testProcCastMax(true);
  writeln("testProcCastMax: when true int(16) int(16) then ", x.type:string);
}

{
  proc testProcMax(arg: bool) {
    var i: int(16) = max(int(16));
    var u: int(16) = max(int(16));
    if arg then return i; else return u;
  }
  var x = testProcMax(true);
  writeln("testProcMax: when true int(16) int(16) then ", x.type:string);
}

{
  proc testParamProcCastZero(param arg: bool) param {
    if arg then return 0:int(16); else return 0:int(16);
  }
  param x = testParamProcCastZero(true);
  writeln("testParamProcCastZero: when true int(16) int(16) then ", x.type:string);
}

{
  proc testParamProcZero(param arg: bool) param {
    param i: int(16) = 0;
    param u: int(16) = 0;
    if arg then return i; else return u;
  }
  param x = testParamProcZero(true);
  writeln("testParamProcZero: when true int(16) int(16) then ", x.type:string);
}

{
  proc testParamProcCastMax(param arg: bool) param {
    if arg then return max(int(16)):int(16); else return max(int(16)):int(16);
  }
  param x = testParamProcCastMax(true);
  writeln("testParamProcCastMax: when true int(16) int(16) then ", x.type:string);
}

{
  proc testParamProcMax(param arg: bool) param {
    param i: int(16) = max(int(16));
    param u: int(16) = max(int(16));
    if arg then return i; else return u;
  }
  param x = testParamProcMax(true);
  writeln("testParamProcMax: when true int(16) int(16) then ", x.type:string);
}

{
  var b: bool = true;
  var x = if b then 0:int(16) else 0:int(16);
  writeln("testExpressionZero: when true int(16) int(16) then ", x.type:string);
}

{
  var b: bool = true;
  var x = if b then max(int(16)):int(16) else max(int(16)):int(16);
  writeln("testExpressionMax: when true int(16) int(16) then ", x.type:string);
}

{
  param x = if true then 0:int(16) else 0:int(16);
  writeln("testParamExpressionZero: when true int(16) int(16) then ", x.type:string);
}

{
  param x = if true then max(int(16)):int(16) else max(int(16)):int(16);
  writeln("testParamExpressionMax: when true int(16) int(16) then ", x.type:string);
}

{
  proc testProcCastZero(arg: bool) {
    if arg then return 0:int(16); else return 0:int(32);
  }
  var x = testProcCastZero(true);
  writeln("testProcCastZero: when true int(16) int(32) then ", x.type:string);
}

{
  proc testProcZero(arg: bool) {
    var i: int(16) = 0;
    var u: int(32) = 0;
    if arg then return i; else return u;
  }
  var x = testProcZero(true);
  writeln("testProcZero: when true int(16) int(32) then ", x.type:string);
}

{
  proc testProcCastMax(arg: bool) {
    if arg then return max(int(16)):int(16); else return max(int(32)):int(32);
  }
  var x = testProcCastMax(true);
  writeln("testProcCastMax: when true int(16) int(32) then ", x.type:string);
}

{
  proc testProcMax(arg: bool) {
    var i: int(16) = max(int(16));
    var u: int(32) = max(int(32));
    if arg then return i; else return u;
  }
  var x = testProcMax(true);
  writeln("testProcMax: when true int(16) int(32) then ", x.type:string);
}

{
  proc testParamProcCastZero(param arg: bool) param {
    if arg then return 0:int(16); else return 0:int(32);
  }
  param x = testParamProcCastZero(true);
  writeln("testParamProcCastZero: when true int(16) int(32) then ", x.type:string);
}

{
  proc testParamProcZero(param arg: bool) param {
    param i: int(16) = 0;
    param u: int(32) = 0;
    if arg then return i; else return u;
  }
  param x = testParamProcZero(true);
  writeln("testParamProcZero: when true int(16) int(32) then ", x.type:string);
}

{
  proc testParamProcCastMax(param arg: bool) param {
    if arg then return max(int(16)):int(16); else return max(int(32)):int(32);
  }
  param x = testParamProcCastMax(true);
  writeln("testParamProcCastMax: when true int(16) int(32) then ", x.type:string);
}

{
  proc testParamProcMax(param arg: bool) param {
    param i: int(16) = max(int(16));
    param u: int(32) = max(int(32));
    if arg then return i; else return u;
  }
  param x = testParamProcMax(true);
  writeln("testParamProcMax: when true int(16) int(32) then ", x.type:string);
}

{
  var b: bool = true;
  var x = if b then 0:int(16) else 0:int(32);
  writeln("testExpressionZero: when true int(16) int(32) then ", x.type:string);
}

{
  var b: bool = true;
  var x = if b then max(int(16)):int(16) else max(int(32)):int(32);
  writeln("testExpressionMax: when true int(16) int(32) then ", x.type:string);
}

{
  param x = if true then 0:int(16) else 0:int(32);
  writeln("testParamExpressionZero: when true int(16) int(32) then ", x.type:string);
}

{
  param x = if true then max(int(16)):int(16) else max(int(32)):int(32);
  writeln("testParamExpressionMax: when true int(16) int(32) then ", x.type:string);
}

{
  proc testProcCastZero(arg: bool) {
    if arg then return 0:int(16); else return 0:int(64);
  }
  var x = testProcCastZero(true);
  writeln("testProcCastZero: when true int(16) int(64) then ", x.type:string);
}

{
  proc testProcZero(arg: bool) {
    var i: int(16) = 0;
    var u: int(64) = 0;
    if arg then return i; else return u;
  }
  var x = testProcZero(true);
  writeln("testProcZero: when true int(16) int(64) then ", x.type:string);
}

{
  proc testProcCastMax(arg: bool) {
    if arg then return max(int(16)):int(16); else return max(int(64)):int(64);
  }
  var x = testProcCastMax(true);
  writeln("testProcCastMax: when true int(16) int(64) then ", x.type:string);
}

{
  proc testProcMax(arg: bool) {
    var i: int(16) = max(int(16));
    var u: int(64) = max(int(64));
    if arg then return i; else return u;
  }
  var x = testProcMax(true);
  writeln("testProcMax: when true int(16) int(64) then ", x.type:string);
}

{
  proc testParamProcCastZero(param arg: bool) param {
    if arg then return 0:int(16); else return 0:int(64);
  }
  param x = testParamProcCastZero(true);
  writeln("testParamProcCastZero: when true int(16) int(64) then ", x.type:string);
}

{
  proc testParamProcZero(param arg: bool) param {
    param i: int(16) = 0;
    param u: int(64) = 0;
    if arg then return i; else return u;
  }
  param x = testParamProcZero(true);
  writeln("testParamProcZero: when true int(16) int(64) then ", x.type:string);
}

{
  proc testParamProcCastMax(param arg: bool) param {
    if arg then return max(int(16)):int(16); else return max(int(64)):int(64);
  }
  param x = testParamProcCastMax(true);
  writeln("testParamProcCastMax: when true int(16) int(64) then ", x.type:string);
}

{
  proc testParamProcMax(param arg: bool) param {
    param i: int(16) = max(int(16));
    param u: int(64) = max(int(64));
    if arg then return i; else return u;
  }
  param x = testParamProcMax(true);
  writeln("testParamProcMax: when true int(16) int(64) then ", x.type:string);
}

{
  var b: bool = true;
  var x = if b then 0:int(16) else 0:int(64);
  writeln("testExpressionZero: when true int(16) int(64) then ", x.type:string);
}

{
  var b: bool = true;
  var x = if b then max(int(16)):int(16) else max(int(64)):int(64);
  writeln("testExpressionMax: when true int(16) int(64) then ", x.type:string);
}

{
  param x = if true then 0:int(16) else 0:int(64);
  writeln("testParamExpressionZero: when true int(16) int(64) then ", x.type:string);
}

{
  param x = if true then max(int(16)):int(16) else max(int(64)):int(64);
  writeln("testParamExpressionMax: when true int(16) int(64) then ", x.type:string);
}

{
  proc testProcCastZero(arg: bool) {
    if arg then return 0:int(16); else return 0:uint(8);
  }
  var x = testProcCastZero(true);
  writeln("testProcCastZero: when true int(16) uint(8) then ", x.type:string);
}

{
  proc testProcZero(arg: bool) {
    var i: int(16) = 0;
    var u: uint(8) = 0;
    if arg then return i; else return u;
  }
  var x = testProcZero(true);
  writeln("testProcZero: when true int(16) uint(8) then ", x.type:string);
}

{
  proc testProcCastMax(arg: bool) {
    if arg then return max(int(16)):int(16); else return max(uint(8)):uint(8);
  }
  var x = testProcCastMax(true);
  writeln("testProcCastMax: when true int(16) uint(8) then ", x.type:string);
}

{
  proc testProcMax(arg: bool) {
    var i: int(16) = max(int(16));
    var u: uint(8) = max(uint(8));
    if arg then return i; else return u;
  }
  var x = testProcMax(true);
  writeln("testProcMax: when true int(16) uint(8) then ", x.type:string);
}

{
  proc testParamProcCastZero(param arg: bool) param {
    if arg then return 0:int(16); else return 0:uint(8);
  }
  param x = testParamProcCastZero(true);
  writeln("testParamProcCastZero: when true int(16) uint(8) then ", x.type:string);
}

{
  proc testParamProcZero(param arg: bool) param {
    param i: int(16) = 0;
    param u: uint(8) = 0;
    if arg then return i; else return u;
  }
  param x = testParamProcZero(true);
  writeln("testParamProcZero: when true int(16) uint(8) then ", x.type:string);
}

{
  proc testParamProcCastMax(param arg: bool) param {
    if arg then return max(int(16)):int(16); else return max(uint(8)):uint(8);
  }
  param x = testParamProcCastMax(true);
  writeln("testParamProcCastMax: when true int(16) uint(8) then ", x.type:string);
}

{
  proc testParamProcMax(param arg: bool) param {
    param i: int(16) = max(int(16));
    param u: uint(8) = max(uint(8));
    if arg then return i; else return u;
  }
  param x = testParamProcMax(true);
  writeln("testParamProcMax: when true int(16) uint(8) then ", x.type:string);
}

{
  var b: bool = true;
  var x = if b then 0:int(16) else 0:uint(8);
  writeln("testExpressionZero: when true int(16) uint(8) then ", x.type:string);
}

{
  var b: bool = true;
  var x = if b then max(int(16)):int(16) else max(uint(8)):uint(8);
  writeln("testExpressionMax: when true int(16) uint(8) then ", x.type:string);
}

{
  param x = if true then 0:int(16) else 0:uint(8);
  writeln("testParamExpressionZero: when true int(16) uint(8) then ", x.type:string);
}

{
  param x = if true then max(int(16)):int(16) else max(uint(8)):uint(8);
  writeln("testParamExpressionMax: when true int(16) uint(8) then ", x.type:string);
}

{
  proc testProcCastZero(arg: bool) {
    if arg then return 0:int(16); else return 0:uint(16);
  }
  var x = testProcCastZero(true);
  writeln("testProcCastZero: when true int(16) uint(16) then ", x.type:string);
}

{
  proc testProcZero(arg: bool) {
    var i: int(16) = 0;
    var u: uint(16) = 0;
    if arg then return i; else return u;
  }
  var x = testProcZero(true);
  writeln("testProcZero: when true int(16) uint(16) then ", x.type:string);
}

{
  proc testProcCastMax(arg: bool) {
    if arg then return max(int(16)):int(16); else return max(uint(16)):uint(16);
  }
  var x = testProcCastMax(true);
  writeln("testProcCastMax: when true int(16) uint(16) then ", x.type:string);
}

{
  proc testProcMax(arg: bool) {
    var i: int(16) = max(int(16));
    var u: uint(16) = max(uint(16));
    if arg then return i; else return u;
  }
  var x = testProcMax(true);
  writeln("testProcMax: when true int(16) uint(16) then ", x.type:string);
}

{
  proc testParamProcCastZero(param arg: bool) param {
    if arg then return 0:int(16); else return 0:uint(16);
  }
  param x = testParamProcCastZero(true);
  writeln("testParamProcCastZero: when true int(16) uint(16) then ", x.type:string);
}

{
  proc testParamProcZero(param arg: bool) param {
    param i: int(16) = 0;
    param u: uint(16) = 0;
    if arg then return i; else return u;
  }
  param x = testParamProcZero(true);
  writeln("testParamProcZero: when true int(16) uint(16) then ", x.type:string);
}

{
  proc testParamProcCastMax(param arg: bool) param {
    if arg then return max(int(16)):int(16); else return max(uint(16)):uint(16);
  }
  param x = testParamProcCastMax(true);
  writeln("testParamProcCastMax: when true int(16) uint(16) then ", x.type:string);
}

{
  proc testParamProcMax(param arg: bool) param {
    param i: int(16) = max(int(16));
    param u: uint(16) = max(uint(16));
    if arg then return i; else return u;
  }
  param x = testParamProcMax(true);
  writeln("testParamProcMax: when true int(16) uint(16) then ", x.type:string);
}

{
  var b: bool = true;
  var x = if b then 0:int(16) else 0:uint(16);
  writeln("testExpressionZero: when true int(16) uint(16) then ", x.type:string);
}

{
  var b: bool = true;
  var x = if b then max(int(16)):int(16) else max(uint(16)):uint(16);
  writeln("testExpressionMax: when true int(16) uint(16) then ", x.type:string);
}

{
  param x = if true then 0:int(16) else 0:uint(16);
  writeln("testParamExpressionZero: when true int(16) uint(16) then ", x.type:string);
}

{
  param x = if true then max(int(16)):int(16) else max(uint(16)):uint(16);
  writeln("testParamExpressionMax: when true int(16) uint(16) then ", x.type:string);
}

{
  proc testProcCastZero(arg: bool) {
    if arg then return 0:int(16); else return 0:uint(32);
  }
  var x = testProcCastZero(true);
  writeln("testProcCastZero: when true int(16) uint(32) then ", x.type:string);
}

{
  proc testProcZero(arg: bool) {
    var i: int(16) = 0;
    var u: uint(32) = 0;
    if arg then return i; else return u;
  }
  var x = testProcZero(true);
  writeln("testProcZero: when true int(16) uint(32) then ", x.type:string);
}

{
  proc testProcCastMax(arg: bool) {
    if arg then return max(int(16)):int(16); else return max(uint(32)):uint(32);
  }
  var x = testProcCastMax(true);
  writeln("testProcCastMax: when true int(16) uint(32) then ", x.type:string);
}

{
  proc testProcMax(arg: bool) {
    var i: int(16) = max(int(16));
    var u: uint(32) = max(uint(32));
    if arg then return i; else return u;
  }
  var x = testProcMax(true);
  writeln("testProcMax: when true int(16) uint(32) then ", x.type:string);
}

{
  proc testParamProcCastZero(param arg: bool) param {
    if arg then return 0:int(16); else return 0:uint(32);
  }
  param x = testParamProcCastZero(true);
  writeln("testParamProcCastZero: when true int(16) uint(32) then ", x.type:string);
}

{
  proc testParamProcZero(param arg: bool) param {
    param i: int(16) = 0;
    param u: uint(32) = 0;
    if arg then return i; else return u;
  }
  param x = testParamProcZero(true);
  writeln("testParamProcZero: when true int(16) uint(32) then ", x.type:string);
}

{
  proc testParamProcCastMax(param arg: bool) param {
    if arg then return max(int(16)):int(16); else return max(uint(32)):uint(32);
  }
  param x = testParamProcCastMax(true);
  writeln("testParamProcCastMax: when true int(16) uint(32) then ", x.type:string);
}

{
  proc testParamProcMax(param arg: bool) param {
    param i: int(16) = max(int(16));
    param u: uint(32) = max(uint(32));
    if arg then return i; else return u;
  }
  param x = testParamProcMax(true);
  writeln("testParamProcMax: when true int(16) uint(32) then ", x.type:string);
}

{
  var b: bool = true;
  var x = if b then 0:int(16) else 0:uint(32);
  writeln("testExpressionZero: when true int(16) uint(32) then ", x.type:string);
}

{
  var b: bool = true;
  var x = if b then max(int(16)):int(16) else max(uint(32)):uint(32);
  writeln("testExpressionMax: when true int(16) uint(32) then ", x.type:string);
}

{
  param x = if true then 0:int(16) else 0:uint(32);
  writeln("testParamExpressionZero: when true int(16) uint(32) then ", x.type:string);
}

{
  param x = if true then max(int(16)):int(16) else max(uint(32)):uint(32);
  writeln("testParamExpressionMax: when true int(16) uint(32) then ", x.type:string);
}

{
  proc testProcCastZero(arg: bool) {
    if arg then return 0:int(16); else return 0:uint(64);
  }
  var x = testProcCastZero(true);
  writeln("testProcCastZero: when true int(16) uint(64) then ", x.type:string);
}

{
  proc testProcZero(arg: bool) {
    var i: int(16) = 0;
    var u: uint(64) = 0;
    if arg then return i; else return u;
  }
  var x = testProcZero(true);
  writeln("testProcZero: when true int(16) uint(64) then ", x.type:string);
}

{
  proc testProcCastMax(arg: bool) {
    if arg then return max(int(16)):int(16); else return max(uint(64)):uint(64);
  }
  var x = testProcCastMax(true);
  writeln("testProcCastMax: when true int(16) uint(64) then ", x.type:string);
}

{
  proc testProcMax(arg: bool) {
    var i: int(16) = max(int(16));
    var u: uint(64) = max(uint(64));
    if arg then return i; else return u;
  }
  var x = testProcMax(true);
  writeln("testProcMax: when true int(16) uint(64) then ", x.type:string);
}

{
  proc testParamProcCastZero(param arg: bool) param {
    if arg then return 0:int(16); else return 0:uint(64);
  }
  param x = testParamProcCastZero(true);
  writeln("testParamProcCastZero: when true int(16) uint(64) then ", x.type:string);
}

{
  proc testParamProcZero(param arg: bool) param {
    param i: int(16) = 0;
    param u: uint(64) = 0;
    if arg then return i; else return u;
  }
  param x = testParamProcZero(true);
  writeln("testParamProcZero: when true int(16) uint(64) then ", x.type:string);
}

{
  proc testParamProcCastMax(param arg: bool) param {
    if arg then return max(int(16)):int(16); else return max(uint(64)):uint(64);
  }
  param x = testParamProcCastMax(true);
  writeln("testParamProcCastMax: when true int(16) uint(64) then ", x.type:string);
}

{
  proc testParamProcMax(param arg: bool) param {
    param i: int(16) = max(int(16));
    param u: uint(64) = max(uint(64));
    if arg then return i; else return u;
  }
  param x = testParamProcMax(true);
  writeln("testParamProcMax: when true int(16) uint(64) then ", x.type:string);
}

{
  var b: bool = true;
  var x = if b then 0:int(16) else 0:uint(64);
  writeln("testExpressionZero: when true int(16) uint(64) then ", x.type:string);
}

{
  var b: bool = true;
  var x = if b then max(int(16)):int(16) else max(uint(64)):uint(64);
  writeln("testExpressionMax: when true int(16) uint(64) then ", x.type:string);
}

{
  param x = if true then 0:int(16) else 0:uint(64);
  writeln("testParamExpressionZero: when true int(16) uint(64) then ", x.type:string);
}

{
  param x = if true then max(int(16)):int(16) else max(uint(64)):uint(64);
  writeln("testParamExpressionMax: when true int(16) uint(64) then ", x.type:string);
}

{
  proc testProcCastZero(arg: bool) {
    if arg then return 0:int(32); else return 0:int(8);
  }
  var x = testProcCastZero(true);
  writeln("testProcCastZero: when true int(32) int(8) then ", x.type:string);
}

{
  proc testProcZero(arg: bool) {
    var i: int(32) = 0;
    var u: int(8) = 0;
    if arg then return i; else return u;
  }
  var x = testProcZero(true);
  writeln("testProcZero: when true int(32) int(8) then ", x.type:string);
}

{
  proc testProcCastMax(arg: bool) {
    if arg then return max(int(32)):int(32); else return max(int(8)):int(8);
  }
  var x = testProcCastMax(true);
  writeln("testProcCastMax: when true int(32) int(8) then ", x.type:string);
}

{
  proc testProcMax(arg: bool) {
    var i: int(32) = max(int(32));
    var u: int(8) = max(int(8));
    if arg then return i; else return u;
  }
  var x = testProcMax(true);
  writeln("testProcMax: when true int(32) int(8) then ", x.type:string);
}

{
  proc testParamProcCastZero(param arg: bool) param {
    if arg then return 0:int(32); else return 0:int(8);
  }
  param x = testParamProcCastZero(true);
  writeln("testParamProcCastZero: when true int(32) int(8) then ", x.type:string);
}

{
  proc testParamProcZero(param arg: bool) param {
    param i: int(32) = 0;
    param u: int(8) = 0;
    if arg then return i; else return u;
  }
  param x = testParamProcZero(true);
  writeln("testParamProcZero: when true int(32) int(8) then ", x.type:string);
}

{
  proc testParamProcCastMax(param arg: bool) param {
    if arg then return max(int(32)):int(32); else return max(int(8)):int(8);
  }
  param x = testParamProcCastMax(true);
  writeln("testParamProcCastMax: when true int(32) int(8) then ", x.type:string);
}

{
  proc testParamProcMax(param arg: bool) param {
    param i: int(32) = max(int(32));
    param u: int(8) = max(int(8));
    if arg then return i; else return u;
  }
  param x = testParamProcMax(true);
  writeln("testParamProcMax: when true int(32) int(8) then ", x.type:string);
}

{
  var b: bool = true;
  var x = if b then 0:int(32) else 0:int(8);
  writeln("testExpressionZero: when true int(32) int(8) then ", x.type:string);
}

{
  var b: bool = true;
  var x = if b then max(int(32)):int(32) else max(int(8)):int(8);
  writeln("testExpressionMax: when true int(32) int(8) then ", x.type:string);
}

{
  param x = if true then 0:int(32) else 0:int(8);
  writeln("testParamExpressionZero: when true int(32) int(8) then ", x.type:string);
}

{
  param x = if true then max(int(32)):int(32) else max(int(8)):int(8);
  writeln("testParamExpressionMax: when true int(32) int(8) then ", x.type:string);
}

{
  proc testProcCastZero(arg: bool) {
    if arg then return 0:int(32); else return 0:int(16);
  }
  var x = testProcCastZero(true);
  writeln("testProcCastZero: when true int(32) int(16) then ", x.type:string);
}

{
  proc testProcZero(arg: bool) {
    var i: int(32) = 0;
    var u: int(16) = 0;
    if arg then return i; else return u;
  }
  var x = testProcZero(true);
  writeln("testProcZero: when true int(32) int(16) then ", x.type:string);
}

{
  proc testProcCastMax(arg: bool) {
    if arg then return max(int(32)):int(32); else return max(int(16)):int(16);
  }
  var x = testProcCastMax(true);
  writeln("testProcCastMax: when true int(32) int(16) then ", x.type:string);
}

{
  proc testProcMax(arg: bool) {
    var i: int(32) = max(int(32));
    var u: int(16) = max(int(16));
    if arg then return i; else return u;
  }
  var x = testProcMax(true);
  writeln("testProcMax: when true int(32) int(16) then ", x.type:string);
}

{
  proc testParamProcCastZero(param arg: bool) param {
    if arg then return 0:int(32); else return 0:int(16);
  }
  param x = testParamProcCastZero(true);
  writeln("testParamProcCastZero: when true int(32) int(16) then ", x.type:string);
}

{
  proc testParamProcZero(param arg: bool) param {
    param i: int(32) = 0;
    param u: int(16) = 0;
    if arg then return i; else return u;
  }
  param x = testParamProcZero(true);
  writeln("testParamProcZero: when true int(32) int(16) then ", x.type:string);
}

{
  proc testParamProcCastMax(param arg: bool) param {
    if arg then return max(int(32)):int(32); else return max(int(16)):int(16);
  }
  param x = testParamProcCastMax(true);
  writeln("testParamProcCastMax: when true int(32) int(16) then ", x.type:string);
}

{
  proc testParamProcMax(param arg: bool) param {
    param i: int(32) = max(int(32));
    param u: int(16) = max(int(16));
    if arg then return i; else return u;
  }
  param x = testParamProcMax(true);
  writeln("testParamProcMax: when true int(32) int(16) then ", x.type:string);
}

{
  var b: bool = true;
  var x = if b then 0:int(32) else 0:int(16);
  writeln("testExpressionZero: when true int(32) int(16) then ", x.type:string);
}

{
  var b: bool = true;
  var x = if b then max(int(32)):int(32) else max(int(16)):int(16);
  writeln("testExpressionMax: when true int(32) int(16) then ", x.type:string);
}

{
  param x = if true then 0:int(32) else 0:int(16);
  writeln("testParamExpressionZero: when true int(32) int(16) then ", x.type:string);
}

{
  param x = if true then max(int(32)):int(32) else max(int(16)):int(16);
  writeln("testParamExpressionMax: when true int(32) int(16) then ", x.type:string);
}

{
  proc testProcCastZero(arg: bool) {
    if arg then return 0:int(32); else return 0:int(32);
  }
  var x = testProcCastZero(true);
  writeln("testProcCastZero: when true int(32) int(32) then ", x.type:string);
}

{
  proc testProcZero(arg: bool) {
    var i: int(32) = 0;
    var u: int(32) = 0;
    if arg then return i; else return u;
  }
  var x = testProcZero(true);
  writeln("testProcZero: when true int(32) int(32) then ", x.type:string);
}

{
  proc testProcCastMax(arg: bool) {
    if arg then return max(int(32)):int(32); else return max(int(32)):int(32);
  }
  var x = testProcCastMax(true);
  writeln("testProcCastMax: when true int(32) int(32) then ", x.type:string);
}

{
  proc testProcMax(arg: bool) {
    var i: int(32) = max(int(32));
    var u: int(32) = max(int(32));
    if arg then return i; else return u;
  }
  var x = testProcMax(true);
  writeln("testProcMax: when true int(32) int(32) then ", x.type:string);
}

{
  proc testParamProcCastZero(param arg: bool) param {
    if arg then return 0:int(32); else return 0:int(32);
  }
  param x = testParamProcCastZero(true);
  writeln("testParamProcCastZero: when true int(32) int(32) then ", x.type:string);
}

{
  proc testParamProcZero(param arg: bool) param {
    param i: int(32) = 0;
    param u: int(32) = 0;
    if arg then return i; else return u;
  }
  param x = testParamProcZero(true);
  writeln("testParamProcZero: when true int(32) int(32) then ", x.type:string);
}

{
  proc testParamProcCastMax(param arg: bool) param {
    if arg then return max(int(32)):int(32); else return max(int(32)):int(32);
  }
  param x = testParamProcCastMax(true);
  writeln("testParamProcCastMax: when true int(32) int(32) then ", x.type:string);
}

{
  proc testParamProcMax(param arg: bool) param {
    param i: int(32) = max(int(32));
    param u: int(32) = max(int(32));
    if arg then return i; else return u;
  }
  param x = testParamProcMax(true);
  writeln("testParamProcMax: when true int(32) int(32) then ", x.type:string);
}

{
  var b: bool = true;
  var x = if b then 0:int(32) else 0:int(32);
  writeln("testExpressionZero: when true int(32) int(32) then ", x.type:string);
}

{
  var b: bool = true;
  var x = if b then max(int(32)):int(32) else max(int(32)):int(32);
  writeln("testExpressionMax: when true int(32) int(32) then ", x.type:string);
}

{
  param x = if true then 0:int(32) else 0:int(32);
  writeln("testParamExpressionZero: when true int(32) int(32) then ", x.type:string);
}

{
  param x = if true then max(int(32)):int(32) else max(int(32)):int(32);
  writeln("testParamExpressionMax: when true int(32) int(32) then ", x.type:string);
}

{
  proc testProcCastZero(arg: bool) {
    if arg then return 0:int(32); else return 0:int(64);
  }
  var x = testProcCastZero(true);
  writeln("testProcCastZero: when true int(32) int(64) then ", x.type:string);
}

{
  proc testProcZero(arg: bool) {
    var i: int(32) = 0;
    var u: int(64) = 0;
    if arg then return i; else return u;
  }
  var x = testProcZero(true);
  writeln("testProcZero: when true int(32) int(64) then ", x.type:string);
}

{
  proc testProcCastMax(arg: bool) {
    if arg then return max(int(32)):int(32); else return max(int(64)):int(64);
  }
  var x = testProcCastMax(true);
  writeln("testProcCastMax: when true int(32) int(64) then ", x.type:string);
}

{
  proc testProcMax(arg: bool) {
    var i: int(32) = max(int(32));
    var u: int(64) = max(int(64));
    if arg then return i; else return u;
  }
  var x = testProcMax(true);
  writeln("testProcMax: when true int(32) int(64) then ", x.type:string);
}

{
  proc testParamProcCastZero(param arg: bool) param {
    if arg then return 0:int(32); else return 0:int(64);
  }
  param x = testParamProcCastZero(true);
  writeln("testParamProcCastZero: when true int(32) int(64) then ", x.type:string);
}

{
  proc testParamProcZero(param arg: bool) param {
    param i: int(32) = 0;
    param u: int(64) = 0;
    if arg then return i; else return u;
  }
  param x = testParamProcZero(true);
  writeln("testParamProcZero: when true int(32) int(64) then ", x.type:string);
}

{
  proc testParamProcCastMax(param arg: bool) param {
    if arg then return max(int(32)):int(32); else return max(int(64)):int(64);
  }
  param x = testParamProcCastMax(true);
  writeln("testParamProcCastMax: when true int(32) int(64) then ", x.type:string);
}

{
  proc testParamProcMax(param arg: bool) param {
    param i: int(32) = max(int(32));
    param u: int(64) = max(int(64));
    if arg then return i; else return u;
  }
  param x = testParamProcMax(true);
  writeln("testParamProcMax: when true int(32) int(64) then ", x.type:string);
}

{
  var b: bool = true;
  var x = if b then 0:int(32) else 0:int(64);
  writeln("testExpressionZero: when true int(32) int(64) then ", x.type:string);
}

{
  var b: bool = true;
  var x = if b then max(int(32)):int(32) else max(int(64)):int(64);
  writeln("testExpressionMax: when true int(32) int(64) then ", x.type:string);
}

{
  param x = if true then 0:int(32) else 0:int(64);
  writeln("testParamExpressionZero: when true int(32) int(64) then ", x.type:string);
}

{
  param x = if true then max(int(32)):int(32) else max(int(64)):int(64);
  writeln("testParamExpressionMax: when true int(32) int(64) then ", x.type:string);
}

{
  proc testProcCastZero(arg: bool) {
    if arg then return 0:int(32); else return 0:uint(8);
  }
  var x = testProcCastZero(true);
  writeln("testProcCastZero: when true int(32) uint(8) then ", x.type:string);
}

{
  proc testProcZero(arg: bool) {
    var i: int(32) = 0;
    var u: uint(8) = 0;
    if arg then return i; else return u;
  }
  var x = testProcZero(true);
  writeln("testProcZero: when true int(32) uint(8) then ", x.type:string);
}

{
  proc testProcCastMax(arg: bool) {
    if arg then return max(int(32)):int(32); else return max(uint(8)):uint(8);
  }
  var x = testProcCastMax(true);
  writeln("testProcCastMax: when true int(32) uint(8) then ", x.type:string);
}

{
  proc testProcMax(arg: bool) {
    var i: int(32) = max(int(32));
    var u: uint(8) = max(uint(8));
    if arg then return i; else return u;
  }
  var x = testProcMax(true);
  writeln("testProcMax: when true int(32) uint(8) then ", x.type:string);
}

{
  proc testParamProcCastZero(param arg: bool) param {
    if arg then return 0:int(32); else return 0:uint(8);
  }
  param x = testParamProcCastZero(true);
  writeln("testParamProcCastZero: when true int(32) uint(8) then ", x.type:string);
}

{
  proc testParamProcZero(param arg: bool) param {
    param i: int(32) = 0;
    param u: uint(8) = 0;
    if arg then return i; else return u;
  }
  param x = testParamProcZero(true);
  writeln("testParamProcZero: when true int(32) uint(8) then ", x.type:string);
}

{
  proc testParamProcCastMax(param arg: bool) param {
    if arg then return max(int(32)):int(32); else return max(uint(8)):uint(8);
  }
  param x = testParamProcCastMax(true);
  writeln("testParamProcCastMax: when true int(32) uint(8) then ", x.type:string);
}

{
  proc testParamProcMax(param arg: bool) param {
    param i: int(32) = max(int(32));
    param u: uint(8) = max(uint(8));
    if arg then return i; else return u;
  }
  param x = testParamProcMax(true);
  writeln("testParamProcMax: when true int(32) uint(8) then ", x.type:string);
}

{
  var b: bool = true;
  var x = if b then 0:int(32) else 0:uint(8);
  writeln("testExpressionZero: when true int(32) uint(8) then ", x.type:string);
}

{
  var b: bool = true;
  var x = if b then max(int(32)):int(32) else max(uint(8)):uint(8);
  writeln("testExpressionMax: when true int(32) uint(8) then ", x.type:string);
}

{
  param x = if true then 0:int(32) else 0:uint(8);
  writeln("testParamExpressionZero: when true int(32) uint(8) then ", x.type:string);
}

{
  param x = if true then max(int(32)):int(32) else max(uint(8)):uint(8);
  writeln("testParamExpressionMax: when true int(32) uint(8) then ", x.type:string);
}

{
  proc testProcCastZero(arg: bool) {
    if arg then return 0:int(32); else return 0:uint(16);
  }
  var x = testProcCastZero(true);
  writeln("testProcCastZero: when true int(32) uint(16) then ", x.type:string);
}

{
  proc testProcZero(arg: bool) {
    var i: int(32) = 0;
    var u: uint(16) = 0;
    if arg then return i; else return u;
  }
  var x = testProcZero(true);
  writeln("testProcZero: when true int(32) uint(16) then ", x.type:string);
}

{
  proc testProcCastMax(arg: bool) {
    if arg then return max(int(32)):int(32); else return max(uint(16)):uint(16);
  }
  var x = testProcCastMax(true);
  writeln("testProcCastMax: when true int(32) uint(16) then ", x.type:string);
}

{
  proc testProcMax(arg: bool) {
    var i: int(32) = max(int(32));
    var u: uint(16) = max(uint(16));
    if arg then return i; else return u;
  }
  var x = testProcMax(true);
  writeln("testProcMax: when true int(32) uint(16) then ", x.type:string);
}

{
  proc testParamProcCastZero(param arg: bool) param {
    if arg then return 0:int(32); else return 0:uint(16);
  }
  param x = testParamProcCastZero(true);
  writeln("testParamProcCastZero: when true int(32) uint(16) then ", x.type:string);
}

{
  proc testParamProcZero(param arg: bool) param {
    param i: int(32) = 0;
    param u: uint(16) = 0;
    if arg then return i; else return u;
  }
  param x = testParamProcZero(true);
  writeln("testParamProcZero: when true int(32) uint(16) then ", x.type:string);
}

{
  proc testParamProcCastMax(param arg: bool) param {
    if arg then return max(int(32)):int(32); else return max(uint(16)):uint(16);
  }
  param x = testParamProcCastMax(true);
  writeln("testParamProcCastMax: when true int(32) uint(16) then ", x.type:string);
}

{
  proc testParamProcMax(param arg: bool) param {
    param i: int(32) = max(int(32));
    param u: uint(16) = max(uint(16));
    if arg then return i; else return u;
  }
  param x = testParamProcMax(true);
  writeln("testParamProcMax: when true int(32) uint(16) then ", x.type:string);
}

{
  var b: bool = true;
  var x = if b then 0:int(32) else 0:uint(16);
  writeln("testExpressionZero: when true int(32) uint(16) then ", x.type:string);
}

{
  var b: bool = true;
  var x = if b then max(int(32)):int(32) else max(uint(16)):uint(16);
  writeln("testExpressionMax: when true int(32) uint(16) then ", x.type:string);
}

{
  param x = if true then 0:int(32) else 0:uint(16);
  writeln("testParamExpressionZero: when true int(32) uint(16) then ", x.type:string);
}

{
  param x = if true then max(int(32)):int(32) else max(uint(16)):uint(16);
  writeln("testParamExpressionMax: when true int(32) uint(16) then ", x.type:string);
}

{
  proc testProcCastZero(arg: bool) {
    if arg then return 0:int(32); else return 0:uint(32);
  }
  var x = testProcCastZero(true);
  writeln("testProcCastZero: when true int(32) uint(32) then ", x.type:string);
}

{
  proc testProcZero(arg: bool) {
    var i: int(32) = 0;
    var u: uint(32) = 0;
    if arg then return i; else return u;
  }
  var x = testProcZero(true);
  writeln("testProcZero: when true int(32) uint(32) then ", x.type:string);
}

{
  proc testProcCastMax(arg: bool) {
    if arg then return max(int(32)):int(32); else return max(uint(32)):uint(32);
  }
  var x = testProcCastMax(true);
  writeln("testProcCastMax: when true int(32) uint(32) then ", x.type:string);
}

{
  proc testProcMax(arg: bool) {
    var i: int(32) = max(int(32));
    var u: uint(32) = max(uint(32));
    if arg then return i; else return u;
  }
  var x = testProcMax(true);
  writeln("testProcMax: when true int(32) uint(32) then ", x.type:string);
}

{
  proc testParamProcCastZero(param arg: bool) param {
    if arg then return 0:int(32); else return 0:uint(32);
  }
  param x = testParamProcCastZero(true);
  writeln("testParamProcCastZero: when true int(32) uint(32) then ", x.type:string);
}

{
  proc testParamProcZero(param arg: bool) param {
    param i: int(32) = 0;
    param u: uint(32) = 0;
    if arg then return i; else return u;
  }
  param x = testParamProcZero(true);
  writeln("testParamProcZero: when true int(32) uint(32) then ", x.type:string);
}

{
  proc testParamProcCastMax(param arg: bool) param {
    if arg then return max(int(32)):int(32); else return max(uint(32)):uint(32);
  }
  param x = testParamProcCastMax(true);
  writeln("testParamProcCastMax: when true int(32) uint(32) then ", x.type:string);
}

{
  proc testParamProcMax(param arg: bool) param {
    param i: int(32) = max(int(32));
    param u: uint(32) = max(uint(32));
    if arg then return i; else return u;
  }
  param x = testParamProcMax(true);
  writeln("testParamProcMax: when true int(32) uint(32) then ", x.type:string);
}

{
  var b: bool = true;
  var x = if b then 0:int(32) else 0:uint(32);
  writeln("testExpressionZero: when true int(32) uint(32) then ", x.type:string);
}

{
  var b: bool = true;
  var x = if b then max(int(32)):int(32) else max(uint(32)):uint(32);
  writeln("testExpressionMax: when true int(32) uint(32) then ", x.type:string);
}

{
  param x = if true then 0:int(32) else 0:uint(32);
  writeln("testParamExpressionZero: when true int(32) uint(32) then ", x.type:string);
}

{
  param x = if true then max(int(32)):int(32) else max(uint(32)):uint(32);
  writeln("testParamExpressionMax: when true int(32) uint(32) then ", x.type:string);
}

{
  proc testProcCastZero(arg: bool) {
    if arg then return 0:int(32); else return 0:uint(64);
  }
  var x = testProcCastZero(true);
  writeln("testProcCastZero: when true int(32) uint(64) then ", x.type:string);
}

{
  proc testProcZero(arg: bool) {
    var i: int(32) = 0;
    var u: uint(64) = 0;
    if arg then return i; else return u;
  }
  var x = testProcZero(true);
  writeln("testProcZero: when true int(32) uint(64) then ", x.type:string);
}

{
  proc testProcCastMax(arg: bool) {
    if arg then return max(int(32)):int(32); else return max(uint(64)):uint(64);
  }
  var x = testProcCastMax(true);
  writeln("testProcCastMax: when true int(32) uint(64) then ", x.type:string);
}

{
  proc testProcMax(arg: bool) {
    var i: int(32) = max(int(32));
    var u: uint(64) = max(uint(64));
    if arg then return i; else return u;
  }
  var x = testProcMax(true);
  writeln("testProcMax: when true int(32) uint(64) then ", x.type:string);
}

{
  proc testParamProcCastZero(param arg: bool) param {
    if arg then return 0:int(32); else return 0:uint(64);
  }
  param x = testParamProcCastZero(true);
  writeln("testParamProcCastZero: when true int(32) uint(64) then ", x.type:string);
}

{
  proc testParamProcZero(param arg: bool) param {
    param i: int(32) = 0;
    param u: uint(64) = 0;
    if arg then return i; else return u;
  }
  param x = testParamProcZero(true);
  writeln("testParamProcZero: when true int(32) uint(64) then ", x.type:string);
}

{
  proc testParamProcCastMax(param arg: bool) param {
    if arg then return max(int(32)):int(32); else return max(uint(64)):uint(64);
  }
  param x = testParamProcCastMax(true);
  writeln("testParamProcCastMax: when true int(32) uint(64) then ", x.type:string);
}

{
  proc testParamProcMax(param arg: bool) param {
    param i: int(32) = max(int(32));
    param u: uint(64) = max(uint(64));
    if arg then return i; else return u;
  }
  param x = testParamProcMax(true);
  writeln("testParamProcMax: when true int(32) uint(64) then ", x.type:string);
}

{
  var b: bool = true;
  var x = if b then 0:int(32) else 0:uint(64);
  writeln("testExpressionZero: when true int(32) uint(64) then ", x.type:string);
}

{
  var b: bool = true;
  var x = if b then max(int(32)):int(32) else max(uint(64)):uint(64);
  writeln("testExpressionMax: when true int(32) uint(64) then ", x.type:string);
}

{
  param x = if true then 0:int(32) else 0:uint(64);
  writeln("testParamExpressionZero: when true int(32) uint(64) then ", x.type:string);
}

{
  param x = if true then max(int(32)):int(32) else max(uint(64)):uint(64);
  writeln("testParamExpressionMax: when true int(32) uint(64) then ", x.type:string);
}

{
  proc testProcCastZero(arg: bool) {
    if arg then return 0:int(64); else return 0:int(8);
  }
  var x = testProcCastZero(true);
  writeln("testProcCastZero: when true int(64) int(8) then ", x.type:string);
}

{
  proc testProcZero(arg: bool) {
    var i: int(64) = 0;
    var u: int(8) = 0;
    if arg then return i; else return u;
  }
  var x = testProcZero(true);
  writeln("testProcZero: when true int(64) int(8) then ", x.type:string);
}

{
  proc testProcCastMax(arg: bool) {
    if arg then return max(int(64)):int(64); else return max(int(8)):int(8);
  }
  var x = testProcCastMax(true);
  writeln("testProcCastMax: when true int(64) int(8) then ", x.type:string);
}

{
  proc testProcMax(arg: bool) {
    var i: int(64) = max(int(64));
    var u: int(8) = max(int(8));
    if arg then return i; else return u;
  }
  var x = testProcMax(true);
  writeln("testProcMax: when true int(64) int(8) then ", x.type:string);
}

{
  proc testParamProcCastZero(param arg: bool) param {
    if arg then return 0:int(64); else return 0:int(8);
  }
  param x = testParamProcCastZero(true);
  writeln("testParamProcCastZero: when true int(64) int(8) then ", x.type:string);
}

{
  proc testParamProcZero(param arg: bool) param {
    param i: int(64) = 0;
    param u: int(8) = 0;
    if arg then return i; else return u;
  }
  param x = testParamProcZero(true);
  writeln("testParamProcZero: when true int(64) int(8) then ", x.type:string);
}

{
  proc testParamProcCastMax(param arg: bool) param {
    if arg then return max(int(64)):int(64); else return max(int(8)):int(8);
  }
  param x = testParamProcCastMax(true);
  writeln("testParamProcCastMax: when true int(64) int(8) then ", x.type:string);
}

{
  proc testParamProcMax(param arg: bool) param {
    param i: int(64) = max(int(64));
    param u: int(8) = max(int(8));
    if arg then return i; else return u;
  }
  param x = testParamProcMax(true);
  writeln("testParamProcMax: when true int(64) int(8) then ", x.type:string);
}

{
  var b: bool = true;
  var x = if b then 0:int(64) else 0:int(8);
  writeln("testExpressionZero: when true int(64) int(8) then ", x.type:string);
}

{
  var b: bool = true;
  var x = if b then max(int(64)):int(64) else max(int(8)):int(8);
  writeln("testExpressionMax: when true int(64) int(8) then ", x.type:string);
}

{
  param x = if true then 0:int(64) else 0:int(8);
  writeln("testParamExpressionZero: when true int(64) int(8) then ", x.type:string);
}

{
  param x = if true then max(int(64)):int(64) else max(int(8)):int(8);
  writeln("testParamExpressionMax: when true int(64) int(8) then ", x.type:string);
}

{
  proc testProcCastZero(arg: bool) {
    if arg then return 0:int(64); else return 0:int(16);
  }
  var x = testProcCastZero(true);
  writeln("testProcCastZero: when true int(64) int(16) then ", x.type:string);
}

{
  proc testProcZero(arg: bool) {
    var i: int(64) = 0;
    var u: int(16) = 0;
    if arg then return i; else return u;
  }
  var x = testProcZero(true);
  writeln("testProcZero: when true int(64) int(16) then ", x.type:string);
}

{
  proc testProcCastMax(arg: bool) {
    if arg then return max(int(64)):int(64); else return max(int(16)):int(16);
  }
  var x = testProcCastMax(true);
  writeln("testProcCastMax: when true int(64) int(16) then ", x.type:string);
}

{
  proc testProcMax(arg: bool) {
    var i: int(64) = max(int(64));
    var u: int(16) = max(int(16));
    if arg then return i; else return u;
  }
  var x = testProcMax(true);
  writeln("testProcMax: when true int(64) int(16) then ", x.type:string);
}

{
  proc testParamProcCastZero(param arg: bool) param {
    if arg then return 0:int(64); else return 0:int(16);
  }
  param x = testParamProcCastZero(true);
  writeln("testParamProcCastZero: when true int(64) int(16) then ", x.type:string);
}

{
  proc testParamProcZero(param arg: bool) param {
    param i: int(64) = 0;
    param u: int(16) = 0;
    if arg then return i; else return u;
  }
  param x = testParamProcZero(true);
  writeln("testParamProcZero: when true int(64) int(16) then ", x.type:string);
}

{
  proc testParamProcCastMax(param arg: bool) param {
    if arg then return max(int(64)):int(64); else return max(int(16)):int(16);
  }
  param x = testParamProcCastMax(true);
  writeln("testParamProcCastMax: when true int(64) int(16) then ", x.type:string);
}

{
  proc testParamProcMax(param arg: bool) param {
    param i: int(64) = max(int(64));
    param u: int(16) = max(int(16));
    if arg then return i; else return u;
  }
  param x = testParamProcMax(true);
  writeln("testParamProcMax: when true int(64) int(16) then ", x.type:string);
}

{
  var b: bool = true;
  var x = if b then 0:int(64) else 0:int(16);
  writeln("testExpressionZero: when true int(64) int(16) then ", x.type:string);
}

{
  var b: bool = true;
  var x = if b then max(int(64)):int(64) else max(int(16)):int(16);
  writeln("testExpressionMax: when true int(64) int(16) then ", x.type:string);
}

{
  param x = if true then 0:int(64) else 0:int(16);
  writeln("testParamExpressionZero: when true int(64) int(16) then ", x.type:string);
}

{
  param x = if true then max(int(64)):int(64) else max(int(16)):int(16);
  writeln("testParamExpressionMax: when true int(64) int(16) then ", x.type:string);
}

{
  proc testProcCastZero(arg: bool) {
    if arg then return 0:int(64); else return 0:int(32);
  }
  var x = testProcCastZero(true);
  writeln("testProcCastZero: when true int(64) int(32) then ", x.type:string);
}

{
  proc testProcZero(arg: bool) {
    var i: int(64) = 0;
    var u: int(32) = 0;
    if arg then return i; else return u;
  }
  var x = testProcZero(true);
  writeln("testProcZero: when true int(64) int(32) then ", x.type:string);
}

{
  proc testProcCastMax(arg: bool) {
    if arg then return max(int(64)):int(64); else return max(int(32)):int(32);
  }
  var x = testProcCastMax(true);
  writeln("testProcCastMax: when true int(64) int(32) then ", x.type:string);
}

{
  proc testProcMax(arg: bool) {
    var i: int(64) = max(int(64));
    var u: int(32) = max(int(32));
    if arg then return i; else return u;
  }
  var x = testProcMax(true);
  writeln("testProcMax: when true int(64) int(32) then ", x.type:string);
}

{
  proc testParamProcCastZero(param arg: bool) param {
    if arg then return 0:int(64); else return 0:int(32);
  }
  param x = testParamProcCastZero(true);
  writeln("testParamProcCastZero: when true int(64) int(32) then ", x.type:string);
}

{
  proc testParamProcZero(param arg: bool) param {
    param i: int(64) = 0;
    param u: int(32) = 0;
    if arg then return i; else return u;
  }
  param x = testParamProcZero(true);
  writeln("testParamProcZero: when true int(64) int(32) then ", x.type:string);
}

{
  proc testParamProcCastMax(param arg: bool) param {
    if arg then return max(int(64)):int(64); else return max(int(32)):int(32);
  }
  param x = testParamProcCastMax(true);
  writeln("testParamProcCastMax: when true int(64) int(32) then ", x.type:string);
}

{
  proc testParamProcMax(param arg: bool) param {
    param i: int(64) = max(int(64));
    param u: int(32) = max(int(32));
    if arg then return i; else return u;
  }
  param x = testParamProcMax(true);
  writeln("testParamProcMax: when true int(64) int(32) then ", x.type:string);
}

{
  var b: bool = true;
  var x = if b then 0:int(64) else 0:int(32);
  writeln("testExpressionZero: when true int(64) int(32) then ", x.type:string);
}

{
  var b: bool = true;
  var x = if b then max(int(64)):int(64) else max(int(32)):int(32);
  writeln("testExpressionMax: when true int(64) int(32) then ", x.type:string);
}

{
  param x = if true then 0:int(64) else 0:int(32);
  writeln("testParamExpressionZero: when true int(64) int(32) then ", x.type:string);
}

{
  param x = if true then max(int(64)):int(64) else max(int(32)):int(32);
  writeln("testParamExpressionMax: when true int(64) int(32) then ", x.type:string);
}

{
  proc testProcCastZero(arg: bool) {
    if arg then return 0:int(64); else return 0:int(64);
  }
  var x = testProcCastZero(true);
  writeln("testProcCastZero: when true int(64) int(64) then ", x.type:string);
}

{
  proc testProcZero(arg: bool) {
    var i: int(64) = 0;
    var u: int(64) = 0;
    if arg then return i; else return u;
  }
  var x = testProcZero(true);
  writeln("testProcZero: when true int(64) int(64) then ", x.type:string);
}

{
  proc testProcCastMax(arg: bool) {
    if arg then return max(int(64)):int(64); else return max(int(64)):int(64);
  }
  var x = testProcCastMax(true);
  writeln("testProcCastMax: when true int(64) int(64) then ", x.type:string);
}

{
  proc testProcMax(arg: bool) {
    var i: int(64) = max(int(64));
    var u: int(64) = max(int(64));
    if arg then return i; else return u;
  }
  var x = testProcMax(true);
  writeln("testProcMax: when true int(64) int(64) then ", x.type:string);
}

{
  proc testParamProcCastZero(param arg: bool) param {
    if arg then return 0:int(64); else return 0:int(64);
  }
  param x = testParamProcCastZero(true);
  writeln("testParamProcCastZero: when true int(64) int(64) then ", x.type:string);
}

{
  proc testParamProcZero(param arg: bool) param {
    param i: int(64) = 0;
    param u: int(64) = 0;
    if arg then return i; else return u;
  }
  param x = testParamProcZero(true);
  writeln("testParamProcZero: when true int(64) int(64) then ", x.type:string);
}

{
  proc testParamProcCastMax(param arg: bool) param {
    if arg then return max(int(64)):int(64); else return max(int(64)):int(64);
  }
  param x = testParamProcCastMax(true);
  writeln("testParamProcCastMax: when true int(64) int(64) then ", x.type:string);
}

{
  proc testParamProcMax(param arg: bool) param {
    param i: int(64) = max(int(64));
    param u: int(64) = max(int(64));
    if arg then return i; else return u;
  }
  param x = testParamProcMax(true);
  writeln("testParamProcMax: when true int(64) int(64) then ", x.type:string);
}

{
  var b: bool = true;
  var x = if b then 0:int(64) else 0:int(64);
  writeln("testExpressionZero: when true int(64) int(64) then ", x.type:string);
}

{
  var b: bool = true;
  var x = if b then max(int(64)):int(64) else max(int(64)):int(64);
  writeln("testExpressionMax: when true int(64) int(64) then ", x.type:string);
}

{
  param x = if true then 0:int(64) else 0:int(64);
  writeln("testParamExpressionZero: when true int(64) int(64) then ", x.type:string);
}

{
  param x = if true then max(int(64)):int(64) else max(int(64)):int(64);
  writeln("testParamExpressionMax: when true int(64) int(64) then ", x.type:string);
}

{
  proc testProcCastZero(arg: bool) {
    if arg then return 0:int(64); else return 0:uint(8);
  }
  var x = testProcCastZero(true);
  writeln("testProcCastZero: when true int(64) uint(8) then ", x.type:string);
}

{
  proc testProcZero(arg: bool) {
    var i: int(64) = 0;
    var u: uint(8) = 0;
    if arg then return i; else return u;
  }
  var x = testProcZero(true);
  writeln("testProcZero: when true int(64) uint(8) then ", x.type:string);
}

{
  proc testProcCastMax(arg: bool) {
    if arg then return max(int(64)):int(64); else return max(uint(8)):uint(8);
  }
  var x = testProcCastMax(true);
  writeln("testProcCastMax: when true int(64) uint(8) then ", x.type:string);
}

{
  proc testProcMax(arg: bool) {
    var i: int(64) = max(int(64));
    var u: uint(8) = max(uint(8));
    if arg then return i; else return u;
  }
  var x = testProcMax(true);
  writeln("testProcMax: when true int(64) uint(8) then ", x.type:string);
}

{
  proc testParamProcCastZero(param arg: bool) param {
    if arg then return 0:int(64); else return 0:uint(8);
  }
  param x = testParamProcCastZero(true);
  writeln("testParamProcCastZero: when true int(64) uint(8) then ", x.type:string);
}

{
  proc testParamProcZero(param arg: bool) param {
    param i: int(64) = 0;
    param u: uint(8) = 0;
    if arg then return i; else return u;
  }
  param x = testParamProcZero(true);
  writeln("testParamProcZero: when true int(64) uint(8) then ", x.type:string);
}

{
  proc testParamProcCastMax(param arg: bool) param {
    if arg then return max(int(64)):int(64); else return max(uint(8)):uint(8);
  }
  param x = testParamProcCastMax(true);
  writeln("testParamProcCastMax: when true int(64) uint(8) then ", x.type:string);
}

{
  proc testParamProcMax(param arg: bool) param {
    param i: int(64) = max(int(64));
    param u: uint(8) = max(uint(8));
    if arg then return i; else return u;
  }
  param x = testParamProcMax(true);
  writeln("testParamProcMax: when true int(64) uint(8) then ", x.type:string);
}

{
  var b: bool = true;
  var x = if b then 0:int(64) else 0:uint(8);
  writeln("testExpressionZero: when true int(64) uint(8) then ", x.type:string);
}

{
  var b: bool = true;
  var x = if b then max(int(64)):int(64) else max(uint(8)):uint(8);
  writeln("testExpressionMax: when true int(64) uint(8) then ", x.type:string);
}

{
  param x = if true then 0:int(64) else 0:uint(8);
  writeln("testParamExpressionZero: when true int(64) uint(8) then ", x.type:string);
}

{
  param x = if true then max(int(64)):int(64) else max(uint(8)):uint(8);
  writeln("testParamExpressionMax: when true int(64) uint(8) then ", x.type:string);
}

{
  proc testProcCastZero(arg: bool) {
    if arg then return 0:int(64); else return 0:uint(16);
  }
  var x = testProcCastZero(true);
  writeln("testProcCastZero: when true int(64) uint(16) then ", x.type:string);
}

{
  proc testProcZero(arg: bool) {
    var i: int(64) = 0;
    var u: uint(16) = 0;
    if arg then return i; else return u;
  }
  var x = testProcZero(true);
  writeln("testProcZero: when true int(64) uint(16) then ", x.type:string);
}

{
  proc testProcCastMax(arg: bool) {
    if arg then return max(int(64)):int(64); else return max(uint(16)):uint(16);
  }
  var x = testProcCastMax(true);
  writeln("testProcCastMax: when true int(64) uint(16) then ", x.type:string);
}

{
  proc testProcMax(arg: bool) {
    var i: int(64) = max(int(64));
    var u: uint(16) = max(uint(16));
    if arg then return i; else return u;
  }
  var x = testProcMax(true);
  writeln("testProcMax: when true int(64) uint(16) then ", x.type:string);
}

{
  proc testParamProcCastZero(param arg: bool) param {
    if arg then return 0:int(64); else return 0:uint(16);
  }
  param x = testParamProcCastZero(true);
  writeln("testParamProcCastZero: when true int(64) uint(16) then ", x.type:string);
}

{
  proc testParamProcZero(param arg: bool) param {
    param i: int(64) = 0;
    param u: uint(16) = 0;
    if arg then return i; else return u;
  }
  param x = testParamProcZero(true);
  writeln("testParamProcZero: when true int(64) uint(16) then ", x.type:string);
}

{
  proc testParamProcCastMax(param arg: bool) param {
    if arg then return max(int(64)):int(64); else return max(uint(16)):uint(16);
  }
  param x = testParamProcCastMax(true);
  writeln("testParamProcCastMax: when true int(64) uint(16) then ", x.type:string);
}

{
  proc testParamProcMax(param arg: bool) param {
    param i: int(64) = max(int(64));
    param u: uint(16) = max(uint(16));
    if arg then return i; else return u;
  }
  param x = testParamProcMax(true);
  writeln("testParamProcMax: when true int(64) uint(16) then ", x.type:string);
}

{
  var b: bool = true;
  var x = if b then 0:int(64) else 0:uint(16);
  writeln("testExpressionZero: when true int(64) uint(16) then ", x.type:string);
}

{
  var b: bool = true;
  var x = if b then max(int(64)):int(64) else max(uint(16)):uint(16);
  writeln("testExpressionMax: when true int(64) uint(16) then ", x.type:string);
}

{
  param x = if true then 0:int(64) else 0:uint(16);
  writeln("testParamExpressionZero: when true int(64) uint(16) then ", x.type:string);
}

{
  param x = if true then max(int(64)):int(64) else max(uint(16)):uint(16);
  writeln("testParamExpressionMax: when true int(64) uint(16) then ", x.type:string);
}

{
  proc testProcCastZero(arg: bool) {
    if arg then return 0:int(64); else return 0:uint(32);
  }
  var x = testProcCastZero(true);
  writeln("testProcCastZero: when true int(64) uint(32) then ", x.type:string);
}

{
  proc testProcZero(arg: bool) {
    var i: int(64) = 0;
    var u: uint(32) = 0;
    if arg then return i; else return u;
  }
  var x = testProcZero(true);
  writeln("testProcZero: when true int(64) uint(32) then ", x.type:string);
}

{
  proc testProcCastMax(arg: bool) {
    if arg then return max(int(64)):int(64); else return max(uint(32)):uint(32);
  }
  var x = testProcCastMax(true);
  writeln("testProcCastMax: when true int(64) uint(32) then ", x.type:string);
}

{
  proc testProcMax(arg: bool) {
    var i: int(64) = max(int(64));
    var u: uint(32) = max(uint(32));
    if arg then return i; else return u;
  }
  var x = testProcMax(true);
  writeln("testProcMax: when true int(64) uint(32) then ", x.type:string);
}

{
  proc testParamProcCastZero(param arg: bool) param {
    if arg then return 0:int(64); else return 0:uint(32);
  }
  param x = testParamProcCastZero(true);
  writeln("testParamProcCastZero: when true int(64) uint(32) then ", x.type:string);
}

{
  proc testParamProcZero(param arg: bool) param {
    param i: int(64) = 0;
    param u: uint(32) = 0;
    if arg then return i; else return u;
  }
  param x = testParamProcZero(true);
  writeln("testParamProcZero: when true int(64) uint(32) then ", x.type:string);
}

{
  proc testParamProcCastMax(param arg: bool) param {
    if arg then return max(int(64)):int(64); else return max(uint(32)):uint(32);
  }
  param x = testParamProcCastMax(true);
  writeln("testParamProcCastMax: when true int(64) uint(32) then ", x.type:string);
}

{
  proc testParamProcMax(param arg: bool) param {
    param i: int(64) = max(int(64));
    param u: uint(32) = max(uint(32));
    if arg then return i; else return u;
  }
  param x = testParamProcMax(true);
  writeln("testParamProcMax: when true int(64) uint(32) then ", x.type:string);
}

{
  var b: bool = true;
  var x = if b then 0:int(64) else 0:uint(32);
  writeln("testExpressionZero: when true int(64) uint(32) then ", x.type:string);
}

{
  var b: bool = true;
  var x = if b then max(int(64)):int(64) else max(uint(32)):uint(32);
  writeln("testExpressionMax: when true int(64) uint(32) then ", x.type:string);
}

{
  param x = if true then 0:int(64) else 0:uint(32);
  writeln("testParamExpressionZero: when true int(64) uint(32) then ", x.type:string);
}

{
  param x = if true then max(int(64)):int(64) else max(uint(32)):uint(32);
  writeln("testParamExpressionMax: when true int(64) uint(32) then ", x.type:string);
}

{
  proc testProcCastZero(arg: bool) {
    if arg then return 0:int(64); else return 0:uint(64);
  }
  var x = testProcCastZero(true);
  writeln("testProcCastZero: when true int(64) uint(64) then ", x.type:string);
}

{
  proc testProcZero(arg: bool) {
    var i: int(64) = 0;
    var u: uint(64) = 0;
    if arg then return i; else return u;
  }
  var x = testProcZero(true);
  writeln("testProcZero: when true int(64) uint(64) then ", x.type:string);
}

{
  proc testProcCastMax(arg: bool) {
    if arg then return max(int(64)):int(64); else return max(uint(64)):uint(64);
  }
  var x = testProcCastMax(true);
  writeln("testProcCastMax: when true int(64) uint(64) then ", x.type:string);
}

{
  proc testProcMax(arg: bool) {
    var i: int(64) = max(int(64));
    var u: uint(64) = max(uint(64));
    if arg then return i; else return u;
  }
  var x = testProcMax(true);
  writeln("testProcMax: when true int(64) uint(64) then ", x.type:string);
}

{
  proc testParamProcCastZero(param arg: bool) param {
    if arg then return 0:int(64); else return 0:uint(64);
  }
  param x = testParamProcCastZero(true);
  writeln("testParamProcCastZero: when true int(64) uint(64) then ", x.type:string);
}

{
  proc testParamProcZero(param arg: bool) param {
    param i: int(64) = 0;
    param u: uint(64) = 0;
    if arg then return i; else return u;
  }
  param x = testParamProcZero(true);
  writeln("testParamProcZero: when true int(64) uint(64) then ", x.type:string);
}

{
  proc testParamProcCastMax(param arg: bool) param {
    if arg then return max(int(64)):int(64); else return max(uint(64)):uint(64);
  }
  param x = testParamProcCastMax(true);
  writeln("testParamProcCastMax: when true int(64) uint(64) then ", x.type:string);
}

{
  proc testParamProcMax(param arg: bool) param {
    param i: int(64) = max(int(64));
    param u: uint(64) = max(uint(64));
    if arg then return i; else return u;
  }
  param x = testParamProcMax(true);
  writeln("testParamProcMax: when true int(64) uint(64) then ", x.type:string);
}

{
  var b: bool = true;
  var x = if b then 0:int(64) else 0:uint(64);
  writeln("testExpressionZero: when true int(64) uint(64) then ", x.type:string);
}

{
  var b: bool = true;
  var x = if b then max(int(64)):int(64) else max(uint(64)):uint(64);
  writeln("testExpressionMax: when true int(64) uint(64) then ", x.type:string);
}

{
  param x = if true then 0:int(64) else 0:uint(64);
  writeln("testParamExpressionZero: when true int(64) uint(64) then ", x.type:string);
}

{
  param x = if true then max(int(64)):int(64) else max(uint(64)):uint(64);
  writeln("testParamExpressionMax: when true int(64) uint(64) then ", x.type:string);
}

{
  proc testProcCastZero(arg: bool) {
    if arg then return 0:uint(8); else return 0:int(8);
  }
  var x = testProcCastZero(true);
  writeln("testProcCastZero: when true uint(8) int(8) then ", x.type:string);
}

{
  proc testProcZero(arg: bool) {
    var i: uint(8) = 0;
    var u: int(8) = 0;
    if arg then return i; else return u;
  }
  var x = testProcZero(true);
  writeln("testProcZero: when true uint(8) int(8) then ", x.type:string);
}

{
  proc testProcCastMax(arg: bool) {
    if arg then return max(uint(8)):uint(8); else return max(int(8)):int(8);
  }
  var x = testProcCastMax(true);
  writeln("testProcCastMax: when true uint(8) int(8) then ", x.type:string);
}

{
  proc testProcMax(arg: bool) {
    var i: uint(8) = max(uint(8));
    var u: int(8) = max(int(8));
    if arg then return i; else return u;
  }
  var x = testProcMax(true);
  writeln("testProcMax: when true uint(8) int(8) then ", x.type:string);
}

{
  proc testParamProcCastZero(param arg: bool) param {
    if arg then return 0:uint(8); else return 0:int(8);
  }
  param x = testParamProcCastZero(true);
  writeln("testParamProcCastZero: when true uint(8) int(8) then ", x.type:string);
}

{
  proc testParamProcZero(param arg: bool) param {
    param i: uint(8) = 0;
    param u: int(8) = 0;
    if arg then return i; else return u;
  }
  param x = testParamProcZero(true);
  writeln("testParamProcZero: when true uint(8) int(8) then ", x.type:string);
}

{
  proc testParamProcCastMax(param arg: bool) param {
    if arg then return max(uint(8)):uint(8); else return max(int(8)):int(8);
  }
  param x = testParamProcCastMax(true);
  writeln("testParamProcCastMax: when true uint(8) int(8) then ", x.type:string);
}

{
  proc testParamProcMax(param arg: bool) param {
    param i: uint(8) = max(uint(8));
    param u: int(8) = max(int(8));
    if arg then return i; else return u;
  }
  param x = testParamProcMax(true);
  writeln("testParamProcMax: when true uint(8) int(8) then ", x.type:string);
}

{
  var b: bool = true;
  var x = if b then 0:uint(8) else 0:int(8);
  writeln("testExpressionZero: when true uint(8) int(8) then ", x.type:string);
}

{
  var b: bool = true;
  var x = if b then max(uint(8)):uint(8) else max(int(8)):int(8);
  writeln("testExpressionMax: when true uint(8) int(8) then ", x.type:string);
}

{
  param x = if true then 0:uint(8) else 0:int(8);
  writeln("testParamExpressionZero: when true uint(8) int(8) then ", x.type:string);
}

{
  param x = if true then max(uint(8)):uint(8) else max(int(8)):int(8);
  writeln("testParamExpressionMax: when true uint(8) int(8) then ", x.type:string);
}

{
  proc testProcCastZero(arg: bool) {
    if arg then return 0:uint(8); else return 0:int(16);
  }
  var x = testProcCastZero(true);
  writeln("testProcCastZero: when true uint(8) int(16) then ", x.type:string);
}

{
  proc testProcZero(arg: bool) {
    var i: uint(8) = 0;
    var u: int(16) = 0;
    if arg then return i; else return u;
  }
  var x = testProcZero(true);
  writeln("testProcZero: when true uint(8) int(16) then ", x.type:string);
}

{
  proc testProcCastMax(arg: bool) {
    if arg then return max(uint(8)):uint(8); else return max(int(16)):int(16);
  }
  var x = testProcCastMax(true);
  writeln("testProcCastMax: when true uint(8) int(16) then ", x.type:string);
}

{
  proc testProcMax(arg: bool) {
    var i: uint(8) = max(uint(8));
    var u: int(16) = max(int(16));
    if arg then return i; else return u;
  }
  var x = testProcMax(true);
  writeln("testProcMax: when true uint(8) int(16) then ", x.type:string);
}

{
  proc testParamProcCastZero(param arg: bool) param {
    if arg then return 0:uint(8); else return 0:int(16);
  }
  param x = testParamProcCastZero(true);
  writeln("testParamProcCastZero: when true uint(8) int(16) then ", x.type:string);
}

{
  proc testParamProcZero(param arg: bool) param {
    param i: uint(8) = 0;
    param u: int(16) = 0;
    if arg then return i; else return u;
  }
  param x = testParamProcZero(true);
  writeln("testParamProcZero: when true uint(8) int(16) then ", x.type:string);
}

{
  proc testParamProcCastMax(param arg: bool) param {
    if arg then return max(uint(8)):uint(8); else return max(int(16)):int(16);
  }
  param x = testParamProcCastMax(true);
  writeln("testParamProcCastMax: when true uint(8) int(16) then ", x.type:string);
}

{
  proc testParamProcMax(param arg: bool) param {
    param i: uint(8) = max(uint(8));
    param u: int(16) = max(int(16));
    if arg then return i; else return u;
  }
  param x = testParamProcMax(true);
  writeln("testParamProcMax: when true uint(8) int(16) then ", x.type:string);
}

{
  var b: bool = true;
  var x = if b then 0:uint(8) else 0:int(16);
  writeln("testExpressionZero: when true uint(8) int(16) then ", x.type:string);
}

{
  var b: bool = true;
  var x = if b then max(uint(8)):uint(8) else max(int(16)):int(16);
  writeln("testExpressionMax: when true uint(8) int(16) then ", x.type:string);
}

{
  param x = if true then 0:uint(8) else 0:int(16);
  writeln("testParamExpressionZero: when true uint(8) int(16) then ", x.type:string);
}

{
  param x = if true then max(uint(8)):uint(8) else max(int(16)):int(16);
  writeln("testParamExpressionMax: when true uint(8) int(16) then ", x.type:string);
}

{
  proc testProcCastZero(arg: bool) {
    if arg then return 0:uint(8); else return 0:int(32);
  }
  var x = testProcCastZero(true);
  writeln("testProcCastZero: when true uint(8) int(32) then ", x.type:string);
}

{
  proc testProcZero(arg: bool) {
    var i: uint(8) = 0;
    var u: int(32) = 0;
    if arg then return i; else return u;
  }
  var x = testProcZero(true);
  writeln("testProcZero: when true uint(8) int(32) then ", x.type:string);
}

{
  proc testProcCastMax(arg: bool) {
    if arg then return max(uint(8)):uint(8); else return max(int(32)):int(32);
  }
  var x = testProcCastMax(true);
  writeln("testProcCastMax: when true uint(8) int(32) then ", x.type:string);
}

{
  proc testProcMax(arg: bool) {
    var i: uint(8) = max(uint(8));
    var u: int(32) = max(int(32));
    if arg then return i; else return u;
  }
  var x = testProcMax(true);
  writeln("testProcMax: when true uint(8) int(32) then ", x.type:string);
}

{
  proc testParamProcCastZero(param arg: bool) param {
    if arg then return 0:uint(8); else return 0:int(32);
  }
  param x = testParamProcCastZero(true);
  writeln("testParamProcCastZero: when true uint(8) int(32) then ", x.type:string);
}

{
  proc testParamProcZero(param arg: bool) param {
    param i: uint(8) = 0;
    param u: int(32) = 0;
    if arg then return i; else return u;
  }
  param x = testParamProcZero(true);
  writeln("testParamProcZero: when true uint(8) int(32) then ", x.type:string);
}

{
  proc testParamProcCastMax(param arg: bool) param {
    if arg then return max(uint(8)):uint(8); else return max(int(32)):int(32);
  }
  param x = testParamProcCastMax(true);
  writeln("testParamProcCastMax: when true uint(8) int(32) then ", x.type:string);
}

{
  proc testParamProcMax(param arg: bool) param {
    param i: uint(8) = max(uint(8));
    param u: int(32) = max(int(32));
    if arg then return i; else return u;
  }
  param x = testParamProcMax(true);
  writeln("testParamProcMax: when true uint(8) int(32) then ", x.type:string);
}

{
  var b: bool = true;
  var x = if b then 0:uint(8) else 0:int(32);
  writeln("testExpressionZero: when true uint(8) int(32) then ", x.type:string);
}

{
  var b: bool = true;
  var x = if b then max(uint(8)):uint(8) else max(int(32)):int(32);
  writeln("testExpressionMax: when true uint(8) int(32) then ", x.type:string);
}

{
  param x = if true then 0:uint(8) else 0:int(32);
  writeln("testParamExpressionZero: when true uint(8) int(32) then ", x.type:string);
}

{
  param x = if true then max(uint(8)):uint(8) else max(int(32)):int(32);
  writeln("testParamExpressionMax: when true uint(8) int(32) then ", x.type:string);
}

{
  proc testProcCastZero(arg: bool) {
    if arg then return 0:uint(8); else return 0:int(64);
  }
  var x = testProcCastZero(true);
  writeln("testProcCastZero: when true uint(8) int(64) then ", x.type:string);
}

{
  proc testProcZero(arg: bool) {
    var i: uint(8) = 0;
    var u: int(64) = 0;
    if arg then return i; else return u;
  }
  var x = testProcZero(true);
  writeln("testProcZero: when true uint(8) int(64) then ", x.type:string);
}

{
  proc testProcCastMax(arg: bool) {
    if arg then return max(uint(8)):uint(8); else return max(int(64)):int(64);
  }
  var x = testProcCastMax(true);
  writeln("testProcCastMax: when true uint(8) int(64) then ", x.type:string);
}

{
  proc testProcMax(arg: bool) {
    var i: uint(8) = max(uint(8));
    var u: int(64) = max(int(64));
    if arg then return i; else return u;
  }
  var x = testProcMax(true);
  writeln("testProcMax: when true uint(8) int(64) then ", x.type:string);
}

{
  proc testParamProcCastZero(param arg: bool) param {
    if arg then return 0:uint(8); else return 0:int(64);
  }
  param x = testParamProcCastZero(true);
  writeln("testParamProcCastZero: when true uint(8) int(64) then ", x.type:string);
}

{
  proc testParamProcZero(param arg: bool) param {
    param i: uint(8) = 0;
    param u: int(64) = 0;
    if arg then return i; else return u;
  }
  param x = testParamProcZero(true);
  writeln("testParamProcZero: when true uint(8) int(64) then ", x.type:string);
}

{
  proc testParamProcCastMax(param arg: bool) param {
    if arg then return max(uint(8)):uint(8); else return max(int(64)):int(64);
  }
  param x = testParamProcCastMax(true);
  writeln("testParamProcCastMax: when true uint(8) int(64) then ", x.type:string);
}

{
  proc testParamProcMax(param arg: bool) param {
    param i: uint(8) = max(uint(8));
    param u: int(64) = max(int(64));
    if arg then return i; else return u;
  }
  param x = testParamProcMax(true);
  writeln("testParamProcMax: when true uint(8) int(64) then ", x.type:string);
}

{
  var b: bool = true;
  var x = if b then 0:uint(8) else 0:int(64);
  writeln("testExpressionZero: when true uint(8) int(64) then ", x.type:string);
}

{
  var b: bool = true;
  var x = if b then max(uint(8)):uint(8) else max(int(64)):int(64);
  writeln("testExpressionMax: when true uint(8) int(64) then ", x.type:string);
}

{
  param x = if true then 0:uint(8) else 0:int(64);
  writeln("testParamExpressionZero: when true uint(8) int(64) then ", x.type:string);
}

{
  param x = if true then max(uint(8)):uint(8) else max(int(64)):int(64);
  writeln("testParamExpressionMax: when true uint(8) int(64) then ", x.type:string);
}

{
  proc testProcCastZero(arg: bool) {
    if arg then return 0:uint(8); else return 0:uint(8);
  }
  var x = testProcCastZero(true);
  writeln("testProcCastZero: when true uint(8) uint(8) then ", x.type:string);
}

{
  proc testProcZero(arg: bool) {
    var i: uint(8) = 0;
    var u: uint(8) = 0;
    if arg then return i; else return u;
  }
  var x = testProcZero(true);
  writeln("testProcZero: when true uint(8) uint(8) then ", x.type:string);
}

{
  proc testProcCastMax(arg: bool) {
    if arg then return max(uint(8)):uint(8); else return max(uint(8)):uint(8);
  }
  var x = testProcCastMax(true);
  writeln("testProcCastMax: when true uint(8) uint(8) then ", x.type:string);
}

{
  proc testProcMax(arg: bool) {
    var i: uint(8) = max(uint(8));
    var u: uint(8) = max(uint(8));
    if arg then return i; else return u;
  }
  var x = testProcMax(true);
  writeln("testProcMax: when true uint(8) uint(8) then ", x.type:string);
}

{
  proc testParamProcCastZero(param arg: bool) param {
    if arg then return 0:uint(8); else return 0:uint(8);
  }
  param x = testParamProcCastZero(true);
  writeln("testParamProcCastZero: when true uint(8) uint(8) then ", x.type:string);
}

{
  proc testParamProcZero(param arg: bool) param {
    param i: uint(8) = 0;
    param u: uint(8) = 0;
    if arg then return i; else return u;
  }
  param x = testParamProcZero(true);
  writeln("testParamProcZero: when true uint(8) uint(8) then ", x.type:string);
}

{
  proc testParamProcCastMax(param arg: bool) param {
    if arg then return max(uint(8)):uint(8); else return max(uint(8)):uint(8);
  }
  param x = testParamProcCastMax(true);
  writeln("testParamProcCastMax: when true uint(8) uint(8) then ", x.type:string);
}

{
  proc testParamProcMax(param arg: bool) param {
    param i: uint(8) = max(uint(8));
    param u: uint(8) = max(uint(8));
    if arg then return i; else return u;
  }
  param x = testParamProcMax(true);
  writeln("testParamProcMax: when true uint(8) uint(8) then ", x.type:string);
}

{
  var b: bool = true;
  var x = if b then 0:uint(8) else 0:uint(8);
  writeln("testExpressionZero: when true uint(8) uint(8) then ", x.type:string);
}

{
  var b: bool = true;
  var x = if b then max(uint(8)):uint(8) else max(uint(8)):uint(8);
  writeln("testExpressionMax: when true uint(8) uint(8) then ", x.type:string);
}

{
  param x = if true then 0:uint(8) else 0:uint(8);
  writeln("testParamExpressionZero: when true uint(8) uint(8) then ", x.type:string);
}

{
  param x = if true then max(uint(8)):uint(8) else max(uint(8)):uint(8);
  writeln("testParamExpressionMax: when true uint(8) uint(8) then ", x.type:string);
}

{
  proc testProcCastZero(arg: bool) {
    if arg then return 0:uint(8); else return 0:uint(16);
  }
  var x = testProcCastZero(true);
  writeln("testProcCastZero: when true uint(8) uint(16) then ", x.type:string);
}

{
  proc testProcZero(arg: bool) {
    var i: uint(8) = 0;
    var u: uint(16) = 0;
    if arg then return i; else return u;
  }
  var x = testProcZero(true);
  writeln("testProcZero: when true uint(8) uint(16) then ", x.type:string);
}

{
  proc testProcCastMax(arg: bool) {
    if arg then return max(uint(8)):uint(8); else return max(uint(16)):uint(16);
  }
  var x = testProcCastMax(true);
  writeln("testProcCastMax: when true uint(8) uint(16) then ", x.type:string);
}

{
  proc testProcMax(arg: bool) {
    var i: uint(8) = max(uint(8));
    var u: uint(16) = max(uint(16));
    if arg then return i; else return u;
  }
  var x = testProcMax(true);
  writeln("testProcMax: when true uint(8) uint(16) then ", x.type:string);
}

{
  proc testParamProcCastZero(param arg: bool) param {
    if arg then return 0:uint(8); else return 0:uint(16);
  }
  param x = testParamProcCastZero(true);
  writeln("testParamProcCastZero: when true uint(8) uint(16) then ", x.type:string);
}

{
  proc testParamProcZero(param arg: bool) param {
    param i: uint(8) = 0;
    param u: uint(16) = 0;
    if arg then return i; else return u;
  }
  param x = testParamProcZero(true);
  writeln("testParamProcZero: when true uint(8) uint(16) then ", x.type:string);
}

{
  proc testParamProcCastMax(param arg: bool) param {
    if arg then return max(uint(8)):uint(8); else return max(uint(16)):uint(16);
  }
  param x = testParamProcCastMax(true);
  writeln("testParamProcCastMax: when true uint(8) uint(16) then ", x.type:string);
}

{
  proc testParamProcMax(param arg: bool) param {
    param i: uint(8) = max(uint(8));
    param u: uint(16) = max(uint(16));
    if arg then return i; else return u;
  }
  param x = testParamProcMax(true);
  writeln("testParamProcMax: when true uint(8) uint(16) then ", x.type:string);
}

{
  var b: bool = true;
  var x = if b then 0:uint(8) else 0:uint(16);
  writeln("testExpressionZero: when true uint(8) uint(16) then ", x.type:string);
}

{
  var b: bool = true;
  var x = if b then max(uint(8)):uint(8) else max(uint(16)):uint(16);
  writeln("testExpressionMax: when true uint(8) uint(16) then ", x.type:string);
}

{
  param x = if true then 0:uint(8) else 0:uint(16);
  writeln("testParamExpressionZero: when true uint(8) uint(16) then ", x.type:string);
}

{
  param x = if true then max(uint(8)):uint(8) else max(uint(16)):uint(16);
  writeln("testParamExpressionMax: when true uint(8) uint(16) then ", x.type:string);
}

{
  proc testProcCastZero(arg: bool) {
    if arg then return 0:uint(8); else return 0:uint(32);
  }
  var x = testProcCastZero(true);
  writeln("testProcCastZero: when true uint(8) uint(32) then ", x.type:string);
}

{
  proc testProcZero(arg: bool) {
    var i: uint(8) = 0;
    var u: uint(32) = 0;
    if arg then return i; else return u;
  }
  var x = testProcZero(true);
  writeln("testProcZero: when true uint(8) uint(32) then ", x.type:string);
}

{
  proc testProcCastMax(arg: bool) {
    if arg then return max(uint(8)):uint(8); else return max(uint(32)):uint(32);
  }
  var x = testProcCastMax(true);
  writeln("testProcCastMax: when true uint(8) uint(32) then ", x.type:string);
}

{
  proc testProcMax(arg: bool) {
    var i: uint(8) = max(uint(8));
    var u: uint(32) = max(uint(32));
    if arg then return i; else return u;
  }
  var x = testProcMax(true);
  writeln("testProcMax: when true uint(8) uint(32) then ", x.type:string);
}

{
  proc testParamProcCastZero(param arg: bool) param {
    if arg then return 0:uint(8); else return 0:uint(32);
  }
  param x = testParamProcCastZero(true);
  writeln("testParamProcCastZero: when true uint(8) uint(32) then ", x.type:string);
}

{
  proc testParamProcZero(param arg: bool) param {
    param i: uint(8) = 0;
    param u: uint(32) = 0;
    if arg then return i; else return u;
  }
  param x = testParamProcZero(true);
  writeln("testParamProcZero: when true uint(8) uint(32) then ", x.type:string);
}

{
  proc testParamProcCastMax(param arg: bool) param {
    if arg then return max(uint(8)):uint(8); else return max(uint(32)):uint(32);
  }
  param x = testParamProcCastMax(true);
  writeln("testParamProcCastMax: when true uint(8) uint(32) then ", x.type:string);
}

{
  proc testParamProcMax(param arg: bool) param {
    param i: uint(8) = max(uint(8));
    param u: uint(32) = max(uint(32));
    if arg then return i; else return u;
  }
  param x = testParamProcMax(true);
  writeln("testParamProcMax: when true uint(8) uint(32) then ", x.type:string);
}

{
  var b: bool = true;
  var x = if b then 0:uint(8) else 0:uint(32);
  writeln("testExpressionZero: when true uint(8) uint(32) then ", x.type:string);
}

{
  var b: bool = true;
  var x = if b then max(uint(8)):uint(8) else max(uint(32)):uint(32);
  writeln("testExpressionMax: when true uint(8) uint(32) then ", x.type:string);
}

{
  param x = if true then 0:uint(8) else 0:uint(32);
  writeln("testParamExpressionZero: when true uint(8) uint(32) then ", x.type:string);
}

{
  param x = if true then max(uint(8)):uint(8) else max(uint(32)):uint(32);
  writeln("testParamExpressionMax: when true uint(8) uint(32) then ", x.type:string);
}

{
  proc testProcCastZero(arg: bool) {
    if arg then return 0:uint(8); else return 0:uint(64);
  }
  var x = testProcCastZero(true);
  writeln("testProcCastZero: when true uint(8) uint(64) then ", x.type:string);
}

{
  proc testProcZero(arg: bool) {
    var i: uint(8) = 0;
    var u: uint(64) = 0;
    if arg then return i; else return u;
  }
  var x = testProcZero(true);
  writeln("testProcZero: when true uint(8) uint(64) then ", x.type:string);
}

{
  proc testProcCastMax(arg: bool) {
    if arg then return max(uint(8)):uint(8); else return max(uint(64)):uint(64);
  }
  var x = testProcCastMax(true);
  writeln("testProcCastMax: when true uint(8) uint(64) then ", x.type:string);
}

{
  proc testProcMax(arg: bool) {
    var i: uint(8) = max(uint(8));
    var u: uint(64) = max(uint(64));
    if arg then return i; else return u;
  }
  var x = testProcMax(true);
  writeln("testProcMax: when true uint(8) uint(64) then ", x.type:string);
}

{
  proc testParamProcCastZero(param arg: bool) param {
    if arg then return 0:uint(8); else return 0:uint(64);
  }
  param x = testParamProcCastZero(true);
  writeln("testParamProcCastZero: when true uint(8) uint(64) then ", x.type:string);
}

{
  proc testParamProcZero(param arg: bool) param {
    param i: uint(8) = 0;
    param u: uint(64) = 0;
    if arg then return i; else return u;
  }
  param x = testParamProcZero(true);
  writeln("testParamProcZero: when true uint(8) uint(64) then ", x.type:string);
}

{
  proc testParamProcCastMax(param arg: bool) param {
    if arg then return max(uint(8)):uint(8); else return max(uint(64)):uint(64);
  }
  param x = testParamProcCastMax(true);
  writeln("testParamProcCastMax: when true uint(8) uint(64) then ", x.type:string);
}

{
  proc testParamProcMax(param arg: bool) param {
    param i: uint(8) = max(uint(8));
    param u: uint(64) = max(uint(64));
    if arg then return i; else return u;
  }
  param x = testParamProcMax(true);
  writeln("testParamProcMax: when true uint(8) uint(64) then ", x.type:string);
}

{
  var b: bool = true;
  var x = if b then 0:uint(8) else 0:uint(64);
  writeln("testExpressionZero: when true uint(8) uint(64) then ", x.type:string);
}

{
  var b: bool = true;
  var x = if b then max(uint(8)):uint(8) else max(uint(64)):uint(64);
  writeln("testExpressionMax: when true uint(8) uint(64) then ", x.type:string);
}

{
  param x = if true then 0:uint(8) else 0:uint(64);
  writeln("testParamExpressionZero: when true uint(8) uint(64) then ", x.type:string);
}

{
  param x = if true then max(uint(8)):uint(8) else max(uint(64)):uint(64);
  writeln("testParamExpressionMax: when true uint(8) uint(64) then ", x.type:string);
}

{
  proc testProcCastZero(arg: bool) {
    if arg then return 0:uint(16); else return 0:int(8);
  }
  var x = testProcCastZero(true);
  writeln("testProcCastZero: when true uint(16) int(8) then ", x.type:string);
}

{
  proc testProcZero(arg: bool) {
    var i: uint(16) = 0;
    var u: int(8) = 0;
    if arg then return i; else return u;
  }
  var x = testProcZero(true);
  writeln("testProcZero: when true uint(16) int(8) then ", x.type:string);
}

{
  proc testProcCastMax(arg: bool) {
    if arg then return max(uint(16)):uint(16); else return max(int(8)):int(8);
  }
  var x = testProcCastMax(true);
  writeln("testProcCastMax: when true uint(16) int(8) then ", x.type:string);
}

{
  proc testProcMax(arg: bool) {
    var i: uint(16) = max(uint(16));
    var u: int(8) = max(int(8));
    if arg then return i; else return u;
  }
  var x = testProcMax(true);
  writeln("testProcMax: when true uint(16) int(8) then ", x.type:string);
}

{
  proc testParamProcCastZero(param arg: bool) param {
    if arg then return 0:uint(16); else return 0:int(8);
  }
  param x = testParamProcCastZero(true);
  writeln("testParamProcCastZero: when true uint(16) int(8) then ", x.type:string);
}

{
  proc testParamProcZero(param arg: bool) param {
    param i: uint(16) = 0;
    param u: int(8) = 0;
    if arg then return i; else return u;
  }
  param x = testParamProcZero(true);
  writeln("testParamProcZero: when true uint(16) int(8) then ", x.type:string);
}

{
  proc testParamProcCastMax(param arg: bool) param {
    if arg then return max(uint(16)):uint(16); else return max(int(8)):int(8);
  }
  param x = testParamProcCastMax(true);
  writeln("testParamProcCastMax: when true uint(16) int(8) then ", x.type:string);
}

{
  proc testParamProcMax(param arg: bool) param {
    param i: uint(16) = max(uint(16));
    param u: int(8) = max(int(8));
    if arg then return i; else return u;
  }
  param x = testParamProcMax(true);
  writeln("testParamProcMax: when true uint(16) int(8) then ", x.type:string);
}

{
  var b: bool = true;
  var x = if b then 0:uint(16) else 0:int(8);
  writeln("testExpressionZero: when true uint(16) int(8) then ", x.type:string);
}

{
  var b: bool = true;
  var x = if b then max(uint(16)):uint(16) else max(int(8)):int(8);
  writeln("testExpressionMax: when true uint(16) int(8) then ", x.type:string);
}

{
  param x = if true then 0:uint(16) else 0:int(8);
  writeln("testParamExpressionZero: when true uint(16) int(8) then ", x.type:string);
}

{
  param x = if true then max(uint(16)):uint(16) else max(int(8)):int(8);
  writeln("testParamExpressionMax: when true uint(16) int(8) then ", x.type:string);
}

{
  proc testProcCastZero(arg: bool) {
    if arg then return 0:uint(16); else return 0:int(16);
  }
  var x = testProcCastZero(true);
  writeln("testProcCastZero: when true uint(16) int(16) then ", x.type:string);
}

{
  proc testProcZero(arg: bool) {
    var i: uint(16) = 0;
    var u: int(16) = 0;
    if arg then return i; else return u;
  }
  var x = testProcZero(true);
  writeln("testProcZero: when true uint(16) int(16) then ", x.type:string);
}

{
  proc testProcCastMax(arg: bool) {
    if arg then return max(uint(16)):uint(16); else return max(int(16)):int(16);
  }
  var x = testProcCastMax(true);
  writeln("testProcCastMax: when true uint(16) int(16) then ", x.type:string);
}

{
  proc testProcMax(arg: bool) {
    var i: uint(16) = max(uint(16));
    var u: int(16) = max(int(16));
    if arg then return i; else return u;
  }
  var x = testProcMax(true);
  writeln("testProcMax: when true uint(16) int(16) then ", x.type:string);
}

{
  proc testParamProcCastZero(param arg: bool) param {
    if arg then return 0:uint(16); else return 0:int(16);
  }
  param x = testParamProcCastZero(true);
  writeln("testParamProcCastZero: when true uint(16) int(16) then ", x.type:string);
}

{
  proc testParamProcZero(param arg: bool) param {
    param i: uint(16) = 0;
    param u: int(16) = 0;
    if arg then return i; else return u;
  }
  param x = testParamProcZero(true);
  writeln("testParamProcZero: when true uint(16) int(16) then ", x.type:string);
}

{
  proc testParamProcCastMax(param arg: bool) param {
    if arg then return max(uint(16)):uint(16); else return max(int(16)):int(16);
  }
  param x = testParamProcCastMax(true);
  writeln("testParamProcCastMax: when true uint(16) int(16) then ", x.type:string);
}

{
  proc testParamProcMax(param arg: bool) param {
    param i: uint(16) = max(uint(16));
    param u: int(16) = max(int(16));
    if arg then return i; else return u;
  }
  param x = testParamProcMax(true);
  writeln("testParamProcMax: when true uint(16) int(16) then ", x.type:string);
}

{
  var b: bool = true;
  var x = if b then 0:uint(16) else 0:int(16);
  writeln("testExpressionZero: when true uint(16) int(16) then ", x.type:string);
}

{
  var b: bool = true;
  var x = if b then max(uint(16)):uint(16) else max(int(16)):int(16);
  writeln("testExpressionMax: when true uint(16) int(16) then ", x.type:string);
}

{
  param x = if true then 0:uint(16) else 0:int(16);
  writeln("testParamExpressionZero: when true uint(16) int(16) then ", x.type:string);
}

{
  param x = if true then max(uint(16)):uint(16) else max(int(16)):int(16);
  writeln("testParamExpressionMax: when true uint(16) int(16) then ", x.type:string);
}

{
  proc testProcCastZero(arg: bool) {
    if arg then return 0:uint(16); else return 0:int(32);
  }
  var x = testProcCastZero(true);
  writeln("testProcCastZero: when true uint(16) int(32) then ", x.type:string);
}

{
  proc testProcZero(arg: bool) {
    var i: uint(16) = 0;
    var u: int(32) = 0;
    if arg then return i; else return u;
  }
  var x = testProcZero(true);
  writeln("testProcZero: when true uint(16) int(32) then ", x.type:string);
}

{
  proc testProcCastMax(arg: bool) {
    if arg then return max(uint(16)):uint(16); else return max(int(32)):int(32);
  }
  var x = testProcCastMax(true);
  writeln("testProcCastMax: when true uint(16) int(32) then ", x.type:string);
}

{
  proc testProcMax(arg: bool) {
    var i: uint(16) = max(uint(16));
    var u: int(32) = max(int(32));
    if arg then return i; else return u;
  }
  var x = testProcMax(true);
  writeln("testProcMax: when true uint(16) int(32) then ", x.type:string);
}

{
  proc testParamProcCastZero(param arg: bool) param {
    if arg then return 0:uint(16); else return 0:int(32);
  }
  param x = testParamProcCastZero(true);
  writeln("testParamProcCastZero: when true uint(16) int(32) then ", x.type:string);
}

{
  proc testParamProcZero(param arg: bool) param {
    param i: uint(16) = 0;
    param u: int(32) = 0;
    if arg then return i; else return u;
  }
  param x = testParamProcZero(true);
  writeln("testParamProcZero: when true uint(16) int(32) then ", x.type:string);
}

{
  proc testParamProcCastMax(param arg: bool) param {
    if arg then return max(uint(16)):uint(16); else return max(int(32)):int(32);
  }
  param x = testParamProcCastMax(true);
  writeln("testParamProcCastMax: when true uint(16) int(32) then ", x.type:string);
}

{
  proc testParamProcMax(param arg: bool) param {
    param i: uint(16) = max(uint(16));
    param u: int(32) = max(int(32));
    if arg then return i; else return u;
  }
  param x = testParamProcMax(true);
  writeln("testParamProcMax: when true uint(16) int(32) then ", x.type:string);
}

{
  var b: bool = true;
  var x = if b then 0:uint(16) else 0:int(32);
  writeln("testExpressionZero: when true uint(16) int(32) then ", x.type:string);
}

{
  var b: bool = true;
  var x = if b then max(uint(16)):uint(16) else max(int(32)):int(32);
  writeln("testExpressionMax: when true uint(16) int(32) then ", x.type:string);
}

{
  param x = if true then 0:uint(16) else 0:int(32);
  writeln("testParamExpressionZero: when true uint(16) int(32) then ", x.type:string);
}

{
  param x = if true then max(uint(16)):uint(16) else max(int(32)):int(32);
  writeln("testParamExpressionMax: when true uint(16) int(32) then ", x.type:string);
}

{
  proc testProcCastZero(arg: bool) {
    if arg then return 0:uint(16); else return 0:int(64);
  }
  var x = testProcCastZero(true);
  writeln("testProcCastZero: when true uint(16) int(64) then ", x.type:string);
}

{
  proc testProcZero(arg: bool) {
    var i: uint(16) = 0;
    var u: int(64) = 0;
    if arg then return i; else return u;
  }
  var x = testProcZero(true);
  writeln("testProcZero: when true uint(16) int(64) then ", x.type:string);
}

{
  proc testProcCastMax(arg: bool) {
    if arg then return max(uint(16)):uint(16); else return max(int(64)):int(64);
  }
  var x = testProcCastMax(true);
  writeln("testProcCastMax: when true uint(16) int(64) then ", x.type:string);
}

{
  proc testProcMax(arg: bool) {
    var i: uint(16) = max(uint(16));
    var u: int(64) = max(int(64));
    if arg then return i; else return u;
  }
  var x = testProcMax(true);
  writeln("testProcMax: when true uint(16) int(64) then ", x.type:string);
}

{
  proc testParamProcCastZero(param arg: bool) param {
    if arg then return 0:uint(16); else return 0:int(64);
  }
  param x = testParamProcCastZero(true);
  writeln("testParamProcCastZero: when true uint(16) int(64) then ", x.type:string);
}

{
  proc testParamProcZero(param arg: bool) param {
    param i: uint(16) = 0;
    param u: int(64) = 0;
    if arg then return i; else return u;
  }
  param x = testParamProcZero(true);
  writeln("testParamProcZero: when true uint(16) int(64) then ", x.type:string);
}

{
  proc testParamProcCastMax(param arg: bool) param {
    if arg then return max(uint(16)):uint(16); else return max(int(64)):int(64);
  }
  param x = testParamProcCastMax(true);
  writeln("testParamProcCastMax: when true uint(16) int(64) then ", x.type:string);
}

{
  proc testParamProcMax(param arg: bool) param {
    param i: uint(16) = max(uint(16));
    param u: int(64) = max(int(64));
    if arg then return i; else return u;
  }
  param x = testParamProcMax(true);
  writeln("testParamProcMax: when true uint(16) int(64) then ", x.type:string);
}

{
  var b: bool = true;
  var x = if b then 0:uint(16) else 0:int(64);
  writeln("testExpressionZero: when true uint(16) int(64) then ", x.type:string);
}

{
  var b: bool = true;
  var x = if b then max(uint(16)):uint(16) else max(int(64)):int(64);
  writeln("testExpressionMax: when true uint(16) int(64) then ", x.type:string);
}

{
  param x = if true then 0:uint(16) else 0:int(64);
  writeln("testParamExpressionZero: when true uint(16) int(64) then ", x.type:string);
}

{
  param x = if true then max(uint(16)):uint(16) else max(int(64)):int(64);
  writeln("testParamExpressionMax: when true uint(16) int(64) then ", x.type:string);
}

{
  proc testProcCastZero(arg: bool) {
    if arg then return 0:uint(16); else return 0:uint(8);
  }
  var x = testProcCastZero(true);
  writeln("testProcCastZero: when true uint(16) uint(8) then ", x.type:string);
}

{
  proc testProcZero(arg: bool) {
    var i: uint(16) = 0;
    var u: uint(8) = 0;
    if arg then return i; else return u;
  }
  var x = testProcZero(true);
  writeln("testProcZero: when true uint(16) uint(8) then ", x.type:string);
}

{
  proc testProcCastMax(arg: bool) {
    if arg then return max(uint(16)):uint(16); else return max(uint(8)):uint(8);
  }
  var x = testProcCastMax(true);
  writeln("testProcCastMax: when true uint(16) uint(8) then ", x.type:string);
}

{
  proc testProcMax(arg: bool) {
    var i: uint(16) = max(uint(16));
    var u: uint(8) = max(uint(8));
    if arg then return i; else return u;
  }
  var x = testProcMax(true);
  writeln("testProcMax: when true uint(16) uint(8) then ", x.type:string);
}

{
  proc testParamProcCastZero(param arg: bool) param {
    if arg then return 0:uint(16); else return 0:uint(8);
  }
  param x = testParamProcCastZero(true);
  writeln("testParamProcCastZero: when true uint(16) uint(8) then ", x.type:string);
}

{
  proc testParamProcZero(param arg: bool) param {
    param i: uint(16) = 0;
    param u: uint(8) = 0;
    if arg then return i; else return u;
  }
  param x = testParamProcZero(true);
  writeln("testParamProcZero: when true uint(16) uint(8) then ", x.type:string);
}

{
  proc testParamProcCastMax(param arg: bool) param {
    if arg then return max(uint(16)):uint(16); else return max(uint(8)):uint(8);
  }
  param x = testParamProcCastMax(true);
  writeln("testParamProcCastMax: when true uint(16) uint(8) then ", x.type:string);
}

{
  proc testParamProcMax(param arg: bool) param {
    param i: uint(16) = max(uint(16));
    param u: uint(8) = max(uint(8));
    if arg then return i; else return u;
  }
  param x = testParamProcMax(true);
  writeln("testParamProcMax: when true uint(16) uint(8) then ", x.type:string);
}

{
  var b: bool = true;
  var x = if b then 0:uint(16) else 0:uint(8);
  writeln("testExpressionZero: when true uint(16) uint(8) then ", x.type:string);
}

{
  var b: bool = true;
  var x = if b then max(uint(16)):uint(16) else max(uint(8)):uint(8);
  writeln("testExpressionMax: when true uint(16) uint(8) then ", x.type:string);
}

{
  param x = if true then 0:uint(16) else 0:uint(8);
  writeln("testParamExpressionZero: when true uint(16) uint(8) then ", x.type:string);
}

{
  param x = if true then max(uint(16)):uint(16) else max(uint(8)):uint(8);
  writeln("testParamExpressionMax: when true uint(16) uint(8) then ", x.type:string);
}

{
  proc testProcCastZero(arg: bool) {
    if arg then return 0:uint(16); else return 0:uint(16);
  }
  var x = testProcCastZero(true);
  writeln("testProcCastZero: when true uint(16) uint(16) then ", x.type:string);
}

{
  proc testProcZero(arg: bool) {
    var i: uint(16) = 0;
    var u: uint(16) = 0;
    if arg then return i; else return u;
  }
  var x = testProcZero(true);
  writeln("testProcZero: when true uint(16) uint(16) then ", x.type:string);
}

{
  proc testProcCastMax(arg: bool) {
    if arg then return max(uint(16)):uint(16); else return max(uint(16)):uint(16);
  }
  var x = testProcCastMax(true);
  writeln("testProcCastMax: when true uint(16) uint(16) then ", x.type:string);
}

{
  proc testProcMax(arg: bool) {
    var i: uint(16) = max(uint(16));
    var u: uint(16) = max(uint(16));
    if arg then return i; else return u;
  }
  var x = testProcMax(true);
  writeln("testProcMax: when true uint(16) uint(16) then ", x.type:string);
}

{
  proc testParamProcCastZero(param arg: bool) param {
    if arg then return 0:uint(16); else return 0:uint(16);
  }
  param x = testParamProcCastZero(true);
  writeln("testParamProcCastZero: when true uint(16) uint(16) then ", x.type:string);
}

{
  proc testParamProcZero(param arg: bool) param {
    param i: uint(16) = 0;
    param u: uint(16) = 0;
    if arg then return i; else return u;
  }
  param x = testParamProcZero(true);
  writeln("testParamProcZero: when true uint(16) uint(16) then ", x.type:string);
}

{
  proc testParamProcCastMax(param arg: bool) param {
    if arg then return max(uint(16)):uint(16); else return max(uint(16)):uint(16);
  }
  param x = testParamProcCastMax(true);
  writeln("testParamProcCastMax: when true uint(16) uint(16) then ", x.type:string);
}

{
  proc testParamProcMax(param arg: bool) param {
    param i: uint(16) = max(uint(16));
    param u: uint(16) = max(uint(16));
    if arg then return i; else return u;
  }
  param x = testParamProcMax(true);
  writeln("testParamProcMax: when true uint(16) uint(16) then ", x.type:string);
}

{
  var b: bool = true;
  var x = if b then 0:uint(16) else 0:uint(16);
  writeln("testExpressionZero: when true uint(16) uint(16) then ", x.type:string);
}

{
  var b: bool = true;
  var x = if b then max(uint(16)):uint(16) else max(uint(16)):uint(16);
  writeln("testExpressionMax: when true uint(16) uint(16) then ", x.type:string);
}

{
  param x = if true then 0:uint(16) else 0:uint(16);
  writeln("testParamExpressionZero: when true uint(16) uint(16) then ", x.type:string);
}

{
  param x = if true then max(uint(16)):uint(16) else max(uint(16)):uint(16);
  writeln("testParamExpressionMax: when true uint(16) uint(16) then ", x.type:string);
}

{
  proc testProcCastZero(arg: bool) {
    if arg then return 0:uint(16); else return 0:uint(32);
  }
  var x = testProcCastZero(true);
  writeln("testProcCastZero: when true uint(16) uint(32) then ", x.type:string);
}

{
  proc testProcZero(arg: bool) {
    var i: uint(16) = 0;
    var u: uint(32) = 0;
    if arg then return i; else return u;
  }
  var x = testProcZero(true);
  writeln("testProcZero: when true uint(16) uint(32) then ", x.type:string);
}

{
  proc testProcCastMax(arg: bool) {
    if arg then return max(uint(16)):uint(16); else return max(uint(32)):uint(32);
  }
  var x = testProcCastMax(true);
  writeln("testProcCastMax: when true uint(16) uint(32) then ", x.type:string);
}

{
  proc testProcMax(arg: bool) {
    var i: uint(16) = max(uint(16));
    var u: uint(32) = max(uint(32));
    if arg then return i; else return u;
  }
  var x = testProcMax(true);
  writeln("testProcMax: when true uint(16) uint(32) then ", x.type:string);
}

{
  proc testParamProcCastZero(param arg: bool) param {
    if arg then return 0:uint(16); else return 0:uint(32);
  }
  param x = testParamProcCastZero(true);
  writeln("testParamProcCastZero: when true uint(16) uint(32) then ", x.type:string);
}

{
  proc testParamProcZero(param arg: bool) param {
    param i: uint(16) = 0;
    param u: uint(32) = 0;
    if arg then return i; else return u;
  }
  param x = testParamProcZero(true);
  writeln("testParamProcZero: when true uint(16) uint(32) then ", x.type:string);
}

{
  proc testParamProcCastMax(param arg: bool) param {
    if arg then return max(uint(16)):uint(16); else return max(uint(32)):uint(32);
  }
  param x = testParamProcCastMax(true);
  writeln("testParamProcCastMax: when true uint(16) uint(32) then ", x.type:string);
}

{
  proc testParamProcMax(param arg: bool) param {
    param i: uint(16) = max(uint(16));
    param u: uint(32) = max(uint(32));
    if arg then return i; else return u;
  }
  param x = testParamProcMax(true);
  writeln("testParamProcMax: when true uint(16) uint(32) then ", x.type:string);
}

{
  var b: bool = true;
  var x = if b then 0:uint(16) else 0:uint(32);
  writeln("testExpressionZero: when true uint(16) uint(32) then ", x.type:string);
}

{
  var b: bool = true;
  var x = if b then max(uint(16)):uint(16) else max(uint(32)):uint(32);
  writeln("testExpressionMax: when true uint(16) uint(32) then ", x.type:string);
}

{
  param x = if true then 0:uint(16) else 0:uint(32);
  writeln("testParamExpressionZero: when true uint(16) uint(32) then ", x.type:string);
}

{
  param x = if true then max(uint(16)):uint(16) else max(uint(32)):uint(32);
  writeln("testParamExpressionMax: when true uint(16) uint(32) then ", x.type:string);
}

{
  proc testProcCastZero(arg: bool) {
    if arg then return 0:uint(16); else return 0:uint(64);
  }
  var x = testProcCastZero(true);
  writeln("testProcCastZero: when true uint(16) uint(64) then ", x.type:string);
}

{
  proc testProcZero(arg: bool) {
    var i: uint(16) = 0;
    var u: uint(64) = 0;
    if arg then return i; else return u;
  }
  var x = testProcZero(true);
  writeln("testProcZero: when true uint(16) uint(64) then ", x.type:string);
}

{
  proc testProcCastMax(arg: bool) {
    if arg then return max(uint(16)):uint(16); else return max(uint(64)):uint(64);
  }
  var x = testProcCastMax(true);
  writeln("testProcCastMax: when true uint(16) uint(64) then ", x.type:string);
}

{
  proc testProcMax(arg: bool) {
    var i: uint(16) = max(uint(16));
    var u: uint(64) = max(uint(64));
    if arg then return i; else return u;
  }
  var x = testProcMax(true);
  writeln("testProcMax: when true uint(16) uint(64) then ", x.type:string);
}

{
  proc testParamProcCastZero(param arg: bool) param {
    if arg then return 0:uint(16); else return 0:uint(64);
  }
  param x = testParamProcCastZero(true);
  writeln("testParamProcCastZero: when true uint(16) uint(64) then ", x.type:string);
}

{
  proc testParamProcZero(param arg: bool) param {
    param i: uint(16) = 0;
    param u: uint(64) = 0;
    if arg then return i; else return u;
  }
  param x = testParamProcZero(true);
  writeln("testParamProcZero: when true uint(16) uint(64) then ", x.type:string);
}

{
  proc testParamProcCastMax(param arg: bool) param {
    if arg then return max(uint(16)):uint(16); else return max(uint(64)):uint(64);
  }
  param x = testParamProcCastMax(true);
  writeln("testParamProcCastMax: when true uint(16) uint(64) then ", x.type:string);
}

{
  proc testParamProcMax(param arg: bool) param {
    param i: uint(16) = max(uint(16));
    param u: uint(64) = max(uint(64));
    if arg then return i; else return u;
  }
  param x = testParamProcMax(true);
  writeln("testParamProcMax: when true uint(16) uint(64) then ", x.type:string);
}

{
  var b: bool = true;
  var x = if b then 0:uint(16) else 0:uint(64);
  writeln("testExpressionZero: when true uint(16) uint(64) then ", x.type:string);
}

{
  var b: bool = true;
  var x = if b then max(uint(16)):uint(16) else max(uint(64)):uint(64);
  writeln("testExpressionMax: when true uint(16) uint(64) then ", x.type:string);
}

{
  param x = if true then 0:uint(16) else 0:uint(64);
  writeln("testParamExpressionZero: when true uint(16) uint(64) then ", x.type:string);
}

{
  param x = if true then max(uint(16)):uint(16) else max(uint(64)):uint(64);
  writeln("testParamExpressionMax: when true uint(16) uint(64) then ", x.type:string);
}

{
  proc testProcCastZero(arg: bool) {
    if arg then return 0:uint(32); else return 0:int(8);
  }
  var x = testProcCastZero(true);
  writeln("testProcCastZero: when true uint(32) int(8) then ", x.type:string);
}

{
  proc testProcZero(arg: bool) {
    var i: uint(32) = 0;
    var u: int(8) = 0;
    if arg then return i; else return u;
  }
  var x = testProcZero(true);
  writeln("testProcZero: when true uint(32) int(8) then ", x.type:string);
}

{
  proc testProcCastMax(arg: bool) {
    if arg then return max(uint(32)):uint(32); else return max(int(8)):int(8);
  }
  var x = testProcCastMax(true);
  writeln("testProcCastMax: when true uint(32) int(8) then ", x.type:string);
}

{
  proc testProcMax(arg: bool) {
    var i: uint(32) = max(uint(32));
    var u: int(8) = max(int(8));
    if arg then return i; else return u;
  }
  var x = testProcMax(true);
  writeln("testProcMax: when true uint(32) int(8) then ", x.type:string);
}

{
  proc testParamProcCastZero(param arg: bool) param {
    if arg then return 0:uint(32); else return 0:int(8);
  }
  param x = testParamProcCastZero(true);
  writeln("testParamProcCastZero: when true uint(32) int(8) then ", x.type:string);
}

{
  proc testParamProcZero(param arg: bool) param {
    param i: uint(32) = 0;
    param u: int(8) = 0;
    if arg then return i; else return u;
  }
  param x = testParamProcZero(true);
  writeln("testParamProcZero: when true uint(32) int(8) then ", x.type:string);
}

{
  proc testParamProcCastMax(param arg: bool) param {
    if arg then return max(uint(32)):uint(32); else return max(int(8)):int(8);
  }
  param x = testParamProcCastMax(true);
  writeln("testParamProcCastMax: when true uint(32) int(8) then ", x.type:string);
}

{
  proc testParamProcMax(param arg: bool) param {
    param i: uint(32) = max(uint(32));
    param u: int(8) = max(int(8));
    if arg then return i; else return u;
  }
  param x = testParamProcMax(true);
  writeln("testParamProcMax: when true uint(32) int(8) then ", x.type:string);
}

{
  var b: bool = true;
  var x = if b then 0:uint(32) else 0:int(8);
  writeln("testExpressionZero: when true uint(32) int(8) then ", x.type:string);
}

{
  var b: bool = true;
  var x = if b then max(uint(32)):uint(32) else max(int(8)):int(8);
  writeln("testExpressionMax: when true uint(32) int(8) then ", x.type:string);
}

{
  param x = if true then 0:uint(32) else 0:int(8);
  writeln("testParamExpressionZero: when true uint(32) int(8) then ", x.type:string);
}

{
  param x = if true then max(uint(32)):uint(32) else max(int(8)):int(8);
  writeln("testParamExpressionMax: when true uint(32) int(8) then ", x.type:string);
}

{
  proc testProcCastZero(arg: bool) {
    if arg then return 0:uint(32); else return 0:int(16);
  }
  var x = testProcCastZero(true);
  writeln("testProcCastZero: when true uint(32) int(16) then ", x.type:string);
}

{
  proc testProcZero(arg: bool) {
    var i: uint(32) = 0;
    var u: int(16) = 0;
    if arg then return i; else return u;
  }
  var x = testProcZero(true);
  writeln("testProcZero: when true uint(32) int(16) then ", x.type:string);
}

{
  proc testProcCastMax(arg: bool) {
    if arg then return max(uint(32)):uint(32); else return max(int(16)):int(16);
  }
  var x = testProcCastMax(true);
  writeln("testProcCastMax: when true uint(32) int(16) then ", x.type:string);
}

{
  proc testProcMax(arg: bool) {
    var i: uint(32) = max(uint(32));
    var u: int(16) = max(int(16));
    if arg then return i; else return u;
  }
  var x = testProcMax(true);
  writeln("testProcMax: when true uint(32) int(16) then ", x.type:string);
}

{
  proc testParamProcCastZero(param arg: bool) param {
    if arg then return 0:uint(32); else return 0:int(16);
  }
  param x = testParamProcCastZero(true);
  writeln("testParamProcCastZero: when true uint(32) int(16) then ", x.type:string);
}

{
  proc testParamProcZero(param arg: bool) param {
    param i: uint(32) = 0;
    param u: int(16) = 0;
    if arg then return i; else return u;
  }
  param x = testParamProcZero(true);
  writeln("testParamProcZero: when true uint(32) int(16) then ", x.type:string);
}

{
  proc testParamProcCastMax(param arg: bool) param {
    if arg then return max(uint(32)):uint(32); else return max(int(16)):int(16);
  }
  param x = testParamProcCastMax(true);
  writeln("testParamProcCastMax: when true uint(32) int(16) then ", x.type:string);
}

{
  proc testParamProcMax(param arg: bool) param {
    param i: uint(32) = max(uint(32));
    param u: int(16) = max(int(16));
    if arg then return i; else return u;
  }
  param x = testParamProcMax(true);
  writeln("testParamProcMax: when true uint(32) int(16) then ", x.type:string);
}

{
  var b: bool = true;
  var x = if b then 0:uint(32) else 0:int(16);
  writeln("testExpressionZero: when true uint(32) int(16) then ", x.type:string);
}

{
  var b: bool = true;
  var x = if b then max(uint(32)):uint(32) else max(int(16)):int(16);
  writeln("testExpressionMax: when true uint(32) int(16) then ", x.type:string);
}

{
  param x = if true then 0:uint(32) else 0:int(16);
  writeln("testParamExpressionZero: when true uint(32) int(16) then ", x.type:string);
}

{
  param x = if true then max(uint(32)):uint(32) else max(int(16)):int(16);
  writeln("testParamExpressionMax: when true uint(32) int(16) then ", x.type:string);
}

{
  proc testProcCastZero(arg: bool) {
    if arg then return 0:uint(32); else return 0:int(32);
  }
  var x = testProcCastZero(true);
  writeln("testProcCastZero: when true uint(32) int(32) then ", x.type:string);
}

{
  proc testProcZero(arg: bool) {
    var i: uint(32) = 0;
    var u: int(32) = 0;
    if arg then return i; else return u;
  }
  var x = testProcZero(true);
  writeln("testProcZero: when true uint(32) int(32) then ", x.type:string);
}

{
  proc testProcCastMax(arg: bool) {
    if arg then return max(uint(32)):uint(32); else return max(int(32)):int(32);
  }
  var x = testProcCastMax(true);
  writeln("testProcCastMax: when true uint(32) int(32) then ", x.type:string);
}

{
  proc testProcMax(arg: bool) {
    var i: uint(32) = max(uint(32));
    var u: int(32) = max(int(32));
    if arg then return i; else return u;
  }
  var x = testProcMax(true);
  writeln("testProcMax: when true uint(32) int(32) then ", x.type:string);
}

{
  proc testParamProcCastZero(param arg: bool) param {
    if arg then return 0:uint(32); else return 0:int(32);
  }
  param x = testParamProcCastZero(true);
  writeln("testParamProcCastZero: when true uint(32) int(32) then ", x.type:string);
}

{
  proc testParamProcZero(param arg: bool) param {
    param i: uint(32) = 0;
    param u: int(32) = 0;
    if arg then return i; else return u;
  }
  param x = testParamProcZero(true);
  writeln("testParamProcZero: when true uint(32) int(32) then ", x.type:string);
}

{
  proc testParamProcCastMax(param arg: bool) param {
    if arg then return max(uint(32)):uint(32); else return max(int(32)):int(32);
  }
  param x = testParamProcCastMax(true);
  writeln("testParamProcCastMax: when true uint(32) int(32) then ", x.type:string);
}

{
  proc testParamProcMax(param arg: bool) param {
    param i: uint(32) = max(uint(32));
    param u: int(32) = max(int(32));
    if arg then return i; else return u;
  }
  param x = testParamProcMax(true);
  writeln("testParamProcMax: when true uint(32) int(32) then ", x.type:string);
}

{
  var b: bool = true;
  var x = if b then 0:uint(32) else 0:int(32);
  writeln("testExpressionZero: when true uint(32) int(32) then ", x.type:string);
}

{
  var b: bool = true;
  var x = if b then max(uint(32)):uint(32) else max(int(32)):int(32);
  writeln("testExpressionMax: when true uint(32) int(32) then ", x.type:string);
}

{
  param x = if true then 0:uint(32) else 0:int(32);
  writeln("testParamExpressionZero: when true uint(32) int(32) then ", x.type:string);
}

{
  param x = if true then max(uint(32)):uint(32) else max(int(32)):int(32);
  writeln("testParamExpressionMax: when true uint(32) int(32) then ", x.type:string);
}

{
  proc testProcCastZero(arg: bool) {
    if arg then return 0:uint(32); else return 0:int(64);
  }
  var x = testProcCastZero(true);
  writeln("testProcCastZero: when true uint(32) int(64) then ", x.type:string);
}

{
  proc testProcZero(arg: bool) {
    var i: uint(32) = 0;
    var u: int(64) = 0;
    if arg then return i; else return u;
  }
  var x = testProcZero(true);
  writeln("testProcZero: when true uint(32) int(64) then ", x.type:string);
}

{
  proc testProcCastMax(arg: bool) {
    if arg then return max(uint(32)):uint(32); else return max(int(64)):int(64);
  }
  var x = testProcCastMax(true);
  writeln("testProcCastMax: when true uint(32) int(64) then ", x.type:string);
}

{
  proc testProcMax(arg: bool) {
    var i: uint(32) = max(uint(32));
    var u: int(64) = max(int(64));
    if arg then return i; else return u;
  }
  var x = testProcMax(true);
  writeln("testProcMax: when true uint(32) int(64) then ", x.type:string);
}

{
  proc testParamProcCastZero(param arg: bool) param {
    if arg then return 0:uint(32); else return 0:int(64);
  }
  param x = testParamProcCastZero(true);
  writeln("testParamProcCastZero: when true uint(32) int(64) then ", x.type:string);
}

{
  proc testParamProcZero(param arg: bool) param {
    param i: uint(32) = 0;
    param u: int(64) = 0;
    if arg then return i; else return u;
  }
  param x = testParamProcZero(true);
  writeln("testParamProcZero: when true uint(32) int(64) then ", x.type:string);
}

{
  proc testParamProcCastMax(param arg: bool) param {
    if arg then return max(uint(32)):uint(32); else return max(int(64)):int(64);
  }
  param x = testParamProcCastMax(true);
  writeln("testParamProcCastMax: when true uint(32) int(64) then ", x.type:string);
}

{
  proc testParamProcMax(param arg: bool) param {
    param i: uint(32) = max(uint(32));
    param u: int(64) = max(int(64));
    if arg then return i; else return u;
  }
  param x = testParamProcMax(true);
  writeln("testParamProcMax: when true uint(32) int(64) then ", x.type:string);
}

{
  var b: bool = true;
  var x = if b then 0:uint(32) else 0:int(64);
  writeln("testExpressionZero: when true uint(32) int(64) then ", x.type:string);
}

{
  var b: bool = true;
  var x = if b then max(uint(32)):uint(32) else max(int(64)):int(64);
  writeln("testExpressionMax: when true uint(32) int(64) then ", x.type:string);
}

{
  param x = if true then 0:uint(32) else 0:int(64);
  writeln("testParamExpressionZero: when true uint(32) int(64) then ", x.type:string);
}

{
  param x = if true then max(uint(32)):uint(32) else max(int(64)):int(64);
  writeln("testParamExpressionMax: when true uint(32) int(64) then ", x.type:string);
}

{
  proc testProcCastZero(arg: bool) {
    if arg then return 0:uint(32); else return 0:uint(8);
  }
  var x = testProcCastZero(true);
  writeln("testProcCastZero: when true uint(32) uint(8) then ", x.type:string);
}

{
  proc testProcZero(arg: bool) {
    var i: uint(32) = 0;
    var u: uint(8) = 0;
    if arg then return i; else return u;
  }
  var x = testProcZero(true);
  writeln("testProcZero: when true uint(32) uint(8) then ", x.type:string);
}

{
  proc testProcCastMax(arg: bool) {
    if arg then return max(uint(32)):uint(32); else return max(uint(8)):uint(8);
  }
  var x = testProcCastMax(true);
  writeln("testProcCastMax: when true uint(32) uint(8) then ", x.type:string);
}

{
  proc testProcMax(arg: bool) {
    var i: uint(32) = max(uint(32));
    var u: uint(8) = max(uint(8));
    if arg then return i; else return u;
  }
  var x = testProcMax(true);
  writeln("testProcMax: when true uint(32) uint(8) then ", x.type:string);
}

{
  proc testParamProcCastZero(param arg: bool) param {
    if arg then return 0:uint(32); else return 0:uint(8);
  }
  param x = testParamProcCastZero(true);
  writeln("testParamProcCastZero: when true uint(32) uint(8) then ", x.type:string);
}

{
  proc testParamProcZero(param arg: bool) param {
    param i: uint(32) = 0;
    param u: uint(8) = 0;
    if arg then return i; else return u;
  }
  param x = testParamProcZero(true);
  writeln("testParamProcZero: when true uint(32) uint(8) then ", x.type:string);
}

{
  proc testParamProcCastMax(param arg: bool) param {
    if arg then return max(uint(32)):uint(32); else return max(uint(8)):uint(8);
  }
  param x = testParamProcCastMax(true);
  writeln("testParamProcCastMax: when true uint(32) uint(8) then ", x.type:string);
}

{
  proc testParamProcMax(param arg: bool) param {
    param i: uint(32) = max(uint(32));
    param u: uint(8) = max(uint(8));
    if arg then return i; else return u;
  }
  param x = testParamProcMax(true);
  writeln("testParamProcMax: when true uint(32) uint(8) then ", x.type:string);
}

{
  var b: bool = true;
  var x = if b then 0:uint(32) else 0:uint(8);
  writeln("testExpressionZero: when true uint(32) uint(8) then ", x.type:string);
}

{
  var b: bool = true;
  var x = if b then max(uint(32)):uint(32) else max(uint(8)):uint(8);
  writeln("testExpressionMax: when true uint(32) uint(8) then ", x.type:string);
}

{
  param x = if true then 0:uint(32) else 0:uint(8);
  writeln("testParamExpressionZero: when true uint(32) uint(8) then ", x.type:string);
}

{
  param x = if true then max(uint(32)):uint(32) else max(uint(8)):uint(8);
  writeln("testParamExpressionMax: when true uint(32) uint(8) then ", x.type:string);
}

{
  proc testProcCastZero(arg: bool) {
    if arg then return 0:uint(32); else return 0:uint(16);
  }
  var x = testProcCastZero(true);
  writeln("testProcCastZero: when true uint(32) uint(16) then ", x.type:string);
}

{
  proc testProcZero(arg: bool) {
    var i: uint(32) = 0;
    var u: uint(16) = 0;
    if arg then return i; else return u;
  }
  var x = testProcZero(true);
  writeln("testProcZero: when true uint(32) uint(16) then ", x.type:string);
}

{
  proc testProcCastMax(arg: bool) {
    if arg then return max(uint(32)):uint(32); else return max(uint(16)):uint(16);
  }
  var x = testProcCastMax(true);
  writeln("testProcCastMax: when true uint(32) uint(16) then ", x.type:string);
}

{
  proc testProcMax(arg: bool) {
    var i: uint(32) = max(uint(32));
    var u: uint(16) = max(uint(16));
    if arg then return i; else return u;
  }
  var x = testProcMax(true);
  writeln("testProcMax: when true uint(32) uint(16) then ", x.type:string);
}

{
  proc testParamProcCastZero(param arg: bool) param {
    if arg then return 0:uint(32); else return 0:uint(16);
  }
  param x = testParamProcCastZero(true);
  writeln("testParamProcCastZero: when true uint(32) uint(16) then ", x.type:string);
}

{
  proc testParamProcZero(param arg: bool) param {
    param i: uint(32) = 0;
    param u: uint(16) = 0;
    if arg then return i; else return u;
  }
  param x = testParamProcZero(true);
  writeln("testParamProcZero: when true uint(32) uint(16) then ", x.type:string);
}

{
  proc testParamProcCastMax(param arg: bool) param {
    if arg then return max(uint(32)):uint(32); else return max(uint(16)):uint(16);
  }
  param x = testParamProcCastMax(true);
  writeln("testParamProcCastMax: when true uint(32) uint(16) then ", x.type:string);
}

{
  proc testParamProcMax(param arg: bool) param {
    param i: uint(32) = max(uint(32));
    param u: uint(16) = max(uint(16));
    if arg then return i; else return u;
  }
  param x = testParamProcMax(true);
  writeln("testParamProcMax: when true uint(32) uint(16) then ", x.type:string);
}

{
  var b: bool = true;
  var x = if b then 0:uint(32) else 0:uint(16);
  writeln("testExpressionZero: when true uint(32) uint(16) then ", x.type:string);
}

{
  var b: bool = true;
  var x = if b then max(uint(32)):uint(32) else max(uint(16)):uint(16);
  writeln("testExpressionMax: when true uint(32) uint(16) then ", x.type:string);
}

{
  param x = if true then 0:uint(32) else 0:uint(16);
  writeln("testParamExpressionZero: when true uint(32) uint(16) then ", x.type:string);
}

{
  param x = if true then max(uint(32)):uint(32) else max(uint(16)):uint(16);
  writeln("testParamExpressionMax: when true uint(32) uint(16) then ", x.type:string);
}

{
  proc testProcCastZero(arg: bool) {
    if arg then return 0:uint(32); else return 0:uint(32);
  }
  var x = testProcCastZero(true);
  writeln("testProcCastZero: when true uint(32) uint(32) then ", x.type:string);
}

{
  proc testProcZero(arg: bool) {
    var i: uint(32) = 0;
    var u: uint(32) = 0;
    if arg then return i; else return u;
  }
  var x = testProcZero(true);
  writeln("testProcZero: when true uint(32) uint(32) then ", x.type:string);
}

{
  proc testProcCastMax(arg: bool) {
    if arg then return max(uint(32)):uint(32); else return max(uint(32)):uint(32);
  }
  var x = testProcCastMax(true);
  writeln("testProcCastMax: when true uint(32) uint(32) then ", x.type:string);
}

{
  proc testProcMax(arg: bool) {
    var i: uint(32) = max(uint(32));
    var u: uint(32) = max(uint(32));
    if arg then return i; else return u;
  }
  var x = testProcMax(true);
  writeln("testProcMax: when true uint(32) uint(32) then ", x.type:string);
}

{
  proc testParamProcCastZero(param arg: bool) param {
    if arg then return 0:uint(32); else return 0:uint(32);
  }
  param x = testParamProcCastZero(true);
  writeln("testParamProcCastZero: when true uint(32) uint(32) then ", x.type:string);
}

{
  proc testParamProcZero(param arg: bool) param {
    param i: uint(32) = 0;
    param u: uint(32) = 0;
    if arg then return i; else return u;
  }
  param x = testParamProcZero(true);
  writeln("testParamProcZero: when true uint(32) uint(32) then ", x.type:string);
}

{
  proc testParamProcCastMax(param arg: bool) param {
    if arg then return max(uint(32)):uint(32); else return max(uint(32)):uint(32);
  }
  param x = testParamProcCastMax(true);
  writeln("testParamProcCastMax: when true uint(32) uint(32) then ", x.type:string);
}

{
  proc testParamProcMax(param arg: bool) param {
    param i: uint(32) = max(uint(32));
    param u: uint(32) = max(uint(32));
    if arg then return i; else return u;
  }
  param x = testParamProcMax(true);
  writeln("testParamProcMax: when true uint(32) uint(32) then ", x.type:string);
}

{
  var b: bool = true;
  var x = if b then 0:uint(32) else 0:uint(32);
  writeln("testExpressionZero: when true uint(32) uint(32) then ", x.type:string);
}

{
  var b: bool = true;
  var x = if b then max(uint(32)):uint(32) else max(uint(32)):uint(32);
  writeln("testExpressionMax: when true uint(32) uint(32) then ", x.type:string);
}

{
  param x = if true then 0:uint(32) else 0:uint(32);
  writeln("testParamExpressionZero: when true uint(32) uint(32) then ", x.type:string);
}

{
  param x = if true then max(uint(32)):uint(32) else max(uint(32)):uint(32);
  writeln("testParamExpressionMax: when true uint(32) uint(32) then ", x.type:string);
}

{
  proc testProcCastZero(arg: bool) {
    if arg then return 0:uint(32); else return 0:uint(64);
  }
  var x = testProcCastZero(true);
  writeln("testProcCastZero: when true uint(32) uint(64) then ", x.type:string);
}

{
  proc testProcZero(arg: bool) {
    var i: uint(32) = 0;
    var u: uint(64) = 0;
    if arg then return i; else return u;
  }
  var x = testProcZero(true);
  writeln("testProcZero: when true uint(32) uint(64) then ", x.type:string);
}

{
  proc testProcCastMax(arg: bool) {
    if arg then return max(uint(32)):uint(32); else return max(uint(64)):uint(64);
  }
  var x = testProcCastMax(true);
  writeln("testProcCastMax: when true uint(32) uint(64) then ", x.type:string);
}

{
  proc testProcMax(arg: bool) {
    var i: uint(32) = max(uint(32));
    var u: uint(64) = max(uint(64));
    if arg then return i; else return u;
  }
  var x = testProcMax(true);
  writeln("testProcMax: when true uint(32) uint(64) then ", x.type:string);
}

{
  proc testParamProcCastZero(param arg: bool) param {
    if arg then return 0:uint(32); else return 0:uint(64);
  }
  param x = testParamProcCastZero(true);
  writeln("testParamProcCastZero: when true uint(32) uint(64) then ", x.type:string);
}

{
  proc testParamProcZero(param arg: bool) param {
    param i: uint(32) = 0;
    param u: uint(64) = 0;
    if arg then return i; else return u;
  }
  param x = testParamProcZero(true);
  writeln("testParamProcZero: when true uint(32) uint(64) then ", x.type:string);
}

{
  proc testParamProcCastMax(param arg: bool) param {
    if arg then return max(uint(32)):uint(32); else return max(uint(64)):uint(64);
  }
  param x = testParamProcCastMax(true);
  writeln("testParamProcCastMax: when true uint(32) uint(64) then ", x.type:string);
}

{
  proc testParamProcMax(param arg: bool) param {
    param i: uint(32) = max(uint(32));
    param u: uint(64) = max(uint(64));
    if arg then return i; else return u;
  }
  param x = testParamProcMax(true);
  writeln("testParamProcMax: when true uint(32) uint(64) then ", x.type:string);
}

{
  var b: bool = true;
  var x = if b then 0:uint(32) else 0:uint(64);
  writeln("testExpressionZero: when true uint(32) uint(64) then ", x.type:string);
}

{
  var b: bool = true;
  var x = if b then max(uint(32)):uint(32) else max(uint(64)):uint(64);
  writeln("testExpressionMax: when true uint(32) uint(64) then ", x.type:string);
}

{
  param x = if true then 0:uint(32) else 0:uint(64);
  writeln("testParamExpressionZero: when true uint(32) uint(64) then ", x.type:string);
}

{
  param x = if true then max(uint(32)):uint(32) else max(uint(64)):uint(64);
  writeln("testParamExpressionMax: when true uint(32) uint(64) then ", x.type:string);
}

{
  proc testProcCastZero(arg: bool) {
    if arg then return 0:uint(64); else return 0:int(8);
  }
  var x = testProcCastZero(true);
  writeln("testProcCastZero: when true uint(64) int(8) then ", x.type:string);
}

{
  proc testProcZero(arg: bool) {
    var i: uint(64) = 0;
    var u: int(8) = 0;
    if arg then return i; else return u;
  }
  var x = testProcZero(true);
  writeln("testProcZero: when true uint(64) int(8) then ", x.type:string);
}

{
  proc testProcCastMax(arg: bool) {
    if arg then return max(uint(64)):uint(64); else return max(int(8)):int(8);
  }
  var x = testProcCastMax(true);
  writeln("testProcCastMax: when true uint(64) int(8) then ", x.type:string);
}

{
  proc testProcMax(arg: bool) {
    var i: uint(64) = max(uint(64));
    var u: int(8) = max(int(8));
    if arg then return i; else return u;
  }
  var x = testProcMax(true);
  writeln("testProcMax: when true uint(64) int(8) then ", x.type:string);
}

{
  proc testParamProcCastZero(param arg: bool) param {
    if arg then return 0:uint(64); else return 0:int(8);
  }
  param x = testParamProcCastZero(true);
  writeln("testParamProcCastZero: when true uint(64) int(8) then ", x.type:string);
}

{
  proc testParamProcZero(param arg: bool) param {
    param i: uint(64) = 0;
    param u: int(8) = 0;
    if arg then return i; else return u;
  }
  param x = testParamProcZero(true);
  writeln("testParamProcZero: when true uint(64) int(8) then ", x.type:string);
}

{
  proc testParamProcCastMax(param arg: bool) param {
    if arg then return max(uint(64)):uint(64); else return max(int(8)):int(8);
  }
  param x = testParamProcCastMax(true);
  writeln("testParamProcCastMax: when true uint(64) int(8) then ", x.type:string);
}

{
  proc testParamProcMax(param arg: bool) param {
    param i: uint(64) = max(uint(64));
    param u: int(8) = max(int(8));
    if arg then return i; else return u;
  }
  param x = testParamProcMax(true);
  writeln("testParamProcMax: when true uint(64) int(8) then ", x.type:string);
}

{
  var b: bool = true;
  var x = if b then 0:uint(64) else 0:int(8);
  writeln("testExpressionZero: when true uint(64) int(8) then ", x.type:string);
}

{
  var b: bool = true;
  var x = if b then max(uint(64)):uint(64) else max(int(8)):int(8);
  writeln("testExpressionMax: when true uint(64) int(8) then ", x.type:string);
}

{
  param x = if true then 0:uint(64) else 0:int(8);
  writeln("testParamExpressionZero: when true uint(64) int(8) then ", x.type:string);
}

{
  param x = if true then max(uint(64)):uint(64) else max(int(8)):int(8);
  writeln("testParamExpressionMax: when true uint(64) int(8) then ", x.type:string);
}

{
  proc testProcCastZero(arg: bool) {
    if arg then return 0:uint(64); else return 0:int(16);
  }
  var x = testProcCastZero(true);
  writeln("testProcCastZero: when true uint(64) int(16) then ", x.type:string);
}

{
  proc testProcZero(arg: bool) {
    var i: uint(64) = 0;
    var u: int(16) = 0;
    if arg then return i; else return u;
  }
  var x = testProcZero(true);
  writeln("testProcZero: when true uint(64) int(16) then ", x.type:string);
}

{
  proc testProcCastMax(arg: bool) {
    if arg then return max(uint(64)):uint(64); else return max(int(16)):int(16);
  }
  var x = testProcCastMax(true);
  writeln("testProcCastMax: when true uint(64) int(16) then ", x.type:string);
}

{
  proc testProcMax(arg: bool) {
    var i: uint(64) = max(uint(64));
    var u: int(16) = max(int(16));
    if arg then return i; else return u;
  }
  var x = testProcMax(true);
  writeln("testProcMax: when true uint(64) int(16) then ", x.type:string);
}

{
  proc testParamProcCastZero(param arg: bool) param {
    if arg then return 0:uint(64); else return 0:int(16);
  }
  param x = testParamProcCastZero(true);
  writeln("testParamProcCastZero: when true uint(64) int(16) then ", x.type:string);
}

{
  proc testParamProcZero(param arg: bool) param {
    param i: uint(64) = 0;
    param u: int(16) = 0;
    if arg then return i; else return u;
  }
  param x = testParamProcZero(true);
  writeln("testParamProcZero: when true uint(64) int(16) then ", x.type:string);
}

{
  proc testParamProcCastMax(param arg: bool) param {
    if arg then return max(uint(64)):uint(64); else return max(int(16)):int(16);
  }
  param x = testParamProcCastMax(true);
  writeln("testParamProcCastMax: when true uint(64) int(16) then ", x.type:string);
}

{
  proc testParamProcMax(param arg: bool) param {
    param i: uint(64) = max(uint(64));
    param u: int(16) = max(int(16));
    if arg then return i; else return u;
  }
  param x = testParamProcMax(true);
  writeln("testParamProcMax: when true uint(64) int(16) then ", x.type:string);
}

{
  var b: bool = true;
  var x = if b then 0:uint(64) else 0:int(16);
  writeln("testExpressionZero: when true uint(64) int(16) then ", x.type:string);
}

{
  var b: bool = true;
  var x = if b then max(uint(64)):uint(64) else max(int(16)):int(16);
  writeln("testExpressionMax: when true uint(64) int(16) then ", x.type:string);
}

{
  param x = if true then 0:uint(64) else 0:int(16);
  writeln("testParamExpressionZero: when true uint(64) int(16) then ", x.type:string);
}

{
  param x = if true then max(uint(64)):uint(64) else max(int(16)):int(16);
  writeln("testParamExpressionMax: when true uint(64) int(16) then ", x.type:string);
}

{
  proc testProcCastZero(arg: bool) {
    if arg then return 0:uint(64); else return 0:int(32);
  }
  var x = testProcCastZero(true);
  writeln("testProcCastZero: when true uint(64) int(32) then ", x.type:string);
}

{
  proc testProcZero(arg: bool) {
    var i: uint(64) = 0;
    var u: int(32) = 0;
    if arg then return i; else return u;
  }
  var x = testProcZero(true);
  writeln("testProcZero: when true uint(64) int(32) then ", x.type:string);
}

{
  proc testProcCastMax(arg: bool) {
    if arg then return max(uint(64)):uint(64); else return max(int(32)):int(32);
  }
  var x = testProcCastMax(true);
  writeln("testProcCastMax: when true uint(64) int(32) then ", x.type:string);
}

{
  proc testProcMax(arg: bool) {
    var i: uint(64) = max(uint(64));
    var u: int(32) = max(int(32));
    if arg then return i; else return u;
  }
  var x = testProcMax(true);
  writeln("testProcMax: when true uint(64) int(32) then ", x.type:string);
}

{
  proc testParamProcCastZero(param arg: bool) param {
    if arg then return 0:uint(64); else return 0:int(32);
  }
  param x = testParamProcCastZero(true);
  writeln("testParamProcCastZero: when true uint(64) int(32) then ", x.type:string);
}

{
  proc testParamProcZero(param arg: bool) param {
    param i: uint(64) = 0;
    param u: int(32) = 0;
    if arg then return i; else return u;
  }
  param x = testParamProcZero(true);
  writeln("testParamProcZero: when true uint(64) int(32) then ", x.type:string);
}

{
  proc testParamProcCastMax(param arg: bool) param {
    if arg then return max(uint(64)):uint(64); else return max(int(32)):int(32);
  }
  param x = testParamProcCastMax(true);
  writeln("testParamProcCastMax: when true uint(64) int(32) then ", x.type:string);
}

{
  proc testParamProcMax(param arg: bool) param {
    param i: uint(64) = max(uint(64));
    param u: int(32) = max(int(32));
    if arg then return i; else return u;
  }
  param x = testParamProcMax(true);
  writeln("testParamProcMax: when true uint(64) int(32) then ", x.type:string);
}

{
  var b: bool = true;
  var x = if b then 0:uint(64) else 0:int(32);
  writeln("testExpressionZero: when true uint(64) int(32) then ", x.type:string);
}

{
  var b: bool = true;
  var x = if b then max(uint(64)):uint(64) else max(int(32)):int(32);
  writeln("testExpressionMax: when true uint(64) int(32) then ", x.type:string);
}

{
  param x = if true then 0:uint(64) else 0:int(32);
  writeln("testParamExpressionZero: when true uint(64) int(32) then ", x.type:string);
}

{
  param x = if true then max(uint(64)):uint(64) else max(int(32)):int(32);
  writeln("testParamExpressionMax: when true uint(64) int(32) then ", x.type:string);
}

{
  proc testProcCastZero(arg: bool) {
    if arg then return 0:uint(64); else return 0:int(64);
  }
  var x = testProcCastZero(true);
  writeln("testProcCastZero: when true uint(64) int(64) then ", x.type:string);
}

{
  proc testProcZero(arg: bool) {
    var i: uint(64) = 0;
    var u: int(64) = 0;
    if arg then return i; else return u;
  }
  var x = testProcZero(true);
  writeln("testProcZero: when true uint(64) int(64) then ", x.type:string);
}

{
  proc testProcCastMax(arg: bool) {
    if arg then return max(uint(64)):uint(64); else return max(int(64)):int(64);
  }
  var x = testProcCastMax(true);
  writeln("testProcCastMax: when true uint(64) int(64) then ", x.type:string);
}

{
  proc testProcMax(arg: bool) {
    var i: uint(64) = max(uint(64));
    var u: int(64) = max(int(64));
    if arg then return i; else return u;
  }
  var x = testProcMax(true);
  writeln("testProcMax: when true uint(64) int(64) then ", x.type:string);
}

{
  proc testParamProcCastZero(param arg: bool) param {
    if arg then return 0:uint(64); else return 0:int(64);
  }
  param x = testParamProcCastZero(true);
  writeln("testParamProcCastZero: when true uint(64) int(64) then ", x.type:string);
}

{
  proc testParamProcZero(param arg: bool) param {
    param i: uint(64) = 0;
    param u: int(64) = 0;
    if arg then return i; else return u;
  }
  param x = testParamProcZero(true);
  writeln("testParamProcZero: when true uint(64) int(64) then ", x.type:string);
}

{
  proc testParamProcCastMax(param arg: bool) param {
    if arg then return max(uint(64)):uint(64); else return max(int(64)):int(64);
  }
  param x = testParamProcCastMax(true);
  writeln("testParamProcCastMax: when true uint(64) int(64) then ", x.type:string);
}

{
  proc testParamProcMax(param arg: bool) param {
    param i: uint(64) = max(uint(64));
    param u: int(64) = max(int(64));
    if arg then return i; else return u;
  }
  param x = testParamProcMax(true);
  writeln("testParamProcMax: when true uint(64) int(64) then ", x.type:string);
}

{
  var b: bool = true;
  var x = if b then 0:uint(64) else 0:int(64);
  writeln("testExpressionZero: when true uint(64) int(64) then ", x.type:string);
}

{
  var b: bool = true;
  var x = if b then max(uint(64)):uint(64) else max(int(64)):int(64);
  writeln("testExpressionMax: when true uint(64) int(64) then ", x.type:string);
}

{
  param x = if true then 0:uint(64) else 0:int(64);
  writeln("testParamExpressionZero: when true uint(64) int(64) then ", x.type:string);
}

{
  param x = if true then max(uint(64)):uint(64) else max(int(64)):int(64);
  writeln("testParamExpressionMax: when true uint(64) int(64) then ", x.type:string);
}

{
  proc testProcCastZero(arg: bool) {
    if arg then return 0:uint(64); else return 0:uint(8);
  }
  var x = testProcCastZero(true);
  writeln("testProcCastZero: when true uint(64) uint(8) then ", x.type:string);
}

{
  proc testProcZero(arg: bool) {
    var i: uint(64) = 0;
    var u: uint(8) = 0;
    if arg then return i; else return u;
  }
  var x = testProcZero(true);
  writeln("testProcZero: when true uint(64) uint(8) then ", x.type:string);
}

{
  proc testProcCastMax(arg: bool) {
    if arg then return max(uint(64)):uint(64); else return max(uint(8)):uint(8);
  }
  var x = testProcCastMax(true);
  writeln("testProcCastMax: when true uint(64) uint(8) then ", x.type:string);
}

{
  proc testProcMax(arg: bool) {
    var i: uint(64) = max(uint(64));
    var u: uint(8) = max(uint(8));
    if arg then return i; else return u;
  }
  var x = testProcMax(true);
  writeln("testProcMax: when true uint(64) uint(8) then ", x.type:string);
}

{
  proc testParamProcCastZero(param arg: bool) param {
    if arg then return 0:uint(64); else return 0:uint(8);
  }
  param x = testParamProcCastZero(true);
  writeln("testParamProcCastZero: when true uint(64) uint(8) then ", x.type:string);
}

{
  proc testParamProcZero(param arg: bool) param {
    param i: uint(64) = 0;
    param u: uint(8) = 0;
    if arg then return i; else return u;
  }
  param x = testParamProcZero(true);
  writeln("testParamProcZero: when true uint(64) uint(8) then ", x.type:string);
}

{
  proc testParamProcCastMax(param arg: bool) param {
    if arg then return max(uint(64)):uint(64); else return max(uint(8)):uint(8);
  }
  param x = testParamProcCastMax(true);
  writeln("testParamProcCastMax: when true uint(64) uint(8) then ", x.type:string);
}

{
  proc testParamProcMax(param arg: bool) param {
    param i: uint(64) = max(uint(64));
    param u: uint(8) = max(uint(8));
    if arg then return i; else return u;
  }
  param x = testParamProcMax(true);
  writeln("testParamProcMax: when true uint(64) uint(8) then ", x.type:string);
}

{
  var b: bool = true;
  var x = if b then 0:uint(64) else 0:uint(8);
  writeln("testExpressionZero: when true uint(64) uint(8) then ", x.type:string);
}

{
  var b: bool = true;
  var x = if b then max(uint(64)):uint(64) else max(uint(8)):uint(8);
  writeln("testExpressionMax: when true uint(64) uint(8) then ", x.type:string);
}

{
  param x = if true then 0:uint(64) else 0:uint(8);
  writeln("testParamExpressionZero: when true uint(64) uint(8) then ", x.type:string);
}

{
  param x = if true then max(uint(64)):uint(64) else max(uint(8)):uint(8);
  writeln("testParamExpressionMax: when true uint(64) uint(8) then ", x.type:string);
}

{
  proc testProcCastZero(arg: bool) {
    if arg then return 0:uint(64); else return 0:uint(16);
  }
  var x = testProcCastZero(true);
  writeln("testProcCastZero: when true uint(64) uint(16) then ", x.type:string);
}

{
  proc testProcZero(arg: bool) {
    var i: uint(64) = 0;
    var u: uint(16) = 0;
    if arg then return i; else return u;
  }
  var x = testProcZero(true);
  writeln("testProcZero: when true uint(64) uint(16) then ", x.type:string);
}

{
  proc testProcCastMax(arg: bool) {
    if arg then return max(uint(64)):uint(64); else return max(uint(16)):uint(16);
  }
  var x = testProcCastMax(true);
  writeln("testProcCastMax: when true uint(64) uint(16) then ", x.type:string);
}

{
  proc testProcMax(arg: bool) {
    var i: uint(64) = max(uint(64));
    var u: uint(16) = max(uint(16));
    if arg then return i; else return u;
  }
  var x = testProcMax(true);
  writeln("testProcMax: when true uint(64) uint(16) then ", x.type:string);
}

{
  proc testParamProcCastZero(param arg: bool) param {
    if arg then return 0:uint(64); else return 0:uint(16);
  }
  param x = testParamProcCastZero(true);
  writeln("testParamProcCastZero: when true uint(64) uint(16) then ", x.type:string);
}

{
  proc testParamProcZero(param arg: bool) param {
    param i: uint(64) = 0;
    param u: uint(16) = 0;
    if arg then return i; else return u;
  }
  param x = testParamProcZero(true);
  writeln("testParamProcZero: when true uint(64) uint(16) then ", x.type:string);
}

{
  proc testParamProcCastMax(param arg: bool) param {
    if arg then return max(uint(64)):uint(64); else return max(uint(16)):uint(16);
  }
  param x = testParamProcCastMax(true);
  writeln("testParamProcCastMax: when true uint(64) uint(16) then ", x.type:string);
}

{
  proc testParamProcMax(param arg: bool) param {
    param i: uint(64) = max(uint(64));
    param u: uint(16) = max(uint(16));
    if arg then return i; else return u;
  }
  param x = testParamProcMax(true);
  writeln("testParamProcMax: when true uint(64) uint(16) then ", x.type:string);
}

{
  var b: bool = true;
  var x = if b then 0:uint(64) else 0:uint(16);
  writeln("testExpressionZero: when true uint(64) uint(16) then ", x.type:string);
}

{
  var b: bool = true;
  var x = if b then max(uint(64)):uint(64) else max(uint(16)):uint(16);
  writeln("testExpressionMax: when true uint(64) uint(16) then ", x.type:string);
}

{
  param x = if true then 0:uint(64) else 0:uint(16);
  writeln("testParamExpressionZero: when true uint(64) uint(16) then ", x.type:string);
}

{
  param x = if true then max(uint(64)):uint(64) else max(uint(16)):uint(16);
  writeln("testParamExpressionMax: when true uint(64) uint(16) then ", x.type:string);
}

{
  proc testProcCastZero(arg: bool) {
    if arg then return 0:uint(64); else return 0:uint(32);
  }
  var x = testProcCastZero(true);
  writeln("testProcCastZero: when true uint(64) uint(32) then ", x.type:string);
}

{
  proc testProcZero(arg: bool) {
    var i: uint(64) = 0;
    var u: uint(32) = 0;
    if arg then return i; else return u;
  }
  var x = testProcZero(true);
  writeln("testProcZero: when true uint(64) uint(32) then ", x.type:string);
}

{
  proc testProcCastMax(arg: bool) {
    if arg then return max(uint(64)):uint(64); else return max(uint(32)):uint(32);
  }
  var x = testProcCastMax(true);
  writeln("testProcCastMax: when true uint(64) uint(32) then ", x.type:string);
}

{
  proc testProcMax(arg: bool) {
    var i: uint(64) = max(uint(64));
    var u: uint(32) = max(uint(32));
    if arg then return i; else return u;
  }
  var x = testProcMax(true);
  writeln("testProcMax: when true uint(64) uint(32) then ", x.type:string);
}

{
  proc testParamProcCastZero(param arg: bool) param {
    if arg then return 0:uint(64); else return 0:uint(32);
  }
  param x = testParamProcCastZero(true);
  writeln("testParamProcCastZero: when true uint(64) uint(32) then ", x.type:string);
}

{
  proc testParamProcZero(param arg: bool) param {
    param i: uint(64) = 0;
    param u: uint(32) = 0;
    if arg then return i; else return u;
  }
  param x = testParamProcZero(true);
  writeln("testParamProcZero: when true uint(64) uint(32) then ", x.type:string);
}

{
  proc testParamProcCastMax(param arg: bool) param {
    if arg then return max(uint(64)):uint(64); else return max(uint(32)):uint(32);
  }
  param x = testParamProcCastMax(true);
  writeln("testParamProcCastMax: when true uint(64) uint(32) then ", x.type:string);
}

{
  proc testParamProcMax(param arg: bool) param {
    param i: uint(64) = max(uint(64));
    param u: uint(32) = max(uint(32));
    if arg then return i; else return u;
  }
  param x = testParamProcMax(true);
  writeln("testParamProcMax: when true uint(64) uint(32) then ", x.type:string);
}

{
  var b: bool = true;
  var x = if b then 0:uint(64) else 0:uint(32);
  writeln("testExpressionZero: when true uint(64) uint(32) then ", x.type:string);
}

{
  var b: bool = true;
  var x = if b then max(uint(64)):uint(64) else max(uint(32)):uint(32);
  writeln("testExpressionMax: when true uint(64) uint(32) then ", x.type:string);
}

{
  param x = if true then 0:uint(64) else 0:uint(32);
  writeln("testParamExpressionZero: when true uint(64) uint(32) then ", x.type:string);
}

{
  param x = if true then max(uint(64)):uint(64) else max(uint(32)):uint(32);
  writeln("testParamExpressionMax: when true uint(64) uint(32) then ", x.type:string);
}

{
  proc testProcCastZero(arg: bool) {
    if arg then return 0:uint(64); else return 0:uint(64);
  }
  var x = testProcCastZero(true);
  writeln("testProcCastZero: when true uint(64) uint(64) then ", x.type:string);
}

{
  proc testProcZero(arg: bool) {
    var i: uint(64) = 0;
    var u: uint(64) = 0;
    if arg then return i; else return u;
  }
  var x = testProcZero(true);
  writeln("testProcZero: when true uint(64) uint(64) then ", x.type:string);
}

{
  proc testProcCastMax(arg: bool) {
    if arg then return max(uint(64)):uint(64); else return max(uint(64)):uint(64);
  }
  var x = testProcCastMax(true);
  writeln("testProcCastMax: when true uint(64) uint(64) then ", x.type:string);
}

{
  proc testProcMax(arg: bool) {
    var i: uint(64) = max(uint(64));
    var u: uint(64) = max(uint(64));
    if arg then return i; else return u;
  }
  var x = testProcMax(true);
  writeln("testProcMax: when true uint(64) uint(64) then ", x.type:string);
}

{
  proc testParamProcCastZero(param arg: bool) param {
    if arg then return 0:uint(64); else return 0:uint(64);
  }
  param x = testParamProcCastZero(true);
  writeln("testParamProcCastZero: when true uint(64) uint(64) then ", x.type:string);
}

{
  proc testParamProcZero(param arg: bool) param {
    param i: uint(64) = 0;
    param u: uint(64) = 0;
    if arg then return i; else return u;
  }
  param x = testParamProcZero(true);
  writeln("testParamProcZero: when true uint(64) uint(64) then ", x.type:string);
}

{
  proc testParamProcCastMax(param arg: bool) param {
    if arg then return max(uint(64)):uint(64); else return max(uint(64)):uint(64);
  }
  param x = testParamProcCastMax(true);
  writeln("testParamProcCastMax: when true uint(64) uint(64) then ", x.type:string);
}

{
  proc testParamProcMax(param arg: bool) param {
    param i: uint(64) = max(uint(64));
    param u: uint(64) = max(uint(64));
    if arg then return i; else return u;
  }
  param x = testParamProcMax(true);
  writeln("testParamProcMax: when true uint(64) uint(64) then ", x.type:string);
}

{
  var b: bool = true;
  var x = if b then 0:uint(64) else 0:uint(64);
  writeln("testExpressionZero: when true uint(64) uint(64) then ", x.type:string);
}

{
  var b: bool = true;
  var x = if b then max(uint(64)):uint(64) else max(uint(64)):uint(64);
  writeln("testExpressionMax: when true uint(64) uint(64) then ", x.type:string);
}

{
  param x = if true then 0:uint(64) else 0:uint(64);
  writeln("testParamExpressionZero: when true uint(64) uint(64) then ", x.type:string);
}

{
  param x = if true then max(uint(64)):uint(64) else max(uint(64)):uint(64);
  writeln("testParamExpressionMax: when true uint(64) uint(64) then ", x.type:string);
}

{
  proc testProcCastZero(arg: bool) {
    if arg then return 0:int(8); else return 0:int(8);
  }
  var x = testProcCastZero(false);
  writeln("testProcCastZero: when false int(8) int(8) then ", x.type:string);
}

{
  proc testProcZero(arg: bool) {
    var i: int(8) = 0;
    var u: int(8) = 0;
    if arg then return i; else return u;
  }
  var x = testProcZero(false);
  writeln("testProcZero: when false int(8) int(8) then ", x.type:string);
}

{
  proc testProcCastMax(arg: bool) {
    if arg then return max(int(8)):int(8); else return max(int(8)):int(8);
  }
  var x = testProcCastMax(false);
  writeln("testProcCastMax: when false int(8) int(8) then ", x.type:string);
}

{
  proc testProcMax(arg: bool) {
    var i: int(8) = max(int(8));
    var u: int(8) = max(int(8));
    if arg then return i; else return u;
  }
  var x = testProcMax(false);
  writeln("testProcMax: when false int(8) int(8) then ", x.type:string);
}

{
  proc testParamProcCastZero(param arg: bool) param {
    if arg then return 0:int(8); else return 0:int(8);
  }
  param x = testParamProcCastZero(false);
  writeln("testParamProcCastZero: when false int(8) int(8) then ", x.type:string);
}

{
  proc testParamProcZero(param arg: bool) param {
    param i: int(8) = 0;
    param u: int(8) = 0;
    if arg then return i; else return u;
  }
  param x = testParamProcZero(false);
  writeln("testParamProcZero: when false int(8) int(8) then ", x.type:string);
}

{
  proc testParamProcCastMax(param arg: bool) param {
    if arg then return max(int(8)):int(8); else return max(int(8)):int(8);
  }
  param x = testParamProcCastMax(false);
  writeln("testParamProcCastMax: when false int(8) int(8) then ", x.type:string);
}

{
  proc testParamProcMax(param arg: bool) param {
    param i: int(8) = max(int(8));
    param u: int(8) = max(int(8));
    if arg then return i; else return u;
  }
  param x = testParamProcMax(false);
  writeln("testParamProcMax: when false int(8) int(8) then ", x.type:string);
}

{
  var b: bool = false;
  var x = if b then 0:int(8) else 0:int(8);
  writeln("testExpressionZero: when false int(8) int(8) then ", x.type:string);
}

{
  var b: bool = false;
  var x = if b then max(int(8)):int(8) else max(int(8)):int(8);
  writeln("testExpressionMax: when false int(8) int(8) then ", x.type:string);
}

{
  param x = if false then 0:int(8) else 0:int(8);
  writeln("testParamExpressionZero: when false int(8) int(8) then ", x.type:string);
}

{
  param x = if false then max(int(8)):int(8) else max(int(8)):int(8);
  writeln("testParamExpressionMax: when false int(8) int(8) then ", x.type:string);
}

{
  proc testProcCastZero(arg: bool) {
    if arg then return 0:int(8); else return 0:int(16);
  }
  var x = testProcCastZero(false);
  writeln("testProcCastZero: when false int(8) int(16) then ", x.type:string);
}

{
  proc testProcZero(arg: bool) {
    var i: int(8) = 0;
    var u: int(16) = 0;
    if arg then return i; else return u;
  }
  var x = testProcZero(false);
  writeln("testProcZero: when false int(8) int(16) then ", x.type:string);
}

{
  proc testProcCastMax(arg: bool) {
    if arg then return max(int(8)):int(8); else return max(int(16)):int(16);
  }
  var x = testProcCastMax(false);
  writeln("testProcCastMax: when false int(8) int(16) then ", x.type:string);
}

{
  proc testProcMax(arg: bool) {
    var i: int(8) = max(int(8));
    var u: int(16) = max(int(16));
    if arg then return i; else return u;
  }
  var x = testProcMax(false);
  writeln("testProcMax: when false int(8) int(16) then ", x.type:string);
}

{
  proc testParamProcCastZero(param arg: bool) param {
    if arg then return 0:int(8); else return 0:int(16);
  }
  param x = testParamProcCastZero(false);
  writeln("testParamProcCastZero: when false int(8) int(16) then ", x.type:string);
}

{
  proc testParamProcZero(param arg: bool) param {
    param i: int(8) = 0;
    param u: int(16) = 0;
    if arg then return i; else return u;
  }
  param x = testParamProcZero(false);
  writeln("testParamProcZero: when false int(8) int(16) then ", x.type:string);
}

{
  proc testParamProcCastMax(param arg: bool) param {
    if arg then return max(int(8)):int(8); else return max(int(16)):int(16);
  }
  param x = testParamProcCastMax(false);
  writeln("testParamProcCastMax: when false int(8) int(16) then ", x.type:string);
}

{
  proc testParamProcMax(param arg: bool) param {
    param i: int(8) = max(int(8));
    param u: int(16) = max(int(16));
    if arg then return i; else return u;
  }
  param x = testParamProcMax(false);
  writeln("testParamProcMax: when false int(8) int(16) then ", x.type:string);
}

{
  var b: bool = false;
  var x = if b then 0:int(8) else 0:int(16);
  writeln("testExpressionZero: when false int(8) int(16) then ", x.type:string);
}

{
  var b: bool = false;
  var x = if b then max(int(8)):int(8) else max(int(16)):int(16);
  writeln("testExpressionMax: when false int(8) int(16) then ", x.type:string);
}

{
  param x = if false then 0:int(8) else 0:int(16);
  writeln("testParamExpressionZero: when false int(8) int(16) then ", x.type:string);
}

{
  param x = if false then max(int(8)):int(8) else max(int(16)):int(16);
  writeln("testParamExpressionMax: when false int(8) int(16) then ", x.type:string);
}

{
  proc testProcCastZero(arg: bool) {
    if arg then return 0:int(8); else return 0:int(32);
  }
  var x = testProcCastZero(false);
  writeln("testProcCastZero: when false int(8) int(32) then ", x.type:string);
}

{
  proc testProcZero(arg: bool) {
    var i: int(8) = 0;
    var u: int(32) = 0;
    if arg then return i; else return u;
  }
  var x = testProcZero(false);
  writeln("testProcZero: when false int(8) int(32) then ", x.type:string);
}

{
  proc testProcCastMax(arg: bool) {
    if arg then return max(int(8)):int(8); else return max(int(32)):int(32);
  }
  var x = testProcCastMax(false);
  writeln("testProcCastMax: when false int(8) int(32) then ", x.type:string);
}

{
  proc testProcMax(arg: bool) {
    var i: int(8) = max(int(8));
    var u: int(32) = max(int(32));
    if arg then return i; else return u;
  }
  var x = testProcMax(false);
  writeln("testProcMax: when false int(8) int(32) then ", x.type:string);
}

{
  proc testParamProcCastZero(param arg: bool) param {
    if arg then return 0:int(8); else return 0:int(32);
  }
  param x = testParamProcCastZero(false);
  writeln("testParamProcCastZero: when false int(8) int(32) then ", x.type:string);
}

{
  proc testParamProcZero(param arg: bool) param {
    param i: int(8) = 0;
    param u: int(32) = 0;
    if arg then return i; else return u;
  }
  param x = testParamProcZero(false);
  writeln("testParamProcZero: when false int(8) int(32) then ", x.type:string);
}

{
  proc testParamProcCastMax(param arg: bool) param {
    if arg then return max(int(8)):int(8); else return max(int(32)):int(32);
  }
  param x = testParamProcCastMax(false);
  writeln("testParamProcCastMax: when false int(8) int(32) then ", x.type:string);
}

{
  proc testParamProcMax(param arg: bool) param {
    param i: int(8) = max(int(8));
    param u: int(32) = max(int(32));
    if arg then return i; else return u;
  }
  param x = testParamProcMax(false);
  writeln("testParamProcMax: when false int(8) int(32) then ", x.type:string);
}

{
  var b: bool = false;
  var x = if b then 0:int(8) else 0:int(32);
  writeln("testExpressionZero: when false int(8) int(32) then ", x.type:string);
}

{
  var b: bool = false;
  var x = if b then max(int(8)):int(8) else max(int(32)):int(32);
  writeln("testExpressionMax: when false int(8) int(32) then ", x.type:string);
}

{
  param x = if false then 0:int(8) else 0:int(32);
  writeln("testParamExpressionZero: when false int(8) int(32) then ", x.type:string);
}

{
  param x = if false then max(int(8)):int(8) else max(int(32)):int(32);
  writeln("testParamExpressionMax: when false int(8) int(32) then ", x.type:string);
}

{
  proc testProcCastZero(arg: bool) {
    if arg then return 0:int(8); else return 0:int(64);
  }
  var x = testProcCastZero(false);
  writeln("testProcCastZero: when false int(8) int(64) then ", x.type:string);
}

{
  proc testProcZero(arg: bool) {
    var i: int(8) = 0;
    var u: int(64) = 0;
    if arg then return i; else return u;
  }
  var x = testProcZero(false);
  writeln("testProcZero: when false int(8) int(64) then ", x.type:string);
}

{
  proc testProcCastMax(arg: bool) {
    if arg then return max(int(8)):int(8); else return max(int(64)):int(64);
  }
  var x = testProcCastMax(false);
  writeln("testProcCastMax: when false int(8) int(64) then ", x.type:string);
}

{
  proc testProcMax(arg: bool) {
    var i: int(8) = max(int(8));
    var u: int(64) = max(int(64));
    if arg then return i; else return u;
  }
  var x = testProcMax(false);
  writeln("testProcMax: when false int(8) int(64) then ", x.type:string);
}

{
  proc testParamProcCastZero(param arg: bool) param {
    if arg then return 0:int(8); else return 0:int(64);
  }
  param x = testParamProcCastZero(false);
  writeln("testParamProcCastZero: when false int(8) int(64) then ", x.type:string);
}

{
  proc testParamProcZero(param arg: bool) param {
    param i: int(8) = 0;
    param u: int(64) = 0;
    if arg then return i; else return u;
  }
  param x = testParamProcZero(false);
  writeln("testParamProcZero: when false int(8) int(64) then ", x.type:string);
}

{
  proc testParamProcCastMax(param arg: bool) param {
    if arg then return max(int(8)):int(8); else return max(int(64)):int(64);
  }
  param x = testParamProcCastMax(false);
  writeln("testParamProcCastMax: when false int(8) int(64) then ", x.type:string);
}

{
  proc testParamProcMax(param arg: bool) param {
    param i: int(8) = max(int(8));
    param u: int(64) = max(int(64));
    if arg then return i; else return u;
  }
  param x = testParamProcMax(false);
  writeln("testParamProcMax: when false int(8) int(64) then ", x.type:string);
}

{
  var b: bool = false;
  var x = if b then 0:int(8) else 0:int(64);
  writeln("testExpressionZero: when false int(8) int(64) then ", x.type:string);
}

{
  var b: bool = false;
  var x = if b then max(int(8)):int(8) else max(int(64)):int(64);
  writeln("testExpressionMax: when false int(8) int(64) then ", x.type:string);
}

{
  param x = if false then 0:int(8) else 0:int(64);
  writeln("testParamExpressionZero: when false int(8) int(64) then ", x.type:string);
}

{
  param x = if false then max(int(8)):int(8) else max(int(64)):int(64);
  writeln("testParamExpressionMax: when false int(8) int(64) then ", x.type:string);
}

{
  proc testProcCastZero(arg: bool) {
    if arg then return 0:int(8); else return 0:uint(8);
  }
  var x = testProcCastZero(false);
  writeln("testProcCastZero: when false int(8) uint(8) then ", x.type:string);
}

{
  proc testProcZero(arg: bool) {
    var i: int(8) = 0;
    var u: uint(8) = 0;
    if arg then return i; else return u;
  }
  var x = testProcZero(false);
  writeln("testProcZero: when false int(8) uint(8) then ", x.type:string);
}

{
  proc testProcCastMax(arg: bool) {
    if arg then return max(int(8)):int(8); else return max(uint(8)):uint(8);
  }
  var x = testProcCastMax(false);
  writeln("testProcCastMax: when false int(8) uint(8) then ", x.type:string);
}

{
  proc testProcMax(arg: bool) {
    var i: int(8) = max(int(8));
    var u: uint(8) = max(uint(8));
    if arg then return i; else return u;
  }
  var x = testProcMax(false);
  writeln("testProcMax: when false int(8) uint(8) then ", x.type:string);
}

{
  proc testParamProcCastZero(param arg: bool) param {
    if arg then return 0:int(8); else return 0:uint(8);
  }
  param x = testParamProcCastZero(false);
  writeln("testParamProcCastZero: when false int(8) uint(8) then ", x.type:string);
}

{
  proc testParamProcZero(param arg: bool) param {
    param i: int(8) = 0;
    param u: uint(8) = 0;
    if arg then return i; else return u;
  }
  param x = testParamProcZero(false);
  writeln("testParamProcZero: when false int(8) uint(8) then ", x.type:string);
}

{
  proc testParamProcCastMax(param arg: bool) param {
    if arg then return max(int(8)):int(8); else return max(uint(8)):uint(8);
  }
  param x = testParamProcCastMax(false);
  writeln("testParamProcCastMax: when false int(8) uint(8) then ", x.type:string);
}

{
  proc testParamProcMax(param arg: bool) param {
    param i: int(8) = max(int(8));
    param u: uint(8) = max(uint(8));
    if arg then return i; else return u;
  }
  param x = testParamProcMax(false);
  writeln("testParamProcMax: when false int(8) uint(8) then ", x.type:string);
}

{
  var b: bool = false;
  var x = if b then 0:int(8) else 0:uint(8);
  writeln("testExpressionZero: when false int(8) uint(8) then ", x.type:string);
}

{
  var b: bool = false;
  var x = if b then max(int(8)):int(8) else max(uint(8)):uint(8);
  writeln("testExpressionMax: when false int(8) uint(8) then ", x.type:string);
}

{
  param x = if false then 0:int(8) else 0:uint(8);
  writeln("testParamExpressionZero: when false int(8) uint(8) then ", x.type:string);
}

{
  param x = if false then max(int(8)):int(8) else max(uint(8)):uint(8);
  writeln("testParamExpressionMax: when false int(8) uint(8) then ", x.type:string);
}

{
  proc testProcCastZero(arg: bool) {
    if arg then return 0:int(8); else return 0:uint(16);
  }
  var x = testProcCastZero(false);
  writeln("testProcCastZero: when false int(8) uint(16) then ", x.type:string);
}

{
  proc testProcZero(arg: bool) {
    var i: int(8) = 0;
    var u: uint(16) = 0;
    if arg then return i; else return u;
  }
  var x = testProcZero(false);
  writeln("testProcZero: when false int(8) uint(16) then ", x.type:string);
}

{
  proc testProcCastMax(arg: bool) {
    if arg then return max(int(8)):int(8); else return max(uint(16)):uint(16);
  }
  var x = testProcCastMax(false);
  writeln("testProcCastMax: when false int(8) uint(16) then ", x.type:string);
}

{
  proc testProcMax(arg: bool) {
    var i: int(8) = max(int(8));
    var u: uint(16) = max(uint(16));
    if arg then return i; else return u;
  }
  var x = testProcMax(false);
  writeln("testProcMax: when false int(8) uint(16) then ", x.type:string);
}

{
  proc testParamProcCastZero(param arg: bool) param {
    if arg then return 0:int(8); else return 0:uint(16);
  }
  param x = testParamProcCastZero(false);
  writeln("testParamProcCastZero: when false int(8) uint(16) then ", x.type:string);
}

{
  proc testParamProcZero(param arg: bool) param {
    param i: int(8) = 0;
    param u: uint(16) = 0;
    if arg then return i; else return u;
  }
  param x = testParamProcZero(false);
  writeln("testParamProcZero: when false int(8) uint(16) then ", x.type:string);
}

{
  proc testParamProcCastMax(param arg: bool) param {
    if arg then return max(int(8)):int(8); else return max(uint(16)):uint(16);
  }
  param x = testParamProcCastMax(false);
  writeln("testParamProcCastMax: when false int(8) uint(16) then ", x.type:string);
}

{
  proc testParamProcMax(param arg: bool) param {
    param i: int(8) = max(int(8));
    param u: uint(16) = max(uint(16));
    if arg then return i; else return u;
  }
  param x = testParamProcMax(false);
  writeln("testParamProcMax: when false int(8) uint(16) then ", x.type:string);
}

{
  var b: bool = false;
  var x = if b then 0:int(8) else 0:uint(16);
  writeln("testExpressionZero: when false int(8) uint(16) then ", x.type:string);
}

{
  var b: bool = false;
  var x = if b then max(int(8)):int(8) else max(uint(16)):uint(16);
  writeln("testExpressionMax: when false int(8) uint(16) then ", x.type:string);
}

{
  param x = if false then 0:int(8) else 0:uint(16);
  writeln("testParamExpressionZero: when false int(8) uint(16) then ", x.type:string);
}

{
  param x = if false then max(int(8)):int(8) else max(uint(16)):uint(16);
  writeln("testParamExpressionMax: when false int(8) uint(16) then ", x.type:string);
}

{
  proc testProcCastZero(arg: bool) {
    if arg then return 0:int(8); else return 0:uint(32);
  }
  var x = testProcCastZero(false);
  writeln("testProcCastZero: when false int(8) uint(32) then ", x.type:string);
}

{
  proc testProcZero(arg: bool) {
    var i: int(8) = 0;
    var u: uint(32) = 0;
    if arg then return i; else return u;
  }
  var x = testProcZero(false);
  writeln("testProcZero: when false int(8) uint(32) then ", x.type:string);
}

{
  proc testProcCastMax(arg: bool) {
    if arg then return max(int(8)):int(8); else return max(uint(32)):uint(32);
  }
  var x = testProcCastMax(false);
  writeln("testProcCastMax: when false int(8) uint(32) then ", x.type:string);
}

{
  proc testProcMax(arg: bool) {
    var i: int(8) = max(int(8));
    var u: uint(32) = max(uint(32));
    if arg then return i; else return u;
  }
  var x = testProcMax(false);
  writeln("testProcMax: when false int(8) uint(32) then ", x.type:string);
}

{
  proc testParamProcCastZero(param arg: bool) param {
    if arg then return 0:int(8); else return 0:uint(32);
  }
  param x = testParamProcCastZero(false);
  writeln("testParamProcCastZero: when false int(8) uint(32) then ", x.type:string);
}

{
  proc testParamProcZero(param arg: bool) param {
    param i: int(8) = 0;
    param u: uint(32) = 0;
    if arg then return i; else return u;
  }
  param x = testParamProcZero(false);
  writeln("testParamProcZero: when false int(8) uint(32) then ", x.type:string);
}

{
  proc testParamProcCastMax(param arg: bool) param {
    if arg then return max(int(8)):int(8); else return max(uint(32)):uint(32);
  }
  param x = testParamProcCastMax(false);
  writeln("testParamProcCastMax: when false int(8) uint(32) then ", x.type:string);
}

{
  proc testParamProcMax(param arg: bool) param {
    param i: int(8) = max(int(8));
    param u: uint(32) = max(uint(32));
    if arg then return i; else return u;
  }
  param x = testParamProcMax(false);
  writeln("testParamProcMax: when false int(8) uint(32) then ", x.type:string);
}

{
  var b: bool = false;
  var x = if b then 0:int(8) else 0:uint(32);
  writeln("testExpressionZero: when false int(8) uint(32) then ", x.type:string);
}

{
  var b: bool = false;
  var x = if b then max(int(8)):int(8) else max(uint(32)):uint(32);
  writeln("testExpressionMax: when false int(8) uint(32) then ", x.type:string);
}

{
  param x = if false then 0:int(8) else 0:uint(32);
  writeln("testParamExpressionZero: when false int(8) uint(32) then ", x.type:string);
}

{
  param x = if false then max(int(8)):int(8) else max(uint(32)):uint(32);
  writeln("testParamExpressionMax: when false int(8) uint(32) then ", x.type:string);
}

{
  proc testProcCastZero(arg: bool) {
    if arg then return 0:int(8); else return 0:uint(64);
  }
  var x = testProcCastZero(false);
  writeln("testProcCastZero: when false int(8) uint(64) then ", x.type:string);
}

{
  proc testProcZero(arg: bool) {
    var i: int(8) = 0;
    var u: uint(64) = 0;
    if arg then return i; else return u;
  }
  var x = testProcZero(false);
  writeln("testProcZero: when false int(8) uint(64) then ", x.type:string);
}

{
  proc testProcCastMax(arg: bool) {
    if arg then return max(int(8)):int(8); else return max(uint(64)):uint(64);
  }
  var x = testProcCastMax(false);
  writeln("testProcCastMax: when false int(8) uint(64) then ", x.type:string);
}

{
  proc testProcMax(arg: bool) {
    var i: int(8) = max(int(8));
    var u: uint(64) = max(uint(64));
    if arg then return i; else return u;
  }
  var x = testProcMax(false);
  writeln("testProcMax: when false int(8) uint(64) then ", x.type:string);
}

{
  proc testParamProcCastZero(param arg: bool) param {
    if arg then return 0:int(8); else return 0:uint(64);
  }
  param x = testParamProcCastZero(false);
  writeln("testParamProcCastZero: when false int(8) uint(64) then ", x.type:string);
}

{
  proc testParamProcZero(param arg: bool) param {
    param i: int(8) = 0;
    param u: uint(64) = 0;
    if arg then return i; else return u;
  }
  param x = testParamProcZero(false);
  writeln("testParamProcZero: when false int(8) uint(64) then ", x.type:string);
}

{
  proc testParamProcCastMax(param arg: bool) param {
    if arg then return max(int(8)):int(8); else return max(uint(64)):uint(64);
  }
  param x = testParamProcCastMax(false);
  writeln("testParamProcCastMax: when false int(8) uint(64) then ", x.type:string);
}

{
  proc testParamProcMax(param arg: bool) param {
    param i: int(8) = max(int(8));
    param u: uint(64) = max(uint(64));
    if arg then return i; else return u;
  }
  param x = testParamProcMax(false);
  writeln("testParamProcMax: when false int(8) uint(64) then ", x.type:string);
}

{
  var b: bool = false;
  var x = if b then 0:int(8) else 0:uint(64);
  writeln("testExpressionZero: when false int(8) uint(64) then ", x.type:string);
}

{
  var b: bool = false;
  var x = if b then max(int(8)):int(8) else max(uint(64)):uint(64);
  writeln("testExpressionMax: when false int(8) uint(64) then ", x.type:string);
}

{
  param x = if false then 0:int(8) else 0:uint(64);
  writeln("testParamExpressionZero: when false int(8) uint(64) then ", x.type:string);
}

{
  param x = if false then max(int(8)):int(8) else max(uint(64)):uint(64);
  writeln("testParamExpressionMax: when false int(8) uint(64) then ", x.type:string);
}

{
  proc testProcCastZero(arg: bool) {
    if arg then return 0:int(16); else return 0:int(8);
  }
  var x = testProcCastZero(false);
  writeln("testProcCastZero: when false int(16) int(8) then ", x.type:string);
}

{
  proc testProcZero(arg: bool) {
    var i: int(16) = 0;
    var u: int(8) = 0;
    if arg then return i; else return u;
  }
  var x = testProcZero(false);
  writeln("testProcZero: when false int(16) int(8) then ", x.type:string);
}

{
  proc testProcCastMax(arg: bool) {
    if arg then return max(int(16)):int(16); else return max(int(8)):int(8);
  }
  var x = testProcCastMax(false);
  writeln("testProcCastMax: when false int(16) int(8) then ", x.type:string);
}

{
  proc testProcMax(arg: bool) {
    var i: int(16) = max(int(16));
    var u: int(8) = max(int(8));
    if arg then return i; else return u;
  }
  var x = testProcMax(false);
  writeln("testProcMax: when false int(16) int(8) then ", x.type:string);
}

{
  proc testParamProcCastZero(param arg: bool) param {
    if arg then return 0:int(16); else return 0:int(8);
  }
  param x = testParamProcCastZero(false);
  writeln("testParamProcCastZero: when false int(16) int(8) then ", x.type:string);
}

{
  proc testParamProcZero(param arg: bool) param {
    param i: int(16) = 0;
    param u: int(8) = 0;
    if arg then return i; else return u;
  }
  param x = testParamProcZero(false);
  writeln("testParamProcZero: when false int(16) int(8) then ", x.type:string);
}

{
  proc testParamProcCastMax(param arg: bool) param {
    if arg then return max(int(16)):int(16); else return max(int(8)):int(8);
  }
  param x = testParamProcCastMax(false);
  writeln("testParamProcCastMax: when false int(16) int(8) then ", x.type:string);
}

{
  proc testParamProcMax(param arg: bool) param {
    param i: int(16) = max(int(16));
    param u: int(8) = max(int(8));
    if arg then return i; else return u;
  }
  param x = testParamProcMax(false);
  writeln("testParamProcMax: when false int(16) int(8) then ", x.type:string);
}

{
  var b: bool = false;
  var x = if b then 0:int(16) else 0:int(8);
  writeln("testExpressionZero: when false int(16) int(8) then ", x.type:string);
}

{
  var b: bool = false;
  var x = if b then max(int(16)):int(16) else max(int(8)):int(8);
  writeln("testExpressionMax: when false int(16) int(8) then ", x.type:string);
}

{
  param x = if false then 0:int(16) else 0:int(8);
  writeln("testParamExpressionZero: when false int(16) int(8) then ", x.type:string);
}

{
  param x = if false then max(int(16)):int(16) else max(int(8)):int(8);
  writeln("testParamExpressionMax: when false int(16) int(8) then ", x.type:string);
}

{
  proc testProcCastZero(arg: bool) {
    if arg then return 0:int(16); else return 0:int(16);
  }
  var x = testProcCastZero(false);
  writeln("testProcCastZero: when false int(16) int(16) then ", x.type:string);
}

{
  proc testProcZero(arg: bool) {
    var i: int(16) = 0;
    var u: int(16) = 0;
    if arg then return i; else return u;
  }
  var x = testProcZero(false);
  writeln("testProcZero: when false int(16) int(16) then ", x.type:string);
}

{
  proc testProcCastMax(arg: bool) {
    if arg then return max(int(16)):int(16); else return max(int(16)):int(16);
  }
  var x = testProcCastMax(false);
  writeln("testProcCastMax: when false int(16) int(16) then ", x.type:string);
}

{
  proc testProcMax(arg: bool) {
    var i: int(16) = max(int(16));
    var u: int(16) = max(int(16));
    if arg then return i; else return u;
  }
  var x = testProcMax(false);
  writeln("testProcMax: when false int(16) int(16) then ", x.type:string);
}

{
  proc testParamProcCastZero(param arg: bool) param {
    if arg then return 0:int(16); else return 0:int(16);
  }
  param x = testParamProcCastZero(false);
  writeln("testParamProcCastZero: when false int(16) int(16) then ", x.type:string);
}

{
  proc testParamProcZero(param arg: bool) param {
    param i: int(16) = 0;
    param u: int(16) = 0;
    if arg then return i; else return u;
  }
  param x = testParamProcZero(false);
  writeln("testParamProcZero: when false int(16) int(16) then ", x.type:string);
}

{
  proc testParamProcCastMax(param arg: bool) param {
    if arg then return max(int(16)):int(16); else return max(int(16)):int(16);
  }
  param x = testParamProcCastMax(false);
  writeln("testParamProcCastMax: when false int(16) int(16) then ", x.type:string);
}

{
  proc testParamProcMax(param arg: bool) param {
    param i: int(16) = max(int(16));
    param u: int(16) = max(int(16));
    if arg then return i; else return u;
  }
  param x = testParamProcMax(false);
  writeln("testParamProcMax: when false int(16) int(16) then ", x.type:string);
}

{
  var b: bool = false;
  var x = if b then 0:int(16) else 0:int(16);
  writeln("testExpressionZero: when false int(16) int(16) then ", x.type:string);
}

{
  var b: bool = false;
  var x = if b then max(int(16)):int(16) else max(int(16)):int(16);
  writeln("testExpressionMax: when false int(16) int(16) then ", x.type:string);
}

{
  param x = if false then 0:int(16) else 0:int(16);
  writeln("testParamExpressionZero: when false int(16) int(16) then ", x.type:string);
}

{
  param x = if false then max(int(16)):int(16) else max(int(16)):int(16);
  writeln("testParamExpressionMax: when false int(16) int(16) then ", x.type:string);
}

{
  proc testProcCastZero(arg: bool) {
    if arg then return 0:int(16); else return 0:int(32);
  }
  var x = testProcCastZero(false);
  writeln("testProcCastZero: when false int(16) int(32) then ", x.type:string);
}

{
  proc testProcZero(arg: bool) {
    var i: int(16) = 0;
    var u: int(32) = 0;
    if arg then return i; else return u;
  }
  var x = testProcZero(false);
  writeln("testProcZero: when false int(16) int(32) then ", x.type:string);
}

{
  proc testProcCastMax(arg: bool) {
    if arg then return max(int(16)):int(16); else return max(int(32)):int(32);
  }
  var x = testProcCastMax(false);
  writeln("testProcCastMax: when false int(16) int(32) then ", x.type:string);
}

{
  proc testProcMax(arg: bool) {
    var i: int(16) = max(int(16));
    var u: int(32) = max(int(32));
    if arg then return i; else return u;
  }
  var x = testProcMax(false);
  writeln("testProcMax: when false int(16) int(32) then ", x.type:string);
}

{
  proc testParamProcCastZero(param arg: bool) param {
    if arg then return 0:int(16); else return 0:int(32);
  }
  param x = testParamProcCastZero(false);
  writeln("testParamProcCastZero: when false int(16) int(32) then ", x.type:string);
}

{
  proc testParamProcZero(param arg: bool) param {
    param i: int(16) = 0;
    param u: int(32) = 0;
    if arg then return i; else return u;
  }
  param x = testParamProcZero(false);
  writeln("testParamProcZero: when false int(16) int(32) then ", x.type:string);
}

{
  proc testParamProcCastMax(param arg: bool) param {
    if arg then return max(int(16)):int(16); else return max(int(32)):int(32);
  }
  param x = testParamProcCastMax(false);
  writeln("testParamProcCastMax: when false int(16) int(32) then ", x.type:string);
}

{
  proc testParamProcMax(param arg: bool) param {
    param i: int(16) = max(int(16));
    param u: int(32) = max(int(32));
    if arg then return i; else return u;
  }
  param x = testParamProcMax(false);
  writeln("testParamProcMax: when false int(16) int(32) then ", x.type:string);
}

{
  var b: bool = false;
  var x = if b then 0:int(16) else 0:int(32);
  writeln("testExpressionZero: when false int(16) int(32) then ", x.type:string);
}

{
  var b: bool = false;
  var x = if b then max(int(16)):int(16) else max(int(32)):int(32);
  writeln("testExpressionMax: when false int(16) int(32) then ", x.type:string);
}

{
  param x = if false then 0:int(16) else 0:int(32);
  writeln("testParamExpressionZero: when false int(16) int(32) then ", x.type:string);
}

{
  param x = if false then max(int(16)):int(16) else max(int(32)):int(32);
  writeln("testParamExpressionMax: when false int(16) int(32) then ", x.type:string);
}

{
  proc testProcCastZero(arg: bool) {
    if arg then return 0:int(16); else return 0:int(64);
  }
  var x = testProcCastZero(false);
  writeln("testProcCastZero: when false int(16) int(64) then ", x.type:string);
}

{
  proc testProcZero(arg: bool) {
    var i: int(16) = 0;
    var u: int(64) = 0;
    if arg then return i; else return u;
  }
  var x = testProcZero(false);
  writeln("testProcZero: when false int(16) int(64) then ", x.type:string);
}

{
  proc testProcCastMax(arg: bool) {
    if arg then return max(int(16)):int(16); else return max(int(64)):int(64);
  }
  var x = testProcCastMax(false);
  writeln("testProcCastMax: when false int(16) int(64) then ", x.type:string);
}

{
  proc testProcMax(arg: bool) {
    var i: int(16) = max(int(16));
    var u: int(64) = max(int(64));
    if arg then return i; else return u;
  }
  var x = testProcMax(false);
  writeln("testProcMax: when false int(16) int(64) then ", x.type:string);
}

{
  proc testParamProcCastZero(param arg: bool) param {
    if arg then return 0:int(16); else return 0:int(64);
  }
  param x = testParamProcCastZero(false);
  writeln("testParamProcCastZero: when false int(16) int(64) then ", x.type:string);
}

{
  proc testParamProcZero(param arg: bool) param {
    param i: int(16) = 0;
    param u: int(64) = 0;
    if arg then return i; else return u;
  }
  param x = testParamProcZero(false);
  writeln("testParamProcZero: when false int(16) int(64) then ", x.type:string);
}

{
  proc testParamProcCastMax(param arg: bool) param {
    if arg then return max(int(16)):int(16); else return max(int(64)):int(64);
  }
  param x = testParamProcCastMax(false);
  writeln("testParamProcCastMax: when false int(16) int(64) then ", x.type:string);
}

{
  proc testParamProcMax(param arg: bool) param {
    param i: int(16) = max(int(16));
    param u: int(64) = max(int(64));
    if arg then return i; else return u;
  }
  param x = testParamProcMax(false);
  writeln("testParamProcMax: when false int(16) int(64) then ", x.type:string);
}

{
  var b: bool = false;
  var x = if b then 0:int(16) else 0:int(64);
  writeln("testExpressionZero: when false int(16) int(64) then ", x.type:string);
}

{
  var b: bool = false;
  var x = if b then max(int(16)):int(16) else max(int(64)):int(64);
  writeln("testExpressionMax: when false int(16) int(64) then ", x.type:string);
}

{
  param x = if false then 0:int(16) else 0:int(64);
  writeln("testParamExpressionZero: when false int(16) int(64) then ", x.type:string);
}

{
  param x = if false then max(int(16)):int(16) else max(int(64)):int(64);
  writeln("testParamExpressionMax: when false int(16) int(64) then ", x.type:string);
}

{
  proc testProcCastZero(arg: bool) {
    if arg then return 0:int(16); else return 0:uint(8);
  }
  var x = testProcCastZero(false);
  writeln("testProcCastZero: when false int(16) uint(8) then ", x.type:string);
}

{
  proc testProcZero(arg: bool) {
    var i: int(16) = 0;
    var u: uint(8) = 0;
    if arg then return i; else return u;
  }
  var x = testProcZero(false);
  writeln("testProcZero: when false int(16) uint(8) then ", x.type:string);
}

{
  proc testProcCastMax(arg: bool) {
    if arg then return max(int(16)):int(16); else return max(uint(8)):uint(8);
  }
  var x = testProcCastMax(false);
  writeln("testProcCastMax: when false int(16) uint(8) then ", x.type:string);
}

{
  proc testProcMax(arg: bool) {
    var i: int(16) = max(int(16));
    var u: uint(8) = max(uint(8));
    if arg then return i; else return u;
  }
  var x = testProcMax(false);
  writeln("testProcMax: when false int(16) uint(8) then ", x.type:string);
}

{
  proc testParamProcCastZero(param arg: bool) param {
    if arg then return 0:int(16); else return 0:uint(8);
  }
  param x = testParamProcCastZero(false);
  writeln("testParamProcCastZero: when false int(16) uint(8) then ", x.type:string);
}

{
  proc testParamProcZero(param arg: bool) param {
    param i: int(16) = 0;
    param u: uint(8) = 0;
    if arg then return i; else return u;
  }
  param x = testParamProcZero(false);
  writeln("testParamProcZero: when false int(16) uint(8) then ", x.type:string);
}

{
  proc testParamProcCastMax(param arg: bool) param {
    if arg then return max(int(16)):int(16); else return max(uint(8)):uint(8);
  }
  param x = testParamProcCastMax(false);
  writeln("testParamProcCastMax: when false int(16) uint(8) then ", x.type:string);
}

{
  proc testParamProcMax(param arg: bool) param {
    param i: int(16) = max(int(16));
    param u: uint(8) = max(uint(8));
    if arg then return i; else return u;
  }
  param x = testParamProcMax(false);
  writeln("testParamProcMax: when false int(16) uint(8) then ", x.type:string);
}

{
  var b: bool = false;
  var x = if b then 0:int(16) else 0:uint(8);
  writeln("testExpressionZero: when false int(16) uint(8) then ", x.type:string);
}

{
  var b: bool = false;
  var x = if b then max(int(16)):int(16) else max(uint(8)):uint(8);
  writeln("testExpressionMax: when false int(16) uint(8) then ", x.type:string);
}

{
  param x = if false then 0:int(16) else 0:uint(8);
  writeln("testParamExpressionZero: when false int(16) uint(8) then ", x.type:string);
}

{
  param x = if false then max(int(16)):int(16) else max(uint(8)):uint(8);
  writeln("testParamExpressionMax: when false int(16) uint(8) then ", x.type:string);
}

{
  proc testProcCastZero(arg: bool) {
    if arg then return 0:int(16); else return 0:uint(16);
  }
  var x = testProcCastZero(false);
  writeln("testProcCastZero: when false int(16) uint(16) then ", x.type:string);
}

{
  proc testProcZero(arg: bool) {
    var i: int(16) = 0;
    var u: uint(16) = 0;
    if arg then return i; else return u;
  }
  var x = testProcZero(false);
  writeln("testProcZero: when false int(16) uint(16) then ", x.type:string);
}

{
  proc testProcCastMax(arg: bool) {
    if arg then return max(int(16)):int(16); else return max(uint(16)):uint(16);
  }
  var x = testProcCastMax(false);
  writeln("testProcCastMax: when false int(16) uint(16) then ", x.type:string);
}

{
  proc testProcMax(arg: bool) {
    var i: int(16) = max(int(16));
    var u: uint(16) = max(uint(16));
    if arg then return i; else return u;
  }
  var x = testProcMax(false);
  writeln("testProcMax: when false int(16) uint(16) then ", x.type:string);
}

{
  proc testParamProcCastZero(param arg: bool) param {
    if arg then return 0:int(16); else return 0:uint(16);
  }
  param x = testParamProcCastZero(false);
  writeln("testParamProcCastZero: when false int(16) uint(16) then ", x.type:string);
}

{
  proc testParamProcZero(param arg: bool) param {
    param i: int(16) = 0;
    param u: uint(16) = 0;
    if arg then return i; else return u;
  }
  param x = testParamProcZero(false);
  writeln("testParamProcZero: when false int(16) uint(16) then ", x.type:string);
}

{
  proc testParamProcCastMax(param arg: bool) param {
    if arg then return max(int(16)):int(16); else return max(uint(16)):uint(16);
  }
  param x = testParamProcCastMax(false);
  writeln("testParamProcCastMax: when false int(16) uint(16) then ", x.type:string);
}

{
  proc testParamProcMax(param arg: bool) param {
    param i: int(16) = max(int(16));
    param u: uint(16) = max(uint(16));
    if arg then return i; else return u;
  }
  param x = testParamProcMax(false);
  writeln("testParamProcMax: when false int(16) uint(16) then ", x.type:string);
}

{
  var b: bool = false;
  var x = if b then 0:int(16) else 0:uint(16);
  writeln("testExpressionZero: when false int(16) uint(16) then ", x.type:string);
}

{
  var b: bool = false;
  var x = if b then max(int(16)):int(16) else max(uint(16)):uint(16);
  writeln("testExpressionMax: when false int(16) uint(16) then ", x.type:string);
}

{
  param x = if false then 0:int(16) else 0:uint(16);
  writeln("testParamExpressionZero: when false int(16) uint(16) then ", x.type:string);
}

{
  param x = if false then max(int(16)):int(16) else max(uint(16)):uint(16);
  writeln("testParamExpressionMax: when false int(16) uint(16) then ", x.type:string);
}

{
  proc testProcCastZero(arg: bool) {
    if arg then return 0:int(16); else return 0:uint(32);
  }
  var x = testProcCastZero(false);
  writeln("testProcCastZero: when false int(16) uint(32) then ", x.type:string);
}

{
  proc testProcZero(arg: bool) {
    var i: int(16) = 0;
    var u: uint(32) = 0;
    if arg then return i; else return u;
  }
  var x = testProcZero(false);
  writeln("testProcZero: when false int(16) uint(32) then ", x.type:string);
}

{
  proc testProcCastMax(arg: bool) {
    if arg then return max(int(16)):int(16); else return max(uint(32)):uint(32);
  }
  var x = testProcCastMax(false);
  writeln("testProcCastMax: when false int(16) uint(32) then ", x.type:string);
}

{
  proc testProcMax(arg: bool) {
    var i: int(16) = max(int(16));
    var u: uint(32) = max(uint(32));
    if arg then return i; else return u;
  }
  var x = testProcMax(false);
  writeln("testProcMax: when false int(16) uint(32) then ", x.type:string);
}

{
  proc testParamProcCastZero(param arg: bool) param {
    if arg then return 0:int(16); else return 0:uint(32);
  }
  param x = testParamProcCastZero(false);
  writeln("testParamProcCastZero: when false int(16) uint(32) then ", x.type:string);
}

{
  proc testParamProcZero(param arg: bool) param {
    param i: int(16) = 0;
    param u: uint(32) = 0;
    if arg then return i; else return u;
  }
  param x = testParamProcZero(false);
  writeln("testParamProcZero: when false int(16) uint(32) then ", x.type:string);
}

{
  proc testParamProcCastMax(param arg: bool) param {
    if arg then return max(int(16)):int(16); else return max(uint(32)):uint(32);
  }
  param x = testParamProcCastMax(false);
  writeln("testParamProcCastMax: when false int(16) uint(32) then ", x.type:string);
}

{
  proc testParamProcMax(param arg: bool) param {
    param i: int(16) = max(int(16));
    param u: uint(32) = max(uint(32));
    if arg then return i; else return u;
  }
  param x = testParamProcMax(false);
  writeln("testParamProcMax: when false int(16) uint(32) then ", x.type:string);
}

{
  var b: bool = false;
  var x = if b then 0:int(16) else 0:uint(32);
  writeln("testExpressionZero: when false int(16) uint(32) then ", x.type:string);
}

{
  var b: bool = false;
  var x = if b then max(int(16)):int(16) else max(uint(32)):uint(32);
  writeln("testExpressionMax: when false int(16) uint(32) then ", x.type:string);
}

{
  param x = if false then 0:int(16) else 0:uint(32);
  writeln("testParamExpressionZero: when false int(16) uint(32) then ", x.type:string);
}

{
  param x = if false then max(int(16)):int(16) else max(uint(32)):uint(32);
  writeln("testParamExpressionMax: when false int(16) uint(32) then ", x.type:string);
}

{
  proc testProcCastZero(arg: bool) {
    if arg then return 0:int(16); else return 0:uint(64);
  }
  var x = testProcCastZero(false);
  writeln("testProcCastZero: when false int(16) uint(64) then ", x.type:string);
}

{
  proc testProcZero(arg: bool) {
    var i: int(16) = 0;
    var u: uint(64) = 0;
    if arg then return i; else return u;
  }
  var x = testProcZero(false);
  writeln("testProcZero: when false int(16) uint(64) then ", x.type:string);
}

{
  proc testProcCastMax(arg: bool) {
    if arg then return max(int(16)):int(16); else return max(uint(64)):uint(64);
  }
  var x = testProcCastMax(false);
  writeln("testProcCastMax: when false int(16) uint(64) then ", x.type:string);
}

{
  proc testProcMax(arg: bool) {
    var i: int(16) = max(int(16));
    var u: uint(64) = max(uint(64));
    if arg then return i; else return u;
  }
  var x = testProcMax(false);
  writeln("testProcMax: when false int(16) uint(64) then ", x.type:string);
}

{
  proc testParamProcCastZero(param arg: bool) param {
    if arg then return 0:int(16); else return 0:uint(64);
  }
  param x = testParamProcCastZero(false);
  writeln("testParamProcCastZero: when false int(16) uint(64) then ", x.type:string);
}

{
  proc testParamProcZero(param arg: bool) param {
    param i: int(16) = 0;
    param u: uint(64) = 0;
    if arg then return i; else return u;
  }
  param x = testParamProcZero(false);
  writeln("testParamProcZero: when false int(16) uint(64) then ", x.type:string);
}

{
  proc testParamProcCastMax(param arg: bool) param {
    if arg then return max(int(16)):int(16); else return max(uint(64)):uint(64);
  }
  param x = testParamProcCastMax(false);
  writeln("testParamProcCastMax: when false int(16) uint(64) then ", x.type:string);
}

{
  proc testParamProcMax(param arg: bool) param {
    param i: int(16) = max(int(16));
    param u: uint(64) = max(uint(64));
    if arg then return i; else return u;
  }
  param x = testParamProcMax(false);
  writeln("testParamProcMax: when false int(16) uint(64) then ", x.type:string);
}

{
  var b: bool = false;
  var x = if b then 0:int(16) else 0:uint(64);
  writeln("testExpressionZero: when false int(16) uint(64) then ", x.type:string);
}

{
  var b: bool = false;
  var x = if b then max(int(16)):int(16) else max(uint(64)):uint(64);
  writeln("testExpressionMax: when false int(16) uint(64) then ", x.type:string);
}

{
  param x = if false then 0:int(16) else 0:uint(64);
  writeln("testParamExpressionZero: when false int(16) uint(64) then ", x.type:string);
}

{
  param x = if false then max(int(16)):int(16) else max(uint(64)):uint(64);
  writeln("testParamExpressionMax: when false int(16) uint(64) then ", x.type:string);
}

{
  proc testProcCastZero(arg: bool) {
    if arg then return 0:int(32); else return 0:int(8);
  }
  var x = testProcCastZero(false);
  writeln("testProcCastZero: when false int(32) int(8) then ", x.type:string);
}

{
  proc testProcZero(arg: bool) {
    var i: int(32) = 0;
    var u: int(8) = 0;
    if arg then return i; else return u;
  }
  var x = testProcZero(false);
  writeln("testProcZero: when false int(32) int(8) then ", x.type:string);
}

{
  proc testProcCastMax(arg: bool) {
    if arg then return max(int(32)):int(32); else return max(int(8)):int(8);
  }
  var x = testProcCastMax(false);
  writeln("testProcCastMax: when false int(32) int(8) then ", x.type:string);
}

{
  proc testProcMax(arg: bool) {
    var i: int(32) = max(int(32));
    var u: int(8) = max(int(8));
    if arg then return i; else return u;
  }
  var x = testProcMax(false);
  writeln("testProcMax: when false int(32) int(8) then ", x.type:string);
}

{
  proc testParamProcCastZero(param arg: bool) param {
    if arg then return 0:int(32); else return 0:int(8);
  }
  param x = testParamProcCastZero(false);
  writeln("testParamProcCastZero: when false int(32) int(8) then ", x.type:string);
}

{
  proc testParamProcZero(param arg: bool) param {
    param i: int(32) = 0;
    param u: int(8) = 0;
    if arg then return i; else return u;
  }
  param x = testParamProcZero(false);
  writeln("testParamProcZero: when false int(32) int(8) then ", x.type:string);
}

{
  proc testParamProcCastMax(param arg: bool) param {
    if arg then return max(int(32)):int(32); else return max(int(8)):int(8);
  }
  param x = testParamProcCastMax(false);
  writeln("testParamProcCastMax: when false int(32) int(8) then ", x.type:string);
}

{
  proc testParamProcMax(param arg: bool) param {
    param i: int(32) = max(int(32));
    param u: int(8) = max(int(8));
    if arg then return i; else return u;
  }
  param x = testParamProcMax(false);
  writeln("testParamProcMax: when false int(32) int(8) then ", x.type:string);
}

{
  var b: bool = false;
  var x = if b then 0:int(32) else 0:int(8);
  writeln("testExpressionZero: when false int(32) int(8) then ", x.type:string);
}

{
  var b: bool = false;
  var x = if b then max(int(32)):int(32) else max(int(8)):int(8);
  writeln("testExpressionMax: when false int(32) int(8) then ", x.type:string);
}

{
  param x = if false then 0:int(32) else 0:int(8);
  writeln("testParamExpressionZero: when false int(32) int(8) then ", x.type:string);
}

{
  param x = if false then max(int(32)):int(32) else max(int(8)):int(8);
  writeln("testParamExpressionMax: when false int(32) int(8) then ", x.type:string);
}

{
  proc testProcCastZero(arg: bool) {
    if arg then return 0:int(32); else return 0:int(16);
  }
  var x = testProcCastZero(false);
  writeln("testProcCastZero: when false int(32) int(16) then ", x.type:string);
}

{
  proc testProcZero(arg: bool) {
    var i: int(32) = 0;
    var u: int(16) = 0;
    if arg then return i; else return u;
  }
  var x = testProcZero(false);
  writeln("testProcZero: when false int(32) int(16) then ", x.type:string);
}

{
  proc testProcCastMax(arg: bool) {
    if arg then return max(int(32)):int(32); else return max(int(16)):int(16);
  }
  var x = testProcCastMax(false);
  writeln("testProcCastMax: when false int(32) int(16) then ", x.type:string);
}

{
  proc testProcMax(arg: bool) {
    var i: int(32) = max(int(32));
    var u: int(16) = max(int(16));
    if arg then return i; else return u;
  }
  var x = testProcMax(false);
  writeln("testProcMax: when false int(32) int(16) then ", x.type:string);
}

{
  proc testParamProcCastZero(param arg: bool) param {
    if arg then return 0:int(32); else return 0:int(16);
  }
  param x = testParamProcCastZero(false);
  writeln("testParamProcCastZero: when false int(32) int(16) then ", x.type:string);
}

{
  proc testParamProcZero(param arg: bool) param {
    param i: int(32) = 0;
    param u: int(16) = 0;
    if arg then return i; else return u;
  }
  param x = testParamProcZero(false);
  writeln("testParamProcZero: when false int(32) int(16) then ", x.type:string);
}

{
  proc testParamProcCastMax(param arg: bool) param {
    if arg then return max(int(32)):int(32); else return max(int(16)):int(16);
  }
  param x = testParamProcCastMax(false);
  writeln("testParamProcCastMax: when false int(32) int(16) then ", x.type:string);
}

{
  proc testParamProcMax(param arg: bool) param {
    param i: int(32) = max(int(32));
    param u: int(16) = max(int(16));
    if arg then return i; else return u;
  }
  param x = testParamProcMax(false);
  writeln("testParamProcMax: when false int(32) int(16) then ", x.type:string);
}

{
  var b: bool = false;
  var x = if b then 0:int(32) else 0:int(16);
  writeln("testExpressionZero: when false int(32) int(16) then ", x.type:string);
}

{
  var b: bool = false;
  var x = if b then max(int(32)):int(32) else max(int(16)):int(16);
  writeln("testExpressionMax: when false int(32) int(16) then ", x.type:string);
}

{
  param x = if false then 0:int(32) else 0:int(16);
  writeln("testParamExpressionZero: when false int(32) int(16) then ", x.type:string);
}

{
  param x = if false then max(int(32)):int(32) else max(int(16)):int(16);
  writeln("testParamExpressionMax: when false int(32) int(16) then ", x.type:string);
}

{
  proc testProcCastZero(arg: bool) {
    if arg then return 0:int(32); else return 0:int(32);
  }
  var x = testProcCastZero(false);
  writeln("testProcCastZero: when false int(32) int(32) then ", x.type:string);
}

{
  proc testProcZero(arg: bool) {
    var i: int(32) = 0;
    var u: int(32) = 0;
    if arg then return i; else return u;
  }
  var x = testProcZero(false);
  writeln("testProcZero: when false int(32) int(32) then ", x.type:string);
}

{
  proc testProcCastMax(arg: bool) {
    if arg then return max(int(32)):int(32); else return max(int(32)):int(32);
  }
  var x = testProcCastMax(false);
  writeln("testProcCastMax: when false int(32) int(32) then ", x.type:string);
}

{
  proc testProcMax(arg: bool) {
    var i: int(32) = max(int(32));
    var u: int(32) = max(int(32));
    if arg then return i; else return u;
  }
  var x = testProcMax(false);
  writeln("testProcMax: when false int(32) int(32) then ", x.type:string);
}

{
  proc testParamProcCastZero(param arg: bool) param {
    if arg then return 0:int(32); else return 0:int(32);
  }
  param x = testParamProcCastZero(false);
  writeln("testParamProcCastZero: when false int(32) int(32) then ", x.type:string);
}

{
  proc testParamProcZero(param arg: bool) param {
    param i: int(32) = 0;
    param u: int(32) = 0;
    if arg then return i; else return u;
  }
  param x = testParamProcZero(false);
  writeln("testParamProcZero: when false int(32) int(32) then ", x.type:string);
}

{
  proc testParamProcCastMax(param arg: bool) param {
    if arg then return max(int(32)):int(32); else return max(int(32)):int(32);
  }
  param x = testParamProcCastMax(false);
  writeln("testParamProcCastMax: when false int(32) int(32) then ", x.type:string);
}

{
  proc testParamProcMax(param arg: bool) param {
    param i: int(32) = max(int(32));
    param u: int(32) = max(int(32));
    if arg then return i; else return u;
  }
  param x = testParamProcMax(false);
  writeln("testParamProcMax: when false int(32) int(32) then ", x.type:string);
}

{
  var b: bool = false;
  var x = if b then 0:int(32) else 0:int(32);
  writeln("testExpressionZero: when false int(32) int(32) then ", x.type:string);
}

{
  var b: bool = false;
  var x = if b then max(int(32)):int(32) else max(int(32)):int(32);
  writeln("testExpressionMax: when false int(32) int(32) then ", x.type:string);
}

{
  param x = if false then 0:int(32) else 0:int(32);
  writeln("testParamExpressionZero: when false int(32) int(32) then ", x.type:string);
}

{
  param x = if false then max(int(32)):int(32) else max(int(32)):int(32);
  writeln("testParamExpressionMax: when false int(32) int(32) then ", x.type:string);
}

{
  proc testProcCastZero(arg: bool) {
    if arg then return 0:int(32); else return 0:int(64);
  }
  var x = testProcCastZero(false);
  writeln("testProcCastZero: when false int(32) int(64) then ", x.type:string);
}

{
  proc testProcZero(arg: bool) {
    var i: int(32) = 0;
    var u: int(64) = 0;
    if arg then return i; else return u;
  }
  var x = testProcZero(false);
  writeln("testProcZero: when false int(32) int(64) then ", x.type:string);
}

{
  proc testProcCastMax(arg: bool) {
    if arg then return max(int(32)):int(32); else return max(int(64)):int(64);
  }
  var x = testProcCastMax(false);
  writeln("testProcCastMax: when false int(32) int(64) then ", x.type:string);
}

{
  proc testProcMax(arg: bool) {
    var i: int(32) = max(int(32));
    var u: int(64) = max(int(64));
    if arg then return i; else return u;
  }
  var x = testProcMax(false);
  writeln("testProcMax: when false int(32) int(64) then ", x.type:string);
}

{
  proc testParamProcCastZero(param arg: bool) param {
    if arg then return 0:int(32); else return 0:int(64);
  }
  param x = testParamProcCastZero(false);
  writeln("testParamProcCastZero: when false int(32) int(64) then ", x.type:string);
}

{
  proc testParamProcZero(param arg: bool) param {
    param i: int(32) = 0;
    param u: int(64) = 0;
    if arg then return i; else return u;
  }
  param x = testParamProcZero(false);
  writeln("testParamProcZero: when false int(32) int(64) then ", x.type:string);
}

{
  proc testParamProcCastMax(param arg: bool) param {
    if arg then return max(int(32)):int(32); else return max(int(64)):int(64);
  }
  param x = testParamProcCastMax(false);
  writeln("testParamProcCastMax: when false int(32) int(64) then ", x.type:string);
}

{
  proc testParamProcMax(param arg: bool) param {
    param i: int(32) = max(int(32));
    param u: int(64) = max(int(64));
    if arg then return i; else return u;
  }
  param x = testParamProcMax(false);
  writeln("testParamProcMax: when false int(32) int(64) then ", x.type:string);
}

{
  var b: bool = false;
  var x = if b then 0:int(32) else 0:int(64);
  writeln("testExpressionZero: when false int(32) int(64) then ", x.type:string);
}

{
  var b: bool = false;
  var x = if b then max(int(32)):int(32) else max(int(64)):int(64);
  writeln("testExpressionMax: when false int(32) int(64) then ", x.type:string);
}

{
  param x = if false then 0:int(32) else 0:int(64);
  writeln("testParamExpressionZero: when false int(32) int(64) then ", x.type:string);
}

{
  param x = if false then max(int(32)):int(32) else max(int(64)):int(64);
  writeln("testParamExpressionMax: when false int(32) int(64) then ", x.type:string);
}

{
  proc testProcCastZero(arg: bool) {
    if arg then return 0:int(32); else return 0:uint(8);
  }
  var x = testProcCastZero(false);
  writeln("testProcCastZero: when false int(32) uint(8) then ", x.type:string);
}

{
  proc testProcZero(arg: bool) {
    var i: int(32) = 0;
    var u: uint(8) = 0;
    if arg then return i; else return u;
  }
  var x = testProcZero(false);
  writeln("testProcZero: when false int(32) uint(8) then ", x.type:string);
}

{
  proc testProcCastMax(arg: bool) {
    if arg then return max(int(32)):int(32); else return max(uint(8)):uint(8);
  }
  var x = testProcCastMax(false);
  writeln("testProcCastMax: when false int(32) uint(8) then ", x.type:string);
}

{
  proc testProcMax(arg: bool) {
    var i: int(32) = max(int(32));
    var u: uint(8) = max(uint(8));
    if arg then return i; else return u;
  }
  var x = testProcMax(false);
  writeln("testProcMax: when false int(32) uint(8) then ", x.type:string);
}

{
  proc testParamProcCastZero(param arg: bool) param {
    if arg then return 0:int(32); else return 0:uint(8);
  }
  param x = testParamProcCastZero(false);
  writeln("testParamProcCastZero: when false int(32) uint(8) then ", x.type:string);
}

{
  proc testParamProcZero(param arg: bool) param {
    param i: int(32) = 0;
    param u: uint(8) = 0;
    if arg then return i; else return u;
  }
  param x = testParamProcZero(false);
  writeln("testParamProcZero: when false int(32) uint(8) then ", x.type:string);
}

{
  proc testParamProcCastMax(param arg: bool) param {
    if arg then return max(int(32)):int(32); else return max(uint(8)):uint(8);
  }
  param x = testParamProcCastMax(false);
  writeln("testParamProcCastMax: when false int(32) uint(8) then ", x.type:string);
}

{
  proc testParamProcMax(param arg: bool) param {
    param i: int(32) = max(int(32));
    param u: uint(8) = max(uint(8));
    if arg then return i; else return u;
  }
  param x = testParamProcMax(false);
  writeln("testParamProcMax: when false int(32) uint(8) then ", x.type:string);
}

{
  var b: bool = false;
  var x = if b then 0:int(32) else 0:uint(8);
  writeln("testExpressionZero: when false int(32) uint(8) then ", x.type:string);
}

{
  var b: bool = false;
  var x = if b then max(int(32)):int(32) else max(uint(8)):uint(8);
  writeln("testExpressionMax: when false int(32) uint(8) then ", x.type:string);
}

{
  param x = if false then 0:int(32) else 0:uint(8);
  writeln("testParamExpressionZero: when false int(32) uint(8) then ", x.type:string);
}

{
  param x = if false then max(int(32)):int(32) else max(uint(8)):uint(8);
  writeln("testParamExpressionMax: when false int(32) uint(8) then ", x.type:string);
}

{
  proc testProcCastZero(arg: bool) {
    if arg then return 0:int(32); else return 0:uint(16);
  }
  var x = testProcCastZero(false);
  writeln("testProcCastZero: when false int(32) uint(16) then ", x.type:string);
}

{
  proc testProcZero(arg: bool) {
    var i: int(32) = 0;
    var u: uint(16) = 0;
    if arg then return i; else return u;
  }
  var x = testProcZero(false);
  writeln("testProcZero: when false int(32) uint(16) then ", x.type:string);
}

{
  proc testProcCastMax(arg: bool) {
    if arg then return max(int(32)):int(32); else return max(uint(16)):uint(16);
  }
  var x = testProcCastMax(false);
  writeln("testProcCastMax: when false int(32) uint(16) then ", x.type:string);
}

{
  proc testProcMax(arg: bool) {
    var i: int(32) = max(int(32));
    var u: uint(16) = max(uint(16));
    if arg then return i; else return u;
  }
  var x = testProcMax(false);
  writeln("testProcMax: when false int(32) uint(16) then ", x.type:string);
}

{
  proc testParamProcCastZero(param arg: bool) param {
    if arg then return 0:int(32); else return 0:uint(16);
  }
  param x = testParamProcCastZero(false);
  writeln("testParamProcCastZero: when false int(32) uint(16) then ", x.type:string);
}

{
  proc testParamProcZero(param arg: bool) param {
    param i: int(32) = 0;
    param u: uint(16) = 0;
    if arg then return i; else return u;
  }
  param x = testParamProcZero(false);
  writeln("testParamProcZero: when false int(32) uint(16) then ", x.type:string);
}

{
  proc testParamProcCastMax(param arg: bool) param {
    if arg then return max(int(32)):int(32); else return max(uint(16)):uint(16);
  }
  param x = testParamProcCastMax(false);
  writeln("testParamProcCastMax: when false int(32) uint(16) then ", x.type:string);
}

{
  proc testParamProcMax(param arg: bool) param {
    param i: int(32) = max(int(32));
    param u: uint(16) = max(uint(16));
    if arg then return i; else return u;
  }
  param x = testParamProcMax(false);
  writeln("testParamProcMax: when false int(32) uint(16) then ", x.type:string);
}

{
  var b: bool = false;
  var x = if b then 0:int(32) else 0:uint(16);
  writeln("testExpressionZero: when false int(32) uint(16) then ", x.type:string);
}

{
  var b: bool = false;
  var x = if b then max(int(32)):int(32) else max(uint(16)):uint(16);
  writeln("testExpressionMax: when false int(32) uint(16) then ", x.type:string);
}

{
  param x = if false then 0:int(32) else 0:uint(16);
  writeln("testParamExpressionZero: when false int(32) uint(16) then ", x.type:string);
}

{
  param x = if false then max(int(32)):int(32) else max(uint(16)):uint(16);
  writeln("testParamExpressionMax: when false int(32) uint(16) then ", x.type:string);
}

{
  proc testProcCastZero(arg: bool) {
    if arg then return 0:int(32); else return 0:uint(32);
  }
  var x = testProcCastZero(false);
  writeln("testProcCastZero: when false int(32) uint(32) then ", x.type:string);
}

{
  proc testProcZero(arg: bool) {
    var i: int(32) = 0;
    var u: uint(32) = 0;
    if arg then return i; else return u;
  }
  var x = testProcZero(false);
  writeln("testProcZero: when false int(32) uint(32) then ", x.type:string);
}

{
  proc testProcCastMax(arg: bool) {
    if arg then return max(int(32)):int(32); else return max(uint(32)):uint(32);
  }
  var x = testProcCastMax(false);
  writeln("testProcCastMax: when false int(32) uint(32) then ", x.type:string);
}

{
  proc testProcMax(arg: bool) {
    var i: int(32) = max(int(32));
    var u: uint(32) = max(uint(32));
    if arg then return i; else return u;
  }
  var x = testProcMax(false);
  writeln("testProcMax: when false int(32) uint(32) then ", x.type:string);
}

{
  proc testParamProcCastZero(param arg: bool) param {
    if arg then return 0:int(32); else return 0:uint(32);
  }
  param x = testParamProcCastZero(false);
  writeln("testParamProcCastZero: when false int(32) uint(32) then ", x.type:string);
}

{
  proc testParamProcZero(param arg: bool) param {
    param i: int(32) = 0;
    param u: uint(32) = 0;
    if arg then return i; else return u;
  }
  param x = testParamProcZero(false);
  writeln("testParamProcZero: when false int(32) uint(32) then ", x.type:string);
}

{
  proc testParamProcCastMax(param arg: bool) param {
    if arg then return max(int(32)):int(32); else return max(uint(32)):uint(32);
  }
  param x = testParamProcCastMax(false);
  writeln("testParamProcCastMax: when false int(32) uint(32) then ", x.type:string);
}

{
  proc testParamProcMax(param arg: bool) param {
    param i: int(32) = max(int(32));
    param u: uint(32) = max(uint(32));
    if arg then return i; else return u;
  }
  param x = testParamProcMax(false);
  writeln("testParamProcMax: when false int(32) uint(32) then ", x.type:string);
}

{
  var b: bool = false;
  var x = if b then 0:int(32) else 0:uint(32);
  writeln("testExpressionZero: when false int(32) uint(32) then ", x.type:string);
}

{
  var b: bool = false;
  var x = if b then max(int(32)):int(32) else max(uint(32)):uint(32);
  writeln("testExpressionMax: when false int(32) uint(32) then ", x.type:string);
}

{
  param x = if false then 0:int(32) else 0:uint(32);
  writeln("testParamExpressionZero: when false int(32) uint(32) then ", x.type:string);
}

{
  param x = if false then max(int(32)):int(32) else max(uint(32)):uint(32);
  writeln("testParamExpressionMax: when false int(32) uint(32) then ", x.type:string);
}

{
  proc testProcCastZero(arg: bool) {
    if arg then return 0:int(32); else return 0:uint(64);
  }
  var x = testProcCastZero(false);
  writeln("testProcCastZero: when false int(32) uint(64) then ", x.type:string);
}

{
  proc testProcZero(arg: bool) {
    var i: int(32) = 0;
    var u: uint(64) = 0;
    if arg then return i; else return u;
  }
  var x = testProcZero(false);
  writeln("testProcZero: when false int(32) uint(64) then ", x.type:string);
}

{
  proc testProcCastMax(arg: bool) {
    if arg then return max(int(32)):int(32); else return max(uint(64)):uint(64);
  }
  var x = testProcCastMax(false);
  writeln("testProcCastMax: when false int(32) uint(64) then ", x.type:string);
}

{
  proc testProcMax(arg: bool) {
    var i: int(32) = max(int(32));
    var u: uint(64) = max(uint(64));
    if arg then return i; else return u;
  }
  var x = testProcMax(false);
  writeln("testProcMax: when false int(32) uint(64) then ", x.type:string);
}

{
  proc testParamProcCastZero(param arg: bool) param {
    if arg then return 0:int(32); else return 0:uint(64);
  }
  param x = testParamProcCastZero(false);
  writeln("testParamProcCastZero: when false int(32) uint(64) then ", x.type:string);
}

{
  proc testParamProcZero(param arg: bool) param {
    param i: int(32) = 0;
    param u: uint(64) = 0;
    if arg then return i; else return u;
  }
  param x = testParamProcZero(false);
  writeln("testParamProcZero: when false int(32) uint(64) then ", x.type:string);
}

{
  proc testParamProcCastMax(param arg: bool) param {
    if arg then return max(int(32)):int(32); else return max(uint(64)):uint(64);
  }
  param x = testParamProcCastMax(false);
  writeln("testParamProcCastMax: when false int(32) uint(64) then ", x.type:string);
}

{
  proc testParamProcMax(param arg: bool) param {
    param i: int(32) = max(int(32));
    param u: uint(64) = max(uint(64));
    if arg then return i; else return u;
  }
  param x = testParamProcMax(false);
  writeln("testParamProcMax: when false int(32) uint(64) then ", x.type:string);
}

{
  var b: bool = false;
  var x = if b then 0:int(32) else 0:uint(64);
  writeln("testExpressionZero: when false int(32) uint(64) then ", x.type:string);
}

{
  var b: bool = false;
  var x = if b then max(int(32)):int(32) else max(uint(64)):uint(64);
  writeln("testExpressionMax: when false int(32) uint(64) then ", x.type:string);
}

{
  param x = if false then 0:int(32) else 0:uint(64);
  writeln("testParamExpressionZero: when false int(32) uint(64) then ", x.type:string);
}

{
  param x = if false then max(int(32)):int(32) else max(uint(64)):uint(64);
  writeln("testParamExpressionMax: when false int(32) uint(64) then ", x.type:string);
}

{
  proc testProcCastZero(arg: bool) {
    if arg then return 0:int(64); else return 0:int(8);
  }
  var x = testProcCastZero(false);
  writeln("testProcCastZero: when false int(64) int(8) then ", x.type:string);
}

{
  proc testProcZero(arg: bool) {
    var i: int(64) = 0;
    var u: int(8) = 0;
    if arg then return i; else return u;
  }
  var x = testProcZero(false);
  writeln("testProcZero: when false int(64) int(8) then ", x.type:string);
}

{
  proc testProcCastMax(arg: bool) {
    if arg then return max(int(64)):int(64); else return max(int(8)):int(8);
  }
  var x = testProcCastMax(false);
  writeln("testProcCastMax: when false int(64) int(8) then ", x.type:string);
}

{
  proc testProcMax(arg: bool) {
    var i: int(64) = max(int(64));
    var u: int(8) = max(int(8));
    if arg then return i; else return u;
  }
  var x = testProcMax(false);
  writeln("testProcMax: when false int(64) int(8) then ", x.type:string);
}

{
  proc testParamProcCastZero(param arg: bool) param {
    if arg then return 0:int(64); else return 0:int(8);
  }
  param x = testParamProcCastZero(false);
  writeln("testParamProcCastZero: when false int(64) int(8) then ", x.type:string);
}

{
  proc testParamProcZero(param arg: bool) param {
    param i: int(64) = 0;
    param u: int(8) = 0;
    if arg then return i; else return u;
  }
  param x = testParamProcZero(false);
  writeln("testParamProcZero: when false int(64) int(8) then ", x.type:string);
}

{
  proc testParamProcCastMax(param arg: bool) param {
    if arg then return max(int(64)):int(64); else return max(int(8)):int(8);
  }
  param x = testParamProcCastMax(false);
  writeln("testParamProcCastMax: when false int(64) int(8) then ", x.type:string);
}

{
  proc testParamProcMax(param arg: bool) param {
    param i: int(64) = max(int(64));
    param u: int(8) = max(int(8));
    if arg then return i; else return u;
  }
  param x = testParamProcMax(false);
  writeln("testParamProcMax: when false int(64) int(8) then ", x.type:string);
}

{
  var b: bool = false;
  var x = if b then 0:int(64) else 0:int(8);
  writeln("testExpressionZero: when false int(64) int(8) then ", x.type:string);
}

{
  var b: bool = false;
  var x = if b then max(int(64)):int(64) else max(int(8)):int(8);
  writeln("testExpressionMax: when false int(64) int(8) then ", x.type:string);
}

{
  param x = if false then 0:int(64) else 0:int(8);
  writeln("testParamExpressionZero: when false int(64) int(8) then ", x.type:string);
}

{
  param x = if false then max(int(64)):int(64) else max(int(8)):int(8);
  writeln("testParamExpressionMax: when false int(64) int(8) then ", x.type:string);
}

{
  proc testProcCastZero(arg: bool) {
    if arg then return 0:int(64); else return 0:int(16);
  }
  var x = testProcCastZero(false);
  writeln("testProcCastZero: when false int(64) int(16) then ", x.type:string);
}

{
  proc testProcZero(arg: bool) {
    var i: int(64) = 0;
    var u: int(16) = 0;
    if arg then return i; else return u;
  }
  var x = testProcZero(false);
  writeln("testProcZero: when false int(64) int(16) then ", x.type:string);
}

{
  proc testProcCastMax(arg: bool) {
    if arg then return max(int(64)):int(64); else return max(int(16)):int(16);
  }
  var x = testProcCastMax(false);
  writeln("testProcCastMax: when false int(64) int(16) then ", x.type:string);
}

{
  proc testProcMax(arg: bool) {
    var i: int(64) = max(int(64));
    var u: int(16) = max(int(16));
    if arg then return i; else return u;
  }
  var x = testProcMax(false);
  writeln("testProcMax: when false int(64) int(16) then ", x.type:string);
}

{
  proc testParamProcCastZero(param arg: bool) param {
    if arg then return 0:int(64); else return 0:int(16);
  }
  param x = testParamProcCastZero(false);
  writeln("testParamProcCastZero: when false int(64) int(16) then ", x.type:string);
}

{
  proc testParamProcZero(param arg: bool) param {
    param i: int(64) = 0;
    param u: int(16) = 0;
    if arg then return i; else return u;
  }
  param x = testParamProcZero(false);
  writeln("testParamProcZero: when false int(64) int(16) then ", x.type:string);
}

{
  proc testParamProcCastMax(param arg: bool) param {
    if arg then return max(int(64)):int(64); else return max(int(16)):int(16);
  }
  param x = testParamProcCastMax(false);
  writeln("testParamProcCastMax: when false int(64) int(16) then ", x.type:string);
}

{
  proc testParamProcMax(param arg: bool) param {
    param i: int(64) = max(int(64));
    param u: int(16) = max(int(16));
    if arg then return i; else return u;
  }
  param x = testParamProcMax(false);
  writeln("testParamProcMax: when false int(64) int(16) then ", x.type:string);
}

{
  var b: bool = false;
  var x = if b then 0:int(64) else 0:int(16);
  writeln("testExpressionZero: when false int(64) int(16) then ", x.type:string);
}

{
  var b: bool = false;
  var x = if b then max(int(64)):int(64) else max(int(16)):int(16);
  writeln("testExpressionMax: when false int(64) int(16) then ", x.type:string);
}

{
  param x = if false then 0:int(64) else 0:int(16);
  writeln("testParamExpressionZero: when false int(64) int(16) then ", x.type:string);
}

{
  param x = if false then max(int(64)):int(64) else max(int(16)):int(16);
  writeln("testParamExpressionMax: when false int(64) int(16) then ", x.type:string);
}

{
  proc testProcCastZero(arg: bool) {
    if arg then return 0:int(64); else return 0:int(32);
  }
  var x = testProcCastZero(false);
  writeln("testProcCastZero: when false int(64) int(32) then ", x.type:string);
}

{
  proc testProcZero(arg: bool) {
    var i: int(64) = 0;
    var u: int(32) = 0;
    if arg then return i; else return u;
  }
  var x = testProcZero(false);
  writeln("testProcZero: when false int(64) int(32) then ", x.type:string);
}

{
  proc testProcCastMax(arg: bool) {
    if arg then return max(int(64)):int(64); else return max(int(32)):int(32);
  }
  var x = testProcCastMax(false);
  writeln("testProcCastMax: when false int(64) int(32) then ", x.type:string);
}

{
  proc testProcMax(arg: bool) {
    var i: int(64) = max(int(64));
    var u: int(32) = max(int(32));
    if arg then return i; else return u;
  }
  var x = testProcMax(false);
  writeln("testProcMax: when false int(64) int(32) then ", x.type:string);
}

{
  proc testParamProcCastZero(param arg: bool) param {
    if arg then return 0:int(64); else return 0:int(32);
  }
  param x = testParamProcCastZero(false);
  writeln("testParamProcCastZero: when false int(64) int(32) then ", x.type:string);
}

{
  proc testParamProcZero(param arg: bool) param {
    param i: int(64) = 0;
    param u: int(32) = 0;
    if arg then return i; else return u;
  }
  param x = testParamProcZero(false);
  writeln("testParamProcZero: when false int(64) int(32) then ", x.type:string);
}

{
  proc testParamProcCastMax(param arg: bool) param {
    if arg then return max(int(64)):int(64); else return max(int(32)):int(32);
  }
  param x = testParamProcCastMax(false);
  writeln("testParamProcCastMax: when false int(64) int(32) then ", x.type:string);
}

{
  proc testParamProcMax(param arg: bool) param {
    param i: int(64) = max(int(64));
    param u: int(32) = max(int(32));
    if arg then return i; else return u;
  }
  param x = testParamProcMax(false);
  writeln("testParamProcMax: when false int(64) int(32) then ", x.type:string);
}

{
  var b: bool = false;
  var x = if b then 0:int(64) else 0:int(32);
  writeln("testExpressionZero: when false int(64) int(32) then ", x.type:string);
}

{
  var b: bool = false;
  var x = if b then max(int(64)):int(64) else max(int(32)):int(32);
  writeln("testExpressionMax: when false int(64) int(32) then ", x.type:string);
}

{
  param x = if false then 0:int(64) else 0:int(32);
  writeln("testParamExpressionZero: when false int(64) int(32) then ", x.type:string);
}

{
  param x = if false then max(int(64)):int(64) else max(int(32)):int(32);
  writeln("testParamExpressionMax: when false int(64) int(32) then ", x.type:string);
}

{
  proc testProcCastZero(arg: bool) {
    if arg then return 0:int(64); else return 0:int(64);
  }
  var x = testProcCastZero(false);
  writeln("testProcCastZero: when false int(64) int(64) then ", x.type:string);
}

{
  proc testProcZero(arg: bool) {
    var i: int(64) = 0;
    var u: int(64) = 0;
    if arg then return i; else return u;
  }
  var x = testProcZero(false);
  writeln("testProcZero: when false int(64) int(64) then ", x.type:string);
}

{
  proc testProcCastMax(arg: bool) {
    if arg then return max(int(64)):int(64); else return max(int(64)):int(64);
  }
  var x = testProcCastMax(false);
  writeln("testProcCastMax: when false int(64) int(64) then ", x.type:string);
}

{
  proc testProcMax(arg: bool) {
    var i: int(64) = max(int(64));
    var u: int(64) = max(int(64));
    if arg then return i; else return u;
  }
  var x = testProcMax(false);
  writeln("testProcMax: when false int(64) int(64) then ", x.type:string);
}

{
  proc testParamProcCastZero(param arg: bool) param {
    if arg then return 0:int(64); else return 0:int(64);
  }
  param x = testParamProcCastZero(false);
  writeln("testParamProcCastZero: when false int(64) int(64) then ", x.type:string);
}

{
  proc testParamProcZero(param arg: bool) param {
    param i: int(64) = 0;
    param u: int(64) = 0;
    if arg then return i; else return u;
  }
  param x = testParamProcZero(false);
  writeln("testParamProcZero: when false int(64) int(64) then ", x.type:string);
}

{
  proc testParamProcCastMax(param arg: bool) param {
    if arg then return max(int(64)):int(64); else return max(int(64)):int(64);
  }
  param x = testParamProcCastMax(false);
  writeln("testParamProcCastMax: when false int(64) int(64) then ", x.type:string);
}

{
  proc testParamProcMax(param arg: bool) param {
    param i: int(64) = max(int(64));
    param u: int(64) = max(int(64));
    if arg then return i; else return u;
  }
  param x = testParamProcMax(false);
  writeln("testParamProcMax: when false int(64) int(64) then ", x.type:string);
}

{
  var b: bool = false;
  var x = if b then 0:int(64) else 0:int(64);
  writeln("testExpressionZero: when false int(64) int(64) then ", x.type:string);
}

{
  var b: bool = false;
  var x = if b then max(int(64)):int(64) else max(int(64)):int(64);
  writeln("testExpressionMax: when false int(64) int(64) then ", x.type:string);
}

{
  param x = if false then 0:int(64) else 0:int(64);
  writeln("testParamExpressionZero: when false int(64) int(64) then ", x.type:string);
}

{
  param x = if false then max(int(64)):int(64) else max(int(64)):int(64);
  writeln("testParamExpressionMax: when false int(64) int(64) then ", x.type:string);
}

{
  proc testProcCastZero(arg: bool) {
    if arg then return 0:int(64); else return 0:uint(8);
  }
  var x = testProcCastZero(false);
  writeln("testProcCastZero: when false int(64) uint(8) then ", x.type:string);
}

{
  proc testProcZero(arg: bool) {
    var i: int(64) = 0;
    var u: uint(8) = 0;
    if arg then return i; else return u;
  }
  var x = testProcZero(false);
  writeln("testProcZero: when false int(64) uint(8) then ", x.type:string);
}

{
  proc testProcCastMax(arg: bool) {
    if arg then return max(int(64)):int(64); else return max(uint(8)):uint(8);
  }
  var x = testProcCastMax(false);
  writeln("testProcCastMax: when false int(64) uint(8) then ", x.type:string);
}

{
  proc testProcMax(arg: bool) {
    var i: int(64) = max(int(64));
    var u: uint(8) = max(uint(8));
    if arg then return i; else return u;
  }
  var x = testProcMax(false);
  writeln("testProcMax: when false int(64) uint(8) then ", x.type:string);
}

{
  proc testParamProcCastZero(param arg: bool) param {
    if arg then return 0:int(64); else return 0:uint(8);
  }
  param x = testParamProcCastZero(false);
  writeln("testParamProcCastZero: when false int(64) uint(8) then ", x.type:string);
}

{
  proc testParamProcZero(param arg: bool) param {
    param i: int(64) = 0;
    param u: uint(8) = 0;
    if arg then return i; else return u;
  }
  param x = testParamProcZero(false);
  writeln("testParamProcZero: when false int(64) uint(8) then ", x.type:string);
}

{
  proc testParamProcCastMax(param arg: bool) param {
    if arg then return max(int(64)):int(64); else return max(uint(8)):uint(8);
  }
  param x = testParamProcCastMax(false);
  writeln("testParamProcCastMax: when false int(64) uint(8) then ", x.type:string);
}

{
  proc testParamProcMax(param arg: bool) param {
    param i: int(64) = max(int(64));
    param u: uint(8) = max(uint(8));
    if arg then return i; else return u;
  }
  param x = testParamProcMax(false);
  writeln("testParamProcMax: when false int(64) uint(8) then ", x.type:string);
}

{
  var b: bool = false;
  var x = if b then 0:int(64) else 0:uint(8);
  writeln("testExpressionZero: when false int(64) uint(8) then ", x.type:string);
}

{
  var b: bool = false;
  var x = if b then max(int(64)):int(64) else max(uint(8)):uint(8);
  writeln("testExpressionMax: when false int(64) uint(8) then ", x.type:string);
}

{
  param x = if false then 0:int(64) else 0:uint(8);
  writeln("testParamExpressionZero: when false int(64) uint(8) then ", x.type:string);
}

{
  param x = if false then max(int(64)):int(64) else max(uint(8)):uint(8);
  writeln("testParamExpressionMax: when false int(64) uint(8) then ", x.type:string);
}

{
  proc testProcCastZero(arg: bool) {
    if arg then return 0:int(64); else return 0:uint(16);
  }
  var x = testProcCastZero(false);
  writeln("testProcCastZero: when false int(64) uint(16) then ", x.type:string);
}

{
  proc testProcZero(arg: bool) {
    var i: int(64) = 0;
    var u: uint(16) = 0;
    if arg then return i; else return u;
  }
  var x = testProcZero(false);
  writeln("testProcZero: when false int(64) uint(16) then ", x.type:string);
}

{
  proc testProcCastMax(arg: bool) {
    if arg then return max(int(64)):int(64); else return max(uint(16)):uint(16);
  }
  var x = testProcCastMax(false);
  writeln("testProcCastMax: when false int(64) uint(16) then ", x.type:string);
}

{
  proc testProcMax(arg: bool) {
    var i: int(64) = max(int(64));
    var u: uint(16) = max(uint(16));
    if arg then return i; else return u;
  }
  var x = testProcMax(false);
  writeln("testProcMax: when false int(64) uint(16) then ", x.type:string);
}

{
  proc testParamProcCastZero(param arg: bool) param {
    if arg then return 0:int(64); else return 0:uint(16);
  }
  param x = testParamProcCastZero(false);
  writeln("testParamProcCastZero: when false int(64) uint(16) then ", x.type:string);
}

{
  proc testParamProcZero(param arg: bool) param {
    param i: int(64) = 0;
    param u: uint(16) = 0;
    if arg then return i; else return u;
  }
  param x = testParamProcZero(false);
  writeln("testParamProcZero: when false int(64) uint(16) then ", x.type:string);
}

{
  proc testParamProcCastMax(param arg: bool) param {
    if arg then return max(int(64)):int(64); else return max(uint(16)):uint(16);
  }
  param x = testParamProcCastMax(false);
  writeln("testParamProcCastMax: when false int(64) uint(16) then ", x.type:string);
}

{
  proc testParamProcMax(param arg: bool) param {
    param i: int(64) = max(int(64));
    param u: uint(16) = max(uint(16));
    if arg then return i; else return u;
  }
  param x = testParamProcMax(false);
  writeln("testParamProcMax: when false int(64) uint(16) then ", x.type:string);
}

{
  var b: bool = false;
  var x = if b then 0:int(64) else 0:uint(16);
  writeln("testExpressionZero: when false int(64) uint(16) then ", x.type:string);
}

{
  var b: bool = false;
  var x = if b then max(int(64)):int(64) else max(uint(16)):uint(16);
  writeln("testExpressionMax: when false int(64) uint(16) then ", x.type:string);
}

{
  param x = if false then 0:int(64) else 0:uint(16);
  writeln("testParamExpressionZero: when false int(64) uint(16) then ", x.type:string);
}

{
  param x = if false then max(int(64)):int(64) else max(uint(16)):uint(16);
  writeln("testParamExpressionMax: when false int(64) uint(16) then ", x.type:string);
}

{
  proc testProcCastZero(arg: bool) {
    if arg then return 0:int(64); else return 0:uint(32);
  }
  var x = testProcCastZero(false);
  writeln("testProcCastZero: when false int(64) uint(32) then ", x.type:string);
}

{
  proc testProcZero(arg: bool) {
    var i: int(64) = 0;
    var u: uint(32) = 0;
    if arg then return i; else return u;
  }
  var x = testProcZero(false);
  writeln("testProcZero: when false int(64) uint(32) then ", x.type:string);
}

{
  proc testProcCastMax(arg: bool) {
    if arg then return max(int(64)):int(64); else return max(uint(32)):uint(32);
  }
  var x = testProcCastMax(false);
  writeln("testProcCastMax: when false int(64) uint(32) then ", x.type:string);
}

{
  proc testProcMax(arg: bool) {
    var i: int(64) = max(int(64));
    var u: uint(32) = max(uint(32));
    if arg then return i; else return u;
  }
  var x = testProcMax(false);
  writeln("testProcMax: when false int(64) uint(32) then ", x.type:string);
}

{
  proc testParamProcCastZero(param arg: bool) param {
    if arg then return 0:int(64); else return 0:uint(32);
  }
  param x = testParamProcCastZero(false);
  writeln("testParamProcCastZero: when false int(64) uint(32) then ", x.type:string);
}

{
  proc testParamProcZero(param arg: bool) param {
    param i: int(64) = 0;
    param u: uint(32) = 0;
    if arg then return i; else return u;
  }
  param x = testParamProcZero(false);
  writeln("testParamProcZero: when false int(64) uint(32) then ", x.type:string);
}

{
  proc testParamProcCastMax(param arg: bool) param {
    if arg then return max(int(64)):int(64); else return max(uint(32)):uint(32);
  }
  param x = testParamProcCastMax(false);
  writeln("testParamProcCastMax: when false int(64) uint(32) then ", x.type:string);
}

{
  proc testParamProcMax(param arg: bool) param {
    param i: int(64) = max(int(64));
    param u: uint(32) = max(uint(32));
    if arg then return i; else return u;
  }
  param x = testParamProcMax(false);
  writeln("testParamProcMax: when false int(64) uint(32) then ", x.type:string);
}

{
  var b: bool = false;
  var x = if b then 0:int(64) else 0:uint(32);
  writeln("testExpressionZero: when false int(64) uint(32) then ", x.type:string);
}

{
  var b: bool = false;
  var x = if b then max(int(64)):int(64) else max(uint(32)):uint(32);
  writeln("testExpressionMax: when false int(64) uint(32) then ", x.type:string);
}

{
  param x = if false then 0:int(64) else 0:uint(32);
  writeln("testParamExpressionZero: when false int(64) uint(32) then ", x.type:string);
}

{
  param x = if false then max(int(64)):int(64) else max(uint(32)):uint(32);
  writeln("testParamExpressionMax: when false int(64) uint(32) then ", x.type:string);
}

{
  proc testProcCastZero(arg: bool) {
    if arg then return 0:int(64); else return 0:uint(64);
  }
  var x = testProcCastZero(false);
  writeln("testProcCastZero: when false int(64) uint(64) then ", x.type:string);
}

{
  proc testProcZero(arg: bool) {
    var i: int(64) = 0;
    var u: uint(64) = 0;
    if arg then return i; else return u;
  }
  var x = testProcZero(false);
  writeln("testProcZero: when false int(64) uint(64) then ", x.type:string);
}

{
  proc testProcCastMax(arg: bool) {
    if arg then return max(int(64)):int(64); else return max(uint(64)):uint(64);
  }
  var x = testProcCastMax(false);
  writeln("testProcCastMax: when false int(64) uint(64) then ", x.type:string);
}

{
  proc testProcMax(arg: bool) {
    var i: int(64) = max(int(64));
    var u: uint(64) = max(uint(64));
    if arg then return i; else return u;
  }
  var x = testProcMax(false);
  writeln("testProcMax: when false int(64) uint(64) then ", x.type:string);
}

{
  proc testParamProcCastZero(param arg: bool) param {
    if arg then return 0:int(64); else return 0:uint(64);
  }
  param x = testParamProcCastZero(false);
  writeln("testParamProcCastZero: when false int(64) uint(64) then ", x.type:string);
}

{
  proc testParamProcZero(param arg: bool) param {
    param i: int(64) = 0;
    param u: uint(64) = 0;
    if arg then return i; else return u;
  }
  param x = testParamProcZero(false);
  writeln("testParamProcZero: when false int(64) uint(64) then ", x.type:string);
}

{
  proc testParamProcCastMax(param arg: bool) param {
    if arg then return max(int(64)):int(64); else return max(uint(64)):uint(64);
  }
  param x = testParamProcCastMax(false);
  writeln("testParamProcCastMax: when false int(64) uint(64) then ", x.type:string);
}

{
  proc testParamProcMax(param arg: bool) param {
    param i: int(64) = max(int(64));
    param u: uint(64) = max(uint(64));
    if arg then return i; else return u;
  }
  param x = testParamProcMax(false);
  writeln("testParamProcMax: when false int(64) uint(64) then ", x.type:string);
}

{
  var b: bool = false;
  var x = if b then 0:int(64) else 0:uint(64);
  writeln("testExpressionZero: when false int(64) uint(64) then ", x.type:string);
}

{
  var b: bool = false;
  var x = if b then max(int(64)):int(64) else max(uint(64)):uint(64);
  writeln("testExpressionMax: when false int(64) uint(64) then ", x.type:string);
}

{
  param x = if false then 0:int(64) else 0:uint(64);
  writeln("testParamExpressionZero: when false int(64) uint(64) then ", x.type:string);
}

{
  param x = if false then max(int(64)):int(64) else max(uint(64)):uint(64);
  writeln("testParamExpressionMax: when false int(64) uint(64) then ", x.type:string);
}

{
  proc testProcCastZero(arg: bool) {
    if arg then return 0:uint(8); else return 0:int(8);
  }
  var x = testProcCastZero(false);
  writeln("testProcCastZero: when false uint(8) int(8) then ", x.type:string);
}

{
  proc testProcZero(arg: bool) {
    var i: uint(8) = 0;
    var u: int(8) = 0;
    if arg then return i; else return u;
  }
  var x = testProcZero(false);
  writeln("testProcZero: when false uint(8) int(8) then ", x.type:string);
}

{
  proc testProcCastMax(arg: bool) {
    if arg then return max(uint(8)):uint(8); else return max(int(8)):int(8);
  }
  var x = testProcCastMax(false);
  writeln("testProcCastMax: when false uint(8) int(8) then ", x.type:string);
}

{
  proc testProcMax(arg: bool) {
    var i: uint(8) = max(uint(8));
    var u: int(8) = max(int(8));
    if arg then return i; else return u;
  }
  var x = testProcMax(false);
  writeln("testProcMax: when false uint(8) int(8) then ", x.type:string);
}

{
  proc testParamProcCastZero(param arg: bool) param {
    if arg then return 0:uint(8); else return 0:int(8);
  }
  param x = testParamProcCastZero(false);
  writeln("testParamProcCastZero: when false uint(8) int(8) then ", x.type:string);
}

{
  proc testParamProcZero(param arg: bool) param {
    param i: uint(8) = 0;
    param u: int(8) = 0;
    if arg then return i; else return u;
  }
  param x = testParamProcZero(false);
  writeln("testParamProcZero: when false uint(8) int(8) then ", x.type:string);
}

{
  proc testParamProcCastMax(param arg: bool) param {
    if arg then return max(uint(8)):uint(8); else return max(int(8)):int(8);
  }
  param x = testParamProcCastMax(false);
  writeln("testParamProcCastMax: when false uint(8) int(8) then ", x.type:string);
}

{
  proc testParamProcMax(param arg: bool) param {
    param i: uint(8) = max(uint(8));
    param u: int(8) = max(int(8));
    if arg then return i; else return u;
  }
  param x = testParamProcMax(false);
  writeln("testParamProcMax: when false uint(8) int(8) then ", x.type:string);
}

{
  var b: bool = false;
  var x = if b then 0:uint(8) else 0:int(8);
  writeln("testExpressionZero: when false uint(8) int(8) then ", x.type:string);
}

{
  var b: bool = false;
  var x = if b then max(uint(8)):uint(8) else max(int(8)):int(8);
  writeln("testExpressionMax: when false uint(8) int(8) then ", x.type:string);
}

{
  param x = if false then 0:uint(8) else 0:int(8);
  writeln("testParamExpressionZero: when false uint(8) int(8) then ", x.type:string);
}

{
  param x = if false then max(uint(8)):uint(8) else max(int(8)):int(8);
  writeln("testParamExpressionMax: when false uint(8) int(8) then ", x.type:string);
}

{
  proc testProcCastZero(arg: bool) {
    if arg then return 0:uint(8); else return 0:int(16);
  }
  var x = testProcCastZero(false);
  writeln("testProcCastZero: when false uint(8) int(16) then ", x.type:string);
}

{
  proc testProcZero(arg: bool) {
    var i: uint(8) = 0;
    var u: int(16) = 0;
    if arg then return i; else return u;
  }
  var x = testProcZero(false);
  writeln("testProcZero: when false uint(8) int(16) then ", x.type:string);
}

{
  proc testProcCastMax(arg: bool) {
    if arg then return max(uint(8)):uint(8); else return max(int(16)):int(16);
  }
  var x = testProcCastMax(false);
  writeln("testProcCastMax: when false uint(8) int(16) then ", x.type:string);
}

{
  proc testProcMax(arg: bool) {
    var i: uint(8) = max(uint(8));
    var u: int(16) = max(int(16));
    if arg then return i; else return u;
  }
  var x = testProcMax(false);
  writeln("testProcMax: when false uint(8) int(16) then ", x.type:string);
}

{
  proc testParamProcCastZero(param arg: bool) param {
    if arg then return 0:uint(8); else return 0:int(16);
  }
  param x = testParamProcCastZero(false);
  writeln("testParamProcCastZero: when false uint(8) int(16) then ", x.type:string);
}

{
  proc testParamProcZero(param arg: bool) param {
    param i: uint(8) = 0;
    param u: int(16) = 0;
    if arg then return i; else return u;
  }
  param x = testParamProcZero(false);
  writeln("testParamProcZero: when false uint(8) int(16) then ", x.type:string);
}

{
  proc testParamProcCastMax(param arg: bool) param {
    if arg then return max(uint(8)):uint(8); else return max(int(16)):int(16);
  }
  param x = testParamProcCastMax(false);
  writeln("testParamProcCastMax: when false uint(8) int(16) then ", x.type:string);
}

{
  proc testParamProcMax(param arg: bool) param {
    param i: uint(8) = max(uint(8));
    param u: int(16) = max(int(16));
    if arg then return i; else return u;
  }
  param x = testParamProcMax(false);
  writeln("testParamProcMax: when false uint(8) int(16) then ", x.type:string);
}

{
  var b: bool = false;
  var x = if b then 0:uint(8) else 0:int(16);
  writeln("testExpressionZero: when false uint(8) int(16) then ", x.type:string);
}

{
  var b: bool = false;
  var x = if b then max(uint(8)):uint(8) else max(int(16)):int(16);
  writeln("testExpressionMax: when false uint(8) int(16) then ", x.type:string);
}

{
  param x = if false then 0:uint(8) else 0:int(16);
  writeln("testParamExpressionZero: when false uint(8) int(16) then ", x.type:string);
}

{
  param x = if false then max(uint(8)):uint(8) else max(int(16)):int(16);
  writeln("testParamExpressionMax: when false uint(8) int(16) then ", x.type:string);
}

{
  proc testProcCastZero(arg: bool) {
    if arg then return 0:uint(8); else return 0:int(32);
  }
  var x = testProcCastZero(false);
  writeln("testProcCastZero: when false uint(8) int(32) then ", x.type:string);
}

{
  proc testProcZero(arg: bool) {
    var i: uint(8) = 0;
    var u: int(32) = 0;
    if arg then return i; else return u;
  }
  var x = testProcZero(false);
  writeln("testProcZero: when false uint(8) int(32) then ", x.type:string);
}

{
  proc testProcCastMax(arg: bool) {
    if arg then return max(uint(8)):uint(8); else return max(int(32)):int(32);
  }
  var x = testProcCastMax(false);
  writeln("testProcCastMax: when false uint(8) int(32) then ", x.type:string);
}

{
  proc testProcMax(arg: bool) {
    var i: uint(8) = max(uint(8));
    var u: int(32) = max(int(32));
    if arg then return i; else return u;
  }
  var x = testProcMax(false);
  writeln("testProcMax: when false uint(8) int(32) then ", x.type:string);
}

{
  proc testParamProcCastZero(param arg: bool) param {
    if arg then return 0:uint(8); else return 0:int(32);
  }
  param x = testParamProcCastZero(false);
  writeln("testParamProcCastZero: when false uint(8) int(32) then ", x.type:string);
}

{
  proc testParamProcZero(param arg: bool) param {
    param i: uint(8) = 0;
    param u: int(32) = 0;
    if arg then return i; else return u;
  }
  param x = testParamProcZero(false);
  writeln("testParamProcZero: when false uint(8) int(32) then ", x.type:string);
}

{
  proc testParamProcCastMax(param arg: bool) param {
    if arg then return max(uint(8)):uint(8); else return max(int(32)):int(32);
  }
  param x = testParamProcCastMax(false);
  writeln("testParamProcCastMax: when false uint(8) int(32) then ", x.type:string);
}

{
  proc testParamProcMax(param arg: bool) param {
    param i: uint(8) = max(uint(8));
    param u: int(32) = max(int(32));
    if arg then return i; else return u;
  }
  param x = testParamProcMax(false);
  writeln("testParamProcMax: when false uint(8) int(32) then ", x.type:string);
}

{
  var b: bool = false;
  var x = if b then 0:uint(8) else 0:int(32);
  writeln("testExpressionZero: when false uint(8) int(32) then ", x.type:string);
}

{
  var b: bool = false;
  var x = if b then max(uint(8)):uint(8) else max(int(32)):int(32);
  writeln("testExpressionMax: when false uint(8) int(32) then ", x.type:string);
}

{
  param x = if false then 0:uint(8) else 0:int(32);
  writeln("testParamExpressionZero: when false uint(8) int(32) then ", x.type:string);
}

{
  param x = if false then max(uint(8)):uint(8) else max(int(32)):int(32);
  writeln("testParamExpressionMax: when false uint(8) int(32) then ", x.type:string);
}

{
  proc testProcCastZero(arg: bool) {
    if arg then return 0:uint(8); else return 0:int(64);
  }
  var x = testProcCastZero(false);
  writeln("testProcCastZero: when false uint(8) int(64) then ", x.type:string);
}

{
  proc testProcZero(arg: bool) {
    var i: uint(8) = 0;
    var u: int(64) = 0;
    if arg then return i; else return u;
  }
  var x = testProcZero(false);
  writeln("testProcZero: when false uint(8) int(64) then ", x.type:string);
}

{
  proc testProcCastMax(arg: bool) {
    if arg then return max(uint(8)):uint(8); else return max(int(64)):int(64);
  }
  var x = testProcCastMax(false);
  writeln("testProcCastMax: when false uint(8) int(64) then ", x.type:string);
}

{
  proc testProcMax(arg: bool) {
    var i: uint(8) = max(uint(8));
    var u: int(64) = max(int(64));
    if arg then return i; else return u;
  }
  var x = testProcMax(false);
  writeln("testProcMax: when false uint(8) int(64) then ", x.type:string);
}

{
  proc testParamProcCastZero(param arg: bool) param {
    if arg then return 0:uint(8); else return 0:int(64);
  }
  param x = testParamProcCastZero(false);
  writeln("testParamProcCastZero: when false uint(8) int(64) then ", x.type:string);
}

{
  proc testParamProcZero(param arg: bool) param {
    param i: uint(8) = 0;
    param u: int(64) = 0;
    if arg then return i; else return u;
  }
  param x = testParamProcZero(false);
  writeln("testParamProcZero: when false uint(8) int(64) then ", x.type:string);
}

{
  proc testParamProcCastMax(param arg: bool) param {
    if arg then return max(uint(8)):uint(8); else return max(int(64)):int(64);
  }
  param x = testParamProcCastMax(false);
  writeln("testParamProcCastMax: when false uint(8) int(64) then ", x.type:string);
}

{
  proc testParamProcMax(param arg: bool) param {
    param i: uint(8) = max(uint(8));
    param u: int(64) = max(int(64));
    if arg then return i; else return u;
  }
  param x = testParamProcMax(false);
  writeln("testParamProcMax: when false uint(8) int(64) then ", x.type:string);
}

{
  var b: bool = false;
  var x = if b then 0:uint(8) else 0:int(64);
  writeln("testExpressionZero: when false uint(8) int(64) then ", x.type:string);
}

{
  var b: bool = false;
  var x = if b then max(uint(8)):uint(8) else max(int(64)):int(64);
  writeln("testExpressionMax: when false uint(8) int(64) then ", x.type:string);
}

{
  param x = if false then 0:uint(8) else 0:int(64);
  writeln("testParamExpressionZero: when false uint(8) int(64) then ", x.type:string);
}

{
  param x = if false then max(uint(8)):uint(8) else max(int(64)):int(64);
  writeln("testParamExpressionMax: when false uint(8) int(64) then ", x.type:string);
}

{
  proc testProcCastZero(arg: bool) {
    if arg then return 0:uint(8); else return 0:uint(8);
  }
  var x = testProcCastZero(false);
  writeln("testProcCastZero: when false uint(8) uint(8) then ", x.type:string);
}

{
  proc testProcZero(arg: bool) {
    var i: uint(8) = 0;
    var u: uint(8) = 0;
    if arg then return i; else return u;
  }
  var x = testProcZero(false);
  writeln("testProcZero: when false uint(8) uint(8) then ", x.type:string);
}

{
  proc testProcCastMax(arg: bool) {
    if arg then return max(uint(8)):uint(8); else return max(uint(8)):uint(8);
  }
  var x = testProcCastMax(false);
  writeln("testProcCastMax: when false uint(8) uint(8) then ", x.type:string);
}

{
  proc testProcMax(arg: bool) {
    var i: uint(8) = max(uint(8));
    var u: uint(8) = max(uint(8));
    if arg then return i; else return u;
  }
  var x = testProcMax(false);
  writeln("testProcMax: when false uint(8) uint(8) then ", x.type:string);
}

{
  proc testParamProcCastZero(param arg: bool) param {
    if arg then return 0:uint(8); else return 0:uint(8);
  }
  param x = testParamProcCastZero(false);
  writeln("testParamProcCastZero: when false uint(8) uint(8) then ", x.type:string);
}

{
  proc testParamProcZero(param arg: bool) param {
    param i: uint(8) = 0;
    param u: uint(8) = 0;
    if arg then return i; else return u;
  }
  param x = testParamProcZero(false);
  writeln("testParamProcZero: when false uint(8) uint(8) then ", x.type:string);
}

{
  proc testParamProcCastMax(param arg: bool) param {
    if arg then return max(uint(8)):uint(8); else return max(uint(8)):uint(8);
  }
  param x = testParamProcCastMax(false);
  writeln("testParamProcCastMax: when false uint(8) uint(8) then ", x.type:string);
}

{
  proc testParamProcMax(param arg: bool) param {
    param i: uint(8) = max(uint(8));
    param u: uint(8) = max(uint(8));
    if arg then return i; else return u;
  }
  param x = testParamProcMax(false);
  writeln("testParamProcMax: when false uint(8) uint(8) then ", x.type:string);
}

{
  var b: bool = false;
  var x = if b then 0:uint(8) else 0:uint(8);
  writeln("testExpressionZero: when false uint(8) uint(8) then ", x.type:string);
}

{
  var b: bool = false;
  var x = if b then max(uint(8)):uint(8) else max(uint(8)):uint(8);
  writeln("testExpressionMax: when false uint(8) uint(8) then ", x.type:string);
}

{
  param x = if false then 0:uint(8) else 0:uint(8);
  writeln("testParamExpressionZero: when false uint(8) uint(8) then ", x.type:string);
}

{
  param x = if false then max(uint(8)):uint(8) else max(uint(8)):uint(8);
  writeln("testParamExpressionMax: when false uint(8) uint(8) then ", x.type:string);
}

{
  proc testProcCastZero(arg: bool) {
    if arg then return 0:uint(8); else return 0:uint(16);
  }
  var x = testProcCastZero(false);
  writeln("testProcCastZero: when false uint(8) uint(16) then ", x.type:string);
}

{
  proc testProcZero(arg: bool) {
    var i: uint(8) = 0;
    var u: uint(16) = 0;
    if arg then return i; else return u;
  }
  var x = testProcZero(false);
  writeln("testProcZero: when false uint(8) uint(16) then ", x.type:string);
}

{
  proc testProcCastMax(arg: bool) {
    if arg then return max(uint(8)):uint(8); else return max(uint(16)):uint(16);
  }
  var x = testProcCastMax(false);
  writeln("testProcCastMax: when false uint(8) uint(16) then ", x.type:string);
}

{
  proc testProcMax(arg: bool) {
    var i: uint(8) = max(uint(8));
    var u: uint(16) = max(uint(16));
    if arg then return i; else return u;
  }
  var x = testProcMax(false);
  writeln("testProcMax: when false uint(8) uint(16) then ", x.type:string);
}

{
  proc testParamProcCastZero(param arg: bool) param {
    if arg then return 0:uint(8); else return 0:uint(16);
  }
  param x = testParamProcCastZero(false);
  writeln("testParamProcCastZero: when false uint(8) uint(16) then ", x.type:string);
}

{
  proc testParamProcZero(param arg: bool) param {
    param i: uint(8) = 0;
    param u: uint(16) = 0;
    if arg then return i; else return u;
  }
  param x = testParamProcZero(false);
  writeln("testParamProcZero: when false uint(8) uint(16) then ", x.type:string);
}

{
  proc testParamProcCastMax(param arg: bool) param {
    if arg then return max(uint(8)):uint(8); else return max(uint(16)):uint(16);
  }
  param x = testParamProcCastMax(false);
  writeln("testParamProcCastMax: when false uint(8) uint(16) then ", x.type:string);
}

{
  proc testParamProcMax(param arg: bool) param {
    param i: uint(8) = max(uint(8));
    param u: uint(16) = max(uint(16));
    if arg then return i; else return u;
  }
  param x = testParamProcMax(false);
  writeln("testParamProcMax: when false uint(8) uint(16) then ", x.type:string);
}

{
  var b: bool = false;
  var x = if b then 0:uint(8) else 0:uint(16);
  writeln("testExpressionZero: when false uint(8) uint(16) then ", x.type:string);
}

{
  var b: bool = false;
  var x = if b then max(uint(8)):uint(8) else max(uint(16)):uint(16);
  writeln("testExpressionMax: when false uint(8) uint(16) then ", x.type:string);
}

{
  param x = if false then 0:uint(8) else 0:uint(16);
  writeln("testParamExpressionZero: when false uint(8) uint(16) then ", x.type:string);
}

{
  param x = if false then max(uint(8)):uint(8) else max(uint(16)):uint(16);
  writeln("testParamExpressionMax: when false uint(8) uint(16) then ", x.type:string);
}

{
  proc testProcCastZero(arg: bool) {
    if arg then return 0:uint(8); else return 0:uint(32);
  }
  var x = testProcCastZero(false);
  writeln("testProcCastZero: when false uint(8) uint(32) then ", x.type:string);
}

{
  proc testProcZero(arg: bool) {
    var i: uint(8) = 0;
    var u: uint(32) = 0;
    if arg then return i; else return u;
  }
  var x = testProcZero(false);
  writeln("testProcZero: when false uint(8) uint(32) then ", x.type:string);
}

{
  proc testProcCastMax(arg: bool) {
    if arg then return max(uint(8)):uint(8); else return max(uint(32)):uint(32);
  }
  var x = testProcCastMax(false);
  writeln("testProcCastMax: when false uint(8) uint(32) then ", x.type:string);
}

{
  proc testProcMax(arg: bool) {
    var i: uint(8) = max(uint(8));
    var u: uint(32) = max(uint(32));
    if arg then return i; else return u;
  }
  var x = testProcMax(false);
  writeln("testProcMax: when false uint(8) uint(32) then ", x.type:string);
}

{
  proc testParamProcCastZero(param arg: bool) param {
    if arg then return 0:uint(8); else return 0:uint(32);
  }
  param x = testParamProcCastZero(false);
  writeln("testParamProcCastZero: when false uint(8) uint(32) then ", x.type:string);
}

{
  proc testParamProcZero(param arg: bool) param {
    param i: uint(8) = 0;
    param u: uint(32) = 0;
    if arg then return i; else return u;
  }
  param x = testParamProcZero(false);
  writeln("testParamProcZero: when false uint(8) uint(32) then ", x.type:string);
}

{
  proc testParamProcCastMax(param arg: bool) param {
    if arg then return max(uint(8)):uint(8); else return max(uint(32)):uint(32);
  }
  param x = testParamProcCastMax(false);
  writeln("testParamProcCastMax: when false uint(8) uint(32) then ", x.type:string);
}

{
  proc testParamProcMax(param arg: bool) param {
    param i: uint(8) = max(uint(8));
    param u: uint(32) = max(uint(32));
    if arg then return i; else return u;
  }
  param x = testParamProcMax(false);
  writeln("testParamProcMax: when false uint(8) uint(32) then ", x.type:string);
}

{
  var b: bool = false;
  var x = if b then 0:uint(8) else 0:uint(32);
  writeln("testExpressionZero: when false uint(8) uint(32) then ", x.type:string);
}

{
  var b: bool = false;
  var x = if b then max(uint(8)):uint(8) else max(uint(32)):uint(32);
  writeln("testExpressionMax: when false uint(8) uint(32) then ", x.type:string);
}

{
  param x = if false then 0:uint(8) else 0:uint(32);
  writeln("testParamExpressionZero: when false uint(8) uint(32) then ", x.type:string);
}

{
  param x = if false then max(uint(8)):uint(8) else max(uint(32)):uint(32);
  writeln("testParamExpressionMax: when false uint(8) uint(32) then ", x.type:string);
}

{
  proc testProcCastZero(arg: bool) {
    if arg then return 0:uint(8); else return 0:uint(64);
  }
  var x = testProcCastZero(false);
  writeln("testProcCastZero: when false uint(8) uint(64) then ", x.type:string);
}

{
  proc testProcZero(arg: bool) {
    var i: uint(8) = 0;
    var u: uint(64) = 0;
    if arg then return i; else return u;
  }
  var x = testProcZero(false);
  writeln("testProcZero: when false uint(8) uint(64) then ", x.type:string);
}

{
  proc testProcCastMax(arg: bool) {
    if arg then return max(uint(8)):uint(8); else return max(uint(64)):uint(64);
  }
  var x = testProcCastMax(false);
  writeln("testProcCastMax: when false uint(8) uint(64) then ", x.type:string);
}

{
  proc testProcMax(arg: bool) {
    var i: uint(8) = max(uint(8));
    var u: uint(64) = max(uint(64));
    if arg then return i; else return u;
  }
  var x = testProcMax(false);
  writeln("testProcMax: when false uint(8) uint(64) then ", x.type:string);
}

{
  proc testParamProcCastZero(param arg: bool) param {
    if arg then return 0:uint(8); else return 0:uint(64);
  }
  param x = testParamProcCastZero(false);
  writeln("testParamProcCastZero: when false uint(8) uint(64) then ", x.type:string);
}

{
  proc testParamProcZero(param arg: bool) param {
    param i: uint(8) = 0;
    param u: uint(64) = 0;
    if arg then return i; else return u;
  }
  param x = testParamProcZero(false);
  writeln("testParamProcZero: when false uint(8) uint(64) then ", x.type:string);
}

{
  proc testParamProcCastMax(param arg: bool) param {
    if arg then return max(uint(8)):uint(8); else return max(uint(64)):uint(64);
  }
  param x = testParamProcCastMax(false);
  writeln("testParamProcCastMax: when false uint(8) uint(64) then ", x.type:string);
}

{
  proc testParamProcMax(param arg: bool) param {
    param i: uint(8) = max(uint(8));
    param u: uint(64) = max(uint(64));
    if arg then return i; else return u;
  }
  param x = testParamProcMax(false);
  writeln("testParamProcMax: when false uint(8) uint(64) then ", x.type:string);
}

{
  var b: bool = false;
  var x = if b then 0:uint(8) else 0:uint(64);
  writeln("testExpressionZero: when false uint(8) uint(64) then ", x.type:string);
}

{
  var b: bool = false;
  var x = if b then max(uint(8)):uint(8) else max(uint(64)):uint(64);
  writeln("testExpressionMax: when false uint(8) uint(64) then ", x.type:string);
}

{
  param x = if false then 0:uint(8) else 0:uint(64);
  writeln("testParamExpressionZero: when false uint(8) uint(64) then ", x.type:string);
}

{
  param x = if false then max(uint(8)):uint(8) else max(uint(64)):uint(64);
  writeln("testParamExpressionMax: when false uint(8) uint(64) then ", x.type:string);
}

{
  proc testProcCastZero(arg: bool) {
    if arg then return 0:uint(16); else return 0:int(8);
  }
  var x = testProcCastZero(false);
  writeln("testProcCastZero: when false uint(16) int(8) then ", x.type:string);
}

{
  proc testProcZero(arg: bool) {
    var i: uint(16) = 0;
    var u: int(8) = 0;
    if arg then return i; else return u;
  }
  var x = testProcZero(false);
  writeln("testProcZero: when false uint(16) int(8) then ", x.type:string);
}

{
  proc testProcCastMax(arg: bool) {
    if arg then return max(uint(16)):uint(16); else return max(int(8)):int(8);
  }
  var x = testProcCastMax(false);
  writeln("testProcCastMax: when false uint(16) int(8) then ", x.type:string);
}

{
  proc testProcMax(arg: bool) {
    var i: uint(16) = max(uint(16));
    var u: int(8) = max(int(8));
    if arg then return i; else return u;
  }
  var x = testProcMax(false);
  writeln("testProcMax: when false uint(16) int(8) then ", x.type:string);
}

{
  proc testParamProcCastZero(param arg: bool) param {
    if arg then return 0:uint(16); else return 0:int(8);
  }
  param x = testParamProcCastZero(false);
  writeln("testParamProcCastZero: when false uint(16) int(8) then ", x.type:string);
}

{
  proc testParamProcZero(param arg: bool) param {
    param i: uint(16) = 0;
    param u: int(8) = 0;
    if arg then return i; else return u;
  }
  param x = testParamProcZero(false);
  writeln("testParamProcZero: when false uint(16) int(8) then ", x.type:string);
}

{
  proc testParamProcCastMax(param arg: bool) param {
    if arg then return max(uint(16)):uint(16); else return max(int(8)):int(8);
  }
  param x = testParamProcCastMax(false);
  writeln("testParamProcCastMax: when false uint(16) int(8) then ", x.type:string);
}

{
  proc testParamProcMax(param arg: bool) param {
    param i: uint(16) = max(uint(16));
    param u: int(8) = max(int(8));
    if arg then return i; else return u;
  }
  param x = testParamProcMax(false);
  writeln("testParamProcMax: when false uint(16) int(8) then ", x.type:string);
}

{
  var b: bool = false;
  var x = if b then 0:uint(16) else 0:int(8);
  writeln("testExpressionZero: when false uint(16) int(8) then ", x.type:string);
}

{
  var b: bool = false;
  var x = if b then max(uint(16)):uint(16) else max(int(8)):int(8);
  writeln("testExpressionMax: when false uint(16) int(8) then ", x.type:string);
}

{
  param x = if false then 0:uint(16) else 0:int(8);
  writeln("testParamExpressionZero: when false uint(16) int(8) then ", x.type:string);
}

{
  param x = if false then max(uint(16)):uint(16) else max(int(8)):int(8);
  writeln("testParamExpressionMax: when false uint(16) int(8) then ", x.type:string);
}

{
  proc testProcCastZero(arg: bool) {
    if arg then return 0:uint(16); else return 0:int(16);
  }
  var x = testProcCastZero(false);
  writeln("testProcCastZero: when false uint(16) int(16) then ", x.type:string);
}

{
  proc testProcZero(arg: bool) {
    var i: uint(16) = 0;
    var u: int(16) = 0;
    if arg then return i; else return u;
  }
  var x = testProcZero(false);
  writeln("testProcZero: when false uint(16) int(16) then ", x.type:string);
}

{
  proc testProcCastMax(arg: bool) {
    if arg then return max(uint(16)):uint(16); else return max(int(16)):int(16);
  }
  var x = testProcCastMax(false);
  writeln("testProcCastMax: when false uint(16) int(16) then ", x.type:string);
}

{
  proc testProcMax(arg: bool) {
    var i: uint(16) = max(uint(16));
    var u: int(16) = max(int(16));
    if arg then return i; else return u;
  }
  var x = testProcMax(false);
  writeln("testProcMax: when false uint(16) int(16) then ", x.type:string);
}

{
  proc testParamProcCastZero(param arg: bool) param {
    if arg then return 0:uint(16); else return 0:int(16);
  }
  param x = testParamProcCastZero(false);
  writeln("testParamProcCastZero: when false uint(16) int(16) then ", x.type:string);
}

{
  proc testParamProcZero(param arg: bool) param {
    param i: uint(16) = 0;
    param u: int(16) = 0;
    if arg then return i; else return u;
  }
  param x = testParamProcZero(false);
  writeln("testParamProcZero: when false uint(16) int(16) then ", x.type:string);
}

{
  proc testParamProcCastMax(param arg: bool) param {
    if arg then return max(uint(16)):uint(16); else return max(int(16)):int(16);
  }
  param x = testParamProcCastMax(false);
  writeln("testParamProcCastMax: when false uint(16) int(16) then ", x.type:string);
}

{
  proc testParamProcMax(param arg: bool) param {
    param i: uint(16) = max(uint(16));
    param u: int(16) = max(int(16));
    if arg then return i; else return u;
  }
  param x = testParamProcMax(false);
  writeln("testParamProcMax: when false uint(16) int(16) then ", x.type:string);
}

{
  var b: bool = false;
  var x = if b then 0:uint(16) else 0:int(16);
  writeln("testExpressionZero: when false uint(16) int(16) then ", x.type:string);
}

{
  var b: bool = false;
  var x = if b then max(uint(16)):uint(16) else max(int(16)):int(16);
  writeln("testExpressionMax: when false uint(16) int(16) then ", x.type:string);
}

{
  param x = if false then 0:uint(16) else 0:int(16);
  writeln("testParamExpressionZero: when false uint(16) int(16) then ", x.type:string);
}

{
  param x = if false then max(uint(16)):uint(16) else max(int(16)):int(16);
  writeln("testParamExpressionMax: when false uint(16) int(16) then ", x.type:string);
}

{
  proc testProcCastZero(arg: bool) {
    if arg then return 0:uint(16); else return 0:int(32);
  }
  var x = testProcCastZero(false);
  writeln("testProcCastZero: when false uint(16) int(32) then ", x.type:string);
}

{
  proc testProcZero(arg: bool) {
    var i: uint(16) = 0;
    var u: int(32) = 0;
    if arg then return i; else return u;
  }
  var x = testProcZero(false);
  writeln("testProcZero: when false uint(16) int(32) then ", x.type:string);
}

{
  proc testProcCastMax(arg: bool) {
    if arg then return max(uint(16)):uint(16); else return max(int(32)):int(32);
  }
  var x = testProcCastMax(false);
  writeln("testProcCastMax: when false uint(16) int(32) then ", x.type:string);
}

{
  proc testProcMax(arg: bool) {
    var i: uint(16) = max(uint(16));
    var u: int(32) = max(int(32));
    if arg then return i; else return u;
  }
  var x = testProcMax(false);
  writeln("testProcMax: when false uint(16) int(32) then ", x.type:string);
}

{
  proc testParamProcCastZero(param arg: bool) param {
    if arg then return 0:uint(16); else return 0:int(32);
  }
  param x = testParamProcCastZero(false);
  writeln("testParamProcCastZero: when false uint(16) int(32) then ", x.type:string);
}

{
  proc testParamProcZero(param arg: bool) param {
    param i: uint(16) = 0;
    param u: int(32) = 0;
    if arg then return i; else return u;
  }
  param x = testParamProcZero(false);
  writeln("testParamProcZero: when false uint(16) int(32) then ", x.type:string);
}

{
  proc testParamProcCastMax(param arg: bool) param {
    if arg then return max(uint(16)):uint(16); else return max(int(32)):int(32);
  }
  param x = testParamProcCastMax(false);
  writeln("testParamProcCastMax: when false uint(16) int(32) then ", x.type:string);
}

{
  proc testParamProcMax(param arg: bool) param {
    param i: uint(16) = max(uint(16));
    param u: int(32) = max(int(32));
    if arg then return i; else return u;
  }
  param x = testParamProcMax(false);
  writeln("testParamProcMax: when false uint(16) int(32) then ", x.type:string);
}

{
  var b: bool = false;
  var x = if b then 0:uint(16) else 0:int(32);
  writeln("testExpressionZero: when false uint(16) int(32) then ", x.type:string);
}

{
  var b: bool = false;
  var x = if b then max(uint(16)):uint(16) else max(int(32)):int(32);
  writeln("testExpressionMax: when false uint(16) int(32) then ", x.type:string);
}

{
  param x = if false then 0:uint(16) else 0:int(32);
  writeln("testParamExpressionZero: when false uint(16) int(32) then ", x.type:string);
}

{
  param x = if false then max(uint(16)):uint(16) else max(int(32)):int(32);
  writeln("testParamExpressionMax: when false uint(16) int(32) then ", x.type:string);
}

{
  proc testProcCastZero(arg: bool) {
    if arg then return 0:uint(16); else return 0:int(64);
  }
  var x = testProcCastZero(false);
  writeln("testProcCastZero: when false uint(16) int(64) then ", x.type:string);
}

{
  proc testProcZero(arg: bool) {
    var i: uint(16) = 0;
    var u: int(64) = 0;
    if arg then return i; else return u;
  }
  var x = testProcZero(false);
  writeln("testProcZero: when false uint(16) int(64) then ", x.type:string);
}

{
  proc testProcCastMax(arg: bool) {
    if arg then return max(uint(16)):uint(16); else return max(int(64)):int(64);
  }
  var x = testProcCastMax(false);
  writeln("testProcCastMax: when false uint(16) int(64) then ", x.type:string);
}

{
  proc testProcMax(arg: bool) {
    var i: uint(16) = max(uint(16));
    var u: int(64) = max(int(64));
    if arg then return i; else return u;
  }
  var x = testProcMax(false);
  writeln("testProcMax: when false uint(16) int(64) then ", x.type:string);
}

{
  proc testParamProcCastZero(param arg: bool) param {
    if arg then return 0:uint(16); else return 0:int(64);
  }
  param x = testParamProcCastZero(false);
  writeln("testParamProcCastZero: when false uint(16) int(64) then ", x.type:string);
}

{
  proc testParamProcZero(param arg: bool) param {
    param i: uint(16) = 0;
    param u: int(64) = 0;
    if arg then return i; else return u;
  }
  param x = testParamProcZero(false);
  writeln("testParamProcZero: when false uint(16) int(64) then ", x.type:string);
}

{
  proc testParamProcCastMax(param arg: bool) param {
    if arg then return max(uint(16)):uint(16); else return max(int(64)):int(64);
  }
  param x = testParamProcCastMax(false);
  writeln("testParamProcCastMax: when false uint(16) int(64) then ", x.type:string);
}

{
  proc testParamProcMax(param arg: bool) param {
    param i: uint(16) = max(uint(16));
    param u: int(64) = max(int(64));
    if arg then return i; else return u;
  }
  param x = testParamProcMax(false);
  writeln("testParamProcMax: when false uint(16) int(64) then ", x.type:string);
}

{
  var b: bool = false;
  var x = if b then 0:uint(16) else 0:int(64);
  writeln("testExpressionZero: when false uint(16) int(64) then ", x.type:string);
}

{
  var b: bool = false;
  var x = if b then max(uint(16)):uint(16) else max(int(64)):int(64);
  writeln("testExpressionMax: when false uint(16) int(64) then ", x.type:string);
}

{
  param x = if false then 0:uint(16) else 0:int(64);
  writeln("testParamExpressionZero: when false uint(16) int(64) then ", x.type:string);
}

{
  param x = if false then max(uint(16)):uint(16) else max(int(64)):int(64);
  writeln("testParamExpressionMax: when false uint(16) int(64) then ", x.type:string);
}

{
  proc testProcCastZero(arg: bool) {
    if arg then return 0:uint(16); else return 0:uint(8);
  }
  var x = testProcCastZero(false);
  writeln("testProcCastZero: when false uint(16) uint(8) then ", x.type:string);
}

{
  proc testProcZero(arg: bool) {
    var i: uint(16) = 0;
    var u: uint(8) = 0;
    if arg then return i; else return u;
  }
  var x = testProcZero(false);
  writeln("testProcZero: when false uint(16) uint(8) then ", x.type:string);
}

{
  proc testProcCastMax(arg: bool) {
    if arg then return max(uint(16)):uint(16); else return max(uint(8)):uint(8);
  }
  var x = testProcCastMax(false);
  writeln("testProcCastMax: when false uint(16) uint(8) then ", x.type:string);
}

{
  proc testProcMax(arg: bool) {
    var i: uint(16) = max(uint(16));
    var u: uint(8) = max(uint(8));
    if arg then return i; else return u;
  }
  var x = testProcMax(false);
  writeln("testProcMax: when false uint(16) uint(8) then ", x.type:string);
}

{
  proc testParamProcCastZero(param arg: bool) param {
    if arg then return 0:uint(16); else return 0:uint(8);
  }
  param x = testParamProcCastZero(false);
  writeln("testParamProcCastZero: when false uint(16) uint(8) then ", x.type:string);
}

{
  proc testParamProcZero(param arg: bool) param {
    param i: uint(16) = 0;
    param u: uint(8) = 0;
    if arg then return i; else return u;
  }
  param x = testParamProcZero(false);
  writeln("testParamProcZero: when false uint(16) uint(8) then ", x.type:string);
}

{
  proc testParamProcCastMax(param arg: bool) param {
    if arg then return max(uint(16)):uint(16); else return max(uint(8)):uint(8);
  }
  param x = testParamProcCastMax(false);
  writeln("testParamProcCastMax: when false uint(16) uint(8) then ", x.type:string);
}

{
  proc testParamProcMax(param arg: bool) param {
    param i: uint(16) = max(uint(16));
    param u: uint(8) = max(uint(8));
    if arg then return i; else return u;
  }
  param x = testParamProcMax(false);
  writeln("testParamProcMax: when false uint(16) uint(8) then ", x.type:string);
}

{
  var b: bool = false;
  var x = if b then 0:uint(16) else 0:uint(8);
  writeln("testExpressionZero: when false uint(16) uint(8) then ", x.type:string);
}

{
  var b: bool = false;
  var x = if b then max(uint(16)):uint(16) else max(uint(8)):uint(8);
  writeln("testExpressionMax: when false uint(16) uint(8) then ", x.type:string);
}

{
  param x = if false then 0:uint(16) else 0:uint(8);
  writeln("testParamExpressionZero: when false uint(16) uint(8) then ", x.type:string);
}

{
  param x = if false then max(uint(16)):uint(16) else max(uint(8)):uint(8);
  writeln("testParamExpressionMax: when false uint(16) uint(8) then ", x.type:string);
}

{
  proc testProcCastZero(arg: bool) {
    if arg then return 0:uint(16); else return 0:uint(16);
  }
  var x = testProcCastZero(false);
  writeln("testProcCastZero: when false uint(16) uint(16) then ", x.type:string);
}

{
  proc testProcZero(arg: bool) {
    var i: uint(16) = 0;
    var u: uint(16) = 0;
    if arg then return i; else return u;
  }
  var x = testProcZero(false);
  writeln("testProcZero: when false uint(16) uint(16) then ", x.type:string);
}

{
  proc testProcCastMax(arg: bool) {
    if arg then return max(uint(16)):uint(16); else return max(uint(16)):uint(16);
  }
  var x = testProcCastMax(false);
  writeln("testProcCastMax: when false uint(16) uint(16) then ", x.type:string);
}

{
  proc testProcMax(arg: bool) {
    var i: uint(16) = max(uint(16));
    var u: uint(16) = max(uint(16));
    if arg then return i; else return u;
  }
  var x = testProcMax(false);
  writeln("testProcMax: when false uint(16) uint(16) then ", x.type:string);
}

{
  proc testParamProcCastZero(param arg: bool) param {
    if arg then return 0:uint(16); else return 0:uint(16);
  }
  param x = testParamProcCastZero(false);
  writeln("testParamProcCastZero: when false uint(16) uint(16) then ", x.type:string);
}

{
  proc testParamProcZero(param arg: bool) param {
    param i: uint(16) = 0;
    param u: uint(16) = 0;
    if arg then return i; else return u;
  }
  param x = testParamProcZero(false);
  writeln("testParamProcZero: when false uint(16) uint(16) then ", x.type:string);
}

{
  proc testParamProcCastMax(param arg: bool) param {
    if arg then return max(uint(16)):uint(16); else return max(uint(16)):uint(16);
  }
  param x = testParamProcCastMax(false);
  writeln("testParamProcCastMax: when false uint(16) uint(16) then ", x.type:string);
}

{
  proc testParamProcMax(param arg: bool) param {
    param i: uint(16) = max(uint(16));
    param u: uint(16) = max(uint(16));
    if arg then return i; else return u;
  }
  param x = testParamProcMax(false);
  writeln("testParamProcMax: when false uint(16) uint(16) then ", x.type:string);
}

{
  var b: bool = false;
  var x = if b then 0:uint(16) else 0:uint(16);
  writeln("testExpressionZero: when false uint(16) uint(16) then ", x.type:string);
}

{
  var b: bool = false;
  var x = if b then max(uint(16)):uint(16) else max(uint(16)):uint(16);
  writeln("testExpressionMax: when false uint(16) uint(16) then ", x.type:string);
}

{
  param x = if false then 0:uint(16) else 0:uint(16);
  writeln("testParamExpressionZero: when false uint(16) uint(16) then ", x.type:string);
}

{
  param x = if false then max(uint(16)):uint(16) else max(uint(16)):uint(16);
  writeln("testParamExpressionMax: when false uint(16) uint(16) then ", x.type:string);
}

{
  proc testProcCastZero(arg: bool) {
    if arg then return 0:uint(16); else return 0:uint(32);
  }
  var x = testProcCastZero(false);
  writeln("testProcCastZero: when false uint(16) uint(32) then ", x.type:string);
}

{
  proc testProcZero(arg: bool) {
    var i: uint(16) = 0;
    var u: uint(32) = 0;
    if arg then return i; else return u;
  }
  var x = testProcZero(false);
  writeln("testProcZero: when false uint(16) uint(32) then ", x.type:string);
}

{
  proc testProcCastMax(arg: bool) {
    if arg then return max(uint(16)):uint(16); else return max(uint(32)):uint(32);
  }
  var x = testProcCastMax(false);
  writeln("testProcCastMax: when false uint(16) uint(32) then ", x.type:string);
}

{
  proc testProcMax(arg: bool) {
    var i: uint(16) = max(uint(16));
    var u: uint(32) = max(uint(32));
    if arg then return i; else return u;
  }
  var x = testProcMax(false);
  writeln("testProcMax: when false uint(16) uint(32) then ", x.type:string);
}

{
  proc testParamProcCastZero(param arg: bool) param {
    if arg then return 0:uint(16); else return 0:uint(32);
  }
  param x = testParamProcCastZero(false);
  writeln("testParamProcCastZero: when false uint(16) uint(32) then ", x.type:string);
}

{
  proc testParamProcZero(param arg: bool) param {
    param i: uint(16) = 0;
    param u: uint(32) = 0;
    if arg then return i; else return u;
  }
  param x = testParamProcZero(false);
  writeln("testParamProcZero: when false uint(16) uint(32) then ", x.type:string);
}

{
  proc testParamProcCastMax(param arg: bool) param {
    if arg then return max(uint(16)):uint(16); else return max(uint(32)):uint(32);
  }
  param x = testParamProcCastMax(false);
  writeln("testParamProcCastMax: when false uint(16) uint(32) then ", x.type:string);
}

{
  proc testParamProcMax(param arg: bool) param {
    param i: uint(16) = max(uint(16));
    param u: uint(32) = max(uint(32));
    if arg then return i; else return u;
  }
  param x = testParamProcMax(false);
  writeln("testParamProcMax: when false uint(16) uint(32) then ", x.type:string);
}

{
  var b: bool = false;
  var x = if b then 0:uint(16) else 0:uint(32);
  writeln("testExpressionZero: when false uint(16) uint(32) then ", x.type:string);
}

{
  var b: bool = false;
  var x = if b then max(uint(16)):uint(16) else max(uint(32)):uint(32);
  writeln("testExpressionMax: when false uint(16) uint(32) then ", x.type:string);
}

{
  param x = if false then 0:uint(16) else 0:uint(32);
  writeln("testParamExpressionZero: when false uint(16) uint(32) then ", x.type:string);
}

{
  param x = if false then max(uint(16)):uint(16) else max(uint(32)):uint(32);
  writeln("testParamExpressionMax: when false uint(16) uint(32) then ", x.type:string);
}

{
  proc testProcCastZero(arg: bool) {
    if arg then return 0:uint(16); else return 0:uint(64);
  }
  var x = testProcCastZero(false);
  writeln("testProcCastZero: when false uint(16) uint(64) then ", x.type:string);
}

{
  proc testProcZero(arg: bool) {
    var i: uint(16) = 0;
    var u: uint(64) = 0;
    if arg then return i; else return u;
  }
  var x = testProcZero(false);
  writeln("testProcZero: when false uint(16) uint(64) then ", x.type:string);
}

{
  proc testProcCastMax(arg: bool) {
    if arg then return max(uint(16)):uint(16); else return max(uint(64)):uint(64);
  }
  var x = testProcCastMax(false);
  writeln("testProcCastMax: when false uint(16) uint(64) then ", x.type:string);
}

{
  proc testProcMax(arg: bool) {
    var i: uint(16) = max(uint(16));
    var u: uint(64) = max(uint(64));
    if arg then return i; else return u;
  }
  var x = testProcMax(false);
  writeln("testProcMax: when false uint(16) uint(64) then ", x.type:string);
}

{
  proc testParamProcCastZero(param arg: bool) param {
    if arg then return 0:uint(16); else return 0:uint(64);
  }
  param x = testParamProcCastZero(false);
  writeln("testParamProcCastZero: when false uint(16) uint(64) then ", x.type:string);
}

{
  proc testParamProcZero(param arg: bool) param {
    param i: uint(16) = 0;
    param u: uint(64) = 0;
    if arg then return i; else return u;
  }
  param x = testParamProcZero(false);
  writeln("testParamProcZero: when false uint(16) uint(64) then ", x.type:string);
}

{
  proc testParamProcCastMax(param arg: bool) param {
    if arg then return max(uint(16)):uint(16); else return max(uint(64)):uint(64);
  }
  param x = testParamProcCastMax(false);
  writeln("testParamProcCastMax: when false uint(16) uint(64) then ", x.type:string);
}

{
  proc testParamProcMax(param arg: bool) param {
    param i: uint(16) = max(uint(16));
    param u: uint(64) = max(uint(64));
    if arg then return i; else return u;
  }
  param x = testParamProcMax(false);
  writeln("testParamProcMax: when false uint(16) uint(64) then ", x.type:string);
}

{
  var b: bool = false;
  var x = if b then 0:uint(16) else 0:uint(64);
  writeln("testExpressionZero: when false uint(16) uint(64) then ", x.type:string);
}

{
  var b: bool = false;
  var x = if b then max(uint(16)):uint(16) else max(uint(64)):uint(64);
  writeln("testExpressionMax: when false uint(16) uint(64) then ", x.type:string);
}

{
  param x = if false then 0:uint(16) else 0:uint(64);
  writeln("testParamExpressionZero: when false uint(16) uint(64) then ", x.type:string);
}

{
  param x = if false then max(uint(16)):uint(16) else max(uint(64)):uint(64);
  writeln("testParamExpressionMax: when false uint(16) uint(64) then ", x.type:string);
}

{
  proc testProcCastZero(arg: bool) {
    if arg then return 0:uint(32); else return 0:int(8);
  }
  var x = testProcCastZero(false);
  writeln("testProcCastZero: when false uint(32) int(8) then ", x.type:string);
}

{
  proc testProcZero(arg: bool) {
    var i: uint(32) = 0;
    var u: int(8) = 0;
    if arg then return i; else return u;
  }
  var x = testProcZero(false);
  writeln("testProcZero: when false uint(32) int(8) then ", x.type:string);
}

{
  proc testProcCastMax(arg: bool) {
    if arg then return max(uint(32)):uint(32); else return max(int(8)):int(8);
  }
  var x = testProcCastMax(false);
  writeln("testProcCastMax: when false uint(32) int(8) then ", x.type:string);
}

{
  proc testProcMax(arg: bool) {
    var i: uint(32) = max(uint(32));
    var u: int(8) = max(int(8));
    if arg then return i; else return u;
  }
  var x = testProcMax(false);
  writeln("testProcMax: when false uint(32) int(8) then ", x.type:string);
}

{
  proc testParamProcCastZero(param arg: bool) param {
    if arg then return 0:uint(32); else return 0:int(8);
  }
  param x = testParamProcCastZero(false);
  writeln("testParamProcCastZero: when false uint(32) int(8) then ", x.type:string);
}

{
  proc testParamProcZero(param arg: bool) param {
    param i: uint(32) = 0;
    param u: int(8) = 0;
    if arg then return i; else return u;
  }
  param x = testParamProcZero(false);
  writeln("testParamProcZero: when false uint(32) int(8) then ", x.type:string);
}

{
  proc testParamProcCastMax(param arg: bool) param {
    if arg then return max(uint(32)):uint(32); else return max(int(8)):int(8);
  }
  param x = testParamProcCastMax(false);
  writeln("testParamProcCastMax: when false uint(32) int(8) then ", x.type:string);
}

{
  proc testParamProcMax(param arg: bool) param {
    param i: uint(32) = max(uint(32));
    param u: int(8) = max(int(8));
    if arg then return i; else return u;
  }
  param x = testParamProcMax(false);
  writeln("testParamProcMax: when false uint(32) int(8) then ", x.type:string);
}

{
  var b: bool = false;
  var x = if b then 0:uint(32) else 0:int(8);
  writeln("testExpressionZero: when false uint(32) int(8) then ", x.type:string);
}

{
  var b: bool = false;
  var x = if b then max(uint(32)):uint(32) else max(int(8)):int(8);
  writeln("testExpressionMax: when false uint(32) int(8) then ", x.type:string);
}

{
  param x = if false then 0:uint(32) else 0:int(8);
  writeln("testParamExpressionZero: when false uint(32) int(8) then ", x.type:string);
}

{
  param x = if false then max(uint(32)):uint(32) else max(int(8)):int(8);
  writeln("testParamExpressionMax: when false uint(32) int(8) then ", x.type:string);
}

{
  proc testProcCastZero(arg: bool) {
    if arg then return 0:uint(32); else return 0:int(16);
  }
  var x = testProcCastZero(false);
  writeln("testProcCastZero: when false uint(32) int(16) then ", x.type:string);
}

{
  proc testProcZero(arg: bool) {
    var i: uint(32) = 0;
    var u: int(16) = 0;
    if arg then return i; else return u;
  }
  var x = testProcZero(false);
  writeln("testProcZero: when false uint(32) int(16) then ", x.type:string);
}

{
  proc testProcCastMax(arg: bool) {
    if arg then return max(uint(32)):uint(32); else return max(int(16)):int(16);
  }
  var x = testProcCastMax(false);
  writeln("testProcCastMax: when false uint(32) int(16) then ", x.type:string);
}

{
  proc testProcMax(arg: bool) {
    var i: uint(32) = max(uint(32));
    var u: int(16) = max(int(16));
    if arg then return i; else return u;
  }
  var x = testProcMax(false);
  writeln("testProcMax: when false uint(32) int(16) then ", x.type:string);
}

{
  proc testParamProcCastZero(param arg: bool) param {
    if arg then return 0:uint(32); else return 0:int(16);
  }
  param x = testParamProcCastZero(false);
  writeln("testParamProcCastZero: when false uint(32) int(16) then ", x.type:string);
}

{
  proc testParamProcZero(param arg: bool) param {
    param i: uint(32) = 0;
    param u: int(16) = 0;
    if arg then return i; else return u;
  }
  param x = testParamProcZero(false);
  writeln("testParamProcZero: when false uint(32) int(16) then ", x.type:string);
}

{
  proc testParamProcCastMax(param arg: bool) param {
    if arg then return max(uint(32)):uint(32); else return max(int(16)):int(16);
  }
  param x = testParamProcCastMax(false);
  writeln("testParamProcCastMax: when false uint(32) int(16) then ", x.type:string);
}

{
  proc testParamProcMax(param arg: bool) param {
    param i: uint(32) = max(uint(32));
    param u: int(16) = max(int(16));
    if arg then return i; else return u;
  }
  param x = testParamProcMax(false);
  writeln("testParamProcMax: when false uint(32) int(16) then ", x.type:string);
}

{
  var b: bool = false;
  var x = if b then 0:uint(32) else 0:int(16);
  writeln("testExpressionZero: when false uint(32) int(16) then ", x.type:string);
}

{
  var b: bool = false;
  var x = if b then max(uint(32)):uint(32) else max(int(16)):int(16);
  writeln("testExpressionMax: when false uint(32) int(16) then ", x.type:string);
}

{
  param x = if false then 0:uint(32) else 0:int(16);
  writeln("testParamExpressionZero: when false uint(32) int(16) then ", x.type:string);
}

{
  param x = if false then max(uint(32)):uint(32) else max(int(16)):int(16);
  writeln("testParamExpressionMax: when false uint(32) int(16) then ", x.type:string);
}

{
  proc testProcCastZero(arg: bool) {
    if arg then return 0:uint(32); else return 0:int(32);
  }
  var x = testProcCastZero(false);
  writeln("testProcCastZero: when false uint(32) int(32) then ", x.type:string);
}

{
  proc testProcZero(arg: bool) {
    var i: uint(32) = 0;
    var u: int(32) = 0;
    if arg then return i; else return u;
  }
  var x = testProcZero(false);
  writeln("testProcZero: when false uint(32) int(32) then ", x.type:string);
}

{
  proc testProcCastMax(arg: bool) {
    if arg then return max(uint(32)):uint(32); else return max(int(32)):int(32);
  }
  var x = testProcCastMax(false);
  writeln("testProcCastMax: when false uint(32) int(32) then ", x.type:string);
}

{
  proc testProcMax(arg: bool) {
    var i: uint(32) = max(uint(32));
    var u: int(32) = max(int(32));
    if arg then return i; else return u;
  }
  var x = testProcMax(false);
  writeln("testProcMax: when false uint(32) int(32) then ", x.type:string);
}

{
  proc testParamProcCastZero(param arg: bool) param {
    if arg then return 0:uint(32); else return 0:int(32);
  }
  param x = testParamProcCastZero(false);
  writeln("testParamProcCastZero: when false uint(32) int(32) then ", x.type:string);
}

{
  proc testParamProcZero(param arg: bool) param {
    param i: uint(32) = 0;
    param u: int(32) = 0;
    if arg then return i; else return u;
  }
  param x = testParamProcZero(false);
  writeln("testParamProcZero: when false uint(32) int(32) then ", x.type:string);
}

{
  proc testParamProcCastMax(param arg: bool) param {
    if arg then return max(uint(32)):uint(32); else return max(int(32)):int(32);
  }
  param x = testParamProcCastMax(false);
  writeln("testParamProcCastMax: when false uint(32) int(32) then ", x.type:string);
}

{
  proc testParamProcMax(param arg: bool) param {
    param i: uint(32) = max(uint(32));
    param u: int(32) = max(int(32));
    if arg then return i; else return u;
  }
  param x = testParamProcMax(false);
  writeln("testParamProcMax: when false uint(32) int(32) then ", x.type:string);
}

{
  var b: bool = false;
  var x = if b then 0:uint(32) else 0:int(32);
  writeln("testExpressionZero: when false uint(32) int(32) then ", x.type:string);
}

{
  var b: bool = false;
  var x = if b then max(uint(32)):uint(32) else max(int(32)):int(32);
  writeln("testExpressionMax: when false uint(32) int(32) then ", x.type:string);
}

{
  param x = if false then 0:uint(32) else 0:int(32);
  writeln("testParamExpressionZero: when false uint(32) int(32) then ", x.type:string);
}

{
  param x = if false then max(uint(32)):uint(32) else max(int(32)):int(32);
  writeln("testParamExpressionMax: when false uint(32) int(32) then ", x.type:string);
}

{
  proc testProcCastZero(arg: bool) {
    if arg then return 0:uint(32); else return 0:int(64);
  }
  var x = testProcCastZero(false);
  writeln("testProcCastZero: when false uint(32) int(64) then ", x.type:string);
}

{
  proc testProcZero(arg: bool) {
    var i: uint(32) = 0;
    var u: int(64) = 0;
    if arg then return i; else return u;
  }
  var x = testProcZero(false);
  writeln("testProcZero: when false uint(32) int(64) then ", x.type:string);
}

{
  proc testProcCastMax(arg: bool) {
    if arg then return max(uint(32)):uint(32); else return max(int(64)):int(64);
  }
  var x = testProcCastMax(false);
  writeln("testProcCastMax: when false uint(32) int(64) then ", x.type:string);
}

{
  proc testProcMax(arg: bool) {
    var i: uint(32) = max(uint(32));
    var u: int(64) = max(int(64));
    if arg then return i; else return u;
  }
  var x = testProcMax(false);
  writeln("testProcMax: when false uint(32) int(64) then ", x.type:string);
}

{
  proc testParamProcCastZero(param arg: bool) param {
    if arg then return 0:uint(32); else return 0:int(64);
  }
  param x = testParamProcCastZero(false);
  writeln("testParamProcCastZero: when false uint(32) int(64) then ", x.type:string);
}

{
  proc testParamProcZero(param arg: bool) param {
    param i: uint(32) = 0;
    param u: int(64) = 0;
    if arg then return i; else return u;
  }
  param x = testParamProcZero(false);
  writeln("testParamProcZero: when false uint(32) int(64) then ", x.type:string);
}

{
  proc testParamProcCastMax(param arg: bool) param {
    if arg then return max(uint(32)):uint(32); else return max(int(64)):int(64);
  }
  param x = testParamProcCastMax(false);
  writeln("testParamProcCastMax: when false uint(32) int(64) then ", x.type:string);
}

{
  proc testParamProcMax(param arg: bool) param {
    param i: uint(32) = max(uint(32));
    param u: int(64) = max(int(64));
    if arg then return i; else return u;
  }
  param x = testParamProcMax(false);
  writeln("testParamProcMax: when false uint(32) int(64) then ", x.type:string);
}

{
  var b: bool = false;
  var x = if b then 0:uint(32) else 0:int(64);
  writeln("testExpressionZero: when false uint(32) int(64) then ", x.type:string);
}

{
  var b: bool = false;
  var x = if b then max(uint(32)):uint(32) else max(int(64)):int(64);
  writeln("testExpressionMax: when false uint(32) int(64) then ", x.type:string);
}

{
  param x = if false then 0:uint(32) else 0:int(64);
  writeln("testParamExpressionZero: when false uint(32) int(64) then ", x.type:string);
}

{
  param x = if false then max(uint(32)):uint(32) else max(int(64)):int(64);
  writeln("testParamExpressionMax: when false uint(32) int(64) then ", x.type:string);
}

{
  proc testProcCastZero(arg: bool) {
    if arg then return 0:uint(32); else return 0:uint(8);
  }
  var x = testProcCastZero(false);
  writeln("testProcCastZero: when false uint(32) uint(8) then ", x.type:string);
}

{
  proc testProcZero(arg: bool) {
    var i: uint(32) = 0;
    var u: uint(8) = 0;
    if arg then return i; else return u;
  }
  var x = testProcZero(false);
  writeln("testProcZero: when false uint(32) uint(8) then ", x.type:string);
}

{
  proc testProcCastMax(arg: bool) {
    if arg then return max(uint(32)):uint(32); else return max(uint(8)):uint(8);
  }
  var x = testProcCastMax(false);
  writeln("testProcCastMax: when false uint(32) uint(8) then ", x.type:string);
}

{
  proc testProcMax(arg: bool) {
    var i: uint(32) = max(uint(32));
    var u: uint(8) = max(uint(8));
    if arg then return i; else return u;
  }
  var x = testProcMax(false);
  writeln("testProcMax: when false uint(32) uint(8) then ", x.type:string);
}

{
  proc testParamProcCastZero(param arg: bool) param {
    if arg then return 0:uint(32); else return 0:uint(8);
  }
  param x = testParamProcCastZero(false);
  writeln("testParamProcCastZero: when false uint(32) uint(8) then ", x.type:string);
}

{
  proc testParamProcZero(param arg: bool) param {
    param i: uint(32) = 0;
    param u: uint(8) = 0;
    if arg then return i; else return u;
  }
  param x = testParamProcZero(false);
  writeln("testParamProcZero: when false uint(32) uint(8) then ", x.type:string);
}

{
  proc testParamProcCastMax(param arg: bool) param {
    if arg then return max(uint(32)):uint(32); else return max(uint(8)):uint(8);
  }
  param x = testParamProcCastMax(false);
  writeln("testParamProcCastMax: when false uint(32) uint(8) then ", x.type:string);
}

{
  proc testParamProcMax(param arg: bool) param {
    param i: uint(32) = max(uint(32));
    param u: uint(8) = max(uint(8));
    if arg then return i; else return u;
  }
  param x = testParamProcMax(false);
  writeln("testParamProcMax: when false uint(32) uint(8) then ", x.type:string);
}

{
  var b: bool = false;
  var x = if b then 0:uint(32) else 0:uint(8);
  writeln("testExpressionZero: when false uint(32) uint(8) then ", x.type:string);
}

{
  var b: bool = false;
  var x = if b then max(uint(32)):uint(32) else max(uint(8)):uint(8);
  writeln("testExpressionMax: when false uint(32) uint(8) then ", x.type:string);
}

{
  param x = if false then 0:uint(32) else 0:uint(8);
  writeln("testParamExpressionZero: when false uint(32) uint(8) then ", x.type:string);
}

{
  param x = if false then max(uint(32)):uint(32) else max(uint(8)):uint(8);
  writeln("testParamExpressionMax: when false uint(32) uint(8) then ", x.type:string);
}

{
  proc testProcCastZero(arg: bool) {
    if arg then return 0:uint(32); else return 0:uint(16);
  }
  var x = testProcCastZero(false);
  writeln("testProcCastZero: when false uint(32) uint(16) then ", x.type:string);
}

{
  proc testProcZero(arg: bool) {
    var i: uint(32) = 0;
    var u: uint(16) = 0;
    if arg then return i; else return u;
  }
  var x = testProcZero(false);
  writeln("testProcZero: when false uint(32) uint(16) then ", x.type:string);
}

{
  proc testProcCastMax(arg: bool) {
    if arg then return max(uint(32)):uint(32); else return max(uint(16)):uint(16);
  }
  var x = testProcCastMax(false);
  writeln("testProcCastMax: when false uint(32) uint(16) then ", x.type:string);
}

{
  proc testProcMax(arg: bool) {
    var i: uint(32) = max(uint(32));
    var u: uint(16) = max(uint(16));
    if arg then return i; else return u;
  }
  var x = testProcMax(false);
  writeln("testProcMax: when false uint(32) uint(16) then ", x.type:string);
}

{
  proc testParamProcCastZero(param arg: bool) param {
    if arg then return 0:uint(32); else return 0:uint(16);
  }
  param x = testParamProcCastZero(false);
  writeln("testParamProcCastZero: when false uint(32) uint(16) then ", x.type:string);
}

{
  proc testParamProcZero(param arg: bool) param {
    param i: uint(32) = 0;
    param u: uint(16) = 0;
    if arg then return i; else return u;
  }
  param x = testParamProcZero(false);
  writeln("testParamProcZero: when false uint(32) uint(16) then ", x.type:string);
}

{
  proc testParamProcCastMax(param arg: bool) param {
    if arg then return max(uint(32)):uint(32); else return max(uint(16)):uint(16);
  }
  param x = testParamProcCastMax(false);
  writeln("testParamProcCastMax: when false uint(32) uint(16) then ", x.type:string);
}

{
  proc testParamProcMax(param arg: bool) param {
    param i: uint(32) = max(uint(32));
    param u: uint(16) = max(uint(16));
    if arg then return i; else return u;
  }
  param x = testParamProcMax(false);
  writeln("testParamProcMax: when false uint(32) uint(16) then ", x.type:string);
}

{
  var b: bool = false;
  var x = if b then 0:uint(32) else 0:uint(16);
  writeln("testExpressionZero: when false uint(32) uint(16) then ", x.type:string);
}

{
  var b: bool = false;
  var x = if b then max(uint(32)):uint(32) else max(uint(16)):uint(16);
  writeln("testExpressionMax: when false uint(32) uint(16) then ", x.type:string);
}

{
  param x = if false then 0:uint(32) else 0:uint(16);
  writeln("testParamExpressionZero: when false uint(32) uint(16) then ", x.type:string);
}

{
  param x = if false then max(uint(32)):uint(32) else max(uint(16)):uint(16);
  writeln("testParamExpressionMax: when false uint(32) uint(16) then ", x.type:string);
}

{
  proc testProcCastZero(arg: bool) {
    if arg then return 0:uint(32); else return 0:uint(32);
  }
  var x = testProcCastZero(false);
  writeln("testProcCastZero: when false uint(32) uint(32) then ", x.type:string);
}

{
  proc testProcZero(arg: bool) {
    var i: uint(32) = 0;
    var u: uint(32) = 0;
    if arg then return i; else return u;
  }
  var x = testProcZero(false);
  writeln("testProcZero: when false uint(32) uint(32) then ", x.type:string);
}

{
  proc testProcCastMax(arg: bool) {
    if arg then return max(uint(32)):uint(32); else return max(uint(32)):uint(32);
  }
  var x = testProcCastMax(false);
  writeln("testProcCastMax: when false uint(32) uint(32) then ", x.type:string);
}

{
  proc testProcMax(arg: bool) {
    var i: uint(32) = max(uint(32));
    var u: uint(32) = max(uint(32));
    if arg then return i; else return u;
  }
  var x = testProcMax(false);
  writeln("testProcMax: when false uint(32) uint(32) then ", x.type:string);
}

{
  proc testParamProcCastZero(param arg: bool) param {
    if arg then return 0:uint(32); else return 0:uint(32);
  }
  param x = testParamProcCastZero(false);
  writeln("testParamProcCastZero: when false uint(32) uint(32) then ", x.type:string);
}

{
  proc testParamProcZero(param arg: bool) param {
    param i: uint(32) = 0;
    param u: uint(32) = 0;
    if arg then return i; else return u;
  }
  param x = testParamProcZero(false);
  writeln("testParamProcZero: when false uint(32) uint(32) then ", x.type:string);
}

{
  proc testParamProcCastMax(param arg: bool) param {
    if arg then return max(uint(32)):uint(32); else return max(uint(32)):uint(32);
  }
  param x = testParamProcCastMax(false);
  writeln("testParamProcCastMax: when false uint(32) uint(32) then ", x.type:string);
}

{
  proc testParamProcMax(param arg: bool) param {
    param i: uint(32) = max(uint(32));
    param u: uint(32) = max(uint(32));
    if arg then return i; else return u;
  }
  param x = testParamProcMax(false);
  writeln("testParamProcMax: when false uint(32) uint(32) then ", x.type:string);
}

{
  var b: bool = false;
  var x = if b then 0:uint(32) else 0:uint(32);
  writeln("testExpressionZero: when false uint(32) uint(32) then ", x.type:string);
}

{
  var b: bool = false;
  var x = if b then max(uint(32)):uint(32) else max(uint(32)):uint(32);
  writeln("testExpressionMax: when false uint(32) uint(32) then ", x.type:string);
}

{
  param x = if false then 0:uint(32) else 0:uint(32);
  writeln("testParamExpressionZero: when false uint(32) uint(32) then ", x.type:string);
}

{
  param x = if false then max(uint(32)):uint(32) else max(uint(32)):uint(32);
  writeln("testParamExpressionMax: when false uint(32) uint(32) then ", x.type:string);
}

{
  proc testProcCastZero(arg: bool) {
    if arg then return 0:uint(32); else return 0:uint(64);
  }
  var x = testProcCastZero(false);
  writeln("testProcCastZero: when false uint(32) uint(64) then ", x.type:string);
}

{
  proc testProcZero(arg: bool) {
    var i: uint(32) = 0;
    var u: uint(64) = 0;
    if arg then return i; else return u;
  }
  var x = testProcZero(false);
  writeln("testProcZero: when false uint(32) uint(64) then ", x.type:string);
}

{
  proc testProcCastMax(arg: bool) {
    if arg then return max(uint(32)):uint(32); else return max(uint(64)):uint(64);
  }
  var x = testProcCastMax(false);
  writeln("testProcCastMax: when false uint(32) uint(64) then ", x.type:string);
}

{
  proc testProcMax(arg: bool) {
    var i: uint(32) = max(uint(32));
    var u: uint(64) = max(uint(64));
    if arg then return i; else return u;
  }
  var x = testProcMax(false);
  writeln("testProcMax: when false uint(32) uint(64) then ", x.type:string);
}

{
  proc testParamProcCastZero(param arg: bool) param {
    if arg then return 0:uint(32); else return 0:uint(64);
  }
  param x = testParamProcCastZero(false);
  writeln("testParamProcCastZero: when false uint(32) uint(64) then ", x.type:string);
}

{
  proc testParamProcZero(param arg: bool) param {
    param i: uint(32) = 0;
    param u: uint(64) = 0;
    if arg then return i; else return u;
  }
  param x = testParamProcZero(false);
  writeln("testParamProcZero: when false uint(32) uint(64) then ", x.type:string);
}

{
  proc testParamProcCastMax(param arg: bool) param {
    if arg then return max(uint(32)):uint(32); else return max(uint(64)):uint(64);
  }
  param x = testParamProcCastMax(false);
  writeln("testParamProcCastMax: when false uint(32) uint(64) then ", x.type:string);
}

{
  proc testParamProcMax(param arg: bool) param {
    param i: uint(32) = max(uint(32));
    param u: uint(64) = max(uint(64));
    if arg then return i; else return u;
  }
  param x = testParamProcMax(false);
  writeln("testParamProcMax: when false uint(32) uint(64) then ", x.type:string);
}

{
  var b: bool = false;
  var x = if b then 0:uint(32) else 0:uint(64);
  writeln("testExpressionZero: when false uint(32) uint(64) then ", x.type:string);
}

{
  var b: bool = false;
  var x = if b then max(uint(32)):uint(32) else max(uint(64)):uint(64);
  writeln("testExpressionMax: when false uint(32) uint(64) then ", x.type:string);
}

{
  param x = if false then 0:uint(32) else 0:uint(64);
  writeln("testParamExpressionZero: when false uint(32) uint(64) then ", x.type:string);
}

{
  param x = if false then max(uint(32)):uint(32) else max(uint(64)):uint(64);
  writeln("testParamExpressionMax: when false uint(32) uint(64) then ", x.type:string);
}

{
  proc testProcCastZero(arg: bool) {
    if arg then return 0:uint(64); else return 0:int(8);
  }
  var x = testProcCastZero(false);
  writeln("testProcCastZero: when false uint(64) int(8) then ", x.type:string);
}

{
  proc testProcZero(arg: bool) {
    var i: uint(64) = 0;
    var u: int(8) = 0;
    if arg then return i; else return u;
  }
  var x = testProcZero(false);
  writeln("testProcZero: when false uint(64) int(8) then ", x.type:string);
}

{
  proc testProcCastMax(arg: bool) {
    if arg then return max(uint(64)):uint(64); else return max(int(8)):int(8);
  }
  var x = testProcCastMax(false);
  writeln("testProcCastMax: when false uint(64) int(8) then ", x.type:string);
}

{
  proc testProcMax(arg: bool) {
    var i: uint(64) = max(uint(64));
    var u: int(8) = max(int(8));
    if arg then return i; else return u;
  }
  var x = testProcMax(false);
  writeln("testProcMax: when false uint(64) int(8) then ", x.type:string);
}

{
  proc testParamProcCastZero(param arg: bool) param {
    if arg then return 0:uint(64); else return 0:int(8);
  }
  param x = testParamProcCastZero(false);
  writeln("testParamProcCastZero: when false uint(64) int(8) then ", x.type:string);
}

{
  proc testParamProcZero(param arg: bool) param {
    param i: uint(64) = 0;
    param u: int(8) = 0;
    if arg then return i; else return u;
  }
  param x = testParamProcZero(false);
  writeln("testParamProcZero: when false uint(64) int(8) then ", x.type:string);
}

{
  proc testParamProcCastMax(param arg: bool) param {
    if arg then return max(uint(64)):uint(64); else return max(int(8)):int(8);
  }
  param x = testParamProcCastMax(false);
  writeln("testParamProcCastMax: when false uint(64) int(8) then ", x.type:string);
}

{
  proc testParamProcMax(param arg: bool) param {
    param i: uint(64) = max(uint(64));
    param u: int(8) = max(int(8));
    if arg then return i; else return u;
  }
  param x = testParamProcMax(false);
  writeln("testParamProcMax: when false uint(64) int(8) then ", x.type:string);
}

{
  var b: bool = false;
  var x = if b then 0:uint(64) else 0:int(8);
  writeln("testExpressionZero: when false uint(64) int(8) then ", x.type:string);
}

{
  var b: bool = false;
  var x = if b then max(uint(64)):uint(64) else max(int(8)):int(8);
  writeln("testExpressionMax: when false uint(64) int(8) then ", x.type:string);
}

{
  param x = if false then 0:uint(64) else 0:int(8);
  writeln("testParamExpressionZero: when false uint(64) int(8) then ", x.type:string);
}

{
  param x = if false then max(uint(64)):uint(64) else max(int(8)):int(8);
  writeln("testParamExpressionMax: when false uint(64) int(8) then ", x.type:string);
}

{
  proc testProcCastZero(arg: bool) {
    if arg then return 0:uint(64); else return 0:int(16);
  }
  var x = testProcCastZero(false);
  writeln("testProcCastZero: when false uint(64) int(16) then ", x.type:string);
}

{
  proc testProcZero(arg: bool) {
    var i: uint(64) = 0;
    var u: int(16) = 0;
    if arg then return i; else return u;
  }
  var x = testProcZero(false);
  writeln("testProcZero: when false uint(64) int(16) then ", x.type:string);
}

{
  proc testProcCastMax(arg: bool) {
    if arg then return max(uint(64)):uint(64); else return max(int(16)):int(16);
  }
  var x = testProcCastMax(false);
  writeln("testProcCastMax: when false uint(64) int(16) then ", x.type:string);
}

{
  proc testProcMax(arg: bool) {
    var i: uint(64) = max(uint(64));
    var u: int(16) = max(int(16));
    if arg then return i; else return u;
  }
  var x = testProcMax(false);
  writeln("testProcMax: when false uint(64) int(16) then ", x.type:string);
}

{
  proc testParamProcCastZero(param arg: bool) param {
    if arg then return 0:uint(64); else return 0:int(16);
  }
  param x = testParamProcCastZero(false);
  writeln("testParamProcCastZero: when false uint(64) int(16) then ", x.type:string);
}

{
  proc testParamProcZero(param arg: bool) param {
    param i: uint(64) = 0;
    param u: int(16) = 0;
    if arg then return i; else return u;
  }
  param x = testParamProcZero(false);
  writeln("testParamProcZero: when false uint(64) int(16) then ", x.type:string);
}

{
  proc testParamProcCastMax(param arg: bool) param {
    if arg then return max(uint(64)):uint(64); else return max(int(16)):int(16);
  }
  param x = testParamProcCastMax(false);
  writeln("testParamProcCastMax: when false uint(64) int(16) then ", x.type:string);
}

{
  proc testParamProcMax(param arg: bool) param {
    param i: uint(64) = max(uint(64));
    param u: int(16) = max(int(16));
    if arg then return i; else return u;
  }
  param x = testParamProcMax(false);
  writeln("testParamProcMax: when false uint(64) int(16) then ", x.type:string);
}

{
  var b: bool = false;
  var x = if b then 0:uint(64) else 0:int(16);
  writeln("testExpressionZero: when false uint(64) int(16) then ", x.type:string);
}

{
  var b: bool = false;
  var x = if b then max(uint(64)):uint(64) else max(int(16)):int(16);
  writeln("testExpressionMax: when false uint(64) int(16) then ", x.type:string);
}

{
  param x = if false then 0:uint(64) else 0:int(16);
  writeln("testParamExpressionZero: when false uint(64) int(16) then ", x.type:string);
}

{
  param x = if false then max(uint(64)):uint(64) else max(int(16)):int(16);
  writeln("testParamExpressionMax: when false uint(64) int(16) then ", x.type:string);
}

{
  proc testProcCastZero(arg: bool) {
    if arg then return 0:uint(64); else return 0:int(32);
  }
  var x = testProcCastZero(false);
  writeln("testProcCastZero: when false uint(64) int(32) then ", x.type:string);
}

{
  proc testProcZero(arg: bool) {
    var i: uint(64) = 0;
    var u: int(32) = 0;
    if arg then return i; else return u;
  }
  var x = testProcZero(false);
  writeln("testProcZero: when false uint(64) int(32) then ", x.type:string);
}

{
  proc testProcCastMax(arg: bool) {
    if arg then return max(uint(64)):uint(64); else return max(int(32)):int(32);
  }
  var x = testProcCastMax(false);
  writeln("testProcCastMax: when false uint(64) int(32) then ", x.type:string);
}

{
  proc testProcMax(arg: bool) {
    var i: uint(64) = max(uint(64));
    var u: int(32) = max(int(32));
    if arg then return i; else return u;
  }
  var x = testProcMax(false);
  writeln("testProcMax: when false uint(64) int(32) then ", x.type:string);
}

{
  proc testParamProcCastZero(param arg: bool) param {
    if arg then return 0:uint(64); else return 0:int(32);
  }
  param x = testParamProcCastZero(false);
  writeln("testParamProcCastZero: when false uint(64) int(32) then ", x.type:string);
}

{
  proc testParamProcZero(param arg: bool) param {
    param i: uint(64) = 0;
    param u: int(32) = 0;
    if arg then return i; else return u;
  }
  param x = testParamProcZero(false);
  writeln("testParamProcZero: when false uint(64) int(32) then ", x.type:string);
}

{
  proc testParamProcCastMax(param arg: bool) param {
    if arg then return max(uint(64)):uint(64); else return max(int(32)):int(32);
  }
  param x = testParamProcCastMax(false);
  writeln("testParamProcCastMax: when false uint(64) int(32) then ", x.type:string);
}

{
  proc testParamProcMax(param arg: bool) param {
    param i: uint(64) = max(uint(64));
    param u: int(32) = max(int(32));
    if arg then return i; else return u;
  }
  param x = testParamProcMax(false);
  writeln("testParamProcMax: when false uint(64) int(32) then ", x.type:string);
}

{
  var b: bool = false;
  var x = if b then 0:uint(64) else 0:int(32);
  writeln("testExpressionZero: when false uint(64) int(32) then ", x.type:string);
}

{
  var b: bool = false;
  var x = if b then max(uint(64)):uint(64) else max(int(32)):int(32);
  writeln("testExpressionMax: when false uint(64) int(32) then ", x.type:string);
}

{
  param x = if false then 0:uint(64) else 0:int(32);
  writeln("testParamExpressionZero: when false uint(64) int(32) then ", x.type:string);
}

{
  param x = if false then max(uint(64)):uint(64) else max(int(32)):int(32);
  writeln("testParamExpressionMax: when false uint(64) int(32) then ", x.type:string);
}

{
  proc testProcCastZero(arg: bool) {
    if arg then return 0:uint(64); else return 0:int(64);
  }
  var x = testProcCastZero(false);
  writeln("testProcCastZero: when false uint(64) int(64) then ", x.type:string);
}

{
  proc testProcZero(arg: bool) {
    var i: uint(64) = 0;
    var u: int(64) = 0;
    if arg then return i; else return u;
  }
  var x = testProcZero(false);
  writeln("testProcZero: when false uint(64) int(64) then ", x.type:string);
}

{
  proc testProcCastMax(arg: bool) {
    if arg then return max(uint(64)):uint(64); else return max(int(64)):int(64);
  }
  var x = testProcCastMax(false);
  writeln("testProcCastMax: when false uint(64) int(64) then ", x.type:string);
}

{
  proc testProcMax(arg: bool) {
    var i: uint(64) = max(uint(64));
    var u: int(64) = max(int(64));
    if arg then return i; else return u;
  }
  var x = testProcMax(false);
  writeln("testProcMax: when false uint(64) int(64) then ", x.type:string);
}

{
  proc testParamProcCastZero(param arg: bool) param {
    if arg then return 0:uint(64); else return 0:int(64);
  }
  param x = testParamProcCastZero(false);
  writeln("testParamProcCastZero: when false uint(64) int(64) then ", x.type:string);
}

{
  proc testParamProcZero(param arg: bool) param {
    param i: uint(64) = 0;
    param u: int(64) = 0;
    if arg then return i; else return u;
  }
  param x = testParamProcZero(false);
  writeln("testParamProcZero: when false uint(64) int(64) then ", x.type:string);
}

{
  proc testParamProcCastMax(param arg: bool) param {
    if arg then return max(uint(64)):uint(64); else return max(int(64)):int(64);
  }
  param x = testParamProcCastMax(false);
  writeln("testParamProcCastMax: when false uint(64) int(64) then ", x.type:string);
}

{
  proc testParamProcMax(param arg: bool) param {
    param i: uint(64) = max(uint(64));
    param u: int(64) = max(int(64));
    if arg then return i; else return u;
  }
  param x = testParamProcMax(false);
  writeln("testParamProcMax: when false uint(64) int(64) then ", x.type:string);
}

{
  var b: bool = false;
  var x = if b then 0:uint(64) else 0:int(64);
  writeln("testExpressionZero: when false uint(64) int(64) then ", x.type:string);
}

{
  var b: bool = false;
  var x = if b then max(uint(64)):uint(64) else max(int(64)):int(64);
  writeln("testExpressionMax: when false uint(64) int(64) then ", x.type:string);
}

{
  param x = if false then 0:uint(64) else 0:int(64);
  writeln("testParamExpressionZero: when false uint(64) int(64) then ", x.type:string);
}

{
  param x = if false then max(uint(64)):uint(64) else max(int(64)):int(64);
  writeln("testParamExpressionMax: when false uint(64) int(64) then ", x.type:string);
}

{
  proc testProcCastZero(arg: bool) {
    if arg then return 0:uint(64); else return 0:uint(8);
  }
  var x = testProcCastZero(false);
  writeln("testProcCastZero: when false uint(64) uint(8) then ", x.type:string);
}

{
  proc testProcZero(arg: bool) {
    var i: uint(64) = 0;
    var u: uint(8) = 0;
    if arg then return i; else return u;
  }
  var x = testProcZero(false);
  writeln("testProcZero: when false uint(64) uint(8) then ", x.type:string);
}

{
  proc testProcCastMax(arg: bool) {
    if arg then return max(uint(64)):uint(64); else return max(uint(8)):uint(8);
  }
  var x = testProcCastMax(false);
  writeln("testProcCastMax: when false uint(64) uint(8) then ", x.type:string);
}

{
  proc testProcMax(arg: bool) {
    var i: uint(64) = max(uint(64));
    var u: uint(8) = max(uint(8));
    if arg then return i; else return u;
  }
  var x = testProcMax(false);
  writeln("testProcMax: when false uint(64) uint(8) then ", x.type:string);
}

{
  proc testParamProcCastZero(param arg: bool) param {
    if arg then return 0:uint(64); else return 0:uint(8);
  }
  param x = testParamProcCastZero(false);
  writeln("testParamProcCastZero: when false uint(64) uint(8) then ", x.type:string);
}

{
  proc testParamProcZero(param arg: bool) param {
    param i: uint(64) = 0;
    param u: uint(8) = 0;
    if arg then return i; else return u;
  }
  param x = testParamProcZero(false);
  writeln("testParamProcZero: when false uint(64) uint(8) then ", x.type:string);
}

{
  proc testParamProcCastMax(param arg: bool) param {
    if arg then return max(uint(64)):uint(64); else return max(uint(8)):uint(8);
  }
  param x = testParamProcCastMax(false);
  writeln("testParamProcCastMax: when false uint(64) uint(8) then ", x.type:string);
}

{
  proc testParamProcMax(param arg: bool) param {
    param i: uint(64) = max(uint(64));
    param u: uint(8) = max(uint(8));
    if arg then return i; else return u;
  }
  param x = testParamProcMax(false);
  writeln("testParamProcMax: when false uint(64) uint(8) then ", x.type:string);
}

{
  var b: bool = false;
  var x = if b then 0:uint(64) else 0:uint(8);
  writeln("testExpressionZero: when false uint(64) uint(8) then ", x.type:string);
}

{
  var b: bool = false;
  var x = if b then max(uint(64)):uint(64) else max(uint(8)):uint(8);
  writeln("testExpressionMax: when false uint(64) uint(8) then ", x.type:string);
}

{
  param x = if false then 0:uint(64) else 0:uint(8);
  writeln("testParamExpressionZero: when false uint(64) uint(8) then ", x.type:string);
}

{
  param x = if false then max(uint(64)):uint(64) else max(uint(8)):uint(8);
  writeln("testParamExpressionMax: when false uint(64) uint(8) then ", x.type:string);
}

{
  proc testProcCastZero(arg: bool) {
    if arg then return 0:uint(64); else return 0:uint(16);
  }
  var x = testProcCastZero(false);
  writeln("testProcCastZero: when false uint(64) uint(16) then ", x.type:string);
}

{
  proc testProcZero(arg: bool) {
    var i: uint(64) = 0;
    var u: uint(16) = 0;
    if arg then return i; else return u;
  }
  var x = testProcZero(false);
  writeln("testProcZero: when false uint(64) uint(16) then ", x.type:string);
}

{
  proc testProcCastMax(arg: bool) {
    if arg then return max(uint(64)):uint(64); else return max(uint(16)):uint(16);
  }
  var x = testProcCastMax(false);
  writeln("testProcCastMax: when false uint(64) uint(16) then ", x.type:string);
}

{
  proc testProcMax(arg: bool) {
    var i: uint(64) = max(uint(64));
    var u: uint(16) = max(uint(16));
    if arg then return i; else return u;
  }
  var x = testProcMax(false);
  writeln("testProcMax: when false uint(64) uint(16) then ", x.type:string);
}

{
  proc testParamProcCastZero(param arg: bool) param {
    if arg then return 0:uint(64); else return 0:uint(16);
  }
  param x = testParamProcCastZero(false);
  writeln("testParamProcCastZero: when false uint(64) uint(16) then ", x.type:string);
}

{
  proc testParamProcZero(param arg: bool) param {
    param i: uint(64) = 0;
    param u: uint(16) = 0;
    if arg then return i; else return u;
  }
  param x = testParamProcZero(false);
  writeln("testParamProcZero: when false uint(64) uint(16) then ", x.type:string);
}

{
  proc testParamProcCastMax(param arg: bool) param {
    if arg then return max(uint(64)):uint(64); else return max(uint(16)):uint(16);
  }
  param x = testParamProcCastMax(false);
  writeln("testParamProcCastMax: when false uint(64) uint(16) then ", x.type:string);
}

{
  proc testParamProcMax(param arg: bool) param {
    param i: uint(64) = max(uint(64));
    param u: uint(16) = max(uint(16));
    if arg then return i; else return u;
  }
  param x = testParamProcMax(false);
  writeln("testParamProcMax: when false uint(64) uint(16) then ", x.type:string);
}

{
  var b: bool = false;
  var x = if b then 0:uint(64) else 0:uint(16);
  writeln("testExpressionZero: when false uint(64) uint(16) then ", x.type:string);
}

{
  var b: bool = false;
  var x = if b then max(uint(64)):uint(64) else max(uint(16)):uint(16);
  writeln("testExpressionMax: when false uint(64) uint(16) then ", x.type:string);
}

{
  param x = if false then 0:uint(64) else 0:uint(16);
  writeln("testParamExpressionZero: when false uint(64) uint(16) then ", x.type:string);
}

{
  param x = if false then max(uint(64)):uint(64) else max(uint(16)):uint(16);
  writeln("testParamExpressionMax: when false uint(64) uint(16) then ", x.type:string);
}

{
  proc testProcCastZero(arg: bool) {
    if arg then return 0:uint(64); else return 0:uint(32);
  }
  var x = testProcCastZero(false);
  writeln("testProcCastZero: when false uint(64) uint(32) then ", x.type:string);
}

{
  proc testProcZero(arg: bool) {
    var i: uint(64) = 0;
    var u: uint(32) = 0;
    if arg then return i; else return u;
  }
  var x = testProcZero(false);
  writeln("testProcZero: when false uint(64) uint(32) then ", x.type:string);
}

{
  proc testProcCastMax(arg: bool) {
    if arg then return max(uint(64)):uint(64); else return max(uint(32)):uint(32);
  }
  var x = testProcCastMax(false);
  writeln("testProcCastMax: when false uint(64) uint(32) then ", x.type:string);
}

{
  proc testProcMax(arg: bool) {
    var i: uint(64) = max(uint(64));
    var u: uint(32) = max(uint(32));
    if arg then return i; else return u;
  }
  var x = testProcMax(false);
  writeln("testProcMax: when false uint(64) uint(32) then ", x.type:string);
}

{
  proc testParamProcCastZero(param arg: bool) param {
    if arg then return 0:uint(64); else return 0:uint(32);
  }
  param x = testParamProcCastZero(false);
  writeln("testParamProcCastZero: when false uint(64) uint(32) then ", x.type:string);
}

{
  proc testParamProcZero(param arg: bool) param {
    param i: uint(64) = 0;
    param u: uint(32) = 0;
    if arg then return i; else return u;
  }
  param x = testParamProcZero(false);
  writeln("testParamProcZero: when false uint(64) uint(32) then ", x.type:string);
}

{
  proc testParamProcCastMax(param arg: bool) param {
    if arg then return max(uint(64)):uint(64); else return max(uint(32)):uint(32);
  }
  param x = testParamProcCastMax(false);
  writeln("testParamProcCastMax: when false uint(64) uint(32) then ", x.type:string);
}

{
  proc testParamProcMax(param arg: bool) param {
    param i: uint(64) = max(uint(64));
    param u: uint(32) = max(uint(32));
    if arg then return i; else return u;
  }
  param x = testParamProcMax(false);
  writeln("testParamProcMax: when false uint(64) uint(32) then ", x.type:string);
}

{
  var b: bool = false;
  var x = if b then 0:uint(64) else 0:uint(32);
  writeln("testExpressionZero: when false uint(64) uint(32) then ", x.type:string);
}

{
  var b: bool = false;
  var x = if b then max(uint(64)):uint(64) else max(uint(32)):uint(32);
  writeln("testExpressionMax: when false uint(64) uint(32) then ", x.type:string);
}

{
  param x = if false then 0:uint(64) else 0:uint(32);
  writeln("testParamExpressionZero: when false uint(64) uint(32) then ", x.type:string);
}

{
  param x = if false then max(uint(64)):uint(64) else max(uint(32)):uint(32);
  writeln("testParamExpressionMax: when false uint(64) uint(32) then ", x.type:string);
}

{
  proc testProcCastZero(arg: bool) {
    if arg then return 0:uint(64); else return 0:uint(64);
  }
  var x = testProcCastZero(false);
  writeln("testProcCastZero: when false uint(64) uint(64) then ", x.type:string);
}

{
  proc testProcZero(arg: bool) {
    var i: uint(64) = 0;
    var u: uint(64) = 0;
    if arg then return i; else return u;
  }
  var x = testProcZero(false);
  writeln("testProcZero: when false uint(64) uint(64) then ", x.type:string);
}

{
  proc testProcCastMax(arg: bool) {
    if arg then return max(uint(64)):uint(64); else return max(uint(64)):uint(64);
  }
  var x = testProcCastMax(false);
  writeln("testProcCastMax: when false uint(64) uint(64) then ", x.type:string);
}

{
  proc testProcMax(arg: bool) {
    var i: uint(64) = max(uint(64));
    var u: uint(64) = max(uint(64));
    if arg then return i; else return u;
  }
  var x = testProcMax(false);
  writeln("testProcMax: when false uint(64) uint(64) then ", x.type:string);
}

{
  proc testParamProcCastZero(param arg: bool) param {
    if arg then return 0:uint(64); else return 0:uint(64);
  }
  param x = testParamProcCastZero(false);
  writeln("testParamProcCastZero: when false uint(64) uint(64) then ", x.type:string);
}

{
  proc testParamProcZero(param arg: bool) param {
    param i: uint(64) = 0;
    param u: uint(64) = 0;
    if arg then return i; else return u;
  }
  param x = testParamProcZero(false);
  writeln("testParamProcZero: when false uint(64) uint(64) then ", x.type:string);
}

{
  proc testParamProcCastMax(param arg: bool) param {
    if arg then return max(uint(64)):uint(64); else return max(uint(64)):uint(64);
  }
  param x = testParamProcCastMax(false);
  writeln("testParamProcCastMax: when false uint(64) uint(64) then ", x.type:string);
}

{
  proc testParamProcMax(param arg: bool) param {
    param i: uint(64) = max(uint(64));
    param u: uint(64) = max(uint(64));
    if arg then return i; else return u;
  }
  param x = testParamProcMax(false);
  writeln("testParamProcMax: when false uint(64) uint(64) then ", x.type:string);
}

{
  var b: bool = false;
  var x = if b then 0:uint(64) else 0:uint(64);
  writeln("testExpressionZero: when false uint(64) uint(64) then ", x.type:string);
}

{
  var b: bool = false;
  var x = if b then max(uint(64)):uint(64) else max(uint(64)):uint(64);
  writeln("testExpressionMax: when false uint(64) uint(64) then ", x.type:string);
}

{
  param x = if false then 0:uint(64) else 0:uint(64);
  writeln("testParamExpressionZero: when false uint(64) uint(64) then ", x.type:string);
}

{
  param x = if false then max(uint(64)):uint(64) else max(uint(64)):uint(64);
  writeln("testParamExpressionMax: when false uint(64) uint(64) then ", x.type:string);
}
