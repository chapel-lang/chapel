proc object.myName: string do return "object";
proc object.ddName(): string { return "object"; }

class A {
  override proc myName { return "A"; }
  override proc ddName() { return myName; }
}

class B:A {
  override proc myName { return "B"; }
  override proc ddName() { return myName; }
}

class C:B {
  override proc myName { return "C"; }
  override proc ddName() { return myName; }
}

class C2:C {
  override proc myName { return "C2"; }
  override proc ddName() { return myName; }
}

class D:C {
  override proc myName { return "D"; }
  override proc ddName() { return myName; }
}

class E:C {
  override proc myName { return "E"; }
  override proc ddName() { return myName; }
}

class F {
  override proc myName { return "F"; }
  override proc ddName() { return myName; }
}

//
// Begin interesting code
//

proc get_cdr(type car, type cdr...?k) type {
  return cdr;
}

proc getSuperType(type t) type {
  proc st(v:t? = nil) type {
    if (t == object) then
      return t;
    else
      return v!.super.type;
  }
  return st();
}

proc nearestMutualParentClass(type t1, type t2) type {
  if t1 == t2 then
    return t1;
  else if isSubtype(t1, t2) then
    return t2;
  else if isSubtype(t2, t1) then
    return t1;
  else
    return nearestMutualParentClass(getSuperType(t1), getSuperType(t2));
}

proc nearestMutualParentClass(type car, type cdr...?k) type where k != 1 {
  type first = nearestMutualParentClass(car, cdr(1));
  return nearestMutualParentClass(first, (...get_cdr((...cdr))));
}

proc main {
  var c: nearestMutualParentClass(borrowed E, borrowed D, borrowed C, borrowed C2, borrowed F) = (new owned E()).borrow();
  var d: nearestMutualParentClass(borrowed E, borrowed D, borrowed C) = (new owned E()).borrow();
  writeln(c.myName);
  writeln(c.ddName());
  writeln(d.myName);
  writeln(d.ddName());
}
