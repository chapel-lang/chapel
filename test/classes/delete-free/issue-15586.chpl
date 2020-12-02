class C { var x: int = 0; }

proc test(type t) {
  var x = new t(128);
  writeln("new made " + x.type:string);
  if isUnmanagedClassType(t) then delete x;
  return;
}
// borrowed
{
  writeln("borrowed C");
  type t = borrowed C;
  test(t);
}
{
  writeln("borrowed C?");
  type t = borrowed C?;
  test(t);
}
// unmanaged
{
  writeln("unmanaged C");
  type t = unmanaged C;
  test(t);
}
{
  writeln("unmanaged C?");
  type t = unmanaged C?;
  test(t);
}
// owned
{
  writeln("owned C");
  type t = owned C;
  test(t);
}
{
  writeln("owned C?");
  type t = owned C?;
  test(t);
}
// shared
{
  writeln("shared C");
  type t = shared C;
  test(t);
}
{
  writeln("shared C?");
  type t = shared C?;
  test(t);
}
// generic management
{
  writeln("C");
  type t = C;
  test(t);
}
{
  writeln("C?");
  type t = C?;
  test(t);
}
