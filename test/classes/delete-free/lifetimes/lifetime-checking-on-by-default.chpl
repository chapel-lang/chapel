
proc identity(ref r) ref {
  return r;
}

proc bad() ref {
  var locl:int;
  return identity(locl);
}

bad();
