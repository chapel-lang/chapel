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


}