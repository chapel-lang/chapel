use Allocators;

class C1 {}

class C2 { var x: int; }

class C3 { type t = int; var x: t; }

class C4 {
  param v; var x = v;
  proc init(param v) {
    this.v = v;
    this.x = v;
  }
}

var pool = new malloc();


{
  var c = newWithAllocator(pool, unmanaged C1);
  writeln(c.type: string, ": ", c);
  deleteWithAllocator(pool, c);
}
{
  var c = newWithAllocator(pool, unmanaged C1?);
  writeln(c.type: string, ": ", c);
  deleteWithAllocator(pool, c);
}

{
  var c = newWithAllocator(pool, unmanaged C2, 1);
  writeln(c.type: string, ": ", c);
  deleteWithAllocator(pool, c);
}
{
  var c = newWithAllocator(pool, unmanaged C2?, 1);
  writeln(c.type: string, ": ", c);
  deleteWithAllocator(pool, c);
}

{
  var c = newWithAllocator(pool, unmanaged C3, 1);
  writeln(c.type: string, ": ", c);
  deleteWithAllocator(pool, c);
}
{
  var c = newWithAllocator(pool, unmanaged C3?, 1);
  writeln(c.type: string, ": ", c);
  deleteWithAllocator(pool, c);
}
{
  var c = newWithAllocator(pool, unmanaged C3(real), 1.0);
  writeln(c.type: string, ": ", c);
  deleteWithAllocator(pool, c);
}
{
  var c = newWithAllocator(pool, unmanaged C3(real)?, 1.0);
  writeln(c.type: string, ": ", c);
  deleteWithAllocator(pool, c);
}

{
  var c = newWithAllocator(pool, unmanaged C4(4));
  writeln(c.type: string, ": ", c);
  deleteWithAllocator(pool, c);
}
{
  var c = newWithAllocator(pool, unmanaged C4(4)?);
  writeln(c.type: string, ": ", c);
  deleteWithAllocator(pool, c);
}
