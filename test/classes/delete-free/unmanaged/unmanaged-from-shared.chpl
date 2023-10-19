
class C {
  var x: int;
}

{
  var c = new shared C();
  writeln((c, c.type:string));

  var u = borrowed.release(b);
  writeln((u, u.type:string));

  u.x = 10;

  writeln((c, c.type:string));
  writeln((u, u.type:string));

  // no delete needed, u is just an unmanaged reference to the borrow
}

{
  var c = new shared C?();
  writeln((c, c.type:string));

  var u = borrowed.release(b);
  writeln((u, u.type:string));

  u!.x = 10;

  writeln((c, c.type:string));
  writeln((u, u.type:string));

  // no delete needed, u is just an unmanaged reference to the borrow
}
