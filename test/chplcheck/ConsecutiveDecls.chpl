module ConsDecls {
  var real1: real;
  var real2: real;

  const constReal1: real;
  const constReal2: real;

  const constReal3:  real;
  const constIntBetween:  int;
  const constReal4:  real;

  const cr1: real;
  var vr1: real;
  const cr2: real;

  const ref crr1: real = cr1;
  const between: real;
  const ref crr2: real = cr1;
  const ref ref1: real = cr1;
  const ref ref2: real = cr1;
  const ref ref3: real = cr1;

  //should not warn
  @chpldoc.nodoc
  var x, y = 42;

  @chpldoc.nodoc
  var x2 = 42;
  var y2 = x2;

  //should warn
  @chpldoc.nodoc
  var x3 = 42;
  @chpldoc.nodoc
  var y3 = x3;

  var preAlphabet: string;
  extern var a: string;
  extern var b: string;
  extern var c: real;
  extern const d: real;
  extern const e: real;
  extern const f: int;
  const f2: int;

  @chpldoc.nodoc
  var g: int;
  @chpldoc.nodoc
  var h: real;
  @chpldoc.nodoc
  extern var i: real;
  @chpldoc.nodoc
  extern var j: real;

  //should not warn
  var k: int;
  var l: string;

  //should not warn
  var m: int;
  class MyClass {
    var n: int;
  }
  var o: int;
  //comment between should not interfere.
  var p: int;
  extern var c_var: int(32); //its type is a function call
  extern var c_var2: int(32);

  pragma "always RVF"
  var q: real;
  pragma "always RVF"
  var r: real;
  pragma "always RVF"
  var s: int;
  var t: int;

}