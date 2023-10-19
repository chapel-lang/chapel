
class C {
  var x: int;
}

{
  var c = new C();
  var b = c.borrow();
  writeln((b, b.type:string));

  var u = borrowed.release(b);
  writeln((u, u.type:string));

  u.x = 10;

  writeln((b, b.type:string));
  writeln((u, u.type:string));

  // no delete needed, u is just an unmanaged reference to the borrow
}

{
  var c = new C?();
  var b = c.borrow();
  writeln((b, b.type:string));

  var u = borrowed.release(b);
  writeln((u, u.type:string));

  u!.x = 10;

  writeln((b, b.type:string));
  writeln((u, u.type:string));

  // no delete needed, u is just an unmanaged reference to the borrow
}
