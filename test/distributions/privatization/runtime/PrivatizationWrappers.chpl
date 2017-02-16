// Helper routines for inserting and getting privatized objects directly
// to/from the runtime interfaces.

class C {
  var i: int;
}

proc insertPrivatized(o: object, pid: int) {
  __primitive("chpl_newPrivatizedClass", o, pid);
}

proc getPrivatized(pid:int): C {
  return __primitive("chpl_getPrivatizedClass", nil:C, pid);
}

proc clearPrivatized(pid:int) {
  extern proc chpl_clearPrivatizedClass(pid:int);
  chpl_clearPrivatizedClass(pid);
}
