// This test is to ensure that you can call auto-included Math symbols with the
// Math module name as a prefix.  We expect this support to be stable, even when
// we determine a better name for AutoMath
{
  use Math;

  writeln(Math.cbrt(27));
}
{
  import Math;

  writeln(Math.cbrt(27));
}
