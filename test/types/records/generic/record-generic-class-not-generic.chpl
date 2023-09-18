class GC {
  type t;
}

record H {
  var myfield: owned GC;
}

{
  writeln(isGeneric(H));
}
