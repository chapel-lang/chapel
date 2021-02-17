/* Tests for modulus div-by-zero error */

config param testParamCases = 0;
config const testExecCases = 0;

var num = 10,
    unum = 10: uint,
    denom = 0,
    udenom = 0: uint;

record R {
  var x = 10;
}

/* Define this so that %= is automatically defined */
operator R.%(lhs: R, rhs: R) return new R(lhs.x % rhs.x);

var rec1 = new R(1),
    rec0 = new R(0);

select testParamCases {
  //operator R.%(param a: int(?w), param b: int(w)) param
  when 1 do
    writeln(10 % 0);

  //operator R.%(param a: uint(?w), param b: uint(w)) param
  when 2 do
    writeln(10 % 0);

  //operator R.%(a: uint(64), param b: uint(64))
  when 3 do
    writeln(unum % 0);
}

select testExecCases {
  //operator R.%(a: int(?w), b: int(w))
  when 0 do
    writeln(num % denom);

  //operator R.%(a: uint(?w), b: uint(w))
  when 1 do
    writeln(unum % udenom);

  //operator R.%=(ref lhs:int(?w), rhs:int(w))
  when 2 do
    num %= denom;

  //operator R.%=(ref lhs:uint(?w), rhs:uint(w))
  when 3 do
    unum %= udenom;

  //operator R.%=(ref lhs, rhs)
  when 4 do
    rec1 %= rec0;

  //operator R.%(param a: uint(64), b: uint(64))
  when 5 do
    writeln(10 % udenom);

  otherwise do
    halt('No tests run');
}
