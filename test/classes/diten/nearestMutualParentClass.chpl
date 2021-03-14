class A {
  var name: string = "A";
  override proc ddName() { return name; }
}

class B:A {
  var name: string = "B";
  override proc ddName() { return name; }
}

class C:B {
  var name: string = "C";
  override proc ddName() { return name; }
}

class C2:C {
  var name: string = "C2";
  override proc ddName() { return name; }
}

class D:C {
  var name: string = "D";
  override proc ddName() { return name; }
}

class E:C {
  var name: string = "E";
  override proc ddName() { return name; }
}

class F {
  var name: string = "F";
  override proc ddName() { return name; }
}

proc object.name: string return "object";
proc object.ddName(): string { return "object"; }

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
  var c: nearestMutualParentClass(borrowed E, borrowed D, borrowed C, borrowed C2, borrowed F) = new borrowed E();
  var d: nearestMutualParentClass(borrowed E, borrowed D, borrowed C) = new borrowed E();
  writeln(c.name);
  writeln(c.ddName());
  writeln(d.name);
  writeln(d.ddName());
}
