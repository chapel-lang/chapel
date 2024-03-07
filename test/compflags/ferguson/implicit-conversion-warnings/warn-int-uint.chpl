// check that with --warn-int-uint (or possibly --warn-unstable),
// the compiler prints a warning for each int->uint implicit conversion

// To make error line numbers nicer, first call fns to check returns
{
  ret_v();
  ret_negone();
  ret_one();
  ret_v_cast();
  ret_negone_cast();
}

proc ret_v() : uint {
  var myInt: int;
  return myInt; // warn
}
proc ret_negone() : uint {
  var myInt: int;
  return -1; // warn
}
proc ret_one() : uint {
  var myInt: int;
  return 1;
}
proc ret_v_cast() : uint {
  var myInt: int;
  return myInt:uint;
}
proc ret_negone_cast() : uint {
  var myInt: int;
  return (-1):uint;
}

proc f(arg: uint) { }

// checking regular variables
{
  var x: int;
  f(x); // warn
  var y: uint = x; // warn
  y = x; // warn
}

// checking global variables also warn
var g_x: int;
f(g_x); // warn
var g_y: uint = g_x; // warn
g_y = g_x; // warn

// checking negative params
{
  param negone: int = -1;
  f(negone); // warn
  var y: uint = negone; // warn
  y = negone; // warn
}

// checking positive params do not warn
{
  param one: int = 1;
  f(one);
  var y: uint = one;
  y = one;
}

// checking regular variables do not warn with a cast
{
  var x: int;
  f(x:uint);
  var y: uint = x:uint;
  y = x:uint;
}

// checking negative params do not warn with a cast
{
  param negone: int = -1;
  f(negone:uint);
  var y: uint = negone:uint;
  y = negone:uint;
}
